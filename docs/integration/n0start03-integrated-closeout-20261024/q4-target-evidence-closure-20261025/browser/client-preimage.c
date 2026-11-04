#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <papacc/pacc/control_protocol.h>
#include <papacc/pacc/capability_negotiation_protocol.h>
#include <papacc/pacc/data_association_protocol.h>
#include <papacc/pacc/frame.h>
#include <papacc/pacc/message_types.h>
#include <papinho_secure_transport_win32.h>

#include "papinho_accelerator_client.h"
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
#include "papinho_pst_attach_aliases.h"
#include "papinho_credential_store.h"
#include "papinho_probe_dependencies.inc"
#include "papinho_candidate_lifetime.inc"
#include "papinho_c2_s6.h"
#endif

#define PAPINHO_ACCELERATOR_MAX_FRAME_PAYLOAD ((PAPACC_U32)0xFFFFFFFFUL)
static void papinho_accel_stream_init(papinho_accelerator_stream_state *stream) {
    if (stream == NULL) return;
    ZeroMemory(stream, sizeof(*stream));
    stream->socket = INVALID_SOCKET;
    (void)papacc_frame_parser_init(&stream->parser,
        PAPINHO_ACCELERATOR_MAX_FRAME_PAYLOAD);
}

static void papinho_accel_stream_teardown(papinho_accelerator_stream_state *stream) {
    if (stream == NULL) return;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!candidate_stream_release(stream))return;
#else
    if (stream->secure_connection != NULL) {
        pst_connection_release(stream->secure_connection);
        stream->secure_connection = NULL;
    }
#endif
    if (stream->socket_open && stream->socket != INVALID_SOCKET) {
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        if(stream->candidate_socket_close_attempted)return;
        stream->candidate_socket_close_attempted=1;
        if(closesocket(stream->socket)==SOCKET_ERROR){
            papinho_accel_pst_group *g=(papinho_accel_pst_group *)stream->candidate_group;
            stream->candidate_socket_close_error=WSAGetLastError();
            if(g){g->state=GROUP_RETAINED;
                if(g->session_storage)session_storage_retain((papinho_session_storage *)g->session_storage);}
            InterlockedExchange(&papinho_pst_admission_stopped,1);return;
        }
#else
        closesocket(stream->socket);
#endif
    }
    papinho_accel_stream_init(stream);
}

static int papinho_accel_transition_allowed(
    papinho_accelerator_transport_profile profile,
    papinho_accelerator_connection_state current,
    papinho_accelerator_connection_state next) {
    if (next == PAPINHO_ACCELERATOR_CONNECTION_TERMINAL) return 1;
    if (current == PAPINHO_ACCELERATOR_CONNECTION_TERMINAL) return 0;
    if (next == PAPINHO_ACCELERATOR_CONNECTION_CLOSING) return 1;
    if (current == PAPINHO_ACCELERATOR_CONNECTION_CLOSING) {
        return next == PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED;
    }
    switch (current) {
    case PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED:
        return next == PAPINHO_ACCELERATOR_CONNECTION_CONNECTING;
    case PAPINHO_ACCELERATOR_CONNECTION_CONNECTING:
        return next == PAPINHO_ACCELERATOR_CONNECTION_TRANSPORT_CONNECTED;
    case PAPINHO_ACCELERATOR_CONNECTION_TRANSPORT_CONNECTED:
        return (profile == PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL &&
                next == PAPINHO_ACCELERATOR_CONNECTION_SECURE_TRANSPORT_READY) ||
            (profile == PAPINHO_ACCELERATOR_TRANSPORT_LEGACY_ENDPOINT &&
                next == PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT);
    case PAPINHO_ACCELERATOR_CONNECTION_SECURE_TRANSPORT_READY:
        return next == PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT;
    case PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT:
        return next == PAPINHO_ACCELERATOR_CONNECTION_CONTROL_ACCEPTED;
    case PAPINHO_ACCELERATOR_CONNECTION_CONTROL_ACCEPTED:
        return next == PAPINHO_ACCELERATOR_CONNECTION_CAPABILITY_NEGOTIATION_SENT;
    case PAPINHO_ACCELERATOR_CONNECTION_CAPABILITY_NEGOTIATION_SENT:
        return next == PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED;
    case PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED:
        return next == PAPINHO_ACCELERATOR_CONNECTION_DATA_TICKET_AVAILABLE;
    case PAPINHO_ACCELERATOR_CONNECTION_DATA_TICKET_AVAILABLE:
        return next == PAPINHO_ACCELERATOR_CONNECTION_DATA_CONNECTING;
    case PAPINHO_ACCELERATOR_CONNECTION_DATA_CONNECTING:
        return next == PAPINHO_ACCELERATOR_CONNECTION_DATA_ATTACHED;
    case PAPINHO_ACCELERATOR_CONNECTION_DATA_ATTACHED:
        return next == PAPINHO_ACCELERATOR_CONNECTION_READY;
    default:
        return 0;
    }
}

void papinho_accelerator_client_runtime_init(
    papinho_accelerator_client_runtime *runtime,
    papinho_accelerator_transport_profile profile) {
    if (runtime == NULL) return;
    ZeroMemory(runtime, sizeof(*runtime));
    runtime->profile = profile;
    runtime->state = PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED;
    runtime->error = PAPINHO_ACCELERATOR_ERROR_NONE;
    papinho_accel_stream_init(&runtime->control);
    papinho_accel_stream_init(&runtime->data);
}

void papinho_accelerator_client_data_reset(
    papinho_accelerator_client_runtime *runtime) {
    if (runtime == NULL) return;
    papinho_accel_stream_teardown(&runtime->data);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(runtime->data.candidate_owner || runtime->data.socket_open)return;
#endif
    ZeroMemory(&runtime->data_ticket, sizeof(runtime->data_ticket));
    runtime->data_ticket_valid = 0;
    runtime->data_ticket_consumed = 0;
    runtime->data_ticket_session_generation = 0;
}

void papinho_accelerator_client_session_reset(
    papinho_accelerator_client_runtime *runtime) {
    if (runtime == NULL) return;
    papinho_accel_stream_teardown(&runtime->control);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(runtime->control.candidate_owner || runtime->control.socket_open)return;
#endif
    papinho_accelerator_client_data_reset(runtime);
    runtime->client_supported_count = 0;
    runtime->client_preference_count = 0;
    runtime->server_granted_count = 0;
    runtime->session_valid = 0;
    runtime->state = PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED;
    runtime->error = PAPINHO_ACCELERATOR_ERROR_NONE;
    ++runtime->session_generation;
    if (runtime->session_generation == 0) runtime->session_generation = 1;
}

void papinho_accelerator_client_teardown(
    papinho_accelerator_client_runtime *runtime) {
    if (runtime == NULL) return;
    papinho_accel_stream_teardown(&runtime->control);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(runtime->control.candidate_owner || runtime->control.socket_open)return;
#endif
    papinho_accelerator_client_data_reset(runtime);
    runtime->client_supported_count = 0;
    runtime->client_preference_count = 0;
    runtime->server_granted_count = 0;
    runtime->session_valid = 0;
    runtime->state = PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED;
}

void papinho_accelerator_client_runtime_destroy(
    papinho_accelerator_client_runtime *runtime) {
    papinho_accelerator_client_teardown(runtime);
}

static unsigned long papinho_accel_reconnect_jitter(
    papinho_accelerator_reconnect_state *state) {
    unsigned long base, minimum, span;
    if (state == NULL) return 0UL;
    base = state->policy.max_delay_ms;
    minimum = (base / 100UL) * 80UL;
    if (minimum > base) minimum = base;
    span = base - minimum + 1UL;
    if (state->jitter_state == 0UL)
        state->jitter_state = state->policy.jitter_seed ^ GetTickCount() ^
            GetCurrentProcessId() ^ state->generation;
    state->jitter_state = state->jitter_state * 1664525UL + 1013904223UL;
    return minimum + (span == 0UL ? 0UL : state->jitter_state % span);
}

static unsigned long papinho_accel_reconnect_delay(
    papinho_accelerator_reconnect_state *state) {
    static const unsigned long first_cycle[] = {
        1000UL, 3000UL, 10000UL, 30000UL, 60000UL };
    unsigned long failure_count;
    if (state == NULL) return 0UL;
    failure_count = state->failure_count;
    if (failure_count == 0UL) return 0UL;
    if (failure_count <= 5UL) return first_cycle[failure_count - 1UL];
    return papinho_accel_reconnect_jitter(state);
}

void papinho_accelerator_reconnect_policy_default(
    papinho_accelerator_reconnect_policy *policy) {
    if (policy == NULL) return;
    ZeroMemory(policy, sizeof(*policy));
    policy->enabled = 1;
    policy->immediate_attempts = 1;
    policy->initial_delay_ms = 0UL;
    policy->max_delay_ms = 60000UL;
    policy->continue_trying = 1;
    policy->rapid_attempt_limit = 4;
    policy->rapid_attempt_window_ms = 10000UL;
    policy->jitter_seed = GetTickCount() ^ GetCurrentProcessId();
}

void papinho_accelerator_reconnect_init(
    papinho_accelerator_reconnect_state *state,
    const papinho_accelerator_client_config *config, unsigned long now_ms) {
    if (state == NULL) return;
    ZeroMemory(state, sizeof(*state));
    papinho_accelerator_reconnect_policy_default(&state->policy);
    if (config != NULL) state->config = *config;
    state->generation = 1;
    state->jitter_state = state->policy.jitter_seed;
    state->availability = state->policy.enabled ?
        PAPINHO_ACCELERATOR_AVAILABILITY_CONNECTING :
        PAPINHO_ACCELERATOR_AVAILABILITY_OFF;
    state->next_attempt_at_ms = now_ms;
    state->automatic_attempt_pending = state->policy.enabled;
}

void papinho_accelerator_reconnect_disable(
    papinho_accelerator_reconnect_state *state) {
    if (state == NULL) return;
    ++state->generation;
    if (state->generation == 0) ++state->generation;
    state->availability = PAPINHO_ACCELERATOR_AVAILABILITY_OFF;
    state->has_been_online = 0;
    state->automatic_attempt_pending = 0;
    state->manual_attempt_pending = 0;
    state->next_attempt_at_ms = 0;
}

void papinho_accelerator_reconnect_reconfigure(
    papinho_accelerator_reconnect_state *state,
    const papinho_accelerator_client_config *config, unsigned long now_ms) {
    if (state == NULL || config == NULL) return;
    state->config = *config;
    ++state->generation;
    state->has_been_online = 0;
    if (state->generation == 0) ++state->generation;
    state->failure_count = 0;
    state->rapid_attempt_count = 0;
    state->rapid_window_started_ms = now_ms;
    state->continued_regime = 0;
    state->manual_attempt_pending = 0;
    state->availability = state->policy.enabled ?
        PAPINHO_ACCELERATOR_AVAILABILITY_CONNECTING :
        PAPINHO_ACCELERATOR_AVAILABILITY_OFFLINE;
    state->next_attempt_at_ms = now_ms;
    state->automatic_attempt_pending = state->policy.enabled;
}

void papinho_accelerator_reconnect_set_policy(
    papinho_accelerator_reconnect_state *state,
    const papinho_accelerator_reconnect_policy *policy, unsigned long now_ms) {
    papinho_accelerator_reconnect_policy previous;
    if (state == NULL || policy == NULL) return;
    previous = state->policy;
    state->policy = *policy;
    if (state->policy.jitter_seed == 0UL)
        state->policy.jitter_seed = GetTickCount() ^ GetCurrentProcessId();
    if (!state->policy.enabled) {
        if (state->availability != PAPINHO_ACCELERATOR_AVAILABILITY_ONLINE) {
            if (state->automatic_attempt_pending && !state->manual_attempt_pending &&
                state->availability == PAPINHO_ACCELERATOR_AVAILABILITY_CONNECTING)
                state->availability = PAPINHO_ACCELERATOR_AVAILABILITY_OFFLINE;
            state->automatic_attempt_pending = 0;
            state->next_attempt_at_ms = 0UL;
        }
        return;
    }
    if (state->availability == PAPINHO_ACCELERATOR_AVAILABILITY_OFFLINE ||
        state->availability == PAPINHO_ACCELERATOR_AVAILABILITY_DEGRADED) {
        if (state->continued_regime && !state->policy.continue_trying &&
            !state->manual_attempt_pending) {
            state->automatic_attempt_pending = 0;
            state->next_attempt_at_ms = 0UL;
            return;
        }
        if (!previous.enabled ||
            (!previous.continue_trying && state->continued_regime)) {
            state->next_attempt_at_ms = now_ms;
            state->automatic_attempt_pending = 1;
        } else if (state->continued_regime &&
                   state->automatic_attempt_pending) {
            state->next_attempt_at_ms = now_ms +
                papinho_accel_reconnect_jitter(state);
        }
    }
}

/* Win32 DWORD/LONG are exactly 32 bits, including under VC6.
 * Addition/subtraction intentionally wraps; never order absolute tick values. */
typedef char papinho_tick_types_are_32_bit[
    (sizeof(DWORD) == 4 && sizeof(LONG) == 4 &&
     sizeof(unsigned long) == 4) ? 1 : -1];

int papinho_tick_deadline_due(DWORD now, DWORD deadline) {
    return (LONG)(now - deadline) >= 0;
}

DWORD papinho_tick_deadline_remaining_ms(DWORD now, DWORD deadline) {
    return papinho_tick_deadline_due(now, deadline) ? 0UL : deadline - now;
}

int papinho_accelerator_reconnect_attempt_due(
    const papinho_accelerator_reconnect_state *state, unsigned long now_ms) {
    return state != NULL && state->automatic_attempt_pending &&
        (state->policy.enabled || state->manual_attempt_pending) &&
        state->availability != PAPINHO_ACCELERATOR_AVAILABILITY_OFF &&
        state->availability != PAPINHO_ACCELERATOR_AVAILABILITY_INCOMPATIBLE &&
        papinho_tick_deadline_due((DWORD)now_ms, (DWORD)state->next_attempt_at_ms);
}

unsigned long papinho_accelerator_reconnect_begin_attempt(
    papinho_accelerator_reconnect_state *state, unsigned long now_ms) {
    if (!papinho_accelerator_reconnect_attempt_due(state, now_ms)) return 0;
    if (state->rapid_window_started_ms == 0 || now_ms -
        state->rapid_window_started_ms >= state->policy.rapid_attempt_window_ms) {
        state->rapid_window_started_ms = now_ms;
        state->rapid_attempt_count = 0;
    }
    ++state->rapid_attempt_count;
    ++state->attempt_count;
    state->availability = PAPINHO_ACCELERATOR_AVAILABILITY_CONNECTING;
    state->automatic_attempt_pending = 0;
    state->manual_attempt_pending = 0;
    return state->generation;
}

int papinho_accelerator_reconnect_complete(
    papinho_accelerator_reconnect_state *state, unsigned long attempt_generation,
    int success, papinho_accelerator_error error, unsigned long now_ms) {
    unsigned long delay;
    if (state == NULL || attempt_generation == 0 ||
        attempt_generation != state->generation ||
        state->availability == PAPINHO_ACCELERATOR_AVAILABILITY_OFF) return 0;
    state->last_error = error;
    if (success) {
        state->availability = PAPINHO_ACCELERATOR_AVAILABILITY_ONLINE;
        state->has_been_online = 1;
        state->failure_count = 0;
        state->rapid_attempt_count = 0;
        state->continued_regime = 0;
        state->automatic_attempt_pending = 0;
        return 1;
    }
    if (error == PAPINHO_ACCELERATOR_ERROR_SECURE ||
        error == PAPINHO_ACCELERATOR_ERROR_PROTOCOL ||
        error == PAPINHO_ACCELERATOR_ERROR_CONTROL_REJECTED ||
        error == PAPINHO_ACCELERATOR_ERROR_CAPABILITY) {
        state->availability = PAPINHO_ACCELERATOR_AVAILABILITY_INCOMPATIBLE;
        state->automatic_attempt_pending = 0;
        return 1;
    }
    state->availability = error == PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH ?
        PAPINHO_ACCELERATOR_AVAILABILITY_DEGRADED :
        PAPINHO_ACCELERATOR_AVAILABILITY_OFFLINE;
    ++state->failure_count;
    if (!state->policy.enabled) {
        state->automatic_attempt_pending = 0;
        return 1;
    }
    if (state->failure_count <= 5UL) {
        delay = papinho_accel_reconnect_delay(state);
        state->next_attempt_at_ms = now_ms + delay;
        state->automatic_attempt_pending = 1;
        return 1;
    }
    state->continued_regime = 1;
    if (!state->policy.continue_trying) {
        state->automatic_attempt_pending = 0;
        return 1;
    }
    delay = papinho_accel_reconnect_delay(state);
    state->next_attempt_at_ms = now_ms + delay;
    state->automatic_attempt_pending = 1;
    return 1;
}

void papinho_accelerator_reconnect_request_manual(
    papinho_accelerator_reconnect_state *state, unsigned long now_ms) {
    if (state == NULL ||
        state->availability == PAPINHO_ACCELERATOR_AVAILABILITY_OFF ||
        state->availability == PAPINHO_ACCELERATOR_AVAILABILITY_ONLINE ||
        state->availability == PAPINHO_ACCELERATOR_AVAILABILITY_INCOMPATIBLE)
        return;
    state->next_attempt_at_ms = now_ms;
    state->manual_attempt_pending = 1;
    state->automatic_attempt_pending = 1;
}

void papinho_accelerator_reconnect_get_snapshot(
    const papinho_accelerator_reconnect_state *state,
    papinho_accelerator_reconnect_snapshot *out_snapshot) {
    if (out_snapshot == NULL) return;
    ZeroMemory(out_snapshot, sizeof(*out_snapshot));
    if (state == NULL) return;
    out_snapshot->availability = state->availability;
    out_snapshot->generation = state->generation;
    out_snapshot->has_been_online = state->has_been_online;
    out_snapshot->attempt_count = state->attempt_count;
    out_snapshot->failure_count = state->failure_count;
    out_snapshot->next_attempt_at_ms = state->next_attempt_at_ms;
    out_snapshot->last_error = state->last_error;
    out_snapshot->profile = state->config.profile;
}
int papinho_accelerator_client_transition(
    papinho_accelerator_client_runtime *runtime,
    papinho_accelerator_connection_state next_state) {
    if (runtime == NULL || !papinho_accel_transition_allowed(runtime->profile,
        runtime->state, next_state)) return -1;
    runtime->state = next_state;
    if (next_state == PAPINHO_ACCELERATOR_CONNECTION_TERMINAL) {
        runtime->session_valid = 0;
    }
    return 0;
}

int papinho_accelerator_client_set_ticket(
    papinho_accelerator_client_runtime *runtime,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket) {
    if (runtime == NULL || ticket == NULL ||
        !papacc_data_association_ticket_is_valid(ticket)) return -1;
    runtime->data_ticket = *ticket;
    runtime->data_ticket_valid = 1;
    runtime->data_ticket_consumed = 0;
    runtime->data_ticket_session_generation = runtime->session_generation;
    return 0;
}

int papinho_accelerator_client_consume_ticket(
    papinho_accelerator_client_runtime *runtime) {
    if (runtime == NULL || !runtime->data_ticket_valid ||
        runtime->data_ticket_consumed || runtime->data_ticket_session_generation !=
        runtime->session_generation) return -1;
    runtime->data_ticket_consumed = 1;
    return 0;
}

int papinho_accelerator_client_capability_granted(
    const papinho_accelerator_client_runtime *runtime,
    const PAPACC_CAPABILITY_KEY *key) {
    PAPACC_CAPABILITY_SET granted = PAPACC_CAPABILITY_SET_INITIALIZER;
    if (runtime == NULL || key == NULL ||
        papacc_capability_set_init(&granted,
        (PAPACC_CAPABILITY_KEY*)runtime->server_granted,
        PAPINHO_ACCELERATOR_CAPABILITY_MAX) != PAPACC_RESULT_OK) return 0;
    granted.count = runtime->server_granted_count;
    return papacc_capability_set_contains(&granted, key) == PAPACC_TRUE ? 1 : 0;
}

int papinho_accelerator_client_session_network_egress_major(
    const papinho_accelerator_client_runtime *runtime) {
    if (runtime == NULL || (runtime->network_egress_major != 1U &&
        runtime->network_egress_major != 2U)) return 0;
    return (int)runtime->network_egress_major;
}

int papinho_accelerator_client_mode_b_requirements_granted(
    const papinho_accelerator_client_runtime *runtime) {
    PAPACC_CAPABILITY_KEY network = PAPACC_PACC_NETWORK_EGRESS_V2_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY tls = PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    if (runtime == NULL) return 0;
    if (runtime->network_egress_major == 1U)
        network.major = PAPACC_PACC_NETWORK_EGRESS_MAJOR_V1;
    else if (runtime->network_egress_major != 2U) return 0;
    return papinho_accelerator_client_capability_granted(runtime, &network) &&
        papinho_accelerator_client_capability_granted(runtime, &tls);
}

const char *papinho_accelerator_secure_stage_name(
papinho_accelerator_secure_stage stage) {
    switch (stage) {
    case PAPINHO_ACCELERATOR_SECURE_STAGE_CONFIG_VALIDATE: return "CONFIG_VALIDATE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_SECURE_ATTACH_BEGIN: return "SECURE_ATTACH_BEGIN";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_TRUST_CONFIGURE: return "TRUST_CONFIGURE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_CLIENT_CERT_CONFIGURE: return "CLIENT_CERT_CONFIGURE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_CLIENT_KEY_CONFIGURE: return "CLIENT_KEY_CONFIGURE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_PEER_NAME_CONFIGURE: return "PEER_NAME_CONFIGURE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_ALPN_CONFIGURE: return "ALPN_CONFIGURE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_PROVIDER_ACQUIRE: return "PST_PROVIDER_ACQUIRE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_RUNTIME_CREATE: return "PST_RUNTIME_CREATE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_TCP_CONNECT_BEGIN: return "TCP_CONNECT_BEGIN";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_TCP_CONNECT_COMPLETE: return "TCP_CONNECT_COMPLETE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_CONNECTION_CREATE: return "PST_CONNECTION_CREATE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_TRANSPORT_ATTACH: return "PST_TRANSPORT_ATTACH";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_TLS_HANDSHAKE: return "TLS_HANDSHAKE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_PEER_VALIDATE: return "PEER_VALIDATE";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_CONTROL_OPEN: return "CONTROL_OPEN";
    case PAPINHO_ACCELERATOR_SECURE_STAGE_CONTROL_ACCEPT: return "CONTROL_ACCEPT";
    default: return "NONE";
    }
}

static void papinho_accel_secure_stage_begin(
    papinho_accelerator_secure_stage_trace *trace,
    papinho_accelerator_secure_stage stage, const char *operation) {
    if (trace == NULL) return;
    ZeroMemory(trace, sizeof(*trace));
    trace->pst_result = PST_RESULT_OK;
    trace->browser_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    strncpy(trace->provider_name, "retrozilla-nss", sizeof(trace->provider_name) - 1U);
    if (operation != NULL) strncpy(trace->operation_name, operation,
        sizeof(trace->operation_name) - 1U);
    trace->last_successful_stage = stage;
}

static void papinho_accel_secure_stage_success(
    papinho_accelerator_secure_stage_trace *trace,
    papinho_accelerator_secure_stage stage, const char *operation) {
    if (trace == NULL) return;
    trace->last_successful_stage = stage;
    trace->pst_result = PST_RESULT_OK;
    if (operation != NULL) {
        strncpy(trace->operation_name, operation, sizeof(trace->operation_name) - 1U);
        trace->operation_name[sizeof(trace->operation_name) - 1U] = '\0';
    }
}

static void papinho_accel_secure_stage_failure(
    papinho_accelerator_secure_stage_trace *trace,
    papinho_accelerator_secure_stage stage, PST_RESULT result,
    papinho_accelerator_error error, const char *operation) {
    if (trace == NULL) return;
    if (trace->first_failed_stage == PAPINHO_ACCELERATOR_SECURE_STAGE_NONE)
        trace->first_failed_stage = stage;
    trace->pst_result = result;
    trace->browser_error = error;
    if (operation != NULL) {
        strncpy(trace->operation_name, operation, sizeof(trace->operation_name) - 1U);
        trace->operation_name[sizeof(trace->operation_name) - 1U] = '\0';
    }
}

static int papinho_accel_cancelled(HANDLE event) {
    return event != NULL && WaitForSingleObject(event, 0) == WAIT_OBJECT_0;
}

static int papinho_accel_wait(SOCKET socket, int write_ready,
    unsigned long timeout_ms, HANDLE cancel_event) {
    unsigned long elapsed = 0;
    while (elapsed < timeout_ms) {
        fd_set set;
        struct timeval tv;
        int rc;
        if (papinho_accel_cancelled(cancel_event)) return -2;
        FD_ZERO(&set); FD_SET(socket, &set);
        tv.tv_sec = 0; tv.tv_usec = 50000;
        rc = select(0, write_ready ? NULL : &set, write_ready ? &set : NULL,
            NULL, &tv);
        if (rc > 0) return 0;
        if (rc < 0) return -1;
        elapsed += 50;
    }
    return -1;
}

