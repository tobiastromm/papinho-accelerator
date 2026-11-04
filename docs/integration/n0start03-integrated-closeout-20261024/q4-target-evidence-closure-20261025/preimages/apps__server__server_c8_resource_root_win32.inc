#include "pal_time.h"
PAPACC_BOOL papacc_server_launcher_generic_io_is_c8(
 const PAPACC_SERVER_NETWORK *network,const PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
 const PAPACC_SERVER_IO_LOOP_WIN32 *loop)
{
 /* Do not condition identity on readiness, ticket currency or Stop. None of
  * those states grants fallback authority to a generic consumer. */
 return network==&root.network||acceptor==&root.acceptor||loop==&root.io_loop?
  PAPACC_TRUE:PAPACC_FALSE;
}
typedef char papacc_c8_resource_slots_match_connections[(PAPACC_C8_RESOURCE_SLOTS==PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY)?1:-1];
/* Capture from the existing committed/current root, never caller grants. */
static PAPACC_RESULT root_target_binding(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_C8_TARGET_BINDING *b)
{
 unsigned i;if(!preparation_owned(o)||!b||
 root_listener_check_mode(o,o->network.is_active?1:0)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 memset(b,0,sizeof(*b));b->invocation=o->invocation;b->owner=o->owner_thread;
 b->revision=o->listener_composition.pin.view.revision;b->epoch=o->listener_composition.pin.epoch;
 memcpy(b->domain,o->listener_composition.pin.view.domain,32U);
 b->managed=o->configuration_preparation.combined_reserved;b->charged=o->resource_admission.managed_used;
 if(b->managed<1851619U+PAPACC_C8_KNOWN_MANAGED_BYTES-1580864U)return PAPACC_RESULT_LIMIT_EXCEEDED;
 b->profile=o->config.listener_profile;b->phase=o->network.is_active?2U:1U;b->configured=b->current=1U;
 b->stopping=o->resource_admission.stopping||o->secure_io.stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING;
 for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i){PAPACC_U32 ticket_state=o->resource_admission.tickets[i].state;
  if(ticket_state&&ticket_state!=PAPACC_C8_RESOURCE_RELEASED)++b->concurrency;
  if(ticket_state==PAPACC_C8_RESOURCE_RETAINED)++b->retained;
 }
 return PAPACC_RESULT_OK;
}
/* Read only durable by-value facts. A historical committed identity is not
 * CURRENT, a grant or permission to resurrect its dependencies. */
