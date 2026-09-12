#include "server_secure_io_win32.h"
#include "security_test_support.h"
#include "pst_provider_bootstrap_win32.h"
#include "papinho_secure_transport_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#define CAPACITY 8U
#define CHECK(x, n) do { if (!(x)) { \
    fprintf(stderr, "check=%d line=%d\n", (n), __LINE__); \
    result = (n); goto cleanup; } } while (0)

static const PAPACC_U8 control_open[20] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,1,0,0,0,0,0,4,0,1,0,0 };
static const PAPACC_U8 control_accept[20] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,2,0,0,0,0,0,4,0,1,0,0 };
static const PAPACC_U8 data_ticket_request[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,3,0,0,0,0,0,0 };
static const PAPACC_U8 data_accept[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,6,0,0,0,0,0,0 };
static const PAPACC_U8 principal_bytes[16] = {
    0x50,0x41,0x50,0x41,0x43,0x43,0x2d,0x33,
    0x45,0x2d,0x50,0x2d,0,0,0,1 };

typedef struct TEST_POLICY {
    PAPACC_SIZE resolve_calls;
    PAPACC_SIZE authorize_calls;
    PAPACC_BOOL principal_a_known;
    PAPACC_U8 principal_a_sha256[PAPACC_CREDENTIAL_SHA256_SIZE];
    PAPACC_BOOL principal_b_known;
    PAPACC_U8 principal_b_sha256[PAPACC_CREDENTIAL_SHA256_SIZE];
    PAPACC_PRINCIPAL_RESOLUTION forced_resolution;
    PAPACC_AUTHORIZATION_DECISION forced_decision;
} TEST_POLICY;

static void discard_log(void *context, const PAPACC_LOG_RECORD *record)
{
    (void)context;
    (void)record;
}

static PAPACC_RESULT resolve_principal(void *context,
    const PAPACC_PEER_EVIDENCE *evidence,
    PAPACC_PRINCIPAL_RESOLUTION *resolution, PAPACC_PRINCIPAL *principal)
{
    TEST_POLICY *policy = (TEST_POLICY *)context;
    ++policy->resolve_calls;
    if (evidence == NULL || evidence->certificate_present != PAPACC_TRUE ||
        evidence->chain_validated != PAPACC_TRUE ||
        evidence->peer_authenticated != PAPACC_TRUE ||
        evidence->certificate_sha256_valid != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (policy->forced_resolution != PAPACC_PRINCIPAL_RESOLVED) {
        *resolution = policy->forced_resolution;
        return PAPACC_RESULT_OK;
    }
    *resolution = PAPACC_PRINCIPAL_RESOLVED;
    principal->valid = PAPACC_TRUE;
    memcpy(principal->value, principal_bytes, sizeof(principal_bytes));
    if (policy->principal_a_known != PAPACC_TRUE) {
        memcpy(policy->principal_a_sha256, evidence->certificate_sha256,
            sizeof(policy->principal_a_sha256));
        policy->principal_a_known = PAPACC_TRUE;
    } else if (memcmp(policy->principal_a_sha256,
            evidence->certificate_sha256,
            sizeof(policy->principal_a_sha256)) != 0) {
        if (policy->principal_b_known != PAPACC_TRUE) {
            memcpy(policy->principal_b_sha256, evidence->certificate_sha256,
                sizeof(policy->principal_b_sha256));
            policy->principal_b_known = PAPACC_TRUE;
        } else if (memcmp(policy->principal_b_sha256,
                evidence->certificate_sha256,
                sizeof(policy->principal_b_sha256)) != 0) {
            *resolution = PAPACC_PRINCIPAL_NOT_ENROLLED;
            principal->valid = PAPACC_FALSE;
            return PAPACC_RESULT_OK;
        }
        principal->value[sizeof(principal->value) - 1U] = 2U;
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT authorize(void *context,
    const PAPACC_PRINCIPAL *principal, PAPACC_AUTHORIZATION_ACTION action,
    PAPACC_AUTHORIZATION_DECISION *decision)
{
    TEST_POLICY *policy = (TEST_POLICY *)context;
    (void)action;
    ++policy->authorize_calls;
    if (principal == NULL || principal->valid != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *decision = policy->forced_decision;
    if (action == PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION &&
        principal->value[sizeof(principal->value) - 1U] == 2U)
        *decision = PAPACC_AUTHORIZATION_DENY;
    return PAPACC_RESULT_OK;
}

static SOCKET connect_loopback(PAPACC_U16 port)
{
    struct sockaddr_in address;
    u_long nonblocking = 1UL;
    SOCKET socket_value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_value == INVALID_SOCKET) return INVALID_SOCKET;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(socket_value, (const struct sockaddr *)&address,
            sizeof(address)) == SOCKET_ERROR ||
        ioctlsocket(socket_value, FIONBIO, &nonblocking) == SOCKET_ERROR) {
        closesocket(socket_value);
        return INVALID_SOCKET;
    }
    return socket_value;
}

static int establish_client(PAPACC_U16 port,
    PAPACC_SECURITY_COMPOSITION *composition,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, pst_credentials *credentials,
    pst_trust *trust, PAPACC_PST_SECURE_PROCESSOR *client)
{
    SOCKET socket_value = INVALID_SOCKET;
    pst_transport *transport = NULL;
    PST_CONNECTION_CONFIG configuration;
    PAPACC_BOOL ownership = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_SIZE index;
    socket_value = connect_loopback(port);
    if (socket_value == INVALID_SOCKET) return 0;
    papacc_test_pst_client_config(&configuration, credentials, trust);
    if (papacc_pst_secure_processor_init(client, composition->runtime,
            &configuration, ~(PAPACC_U64)0) != PAPACC_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)socket_value, &transport) !=
            PST_RESULT_OK ||
        papacc_pst_secure_processor_attach(client, transport, &ownership) !=
            PAPACC_RESULT_OK || ownership != PAPACC_TRUE) {
        if (transport != NULL) pst_transport_release(transport);
        else closesocket(socket_value);
        return 0;
    }
    transport = NULL;
    socket_value = INVALID_SOCKET;
    for (index = 0U; index < 500U && client->state !=
            PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED; ++index) {
        if (papacc_pst_secure_processor_handshake_once(client, &step) !=
                PAPACC_RESULT_OK ||
            papacc_server_secure_io_win32_poll_once(secure_io, 10U) !=
                PAPACC_RESULT_OK) return 0;
    }
    return client->state == PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED;
}