/* Opt-in observation only; preserve the caller's Win32/Winsock error. */
static void papinho_accel_tcp_diagnostic(const char *stage,
    const papinho_accelerator_client_config *config, SOCKET socket_value,
    int rc, const void *identity) {
    DWORD saved_error = GetLastError();
    int saved_wsa = WSAGetLastError();
    char path[MAX_PATH];
    DWORD length;
    FILE *file;
    length = GetEnvironmentVariableA("PAPINHO_ACCELERATOR_TCP_DIAGNOSTIC",
        path, sizeof(path));
    if (length > 0 && length < sizeof(path)) {
        file = fopen(path, "ab");
        if (file != NULL) {
            fprintf(file, "%lu T%lu %s host=%s port=%u profile=%d timeout=%lu socket=%lu rc=%d wsa=%d identity=%p runtime=%p\n",
                GetTickCount(), GetCurrentThreadId(), stage,
                config != NULL ? config->host : "-",
                config != NULL ? (unsigned int)config->port : 0U,
                config != NULL ? (int)config->profile : -1,
                config != NULL ? config->timeout_ms : 0UL,
                (unsigned long)socket_value, rc, saved_wsa, identity,
                config != NULL ? config->secure_runtime : NULL);
            fclose(file);
        }
    }
    WSASetLastError(saved_wsa);
    SetLastError(saved_error);
}

static SOCKET papinho_accel_connect(
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    struct sockaddr_in address;
    unsigned long nonblocking = 1;
    SOCKET s;
    int rc;
    papinho_accel_tcp_diagnostic("socket-enter", config, INVALID_SOCKET, 0, NULL);
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    papinho_accel_tcp_diagnostic("socket-return", config, s, 0, NULL);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;
    if (ioctlsocket(s, FIONBIO, &nonblocking) != 0) {
        closesocket(s); return INVALID_SOCKET;
    }
    ZeroMemory(&address, sizeof(address));
    address.sin_family = AF_INET; address.sin_port = htons(config->port);
    address.sin_addr.s_addr = inet_addr(config->host);
    if (address.sin_addr.s_addr == INADDR_NONE) {
        closesocket(s); return INVALID_SOCKET;
    }
    papinho_accel_tcp_diagnostic("connect-enter", config, s, 0, NULL);
    rc = connect(s, (struct sockaddr*)&address, sizeof(address));
    papinho_accel_tcp_diagnostic("connect-return", config, s, rc, NULL);
    if (rc != 0 && WSAGetLastError() != WSAEWOULDBLOCK &&
        WSAGetLastError() != WSAEINPROGRESS) {
        closesocket(s); return INVALID_SOCKET;
    }
    if (rc != 0) {
        int error = 0;
        int error_size = sizeof(error);
        papinho_accel_tcp_diagnostic("connect-wait-enter", config, s, 0, NULL);
        if (papinho_accel_wait(s, 1, config->timeout_ms, cancel_event) != 0 ||
            getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&error,
                &error_size) != 0 || error != 0) {
            papinho_accel_tcp_diagnostic("connect-wait-failed", config, s, error, NULL);
            closesocket(s); return INVALID_SOCKET;
        }
    }
    papinho_accel_tcp_diagnostic("connect-success", config, s, 0, NULL);
    return s;
}

static int papinho_accel_send_all(SOCKET socket, const PAPACC_U8 *bytes,
    int count, const papinho_accelerator_client_config *config,
    HANDLE cancel_event) {
    int sent = 0;
    while (sent < count) {
        int rc;
        if (papinho_accel_wait(socket, 1, config->timeout_ms, cancel_event) != 0)
            return -1;
        rc = send(socket, (const char*)bytes + sent, count - sent, 0);
        if (rc <= 0) return -1;
        sent += rc;
    }
    return 0;
}

static int papinho_accel_secure_wait(pst_connection *connection,
    unsigned long timeout_ms, HANDLE cancel_event) {
    PST_WAIT_RESULT wait_result;
    if (connection == NULL || papinho_accel_cancelled(cancel_event)) return -1;
    if (pst_connection_wait(connection, (pst_u32)timeout_ms, &wait_result) !=
        PST_RESULT_OK || wait_result.timed_out != 0U ||
        papinho_accel_cancelled(cancel_event)) return -1;
    return 0;
}

static int papinho_accel_secure_attach(
    papinho_accelerator_stream_state *stream,
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_secure_stage_trace *trace) {
    static const pst_u8 alpn_bytes[] = "papacc/1";
    static const pst_u8 wrong_alpn_bytes[] = "papacc-test-wrong";
    const pst_u8 *expected_alpn = alpn_bytes;
    pst_size expected_alpn_size = sizeof(alpn_bytes) - 1U;
    PST_ALPN_PROTOCOL alpn;
    PST_CONNECTION_CONFIG secure_config;
    PST_PEER_INFO_SUMMARY peer_summary;
    pst_transport *transport = NULL;
    pst_peer_info *peer_info = NULL;
    pst_u8 selected_alpn[16];
    pst_size selected_alpn_size = 0U;
    pst_u32 accepted = 0U;
    pst_u32 operation = PST_OPERATION_NEED_READ;
    PST_RESULT source_error = PST_RESULT_OK;
    PST_RESULT pst_result = PST_RESULT_OK;
    unsigned int attempts = 0U;
    const char *peer_name;


    if (stream == NULL || config == NULL || stream->socket == INVALID_SOCKET ||
        config->secure_runtime == NULL || config->secure_credentials == NULL ||
        config->secure_trust == NULL) {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_SECURE_ATTACH_BEGIN, PST_RESULT_INVALID_ARGUMENT,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "secure_attach_arguments");
        return -1;
    }
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_SECURE_ATTACH_BEGIN, "secure_attach_begin");
    peer_name = config->secure_peer_name[0] != '\0' ?
        config->secure_peer_name : config->host;
    if (peer_name[0] == '\0') {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_PEER_NAME_CONFIGURE, PST_RESULT_INVALID_ARGUMENT,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "peer_name");
        return -1;
    }
    ZeroMemory(&secure_config, sizeof(secure_config));
    if (config->fixture_alpn_mode == PAPINHO_ACCELERATOR_FIXTURE_ALPN_WRONG) {
        expected_alpn = wrong_alpn_bytes;
        expected_alpn_size = sizeof(wrong_alpn_bytes) - 1U;
    } else if (config->fixture_alpn_mode == PAPINHO_ACCELERATOR_FIXTURE_ALPN_MISSING) {
        expected_alpn = NULL;
        expected_alpn_size = 0U;
    }
    alpn.data = expected_alpn; alpn.size = expected_alpn_size;
    secure_config.struct_size = (pst_u32)sizeof(secure_config);
    secure_config.api_version = PST_API_VERSION;
    secure_config.role = PST_CONNECTION_ROLE_CLIENT;
    secure_config.provider_selection.struct_size =
        (pst_u32)sizeof(secure_config.provider_selection);
    secure_config.provider_selection.api_version = PST_API_VERSION;
    secure_config.provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    secure_config.provider_selection.exact_provider_id = "retrozilla-nss";
    secure_config.provider_selection.required_capabilities = PST_CAP_TLS_1_3 |
        PST_CAP_ROLE_CLIENT | PST_CAP_LOCAL_IDENTITY | PST_CAP_PEER_CERT_AUTH |
        PST_CAP_ALPN_CLIENT | PST_CAP_CUSTOM_TRUST | PST_CAP_PEER_NAME_VERIFY |
        PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT | PST_CAP_GRACEFUL_SHUTDOWN;
    secure_config.local_identity.struct_size =
        (pst_u32)sizeof(secure_config.local_identity);
    secure_config.local_identity.api_version = PST_API_VERSION;
    secure_config.local_identity.credentials = config->secure_credentials;
    secure_config.peer_authentication.struct_size =
        (pst_u32)sizeof(secure_config.peer_authentication);
    secure_config.peer_authentication.api_version = PST_API_VERSION;
    secure_config.peer_authentication.certificate_mode = PST_PEER_CERTIFICATE_REQUIRED;
    secure_config.peer_authentication.trust = config->secure_trust;
    secure_config.peer_authentication.expected_peer_name = peer_name;
    secure_config.peer_authentication.expected_peer_name_size = strlen(peer_name);
    secure_config.tls.struct_size = (pst_u32)sizeof(secure_config.tls);
    secure_config.tls.api_version = PST_API_VERSION;
    secure_config.tls.minimum_version = PST_TLS_VERSION_1_3;
    secure_config.tls.maximum_version = PST_TLS_VERSION_1_3;
    secure_config.tls.resumption = PST_FEATURE_DISABLED;
    secure_config.tls.early_data = PST_FEATURE_DISABLED;
    secure_config.tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    secure_config.alpn.struct_size = (pst_u32)sizeof(secure_config.alpn);
    secure_config.alpn.api_version = PST_API_VERSION;
    secure_config.alpn.mode = expected_alpn != NULL ? PST_FEATURE_REQUIRED :
        PST_FEATURE_DISABLED;
    secure_config.alpn.protocols = expected_alpn != NULL ? &alpn : NULL;
    secure_config.alpn.protocol_count = expected_alpn != NULL ? 1U : 0U;
    secure_config.server_name_indication_mode = PST_SNI_MODE_COMPAT;
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_PEER_NAME_CONFIGURE, "secure_config.peer_name");
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_ALPN_CONFIGURE, "secure_config.alpn");

#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    {
        papinho_accel_pst_group *g=(papinho_accel_pst_group *)config->candidate_group;
        papinho_pst_owner *o;
        if(!g || g->runtime!=config->secure_runtime || stream->candidate_owner)goto fail;
        o=papinho_pst_group_reserve(g,NULL);if(!o)goto fail;
        stream->candidate_owner=o;stream->candidate_group=g;
        /* Constructor writes the sole durable owner slot, never a stack slot. */
        pst_result=pst_connection_create(config->secure_runtime,&secure_config,&o->connection);
        stream->secure_connection=o->connection;
    }
#else
    pst_result = pst_connection_create(config->secure_runtime, &secure_config,
        &stream->secure_connection);
#endif
    if (pst_result != PST_RESULT_OK) {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_CONNECTION_CREATE, pst_result,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_connection_create");
        goto fail;
    }
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_CONNECTION_CREATE, "pst_connection_create");
    papinho_accel_tcp_diagnostic("pst-connection-created", config,
        stream->socket, 0, stream->secure_connection);
    pst_result = pst_win32_socket_transport_create((pst_size)stream->socket, &transport);
    if (pst_result != PST_RESULT_OK) {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_TRANSPORT_ATTACH, pst_result,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_win32_socket_transport_create");
        goto fail;
    }
    pst_result = pst_connection_attach(stream->secure_connection, transport,
        PST_OWNERSHIP_TRANSFERRED, &accepted);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    /* Transfer and operation result are independent (ADR-0009 A2/A3).
     * Invalidate before the error path can release a consumed wrapper. */
    papinho_pst_attach_aliases(accepted, &transport,
        &stream->socket, &stream->socket_open);
#endif
    if (pst_result != PST_RESULT_OK || accepted == 0U) {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_TRANSPORT_ATTACH,
            pst_result != PST_RESULT_OK ? pst_result : PST_RESULT_PROTOCOL_FAILURE,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_connection_attach");
        goto fail;
    }
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_TRANSPORT_ATTACH, "pst_connection_attach");

    transport = NULL;
    stream->socket = INVALID_SOCKET;
    stream->socket_open = 0;


    papinho_accel_tcp_diagnostic("handshake-enter", config,
        stream->socket, 0, stream->secure_connection);
    while (attempts++ < 64U) {
        pst_result = pst_connection_handshake(stream->secure_connection, &operation,
            &source_error);
        if (pst_result != PST_RESULT_OK || operation == PST_OPERATION_FAILED ||
            operation == PST_OPERATION_CLOSED) {
            papinho_accel_secure_stage_failure(trace,
                PAPINHO_ACCELERATOR_SECURE_STAGE_TLS_HANDSHAKE,
                pst_result != PST_RESULT_OK ? pst_result : source_error,
                PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_connection_handshake");
            goto fail;
        }
        if (operation == PST_OPERATION_COMPLETE) break;
        if (papinho_accel_secure_wait(stream->secure_connection,
            config->timeout_ms, cancel_event) != 0) {
            papinho_accel_secure_stage_failure(trace,
                PAPINHO_ACCELERATOR_SECURE_STAGE_TLS_HANDSHAKE, PST_RESULT_TRANSPORT_FAILURE,
                papinho_accel_cancelled(cancel_event) ? PAPINHO_ACCELERATOR_ERROR_CANCELLED : PAPINHO_ACCELERATOR_ERROR_SECURE,
                "pst_connection_wait");
            goto fail;
        }
    }
    if (operation != PST_OPERATION_COMPLETE) {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_TLS_HANDSHAKE, PST_RESULT_TRANSPORT_FAILURE,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_connection_handshake_timeout");
        goto fail;
    }
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_TLS_HANDSHAKE, "pst_connection_handshake");

    ZeroMemory(&peer_summary, sizeof(peer_summary));
    peer_summary.struct_size = (pst_u32)sizeof(peer_summary);
    peer_summary.api_version = PST_API_VERSION;
    if (pst_connection_get_peer_info(stream->secure_connection, &peer_info) !=
        PST_RESULT_OK || pst_peer_info_get_summary(peer_info, &peer_summary) !=
        PST_RESULT_OK || peer_summary.tls_version != PST_TLS_VERSION_1_3 ||
        peer_summary.peer_authenticated == 0U || peer_summary.chain_validated == 0U ||
        peer_summary.peer_name_validated == 0U ||
        (expected_alpn != NULL &&
        (pst_connection_get_negotiated_alpn(stream->secure_connection,
        selected_alpn, sizeof(selected_alpn), &selected_alpn_size) != PST_RESULT_OK ||
        selected_alpn_size != expected_alpn_size ||
        memcmp(selected_alpn, expected_alpn, expected_alpn_size) != 0))) goto fail;
    pst_peer_info_release(peer_info);
    return 0;
fail:
    if (peer_info != NULL) pst_peer_info_release(peer_info);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if (transport != NULL) {
        pst_transport_release(transport);
        stream->socket=INVALID_SOCKET;stream->socket_open=0;
    }
#else
    if (transport != NULL) pst_transport_release(transport);
#endif
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    /* No independent release from the failure stack. The same reserved owner
     * is finalized by checked Session teardown or synchronous group teardown. */
#else
    if (stream->secure_connection != NULL) {
        pst_connection_release(stream->secure_connection);
        stream->secure_connection = NULL;
    }
#endif
    return -1;
}

static int papinho_accel_stream_send_all(
    papinho_accelerator_stream_state *stream, const PAPACC_U8 *bytes,
    int count, const papinho_accelerator_client_config *config,
    HANDLE cancel_event) {
    int sent = 0;
    if (stream == NULL) return -1;
    if (stream->secure_connection == NULL)
        return papinho_accel_send_all(stream->socket, bytes, count, config,
            cancel_event);
    while (sent < count) {
        PST_IO_RESULT io_result;
        if (papinho_accel_cancelled(cancel_event) ||
            pst_connection_write(stream->secure_connection, bytes + sent,
            (pst_size)(count - sent), &io_result) != PST_RESULT_OK ||
            io_result.operation == PST_OPERATION_FAILED ||
            io_result.operation == PST_OPERATION_CLOSED) return -1;
        sent += (int)io_result.bytes_transferred;
        if (sent < count && papinho_accel_secure_wait(stream->secure_connection,
            config->timeout_ms, cancel_event) != 0) return -1;
    }
    return 0;
}

static int papinho_accel_stream_receive(
    papinho_accelerator_stream_state *stream, PAPACC_U8 *bytes, int capacity,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    PST_IO_RESULT io_result;
    if (stream == NULL) return -1;
    if (stream->secure_connection == NULL) {
        if (papinho_accel_wait(stream->socket, 0, config->timeout_ms,
            cancel_event) != 0) return -2;
        return recv(stream->socket, (char*)bytes, capacity, 0);
    }
    for (;;) {
        if (papinho_accel_cancelled(cancel_event) ||
            pst_connection_read(stream->secure_connection, bytes, (pst_size)capacity,
            &io_result) != PST_RESULT_OK ||
            io_result.operation == PST_OPERATION_FAILED) return -1;
        if (io_result.operation == PST_OPERATION_CLOSED) return 0;
        if (io_result.bytes_transferred != 0U)
            return (int)io_result.bytes_transferred;
        if (papinho_accel_secure_wait(stream->secure_connection,
            config->timeout_ms, cancel_event) != 0) return -2;
    }
}

static int papinho_accel_control_fail(
    papinho_accelerator_client_runtime *runtime,
    papinho_accelerator_error error) {
    runtime->error = error;
    (void)papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_TERMINAL);
    return -1;
}

static int papinho_accel_data_fail(papinho_accelerator_client_runtime *runtime,
    papinho_accelerator_error error) {
    if (runtime == NULL) return -1;
    runtime->error = error;
    runtime->data.terminal = 1;
    papinho_accelerator_client_data_reset(runtime);
    return -1;
}

static int papinho_accel_control_queue_open(
    papinho_accelerator_client_runtime *runtime) {
    PAPACC_CONTROL_PROTOCOL_VERSION version =
        PAPACC_CONTROL_PROTOCOL_VERSION_INITIALIZER;
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_U8 payload[PAPACC_CONTROL_VERSION_PAYLOAD_SIZE];
    PAPACC_SIZE payload_written = 0;
    PAPACC_SIZE header_written = 0;
    if (runtime == NULL || (runtime->state !=
        PAPINHO_ACCELERATOR_CONNECTION_TRANSPORT_CONNECTED && runtime->state !=
        PAPINHO_ACCELERATOR_CONNECTION_SECURE_TRANSPORT_READY)) return -1;
    version.major = PAPACC_CONTROL_PROTOCOL_MAJOR;
    version.minor = PAPACC_CONTROL_PROTOCOL_MINOR;
    header = papacc_control_open_frame_header();
    if (papacc_control_open_encode(&version, payload, sizeof(payload),
        &payload_written) != PAPACC_RESULT_OK ||
        papacc_frame_header_encode(&header, runtime->control.output_buffer,
        sizeof(runtime->control.output_buffer), &header_written) !=
        PAPACC_RESULT_OK || payload_written + header_written >
        sizeof(runtime->control.output_buffer)) return -1;
    memcpy(runtime->control.output_buffer + header_written, payload,
        payload_written);
    runtime->control.output_begin = 0;
    runtime->control.output_end = header_written + payload_written;
    return papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT);
}

static int papinho_accel_capability_sets(
    papinho_accelerator_client_runtime *runtime,
    PAPACC_CAPABILITY_SET *supported, PAPACC_CAPABILITY_SET *preference,
    PAPACC_CAPABILITY_SET *granted) {
    ZeroMemory(supported, sizeof(*supported));
    ZeroMemory(preference, sizeof(*preference));
    ZeroMemory(granted, sizeof(*granted));
    if (papacc_capability_set_init(supported, runtime->client_supported,
        PAPINHO_ACCELERATOR_CAPABILITY_MAX) != PAPACC_RESULT_OK ||
        papacc_capability_set_init(preference, runtime->client_preference,
        PAPINHO_ACCELERATOR_CAPABILITY_MAX) != PAPACC_RESULT_OK ||
        papacc_capability_set_init(granted, runtime->server_granted,
        PAPINHO_ACCELERATOR_CAPABILITY_MAX) != PAPACC_RESULT_OK) return -1;
    supported->count = runtime->client_supported_count;
    preference->count = runtime->client_preference_count;
    granted->count = runtime->server_granted_count;
    return 0;
}

static int papinho_accel_control_queue_negotiate(
    papinho_accelerator_client_runtime *runtime) {
    PAPACC_CAPABILITY_SET supported;
    PAPACC_CAPABILITY_SET preference;
    PAPACC_CAPABILITY_SET granted;
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_SIZE payload_written = 0;
    PAPACC_SIZE header_written = 0;
    if (runtime == NULL || runtime->state !=
        PAPINHO_ACCELERATOR_CONNECTION_CONTROL_ACCEPTED ||
        papinho_accel_capability_sets(runtime, &supported, &preference,
        &granted) != 0) return -1;
    if (papacc_capability_negotiate_encode(&supported, &preference,
        runtime->control.output_buffer + PAPACC_FRAME_BASE_HEADER_SIZE,
        sizeof(runtime->control.output_buffer) - PAPACC_FRAME_BASE_HEADER_SIZE,
        &payload_written) != PAPACC_RESULT_OK) return -1;
    header = papacc_capability_negotiate_frame_header((PAPACC_U32)payload_written);
    if (papacc_frame_header_encode(&header, runtime->control.output_buffer,
        sizeof(runtime->control.output_buffer), &header_written) !=
        PAPACC_RESULT_OK) return -1;
    runtime->control.output_begin = 0;
    runtime->control.output_end = header_written + payload_written;
    return papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CAPABILITY_NEGOTIATION_SENT);
}

static int papinho_accel_control_queue_ticket_request(
    papinho_accelerator_client_runtime *runtime) {
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_SIZE written = 0;
    if (runtime == NULL || runtime->state !=
        PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED) return -1;
    header = papacc_data_ticket_request_frame_header();
    if (papacc_frame_header_encode(&header, runtime->control.output_buffer,
        sizeof(runtime->control.output_buffer), &written) != PAPACC_RESULT_OK ||
        written != PAPACC_FRAME_BASE_HEADER_SIZE) return -1;
    runtime->control.output_begin = 0;
    runtime->control.output_end = written;
    return 0;
}

static int papinho_accel_data_queue_attach(
    papinho_accelerator_client_runtime *runtime) {
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_SIZE payload_written = 0;
    PAPACC_SIZE header_written = 0;
    if (runtime == NULL || runtime->state !=
        PAPINHO_ACCELERATOR_CONNECTION_DATA_CONNECTING ||
        !runtime->data_ticket_valid || runtime->data_ticket_consumed ||
        runtime->data_ticket_session_generation != runtime->session_generation)
        return -1;
    header = papacc_data_attach_frame_header();
    if (papacc_data_attach_encode(&runtime->data_ticket,
        runtime->data.output_buffer + PAPACC_FRAME_BASE_HEADER_SIZE,
        sizeof(runtime->data.output_buffer) - PAPACC_FRAME_BASE_HEADER_SIZE,
        &payload_written) != PAPACC_RESULT_OK ||
        papacc_frame_header_encode(&header, runtime->data.output_buffer,
        sizeof(runtime->data.output_buffer), &header_written) != PAPACC_RESULT_OK)
        return -1;
    runtime->data.output_begin = 0;
    runtime->data.output_end = header_written + payload_written;
    return 0;
}

