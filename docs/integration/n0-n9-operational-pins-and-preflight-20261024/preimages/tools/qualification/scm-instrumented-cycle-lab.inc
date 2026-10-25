/* Successor of the SAME fixed SCM cycle; all positive effects MANUAL ONLY. */
#include <stdlib.h>
#define PAPACC_PSR_FILE_CAPSULE_LAB 1
#include "psr-file-capture-lab.inc"
#include "scm-instrumented-observer.inc"
static IG_GATE ig_gate;
static PSRF_FILE ig_diag;
static HANDLE ig_guard_snapshot,ig_approval_file;
static PSECURITY_DESCRIPTOR ig_diag_sd;
static unsigned ig_locked;
static OVERLAPPED ig_lock;
static const WCHAR ig_diag_path[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.diag";
static const WCHAR ig_intent_path[]=L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\first-instrumented-native-lab-start-preparation-20261023\\manual-native-intent.txt";
static DWORD ig_approval(const char *expected)
{
 unsigned i;DWORD e;BY_HANDLE_FILE_INFORMATION id;
 if(!expected||strlen(expected)!=64)return ERROR_ACCESS_DENIED;
 for(i=0;i<64;i++)if(!((expected[i]>='0'&&expected[i]<='9')||(expected[i]>='a'&&expected[i]<='f')))return ERROR_ACCESS_DENIED;
 ig_approval_file=CreateFileW(L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\canonical-preflight-busy-diagnosis-20261023\\EXACT-OPERATION-APPROVED.json",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(ig_approval_file==INVALID_HANDLE_VALUE){ig_approval_file=NULL;return lab_error();}
 if(!GetFileInformationByHandle(ig_approval_file,&id)||id.nFileSizeHigh||!id.nFileSizeLow||id.nFileSizeLow>16384||id.nNumberOfLinks!=1||(id.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))return ERROR_INVALID_DATA;
 e=lab_hash(ig_approval_file,expected);return e;
}
static DWORD ig_actor(unsigned role,DWORD pid,ULONGLONG creation)
{
 FILETIME ct,et,kt,ut;WCHAR image[1024];DWORD cap=1024;ULONGLONG actual;HANDLE token=NULL;union {TOKEN_USER aligned;BYTE bytes[256];} u;DWORD n;
 if(role>1||!pid||pid==GetCurrentProcessId()||ig_observer.process[role])return ERROR_INVALID_PARAMETER;
 ig_observer.process[role]=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,pid);if(!ig_observer.process[role])return lab_error();
 if(!GetProcessTimes(ig_observer.process[role],&ct,&et,&kt,&ut)||!QueryFullProcessImageNameW(ig_observer.process[role],0,image,&cap)||_wcsicmp(image,L"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe"))return ERROR_INVALID_DATA;
 actual=((ULONGLONG)ct.dwHighDateTime<<32)|ct.dwLowDateTime;if(actual!=creation)return ERROR_INVALID_DATA;
 if(!OpenProcessToken(ig_observer.process[role],TOKEN_QUERY,&token))return lab_error();
 n=0;if(!GetTokenInformation(token,TokenUser,&u,sizeof(u),&n)||n>sizeof(u)||!cycle_sid_inside(u.bytes,n,u.aligned.User.Sid)||!EqualSid(u.aligned.User.Sid,lab_sid)){if(!lab_close_aux(token))return ERROR_INVALID_HANDLE;return ERROR_INVALID_OWNER;}
 if(!lab_close_aux(token))return ERROR_INVALID_HANDLE;
 ig_observer.creation[role]=ct;ig_observer.pid[role]=pid;ig_observer.powershell[role]=1;return 0;
}
static DWORD ig_project_census(void)
{
 PROCESSENTRY32W pe;DWORD e=0;unsigned j,n0_seen=0;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.bd.used){
  if(!native.admission.bd.bound)return ERROR_ACCESS_DENIED;
  e=papacc_server_service_admission_bd_revalidate(native.admission.bd.check);if(e)return e;
  e=ps_bd_inventory(&native.admission.bd.binding,&native.admission.bd.inventory,GetTickCount64());if(e)return e;
 }
#endif
if(ig_guard_snapshot||native.retained)return ERROR_INVALID_STATE;ig_guard_snapshot=ig_diag_handle(cycle_stage,"CreateToolhelp32Snapshot","process_snapshot",CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0));if(ig_guard_snapshot==INVALID_HANDLE_VALUE){ig_guard_snapshot=NULL;return lab_error();}
 ZeroMemory(&pe,sizeof(pe));pe.dwSize=sizeof(pe);if(!ig_diag_bool(cycle_stage,"Process32FirstW","process_snapshot_entry",Process32FirstW(ig_guard_snapshot,&pe)))return lab_error();
 do{int participant=0,child=0;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.bd.bound){
  if(native.admission.bd.binding.n0){for(j=0;j<PS_BD_ROLES;j++)if(native.admission.bd.inventory.present[j]&&pe.th32ProcessID==native.admission.bd.inventory.role[j].pid)n0_seen|=1U<<j;}
  e=ps_bd_census(&native.admission.bd.binding,&native.admission.bd.inventory,GetTickCount64(),pe.th32ProcessID,pe.th32ParentProcessID,(unsigned)(!_wcsnicmp(pe.szExeFile,L"papacc",6)));
  if(e){ig_diag_note(cycle_stage,"bd_census_predicate","process_snapshot",e,e,pe.th32ProcessID,4);break;}
  continue;
 }
