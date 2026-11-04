#include "personal_scq_start_plan_data_win32.inc"
#include "personal_scq_start_assurance_win32.inc"
#include "personal_scq_existing_data_win32.inc"
#include "personal_scq_native_name_win32.inc"
#include "personal_scq_native_sd_win32.inc"
/* Stages: 1=count query failed, 2=guard124, 3=SD query, 4=process times,
 * 5=process exit query, 6=NtCreateFile result rejected, 7=P1 name resolution.
 * Ref3 is not a file. */
static DWORD scqc_first_cause(PAPACC_CONFIGURATION_COMMIT *o,DWORD stage,DWORD reference,DWORD mapped,DWORD native_error)
{
 PAPACC_SCQ_COLD_DIAGNOSTIC *d=&o->pending_commit.personal_native.scq_cold_diagnostic;
 if(!mapped)mapped=ERROR_GEN_FAILURE;
 if(!d->mapped){d->stage=stage;d->reference=reference;d->mapped=mapped;d->native_error=native_error;}
 return mapped;
}
static DWORD scqc_descriptor(PAPACC_CONFIGURATION_COMMIT *o,unsigned index)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;DWORD e;
 if(n->install_paths.mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT){
  if(n->install_paths.phase!=PAPACC_PERSONAL_PATHS_READY||n->install_paths.selected_role!=1U||
   n->install_paths.thread!=GetCurrentThreadId()||n->install_paths.selected.architecture!=0x8664U)return ERROR_INVALID_STATE;
  e=scqi_native_file_sd(n->scq_cold_raw[index],&n->scq_inputs.checking,&n->scq_cold_diagnostic.native_sd,&o->pending_commit.stop);
  if(e||!n->scq_inputs.checking){if(n->scq_inputs.checking)o->unresolved=1;return scqc_first_cause(o,8U,index,e?e:ERROR_INVALID_SECURITY_DESCR,n->scq_cold_diagnostic.native_sd.status);}
  return 0;
 }
 e=GetSecurityInfo(n->scq_cold_raw[index],SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->scq_inputs.checking);
 if(e||!n->scq_inputs.checking){if(n->scq_inputs.checking)o->unresolved=1;return scqc_first_cause(o,3U,index,e?e:ERROR_INVALID_SECURITY_DESCR,e);}
 return 0;
}
static DWORD scqc_process_times(PAPACC_CONFIGURATION_COMMIT *o,FILETIME *created,FILETIME *exited,FILETIME *kernel,FILETIME *user)
{DWORD e;if(GetProcessTimes(GetCurrentProcess(),created,exited,kernel,user))return 0;e=GetLastError();return scqc_first_cause(o,4U,3U,e,e);}
static DWORD scqc_process_exit(PAPACC_CONFIGURATION_COMMIT *o,DWORD *code)
{DWORD e;if(GetExitCodeProcess(GetCurrentProcess(),code))return 0;e=GetLastError();return scqc_first_cause(o,5U,3U,e,e);}
/* Diagnostic only. Never enters scqc_plan/open_raw, admission or an effect.
 * Slots0/1 are captured by the caller before/after the existing owner acquire.
 * No output before the final sample; CRT/loader before main remain combined. */