static int papinho_accel_control_feed(
    papinho_accelerator_client_runtime *runtime, const PAPACC_U8 *input,
    PAPACC_SIZE input_length) {
    PAPACC_SIZE offset = 0;
    if (runtime == NULL || (input_length != 0 && input == NULL)) return -1;
    while (offset < input_length) {
        PAPACC_FRAME_PARSER_EVENT event = PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
        PAPACC_SIZE consumed = 0;
        PAPACC_RESULT result = papacc_frame_parser_feed(&runtime->control.parser,
            input + offset, input_length - offset, &consumed, &event);
        if (result != PAPACC_RESULT_OK || consumed == 0) return papinho_accel_control_fail(
            runtime, PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
        offset += consumed;
        if (event.type == PAPACC_FRAME_PARSER_EVENT_NONE) continue;
        if (event.type == PAPACC_FRAME_PARSER_EVENT_HEADER_READY) {
            if (!((runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT &&
                event.header.message_type == PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT &&
                event.header.payload_length == PAPACC_CONTROL_VERSION_PAYLOAD_SIZE) ||
                (runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CAPABILITY_NEGOTIATION_SENT &&
                event.header.message_type == PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT) ||
                (runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED &&
                event.header.message_type == PAPACC_MESSAGE_TYPE_DATA_TICKET &&
                event.header.payload_length == PAPACC_DATA_ASSOCIATION_TICKET_SIZE)) ||
                event.header.flags != 0) return papinho_accel_control_fail(runtime,
                    PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
            runtime->control_pending_header = event.header;
            runtime->control_pending_frame = 1;
            runtime->control.input_count = 0;
            continue;
        }
        if (event.type == PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK ||
            event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
            PAPACC_CONTROL_PROTOCOL_VERSION version =
                PAPACC_CONTROL_PROTOCOL_VERSION_INITIALIZER;
            if (!runtime->control_pending_frame || event.payload == NULL ||
                runtime->control.input_count + event.payload_length >
                sizeof(runtime->control.input_buffer)) return papinho_accel_control_fail(
                    runtime, PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
            memcpy(runtime->control.input_buffer + runtime->control.input_count,
                event.payload, event.payload_length);
            runtime->control.input_count += event.payload_length;
            if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
                if (runtime->control_pending_header.message_type ==
                    PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT) {
                    if (papacc_control_accept_decode(runtime->control.input_buffer,
                        runtime->control.input_count, &version) != PAPACC_RESULT_OK ||
                        version.major != PAPACC_CONTROL_PROTOCOL_MAJOR ||
                        version.minor != PAPACC_CONTROL_PROTOCOL_MINOR ||
                        papinho_accelerator_client_transition(runtime,
                        PAPINHO_ACCELERATOR_CONNECTION_CONTROL_ACCEPTED) != 0)
                        return papinho_accel_control_fail(runtime,
                            PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                } else if (runtime->control_pending_header.message_type ==
                    PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT) {
                    PAPACC_CAPABILITY_SET supported;
                    PAPACC_CAPABILITY_SET preference;
                    PAPACC_CAPABILITY_SET granted;
                    PAPACC_CAPABILITY_KEY network_v1 =
                        PAPACC_PACC_NETWORK_EGRESS_V1_KEY_INITIALIZER;
                    PAPACC_CAPABILITY_KEY network_v2 =
                        PAPACC_PACC_NETWORK_EGRESS_V2_KEY_INITIALIZER;
                    if (papinho_accel_capability_sets(runtime, &supported,
                        &preference, &granted) != 0 ||
                        papacc_capability_negotiate_result_decode(
                        runtime->control.input_buffer, runtime->control.input_count,
                        &granted) != PAPACC_RESULT_OK ||
                        papinho_accelerator_client_transition(runtime,
                        PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED) != 0)
                        return papinho_accel_control_fail(runtime,
                            PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                    runtime->server_granted_count = granted.count;
                    runtime->network_egress_major = 0U;
                    if (papinho_accelerator_client_capability_granted(runtime,
                        &network_v2)) runtime->network_egress_major = 2U;
                    else if (papinho_accelerator_client_capability_granted(runtime,
                        &network_v1)) runtime->network_egress_major = 1U;
                } else {
                    PAPACC_DATA_ASSOCIATION_TICKET ticket =
                        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
                    if (papacc_data_ticket_decode(runtime->control.input_buffer,
                        runtime->control.input_count, &ticket) != PAPACC_RESULT_OK ||
                        papinho_accelerator_client_set_ticket(runtime, &ticket) != 0 ||
                        papinho_accelerator_client_transition(runtime,
                        PAPINHO_ACCELERATOR_CONNECTION_DATA_TICKET_AVAILABLE) != 0)
                        return papinho_accel_control_fail(runtime,
                            PAPINHO_ACCELERATOR_ERROR_TICKET);
                }
                runtime->control_pending_frame = 0;
                runtime->control.input_count = 0;
            }
        }
    }
    return 0;
}

static int papinho_accel_data_feed(papinho_accelerator_client_runtime *runtime,
    const PAPACC_U8 *input, PAPACC_SIZE input_length) {
    PAPACC_SIZE offset = 0;
    if (runtime == NULL || (input_length != 0 && input == NULL)) return -1;
    while (offset < input_length) {
        PAPACC_FRAME_PARSER_EVENT event = PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
        PAPACC_SIZE consumed = 0;
        if (papacc_frame_parser_feed(&runtime->data.parser, input + offset,
            input_length - offset, &consumed, &event) != PAPACC_RESULT_OK ||
            consumed == 0) return papinho_accel_data_fail(runtime,
                PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
        offset += consumed;
        if (event.type == PAPACC_FRAME_PARSER_EVENT_NONE) continue;
        if (event.type == PAPACC_FRAME_PARSER_EVENT_HEADER_READY) {
            if (runtime->state != PAPINHO_ACCELERATOR_CONNECTION_DATA_CONNECTING ||
                event.header.message_type != PAPACC_MESSAGE_TYPE_DATA_ACCEPT ||
                event.header.payload_length != 0 || event.header.flags != 0)
                return papinho_accel_data_fail(runtime,
                    PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH);
            runtime->data_pending_header = event.header;
            runtime->data_pending_frame = 1;
            continue;
        }
        if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
            if (event.header.message_type != PAPACC_MESSAGE_TYPE_DATA_ACCEPT ||
                event.header.payload_length != 0 ||
                papinho_accelerator_client_consume_ticket(runtime) != 0 ||
                papinho_accelerator_client_transition(runtime,
                PAPINHO_ACCELERATOR_CONNECTION_DATA_ATTACHED) != 0)
                return papinho_accel_data_fail(runtime,
                    PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH);
            runtime->data_pending_frame = 0;
        }
    }
    return 0;
}

static int papinho_accel_control_send_pending(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    PAPACC_SIZE remaining;
    if (runtime == NULL || runtime->control.output_end <
        runtime->control.output_begin) return -1;
    remaining = runtime->control.output_end - runtime->control.output_begin;
    if (remaining != 0 && papinho_accel_stream_send_all(&runtime->control,
        runtime->control.output_buffer + runtime->control.output_begin,
        (int)remaining, config, cancel_event) != 0) return -1;
    runtime->control.output_begin = 0;
    runtime->control.output_end = 0;
    return 0;
}

static int papinho_accel_data_send_pending(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    PAPACC_SIZE remaining;
    if (runtime == NULL || runtime->data.output_end < runtime->data.output_begin)
        return -1;
    remaining = runtime->data.output_end - runtime->data.output_begin;
    if (remaining != 0 && papinho_accel_stream_send_all(&runtime->data,
        runtime->data.output_buffer + runtime->data.output_begin, (int)remaining,
        config, cancel_event) != 0) return -1;
    runtime->data.output_begin = 0;
    runtime->data.output_end = 0;
    return 0;
}

static int papinho_accel_control_receive_accept(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    PAPACC_U8 bytes[256];
    while (runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT ||
        runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CAPABILITY_NEGOTIATION_SENT) {
        int rc;
        rc = papinho_accel_stream_receive(&runtime->control, bytes, sizeof(bytes),
            config, cancel_event);
        if (rc == -2) return papinho_accel_control_fail(runtime,
            papinho_accel_cancelled(cancel_event) ? PAPINHO_ACCELERATOR_ERROR_CANCELLED :
            PAPINHO_ACCELERATOR_ERROR_TIMEOUT);
        if (rc == 0) return papinho_accel_control_fail(runtime,
            PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE);
        if (rc < 0) return papinho_accel_control_fail(runtime,
            PAPINHO_ACCELERATOR_ERROR_TRANSPORT);
        if (papinho_accel_control_feed(runtime, bytes, (PAPACC_SIZE)rc) != 0)
            return -1;
    }
    return (runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CONTROL_ACCEPTED ||
        runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED) ?
        0 : -1;
}

static int papinho_accel_control_receive_ticket(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    PAPACC_U8 bytes[256];
    while (runtime->state == PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED) {
        int rc;
        rc = papinho_accel_stream_receive(&runtime->control, bytes, sizeof(bytes),
            config, cancel_event);
        if (rc == -2) return papinho_accel_control_fail(runtime,
            papinho_accel_cancelled(cancel_event) ? PAPINHO_ACCELERATOR_ERROR_CANCELLED :
            PAPINHO_ACCELERATOR_ERROR_TIMEOUT);
        if (rc == 0) return papinho_accel_control_fail(runtime,
            PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE);
        if (rc < 0 || papinho_accel_control_feed(runtime, bytes,
            (PAPACC_SIZE)rc) != 0) return -1;
    }
    return runtime->state == PAPINHO_ACCELERATOR_CONNECTION_DATA_TICKET_AVAILABLE ?
        0 : -1;
}

static int papinho_accel_data_receive_accept(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    PAPACC_U8 bytes[256];
    while (runtime->state == PAPINHO_ACCELERATOR_CONNECTION_DATA_CONNECTING) {
        int rc;
        rc = papinho_accel_stream_receive(&runtime->data, bytes, sizeof(bytes),
            config, cancel_event);
        if (rc == -2) return papinho_accel_data_fail(runtime,
            papinho_accel_cancelled(cancel_event) ? PAPINHO_ACCELERATOR_ERROR_CANCELLED :
            PAPINHO_ACCELERATOR_ERROR_TIMEOUT);
        if (rc == 0) return papinho_accel_data_fail(runtime,
            PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE);
        if (rc < 0 || papinho_accel_data_feed(runtime, bytes,
            (PAPACC_SIZE)rc) != 0) return -1;
    }
    return runtime->state == PAPINHO_ACCELERATOR_CONNECTION_DATA_ATTACHED ? 0 : -1;
}

int papinho_accelerator_client_parse_endpoint(const char *text,
    papinho_accelerator_client_config *out_config) {
    const char *colon;
    char *end;
    unsigned long port;
    size_t host_length;
    if (text == NULL || out_config == NULL || text[0] == '\0') return -1;
    colon = strrchr(text, ':');
    if (colon == NULL || colon == text || colon[1] == '\0') return -1;
    host_length = (size_t)(colon - text);
    if (host_length >= sizeof(out_config->host)) return -1;
    port = strtoul(colon + 1, &end, 10);
    if (*end != '\0' || port == 0 || port > 65535UL) return -1;
    ZeroMemory(out_config, sizeof(*out_config));
    memcpy(out_config->host, text, host_length);
    out_config->host[host_length] = '\0';
    out_config->port = (unsigned short)port;
    out_config->timeout_ms = 2000;
    /* New Browser sessions probe v2 first; one v1 Session is permitted only
     * after a valid v2 capability result and before egress. */
    out_config->network_egress_major = 2U;
    out_config->network_egress_v1_fallback_allowed = 1;
    /* Endpoint syntax has no transport authority. Caller selects profile. */
    return 0;
}

static void papinho_accel_fixture_capabilities_for_major(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, unsigned short major) {
    PAPACC_CAPABILITY_KEY network = PAPACC_PACC_NETWORK_EGRESS_V2_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY tls = PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    if (runtime == NULL || config == NULL || config->profile !=
        PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL ||
        (major != 1U && major != 2U)) return;
    network.major = (PAPACC_CAPABILITY_VERSION)major;
    runtime->client_supported[0] = network; runtime->client_supported[1] = tls;
    runtime->client_supported_count = 2;
    runtime->client_preference[0] = network; runtime->client_preference[1] = tls;
    runtime->client_preference_count = 2;
}

static void papinho_accel_fixture_capabilities(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config) {
    unsigned short major = 2U;
    if (config != NULL && config->network_egress_major == 1U) major = 1U;
    papinho_accel_fixture_capabilities_for_major(runtime, config, major);
}

static int papinho_accel_control_establish(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_secure_stage_trace *trace) {
    if (runtime == NULL || config == NULL) return -1;
    if (papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CONNECTING) != 0) return -1;

    runtime->control.socket = papinho_accel_connect(config, cancel_event);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    runtime->control.candidate_group=config->candidate_group;
#endif

    if (runtime->control.socket == INVALID_SOCKET) {
        runtime->error = papinho_accel_cancelled(cancel_event) ?
            PAPINHO_ACCELERATOR_ERROR_CANCELLED : PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE;
        return -1;
    }
    runtime->control.socket_open = 1;
    if (papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_TRANSPORT_CONNECTED) != 0 ||
        (config->profile == PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL &&
        (papinho_accel_secure_attach(&runtime->control, config, cancel_event, trace) != 0 ||
        papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_SECURE_TRANSPORT_READY) != 0))) {
        if (runtime->error == PAPINHO_ACCELERATOR_ERROR_NONE)
            runtime->error = PAPINHO_ACCELERATOR_ERROR_SECURE;
        return -1;
    }
    if (papinho_accel_control_queue_open(runtime) != 0 ||
        papinho_accel_control_send_pending(runtime, config, cancel_event) != 0) {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_CONTROL_OPEN, PST_RESULT_PROTOCOL_FAILURE,
            runtime->error, "control_open_send");
        return -1;
    }
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_CONTROL_OPEN, "control_open_send");
    if (papinho_accel_control_receive_accept(runtime, config, cancel_event) != 0) {
        papinho_accel_secure_stage_failure(trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_CONTROL_ACCEPT, PST_RESULT_PROTOCOL_FAILURE,
            runtime->error, "control_accept_receive");
        return -1;
    }
    papinho_accel_secure_stage_success(trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_CONTROL_ACCEPT, "control_accept_receive");
    if (papinho_accel_control_queue_negotiate(runtime) != 0 ||
        papinho_accel_control_send_pending(runtime, config, cancel_event) != 0 ||
        papinho_accel_control_receive_accept(runtime, config, cancel_event) != 0)
        return -1;
    return 0;
}

static int papinho_accel_control_request_ticket(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    if (runtime == NULL || config == NULL || runtime->state !=
        PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED) return -1;
    if (papinho_accel_control_queue_ticket_request(runtime) != 0 ||
        papinho_accel_control_send_pending(runtime, config, cancel_event) != 0 ||
        papinho_accel_control_receive_ticket(runtime, config, cancel_event) != 0) {
        runtime->error = PAPINHO_ACCELERATOR_ERROR_TICKET;
        return -1;
    }
    return 0;
}

static int papinho_accel_data_with_ticket(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event) {
    if (runtime == NULL || config == NULL ||
        papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_DATA_CONNECTING) != 0) return -1;

    runtime->data.socket = papinho_accel_connect(config, cancel_event);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    runtime->data.candidate_group=config->candidate_group;
#endif
    if (runtime->data.socket == INVALID_SOCKET) {
        runtime->error = PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH; return -1;
    }
    runtime->data.socket_open = 1;
    if (config->profile == PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL &&
        papinho_accel_secure_attach(&runtime->data, config, cancel_event, NULL) != 0) {
        runtime->error = PAPINHO_ACCELERATOR_ERROR_SECURE; return -1;
    }
    if (papinho_accel_data_queue_attach(runtime) != 0 ||
        papinho_accel_data_send_pending(runtime, config, cancel_event) != 0 ||
        papinho_accel_data_receive_accept(runtime, config, cancel_event) != 0 ||
        papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_READY) != 0) {
        runtime->error = PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH; return -1;
    }
    runtime->session_valid = 1;
    return 0;
}

static void papinho_accel_fixture_detail(char *detail, size_t detail_size,
    int rc, papinho_accelerator_error error) {
    const char *message;
    if (detail == NULL || detail_size == 0) return;
    message = rc == 0 ? "CONTROL/DATA bound" :
        (error == PAPINHO_ACCELERATOR_ERROR_CANCELLED ? "cancelled" :
        (error == PAPINHO_ACCELERATOR_ERROR_SECURE ? "secure transport unavailable" :
        (error == PAPINHO_ACCELERATOR_ERROR_TIMEOUT ? "timeout" :
        (error == PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH ? "DATA attach failed" :
        (error == PAPINHO_ACCELERATOR_ERROR_TICKET ? "DATA ticket failed" :
        (error == PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE ? "remote secure close" :
        (error == PAPINHO_ACCELERATOR_ERROR_CAPABILITY ?
            "required capability not granted" : "connect/protocol failure")))))));
    strncpy(detail, message, detail_size - 1);
    detail[detail_size - 1] = '\0';
}

typedef struct papinho_accel_fixture_blob { pst_u8 *data; pst_size size; } papinho_accel_fixture_blob;
static int papinho_accel_fixture_load(const char *path, papinho_accel_fixture_blob *blob) {
    FILE *f; long n; char *text; pst_u8 *out; size_t i, used=0; unsigned long acc=0; int bits=0;
    if (path==NULL || blob==NULL) return 0; ZeroMemory(blob,sizeof(*blob)); f=fopen(path,"rb"); if(f==NULL) return 0;
    if(fseek(f,0,SEEK_END)!=0 || (n=ftell(f))<=0 || fseek(f,0,SEEK_SET)!=0) { fclose(f); return 0; }
    text=(char*)malloc((size_t)n); out=(pst_u8*)malloc(((size_t)n*3U)/4U+4U);
    if(text==NULL || out==NULL || fread(text,1,(size_t)n,f)!=(size_t)n) { free(text); free(out); fclose(f); return 0; } fclose(f);
    for(i=0;i<(size_t)n;i++){ int x=-1, ch=(unsigned char)text[i]; if(ch>='A'&&ch<='Z')x=ch-'A'; else if(ch>='a'&&ch<='z')x=ch-'a'+26; else if(ch>='0'&&ch<='9')x=ch-'0'+52; else if(ch=='+')x=62; else if(ch=='/')x=63; else if(ch=='=')break; else continue; acc=(acc<<6)|(unsigned long)x; bits+=6; if(bits>=8){bits-=8;out[used++]=(pst_u8)((acc>>bits)&255U);} }
    free(text); if(used==0){free(out);return 0;} blob->data=out;blob->size=(pst_size)used;return 1;
}
static int papinho_accel_secure_provider_prepare(void) {
    return pst_win32_register_retrozilla_nss() == PST_RESULT_OK ? 0 : -1;
}

int papinho_accelerator_client_fixture_secure_provider_prepare(void) {
    return papinho_accel_secure_provider_prepare();
}

static void papinho_accel_response_storage_release(papinho_accelerator_response_storage *storage) {
    if (storage == NULL) return;
    free(storage->data);
    ZeroMemory(storage, sizeof(*storage));
}

static int papinho_accel_response_storage_append(papinho_accelerator_response_storage *storage,
    const PAPACC_U8 *data, PAPACC_SIZE size) {
    PAPACC_SIZE required, capacity, next_capacity; PAPACC_U8 *grown;
    if (storage == NULL || data == NULL || size == 0U || storage->max_size == 0U) return -1;
    if (size > storage->max_size - storage->size) return -1;
    required = storage->size + size; capacity = storage->capacity;
    if (required > capacity) {
        if (capacity == 0U) capacity = PAPINHO_ACCELERATOR_PERSISTENT_RESPONSE_INITIAL;
        while (capacity < required) {
            if (capacity >= storage->max_size) return -1;
            next_capacity = capacity > storage->max_size / 2U ? storage->max_size : capacity * 2U;
            if (next_capacity <= capacity) return -1;
            capacity = next_capacity;
        }
        grown = (PAPACC_U8 *)realloc(storage->data, (size_t)capacity);
        if (grown == NULL) return -1;
        storage->data = grown; storage->capacity = capacity;
    }
    memcpy(storage->data + storage->size, data, (size_t)size);
    storage->size = required;
    return 0;
}
static int papinho_accel_http_ci_equal_at(const PAPACC_U8 *data,
    PAPACC_SIZE data_size, PAPACC_SIZE offset, const char *text) {
    PAPACC_SIZE i;
    if (data == NULL || text == NULL) return 0;
    for (i = 0U; text[i] != '\0'; ++i) {
        unsigned char a, b;
        if (offset + i >= data_size) return 0;
        a = (unsigned char)data[offset + i]; b = (unsigned char)text[i];
        if (a >= 'A' && a <= 'Z') a = (unsigned char)(a + ('a' - 'A'));
        if (b >= 'A' && b <= 'Z') b = (unsigned char)(b + ('a' - 'A'));
        if (a != b) return 0;
    }
    return 1;
}

int papinho_http_find_header_body_split(const PAPACC_U8 *data,
    PAPACC_SIZE size, PAPACC_SIZE *out_size) {
    PAPACC_SIZE i;
    if (data == NULL || out_size == NULL) return -1;
    for (i = 0U; i + 3U < size; ++i) {
        if (data[i] == '\r' && data[i + 1U] == '\n' &&
            data[i + 2U] == '\r' && data[i + 3U] == '\n') {
            *out_size = i + 4U; return 1;
        }
    }
    return 0;
}

static int papinho_accel_http_contains_ci(const PAPACC_U8 *data,
    PAPACC_SIZE size, const char *needle) {
    PAPACC_SIZE i;
    if (data == NULL || needle == NULL) return 0;
    for (i = 0U; i < size; ++i)
        if (papinho_accel_http_ci_equal_at(data, size, i, needle)) return 1;
    return 0;
}

/* Matches the existing Browser Content-Length authority: the first recognised
 * field controls final extraction; no duplicate-field policy is invented here. */
int papinho_http_parse_content_length(const PAPACC_U8 *data,
    PAPACC_SIZE header_size, PAPACC_SIZE *out_length) {
    static const char name[] = "content-length:";
    PAPACC_SIZE i, p, value;
    int found;
    if (data == NULL || out_length == NULL) return -1;
    found = 0;
    for (i = 0U; i < header_size; ++i) {
        if (i != 0U && !(i >= 2U && data[i - 2U] == '\r' && data[i - 1U] == '\n'))
            continue;
        if (!papinho_accel_http_ci_equal_at(data, header_size, i, name)) continue;
        p = i + (PAPACC_SIZE)(sizeof(name) - 1U);
        while (p < header_size && (data[p] == ' ' || data[p] == '\t')) ++p;
        if (p >= header_size || data[p] < '0' || data[p] > '9') return -1;
        value = 0U;
        while (p < header_size && data[p] >= '0' && data[p] <= '9') {
            PAPACC_SIZE digit = (PAPACC_SIZE)(data[p] - '0');
            if (value > (((PAPACC_SIZE)~(PAPACC_SIZE)0) - digit) / 10U) return -1;
            value = value * 10U + digit; ++p;
        }
        *out_length = value; found = 1; break;
    }
    return found;
}

/* The completion scan uses precisely the Browser decoder's existing rule:
 * chunk extensions are ignored and the zero-sized line is terminal. */
static int papinho_accel_http_chunked_complete(const PAPACC_U8 *data,
    PAPACC_SIZE size, PAPACC_SIZE body_offset, PAPACC_SIZE *out_offset) {
    PAPACC_SIZE pos;
    if (data == NULL || out_offset == NULL || body_offset > size) return -1;
    pos = body_offset;
    for (;;) {
        PAPACC_SIZE line_start, line_end, i, chunk_size;
        int hex_seen;
        line_start = pos;
        while (pos + 1U < size && !(data[pos] == '\r' && data[pos + 1U] == '\n')) ++pos;
        if (pos + 1U >= size) return 0;
        line_end = pos; pos += 2U;
        if (line_end == line_start) return -1;
        chunk_size = 0U; hex_seen = 0;
        for (i = line_start; i < line_end; ++i) {
            unsigned char c = data[i]; unsigned int value;
            if (c == ';') break;
            if (c >= '0' && c <= '9') value = (unsigned int)(c - '0');
            else if (c >= 'a' && c <= 'f') value = (unsigned int)(c - 'a' + 10U);
            else if (c >= 'A' && c <= 'F') value = (unsigned int)(c - 'A' + 10U);
            else return -1;
            if (chunk_size > (((PAPACC_SIZE)~(PAPACC_SIZE)0) - value) / 16U) return -1;
            chunk_size = chunk_size * 16U + (PAPACC_SIZE)value; hex_seen = 1;
        }
        if (!hex_seen) return -1;
        if (chunk_size == 0U) {
            /* The zero chunk is not enough: consume complete trailers and
             * their terminating empty line before declaring HTTP complete. */
            for (;;) {
                PAPACC_SIZE trailer_start = pos, trailer_end;
                int colon_seen = 0;
                while (pos + 1U < size && !(data[pos] == '\r' &&
                    data[pos + 1U] == '\n')) ++pos;
                if (pos + 1U >= size) return 0;
                trailer_end = pos; pos += 2U;
                if (trailer_end == trailer_start) {
                    *out_offset = pos; return 1;
                }
                for (i = trailer_start; i < trailer_end; ++i)
                    if (data[i] == ':') { colon_seen = 1; break; }
                if (!colon_seen) return -1;
            }
        }
        if (chunk_size > size - pos || size - pos - chunk_size < 2U) return 0;
        pos += chunk_size;
        if (data[pos] != '\r' || data[pos + 1U] != '\n') return -1;
        pos += 2U;
    }
}

static int papinho_accel_http_framing_progress(
    papinho_accelerator_mode_b_request *request) {
    papinho_accelerator_http_framing *framing;
    PAPACC_SIZE chunk_offset, body_size;
    int split, content_length;
    if (request == NULL || request->response_bytes == NULL) return -1;
    framing = &request->http_framing;
    if (framing->state == PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE) return 1;
    if (framing->state == PAPINHO_ACCELERATOR_HTTP_FRAMING_ERROR) return -1;
    if (!framing->header_complete) {
        split = papinho_http_find_header_body_split(request->response_bytes,
            request->response_size, &framing->header_size);
        if (split < 0) goto malformed;
        if (split == 0) return 0;
        framing->header_complete = 1;
        framing->transfer_chunked = papinho_accel_http_contains_ci(
            request->response_bytes, framing->header_size,
            "transfer-encoding: chunked");
        content_length = papinho_http_parse_content_length(request->response_bytes,
            framing->header_size, &framing->content_length);
        if (content_length < 0) goto malformed;
        framing->content_length_known = content_length > 0 ? 1 : 0;
        framing->close_delimited = !framing->transfer_chunked &&
            !framing->content_length_known;
    }
    if (request->response_size < framing->header_size) goto malformed;
    body_size = request->response_size - framing->header_size;
    if (framing->transfer_chunked) {
        split = papinho_accel_http_chunked_complete(request->response_bytes,
            request->response_size, framing->header_size, &chunk_offset);
        if (split < 0) goto malformed;
        if (split == 0) return 0;
        framing->chunk_scan_offset = chunk_offset;
        framing->state = PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE;
        return 1;
    }
    if (framing->content_length_known) {
        if (body_size > framing->content_length) goto malformed;
        if (body_size < framing->content_length) return 0;
        framing->state = PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE;
        return 1;
    }
    return 0;
malformed:
    framing->state = PAPINHO_ACCELERATOR_HTTP_FRAMING_ERROR;
    return -1;
}

static int papinho_accel_http_framing_remote_close(
    papinho_accelerator_mode_b_request *request) {
    papinho_accelerator_http_framing *framing;
    if (request == NULL) return -1;
    framing = &request->http_framing;
    if (framing->state == PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE) return 0;
    if (framing->state == PAPINHO_ACCELERATOR_HTTP_FRAMING_ERROR ||
        !framing->header_complete || !framing->close_delimited) {
        framing->state = PAPINHO_ACCELERATOR_HTTP_FRAMING_ERROR;
        return -1;
    }
    framing->state = PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE;
    return 0;
}


