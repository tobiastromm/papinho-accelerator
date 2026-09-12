#include "security_test_support.h"
#include "papinho_secure_transport_win32.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RC_LIMIT 1000U
#define RC_NSS_CLIENT_CAPS 0x00047ab7UL

static const unsigned char control_open[20] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,1,0,0,0,0,0,4,0,1,0,0 };
static const unsigned char control_accept[20] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,2,0,0,0,0,0,4,0,1,0,0 };
static const unsigned char ticket_request[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,3,0,0,0,0,0,0 };
static const unsigned char data_accept[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,6,0,0,0,0,0,0 };

typedef struct RC_CONNECTION {
    pst_connection *connection;
} RC_CONNECTION;

static int rc_wait(pst_connection *connection)
{
    PST_WAIT_RESULT wait_result;
    return pst_connection_wait(connection, 1000U, &wait_result) ==
        PST_RESULT_OK && wait_result.timed_out == 0U;
}

static int rc_wait_shutdown(pst_connection *connection)
{
    PST_WAIT_RESULT wait_result;
    return pst_connection_wait(connection, 5000U, &wait_result) ==
        PST_RESULT_OK && wait_result.timed_out == 0U;
}

static int rc_connect_socket(unsigned short port, SOCKET *out_socket)
{
    struct sockaddr_in address;
    u_long nonblocking = 1UL;
    SOCKET value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (value == INVALID_SOCKET) return 0;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(value, (const struct sockaddr *)&address, sizeof(address)) ==
            SOCKET_ERROR || ioctlsocket(value, FIONBIO, &nonblocking) ==
            SOCKET_ERROR) {
        closesocket(value);
        return 0;
    }
    *out_socket = value;
    return 1;
}

static int rc_open_config(pst_runtime *runtime, unsigned short port,
    const PST_CONNECTION_CONFIG *config, RC_CONNECTION *out)
{
    pst_transport *transport = NULL;
    SOCKET socket_value = INVALID_SOCKET;
    pst_u32 accepted = 0U;
    pst_u32 operation = 0U;
    PST_RESULT source_error = PST_RESULT_OK;
    PST_PEER_INFO_SUMMARY summary;
    pst_peer_info *peer = NULL;
    unsigned char alpn[16];
    pst_size alpn_size = 0U;
    unsigned int index;
    if (!rc_connect_socket(port, &socket_value) ||
        pst_connection_create(runtime, config, &out->connection) !=
            PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)socket_value,
            &transport) != PST_RESULT_OK ||
        pst_connection_attach(out->connection, transport,
            PST_OWNERSHIP_TRANSFERRED, &accepted) != PST_RESULT_OK ||
        accepted == 0U) goto fail;
    transport = NULL;
    socket_value = INVALID_SOCKET;
    for (index = 0U; index < RC_LIMIT; ++index) {
        if (pst_connection_handshake(out->connection, &operation,
                &source_error) != PST_RESULT_OK) goto fail;
        if (operation == PST_OPERATION_COMPLETE) break;
        if (operation == PST_OPERATION_CLOSED ||
            operation == PST_OPERATION_FAILED || !rc_wait(out->connection))
            goto fail;
    }
    if (operation != PST_OPERATION_COMPLETE) goto fail;
    memset(&summary, 0, sizeof(summary));
    summary.struct_size = sizeof(summary);
    summary.api_version = PST_API_VERSION;
    if (pst_connection_get_peer_info(out->connection, &peer) != PST_RESULT_OK ||
        pst_peer_info_get_summary(peer, &summary) != PST_RESULT_OK ||
        summary.tls_version != PST_TLS_VERSION_1_3 ||
        summary.peer_authenticated == 0U || summary.chain_validated == 0U ||
        pst_connection_get_negotiated_alpn(out->connection, alpn,
            sizeof(alpn), &alpn_size) != PST_RESULT_OK || alpn_size != 8U ||
        memcmp(alpn, "papacc/1", 8U) != 0) goto fail;
    pst_peer_info_release(peer);
    return 1;
fail:
    if (peer != NULL) pst_peer_info_release(peer);
    if (transport != NULL) pst_transport_release(transport);
    else if (socket_value != INVALID_SOCKET) closesocket(socket_value);
    if (out->connection != NULL) pst_connection_release(out->connection);
    out->connection = NULL;
    return 0;
}

static int rc_open(pst_runtime *runtime, unsigned short port,
    pst_credentials *credentials, pst_trust *trust, RC_CONNECTION *out)
{
    PST_CONNECTION_CONFIG config;
    papacc_test_pst_client_config(&config, credentials, trust);
    return rc_open_config(runtime, port, &config, out);
}