static DWORD scqc_observe_count(DWORD *sample)
{
 DWORD count=0,error;sample[0]=sample[1]=sample[2]=0;
 if(!GetProcessHandleCount(GetCurrentProcess(),&count)){
  error=GetLastError();sample[2]=error?error:ERROR_GEN_FAILURE;return sample[2];
 }
 sample[0]=1;sample[1]=count;return 0;
}
DWORD papacc_configuration_personal_cold_handle_observe(PAPACC_CONFIGURATION_COMMIT *o,DWORD samples[15])
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;DWORD error;WCHAR drive[4];
 if(!o||!samples||o->writer!=GetCurrentThreadId()||o->unresolved||o->pending_commit.active||o->pending_commit.stop)return ERROR_INVALID_STATE;
 n=&o->pending_commit.personal_native;if(n->phase||n->scq_cold_raw[0]||n->scq_cold_raw[1]||n->scq_cold_raw[2])return ERROR_INVALID_STATE;
 n->phase=1U;
 if(papacc_server_console_win32_stop_requested()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK)return ERROR_CANCELLED;
 error=scqc_observe_count(samples+6);if(error)return error;
 if(!personal_token(&o->pending_commit,1))return ERROR_ACCESS_DENIED;
 error=scqc_observe_count(samples+9);if(error)return error;
 if(papacc_server_console_win32_stop_requested())return ERROR_CANCELLED;
 error=papacc_personal_paths_binary(&n->install_paths,&o->pending_commit.stop);if(error)return error;
 drive[0]=n->install_paths.binary[0];drive[1]=L':';drive[2]=L'\\';drive[3]=0;
 if(GetDriveTypeW(drive)!=DRIVE_FIXED)return ERROR_INVALID_NAME;
 return scqc_observe_count(samples+12);
}
static int scqc_local_path(const WCHAR *p)
{
 size_t n;if(!p)return 0;n=wcsnlen(p,MAX_PATH);
 return n>3U&&n<MAX_PATH&&((p[0]>=L'A'&&p[0]<=L'Z')||(p[0]>=L'a'&&p[0]<=L'z'))&&p[1]==L':'&&p[2]==L'\\';
}
typedef NTSTATUS (NTAPI *SCQC_NT_OPEN)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
/* P1 cold TO only: resolve the DOS selector as candidate location DATA, as
 * native_open already does. Never traverse a filesystem reparse or infer
 * authority/identity from the mapping. All original-file checks remain later.
 * Only a direct device name is accepted; SUBST/UNC/recursive aliases deny. */
