#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <string.h>
#include <wchar.h>
#include "../../tools/qualification/storage-hash-capsule/storage_sha256.h"
#include "server_service_data.h"
#include "server_service_win32.h"
#include "server_console_win32.h"
#include "server_service_identity_win32.h"
static const wchar_t ps_name[]=PAPACC_SCM_NAME_LITERAL;
/* One private CLI invocation, never overlaps runtime via canonical main. */
#ifdef PAPACC_SCM_RUNTIME_LAB
#ifndef PAPACC_SCM_REGISTRATION_LAB
#error Runtime LAB requires the fixed LAB identity
#endif
typedef struct PS_RUNTIME {
 HANDLE parent,configuration,relative_image,token,access_token,unexpected,check_token;
 PSECURITY_DESCRIPTOR descriptor;PSID approved_sid;
 TOKEN_STATISTICS statistics;
 union {TOKEN_USER align;BYTE bytes[128];} user;
 union {TOKEN_GROUPS align;BYTE bytes[4096];} groups;
 union {TOKEN_PRIVILEGES align;BYTE bytes[2048];} privileges;
 union {PRIVILEGE_SET align;BYTE bytes[1024];} access_privileges;
 BYTE bytes[128];DWORD group_bytes,privilege_bytes,elevated;
 unsigned ready,retained,managers_ready,acl_captured;unsigned char acl_hash[3][32];
} PS_RUNTIME;
#endif
typedef struct PS_NATIVE {
 SC_HANDLE manager,service;DWORD thread,error;unsigned retained;
#ifdef PAPACC_SCM_RUNTIME_LAB
 PS_RUNTIME runtime;
#endif
 PAPACC_SERVER_CONSOLE_WIN32 console;
 HANDLE image;BY_HANDLE_FILE_INFORMATION image_id;unsigned image_ready;
 wchar_t image_path[1024],command[1050];unsigned char image_sha256[32];
 unsigned install_effect,install_disposition,readback;DWORD sid_size;SID_NAME_USE sid_use;
 BYTE service_sid[SECURITY_MAX_SID_SIZE];wchar_t sid_domain[128];
 union { QUERY_SERVICE_CONFIGW aligned;BYTE bytes[8192]; } query;
} PS_NATIVE;
static PS_NATIVE native;
static volatile LONG cli_active;
static DWORD ps_last_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
typedef struct PS_HOST {
 SERVICE_STATUS_HANDLE handle;SERVICE_STATUS status;PS_LIFETIME life;
 volatile LONG stop;DWORD thread;unsigned run_active,terminal_receipt;int argc;char **argv;int (*run)(int,char **);int result;
} PS_HOST;
static PS_HOST host;
size_t papacc_server_service_storage_bytes(void){return sizeof(native)+sizeof(host);}
/* Virtual identity needs no credential producer, but factual installation,
 * configuration, ACL/token and operational admission are still absent. This
 * cannot be supplied by argv, environment, SID string, elevation or SCM name. */
static int ps_binding_ready(void){return 0;}
#include "server_service_virtual_win32.inc"
#ifdef PAPACC_SCM_RUNTIME_LAB
#include "server_service_runtime_win32.inc"
#endif
static DWORD ps_close(void)
{
 DWORD e;
 if(native.service){if(!CloseServiceHandle(native.service)){e=ps_last_error();native.retained=1U;return e;}native.service=NULL;}
 if(native.manager){if(!CloseServiceHandle(native.manager)){e=ps_last_error();native.retained=1U;return e;}native.manager=NULL;}
 if(native.image){if(!CloseHandle(native.image)){e=ps_last_error();native.retained=1U;return e;}native.image=NULL;native.image_ready=0U;}
 if(native.console.is_installed&&papacc_server_console_win32_uninstall(&native.console)!=PAPACC_RESULT_OK){native.retained=1U;return ERROR_GEN_FAILURE;}
 return ERROR_SUCCESS;
}
/* Mutations are implemented behind factual binding; none are authorized by
 * successful read-only query. Install uses the explicit virtual account and NULL password; configuration
 * confirmation does not grant installation/CA/runtime authority. */
