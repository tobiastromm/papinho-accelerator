/* Same native host; substitutions are isolated to this model translation unit. */
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include "server_console_win32.h"
#include "server_service_identity_win32.h"
static unsigned dispatches,registrations,reports,parks,runs,delivery,fail_report,fail_register,stop_on_running,model_stop_flag;
static DWORD states[8],errors[8];
static const wchar_t model_expected_name[]=PAPACC_SCM_NAME_LITERAL;
static BOOL WINAPI m_dispatch(const SERVICE_TABLE_ENTRYW *table)
{
 wchar_t *v[2];dispatches++;
 if(!table||!table[0].lpServiceName||wcscmp(table[0].lpServiceName,model_expected_name)||!table[0].lpServiceProc||table[1].lpServiceName||table[1].lpServiceProc)return FALSE;
 if(!delivery){SetLastError(ERROR_FAILED_SERVICE_CONTROLLER_CONNECT);return FALSE;}
 v[0]=(wchar_t *)model_expected_name;v[1]=L"extra";if(delivery==2U)v[0]=L"WrongService";
 table[0].lpServiceProc(delivery==3U?2U:(delivery==4U?0U:1U),delivery==4U?NULL:v);return TRUE;
}
static SERVICE_STATUS_HANDLE WINAPI m_register(LPCWSTR name,LPHANDLER_FUNCTION_EX fn,LPVOID ctx)
{registrations++;if(wcscmp(name,model_expected_name)||!fn||ctx||fail_register)return NULL;return (SERVICE_STATUS_HANDLE)(ULONG_PTR)9;}
static BOOL WINAPI m_report(SERVICE_STATUS_HANDLE h,LPSERVICE_STATUS s)
{
 if(h!=(SERVICE_STATUS_HANDLE)(ULONG_PTR)9||reports>=8U)return FALSE;
 if(s->dwControlsAccepted!=(s->dwCurrentState==SERVICE_RUNNING?(SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN):0U))return FALSE;
 states[reports]=s->dwCurrentState;errors[reports++]=s->dwWin32ExitCode;
 if(stop_on_running&&s->dwCurrentState==SERVICE_RUNNING)model_stop_flag=1U;
 return fail_report==reports?FALSE:TRUE;
}
static void WINAPI m_sleep(DWORD ms){if(ms==INFINITE)parks++;}
static PAPACC_BOOL m_stopped(void){return model_stop_flag?PAPACC_TRUE:PAPACC_FALSE;}
static void m_stop(void){model_stop_flag=1U;}
#define StartServiceCtrlDispatcherW m_dispatch
#define RegisterServiceCtrlHandlerExW m_register
#define SetServiceStatus m_report
#define Sleep m_sleep
#define papacc_server_console_win32_stop_requested m_stopped
#define papacc_server_console_win32_request_stop m_stop
#include "../apps/server/server_service_win32.c"
#define T(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static int m_run(int argc,char **argv){(void)argc;(void)argv;runs++;return 0;}
static void reset(void)
{
 /* Independent simulated processes; no reset of a factual live Stop. */
 ZeroMemory(&host,sizeof(host));ZeroMemory(states,sizeof(states));ZeroMemory(errors,sizeof(errors));
 dispatches=registrations=reports=parks=runs=delivery=fail_report=fail_register=stop_on_running=model_stop_flag=0U;
}
static void callback(void){reset();host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)9;host.thread=GetCurrentThreadId();host.run_active=1U;(void)ps_begin(&host.life);}
int main(void)
{
 char *v[]={"absolute-model-image","service","host","extra"};unsigned n;
 T(!ps_binding_ready());T(papacc_server_service_dispatch(2,v,m_run)==-1&&!dispatches);
 T(papacc_server_service_dispatch(4,v,m_run)==1&&!dispatches);
 v[2]="Host";T(papacc_server_service_dispatch(3,v,m_run)==-1);v[2]="host";
 T(papacc_server_service_dispatch(3,v,NULL)==1&&!dispatches);
 T(papacc_server_service_dispatch(3,v,m_run)==2&&dispatches==1U&&!registrations&&!runs);
 reset();delivery=1U;T(papacc_server_service_dispatch(3,v,m_run)==2);
 T(registrations==1U&&reports==2U&&states[0]==SERVICE_START_PENDING&&states[1]==SERVICE_STOPPED);
 T(errors[1]==ERROR_ACCESS_DENIED&&host.life.terminal&&!runs&&!parks);
 n=dispatches;T(papacc_server_service_dispatch(3,v,m_run)==1&&dispatches==n);
#ifdef PAPACC_SCM_REGISTRATION_LAB
 reset();delivery=2U;T(papacc_server_service_dispatch(3,v,m_run)==2&&!registrations&&!reports);
 reset();delivery=3U;T(papacc_server_service_dispatch(3,v,m_run)==2&&!registrations&&!reports);
 reset();delivery=4U;T(papacc_server_service_dispatch(3,v,m_run)==2&&!registrations&&!reports);
#endif
 reset();delivery=1U;fail_register=1U;T(papacc_server_service_dispatch(3,v,m_run)==2&&!reports&&!runs);
 reset();delivery=1U;fail_report=1U;T(papacc_server_service_dispatch(3,v,m_run)==2&&parks==1U&&host.life.phase==PS_RETAINED&&!runs);
 reset();delivery=1U;fail_report=2U;T(papacc_server_service_dispatch(3,v,m_run)==2&&parks==1U&&host.life.phase==PS_RETAINED&&!runs);
 /* Existing producer hooks exercised with model-owned callback state only.
  * No synthetic readiness can reach real ServiceMain or its closed gate. */
 callback();host.thread++;T(!papacc_server_service_runtime_ready()&&!reports);papacc_server_service_runtime_closed();T(!host.terminal_receipt);
 callback();host.run_active=0U;T(!papacc_server_service_runtime_ready()&&!reports);
 callback();T(ps_handler(SERVICE_CONTROL_STOP,0,NULL,NULL)==NO_ERROR);
 T(!papacc_server_service_runtime_ready()&&reports==1U&&states[0]==SERVICE_STOP_PENDING);
 T(!host.terminal_receipt&&host.life.phase==PS_STOP_PENDING);papacc_server_service_runtime_closed();T(host.terminal_receipt==1U&&host.life.phase==PS_STOPPED&&reports==1U);
 callback();stop_on_running=1U;T(!papacc_server_service_runtime_ready()&&reports==2U&&states[0]==SERVICE_RUNNING&&states[1]==SERVICE_STOP_PENDING&&!host.terminal_receipt);
 callback();T(papacc_server_service_runtime_ready()&&host.life.phase==PS_RUNNING);
 T(ps_handler(SERVICE_CONTROL_STOP,0,NULL,NULL)==NO_ERROR&&host.life.phase==PS_RUNNING);
 T(papacc_server_service_runtime_stopping()&&host.life.phase==PS_STOP_PENDING&&!host.terminal_receipt);
 n=reports;T(papacc_server_service_runtime_stopping()&&reports==n);papacc_server_service_runtime_closed();T(host.terminal_receipt==1U&&host.life.phase==PS_STOPPED&&reports==n);
 papacc_server_service_runtime_retained();T(host.terminal_receipt==1U);
 callback();papacc_server_service_runtime_retained();T(host.life.phase==PS_RETAINED&&host.terminal_receipt==2U);
 papacc_server_service_runtime_closed();T(host.life.phase==PS_RETAINED&&host.terminal_receipt==2U);
 callback();host.run_active=0U;host.result=0;T(!host.terminal_receipt&&host.life.phase==PS_START_PENDING);ps_park_retained();T(parks==1U&&host.life.phase==PS_RETAINED&&!reports);
 callback();T(ps_handler(SERVICE_CONTROL_INTERROGATE,0,NULL,NULL)==NO_ERROR&&!model_stop_flag);
 T(ps_handler(12345,0,NULL,NULL)==ERROR_CALL_NOT_IMPLEMENTED&&!model_stop_flag);
 T(ps_handler(SERVICE_CONTROL_SHUTDOWN,0,NULL,NULL)==NO_ERROR&&model_stop_flag&&!reports);
 printf("Host dispatcher/name/args/denied-start/readiness-race/Stop/quiescence/retention MODEL PASS; native=%lu host=%lu atomic=%lu\n",(unsigned long)sizeof(native),(unsigned long)sizeof(host),(unsigned long)sizeof(cli_active));return 0;
}
