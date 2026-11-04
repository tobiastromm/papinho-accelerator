/* Actual initial QUERY constructor, native candidate checks, local scope
 * freeze, admission and reciprocal close. All protected APIs are MODELED. */
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <wchar.h>
static volatile LONG *query_stop_ptr;
static unsigned query_fault;static int destination_override;static DWORD destination_count;
static BOOL WINAPI query_frequency(LARGE_INTEGER *v){v->QuadPart=1000000;return TRUE;}
#define QueryPerformanceFrequency query_frequency
static ULONGLONG query_qpc=1010U;
static BOOL WINAPI query_counter(LARGE_INTEGER *v){v->QuadPart=(LONGLONG)query_qpc;return TRUE;}
static BOOL WINAPI query_handles(HANDLE p,PDWORD n){(void)p;if(destination_override){*n=destination_count;return destination_override==1;}*n=query_fault==4U?124U:query_fault==99U?188U:100U;return TRUE;}
static BOOL WINAPI query_process_image(HANDLE p,DWORD flags,LPWSTR out,PDWORD n)
{static const WCHAR name[]=L"\\Device\\Model\\papacc_server.exe";unsigned len=(unsigned)wcslen(name);(void)p;(void)flags;if(*n<=len)return FALSE;memcpy(out,name,sizeof(name));*n=len;return TRUE;}
static DWORD WINAPI query_file_image(HANDLE h,LPWSTR out,DWORD n,DWORD flags)
{DWORD bytes=n;(void)h;(void)flags;if(!query_process_image(NULL,0,out,&bytes))return 0;if(query_fault==8U)out[3]^=1;return bytes;}
static void query_copy_stale(void){query_qpc=101001U;}
static void query_copy_stop(void){if(query_stop_ptr)*query_stop_ptr=1;}
static void query_copy_guard(void){query_fault=99U;}
#define GetProcessHandleCount query_handles
#define QueryPerformanceCounter query_counter
#define QueryFullProcessImageNameW query_process_image
#define GetFinalPathNameByHandleW query_file_image
#define PAPACC_SCQ_REPORT_ORIGINAL_MODEL 1
struct PS_SCQ_CARRIER;
static DWORD query_report_carrier(struct PS_SCQ_CARRIER *);
static DWORD query_report_namespace(void);
static DWORD query_report_resource(void);
#define PAPACC_BOOTSTRAP_CAPTURE_ENTRY preserved_bootstrap_capture_main
#include "scq_bootstrap_capture_test.c"
#include "../apps/server/personal_scq_start_assurance_win32.inc"
static CAPTURE_FIXTURE qc[71];static PAPACC_SCQ_START_ASSURANCE qg[71],qr[71];
/* Test-only bridges to ACTUAL original actors, never installed in product.
 * Native acquisition/identity/resource API boundaries remain modeled. */
