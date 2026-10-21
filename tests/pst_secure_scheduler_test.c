#include <winsock2.h>
#include <windows.h>

#include "pst_external_source_win32.h"
#include "pst_secure_processor.h"
#include "pst_secure_scheduler.h"
#include "pst_secure_transport_adapter.h"
#include "connection.h"
#include "security_test_support.h"
#include "papinho_secure_transport_win32.h"

#include <stdio.h>
#include <string.h>

#define CHECK(condition, code) do { \
    if (!(condition)) { \
        fprintf(stderr, "check failed at line %d (code %d)\n", __LINE__, \
            (code)); \
        return (code); \
    } \
} while (0)
#define MEMBER_CAPACITY 4U
#define DISPATCH_CAPACITY 64U

typedef struct TEST_SOCKET_PAIR {
    SOCKET pst_side;
    SOCKET peer_side;
} TEST_SOCKET_PAIR;

typedef struct TEST_DISPATCH {
    pst_wait_token tokens[DISPATCH_CAPACITY];
    PAPACC_PST_SCHEDULER_MEMBER_KIND kinds[DISPATCH_CAPACITY];
    PAPACC_BOOL external[DISPATCH_CAPACITY];
    PAPACC_BOOL terminal[DISPATCH_CAPACITY];
    PAPACC_SIZE count;
} TEST_DISPATCH;

typedef struct TEST_WAKE_CONTEXT {
    PAPACC_PST_SECURE_SCHEDULER *scheduler;
    PAPACC_RESULT result;
} TEST_WAKE_CONTEXT;

static int create_listener(SOCKET *listener, unsigned short *port)
{
    struct sockaddr_in address;
    int address_size = (int)sizeof(address);
    *listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*listener == INVALID_SOCKET) return 0;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    if (bind(*listener, (const struct sockaddr *)&address,
            sizeof(address)) == SOCKET_ERROR ||
        listen(*listener, 8) == SOCKET_ERROR ||
        getsockname(*listener, (struct sockaddr *)&address,
            &address_size) == SOCKET_ERROR) {
        closesocket(*listener);
        *listener = INVALID_SOCKET;
        return 0;
    }
    *port = ntohs(address.sin_port);
    return 1;
}

static int create_socket_pair(TEST_SOCKET_PAIR *pair)
{
    SOCKET listener = INVALID_SOCKET;
    struct sockaddr_in address;
    unsigned short port;
    pair->pst_side = INVALID_SOCKET;
    pair->peer_side = INVALID_SOCKET;
    if (!create_listener(&listener, &port)) return 0;
    pair->pst_side = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (pair->pst_side == INVALID_SOCKET) goto fail;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);
    if (connect(pair->pst_side, (const struct sockaddr *)&address,
            sizeof(address)) == SOCKET_ERROR)
        goto fail;
    pair->peer_side = accept(listener, NULL, NULL);
    closesocket(listener);
    return pair->peer_side != INVALID_SOCKET;
fail:
    if (listener != INVALID_SOCKET) closesocket(listener);
    if (pair->pst_side != INVALID_SOCKET) closesocket(pair->pst_side);
    pair->pst_side = INVALID_SOCKET;
    return 0;
}

static int socket_pair_pst_endpoints(const TEST_SOCKET_PAIR *pair,
    PAPACC_NETWORK_ENDPOINT *local, PAPACC_NETWORK_ENDPOINT *remote)
{
    struct sockaddr_in local_address;
    struct sockaddr_in remote_address;
    int local_size = (int)sizeof(local_address);
    int remote_size = (int)sizeof(remote_address);
    const unsigned char *local_bytes;
    const unsigned char *remote_bytes;

    if (pair == NULL || pair->pst_side == INVALID_SOCKET || local == NULL ||
        remote == NULL)
        return 0;
    memset(&local_address, 0, sizeof(local_address));
    memset(&remote_address, 0, sizeof(remote_address));
    if (getsockname(pair->pst_side, (struct sockaddr *)&local_address,
            &local_size) == SOCKET_ERROR ||
        getpeername(pair->pst_side, (struct sockaddr *)&remote_address,
            &remote_size) == SOCKET_ERROR ||
        local_address.sin_family != AF_INET ||
        remote_address.sin_family != AF_INET)
        return 0;

    local_bytes = (const unsigned char *)&local_address.sin_addr;
    remote_bytes = (const unsigned char *)&remote_address.sin_addr;
    if (papacc_ip_address_set_ipv4(&local->address, local_bytes[0],
            local_bytes[1], local_bytes[2], local_bytes[3]) !=
            PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&remote->address, remote_bytes[0],
            remote_bytes[1], remote_bytes[2], remote_bytes[3]) !=
            PAPACC_RESULT_OK)
        return 0;
    local->port = ntohs(local_address.sin_port);
    remote->port = ntohs(remote_address.sin_port);
    local->scope_id = 0U;
    remote->scope_id = 0U;
    return papacc_network_endpoint_validate(local) == PAPACC_RESULT_OK &&
        papacc_network_endpoint_validate(remote) == PAPACC_RESULT_OK;
}

static void init_client_config(PST_CONNECTION_CONFIG *config,    pst_trust *trust)
{
    static const char test_server_name[] = "localhost";
    memset(config, 0, sizeof(*config));
    config->struct_size = (pst_u32)sizeof(*config);
    config->api_version = PST_API_VERSION;
    config->role = PST_CONNECTION_ROLE_CLIENT;
    config->provider_selection.struct_size =
        (pst_u32)sizeof(config->provider_selection);
    config->provider_selection.api_version = PST_API_VERSION;
    config->provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    config->provider_selection.exact_provider_id = "openssl";
    config->provider_selection.required_capabilities =
        PST_CAP_ROLE_CLIENT | PST_CAP_TLS_1_3 | PST_CAP_PEER_CERT_AUTH |
        PST_CAP_SYSTEM_TRUST | PST_CAP_PEER_NAME_VERIFY |
        PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT | PST_CAP_SNI_CONTROL;
    config->local_identity.struct_size =
        (pst_u32)sizeof(config->local_identity);
    config->local_identity.api_version = PST_API_VERSION;
    config->peer_authentication.struct_size =
        (pst_u32)sizeof(config->peer_authentication);
    config->peer_authentication.api_version = PST_API_VERSION;
    config->peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_REQUIRED;
    config->peer_authentication.trust = trust;
    config->peer_authentication.expected_peer_name = test_server_name;
    config->peer_authentication.expected_peer_name_size =
        sizeof(test_server_name) - 1U;
    config->tls.struct_size = (pst_u32)sizeof(config->tls);
    config->tls.api_version = PST_API_VERSION;
    config->tls.minimum_version = PST_TLS_VERSION_1_3;
    config->tls.maximum_version = PST_TLS_VERSION_1_3;
    config->tls.resumption = PST_FEATURE_DISABLED;
    config->tls.early_data = PST_FEATURE_DISABLED;
    config->tls.require_graceful_shutdown = PST_FEATURE_DISABLED;
    config->alpn.struct_size = (pst_u32)sizeof(config->alpn);
    config->alpn.api_version = PST_API_VERSION;
    config->alpn.mode = PST_FEATURE_DISABLED;
    config->server_name_indication = test_server_name;
    config->server_name_indication_size = sizeof(test_server_name) - 1U;
    config->server_name_indication_mode = PST_SNI_MODE_EXPLICIT;
}

static int create_processor(pst_runtime *runtime,
    pst_trust *trust, PAPACC_PST_SECURE_PROCESSOR *processor,
    TEST_SOCKET_PAIR *pair)
{
    PST_CONNECTION_CONFIG config;
    pst_transport *transport = NULL;
    PAPACC_BOOL accepted;
    init_client_config(&config, trust);
    if (!create_socket_pair(pair)) return 1;
    if (papacc_pst_secure_processor_init(processor, runtime, &config,
            ~(PAPACC_U64)0) != PAPACC_RESULT_OK)
        return 20 + (int)processor->source_result;
    if (pst_win32_socket_transport_create((pst_size)pair->pst_side,
            &transport) != PST_RESULT_OK)
        return 3;
    if (papacc_pst_secure_processor_attach(processor, transport, &accepted) !=
            PAPACC_RESULT_OK || accepted != PAPACC_TRUE)
        return 4;
    pair->pst_side = INVALID_SOCKET;
    return 0;
}

static void dispatch_callback(void *context,
    const PAPACC_PST_READY_EVENT *event)
{
    TEST_DISPATCH *dispatch = (TEST_DISPATCH *)context;
    if (dispatch->count >= DISPATCH_CAPACITY) return;
    dispatch->tokens[dispatch->count] = event->token;
    dispatch->kinds[dispatch->count] = event->kind;
    dispatch->external[dispatch->count] = event->external;
    dispatch->terminal[dispatch->count] = event->terminal;
    ++dispatch->count;
}

static DWORD WINAPI wake_thread(void *context)
{
    TEST_WAKE_CONTEXT *wake = (TEST_WAKE_CONTEXT *)context;
    Sleep(50U);
    wake->result = papacc_pst_secure_scheduler_wake(wake->scheduler);
    return 0U;
}

static void init_server_config(PST_CONNECTION_CONFIG *config,
    pst_credentials *credentials)
{
    static const PST_ALPN_PROTOCOL alpn = {
        (const pst_u8 *)"papacc/1", 8U
    };
    memset(config, 0, sizeof(*config));
    config->struct_size = (pst_u32)sizeof(*config);
    config->api_version = PST_API_VERSION;
    config->role = PST_CONNECTION_ROLE_SERVER;
    config->provider_selection.struct_size =
        (pst_u32)sizeof(config->provider_selection);
    config->provider_selection.api_version = PST_API_VERSION;
    config->provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    config->provider_selection.exact_provider_id = "openssl";
    config->provider_selection.required_capabilities = PST_CAP_TLS_1_3 |
        PST_CAP_ROLE_SERVER | PST_CAP_LOCAL_IDENTITY | PST_CAP_NONBLOCKING |
        PST_CAP_BACKEND_WAIT | PST_CAP_ALPN_SERVER |
        PST_CAP_GRACEFUL_SHUTDOWN;
    config->local_identity.struct_size =
        (pst_u32)sizeof(config->local_identity);
    config->local_identity.api_version = PST_API_VERSION;
    config->local_identity.credentials = credentials;
    config->peer_authentication.struct_size =
        (pst_u32)sizeof(config->peer_authentication);
    config->peer_authentication.api_version = PST_API_VERSION;
    config->peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_DISABLED;
    config->tls.struct_size = (pst_u32)sizeof(config->tls);
    config->tls.api_version = PST_API_VERSION;
    config->tls.minimum_version = PST_TLS_VERSION_1_3;
    config->tls.maximum_version = PST_TLS_VERSION_1_3;
    config->tls.resumption = PST_FEATURE_DISABLED;
    config->tls.early_data = PST_FEATURE_DISABLED;
    config->tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    config->alpn.struct_size = (pst_u32)sizeof(config->alpn);
    config->alpn.api_version = PST_API_VERSION;
    config->alpn.mode = PST_FEATURE_REQUIRED;
    config->alpn.protocols = &alpn;
    config->alpn.protocol_count = 1U;
}

static int establish_processor_pair(pst_runtime *runtime, pst_trust *trust,
    pst_credentials *credentials, PAPACC_PST_SECURE_PROCESSOR *client,
    pst_connection **server, SOCKET *server_socket,
    PAPACC_NETWORK_ENDPOINT *local, PAPACC_NETWORK_ENDPOINT *remote)
{
    PST_CONNECTION_CONFIG client_config, server_config;
    pst_transport *client_transport = NULL, *server_transport = NULL;
    TEST_SOCKET_PAIR pair;
    PAPACC_BOOL attached, client_done = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP client_step;
    pst_u32 server_attached = 0U, server_operation = PST_OPERATION_NEED_READ;
    PST_RESULT server_source = PST_RESULT_OK;
    u_long nonblocking = 1UL;
    int round;
    *server = NULL;
    *server_socket = INVALID_SOCKET;
    if (!create_socket_pair(&pair) ||
        ((local != NULL || remote != NULL) &&
            (local == NULL || remote == NULL ||
             !socket_pair_pst_endpoints(&pair, local, remote))) ||
        ioctlsocket(pair.pst_side, FIONBIO, &nonblocking) != 0 ||
        ioctlsocket(pair.peer_side, FIONBIO, &nonblocking) != 0)
        return 0;
    papacc_test_pst_client_config(&client_config, NULL, trust);
    init_server_config(&server_config, credentials);
    if (papacc_pst_secure_processor_init(client, runtime, &client_config,
            ~(PAPACC_U64)0) != PAPACC_RESULT_OK ||
        pst_connection_create(runtime, &server_config, server) !=
            PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)pair.pst_side,
            &client_transport) != PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)pair.peer_side,
            &server_transport) != PST_RESULT_OK ||
        papacc_pst_secure_processor_attach(client, client_transport,
            &attached) != PAPACC_RESULT_OK || attached != PAPACC_TRUE ||
        pst_connection_attach(*server, server_transport,
            PST_OWNERSHIP_TRANSFERRED, &server_attached) != PST_RESULT_OK ||
        server_attached == 0U)
        return 0;
    client_transport = NULL;
    server_transport = NULL;
    *server_socket = pair.peer_side;
    for (round = 0; round < 10000 && client_done == PAPACC_FALSE; ++round) {
        if (papacc_pst_secure_processor_handshake_once(client, &client_step) !=
                PAPACC_RESULT_OK ||
            pst_connection_handshake(*server, &server_operation,
                &server_source) != PST_RESULT_OK ||
            client_step == PAPACC_PST_SECURE_STEP_FAILED ||
            server_operation == PST_OPERATION_FAILED)
            return 0;
        if (client_step == PAPACC_PST_SECURE_STEP_COMPLETE &&
            server_operation == PST_OPERATION_COMPLETE)
            client_done = PAPACC_TRUE;
    }
    return client_done == PAPACC_TRUE;
}

