/* Isolated actual applied branch; no LAB, files, SCM or admission. */
#define WIN32_LEAN_AND_MEAN 1
#define _WIN32_WINNT 0x0603
#include <windows.h>
#include <stdio.h>
#include <string.h>
static struct {DWORD thread;unsigned retained;} native;
static unsigned stopped,injected,close_calls,peak;
static int papacc_server_console_win32_stop_requested(void){return stopped!=0;}
static void papacc_server_console_win32_request_stop(void){stopped=1;}
#include "../../../tools/qualification/scm-aux-reservation-core.inc"
#include "../../../tools/qualification/scm-aux-reservation-bridge.inc"
static HANDLE lab_token;
static TOKEN_STATISTICS lab_stats;
static const char *cycle_stage;
static DWORD lab_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
static DWORD count(void){DWORD n=0;if(!GetProcessHandleCount(GetCurrentProcess(),&n))return 0;if(n>peak)peak=n;return n;}
static BOOL WINAPI observe_info(HANDLE h,TOKEN_INFORMATION_CLASS c,LPVOID p,DWORD n,PDWORD used){BOOL r=GetTokenInformation(h,c,p,n,used);DWORD e=GetLastError();(void)count();SetLastError(e);return r;}
#define GetTokenInformation observe_info
#include "operator-current.UNMODIFIED.inc"
#undef GetTokenInformation
static BOOL WINAPI branch_close(HANDLE h){++close_calls;if(injected){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return CloseHandle(h);}
#define CloseHandle branch_close
#include "applied-branch.UNMODIFIED.inc"
#undef CloseHandle
int main(int argc,char **argv)
{
 DWORD n,before,after,final,e=0;unsigned i;int applied;
 (void)lab_aux_console;(void)lab_aux_borrow_pair;(void)a1_cancel;(void)cycle_stage;
 if(argc!=3)return 64;applied=!strcmp(argv[1],"applied");native.thread=GetCurrentThreadId();
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&lab_token))return 65;
 if(!GetTokenInformation(lab_token,TokenStatistics,&lab_stats,sizeof(lab_stats),&n))return 66;
 before=count();injected=!strcmp(argv[2],"unknown");
 if(applied)e=test_after_initial_success(1,1);after=count();
 if(injected){
  unsigned old=close_calls;DWORD prior=lab_aux_owner.first_error;
  if(e!=ERROR_ACCESS_DENIED||!lab_token||!native.retained||!stopped||!(lab_aux_owner.state&A1_FROZEN))return 67;
  if(lab_operator_current()!=prior||close_calls!=old||!lab_token)return 68;
  printf("UNKNOWN original_retained=1 branch_close_calls=%u no_retry=1 no_new_acquisition=1 process_exit_containment=1 NOT_NATIVE_CLOSE_SUCCESS\n",close_calls);return 2;
 }
 if(e||((int)after-(int)before)!=(applied?-1:0))return 69;
 if(!strcmp(argv[2],"token"))++lab_stats.TokenId.LowPart;
 if(!strcmp(argv[2],"modified"))++lab_stats.ModifiedId.LowPart;
 if(!strcmp(argv[2],"stop"))stopped=1;
 for(i=0;i<64;i++){e=lab_operator_current();if(e)break;}
 if(!strcmp(argv[2],"clean")){if(e||i!=64)return 70;}else if(!e)return 71;
 final=count();if(final!=after||!a1_finish(&lab_aux_owner)||native.retained)return 72;
 if(lab_token){if(!CloseHandle(lab_token))return 73;lab_token=NULL;}
 printf("NATIVE mode=%s case=%s before=%lu after=%lu delta=%d transient_peak=%u current_calls=%u result=%lu close_calls=%u all_own_closed=1 NO_LAB_NO_AUTHORITY\n",argv[1],argv[2],(unsigned long)before,(unsigned long)after,(int)after-(int)before,peak,(i==64?64:i+1),(unsigned long)e,close_calls);
 return 0;
}
