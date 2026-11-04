/* Read-only diagnostic in the existing launcher root/packet. No staging
 * preparation, mutation, approval generation, AccessCheck or policy relaxation. */
/* Sum of named automatic storage across the synchronous call chain, including
 * mutually exclusive frames conservatively. This is not a native stack/heap
 * ceiling: ABI/compiler/CRT/provider/kernel costs remain opaque. */
#define CHAIN_KNOWN_AUTOMATIC_BYTES (64U*sizeof(WCHAR)+2U*MAX_PATH*sizeof(WCHAR)+\
 sizeof(TOKEN_STATISTICS)*2U+sizeof(BY_HANDLE_FILE_INFORMATION)+sizeof(OBJECT_ATTRIBUTES)+\
 sizeof(UNICODE_STRING)+sizeof(PACC_STORAGE_SHA256)+2U*SECURITY_MAX_SID_SIZE+32U+\
 32U*sizeof(DWORD)+32U*sizeof(void *)+8U*sizeof(size_t)+8U*sizeof(int))
static DWORD chain_error(DWORD e)
{if(!e)e=ERROR_GEN_FAILURE;if(!staging_act()->first)staging_act()->first=e;return e;}
static int chain_stop(void)
{return papacc_server_console_win32_stop_requested()||root.configuration_commit.pending_commit.stop;}
static int chain_sd(unsigned index,FILE *out,int first)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();PAPACC_STAGING_OBJECT *o=&s->objects[index];
 DWORD bytes,rev,reason,ace,mask,i;SECURITY_DESCRIPTOR_CONTROL control;BYTE hash[32];
 PSID owner;PACL acl;BOOL def,present;int valid=0;
 if(s->scratch)return 0;
 {DWORD e=GetSecurityInfo(o->handle,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&s->scratch);
 if(e){chain_error(e);return 0;}}
 if(!s->scratch||!IsValidSecurityDescriptor(s->scratch)){chain_error(ERROR_INVALID_SECURITY_DESCR);goto done;}
 bytes=GetSecurityDescriptorLength(s->scratch);
 if(!bytes||bytes>65536U||!GetSecurityDescriptorControl(s->scratch,&control,&rev)||
 !GetSecurityDescriptorOwner(s->scratch,&owner,&def)||!owner||!IsValidSid(owner)||
 !GetSecurityDescriptorDacl(s->scratch,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl)){
  chain_error(ERROR_INVALID_SECURITY_DESCR);goto done;
 }
 if(!pacc_storage_hash_callback(NULL,(BYTE *)s->scratch,bytes,hash)){chain_error(ERROR_CRC);goto done;}
 if(!first&&memcmp(hash,o->acl,32)){chain_error(ERROR_REVISION_MISMATCH);goto done;}
 if(first)memcpy(o->acl,hash,32);
 if(first){
  /* SID text lives in the existing packet until checked LocalFree. */
  PAPACC_PERSONAL_NATIVE_CONTEXT *n=&root.configuration_commit.pending_commit.personal_native;
  if(n->sid_text||!ConvertSidToStringSidA(owner,&n->sid_text)){chain_error(ERROR_INVALID_SID);goto done;}
  fprintf(out,"CHAIN_SD index=%u owner=%s control=0x%04x bytes=%lu sha256=",index,n->sid_text,(unsigned)control,(unsigned long)bytes);
  for(i=0;i<32U;i++)fprintf(out,"%02x",(unsigned)hash[i]);fputc('\n',out);
  if(LocalFree(n->sid_text)){s->phase=8U;chain_error(ERROR_INVALID_HANDLE);goto done;}n->sid_text=NULL;
  reason=staging_descriptor_classify(s->scratch,1,&ace,&mask);
  fprintf(out,"CHAIN_STRUCTURAL index=%u reason=%lu ace=%lu mask=0x%08lx; NOT_ACCESS_CHECK_NOT_AUTHORITY\n",index,(unsigned long)reason,(unsigned long)ace,(unsigned long)mask);
  for(i=0;i<acl->AceCount;i++){
   PACE_HEADER h;ACCESS_ALLOWED_ACE *a;PSID sid;
   if(chain_stop()){chain_error(ERROR_CANCELLED);goto done;}
   if(!GetAce(acl,i,(void **)&h)||h->AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U||
    (h->AceType!=ACCESS_ALLOWED_ACE_TYPE&&h->AceType!=ACCESS_DENIED_ACE_TYPE)){chain_error(ERROR_NOT_SUPPORTED);goto done;}
   a=(ACCESS_ALLOWED_ACE *)h;sid=&a->SidStart;
   if(!IsValidSid(sid)||GetLengthSid(sid)>(DWORD)(h->AceSize-FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart))||
    !ConvertSidToStringSidA(sid,&n->sid_text)){chain_error(ERROR_INVALID_SID);goto done;}
   fprintf(out,"CHAIN_ACE index=%u ace=%lu type=%u flags=0x%02x mask=0x%08lx sid=%s\n",index,(unsigned long)i,(unsigned)h->AceType,(unsigned)h->AceFlags,(unsigned long)a->Mask,n->sid_text);
   if(LocalFree(n->sid_text)){s->phase=8U;chain_error(ERROR_INVALID_HANDLE);goto done;}n->sid_text=NULL;
  }
 }
 valid=1;
