#include <windows.h>
#include <string.h>
#include <wchar.h>
#include "../../tools/qualification/storage-hash-capsule/storage_sha256.h"
#include "server_service_data.h"
#include "server_service_win32.h"
#include "server_console_win32.h"
static const wchar_t ps_name[]=L"PapinhoAccelerator";
/* One private CLI invocation, never overlaps runtime via canonical main. */
typedef struct PS_NATIVE {
 SC_HANDLE manager,service;DWORD thread,error;unsigned retained;
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
 volatile LONG stop;int argc;char **argv;int (*run)(int,char **);int result;
} PS_HOST;
static PS_HOST host;
size_t papacc_server_service_storage_bytes(void){return sizeof(native)+sizeof(host);}
/* Virtual identity needs no credential producer, but factual installation,
 * configuration, ACL/token and operational admission are still absent. This
 * cannot be supplied by argv, environment, SID string, elevation or SCM name. */
static int ps_binding_ready(void){return 0;}
#include "server_service_virtual_win32.inc"
static DWORD ps_close(void)
{
 DWORD e;
 if(native.service){if(!CloseServiceHandle(native.service)){e=ps_last_error();native.retained=1U;return e;}native.service=NULL;}
 if(native.manager){if(!CloseServiceHandle(native.manager)){e=ps_last_error();native.retained=1U;return e;}native.manager=NULL;}
 if(native.image){if(!CloseHandle(native.image)){e=ps_last_error();native.retained=1U;return e;}native.image=NULL;native.image_ready=0U;}
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
int papacc_server_service_cli(int argc,const char *const *argv,FILE *out,FILE *err)
{
 PS_REQUEST q;SERVICE_STATUS_PROCESS s;DWORD e=0,close_error,bytes=0,rights;int parsed;
 parsed=ps_parse(argc,argv,&q);if(parsed<0)return -1;
 if(!out||!err)return 1;
 if(!parsed){fputs("SERVICE=INVALID_COMMAND; expected install | start | stop | status | startup automatic|manual | remove\n",err);return 1;}
 if(InterlockedCompareExchange(&cli_active,1,0)!=0){fputs("SERVICE=BUSY_OR_RETAINED; REUSE=DENY\n",err);return 2;}
 if(native.retained||native.manager||native.service||native.image||native.install_effect){fputs("SERVICE=RETAINED; REUSE=DENY\n",err);return 2;}
 native.thread=GetCurrentThreadId();native.error=0;
 if(q.action==PS_INSTALL){e=ps_image_open();if(e)goto done;}
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
 /* Startup is denied before runtime. No positive readiness/Stop controls
  * advertised without the missing native owner/config/admission binding. */
 return SetServiceStatus(host.handle,&host.status)!=0;
}
static void ps_park_retained(void)
{
 /* Query-only retained process containment, NOT native retry or a timeout
  * release. No STOPPED report: SCM could kill protected dependencies. */
 host.life.phase=PS_RETAINED;host.result=2;Sleep(INFINITE);
}
static void WINAPI ps_main(DWORD argc,wchar_t **argv)
{
 (void)argc;(void)argv;
 if(!ps_begin(&host.life)){host.result=2;return;}
 host.handle=RegisterServiceCtrlHandlerExW(ps_name,ps_handler,NULL);
 if(!host.handle){host.result=2;return;}
 if(!ps_report(SERVICE_START_PENDING,ERROR_SUCCESS)){ps_park_retained();return;}
 if(!ps_binding_ready()){
  /* No runtime, token, root, key or worker was acquired: factual empty set.
   * Complete before STOPPED; perform NO work after that report. */
  host.result=2;(void)ps_finish(&host.life,1,0);
  if(!ps_report(SERVICE_STOPPED,ERROR_ACCESS_DENIED))ps_park_retained();
  return;
 }
 /* A future factual producer must supply readiness AND final quiescence.
  * Sharing this callback is not permission to run today. Never duplicate the
  * server runtime or convert its integer exit code to resource destruction. */
 host.result=host.run(host.argc,host.argv);
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
