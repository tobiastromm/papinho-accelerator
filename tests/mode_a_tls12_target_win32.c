#include "security_test_support.h"
#include "pst_provider_bootstrap_win32.h"
#include "papinho_secure_transport_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODE_A_DEFAULT_PORT 19443U
#define MODE_A_HOSTNAME "localhost"

static const char response[] =
    "HTTP/1.1 200 OK\r\nContent-Length: 24\r\nConnection: close\r\n\r\n"
    "papinho-mode-a-tls12-ok\n";

static int parse_port(const char *text, unsigned short *port)
{
    char *end = NULL;
    unsigned long value;
    if (text == NULL || port == NULL || text[0] == '\0') return 0;
    value = strtoul(text, &end, 10);
    if (end == text || *end != '\0' || value == 0UL || value > 65535UL)
        return 0;
    *port = (unsigned short)value;
    return 1;
}

static int has_headers(const unsigned char *bytes, size_t size)
{
    size_t index;
    for (index = 3U; index < size; ++index)
        if (bytes[index - 3U] == '\r' && bytes[index - 2U] == '\n' &&
            bytes[index - 1U] == '\r' && bytes[index] == '\n') return 1;
    return 0;
}

static void server_config(PST_CONNECTION_CONFIG *config,
    pst_credentials *credentials)
{
    memset(config, 0, sizeof(*config));
    config->struct_size = sizeof(*config);
    config->api_version = PST_API_VERSION;
    config->role = PST_CONNECTION_ROLE_SERVER;
    config->provider_selection.struct_size = sizeof(config->provider_selection);
    config->provider_selection.api_version = PST_API_VERSION;
    config->provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    config->provider_selection.exact_provider_id = "openssl";
    config->provider_selection.required_capabilities = PST_CAP_TLS_1_2 |
        PST_CAP_ROLE_SERVER | PST_CAP_LOCAL_IDENTITY | PST_CAP_NONBLOCKING |
        PST_CAP_BACKEND_WAIT | PST_CAP_GRACEFUL_SHUTDOWN;
    config->local_identity.struct_size = sizeof(config->local_identity);
    config->local_identity.api_version = PST_API_VERSION;
    config->local_identity.credentials = credentials;
    config->peer_authentication.struct_size =
        sizeof(config->peer_authentication);
    config->peer_authentication.api_version = PST_API_VERSION;
    config->peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_DISABLED;
    config->tls.struct_size = sizeof(config->tls);
    config->tls.api_version = PST_API_VERSION;
    config->tls.minimum_version = PST_TLS_VERSION_1_2;
    config->tls.maximum_version = PST_TLS_VERSION_1_2;
    config->tls.resumption = PST_FEATURE_DISABLED;
    config->tls.early_data = PST_FEATURE_DISABLED;
    config->tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    config->alpn.struct_size = sizeof(config->alpn);
    config->alpn.api_version = PST_API_VERSION;
    config->alpn.mode = PST_FEATURE_DISABLED;
    config->server_name_indication_mode = PST_SNI_MODE_COMPAT;
}