done:
 if(s->scratch){if(LocalFree(s->scratch)){s->phase=8U;chain_error(ERROR_INVALID_HANDLE);return 0;}s->scratch=NULL;}
 return valid;
}
static int chain_object(unsigned index,FILE *out,int first)
{
 PAPACC_STAGING_OBJECT *o=&staging_act()->objects[index];BY_HANDLE_FILE_INFORMATION f;
 if(chain_stop()){chain_error(ERROR_CANCELLED);return 0;}
 if(!GetFileInformationByHandle(o->handle,&f)){chain_error(GetLastError());return 0;}
 if(!(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)){
  chain_error(ERROR_REPARSE_TAG_INVALID);return 0;
 }
 if(!first&&(f.dwVolumeSerialNumber!=o->identity.dwVolumeSerialNumber||f.nFileIndexHigh!=o->identity.nFileIndexHigh||
  f.nFileIndexLow!=o->identity.nFileIndexLow||f.dwFileAttributes!=o->identity.dwFileAttributes)){
  chain_error(ERROR_REVISION_MISMATCH);return 0;
 }
 if(first){o->identity=f;fprintf(out,"CHAIN_OBJECT index=%u volume=%lu file=%lu:%lu attributes=0x%08lx\n",index,(unsigned long)f.dwVolumeSerialNumber,(unsigned long)f.nFileIndexHigh,(unsigned long)f.nFileIndexLow,(unsigned long)f.dwFileAttributes);}
 return chain_sd(index,out,first);
}
/* Return 1 acquired, 0 absent, -1 failed. IOSB/handle/name backing survive
 * unexpected pending status in the SAME existing packet. No second open. */
