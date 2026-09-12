#include "server_secure_io_win32.h"

#include "pal_time.h"
#include "papinho_secure_transport_win32.h"

static PAPACC_U64 papacc_secure_deadline(PAPACC_U64 now, PAPACC_U64 timeout)
{
    PAPACC_U64 maximum = ~(PAPACC_U64)0;
    return now > maximum - timeout ? maximum : now + timeout;
}

static PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *papacc_secure_free_candidate(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        if (secure_io->candidates[index].state ==
                PAPACC_SERVER_SECURE_CANDIDATE_UNUSED ||
            secure_io->candidates[index].state ==
                PAPACC_SERVER_SECURE_CANDIDATE_CLOSED)
            return &secure_io->candidates[index];
    }
    return NULL;
}

static PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *papacc_secure_find_token(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, pst_wait_token token)
{
    PAPACC_SIZE index;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        if (secure_io->candidates[index].state !=
                PAPACC_SERVER_SECURE_CANDIDATE_UNUSED &&
            secure_io->candidates[index].state !=
                PAPACC_SERVER_SECURE_CANDIDATE_CLOSED &&
            secure_io->candidates[index].wait_token == token)
            return &secure_io->candidates[index];
    }
    return NULL;
}

static PAPACC_RESULT papacc_secure_lookup_session(void *context,
    PAPACC_U64 session_instance_id,
    const PAPACC_SESSION_SECURITY_CONTEXT **out_context)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io =
        (PAPACC_SERVER_SECURE_IO_WIN32 *)context;
    PAPACC_SIZE index;
    *out_context = NULL;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot;
        if (candidate->connection_instance_id == 0U ||
            candidate->session_security.published != PAPACC_TRUE)
            continue;
        slot = papacc_server_io_loop_win32_find_connection_slot(
            secure_io->protocol_loop, candidate->connection_instance_id);
        if (slot != NULL && slot->kind ==
                PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL &&
            slot->post_control_processor.session_instance_id ==
                session_instance_id) {
            *out_context = &candidate->session_security;
            return PAPACC_RESULT_OK;
        }
    }
    return PAPACC_RESULT_INVALID_STATE;
}

static PAPACC_RESULT papacc_secure_issue_gate(void *context,
    PAPACC_U64 session_instance_id, PAPACC_U64 now_ns,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket,
    PAPACC_U64 *out_deadline_ns)
{
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
        (PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *)context;
    PAPACC_SECURE_DATA_OUTCOME outcome;
    PAPACC_RESULT result = papacc_secure_data_ticket_issue(
        &candidate->owner->protocol_loop->association_manager,
        session_instance_id, &candidate->session_security,
        candidate->owner->security_configuration->authorize,
        candidate->owner->security_configuration->authorization_context,
        now_ns, out_ticket, out_deadline_ns, &outcome);
    if (result != PAPACC_RESULT_OK) return result;
    return outcome == PAPACC_SECURE_DATA_OUTCOME_ACCEPTED ? PAPACC_RESULT_OK :
        PAPACC_RESULT_INVALID_STATE;
}

static PAPACC_RESULT papacc_secure_data_gate(void *context,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U64 now_ns,
    PAPACC_U64 connection_instance_id, PAPACC_U64 *out_session_instance_id,
    PAPACC_U64 *out_channel_instance_id)
{
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
        (PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *)context;
    PAPACC_SECURE_DATA_OUTCOME outcome;
    PAPACC_RESULT result = papacc_secure_data_attach(
        &candidate->owner->protocol_loop->association_manager,
        &candidate->owner->protocol_loop->channel_manager, ticket, now_ns,
        connection_instance_id, &candidate->connection_security,
        papacc_secure_lookup_session, candidate->owner,
        candidate->owner->security_configuration->authorize,
        candidate->owner->security_configuration->authorization_context,
        out_session_instance_id, out_channel_instance_id, &outcome);
    if (result != PAPACC_RESULT_OK) return result;
    return outcome == PAPACC_SECURE_DATA_OUTCOME_ACCEPTED ? PAPACC_RESULT_OK :
        PAPACC_RESULT_INVALID_STATE;
}

