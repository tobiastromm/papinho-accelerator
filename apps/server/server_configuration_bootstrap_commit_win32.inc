/* Ephemeral proof in existing packet scratch, never a persistent authority.
 * Captured from the actual process token by the native worker. */
typedef struct BOOTSTRAP_AUTH_PROOF {
 DWORD version,scope,sid_bytes;
 BYTE sid[SECURITY_MAX_SID_SIZE];TOKEN_STATISTICS statistics;
 BYTE root[32],candidate[32];DWORD volume,high,low;
} BOOTSTRAP_AUTH_PROOF;
#define BOOTSTRAP_AUTH_OFFSET 256U
static int bootstrap_auth_native(PAPACC_CONFIG_PENDING *p,BOOTSTRAP_AUTH_PROOF *proof,int capture)
{
 HANDLE token=NULL;BYTE storage[128];TOKEN_USER *user=(TOKEN_USER *)storage;
 TOKEN_STATISTICS statistics;DWORD n;int ok=0;
 if(!bootstrap_admin(p)||p->close_error)return 0;
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return 0;
 if(!GetTokenInformation(token,TokenUser,storage,sizeof(storage),&n)||
    !IsValidSid(user->User.Sid)||GetLengthSid(user->User.Sid)>SECURITY_MAX_SID_SIZE||
    !GetTokenInformation(token,TokenStatistics,&statistics,sizeof(statistics),&n))goto done;
 if(capture){
  memset(proof,0,sizeof(*proof));proof->version=1U;proof->scope=1U;
  proof->sid_bytes=GetLengthSid(user->User.Sid);
  if(!CopySid(sizeof(proof->sid),proof->sid,user->User.Sid))goto done;
  proof->statistics=statistics;memcpy(proof->root,p->previous+128U,32U);
  proof->volume=p->bootstrap_volume;proof->high=p->bootstrap_high;proof->low=p->bootstrap_low;
  ok=1;
 }else ok=proof->version==1U&&proof->sid_bytes>0U&&proof->sid_bytes<=sizeof(proof->sid)&&
  IsValidSid(proof->sid)&&proof->sid_bytes==GetLengthSid(proof->sid)&&EqualSid(proof->sid,user->User.Sid)&&
  !memcmp(&proof->statistics.TokenId,&statistics.TokenId,sizeof(LUID))&&
  !memcmp(&proof->statistics.AuthenticationId,&statistics.AuthenticationId,sizeof(LUID))&&
  !memcmp(&proof->statistics.ModifiedId,&statistics.ModifiedId,sizeof(LUID))&&
  !memcmp(proof->root,p->previous+128U,32U)&&proof->volume==p->bootstrap_volume&&
  proof->high==p->bootstrap_high&&proof->low==p->bootstrap_low;
done:
 if(!CloseHandle(token)){p->bootstrap_token=token;p->close_error=GetLastError();return 0;}
 return ok;
}
static int bootstrap_auth_capture(PAPACC_CONFIG_PENDING *p)
{
 BOOTSTRAP_AUTH_PROOF proof;
 if(!bootstrap_auth_native(p,&proof,1))return 0;
 memcpy(p->previous+BOOTSTRAP_AUTH_OFFSET,&proof,sizeof(proof));return 1;
}
typedef HANDLE (NTAPI *BOOTSTRAP_GET_TRANSACTION)(void);
typedef BOOLEAN (NTAPI *BOOTSTRAP_SET_TRANSACTION)(HANDLE);
static void bootstrap_commit_worker(PAPACC_CONFIG_PENDING *p)
{
 BOOTSTRAP_AUTH_PROOF proof;BYTE hash[32];HMODULE module;FARPROC address;
 BOOTSTRAP_NT_CREATE create;BOOTSTRAP_GET_TRANSACTION gettx;BOOTSTRAP_SET_TRANSACTION settx;
 UNICODE_STRING name;OBJECT_ATTRIBUTES attr;IO_STATUS_BLOCK io;WCHAR leaf[65];
 BY_HANDLE_FILE_INFORMATION identity,after;NTSTATUS status;DWORD n;unsigned i;
 int diagnostic=0,verified=0,context_set=0;
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
 diagnostic=p->checkpoint_only==91;
#endif
 memcpy(&proof,p->previous+BOOTSTRAP_AUTH_OFFSET,sizeof(proof));
 if(!diagnostic&&(!bootstrap_auth_native(p,&proof,0)||proof.scope!=2U))return;
 if(InterlockedCompareExchange(&p->stop,0,0)||p->close_error)return;
 if(!bootstrap_identity(p->bootstrap_parent,&p->bootstrap_parent_volume,&p->bootstrap_parent_high,&p->bootstrap_parent_low,0)||
    !bootstrap_identity(p->bootstrap_container,&p->bootstrap_volume,&p->bootstrap_high,&p->bootstrap_low,0)){
  p->result=PAPACC_CAS_CONFLICT;return;
 }
 if(!store03_hash(p->next,p->next_bytes,hash)||memcmp(hash,proof.candidate,32U)){
  p->result=PAPACC_CAS_CONFLICT;return;
 }
 module=GetModuleHandleA("ntdll.dll");if(!module)return;
 address=GetProcAddress(module,"NtCreateFile");if(!address)return;memcpy(&create,&address,sizeof(create));
 address=GetProcAddress(module,"RtlGetCurrentTransaction");if(!address)return;memcpy(&gettx,&address,sizeof(gettx));
 address=GetProcAddress(module,"RtlSetCurrentTransaction");if(!address)return;memcpy(&settx,&address,sizeof(settx));
 if(gettx()!=NULL)return;
 p->transaction=CreateTransaction(NULL,NULL,0,0,0,0,NULL);
 if(p->transaction==INVALID_HANDLE_VALUE){p->transaction=NULL;p->native_error=GetLastError();return;}
 if(!settx(p->transaction))goto uncertain;context_set=1;
 for(i=0;p->path[i];i++)leaf[i]=(WCHAR)(unsigned char)p->path[i];leaf[i]=0;
 name.Buffer=leaf;name.Length=(USHORT)(i*2U);name.MaximumLength=name.Length;
 memset(&attr,0,sizeof(attr));memset(&io,0,sizeof(io));attr.Length=sizeof(attr);
 attr.RootDirectory=p->bootstrap_container;attr.ObjectName=&name;attr.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 /* Relative FILE_CREATE joins THIS worker's existing transaction; never reopens a path. */
 status=create(&p->file,GENERIC_READ|GENERIC_WRITE|SYNCHRONIZE,&attr,&io,NULL,FILE_ATTRIBUTE_NORMAL,
  0U,2U,0x00200060U,NULL,0U);
 if(!settx(NULL))goto uncertain;context_set=0;
 if(status<0){p->file=INVALID_HANDLE_VALUE;p->native_error=(DWORD)status;
  if(status==(NTSTATUS)0xc0000035L)p->result=PAPACC_CAS_CONFLICT;goto rollback;}
 if(io.Information!=2U||!GetFileInformationByHandle(p->file,&identity)||
    (identity.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))goto uncertain;
 /* Snapshot metadata from THIS file handle; it is not used to reopen IO. */
 p->expected_volume=identity.dwVolumeSerialNumber;p->expected_index_high=identity.nFileIndexHigh;p->expected_index_low=identity.nFileIndexLow;
 n=GetFinalPathNameByHandleA(p->file,(char *)p->previous+1536U,MAX_PATH+4U,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
 if(n<7U||n>=MAX_PATH+4U||memcmp(p->previous+1536U,"\\\\?\\",4U)||
    p->previous[1541U]!=':'||p->previous[1542U]!='\\')goto uncertain;
 memmove(p->previous+1536U,p->previous+1540U,(size_t)n-3U);
 verified=1;
 if(InterlockedCompareExchange(&p->stop,0,0))goto rollback;
 if(!WriteFile(p->file,p->next,p->next_bytes,&n,NULL)||n!=p->next_bytes||!FlushFileBuffers(p->file))goto uncertain;
 if(SetFilePointer(p->file,0,NULL,FILE_BEGIN)==INVALID_SET_FILE_POINTER||
    pending_compare_file(p,p->next,p->next_bytes)!=1)goto uncertain;
 if(!GetFileInformationByHandle(p->file,&after)||identity.dwVolumeSerialNumber!=after.dwVolumeSerialNumber||
    identity.nFileIndexHigh!=after.nFileIndexHigh||identity.nFileIndexLow!=after.nFileIndexLow||
    !bootstrap_identity(p->bootstrap_container,&p->bootstrap_volume,&p->bootstrap_high,&p->bootstrap_low,0))goto uncertain;
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 if(p->test_entered){if(!SetEvent(p->test_entered)||WaitForSingleObject(p->test_release,INFINITE)!=WAIT_OBJECT_0)goto uncertain;}
 if(p->test_unknown)goto uncertain;
#endif
 if(InterlockedCompareExchange(&p->stop,0,0))goto rollback;
 /* Revalidation closes the prepare-to-effect authority window; no historical boolean suffices. */
 if(!diagnostic&&!bootstrap_auth_native(p,&proof,0))goto rollback;
 if(!CloseHandle(p->file)){p->close_error=GetLastError();goto uncertain;}p->file=INVALID_HANDLE_VALUE;
 if(!CommitTransaction(p->transaction))goto uncertain;
 p->committed=1;p->result=PAPACC_CAS_SUCCESS;
 if(!CloseHandle(p->transaction)){p->close_error=GetLastError();return;}p->transaction=NULL;return;
rollback:
 if(context_set){if(!settx(NULL))goto uncertain;context_set=0;}
 if(p->file!=INVALID_HANDLE_VALUE){if(!CloseHandle(p->file)){p->close_error=GetLastError();goto uncertain;}p->file=INVALID_HANDLE_VALUE;}
 if(!RollbackTransaction(p->transaction))goto uncertain;
 if(!CloseHandle(p->transaction)){p->close_error=GetLastError();goto uncertain;}p->transaction=NULL;
 if(p->result!=PAPACC_CAS_CONFLICT)p->result=PAPACC_COMMIT_FAILED;
 (void)verified;return;
uncertain:
 if(context_set)(void)settx(NULL); /* best effort context detach is not rollback proof */
 if(!p->native_error)p->native_error=GetLastError();p->result=PAPACC_CAS_UNKNOWN;
}
