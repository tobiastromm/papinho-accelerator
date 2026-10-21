/* Bounded source-only diagnostic collector. No runtime qualification. */
/* Metadata copy only: no callback, I/O, allocation, wait or owner decision. */
/* Only adjacent DATA transport accepts with identical complete snapshots
 * can form an interval. ATTEMPT/authorization and every other event split it.
 * No general snapshot/retry repetition elision is enabled. */
static int e10_progress_valid(const PAPACC_C2_PUBLICATION_EVENT*e){
 unsigned i;if(e->kind!=PAPACC_C2_PUBLICATION_LOWER_ACCEPTED||e->type!=22U||e->result!=0||!e->accepted||!e->run||!e->process||!e->owner||!e->session||!e->operation||!e->generation||!e->connection||!e->bind_channel||!e->e10_snapshot_valid||e->incoming_role<1U||e->incoming_role>2U||e->incoming_direction>1U||e->incoming_operation!=e->operation||e->incoming_generation!=e->generation||e->data_length>16356U||e->progress_prefix>44U+e->data_length||(PAPACC_U64)e->accepted>44U+e->data_length-e->progress_prefix)return 0;
 for(i=0U;i<2U;++i){const PAPACC_C2_E10_SLOT_SNAPSHOT*s=&e->e10_slots[i];
  if(s->operation==e->operation&&s->generation==e->generation&&s->session==e->session&&s->principal_valid&&s->owner==e->owner&&s->channels[e->incoming_role-1U]==e->bind_channel&&s->claimed[e->incoming_role-1U]&&(s->state==3U||s->state==4U)&&!s->revoked&&!s->bridge_retained)return 1;
 }return 0;
}
static int e10_progress_equal(const PAPACC_C2_PUBLICATION_EVENT*a,const PAPACC_C2_PUBLICATION_EVENT*b){
 PAPACC_C2_PUBLICATION_EVENT x=*a,y=*b;
 x.sequence=y.sequence=0U;x.accepted=y.accepted=0U;x.progress_prefix=y.progress_prefix=0U;
 return memcmp(&x,&y,sizeof(x))==0;
}
static void e10_atomic(PAPACC_C2_E10_COLLECTOR*b,const PAPACC_C2_PUBLICATION_EVENT*e){
 PAPACC_C2_E10_INTERVAL*v=&b->intervals[b->used];memset(v,0,sizeof(*v));
 v->first_sequence=v->last_sequence=e->sequence;v->count=1U;v->bytes=e->accepted;
 if(e10_progress_valid(e)){v->prefix_first=e->progress_prefix;v->prefix_last=e->progress_prefix+(PAPACC_U64)e->accepted;}
 b->events[b->used++]=*e;
}
/* Saturation invalidates evidence; it never changes functional owner state. */
static void e10_count(PAPACC_C2_E10_COLLECTOR*b,PAPACC_U64*v){
 if(*v==(PAPACC_U64)-1)b->overflow=PAPACC_TRUE;else ++*v;
}
static void e10_store(PAPACC_SERVER_C2_OWNER_WIN32*o,PAPACC_C2_PUBLICATION_EVENT*e){
 PAPACC_C2_E10_COLLECTOR*b=&o->e10;unsigned i;PAPACC_BOOL history_pending=PAPACC_FALSE;
 if(!b->enabled||b->sealed)return;
 /* Incomplete means a frozen captured prefix; never merge past a lost boundary. */
 e10_count(b,&b->observations);
 if(b->overflow){e10_count(b,&b->dropped_inputs);return;}
 if(b->busy){b->reentrant=PAPACC_TRUE;b->overflow=PAPACC_TRUE;e10_count(b,&b->dropped_inputs);return;}
 if(b->next_sequence==(PAPACC_U64)-1){b->overflow=PAPACC_TRUE;e10_count(b,&b->dropped_inputs);return;}
 b->busy=PAPACC_TRUE;e->run=b->run;e->sequence=b->next_sequence+1U;e->process=(PAPACC_U32)GetCurrentProcessId();e->owner=o->manager->owner;
 if(e10_progress_valid(e)&&e->progress_prefix>(PAPACC_U64)-1-(PAPACC_U64)e->accepted){b->overflow=PAPACC_TRUE;e10_count(b,&b->dropped_inputs);b->busy=PAPACC_FALSE;return;}
 for(i=0U;i<2U;++i)if(b->retired_operation[i])history_pending=PAPACC_TRUE;
 if(b->used&&!history_pending&&e10_progress_valid(e)){
  PAPACC_C2_E10_INTERVAL*v=&b->intervals[b->used-1U];PAPACC_C2_PUBLICATION_EVENT*p=&b->events[b->used-1U];
  if(e10_progress_valid(p)&&v->last_sequence+1U==e->sequence&&v->prefix_last==e->progress_prefix&&e10_progress_equal(p,e)){
   if(b->aggregated_inputs==(PAPACC_U64)-1||v->count==(PAPACC_U64)-1||v->bytes>(PAPACC_U64)-1-(PAPACC_U64)e->accepted){b->overflow=PAPACC_TRUE;e10_count(b,&b->dropped_inputs);b->busy=PAPACC_FALSE;return;}
   e10_count(b,&b->aggregated_inputs);
   ++v->count;v->last_sequence=e->sequence;v->bytes+=(PAPACC_U64)e->accepted;v->prefix_last+=(PAPACC_U64)e->accepted;
   b->next_sequence=e->sequence;b->busy=PAPACC_FALSE;return;
  }
 }
 if(b->used>=PAPACC_C2_E10_EVENT_CAPACITY){b->overflow=PAPACC_TRUE;e10_count(b,&b->dropped_inputs);b->busy=PAPACC_FALSE;return;}
 b->next_sequence=e->sequence;e10_atomic(b,e);
 for(i=0U;i<2U;++i)if(b->retired_operation[i]&&e->e10_slots[i].grant_written&&e->e10_slots[i].operation!=b->retired_operation[i]){
  if(b->used>=PAPACC_C2_E10_EVENT_CAPACITY||b->next_sequence==(PAPACC_U64)-1)b->overflow=PAPACC_TRUE;
  else{PAPACC_C2_PUBLICATION_EVENT reused=*e;reused.kind=PAPACC_C2_E10_HISTORY_REUSED;reused.sequence=++b->next_sequence;e10_atomic(b,&reused);}
  b->retired_operation[i]=0U;b->retired_generation[i]=0U;
 }
 b->busy=PAPACC_FALSE;
}
static void e10_snapshot(PAPACC_SERVER_C2_OWNER_WIN32*o,PAPACC_C2_PUBLICATION_EVENT*event){
 unsigned i,j;PAPACC_C2_OPERATION*r;PAPACC_C2_E10_SLOT_SNAPSHOT*s;PAPACC_C3_BRIDGE*b;
 if(!o->e10.enabled||o->e10.sealed)return;
 event->e10_snapshot_valid=PAPACC_TRUE;
 for(i=0U;i<2U;++i){
  r=&o->manager->operations[i];s=&event->e10_slots[i];b=o->bridges[i];
  s->state=(PAPACC_U32)r->state;if(r->state==PAPACC_C2_OP_EMPTY)continue;
  s->owner=o->manager->owner;s->operation=r->id;s->generation=r->generation;s->epoch=r->epoch;
  if(r->session){s->session=r->session->incarnation;
   for(j=0U;j<4U;++j)if(o->manager->sessions[j].session==r->session&&o->manager->sessions[j].principal.valid){s->principal_valid=PAPACC_TRUE;memcpy(s->principal_public,o->manager->sessions[j].principal.value,sizeof(s->principal_public));break;}
   s->session_state=(PAPACC_U32)r->session->state;s->grant_count=r->session->grant_count;
   for(j=0U;j<2U&&j<r->session->grant_count;++j)s->grants[j]=r->session->grants[j];}
  for(j=0U;j<2U;++j){s->channels[j]=r->tokens.channels[j];s->claimed[j]=r->tokens.claimed[j];s->ack_written[j]=r->tokens.ack_written[j];}
  for(j=0U;j<4U;++j){s->admitted[j]=r->flow[j].admitted;s->downstream[j]=r->flow[j].accepted;s->submitted[j]=r->flow[j].submitted;s->escaped[j]=r->flow[j].escaped;s->queued[j]=r->flow[j].size;}
  s->grant_written=r->tokens.grant_written;s->callbacks=r->callbacks;s->borrowers=r->tokens.borrowers;
  s->site_ready=r->site_ready;s->tls_ready=r->tls_ready_written;s->revoked=r->revoked;
  s->cause=r->first_cause;s->ownership=r->ownership;s->final_queued=r->final_queued;
  s->final_written=r->final_written;s->cleanup_confirmed=r->cleanup_confirmed;
  s->release_attempted=o->child_release_attempted[i];
  if(b){s->bridge_present=PAPACC_TRUE;s->bridge_retained=b->retained;s->membership=b->registered;
   s->child_destroyed=b->child_destroyed;s->ordered_eof=b->ab_eof;
   s->pst_calls=b->processor.e10_release_calls;s->pst_returns=b->processor.e10_release_returns;
   s->pst_count_overflow=b->processor.e10_release_overflow;s->pst_synthetic=b->processor.e10_release_synthetic;
   s->release_result=(PAPACC_U32)b->release_info.result;s->release_disposition=(PAPACC_U32)b->release_info.disposition;}
 }
}
static void e10_boundary(PAPACC_SERVER_C2_OWNER_WIN32*o,PAPACC_C2_OPERATION*r,PAPACC_U64 connection,PAPACC_C2_PUBLICATION_EVENT_KIND kind,PAPACC_U16 type,PAPACC_RESULT result,const PAPACC_U8*p,PAPACC_SIZE n){
 PAPACC_C2_PUBLICATION_EVENT event;PAPACC_CHANNEL*source_channel;
 if(!o->e10.enabled||o->e10.sealed)return;
 memset(&event,0,sizeof(event));event.kind=kind;event.connection=connection;event.type=type;event.result=result;
 if(r){event.operation=r->id;event.generation=r->generation;if(r->session)event.session=r->session->incarnation;}
 source_channel=connection?papacc_channel_manager_find_by_connection(&o->loop->channel_manager,connection):NULL;
 if(source_channel)event.session=source_channel->session_instance_id;
 /* CONTROL hooks are reached only after the existing Session authorization gate. */
 if(type==26U&&(kind==PAPACC_C2_E10_CONTROL_BEFORE||kind==PAPACC_C2_E10_CONTROL_RETURN))event.cancel_authority_prevalidated=PAPACC_TRUE;
 if(type==22U&&p!=NULL&&n>=28U){event.incoming_operation=owner_read64(p);event.incoming_generation=owner_read64(p+8U);event.incoming_role=p[16];event.incoming_direction=p[17];event.offset=owner_read64(p+20U);event.data_length=n-28U;}
 if(type==26U&&p!=NULL&&n==40U){event.target_kind=((unsigned)p[0]<<8)|p[1];event.target_request=owner_read64(p+8U);event.incoming_operation=owner_read64(p+16U);event.incoming_generation=owner_read64(p+24U);event.target_epoch=owner_read64(p+32U);}
 e10_snapshot(o,&event);e10_store(o,&event);
 if(kind==PAPACC_C2_E10_RELEASE_RETURN&&r!=NULL){int slot=index_of(o,r);if(slot>=0){PAPACC_C2_E10_SLOT_SNAPSHOT*v=&event.e10_slots[slot];
  if(v->pst_calls!=0U&&v->pst_returns==v->pst_calls&&!v->pst_count_overflow&&!v->pst_synthetic){event.kind=PAPACC_C2_E10_PST_CALL_OBSERVED;e10_store(o,&event);event.kind=PAPACC_C2_E10_PST_RETURN;e10_store(o,&event);event.kind=PAPACC_C2_E10_MODEL_E_DISPOSITION;e10_store(o,&event);}
  if(v->child_destroyed){event.kind=PAPACC_C2_E10_DESTROYED_CONFIRMED;e10_store(o,&event);}
  if(v->bridge_retained){event.kind=PAPACC_C2_E10_RETAINED_CONFIRMED;e10_store(o,&event);}
 }}
}