static void papacc_secure_candidate_close(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
    PAPACC_CONNECTION *connection;
    if (candidate->owner != NULL && candidate->connection_instance_id != 0U) {
        connection = papacc_connection_manager_find(
            candidate->owner->protocol_loop->connection_manager,
            candidate->connection_instance_id);
        if (connection != NULL) papacc_connection_close(connection);
    } else if (candidate->owner != NULL &&
               candidate->secure_processor.connection != NULL) {
        (void)papacc_pst_secure_scheduler_remove_connection(
            &candidate->owner->scheduler,
            candidate->secure_processor.connection);
        papacc_pst_secure_processor_release(&candidate->secure_processor);
    }
    papacc_connection_security_context_release(
        &candidate->connection_security);
    papacc_session_security_context_release(&candidate->session_security);
    candidate->state = PAPACC_SERVER_SECURE_CANDIDATE_CLOSED;
}

static PAPACC_BOOL papacc_secure_candidate_is_control(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot;
    if (candidate->connection_instance_id == 0U) return PAPACC_FALSE;
    slot = papacc_server_io_loop_win32_find_connection_slot(
        candidate->owner->protocol_loop, candidate->connection_instance_id);
    if (slot == NULL) return PAPACC_FALSE;
    return slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_CONTROL_ESTABLISHMENT ||
        slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL ?
        PAPACC_TRUE : PAPACC_FALSE;
}

static void papacc_secure_begin_candidate_shutdown(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
    PAPACC_PST_SECURE_STEP step;
    candidate->state = PAPACC_SERVER_SECURE_CANDIDATE_SHUTTING_DOWN;
    if (papacc_pst_secure_processor_shutdown_once(
            &candidate->secure_processor, &step) != PAPACC_RESULT_OK ||
        step == PAPACC_PST_SECURE_STEP_FAILED ||
        step == PAPACC_PST_SECURE_STEP_COMPLETE ||
        step == PAPACC_PST_SECURE_STEP_CLOSED)
        papacc_secure_candidate_close(candidate);
}

static void papacc_secure_advance_stop(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    PAPACC_BOOL non_control_active = PAPACC_FALSE;
    if (secure_io->stop_requested != PAPACC_TRUE) return;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        if (candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_UNUSED &&
            candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_CLOSED &&
            papacc_secure_candidate_is_control(candidate) != PAPACC_TRUE) {
            non_control_active = PAPACC_TRUE;
            break;
        }
    }
    if (non_control_active == PAPACC_TRUE) return;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        if (candidate->state == PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL &&
            papacc_secure_candidate_is_control(candidate) == PAPACC_TRUE)
            papacc_secure_begin_candidate_shutdown(candidate);
    }
}

