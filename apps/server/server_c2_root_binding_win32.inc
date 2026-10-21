/* Fixed existing-root boundary. No caller Principal, epoch, grant or resolver.
 * The closed diagnostic profile cannot produce these authentication facts. */
static int c8_root_binding_ready(void)
{
 return preparation_owned(&root)&&root.listener_composition.state==PAPACC_ROOT_LISTENER_READY&&
  root_listener_check_mode(&root,root.network.is_active?1:0)==PAPACC_RESULT_OK&&configuration_c2_owner.initialized&&
  !configuration_c2_owner.manager->stopping&&root.secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_RUNNING;
}
static PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c8_authenticated_control(PAPACC_POST_CONTROL_PROCESSOR *p)
{
 PAPACC_SERVER_C2_OWNER_WIN32 *o=&configuration_c2_owner;PAPACC_SESSION *native;
 PAPACC_CHANNEL *channel;PAPACC_CONNECTION *connection;PAPACC_SIZE i;int owned=0;
 if(!c8_root_binding_ready()||!p)return NULL;
 for(i=0U;i<o->loop->processor_capacity;++i)
  if(o->loop->processor_slots[i].kind==PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL&&
     p==&o->loop->processor_slots[i].post_control_processor)owned=1;
 if(!owned||p->c2_context!=o||!p->connection||p->close_dispatched)return NULL;
 native=papacc_session_manager_find(&o->loop->session_manager,p->session_instance_id);
 channel=papacc_channel_manager_find_control(&o->loop->channel_manager,p->session_instance_id);
 connection=papacc_connection_manager_find(o->loop->connection_manager,p->connection->connection_instance_id);
 if(!native||native->state!=PAPACC_SESSION_STATE_ACTIVE||!channel||
    channel->state!=PAPACC_CHANNEL_STATE_BOUND||channel->channel_instance_id!=p->control_channel_instance_id||
    channel->connection_instance_id!=p->connection->connection_instance_id||
    connection!=p->connection||connection->state!=PAPACC_CONNECTION_STATE_ASSOCIATED)return NULL;
 for(i=0U;i<o->secure->candidate_capacity;++i){PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c=&o->secure->candidates[i];
  if(c->owner==o->secure&&c->state==PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL&&
     c->connection_instance_id==connection->connection_instance_id&&c->control_authorized&&
     !c->close_requested&&c->secure_processor.connection&&c->session_security.published&&
     c->connection_security.state==PAPACC_SECURITY_CONTEXT_AUTHORIZED&&
     papacc_principal_equal(&c->session_security.principal,&c->connection_security.principal))return c;
 }
 return NULL;
}
PAPACC_RESULT papacc_server_launcher_c2_capability_policy(const PAPACC_POST_CONTROL_PROCESSOR *input,
 const PAPACC_CAPABILITY_KEY *key,PAPACC_CAPABILITY_POLICY_DECISION *out)
{
 PAPACC_ADMIN_POLICY_CONTEXT context;PAPACC_U32 decision;PAPACC_SIZE matched,i;
 PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c;PAPACC_RESULT result;
 const PAPACC_ADMIN_POLICY *policy=&root.config.policy;
 if(!out||!key)return PAPACC_RESULT_INVALID_ARGUMENT;*out=PAPACC_CAPABILITY_POLICY_DENY;
 c=c8_authenticated_control((PAPACC_POST_CONTROL_PROCESSOR *)input);
 if(!c)return PAPACC_RESULT_INVALID_STATE;
 /* Never skip a historical32-byte DENY then fall through to an ACCEPT.
  * There is no authorized conversion of Resolver16 to historical identity32. */
 if(policy->representation==0U)for(i=0U;i<policy->rule_count;++i)
  if(policy->rules[i].conditions&PAPACC_ADMIN_POLICY_PRINCIPAL)return PAPACC_RESULT_NOT_SUPPORTED;
 memset(&context,0,sizeof(context));context.profile=policy->profile;
 context.listener_port=policy->listener_port;context.admission=policy->admission;
 context.representation=policy->representation;
 if(policy->representation==PAPACC_ADMIN_REPRESENTATION_04){
  context.principal_valid=PAPACC_TRUE;context.principal16.type=1U;context.principal16.length=16U;
  memcpy(context.principal16.value,c->session_security.principal.value,16U);
 }
 result=papacc_server_policy_evaluate(&root.config,&context,key,&decision,&matched);
 if(result==PAPACC_RESULT_OK&&decision==PAPACC_ADMIN_POLICY_ACCEPT)*out=PAPACC_CAPABILITY_POLICY_ALLOW;
 return result;
}
static int c8_negotiated(PAPACC_POST_CONTROL_PROCESSOR *p,PAPACC_SESSION_CAPABILITY_SNAPSHOT **out)
{
 PAPACC_CAPABILITY_KEY tls={2U,1U},mode={3U,1U};PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;
 PAPACC_CAPABILITY_POLICY_DECISION decision;
 if(!c8_authenticated_control(p)||!papacc_post_control_processor_c8_result_complete(p))return 0;
 snapshot=papacc_session_capability_store_find(&configuration_c2_owner.loop->capability_store,p->session_instance_id);
 if(!snapshot||!snapshot->published||snapshot->session_instance_id!=p->session_instance_id||
    !papacc_session_capability_snapshot_contains(snapshot,&tls)||!papacc_session_capability_snapshot_contains(snapshot,&mode))return 0;
 if(papacc_server_launcher_c2_capability_policy(p,&tls,&decision)!=PAPACC_RESULT_OK||decision!=PAPACC_CAPABILITY_POLICY_ALLOW||
    papacc_server_launcher_c2_capability_policy(p,&mode,&decision)!=PAPACC_RESULT_OK||decision!=PAPACC_CAPABILITY_POLICY_ALLOW)return 0;
 *out=snapshot;return 1;
}
PAPACC_BOOL papacc_server_launcher_c2_session_current(const PAPACC_C2_SESSION *s)
{
 PAPACC_SERVER_C2_OWNER_WIN32 *o=&configuration_c2_owner;PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot;
 PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c;PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;PAPACC_ROOT_CURRENT_PIN *admitted=NULL;unsigned i;int owned=0;
 if(!s||!c8_root_binding_ready())return PAPACC_FALSE;
 for(i=0U;i<4U;++i)if(s==&o->sessions[i]){owned=1;admitted=&root.session_revision_pins[i];}
 if(!owned||!s->initialized||!s->root_bound||s->principal!=0U||s->assurance!=1U||
    s->state!=PAPACC_C2_SESSION_ELIGIBLE||s->owner!=o->manager->owner||
    !admitted||!root_operational_reference_current(&root,admitted)||admitted->session_incarnation!=s->incarnation||
    s->root_invocation!=root.invocation||s->policy_epoch!=root.configuration_preparation.current_policy_epoch||s->policy_epoch!=admitted->epoch||
    s->root_revision!=admitted->view.revision||
    memcmp(s->root_domain,admitted->view.domain,32U))return PAPACC_FALSE;
 slot=papacc_server_io_loop_win32_find_connection_slot(o->loop,s->control_connection);
 if(!slot||slot->kind!=PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL||
    slot->post_control_processor.session_instance_id!=s->incarnation||
    slot->post_control_processor.control_channel_instance_id!=s->control_channel||
    !c8_negotiated(&slot->post_control_processor,&snapshot))return PAPACC_FALSE;
 c=c8_authenticated_control(&slot->post_control_processor);
 return c&&memcmp(s->authenticated_principal,c->session_security.principal.value,16U)==0?PAPACC_TRUE:PAPACC_FALSE;
}
PAPACC_RESULT papacc_server_launcher_c2_adopt(PAPACC_SERVER_C2_OWNER_WIN32 *o,PAPACC_U64 connection)
{
 PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot;PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c;
 PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;PAPACC_C2_SESSION *s=NULL;PAPACC_ROOT_CURRENT_PIN *admitted;PAPACC_RESULT result;unsigned i;
 if(o!=&configuration_c2_owner||!connection||!c8_root_binding_ready())return PAPACC_RESULT_INVALID_STATE;
 slot=papacc_server_io_loop_win32_find_connection_slot(o->loop,connection);
 if(!slot||slot->kind!=PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL||
    !c8_negotiated(&slot->post_control_processor,&snapshot))return PAPACC_RESULT_INVALID_STATE;
 c=c8_authenticated_control(&slot->post_control_processor);if(!c)return PAPACC_RESULT_INVALID_STATE;
 /* Adoption can run in dispatch. Reclamation belongs exclusively to
  * the existing nondispatch maintenance/begin_wake/abort boundaries. */
 for(i=0U;i<4U;++i){
  if(o->sessions[i].initialized&&o->sessions[i].incarnation==snapshot->session_instance_id)return PAPACC_RESULT_INVALID_STATE;
  if(!o->sessions[i].initialized&&!s)s=&o->sessions[i];
 }
 if(!s)return PAPACC_RESULT_LIMIT_EXCEEDED;
 admitted=&root.session_revision_pins[(unsigned)(s-o->sessions)];
 if(admitted->pin.ticket)return PAPACC_RESULT_INVALID_STATE;
 result=papacc_server_launcher_configuration_current_pin(&root,admitted);if(result!=PAPACC_RESULT_OK)return result;
 if(!papacc_server_launcher_configuration_current(&root,admitted)||!root_operational_reference_current(&root,admitted)){
  if(papacc_server_launcher_configuration_current_unpin(&root,admitted)==PAPACC_RESULT_OK)memset(admitted,0,sizeof(*admitted));
  return PAPACC_RESULT_INVALID_STATE;
 }
 admitted->session_incarnation=snapshot->session_instance_id;
 /* Adopt only an already published snapshot after the real full RESULT write.
  * No local OFFER/RESULT, epoch argument, Principal ordinal or new wire grant. */
 memset(s,0,sizeof(*s));s->owner=o->manager->owner;s->incarnation=snapshot->session_instance_id;
 s->policy_epoch=admitted->epoch;s->assurance=1U;s->tls_allowed=PAPACC_TRUE;
 s->c3_supported=PAPACC_TRUE;s->initialized=PAPACC_TRUE;s->state=PAPACC_C2_SESSION_ELIGIBLE;
 s->grants[0]=PAPACC_CAPABILITY_TLS_OFFLOAD_V1;s->grants[1].id=3U;s->grants[1].major=1U;s->grant_count=2U;
 memcpy(s->authenticated_principal,c->session_security.principal.value,16U);
 memcpy(s->root_domain,admitted->view.domain,32U);
 s->root_invocation=root.invocation;s->root_revision=admitted->view.revision;
 s->control_connection=connection;s->control_channel=slot->post_control_processor.control_channel_instance_id;s->root_bound=PAPACC_TRUE;
 result=papacc_c2_operation_session_attach(o->manager,s,&c->session_security.principal);
 if(result!=PAPACC_RESULT_OK){
  if(papacc_server_launcher_configuration_current_unpin(&root,admitted)!=PAPACC_RESULT_OK){s->state=PAPACC_C2_SESSION_RETAINED;return result;}
  memset(admitted,0,sizeof(*admitted));SecureZeroMemory(s,sizeof(*s));return result;
 }
 slot->post_control_processor.reader.parser.max_payload_length=803U;return PAPACC_RESULT_OK;
}