#endif

 for(j=0;j<4;j++){if(ig_observer.pid[j]&&pe.th32ProcessID==ig_observer.pid[j])participant=1;if(ig_observer.pid[j]&&pe.th32ParentProcessID==ig_observer.pid[j])child=1;}
 if(!participant&&(child||!_wcsnicmp(pe.szExeFile,L"papacc",6))){unsigned k;ig_diag_note(cycle_stage,"process_census_predicate","process_snapshot",ERROR_BUSY,ERROR_BUSY,pe.th32ProcessID,4);printf("IG_CENSUS_REJECT pid=%lu parent=%lu child=%u project_prefix=%u; NO_ADMISSION\n",(unsigned long)pe.th32ProcessID,(unsigned long)pe.th32ParentProcessID,(unsigned)child,(unsigned)(!_wcsnicmp(pe.szExeFile,L"papacc",6)));printf("IG_CENSUS_NAME_UTF16=");for(k=0;k<64U&&pe.szExeFile[k];k++)printf("%04x",(unsigned)(unsigned short)pe.szExeFile[k]);printf("; TRUNCATED=%u; SNAPSHOT_NAME_ONLY_NO_CREATION_PROOF\n",(unsigned)(k==64U&&pe.szExeFile[k]!=0));e=ERROR_BUSY;break;}}while(ig_diag_next(cycle_stage,"Process32NextW","process_snapshot_entry",Process32NextW(ig_guard_snapshot,&pe)));
 if(!e&&GetLastError()!=ERROR_NO_MORE_FILES)e=lab_error();
#ifdef PAPACC_N0_N9_NATIVE_OWNER
 if(!e&&native.admission.bd.bound&&native.admission.bd.binding.n0){unsigned expected=0;
 for(j=0;j<PS_BD_ROLES;j++)if(native.admission.bd.inventory.present[j])expected|=1U<<j;
 if(n0_seen!=expected){e=ERROR_INVALID_DATA;ig_diag_note(cycle_stage,"n0_original_roles_missing","process_snapshot",e,e,0,4);}}
#else
 (void)n0_seen;