static int rc_write_all(RC_CONNECTION *connection,
    const unsigned char *bytes, size_t size)
{
    size_t offset = 0U;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT && offset < size; ++index) {
        PST_IO_RESULT io;
        if (pst_connection_write(connection->connection, bytes + offset,
                (pst_size)(size - offset), &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) return 0;
        offset += (size_t)io.bytes_transferred;
        if (offset < size && !rc_wait(connection->connection)) return 0;
    }
    return offset == size;
}

static int rc_read_exact(RC_CONNECTION *connection, unsigned char *bytes,
    size_t size)
{
    size_t offset = 0U;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT && offset < size; ++index) {
        PST_IO_RESULT io;
        if (pst_connection_read(connection->connection, bytes + offset,
                (pst_size)(size - offset), &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) return 0;
        offset += (size_t)io.bytes_transferred;
        if (offset < size && !rc_wait(connection->connection)) return 0;
    }
    return offset == size;
}

static int rc_expect_close(RC_CONNECTION *connection)
{
    unsigned char byte;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT; ++index) {
        PST_IO_RESULT io;
        PST_RESULT result = pst_connection_read(
            connection->connection, &byte, 1U, &io);
        if (result != PST_RESULT_OK || io.operation == PST_OPERATION_CLOSED ||
            io.operation == PST_OPERATION_FAILED) return 1;
        if (!rc_wait(connection->connection)) return 0;
    }
    return 0;
}

static int rc_accept_clean_close(RC_CONNECTION *connection)
{
    unsigned char byte;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT; ++index) {
        PST_IO_RESULT io;
        if (pst_connection_read(connection->connection, &byte, 1U, &io) !=
                PST_RESULT_OK) return 0;
        if (io.operation == PST_OPERATION_CLOSED) {
            int clean = io.close_kind == PST_CLOSE_CLEAN;
            pst_connection_release(connection->connection);
            connection->connection = NULL;
            return clean;
        }
        if (io.operation == PST_OPERATION_FAILED ||
            !rc_wait_shutdown(connection->connection)) return 0;
    }
    return 0;
}

