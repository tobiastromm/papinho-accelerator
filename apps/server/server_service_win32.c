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
#if defined(PAPACC_PSR_FILE_RUNTIME_LAB) && !defined(PAPACC_PSR_TRACE_LAB)
#error File diagnostic LAB requires original status recorder
#endif
#if defined(PAPACC_PSR_FILE_RUNTIME_LAB) && defined(PAPACC_PSR_TRACE_LAB)
#define PSR_FILE_ENABLED 1
#define PAPACC_PSR_FILE_CAPSULE_LAB 1
#define PAPACC_PSR_FILE_CONSUMER_ONLY 1
#include "../../tools/qualification/psr-file-capture-lab.inc"
typedef struct PS_DIAGNOSTIC {
 PSRF_FILE file;HANDLE parent;
 union {SECURITY_DESCRIPTOR aligned;BYTE bytes[1024];} sd;
 WCHAR parent_path[512],file_path[600];DWORD error;unsigned began,bound,emitted,receipt;
} PS_DIAGNOSTIC;
static void psrd_emit(const char *,unsigned);
static int psrd_quiescent(void);
#define PSRT_OUTPUT psrd_emit
#endif
typedef struct PS_RUNTIME {
#ifdef PSR_FILE_ENABLED
 PS_DIAGNOSTIC diagnostic;
#endif
 HANDLE parent,configuration,relative_image,token,access_token,unexpected,check_token;
 PSECURITY_DESCRIPTOR descriptor;PSID approved_sid;
 TOKEN_STATISTICS statistics;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 TOKEN_STATISTICS access_statistics;DWORD user_bytes,r4_error;unsigned r4_busy;
#endif
 union {TOKEN_USER align;BYTE bytes[128];} user;
 union {TOKEN_GROUPS align;BYTE bytes[4096];} groups;
 union {TOKEN_PRIVILEGES align;BYTE bytes[2048];} privileges;
 union {PRIVILEGE_SET align;BYTE bytes[1024];} access_privileges;
 BYTE bytes[128];DWORD group_bytes,privilege_bytes,elevated;
 unsigned ready,retained,managers_ready,acl_captured;unsigned char acl_hash[3][32];
} PS_RUNTIME;
#endif
typedef struct PS_NATIVE {
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 PS_ADMISSION admission;
#endif
 SC_HANDLE manager,service;DWORD thread,error;unsigned retained;
#ifdef PAPACC_SCM_RUNTIME_LAB
 PS_RUNTIME runtime;
#endif
 PAPACC_SERVER_CONSOLE_WIN32 console;
 HANDLE image;BY_HANDLE_FILE_INFORMATION image_id;unsigned image_ready;
 wchar_t image_path[1024],command[1050];unsigned char image_sha256[32];
 unsigned install_effect,install_disposition,readback,install_profile;DWORD sid_size;SID_NAME_USE sid_use;
 PAPACC_SERVICE_REGISTRATION_VIEW registration_view;unsigned registration_active;
 PS_SCQ_CARRIER start_carrier;WCHAR start_text[PS_SCQ_CARRIER_CHARS];unsigned start_selected,start_effect;HANDLE start_runtime;
 BYTE service_sid[SECURITY_MAX_SID_SIZE];wchar_t sid_domain[128];
 union { QUERY_SERVICE_CONFIGW aligned;BYTE bytes[8192]; } query;
} PS_NATIVE;
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
#include "../../tools/qualification/scm-busy-diagnostic.h"
#define PSA_DIAGNOSTIC(api,res,code,value) ig_diag_note("CANONICAL_ADMISSION",api,res,code,value,0,4)
#endif
static PS_NATIVE native;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
#include "server_service_admission_win32.inc"
#endif
static volatile LONG cli_active;
static DWORD ps_last_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
/* Local diagnostic metadata only. No query, authority or lifetime change. */
static void ps_start_stage(DWORD *stage,DWORD value){if(stage)*stage=value;}
typedef struct PS_HOST {
 SERVICE_STATUS_HANDLE handle;SERVICE_STATUS status;PS_LIFETIME life;
 volatile LONG stop;DWORD thread;unsigned run_active,terminal_receipt;int argc;char **argv;int (*run)(int,char **);int result;volatile LONG startup_first_source;unsigned closed_mode,closed_ready;
 PS_SCQ_CARRIER carrier;unsigned carrier_present; /* metadata only, never a grant */
} PS_HOST;
static PS_HOST host;
#if defined(PAPACC_SCM_RUNTIME_LAB) && defined(PAPACC_PSR_TRACE_LAB)
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
#define PT_STOP() (host.stop || native.retained || native.runtime.retained || papacc_server_console_win32_stop_requested())
#else
#define PT_STOP() (host.stop || native.runtime.retained || papacc_server_console_win32_stop_requested())
#endif
#include "server_service_observation_win32.h"
#undef PT_STOP
static DWORD psrt_observation(char *,size_t);
#define PSRT_OBSERVATION psrt_observation
#include "server_service_psr_trace_win32.inc"
/* Uses only the original owner token and bounded snapshots. No new handles. */
static DWORD psrt_observation(char *out,size_t capacity)
{
 PT_META m=pt_token(native.runtime.token,&native.runtime.statistics,
 (const BYTE *)psrt_record.token[0].data,psrt_record.token[0].length,
 (const BYTE *)psrt_record.token[1].data,psrt_record.token[1].length,native.service_sid);
 (void)_snprintf_s(out,capacity,_TRUNCATE," q_error=%lu q_known=%lu q_complete=%lu sid_count=%lu sid_attrs=%08lx filtered_raw=%lu filtered_bytes=%lu restricted_count=%lu",
 (unsigned long)m.error,(unsigned long)m.known,(unsigned long)m.complete,(unsigned long)m.matches,(unsigned long)m.attributes,(unsigned long)m.filtered_ever,(unsigned long)m.filter_bytes,(unsigned long)m.restricting_count);
 return m.error?m.error:(m.complete?ERROR_SUCCESS:ERROR_INVALID_DATA);
}

