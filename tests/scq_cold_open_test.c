/* Actual native raw-open helper with Win32/NT calls modeled. No file opens. */
#define PAPACC_NATIVE_SELECTION_ENTRY preserved_cold_open_selection_main
#include "scq_native_selection_capture_test.c"
#include <sddl.h>
#undef LocalFree
static HLOCAL WINAPI interval_real_free(HLOCAL p){return LocalFree(p);}
static unsigned raw_fault,raw_calls,raw_queries,raw_closes,interval_bad_query,interval_stop_query;static DWORD raw_native_error;static PAPACC_CONFIGURATION_COMMIT raw_owner;
static PSECURITY_DESCRIPTOR interval_sd;
static UINT WINAPI raw_drive(LPCWSTR path){(void)path;return raw_fault==4U?DRIVE_REMOTE:DRIVE_FIXED;}
static DWORD envelope_count;static int envelope_override;
static BOOL WINAPI raw_count(HANDLE process,PDWORD count){C(process==GetCurrentProcess());raw_queries++;if(envelope_override){*count=envelope_count;return envelope_override==1;}
 if(raw_fault>=60U){*count=(raw_fault==61U?112U:100U)+(raw_calls?1U:0U)+(raw_queries>=6U?12U:0U);if(raw_fault==62U)*count=124U;if(raw_fault==63U)*count=128U;
  if(raw_fault==72U&&raw_queries==2U)*count=124U;if(raw_fault==73U&&raw_queries==2U)*count=129U;
  if(raw_fault==74U&&raw_queries==5U)*count=124U;if(raw_fault==75U&&raw_queries==6U)*count=129U;
  if(raw_fault>=76U&&raw_fault<=79U&&raw_queries==5U)*count=128U;
  if(raw_queries==interval_stop_query)raw_owner.pending_commit.stop=1;return raw_queries!=interval_bad_query;}
 *count=raw_fault>=40U?113U:raw_fault==5U?124U:raw_fault==13U?123U:raw_fault==14U?128U:raw_fault==27U?136U:100U;return raw_fault<15U||raw_fault>17U;}
static DWORD WINAPI raw_error(void){return raw_native_error;}
static unsigned raw_maps;
static DWORD WINAPI raw_device(LPCWSTR drive,LPWSTR output,DWORD capacity)
{
 const WCHAR *mapped=raw_fault==51U?L"\\??\\D:\\alias":raw_fault==52U?L"\\Device\\Volume\\subdir":raw_fault==53U?L"\\Device\\":L"\\Device\\HarddiskVolume3";
 C(!wcscmp(drive,L"C:"));raw_maps++;
 if(raw_fault==50U)return 0;
 wcscpy_s(output,capacity,mapped);return (DWORD)wcslen(mapped)+2U;
}
#define QueryDosDeviceW raw_device
static DWORD WINAPI raw_sd(HANDLE h,SE_OBJECT_TYPE type,SECURITY_INFORMATION info,PSID *owner,PSID *group,PACL *dacl,PACL *sacl,PSECURITY_DESCRIPTOR *sd)
{(void)h;(void)type;(void)info;(void)owner;(void)group;(void)dacl;(void)sacl;
 if(raw_fault>=60U){if(raw_fault==64U){*sd=NULL;return ERROR_ACCESS_DENIED;}C(ConvertStringSecurityDescriptorToSecurityDescriptorA("O:BAG:BAD:P(A;;FA;;;BA)",SDDL_REVISION_1,sd,NULL));interval_sd=*sd;if(raw_fault==79U)raw_owner.unresolved=1;return 0;}
 *sd=raw_fault==21U?(PSECURITY_DESCRIPTOR)(ULONG_PTR)1:NULL;return raw_native_error;}
