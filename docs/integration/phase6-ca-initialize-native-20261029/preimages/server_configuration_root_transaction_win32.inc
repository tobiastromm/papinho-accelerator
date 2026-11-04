#include "../../src/core/ed11_process_profile.h"
/* Same application transaction authority, first dormant commit only.
 * Native persistence stays outside active owner callbacks; no network calls. */
#include <bcrypt.h>
static int root_snapshot_model_equal(const PAPACC_CONFIG_VIEW *,const PAPACC_CONFIG_VIEW *);
static int root_dormant_owned(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
 if(!preparation_owned(o))return 0;t=&o->configuration_preparation;
 return t->state==PAPACC_ROOT_PREPARATION_PARTIAL&&t->invocation==o->invocation&&
  (!t->dormant_applied||!o->config.site_tls.present||
   (o->pst_system_trust&&o->pst_trust_source.kind==PST_TRUST_SOURCE_SYSTEM&&
    !o->pst_trust_source.anchors&&!o->pst_trust_source.anchor_count&&
    configuration_c3_manager.initialized&&configuration_c3_manager.storage==configuration_c3_connections&&
    configuration_c3_manager.capacity==2U&&!configuration_c3_manager.count&&
    papacc_site_tls_catalog_validate(&o->config.site_tls)==PAPACC_RESULT_OK))&&
  root_policy_model_valid(o)&&root_policy_inputs_valid(t)&&t->policy.prepared&&t->policy.from_model&&
  memcmp(&t->policy.configured_model,&t->model.config,sizeof(t->model.config))==0&&
  t->participants.listener_prepared&&t->participants.capabilities_prepared&&t->participants.c2_prepared&&
  t->participants.invocation==o->invocation&&t->provider_connection&&t->provider_result==PST_RESULT_OK&&
  o->security_composition.state==PAPACC_SECURITY_COMPOSITION_READY&&
  !o->network.is_active&&!o->network.tcp_platform.initialized&&!o->network.listener_set.count&&
  !o->io_loop.initialized&&!o->io_loop.c2_owner&&!o->secure_io.initialized&&
  !o->secure_io.scheduler.initialized&&!o->acceptor.initialized&&
  !o->acceptor.connection_manager.initialized;
}
PAPACC_RESULT papacc_server_launcher_configuration_apply_dormant(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
 if(!root_dormant_owned(o))return PAPACC_RESULT_INVALID_STATE;t=&o->configuration_preparation;
if(t->dormant_applied||(o->configuration_commit.initialized&&!root_recovery_candidate_matches(o))||t->normative_committed||
    t->combined_reserved>PAPACC_ED11_PROCESS_LIMIT)return PAPACC_RESULT_INVALID_STATE;
 if(t->source.state==PAPACC_SECURITY_SOURCE_PREPARED){
  result=papacc_server_security_source_commit(&t->source);if(result!=PAPACC_RESULT_OK)return result;
 }
 if(t->source.state==PAPACC_SECURITY_SOURCE_COMMITTED){
  result=papacc_server_security_source_publish(&t->source);if(result!=PAPACC_RESULT_OK)return result;
 }
 result=papacc_server_c2_configuration_apply(&configuration_c2_owner,&configuration_c2,
  &t->model,&t->policy_inputs,&t->source,&o->security_composition,t->combined_reserved);
 if(result==PAPACC_RESULT_OK){t->dormant_applied=PAPACC_TRUE;result=configuration_site_prepare(o);}
 return result;
}
PAPACC_RESULT papacc_server_launcher_configuration_transaction_prepare_store(
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const char *path,PAPACC_U64 expected)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
 if(!root_dormant_owned(o))return PAPACC_RESULT_INVALID_STATE;t=&o->configuration_preparation;
 /* No live reload or existing store adoption by this first-commit entry. */
 if(expected||o->configuration_commit.initialized||t->normative_committed||!t->dormant_applied||
    papacc_server_c2_configuration_validate(&configuration_c2_owner)!=PAPACC_RESULT_OK)
  return PAPACC_RESULT_INVALID_STATE;
 result=t->model.config.site_tls.present?papacc_configuration_commit_init05(&o->configuration_commit,path):
 t->model.config.policy.representation==PAPACC_ADMIN_REPRESENTATION_04?
  papacc_configuration_commit_init04(&o->configuration_commit,path):papacc_configuration_commit_init03(&o->configuration_commit,path);
 if(result!=PAPACC_RESULT_OK)return result;
 result=t->model.config.site_tls.present?papacc_configuration_prepare05(&o->configuration_commit,&t->model.config,expected):
 t->model.config.policy.representation==PAPACC_ADMIN_REPRESENTATION_04?
  papacc_configuration_prepare04(&o->configuration_commit,&t->model.config,expected):papacc_configuration_prepare03(&o->configuration_commit,&t->model.config,expected);
 if(result!=PAPACC_RESULT_OK){
  if(papacc_configuration_stop(&o->configuration_commit)==PAPACC_RESULT_OK&&
     papacc_configuration_finish(&o->configuration_commit)==PAPACC_RESULT_OK)
   memset(&o->configuration_commit,0,sizeof(o->configuration_commit));
  else (void)papacc_server_launcher_configuration_preparation_retain(o);
 }
 return result;
}
static PAPACC_RESULT configuration_root_commit(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_CONFIGURATION_COMMIT *c;PAPACC_RESULT result;
 if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;t=&o->configuration_preparation;c=&o->configuration_commit;
 if(!t->participants.c2_prepared)return PAPACC_RESULT_INVALID_STATE;
 if(!root_dormant_owned(o)||!t->dormant_applied||!c->initialized||!c->prepared)
  return PAPACC_RESULT_NOT_SUPPORTED;
 if(t->normative_committed||t->source.borrowers!=1U||
    !root_snapshot_model_equal(&configuration_c2_owner.configuration.view,&t->model)||
    papacc_server_c2_configuration_validate(&configuration_c2_owner)!=PAPACC_RESULT_OK)
  return PAPACC_RESULT_INVALID_STATE;
 /* All administrative apply facts hold behind closed admission. BODY0.3
  * persists policy/context/reference provenance, not a live Session epoch. */
 result=papacc_configuration_commit(c);
 if(result==PAPACC_RESULT_OK)t->normative_committed=PAPACC_TRUE;
 else if(c->unresolved){
  if(c->committed_pending)t->normative_committed=PAPACC_TRUE;
  (void)papacc_server_launcher_configuration_preparation_retain(o);
 }
 return result;
}
/* Called under the existing configuration lock AFTER successful publish.
 * Copies actual R/A/P; never issues epoch or changes operational authority. */
