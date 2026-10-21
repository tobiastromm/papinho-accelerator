/* Exercise the exact fixture implementation, not a copied TLS worker. */
#define main fixture_program_main
#include "secure_fixture_server_win32.c"
#undef main

static DWORD WINAPI error_worker(void *context)
{
    (void)context;
    return 4U;
}

static DWORD WINAPI late_worker(void *context)
{
    return WaitForSingleObject((HANDLE)context, INFINITE) == WAIT_OBJECT_0 ? 0U : 1U;
}

static int join_case(char which)
{
    HANDLE stop = CreateEventA(NULL, TRUE, FALSE, NULL);
    HANDLE worker;
    DWORD code;
    if (stop == NULL) return 90;
    worker = CreateThread(NULL, 0U, which == 'D' ? error_worker : late_worker,
        stop, 0U, NULL);
    if (worker == NULL) { CloseHandle(stop); return 91; }
    code = fixture_join_thread(&worker, which == 'E' ? 20U : 1000U, stop, NULL);
    printf("JOIN_RESULT=%lu HANDLE_RELEASED_AFTER_EXIT=%s\n",
        (unsigned long)code, worker == NULL ? "YES" : "NO");
    CloseHandle(stop);
    /* The process really returns the error; the outer harness expects 4. */
    return (int)code;
}

static int client_wait(pst_connection *connection, ULONGLONG deadline)
{
    PST_WAIT_RESULT observed;
    ULONGLONG now = GetTickCount64();
    return now < deadline && pst_connection_wait(connection,
        (pst_u32)(deadline - now), &observed) == PST_RESULT_OK;
}