#endif
 if(!ig_diag_bool(cycle_stage,"CloseHandle","process_snapshot_handle",CloseHandle(ig_guard_snapshot))){native.retained=1;return ERROR_INVALID_HANDLE;}ig_guard_snapshot=NULL;
 /* One self query at the post-census/pre-PSS boundary, not admission authority.
  * No real handle acquisition, snapshot, external process or recorder. */
 if(e==ERROR_BUSY){DWORD saved=GetLastError(),handles=0,metric_error=0;BOOL observed;
  observed=GetProcessHandleCount(GetCurrentProcess(),&handles);if(!observed){metric_error=GetLastError();if(!metric_error)metric_error=ERROR_GEN_FAILURE;}
  printf("IG_HANDLE_REQUALIFICATION observed=%u handles=%lu limit=128 metric_error=%lu pid=%lu tick=%llu; POINT=POST_CENSUS_CLOSE_PRE_METRICS_PSS; HANDLE_GATE=NOT_REACHED; FIRST_ERROR_UNCHANGED; NO_ADMISSION\n",(unsigned)observed,(unsigned long)handles,(unsigned long)metric_error,(unsigned long)GetCurrentProcessId(),GetTickCount64());
  SetLastError(saved);
 }
 return e;
}
static DWORD ig_confirm(void)
{
 HANDLE in,out;DWORD im,om,n,w,e=0;WCHAR reply[128];
 static const WCHAR text[]=L"ONE INSTRUMENTED LAB ATTEMPT 75330d11cc7e4123ae62eadc9a59c2f5\r\nExisting PapinhoAcceleratorLab20261023; same virtual identity; DEMAND/STOPPED.\r\nExclusive protected NEW papacc_server_lab_host_ntstatus.exe; SHA256 eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b.\r\nConfig a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44; unchanged parent/dependencies.\r\nCREATE_NEW one protected diagnostic papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.diag; arm1024/final2048.\r\nOne explicit LAB image rebind, ONE Start and at most ONE safe Stop. No delete, overwrite, rollback, retry or kill.\r\nED11 preflight60s/final1s; host admission5GiB physical/9GiB commit, monitor floors4/8GiB. NONE_GRANTED credits.\r\nAt most four participant processes; snapshot stack/kernel/cache remains partially UNKNOWN.\r\nNo listener/network/CA/key/certificate/bootstrap/CURRENT/signature/authority. Retention remains UNKNOWN if not factual.\r\nRequires SEPARATE exact operation approval; preparing this launcher does not approve execution.\r\nType ENTIRE line; Esc/mismatch cancels before intent consumption:\r\nINSTRUMENT_SCM_LAB 75330d11cc7e4123ae62eadc9a59c2f5\r\n> ";
 in=CreateFileW(L"CONIN$",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);if(in==INVALID_HANDLE_VALUE)return lab_error();
 out=CreateFileW(L"CONOUT$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);if(out==INVALID_HANDLE_VALUE){e=lab_error();if(!lab_close_aux(in))return ERROR_INVALID_HANDLE;return e;}
 if(!GetConsoleMode(in,&im)||!GetConsoleMode(out,&om)||(im&(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT))!=(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT)||!WriteConsoleW(out,text,(DWORD)wcslen(text),&w,NULL)||w!=wcslen(text)||!ReadConsoleW(in,reply,127,&n,NULL)){e=ERROR_ACCESS_DENIED;goto done;}
 while(n&&(reply[n-1]==L'\r'||reply[n-1]==L'\n'))n--;reply[n]=0;
 if(wcscmp(reply,L"INSTRUMENT_SCM_LAB 75330d11cc7e4123ae62eadc9a59c2f5")||papacc_server_console_win32_stop_requested())e=ERROR_CANCELLED;
 done:if(!lab_close_aux(in))e=ERROR_INVALID_HANDLE;if(!lab_close_aux(out))e=ERROR_INVALID_HANDLE;return e;
}
static DWORD ig_create_intent(void)
{
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 return papacc_server_service_admission_consume();
#else
 return ERROR_NOT_SUPPORTED;
#endif
}
static DWORD ig_create_diagnostic(void)
{
 SECURITY_ATTRIBUTES sa={sizeof(sa),NULL,FALSE};DWORD e;
 if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;FA;;;S-1-5-21-785699550-1440458426-1682014700-1108)(A;;FA;;;SY)(A;;FA;;;BA)(A;;0x120083;;;S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451)",SDDL_REVISION_1,&ig_diag_sd,NULL))return lab_error();
 sa.lpSecurityDescriptor=ig_diag_sd;e=psrf_create(&ig_diag,ig_diag_path,&sa,(const BYTE *)IG_ATTEMPT);if(e)return e;
 if(LocalFree(ig_diag_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}ig_diag_sd=NULL;
 if(!ig_diag.closed||ig_diag.handle||ig_diag.retained)return ERROR_INVALID_STATE;
 printf("IG_DIAGNOSTIC_CREATED=%lu:%lu:%lu; SIZE=1024; PREPARER_CLOSED=1; NO_DELETE\n",(unsigned long)ig_diag.identity.dwVolumeSerialNumber,(unsigned long)ig_diag.identity.nFileIndexHigh,(unsigned long)ig_diag.identity.nFileIndexLow);return 0;
}
static DWORD ig_extract(void)
{
 HANDLE h;BY_HANDLE_FILE_INFORMATION id;LARGE_INTEGER size,offset;DWORD n,e=0;PSRF_FRAME f;
 h=CreateFileW(ig_diag_path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(h==INVALID_HANDLE_VALUE)return lab_error();
 if(!GetFileInformationByHandle(h,&id)||!psrf_same(&id,&ig_diag.identity)||!GetFileSizeEx(h,&size)||size.QuadPart!=2048){e=ERROR_INVALID_DATA;goto done;}
 offset.QuadPart=1024;if(!SetFilePointerEx(h,offset,NULL,FILE_BEGIN)||!ReadFile(h,&f,sizeof(f),&n,NULL)||n!=sizeof(f)||!psrf_valid(&f,(const BYTE *)IG_ATTEMPT,f.kind)||(f.kind!=1U&&f.kind!=2U)){e=ERROR_INVALID_DATA;goto done;}
 printf("IG_DIAGNOSTIC_RECEIPT kind=%lu length=%lu; ID_MATCH=1; NO_AUTHORITY\n",(unsigned long)f.kind,(unsigned long)f.length);printf("IG_FIRST_RECORD=%.*s\n",(int)f.length,(const char *)f.text);
 done:if(!lab_close_aux(h))return ERROR_INVALID_HANDLE;return e;
}
#ifdef PAPACC_N0_N9_NATIVE_OWNER
#include "scm-n0-context.inc"
#endif
static DWORD ig_cleanup(void)
{
 unsigned i;DWORD e;
 if(ig_observer.retained||ig_diag.retained||ig_gate.retained||native.retained||!ig_retire(&ig_gate))return ERROR_INVALID_STATE;
 if((e=ig_observer_close_snapshot()))return e;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(ig_observer.bd_borrowed){for(i=0;i<4;i++)ig_observer.process[i]=NULL;ig_observer.bd_borrowed=0;}
 if(native.admission.bd.used){
  /* End synchronous sample borrows before canonical references can close.
   * Numeric measurements remain; a retired sample cannot authorize anything. */
  ig_observer.sample.error=ERROR_INVALID_STATE;ig_observer.sample.bd=0;
  ZeroMemory(&ig_observer.sample.binding,sizeof(ig_observer.sample.binding));ZeroMemory(&ig_observer.sample.inventory,sizeof(ig_observer.sample.inventory));
  if((e=papacc_server_service_admission_bd_close(1)))return e;
 }
#endif
 if(ig_observer.event){if(!EvtClose(ig_observer.event))return ERROR_INVALID_HANDLE;ig_observer.event=NULL;}
 if(ig_observer.event_query){if(!EvtClose(ig_observer.event_query))return ERROR_INVALID_HANDLE;ig_observer.event_query=NULL;}
 if(ig_guard_snapshot){if(!CloseHandle(ig_guard_snapshot))return ERROR_INVALID_HANDLE;ig_guard_snapshot=NULL;}
 if(ig_diag_sd){if(LocalFree(ig_diag_sd))return ERROR_INVALID_HANDLE;ig_diag_sd=NULL;}
 if(ig_diag.handle)return ERROR_INVALID_STATE;
 for(i=0;i<2;i++)if(ig_observer.process[i]){if(!CloseHandle(ig_observer.process[i]))return ERROR_INVALID_HANDLE;ig_observer.process[i]=NULL;}
 if(ig_locked){if(!UnlockFileEx(lab_config,0,1,0,&ig_lock))return ERROR_INVALID_HANDLE;ig_locked=0;}
 if(ig_approval_file){if(!CloseHandle(ig_approval_file))return ERROR_INVALID_HANDLE;ig_approval_file=NULL;}
#ifdef PAPACC_N0_N9_NATIVE_OWNER
 if((e=ig_n0_close()))return e;
#endif
 e=cycle_cleanup();if(e)return e;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.phase==1||native.admission.phase==0)return papacc_server_service_admission_finish(1);
 if(native.admission.phase==3)return ERROR_INVALID_STATE;
#endif
 return 0;
}
static int ig_number(const char *s,ULONGLONG *value)
{char *end;const char *p;if(!s||!*s)return 0;for(p=s;*p;p++)if(*p<'0'||*p>'9')return 0;*value=_strtoui64(s,&end,10);return !*end&&*value!=~(ULONGLONG)0&&*value!=0;}
static DWORD ig_bind_service(DWORD pid)
{
 DWORD n=1024;unsigned role=3;WCHAR image[1024];FILETIME et,kt,ut;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.bd.used){DWORD e;
  if(!native.admission.bd.bound)return ERROR_ACCESS_DENIED;
  e=papacc_server_service_admission_bd_revalidate(native.admission.bd.check);if(e)return e;
  e=ps_bd_inventory(&native.admission.bd.binding,&native.admission.bd.inventory,GetTickCount64());if(e)return e;
  role=PS_BD_SERVICE;
 }
#endif
 if(cycle_process)return ig_observer.pid[role]==pid?0:ERROR_INVALID_DATA;
 cycle_process=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,pid);if(!cycle_process)return lab_error();
 if(!QueryFullProcessImageNameW(cycle_process,0,image,&n)||_wcsicmp(image,cycle_path)||!GetProcessTimes(cycle_process,&ig_observer.creation[role],&et,&kt,&ut))return ERROR_INVALID_DATA;
 ig_observer.process[role]=cycle_process;ig_observer.pid[role]=pid;ig_observer.count=4;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.bd.bound){DWORD session,e;PS_BD_INVENTORY *v=&native.admission.bd.inventory;
  if(v->present[PS_BD_SERVICE]||!ProcessIdToSessionId(pid,&session))return ERROR_INVALID_DATA;
  v->role[PS_BD_SERVICE].handle=cycle_process;v->role[PS_BD_SERVICE].pid=pid;v->role[PS_BD_SERVICE].session=session;
  v->role[PS_BD_SERVICE].creation=((ULONGLONG)ig_observer.creation[role].dwHighDateTime<<32)|ig_observer.creation[role].dwLowDateTime;
  v->present[PS_BD_SERVICE]=1;v->count++;e=ps_bd_inventory(&native.admission.bd.binding,v,GetTickCount64());if(e)return e;
 }