PAPACC_RESULT papacc_server_launcher_target_observation_binding(
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_C8_TARGET_BINDING *b)
{
 PAPACC_U32 owner_state;PAPACC_C8_TARGET_BINDING operational;unsigned i;
 if(!b)return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(b,0,sizeof(*b));
 if(o!=&root||GetCurrentThreadId()!=o->owner_thread||!o->invocation||o->secure_io.dispatch_active)
  return PAPACC_RESULT_INVALID_STATE;
 owner_state=papacc_server_launcher_owner_state();
 if(owner_state!=PAPACC_LAUNCHER_ACTIVE&&owner_state!=PAPACC_LAUNCHER_RETAINED)return PAPACC_RESULT_INVALID_STATE;
 b->invocation=o->invocation;b->owner=o->owner_thread;b->profile=o->config.listener_profile;
 b->managed=o->configuration_preparation.combined_reserved;b->charged=o->resource_admission.managed_used;
 b->stopping=owner_state==PAPACC_LAUNCHER_RETAINED||o->resource_admission.stopping||
  o->secure_io.stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING;
 for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i){PAPACC_U32 ticket_state=o->resource_admission.tickets[i].state;
  if(ticket_state&&ticket_state!=PAPACC_C8_RESOURCE_RELEASED)++b->concurrency;
  if(ticket_state==PAPACC_C8_RESOURCE_RETAINED)++b->retained;
 }
 b->phase=o->network.is_active?PAPACC_C8_OBSERVATION_ACTIVE:PAPACC_C8_OBSERVATION_BASELINE;
 if(o->attachment_state==3U&&!o->network.is_active)b->phase=PAPACC_C8_OBSERVATION_PREPARED;
 if(o->secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_REQUESTED||
    o->secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING||
    o->secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL)b->phase=PAPACC_C8_OBSERVATION_SHUTDOWN_PENDING;
 if(o->secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_DRAINING)b->phase=PAPACC_C8_OBSERVATION_DRAINING;
 /* STOPPED is a consumer teardown observation window, not proof of native close. */
 if(o->secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_STOPPED||
    o->configuration_preparation.state==PAPACC_ROOT_PREPARATION_CLOSED||
    o->configuration_preparation.state==PAPACC_ROOT_PREPARATION_ROLLED_BACK)b->phase=PAPACC_C8_OBSERVATION_TEARDOWN;
 if(owner_state==PAPACC_LAUNCHER_RETAINED||b->retained||
    o->configuration_preparation.state==PAPACC_ROOT_PREPARATION_RETAINED)b->phase=PAPACC_C8_OBSERVATION_RETAINED;
 if(b->phase==PAPACC_C8_OBSERVATION_DRAINING||b->phase==PAPACC_C8_OBSERVATION_TEARDOWN||
    b->phase==PAPACC_C8_OBSERVATION_RETAINED||b->phase==PAPACC_C8_OBSERVATION_SHUTDOWN_PENDING)b->stopping=1U;
 /* Capture historical root identity only when the actual durable fields say
  * it was committed for this invocation; never consult released lease pointers. */
 if(o->configuration_preparation.normative_committed&&o->configuration_preparation.invocation==o->invocation&&
    o->listener_composition.invocation==o->invocation&&o->listener_composition.pin.view.revision&&
    o->listener_composition.pin.epoch){
  b->configured=1U;b->revision=o->listener_composition.pin.view.revision;b->epoch=o->listener_composition.pin.epoch;
  memcpy(b->domain,o->listener_composition.pin.view.domain,32U);
 }
 if(owner_state==PAPACC_LAUNCHER_ACTIVE&&!b->stopping&&
    root_target_binding(o,&operational)==PAPACC_RESULT_OK){*b=operational;}
 return PAPACC_RESULT_OK;
}
/* Explicit pre-dispatch observation; NEVER writes the admission ledger. */
PAPACC_RESULT papacc_server_launcher_target_refresh(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_C8_TARGET_BINDING b;PAPACC_RESULT result;
 result=papacc_server_launcher_target_observation_binding(o,&b);
 if(result!=PAPACC_RESULT_OK)return result;
 return papacc_c8_target_collect(&o->target_producer,&b);
}
PAPACC_RESULT papacc_server_launcher_target_process_provenance(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_C8_PROCESS_PROVENANCE *out)
{
 PAPACC_C8_TARGET_BINDING b;PAPACC_RESULT result;
 if(!out)return PAPACC_RESULT_INVALID_ARGUMENT;
 /* Reject aliases of the durable owner before touching caller output. */
 {ULONG_PTR a=(ULONG_PTR)o,z=(ULONG_PTR)out;
 if((z>=a&&z-a<sizeof(*o))||(z<a&&a-z<sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;}
 result=papacc_server_launcher_target_observation_binding(o,&b);
 if(result!=PAPACC_RESULT_OK)return result;
 if(o->network.is_active||o->secure_io.dispatch_active)return PAPACC_RESULT_INVALID_STATE;
 return papacc_c8_target_process_provenance(&o->target_producer,&b,out);
}
static PAPACC_RESULT root_resource_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_C8_TARGET_BINDING b;PAPACC_U64 now;PAPACC_C8_RESOURCE_REASON result;
 if(root_target_binding(o,&b)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 if(o->target_producer.process!=GetCurrentProcessId()||o->target_producer.thread!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INTERNAL_ERROR;
 #if defined(PAPACC_C8_SUPERVISED_DIAGNOSTIC)
 {
 PAPACC_C8_RESOURCE_CONTEXT context;memset(&context,0,sizeof(context));
 context.invocation=b.invocation;context.owner=b.owner;context.revision=b.revision;context.epoch=b.epoch;
 memcpy(context.domain,b.domain,32U);
 if(root_sv_exception(&o->resource_admission,&context,now/1000000U)){
  o->resource_admission.evidence=o->target_producer.evidence;return PAPACC_RESULT_OK;
 }
 }
#endif
 result=papacc_c8_target_admission(&o->target_producer,&b,o->target_producer.evidence.target,now/1000000U);
 return result==PAPACC_C8_RESOURCE_OK?PAPACC_RESULT_OK:PAPACC_RESULT_NOT_SUPPORTED;
}
PAPACC_RESULT papacc_server_launcher_client_resource_reserve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_C8_RESOURCE_KEY *ticket)
{
 if(!ticket)return PAPACC_RESULT_INVALID_ARGUMENT;memset(ticket,0,sizeof(*ticket));
 if(!preparation_owned(o)||!papacc_server_launcher_active_cohort_current(o))return PAPACC_RESULT_INVALID_STATE;
 /* Compatibility query has no candidate lifetime: never accept caller-issued
  * Connection/generation or measurement. Real accept uses accept_prepare. */
 return root_resource_current(o);
}
static PAPACC_RESULT root_activation_retire(void);
PAPACC_RESULT papacc_server_launcher_resource_stop(PAPACC_SERVER_SECURE_IO_WIN32 *secure)
{
 PAPACC_C8_RESOURCE_ADMISSION *s=&root.resource_admission;unsigned i;
 if(secure!=&root.secure_io)return PAPACC_RESULT_OK;
 if(GetCurrentThreadId()!=root.owner_thread)return PAPACC_RESULT_INVALID_STATE;
 if(!s->invocation){s->stopping=1U;return PAPACC_RESULT_OK;}
 if(papacc_c8_resource_stop(s,root.invocation,root.owner_thread)!=0)return PAPACC_RESULT_INVALID_STATE;
 if(root.secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_DRAINING&&!root.secure_io.dispatch_active)
  for(i=0U;i<root.secure_io.candidate_capacity&&i<PAPACC_C8_RESOURCE_SLOTS;++i)
   if(root.accept_lifetimes[i].phase&&root.secure_candidates[i].state==PAPACC_SERVER_SECURE_CANDIDATE_CLOSED)
    if(papacc_server_launcher_accept_complete(&root.secure_candidates[i])!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 if(root.secure_io.stop_state==PAPACC_SERVER_SECURE_STOP_DRAINING&&
    !root.secure_io.scheduler.member_count&&!root.acceptor.connection_manager.count&&
    !root.io_loop.session_manager.count&&!root.io_loop.channel_manager.count)
  return root_activation_retire();
 return PAPACC_RESULT_OK;
}
PAPACC_BOOL papacc_server_launcher_resource_dependencies(const PAPACC_SERVER_SECURE_IO_WIN32 *secure)
{
 unsigned i;if(secure!=&root.secure_io)return PAPACC_FALSE;
 for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i)if(root.resource_admission.tickets[i].state&&
  root.resource_admission.tickets[i].state!=PAPACC_C8_RESOURCE_RELEASED)return PAPACC_TRUE;
 return root.resource_admission.managed_used||root.resource_admission.user_used||root.resource_admission.kernel_used?PAPACC_TRUE:PAPACC_FALSE;
}

/* The legacy network_start route remains denied even with future evidence.
 * C8 activation uses activate below and owner-preserving native acquisition;
 * void legacy cleanup cannot prove a quota refund. */
PAPACC_RESULT papacc_server_launcher_network_reserve(PAPACC_SERVER_NETWORK *network)
{
 PAPACC_RESULT result;
 if(network!=&root.network||!preparation_owned(&root)||
    root_listener_check(&root)!=PAPACC_RESULT_OK||
    root.listener_composition.state!=PAPACC_ROOT_LISTENER_READY)
  return PAPACC_RESULT_INVALID_STATE;
 result=root_resource_current(&root);if(result!=PAPACC_RESULT_OK)return result;
 return PAPACC_RESULT_NOT_SUPPORTED;
}
PAPACC_BOOL papacc_server_launcher_owns_secure(const PAPACC_SERVER_SECURE_IO_WIN32 *secure)
{return secure==&root.secure_io?PAPACC_TRUE:PAPACC_FALSE;}
static PAPACC_C8_ACCEPT_LIFETIME *root_accept_lifetime(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
 PAPACC_SIZE i;
 if(!candidate||candidate->owner!=&root.secure_io||GetCurrentThreadId()!=root.owner_thread||
    root.secure_candidates!=root.secure_io.candidates)return NULL;
 for(i=0U;i<root.secure_io.candidate_capacity&&i<PAPACC_C8_RESOURCE_SLOTS;++i)
  if(candidate==&root.secure_candidates[i])return &root.accept_lifetimes[i];
 return NULL;
}
PAPACC_RESULT papacc_server_launcher_accept_prepare(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,PAPACC_U64 now_ms)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=root_accept_lifetime(candidate);PAPACC_C8_RESOURCE_CONTEXT *c;
 PAPACC_C8_TARGET_EVIDENCE *e=&root.resource_admission.evidence;PAPACC_RESULT result;
 if(!a||a->phase||!papacc_server_launcher_active_cohort_current(&root)||
    root.secure_io.stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING||root.accept_generation==(PAPACC_U64)-1)
  return PAPACC_RESULT_INVALID_STATE;
 result=root_resource_current(&root);if(result!=PAPACC_RESULT_OK)return result;
 /* Identity is a reservation of this durable candidate's NEXT native lifetime,
  * not a fabricated existing protocol Connection or authenticated Session. */
 memset(a,0,sizeof(*a));a->socket=(PAPACC_TCP_ACCEPTED_SOCKET_WIN32)PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
 c=&a->context;c->invocation=root.invocation;c->owner=root.owner_thread;
 c->connection=(PAPACC_U64)(a-root.accept_lifetimes)+1U;c->generation=++root.accept_generation;
 c->revision=root.listener_composition.pin.view.revision;c->epoch=root.listener_composition.pin.epoch;
 memcpy(c->domain,root.listener_composition.pin.view.domain,32U);
 c->profile=e->profile;c->source_id=e->source_id;c->origin=2U;c->phase=e->phase;
 memcpy(c->target,e->target,32U);memcpy(c->sdk_sha256,e->sdk_sha256,32U);memcpy(c->provenance,e->provenance,32U);
 c->policy_current=1U;c->connection_state=1U;
 if(papacc_c8_resource_reserve(&root.resource_admission,c,now_ms,&a->ticket)!=PAPACC_C8_RESOURCE_OK)
  return PAPACC_RESULT_LIMIT_EXCEEDED;
 a->phase=1U;
 if(papacc_c8_resource_observe_acquisition(&root.resource_admission,c,&a->ticket,NATIVE_SOCKET_NOT_ACQUIRED,PST_CHILD_NOT_CREATED)!=0)
  return PAPACC_RESULT_INVALID_STATE;
 /* Revalidate current authority and mark LIVE before accept can acquire. */
 if(!papacc_server_launcher_active_cohort_current(&root)||
    papacc_c8_resource_commit(&root.resource_admission,c,now_ms,&a->ticket)!=0){
  if(papacc_c8_resource_rollback(&root.resource_admission,c,&a->ticket)==0)a->phase=0U;
  return PAPACC_RESULT_INVALID_STATE;
 }
 c->connection_state=2U;a->phase=2U;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_accept_observe(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,
 const PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *socket,PAPACC_BOOL transferred)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=root_accept_lifetime(candidate);PAPACC_C8_NATIVE_ACQUISITION n;PAPACC_C8_PST_ACQUISITION p;
 if(!a||a->phase!=2U||!socket)return PAPACC_RESULT_INVALID_STATE;
 if(socket->native_socket!=INVALID_SOCKET){a->socket=*socket;}
 if(transferred){a->socket_transferred=1U;a->socket.native_socket=INVALID_SOCKET;a->socket.is_open=PAPACC_FALSE;}
 n=(a->socket.native_socket!=INVALID_SOCKET||a->socket_transferred||a->native_close_confirmed)?NATIVE_SOCKET_ACQUIRED:NATIVE_SOCKET_NOT_ACQUIRED;
 p=candidate->secure_processor.connection?PST_CHILD_CREATED:PST_CHILD_NOT_CREATED;
 return papacc_c8_resource_observe_acquisition(&root.resource_admission,&a->context,&a->ticket,n,p)==0?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_server_launcher_accept_close_socket(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *socket)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=root_accept_lifetime(candidate);
 if(!a||a->phase!=2U||!socket||a->socket_transferred)return PAPACC_RESULT_INVALID_STATE;
 if(a->socket.native_socket==INVALID_SOCKET)return PAPACC_RESULT_OK;
 if(a->socket.native_socket!=socket->native_socket||closesocket(a->socket.native_socket)!=0){
  (void)papacc_c8_resource_retain(&root.resource_admission,&a->context,&a->ticket);a->phase=3U;
  root.secure_io.stop_state=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL;return PAPACC_RESULT_INTERNAL_ERROR;
 }
 a->native_close_confirmed=1U;a->socket=(PAPACC_TCP_ACCEPTED_SOCKET_WIN32)PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
 *socket=a->socket;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_accept_complete(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=root_accept_lifetime(candidate);PAPACC_C8_RESOURCE_TEARDOWN proof;
 PAPACC_C8_RESOURCE_TICKET *t;
 if(!a||!a->phase)return PAPACC_RESULT_OK;
 if(a->phase!=2U)return PAPACC_RESULT_INVALID_STATE;
 if(candidate->secure_processor.state==PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL){
  (void)papacc_c8_resource_retain(&root.resource_admission,&a->context,&a->ticket);a->phase=3U;return PAPACC_RESULT_INVALID_STATE;
 }
 if(root.secure_io.dispatch_active||candidate->state!=PAPACC_SERVER_SECURE_CANDIDATE_CLOSED||
    candidate->secure_processor.connection||candidate->transport_adapter.registered||a->unaccepted_transport||a->socket.native_socket!=INVALID_SOCKET||
    (candidate->connection_instance_id&&papacc_server_io_loop_win32_find_connection_slot(&root.io_loop,candidate->connection_instance_id)))
  return PAPACC_RESULT_INVALID_STATE;
 t=resource_find(&root.resource_admission,&a->context,&a->ticket);if(!t)return PAPACC_RESULT_INVALID_STATE;
 memset(&proof,0,sizeof(proof));proof.key=a->ticket;
 proof.native_state=t->native_acquisition;proof.pst_state=t->pst_acquisition;
 if(t->pst_acquisition==PST_CHILD_CREATED){
  if(candidate->secure_processor.e10_release_synthetic||candidate->secure_processor.source_result!=PST_RESULT_OK||
     candidate->secure_processor.release_info.disposition!=PST_RELEASE_DISPOSITION_DESTROYED)return PAPACC_RESULT_INVALID_STATE;
  proof.pst_state=PST_CHILD_DESTROYED;proof.pst_disposition=1U;
 }
 if(t->native_acquisition==NATIVE_SOCKET_ACQUIRED){
  if(!a->native_close_confirmed&&!(a->socket_transferred&&proof.pst_state==PST_CHILD_DESTROYED))return PAPACC_RESULT_INVALID_STATE;
  proof.native_state=NATIVE_SOCKET_DESTROYED;
 }
 if(a->protocol_connection&&a->protocol_connection!=candidate->connection_instance_id)return PAPACC_RESULT_INVALID_STATE;
 /* Each callback-bearing registration owns one reference. Completion returns
  * it once, after actual membership/Slot absence, even after revocation. */
 if(a->scheduler_borrow){
  if(papacc_c8_resource_borrow(&root.resource_admission,&a->context,&a->ticket,EXISTING_BORROW_RETURN,0U)!=0)return PAPACC_RESULT_INVALID_STATE;
  a->scheduler_borrow=0U;
 }
 if(a->protocol_borrow){
  if(papacc_c8_resource_borrow(&root.resource_admission,&a->context,&a->ticket,EXISTING_BORROW_RETURN,0U)!=0)return PAPACC_RESULT_INVALID_STATE;
  a->protocol_borrow=0U;
 }
 proof.waitset_removed=proof.transport_destroyed=proof.callbacks_quiescent=proof.leases_quiescent=1U;
 a->context.connection_state=3U;
 if(papacc_c8_resource_release(&root.resource_admission,&a->context,&proof)!=0)return PAPACC_RESULT_INVALID_STATE;
 a->phase=0U;return PAPACC_RESULT_OK;
}

/* Public pst_transport_release is void and the unconsumed Win32 wrapper may
 * close its socket. Preserve that wrapper instead of closing its alias again
 * or calling its destruction proof a successful native close. */
PAPACC_RESULT papacc_server_launcher_accept_retain_transport(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,pst_transport *transport)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=root_accept_lifetime(candidate);
 if(!a||a->phase!=2U||!transport||a->socket_transferred||a->unaccepted_transport)return PAPACC_RESULT_INVALID_STATE;
 a->unaccepted_transport=transport;
 a->socket.native_socket=INVALID_SOCKET;a->socket.is_open=PAPACC_FALSE; /* wrapper owns it */
 (void)papacc_c8_resource_retain(&root.resource_admission,&a->context,&a->ticket);
 a->phase=3U;candidate->state=PAPACC_SERVER_SECURE_CANDIDATE_FAILED;
 root.secure_io.stop_state=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL;
 root.secure_io.stop_last_remove_result=PAPACC_RESULT_INTERNAL_ERROR;
 return PAPACC_RESULT_INTERNAL_ERROR;
}

/* One-listener isolated profile, matching the already charged closed-attach
 * arrays. More targets deny before effects; no implicit wider profile. */
PAPACC_RESULT papacc_server_launcher_interface_consumption_check(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,
 const PAPACC_ROOT_CURRENT_PIN *pin)
{
 PAPACC_INTERFACE_CURRENT_REPORT binding;PAPACC_RESULT result;
 if(!preparation_owned(o)||!pin)return PAPACC_RESULT_INVALID_STATE;
 if(o->config.bind_selection.mode!=PAPACC_BIND_SELECTION_SELECTED_INTERFACES)return PAPACC_RESULT_NOT_SUPPORTED;
 result=papacc_server_launcher_configuration_interface_current(o,pin,&binding);
 if(result!=PAPACC_RESULT_OK)return result;
 /* CURRENT correlation/MODEL_BOUND alone never authorize consumption. The
  * current producer cannot attest topology applicability, so this denies. */
 if(binding.current_bound!=PAPACC_TRUE||binding.topology_proven!=PAPACC_TRUE||binding.resolved!=PAPACC_TRUE)
  return PAPACC_RESULT_NOT_SUPPORTED;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_activate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_C8_ACCEPT_LIFETIME *a;PAPACC_C8_RESOURCE_CONTEXT *c;
 PAPACC_C8_TARGET_EVIDENCE *e;PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry;
 PAPACC_U64 now;PAPACC_SIZE i;PAPACC_RESULT result;
 /* Before ANY activation reservation/allocation/native acquisition. The
  * separate ED11/admission/READY gates below remain mandatory. */
 if(o!=&root||!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
 if(o->config.bind_selection.mode==PAPACC_BIND_SELECTION_SELECTED_INTERFACES){
  result=papacc_server_launcher_interface_consumption_check(o,&o->listener_composition.pin);
  if(result!=PAPACC_RESULT_OK)return result;
 }
 if(o!=&root||!preparation_owned(o)||o->secure_io.dispatch_active||o->activation_lifetime.phase||
    o->listener_composition.state!=PAPACC_ROOT_LISTENER_READY||root_listener_check(o)!=PAPACC_RESULT_OK||
    o->configuration_preparation.participants.target_count!=1U||!o->config.control_port||
    o->network.listener_storage||o->context_storage||o->network.tcp_platform.initialized||
    o->accept_generation==(PAPACC_U64)-1)return PAPACC_RESULT_INVALID_STATE;
 result=root_resource_current(o);if(result!=PAPACC_RESULT_OK)return result;
 if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 a=&o->activation_lifetime;c=&a->context;e=&o->resource_admission.evidence;
 memset(a,0,sizeof(*a));a->socket=(PAPACC_TCP_ACCEPTED_SOCKET_WIN32)PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
 c->invocation=o->invocation;c->owner=o->owner_thread;c->connection=PAPACC_C8_RESOURCE_SLOTS+1U;
 c->generation=++o->accept_generation;c->revision=o->listener_composition.pin.view.revision;c->epoch=o->listener_composition.pin.epoch;
 memcpy(c->domain,o->listener_composition.pin.view.domain,32U);c->policy_current=1U;c->connection_state=1U;
 c->profile=e->profile;c->source_id=e->source_id;c->phase=e->phase;c->origin=2U;
 memcpy(c->target,e->target,32U);memcpy(c->sdk_sha256,e->sdk_sha256,32U);memcpy(c->provenance,e->provenance,32U);
 if(papacc_c8_resource_reserve(&o->resource_admission,c,now/1000000U,&a->ticket)!=0)return PAPACC_RESULT_LIMIT_EXCEEDED;
 a->phase=1U;
 if(papacc_c8_resource_observe_acquisition(&o->resource_admission,c,&a->ticket,NATIVE_SOCKET_NOT_ACQUIRED,PST_CHILD_NOT_CREATED)!=0)
  return PAPACC_RESULT_INVALID_STATE;
 if(root_listener_check(o)!=PAPACC_RESULT_OK||papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK||
    papacc_c8_resource_commit(&o->resource_admission,c,now/1000000U,&a->ticket)!=0){
  if(papacc_c8_resource_rollback(&o->resource_admission,c,&a->ticket)==0)a->phase=0U;
  return PAPACC_RESULT_INVALID_STATE;
 }
 a->phase=2U;c->connection_state=2U;
 /* Arrays are included in known managed storage; this allocates them once. */
 o->context_storage=calloc(PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY,sizeof(*o->context_storage));
 o->network.listener_storage=calloc(1U,sizeof(*o->network.listener_storage));
 if(!o->context_storage||!o->network.listener_storage)goto retained;
 for(i=0U;i<PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY;++i)
  o->context_storage[i]=(PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
 o->network.listener_storage_capacity=1U;entry=o->network.listener_storage;
 *entry=(PAPACC_TCP_LISTENER_ENTRY_WIN32)PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
 entry->target=o->configuration_preparation.participants.targets[0];
 if(papacc_tcp_platform_init(&o->network.tcp_platform)!=PAPACC_RESULT_OK)goto retained;
 /* Revalidate immediately before the first native socket/bind effect. */
 if(root_listener_check(o)!=PAPACC_RESULT_OK||papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK||
    papacc_c8_resource_check(&o->resource_admission,c,now/1000000U)!=0)goto retained;
 if(o->config.bind_selection.mode==PAPACC_BIND_SELECTION_SELECTED_INTERFACES&&
    papacc_server_launcher_interface_consumption_check(o,&o->listener_composition.pin)!=PAPACC_RESULT_OK)goto retained;
 result=papacc_tcp_socket_win32_bind_owned(&o->network.tcp_platform,&entry->target,o->config.control_port,&entry->socket);
 if(papacc_c8_resource_observe_acquisition(&o->resource_admission,c,&a->ticket,
    entry->socket.native_socket==INVALID_SOCKET?NATIVE_SOCKET_NOT_ACQUIRED:NATIVE_SOCKET_ACQUIRED,PST_CHILD_NOT_CREATED)!=0)
  goto retained;
 if(result!=PAPACC_RESULT_OK)goto retained;
 if(root_listener_check(o)!=PAPACC_RESULT_OK||papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK||
    papacc_c8_resource_check(&o->resource_admission,c,now/1000000U)!=0)goto retained;
 if(papacc_tcp_socket_win32_listen(&entry->socket)!=PAPACC_RESULT_OK)goto retained;
 o->network.listener_set.entries=entry;o->network.listener_set.capacity=1U;
 o->network.listener_set.count=1U;o->network.listener_set.bound_port=o->config.control_port;
 if(papacc_pst_external_source_win32_create((PAPACC_SIZE)entry->socket.native_socket,&o->listener_sources[0])!=PAPACC_RESULT_OK)goto retained;
 if(papacc_pst_secure_scheduler_add_external(&o->secure_io.scheduler,o->listener_sources[0],PAPACC_TRUE,PAPACC_FALSE,&o->listener_tokens[0])!=PAPACC_RESULT_OK)goto retained;
 o->secure_io.listener_count=1U;
 /* The manager is the existing initialized closed-attach manager. Binding its
  * already charged context storage does not reinitialize or reset identities. */
 if(!o->acceptor.connection_manager.initialized||o->acceptor.connection_manager.count||
    o->acceptor.connection_manager.storage!=o->connection_storage)goto retained;
 o->acceptor.server_network=&o->network;o->acceptor.transport_context_storage=o->context_storage;
 o->acceptor.transport_context_capacity=PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY;o->acceptor.next_listener_index=0U;
 o->acceptor.initialized=PAPACC_TRUE;o->io_loop.acceptor=&o->acceptor;
 /* Mode2 validates factual staged bindings without publishing active flags. */
 if(root_listener_check_mode(o,2)!=PAPACC_RESULT_OK||papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK||
    papacc_c8_resource_check(&o->resource_admission,c,now/1000000U)!=0)goto retained;
 o->network.listener_set.is_active=PAPACC_TRUE;o->network.is_active=PAPACC_TRUE;
 return PAPACC_RESULT_OK;
retained:
 (void)papacc_c8_resource_retain(&o->resource_admission,c,&a->ticket);a->phase=3U;
 return attachment_retain(o,PAPACC_RESULT_INTERNAL_ERROR);
}
static PAPACC_RESULT root_activation_retire(void)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=&root.activation_lifetime;PAPACC_C8_RESOURCE_TEARDOWN proof;
 PAPACC_TCP_SOCKET_WIN32 *socket;PAPACC_C8_RESOURCE_TICKET *ticket;unsigned i;
 if(!a->phase)return PAPACC_RESULT_OK;
 if(a->phase!=2U||root.secure_io.dispatch_active||root.secure_io.scheduler.member_count||
    root.secure_io.stop_state!=PAPACC_SERVER_SECURE_STOP_DRAINING)return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<root.secure_io.listener_count;++i)if(root.listener_sources[i])return PAPACC_RESULT_INVALID_STATE;
 if(!root.network.listener_storage||root.network.listener_set.count!=1U)return PAPACC_RESULT_INVALID_STATE;
 ticket=resource_find(&root.resource_admission,&a->context,&a->ticket);
 if(!ticket||ticket->state!=PAPACC_C8_RESOURCE_LIVE||ticket->borrowers)return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<PAPACC_C8_RESOURCE_SLOTS;++i)
  if(&root.resource_admission.tickets[i]!=ticket&&root.resource_admission.tickets[i].state&&
     root.resource_admission.tickets[i].state!=PAPACC_C8_RESOURCE_RELEASED)return PAPACC_RESULT_INVALID_STATE;
 socket=&root.network.listener_storage[0].socket;
 if(socket->native_socket==INVALID_SOCKET||papacc_tcp_socket_win32_try_close(socket)!=PAPACC_RESULT_OK){
  (void)papacc_c8_resource_retain(&root.resource_admission,&a->context,&a->ticket);a->phase=3U;
  return attachment_retain(&root,PAPACC_RESULT_INTERNAL_ERROR);
 }
 *socket=(PAPACC_TCP_SOCKET_WIN32)PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
 memset(&proof,0,sizeof(proof));proof.key=a->ticket;proof.native_state=NATIVE_SOCKET_DESTROYED;proof.pst_state=PST_CHILD_NOT_CREATED;
 proof.waitset_removed=proof.transport_destroyed=proof.callbacks_quiescent=proof.leases_quiescent=1U;
 a->context.connection_state=3U;
 /* Reaching here is factual native completion, not a close attempt/status of
  * an unrelated child. Refund still requires no outstanding borrowers. */
 if(papacc_c8_resource_release(&root.resource_admission,&a->context,&proof)!=0){
  (void)papacc_c8_resource_retain(&root.resource_admission,&a->context,&a->ticket);a->phase=3U;return PAPACC_RESULT_INVALID_STATE;
 }
 a->phase=0U;root.network.is_active=PAPACC_FALSE;root.network.listener_set.is_active=PAPACC_FALSE;
 root.network.listener_set.count=0U;return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_launcher_accept_borrow(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,PAPACC_U32 kind,PAPACC_U64 now_ms)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=root_accept_lifetime(candidate);PAPACC_U32 *held;
 if(!a||a->phase!=2U||kind<1U||kind>2U||!papacc_server_launcher_active_cohort_current(&root))return PAPACC_RESULT_INVALID_STATE;
 held=kind==1U?&a->scheduler_borrow:&a->protocol_borrow;
 if(*held||papacc_c8_resource_borrow(&root.resource_admission,&a->context,&a->ticket,NEW_OPERATIONAL_BORROW,now_ms)!=0)
  return PAPACC_RESULT_INVALID_STATE;
 *held=1U;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_accept_bind_connection(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,const PAPACC_CONNECTION *connection)
{
 PAPACC_C8_ACCEPT_LIFETIME *a=root_accept_lifetime(candidate);
 if(!a||a->phase!=2U||!a->protocol_borrow||a->protocol_connection||!connection||
    !connection->connection_instance_id||connection->transport.context!=&candidate->transport_adapter||
    papacc_connection_manager_find(&root.acceptor.connection_manager,connection->connection_instance_id)!=connection)
  return PAPACC_RESULT_INVALID_STATE;
 a->protocol_connection=connection->connection_instance_id;return PAPACC_RESULT_OK;
}
