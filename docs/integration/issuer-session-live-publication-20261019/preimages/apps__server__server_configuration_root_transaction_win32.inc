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
    t->combined_reserved>2097152U)return PAPACC_RESULT_INVALID_STATE;
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
  t->administrative_published=PAPACC_TRUE;
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
static int root_snapshot_held(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 PAPACC_CONFIG_VIEW v;PAPACC_U64 epoch;
 if(!preparation_owned(o)||o->configuration_commit.pending_commit.active||o->configuration_commit.authority_lost||!p||p->invocation!=o->invocation||!p->epoch||
    papacc_configuration_pin_read(&o->configuration_commit,&p->pin,&v,&epoch)!=PAPACC_RESULT_OK)return 0;
 return epoch==p->epoch&&v.revision==p->view.revision&&v.predecessor==p->view.predecessor&&
  !memcmp(v.domain,p->view.domain,32)&&root_snapshot_model_equal(&v,&p->view)&&
  root_snapshot_model_equal(&v,&o->configuration_commit.slots[o->configuration_commit.current].view);
}
static int root_snapshot_live(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 PAPACC_CONFIGURATION_COMMIT *c=&o->configuration_commit;PAPACC_ROOT_CONFIGURATION_PREPARATION *t=&o->configuration_preparation;
 unsigned i;PAPACC_CONFIG_VIEW *v=&c->slots[c->current].view;
 if(!t->administrative_published||!t->normative_committed||t->state!=PAPACC_ROOT_PREPARATION_PARTIAL||
    p->invocation!=o->invocation||!p->epoch||p->epoch!=t->current_policy_epoch||
    p->pin.issuer!=c||p->pin.index!=c->current||p->view.revision!=v->revision||
    memcmp(p->view.domain,v->domain,32U)||!root_snapshot_model_equal(&p->view,v))return 0;
 for(i=0U;i<32U;++i)if(c->tickets[i].ticket==p->pin.ticket&&p->pin.ticket&&
    c->tickets[i].serial==p->pin.serial&&c->tickets[i].index==p->pin.index)
  return papacc_configuration_current(c,&p->view)==PAPACC_TRUE;
 return 0;
}
PAPACC_RESULT papacc_server_launcher_configuration_current_pin(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ROOT_CURRENT_PIN *out)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
 if(!out||out->pin.ticket)return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 if(o!=&root||papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE||!o->configuration_commit.initialized)
  return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration_commit;t=&o->configuration_preparation;EnterCriticalSection(&c->lock);
 if(!t->administrative_published||!t->normative_committed||t->state!=PAPACC_ROOT_PREPARATION_PARTIAL){LeaveCriticalSection(&c->lock);return PAPACC_RESULT_INVALID_STATE;}
 result=papacc_configuration_pin(c,&out->pin,&out->view);
 if(result==PAPACC_RESULT_OK){out->epoch=t->current_policy_epoch;out->invocation=o->invocation;}
 LeaveCriticalSection(&c->lock);return result;
}
PAPACC_BOOL papacc_server_launcher_configuration_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *p)
{
 int live;PAPACC_CONFIGURATION_COMMIT *c;
 if(o!=&root||!p||papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE||!o->configuration_commit.initialized)return PAPACC_FALSE;
 c=&o->configuration_commit;EnterCriticalSection(&c->lock);live=root_snapshot_live(o,p);LeaveCriticalSection(&c->lock);
 return live?PAPACC_TRUE:PAPACC_FALSE;
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
 if(!root_dormant_owned(o)||!o->configuration_preparation.normative_committed||!o->configuration_preparation.administrative_published)return PAPACC_ISSUER_DENIED;
 if(papacc_configuration_issuer_copy06(&o->configuration_commit,image)!=PAPACC_RESULT_OK)return PAPACC_ISSUER_UNSUPPORTED;
 r=papacc_issuer_image_validate(image,v,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 r=papacc_issuer_image_find(image,id,record,papacc_configuration_issuer_hash06,&o->configuration_commit);
 if(r==PAPACC_ISSUER_OK)*found=1;else if(r!=PAPACC_ISSUER_ABSENT)return r;
 /* No key/freshness/prefix-history/CURRENT proof exists yet. Leaving epoch
  * and proofs absent is deliberate: core init denies, no implicit authority. */
 return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT issuer_root_port_cas(void *ctx,const PAPACC_ISSUER_VIEW *expected,const PAPACC_ISSUER_RECORD *old,const PAPACC_ISSUER_RECORD *next,PAPACC_U64 high,const PAPACC_U8 *der,PAPACC_SIZE size,int publish)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=ctx;PAPACC_U8 image[8192];PAPACC_ISSUER_VIEW actual;PAPACC_ISSUER_RECORD prior;PAPACC_ISSUER_RESULT r;PAPACC_RESULT result;
 if(publish)return PAPACC_ISSUER_UNSUPPORTED;
 if(!expected||!next||!issuer_root_dormant_ready(o)||expected->epoch!=o->configuration_preparation.current_policy_epoch||next->approved_epoch!=expected->epoch)return PAPACC_ISSUER_DENIED;
 if(papacc_configuration_issuer_copy06(&o->configuration_commit,image)!=0)return PAPACC_ISSUER_UNSUPPORTED;
 r=papacc_issuer_image_validate(image,&actual,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 if(actual.revision!=expected->revision||actual.high_water!=expected->high_water||memcmp(actual.root,expected->root,32)||memcmp(actual.authority,expected->authority,16)||memcmp(actual.generation,expected->generation,16))return PAPACC_ISSUER_CONFLICT;
 r=papacc_issuer_image_find(image,next->request.id,&prior,papacc_configuration_issuer_hash06,&o->configuration_commit);
 if((old&&(r!=0||prior.state!=old->state||prior.counter!=old->counter||memcmp(old->request.id,next->request.id,16)))||(!old&&r!=PAPACC_ISSUER_ABSENT))return PAPACC_ISSUER_CONFLICT;
 r=papacc_issuer_image_append(image,next,der,size,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 r=papacc_issuer_image_validate(image,&actual,papacc_configuration_issuer_hash06,&o->configuration_commit);if(r)return r;
 if(actual.high_water!=high)return PAPACC_ISSUER_CONFLICT;
 result=papacc_server_launcher_configuration_issuer_prepare_dormant(o,image,expected->revision,expected->high_water);
 if(result!=PAPACC_RESULT_OK)return PAPACC_ISSUER_DENIED;
 result=papacc_server_launcher_configuration_issuer_commit_dormant(o);(void)result;
 /* A commit alone cannot acknowledge the full COMMIT/CURRENT/EPOCH port.
  * Never return OK to the signer while normative publication is absent. */
 return PAPACC_ISSUER_UNCERTAIN;
}
static PAPACC_ISSUER_RESULT issuer_root_port_certificate(void *ctx,const PAPACC_U8 id[16],const PAPACC_U8 digest[32],PAPACC_U8 *out,PAPACC_SIZE capacity,PAPACC_SIZE *size)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=ctx;PAPACC_U8 image[8192];PAPACC_ISSUER_RECORD r;PAPACC_ISSUER_RESULT result;
 if(!id||!digest||!out||!size||!root_dormant_owned(o))return PAPACC_ISSUER_DENIED;
 if(papacc_configuration_issuer_copy06(&o->configuration_commit,image)!=0)return PAPACC_ISSUER_UNSUPPORTED;
 result=papacc_issuer_image_find(image,id,&r,papacc_configuration_issuer_hash06,&o->configuration_commit);if(result)return result;
 if(memcmp(digest,r.certificate_digest,32))return PAPACC_ISSUER_CONFLICT;
 return papacc_issuer_image_certificate(image,id,out,capacity,size,papacc_configuration_issuer_hash06,&o->configuration_commit);
}
PAPACC_RESULT papacc_server_launcher_configuration_issuer_port(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ISSUER_ROOT_PORT *port)
{
 if(!port)return PAPACC_RESULT_INVALID_ARGUMENT;memset(port,0,sizeof(*port));if(!root_dormant_owned(o))return PAPACC_RESULT_INVALID_STATE;
 port->ctx=o;port->read=issuer_root_port_read;port->cas=issuer_root_port_cas;port->copy_certificate=issuer_root_port_certificate;return PAPACC_RESULT_OK;
}