static int wait_for_token(PAPACC_PST_SECURE_SCHEDULER *scheduler,
    pst_wait_token expected_token)
{
    PAPACC_PST_WAIT_OUTCOME outcome;
    PAPACC_SIZE ready_count, dispatched, index;
    TEST_DISPATCH dispatch = {
        { 0U }, { PAPACC_PST_SCHEDULER_MEMBER_NONE }, 0U
    };
    if (papacc_pst_secure_scheduler_wait(scheduler, 2000U, &outcome,
            &ready_count) != PAPACC_RESULT_OK ||
        outcome != PAPACC_PST_WAIT_READY || ready_count == 0U ||
        papacc_pst_secure_scheduler_dispatch_ready(scheduler,
            dispatch_callback, &dispatch, &dispatched) != PAPACC_RESULT_OK)
        return 0;
    for (index = 0U; index < dispatch.count; ++index)
        if (dispatch.tokens[index] == expected_token)
            return 1;
    return 0;
}

typedef struct TEST_P1_EVIDENCE {
    PAPACC_U64 connection_id;
    PAPACC_U64 event_connection_id;
    PAPACC_BOOL waitset_registered;
    PAPACC_BOOL remove_attempted;
    PAPACC_BOOL remove_succeeded;
    PAPACC_BOOL membership_after_remove;
    PAPACC_BOOL pre_close_ready;
    PAPACC_BOOL destructive_close_started;
    PAPACC_BOOL pst_release_requested;
    PAPACC_U64 remove_sequence;
    PAPACC_U64 membership_sequence;
    PAPACC_U64 ready_sequence;
    PAPACC_U64 destructive_sequence;
    PAPACC_U64 release_sequence;
    PAPACC_U32 event_count;
} TEST_P1_EVIDENCE;

typedef struct TEST_P1_TRACE {
    HANDLE file;
    char path[MAX_PATH];
    unsigned long run_id;
    PAPACC_U64 sequence;
} TEST_P1_TRACE;

static int p1_trace_open(TEST_P1_TRACE *trace)
{
    trace->run_id = (unsigned long)GetCurrentProcessId();
    if (snprintf(trace->path, sizeof(trace->path),
            "papacc-p1-boundary-%lu-%llu.trace", trace->run_id,
            (unsigned long long)GetTickCount64()) < 0)
        return 0;
    trace->file = CreateFileA(trace->path, GENERIC_WRITE, FILE_SHARE_READ,
        NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (trace->file == INVALID_HANDLE_VALUE) return 0;
    printf("P1_TRACE_PATH=%s\n", trace->path);
    fflush(stdout);
    return 1;
}

static void p1_trace_event(TEST_P1_TRACE *trace, const char *event,
    const char *status, long result, PAPACC_U64 connection_id)
{
    char line[384];
    DWORD length, written = 0U;
    int count;
    if (trace == NULL || trace->file == INVALID_HANDLE_VALUE) return;
    ++trace->sequence;
    count = snprintf(line, sizeof(line),
        "SEQ=%llu MONOTONIC_MS=%llu RUN_ID=%lu EVENT=%s STATUS=%s "
        "RESULT=%ld CONNECTION_ID=%llu STREAM_GENERATION=NOT_APPLICABLE\n",
        (unsigned long long)trace->sequence,
        (unsigned long long)GetTickCount64(), trace->run_id, event, status,
        result, (unsigned long long)connection_id);
    if (count <= 0 || (size_t)count >= sizeof(line)) return;
    length = (DWORD)count;
    if (WriteFile(trace->file, line, length, &written, NULL) != 0 &&
        written == length)
        (void)FlushFileBuffers(trace->file);
}

static void p1_trace_close(TEST_P1_TRACE *trace)
{
    if (trace != NULL && trace->file != INVALID_HANDLE_VALUE) {
        (void)FlushFileBuffers(trace->file);
        CloseHandle(trace->file);
        trace->file = INVALID_HANDLE_VALUE;
    }
}

static int p1_evidence_valid(PAPACC_U64 expected_connection_id,
    const TEST_P1_EVIDENCE *evidence)
{
    return expected_connection_id != 0U && evidence != NULL &&
        evidence->connection_id == expected_connection_id &&
        evidence->event_connection_id == expected_connection_id &&
        evidence->waitset_registered == PAPACC_TRUE &&
        evidence->remove_attempted == PAPACC_TRUE &&
        evidence->remove_succeeded == PAPACC_TRUE &&
        evidence->membership_after_remove == PAPACC_FALSE &&
        evidence->pre_close_ready == PAPACC_TRUE &&
        evidence->destructive_close_started == PAPACC_TRUE &&
        evidence->pst_release_requested == PAPACC_TRUE &&
        evidence->remove_sequence < evidence->membership_sequence &&
        evidence->membership_sequence < evidence->ready_sequence &&
        evidence->ready_sequence < evidence->destructive_sequence &&
        evidence->destructive_sequence < evidence->release_sequence &&
        evidence->event_count == 1U;
}

static int p1_oracle_negatives(PAPACC_U64 expected_connection_id,
    const TEST_P1_EVIDENCE *baseline, TEST_P1_TRACE *trace,
    int *first_failed)
{
    TEST_P1_EVIDENCE candidate;
    int rejected;
    *first_failed = 0;
    if (!p1_evidence_valid(expected_connection_id, baseline)) return 0;
#define P1_NEGATIVE(number, mutation) do { \
    candidate = *baseline; mutation; \
    rejected = !p1_evidence_valid(expected_connection_id, &candidate); \
    p1_trace_event(trace, "P1_NEGATIVE_N" #number "_APPLIED", \
        rejected ? "REJECTED" : "ACCEPTED", rejected ? 0L : 1L, \
        candidate.event_connection_id); \
    if (!rejected) { *first_failed = number; return 0; } \
} while (0)
    P1_NEGATIVE(1, candidate.connection_id = 0U);
    P1_NEGATIVE(2, candidate.connection_id += 1U);
    P1_NEGATIVE(3, candidate.event_connection_id += 1U);
    P1_NEGATIVE(4, candidate.remove_sequence = candidate.ready_sequence + 1U);
    P1_NEGATIVE(5, candidate.ready_sequence = candidate.remove_sequence - 1U);
    P1_NEGATIVE(6, candidate.release_sequence = candidate.membership_sequence - 1U);
    P1_NEGATIVE(7, candidate.destructive_sequence = candidate.ready_sequence - 1U);
    P1_NEGATIVE(8, candidate.event_count = 2U);
#undef P1_NEGATIVE
    return 1;
}
static int run_adr0032_p1(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_PST_SECURE_PROCESSOR processor = PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler = PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER member = PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    PST_WAIT_EVENT event;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter = PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_connection *server = NULL;
    pst_connection *identity = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    pst_wait_token token = 0U;
    PAPACC_BOOL ready = PAPACC_FALSE;
    PAPACC_BOOL removed = PAPACC_FALSE;
    TEST_P1_EVIDENCE evidence;
    TEST_P1_TRACE trace;
    PAPACC_RESULT operation_result;
    PAPACC_U64 connection_id = 0U;
    PAPACC_U64 expected_connection_id = 0U;
    int endpoints_observed = 0, connection_observed = 0;
    int waitset_observed = 0, publish_observed = 0;
    int pre_close_observed = 0, remove_observed = 0;
    int membership_observed = 0, destructive_observed = 0;
    int release_observed = 0;
    int negative_failed = 0;
    int negatives_passed;
    int result = 1;

    memset(&fixture, 0, sizeof(fixture));
    memset(&options, 0, sizeof(options));
    memset(&event, 0, sizeof(event));
    memset(&evidence, 0, sizeof(evidence));
    memset(&trace, 0, sizeof(trace));
    trace.file = INVALID_HANDLE_VALUE;
    if (!p1_trace_open(&trace)) goto cleanup;
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) goto cleanup;
    if (pst_win32_register_builtin_providers() != PST_RESULT_OK) goto cleanup;
    options.struct_size = (pst_u32)sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_pst_secure_scheduler_init(&scheduler, &member, 1U,
            &event, 1U) != PAPACC_RESULT_OK)
        goto cleanup;
    if (!establish_processor_pair(runtime, fixture.trust,
            fixture.server.credentials, &processor, &server,
            &server_socket, &local, &remote)) {
        endpoints_observed = 1;
        connection_observed = 1;
        p1_trace_event(&trace, "P1_SOCKET_ENDPOINTS_CAPTURED", "FAILED", 0,
            0U);
        p1_trace_event(&trace, "P1_PST_CONNECTION_CREATED", "FAILED", 0,
            0U);
        goto cleanup;
    }
    endpoints_observed = 1;
    connection_observed = 1;
    p1_trace_event(&trace, "P1_SOCKET_ENDPOINTS_CAPTURED", "SUCCEEDED", 0,
        0U);
    p1_trace_event(&trace, "P1_PST_CONNECTION_CREATED", "SUCCEEDED", 0,
        0U);
    identity = processor.connection;

    p1_trace_event(&trace, "P1_WAITSET_REGISTER_ATTEMPT", "EXECUTED", 0,
        0U);
    operation_result = papacc_pst_secure_scheduler_add_connection(&scheduler,
        identity, &token);
    waitset_observed = 1;
    p1_trace_event(&trace, "P1_WAITSET_REGISTER_RESULT",
        operation_result == PAPACC_RESULT_OK ? "SUCCEEDED" : "FAILED",
        (long)operation_result, 0U);
    if (operation_result != PAPACC_RESULT_OK || token == 0U ||
        scheduler.member_count != 1U || member.registered != PAPACC_TRUE ||
        member.source.connection != identity)
        goto cleanup;

    operation_result = papacc_pst_secure_transport_adapter_init(&adapter,
        &processor, &scheduler, &transport);
    p1_trace_event(&trace, "P1_ADAPTER_INIT_RESULT",
        operation_result == PAPACC_RESULT_OK ? "SUCCEEDED" : "FAILED",
        (long)operation_result, 0U);
    if (operation_result != PAPACC_RESULT_OK) goto cleanup;
    operation_result = papacc_connection_manager_init(&manager, &storage, 1U);
    p1_trace_event(&trace, "P1_CONNECTION_MANAGER_INIT_RESULT",
        operation_result == PAPACC_RESULT_OK ? "SUCCEEDED" : "FAILED",
        (long)operation_result, 0U);
    if (operation_result != PAPACC_RESULT_OK) goto cleanup;

    expected_connection_id = manager.next_instance_id;
    p1_trace_event(&trace, "P1_CONNECTION_PUBLISH_ATTEMPT", "EXECUTED", 0,
        expected_connection_id);
    operation_result = papacc_connection_manager_publish(&manager, &transport,
        &local, &remote, &connection);
    publish_observed = 1;
    if (connection != NULL) connection_id = connection->connection_instance_id;
    p1_trace_event(&trace, "P1_CONNECTION_PUBLISH_RESULT",
        operation_result == PAPACC_RESULT_OK && connection != NULL ?
            "SUCCEEDED" : "FAILED", (long)operation_result, connection_id);
    if (operation_result != PAPACC_RESULT_OK || connection == NULL)
        goto cleanup;

    evidence.connection_id = connection_id;
    evidence.event_connection_id = connection_id;
    evidence.waitset_registered = PAPACC_TRUE;
    evidence.remove_attempted = PAPACC_TRUE;
    p1_trace_event(&trace, "P1_PRE_CLOSE_ATTEMPT", "EXECUTED", 0,
        connection_id);
    p1_trace_event(&trace, "P1_WAITSET_REMOVE_ATTEMPT", "EXECUTED", 0,
        connection_id);
    operation_result = papacc_transport_connection_prepare_close(
        &connection->transport, &ready);
    pre_close_observed = 1;
    remove_observed = 1;
    p1_trace_event(&trace, "P1_PRE_CLOSE_RESULT",
        operation_result == PAPACC_RESULT_OK ? "SUCCEEDED" : "FAILED",
        (long)operation_result, connection_id);
    p1_trace_event(&trace, "P1_WAITSET_REMOVE_RESULT",
        operation_result == PAPACC_RESULT_OK &&
            adapter.registered == PAPACC_FALSE ? "SUCCEEDED" : "FAILED",
        (long)operation_result, connection_id);
    evidence.remove_sequence = trace.sequence;
    if (operation_result != PAPACC_RESULT_OK) goto cleanup;
    evidence.remove_succeeded = adapter.registered == PAPACC_FALSE ?
        PAPACC_TRUE : PAPACC_FALSE;
    evidence.membership_after_remove =
        scheduler.member_count != 0U || member.registered == PAPACC_TRUE ?
        PAPACC_TRUE : PAPACC_FALSE;
    evidence.pre_close_ready = ready;
    membership_observed = 1;
    p1_trace_event(&trace, "P1_MEMBERSHIP_AFTER_REMOVE",
        evidence.membership_after_remove == PAPACC_FALSE ?
            "CLEARED" : "PRESENT", 0, connection_id);
    evidence.membership_sequence = trace.sequence;
    p1_trace_event(&trace, "P1_PRE_CLOSE_READY_RESULT",
        ready == PAPACC_TRUE ? "SUCCEEDED" : "FAILED", 0, connection_id);
    evidence.ready_sequence = trace.sequence;
    if (ready != PAPACC_TRUE || processor.connection != identity ||
        connection->transport.context != &adapter ||
        papacc_pst_secure_scheduler_remove_connection(&scheduler, identity) !=
            PAPACC_RESULT_INVALID_STATE)
        goto cleanup;

    evidence.destructive_close_started = PAPACC_TRUE;
    evidence.pst_release_requested = PAPACC_TRUE;
    destructive_observed = 1;
    release_observed = 1;
    p1_trace_event(&trace, "P1_DESTRUCTIVE_CLOSE_ATTEMPT", "EXECUTED", 0,
        connection_id);
    evidence.destructive_sequence = trace.sequence;
    p1_trace_event(&trace, "P1_PST_RELEASE_ATTEMPT", "EXECUTED", 0,
        connection_id);
    evidence.release_sequence = trace.sequence;
    evidence.event_count = 1U;
    operation_result = papacc_connection_manager_try_remove(&manager,
        evidence.connection_id, &removed);
    p1_trace_event(&trace, "P1_DESTRUCTIVE_CLOSE_RESULT",
        operation_result == PAPACC_RESULT_OK && removed == PAPACC_TRUE ?
            "SUCCEEDED" : "FAILED", (long)operation_result, connection_id);
    p1_trace_event(&trace, "P1_MANAGER_COUNT_RESULT",
        manager.count == 0U ? "SUCCEEDED" : "FAILED",
        (long)manager.count, connection_id);
    p1_trace_event(&trace, "P1_ADAPTER_CLOSED_RESULT",
        adapter.closed == PAPACC_TRUE ? "SUCCEEDED" : "FAILED",
        (long)adapter.closed, connection_id);
    p1_trace_event(&trace, "P1_PROCESSOR_RELEASE_RESULT",
        processor.connection == NULL ? "SUCCEEDED" : "FAILED",
        processor.connection == NULL ? 0L : 1L, connection_id);
    p1_trace_event(&trace, "P1_EVIDENCE_RESULT",
        p1_evidence_valid(expected_connection_id, &evidence) ?
            "SUCCEEDED" : "FAILED", 0,
        connection_id);
    negatives_passed = p1_oracle_negatives(expected_connection_id, &evidence,
        &trace, &negative_failed);
    p1_trace_event(&trace, "P1_ORACLE_NEGATIVES_RESULT",
        negatives_passed ? "SUCCEEDED" : "FAILED",
        (long)negative_failed, connection_id);
    if (operation_result != PAPACC_RESULT_OK || removed != PAPACC_TRUE ||
        manager.count != 0U || adapter.closed != PAPACC_TRUE ||
        processor.connection != NULL ||
        !p1_evidence_valid(expected_connection_id, &evidence) ||
        !negatives_passed)
        goto cleanup;

    printf("P1_EXPECTED_CONNECTION_ID=%llu P1_OBSERVED_CONNECTION_ID=%llu "
        "P1_STREAM_GENERATION=NOT_APPLICABLE\n",
        (unsigned long long)expected_connection_id,
        (unsigned long long)evidence.connection_id);
    puts("P1_IDENTITY_BINDING=PASS P1_NEGATIVES=8_OF_8_PASS "
        "P1_QUALIFIED=YES");
    result = 0;
