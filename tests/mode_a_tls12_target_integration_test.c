#include "security_test_support.h"
#include "pst_provider_bootstrap_win32.h"
#include "papinho_secure_transport_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char request[] =
    "GET / HTTP/1.1\r\nHost: localhost\r\n"
    "Connection: close\r\n\r\n";

static int file_exists(const char *path)
{
    DWORD attributes = GetFileAttributesA(path);
    return attributes != INVALID_FILE_ATTRIBUTES &&
        (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0U;
}

static unsigned short reserve_port(void)
{
    SOCKET value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in address;
    int size = sizeof(address);
    unsigned short port = 0U;
    if (value == INVALID_SOCKET) return 0U;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(value, (const struct sockaddr *)&address, sizeof(address)) == 0 &&
        getsockname(value, (struct sockaddr *)&address, &size) == 0)
        port = ntohs(address.sin_port);
    closesocket(value);
    return port;
}

static SOCKET connect_loopback(unsigned short port)
{
    SOCKET value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in address;
    if (value == INVALID_SOCKET) return INVALID_SOCKET;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);
    if (connect(value, (const struct sockaddr *)&address, sizeof(address)) ==
            SOCKET_ERROR) {
        closesocket(value);
        return INVALID_SOCKET;
    }
    return value;
}

static int contains(const char *bytes, size_t size, const char *needle)
{
    size_t needle_size = strlen(needle), index;
    if (needle_size > size) return 0;
    for (index = 0U; index + needle_size <= size; ++index)
        if (memcmp(bytes + index, needle, needle_size) == 0) return 1;
    return 0;
}

static void client_config(PST_CONNECTION_CONFIG *config, pst_trust *trust,
    pst_u32 version, const char *name)
{
    memset(config, 0, sizeof(*config));
    config->struct_size = sizeof(*config);
    config->api_version = PST_API_VERSION;
    config->role = PST_CONNECTION_ROLE_CLIENT;
    config->provider_selection.struct_size = sizeof(config->provider_selection);
    config->provider_selection.api_version = PST_API_VERSION;
    config->provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    config->provider_selection.exact_provider_id = "openssl";
    config->provider_selection.required_capabilities =
        (version == PST_TLS_VERSION_1_2 ? PST_CAP_TLS_1_2 : PST_CAP_TLS_1_3) |
        PST_CAP_ROLE_CLIENT | PST_CAP_PEER_CERT_AUTH | PST_CAP_CUSTOM_TRUST |
        PST_CAP_PEER_NAME_VERIFY | PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT |
        PST_CAP_GRACEFUL_SHUTDOWN;
    config->local_identity.struct_size = sizeof(config->local_identity);
    config->local_identity.api_version = PST_API_VERSION;
    config->peer_authentication.struct_size =
        sizeof(config->peer_authentication);
    config->peer_authentication.api_version = PST_API_VERSION;
    config->peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_REQUIRED;
    config->peer_authentication.trust = trust;
    config->peer_authentication.expected_peer_name = name;
    config->peer_authentication.expected_peer_name_size = strlen(name);
    config->tls.struct_size = sizeof(config->tls);
    config->tls.api_version = PST_API_VERSION;
    config->tls.minimum_version = version;
    config->tls.maximum_version = version;
    config->tls.resumption = PST_FEATURE_DISABLED;
    config->tls.early_data = PST_FEATURE_DISABLED;
    config->tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    config->alpn.struct_size = sizeof(config->alpn);
    config->alpn.api_version = PST_API_VERSION;
    config->alpn.mode = PST_FEATURE_DISABLED;
    config->server_name_indication_mode = PST_SNI_MODE_COMPAT;
}