static SESSION_FIXTURE *report_fixture;static PAPACC_SCQ_START_ASSURANCE *report_scope;
static DWORD report_resource_error;
static DWORD query_report_carrier(PS_SCQ_CARRIER *out)
{return report_fixture?scqa_start_carrier(&report_fixture->source,&report_fixture->administrator_owner,out,&report_fixture->stop):ERROR_NOT_FOUND;}
static DWORD query_report_resource(void)
{
 PAPACC_SCM_RESOURCE_SAMPLE sample;
 if(report_resource_error)return report_resource_error;
 if(!report_fixture||!report_scope||!scqg_selected_exact_mode(&report_scope->limits,240000U,report_scope->p1_handles)||
 !report_scope->global_until||receiver_now>=report_scope->global_until)return ERROR_ACCESS_DENIED;
 sample=report_scope->ready;sample.begin=sample.end+1U;sample.end=sample.begin+1U;
 return scqg_admit(report_scope,&report_fixture->administrator_owner,&sample,sample.end,1,&report_fixture->stop);
}
static DWORD query_report_namespace(void)
{
 if(!report_fixture||query_report_resource()||!scqt_owned(&report_fixture->controller,&report_fixture->administrator_owner)||
 report_fixture->controller.transport.phase!=2U||!report_fixture->controller.transport.pipe||!report_fixture->controller.transport.event||
 report_fixture->controller.transport.original!=&report_fixture->source.value||
 !report_fixture->source.value.controller_consumed||!report_fixture->source.endpoint_consumed||report_fixture->source.start_consumed)return ERROR_ACCESS_DENIED;
 return scqa_startup_original(&report_fixture->source,&report_fixture->administrator_owner,&report_fixture->stop)?0:ERROR_ACCESS_DENIED;
}
static DWORD query_confirm(PAPACC_SCQ_ADMIN_SOURCE *s,SESSION_FIXTURE *f,unsigned kind)
{
 const char *prefix=kind==0U?"OWNER_SCQ ":kind==1U?"OWNER_SCQ_PRIVATE_ENDPOINT ":"OWNER_SCQ_DIAGNOSTIC_START ";
 char reply[100];size_t n=strlen(prefix);unsigned j;memcpy(reply,prefix,n);
 for(j=0;j<32U;j++){reply[n+2U*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[n+2U*j+1U]="0123456789abcdef"[f->plan.attempt[j]&15U];}
 if(!kind)return scqa_confirm(s,&f->administrator_owner,reply,n+64U,&f->stop);
 if(kind==1U)return scqa_endpoint_confirm(s,&f->administrator_owner,reply,n+64U,&f->stop);
 return scqa_start_confirm(s,&f->administrator_owner,reply,n+64U,&f->stop);
}
#ifndef PAPACC_QUERY_COLD_ENTRY
#define PAPACC_QUERY_COLD_ENTRY main
#endif
int PAPACC_QUERY_COLD_ENTRY(void)
{
 unsigned i,j,before=checks,complete=0;DWORD e;PAPACC_SCQ_PLAN_DATA controller_plan,p;
 C(PS_SCQ_CARRIER_BYTES==308U&&PAPACC_SCQ_RECORD_BYTES==1048U&&PAPACC_SCQ_RECEIPT_BYTES==1052U&&PAPACC_SCQ_CLOSE_BYTES==1076U&&PAPACC_SCQ_BOOTSTRAP_BYTES==1084U);
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<71U;i++){
  CAPTURE_FIXTURE *b=&qc[i];SESSION_FIXTURE *f=&b->f;PAPACC_SCQ_START_LIMITS limits;PAPACC_SERVICE_DIAGNOSTIC_START_REPORT start_report;
  PAPACC_SCM_RESOURCE_SAMPLE sample={1000000U,1000U,1001U,1000000U,2000000U,8000000000ULL,11000000000ULL,100U,1U};
  PAPACC_PERSONAL_INSTALL_PATHS paths;memset(&paths,0,sizeof(paths));
  mcase=mcreated=mopened=mevents=mposition=mincomplete=mcancel=capture_fault=query_fault=origin_fault=mt_query_fault=mt_admin_reopens=0;receiver_identity_alias=1U;query_qpc=1010U;mt_local_dup_hook=NULL;query_stop_ptr=&f->stop;receiver_admin_copy_model=i==0U||i>=34U;receiver_admin_closed_mask=0;
  C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
  report_fixture=NULL;report_scope=NULL;report_resource_error=0;
  C(papacc_server_service_diagnostic_start_status(&start_report)!=0&&start_report.initial_receiver_source_unavailable);
  C(start_report.original_checked&&start_report.original_error==ERROR_NOT_FOUND&&!start_report.resource_checked&&!start_report.resource_error);
  report_fixture=f;report_scope=&qg[i];
  /* Reacquire fresh controller source with the selected original window.
   * The recipient selection remains ZERO until actual QUERY acquisition. */
  memset(&f->source,0,sizeof(f->source));memset(&f->controller_profile,0,sizeof(f->controller_profile));
  f->plan.valid_from[0]=100U;f->plan.valid_from[1]=0;f->plan.valid_until[0]=240100U;f->plan.valid_until[1]=0;
  f->plan.administrator.image[0]=10;f->plan.administrator.image[1]=0;f->plan.administrator.image[2]=43;memcpy(f->plan.administrator.image_digest,f->plan.refs[2].digest,32);
  controller_plan=f->plan;memset(&controller_plan.runtime,0,sizeof(controller_plan.runtime));controller_plan.runtime.sid_bytes=f->plan.runtime.sid_bytes;memcpy(controller_plan.runtime.sid,f->plan.runtime.sid,68);
  C(!papacc_scq_profile_prepare_transport_data(&f->controller_profile,&f->administrator_owner,1U,&controller_plan,&controller_plan.administrator));
  registration(&f->policy,&controller_plan);f->policy.controller=&f->administrator_owner;f->policy.source=&f->source;
  f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
  f->original.deadline=240100U;f->original.recipient=&f->administrator_owner;
  C(!scqa_capture(&f->source,&f->administrator_owner,&f->policy,&f->controller_profile,&f->original,&f->stop));
  if((i==0U||i>=34U))scqg_selected_p1(&limits);else scqg_selected(&limits);C(scqg_selected_exact_mode(&limits,240000U,(i==0U||i>=34U)));
  C(!scqg_capture_mode(&qg[i],&f->administrator_owner,&f->source,&limits,&f->stop,(i==0U||i>=34U)));if((i==0U||i>=34U)){qg[i].initial_known=1U;qg[i].initial_handles=128U;}C(!scqg_presented(&qg[i],&f->administrator_owner,&f->stop));
  C(!scqa_present(&f->source,&f->administrator_owner,&f->stop));C(!query_confirm(&f->source,f,0));
  C(!scqa_endpoint_present(&f->source,&f->administrator_owner,&f->stop));C(!query_confirm(&f->source,f,1));
  C(!scqa_start_present(&f->source,&f->administrator_owner,&f->stop));C(!query_confirm(&f->source,f,2));
  C(papacc_server_service_diagnostic_start_status(&start_report)!=0&&start_report.initial_receiver_source_unavailable&&!start_report.limited_eligible);
  receiver_now=i==18U?180100U:100U;
  C(!scqg_ready(&qg[i],&f->administrator_owner,&sample,1001U,&f->stop));C(!scqg_controller_deadline(&qg[i],receiver_now));
  qg[i].observations.last=sample;
  C(papacc_server_service_diagnostic_start_status(&start_report)!=0&&!start_report.initial_receiver_source_unavailable&&
    start_report.limited_eligible&&(start_report.missing_pre_start&PS_DIAGNOSTIC_START_NEEDS_DELIVERY_CONTINUATION));
  C(start_report.original_checked&&!start_report.original_error&&start_report.resource_checked&&!start_report.resource_error);
  if((i==0U||i>=34U)){
   static const DWORD errors[]={ERROR_NOT_ENOUGH_MEMORY,ERROR_NOT_ENOUGH_QUOTA,ERROR_INVALID_DATA,ERROR_TIMEOUT,ERROR_NOT_SUPPORTED};unsigned cause;
   for(cause=0;cause<sizeof(errors)/sizeof(errors[0]);cause++){
    report_resource_error=errors[cause];
    C(papacc_server_service_diagnostic_start_status(&start_report)==ERROR_ACCESS_DENIED);
    C(start_report.original_checked&&!start_report.original_error&&start_report.resource_checked&&start_report.resource_error==errors[cause]);
    C(start_report.initial_receiver_source_unavailable&&!start_report.limited_eligible&&start_report.resource_pre_missing==63U);
    C(!f->source.value.controller_consumed&&!f->source.endpoint_consumed&&!f->source.start_consumed);
   }
   report_resource_error=0;
  }
  C(scqg_controller_deadline(&qg[i],receiver_now+1U)!=0&&qg[i].global_until==(i==18U?240100U:60100U));
  C(!scqa_start_carrier(&f->source,&f->administrator_owner,&b->carrier,&f->stop));
  b->carrier.global_until[0]=(unsigned)qg[i].global_until;
  C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&b->controller_runtime));
  C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));
  C(!scqa_endpoint_take(&f->source,&f->administrator_owner,&f->stop));f->controller.transport.resource_scope=&qg[i];f->controller.transport.global_until=qg[i].global_until;C(!scqt_reserve(&f->controller,&f->administrator_owner,&f->stop));
  C(!papacc_server_service_diagnostic_start_status(&start_report)&&!start_report.initial_receiver_source_unavailable&&
    start_report.missing_pre_start==PS_DIAGNOSTIC_START_NEEDS_RESOURCE_PLAN&&start_report.later_obligations==15U&&start_report.resource_pre_missing==63U&&!cs_running&&
    !f->runtime.phase&&!b->controller_runtime.phase&&!f->source.value.runtime_pid);
  if(i>=27U&&i<34U){
   if(i==27U)qg[i].limits.private_growth++;
   if(i==28U)f->source.registration=NULL;
   if(i==29U)f->controller.transport.pipe=NULL;
   if(i==30U)f->source.start_phase=0;
   if(i==31U)f->source.endpoint_phase=0;
   if(i==32U)f->source.phase=2U;
   if(i==33U)receiver_now=qg[i].global_until;
   C(papacc_server_service_diagnostic_start_status(&start_report)!=0&&f->source.value.controller_consumed&&
     f->source.endpoint_consumed&&!f->source.start_consumed&&start_report.later_obligations==15U&&start_report.resource_pre_missing==63U);
   if(i==29U)C(!start_report.initial_receiver_source_unavailable&&(start_report.missing_pre_start&PS_DIAGNOSTIC_START_NEEDS_DELIVERY_CONTINUATION));
   else C(start_report.initial_receiver_source_unavailable&&!start_report.limited_eligible);
   continue;
  }
  C(!scqa_start_take(&f->source,&f->administrator_owner,&f->stop));
  runtime_service_state=SERVICE_START_PENDING;C(!observed_runtime_identity_mode(f,1U));C(!papacc_personal_runtime_capture_closed(&b->controller_runtime,(HANDLE)1,virtual_sid,1U));
  {WCHAR empty[112]={0};C(!papacc_personal_scq_prepare_transport(&f->controller_dispatch,&b->controller_runtime,f->plan.attempt,empty));}
  C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
  C(!scqg_query_ready_mode(&qr[i],&f->runtime_owner,&sample,1010U,receiver_now,qg[i].global_until,&f->stop,(i==0U||i>=34U)));qr[i].observations.last=sample;
  f->recipient.transport.resource_scope=&qr[i];
  query_fault=i;C(!b->receiver_selection.controller&&!b->receiver_selection.source&&!b->receiver_selection.controller_pid);
  if(i==2U)b->carrier.endpoint[0]^=1U;if(i==3U)b->carrier.runtime_sid[8]^=1U;if(i==10U)f->stop=1U;
  mpeer=f->plan.administrator.pid;if(i==11U)mpeer++;
  e=scqb_open_query(&f->recipient,&f->runtime_owner,i==1U?NULL:&b->carrier,&b->receiver_selection,&f->runtime,&f->runtime_profile,&f->runtime_receiver,&f->runtime_dispatch,&f->stop);
  if(i==1U||i==2U||i==3U||i==4U||i==10U||i==11U){C(e!=0&&!b->origin.phase);continue;}
  C(!e&&!b->origin.phase&&!b->receiver_selection.source&&!b->receiver_selection.controller);
  C(!scqb_initial_wait(&f->recipient,&f->runtime_owner,&f->stop));
  if(i==0U||i>=34U)C(f->recipient.transport.query_refs3&&!f->recipient.transport.capture_controller&&!f->recipient.transport.capture_token&&!mt_admin_reopens);
  if(i==34U)mt_query_fault=1U; /* Both administrative reopen APIs would DENY. */
  if(i>=35U&&i<=39U)mt_query_fault=i-33U;
  if(i>=40U&&i<=43U)mt_query_fault=i-33U;
  if(i==61U)mt_query_fault=21U;
  if(i==9U)C(scqb_open_query(&f->recipient,&f->runtime_owner,&b->carrier,&b->receiver_selection,&f->runtime,&f->runtime_profile,&f->runtime_receiver,&f->runtime_dispatch,&f->stop)!=0);
  mpeer=f->plan.runtime.pid;C(!scqt_connect(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
  C(!scqt_connected(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
  e=scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop);
  if(i>=35U&&i<=39U){C(e!=0&&f->controller.transport.phase==6U&&f->source.value.retained);continue;}C(!e);
  if(i==59U)C(scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop)!=0);
  if(i==16U)f->source.start_consumed=0;
  if(i==25U)f->controller.transport.global_until++;
  if(i==22U||i==57U)mcase=1U;if(i==23U||i==58U)mcase=2U;
  for(j=0;j<PAPACC_SCQ_BOOTSTRAP_BYTES+1U;j++){e=scqb_write(&f->controller,&f->administrator_owner,&f->stop);if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)break;}
  if(i==16U||i==25U){C(e!=0&&!b->origin.phase);continue;}C(!e);
  if(i==44U)memcpy(wire+PAPACC_SCQ_BOOTSTRAP_BYTES,wire+PAPACC_SCQ_RECEIPT_BYTES,8);
  if(i==45U)memcpy(wire+PAPACC_SCQ_BOOTSTRAP_BYTES+8U,wire+PAPACC_SCQ_BOOTSTRAP_BYTES,8);
  if(i==46U){BYTE swap[8];memcpy(swap,wire+PAPACC_SCQ_BOOTSTRAP_BYTES,8);memcpy(wire+PAPACC_SCQ_BOOTSTRAP_BYTES,wire+PAPACC_SCQ_BOOTSTRAP_BYTES+8,8);memcpy(wire+PAPACC_SCQ_BOOTSTRAP_BYTES+8,swap,8);}
  if(i==65U)memset(wire+PAPACC_SCQ_BOOTSTRAP_BYTES,255,8);
  if(i==66U)mcase=15U;
  if(i==20U)wire[PAPACC_SCQ_CLOSE_BYTES]^=1U;if(i==21U)wire[3]='1';
  mpeer=f->plan.administrator.pid;mposition=0;if(i==17U)query_qpc=101001U;
  if(i==24U)receiver_now=qg[i].global_until;
  for(j=0;j<PAPACC_SCQ_BOOTSTRAP_BYTES+1U;j++){e=scqb_read(&f->recipient,&f->runtime_owner,&f->stop);if(e==ERROR_IO_PENDING&&f->recipient.transport.query_refs3)C(scqb_initial_wait(&f->recipient,&f->runtime_owner,&f->stop));if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)break;}
  if(i==17U||i==24U||i==66U){C(e!=0&&!b->origin.phase);continue;}C(!e);
  if(i==49U)receiver_now=qg[i].global_until;if(i==50U)f->stop=1U;
  if(i==47U)mcase=21U;
  if(i==5U)receiver_fault=6U;if(i==6U)capture_fault=1U;
  e=scqb_candidates(&f->recipient,&f->runtime_owner,&p,&f->stop);if(i==5U||i==6U||i==20U||i==21U||(i>=40U&&i<=47U)||i==49U||i==50U||i==65U){C(e!=0&&!b->origin.phase);continue;}C(!e);
  if(i==48U||i==60U){C(scqb_candidates(&f->recipient,&f->runtime_owner,&p,&f->stop)!=0);continue;}
  if(i==63U){C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);continue;}
  C(!scqb_initial_wait(&f->recipient,&f->runtime_owner,&f->stop));
  /* Production receiver chooses its locator BEFORE first_prepare. A zero
   * profile is expected; it must not route this QUERY through paths_new. */
  C(!f->runtime_profile.phase&&!f->runtime_profile.pre_readiness);
  C(scqb_existing_target_query(&f->recipient,&p)==(qr[i].p1_handles!=0U));
  {DWORD saved=p.purpose;p.purpose=0U;C(!scqb_existing_target_query(&f->recipient,&p));p.purpose=saved;}
  {DWORD saved=f->recipient.role;f->recipient.role=1U;C(!scqb_existing_target_query(&f->recipient,&p));f->recipient.role=saved;}
  {DWORD saved=f->runtime.closed_preparation;f->runtime.closed_preparation=0U;C(!scqb_existing_target_query(&f->recipient,&p));f->runtime.closed_preparation=saved;}
  {DWORD saved=f->recipient.transport.capture_valid;f->recipient.transport.capture_valid=3U;C(!scqb_existing_target_query(&f->recipient,&p));f->recipient.transport.capture_valid=saved;}
  {DWORD saved=qr[i].p1_handles;qr[i].p1_handles=0U;C(!scqb_existing_target_query(&f->recipient,&p));qr[i].p1_handles=saved;}
  C(!capture_input_model(b,&p)); /* native content/SD boundary modeled */
  if(i==7U)b->inputs.digest[0][0]^=1U;
  e=scqb_query_image(&f->recipient,&p,&b->inputs);if(i==7U||i==8U){C(e!=0&&!b->origin.phase);continue;}C(!e);
  C(!scqb_bind_query(&f->recipient,&f->runtime_owner,&b->receiver_selection,&p,&b->inputs,&paths,&f->stop));
  C(scqb_bind_query(&f->recipient,&f->runtime_owner,&b->receiver_selection,&p,&b->inputs,&paths,&f->stop)!=0);
  if(i==19U){f->recipient.transport.capture_carrier.global_until[0]++;C(scqb_original_deadline(&f->recipient,&p)!=0&&!b->origin.phase);continue;}
  C(!scqb_original_deadline(&f->recipient,&p));
  C(!b->receiver_selection.source&&!b->receiver_selection.controller);
  if(i==64U)mt_local_dup_hook=query_copy_stale;if(i==68U)mt_local_dup_hook=query_copy_stop;if(i==69U)mt_local_dup_hook=query_copy_guard;
  if(i==70U)f->recipient.transport.capture_valid=6U;
  e=scqr_first_prepare(&b->origin,&f->recipient,&f->runtime_owner,&b->receiver_selection,&f->runtime_profile,&b->inputs,&f->runtime,&f->runtime_dispatch,&f->runtime_receiver,&p,f->recipient.transport.capture_controller,&f->stop);
  if(i==61U||i==64U||i==68U||i==69U){C(e!=0&&b->origin.value.retained);continue;}if(i==70U){C(e!=0&&!b->origin.phase);continue;}C(!e);
  C(!b->origin.value.controller_consumed);if(i==52U)mcase=13U;
  e=scqb_promote(&f->recipient,&f->runtime_owner,&b->origin,&b->inputs,&f->stop);
  if(i==52U){C(e!=0&&b->origin.value.retained);continue;}C(!e);
  if(i==34U)C(!mt_admin_reopens);
  if(f->recipient.transport.query_refs3)C(receiver_admin_closed_mask==3U&&b->origin.value.operator_process==(HANDLE)644&&b->origin.value.operator_token==(HANDLE)645);
  if(f->recipient.transport.query_refs3)C(f->recipient.transport.closed_query[0]&&f->recipient.transport.closed_query[1]&&b->origin.value.operator_process&&b->origin.value.operator_token);
  C(scqb_promote(&f->recipient,&f->runtime_owner,&b->origin,&b->inputs,&f->stop)!=0);
  C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);
  if(i==12U||i==55U)continue; /* No receipt: never ready. */
  mcase=0;C(!scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
  mpeer=f->plan.administrator.pid;C(!transfer(&f->recipient,&f->runtime_owner,1U,f,&b->origin.value));mpeer=f->plan.runtime.pid;
  C(!transfer(&f->controller,&f->administrator_owner,2U,f,&f->source.value));mpeer=f->plan.administrator.pid;
  C(!scqt_produce_close(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&b->origin.value,&f->stop));
  C(!transfer(&f->recipient,&f->runtime_owner,4U,f,&b->origin.value));mpeer=f->plan.runtime.pid;
  if(i==53U)memcpy(wire,"SQC1",4);if(i==54U)wire[PAPACC_SCQ_CLOSE_BYTES]^=1U;
  if(i==53U||i==54U){C(transfer(&f->controller,&f->administrator_owner,8U,f,&f->source.value)!=0);continue;}
  C(!transfer(&f->controller,&f->administrator_owner,8U,f,&f->source.value));C(!transfer(&f->controller,&f->administrator_owner,16U,f,&f->source.value));mpeer=f->plan.administrator.pid;
  if(i==56U){C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);continue;}
  if(i==67U){wire[PAPACC_SCQ_CLOSE_BYTES+8U]^=1U;C(transfer(&f->recipient,&f->runtime_owner,32U,f,&b->origin.value)!=0);continue;}
  C(!transfer(&f->recipient,&f->runtime_owner,32U,f,&b->origin.value));
  if(i==51U)receiver_fault=5U;if(i==13U)f->recipient.transport.close_confirmed=0;if(i==14U)f->stop=1;if(i==26U)f->recipient.peer_lost=1U;
  e=scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop);if(i==13U||i==14U||i==26U||i==51U){C(e!=0);continue;}C(!e);complete++;
  if(i==15U||i==62U)mcase=13U;
  e=scqs_close(&f->recipient,&f->runtime_owner,1U);if(i==15U||i==62U){C(e!=0&&b->origin.value.retained);continue;}C(!e);C(!scqr_close(&b->origin,&f->runtime_owner));if(f->recipient.transport.query_refs3)C(receiver_admin_closed_mask==15U);
  C(!scqs_close(&f->controller,&f->administrator_owner,1U));
  for(j=0;j<3U;j++)C(!f->recipient.transport.capture_refs[j]);
 }
 {PAPACC_SCQ_SESSION_DRIVER c;PAPACC_SCQ_START_ASSURANCE g;volatile LONG stop=0;unsigned k;
  memset(&c,0,sizeof(c));memset(&g,0,sizeof(g));c.transport.resource_scope=&g;c.owner=&c;g.self=&g;g.owner=&c;g.thread=GetCurrentThreadId();g.phase=5U;g.p1_handles=1U;scqg_selected_p1(&g.limits);g.global_until=GetTickCount64()+60000U;
  destination_override=1;
  for(k=187U;k<=189U;k++){destination_count=k;C((scqt_destination_effect(&c,(HANDLE)44,&stop)==0)==(k==187U));}
  destination_override=2;C(scqt_destination_effect(&c,(HANDLE)44,&stop)!=0);
  destination_override=1;destination_count=100U;C(scqt_destination_effect(&c,NULL,&stop)!=0);
  stop=1;C(scqt_destination_effect(&c,(HANDLE)44,&stop)!=0);stop=0;
  g.limits.max_handles=128U;C(scqt_destination_effect(&c,(HANDLE)44,&stop)!=0);
  destination_override=0;
 }
 /* SQF1 actual codec/send/receive with held original test packets; native APIs
  * remain mocked. No negative may set ack/close/readiness or acquire handles. */
 {PAPACC_SCQ_TRANSPORT frozen;BYTE attempt[32];unsigned k;
  memset(&frozen,0,sizeof(frozen));memset(attempt,7,sizeof(attempt));memcpy(frozen.capture_carrier.attempt,attempt,32);
  scqf_freeze(&frozen,9U,ERROR_ACCESS_DENIED);C(scqf_frame(frozen.failure_frame,56U,attempt));
  scqf_freeze(&frozen,10U,ERROR_INVALID_DATA);C(frozen.failure_stage==9U&&frozen.failure_error==ERROR_ACCESS_DENIED);
  C(!scqf_frame(frozen.failure_frame,55U,attempt));C(!scqf_frame(frozen.failure_frame,57U,attempt));
  for(k=0;k<56U;k++){BYTE saved=frozen.failure_frame[k];frozen.failure_frame[k]^=128U;
   if(k<12U||k>=24U)C(!scqf_frame(frozen.failure_frame,56U,attempt));frozen.failure_frame[k]=saved;}
  scqf_put(frozen.failure_frame+16,3U);C(!scqf_frame(frozen.failure_frame,56U,attempt));
  scqf_put(frozen.failure_frame+16,0U);scqf_put(frozen.failure_frame+20,1U);C(!scqf_frame(frozen.failure_frame,56U,attempt));
  frozen.failure_error=ERROR_IO_PENDING;C(scqf_terminal(&frozen)==ERROR_ACCESS_DENIED&&frozen.failure_error==ERROR_IO_PENDING);
  frozen.failure_error=ERROR_MORE_DATA;C(scqf_terminal(&frozen)==ERROR_ACCESS_DENIED&&frozen.failure_error==ERROR_MORE_DATA);
 }
 {CAPTURE_FIXTURE *b=&qc[55];SESSION_FIXTURE *f=&b->f;PAPACC_SCQ_SESSION_DRIVER saved=f->recipient;
  unsigned k;DWORD before_handles=mduplicates,before_close=mcloses;
  query_fault=origin_fault=receiver_fault=mt_query_fault=0;runtime_service_state=SERVICE_START_PENDING;query_qpc=1010U;receiver_now=1000U;receiver_admin_closed_mask=0;
  for(k=0;k<16U;k++){
   f->recipient=saved;f->stop=0;query_qpc=1010U;receiver_now=1000U;mcase=mcalls=mposition=mincomplete=mcancel=0;mpeer=f->plan.administrator.pid;
   scqf_freeze(&f->recipient.transport,9U,ERROR_ACCESS_DENIED);
   if(k==1U)f->recipient.transport.pending=1U;if(k==2U)f->stop=1;
   if(k==3U)f->recipient.transport.query_refs3=0;if(k==4U)mcase=5U;
   if(k==5U)mcase=15U;if(k==6U)mcase=17U;if(k==7U)mcase=2U;if(k==8U)mcase=12U;
   if(k==9U)query_qpc=101001U;if(k==10U)receiver_now=qr[55].global_until;
   if(k==11U)f->recipient.transport.pipe=NULL;if(k==12U)f->recipient.transport.event=NULL;
   if(k==13U)f->recipient.transport.cancelled=1U;if(k==14U)mcase=22U;if(k==15U)mcase=34U;
   e=scqf_send(&f->recipient,&f->runtime_owner,&f->stop);
   if(k==0U)C(e==ERROR_ACCESS_DENIED&&mcalls==1U&&f->recipient.transport.failure_state==3U&&!f->recipient.transport.failure_secondary);
   if((k>=1U&&k<=4U)||(k>=9U&&k<=14U))C(e==ERROR_ACCESS_DENIED&&!mcalls&&f->recipient.transport.failure_state==3U);
   if(k==5U||k==6U)C(e==ERROR_ACCESS_DENIED&&mcalls==1U&&f->recipient.transport.failure_secondary);
   if(k==7U||k==8U){C(e==ERROR_IO_PENDING&&mcalls==1U&&f->recipient.transport.failure_pending);
    C(scqf_send(&f->recipient,&f->runtime_owner,&f->stop)==ERROR_IO_PENDING);
    if(k==7U)C(scqf_send(&f->recipient,&f->runtime_owner,&f->stop)==ERROR_ACCESS_DENIED&&!f->recipient.transport.failure_pending);
   }
   if(k==15U){C(e==ERROR_IO_PENDING&&mcalls==1U);
    C(scqf_send(&f->recipient,&f->runtime_owner,&f->stop)==ERROR_ACCESS_DENIED);
    C(f->recipient.transport.failure_pending&&f->recipient.transport.pending&&f->recipient.transport.failure_secondary&&f->recipient.transport.failure_state==3U);}
   C(f->recipient.transport.failure_stage==9U&&f->recipient.transport.failure_error==ERROR_ACCESS_DENIED);
   {DWORD calls=mcalls;if(k!=8U)C(scqf_send(&f->recipient,&f->runtime_owner,&f->stop)==ERROR_ACCESS_DENIED);C(mcalls==calls);}
   C(mduplicates==before_handles&&mcloses==before_close);
  }
  f->recipient=saved;f->stop=0;
 }
 {CAPTURE_FIXTURE *b=&qc[55];SESSION_FIXTURE *f=&b->f;PAPACC_SCQ_SESSION_DRIVER saved=f->controller;
  PAPACC_SCQ_ADMIN_SOURCE source=f->source;PAPACC_SCQ_TRANSPORT encoded;unsigned k;
  memset(&encoded,0,sizeof(encoded));memcpy(encoded.capture_carrier.attempt,f->plan.attempt,32);scqf_freeze(&encoded,10U,ERROR_CRC);
  for(k=0;k<14U;k++){
   PAPACC_SCQ_TRANSPORT *t;LONG ack=f->controller_profile.ack;DWORD legs;
   f->controller=saved;f->source=source;t=&f->controller.transport;
   t->operation=2U;t->pending=0;t->offset=0;t->total=1052U;legs=t->legs;
   memcpy(f->controller.buffer,encoded.failure_frame,56U);mpeer=f->plan.runtime.pid;mcase=0;
   if(k==1U)f->controller.buffer[24]^=1U;if(k==2U)scqf_put(f->controller.buffer+16,3U);
   if(k==3U)scqf_put(f->controller.buffer+20,1U);if(k==4U)t->failure_reported=1U;
   if(k==5U)t->query_refs3=0;if(k==6U)mcase=5U;
   if(k==7U)scqf_put(f->controller.buffer+4,57U);
   if(k==12U)scqf_put(f->controller.buffer+12,ERROR_IO_PENDING);if(k==13U)scqf_put(f->controller.buffer+12,ERROR_MORE_DATA);
   if(k==10U)f->controller.buffer[3]='2';if(k==11U){scqf_put(f->controller.buffer+16,1U);scqf_put(f->controller.buffer+20,0xc0000022U);}
   if(k==8U){C(scqt_io_result(&f->controller,57U)!=0&&!t->failure_reported);continue;}
   if(k==9U){C(scqt_io_result(&f->controller,55U)==ERROR_MORE_DATA&&!t->failure_reported&&t->total==56U);continue;}
   if(k==0U||k>=11U){C(scqt_io_result(&f->controller,1U)==ERROR_MORE_DATA);C(scqt_io_result(&f->controller,3U)==ERROR_MORE_DATA&&t->total==56U);e=scqt_io_result(&f->controller,52U);C(e==ERROR_ACCESS_DENIED&&t->failure_reported&&t->failure_stage==10U);if(k==11U)C(t->failure_kind==1U&&t->failure_raw==0xc0000022U);}
   else C(scqt_io_result(&f->controller,56U)!=0);
   C(f->controller_profile.ack==ack&&t->legs==legs&&!t->close_accepted&&!t->close_confirmed);
  }
  f->controller=saved;f->source=source;
 }
 /* Refine the real first-error latch, not a guessed API from win325. */
 {static PAPACC_PERSONAL_SCQ_INPUTS in;PAPACC_SCQ_TRANSPORT t;unsigned k;
  for(k=0;k<9U;k++){
   memset(&in,0,sizeof(in));memset(&t,0,sizeof(t));memset(t.capture_carrier.attempt,1,32);
   in.error=ERROR_ACCESS_DENIED;
   if(k==0U)in.first=31U;
   else if(k<=3U){in.first=15U;if(k>=2U)in.reader[0]=(HANDLE)1;if(k==3U)in.reader[1]=(HANDLE)2;}
   else if(k==4U)in.first=24U;else if(k==5U)in.first=5U;
   else if(k==6U){in.first=15U;in.reader[1]=(HANDLE)2;}
   else if(k==7U){in.first=15U;in.error=ERROR_INVALID_DATA;}
   scqf_freeze_inputs(&t,10U,ERROR_ACCESS_DENIED,&in);
   C(t.failure_stage==(k<=3U?20U+k:k==4U||k==5U?24U:k==7U?21U:10U));
   C(t.failure_kind==(k<=3U?2U:0U)&&t.failure_raw==(k<=3U?ERROR_ACCESS_DENIED:0U));
   C(scqf_frame(t.failure_frame,56U,t.capture_carrier.attempt));
   {DWORD stage=t.failure_stage;scqf_freeze_inputs(&t,10U,ERROR_INVALID_DATA,&in);C(t.failure_stage==stage&&t.failure_error==ERROR_ACCESS_DENIED);}
  }
 }
 {static PAPACC_PERSONAL_SCQ_INPUTS in;PAPACC_SCQ_TRANSPORT t;
  memset(&in,0,sizeof(in));memset(&t,0,sizeof(t));in.first=31U;in.error=ERROR_NOT_ENOUGH_QUOTA;
  scqf_freeze_inputs(&t,10U,ERROR_NOT_ENOUGH_QUOTA,&in);
  C(t.failure_stage==20U&&t.failure_error==ERROR_NOT_ENOUGH_QUOTA&&!t.failure_kind&&!t.failure_raw);
 }
 C(complete==11U&&!cs_running);
 printf("PASS QUERY_COLD_CHAIN cases=71 P1_FULL_CHAIN_AND_DESTINATION_BOUNDARIES prestart_report=ACTUAL_WITH_ORIGINAL_ACTOR_MODEL_BRIDGES complete_before_fault=%u checks=%u NO_FUTURE_RUNTIME_PROOF_BEFORE_START UNKNOWN_63_AND_LATER_15_PRESERVED SQB3_SQC3_QUERY_REFERENCES_AND_Q6_CUSTODY_GLOBAL_DEADLINE_BOUND NO_RENEWAL_NO_DOWNGRADE FRESHNESS_AT_REAL_HELPER_BOUNDARY ZERO_PRESEEDED_RECIPIENT_ACT THREE_CONTROLLER_ACTS ORIGINAL_OFFER_RECEIPT_CLOSE NATIVE_APIS_MODELED NO_REAL_ENDPOINT_NO_START\n",complete,checks-before);return 0;
}