#endif
 return 0;
}
int main(int argc,char **argv)
{
 DWORD e=0,close_error=0,k,n;ULONGLONG p1=0,p2=0,t1=0,t2=0,began;SERVICE_STATUS_PROCESS status;SERVICE_STATUS stopped;FILETIME et,kt,ut;int positive=0,diagnostic_attempted=0,n0=0;ULONGLONG control=0;SYSTEM_INFO sys;
 if(argc==4&&!strcmp(argv[1],"--observe-peer")){
  if(!ig_number(argv[2],&p1)||p1>MAXDWORD||!ig_number(argv[3],&t1))return 2;
  if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&lab_sid))return 2;
  e=ig_actor(0,(DWORD)p1,t1);if(!e){ig_observer.count=1;ig_observer.identity=IG_ID_ALL;e=ig_observe();}
  close_error=ig_cleanup();if(close_error){puts("IG_RETAINED_PEER; NO_CLOSE_NO_RETRY");fflush(stdout);Sleep(INFINITE);return 2;}
  printf("IG_OBSERVER_PEER error=%lu close=0; NO_SCM_NO_FILE_NO_START\n",(unsigned long)e);return e?2:0;
 }
 if(argc==2&&!strcmp(argv[1],"--observe-boot")){e=ig_boot();close_error=ig_cleanup();if(close_error){puts("IG_RETAINED_BOOT; NO_CLOSE_NO_RETRY");fflush(stdout);Sleep(INFINITE);return 2;}printf("IG_BOOT_OBSERVER error=%lu close=0; NO_SCM_NO_START\n",(unsigned long)e);return e?2:0;}
 if(argc==2&&!strcmp(argv[1],"--observe-self")){
  ig_observer.count=1;ig_observer.identity=IG_ID_ALL;ig_observer.process[0]=GetCurrentProcess();ig_observer.pid[0]=GetCurrentProcessId();ig_observer.powershell[0]=0;
  if(!GetProcessTimes(GetCurrentProcess(),&ig_observer.creation[0],&et,&kt,&ut))return 2;
  e=ig_observe();close_error=ig_observer_close_snapshot();if(ig_observer.retained||close_error){puts("IG_RETAINED_SELF; NO_CLOSE_NO_RETRY");fflush(stdout);Sleep(INFINITE);return 2;}printf("IG_STORAGE observer=%llu gate=%llu preparer=%llu native=%llu host=%llu; FRAME_NESTED_NO_NEW_SERVICE_STORAGE\n",(ULONGLONG)sizeof(ig_observer),(ULONGLONG)sizeof(ig_gate),(ULONGLONG)sizeof(ig_diag),(ULONGLONG)sizeof(native),(ULONGLONG)sizeof(host));printf("IG_OBSERVER_SELF error=%lu close=%lu; NO_SCM_NO_FILE_NO_START\n",(unsigned long)e,(unsigned long)close_error);return e||close_error?2:0;
 }