static DWORD ps_mutate(const PS_REQUEST *q,SERVICE_STATUS_PROCESS *s)
{
 SERVICE_STATUS stopped;
 if(!ps_binding_ready())return ERROR_ACCESS_DENIED;
 if(q->action==PS_INSTALL)return ps_install_virtual();
 if(q->action==PS_START)return StartServiceW(native.service,0,NULL)?ERROR_SUCCESS:ps_last_error();
 if(q->action==PS_STOP)return ControlService(native.service,SERVICE_CONTROL_STOP,&stopped)?ERROR_SUCCESS:ps_last_error();
 if(q->action==PS_AUTOMATIC||q->action==PS_MANUAL)
  return ChangeServiceConfigW(native.service,SERVICE_NO_CHANGE,q->action==PS_AUTOMATIC?SERVICE_AUTO_START:SERVICE_DEMAND_START,
   SERVICE_NO_CHANGE,NULL,NULL,NULL,NULL,NULL,NULL,NULL)?ERROR_SUCCESS:ps_last_error();
 if(q->action==PS_REMOVE){
  if(s->dwCurrentState!=SERVICE_STOPPED)return ERROR_SERVICE_CANNOT_ACCEPT_CTRL;
  return DeleteService(native.service)?ERROR_SUCCESS:ps_last_error();
 }
 return ERROR_INVALID_PARAMETER;
}
int papacc_server_service_cli(int argc,const char *const *argv,FILE *out,FILE *err,PAPACC_SERVICE_PREPARE prepare)
{
 PS_REQUEST q;PAPACC_SERVICE_IMAGE_VIEW view;SERVICE_STATUS_PROCESS s;DWORD e=0,close_error,bytes=0,rights;int parsed;
 parsed=ps_parse(argc,argv,&q);if(parsed<0)return -1;
 if(!out||!err)return 1;
 if(!parsed){fputs("SERVICE=INVALID_COMMAND; expected install | start | stop | status | startup automatic|manual | remove\n",err);return 1;}
 if(InterlockedCompareExchange(&cli_active,1,0)!=0){fputs("SERVICE=BUSY_OR_RETAINED; REUSE=DENY\n",err);return 2;}
 if(native.retained||native.manager||native.service||native.image||native.install_effect){fputs("SERVICE=RETAINED; REUSE=DENY\n",err);return 2;}
 native.thread=GetCurrentThreadId();native.error=0;
 if(q.action==PS_INSTALL){
  if(papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
  if(papacc_server_console_win32_install(&native.console)!=PAPACC_RESULT_OK){e=ERROR_INVALID_STATE;goto done;}
  e=ps_image_open();if(e)goto done;
 }
 if(q.prepare_lab){
  if(!prepare){e=ERROR_NOT_SUPPORTED;goto done;}
  view.handle=native.image;view.identity=native.image_id;view.path=native.image_path;view.sha256=native.image_sha256;view.owner_thread=native.thread;
  e=prepare(q.installation,q.configuration,&view,out,err)?ERROR_ACCESS_DENIED:ERROR_SUCCESS;
  goto done;
 }
 if(q.action!=PS_STATUS&&!ps_binding_ready()){
  fputs("SERVICE=DENY; INSTALLATION_CONFIG_ACL_TOKEN_BINDING=NOT_QUALIFIED; MUTATION=NOT_RUN; CA_AUTHORITY=DENY\n",err);e=ERROR_ACCESS_DENIED;goto done;
 }
 rights=SERVICE_QUERY_STATUS;
 if(q.action==PS_START)rights|=SERVICE_START;
 if(q.action==PS_STOP)rights|=SERVICE_STOP;
 if(q.action==PS_REMOVE)rights|=DELETE;
 if(q.action==PS_AUTOMATIC||q.action==PS_MANUAL)rights|=SERVICE_CHANGE_CONFIG;
 native.manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT|(q.action==PS_INSTALL?SC_MANAGER_CREATE_SERVICE:0));
 if(!native.manager){e=ps_last_error();goto done;}
 if(q.action==PS_INSTALL){e=ps_install_virtual();goto done;}
 native.service=OpenServiceW(native.manager,ps_name,rights);
 if(!native.service){e=ps_last_error();if(q.action==PS_STATUS&&e==ERROR_SERVICE_DOES_NOT_EXIST){fputs("SERVICE=ABSENT; IDENTITY=NOT_QUALIFIED\n",out);e=0;}goto done;}
 memset(&s,0,sizeof(s));
 if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(LPBYTE)&s,sizeof(s),&bytes)){e=ps_last_error();goto done;}
 if(q.action==PS_STATUS)fprintf(out,"SERVICE=OBSERVED; STATE=%lu; PID=%lu; WIN32_EXIT=%lu; IDENTITY=NOT_QUALIFIED; AUTHORITY=NONE\n",
  (unsigned long)s.dwCurrentState,(unsigned long)s.dwProcessId,(unsigned long)s.dwWin32ExitCode);
 else {
  e=ps_mutate(&q,&s);
  if(!e)fputs(q.action==PS_REMOVE?"SERVICE=DELETE_REQUESTED; ABSENCE=NOT_PROVEN\n":"SERVICE=REQUEST_SUBMITTED; COMPLETION=NOT_PROVEN\n",out);
 }
 done:
 native.error=e;close_error=ps_close();if(!native.error)native.error=close_error;
 if(!native.retained&&!native.install_effect)InterlockedExchange(&cli_active,0);
 if(e||close_error){fprintf(err,"SERVICE=ERROR; WIN32=%lu; CLOSE=%lu; RETAINED=%u; NO_AUTOMATIC_RETRY\n",(unsigned long)e,(unsigned long)close_error,native.retained);return 2;}
 return 0;
}
/* Only ServiceMain owns life/status. Handler performs one atomic signal and
 * reuses the existing application Stop bridge; no cleanup or blocking work. */