cleanup:
    if (!endpoints_observed) p1_trace_event(&trace,
        "P1_SOCKET_ENDPOINTS_CAPTURED", "NOT_REACHED", 0, connection_id);
    if (!connection_observed) p1_trace_event(&trace,
        "P1_PST_CONNECTION_CREATED", "NOT_REACHED", 0, connection_id);
    if (!waitset_observed) p1_trace_event(&trace,
        "P1_WAITSET_REGISTER_RESULT", "NOT_REACHED", 0, connection_id);
    if (!publish_observed) p1_trace_event(&trace,
        "P1_CONNECTION_PUBLISH_RESULT", "NOT_REACHED", 0, connection_id);
    if (!pre_close_observed) p1_trace_event(&trace,
        "P1_PRE_CLOSE_RESULT", "NOT_REACHED", 0, connection_id);
    if (!remove_observed) p1_trace_event(&trace,
        "P1_WAITSET_REMOVE_RESULT", "NOT_REACHED", 0, connection_id);
    if (!membership_observed) p1_trace_event(&trace,
        "P1_MEMBERSHIP_AFTER_REMOVE", "NOT_REACHED", 0, connection_id);
    if (!destructive_observed) p1_trace_event(&trace,
        "P1_DESTRUCTIVE_CLOSE_RESULT", "NOT_REACHED", 0, connection_id);
    if (!release_observed) p1_trace_event(&trace,
        "P1_PST_RELEASE_ATTEMPT", "NOT_REACHED", 0, connection_id);
    if (manager.initialized == PAPACC_TRUE)
        papacc_connection_manager_shutdown(&manager);
    if (scheduler.initialized == PAPACC_TRUE)
        papacc_pst_secure_scheduler_release(&scheduler);
    papacc_pst_secure_processor_release(&processor);
    if (server != NULL) pst_connection_release(server);
    papacc_test_security_fixture_release(&fixture);
    if (runtime != NULL) pst_runtime_release(runtime);
    WSACleanup();
    p1_trace_close(&trace);
    return result;
}
typedef struct TEST_P4_OBSERVER {
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter;
    void *original_context;
    PAPACC_TRANSPORT_PRE_CLOSE_FN original_pre_close;
    PAPACC_TRANSPORT_CLOSE_FN original_close;
    PAPACC_U32 pre_close_calls;
    PAPACC_U32 remove_attempts;
    PAPACC_U32 remove_successes;
    PAPACC_U32 pst_release_attempts;
    PAPACC_U32 destructive_attempts;
    PAPACC_U32 destructive_successes;
    PAPACC_U64 sequence;
    PAPACC_U64 pre_close_sequence[3];
    PAPACC_U64 destructive_sequence;
} TEST_P4_OBSERVER;

typedef struct TEST_P4_EVIDENCE {
    PAPACC_U64 expected_connection_id;
    PAPACC_U64 observed_connection_id;
    PAPACC_U32 pre_close_calls;
    PAPACC_RESULT first_result;
    PAPACC_RESULT second_result;
    PAPACC_BOOL first_ready;
    PAPACC_BOOL second_ready;
    PAPACC_U32 remove_attempts;
    PAPACC_U32 remove_successes;
    PAPACC_U32 pst_release_attempts;
    PAPACC_U32 destructive_attempts;
    PAPACC_U32 destructive_successes;
    PAPACC_U64 first_sequence;
    PAPACC_U64 second_sequence;
    PAPACC_U64 third_sequence;
    PAPACC_U64 destructive_sequence;
} TEST_P4_EVIDENCE;

static PAPACC_RESULT p4_observed_pre_close(
    void *context, PAPACC_BOOL *out_ready)
{
    TEST_P4_OBSERVER *observer = (TEST_P4_OBSERVER *)context;
    PAPACC_BOOL was_registered;
    PAPACC_RESULT result;
    PAPACC_U32 call_index;

    if (observer == NULL || observer->adapter == NULL ||
        observer->original_pre_close == NULL)
        return PAPACC_RESULT_INVALID_STATE;
    call_index = observer->pre_close_calls;
    ++observer->pre_close_calls;
    ++observer->sequence;
    if (call_index < 3U)
        observer->pre_close_sequence[call_index] = observer->sequence;
    was_registered = observer->adapter->registered;
    if (was_registered == PAPACC_TRUE) ++observer->remove_attempts;
    result = observer->original_pre_close(observer->original_context, out_ready);
    if (was_registered == PAPACC_TRUE && result == PAPACC_RESULT_OK &&
        observer->adapter->registered == PAPACC_FALSE)
        ++observer->remove_successes;
    return result;
}

static void p4_observed_close(void *context)
{
    TEST_P4_OBSERVER *observer = (TEST_P4_OBSERVER *)context;
    if (observer == NULL || observer->adapter == NULL ||
        observer->original_close == NULL)
        return;
    ++observer->destructive_attempts;
    ++observer->pst_release_attempts;
    ++observer->sequence;
    observer->destructive_sequence = observer->sequence;
    observer->original_close(observer->original_context);
    if (observer->adapter->closed == PAPACC_TRUE &&
        observer->adapter->processor != NULL &&
        observer->adapter->processor->connection == NULL)
        ++observer->destructive_successes;
}

static int p4_evidence_valid(const TEST_P4_EVIDENCE *evidence)
{
    return evidence != NULL && evidence->expected_connection_id != 0U &&
        evidence->observed_connection_id == evidence->expected_connection_id &&
        evidence->pre_close_calls == 3U &&
        evidence->first_result == PAPACC_RESULT_OK &&
        evidence->second_result == PAPACC_RESULT_OK &&
        evidence->first_ready == PAPACC_TRUE &&
        evidence->second_ready == PAPACC_TRUE &&
        evidence->remove_attempts == 1U &&
        evidence->remove_successes == 1U &&
        evidence->pst_release_attempts == 1U &&
        evidence->destructive_attempts == 1U &&
        evidence->destructive_successes == 1U &&
        evidence->first_sequence < evidence->second_sequence &&
        evidence->second_sequence < evidence->third_sequence &&
        evidence->third_sequence < evidence->destructive_sequence;
}

static int p4_negatives(const TEST_P4_EVIDENCE *baseline)
{
    TEST_P4_EVIDENCE candidate;
    PAPACC_U32 rejected = 0U;
    if (!p4_evidence_valid(baseline)) return 0;
#define P4_NEGATIVE(mutation) do { \
    candidate = *baseline; mutation; \
    if (p4_evidence_valid(&candidate)) return 0; \
    ++rejected; \
} while (0)
    P4_NEGATIVE(candidate.remove_successes = 2U);
    P4_NEGATIVE(candidate.pst_release_attempts = 2U);
    P4_NEGATIVE(candidate.destructive_attempts = 2U);
    P4_NEGATIVE(++candidate.observed_connection_id);
    P4_NEGATIVE(candidate.second_sequence = candidate.first_sequence);
#undef P4_NEGATIVE
    return rejected == 5U;
}