/* BIND recording only. stage: 1 existing outer authority gate, 2 actual
 * operation_bind return, 3 actual ACK queue return. Never inspect secret bytes.
 * A claimed slot is a snapshot fact, NOT proof that a token comparison ran. */
static void e10_bind(PAPACC_SERVER_C2_OWNER_WIN32*o,PAPACC_C2_SESSION*s,
 PAPACC_SERVER_SECURE_CANDIDATE_WIN32*c,PAPACC_U64 sid,PAPACC_U64 channel,
 PAPACC_U64 connection,PAPACC_C2_PUBLICATION_EVENT_KIND kind,PAPACC_U32 stage,
 PAPACC_RESULT result,const PAPACC_U8*p,PAPACC_SIZE n){
 PAPACC_C2_PUBLICATION_EVENT event;
 if(!o->e10.enabled||o->e10.sealed)return;
 memset(&event,0,sizeof(event));event.kind=kind;event.type=18U;
 event.session=s?s->incarnation:sid;event.connection=connection;
 event.bind_channel=channel;event.bind_stage=stage;event.result=result;
 if(c&&c->connection_security.state==PAPACC_SECURITY_CONTEXT_AUTHORIZED&&c->connection_security.principal.valid){
  event.bind_principal_valid=PAPACC_TRUE;
  memcpy(event.bind_principal_public,c->connection_security.principal.value,sizeof(event.bind_principal_public));
 }
 /* Public identity fields only; never copy attachment authority at byte36. */
 if(p&&n>=16U){event.incoming_operation=owner_read64(p);event.incoming_generation=owner_read64(p+8U);}
 if(p&&n>=24U)event.target_epoch=owner_read64(p+16U);
 if(p&&n>=33U)event.incoming_role=p[32];
 e10_snapshot(o,&event);e10_store(o,&event);
}