static int chain_open(unsigned index,HANDLE parent,const WCHAR *name)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();STAGING_NT_CREATE fn;HMODULE module;FARPROC proc;
 UNICODE_STRING u;OBJECT_ATTRIBUTES a;NTSTATUS status;size_t len=wcslen(name);
 if(chain_stop()){chain_error(ERROR_CANCELLED);return -1;}
 if(index>=4U||s->objects[index].handle||!len||len>=MAX_PATH){chain_error(ERROR_INVALID_STATE);return -1;}
 module=GetModuleHandleW(L"ntdll.dll");proc=module?GetProcAddress(module,"NtCreateFile"):NULL;
 if(!proc){chain_error(ERROR_NOT_SUPPORTED);return -1;}memcpy(&fn,&proc,sizeof(fn));
 /* source[] is packet-owned name backing. */
 memcpy(s->source,name,(len+1U)*sizeof(WCHAR));u.Buffer=s->source;u.Length=(USHORT)(len*sizeof(WCHAR));u.MaximumLength=u.Length;
 ZeroMemory(&a,sizeof(a));a.Length=sizeof(a);a.RootDirectory=parent;a.ObjectName=&u;a.Attributes=OBJ_CASE_INSENSITIVE|0x1000U;
 ZeroMemory(&s->resources.volume_io,sizeof(s->resources.volume_io));
 status=fn(&s->objects[index].handle,READ_CONTROL|FILE_READ_ATTRIBUTES|SYNCHRONIZE,&a,&s->resources.volume_io,NULL,
  FILE_ATTRIBUTE_DIRECTORY,FILE_SHARE_READ,1U,0x00200021U,NULL,0);
 if(status==0&&s->resources.volume_io.Status==0&&s->resources.volume_io.Information==1U&&s->objects[index].handle)return 1;
 if(status>0||s->resources.volume_io.Status==(NTSTATUS)0x103L){s->phase=8U;chain_error(ERROR_IO_PENDING);return -1;}
 if(status!=0&&s->objects[index].handle){s->phase=8U;chain_error((DWORD)status);return -1;}
 if(!s->objects[index].handle&&(status==(NTSTATUS)0xc0000034L||status==(NTSTATUS)0xc000003aL))return 0;
 chain_error(status?(DWORD)status:ERROR_INVALID_DATA);return -1;
}
static int chain_token(void)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&root.configuration_commit.pending_commit.personal_native;DWORD count;
 TOKEN_USER *user=(TOKEN_USER *)staging_act()->block;TOKEN_STATISTICS now;
 if(!GetTokenInformation(n->check_token,TokenUser,user,sizeof(staging_act()->block),&count)||
  !IsValidSid(user->User.Sid)||GetLengthSid(user->User.Sid)>sizeof(n->sid)){chain_error(ERROR_INVALID_SID);return 0;}
 if(n->sid_bytes){if(!EqualSid(user->User.Sid,n->sid)){chain_error(ERROR_ACCESS_DENIED);return 0;}}
 else {n->sid_bytes=GetLengthSid(user->User.Sid);memcpy(n->sid,user->User.Sid,n->sid_bytes);}
 if(!GetTokenInformation(n->check_token,TokenStatistics,&now,sizeof(now),&count)){chain_error(GetLastError());return 0;}
 if(n->token.TokenId.LowPart||n->token.TokenId.HighPart){
  if(memcmp(&now.TokenId,&n->token.TokenId,sizeof(LUID))||memcmp(&now.ModifiedId,&n->token.ModifiedId,sizeof(LUID))){chain_error(ERROR_REVISION_MISMATCH);return 0;}
 }else n->token=now;
 return 1;
}
int papacc_server_launcher_chain_metadata(FILE *out,FILE *err)
{
 PAPACC_CONFIGURATION_COMMIT *c=&root.configuration_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&c->pending_commit.personal_native;
 PAPACC_QUALIFICATION_STAGING *s=&n->staging;WCHAR host[64],device[MAX_PATH],again[MAX_PATH];DWORD size=64U;
 unsigned i,missing=4U;int acquired,clean=1,result=2;PSID expected=NULL;TOKEN_STATISTICS effective;DWORD token_bytes;
 static const WCHAR *const relative[4]={NULL,L"Program Files",L"Papinho",L"Accelerator"};
 if(!out||!err||InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,0)!=0)return 2;
 if(next_invocation==(PAPACC_U64)-1){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return 2;}
 ZeroMemory(&root,sizeof(root));root.owner_thread=GetCurrentThreadId();root.invocation=++next_invocation;c->writer=root.owner_thread;
 s->self=s;s->thread=root.owner_thread;s->invocation=root.invocation;s->phase=1U;
 root.reserved_bytes=PAPACC_C8_KNOWN_MANAGED_BYTES+papacc_server_service_storage_bytes()+sizeof(LONG)+
  CHAIN_KNOWN_AUTOMATIC_BYTES;
 if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT){chain_error(ERROR_NOT_ENOUGH_QUOTA);goto done;}
 if(papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK||chain_stop()){chain_error(ERROR_CANCELLED);goto done;}
 if(!GetComputerNameW(host,&size)||wcscmp(host,L"TCORP-TOBIAS")){chain_error(ERROR_ACCESS_DENIED);goto done;}
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&n->unexpected_thread)||GetLastError()!=ERROR_NO_TOKEN){chain_error(ERROR_ACCESS_DENIED);goto done;}
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&n->check_token)||!chain_token()){chain_error(ERROR_ACCESS_DENIED);goto done;}
 /* Both temporary SIDs use existing packet custody; no standalone authority. */
 if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&expected)){chain_error(ERROR_INVALID_SID);goto done;}
 s->trusted_installer=expected;expected=NULL;
 if(!EqualSid(n->sid,s->trusted_installer)){chain_error(ERROR_ACCESS_DENIED);goto done;}
 if(LocalFree(s->trusted_installer)){s->phase=8U;chain_error(ERROR_INVALID_HANDLE);goto done;}s->trusted_installer=NULL;
 if(!ConvertStringSidToSidW(L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464",&s->trusted_installer)){chain_error(ERROR_INVALID_SID);goto done;}
 if(!ConvertSidToStringSidA(n->sid,&n->sid_text)){chain_error(ERROR_INVALID_SID);goto done;}
 fprintf(out,"CHAIN_CONTROLLER sid=%s token=%lu:%lu modified=%lu:%lu; OBSERVATION_NOT_AUTHORIZATION\n",n->sid_text,
  (unsigned long)n->token.TokenId.HighPart,(unsigned long)n->token.TokenId.LowPart,(unsigned long)n->token.ModifiedId.HighPart,(unsigned long)n->token.ModifiedId.LowPart);
 if(LocalFree(n->sid_text)){s->phase=8U;chain_error(ERROR_INVALID_HANDLE);goto done;}n->sid_text=NULL;
 if(papacc_configuration_personal_layout_binary(c)!=PAPACC_RESULT_OK||wcscmp(n->install_paths.binary,L"C:\\Program Files\\Papinho\\Accelerator")){chain_error(ERROR_INVALID_NAME);goto done;}
 if(!QueryDosDeviceW(L"C:",device,MAX_PATH)||wcsncmp(device,L"\\Device\\",8U)||wcslen(device)+1U>=MAX_PATH){chain_error(ERROR_INVALID_NAME);goto done;}
 wcscpy_s(s->program_files,MAX_PATH,device);wcscat_s(device,MAX_PATH,L"\\");
 fprintf(out,"CHAIN_BEGIN host=TCORP-TOBIAS pid=%lu uptime=%llu boot_identity=UNKNOWN; RIGHTS=0x00120080 SHARE=READ OPEN_EXISTING; AUTHORITY=DENY\n",(unsigned long)GetCurrentProcessId(),(unsigned long long)GetTickCount64());
 for(i=0;i<4U;i++){
  if(ferror(out)){chain_error(ERROR_WRITE_FAULT);goto done;}
  acquired=chain_open(i,i?s->objects[i-1U].handle:NULL,i?relative[i]:device);
  if(acquired<0)goto done;
  if(!acquired){if(i<2U){chain_error(ERROR_PATH_NOT_FOUND);goto done;}missing=i;fprintf(out,"CHAIN_FIRST_ABSENT index=%u name=%ls; NO_ADOPTION_NO_CREATION\n",i,relative[i]);break;}
  s->ancestor_count=i+1U;if(!chain_object(i,out,1))goto done;
 }
 size=64U;
 if(!GetComputerNameW(host,&size)||wcscmp(host,L"TCORP-TOBIAS")||
  !QueryDosDeviceW(L"C:",again,MAX_PATH)||wcscmp(again,s->program_files)||!chain_token()||
  papacc_configuration_personal_layout_binary_current(c)!=PAPACC_RESULT_OK){chain_error(ERROR_REVISION_MISMATCH);goto done;}
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&n->unexpected_thread)||GetLastError()!=ERROR_NO_TOKEN){chain_error(ERROR_ACCESS_DENIED);goto done;}
 /* No thread token exists here. Reuse the empty temporary-query slot for a
  * fresh process-token handle; compare against the retained ORIGINAL token. */
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&n->unexpected_thread)||
  !GetTokenInformation(n->unexpected_thread,TokenStatistics,&effective,sizeof(effective),&token_bytes)||
  memcmp(&effective.TokenId,&n->token.TokenId,sizeof(LUID))||memcmp(&effective.ModifiedId,&n->token.ModifiedId,sizeof(LUID))){chain_error(ERROR_REVISION_MISMATCH);goto done;}
 for(i=0;i<s->ancestor_count;i++)if(!chain_object(i,out,0))goto done;
 if(chain_stop()){chain_error(ERROR_CANCELLED);goto done;}result=0;