#else
#define PSRT_STORAGE_BYTES 0U
#define PSRT_VALUES(a,b) ((void)0)
#define PSRT_PREPARED() ERROR_SUCCESS
#define PSRT_TOKEN(tag,cls,buf,capacity,length,expr) (expr)
#define PSRT_CONTEXT(tag) ((void)0)
#define PSRT_BOOL(tag,request,expr) (expr)
#define PSRT_HANDLE(tag,request,expr) (expr)
#define PSRT_STATUS(tag,request,expr) (expr)
#define PSRT_COUNT(tag,expr) (expr)
#define PSRT_NT(tag,request,expr) (expr)
#define PSRT_PRED(tag,expr) (expr)
#define PSRT_RETURN(value,branch) (value)
#endif
size_t papacc_server_service_storage_bytes(void){return sizeof(native)+sizeof(host)+PSRT_STORAGE_BYTES+PAPACC_CONSOLE_BRIDGE_STORAGE_BYTES;}
/* Virtual identity needs no credential producer, but factual installation,
 * configuration, ACL/token and operational admission are still absent. This
 * cannot be supplied by argv, environment, SID string, elevation or SCM name. */
#include "server_service_startup_win32.inc"
#include "server_service_virtual_win32.inc"
#ifdef PAPACC_SCM_RUNTIME_LAB
#include "server_service_runtime_win32.inc"
#endif
int papacc_server_service_registration_view_current(const PAPACC_SERVICE_REGISTRATION_VIEW *v)
{
 return v==&native.registration_view&&v->recipient==&native&&native.thread==GetCurrentThreadId()&&
 InterlockedCompareExchange(&cli_active,0,0)==1&&native.console.is_installed&&!native.retained&&
 native.install_profile==PS_INSTALL_SCQ_CLOSED&&native.image&&native.image_ready&&
 v->image.handle==native.image&&v->image.path==native.image_path&&v->image.sha256==native.image_sha256&&
 v->command==native.command&&ps_image_same(&v->image.identity,&native.image_id);
}
static DWORD ps_close(void)
{
 DWORD e;
 /* StartService return is not bilateral close/quiescence. No release of the
  * SCM/image cohort until an original terminal path exists. */
 if(native.start_effect==1U){e=ERROR_INVALID_STATE;goto retained;}
 if(native.start_runtime){if(!CloseHandle(native.start_runtime)){e=ps_last_error();goto retained;}native.start_runtime=NULL;}
 /* A partial/uncertain registration is not made disposable by synchronous API
  * return. Keep both original cohorts; no inferred cleanup or auto-retry. */
 if(native.registration_active&&native.install_effect&&native.install_disposition!=1U&&native.install_disposition!=4U){e=ERROR_INVALID_STATE;goto retained;}
 if(native.service){if(!CloseServiceHandle(native.service)){e=ps_last_error();goto retained;}native.service=NULL;}
 if(native.manager){if(!CloseServiceHandle(native.manager)){e=ps_last_error();goto retained;}native.manager=NULL;}
 if(native.image){if(!CloseHandle(native.image)){e=ps_last_error();goto retained;}native.image=NULL;native.image_ready=0U;}
 if(native.console.is_installed&&papacc_server_console_win32_uninstall(&native.console)!=PAPACC_RESULT_OK){e=ERROR_GEN_FAILURE;goto retained;}
 if(native.registration_active){e=papacc_server_launcher_scm_registration_finish(&native.registration_view,1);if(e)goto retained;native.registration_active=0U;}
 return ERROR_SUCCESS;
retained:
 native.retained=1U;
 if(native.registration_active)(void)papacc_server_launcher_scm_registration_finish(&native.registration_view,0);
 return e;
}
/* Mutations are implemented behind factual binding; none are authorized by
 * successful read-only query. Install uses the explicit virtual account and NULL password; configuration
 * confirmation does not grant installation/CA/runtime authority. */