#ifdef PAPACC_N0_N9_NATIVE_OWNER
 if(argc==8&&!strcmp(argv[1],"--prepare-n0")){
  if(strcmp(argv[2],IG_ATTEMPT)||!ig_number(argv[3],&p1)||!ig_number(argv[4],&t1)||!ig_number(argv[5],&control)||!ig_number(argv[6],&p2)||!ig_number(argv[7],&t2)||p1>MAXDWORD||p2>MAXDWORD||control>MAXDWORD||p1==p2)return 2;
  n0=1;goto validated_arguments;
 }
#endif
 if((argc!=7&&argc!=8)||(strcmp(argv[1],"--prepare")&&strcmp(argv[1],"--cycle-lab"))||strcmp(argv[2],IG_ATTEMPT)||!ig_number(argv[3],&p1)||!ig_number(argv[4],&t1)||!ig_number(argv[5],&p2)||!ig_number(argv[6],&t2)||p1>MAXDWORD||p2>MAXDWORD||p1==p2){puts("IG_ARGUMENT_IDENTITY_DENY; EFFECTS=NONE");return 2;}
 positive=!strcmp(argv[1],"--cycle-lab");if((positive&&argc!=8)||(!positive&&argc!=7)){puts("IG_SEPARATE_APPROVAL_REQUIRED; EFFECTS=NONE");return 2;}
