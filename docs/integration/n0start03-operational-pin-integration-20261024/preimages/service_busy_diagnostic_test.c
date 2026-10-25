/* Isolated API fault injection. The qualifier entry is compiled, NEVER run. */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winternl.h>
#include <aclapi.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <processsnapshot.h>
#include <winevt.h>
#include <wintrust.h>
static unsigned aq_active,aq_mode,aq_calls,aq_closes,aq_close_fail,aq_parent_bad,aq_acl_bad,aq_race;
static NTSTATUS aq_status[2];
static BOOL WINAPI aq_info(HANDLE h,LPBY_HANDLE_FILE_INFORMATION p)
{if(aq_active&&h==(HANDLE)(ULONG_PTR)0x88888){ZeroMemory(p,sizeof(*p));p->dwVolumeSerialNumber=417614100;p->nFileIndexHigh=1966080;p->nFileIndexLow=79426+(aq_parent_bad==1?1:0);p->dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;if(aq_parent_bad==2)p->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;return TRUE;}return GetFileInformationByHandle(h,p);}
static DWORD WINAPI aq_security(HANDLE h,SE_OBJECT_TYPE type,SECURITY_INFORMATION mask,PSID *o,PSID *g,PACL *d,PACL *a,PSECURITY_DESCRIPTOR *sd)
{if(aq_active&&h==(HANDLE)(ULONG_PTR)0x88888){(void)type;(void)mask;(void)o;(void)g;(void)d;(void)a;return ConvertStringSecurityDescriptorToSecurityDescriptorW((aq_acl_bad?L"O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;AU)":L"O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)"),1,sd,NULL)?0:GetLastError();}return GetSecurityInfo(h,type,mask,o,g,d,a,sd);}
static NTSTATUS NTAPI aq_create(PHANDLE out,ACCESS_MASK access,POBJECT_ATTRIBUTES oa,PIO_STATUS_BLOCK io,PLARGE_INTEGER size,ULONG attr,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG bytes)
{unsigned i=aq_calls++;NTSTATUS st=aq_status[i<2?i:1];
 if(aq_race&&disposition==2){*out=NULL;io->Status=(NTSTATUS)0xc0000035L;return io->Status;}
 if(oa->RootDirectory!=(HANDLE)(ULONG_PTR)0x88888||access!=(FILE_READ_ATTRIBUTES|SYNCHRONIZE)||size||attr||share!=7||disposition!=1||options!=0x00200020U||ea||bytes||oa->Attributes!=0x1040U){st=(NTSTATUS)0xc000000dL;}
 if(!oa->ObjectName||!oa->ObjectName->Buffer||wcscmp(oa->ObjectName->Buffer,i==0?L"papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe":L"papacc-psr-0b1de869e11493b7cb08fb329578f6ce.diag"))st=(NTSTATUS)0xc000000dL;
 *out=(aq_mode!=2&&(st==0||aq_mode==1))?(HANDLE)(ULONG_PTR)0x66666:NULL;io->Status=st;return st;}
