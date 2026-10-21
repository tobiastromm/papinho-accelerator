#include "server_run_win32.h"
#include "server_launcher_owner_win32.h"
#include "server_application_lifetime_win32.h"

#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>

#include "papacc/version.h"
#include "server_egress_source_win32.h"
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
#include "papinho_secure_transport.h"
#include "pst_log_adapter.h"
#include "pst_provider_bootstrap_win32.h"
#include "server_secure_io_win32.h"
#include "server_security_source_win32.h"
#include "server_outbound_tls_source_win32.h"
#endif

static PAPACC_RESULT papacc_server_print_listeners(
    FILE *output,
    const PAPACC_SERVER_NETWORK *network)
{
    PAPACC_SIZE index;

    fputs("PapinhoAccelerator Server\n", output);
    fprintf(output, "Software version %s\n\n", papacc_version_string());
    fputs("Server network started.\n\nListening:\n", output);
    for (index = 0; index < network->listener_set.count; ++index) {
        const PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry =
            &network->listener_set.entries[index];
        char address_text[40];
        PAPACC_RESULT result = papacc_ip_address_format(
            &entry->target.address, address_text, sizeof(address_text));
        if (result != PAPACC_RESULT_OK) {
            return result;
        }
        if (entry->target.address.family == PAPACC_IP_FAMILY_IPV6) {
            fprintf(output, "  IPv6 [%s]:%u", address_text,
                    (unsigned int)network->listener_set.bound_port);
            if (entry->target.scope_id != 0) {
                fprintf(output, "  Scope ID: %lu",
                        (unsigned long)entry->target.scope_id);
            }
        } else {
            fprintf(output, "  IPv4 %s:%u", address_text,
                    (unsigned int)network->listener_set.bound_port);
        }
        if (entry->target.interface_instance_id != 0) {
            fprintf(output, "  Runtime Interface ID: %lu",
                    (unsigned long)entry->target.interface_instance_id);
        }
        fputc('\n', output);
    }
    fputs("\nPress Ctrl+C to stop.\n", output);
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_run_win32(
    PAPACC_SERVER_NETWORK *server_network,
    const PAPACC_SERVER_CONFIG *server_config,
    FILE *output,
    FILE *error_output,
    const PAPACC_LOGGER *logger)
{
    const PAPACC_SIZE capacity =
        (PAPACC_SIZE)PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY;
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *owner = NULL;
    PAPACC_SERVER_NETWORK *caller_network = server_network;
    PAPACC_RESULT result = PAPACC_RESULT_OK;
    PAPACC_BOOL manager_stopped = PAPACC_FALSE;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION security_resolution =
        PAPACC_SECURITY_CONFIGURATION_RESOLVER_ERROR;
    PAPACC_SIZE secure_scheduler_capacity = 0U;
    PAPACC_BOOL secure_cleanup_allowed = PAPACC_TRUE;
    PAPACC_RESULT secure_shutdown_result = PAPACC_RESULT_OK;
#endif
    PAPACC_SIZE index;

    if (server_network == NULL || server_config == NULL || output == NULL || error_output == NULL ||
        logger == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    result = papacc_server_launcher_owner_acquire(server_network,
        server_config, logger, &owner);
    if (result != PAPACC_RESULT_OK) return result;
    server_network = &owner->network;
    server_config = &owner->config;
    logger = &owner->logger;
    if (capacity > SIZE_MAX / sizeof(*owner->connection_storage) ||
        capacity > SIZE_MAX / sizeof(*owner->context_storage) ||
        capacity > SIZE_MAX / sizeof(*owner->session_storage) ||
        capacity > SIZE_MAX / sizeof(*owner->channel_storage) ||
        capacity > SIZE_MAX / sizeof(*owner->association_storage) ||
        capacity > SIZE_MAX / sizeof(*owner->processor_storage)) {
        result = PAPACC_RESULT_LIMIT_EXCEEDED;
        goto cleanup;
    }
    owner->connection_storage = (PAPACC_CONNECTION *)malloc(
        capacity * sizeof(*owner->connection_storage));
    owner->context_storage =
        (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *)malloc(
            capacity * sizeof(*owner->context_storage));
    owner->session_storage = (PAPACC_SESSION *)malloc(
        capacity * sizeof(*owner->session_storage));
    owner->channel_storage = (PAPACC_CHANNEL *)malloc(
        capacity * sizeof(*owner->channel_storage));
    owner->association_storage = (PAPACC_DATA_ASSOCIATION_ENTRY *)malloc(
        capacity * sizeof(*owner->association_storage));
    owner->processor_storage = (PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *)malloc(
        capacity * sizeof(*owner->processor_storage));
    if (owner->connection_storage == NULL || owner->context_storage == NULL ||
        owner->session_storage == NULL || owner->channel_storage == NULL ||
        owner->association_storage == NULL || owner->processor_storage == NULL) {
        result = PAPACC_RESULT_OUT_OF_MEMORY;
        goto cleanup;
    }
    for (index = 0; index < capacity; ++index) {
        owner->context_storage[index] =
            (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
                PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    }
    result = papacc_server_acceptor_win32_init(
        &owner->acceptor, server_network, owner->connection_storage, capacity,
        owner->context_storage, capacity);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }
    result = papacc_server_io_loop_win32_init(
        &owner->io_loop, server_network, &owner->acceptor, owner->session_storage, capacity,
        owner->channel_storage, capacity, owner->association_storage, capacity,
        owner->processor_storage, capacity,
        (PAPACC_U64)PAPACC_SERVER_ESTABLISHMENT_TIMEOUT_NS);
    if (result != PAPACC_RESULT_OK) goto cleanup;
    papacc_server_io_loop_win32_set_logger(&owner->io_loop, logger);
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    if (server_config->listener_profile ==
            PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL) {
        if (server_network->listener_set.count > SIZE_MAX - capacity -
                PAPACC_SERVER_NETWORK_STREAM_CAPACITY) {
            result = PAPACC_RESULT_LIMIT_EXCEEDED;
            goto cleanup;
        }
        secure_scheduler_capacity = capacity +
            server_network->listener_set.count +
            PAPACC_SERVER_NETWORK_STREAM_CAPACITY;
        owner->secure_candidates = (PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *)calloc(
            capacity, sizeof(*owner->secure_candidates));
        owner->secure_members = (PAPACC_PST_SCHEDULER_MEMBER *)calloc(
            secure_scheduler_capacity, sizeof(*owner->secure_members));
        owner->secure_events = (PST_WAIT_EVENT *)calloc(
            secure_scheduler_capacity, sizeof(*owner->secure_events));
        owner->listener_sources = (pst_external_source **)calloc(
            server_network->listener_set.count, sizeof(*owner->listener_sources));
        owner->listener_tokens = (pst_wait_token *)calloc(
            server_network->listener_set.count, sizeof(*owner->listener_tokens));
        if (owner->secure_candidates == NULL || owner->secure_members == NULL ||
            owner->secure_events == NULL || owner->listener_sources == NULL ||
            owner->listener_tokens == NULL) {
            result = PAPACC_RESULT_OUT_OF_MEMORY;
            goto cleanup;
        }
        owner->secure_listener.bind_selection = server_config->bind_selection;
        owner->secure_listener.port = server_config->control_port;
        owner->secure_listener.transport_profile =
            PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL;
        result = papacc_security_configuration_ref_set(
            &owner->secure_listener.security_configuration_ref,
            server_config->security_configuration_ref,
            server_config->security_configuration_ref_length);
        if (result != PAPACC_RESULT_OK) goto cleanup;
        owner->secure_listener_set.listeners = &owner->secure_listener;
        owner->secure_listener_set.count = 1U;
        result = papacc_server_security_configuration_resolve_and_compose(
            &owner->secure_listener.security_configuration_ref,
            papacc_server_security_environment_resolve, (void *)logger,
            &security_resolution, &owner->resolved_security,
            &owner->security_composition);
        if (result != PAPACC_RESULT_OK || security_resolution !=
                PAPACC_SECURITY_CONFIGURATION_RESOLVED) {
            result = PAPACC_RESULT_INVALID_STATE;
            goto cleanup;
        }
        result = papacc_server_secure_io_win32_init(&owner->secure_io,
            server_network, &owner->secure_listener_set, &owner->io_loop,
            &owner->security_composition, &owner->resolved_security, owner->secure_candidates,
            capacity, owner->secure_members, owner->secure_events,
            secure_scheduler_capacity, owner->listener_sources, owner->listener_tokens,
            server_network->listener_set.count,
            (PAPACC_U64)PAPACC_SERVER_ESTABLISHMENT_TIMEOUT_NS);
        if (result != PAPACC_RESULT_OK) goto cleanup;
    }
#else
    if (server_config->listener_profile ==
            PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL) {
        result = PAPACC_RESULT_NOT_SUPPORTED;
        goto cleanup;
    }
#endif
    if(server_config->allow_network_egress==PAPACC_TRUE){
        result=papacc_server_egress_environment_resolve(server_config,
            &owner->egress_source);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
        result=papacc_server_io_loop_win32_set_egress_config(
            &owner->io_loop,&owner->egress_source.runtime);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
        result=papacc_server_io_loop_win32_set_dns_resolver(
            &owner->io_loop,&owner->egress_source.resolver);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
        result=papacc_pst_provider_bootstrap_win32(NULL);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
        result=papacc_pst_log_adapter_init(&owner->pst_log_adapter,logger);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
        result=papacc_pst_log_adapter_make_config(&owner->pst_log_adapter,
            &owner->pst_log_config);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
        memset(&owner->pst_options,0,sizeof(owner->pst_options));
        owner->pst_options.struct_size=(pst_u32)sizeof(owner->pst_options);
        owner->pst_options.api_version=PST_API_VERSION;
        if(pst_runtime_create_with_logging(&owner->pst_options,&owner->pst_log_config,
            &owner->pst_runtime_instance,NULL)!=PST_RESULT_OK){
            result=PAPACC_RESULT_INTERNAL_ERROR;goto cleanup;
        }
        result=papacc_server_outbound_tls_environment_resolve(server_config,
            &owner->resolved_outbound_tls);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
        memset(&owner->pst_trust_source,0,sizeof(owner->pst_trust_source));
        owner->pst_trust_source.struct_size=(pst_u32)sizeof(owner->pst_trust_source);
        owner->pst_trust_source.api_version=PST_API_VERSION;
        if(owner->resolved_outbound_tls.trust_kind==
                PAPACC_OUTBOUND_TLS_TRUST_CUSTOM_CA_DER){
            owner->pst_trust_anchor.data=owner->resolved_outbound_tls.custom_ca_der;
            owner->pst_trust_anchor.size=owner->resolved_outbound_tls.custom_ca_der_size;
            owner->pst_trust_source.kind=PST_TRUST_SOURCE_CUSTOM_CA_DER;
            owner->pst_trust_source.anchors=&owner->pst_trust_anchor;
            owner->pst_trust_source.anchor_count=1U;
        }else owner->pst_trust_source.kind=PST_TRUST_SOURCE_SYSTEM;
        if(pst_trust_create(&owner->pst_trust_source,&owner->pst_system_trust)!=
            PST_RESULT_OK){result=PAPACC_RESULT_INTERNAL_ERROR;goto cleanup;}
        memset(&owner->outbound_tls_policy,0,sizeof(owner->outbound_tls_policy));
        owner->outbound_tls_policy.runtime=owner->pst_runtime_instance;
        owner->outbound_tls_policy.trust=owner->pst_system_trust;
        owner->outbound_tls_policy.provider_id=owner->resolved_outbound_tls.provider_id;
        result=papacc_server_io_loop_win32_set_outbound_tls(&owner->io_loop,
            &owner->outbound_tls_policy);
        if(result!=PAPACC_RESULT_OK)goto cleanup;
#endif
    }
    result = papacc_server_print_listeners(output, server_network);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }
    fprintf(output, "Listener profile: %s\n",
        server_config->listener_profile ==
            PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL ?
            "SECURE_PRINCIPAL" : "LEGACY_ENDPOINT");
    fputs("Accepting Control establishment connections.\n", output);
    papacc_log_event(logger, PAPACC_LOG_INFO, PAPACC_LOG_EVENT_SERVER_STARTED,
        PAPACC_LOG_CATEGORY_NETWORK, PAPACC_LOG_COMPONENT_SERVER,
        PAPACC_LOG_OPERATION_STARTUP, PAPACC_TRUE, PAPACC_RESULT_OK, NULL,
        "Listener started; accepting Phase 2 connections");
    (void)fflush(output);
    while (papacc_server_console_win32_stop_requested() == PAPACC_FALSE) {
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
        if (server_config->listener_profile ==
                PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL)
            result = papacc_server_secure_io_win32_poll_once(
                &owner->secure_io, PAPACC_SERVER_IO_LOOP_POLL_TIMEOUT_MS);
        else
#endif
        result = papacc_server_io_loop_win32_poll_once(
            &owner->io_loop, PAPACC_SERVER_IO_LOOP_POLL_TIMEOUT_MS);
        if (result != PAPACC_RESULT_OK) {
            fprintf(error_output, "Server I/O Loop failed: %d\n", (int)result);
            (void)fflush(error_output);
            break;
        }
    }

cleanup:
    papacc_log_event(logger, PAPACC_LOG_INFO, PAPACC_LOG_EVENT_SERVER_STOPPING,
        PAPACC_LOG_CATEGORY_RUNTIME, PAPACC_LOG_COMPONENT_SERVER,
        PAPACC_LOG_OPERATION_SHUTDOWN, PAPACC_TRUE, result, NULL,
        "Server stopping");
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    if (owner->secure_io.initialized == PAPACC_TRUE) {
        secure_shutdown_result =
            papacc_server_secure_io_win32_request_stop(&owner->secure_io);
        for (index = 0U; index < 400U &&
                papacc_server_secure_io_win32_is_stopped(&owner->secure_io) !=
                    PAPACC_TRUE &&
                owner->secure_io.stop_state !=
                    PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL; ++index) {
            PAPACC_RESULT poll_result =
                papacc_server_secure_io_win32_poll_once(&owner->secure_io, 25U);
            if (poll_result != PAPACC_RESULT_OK)
                secure_shutdown_result = poll_result;
        }
        if (papacc_server_secure_io_win32_is_stopped(&owner->secure_io) ==
                PAPACC_TRUE)
            secure_shutdown_result =
                papacc_server_secure_io_win32_try_shutdown(&owner->secure_io);
        else if (secure_shutdown_result == PAPACC_RESULT_OK)
            secure_shutdown_result = PAPACC_RESULT_INVALID_STATE;
        if (secure_shutdown_result != PAPACC_RESULT_OK) {
            if (result == PAPACC_RESULT_OK) result = secure_shutdown_result;
            secure_cleanup_allowed = PAPACC_FALSE;
        }
    } else if (owner->secure_io.scheduler.initialized == PAPACC_TRUE) {
        secure_shutdown_result =
            papacc_server_secure_io_win32_try_shutdown(&owner->secure_io);
        if (secure_shutdown_result != PAPACC_RESULT_OK) {
            if (result == PAPACC_RESULT_OK) result = secure_shutdown_result;
            secure_cleanup_allowed = PAPACC_FALSE;
        }
    }
    if (secure_cleanup_allowed != PAPACC_TRUE) goto retain;
#endif
    /* Void loop shutdown may preserve retained outbound children. Observe its
     * postcondition before releasing any runtime/configuration/buffer lease. */
    papacc_server_io_loop_win32_shutdown(&owner->io_loop);
    if (owner->io_loop.initialized == PAPACC_TRUE) goto retain;
    if (owner->acceptor.connection_manager.initialized == PAPACC_TRUE) {
        PAPACC_RESULT manager_result = papacc_connection_manager_try_shutdown(
            &owner->acceptor.connection_manager, &manager_stopped);
        if (manager_result != PAPACC_RESULT_OK || manager_stopped != PAPACC_TRUE)
            goto retain;
    }
    if(papacc_server_acceptor_win32_try_shutdown(&owner->acceptor)!=PAPACC_RESULT_OK)goto retain;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    papacc_security_composition_release(&owner->security_composition);
    papacc_resolved_server_security_configuration_release(&owner->resolved_security);
    if(owner->pst_system_trust!=NULL){
        pst_trust_release(owner->pst_system_trust);owner->pst_system_trust=NULL;
    }
    if(owner->pst_runtime_instance!=NULL){
        pst_runtime_release(owner->pst_runtime_instance);owner->pst_runtime_instance=NULL;
    }
    papacc_resolved_outbound_tls_profile_release(&owner->resolved_outbound_tls);
    papacc_pst_log_adapter_release(&owner->pst_log_adapter);
    free(owner->listener_tokens); owner->listener_tokens = NULL;
    free(owner->listener_sources); owner->listener_sources = NULL;
    free(owner->secure_events); owner->secure_events = NULL;
    free(owner->secure_members); owner->secure_members = NULL;
    free(owner->secure_candidates); owner->secure_candidates = NULL;
#endif
    free(owner->processor_storage); owner->processor_storage = NULL;
    free(owner->association_storage); owner->association_storage = NULL;
    free(owner->channel_storage); owner->channel_storage = NULL;
    free(owner->session_storage); owner->session_storage = NULL;
    free(owner->context_storage); owner->context_storage = NULL;
    free(owner->connection_storage); owner->connection_storage = NULL;
    if (papacc_server_launcher_owner_finish(owner, caller_network,
            PAPACC_TRUE) != PAPACC_RESULT_OK) {
        (void)papacc_application_parent_retain();
        return PAPACC_RESULT_INVALID_STATE;
    }
    return result;
retain:
    (void)papacc_application_parent_retain();
    /* Retained ownership transfers the network/WinSock lease into the root.
     * The caller's aggregate stays empty; no caller WSACleanup can reach it. */
    (void)papacc_server_launcher_owner_finish(owner, caller_network, PAPACC_FALSE);
    return result == PAPACC_RESULT_OK ? PAPACC_RESULT_INVALID_STATE : result;
}
