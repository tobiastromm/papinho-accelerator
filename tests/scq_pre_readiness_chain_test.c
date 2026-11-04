/* Full pre-readiness composition. Every token/process/SCM/IO/RNG/KSP boundary
 * is modeled. Real SID/SD helpers only. No endpoint or operational API effect. */
#define PAPACC_SCQ_RECIPIENT_ENTRY preserved_recipient_main
#include "scq_recipient_close_test.c"
#include "server_console_win32.h"
#include "server_service_identity_win32.h"
#include "server_service_win32.h"
static SESSION_FIXTURE pf[57];static PAPACC_SCQ_RECIPIENT_ORIGIN pr[57];static PAPACC_PERSONAL_SCQ_INPUTS pi[57];
static SESSION_FIXTURE *active_f;static PAPACC_SCQ_RECIPIENT_ORIGIN *active_r;static PAPACC_PERSONAL_SCQ_INPUTS *active_i;
static unsigned scenario,cs_reports,cs_runs,cs_parks,cs_running,cs_ready,cs_stop,cs_callback_checked,cs_armed,cs_premature;
static BOOL WINAPI cs_dispatch(const SERVICE_TABLE_ENTRYW *table)
{WCHAR *v[1];if(!table||!table[0].lpServiceProc)return FALSE;v[0]=(WCHAR *)PAPACC_SCM_NAME_LITERAL;table[0].lpServiceProc(1U,v);return TRUE;}
static SERVICE_STATUS_HANDLE WINAPI cs_register(LPCWSTR name,LPHANDLER_FUNCTION_EX fn,LPVOID context)
{(void)context;if(!fn||wcscmp(name,PAPACC_SCM_NAME_LITERAL))return NULL;return (SERVICE_STATUS_HANDLE)(ULONG_PTR)9;}
static BOOL WINAPI cs_report(SERVICE_STATUS_HANDLE h,LPSERVICE_STATUS s)
{if(h!=(SERVICE_STATUS_HANDLE)(ULONG_PTR)9)return FALSE;cs_reports++;if(s->dwCurrentState==SERVICE_RUNNING){cs_running++;if(!cs_armed)cs_premature++;if(scenario==14U)return FALSE;runtime_service_state=SERVICE_RUNNING;if(scenario==15U)cs_stop=1U;}return TRUE;}
static void WINAPI cs_sleep(DWORD ms){if(ms==INFINITE)cs_parks++;}
static PAPACC_BOOL cs_stopped(void){return cs_stop?PAPACC_TRUE:PAPACC_FALSE;}
static void cs_request_stop(void){cs_stop=1U;}
#define StartServiceCtrlDispatcherW cs_dispatch
#define RegisterServiceCtrlHandlerExW cs_register
#define SetServiceStatus cs_report
#define Sleep cs_sleep
#define papacc_server_console_win32_stop_requested cs_stopped
#define papacc_server_console_win32_request_stop cs_request_stop
/* This test owns only SCQ startup. Registration remains unavailable; do not
 * pull a second canonical launcher/configuration owner into the model. */