static void interface_bind_committed_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t=&o->configuration_preparation;
 PAPACC_CONFIGURATION_COMMIT *c=&o->configuration_commit;
 PAPACC_INTERFACE_CURRENT_BINDING *b=&t->participants.interface_current;
 PAPACC_INTERFACE_SNAPSHOT_COPY *copy=&t->participants.interface_snapshot;
 const PAPACC_CONFIG_VIEW *v=&c->slots[c->current].view;
 memset(b,0,sizeof(*b));
 if(!t->normative_committed||!t->administrative_published||!c->published||c->unresolved||c->stopped||
    c->authority_lost||c->recovery_selected||papacc_server_console_win32_stop_requested()==PAPACC_TRUE||
    !t->participants.listener_prepared||t->participants.invocation!=o->invocation||
    copy->present!=PAPACC_TRUE||copy->retained!=PAPACC_FALSE||copy->invocation!=o->invocation||
    copy->preparation_revision!=v->predecessor||!v->revision||
    !c->slots[c->current].context_epoch||!c->slots[c->current].operational_epoch||
    !root_snapshot_model_equal(&t->model,v))return;
 memcpy(b->domain,v->domain,32U);b->invocation=o->invocation;b->preparation_revision=copy->preparation_revision;
 b->revision=v->revision;b->administrative_epoch=c->slots[c->current].context_epoch;
 b->policy_epoch=c->slots[c->current].operational_epoch;b->bound=PAPACC_TRUE;
}
PAPACC_RESULT papacc_server_launcher_configuration_transaction_publish(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_CONFIGURATION_COMMIT *c;
 PAPACC_CONFIG_VIEW *v;PAPACC_U64 epoch;PAPACC_RESULT result;
 if(!root_dormant_owned(o))return PAPACC_RESULT_INVALID_STATE;t=&o->configuration_preparation;c=&o->configuration_commit;
 if(!c->initialized||!t->normative_committed||t->administrative_published||
    c->unresolved||(!c->committed_pending&&!(c->recovery_selected&&c->recovery_guard)))return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&c->lock);
 v=&c->slots[c->recovery_selected?c->current:c->pending].view;
 /* The SAME current-policy authority issues one initial Session-context epoch
  * after durable commit. No caller epoch, revision projection or shadow counter.
  * A later context transition/reload is unsupported, never wrapped/reused. */
 if(!t->current_policy_epoch){
  if(BCryptGenRandom(NULL,(PUCHAR)&epoch,sizeof(epoch),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0){
   LeaveCriticalSection(&c->lock);return PAPACC_RESULT_INTERNAL_ERROR;
  }
  t->current_policy_epoch=epoch|((PAPACC_U64)1U<<63);
 }
 result=papacc_server_c2_configuration_associate(&configuration_c2_owner,v,t->current_policy_epoch);
 if(result==PAPACC_RESULT_OK)result=c->recovery_selected?(c->store02==4?papacc_configuration_recovery_publish04(c):papacc_configuration_recovery_publish03(c)):papacc_configuration_publish(c);
 if(result==PAPACC_RESULT_OK){
  memcpy(t->model.domain,v->domain,32U);t->model.revision=v->revision;t->model.predecessor=v->predecessor;
  c->slots[c->current].context_epoch=t->current_policy_epoch;
  c->slots[c->current].operational_epoch=t->current_policy_epoch;
  t->administrative_published=PAPACC_TRUE;
  interface_bind_committed_current(o);
 }
 LeaveCriticalSection(&c->lock);return result;
}
static int root_snapshot_model_equal(const PAPACC_CONFIG_VIEW *a,const PAPACC_CONFIG_VIEW *b)
{
 PAPACC_SERVER_CONFIG x=a->config,y=b->config;PAPACC_SIZE count=x.bind_selection.interface_persistent_id_count;
 if(count>PAPACC_CONFIG_IDS||count!=y.bind_selection.interface_persistent_id_count)return 0;
 x.bind_selection.interface_persistent_ids=NULL;y.bind_selection.interface_persistent_ids=NULL;
 return memcmp(&x,&y,sizeof(x))==0&&(!count||memcmp(a->ids,b->ids,count*sizeof(a->ids[0]))==0);
}
/* A held pin proves its own lifetime/provenance, never CURRENT alone. */
/* A held pin proves its own lifetime/provenance, never CURRENT alone. */
static int root_snapshot_held(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 PAPACC_CONFIG_VIEW v;PAPACC_U64 administrative,policy;PAPACC_CONFIGURATION_COMMIT *c;
 if(!preparation_owned(o)||!p||p->invocation!=o->invocation||!p->epoch||!p->administrative_epoch)return 0;c=&o->configuration_commit;
 if(papacc_configuration_pin_read(c,&p->pin,&v,&administrative,&policy)!=PAPACC_RESULT_OK)return 0;
 return policy==p->epoch&&administrative==p->administrative_epoch&&v.revision==p->view.revision&&v.predecessor==p->view.predecessor&&
  !memcmp(v.domain,p->view.domain,32)&&papacc_configuration_issuer_projection06(&v.config,&p->view.config)&&
  p->issuer_present==(c->issuer_snapshot_valid[p->pin.index]?PAPACC_TRUE:PAPACC_FALSE)&&
  (!p->issuer_present||p->issuer_revision==c->slots[p->pin.index].issuer_revision);
}
/* CURRENT operational projection is checked independently of the held R.
 * Issuer-only pending bytes cannot alter P or the selected operational graph. */