done:
 /* A pending native open or failed free/close retains all remaining backing.
  * Never retry CloseHandle/LocalFree after uncertainty. */
 if(s->phase==8U||s->scratch||n->sid_text)clean=0;
 for(i=4U;clean&&i;i--)if(s->objects[i-1U].handle){if(!CloseHandle(s->objects[i-1U].handle))clean=0;else s->objects[i-1U].handle=NULL;}
 if(clean&&n->check_token){if(!CloseHandle(n->check_token))clean=0;else n->check_token=NULL;}
 if(clean&&n->unexpected_thread){if(!CloseHandle(n->unexpected_thread))clean=0;else n->unexpected_thread=NULL;}
 if(clean&&s->trusted_installer){if(LocalFree(s->trusted_installer))clean=0;else s->trusted_installer=NULL;}
 if(clean&&papacc_server_console_win32_uninstall(&n->console)!=PAPACC_RESULT_OK)clean=0;
 if(!clean){chain_error(ERROR_INVALID_HANDLE);s->phase=8U;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);result=2;}
 else {s->phase=6U;SecureZeroMemory(s->block,sizeof(s->block));InterlockedExchange(&state,0);}
 fprintf(out,"CHAIN_RESULT collected=%u first=%lu existing=%u first_absent=%u close_known=%u retained=%u; CONTINUITY=SNAPSHOT_NOT_UNIVERSAL; OS_QUIESCENCE=UNKNOWN; AUTHORITY=DENY; EFFECTS=0; NO_RETRY\n",result==0?1U:0U,(unsigned long)s->first,s->ancestor_count,missing,clean?1U:0U,clean?0U:1U);
 if(ferror(out)||ferror(err))result=2;return result;
}
