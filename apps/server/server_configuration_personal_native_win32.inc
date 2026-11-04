#include <wchar.h>
#include <stdio.h>
#include <aclapi.h>
#include <sddl.h>
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
#include "server_service_win32.h"
#endif
#include "server_personal_token_checks_win32.inc"
#include "personal_runtime_identity_win32.inc"
#include "personal_cng_probe_win32.inc"
#include "personal_install_paths_win32.inc"
#include "personal_scq_dispatch_win32.inc"
#include "personal_scq_handoff_win32.inc"
#include "personal_scq_inputs_win32.inc"
#include "personal_scq_profile_win32.inc"
#include "personal_scq_admin_win32.inc"
#include "personal_scq_start_assurance_win32.inc"
#include "server_personal_resource_sample_win32.inc"
#include "personal_scq_session_win32.inc"
#include "personal_scq_transport_win32.inc"
#include "personal_scq_recipient_win32.inc"
#include "personal_scq_read_diagnostic_win32.inc"
#include "personal_scq_read_runtime_win32.inc"
#define PAPACC_MACHINE_FIXTURE_DATA_HASH store_data_hash /* existing private DATA hash, never an authority or KSP backend */
#include "personal_machine_fixture_win32.inc"
#include "personal_scq_bootstrap_win32.inc"
#include "personal_confirmation_console_win32.inc"
/* Query an original already held by this owner. The private acquisition path
 * below builds only a candidate; attributed SCQ and Start acts are separate.
 * No CLI/file/IPC grant loader or arbitrary registration setter exists. */
static const PAPACC_SCQ_OWNER_DECISION *personal_scq_registered_owner_decision(PAPACC_CONFIGURATION_COMMIT *o);
/* Separate endpoint scope is never inferred from a generic SCQ approval. */
static const PAPACC_SCQ_OWNER_DECISION *personal_scq_registered_endpoint_decision(PAPACC_CONFIGURATION_COMMIT *o);
static DWORD personal_scq_query_resources(PAPACC_CONFIGURATION_COMMIT *,int);
static const PAPACC_SCQ_OWNER_DECISION *personal_scq_registered_cng_decision(void)
{return NULL; /* endpoint approval is not a factual KSP probe approval */}
static int personal_scq_recipient_native_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.stop)return 0;
 if(papacc_server_console_win32_stop_requested()){InterlockedExchange(&o->pending_commit.stop,1);return 0;}
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
 n=&o->pending_commit.personal_native;
 if(n->scq_profile_data.pre_readiness){
  if(!n->runtime_identity.closed_preparation||!papacc_server_service_scq_preparation_current())return 0;
 }else if(!papacc_server_service_scq_host_current())return 0;
 return personal_scq_registered_owner_decision(o)&&n->scq_profile_data.role==2U&&n->scq_recipient.registration==personal_scq_registered_owner_decision(o)&&
 !scqr_current(&n->scq_recipient,o,&n->scq_profile_data,&n->scq_inputs,&n->runtime_identity,&n->scq_dispatch,&o->pending_commit.stop);
#else
 (void)n;return 0;
#endif
}
static DWORD personal_scq_paths_current(const PAPACC_PERSONAL_INSTALL_PATHS *,DWORD,volatile LONG *);
static int personal_scq_admin_native_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard)return 0;
 p=&o->pending_commit;n=&p->personal_native;
 if(papacc_server_console_win32_stop_requested()){InterlockedExchange(&p->stop,1);return 0;}
 return !p->active&&!p->thread&&!p->stop&&!p->personal_ca&&!p->personal_ca_candidate&&
  n->scq_inputs.content_verified&&personal_scq_registered_owner_decision(o)&&
  n->scq_admin.registration==personal_scq_registered_owner_decision(o)&&
  n->scq_admin.registration->binary&&n->scq_admin.registration->parent&&n->scq_admin.registration->data&&
  !wcscmp(n->scq_admin.registration->binary,n->install_paths.binary)&&!wcscmp(n->scq_admin.registration->parent,n->install_paths.parent)&&
  !wcscmp(n->scq_admin.registration->data,n->install_paths.data)&&personal_token(p,0)&&
  !personal_scq_inputs_check(&n->scq_inputs,n->sid,&p->stop,0)&&!personal_scq_paths_current(&n->install_paths,n->scq_admin.registration->purpose,&p->stop);
}
#include "personal_scq_selection_win32.inc"
static const PAPACC_SCQ_OWNER_DECISION *personal_scq_registered_endpoint_decision(PAPACC_CONFIGURATION_COMMIT *o)
{
 /* Controller-local attributed endpoint act only. A cold receiver cannot
  * reconstruct it from SCQ1, endpoint possession, Start or a DATA ack. */
 if(!personal_scq_admin_native_current(o)){
  PAPACC_PERSONAL_NATIVE_CONTEXT *n;if(!o)return NULL;n=&o->pending_commit.personal_native;
  if(n->scq_recipient.phase==2U&&n->scq_recipient.value.controller_consumed&&personal_scq_recipient_native_current(o))return n->scq_recipient.registration;
  return NULL;
 }
 return scqa_endpoint_selection(&o->pending_commit.personal_native.scq_admin,o,&o->pending_commit.stop);
}