static int papinho_accel_persistent_jobs_ensure(
    papinho_accelerator_persistent_session *session) {
    unsigned int i;
    if (session == NULL) return -1;
    if (!session->job_lock_initialized) {
        InitializeCriticalSection(&session->job_lock);
        session->job_lock_initialized = 1;
        session->active_job_slot = -1;
        session->next_job_id = 1U;
    }
    if (session->active_operation_cancel_event == NULL) {
        session->active_operation_cancel_event = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (session->active_operation_cancel_event == NULL) return -1;
    }
    if (session->jobs != NULL) return 0;
    session->jobs = (papinho_accelerator_persistent_request_job *)calloc(
        PAPINHO_ACCELERATOR_PERSISTENT_JOB_QUEUE_LIMIT,
        sizeof(*session->jobs));
    if (session->jobs == NULL) return -1;
    for (i = 0U; i < PAPINHO_ACCELERATOR_PERSISTENT_JOB_QUEUE_LIMIT; ++i)
        session->jobs[i].state = PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_EMPTY;
    return 0;
}

void papinho_accelerator_persistent_session_init(
    papinho_accelerator_persistent_session *session) {
    if (session == NULL) return;
    ZeroMemory(session, sizeof(*session));
    (void)papinho_accel_persistent_jobs_ensure(session);
}

static int papinho_accel_persistent_job_find(
    papinho_accelerator_persistent_session *session, unsigned long job_id) {
    unsigned int i;
    if (session == NULL || session->jobs == NULL || job_id == 0U) return -1;
    for (i = 0U; i < PAPINHO_ACCELERATOR_PERSISTENT_JOB_QUEUE_LIMIT; ++i)
        if (session->jobs[i].job_id == job_id &&
            session->jobs[i].state != PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_EMPTY)
            return (int)i;
    return -1;
}


static void papinho_accel_persistent_jobs_release(
    papinho_accelerator_persistent_session *session) {
    unsigned int i;
    if (session == NULL || !session->job_lock_initialized) return;
    EnterCriticalSection(&session->job_lock);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    /* Teardown runs on the serialized operation owner. A queued probe has
     * never begun resolving credentials; return its independently owned inputs
     * only after notifying its waiter. An executing probe retains its capsule. */
    if (session->control_probe.state == 1) {
        papinho_probe_dependencies *dependencies =
            (papinho_probe_dependencies *)session->control_probe.dependencies;
        session->control_probe.rc = -1;
        session->control_probe.error = PAPINHO_ACCELERATOR_ERROR_CANCELLED;
        (void)papinho_probe_dependencies_complete(dependencies);
        session->control_probe.dependencies = NULL;
        session->control_probe.config.credential_lease = NULL;
        session->control_probe.cancel_event = NULL;
        session->control_probe.completion_event = NULL;
        session->control_probe.state = 3;
    }
#endif
    if (session->jobs != NULL) {
        for (i = 0U; i < PAPINHO_ACCELERATOR_PERSISTENT_JOB_QUEUE_LIMIT; ++i) {
            if (session->jobs[i].cancel_event != NULL)
                CloseHandle(session->jobs[i].cancel_event);
            if (session->jobs[i].completion_event != NULL)
                CloseHandle(session->jobs[i].completion_event);
            papinho_accel_response_storage_release(&session->jobs[i].response_storage);
        }
        free(session->jobs);
        session->jobs = NULL;
    }
    session->job_queue_count = 0U;
    session->active_job_slot = -1;
    LeaveCriticalSection(&session->job_lock);
    if (session->active_operation_cancel_event != NULL) {
        CloseHandle(session->active_operation_cancel_event);
        session->active_operation_cancel_event = NULL;
    }
}
static DWORD WINAPI papinho_accel_persistent_session_wake_bridge(LPVOID parameter) {
    papinho_accelerator_persistent_session *session =
        (papinho_accelerator_persistent_session *)parameter;
    HANDLE events[3]; DWORD count = 2;
    if (session == NULL) return 1;
    events[0] = session->bridge_stop_event;
    events[1] = session->wake_event;
    if (session->cancel_event != NULL) { events[2] = session->cancel_event; count = 3; }
    for (;;) {
        DWORD result = WaitForMultipleObjects(count, events, FALSE, INFINITE);
        if (result == WAIT_OBJECT_0 || result == WAIT_FAILED) break;
        /* wake_event is a coalesced manual-reset job hint.  Consume it before
         * forwarding to the PST wait-set: jobs already submitted remain in the
         * authoritative queue, while a later SetEvent stays pending. */
        if (result == WAIT_OBJECT_0 + 1)
            ResetEvent(session->wake_event);
        if (count == 3 && result == WAIT_OBJECT_0 + 2 &&
            session->active_operation_cancel_event != NULL)
            SetEvent(session->active_operation_cancel_event);
        if ((result == WAIT_OBJECT_0 + 1 || (count == 3 && result == WAIT_OBJECT_0 + 2)) && session->wait_set != NULL)
            (void)pst_wait_set_wake(session->wait_set);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        if(count==3 && result==WAIT_OBJECT_0+2)break;
#endif
    }
    return 0;
}

static void papinho_accel_persistent_session_release_materials(
    papinho_accelerator_persistent_session *session) {
    if (session == NULL) return;
    if (session->secure_runtime != NULL && session->secure_runtime_owned)
        pst_runtime_release(session->secure_runtime);
    if (session->secure_credentials != NULL) pst_credentials_release(session->secure_credentials);
    if (session->secure_trust != NULL) pst_trust_release(session->secure_trust);
    session->secure_runtime = NULL;
    session->secure_runtime_owned = 0;
    session->secure_credentials = NULL;
    session->secure_trust = NULL;
}
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
#include "papinho_candidate_session_teardown.inc"
#endif

void papinho_accelerator_persistent_session_teardown(
    papinho_accelerator_persistent_session *session) {
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    (void)candidate_session_teardown(session);
#else
    if (session == NULL) return;
    if (session->bridge_stop_event != NULL) SetEvent(session->bridge_stop_event);
    if (session->wake_event != NULL) SetEvent(session->wake_event);
    if (session->bridge_thread != NULL) {
        (void)WaitForSingleObject(session->bridge_thread, INFINITE);
        CloseHandle(session->bridge_thread);
        session->bridge_thread = NULL;
    }
    if (session->wait_set != NULL) {
        if (session->runtime.control.secure_connection != NULL)
            (void)pst_wait_set_remove_connection(session->wait_set,
                session->runtime.control.secure_connection);
        if (session->runtime.data.secure_connection != NULL)
            (void)pst_wait_set_remove_connection(session->wait_set,
                session->runtime.data.secure_connection);
        (void)pst_wait_set_destroy(session->wait_set);
        session->wait_set = NULL;
    }
    if (session->bridge_stop_event != NULL) CloseHandle(session->bridge_stop_event);
    if (session->wake_event != NULL) CloseHandle(session->wake_event);
    session->bridge_stop_event = NULL;
    session->wake_event = NULL;
    session->cancel_event = NULL;
    papinho_accel_persistent_jobs_release(session);
    papinho_accelerator_client_teardown(&session->runtime);
    papinho_accel_persistent_session_release_materials(session);
    session->established = 0;
    session->observing = 0;
    session->owner_thread_id = 0;
#endif
}

int papinho_accelerator_persistent_session_establish(
    papinho_accelerator_persistent_session *session,
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_error *out_error) {
    papinho_accel_fixture_blob ca, cert, key;
    PST_DER_ITEM anchor, leaf;
    PST_TRUST_SOURCE trust_source;
    PST_CREDENTIAL_SOURCE credential_source;
    PST_RUNTIME_OPTIONS runtime_options;
    papinho_accelerator_client_config attempt;
    const char *ca_path;
    const char *certificate_path;
    const char *key_path;
    int rc = -1;
    PST_RESULT pst_result;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (session != NULL) papinho_accel_secure_stage_begin(&session->secure_stage_trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_CONFIG_VALIDATE, "config_validate");
    if (session == NULL || config == NULL || session->established ||
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        !candidate_storage(session) || root_read(&candidate_storage(session)->state)!=SESSION_ACTIVE ||
#endif
        (!config->fixture_secure_materials_enabled && !config->secure_materials_enabled && !config->credential_reference[0]) ||
        config->profile != PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL ||
        papinho_accel_persistent_jobs_ensure(session) != 0) {
        if (session != NULL) papinho_accel_secure_stage_failure(&session->secure_stage_trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_CONFIG_VALIDATE, PST_RESULT_INVALID_ARGUMENT,
            PAPINHO_ACCELERATOR_ERROR_PROTOCOL, "config_validate");
        return -1;
    }
    papinho_accel_secure_stage_success(&session->secure_stage_trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_CONFIG_VALIDATE, "config_validate");
    ZeroMemory(&ca, sizeof(ca)); ZeroMemory(&cert, sizeof(cert));
    ZeroMemory(&key, sizeof(key)); ZeroMemory(&attempt, sizeof(attempt));
    if (config->credential_reference[0]) {
        if (!config->credential_resolve || !config->credential_resolve(
            config->credential_lease,config->credential_reference,config->secure_peer_name,
            &session->secure_trust,&session->secure_credentials)) goto done;
        goto profile_materials_ready;
    }
    ca_path = config->secure_materials_enabled ? config->secure_ca_path : config->fixture_ca_path;
    certificate_path = config->secure_materials_enabled ? config->secure_certificate_path : config->fixture_certificate_path;
    key_path = config->secure_materials_enabled ? config->secure_key_path : config->fixture_key_path;
    if (!papinho_accel_fixture_load(ca_path, &ca) ||
        !papinho_accel_fixture_load(certificate_path, &cert) ||
        !papinho_accel_fixture_load(key_path, &key)) {
        papinho_accel_secure_stage_failure(&session->secure_stage_trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_CONFIG_VALIDATE, PST_RESULT_UNAVAILABLE,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "secure_material_load");
        goto done;
    }
    anchor.data = ca.data; anchor.size = ca.size;
    ZeroMemory(&trust_source, sizeof(trust_source));
    trust_source.struct_size = sizeof(trust_source);
    trust_source.api_version = PST_API_VERSION;
    trust_source.kind = PST_TRUST_SOURCE_CUSTOM_CA_DER;
    trust_source.anchors = &anchor; trust_source.anchor_count = 1;
    pst_result = pst_trust_create(&trust_source, &session->secure_trust);
    if (pst_result != PST_RESULT_OK) {
        papinho_accel_secure_stage_failure(&session->secure_stage_trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_TRUST_CONFIGURE, pst_result,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_trust_create");
        goto done;
    }
    papinho_accel_secure_stage_success(&session->secure_stage_trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_TRUST_CONFIGURE, "pst_trust_create");
    leaf.data = cert.data; leaf.size = cert.size;
    ZeroMemory(&credential_source, sizeof(credential_source));
    credential_source.struct_size = sizeof(credential_source);
    credential_source.api_version = PST_API_VERSION;
    credential_source.kind = PST_CREDENTIAL_SOURCE_CERT_CHAIN_DER_PKCS8_DER;
    credential_source.certificate_chain = &leaf;
    credential_source.certificate_count = 1;
    credential_source.private_key_der = key.data;
    credential_source.private_key_der_size = key.size;
    papinho_accel_secure_stage_success(&session->secure_stage_trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_CLIENT_CERT_CONFIGURE, "credential_source.certificate");
    pst_result = pst_credentials_create(&credential_source, &session->secure_credentials);
    if (pst_result != PST_RESULT_OK) {
        papinho_accel_secure_stage_failure(&session->secure_stage_trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_CLIENT_KEY_CONFIGURE, pst_result,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_credentials_create");
        goto done;
    }
    papinho_accel_secure_stage_success(&session->secure_stage_trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_CLIENT_KEY_CONFIGURE, "pst_credentials_create");
profile_materials_ready:
    /* PST seals the provider registry at runtime creation.  Every Secure
     * Principal path must therefore register the exact provider first. */
    pst_result = pst_win32_register_retrozilla_nss();
    if (pst_result != PST_RESULT_OK) {
        papinho_accel_secure_stage_failure(&session->secure_stage_trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_PROVIDER_ACQUIRE, pst_result,
            PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_win32_register_retrozilla_nss");
        goto done;
    }
    papinho_accel_secure_stage_success(&session->secure_stage_trace,
        PAPINHO_ACCELERATOR_SECURE_STAGE_PROVIDER_ACQUIRE, "pst_win32_register_retrozilla_nss");
    ZeroMemory(&runtime_options, sizeof(runtime_options));
    runtime_options.struct_size = sizeof(runtime_options);
    runtime_options.api_version = PST_API_VERSION;
    if (config->secure_runtime != NULL) {
        session->secure_runtime = config->secure_runtime;
        session->secure_runtime_owned = 0;
        papinho_accel_secure_stage_success(&session->secure_stage_trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_RUNTIME_CREATE, "shared_pst_runtime");
    } else {
        pst_result = pst_runtime_create(&runtime_options, &session->secure_runtime);
        if (pst_result != PST_RESULT_OK) {
            papinho_accel_secure_stage_failure(&session->secure_stage_trace,
                PAPINHO_ACCELERATOR_SECURE_STAGE_RUNTIME_CREATE, pst_result,
                PAPINHO_ACCELERATOR_ERROR_SECURE, "pst_runtime_create");
            goto done;
        }
        session->secure_runtime_owned = 1;
        papinho_accel_secure_stage_success(&session->secure_stage_trace,
            PAPINHO_ACCELERATOR_SECURE_STAGE_RUNTIME_CREATE, "pst_runtime_create");
    }
    attempt = *config;
    attempt.secure_runtime = session->secure_runtime;
    attempt.secure_trust = session->secure_trust;
    attempt.secure_credentials = session->secure_credentials;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!candidate_adopt(session,config))goto done;
    attempt.candidate_group=candidate_storage(session)->group;
#endif
    papinho_accelerator_client_runtime_init(&session->runtime, attempt.profile);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
candidate_control_establish:
#endif
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(config->candidate_c2){
        PAPACC_CAPABILITY_KEY tls=PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
        PAPACC_CAPABILITY_KEY c2={3U,1U};
        session->runtime.client_supported[0]=tls;session->runtime.client_supported[1]=c2;
        session->runtime.client_preference[0]=tls;session->runtime.client_preference[1]=c2;
        session->runtime.client_supported_count=2U;session->runtime.client_preference_count=2U;
    }else
#endif
    papinho_accel_fixture_capabilities(&session->runtime, &attempt);
    if (papinho_accel_control_establish(&session->runtime, &attempt, cancel_event,
        &session->secure_stage_trace) != 0)
        goto done;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(config->candidate_c2){
        PAPACC_CAPABILITY_KEY tls=PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
        PAPACC_CAPABILITY_KEY c2={3U,1U};
        if(session->runtime.server_granted_count!=2U ||
           !papinho_accelerator_client_capability_granted(&session->runtime,&tls) ||
           !papinho_accelerator_client_capability_granted(&session->runtime,&c2)){
            session->runtime.error=PAPINHO_ACCELERATOR_ERROR_CAPABILITY;goto done;
        }
    }else
#endif
    if (!papinho_accelerator_client_mode_b_requirements_granted(&session->runtime)) {
        PAPACC_CAPABILITY_KEY tls = PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
        /* A valid v2 probe may choose exactly one fresh v1 Session before
         * any ticket or egress.  No transport/auth/protocol failure reaches
         * this branch because control_establish already succeeded. */
        if (session->runtime.network_egress_major == 0U &&
            papinho_accelerator_client_capability_granted(&session->runtime, &tls) &&
            config->network_egress_v1_fallback_allowed &&
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
            attempt.network_egress_major != 1U &&
#endif
            config->network_egress_major != 1U) {
            papinho_accelerator_client_config retry = *config;
            retry.network_egress_major = 1U;
            retry.network_egress_v1_fallback_allowed = 0;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
            /* Establishment has not published sources, started a bridge or
             * registered a set. All synchronous CONTROL calls have returned. */
            {
                papinho_session_storage *s=candidate_storage(session);
                papinho_pst_owner *o=(papinho_pst_owner *)session->runtime.control.candidate_owner;
                if(s->published || session->bridge_thread || session->wait_set)goto done;
                if(o)o->quiescence_confirmed=1;
                papinho_accelerator_client_teardown(&session->runtime);
                if(session->runtime.control.candidate_owner){session_storage_retain(s);goto done;}
            }
            attempt.network_egress_major=retry.network_egress_major;
            attempt.network_egress_v1_fallback_allowed=retry.network_egress_v1_fallback_allowed;
            papinho_accelerator_client_runtime_init(&session->runtime,attempt.profile);
            goto candidate_control_establish;
#else
            papinho_accelerator_persistent_session_teardown(session);
            papinho_accelerator_persistent_session_init(session);
            return papinho_accelerator_persistent_session_establish(session,
                &retry, cancel_event, out_error);
#endif
        }
        session->runtime.error = PAPINHO_ACCELERATOR_ERROR_CAPABILITY; goto done;
    }
    session->config = *config;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    session->config.candidate_group=candidate_storage(session)->group;
    session->config.network_egress_major=attempt.network_egress_major;
#endif
    session->config.secure_runtime = NULL; session->config.secure_trust = NULL;
    session->config.secure_credentials = NULL;
    session->established = 1;
    session->owner_thread_id = GetCurrentThreadId();
    rc = 0;
done:
    if (out_error != NULL) *out_error = session->runtime.error;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(config->candidate_c2 && key.data)papinho_c2_wipe(key.data,key.size);
#endif
    free(key.data); free(cert.data); free(ca.data);
    if (rc != 0) papinho_accelerator_persistent_session_teardown(session);
    return rc;
}

int papinho_accelerator_persistent_session_observe_start(
    papinho_accelerator_persistent_session *session, HANDLE cancel_event) {
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    papinho_session_storage *s=candidate_storage(session);papinho_pst_owner *o;
    HANDLE *tests[7];int i;
    if(!s || !session->established || session->observing ||
        s->group->thread_id!=GetCurrentThreadId() || root_read(&s->state)!=SESSION_ACTIVE)return -1;
    if(cancel_event && !DuplicateHandle(GetCurrentProcess(),cancel_event,
        GetCurrentProcess(),&session->cancel_event,0,FALSE,DUPLICATE_SAME_ACCESS))goto candidate_observe_fail;
    if(pst_wait_set_create(&session->wait_set)!=PST_RESULT_OK)goto candidate_observe_fail;
    s->group->wait_set=session->wait_set;
    o=(papinho_pst_owner *)session->runtime.control.candidate_owner;
    if(!o || pst_wait_set_add_connection(session->wait_set,o->connection,1U)!=PST_RESULT_OK)
        goto candidate_observe_fail;
    o->wait_set=session->wait_set;
    tests[0]=&session->test_operation_started_event;tests[1]=&session->test_operation_release_event;
    tests[2]=&session->test_operation_inflight_event;tests[3]=&session->test_operation_inflight_release_event;
    tests[4]=&session->test_precompletion_event;tests[5]=&session->test_precompletion_release_event;
    tests[6]=&session->test_action_request_event;
    for(i=0;i<7;i++)if(*tests[i]){
        if(!DuplicateHandle(GetCurrentProcess(),*tests[i],GetCurrentProcess(),
            &s->test_handles[i],0,FALSE,DUPLICATE_SAME_ACCESS))goto candidate_observe_fail;
        *tests[i]=s->test_handles[i];
    }
    session->wake_event=CreateEvent(NULL,TRUE,FALSE,NULL);
    session->bridge_stop_event=CreateEvent(NULL,TRUE,FALSE,NULL);
    if(!session->wake_event || !session->bridge_stop_event ||
       !session_storage_bridge_start(s,papinho_accel_persistent_session_wake_bridge))goto candidate_observe_fail;
    session->observing=1;return 0;
candidate_observe_fail:
    papinho_accelerator_persistent_session_teardown(session);return -1;
#else
    DWORD thread_id;
    if (session == NULL || !session->established || session->observing ||
        session->owner_thread_id != GetCurrentThreadId() ||
        session->runtime.control.secure_connection == NULL) return -1;
    if (pst_wait_set_create(&session->wait_set) != PST_RESULT_OK ||
        pst_wait_set_add_connection(session->wait_set,
            session->runtime.control.secure_connection, 1U) != PST_RESULT_OK) {
        papinho_accelerator_persistent_session_teardown(session);
        return -1;
    }
    session->cancel_event = cancel_event;
    session->wake_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    session->bridge_stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (session->wake_event == NULL || session->bridge_stop_event == NULL) {
        papinho_accelerator_persistent_session_teardown(session);
        return -1;
    }
    session->bridge_thread = CreateThread(NULL, 0,
        papinho_accel_persistent_session_wake_bridge, session, 0, &thread_id);
    if (session->bridge_thread == NULL) {
        papinho_accelerator_persistent_session_teardown(session);
        return -1;
    }
    session->observing = 1;
    return 0;
#endif
}

int papinho_accelerator_persistent_session_request_wake(
    papinho_accelerator_persistent_session *session) {
    if (session == NULL || session->wake_event == NULL) return -1;
    return SetEvent(session->wake_event) ? 0 : -1;
}

static int papinho_accel_persistent_event_classify(
    const PST_WAIT_EVENT *event,
    papinho_accelerator_persistent_connection *out_connection,
    int *out_terminal) {
    if (out_connection != NULL)
        *out_connection = PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_NONE;
    if (out_terminal != NULL) *out_terminal = 0;
    if (event == NULL || out_connection == NULL || out_terminal == NULL)
        return -1;
    if (event->token == 1U)
        *out_connection = PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_CONTROL;
    else if (event->token == 2U)
        *out_connection = PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_DATA;
    else
        return -1;
    *out_terminal = (event->flags & PST_WAIT_READY_TERMINAL) != 0U;
    return 0;
}

int papinho_accelerator_persistent_session_observe_wait(
    papinho_accelerator_persistent_session *session,
    papinho_accelerator_error *out_error,
    papinho_accelerator_persistent_connection *out_connection) {
    PST_WAIT_EVENT events[2]; PST_WAIT_SET_RESULT wait_result;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (out_connection != NULL)
        *out_connection = PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_NONE;
    if (session == NULL || !session->observing ||
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        !candidate_storage(session) || root_read(&candidate_storage(session)->state)!=SESSION_ACTIVE ||
#endif
        session->owner_thread_id != GetCurrentThreadId()) return -1;
    for (;;) {
        PST_RESULT result;
        ZeroMemory(events, sizeof(events)); ZeroMemory(&wait_result, sizeof(wait_result));
        result = pst_wait_set_wait(session->wait_set, INFINITE, events, 2U,
            &wait_result);
        if (result == PST_RESULT_WAIT_WOKEN) return 1;
        if (result != PST_RESULT_OK || wait_result.event_count == 0U) {
            if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_TRANSPORT;
            return 0;
        }
        { pst_size i;
          for (i = 0; i < wait_result.event_count; ++i) {
            papinho_accelerator_stream_state *stream;
            PST_IO_RESULT io_result; PAPACC_U8 byte;
            papinho_accelerator_persistent_connection connection;
            int terminal;
            if (papinho_accel_persistent_event_classify(&events[i], &connection,
                &terminal) != 0) {
                if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_PROTOCOL;
                return 0;
            }
            if (out_connection != NULL) *out_connection = connection;
            if (terminal) {
                if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE;
                return 0;
            }
            stream = connection == PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_CONTROL ?
                &session->runtime.control :
                (connection == PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_DATA ?
                &session->runtime.data : NULL);
            if (stream == NULL) {
                if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_PROTOCOL;
                return 0;
            }
            ZeroMemory(&io_result, sizeof(io_result));
            if (pst_connection_read(stream->secure_connection, &byte, 1U,
                &io_result) != PST_RESULT_OK || io_result.operation ==
                PST_OPERATION_FAILED) {
                if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_TRANSPORT;
                return 0;
            }
            if (io_result.operation == PST_OPERATION_CLOSED) {
                if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE;
                return 0;
            }
            if (io_result.bytes_transferred != 0U) {
                if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_PROTOCOL;
                return 0;
            }
          }
        }
    }
}

int papinho_accelerator_client_fixture_secure_control_data_test(const papinho_accelerator_client_config *config, HANDLE cancel_event, char *detail, size_t detail_size) {
    papinho_accel_fixture_blob ca,cert,key; PST_DER_ITEM anchor,leaf; PST_TRUST_SOURCE ts; PST_CREDENTIAL_SOURCE cs; PST_RUNTIME_OPTIONS ro; pst_trust *trust=NULL; pst_credentials *credentials=NULL; pst_runtime *runtime=NULL; papinho_accelerator_client_config attempt; int rc=-1;
    ZeroMemory(&ca,sizeof(ca));ZeroMemory(&cert,sizeof(cert));ZeroMemory(&key,sizeof(key));ZeroMemory(&attempt,sizeof(attempt));
    if(config==NULL || !config->fixture_secure_materials_enabled || config->profile!=PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL) goto done;

    if(!papinho_accel_fixture_load(config->fixture_ca_path,&ca)) goto done;


    if(!papinho_accel_fixture_load(config->fixture_certificate_path,&cert)) goto done;


    if(!papinho_accel_fixture_load(config->fixture_key_path,&key)) goto done;

anchor.data=ca.data;anchor.size=ca.size;ZeroMemory(&ts,sizeof(ts));ts.struct_size=sizeof(ts);ts.api_version=PST_API_VERSION;ts.kind=PST_TRUST_SOURCE_CUSTOM_CA_DER;ts.anchors=&anchor;ts.anchor_count=1;if(pst_trust_create(&ts,&trust)!=PST_RESULT_OK)goto done;

leaf.data=cert.data;leaf.size=cert.size;ZeroMemory(&cs,sizeof(cs));cs.struct_size=sizeof(cs);cs.api_version=PST_API_VERSION;cs.kind=PST_CREDENTIAL_SOURCE_CERT_CHAIN_DER_PKCS8_DER;cs.certificate_chain=&leaf;cs.certificate_count=1;cs.private_key_der=key.data;cs.private_key_der_size=key.size;if(pst_credentials_create(&cs,&credentials)!=PST_RESULT_OK)goto done;

ZeroMemory(&ro,sizeof(ro));ro.struct_size=sizeof(ro);ro.api_version=PST_API_VERSION;if(pst_runtime_create(&ro,&runtime)!=PST_RESULT_OK)goto done;

    attempt=*config;attempt.secure_runtime=runtime;attempt.secure_trust=trust;attempt.secure_credentials=credentials;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!papinho_candidate_materials_take(&attempt))goto done;
    runtime=NULL;trust=NULL;credentials=NULL;
#endif

    rc=papinho_accelerator_client_control_data_test(&attempt,cancel_event,detail,detail_size);

done:
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!papinho_candidate_materials_finish(&attempt))rc=-1;
#endif
    if(runtime!=NULL)pst_runtime_release(runtime);if(credentials!=NULL)pst_credentials_release(credentials);if(trust!=NULL)pst_trust_release(trust);free(key.data);free(cert.data);free(ca.data);return rc;
}static void papinho_accel_control_probe_detail(char *detail, size_t detail_size,
    int rc, papinho_accelerator_error error) {
    const char *message;
    if (detail == NULL || detail_size == 0) return;
    if (rc == 0) message = "CONTROL/capabilities accepted";
    else if (error == PAPINHO_ACCELERATOR_ERROR_CANCELLED) message = "cancelled";
    else if (error == PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE) message = "server unavailable";
    else if (error == PAPINHO_ACCELERATOR_ERROR_SECURE) message = "secure transport unavailable";
    else if (error == PAPINHO_ACCELERATOR_ERROR_CAPABILITY) message = "required capability not granted";
    else if (error == PAPINHO_ACCELERATOR_ERROR_CONTROL_REJECTED) message = "CONTROL rejected";
    else message = "connect/protocol failure";
    strncpy(detail, message, detail_size - 1);
    detail[detail_size - 1] = '\0';
}