#ifdef PAPACC_N0_N9_NATIVE_OWNER
 validated_arguments:
#endif
 setvbuf(stdout,NULL,_IONBF,0);native.thread=GetCurrentThreadId();
 GetNativeSystemInfo(&sys);if(sys.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_AMD64){e=ERROR_NOT_SUPPORTED;goto done;}
 if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&lab_sid)){e=lab_error();goto done;}
 if(papacc_server_console_win32_install(&native.console)!=PAPACC_RESULT_OK){e=ERROR_INVALID_STATE;goto done;}
 if(positive&&(e=ig_approval(argv[7])))goto done;
 /* B4.B.5 review: config lock/census do not serialize other canonical owners.
  * Separate operation approval cannot waive this unqualified requirement. */
#ifndef PAPACC_SINGLE_LAB_ADMISSION
 if(positive){puts("IG_CANONICAL_OWNER_EXCLUSIVITY=UNQUALIFIED; STOP_BEFORE_INTENT_AND_EFFECTS");e=ERROR_NOT_SUPPORTED;goto done;}
#endif
 if((e=lab_operator())||(!n0&&((e=ig_actor(0,(DWORD)p1,t1))||(e=ig_actor(1,(DWORD)p2,t2))))||(e=ig_boot()))goto done;
 ig_observer.count=n0?0:3;ig_observer.process[2]=n0?NULL:GetCurrentProcess();ig_observer.pid[2]=n0?0:GetCurrentProcessId();
 if(!GetProcessTimes(GetCurrentProcess(),&ig_observer.creation[2],&et,&kt,&ut)){e=lab_error();goto done;}
 printf("IG_QUALIFIER_INSTANCE pid=%lu creation=%lu:%lu; ATTEMPT=" IG_ATTEMPT "; POLICY=ACCEPTED; AUTHORITY=NONE\n",(unsigned long)GetCurrentProcessId(),(unsigned long)ig_observer.creation[2].dwHighDateTime,(unsigned long)ig_observer.creation[2].dwLowDateTime);
 cycle_stage="PROTECTED_BINDING";
 lab_parent=CreateFileW(lab_parent_path,READ_CONTROL|FILE_READ_ATTRIBUTES|FILE_TRAVERSE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(lab_parent==INVALID_HANDLE_VALUE){lab_parent=NULL;e=lab_error();goto done;}
 lab_config=CreateFileW(lab_config_path,GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(lab_config==INVALID_HANDLE_VALUE){lab_config=NULL;e=lab_error();goto done;}
 native.image=CreateFileW(lab_image_path,GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(native.image==INVALID_HANDLE_VALUE){native.image=NULL;e=lab_error();goto done;}
 if((e=lab_bound(0))||(e=cycle_dependencies_current(1)))goto done;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(positive&&(e=papacc_server_service_admission_bind_launch((DWORD)p2,t2)))goto done;
 if(positive){
  int secure;
  e=GetSecurityInfo(native.admission.launch,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&lab_pending_sd);if(e)goto done;
  secure=scm_descriptor(lab_pending_sd,(BYTE *)lab_sid);
  if(LocalFree(lab_pending_sd)){native.retained=1;e=ERROR_INVALID_HANDLE;goto done;}lab_pending_sd=NULL;
  if(!secure){e=ERROR_ACCESS_DENIED;goto done;}
 }
 cycle_stage="CANONICAL_ADMISSION";
 if((e=papacc_server_service_admission_enter(positive?lab_config:NULL,positive?IG_ATTEMPT:NULL)))goto done;
#endif
 cycle_stage="SOURCE_IMAGE";
 cycle_source_file=CreateFileW(cycle_source,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(cycle_source_file==INVALID_HANDLE_VALUE){cycle_source_file=NULL;e=lab_error();goto done;}
 if((e=lab_hash(cycle_source_file,cycle_sha)))goto done;
 native.command[0]=0;if(!lab_append(native.command,1050,L"\"")||!lab_append(native.command,1050,lab_parent_path)||!lab_append(native.command,1050,L"\\papacc_server_lab_host.exe\" service host")){e=ERROR_INVALID_DATA;goto done;}
 cycle_stage="SCM_READBACK";
 native.manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT);if(!native.manager){e=lab_error();goto done;}
 native.service=OpenServiceW(native.manager,ps_name,SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS|(positive?(SERVICE_CHANGE_CONFIG|SERVICE_START|SERVICE_STOP):0));if(!native.service){e=lab_error();goto done;}
 if((e=cycle_readback(SERVICE_DEMAND_START,0))||(e=cycle_status(&status)))goto done;
 if(status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId){e=ERROR_INVALID_STATE;goto done;}
 /* Atomic lock on an existing protected native object, beyond config contents.
  * Existing consumed historical intent denies the old positive cycle path. */
 if(GetFileAttributesW(L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\first-native-lab-service-start-stop-20261023\\manual-native-intent.txt")==INVALID_FILE_ATTRIBUTES){e=ERROR_INVALID_STATE;goto done;}
#ifdef PAPACC_N0_N9_NATIVE_OWNER
 if(n0){cycle_stage="N0_PROTECTED_CONTEXT";if((e=ig_n0_bind((DWORD)p1,t1,(DWORD)control,(DWORD)p2,t2)))goto done;}
#endif
 cycle_stage="CONFIG_BYTE_LOCK";
 ig_lock.Offset=4096;if(!LockFileEx(lab_config,LOCKFILE_EXCLUSIVE_LOCK|LOCKFILE_FAIL_IMMEDIATELY,0,1,0,&ig_lock)){e=lab_error();goto done;}ig_locked=1;
 cycle_stage="PREFLIGHT_CENSUS";
 if((e=ig_project_census()))goto done;
 if(GetFileAttributesW(ig_intent_path)!=INVALID_FILE_ATTRIBUTES){e=ERROR_FILE_EXISTS;goto done;}else if(GetLastError()!=ERROR_FILE_NOT_FOUND){e=lab_error();goto done;}
 ig_observer.identity=IG_ID_ALL;
 cycle_stage="PREFLIGHT_METRICS";
 if((e=ig_observe())||(e=ig_preflight(&ig_gate,&ig_observer.sample,GetTickCount64())))goto done;
 if(!positive){puts("IG_PREPARED_READ_ONLY; NO_CREATE_NO_REBIND_NO_START; EXACT_OPERATION_APPROVAL_STILL_REQUIRED");goto done;}
 cycle_stage="CONFIRM_REVALIDATION";
 if((e=ig_confirm())||(e=lab_operator_current())||(e=lab_bound(1))||(e=cycle_dependencies_current(0))||(e=cycle_readback(SERVICE_DEMAND_START,0))||(e=ig_project_census()))goto done;
 if(GetTickCount64()-ig_gate.preflight>60000U||papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
 cycle_stage="CONSUME_INTENT";
 if((e=ig_consume(&ig_gate))||(e=ig_create_intent()))goto done;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if((e=papacc_server_service_admission_effect()))goto done;
#endif
 if((e=cycle_copy())||(e=ig_create_diagnostic()))goto done;
 cycle_stage="EXPLICIT_INSTRUMENTED_REBIND";
 {WCHAR command[1050];command[0]=0;if(!lab_append(command,1050,L"\"")||!lab_append(command,1050,cycle_path)||!lab_append(command,1050,L"\" service host")){e=ERROR_INVALID_DATA;goto done;}
 cycle_effect=2;if(!ChangeServiceConfigW(native.service,SERVICE_NO_CHANGE,SERVICE_NO_CHANGE,SERVICE_NO_CHANGE,command,NULL,NULL,NULL,NULL,NULL,NULL)){e=lab_error();goto done;}}
 if((e=cycle_readback(SERVICE_DEMAND_START,1))||(e=cycle_files())||(e=cycle_status(&status))||(e=ig_project_census()))goto done;
 if(status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId||papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
 /* Final metric generation after all placement/rebind/preparation, no wait. */
 if((e=ig_observe())||(e=ig_metric(&ig_observer.sample,1)))goto done;
 if(GetTickCount64()<ig_gate.preflight||GetTickCount64()-ig_gate.preflight>60000U||GetTickCount64()-ig_observer.sample.oldest>1000U){e=ERROR_TIMEOUT;goto done;}
 cycle_stage="START_ONCE";if((e=ig_start(&ig_gate,&ig_observer.sample,GetTickCount64())))goto done;
 cycle_start_requested=1;if(!StartServiceW(native.service,0,NULL)){e=lab_error();goto done;}
 began=GetTickCount64();cycle_stage="OBSERVE_FROM_START_PENDING";
 for(;;){DWORD z;if((z=cycle_status(&status))){if(!e)e=z;goto done;}
  printf("IG_SERVICE state=%lu pid=%lu win32=%lu specific=%lu\n",(unsigned long)status.dwCurrentState,(unsigned long)status.dwProcessId,(unsigned long)status.dwWin32ExitCode,(unsigned long)status.dwServiceSpecificExitCode);
  if(status.dwCurrentState==SERVICE_STOPPED&&!cycle_stop_requested&&!e)e=status.dwWin32ExitCode?status.dwWin32ExitCode:ERROR_INVALID_STATE;
  if(status.dwProcessId&&!cycle_process){z=ig_bind_service(status.dwProcessId);if(z&&!e)e=z;}
  if((z=ig_project_census())!=0&&!e)e=z;
  if(cycle_process&&(z=ig_observe())!=0){if(!e)e=z;}else if(cycle_process&&(z=ig_monitor(&ig_gate,&ig_observer.sample,GetTickCount64()))!=0){if(!e)e=z;}
  if(status.dwCurrentState==SERVICE_STOPPED){if(!e&&(!cycle_stop_requested||status.dwWin32ExitCode||status.dwServiceSpecificExitCode))e=status.dwWin32ExitCode?status.dwWin32ExitCode:ERROR_INVALID_STATE;break;}
  if(status.dwCurrentState==SERVICE_RUNNING&&!cycle_stop_requested){
   if(!e){z=cycle_live(status.dwProcessId);if(z)e=z;}
   if((status.dwControlsAccepted&SERVICE_ACCEPT_STOP)==0){if(!e)e=ERROR_INVALID_STATE;goto done;}
   #ifdef PAPACC_SINGLE_LAB_ADMISSION
   (void)papacc_server_service_admission_stop();
#endif
   if((z=ig_stop(&ig_gate))!=0){if(!e)e=z;goto done;}cycle_stop_requested=1;
   if(!ControlService(native.service,SERVICE_CONTROL_STOP,&stopped)){if(!e)e=lab_error();goto done;}
  }
  if(GetTickCount64()-began>=30000U||papacc_server_console_win32_stop_requested()){if(!e)e=ERROR_TIMEOUT;goto done;}
  Sleep(100);
 }
 if(cycle_process&&WaitForSingleObject(cycle_process,30000)==WAIT_OBJECT_0){cycle_quiescent=1;ig_gate.quiescent=1;}
 if(!diagnostic_attempted){DWORD z;diagnostic_attempted=1;z=ig_extract();if(z&&!e)e=z;}
 if(!e)puts("IG_CYCLE_QUALIFIED; READINESS_TOKEN_STOP_QUIESCENCE_CAPTURE_FACTUAL; NO_AUTHORITY");
 done:
 if(e)ig_diag_note(cycle_stage,"gate_or_uninstrumented_api","qualifier",e,e,0,4);
 if(ig_diag.created&&!diagnostic_attempted){DWORD z;diagnostic_attempted=1;z=ig_extract();if(z&&!e)e=z;}
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.phase==3)ig_gate.retained=1;
 if(!ig_retire(&ig_gate)||ig_observer.retained||ig_diag.retained)papacc_server_service_admission_retain();
#endif
 if(!ig_retire(&ig_gate)||ig_observer.retained||ig_diag.retained){printf("IG_RETAINED stage=%s first=%lu starts=%u stops=%u; REFERENCES_RETAINED; UNKNOWN; NO_CLOSE_NO_RETRY_NO_KILL\n",cycle_stage,(unsigned long)e,ig_gate.starts,ig_gate.stops);fflush(stdout);Sleep(INFINITE);return 2;}
 close_error=ig_cleanup();if(close_error){ig_diag_note("CLEANUP","ig_cleanup","qualifier_refs",close_error,close_error,0,4);printf("IG_RETAINED stage=%s close=%lu first=%lu; UNKNOWN_NO_SECOND_CLOSE\n",cycle_stage,(unsigned long)close_error,(unsigned long)e);fflush(stdout);Sleep(INFINITE);return 2;}
 printf("IG_FINAL stage=%s first=%lu close=0 starts=%u stops=%u quiescent=%u intent_consumed=%u effect=%u diagnostic_created=%u; ED11=PARTIAL; CREDITS=NONE_GRANTED; NO_RETRY_NO_DELETE\n",cycle_stage,(unsigned long)e,ig_gate.starts,ig_gate.stops,ig_gate.quiescent,ig_gate.consumed,cycle_effect,ig_diag.created);
 /* Referenced historical helpers remain compiled and testable, not called. */
 (void)cycle_confirm;(void)cycle_intent;(void)cycle_wait;(void)ig_final;(void)psrf_capture;(void)k;(void)n;
 return e?2:0;
}