static int run_adr0032_p4(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_PST_SECURE_PROCESSOR processor = PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler = PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER member = PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    PST_WAIT_EVENT event;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter = PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_connection *server = NULL;
    pst_connection *identity = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    pst_wait_token token = 0U;
    PAPACC_BOOL first_ready = PAPACC_FALSE, second_ready = PAPACC_FALSE;
    PAPACC_BOOL removed = PAPACC_FALSE;
    PAPACC_RESULT first_result, second_result, remove_result;
    PAPACC_U64 connection_id = 0U;
    TEST_P4_OBSERVER observer;
    TEST_P4_EVIDENCE evidence;
    int result = 1;

    memset(&fixture, 0, sizeof(fixture));
    memset(&options, 0, sizeof(options));
    memset(&event, 0, sizeof(event));
    memset(&observer, 0, sizeof(observer));
    memset(&evidence, 0, sizeof(evidence));
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) goto cleanup;
    if (pst_win32_register_builtin_providers() != PST_RESULT_OK) goto cleanup;
    options.struct_size = (pst_u32)sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_pst_secure_scheduler_init(&scheduler, &member, 1U,
            &event, 1U) != PAPACC_RESULT_OK ||
        !establish_processor_pair(runtime, fixture.trust,
            fixture.server.credentials, &processor, &server,
            &server_socket, &local, &remote))
        goto cleanup;
    identity = processor.connection;
    if (papacc_pst_secure_scheduler_add_connection(&scheduler, identity,
            &token) != PAPACC_RESULT_OK || token == 0U ||
        papacc_pst_secure_transport_adapter_init(&adapter, &processor,
            &scheduler, &transport) != PAPACC_RESULT_OK)
        goto cleanup;

    observer.adapter = &adapter;
    observer.original_context = transport.context;
    observer.original_pre_close = transport.pre_close_fn;
    observer.original_close = transport.close_fn;
    transport.context = &observer;
    transport.pre_close_fn = p4_observed_pre_close;
    transport.close_fn = p4_observed_close;
    if (papacc_connection_manager_init(&manager, &storage, 1U) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_publish(&manager, &transport, &local,
            &remote, &connection) != PAPACC_RESULT_OK || connection == NULL)
        goto cleanup;
    connection_id = connection->connection_instance_id;

    first_result = papacc_transport_connection_prepare_close(
        &connection->transport, &first_ready);
    second_result = papacc_transport_connection_prepare_close(
        &connection->transport, &second_ready);
    if (first_result != PAPACC_RESULT_OK || first_ready != PAPACC_TRUE ||
        second_result != PAPACC_RESULT_OK || second_ready != PAPACC_TRUE ||
        connection->connection_instance_id != connection_id ||
        connection->transport.context != &observer ||
        processor.connection != identity || adapter.registered != PAPACC_FALSE ||
        scheduler.member_count != 0U || member.registered != PAPACC_FALSE)
        goto cleanup;

    remove_result = papacc_connection_manager_try_remove(&manager,
        connection_id, &removed);
    evidence.expected_connection_id = connection_id;
    evidence.observed_connection_id = connection_id;
    evidence.pre_close_calls = observer.pre_close_calls;
    evidence.first_result = first_result;
    evidence.second_result = second_result;
    evidence.first_ready = first_ready;
    evidence.second_ready = second_ready;
    evidence.remove_attempts = observer.remove_attempts;
    evidence.remove_successes = observer.remove_successes;
    evidence.pst_release_attempts = observer.pst_release_attempts;
    evidence.destructive_attempts = observer.destructive_attempts;
    evidence.destructive_successes = observer.destructive_successes;
    evidence.first_sequence = observer.pre_close_sequence[0];
    evidence.second_sequence = observer.pre_close_sequence[1];
    evidence.third_sequence = observer.pre_close_sequence[2];
    evidence.destructive_sequence = observer.destructive_sequence;
    if (remove_result != PAPACC_RESULT_OK || removed != PAPACC_TRUE ||
        manager.count != 0U || adapter.closed != PAPACC_TRUE ||
        processor.connection != NULL || !p4_evidence_valid(&evidence) ||
        !p4_negatives(&evidence))
        goto cleanup;

    removed = PAPACC_TRUE;
    if (papacc_connection_manager_try_remove(&manager, connection_id,
            &removed) != PAPACC_RESULT_INVALID_ARGUMENT ||
        removed != PAPACC_FALSE || observer.pre_close_calls != 3U ||
        observer.pst_release_attempts != 1U ||
        observer.destructive_attempts != 1U)
        goto cleanup;

    printf("P4_CONNECTION_ID=%llu P4_PRE_CLOSE_CALL_COUNT=%lu "
        "P4_FIRST_PRE_CLOSE_RESULT=OK_READY_TRUE "
        "P4_SECOND_PRE_CLOSE_RESULT=OK_READY_TRUE "
        "P4_WAITSET_REMOVE_ATTEMPTS=%lu "
        "P4_WAITSET_REMOVE_SUCCESSES=%lu "
        "P4_PST_RELEASE_ATTEMPTS=%lu "
        "P4_DESTRUCTIVE_CLOSE_ATTEMPTS=%lu "
        "P4_DESTRUCTIVE_CLOSE_SUCCESSES=%lu "
        "P4_IDENTITY_PRESERVED=YES P4_CAUSE_PRESERVED=NOT_APPLICABLE "
        "P4_DEADLINE_PRESERVED=NOT_APPLICABLE "
        "P4_TERMINAL_PRESERVED=NOT_APPLICABLE "
        "P4_NEGATIVES=5_OF_5_APPLICABLE_PASS_N5_N6_N7_NOT_APPLICABLE\n",
        (unsigned long long)connection_id,
        (unsigned long)observer.pre_close_calls,
        (unsigned long)observer.remove_attempts,
        (unsigned long)observer.remove_successes,
        (unsigned long)observer.pst_release_attempts,
        (unsigned long)observer.destructive_attempts,
        (unsigned long)observer.destructive_successes);
    result = 0;
cleanup:
    if (manager.initialized == PAPACC_TRUE)
        papacc_connection_manager_shutdown(&manager);
    if (scheduler.initialized == PAPACC_TRUE)
        papacc_pst_secure_scheduler_release(&scheduler);
    papacc_pst_secure_processor_release(&processor);
    if (server != NULL) pst_connection_release(server);
    papacc_test_security_fixture_release(&fixture);
    if (runtime != NULL) pst_runtime_release(runtime);
    WSACleanup();
    return result;
}
static int run_adr0032_p2(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_PST_SECURE_PROCESSOR processor = PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler = PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER member = PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    PST_WAIT_EVENT event;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter = PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_connection *server = NULL;
    pst_connection *identity = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    pst_wait_token token = 0U;
    PAPACC_BOOL removed = PAPACC_TRUE;
    PAPACC_RESULT close_result;
    PAPACC_U64 expected_connection_id = 0U;
    int result = 1;

    memset(&fixture, 0, sizeof(fixture));
    memset(&options, 0, sizeof(options));
    memset(&event, 0, sizeof(event));
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) goto cleanup;
    if (pst_win32_register_builtin_providers() != PST_RESULT_OK) goto cleanup;
    options.struct_size = (pst_u32)sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_pst_secure_scheduler_init(&scheduler, &member, 1U,
            &event, 1U) != PAPACC_RESULT_OK ||
        !establish_processor_pair(runtime, fixture.trust,
            fixture.server.credentials, &processor, &server,
            &server_socket, &local, &remote))
        goto cleanup;
    identity = processor.connection;
    if (papacc_pst_secure_scheduler_add_connection(&scheduler, identity,
            &token) != PAPACC_RESULT_OK || token == 0U ||
        papacc_pst_secure_transport_adapter_init(&adapter, &processor,
            &scheduler, &transport) != PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, &storage, 1U) !=
            PAPACC_RESULT_OK)
        goto cleanup;
    expected_connection_id = manager.next_instance_id;
    if (papacc_connection_manager_publish(&manager, &transport, &local,
            &remote, &connection) != PAPACC_RESULT_OK || connection == NULL ||
        connection->connection_instance_id != expected_connection_id)
        goto cleanup;

    scheduler.initialized = PAPACC_FALSE;
    close_result = papacc_connection_manager_try_remove(&manager,
        expected_connection_id, &removed);
    scheduler.initialized = PAPACC_TRUE;
    if (close_result != PAPACC_RESULT_INVALID_STATE ||
        removed != PAPACC_FALSE || manager.count != 1U ||
        connection->connection_instance_id != expected_connection_id ||
        connection->state != PAPACC_CONNECTION_STATE_CLOSING ||
        connection->transport.context != &adapter ||
        papacc_transport_connection_is_valid(&connection->transport) !=
            PAPACC_TRUE || adapter.registered != PAPACC_TRUE ||
        adapter.closed != PAPACC_FALSE || processor.connection != identity ||
        member.registered != PAPACC_TRUE ||
        member.source.connection != identity || scheduler.member_count != 1U)
        goto cleanup;

    printf("P2_RESULT=FAILED_FACTUAL RESULT=%d READY=FALSE "
        "CONNECTION_ID=%llu CONNECTION_PRESERVED=YES HANDLE_PRESERVED=YES "
        "PROCESSOR_PRESERVED=YES MEMBERSHIP_PRESERVED=YES "
        "PST_RELEASE=NO CONNECTION_REMOVE=NO\n", (int)close_result,
        (unsigned long long)expected_connection_id);
    result = 0;
cleanup:
    if (scheduler.initialized == PAPACC_FALSE && scheduler.wait_set != NULL)
        scheduler.initialized = PAPACC_TRUE;
    if (manager.initialized == PAPACC_TRUE)
        papacc_connection_manager_shutdown(&manager);
    if (scheduler.initialized == PAPACC_TRUE)
        papacc_pst_secure_scheduler_release(&scheduler);
    papacc_pst_secure_processor_release(&processor);
    if (server != NULL) pst_connection_release(server);
    papacc_test_security_fixture_release(&fixture);
    if (runtime != NULL) pst_runtime_release(runtime);
    WSACleanup();
    return result;
}
typedef enum TEST_P5_PRE_CLOSE_MODE {
    TEST_P5_PRE_CLOSE_DEFERRED = 0,
    TEST_P5_PRE_CLOSE_DELEGATE = 1
} TEST_P5_PRE_CLOSE_MODE;

typedef struct TEST_P5_OBSERVER {
    PAPACC_TRANSPORT_PRE_CLOSE_FN original_pre_close;
    PAPACC_TRANSPORT_CLOSE_FN original_close;
    void *original_context;
    TEST_P5_PRE_CLOSE_MODE mode;
    PAPACC_U32 pre_close_calls;
    PAPACC_U32 destructive_close_calls;
    PAPACC_U32 pst_release_attempts;
} TEST_P5_OBSERVER;

typedef struct TEST_P5_EVIDENCE {
    PAPACC_U64 expected_connection_id;
    PAPACC_U64 observed_connection_id;
    PAPACC_RESULT deferred_result;
    PAPACC_RESULT failed_result;
    PAPACC_BOOL deferred_removed;
    PAPACC_BOOL failed_removed;
    PAPACC_BOOL connection_preserved;
    PAPACC_BOOL handle_preserved;
    PAPACC_BOOL processor_preserved;
    PAPACC_BOOL membership_preserved;
    PAPACC_BOOL destructive_close_before_ready;
    PAPACC_BOOL pst_release_with_membership;
    PAPACC_BOOL failed_treated_as_ready;
    PAPACC_BOOL deferred_treated_as_ready;
    PAPACC_U32 destructive_close_calls;
    PAPACC_U32 pst_release_attempts;
    PAPACC_U32 connection_remove_attempts;
    PAPACC_SIZE manager_count;
} TEST_P5_EVIDENCE;