static int rc_close(RC_CONNECTION *connection)
{
    pst_u32 operation = 0U;
    PST_RESULT source_error = PST_RESULT_OK;
    unsigned int index;
    if (connection->connection == NULL) return 1;
    for (index = 0U; index < RC_LIMIT; ++index) {
        PST_RESULT shutdown_result = pst_connection_shutdown(
            connection->connection, &operation, &source_error);
        if (shutdown_result != PST_RESULT_OK) {
            fprintf(stderr, "shutdown result=%lu operation=%lu source=%lu\n",
                (unsigned long)shutdown_result, (unsigned long)operation,
                (unsigned long)source_error);
            break;
        }
        if (operation == PST_OPERATION_COMPLETE ||
            operation == PST_OPERATION_CLOSED) break;
        if (operation == PST_OPERATION_FAILED ||
            !rc_wait_shutdown(connection->connection)) {
            fprintf(stderr, "shutdown step operation=%lu source=%lu\n",
                (unsigned long)operation, (unsigned long)source_error);
            break;
        }
    }
    pst_connection_release(connection->connection);
    connection->connection = NULL;
    return operation == PST_OPERATION_COMPLETE ||
        operation == PST_OPERATION_CLOSED;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture =
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    RC_CONNECTION control = { NULL }, data = { NULL }, replay = { NULL };
    RC_CONNECTION foreign = { NULL }, rightful = { NULL };
    unsigned char response[32];
    unsigned char attach[32] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
    unsigned short port;
    HANDLE shutdown_event = NULL;
    HANDLE shutdown_ack_event = NULL;
    int result = 1;
    int stage = 0;
    const char *scenario;
    if (argc != 5) return 2;
    port = (unsigned short)strtoul(argv[1], NULL, 10);
    scenario = argv[4];
    shutdown_event = OpenEventA(EVENT_MODIFY_STATE, FALSE, argv[2]);
    shutdown_ack_event = OpenEventA(SYNCHRONIZE, FALSE, argv[3]);
    if (port == 0U || shutdown_event == NULL || shutdown_ack_event == NULL ||
        WSAStartup(MAKEWORD(2, 2), &winsock) != 0) return 3;
    if ((papacc_test_pst_client_capabilities() & ~RC_NSS_CLIENT_CAPS) != 0U ||
        (papacc_test_pst_client_capabilities() & PST_CAP_SNI_CONTROL) != 0U ||
        !papacc_test_security_fixture_init(&fixture) ||
        pst_win32_register_builtin_providers() != PST_RESULT_OK) {
        stage = 1; goto cleanup;
    }
    memset(&options, 0, sizeof(options));
    options.struct_size = sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK) {
        stage = 2; goto cleanup;
    }
    if (strcmp(scenario, "tls12") == 0 ||
        strcmp(scenario, "wrong-alpn") == 0 ||
        strcmp(scenario, "missing-credential") == 0) {
        PST_CONNECTION_CONFIG negative_config;
        static const unsigned char wrong_bytes[] = "wrong/1";
        static const PST_ALPN_PROTOCOL wrong = {
            wrong_bytes, sizeof(wrong_bytes) - 1U };
        papacc_test_pst_client_config(&negative_config,
            fixture.client_a.credentials, fixture.trust);
        if (strcmp(scenario, "tls12") == 0) {
            negative_config.tls.minimum_version = PST_TLS_VERSION_1_2;
            negative_config.tls.maximum_version = PST_TLS_VERSION_1_2;
        } else if (strcmp(scenario, "wrong-alpn") == 0) {
            negative_config.alpn.protocols = &wrong;
        } else {
            negative_config.local_identity.credentials = NULL;
        }
        if (rc_open_config(runtime, port, &negative_config, &control)) {
            if (!rc_expect_close(&control)) { stage = 14; goto cleanup; }
            pst_connection_release(control.connection);
            control.connection = NULL;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "not-enrolled") == 0 ||
        strcmp(scenario, "deny") == 0) {
        pst_credentials *identity = strcmp(scenario, "deny") == 0 ?
            fixture.client_b.credentials : fixture.client_c.credentials;
        if (!rc_open(runtime, port, identity, fixture.trust, &control) ||
            !rc_write_all(&control, control_open, sizeof(control_open)) ||
            !rc_expect_close(&control)) { stage = 15; goto cleanup; }
        pst_connection_release(control.connection);
        control.connection = NULL;
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "main") != 0) { stage = 16; goto cleanup; }
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &control) || !rc_write_all(&control, control_open,
            sizeof(control_open)) || !rc_read_exact(&control, response, 20U) ||
        memcmp(response, control_accept, 20U) != 0 ||
        !rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U) || response[9] != 4U) {
        stage = 3; goto cleanup;
    }
    memcpy(attach + 16U, response + 16U, 16U);
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &data) || !rc_write_all(&data, attach, sizeof(attach)) ||
        !rc_read_exact(&data, response, 16U) ||
        memcmp(response, data_accept, 16U) != 0) {
        stage = 4; goto cleanup;
    }
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &replay) || !rc_write_all(&replay, attach, sizeof(attach)) ||
        !rc_expect_close(&replay)) { stage = 5; goto cleanup; }
    pst_connection_release(replay.connection); replay.connection = NULL;
    if (!rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U)) { stage = 6; goto cleanup; }
    memcpy(attach + 16U, response + 16U, 16U);
    if (!rc_open(runtime, port, fixture.client_b.credentials, fixture.trust,
            &foreign) || !rc_write_all(&foreign, attach, sizeof(attach)) ||
        !rc_expect_close(&foreign)) { stage = 7; goto cleanup; }
    pst_connection_release(foreign.connection); foreign.connection = NULL;
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &rightful) || !rc_write_all(&rightful, attach, sizeof(attach)) ||
        !rc_read_exact(&rightful, response, 16U) ||
        memcmp(response, data_accept, 16U) != 0) {
        stage = 8; goto cleanup;
    }
    if (!SetEvent(shutdown_event) ||
        WaitForSingleObject(shutdown_ack_event, 5000U) != WAIT_OBJECT_0) {
        stage = 13; goto cleanup;
    }
    if (!rc_close(&rightful)) { stage = 10; goto cleanup; }
    if (!rc_close(&data)) { stage = 11; goto cleanup; }
    if (!rc_accept_clean_close(&control)) { stage = 12; goto cleanup; }
    puts("REFERENCE_CLIENT_CONTROL_INTEROP=PASS");
    puts("REFERENCE_CLIENT_DATA_INTEROP=PASS");
    puts("REFERENCE_CLIENT_TICKET_REPLAY_REJECTED=YES");
    puts("REFERENCE_CLIENT_FOREIGN_CREDENTIAL_REJECTED=YES");
    puts("REFERENCE_CLIENT_RIGHTFUL_RETRY_SAME_TICKET=YES");
    puts("REFERENCE_CLIENT_PROFILE_NT4_NSS_COMPATIBLE=YES");
    result = 0;
cleanup:
    if (result != 0) fprintf(stderr, "REFERENCE_CLIENT_FAIL_STAGE=%d\n", stage);
    if (rightful.connection != NULL) pst_connection_release(rightful.connection);
    if (foreign.connection != NULL) pst_connection_release(foreign.connection);
    if (replay.connection != NULL) pst_connection_release(replay.connection);
    if (data.connection != NULL) pst_connection_release(data.connection);
    if (control.connection != NULL) pst_connection_release(control.connection);
    if (runtime != NULL) pst_runtime_release(runtime);
    papacc_test_security_fixture_release(&fixture);
    if (shutdown_event != NULL) CloseHandle(shutdown_event);
    if (shutdown_ack_event != NULL) CloseHandle(shutdown_ack_event);
    WSACleanup();
    return result == 0 ? 0 : (stage == 0 ? 1 : stage);
}