/* I3: original observed SCM/process terminal only, never an authority getter. */
static int ps_scq_terminal_status(const SERVICE_STATUS_PROCESS *s,DWORD exit_code,DWORD expected)
{
 return s&&(expected==ERROR_SUCCESS||expected==ERROR_NOT_FOUND)&&exit_code==expected&&
  s->dwServiceType==SERVICE_WIN32_OWN_PROCESS&&s->dwCurrentState==SERVICE_STOPPED&&!s->dwProcessId&&
  s->dwWin32ExitCode==(DWORD)(expected?ERROR_SERVICE_SPECIFIC_ERROR:ERROR_SUCCESS)&&s->dwServiceSpecificExitCode==expected;
}
static DWORD ps_mutate(const PS_REQUEST *q,SERVICE_STATUS_PROCESS *s,DWORD *stage)
{
 SERVICE_STATUS stopped;
 if(q->action==PS_START&&q->start_profile==PS_START_SCQ_CLOSED){PAPACC_SERVICE_DIAGNOSTIC_START_REPORT report;
  ps_start_stage(stage,201U);
  if(papacc_server_service_diagnostic_start_status(&report))return ERROR_ACCESS_DENIED;
 }else if(!(q->action==PS_INSTALL?ps_registration_binding_ready():ps_binding_ready()))return ERROR_ACCESS_DENIED;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 {DWORD admission=papacc_server_service_admission_enter(NULL,NULL);if(admission)return admission;}
#endif
 if(q->action==PS_INSTALL)return ps_install_virtual();
 if(q->action==PS_START){
  if(q->start_profile==PS_START_SCQ_CLOSED){
   PS_SCQ_CARRIER current;const WCHAR *args[4];WCHAR deadline[17];ULONGLONG until,tick;unsigned k;DWORD e;PAPACC_SERVICE_IMAGE_VIEW image;
   ps_start_stage(stage,202U);if(!native.start_selected||native.start_effect||s->dwCurrentState!=SERVICE_STOPPED||papacc_server_console_win32_stop_requested())return ERROR_ACCESS_DENIED;
   ps_start_stage(stage,203U);e=papacc_server_launcher_scq_start_carrier(&current);if(e||!ps_scq_carrier_equal(&current,&native.start_carrier))return ERROR_ACCESS_DENIED;
   until=((ULONGLONG)current.global_until[1]<<32)|current.global_until[0];
   ps_start_stage(stage,204U);tick=GetTickCount64();if(until<=tick||until-tick>60000U)return ERROR_TIMEOUT;
   for(k=0;k<16U;k++)deadline[k]=L"0123456789abcdef"[(until>>(4U*(15U-k)))&15U];deadline[16]=0;
   ps_start_stage(stage,205U);e=ps_virtual_readback();if(e)return e;
   ps_start_stage(stage,206U);if(memcmp(native.image_sha256,current.image,32))return ERROR_ACCESS_DENIED;
   image.handle=native.image;image.identity=native.image_id;image.path=native.image_path;image.sha256=native.image_sha256;image.owner_thread=native.thread;
   ps_start_stage(stage,207U);e=papacc_server_launcher_scq_start_take(&image);if(e)return e;
   args[0]=L"scq-to";args[1]=native.start_text;args[2]=L"g3-until";args[3]=deadline;
   /* The original act is already consumed. Revalidate freshness after image
    * checks and immediately before submission; failure never rearms it. */
   ps_start_stage(stage,208U);e=papacc_server_launcher_scq_start_resource_current();if(e)return e;
   if(papacc_server_console_win32_stop_requested())return ERROR_CANCELLED;
   ps_start_stage(stage,209U);native.start_effect=1U;
   if(!StartServiceW(native.service,4U,args))return ps_last_error();
   {SERVICE_STATUS_PROCESS fresh;DWORD n=0,exit_code,progress,expected_exit=0;
    memset(&fresh,0,sizeof(fresh));
    ps_start_stage(stage,210U);if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(LPBYTE)&fresh,sizeof(fresh),&n))return ps_last_error();
    if(n!=sizeof(fresh)||fresh.dwServiceType!=SERVICE_WIN32_OWN_PROCESS||fresh.dwCurrentState!=SERVICE_START_PENDING||!fresh.dwProcessId)return ERROR_INVALID_STATE;
    ps_start_stage(stage,208U);e=papacc_server_launcher_scq_start_resource_current();if(e)return e;
    ps_start_stage(stage,211U);native.start_runtime=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,fresh.dwProcessId);
    if(!native.start_runtime||GetProcessId(native.start_runtime)!=fresh.dwProcessId)return ERROR_ACCESS_DENIED;
    for(;;){
     if(papacc_server_console_win32_stop_requested())return ERROR_CANCELLED;
     ps_start_stage(stage,212U);progress=papacc_server_launcher_scq_start_progress(native.start_runtime);
     if(progress==ERROR_MORE_DATA)continue;
     if(progress==ERROR_IO_PENDING){progress=papacc_server_launcher_scq_start_wait(native.start_runtime);if(!progress)continue;}
     if(progress==PAPACC_SCQ_PROGRESS_CLOSED_INCONCLUSIVE){expected_exit=ERROR_NOT_FOUND;break;}
     if(progress)return progress;break;
    }
    ps_start_stage(stage,213U);e=papacc_server_launcher_scq_start_wait_terminal(native.start_runtime);if(e)return e;
    ps_start_stage(stage,214U);if(!GetExitCodeProcess(native.start_runtime,&exit_code)||exit_code!=expected_exit)return ERROR_INVALID_STATE;
    n=0;memset(&fresh,0,sizeof(fresh));
    ps_start_stage(stage,210U);if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(LPBYTE)&fresh,sizeof(fresh),&n))return ps_last_error();
    if(n!=sizeof(fresh)||!ps_scq_terminal_status(&fresh,exit_code,expected_exit))return ERROR_INVALID_STATE;
    ps_start_stage(stage,205U);e=ps_virtual_readback();if(e)return e;
    ps_start_stage(stage,215U);e=papacc_server_launcher_scq_start_finish(native.start_runtime);if(e)return e;
    native.start_effect=expected_exit?3U:2U;return 0; /* finite closed run, not productive readiness */
   }
  }
  return StartServiceW(native.service,0,NULL)?ERROR_SUCCESS:ps_last_error();
 }
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
 PS_REQUEST q;PAPACC_SERVICE_IMAGE_VIEW view;SERVICE_STATUS_PROCESS s;DWORD e=0,close_error,bytes=0,rights,start_stage=0,original_close_error=0;int parsed;
 parsed=ps_parse(argc,argv,&q);if(parsed<0)return -1;
 if(!out||!err)return 1;
