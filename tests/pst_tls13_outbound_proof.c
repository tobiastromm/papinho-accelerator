#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>
#include <string.h>

#include "papinho_secure_transport.h"
#include "papinho_secure_transport_win32.h"

#define PAPACC_PROOF_HOST "google.com"
#define PAPACC_PROOF_PORT "443"
#define PAPACC_PROOF_WRONG_HOST "wrong-hostname.invalid"
#define PAPACC_PROOF_DEADLINE_MS 30000UL
#define PAPACC_PROOF_CONNECT_WAIT_MS 5000L
#define PAPACC_PROOF_MAX_STEPS 512U
#define PAPACC_PROOF_MAX_RESPONSE_BYTES 65536UL

typedef struct PAPACC_PROOF_CONTEXT {
    WSADATA winsock_data;
    int winsock_started;
    SOCKET socket_value;
    pst_runtime *runtime;
    pst_trust *trust;
    pst_connection *connection;
    pst_transport *transport;
    char provider_id[32];
    char status_line[256];
    size_t status_length;
    int status_complete;
    unsigned long response_bytes;
    pst_u32 shutdown_close_kind;
} PAPACC_PROOF_CONTEXT;

static ULONGLONG papacc_proof_now_ms(void)
{
    return GetTickCount64();
}

static unsigned long papacc_proof_remaining_ms(ULONGLONG deadline)
{
    ULONGLONG now = papacc_proof_now_ms();
    ULONGLONG remaining;
    if (now >= deadline) return 0UL;
    remaining = deadline - now;
    return remaining > 250ULL ? 250UL : (unsigned long)remaining;
}

static void PST_CALL papacc_proof_pst_log(
    void *user_context, const PST_LOG_EVENT *event)
{
    (void)user_context;
    if (event == NULL) return;
    printf("PST_EVENT level=%lu event=%lu result=%ld operation=%lu backend=%s\n",
        (unsigned long)event->level, (unsigned long)event->event_id,
        (long)event->normalized_result, (unsigned long)event->operation,
        event->backend_id);
}

static void papacc_proof_cleanup(PAPACC_PROOF_CONTEXT *context)
{
    if (context->connection != NULL)
        pst_connection_release(context->connection);
    if (context->transport != NULL)
        pst_transport_release(context->transport);
    else if (context->socket_value != INVALID_SOCKET)
        closesocket(context->socket_value);
    if (context->trust != NULL) pst_trust_release(context->trust);
    if (context->runtime != NULL) pst_runtime_release(context->runtime);
    if (context->winsock_started) WSACleanup();
}

static int papacc_proof_tcp_connect(PAPACC_PROOF_CONTEXT *context)
{
    struct addrinfo hints;
    struct addrinfo *addresses = NULL;
    struct addrinfo *current;
    int dns_result;
    unsigned int attempts = 0U;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    dns_result = getaddrinfo(PAPACC_PROOF_HOST, PAPACC_PROOF_PORT,
        &hints, &addresses);
    if (dns_result != 0) {
        printf("DNS_RESULT=FAIL code=%d\n", dns_result);
        return 0;
    }
    puts("DNS_RESULT=PASS");

    for (current = addresses; current != NULL && attempts < 16U;
         current = current->ai_next) {
        u_long nonblocking = 1UL;
        fd_set write_set;
        fd_set error_set;
        struct timeval timeout;
        int selected;
        int socket_error = 0;
        int socket_error_size = (int)sizeof(socket_error);
        SOCKET candidate;
        ++attempts;
        candidate = socket(current->ai_family, current->ai_socktype,
            current->ai_protocol);
        if (candidate == INVALID_SOCKET) continue;
        if (ioctlsocket(candidate, FIONBIO, &nonblocking) == SOCKET_ERROR) {
            closesocket(candidate);
            continue;
        }
        if (connect(candidate, current->ai_addr, (int)current->ai_addrlen) ==
                SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
            closesocket(candidate);
            continue;
        }
        FD_ZERO(&write_set);
        FD_ZERO(&error_set);
        FD_SET(candidate, &write_set);
        FD_SET(candidate, &error_set);
        timeout.tv_sec = PAPACC_PROOF_CONNECT_WAIT_MS / 1000L;
        timeout.tv_usec = (PAPACC_PROOF_CONNECT_WAIT_MS % 1000L) * 1000L;
        selected = select(0, NULL, &write_set, &error_set, &timeout);
        if (selected > 0 && FD_ISSET(candidate, &write_set) &&
            getsockopt(candidate, SOL_SOCKET, SO_ERROR,
                (char *)&socket_error, &socket_error_size) == 0 &&
            socket_error == 0) {
            context->socket_value = candidate;
            break;
        }
        closesocket(candidate);
    }
    freeaddrinfo(addresses);
    if (context->socket_value == INVALID_SOCKET) {
        printf("TCP_RESULT=FAIL attempts=%u\n", attempts);
        return 0;
    }
    printf("TCP_RESULT=PASS host=%s port=%s attempts=%u\n",
        PAPACC_PROOF_HOST, PAPACC_PROOF_PORT, attempts);
    return 1;
}