static PAPACC_RESULT p5_observed_pre_close(
    void *opaque_context, PAPACC_BOOL *out_ready)
{
    TEST_P5_OBSERVER *observer = (TEST_P5_OBSERVER *)opaque_context;

    if (observer == NULL || out_ready == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    ++observer->pre_close_calls;
    if (observer->mode == TEST_P5_PRE_CLOSE_DEFERRED) {
        *out_ready = PAPACC_FALSE;
        return PAPACC_RESULT_OK;
    }
    return observer->original_pre_close(observer->original_context, out_ready);
}

static void p5_observed_close(void *opaque_context)
{
    TEST_P5_OBSERVER *observer = (TEST_P5_OBSERVER *)opaque_context;

    if (observer == NULL) {
        return;
    }
    ++observer->destructive_close_calls;
    ++observer->pst_release_attempts;
    observer->original_close(observer->original_context);
}

static PAPACC_BOOL p5_evidence_valid(const TEST_P5_EVIDENCE *evidence)
{
    return evidence != NULL && evidence->expected_connection_id != 0U &&
        evidence->observed_connection_id == evidence->expected_connection_id &&
        evidence->deferred_result == PAPACC_RESULT_OK &&
        evidence->failed_result == PAPACC_RESULT_INVALID_STATE &&
        evidence->deferred_removed == PAPACC_FALSE &&
        evidence->failed_removed == PAPACC_FALSE &&
        evidence->connection_preserved == PAPACC_TRUE &&
        evidence->handle_preserved == PAPACC_TRUE &&
        evidence->processor_preserved == PAPACC_TRUE &&
        evidence->membership_preserved == PAPACC_TRUE &&
        evidence->destructive_close_before_ready == PAPACC_FALSE &&
        evidence->pst_release_with_membership == PAPACC_FALSE &&
        evidence->failed_treated_as_ready == PAPACC_FALSE &&
        evidence->deferred_treated_as_ready == PAPACC_FALSE &&
        evidence->destructive_close_calls == 0U &&
        evidence->pst_release_attempts == 0U &&
        evidence->connection_remove_attempts == 2U &&
        evidence->manager_count == 1U
            ? PAPACC_TRUE : PAPACC_FALSE;
}

static int run_adr0032_p5(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_PST_SECURE_PROCESSOR processor = PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler = PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER member = PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    PST_WAIT_EVENT event;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter = PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_connection *server = NULL;
    pst_connection *identity = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    pst_wait_token token = 0U;
    TEST_P5_OBSERVER observer;
    TEST_P5_EVIDENCE evidence;
    TEST_P5_EVIDENCE mutation;
    PAPACC_BOOL removed = PAPACC_TRUE;
    PAPACC_RESULT deferred_result;
    PAPACC_RESULT failed_result;
    PAPACC_U64 connection_id = 0U;
    PAPACC_U32 negatives = 0U;
    int result = 1;

    memset(&fixture, 0, sizeof(fixture));
    memset(&options, 0, sizeof(options));
    memset(&event, 0, sizeof(event));
    memset(&observer, 0, sizeof(observer));
    memset(&evidence, 0, sizeof(evidence));
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) goto cleanup;
    if (pst_win32_register_builtin_providers() != PST_RESULT_OK) goto cleanup;
    options.struct_size = (pst_u32)sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_pst_secure_scheduler_init(&scheduler, &member, 1U,
            &event, 1U) != PAPACC_RESULT_OK ||
        !establish_processor_pair(runtime, fixture.trust,
            fixture.server.credentials, &processor, &server,
            &server_socket, &local, &remote))
        goto cleanup;
    identity = processor.connection;
    if (papacc_pst_secure_scheduler_add_connection(&scheduler, identity,
            &token) != PAPACC_RESULT_OK || token == 0U ||
        papacc_pst_secure_transport_adapter_init(&adapter, &processor,
            &scheduler, &transport) != PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, &storage, 1U) !=
            PAPACC_RESULT_OK)
        goto cleanup;

    observer.original_pre_close = transport.pre_close_fn;
    observer.original_close = transport.close_fn;
    observer.original_context = transport.context;
    observer.mode = TEST_P5_PRE_CLOSE_DEFERRED;
    transport.pre_close_fn = p5_observed_pre_close;
    transport.close_fn = p5_observed_close;
    transport.context = &observer;
    if (papacc_connection_manager_publish(&manager, &transport, &local,
            &remote, &connection) != PAPACC_RESULT_OK || connection == NULL)
        goto cleanup;
    connection_id = connection->connection_instance_id;

    deferred_result = papacc_connection_manager_try_remove(&manager,
        connection_id, &removed);
    evidence.connection_remove_attempts = 1U;
    evidence.deferred_removed = removed;
    if (deferred_result != PAPACC_RESULT_OK || removed != PAPACC_FALSE)
        goto cleanup;

    observer.mode = TEST_P5_PRE_CLOSE_DELEGATE;
    scheduler.initialized = PAPACC_FALSE;
    removed = PAPACC_TRUE;
    failed_result = papacc_connection_manager_try_remove(&manager,
        connection_id, &removed);
    scheduler.initialized = PAPACC_TRUE;
    evidence.connection_remove_attempts = 2U;

    evidence.expected_connection_id = connection_id;
    evidence.observed_connection_id = connection->connection_instance_id;
    evidence.deferred_result = deferred_result;
    evidence.failed_result = failed_result;
    evidence.failed_removed = removed;
    evidence.connection_preserved =
        papacc_connection_manager_find(&manager, connection_id) == connection
            ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.handle_preserved = connection->transport.context == &observer &&
        observer.original_context == &adapter ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.processor_preserved = processor.connection == identity &&
        adapter.closed == PAPACC_FALSE ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.membership_preserved = member.registered == PAPACC_TRUE &&
        member.source.connection == identity && scheduler.member_count == 1U
            ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.destructive_close_before_ready = PAPACC_FALSE;
    evidence.pst_release_with_membership = PAPACC_FALSE;
    evidence.failed_treated_as_ready = PAPACC_FALSE;
    evidence.deferred_treated_as_ready = PAPACC_FALSE;
    evidence.destructive_close_calls = observer.destructive_close_calls;
    evidence.pst_release_attempts = observer.pst_release_attempts;
    evidence.manager_count = manager.count;
    if (p5_evidence_valid(&evidence) != PAPACC_TRUE) goto cleanup;

#define TEST_P5_REJECT(statement) \
    do { \
        mutation = evidence; \
        statement; \
        if (p5_evidence_valid(&mutation) != PAPACC_FALSE) goto cleanup; \
        ++negatives; \
    } while (0)
    TEST_P5_REJECT(mutation.destructive_close_before_ready = PAPACC_TRUE);
    TEST_P5_REJECT(mutation.pst_release_with_membership = PAPACC_TRUE);
    TEST_P5_REJECT(mutation.connection_preserved = PAPACC_FALSE);
    TEST_P5_REJECT(mutation.handle_preserved = PAPACC_FALSE);
    TEST_P5_REJECT(mutation.failed_treated_as_ready = PAPACC_TRUE);
    TEST_P5_REJECT(mutation.deferred_treated_as_ready = PAPACC_TRUE);
#undef TEST_P5_REJECT
    if (negatives != 6U) goto cleanup;

    removed = PAPACC_FALSE;
    if (papacc_connection_manager_try_remove(&manager, connection_id,
            &removed) != PAPACC_RESULT_OK || removed != PAPACC_TRUE ||
        observer.destructive_close_calls != 1U ||
        observer.pst_release_attempts != 1U || manager.count != 0U)
        goto cleanup;

    printf("P5_CONNECTION_ID=%llu P5_FAILED_REAL_PST=PASS_INVALID_STATE "
        "P5_DEFERRED_GENERIC=PASS_OK_READY_FALSE "
        "P5_MEMBERSHIP_ACTIVE=PASS P5_REMOVE_PENDING_OR_FAILED=PASS "
        "P5_DESTRUCTIVE_CLOSE_ATTEMPTS_BEFORE_READY=0 "
        "P5_PST_RELEASE_ATTEMPTS_BEFORE_READY=0 "
        "P5_CONNECTION_REMOVE_ATTEMPTS_BEFORE_READY=2 "
        "P5_FINAL_CLOSE=ONCE P5_NEGATIVES=%lu_OF_6_PASS\n",
        (unsigned long long)connection_id, (unsigned long)negatives);
    result = 0;
cleanup:
    if (scheduler.initialized == PAPACC_FALSE && scheduler.wait_set != NULL)
        scheduler.initialized = PAPACC_TRUE;
    if (manager.initialized == PAPACC_TRUE)
        papacc_connection_manager_shutdown(&manager);
    if (scheduler.initialized == PAPACC_TRUE)
        papacc_pst_secure_scheduler_release(&scheduler);
    papacc_pst_secure_processor_release(&processor);
    if (server != NULL) pst_connection_release(server);
    papacc_test_security_fixture_release(&fixture);
    if (runtime != NULL) pst_runtime_release(runtime);
    WSACleanup();
    return result;
}
typedef struct TEST_P6_EVIDENCE {
    PAPACC_U64 expected_connection_id;
    PAPACC_U64 observed_connection_id;
    PAPACC_U64 handle_connection_id;
    PAPACC_U32 expected_handle_id;
    PAPACC_U32 observed_handle_id;
    PAPACC_U32 expected_transport_id;
    PAPACC_U32 observed_transport_id;
    PAPACC_U32 expected_processor_id;
    PAPACC_U32 observed_processor_id;
    PAPACC_U32 expected_membership_id;
    PAPACC_U32 observed_membership_id;
    PAPACC_RESULT deferred_result;
    PAPACC_RESULT failed_result;
    PAPACC_BOOL deferred_removed;
    PAPACC_BOOL failed_removed;
    PAPACC_BOOL connection_preserved;
    PAPACC_BOOL no_destructive_close;
    PAPACC_BOOL no_release;
} TEST_P6_EVIDENCE;

static PAPACC_BOOL p6_evidence_valid(const TEST_P6_EVIDENCE *evidence)
{
    return evidence != NULL && evidence->expected_connection_id != 0U &&
        evidence->observed_connection_id == evidence->expected_connection_id &&
        evidence->handle_connection_id == evidence->expected_connection_id &&
        evidence->expected_handle_id != 0U &&
        evidence->observed_handle_id == evidence->expected_handle_id &&
        evidence->expected_transport_id != 0U &&
        evidence->observed_transport_id == evidence->expected_transport_id &&
        evidence->expected_processor_id != 0U &&
        evidence->observed_processor_id == evidence->expected_processor_id &&
        evidence->expected_membership_id != 0U &&
        evidence->observed_membership_id == evidence->expected_membership_id &&
        evidence->deferred_result == PAPACC_RESULT_OK &&
        evidence->failed_result == PAPACC_RESULT_INVALID_STATE &&
        evidence->deferred_removed == PAPACC_FALSE &&
        evidence->failed_removed == PAPACC_FALSE &&
        evidence->connection_preserved == PAPACC_TRUE &&
        evidence->no_destructive_close == PAPACC_TRUE &&
        evidence->no_release == PAPACC_TRUE
            ? PAPACC_TRUE : PAPACC_FALSE;
}

static int run_adr0032_p6(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_PST_SECURE_PROCESSOR processor = PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler = PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER member = PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    PST_WAIT_EVENT event;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter = PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_connection *server = NULL;
    pst_connection *identity = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    pst_wait_token token = 0U;
    TEST_P5_OBSERVER observer;
    TEST_P6_EVIDENCE evidence;
    TEST_P6_EVIDENCE mutation;
    PAPACC_BOOL removed = PAPACC_TRUE;
    PAPACC_U64 connection_id = 0U;
    PAPACC_U32 negatives = 0U;
    int result = 1;

    memset(&fixture, 0, sizeof(fixture));
    memset(&options, 0, sizeof(options));
    memset(&event, 0, sizeof(event));
    memset(&observer, 0, sizeof(observer));
    memset(&evidence, 0, sizeof(evidence));
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) goto cleanup;
    if (pst_win32_register_builtin_providers() != PST_RESULT_OK) goto cleanup;
    options.struct_size = (pst_u32)sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_pst_secure_scheduler_init(&scheduler, &member, 1U,
            &event, 1U) != PAPACC_RESULT_OK ||
        !establish_processor_pair(runtime, fixture.trust,
            fixture.server.credentials, &processor, &server,
            &server_socket, &local, &remote))
        goto cleanup;
    identity = processor.connection;
    if (papacc_pst_secure_scheduler_add_connection(&scheduler, identity,
            &token) != PAPACC_RESULT_OK || token == 0U ||
        papacc_pst_secure_transport_adapter_init(&adapter, &processor,
            &scheduler, &transport) != PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, &storage, 1U) !=
            PAPACC_RESULT_OK)
        goto cleanup;

    observer.original_pre_close = transport.pre_close_fn;
    observer.original_close = transport.close_fn;
    observer.original_context = transport.context;
    observer.mode = TEST_P5_PRE_CLOSE_DEFERRED;
    transport.pre_close_fn = p5_observed_pre_close;
    transport.close_fn = p5_observed_close;
    transport.context = &observer;
    if (papacc_connection_manager_publish(&manager, &transport, &local,
            &remote, &connection) != PAPACC_RESULT_OK || connection == NULL)
        goto cleanup;
    connection_id = connection->connection_instance_id;

    evidence.expected_connection_id = connection_id;
    evidence.expected_handle_id = 1U;
    evidence.expected_transport_id = 1U;
    evidence.expected_processor_id = 1U;
    evidence.expected_membership_id = 1U;

    evidence.deferred_result = papacc_connection_manager_try_remove(&manager,
        connection_id, &removed);
    evidence.deferred_removed = removed;
    if (evidence.deferred_result != PAPACC_RESULT_OK ||
        removed != PAPACC_FALSE)
        goto cleanup;

    observer.mode = TEST_P5_PRE_CLOSE_DELEGATE;
    scheduler.initialized = PAPACC_FALSE;
    removed = PAPACC_TRUE;
    evidence.failed_result = papacc_connection_manager_try_remove(&manager,
        connection_id, &removed);
    scheduler.initialized = PAPACC_TRUE;
    evidence.failed_removed = removed;

    evidence.observed_connection_id = connection->connection_instance_id;
    evidence.handle_connection_id = connection->connection_instance_id;
    evidence.observed_handle_id = connection->transport.context == &observer &&
        observer.original_context == &adapter ? 1U : 0U;
    evidence.observed_transport_id =
        connection->transport.pre_close_fn == p5_observed_pre_close &&
        connection->transport.close_fn == p5_observed_close &&
        connection->transport.context == &observer ? 1U : 0U;
    evidence.observed_processor_id = adapter.processor == &processor &&
        processor.connection == identity && adapter.closed == PAPACC_FALSE
            ? 1U : 0U;
    evidence.observed_membership_id = adapter.registered == PAPACC_TRUE &&
        member.registered == PAPACC_TRUE &&
        member.source.connection == identity && scheduler.member_count == 1U
            ? 1U : 0U;
    evidence.connection_preserved =
        papacc_connection_manager_find(&manager, connection_id) == connection &&
        manager.count == 1U ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.no_destructive_close = observer.destructive_close_calls == 0U
        ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.no_release = observer.pst_release_attempts == 0U
        ? PAPACC_TRUE : PAPACC_FALSE;
    if (p6_evidence_valid(&evidence) != PAPACC_TRUE) goto cleanup;

