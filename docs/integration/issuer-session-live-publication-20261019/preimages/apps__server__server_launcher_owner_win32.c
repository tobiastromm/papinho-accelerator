#include "server_launcher_owner_win32.h"
#include "server_application_lifetime_win32.h"
#include "server_c2_participant_win32.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
#include "server_c8_private_storage_layout_win32.h"
#endif
/* One bounded process-lifetime root, not a registry of orphan allocations. */
static PAPACC_SERVER_LAUNCHER_OWNER_WIN32 root;
static volatile LONG state;
static PAPACC_U64 next_invocation;
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
/* The one manager already included by the full C8 layout census. Never a
 * second authority: this is the durable root's dormant zero-Session child. */
static PAPACC_SERVER_C2_PARTICIPANT configuration_c2_participant;
#define configuration_c2 (configuration_c2_participant.manager)
static PAPACC_SERVER_C2_OWNER_WIN32 configuration_c2_owner;
/* Actual instances of the pools already charged in the existing full layout. */
static PAPACC_C3_BRIDGE configuration_c3_bridges[2];
static PAPACC_CONNECTION_MANAGER configuration_c3_manager;
static PAPACC_CONNECTION configuration_c3_connections[2];
static PAPACC_RESULT configuration_site_prepare(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
static PAPACC_RESULT configuration_site_cleanup(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
static PAPACC_RESULT configuration_site_wire(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
static PAPACC_RESULT configuration_dormant_unapply(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
static PAPACC_RESULT configuration_root_commit(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
static PAPACC_RESULT configuration_participants_rollback(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
#endif
static void launcher_log(void *context,const PAPACC_LOG_RECORD *record)
{
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o = context;
    /* The external sink is borrowed only during the synchronous launcher call.
     * A late provider callback sees a durable proxy, never the old sink context.
     * Provider callbacks on another thread cannot call the borrowed sink. */
    if(o != &root || GetCurrentThreadId()!=o->owner_thread ||
        InterlockedCompareExchange(&state,0,0)!=PAPACC_LAUNCHER_ACTIVE)return;
    if(o->forward_logger.sink!=NULL)
        o->forward_logger.sink(o->forward_logger.sink_context,record);
}
PAPACC_U32 papacc_server_launcher_owner_state(void)
{return (PAPACC_U32)InterlockedCompareExchange(&state,0,0);}
PAPACC_RESULT papacc_server_launcher_owner_acquire(
    PAPACC_SERVER_NETWORK *network,const PAPACC_SERVER_CONFIG *config,
    const PAPACC_LOGGER *logger,PAPACC_SERVER_LAUNCHER_OWNER_WIN32 **out)
{
    PAPACC_SIZE count,bytes,reservation,remaining,listener_count,index;
    LONG previous;
    if(out!=NULL)*out=NULL;
    if(network==NULL||config==NULL||logger==NULL||out==NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if(sizeof(root)>PAPACC_LAUNCHER_MANAGED_LIMIT)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    /* Reserve all known managed launcher storage before ownership transfer or
     * allocation. PST/provider/kernel allocations remain external ED11 gates. */
    reservation=sizeof(root)+
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
        PAPACC_PENDING_STACK_BYTES+
#endif
        PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY*(
        sizeof(*root.connection_storage)+sizeof(*root.context_storage)+
        sizeof(*root.session_storage)+sizeof(*root.channel_storage)+
        sizeof(*root.association_storage)+sizeof(*root.processor_storage));
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    reservation+=PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY*sizeof(*root.secure_candidates);
    reservation+=(PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY+
        PAPACC_SERVER_NETWORK_STREAM_CAPACITY)*
        (sizeof(*root.secure_members)+sizeof(*root.secure_events));
#endif
    if(reservation>PAPACC_LAUNCHER_MANAGED_LIMIT)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    remaining=PAPACC_LAUNCHER_MANAGED_LIMIT-reservation;
    if(network->listener_storage_capacity>remaining/sizeof(*network->listener_storage))
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    reservation+=network->listener_storage_capacity*sizeof(*network->listener_storage);
    remaining=PAPACC_LAUNCHER_MANAGED_LIMIT-reservation;
    listener_count=network->listener_set.count;
    if(listener_count>network->listener_set.capacity||
        listener_count>network->listener_storage_capacity||
        (listener_count!=0U && (network->listener_set.entries==NULL||
         network->listener_set.entries!=network->listener_storage)))
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if(listener_count>remaining/sizeof(*root.listener_copy))
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    reservation+=listener_count*sizeof(*root.listener_copy);
    remaining=PAPACC_LAUNCHER_MANAGED_LIMIT-reservation;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    if(listener_count>remaining/(sizeof(*root.secure_members)+
        sizeof(*root.secure_events)+sizeof(*root.listener_sources)+sizeof(*root.listener_tokens)))
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    reservation+=listener_count*(sizeof(*root.secure_members)+
        sizeof(*root.secure_events)+sizeof(*root.listener_sources)+sizeof(*root.listener_tokens));
#else
    (void)listener_count;
#endif
    remaining=PAPACC_LAUNCHER_MANAGED_LIMIT-reservation;
    count=config->bind_selection.interface_persistent_id_count;
    if(count>remaining/sizeof(*root.bind_ids)||
        (count!=0U&&config->bind_selection.interface_persistent_ids==NULL))
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    previous=InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,0);
    if(previous!=0 && (previous!=PAPACC_LAUNCHER_FINISHED ||
        InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,
            PAPACC_LAUNCHER_FINISHED)!=PAPACC_LAUNCHER_FINISHED))
        return PAPACC_RESULT_INVALID_STATE;
    /* Normal finish guarantees no remaining users. Retained never reaches here. */
    if(next_invocation==(PAPACC_U64)-1){
        InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    memset(&root,0,sizeof(root));root.invocation=++next_invocation;
    root.owner_thread=GetCurrentThreadId();
    bytes=count*sizeof(*root.bind_ids);
    root.reserved_bytes=reservation+bytes;
    if(bytes!=0U){
        root.bind_ids=malloc(bytes);
        if(root.bind_ids==NULL){
            InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);
            return PAPACC_RESULT_OUT_OF_MEMORY;
        }
        memcpy(root.bind_ids,config->bind_selection.interface_persistent_ids,bytes);
    }
    if(listener_count!=0U){
        root.listener_copy=malloc(listener_count*sizeof(*root.listener_copy));
        if(root.listener_copy==NULL){
            free(root.bind_ids);root.bind_ids=NULL;
            InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);
            return PAPACC_RESULT_OUT_OF_MEMORY;
        }
        memcpy(root.listener_copy,network->listener_set.entries,
            listener_count*sizeof(*root.listener_copy));
    }
    root.config=*config;
    root.config.bind_selection.interface_persistent_ids=root.bind_ids;
    root.forward_logger=*logger;
    root.logger.sink=launcher_log;root.logger.sink_context=&root;
    root.logger.minimum_level=logger->minimum_level;
    root.acceptor=(PAPACC_SERVER_ACCEPTOR_WIN32)PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    root.io_loop=(PAPACC_SERVER_IO_LOOP_WIN32)PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    root.egress_source=(PAPACC_SERVER_EGRESS_SOURCE_WIN32)PAPACC_SERVER_EGRESS_SOURCE_WIN32_INITIALIZER;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    root.resolved_outbound_tls=(PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE)PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE_INITIALIZER;
    root.pst_log_adapter=(PAPACC_PST_LOG_ADAPTER)PAPACC_PST_LOG_ADAPTER_INITIALIZER;
    root.secure_listener=(PAPACC_SERVER_LISTENER_CONFIGURATION)PAPACC_SERVER_LISTENER_CONFIGURATION_INITIALIZER;
    root.secure_listener_set=(PAPACC_SERVER_LISTENER_CONFIGURATION_SET)PAPACC_SERVER_LISTENER_CONFIGURATION_SET_INITIALIZER;
    root.resolved_security=(PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION)PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION_INITIALIZER;
    root.security_composition=(PAPACC_SECURITY_COMPOSITION)PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    root.secure_io=(PAPACC_SERVER_SECURE_IO_WIN32)PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
#endif
    root.return_network=*network;root.network=*network;
    root.network.listener_storage=root.listener_copy;
    root.network.listener_storage_capacity=listener_count;
    root.network.listener_set.entries=root.listener_copy;
    root.network.listener_set.capacity=listener_count;
    /* Invalidate old socket aliases without closing transferred sockets. The
     * caller keeps only ownership of its original storage allocation. */
    for(index=0U;index<listener_count;++index)
        network->listener_set.entries[index]=(PAPACC_TCP_LISTENER_ENTRY_WIN32)
            PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    *network=(PAPACC_SERVER_NETWORK)PAPACC_SERVER_NETWORK_INITIALIZER;
    network->listener_storage=root.return_network.listener_storage;
    network->listener_storage_capacity=root.return_network.listener_storage_capacity;
    *out=&root;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_owner_finish(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_SERVER_NETWORK *network,
    PAPACC_BOOL quiescent)
{
    if(o!=&root||network==NULL||o->owner_thread!=GetCurrentThreadId()||
        papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE)
        return PAPACC_RESULT_INVALID_STATE;
    /* Only confirmed teardown returns listener/WinSock ownership. Retention
     * clears every borrowed return view and has no cleanup/reuse path. */
    if(quiescent==PAPACC_TRUE &&
        (o->io_loop.initialized||o->acceptor.initialized||
         o->acceptor.connection_manager.initialized||
         o->io_loop.network_job_manager.initialized||
         o->connection_storage||o->context_storage||o->session_storage||
         o->channel_storage||o->association_storage||o->processor_storage))
        quiescent=PAPACC_FALSE;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    if(quiescent==PAPACC_TRUE &&
        (o->secure_io.initialized||o->secure_io.dispatch_active||
         o->secure_io.scheduler.initialized||o->secure_candidates||
         o->secure_members||o->secure_events||o->listener_sources||
         o->listener_tokens||o->pst_runtime_instance||o->pst_system_trust||
         o->security_composition.runtime||o->resolved_security.valid))
        quiescent=PAPACC_FALSE;
#endif
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    if(o->configuration_commit.initialized)quiescent=PAPACC_FALSE;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    {unsigned resource_slot;
     if(o->resource_admission.managed_used||o->resource_admission.user_used||o->resource_admission.kernel_used)
         quiescent=PAPACC_FALSE;
     for(resource_slot=0U;resource_slot<PAPACC_C8_RESOURCE_SLOTS;++resource_slot)
         if((o->resource_admission.tickets[resource_slot].state&&o->resource_admission.tickets[resource_slot].state!=PAPACC_C8_RESOURCE_RELEASED)||o->resource_admission.tickets[resource_slot].borrowers)
             quiescent=PAPACC_FALSE;
    }
    if(o->configuration_preparation.state!=PAPACC_ROOT_PREPARATION_EMPTY &&
       o->configuration_preparation.state!=PAPACC_ROOT_PREPARATION_ROLLED_BACK)
        if(o->configuration_preparation.state!=PAPACC_ROOT_PREPARATION_CLOSED)
            quiescent=PAPACC_FALSE;
#endif
#endif
    o->forward_logger.sink=NULL;o->forward_logger.sink_context=NULL;
    if(quiescent!=PAPACC_TRUE){
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
        if(o->configuration_preparation.state==PAPACC_ROOT_PREPARATION_PARTIAL){
            (void)papacc_server_launcher_configuration_preparation_retain(o);
            o->configuration_preparation.state=PAPACC_ROOT_PREPARATION_RETAINED;
        }
#endif
        o->return_network=(PAPACC_SERVER_NETWORK)PAPACC_SERVER_NETWORK_INITIALIZER;
        InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);
        return PAPACC_RESULT_INVALID_STATE;
    }
    if(o->network.listener_set.count!=0U)
        memcpy(o->return_network.listener_set.entries,o->listener_copy,
            o->network.listener_set.count*sizeof(*o->listener_copy));
    *network=o->return_network;
    o->return_network=(PAPACC_SERVER_NETWORK)PAPACC_SERVER_NETWORK_INITIALIZER;
    o->network=(PAPACC_SERVER_NETWORK)PAPACC_SERVER_NETWORK_INITIALIZER;
    free(o->listener_copy);o->listener_copy=NULL;
    free(o->bind_ids);o->bind_ids=NULL;
    o->config.bind_selection.interface_persistent_ids=NULL;
    InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);
    return PAPACC_RESULT_OK;
}

#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
static int configuration_owned_sets(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    const PAPACC_CAPABILITY_SET *sets[2];
    const PAPACC_CAPABILITY_DESCRIPTOR *found;
    PAPACC_SIZE i,j;
    sets[0]=&o->io_loop.supported_capabilities;
    sets[1]=&o->io_loop.enabled_capabilities;
    if(o->io_loop.capability_registry.initialized!=PAPACC_TRUE ||
        o->io_loop.capability_registry.storage!=o->io_loop.capability_descriptors ||
        o->io_loop.capability_registry.capacity>PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS ||
        o->io_loop.capability_registry.count>o->io_loop.capability_registry.capacity)return 0;
    for(i=0U;i<2U;++i){
        if(sets[i]->initialized!=PAPACC_TRUE ||
            sets[i]->storage!=(i==0U?o->io_loop.supported_capability_keys:o->io_loop.enabled_capability_keys) ||
            sets[i]->capacity>PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS || sets[i]->count>sets[i]->capacity)return 0;
        for(j=0U;j<sets[i]->count;++j){
            if(papacc_capability_registry_find(&o->io_loop.capability_registry,
                &sets[i]->storage[j],&found)!=PAPACC_RESULT_OK)return 0;
            if(i==1U && !papacc_capability_set_contains(sets[0],&sets[i]->storage[j]))return 0;
        }
    }
    return 1;
}
PAPACC_RESULT papacc_server_launcher_configuration_preflight(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_CONFIG_PARTICIPANT_REPORT *out)
{
    PAPACC_SERVER_LISTENER_CONFIGURATION intent=PAPACC_SERVER_LISTENER_CONFIGURATION_INITIALIZER;
    const PAPACC_U32 required=1023U;
    if(out==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    memset(out,0,sizeof(*out));out->missing=required;
    out->model_result=out->listener_result=out->security_result=PAPACC_RESULT_INVALID_STATE;
    if(o!=&root || GetCurrentThreadId()!=o->owner_thread ||
        papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE)return PAPACC_RESULT_INVALID_STATE;
    out->model_result=papacc_server_config_validate(&o->config);
    if(out->model_result!=PAPACC_RESULT_OK)return out->model_result;
    out->validated|=PAPACC_CFG_MODEL;
    if(o->config.listener_profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL)
        return PAPACC_RESULT_NOT_SUPPORTED;
    intent.bind_selection=o->config.bind_selection;intent.port=o->config.control_port;
    intent.transport_profile=PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL;
    out->listener_result=papacc_security_configuration_ref_set(&intent.security_configuration_ref,
        o->config.security_configuration_ref,o->config.security_configuration_ref_length);
    if(out->listener_result==PAPACC_RESULT_OK)
        out->listener_result=papacc_server_listener_configuration_validate(&intent);
    if(out->listener_result==PAPACC_RESULT_OK)out->validated|=PAPACC_CFG_LISTENER_INTENT;
    out->security_result=papacc_resolved_server_security_configuration_validate(&o->resolved_security);
    if(out->security_result==PAPACC_RESULT_OK &&
        o->security_composition.state==PAPACC_SECURITY_COMPOSITION_READY &&
        o->security_composition.runtime!=NULL && o->security_composition.local_credentials!=NULL &&
        o->security_composition.peer_trust!=NULL &&
        papacc_security_configuration_ref_equal(&intent.security_configuration_ref,
            &o->secure_listener.security_configuration_ref))out->validated|=PAPACC_CFG_SECURITY_LEASE;
    if(o->resolved_security.principal_resolve!=NULL)out->validated|=PAPACC_CFG_PRINCIPAL_RESOLVER;
    if(o->resolved_security.authorize!=NULL)out->validated|=PAPACC_CFG_AUTHORIZATION_PROVIDER;
    /* Registry facts are not capability grants. */
    if(o->io_loop.capability_registry.initialized==PAPACC_TRUE &&
        o->io_loop.capability_registry.storage==o->io_loop.capability_descriptors &&
        o->io_loop.capability_registry.count<=o->io_loop.capability_registry.capacity &&
        o->io_loop.capability_registry.capacity<=PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS)
        out->validated|=PAPACC_CFG_REGISTRY;
    if(configuration_owned_sets(o))out->validated|=PAPACC_CFG_CAPABILITY_SETS;
    if(o->io_loop.c2_owner!=NULL)out->validated|=PAPACC_CFG_C2_OWNER_PRESENT;
    /* A diagnostic record alone is not the real participant commit. No pin,
     * issuance or publication bookkeeping is changed by this audit. */
    /* No existing committed-config/current-policy producer is wired here.
     * Callback presence or captured Session epoch MUST NOT fabricate this fact.
     * Never call commit/publish or enable C8 on incomplete participant proof. */
    out->missing=required&~out->validated;
    return PAPACC_RESULT_NOT_SUPPORTED;
}
#endif

#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
static PAPACC_RESULT configuration_recover_profile(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const char *path,int profile)
{
    PAPACC_RESULT result;
    if(o!=&root || GetCurrentThreadId()!=o->owner_thread ||
        papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE)
        return PAPACC_RESULT_INVALID_STATE;
    /* No overwrite of a live, retained or previously used configuration owner.
     * Every accepted output is a by-value diagnostic model, never a lease. */
    if(o->configuration_commit.initialized||o->configuration_commit.writer!=0U||
       o->configuration_preparation.state!=PAPACC_ROOT_PREPARATION_EMPTY||
       o->network.is_active||o->network.tcp_platform.initialized||o->network.listener_set.count||
       o->acceptor.initialized||o->acceptor.connection_manager.initialized||
       o->io_loop.initialized||o->io_loop.c2_owner||o->io_loop.network_job_manager.initialized||
       o->connection_storage||o->context_storage||o->session_storage||o->channel_storage||
       o->association_storage||o->processor_storage||o->secure_io.initialized||o->secure_io.dispatch_active||
       o->secure_io.scheduler.initialized||o->secure_candidates||o->secure_members||o->secure_events||
       o->listener_sources||o->listener_tokens||o->pst_runtime_instance||o->pst_system_trust||
       o->security_composition.runtime||o->security_composition.local_credentials||
       o->security_composition.peer_trust||o->security_composition.log_adapter||o->resolved_security.valid||
       o->resolved_security.release||o->resolved_security.release_context||
       o->resolved_security.principal_resolve||o->resolved_security.principal_resolve_context||
       o->resolved_security.authorize||o->resolved_security.authorization_context)
        return PAPACC_RESULT_INVALID_STATE;
    result=profile==4?papacc_configuration_recovery_init04(&o->configuration_commit,path):
        profile==3?papacc_configuration_recovery_init03(&o->configuration_commit,path):
        papacc_configuration_recovery_init02(&o->configuration_commit,path);
    if(result!=PAPACC_RESULT_OK)return result;
    return papacc_configuration_recovery_inspect(&o->configuration_commit,path);
}
PAPACC_RESULT papacc_server_launcher_configuration_recover(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const char *path)
{return configuration_recover_profile(o,path,2);}
PAPACC_RESULT papacc_server_launcher_configuration_recover03(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const char *path)
{return configuration_recover_profile(o,path,3);}
PAPACC_RESULT papacc_server_launcher_configuration_recover04(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const char *path)
{return configuration_recover_profile(o,path,4);}
#endif

#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
static int root_snapshot_model_equal(const PAPACC_CONFIG_VIEW *,const PAPACC_CONFIG_VIEW *);
static int root_recovery_candidate_matches(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_CONFIGURATION_COMMIT *c=&o->configuration_commit;PAPACC_CONFIG_VIEW candidate;
 if(!c->initialized||!c->recovery_guard||!c->recovery_selected||(c->store02!=3&&c->store02!=4)||
    c->unresolved||c->stopped||c->published||c->prepared||c->committed_pending)return 0;
 memset(&candidate,0,sizeof(candidate));candidate.config=o->config;
 if(o->config.bind_selection.interface_persistent_id_count>PAPACC_CONFIG_IDS)return 0;
 if(o->config.bind_selection.interface_persistent_id_count)
  memcpy(candidate.ids,o->config.bind_selection.interface_persistent_ids,
   o->config.bind_selection.interface_persistent_id_count*sizeof(candidate.ids[0]));
 return root_snapshot_model_equal(&candidate,&c->slots[c->current].view);
}
static int preparation_owned(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    return o==&root && GetCurrentThreadId()==o->owner_thread &&
        papacc_server_launcher_owner_state()==PAPACC_LAUNCHER_ACTIVE;
}
/* No policy provider is introduced. This bounded candidate belongs to the
 * same configuration root; no pointer received from a caller is retained. */
static int root_policy_model_valid(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t=&o->configuration_preparation;
    PAPACC_SERVER_CONFIG current=o->config, prepared=t->model.config;
    PAPACC_SIZE count=current.bind_selection.interface_persistent_id_count;
    if(papacc_server_config_validate(&current)!=PAPACC_RESULT_OK ||
       papacc_server_config_validate(&prepared)!=PAPACC_RESULT_OK ||
       count>PAPACC_CONFIG_IDS || count!=prepared.bind_selection.interface_persistent_id_count)
        return 0;
    current.bind_selection.interface_persistent_ids=NULL;
    prepared.bind_selection.interface_persistent_ids=NULL;
    /* Exact prepared-view fence, not semantic equality or a revision issuer. */
    if(memcmp(&current,&prepared,sizeof(current))!=0)return 0;
    if(count && memcmp(o->config.bind_selection.interface_persistent_ids,
       t->model.ids,count*sizeof(t->model.ids[0]))!=0)return 0;
    return 1;
}
static int root_policy_inputs_valid(PAPACC_ROOT_CONFIGURATION_PREPARATION *t)
{
    return t->registry.initialized && t->registry.storage==t->descriptors &&
        t->registry.capacity==PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS &&
        t->registry.count<=t->registry.capacity &&
        t->policy_inputs.registry==&t->registry &&
        t->policy_inputs.server_supported==&t->supported &&
        t->policy_inputs.server_enabled==&t->enabled &&
        t->supported.initialized && t->supported.storage==t->supported_keys &&
        t->supported.capacity==PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS &&
        t->supported.count<=t->supported.capacity &&
        t->enabled.initialized && t->enabled.storage==t->enabled_keys &&
        t->enabled.capacity==PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS &&
        t->enabled.count<=t->enabled.capacity;
}
PAPACC_RESULT papacc_server_launcher_configuration_policy_configure(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ROOT_POLICY_INTENT *intent)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
    const PAPACC_CAPABILITY_DESCRIPTOR *descriptor;
    PAPACC_SIZE i,j;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    if(intent==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    t=&o->configuration_preparation;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL || t->invocation!=o->invocation ||
       t->policy.configured || o->configuration_commit.initialized ||
       o->network.is_active || o->network.tcp_platform.initialized || o->io_loop.initialized ||
       o->io_loop.c2_owner || o->secure_io.initialized || o->acceptor.initialized ||
       !root_policy_model_valid(o) || !root_policy_inputs_valid(t) ||
       papacc_server_security_source_validate(&t->source)!=PAPACC_RESULT_OK)
        return PAPACC_RESULT_INVALID_STATE;
    if(intent->enabled_count>PAPACC_ROOT_POLICY_KEYS || intent->rule_count>PAPACC_ROOT_POLICY_RULES)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    if(intent->profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL ||
       intent->profile!=t->model.config.listener_profile ||
       intent->listener_port!=t->model.config.control_port ||
       (intent->admission!=PAPACC_ROOT_POLICY_OPEN && intent->admission!=PAPACC_ROOT_POLICY_CLOSED))
        return PAPACC_RESULT_INVALID_ARGUMENT;
    for(i=0U;i<intent->enabled_count;++i){
        if(papacc_capability_registry_find(&t->registry,&intent->enabled[i],&descriptor)!=PAPACC_RESULT_OK)
            return PAPACC_RESULT_NOT_SUPPORTED;
        for(j=0U;j<i;++j)if(papacc_capability_key_compare(&intent->enabled[i],&intent->enabled[j])==0)
            return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    for(i=0U;i<intent->rule_count;++i){
        if(papacc_capability_registry_find(&t->registry,&intent->rules[i].key,&descriptor)!=PAPACC_RESULT_OK ||
           intent->rules[i].decision!=PAPACC_CAPABILITY_POLICY_DENY || intent->rules[i].unsupported_conditions)
            return PAPACC_RESULT_NOT_SUPPORTED;
    }
    /* This write is after the source transaction's complete fixed-layout
     * reserve. No new storage, callback, policy epoch or effect is allocated. */
    t->policy.intent=*intent;t->policy.configured_model=t->model.config;
    t->policy.invocation=o->invocation;t->policy.configured=PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_policy_prepare(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_SIZE i;PAPACC_RESULT result;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL || t->invocation!=o->invocation ||
       !t->policy.configured || t->policy.prepared || t->policy.invocation!=o->invocation ||
       (o->configuration_commit.initialized&&(!t->policy.from_model||!root_recovery_candidate_matches(o))) || o->network.is_active ||
       o->network.tcp_platform.initialized || o->io_loop.initialized || o->io_loop.c2_owner ||
       o->secure_io.initialized || o->acceptor.initialized || !root_policy_model_valid(o) ||
       !root_policy_inputs_valid(t) || memcmp(&t->policy.configured_model,&t->model.config,
       sizeof(t->model.config))!=0 || !t->runtime_attempted || t->runtime_result!=PAPACC_RESULT_OK ||
       !t->provider_attempted || t->provider_result!=PST_RESULT_OK || !t->provider_connection ||
       !t->runtime_provider.available || !t->runtime_provider.initialized ||
       papacc_server_security_source_validate(&t->source)!=PAPACC_RESULT_OK)
        return PAPACC_RESULT_INVALID_STATE;
    /* Immutable CONFIGURATION snapshot only, never a Session/grant snapshot.
     * Enabled intent is NOT installed in current operational enabled sets. */
    result=papacc_capability_set_init(&t->policy.snapshot,t->policy.snapshot_keys,PAPACC_ROOT_POLICY_KEYS);
    for(i=0U;result==PAPACC_RESULT_OK && i<t->policy.intent.enabled_count;++i)
        result=papacc_capability_set_insert(&t->policy.snapshot,&t->policy.intent.enabled[i]);
    if(result!=PAPACC_RESULT_OK){papacc_capability_set_release(&t->policy.snapshot);return result;}
    t->policy.prepared=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_policy_prepare_model(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;const PAPACC_ADMIN_POLICY *p;PAPACC_SIZE i;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;p=&t->model.config.policy;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL || t->invocation!=o->invocation ||
       t->policy.configured || !root_policy_model_valid(o) || !root_policy_inputs_valid(t) ||
       !p->present || papacc_server_config_validate(&t->model.config)!=PAPACC_RESULT_OK ||
(o->configuration_commit.initialized&&!root_recovery_candidate_matches(o)) || !t->provider_connection ||
       t->provider_result!=PST_RESULT_OK || !t->runtime_provider.initialized ||
       o->network.is_active || o->network.tcp_platform.initialized || o->io_loop.initialized ||
       o->io_loop.c2_owner || o->secure_io.initialized || o->acceptor.initialized ||
       papacc_server_security_source_validate(&t->source)!=PAPACC_RESULT_OK)
        return PAPACC_RESULT_INVALID_STATE;
    /* Source is the same real application model copied at acquire/prepare.
     * No external callback/reference can select a different policy here. */
    t->policy.configured_model=t->model.config;t->policy.invocation=o->invocation;
    t->policy.intent.profile=(PAPACC_SERVER_LISTENER_PROFILE)p->profile;
    t->policy.intent.listener_port=p->listener_port;
    t->policy.intent.admission=(PAPACC_ROOT_POLICY_ADMISSION)p->admission;
    t->policy.intent.enabled_count=p->enabled_count;t->policy.intent.rule_count=p->rule_count;
    for(i=0U;i<p->enabled_count;++i)t->policy.intent.enabled[i]=p->enabled[i];
    for(i=0U;i<p->rule_count;++i){t->policy.intent.rules[i].key=p->rules[i].key;
        t->policy.intent.rules[i].decision=(PAPACC_CAPABILITY_POLICY_DECISION)p->rules[i].action;
        t->policy.intent.rules[i].unsupported_conditions=p->rules[i].conditions;}
    t->policy.from_model=PAPACC_TRUE;t->policy.configured=PAPACC_TRUE;
    return papacc_server_launcher_configuration_policy_prepare(o);
}
PAPACC_RESULT papacc_server_launcher_configuration_policy_preview(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_ADMIN_POLICY_CONTEXT *context,
    const PAPACC_CAPABILITY_KEY *key,PAPACC_U32 *decision,PAPACC_SIZE *matched)
{
    PAPACC_ROOT_POLICY_REPORT report;
    if(decision==NULL||matched==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    *decision=PAPACC_ADMIN_POLICY_DENY;*matched=PAPACC_ADMIN_POLICY_RULES;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    if(papacc_server_launcher_configuration_policy_report(o,&report)!=PAPACC_RESULT_OK ||
       !report.prepared || !report.model_source)return PAPACC_RESULT_INVALID_STATE;
    /* A rule preview is not authentication, admission, current policy, grant,
     * normative commit, epoch, publication or a C8 operation authorization. */
    return papacc_server_policy_evaluate(&o->configuration_preparation.policy.configured_model,
        context,key,decision,matched);
}
PAPACC_RESULT papacc_server_launcher_configuration_policy_report(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ROOT_POLICY_REPORT *out)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
    if(out==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    memset(out,0,sizeof(*out));
    if(o!=&root || GetCurrentThreadId()!=o->owner_thread ||
       (papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE &&
        papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_RETAINED))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->invocation!=o->invocation || t->state==PAPACC_ROOT_PREPARATION_EMPTY ||
       t->state==PAPACC_ROOT_PREPARATION_ROLLED_BACK)return PAPACC_RESULT_INVALID_STATE;
    out->model_source=t->policy.from_model;
    out->invocation=t->policy.invocation;out->configured=t->policy.configured;
    out->retained=t->state==PAPACC_ROOT_PREPARATION_RETAINED?PAPACC_TRUE:PAPACC_FALSE;
    out->configured_enabled=t->policy.intent.enabled_count;out->rules=t->policy.intent.rule_count;
    out->prepared_enabled=t->policy.snapshot.count;
    out->profile=t->policy.intent.profile;out->listener_port=t->policy.intent.listener_port;
    out->admission=t->policy.intent.admission;
    out->prepared=t->policy.prepared && !out->retained && root_policy_model_valid(o) &&
        root_policy_inputs_valid(t)?PAPACC_TRUE:PAPACC_FALSE;
    /* Configured/prepared do NOT assert committed/current. No revision or epoch
     * is minted from invocation, source validity, pointer, hash or fixture. */
    out->committed=t->normative_committed;
    out->current=t->administrative_published&&!out->retained?PAPACC_TRUE:PAPACC_FALSE;
    if(out->committed)out->config_revision=o->configuration_commit.slots[o->configuration_commit.current].view.revision;
    if(out->current)out->c2_policy_epoch=t->current_policy_epoch;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_policy_decide(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_CAPABILITY_KEY *key,
    PAPACC_SERVER_LISTENER_PROFILE profile,PAPACC_U16 port,PAPACC_ROOT_POLICY_ADMISSION admission,
    PAPACC_CAPABILITY_POLICY_DECISION *decision)
{
    PAPACC_ROOT_POLICY_REPORT report;
    if(decision==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    *decision=PAPACC_CAPABILITY_POLICY_DENY;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    if(papacc_capability_key_valid(key)!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(papacc_server_launcher_configuration_policy_report(o,&report)!=PAPACC_RESULT_OK ||
       !report.prepared || profile!=report.profile || port!=report.listener_port ||
       admission!=report.admission)return PAPACC_RESULT_INVALID_STATE;
    /* No configured input or caller reference can bypass absence of normative
     * commit/current policy/Session/grants/epoch. No callback is installed. */
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_prepare_source(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_SIZE additional,PAPACC_SIZE credit)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
    PAPACC_SECURITY_CONFIGURATION_REF reference=PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;
    PAPACC_SIZE base,combined,count,launcher_fixed,extra;
    PAPACC_CAPABILITY_KEY c2_key={3U,1U};
    PAPACC_RESULT result;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    /* No mutation of a running, selected, retained or previously prepared graph.
     * Normal launch/bootstrap paths are neither invoked nor repaired here. */
if(t->state!=PAPACC_ROOT_PREPARATION_EMPTY ||
       (o->configuration_commit.initialized&&!root_recovery_candidate_matches(o)) ||
       o->network.is_active || o->network.tcp_platform.initialized ||
       o->network.listener_set.count || o->acceptor.initialized ||
       o->acceptor.connection_manager.initialized || o->io_loop.initialized ||
       o->io_loop.c2_owner || o->secure_io.initialized || o->secure_io.dispatch_active ||
       o->secure_io.scheduler.initialized || o->pst_runtime_instance ||
       o->pst_system_trust || o->security_composition.runtime ||
       o->security_composition.local_credentials || o->security_composition.peer_trust ||
       o->security_composition.log_adapter || o->resolved_security.valid ||
       o->resolved_security.release || o->resolved_security.release_context ||
       o->resolved_security.principal_resolve || o->resolved_security.authorize ||
       o->connection_storage || o->context_storage || o->session_storage ||
       o->channel_storage || o->association_storage || o->processor_storage ||
       o->secure_candidates || o->secure_members || o->secure_events ||
       o->listener_sources || o->listener_tokens)return PAPACC_RESULT_INVALID_STATE;
    result=papacc_server_config_validate(&o->config);
    if(result!=PAPACC_RESULT_OK)return result;
    if(o->config.listener_profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL)
        return PAPACC_RESULT_NOT_SUPPORTED;
    count=o->config.bind_selection.interface_persistent_id_count;
    if(count>PAPACC_CONFIG_IDS)return PAPACC_RESULT_LIMIT_EXCEEDED;
    /* Layout includes the root (and this source transaction) exactly once,
     * C2/manager/bridges/stages and all fixed generic arrays. Additional credit
     * MUST price caller stack, scratch and any other simultaneous managed users.
     * Listener/ID storage charged at acquire is added without double counting
     * the fixed generic arrays already present in both reservation formulas. */
    /* Replace the layout's single assumed bind ID by actual acquire charges.
     * Keep its two C3/listener entries and one external-source schedule reserve.
     * Actual acquire charges use (capacity+streams), without that extra entry. */
    base=PAPACC_C8_KNOWN_MANAGED_BYTES-sizeof(PAPACC_NETWORK_INTERFACE_PERSISTENT_ID);
    launcher_fixed=sizeof(*o)+PAPACC_PENDING_STACK_BYTES+PAPACC_C8_BASE_ARRAY_BYTES+PAPACC_C8_SECURE_ARRAY_BYTES+
        (PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY+PAPACC_SERVER_NETWORK_STREAM_CAPACITY)*
        (sizeof(*o->secure_members)+sizeof(*o->secure_events));
    if(o->reserved_bytes<launcher_fixed)return PAPACC_RESULT_INVALID_STATE;
    extra=o->reserved_bytes-launcher_fixed;
    if(extra>PAPACC_LAUNCHER_MANAGED_LIMIT-base)return PAPACC_RESULT_LIMIT_EXCEEDED;
    base+=extra;
    if(base>PAPACC_LAUNCHER_MANAGED_LIMIT || additional>PAPACC_LAUNCHER_MANAGED_LIMIT-base ||
       credit==0U || credit>PAPACC_LAUNCHER_MANAGED_LIMIT-base-additional)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    combined=base+additional+credit;
    result=papacc_security_configuration_ref_set(&reference,o->config.security_configuration_ref,
        o->config.security_configuration_ref_length);
    if(result!=PAPACC_RESULT_OK)return result;
    t->combined_reserved=combined;t->material_credit=credit;t->invocation=o->invocation;
    t->state=PAPACC_ROOT_PREPARATION_PARTIAL;t->model.config=o->config;
    if(count)memcpy(t->model.ids,o->config.bind_selection.interface_persistent_ids,
        count*sizeof(t->model.ids[0]));
    t->model.config.bind_selection.interface_persistent_ids=count?t->model.ids:NULL;
    /* Real registry metadata; empty support/enabled sets deliberately confer no
     * backend capability, policy ALLOW, snapshot or grant. */
    result=papacc_capability_registry_init(&t->registry,t->descriptors,PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
    if(result==PAPACC_RESULT_OK)result=papacc_builtin_capabilities_register(&t->registry);
    /* Existing accepted C2 allocation, metadata only: never backend support,
     * enablement, a client snapshot, grant or policy permission. */
    if(result==PAPACC_RESULT_OK)result=papacc_capability_registry_register(&t->registry,&c2_key,"MODE_C_OPERATION");
    if(result==PAPACC_RESULT_OK)result=papacc_capability_set_init(&t->supported,t->supported_keys,PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
    if(result==PAPACC_RESULT_OK)result=papacc_capability_set_init(&t->enabled,t->enabled_keys,PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
    t->policy_inputs.registry=&t->registry;
    t->policy_inputs.server_supported=&t->supported;
    t->policy_inputs.server_enabled=&t->enabled;
    /* No current-policy authority can be inferred from Secure authentication
     * callbacks or a bootstrapped provider. Absent callback stays fail-closed. */
    t->policy_inputs.principal_policy=NULL;
    t->policy_inputs.principal_policy_context=NULL;
    if(result==PAPACC_RESULT_OK)result=papacc_server_security_source_prepare(&t->source,
        &reference,&o->logger,combined-sizeof(t->source)-credit,credit);
    t->preparation_result=result;
    if(t->source.state==PAPACC_SECURITY_SOURCE_RETAINED)t->state=PAPACC_ROOT_PREPARATION_RETAINED;
    /* Source preparation is not ready credentials/trust/runtime. No provider
     * callback, configuration record, epoch or source publish is attempted. */
    return result==PAPACC_RESULT_OK?PAPACC_RESULT_NOT_SUPPORTED:result;
}
PAPACC_RESULT papacc_server_launcher_configuration_preparation_report(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_CONFIG_PARTICIPANT_REPORT *out)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
    if(out==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    memset(out,0,sizeof(*out));out->missing=1023U;
    out->model_result=out->listener_result=out->security_result=PAPACC_RESULT_INVALID_STATE;
    /* By-value query remains safe on the retained process root. Mutating APIs
     * still require ACTIVE and cannot resume or reclaim retained dependencies. */
    if(o!=&root || GetCurrentThreadId()!=o->owner_thread ||
       (papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE &&
        papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_RETAINED))
        return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL && t->state!=PAPACC_ROOT_PREPARATION_RETAINED)
        return PAPACC_RESULT_INVALID_STATE;
    if(t->invocation!=o->invocation)return PAPACC_RESULT_INVALID_STATE;
    out->model_result=papacc_server_config_validate(&t->model.config);
    if(out->model_result==PAPACC_RESULT_OK)out->validated|=PAPACC_CFG_MODEL;
    if(t->registry.initialized && t->registry.storage==t->descriptors &&
       t->registry.count<=t->registry.capacity && t->registry.capacity==PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS)
        out->validated|=PAPACC_CFG_REGISTRY;
    out->security_result=papacc_server_security_source_validate(&t->source);
    if(out->security_result==PAPACC_RESULT_OK){
        if(t->source.configuration.principal_resolve)out->validated|=PAPACC_CFG_PRINCIPAL_RESOLVER;
        if(t->source.configuration.authorize)out->validated|=PAPACC_CFG_AUTHORIZATION_PROVIDER;
    }
    if(out->security_result==PAPACC_RESULT_OK && t->runtime_attempted &&
       t->runtime_result==PAPACC_RESULT_OK &&
       o->security_composition.state==PAPACC_SECURITY_COMPOSITION_READY &&
       o->security_composition.runtime && o->security_composition.local_credentials &&
       o->security_composition.peer_trust && o->security_composition.log_adapter &&
       t->runtime_provider.available && t->runtime_provider.initialized &&
       t->provider_attempted && t->provider_result==PST_RESULT_OK &&
       t->provider_connection!=NULL)
        out->validated|=PAPACC_CFG_SECURITY_LEASE;
    /* Prepared runtime/credential/trust handles still prove no authentication,
     * current policy, Session/grant, epoch or normative participant commit. */
    out->missing=1023U&~out->validated;
    return PAPACC_RESULT_NOT_SUPPORTED;
}
static PAPACC_RESULT configuration_preparation_cleanup(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_BOOL post_commit)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result=PAPACC_RESULT_OK;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL || t->invocation!=o->invocation ||
       t->source.close_uncertain || o->configuration_commit.initialized ||
       (t->normative_committed && !post_commit))
        return PAPACC_RESULT_INVALID_STATE;
    /* No child runtime lease is exported by this offline entry. Never apply
     * its zero-child cleanup to a root subsequently used by IO/C2 or transport. */
    if(t->runtime_attempted && (o->io_loop.initialized || o->io_loop.c2_owner ||
       o->secure_io.initialized || o->secure_io.scheduler.initialized ||
       o->acceptor.initialized || o->acceptor.connection_manager.initialized ||
       o->network.is_active || o->network.tcp_platform.initialized))
        return PAPACC_RESULT_INVALID_STATE;
    result=configuration_dormant_unapply(o);
    if(result!=PAPACC_RESULT_OK || t->source.borrowers){
        (void)papacc_server_launcher_configuration_preparation_retain(o);
        return PAPACC_RESULT_INVALID_STATE;
    }
    result=configuration_site_cleanup(o);
    if(result!=PAPACC_RESULT_OK){(void)papacc_server_launcher_configuration_preparation_retain(o);return result;}
    if(t->provider_connection!=NULL){
        if(t->provider_release_attempted)return PAPACC_RESULT_INVALID_STATE;
        if(pst_release_info_init(&t->provider_release,(pst_u32)sizeof(t->provider_release))!=PST_RESULT_OK){
            (void)papacc_server_launcher_configuration_preparation_retain(o);
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        t->provider_release_attempted=PAPACC_TRUE;
        t->provider_release_result=pst_connection_try_release(t->provider_connection,&t->provider_release);
        if(t->provider_release_result!=PST_RESULT_OK ||
           t->provider_release.disposition!=PST_RELEASE_DISPOSITION_DESTROYED ||
           t->provider_release.result!=PST_RESULT_OK){
            (void)papacc_server_launcher_configuration_preparation_retain(o);
            return PAPACC_RESULT_INVALID_STATE;
        }
        t->provider_connection=NULL;
    }
    result=configuration_participants_rollback(o);
    if(result!=PAPACC_RESULT_OK){
        (void)papacc_server_launcher_configuration_preparation_retain(o);
        return result;
    }
    if(t->runtime_attempted && o->security_composition.state==PAPACC_SECURITY_COMPOSITION_READY)
        papacc_security_composition_release(&o->security_composition);
    /* A dormant source lease has been returned after factual zero-child
     * quiescence; a committed record is never described as rolled back. */
    if(t->source.state==PAPACC_SECURITY_SOURCE_PREPARED)
        result=papacc_server_security_source_rollback(&t->source);
    else if(t->source.state==PAPACC_SECURITY_SOURCE_REJECTED ||
            t->source.state==PAPACC_SECURITY_SOURCE_COMMITTED ||
            t->source.state==PAPACC_SECURITY_SOURCE_PUBLISHED)
        result=papacc_server_security_source_finish(&t->source,PAPACC_TRUE);
    else if(t->source.state!=PAPACC_SECURITY_SOURCE_EMPTY)result=PAPACC_RESULT_INVALID_STATE;
    if(result!=PAPACC_RESULT_OK){t->state=PAPACC_ROOT_PREPARATION_RETAINED;return result;}
    papacc_capability_set_release(&t->policy.snapshot);
    t->policy.prepared=PAPACC_FALSE;
    papacc_capability_set_release(&t->enabled);papacc_capability_set_release(&t->supported);
    papacc_capability_registry_release(&t->registry);
    t->state=post_commit?PAPACC_ROOT_PREPARATION_CLOSED:PAPACC_ROOT_PREPARATION_ROLLED_BACK;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_preparation_rollback(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{return configuration_preparation_cleanup(o,PAPACC_FALSE);}
PAPACC_RESULT papacc_server_launcher_configuration_prepare_runtime(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
    const PAPACC_SECURITY_COMPOSITION_INPUTS *inputs;
    PAPACC_SIZE count,credit,bytes;PAPACC_RESULT result;
    PST_RUNTIME_INFO info;PST_PROVIDER_INFO provider;pst_size index;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL || t->invocation!=o->invocation ||
t->runtime_attempted || (o->configuration_commit.initialized&&!root_recovery_candidate_matches(o)) ||
       o->security_composition.state!=PAPACC_SECURITY_COMPOSITION_UNINITIALIZED ||
       o->io_loop.initialized || o->io_loop.c2_owner || o->secure_io.initialized ||
       o->secure_io.scheduler.initialized || o->acceptor.initialized ||
       o->acceptor.connection_manager.initialized || o->network.is_active ||
       o->network.tcp_platform.initialized || t->source.borrowers ||
       t->source.state!=PAPACC_SECURITY_SOURCE_PREPARED)
        return PAPACC_RESULT_INVALID_STATE;
    result=papacc_server_security_source_validate(&t->source);
    if(result!=PAPACC_RESULT_OK)return result;
    inputs=&t->source.configuration.composition_inputs;
    /* Price caller adapter + temporary descriptor arrays and a separate credit
     * for PST copies of bounded source bytes/descriptors before bootstrap.
     * Opaque PST handle/provider/allocator overhead is UNKNOWN, not zero;
     * this diagnostic preparation cannot authorize full ED11 admission. */
    credit=sizeof(PAPACC_PST_LOG_ADAPTER);
    if(inputs->local_certificate_count>PAPACC_LAUNCHER_MANAGED_LIMIT/(2U*sizeof(PST_DER_ITEM)) ||
       inputs->peer_trust_anchor_count>PAPACC_LAUNCHER_MANAGED_LIMIT/(2U*sizeof(PST_DER_ITEM)))
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    bytes=2U*(inputs->local_certificate_count+inputs->peer_trust_anchor_count)*sizeof(PST_DER_ITEM);
    if(bytes>PAPACC_LAUNCHER_MANAGED_LIMIT-credit)return PAPACC_RESULT_LIMIT_EXCEEDED;
    credit+=bytes;
    if(inputs->local_private_key_pkcs8_der_size>PAPACC_LAUNCHER_MANAGED_LIMIT-credit)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    credit+=inputs->local_private_key_pkcs8_der_size;
    for(count=0U;count<inputs->local_certificate_count;++count){
        bytes=inputs->local_certificate_chain[count].size;
        if(bytes>PAPACC_LAUNCHER_MANAGED_LIMIT-credit)return PAPACC_RESULT_LIMIT_EXCEEDED;
        credit+=bytes;
    }
    for(count=0U;count<inputs->peer_trust_anchor_count;++count){
        bytes=inputs->peer_trust_anchors[count].size;
        if(bytes>PAPACC_LAUNCHER_MANAGED_LIMIT-credit)return PAPACC_RESULT_LIMIT_EXCEEDED;
        credit+=bytes;
    }
    if(t->combined_reserved>PAPACC_LAUNCHER_MANAGED_LIMIT ||
       credit>PAPACC_LAUNCHER_MANAGED_LIMIT-t->combined_reserved)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    t->combined_reserved+=credit;t->runtime_credit=credit;
    t->source.other_reserved+=credit;t->runtime_attempted=PAPACC_TRUE;
    result=papacc_security_composition_init(&o->security_composition,inputs);
    t->runtime_result=result;
    if(result!=PAPACC_RESULT_OK)return result;
    memset(&info,0,sizeof(info));info.struct_size=(pst_u32)sizeof(info);info.api_version=PST_API_VERSION;
    if(pst_runtime_get_info(o->security_composition.runtime,&info)!=PST_RESULT_OK){
        t->runtime_result=PAPACC_RESULT_INTERNAL_ERROR;return t->runtime_result;
    }
    for(index=0U;index<info.provider_count;++index){
        memset(&provider,0,sizeof(provider));provider.struct_size=(pst_u32)sizeof(provider);
        provider.api_version=PST_API_VERSION;
        if(pst_runtime_get_provider_info(o->security_composition.runtime,index,&provider)!=PST_RESULT_OK){
            t->runtime_result=PAPACC_RESULT_INTERNAL_ERROR;return t->runtime_result;
        }
        if(memcmp(provider.provider_id,o->security_composition.provider_id,
            sizeof(provider.provider_id))==0){
            t->runtime_provider=provider;
            return PAPACC_RESULT_OK;
        }
    }
    t->runtime_result=PAPACC_RESULT_NOT_SUPPORTED;return t->runtime_result;
}
PAPACC_RESULT papacc_server_launcher_configuration_prepare_provider(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;PAPACC_SIZE credit;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL || t->invocation!=o->invocation ||
       !t->runtime_attempted || t->runtime_result!=PAPACC_RESULT_OK ||
       t->provider_attempted || t->provider_connection ||
       o->security_composition.state!=PAPACC_SECURITY_COMPOSITION_READY ||
(o->configuration_commit.initialized&&!root_recovery_candidate_matches(o)) || o->io_loop.initialized ||
       o->io_loop.c2_owner || o->secure_io.initialized || o->secure_io.scheduler.initialized ||
       o->acceptor.initialized || o->network.is_active || o->network.tcp_platform.initialized ||
       t->source.state!=PAPACC_SECURITY_SOURCE_PREPARED || t->source.borrowers)
        return PAPACC_RESULT_INVALID_STATE;
    result=papacc_server_security_source_validate(&t->source);
    if(result!=PAPACC_RESULT_OK)return result;
    /* Public snapshot/list/ALPN/provider-string copy allowance is separate from
     * the embedded config. Opaque core/provider TLS allocations remain UNKNOWN. */
    credit=sizeof(PST_CONNECTION_CONFIG)+sizeof(PST_ALPN_PROTOCOL)+
        PAPACC_SECURITY_ALPN_PAPACC_1_SIZE+PAPACC_SECURITY_PROVIDER_ID_CAPACITY;
    if(t->combined_reserved>PAPACC_LAUNCHER_MANAGED_LIMIT ||
       credit>PAPACC_LAUNCHER_MANAGED_LIMIT-t->combined_reserved)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    result=papacc_security_composition_build_secure_principal_config(&o->security_composition,
        &t->provider_config);
    if(result!=PAPACC_RESULT_OK)return result;
    t->combined_reserved+=credit;t->runtime_credit+=credit;t->source.other_reserved+=credit;
    t->provider_attempted=PAPACC_TRUE;
    memset(&t->provider_diagnostic,0,sizeof(t->provider_diagnostic));
    t->provider_diagnostic.struct_size=(pst_u32)sizeof(t->provider_diagnostic);
    t->provider_diagnostic.api_version=PST_API_VERSION;
    t->provider_result=pst_connection_create_ex(o->security_composition.runtime,
        &t->provider_config,&t->provider_connection,&t->provider_diagnostic);
    if(t->provider_result!=PST_RESULT_OK){
        if(t->provider_connection!=NULL)
            (void)papacc_server_launcher_configuration_preparation_retain(o);
        return t->provider_result==PST_RESULT_UNSUPPORTED?PAPACC_RESULT_NOT_SUPPORTED:
            PAPACC_RESULT_INTERNAL_ERROR;
    }
    if(t->provider_connection==NULL){
        (void)papacc_server_launcher_configuration_preparation_retain(o);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    memset(&t->runtime_provider,0,sizeof(t->runtime_provider));
    t->runtime_provider.struct_size=(pst_u32)sizeof(t->runtime_provider);
    t->runtime_provider.api_version=PST_API_VERSION;
    t->provider_result=pst_connection_get_provider_info(t->provider_connection,&t->runtime_provider);
    if(t->provider_result!=PST_RESULT_OK || !t->runtime_provider.available ||
       !t->runtime_provider.initialized)return PAPACC_RESULT_NOT_SUPPORTED;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_preparation_retain(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
    if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->state!=PAPACC_ROOT_PREPARATION_PARTIAL || t->invocation!=o->invocation ||
       (t->source.state!=PAPACC_SECURITY_SOURCE_PREPARED &&
        t->source.state!=PAPACC_SECURITY_SOURCE_COMMITTED &&
        t->source.state!=PAPACC_SECURITY_SOURCE_PUBLISHED))return PAPACC_RESULT_INVALID_STATE;
    if(t->dormant_applied)(void)papacc_server_c2_configuration_retain(&configuration_c2_owner);
    if(o->configuration_commit.initialized){
        EnterCriticalSection(&o->configuration_commit.lock);
        t->administrative_published=PAPACC_FALSE;
        LeaveCriticalSection(&o->configuration_commit.lock);
    }
    result=papacc_server_security_source_retain(&t->source);
    if(result==PAPACC_RESULT_OK)t->state=PAPACC_ROOT_PREPARATION_RETAINED;
    return result;
}
#endif

#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
#include "server_configuration_participants_win32.inc"
#include "server_configuration_root_transaction_win32.inc"
#include "server_configuration_attachment_win32.inc"
#endif

PAPACC_RESULT papacc_server_launcher_bootstrap_cli_preflight(const PAPACC_SERVER_CONFIG *config)
{
 PAPACC_RESULT result=papacc_server_config_validate(config);
 if(result!=PAPACC_RESULT_OK)return result;
 /* CLI is an intent source, not the actual committed/current root. No policy,
  * access mode, epoch, lease or grant may be inferred from textual validation.
  * Activation is intentionally unavailable until a coherent bootstrap owner
  * and the separate controlled-admission gate are integrated. */
 return PAPACC_RESULT_NOT_SUPPORTED;
}
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
#if defined(PAPACC_C8_SUPERVISED_DIAGNOSTIC)
#include "server_c8_supervision_win32.h"
static int root_sv_exception(const PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,PAPACC_U64);
#endif
#include "server_c8_resource_admission_win32.inc"
#include "server_admission_preflight_win32.inc"
static PAPACC_RESULT root_resource_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
#include "server_listener_composition_win32.inc"
#include "server_c8_resource_root_win32.inc"
#if defined(PAPACC_C8_SUPERVISED_DIAGNOSTIC)
#include "server_c8_supervision_root_win32.inc"
#endif
#include "server_c2_root_binding_win32.inc"
#if defined(PAPACC_C8_SUPERVISED_HOST)
#include "server_c8_supervised_host_win32.inc"
#endif
#endif

#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
PAPACC_BOOL papacc_server_launcher_owns_listener(const PAPACC_TCP_SOCKET_WIN32 *listener)
{
 PAPACC_SIZE i;
 if(!listener||!root.network.listener_storage)return PAPACC_FALSE;
 if(root.network.listener_storage_capacity>PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY)return PAPACC_TRUE;
 for(i=0;i<root.network.listener_storage_capacity;++i)
  if(listener==&root.network.listener_storage[i].socket)return PAPACC_TRUE;
 return PAPACC_FALSE;
}
#endif

/* Same root/current authority, not a second resolver/provider. */
static int site_alias(const PAPACC_SITE_TLS_CATALOG *out,const void *object,PAPACC_SIZE size)
{
 ULONG_PTR a=(ULONG_PTR)out,b=(ULONG_PTR)object;
 return a<=b?b-a<sizeof(*out):a-b<size;
}
PAPACC_RESULT papacc_server_launcher_site_profile(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,
 const PAPACC_ROOT_CURRENT_PIN *pin,PAPACC_U32 ref,PAPACC_U64 revision,PAPACC_SITE_TLS_CATALOG *out)
{
 FILETIME ft;ULARGE_INTEGER time;
 if(!out||(site_alias(out,&root,sizeof(root))||site_alias(out,&configuration_c2_owner,sizeof(configuration_c2_owner))||site_alias(out,&configuration_c2_participant,sizeof(configuration_c2_participant)))||(pin&&site_alias(out,pin,sizeof(*pin))))return PAPACC_RESULT_INVALID_ARGUMENT;memset(out,0,sizeof(*out));
 if(!preparation_owned(o)||!pin||!papacc_server_launcher_configuration_current(o,pin)||
 !o->configuration_preparation.normative_committed||!o->configuration_preparation.administrative_published||
 !pin->epoch||pin->epoch!=o->configuration_preparation.current_policy_epoch||
 memcmp(&pin->view.config.site_tls,&o->config.site_tls,sizeof(o->config.site_tls)))
 return PAPACC_RESULT_INVALID_STATE;
 GetSystemTimeAsFileTime(&ft);time.LowPart=ft.dwLowDateTime;time.HighPart=ft.dwHighDateTime;
 return papacc_site_tls_catalog_select(&pin->view.config.site_tls,ref,revision,time.QuadPart/10000U,out);
}
PAPACC_RESULT papacc_server_launcher_c2_site_template(const PAPACC_C2_SESSION *session,
 PAPACC_U32 ref,const PST_CONNECTION_CONFIG *config,PAPACC_SITE_TLS_CATALOG *out)
{
 PAPACC_RESULT r;
 if(!out||(site_alias(out,&root,sizeof(root))||site_alias(out,&configuration_c2_owner,sizeof(configuration_c2_owner))||site_alias(out,&configuration_c2_participant,sizeof(configuration_c2_participant))))return PAPACC_RESULT_INVALID_ARGUMENT;
 if((session&&site_alias(out,session,sizeof(*session)))||(config&&site_alias(out,config,sizeof(*config))))return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 if(!session||!config||!papacc_server_launcher_c2_session_current(session))return PAPACC_RESULT_INVALID_STATE;
 r=papacc_server_launcher_site_profile(&root,&root.listener_composition.pin,ref,
 root.config.site_tls.revision,out);
 if(r!=PAPACC_RESULT_OK)return r;
 /* No arbitrary or borrowed trust can be promoted to approved SYSTEM trust.
  * Operational preparation must materialize this root-owned lease before use. */
 if(!root.pst_system_trust||root.pst_trust_source.kind!=PST_TRUST_SOURCE_SYSTEM||
 root.pst_trust_source.anchors||root.pst_trust_source.anchor_count||config->peer_authentication.trust!=root.pst_system_trust||
 config->role!=PST_CONNECTION_ROLE_CLIENT||config->peer_authentication.certificate_mode!=PST_PEER_CERTIFICATE_REQUIRED||
 config->local_identity.credentials||config->tls.minimum_version!=PST_TLS_VERSION_1_3||
 config->tls.maximum_version!=PST_TLS_VERSION_1_3||
 config->tls.resumption!=PST_FEATURE_DISABLED||config->tls.early_data!=PST_FEATURE_DISABLED||
 config->tls.require_graceful_shutdown!=PST_FEATURE_REQUIRED||config->server_name_indication_mode!=PST_SNI_MODE_EXPLICIT||
 !config->peer_authentication.expected_peer_name||!config->peer_authentication.expected_peer_name_size||
 config->peer_authentication.expected_peer_name_size>253U||!config->server_name_indication||
 config->server_name_indication_size!=config->peer_authentication.expected_peer_name_size||
 memcmp(config->server_name_indication,config->peer_authentication.expected_peer_name,config->server_name_indication_size)||
 config->alpn.mode!=PST_FEATURE_REQUIRED||!config->alpn.protocols||!config->alpn.protocol_count||config->alpn.protocol_count>8U){
 memset(out,0,sizeof(*out));return PAPACC_RESULT_INVALID_STATE;}
 {
  unsigned i;PAPACC_U8 vector[256];PAPACC_SIZE n=0;
  for(i=0;i<config->alpn.protocol_count;++i){PAPACC_SIZE len=config->alpn.protocols[i].size;
   if(!len||len>255U||n>=sizeof(vector)||len>sizeof(vector)-n-1U||!config->alpn.protocols[i].data){memset(out,0,sizeof(*out));return PAPACC_RESULT_INVALID_STATE;}
   vector[n++]=(PAPACC_U8)len;memcpy(vector+n,config->alpn.protocols[i].data,len);n+=len;
  }
  if(papacc_site_tls_alpn_allowed(out,vector,n)!=PAPACC_RESULT_OK){memset(out,0,sizeof(*out));return PAPACC_RESULT_INVALID_STATE;}
 }
 /* Existing application/ED11 admission boundary stays closed in this slice. */
 r=papacc_application_require_operational_admission();
 if(r!=PAPACC_RESULT_OK)memset(out,0,sizeof(*out));
 return r;
}

#include "server_c2_dispatch_binding_win32.inc"

