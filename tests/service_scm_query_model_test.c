/* SAME native query/close code, only the four SCM observation functions
 * substituted in this isolated TU. No fake handle reaches a real close. */
#include <windows.h>
#include <stdio.h>
static unsigned opens,closes,mode;
static SC_HANDLE WINAPI model_manager(LPCWSTR a,LPCWSTR b,DWORD c)
{(void)a;(void)b;(void)c;opens++;return (SC_HANDLE)(ULONG_PTR)1;}
static SC_HANDLE WINAPI model_open(SC_HANDLE a,LPCWSTR b,DWORD c)
{(void)a;(void)b;(void)c;if(mode==1){SetLastError(ERROR_SERVICE_DOES_NOT_EXIST);return NULL;}
 if(mode==2){SetLastError(ERROR_ACCESS_DENIED);return NULL;}return (SC_HANDLE)(ULONG_PTR)2;}
static BOOL WINAPI model_query(SC_HANDLE a,SC_STATUS_TYPE b,LPBYTE p,DWORD n,LPDWORD z)
{SERVICE_STATUS_PROCESS *s=(SERVICE_STATUS_PROCESS *)p;(void)a;(void)b;
 if(mode==3){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(n<sizeof(*s))return FALSE;ZeroMemory(s,sizeof(*s));s->dwCurrentState=SERVICE_STOPPED;*z=sizeof(*s);return TRUE;}
static BOOL WINAPI model_close(SC_HANDLE a)
{(void)a;closes++;if(mode==4){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(mode==5&&a==(SC_HANDLE)(ULONG_PTR)1){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static unsigned status_calls,park_calls,runtime_calls,host_mode;static DWORD observed_states[8];
static SERVICE_STATUS_HANDLE WINAPI model_register(LPCWSTR name,LPHANDLER_FUNCTION_EX fn,LPVOID ctx)
{(void)name;(void)fn;(void)ctx;if(host_mode==1){SetLastError(ERROR_ACCESS_DENIED);return NULL;}
 return (SERVICE_STATUS_HANDLE)(ULONG_PTR)3;}
static BOOL WINAPI model_report(SERVICE_STATUS_HANDLE handle,LPSERVICE_STATUS status)
{
 (void)handle;if(status_calls>=8||(status->dwCurrentState==SERVICE_RUNNING?status->dwControlsAccepted!=(SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN):status->dwControlsAccepted!=0)){SetLastError(ERROR_INVALID_DATA);return FALSE;}
 observed_states[status_calls++]=status->dwCurrentState;
 if(host_mode==2||(host_mode==3&&status->dwCurrentState==SERVICE_STOPPED)){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 return TRUE;
}
static void WINAPI model_park(DWORD duration){if(duration==INFINITE)park_calls++;}
static int model_runtime(int argc,char **argv){(void)argc;(void)argv;runtime_calls++;return 0;}
#define RegisterServiceCtrlHandlerExW model_register
#define SetServiceStatus model_report
#define Sleep model_park
#define OpenSCManagerW model_manager
#define OpenServiceW model_open
#define QueryServiceStatusEx model_query
#define CloseServiceHandle model_close
#include "../apps/server/server_service_win32.c"
#define T(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
int main(void)
{
 const char *v[]={"p","service","status"};FILE *o=tmpfile(),*e=tmpfile();unsigned n;
 if(!o||!e)return 1;
 T(papacc_server_service_cli(3,v,o,e,NULL)==0);T(opens==1&&closes==2);
 mode=1;T(papacc_server_service_cli(3,v,o,e,NULL)==0);T(closes==3);
 mode=2;T(papacc_server_service_cli(3,v,o,e,NULL)==2);T(closes==4);
 mode=3;T(papacc_server_service_cli(3,v,o,e,NULL)==2);T(closes==6);
 mode=4;T(papacc_server_service_cli(3,v,o,e,NULL)==2);T(native.retained&&native.service&&native.manager);
 n=closes;T(papacc_server_service_cli(3,v,o,e,NULL)==2);T(closes==n);
 /* New isolated MODEL owner after previous simulated process containment. */
 ZeroMemory(&native,sizeof(native));InterlockedExchange(&cli_active,0);mode=5;
 T(papacc_server_service_cli(3,v,o,e,NULL)==2);T(!native.service&&native.manager&&native.retained);
 n=closes;T(papacc_server_service_cli(3,v,o,e,NULL)==2);T(closes==n);
 /* Independent simulated ServiceMain instances; all handles/reporting are
  * model-only and model Sleep records retention rather than actually waiting. */
 ZeroMemory(&host,sizeof(host));host.run=model_runtime;host_mode=0;ps_main(0,NULL);
 T(status_calls==2&&observed_states[0]==SERVICE_START_PENDING&&observed_states[1]==SERVICE_STOPPED);
 T(host.life.terminal&&host.result==2&&runtime_calls==0&&park_calls==0);
 ps_main(0,NULL);T(status_calls==2); /* no repeated terminal report */
 ZeroMemory(&host,sizeof(host));status_calls=0;host_mode=1;ps_main(0,NULL);
 T(!host.handle&&status_calls==0&&runtime_calls==0);
 ZeroMemory(&host,sizeof(host));status_calls=0;host_mode=2;ps_main(0,NULL);
 T(status_calls==1&&host.life.phase==PS_RETAINED&&park_calls==1&&runtime_calls==0);
 ZeroMemory(&host,sizeof(host));status_calls=0;host_mode=3;ps_main(0,NULL);
 T(status_calls==2&&host.life.phase==PS_RETAINED&&park_calls==2&&runtime_calls==0);
 /* Ready receipt MODEL before any real Stop has been requested. */
 ZeroMemory(&host,sizeof(host));host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)3;host.thread=GetCurrentThreadId();host.run_active=1;host_mode=0;status_calls=0;T(ps_begin(&host.life));
 T(papacc_server_service_host_active());T(papacc_server_service_runtime_ready());T(host.life.phase==PS_RUNNING&&status_calls==1&&observed_states[0]==SERVICE_RUNNING);
 T(!papacc_server_service_runtime_ready());T(papacc_server_service_runtime_stopping());T(host.life.phase==PS_STOP_PENDING&&status_calls==2);papacc_server_service_runtime_closed();T(host.life.phase==PS_STOPPED&&host.terminal_receipt==1&&status_calls==2);
 ZeroMemory(&host,sizeof(host));host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)3;host.thread=GetCurrentThreadId()+1;host.run_active=1;T(ps_begin(&host.life));
 T(!papacc_server_service_host_active()&&!papacc_server_service_runtime_ready());papacc_server_service_runtime_closed();T(!host.terminal_receipt);
 n=closes;T(ps_handler(SERVICE_CONTROL_INTERROGATE,0,NULL,NULL)==NO_ERROR);
 T(ps_handler(999,0,NULL,NULL)==ERROR_CALL_NOT_IMPLEMENTED);
 T(ps_handler(SERVICE_CONTROL_STOP,0,NULL,NULL)==NO_ERROR);
 T(InterlockedCompareExchange(&host.stop,0,0)==1&&papacc_server_console_win32_stop_requested()==PAPACC_TRUE);
 T(ps_handler(SERVICE_CONTROL_SHUTDOWN,0,NULL,NULL)==NO_ERROR);T(closes==n&&runtime_calls==0);
 /* Runtime producer receipt MODEL: missing/wrong thread, Stop before ready,
  * ready state, retained teardown and no fabricated STOPPED. */
 ZeroMemory(&host,sizeof(host));T(papacc_server_service_runtime_ready());
 host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)3;host.thread=GetCurrentThreadId();T(ps_begin(&host.life));
 T(!papacc_server_service_runtime_ready());host.run_active=1;host_mode=0;status_calls=0;
 /* Existing Stop bridge from previous handler tests is monotonic; test only
  * Stop-before-ready here, without clearing a factual live Stop. */
 T(!papacc_server_service_runtime_ready());T(host.life.phase==PS_STOP_PENDING);
 T(status_calls==1&&observed_states[0]==SERVICE_STOP_PENDING);
 papacc_server_service_runtime_closed();T(host.terminal_receipt==1&&host.life.phase==PS_STOPPED);
 papacc_server_service_runtime_retained();T(host.terminal_receipt==1); /* terminal cannot be overwritten */
 ZeroMemory(&host,sizeof(host));host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)3;host.thread=GetCurrentThreadId();host.run_active=1;T(ps_begin(&host.life));
 papacc_server_service_runtime_retained();T(host.terminal_receipt==2&&host.life.phase==PS_RETAINED);
 papacc_server_service_runtime_closed();T(host.terminal_receipt==2&&host.life.phase==PS_RETAINED);
 T(fclose(o)==0);T(fclose(e)==0);
 puts("Modeled SCM observations/access-denied/partial close/retention/no retry/ServiceMain/control bridge PASS");return 0;
}
