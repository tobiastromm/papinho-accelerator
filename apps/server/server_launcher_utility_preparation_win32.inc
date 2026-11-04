/* Accepted029a611b: one exclusive utility bootstrap, SAME owner/packet.
 * No SCM, rename, directory creation, ACL edit or execution of the utility. */
static DWORD utility_prep_controller_current(void)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();PAPACC_QUALIFICATION_STAGING *s=staging_act();
 FILETIME created,exited,kernel,user;WCHAR process_name[MAX_PATH],file_name[MAX_PATH];
 DWORD a=MAX_PATH,b;BYTE digest[32];unsigned i;
 if(!staged_update_owned()||(u->preparation_mode!=1U&&u->preparation_mode!=2U)||!u->execute_mode||!u->executor_pid||
  !u->prep_chain_count||u->prep_chain_count>6U||!s->objects[7].handle||staged_update_stop())return staged_update_fail(ERROR_ACCESS_DENIED);
 if(u->executor_pid!=GetCurrentProcessId()||papacc_configuration_personal_native_current(&root.configuration_commit)!=PAPACC_RESULT_OK||
  !GetProcessTimes(GetCurrentProcess(),&created,&exited,&kernel,&user)||memcmp(&created,&u->executor_creation,sizeof(created)))return staged_update_fail(ERROR_REVISION_MISMATCH);
 for(i=0;i<u->prep_chain_count;i++)if(staged_update_object(u->prep_slots[i],0))return u->first;
 if(staged_update_object(7U,0)||!scm_register_hash(s->objects[7].handle,digest)||memcmp(digest,u->executor_hash,32))return staged_update_fail(ERROR_CRC);
 if(!QueryFullProcessImageNameW(GetCurrentProcess(),PROCESS_NAME_NATIVE,process_name,&a)||!a||a>=MAX_PATH)return staged_update_fail(ERROR_ACCESS_DENIED);
 b=GetFinalPathNameByHandleW(s->objects[7].handle,file_name,MAX_PATH,FILE_NAME_NORMALIZED|VOLUME_NAME_NT);
 if(!b||b>=MAX_PATH||a!=b||CompareStringOrdinal(process_name,(int)a,file_name,(int)b,FALSE)!=CSTR_EQUAL)return staged_update_fail(ERROR_ACCESS_DENIED);
 for(i=0;i<2U;i++){
  HMODULE module=GetModuleHandleW(staging_leaf[i+1U]);a=module?GetModuleFileNameW(module,process_name,MAX_PATH):0;
  b=GetFinalPathNameByHandleW(s->objects[9U+i].handle,file_name,MAX_PATH,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
  if(!a||a>=MAX_PATH||!b||b>=MAX_PATH||b!=a+4U||wcsncmp(file_name,L"\\\\?\\",4U)||
   CompareStringOrdinal(process_name,(int)a,file_name+4,(int)a,TRUE)!=CSTR_EQUAL)return staged_update_fail(ERROR_ACCESS_DENIED);
 }
 return 0; /* association since acquisition, never mapped-page attestation */
}
static DWORD utility_prep_current(void)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();PAPACC_QUALIFICATION_STAGING *s=staging_act();
 const unsigned file_slots[5]={7U,8U,9U,10U,13U};unsigned i,j;BYTE digest[32];
 if(!staged_update_owned()||(u->preparation_mode!=1U&&u->preparation_mode!=2U)||staged_update_stop()||
  papacc_configuration_personal_layout_binary_current(&root.configuration_commit)!=PAPACC_RESULT_OK||
  utility_prep_controller_current())return staged_update_fail(ERROR_ACCESS_DENIED);
 if(u->preparation_mode==2U&&staged_update_service())return u->first;
 for(i=0;i<s->ancestor_count;i++)if(staged_update_object(i,0))return u->first;
 for(i=0;i<5U;i++){
  unsigned k=file_slots[i];const BY_HANDLE_FILE_INFORMATION *f=&s->objects[k].identity;
  if(!s->objects[k].handle||staged_update_object(k,0))return staged_update_fail(ERROR_INVALID_STATE);
  if(k==13U&&(f->dwVolumeSerialNumber!=u->expected_old_id[0]||f->nFileIndexHigh!=u->expected_old_id[1]||f->nFileIndexLow!=u->expected_old_id[2]))return staged_update_fail(ERROR_REVISION_MISMATCH);
  if(k!=8U){const BYTE *pin=k==7U?u->executor_hash:k==13U?u->old_hash:u->dll_hash[k-9U];
   if(!scm_register_hash(s->objects[k].handle,digest)||memcmp(digest,pin,32))return staged_update_fail(ERROR_CRC);}
  for(j=0;j<i;j++){const BY_HANDLE_FILE_INFORMATION *g=&s->objects[file_slots[j]].identity;
   if(f->dwVolumeSerialNumber==g->dwVolumeSerialNumber&&f->nFileIndexHigh==g->nFileIndexHigh&&f->nFileIndexLow==g->nFileIndexLow)return staged_update_fail(ERROR_ACCESS_DENIED);}
  if(u->created){const BY_HANDLE_FILE_INFORMATION *g=&s->objects[14].identity;
   if(f->dwVolumeSerialNumber==g->dwVolumeSerialNumber&&f->nFileIndexHigh==g->nFileIndexHigh&&f->nFileIndexLow==g->nFileIndexLow)return staged_update_fail(ERROR_ACCESS_DENIED);}
 }
 if(u->confirmed&&(!staged_update_plan(digest)||memcmp(digest,u->plan_hash,32)))return staged_update_fail(ERROR_REVISION_MISMATCH);
 return staged_update_candidate_current();
}
static DWORD utility_prep_barrier(ULONGLONG remaining)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();
 if((u->preparation_mode!=1U&&u->preparation_mode!=2U)||!u->confirmed||!u->consumed||u->act_state!=3U||u->first)return staged_update_fail(ERROR_ACCESS_DENIED);
 if(utility_prep_current())return u->first;
 return staged_update_resources(remaining); /* own K_PREP and accepted policy */
}
size_t papacc_server_launcher_utility_prep_known_bytes(void)
{
 /* Explicit conservative census of reused static storage plus named caller,
  * chain, presenter, parser/hash and observer frames. Not UPDATE K or a peak. */
 return PAPACC_C8_KNOWN_MANAGED_BYTES+papacc_server_service_storage_bytes()+sizeof(LONG)+
 SCM_RESOURCE_KNOWN_AUTOMATIC_BYTES+sizeof(PACC_STORAGE_SHA256)+1024U+
 7U*MAX_PATH*sizeof(WCHAR)+1024U+96U+10U*sizeof(FILETIME)+
 5U*32U+6U*sizeof(DWORD)+128U*sizeof(void *)+128U*sizeof(DWORD)+
 sizeof(BY_HANDLE_FILE_INFORMATION)+128U;
}
static DWORD utility_prep_capture_source(const WCHAR *source)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();PAPACC_QUALIFICATION_STAGING *s=staging_act();
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&root.configuration_commit.pending_commit.personal_native;
 WCHAR part[MAX_PATH];size_t i,start,len,prefix;HANDLE parent;FILETIME exited,kernel,user;DWORD bytes;
 u->location=930U;
 bytes=GetModuleFileNameW(NULL,u->executor_path,MAX_PATH);
 if(!bytes||bytes>=MAX_PATH||bytes<4U||u->executor_path[1]!=L':'||u->executor_path[2]!=L'\\'||
  _wcsnicmp(u->executor_path,n->install_paths.binary,2U)||wcsstr(u->executor_path,L".."))return staged_update_fail(ERROR_INVALID_NAME);
 len=wcslen(u->executor_path);prefix=len;
 while(prefix>3U&&u->executor_path[prefix-1U]!=L'\\')prefix--;
 if(wcslen(source)!=prefix+wcslen(L"papacc_update.exe")||wcscmp(u->executor_path+prefix,L"papacc_server.exe")||
  _wcsnicmp(source,u->executor_path,prefix)||wcscmp(source+prefix,L"papacc_update.exe"))return staged_update_fail(ERROR_ACCESS_DENIED);
 /* Every original source-directory handle is retained; no quota expansion.
  * Collision with protected ancestors/reserved files means DENY. */
 parent=root.configuration_commit.pending_commit.bootstrap_parent;start=3U;
 for(i=start;i<prefix;i++)if(u->executor_path[i]==L'\\'){
  unsigned slot;
  if(i==start||u->prep_chain_count==6U)return staged_update_fail(ERROR_INSUFFICIENT_BUFFER);
  for(slot=0;slot<14U;slot++)if(slot!=7U&&slot!=8U&&slot!=9U&&slot!=10U&&slot!=13U&&!s->objects[slot].handle)break;
  if(slot>=14U)return staged_update_fail(ERROR_INSUFFICIENT_BUFFER);
  u->prep_slots[u->prep_chain_count++]=slot;
  memcpy(part,u->executor_path+start,(i-start)*sizeof(WCHAR));part[i-start]=0;
  if(staged_update_open(slot,parent,part,1))return u->first;parent=s->objects[slot].handle;start=i+1U;
 }
 if(!u->prep_chain_count||staged_update_open(7U,parent,L"papacc_server.exe",0))return staged_update_fail(ERROR_ACCESS_DENIED);
 u->executor_id[0]=s->objects[7].identity.dwVolumeSerialNumber;u->executor_id[1]=s->objects[7].identity.nFileIndexHigh;u->executor_id[2]=s->objects[7].identity.nFileIndexLow;
 u->executor_pid=GetCurrentProcessId();
 if(!u->executor_pid||!GetProcessTimes(GetCurrentProcess(),&u->executor_creation,&exited,&kernel,&user))return staged_update_fail(ERROR_ACCESS_DENIED);
 for(i=0;i<2U;i++)if(staged_update_open(9U+(unsigned)i,parent,staging_leaf[i+1U],0))return u->first;
 if(utility_prep_controller_current()||staged_update_open(8U,parent,L"papacc_update.exe",0))return u->first;
 return 0;
}
static DWORD utility_prep_present(FILE *out)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();PAPACC_PERSONAL_NATIVE_CONTEXT *n=&root.configuration_commit.pending_commit.personal_native;
 PAPACC_QUALIFICATION_STAGING *s=staging_act();
 HANDLE input=GetStdHandle(STD_INPUT_HANDLE),output=GetStdHandle(STD_OUTPUT_HANDLE);DWORD mode;unsigned i,j;BYTE nonzero=0;
 char text[1024],reply[96];static const char hex[]="0123456789abcdef";
 if((u->preparation_mode!=1U&&u->preparation_mode!=2U)||u->act_state||u->consumed||staged_update_stop()||
 !GetConsoleMode(input,&mode)||!GetConsoleMode(output,&mode))return staged_update_fail(ERROR_ACCESS_DENIED);
 u->location=940U;
 if(BCryptGenRandom(NULL,u->attempt,32U,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0)return staged_update_fail(ERROR_GEN_FAILURE);
 for(i=0;i<32U;i++)nonzero|=u->attempt[i];if(!nonzero)return staged_update_fail(ERROR_INVALID_DATA);
 if(u->preparation_mode==2U){
  for(i=0;i<32U;i++){text[2U*i]=hex[u->attempt[i]>>4];text[2U*i+1U]=hex[u->attempt[i]&15U];}text[64]=0;
  if(swprintf_s(u->candidate,96,L"papacc_update.%hs.candidate",text)<0||swprintf_s(u->previous,96,L"papacc_update.%hs.previous",text)<0)return staged_update_fail(ERROR_INVALID_NAME);
 }
 if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)",SDDL_REVISION_1,&staging_act()->protection,NULL)||
  utility_prep_current()||staged_update_resources(u->new_bytes)||!staged_update_plan(u->plan_hash))return staged_update_fail(ERROR_ACCESS_DENIED);
 if(!personal_console_write(output,u->preparation_mode==2U?"\nUTILITY_UPDATE_CLOSED - ONE PERSONAL_LOCAL UTILITY ONLY\nOperator: ":"\nUTILITY_PREPARE_EXCLUSIVE - ONE PERSONAL_LOCAL FILE\nOperator: ")||!personal_console_write(output,n->sid_text))return staged_update_fail(ERROR_WRITE_FAULT);
 if(!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,n->install_paths.binary,-1,text,sizeof(text),NULL,NULL)||
  !personal_console_write(output,"\nExisting parent: ")||!personal_console_token(output,text)||
  !WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,u->executor_path,-1,text,sizeof(text),NULL,NULL)||
  !personal_console_write(output,"\nBootstrap controller: ")||!personal_console_token(output,text)||
  !WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,u->source_path,-1,text,sizeof(text),NULL,NULL)||
  !personal_console_write(output,"\nUtility DATA: ")||!personal_console_token(output,text))return staged_update_fail(ERROR_WRITE_FAULT);
 sprintf_s(text,sizeof(text),"\nParent ID=%lu:%lu:%lu; old canonical=%lu:%lu:%lu; controller=%lu:%lu:%lu PID=%lu.\nBytes=%llu; K_PREP=%llu; physical/commit floor=K_PREP; R_PREP=%llu actual unit=%llu.\n",
 (unsigned long)u->expected_parent_id[0],(unsigned long)u->expected_parent_id[1],(unsigned long)u->expected_parent_id[2],
 (unsigned long)u->expected_old_id[0],(unsigned long)u->expected_old_id[1],(unsigned long)u->expected_old_id[2],
 (unsigned long)u->executor_id[0],(unsigned long)u->executor_id[1],(unsigned long)u->executor_id[2],(unsigned long)u->executor_pid,u->new_bytes,(ULONGLONG)root.reserved_bytes,u->rounded,u->unit);
 if(!personal_console_write(output,text))return staged_update_fail(ERROR_WRITE_FAULT);
 for(i=0;i<14U;i++)if(staging_act()->objects[i].handle){
  const PAPACC_STAGING_OBJECT *o=&staging_act()->objects[i];
  sprintf_s(text,sizeof(text),"Original slot=%u role=%s ID=%lu:%lu:%lu bytes=%llu SD_SHA256=",i,
   i<s->ancestor_count?"protected_ancestor":utility_prep_chain_slot(i)?"bootstrap_ancestor":i==7U?"controller":i==8U?"utility_DATA":i==13U?"canonical_readonly":"controller_dependency",
   (unsigned long)o->identity.dwVolumeSerialNumber,(unsigned long)o->identity.nFileIndexHigh,(unsigned long)o->identity.nFileIndexLow,
   ((ULONGLONG)o->identity.nFileSizeHigh<<32)|o->identity.nFileSizeLow);
  if(!personal_console_write(output,text))return staged_update_fail(ERROR_WRITE_FAULT);
  for(j=0;j<32U;j++){text[j*2U]=hex[o->acl[j]>>4];text[j*2U+1U]=hex[o->acl[j]&15U];}text[64]=0;
  if(!personal_console_write(output,text)||!personal_console_write(output,"\n"))return staged_update_fail(ERROR_WRITE_FAULT);
 }
 for(i=0;i<5U;i++){
  const BYTE *pin=i==0U?u->executor_hash:i==1U?u->new_hash:i==4U?u->old_hash:u->dll_hash[i-2U];
  const char *label=i==0U?"Controller SHA256: ":i==1U?"Utility SHA256: ":i==4U?(u->preparation_mode==2U?"Old utility SHA256: ":"Unchanged canonical SHA256: "):i==2U?"Controller libssl SHA256: ":"Controller libcrypto SHA256: ";
  if(!personal_console_write(output,label))return staged_update_fail(ERROR_WRITE_FAULT);
  for(j=0;j<32U;j++){text[j*2U]=hex[pin[j]>>4];text[j*2U+1U]=hex[pin[j]&15U];}text[64]=0;
  if(!personal_console_write(output,text)||!personal_console_write(output,"\n"))return staged_update_fail(ERROR_WRITE_FAULT);
 }
 if(u->preparation_mode==2U){
  if(!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,u->candidate,-1,text,sizeof(text),NULL,NULL)||!personal_console_write(output,"\nExclusive candidate: ")||!personal_console_token(output,text)||
   !WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,u->previous,-1,text,sizeof(text),NULL,NULL)||!personal_console_write(output,"\nRetain old utility: ")||!personal_console_token(output,text)||
   !personal_console_write(output,"\nONLY papacc_update.exe: one exclusive protected candidate/copy/hash/flush, old utility->previous then candidate->papacc_update.exe, NO OVERWRITE. SCM STOPPED/PID0 and original config revalidated. No server, prior candidate, DLL, data, ACL or SCM writes. Bootstrap/opaque costs UNKNOWN, partial canonical absence and persistent retention accepted only for this act. NO ROLLBACK/DELETE/RETRY/START. Does not resolve historical retention. Declare legitimate administration and confirm ONLY utility maintenance.\n"))return staged_update_fail(ERROR_WRITE_FAULT);
 }else if(!personal_console_write(output,"Exact leaf papacc_update.exe. ONE exclusive FILE_CREATE, BA/SY full AU read/execute protected DACL, copy/hash/flush/readback. Existing leaf denies. No directories, rename, overwrite, existing ACL edit, SCM, Start, endpoint, CA/CURRENT or DLL/server writes. Workspace bootstrap and opaque cost risks accepted only for preparation, not U2 UPDATE. Locks/hashes are not mapped-image attestation or administrative authority. Partial file retained, NO DELETE/ROLLBACK/RETRY. Declare legitimate administration and confirm ONLY this preparation.\n"))return staged_update_fail(ERROR_WRITE_FAULT);
 strcpy_s(reply,sizeof(reply),u->preparation_mode==2U?"OWNER_UTILITY_UPDATE ":"OWNER_UTILITY_PREPARE ");j=(unsigned)strlen(reply);
 for(i=0;i<32U;i++){reply[j+i*2U]=hex[u->attempt[i]>>4];reply[j+i*2U+1U]=hex[u->attempt[i]&15U];}reply[j+64U]=0;
 u->act_state=1U;
 if(!personal_console_write(output,reply)||!personal_console_write(output,"\n")||!personal_console_confirm(input,output,reply))return staged_update_fail(ERROR_CANCELLED);
 if(utility_prep_current()||!staged_update_plan((BYTE *)text)||memcmp(text,u->plan_hash,32))return staged_update_fail(ERROR_REVISION_MISMATCH);
 u->confirmed=1U;u->act_state=2U;u->missing=PAPACC_UPDATE_MISSING_NATIVE;(void)out;return 0;
}
static DWORD utility_prep_apply(void)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();
 if(!staged_update_owned()||u->preparation_mode!=1U||u->act_state!=2U||!u->confirmed||u->consumed||u->first)return ERROR_ACCESS_DENIED;
 if(utility_prep_current()||staged_update_resources(u->new_bytes))return u->first;
 if(InterlockedCompareExchange((volatile LONG *)&u->consumed,1,0)!=0)return ERROR_ACCESS_DENIED;u->act_state=3U;
 if(staged_update_create()||staged_binary_copy_and_flush()||staged_update_readback(15U,L"papacc_update.exe",14U,u->new_hash)||utility_prep_current())return u->first;
 return 0;
}
static DWORD utility_prep_close(void)
{
 PAPACC_STAGED_UPDATE_STATE *u=staged_update_state();PAPACC_QUALIFICATION_STAGING *s=staging_act();
 static const unsigned file_slots[7]={15U,14U,8U,7U,10U,9U,13U};unsigned i,k;
 if(!staged_update_owned()||(u->preparation_mode!=1U&&u->preparation_mode!=2U)||s->phase==8U||u->phase==8U)return ERROR_INVALID_STATE;
 if(u->effects&&u->first)return staged_update_retain(u->first);
 /* Child files first, then bootstrap directories in reverse, then protected
  * ancestors/native context via the existing close. Uncertain close stops. */
 for(i=0;i<7U+u->prep_chain_count;i++){
  k=i<7U?file_slots[i]:u->prep_slots[u->prep_chain_count-1U-(i-7U)];
  if(!s->objects[k].handle)continue;
  if(s->close_attempts&(1U<<k))return staged_update_retain(ERROR_INVALID_HANDLE);s->close_attempts|=1U<<k;
  if(!CloseHandle(s->objects[k].handle))return staged_update_retain(ERROR_INVALID_HANDLE);s->objects[k].handle=NULL;
 }
 return staged_update_close();
}
#include "server_launcher_utility_replacement_win32.inc"
static int utility_bootstrap_entry(const WCHAR *source,const DWORD parent_id[3],const DWORD old_id[3],const BYTE pins[5][32],FILE *out,unsigned purpose)
{
 PAPACC_CONFIGURATION_COMMIT *c=&root.configuration_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&c->pending_commit.personal_native;
 PAPACC_QUALIFICATION_STAGING *s=&n->staging;PAPACC_STAGED_UPDATE_STATE *u=&n->staged_update;
 WCHAR part[MAX_PATH];size_t i,start,len;int clean=0;BYTE digest[32];
 if((purpose!=1U&&purpose!=2U)||!source||!parent_id||!old_id||!pins||!out||wcsnlen(source,MAX_PATH)>=MAX_PATH||wcslen(source)<4U||source[1]!=L':'||source[2]!=L'\\'||wcsstr(source,L".."))return 2;
 if(InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,0)!=0)return 2;
 if(next_invocation==(PAPACC_U64)-1){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return 2;}
 ZeroMemory(&root,sizeof(root));root.owner_thread=GetCurrentThreadId();root.invocation=++next_invocation;c->writer=root.owner_thread;
 s->self=s;s->thread=root.owner_thread;s->invocation=root.invocation;s->phase=1U;
 u->self=u;u->thread=root.owner_thread;u->invocation=root.invocation;u->phase=1U;u->preparation_mode=purpose;u->execute_mode=1U;
 u->missing=PAPACC_UPDATE_MISSING_NATIVE|PAPACC_UPDATE_MISSING_ACT;
 memcpy(u->expected_parent_id,parent_id,12);memcpy(u->expected_old_id,old_id,12);
 memcpy(u->old_hash,pins[0],32);memcpy(u->new_hash,pins[1],32);memcpy(u->dll_hash,pins[2],64);memcpy(u->executor_hash,pins[4],32);
 wcscpy_s(u->source_path,MAX_PATH,source);wcscpy_s(u->candidate,96,L"papacc_update.exe");u->resources.sink=out;
 root.reserved_bytes=purpose==2U?papacc_server_launcher_utility_update_known_bytes():papacc_server_launcher_utility_prep_known_bytes();
 if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT){staged_update_fail(ERROR_NOT_ENOUGH_QUOTA);goto done;}
 if(staged_update_stop()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK){staged_update_fail(ERROR_CANCELLED);goto done;}
 if(papacc_configuration_personal_layout_binary(c)!=PAPACC_RESULT_OK){staged_update_fail(ERROR_ACCESS_DENIED);goto done;}
 {char volume[4]={(char)n->install_paths.binary[0],':','\\',0};if(papacc_configuration_personal_native_open(c,volume)!=PAPACC_RESULT_OK){staged_update_fail(ERROR_ACCESS_DENIED);goto done;}}
 if(!ConvertStringSidToSidW(L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464",&s->trusted_installer)){staged_update_fail(ERROR_INVALID_SID);goto done;}
 len=wcslen(n->install_paths.binary);start=3U;
 for(i=start;i<=len;i++)if(n->install_paths.binary[i]==L'\\'||!n->install_paths.binary[i]){
  unsigned k=s->ancestor_count;if(i==start||k==7U){staged_update_fail(ERROR_INSUFFICIENT_BUFFER);goto done;}
  memcpy(part,n->install_paths.binary+start,(i-start)*sizeof(WCHAR));part[i-start]=0;
  if(staged_update_open(k,k?s->objects[k-1U].handle:c->pending_commit.bootstrap_parent,part,i==len?2:1))goto done;
  s->ancestor_count++;start=i+1U;
 }
 if(!s->ancestor_count){staged_update_fail(ERROR_INVALID_NAME);goto done;}
 {const BY_HANDLE_FILE_INFORMATION *f=&s->objects[s->ancestor_count-1U].identity;
 if(f->dwVolumeSerialNumber!=parent_id[0]||f->nFileIndexHigh!=parent_id[1]||f->nFileIndexLow!=parent_id[2]){staged_update_fail(ERROR_REVISION_MISMATCH);goto done;}}
 if(staged_update_open(13U,s->objects[s->ancestor_count-1U].handle,purpose==2U?L"papacc_update.exe":L"papacc_server.exe",0)||utility_prep_capture_source(source))goto done;
 if(purpose==2U){
  u->manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT);if(!u->manager){staged_update_fail(GetLastError());goto done;}
  u->service=OpenServiceW(u->manager,L"PapinhoAccelerator",SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS);if(!u->service){staged_update_fail(GetLastError());goto done;}
 }
 if(!scm_register_hash(s->objects[8].handle,digest)||memcmp(digest,u->new_hash,32)){staged_update_fail(ERROR_CRC);goto done;}
 u->new_bytes=((ULONGLONG)s->objects[8].identity.nFileSizeHigh<<32)|s->objects[8].identity.nFileSizeLow;
 u->old_bytes=((ULONGLONG)s->objects[13].identity.nFileSizeHigh<<32)|s->objects[13].identity.nFileSizeLow;
 if(!u->new_bytes||u->new_bytes>MAXDWORD-4U||utility_prep_current()){staged_update_fail(ERROR_INVALID_DATA);goto done;}
 u->prepared=1U;
 if(utility_prep_present(out)||(purpose==2U?utility_update_apply():utility_prep_apply()))goto done;
done:
 if(u->phase!=8U&&s->phase!=8U){(void)utility_prep_close();clean=u->phase==4U&&s->phase==6U;}
 if(!clean){root.configuration_commit.unresolved=1;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);}else InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);
 fprintf(out,"%s prepared=%u first=%lu location=%lu consumed=%u submitted=%u created=%u close_known=%u K_PREP=%llu R_PREP=%llu; %s; OPAQUE_UNKNOWN; OS_QUIESCENCE=UNKNOWN; NO_RETRY_NO_UPDATE_NO_START\n",purpose==2U?"UTILITY_UPDATE_RESULT":"UTILITY_PREP_RESULT",
 u->prepared,(unsigned long)u->first,(unsigned long)u->first_location,u->consumed,u->effects,u->created,(unsigned)clean,(ULONGLONG)root.reserved_bytes,u->rounded,
 !u->first&&u->created&&clean?(purpose==2U&&u->rename_done==3U?"UTILITY_UPDATE_VERIFIED_CLOSED":purpose==1U?"UTILITY_EXCLUSIVE_VERIFIED_CLOSED":"AUTHORITY=DENY"):"AUTHORITY=DENY");
 if(purpose==2U)fprintf(out,"UTILITY_UPDATE_RENAMES mask=%u canonical_verified_closed=%u; HISTORICAL_RETENTION_UNCHANGED\n",u->rename_done,(unsigned)(!u->first&&u->created&&clean&&u->rename_done==3U));
 if(u->created)fprintf(out,"UTILITY_PREP_OBJECT volume=%lu high=%lu low=%lu bytes=%llu; METADATA_NOT_AUTHORITY\n",
 (unsigned long)s->objects[14].identity.dwVolumeSerialNumber,(unsigned long)s->objects[14].identity.nFileIndexHigh,(unsigned long)s->objects[14].identity.nFileIndexLow,u->new_bytes);
 return !u->first&&u->created&&clean&&(purpose==1U||u->rename_done==3U)?0:2;
}
static int utility_prep_entry(const WCHAR *source,const DWORD parent_id[3],const DWORD old_id[3],const BYTE pins[5][32],FILE *out)
{return utility_bootstrap_entry(source,parent_id,old_id,pins,out,1U);}
int papacc_server_launcher_utility_prep_cli(int argc,const char *const *argv,FILE *out,FILE *err)
{
 WCHAR source[MAX_PATH];DWORD ids[6];BYTE pins[5][32];unsigned i;(void)err;
 if(argc<2||!argv||!argv[1]||strcmp(argv[1],"--owner-manual-prepare-update-utility"))return -1;
 if(argc!=14)return 2;
 for(i=0;i<6U;i++)if(!staged_update_decimal(argv[3U+i],&ids[i]))return 2;
 for(i=0;i<5U;i++)if(!staged_update_hex(argv[9U+i],pins[i]))return 2;
 if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argv[2],-1,source,MAX_PATH))return 2;
 return utility_prep_entry(source,ids,ids+3,(const BYTE (*)[32])pins,out);
}
int papacc_server_launcher_utility_update_cli(int argc,const char *const *argv,FILE *out,FILE *err)
{
 WCHAR source[MAX_PATH];DWORD ids[6];BYTE pins[5][32];unsigned i;(void)err;
 if(argc<2||!argv||!argv[1]||strcmp(argv[1],"--owner-manual-update-utility"))return -1;
 if(argc!=14)return 2;
 for(i=0;i<6U;i++)if(!staged_update_decimal(argv[3U+i],&ids[i]))return 2;
 for(i=0;i<5U;i++)if(!staged_update_hex(argv[9U+i],pins[i]))return 2;
 if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argv[2],-1,source,MAX_PATH))return 2;
 return utility_bootstrap_entry(source,ids,ids+3,(const BYTE (*)[32])pins,out,2U);
}