static int papacc_proof_wait(
    pst_connection *connection, ULONGLONG deadline)
{
    PST_WAIT_RESULT wait_result;
    PST_RESULT result;
    unsigned long remaining = papacc_proof_remaining_ms(deadline);
    if (remaining == 0UL) return 0;
    memset(&wait_result, 0, sizeof(wait_result));
    result = pst_connection_wait(connection, (pst_u32)remaining, &wait_result);
    if (result != PST_RESULT_OK) return 0;
    return wait_result.timed_out == 0UL ? 1 :
        papacc_proof_remaining_ms(deadline) != 0UL;
}

static PST_RESULT papacc_proof_handshake(
    pst_connection *connection, ULONGLONG deadline)
{
    unsigned int step;
    for (step = 0U; step < PAPACC_PROOF_MAX_STEPS; ++step) {
        pst_u32 operation = PST_OPERATION_FAILED;
        PST_RESULT error = PST_RESULT_BACKEND_FAILURE;
        PST_RESULT result = pst_connection_handshake(
            connection, &operation, &error);
        if (result != PST_RESULT_OK) return result;
        if (operation == PST_OPERATION_COMPLETE) return PST_RESULT_OK;
        if (operation == PST_OPERATION_FAILED) return error;
        if (!papacc_proof_wait(connection, deadline))
            return PST_RESULT_TRANSPORT_FAILURE;
    }
    return PST_RESULT_RESOURCE_FAILURE;
}

static PST_RESULT papacc_proof_write_all(
    pst_connection *connection, const unsigned char *data, pst_size length,
    ULONGLONG deadline)
{
    pst_size total = 0U;
    unsigned int step;
    for (step = 0U; step < PAPACC_PROOF_MAX_STEPS && total < length; ++step) {
        PST_IO_RESULT io_result;
        PST_RESULT result;
        memset(&io_result, 0, sizeof(io_result));
        result = pst_connection_write(connection, data + total,
            length - total, &io_result);
        if (result != PST_RESULT_OK) return result;
        total += io_result.bytes_transferred;
        if (io_result.operation == PST_OPERATION_FAILED) return io_result.error;
        if (io_result.operation == PST_OPERATION_CLOSED) return PST_RESULT_CLOSED;
        if (total < length && !papacc_proof_wait(connection, deadline))
            return PST_RESULT_TRANSPORT_FAILURE;
    }
    return total == length ? PST_RESULT_OK : PST_RESULT_RESOURCE_FAILURE;
}

static void papacc_proof_capture_status(
    PAPACC_PROOF_CONTEXT *context, const unsigned char *data, pst_size size)
{
    pst_size index;
    for (index = 0U; index < size &&
         context->status_length + 1U < sizeof(context->status_line); ++index) {
        unsigned char value = data[index];
        if (value == '\r') continue;
        if (value == '\n') {
            context->status_line[context->status_length] = '\0';
            context->status_complete = 1;
            return;
        }
        context->status_line[context->status_length++] = (char)value;
    }
    context->status_line[context->status_length] = '\0';
}

static PST_RESULT papacc_proof_read_response(
    PAPACC_PROOF_CONTEXT *context, ULONGLONG deadline)
{
    unsigned char buffer[2048];
    unsigned int step;
    for (step = 0U; step < PAPACC_PROOF_MAX_STEPS; ++step) {
        PST_IO_RESULT io_result;
        PST_RESULT result;
        memset(&io_result, 0, sizeof(io_result));
        result = pst_connection_read(context->connection, buffer,
            sizeof(buffer), &io_result);
        if (result != PST_RESULT_OK) return result;
        if (io_result.bytes_transferred != 0U) {
            if (context->response_bytes > PAPACC_PROOF_MAX_RESPONSE_BYTES -
                    (unsigned long)io_result.bytes_transferred)
                return PST_RESULT_RESOURCE_FAILURE;
            if (!context->status_complete)
                papacc_proof_capture_status(context, buffer,
                    io_result.bytes_transferred);
            context->response_bytes += (unsigned long)io_result.bytes_transferred;
        }
        if (io_result.operation == PST_OPERATION_FAILED) return io_result.error;
        if (io_result.operation == PST_OPERATION_CLOSED) {
            context->shutdown_close_kind = io_result.close_kind;
            return PST_RESULT_OK;
        }
        if (context->response_bytes >= PAPACC_PROOF_MAX_RESPONSE_BYTES)
            return PST_RESULT_RESOURCE_FAILURE;
        if (io_result.bytes_transferred == 0U &&
            !papacc_proof_wait(context->connection, deadline))
            return PST_RESULT_TRANSPORT_FAILURE;
    }
    return PST_RESULT_RESOURCE_FAILURE;
}