static int root_operational_reference_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
 if(!root_snapshot_held(o,p))return 0;c=&o->configuration_commit;t=&o->configuration_preparation;
 if(!t->normative_committed||!t->administrative_published||t->state!=PAPACC_ROOT_PREPARATION_PARTIAL||
    !t->current_policy_epoch||!c->published||c->stopped||c->unresolved||c->recovery_selected||c->pending_commit.active||c->authority_lost||
    p->epoch!=t->current_policy_epoch||c->slots[c->current].operational_epoch!=t->current_policy_epoch||
    !c->slots[c->current].context_epoch||t->model.revision!=c->slots[c->current].view.revision||
    memcmp(t->model.domain,c->slots[c->current].view.domain,32)||
    configuration_c2_owner.configuration.state!=PAPACC_C2_CONFIG_CURRENT||
    configuration_c2_owner.configuration.epoch!=t->current_policy_epoch||
    papacc_server_c2_configuration_authority_current(&configuration_c2_owner)!=PAPACC_RESULT_OK)return 0;
 if(!papacc_configuration_issuer_projection06(&p->view.config,&c->slots[c->current].view.config)||
    !papacc_configuration_issuer_projection06(&o->config,&c->slots[c->current].view.config)||
    !papacc_configuration_issuer_projection06(&t->model.config,&c->slots[c->current].view.config)||
    !papacc_configuration_issuer_projection06(&configuration_c2_owner.configuration.view.config,&c->slots[c->current].view.config))return 0;
 if(c->committed_pending&&(!o->issuer_projection_validated||
    o->issuer_expected_revision!=c->slots[c->current].view.revision||o->issuer_expected_policy_epoch!=t->current_policy_epoch||
    !c->issuer_snapshot_valid[c->pending]||!papacc_configuration_issuer_projection06(&c->slots[c->pending].view.config,&c->slots[c->current].view.config)))return 0;
 return 1;
}
static int root_snapshot_live(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 PAPACC_CONFIGURATION_COMMIT *c=&o->configuration_commit;
 return !c->committed_pending&&root_operational_reference_current(o,p)&&p->pin.index==c->current&&
  p->view.revision==c->slots[c->current].view.revision&&p->administrative_epoch==c->slots[c->current].context_epoch;
}
PAPACC_RESULT papacc_server_launcher_configuration_current_pin(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ROOT_CURRENT_PIN *out)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
 if(!out||out->pin.ticket)return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 if(o!=&root||papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE||!o->configuration_commit.initialized)
  return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration_commit;t=&o->configuration_preparation;EnterCriticalSection(&c->lock);
 if(!t->administrative_published||!t->normative_committed||t->state!=PAPACC_ROOT_PREPARATION_PARTIAL||c->slots[c->current].operational_epoch!=t->current_policy_epoch||!c->slots[c->current].context_epoch){LeaveCriticalSection(&c->lock);return PAPACC_RESULT_INVALID_STATE;}
 result=papacc_configuration_pin(c,&out->pin,&out->view);
 if(result==PAPACC_RESULT_OK){out->epoch=c->slots[c->current].operational_epoch;out->administrative_epoch=c->slots[c->current].context_epoch;out->invocation=o->invocation;out->issuer_present=c->issuer_snapshot_valid[c->current]?PAPACC_TRUE:PAPACC_FALSE;out->issuer_revision=out->issuer_present?c->slots[c->current].issuer_revision:0;}
 LeaveCriticalSection(&c->lock);return result;
}
PAPACC_BOOL papacc_server_launcher_configuration_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 int live;PAPACC_CONFIGURATION_COMMIT *c;
 if(o!=&root||!p||papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE||!o->configuration_commit.initialized)return PAPACC_FALSE;
 c=&o->configuration_commit;EnterCriticalSection(&c->lock);live=root_snapshot_live(o,p);LeaveCriticalSection(&c->lock);
 return live?PAPACC_TRUE:PAPACC_FALSE;
}
#if defined(PAPACC_ED11_WINDOWS_3MIB) && defined(PAPACC_OFFLINE_D3_WORKER)
PAPACC_RESULT papacc_server_launcher_configuration_v3_evidence_preflight(
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *pin)
{
 PAPACC_RESULT data_result;
 if(o!=&root||!pin)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!preparation_owned(o)||papacc_server_console_win32_stop_requested()==PAPACC_TRUE||
 papacc_server_launcher_configuration_current(o,pin)!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 data_result=papacc_configuration_v3_checkpoint_match_data(&o->configuration_commit,&pin->pin);
 if(data_result!=PAPACC_RESULT_OK)return data_result;
 /* Native v3 commit/CP3 publication and full protected predecessor/disposition
  * provenance have no qualified producer yet. Do not infer them from DATA,
  * epochs or the old-format CURRENT guard. No opaque E0 object is fabricated. */
 return PAPACC_RESULT_NOT_SUPPORTED;
}
#endif
static int interface_report_overlaps(const PAPACC_INTERFACE_CURRENT_REPORT *out,const void *object,PAPACC_SIZE size)
{
 uintptr_t a=(uintptr_t)(const void *)out,b=(uintptr_t)object;
 return a>=b?a-b<size:b-a<sizeof(*out);
}
PAPACC_RESULT papacc_server_launcher_configuration_interface_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,
 const PAPACC_ROOT_CURRENT_PIN *pin,PAPACC_INTERFACE_CURRENT_REPORT *out)
{
 const PAPACC_ROOT_TRANSACTION_PARTICIPANTS *p;const PAPACC_INTERFACE_CURRENT_BINDING *b;
 if(!out||interface_report_overlaps(out,&root,sizeof(root))||
    interface_report_overlaps(out,&configuration_c2_owner,sizeof(configuration_c2_owner))||
    interface_report_overlaps(out,&configuration_c2_participant,sizeof(configuration_c2_participant))||
    (o&&interface_report_overlaps(out,o,sizeof(*o)))||
    (pin&&interface_report_overlaps(out,pin,sizeof(*pin))))return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 if(!preparation_owned(o)||!pin||papacc_server_console_win32_stop_requested()==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 /* Caller output plus the existing conservative synchronous-copy allowance;
  * no quota/credit is changed and insufficient capacity fails closed. */
 if(sizeof(*out)+16384U>PAPACC_LAUNCHER_MANAGED_LIMIT||
    o->configuration_preparation.combined_reserved>PAPACC_LAUNCHER_MANAGED_LIMIT-sizeof(*out)-16384U)
  return PAPACC_RESULT_LIMIT_EXCEEDED;
 if(papacc_server_launcher_configuration_current(o,pin)!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 p=&o->configuration_preparation.participants;b=&p->interface_current;
 if(!p->listener_prepared||p->invocation!=o->invocation||b->bound!=PAPACC_TRUE||
    p->interface_snapshot.present!=PAPACC_TRUE||p->interface_snapshot.retained!=PAPACC_FALSE||
    b->invocation!=o->invocation||p->interface_snapshot.invocation!=b->invocation||
    p->interface_snapshot.preparation_revision!=b->preparation_revision||
    memcmp(b->domain,pin->view.domain,32U)||b->revision!=pin->view.revision||
    b->administrative_epoch!=pin->administrative_epoch||b->policy_epoch!=pin->epoch)return PAPACC_RESULT_INVALID_STATE;
 out->binding=*b;out->current_bound=PAPACC_TRUE;
 if(papacc_server_console_win32_stop_requested()==PAPACC_TRUE||
    papacc_server_launcher_configuration_current(o,pin)!=PAPACC_TRUE){
  memset(out,0,sizeof(*out));return PAPACC_RESULT_INVALID_STATE;
 }
 /* No topology revision/continuity proof exists in the current backend.
  * Stored CURRENT correlation is never promoted to RESOLVED. */
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_current_unpin(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 if(o!=&root||!p||p->invocation!=o->invocation||!o->configuration_commit.initialized)return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_unpin(&o->configuration_commit,&p->pin,0);
}
static PAPACC_RESULT configuration_dormant_unapply(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_RESULT result;
 if(!o->configuration_preparation.dormant_applied)return PAPACC_RESULT_OK;
 result=papacc_server_c2_configuration_unapply(&configuration_c2_owner);
 if(result==PAPACC_RESULT_OK)o->configuration_preparation.dormant_applied=PAPACC_FALSE;
 return result;
}
PAPACC_RESULT papacc_server_launcher_configuration_transaction_abort(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_RESULT result;
 if(!preparation_owned(o)||o->configuration_preparation.normative_committed)return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration_commit;
 if(c->initialized){
  if(c->unresolved)return PAPACC_RESULT_INVALID_STATE;
  if(c->prepared){result=papacc_configuration_abort(c);if(result!=PAPACC_RESULT_OK)return result;}
  result=papacc_configuration_stop(c);if(result==PAPACC_RESULT_OK)result=papacc_configuration_finish(c);
  if(result!=PAPACC_RESULT_OK)return result;
 }
 return configuration_preparation_cleanup(o,PAPACC_FALSE);
}
PAPACC_RESULT papacc_server_launcher_configuration_transaction_close(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_CONFIGURATION_COMMIT *c;PAPACC_RESULT result;
 if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;t=&o->configuration_preparation;c=&o->configuration_commit;
 if(o->listener_composition.pin.pin.ticket||!t->normative_committed||!t->administrative_published||!c->initialized)return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&c->lock);t->administrative_published=PAPACC_FALSE;
 result=papacc_configuration_stop(c);LeaveCriticalSection(&c->lock);
 if(result==PAPACC_RESULT_OK)result=papacc_configuration_finish(c);
 if(result!=PAPACC_RESULT_OK){(void)papacc_server_launcher_configuration_preparation_retain(o);return result;}
 return configuration_preparation_cleanup(o,PAPACC_TRUE);
}

PAPACC_RESULT papacc_server_launcher_configuration_restart_guard03(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_NORMAL_RESTART_WITNESS *w)
{
 if(!preparation_owned(o)||o->configuration_preparation.state!=PAPACC_ROOT_PREPARATION_EMPTY)
  return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_recovery_guard03(&o->configuration_commit,w);
}
PAPACC_RESULT papacc_server_launcher_configuration_restart_publish03(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_CONFIGURATION_COMMIT *c;PAPACC_RESULT result;
 if(!root_dormant_owned(o)||!root_recovery_candidate_matches(o))return PAPACC_RESULT_INVALID_STATE;
 t=&o->configuration_preparation;c=&o->configuration_commit;
 if(!t->dormant_applied||t->normative_committed||t->administrative_published||t->source.borrowers!=1U||
    !root_snapshot_model_equal(&t->model,&c->slots[c->current].view)||
    papacc_server_c2_configuration_validate(&configuration_c2_owner)!=PAPACC_RESULT_OK)
  return PAPACC_RESULT_INVALID_STATE;
 result=papacc_configuration_recovery_validate03(c);if(result!=PAPACC_RESULT_OK)return result;
 /* The original native committed selection, never a new commit, now has its
  * complete same-root reconstructed dormant cohort. No old Session is restored. */
 t->normative_committed=PAPACC_TRUE;
 return papacc_server_launcher_configuration_transaction_publish(o);
}

PAPACC_RESULT papacc_server_launcher_configuration_restart_guard04(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_NORMAL_RESTART_WITNESS *w)
{
 if(!preparation_owned(o)||o->configuration_preparation.state!=PAPACC_ROOT_PREPARATION_EMPTY)
  return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_recovery_guard04(&o->configuration_commit,w);
}
PAPACC_RESULT papacc_server_launcher_configuration_restart_publish04(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_CONFIGURATION_COMMIT *c;PAPACC_RESULT result;
 if(!root_dormant_owned(o)||!root_recovery_candidate_matches(o))return PAPACC_RESULT_INVALID_STATE;
 t=&o->configuration_preparation;c=&o->configuration_commit;
 if(!t->dormant_applied||t->normative_committed||t->administrative_published||t->source.borrowers!=1U||
    !root_snapshot_model_equal(&t->model,&c->slots[c->current].view)||
    papacc_server_c2_configuration_validate(&configuration_c2_owner)!=PAPACC_RESULT_OK)
  return PAPACC_RESULT_INVALID_STATE;
 result=papacc_configuration_recovery_validate04(c);if(result!=PAPACC_RESULT_OK)return result;
 /* The original native committed selection, never a new commit, now has its
  * complete same-root reconstructed dormant cohort. No old Session is restored. */
 t->normative_committed=PAPACC_TRUE;
 return papacc_server_launcher_configuration_transaction_publish(o);
}

static int issuer06_root_output_overlap(const void *o,const void *out,PAPACC_SIZE n)
{ULONG_PTR a=(ULONG_PTR)o,b=(ULONG_PTR)out;return a<=b?b-a<sizeof(root):a-b<n;}
static PAPACC_ROOT_CURRENT_PIN *root_session_reference(const PAPACC_C2_SESSION *s)
{
 unsigned i;for(i=0;i<4;i++)if(s==&configuration_c2_owner.sessions[i])return &root.session_revision_pins[i];return NULL;
}
/* Existing owner, same policy/source participants; no general live policy
 * reload. This route rejects changed operational projection before staging. */
static int issuer_root_live_ready(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
 if(!preparation_owned(o))return 0;c=&o->configuration_commit;t=&o->configuration_preparation;
 return c->initialized&&c->published&&!c->stopped&&!c->unresolved&&!c->recovery_selected&&!c->authority_lost&&
  t->normative_committed&&t->administrative_published&&t->state==PAPACC_ROOT_PREPARATION_PARTIAL&&
  t->dormant_applied&&t->participants.c2_prepared&&t->participants.listener_prepared&&t->participants.capabilities_prepared&&
  t->participants.invocation==o->invocation&&t->current_policy_epoch&&
  c->slots[c->current].operational_epoch==t->current_policy_epoch&&t->model.revision==c->slots[c->current].view.revision&&
  !memcmp(t->model.domain,c->domain,32)&&configuration_c2_owner.configuration.state==PAPACC_C2_CONFIG_CURRENT&&
  configuration_c2_owner.configuration.epoch==t->current_policy_epoch&&
  configuration_c2_owner.manager&&configuration_c2_owner.manager->initialized&&!configuration_c2_owner.manager->stopping&&!o->secure_io.dispatch_active&&
  (!configuration_c2_owner.configuration.io_attached||o->secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_RUNNING)&&
  o->security_composition.state==PAPACC_SECURITY_COMPOSITION_READY&&
  papacc_server_c2_configuration_authority_current(&configuration_c2_owner)==PAPACC_RESULT_OK&&
  root_policy_model_valid(o)&&root_policy_inputs_valid(t)&&t->combined_reserved<=PAPACC_ED11_PROCESS_LIMIT&&
  papacc_configuration_issuer_projection06(&o->config,&t->model.config)&&
  papacc_configuration_issuer_projection06(&t->model.config,&c->slots[c->current].view.config)&&
  papacc_configuration_issuer_projection06(&configuration_c2_owner.configuration.view.config,&c->slots[c->current].view.config);
}
PAPACC_RESULT papacc_server_launcher_configuration_issuer_prepare_live(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_U8 *image,const PAPACC_SERVER_CONFIG *projection,PAPACC_U64 expected,PAPACC_U64 high)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD tip;PAPACC_SIZE pos=256,last=0,used;PAPACC_U32 len;PAPACC_RESULT result;
 if(!image||!projection||!issuer_root_live_ready(o)||o->issuer_projection_validated)return PAPACC_RESULT_INVALID_STATE;c=&o->configuration_commit;
 if(issuer06_root_output_overlap(o,image,PAPACC_ISSUER_IMAGE_BYTES)||c->prepared||c->committed_pending||c->pending_commit.active||!papacc_configuration_issuer_projection06(projection,&c->slots[c->current].view.config)||
    expected!=c->slots[c->current].view.revision)return PAPACC_RESULT_INVALID_STATE;
 if(papacc_issuer_image_validate(image,&v,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||memcmp(v.policy,o->config.policy.identity,32))return PAPACC_RESULT_INVALID_STATE;
 used=(PAPACC_SIZE)(((PAPACC_U32)image[12]<<24)|((PAPACC_U32)image[13]<<16)|((PAPACC_U32)image[14]<<8)|image[15]);
 while(pos<used){last=pos;len=((PAPACC_U32)image[pos+8]<<24)|((PAPACC_U32)image[pos+9]<<16)|((PAPACC_U32)image[pos+10]<<8)|image[pos+11];pos+=len;}
 if(last&&(papacc_issuer_image_find(image,image+last+48,&tip,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||tip.approved_epoch!=o->configuration_preparation.current_policy_epoch))return PAPACC_RESULT_INVALID_STATE;
 result=papacc_configuration_issuer_prepare06(c,image,expected,high);
 if(result==PAPACC_RESULT_OK){o->issuer_expected_revision=expected;o->issuer_expected_policy_epoch=o->configuration_preparation.current_policy_epoch;o->issuer_projection_validated=PAPACC_TRUE;o->issuer_commit_started=PAPACC_FALSE;o->issuer_publication_attempted=PAPACC_FALSE;}
 return result;
}
/* Begin exactly once, then poll on subsequent owner turns. The worker never
 * publishes the model. Stop/unknown must still permit factual completion query. */
PAPACC_RESULT papacc_server_launcher_configuration_issuer_publish_live(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ISSUER_CAS_RESULT *out)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
 PAPACC_ISSUER_CAS_RESULT observed;
 if(!preparation_owned(o)||!out||!o->issuer_projection_validated)return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration_commit;t=&o->configuration_preparation;
 if(issuer06_root_output_overlap(o,out,sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 *out=PAPACC_COMMIT_IDLE;
 if(!o->issuer_commit_started){
  if(!issuer_root_live_ready(o)||c->pending_commit.active||c->committed_pending||
     o->issuer_expected_revision!=c->slots[c->current].view.revision||o->issuer_expected_policy_epoch!=t->current_policy_epoch||
     !c->issuer_stage_prepared||!papacc_configuration_issuer_projection06(&c->slots[c->pending].view.config,&c->slots[c->current].view.config))return PAPACC_RESULT_INVALID_STATE;
  o->issuer_commit_started=PAPACC_TRUE;
  result=papacc_configuration_issuer_begin06(c);
  *out=result==PAPACC_RESULT_OK?PAPACC_COMMIT_PENDING:c->pending_commit.result;
  return result;
 }
 if(c->pending_commit.active){
  result=papacc_configuration_issuer_poll06(c,&observed);*out=observed;
  if(result!=PAPACC_RESULT_OK||observed==PAPACC_COMMIT_PENDING)return result;
 }else{observed=c->pending_commit.result;*out=observed;}
 /* Terminal failure is latched: no second commit or guessed CURRENT. */
 if(observed!=PAPACC_CAS_SUCCESS||!c->committed_pending||!issuer_root_live_ready(o)||
    c->unresolved||c->authority_lost||c->stopped||
    o->issuer_expected_revision!=c->slots[c->current].view.revision||o->issuer_expected_policy_epoch!=t->current_policy_epoch||
    !papacc_configuration_issuer_projection06(&c->slots[c->pending].view.config,&c->slots[c->current].view.config))return PAPACC_RESULT_INVALID_STATE;
 if(o->issuer_publication_attempted)return PAPACC_RESULT_INVALID_STATE;
 o->issuer_publication_attempted=PAPACC_TRUE;
 EnterCriticalSection(&c->lock);
 result=papacc_configuration_issuer_publish06(c);
 if(result==PAPACC_RESULT_OK){
  t->model=c->slots[c->current].view;t->model.config.bind_selection.interface_persistent_ids=t->model.config.bind_selection.interface_persistent_id_count?t->model.ids:NULL;
  o->issuer_projection_validated=PAPACC_FALSE;o->issuer_commit_started=PAPACC_FALSE;o->issuer_publication_attempted=PAPACC_FALSE;o->issuer_expected_revision=0;o->issuer_expected_policy_epoch=0;
 }
 LeaveCriticalSection(&c->lock);return result;
}

/* Explicit administrative checkpoint AFTER live publication. No caller epoch.
 * Full same-root CURRENT/P/projection gates before native worker begins. */
PAPACC_RESULT papacc_server_launcher_configuration_checkpoint_begin(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 if(!issuer_root_live_ready(o)||o->issuer_projection_validated||
  o->configuration_commit.slots[o->configuration_commit.current].view.revision!=o->configuration_preparation.model.revision)
  return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_checkpoint_begin07(&o->configuration_commit);
}
PAPACC_RESULT papacc_server_launcher_configuration_checkpoint_poll(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ISSUER_CAS_RESULT *out)
{
 if(!preparation_owned(o)||!out||issuer06_root_output_overlap(o,out,sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 /* Completion facts remain collectable after Stop; no new publication. */
 return papacc_configuration_checkpoint_poll07(&o->configuration_commit,out);
}

/* Same normative application's durable owner. Staging only; no live/current
 * issuer publication until a new C2 association lifetime is proven. */
static int issuer_root_dormant_ready(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;unsigned i;
 if(!root_dormant_owned(o))return 0;t=&o->configuration_preparation;
 /* Keep the existing application guard: component snapshot retention alone
  * does not establish active C2/session publication lifetime. */
 for(i=0;i<PAPACC_CONFIG_SLOTS;i++)if(o->configuration_commit.slots[i].readers||o->configuration_commit.slots[i].retained)return 0;
 return t->normative_committed&&t->administrative_published&&t->current_policy_epoch&&
  t->source.borrowers==1U&&configuration_c2_owner.configuration.epoch==t->current_policy_epoch&&
  !configuration_c2_owner.configuration.io_attached&&
  papacc_server_c2_configuration_validate(&configuration_c2_owner)==PAPACC_RESULT_OK&&
  papacc_configuration_current(&o->configuration_commit,&configuration_c2_owner.configuration.view);
}
PAPACC_RESULT papacc_server_launcher_configuration_issuer_prepare_dormant(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_U8 *image,PAPACC_U64 expected,PAPACC_U64 high)
{
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD tip;PAPACC_SIZE pos=256,last=0,used;PAPACC_U32 len;
 if(!image||!issuer_root_dormant_ready(o))return PAPACC_RESULT_INVALID_STATE;
 if(papacc_issuer_image_validate(image,&v,papacc_configuration_issuer_hash06,&o->configuration_commit)!=PAPACC_ISSUER_OK||
  memcmp(v.policy,o->configuration_preparation.policy.configured_model.policy.identity,32))return PAPACC_RESULT_INVALID_STATE;
 used=(PAPACC_SIZE)(((PAPACC_U32)image[12]<<24)|((PAPACC_U32)image[13]<<16)|((PAPACC_U32)image[14]<<8)|image[15]);
 while(pos<used){last=pos;len=((PAPACC_U32)image[pos+8]<<24)|((PAPACC_U32)image[pos+9]<<16)|((PAPACC_U32)image[pos+10]<<8)|image[pos+11];pos+=len;}
 if(last&&(papacc_issuer_image_find(image,image+last+48,&tip,papacc_configuration_issuer_hash06,&o->configuration_commit)!=PAPACC_ISSUER_OK||
  tip.approved_epoch!=o->configuration_preparation.current_policy_epoch))return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_issuer_prepare06(&o->configuration_commit,image,expected,high);
}
PAPACC_RESULT papacc_server_launcher_configuration_issuer_commit_dormant(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_RESULT r;
 if(!issuer_root_dormant_ready(o)||!o->configuration_commit.issuer_stage_prepared)return PAPACC_RESULT_INVALID_STATE;
 r=papacc_configuration_commit(&o->configuration_commit);
 if(r!=PAPACC_RESULT_OK&&o->configuration_commit.unresolved)(void)papacc_server_launcher_configuration_preparation_retain(o);
 /* OK proves only metadata commit. It does not emit epoch or publish issuer. */
 return r;
}
static PAPACC_ISSUER_RESULT issuer_root_port_read(void *ctx,const PAPACC_U8 id[16],PAPACC_ISSUER_VIEW *v,PAPACC_ISSUER_RECORD *record,int *found)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=ctx;PAPACC_U8 image[8192];PAPACC_ISSUER_RESULT r;
 if(!id||!v||!record||!found)return PAPACC_ISSUER_INVALID;memset(v,0,sizeof(*v));memset(record,0,sizeof(*record));*found=0;
 if((!root_dormant_owned(o)&&!issuer_root_live_ready(o))||!o->configuration_preparation.normative_committed||!o->configuration_preparation.administrative_published)return PAPACC_ISSUER_DENIED;
 if(o->issuer_projection_validated||o->configuration_commit.pending_commit.active||o->configuration_commit.committed_pending)return PAPACC_ISSUER_PENDING;
 if(o->issuer_port_pending.active==2U)return PAPACC_ISSUER_UNCERTAIN;
 if(o->issuer_port_pending.active==1U)return PAPACC_ISSUER_PENDING;
 if(papacc_configuration_issuer_copy06(&o->configuration_commit,image)!=PAPACC_RESULT_OK)return PAPACC_ISSUER_UNSUPPORTED;
 r=papacc_issuer_image_validate(image,v,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 r=papacc_issuer_image_find(image,id,record,papacc_configuration_issuer_hash06,&o->configuration_commit);
 if(r==PAPACC_ISSUER_OK)*found=1;else if(r!=PAPACC_ISSUER_ABSENT)return r;
 /* P is factual only under the existing live root guards; never substitute
  * A or a revision. Missing key/history/exclusivity proof still denies init. */
 if(issuer_root_live_ready(o)){v->epoch=o->configuration_preparation.current_policy_epoch;v->administrative_epoch=o->configuration_commit.slots[o->configuration_commit.current].context_epoch;v->continuity_profile=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL;}
 return PAPACC_ISSUER_OK;
}
static int issuer_port_same_record(const PAPACC_ISSUER_RECORD *a,const PAPACC_ISSUER_RECORD *b)
{
 return !memcmp(a->request.id,b->request.id,16)&&!memcmp(a->request.device,b->request.device,16)&&
  !memcmp(a->request.csr_digest,b->request.csr_digest,32)&&!memcmp(a->request.key_digest,b->request.key_digest,32)&&
  !memcmp(a->request.predecessor,b->request.predecessor,16)&&a->request.purpose==b->request.purpose&&
  a->request.not_before==b->request.not_before&&a->request.not_after==b->request.not_after&&
  a->state==b->state&&a->counter==b->counter&&a->approved_epoch==b->approved_epoch&&a->published_epoch==b->published_epoch&&
  a->certificate_size==b->certificate_size&&!memcmp(a->authority,b->authority,16)&&!memcmp(a->generation,b->generation,16)&&
  !memcmp(a->policy,b->policy,32)&&!memcmp(a->serial,b->serial,16)&&!memcmp(a->certificate_digest,b->certificate_digest,32);
}
static int issuer_port_same_view(const PAPACC_ISSUER_VIEW *a,const PAPACC_ISSUER_VIEW *b)
{
 return a->revision==b->revision&&a->epoch==b->epoch&&a->administrative_epoch==b->administrative_epoch&&a->high_water==b->high_water&&a->ca_not_after==b->ca_not_after&&
  a->proofs==b->proofs&&a->continuity_profile==b->continuity_profile&&!memcmp(a->root,b->root,32)&&
  !memcmp(a->authority,b->authority,16)&&!memcmp(a->generation,b->generation,16)&&!memcmp(a->prefix,b->prefix,8)&&
  !memcmp(a->policy,b->policy,32)&&!memcmp(a->ca_spki_digest,b->ca_spki_digest,32);
}
#include "server_issuer_publication_v2_win32.inc"
/* Synchronous, lock-held observation of the selected immutable issuer image.
 * Unlike image_credential DATA, this boundary checks the existing root CURRENT.
 * It grants no issuer proof, CA lease, recovery or publication authority. */
static PAPACC_ISSUER_RESULT issuer_root_credential_status(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_U8 digest[32],PAPACC_ISSUER_RECORD *record)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_CONFIG_SLOT *slot;const PAPACC_U8 *image;
 PAPACC_ISSUER_RESULT result=PAPACC_ISSUER_DENIED;
 if(!preparation_owned(o)||!digest||!record)return result;c=&o->configuration_commit;
 if(!c->initialized)return result;EnterCriticalSection(&c->lock);slot=&c->slots[c->current];
 if(!c->published||c->stopped||c->unresolved||c->authority_lost||c->recovery_selected||c->prepared||c->committed_pending||c->pending_commit.active||
  o->issuer_port_pending.active||o->issuer_projection_validated||c->store02!=6||
  !o->configuration_preparation.normative_committed||!o->configuration_preparation.administrative_published||
  !slot->context_epoch||!slot->operational_epoch||slot->operational_epoch!=o->configuration_preparation.current_policy_epoch||
  slot->view.revision!=o->configuration_preparation.model.revision||!c->issuer_snapshot_valid[c->current]||
  slot->issuer_revision!=slot->view.revision||memcmp(c->domain,o->configuration_preparation.model.domain,32)||
  configuration_c2_owner.configuration.state!=PAPACC_C2_CONFIG_CURRENT||configuration_c2_owner.configuration.epoch!=slot->operational_epoch||
  papacc_server_c2_configuration_authority_current(&configuration_c2_owner)!=PAPACC_RESULT_OK||
  !root_policy_model_valid(o)||!root_policy_inputs_valid(&o->configuration_preparation)||
  !papacc_configuration_issuer_projection06(&o->config,&slot->view.config))goto done;
 /* This exact snapshot was validated before the native commit and copied
  * from its confirmed packet. No BCrypt or native observer on dispatch. */
 image=c->issuer_snapshots[c->current];
 if(memcmp(image+32,c->domain,32)||memcmp(image+104,o->config.policy.identity,32)){result=PAPACC_ISSUER_UNCERTAIN;goto done;}
 if(!memcmp(image,"PACIS602",8)&&
  (c->committed_bytes!=16896U||memcmp(c->committed+16384,"PACICP02",8)||
   publication_u64(c->committed+16384+48)!=slot->view.revision||publication_u64(c->committed+16384+64)!=slot->context_epoch||
   publication_u64(c->committed+16384+72)!=slot->operational_epoch||memcmp(c->committed+8192,image,8192)))goto done;
 result=papacc_issuer_credential_snapshot_data(image,digest,record);
done:LeaveCriticalSection(&c->lock);return result;
}
static PAPACC_ISSUER_RESULT issuer_root_port_poll(void *ctx,const PAPACC_ISSUER_VIEW *expected,const PAPACC_ISSUER_RECORD *next,int stopped)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=ctx;PAPACC_ISSUER_PENDING_STORE *p;
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ISSUER_CAS_RESULT outcome;PAPACC_RESULT result;
 PAPACC_U8 image[PAPACC_ISSUER_IMAGE_BYTES];PAPACC_ISSUER_VIEW actual;PAPACC_ISSUER_RECORD record;
 if(!preparation_owned(o)||!expected||!next)return PAPACC_ISSUER_DENIED;
 p=&o->issuer_port_pending;c=&o->configuration_commit;
 if(p->active==2U)return PAPACC_ISSUER_RETAINED;
 if(p->active!=1U)return PAPACC_ISSUER_INVALID;
 /* A different caller cannot advance or discard the owned transaction. */
 if(!issuer_port_same_view(expected,&p->before)||!issuer_port_same_record(next,&p->next))return PAPACC_ISSUER_CONFLICT;
 if(p->continuation)return issuer_publication_poll_v2(o,stopped);
 if(stopped)(void)papacc_configuration_stop(c); /* component Stop; never SCM */
 result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);
 if(c->pending_commit.active)return PAPACC_ISSUER_PENDING;
 if(result!=PAPACC_RESULT_OK||outcome!=PAPACC_CAS_SUCCESS){p->active=2U;c->authority_lost=1;return stopped?PAPACC_ISSUER_STOPPED:outcome==PAPACC_CAS_CONFLICT?PAPACC_ISSUER_CONFLICT:PAPACC_ISSUER_UNCERTAIN;}
 /* Commit alone is not success. Check the root-selected A, preserved P,
  * exact native worker packet and current slot before acknowledging CAS. */
 if(!issuer_root_live_ready(o)||o->issuer_projection_validated||c->committed_pending||c->prepared||c->unresolved||c->authority_lost||
  c->slots[c->current].view.revision!=p->before.revision+1U||!c->slots[c->current].context_epoch||
  c->slots[c->current].context_epoch==o->issuer_port_previous_administrative_epoch||
  c->slots[c->current].operational_epoch!=p->before.epoch||c->slots[c->current].issuer_revision!=p->before.revision+1U||
  !c->issuer_snapshot_valid[c->current]||c->pending_commit.next_bytes!=16384U||
  memcmp(c->committed,c->pending_commit.next,16384U)||
  papacc_configuration_issuer_copy06(c,image)!=PAPACC_RESULT_OK||memcmp(image,c->issuer_snapshots[c->current],sizeof(image))||
  papacc_issuer_image_validate(image,&actual,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||
  actual.revision!=p->before.revision+1U||actual.high_water!=p->high||
  papacc_issuer_image_find(image,p->next.request.id,&record,papacc_configuration_issuer_hash06,c)!=PAPACC_ISSUER_OK||
  !issuer_port_same_record(&record,&p->next)){p->active=2U;c->authority_lost=1;return PAPACC_ISSUER_UNCERTAIN;}
 p->active=0;return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT issuer_root_port_cas(void *ctx,const PAPACC_ISSUER_VIEW *expected,const PAPACC_ISSUER_RECORD *old,const PAPACC_ISSUER_RECORD *next,PAPACC_U64 high,const PAPACC_U8 *der,PAPACC_SIZE size,int publish)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=ctx;PAPACC_U8 image[8192];PAPACC_ISSUER_VIEW actual;PAPACC_ISSUER_RECORD prior;PAPACC_ISSUER_RESULT r;PAPACC_RESULT result;PAPACC_ISSUER_CAS_RESULT outcome;

 if(!expected||!next||!preparation_owned(o))return PAPACC_ISSUER_DENIED;
 if(o->issuer_port_pending.active)return o->issuer_port_pending.active==1U?PAPACC_ISSUER_BUSY:PAPACC_ISSUER_RETAINED;
 if(!issuer_root_live_ready(o)||expected->epoch!=o->configuration_preparation.current_policy_epoch||next->approved_epoch!=expected->epoch)return PAPACC_ISSUER_DENIED;
 if(papacc_configuration_issuer_copy06(&o->configuration_commit,image)!=0)return PAPACC_ISSUER_UNSUPPORTED;
 r=papacc_issuer_image_validate(image,&actual,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 if(publish&&memcmp(image,"PACIS602",8))return PAPACC_ISSUER_UNSUPPORTED;
 actual.epoch=o->configuration_preparation.current_policy_epoch;actual.administrative_epoch=o->configuration_commit.slots[o->configuration_commit.current].context_epoch;actual.continuity_profile=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL;
 if(!issuer_port_same_view(&actual,expected))return PAPACC_ISSUER_CONFLICT;
 r=papacc_issuer_image_find(image,next->request.id,&prior,papacc_configuration_issuer_hash06,&o->configuration_commit);
 if((old&&(r!=0||!issuer_port_same_record(&prior,old)||memcmp(old->request.id,next->request.id,16)))||(!old&&r!=PAPACC_ISSUER_ABSENT))return PAPACC_ISSUER_CONFLICT;
 if(publish){PAPACC_U8 correlation[32];
  if(size||high!=expected->high_water||!old||old->state!=PAPACC_ISSUER_ISSUED_UNPUBLISHED||next->state!=PAPACC_ISSUER_PUBLISHED||next->published_epoch)return PAPACC_ISSUER_CONFLICT;
  /* Same original current image/request and authorized core invocation. The
   * correlation is an identifier, never proof of administrative permission. */
  if(!papacc_configuration_issuer_hash06(&o->configuration_commit,image,sizeof(image),correlation))return PAPACC_ISSUER_UNCERTAIN;
  r=papacc_issuer_image_intent(image,next,correlation,expected->epoch,papacc_configuration_issuer_hash06,&o->configuration_commit);
 }else r=papacc_issuer_image_append(image,next,der,size,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 r=papacc_issuer_image_validate(image,&actual,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 if(actual.high_water!=high)return PAPACC_ISSUER_CONFLICT;
 result=papacc_server_launcher_configuration_issuer_prepare_live(o,image,&o->config,expected->revision,expected->high_water);
 if(result!=PAPACC_RESULT_OK)return result==PAPACC_RESULT_LIMIT_EXCEEDED?PAPACC_ISSUER_LIMIT:PAPACC_ISSUER_DENIED;
 memset(&o->issuer_port_pending,0,sizeof(o->issuer_port_pending));
 o->issuer_port_pending.before=*expected;o->issuer_port_pending.next=*next;o->issuer_port_pending.high=high;o->issuer_port_pending.active=1U;
 o->issuer_port_pending.publish=publish?1U:0U;o->issuer_port_pending.continuation=!memcmp(image,"PACIS602",8)?1U:0U;
 memset(&o->issuer_port_publication,0,sizeof(o->issuer_port_publication));
 o->issuer_port_previous_administrative_epoch=o->configuration_commit.slots[o->configuration_commit.current].context_epoch;
 /* The native staged image has copied DER; core retains its lease/output
  * until this correlation is terminal. Begin once, no waits on owner turns. */
 result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);
 if(result==PAPACC_RESULT_OK&&outcome==PAPACC_COMMIT_PENDING)return PAPACC_ISSUER_PENDING;
 o->issuer_port_pending.active=2U;o->configuration_commit.authority_lost=1;return PAPACC_ISSUER_UNCERTAIN;
}
static PAPACC_ISSUER_RESULT issuer_root_port_certificate(void *ctx,const PAPACC_U8 id[16],const PAPACC_U8 digest[32],PAPACC_U8 *out,PAPACC_SIZE capacity,PAPACC_SIZE *size)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=ctx;PAPACC_U8 image[8192];PAPACC_ISSUER_RECORD r;PAPACC_ISSUER_RESULT result;
 if(!id||!digest||!out||!size||(!root_dormant_owned(o)&&!issuer_root_live_ready(o)))return PAPACC_ISSUER_DENIED;
 if(o->issuer_projection_validated||o->issuer_port_pending.active||o->configuration_commit.committed_pending)return PAPACC_ISSUER_DENIED;
 if(papacc_configuration_issuer_copy06(&o->configuration_commit,image)!=0)return PAPACC_ISSUER_UNSUPPORTED;
 result=papacc_issuer_image_find(image,id,&r,papacc_configuration_issuer_hash06,&o->configuration_commit);if(result)return result;
 if(memcmp(digest,r.certificate_digest,32))return PAPACC_ISSUER_CONFLICT;
 return papacc_issuer_image_certificate(image,id,out,capacity,size,papacc_configuration_issuer_hash06,&o->configuration_commit);
}
static PAPACC_ISSUER_RESULT issuer_root_port_revoke(void *ctx,const PAPACC_ISSUER_VIEW *view,const PAPACC_ISSUER_RECORD *record)
{return papacc_server_launcher_credential_revoke((PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *)ctx,view,record);}
PAPACC_RESULT papacc_server_launcher_configuration_issuer_port(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ISSUER_ROOT_PORT *port)
{
 if(!port)return PAPACC_RESULT_INVALID_ARGUMENT;memset(port,0,sizeof(*port));if(!root_dormant_owned(o)&&!issuer_root_live_ready(o))return PAPACC_RESULT_INVALID_STATE;
 port->ctx=o;port->read=issuer_root_port_read;port->cas=issuer_root_port_cas;port->poll=issuer_root_port_poll;port->copy_certificate=issuer_root_port_certificate;port->revoke_dependents=issuer_root_port_revoke;port->confirm_publication=issuer_publication_confirm_v2;return PAPACC_RESULT_OK;
}
#ifdef PAPACC_OFFLINE_D3_WORKER
#include "server_issuer_x509_win32.h"
PAPACC_ISSUER_RESULT papacc_server_launcher_configuration_issuer_existing_ca_bind(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ISSUER_CNG_BACKEND *b)
{
 if(!preparation_owned(o)||!issuer_root_live_ready(o)||!b||!b->initialized||b->thread!=GetCurrentThreadId()||
  b->custody||b->source_kind||b->original_source_root||b->root.test_only||b->ports.test_only||
  b->root.ctx!=o||b->root.read!=issuer_root_port_read||b->root.cas!=issuer_root_port_cas||b->root.poll!=issuer_root_port_poll||
  b->root.copy_certificate!=issuer_root_port_certificate||b->root.confirm_publication!=issuer_publication_confirm_v2||b->root.revoke_dependents!=issuer_root_port_revoke)
  return PAPACC_ISSUER_DENIED;
 return papacc_configuration_existing_ca_bind(&o->configuration_commit,b);
}
PAPACC_ISSUER_RESULT papacc_server_launcher_configuration_issuer_normal_activate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ISSUER_CNG_BACKEND *b)
{
 if(!preparation_owned(o)||!issuer_root_live_ready(o)||!b||!b->initialized||b->thread!=GetCurrentThreadId()||
  b->original_source_root!=&o->configuration_commit||!b->custody||b->root.test_only||b->ports.test_only||
  b->root.ctx!=o||b->root.read!=issuer_root_port_read||b->root.cas!=issuer_root_port_cas||b->root.poll!=issuer_root_port_poll||
  b->root.copy_certificate!=issuer_root_port_certificate||b->root.confirm_publication!=issuer_publication_confirm_v2||b->root.revoke_dependents!=issuer_root_port_revoke)
  return PAPACC_ISSUER_DENIED;
 /* Original descriptor is latched by the source, never caller-selected
  * attempt/root/epoch. Actual read still denies without NORMAL facts. */
 if(b->source_kind==PAPACC_CA_SOURCE_NEW_INSTALL)return papacc_configuration_personal_ca_activate_normal(&o->configuration_commit,b,b->original_attempt,b->original_authority,b->original_generation);
 if(b->source_kind==PAPACC_CA_SOURCE_EXISTING_CURRENT)return papacc_configuration_existing_ca_activate_normal(&o->configuration_commit,b);
 return PAPACC_ISSUER_DENIED;
}
#else
PAPACC_ISSUER_RESULT papacc_server_launcher_configuration_issuer_normal_activate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,struct PAPACC_ISSUER_CNG_BACKEND *b)
{(void)o;(void)b;return PAPACC_ISSUER_UNSUPPORTED;}
PAPACC_ISSUER_RESULT papacc_server_launcher_configuration_issuer_existing_ca_bind(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,struct PAPACC_ISSUER_CNG_BACKEND *b)
{(void)o;(void)b;return PAPACC_ISSUER_UNSUPPORTED;}
#endif
#if defined(PAPACC_ED11_WINDOWS_3MIB) && defined(PAPACC_OFFLINE_D3_WORKER)
PAPACC_RESULT papacc_server_launcher_configuration_v3_update_hold(
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_U8 *candidate,PAPACC_SIZE bytes,PAPACC_SIZE reserved)
{
 PAPACC_ROOT_CURRENT_PIN base;PAPACC_CONFIGURATION_COMMIT *c;PAPACC_RESULT result,release;
 const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *source=NULL;unsigned session;
 if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;c=&o->configuration_commit;
 /* Explicit version dispatch in the SAME canonical preparation entry. */
 if(!candidate||bytes<PC_ROOT8_V3_MIN||bytes>PC_ROOT8_V4_MAX||
 (ULONG_PTR)candidate>MAXULONG_PTR-(bytes-1U)||(candidate[8]!=3U&&candidate[8]!=4U)||candidate[9])return PAPACC_RESULT_INVALID_ARGUMENT;
 /* Bounded dormant UPDATE only. Active/retained Session coexistence has no
  * qualified complete budget; do not infer it from the fixed census. */
 if(o->network.is_active||o->network.tcp_platform.initialized||o->io_loop.initialized||
 o->secure_io.initialized||o->acceptor.initialized||o->attachment_state||o->listener_composition.pin.pin.ticket)return PAPACC_RESULT_INVALID_STATE;
 for(session=0;session<4U;session++)if(o->session_revision_pins[session].pin.ticket)return PAPACC_RESULT_INVALID_STATE;
 if(PAPACC_V3_UPDATE_BUSY(c)||papacc_server_console_win32_stop_requested()==PAPACC_TRUE||
 reserved<o->configuration_preparation.combined_reserved||reserved>PAPACC_ED11_PROCESS_LIMIT||
 sizeof(base)>PAPACC_ED11_PROCESS_LIMIT-reserved)return PAPACC_RESULT_INVALID_STATE;
 memset(&base,0,sizeof(base));result=papacc_server_launcher_configuration_current_pin(o,&base);
 if(result!=PAPACC_RESULT_OK)return result;
 if(papacc_server_launcher_configuration_current(o,&base)!=PAPACC_TRUE)result=PAPACC_RESULT_INVALID_STATE;
 else result=candidate[8]==3U?papacc_configuration_v3_update_hold_data(c,&base.pin,candidate,bytes,reserved+sizeof(base),o->invocation):
  papacc_configuration_v4_update_hold_data(c,&base.pin,candidate,bytes,reserved+sizeof(base),o->invocation);
 if(base.pin.ticket){
  release=papacc_configuration_unpin(c,&base.pin,0);
  if(release!=PAPACC_RESULT_OK){
   c->v3_update.predecessor_pin=base.pin;c->v3_update.invocation=o->invocation;
   c->v3_update.phase=PAPACC_V3_UPDATE_RETAINED;return release;
  }
 }
 if(result!=PAPACC_RESULT_OK)return result;
 if(papacc_server_console_win32_stop_requested()==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 /* Keep the PUBLISHED source immutable; only its existing borrower ticket is
  * acquired. This does not resolve arbitrary candidate refs or policy. */
 result=papacc_server_security_source_borrow(&o->configuration_preparation.source,&c->v3_update.source_ticket,&source);
 if(result!=PAPACC_RESULT_OK||!source)return result==PAPACC_RESULT_OK?PAPACC_RESULT_INVALID_STATE:result;
 return PAPACC_RESULT_NOT_SUPPORTED; /* HELD, not RESOLVED/VALIDATED or commit */
}
static PAPACC_RESULT v3_update_current_sources(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_V3_UPDATE_COHORT *u;PAPACC_ROOT_CURRENT_PIN p;PAPACC_RESULT result;unsigned j;
 if(!preparation_owned(o)||papacc_server_console_win32_stop_requested()==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 u=&o->configuration_commit.v3_update;
 if(u->phase!=PAPACC_V3_UPDATE_HELD||u->invocation!=o->invocation||!u->source_ticket||
 papacc_server_security_source_validate(&o->configuration_preparation.source)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<8U;j++)if(o->configuration_preparation.source.tickets[j]==u->source_ticket)break;
 if(j==8U)return PAPACC_RESULT_INVALID_STATE;
 /* Rebuild a view of the SAME owned canonical ticket, never mint a pin/epoch. */
 memset(&p,0,sizeof(p));p.pin=u->predecessor_pin;p.invocation=o->invocation;
 result=papacc_configuration_pin_read(&o->configuration_commit,&p.pin,&p.view,&p.administrative_epoch,&p.epoch);
 if(result!=PAPACC_RESULT_OK)return result;
 p.issuer_present=o->configuration_commit.issuer_snapshot_valid[p.pin.index]?PAPACC_TRUE:PAPACC_FALSE;
 p.issuer_revision=p.issuer_present?o->configuration_commit.slots[p.pin.index].issuer_revision:0;
 if(papacc_server_launcher_configuration_current(o,&p)!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK; /* Existing CURRENT/source only; no candidate approval. */
}
PAPACC_RESULT papacc_server_launcher_configuration_v3_update_approve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_U8 *selected,PAPACC_SIZE bytes)
{
 PAPACC_RESULT result=v3_update_current_sources(o);
 if(result!=PAPACC_RESULT_OK){
  if(o==&root&&GetCurrentThreadId()==o->owner_thread&&o->configuration_commit.v3_update.act.state==PAPACC_V3_ACT_CAPTURED)
   o->configuration_commit.v3_update.act.state=PAPACC_V3_ACT_INVALID;
  return result;
 }
 result=papacc_configuration_v3_update_act_capture(&o->configuration_commit,selected,bytes);
 if(result!=PAPACC_RESULT_OK)return result;
 if(v3_update_current_sources(o)!=PAPACC_RESULT_OK){o->configuration_commit.v3_update.act.state=PAPACC_V3_ACT_INVALID;return PAPACC_RESULT_INVALID_STATE;}
 return PAPACC_RESULT_NOT_SUPPORTED; /* Act captured, issuer UTC/status source absent. */
}
PAPACC_RESULT papacc_server_launcher_configuration_v3_update_validate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_RESULT result=v3_update_current_sources(o);
 if(result!=PAPACC_RESULT_OK){
  if(o==&root&&GetCurrentThreadId()==o->owner_thread&&o->configuration_commit.v3_update.act.state==PAPACC_V3_ACT_CAPTURED)
   o->configuration_commit.v3_update.act.state=PAPACC_V3_ACT_INVALID;
  return result;
 }
 result=papacc_configuration_v3_update_act_revalidate(&o->configuration_commit);
 if(result!=PAPACC_RESULT_OK)return result;
 return PAPACC_RESULT_NOT_SUPPORTED; /* No qualified issuer UTC/status source. */
}
PAPACC_RESULT papacc_server_launcher_configuration_v3_update_commit(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{return papacc_server_launcher_configuration_v3_update_validate(o); /* unresolved never reaches packet11 */}
PAPACC_RESULT papacc_server_launcher_configuration_v3_update_abort(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_V3_UPDATE_COHORT *u;PAPACC_RESULT result;
 /* Retained owner remains reachable for cleanup; Stop does not destroy it. */
 if(o!=&root||GetCurrentThreadId()!=o->owner_thread||!o->configuration_commit.initialized)return PAPACC_RESULT_INVALID_STATE;
 u=&o->configuration_commit.v3_update;
 if(!PAPACC_V3_UPDATE_BUSY(&o->configuration_commit)||u->invocation!=o->invocation)return PAPACC_RESULT_INVALID_STATE;
 if(u->data_active)return PAPACC_RESULT_INVALID_STATE;
 if(u->act.retained_token||u->act.retained_process_token||o->configuration_commit.pending_commit.active||o->configuration_commit.pending_commit.thread||
 o->configuration_commit.pending_commit.transaction||(o->configuration_commit.pending_commit.file&&o->configuration_commit.pending_commit.file!=INVALID_HANDLE_VALUE)||
 o->configuration_commit.pending_commit.close_error||o->configuration_commit.unresolved||o->configuration_commit.committed_pending||
 o->configuration_commit.authority_lost||o->configuration_commit.pending_commit.result!=PAPACC_COMMIT_IDLE||o->configuration_commit.pending_commit.packet_format){
 u->phase=PAPACC_V3_UPDATE_RETAINED;return PAPACC_RESULT_INVALID_STATE;}
 if(u->source_ticket){
  result=papacc_server_security_source_unborrow(&o->configuration_preparation.source,u->source_ticket);
  if(result!=PAPACC_RESULT_OK){u->phase=PAPACC_V3_UPDATE_RETAINED;return result;}u->source_ticket=0;
 }
 return papacc_configuration_v3_update_abort_data(&o->configuration_commit);
}
#endif