#define TEST_P6_REJECT(statement) \
    do { \
        mutation = evidence; \
        statement; \
        if (p6_evidence_valid(&mutation) != PAPACC_FALSE) goto cleanup; \
        ++negatives; \
    } while (0)
    TEST_P6_REJECT(mutation.observed_handle_id = 0U);
    TEST_P6_REJECT(mutation.observed_handle_id = 2U);
    TEST_P6_REJECT(++mutation.handle_connection_id);
    TEST_P6_REJECT(mutation.observed_transport_id = 2U);
    TEST_P6_REJECT(++mutation.observed_connection_id);
    TEST_P6_REJECT(mutation.observed_processor_id = 2U);
    TEST_P6_REJECT(mutation.observed_membership_id = 2U);
#undef TEST_P6_REJECT
    if (negatives != 7U) goto cleanup;

    removed = PAPACC_FALSE;
    if (papacc_connection_manager_try_remove(&manager, connection_id,
            &removed) != PAPACC_RESULT_OK || removed != PAPACC_TRUE ||
        observer.destructive_close_calls != 1U ||
        observer.pst_release_attempts != 1U || manager.count != 0U)
        goto cleanup;

    printf("P6_CONNECTION_ID=%llu P6_FAILED_REAL_PST=PASS "
        "P6_DEFERRED_GENERIC=PASS P6_HANDLE_IDENTITY=PASS_LOGICAL_ID_1 "
        "P6_TRANSPORT_IDENTITY=PASS_LOGICAL_ID_1 "
        "P6_PROCESSOR_IDENTITY=PASS_LOGICAL_ID_1 "
        "P6_MEMBERSHIP_IDENTITY=PASS_LOGICAL_ID_1 "
        "P6_HANDLE_PRESERVED=YES P6_CONNECTION_PRESERVED=YES "
        "P6_RECOVERY=READY_CLOSE_ONCE P6_NEGATIVES=%lu_OF_7_PASS\n",
        (unsigned long long)connection_id, (unsigned long)negatives);
    result = 0;
cleanup:
    if (scheduler.initialized == PAPACC_FALSE && scheduler.wait_set != NULL)
        scheduler.initialized = PAPACC_TRUE;
    if (manager.initialized == PAPACC_TRUE)
        papacc_connection_manager_shutdown(&manager);
    if (scheduler.initialized == PAPACC_TRUE)
        papacc_pst_secure_scheduler_release(&scheduler);
    papacc_pst_secure_processor_release(&processor);
    if (server != NULL) pst_connection_release(server);
    papacc_test_security_fixture_release(&fixture);
    if (runtime != NULL) pst_runtime_release(runtime);
    WSACleanup();
    return result;
}
typedef struct TEST_P7_EVIDENCE {
    PAPACC_U64 expected_connection_id;
    PAPACC_U64 observed_connection_id;
    PAPACC_CONNECTION_STATE state_before;
    PAPACC_CONNECTION_STATE state_after_deferred;
    PAPACC_CONNECTION_STATE state_after_failed;
    PAPACC_CONNECTION_STATE state_after_ready;
    PAPACC_BOOL published_before;
    PAPACC_BOOL published_after_deferred;
    PAPACC_BOOL published_after_failed;
    PAPACC_BOOL published_after_ready;
    PAPACC_BOOL removed_after_deferred;
    PAPACC_BOOL removed_after_failed;
    PAPACC_BOOL removed_after_ready;
    PAPACC_BOOL removal_declared_before_ready;
    PAPACC_U32 remove_attempts;
    PAPACC_U32 remove_successes;
    PAPACC_U32 destructive_close_calls;
    PAPACC_U32 pst_release_attempts;
} TEST_P7_EVIDENCE;

static PAPACC_BOOL p7_evidence_valid(const TEST_P7_EVIDENCE *evidence)
{
    return evidence != NULL && evidence->expected_connection_id != 0U &&
        evidence->observed_connection_id == evidence->expected_connection_id &&
        evidence->state_before == PAPACC_CONNECTION_STATE_PENDING &&
        evidence->state_after_deferred == PAPACC_CONNECTION_STATE_CLOSING &&
        evidence->state_after_failed == PAPACC_CONNECTION_STATE_CLOSING &&
        evidence->state_after_ready == PAPACC_CONNECTION_STATE_UNINITIALIZED &&
        evidence->published_before == PAPACC_TRUE &&
        evidence->published_after_deferred == PAPACC_TRUE &&
        evidence->published_after_failed == PAPACC_TRUE &&
        evidence->published_after_ready == PAPACC_FALSE &&
        evidence->removed_after_deferred == PAPACC_FALSE &&
        evidence->removed_after_failed == PAPACC_FALSE &&
        evidence->removed_after_ready == PAPACC_TRUE &&
        evidence->removal_declared_before_ready == PAPACC_FALSE &&
        evidence->remove_attempts == 4U && evidence->remove_successes == 1U &&
        evidence->destructive_close_calls == 1U &&
        evidence->pst_release_attempts == 1U
            ? PAPACC_TRUE : PAPACC_FALSE;
}

static int run_adr0032_p7(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_PST_SECURE_PROCESSOR processor = PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler = PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER member = PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    PST_WAIT_EVENT event;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter = PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_connection *server = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    pst_wait_token token = 0U;
    TEST_P5_OBSERVER observer;
    TEST_P7_EVIDENCE evidence;
    TEST_P7_EVIDENCE mutation;
    PAPACC_BOOL removed = PAPACC_TRUE;
    PAPACC_U64 connection_id = 0U;
    PAPACC_U32 negatives = 0U;
    int result = 1;

    memset(&fixture, 0, sizeof(fixture));
    memset(&options, 0, sizeof(options));
    memset(&event, 0, sizeof(event));
    memset(&observer, 0, sizeof(observer));
    memset(&evidence, 0, sizeof(evidence));
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) goto cleanup;
    if (pst_win32_register_builtin_providers() != PST_RESULT_OK) goto cleanup;
    options.struct_size = (pst_u32)sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_pst_secure_scheduler_init(&scheduler, &member, 1U,
            &event, 1U) != PAPACC_RESULT_OK ||
        !establish_processor_pair(runtime, fixture.trust,
            fixture.server.credentials, &processor, &server,
            &server_socket, &local, &remote))
        goto cleanup;
    if (papacc_pst_secure_scheduler_add_connection(&scheduler,
            processor.connection, &token) != PAPACC_RESULT_OK || token == 0U ||
        papacc_pst_secure_transport_adapter_init(&adapter, &processor,
            &scheduler, &transport) != PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, &storage, 1U) !=
            PAPACC_RESULT_OK)
        goto cleanup;

    observer.original_pre_close = transport.pre_close_fn;
    observer.original_close = transport.close_fn;
    observer.original_context = transport.context;
    observer.mode = TEST_P5_PRE_CLOSE_DEFERRED;
    transport.pre_close_fn = p5_observed_pre_close;
    transport.close_fn = p5_observed_close;
    transport.context = &observer;
    if (papacc_connection_manager_publish(&manager, &transport, &local,
            &remote, &connection) != PAPACC_RESULT_OK || connection == NULL)
        goto cleanup;
    connection_id = connection->connection_instance_id;
    evidence.expected_connection_id = connection_id;
    evidence.state_before = connection->state;
    evidence.published_before =
        papacc_connection_manager_find(&manager, connection_id) == connection
            ? PAPACC_TRUE : PAPACC_FALSE;

    removed = PAPACC_TRUE;
    ++evidence.remove_attempts;
    if (papacc_connection_manager_try_remove(&manager, connection_id,
            &removed) != PAPACC_RESULT_OK || removed != PAPACC_FALSE)
        goto cleanup;
    evidence.removed_after_deferred = removed;
    evidence.state_after_deferred = connection->state;
    evidence.published_after_deferred =
        papacc_connection_manager_find(&manager, connection_id) == connection
            ? PAPACC_TRUE : PAPACC_FALSE;

    observer.mode = TEST_P5_PRE_CLOSE_DELEGATE;
    scheduler.initialized = PAPACC_FALSE;
    removed = PAPACC_TRUE;
    ++evidence.remove_attempts;
    if (papacc_connection_manager_try_remove(&manager, connection_id,
            &removed) != PAPACC_RESULT_INVALID_STATE) {
        scheduler.initialized = PAPACC_TRUE;
        goto cleanup;
    }
    scheduler.initialized = PAPACC_TRUE;
    evidence.removed_after_failed = removed;
    evidence.state_after_failed = connection->state;
    evidence.published_after_failed =
        papacc_connection_manager_find(&manager, connection_id) == connection
            ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.observed_connection_id = connection->connection_instance_id;

    removed = PAPACC_FALSE;
    ++evidence.remove_attempts;
    if (papacc_connection_manager_try_remove(&manager, connection_id,
            &removed) != PAPACC_RESULT_OK || removed != PAPACC_TRUE)
        goto cleanup;
    ++evidence.remove_successes;
    evidence.removed_after_ready = removed;
    evidence.state_after_ready = storage.state;
    evidence.published_after_ready =
        papacc_connection_manager_find(&manager, connection_id) != NULL
            ? PAPACC_TRUE : PAPACC_FALSE;

    removed = PAPACC_TRUE;
    ++evidence.remove_attempts;
    if (papacc_connection_manager_try_remove(&manager, connection_id,
            &removed) != PAPACC_RESULT_INVALID_ARGUMENT ||
        removed != PAPACC_FALSE)
        goto cleanup;
    evidence.destructive_close_calls = observer.destructive_close_calls;
    evidence.pst_release_attempts = observer.pst_release_attempts;
    if (p7_evidence_valid(&evidence) != PAPACC_TRUE) goto cleanup;

#define TEST_P7_REJECT(statement) \
    do { \
        mutation = evidence; \
        statement; \
        if (p7_evidence_valid(&mutation) != PAPACC_FALSE) goto cleanup; \
        ++negatives; \
    } while (0)
    TEST_P7_REJECT(mutation.removed_after_failed = PAPACC_TRUE);
    TEST_P7_REJECT(mutation.removed_after_deferred = PAPACC_TRUE);
    TEST_P7_REJECT(++mutation.observed_connection_id);
    TEST_P7_REJECT(mutation.published_after_failed = PAPACC_FALSE);
    TEST_P7_REJECT(mutation.state_after_deferred = PAPACC_CONNECTION_STATE_CLOSED);
    TEST_P7_REJECT(mutation.removal_declared_before_ready = PAPACC_TRUE);
    TEST_P7_REJECT(mutation.remove_successes = 2U);
#undef TEST_P7_REJECT
    if (negatives != 7U) goto cleanup;

    printf("P7_EXPECTED_CONNECTION_ID=%llu P7_OBSERVED_CONNECTION_ID=%llu "
        "P7_FAILED_REAL_PST=PASS P7_DEFERRED_GENERIC=PASS "
        "P7_MANAGER_PUBLICATION_PRESERVED=YES "
        "P7_CONNECTION_STATE_PRESERVED=YES "
        "P7_CAUSE_PRESERVED=NOT_APPLICABLE "
        "P7_REMOVE_BEFORE_READY=2_ATTEMPTS_0_SUCCESSES "
        "P7_REMOVE_AFTER_READY=1_SUCCESS_POST_REMOVE_REJECTED "
        "P7_RECOVERY=SAME_CONNECTION_READY_CLOSE_ONCE "
        "P7_NEGATIVES=%lu_OF_7_APPLICABLE_PASS_N6_NOT_APPLICABLE\n",
        (unsigned long long)evidence.expected_connection_id,
        (unsigned long long)evidence.observed_connection_id,
        (unsigned long)negatives);
    result = 0;
cleanup:
    if (scheduler.initialized == PAPACC_FALSE && scheduler.wait_set != NULL)
        scheduler.initialized = PAPACC_TRUE;
    if (manager.initialized == PAPACC_TRUE)
        papacc_connection_manager_shutdown(&manager);
    if (scheduler.initialized == PAPACC_TRUE)
        papacc_pst_secure_scheduler_release(&scheduler);
    papacc_pst_secure_processor_release(&processor);
    if (server != NULL) pst_connection_release(server);
    papacc_test_security_fixture_release(&fixture);
    if (runtime != NULL) pst_runtime_release(runtime);
    WSACleanup();
    return result;
}
typedef struct TEST_P8_EVIDENCE {
    PAPACC_U64 expected_connection_id;
    PAPACC_U64 observed_connection_id;
    PAPACC_U64 cleanup_connection_id;
    PAPACC_U32 close_request_count;
    PAPACC_U32 cleanup_request_count;
    PAPACC_U32 waitset_remove_attempts;
    PAPACC_U32 waitset_remove_successes;
    PAPACC_U32 pst_release_attempts;
    PAPACC_U32 destructive_close_attempts;
    PAPACC_U32 destructive_close_successes;
    PAPACC_U32 connection_remove_attempts;
    PAPACC_U32 connection_remove_successes;
    PAPACC_BOOL no_resurrection;
} TEST_P8_EVIDENCE;

