/* Isolated API fault injection. The qualifier entry is compiled, NEVER run. */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <processsnapshot.h>
#include <winevt.h>
#include <wintrust.h>
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
{if(h==(HANDLE)(ULONG_PTR)0x44444){++cp_failed_closes;SetLastError(ERROR_ACCESS_DENIED);return FALSE;}if(h==(HANDLE)(ULONG_PTR)0x22222){++close_calls;if(census_close_fail){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}return CloseHandle(h);}
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
 /* All retained refs above are fake isolated handles, never live native refs. */
 native.retained=0;ZeroMemory(&ig_console_proof,sizeof(ig_console_proof));papacc_server_console_win32_request_stop();ig_cp_capture(12345,54321);T(cp_open_calls==1&&ig_console_proof.facts.stopped&&!ig_cp_window_candidate(&ig_console_proof.facts));++cases;
 T(pss_capture_calls==0&&trust_calls==0);++cases;
 /* New adapter input tests: synthetic data only, never qualifier entry. */
 {IG_N0_FILE_RECORD record;char text[512],bad[512];int size;unsigned old=cp_open_calls;
 size=sprintf_s(text,sizeof(text),"PACC_N2_OPENED_FILE_V1\nvolume=1\nhigh=0\nlow=2\nsize=867840\nlinks=2\nsha256=%s\nsecurity_sha256=%s\nwindows_evidence_sha256=%s\n",
 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb","cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc");
 T(size>0&&ig_n0_parse(text,(size_t)size,&record)&&record.volume==1&&record.low==2&&record.size==867840&&record.links==2);++cases;
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
 b=a;b.configuration=(HANDLE)(ULONG_PTR)123;b.auxiliary.file=(HANDLE)(ULONG_PTR)456;T(!ig_n0_plan(&b,y)&&!strcmp(x,y));++cases;
 }
 printf("N0_CONTEXT_INPUT=MODEL_PASS; MISSING_NATIVE_PROVENANCE_DENY_BEFORE_OPEN; NO_INLINE_TRUST_NO_OPERATIONAL_ENTRY; STATE_BYTES=%llu\n",(ULONGLONG)sizeof(ig_n0));
 }
 printf("REJECTION_REDUCTION=PASS; CENSUS_DENY_PRESERVED; NO_REJECTION_OPEN_PROCESS_PSS_OR_TRUST; HISTORICAL_CAPTURE_FIXTURE_ONLY\n");
 printf("CONSOLE_CAPTURE_STORAGE bytes=%llu; NEW_REFS_PROCESS_FILE_TRUST_PLUS_SHARED_SNAPSHOT; NO_ADMISSION\n",(ULONGLONG)sizeof(ig_console_proof));
 printf("BUSY_DIAGNOSTIC_MODEL=PASS cases=%u diagnostic_bytes=%llu; API_BUSY_CENSUS_FIRST_ERROR_LAST_ERROR_STOP_RETENTION_CLOSE_NO_RETRY; NO_PREFLIGHT_NO_SCM_NO_START\n",cases,(unsigned long long)sizeof(IG_DIAGNOSTIC));return 0;
}