static int write_all(PAPACC_PST_SECURE_PROCESSOR *client,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, const PAPACC_U8 *bytes,
    PAPACC_SIZE size, PAPACC_SIZE fragment_size)
{
    PAPACC_PST_SECURE_STEP step;
    PAPACC_SIZE offset = 0U;
    PAPACC_SIZE transferred;
    PAPACC_SIZE requested;
    PAPACC_SIZE index;
    for (index = 0U; index < 1000U && offset < size; ++index) {
        requested = size - offset;
        if (fragment_size != 0U && requested > fragment_size)
            requested = fragment_size;
        if (papacc_pst_secure_processor_write_once(client, bytes + offset,
                requested, &transferred, &step) != PAPACC_RESULT_OK ||
            papacc_server_secure_io_win32_poll_once(secure_io, 10U) !=
                PAPACC_RESULT_OK) return 0;
        offset += transferred;
    }
    return offset == size;
}

static int read_exact(PAPACC_PST_SECURE_PROCESSOR *client,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U8 *bytes,
    PAPACC_SIZE size)
{
    PAPACC_PST_SECURE_STEP step;
    PAPACC_SIZE offset = 0U;
    PAPACC_SIZE transferred;
    PAPACC_SIZE index;
    for (index = 0U; index < 1000U && offset < size; ++index) {
        if (papacc_server_secure_io_win32_poll_once(secure_io, 10U) !=
                PAPACC_RESULT_OK ||
            papacc_pst_secure_processor_read_once(client, bytes + offset,
                size - offset, &transferred, &step) != PAPACC_RESULT_OK)
            return 0;
        offset += transferred;
    }
    return offset == size;
}

static int raw_candidate_rejected(PAPACC_U16 port,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, const PAPACC_U8 *bytes,
    PAPACC_SIZE size)
{
    SOCKET socket_value = connect_loopback(port);
    PAPACC_SIZE index;
    int sent;
    if (socket_value == INVALID_SOCKET) return 0;
    sent = send(socket_value, (const char *)bytes, (int)size, 0);
    if (sent != (int)size) {
        closesocket(socket_value);
        return 0;
    }
    for (index = 0U; index < 100U; ++index) {
        PAPACC_RESULT poll_result =
            papacc_server_secure_io_win32_poll_once(secure_io, 1U);
        if (poll_result != PAPACC_RESULT_OK) {
            fprintf(stderr, "raw poll result=%d round=%zu\n",
                (int)poll_result, (size_t)index);
            closesocket(socket_value);
            return 0;
        }
    }
    closesocket(socket_value);
    return 1;
}