/* Exact old-reference cleanup only; no admission or DATA authority. */
PAPACC_RESULT papacc_server_launcher_c2_session_unpin(PAPACC_SERVER_C2_OWNER_WIN32 *o,const PAPACC_C2_SESSION *s)
{
 unsigned i,j;PAPACC_ROOT_CURRENT_PIN *p;PAPACC_CONFIG_VIEW view;PAPACC_U64 administrative,policy;PAPACC_RESULT result;
 if(o!=&configuration_c2_owner||!preparation_owned(&root)||!s||!o->initialized||!o->manager||!o->loop||!o->secure||
    !o->manager->initialized||!o->loop->initialized||!o->loop->connection_manager||!o->secure->initialized||o->manager->owner!=GetCurrentThreadId()||
    o->secure->dispatch_active||o->configuration.state==PAPACC_C2_CONFIG_RETAINED||o->secure->stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL||
    !o->loop->session_manager.initialized||!o->loop->channel_manager.initialized)return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<4;i++)if(s==&o->sessions[i])break;
 if(i==4||!s->initialized||s->state!=PAPACC_C2_SESSION_REVOKED||!s->incarnation||papacc_session_manager_find(&o->loop->session_manager,s->incarnation))return PAPACC_RESULT_INVALID_STATE;
 if(s->control_connection&&papacc_connection_manager_find(o->loop->connection_manager,s->control_connection))return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<2;j++)if(o->manager->operations[j].session==s)return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<6;j++)if(o->writes[j].denial_session==s||(o->writes[j].record&&o->writes[j].record->session==s))return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<o->loop->channel_manager.capacity;j++)if(o->loop->channel_manager.storage[j].state!=PAPACC_CHANNEL_STATE_UNINITIALIZED&&o->loop->channel_manager.storage[j].session_instance_id==s->incarnation)return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<o->loop->processor_capacity;j++){
  PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot=&o->loop->processor_slots[j];
  if(slot->in_use&&((s->control_connection&&slot->connection_instance_id==s->control_connection)||(slot->kind==PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL&&slot->post_control_processor.session_instance_id==s->incarnation)))return PAPACC_RESULT_INVALID_STATE;
 }
 for(j=0;j<o->secure->candidate_capacity;j++){
  PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c=&o->secure->candidates[j];
  if(c->control_close_result.session_instance_id==s->incarnation||(s->control_connection&&c->connection_instance_id==s->control_connection)){
   if(c->state!=PAPACC_SERVER_SECURE_CANDIDATE_CLOSED||c->secure_processor.connection||c->secure_processor.state==PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL||c->session_security.published||c->transport_adapter.registered||papacc_connection_manager_find(o->loop->connection_manager,c->connection_instance_id))return PAPACC_RESULT_INVALID_STATE;
  }
 }
 p=&root.session_revision_pins[i];
 if(!s->root_bound||p->session_incarnation!=s->incarnation||p->invocation!=s->root_invocation||
    papacc_configuration_pin_read(&root.configuration_commit,&p->pin,&view,&administrative,&policy)!=PAPACC_RESULT_OK||
    !administrative||administrative!=p->administrative_epoch||!policy||policy!=p->epoch||policy!=s->policy_epoch||
    view.revision!=s->root_revision||view.revision!=p->view.revision||memcmp(view.domain,s->root_domain,32)||memcmp(view.domain,p->view.domain,32))return PAPACC_RESULT_INVALID_STATE;
 result=papacc_server_launcher_configuration_current_unpin(&root,p);
 if(result==PAPACC_RESULT_OK)memset(p,0,sizeof(*p));return result;
}