static HLOCAL WINAPI interval_free(HLOCAL h){if(raw_fault==67U||raw_fault==77U)return h;if(h==interval_sd)interval_sd=NULL;return LocalFree(h);}
static BOOL WINAPI interval_close(HANDLE h){C(h==(HANDLE)71);raw_closes++;return raw_fault!=68U&&raw_fault!=78U;}
static DWORD WINAPI interval_module_name(HMODULE module,LPWSTR out,DWORD cap){(void)module;wcscpy_s(out,cap,L"C:\\workspace\\papacc_server.exe");return (DWORD)wcslen(out);}
static DWORD WINAPI interval_type(HANDLE h){C(h==(HANDLE)71);return FILE_TYPE_DISK;}
static BOOL WINAPI interval_info(HANDLE h,LPBY_HANDLE_FILE_INFORMATION f){C(h==(HANDLE)71);memset(f,0,sizeof(*f));f->dwVolumeSerialNumber=417614100U;f->nFileIndexHigh=1441792U;f->nFileIndexLow=raw_fault==69U?999U:542844U;f->nFileSizeLow=16483U;f->nNumberOfLinks=1;return TRUE;}
static BOOL WINAPI raw_times(HANDLE h,LPFILETIME a,LPFILETIME b,LPFILETIME c,LPFILETIME d)
{(void)h;(void)a;(void)b;(void)c;(void)d;return raw_fault!=22U;}
static BOOL WINAPI raw_exit(HANDLE h,LPDWORD code){(void)h;*code=0;return raw_fault!=23U;}
static NTSTATUS NTAPI raw_open(PHANDLE out,ACCESS_MASK rights,POBJECT_ATTRIBUTES a,PIO_STATUS_BLOCK io,PLARGE_INTEGER size,ULONG attributes,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_bytes)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;(void)size;(void)attributes;
 raw_calls++;
 if(rights!=PAPACC_SCQ_FILE_READ_RIGHTS||a!=&n->scq_cold_attributes||a->ObjectName!=&n->scq_cold_unicode||
  a->ObjectName->Buffer!=n->scq_cold_name||a->RootDirectory||a->Attributes!=(OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE)||
  io!=&n->scq_cold_io||share!=FILE_SHARE_READ||disposition!=1U||options!=0x00200060U||ea||ea_bytes)return (NTSTATUS)0xc000000dL;
 if(raw_fault>=60U){C(!wcscmp(a->ObjectName->Buffer,L"\\Device\\HarddiskVolume3\\Projetos\\PapinhoAccelerator\\docs\\integration\\phase6-server-update-after-utility-20261101\\FIRST-P1-CHAIN-SESSION-REQUEST.ORIGINAL.md"));if(raw_fault==70U)return (NTSTATUS)0xc000050bL;if(raw_fault==71U){*out=(HANDLE)71;io->Status=(NTSTATUS)0x103L;return (NTSTATUS)0x103L;}}
 if(raw_fault==3U)return (NTSTATUS)0xc000050bL; /* reparse: never followed */
 if(raw_fault==40U)return (NTSTATUS)0xc0000022L;
 if(raw_fault==41U)return (NTSTATUS)0xc0000043L;
 if(raw_fault==42U)return (NTSTATUS)0xc000050bL;
 if(raw_fault==43U){io->Status=(NTSTATUS)0xc0000022L;return 0;}
 if(raw_fault==44U){io->Information=0;return 0;}
 *out=(HANDLE)71;io->Status=0;io->Information=1;
 if(raw_fault==8U){io->Status=(NTSTATUS)0x103L;return (NTSTATUS)0x103L;}
 if(raw_fault==9U)return (NTSTATUS)0xc0000022L;
 if(raw_fault==10U)io->Information=2;
 return 0;
}
static HMODULE WINAPI raw_module(LPCWSTR path){(void)path;return raw_fault==6U?NULL:(HMODULE)1;}
static FARPROC WINAPI raw_address(HMODULE module,LPCSTR name)
{FARPROC address=NULL;NTSTATUS (NTAPI *f)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG)=raw_open;if(module&&raw_fault!=7U&&!strcmp(name,"NtCreateFile"))memcpy(&address,&f,sizeof(address));return address;}
#define GetDriveTypeW raw_drive
#undef GetProcessHandleCount
#define GetProcessHandleCount raw_count
#define GetModuleHandleW raw_module
#define GetProcAddress raw_address
#undef GetModuleFileNameW
#define GetModuleFileNameW interval_module_name
#undef GetFileType
#define GetFileType interval_type
#undef GetFileInformationByHandle
#define GetFileInformationByHandle interval_info
#undef LocalFree
#define LocalFree interval_free
#undef CloseHandle
#define CloseHandle interval_close
#undef GetLastError
#define GetLastError raw_error
#undef GetSecurityInfo
#define GetSecurityInfo raw_sd
#undef GetProcessTimes
#define GetProcessTimes raw_times
#undef GetExitCodeProcess
#define GetExitCodeProcess raw_exit
static PAPACC_RESULT observe_console(PAPACC_SERVER_CONSOLE_WIN32 *c)
{if(raw_fault==24U)return PAPACC_RESULT_INTERNAL_ERROR;c->is_installed=PAPACC_TRUE;return PAPACC_RESULT_OK;}
static int observe_token(PAPACC_CONFIG_PENDING *p,int capture)
{C(capture==1);if(raw_fault==25U||p->stop)return 0;p->bootstrap_token=(HANDLE)(ULONG_PTR)77U;return 1;}
static DWORD observe_paths(PAPACC_PERSONAL_INSTALL_PATHS *p,volatile LONG *stop)
{(void)p;(void)stop;return raw_fault==26U?ERROR_PATH_NOT_FOUND:0;}
#undef personal_token
#define personal_token observe_token
#define papacc_personal_paths_binary observe_paths
#define papacc_server_console_win32_install observe_console
static DWORD cold_original_chain(PAPACC_SCQ_IMAGE_CHAIN *chain,const void *owner,const PAPACC_PERSONAL_INSTALL_PATHS *paths,
 const PAPACC_SCQ_PLAN_DATA *plan,const BY_HANDLE_FILE_INFORMATION *image,PSID actor,volatile LONG *stop)
{(void)chain;(void)owner;(void)paths;(void)plan;(void)image;(void)actor;(void)stop;return ERROR_ACCESS_DENIED;}
static unsigned p1_reads,p1_selected,p1_read_line,p1_read_selected;
static BOOL WINAPI p1_console_mode(HANDLE h,LPDWORD mode){(void)h;*mode=0;return raw_fault!=36U;}
static HANDLE WINAPI p1_std(DWORD id){return (HANDLE)(ULONG_PTR)id;}
static int p1_write(HANDLE h,const char *s){(void)h;(void)s;return 1;}
static int p1_line(HANDLE in,HANDLE out,char *line,size_t cap)
{(void)in;(void)out;p1_reads++;if(raw_fault==37U)return 0;
 if(raw_fault==39U){line[0]=0;return 1;}
 if(p1_read_line){strcpy_s(line,cap,"P1 MACHINE_READ AMD64 7:0:1 7:0:2 7:0:3 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87");return 1;}
 strcpy_s(line,cap,raw_fault==35U?"P1 READ AMD64 7:0:1 7:0:2 7:0:3 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87":
 "P1 TRANSPORT_ONLY AMD64 7:0:1 7:0:2 7:0:3 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87");return 1;}