int papinho_accelerator_client_control_test_with_trace(
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_error *out_error, char *detail, size_t detail_size,
    papinho_accelerator_secure_stage_trace *out_trace) {
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    papinho_accelerator_persistent_session *candidate_session=NULL;
#define session (*candidate_session)
#else
    papinho_accelerator_persistent_session session;
#endif
    papinho_accelerator_client_runtime runtime;
    papinho_accelerator_error error;
    int rc;
    if (detail != NULL && detail_size != 0) detail[0] = '\0';
    if (out_trace != NULL) ZeroMemory(out_trace, sizeof(*out_trace));
    error = PAPINHO_ACCELERATOR_ERROR_NONE;
    rc = -1;
    if (config == NULL || config->host[0] == '\0' || config->port == 0 ||
        (config->profile != PAPINHO_ACCELERATOR_TRANSPORT_LEGACY_ENDPOINT &&
        config->profile != PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL)) {
        error = PAPINHO_ACCELERATOR_ERROR_PROTOCOL;
    } else if (config->profile == PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL) {
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        candidate_session=papinho_candidate_session_create();
        if(!candidate_session){error=PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE;goto candidate_control_done;}
#else
        papinho_accelerator_persistent_session_init(&session);
#endif
        if (papinho_accelerator_persistent_session_establish(&session, config,
            cancel_event, &error) == 0) rc = 0;
        if (out_trace != NULL) *out_trace = session.secure_stage_trace;
        papinho_accelerator_persistent_session_teardown(&session);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        if(!papinho_candidate_session_finish(candidate_session))rc=-1;
#endif
    } else {
        papinho_accelerator_client_runtime_init(&runtime, config->profile);
        papinho_accel_fixture_capabilities(&runtime, config);
        if (papinho_accel_control_establish(&runtime, config, cancel_event, NULL) == 0)
            rc = 0;
        error = runtime.error;
        papinho_accelerator_client_teardown(&runtime);
    }
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
candidate_control_done:
#undef session
#endif
    if (out_error != NULL) *out_error = error;
    papinho_accel_control_probe_detail(detail, detail_size, rc, error);
    return rc;
}

int papinho_accelerator_client_control_test(
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_error *out_error, char *detail, size_t detail_size) {
    return papinho_accelerator_client_control_test_with_trace(config, cancel_event,
        out_error, detail, detail_size, NULL);
}

int papinho_accelerator_client_control_data_test(
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    char *detail, size_t detail_size) {
    papinho_accelerator_client_runtime runtime;
    int rc = -1;
    if (detail != NULL && detail_size != 0) detail[0] = '\0';
    if (config == NULL || config->host[0] == '\0' || config->port == 0 ||
        (config->profile != PAPINHO_ACCELERATOR_TRANSPORT_LEGACY_ENDPOINT &&
        config->profile != PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL)) return -1;

    papinho_accelerator_client_runtime_init(&runtime, config->profile);
    papinho_accel_fixture_capabilities(&runtime, config);

    if (papinho_accel_control_establish(&runtime, config, cancel_event, NULL) == 0 &&
        papinho_accel_control_request_ticket(&runtime, config, cancel_event) == 0)
        rc = papinho_accel_data_with_ticket(&runtime, config, cancel_event);
    papinho_accel_fixture_detail(detail, detail_size, rc, runtime.error);
    papinho_accelerator_client_teardown(&runtime);
    return rc;
}

int papinho_accelerator_client_fixture_control_to_ticket(
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket,
    papinho_accelerator_error *out_error) {
    papinho_accelerator_client_runtime runtime;
    int rc = -1;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (config == NULL || out_ticket == NULL || config->host[0] == '\0' ||
        config->port == 0) return -1;
    ZeroMemory(out_ticket, sizeof(*out_ticket));
    papinho_accelerator_client_runtime_init(&runtime, config->profile);
    papinho_accel_fixture_capabilities(&runtime, config);
    if (papinho_accel_control_establish(&runtime, config, cancel_event, NULL) == 0 &&
        papinho_accel_control_request_ticket(&runtime, config, cancel_event) == 0 &&
        runtime.data_ticket_valid) {
        *out_ticket = runtime.data_ticket;
        rc = 0;
    }
    if (out_error != NULL) *out_error = runtime.error;
    papinho_accelerator_client_teardown(&runtime);
    return rc;
}

int papinho_accelerator_client_fixture_control_then_data_with_ticket(
    const papinho_accelerator_client_config *config,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, HANDLE cancel_event,
    papinho_accelerator_error *out_error) {
    papinho_accelerator_client_runtime runtime;
    int rc = -1;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (config == NULL || ticket == NULL || config->host[0] == '\0' ||
        config->port == 0 || !papacc_data_association_ticket_is_valid(ticket))
        return -1;
    papinho_accelerator_client_runtime_init(&runtime, config->profile);
    papinho_accel_fixture_capabilities(&runtime, config);
    if (papinho_accel_control_establish(&runtime, config, cancel_event, NULL) == 0 &&
        papinho_accel_control_request_ticket(&runtime, config, cancel_event) == 0 &&
        papinho_accelerator_client_set_ticket(&runtime, ticket) == 0)
        rc = papinho_accel_data_with_ticket(&runtime, config, cancel_event);
    if (out_error != NULL) *out_error = runtime.error;
    papinho_accelerator_client_teardown(&runtime);
    return rc;
}
/* Mode-B data frames are parsed directly from the already authenticated DATA
 * channel.  The request and response buffers belong to the caller; this
 * boundary never retains those pointers after papinho_accelerator_client_mode_b_tls_request returns. */
typedef struct papinho_accel_mode_b_receive_state {
    papinho_accelerator_mode_b_request *request;
    int open_complete;
    int remote_close;
    int terminal_received;
} papinho_accel_mode_b_receive_state;

static int papinho_accel_mode_b_fail(papinho_accelerator_client_runtime *runtime,
    papinho_accelerator_error error) {
    if (runtime != NULL) runtime->error = error;
    return -1;
}

static int papinho_accel_mode_b_send_frame(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    PAPACC_U16 message_type, const PAPACC_U8 *payload, PAPACC_SIZE payload_size) {
    PAPACC_FRAME_HEADER header;
    PAPACC_U8 *frame;
    PAPACC_SIZE header_size = 0U;
    int rc = -1;
    if (runtime == NULL || config == NULL ||
        (payload_size != 0U && payload == NULL) ||
        (message_type == PAPACC_MESSAGE_TYPE_NETWORK_DATA &&
         papacc_network_data_validate(payload_size) != PAPACC_RESULT_OK) ||
        (message_type != PAPACC_MESSAGE_TYPE_NETWORK_DATA &&
         papacc_network_terminal_validate(message_type, payload_size) != PAPACC_RESULT_OK &&
         message_type != PAPACC_MESSAGE_TYPE_NETWORK_OPEN)) return -1;
    frame = (PAPACC_U8 *)malloc(PAPACC_FRAME_BASE_HEADER_SIZE + payload_size);
    if (frame == NULL) return papinho_accel_mode_b_fail(runtime,
        PAPINHO_ACCELERATOR_ERROR_TRANSPORT);
    header = papacc_network_frame_header(message_type, (PAPACC_U32)payload_size);
    if (papacc_frame_header_encode(&header, frame,
        PAPACC_FRAME_BASE_HEADER_SIZE + payload_size, &header_size) != PAPACC_RESULT_OK ||
        header_size != PAPACC_FRAME_BASE_HEADER_SIZE) goto cleanup;
    if (payload_size != 0U) memcpy(frame + header_size, payload, payload_size);
    if (papinho_accel_stream_send_all(&runtime->data, frame,
        (int)(header_size + payload_size), config, cancel_event) != 0) {
        (void)papinho_accel_mode_b_fail(runtime, papinho_accel_cancelled(cancel_event) ?
            PAPINHO_ACCELERATOR_ERROR_CANCELLED : PAPINHO_ACCELERATOR_ERROR_TRANSPORT);
        goto cleanup;
    }
    rc = 0;
cleanup:
    free(frame);
    return rc;
}

typedef int (*papinho_close_sender)(papinho_accelerator_client_runtime *,
    const papinho_accelerator_client_config *, HANDLE, PAPACC_U16,
    const PAPACC_U8 *, PAPACC_SIZE);

/* Only the operation owner invokes this once after application completion.
 * No resource release, state resurrection or TLS shutdown occurs here. */
static void papinho_accel_mode_b_close_result(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_mode_b_request *request, int remote_close,
    papinho_close_sender sender) {
    papinho_accelerator_data_result *facts = &request->data_result;
    int rc;
    facts->data_terminal_received = request->terminal_received;
    if (facts->network_close_send_result != PAPINHO_CLOSE_NOT_ATTEMPTED ||
        !facts->application_complete) return;
    if (remote_close || request->terminal_received) {
        facts->network_close_send_result = PAPINHO_CLOSE_SKIPPED_TERMINAL;
        return;
    }
    if (runtime->data.terminal ||
        (runtime->data.secure_connection == NULL && !runtime->data.socket_open)) {
        facts->network_close_send_result = PAPINHO_CLOSE_SKIPPED_DATA_CLOSED;
        return;
    }
    facts->transport_send_attempted = 1;
    rc = sender(runtime, config, cancel_event,
        PAPACC_MESSAGE_TYPE_NETWORK_CLOSE, NULL, 0U);
    if (rc == 0) {
        facts->network_close_send_result = PAPINHO_CLOSE_SENT;
        request->normal_close_sent = 1;
    } else {
        facts->network_close_error = runtime->error;
        facts->transport_send_error = runtime->error;
        facts->network_close_send_result =
            runtime->error == PAPINHO_ACCELERATOR_ERROR_CANCELLED ?
            PAPINHO_CLOSE_CANCELLED : PAPINHO_CLOSE_FAILED;
    }
}