static int serve(pst_runtime *runtime, pst_credentials *credentials,
    SOCKET accepted)
{
    PST_CONNECTION_CONFIG config;
    pst_connection *connection = NULL;
    pst_transport *transport = NULL;
    pst_u32 attached = 0U, operation = PST_OPERATION_NEED_READ;
    PST_RESULT source = PST_RESULT_OK;
    unsigned char request[8192];
    size_t used = 0U, offset = 0U;
    u_long nonblocking = 1UL;
    int index, success = 0;
    server_config(&config, credentials);
    if (ioctlsocket(accepted, FIONBIO, &nonblocking) == SOCKET_ERROR ||
        pst_connection_create(runtime, &config, &connection) != PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)accepted, &transport) !=
            PST_RESULT_OK ||
        pst_connection_attach(connection, transport, PST_OWNERSHIP_TRANSFERRED,
            &attached) != PST_RESULT_OK || attached == 0U) goto done;
    accepted = INVALID_SOCKET;
    transport = NULL;
    for (index = 0; index < 5000; ++index) {
        PST_WAIT_RESULT wait_result;
        if (pst_connection_handshake(connection, &operation, &source) !=
                PST_RESULT_OK || operation == PST_OPERATION_FAILED ||
            operation == PST_OPERATION_CLOSED) goto done;
        if (operation == PST_OPERATION_COMPLETE) break;
        if (pst_connection_wait(connection, 1000U, &wait_result) !=
                PST_RESULT_OK) goto done;
    }
    if (operation != PST_OPERATION_COMPLETE) goto done;
    while (used < sizeof(request) && !has_headers(request, used)) {
        PST_IO_RESULT io;
        PST_WAIT_RESULT wait_result;
        if (pst_connection_read(connection, request + used,
                sizeof(request) - used, &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) goto done;
        used += io.bytes_transferred;
        if (!has_headers(request, used) &&
            pst_connection_wait(connection, 1000U, &wait_result) !=
                PST_RESULT_OK) goto done;
    }
    if (used < 4U || memcmp(request, "GET ", 4U) != 0) goto done;
    while (offset < sizeof(response) - 1U) {
        PST_IO_RESULT io;
        PST_WAIT_RESULT wait_result;
        if (pst_connection_write(connection,
                (const pst_u8 *)response + offset,
                sizeof(response) - 1U - offset, &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) goto done;
        offset += io.bytes_transferred;
        if (offset < sizeof(response) - 1U &&
            pst_connection_wait(connection, 1000U, &wait_result) !=
                PST_RESULT_OK) goto done;
    }
    success = 1;
    operation = PST_OPERATION_NEED_READ;
    for (index = 0; index < 100; ++index) {
        PST_WAIT_RESULT wait_result;
        if (pst_connection_shutdown(connection, &operation, &source) !=
                PST_RESULT_OK || operation == PST_OPERATION_FAILED) break;
        if (operation == PST_OPERATION_COMPLETE ||
            operation == PST_OPERATION_CLOSED) break;
        if (pst_connection_wait(connection, 100U, &wait_result) !=
                PST_RESULT_OK) break;
    }
done:
    if (connection != NULL) pst_connection_release(connection);
    if (transport != NULL) pst_transport_release(transport);
    if (accepted != INVALID_SOCKET) closesocket(accepted);
    return success;
}

static int publish_ready(const char *path, unsigned short port,
    const char *stop_event)
{
    FILE *file = NULL;
    if (path == NULL) return 1;
    if (fopen_s(&file, path, "wb") != 0 || file == NULL) return 0;
    fprintf(file, "hostname=%s\nport=%u\ntls_min=1.2\ntls_max=1.2\n"
        "http_body=papinho-mode-a-tls12-ok\nstop_event=%s\n",
        MODE_A_HOSTNAME, (unsigned int)port, stop_event);
    fclose(file);
    return 1;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    PAPACC_TEST_IDENTITY identity = { { NULL, 0U }, { NULL, 0U }, NULL };
    SOCKET listener = INVALID_SOCKET;
    struct sockaddr_in address;
    HANDLE stop = NULL;
    const char *ready_path = NULL;
    const char *stop_name = "Local\\PapaccModeATls12TargetStop";
    unsigned short port = MODE_A_DEFAULT_PORT;
    u_long nonblocking = 1UL;
    int index, result = 1;
    for (index = 1; index < argc; ++index) {
        if (strcmp(argv[index], "--port") == 0 && index + 1 < argc) {
            if (!parse_port(argv[++index], &port)) return 2;
        } else if (strcmp(argv[index], "--ready-file") == 0 &&
                index + 1 < argc) ready_path = argv[++index];
        else if (strcmp(argv[index], "--stop-event") == 0 &&
                index + 1 < argc) stop_name = argv[++index];
        else {
            fprintf(stderr, "usage: %s [--port <1..65535>] "
                "[--ready-file <path>] [--stop-event <name>]\n", argv[0]);
            return 2;
        }
    }
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0 ||
        papacc_pst_provider_bootstrap_win32(NULL) != PAPACC_RESULT_OK ||
        !papacc_test_security_load_identity("mode-a-server.der",
            "mode-a-server.pk8", &identity)) goto done;
    memset(&options, 0, sizeof(options));
    options.struct_size = sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK) goto done;
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == INVALID_SOCKET) goto done;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);
    if (bind(listener, (const struct sockaddr *)&address, sizeof(address)) ==
            SOCKET_ERROR || listen(listener, SOMAXCONN) == SOCKET_ERROR ||
        ioctlsocket(listener, FIONBIO, &nonblocking) == SOCKET_ERROR) goto done;
    stop = CreateEventA(NULL, TRUE, FALSE, stop_name);
    if (stop == NULL || !publish_ready(ready_path, port, stop_name)) goto done;
    printf("MODE_A_TLS12_TARGET_READY hostname=%s port=%u tls=1.2-only\n",
        MODE_A_HOSTNAME, (unsigned int)port);
    fflush(stdout);
    while (WaitForSingleObject(stop, 0U) == WAIT_TIMEOUT) {
        SOCKET accepted = accept(listener, NULL, NULL);
        if (accepted == INVALID_SOCKET) {
            if (WSAGetLastError() != WSAEWOULDBLOCK) goto done;
            Sleep(1U);
            continue;
        }
        (void)serve(runtime, identity.credentials, accepted);
    }
    result = 0;
done:
    if (ready_path != NULL) DeleteFileA(ready_path);
    if (stop != NULL) CloseHandle(stop);
    if (listener != INVALID_SOCKET) closesocket(listener);
    if (runtime != NULL) pst_runtime_release(runtime);
    papacc_test_security_identity_release(&identity);
    WSACleanup();
    return result;
}