PAPACC_RESULT papacc_configuration_personal_scq_admin_prepare(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA plan;
 PAPACC_PERSONAL_SCQ_ORIGINAL v;const PAPACC_SCQ_OWNER_DECISION *registered=personal_scq_registered_owner_decision(o);FILETIME exited,kernel,user;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard)return PAPACC_RESULT_INVALID_STATE;
 if(!registered){
  /* Only the native original-acquisition sequence below may create this
   * candidate. A carrier, runtime callback or metadata getter cannot. */
  PAPACC_PERSONAL_NATIVE_CONTEXT *owned;
  if(!o)return PAPACC_RESULT_INVALID_STATE;owned=&o->pending_commit.personal_native;
  if(owned->scq_selection_captured!=1U||owned->scq_admin.phase)return PAPACC_RESULT_NOT_SUPPORTED;
  registered=&owned->scq_selection;
 }
 p=&o->pending_commit;n=&p->personal_native;
 if(n->scq_admin.phase||!registered->binary||!registered->parent||!registered->data||
  wcscmp(registered->binary,n->install_paths.binary)||wcscmp(registered->parent,n->install_paths.parent)||wcscmp(registered->data,n->install_paths.data)||!n->scq_inputs.content_verified||p->active||p->thread||p->stop||!personal_token(p,0)||
  personal_scq_paths_current(&n->install_paths,registered->purpose,&p->stop)||personal_scq_inputs_check(&n->scq_inputs,n->sid,&p->stop,0)||
  scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&plan)||!scqa_scope(registered,&plan))return PAPACC_RESULT_INVALID_STATE;
 memset(&v,0,sizeof(v));v.controller=o;v.recipient=o;v.purpose=plan.purpose;
 v.request=n->scq_inputs.reader[0];v.decision=n->scq_inputs.reader[1];v.image=n->scq_inputs.reader[2];
 v.request_id=n->scq_inputs.identity[0];v.decision_id=n->scq_inputs.identity[1];v.image_id=n->scq_inputs.identity[2];
 v.operator_process=GetCurrentProcess();v.operator_pid=GetCurrentProcessId();v.operator_token=p->bootstrap_token;v.operator_statistics=n->token;
 if(!GetProcessTimes(v.operator_process,&v.operator_creation,&exited,&kernel,&user))return PAPACC_RESULT_INVALID_STATE;
 memcpy(v.operator_sid,n->sid,n->sid_bytes);v.operator_sid_bytes=n->sid_bytes;
 v.deadline=((ULONGLONG)plan.valid_until[1]<<32)|plan.valid_until[0];
 return scqa_capture(&n->scq_admin,o,registered,&n->scq_profile_data,&v,&p->stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
/* Controller source only, pre-runtime. No return of the borrowed pointer
 * across principals and no permission for endpoint/lifecycle/KSP effects. */
PAPACC_RESULT papacc_configuration_personal_scq_startup_source_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;
 return scqa_startup_original(&n->scq_admin,o,&o->pending_commit.stop)?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_personal_scq_closed_ready_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!personal_scq_recipient_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;
 if(!n->scq_profile_data.pre_readiness||!n->runtime_identity.closed_preparation)return PAPACC_RESULT_INVALID_STATE;
 if(scqr_closed_ready_current(&n->scq_recipient,&n->scq_session,o,&n->scq_inputs,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 /* The receiver query above requires the original controller return leg
  * for TRANSPORT_ONLY, not local write completion or a caller DATA bit. */
 return PAPACC_RESULT_OK;
}
#include "personal_scq_inconclusive_controller_win32.inc"
PAPACC_RESULT papacc_configuration_personal_scq_closed_inconclusive_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!personal_scq_recipient_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;
 if(!n->scq_profile_data.pre_readiness||n->scq_profile_data.read_diagnostic!=1U||n->runtime_identity.closed_preparation!=1U||n->runtime_identity.read_diagnostic!=1U)return PAPACC_RESULT_INVALID_STATE;
 return scqr_closed_terminal_current(&n->scq_recipient,&n->scq_session,o,&n->scq_inputs,&o->pending_commit.stop,1U)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_closed_arm_running(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(papacc_configuration_personal_scq_closed_ready_current(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 return scqr_arm_closed_running(&n->scq_recipient,&n->scq_session,o,&n->scq_inputs,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
#include "personal_scq_console_projection_win32.inc"
PAPACC_RESULT papacc_configuration_personal_scq_admin_confirm_console(PAPACC_CONFIGURATION_COMMIT *o)
{
 HANDLE in,out;DWORD mode;PAPACC_SCQ_PLAN_DATA plan;char reply[75];unsigned i;
 PAPACC_SCQ_ADMIN_SOURCE *a;
 if(!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 a=&o->pending_commit.personal_native.scq_admin;in=GetStdHandle(STD_INPUT_HANDLE);out=GetStdHandle(STD_OUTPUT_HANDLE);
 if(!o->pending_commit.personal_native.console.is_installed||!GetConsoleMode(in,&mode)||!GetConsoleMode(out,&mode)||scqp_decode_fields(a->plan->record,sizeof(a->plan->record),&plan)||
  scqa_present(a,o,&o->pending_commit.stop)||!scqa_console_projection(o,out,&plan,&o->pending_commit.personal_native.install_paths)){(void)scqa_fail(a,ERROR_ACCESS_DENIED);return PAPACC_RESULT_INVALID_STATE;}
 memcpy(reply,"OWNER_SCQ ",10);for(i=0;i<32;i++){reply[10+2*i]="0123456789abcdef"[plan.attempt[i]>>4];reply[11+2*i]="0123456789abcdef"[plan.attempt[i]&15U];}reply[74]=0;
 if(!personal_console_write(out,reply)||!personal_console_write(out,"\r\n")||!personal_console_confirm(in,out,reply)||!personal_scq_admin_native_current(o)){
  (void)scqa_fail(a,ERROR_CANCELLED);return PAPACC_RESULT_INVALID_STATE;}
 return scqa_confirm(a,o,reply,74,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
/* Separate operation-specific attributed event, not reuse of OWNER_SCQ or
 * OWNER_SCM_REGISTER. Its scope borrows the original immutable SCQ plan.
 * This producer neither calls StartService nor authorizes an endpoint. */
PAPACC_RESULT papacc_configuration_personal_scq_start_confirm_console(PAPACC_CONFIGURATION_COMMIT *o)
{
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";
 HANDLE in,out;DWORD mode;PAPACC_SCQ_PLAN_DATA plan;char reply[sizeof(prefix)+64U];unsigned i;
 PAPACC_SCQ_ADMIN_SOURCE *a;
 if(!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 a=&o->pending_commit.personal_native.scq_admin;in=GetStdHandle(STD_INPUT_HANDLE);out=GetStdHandle(STD_OUTPUT_HANDLE);
 if(!o->pending_commit.personal_native.console.is_installed||!GetConsoleMode(in,&mode)||!GetConsoleMode(out,&mode)||
  scqp_decode_fields(a->plan->record,sizeof(a->plan->record),&plan)||
  !scqa_console_projection(o,out,&plan,&o->pending_commit.personal_native.install_paths)||
  !personal_console_write(out,"ONE DIAGNOSTIC START ONLY: PapinhoAccelerator / NT SERVICE\\PapinhoAccelerator; registered command service host scq-closed. No registration, Start retry, Stop, endpoint, KSP, CA, CURRENT or productive readiness authority. Independent lifecycle/resource gates remain mandatory.\r\n")||
  scqa_start_present(a,o,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 memcpy(reply,prefix,sizeof(prefix)-1U);for(i=0;i<32U;i++){reply[sizeof(prefix)-1U+2U*i]="0123456789abcdef"[plan.attempt[i]>>4];reply[sizeof(prefix)+2U*i]="0123456789abcdef"[plan.attempt[i]&15U];}reply[sizeof(reply)-1U]=0;
 if(!personal_console_write(out,reply)||!personal_console_write(out,"\r\n")||!personal_console_confirm(in,out,reply)||!personal_scq_admin_native_current(o)){
  (void)scqa_start_fail(a,ERROR_CANCELLED);return PAPACC_RESULT_INVALID_STATE;
 }
 return scqa_start_confirm(a,o,reply,sizeof(reply)-1U,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_endpoint_confirm_console(PAPACC_CONFIGURATION_COMMIT *o)
{
 static const char prefix[]="OWNER_SCQ_PRIVATE_ENDPOINT ";
 HANDLE in,out;DWORD mode;PAPACC_SCQ_PLAN_DATA plan;char reply[sizeof(prefix)+64U];unsigned i;
 PAPACC_SCQ_ADMIN_SOURCE *a;
 if(!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 a=&o->pending_commit.personal_native.scq_admin;in=GetStdHandle(STD_INPUT_HANDLE);out=GetStdHandle(STD_OUTPUT_HANDLE);
 if(!o->pending_commit.personal_native.console.is_installed||!GetConsoleMode(in,&mode)||!GetConsoleMode(out,&mode)||
  scqp_decode_fields(a->plan->record,sizeof(a->plan->record),&plan)||
  !scqa_console_projection(o,out,&plan,&o->pending_commit.personal_native.install_paths)||
  !personal_console_write(out,a->plan->read_diagnostic?"ONE PRIVATE MACHINE_READ ENDPOINT ONLY; E IS NOT Q, S OR P; NO_KSP_AUTHORITY_NO_PRODUCTION; ORIGINAL_SOURCE_RESOURCE_AND_BILATERAL_GATES_REQUIRED.\r\n":"ONE PRIVATE TRANSPORT_ONLY ENDPOINT ONLY: exact displayed attempt and controller/runtime pair. Exclusive local SCQ namespace; no adoption, reconnect or retry. No Start/Stop, registration, KSP, CA, CURRENT, productive network or readiness authority. Independent original-source and resource gates remain mandatory.\r\n")||
  scqa_endpoint_present(a,o,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 memcpy(reply,prefix,sizeof(prefix)-1U);for(i=0;i<32U;i++){reply[sizeof(prefix)-1U+2U*i]="0123456789abcdef"[plan.attempt[i]>>4];reply[sizeof(prefix)+2U*i]="0123456789abcdef"[plan.attempt[i]&15U];}reply[sizeof(reply)-1U]=0;
 if(!personal_console_write(out,reply)||!personal_console_write(out,"\r\n")||!personal_console_confirm(in,out,reply)||!personal_scq_admin_native_current(o)){
  (void)scqa_endpoint_fail(a,ERROR_CANCELLED);return PAPACC_RESULT_INVALID_STATE;
 }
 return scqa_endpoint_confirm(a,o,reply,sizeof(reply)-1U,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_start_plan_capture(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_START_LIMITS *limits)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;
 if(!scqg_selected_exact_mode(limits,240000U,PAPACC_SCQ_P1_PATHS(&n->install_paths)))return PAPACC_RESULT_INVALID_STATE;
 if(n->scq_profile_data.read_diagnostic){if(!PAPACC_SCQ_P1_PATHS(&n->install_paths)||n->install_paths.read_diagnostic!=1U||scqg_capture_machine_read(&n->scq_start_assurance,o,&n->scq_admin,limits,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;}
 else if(scqg_capture_mode(&n->scq_start_assurance,o,&n->scq_admin,limits,&o->pending_commit.stop,PAPACC_SCQ_P1_PATHS(&n->install_paths)))return PAPACC_RESULT_INVALID_STATE;
 if(n->scq_start_assurance.p1_handles){
  n->scq_start_assurance.initial_known=n->scq_cold_diagnostic.initial_known;
  n->scq_start_assurance.initial_handles=n->scq_cold_diagnostic.initial_handles;
  if(!n->scq_start_assurance.initial_known||n->scq_start_assurance.initial_handles>PAPACC_SCQ_P1_ENTRY)return PAPACC_RESULT_INVALID_STATE;
 }
 return PAPACC_RESULT_OK;
}
DWORD papacc_configuration_personal_scq_start_resource_current(PAPACC_CONFIGURATION_COMMIT *o,int creating)
{
 PAPACC_SCQ_START_ASSURANCE *g;LARGE_INTEGER now;DWORD e;
 if(!personal_scq_admin_native_current(o))return ERROR_NOT_SUPPORTED;
 g=&o->pending_commit.personal_native.scq_start_assurance;
 if(!scqg_scope(g,o,&o->pending_commit.stop))return ERROR_NOT_SUPPORTED;
 e=personal_resource_collect(&g->observations,creating?1U:2U,0);if(e)return scqg_fail(g,e);
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0)return scqg_fail(g,ERROR_INVALID_DATA);
 if(g->phase==1U){ULONGLONG tick=GetTickCount64();
  e=scqg_ready(g,o,&g->observations.last,(ULONGLONG)now.QuadPart,&o->pending_commit.stop);if(e)return e;
  return scqg_controller_deadline(g,tick);
 }
 if(!g->global_until||GetTickCount64()>=g->global_until)return scqg_fail(g,ERROR_TIMEOUT);
 return scqg_admit(g,o,&g->observations.last,(ULONGLONG)now.QuadPart,creating,&o->pending_commit.stop);
}
#include "personal_scq_start_take_win32.inc"
PAPACC_RESULT papacc_configuration_personal_scq_start_carrier(PAPACC_CONFIGURATION_COMMIT *o,PS_SCQ_CARRIER *out)
{
 PAPACC_RESULT result;ULONGLONG deadline;
 if(!out||!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 result=scqa_start_carrier(&o->pending_commit.personal_native.scq_admin,o,out,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
 deadline=o->pending_commit.personal_native.scq_start_assurance.global_until;
 if(result==PAPACC_RESULT_OK){out->global_until[0]=(unsigned)deadline;out->global_until[1]=(unsigned)(deadline>>32);}return result;
}
PAPACC_RESULT papacc_configuration_personal_scq_admin_take(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 return scqa_take(&o->pending_commit.personal_native.scq_admin,o,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static PAPACC_PERSONAL_SCQ_ORIGINAL *personal_scq_native_original(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 if(n->scq_profile_data.role==2U){
  if(!personal_scq_recipient_native_current(o)||n->scq_recipient.phase!=2U)return NULL;
  return &n->scq_recipient.value;
 }
 if(!personal_scq_admin_native_current(o))return NULL;
 return scqa_borrow(&n->scq_admin,o,&n->scq_dispatch,&n->runtime_identity,&o->pending_commit.stop);
}

static PAPACC_RESULT personal_scq_profile_prepare_mode(PAPACC_CONFIGURATION_COMMIT *o,DWORD role,const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_SCQ_PEER_DATA *local,unsigned closed)
{
 PAPACC_CONFIG_PENDING *p;
 if(!o||o->writer!=GetCurrentThreadId()||o->initialized||o->stopped||o->unresolved||o->published||o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;
 if(p->active||p->thread||p->stop||p->personal_ca||p->personal_ca_candidate||p->personal_native.install_paths.phase!=PAPACC_PERSONAL_PATHS_READY||
  personal_scq_paths_current(&p->personal_native.install_paths,closed==2U&&plan?plan->purpose:PAPACC_SCQ_PURPOSE_READ,&p->stop))return PAPACC_RESULT_INVALID_STATE;
 /* Valid projection DATA remains untrusted as native peer/source evidence. */
 if(closed==3U&&(!plan||plan->purpose!=PAPACC_SCQ_PURPOSE_READ||!PAPACC_SCQ_P1_PATHS(&p->personal_native.install_paths)||p->personal_native.install_paths.read_diagnostic!=1U))return PAPACC_RESULT_INVALID_STATE;
 return (closed==3U?papacc_scq_profile_prepare_machine_read_data(&p->personal_native.scq_profile_data,o,role,plan,local):closed==2U?papacc_scq_profile_prepare_transport_data(&p->personal_native.scq_profile_data,o,role,plan,local):closed?papacc_scq_profile_prepare_closed_data(&p->personal_native.scq_profile_data,o,role,plan,local):papacc_scq_profile_prepare_data(&p->personal_native.scq_profile_data,o,role,plan,local))?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_data(PAPACC_CONFIGURATION_COMMIT *o,DWORD role,const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_SCQ_PEER_DATA *local)
{return personal_scq_profile_prepare_mode(o,role,plan,local,0U);}
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_closed_data(PAPACC_CONFIGURATION_COMMIT *o,DWORD role,const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_SCQ_PEER_DATA *local)
{return personal_scq_profile_prepare_mode(o,role,plan,local,1U);}
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_machine_read_data(PAPACC_CONFIGURATION_COMMIT *o,DWORD role,const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_SCQ_PEER_DATA *local)
{return personal_scq_profile_prepare_mode(o,role,plan,local,3U);}
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_transport_data(PAPACC_CONFIGURATION_COMMIT *o,DWORD role,const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_SCQ_PEER_DATA *local)
{return personal_scq_profile_prepare_mode(o,role,plan,local,2U);}
PAPACC_RESULT papacc_configuration_personal_scq_acquire_contents(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_SCQ_PLAN_DATA plan;PAPACC_SCQ_PROFILE_DATA *v;ULONGLONG until;FILETIME created,exited,kernel,user;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;v=&p->personal_native.scq_profile_data;
 if(p->active||p->thread||p->stop||p->personal_ca||p->personal_ca_candidate||
  !scqp_owned(v,o)||v->role!=1U||v->phase!=1U||
  scqp_decode_fields(v->record,sizeof(v->record),&plan)||!scqp_static(&plan)||!scqp_window(&plan)||
  personal_scq_paths_current(&p->personal_native.install_paths,plan.purpose,&p->stop)||
  memcmp(plan.request,plan.refs[0].digest,32)||memcmp(plan.decision,plan.refs[1].digest,32))return PAPACC_RESULT_INVALID_STATE;
 if(!personal_token(p,0)||plan.administrator.pid!=GetCurrentProcessId()||plan.administrator.sid_bytes!=p->personal_native.sid_bytes||
  memcmp(plan.administrator.sid,p->personal_native.sid,plan.administrator.sid_bytes)||plan.administrator.session!=p->personal_native.session||
  memcmp(plan.administrator.token_id,&p->personal_native.token.TokenId,sizeof(LUID))||
  memcmp(plan.administrator.authentication,&p->personal_native.token.AuthenticationId,sizeof(LUID))||
  memcmp(plan.administrator.modified,&p->personal_native.token.ModifiedId,sizeof(LUID))||
  !GetProcessTimes(GetCurrentProcess(),&created,&exited,&kernel,&user)||memcmp(plan.administrator.creation,&created,sizeof(created)))return PAPACC_RESULT_INVALID_STATE;
 until=((ULONGLONG)plan.valid_until[1]<<32)|plan.valid_until[0];
 return personal_scq_inputs_contents(&p->personal_native.scq_inputs,p->personal_native.sid,plan.refs,until,&p->stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
/* SCQ has its own attributed original source and consumption. The immutable
 * operational registration remains empty; no NEW_INSTALL or DATA grant. */
static DWORD personal_scq_native_source_status(PAPACC_CONFIGURATION_COMMIT *o)
{PAPACC_CONFIG_PENDING *p=&o->pending_commit;if(personal_scq_native_original(o))return personal_scq_registered_cng_decision()&&personal_scq_registered_cng_decision()==personal_scq_registered_owner_decision(o)?0:PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE;return p->personal_native.scq_inputs.phase==1U&&!p->personal_native.scq_inputs.first?
 PAPACC_SCQ_GATE_ORIGINAL_PROVENANCE_UNAVAILABLE:PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE;}
/* Source-independent custody step: uses actual administrative token checks,
 * but cannot confirm SCQ, manufacture an original approval or admit dispatch. */
PAPACC_RESULT papacc_configuration_personal_scq_capture_inputs(PAPACC_CONFIGURATION_COMMIT *o,const HANDLE originals[3])
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA plan;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard||!originals)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(n->install_paths.mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT){
  /* P0-P7 cold controller already owns token/chain. Never recapture their
   * lifetime or route READ/NEW_INSTALL through this specialized entry. */
  if(p->active||p->thread||p->stop||p->personal_ca||p->personal_ca_candidate||
   n->phase!=1U||!p->bootstrap_token||n->scq_inputs.phase||
   n->install_paths.selected_role!=1U||n->scq_inputs.image_chain.owner!=o||
   !scqp_owned(&n->scq_profile_data,o)||n->scq_profile_data.role!=1U||
   !n->scq_profile_data.pre_readiness||!n->scq_profile_data.waiting_runtime||
   scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&plan)||
   !PAPACC_SCQ_EXISTING_PATH_PURPOSE(&n->install_paths,plan.purpose)||n->scq_profile_data.read_diagnostic!=n->install_paths.read_diagnostic||
   personal_scq_paths_current(&n->install_paths,plan.purpose,&p->stop)||!personal_token(p,0))return PAPACC_RESULT_INVALID_STATE;
  return (n->scq_profile_data.read_diagnostic?personal_scq_inputs_capture_machine_read:personal_scq_inputs_capture_transport)(&n->scq_inputs,originals,n->sid,&plan,&n->install_paths,o,&p->stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
 }
 if(p->active||p->thread||p->stop||p->personal_ca||p->personal_ca_candidate||n->phase||p->bootstrap_token||
  n->install_paths.phase!=PAPACC_PERSONAL_PATHS_READY||n->install_paths.mode!=PAPACC_PERSONAL_PATHS_NEW||n->scq_inputs.phase||
  papacc_personal_paths_current(&n->install_paths,&p->stop))return PAPACC_RESULT_INVALID_STATE;
 n->phase=1;
 if(!personal_token(p,1))return PAPACC_RESULT_INVALID_STATE;
 if(scqp_owned(&n->scq_profile_data,o)&&!scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&plan)&&
  plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)
  return personal_scq_inputs_capture_transport(&n->scq_inputs,originals,n->sid,&plan,&n->install_paths,o,&p->stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
 return personal_scq_inputs_capture(&n->scq_inputs,originals,n->sid,&p->stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_inputs_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;
 if(p->active||p->thread||p->stop)return PAPACC_RESULT_INVALID_STATE;
 if(!personal_token(p,0)){(void)personal_scq_inputs_fail(&p->personal_native.scq_inputs,11,ERROR_ACCESS_DENIED);return PAPACC_RESULT_INVALID_STATE;}
 return personal_scq_inputs_check(&p->personal_native.scq_inputs,p->personal_native.sid,&p->stop,0)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
/* Original producer and receiver share the canonical packet locally. No
 * pointer is serialized into a peer; native bilateral delivery remains a
 * separate gate. Empty operational registration supplies no original. */
PAPACC_RESULT papacc_configuration_personal_scq_receive_original(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;DWORD e;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.personal_ca||o->pending_commit.personal_ca_candidate)return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 if(n->scq_dispatch.phase!=1U||n->scq_receiver.phase)return PAPACC_RESULT_INVALID_STATE;
 e=personal_scq_receive_original(&n->scq_receiver,personal_scq_native_original(o),o,&n->scq_dispatch,&n->runtime_identity,&o->pending_commit.stop);
 n->scq_dispatch.gate_reason=personal_scq_native_source_status(o);
 return !e?PAPACC_RESULT_OK:e==PAPACC_SCQ_RECEIVE_ABSENT?PAPACC_RESULT_NOT_SUPPORTED:PAPACC_RESULT_INVALID_STATE;
}
static int personal_layout_mutable(PAPACC_CONFIGURATION_COMMIT *o)
{return o&&o->writer==GetCurrentThreadId()&&!o->stopped&&!o->unresolved&&!o->pending_commit.stop&&!o->pending_commit.active&&!o->pending_commit.thread&&
 !o->pending_commit.personal_ca&&!o->pending_commit.personal_ca_candidate&&!o->pending_commit.personal_native.scq_profile_data.phase&&!o->pending_commit.personal_native.scq_inputs.phase&&!o->pending_commit.personal_native.scq_dispatch.phase&&
 o->pending_commit.personal_data.phase<PAPACC_PERSONAL_DATA_PRESENTED;}
PAPACC_RESULT papacc_configuration_personal_layout_new(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!personal_layout_mutable(o)||o->initialized||o->published||o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard)return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_paths_new(&o->pending_commit.personal_native.install_paths,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static int personal_binary_context_owned(const PAPACC_PERSONAL_NATIVE_CONTEXT *n)
{
 const PAPACC_QUALIFICATION_STAGING *s=&n->staging;const PAPACC_SCM_REGISTRATION_ACT *a=&n->scm_registration;
 return (s->self==s&&s->thread==GetCurrentThreadId()&&s->invocation&&s->phase>=1U&&s->phase<=4U)||
 (a->self==a&&a->original&&a->thread==GetCurrentThreadId()&&a->invocation&&a->phase>=1U&&a->phase<=4U);
}
static int personal_binary_staging_owned(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return 0;
 return personal_binary_context_owned(&o->pending_commit.personal_native);
}
PAPACC_RESULT papacc_configuration_personal_layout_binary(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!personal_binary_staging_owned(o)||!personal_layout_mutable(o)||o->initialized||o->published||o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard)return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_paths_binary(&o->pending_commit.personal_native.install_paths,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_layout_binary_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!personal_binary_staging_owned(o))return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_paths_binary_current(&o->pending_commit.personal_native.install_paths,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_layout_existing(PAPACC_CONFIGURATION_COMMIT *o,const WCHAR *binary,const WCHAR *parent,const WCHAR *leaf)
{
 if(!personal_layout_mutable(o))return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_paths_existing(&o->pending_commit.personal_native.install_paths,binary,parent,leaf)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_layout_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_paths_current(&o->pending_commit.personal_native.install_paths,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_prepare_data(PAPACC_CONFIGURATION_COMMIT *o,const BYTE association[32],const WCHAR *name)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.personal_ca||o->pending_commit.personal_ca_candidate)return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 if(n->runtime_cng_probe.phase||papacc_personal_paths_current(&n->install_paths,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_scq_prepare(&n->scq_dispatch,&n->runtime_identity,association,name)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_dispatch(PAPACC_CONFIGURATION_COMMIT *o,const BYTE association[32])
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(o&&(o->pending_commit.personal_native.scq_dispatch.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||o->pending_commit.personal_native.scq_profile_data.pre_readiness))return PAPACC_RESULT_NOT_SUPPORTED; /* no productive/KSP transition from closed prep */
 if(!o||o->writer!=GetCurrentThreadId()||!association)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(n->scq_dispatch.phase!=1U||p->active||p->thread||p->personal_ca||p->personal_ca_candidate)return PAPACC_RESULT_INVALID_STATE;
 if(o->stopped||p->stop||o->unresolved){n->scq_dispatch.result=PAPACC_CNG_PROBE_STOPPED;n->scq_dispatch.phase=3U;return PAPACC_RESULT_INVALID_STATE;}
 if(memcmp(association,n->scq_dispatch.association,32U)||papacc_personal_paths_current(&n->install_paths,&p->stop)){
  n->scq_dispatch.result=PAPACC_CNG_PROBE_LOCAL_DENY;n->scq_dispatch.phase=3U;return PAPACC_RESULT_INVALID_STATE;}
 /* Frozen DATA, path resolution and matching identity do NOT grant the
  * native operation. Original application admission remains mandatory. */
 n->scq_dispatch.gate_reason=personal_scq_native_source_status(o);
 if(n->scq_dispatch.gate_reason){
  n->scq_dispatch.gate_result=PAPACC_RESULT_NOT_SUPPORTED;n->scq_dispatch.result=PAPACC_CNG_PROBE_LOCAL_DENY;n->scq_dispatch.phase=3U;return PAPACC_RESULT_NOT_SUPPORTED;}
 if(personal_scq_receive_consume(&n->scq_receiver,o,&p->stop)){
  n->scq_dispatch.result=PAPACC_CNG_PROBE_LOCAL_DENY;n->scq_dispatch.phase=3U;return PAPACC_RESULT_INVALID_STATE;}
 (void)personal_scq_dispatch_body(&n->scq_dispatch,&n->runtime_identity,&n->runtime_cng_probe,association,&p->stop);
 if(personal_scq_receive_finish(&n->scq_receiver,o,n->scq_dispatch.phase==3U)){
  o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(n->scq_dispatch.phase==4U){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 return PAPACC_RESULT_OK; /* observation receipt only; never CA/admission */
}
PAPACC_RESULT papacc_configuration_personal_runtime_cng_probe(PAPACC_CONFIGURATION_COMMIT *o,const WCHAR *name)
{
 PAPACC_CONFIG_PENDING *p;size_t name_bytes;
 if(o&&(o->pending_commit.personal_native.scq_dispatch.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||o->pending_commit.personal_native.scq_profile_data.pre_readiness))return PAPACC_RESULT_NOT_SUPPORTED; /* no productive/KSP transition from closed prep */
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;
 if(p->stop||p->active||p->thread||p->personal_ca||p->personal_ca_candidate)return PAPACC_RESULT_INVALID_STATE;
 /* Existing native authority gate stays closed. This API cannot activate
  * native effects by argv, DATA or fixture observations. */
 p->personal_native.scq_dispatch.gate_reason=personal_scq_native_source_status(o);
 if(p->personal_native.scq_dispatch.gate_reason)return PAPACC_RESULT_NOT_SUPPORTED;
 if(!name||!pcp_name(name,&name_bytes)||wcscmp(name,p->personal_native.scq_dispatch.name))return PAPACC_RESULT_INVALID_ARGUMENT;
 return papacc_configuration_personal_scq_dispatch(o,p->personal_native.scq_dispatch.association);
}
PAPACC_RESULT papacc_configuration_personal_runtime_bind(PAPACC_CONFIGURATION_COMMIT *o,HANDLE process,PSID sid,unsigned profile,DWORD sid_type)
{
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 if(o->pending_commit.personal_native.scq_profile_data.pre_readiness){
  /* The controller must already hold its independently confirmed/taken act;
   * selecting a pending peer is not authority to Start or open an endpoint. */
  if(profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||papacc_configuration_personal_scq_startup_source_current(o)!=PAPACC_RESULT_OK||!o->pending_commit.personal_native.scq_admin.registration)return PAPACC_RESULT_NOT_SUPPORTED;
  return (o->pending_commit.personal_native.scq_admin.registration->read_diagnostic==1U?papacc_personal_runtime_capture_machine_read_closed:o->pending_commit.personal_native.scq_admin.registration->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?papacc_personal_runtime_capture_transport_closed:papacc_personal_runtime_capture_closed)(&o->pending_commit.personal_native.runtime_identity,process,sid,sid_type)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
 }
 return papacc_personal_runtime_capture(&o->pending_commit.personal_native.runtime_identity,process,sid,profile,sid_type)==0?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_personal_runtime_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_runtime_current(&o->pending_commit.personal_native.runtime_identity)==0?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_personal_scq_observe_runtime_data(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_SCQ_RUNTIME_OBSERVATION *out)
{
 ULONG_PTR a=(ULONG_PTR)out,b=(ULONG_PTR)o;
 if(!o||!out||(a>=b?a-b<sizeof(*o):b-a<sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 if(o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread||
  o->pending_commit.personal_ca||o->pending_commit.personal_ca_candidate)return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_runtime_observe_data(&o->pending_commit.personal_native.runtime_identity,out,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_observe_image_data(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_SCQ_RUNTIME_OBSERVATION *out)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;ULONG_PTR a=(ULONG_PTR)out,b=(ULONG_PTR)o;
 if(!o||!out||(a>=b?a-b<sizeof(*o):b-a<sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(papacc_configuration_personal_scq_observe_runtime_data(o,out)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 return personal_scq_inputs_image_data(&n->scq_inputs,n->sid,&n->runtime_identity,out,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_runtime_close(PAPACC_CONFIGURATION_COMMIT *o)
{
 DWORD e;if(!o||o->writer!=GetCurrentThreadId()||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.personal_native.scq_receiver.original||o->pending_commit.personal_native.scq_receiver.phase==4U||o->pending_commit.personal_ca||o->pending_commit.personal_ca_candidate||
 (o->pending_commit.personal_native.scq_dispatch.phase&&o->pending_commit.personal_native.scq_dispatch.phase!=3U)||(o->pending_commit.personal_native.runtime_cng_probe.phase&&o->pending_commit.personal_native.runtime_cng_probe.phase!=3U))return PAPACC_RESULT_INVALID_STATE;
 e=papacc_personal_runtime_close(&o->pending_commit.personal_native.runtime_identity);
 if(o->pending_commit.personal_native.runtime_identity.phase==4U)o->unresolved=1;
 return e?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
#include "server_configuration_personal_open_win32.inc"
static PAPACC_RESULT personal_packet_current(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;BY_HANDLE_FILE_INFORMATION f;int same;
 if(InterlockedCompareExchange(&p->stop,0,0))return PAPACC_RESULT_INVALID_STATE;
 n=&p->personal_native;
 if(n->install_paths.phase){
  if(n->install_paths.mode==PAPACC_PERSONAL_PATHS_BINARY_ONLY){
   if(!personal_binary_context_owned(n)||
    papacc_personal_paths_binary_current(&n->install_paths,&p->stop))return PAPACC_RESULT_INVALID_STATE;
  }else if(papacc_personal_paths_current(&n->install_paths,&p->stop))return PAPACC_RESULT_INVALID_STATE;
 }
 if(!personal_token(p,0)||!p->bootstrap_parent||!n->descriptor||n->check_descriptor||
 !GetFileInformationByHandle(p->bootstrap_parent,&f)||memcmp(&f.dwVolumeSerialNumber,&n->parent.dwVolumeSerialNumber,sizeof(DWORD))||
 f.nFileIndexHigh!=n->parent.nFileIndexHigh||f.nFileIndexLow!=n->parent.nFileIndexLow||
 !(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return PAPACC_RESULT_INVALID_STATE;
 if(GetSecurityInfo(p->bootstrap_parent,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->check_descriptor)!=ERROR_SUCCESS)return PAPACC_RESULT_INVALID_STATE;
 same=IsValidSecurityDescriptor(n->check_descriptor)&&n->descriptor_bytes==GetSecurityDescriptorLength(n->check_descriptor)&&
 !memcmp(n->descriptor,n->check_descriptor,n->descriptor_bytes);
 if(LocalFree(n->check_descriptor)){n->phase=2U;return PAPACC_RESULT_INTERNAL_ERROR;}n->check_descriptor=NULL;
 return same?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_personal_native_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_RESULT r;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 r=personal_packet_current(&o->pending_commit);
 if(o->pending_commit.personal_native.phase==2U)o->unresolved=1;
 return r;
}
/* No endpoint or native IO backend is installed by this composition. All
 * outputs below are private DATA, never an authenticated acknowledgement. */
static void personal_scq_session_native_failed(PAPACC_CONFIGURATION_COMMIT *o,DWORD error)
{
 PAPACC_SCQ_SESSION_DRIVER *c;
 if(!o||o->writer!=GetCurrentThreadId())return;c=&o->pending_commit.personal_native.scq_session;
 if(scqs_owned(c,o)&&c->phase>=1U&&c->phase<=4U)(void)scqs_fail(c,error);
}
static int personal_scq_session_native_current(PAPACC_CONFIGURATION_COMMIT *o)
{if(personal_scq_recipient_native_current(o))return 1;
 if(personal_scq_admin_native_current(o))return 1;personal_scq_session_native_failed(o,ERROR_ACCESS_DENIED);return 0;}
#include "personal_scq_acquire_originals_win32.inc"
PAPACC_RESULT papacc_configuration_personal_scq_session_prepare(PAPACC_CONFIGURATION_COMMIT *o,DWORD role)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 if(!personal_scq_registered_owner_decision(o))return PAPACC_RESULT_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;
 if(role==1U){
  if(!n->scq_inputs.content_verified&&papacc_configuration_personal_scq_acquire_contents(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
  if(!n->scq_admin.phase&&papacc_configuration_personal_scq_admin_prepare(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
  /* Presentation/confirmation remains a separate explicit manual action. */
  if(!personal_scq_session_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
  if(n->scq_admin.phase!=3U)return PAPACC_RESULT_INVALID_STATE; /* awaiting explicit confirmation, driver not armed */
 }
 return scqs_prepare(&n->scq_session,o,role,&n->scq_profile_data,role==1U?&n->scq_admin:NULL,&n->scq_receiver,&n->scq_dispatch,&n->runtime_identity)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
#include "personal_scq_first_controller_win32.inc"
#include "personal_scq_cold_acquisition_win32.inc"
#include "personal_scq_cold_start_win32.inc"
#include "personal_machine_fixture_source_win32.inc"
PAPACC_RESULT papacc_configuration_personal_scq_session_take_data(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_RUNTIME_OBSERVATION r;const PAPACC_SCQ_PEER_DATA *peer=NULL;
 if(!personal_scq_session_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;n=&o->pending_commit.personal_native;
 if(scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p))return PAPACC_RESULT_INVALID_STATE;
 if(!n->scq_profile_data.waiting_runtime){if(papacc_configuration_personal_scq_observe_image_data(o,&r)!=PAPACC_RESULT_OK||r.known!=PAPACC_SCQ_OBS_COMPLETE)return PAPACC_RESULT_INVALID_STATE;peer=&r.peer;}
 return scqs_take(&n->scq_session,o,&p.administrator,peer,p.refs,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
#include "personal_scq_offer_data_win32.inc"
PAPACC_RESULT papacc_configuration_personal_scq_session_receive_data(PAPACC_CONFIGURATION_COMMIT *o,const BYTE *frame,DWORD bytes)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_SCQ_ORIGINAL *proof;PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_RUNTIME_OBSERVATION r;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 proof=personal_scq_native_original(o);if(!proof){personal_scq_session_native_failed(o,ERROR_NOT_FOUND);return PAPACC_RESULT_NOT_SUPPORTED;}n=&o->pending_commit.personal_native;
 if(scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p)||papacc_configuration_personal_scq_observe_image_data(o,&r)!=PAPACC_RESULT_OK||r.known!=PAPACC_SCQ_OBS_COMPLETE)return PAPACC_RESULT_INVALID_STATE;
 return scqs_receive(&n->scq_session,o,&r.peer,&p.administrator,p.refs,frame,bytes,proof,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_session_receipt_data(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_RUNTIME_OBSERVATION r;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 if(!personal_scq_native_original(o)){personal_scq_session_native_failed(o,ERROR_NOT_FOUND);return PAPACC_RESULT_NOT_SUPPORTED;}n=&o->pending_commit.personal_native;
 if(scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p)||papacc_configuration_personal_scq_observe_image_data(o,&r)!=PAPACC_RESULT_OK||r.known!=PAPACC_SCQ_OBS_COMPLETE)return PAPACC_RESULT_INVALID_STATE;
 return scqs_receipt(&n->scq_session,o,&r.peer,&p.administrator,p.refs,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_session_ack_data(PAPACC_CONFIGURATION_COMMIT *o,const BYTE *receipt,DWORD bytes)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_RUNTIME_OBSERVATION r;
 if(!personal_scq_session_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;n=&o->pending_commit.personal_native;
 if(scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p)||papacc_configuration_personal_scq_observe_image_data(o,&r)!=PAPACC_RESULT_OK||r.known!=PAPACC_SCQ_OBS_COMPLETE)return PAPACC_RESULT_INVALID_STATE;
 return scqs_ack(&n->scq_session,o,&p.administrator,&r.peer,p.refs,receipt,bytes,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
const BYTE *papacc_configuration_personal_scq_session_output_data(PAPACC_CONFIGURATION_COMMIT *o,DWORD *bytes)
{
 PAPACC_SCQ_SESSION_DRIVER *c;if(!bytes)return NULL;*bytes=0;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop)return NULL;
 c=&o->pending_commit.personal_native.scq_session;
 if(!scqs_owned(c,o)||(c->phase!=3U&&c->phase!=4U)||!c->bytes||c->transport.pending||c->transport.operation||c->transport.total)return NULL;
 *bytes=c->bytes;return c->buffer; /* local nonsecret buffer, no authority */
}
/* No endpoint factory activation/registration is provided in this offline slice.
 * Only resources already held by this same packet could be polled/cancelled;
 * caller cannot inject HANDLEs, a remote pointer or an approval. */
/* Receiver origin acquisition has no personal_token/admin fallback. It
 * validates the actual SCM virtual process through its existing observer. */
PAPACC_RESULT papacc_configuration_personal_scq_carrier_capture(PAPACC_CONFIGURATION_COMMIT *o)
{
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PS_SCQ_CARRIER value;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->initialized||o->published||
  !papacc_server_service_scq_carrier(&value))return PAPACC_RESULT_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;if(n->scq_carrier_captured)return PAPACC_RESULT_INVALID_STATE;
 n->scq_carrier=value;n->scq_carrier_captured=1U;
 return PAPACC_RESULT_OK; /* no reader, source, registration, IO or permission */
#else
 (void)o;return PAPACC_RESULT_NOT_SUPPORTED;
#endif
}
PAPACC_RESULT papacc_configuration_personal_scq_recipient_prepare(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_PLAN_DATA *plan,const HANDLE originals[3],HANDLE controller)
{
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;const PAPACC_SCQ_OWNER_DECISION *registered=personal_scq_registered_owner_decision(o);
 PAPACC_SCQ_RUNTIME_OBSERVATION observed;WCHAR key[112];DWORD e;unsigned i;
 if(!registered||!o||o->writer!=GetCurrentThreadId())return PAPACC_RESULT_NOT_SUPPORTED;
 if(!plan||!originals||!controller||!scqa_scope(registered,plan)||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread||
 o->pending_commit.personal_ca||o->pending_commit.personal_ca_candidate||
 !(plan->runtime.service_state==SERVICE_START_PENDING?papacc_server_service_scq_preparation_current():papacc_server_service_scq_host_current()))return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 if(plan->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&plan->runtime.service_state==SERVICE_START_PENDING&&
  (!n->scq_carrier_captured||!scqa_carrier_scope(&n->scq_carrier,plan)))return PAPACC_RESULT_INVALID_STATE;
 if(n->scq_recipient.phase||n->runtime_identity.phase||n->scq_inputs.phase||n->scq_profile_data.phase||n->scq_session.phase)return PAPACC_RESULT_INVALID_STATE;
 if(plan->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&!n->install_paths.phase&&
  papacc_personal_paths_new(&n->install_paths,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 e=plan->runtime.service_state==SERVICE_START_PENDING?(plan->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?papacc_personal_runtime_capture_transport_closed:papacc_personal_runtime_capture_closed)(&n->runtime_identity,GetCurrentProcess(),(PSID)plan->runtime.sid,plan->runtime.service_sid_type):papacc_personal_runtime_capture(&n->runtime_identity,GetCurrentProcess(),(PSID)plan->runtime.sid,PAPACC_PERSONAL_RUNTIME_VIRTUAL,plan->runtime.service_sid_type);if(e)goto failed;
 if(n->runtime_identity.pid!=GetCurrentProcessId()){e=ERROR_ACCESS_DENIED;goto failed;}
 e=plan->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?
  personal_scq_inputs_capture_transport(&n->scq_inputs,originals,(PSID)plan->administrator.sid,plan,&n->install_paths,o,&o->pending_commit.stop):
  personal_scq_inputs_capture(&n->scq_inputs,originals,(PSID)plan->administrator.sid,&o->pending_commit.stop);if(e)goto failed;
 e=personal_scq_inputs_contents(&n->scq_inputs,(PSID)plan->administrator.sid,plan->refs,((ULONGLONG)plan->valid_until[1]<<32)|plan->valid_until[0],&o->pending_commit.stop);if(e)goto failed;
 e=papacc_personal_runtime_observe_data(&n->runtime_identity,&observed,&o->pending_commit.stop);if(e)goto failed;
 e=personal_scq_inputs_image_data(&n->scq_inputs,(PSID)plan->administrator.sid,&n->runtime_identity,&observed,&o->pending_commit.stop);if(e)goto failed;
 if(observed.known!=PAPACC_SCQ_OBS_COMPLETE||memcmp(&observed.peer,&plan->runtime,sizeof(observed.peer))){e=ERROR_ACCESS_DENIED;goto failed;}
 if(plan->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&plan->runtime.service_state==SERVICE_START_PENDING){
  e=scqr_first_prepare(&n->scq_recipient,&n->scq_session,o,registered,&n->scq_profile_data,&n->scq_inputs,&n->runtime_identity,
   &n->scq_dispatch,&n->scq_receiver,plan,controller,&o->pending_commit.stop);if(e)goto failed;
  return PAPACC_RESULT_OK;
 }
 for(i=0;i<112;i++)key[i]=(WCHAR)(BYTE)plan->key[i];
 e=plan->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?papacc_personal_scq_prepare_transport(&n->scq_dispatch,&n->runtime_identity,plan->attempt,key):papacc_personal_scq_prepare(&n->scq_dispatch,&n->runtime_identity,plan->attempt,key);if(e)goto failed;
 e=plan->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?papacc_scq_profile_prepare_transport_data(&n->scq_profile_data,o,2U,plan,&observed.peer):n->runtime_identity.closed_preparation?papacc_scq_profile_prepare_closed_data(&n->scq_profile_data,o,2U,plan,&observed.peer):papacc_scq_profile_prepare_data(&n->scq_profile_data,o,2U,plan,&observed.peer);if(e)goto failed;
 e=scqr_acquire(&n->scq_recipient,o,registered,&n->scq_profile_data,&n->scq_inputs,&n->runtime_identity,&n->scq_dispatch,controller,&o->pending_commit.stop);if(e)goto failed;
 e=scqs_prepare(&n->scq_session,o,2U,&n->scq_profile_data,NULL,&n->scq_receiver,&n->scq_dispatch,&n->runtime_identity);if(e)goto failed;
 return PAPACC_RESULT_OK;
failed:
 if(!n->error)n->error=e;o->unresolved=1;n->scq_recipient.phase=4U;n->scq_recipient.value.retained=1;return PAPACC_RESULT_INVALID_STATE;
#else
 (void)o;(void)plan;(void)originals;(void)controller;return PAPACC_RESULT_NOT_SUPPORTED;
#endif
}
PAPACC_RESULT papacc_configuration_personal_scq_recipient_acquire(PAPACC_CONFIGURATION_COMMIT *o,HANDLE controller)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;const PAPACC_SCQ_OWNER_DECISION *registration=personal_scq_registered_owner_decision(o);
 if(!o||o->writer!=GetCurrentThreadId()||!registration)return PAPACC_RESULT_NOT_SUPPORTED;
 if(o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->initialized||o->published)return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 if(n->runtime_identity.pid!=GetCurrentProcessId())return PAPACC_RESULT_INVALID_STATE;
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
 if(n->scq_profile_data.pre_readiness?!papacc_server_service_scq_preparation_current():!papacc_server_service_scq_host_current())return PAPACC_RESULT_INVALID_STATE;
 return scqr_acquire(&n->scq_recipient,o,registration,&n->scq_profile_data,&n->scq_inputs,&n->runtime_identity,&n->scq_dispatch,controller,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
#else
 (void)controller;return PAPACC_RESULT_NOT_SUPPORTED;
#endif
}
DWORD papacc_configuration_personal_scq_transport_enter(PAPACC_CONFIGURATION_COMMIT *o,DWORD stage,const WCHAR *name)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_PERSONAL_SCQ_ORIGINAL *original;
 const PAPACC_SCQ_OWNER_DECISION *endpoint=personal_scq_registered_endpoint_decision(o);
 if(!endpoint||endpoint!=personal_scq_registered_owner_decision(o)||!personal_scq_session_native_current(o))return ERROR_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;if(scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p))return ERROR_INVALID_DATA;
 if(n->scq_session.role==2U){
  if(stage==0U)return scqr_open(&n->scq_recipient,&n->scq_session,o,&n->scq_inputs,name,&p.runtime,&p.administrator,p.refs,&o->pending_commit.stop);
  if(stage==1U)return scqr_admit_offer(&n->scq_recipient,&n->scq_session,o,&n->scq_inputs,&p.runtime,&p.administrator,p.refs,&o->pending_commit.stop);
  return ERROR_INVALID_PARAMETER;
 }
 if(stage==0U&&scqa_closed_diagnostic_scope(&n->scq_admin,&p)){
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
  PAPACC_SERVICE_DIAGNOSTIC_START_REPORT resources;
  (void)papacc_server_service_diagnostic_start_status(&resources);
  /* Original endpoint confirmation is not native resource admission. Do not
   * consume it or enter a provider while TO7 terms remain unknown. */
  if(!resources.limited_eligible)return ERROR_NOT_SUPPORTED;
  if(!n->scq_profile_data.waiting_runtime)return ERROR_ACCESS_DENIED;
  if(scqa_endpoint_take(&n->scq_admin,o,&o->pending_commit.stop))return ERROR_ACCESS_DENIED;
  n->scq_session.transport.resource_scope=&n->scq_start_assurance;
  n->scq_session.transport.global_until=n->scq_start_assurance.global_until;
  return scqt_reserve(&n->scq_session,o,&o->pending_commit.stop);
#else
  return ERROR_NOT_SUPPORTED;
#endif
 }
 if(n->scq_session.role==1U&&scqa_closed_diagnostic_scope(&n->scq_admin,&p)&&
  (n->scq_admin.endpoint_phase!=3U||n->scq_admin.endpoint_consumed!=1))return ERROR_ACCESS_DENIED;
 original=personal_scq_native_original(o);if(!original)return ERROR_NOT_SUPPORTED;
 if(stage==0U){
  return scqt_create(&n->scq_session,o,&p.administrator,&p.runtime,p.refs,original,&o->pending_commit.stop);
 }
 if(stage==1U)return scqt_connect(&n->scq_session,o,&p.administrator,&p.runtime,p.refs,original,&o->pending_commit.stop);
 if(stage==2U)return scqt_connected(&n->scq_session,o,&p.administrator,&p.runtime,p.refs,original,&o->pending_commit.stop);
 return ERROR_INVALID_PARAMETER;
}
DWORD papacc_configuration_personal_scq_transport_references(PAPACC_CONFIGURATION_COMMIT *o,HANDLE local_owned[3])
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_PERSONAL_SCQ_ORIGINAL *original;DWORD e;
 if(!personal_scq_registered_endpoint_decision(o)||personal_scq_registered_endpoint_decision(o)!=personal_scq_registered_owner_decision(o)||!personal_scq_session_native_current(o))return ERROR_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;original=personal_scq_native_original(o);
 if(!original||scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p))return ERROR_NOT_SUPPORTED;
 if(n->scq_session.role==1U){if(local_owned)return ERROR_INVALID_PARAMETER;
  if(scqa_closed_diagnostic_scope(&n->scq_admin,&p)&&(n->scq_admin.endpoint_phase!=3U||n->scq_admin.endpoint_consumed!=1))return ERROR_ACCESS_DENIED;
  return scqt_deliver_refs(&n->scq_session,o,&p.administrator,&p.runtime,p.refs,original,&o->pending_commit.stop);}
 if(!local_owned)return ERROR_INVALID_PARAMETER;
 e=scqt_accept_refs(&n->scq_session,o,local_owned,&p.runtime,&p.administrator,p.refs,original,&o->pending_commit.stop);if(e)return e;
 return scqt_adopt_delivery(&n->scq_session,o,local_owned,&p.runtime,&p.administrator,p.refs,original,&o->pending_commit.stop);
}
DWORD papacc_configuration_personal_scq_transport_data(PAPACC_CONFIGURATION_COMMIT *o,DWORD operation,int apply)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_PERSONAL_SCQ_ORIGINAL *original;
 const PAPACC_SCQ_PEER_DATA *local,*remote;
 if(!personal_scq_session_native_current(o))return ERROR_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;
 /* No caller can install endpoint refs; only a separately governed future
  * original-endpoint acquisition can make this context live. */
 if(!n->scq_session.transport.phase||scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p))return ERROR_NOT_SUPPORTED;
 original=personal_scq_native_original(o);
 if(!original&&n->scq_session.role==2U&&n->scq_recipient.phase==1U&&n->scq_session.transport.query_only&&!apply&&operation==2U)original=&n->scq_recipient.value;
 if(!original)return ERROR_NOT_SUPPORTED;
 local=n->scq_session.role==1U?&p.administrator:&p.runtime;remote=n->scq_session.role==1U?&p.runtime:&p.administrator;
 return apply?(operation==4U||operation==8U||operation==16U||operation==32U?scqt_apply_close(&n->scq_session,o,operation,local,remote,p.refs,original,&o->pending_commit.stop):scqt_apply(&n->scq_session,o,operation,local,remote,p.refs,original,&o->pending_commit.stop)):
 scqt_start_io(&n->scq_session,o,operation,local,remote,p.refs,original,&o->pending_commit.stop);
}
DWORD papacc_configuration_personal_scq_transport_produce_close(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_PERSONAL_SCQ_ORIGINAL *original;
 if(!personal_scq_session_native_current(o))return ERROR_NOT_SUPPORTED;n=&o->pending_commit.personal_native;
 original=personal_scq_native_original(o);if(!original||scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p))return ERROR_NOT_SUPPORTED;
 return scqt_produce_close(&n->scq_session,o,&p.runtime,&p.administrator,p.refs,original,&o->pending_commit.stop);
}
DWORD papacc_configuration_personal_scq_transport_poll(PAPACC_CONFIGURATION_COMMIT *o)
{
 DWORD e;if(!o||o->writer!=GetCurrentThreadId()||!personal_scq_registered_owner_decision(o))return ERROR_NOT_SUPPORTED;
 e=scqt_poll(&o->pending_commit.personal_native.scq_session,o);
 return e;
}
/* The same packet and registrations gate the bounded continuation. No DATA
 * policy/boolean installs authority and no endpoint is constructed here. */
#include "personal_scq_machine_read_continue_win32.inc"
#include "personal_scq_transport_continue_win32.inc"
static DWORD personal_scq_query_resources(PAPACC_CONFIGURATION_COMMIT *o,int creating)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;PAPACC_SCQ_START_ASSURANCE *g=&n->scq_start_assurance;
 LARGE_INTEGER now;ULONGLONG tick=GetTickCount64(),until,original;DWORD e;
 until=((ULONGLONG)n->scq_carrier.global_until[1]<<32)|n->scq_carrier.global_until[0];
 original=((ULONGLONG)n->scq_carrier.until[1]<<32)|n->scq_carrier.until[0];
 if(!until||until>original||until<=tick)return ERROR_TIMEOUT;
 e=personal_resource_collect(&g->observations,creating?1U:2U,0);if(e)return scqg_fail(g,e);
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0)return ERROR_INVALID_DATA;
 if(!g->phase){
  if(!n->runtime_identity.closed_preparation||!n->scq_carrier_captured)return ERROR_ACCESS_DENIED;
  {DWORD sampled=scqg_query_ready_mode(g,o,&g->observations.last,(ULONGLONG)now.QuadPart,tick,until,&o->pending_commit.stop,1);if(!sampled)g->read_diagnostic=n->runtime_identity.read_diagnostic;return sampled;}
 }
 if(g->global_until!=until)return ERROR_ACCESS_DENIED;
 return scqg_query_admit(g,o,&g->observations.last,(ULONGLONG)now.QuadPart,GetTickCount64(),creating,&o->pending_commit.stop);
}
/* Internal wait predicate, not an approval/source getter. */
#include "personal_scq_ingress_win32.inc"
#include "personal_scq_start_progress_win32.inc"
PAPACC_RESULT papacc_configuration_personal_scq_start_namespace_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_SESSION_DRIVER *c;DWORD flags;
 const PAPACC_SCQ_OWNER_DECISION *endpoint=personal_scq_registered_endpoint_decision(o);
 if(!endpoint||endpoint!=personal_scq_registered_owner_decision(o)||!personal_scq_admin_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;c=&n->scq_session;
 if(!scqa_startup_original(&n->scq_admin,o,&o->pending_commit.stop)||n->scq_admin.start_phase!=2U||n->scq_admin.start_consumed||
  n->scq_admin.endpoint_phase!=3U||n->scq_admin.endpoint_consumed!=1||
  !scqt_owned(c,o)||c->role!=1U||c->phase!=2U||c->transport.phase!=2U||c->transport.original!=&n->scq_admin.value||
  !c->transport.pipe||!c->transport.event||GetFileType(c->transport.pipe)!=FILE_TYPE_PIPE||
  !GetHandleInformation(c->transport.pipe,&flags)||(flags&HANDLE_FLAG_INHERIT)||
  !GetHandleInformation(c->transport.event,&flags)||(flags&HANDLE_FLAG_INHERIT))return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK; /* original namespace only, no runtime/ready claim */
}
DWORD papacc_configuration_personal_scq_transport_stop(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!o||o->writer!=GetCurrentThreadId()||!o->pending_commit.personal_native.scq_session.transport.phase)return ERROR_NOT_SUPPORTED;
 (void)scqt_stop(&o->pending_commit.personal_native.scq_session,o);return ERROR_CANCELLED;
}
PAPACC_RESULT papacc_configuration_personal_scq_session_peer_lost(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!o||o->writer!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 (void)scqs_peer_lost(&o->pending_commit.personal_native.scq_session,o);return PAPACC_RESULT_INVALID_STATE;
}
static int scm_registration_files_close(PAPACC_PERSONAL_NATIVE_CONTEXT *);
#include "server_configuration_personal_close_win32.inc"
static PAPACC_RESULT personal_packet_source(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;BY_HANDLE_FILE_INFORMATION f;LARGE_INTEGER zero;
 const unsigned char *configuration=p->personal_data.plan;size_t bytes=p->personal_data.bytes;
 if(InterlockedCompareExchange(&p->stop,0,0))return PAPACC_RESULT_INVALID_STATE;
 if(p->personal_data.effectful&&!papacc_personal_effect_source(p->personal_data.plan,p->personal_data.bytes,&configuration,&bytes))return PAPACC_RESULT_INVALID_STATE;
 n=&p->personal_native;zero.QuadPart=0;
 if(n->phase!=1U||!p->file||p->file==INVALID_HANDLE_VALUE||!GetFileInformationByHandle(p->file,&f)||
 f.dwVolumeSerialNumber!=n->source.dwVolumeSerialNumber||f.nFileIndexHigh!=n->source.nFileIndexHigh||f.nFileIndexLow!=n->source.nFileIndexLow||
 f.nFileSizeHigh||f.nFileSizeLow!=bytes||f.nFileSizeLow!=n->source.nFileSizeLow||
 !SetFilePointerEx(p->file,zero,NULL,FILE_BEGIN)||pending_compare_file(p,configuration,f.nFileSizeLow)!=1)return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_source_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 return personal_packet_source(&o->pending_commit);
}
#include "server_configuration_first_prepare_win32.inc"
#include "server_configuration_prepared_validate_win32.inc"
/* Same pending_worker dispatch. All inputs belong to the frozen packet. Never
 * reads owner-only DATA phase/consumed while Stop may invalidate that phase.
 * Source bytes/attempt/native refs remain owned until factual quiescence. */
static void personal_handoff_worker(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 LONG result=PAPACC_PERSONAL_HANDOFF_DENIED;
 if(InterlockedCompareExchange(&n->handoff_state,0,0)!=PAPACC_PERSONAL_HANDOFF_SUBMITTED)return;
 if(!InterlockedCompareExchange(&p->stop,0,0)&&
    !memcmp(n->handoff_attempt,p->personal_data.attempt,32U)&&
    n->plan_leaf[0]&&memchr(n->plan_leaf,0,sizeof(n->plan_leaf))&&
    !memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf))&&
    personal_packet_current(p)==PAPACC_RESULT_OK&&
    personal_packet_source(p)==PAPACC_RESULT_OK&&
    !InterlockedCompareExchange(&p->stop,0,0))result=PAPACC_PERSONAL_HANDOFF_VERIFIED;
 if(n->phase==2U||p->close_error){result=PAPACC_PERSONAL_HANDOFF_RETAINED;p->result=PAPACC_CAS_UNKNOWN;}
 /* No FILE_CREATE, owner lookup, key borrow, commit or publication here.
  * Existing child creation remains separately denied by its native gate. */
 InterlockedExchange(&n->handoff_state,result);
}

/* Effect-capable sibling dispatch in the SAME worker. The current read-only
 * ceremony never authorizes it; the production gate is independently checked.
 * LAB93 tests mechanics only, never an eligible actor or administrative grant. */
#include "server_configuration_personal_target_worker_win32.inc"
/* SCM LAB files are opened relative to the SAME protected parent. No creation,
 * NEW_INSTALL confirmation, CA reference or administrative grant is inferred. */
static int scm_leaf(const WCHAR *s)
{
 unsigned i;if(!s||!s[0])return 0;
 for(i=0;s[i];i++)if(i>=64U||!((s[i]>=L'a'&&s[i]<=L'z')||(s[i]>=L'A'&&s[i]<=L'Z')||(s[i]>=L'0'&&s[i]<=L'9')||s[i]==L'.'||s[i]==L'_'||s[i]==L'-'))return 0;
 return wcscmp(s,L".")&&wcscmp(s,L"..");
}
static PAPACC_RESULT scm_open_relative(PAPACC_CONFIG_PENDING *p,const WCHAR *leaf,HANDLE *handle)
{
 HMODULE mod;FARPROC proc;BOOTSTRAP_NT_CREATE create;UNICODE_STRING name;OBJECT_ATTRIBUTES a;IO_STATUS_BLOCK io;
 if(!scm_leaf(leaf)||(*handle&&*handle!=INVALID_HANDLE_VALUE))return PAPACC_RESULT_INVALID_ARGUMENT;
 mod=GetModuleHandleA("ntdll.dll");if(!mod)return PAPACC_RESULT_NOT_SUPPORTED;proc=GetProcAddress(mod,"NtCreateFile");if(!proc)return PAPACC_RESULT_NOT_SUPPORTED;memcpy(&create,&proc,sizeof(create));
 memset(&a,0,sizeof(a));memset(&io,0,sizeof(io));name.Buffer=(WCHAR *)leaf;name.Length=(USHORT)(wcslen(leaf)*sizeof(WCHAR));name.MaximumLength=name.Length;
 a.Length=sizeof(a);a.RootDirectory=p->bootstrap_parent;a.ObjectName=&name;a.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 if(create(handle,GENERIC_READ|READ_CONTROL|SYNCHRONIZE,&a,&io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1U,0x00200060U,NULL,0U)<0)return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK;
}
#include "server_scm_registration_files_win32.inc"
PAPACC_RESULT papacc_configuration_scm_lab_files_open(PAPACC_CONFIGURATION_COMMIT *o,const WCHAR *image,const char *configuration)
{
 PAPACC_CONFIG_PENDING *p;WCHAR leaf[65];unsigned i;DWORD count;
 if(!o||!configuration||papacc_configuration_personal_native_current(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;if(p->file!=INVALID_HANDLE_VALUE||p->personal_native.scm_image)return PAPACC_RESULT_INVALID_STATE;
 for(i=0;configuration[i];i++){if(i>=64U||(unsigned char)configuration[i]>126U)return PAPACC_RESULT_INVALID_ARGUMENT;leaf[i]=(WCHAR)(unsigned char)configuration[i];}leaf[i]=0;
 if(scm_open_relative(p,image,&p->personal_native.scm_image)!=PAPACC_RESULT_OK||scm_open_relative(p,leaf,&p->file)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 if(!GetFileInformationByHandle(p->file,&p->personal_native.source)||(p->personal_native.source.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||
 p->personal_native.source.nFileSizeHigh||!p->personal_native.source.nFileSizeLow||p->personal_native.source.nFileSizeLow>8192U)return PAPACC_RESULT_INVALID_STATE;
 if(!ReadFile(p->file,p->next,8193U,&count,NULL)||count!=p->personal_native.source.nFileSizeLow)return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_scm_lab_files_current(PAPACC_CONFIGURATION_COMMIT *o,const BY_HANDLE_FILE_INFORMATION *image)
{
 PAPACC_CONFIG_PENDING *p;BY_HANDLE_FILE_INFORMATION f;LARGE_INTEGER zero;
 if(!o||!image||papacc_configuration_personal_native_current(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;if(!p->personal_native.scm_image||p->file==INVALID_HANDLE_VALUE)return PAPACC_RESULT_INVALID_STATE;
 if(!GetFileInformationByHandle(p->personal_native.scm_image,&f)||f.dwVolumeSerialNumber!=image->dwVolumeSerialNumber||
 f.nFileIndexHigh!=image->nFileIndexHigh||f.nFileIndexLow!=image->nFileIndexLow||f.nFileSizeHigh!=image->nFileSizeHigh||f.nFileSizeLow!=image->nFileSizeLow||
 memcmp(&f.ftLastWriteTime,&image->ftLastWriteTime,sizeof(FILETIME))||(f.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))return PAPACC_RESULT_INVALID_STATE;
 if(!GetFileInformationByHandle(p->file,&f)||f.dwVolumeSerialNumber!=p->personal_native.source.dwVolumeSerialNumber||f.nFileIndexHigh!=p->personal_native.source.nFileIndexHigh||
 f.nFileIndexLow!=p->personal_native.source.nFileIndexLow||f.nFileSizeHigh||f.nFileSizeLow!=p->personal_native.source.nFileSizeLow||
 (f.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))return PAPACC_RESULT_INVALID_STATE;
 zero.QuadPart=0;
 if(!SetFilePointerEx(p->file,zero,NULL,FILE_BEGIN)||pending_compare_file(p,p->next,f.nFileSizeLow)!=1)return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK;
}