static DWORD scqc_selected_name(PAPACC_PERSONAL_NATIVE_CONTEXT *n,const WCHAR *path)
{
 if(n->install_paths.mode!=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT){
  wcscpy_s(n->scq_cold_name,MAX_PATH+4U,L"\\??\\");wcscat_s(n->scq_cold_name,MAX_PATH+4U,path);return 0;
 }
 if(n->install_paths.phase!=PAPACC_PERSONAL_PATHS_READY||n->install_paths.selected_role!=1U||
  n->install_paths.thread!=GetCurrentThreadId()||n->install_paths.selected.architecture!=0x8664U)return ERROR_INVALID_STATE;
 return scq_p1_native_name(path,0,n->scq_cold_name);
}
static DWORD scqc_interval_flags(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 return (n->scq_cold_raw[0]?1U:0U)|(n->scq_inputs.checking?2U:0U)|
  (n->scq_inputs.content_hash.hash?4U:0U)|(n->scq_inputs.content_hash.algorithm?8U:0U)|
  (n->scq_inputs.content_hash.active?16U:0U)|(n->scq_inputs.content_hash.retained?32U:0U)|
  (o->pending_commit.stop?64U:0U);
}
static DWORD scqc_interval_error(PAPACC_SCQ_INTERVAL_OBSERVATION *v,unsigned cut,DWORD error)
{if(v&&!v->first_error){v->first_error=error?error:ERROR_GEN_FAILURE;v->first_cut=cut;}return error?error:ERROR_GEN_FAILURE;}
static DWORD scqc_interval_sample(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_SCQ_INTERVAL_OBSERVATION *v,unsigned cut)
{
 DWORD count=0,error;if(!v)return 0;
 v->sample[cut][3]=scqc_interval_flags(o);
 if(!GetProcessHandleCount(GetCurrentProcess(),&count)){
  error=GetLastError();v->sample[cut][2]=error?error:ERROR_GEN_FAILURE;return scqc_interval_error(v,cut,v->sample[cut][2]);
 }
 v->sample[cut][0]=1;v->sample[cut][1]=count;
 if(count>128U)return scqc_interval_error(v,cut,ERROR_NOT_ENOUGH_QUOTA);
 /* Extra observations cannot license an acquisition after learning that the
  * original pre-acquisition guard is no longer satisfied. */
 if((cut==1U||cut==4U)&&count>=124U)return scqc_interval_error(v,cut,ERROR_NOT_ENOUGH_QUOTA);
 if(o->pending_commit.stop||papacc_server_console_win32_stop_requested())return scqc_interval_error(v,cut,ERROR_CANCELLED);
 return 0;
}
static DWORD scqc_open_raw_mode(PAPACC_CONFIGURATION_COMMIT *o,unsigned index,const WCHAR *path,PAPACC_SCQ_INTERVAL_OBSERVATION *v)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 WCHAR drive[4];SCQC_NT_OPEN open_file;FARPROC address;HMODULE module;DWORD own,error;BOOL queried;NTSTATUS status;
 PAPACC_SCQ_COLD_DIAGNOSTIC *diagnostic;
 if(!o||o->writer!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 if(n->phase!=1U||index>=3U||!scqc_local_path(path)||n->scq_cold_raw[index]||o->unresolved||o->pending_commit.stop)return ERROR_INVALID_PARAMETER;
 drive[0]=path[0];drive[1]=L':';drive[2]=L'\\';drive[3]=0;
 if(GetDriveTypeW(drive)!=DRIVE_FIXED)return ERROR_INVALID_NAME;
 /* Diagnostic mode uses only its fixed DATA locator, never installs a selected
  * target state or enters original admission. The low-level resolver is shared. */
 if(v&&index)return ERROR_INVALID_PARAMETER;
 error=v?scq_p1_native_name(path,0,n->scq_cold_name):scqc_selected_name(n,path);if(error)return scqc_first_cause(o,7U,index,error,0);
 diagnostic=&n->scq_cold_diagnostic;
 if(!diagnostic->mapped){diagnostic->query_attempted=1U;diagnostic->query_succeeded=0U;diagnostic->own_count=0U;diagnostic->query_reference=index;}
 queried=GetProcessHandleCount(GetCurrentProcess(),&own);
 if(!queried){error=GetLastError();if(v){v->sample[0][2]=error?error:ERROR_GEN_FAILURE;v->sample[0][3]=scqc_interval_flags(o);}return scqc_first_cause(o,1U,index,error,error);}
 if(!diagnostic->mapped){diagnostic->query_succeeded=1U;diagnostic->own_count=own;}
 if(v){v->sample[0][0]=1;v->sample[0][1]=own;v->sample[0][3]=scqc_interval_flags(o);}
 if(!v&&PAPACC_SCQ_P1_PATHS(&n->install_paths)&&index==0U){
  if(diagnostic->initial_known)return ERROR_INVALID_STATE;
  diagnostic->initial_known=1U;diagnostic->initial_handles=own;
  if(own>PAPACC_SCQ_P1_ENTRY)return scqc_first_cause(o,2U,index,ERROR_NOT_ENOUGH_QUOTA,0);
 }
 if(own>=PAPACC_SCQ_HANDLE_GUARD(!v&&PAPACC_SCQ_P1_PATHS(&n->install_paths)))return scqc_first_cause(o,2U,index,ERROR_NOT_ENOUGH_QUOTA,0);
 module=GetModuleHandleW(L"ntdll.dll");address=module?GetProcAddress(module,"NtCreateFile"):NULL;
 if(!address)return ERROR_NOT_SUPPORTED;memcpy(&open_file,&address,sizeof(open_file));
 error=scqc_interval_sample(o,v,1U);if(error)return error;
 memset(&n->scq_cold_io,0,sizeof(n->scq_cold_io));memset(&n->scq_cold_attributes,0,sizeof(n->scq_cold_attributes));
 n->scq_cold_unicode.Buffer=n->scq_cold_name;n->scq_cold_unicode.Length=(USHORT)(wcslen(n->scq_cold_name)*sizeof(WCHAR));n->scq_cold_unicode.MaximumLength=n->scq_cold_unicode.Length;
 n->scq_cold_attributes.Length=sizeof(n->scq_cold_attributes);n->scq_cold_attributes.ObjectName=&n->scq_cold_unicode;
 n->scq_cold_attributes.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 if(o->pending_commit.stop||papacc_server_console_win32_stop_requested())return ERROR_CANCELLED;
 if(!diagnostic->mapped){diagnostic->open_attempted=1U;diagnostic->open_reference=index;}
 status=open_file(&n->scq_cold_raw[index],PAPACC_SCQ_FILE_READ_RIGHTS,&n->scq_cold_attributes,&n->scq_cold_io,NULL,FILE_ATTRIBUTE_NORMAL,
  FILE_SHARE_READ,1U,0x00200060U,NULL,0);n->scq_cold_status=status;
 if(!diagnostic->mapped){
  diagnostic->open_status=(DWORD)status;diagnostic->io_status=(DWORD)n->scq_cold_io.Status;
  diagnostic->io_information=(ULONGLONG)n->scq_cold_io.Information;diagnostic->returned_handle=n->scq_cold_raw[index]?1U:0U;
 }
 if(status||n->scq_cold_io.Status||n->scq_cold_io.Information!=1U||!n->scq_cold_raw[index]){
  if(status>0||n->scq_cold_io.Status==(NTSTATUS)0x103L||n->scq_cold_raw[index])o->unresolved=1;
  return scqc_first_cause(o,6U,index,o->unresolved?ERROR_IO_INCOMPLETE:ERROR_ACCESS_DENIED,0);
 }
 return scqc_interval_sample(o,v,2U);
}
static DWORD scqc_open_raw(PAPACC_CONFIGURATION_COMMIT *o,unsigned index,const WCHAR *path)
{return scqc_open_raw_mode(o,index,path,NULL);}
/* The same SD/hash/cleanup sequence in the real cold acquisition and observer.
 * No provider substitution, eager close or change to NULL-observer behavior. */
