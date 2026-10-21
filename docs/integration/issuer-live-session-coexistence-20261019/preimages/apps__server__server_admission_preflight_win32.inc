/* Read-only currency of the actual published graph; never publishes sockets.
 * All common policy/epoch/lease checks remain in root_cohort_preflight. */
static int root_active_io_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,int staged)
{
 PAPACC_SIZE i,j,k;PAPACC_SERVER_NETWORK *n=&o->network;
 PAPACC_PST_SECURE_SCHEDULER *scheduler=&o->secure_io.scheduler;

 if(n->is_active!=(staged?PAPACC_FALSE:PAPACC_TRUE)||!n->tcp_platform.initialized||
    n->listener_set.is_active!=(staged?PAPACC_FALSE:PAPACC_TRUE)||
    !n->listener_set.count||n->listener_set.count!=o->configuration_preparation.participants.target_count||
    n->listener_set.count>n->listener_set.capacity||n->listener_set.count>n->listener_storage_capacity||
    n->listener_set.entries!=n->listener_storage||!n->listener_storage||
    n->listener_set.bound_port!=o->config.control_port||
    o->io_loop.network!=n||o->io_loop.acceptor!=&o->acceptor||!o->acceptor.initialized||
    o->acceptor.server_network!=n||o->io_loop.connection_manager!=&o->acceptor.connection_manager||
    !o->acceptor.connection_manager.initialized||
    o->acceptor.connection_manager.storage!=o->connection_storage||!o->connection_storage||
    o->acceptor.transport_context_storage!=o->context_storage||!o->context_storage||
    o->io_loop.processor_slots!=o->processor_storage||
    o->io_loop.session_manager.storage!=o->session_storage||
    o->io_loop.channel_manager.storage!=o->channel_storage||o->secure_io.network!=n||
    o->secure_io.listener_configuration!=&o->secure_listener_set||
    o->secure_listener_set.listeners!=&o->secure_listener||o->secure_listener_set.count!=1U||
    o->secure_io.listener_count!=n->listener_set.count||
    o->secure_io.listener_sources!=o->listener_sources||!o->listener_sources||
    o->secure_io.listener_tokens!=o->listener_tokens||!o->listener_tokens||
    !scheduler->initialized||scheduler->stopping||scheduler->partition_failed||
    scheduler->members!=o->secure_members||!scheduler->members||
    !scheduler->wait_set_count||scheduler->wait_set_count>3U||
    scheduler->member_count>scheduler->member_capacity)return 0;
 for(i=0U;i<n->listener_set.count;++i){
  PAPACC_TCP_LISTENER_ENTRY_WIN32 *e=&n->listener_set.entries[i];
  const PAPACC_BIND_TARGET *target=&o->configuration_preparation.participants.targets[i];int found=0;
  if(!e->socket.is_open||!e->socket.is_bound||!e->socket.is_listening||e->socket.native_socket==INVALID_SOCKET||e->socket.family!=target->address.family||
     !papacc_ip_address_equal(&e->target.address,&target->address)||
     e->target.scope_id!=target->scope_id||e->target.interface_instance_id!=target->interface_instance_id||
     !o->listener_sources[i]||!o->listener_tokens[i])return 0;
  for(j=0U;j<scheduler->member_capacity;++j){PAPACC_PST_SCHEDULER_MEMBER *m=&scheduler->members[j];
   if(m->registered&&m->kind==PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL&&
      m->source.external_source==o->listener_sources[i]&&m->token==o->listener_tokens[i]&&m->membership_wait_set){
    for(k=0U;k<scheduler->wait_set_count;++k)if(scheduler->wait_sets[k]==m->membership_wait_set)++found;
   }
  }
  if(found!=1)return 0;
 }
 return 1;
}
/* Read-only offline stage, no I/O, allocation, callbacks or publication.
 * Even a successful OPEN preflight is not authentication or operation grant. */