static int attempt(pst_runtime *runtime, pst_trust *trust,
    unsigned short port, pst_u32 version, const char *name, int expect_http)
{
    SOCKET socket_value = connect_loopback(port);
    pst_transport *transport = NULL;
    pst_connection *connection = NULL;
    PST_CONNECTION_CONFIG config;
    pst_u32 attached = 0U, operation = PST_OPERATION_NEED_READ;
    PST_RESULT source = PST_RESULT_OK;
    char response_bytes[4096];
    size_t used = 0U, offset = 0U;
    u_long nonblocking = 1UL;
    int index, handshake_complete = 0, http_ok = 0;
    if (socket_value == INVALID_SOCKET) return 0;
    client_config(&config, trust, version, name);
    if (ioctlsocket(socket_value, FIONBIO, &nonblocking) == SOCKET_ERROR ||
        pst_connection_create(runtime, &config, &connection) != PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)socket_value, &transport) !=
            PST_RESULT_OK ||
        pst_connection_attach(connection, transport, PST_OWNERSHIP_TRANSFERRED,
            &attached) != PST_RESULT_OK || attached == 0U) goto done;
    socket_value = INVALID_SOCKET;
    transport = NULL;
    for (index = 0; index < 5000; ++index) {
        PST_WAIT_RESULT wait_result;
        if (pst_connection_handshake(connection, &operation, &source) !=
                PST_RESULT_OK || operation == PST_OPERATION_FAILED ||
            operation == PST_OPERATION_CLOSED) break;
        if (operation == PST_OPERATION_COMPLETE) {
            handshake_complete = 1;
            break;
        }
        if (pst_connection_wait(connection, 1000U, &wait_result) !=
                PST_RESULT_OK) break;
    }
    if (!expect_http) {
        http_ok = !handshake_complete;
        goto done;
    }
    if (!handshake_complete) goto done;
    while (offset < sizeof(request) - 1U) {
        PST_IO_RESULT io;
        PST_WAIT_RESULT wait_result;
        if (pst_connection_write(connection, (const pst_u8 *)request + offset,
                sizeof(request) - 1U - offset, &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) goto done;
        offset += io.bytes_transferred;
        if (offset < sizeof(request) - 1U &&
            pst_connection_wait(connection, 1000U, &wait_result) !=
                PST_RESULT_OK) goto done;
    }
    while (used < sizeof(response_bytes)) {
        PST_IO_RESULT io;
        PST_WAIT_RESULT wait_result;
        if (pst_connection_read(connection, (pst_u8 *)response_bytes + used,
                sizeof(response_bytes) - used, &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED) goto done;
        used += io.bytes_transferred;
        if (contains(response_bytes, used, "papinho-mode-a-tls12-ok\n")) break;
        if (io.operation == PST_OPERATION_CLOSED) break;
        if (pst_connection_wait(connection, 1000U, &wait_result) !=
                PST_RESULT_OK) goto done;
    }
    http_ok = contains(response_bytes, used, "HTTP/1.1 200 OK") &&
        contains(response_bytes, used, "papinho-mode-a-tls12-ok\n");
done:
    if (connection != NULL) pst_connection_release(connection);
    if (transport != NULL) pst_transport_release(transport);
    if (socket_value != INVALID_SOCKET) closesocket(socket_value);
    return http_ok;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    PAPACC_TEST_BLOB ca = { NULL, 0U };
    PST_DER_ITEM anchor;
    PST_TRUST_SOURCE trust_source;
    PST_RUNTIME_OPTIONS options;
    pst_trust *trust = NULL;
    pst_runtime *runtime = NULL;
    PROCESS_INFORMATION process;
    STARTUPINFOA startup;
    HANDLE stop = NULL;
    char command[4096], ready[MAX_PATH * 2], temp[MAX_PATH];
    const char *stop_name = "Local\\PapaccModeATls12IntegrationStop";
    unsigned short port;
    DWORD attempt_index, exit_code = 1U;
    int index, result = 1, stage = 0;
    if (argc != 2 || WSAStartup(MAKEWORD(2, 2), &winsock) != 0) return 2;
    port = reserve_port();
    if (port == 0U || GetTempPathA(sizeof(temp), temp) == 0U ||
        sprintf_s(ready, sizeof(ready), "%spapacc-mode-a-tls12.ready", temp) <
            0) { stage = 1; goto done; }
    DeleteFileA(ready);
    stop = CreateEventA(NULL, TRUE, FALSE, stop_name);
    if (stop == NULL) { stage = 2; goto done; }
    memset(&startup, 0, sizeof(startup));
    memset(&process, 0, sizeof(process));
    startup.cb = sizeof(startup);
    if (sprintf_s(command, sizeof(command),
            "\"%s\" --port %u --ready-file \"%s\" --stop-event \"%s\"",
            argv[1], (unsigned int)port, ready, stop_name) < 0 ||
        !CreateProcessA(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW,
            NULL, NULL, &startup, &process)) { stage = 3; goto done; }
    for (attempt_index = 0U; attempt_index < 200U && !file_exists(ready);
            ++attempt_index) Sleep(25U);
    if (!file_exists(ready) ||
        papacc_pst_provider_bootstrap_win32(NULL) != PAPACC_RESULT_OK ||
        !papacc_test_security_load_blob("mode-a-ca.der", &ca)) {
        stage = 4; goto stop_process;
    }
    anchor.data = ca.data;
    anchor.size = ca.size;
    memset(&trust_source, 0, sizeof(trust_source));
    trust_source.struct_size = sizeof(trust_source);
    trust_source.api_version = PST_API_VERSION;
    trust_source.kind = PST_TRUST_SOURCE_CUSTOM_CA_DER;
    trust_source.anchors = &anchor;
    trust_source.anchor_count = 1U;
    if (pst_trust_create(&trust_source, &trust) != PST_RESULT_OK) {
        stage = 5; goto stop_process;
    }
    memset(&options, 0, sizeof(options));
    options.struct_size = sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK) {
        stage = 6; goto stop_process;
    }
    for (index = 0; index < 3; ++index)
        if (!attempt(runtime, trust, port, PST_TLS_VERSION_1_2,
                "localhost", 1)) {
            stage = 10 + index; goto stop_process;
        }
    if (!attempt(runtime, trust, port, PST_TLS_VERSION_1_3,
            "localhost", 0)) { stage = 20; goto stop_process; }
    if (!attempt(runtime, trust, port, PST_TLS_VERSION_1_2,
            "wrong.example", 0)) { stage = 21; goto stop_process; }
    if (!SetEvent(stop) ||
        WaitForSingleObject(process.hProcess, 15000U) != WAIT_OBJECT_0 ||
        !GetExitCodeProcess(process.hProcess, &exit_code) || exit_code != 0U) {
        stage = 30; goto stop_process;
    }
    result = 0;
    goto done_process;
stop_process:
    SetEvent(stop);
    if (process.hProcess != NULL &&
        WaitForSingleObject(process.hProcess, 15000U) != WAIT_OBJECT_0)
        TerminateProcess(process.hProcess, 99U);
done_process:
    if (process.hThread != NULL) CloseHandle(process.hThread);
    if (process.hProcess != NULL) CloseHandle(process.hProcess);
done:
    if (runtime != NULL) pst_runtime_release(runtime);
    if (trust != NULL) pst_trust_release(trust);
    free(ca.data);
    if (stop != NULL) CloseHandle(stop);
    DeleteFileA(ready);
    WSACleanup();
    if (result == 0)
        puts("TLS12_REFERENCE=PASS\nTLS13_REFERENCE=REJECTED\n"
            "TLS12_THREE_CONNECTIONS=PASS\nMODE_A_HOSTNAME_MATCH=PASS\n"
            "MODE_A_HOSTNAME_MISMATCH_REJECTED=YES\n"
            "MODE_A_TARGET_CLEAN_STOP=PASS");
    else fprintf(stderr, "MODE_A_TLS12_TARGET_FAILURE stage=%d exit=%lu\n",
        stage, (unsigned long)exit_code);
    return result;
}