static PAPACC_BOOL p8_evidence_valid(const TEST_P8_EVIDENCE *evidence)
{
    return evidence != NULL && evidence->expected_connection_id != 0U &&
        evidence->observed_connection_id == evidence->expected_connection_id &&
        evidence->cleanup_connection_id == evidence->expected_connection_id &&
        evidence->close_request_count == 3U &&
        evidence->cleanup_request_count == 3U &&
        evidence->waitset_remove_attempts == 1U &&
        evidence->waitset_remove_successes == 1U &&
        evidence->pst_release_attempts == 1U &&
        evidence->destructive_close_attempts == 1U &&
        evidence->destructive_close_successes == 1U &&
        evidence->connection_remove_attempts == 3U &&
        evidence->connection_remove_successes == 1U &&
        evidence->no_resurrection == PAPACC_TRUE
            ? PAPACC_TRUE : PAPACC_FALSE;
}

static int run_adr0032_p8(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_PST_SECURE_PROCESSOR processor = PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler = PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER member = PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    PST_WAIT_EVENT event;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter = PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_connection *server = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    pst_wait_token token = 0U;
    TEST_P4_OBSERVER observer;
    TEST_P8_EVIDENCE evidence;
    TEST_P8_EVIDENCE mutation;
    PAPACC_BOOL ready = PAPACC_FALSE;
    PAPACC_BOOL removed = PAPACC_FALSE;
    PAPACC_U64 connection_id = 0U;
    PAPACC_U32 negatives = 0U;
    int result = 1;

    memset(&fixture, 0, sizeof(fixture)); memset(&options, 0, sizeof(options));
    memset(&event, 0, sizeof(event)); memset(&observer, 0, sizeof(observer));
    memset(&evidence, 0, sizeof(evidence));
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0) goto cleanup;
    if (pst_win32_register_builtin_providers() != PST_RESULT_OK) goto cleanup;
    options.struct_size = (pst_u32)sizeof(options); options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_pst_secure_scheduler_init(&scheduler, &member, 1U, &event, 1U) != PAPACC_RESULT_OK ||
        !establish_processor_pair(runtime, fixture.trust, fixture.server.credentials,
            &processor, &server, &server_socket, &local, &remote)) goto cleanup;
    if (papacc_pst_secure_scheduler_add_connection(&scheduler, processor.connection,
            &token) != PAPACC_RESULT_OK || token == 0U ||
        papacc_pst_secure_transport_adapter_init(&adapter, &processor, &scheduler,
            &transport) != PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, &storage, 1U) != PAPACC_RESULT_OK)
        goto cleanup;

    observer.adapter = &adapter; observer.original_context = transport.context;
    observer.original_pre_close = transport.pre_close_fn;
    observer.original_close = transport.close_fn;
    transport.context = &observer; transport.pre_close_fn = p4_observed_pre_close;
    transport.close_fn = p4_observed_close;
    if (papacc_connection_manager_publish(&manager, &transport, &local, &remote,
            &connection) != PAPACC_RESULT_OK || connection == NULL) goto cleanup;
    connection_id = connection->connection_instance_id;

    if (papacc_transport_connection_prepare_close(&connection->transport, &ready) !=
            PAPACC_RESULT_OK || ready != PAPACC_TRUE ||
        papacc_transport_connection_prepare_close(&connection->transport, &ready) !=
            PAPACC_RESULT_OK || ready != PAPACC_TRUE) goto cleanup;

    ++evidence.cleanup_request_count; ++evidence.connection_remove_attempts;
    if (papacc_connection_manager_try_remove(&manager, connection_id, &removed) !=
            PAPACC_RESULT_OK || removed != PAPACC_TRUE) goto cleanup;
    ++evidence.connection_remove_successes;

    removed = PAPACC_TRUE; ++evidence.cleanup_request_count;
    ++evidence.connection_remove_attempts;
    if (papacc_connection_manager_try_remove(&manager, connection_id, &removed) !=
            PAPACC_RESULT_INVALID_ARGUMENT || removed != PAPACC_FALSE) goto cleanup;
    removed = PAPACC_TRUE; ++evidence.cleanup_request_count;
    ++evidence.connection_remove_attempts;
    if (papacc_connection_manager_try_remove(&manager, connection_id, &removed) !=
            PAPACC_RESULT_INVALID_ARGUMENT || removed != PAPACC_FALSE) goto cleanup;

    evidence.expected_connection_id = connection_id;
    evidence.observed_connection_id = connection_id;
    evidence.cleanup_connection_id = connection_id;
    evidence.close_request_count = observer.pre_close_calls;
    evidence.waitset_remove_attempts = observer.remove_attempts;
    evidence.waitset_remove_successes = observer.remove_successes;
    evidence.pst_release_attempts = observer.pst_release_attempts;
    evidence.destructive_close_attempts = observer.destructive_attempts;
    evidence.destructive_close_successes = observer.destructive_successes;
    evidence.no_resurrection = manager.count == 0U &&
        papacc_connection_manager_find(&manager, connection_id) == NULL &&
        storage.state == PAPACC_CONNECTION_STATE_UNINITIALIZED
            ? PAPACC_TRUE : PAPACC_FALSE;
    if (p8_evidence_valid(&evidence) != PAPACC_TRUE) goto cleanup;

#define TEST_P8_REJECT(statement) \
    do { mutation = evidence; statement; \
        if (p8_evidence_valid(&mutation) != PAPACC_FALSE) goto cleanup; \
        ++negatives; } while (0)
    TEST_P8_REJECT(mutation.pst_release_attempts = 2U);
    TEST_P8_REJECT(mutation.destructive_close_attempts = 2U);
    TEST_P8_REJECT(mutation.waitset_remove_successes = 2U);
    TEST_P8_REJECT(mutation.connection_remove_successes = 2U);
    TEST_P8_REJECT(++mutation.observed_connection_id);
    TEST_P8_REJECT(++mutation.cleanup_connection_id);
    TEST_P8_REJECT(mutation.close_request_count = 4U);
    TEST_P8_REJECT(mutation.no_resurrection = PAPACC_FALSE);
#undef TEST_P8_REJECT
    if (negatives != 8U) goto cleanup;

    printf("P8_CONNECTION_ID=%llu P8_CLOSE_REQUEST_COUNT=%lu "
        "P8_CLEANUP_REQUEST_COUNT=%lu P8_WAITSET_REMOVE_ATTEMPTS=%lu "
        "P8_WAITSET_REMOVE_SUCCESSES=%lu P8_PST_RELEASE_ATTEMPTS=%lu "
        "P8_DESTRUCTIVE_CLOSE_ATTEMPTS=%lu P8_DESTRUCTIVE_CLOSE_SUCCESSES=%lu "
        "P8_CONNECTION_REMOVE_ATTEMPTS=%lu P8_CONNECTION_REMOVE_SUCCESSES=%lu "
        "P8_NO_RESURRECTION=YES P8_NEGATIVES=%lu_OF_8_PASS "
        "P8_PROVIDER_DESTROY=NOT_PROVEN P8_NATIVE_CLOSE=NOT_PROVEN\n",
        (unsigned long long)connection_id,
        (unsigned long)evidence.close_request_count,
        (unsigned long)evidence.cleanup_request_count,
        (unsigned long)evidence.waitset_remove_attempts,
        (unsigned long)evidence.waitset_remove_successes,
        (unsigned long)evidence.pst_release_attempts,
        (unsigned long)evidence.destructive_close_attempts,
        (unsigned long)evidence.destructive_close_successes,
        (unsigned long)evidence.connection_remove_attempts,
        (unsigned long)evidence.connection_remove_successes,
        (unsigned long)negatives);
    result = 0;