static int tls_case(char which)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE material = PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PST_RUNTIME_OPTIONS options;
    PST_CONNECTION_CONFIG config;
    FIXTURE_HTTPS_TARGET target;
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    pst_runtime *runtime = NULL;
    pst_connection *client = NULL;
    pst_transport *transport = NULL;
    SOCKET listener = INVALID_SOCKET, socket_value = INVALID_SOCKET;
    HANDLE worker = NULL;
    HANDLE observed_worker = NULL;
    int graceful_joined = 0;
    PAPACC_U16 port = 0U;
    struct sockaddr_in address;
    pst_u32 ownership = 0U, operation = PST_OPERATION_NEED_READ;
    PST_RESULT source = PST_RESULT_OK;
    ULONGLONG deadline, stopped_at = 0U;
    DWORD code = 99U;
    u_long nonblocking = 1UL;
    int result = 1;
    unsigned int failure_line = 0U;
    size_t sent = 0U, received = 0U;
    char response[256];
    const char request[] = "GET / HTTP/1.1\r\nHost: papacc-test-server\r\n\r\n";
    memset(&target, 0, sizeof(target));
    if (WSAStartup(MAKEWORD(2,2), &winsock) != 0) return 1;
    if (papacc_pst_provider_bootstrap_win32(NULL) != PAPACC_RESULT_OK ||
        !papacc_test_security_fixture_init(&material)) { failure_line = __LINE__; goto done; }
    memset(&options, 0, sizeof(options)); options.struct_size = sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK ||
        !create_target_listener(&listener, &port, &endpoint)) { failure_line = __LINE__; goto done; }
    target.listener = listener; target.runtime = runtime;
    target.credentials = material.server.credentials;
    target.use_tls = PAPACC_TRUE; target.require_alpn = PAPACC_TRUE;
    target.stop_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    target.read_wait_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    target.shutdown_wait_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (!target.stop_event || !target.read_wait_event || !target.shutdown_wait_event) { failure_line = __LINE__; goto done; }
    if (which == 'A') {
        target.worker_published_event = CreateEventA(NULL, TRUE, FALSE, NULL);
        if (!target.worker_published_event) { failure_line = __LINE__; goto done; }
        target.observed_worker_handle = &observed_worker;
    }
    worker = CreateThread(NULL, 0U, https_target_thread, &target, 0U, NULL);
    if (worker == NULL) { failure_line = __LINE__; goto done; }
    socket_value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&address, 0, sizeof(address)); address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); address.sin_port = htons(port);
    if (socket_value == INVALID_SOCKET || connect(socket_value,
        (struct sockaddr *)&address, sizeof(address)) != 0 ||
        ioctlsocket(socket_value, FIONBIO, &nonblocking) != 0) { failure_line = __LINE__; goto done; }
    papacc_test_pst_client_config(&config, NULL, material.trust);
    if (pst_connection_create(runtime, &config, &client) != PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)socket_value, &transport) != PST_RESULT_OK ||
        pst_connection_attach(client, transport, PST_OWNERSHIP_TRANSFERRED, &ownership) != PST_RESULT_OK ||
        ownership == 0U) { failure_line = __LINE__; goto done; }
    socket_value = INVALID_SOCKET; transport = NULL;
    deadline = GetTickCount64() + 5000U;
    do {
        if (pst_connection_handshake(client, &operation, &source) != PST_RESULT_OK ||
            operation == PST_OPERATION_FAILED || operation == PST_OPERATION_CLOSED) { failure_line = __LINE__; goto done; }
        if (operation == PST_OPERATION_COMPLETE) break;
        if (!client_wait(client, deadline)) { failure_line = __LINE__; goto done; }
    } while (GetTickCount64() < deadline);
    if (operation != PST_OPERATION_COMPLETE) { failure_line = __LINE__; goto done; }
    if (which == 'B') {
        if (WaitForSingleObject(target.read_wait_event, 2000U) != WAIT_OBJECT_0) { failure_line = __LINE__; goto done; }
    } else {
        while (sent < sizeof(request) - 1U) {
            PST_IO_RESULT io;
            if (pst_connection_write(client, (const pst_u8 *)request + sent,
                sizeof(request) - 1U - sent, &io) != PST_RESULT_OK ||
                io.operation == PST_OPERATION_FAILED || io.operation == PST_OPERATION_CLOSED) { failure_line = __LINE__; goto done; }
            sent += io.bytes_transferred;
            if (sent < sizeof(request) - 1U && !client_wait(client, deadline)) { failure_line = __LINE__; goto done; }
        }
        while (received < sizeof(fixture_http_response) - 1U) {
            PST_IO_RESULT io;
            if (pst_connection_read(client, (pst_u8 *)response + received,
                sizeof(response) - received, &io) != PST_RESULT_OK ||
                io.operation == PST_OPERATION_FAILED || io.operation == PST_OPERATION_CLOSED) { failure_line = __LINE__; goto done; }
            received += io.bytes_transferred;
            if (received < sizeof(fixture_http_response) - 1U && !client_wait(client, deadline)) { failure_line = __LINE__; goto done; }
        }
        if (received != sizeof(fixture_http_response) - 1U ||
            memcmp(response, fixture_http_response, received) != 0) { failure_line = __LINE__; goto done; }
        puts("EXACT_HTTP_RESPONSE_BEFORE_STOP=PASS");
        if (which == 'A') {
            do {
                if (pst_connection_shutdown(client, &operation, &source) != PST_RESULT_OK ||
                    operation == PST_OPERATION_FAILED) { failure_line = __LINE__; goto done; }
                if (operation == PST_OPERATION_COMPLETE || operation == PST_OPERATION_CLOSED) break;
                if (!client_wait(client, deadline)) { failure_line = __LINE__; goto done; }
            } while (GetTickCount64() < deadline);
            if (operation != PST_OPERATION_COMPLETE && operation != PST_OPERATION_CLOSED) { failure_line = __LINE__; goto done; }
            /* Client completion does not authorize stopping the server. Wait
               for the actual server thread exit, then inspect its terminal. */
            if (WaitForSingleObject(target.worker_published_event, 5000U) != WAIT_OBJECT_0 ||
                !observed_worker || WaitForSingleObject(observed_worker, 5000U) != WAIT_OBJECT_0) {
                puts("CASE_A=FAIL ROOT_CAUSE=NOT_YET_PROVEN WORKER_WAIT=FAILED_OR_TIMEOUT");
                { failure_line = __LINE__; goto done; }
            }
            if (!GetExitCodeThread(observed_worker, &code)) { failure_line = __LINE__; goto done; }
            printf("CASE_A_WORKER_EXIT_CODE=%lu SHUTDOWN_OPERATION=%lu SHUTDOWN_SOURCE=%lu\n",
                (unsigned long)code, (unsigned long)target.observed_shutdown_operation,
                (unsigned long)target.observed_shutdown_source);
            if (code != 0U || target.graceful_count != 1 || target.cancelled_shutdown_count != 0 ||
                target.observed_shutdown_operation != PST_OPERATION_COMPLETE ||
                target.observed_shutdown_source != PST_RESULT_OK ||
                WaitForSingleObject(target.stop_event, 0U) != WAIT_TIMEOUT) { failure_line = __LINE__; goto done; }
            graceful_joined = 1;
            puts("GRACEFUL_SHUTDOWN_AUTHORITY=PST_SHUTDOWN_COMPLETE_SOURCE_OK_AND_WORKER_EXIT");
            puts("CASE_A_STOP_BEFORE_COMPLETION=NO");
            puts("CASE_A_GRACEFUL_SHUTDOWN=PASS CASE_A_WORKER_EXIT=PASS CASE_A_JOIN=PASS");
            puts("CASE_A_LISTENER_CLEANUP_STOP_AFTER_WORKER_EXIT=YES");
        } else if (WaitForSingleObject(target.shutdown_wait_event, 2000U) != WAIT_OBJECT_0) { failure_line = __LINE__; goto done; }
    }
    stopped_at = GetTickCount64(); SetEvent(target.stop_event);
    code = fixture_join_thread(&worker, 2000U, target.stop_event, NULL);
    if (GetTickCount64() - stopped_at > 2000U) { failure_line = __LINE__; goto done; }
    if (which == 'B') {
        if (code != 4U || target.response_count != 0) { failure_line = __LINE__; goto done; }
        puts("CANCEL_DURING_READ=BOUNDED_FAILURE_NO_FALSE_RESPONSE");
    } else if (code != 0U || target.response_count != 1 ||
        (which == 'A' ? target.graceful_count != 1 :
            target.cancelled_shutdown_count != 1 || target.graceful_count != 0)) { failure_line = __LINE__; goto done; }
    printf("CASE_%c=PASS STOP_TO_JOIN_MS=%llu\n", which,
        (unsigned long long)(GetTickCount64() - stopped_at));
    result = 0;