static DWORD WINAPI ps_handler(DWORD control,DWORD kind,void *data,void *context)
{
 (void)kind;(void)data;(void)context;
 if(control==SERVICE_CONTROL_INTERROGATE)return NO_ERROR;
 if(control==SERVICE_CONTROL_STOP||control==SERVICE_CONTROL_SHUTDOWN){
  InterlockedExchange(&host.stop,1);papacc_server_console_win32_request_stop();return NO_ERROR;
 }
 return ERROR_CALL_NOT_IMPLEMENTED;
}
static int ps_report(DWORD state,DWORD error)
{
 memset(&host.status,0,sizeof(host.status));host.status.dwServiceType=SERVICE_WIN32_OWN_PROCESS;
 host.status.dwCurrentState=state;host.status.dwWin32ExitCode=error;
 if(error==ERROR_SERVICE_SPECIFIC_ERROR)host.status.dwServiceSpecificExitCode=(DWORD)host.result;
 if(state==SERVICE_RUNNING)host.status.dwControlsAccepted=SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;
 /* Startup is denied before runtime. No positive readiness/Stop controls
  * advertised without the missing native owner/config/admission binding. */
 return SetServiceStatus(host.handle,&host.status)!=0;
}
int papacc_server_service_host_active(void)
{return host.handle&&host.run_active&&host.thread==GetCurrentThreadId();}
#ifndef PAPACC_SCM_RUNTIME_LAB
int papacc_server_service_qualification_managers_ready(const struct PAPACC_SERVER_IO_LOOP_WIN32 *l){(void)l;return 0;}
#endif
int papacc_server_service_qualification_active(void)
{
#ifdef PAPACC_SCM_RUNTIME_LAB
 return papacc_server_service_host_active()&&native.runtime.ready&&!native.runtime.retained;
#else
 return 0;
#endif
}
const unsigned char *papacc_server_service_qualification_source(size_t *bytes)
{
 if(bytes)*bytes=0;
#ifdef PAPACC_SCM_RUNTIME_LAB
 if(bytes&&papacc_server_service_qualification_active()&&psr_current()==ERROR_SUCCESS){*bytes=116U;return native.runtime.bytes;}
#endif
 return NULL;
}
int papacc_server_service_runtime_ready(void)
{
 if(!host.handle)return 1; /* direct console runtime, no SCM requirement */
 if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt||host.life.phase!=PS_START_PENDING)return 0;
#ifdef PAPACC_SCM_RUNTIME_LAB
 if(!native.runtime.ready||!native.runtime.managers_ready||psr_current()!=ERROR_SUCCESS)return 0;