static int papinho_accel_mode_b_feed(
    papinho_accelerator_client_runtime *runtime,
    papinho_accel_mode_b_receive_state *state, const PAPACC_U8 *input,
    PAPACC_SIZE input_length) {
    PAPACC_SIZE offset = 0U;
    if (runtime == NULL || state == NULL || state->request == NULL ||
        (input_length != 0U && input == NULL)) return -1;
    while (offset < input_length) {
        PAPACC_FRAME_PARSER_EVENT event = PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
        PAPACC_SIZE consumed = 0U;
        if (papacc_frame_parser_feed(&runtime->data.parser, input + offset,
            input_length - offset, &consumed, &event) != PAPACC_RESULT_OK ||
            consumed == 0U) return papinho_accel_mode_b_fail(runtime,
                PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
        offset += consumed;
        if (event.type == PAPACC_FRAME_PARSER_EVENT_NONE) continue;
        if (event.type == PAPACC_FRAME_PARSER_EVENT_HEADER_READY) {
            state->request->terminal_message_type = event.header.message_type;
            state->request->terminal_payload_length = event.header.payload_length;
            if (runtime->state != PAPINHO_ACCELERATOR_CONNECTION_READY ||
                event.header.flags != 0U ||
                !((event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT &&
                   event.header.payload_length == 1U && !state->open_complete) ||
                  (event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_DATA &&
                   state->open_complete && !state->terminal_received &&
                   papacc_network_data_validate(event.header.payload_length) == PAPACC_RESULT_OK) ||
                  ((event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE ||
                    event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_CLOSE ||
                    event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_ABORT) &&
                   state->open_complete && !state->terminal_received &&
                   event.header.payload_length == 0U) ||
                  (event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON &&
                   state->open_complete && !state->terminal_received &&
                   runtime->network_egress_major == 2U &&
                   state->request->transport_mode == PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED &&
                   event.header.payload_length == 2U)))
                return papinho_accel_mode_b_fail(runtime,
                    PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
            runtime->data_pending_header = event.header;
            runtime->data_pending_frame = 1;
            runtime->data.input_count = 0U;
            continue;
        }
        if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE &&
            !runtime->data_pending_frame &&
            runtime->state == PAPINHO_ACCELERATOR_CONNECTION_READY &&
            state->open_complete && !state->terminal_received &&
            event.header.flags == 0U && event.header.payload_length == 0U &&
            (event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE ||
             event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_CLOSE ||
             event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_ABORT)) {
            state->request->terminal_message_type = event.header.message_type;
            state->request->terminal_payload_length = event.header.payload_length;
            state->terminal_received = 1;
            state->request->terminal_received = 1;
            state->request->terminal_reason =
                PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED;
            if (event.header.message_type == PAPACC_MESSAGE_TYPE_NETWORK_ABORT)
                return papinho_accel_mode_b_fail(runtime,
                    PAPINHO_ACCELERATOR_ERROR_TRANSPORT);
            state->remote_close = 1;
            continue;
        }
        if (event.type == PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK ||
            event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
            if (!runtime->data_pending_frame) return papinho_accel_mode_b_fail(runtime,
                PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
            if (runtime->data_pending_header.message_type ==
                PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT) {
                if (event.payload == NULL || event.payload_length == 0U ||
                    runtime->data.input_count + event.payload_length > 1U)
                    return papinho_accel_mode_b_fail(runtime,
                        PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                runtime->data.input_buffer[runtime->data.input_count++] = event.payload[0];
            } else if (runtime->data_pending_header.message_type ==
                PAPACC_MESSAGE_TYPE_NETWORK_DATA && event.payload_length != 0U) {
                if (event.payload == NULL) return papinho_accel_mode_b_fail(runtime,
                    PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                if (state->request->response_storage != NULL) {
                    if (papinho_accel_response_storage_append(
                        state->request->response_storage, event.payload,
                        event.payload_length) != 0) return papinho_accel_mode_b_fail(runtime,
                        PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                    state->request->response_bytes = state->request->response_storage->data;
                    state->request->response_size = state->request->response_storage->size;
                    state->request->response_capacity = state->request->response_storage->capacity;
                } else {
                    if (state->request->response_size + event.payload_length >
                        state->request->response_capacity) return papinho_accel_mode_b_fail(runtime,
                        PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                    memcpy(state->request->response_bytes + state->request->response_size,
                        event.payload, event.payload_length);
                    state->request->response_size += event.payload_length;
                }
                if (state->request->completion_mode ==
                    PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING &&
                    papinho_accel_http_framing_progress(state->request) < 0)
                    return papinho_accel_mode_b_fail(runtime,
                        PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
            } else if (runtime->data_pending_header.message_type ==
                PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON) {
                if (event.payload == NULL || runtime->data.input_count +
                    event.payload_length > sizeof(runtime->data.input_buffer))
                    return papinho_accel_mode_b_fail(runtime,
                        PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                memcpy(runtime->data.input_buffer + runtime->data.input_count,
                    event.payload, event.payload_length);
                runtime->data.input_count += event.payload_length;
            }
            if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
                if (runtime->data_pending_header.message_type ==
                    PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT) {
                    if (runtime->data.input_count != 1U ||
                        papacc_network_open_result_decode(runtime->data.input_buffer,
                        runtime->data.input_count, &state->request->open_status) != PAPACC_RESULT_OK)
                        return papinho_accel_mode_b_fail(runtime,
                            PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                    state->open_complete = 1;
                    if (state->request->open_status != PAPACC_NETWORK_OPEN_OK)
                        return papinho_accel_mode_b_fail(runtime,
                            state->request->open_status == PAPACC_NETWORK_OPEN_UNAVAILABLE ?
                            PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE :
                            PAPINHO_ACCELERATOR_ERROR_CAPABILITY);
                } else if (runtime->data_pending_header.message_type ==
                    PAPACC_MESSAGE_TYPE_NETWORK_ABORT) {
                    state->terminal_received = 1;
                    state->request->terminal_received = 1;
                    state->request->terminal_reason =
                        PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED;
                    return papinho_accel_mode_b_fail(runtime,
                        PAPINHO_ACCELERATOR_ERROR_TRANSPORT);
                } else if (runtime->data_pending_header.message_type ==
                    PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON) {
                    PAPACC_NETWORK_ABORT_REASON_MESSAGE terminal =
                        PAPACC_NETWORK_ABORT_REASON_MESSAGE_INITIALIZER;
                    if (papacc_network_abort_reason_decode(runtime->data.input_buffer,
                        runtime->data.input_count, &terminal) != PAPACC_RESULT_OK ||
                        terminal.reason == PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED)
                        return papinho_accel_mode_b_fail(runtime,
                            PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
                    state->terminal_received = 1; state->remote_close = 1;
                    state->request->terminal_received = 1;
                    state->request->terminal_reason = terminal.reason;
                    if (terminal.reason != PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED)
                        return papinho_accel_mode_b_fail(runtime,
                            PAPINHO_ACCELERATOR_ERROR_TRANSPORT);
                } else if (runtime->data_pending_header.message_type ==
                    PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE ||
                    runtime->data_pending_header.message_type ==
                    PAPACC_MESSAGE_TYPE_NETWORK_CLOSE) {
                    state->terminal_received = 1; state->remote_close = 1;
                    state->request->terminal_received = 1;
                    state->request->terminal_reason =
                        PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED;
                }
                runtime->data_pending_frame = 0;
                runtime->data.input_count = 0U;
            }
        }
    }
    return 0;
}

static int papinho_accel_mode_b_response_complete(
    const papinho_accel_mode_b_receive_state *state) {
    if (state == NULL || state->request == NULL) return 0;
    if (state->request->completion_mode ==
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING)
        return state->request->http_framing.state ==
            PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE;
    return state->request->response_complete_size != 0U &&
        state->request->response_size >= state->request->response_complete_size;
}

static int papinho_accel_http_tls_truncated_acceptable(
    const papinho_accelerator_mode_b_request *request) {
    if (request == NULL || request->completion_mode !=
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING ||
        request->terminal_reason !=
        PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED) return 0;
    return request->http_framing.state ==
        PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE &&
        !request->http_framing.close_delimited;
}
static int papinho_accel_mode_b_receive_until(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accel_mode_b_receive_state *state, int wait_for_open) {
    PAPACC_U8 bytes[512];
    while ((wait_for_open && !state->open_complete) ||
        (!wait_for_open && ((runtime->network_egress_major == 2U &&
          !state->terminal_received) || (runtime->network_egress_major != 2U &&
          !state->remote_close && !papinho_accel_mode_b_response_complete(state))))) {
        int rc = papinho_accel_stream_receive(&runtime->data, bytes, sizeof(bytes),
            config, cancel_event);
        if (rc == -2) return papinho_accel_mode_b_fail(runtime,
            papinho_accel_cancelled(cancel_event) ?
            PAPINHO_ACCELERATOR_ERROR_CANCELLED : PAPINHO_ACCELERATOR_ERROR_TIMEOUT);
        if (rc == 0) return papinho_accel_mode_b_fail(runtime,
            PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE);
        if (rc < 0) return papinho_accel_mode_b_fail(runtime, PAPINHO_ACCELERATOR_ERROR_TRANSPORT);
        if (papinho_accel_mode_b_feed(runtime, state, bytes, (PAPACC_SIZE)rc) != 0) { if (runtime->error == PAPINHO_ACCELERATOR_ERROR_NONE) runtime->error = PAPINHO_ACCELERATOR_ERROR_PROTOCOL; return -1; }
        if (!wait_for_open && state->remote_close &&
            state->request->completion_mode == PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING) {
            if (state->request->terminal_reason ==
                PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED) {
                if (!papinho_accel_http_tls_truncated_acceptable(
                    state->request))
                    return papinho_accel_mode_b_fail(runtime,
                        PAPINHO_ACCELERATOR_ERROR_TLS_TRUNCATED);
            } else if (papinho_accel_http_framing_remote_close(state->request) != 0)
                return papinho_accel_mode_b_fail(runtime,
                    PAPINHO_ACCELERATOR_ERROR_PROTOCOL);
        }
    }
    return 0;
}

static int papinho_accel_mode_b_execute_ready(
    papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_mode_b_request *request,
    papinho_accelerator_error *out_error) {
    papinho_accel_mode_b_receive_state receive_state;
    PAPACC_NETWORK_OPEN open = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_CAPABILITY_KEY network = PAPACC_PACC_NETWORK_EGRESS_V2_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY tls = PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    PAPACC_U8 open_payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_SIZE open_size = 0U, offset = 0U, fragment;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (runtime == NULL || config == NULL || request == NULL ||
        runtime->state != PAPINHO_ACCELERATOR_CONNECTION_READY ||
        request->destination[0] == '\0' || request->port == 0U ||
        request->request_bytes == NULL || request->request_size == 0U ||
        ((request->response_storage == NULL) &&
         (request->response_bytes == NULL || request->response_capacity == 0U)) ||
        (request->response_storage != NULL && request->response_storage->max_size == 0U) ||
        (request->transport_mode != PAPACC_NETWORK_TRANSPORT_TCP_PLAIN &&
         request->transport_mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)) goto invalid;
    request->response_size = 0U; request->open_status = PAPACC_NETWORK_OPEN_FAILED;
    request->terminal_reason = PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED;
    request->terminal_received = 0; request->stream_reusable = 0;
    request->normal_close_sent = 0;
    ZeroMemory(&request->data_result, sizeof(request->data_result));
    if (runtime->network_egress_major == 1U)
        network.major = PAPACC_PACC_NETWORK_EGRESS_MAJOR_V1;
    else if (runtime->network_egress_major != 2U) {
        runtime->error = PAPINHO_ACCELERATOR_ERROR_CAPABILITY; goto done;
    }
    if (!papinho_accelerator_client_capability_granted(runtime, &network) ||
        (request->transport_mode == PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED &&
         !papinho_accelerator_client_capability_granted(runtime, &tls))) {
        runtime->error = PAPINHO_ACCELERATOR_ERROR_CAPABILITY; goto done;
    }
    open.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    open.transport_mode = request->transport_mode; open.port = request->port;
    open.destination_size = (PAPACC_U16)strlen(request->destination);
    if (open.destination_size == 0U || open.destination_size > PAPACC_NETWORK_DESTINATION_CAPACITY) { runtime->error = PAPINHO_ACCELERATOR_ERROR_PROTOCOL; goto done; }
    memcpy(open.destination, request->destination, open.destination_size);
    if (open.transport_mode == PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED) { open.alpn_count = 1U; open.alpn[0].size = 8U; memcpy(open.alpn[0].bytes, "papacc/1", 8U); }
    if (papacc_network_open_encode(&open, open_payload, sizeof(open_payload),
        &open_size) != PAPACC_RESULT_OK ||
        papinho_accel_mode_b_send_frame(runtime, config, cancel_event,
        PAPACC_MESSAGE_TYPE_NETWORK_OPEN, open_payload, open_size) != 0) goto done;
    if (runtime->test_counter_enabled)
        InterlockedIncrement(&runtime->test_network_open_count);
    ZeroMemory(&receive_state, sizeof(receive_state)); receive_state.request = request;
    if (papinho_accel_mode_b_receive_until(runtime, config, cancel_event, &receive_state, 1) != 0) goto done;
    fragment = request->request_fragment_size; if (fragment == 0U || fragment > PAPACC_NETWORK_DATA_MAX_PAYLOAD) fragment = PAPACC_NETWORK_DATA_MAX_PAYLOAD;
    while (offset < request->request_size) { PAPACC_SIZE amount = request->request_size - offset; if (amount > fragment) amount = fragment; if (papinho_accel_mode_b_send_frame(runtime, config, cancel_event, PAPACC_MESSAGE_TYPE_NETWORK_DATA, request->request_bytes + offset, amount) != 0) goto done; offset += amount; }
    if (papinho_accel_mode_b_receive_until(runtime, config, cancel_event, &receive_state, 0) != 0) goto done;
    request->data_result.application_complete = 1;
    papinho_accel_mode_b_close_result(runtime, config, cancel_event, request,
        receive_state.remote_close, papinho_accel_mode_b_send_frame);
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    return 0;
invalid:
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_LEGACY_RESTRICTED;
    return -1;
done:
    if (out_error != NULL) *out_error = runtime->error;
    return -1;
}

int papinho_accelerator_client_mode_b_tls_request(
    const papinho_accelerator_client_config *config, HANDLE cancel_event,
    papinho_accelerator_mode_b_request *request,
    papinho_accelerator_error *out_error) {
    papinho_accelerator_client_runtime runtime; int rc;
    PAPACC_CAPABILITY_KEY tls = PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    papinho_accelerator_client_config retry;
    if (config == NULL || request == NULL || config->profile != PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL) { if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_LEGACY_RESTRICTED; return -1; }
    request->transport_mode = PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    papinho_accelerator_client_runtime_init(&runtime, config->profile);
    papinho_accel_fixture_capabilities(&runtime, config);
    if (papinho_accel_control_establish(&runtime, config, cancel_event, NULL) != 0) rc = -1;
    else if (!papinho_accelerator_client_mode_b_requirements_granted(&runtime)) {
        if (runtime.network_egress_major == 0U &&
            papinho_accelerator_client_capability_granted(&runtime, &tls) &&
            config->network_egress_v1_fallback_allowed &&
            config->network_egress_major != 1U) {
            retry = *config; retry.network_egress_major = 1U;
            retry.network_egress_v1_fallback_allowed = 0;
            papinho_accelerator_client_teardown(&runtime);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
            if(runtime.control.candidate_owner || runtime.data.candidate_owner)return -1;
#endif
            return papinho_accelerator_client_mode_b_tls_request(&retry,
                cancel_event, request, out_error);
        }
        runtime.error = PAPINHO_ACCELERATOR_ERROR_CAPABILITY; rc = -1;
    } else if (papinho_accel_control_request_ticket(&runtime, config, cancel_event) != 0 ||
        papinho_accel_data_with_ticket(&runtime, config, cancel_event) != 0) rc = -1;
    else rc = papinho_accel_mode_b_execute_ready(&runtime, config, cancel_event, request, out_error);
    if (out_error != NULL && rc != 0) *out_error = runtime.error;
    papinho_accelerator_client_teardown(&runtime); return rc;
}

static void papinho_accel_persistent_data_finish(
    papinho_accelerator_persistent_session *session) {
    if (session == NULL) return;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    {
        papinho_session_storage *s=candidate_storage(session);
        papinho_pst_owner *o=(papinho_pst_owner *)session->runtime.data.candidate_owner;
        if(!s || root_read(&s->state)!=SESSION_ACTIVE)return;
        /* This serialized DATA operation has returned from its last PST call.
         * No wait is active on this owner; the bridge only wakes the set. */
        if(o && !papinho_pst_owner_remove_membership(o)){
            (void)session_storage_stop(s,5000);session_storage_retain(s);return;}
        session->active_data_registered=0;
        if(o)o->quiescence_confirmed=1;
        papinho_accelerator_client_data_reset(&session->runtime);
        if(session->runtime.data.candidate_owner || session->runtime.data.socket_open){
            /* Stop/join the independent bridge once even when the DATA child
             * is retained. This never retries PST release or drops materials. */
            (void)session_storage_stop(s,5000);session_storage_retain(s);return;}
    }
#else
    if (session->wait_set != NULL && session->active_data_registered &&
        session->runtime.data.secure_connection != NULL)
        (void)pst_wait_set_remove_connection(session->wait_set, session->runtime.data.secure_connection);
    session->active_data_registered = 0;
    papinho_accelerator_client_data_reset(&session->runtime);
#endif
    /* DATA is operation-scoped. A CONTROL terminal remains session-terminal;
     * only a nonterminal DATA completion returns the durable CONTROL to caps. */
    if (session->established && session->runtime.control.secure_connection != NULL &&
        session->runtime.state != PAPINHO_ACCELERATOR_CONNECTION_TERMINAL)
        session->runtime.state = PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED;
}

int papinho_accelerator_persistent_session_request(
    papinho_accelerator_persistent_session *session, HANDLE cancel_event,
    papinho_accelerator_mode_b_request *request,
    papinho_accelerator_error *out_error) {
    papinho_accelerator_client_config attempt; HANDLE operation_cancel_event; int rc = -1;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (session == NULL || request == NULL || !session->established ||
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        !candidate_storage(session) || root_read(&candidate_storage(session)->state)!=SESSION_ACTIVE ||
#endif
        session->owner_thread_id != GetCurrentThreadId() ||
        session->active_data_registered || session->runtime.state !=
        PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED) goto done;
    operation_cancel_event = session->active_operation_cancel_event != NULL ?
        session->active_operation_cancel_event : cancel_event;
    if (papinho_accel_cancelled(session->cancel_event) &&
        operation_cancel_event != NULL) SetEvent(operation_cancel_event);
    if (papinho_accel_cancelled(operation_cancel_event)) {
        session->runtime.error = PAPINHO_ACCELERATOR_ERROR_CANCELLED;
        goto done;
    }
    attempt = session->config; attempt.secure_runtime = session->secure_runtime;
    attempt.secure_trust = session->secure_trust; attempt.secure_credentials = session->secure_credentials;
    session->runtime.test_counter_enabled = session->test_counter_enabled;
    if (session->test_counter_enabled) {
        InterlockedIncrement(&session->test_data_ticket_request_count);
        if (session->test_publish_data_ticket_request_count != NULL)
            InterlockedIncrement((LONG *)session->test_publish_data_ticket_request_count);
    }
    if (papinho_accel_control_request_ticket(&session->runtime, &attempt, operation_cancel_event) != 0) goto done;
    if (papinho_accel_data_with_ticket(&session->runtime, &attempt, operation_cancel_event) != 0) goto done;
    if (session->test_counter_enabled) {
        InterlockedIncrement(&session->test_data_connection_count);
        if (session->test_publish_data_connection_count != NULL)
            InterlockedIncrement((LONG *)session->test_publish_data_connection_count);
    }
    if (session->wait_set == NULL || session->runtime.data.secure_connection == NULL ||
        pst_wait_set_add_connection(session->wait_set,
        session->runtime.data.secure_connection, 2U) != PST_RESULT_OK) {
        session->runtime.error = PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH; goto done;
    }
    session->active_data_registered = 1;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    ((papinho_pst_owner *)session->runtime.data.candidate_owner)->wait_set=session->wait_set;
#endif
    if (session->test_operation_inflight_event != NULL)
        SetEvent(session->test_operation_inflight_event);
    if (session->test_operation_inflight_release_event != NULL) {
        HANDLE waits[2];
        DWORD count = 1U;
        waits[0] = session->test_operation_inflight_release_event;
        if (operation_cancel_event != NULL) {
            waits[1] = operation_cancel_event;
            count = 2U;
        }
        (void)WaitForMultipleObjects(count, waits, FALSE, INFINITE);
    }
    rc = papinho_accel_mode_b_execute_ready(&session->runtime, &attempt, operation_cancel_event, request, out_error);
    if (rc != 0 && session->runtime.error == PAPINHO_ACCELERATOR_ERROR_CANCELLED)
        (void)papinho_accel_mode_b_send_frame(&session->runtime, &attempt, operation_cancel_event, PAPACC_MESSAGE_TYPE_NETWORK_ABORT, NULL, 0U);
done:
    if (out_error != NULL && rc != 0) *out_error = session->runtime.error;
    papinho_accel_persistent_data_finish(session);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(root_read(&candidate_storage(session)->state)==SESSION_RETAINED){
        request->stream_reusable=0;rc=-1;
        if(out_error)*out_error=PAPINHO_ACCELERATOR_ERROR_SECURE;
    }
#endif
    if (session->test_counter_enabled)
        InterlockedIncrement(&session->test_active_cleanup_count);
    return rc;
}
int papinho_accelerator_persistent_session_submit(
    papinho_accelerator_persistent_session *session, unsigned long generation,
    const papinho_accelerator_mode_b_request *request,
    unsigned long *out_job_id) {
    unsigned int i;
    papinho_accelerator_persistent_request_job *job = NULL;
    if (out_job_id != NULL) *out_job_id = 0U;
    if (session == NULL || request == NULL || generation == 0U ||
        request->destination[0] == '\0' || request->port == 0U ||
        request->request_bytes == NULL || request->request_size == 0U ||
        request->request_size > PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_MAX ||
        request->response_complete_size > PAPINHO_ACCELERATOR_PERSISTENT_RESPONSE_MAX ||
        (request->completion_mode != PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_FIXED_SIZE &&
         request->completion_mode != PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING) ||
        (request->transport_mode != PAPACC_NETWORK_TRANSPORT_TCP_PLAIN &&
         request->transport_mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED) ||
        papinho_accel_persistent_jobs_ensure(session) != 0) return -1;
    EnterCriticalSection(&session->job_lock);
    for (i = 0U; i < PAPINHO_ACCELERATOR_PERSISTENT_JOB_QUEUE_LIMIT; ++i) {
        LONG state = session->jobs[i].state;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        if(session->jobs[i].caller_outstanding)continue;
#endif
        if (state != PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_QUEUED &&
            state != PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_ACTIVE) {
            job = &session->jobs[i];
            break;
        }
    }
    if (job == NULL) { LeaveCriticalSection(&session->job_lock); return -1; }
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(session->next_job_id==ULONG_MAX){LeaveCriticalSection(&session->job_lock);return -1;}
#endif
    if (job->completion_event == NULL)
        job->completion_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (job->cancel_event == NULL)
        job->cancel_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (job->completion_event == NULL || job->cancel_event == NULL) {
        LeaveCriticalSection(&session->job_lock); return -1;
    }
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!ResetEvent(job->completion_event) || !ResetEvent(job->cancel_event)){
        papinho_candidate_session_retain(session);LeaveCriticalSection(&session->job_lock);return -1;}
    job->caller_outstanding=1;
#else
    ResetEvent(job->completion_event); ResetEvent(job->cancel_event);
#endif
    job->generation = generation;
    job->job_id = session->next_job_id++;
    if (session->next_job_id == 0U) session->next_job_id = 1U;
    strncpy(job->destination, request->destination, sizeof(job->destination) - 1U);
    job->destination[sizeof(job->destination) - 1U] = '\0';
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    job->candidate_c2=request->candidate_c2;job->c2_navigation=request->c2_navigation;
#endif
    job->port = request->port; job->transport_mode = request->transport_mode;
    memcpy(job->request_bytes, request->request_bytes, request->request_size);
    job->request_size = request->request_size;
    job->request_fragment_size = request->request_fragment_size;
    papinho_accel_response_storage_release(&job->response_storage);
    job->response_storage.max_size = PAPINHO_ACCELERATOR_PERSISTENT_RESPONSE_MAX;
    job->response_complete_size = request->response_complete_size;
    job->completion_mode = request->completion_mode;
    job->http_framing = request->http_framing;
    job->open_status = PAPACC_NETWORK_OPEN_FAILED;
    job->normal_close_sent = 0;
    ZeroMemory(&job->data_result, sizeof(job->data_result));
    job->error = PAPINHO_ACCELERATOR_ERROR_NONE;
    job->state = PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_QUEUED;
    ++session->job_queue_count;
    if (session->test_counter_enabled) {
        InterlockedIncrement(&session->test_job_queued_count);
        if (session->test_publish_job_queued_count != NULL)
            InterlockedIncrement((LONG *)session->test_publish_job_queued_count);
    }
    if (out_job_id != NULL) *out_job_id = job->job_id;
    LeaveCriticalSection(&session->job_lock);
    if (session->observing) (void)papinho_accelerator_persistent_session_request_wake(session);
    return 0;
}

int papinho_accelerator_persistent_session_cancel_job(
    papinho_accelerator_persistent_session *session, unsigned long job_id) {
    int slot;
    if (session == NULL || !session->job_lock_initialized) return -1;
    EnterCriticalSection(&session->job_lock);
    slot = papinho_accel_persistent_job_find(session, job_id);
    if (slot < 0) { LeaveCriticalSection(&session->job_lock); return -1; }
    if (session->jobs[slot].state == PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_QUEUED) {
        session->jobs[slot].state = PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_CANCELLED;
        session->jobs[slot].error = PAPINHO_ACCELERATOR_ERROR_CANCELLED;
        if (session->job_queue_count != 0U) --session->job_queue_count;
        if (session->test_counter_enabled) {
            InterlockedIncrement(&session->test_job_cancelled_queued_count);
            if (session->test_publish_job_cancelled_queued_count != NULL)
                InterlockedIncrement((LONG *)session->test_publish_job_cancelled_queued_count);
        }
        SetEvent(session->jobs[slot].cancel_event);
        SetEvent(session->jobs[slot].completion_event);
    } else if (session->jobs[slot].state == PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_ACTIVE) {
        SetEvent(session->jobs[slot].cancel_event);
        if (session->active_operation_cancel_event != NULL)
            SetEvent(session->active_operation_cancel_event);
    }
    LeaveCriticalSection(&session->job_lock);
    if (session->observing) (void)papinho_accelerator_persistent_session_request_wake(session);
    return 0;
}

void papinho_accelerator_persistent_session_cancel_all_jobs(
    papinho_accelerator_persistent_session *session) {
    unsigned int i;
    /* Teardown releases queue storage but deliberately keeps the lock available
     * for a later fresh establish.  Final defensive cancellation is therefore
     * permitted after teardown, but must not enumerate released jobs. */
    if (session == NULL || !session->job_lock_initialized || session->jobs == NULL)
        return;
    EnterCriticalSection(&session->job_lock);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    (void)candidate_cancel_probe(session);
    if(session->active_operation_cancel_event)SetEvent(session->active_operation_cancel_event);
#endif
    for (i = 0U; i < PAPINHO_ACCELERATOR_PERSISTENT_JOB_QUEUE_LIMIT; ++i) {
        papinho_accelerator_persistent_request_job *job = &session->jobs[i];
        if (job->state == PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_QUEUED) {
            job->state = PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_CANCELLED;
            job->error = PAPINHO_ACCELERATOR_ERROR_CANCELLED;
            if (session->job_queue_count != 0U) --session->job_queue_count;
            if (session->test_counter_enabled) {
                InterlockedIncrement(&session->test_job_cancelled_queued_count);
                if (session->test_publish_job_cancelled_queued_count != NULL)
                    InterlockedIncrement((LONG *)session->test_publish_job_cancelled_queued_count);
            }
            SetEvent(job->cancel_event); SetEvent(job->completion_event);
        } else if (job->state == PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_ACTIVE) {
            SetEvent(job->cancel_event);
            if (session->active_operation_cancel_event != NULL)
                SetEvent(session->active_operation_cancel_event);
        }
    }
    LeaveCriticalSection(&session->job_lock);
    if (session->observing) (void)papinho_accelerator_persistent_session_request_wake(session);
}

int papinho_accelerator_persistent_session_wait_job(
    papinho_accelerator_persistent_session *session, unsigned long job_id,
    HANDLE cancel_event, DWORD timeout_ms) {
    HANDLE completion = NULL; HANDLE waits[2]; DWORD count = 1U; DWORD wait_rc;
    int slot;
    if (session == NULL || !session->job_lock_initialized) return -1;
    EnterCriticalSection(&session->job_lock);
    slot = papinho_accel_persistent_job_find(session, job_id);
    if (slot >= 0 && session->jobs[slot].completion_event != NULL)
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        /* The job slot and Session receipt remain live through caller return.
         * The job owns this handle; waiting never acquires a second owner. */
        completion=session->jobs[slot].completion_event;
#else
        (void)DuplicateHandle(GetCurrentProcess(), session->jobs[slot].completion_event,
            GetCurrentProcess(), &completion, 0, FALSE, DUPLICATE_SAME_ACCESS);
#endif
    LeaveCriticalSection(&session->job_lock);
    if (completion == NULL) return -1;
    waits[0] = completion;
    if (cancel_event != NULL) { waits[1] = cancel_event; count = 2U; }
    wait_rc = WaitForMultipleObjects(count, waits, FALSE, timeout_ms);
    if (wait_rc == WAIT_OBJECT_0) {
#if !defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        CloseHandle(completion);
#endif
        return 0;
    }
    if (count == 2U && wait_rc == WAIT_OBJECT_0 + 1U) {
        (void)papinho_accelerator_persistent_session_cancel_job(session, job_id);
        wait_rc = WaitForSingleObject(completion, INFINITE);
#if !defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        CloseHandle(completion);
#endif
        return wait_rc == WAIT_OBJECT_0 ? 1 : -1;
    }
#if !defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    CloseHandle(completion);
#endif
    return -1;
}
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
#include "papinho_c2_budget.inc"
void papinho_candidate_job_return(papinho_accelerator_persistent_session *v,unsigned long job_id) {
    int slot;
    if(!v || !v->job_lock_initialized)return;
    EnterCriticalSection(&v->job_lock);slot=papinho_accel_persistent_job_find(v,job_id);
    if(slot>=0){v->jobs[slot].caller_outstanding=0;c2_job_return_charge(&v->jobs[slot]);}
    LeaveCriticalSection(&v->job_lock);
}
#endif
#include "papinho_control_probe_submit.inc"

int papinho_accelerator_persistent_session_control_probe_collect(
    papinho_accelerator_persistent_session *session,
    papinho_accelerator_error *out_error, char *detail, size_t detail_size,
    papinho_accelerator_secure_stage_trace *out_trace) {
    int operation_rc;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (detail != NULL && detail_size != 0U) detail[0] = '\0';
    if (out_trace != NULL) ZeroMemory(out_trace, sizeof(*out_trace));
    if (session == NULL || !session->job_lock_initialized) return -1;
    EnterCriticalSection(&session->job_lock);
    if (session->control_probe.state != 3) {
        LeaveCriticalSection(&session->job_lock); return -1;
    }
    if (out_error != NULL) *out_error = session->control_probe.error;
    if (detail != NULL && detail_size != 0U) {
        strncpy(detail, session->control_probe.detail, detail_size - 1U);
        detail[detail_size - 1U] = '\0';
    }
    if (out_trace != NULL) *out_trace = session->control_probe.trace;
    operation_rc = session->control_probe.rc;
    session->control_probe.state = 0;
    LeaveCriticalSection(&session->job_lock);
    return operation_rc;
}

static int papinho_accel_persistent_control_probe_process(
    papinho_accelerator_persistent_session *session) {
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    papinho_accelerator_persistent_session *candidate_temporary=NULL;
#define temporary (*candidate_temporary)
#else
    papinho_accelerator_persistent_session temporary;
#endif
    papinho_accelerator_client_config config;
    papinho_accelerator_error error;
    HANDLE cancel_event;
#if !defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    HANDLE completion_event;
#endif
    int rc;
    if (session == NULL || !session->job_lock_initialized) return 0;
    EnterCriticalSection(&session->job_lock);
    if (session->control_probe.state != 1) {
        LeaveCriticalSection(&session->job_lock); return 0;
    }
    session->control_probe.state = 2;
    config = session->control_probe.config;
    cancel_event = session->control_probe.cancel_event;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!cancel_event)cancel_event=session->active_operation_cancel_event;
    if(!papinho_accel_cancelled(session->cancel_event) && session->active_operation_cancel_event)
        ResetEvent(session->active_operation_cancel_event);
#endif
#if !defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    completion_event = session->control_probe.completion_event;
#endif
    LeaveCriticalSection(&session->job_lock);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    candidate_temporary=papinho_candidate_session_create();
    if(!candidate_temporary){
        EnterCriticalSection(&session->job_lock);
        session->control_probe.rc=-1;session->control_probe.error=PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE;
        if(!papinho_probe_dependencies_complete((papinho_probe_dependencies *)session->control_probe.dependencies))
            papinho_candidate_session_retain(session);
        session->control_probe.dependencies=NULL;session->control_probe.cancel_event=NULL;
        session->control_probe.completion_event=NULL;session->control_probe.config.credential_lease=NULL;
        session->control_probe.state=3;LeaveCriticalSection(&session->job_lock);return 1;
    }
    config.candidate_group=candidate_storage(session)->group;
#else
    ZeroMemory(&temporary, sizeof(temporary));
    papinho_accelerator_persistent_session_init(&temporary);
#endif
    config.secure_runtime = session->secure_runtime;
    error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (papinho_accel_cancelled(cancel_event)) {
        rc = -1; error = PAPINHO_ACCELERATOR_ERROR_CANCELLED;
    } else {
        rc = papinho_accelerator_persistent_session_establish(&temporary,
            &config, cancel_event, &error);
    }
    papinho_accel_control_probe_detail(session->control_probe.detail,
        sizeof(session->control_probe.detail), rc, error);
    EnterCriticalSection(&session->job_lock);
    session->control_probe.rc = rc;
    session->control_probe.error = error;
    session->control_probe.trace = temporary.secure_stage_trace;
#if !defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    session->control_probe.state = 3;
#endif
    LeaveCriticalSection(&session->job_lock);
    papinho_accelerator_persistent_session_teardown(&temporary);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!papinho_candidate_session_finish(candidate_temporary))
        papinho_candidate_session_retain(session);
#undef temporary
    /* Establish/teardown on this owner have returned. The temporary probe
     * does not launch a bridge; it has ceased using these input aliases.
     * This receipt says nothing about PST destruction or parent quiescence. */
    EnterCriticalSection(&session->job_lock);
    if(!papinho_probe_dependencies_complete(
        (papinho_probe_dependencies *)session->control_probe.dependencies))
        papinho_candidate_session_retain(session);
    session->control_probe.dependencies = NULL;
    session->control_probe.config.credential_lease = NULL;
    session->control_probe.cancel_event = NULL;
    session->control_probe.completion_event = NULL;
    session->control_probe.state = 3;
    LeaveCriticalSection(&session->job_lock);
#else
    if (completion_event != NULL) SetEvent(completion_event);
#endif
    return 1;
}

#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
#include "C:/Projetos/PapinhoAccelerator/docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025/browser/papinho_c2_s6.PREIMAGE.inc"
#endif
int papinho_accelerator_persistent_session_process_next(
    papinho_accelerator_persistent_session *session) {
    papinho_accelerator_persistent_request_job *job = NULL;
    papinho_accelerator_mode_b_request request;
    papinho_accelerator_error error = PAPINHO_ACCELERATOR_ERROR_NONE;
    unsigned int i; int slot = -1; int rc;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    int candidate_c2_job;
#endif
    if (session == NULL || !session->established ||
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
        !candidate_storage(session) || root_read(&candidate_storage(session)->state)!=SESSION_ACTIVE ||
#endif
        session->owner_thread_id != GetCurrentThreadId() ||
        !session->job_lock_initialized) return -1;
    rc = papinho_accel_persistent_control_probe_process(session);
    if (rc != 0) return rc;
    EnterCriticalSection(&session->job_lock);
    for (i = 0U; i < PAPINHO_ACCELERATOR_PERSISTENT_JOB_QUEUE_LIMIT; ++i)
        if (session->jobs[i].state == PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_QUEUED) {
            slot = (int)i; job = &session->jobs[i]; break;
        }
    if (job == NULL) { LeaveCriticalSection(&session->job_lock); return 0; }
    job->state = PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_ACTIVE;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    candidate_c2_job=job->candidate_c2;
#endif
    session->active_job_slot = slot;
    if (session->active_operation_cancel_event != NULL) {
        ResetEvent(session->active_operation_cancel_event);
        if (papinho_accel_cancelled(session->cancel_event))
            SetEvent(session->active_operation_cancel_event);
    }
    if (session->test_counter_enabled) {
        InterlockedIncrement(&session->test_job_active_count);
        if (session->test_publish_job_active_count != NULL)
            InterlockedIncrement((LONG *)session->test_publish_job_active_count);
    }
    if (session->job_queue_count != 0U) --session->job_queue_count;
    LeaveCriticalSection(&session->job_lock);
    if (session->test_operation_started_event != NULL)
        SetEvent(session->test_operation_started_event);
    if (session->test_operation_release_event != NULL) {
        HANDLE waits[3]; DWORD count = 1U; waits[0] = session->test_operation_release_event;
        if (job->cancel_event != NULL) { waits[count++] = job->cancel_event; }
        if (session->active_operation_cancel_event != NULL)
            waits[count++] = session->active_operation_cancel_event;
        (void)WaitForMultipleObjects(count, waits, FALSE, INFINITE);
    }
    ZeroMemory(&request, sizeof(request));
    strncpy(request.destination, job->destination, sizeof(request.destination) - 1U);
    request.port = job->port; request.transport_mode = job->transport_mode;
    request.request_bytes = job->request_bytes; request.request_size = job->request_size;
    request.request_fragment_size = job->request_fragment_size;
    request.response_storage = &job->response_storage;
    request.response_bytes = job->response_storage.data;
    request.response_capacity = job->response_storage.capacity;
    request.response_complete_size = job->response_complete_size;
    request.completion_mode = job->completion_mode;
    request.http_framing = job->http_framing;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(job->candidate_c2)rc=candidate_c2_execute(session,job,&request,&error);else
#endif
    rc = papinho_accelerator_persistent_session_request(session,
        session->active_operation_cancel_event != NULL ?
        session->active_operation_cancel_event : job->cancel_event,
        &request, &error);
    if (session->test_precompletion_event != NULL)
        SetEvent(session->test_precompletion_event);
    if (session->test_precompletion_release_event != NULL) {
        HANDLE waits[2];
        DWORD count = 1U;
        DWORD wait_rc;
        waits[0] = session->test_precompletion_release_event;
        if (session->test_action_request_event != NULL) {
            waits[1] = session->test_action_request_event;
            count = 2U;
        }
        for (;;) {
            wait_rc = WaitForMultipleObjects(count, waits, FALSE, INFINITE);
            if (wait_rc == WAIT_OBJECT_0) break;
            if (count == 2U && wait_rc == WAIT_OBJECT_0 + 1U &&
                session->test_action_consumer != NULL) {
                session->test_action_consumer();
                continue;
            }
            break;
        }
    }
    EnterCriticalSection(&session->job_lock);
    job->response_storage.size = request.response_size;
    job->http_framing = request.http_framing;
    job->open_status = request.open_status;
    job->terminal_reason = request.terminal_reason;
    job->terminal_message_type = request.terminal_message_type;
    job->terminal_payload_length = request.terminal_payload_length;
    job->terminal_received = request.terminal_received;
    job->stream_reusable = request.stream_reusable;
    job->normal_close_sent = request.normal_close_sent;
    job->data_result = request.data_result;
    job->error = error;
    session->active_job_slot = -1;
    job->state = error == PAPINHO_ACCELERATOR_ERROR_CANCELLED ?
        PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_CANCELLED :
        PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_COMPLETE;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!SetEvent(job->completion_event))papinho_candidate_session_retain(session);
    c2_job_return_charge(job);
#else
    SetEvent(job->completion_event);
#endif
    LeaveCriticalSection(&session->job_lock);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    /* An isolated C2 operation error is a completed queue item, not failure
     * of the parent CONTROL. Continue already queued Mode B work unless the
     * canonical owner/session/root was actually retained or stopped. */
    if(candidate_c2_job && !root_read(&papinho_pst_admission_stopped) &&
       root_read(&candidate_storage(session)->state)==SESSION_ACTIVE)return 1;
#endif
    return rc == 0 ? 1 : -1;
}

int papinho_accelerator_persistent_session_job_copy_result(
    papinho_accelerator_persistent_session *session, unsigned long job_id,
    PAPACC_U8 *response, PAPACC_SIZE response_capacity,
    PAPACC_SIZE *out_response_size, papinho_accelerator_error *out_error,
    papinho_accelerator_persistent_result_observation *out_observation) {
    int slot;
    int result;
    if (out_response_size != NULL) *out_response_size = 0U;
    if (out_error != NULL) *out_error = PAPINHO_ACCELERATOR_ERROR_NONE;
    if (out_observation != NULL) ZeroMemory(out_observation, sizeof(*out_observation));
    if (session == NULL || !session->job_lock_initialized) return -1;
    EnterCriticalSection(&session->job_lock);
    slot = papinho_accel_persistent_job_find(session, job_id);
    if (slot < 0 || (session->jobs[slot].state !=
        PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_COMPLETE &&
        session->jobs[slot].state != PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_CANCELLED)) {
        LeaveCriticalSection(&session->job_lock); return -1;
    }
    if (out_error != NULL) *out_error = session->jobs[slot].error;
    if (out_observation != NULL) {
        out_observation->http_framing = session->jobs[slot].http_framing;
        out_observation->terminal_reason = session->jobs[slot].terminal_reason;
        out_observation->terminal_message_type = session->jobs[slot].terminal_message_type;
        out_observation->terminal_payload_length = session->jobs[slot].terminal_payload_length;
        out_observation->terminal_received = session->jobs[slot].terminal_received;
        out_observation->stream_reusable = session->jobs[slot].stream_reusable;
        out_observation->normal_close_sent = session->jobs[slot].normal_close_sent;
        out_observation->data_result = session->jobs[slot].data_result;
    }
    if (out_response_size != NULL) *out_response_size = session->jobs[slot].response_storage.size;
    if (response != NULL && response_capacity >= session->jobs[slot].response_storage.size)
        memcpy(response, session->jobs[slot].response_storage.data,
            session->jobs[slot].response_storage.size);
    else if (session->jobs[slot].response_storage.size != 0U) {
        LeaveCriticalSection(&session->job_lock); return -1;
    }
    result = session->jobs[slot].error == PAPINHO_ACCELERATOR_ERROR_NONE ? 0 : -1;
    papinho_accel_response_storage_release(&session->jobs[slot].response_storage);
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    session->jobs[slot].caller_outstanding=0;
#endif
    LeaveCriticalSection(&session->job_lock);
    return result;
}
static int papinho_accel_control_selftest_prepare(
    papinho_accelerator_client_runtime *runtime) {
    papinho_accelerator_client_runtime_init(runtime,
        PAPINHO_ACCELERATOR_TRANSPORT_LEGACY_ENDPOINT);
    return papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CONNECTING) == 0 &&
        papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_TRANSPORT_CONNECTED) == 0 &&
        papinho_accel_control_queue_open(runtime) == 0 ? 0 : -1;
}

static int papinho_accel_control_selftest_accept(PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_length) {
    PAPACC_CONTROL_PROTOCOL_VERSION version =
        PAPACC_CONTROL_PROTOCOL_VERSION_INITIALIZER;
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_SIZE header_length = 0;
    PAPACC_SIZE payload_length = 0;
    if (output == NULL || out_length == NULL) return -1;
    *out_length = 0;
    version.major = PAPACC_CONTROL_PROTOCOL_MAJOR;
    version.minor = PAPACC_CONTROL_PROTOCOL_MINOR;
    header = papacc_control_accept_frame_header();
    if (capacity < PAPACC_FRAME_BASE_HEADER_SIZE +
        PAPACC_CONTROL_VERSION_PAYLOAD_SIZE ||
        papacc_frame_header_encode(&header, output, capacity, &header_length) !=
        PAPACC_RESULT_OK || papacc_control_accept_encode(&version,
        output + header_length, capacity - header_length, &payload_length) !=
        PAPACC_RESULT_OK) return -1;
    *out_length = header_length + payload_length;
    return 0;
}

static int papinho_accel_capability_set_copy(PAPACC_CAPABILITY_KEY *storage,
    PAPACC_SIZE *count, const PAPACC_CAPABILITY_KEY *keys, PAPACC_SIZE key_count) {
    PAPACC_CAPABILITY_SET set = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_SIZE i;
    if (storage == NULL || count == NULL || (key_count != 0 && keys == NULL) ||
        papacc_capability_set_init(&set, storage,
        PAPINHO_ACCELERATOR_CAPABILITY_MAX) != PAPACC_RESULT_OK) return -1;
    for (i = 0; i < key_count; ++i)
        if (papacc_capability_set_insert(&set, &keys[i]) != PAPACC_RESULT_OK)
            return -1;
    *count = set.count;
    return 0;
}

static int papinho_accel_control_selftest_negotiate(
    papinho_accelerator_client_runtime *runtime,
    const PAPACC_CAPABILITY_KEY *supported, PAPACC_SIZE supported_count,
    const PAPACC_CAPABILITY_KEY *preference, PAPACC_SIZE preference_count) {
    PAPACC_U8 accept[PAPACC_FRAME_BASE_HEADER_SIZE +
        PAPACC_CONTROL_VERSION_PAYLOAD_SIZE];
    PAPACC_SIZE accept_length = 0;
    if (papinho_accel_control_selftest_prepare(runtime) != 0 ||
        papinho_accel_control_selftest_accept(accept, sizeof(accept),
        &accept_length) != 0 || papinho_accel_control_feed(runtime, accept,
        accept_length) != 0 || papinho_accel_capability_set_copy(
        runtime->client_supported, &runtime->client_supported_count, supported,
        supported_count) != 0 || papinho_accel_capability_set_copy(
        runtime->client_preference, &runtime->client_preference_count, preference,
        preference_count) != 0 || papinho_accel_control_queue_negotiate(runtime) != 0)
        return -1;
    return runtime->state ==
        PAPINHO_ACCELERATOR_CONNECTION_CAPABILITY_NEGOTIATION_SENT ? 0 : -1;
}

static int papinho_accel_control_selftest_result(PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_length,
    const PAPACC_CAPABILITY_KEY *keys, PAPACC_SIZE key_count) {
    PAPACC_CAPABILITY_KEY storage[PAPINHO_ACCELERATOR_CAPABILITY_MAX];
    PAPACC_CAPABILITY_SET set = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_SIZE header_length = 0;
    PAPACC_SIZE payload_length = 0;
    PAPACC_SIZE i;
    if (output == NULL || out_length == NULL || key_count >
        PAPINHO_ACCELERATOR_CAPABILITY_MAX || (key_count != 0 && keys == NULL) ||
        papacc_capability_set_init(&set, storage, sizeof(storage) /
        sizeof(storage[0])) != PAPACC_RESULT_OK) return -1;
    for (i = 0; i < key_count; ++i)
        if (papacc_capability_set_insert(&set, &keys[i]) != PAPACC_RESULT_OK)
            return -1;
    if (papacc_capability_negotiate_result_encode(&set,
        output + PAPACC_FRAME_BASE_HEADER_SIZE,
        capacity - PAPACC_FRAME_BASE_HEADER_SIZE, &payload_length) !=
        PAPACC_RESULT_OK) return -1;
    header = papacc_capability_negotiate_result_frame_header(
        (PAPACC_U32)payload_length);
    if (papacc_frame_header_encode(&header, output, capacity, &header_length) !=
        PAPACC_RESULT_OK) return -1;
    *out_length = header_length + payload_length;
    return 0;
}

static int papinho_accel_data_selftest_ticket(PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_length,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket) {
    PAPACC_FRAME_HEADER header = papacc_data_ticket_frame_header();
    PAPACC_SIZE header_length = 0;
    PAPACC_SIZE payload_length = 0;
    if (output == NULL || out_length == NULL || ticket == NULL ||
        papacc_frame_header_encode(&header, output, capacity, &header_length) !=
        PAPACC_RESULT_OK || papacc_data_ticket_encode(ticket, output +
        header_length, capacity - header_length, &payload_length) !=
        PAPACC_RESULT_OK) return -1;
    *out_length = header_length + payload_length;
    return 0;
}

static int papinho_accel_data_selftest_accept(PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_length) {
    PAPACC_FRAME_HEADER header = papacc_data_accept_frame_header();
    return output == NULL || out_length == NULL ||
        papacc_frame_header_encode(&header, output, capacity, out_length) !=
        PAPACC_RESULT_OK ? -1 : 0;
}

static int papinho_accel_data_selftest_prepare(
    papinho_accelerator_client_runtime *runtime,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket) {
    PAPACC_U8 result[PAPACC_FRAME_BASE_HEADER_SIZE + 2];
    PAPACC_U8 ticket_frame[PAPACC_FRAME_BASE_HEADER_SIZE +
        PAPACC_DATA_ASSOCIATION_TICKET_SIZE];
    PAPACC_SIZE result_length = 0;
    PAPACC_SIZE ticket_length = 0;
    if (papinho_accel_control_selftest_negotiate(runtime, NULL, 0, NULL, 0) != 0 ||
        papinho_accel_control_selftest_result(result, sizeof(result),
        &result_length, NULL, 0) != 0 || papinho_accel_control_feed(runtime,
        result, result_length) != 0 || papinho_accel_control_queue_ticket_request(
        runtime) != 0 || papinho_accel_data_selftest_ticket(ticket_frame,
        sizeof(ticket_frame), &ticket_length, ticket) != 0 ||
        papinho_accel_control_feed(runtime, ticket_frame, 1) != 0 ||
        papinho_accel_control_feed(runtime, ticket_frame + 1,
        PAPACC_FRAME_BASE_HEADER_SIZE - 1) != 0 || papinho_accel_control_feed(
        runtime, ticket_frame + PAPACC_FRAME_BASE_HEADER_SIZE, 1) != 0 ||
        papinho_accel_control_feed(runtime, ticket_frame +
        PAPACC_FRAME_BASE_HEADER_SIZE + 1, ticket_length -
        PAPACC_FRAME_BASE_HEADER_SIZE - 1) != 0 || runtime->state !=
        PAPINHO_ACCELERATOR_CONNECTION_DATA_TICKET_AVAILABLE ||
        papinho_accelerator_client_transition(runtime,
        PAPINHO_ACCELERATOR_CONNECTION_DATA_CONNECTING) != 0) return -1;
    return 0;
}

static int papinho_accelerator_continue_off_selftest(void) {
    papinho_accelerator_reconnect_state state, waiting, active;
    papinho_accelerator_client_config config;
    papinho_accelerator_reconnect_policy policy;
    unsigned long now = 100UL, g, deadline, jitter, failures;
    int i;
    ZeroMemory(&config, sizeof(config));
    strcpy(config.host, "127.0.0.1"); config.port = 39999;
    papinho_accelerator_reconnect_init(&state, &config, now);
    g = papinho_accelerator_reconnect_begin_attempt(&state, now);
    if (!g || !papinho_accelerator_reconnect_complete(&state, g, 0,
        PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, now)) return -1;
    deadline = state.next_attempt_at_ms;
    policy = state.policy; policy.continue_trying = 0;
    papinho_accelerator_reconnect_set_policy(&state, &policy, now);
    if (state.continued_regime || !state.automatic_attempt_pending ||
        state.next_attempt_at_ms != deadline) return -1;
    policy.continue_trying = 1;
    papinho_accelerator_reconnect_set_policy(&state, &policy, now);
    for (i = 0; i < 5; ++i) {
        now = state.next_attempt_at_ms;
        g = papinho_accelerator_reconnect_begin_attempt(&state, now);
        if (!g || !papinho_accelerator_reconnect_complete(&state, g, 0,
            PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, now)) return -1;
    }
    if (!state.continued_regime || !state.automatic_attempt_pending) return -1;
    waiting = state; jitter = state.jitter_state; failures = state.failure_count;
    policy.continue_trying = 0;
    papinho_accelerator_reconnect_set_policy(&state, &policy, now);
    if (state.automatic_attempt_pending || state.next_attempt_at_ms != 0UL ||
        state.jitter_state != jitter || state.failure_count != failures ||
        !state.continued_regime || papinho_accelerator_reconnect_attempt_due(&state, now)) return -1;
    policy.continue_trying = 1;
    papinho_accelerator_reconnect_set_policy(&state, &policy, now);
    if (!papinho_accelerator_reconnect_attempt_due(&state, now)) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, now);
    policy.continue_trying = 0;
    papinho_accelerator_reconnect_set_policy(&state, &policy, now);
    if (state.availability != PAPINHO_ACCELERATOR_AVAILABILITY_CONNECTING) return -1;
    active = state;
    if (!papinho_accelerator_reconnect_complete(&state, g, 0,
        PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, now) || state.automatic_attempt_pending) return -1;
    if (!papinho_accelerator_reconnect_complete(&active, g, 1,
        PAPINHO_ACCELERATOR_ERROR_NONE, now) || active.failure_count || active.continued_regime ||
        active.availability != PAPINHO_ACCELERATOR_AVAILABILITY_ONLINE) return -1;
    state = waiting;
    papinho_accelerator_reconnect_set_policy(&state, &policy, now);
    papinho_accelerator_reconnect_request_manual(&state, now);
    papinho_accelerator_reconnect_set_policy(&state, &policy, now);
    if (!state.manual_attempt_pending) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, now);
    if (!g || !papinho_accelerator_reconnect_complete(&state, g, 0,
        PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, now) || state.automatic_attempt_pending) return -1;
    return 0;
}
static int papinho_accelerator_reconnect_selftest(void) {
    papinho_accelerator_reconnect_state state; papinho_accelerator_client_config config; papinho_accelerator_reconnect_policy policy; unsigned long g, delay1, delay2;
    if (papinho_accelerator_continue_off_selftest() != 0) return -1;
    ZeroMemory(&config, sizeof(config)); strcpy(config.host, "127.0.0.1"); config.port = 39999; config.profile = PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL;
    papinho_accelerator_reconnect_init(&state, &config, 100UL);
    if (!papinho_accelerator_reconnect_attempt_due(&state, 100UL) || (g = papinho_accelerator_reconnect_begin_attempt(&state, 100UL)) == 0) return -1;
    if (!papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 100UL) || state.availability != PAPINHO_ACCELERATOR_AVAILABILITY_OFFLINE || state.next_attempt_at_ms != 1100UL) return -1;
    if (papinho_accelerator_reconnect_attempt_due(&state, 1099UL) || !papinho_accelerator_reconnect_attempt_due(&state, 1100UL)) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 1100UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 1100UL) || state.next_attempt_at_ms != 4100UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 4100UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 4100UL) || state.next_attempt_at_ms != 14100UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 14100UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 14100UL) || state.next_attempt_at_ms != 44100UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 44100UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 44100UL) || state.next_attempt_at_ms != 104100UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 104100UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 1, PAPINHO_ACCELERATOR_ERROR_NONE, 104100UL) || state.availability != PAPINHO_ACCELERATOR_AVAILABILITY_ONLINE || state.failure_count != 0) return -1;
    papinho_accelerator_reconnect_reconfigure(&state, &config, 200000UL); g = papinho_accelerator_reconnect_begin_attempt(&state, 200000UL); papinho_accelerator_reconnect_disable(&state); if (papinho_accelerator_reconnect_complete(&state, g, 1, PAPINHO_ACCELERATOR_ERROR_NONE, 200001UL) || state.availability != PAPINHO_ACCELERATOR_AVAILABILITY_OFF) return -1;
    papinho_accelerator_reconnect_reconfigure(&state, &config, 300000UL); g = papinho_accelerator_reconnect_begin_attempt(&state, 300000UL); config.profile = PAPINHO_ACCELERATOR_TRANSPORT_LEGACY_ENDPOINT; papinho_accelerator_reconnect_reconfigure(&state, &config, 300001UL); if (papinho_accelerator_reconnect_complete(&state, g, 1, PAPINHO_ACCELERATOR_ERROR_NONE, 300002UL)) return -1;
    state.policy.continue_trying = 0;
    papinho_accelerator_reconnect_reconfigure(&state, &config, 400000UL);
    g = papinho_accelerator_reconnect_begin_attempt(&state, 400000UL);
    if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 400000UL) || !state.automatic_attempt_pending || state.next_attempt_at_ms != 401000UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 401000UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 401000UL) || state.next_attempt_at_ms != 404000UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 404000UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 404000UL) || state.next_attempt_at_ms != 414000UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 414000UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 414000UL) || state.next_attempt_at_ms != 444000UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 444000UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 444000UL) || !state.automatic_attempt_pending || state.next_attempt_at_ms != 504000UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 504000UL); if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 504000UL) || state.automatic_attempt_pending || !state.continued_regime) return -1;
    policy = state.policy; policy.continue_trying = 1;
    papinho_accelerator_reconnect_set_policy(&state, &policy, 504001UL);
    if (!state.automatic_attempt_pending || !papinho_accelerator_reconnect_attempt_due(&state, 504001UL)) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, 504001UL);
    if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, 504001UL) || !state.automatic_attempt_pending) return -1;
    delay1 = state.next_attempt_at_ms - 504001UL;
    if (delay1 < 48000UL || delay1 > 60000UL) return -1;
    g = papinho_accelerator_reconnect_begin_attempt(&state, state.next_attempt_at_ms);
    if (g == 0 || !papinho_accelerator_reconnect_complete(&state, g, 0, PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE, state.next_attempt_at_ms) || !state.automatic_attempt_pending) return -1;
    delay2 = state.next_attempt_at_ms - (504001UL + delay1);
    if (delay2 < 48000UL || delay2 > 60000UL) return -1;
    policy.max_delay_ms = 120000UL;
    papinho_accelerator_reconnect_set_policy(&state, &policy, 700000UL);
    if (!state.automatic_attempt_pending || state.next_attempt_at_ms < 796000UL || state.next_attempt_at_ms > 820000UL) return -1;
    policy.enabled = 0;
    papinho_accelerator_reconnect_set_policy(&state, &policy, 700001UL);
    if (state.automatic_attempt_pending) return -1;
    policy.enabled = 1;
    papinho_accelerator_reconnect_set_policy(&state, &policy, 700002UL);
    if (!state.automatic_attempt_pending || !papinho_accelerator_reconnect_attempt_due(&state, 700002UL)) return -1;
    policy.enabled = 0;
    papinho_accelerator_reconnect_set_policy(&state, &policy, 700003UL);
    papinho_accelerator_reconnect_request_manual(&state, 700003UL);
    if (!state.automatic_attempt_pending || !state.manual_attempt_pending || !papinho_accelerator_reconnect_attempt_due(&state, 700003UL)) return -1;
    return 0;
}