static int tls_candidate_rejected(PAPACC_U16 port,
    PAPACC_SECURITY_COMPOSITION *composition,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,
    const PST_CONNECTION_CONFIG *configuration)
{
    SOCKET socket_value = INVALID_SOCKET;
    pst_transport *transport = NULL;
    PAPACC_PST_SECURE_PROCESSOR client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_BOOL ownership = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_RESULT client_result;
    PAPACC_SIZE index;
    int ok = 1;
    socket_value = connect_loopback(port);
    if (socket_value == INVALID_SOCKET ||
        papacc_pst_secure_processor_init(&client, composition->runtime,
            configuration, ~(PAPACC_U64)0) != PAPACC_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)socket_value, &transport) !=
            PST_RESULT_OK ||
        papacc_pst_secure_processor_attach(&client, transport, &ownership) !=
            PAPACC_RESULT_OK || ownership != PAPACC_TRUE) {
        ok = 0;
        goto cleanup;
    }
    transport = NULL;
    socket_value = INVALID_SOCKET;
    for (index = 0U; index < 200U; ++index) {
        client_result = papacc_pst_secure_processor_handshake_once(
            &client, &step);
        if (papacc_server_secure_io_win32_poll_once(secure_io, 5U) !=
                PAPACC_RESULT_OK) {
            ok = 0;
            break;
        }
        if (client_result != PAPACC_RESULT_OK ||
            step == PAPACC_PST_SECURE_STEP_FAILED ||
            step == PAPACC_PST_SECURE_STEP_CLOSED) break;
    }
cleanup:
    papacc_pst_secure_processor_release(&client);
    if (transport != NULL) pst_transport_release(transport);
    if (socket_value != INVALID_SOCKET) closesocket(socket_value);
    for (index = 0U; index < 50U; ++index) {
        if (papacc_server_secure_io_win32_poll_once(secure_io, 1U) !=
                PAPACC_RESULT_OK) ok = 0;
    }
    return ok;
}

static int run_reference_client(PAPACC_U16 port,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, const char *scenario,
    PAPACC_BOOL stop_for_close)
{
    char executable[MAX_PATH];
    char command[MAX_PATH + 128];
    char event_name[96];
    char ack_event_name[96];
    char *separator;
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    DWORD exit_code = STILL_ACTIVE;
    PAPACC_SIZE rounds = 0U;
    PAPACC_BOOL stop_requested = PAPACC_FALSE;
    HANDLE shutdown_event;
    HANDLE shutdown_ack_event;
    if (GetModuleFileNameA(NULL, executable, sizeof(executable)) == 0U)
        return 0;
    separator = strrchr(executable, '\\');
    if (separator == NULL) return 0;
    strcpy_s(separator + 1U,
        sizeof(executable) - (size_t)(separator + 1U - executable),
        "papacc_secure_reference_client.exe");
    if (sprintf_s(event_name, sizeof(event_name),
            "Local\\papacc-3f-%lu-%lu", (unsigned long)GetCurrentProcessId(),
            (unsigned long)GetTickCount()) < 0) return 0;
    shutdown_event = CreateEventA(NULL, TRUE, FALSE, event_name);
    if (shutdown_event == NULL) return 0;
    if (sprintf_s(ack_event_name, sizeof(ack_event_name), "%s-ack",
            event_name) < 0) {
        CloseHandle(shutdown_event);
        return 0;
    }
    shutdown_ack_event = CreateEventA(NULL, TRUE, FALSE, ack_event_name);
    if (shutdown_ack_event == NULL ||
        sprintf_s(command, sizeof(command), "\"%s\" %u %s %s %s", executable,
            (unsigned int)port, event_name, ack_event_name, scenario) < 0) {
        if (shutdown_ack_event != NULL) CloseHandle(shutdown_ack_event);
        CloseHandle(shutdown_event);
        return 0;
    }
    memset(&startup, 0, sizeof(startup));
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startup.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    memset(&process, 0, sizeof(process));
    if (!CreateProcessA(executable, command, NULL, NULL, TRUE,
            CREATE_NO_WINDOW, NULL, NULL, &startup, &process)) return 0;
    while (exit_code == STILL_ACTIVE && rounds++ < 5000U) {
        if (papacc_server_secure_io_win32_poll_once(secure_io, 10U) !=
                PAPACC_RESULT_OK ||
            !GetExitCodeProcess(process.hProcess, &exit_code)) {
            TerminateProcess(process.hProcess, 255U);
            exit_code = 255U;
        }
        if (stop_for_close == PAPACC_TRUE &&
            stop_requested != PAPACC_TRUE &&
            WaitForSingleObject(shutdown_event, 0U) == WAIT_OBJECT_0) {
            if (papacc_server_secure_io_win32_request_stop(secure_io) !=
                    PAPACC_RESULT_OK) {
                TerminateProcess(process.hProcess, 254U);
                exit_code = 254U;
            } else {
                stop_requested = PAPACC_TRUE;
                (void)SetEvent(shutdown_ack_event);
            }
        }
    }
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    CloseHandle(shutdown_event);
    CloseHandle(shutdown_ack_event);
    if (exit_code != 0U)
        fprintf(stderr, "reference client exit=%lu rounds=%zu\n",
            (unsigned long)exit_code, (size_t)rounds);
    return exit_code == 0U;
}