done:
    if (result != 0)
        fprintf(stderr, "FIXTURE_CLIENT_FAILURE line=%u operation=%lu source=%lu wsa=%d\n",
            failure_line, (unsigned long)operation, (unsigned long)source,
            WSAGetLastError());
    if (which == 'A' && result != 0 && !graceful_joined)
        puts("CASE_A_RECOVERY_STOP=YES CASE_A=FAIL");
    if (target.stop_event) SetEvent(target.stop_event);
    if (worker) (void)fixture_join_thread(&worker, 20000U, target.stop_event, NULL);
    if (observed_worker) CloseHandle(observed_worker);
    if (target.worker_published_event) CloseHandle(target.worker_published_event);
    if (client) pst_connection_release(client);
    if (transport) pst_transport_release(transport);
    if (socket_value != INVALID_SOCKET) closesocket(socket_value);
    if (listener != INVALID_SOCKET) closesocket(listener);
    if (target.stop_event) CloseHandle(target.stop_event);
    if (target.read_wait_event) CloseHandle(target.read_wait_event);
    if (target.shutdown_wait_event) CloseHandle(target.shutdown_wait_event);
    if (runtime) pst_runtime_release(runtime);
    papacc_test_security_fixture_release(&material); WSACleanup();
    return result;
}

int main(int argc, char **argv)
{
    WSANETWORKEVENTS events;
    memset(&events, 0x5A, sizeof(events));
    events.lNetworkEvents = 0L;
    if (fixture_accept_event_failed(&events)) return 80;
    events.lNetworkEvents = FD_READ;
    if (fixture_accept_event_failed(&events)) return 81;
    events.lNetworkEvents = FD_ACCEPT;
    events.iErrorCode[FD_ACCEPT_BIT] = WSAECONNABORTED;
    if (!fixture_accept_event_failed(&events)) return 82;
    events.iErrorCode[FD_ACCEPT_BIT] = 0;
    if (fixture_accept_event_failed(&events)) return 83;
    puts("FIXTURE_EVENT_MASK_NEGATIVES=4_PASS");
    if (argc != 2 || strlen(argv[1]) != 1U) return 2;
    if (argv[1][0] == 'D' || argv[1][0] == 'E') return join_case(argv[1][0]);
    if (argv[1][0] == 'A' || argv[1][0] == 'B' || argv[1][0] == 'C' || argv[1][0] == 'F')
        return tls_case(argv[1][0]);
    return 2;
}