static DWORD scqc_descriptor_digest(PAPACC_CONFIGURATION_COMMIT *o,unsigned index,BYTE digest[32],PAPACC_SCQ_INTERVAL_OBSERVATION *v)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 PAPACC_PERSONAL_SCQ_INPUTS *in=&n->scq_inputs;DWORD e;
 e=scqc_descriptor(o,index);if(e)return v?scqc_interval_error(v,4U,e):e;
 if(!IsValidSecurityDescriptor(in->checking))return v?scqc_interval_error(v,4U,ERROR_INVALID_SECURITY_DESCR):ERROR_INVALID_SECURITY_DESCR;
 e=scqc_interval_sample(o,v,4U);if(e)return e;
 if(v)v->hash_called=1U;
 if(!papacc_store_hash(&in->content_hash,(const BYTE *)in->checking,GetSecurityDescriptorLength(in->checking),digest)){
  if(v){v->hash_status=(DWORD)in->content_hash.operation_status;v->hash_cleanup_status=(DWORD)in->content_hash.cleanup_status;}
  if(in->content_hash.retained)o->unresolved=1;
  return v?scqc_interval_error(v,5U,ERROR_INVALID_SECURITY_DESCR):ERROR_INVALID_SECURITY_DESCR;
 }
 if(v){v->hash_status=(DWORD)in->content_hash.operation_status;v->hash_cleanup_status=(DWORD)in->content_hash.cleanup_status;}
 e=scqc_interval_sample(o,v,5U);if(e)return e;
 if(LocalFree(in->checking)){o->unresolved=1;return v?scqc_interval_error(v,6U,ERROR_IO_INCOMPLETE):ERROR_IO_INCOMPLETE;}in->checking=NULL;
 return scqc_interval_sample(o,v,6U);
}
/* Diagnostic projection AFTER the unchanged unbound predicate rejects.
 * No field value, SID, token, reference or secret is emitted. This projection
 * is never an admission predicate and cannot turn a rejection into PASS. */