static DWORD p1_paths(PAPACC_PERSONAL_INSTALL_PATHS *p,const PAPACC_SCQ_EXISTING_SELECTION *s,DWORD role,const void *chain,volatile LONG *stop)
{(void)p;(void)chain;(void)stop;p1_selected++;C(role==1U&&s->architecture==0x8664U&&s->identity[8]==3U);return ERROR_NOT_READY;}
#define GetConsoleMode p1_console_mode
#define GetStdHandle p1_std
#define personal_console_write p1_write
#define personal_console_line p1_line
#define papacc_personal_paths_selected p1_paths
static DWORD p1_read_paths(PAPACC_PERSONAL_INSTALL_PATHS *p,const PAPACC_SCQ_EXISTING_SELECTION *s,DWORD r,const void *c,volatile LONG *st,const BYTE pub[32])
{C(pub[0]==7U);p1_read_selected++;return p1_paths(p,s,r,c,st);}
#define papacc_personal_paths_selected_machine_read p1_read_paths
static int p1_hash(PAPACC_STORE_HASH *h,const BYTE *bytes,unsigned n,BYTE digest[32])
{(void)bytes;(void)n;
 if(raw_fault>=60U){if(raw_fault==65U){h->operation_status=(NTSTATUS)0xc0000001L;return 0;}if(raw_fault==66U){h->retained=1;h->hash=(BCRYPT_HASH_HANDLE)1;h->algorithm=(BCRYPT_ALG_HANDLE)2;h->cleanup_status=(NTSTATUS)0xc0000001L;return 0;}memset(digest,42,32);return 1;}
 (void)h;(void)digest;return 0;} /* normal plan tests stop before hashing/RNG */
