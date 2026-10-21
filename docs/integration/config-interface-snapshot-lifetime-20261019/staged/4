/* Same-root dormant participant preparation, never activation or an issuer. */
static int participant_candidate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
    if(!preparation_owned(o))return 0;
    t=&o->configuration_preparation;
    return t->state==PAPACC_ROOT_PREPARATION_PARTIAL && t->invocation==o->invocation &&
        t->policy.prepared && t->policy.from_model && root_policy_model_valid(o) &&
root_policy_inputs_valid(t) && (!o->configuration_commit.initialized||root_recovery_candidate_matches(o)) &&
        !o->network.is_active && !o->network.tcp_platform.initialized &&
        !o->network.listener_set.count && !o->io_loop.initialized && !o->io_loop.c2_owner &&
        !o->secure_io.initialized && !o->secure_io.scheduler.initialized &&
        !o->acceptor.initialized && !o->acceptor.connection_manager.initialized &&
        t->provider_connection && t->provider_result==PST_RESULT_OK &&
        papacc_server_security_source_validate(&t->source)==PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_prepare_listener(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *catalog)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
    PAPACC_ROOT_TRANSACTION_PARTICIPANTS next;
    PAPACC_BIND_SELECTION selection=PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT owned=PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_SIZE required;PAPACC_RESULT result;
    if(!participant_candidate(o)||papacc_server_console_win32_stop_requested()==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->participants.listener_prepared || catalog==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    /* Embedded copy is priced in sizeof(root); temporary next plus bounded
     * helper stack are additional here, never a historical headroom grant. */
    if(t->combined_reserved>PAPACC_LAUNCHER_MANAGED_LIMIT||
       sizeof(next)+4096U>PAPACC_LAUNCHER_MANAGED_LIMIT-t->combined_reserved)return PAPACC_RESULT_LIMIT_EXCEEDED;
    memset(&next,0,sizeof(next));
    result=papacc_interface_snapshot_copy_prepare(&next.interface_snapshot,catalog,o->invocation,t->model.revision);
    if(result==PAPACC_RESULT_OK)result=papacc_interface_snapshot_copy_view(&next.interface_snapshot,o->invocation,t->model.revision,&owned);
    if(result!=PAPACC_RESULT_OK)return result;
    next.listener.bind_selection=t->model.config.bind_selection;
    next.listener.port=t->model.config.control_port;
    /* Explicit Secure only. Legacy model validity never activates Legacy. */
    if(t->model.config.listener_profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL)
        return PAPACC_RESULT_NOT_SUPPORTED;
    next.listener.transport_profile=PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL;
    result=papacc_security_configuration_ref_set(&next.listener.security_configuration_ref,
        t->model.config.security_configuration_ref,t->model.config.security_configuration_ref_length);
    if(result==PAPACC_RESULT_OK)result=papacc_server_listener_configuration_validate(&next.listener);
    if(result==PAPACC_RESULT_OK)result=papacc_persistent_bind_selection_resolve(
        &next.listener.bind_selection,&owned,next.runtime_ids,PAPACC_CONFIG_IDS,&selection,&required);
    if(result==PAPACC_RESULT_OK)result=papacc_bind_targets_resolve(&selection,&owned,
        next.targets,PAPACC_ROOT_BIND_TARGETS,&next.target_count,&required);
    if(result!=PAPACC_RESULT_OK)return result;
    if(!next.target_count)return PAPACC_RESULT_INVALID_STATE;
    next.invocation=o->invocation;next.listener_prepared=PAPACC_TRUE;
    if(!participant_candidate(o)||papacc_server_console_win32_stop_requested()==PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    /* Only completed immutable value storage escapes, owned by this existing
     * preparation invocation. No source or presentation pointer is retained.
     * Preparation revision zero remains pending, not an invented CURRENT. */
    t->participants=next;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_prepare_capabilities(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;const PAPACC_CAPABILITY_DESCRIPTOR *descriptor;
    const PAPACC_CAPABILITY_KEY tls={2U,1U},mode_c={3U,1U};
    pst_u32 required=PST_CAP_TLS_1_3|PST_CAP_ROLE_CLIENT|PST_CAP_PEER_CERT_AUTH|
        PST_CAP_PEER_NAME_VERIFY|PST_CAP_ALPN_CLIENT|PST_CAP_NONBLOCKING|
        PST_CAP_SNI_CONTROL|PST_CAP_GRACEFUL_SHUTDOWN;
    PAPACC_SIZE i;PAPACC_RESULT result;
    if(!participant_candidate(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(!t->participants.listener_prepared || t->participants.capabilities_prepared ||
        t->supported.count || t->enabled.count)return PAPACC_RESULT_INVALID_STATE;
    /* Backend mechanism support is not remote TLS verification or admission. */
    if(!t->runtime_provider.available || !t->runtime_provider.initialized ||
        (t->runtime_provider.client_capabilities&required)!=required)return PAPACC_RESULT_NOT_SUPPORTED;
    for(i=0U;i<t->model.config.policy.enabled_count;++i){
        const PAPACC_CAPABILITY_KEY *key=&t->model.config.policy.enabled[i];
        if((!papacc_capability_key_equal(key,&tls) && !papacc_capability_key_equal(key,&mode_c)) ||
            papacc_capability_registry_find(&t->registry,key,&descriptor)!=PAPACC_RESULT_OK)
            return PAPACC_RESULT_NOT_SUPPORTED; /* Egress participant absent, no inference. */
    }
    if(papacc_capability_set_contains(&t->policy.snapshot,&mode_c) &&
       !papacc_capability_set_contains(&t->policy.snapshot,&tls))return PAPACC_RESULT_INVALID_STATE;
    result=papacc_capability_set_insert(&t->supported,&tls);
    if(result==PAPACC_RESULT_OK)result=papacc_capability_set_insert(&t->supported,&mode_c);
    for(i=0U;result==PAPACC_RESULT_OK && i<t->model.config.policy.enabled_count;++i)
        result=papacc_capability_set_insert(&t->enabled,&t->model.config.policy.enabled[i]);
    if(result!=PAPACC_RESULT_OK){
        papacc_capability_set_release(&t->supported);papacc_capability_set_release(&t->enabled);
        (void)papacc_capability_set_init(&t->supported,t->supported_keys,PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
        (void)papacc_capability_set_init(&t->enabled,t->enabled_keys,PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
        return result;
    }
    t->participants.capabilities_prepared=PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_prepare_c2(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
    if(!participant_candidate(o))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(!t->participants.capabilities_prepared || t->participants.c2_prepared ||
        configuration_c2.initialized)return PAPACC_RESULT_INVALID_STATE;
    /* Disabled capabilities are a valid dormant administrative configuration;
     * this creates neither a Session nor any positive operational grant. */
    if(t->combined_reserved<sizeof(configuration_c2_participant))return PAPACC_RESULT_LIMIT_EXCEEDED;
    result=papacc_server_c2_participant_prepare(&configuration_c2_participant,o,o->invocation,
        &t->model.config,&t->policy_inputs,t->combined_reserved-sizeof(configuration_c2_participant));
    if(result!=PAPACC_RESULT_OK)return result;
    /* Actual zero-Session manager. No fake negotiation, incarnation, principal,
     * operation, generation, positive epoch, token, grant or owner install. */
    t->participants.c2_prepared=PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_transaction_report(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ROOT_TRANSACTION_REPORT *out)
{
    PAPACC_ROOT_CONFIGURATION_PREPARATION *t;unsigned i;
    if(out==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    memset(out,0,sizeof(*out));
    if(o!=&root || GetCurrentThreadId()!=o->owner_thread ||
       (papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE &&
        papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_RETAINED))return PAPACC_RESULT_INVALID_STATE;
    t=&o->configuration_preparation;
    if(t->invocation!=o->invocation)return PAPACC_RESULT_INVALID_STATE;
    out->invocation=t->participants.invocation;
    out->retained=t->state==PAPACC_ROOT_PREPARATION_RETAINED?PAPACC_TRUE:PAPACC_FALSE;
    out->listener_prepared=t->participants.listener_prepared;
    out->capabilities_prepared=t->participants.capabilities_prepared;
    out->c2_prepared=t->participants.c2_prepared;
    out->c2_installed=configuration_c2_owner.initialized;
    out->targets=t->participants.target_count;out->supported=t->supported.count;out->enabled=t->enabled.count;
    if(t->participants.c2_prepared && configuration_c2.initialized){
        for(i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i)if(configuration_c2.sessions[i].session)++out->c2_sessions;
        for(i=0U;i<PAPACC_C2_OPERATION_CAPACITY;++i)
            if(configuration_c2.operations[i].state!=PAPACC_C2_OP_EMPTY)++out->c2_operations;
    }
    out->apply_confirmed=t->dormant_applied&&!out->retained;
    out->committed=t->normative_committed;
    out->current=t->administrative_published&&!out->retained;
    /* Administrative CURRENT never means active IO/Session eligibility. */
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_configuration_transaction_commit(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    return configuration_root_commit(o);
}
static PAPACC_RESULT configuration_participants_rollback(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
    PAPACC_ROOT_TRANSACTION_PARTICIPANTS *p=&o->configuration_preparation.participants;
    unsigned i;PAPACC_RESULT result;
    if(p->c2_prepared){
        if(configuration_c2_owner.configuration.state!=PAPACC_C2_CONFIG_EMPTY &&
           configuration_c2_owner.configuration.state!=PAPACC_C2_CONFIG_CLOSED)
            return PAPACC_RESULT_INVALID_STATE;
        if(!configuration_c2.initialized || configuration_c2.context!=&configuration_c2_participant ||
            configuration_c2.owner!=o->owner_thread)return PAPACC_RESULT_INVALID_STATE;
        for(i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i)
            if(configuration_c2.sessions[i].session)return PAPACC_RESULT_INVALID_STATE;
        for(i=0U;i<PAPACC_C2_OPERATION_CAPACITY;++i)
            if(configuration_c2.operations[i].state!=PAPACC_C2_OP_EMPTY)return PAPACC_RESULT_INVALID_STATE;
        if(configuration_c2.stopping)
            result=papacc_server_c2_participant_retire(&configuration_c2_participant,o,o->invocation,&configuration_c2_owner);
        else result=papacc_server_c2_participant_stop(&configuration_c2_participant,o,o->invocation);
        if(result!=PAPACC_RESULT_OK)return result;
        /* No lifetime identity was ever issued/borrowed. Zero-child rollback
         * cannot reclaim a manager installed in an IO owner (guarded above). */
        /* Participant CLOSED tombstone is preserved; no second manager. */
        memset(&configuration_c2_owner,0,sizeof(configuration_c2_owner));
    }
    memset(p,0,sizeof(*p));return PAPACC_RESULT_OK;
}