static int papinho_accel_http_framing_append_test(
    papinho_accelerator_mode_b_request *request, const char *text,
    PAPACC_SIZE amount) {
    if (request == NULL || text == NULL || request->response_bytes == NULL ||
        amount > request->response_capacity - request->response_size) return -1;
    memcpy(request->response_bytes + request->response_size, text, (size_t)amount);
    request->response_size += amount;
    return papinho_accel_http_framing_progress(request);
}

static int papinho_accel_http_framing_selftest(void) {
    static const char content_length[] =
        "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhello";
    static const char content_length_zero[] =
        "HTTP/1.1 204 No Content\r\nContent-Length: 0\r\n\r\n";
    static const char chunked[] =
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n";
    static const char premature[] =
        "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nhe";
    static const char malformed[] =
        "HTTP/1.1 200 OK\r\nContent-Length: x\r\n\r\n";
    static const char close_delimited[] =
        "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nabc";
    static const char chunked_with_trailer[] =
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "1\r\na\r\n0\r\nX-Check: ok\r\n\r\n";
    static const char chunked_incomplete_trailer[] =
        "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "1\r\na\r\n0\r\nX-Check: ok\r\n";
    PAPACC_U8 response[512];
    papinho_accelerator_mode_b_request request;
    PAPACC_SIZE i, length;
    int complete_seen;
    ZeroMemory(&request, sizeof(request));
    request.response_bytes = response; request.response_capacity = sizeof(response);
    request.completion_mode = PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    length = (PAPACC_SIZE)strlen(content_length);
    for (i = 0U; i < length; ++i) {
        int rc = papinho_accel_http_framing_append_test(&request,
            content_length + i, 1U);
        if ((i + 1U < length && rc != 0) || (i + 1U == length && rc != 1)) return -1;
    }
    if (!request.http_framing.header_complete || !request.http_framing.content_length_known ||
        request.http_framing.content_length != 5U) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, content_length_zero,
        (PAPACC_SIZE)strlen(content_length_zero)) != 1 ||
        request.http_framing.content_length != 0U) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    length = (PAPACC_SIZE)strlen(chunked); complete_seen = 0;
    for (i = 0U; i < length; ++i) {
        int rc = papinho_accel_http_framing_append_test(&request, chunked + i, 1U);
        if (rc < 0 || (!complete_seen && rc != 0 && rc != 1)) return -1;
        if (rc == 1) complete_seen = 1;
    }
    if (!complete_seen || !request.http_framing.transfer_chunked) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, premature,
        (PAPACC_SIZE)strlen(premature)) != 0 ||
        papinho_accel_http_framing_remote_close(&request) == 0) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, malformed,
        (PAPACC_SIZE)strlen(malformed)) == 0) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, close_delimited,
        (PAPACC_SIZE)strlen(close_delimited)) != 0 ||
        papinho_accel_http_framing_remote_close(&request) != 0 ||
        request.http_framing.state != PAPINHO_ACCELERATOR_HTTP_FRAMING_COMPLETE) return -1;
    request.terminal_reason = PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if (papinho_accel_http_tls_truncated_acceptable(&request)) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, content_length,
        (PAPACC_SIZE)strlen(content_length)) != 1) return -1;
    request.terminal_reason = PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if (!papinho_accel_http_tls_truncated_acceptable(&request)) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, premature,
        (PAPACC_SIZE)strlen(premature)) != 0) return -1;
    request.terminal_reason = PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if (papinho_accel_http_tls_truncated_acceptable(&request)) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, chunked_with_trailer,
        (PAPACC_SIZE)strlen(chunked_with_trailer)) != 1) return -1;
    request.terminal_reason = PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if (!papinho_accel_http_tls_truncated_acceptable(&request)) return -1;
    ZeroMemory(&request, sizeof(request)); request.response_bytes = response;
    request.response_capacity = sizeof(response); request.completion_mode =
        PAPINHO_ACCELERATOR_RESPONSE_COMPLETION_HTTP_FRAMING;
    if (papinho_accel_http_framing_append_test(&request, chunked_incomplete_trailer,
        (PAPACC_SIZE)strlen(chunked_incomplete_trailer)) != 0) return -1;
    request.terminal_reason = PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if (papinho_accel_http_tls_truncated_acceptable(&request)) return -1;
    return 0;
}
static int papinho_accel_network_terminal_selftest(void) {
    papinho_accelerator_client_runtime runtime;
    papinho_accelerator_mode_b_request request;
    papinho_accel_mode_b_receive_state state;
    PAPACC_NETWORK_ABORT_REASON_MESSAGE terminal =
        PAPACC_NETWORK_ABORT_REASON_MESSAGE_INITIALIZER;
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_U8 frame[PAPACC_FRAME_BASE_HEADER_SIZE + 2U];
    PAPACC_U8 data_frame[PAPACC_FRAME_BASE_HEADER_SIZE + 1U];
    PAPACC_SIZE header_size = 0U;
    PAPACC_SIZE payload_size = 0U;
    PAPACC_SIZE i;
    ZeroMemory(&runtime, sizeof(runtime));
    papinho_accelerator_client_runtime_init(&runtime,
        PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL);
    runtime.state = PAPINHO_ACCELERATOR_CONNECTION_READY;
    runtime.network_egress_major = 2U;
    ZeroMemory(&request, sizeof(request));
    request.transport_mode = PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    ZeroMemory(&state, sizeof(state)); state.request = &request;
    state.open_complete = 1;
    terminal.reason = PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    header = papacc_network_frame_header(PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON, 2U);
    if (papacc_frame_header_encode(&header, frame, sizeof(frame), &header_size) !=
        PAPACC_RESULT_OK || header_size != PAPACC_FRAME_BASE_HEADER_SIZE ||
        papacc_network_abort_reason_encode(&terminal, frame + header_size,
        sizeof(frame) - header_size, &payload_size) != PAPACC_RESULT_OK ||
        payload_size != 2U) return -1;
    for (i = 0U; i < header_size + payload_size; ++i)
        if (papinho_accel_mode_b_feed(&runtime, &state, frame + i, 1U) != 0)
            return -1;
    if (!state.terminal_received || !request.terminal_received ||
        request.terminal_reason != PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED)
        return -1;
    header = papacc_network_frame_header(PAPACC_MESSAGE_TYPE_NETWORK_DATA, 1U);
    if (papacc_frame_header_encode(&header, data_frame, sizeof(data_frame),
        &header_size) != PAPACC_RESULT_OK) return -1;
    data_frame[header_size] = 0U;
    if (papinho_accel_mode_b_feed(&runtime, &state, data_frame,
        header_size + 1U) == 0 || runtime.error != PAPINHO_ACCELERATOR_ERROR_PROTOCOL)
        return -1;
    papinho_accelerator_client_runtime_init(&runtime,
        PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL);
    runtime.state = PAPINHO_ACCELERATOR_CONNECTION_READY;
    runtime.network_egress_major = 2U;
    ZeroMemory(&request, sizeof(request));
    request.transport_mode = PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    ZeroMemory(&state, sizeof(state)); state.request = &request;
    state.open_complete = 1;
    frame[PAPACC_FRAME_BASE_HEADER_SIZE] = 0U;
    frame[PAPACC_FRAME_BASE_HEADER_SIZE + 1U] = 2U;
    if (papinho_accel_mode_b_feed(&runtime, &state, frame,
        PAPACC_FRAME_BASE_HEADER_SIZE + 2U) == 0 ||
        request.terminal_reason != 2U ||
        runtime.error != PAPINHO_ACCELERATOR_ERROR_TRANSPORT) return -1;
    papinho_accelerator_client_runtime_init(&runtime,
        PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL);
    runtime.state = PAPINHO_ACCELERATOR_CONNECTION_READY;
    runtime.network_egress_major = 1U;
    ZeroMemory(&request, sizeof(request));
    request.transport_mode = PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    ZeroMemory(&state, sizeof(state)); state.request = &request;
    state.open_complete = 1;
    if (papinho_accel_mode_b_feed(&runtime, &state, frame,
        PAPACC_FRAME_BASE_HEADER_SIZE + 2U) == 0 ||
        runtime.error != PAPINHO_ACCELERATOR_ERROR_PROTOCOL) return -1;
    return 0;
}
static int papinho_accel_response_storage_selftest(void) {
    static const PAPACC_SIZE sizes[] = { 65535U, 65536U, 65537U, 131072U, 524288U, 1048575U, 1048576U };
    papinho_accelerator_response_storage storage;
    PAPACC_U8 chunk[4096];
    PAPACC_SIZE i, remaining, amount, prior_size, prior_capacity;
    ZeroMemory(chunk, sizeof(chunk));
    for (i = 0U; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        ZeroMemory(&storage, sizeof(storage));
        storage.max_size = PAPINHO_ACCELERATOR_PERSISTENT_RESPONSE_MAX;
        remaining = sizes[i];
        while (remaining != 0U) {
            amount = remaining > sizeof(chunk) ? sizeof(chunk) : remaining;
            if (papinho_accel_response_storage_append(&storage, chunk, amount) != 0) {
                papinho_accel_response_storage_release(&storage); return -1;
            }
            remaining -= amount;
        }
        if (storage.size != sizes[i] || storage.capacity < storage.size ||
            storage.capacity > storage.max_size) {
            papinho_accel_response_storage_release(&storage); return -1;
        }
        papinho_accel_response_storage_release(&storage);
    }
    ZeroMemory(&storage, sizeof(storage));
    storage.max_size = PAPINHO_ACCELERATOR_PERSISTENT_RESPONSE_MAX;
    remaining = storage.max_size;
    while (remaining != 0U) {
        amount = remaining > sizeof(chunk) ? sizeof(chunk) : remaining;
        if (papinho_accel_response_storage_append(&storage, chunk, amount) != 0) {
            papinho_accel_response_storage_release(&storage); return -1;
        }
        remaining -= amount;
    }
    prior_size = storage.size; prior_capacity = storage.capacity;
    if (papinho_accel_response_storage_append(&storage, chunk, 1U) == 0 ||
        storage.size != prior_size || storage.capacity != prior_capacity) {
        papinho_accel_response_storage_release(&storage); return -1;
    }
    papinho_accel_response_storage_release(&storage);
    return 0;
}

