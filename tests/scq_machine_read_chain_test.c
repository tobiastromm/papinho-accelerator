/* Actual READ QUERY/delivery/P/CA/read-result/receipt/close/lifecycle bodies.
 * Every operational OS API is modeled. No installed service/provider/key used. */
#define PAPACC_QUERY_COLD_ENTRY preserved_query_chain
#include "scq_query_cold_chain_test.c"
#include "../apps/server/personal_scq_read_diagnostic_win32.inc"
#include "../apps/server/personal_scq_read_runtime_win32.inc"
#include "server_configuration_commit_win32.h"
static int owner(PAPACC_CONFIGURATION_COMMIT *o){(void)o;return 0;}
#include "../apps/server/server_store_data_hash_win32.inc"
DWORD read_owner_machine_read_bind(PAPACC_CA_KEY *,const struct PAPACC_SCQ_SESSION_DRIVER *,const struct PAPACC_SCQ_READ_DIAGNOSTIC *,const void *,volatile LONG *);
SECURITY_STATUS read_owner_fixture_read_open(PAPACC_CA_KEY *,const BYTE[16],const BYTE[16],PSID,PSID);
SECURITY_STATUS read_owner_validate(PAPACC_CA_KEY *);SECURITY_STATUS read_owner_close(PAPACC_CA_KEY *);
SECURITY_STATUS read_owner_fixture_control(PAPACC_CA_KEY *,BYTE *,DWORD,DWORD *);
#define papacc_ca_key_machine_read_bind read_owner_machine_read_bind
#define papacc_ca_key_fixture_read_open read_owner_fixture_read_open
#define papacc_ca_key_validate read_owner_validate
#define papacc_ca_key_close read_owner_close
#define papacc_ca_key_fixture_control read_owner_fixture_control
#define PAPACC_MACHINE_FIXTURE_DATA_HASH store_data_hash
#include "../apps/server/personal_machine_fixture_win32.inc"
#include "../apps/server/personal_scq_machine_read_continue_win32.inc"
#include "../apps/server/personal_scq_inconclusive_controller_win32.inc"
static PAPACC_CONFIGURATION_COMMIT terminal_owner;
int read_ca_model_begin(PSID,PSID,unsigned,unsigned);
DWORD read_ca_model_creates(void);DWORD read_ca_model_opens(void);DWORD read_ca_model_calls(void);
static CAPTURE_FIXTURE rb[27];static PAPACC_SCQ_READ_DIAGNOSTIC rk[27];static PAPACC_SCQ_READ_RECIPIENT rr[27];
static PAPACC_SCQ_START_ASSURANCE rg[27][2];static PAPACC_MACHINE_FIXTURE_CONTROL key_fixtures[27];static PAPACC_PERSONAL_CNG_PROBE rp[27][2];
static BYTE public_id[32];static unsigned chain_case;
static DWORD chain_step(unsigned i,int runtime)
{
 CAPTURE_FIXTURE *b=&rb[i];SESSION_FIXTURE *f=&b->f;
 return scqkm_continue_session(runtime?&f->recipient:&f->controller,runtime?&f->runtime_owner:&f->administrator_owner,&rk[i],&rr[i],&b->origin,&b->inputs,&key_fixtures[i],&rp[i][runtime?1:0],runtime?f->recipient.transport.read_public:f->controller.transport.read_public,runtime?&b->origin.value:&f->source.value,&f->stop);
}
static DWORD chain_until(unsigned i,int runtime,DWORD goal)
{
 SESSION_FIXTURE *f=&rb[i].f;PAPACC_SCQ_SESSION_DRIVER *c=runtime?&f->recipient:&f->controller;unsigned n;DWORD e;
 mpeer=runtime?f->plan.administrator.pid:f->plan.runtime.pid;mposition=mincomplete=0;
 for(n=0;n<4000U;n++){
  e=chain_step(i,runtime);
  if((goal==0x10000U&&rr[i].phase==3U)||(goal==0x20000U&&c->phase==4U&&c->profile->ack)||(goal<0x10000U&&(c->transport.legs&goal)))return e==ERROR_MORE_DATA||e==ERROR_IO_PENDING?0:e;
  if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)return e?e:ERROR_INVALID_DATA;
 }return ERROR_TIMEOUT;
}
static int read_service_run(int argc,char **argv)
{
 (void)argc;(void)argv;C(host.life.phase==PS_START_PENDING&&!host.closed_ready&&!papacc_server_service_runtime_ready());
 C(papacc_server_service_scq_closed_ready());C(cs_running==1U&&!cs_premature&&host.life.phase==PS_RUNNING&&!papacc_server_service_runtime_ready());
 C(!scqs_close(&active_f->recipient,&active_f->runtime_owner,1U));C(!scqr_close(active_r,&active_f->runtime_owner));
 C(!scqs_close(&active_f->controller,&active_f->administrator_owner,1U));
 C(papacc_server_service_runtime_stopping());papacc_server_service_runtime_closed();return 0;
}
static int read_inconclusive_service_run(int argc,char **argv)
{
 (void)argc;(void)argv;
 C(host.life.phase==PS_START_PENDING&&!host.closed_ready&&!papacc_server_service_runtime_ready());
 C(!papacc_server_service_scq_closed_ready()&&!cs_running&&!cs_armed);
 C(!scqr_closed_terminal_current(active_r,&active_f->recipient,&active_f->runtime_owner,active_i,&active_f->stop,1U));
 C(!scqs_close(&active_f->recipient,&active_f->runtime_owner,1U));C(!scqr_close(active_r,&active_f->runtime_owner));
 C(!scqs_close(&active_f->controller,&active_f->administrator_owner,1U));
 C(papacc_server_service_runtime_stopping());papacc_server_service_runtime_closed();return ERROR_NOT_FOUND;
}
int main(int argc,char **argv)
{
 unsigned i,j,start=checks,closed=0;DWORD e;BYTE preimage[84],blob[283];PAPACC_STORE_HASH hash;BCRYPT_RSAKEY_BLOB *rsa=(BCRYPT_RSAKEY_BLOB*)blob;
 unsigned observed_sd=argc==2&&!strcmp(argv[1],"observed-sd")?1U:0U;C(argc==1||(argc==2&&observed_sd));
 C(ConvertStringSidToSidA("S-1-5-6",&service_sid));
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 memset(blob,0,sizeof(blob));rsa->Magic=BCRYPT_RSAPUBLIC_MAGIC;rsa->BitLength=2048;rsa->cbPublicExp=3;rsa->cbModulus=256;blob[sizeof(*rsa)]=blob[sizeof(*rsa)+2U]=blob[282]=1;memset(&hash,0,sizeof(hash));
 C(store_data_hash(&hash,blob,sizeof(blob),public_id));
 {PAPACC_SCQ_START_LIMITS limits;scqg_selected_p1(&limits);C(!scqk_policy_data(&limits,preimage));C(store_data_hash(&hash,preimage,sizeof(preimage),machine_read_policy));}
 /* Actual terminal wrapper: preserve native close errors/pending and deny
  * every non-permission result; no provider or key API is called here. */
 {PAPACC_PERSONAL_CNG_PROBE terminal;unsigned v;memset(&terminal,0,sizeof(terminal));
  terminal.phase=3U;terminal.provider_calls=terminal.open_calls=1U;
  terminal.open_result=NTE_PERM;terminal.classification=PAPACC_CNG_PROBE_PERMISSION_OBSERVED;
  C(!scqkm_close_result(0,1U,&terminal,1U));
  C(scqkm_close_result(ERROR_ACCESS_DENIED,1U,&terminal,1U)==ERROR_ACCESS_DENIED);
  C(scqkm_close_result(ERROR_TIMEOUT,1U,&terminal,1U)==ERROR_TIMEOUT);
  C(scqkm_close_result(ERROR_IO_PENDING,1U,&terminal,1U)==ERROR_IO_PENDING);
  C(scqkm_close_result(ERROR_MORE_DATA,1U,&terminal,1U)==ERROR_MORE_DATA);
  C(!scqkm_close_result(0,2U,NULL,0U));
  C(scqkm_close_result(0,1U,&terminal,0U)==ERROR_ACCESS_DENIED);
  C(scqkm_close_result(0,1U,NULL,1U)==ERROR_ACCESS_DENIED);
  for(v=0;v<=PAPACC_CNG_PROBE_CLOSE_UNKNOWN;v++){if(v==PAPACC_CNG_PROBE_PERMISSION_OBSERVED)continue;terminal.classification=v;C(scqkm_close_result(0,1U,&terminal,1U)==ERROR_ACCESS_DENIED);}
  terminal.classification=PAPACC_CNG_PROBE_PERMISSION_OBSERVED;terminal.close_error=ERROR_INVALID_HANDLE;
  C(scqkm_close_result(0,1U,&terminal,1U)==ERROR_ACCESS_DENIED);
 }
 {PAPACC_PERSONAL_CNG_PROBE p,q;unsigned z;memset(&p,0,sizeof(p));p.phase=3U;p.classification=PAPACC_CNG_PROBE_NOT_FOUND;
  p.provider_calls=p.open_calls=p.close_attempted=p.close_calls=1U;p.open_result=p.first_error=NTE_BAD_KEYSET;p.first_stage=4U;p.first_origin=3U;
  C(papacc_personal_cng_inconclusive(&p));C(!papacc_personal_cng_inconclusive(NULL));
  C(scqkm_close_result(0,1U,&p,1U)==ERROR_NOT_FOUND);C(scqkm_close_result(0,1U,&p,0U)==ERROR_ACCESS_DENIED);
  for(z=0;z<15U;z++){q=p;switch(z){
   case 0:q.phase=4U;break;case 1:q.classification=PAPACC_CNG_PROBE_UNEXPECTED_OPEN;break;
   case 2:q.provider_calls=0;break;case 3:q.provider_result=NTE_FAIL;break;case 4:q.open_calls=2;break;
   case 5:q.open_result=NTE_PERM;break;case 6:q.first_stage=2;break;case 7:q.first_origin=1;break;
   case 8:q.first_error=NTE_FAIL;break;case 9:q.close_attempted=0;break;case 10:q.close_calls=0;break;
   case 11:q.close_error=NTE_FAIL;break;case 12:q.provider=1;break;case 13:q.key=1;break;default:q.unexpected_thread=(HANDLE)1;break;}
   C(!papacc_personal_cng_inconclusive(&q));C(scqkm_close_result(0,1U,&q,1U)==ERROR_ACCESS_DENIED);
  }
 }
 {SERVICE_STATUS_PROCESS observed;DWORD expected;unsigned z;
  for(z=0;z<2U;z++){expected=z?ERROR_NOT_FOUND:0;memset(&observed,0,sizeof(observed));observed.dwServiceType=SERVICE_WIN32_OWN_PROCESS;observed.dwCurrentState=SERVICE_STOPPED;
   observed.dwWin32ExitCode=z?ERROR_SERVICE_SPECIFIC_ERROR:ERROR_SUCCESS;observed.dwServiceSpecificExitCode=expected;
   C(ps_scq_terminal_status(&observed,expected,expected));C(!ps_scq_terminal_status(&observed,expected+1,expected));
   C(!ps_scq_terminal_status(&observed,expected,ERROR_ACCESS_DENIED));observed.dwProcessId=1;C(!ps_scq_terminal_status(&observed,expected,expected));observed.dwProcessId=0;
   observed.dwCurrentState=SERVICE_RUNNING;C(!ps_scq_terminal_status(&observed,expected,expected));observed.dwCurrentState=SERVICE_STOPPED;
   observed.dwServiceSpecificExitCode++;C(!ps_scq_terminal_status(&observed,expected,expected));
  }
 }
 scq_fixture_window=240000U;
 for(i=0;i<27U;i++){
  CAPTURE_FIXTURE *b=&rb[i];SESSION_FIXTURE *f=&b->f;PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_START_LIMITS limits;PAPACC_PERSONAL_INSTALL_PATHS paths;char reply[88];
  PAPACC_SCM_RESOURCE_SAMPLE sample={1000U,100U,100U,1000000U,2000000U,8000000000ULL,11000000000ULL,100U,1U};
  chain_case=i;mcase=mcreated=mopened=mevents=mposition=mincomplete=mcancel=capture_fault=query_fault=origin_fault=mt_query_fault=mt_admin_reopens=0;receiver_identity_alias=receiver_admin_copy_model=1;receiver_admin_closed_mask=0;query_qpc=100;mt_local_dup_hook=NULL;
  C(!setup_session_typed_capture(f,1,1U,PAPACC_SCQ_PURPOSE_READ,1U,0));observation_service=1U;query_stop_ptr=&f->stop;f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=f->policy.data=L"";
  C(read_ca_model_begin(sam_sid,virtual_sid,i==18U?1U:i==19U?2U:i==21U?4U:0U,observed_sd));
  if(i==1U)f->source.value.request=NULL;if(i==2U)f->source.value.request_id.nFileIndexLow++;
  scqg_selected_p1(&limits);e=scqg_capture_machine_read(&rg[i][0],&f->administrator_owner,&f->source,&limits,&f->stop);
  if(i==1U||i==2U){C(e!=0&&!read_ca_model_calls());continue;}C(!e);rg[i][0].initial_known=1U;rg[i][0].initial_handles=128U;C(!scqg_presented(&rg[i][0],&f->administrator_owner,&f->stop));
  C(!scqa_present(&f->source,&f->administrator_owner,&f->stop));C(!query_confirm(&f->source,f,0));C(!scqa_endpoint_present(&f->source,&f->administrator_owner,&f->stop));C(!query_confirm(&f->source,f,1));C(!scqa_start_present(&f->source,&f->administrator_owner,&f->stop));C(!query_confirm(&f->source,f,2));
  C(!scqk_prepare(&rk[i],&f->administrator_owner,&f->source,public_id,&f->stop));
  memcpy(reply,"OWNER_SCQ_MACHINE_READ ",23);for(j=0;j<32U;j++){reply[23U+2U*j]="0123456789abcdef"[rk[i].attempt[j]>>4];reply[24U+2U*j]="0123456789abcdef"[rk[i].attempt[j]&15];}reply[87]=0;
  if(i!=25U)C(!scqk_confirm(&rk[i],&f->administrator_owner,reply,87,&f->stop));
  if(i==20U){rk[i].key_public[0]^=1;}
  C(!scqg_ready(&rg[i][0],&f->administrator_owner,&sample,100,&f->stop));C(!scqg_controller_deadline(&rg[i][0],100));rg[i][0].observations.last=sample;
  C(!scqa_start_carrier(&f->source,&f->administrator_owner,&b->carrier,&f->stop));b->carrier.global_until[0]=(DWORD)rg[i][0].global_until;
  C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&b->controller_runtime));f->controller.read_act=&rk[i];
  C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));if(i==26U)C(scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop)!=0);
  C(!scqa_endpoint_take(&f->source,&f->administrator_owner,&f->stop));f->controller.transport.resource_scope=&rg[i][0];f->controller.transport.global_until=rg[i][0].global_until;
  e=scqt_reserve(&f->controller,&f->administrator_owner,&f->stop);if(i==25U){C(e!=0&&!rk[i].consumed&&!read_ca_model_calls());continue;}C(!e);
  C(!scqa_start_take(&f->source,&f->administrator_owner,&f->stop));
  runtime_service_state=SERVICE_START_PENDING;C(!observed_runtime_identity_mode(f,1U));C(!papacc_personal_runtime_capture_machine_read_closed(&b->controller_runtime,(HANDLE)1,virtual_sid,1U));
  C(!papacc_personal_scq_prepare(&f->controller_dispatch,&b->controller_runtime,f->plan.attempt,fixture_name));
  if(i==3U)f->plan.runtime.pid++;e=scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop);if(i==3U){C(e!=0&&!read_ca_model_calls());continue;}C(!e);
  if(i==14U)sample.handles=129U;e=scqg_query_ready_mode(&rg[i][1],&f->runtime_owner,&sample,100,100,rg[i][0].global_until,&f->stop,1);if(i==14U){C(e!=0&&!read_ca_model_calls());continue;}C(!e);rg[i][1].read_diagnostic=1U;rg[i][1].observations.last=sample;f->recipient.transport.resource_scope=&rg[i][1];
  if(i==8U)f->stop=1;mpeer=f->plan.administrator.pid;
  e=scqb_open_query_variant(&f->recipient,&f->runtime_owner,&b->carrier,&b->receiver_selection,&f->runtime,&f->runtime_profile,&f->runtime_receiver,&f->runtime_dispatch,&f->stop,1U);if(i==8U){C(e!=0&&!read_ca_model_calls());continue;}C(!e&&!b->origin.phase&&!b->receiver_selection.source);
  mpeer=f->plan.runtime.pid;C(!scqt_connect(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));C(!scqt_connected(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));C(!scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
  C(f->controller.transport.query_refs3&&f->controller.transport.remote_query[0]&&f->controller.transport.remote_query[1]&&!mt_admin_reopens);
  for(j=0;j<2000U;j++){e=scqb_write(&f->controller,&f->administrator_owner,&f->stop);if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)break;}C(!e&&!rk[i].consumed&&!read_ca_model_calls());
  mpeer=f->plan.administrator.pid;mposition=0;for(j=0;j<2000U;j++){e=scqb_read(&f->recipient,&f->runtime_owner,&f->stop);if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)break;}C(!e);
  if(i==5U)mpeer++;e=scqb_candidates(&f->recipient,&f->runtime_owner,&p,&f->stop);if(i==5U){C(e!=0&&!read_ca_model_calls());continue;}C(!e);C(!capture_input_model(b,&p));
  memset(&paths,0,sizeof(paths));paths.read_diagnostic=1U;memcpy(paths.read_public,f->recipient.transport.read_public,32);C(!scqb_query_image(&f->recipient,&p,&b->inputs));C(!scqb_bind_query(&f->recipient,&f->runtime_owner,&b->receiver_selection,&p,&b->inputs,&paths,&f->stop));C(!scqb_original_deadline(&f->recipient,&p));
  C(!scqr_first_prepare(&b->origin,&f->recipient,&f->runtime_owner,&b->receiver_selection,&f->runtime_profile,&b->inputs,&f->runtime,&f->runtime_dispatch,&f->runtime_receiver,&p,f->recipient.transport.capture_controller,&f->stop));
  if(i==4U){C(chain_step(i,1)!=0&&!rk[i].consumed&&!read_ca_model_calls());continue;}
  C(!scqb_promote(&f->recipient,&f->runtime_owner,&b->origin,&b->inputs,&f->stop));if(i==6U)C(scqb_promote(&f->recipient,&f->runtime_owner,&b->origin,&b->inputs,&f->stop)!=0);
  C(receiver_admin_closed_mask==3U&&b->origin.value.consumed&&b->origin.value.borrowed&&!rk[i].consumed&&!read_ca_model_calls());
  qfault=qopens=qproviders=qfrees=0;qstop=0;
  C(!chain_until(i,1,SCQK_READY_SEND));C(!qproviders&&!read_ca_model_calls());
  if(i==7U)receiver_now=rg[i][0].global_until;if(i==24U)wire[36]^=1U;if(i==13U)rg[i][0].observations.last.handles=193U;
  e=chain_until(i,0,SCQK_READY_READ);if(i==7U||i==13U||i==24U){C(e!=0&&!rk[i].consumed&&!read_ca_model_calls());continue;}C(!e&&!rk[i].consumed);
  if(i==9U)f->stop=1;e=chain_until(i,0,SCQK_P_SEND);
  if(e&&i==0U)printf("READ_CONTROL_FAILURE e=%lu pphase=%lu key_state=%u key_error=%08lx id_phase=%lu id_first=%lu ca_function=%lu ca_stage=%lu ca_origin=%lu ca_raw=%08lx ca_calls=%lu opens=%lu\n",(unsigned long)e,(unsigned long)rk[i].phase,(unsigned)key_fixtures[i].key.state,(unsigned long)key_fixtures[i].key.error,(unsigned long)key_fixtures[i].phase,(unsigned long)key_fixtures[i].first,(unsigned long)key_fixtures[i].key.first_failure.function,(unsigned long)key_fixtures[i].key.first_failure.stage,(unsigned long)key_fixtures[i].key.first_failure.origin,(unsigned long)key_fixtures[i].key.first_failure.original_code,(unsigned long)read_ca_model_calls(),(unsigned long)read_ca_model_opens());
  if(i==9U||i==18U||i==19U||i==20U||i==21U){C(e!=0&&!qproviders&&!read_ca_model_creates());continue;}C(!e&&rk[i].consumed&&read_ca_model_opens()==2U&&!read_ca_model_creates());
  if(i==15U)C(scqk_take(&rk[i],&f->administrator_owner,&f->stop)!=0);
  if(i==16U)qfault=3;if(i==17U)qfault=5;if(i==22U)qfault=9;
  e=chain_until(i,1,0x10000U);if(i==22U){C(e!=0&&rp[i][1].phase==4U&&!f->runtime_profile.ack);continue;}C(!e&&qopens==1U&&qproviders==1U);
  C(!chain_until(i,1,SCQK_RESULT_SEND));C(!chain_until(i,0,SCQK_RESULT_READ));
  C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);
  if(i==10U)continue;
  C(!chain_until(i,1,1U));C(!chain_until(i,0,0x20000U));
  if(i==11U){C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);continue;}
  if(i==12U)mcase=13U;if(i==23U)f->stop=1;
  e=chain_until(i,1,4U);if(i==12U||i==23U){C(e!=0&&!f->recipient.transport.close_confirmed);continue;}C(!e);
  C(!chain_until(i,0,8U));
  if(i==16U){
   /* Regression schedule absent from the original serial model: the kernel
    * can expose the already-submitted write to the peer before this owner
    * observes its OVERLAPPED completion. No second write/act is submitted. */
   PAPACC_PERSONAL_RUNTIME_IDENTITY original=*f->controller.runtime;unsigned before_calls;
   mpeer=f->plan.runtime.pid;mposition=0;mcase=12U;
   observation_service=1U; /* full original service-logon token query, not a scalar-only observation */
   e=chain_step(i,0);
   C(e==ERROR_IO_PENDING&&f->controller.transport.pending&&f->controller.transport.operation==16U);
   C(pending_write&&pending_buffer==f->controller.buffer&&pending_bytes==scqt_close_size(&f->controller.transport));
   memcpy(wire,pending_buffer,pending_bytes); /* modeled kernel delivery, not a new IO */
   mcase=0;mposition=0;C(!chain_until(i,1,32U));
   C(f->recipient.transport.close_confirmed&&f->controller.transport.pending&&!(f->controller.transport.legs&16U));
   before_calls=mcalls;fault=28U; /* original retained process is signaled */
   C(papacc_personal_runtime_current(f->controller.runtime)==ERROR_PROCESS_ABORTED);
   C(f->controller.runtime->phase==2U&&f->controller.runtime->first_error==ERROR_PROCESS_ABORTED&&mcalls==before_calls);
   C(f->controller.transport.pending&&f->controller.transport.operation==16U&&!(f->controller.transport.legs&16U));
   printf("PASS TERMINAL_INTERLEAVING_REPRODUCED peer_close_confirmed=1 controller_operation=16 pending=1 live_guard=1067 NO_NEW_WRITE_NO_REAL_PROCESS\n");
   /* The product wrapper selects the held completion before calling live
    * original acquisition. Test actual completion body with the process
    * still signaled; reset only the deliberately poisoned old-gate snapshot. */
   *f->controller.runtime=original;mposition=0;
   {PAPACC_SCQ_SESSION_DRIVER cbase=f->controller;PAPACC_SCQ_ADMIN_SOURCE abase=f->source;PAPACC_SCQ_READ_DIAGNOSTIC kbase=rk[i];
    PAPACC_SCQ_PROFILE_DATA pbase=f->controller_profile;ULONGLONG tickbase=receiver_now;unsigned z,calls=mcalls;
    for(z=0;z<17U;z++){
     f->controller=cbase;f->source=abase;rk[i]=kbase;f->controller_profile=pbase;receiver_now=tickbase;mcase=0;mposition=0;
     if(z==0U)mcase=12U;if(z==1U)mcase=34U;if(z==2U)f->controller.buffer[0]^=1;
     if(z==3U)f->controller.buffer[PAPACC_SCQ_CLOSE_BYTES]^=1;if(z==4U)f->controller.transport.original=NULL;
     if(z==5U)f->controller_profile.read_diagnostic=0;if(z==6U)f->stop=1;
     if(z==7U)receiver_now=rg[i][0].global_until;if(z==8U)rk[i].consumed=0;
     if(z==9U)f->controller.transport.operation=8U;if(z==10U)f->controller.transport.legs|=16U;
     if(z==11U)mcase=21U;if(z==12U)mcase=1U;
     if(z==13U){f->controller.transport.operation=f->controller.transport.pending=f->controller.transport.total=f->controller.transport.offset=0;}
     if(z==14U)f->controller.runtime->pid++;if(z==15U)f->controller.runtime->creation.dwLowDateTime++;
     if(z==16U)f->controller.runtime->statistics.TokenId.LowPart++;
     e=scqkm_return_inconclusive(&f->controller,&f->administrator_owner,&rk[i],&key_fixtures[i],&rp[i][0],&f->stop);
     C(e!=0&&e!=ERROR_NOT_FOUND&&mcalls==calls);if(z==0U)C(e==ERROR_IO_PENDING&&f->controller.transport.pending);
     C(!(f->controller.transport.legs&16U)||z==10U);f->stop=0;*f->controller.runtime=original;
    }
    f->controller=cbase;f->source=abase;rk[i]=kbase;f->controller_profile=pbase;receiver_now=tickbase;mcase=0;mposition=0;
   }
   C(scqkm_return_inconclusive(&f->controller,&f->administrator_owner,&rk[i],&key_fixtures[i],&rp[i][0],&f->stop)==ERROR_NOT_FOUND);
   C((f->controller.transport.legs&16U)&&!f->controller.transport.pending&&!f->controller.transport.operation);
   C(fault==28U);fault=0;
  }
  e=chain_until(i,0,16U);
  if(i==17U){C(e!=0&&rp[i][0].classification!=PAPACC_CNG_PROBE_PERMISSION_OBSERVED);continue;}
  if(i==16U){
   PAPACC_PERSONAL_NATIVE_CONTEXT *n=&terminal_owner.pending_commit.personal_native;
   C(e==ERROR_NOT_FOUND&&papacc_personal_cng_inconclusive(&rp[i][0]));
   memset(&terminal_owner,0,sizeof(terminal_owner));terminal_owner.writer=GetCurrentThreadId();
   n->scq_profile_data=f->controller_profile;n->scq_dispatch=f->controller_dispatch;n->scq_admin=f->source;
   n->scq_session=f->controller;n->scq_read_diagnostic=rk[i];n->machine_fixture=key_fixtures[i];n->runtime_cng_probe=rp[i][0];
   n->scq_session.self=&n->scq_session;n->scq_session.owner=&terminal_owner;n->scq_session.transport.self=&n->scq_session.transport;n->scq_session.transport.owner=&terminal_owner;
   n->scq_session.profile=&n->scq_profile_data;n->scq_session.dispatch=&n->scq_dispatch;n->scq_session.source=&n->scq_admin;
   n->scq_session.read_act=&n->scq_read_diagnostic;n->scq_read_diagnostic.self=&n->scq_read_diagnostic;n->scq_read_diagnostic.owner=&terminal_owner;n->scq_read_diagnostic.source=&n->scq_admin;
   C(papacc_configuration_personal_scq_controller_inconclusive(&terminal_owner)==PAPACC_RESULT_OK);
   terminal_owner.pending_commit.stop=1;C(papacc_configuration_personal_scq_controller_inconclusive(&terminal_owner)!=PAPACC_RESULT_OK);terminal_owner.pending_commit.stop=0;
   n->scq_session.transport.legs&=~16U;C(papacc_configuration_personal_scq_controller_inconclusive(&terminal_owner)!=PAPACC_RESULT_OK);n->scq_session.transport.legs|=16U;
   n->machine_fixture.key.diagnostic_control_confirmed=0;C(papacc_configuration_personal_scq_controller_inconclusive(&terminal_owner)!=PAPACC_RESULT_OK);n->machine_fixture.key.diagnostic_control_confirmed=1;
   n->scq_profile_data.read_diagnostic=0;C(papacc_configuration_personal_scq_controller_inconclusive(&terminal_owner)!=PAPACC_RESULT_OK);
  }else C(!e);
  C(!chain_until(i,1,32U));
  if(i==16U){
   PAPACC_PERSONAL_CNG_PROBE saved=rp[i][1];DWORD saved_leg=f->recipient.transport.legs;
   C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);
   f->recipient.transport.legs&=~32U;C(scqr_closed_terminal_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop,1U)!=0);f->recipient.transport.legs=saved_leg;
   f->recipient.transport.close_confirmed=0;C(scqr_closed_terminal_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop,1U)!=0);f->recipient.transport.close_confirmed=1U;
   f->runtime_profile.read_diagnostic=0;C(scqr_closed_terminal_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop,1U)!=0);f->runtime_profile.read_diagnostic=1U;
   f->recipient.transport.pending=1U;C(scqr_closed_terminal_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop,1U)!=0);f->recipient.transport.pending=0;
   rp[i][1].close_error=ERROR_INVALID_HANDLE;C(scqr_closed_terminal_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop,1U)!=0);rp[i][1]=saved;
   {char *args[]={"PapinhoAccelerator","service","host","scq-closed"};active_f=f;active_r=&b->origin;active_i=&b->inputs;scenario=cs_stop=cs_armed=cs_premature=cs_running=0;
    memset(&host,0,sizeof(host)); /* independent modeled SCM process, not a retry */
    mpeer=f->plan.administrator.pid;e=(DWORD)papacc_server_service_dispatch(4,args,read_inconclusive_service_run);
    C(e==ERROR_NOT_FOUND&&host.life.phase==PS_STOPPED&&!cs_running&&!cs_armed&&!host.closed_ready&&!papacc_server_service_runtime_ready());}
   C(!scqkf_close(&key_fixtures[i],&f->administrator_owner));closed++;continue;
  }
  mpeer=f->plan.administrator.pid;C(!scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop));
  if(i==0U){char *args[]={"PapinhoAccelerator","service","host","scq-closed"};active_f=f;active_r=&b->origin;active_i=&b->inputs;scenario=cs_stop=cs_armed=cs_premature=cs_running=0;
   e=(DWORD)papacc_server_service_dispatch(4,args,read_service_run);if(e||host.life.phase!=PS_STOPPED||papacc_server_service_runtime_ready())printf("SERVICE_READ_MODEL e=%lu phase=%u closed=%u running=%u\n",(unsigned long)e,host.life.phase,host.closed_ready,cs_running);C(!e);C(host.life.phase==PS_STOPPED&&!papacc_server_service_runtime_ready());}
  else {C(!scqs_close(&f->recipient,&f->runtime_owner,1U));C(!scqr_close(&b->origin,&f->runtime_owner));C(!scqs_close(&f->controller,&f->administrator_owner,1U));}
  C(!scqkf_close(&key_fixtures[i],&f->administrator_owner));C(!key_fixtures[i].key.created&&!f->controller.transport.delivery_process);closed++;
 }
 printf("READ_S1_SD_FORM=%s\n",observed_sd?"D01F01FF":"GENERIC_ALL");
 C(closed==5U);C(!LocalFree(virtual_sid)&&!LocalFree(sam_sid)&&!LocalFree(service_sid));
 printf("PASS MACHINE_READ_COLD_CHAIN cases=27 closed=%u checks=%u ACTUAL_FIVE_QUERY_DELIVERY_AND_CA_P_RESULT_RECEIPT_CLOSE_SERVICE_LIFECYCLE NO_REAL_FILE_TOKEN_PROVIDER_KEY_ENDPOINT_SCM_START\n",closed,checks-start);return 0;
}
