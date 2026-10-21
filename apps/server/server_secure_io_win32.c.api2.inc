#include "server_secure_io_win32.h"
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
#include "pst_test_only_socket_correlation_observer.h"
#include <stdio.h>
#endif

#include "pal_time.h"
#include <stddef.h>
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

typedef enum PAPACC_SECURE_REMOVE_ORIGIN {
    PAPACC_SECURE_REMOVE_ORIGIN_NORMAL = 0,
    PAPACC_SECURE_REMOVE_ORIGIN_STOP_REMOVE_ROUND
} PAPACC_SECURE_REMOVE_ORIGIN;

static PAPACC_BOOL papacc_secure_stop_retains_resources(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    return secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_REQUESTED ||
        secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING ||
        secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL ?
        PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_RESULT papacc_secure_stop_pending_result(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    return secure_io->stop_last_remove_result == PAPACC_RESULT_OK ?
        PAPACC_RESULT_INVALID_STATE : secure_io->stop_last_remove_result;
}

static PAPACC_RESULT papacc_secure_owner_remove_connection(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,pst_connection *connection,
    PAPACC_SECURE_REMOVE_ORIGIN origin)
{
    PAPACC_RESULT result;
    if (secure_io->owner_thread_id != (PAPACC_U32)GetCurrentThreadId() ||
        secure_io->dispatch_active == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (origin == PAPACC_SECURE_REMOVE_ORIGIN_NORMAL &&
        papacc_secure_stop_retains_resources(secure_io) == PAPACC_TRUE)
        return papacc_secure_stop_pending_result(secure_io);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *observation;
    PAPACC_BOOL target_selected;
    if(secure_io!=NULL&&connection!=NULL&&
        secure_io->test_remove_selected_connection!=NULL){
        observation=&secure_io->test_remove_observation;
        target_selected=secure_io->test_remove_selected_connection==connection?
            PAPACC_TRUE:PAPACC_FALSE;
        ++observation->remove_attempt_count;
        if(origin==PAPACC_SECURE_REMOVE_ORIGIN_STOP_REMOVE_ROUND){
            ++observation->stop_remove_attempt_count;
            if(target_selected==PAPACC_TRUE)
                ++observation->target_stop_remove_attempt_count;
            else
                ++observation->other_stop_remove_attempt_count;
        }else
            ++observation->normal_remove_attempt_count;
        if(target_selected==PAPACC_TRUE){
            observation->owner_thread_id=(PAPACC_U32)GetCurrentThreadId();
            if(secure_io->dispatch_active==PAPACC_TRUE)
                observation->dispatch_active_at_attempt=PAPACC_TRUE;
        }
        if(target_selected==PAPACC_TRUE&&
           origin==PAPACC_SECURE_REMOVE_ORIGIN_STOP_REMOVE_ROUND&&
           (secure_io->test_remove_fault_mode==
                PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE||
            secure_io->test_remove_fault_mode==
                PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT)){
            ++observation->injected_failure_count;
            ++observation->target_injected_failure_count;
            if(secure_io->test_remove_fault_mode==
                    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE)
                secure_io->test_remove_fault_mode=
                    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF;
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        ++observation->real_remove_call_count;
        if(target_selected==PAPACC_TRUE&&
           origin==PAPACC_SECURE_REMOVE_ORIGIN_STOP_REMOVE_ROUND)
            ++observation->target_real_remove_call_count;
        result=papacc_pst_secure_scheduler_remove_connection(
            &secure_io->scheduler,connection);
        if(result==PAPACC_RESULT_OK){
            ++observation->real_remove_success_count;
            if(origin==PAPACC_SECURE_REMOVE_ORIGIN_NORMAL)
                ++observation->normal_remove_success_count;
            else if(target_selected==PAPACC_TRUE)
                ++observation->target_real_remove_success_count;
        }
        return result;
    }
#endif
    result=papacc_pst_secure_scheduler_remove_connection(
        &secure_io->scheduler,connection);
    return result;
}

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
PAPACC_RESULT papacc_server_secure_io_win32_test_set_remove_fault(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,pst_connection *connection,
    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_MODE mode,
    PAPACC_U64 connection_instance_id,PAPACC_U64 operation_id,
    PAPACC_U64 stream_generation)
{
    if(secure_io==NULL||secure_io->initialized!=PAPACC_TRUE||
        secure_io->owner_thread_id!=(PAPACC_U32)GetCurrentThreadId()||
        mode>PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if(mode!=PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF&&
        (connection==NULL||connection_instance_id==0U||operation_id==0U||
         stream_generation==0U))return PAPACC_RESULT_INVALID_ARGUMENT;
    secure_io->test_remove_selected_connection=connection;
    secure_io->test_remove_fault_mode=mode;
    memset(&secure_io->test_remove_observation,0,
        sizeof(secure_io->test_remove_observation));
    secure_io->test_remove_observation.connection_instance_id=
        connection_instance_id;
    secure_io->test_remove_observation.operation_id=operation_id;
    secure_io->test_remove_observation.stream_generation=stream_generation;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_secure_io_win32_test_get_remove_observation(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *out_observation)
{
    if(secure_io==NULL||out_observation==NULL||
        secure_io->initialized!=PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *out_observation=secure_io->test_remove_observation;
    return PAPACC_RESULT_OK;
}
#endif
static PAPACC_RESULT papacc_secure_candidate_try_close(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate,
    PAPACC_BOOL *out_closed)
{
    PAPACC_CONNECTION *connection;
    PAPACC_BOOL connection_closed = PAPACC_FALSE;
    PAPACC_BOOL candidate_owned = PAPACC_FALSE;
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (out_closed != NULL) *out_closed = PAPACC_FALSE;
    if (candidate == NULL || out_closed == NULL || candidate->owner == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (candidate->owner->owner_thread_id !=
            (PAPACC_U32)GetCurrentThreadId())
        return PAPACC_RESULT_INVALID_STATE;
    for (index = 0U; index < candidate->owner->candidate_capacity; ++index)
        if (&candidate->owner->candidates[index] == candidate)
            candidate_owned = PAPACC_TRUE;
    if (candidate_owned != PAPACC_TRUE ||
        (candidate->state == PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL &&
         candidate->connection_instance_id == 0U))
        return PAPACC_RESULT_INVALID_STATE;
    if (candidate->state == PAPACC_SERVER_SECURE_CANDIDATE_CLOSED) {
        candidate->close_requested = PAPACC_FALSE;
        *out_closed = PAPACC_TRUE;
        return PAPACC_RESULT_OK;
    }
    candidate->close_requested = PAPACC_TRUE;
    if (papacc_secure_stop_retains_resources(candidate->owner) == PAPACC_TRUE)
        return papacc_secure_stop_pending_result(candidate->owner);
    if (candidate->owner->dispatch_active == PAPACC_TRUE)
        return PAPACC_RESULT_OK;
    if (candidate->connection_instance_id != 0U) {
        connection = papacc_connection_manager_find(
            candidate->owner->protocol_loop->connection_manager,
            candidate->connection_instance_id);
        if (connection != NULL) {
            /* Numeric identity does not transfer authority between candidates.
             * A live transport must still belong to this exact adapter. */
            if (connection->state != PAPACC_CONNECTION_STATE_CLOSED &&
                connection->transport.context != &candidate->transport_adapter)
                return PAPACC_RESULT_INVALID_STATE;
            result = papacc_connection_try_close(
                connection, &connection_closed);
            if (result != PAPACC_RESULT_OK ||
                connection_closed != PAPACC_TRUE) {
                candidate->owner->stop_last_remove_result = result;
                return result;
            }
            return PAPACC_RESULT_OK;
        } else if (candidate->transport_adapter.closed != PAPACC_TRUE ||
                   candidate->transport_adapter.registered == PAPACC_TRUE ||
                   candidate->secure_processor.connection != NULL) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    } else if (candidate->secure_processor.connection != NULL) {
        result = papacc_secure_owner_remove_connection(candidate->owner,
            candidate->secure_processor.connection,
            PAPACC_SECURE_REMOVE_ORIGIN_NORMAL);
        if (result != PAPACC_RESULT_OK) {
            candidate->owner->stop_last_remove_result = result;
            return result;
        }
        papacc_pst_secure_processor_release(&candidate->secure_processor);
    }
    /* Close intent and a consumed transport do not revoke a live Protocol
     * Slot. ADR-0031 requires factual Slot absence before candidate reuse. */
    if (candidate->connection_instance_id != 0U &&
        papacc_server_io_loop_win32_find_connection_slot(
            candidate->owner->protocol_loop,
            candidate->connection_instance_id) != NULL)
        return PAPACC_RESULT_OK;
    papacc_connection_security_context_release(
        &candidate->connection_security);
    papacc_session_security_context_release(&candidate->session_security);
    candidate->state = PAPACC_SERVER_SECURE_CANDIDATE_CLOSED;
    candidate->close_requested = PAPACC_FALSE;
    *out_closed = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

static void papacc_secure_candidate_close(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
    PAPACC_BOOL closed;
    PAPACC_RESULT result = papacc_secure_candidate_try_close(
        candidate, &closed);

    if (result != PAPACC_RESULT_OK && candidate != NULL &&
        candidate->owner != NULL)
        candidate->owner->stop_last_remove_result = result;
}

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
PAPACC_RESULT papacc_server_secure_io_win32_test_candidate_close(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate, PAPACC_BOOL *out_closed)
{
    return papacc_secure_candidate_try_close(candidate, out_closed);
}
#endif

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

static PAPACC_RESULT papacc_secure_progress_stop_stream_close(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;

    if (secure_io->stop_state != PAPACC_SERVER_SECURE_STOP_DRAINING)
        return PAPACC_RESULT_OK;
    for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY; ++index) {
        PAPACC_NETWORK_STREAM *stream =
            &secure_io->protocol_loop->network_stream_storage[index];
        PAPACC_RESULT result;

        if (stream->state != PAPACC_NETWORK_STREAM_OPEN ||
            stream->transport_mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)
            continue;
        if (stream->remote_half_close_signalled == PAPACC_TRUE ||
            stream->terminal_pending == PAPACC_TRUE) {
            result = papacc_network_stream_close_channel(
                &secure_io->protocol_loop->network_stream_manager,
                stream->data_channel_instance_id);
        } else {
            result = papacc_network_stream_request_close(
                stream, secure_io->shutdown_deadline_ns);
        }
        if (result != PAPACC_RESULT_OK) return result;
    }
    return PAPACC_RESULT_OK;
}

static void papacc_secure_advance_stop(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    PAPACC_BOOL non_control_active = PAPACC_FALSE;
    if (secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_RUNNING) return;
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

/* Adapter is embedded in its owning candidate; retain the existing transport
 * context and ABI while checking owner authority at the pre-close root. */
static PAPACC_RESULT papacc_secure_candidate_pre_close(
    void *context, PAPACC_BOOL *out_ready)
{
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter =
        (PAPACC_PST_SECURE_TRANSPORT_ADAPTER *)context;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate;
    PAPACC_SERVER_SECURE_IO_WIN32 *owner;
    PAPACC_RESULT result;
    if (out_ready == NULL || adapter == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *out_ready = PAPACC_FALSE;
    candidate = (PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *)((char *)adapter -
        offsetof(PAPACC_SERVER_SECURE_CANDIDATE_WIN32, transport_adapter));
    owner = candidate->owner;
    if (owner == NULL || owner->owner_thread_id !=
            (PAPACC_U32)GetCurrentThreadId() || owner->dispatch_active ==
            PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (papacc_secure_stop_retains_resources(owner) == PAPACC_TRUE)
        return papacc_secure_stop_pending_result(owner);
    result = papacc_pst_secure_transport_adapter_prepare_close(adapter);
    if (result == PAPACC_RESULT_OK) *out_ready = PAPACC_TRUE;
    return result;
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
    transport.pre_close_fn = papacc_secure_candidate_pre_close;
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

#define PAPACC_SECURE_PROTOCOL_DRAIN_STEP_BOUND 8U

static PAPACC_RESULT papacc_secure_control_close(
    void *context, const PAPACC_CONTROL_CLOSE_RESULT *close_result)
{
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
        (PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *)context;
    PAPACC_NETWORK_STREAM_TERMINAL_CAUSE stream_cause =
        PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE;
    PAPACC_RESULT result;
    if (candidate == NULL || close_result == NULL ||
        candidate->owner == NULL || candidate->control_cascade_count != 0U)
        return PAPACC_RESULT_INVALID_STATE;
    if (close_result->origin ==
            PAPACC_CONTROL_CLOSE_ORIGIN_PACC_PROTOCOL_FAILURE)
        stream_cause =
            PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_PACC_PROTOCOL_FAILURE;
    else if (close_result->origin ==
            PAPACC_CONTROL_CLOSE_ORIGIN_OWNER_FAILURE) {
        PST_RESULT source = candidate->secure_processor.source_result;
        if (source == PST_RESULT_TRANSPORT_FAILURE ||
            source == PST_RESULT_TRUNCATED)
            stream_cause =
                PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_CONTROL_TRANSPORT_LOSS;
        else if (source == PST_RESULT_PROTOCOL_FAILURE ||
                 source == PST_RESULT_AUTH_FAILURE ||
                 source == PST_RESULT_PEER_NAME_MISMATCH ||
                 source == PST_RESULT_POLICY_VIOLATION ||
                 source == PST_RESULT_BACKEND_FAILURE)
            stream_cause =
                PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_CONTROL_TLS_FAILURE;
        else
            return PAPACC_RESULT_INVALID_STATE;
    } else if (close_result->origin != PAPACC_CONTROL_CLOSE_ORIGIN_NORMAL)
        return PAPACC_RESULT_INVALID_STATE;
    candidate->control_close_result = *close_result;
    candidate->control_stream_cause = stream_cause;
    result = papacc_network_stream_close_session_with_cause(
        &candidate->owner->protocol_loop->network_stream_manager,
        close_result->session_instance_id, stream_cause);
    if (result != PAPACC_RESULT_OK) return result;
    (void)papacc_data_association_manager_invalidate_session(
        &candidate->owner->protocol_loop->association_manager,
        close_result->session_instance_id);
    result = papacc_channel_manager_close(
        &candidate->owner->protocol_loop->channel_manager,
        close_result->control_channel_instance_id);
    if (result == PAPACC_RESULT_OK) candidate->control_cascade_count = 1U;
    return result;
}

static PAPACC_RESULT papacc_secure_after_protocol_step(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate)
{
    PAPACC_SIZE step;
    for (step = 0U; step < PAPACC_SECURE_PROTOCOL_DRAIN_STEP_BOUND; ++step) {
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot =
            papacc_server_io_loop_win32_find_connection_slot(
                candidate->owner->protocol_loop,
                candidate->connection_instance_id);
        PAPACC_AUTHORIZATION_DECISION decision = PAPACC_AUTHORIZATION_DENY;
        PAPACC_BOOL read_interest;
        PAPACC_BOOL write_interest;
        PAPACC_BOOL buffered_input;
        PAPACC_SIZE slot_index;
        PAPACC_RESULT result;
        if (slot == NULL) return PAPACC_RESULT_INVALID_STATE;
        if (slot->kind ==
                PAPACC_SERVER_PROTOCOL_SLOT_KIND_CONTROL_ESTABLISHMENT &&
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
            candidate->control_close_handler_installed != PAPACC_TRUE) {
            result = papacc_post_control_processor_set_close_handler(
                &slot->post_control_processor, papacc_secure_control_close,
                candidate);
            if (result != PAPACC_RESULT_OK) return result;
            candidate->control_close_handler_installed = PAPACC_TRUE;
        }
        if (slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL &&
            candidate->issuance_gate_installed != PAPACC_TRUE) {
            result = papacc_session_security_context_publish(
                &candidate->session_security,
                &candidate->connection_security);
            if (result != PAPACC_RESULT_OK) return result;
            result = papacc_post_control_processor_set_ticket_issue_gate(
                &slot->post_control_processor, papacc_secure_issue_gate,
                candidate);
            if (result != PAPACC_RESULT_OK) return result;
            candidate->issuance_gate_installed = PAPACC_TRUE;
        }
        if (slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH &&
            candidate->data_gate_installed != PAPACC_TRUE) {
            result = papacc_data_attach_processor_set_commit_gate(
                &slot->data_attach_processor, papacc_secure_data_gate,
                candidate);
            if (result != PAPACC_RESULT_OK) return result;
            candidate->data_gate_installed = PAPACC_TRUE;
        }
        slot_index = (PAPACC_SIZE)(slot -
            candidate->owner->protocol_loop->processor_slots);
        result = papacc_server_io_loop_win32_processor_interest(
            candidate->owner->protocol_loop, slot_index,
            &read_interest, &write_interest);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_server_io_loop_win32_processor_has_buffered_input(
            candidate->owner->protocol_loop, slot_index, &buffered_input);
        if (result != PAPACC_RESULT_OK) return result;
        if (write_interest == PAPACC_TRUE) {
            result = papacc_server_io_loop_win32_process_connection_once(
                candidate->owner->protocol_loop,
                candidate->connection_instance_id,
                PAPACC_FALSE, PAPACC_TRUE, 0U);
            if (result != PAPACC_RESULT_OK) return result;
            slot = papacc_server_io_loop_win32_find_connection_slot(
                candidate->owner->protocol_loop,
                candidate->connection_instance_id);
            if (slot == NULL) return PAPACC_RESULT_INVALID_STATE;
        } else if (read_interest == PAPACC_TRUE &&
                   buffered_input == PAPACC_TRUE) {
            result = papacc_server_io_loop_win32_process_connection_once(
                candidate->owner->protocol_loop,
                candidate->connection_instance_id,
                PAPACC_TRUE, PAPACC_FALSE, 0U);
        } else {
            return PAPACC_RESULT_OK;
        }
        if (result != PAPACC_RESULT_OK) return result;
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
    PAPACC_BOOL outbound_ready;
} PAPACC_SECURE_DISPATCH_CONTEXT;

static PAPACC_BOOL papacc_secure_is_outbound_token(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, pst_wait_token token)
{
    PAPACC_SIZE index;
    for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY; ++index) {
        PAPACC_SERVER_OUTBOUND_READY_WIN32 *ready =
            &secure_io->outbound_ready[index];
        if (ready->connection != NULL && ready->wait_token == token)
            return PAPACC_TRUE;
    }
    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_secure_unregister_outbound(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_BOOL preserve_identity,
    PAPACC_SECURE_REMOVE_ORIGIN origin)
{
    PAPACC_SIZE index;
    for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY; ++index) {
        PAPACC_SERVER_OUTBOUND_READY_WIN32 *ready =
            &secure_io->outbound_ready[index];
        if (ready->registered == PAPACC_TRUE) {
            PAPACC_RESULT result = papacc_secure_owner_remove_connection(
                secure_io, ready->connection, origin);
            if (result != PAPACC_RESULT_OK) return result;
            ready->registered = PAPACC_FALSE;
            if (preserve_identity != PAPACC_TRUE)
                *ready = (PAPACC_SERVER_OUTBOUND_READY_WIN32)
                    PAPACC_SERVER_OUTBOUND_READY_WIN32_INITIALIZER;
        }
    }
    return PAPACC_RESULT_OK;
}

static void papacc_secure_clear_outbound_identity(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY; ++index)
        if (secure_io->outbound_ready[index].registered != PAPACC_TRUE)
            secure_io->outbound_ready[index] =
                (PAPACC_SERVER_OUTBOUND_READY_WIN32)
                    PAPACC_SERVER_OUTBOUND_READY_WIN32_INITIALIZER;
}

static PAPACC_RESULT papacc_secure_register_outbound(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    if (secure_io->stop_state != PAPACC_SERVER_SECURE_STOP_RUNNING) return PAPACC_RESULT_OK;
    for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY; ++index) {
        PAPACC_NETWORK_STREAM *stream =
            &secure_io->protocol_loop->network_stream_storage[index];
        PAPACC_SERVER_OUTBOUND_READY_WIN32 *ready =
            &secure_io->outbound_ready[index];
        PAPACC_OUTBOUND_TLS *tls;
        PAPACC_RESULT result;
        if (ready->registered == PAPACC_TRUE) continue;
        if ((stream->state != PAPACC_NETWORK_STREAM_OPEN &&
             stream->state != PAPACC_NETWORK_STREAM_CLOSING) ||
            stream->transport_mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED ||
            stream->io.context == NULL)
            continue;
        tls = (PAPACC_OUTBOUND_TLS *)stream->io.context;
        if (tls->processor.connection == NULL) continue;
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
        {
            char mode[32];DWORD mode_size=GetEnvironmentVariableA(
                "PAPACC_ADR0032_P10",mode,(DWORD)sizeof(mode));
            if((mode_size==6U&&memcmp(mode,"normal",6U)==0)||
               (mode_size==4U&&memcmp(mode,"once",4U)==0)||
               (mode_size==10U&&memcmp(mode,"persistent",10U)==0)||
               (mode_size==11U&&memcmp(mode,"normal-once",11U)==0)||
               (mode_size==17U&&memcmp(mode,"normal-persistent",17U)==0)){
                PAPACC_SIZE slot_index;PAPACC_U64 connection_id=0U;
                LARGE_INTEGER counter;pst_test_correlation_token token;
                for(slot_index=0U;
                    slot_index<secure_io->protocol_loop->processor_capacity;
                    ++slot_index){
                    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot=
                        &secure_io->protocol_loop->processor_slots[slot_index];
                    if(slot->in_use==PAPACC_TRUE&&
                       slot->network_data_protocol.channel_instance_id==
                            stream->data_channel_instance_id){
                        connection_id=slot->connection_instance_id;break;
                    }
                }
                if(connection_id==0U)return PAPACC_RESULT_INTERNAL_ERROR;
                if(tls->test_correlation_associated==PAPACC_TRUE){
                    if(tls->test_correlation_connection!=
                            tls->processor.connection)
                        return PAPACC_RESULT_INTERNAL_ERROR;
                    ++tls->test_correlation_waitset_reregistrations;
                    (void)fprintf(stderr,
                        "P10_CORRELATION_REUSED TOKEN=%08lX%08lX CONNECTION_INSTANCE_ID=%llu OPERATION_ID=%llu STREAM_GENERATION=%llu ASSOCIATION_ATTEMPTS=%lu ASSOCIATION_SUCCESSES=%lu WAITSET_REREGISTRATIONS=%lu\n",
                        (unsigned long)tls->test_correlation_token_high,
                        (unsigned long)tls->test_correlation_token_low,
                        (unsigned long long)connection_id,
                        (unsigned long long)stream->data_channel_instance_id,
                        (unsigned long long)stream->stream_instance_id,
                        (unsigned long)tls->test_correlation_association_attempts,
                        (unsigned long)tls->test_correlation_association_successes,
                        (unsigned long)tls->test_correlation_waitset_reregistrations);
                    (void)fflush(stderr);
                }else{
                    if(QueryPerformanceCounter(&counter)==0)
                        return PAPACC_RESULT_INTERNAL_ERROR;
                    token.high=(pst_u32)GetCurrentProcessId()^
                        (pst_u32)(((PAPACC_U64)counter.QuadPart)>>32);
                    token.low=(pst_u32)counter.QuadPart^
                        (pst_u32)connection_id^
                        (pst_u32)stream->data_channel_instance_id^
                        (pst_u32)stream->stream_instance_id;
                    if((token.high|token.low)==0U)token.low=1U;
                    ++tls->test_correlation_association_attempts;
                    if(pst_test_only_correlation_associate(
                            tls->processor.connection,token,
                            tls->test_local_endpoint,
                            tls->test_remote_endpoint)!=PST_RESULT_OK)
                        return PAPACC_RESULT_INTERNAL_ERROR;
                    tls->test_correlation_connection=
                        tls->processor.connection;
                    tls->test_correlation_token_high=(PAPACC_U32)token.high;
                    tls->test_correlation_token_low=(PAPACC_U32)token.low;
                    ++tls->test_correlation_association_successes;
                    tls->test_correlation_associated=PAPACC_TRUE;
                    (void)fprintf(stderr,
                        "P10_CORRELATION_ASSOCIATED TOKEN=%08lX%08lX CONNECTION_INSTANCE_ID=%llu OPERATION_ID=%llu STREAM_GENERATION=%llu OWNER_THREAD=%lu LOCAL_ENDPOINT=%s REMOTE_ENDPOINT=%s ASSOCIATION_ATTEMPTS=%lu ASSOCIATION_SUCCESSES=%lu WAITSET_REREGISTRATIONS=%lu\n",
                        (unsigned long)token.high,(unsigned long)token.low,
                        (unsigned long long)connection_id,
                        (unsigned long long)stream->data_channel_instance_id,
                        (unsigned long long)stream->stream_instance_id,
                        (unsigned long)GetCurrentThreadId(),
                        tls->test_local_endpoint,tls->test_remote_endpoint,
                        (unsigned long)tls->test_correlation_association_attempts,
                        (unsigned long)tls->test_correlation_association_successes,
                        (unsigned long)tls->test_correlation_waitset_reregistrations);
                    (void)fflush(stderr);
                }
            }
        }
#endif
        result = papacc_pst_secure_scheduler_add_connection(
            &secure_io->scheduler, tls->processor.connection,
            &ready->wait_token);
        if (result != PAPACC_RESULT_OK) {
            (void)papacc_secure_unregister_outbound(
                secure_io, PAPACC_FALSE, PAPACC_SECURE_REMOVE_ORIGIN_NORMAL);
            return result;
        }
        ready->connection = tls->processor.connection;
        ready->stream_instance_id = stream->stream_instance_id;
        ready->registered = PAPACC_TRUE;
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_secure_drive_pending_protocol(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U64 now_ns)
{
    PAPACC_SIZE index;
    (void)now_ns;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot;
        PAPACC_RESULT result;
        if (candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL)
            continue;
        slot = papacc_server_io_loop_win32_find_connection_slot(
            secure_io->protocol_loop, candidate->connection_instance_id);
        if (slot == NULL) continue;
        result = papacc_secure_after_protocol_step(candidate);
        if (result != PAPACC_RESULT_OK) {
            papacc_secure_candidate_close(candidate);
            continue;
        }
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_BOOL papacc_secure_has_pending_protocol(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot;
        PAPACC_BOOL read_interest;
        PAPACC_BOOL write_interest;
        PAPACC_BOOL buffered_input;
        PAPACC_SIZE slot_index;
        if (candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL)
            continue;
        slot = papacc_server_io_loop_win32_find_connection_slot(
            secure_io->protocol_loop, candidate->connection_instance_id);
        if (slot == NULL) continue;
        slot_index = (PAPACC_SIZE)(slot -
            secure_io->protocol_loop->processor_slots);
        if (papacc_server_io_loop_win32_processor_interest(
                secure_io->protocol_loop, slot_index,
                &read_interest, &write_interest) != PAPACC_RESULT_OK ||
            papacc_server_io_loop_win32_processor_has_buffered_input(
                secure_io->protocol_loop, slot_index,
                &buffered_input) != PAPACC_RESULT_OK)
            continue;
        if (read_interest == PAPACC_TRUE && buffered_input == PAPACC_TRUE)
            return PAPACC_TRUE;
    }
    return PAPACC_FALSE;
}

static void papacc_secure_dispatch(void *context,
    const PAPACC_PST_READY_EVENT *event)
{
    PAPACC_SECURE_DISPATCH_CONTEXT *dispatch =
        (PAPACC_SECURE_DISPATCH_CONTEXT *)context;
    PAPACC_SIZE index;
    if (dispatch->result != PAPACC_RESULT_OK) return;
    if (papacc_secure_stop_retains_resources(dispatch->secure_io) == PAPACC_TRUE)
        return;
    if (event->kind == PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL) {
        if (dispatch->secure_io->stop_state != PAPACC_SERVER_SECURE_STOP_RUNNING) return;
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
            if (papacc_secure_is_outbound_token(dispatch->secure_io,
                    event->token) == PAPACC_TRUE) {
                dispatch->outbound_ready = PAPACC_TRUE;
                return;
            }
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

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
/* Diagnostic-only probe of the existing owner boundaries, no production API. */
PAPACC_RESULT papacc_server_secure_io_win32_test_probe_stop_retention(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, pst_connection *connection,
    pst_wait_token token, PAPACC_U64 connection_instance_id)
{
    PAPACC_SECURE_DISPATCH_CONTEXT dispatch;
    PAPACC_PST_READY_EVENT event;
    PAPACC_SIZE index;
    PAPACC_RESULT expected;
    if (secure_io == NULL || connection == NULL ||
        papacc_secure_stop_retains_resources(secure_io) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    expected = papacc_secure_stop_pending_result(secure_io);
    if (papacc_secure_owner_remove_connection(secure_io, connection,
            PAPACC_SECURE_REMOVE_ORIGIN_NORMAL) != expected)
        return PAPACC_RESULT_INTERNAL_ERROR;
    memset(&dispatch, 0, sizeof(dispatch));
    memset(&event, 0, sizeof(event));
    dispatch.secure_io = secure_io;
    dispatch.result = PAPACC_RESULT_OK;
    event.token = token;
    event.kind = PAPACC_PST_SCHEDULER_MEMBER_CONNECTION;
    event.terminal = PAPACC_TRUE;
    papacc_secure_dispatch(&dispatch, &event);
    if (dispatch.result != PAPACC_RESULT_OK ||
        dispatch.outbound_ready == PAPACC_TRUE)
        return PAPACC_RESULT_INTERNAL_ERROR;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        if (candidate->connection_instance_id == connection_instance_id) {
            PAPACC_BOOL closed = PAPACC_FALSE;
            event.token = candidate->wait_token;
            papacc_secure_dispatch(&dispatch, &event);
            if (papacc_secure_candidate_try_close(candidate, &closed) !=
                    expected || closed == PAPACC_TRUE ||
                dispatch.result != PAPACC_RESULT_OK)
                return PAPACC_RESULT_INTERNAL_ERROR;
            return PAPACC_RESULT_OK;
        }
    }
    return PAPACC_RESULT_INVALID_STATE;
}
#endif

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
        scheduler_capacity < candidate_capacity + network->listener_set.count +
            PAPACC_SERVER_NETWORK_STREAM_CAPACITY ||
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
    secure_io->stop_retry_not_before_ns = 0U;
    secure_io->stop_first_pending_stream_instance_id = 0U;
    secure_io->stop_remove_attempt_count = 0U;
    secure_io->owner_thread_id = (PAPACC_U32)GetCurrentThreadId();
    secure_io->stop_last_remove_result = PAPACC_RESULT_OK;
    secure_io->stop_cause = PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE;
    secure_io->stop_state = PAPACC_SERVER_SECURE_STOP_RUNNING;
    secure_io->dispatch_active = PAPACC_FALSE;
    for (index = 0U; index < candidate_capacity; ++index)
        candidates[index] = (PAPACC_SERVER_SECURE_CANDIDATE_WIN32)
            PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER;
    for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY; ++index)
        secure_io->outbound_ready[index] =
            (PAPACC_SERVER_OUTBOUND_READY_WIN32)
                PAPACC_SERVER_OUTBOUND_READY_WIN32_INITIALIZER;
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

#define PAPACC_SECURE_STOP_RETRY_NS 10000000ULL

static PAPACC_BOOL papacc_secure_has_registered_outbound(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY; ++index)
        if (secure_io->outbound_ready[index].registered == PAPACC_TRUE)
            return PAPACC_TRUE;
    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_secure_reconcile_close_requests(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;

    if (secure_io->dispatch_active == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        PAPACC_BOOL closed;
        PAPACC_RESULT result;
        PAPACC_CONNECTION *connection = papacc_connection_manager_find(
            secure_io->protocol_loop->connection_manager,
            candidate->connection_instance_id);

        if (connection != NULL &&
            connection->state == PAPACC_CONNECTION_STATE_CLOSING)
            candidate->close_requested = PAPACC_TRUE;
        if (candidate->close_requested != PAPACC_TRUE &&
            candidate->connection_instance_id != 0U &&
            papacc_connection_manager_find(
                secure_io->protocol_loop->connection_manager,
                candidate->connection_instance_id) == NULL &&
            papacc_server_io_loop_win32_find_connection_slot(
                secure_io->protocol_loop,
                candidate->connection_instance_id) == NULL &&
            candidate->transport_adapter.registered != PAPACC_TRUE &&
            candidate->transport_adapter.closed == PAPACC_TRUE &&
            candidate->secure_processor.connection == NULL)
            candidate->close_requested = PAPACC_TRUE;
        if (candidate->close_requested != PAPACC_TRUE) continue;
        result = papacc_secure_candidate_try_close(candidate, &closed);
        if (result != PAPACC_RESULT_OK) return result;
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_secure_stop_remove_round(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U64 now_ns)
{
    PAPACC_SIZE index;
    PAPACC_RESULT result;
    if (secure_io->dispatch_active == PAPACC_TRUE) {
        secure_io->stop_state = PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING;
        secure_io->stop_last_remove_result = PAPACC_RESULT_INVALID_STATE;
        return PAPACC_RESULT_INVALID_STATE;
    }
    ++secure_io->stop_remove_attempt_count;
    result = papacc_secure_unregister_outbound(secure_io, PAPACC_TRUE,
        PAPACC_SECURE_REMOVE_ORIGIN_STOP_REMOVE_ROUND);
    if (result == PAPACC_RESULT_OK) {
        for (index = 0U; index < secure_io->listener_count; ++index) {
            if (secure_io->listener_sources[index] == NULL) continue;
            result = papacc_pst_secure_scheduler_remove_external(
                &secure_io->scheduler, secure_io->listener_sources[index]);
            if (result != PAPACC_RESULT_OK) break;
            pst_external_source_release(secure_io->listener_sources[index]);
            secure_io->listener_sources[index] = NULL;
        }
    }
    if (result != PAPACC_RESULT_OK) {
        secure_io->stop_state = now_ns >= secure_io->shutdown_deadline_ns ?
            PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL :
            PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING;
        secure_io->stop_last_remove_result = result;
        secure_io->stop_retry_not_before_ns = papacc_secure_deadline(
            now_ns, PAPACC_SECURE_STOP_RETRY_NS);
        secure_io->stop_first_pending_stream_instance_id = 0U;
        for (index = 0U; index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY;
             ++index) {
            if (secure_io->outbound_ready[index].registered == PAPACC_TRUE) {
                secure_io->stop_first_pending_stream_instance_id =
                    secure_io->outbound_ready[index].stream_instance_id;
                break;
            }
        }
        return result;
    }
    papacc_secure_clear_outbound_identity(secure_io);
    secure_io->stop_first_pending_stream_instance_id = 0U;
    secure_io->stop_last_remove_result = PAPACC_RESULT_OK;
    secure_io->stop_state = PAPACC_SERVER_SECURE_STOP_DRAINING;
    result = papacc_secure_progress_stop_stream_close(secure_io);
    if (result != PAPACC_RESULT_OK) return result;
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        if (candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_UNUSED &&
            candidate->state != PAPACC_SERVER_SECURE_CANDIDATE_CLOSED &&
            papacc_secure_candidate_is_control(candidate) != PAPACC_TRUE)
            papacc_secure_begin_candidate_shutdown(candidate);
    }
    return PAPACC_RESULT_OK;
}

static void papacc_secure_stop_update(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    if (secure_io->stop_state != PAPACC_SERVER_SECURE_STOP_DRAINING)
        return;
    if (papacc_secure_has_registered_outbound(secure_io) == PAPACC_TRUE ||
        secure_io->scheduler.member_count != 0U ||
        secure_io->protocol_loop->connection_manager->count != 0U ||
        secure_io->protocol_loop->session_manager.count != 0U ||
        secure_io->protocol_loop->channel_manager.count != 0U ||
        secure_io->protocol_loop->network_job_manager.count != 0U ||
        secure_io->protocol_loop->network_stream_manager.count != 0U)
        return;
    for (index = 0U; index < secure_io->candidate_capacity; ++index)
        if (secure_io->candidates[index].state !=
                PAPACC_SERVER_SECURE_CANDIDATE_UNUSED &&
            secure_io->candidates[index].state !=
                PAPACC_SERVER_SECURE_CANDIDATE_CLOSED)
            return;
    secure_io->stop_state = PAPACC_SERVER_SECURE_STOP_STOPPED;
}
PAPACC_RESULT papacc_server_secure_io_win32_poll_once(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U32 timeout_ms)
{
    PAPACC_PST_WAIT_OUTCOME outcome;
    PAPACC_SIZE ready_count, dispatched;
    PAPACC_SECURE_DISPATCH_CONTEXT dispatch;
    PAPACC_RESULT result;
    PAPACC_U64 now_ns;
    if (secure_io == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    secure_io->test_last_non_ok_function = NULL;
#endif
    if (secure_io->initialized != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (secure_io->owner_thread_id != (PAPACC_U32)GetCurrentThreadId())
        return PAPACC_RESULT_INVALID_STATE;
    if (papacc_pal_monotonic_time_ns(&now_ns) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INTERNAL_ERROR;
    if (secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL)
        return secure_io->stop_last_remove_result == PAPACC_RESULT_OK ?
            PAPACC_RESULT_INTERNAL_ERROR : secure_io->stop_last_remove_result;
    if (secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING &&
        now_ns >= secure_io->shutdown_deadline_ns) {
        secure_io->stop_state = PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL;
        return secure_io->stop_last_remove_result == PAPACC_RESULT_OK ?
            PAPACC_RESULT_INTERNAL_ERROR : secure_io->stop_last_remove_result;
    }
    if ((secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_REQUESTED ||
         secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING) &&
        now_ns >= secure_io->stop_retry_not_before_ns) {
        result = papacc_secure_stop_remove_round(secure_io, now_ns);
        if (result != PAPACC_RESULT_OK) {
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            secure_io->test_last_non_ok_function =
                "papacc_secure_stop_remove_round";
#endif
            return result;
        }
    }
    if (papacc_secure_stop_retains_resources(secure_io) == PAPACC_TRUE) {
        PAPACC_U64 wait_until = secure_io->stop_retry_not_before_ns;
        PAPACC_U64 remaining_ns;
        PAPACC_U32 wait_ms;
        if (wait_until > secure_io->shutdown_deadline_ns)
            wait_until = secure_io->shutdown_deadline_ns;
        remaining_ns = wait_until > now_ns ? wait_until - now_ns : 0U;
        wait_ms = remaining_ns / 1000000U >= timeout_ms ? timeout_ms :
            (PAPACC_U32)(remaining_ns / 1000000U +
                (remaining_ns % 1000000U != 0U ? 1U : 0U));
        if (wait_ms != 0U) Sleep(wait_ms);
        return papacc_secure_stop_pending_result(secure_io);
    }
    result = papacc_secure_unregister_outbound(secure_io, PAPACC_FALSE,
        PAPACC_SECURE_REMOVE_ORIGIN_NORMAL);
    if (result != PAPACC_RESULT_OK) {
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        secure_io->test_last_non_ok_function =
            "papacc_secure_unregister_outbound";
#endif
        return result;
    }
    result = papacc_secure_drive_pending_protocol(secure_io, 0U);
    if (result != PAPACC_RESULT_OK) {
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        secure_io->test_last_non_ok_function =
            "papacc_secure_drive_pending_protocol_pre_wait";
#endif
        return result;
    }
    result = papacc_secure_register_outbound(secure_io);
    if (result != PAPACC_RESULT_OK) return result;
    if (papacc_secure_has_pending_protocol(secure_io) == PAPACC_TRUE)
        timeout_ms = 0U;
    result = papacc_pst_secure_scheduler_wait(&secure_io->scheduler,
        timeout_ms, &outcome, &ready_count);
    if (result != PAPACC_RESULT_OK) {
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        secure_io->test_last_non_ok_function =
            "papacc_pst_secure_scheduler_wait";
#endif
        return result;
    }
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
        result = papacc_secure_unregister_outbound(
            secure_io, PAPACC_FALSE, PAPACC_SECURE_REMOVE_ORIGIN_NORMAL);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_secure_drive_pending_protocol(
            secure_io, dispatch.now_ns);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_server_io_loop_win32_maintenance(
            secure_io->protocol_loop, dispatch.now_ns);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_secure_reconcile_close_requests(secure_io);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_secure_progress_stop_stream_close(secure_io);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_secure_register_outbound(secure_io);
        if (result != PAPACC_RESULT_OK) return result;
        papacc_secure_stop_update(secure_io);
        return PAPACC_RESULT_OK;
    }
    dispatch.secure_io = secure_io;
    dispatch.result = PAPACC_RESULT_OK;
    dispatch.outbound_ready = PAPACC_FALSE;
    {
        PAPACC_SIZE event_index, outbound_index;
        for (event_index = 0U;
             event_index < secure_io->scheduler.event_count; ++event_index)
            for (outbound_index = 0U;
                 outbound_index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY;
                 ++outbound_index)
                if (secure_io->outbound_ready[outbound_index].registered ==
                        PAPACC_TRUE &&
                    secure_io->outbound_ready[outbound_index].wait_token ==
                        secure_io->scheduler.events[event_index].token)
                    dispatch.outbound_ready = PAPACC_TRUE;
    }
    result = papacc_secure_unregister_outbound(secure_io, PAPACC_TRUE,
        PAPACC_SECURE_REMOVE_ORIGIN_NORMAL);
    if (result != PAPACC_RESULT_OK) return result;
    secure_io->dispatch_active = PAPACC_TRUE;
    result = papacc_pst_secure_scheduler_dispatch_ready(
        &secure_io->scheduler, papacc_secure_dispatch, &dispatch, &dispatched);
    secure_io->dispatch_active = PAPACC_FALSE;
    if (result != PAPACC_RESULT_OK) {
        papacc_secure_clear_outbound_identity(secure_io);
        return result;
    }
    result = papacc_secure_reconcile_close_requests(secure_io);
    if (result != PAPACC_RESULT_OK) {
        papacc_secure_clear_outbound_identity(secure_io);
        return result;
    }
    (void)ready_count;
    (void)dispatched;
    papacc_secure_clear_outbound_identity(secure_io);
    if (dispatch.result != PAPACC_RESULT_OK) return dispatch.result;
    result = papacc_secure_drive_pending_protocol(
        secure_io, dispatch.now_ns);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_server_io_loop_win32_maintenance_budget(
        secure_io->protocol_loop, dispatch.now_ns,
        dispatch.outbound_ready == PAPACC_TRUE ?
            PAPACC_SERVER_NETWORK_STREAM_WAKE_BUDGET : 1U);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_secure_reconcile_close_requests(secure_io);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_secure_progress_stop_stream_close(secure_io);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_secure_register_outbound(secure_io);
    if (result != PAPACC_RESULT_OK) return result;
    papacc_secure_stop_update(secure_io);
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_secure_io_win32_request_stop(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_U64 now_ns;
    PAPACC_RESULT result;
    if (secure_io == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    secure_io->test_last_non_ok_function = NULL;
#endif
    if (secure_io->initialized != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (secure_io->owner_thread_id != (PAPACC_U32)GetCurrentThreadId())
        return PAPACC_RESULT_INVALID_STATE;
    if (secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_DRAINING ||
        secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_STOPPED)
        return PAPACC_RESULT_OK;
    if (secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL)
        return secure_io->stop_last_remove_result == PAPACC_RESULT_OK ?
            PAPACC_RESULT_INTERNAL_ERROR : secure_io->stop_last_remove_result;
    if (secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING ||
        secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_REQUESTED)
        return secure_io->stop_last_remove_result == PAPACC_RESULT_OK ?
            PAPACC_RESULT_INVALID_STATE : secure_io->stop_last_remove_result;
    if (papacc_pal_monotonic_time_ns(&now_ns) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INTERNAL_ERROR;
    secure_io->stop_state = PAPACC_SERVER_SECURE_STOP_REQUESTED;
    secure_io->stop_cause =
        PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SERVER_STOP_REQUESTED;
    secure_io->shutdown_deadline_ns = papacc_secure_deadline(
        now_ns, secure_io->establishment_timeout_ns);
    secure_io->stop_retry_not_before_ns = now_ns;
    secure_io->stop_last_remove_result = PAPACC_RESULT_OK;
    if (secure_io->dispatch_active == PAPACC_TRUE) {
        secure_io->stop_state = PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING;
        secure_io->stop_last_remove_result = PAPACC_RESULT_INVALID_STATE;
        return papacc_pst_secure_scheduler_wake(&secure_io->scheduler) ==
            PAPACC_RESULT_OK ? PAPACC_RESULT_INVALID_STATE :
            PAPACC_RESULT_INTERNAL_ERROR;
    }
    result = papacc_secure_stop_remove_round(secure_io, now_ns);
    if (result != PAPACC_RESULT_OK) return result;
    return papacc_pst_secure_scheduler_wake(&secure_io->scheduler);
}

PAPACC_BOOL papacc_server_secure_io_win32_is_stopped(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    return secure_io != NULL && secure_io->initialized == PAPACC_TRUE &&
        secure_io->stop_state == PAPACC_SERVER_SECURE_STOP_STOPPED ?
        PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_server_secure_io_win32_try_shutdown(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index;
    PAPACC_RESULT result;
    if (secure_io == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (secure_io->owner_thread_id != 0U &&
        secure_io->owner_thread_id != (PAPACC_U32)GetCurrentThreadId())
        return PAPACC_RESULT_INVALID_STATE;
    if (secure_io->dispatch_active == PAPACC_TRUE ||
        papacc_secure_has_registered_outbound(secure_io) == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (secure_io->initialized == PAPACC_TRUE &&
        secure_io->stop_state != PAPACC_SERVER_SECURE_STOP_STOPPED)
        return secure_io->stop_state ==
                PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL &&
                secure_io->stop_last_remove_result != PAPACC_RESULT_OK ?
            secure_io->stop_last_remove_result : PAPACC_RESULT_INVALID_STATE;
    if (secure_io->listener_sources != NULL) {
        for (index = 0U; index < secure_io->listener_count; ++index) {
            if (secure_io->listener_sources[index] == NULL) continue;
            result = papacc_pst_secure_scheduler_remove_external(
                &secure_io->scheduler, secure_io->listener_sources[index]);
            if (result != PAPACC_RESULT_OK) return result;
            pst_external_source_release(secure_io->listener_sources[index]);
            secure_io->listener_sources[index] = NULL;
        }
    }
    if (secure_io->scheduler.initialized == PAPACC_TRUE &&
        secure_io->scheduler.member_count != 0U)
        return PAPACC_RESULT_INVALID_STATE;
    papacc_pst_secure_scheduler_release(&secure_io->scheduler);
    *secure_io = (PAPACC_SERVER_SECURE_IO_WIN32)
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    return PAPACC_RESULT_OK;
}

void papacc_server_secure_io_win32_shutdown(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    (void)papacc_server_secure_io_win32_try_shutdown(secure_io);
}