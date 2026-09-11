#include <winsock2.h>
#include <windows.h>

#include "pst_external_source_win32.h"
#include "pst_secure_processor.h"
#include "pst_secure_scheduler.h"
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

typedef struct TEST_SOCKET_PAIR {
    SOCKET pst_side;
    SOCKET peer_side;
} TEST_SOCKET_PAIR;

typedef struct TEST_DISPATCH {
    pst_wait_token tokens[MEMBER_CAPACITY];
    PAPACC_PST_SCHEDULER_MEMBER_KIND kinds[MEMBER_CAPACITY];
    PAPACC_BOOL external[MEMBER_CAPACITY];
    PAPACC_BOOL terminal[MEMBER_CAPACITY];
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

static void init_client_config(PST_CONNECTION_CONFIG *config,
    pst_trust *trust)
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
    if (dispatch->count >= MEMBER_CAPACITY) return;
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

int main(void)
{
    WSADATA winsock;
    PST_RUNTIME_OPTIONS runtime_options;
    PST_TRUST_SOURCE trust_source;
    pst_runtime *runtime = NULL;
    pst_trust *trust = NULL;
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
    PAPACC_SIZE event_index;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_BOOL read_interest, write_interest, expired;
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

    result = (int)papacc_pst_secure_scheduler_wait(&scheduler, 1000U,
        &outcome, &ready_count);
    if (result != (int)PAPACC_RESULT_OK ||
        outcome != PAPACC_PST_WAIT_READY || ready_count < 3U)
        fprintf(stderr, "mixed wait result=%d outcome=%d ready=%llu\n",
            result, (int)outcome, (unsigned long long)ready_count);
    CHECK(result == (int)PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_READY && ready_count >= 3U, 24);
    CHECK(papacc_pst_secure_scheduler_dispatch_ready(&scheduler,
        dispatch_callback, &first, &dispatched) == PAPACC_RESULT_OK &&
        dispatched >= 3U && first.count == dispatched, 25);
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
    CHECK(papacc_pst_secure_scheduler_wait(&scheduler, 1000U, &outcome,
        &ready_count) == PAPACC_RESULT_OK &&
        outcome == PAPACC_PST_WAIT_READY, 29);
    CHECK(papacc_pst_secure_scheduler_dispatch_ready(&scheduler,
        dispatch_callback, &second, &dispatched) == PAPACC_RESULT_OK &&
        dispatched >= 2U && first.tokens[0] != second.tokens[0], 30);

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
    if (listener_accepted != INVALID_SOCKET) closesocket(listener_accepted);
    if (listener_client != INVALID_SOCKET) closesocket(listener_client);
    if (listener != INVALID_SOCKET) closesocket(listener);
    if (pair_a.pst_side != INVALID_SOCKET) closesocket(pair_a.pst_side);
    if (pair_a.peer_side != INVALID_SOCKET) closesocket(pair_a.peer_side);
    if (pair_b.pst_side != INVALID_SOCKET) closesocket(pair_b.pst_side);
    if (pair_b.peer_side != INVALID_SOCKET) closesocket(pair_b.peer_side);
    if (runtime != NULL) pst_runtime_release(runtime);
    if (trust != NULL) pst_trust_release(trust);
    WSACleanup();
    return 0;
}