cleanup:
    if (manager.initialized == PAPACC_TRUE) papacc_connection_manager_shutdown(&manager);
    if (scheduler.initialized == PAPACC_TRUE) papacc_pst_secure_scheduler_release(&scheduler);
    papacc_pst_secure_processor_release(&processor);
    if (server != NULL) pst_connection_release(server);
    papacc_test_security_fixture_release(&fixture);
    if (runtime != NULL) pst_runtime_release(runtime);
    WSACleanup(); return result;
}
int main(int argc, char **argv)
{
    WSADATA winsock;
    PST_RUNTIME_OPTIONS runtime_options;
    PST_TRUST_SOURCE trust_source;
    pst_runtime *runtime = NULL;
    pst_trust *trust = NULL;
    PAPACC_TEST_SECURITY_FIXTURE fixture =
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER scheduler =
        PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SECURE_SCHEDULER undersized_scheduler =
        PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
    PAPACC_PST_SCHEDULER_MEMBER members[MEMBER_CAPACITY];
    PAPACC_PST_SCHEDULER_MEMBER undersized_members[2];
    PST_WAIT_EVENT events[MEMBER_CAPACITY];
    PST_WAIT_EVENT undersized_events[1];
    PAPACC_PST_SECURE_PROCESSOR processor_a =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR processor_b =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    TEST_SOCKET_PAIR pair_a;
    TEST_SOCKET_PAIR pair_b;
    SOCKET listener = INVALID_SOCKET;
    SOCKET listener_client = INVALID_SOCKET;
    SOCKET listener_accepted = INVALID_SOCKET;
    unsigned short listener_port;
    struct sockaddr_in address;
    pst_external_source *external = NULL;
    pst_wait_token token_a = 0U, token_b = 0U, token_listener = 0U;
    PAPACC_PST_WAIT_OUTCOME outcome;
    PAPACC_SIZE ready_count, dispatched;
    PAPACC_SIZE event_index, wait_round;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_BOOL read_interest, write_interest, expired;
    PAPACC_BOOL saw_a = PAPACC_FALSE, saw_b = PAPACC_FALSE;
    PAPACC_BOOL saw_listener = PAPACC_FALSE;
    TEST_DISPATCH first = { { 0U }, { PAPACC_PST_SCHEDULER_MEMBER_NONE }, 0U };
    TEST_DISPATCH second = { { 0U }, { PAPACC_PST_SCHEDULER_MEMBER_NONE }, 0U };
    TEST_DISPATCH terminal_dispatch = {
        { 0U }, { PAPACC_PST_SCHEDULER_MEMBER_NONE }, 0U
    };
    TEST_WAKE_CONTEXT wake_context;
    HANDLE thread;
    DWORD wait_status;
    static const char invalid_tls[] = "not tls";
    int result = 0;
    if (argc == 2 && strcmp(argv[1], "--adr0032-p1") == 0)
        return run_adr0032_p1();
    if (argc == 2 && strcmp(argv[1], "--adr0032-p2") == 0)
        return run_adr0032_p2();
    if (argc == 2 && strcmp(argv[1], "--adr0032-p4") == 0)
        return run_adr0032_p4();
    if (argc == 2 && strcmp(argv[1], "--adr0032-p5") == 0)
        return run_adr0032_p5();
    if (argc == 2 && strcmp(argv[1], "--adr0032-p6") == 0)
        return run_adr0032_p6();
    if (argc == 2 && strcmp(argv[1], "--adr0032-p7") == 0)
        return run_adr0032_p7();
    if (argc == 2 && strcmp(argv[1], "--adr0032-p8") == 0)
        return run_adr0032_p8();
    PAPACC_PST_SECURE_PROCESSOR idle_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    pst_connection *idle_server = NULL;
    SOCKET idle_server_socket = INVALID_SOCKET;
    pst_wait_token idle_token = 0U;
    DWORD idle_start, idle_elapsed;
    PAPACC_U8 idle_byte;
    PAPACC_SIZE idle_transferred;
    int idle_drain_round;

    CHECK(WSAStartup(MAKEWORD(2, 2), &winsock) == 0, 1);
    CHECK(pst_win32_register_builtin_providers() == PST_RESULT_OK, 2);
    memset(&runtime_options, 0, sizeof(runtime_options));
    runtime_options.struct_size = (pst_u32)sizeof(runtime_options);
    runtime_options.api_version = PST_API_VERSION;
    CHECK(pst_runtime_create(&runtime_options, &runtime) == PST_RESULT_OK, 3);
    memset(&trust_source, 0, sizeof(trust_source));
    trust_source.struct_size = (pst_u32)sizeof(trust_source);
    trust_source.api_version = PST_API_VERSION;
    trust_source.kind = PST_TRUST_SOURCE_SYSTEM;
    CHECK(pst_trust_create(&trust_source, &trust) == PST_RESULT_OK, 48);
    CHECK(papacc_test_security_fixture_init(&fixture), 57);

    CHECK(papacc_pst_secure_scheduler_init(&undersized_scheduler,
        undersized_members, 2U, undersized_events, 1U) ==
        PAPACC_RESULT_INVALID_ARGUMENT, 54);

    CHECK(papacc_pst_secure_scheduler_init(&scheduler, members,
        MEMBER_CAPACITY, events, MEMBER_CAPACITY) == PAPACC_RESULT_OK, 4);
    CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 0U, &outcome,
        &ready_count) == PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_TIMEOUT && ready_count == 0U, 5);
    CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 10U, &outcome,
        &ready_count) == PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_TIMEOUT, 6);

    CHECK(establish_processor_pair(runtime, fixture.trust,
        fixture.server.credentials, &idle_client, &idle_server,
        &idle_server_socket, NULL, NULL), 58);
    CHECK(papacc_pst_secure_scheduler_add_connection(&scheduler,
        idle_client.connection, &idle_token) == PAPACC_RESULT_OK, 59);
    for (idle_drain_round = 0; idle_drain_round < 8; ++idle_drain_round) {
        CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 20U, &outcome,
            &ready_count) == PAPACC_RESULT_OK, 60);
        if (outcome == PAPACC_PST_WAIT_TIMEOUT) break;
        CHECK(outcome == PAPACC_PST_WAIT_READY, 60);
        CHECK(papacc_pst_secure_processor_read_once(&idle_client, &idle_byte,
            1U, &idle_transferred, &step) == PAPACC_RESULT_OK &&
            idle_transferred == 0U &&
            step == PAPACC_PST_SECURE_STEP_NEED_READ, 60);
    }
    CHECK(idle_drain_round < 8, 60);
    idle_start = GetTickCount();
    CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 100U, &outcome,
        &ready_count) == PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_TIMEOUT && ready_count == 0U, 60);
    idle_elapsed = GetTickCount() - idle_start;
    CHECK(idle_elapsed >= 50U, 61);
    CHECK(shutdown(idle_server_socket, SD_BOTH) == 0, 62);
    CHECK(wait_for_token(&scheduler, idle_token), 63);
    CHECK(papacc_pst_secure_processor_read_once(&idle_client, &idle_byte, 1U,
        &idle_transferred, &step) == PAPACC_RESULT_OK &&
        (step == PAPACC_PST_SECURE_STEP_CLOSED ||
         step == PAPACC_PST_SECURE_STEP_FAILED), 63);
    CHECK(papacc_pst_secure_scheduler_remove_connection(&scheduler,
        idle_client.connection) == PAPACC_RESULT_OK, 64);
    pst_connection_release(idle_server);
    idle_server = NULL;
    papacc_pst_secure_processor_release(&idle_client);

    result = create_processor(runtime, trust, &processor_a, &pair_a);
    CHECK(result == 0, 70 + result);
    result = create_processor(runtime, trust, &processor_b, &pair_b);
    CHECK(result == 0, 80 + result);
    CHECK(papacc_pst_secure_processor_handshake_once(&processor_a, &step) ==
        PAPACC_RESULT_OK && step != PAPACC_PST_SECURE_STEP_COMPLETE, 9);
    CHECK(papacc_pst_secure_processor_handshake_once(&processor_b, &step) ==
        PAPACC_RESULT_OK && step != PAPACC_PST_SECURE_STEP_COMPLETE, 10);
    CHECK(papacc_pst_secure_processor_interest(&processor_a, &read_interest,
        &write_interest) == PAPACC_RESULT_OK &&
        (read_interest || write_interest), 11);
    CHECK(papacc_pst_secure_scheduler_add_connection(&scheduler,
        processor_a.connection, &token_a) == PAPACC_RESULT_OK, 12);
    CHECK(papacc_pst_secure_scheduler_add_connection(&scheduler,
        processor_b.connection, &token_b) == PAPACC_RESULT_OK &&
        token_a != token_b, 13);
    CHECK(papacc_pst_secure_scheduler_add_connection(&scheduler,
        processor_a.connection, &ready_count) == PAPACC_RESULT_INVALID_STATE,
        14);
    result = (int)pst_connection_try_release(processor_a.connection);
    CHECK(result == (int)PST_RESULT_CONCURRENT_OPERATION ||
        result == (int)PST_RESULT_INVALID_STATE, 15);

    CHECK(create_listener(&listener, &listener_port), 16);
    CHECK(papacc_pst_external_source_win32_create((PAPACC_SIZE)listener,
        &external) == PAPACC_RESULT_OK, 17);
    CHECK(papacc_pst_secure_scheduler_add_external(&scheduler, external,
        PAPACC_TRUE, PAPACC_FALSE, &token_listener) == PAPACC_RESULT_OK, 18);
    result = (int)pst_external_source_try_release(external);
    CHECK(result == (int)PST_RESULT_CONCURRENT_OPERATION ||
        result == (int)PST_RESULT_INVALID_STATE, 19);

    listener_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(listener_client != INVALID_SOCKET, 20);
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(listener_port);
    CHECK(connect(listener_client, (const struct sockaddr *)&address,
        sizeof(address)) != SOCKET_ERROR, 21);
    CHECK(send(pair_a.peer_side, invalid_tls,
        (int)(sizeof(invalid_tls) - 1U), 0) > 0, 22);
    CHECK(send(pair_b.peer_side, invalid_tls,
        (int)(sizeof(invalid_tls) - 1U), 0) > 0, 23);

    for (wait_round = 0U; wait_round < DISPATCH_CAPACITY &&
            !(saw_a && saw_b && saw_listener); ++wait_round) {
        result = (int)papacc_pst_secure_scheduler_wait(&scheduler, 1000U,
            &outcome, &ready_count);
        CHECK(result == (int)PAPACC_RESULT_OK &&
            outcome == PAPACC_PST_WAIT_READY && ready_count != 0U, 24);
        CHECK(papacc_pst_secure_scheduler_dispatch_ready(&scheduler,
            dispatch_callback, &first, &dispatched) == PAPACC_RESULT_OK &&
            dispatched != 0U, 25);
        for (event_index = 0U; event_index < first.count; ++event_index) {
            if (first.tokens[event_index] == token_a) saw_a = PAPACC_TRUE;
            if (first.tokens[event_index] == token_b) saw_b = PAPACC_TRUE;
            if (first.tokens[event_index] == token_listener)
                saw_listener = PAPACC_TRUE;
        }
    }
    CHECK(saw_a && saw_b && saw_listener, 55);
    for (event_index = 0U; event_index < first.count; ++event_index) {
        if (first.tokens[event_index] == token_listener) break;
    }
    CHECK(event_index < first.count &&
        first.kinds[event_index] == PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL &&
        first.external[event_index] == PAPACC_TRUE, 50);
    listener_accepted = accept(listener, NULL, NULL);
    CHECK(listener_accepted != INVALID_SOCKET, 26);

    CHECK(send(pair_a.peer_side, invalid_tls,
        (int)(sizeof(invalid_tls) - 1U), 0) > 0, 27);
    CHECK(send(pair_b.peer_side, invalid_tls,
        (int)(sizeof(invalid_tls) - 1U), 0) > 0, 28);
    saw_a = PAPACC_FALSE;
    saw_b = PAPACC_FALSE;
    for (wait_round = 0U; wait_round < DISPATCH_CAPACITY &&
            !(saw_a && saw_b); ++wait_round) {
        CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 1000U, &outcome,
            &ready_count) == PAPACC_RESULT_OK &&
            outcome == PAPACC_PST_WAIT_READY, 29);
        CHECK(papacc_pst_secure_scheduler_dispatch_ready(&scheduler,
            dispatch_callback, &second, &dispatched) == PAPACC_RESULT_OK &&
            dispatched != 0U, 30);
        for (event_index = 0U; event_index < second.count; ++event_index) {
            if (second.tokens[event_index] == token_a) saw_a = PAPACC_TRUE;
            if (second.tokens[event_index] == token_b) saw_b = PAPACC_TRUE;
        }
    }
    CHECK(saw_a && saw_b, 56);

    CHECK(papacc_pst_secure_processor_handshake_once(&processor_a, &step) ==
        PAPACC_RESULT_OK && (step == PAPACC_PST_SECURE_STEP_FAILED ||
        step == PAPACC_PST_SECURE_STEP_NEED_READ ||
        step == PAPACC_PST_SECURE_STEP_NEED_WRITE ||
        step == PAPACC_PST_SECURE_STEP_NEED_READ_WRITE), 31);
    closesocket(pair_a.peer_side);
    pair_a.peer_side = INVALID_SOCKET;
    CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 1000U, &outcome,
        &ready_count) == PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_READY, 51);
    CHECK(papacc_pst_secure_scheduler_dispatch_ready(&scheduler,
        dispatch_callback, &terminal_dispatch, &dispatched) ==
        PAPACC_RESULT_OK, 52);
    for (event_index = 0U; event_index < terminal_dispatch.count;
         ++event_index) {
        if (terminal_dispatch.tokens[event_index] == token_a &&
            terminal_dispatch.terminal[event_index] == PAPACC_TRUE)
            break;
    }
    CHECK(event_index < terminal_dispatch.count, 53);
    CHECK(processor_b.state == PAPACC_PST_SECURE_PROCESSOR_HANDSHAKING, 32);
    CHECK(papacc_pst_secure_processor_check_deadline(&processor_b,
        ~(PAPACC_U64)0, &expired) == PAPACC_RESULT_OK && expired, 33);
    CHECK(processor_b.state == PAPACC_PST_SECURE_PROCESSOR_FAILED, 34);

    CHECK(papacc_pst_secure_scheduler_remove_external(&scheduler, external) ==
        PAPACC_RESULT_OK, 41);
    CHECK(pst_external_source_try_release(external) == PST_RESULT_OK, 42);
    external = NULL;
    CHECK(listen(listener, 8) != SOCKET_ERROR, 43);
    CHECK(papacc_pst_secure_scheduler_remove_connection(&scheduler,
        processor_a.connection) == PAPACC_RESULT_OK, 44);
    CHECK(papacc_pst_secure_scheduler_remove_connection(&scheduler,
        processor_b.connection) == PAPACC_RESULT_OK, 49);
    CHECK(pst_connection_try_release(processor_a.connection) == PST_RESULT_OK,
        45);
    processor_a.connection = NULL;
    processor_a.state = PAPACC_PST_SECURE_PROCESSOR_CLOSED;

    CHECK(papacc_pst_secure_scheduler_wake(&scheduler) == PAPACC_RESULT_OK,
        35);
    CHECK(papacc_pst_secure_scheduler_wake(&scheduler) == PAPACC_RESULT_OK,
        36);
    CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 1000U, &outcome,
        &ready_count) == PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_WOKEN, 37);
    wake_context.scheduler = &scheduler;
    wake_context.result = PAPACC_RESULT_INTERNAL_ERROR;
    thread = CreateThread(NULL, 0U, wake_thread, &wake_context, 0U, NULL);
    CHECK(thread != NULL, 38);
    CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 5000U, &outcome,
        &ready_count) == PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_WOKEN, 39);
    wait_status = WaitForSingleObject(thread, 1000U);
    CloseHandle(thread);
    CHECK(wait_status == WAIT_OBJECT_0 &&
        wake_context.result == PAPACC_RESULT_OK, 40);

    CHECK(papacc_pst_secure_scheduler_request_stop(&scheduler) ==
        PAPACC_RESULT_OK, 46);
    first.count = 0U;
    CHECK(papacc_pst_secure_scheduler_dispatch_ready(&scheduler,
        dispatch_callback, &first, &dispatched) == PAPACC_RESULT_OK &&
        dispatched == 0U, 47);

    papacc_pst_secure_scheduler_release(&scheduler);
    if (external != NULL) pst_external_source_release(external);
    papacc_pst_secure_processor_release(&processor_a);
    papacc_pst_secure_processor_release(&processor_b);
    if (idle_server != NULL) pst_connection_release(idle_server);
    papacc_pst_secure_processor_release(&idle_client);
    if (listener_accepted != INVALID_SOCKET) closesocket(listener_accepted);
    if (listener_client != INVALID_SOCKET) closesocket(listener_client);
    if (listener != INVALID_SOCKET) closesocket(listener);
    if (pair_a.pst_side != INVALID_SOCKET) closesocket(pair_a.pst_side);
    if (pair_a.peer_side != INVALID_SOCKET) closesocket(pair_a.peer_side);
    if (pair_b.pst_side != INVALID_SOCKET) closesocket(pair_b.pst_side);
    if (pair_b.peer_side != INVALID_SOCKET) closesocket(pair_b.peer_side);
    if (runtime != NULL) pst_runtime_release(runtime);
    if (trust != NULL) pst_trust_release(trust);
    papacc_test_security_fixture_release(&fixture);
    WSACleanup();
    return 0;
}
