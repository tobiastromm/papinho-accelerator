/* Stop/wake lifetime model; no event, wait or handler reaches Windows. */
#include <windows.h>
#include <stdio.h>
static unsigned mode,signals,waits;
static BOOL WINAPI mw_flags(HANDLE h,LPDWORD flags){if(h!=(HANDLE)1)return FALSE;*flags=mode==1U?HANDLE_FLAG_INHERIT:0;return TRUE;}
static BOOL WINAPI mw_signal(HANDLE h){signals++;return h==(HANDLE)1&&mode!=4U;}
static DWORD WINAPI mw_wait(DWORD n,const HANDLE *h,BOOL all,DWORD ms);
static BOOL WINAPI mw_handler(PHANDLER_ROUTINE f,BOOL on){(void)f;(void)on;return TRUE;}
#define GetHandleInformation mw_flags
#define SetEvent mw_signal
#define WaitForMultipleObjects mw_wait
#define SetConsoleCtrlHandler mw_handler
#include "../apps/server/server_console_win32.c"
static DWORD WINAPI mw_wait(DWORD n,const HANDLE *h,BOOL all,DWORD ms)
{
 waits++;if(n!=2U||h[0]!=(HANDLE)1||h[1]!=(HANDLE)2||all||ms!=99U)return WAIT_FAILED;
 if(mode==2U||mode==4U)papacc_server_console_win32_request_stop();
 if(mode==3U)InterlockedExchange(&papacc_server_console_wait_users,1);
 return mode==5U?WAIT_OBJECT_0+1U:mode==6U?WAIT_TIMEOUT:WAIT_OBJECT_0;
}
#define W(x) do{if(!(x)){printf("FAIL wait line=%d mode=%u\n",__LINE__,mode);return 1;}}while(0)
int main(void)
{
 DWORD result;unsigned i;
 for(i=0;i<7U;i++){
  /* Independent simulated process state, never rearm a native live Stop. */
  mode=i;signals=waits=0;papacc_server_console_stop_requested=0;papacc_server_console_wait_event=NULL;papacc_server_console_wait_users=0;papacc_server_console_wait_error=0;
  W(papacc_server_console_win32_wait_original((HANDLE)1,(HANDLE)2,99U,&result)==(i==1U?PAPACC_RESULT_INVALID_ARGUMENT:(i==3U||i==4U)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK));
  W(!papacc_server_console_wait_event);
  if(i==2U)W(signals==1U&&papacc_server_console_win32_stop_requested());
  if(i==5U)W(result==WAIT_OBJECT_0+1U);
  if(i==6U)W(result==WAIT_TIMEOUT);
  if(i==1U)W(!waits&&!signals);
  if(i==3U)W(result==WAIT_FAILED&&papacc_server_console_wait_users==1);
  if(i==4U)W(result==WAIT_FAILED&&papacc_server_console_wait_error);
 }
 puts("PASS CONSOLE_ORIGINAL_WAIT cases=7 STOP_WAKE LEASE_BUSY_RETAINS SIGNAL_FAILURE_UNKNOWN PEER_EXIT TIMEOUT NO_REAL_WAIT_OR_EVENT");return 0;
}