PAPACC_RESULT papacc_server_c2_e10_enable_reserved(PAPACC_SERVER_C2_OWNER_WIN32*o,PAPACC_U64 run,PAPACC_SIZE charged){
 if(!valid(o)||o->secure->dispatch_active||!run||charged<sizeof(*o)||o->e10.enabled)return PAPACC_RESULT_INVALID_STATE;
 memset(&o->e10,0,sizeof(o->e10));o->e10.run=run;o->e10.reserved_owner_bytes=charged;o->e10.reservation_verified=PAPACC_TRUE;o->e10.enabled=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_c2_e10_seal(PAPACC_SERVER_C2_OWNER_WIN32*o){
 unsigned i;
 if(o==NULL||!o->initialized||!o->e10.enabled||o->e10.sealed||o->manager==NULL||o->secure==NULL||o->manager->owner!=(PAPACC_U32)GetCurrentThreadId()||o->secure->dispatch_active||o->e10.busy)return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<6U;++i)if(o->writes[i].pending)return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<2U;++i){PAPACC_C2_OPERATION*r=&o->manager->operations[i];
  if(r->state!=PAPACC_C2_OP_EMPTY&&((r->state!=PAPACC_C2_OP_RELEASED&&r->state!=PAPACC_C2_OP_RETAINED)||r->callbacks||r->tokens.borrowers))return PAPACC_RESULT_INVALID_STATE;
 }
 o->e10.sealed=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_c2_e10_copy(const PAPACC_SERVER_C2_OWNER_WIN32*o,PAPACC_SIZE index,PAPACC_C2_PUBLICATION_EVENT*out){
 if(o==NULL||out==NULL||!o->e10.sealed||index>=o->e10.used)return PAPACC_RESULT_INVALID_STATE;
 *out=o->e10.events[index];return PAPACC_RESULT_OK;
}