static int papinho_accel_persistent_terminal_identity_selftest(void) {
    PST_WAIT_EVENT event;
    papinho_accelerator_persistent_connection connection;
    int terminal;
    ZeroMemory(&event, sizeof(event));
    event.token = 1U; event.flags = PST_WAIT_READY_TERMINAL;
    if (papinho_accel_persistent_event_classify(&event, &connection, &terminal) != 0 ||
        connection != PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_CONTROL || !terminal) return -1;
    event.token = 2U;
    if (papinho_accel_persistent_event_classify(&event, &connection, &terminal) != 0 ||
        connection != PAPINHO_ACCELERATOR_PERSISTENT_CONNECTION_DATA || !terminal) return -1;
    event.token = 3U;
    return papinho_accel_persistent_event_classify(&event, &connection, &terminal) == 0 ? -1 : 0;
}

static int papinho_accel_persistent_cancel_all_jobs_lifetime_selftest(void) {
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    papinho_accelerator_persistent_session *candidate_session=NULL;
#define session (*candidate_session)
#else
    papinho_accelerator_persistent_session session;
#endif
    papinho_accelerator_persistent_request_job *queued;
    papinho_accelerator_persistent_request_job *active;
    int result = -1;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    candidate_session=papinho_candidate_session_create();if(!candidate_session)return -1;
#else
    ZeroMemory(&session, sizeof(session));
    papinho_accelerator_persistent_session_init(&session);
#endif
    if (session.jobs == NULL) return -1;
    queued = &session.jobs[0];
    active = &session.jobs[1];
    queued->state = PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_QUEUED;
    queued->cancel_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    queued->completion_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    active->state = PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_ACTIVE;
    active->cancel_event = CreateEvent(NULL, TRUE, FALSE, NULL);
#if !defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    session.active_operation_cancel_event = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
    session.job_queue_count = 1U;
    if (queued->cancel_event == NULL || queued->completion_event == NULL ||
        active->cancel_event == NULL || session.active_operation_cancel_event == NULL)
        goto done;
    papinho_accelerator_persistent_session_cancel_all_jobs(&session);
    if (queued->state != PAPINHO_ACCELERATOR_PERSISTENT_REQUEST_CANCELLED ||
        WaitForSingleObject(queued->cancel_event, 0) != WAIT_OBJECT_0 ||
        WaitForSingleObject(queued->completion_event, 0) != WAIT_OBJECT_0 ||
        WaitForSingleObject(active->cancel_event, 0) != WAIT_OBJECT_0 ||
        WaitForSingleObject(session.active_operation_cancel_event, 0) != WAIT_OBJECT_0)
        goto done;
    result = 0;
done:
    papinho_accelerator_persistent_session_teardown(&session);
    if (result == 0 && session.jobs == NULL)
        papinho_accelerator_persistent_session_cancel_all_jobs(&session);
    else
        result = -1;
#if defined(PAPINHO_PST_TARGET_CANDIDATE_065)
    if(!papinho_candidate_session_finish(candidate_session))result=-1;
#undef session
#endif
    return result;
}

static DWORD WINAPI papinho_close_test_cancel(LPVOID event) {
    SetEvent((HANDLE)event);
    return 0;
}

static int papinho_close_test_sender(papinho_accelerator_client_runtime *runtime,
    const papinho_accelerator_client_config *config, HANDLE cancel,
    PAPACC_U16 type, const PAPACC_U8 *payload, PAPACC_SIZE size) {
    HANDLE thread;
    DWORD joined;
    ++runtime->test_network_open_count; /* isolated call counter */
    if (type != PAPACC_MESSAGE_TYPE_NETWORK_CLOSE || payload != NULL || size)
        return -1;
    if (config->timeout_ms == 3U) {
        /* Send completed, concurrent cancellation arrives before publication.
         * Do not retrospectively relabel successful submission as cancelled. */
        thread = CreateThread(NULL, 0, papinho_close_test_cancel, cancel, 0, NULL);
        if (thread == NULL) return -1;
        joined = WaitForSingleObject(thread, 2000U);
        CloseHandle(thread);
        if (joined != WAIT_OBJECT_0) return -1;
        return 0;
    }
    if (config->timeout_ms == 1U || config->timeout_ms == 2U) {
        runtime->error = config->timeout_ms == 1U ?
            PAPINHO_ACCELERATOR_ERROR_CANCELLED : PAPINHO_ACCELERATOR_ERROR_TRANSPORT;
        return -1;
    }
    return 0;
}

static int papinho_close_result_selftest(void) {
    papinho_accelerator_client_runtime runtime;
    papinho_accelerator_client_config config;
    papinho_accelerator_mode_b_request request;
    HANDLE cancel;
    int i, expected_calls;
    papinho_accelerator_close_send_result expected[8] = {
        PAPINHO_CLOSE_SENT, PAPINHO_CLOSE_CANCELLED, PAPINHO_CLOSE_FAILED,
        PAPINHO_CLOSE_NOT_ATTEMPTED, PAPINHO_CLOSE_SKIPPED_TERMINAL,
        PAPINHO_CLOSE_SENT, PAPINHO_CLOSE_SKIPPED_DATA_CLOSED, PAPINHO_CLOSE_SENT
    };
    cancel = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (cancel == NULL) return -1;
    for (i = 0; i < 8; ++i) {
        papinho_accelerator_client_runtime_init(&runtime,
            PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL);
        ZeroMemory(&config, sizeof(config));
        ZeroMemory(&request, sizeof(request));
        runtime.state = PAPINHO_ACCELERATOR_CONNECTION_READY;
        runtime.control.socket_open = 1;
        runtime.data.socket_open = i != 6;
        request.response_size = i == 3 ? 7U : 12U;
        request.data_result.application_complete = i != 3;
        request.terminal_received = i == 4;
        config.timeout_ms = i == 1 ? 1U : (i == 2 || i == 3 ? 2U : (i == 7 ? 3U : 0U));
        ResetEvent(cancel);
        papinho_accel_mode_b_close_result(&runtime, &config, cancel, &request,
            i == 4, papinho_close_test_sender);
        if (i == 5)
            papinho_accel_mode_b_close_result(&runtime, &config, cancel, &request,
                0, papinho_close_test_sender);
        expected_calls = i == 3 || i == 4 || i == 6 ? 0 : 1;
        if (request.data_result.network_close_send_result != expected[i] ||
            runtime.test_network_open_count != expected_calls ||
            request.normal_close_sent != (expected[i] == PAPINHO_CLOSE_SENT) ||
            request.data_result.application_complete != (i != 3) ||
            request.response_size != (i == 3 ? 7U : 12U) ||
            request.data_result.data_terminal_received != (i == 4) ||
            request.data_result.transport_send_attempted != expected_calls ||
            request.data_result.network_close_error != (i == 1 ?
                PAPINHO_ACCELERATOR_ERROR_CANCELLED : (i == 2 ?
                PAPINHO_ACCELERATOR_ERROR_TRANSPORT : PAPINHO_ACCELERATOR_ERROR_NONE)) ||
            request.data_result.transport_send_error != request.data_result.network_close_error ||
            runtime.state != PAPINHO_ACCELERATOR_CONNECTION_READY ||
            !runtime.control.socket_open || runtime.data.socket_open != (i != 6) ||
            (i == 7 && WaitForSingleObject(cancel, 0) != WAIT_OBJECT_0)) {
            printf("DATA_CLOSE_CASE_%c=FAIL\n", 'A' + i);
            CloseHandle(cancel); return -1;
        }
        printf("DATA_CLOSE_CASE_%c=PASS application=%d close=%d cause=%d terminal=%d transport_attempted=%d calls=%d\n",
            'A' + i, request.data_result.application_complete,
            (int)request.data_result.network_close_send_result,
            request.data_result.network_close_error,
            request.data_result.data_terminal_received,
            request.data_result.transport_send_attempted, expected_calls);
    }
    CloseHandle(cancel);
    return 0;
}

int papinho_accelerator_client_selftest(void) {
    papinho_accelerator_client_runtime runtime;
    PAPACC_CAPABILITY_KEY network =
        PAPACC_PACC_NETWORK_EGRESS_V2_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY network_v1 =
        PAPACC_PACC_NETWORK_EGRESS_V1_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY tls = PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY unknown = { 99U, 1U };
    PAPACC_CAPABILITY_KEY both[2];
    PAPACC_CAPABILITY_KEY both_v1[2];
    PAPACC_CAPABILITY_KEY one_key[1];
    PAPACC_CAPABILITY_KEY two_keys[2];
    PAPACC_CAPABILITY_SET one_set = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET two_set = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_U8 result_bytes[128];
    PAPACC_SIZE result_length = 0;
    PAPACC_DATA_ASSOCIATION_TICKET ticket = PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_U8 data_accept[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE data_accept_length = 0;
    PAPACC_FRAME_HEADER header;
    PAPACC_U8 encoded[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE written = 0;
    if (papinho_close_result_selftest() != 0 ||
        papinho_accel_persistent_cancel_all_jobs_lifetime_selftest() != 0 ||
        papinho_accelerator_reconnect_selftest() != 0 ||
        papinho_accel_response_storage_selftest() != 0 ||
        papinho_accel_http_framing_selftest() != 0 ||
        papinho_accel_network_terminal_selftest() != 0 ||
        papinho_accel_persistent_terminal_identity_selftest() != 0) return -1;
    ticket.bytes[0] = 1;
    both[0] = network; both[1] = tls;
    if (papacc_capability_set_init(&one_set, one_key, 1) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&one_set, &network) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&one_set, &network) !=
        PAPACC_RESULT_INVALID_STATE || papacc_capability_set_insert(&one_set,
        &tls) != PAPACC_RESULT_LIMIT_EXCEEDED ||
        papacc_capability_set_init(&two_set, two_keys, 2) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&two_set, &network) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&two_set, &tls) != PAPACC_RESULT_OK ||
        papacc_capability_negotiate_encode(&one_set, &two_set, result_bytes,
        sizeof(result_bytes), &result_length) == PAPACC_RESULT_OK) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, both, 2) != 0 ||
        runtime.control.output_end <= PAPACC_FRAME_BASE_HEADER_SIZE ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, both, 2) != 0) return -1;
    if (papinho_accel_control_feed(&runtime, result_bytes, 1) != 0 ||
        papinho_accel_control_feed(&runtime, result_bytes + 1,
        PAPACC_FRAME_BASE_HEADER_SIZE - 1) != 0 ||
        papinho_accel_control_feed(&runtime, result_bytes +
        PAPACC_FRAME_BASE_HEADER_SIZE, 1) != 0 || papinho_accel_control_feed(
        &runtime, result_bytes + PAPACC_FRAME_BASE_HEADER_SIZE + 1,
        result_length - PAPACC_FRAME_BASE_HEADER_SIZE - 1) != 0 ||
        runtime.state != PAPINHO_ACCELERATOR_CONNECTION_CAPABILITIES_NEGOTIATED ||
        runtime.server_granted_count != 2 ||
        !papinho_accelerator_client_mode_b_requirements_granted(&runtime)) return -1;
    both_v1[0] = network_v1; both_v1[1] = tls;
    if (papinho_accel_control_selftest_negotiate(&runtime, both_v1, 2, both_v1, 2) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, both_v1, 2) != 0 || papinho_accel_control_feed(&runtime,
        result_bytes, result_length) != 0 || runtime.network_egress_major != 1U ||
        !papinho_accelerator_client_mode_b_requirements_granted(&runtime)) return -1;
    if (papinho_accel_control_queue_negotiate(&runtime) == 0) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, &network, 1) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, &network, 1) != 0 || papinho_accel_control_feed(&runtime,
        result_bytes, result_length) != 0 ||
        !papinho_accelerator_client_capability_granted(&runtime, &network) ||
        papinho_accelerator_client_capability_granted(&runtime, &tls) ||
        papinho_accelerator_client_mode_b_requirements_granted(&runtime)) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, &tls, 1) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, &tls, 1) != 0 || papinho_accel_control_feed(&runtime,
        result_bytes, result_length) != 0 ||
        papinho_accelerator_client_capability_granted(&runtime, &network) ||
        !papinho_accelerator_client_capability_granted(&runtime, &tls) ||
        papinho_accelerator_client_mode_b_requirements_granted(&runtime)) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, NULL, 0) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, NULL, 0) != 0 || papinho_accel_control_feed(&runtime,
        result_bytes, result_length) != 0 || runtime.server_granted_count != 0 ||
        papinho_accelerator_client_capability_granted(&runtime, &network)) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, NULL, 0) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, &unknown, 1) != 0 || papinho_accel_control_feed(&runtime,
        result_bytes, result_length) != 0 || runtime.server_granted_count != 1 ||
        !papinho_accelerator_client_capability_granted(&runtime, &unknown) ||
        papinho_accelerator_client_capability_granted(&runtime, &network)) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, both, 2) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, both, 2) != 0) return -1;
    memcpy(result_bytes + result_length, result_bytes, result_length);
    if (papinho_accel_control_feed(&runtime, result_bytes, result_length * 2) == 0 ||
        runtime.error != PAPINHO_ACCELERATOR_ERROR_PROTOCOL) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, both, 2) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, both, 2) != 0) return -1;
    result_bytes[0] = 0;
    if (papinho_accel_control_feed(&runtime, result_bytes, result_length) == 0 ||
        runtime.error != PAPINHO_ACCELERATOR_ERROR_PROTOCOL) return -1;
    if (papinho_accel_control_selftest_negotiate(&runtime, both, 2, both, 2) != 0 ||
        papinho_accel_control_selftest_result(result_bytes, sizeof(result_bytes),
        &result_length, both, 2) != 0) return -1;
    result_bytes[PAPACC_FRAME_BASE_HEADER_SIZE + 2 +
        PAPACC_CAPABILITY_WIRE_KEY_SIZE + 3] = 1;
    if (papinho_accel_control_feed(&runtime, result_bytes, result_length) == 0 ||
        runtime.error != PAPINHO_ACCELERATOR_ERROR_PROTOCOL) return -1;
    if (papinho_accel_data_selftest_prepare(&runtime, &ticket) != 0 ||
        runtime.control.parser.state == runtime.data.parser.state &&
        &runtime.control == &runtime.data) return -1;
    ZeroMemory(&header, sizeof(header));
    if (papacc_frame_header_decode(runtime.control.output_buffer,
        runtime.control.output_end, PAPINHO_ACCELERATOR_MAX_FRAME_PAYLOAD,
        &header) != PAPACC_RESULT_OK || header.message_type !=
        PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST || header.payload_length != 0 ||
        !runtime.data_ticket_valid || runtime.data_ticket_consumed ||
        papinho_accel_data_queue_attach(&runtime) != 0 ||
        runtime.data.output_end != PAPACC_FRAME_BASE_HEADER_SIZE +
        PAPACC_DATA_ASSOCIATION_TICKET_SIZE || papinho_accel_data_selftest_accept(
        data_accept, sizeof(data_accept), &data_accept_length) != 0) return -1;
    runtime.data.output_begin = 1;
    if (runtime.data.output_begin >= runtime.data.output_end) return -1;
    for (written = 0; written < data_accept_length; ++written)
        if (papinho_accel_data_feed(&runtime, data_accept + written, 1) != 0)
            return -1;
    if (runtime.state != PAPINHO_ACCELERATOR_CONNECTION_DATA_ATTACHED ||
        !runtime.data_ticket_consumed ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_READY) != 0) return -1;
    if (papinho_accel_data_selftest_prepare(&runtime, &ticket) != 0 ||
        papinho_accel_data_queue_attach(&runtime) != 0) return -1;
    header = papacc_data_ticket_request_frame_header();
    if (papacc_frame_header_encode(&header, data_accept, sizeof(data_accept),
        &data_accept_length) != PAPACC_RESULT_OK || papinho_accel_data_feed(
        &runtime, data_accept, data_accept_length) == 0 || runtime.error !=
        PAPINHO_ACCELERATOR_ERROR_DATA_ATTACH) return -1;
    if (papinho_accel_data_selftest_prepare(&runtime, &ticket) != 0) return -1;
    papinho_accelerator_client_session_reset(&runtime);
    if (runtime.data_ticket_valid || runtime.data_ticket_session_generation != 0 ||
        papinho_accelerator_client_consume_ticket(&runtime) == 0) return -1;
    if (papinho_accel_control_selftest_prepare(&runtime) != 0) return -1;
    if (papinho_accel_control_fail(&runtime,
        PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE) == 0 ||
        runtime.state == PAPINHO_ACCELERATOR_CONNECTION_CONTROL_ACCEPTED ||
        runtime.error != PAPINHO_ACCELERATOR_ERROR_REMOTE_CLOSE) return -1;
    if (papinho_accel_control_selftest_prepare(&runtime) != 0) return -1;
    if (papinho_accel_control_fail(&runtime,
        PAPINHO_ACCELERATOR_ERROR_TIMEOUT) == 0 || runtime.error !=
        PAPINHO_ACCELERATOR_ERROR_TIMEOUT) return -1;
    if (papinho_accel_control_selftest_prepare(&runtime) != 0 ||
        papinho_accel_control_fail(&runtime,
        PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE) == 0 || runtime.error !=
        PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE) return -1;
    if (papinho_accel_control_selftest_prepare(&runtime) != 0) return -1;
    papinho_accelerator_client_teardown(&runtime);
    if (runtime.control.socket_open || runtime.state !=
        PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED) return -1;
    papinho_accelerator_client_runtime_init(&runtime,
        PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL);
    if (runtime.state != PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED ||
        &runtime.control == &runtime.data ||
        runtime.control.parser.state != PAPACC_FRAME_PARSER_STATE_READING_HEADER ||
        runtime.data.parser.state != PAPACC_FRAME_PARSER_STATE_READING_HEADER ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_READY) == 0) return -1;
    if (papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CONNECTING) != 0 ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_TRANSPORT_CONNECTED) != 0 ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_SECURE_TRANSPORT_READY) != 0 ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT) != 0) return -1;
    runtime.client_supported[0].id = 1U;
    runtime.client_supported[0].major = 1U;
    runtime.client_supported_count = 1;
    runtime.client_preference[0] = runtime.client_supported[0];
    runtime.client_preference_count = 1;
    runtime.server_granted_count = 0;
    if (papinho_accelerator_client_set_ticket(&runtime, &ticket) != 0 ||
        papinho_accelerator_client_consume_ticket(&runtime) != 0) return -1;
    papinho_accelerator_client_session_reset(&runtime);
    if (runtime.client_supported_count != 0 || runtime.client_preference_count != 0 ||
        runtime.server_granted_count != 0 || runtime.data_ticket_valid ||
        runtime.data_ticket_consumed || papinho_accelerator_client_consume_ticket(&runtime) == 0 ||
        runtime.state != PAPINHO_ACCELERATOR_CONNECTION_DISCONNECTED) return -1;
    papinho_accelerator_client_runtime_init(&runtime,
        PAPINHO_ACCELERATOR_TRANSPORT_LEGACY_ENDPOINT);
    if (papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CONNECTING) != 0 ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_TRANSPORT_CONNECTED) != 0 ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_SECURE_TRANSPORT_READY) == 0 ||
        papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_CONTROL_OPEN_SENT) != 0) return -1;
    header = papacc_data_attach_frame_header();
    if (papacc_frame_header_encode(&header, encoded, sizeof(encoded), &written) !=
        PAPACC_RESULT_OK || written != sizeof(encoded) ||
        encoded[8] != 0 || encoded[9] != 5) return -1;
    if (papinho_accelerator_client_transition(&runtime,
        PAPINHO_ACCELERATOR_CONNECTION_TERMINAL) != 0 ||
        runtime.state != PAPINHO_ACCELERATOR_CONNECTION_TERMINAL) return -1;
    papinho_accelerator_client_teardown(&runtime);
    return runtime.control.socket_open || runtime.data.socket_open ? -1 : 0;
}
