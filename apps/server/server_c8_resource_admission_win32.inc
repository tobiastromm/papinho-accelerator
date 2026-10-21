/* No measurements, allocation, I/O or PST calls. Context/teardown facts must
 * originate in the actual owner; synthetic callers are offline evidence only. */
static int resource_bytes(const PAPACC_U8 *p){unsigned i;for(i=0U;i<32U;++i)if(p[i])return 1;return 0;}
static int resource_context_owner(const PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c)
{return s&&c&&s->invocation&&s->owner&&s->invocation==c->invocation&&s->owner==c->owner;}
static int resource_key_equal(const PAPACC_C8_RESOURCE_KEY *a,const PAPACC_C8_RESOURCE_KEY *b)
{return a&&b&&a->invocation==b->invocation&&a->owner==b->owner&&a->connection==b->connection&&
 a->generation==b->generation&&a->ticket==b->ticket&&a->revision==b->revision&&a->epoch==b->epoch&&!memcmp(a->domain,b->domain,32U);}
static PAPACC_C8_RESOURCE_TICKET *resource_find(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,const PAPACC_C8_RESOURCE_KEY *key)
{
 unsigned i;if(!resource_context_owner(s,c)||!key||!key->ticket||key->invocation!=c->invocation||key->owner!=c->owner||
  key->connection!=c->connection||key->generation!=c->generation)return NULL;
 for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i)if(resource_key_equal(&s->tickets[i].key,key))return &s->tickets[i];
 return NULL;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_check(const PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,PAPACC_U64 now)
{
 const PAPACC_C8_TARGET_EVIDENCE *e;PAPACC_SIZE managed=0U,user=0U,kernel=0U;unsigned i,j,count=0U;int empirical=0;
 if(!resource_context_owner(s,c))return PAPACC_C8_RESOURCE_STATE_ERROR;e=&s->evidence;
 if(!e->source_id||!resource_bytes(e->provenance)||!resource_bytes(e->target)||!resource_bytes(e->sdk_sha256))return PAPACC_C8_RESOURCE_UNKNOWN;
 if(e->version!=0x000605U||e->api!=0x030000U||e->spi!=0x0400U||e->origin<1U||e->origin>2U||
  e->origin!=c->origin||e->profile!=c->profile||e->source_id!=c->source_id||!e->phase||e->phase!=c->phase||
  memcmp(e->target,c->target,32U)||memcmp(e->sdk_sha256,c->sdk_sha256,32U)||memcmp(e->provenance,c->provenance,32U))return PAPACC_C8_RESOURCE_STATE_ERROR;
 if(now<e->sampled_ms||now>e->expires_ms||e->expires_ms<e->sampled_ms||e->expires_ms-e->sampled_ms>1000U)return PAPACC_C8_RESOURCE_STALE;
 if(!c->policy_current||!c->connection||!c->generation||c->connection_state<1U||c->connection_state>2U||
  c->revision!=s->revision||c->epoch!=s->epoch||!s->revision||!s->epoch||memcmp(c->domain,s->domain,32U)||
  !s->listener_reserved||s->stopping||!s->connection_limit||s->connection_limit>PAPACC_C8_RESOURCE_SLOTS)return PAPACC_C8_RESOURCE_STATE_ERROR;
#if defined(PAPACC_C8_SUPERVISED_DIAGNOSTIC)
 empirical=root_sv_exception(s,c,now);
#endif
 for(i=0U;i<PAPACC_C8_RESOURCE_METRICS;++i){const PAPACC_C8_RESOURCE_METRIC *m=&e->metrics[i];
  if(m->kind!=i+1U||m->classification>3U)return PAPACC_C8_RESOURCE_STATE_ERROR;
  if(i<6U&&!m->classification&&!(empirical&&i>=4U))return PAPACC_C8_RESOURCE_UNKNOWN;
 }
 if((!empirical&&(!e->concurrency||s->connection_limit>e->concurrency))||e->retained>e->concurrency||
  e->metrics[2].value>e->peak||e->baseline>e->peak||e->final_class>3U||(e->final_class&&e->final_value>e->peak))return PAPACC_C8_RESOURCE_STATE_ERROR;
 if(s->host_physical_credit>e->metrics[0].value||s->host_commit_credit>e->metrics[1].value||
  (!empirical&&(s->connection_user<e->metrics[4].value||s->connection_kernel<e->metrics[5].value)))return PAPACC_C8_RESOURCE_CAPACITY;
 if(s->base_managed>PAPACC_C8_RESOURCE_LIMIT)return PAPACC_C8_RESOURCE_CAPACITY;
 for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i){const PAPACC_C8_RESOURCE_TICKET *t=&s->tickets[i];
  if(!t->state||t->state==PAPACC_C8_RESOURCE_RELEASED){if(t->managed||t->user||t->kernel||t->borrowers)return PAPACC_C8_RESOURCE_STATE_ERROR;continue;}
  for(j=0U;j<i;++j)if(t->key.ticket==s->tickets[j].key.ticket)return PAPACC_C8_RESOURCE_STATE_ERROR;
  if(t->native_acquisition>2U||t->pst_acquisition>2U)return PAPACC_C8_RESOURCE_STATE_ERROR;
  if(t->key.invocation!=s->invocation||t->key.owner!=s->owner||!t->key.connection||!t->key.generation||t->state>3U||!t->key.ticket||t->key.ticket>s->serial||t->managed!=s->connection_managed||t->user!=s->connection_user||t->kernel!=s->connection_kernel)
   return PAPACC_C8_RESOURCE_STATE_ERROR;
  if(t->managed>(PAPACC_SIZE)-1-managed||t->user>(PAPACC_SIZE)-1-user||t->kernel>(PAPACC_SIZE)-1-kernel)
   return PAPACC_C8_RESOURCE_STATE_ERROR;
  managed+=t->managed;user+=t->user;kernel+=t->kernel;++count;
 }
 if(count>s->connection_limit||managed!=s->managed_used||user!=s->user_used||kernel!=s->kernel_used)
  return PAPACC_C8_RESOURCE_STATE_ERROR;
 if(s->user_limit>33554432U||s->kernel_limit>8388608U||
    managed>PAPACC_C8_RESOURCE_LIMIT-s->base_managed||user>s->user_limit||kernel>s->kernel_limit||
    s->physical_margin>s->host_physical_credit||user>s->host_physical_credit-s->physical_margin||
    s->commit_margin>s->host_commit_credit||user>s->host_commit_credit-s->commit_margin)
  return PAPACC_C8_RESOURCE_CAPACITY;
 return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_reserve(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,PAPACC_U64 now,PAPACC_C8_RESOURCE_KEY *key)
{
 unsigned i,count=0U,slot=PAPACC_C8_RESOURCE_SLOTS;PAPACC_C8_RESOURCE_REASON result;PAPACC_C8_RESOURCE_TICKET *t;
 if(!key)return PAPACC_C8_RESOURCE_STATE_ERROR;memset(key,0,sizeof(*key));
 result=papacc_c8_resource_check(s,c,now);if(result!=0)return result;
#if defined(PAPACC_C8_SUPERVISED_DIAGNOSTIC)
 if(root_sv_exception(s,c,now)&&s->serial>=2U)return PAPACC_C8_RESOURCE_CAPACITY;
#endif
 for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i){t=&s->tickets[i];
  if(t->state&&t->state!=PAPACC_C8_RESOURCE_RELEASED){++count;
   if(t->key.connection==c->connection&&t->key.generation==c->generation)return PAPACC_C8_RESOURCE_STATE_ERROR;
  }else if(slot==PAPACC_C8_RESOURCE_SLOTS)slot=i;
 }
 if(count>=s->connection_limit||slot==PAPACC_C8_RESOURCE_SLOTS||s->serial==(PAPACC_U64)-1||
  s->connection_managed>PAPACC_C8_RESOURCE_LIMIT-s->base_managed-s->managed_used||
  s->connection_user>s->user_limit-s->user_used||s->connection_kernel>s->kernel_limit-s->kernel_used||
  s->connection_user>s->host_physical_credit-s->physical_margin-s->user_used||
  s->connection_user>s->host_commit_credit-s->commit_margin-s->user_used)return PAPACC_C8_RESOURCE_CAPACITY;
 t=&s->tickets[slot];memset(t,0,sizeof(*t));t->key.invocation=c->invocation;t->key.owner=c->owner;
 t->key.connection=c->connection;t->key.generation=c->generation;t->key.ticket=++s->serial;
 t->expires_ms=s->evidence.expires_ms;t->key.revision=c->revision;t->key.epoch=c->epoch;memcpy(t->key.domain,c->domain,32U);
 t->managed=s->connection_managed;t->user=s->connection_user;t->kernel=s->connection_kernel;t->state=PAPACC_C8_RESOURCE_RESERVED;
 s->managed_used+=t->managed;s->user_used+=t->user;s->kernel_used+=t->kernel;*key=t->key;return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_commit(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,PAPACC_U64 now,const PAPACC_C8_RESOURCE_KEY *key)
{
 PAPACC_C8_RESOURCE_TICKET *t;PAPACC_C8_RESOURCE_REASON result=papacc_c8_resource_check(s,c,now);
 if(result!=0)return result;t=resource_find(s,c,key);
 if(!t||now>t->expires_ms||t->state!=PAPACC_C8_RESOURCE_RESERVED||key->revision!=c->revision||key->epoch!=c->epoch||memcmp(key->domain,c->domain,32U))return PAPACC_C8_RESOURCE_STATE_ERROR;
 t->state=PAPACC_C8_RESOURCE_LIVE;return PAPACC_C8_RESOURCE_OK;
}
static PAPACC_C8_RESOURCE_REASON resource_refund(PAPACC_C8_RESOURCE_ADMISSION *s,PAPACC_C8_RESOURCE_TICKET *t)
{
 if(t->borrowers||t->managed>s->managed_used||t->user>s->user_used||t->kernel>s->kernel_used)return PAPACC_C8_RESOURCE_STATE_ERROR;
 s->managed_used-=t->managed;s->user_used-=t->user;s->kernel_used-=t->kernel;
 t->managed=t->user=t->kernel=0U;t->state=PAPACC_C8_RESOURCE_RELEASED;return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_rollback(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,const PAPACC_C8_RESOURCE_KEY *key)
{
 PAPACC_C8_RESOURCE_TICKET *t=resource_find(s,c,key);
 if(!t||t->state!=PAPACC_C8_RESOURCE_RESERVED||c->connection_state!=1U||
  t->native_acquisition!=NATIVE_SOCKET_NOT_ACQUIRED||t->pst_acquisition!=PST_CHILD_NOT_CREATED)return PAPACC_C8_RESOURCE_STATE_ERROR;
 return resource_refund(s,t);
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_retain(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,const PAPACC_C8_RESOURCE_KEY *key)
{
 PAPACC_C8_RESOURCE_TICKET *t=resource_find(s,c,key);
 if(!t||(t->state!=PAPACC_C8_RESOURCE_LIVE&&t->state!=PAPACC_C8_RESOURCE_RETAINED))return PAPACC_C8_RESOURCE_STATE_ERROR;
 t->state=PAPACC_C8_RESOURCE_RETAINED;return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_observe_acquisition(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,const PAPACC_C8_RESOURCE_KEY *key,PAPACC_C8_NATIVE_ACQUISITION native_state,PAPACC_C8_PST_ACQUISITION pst_state)
{
 PAPACC_C8_RESOURCE_TICKET *t=resource_find(s,c,key);
 if(!t||(t->state!=PAPACC_C8_RESOURCE_RESERVED&&t->state!=PAPACC_C8_RESOURCE_LIVE)||
  native_state<PAPACC_C8_NATIVE_UNKNOWN||native_state>NATIVE_SOCKET_ACQUIRED||
  (native_state==PAPACC_C8_NATIVE_UNKNOWN&&pst_state==PAPACC_C8_PST_UNKNOWN)||
  pst_state<PAPACC_C8_PST_UNKNOWN||pst_state>PST_CHILD_CREATED||
  (t->native_acquisition&&(PAPACC_U32)native_state<t->native_acquisition)||
  (t->pst_acquisition&&(PAPACC_U32)pst_state<t->pst_acquisition))return PAPACC_C8_RESOURCE_STATE_ERROR;
 /* Observation only, including late completion of an already-owned effect.
  * Never authorizes a native operation, child creation, close or callback. */
 t->native_acquisition=(PAPACC_U32)native_state;t->pst_acquisition=(PAPACC_U32)pst_state;
 return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_release(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,const PAPACC_C8_RESOURCE_TEARDOWN *proof)
{
 PAPACC_C8_RESOURCE_TICKET *t;PAPACC_C8_RESOURCE_REASON result;if(!proof)return PAPACC_C8_RESOURCE_STATE_ERROR;t=resource_find(s,c,&proof->key);
 if(!t||t->state!=PAPACC_C8_RESOURCE_LIVE||c->connection_state!=3U||
  !proof->waitset_removed||!proof->transport_destroyed||!proof->callbacks_quiescent||!proof->leases_quiescent)
  return PAPACC_C8_RESOURCE_STATE_ERROR;
 if(t->native_acquisition==NATIVE_SOCKET_NOT_ACQUIRED){
  if(proof->native_state!=NATIVE_SOCKET_NOT_ACQUIRED||proof->native_close_result!=0)return PAPACC_C8_RESOURCE_STATE_ERROR;
 }else if(t->native_acquisition==NATIVE_SOCKET_ACQUIRED){
  if(proof->native_state!=NATIVE_SOCKET_DESTROYED||proof->native_close_result!=0)return PAPACC_C8_RESOURCE_STATE_ERROR;
 }else return PAPACC_C8_RESOURCE_UNKNOWN;
 if(t->pst_acquisition==PST_CHILD_NOT_CREATED){
  /* Unused result fields must be zero, NOT interpreted as a PST success. */
  if(proof->pst_state!=PST_CHILD_NOT_CREATED||proof->pst_result!=0||proof->pst_disposition!=0U)return PAPACC_C8_RESOURCE_STATE_ERROR;
 }else if(t->pst_acquisition==PST_CHILD_CREATED){
  /* Private disposition1 maps factual public OK+DESTROYED, never an attempt. */
  if(proof->pst_state!=PST_CHILD_DESTROYED||proof->pst_result!=0||proof->pst_disposition!=1U)return PAPACC_C8_RESOURCE_STATE_ERROR;
 }else return PAPACC_C8_RESOURCE_UNKNOWN;
 result=resource_refund(s,t);if(result!=0)return result;
 t->native_acquisition=proof->native_state;t->pst_acquisition=proof->pst_state;
 return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_borrow(PAPACC_C8_RESOURCE_ADMISSION *s,const PAPACC_C8_RESOURCE_CONTEXT *c,const PAPACC_C8_RESOURCE_KEY *key,PAPACC_C8_BORROW_ACTION action,PAPACC_U64 now)
{
 PAPACC_C8_RESOURCE_TICKET *t=resource_find(s,c,key);PAPACC_C8_RESOURCE_REASON result;
 if(!t||!t->state||t->state==PAPACC_C8_RESOURCE_RELEASED)return PAPACC_C8_RESOURCE_STATE_ERROR;
 if(action==NEW_OPERATIONAL_BORROW){
  result=papacc_c8_resource_check(s,c,now);if(result!=0)return result;
  if(t->state!=PAPACC_C8_RESOURCE_LIVE||c->connection_state!=2U||now>t->expires_ms||
   !t->native_acquisition||!t->pst_acquisition||
   key->revision!=c->revision||key->epoch!=c->epoch||memcmp(key->domain,c->domain,32U)||
   t->borrowers==(PAPACC_U32)-1)return PAPACC_C8_RESOURCE_STATE_ERROR;
  ++t->borrowers;
 }else if(action==EXISTING_BORROW_RETURN||action==GOVERNED_CLEANUP_CALLBACK){
  /* A cleanup completion can only surrender its previously held reference.
   * It acquires no operational authority and performs no resource cleanup. */
  if(!t->borrowers)return PAPACC_C8_RESOURCE_STATE_ERROR;--t->borrowers;
 }else return PAPACC_C8_RESOURCE_STATE_ERROR;
 return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_stop(PAPACC_C8_RESOURCE_ADMISSION *s,PAPACC_U64 invocation,PAPACC_U64 owner)
{
 unsigned i;if(!s||!invocation||!owner||s->invocation!=invocation||s->owner!=owner)return PAPACC_C8_RESOURCE_STATE_ERROR;
 s->stopping=1U;for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i)if(s->tickets[i].state==PAPACC_C8_RESOURCE_RESERVED&&!s->tickets[i].borrowers&&
  s->tickets[i].native_acquisition==NATIVE_SOCKET_NOT_ACQUIRED&&s->tickets[i].pst_acquisition==PST_CHILD_NOT_CREATED){
  PAPACC_C8_RESOURCE_REASON result=resource_refund(s,&s->tickets[i]);if(result!=0)return result;
 }return PAPACC_C8_RESOURCE_OK;
}