int main(void)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture =
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_CONNECTION connections[CAPACITY];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT contexts[CAPACITY];
    PAPACC_SESSION sessions[CAPACITY];
    PAPACC_CHANNEL channels[CAPACITY];
    PAPACC_DATA_ASSOCIATION_ENTRY associations[CAPACITY];
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 protocol_slots[CAPACITY];
    PAPACC_SERVER_IO_LOOP_WIN32 protocol_loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_SECURITY_COMPOSITION composition =
        PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION security =
        PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION_INITIALIZER;
    PAPACC_SECURITY_DER_ITEM server_certificate;
    PAPACC_SECURITY_DER_ITEM trust_anchor;
    PAPACC_LOGGER logger;
    TEST_POLICY policy = { 0U, 0U, PAPACC_FALSE, { 0 }, PAPACC_FALSE, { 0 },
        PAPACC_PRINCIPAL_RESOLVED, PAPACC_AUTHORIZATION_ALLOW };
    PAPACC_SERVER_LISTENER_CONFIGURATION listener =
        PAPACC_SERVER_LISTENER_CONFIGURATION_INITIALIZER;
    PAPACC_SERVER_LISTENER_CONFIGURATION_SET listener_set = { &listener, 1U };
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidates[CAPACITY];
    PAPACC_PST_SCHEDULER_MEMBER scheduler_members[CAPACITY + 1U];
    PST_WAIT_EVENT scheduler_events[CAPACITY + 1U];
    pst_external_source *listener_sources[1];
    pst_wait_token listener_tokens[1];
    PAPACC_SERVER_SECURE_IO_WIN32 secure_io =
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    struct sockaddr_in native_address;
    int native_length = sizeof(native_address);
    PAPACC_U16 port;
    SOCKET client_socket = INVALID_SOCKET;
    pst_transport *client_transport = NULL;
    PAPACC_PST_SECURE_PROCESSOR client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR data_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR replay_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR foreign_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR matched_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR bad_protocol_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PST_CONNECTION_CONFIG client_config;
    PAPACC_BOOL ownership = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_RESULT server_result;
    PAPACC_SIZE offset = 0U;
    PAPACC_SIZE transferred;
    PAPACC_U8 response[sizeof(control_accept)];
    PAPACC_SIZE response_size = 0U;
    PAPACC_U8 ticket_frame[32];
    PAPACC_SIZE ticket_frame_size = 0U;
    PAPACC_U8 data_attach[32] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
    PAPACC_U8 data_response[sizeof(data_accept)];
    PAPACC_SIZE data_response_size = 0U;
    PAPACC_SIZE index;
    PAPACC_BOOL control_shutdown = PAPACC_FALSE;
    PAPACC_BOOL data_shutdown = PAPACC_FALSE;
    PAPACC_BOOL matched_shutdown = PAPACC_FALSE;
    int result = 0;

    CHECK(WSAStartup(MAKEWORD(2, 2), &winsock) == 0, 1);
    CHECK(papacc_test_security_fixture_init(&fixture), 2);
    CHECK(papacc_logger_init(&logger, discard_log, NULL,
        PAPACC_LOG_LEVEL_OFF) == PAPACC_RESULT_OK, 3);
    server_certificate.data = fixture.server.certificate.data;
    server_certificate.size = fixture.server.certificate.size;
    trust_anchor.data = fixture.ca.data;
    trust_anchor.size = fixture.ca.size;
    security.valid = PAPACC_TRUE;
    security.composition_inputs.local_certificate_chain = &server_certificate;
    security.composition_inputs.local_certificate_count = 1U;
    security.composition_inputs.local_private_key_pkcs8_der =
        fixture.server.private_key.data;
    security.composition_inputs.local_private_key_pkcs8_der_size =
        fixture.server.private_key.size;
    security.composition_inputs.peer_trust_anchors = &trust_anchor;
    security.composition_inputs.peer_trust_anchor_count = 1U;
    security.composition_inputs.secure_principal_alpn =
        (const PAPACC_U8 *)PAPACC_SECURITY_ALPN_PAPACC_1;
    security.composition_inputs.secure_principal_alpn_size =
        PAPACC_SECURITY_ALPN_PAPACC_1_SIZE;
    security.composition_inputs.provider_id = "openssl";
    security.composition_inputs.provider_bootstrap =
        papacc_pst_provider_bootstrap_win32;
    security.composition_inputs.logger = &logger;
    security.principal_resolve = resolve_principal;
    security.principal_resolve_context = &policy;
    security.authorize = authorize;
    security.authorization_context = &policy;
    CHECK(papacc_security_composition_init(&composition,
        &security.composition_inputs) == PAPACC_RESULT_OK, 4);

    CHECK(papacc_tcp_platform_init(&network.tcp_platform) == PAPACC_RESULT_OK &&
        papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1) ==
            PAPACC_RESULT_OK &&
        papacc_tcp_socket_win32_bind(&network.tcp_platform, &target, 0,
            &entry.socket) == PAPACC_RESULT_OK &&
        papacc_tcp_socket_win32_listen(&entry.socket) == PAPACC_RESULT_OK &&
        getsockname(entry.socket.native_socket,
            (struct sockaddr *)&native_address, &native_length) != SOCKET_ERROR,
        5);
    port = (PAPACC_U16)ntohs(native_address.sin_port);
    entry.target = target;
    network.listener_set.entries = &entry;
    network.listener_set.capacity = network.listener_set.count = 1U;
    network.listener_set.bound_port = port;
    network.listener_set.is_active = PAPACC_TRUE;
    network.listener_storage = &entry;
    network.listener_storage_capacity = 1U;
    network.is_active = PAPACC_TRUE;
    for (index = 0U; index < CAPACITY; ++index)
        contexts[index] = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
            PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    CHECK(papacc_server_acceptor_win32_init(&acceptor, &network, connections,
        CAPACITY, contexts, CAPACITY) == PAPACC_RESULT_OK, 6);
    CHECK(papacc_server_io_loop_win32_init(&protocol_loop, &network, &acceptor,
        sessions, CAPACITY, channels, CAPACITY, associations, CAPACITY,
        protocol_slots, CAPACITY, 5000000000ULL) == PAPACC_RESULT_OK, 7);
    listener.bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    listener.port = port;
    listener.transport_profile =
        PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL;
    CHECK(papacc_security_configuration_ref_set(
        &listener.security_configuration_ref, (const PAPACC_U8 *)"test", 4U) ==
        PAPACC_RESULT_OK, 8);
    CHECK(papacc_server_secure_io_win32_init(&secure_io, &network,
        &listener_set, &protocol_loop, &composition, &security, candidates,
        CAPACITY, scheduler_members, scheduler_events, CAPACITY + 1U,
        listener_sources, listener_tokens, 1U, 5000000000ULL) ==
        PAPACC_RESULT_OK, 9);

    client_socket = connect_loopback(port);
    CHECK(client_socket != INVALID_SOCKET, 10);
    papacc_test_pst_client_config(&client_config,
        fixture.client_a.credentials, fixture.trust);
    CHECK(papacc_pst_secure_processor_init(&client, composition.runtime,
        &client_config, ~(PAPACC_U64)0) == PAPACC_RESULT_OK, 11);
    CHECK(pst_win32_socket_transport_create((pst_size)client_socket,
        &client_transport) == PST_RESULT_OK, 12);
    CHECK(papacc_pst_secure_processor_attach(&client, client_transport,
        &ownership) == PAPACC_RESULT_OK && ownership == PAPACC_TRUE, 13);
    client_transport = NULL;
    client_socket = INVALID_SOCKET;
    for (index = 0U; index < 500U && client.state !=
            PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED; ++index) {
        CHECK(papacc_pst_secure_processor_handshake_once(&client, &step) ==
            PAPACC_RESULT_OK, 14);
        server_result = papacc_server_secure_io_win32_poll_once(
            &secure_io, 10U);
        if (server_result != PAPACC_RESULT_OK)
            fprintf(stderr, "server_result=%d round=%zu candidate=%d members=%zu\n",
                (int)server_result, (size_t)index, (int)candidates[0].state,
                (size_t)secure_io.scheduler.member_count);
        CHECK(server_result == PAPACC_RESULT_OK, 15);
    }
    CHECK(client.state == PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED &&
        policy.resolve_calls == 1U && policy.authorize_calls >= 1U, 16);

    for (index = 0U; index < 500U && offset < sizeof(control_open); ++index) {
        CHECK(papacc_pst_secure_processor_write_once(&client,
            control_open + offset, 1U,
            &transferred, &step) == PAPACC_RESULT_OK, 17);
        offset += transferred;
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 18);
    }
    CHECK(offset == sizeof(control_open), 19);
    for (index = 0U; index < 500U && response_size < sizeof(response); ++index) {
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 20);
        CHECK(papacc_pst_secure_processor_read_once(&client,
            response + response_size, sizeof(response) - response_size,
            &transferred, &step) == PAPACC_RESULT_OK, 21);
        response_size += transferred;
    }
    if (!(response_size == sizeof(response) &&
        memcmp(response, control_accept, sizeof(response)) == 0 &&
        protocol_loop.session_manager.count == 1U &&
        candidates[0].session_security.published == PAPACC_TRUE))
        fprintf(stderr, "response=%zu sessions=%zu published=%d kind=%d\n",
            (size_t)response_size,
            (size_t)protocol_loop.session_manager.count,
            (int)candidates[0].session_security.published,
            (int)protocol_slots[0].kind);
    CHECK(response_size == sizeof(response) &&
        memcmp(response, control_accept, sizeof(response)) == 0 &&
        protocol_loop.session_manager.count == 1U &&
        candidates[0].session_security.published == PAPACC_TRUE, 22);

    offset = 0U;
    for (index = 0U; index < 500U && offset < sizeof(data_ticket_request);
            ++index) {
        CHECK(papacc_pst_secure_processor_write_once(&client,
            data_ticket_request + offset,
            sizeof(data_ticket_request) - offset, &transferred, &step) ==
            PAPACC_RESULT_OK, 23);
        offset += transferred;
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 24);
    }
    CHECK(offset == sizeof(data_ticket_request), 25);
    for (index = 0U; index < 500U && ticket_frame_size < sizeof(ticket_frame);
            ++index) {
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 26);
        CHECK(papacc_pst_secure_processor_read_once(&client,
            ticket_frame + ticket_frame_size,
            sizeof(ticket_frame) - ticket_frame_size, &transferred, &step) ==
            PAPACC_RESULT_OK, 27);
        ticket_frame_size += transferred;
    }
    CHECK(ticket_frame_size == sizeof(ticket_frame) &&
        ticket_frame[0] == 0x50 && ticket_frame[8] == 0x00 &&
        ticket_frame[9] == 0x04 && ticket_frame[15] == 0x10, 28);
    memcpy(data_attach + 16U, ticket_frame + 16U, 16U);

    client_socket = connect_loopback(port);
    CHECK(client_socket != INVALID_SOCKET, 29);
    papacc_test_pst_client_config(&client_config,
        fixture.client_a.credentials, fixture.trust);
    CHECK(papacc_pst_secure_processor_init(&data_client, composition.runtime,
        &client_config, ~(PAPACC_U64)0) == PAPACC_RESULT_OK, 30);
    CHECK(pst_win32_socket_transport_create((pst_size)client_socket,
        &client_transport) == PST_RESULT_OK, 31);
    CHECK(papacc_pst_secure_processor_attach(&data_client, client_transport,
        &ownership) == PAPACC_RESULT_OK && ownership == PAPACC_TRUE, 32);
    client_transport = NULL;
    client_socket = INVALID_SOCKET;
    for (index = 0U; index < 500U && data_client.state !=
            PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED; ++index) {
        CHECK(papacc_pst_secure_processor_handshake_once(&data_client, &step) ==
            PAPACC_RESULT_OK, 33);
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 34);
    }
    CHECK(data_client.state == PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED, 35);

    offset = 0U;
    for (index = 0U; index < 500U && offset < sizeof(data_attach); ++index) {
        CHECK(papacc_pst_secure_processor_write_once(&data_client,
            data_attach + offset, sizeof(data_attach) - offset,
            &transferred, &step) == PAPACC_RESULT_OK, 36);
        offset += transferred;
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 37);
    }
    CHECK(offset == sizeof(data_attach), 38);
    for (index = 0U; index < 500U &&
            data_response_size < sizeof(data_response); ++index) {
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 39);
        CHECK(papacc_pst_secure_processor_read_once(&data_client,
            data_response + data_response_size,
            sizeof(data_response) - data_response_size,
            &transferred, &step) == PAPACC_RESULT_OK, 40);
        data_response_size += transferred;
    }
    if (!(data_response_size == sizeof(data_response) &&
        memcmp(data_response, data_accept, sizeof(data_response)) == 0 &&
        protocol_loop.channel_manager.count == 2U &&
        protocol_loop.association_manager.count == 0U &&
        candidates[1].connection_security.state ==
            PAPACC_SECURITY_CONTEXT_AUTHORIZED &&
        candidates[1].data_gate_installed == PAPACC_TRUE))
        fprintf(stderr, "data_response=%zu channels=%zu associations=%zu "
            "security_state=%d data_gate=%d kind=%d candidate_state=%d\n",
            (size_t)data_response_size,
            (size_t)protocol_loop.channel_manager.count,
            (size_t)protocol_loop.association_manager.count,
            (int)candidates[1].connection_security.state,
            (int)candidates[1].data_gate_installed,
            (int)protocol_slots[1].kind, (int)candidates[1].state);
    CHECK(data_response_size == sizeof(data_response) &&
        memcmp(data_response, data_accept, sizeof(data_response)) == 0 &&
        protocol_loop.channel_manager.count == 2U &&
        protocol_loop.association_manager.count == 0U &&
        candidates[1].connection_security.state ==
            PAPACC_SECURITY_CONTEXT_AUTHORIZED &&
        candidates[1].data_gate_installed == PAPACC_TRUE, 41);

    /* A consumed ticket is rejected without disturbing the CONTROL Session. */
    CHECK(establish_client(port, &composition, &secure_io,
        fixture.client_a.credentials, fixture.trust, &replay_client), 42);
    CHECK(write_all(&replay_client, &secure_io, data_attach,
        sizeof(data_attach), 3U), 43);
    for (index = 0U; index < 100U; ++index)
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
            PAPACC_RESULT_OK, 44);
    CHECK(protocol_loop.session_manager.count == 1U &&
        protocol_loop.channel_manager.count == 2U &&
        protocol_loop.association_manager.count == 0U, 45);
    papacc_pst_secure_processor_release(&replay_client);

    /* Obtain a fresh ticket, reject Principal B, then accept Principal A. */
    CHECK(write_all(&client, &secure_io, data_ticket_request,
        sizeof(data_ticket_request), 2U), 46);
    ticket_frame_size = 0U;
    CHECK(read_exact(&client, &secure_io, ticket_frame,
        sizeof(ticket_frame)), 47);
    memcpy(data_attach + 16U, ticket_frame + 16U, 16U);
    CHECK(protocol_loop.association_manager.count == 1U, 48);

    CHECK(establish_client(port, &composition, &secure_io,
        fixture.client_b.credentials, fixture.trust, &foreign_client), 49);
    CHECK(write_all(&foreign_client, &secure_io, data_attach,
        sizeof(data_attach), 1U), 50);
    for (index = 0U; index < 100U; ++index)
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
            PAPACC_RESULT_OK, 51);
    CHECK(protocol_loop.session_manager.count == 1U &&
        protocol_loop.channel_manager.count == 2U &&
        protocol_loop.association_manager.count == 1U, 52);
    papacc_pst_secure_processor_release(&foreign_client);

    CHECK(establish_client(port, &composition, &secure_io,
        fixture.client_a.credentials, fixture.trust, &matched_client), 53);
    CHECK(write_all(&matched_client, &secure_io, data_attach,
        sizeof(data_attach), 1U), 54);
    data_response_size = 0U;
    CHECK(read_exact(&matched_client, &secure_io, data_response,
        sizeof(data_response)), 55);
    CHECK(memcmp(data_response, data_accept, sizeof(data_response)) == 0 &&
        protocol_loop.session_manager.count == 1U &&
        protocol_loop.channel_manager.count == 3U &&
        protocol_loop.association_manager.count == 0U, 56);

    /* Pre-TLS garbage and plaintext PACC never reach the classifier. */
    {
        static const PAPACC_U8 malformed_tls[9] =
            { 0x16, 0x03, 0x03, 0x00, 0x04, 0xde, 0xad, 0xbe, 0xef };
        CHECK(raw_candidate_rejected(port, &secure_io, malformed_tls,
            sizeof(malformed_tls)), 57);
        CHECK(raw_candidate_rejected(port, &secure_io, control_open,
            sizeof(control_open)), 58);
        CHECK(raw_candidate_rejected(port, &secure_io, data_attach,
            sizeof(data_attach)), 59);
        CHECK(protocol_loop.session_manager.count == 1U &&
            protocol_loop.channel_manager.count == 3U &&
            protocol_loop.association_manager.count == 0U &&
            listener.transport_profile ==
                PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL, 60);
    }

    {
        static const PAPACC_U8 wrong_alpn_bytes[] = "wrong/1";
        static const PST_ALPN_PROTOCOL wrong_alpn = {
            wrong_alpn_bytes, sizeof(wrong_alpn_bytes) - 1U };
        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        policy.forced_resolution = PAPACC_PRINCIPAL_NOT_ENROLLED;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 68);
        policy.forced_resolution = PAPACC_PRINCIPAL_RESOLVED;

        policy.forced_decision = PAPACC_AUTHORIZATION_DENY;
        papacc_test_pst_client_config(&client_config,
            fixture.client_a.credentials, fixture.trust);
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 69);
        policy.forced_decision = PAPACC_AUTHORIZATION_ALLOW;

        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        client_config.alpn.protocols = &wrong_alpn;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 70);

        papacc_test_pst_client_config(&client_config, NULL, fixture.trust);
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 71);

        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        client_config.tls.minimum_version = PST_TLS_VERSION_1_2;
        client_config.tls.maximum_version = PST_TLS_VERSION_1_2;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 73);

        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        client_config.alpn.mode = PST_FEATURE_DISABLED;
        client_config.alpn.protocols = NULL;
        client_config.alpn.protocol_count = 0U;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 74);

        {
            static const PAPACC_U8 invalid_first_frame[16] = {
                0x50,0x41,0x43,0x43,1,0,0,16,0,0x7f,0,0,0,0,0,0 };
            CHECK(establish_client(port, &composition, &secure_io,
                fixture.client_a.credentials, fixture.trust,
                &bad_protocol_client), 75);
            CHECK(write_all(&bad_protocol_client, &secure_io,
                invalid_first_frame, sizeof(invalid_first_frame), 1U), 76);
            for (index = 0U; index < 50U; ++index)
                CHECK(papacc_server_secure_io_win32_poll_once(
                    &secure_io, 1U) == PAPACC_RESULT_OK, 77);
            papacc_pst_secure_processor_release(&bad_protocol_client);
        }
        CHECK(protocol_loop.session_manager.count == 1U &&
            protocol_loop.channel_manager.count == 3U &&
            protocol_loop.connection_manager->count == 3U &&
            listener.transport_profile ==
                PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL, 72);
    }

    CHECK(run_reference_client(port, &secure_io, "tls12", PAPACC_FALSE), 83);
    CHECK(run_reference_client(port, &secure_io, "wrong-alpn",
        PAPACC_FALSE), 84);
    CHECK(run_reference_client(port, &secure_io, "missing-credential",
        PAPACC_FALSE), 85);
    CHECK(run_reference_client(port, &secure_io, "not-enrolled",
        PAPACC_FALSE), 86);
    CHECK(run_reference_client(port, &secure_io, "deny", PAPACC_FALSE), 87);

    papacc_pst_secure_processor_release(&matched_client);
    papacc_pst_secure_processor_release(&data_client);
    papacc_pst_secure_processor_release(&client);
    control_shutdown = data_shutdown = matched_shutdown = PAPACC_TRUE;
    for (index = 0U; index < 200U &&
            protocol_loop.connection_manager->count != 0U; ++index)
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
            PAPACC_RESULT_OK, 81);
    CHECK(protocol_loop.connection_manager->count == 0U, 82);
    CHECK(run_reference_client(port, &secure_io, "main", PAPACC_TRUE), 80);
    CHECK(papacc_server_secure_io_win32_request_stop(&secure_io) ==
        PAPACC_RESULT_OK, 61);
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_TRUE, 62);
    for (index = 0U; index < 500U &&
            papacc_server_secure_io_win32_is_stopped(&secure_io) !=
                PAPACC_TRUE; ++index) {
        if (control_shutdown != PAPACC_TRUE) {
            CHECK(papacc_pst_secure_processor_shutdown_once(&client, &step) ==
                PAPACC_RESULT_OK, 63);
            if (step == PAPACC_PST_SECURE_STEP_COMPLETE ||
                step == PAPACC_PST_SECURE_STEP_CLOSED)
                control_shutdown = PAPACC_TRUE;
        }
        if (data_shutdown != PAPACC_TRUE) {
            CHECK(papacc_pst_secure_processor_shutdown_once(
                &data_client, &step) == PAPACC_RESULT_OK, 64);
            if (step == PAPACC_PST_SECURE_STEP_COMPLETE ||
                step == PAPACC_PST_SECURE_STEP_CLOSED)
                data_shutdown = PAPACC_TRUE;
        }
        if (matched_shutdown != PAPACC_TRUE) {
            CHECK(papacc_pst_secure_processor_shutdown_once(
                &matched_client, &step) == PAPACC_RESULT_OK, 65);
            if (step == PAPACC_PST_SECURE_STEP_COMPLETE ||
                step == PAPACC_PST_SECURE_STEP_CLOSED)
                matched_shutdown = PAPACC_TRUE;
        }
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 66);
    }
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_TRUE && protocol_loop.connection_manager->count == 0U &&
        protocol_loop.session_manager.count == 0U &&
        protocol_loop.channel_manager.count == 0U, 67);

cleanup:
    papacc_server_secure_io_win32_shutdown(&secure_io);
    papacc_pst_secure_processor_release(&bad_protocol_client);
    papacc_pst_secure_processor_release(&matched_client);
    papacc_pst_secure_processor_release(&foreign_client);
    papacc_pst_secure_processor_release(&replay_client);
    papacc_pst_secure_processor_release(&data_client);
    papacc_pst_secure_processor_release(&client);
    if (client_transport != NULL) pst_transport_release(client_transport);
    if (client_socket != INVALID_SOCKET) closesocket(client_socket);
    papacc_server_io_loop_win32_shutdown(&protocol_loop);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    papacc_tcp_socket_win32_close(&entry.socket);
    papacc_tcp_platform_shutdown(&network.tcp_platform);
    papacc_security_composition_release(&composition);
    papacc_test_security_fixture_release(&fixture);
    WSACleanup();
    return result;
}