#define papacc_store_hash p1_hash
#define scqi_chain_capture cold_original_chain
static unsigned runtime_sid_fault,runtime_sid_lookups;
static BOOL WINAPI runtime_sid_lookup(LPCWSTR system,LPCWSTR name,PSID sid,LPDWORD capacity,LPWSTR domain,LPDWORD domain_capacity,PSID_NAME_USE use)
{
 static const BYTE runtime_sid_fixture_bytes[32]={1,6,0,0,0,0,0,5,80,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,4,0,0,0,5,0,0,0};
 BYTE *b=sid;C(!system&&!wcscmp(name,L"NT SERVICE\\PapinhoAccelerator")&&*capacity==68U&&*domain_capacity==128U);
 runtime_sid_lookups++;(void)domain;
 if(runtime_sid_fault==2U)return FALSE;
 memcpy(b,runtime_sid_fixture_bytes,sizeof(runtime_sid_fixture_bytes));*use=runtime_sid_fault==3U?SidTypeGroup:SidTypeWellKnownGroup;
 if(runtime_sid_fault==1U)*capacity=32U; /* canonical size returned */
 /* fault0 deliberately leaves input capacity68 unchanged on success. */
 if(runtime_sid_fault==4U)*capacity=0U;if(runtime_sid_fault==5U)*capacity=69U;
 if(runtime_sid_fault==6U)*capacity=16U;if(runtime_sid_fault==7U)b[1]=16U;
 if(runtime_sid_fault==8U)b[0]=0;if(runtime_sid_fault==9U)b[32]=1U;
 if(runtime_sid_fault==10U)b[7]=6U;if(runtime_sid_fault==11U)b[8]=81U;
 if(runtime_sid_fault==12U){b[1]=5U;memset(b+28,0,4);}
 if(runtime_sid_fault==13U)*capacity=40U;
 if(runtime_sid_fault==14U){memset(b,0,68);*capacity=68U;}
 return TRUE;
}
#undef LookupAccountNameW
#define LookupAccountNameW runtime_sid_lookup
static ULONGLONG final_plan_now;static unsigned final_plan_ticks;
static ULONGLONG WINAPI final_plan_tick(void){final_plan_ticks++;return final_plan_now;}
#undef GetTickCount64
#define GetTickCount64 final_plan_tick
#include "../apps/server/personal_scq_cold_acquisition_win32.inc"
int main(void)
{
 unsigned i,before=checks;DWORD e;WCHAR path[]=L"C:\\request.md";
 for(i=0;i<18U;i++){
  memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();raw_owner.pending_commit.personal_native.phase=1U;
  raw_fault=i;raw_calls=raw_queries=0;raw_native_error=i==15U?ERROR_ACCESS_DENIED:i==16U?0:ERROR_NOT_ENOUGH_QUOTA;
  if(i==11U)raw_owner.pending_commit.stop=1;
  if(i==12U)raw_owner.writer++;
  e=scqc_open_raw(&raw_owner,0U,i==1U?L"relative":i==2U?L"\\\\remote\\share\\request.md":path);
  if(!i||i==13U){C(!e&&raw_calls==1U&&raw_queries==1U&&raw_owner.pending_commit.personal_native.scq_cold_raw[0]==(HANDLE)71);C(raw_owner.pending_commit.personal_native.scq_cold_diagnostic.query_succeeded&&raw_owner.pending_commit.personal_native.scq_cold_diagnostic.own_count==(i==13U?123U:100U));continue;}
  C(e!=0);
  if(i>=8U&&i<=10U){C(raw_owner.unresolved&&raw_owner.pending_commit.personal_native.scq_cold_raw[0]);C(scqc_open_raw(&raw_owner,1U,path)!=0&&raw_calls==1U);}
  if(i==1U||i==2U||i==4U||i==5U||i==6U||i==7U||i==11U||i==12U)C(!raw_calls);
  if(i==5U||i==14U){C(e==ERROR_NOT_ENOUGH_QUOTA&&!raw_calls&&raw_queries==1U);C(raw_owner.pending_commit.personal_native.scq_cold_diagnostic.stage==2U&&raw_owner.pending_commit.personal_native.scq_cold_diagnostic.query_succeeded&&raw_owner.pending_commit.personal_native.scq_cold_diagnostic.own_count==(i==5U?124U:128U));}
  if(i>=15U){PAPACC_SCQ_COLD_DIAGNOSTIC saved=raw_owner.pending_commit.personal_native.scq_cold_diagnostic;C(e==(raw_native_error?raw_native_error:ERROR_GEN_FAILURE)&&!raw_calls&&raw_queries==1U);C(saved.stage==1U&&saved.reference==0&&!saved.query_succeeded&&saved.native_error==raw_native_error);C(scqc_first_cause(&raw_owner,3U,2U,ERROR_INVALID_DATA,99U)==ERROR_INVALID_DATA);C(!memcmp(&saved,&raw_owner.pending_commit.personal_native.scq_cold_diagnostic,sizeof(saved)));}
 }
 for(i=0;i<3U;i++){
  PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;
  memset(&raw_owner,0,sizeof(raw_owner));raw_native_error=ERROR_NOT_ENOUGH_QUOTA;raw_fault=i?21U:20U;
  if(i==2U){raw_native_error=0;raw_fault=20U;}
  e=scqc_descriptor(&raw_owner,i);
  C(e==(DWORD)(i==2U?ERROR_INVALID_SECURITY_DESCR:ERROR_NOT_ENOUGH_QUOTA));
  C(n->scq_cold_diagnostic.stage==3U&&n->scq_cold_diagnostic.reference==i&&n->scq_cold_diagnostic.native_error==raw_native_error);
  C((raw_owner.unresolved!=0)==(i==1U)&&((n->scq_inputs.checking!=NULL)==(i==1U)));
 }
 for(i=0;i<4U;i++){
  FILETIME a,b,c,d;DWORD code;
  memset(&raw_owner,0,sizeof(raw_owner));raw_fault=i<2U?22U:23U;raw_native_error=(i&1U)?0:ERROR_NOT_ENOUGH_QUOTA;
  e=i<2U?scqc_process_times(&raw_owner,&a,&b,&c,&d):scqc_process_exit(&raw_owner,&code);
  C(e==(raw_native_error?raw_native_error:ERROR_GEN_FAILURE));C(raw_owner.pending_commit.personal_native.scq_cold_diagnostic.stage==(i<2U?4U:5U)&&raw_owner.pending_commit.personal_native.scq_cold_diagnostic.reference==3U);
 }
 for(i=0;i<7U;i++){
  DWORD samples[15]={0};PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;
  memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();raw_calls=raw_queries=0;
  raw_fault=i==1U?24U:i==2U?25U:i==3U?26U:i==4U?15U:i==6U?27U:0U;raw_native_error=ERROR_ACCESS_DENIED;
  if(i==5U)raw_owner.pending_commit.stop=1;
  e=papacc_configuration_personal_cold_handle_observe(&raw_owner,samples);
  C(!raw_calls&&!n->scq_cold_raw[0]&&!n->scq_cold_raw[1]&&!n->scq_cold_raw[2]&&!n->scq_admin.value.controller_consumed);
  if(!i||i==6U){C(!e&&raw_queries==3U&&samples[6]==1U&&samples[9]==1U&&samples[12]==1U&&samples[7]==(i==6U?136U:100U));}
  else {C(e!=0);C(raw_queries==(i==2U?1U:i==3U?2U:i==4U?1U:0U));}
  if(i==4U)C(!samples[6]&&samples[8]==ERROR_ACCESS_DENIED&&!samples[9]);
 }
 C(!mcreated&&!mopened&&!mevents&&!cs_running);
 {
  static const char *bad[]={"","P1 READ AMD64 7:0:1 7:0:2 7:0:3 00","P1 NEW_INSTALL AMD64 7:0:1 7:0:2 7:0:3 00",
   "P1 TRANSPORT_ONLY X86 7:0:1 7:0:2 7:0:3 00","P1 TRANSPORT_ONLY AMD64 4294967296:0:1 7:0:2 7:0:3 00",
   "P1 TRANSPORT_ONLY AMD64 -7:0:1 7:0:2 7:0:3 00","P1 TRANSPORT_ONLY AMD64 7:0:0 7:0:2 7:0:3 00"};
  PAPACC_SCQ_EXISTING_SELECTION value,saved;const char *valid="P1 TRANSPORT_ONLY AMD64 7:0:1 7:0:2 7:0:3 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87";
  C(!scqc_existing_data_parse(valid,&value)&&value.architecture==0x8664U&&value.identity[8]==3U);
  for(i=0;i<sizeof(bad)/sizeof(bad[0]);i++){
   memset(&value,0xa5,sizeof(value));saved=value;
   C(scqc_existing_data_parse(bad[i],&value)!=0&&!memcmp(&value,&saved,sizeof(value)));
  }
  C(scqc_existing_data_parse(NULL,&value)!=0&&scqc_existing_data_parse(valid,NULL)!=0);
 }
 for(i=0;i<6U;i++){
  PAPACC_SCQ_COLD_START_INPUT input;PAPACC_SCQ_PLAN_DATA plan;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;
  memset(&raw_owner,0,sizeof(raw_owner));memset(&input,0,sizeof(input));raw_owner.writer=GetCurrentThreadId();
  n->phase=1U;n->console.is_installed=PAPACC_TRUE;raw_calls=p1_reads=p1_selected=0;raw_fault=34U+i;
  input.request_path=L"C:\\request.md";input.decision_path=L"C:\\decision.md";
  memset(input.pins,1,sizeof(input.pins));scqg_selected_p1(&input.limits);input.authorization_ms=240000U;input.existing_console_data=1U;
  if(i==4U)input.existing_console_data=2U;
  e=scqc_plan(&raw_owner,&input,&plan);
  C(e!=0&&!raw_calls&&!n->scq_admin.value.controller_consumed&&!n->scq_admin.endpoint_consumed&&!n->scq_admin.start_consumed);
  C(p1_selected==(i?0U:1U));C(p1_reads==(i==2U||i==4U?0U:1U));
 }
 for(i=40U;i<=44U;i++){
  PAPACC_SCQ_COLD_DIAGNOSTIC *d=&raw_owner.pending_commit.personal_native.scq_cold_diagnostic;PAPACC_SCQ_COLD_DIAGNOSTIC saved;
  memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();raw_owner.pending_commit.personal_native.phase=1U;
  raw_fault=i;raw_queries=raw_calls=0;raw_native_error=777U;
  C(scqc_open_raw(&raw_owner,0U,path)==ERROR_ACCESS_DENIED);C(raw_queries==1U&&raw_calls==1U);
  C(d->stage==6U&&d->reference==0U&&d->mapped==ERROR_ACCESS_DENIED&&d->native_error==0U);
  C(d->query_succeeded&&d->own_count==113U&&d->open_attempted&&d->open_reference==0U&&!d->returned_handle);
  C(d->open_status==(i==40U?0xc0000022U:i==41U?0xc0000043U:i==42U?0xc000050bU:0U));
  C(d->io_status==(i==43U?0xc0000022U:0U)&&d->io_information==0U&&!raw_owner.unresolved);
  saved=*d;C(scqc_first_cause(&raw_owner,3U,2U,ERROR_INVALID_DATA,99U)==ERROR_INVALID_DATA&&!memcmp(&saved,d,sizeof(saved)));
 }
 for(i=0;i<12U;i++){
  PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;
  memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();n->phase=1U;
  n->install_paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;
  n->install_paths.selected_role=1U;n->install_paths.thread=GetCurrentThreadId();n->install_paths.selected.architecture=0x8664U;
  raw_fault=i>=1U&&i<=4U?49U+i:i==5U?3U:i==6U?5U:i==7U?14U:0U;raw_native_error=ERROR_ACCESS_DENIED;raw_maps=raw_calls=raw_queries=0;
  if(i==8U)n->install_paths.selected_role=2U;
  if(i==9U)n->install_paths.selected.architecture=0x14cU;
  if(i==10U)n->install_paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;
  e=scqc_open_raw(&raw_owner,0U,i==11U?L"C:\\a\\..\\request.md":path);
  if(!i){C(!e&&raw_maps==1U&&raw_queries==1U&&raw_calls==1U);C(!wcscmp(n->scq_cold_name,L"\\Device\\HarddiskVolume3\\request.md"));}
  else if(i==10U){C(!e&&!raw_maps&&raw_calls==1U);C(!wcscmp(n->scq_cold_name,L"\\??\\C:\\request.md"));}
  else {C(e!=0);if(i==5U){C(raw_calls==1U&&n->scq_cold_diagnostic.open_status==0xc000050bU);}else C(!raw_calls);}
  if(i==6U||i==7U)C(e==ERROR_NOT_ENOUGH_QUOTA&&raw_maps==1U&&raw_queries==1U);
  C(!n->scq_admin.endpoint_consumed&&!n->scq_admin.start_consumed&&!n->scq_admin.value.controller_consumed);
 }
 for(i=0;i<38U;i++){
  PAPACC_SCQ_INTERVAL_OBSERVATION v;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;unsigned k;
  memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();raw_calls=raw_queries=raw_closes=raw_maps=0;interval_bad_query=interval_stop_query=0;interval_sd=NULL;raw_native_error=ERROR_ACCESS_DENIED;
  raw_fault=i<12U?60U+i:i>=30U?72U+i-30U:60U;if(i>=12U&&i<21U)interval_bad_query=i-11U;if(i>=21U&&i<30U)interval_stop_query=i-20U;
  e=papacc_configuration_personal_request_interval_observe(&raw_owner,&v);
  C(!n->install_paths.phase&&!n->scq_cold_raw[1]&&!n->scq_cold_raw[2]&&!n->scq_inputs.image_chain.phase&&!n->scq_admin.value.controller_consumed&&!n->scq_admin.endpoint_consumed&&!n->scq_admin.start_consumed);
  C(raw_calls<=1U&&raw_maps<=2U&&raw_queries<=9U);C(papacc_configuration_personal_request_interval_observe(&raw_owner,&v)!=0);
  if(i==0U||i==1U){for(k=0;k<9U;k++)C(v.sample[k][0]);C(raw_queries==9U&&raw_calls==1U&&raw_closes==1U&&v.identity_checked);C(v.sample[4][3]==3U&&v.sample[5][3]==3U&&v.sample[6][3]==1U);C(i?e==ERROR_NOT_ENOUGH_QUOTA&&v.final_guard&&v.sample[8][1]==125U:!e);}
  else C(e!=0);
  if(i==2U||i==3U||i==12U||i==13U)C(!raw_calls);
  if(i==10U)C(n->scq_cold_diagnostic.open_status==0xc000050bU&&!raw_closes);
  if(i==11U)C(raw_owner.unresolved&&!raw_closes&&n->scq_cold_diagnostic.open_status==0x103U);
  if(i==5U)C(v.hash_status==0xc0000001U);if(i==6U)C(v.hash_cleanup_status==0xc0000001U&&raw_owner.unresolved);
  if(i==8U)C(raw_closes==1U&&raw_owner.unresolved);
  if(i==30U||i==31U)C(!raw_calls&&v.first_cut==1U&&e==ERROR_NOT_ENOUGH_QUOTA);
  if(i==32U||i==34U){C(!v.hash_called&&v.first_cut==4U&&!raw_owner.unresolved&&!n->scq_inputs.checking&&!n->scq_cold_raw[0]&&raw_closes==1U);C(e==ERROR_NOT_ENOUGH_QUOTA&&raw_queries==5U&&!v.final_guard);}
  if(i==33U)C(v.hash_called&&v.first_cut==5U&&raw_owner.unresolved);
  if(i>=34U){C(v.sample[4][0]&&v.sample[4][1]==128U&&v.sample[4][3]==3U&&!v.sample[5][0]);C(e==ERROR_NOT_ENOUGH_QUOTA&&v.first_cut==4U&&!v.hash_called);}
  if(i==35U)C(raw_owner.unresolved&&n->scq_inputs.checking&&raw_closes==0U);
  if(i==36U)C(raw_owner.unresolved&&!n->scq_inputs.checking&&raw_closes==1U&&n->scq_cold_raw[0]);
  if(i==37U)C(raw_owner.unresolved&&n->scq_inputs.checking&&raw_closes==0U);
  /* Only test-created SD memory is disposed here; real cleanup was withheld
   * by the shim, never failed natively. Not a production recovery/second close. */
  if(interval_sd){C(!interval_real_free(interval_sd));interval_sd=NULL;}
 }
 {PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;
  memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();raw_fault=60U;
  n->install_paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;
  C(scqc_descriptor(&raw_owner,0U)==ERROR_INVALID_STATE); /* not an installed-source bypass */
  n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;n->install_paths.selected_role=1U;n->install_paths.thread=GetCurrentThreadId();n->install_paths.selected.architecture=0x8664U;n->scq_cold_raw[0]=(HANDLE)71;
  C(scqc_descriptor(&raw_owner,0U)==ERROR_NOT_SUPPORTED&&n->scq_cold_diagnostic.stage==8U&&!n->scq_inputs.checking&&!n->scq_cold_diagnostic.native_sd.calls); /* missing export denies, no GetSecurityInfo fallback */
 }
 {unsigned k;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&raw_owner.pending_commit.personal_native;
  for(k=0;k<10U;k++){
   memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();n->phase=1U;raw_fault=0;raw_calls=0;envelope_override=1;
   n->install_paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;
   n->install_paths.thread=GetCurrentThreadId();n->install_paths.selected_role=1U;n->install_paths.selected.architecture=0x8664U;n->install_paths.original_chain=&n->scq_inputs.image_chain;
   envelope_count=k<3U?127U+k:k<6U?184U+k:k==6U?MAXDWORD:187U;
   if(k==7U)n->install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;
   if(k==8U)envelope_override=2;
   if(k==9U)raw_owner.pending_commit.stop=1;
   e=scqc_open_raw(&raw_owner,k>=3U?1U:0U,path);
   C((e==0)==(k<2U||k==3U));C((raw_calls!=0)==(e==0));
   if(k<3U)C(n->scq_cold_diagnostic.initial_known&&n->scq_cold_diagnostic.initial_handles==envelope_count);
  }
  envelope_override=0;
 }
 {unsigned k;PAPACC_SCQ_PLAN_DATA plan;PAPACC_SCQ_PEER_DATA bound;
  for(k=0;k<12U;k++){
   PAPACC_SCQ_COLD_DIAGNOSTIC *d=&raw_owner.pending_commit.personal_native.scq_cold_diagnostic;
   memset(&raw_owner,0,sizeof(raw_owner));C(scq_fixture_plan(&plan));bound=plan.runtime;
   memset(&plan.runtime,0,sizeof(plan.runtime));plan.runtime.sid_bytes=bound.sid_bytes;memcpy(plan.runtime.sid,bound.sid,68);
   plan.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;memset(plan.key,0,sizeof(plan.key));
   plan.valid_from[0]=1000U;plan.valid_from[1]=0;plan.valid_until[0]=2000U;plan.valid_until[1]=0;
   final_plan_now=1500U;final_plan_ticks=raw_queries=raw_calls=0;
   if(k==1U)plan.version=0;if(k==2U)plan.runtime.pid=9U;if(k==3U)plan.runtime.image_digest[0]=1U;
   if(k==4U)plan.runtime.sid_bytes=69U;if(k==5U)plan.runtime.sid[0]=0;
   if(k==6U)plan.runtime.sid[plan.runtime.sid_bytes]=1U;if(k==7U)plan.runtime.sid[2]=1U;
   if(k==8U)final_plan_now=999U;if(k==9U)final_plan_now=2000U;
   if(k==10U)plan.valid_until[0]=1000U;if(k==11U)plan.runtime.sid[8]=81U;
   e=scqc_validate_final(&raw_owner,&plan);C((e==0)==(k==0U));C(d->plan_attempted&&!raw_queries&&!raw_calls);
   if(k==0U){C(d->plan_passed==7U&&!d->plan_rejected&&d->window_known&&final_plan_ticks==1U);continue;}
   C(e==ERROR_INVALID_DATA&&d->mapped==ERROR_INVALID_DATA&&d->stage==9U&&d->reference==3U);
   C(d->plan_rejected==(k==1U?1U:k>=8U&&k<=10U?3U:2U));
   C(d->plan_passed==(k==1U?0U:k>=8U&&k<=10U?3U:1U));
   C(final_plan_ticks==(k>=8U&&k<=10U?1U:0U));
   if(k>=2U&&k<=7U)C(d->unbound_reason==k-1U);if(k==11U)C(d->unbound_reason==6U);
  }
  /* Every unexpected future-runtime scalar stays denied, with index only. */
  for(k=0;k<PAPACC_SCQ_PEER_WORDS;k++){
   DWORD field;unsigned skip=(unsigned)(offsetof(PAPACC_SCQ_PEER_DATA,sid_bytes)/sizeof(DWORD));
   if(k==skip)continue;
   C(scq_fixture_plan(&plan));bound=plan.runtime;memset(&plan.runtime,0,sizeof(plan.runtime));
   plan.runtime.sid_bytes=bound.sid_bytes;memcpy(plan.runtime.sid,bound.sid,68);
   ((DWORD *)&plan.runtime)[k]=1U;
   C(!scqp_unbound(&plan.runtime));C(scqc_unbound_reason(&plan.runtime,&field)==1U);
   C(field==(k<skip?k+1U:k));
  }
 }
 {unsigned k;PAPACC_SCQ_PEER_DATA runtime;WCHAR domain[128];DWORD field;
  for(k=0;k<17U;k++){
   memset(&runtime,0,sizeof(runtime));memset(domain,0,sizeof(domain));runtime_sid_fault=k;runtime_sid_lookups=0;
   if(k==15U)runtime.pid=99U;if(k==16U)runtime.sid_bytes=32U;
   e=scqc_runtime_sid(&runtime,domain);
   C((e==0)==(k==0U||k==1U||k==13U));C(runtime_sid_lookups==(k>=15U?0U:1U));
   if(!e){
    C(runtime.sid_bytes==32U&&scqp_unbound(&runtime));
    if(k==0U){runtime.sid_bytes=68U;C(!scqp_unbound(&runtime)&&scqc_unbound_reason(&runtime,&field)==3U);}
   }
   if(k==9U)C(runtime.sid[32]==1U); /* rejection never clears/trims invalid padding */
   if(k==7U)C(runtime.sid[1]==16U); /* malformed count not normalized */
   if(k>=15U)C(e==ERROR_INVALID_STATE);
  }
 }
 {PAPACC_SCQ_COLD_START_INPUT input;PAPACC_SCQ_PLAN_DATA plan;unsigned k;
  for(k=0;k<5U;k++){
   memset(&input,0,sizeof(input));memset(&raw_owner,0,sizeof(raw_owner));raw_queries=raw_calls=0;
   input.request_path=L"C:\\request.md";input.decision_path=L"C:\\decision.txt";input.authorization_ms=240000U;
   scqg_selected_p1(&input.limits);
   if(k==1U){input.existing_console_data=1U;input.limits.commit_floor=9823326208ULL;}
   if(k==2U)input.existing_console_data=2U;
   if(k==3U){input.existing_console_data=1U;input.limits.commit_floor=~0ULL;}
   if(k==4U){input.existing_console_data=1U;input.limits.commit_floor=8589934592ULL;}
   C(scqc_plan(&raw_owner,&input,&plan)==ERROR_INVALID_DATA&&!raw_queries&&!raw_calls);
  }
  printf("PASS COLD_COMMIT_SELECTION negative_cases=5 NO_SELECTION_OLD_OR_OVERFLOW_FLOOR_DENIED_BEFORE_NATIVE\n");
 }

 {PAPACC_SCQ_READ_COLD_INPUT read;PAPACC_SCQ_PLAN_DATA plan,seed;PAPACC_SCQ_EXISTING_SELECTION selected;unsigned k;
  C(scq_fixture_plan(&seed));
  for(k=0;k<8U;k++){
   memset(&read,0,sizeof(read));memset(&raw_owner,0,sizeof(raw_owner));raw_owner.writer=GetCurrentThreadId();raw_owner.pending_commit.personal_native.console.is_installed=1;
   read.cold.request_path=L"C:\\request";read.cold.decision_path=L"C:\\decision";read.cold.existing_console_data=1;read.cold.authorization_ms=240000U;
   memset(read.cold.pins,1,sizeof(read.cold.pins));scqg_selected_p1(&read.cold.limits);memcpy(read.key,seed.key,112);memset(read.public_id,7,32);
   raw_fault=0;p1_read_line=1;p1_read_selected=p1_selected=p1_reads=raw_calls=raw_queries=0;
   if(k==1U)read.cold.existing_console_data=0;if(k==2U)read.key[0]=0;if(k==3U)memset(read.public_id,0,32);
   if(k==4U)p1_read_line=0;if(k==5U)raw_owner.pending_commit.stop=1;if(k==6U)read.cold.limits.commit_floor=8589934592ULL;
   if(k==7U){memset(&selected,0,sizeof(selected));read.cold.existing=&selected;}
   e=scqc_plan_variant(&raw_owner,&read.cold,&plan,&read);C(e!=0&&!raw_calls&&!raw_queries);
   if(p1_read_selected!=(k==0U?1U:0U))printf("READ_COLD k=%u e=%lu selected=%u reads=%u keyvalid=%d\n",k,(unsigned long)e,p1_read_selected,p1_reads,scqp_key(read.key));C(p1_read_selected==(k==0U?1U:0U));C(!raw_owner.pending_commit.personal_native.scq_admin.value.controller_consumed);
  }
  p1_read_line=0;printf("PASS READ_COLD_SELECTION cases=8 EXPLICIT_PROFILE_PUBLIC_PIN_NO_FALLBACK_NO_NATIVE\n");
 }
 printf("PASS COLD_RAW_OPEN RUNTIME_SID_17_CASES FINAL_PLAN_41_CASES P1_ENVELOPE_10_CASES cases=104 checks=%u NINE_CUTS_38_MODELED_CASES_QUOTA_ABORT_OWNED_SD_CLOSE_NO_RETRY_NO_DECISION_NO_RNG_NO_SCM P1_DEVICE_DATA_NOT_AUTHORITY STRICT_OTHER_PROFILES NT_API_MODELED NO_REAL_OPEN\n",checks-before);return 0;
}
