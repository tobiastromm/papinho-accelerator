#include "../../src/core/ed11_process_profile.h"
/* Private same-root attachment. No application listener/Connection admission. */
#define PAPACC_ATTACHMENT_WAKE_CREDIT 4096U
#ifdef PAPACC_C8_TEST_ATTACHMENT_FAULTS
/* Controlled CONSUMER failures only; never fabricate a PST/Winsock return. */
static PAPACC_U32 attachment_test_fault;
static PAPACC_U64 attachment_test_invocation;
PAPACC_RESULT papacc_server_launcher_attachment_test_fault(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_U32 fault)
{if(!preparation_owned(o)||o->attachment_state||fault>3U)return PAPACC_RESULT_INVALID_STATE;attachment_test_fault=fault;attachment_test_invocation=o->invocation;return PAPACC_RESULT_OK;}
#endif
static PAPACC_RESULT attachment_retain(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_RESULT cause)
{
 PAPACC_SERVER_NETWORK unused=PAPACC_SERVER_NETWORK_INITIALIZER;
 if(cause==PAPACC_RESULT_OK)cause=PAPACC_RESULT_INVALID_STATE;
 o->attachment_state=5U;
 /* This is consumer terminal containment, never a fabricated PST/native error.
  * Preserve any earlier cause/deadline and block direct Secure IO teardown. */
 if(o->secure_io.initialized){
  if(o->secure_io.stop_last_remove_result==PAPACC_RESULT_OK)o->secure_io.stop_last_remove_result=cause;
  o->secure_io.stop_state=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL;
 }
 /* Existing containment clears borrowed logger/return views, with no close. */
 (void)papacc_server_launcher_owner_finish(o,&unused,PAPACC_FALSE);
 return cause;
}
static PAPACC_RESULT attachment_cleanup(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_RESULT result;
 if(o->secure_io.initialized){
  result=papacc_server_secure_io_win32_request_stop(&o->secure_io);if(result!=PAPACC_RESULT_OK)return attachment_retain(o,result);
  if(!papacc_server_secure_io_win32_is_stopped(&o->secure_io)){
   result=papacc_server_secure_io_win32_poll_once(&o->secure_io,0U);
   if(result!=PAPACC_RESULT_OK||!papacc_server_secure_io_win32_is_stopped(&o->secure_io))return attachment_retain(o,PAPACC_RESULT_INVALID_STATE);
  }
  result=papacc_server_secure_io_win32_try_shutdown(&o->secure_io);if(result!=PAPACC_RESULT_OK||o->secure_io.scheduler.initialized)return attachment_retain(o,PAPACC_RESULT_INVALID_STATE);
 }else if(o->secure_io.scheduler.initialized)return attachment_retain(o,PAPACC_RESULT_INVALID_STATE);
 if(configuration_c2_owner.configuration.io_attached&&
    papacc_server_c2_owner_win32_detach_closed(&configuration_c2_owner)!=PAPACC_RESULT_OK)return attachment_retain(o,PAPACC_RESULT_INVALID_STATE);
 if(o->io_loop.initialized){papacc_server_io_loop_win32_shutdown(&o->io_loop);if(o->io_loop.initialized)return attachment_retain(o,PAPACC_RESULT_INVALID_STATE);}
 if(o->acceptor.connection_manager.initialized){PAPACC_BOOL complete=PAPACC_FALSE;
  result=papacc_connection_manager_try_shutdown(&o->acceptor.connection_manager,&complete);
  if(result!=PAPACC_RESULT_OK||!complete)return attachment_retain(o,PAPACC_RESULT_INVALID_STATE);
 }
 result=papacc_server_acceptor_win32_try_shutdown(&o->acceptor);
 if(result!=PAPACC_RESULT_OK)return attachment_retain(o,result);
 o->acceptor.initialized=PAPACC_FALSE;o->acceptor.server_network=NULL;
 o->acceptor.transport_context_storage=NULL;o->acceptor.transport_context_capacity=0U;
 free(o->context_storage);o->context_storage=NULL;
 result=papacc_server_network_try_shutdown(&o->network);
 if(result!=PAPACC_RESULT_OK)return attachment_retain(o,result);
 free(o->connection_storage);o->connection_storage=NULL;free(o->session_storage);o->session_storage=NULL;
 free(o->channel_storage);o->channel_storage=NULL;free(o->association_storage);o->association_storage=NULL;
 free(o->processor_storage);o->processor_storage=NULL;free(o->secure_candidates);o->secure_candidates=NULL;
 free(o->secure_members);o->secure_members=NULL;free(o->secure_events);o->secure_events=NULL;
 free(o->listener_sources);o->listener_sources=NULL;free(o->listener_tokens);o->listener_tokens=NULL;
 if(o->attachment_pin.pin.ticket){
  result=papacc_server_launcher_configuration_current_unpin(o,&o->attachment_pin);if(result!=PAPACC_RESULT_OK)return attachment_retain(o,result);
  memset(&o->attachment_pin,0,sizeof(o->attachment_pin));
 }
 o->attachment_state=4U;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_attach_closed(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_SIZE reservation)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_ROOT_CURRENT_PIN pin;PAPACC_RESULT result;
 PAPACC_SIZE capacity=PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY,sched=capacity+PAPACC_SERVER_NETWORK_STREAM_CAPACITY+1U,i;
 if(!preparation_owned(o)||o->attachment_state||!root_dormant_owned(o))return PAPACC_RESULT_INVALID_STATE;
 t=&o->configuration_preparation;
 if(!papacc_security_configuration_ref_equal(&t->participants.listener.security_configuration_ref,&t->source.reference))return PAPACC_RESULT_INVALID_STATE;
 if(t->participants.listener.bind_selection.interface_persistent_id_count!=o->config.bind_selection.interface_persistent_id_count)return PAPACC_RESULT_INVALID_STATE;
 if(t->participants.listener.bind_selection.interface_persistent_id_count){
  if(!t->participants.listener.bind_selection.interface_persistent_ids)return PAPACC_RESULT_INVALID_STATE;
  if(t->participants.listener.bind_selection.interface_persistent_id_count>PAPACC_CONFIG_IDS)return PAPACC_RESULT_INVALID_STATE;
  for(i=0U;i<t->participants.listener.bind_selection.interface_persistent_id_count;++i)
   if(t->participants.listener.bind_selection.interface_persistent_ids[i].is_valid!=o->config.bind_selection.interface_persistent_ids[i].is_valid||
      t->participants.listener.bind_selection.interface_persistent_ids[i].value!=o->config.bind_selection.interface_persistent_ids[i].value)return PAPACC_RESULT_INVALID_STATE;
 }
 if(!t->administrative_published||!t->normative_committed||!t->dormant_applied||
    t->participants.listener.port!=o->config.control_port||
    t->participants.listener.transport_profile!=PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL||
    t->participants.listener.bind_selection.mode!=o->config.bind_selection.mode||
    t->participants.listener.bind_selection.interface_persistent_id_count!=o->config.bind_selection.interface_persistent_id_count||
    papacc_server_c2_configuration_validate(&configuration_c2_owner)!=PAPACC_RESULT_OK||
    t->combined_reserved>PAPACC_ED11_PROCESS_LIMIT-PAPACC_ATTACHMENT_WAKE_CREDIT||
    reservation!=t->combined_reserved+PAPACC_ATTACHMENT_WAKE_CREDIT)return PAPACC_RESULT_INVALID_STATE;
 memset(&pin,0,sizeof(pin));result=papacc_server_launcher_configuration_current_pin(o,&pin);
 if(result!=PAPACC_RESULT_OK)return result;
 if(!papacc_server_launcher_configuration_current(o,&pin)||pin.epoch!=configuration_c2_owner.configuration.epoch){
  result=papacc_server_launcher_configuration_current_unpin(o,&pin);
  if(result!=PAPACC_RESULT_OK){o->attachment_pin=pin;
   o->attachment_pin.view.config.bind_selection.interface_persistent_ids=pin.view.config.bind_selection.interface_persistent_id_count?o->attachment_pin.view.ids:NULL;
   return attachment_retain(o,PAPACC_RESULT_INVALID_STATE);}
  return PAPACC_RESULT_INVALID_STATE;
 }
 o->attachment_pin=pin;
 o->attachment_pin.view.config.bind_selection.interface_persistent_ids=pin.view.config.bind_selection.interface_persistent_id_count?o->attachment_pin.view.ids:NULL;
 o->attachment_state=1U;
 /* Price native empty wait-set payload/observer metadata before creation.
  * No member is registered; allocator/kernel overhead stays UNKNOWN. */
 t->combined_reserved=reservation;t->source.other_reserved+=PAPACC_ATTACHMENT_WAKE_CREDIT;
 configuration_c2_owner.configuration.reserved=reservation;
 o->connection_storage=calloc(capacity,sizeof(*o->connection_storage));o->session_storage=calloc(capacity,sizeof(*o->session_storage));
 o->channel_storage=calloc(capacity,sizeof(*o->channel_storage));o->association_storage=calloc(capacity,sizeof(*o->association_storage));
 o->processor_storage=calloc(capacity,sizeof(*o->processor_storage));o->secure_candidates=calloc(capacity,sizeof(*o->secure_candidates));
 o->secure_members=calloc(sched,sizeof(*o->secure_members));o->secure_events=calloc(sched,sizeof(*o->secure_events));
 o->listener_sources=calloc(1U,sizeof(*o->listener_sources));o->listener_tokens=calloc(1U,sizeof(*o->listener_tokens));
 result=PAPACC_RESULT_OUT_OF_MEMORY;
 if(!o->connection_storage||!o->session_storage||!o->channel_storage||!o->association_storage||!o->processor_storage||
    !o->secure_candidates||!o->secure_members||!o->secure_events||!o->listener_sources||!o->listener_tokens)goto fail;
 result=papacc_connection_manager_init(&o->acceptor.connection_manager,o->connection_storage,capacity);if(result!=PAPACC_RESULT_OK)goto fail;
 result=papacc_server_io_loop_win32_init_closed(&o->io_loop,&o->network,NULL,o->session_storage,capacity,o->channel_storage,capacity,
  o->association_storage,capacity,o->processor_storage,capacity,1000000000ULL,&o->acceptor.connection_manager);if(result!=PAPACC_RESULT_OK)goto fail;
 /* Replace historical implicit enablement by this exact prepared cohort. */
 papacc_capability_set_release(&o->io_loop.supported_capabilities);papacc_capability_set_release(&o->io_loop.enabled_capabilities);
 result=papacc_capability_set_init(&o->io_loop.supported_capabilities,o->io_loop.supported_capability_keys,PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
 if(result==PAPACC_RESULT_OK)result=papacc_capability_set_init(&o->io_loop.enabled_capabilities,o->io_loop.enabled_capability_keys,PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
 {PAPACC_CAPABILITY_KEY key={3U,1U};if(result==PAPACC_RESULT_OK)result=papacc_capability_registry_register(&o->io_loop.capability_registry,&key,"MODE_C_OPERATION");}
 for(i=0U;result==PAPACC_RESULT_OK&&i<t->supported.count;++i)result=papacc_capability_set_insert(&o->io_loop.supported_capabilities,&t->supported.storage[i]);
 for(i=0U;result==PAPACC_RESULT_OK&&i<t->enabled.count;++i)result=papacc_capability_set_insert(&o->io_loop.enabled_capabilities,&t->enabled.storage[i]);
 if(result!=PAPACC_RESULT_OK)goto fail;
#ifdef PAPACC_C8_TEST_ATTACHMENT_FAULTS
 if(attachment_test_invocation==o->invocation&&attachment_test_fault==1U){result=PAPACC_RESULT_INTERNAL_ERROR;goto fail;}
#endif
 o->secure_listener=t->participants.listener;o->secure_listener_set.listeners=&o->secure_listener;o->secure_listener_set.count=1U;
 result=papacc_server_secure_io_win32_init_closed(&o->secure_io,&o->network,&o->secure_listener_set,&o->io_loop,
  &o->security_composition,&t->source.configuration,o->secure_candidates,capacity,o->secure_members,o->secure_events,sched,
  o->listener_sources,o->listener_tokens,1U,1000000000ULL);if(result!=PAPACC_RESULT_OK)goto fail;
#ifdef PAPACC_C8_TEST_ATTACHMENT_FAULTS
 if(attachment_test_invocation==o->invocation&&attachment_test_fault==2U){result=PAPACC_RESULT_INTERNAL_ERROR;goto fail;}
#endif
 if(!papacc_server_launcher_configuration_current(o,&o->attachment_pin)){result=PAPACC_RESULT_INVALID_STATE;goto fail;}
 o->attachment_state=2U;result=papacc_server_c2_owner_win32_attach_closed(&configuration_c2_owner,&o->io_loop,&o->secure_io);
 if(result!=PAPACC_RESULT_OK)goto fail;
 result=configuration_site_wire(o);if(result!=PAPACC_RESULT_OK)goto fail;
 o->attachment_state=3U;return PAPACC_RESULT_OK;
fail:
 if(attachment_cleanup(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 return result;
}
PAPACC_RESULT papacc_server_launcher_attachment_wake(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_RESULT result;
 if(!preparation_owned(o)||o->attachment_state!=3U||
    !root_operational_reference_current(o,&o->attachment_pin)||!o->secure_io.initialized||
    o->secure_io.stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING)return PAPACC_RESULT_INVALID_STATE;
#ifdef PAPACC_C8_TEST_ATTACHMENT_FAULTS
 if(attachment_test_invocation==o->invocation&&attachment_test_fault==3U)return attachment_retain(o,PAPACC_RESULT_INVALID_STATE); /* No PST call made. */
#endif
 result=papacc_pst_secure_scheduler_wake(&o->secure_io.scheduler);
 if(result!=PAPACC_RESULT_OK)return attachment_retain(o,result);
 return result;
}
PAPACC_RESULT papacc_server_launcher_detach_closed(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 if(!preparation_owned(o)||o->listener_composition.pin.pin.ticket||o->attachment_state!=3U||!o->attachment_pin.pin.ticket||
    !root_snapshot_held(o,&o->attachment_pin))return PAPACC_RESULT_INVALID_STATE;
 return attachment_cleanup(o);
}