static PAPACC_RESULT root_cohort_preflight(
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *pin,
 PAPACC_ADMISSION_PREFLIGHT *report,int active)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
 const PAPACC_SERVER_CONFIG *c;PAPACC_CAPABILITY_KEY tls={2U,1U},mode={3U,1U};
 if(report==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(report,0,sizeof(*report));report->missing=PAPACC_ADMISSION_ALL;
 if(!preparation_owned(o)||!pin)return PAPACC_RESULT_INVALID_STATE;
 t=&o->configuration_preparation;c=&o->config;
 report->invocation=o->invocation;report->reserved=t->combined_reserved;
 report->profile=(PAPACC_U32)c->listener_profile;report->admission=c->policy.admission;
 if(papacc_server_config_validate(c)==PAPACC_RESULT_OK&&c->policy.present&&
    c->policy.profile==(PAPACC_U32)c->listener_profile&&c->policy.listener_port==c->control_port)
  report->checked|=PAPACC_ADMISSION_MODEL;
 if(t->normative_committed&&t->administrative_published&&t->policy.prepared&&t->policy.from_model&&
    papacc_server_launcher_configuration_current(o,pin)&&
    root_snapshot_model_equal(&t->model,&pin->view)&&root_policy_model_valid(o)){
  report->checked|=PAPACC_ADMISSION_CURRENT;report->revision=pin->view.revision;
 }
 if(root_policy_inputs_valid(t)&&papacc_capability_set_contains(&t->supported,&tls)&&papacc_capability_set_contains(&t->supported,&mode)&&
    papacc_capability_set_contains(&t->enabled,&tls)&&papacc_capability_set_contains(&t->enabled,&mode))
  report->checked|=PAPACC_ADMISSION_CAPABILITY;
 if(pin->epoch&&pin->epoch==t->current_policy_epoch&&
    configuration_c2_owner.configuration.epoch==pin->epoch&&
    configuration_c2_owner.configuration.state==PAPACC_C2_CONFIG_CURRENT&&
    papacc_server_c2_configuration_authority_current(&configuration_c2_owner)==PAPACC_RESULT_OK){
  report->checked|=PAPACC_ADMISSION_EPOCH;report->epoch=pin->epoch;
 }
 if(c->listener_profile==PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL&&
    configuration_c2_owner.configuration.source==&t->source&&
    configuration_c2_owner.configuration.secure_lease==&o->security_composition&&
    t->source.state==PAPACC_SECURITY_SOURCE_PUBLISHED&&
    papacc_server_security_source_validate(&t->source)==PAPACC_RESULT_OK&&
    papacc_security_composition_is_ready(&o->security_composition)==PAPACC_TRUE)
  report->checked|=PAPACC_ADMISSION_LEASE;
 if(o->security_composition.runtime&&t->provider_connection&&
    t->runtime_attempted&&t->runtime_result==PAPACC_RESULT_OK&&
    t->provider_attempted&&t->provider_result==PST_RESULT_OK&&!t->provider_release_attempted)
  report->checked|=PAPACC_ADMISSION_PROVIDER;
 if(o->attachment_state==3U&&o->io_loop.initialized&&o->secure_io.initialized&&
    ((active&&root_active_io_current(o,active==2))||(!active&&o->io_loop.acceptor==NULL&&!o->acceptor.initialized&&!o->network.is_active&&
    !o->network.listener_set.count))&&o->secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_RUNNING&&
    configuration_c2_owner.configuration.io_attached&&
    o->io_loop.c2_owner==&configuration_c2_owner&&o->secure_io.protocol_loop==&o->io_loop)
  report->checked|=PAPACC_ADMISSION_LIFECYCLE;
 if(t->combined_reserved&&t->combined_reserved<=PAPACC_LAUNCHER_MANAGED_LIMIT&&
    configuration_c2_owner.configuration.reserved==t->combined_reserved&&
    t->source.other_reserved<=PAPACC_LAUNCHER_MANAGED_LIMIT-sizeof(t->source)&&
    t->source.credit<=PAPACC_LAUNCHER_MANAGED_LIMIT-sizeof(t->source)-t->source.other_reserved&&
    t->combined_reserved==sizeof(t->source)+t->source.other_reserved+t->source.credit)
  report->checked|=PAPACC_ADMISSION_RESOURCE;
 /* No governed CLOSED access-auth implementation is currently composed.
  * OPEN only bypasses application login, not future Secure mTLS or grants. */
 if(c->policy.present&&c->policy.admission==PAPACC_ADMIN_OPEN)
  report->checked|=PAPACC_ADMISSION_ACCESS;
 report->missing=PAPACC_ADMISSION_ALL&~report->checked;
 report->offline_ready=report->missing==0U?PAPACC_TRUE:PAPACC_FALSE;
 /* Provider/kernel target bounds UNKNOWN, client/Session/grants absent and
  * application publication disabled. These fields ALWAYS stay false. */
 return report->offline_ready?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}

PAPACC_RESULT papacc_server_launcher_admission_preflight(
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_CURRENT_PIN *pin,PAPACC_ADMISSION_PREFLIGHT *report)
{return root_cohort_preflight(o,pin,report,0);}