/* Called only by the existing valid owner step. No functional query is retried. */
static void s1_finish(PAPACC_SERVER_C2_OWNER_WIN32*o,const PAPACC_C2_S1_FACT*f,
 PAPACC_U64 connection,PAPACC_U64 operation,PAPACC_U64 generation,PAPACC_U16 type,
 PAPACC_RESULT returned){
 PAPACC_C2_PUBLICATION_EVENT e;unsigned i;
 if(!o->e10.enabled||o->e10.sealed)return;
 memset(&e,0,sizeof(e));e.kind=(PAPACC_C2_PUBLICATION_EVENT_KIND)PAPACC_S1_STEP_RETURN;
 e.connection=connection;e.operation=operation;e.generation=generation;e.type=type;
 e.result=returned;e.s1=*f;
 for(i=0U;i<2U;++i){const PAPACC_C2_OPERATION*r=&o->manager->operations[i];
  if(operation&&r->id==operation&&r->generation==generation&&r->state!=PAPACC_C2_OP_EMPTY){
   e.s1.phases|=PAPACC_S1_STATE_AFTER;e.s1.state_after=(PAPACC_U32)r->state;
   if(r->session)e.session=r->session->incarnation;break;}}
 e10_snapshot(o,&e);e10_store(o,&e);
}