DWORD papacc_server_launcher_scm_registration_target(WCHAR *out,size_t cap){(void)out;(void)cap;return ERROR_NOT_SUPPORTED;}
int papacc_server_launcher_scm_registration_present(const PAPACC_SERVICE_REGISTRATION_VIEW *v,FILE *out,FILE *err){(void)v;(void)out;(void)err;return 2;}
DWORD papacc_server_launcher_scm_registration_current(const PAPACC_SERVICE_REGISTRATION_VIEW *v){(void)v;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_take(const PAPACC_SERVICE_REGISTRATION_VIEW *v){(void)v;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_effect(const PAPACC_SERVICE_REGISTRATION_VIEW *v,unsigned effect,unsigned returned,DWORD result)
{(void)v;(void)effect;(void)returned;(void)result;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_finish(const PAPACC_SERVICE_REGISTRATION_VIEW *v,int closed){(void)v;(void)closed;return ERROR_ACCESS_DENIED;}
DWORD papacc_server_launcher_scm_registration_resource_report(const PAPACC_SERVICE_REGISTRATION_VIEW *v,PAPACC_SCM_RESOURCE_REPORT *out){(void)v;(void)out;return ERROR_ACCESS_DENIED;}
#include "server_service_win32.h"
/* Compile-time negative boundary, never an installable product callback. */
static DWORD model_no_start_carrier(PS_SCQ_CARRIER *out){
#ifdef PAPACC_SCQ_REPORT_ORIGINAL_MODEL
 return query_report_carrier(out);
#else
 (void)out;return ERROR_ACCESS_DENIED;
#endif
}
static DWORD model_no_start_take(const PAPACC_SERVICE_IMAGE_VIEW *view){(void)view;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_progress(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_wait(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_terminal(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_finish(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_namespace(void){
#ifdef PAPACC_SCQ_REPORT_ORIGINAL_MODEL
 return query_report_namespace();
#else
 return ERROR_ACCESS_DENIED;
#endif
}
static DWORD model_no_first_controller(void){return ERROR_ACCESS_DENIED;}
static DWORD model_prestart_close(void){return 0;}
static DWORD model_no_start_resource(void){
#ifdef PAPACC_SCQ_REPORT_ORIGINAL_MODEL
 return query_report_resource();
#else
 return ERROR_ACCESS_DENIED;
#endif
}
#define papacc_server_launcher_scq_start_resource_current model_no_start_resource
static DWORD model_no_namespace_prepare(void){return ERROR_ACCESS_DENIED;}
#define papacc_server_launcher_scq_start_namespace_prepare model_no_namespace_prepare
#define papacc_server_launcher_scq_first_controller model_no_first_controller
#define papacc_server_launcher_scq_prestart_close model_prestart_close
#define papacc_server_launcher_scq_start_carrier model_no_start_carrier
#define papacc_server_launcher_scq_start_take model_no_start_take
#define papacc_server_launcher_scq_start_progress model_no_start_progress
#define papacc_server_launcher_scq_start_wait model_no_start_wait
#define papacc_server_launcher_scq_start_wait_terminal model_no_start_terminal
#define papacc_server_launcher_scq_start_finish model_no_start_finish
#define papacc_server_launcher_scq_start_namespace_current model_no_start_namespace
#include "../apps/server/server_service_win32.c"
int papacc_server_launcher_scq_closed_evidence_current(void)
{
 if(!active_f||!active_r||!active_i||!papacc_server_service_scq_preparation_current())return 0;
 return active_f->runtime_profile.pre_readiness&&active_f->runtime.closed_preparation&&
 !scqr_closed_ready_current(active_r,&active_f->recipient,&active_f->runtime_owner,active_i,&active_f->stop);
}
int papacc_server_launcher_scq_closed_arm_running(void)
{
 if(!papacc_server_launcher_scq_closed_evidence_current())return 0;
 if(scqr_arm_closed_running(active_r,&active_f->recipient,&active_f->runtime_owner,active_i,&active_f->stop))return 0;
 cs_armed=1U;return 1;
}
static int cs_run(int argc,char **argv)
{
 SESSION_FIXTURE *f=active_f;PAPACC_SCQ_RECIPIENT_ORIGIN *r=active_r;PAPACC_PERSONAL_SCQ_INPUTS *in=active_i;
 DWORD e;unsigned j;HANDLE moved[3];(void)argc;(void)argv;cs_runs++;
 C(papacc_server_service_scq_preparation_current()&&!papacc_server_service_scq_host_current());
 C(!papacc_server_service_runtime_ready()&&!papacc_server_service_scq_closed_ready()&&!cs_running);
 C(host.life.phase==PS_START_PENDING&&!host.closed_ready);
 if(scenario==1U){C(!f->source.value.controller_consumed);papacc_server_service_runtime_closed();cs_callback_checked=1U;return 2;}
 C(f->source.value.controller_consumed&&!f->runtime.phase&&!f->source.value.runtime_pid);
 if(f->plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)C(f->source.start_consumed&&f->model_effects==1U);
 runtime_service_state=scenario==11U?SERVICE_RUNNING:SERVICE_START_PENDING;
 if(scenario==2U)fault=13;
 if(scenario==2U||scenario==11U){
  e=papacc_personal_runtime_capture_closed(&f->runtime,(HANDLE)1,virtual_sid,1U);
  C(e!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;
 }
 if(f->plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY){
  WCHAR empty[112]={0};
  e=(DWORD)observed_runtime_identity_mode(f,1U);
  C(!e&&!f->runtime_profile.phase&&!f->recipient.phase&&!f->runtime_dispatch.phase&&!r->phase);
  C(!papacc_personal_scq_prepare_transport(&f->controller_dispatch,&f->runtime,f->plan.attempt,empty));
 }else e=(DWORD)observed_runtime_mode(f,1U);
 C(!e&&f->runtime.closed_preparation&&f->plan.runtime.service_state==SERVICE_START_PENDING);
 {PAPACC_PERSONAL_CNG_PROBE probe;memset(&probe,0,sizeof(probe));
  C(papacc_personal_cng_probe(&probe,&f->runtime,fixture_name,&f->stop)==PAPACC_CNG_PROBE_LOCAL_DENY);
  C(probe.classification==PAPACC_CNG_PROBE_LOCAL_DENY&&!probe.provider_calls&&!probe.open_calls&&!qproviders&&!qopens);}
 {BYTE ordinary[PAPACC_SCQ_RECORD_BYTES];PAPACC_SCQ_PLAN_DATA decoded;
  C(papacc_scq_plan_encode(&f->plan,ordinary,sizeof(ordinary))!=0);
  C(!scqp_encode_fields(&f->plan,ordinary,sizeof(ordinary)));
  C(papacc_scq_plan_decode(ordinary,sizeof(ordinary),&decoded)!=0);}

 if(scenario==12U){PAPACC_SCQ_PROFILE_DATA normal;memset(&normal,0,sizeof(normal));
  C(papacc_scq_profile_prepare_data(&normal,&f->runtime_owner,2U,&f->plan,&f->plan.runtime)!=0);}
 C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
 C(f->controller_profile.pre_readiness&&!f->controller_profile.waiting_runtime&&!cs_running);
 in->phase=1;in->content_verified=1;in->reader[0]=(HANDLE)41;in->reader[1]=(HANDLE)42;in->reader[2]=(HANDLE)43;
 in->identity[0]=f->original.request_id;in->identity[1]=f->original.decision_id;in->identity[2]=f->original.image_id;
 for(j=0;j<3U;j++){memcpy(in->digest[j],f->plan.refs[j].digest,32);memcpy(in->descriptor_digest[j],f->plan.refs[j].descriptor,32);}
 if(scenario==3U)f->policy.runtime[8]^=1;if(scenario==4U)origin_fault=1;
 if(f->plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)
  e=scqr_first_prepare(r,&f->recipient,&f->runtime_owner,&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,&f->runtime_receiver,&f->plan,(HANDLE)44,&f->stop);
 else e=scqr_acquire(r,&f->runtime_owner,&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,(HANDLE)44,&f->stop);
 if(scenario==3U||scenario==4U){C(e!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 C(!e&&!r->value.controller_consumed&&!papacc_server_service_scq_closed_ready());
 if(f->plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY){
  C(scqr_first_prepare(r,&f->recipient,&f->runtime_owner,&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,&f->runtime_receiver,&f->plan,(HANDLE)44,&f->stop)!=0);
  C(r->phase==1U&&f->recipient.phase==1U&&!r->value.controller_consumed&&!cs_running);
 }
 if(scenario==21U){r->value.purpose=PAPACC_SCQ_PURPOSE_READ;C(scqr_current(r,&f->runtime_owner,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,&f->stop)!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 if(scenario==22U){f->runtime_dispatch.purpose=PAPACC_SCQ_PURPOSE_READ;C(scqr_current(r,&f->runtime_owner,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,&f->stop)!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 if(scenario==23U){r->value.key[111]=L'X';C(scqr_current(r,&f->runtime_owner,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,&f->stop)!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 if(scenario==24U){f->source.value.purpose=PAPACC_SCQ_PURPOSE_READ;C(scqa_current(&f->source,&f->administrator_owner,&f->stop)!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 if(scenario==25U){f->source.value.key[111]=L'X';C(scqa_current(&f->source,&f->administrator_owner,&f->stop)!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}

 mpeer=f->plan.runtime.pid;
 C(!scqt_prepare(&f->controller,&f->administrator_owner,(HANDLE)101,(HANDLE)102,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
 C(!transfer(&f->controller,&f->administrator_owner,1U,f,&f->source.value));
 receiver_identity_alias=1U;
 C(!scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));mpeer=f->plan.administrator.pid;
 C(!scqr_open(r,&f->recipient,&f->runtime_owner,in,L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.0123456789abcdef0123456789abcdef",&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
 mposition=0;C(!scqt_start_io(&f->recipient,&f->runtime_owner,2U,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 C(!papacc_server_service_scq_closed_ready());
 if(scenario==5U)mpeer++;
 e=scqr_admit_offer(r,&f->recipient,&f->runtime_owner,in,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop);
 if(scenario==5U){C(e!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 C(!e);moved[0]=(HANDLE)541;moved[1]=(HANDLE)542;moved[2]=(HANDLE)543;
 C(!scqt_accept_refs(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 C(!scqt_adopt_delivery(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 C(!scqt_apply(&f->recipient,&f->runtime_owner,2U,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 C(host.life.phase==PS_START_PENDING&&!cs_running);
 C(!papacc_server_service_scq_closed_ready()); /* original offer is not close evidence */
 C(!scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
 C(!papacc_server_service_scq_closed_ready()); /* DATA receipt is still not native close */
 C(!transfer(&f->recipient,&f->runtime_owner,1U,f,&r->value));mpeer=f->plan.runtime.pid;
 C(!transfer(&f->controller,&f->administrator_owner,2U,f,&f->source.value));mpeer=f->plan.administrator.pid;
 if(scenario==16U)mcase=13;
 e=scqt_produce_close(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop);
 if(scenario==16U){C(e!=0&&!papacc_server_service_scq_closed_ready()&&!cs_running);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 C(!e);
 C(!papacc_server_service_scq_closed_ready()); /* close not yet delivered */
 C(!transfer(&f->recipient,&f->runtime_owner,4U,f,&r->value));mpeer=f->plan.runtime.pid;
 if(scenario==17U)wire[PAPACC_SCQ_RECEIPT_BYTES]^=1U;
 e=(DWORD)transfer(&f->controller,&f->administrator_owner,8U,f,&f->source.value);mpeer=f->plan.administrator.pid;
 if(scenario==17U){C(e!=0&&!cs_running);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 C(!e);
 if(f->plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY){
  C(!papacc_server_service_scq_closed_ready()); /* local close and controller-local acceptance are insufficient */
  mpeer=f->plan.runtime.pid;C(!transfer(&f->controller,&f->administrator_owner,16U,f,&f->source.value));
  mpeer=f->plan.administrator.pid;C(!transfer(&f->recipient,&f->runtime_owner,32U,f,&r->value));
 }
 C(f->controller.transport.close_accepted&&f->recipient.transport.close_produced==3U);
 if(scenario==18U)f->runtime_profile.ack=0;
 if(scenario==19U)f->recipient.transport.close_produced=0;
 if(scenario==20U)f->recipient.transport.legs&=~4U;
 if(scenario==7U){cs_stop=1U;f->stop=1;}
 if(scenario==9U)f->recipient.peer_lost=1U;if(scenario==10U)f->policy.target[0]^=1;
 if(scenario==13U)f->runtime.creation.dwLowDateTime++;
 e=(DWORD)papacc_server_service_scq_closed_ready();
 if(scenario==14U||scenario==15U){C(!e&&!papacc_server_service_scq_closed_ready_current());papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}

 if(scenario==7U||scenario==9U||scenario==10U||scenario==13U||scenario>=18U){C(!e&&!host.closed_ready);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 C(e&&papacc_server_service_scq_closed_ready_current()&&host.closed_ready);cs_ready++;
 C(cs_running==1U&&!cs_premature&&host.life.phase==PS_RUNNING&&!papacc_server_service_runtime_ready()&&!papacc_server_service_scq_host_current());
 if(scenario==6U)C(!papacc_server_service_scq_closed_ready()); /* no second readiness transition */
 if(scenario==8U)mcase=13;
 e=scqs_close(&f->recipient,&f->runtime_owner,1);
 if(scenario==8U){C(e!=0);papacc_server_service_runtime_retained();cs_callback_checked=1U;return 2;}
 C(!e&&!scqr_close(r,&f->runtime_owner));
 C(!scqs_close(&f->controller,&f->administrator_owner,1U)&&f->source.phase==5U);
 C(papacc_server_service_runtime_stopping());papacc_server_service_runtime_closed();cs_callback_checked=1U;return 0;
}
#ifndef PAPACC_SCQ_PRE_READINESS_ENTRY
#define PAPACC_SCQ_PRE_READINESS_ENTRY main
#endif
int PAPACC_SCQ_PRE_READINESS_ENTRY(void)
{
 unsigned before=checks,ready_total=0,retained=0,mode,j,denied_start=0;int result;char reply[96];PAPACC_SCQ_ADMIN_SOURCE copied;
 char *args[]={"server","service","host","scq-closed"};
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(mode=0;mode<2U;mode++)for(scenario=0;scenario<(mode?36U:21U);scenario++){
  active_f=&pf[21U*mode+scenario];active_r=&pr[21U*mode+scenario];active_i=&pi[21U*mode+scenario];cs_reports=cs_runs=cs_parks=cs_running=cs_ready=cs_stop=cs_callback_checked=cs_armed=cs_premature=0;
  memset(&host,0,sizeof(host));memset(&native,0,sizeof(native));mcase=origin_fault=receiver_identity_alias=0;runtime_service_state=SERVICE_RUNNING;
  C(!setup_session_purpose(active_f,1,1U,mode?PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY:PAPACC_SCQ_PURPOSE_READ));
  if(mode){
   static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";
   active_f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";active_f->policy.parent=L"C:\\ProgramData\\Papinho";active_f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
   C(scqa_start_take(&active_f->source,&active_f->administrator_owner,&active_f->stop)!=0);
   if(scenario==26U){C(!active_f->source.start_phase&&!active_f->source.start_consumed&&!active_f->runtime.phase);denied_start++;continue;}
   C(!scqa_start_present(&active_f->source,&active_f->administrator_owner,&active_f->stop));
   C(scqa_start_present(&active_f->source,&active_f->administrator_owner,&active_f->stop)!=0);
   memcpy(reply,prefix,sizeof(prefix)-1U);for(j=0;j<32U;j++){reply[sizeof(prefix)-1U+2U*j]="0123456789abcdef"[active_f->plan.attempt[j]>>4];reply[sizeof(prefix)+2U*j]="0123456789abcdef"[active_f->plan.attempt[j]&15U];}reply[sizeof(prefix)-1U+64U]=0;
   if(scenario==27U){C(scqa_start_confirm(&active_f->source,&active_f->administrator_owner,"OWNER_SCQ ",10,&active_f->stop)!=0&&!active_f->source.start_consumed);denied_start++;continue;}
   if(scenario==28U){reply[sizeof(prefix)-1U]^=1;C(scqa_start_confirm(&active_f->source,&active_f->administrator_owner,reply,sizeof(prefix)-1U+64U,&active_f->stop)!=0);denied_start++;continue;}
   if(scenario==29U){copied=active_f->source;C(scqa_start_confirm(&copied,&active_f->administrator_owner,reply,sizeof(prefix)-1U+64U,&active_f->stop)!=0);denied_start++;continue;}
   if(scenario==30U){C(scqa_start_confirm(&active_f->source,&active_f->runtime_owner,reply,sizeof(prefix)-1U+64U,&active_f->stop)!=0);denied_start++;continue;}
   C(!scqa_start_confirm(&active_f->source,&active_f->administrator_owner,reply,sizeof(prefix)-1U+64U,&active_f->stop));
   C(scqa_start_confirm(&active_f->source,&active_f->administrator_owner,reply,sizeof(prefix)-1U+64U,&active_f->stop)!=0);
  }
  C(!scqs_prepare(&active_f->controller,&active_f->administrator_owner,1U,&active_f->controller_profile,&active_f->source,&active_f->controller_receiver,&active_f->controller_dispatch,&active_f->runtime));
  if(scenario!=1U)C(!scqs_take(&active_f->controller,&active_f->administrator_owner,&active_f->plan.administrator,NULL,active_f->plan.refs,&active_f->stop));
  if(mode&&scenario!=1U){
   if(scenario==31U)active_f->stop=1;
   if(scenario==32U)active_f->policy.decision[0]^=1;
   if(scenario==33U)receiver_now=active_f->plan.valid_until[0];
   if(scenario==34U){PAPACC_SCQ_ADMIN_SOURCE restarted;memset(&restarted,0,sizeof(restarted));C(scqa_start_take(&restarted,&active_f->administrator_owner,&active_f->stop)!=0);denied_start++;continue;}
   result=(int)scqa_start_take(&active_f->source,&active_f->administrator_owner,&active_f->stop);
   if(scenario>=31U&&scenario<=33U){C(result!=0&&!active_f->source.start_consumed);denied_start++;continue;}
   C(!result&&active_f->source.start_consumed&&!active_f->runtime.phase);
   C(scqa_start_take(&active_f->source,&active_f->administrator_owner,&active_f->stop)!=0);
   if(scenario==35U){C(scqs_close(&active_f->controller,&active_f->administrator_owner,1)!=0&&active_f->source.start_phase==4U&&active_f->source.value.retained);denied_start++;continue;}
   active_f->model_effects++; /* The single modeled Start, after BOTH acts. */
  }
  C(!active_f->runtime.phase);result=papacc_server_service_dispatch(4,args,cs_run);
  C(cs_callback_checked&&cs_runs==1U&&!cs_premature&&!qopens&&!qproviders);ready_total+=cs_ready;
  if(scenario==0U||scenario==6U||scenario==12U){C(result==0&&!cs_parks&&host.life.phase==PS_STOPPED);}
  else if(scenario==1U){C(result==2&&!cs_parks&&host.life.phase==PS_STOPPED&&!host.closed_ready);}
  else{C(result==2&&cs_parks==1U&&host.life.phase==PS_RETAINED);retained++;}
 }
 C(denied_start==10U);
 printf("PASS SCQ_PRE_READINESS_CHAIN cases=57 transport_cases=36 independent_start_denied=%u checks=%u ready=%u retained=%u EMPTY_RECIPIENT_FIRST_ACQUISITION PENDING_UNTIL_BILATERAL CLOSED_RUNNING_ONLY NATIVE_CLOSE_BEFORE_READY NO_PREMATURE_OR_PRODUCTIVE_RUNNING MODELED_ORIGINALS_TOKENS_SCM_IO_RNG_KSP NO_ENDPOINT\n",denied_start,checks-before,ready_total,retained);return 0;
}