static DWORD scqc_unbound_reason(const PAPACC_SCQ_PEER_DATA *p,DWORD *field)
{
 unsigned i;SID_IDENTIFIER_AUTHORITY nt=SECURITY_NT_AUTHORITY;PSID sid=(PSID)p->sid;
 *field=0;i=0;
#define SCQC_UNBOUND_FIELD(v) do{i++;if(p->v){*field=i;return 1U;}}while(0)
 SCQC_UNBOUND_FIELD(pid);
 SCQC_UNBOUND_FIELD(creation[0]);
 SCQC_UNBOUND_FIELD(creation[1]);
 SCQC_UNBOUND_FIELD(token_id[0]);
 SCQC_UNBOUND_FIELD(token_id[1]);
 SCQC_UNBOUND_FIELD(authentication[0]);
 SCQC_UNBOUND_FIELD(authentication[1]);
 SCQC_UNBOUND_FIELD(modified[0]);
 SCQC_UNBOUND_FIELD(modified[1]);
 SCQC_UNBOUND_FIELD(groups);
 SCQC_UNBOUND_FIELD(enumerated);
 SCQC_UNBOUND_FIELD(restricted);
 SCQC_UNBOUND_FIELD(elevated);
 SCQC_UNBOUND_FIELD(elevation_type);
 SCQC_UNBOUND_FIELD(matches);
 SCQC_UNBOUND_FIELD(attributes);
 SCQC_UNBOUND_FIELD(token_type);
 SCQC_UNBOUND_FIELD(query_error);
 SCQC_UNBOUND_FIELD(wait_status);
 SCQC_UNBOUND_FIELD(exit_code);
 SCQC_UNBOUND_FIELD(session);
 SCQC_UNBOUND_FIELD(logon);
 SCQC_UNBOUND_FIELD(administrator_attributes);
 SCQC_UNBOUND_FIELD(service_type);
 SCQC_UNBOUND_FIELD(service_state);
 SCQC_UNBOUND_FIELD(service_sid_type);
 SCQC_UNBOUND_FIELD(image[0]);
 SCQC_UNBOUND_FIELD(image[1]);
 SCQC_UNBOUND_FIELD(image[2]);
#undef SCQC_UNBOUND_FIELD
 if(scqp_nonzero(p->image_digest,32))return 2U;
 if(p->sid_bytes<8U||p->sid_bytes>68U||p->sid[1]>SID_MAX_SUB_AUTHORITIES||8U+4U*p->sid[1]!=p->sid_bytes)return 3U;
 if(!IsValidSid(sid)||GetLengthSid(sid)!=p->sid_bytes)return 4U;
 for(i=p->sid_bytes;i<68U;i++)if(p->sid[i]){*field=i;return 5U;}
 if(memcmp(GetSidIdentifierAuthority(sid),&nt,sizeof(nt))||*GetSidSubAuthorityCount(sid)!=6U||*GetSidSubAuthority(sid,0)!=80U)return 6U;
 return 7U; /* discrepancy stays UNKNOWN, never permissive */
}
static DWORD scqc_validate_final(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_PLAN_DATA *plan)
{
 PAPACC_SCQ_COLD_DIAGNOSTIC *d=&o->pending_commit.personal_native.scq_cold_diagnostic;
 d->plan_attempted=1U;
 if(!scqp_static(plan)){d->plan_rejected=1U;goto rejected;}d->plan_passed|=1U;
 if(!scqp_unbound(&plan->runtime)){
  d->plan_rejected=2U;d->unbound_reason=scqc_unbound_reason(&plan->runtime,&d->unbound_field);goto rejected;
 }d->plan_passed|=2U;
 /* Same single GetTickCount64, same position after static/unbound and after
  * the existing stdout marker; do not move it earlier to buy lifetime. */
 d->plan_from=((ULONGLONG)plan->valid_from[1]<<32)|plan->valid_from[0];
 d->plan_until=((ULONGLONG)plan->valid_until[1]<<32)|plan->valid_until[0];
 d->plan_now=GetTickCount64();d->window_known=1U;
 if(!(d->plan_until>d->plan_from&&d->plan_now>=d->plan_from&&d->plan_now<d->plan_until)){d->plan_rejected=3U;goto rejected;}
 d->plan_passed|=4U;return 0;
rejected:return scqc_first_cause(o,9U,3U,ERROR_INVALID_DATA,0);
}
/* cbSid is buffer capacity, not a promised semantic length on success.
 * Admit only the untouched, bounded, valid service SID returned by the SAME
 * original lookup. Never trim/copy/clear invalid bytes to satisfy unbound. */