#endif
 if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested()){
  (void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;
 }
 if(!ps_running(&host.life)||!ps_report(SERVICE_RUNNING,ERROR_SUCCESS))return 0;
 if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested()){(void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;}
 return 1;
}
int papacc_server_service_runtime_stopping(void)
{
 if(!host.handle)return 1;
 if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return 0;
 if(host.life.phase==PS_STOP_PENDING)return 1;
 if(!ps_stop(&host.life))return 0;
 return ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);
}
void papacc_server_service_runtime_closed(void)
{
 if(!host.handle||!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return;
 if(ps_finish(&host.life,1,0))host.terminal_receipt=1U;
}
void papacc_server_service_runtime_retained(void)
{
 if(!host.handle||!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return;
 (void)ps_finish(&host.life,0,1);host.terminal_receipt=2U;
}
static void ps_park_retained(void)
{
 /* Query-only retained process containment, NOT native retry or a timeout
  * release. No STOPPED report: SCM could kill protected dependencies. */
 host.life.phase=PS_RETAINED;host.result=2;Sleep(INFINITE);
}
static void WINAPI ps_main(DWORD argc,wchar_t **argv)
{
#ifdef PAPACC_SCM_REGISTRATION_LAB
 /* Table name alone is not identity for an own-process service. Fail closed
  * for missing/mismatched LAB name or extra service-start arguments. This
  * predicate never grants factual binding, token isolation or admission. */
 if(argc!=1U||!argv||!argv[0]||_wcsicmp(argv[0],ps_name)){host.result=2;return;}
#else
 (void)argc;(void)argv;
#endif
 host.thread=GetCurrentThreadId();
 if(!ps_begin(&host.life)){host.result=2;return;}
 host.handle=RegisterServiceCtrlHandlerExW(ps_name,ps_handler,NULL);
 if(!host.handle){host.result=2;return;}
 if(!ps_report(SERVICE_START_PENDING,ERROR_SUCCESS)){ps_park_retained();return;}
#ifdef PAPACC_SCM_RUNTIME_LAB
 native.error=psr_prepare();
 if(native.error!=ERROR_SUCCESS){
  host.result=2;
  if(psr_close()!=ERROR_SUCCESS||ps_close()!=ERROR_SUCCESS){ps_park_retained();return;}
  (void)ps_finish(&host.life,1,0);
  if(!ps_report(SERVICE_STOPPED,native.error))ps_park_retained();
  return;
 }
#else
 if(!ps_binding_ready()){
  /* No runtime, token, root, key or worker was acquired: factual empty set.
   * Complete before STOPPED; perform NO work after that report. */
  host.result=2;(void)ps_finish(&host.life,1,0);
  if(!ps_report(SERVICE_STOPPED,ERROR_ACCESS_DENIED))ps_park_retained();
  return;
 }
#endif
 /* The same runtime must supply readiness AND final quiescence.
  * Sharing this callback is not permission to run today. Never duplicate the
  * server runtime or convert its integer exit code to resource destruction. */
 host.run_active=1U;host.result=host.run(host.argc,host.argv);host.run_active=0U;
 if(host.terminal_receipt==1U&&host.life.phase==PS_STOPPED){
#ifdef PAPACC_SCM_RUNTIME_LAB
  if(psr_close()!=ERROR_SUCCESS||ps_close()!=ERROR_SUCCESS){ps_park_retained();return;}
#endif
  if(!ps_report(SERVICE_STOPPED,host.result?ERROR_SERVICE_SPECIFIC_ERROR:ERROR_SUCCESS))ps_park_retained();
  return;
 }
 ps_park_retained();
}
int papacc_server_service_dispatch(int argc,char **argv,int (*run)(int,char **))
{
 SERVICE_TABLE_ENTRYW table[2];
 if(argc<3||!argv||!argv[1]||!argv[2]||strcmp(argv[1],"service")||strcmp(argv[2],"host"))return -1;
 if(argc!=3||!run||host.handle||host.life.phase)return 1;
 host.argc=1;host.argv=argv;host.run=run;host.result=2;
 table[0].lpServiceName=(LPWSTR)ps_name;table[0].lpServiceProc=ps_main;
 table[1].lpServiceName=NULL;table[1].lpServiceProc=NULL;
 if(!StartServiceCtrlDispatcherW(table))return 2; /* no console fallback */
 return host.result;
}