static PST_RESULT papacc_proof_shutdown(
    PAPACC_PROOF_CONTEXT *context, ULONGLONG deadline)
{
    unsigned int step;
    for (step = 0U; step < PAPACC_PROOF_MAX_STEPS; ++step) {
        pst_u32 operation = PST_OPERATION_FAILED;
        PST_RESULT error = PST_RESULT_BACKEND_FAILURE;
        PST_RESULT result = pst_connection_shutdown(
            context->connection, &operation, &error);
        if (result != PST_RESULT_OK) return result;
        if (operation == PST_OPERATION_COMPLETE) return PST_RESULT_OK;
        if (operation == PST_OPERATION_FAILED) return error;
        if (!papacc_proof_wait(context->connection, deadline))
            return PST_RESULT_TRANSPORT_FAILURE;
    }
    return PST_RESULT_RESOURCE_FAILURE;
}

static int papacc_proof_run(int wrong_hostname)
{
    PAPACC_PROOF_CONTEXT context;
    PST_RUNTIME_OPTIONS options;
    PST_RUNTIME_INFO runtime_info;
    PST_PROVIDER_INFO provider_info;
    PST_TRUST_SOURCE trust_source;
    PST_CONNECTION_CONFIG connection_config;
    PST_LOG_CONFIG log_config;
    PST_DIAGNOSTIC_INFO diagnostic;
    PST_PEER_INFO_SUMMARY peer_summary;
    pst_peer_info *peer_info = NULL;
    PST_RESULT result;
    ULONGLONG deadline;
    static const unsigned char request[] =
        "GET / HTTP/1.1\r\nHost: google.com\r\nConnection: close\r\n\r\n";
    int exit_code = 1;

    memset(&context, 0, sizeof(context));
    context.socket_value = INVALID_SOCKET;
    if (WSAStartup(MAKEWORD(2, 2), &context.winsock_data) != 0) {
        puts("WINSOCK_RESULT=FAIL");
        return 1;
    }
    context.winsock_started = 1;
    puts("WINSOCK_RESULT=PASS");
    if (!papacc_proof_tcp_connect(&context)) goto cleanup;

    result = pst_win32_register_builtin_providers();
    if (result != PST_RESULT_OK) goto pst_failure;
    memset(&options, 0, sizeof(options));
    options.struct_size = (pst_u32)sizeof(options);
    options.api_version = PST_API_VERSION;
    pst_log_config_init(&log_config);
    log_config.level = PST_LOG_LEVEL_INFO;
    log_config.callback = papacc_proof_pst_log;
    pst_diagnostic_info_init(&diagnostic);
    result = pst_runtime_create_with_logging(&options, &log_config,
        &context.runtime, &diagnostic);
    if (result != PST_RESULT_OK) goto pst_failure;
    memset(&runtime_info, 0, sizeof(runtime_info));
    runtime_info.struct_size = (pst_u32)sizeof(runtime_info);
    runtime_info.api_version = PST_API_VERSION;
    result = pst_runtime_get_info(context.runtime, &runtime_info);
    if (result != PST_RESULT_OK || runtime_info.provider_count == 0U)
        goto pst_failure;
    printf("PST_RUNTIME_RESULT=PASS providers=%llu\n",
        (unsigned long long)runtime_info.provider_count);

    memset(&trust_source, 0, sizeof(trust_source));
    trust_source.struct_size = (pst_u32)sizeof(trust_source);
    trust_source.api_version = PST_API_VERSION;
    trust_source.kind = PST_TRUST_SOURCE_SYSTEM;
    result = pst_trust_create(&trust_source, &context.trust);
    if (result != PST_RESULT_OK) goto pst_failure;
    memset(&connection_config, 0, sizeof(connection_config));
    connection_config.struct_size = (pst_u32)sizeof(connection_config);
    connection_config.api_version = PST_API_VERSION;
    connection_config.role = PST_CONNECTION_ROLE_CLIENT;
    connection_config.provider_selection.struct_size =
        (pst_u32)sizeof(connection_config.provider_selection);
    connection_config.provider_selection.api_version = PST_API_VERSION;
    connection_config.provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    connection_config.provider_selection.exact_provider_id = "openssl";
    connection_config.provider_selection.required_capabilities =
        PST_CAP_TLS_1_3 | PST_CAP_ROLE_CLIENT | PST_CAP_PEER_CERT_AUTH |
        PST_CAP_SYSTEM_TRUST | PST_CAP_PEER_NAME_VERIFY | PST_CAP_PEER_INFO |
        PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT;
    connection_config.local_identity.struct_size =
        (pst_u32)sizeof(connection_config.local_identity);
    connection_config.local_identity.api_version = PST_API_VERSION;
    connection_config.peer_authentication.struct_size =
        (pst_u32)sizeof(connection_config.peer_authentication);
    connection_config.peer_authentication.api_version = PST_API_VERSION;
    connection_config.peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_REQUIRED;
    connection_config.peer_authentication.trust = context.trust;
    connection_config.peer_authentication.expected_peer_name = wrong_hostname ?
        PAPACC_PROOF_WRONG_HOST : PAPACC_PROOF_HOST;
    connection_config.peer_authentication.expected_peer_name_size = strlen(
        connection_config.peer_authentication.expected_peer_name);
    connection_config.tls.struct_size =
        (pst_u32)sizeof(connection_config.tls);
    connection_config.tls.api_version = PST_API_VERSION;
    connection_config.tls.minimum_version = PST_TLS_VERSION_1_3;
    connection_config.tls.maximum_version = PST_TLS_VERSION_1_3;
    connection_config.tls.resumption = PST_FEATURE_DISABLED;
    connection_config.tls.early_data = PST_FEATURE_DISABLED;
    connection_config.tls.require_graceful_shutdown = PST_FEATURE_DISABLED;
    connection_config.alpn.struct_size =
        (pst_u32)sizeof(connection_config.alpn);
    connection_config.alpn.api_version = PST_API_VERSION;
    connection_config.alpn.mode = PST_FEATURE_DISABLED;
    result = pst_connection_create_ex(context.runtime, &connection_config,
        &context.connection, &diagnostic);
    if (result != PST_RESULT_OK) goto pst_failure;
    memset(&provider_info, 0, sizeof(provider_info));
    provider_info.struct_size = (pst_u32)sizeof(provider_info);
    provider_info.api_version = PST_API_VERSION;
    result = pst_connection_get_provider_info(context.connection,
        &provider_info);
    if (result != PST_RESULT_OK) goto pst_failure;
    strncpy_s(context.provider_id, sizeof(context.provider_id),
        provider_info.provider_id, _TRUNCATE);
    printf("PST_PROVIDER_SELECTION=PASS role=CLIENT provider=%s\n",
        context.provider_id);
    result = pst_win32_socket_transport_create(
        (pst_size)context.socket_value, &context.transport);
    if (result != PST_RESULT_OK) goto pst_failure;
    context.socket_value = INVALID_SOCKET;
    {
        pst_u32 ownership_accepted = 0UL;
        result = pst_connection_attach(context.connection, context.transport,
            PST_OWNERSHIP_TRANSFERRED, &ownership_accepted);
        if (ownership_accepted != 0UL) context.transport = NULL;
        if (result != PST_RESULT_OK) goto pst_failure;
        if (ownership_accepted == 0UL) {
            result = PST_RESULT_INVALID_STATE;
            goto pst_failure;
        }
    }

    deadline = papacc_proof_now_ms() + PAPACC_PROOF_DEADLINE_MS;
    result = papacc_proof_handshake(context.connection, deadline);
    if (wrong_hostname) {
        if (result == PST_RESULT_PEER_NAME_MISMATCH ||
            result == PST_RESULT_AUTH_FAILURE) {
            printf("WRONG_HOSTNAME_RESULT=PASS result=%s\n",
                pst_result_string(result));
            puts("PLAINTEXT_PORT_ATTEMPTS=0");
            puts("NO_PLAINTEXT_FALLBACK=PASS");
            exit_code = 0;
            goto cleanup;
        }
        printf("WRONG_HOSTNAME_RESULT=FAIL result=%s\n",
            pst_result_string(result));
        goto cleanup;
    }
    if (result != PST_RESULT_OK) goto pst_failure;
    puts("TLS_HANDSHAKE_RESULT=PASS");

    result = pst_connection_get_peer_info(context.connection, &peer_info);
    if (result != PST_RESULT_OK) goto pst_failure;
    memset(&peer_summary, 0, sizeof(peer_summary));
    peer_summary.struct_size = (pst_u32)sizeof(peer_summary);
    peer_summary.api_version = PST_API_VERSION;
    result = pst_peer_info_get_summary(peer_info, &peer_summary);
    if (result != PST_RESULT_OK) goto pst_failure;
    printf("TLS_VERSION=%s public_value=%lu wire=0x0304\n",
        peer_summary.tls_version == PST_TLS_VERSION_1_3 ? "PASS" : "FAIL",
        (unsigned long)peer_summary.tls_version);
    printf("CERTIFICATE_PRESENT=%s\n",
        peer_summary.certificate_present == PST_KNOWN_TRUE ? "PASS" : "FAIL");
    printf("CHAIN_VALIDATED=%s\n",
        peer_summary.chain_validated == PST_KNOWN_TRUE ? "PASS" : "FAIL");
    printf("HOSTNAME_VALIDATED=%s\n",
        peer_summary.peer_name_validated == PST_KNOWN_TRUE ? "PASS" : "FAIL");
    printf("PEER_AUTHENTICATED=%s\n",
        peer_summary.peer_authenticated == PST_KNOWN_TRUE ? "PASS" : "FAIL");
    if (peer_summary.tls_version != PST_TLS_VERSION_1_3 ||
        peer_summary.certificate_present != PST_KNOWN_TRUE ||
        peer_summary.chain_validated != PST_KNOWN_TRUE ||
        peer_summary.peer_name_validated != PST_KNOWN_TRUE ||
        peer_summary.peer_authenticated != PST_KNOWN_TRUE) goto cleanup;
    pst_peer_info_release(peer_info);
    peer_info = NULL;

    result = papacc_proof_write_all(context.connection, request,
        sizeof(request) - 1U, deadline);
    if (result != PST_RESULT_OK) goto pst_failure;
    result = papacc_proof_read_response(&context, deadline);
    if (result == PST_RESULT_TRUNCATED)
        context.shutdown_close_kind = PST_CLOSE_TRUNCATED;
    else if (result != PST_RESULT_OK)
        goto pst_failure;
    if (strncmp(context.status_line, "HTTP/1.1 ", 9U) != 0 &&
        strncmp(context.status_line, "HTTP/1.0 ", 9U) != 0) {
        printf("HTTPS_RESPONSE=FAIL status=%s bytes=%lu\n",
            context.status_line, context.response_bytes);
        goto cleanup;
    }
    printf("HTTPS_RESPONSE=PASS status=%s bytes=%lu\n",
        context.status_line, context.response_bytes);
    if (context.shutdown_close_kind == PST_CLOSE_CLEAN) {
        puts("TLS_SHUTDOWN=PEER_CLEAN_CLOSE");
    } else if (context.shutdown_close_kind == PST_CLOSE_TRUNCATED) {
        puts("TLS_SHUTDOWN=PEER_TRUNCATED_CLOSE");
    } else {
        result = papacc_proof_shutdown(&context,
            papacc_proof_now_ms() + PAPACC_PROOF_DEADLINE_MS);
        if (result == PST_RESULT_OK) puts("TLS_SHUTDOWN=LOCAL_CLEAN_CLOSE");
        else if (result == PST_RESULT_TRUNCATED)
            puts("TLS_SHUTDOWN=LOCAL_TRUNCATED_CLOSE");
        else printf("TLS_SHUTDOWN=%s\n", pst_result_string(result));
    }
    puts("PLAINTEXT_PORT_ATTEMPTS=0");
    puts("NO_PLAINTEXT_FALLBACK=PASS");
    exit_code = 0;
    goto cleanup;

pst_failure:
    printf("PST_RESULT=FAIL result=%s\n", pst_result_string(result));
cleanup:
    if (peer_info != NULL) pst_peer_info_release(peer_info);
    papacc_proof_cleanup(&context);
    return exit_code;
}

int main(int argc, char **argv)
{
    int wrong_hostname = 0;
    if (argc == 2 && strcmp(argv[1], "--wrong-hostname") == 0)
        wrong_hostname = 1;
    else if (argc != 1) {
        fprintf(stderr, "Usage: %s [--wrong-hostname]\n", argv[0]);
        return 2;
    }
    printf("PROOF_DESTINATION=%s:%s\n", PAPACC_PROOF_HOST, PAPACC_PROOF_PORT);
    printf("EXPECTED_HOSTNAME=%s\n", wrong_hostname ?
        PAPACC_PROOF_WRONG_HOST : PAPACC_PROOF_HOST);
    return papacc_proof_run(wrong_hostname);
}
