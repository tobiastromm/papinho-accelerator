/* Actual ingress body; each native acquisition/validation boundary injected
 * separately. Does not replace pipeline by copying expected content digests. */
#define PAPACC_QUERY_COLD_ENTRY preserved_sqf_cold_chain_main
#include "scq_query_cold_chain_test.c"
#undef PAPACC_QUERY_COLD_ENTRY
#include "../apps/server/personal_scq_start_plan_data_win32.inc"
typedef struct SQFI_NATIVE {
 PAPACC_SCQ_OWNER_DECISION scq_selection;PAPACC_PERSONAL_RUNTIME_IDENTITY runtime_identity;
 PAPACC_SCQ_SESSION_DRIVER scq_session;PAPACC_PERSONAL_SCQ_INPUTS scq_inputs;
 PAPACC_SCQ_START_ASSURANCE scq_start_assurance;PAPACC_SCQ_PROFILE_DATA scq_profile_data;
 PAPACC_PERSONAL_SCQ_RECEIVER scq_receiver;PAPACC_PERSONAL_SCQ_DISPATCH scq_dispatch;
 PAPACC_SCQ_RECIPIENT_ORIGIN scq_recipient;PAPACC_PERSONAL_INSTALL_PATHS install_paths;
 PS_SCQ_CARRIER scq_carrier;DWORD scq_carrier_captured,error;
} SQFI_NATIVE;
typedef struct SQFI_OWNER {
 DWORD writer,stopped,unresolved,initialized,published;
 struct {SQFI_NATIVE personal_native;volatile LONG stop;DWORD active,thread,personal_ca,personal_ca_candidate;} pending_commit;
} SQFI_OWNER;
static SQFI_OWNER sqfi_owner;
static DWORD sqfi_fault,sqfi_seen[20],sqfi_sends,sqfi_frozen,sqfi_code;
static int sqfi_waiteligible,sqfi_initialeligible,sqfi_service=1;
static PAPACC_SCQ_PLAN_DATA sqfi_plan;
static DWORD sqfi_step(DWORD stage){sqfi_seen[stage]++;return sqfi_fault==stage?ERROR_ACCESS_DENIED:0;}
static DWORD sqfi_runtime(void){SQFI_NATIVE *n=&sqfi_owner.pending_commit.personal_native;n->runtime_identity.phase=1U;n->runtime_identity.pid=GetCurrentProcessId();return sqfi_step(2U);}
static DWORD sqfi_open(void){sqfi_owner.pending_commit.personal_native.scq_session.transport.phase=3U;return sqfi_step(5U);}
static DWORD sqfi_candidates(PAPACC_SCQ_PLAN_DATA *p){*p=sqfi_plan;return sqfi_step(7U);}
static DWORD sqfi_paths(void){sqfi_owner.pending_commit.personal_native.install_paths.phase=2U;return sqfi_step(8U);}
static DWORD sqfi_observe(PAPACC_SCQ_RUNTIME_OBSERVATION *v){v->known=PAPACC_SCQ_OBS_COMPLETE;v->peer=sqfi_plan.runtime;return sqfi_step(11U);}
static DWORD sqfi_send(void){sqfi_sends++;return sqfi_code;}
static unsigned sqfi_hash_calls,sqfi_g3_calls;
static int sqfi_policy_hash(BYTE *d){memset(d,(++sqfi_hash_calls)&1U,32);return 1;}
#define store_data_hash(h,p,n,d) ((void)(h),(void)(p),(void)(n),sqfi_policy_hash(d))
#define scqk_policy_data(a,b) ((void)(a),(void)(b),0)
#define papacc_personal_runtime_capture_machine_read_closed(...) sqfi_runtime()
#define personal_scq_inputs_capture_machine_read(...) sqfi_step(9U)
#define papacc_personal_paths_selected_machine_read(...) sqfi_paths()
#define scqb_open_query_variant(c,o,carrier,r,run,pf,rc,d,s,read) ((void)(r),sqfi_open())
#define PAPACC_CONFIGURATION_COMMIT SQFI_OWNER
#define PAPACC_PERSONAL_NATIVE_CONTEXT SQFI_NATIVE
#define PAPACC_PERSONAL_SCM_HOST_GATE 1
#define papacc_configuration_personal_scq_ingress_continue sqfi_ingress
#define papacc_server_service_scq_preparation_current() sqfi_service
#define papacc_configuration_personal_scq_initial_wait_current sqfi_wait
#define scqf_wait_current(...) sqfi_waiteligible
#define scqb_initial_wait(...) sqfi_initialeligible
#define papacc_personal_runtime_capture_transport_closed(...) sqfi_runtime()
#define scqg_selected_p1(a) memset((a),0,sizeof(*(a)))
#define scqg_plan_data_mode(a,b,c,d) ((void)(a),(void)(b),(void)(c),(void)(d),(++sqfi_g3_calls==1U?0:sqfi_step(3U)))
#define papacc_store_hash(h,p,n,d) ((void)(h),(void)(p),(void)(n),memset((d),0,32),1)
#define personal_scq_query_resources(...) sqfi_step(4U)
#define scqb_open_query(c,o,carrier,r,run,pf,rc,d,s) ((void)(r),sqfi_open())
#define scqb_read(...) sqfi_step(6U)
#define scqb_candidates(c,o,p,s) sqfi_candidates(p)
#define scqb_existing_target_query(...) 1
#define papacc_personal_paths_selected(...) sqfi_paths()
#define papacc_personal_paths_new(...) ERROR_NOT_SUPPORTED
#define personal_scq_inputs_capture_transport(...) sqfi_step(9U)
#define personal_scq_inputs_contents(...) sqfi_step(10U)
#define papacc_personal_runtime_observe_data(r,v,s) sqfi_observe(v)
#define personal_scq_inputs_image_data(...) sqfi_step(12U)
#define scqb_query_image(...) sqfi_step(13U)
#define scqb_bind_query(...) sqfi_step(14U)
#define scqb_original_deadline(...) sqfi_step(15U)
#define scqr_first_prepare(...) sqfi_step(16U)
#define scqb_promote(...) sqfi_step(17U)
#define papacc_configuration_personal_scq_transport_continue(...) sqfi_step(sqfi_owner.pending_commit.personal_native.scq_session.phase==3U?19U:18U)
#define scqf_freeze(t,s,e) do{if(!sqfi_frozen){sqfi_frozen=(s);sqfi_code=(e);}(t)->failure_stage=sqfi_frozen;}while(0)
#define scqf_freeze_inputs(t,s,e,c) scqf_freeze(t,s,e)
#define scqf_send(...) sqfi_send()
#define scqt_owned(...) 0
#include "../apps/server/personal_scq_ingress_win32.inc"
int main(void)
{
 unsigned i,j,initial=checks;DWORD e;
 memset(&sqfi_plan,0,sizeof(sqfi_plan));sqfi_plan.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;
 for(i=2U;i<=19U;i++){
  SQFI_NATIVE *n=&sqfi_owner.pending_commit.personal_native;
  sqfi_hash_calls=sqfi_g3_calls=0;memset(&sqfi_owner,0,sizeof(sqfi_owner));memset(sqfi_seen,0,sizeof(sqfi_seen));
  sqfi_owner.writer=GetCurrentThreadId();n->scq_carrier_captured=1U;
  n->scq_session.transport.capture_query=1U;n->runtime_identity.closed_preparation=1U;
  sqfi_fault=i;sqfi_sends=sqfi_frozen=sqfi_code=0;
  /* Hash is a boundary success stub; initialize policy/digest buffers in the
   * actual actor explicitly below, not opaque native content success. */
  if(i>=6U){n->runtime_identity.phase=1U;n->scq_session.transport.phase=3U;}
  if(i>=18U)n->scq_session.phase=i==19U?3U:1U;
  e=sqfi_ingress(&sqfi_owner);
  C(e==ERROR_ACCESS_DENIED&&sqfi_owner.unresolved&&n->error==ERROR_ACCESS_DENIED);
  C(sqfi_frozen==i&&sqfi_code==ERROR_ACCESS_DENIED&&sqfi_sends==1U);
  for(j=i+1U;j<=19U;j++)C(!sqfi_seen[j]);
  sqfi_fault=0;C(sqfi_ingress(&sqfi_owner)==ERROR_ACCESS_DENIED&&sqfi_frozen==i&&sqfi_sends==2U);
 }
 /* Actual actor positive order: open first, then all capture/validation
  * boundaries; no receipt/ack is fabricated by this composition test. */
 {SQFI_NATIVE *n=&sqfi_owner.pending_commit.personal_native;
  sqfi_hash_calls=sqfi_g3_calls=0;memset(&sqfi_owner,0,sizeof(sqfi_owner));memset(sqfi_seen,0,sizeof(sqfi_seen));
  sqfi_owner.writer=GetCurrentThreadId();n->scq_carrier_captured=1U;
  n->scq_session.transport.capture_query=1U;n->runtime_identity.closed_preparation=1U;
  sqfi_fault=sqfi_sends=sqfi_frozen=sqfi_code=0;
  C(sqfi_ingress(&sqfi_owner)==ERROR_MORE_DATA&&!sqfi_sends&&!sqfi_owner.unresolved);
  C(sqfi_ingress(&sqfi_owner)==ERROR_MORE_DATA&&!sqfi_sends&&!sqfi_owner.unresolved);
  for(j=2U;j<=17U;j++)C(sqfi_seen[j]);
 }
 /* Same production wait predicate: unresolved never permits initial QUERY,
  * only the specifically valid already-submitted diagnostic write. */
 sqfi_owner.unresolved=1;sqfi_initialeligible=1;sqfi_waiteligible=0;C(!sqfi_wait(&sqfi_owner));
 sqfi_waiteligible=1;C(sqfi_wait(&sqfi_owner));
 sqfi_owner.stopped=1;C(!sqfi_wait(&sqfi_owner));sqfi_owner.stopped=0;
 sqfi_service=0;C(!sqfi_wait(&sqfi_owner));sqfi_service=1;
 sqfi_owner.unresolved=0;sqfi_waiteligible=0;C(sqfi_wait(&sqfi_owner));
 sqfi_initialeligible=0;C(!sqfi_wait(&sqfi_owner));
 printf("PASS SQF1_INGRESS stages=18 checks=%u ACTUAL_BODY_BOUNDARY_FAULTS_NO_DIGEST_COPY NO_NATIVE_QUERY_NO_ENDPOINT_NO_START\n",checks-initial);
 return 0;
}