#ifdef PAPACC_SCM_REGISTRATION_LAB
 if(parsed>0&&q.start_profile==PS_START_SCQ_CLOSED){fputs("SERVICE=DENY; SCQ_START_NOT_LAB; MUTATION=NOT_RUN\n",err);return 1;}
#endif
 if(!parsed){fputs("SERVICE=INVALID_COMMAND; expected install | start | stop | status | startup automatic|manual | remove\n",err);return 1;}
 if(InterlockedCompareExchange(&cli_active,1,0)!=0){fputs("SERVICE=BUSY_OR_RETAINED; REUSE=DENY\n",err);return 2;}
 if(native.retained||native.manager||native.service||native.image||native.install_effect){fputs("SERVICE=RETAINED; REUSE=DENY\n",err);return 2;}
 native.thread=GetCurrentThreadId();native.error=0;
 if(q.action==PS_INSTALL){
  native.install_profile=q.install_profile; /* command selection only, no grant */
  if(papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
  if(papacc_server_console_win32_install(&native.console)!=PAPACC_RESULT_OK){e=ERROR_INVALID_STATE;goto done;}
  e=ps_image_open(q.action==PS_START?&start_stage:NULL);if(e)goto done;
 }
 if(q.prepare_lab){
  if(!prepare){e=ERROR_NOT_SUPPORTED;goto done;}
  view.handle=native.image;view.identity=native.image_id;view.path=native.image_path;view.sha256=native.image_sha256;view.owner_thread=native.thread;
  e=prepare(q.installation,q.configuration,&view,out,err)?ERROR_ACCESS_DENIED:ERROR_SUCCESS;
  goto done;
 }
#ifndef PAPACC_SCM_REGISTRATION_LAB
 if(q.action==PS_INSTALL&&q.install_profile==PS_INSTALL_SCQ_CLOSED){
  ps_registration_view_bind();
  if(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)){e=ERROR_ACCESS_DENIED;goto done;}
  native.registration_active=1U;
 }
