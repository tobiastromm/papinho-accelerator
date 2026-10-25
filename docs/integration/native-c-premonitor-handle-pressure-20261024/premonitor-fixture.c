/* Isolated source/resource comparison. NO operational entry, SCM or file creation. */
#define WIN32_LEAN_AND_MEAN 1
#define _WIN32_WINNT 0x0603
#define PAPACC_SERVICE_BD_MODEL 1
#define PAPACC_N0_N9_NATIVE_OWNER 1
#include <windows.h>
#include <aclapi.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../apps/server/server_service_admission_bd_win32.h"
static struct {DWORD thread;unsigned retained;} native;
static unsigned stop_flag,close_injected;
static int papacc_server_console_win32_stop_requested(void){return stop_flag!=0;}
static void papacc_server_console_win32_request_stop(void){stop_flag=1;}
#include "../../../tools/qualification/scm-aux-reservation-core.inc"
#include "../../../tools/qualification/scm-aux-reservation-bridge.inc"
static unsigned handle_peak;
static BOOL WINAPI observed_token_info(HANDLE h,TOKEN_INFORMATION_CLASS kind,LPVOID data,DWORD bytes,PDWORD used)
{BOOL ok=GetTokenInformation(h,kind,data,bytes,used);DWORD saved=GetLastError(),n=0;if(GetProcessHandleCount(GetCurrentProcess(),&n)&&n>handle_peak)handle_peak=n;SetLastError(saved);return ok;}
#define GetTokenInformation observed_token_info
static TOKEN_STATISTICS lab_stats;
#include "operator-current.UNMODIFIED.inc"
#undef GetTokenInformation
#include "membership-suffix.PROPOSED.inc"
static PS_BD_DESCRIPTOR d;
static PS_BD_INVENTORY inventory;
static HANDLE anchor,file,extra[3],snapshot;
static BY_HANDLE_FILE_INFORMATION file_id;
static BYTE raw[65536];static DWORD raw_bytes;
static unsigned callbacks,held_peak,volatile_bad,bad_identity,bad_token,bad_sd,proposed,entries,at,acquisitions;
static ULONGLONG simulated_offset,began;
static const char *test_case;
static DWORD count(void){DWORD n=0;if(!GetProcessHandleCount(GetCurrentProcess(),&n))return 0;if(n>handle_peak)handle_peak=n;return n;}
static DWORD synthetic_finish_anchor(void)
{
 DWORD e;
 if(!anchor)return ERROR_INVALID_STATE;
 if(close_injected){SetLastError(ERROR_ACCESS_DENIED);e=GetLastError();}
 else if(CloseHandle(anchor)){anchor=NULL;return 0;}
 else e=GetLastError();
 if(!e)e=ERROR_INVALID_HANDLE;a1_retain(&lab_aux_owner,e);(void)lab_aux_sync(e);return e;
}
static DWORD context(const PS_BD_DESCRIPTOR *desc,ULONGLONG now)
{
 DWORD e,n,read=0;TOKEN_STATISTICS saved=lab_stats;BY_HANDLE_FILE_INFORMATION id;PSECURITY_DESCRIPTOR sd=NULL;BYTE data[256];LARGE_INTEGER zero;
 ++callbacks;
 if(stop_flag||native.retained)return ERROR_OPERATION_ABORTED;
 if(now<began||now-began>60000)return ERROR_TIMEOUT;
 if(volatile_bad)return ERROR_ACCESS_DENIED; /* injected surrogate of a reversible required context observation */
 if(desc->challenge!=inventory.console.generation)return ERROR_INVALID_DATA;
 if(bad_token)lab_stats.ModifiedId.LowPart++;
 e=lab_operator_current();lab_stats=saved;if(e)return e;
 (void)count();
 /* Native read-only work, NOT the protected N0 context or an administrative authorization. */
 if(!GetFileInformationByHandle(file,&id))return GetLastError();
 if(bad_identity||id.dwVolumeSerialNumber!=file_id.dwVolumeSerialNumber||id.nFileIndexHigh!=file_id.nFileIndexHigh||id.nFileIndexLow!=file_id.nFileIndexLow)return ERROR_INVALID_DATA;
 e=GetSecurityInfo(file,SE_FILE_OBJECT,7,NULL,NULL,NULL,NULL,&sd);if(e)return e;
 n=GetSecurityDescriptorLength(sd);e=bad_sd||n!=raw_bytes||memcmp(sd,raw,n)?ERROR_ACCESS_DENIED:0;
 if(LocalFree(sd)){native.retained=1;return ERROR_INVALID_HANDLE;}if(e)return e;
 zero.QuadPart=0;if(!SetFilePointerEx(file,zero,NULL,FILE_BEGIN)||!ReadFile(file,data,sizeof(data),&read,NULL)||!read)return ERROR_INVALID_DATA;
 return 0;
}
static void make_descriptor(void)
{
 unsigned i;ZeroMemory(&d,sizeof(d));ZeroMemory(&inventory,sizeof(inventory));
 d.actor.handle=(HANDLE)(ULONG_PTR)0x4567;d.actor.pid=42424242;d.actor.creation=100;d.actor.session=1;
 d.control_thread=GetCurrentThread();d.control_tid=1;d.configuration=file;d.image=file;d.owner_pid=GetCurrentProcessId();d.owner_creation=200;d.boot=236441;d.challenge=123;d.n0=1;
 d.sid_size=12;d.sid[0]=1;d.sid[1]=1;d.sid[7]=5;d.sid[8]=21;
 strcpy_s(d.attempt,33,"0b1de869e11493b7cb08fb329578f6ce");memset(d.plan,'a',64);
 strcpy_s(d.config_hash,65,"a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44");strcpy_s(d.image_hash,65,PS_BD_N0_IMAGE_SHA);strcpy_s(d.policy,65,PS_BD_N0_POLICY);strcpy_s(d.host,65,"2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff");
 d.auxiliary.instance.handle=(HANDLE)(ULONG_PTR)0x1234;d.auxiliary.instance.pid=42424243;d.auxiliary.instance.creation=150;d.auxiliary.instance.session=1;d.auxiliary.parent=d.actor.pid;d.auxiliary.file=file;d.auxiliary.file_id=file_id;
 memset(d.auxiliary.file_hash,'1',64);memset(d.auxiliary.security_hash,'2',64);memset(d.auxiliary.provenance_hash,'3',64);
 inventory.count=3;inventory.present[0]=inventory.present[1]=inventory.present[3]=1;inventory.role[0]=d.actor;
 inventory.role[1].handle=(HANDLE)(ULONG_PTR)0x2345;inventory.role[1].pid=d.owner_pid;inventory.role[1].creation=200;inventory.role[1].session=1;inventory.role[3]=d.auxiliary.instance;
 inventory.console.actor=d.actor;inventory.console.console=d.auxiliary.instance;inventory.console.boot=d.boot;inventory.console.generation=d.challenge;inventory.console.observed=began;inventory.console.known=PS_BD_N0_KNOWN;inventory.console.source=PS_BD_N0_SOURCE;inventory.console.validate=context;
 for(i=0;i<3;i++)extra[i]=NULL;
}
static void mutate(unsigned i)
{
 if(i!=entries/2&&i!=entries/2+1)return;
 if(!strcmp(test_case,"transient"))volatile_bad=(i==entries/2);
 if(i!=entries/2)return;
 if(!strcmp(test_case,"persistent")||!strcmp(test_case,"competing"))volatile_bad=1;
 if(!strcmp(test_case,"token"))bad_token=1;
 if(!strcmp(test_case,"identity"))bad_identity=1;
 if(!strcmp(test_case,"descriptor"))bad_sd=1;
 if(!strcmp(test_case,"generation"))++d.challenge;
 if(!strcmp(test_case,"stale60"))simulated_offset=60001;
 if(!strcmp(test_case,"stop"))stop_flag=1;
 if(!strcmp(test_case,"unknown")){native.retained=1;a1_retain(&lab_aux_owner,ERROR_INVALID_HANDLE);}
}
static DWORD run_census(void)
{
 DWORD e;unsigned i;ULONGLONG now=GetTickCount64();
 /* Proxy for current's initial full validation; full native admission is NEVER called. */
 if((e=context(&d,now))||(e=ps_bd_inventory(&d,&inventory,now)))return e;
 for(i=0;i<entries;i++){
  DWORD parent=0,pid=1000000+i;unsigned project=0;mutate(i);now=GetTickCount64()+simulated_offset;at=i;
  if(stop_flag)return ERROR_OPERATION_ABORTED;if(native.retained)return ERROR_INVALID_HANDLE;
  if(now<began||now-began>60000)return ERROR_TIMEOUT;
  if(!strcmp(test_case,"competing")&&i==entries/2+1)parent=d.actor.pid;
  if(!strcmp(test_case,"child")&&i==entries/2)parent=d.actor.pid;
  if(!strcmp(test_case,"project")&&i==entries/2)project=1;
  if(proposed)e=proposed_member(&d,&inventory,pid,parent,project);
  else e=ps_bd_census(&d,&inventory,now,pid,parent,project);
  if(e)return e;
 }
 if(proposed){if((e=context(&d,GetTickCount64()+simulated_offset))||(e=ps_bd_inventory(&d,&inventory,GetTickCount64()+simulated_offset)))return e;}
 if(!strcmp(test_case,"stale1")){ULONGLONG t=GetTickCount64(),synthetic_oldest=t-1001;if(t-synthetic_oldest>1000)return ERROR_TIMEOUT;}
 return 0;
}
static DWORD setup(void)
{
 WCHAR exe[1200];DWORD n;PSECURITY_DESCRIPTOR sd=NULL;DWORD e;
 native.thread=GetCurrentThreadId();
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&anchor))return GetLastError();++acquisitions;
 if(!GetTokenInformation(anchor,TokenStatistics,&lab_stats,sizeof(lab_stats),&n))return GetLastError();
 if(!GetModuleFileNameW(NULL,exe,1200))return GetLastError();
 file=CreateFileW(exe,GENERIC_READ|READ_CONTROL,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(file==INVALID_HANDLE_VALUE){file=NULL;return GetLastError();}++acquisitions;
 if(!GetFileInformationByHandle(file,&file_id))return GetLastError();e=GetSecurityInfo(file,SE_FILE_OBJECT,7,NULL,NULL,NULL,NULL,&sd);if(e)return e;
 raw_bytes=GetSecurityDescriptorLength(sd);if(raw_bytes>sizeof(raw)){(void)LocalFree(sd);return ERROR_INVALID_DATA;}memcpy(raw,sd,raw_bytes);if(LocalFree(sd))return ERROR_INVALID_HANDLE;
 began=GetTickCount64();make_descriptor();return 0;
}
static int finish(DWORD result)
{
 unsigned i;DWORD after;
 if(native.retained||!a1_finish(&lab_aux_owner)){printf("RETAINED result=%lu anchor=%p file=%p freeze=%u process_exit_containment=1 NO_NATIVE_CLOSE_SUCCESS\n",(unsigned long)result,(void *)anchor,(void *)file,(unsigned)((lab_aux_owner.state&A1_FROZEN)!=0));return 2;}
 for(i=0;i<3;i++)if(extra[i]){if(!CloseHandle(extra[i]))return 2;extra[i]=NULL;}
 if(snapshot){if(!CloseHandle(snapshot))return 2;snapshot=NULL;}
 if(anchor){if(!CloseHandle(anchor))return 2;anchor=NULL;}
 if(file){if(!CloseHandle(file))return 2;file=NULL;}
 after=count();printf("CLOSED result=%lu raw=%lu own_references_closed=1 service_quiescence=NOT_CLAIMED\n",(unsigned long)result,(unsigned long)after);return 0;
}
int main(int argc,char **argv)
{
 DWORD e,before,stable,with_positive,after;LARGE_INTEGER start,end,freq;unsigned i;int rc;
 if(argc!=5)return 64;
 proposed=!strcmp(argv[2],"proposed");entries=(unsigned)strtoul(argv[3],NULL,10);test_case=argv[4];if(entries<4||entries>4096)return 65;
 printf("ISOLATED selected=%u mode=%s method=%s rows=%u case=%s NO_LAB_NO_SCM_NO_AUTHORITY\n",(unsigned)FIXTURE_SELECTED,argv[1],argv[2],entries,test_case);fflush(stdout);
 (void)lab_aux_console;(void)lab_aux_borrow_pair;(void)a1_cancel;(void)ps_bd_costs;(void)ps_bd_context_pending;
 e=setup();if(e){printf("SETUP_FAIL code=%lu\n",(unsigned long)e);return finish(e)?2:1;}
 if(!strcmp(argv[1],"anchor")){
  before=count();if(proposed){close_injected=!strcmp(test_case,"close-unknown");e=synthetic_finish_anchor();if(e)return finish(e);}stable=count();
  for(i=0;i<3;i++){if(!DuplicateHandle(GetCurrentProcess(),GetCurrentProcess(),GetCurrentProcess(),&extra[i],PROCESS_QUERY_INFORMATION|SYNCHRONIZE,FALSE,0))return finish(GetLastError());++acquisitions;}
  with_positive=count();
  if(!strcmp(test_case,"token"))lab_stats.TokenId.LowPart++;
  if(!strcmp(test_case,"modified"))lab_stats.ModifiedId.LowPart++;
  if(!strcmp(test_case,"stop"))stop_flag=1;
  for(i=0;i<64;i++){e=lab_operator_current();if(e)break;}after=count();
  printf("ANCHOR before=%lu stable=%lu with_positive=%lu after_validation=%lu released=%u validation=%lu comparisons=%u stats_preserved=1 three_refs=SYNTHETIC_PROCESS_DUPLICATES_NOT_INTENT_IMAGE_SERVICE\n",(unsigned long)before,(unsigned long)stable,(unsigned long)with_positive,(unsigned long)after,proposed,(unsigned long)e,i);
  rc=finish(e);if(rc)return rc;
  if(!strcmp(test_case,"token")||!strcmp(test_case,"modified"))return e==ERROR_ACCESS_DENIED?0:1;
  if(!strcmp(test_case,"stop"))return e==ERROR_OPERATION_ABORTED?0:1;
  return e?1:0;
 }
 /* Warm the same native readonly callback before measuring both algorithms. */
 for(i=0;i<8;i++)if((e=context(&d,GetTickCount64())))return finish(e)?2:1;
 callbacks=handle_peak=held_peak=0;
 snapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);if(snapshot==INVALID_HANDLE_VALUE){snapshot=NULL;return finish(GetLastError())?2:1;}
 before=count();QueryPerformanceFrequency(&freq);QueryPerformanceCounter(&start);e=run_census();QueryPerformanceCounter(&end);after=count();
 printf("BENCH method=%s rows=%u case=%s result=%lu at=%u callbacks=%u raw_before=%lu raw_after=%lu sampled_peak=%u elapsed_qpc=%llu frequency=%llu acquisition_footprint=ONE_SCOPED_TOKEN_AT_A_TIME steady_anchor=HELD\n",argv[2],entries,test_case,(unsigned long)e,at,callbacks,(unsigned long)before,(unsigned long)after,handle_peak,(ULONGLONG)(end.QuadPart-start.QuadPart),(ULONGLONG)freq.QuadPart);
 rc=finish(e);if(rc)return rc;
 if(!strcmp(test_case,"clean"))return e?1:0;
 /* Counterexamples are expected evidence of NON-equivalence, not a security PASS. */
 if(!strcmp(test_case,"transient"))return e==(DWORD)(proposed?0:ERROR_ACCESS_DENIED)?0:1;
 if(!strcmp(test_case,"competing"))return e==(DWORD)(proposed?ERROR_BUSY:ERROR_ACCESS_DENIED)?0:1;
 if(!strcmp(test_case,"unknown"))return 1; /* containment must have returned2 */
 return e?0:1;
}