#include "personal_service_sid_win32.inc"
static DWORD scqc_plan_variant(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_COLD_START_INPUT *input,PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_SCQ_READ_COLD_INPUT *read)
{
 PAPACC_CONFIG_PENDING *pending=&o->pending_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&pending->personal_native;
 PAPACC_PERSONAL_SCQ_INPUTS *in=&n->scq_inputs;PAPACC_SCQ_PEER_DATA *peer=&plan->administrator;
 WCHAR image[MAX_PATH],process_name[1024],file_name[1024],domain[128];DWORD z,bytes,exit_code;
 BYTE g3_preimage[PAPACC_SCQ_G3_PREIMAGE_BYTES];
 BY_HANDLE_FILE_INFORMATION info[3];FILETIME created,exited,kernel,user;
 union {TOKEN_GROUPS align;BYTE bytes[8192];} groups;TOKEN_GROUPS *g=(TOKEN_GROUPS *)groups.bytes;
 TOKEN_ELEVATION elevation;TOKEN_ELEVATION_TYPE elevation_type;ULONGLONG now;unsigned i;
 PAPACC_SCQ_EXISTING_SELECTION selected;const PAPACC_SCQ_EXISTING_SELECTION *selection;
 if(read&&(&read->cold!=input||!scqp_key(read->key)||!scqp_nonzero(read->public_id,32)||(!input->existing&&input->existing_console_data!=1U)))return ERROR_ACCESS_DENIED;
 if(!input||!input->authorization_ms||!scqc_local_path(input->request_path)||!scqc_local_path(input->decision_path))return ERROR_INVALID_DATA;
 if(!scqg_selected_exact_mode(&input->limits,input->authorization_ms,input->existing_console_data==1U||input->existing!=NULL))return ERROR_INVALID_DATA;
 for(i=0;i<3U;i++)if(!scqp_nonzero(input->pins[i],32))return ERROR_INVALID_DATA;
 if(!input->limits.private_growth||!input->limits.working_growth||!input->limits.physical_floor||!input->limits.commit_floor||
  !input->limits.max_handles||input->limits.max_handles>PAPACC_SCQ_HANDLE_CAP(input->existing_console_data==1U||input->existing!=NULL)||!input->limits.freshness_ms||!input->limits.duration_ms)return ERROR_INVALID_DATA;
 memset(plan,0,sizeof(*plan));
 if(papacc_server_console_win32_stop_requested()||!personal_token(pending,1))return ERROR_ACCESS_DENIED;
 selection=input->existing;
 if(input->existing_console_data){
  HANDLE console_in=GetStdHandle(STD_INPUT_HANDLE),console_out=GetStdHandle(STD_OUTPUT_HANDLE);DWORD mode;char line[256];
  if(input->existing_console_data!=1U||selection||!n->console.is_installed||
   !GetConsoleMode(console_in,&mode)||!GetConsoleMode(console_out,&mode)||
   !personal_console_write(console_out,read?"P1 MACHINE_READ DATA ONLY: copy the exact original installed-object tuple; not E/Q/S/P authority.\r\n":"P1 SELECTOR DATA ONLY: copy the TO/AMD64 object tuple and provenance from the NEW approved original request. Not an approval; E/Q/S remain separate.\r\n")||
   !personal_console_line(console_in,console_out,line,sizeof(line)))return ERROR_ACCESS_DENIED;
  z=scqc_existing_data_parse_mode(line,&selected,read!=NULL);if(z)return z;selection=&selected;
 }
 z=read?papacc_personal_paths_selected_machine_read(&n->install_paths,selection,1U,&in->image_chain,&pending->stop,read->public_id):selection?papacc_personal_paths_selected(&n->install_paths,selection,1U,&in->image_chain,&pending->stop):
  papacc_personal_paths_binary(&n->install_paths,&pending->stop);if(z)return z;
 if(wcscpy_s(image,MAX_PATH,n->install_paths.binary)||wcscat_s(image,MAX_PATH,L"\\papacc_server.exe"))return ERROR_BUFFER_OVERFLOW;
 for(i=0;i<3U;i++){
  const WCHAR *path=i==0U?input->request_path:i==1U?input->decision_path:image;
  if(pending->stop||papacc_server_console_win32_stop_requested())return ERROR_CANCELLED;
  z=scqc_open_raw(o,i,path);if(z)return z;
  if(!n->scq_cold_raw[i]||GetFileType(n->scq_cold_raw[i])!=FILE_TYPE_DISK||!GetFileInformationByHandle(n->scq_cold_raw[i],&info[i]))return ERROR_INVALID_HANDLE;
  plan->refs[i].role=i+1U;plan->refs[i].rights=PAPACC_SCQ_FILE_READ_RIGHTS;
  plan->refs[i].volume=info[i].dwVolumeSerialNumber;plan->refs[i].high=info[i].nFileIndexHigh;plan->refs[i].low=info[i].nFileIndexLow;
  plan->refs[i].size_low=info[i].nFileSizeLow;plan->refs[i].size_high=info[i].nFileSizeHigh;
  plan->refs[i].attributes=info[i].dwFileAttributes;plan->refs[i].links=info[i].nNumberOfLinks;
  memcpy(plan->refs[i].digest,input->pins[i],32);
  z=scqc_descriptor_digest(o,i,plan->refs[i].descriptor,NULL);if(z)return z;
 }
 z=1024U;if(!QueryFullProcessImageNameW(GetCurrentProcess(),PROCESS_NAME_NATIVE,process_name,&z)||!z||z>=1024U)return ERROR_ACCESS_DENIED;
 bytes=GetFinalPathNameByHandleW(n->scq_cold_raw[2],file_name,1024U,FILE_NAME_NORMALIZED|VOLUME_NAME_NT);
 if(!bytes||bytes>=1024U||bytes!=z||CompareStringOrdinal(process_name,(int)z,file_name,(int)bytes,FALSE)!=CSTR_EQUAL)return ERROR_ACCESS_DENIED;
 plan->version=1U;plan->profile=PAPACC_SCQ_PROFILE;plan->purpose=read?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;if(read)memcpy(plan->key,read->key,112);
 plan->schema=PAPACC_SCQ_NON_SECRET_SCHEMA;plan->service=PAPACC_SCQ_SERVICE_PAPINHO;
 now=GetTickCount64();if(now>((ULONGLONG)-1)-input->authorization_ms)return ERROR_ARITHMETIC_OVERFLOW;
 plan->valid_from[0]=(DWORD)now;plan->valid_from[1]=(DWORD)(now>>32);now+=input->authorization_ms;
 plan->valid_until[0]=(DWORD)now;plan->valid_until[1]=(DWORD)(now>>32);
 if(pending->stop||papacc_server_console_win32_stop_requested())return ERROR_CANCELLED;
 if(BCryptGenRandom(NULL,plan->attempt,32,BCRYPT_USE_SYSTEM_PREFERRED_RNG))return ERROR_GEN_FAILURE;
 if((read?scqk_policy_data(&input->limits,g3_preimage):scqg_plan_data_mode(&input->limits,input->authorization_ms,g3_preimage,PAPACC_SCQ_P1_PATHS(&n->install_paths)))||
  !papacc_store_hash(&in->content_hash,g3_preimage,sizeof(g3_preimage),plan->policy)){
  if(in->content_hash.retained)o->unresolved=1;return ERROR_INVALID_DATA;
 }
 memcpy(plan->request,input->pins[0],32);memcpy(plan->decision,input->pins[1],32);
 if(!papacc_store_hash(&in->content_hash,(const BYTE *)&info[2],sizeof(info[2]),plan->target)){if(in->content_hash.retained)o->unresolved=1;return ERROR_INVALID_DATA;}
 peer->pid=GetCurrentProcessId();
 z=scqc_process_times(o,&created,&exited,&kernel,&user);if(z)return z;
 z=scqc_process_exit(o,&exit_code);if(z)return z;
 memcpy(peer->creation,&created,sizeof(created));memcpy(peer->token_id,&n->token.TokenId,sizeof(LUID));
 memcpy(peer->authentication,&n->token.AuthenticationId,sizeof(LUID));memcpy(peer->modified,&n->token.ModifiedId,sizeof(LUID));
 peer->sid_bytes=n->sid_bytes;memcpy(peer->sid,n->sid,n->sid_bytes);peer->token_type=n->token.TokenType;
 peer->session=n->session;peer->logon=2U;peer->wait_status=WaitForSingleObject(GetCurrentProcess(),0);peer->exit_code=exit_code;
 peer->image[0]=info[2].dwVolumeSerialNumber;peer->image[1]=info[2].nFileIndexHigh;peer->image[2]=info[2].nFileIndexLow;memcpy(peer->image_digest,input->pins[2],32);
 if(!GetTokenInformation(pending->bootstrap_token,TokenGroups,&groups,sizeof(groups),&bytes)||bytes>sizeof(groups)||bytes<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||
  g->GroupCount!=n->token.GroupCount||g->GroupCount>(bytes-(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES))return ERROR_INVALID_DATA;
 peer->groups=peer->enumerated=g->GroupCount;
 for(i=0;i<g->GroupCount;i++){
  if(!pri_sid_inside(groups.bytes,bytes,g->Groups[i].Sid))return ERROR_INVALID_SID;
  if(IsWellKnownSid(g->Groups[i].Sid,WinBuiltinAdministratorsSid)){peer->matches++;peer->attributes=peer->administrator_attributes=g->Groups[i].Attributes;}
 }
 peer->restricted=IsTokenRestricted(pending->bootstrap_token)?1U:0U;
 if(!GetTokenInformation(pending->bootstrap_token,TokenElevation,&elevation,sizeof(elevation),&z)||z!=sizeof(elevation)||
  !GetTokenInformation(pending->bootstrap_token,TokenElevationType,&elevation_type,sizeof(elevation_type),&z)||z!=sizeof(elevation_type))return ERROR_INVALID_DATA;
 peer->elevated=elevation.TokenIsElevated;peer->elevation_type=elevation_type;
 z=scqc_runtime_sid(&plan->runtime,domain);if(z)return z;
 if(selection){PAPACC_SCQ_IMAGE_CHAIN *selected_chain=&in->image_chain;
  z=scqi_chain_capture(selected_chain,o,&n->install_paths,plan,&info[2],(PSID)n->sid,&pending->stop);if(z)return z;
  memcpy(plan->target,selected_chain->plan.target,32);
  (void)fprintf(stdout,"EXISTING_TARGET_CANDIDATE root_expected=%lu:%lu:%lu root_observed=%lu:%lu:%lu parent_expected=%lu:%lu:%lu image_expected=%lu:%lu:%lu; ORIGINALS_AND_THREE_ATTRIBUTED_ACTS_STILL_REQUIRED; NOT_CURRENT_WINDOWS_MAPPING\n",
   (unsigned long)n->install_paths.selected.identity[0],(unsigned long)n->install_paths.selected.identity[1],(unsigned long)n->install_paths.selected.identity[2],
   (unsigned long)selected_chain->identity[selected_chain->count-3U].dwVolumeSerialNumber,(unsigned long)selected_chain->identity[selected_chain->count-3U].nFileIndexHigh,(unsigned long)selected_chain->identity[selected_chain->count-3U].nFileIndexLow,
   (unsigned long)n->install_paths.selected.identity[3],(unsigned long)n->install_paths.selected.identity[4],(unsigned long)n->install_paths.selected.identity[5],
   (unsigned long)n->install_paths.selected.identity[6],(unsigned long)n->install_paths.selected.identity[7],(unsigned long)n->install_paths.selected.identity[8]);
 }
 return scqc_validate_final(o,plan);
}
static DWORD scqc_plan(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_COLD_START_INPUT *input,PAPACC_SCQ_PLAN_DATA *plan)
{return scqc_plan_variant(o,input,plan,NULL);}
#include "personal_scq_cold_close_win32.inc"
#include "personal_scq_interval_observe_win32.inc"