static FARPROC WINAPI aq_address(HMODULE m,LPCSTR name)
{if(aq_active&&!strcmp(name,"NtCreateFile")){typedef NTSTATUS (NTAPI *FN)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);FN f=aq_create;FARPROC p;memcpy(&p,&f,sizeof(p));return p;}return GetProcAddress(m,name);}
static HANDLE WINAPI aq_file(LPCWSTR path,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES sa,DWORD disposition,DWORD flags,HANDLE template_file)
{if(aq_active){(void)path;(void)access;(void)share;(void)sa;(void)flags;(void)template_file;SetLastError(disposition==CREATE_NEW?ERROR_FILE_EXISTS:ERROR_INVALID_PARAMETER);return INVALID_HANDLE_VALUE;}return CreateFileW(path,access,share,sa,disposition,flags,template_file);}
#define CreateFileW aq_file
#define GetFileInformationByHandle aq_info
#define GetSecurityInfo aq_security
#define GetProcAddress aq_address
static unsigned cp_open_calls,cp_failed_closes,pss_capture_calls,trust_calls;
static unsigned census_child_fixture,census_close_fail;
static DWORD WINAPI rejected_pss(HANDLE process,PSS_CAPTURE_FLAGS flags,DWORD context,HPSS *snapshot)
{(void)process;(void)flags;(void)context;++pss_capture_calls;*snapshot=NULL;return ERROR_BUSY;}
static LONG WINAPI rejected_trust(HWND window,GUID *action,LPVOID data)
{(void)window;(void)action;(void)data;++trust_calls;return ERROR_ACCESS_DENIED;}
static HANDLE WINAPI cp_denied_open(DWORD rights,BOOL inherit,DWORD pid){(void)rights;(void)inherit;(void)pid;++cp_open_calls;SetLastError(ERROR_ACCESS_DENIED);return NULL;}
static unsigned self_handle_calls,self_handle_failure;static DWORD self_handle_value=141;
static BOOL WINAPI model_handle_count(HANDLE process,PDWORD count)
{if(process!=GetCurrentProcess())return FALSE;++self_handle_calls;if(self_handle_failure){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}*count=self_handle_value;return TRUE;}
#define GetProcessHandleCount model_handle_count
static unsigned perf_calls,snapshot_calls,first_calls,next_calls,close_calls,free_calls;
static BOOL WINAPI busy_perf(PPERFORMANCE_INFORMATION p,DWORD bytes)
{(void)p;(void)bytes;++perf_calls;SetLastError(ERROR_BUSY);return FALSE;}
static HANDLE WINAPI busy_snapshot(DWORD flags,DWORD pid)
{(void)flags;(void)pid;++snapshot_calls;return (HANDLE)(ULONG_PTR)0x22222;}
static BOOL WINAPI busy_first(HANDLE h,LPPROCESSENTRY32W p)
{(void)h;++first_calls;p->th32ProcessID=12345;p->th32ParentProcessID=54321;wcscpy_s(p->szExeFile,260,census_child_fixture?L"conhost.exe":L"papacc_conflict_fixture.exe");return TRUE;}
static BOOL WINAPI busy_next(HANDLE h,LPPROCESSENTRY32W p)
{(void)h;(void)p;++next_calls;SetLastError(ERROR_NO_MORE_FILES);return FALSE;}
static BOOL WINAPI busy_close(HANDLE h)
{if(h==(HANDLE)(ULONG_PTR)0x66666){++aq_closes;if(aq_close_fail){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}if(h==(HANDLE)(ULONG_PTR)0x44444){++cp_failed_closes;SetLastError(ERROR_ACCESS_DENIED);return FALSE;}if(h==(HANDLE)(ULONG_PTR)0x22222){++close_calls;if(census_close_fail){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}return CloseHandle(h);}
static DWORD WINAPI busy_free(HANDLE process,HPSS snapshot)
{(void)process;(void)snapshot;++free_calls;return ERROR_ACCESS_DENIED;}
#undef GetPerformanceInfo
#define GetPerformanceInfo busy_perf
#define CreateToolhelp32Snapshot busy_snapshot
#define Process32FirstW busy_first
#define Process32NextW busy_next
#define CloseHandle busy_close
#define PssFreeSnapshot busy_free
#define PssCaptureSnapshot rejected_pss
#define WinVerifyTrust rejected_trust
#define OpenProcess cp_denied_open
static DWORD recovery_mode,recovery_calls;
static BOOL WINAPI model_config2(SC_HANDLE service,DWORD level,LPBYTE buffer,DWORD size,LPDWORD needed)
{
 (void)service;(void)size;++recovery_calls;*needed=0;
 if(recovery_mode==1||(recovery_mode==8&&level==SERVICE_CONFIG_FAILURE_ACTIONS_FLAG)||(recovery_mode==9&&level==SERVICE_CONFIG_TRIGGER_INFO)){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(level==SERVICE_CONFIG_FAILURE_ACTIONS){SERVICE_FAILURE_ACTIONSW *a=(SERVICE_FAILURE_ACTIONSW *)buffer;
  if(recovery_mode==2||recovery_mode==3){a->cActions=1;a->lpsaActions=(SC_ACTION *)(buffer+512);a->lpsaActions[0].Type=recovery_mode==2?SC_ACTION_RESTART:SC_ACTION_RUN_COMMAND;}
  if(recovery_mode==4){a->cActions=33;}if(recovery_mode==5){a->cActions=1;a->lpsaActions=(SC_ACTION *)(ULONG_PTR)1;}
 }else if(level==SERVICE_CONFIG_FAILURE_ACTIONS_FLAG){((SERVICE_FAILURE_ACTIONS_FLAG *)buffer)->fFailureActionsOnNonCrashFailures=recovery_mode==6;}
 else if(level==SERVICE_CONFIG_TRIGGER_INFO){((SERVICE_TRIGGER_INFO *)buffer)->cTriggers=recovery_mode==7?1:0;if(recovery_mode==10)((SERVICE_TRIGGER_INFO *)buffer)->pReserved=(BYTE *)(ULONG_PTR)1;}
 else {SetLastError(ERROR_INVALID_LEVEL);return FALSE;}
 return TRUE;
}
#define QueryServiceConfig2W model_config2
#define main unexecuted_instrumented_entry
#include "../tools/qualification/scm-registration-lab.c"
#undef main
/* Historical supplemental collector is compiled ONLY in this isolated fixture.
 * The active qualifier no longer includes, links or invokes it. */
#include "../tools/qualification/scm-console-proof.inc"
#define T(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
int main(void)
{
 DWORD e;IG_GATE g;unsigned cases=0;
 ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));cycle_stage="PREFLIGHT_METRICS";
 e=ig_observe();T(e==ERROR_BUSY&&perf_calls==1);T(ig_diagnostic.present&&ig_diagnostic.code==ERROR_BUSY&&ig_diagnostic.native==ERROR_BUSY);T(!strcmp(ig_diagnostic.api,"GetPerformanceInfo")&&!strcmp(ig_diagnostic.stage,"PREFLIGHT_METRICS"));++cases;
 SetLastError(123456);ig_diag_note("CLEANUP","CloseHandle","fixture",ERROR_ACCESS_DENIED,ERROR_ACCESS_DENIED,0,4);T(GetLastError()==123456&&ig_diagnostic.code==ERROR_BUSY&&!strcmp(ig_diagnostic.stage,"PREFLIGHT_METRICS"));++cases;
 /* A cleanup failure cannot overwrite the first failure or allow a second free. */
 ig_observer.snapshot=(HPSS)(ULONG_PTR)0x33333;e=ig_observer_close_snapshot();T(e==ERROR_ACCESS_DENIED&&ig_observer.retained&&free_calls==1&&ig_diagnostic.code==ERROR_BUSY);e=ig_observer_close_snapshot();T(e==ERROR_INVALID_STATE&&free_calls==1);++cases;
 /* Reset only fake isolated fixture state, never a real retained owner. */
 ZeroMemory(&ig_observer,sizeof(ig_observer));ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));cycle_stage="PREFLIGHT_CENSUS";
 e=ig_project_census();T(e==ERROR_BUSY&&snapshot_calls==1&&first_calls==1&&next_calls==0&&close_calls==1&&!ig_guard_snapshot);T(ig_diagnostic.code==ERROR_BUSY&&ig_diagnostic.pid==12345&&!strcmp(ig_diagnostic.api,"process_census_predicate"));++cases;
 T(cp_open_calls==0&&pss_capture_calls==0&&trust_calls==0);++cases;
 T(self_handle_calls==1);++cases;
 /* Observation success/failure and threshold boundary never replace first170. */
 {DWORD values[]={127,128,129};unsigned i;
 for(i=0;i<3;i++){ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));self_handle_value=values[i];e=ig_project_census();T(e==ERROR_BUSY&&ig_diagnostic.code==ERROR_BUSY);}++cases;
 self_handle_failure=1;ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));e=ig_project_census();T(e==ERROR_BUSY&&ig_diagnostic.code==ERROR_BUSY);self_handle_failure=0;++cases;}
 /* Non-prefix child is still rejected; no console-name exception. */
 ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));census_child_fixture=1;ig_observer.pid[0]=54321;
 e=ig_project_census();T(e==ERROR_BUSY&&ig_diagnostic.code==ERROR_BUSY&&cp_open_calls==0&&pss_capture_calls==0&&trust_calls==0);++cases;
 /* Failed snapshot close retains the fake owner and never retries acquisition. */
 ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));census_close_fail=1;
 e=ig_project_census();T(e==ERROR_INVALID_HANDLE&&native.retained&&ig_guard_snapshot&&ig_diagnostic.code==ERROR_BUSY);
 {unsigned acquired=snapshot_calls;e=ig_project_census();T(e==ERROR_INVALID_STATE&&snapshot_calls==acquired&&cp_open_calls==0&&pss_capture_calls==0&&trust_calls==0);}++cases;
 /* Reset ONLY injected fake ownership; no real reference is discarded. */
 native.retained=0;ig_guard_snapshot=NULL;census_close_fail=0;census_child_fixture=0;
 ZeroMemory(&ig_observer,sizeof(ig_observer));
 ZeroMemory(&g,sizeof(g));g.preflight_valid=1;g.consumed=1;g.starts=1;T(ig_stop(&g)==0&&g.stops==1);T(ig_stop(&g)==ERROR_INVALID_STATE&&g.stops==1);T(!ig_retire(&g));g.quiescent=1;T(ig_retire(&g));g.retained=1;T(!ig_retire(&g));++cases;
 ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));SetLastError(ERROR_BUSY);T(!ig_diag_bool("PREFLIGHT_METRICS","native_fixture","host",FALSE));T(GetLastError()==ERROR_BUSY&&ig_diagnostic.code==ERROR_BUSY);T(ig_diag_status("CLEANUP","PssFreeSnapshot","snapshot",ERROR_ACCESS_DENIED,0)==ERROR_ACCESS_DENIED&&ig_diagnostic.code==ERROR_BUSY);++cases;

 ZeroMemory(&ig_console_proof,sizeof(ig_console_proof));SetLastError(123456);ig_cp_capture(12345,54321);T(cp_open_calls==1&&ig_console_proof.invoked&&!ig_console_proof.process&&!ig_console_proof.image&&GetLastError()==123456&&ig_diagnostic.code==ERROR_BUSY);ig_cp_capture(12345,54321);T(cp_open_calls==1);++cases;
 {HANDLE ref=(HANDLE)(ULONG_PTR)0x22222;unsigned old=close_calls;T(ig_cp_close_ref(&ref)&&!ref&&close_calls==old+1);T(ig_cp_close_ref(&ref)&&close_calls==old+1);}++cases;
 {HANDLE ref=(HANDLE)(ULONG_PTR)0x44444;T(!ig_cp_close_ref(&ref)&&native.retained&&ref&&cp_failed_closes==1);T(!ig_cp_close_ref(&ref)&&cp_failed_closes==1);}++cases;
 /* Actual preconsume gate, isolated native API faults; no operational entry. */
 {unsigned k;NTSTATUS faults[]={(NTSTATUS)0xc0000022L,(NTSTATUS)0xc0000043L,(NTSTATUS)0xc000003aL,(NTSTATUS)0xc000050bL,(NTSTATUS)0xc0000001L,(NTSTATUS)0x103L,0};
 for(k=0;k<19;k++){
  ZeroMemory(&native.admission,sizeof(native.admission));ZeroMemory(&ig_gate,sizeof(ig_gate));ZeroMemory(&ig_diag,sizeof(ig_diag));ZeroMemory(&ig_absence,sizeof(ig_absence));ZeroMemory(&ig_diagnostic,sizeof(ig_diagnostic));
  native.retained=0;cycle_file=NULL;cycle_effect=0;cycle_start_requested=0;aq_active=1;aq_mode=0;aq_calls=aq_closes=aq_close_fail=aq_parent_bad=aq_acl_bad=aq_race=0;
  native.admission.phase=1;native.admission.owned=1;native.admission.mutex=(HANDLE)(ULONG_PTR)0x77777;native.admission.thread=GetCurrentThreadId();native.admission.bd.bound=1;native.admission.bd.binding.n0=1;
  lab_parent=(HANDLE)(ULONG_PTR)0x88888;T(ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&lab_sid));T(lab_acl(lab_parent,0,0)==0);
  aq_status[0]=aq_status[1]=(NTSTATUS)0xc0000034L;
  if(k==1)aq_status[0]=0;if(k==2)aq_status[1]=0;if(k==3)aq_parent_bad=1;if(k==4)aq_parent_bad=2;
  if(k>=5&&k<=10)aq_status[0]=faults[k-5];
  if(k==11){aq_status[0]=0;aq_close_fail=1;}if(k==12)native.admission.owned=0;if(k==13)aq_mode=1;if(k==14){aq_mode=2;aq_status[0]=0;}if(k==15)aq_acl_bad=1;if(k==18)papacc_server_console_win32_request_stop();
  e=ig_preconsume_absence();T(!ig_gate.consumed&&!native.admission.consumed&&!cycle_effect&&!ig_diag.created&&!cycle_start_requested);
  if(k==0||k==16||k==17){T(e==0&&aq_calls==2&&ig_absence.mask==3&&!cycle_file);T(ig_preconsume_absence()==ERROR_INVALID_STATE&&aq_calls==2);}
  else T(e!=0);
  if(k==16){ig_gate.consumed=1;native.admission.consumed=1;aq_race=1;T(cycle_open_relative(1)==ERROR_FILE_EXISTS&&ig_gate.consumed&&!cycle_effect&&!cycle_file);}
  if(k==17){PSRF_FILE f;SECURITY_ATTRIBUTES sa;BYTE attempt[32]={0};ZeroMemory(&f,sizeof(f));ZeroMemory(&sa,sizeof(sa));sa.nLength=sizeof(sa);sa.lpSecurityDescriptor=&sa;ig_gate.consumed=1;native.admission.consumed=1;T(psrf_create(&f,ig_diag_path,&sa,attempt)==ERROR_FILE_EXISTS&&f.consumed&&!f.created&&!f.handle&&ig_gate.consumed&&!cycle_effect);}
  if(k==18)T(e==ERROR_OPERATION_ABORTED&&aq_calls==0);
  if(k==1||k==2)T(e==ERROR_FILE_EXISTS&&aq_closes==1&&!cycle_file);
  if(k==3||k==4||k==12||k==15)T(aq_calls==0);
  if(k==10||k==11||k==13||k==14){T(native.retained&&ig_gate.retained);{unsigned saved=aq_calls;T(ig_preconsume_absence()==ERROR_INVALID_STATE&&aq_calls==saved);}}
  T(LocalFree(lab_sid)==NULL);lab_sid=NULL;++cases;
 }
 printf("PRECONSUME_QUERY_FRAME_BYTES=%llu; NATIVE_API_FAULT_FIXTURES_ONLY\n",(unsigned long long)sizeof(ig_absence));
 aq_active=0;lab_parent=NULL;cycle_file=NULL;native.retained=0;ZeroMemory(&native.admission,sizeof(native.admission));ZeroMemory(&ig_gate,sizeof(ig_gate));ZeroMemory(&ig_absence,sizeof(ig_absence));
 }
 /* All retained refs above are fake isolated handles, never live native refs. */
 native.retained=0;ZeroMemory(&ig_console_proof,sizeof(ig_console_proof));papacc_server_console_win32_request_stop();ig_cp_capture(12345,54321);T(cp_open_calls==1&&ig_console_proof.facts.stopped&&!ig_cp_window_candidate(&ig_console_proof.facts));++cases;
 T(pss_capture_calls==0&&trust_calls==0);++cases;
 /* New adapter input tests: synthetic data only, never qualifier entry. */
 {IG_N0_FILE_RECORD record;char text[512],bad[512];int size;unsigned old=cp_open_calls;
 size=sprintf_s(text,sizeof(text),"PACC_N2_OPENED_FILE_V2\nattempt=0b1de869e11493b7cb08fb329578f6ce\noperation=N0START-20261024-03\nvolume=1\nhigh=0\nlow=2\nsize=867840\nlinks=2\nsha256=%s\nsecurity_sha256=%s\nwindows_evidence_sha256=%s\n",
 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb","cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc");
 T(size>0&&ig_n0_parse(text,(size_t)size,&record)&&record.volume==1&&record.low==2&&record.size==867840&&record.links==2);++cases;
 strcpy_s(bad,sizeof(bad),text);memcpy(strstr(bad,"attempt=")+8,"75330d11cc7e4123ae62eadc9a59c2f5",32);T(!ig_n0_parse(bad,strlen(bad),&record));++cases;
 strcpy_s(bad,sizeof(bad),text);strstr(bad,"N0START-20261024-03")[17]='2';T(!ig_n0_parse(bad,strlen(bad),&record));++cases;
 strcpy_s(bad,sizeof(bad),text);bad[0]='X';T(!ig_n0_parse(bad,(size_t)size,&record));++cases;
 strcpy_s(bad,sizeof(bad),text);bad[size-2]='z';T(!ig_n0_parse(bad,(size_t)size,&record));++cases;
 T(!ig_n0_parse(text,(size_t)size-1,&record));++cases;
 strcpy_s(bad,sizeof(bad),text);bad[10]=0;T(!ig_n0_parse(bad,(size_t)size,&record));++cases;
 strcpy_s(bad,sizeof(bad),text);strcat_s(bad,sizeof(bad),"extra=1\n");T(!ig_n0_parse(bad,strlen(bad),&record));++cases;
 T(!ps_bd_hex(IG_N0_DOSSIER_SHA)&&!ps_bd_hex(IG_N0_EVIDENCE_SHA));
 T(ig_n0_bind(424242,55,111,424243,56)==ERROR_NOT_SUPPORTED&&cp_open_calls==old&&!ig_n0.prepared&&!ig_n0.dossier&&!ig_n0.auxiliary);++cases;
 {PS_BD_DESCRIPTOR a,b;char x[65],y[65];ZeroMemory(&a,sizeof(a));a.owner_pid=1;strcpy_s(a.auxiliary.provenance_hash,65,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");b=a;
 T(!ig_n0_plan(&a,x)&&!ig_n0_plan(&b,y)&&!strcmp(x,y));b.auxiliary.instance.creation++;T(!ig_n0_plan(&b,y)&&strcmp(x,y));++cases;
 b=a;b.statistics.ModifiedId.LowPart++;T(!ig_n0_plan(&b,y)&&strcmp(x,y));++cases;
 b=a;b.auxiliary.file_hash[0]='f';T(!ig_n0_plan(&b,y)&&strcmp(x,y));++cases;
 b=a;b.auxiliary.provenance_hash[0]='f';T(!ig_n0_plan(&b,y)&&strcmp(x,y));++cases;
 ig_n0.dossier_id.nFileIndexLow=1;T(!ig_n0_plan(&a,y)&&strcmp(x,y));ig_n0.dossier_id.nFileIndexLow=0;++cases;
 ig_n0.evidence_id.nFileIndexLow=1;T(!ig_n0_plan(&a,y)&&strcmp(x,y));ig_n0.evidence_id.nFileIndexLow=0;++cases;
 b=a;b.configuration=(HANDLE)(ULONG_PTR)123;b.auxiliary.file=(HANDLE)(ULONG_PTR)456;T(!ig_n0_plan(&b,y)&&!strcmp(x,y));++cases;
 }
 {static const char original[]="PACC_N2_OPENED_FILE_V1\nvolume=417614100\nhigh=327680\nlow=1591236\nsize=867840\nlinks=2\nsha256=b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963\nsecurity_sha256=7852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0\nwindows_evidence_sha256=babab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0\n";
 BYTE digest[32];char hash[65];IG_N0_FILE_RECORD original_record;
 T(sizeof(original)-1==326&&!ig_n0_parse(original,sizeof(original)-1,&original_record));++cases;
 T(pacc_storage_hash_callback(NULL,(const BYTE *)original,sizeof(original)-1,digest));ig_n0_hex(digest,hash);T(!strcmp(hash,"5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294"));++cases;
 }
 /* Reject CRLF/noncanonical text; original record is exact LF bytes. */
 {char crlf[512];size_t i,j;IG_N0_FILE_RECORD parsed;for(i=0,j=0;i<(size_t)size;i++){if(text[i]=='\n')crlf[j++]='\r';crlf[j++]=text[i];}crlf[j]=0;
 T(!ig_n0_parse(crlf,j,&parsed));++cases;
 for(i=0;i<j;i++)if(crlf[i]=='\r'){memmove(crlf+i,crlf+i+1,j-i);--j;break;}T(!ig_n0_parse(crlf,j,&parsed));++cases;}
 {BY_HANDLE_FILE_INFORMATION f;unsigned i;ZeroMemory(&f,sizeof(f));f.dwVolumeSerialNumber=417614100;f.nNumberOfLinks=1;f.nFileIndexHigh=131072;f.nFileIndexLow=2841430;f.nFileSizeLow=326;
 T(!ig_n0_input_identity(&f,0)&&!ig_n0_input_identity(&f,1));T(ig_n0_input_match(&f,417614100,131072,2841430,326));++cases;
 f.nFileIndexLow++;T(!ig_n0_input_identity(&f,0));f.nFileIndexLow--; ++cases;
 f.nFileSizeLow++;T(!ig_n0_input_identity(&f,0));f.nFileSizeLow--; ++cases;
 f.nNumberOfLinks=2;T(!ig_n0_input_identity(&f,0));f.nNumberOfLinks=1;++cases;
 f.dwFileAttributes=FILE_ATTRIBUTE_REPARSE_POINT;T(!ig_n0_input_identity(&f,0));++cases;
 f.dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;T(!ig_n0_input_identity(&f,0));++cases;
 f.dwFileAttributes=0;f.nFileIndexHigh=196608;f.nFileIndexLow=2841429;f.nFileSizeLow=5358;T(!ig_n0_input_identity(&f,1));T(ig_n0_input_match(&f,417614100,196608,2841429,5358));++cases;
 for(i=0;i<5;i++){static const DWORD hi[]={327680,131072,786432,851968,720896},lo[]={5,227641,1087874,2672648,2672656};f.nFileIndexHigh=hi[i];f.nFileIndexLow=lo[i];f.dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;
 T(ig_n0_root_identity(&f,i));++cases;f.nFileIndexLow++;T(!ig_n0_root_identity(&f,i));++cases;}
 }
 T(ig_n0_fresh(61000,1000,60000)&&!ig_n0_fresh(61001,1000,60000)&&!ig_n0_fresh(999,1000,60000));++cases;
 {PS_BD_CONTEXT_STORAGE c;unsigned i;ZeroMemory(&c,sizeof(c));T(!ps_bd_context_pending(&c));++cases;
 for(i=0;i<5;i++){c.input_root[i]=(HANDLE)(ULONG_PTR)0x22222;T(ps_bd_context_pending(&c));c.input_root[i]=NULL;++cases;}
 c.policy[3]=(HANDLE)(ULONG_PTR)0x22222;T(ps_bd_context_pending(&c));++cases;}
 {PS_BD_DESCRIPTOR a;char x[65],y[65];ZeroMemory(&a,sizeof(a));T(!ig_n0_plan(&a,x));ig_n0_positive=1;ig_n0_approval_sha="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";T(!ig_n0_plan(&a,y)&&strcmp(x,y));++cases;
 strcpy_s(x,sizeof(x),y);ig_n0_approval_sha="bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";T(!ig_n0_plan(&a,y)&&strcmp(x,y));++cases;ig_n0_positive=0;ig_n0_approval_sha=NULL;}
 {char *args[]={"fixture","--cycle-n0","attempt","actor","birth","thread","aux","auxbirth","approval"};T(ig_n0_positive_command(9,args)&&!ig_n0_readonly_command(9,args));T(!ig_n0_positive_command(8,args)&&!ig_n0_positive_command(9,NULL));++cases;args[1]="--cycle-lab";T(!ig_n0_positive_command(9,args));++cases;}
 /* Every handle below is a FAKE fixture; no protected input is opened. */
 ZeroMemory(&ig_n0,sizeof(ig_n0));native.retained=0;native.admission.phase=1;
 ig_n0.input_root[4]=(HANDLE)(ULONG_PTR)0x44444;{unsigned close_attempt_before=cp_failed_closes;T(ig_n0_close()==ERROR_INVALID_HANDLE&&native.retained&&ig_n0.input_root[4]&&cp_failed_closes==close_attempt_before+1);T(ig_n0_close()==ERROR_INVALID_STATE&&cp_failed_closes==close_attempt_before+1);++cases;}
 native.retained=0;ZeroMemory(&ig_n0,sizeof(ig_n0));
 {char *args[]={"fixture","--prepare-n0","attempt","actor","birth","thread","aux","auxbirth"};
 T(ig_n0_readonly_command(8,args));++cases;
 T(!ig_n0_readonly_command(7,args)&&!ig_n0_readonly_command(8,NULL));++cases;
 args[1]="--cycle-lab";T(!ig_n0_readonly_command(8,args));++cases;
 args[1]="--prepare";T(!ig_n0_readonly_command(8,args));++cases;
 args[1]="--observe-self";T(!ig_n0_readonly_command(8,args));++cases;
 }
 T(ig_n0_volume_profile(417614100UL,FS_PERSISTENT_ACLS,L"NTFS"));++cases;
 T(!ig_n0_volume_profile(417614101UL,FS_PERSISTENT_ACLS,L"NTFS"));++cases;
 T(!ig_n0_volume_profile(417614100UL,0,L"NTFS"));++cases;
 T(!ig_n0_volume_profile(417614100UL,FS_PERSISTENT_ACLS,L"FAT32"));++cases;
 printf("N0_CONTEXT_INPUT=MODEL_PASS; MISSING_NATIVE_PROVENANCE_DENY_BEFORE_OPEN; NO_INLINE_TRUST_NO_OPERATIONAL_ENTRY; STATE_BYTES=%llu\n",(ULONGLONG)sizeof(ig_n0));
 }
 {unsigned i;recovery_mode=0;recovery_calls=0;T(!ig_recovery_readback()&&recovery_calls==3);++cases;
 for(i=1;i<=10;i++){recovery_mode=i;T(ig_recovery_readback()!=0);++cases;}
 recovery_mode=0;}
 T(ig_n0_content_phase(1,0,0,1000,5000)==1);++cases;
 T(ig_n0_content_phase(1,1,5000,1000,5001)==0);++cases;
 T(ig_n0_content_phase(1,1,0,1000,5001)<0);++cases;
 T(ig_n0_content_phase(1,1,999,1000,5001)<0);++cases;
 T(ig_n0_content_phase(1,1,5000,1000,61001)<0);++cases;
 T(ig_n0_content_phase(1,0,0,1000,61001)<0);++cases;
 T(ig_n0_content_phase(1,1,5000,1000,999)<0);++cases;
 T(ig_n0_content_phase(0,1,5000,1000,5001)==1);++cases;
 {PS_BD_CONTEXT_STORAGE c;ZeroMemory(&c,sizeof(c));c.metric_window=1;T(ps_bd_context_pending(&c));++cases;}
 printf("REJECTION_REDUCTION=PASS; CENSUS_DENY_PRESERVED; NO_REJECTION_OPEN_PROCESS_PSS_OR_TRUST; HISTORICAL_CAPTURE_FIXTURE_ONLY\n");
 printf("CONSOLE_CAPTURE_STORAGE bytes=%llu; NEW_REFS_PROCESS_FILE_TRUST_PLUS_SHARED_SNAPSHOT; NO_ADMISSION\n",(ULONGLONG)sizeof(ig_console_proof));
 printf("BUSY_DIAGNOSTIC_MODEL=PASS cases=%u diagnostic_bytes=%llu; API_BUSY_CENSUS_FIRST_ERROR_LAST_ERROR_STOP_RETENTION_CLOSE_NO_RETRY; NO_PREFLIGHT_NO_SCM_NO_START\n",cases,(unsigned long long)sizeof(IG_DIAGNOSTIC));return 0;
}