#endif
 if(q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED){
  char text[PS_SCQ_CARRIER_CHARS];unsigned i;PAPACC_SERVICE_DIAGNOSTIC_START_REPORT report;
  e=papacc_server_launcher_scq_start_carrier(&native.start_carrier);
  if(e){
   e=papacc_server_launcher_scq_first_controller();
   if(!e)e=papacc_server_launcher_scq_start_carrier(&native.start_carrier);
   if(e==ERROR_IO_INCOMPLETE)native.retained=1U;
  }
  if(e){
   (void)papacc_server_service_diagnostic_start_status(&report);
   fprintf(err,"SCQ_START_ADMISSION=DENY; PRE_MISSING=%lu; RESOURCE_PRE_MISSING=%lu; RESOURCE_EFFECT_OBLIGATIONS=%lu; SCM_COMPILED_BYTES=%zu; FUTURE_RUNTIME_NOT_PRECONDITION; EFFECTS=0; NO_RETRY\n",
    (unsigned long)report.missing_pre_start,(unsigned long)report.resource_pre_missing,
    (unsigned long)report.resource_effect_obligations,report.scm_compiled_storage_bytes);goto done;
  }
  if(!ps_scq_carrier_encode(&native.start_carrier,text,sizeof(text))){e=ERROR_INVALID_DATA;goto done;}
  for(i=0;i<PS_SCQ_CARRIER_CHARS;i++)native.start_text[i]=(WCHAR)(BYTE)text[i];native.start_selected=1U;
  e=papacc_server_service_diagnostic_start_status(&report);
  if(e&&report.limited_eligible&&!report.initial_receiver_source_unavailable&&
   (report.missing_pre_start&~PS_DIAGNOSTIC_START_NEEDS_RESOURCE_PLAN)==PS_DIAGNOSTIC_START_NEEDS_DELIVERY_CONTINUATION){
   e=papacc_server_launcher_scq_start_namespace_prepare();
   if(!e)e=papacc_server_service_diagnostic_start_status(&report);
  }
  if(e){
   fprintf(err,"SCQ_START_ADMISSION=DENY; INITIAL_RECEIVER_SOURCE_UNAVAILABLE=%lu; G0_G9_ELIGIBLE=%lu; RESOURCE_UNKNOWN=%lu; ORIGINAL_CHECKED=%lu; ORIGINAL_ERROR=%lu; RESOURCE_CHECKED=%lu; RESOURCE_ERROR=%lu; EFFECTS=0; NO_RETRY\n",
    (unsigned long)report.initial_receiver_source_unavailable,(unsigned long)report.limited_eligible,(unsigned long)report.resource_pre_missing,
    (unsigned long)report.original_checked,(unsigned long)report.original_error,(unsigned long)report.resource_checked,(unsigned long)report.resource_error);
   goto done;
  }
  fputs("SCQ_START_ASSURANCE=LIMITED_RISK_ELIGIBLE; OPAQUE_RESOURCE_UNKNOWN_PRESERVED; NO_PRODUCTIVE_PROMOTION\n",out);
  e=ps_image_open(q.action==PS_START?&start_stage:NULL);if(e)goto done;
 }
 if(q.action!=PS_STATUS&&!(q.action==PS_INSTALL?ps_registration_binding_ready():q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED?native.start_selected:ps_binding_ready())){
  if(q.action==PS_INSTALL){PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT r;
   (void)papacc_server_service_registration_source_status(&r);
   fprintf(err,"SERVICE=DENY; ACTION=REGISTER_SCM; REASON=%lu; MISSING=%lu; PROFILE=%lu; RESOURCE_SAMPLES=%lu; RESOURCE_ERROR=%lu; RESOURCE_MISSING=%lu; RESOURCE_PRE_MISSING=%lu; RESOURCE_EFFECT_MISSING=%lu; FUTURE_RUNTIME=NOT_REQUIRED; MUTATION=NOT_RUN; CA_AUTHORITY=DENY\n",
    (unsigned long)r.reason,(unsigned long)r.missing_source_fields,(unsigned long)r.selected_profile,
    (unsigned long)r.resource_observations,(unsigned long)r.resource_error,(unsigned long)r.resource_missing,
    (unsigned long)r.resource_pre_missing,(unsigned long)r.resource_effect_missing);
  }else fputs("SERVICE=DENY; INSTALLATION_CONFIG_ACL_TOKEN_BINDING=NOT_QUALIFIED; MUTATION=NOT_RUN; CA_AUTHORITY=DENY\n",err);
  e=ERROR_ACCESS_DENIED;goto done;
 }
 if(q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED)start_stage=301U;
 rights=SERVICE_QUERY_STATUS;
 if(q.action==PS_START)rights|=SERVICE_START|(q.start_profile==PS_START_SCQ_CLOSED?SERVICE_QUERY_CONFIG:0U);
 if(q.action==PS_STOP)rights|=SERVICE_STOP;
 if(q.action==PS_REMOVE)rights|=DELETE;
 if(q.action==PS_AUTOMATIC||q.action==PS_MANUAL)rights|=SERVICE_CHANGE_CONFIG;
 native.manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT|(q.action==PS_INSTALL?SC_MANAGER_CREATE_SERVICE:0));
 if(!native.manager){e=ps_last_error();goto done;}
 if(q.action==PS_INSTALL){e=ps_install_virtual();goto done;}
 if(q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED)start_stage=302U;
 native.service=OpenServiceW(native.manager,ps_name,rights);
 if(!native.service){e=ps_last_error();if(q.action==PS_STATUS&&e==ERROR_SERVICE_DOES_NOT_EXIST){fputs("SERVICE=ABSENT; IDENTITY=NOT_QUALIFIED\n",out);e=0;}goto done;}
 memset(&s,0,sizeof(s));
 if(q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED)start_stage=303U;
 if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(LPBYTE)&s,sizeof(s),&bytes)){e=ps_last_error();goto done;}
 if(q.action==PS_STATUS)fprintf(out,"SERVICE=OBSERVED; STATE=%lu; PID=%lu; WIN32_EXIT=%lu; IDENTITY=NOT_QUALIFIED; AUTHORITY=NONE\n",
  (unsigned long)s.dwCurrentState,(unsigned long)s.dwProcessId,(unsigned long)s.dwWin32ExitCode);
 else {
  e=ps_mutate(&q,&s,q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED?&start_stage:NULL);
  if(!e)fputs(q.action==PS_REMOVE?"SERVICE=DELETE_REQUESTED; ABSENCE=NOT_PROVEN\n":"SERVICE=REQUEST_SUBMITTED; COMPLETION=NOT_PROVEN\n",out);
 }
 done:
 if(q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED&&!native.start_effect){
  DWORD original_close=papacc_server_launcher_scq_prestart_close();
  original_close_error=original_close;
  if(original_close){native.retained=1U;if(!e)e=original_close;}
 }
 native.error=e;close_error=ps_close();
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.phase==1){DWORD admission=papacc_server_service_admission_finish(!native.retained);if(!close_error)close_error=admission;}
#endif
 if(!native.error)native.error=close_error;
 if(!native.retained&&!native.install_effect&&!native.start_effect)InterlockedExchange(&cli_active,0);
 if(e||close_error){
  if(q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED)fprintf(err,"SCQ_START_FIRST_CAUSE stage=%lu error=%lu start_submission_phase=%u prestart_close=%lu service_close=%lu retained=%u; LAST_ATTEMPT_NOT_NEW_QUERY; HISTORICAL_CAUSE_NOT_INFERRED; METADATA_NOT_AUTHORITY\n",
   (unsigned long)start_stage,(unsigned long)e,native.start_effect,(unsigned long)original_close_error,(unsigned long)close_error,native.retained);
  fprintf(err,"SERVICE=ERROR; WIN32=%lu; CLOSE=%lu; RETAINED=%u; NO_AUTOMATIC_RETRY\n",(unsigned long)e,(unsigned long)close_error,native.retained);
  if(native.retained&&q.action==PS_START&&q.start_profile==PS_START_SCQ_CLOSED){fflush(err);Sleep(INFINITE);} /* no process-exit disposal of unknown borrows */
  return 2;}
 if(native.start_effect==3U){fputs("SCQ_START=SESSION_CLOSED_INCONCLUSIVE; ISOLATION=DENY; CONSUMED=1; CONTROLLED_CLOSE=KNOWN; RUNNING=NOT_REPORTED; TERMINAL_CODE=1168; OS_QUIESCENCE=UNKNOWN; NO_PRODUCTIVE_PROMOTION; NO_RETRY\n",out);return 3;}
 if(native.start_effect==2U)fputs("SCQ_START=DIAGNOSTIC_CLOSED_QUALIFIED; CONSUMED=1; CONTROLLED_CLOSE=KNOWN; OS_QUIESCENCE=UNKNOWN; NO_PRODUCTIVE_PROMOTION; NO_RETRY\n",out);
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
/* Existing canonical root is the only evidence producer; no callback/setter. */
int papacc_server_launcher_scq_closed_evidence_current(void);
int papacc_server_launcher_scq_closed_arm_running(void);
int papacc_server_service_scq_preparation_current(void)
{
#ifdef PAPACC_SCM_REGISTRATION_LAB
 return 0;
#else
 return host.closed_mode&&papacc_server_service_host_active()&&
 (host.life.phase==PS_START_PENDING||(host.life.phase==PS_RUNNING&&host.closed_ready))&&
 !host.terminal_receipt&&!InterlockedCompareExchange(&host.stop,0,0)&&!papacc_server_console_win32_stop_requested();
#endif
}
int papacc_server_service_scq_closed_ready_current(void)
{return host.closed_ready&&papacc_server_service_scq_preparation_current()&&papacc_server_launcher_scq_closed_evidence_current();}
int papacc_server_service_scq_closed_ready(void)
{
 if(!papacc_server_service_scq_preparation_current()||host.closed_ready||!papacc_server_launcher_scq_closed_evidence_current())return 0;
 if(!papacc_server_launcher_scq_closed_arm_running())return 0;
 if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested())return 0;
 if(!ps_running(&host.life)||!ps_report(SERVICE_RUNNING,ERROR_SUCCESS)){
  papacc_server_service_runtime_retained();return 0;
 }
 host.closed_ready=1U; /* explicitly closed; runtime_ready() remains DENY */
 if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested()){
  (void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;
 }
 return papacc_server_service_scq_closed_ready_current();
}
int papacc_server_service_scq_host_current(void)
{
#ifdef PAPACC_SCM_REGISTRATION_LAB
 return 0; /* SCQ never inherits a LAB hosting identity or readiness. */
#else
 return !host.closed_mode&&papacc_server_service_host_active()&&host.life.phase==PS_RUNNING&&
 !host.terminal_receipt&&!InterlockedCompareExchange(&host.stop,0,0)&&
 !papacc_server_console_win32_stop_requested();
#endif
}
#ifndef PAPACC_SCM_RUNTIME_LAB
int papacc_server_service_qualification_managers_ready(const struct PAPACC_SERVER_IO_LOOP_WIN32 *l){(void)l;return 0;}
#endif
int papacc_server_service_qualification_active(void)
{
#ifdef PAPACC_SCM_RUNTIME_LAB
 return papacc_server_service_host_active()&&native.runtime.ready&&!native.runtime.retained
#ifdef PSR_FILE_ENABLED
 &&native.runtime.diagnostic.receipt==2U&&!native.runtime.diagnostic.error&&psrd_quiescent()
#endif
 ;
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
 if(host.closed_mode)return 0; /* closed evidence never becomes productive readiness */
 if(!host.handle)return 1; /* direct console runtime, no SCM requirement */
 if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt||host.life.phase!=PS_START_PENDING)return 0;
