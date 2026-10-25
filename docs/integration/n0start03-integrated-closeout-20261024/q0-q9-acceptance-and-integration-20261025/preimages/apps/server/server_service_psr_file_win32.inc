/* Same LAB runtime owner; no creation, external receiver or authority. */
static const BYTE psrd_attempt[33]="0b1de869e11493b7cb08fb329578f6ce";
static const WCHAR psrd_leaf[]=L"papacc-psr-0b1de869e11493b7cb08fb329578f6ce.diag";
static DWORD psrd_fail(DWORD e)
{
 PS_DIAGNOSTIC *d=&native.runtime.diagnostic;if(!d->error)d->error=e?e:ERROR_GEN_FAILURE;
 if(d->parent||d->file.handle||d->file.retained)native.runtime.retained=1U;
 return d->error;
}
static int psrd_sid(BYTE out[SECURITY_MAX_SID_SIZE],DWORD first,const DWORD *tail,unsigned count)
{
 SID_IDENTIFIER_AUTHORITY authority=SECURITY_NT_AUTHORITY;unsigned i;
 if(count>7U||!InitializeSid((PSID)out,&authority,(BYTE)(count+1U)))return 0;
 *GetSidSubAuthority(out,0)=first;for(i=0;i<count;i++)*GetSidSubAuthority(out,i+1U)=tail[i];return IsValidSid(out)!=FALSE;
}
static int psrd_descriptor(PSECURITY_DESCRIPTOR sd)
{
 PSID owner=NULL;PACL acl=NULL;BOOL def,present;SECURITY_DESCRIPTOR_CONTROL control;DWORD revision,i,seen=0;
 BYTE op[SECURITY_MAX_SID_SIZE],system[SECURITY_MAX_SID_SIZE],admin[SECURITY_MAX_SID_SIZE],service[SECURITY_MAX_SID_SIZE];DWORD n;
 const DWORD owner_tail[]={785699550UL,1440458426UL,1682014700UL,1108UL};
 const DWORD service_tail[]={3567404667UL,26456103UL,3018599297UL,139793421UL,3962028451UL};
 if(!IsValidSecurityDescriptor(sd)||!GetSecurityDescriptorControl(sd,&control,&revision)||!(control&SE_DACL_PROTECTED)||
 !GetSecurityDescriptorOwner(sd,&owner,&def)||!owner||!GetSecurityDescriptorDacl(sd,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl)||acl->AceCount!=4U)return 0;
 n=sizeof(system);if(!CreateWellKnownSid(WinLocalSystemSid,NULL,system,&n))return 0;n=sizeof(admin);if(!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admin,&n))return 0;
 if(!psrd_sid(op,21,owner_tail,4)||!psrd_sid(service,80,service_tail,5)||!EqualSid(owner,op))return 0;
 for(i=0;i<acl->AceCount;i++){
  ACCESS_ALLOWED_ACE *ace=NULL;PSID sid;DWORD bit=0,mask=FILE_ALL_ACCESS;
  if(!GetAce(acl,i,(void **)&ace)||ace->Header.AceType!=ACCESS_ALLOWED_ACE_TYPE||ace->Header.AceFlags||ace->Header.AceSize<sizeof(ACCESS_ALLOWED_ACE))return 0;
  sid=(PSID)&ace->SidStart;if(!psr_inside_sid((BYTE *)ace,ace->Header.AceSize,sid))return 0;
  if(EqualSid(sid,op))bit=1U;else if(EqualSid(sid,system))bit=2U;else if(EqualSid(sid,admin))bit=4U;else if(EqualSid(sid,service)){bit=8U;mask=FILE_READ_DATA|FILE_WRITE_DATA|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE;}
  if(!bit||(seen&bit)||ace->Mask!=mask)return 0;seen|=bit;
 }
 return seen==15U;
}
static DWORD psrd_security(HANDLE file,int parent)
{
 PS_DIAGNOSTIC *d=&native.runtime.diagnostic;DWORD used=0;PSID approved=NULL;BYTE op[SECURITY_MAX_SID_SIZE];const DWORD tail[]={785699550UL,1440458426UL,1682014700UL,1108UL};SECURITY_DESCRIPTOR_CONTROL control;DWORD revision;
 if(!GetKernelObjectSecurity(file,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,d->sd.bytes,sizeof(d->sd.bytes),&used))return psrd_fail(GetLastError());
 if(!used||used>sizeof(d->sd.bytes)||!IsValidSecurityDescriptor(d->sd.bytes)||!GetSecurityDescriptorControl(d->sd.bytes,&control,&revision)||!(control&SE_DACL_PROTECTED))return psrd_fail(ERROR_INVALID_SECURITY_DESCR);
 if(parent){if(!psrd_sid(op,21,tail,4))return psrd_fail(ERROR_INVALID_SID);approved=op;if(!scm_descriptor(d->sd.bytes,(BYTE *)approved))return psrd_fail(ERROR_ACCESS_DENIED);}
 else if(!psrd_descriptor(d->sd.bytes))return psrd_fail(ERROR_ACCESS_DENIED);
 return ERROR_SUCCESS;
}
static DWORD psrd_bind(void)
{
 PS_DIAGNOSTIC *d=&native.runtime.diagnostic;BY_HANDLE_FILE_INFORMATION id;DWORD n,m;size_t a,b;DWORD e;
 if(d->began||d->parent||d->file.handle)return ERROR_INVALID_STATE;d->began=1U;
 if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested())return psrd_fail(ERROR_CANCELLED);
 d->parent=CreateFileW(psr_parent,FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(d->parent==INVALID_HANDLE_VALUE){d->parent=NULL;return psrd_fail(GetLastError());}
 if(!GetFileInformationByHandle(d->parent,&id))return psrd_fail(GetLastError());
 if(id.dwVolumeSerialNumber!=417614100UL||id.nFileIndexHigh!=1966080UL||id.nFileIndexLow!=79426UL||!(id.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(id.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return psrd_fail(ERROR_INVALID_DATA);
 if((e=psrd_security(d->parent,1)))return e;
 n=GetFinalPathNameByHandleW(d->parent,d->parent_path,512,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);if(!n||n>=512)return psrd_fail(n?ERROR_BUFFER_OVERFLOW:GetLastError());
 a=wcslen(psr_parent);b=wcslen(psrd_leaf);if(a+b+2U>600U)return psrd_fail(ERROR_BUFFER_OVERFLOW);
 memcpy(d->file_path,psr_parent,a*sizeof(WCHAR));d->file_path[a]=L'\\';memcpy(d->file_path+a+1U,psrd_leaf,(b+1U)*sizeof(WCHAR));
 d->file.consumed=1U;d->file.handle=CreateFileW(d->file_path,FILE_READ_DATA|FILE_WRITE_DATA|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,0,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(d->file.handle==INVALID_HANDLE_VALUE){d->file.handle=NULL;return psrd_fail(GetLastError());}
 if(!GetFileInformationByHandle(d->file.handle,&d->file.identity))return psrd_fail(GetLastError());
 if(d->file.identity.dwVolumeSerialNumber!=id.dwVolumeSerialNumber)return psrd_fail(ERROR_INVALID_DATA);
 m=GetFinalPathNameByHandleW(d->file.handle,d->file_path,600,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
 if(!m||m>=600)return psrd_fail(m?ERROR_BUFFER_OVERFLOW:GetLastError());
 if(m!=n+1U+b||wcsncmp(d->file_path,d->parent_path,n)||d->file_path[n]!=L'\\'||wcscmp(d->file_path+n+1U,psrd_leaf))return psrd_fail(ERROR_INVALID_NAME);
 if((e=psrd_security(d->file.handle,0)))return e;
 e=psrf_read_arm(&d->file,&d->file.identity,psrd_attempt);if(e)return psrd_fail(e);
 d->bound=1U;return ERROR_SUCCESS;
}
static void psrd_emit(const char *text,unsigned kind)
{
 DWORD saved=GetLastError(),e;PS_DIAGNOSTIC *d=&native.runtime.diagnostic;int n;
 if(!kind||d->emitted){SetLastError(saved);return;}d->emitted=1U;
 if(!d->bound||d->error){(void)psrd_fail(d->error?d->error:ERROR_INVALID_STATE);SetLastError(saved);return;}
 n=_snprintf_s((char *)d->file.frame.text,PSRF_TEXT,_TRUNCATE,"attempt=0b1de869e11493b7cb08fb329578f6ce service=PapinhoAcceleratorLab20261023 expected_config_sha=%s stage=psr_prepare\n%s",psr_config_sha,text);
 if(n<=0){(void)psrd_fail(ERROR_BUFFER_OVERFLOW);SetLastError(saved);return;}
 e=psrf_finish(&d->file,psrd_attempt,d->file.frame.text,(DWORD)n,kind,&host.stop);
 if(e){(void)psrd_fail(e);SetLastError(saved);return;}
 if(d->parent){if(!CloseHandle(d->parent)){(void)psrd_fail(GetLastError());SetLastError(saved);return;}d->parent=NULL;}
 d->receipt=kind;SetLastError(saved);
}
static int psrd_quiescent(void)
{return !native.runtime.diagnostic.parent&&!native.runtime.diagnostic.file.handle&&!native.runtime.diagnostic.file.retained;}

static DWORD psrd_prepare_complete(void)
{
 PSRT_PREPARED();if(native.runtime.diagnostic.error)return native.runtime.diagnostic.error;
 if(native.runtime.diagnostic.receipt!=2U||!psrd_quiescent())return ERROR_WRITE_FAULT;
 native.runtime.ready=1U;return ERROR_SUCCESS;
}
