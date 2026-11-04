/* Same native host; substitutions are isolated to this model translation unit. */
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include "server_console_win32.h"
#include "server_service_win32.h"
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
#ifndef PAPACC_HOST_CANONICAL_EMPTY
int papacc_server_launcher_scq_closed_arm_running(void){return 0;}
int papacc_server_launcher_scq_closed_evidence_current(void){return 0;} /* no originals in this host-only model */
int papacc_server_launcher_scm_registration_present(const PAPACC_SERVICE_REGISTRATION_VIEW *v,FILE *out,FILE *err){(void)v;(void)out;(void)err;return 2;}
DWORD papacc_server_launcher_scm_registration_target(WCHAR *out,size_t cap){(void)out;(void)cap;return ERROR_NOT_SUPPORTED;}
DWORD papacc_server_launcher_scm_registration_current(const PAPACC_SERVICE_REGISTRATION_VIEW *v){(void)v;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_take(const PAPACC_SERVICE_REGISTRATION_VIEW *v){(void)v;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_finish(const PAPACC_SERVICE_REGISTRATION_VIEW *v,int closed){(void)v;(void)closed;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_resource_report(const PAPACC_SERVICE_REGISTRATION_VIEW *v,PAPACC_SCM_RESOURCE_REPORT *out){(void)v;(void)out;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_effect(const PAPACC_SERVICE_REGISTRATION_VIEW *v,unsigned effect,unsigned returned,DWORD result)
{(void)v;(void)effect;(void)returned;(void)result;return ERROR_ACCESS_DENIED;}
#endif
#include "../apps/server/server_service_win32.c"
/* Negative source only: carrier DATA cannot create an operational act. */
#ifndef PAPACC_HOST_CANONICAL_EMPTY
DWORD papacc_server_launcher_scq_first_controller(void){return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_resource_current(void){return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_namespace_prepare(void){return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_prestart_close(void){return 0;}
DWORD papacc_server_launcher_scq_start_carrier(PS_SCQ_CARRIER *out){(void)out;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_take(const PAPACC_SERVICE_IMAGE_VIEW *view){(void)view;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_progress(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_wait(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_wait_terminal(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_finish(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scq_start_namespace_current(void){return ERROR_ACCESS_DENIED;}
#endif
#define T(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
#ifdef PAPACC_HOST_CANONICAL_EMPTY
int papacc_server_launcher_scq_closed_prepare(void);
#endif
static int m_run(int argc,char **argv){(void)argc;(void)argv;runs++;
#ifdef PAPACC_HOST_CANONICAL_EMPTY
 if(papacc_server_service_scq_preparation_current())return papacc_server_launcher_scq_closed_prepare();
#endif
 return 0;}

static void reset(void)
{
 /* Independent simulated processes; no reset of a factual live Stop. */
 ZeroMemory(&host,sizeof(host));ZeroMemory(states,sizeof(states));ZeroMemory(errors,sizeof(errors));
 dispatches=registrations=reports=parks=runs=delivery=fail_report=fail_register=stop_on_running=model_stop_flag=0U;
}
static void callback(void){reset();host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)9;host.thread=GetCurrentThreadId();host.run_active=1U;(void)ps_begin(&host.life);}
#ifdef PAPACC_HOST_CANONICAL_EMPTY
int papacc_server_launcher_scq_original_cli(int,const char *const *,FILE *,FILE *);
#endif
int main(void)
{
 char *v[]={"absolute-model-image","service","host","extra"};unsigned n;PAPACC_SERVICE_STARTUP_SOURCE_REPORT source;
#ifdef PAPACC_HOST_CANONICAL_EMPTY
 {const char *missing_originals[]={"model-image","--owner-manual-first-scq-start"};
  T(papacc_server_launcher_scq_original_cli(2,missing_originals,stdout,stderr)==1);
 }
 /* Actual launcher bridges: empty owner cannot fabricate an act or close an unrelated owner. */
 T(papacc_server_launcher_scq_first_controller()!=0);
 T(papacc_server_launcher_scq_prestart_close()==0);
 T(!dispatches&&!registrations&&!reports&&!runs);
#endif
 {PAPACC_SERVICE_DIAGNOSTIC_START_REPORT diagnostic;
  T(papacc_server_service_diagnostic_start_status(NULL)==ERROR_INVALID_PARAMETER);
  T(papacc_server_service_diagnostic_start_status(&diagnostic)==ERROR_ACCESS_DENIED);
  T(diagnostic.missing_pre_start==15U&&diagnostic.later_obligations==15U&&diagnostic.initial_receiver_source_unavailable==1U);
  T(!diagnostic.available_metadata&&!dispatches&&!registrations&&!reports&&!runs);
 }
 T(papacc_server_service_startup_source_status(NULL)==ERROR_INVALID_PARAMETER);
 T(papacc_server_service_startup_source_status(&source)==ERROR_ACCESS_DENIED&&source.first_reason==0&&source.missing_source_fields==31U);
 T(!papacc_server_service_scq_host_current());T(!ps_binding_ready());
 T(papacc_server_service_startup_source_status(&source)==ERROR_ACCESS_DENIED&&source.first_reason==source.reason);
 /* Fully populated observations cannot be substituted for an original act.
  * No file/token/query/SCM API is executed for these modeled metadata values. */
 native.image=(HANDLE)(ULONG_PTR)41;native.image_ready=1U;native.service=(SC_HANDLE)(ULONG_PTR)42;native.readback=1U;
 {PAPACC_SERVICE_DIAGNOSTIC_START_REPORT diagnostic;
  T(papacc_server_service_diagnostic_start_status(&diagnostic)==ERROR_ACCESS_DENIED);
  T(diagnostic.available_metadata==3U&&diagnostic.missing_pre_start==15U&&diagnostic.later_obligations==15U);
  T(!dispatches&&!registrations&&!reports&&!runs);
 }
 T(!ps_binding_ready());T(papacc_server_service_startup_source_status(&source)==ERROR_ACCESS_DENIED&&source.available_metadata==3U&&source.missing_source_fields==31U);
 native.retained=1U;T(papacc_server_service_startup_source_status(&source)==ERROR_ACCESS_DENIED&&source.reason==PS_STARTUP_RETAINED_UNKNOWN&&source.first_reason!=PS_STARTUP_RETAINED_UNKNOWN);
 native.retained=0U;native.thread=GetCurrentThreadId()+1U;T(papacc_server_service_startup_source_status(&source)==ERROR_ACCESS_DENIED&&source.reason==PS_STARTUP_WRONG_THREAD);native.thread=0U;
 model_stop_flag=1U;T(papacc_server_service_startup_source_status(&source)==ERROR_ACCESS_DENIED&&source.reason==PS_STARTUP_STOPPED);model_stop_flag=0U;
 native.image=NULL;native.image_ready=0U;native.service=NULL;native.readback=0U;T(papacc_server_service_dispatch(2,v,m_run)==-1&&!dispatches);
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
 reset();v[3]="scq-closed";delivery=1U;
#ifdef PAPACC_SCM_REGISTRATION_LAB
 T(papacc_server_service_dispatch(4,v,m_run)==1&&!dispatches&&!registrations&&!runs);
#else
#ifdef PAPACC_HOST_CANONICAL_EMPTY
 T(papacc_server_service_dispatch(4,v,m_run)==2&&runs==1U&&!parks&&host.life.phase==PS_STOPPED&&host.terminal_receipt==1U);
 puts("CANONICAL_CLOSED_EMPTY_OWNER_ACQUIRE_FINISH PASS NO_ORIGINAL_NO_ENDPOINT_NO_CNG_NO_PRODUCTION");
#else
 T(papacc_server_service_dispatch(4,v,m_run)==2&&runs==1U&&parks==1U&&host.life.phase==PS_RETAINED);
#endif
 T(!host.closed_ready&&!papacc_server_service_scq_closed_ready_current()&&!papacc_server_service_runtime_ready());
 reset();delivery=2U;T(papacc_server_service_dispatch(4,v,m_run)==2&&!runs&&!registrations);
#endif
 v[3]="extra";
 /* Existing producer hooks exercised with model-owned callback state only.
  * No synthetic readiness can reach real ServiceMain or its closed gate. */
 callback();host.thread++;T(!papacc_server_service_runtime_ready()&&!reports);papacc_server_service_runtime_closed();T(!host.terminal_receipt);
 callback();host.run_active=0U;T(!papacc_server_service_runtime_ready()&&!reports);
 callback();T(ps_handler(SERVICE_CONTROL_STOP,0,NULL,NULL)==NO_ERROR);
 T(!papacc_server_service_runtime_ready()&&reports==1U&&states[0]==SERVICE_STOP_PENDING);
 T(!host.terminal_receipt&&host.life.phase==PS_STOP_PENDING);papacc_server_service_runtime_closed();T(host.terminal_receipt==1U&&host.life.phase==PS_STOPPED&&reports==1U);
 callback();stop_on_running=1U;T(!papacc_server_service_runtime_ready()&&reports==2U&&states[0]==SERVICE_RUNNING&&states[1]==SERVICE_STOP_PENDING&&!host.terminal_receipt);
 callback();T(!papacc_server_service_scq_host_current());T(papacc_server_service_runtime_ready()&&host.life.phase==PS_RUNNING);
#ifdef PAPACC_SCM_REGISTRATION_LAB
 T(!papacc_server_service_scq_host_current());
#else
 T(papacc_server_service_scq_host_current());T(!ps_binding_ready());T(papacc_server_service_startup_source_status(&source)==ERROR_ACCESS_DENIED&&source.reason==PS_STARTUP_POST_RUNNING_CIRCULAR);host.thread++;T(!papacc_server_service_scq_host_current());host.thread--;
#endif
 T(ps_handler(SERVICE_CONTROL_STOP,0,NULL,NULL)==NO_ERROR&&host.life.phase==PS_RUNNING);
 T(papacc_server_service_runtime_stopping()&&host.life.phase==PS_STOP_PENDING&&!host.terminal_receipt);T(!papacc_server_service_scq_host_current());
 n=reports;T(papacc_server_service_runtime_stopping()&&reports==n);papacc_server_service_runtime_closed();T(host.terminal_receipt==1U&&host.life.phase==PS_STOPPED&&reports==n);
 papacc_server_service_runtime_retained();T(host.terminal_receipt==1U);
 callback();papacc_server_service_runtime_retained();T(host.life.phase==PS_RETAINED&&host.terminal_receipt==2U);
 papacc_server_service_runtime_closed();T(host.life.phase==PS_RETAINED&&host.terminal_receipt==2U);
 callback();host.run_active=0U;host.result=0;T(!host.terminal_receipt&&host.life.phase==PS_START_PENDING);ps_park_retained();T(parks==1U&&host.life.phase==PS_RETAINED&&!reports);
 callback();T(ps_handler(SERVICE_CONTROL_INTERROGATE,0,NULL,NULL)==NO_ERROR&&!model_stop_flag);
 T(ps_handler(12345,0,NULL,NULL)==ERROR_CALL_NOT_IMPLEMENTED&&!model_stop_flag);
 T(ps_handler(SERVICE_CONTROL_SHUTDOWN,0,NULL,NULL)==NO_ERROR&&model_stop_flag&&!reports);
 printf("Host dispatcher/name/args/denied-start/readiness-race/Stop/quiescence/retention/startup-source MODEL PASS; native=%lu host=%lu atomic=%lu\n",(unsigned long)sizeof(native),(unsigned long)sizeof(host),(unsigned long)sizeof(cli_active));return 0;
}