static PAPACC_RESULT papacc_secure_accept(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_SIZE listener_index,
    PAPACC_U64 now_ns)
{
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted =
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate;
    PST_CONNECTION_CONFIG config;
    pst_transport *transport = NULL;
    PAPACC_BOOL ownership_accepted = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP handshake_step;
    PAPACC_RESULT result;
    if (listener_index >= secure_io->network->listener_set.count)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    candidate = papacc_secure_free_candidate(secure_io);
    if (candidate == NULL) return PAPACC_RESULT_LIMIT_EXCEEDED;
    *candidate = (PAPACC_SERVER_SECURE_CANDIDATE_WIN32)
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER;
    candidate->owner = secure_io;
    result = papacc_tcp_socket_win32_accept(
        &secure_io->network->listener_set.entries[listener_index].socket,
        &accepted);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_tcp_accepted_socket_win32_set_nonblocking(
        &accepted, PAPACC_TRUE);
    if (result != PAPACC_RESULT_OK) goto fail;
    candidate->local_endpoint = accepted.local_endpoint;
    candidate->remote_endpoint = accepted.remote_endpoint;
    result = papacc_security_composition_build_secure_principal_config(
        secure_io->security_composition, &config);
    if (result != PAPACC_RESULT_OK) goto fail;
    result = papacc_pst_secure_processor_init(&candidate->secure_processor,
        secure_io->security_composition->runtime, &config,
        papacc_secure_deadline(now_ns, secure_io->establishment_timeout_ns));
    if (result != PAPACC_RESULT_OK) goto fail;
    if (pst_win32_socket_transport_create((pst_size)accepted.native_socket,
            &transport) != PST_RESULT_OK) {
        result = PAPACC_RESULT_INTERNAL_ERROR;
        goto fail;
    }
    result = papacc_pst_secure_processor_attach(&candidate->secure_processor,
        transport, &ownership_accepted);
    if (ownership_accepted == PAPACC_TRUE) {
        accepted.native_socket = INVALID_SOCKET;
        accepted.is_open = PAPACC_FALSE;
    }
    if (result != PAPACC_RESULT_OK) goto fail;
    result = papacc_pst_secure_processor_handshake_once(
        &candidate->secure_processor, &handshake_step);
    if (result != PAPACC_RESULT_OK ||
        handshake_step == PAPACC_PST_SECURE_STEP_FAILED ||
        handshake_step == PAPACC_PST_SECURE_STEP_CLOSED ||
        handshake_step == PAPACC_PST_SECURE_STEP_COMPLETE) {
        if (result == PAPACC_RESULT_OK)
            result = PAPACC_RESULT_INVALID_STATE;
        goto fail;
    }
    result = papacc_pst_secure_scheduler_add_connection(&secure_io->scheduler,
        candidate->secure_processor.connection, &candidate->wait_token);
    if (result != PAPACC_RESULT_OK) goto fail;
    candidate->state = PAPACC_SERVER_SECURE_CANDIDATE_HANDSHAKING;
    return PAPACC_RESULT_OK;
fail:
    if (ownership_accepted != PAPACC_TRUE && transport != NULL)
        pst_transport_release(transport);
    papacc_tcp_accepted_socket_win32_close(&accepted);
    if (candidate->secure_processor.connection != NULL)
        papacc_pst_secure_processor_release(&candidate->secure_processor);
    candidate->state = PAPACC_SERVER_SECURE_CANDIDATE_CLOSED;
    /* The listener and scheduler remain healthy when one accepted peer fails
       during candidate-local transport setup or its first TLS step. */
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_secure_publish(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate, PAPACC_U64 now_ns)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io = candidate->owner;
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_RESULT result = papacc_pst_secure_transport_adapter_init(
        &candidate->transport_adapter, &candidate->secure_processor,
        &secure_io->scheduler, &transport);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_connection_manager_publish(
        secure_io->protocol_loop->connection_manager, &transport,
        &candidate->local_endpoint, &candidate->remote_endpoint, &connection);
    if (result != PAPACC_RESULT_OK) {
        papacc_transport_connection_close(&transport);
        return result;
    }
    candidate->connection_instance_id = connection->connection_instance_id;
    result = papacc_server_io_loop_win32_attach_connection(
        secure_io->protocol_loop, candidate->connection_instance_id, now_ns);
    if (result != PAPACC_RESULT_OK) {
        papacc_connection_close(connection);
        return result;
    }
    candidate->state = PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_secure_after_protocol_step(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot =
        papacc_server_io_loop_win32_find_connection_slot(
            candidate->owner->protocol_loop,
            candidate->connection_instance_id);
    PAPACC_AUTHORIZATION_DECISION decision = PAPACC_AUTHORIZATION_DENY;
    PAPACC_BOOL read_interest;
    PAPACC_BOOL write_interest;
    PAPACC_SERVER_PROTOCOL_SLOT_KIND_WIN32 kind_before;
    PAPACC_RESULT result;
    if (slot == NULL) return PAPACC_RESULT_INVALID_STATE;
    if (slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_CONTROL_ESTABLISHMENT &&
        candidate->control_authorized != PAPACC_TRUE) {
        result = candidate->owner->security_configuration->authorize(
            candidate->owner->security_configuration->authorization_context,
            &candidate->connection_security.principal,
            PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION, &decision);
        if (result != PAPACC_RESULT_OK ||
            decision != PAPACC_AUTHORIZATION_ALLOW)
            return PAPACC_RESULT_INVALID_STATE;
        candidate->control_authorized = PAPACC_TRUE;
    }
    if (slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL &&
        candidate->issuance_gate_installed != PAPACC_TRUE) {
        result = papacc_session_security_context_publish(
            &candidate->session_security,
            &candidate->connection_security);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_post_control_processor_set_ticket_issue_gate(
            &slot->post_control_processor, papacc_secure_issue_gate, candidate);
        if (result != PAPACC_RESULT_OK) return result;
        candidate->issuance_gate_installed = PAPACC_TRUE;
    }
    if (slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH &&
        candidate->data_gate_installed != PAPACC_TRUE) {
        result = papacc_data_attach_processor_set_commit_gate(
            &slot->data_attach_processor, papacc_secure_data_gate, candidate);
        if (result != PAPACC_RESULT_OK) return result;
        candidate->data_gate_installed = PAPACC_TRUE;
    }
    result = papacc_server_io_loop_win32_processor_interest(
        candidate->owner->protocol_loop,
        (PAPACC_SIZE)(slot - candidate->owner->protocol_loop->processor_slots),
        &read_interest, &write_interest);
    if (result != PAPACC_RESULT_OK) return result;
    if (read_interest == PAPACC_TRUE) {
        kind_before = slot->kind;
        result = papacc_server_io_loop_win32_process_connection_once(
            candidate->owner->protocol_loop,
            candidate->connection_instance_id, PAPACC_TRUE, PAPACC_FALSE, 0U);
        if (result != PAPACC_RESULT_OK) return result;
        slot = papacc_server_io_loop_win32_find_connection_slot(
            candidate->owner->protocol_loop,
            candidate->connection_instance_id);
        if (slot == NULL) return PAPACC_RESULT_INVALID_STATE;
        result = papacc_server_io_loop_win32_processor_interest(
            candidate->owner->protocol_loop,
            (PAPACC_SIZE)(slot -
                candidate->owner->protocol_loop->processor_slots),
            &read_interest, &write_interest);
        if (result != PAPACC_RESULT_OK) return result;
        if (slot->kind != kind_before || write_interest == PAPACC_TRUE)
            return papacc_secure_after_protocol_step(candidate);
    }
    if (write_interest == PAPACC_TRUE) {
        kind_before = slot->kind;
        result = papacc_server_io_loop_win32_process_connection_once(
            candidate->owner->protocol_loop,
            candidate->connection_instance_id, PAPACC_FALSE, PAPACC_TRUE, 0U);
        if (result != PAPACC_RESULT_OK) return result;
        slot = papacc_server_io_loop_win32_find_connection_slot(
            candidate->owner->protocol_loop,
            candidate->connection_instance_id);
        if (slot != NULL && slot->kind != kind_before)
            return papacc_secure_after_protocol_step(candidate);
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_secure_candidate_ready(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,
    const PAPACC_PST_READY_EVENT *event, PAPACC_U64 now_ns)
{
    PAPACC_RESULT result;
    if (candidate->state ==
            PAPACC_SERVER_SECURE_CANDIDATE_SHUTTING_DOWN) {
        PAPACC_PST_SECURE_STEP step;
        result = papacc_pst_secure_processor_shutdown_once(
            &candidate->secure_processor, &step);
        if (result != PAPACC_RESULT_OK) return result;
        if (step == PAPACC_PST_SECURE_STEP_COMPLETE ||
            step == PAPACC_PST_SECURE_STEP_CLOSED)
            papacc_secure_candidate_close(candidate);
        return PAPACC_RESULT_OK;
    }
    if (candidate->state == PAPACC_SERVER_SECURE_CANDIDATE_HANDSHAKING) {
        PAPACC_PST_SECURE_STEP step;
        PAPACC_PEER_EVIDENCE evidence;
        PAPACC_PRINCIPAL_RESOLUTION resolution;
        PAPACC_AUTHORIZATION_DECISION decision;
        result = papacc_pst_secure_processor_handshake_once(
            &candidate->secure_processor, &step);
        if (result != PAPACC_RESULT_OK ||
            step == PAPACC_PST_SECURE_STEP_FAILED ||
            step == PAPACC_PST_SECURE_STEP_CLOSED)
            return PAPACC_RESULT_INVALID_STATE;
        if (step != PAPACC_PST_SECURE_STEP_COMPLETE) return PAPACC_RESULT_OK;
        result = papacc_pst_peer_evidence_extract(
            &candidate->secure_processor, &evidence);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_authenticate_and_authorize(
            &candidate->connection_security, &evidence,
            candidate->owner->security_configuration->principal_resolve,
            candidate->owner->security_configuration->principal_resolve_context,
            candidate->owner->security_configuration->authorize,
            candidate->owner->security_configuration->authorization_context,
            PAPACC_AUTHORIZATION_ACCESS_ACCELERATOR, &resolution, &decision);
        if (result != PAPACC_RESULT_OK ||
            candidate->connection_security.state !=
                PAPACC_SECURITY_CONTEXT_AUTHORIZED)
            return PAPACC_RESULT_INVALID_STATE;
        return papacc_secure_publish(candidate, now_ns);
    }
    if (candidate->state == PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL) {
        result = papacc_server_io_loop_win32_process_connection_once(
            candidate->owner->protocol_loop,
            candidate->connection_instance_id, event->read_ready,
            event->write_ready, now_ns);
        if (result != PAPACC_RESULT_OK) return result;
        return papacc_secure_after_protocol_step(candidate);
    }
    return PAPACC_RESULT_INVALID_STATE;
}

typedef struct PAPACC_SECURE_DISPATCH_CONTEXT {
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io;
    PAPACC_U64 now_ns;
    PAPACC_RESULT result;
} PAPACC_SECURE_DISPATCH_CONTEXT;

static PAPACC_RESULT papacc_secure_drive_pending_writes(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U64 now_ns)
{
    PAPACC_SIZE index;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot;
        PAPACC_BOOL read_interest;
        PAPACC_BOOL write_interest;
        PAPACC_RESULT result;
        if (candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL)
            continue;
        slot = papacc_server_io_loop_win32_find_connection_slot(
            secure_io->protocol_loop, candidate->connection_instance_id);
        if (slot == NULL) continue;
        result = papacc_server_io_loop_win32_processor_interest(
            secure_io->protocol_loop,
            (PAPACC_SIZE)(slot - secure_io->protocol_loop->processor_slots),
            &read_interest, &write_interest);
        if (result != PAPACC_RESULT_OK) return result;
        (void)read_interest;
        if (write_interest != PAPACC_TRUE) continue;
        result = papacc_server_io_loop_win32_process_connection_once(
            secure_io->protocol_loop, candidate->connection_instance_id,
            PAPACC_FALSE, PAPACC_TRUE, now_ns);
        if (result != PAPACC_RESULT_OK) {
            papacc_secure_candidate_close(candidate);
            continue;
        }
        result = papacc_secure_after_protocol_step(candidate);
        if (result != PAPACC_RESULT_OK) {
            papacc_secure_candidate_close(candidate);
            continue;
        }
    }
    return PAPACC_RESULT_OK;
}

static void papacc_secure_dispatch(void *context,
    const PAPACC_PST_READY_EVENT *event)
{
    PAPACC_SECURE_DISPATCH_CONTEXT *dispatch =
        (PAPACC_SECURE_DISPATCH_CONTEXT *)context;
    PAPACC_SIZE index;
    if (dispatch->result != PAPACC_RESULT_OK) return;
    if (event->kind == PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL) {
        if (dispatch->secure_io->stop_requested == PAPACC_TRUE) return;
        for (index = 0U; index < dispatch->secure_io->listener_count; ++index) {
            if (dispatch->secure_io->listener_tokens[index] == event->token) {
                dispatch->result = papacc_secure_accept(
                    dispatch->secure_io, index, dispatch->now_ns);
                return;
            }
        }
        dispatch->result = PAPACC_RESULT_INVALID_STATE;
        return;
    }
    {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            papacc_secure_find_token(dispatch->secure_io, event->token);
        if (candidate == NULL) {
            dispatch->result = PAPACC_RESULT_INVALID_STATE;
            return;
        }
        dispatch->result = papacc_secure_candidate_ready(
            candidate, event, dispatch->now_ns);
        if (dispatch->result != PAPACC_RESULT_OK) {
            papacc_secure_candidate_close(candidate);
            dispatch->result = PAPACC_RESULT_OK;
        }
    }
}

PAPACC_RESULT papacc_server_secure_io_win32_init(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_SERVER_NETWORK *network,
    const PAPACC_SERVER_LISTENER_CONFIGURATION_SET *listener_configuration,
    PAPACC_SERVER_IO_LOOP_WIN32 *protocol_loop,
    PAPACC_SECURITY_COMPOSITION *security_composition,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *security_configuration,
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidates,
    PAPACC_SIZE candidate_capacity,
    PAPACC_PST_SCHEDULER_MEMBER *scheduler_members,
    PST_WAIT_EVENT *scheduler_events, PAPACC_SIZE scheduler_capacity,
    pst_external_source **listener_sources, pst_wait_token *listener_tokens,
    PAPACC_SIZE listener_capacity, PAPACC_U64 establishment_timeout_ns)
{
    PAPACC_SIZE index;
    PAPACC_RESULT result;
    if (secure_io == NULL || network == NULL ||
        listener_configuration == NULL || protocol_loop == NULL ||
        security_composition == NULL || security_configuration == NULL ||
        candidates == NULL || candidate_capacity == 0U ||
        scheduler_members == NULL || scheduler_events == NULL ||
        scheduler_capacity < candidate_capacity + network->listener_set.count ||
        listener_sources == NULL || listener_tokens == NULL ||
        listener_capacity < network->listener_set.count ||
        establishment_timeout_ns == 0U || listener_configuration->count != 1U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (secure_io->initialized == PAPACC_TRUE ||
        network->is_active != PAPACC_TRUE || protocol_loop->initialized !=
            PAPACC_TRUE ||
        papacc_security_composition_is_ready(security_composition) !=
            PAPACC_TRUE ||
        papacc_resolved_server_security_configuration_validate(
            security_configuration) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INVALID_STATE;
    if (papacc_server_listener_configuration_set_validate(
            listener_configuration) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INVALID_STATE;
    if (listener_configuration->listeners[0].transport_profile !=
            PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_pst_secure_scheduler_init(&secure_io->scheduler,
        scheduler_members, scheduler_capacity, scheduler_events,
        scheduler_capacity);
    if (result != PAPACC_RESULT_OK) return result;
    secure_io->network = network;
    secure_io->listener_configuration = listener_configuration;
    secure_io->protocol_loop = protocol_loop;
    secure_io->security_composition = security_composition;
    secure_io->security_configuration = security_configuration;
    secure_io->candidates = candidates;
    secure_io->candidate_capacity = candidate_capacity;
    secure_io->listener_sources = listener_sources;
    secure_io->listener_tokens = listener_tokens;
    secure_io->listener_count = network->listener_set.count;
    secure_io->establishment_timeout_ns = establishment_timeout_ns;
    secure_io->shutdown_deadline_ns = 0U;
    secure_io->stop_requested = PAPACC_FALSE;
    for (index = 0U; index < candidate_capacity; ++index)
        candidates[index] = (PAPACC_SERVER_SECURE_CANDIDATE_WIN32)
            PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER;
    for (index = 0U; index < secure_io->listener_count; ++index) {
        listener_sources[index] = NULL;
        result = papacc_pst_external_source_win32_create(
            (PAPACC_SIZE)network->listener_set.entries[index].socket.native_socket,
            &listener_sources[index]);
        if (result == PAPACC_RESULT_OK)
            result = papacc_pst_secure_scheduler_add_external(
                &secure_io->scheduler, listener_sources[index], PAPACC_TRUE,
                PAPACC_FALSE, &listener_tokens[index]);
        if (result != PAPACC_RESULT_OK) {
            secure_io->listener_count = index + 1U;
            papacc_server_secure_io_win32_shutdown(secure_io);
            return result;
        }
    }
    secure_io->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_secure_io_win32_poll_once(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U32 timeout_ms)
{
    PAPACC_PST_WAIT_OUTCOME outcome;
    PAPACC_SIZE ready_count, dispatched;
    PAPACC_SECURE_DISPATCH_CONTEXT dispatch;
    PAPACC_RESULT result;
    if (secure_io == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (secure_io->initialized != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_pst_secure_scheduler_wait(&secure_io->scheduler,
        timeout_ms, &outcome, &ready_count);
    if (result != PAPACC_RESULT_OK) return result;
    if (papacc_pal_monotonic_time_ns(&dispatch.now_ns) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INTERNAL_ERROR;
    {
        PAPACC_SIZE index;
        for (index = 0U; index < secure_io->candidate_capacity; ++index) {
            PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
                &secure_io->candidates[index];
            PAPACC_BOOL expired = PAPACC_FALSE;
            if (candidate->state !=
                    PAPACC_SERVER_SECURE_CANDIDATE_HANDSHAKING &&
                candidate->state !=
                    PAPACC_SERVER_SECURE_CANDIDATE_SHUTTING_DOWN)
                continue;
            if (candidate->state ==
                    PAPACC_SERVER_SECURE_CANDIDATE_SHUTTING_DOWN) {
                if (dispatch.now_ns >= secure_io->shutdown_deadline_ns)
                    papacc_secure_candidate_close(candidate);
                continue;
            }
            result = papacc_pst_secure_processor_check_deadline(
                &candidate->secure_processor, dispatch.now_ns, &expired);
            if (result != PAPACC_RESULT_OK || expired == PAPACC_TRUE)
                papacc_secure_candidate_close(candidate);
        }
    }
    papacc_secure_advance_stop(secure_io);
    if (outcome != PAPACC_PST_WAIT_READY) {
        result = papacc_secure_drive_pending_writes(
            secure_io, dispatch.now_ns);
        if (result != PAPACC_RESULT_OK) return result;
        return papacc_server_io_loop_win32_maintenance(
            secure_io->protocol_loop, dispatch.now_ns);
    }
    dispatch.secure_io = secure_io;
    dispatch.result = PAPACC_RESULT_OK;
    result = papacc_pst_secure_scheduler_dispatch_ready(
        &secure_io->scheduler, papacc_secure_dispatch, &dispatch, &dispatched);
    if (result != PAPACC_RESULT_OK) return result;
    (void)ready_count;
    (void)dispatched;
    if (dispatch.result != PAPACC_RESULT_OK) return dispatch.result;
    result = papacc_secure_drive_pending_writes(
        secure_io, dispatch.now_ns);
    if (result != PAPACC_RESULT_OK) return result;
    return papacc_server_io_loop_win32_maintenance(
        secure_io->protocol_loop, dispatch.now_ns);
}

PAPACC_RESULT papacc_server_secure_io_win32_request_stop(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    PAPACC_U64 now_ns;
    if (secure_io == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (secure_io->initialized != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (secure_io->stop_requested == PAPACC_TRUE)
        return PAPACC_RESULT_OK;
    if (papacc_pal_monotonic_time_ns(&now_ns) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INTERNAL_ERROR;
    secure_io->stop_requested = PAPACC_TRUE;
    secure_io->shutdown_deadline_ns = papacc_secure_deadline(
        now_ns, secure_io->establishment_timeout_ns);
    for (index = 0U; index < secure_io->listener_count; ++index) {
        if (secure_io->listener_sources[index] != NULL) {
            (void)papacc_pst_secure_scheduler_remove_external(
                &secure_io->scheduler, secure_io->listener_sources[index]);
            pst_external_source_release(secure_io->listener_sources[index]);
            secure_io->listener_sources[index] = NULL;
        }
    }
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        if (candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_UNUSED &&
            candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_CLOSED) {
            if (papacc_secure_candidate_is_control(candidate) != PAPACC_TRUE)
                papacc_secure_begin_candidate_shutdown(candidate);
        }
    }
    return papacc_pst_secure_scheduler_wake(&secure_io->scheduler);
}

PAPACC_BOOL papacc_server_secure_io_win32_is_stopped(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    if (secure_io == NULL || secure_io->initialized != PAPACC_TRUE ||
        secure_io->stop_requested != PAPACC_TRUE)
        return PAPACC_FALSE;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        if (secure_io->candidates[index].state !=
                PAPACC_SERVER_SECURE_CANDIDATE_UNUSED &&
            secure_io->candidates[index].state !=
                PAPACC_SERVER_SECURE_CANDIDATE_CLOSED)
            return PAPACC_FALSE;
        if (secure_io->candidates[index].connection_instance_id != 0U &&
            papacc_server_io_loop_win32_find_connection_slot(
                secure_io->protocol_loop,
                secure_io->candidates[index].connection_instance_id) != NULL)
            return PAPACC_FALSE;
    }
    if (secure_io->protocol_loop->connection_manager->count != 0U)
        return PAPACC_FALSE;
    return PAPACC_TRUE;
}

void papacc_server_secure_io_win32_shutdown(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    if (secure_io == NULL) return;
    if (secure_io->candidates != NULL) {
        for (index = 0U; index < secure_io->candidate_capacity; ++index) {
            if (secure_io->candidates[index].state !=
                    PAPACC_SERVER_SECURE_CANDIDATE_UNUSED &&
                secure_io->candidates[index].state !=
                    PAPACC_SERVER_SECURE_CANDIDATE_CLOSED)
                papacc_secure_candidate_close(&secure_io->candidates[index]);
        }
    }
    if (secure_io->listener_sources != NULL) {
        for (index = 0U; index < secure_io->listener_count; ++index) {
            if (secure_io->listener_sources[index] != NULL) {
                (void)papacc_pst_secure_scheduler_remove_external(
                    &secure_io->scheduler,
                    secure_io->listener_sources[index]);
                pst_external_source_release(
                    secure_io->listener_sources[index]);
                secure_io->listener_sources[index] = NULL;
            }
        }
    }
    papacc_pst_secure_scheduler_release(&secure_io->scheduler);
    *secure_io = (PAPACC_SERVER_SECURE_IO_WIN32)
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
}