#ifdef PAPACC_SCM_RUNTIME_LAB
#ifdef PSR_FILE_ENABLED
 if(native.runtime.diagnostic.receipt!=2U||native.runtime.diagnostic.error||!psrd_quiescent())return 0;
#endif
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
#ifdef PSR_FILE_ENABLED
 if(!psrd_quiescent()){papacc_server_service_runtime_retained();return;}
#endif
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
 if(host.closed_mode){
  if(!argv||!argv[0]||_wcsicmp(argv[0],ps_name)||(argc!=1U&&argc!=3U&&argc!=5U)){host.result=2;return;}
  if(argc==3U||argc==5U){char text[PS_SCQ_CARRIER_CHARS];unsigned i;
   if(!argv[1]||wcscmp(argv[1],L"scq-to")||!argv[2]){host.result=2;return;}
   for(i=0;i<PS_SCQ_CARRIER_CHARS;i++){if(argv[2][i]>127){host.result=2;return;}text[i]=(char)argv[2][i];if(!text[i])break;}
   if(i!=PS_SCQ_CARRIER_CHARS-1U||!ps_scq_carrier_decode(text,i,&host.carrier)){host.result=2;return;}
   host.carrier_present=1U; /* Still START_PENDING without original authority. */
   if(argc==5U){ULONGLONG deadline=0,now=GetTickCount64(),window;unsigned j;WCHAR ch;
    if(!argv[3]||wcscmp(argv[3],L"g3-until")||!argv[4]||wcsnlen(argv[4],17U)!=16U){host.result=2;return;}
    for(j=0;j<16U;j++){ch=argv[4][j];if(!((ch>=L'0'&&ch<=L'9')||(ch>=L'a'&&ch<=L'f'))){host.result=2;return;}deadline=(deadline<<4)|(ch<=L'9'?ch-L'0':ch-L'a'+10);}
    window=((ULONGLONG)host.carrier.until[1]<<32)|host.carrier.until[0];
    if(deadline<=now||deadline-now>60000U||deadline>window){host.result=2;return;}
    host.carrier.global_until[0]=(unsigned)deadline;host.carrier.global_until[1]=(unsigned)(deadline>>32);
   }
  }
 }
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
 if(!host.closed_mode&&!ps_binding_ready()){
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
int papacc_server_service_scq_carrier(PS_SCQ_CARRIER *out)
{
 if(!out||!host.carrier_present||!papacc_server_service_scq_preparation_current())return 0;
 *out=host.carrier;return 1; /* identity/generation/lifetime must be revalidated */
}
int papacc_server_service_dispatch(int argc,char **argv,int (*run)(int,char **))
{
 SERVICE_TABLE_ENTRYW table[2];
 if(argc<3||!argv||!argv[1]||!argv[2]||strcmp(argv[1],"service")||strcmp(argv[2],"host"))return -1;
 if((argc!=3&&!(argc==4&&argv[3]&&!strcmp(argv[3],"scq-closed")))||!run||host.handle||host.life.phase)return 1;
#ifdef PAPACC_SCM_REGISTRATION_LAB
 if(argc!=3)return 1; /* no closed-profile inheritance from LAB */
#endif
 host.closed_mode=argc==4?1U:0U; /* selection only, no admission */
 host.argc=1;host.argv=argv;host.run=run;host.result=2;
 table[0].lpServiceName=(LPWSTR)ps_name;table[0].lpServiceProc=ps_main;
 table[1].lpServiceName=NULL;table[1].lpServiceProc=NULL;
 if(!StartServiceCtrlDispatcherW(table))return 2; /* no console fallback */
 return host.result;
}
