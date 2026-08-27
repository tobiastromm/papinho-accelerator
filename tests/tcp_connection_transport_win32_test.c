#include "connection.h"
#include "tcp_connection_transport_win32.h"

#include <ws2tcpip.h>

static PAPACC_RESULT papacc_test_listener_port(
    const PAPACC_TCP_SOCKET_WIN32 *listener,
    PAPACC_U16 *out_port)
{
    struct sockaddr_in address;
    int address_length = (int)sizeof(address);
    if (getsockname(listener->native_socket, (struct sockaddr *)&address,
                    &address_length) == SOCKET_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    *out_port = (PAPACC_U16)ntohs(address.sin_port);
    return PAPACC_RESULT_OK;
}

static SOCKET papacc_test_connect(PAPACC_U16 port)
{
    struct sockaddr_in address;
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(client, (const struct sockaddr *)&address,
                (int)sizeof(address)) == SOCKET_ERROR) {
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    return client;
}

int main(void)
{
    PAPACC_TCP_PLATFORM platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_SOCKET_WIN32 listener = PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted =
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT context =
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage[1];
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local_endpoint;
    PAPACC_NETWORK_ENDPOINT remote_endpoint;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_U16 port = 0;
    SOCKET client = INVALID_SOCKET;
    SOCKET accepted_socket;
    int result = 0;

    if (papacc_tcp_platform_init(&platform) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(&platform, &target, 0, &listener) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&listener) != PAPACC_RESULT_OK ||
        papacc_test_listener_port(&listener, &port) != PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, storage, 1) !=
            PAPACC_RESULT_OK) {
        result = 1;
        goto cleanup;
    }
    client = papacc_test_connect(port);
    if (client == INVALID_SOCKET ||
        papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_OK) {
        result = 2;
        goto cleanup;
    }
    local_endpoint = accepted.local_endpoint;
    remote_endpoint = accepted.remote_endpoint;
    accepted_socket = accepted.native_socket;
    context.native_socket = (SOCKET)0;
    context.owns_socket = PAPACC_TRUE;
    if (papacc_tcp_connection_transport_win32_move(
            &accepted, &context, &transport) != PAPACC_RESULT_INVALID_STATE ||
        accepted.native_socket != accepted_socket ||
        accepted.is_open != PAPACC_TRUE || transport.context != NULL) {
        result = 3;
        goto cleanup;
    }
    context = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    if (papacc_tcp_connection_transport_win32_move(
            &accepted, &context, &transport) != PAPACC_RESULT_OK ||
        accepted.is_open != PAPACC_FALSE ||
        accepted.native_socket != INVALID_SOCKET ||
        context.owns_socket != PAPACC_TRUE ||
        papacc_connection_manager_publish(
            &manager, &transport, &local_endpoint, &remote_endpoint,
            &connection) != PAPACC_RESULT_OK ||
        connection == NULL ||
        connection->state != PAPACC_CONNECTION_STATE_PENDING ||
        papacc_network_endpoint_equal(
            &connection->local_endpoint, &local_endpoint) != PAPACC_TRUE ||
        papacc_network_endpoint_equal(
            &connection->remote_endpoint, &remote_endpoint) != PAPACC_TRUE) {
        result = 4;
        goto cleanup;
    }
    (void)closesocket(client);
    client = INVALID_SOCKET;
    if (papacc_connection_manager_remove(
            &manager, connection->connection_instance_id) != PAPACC_RESULT_OK ||
        context.owns_socket != PAPACC_FALSE ||
        listener.is_listening != PAPACC_TRUE) {
        result = 5;
        goto cleanup;
    }
    client = papacc_test_connect(port);
    if (client == INVALID_SOCKET ||
        papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_OK ||
        listener.is_listening != PAPACC_TRUE) {
        result = 6;
    }

cleanup:
    papacc_connection_manager_shutdown(&manager);
    papacc_transport_connection_close(&transport);
    papacc_tcp_accepted_socket_win32_close(&accepted);
    if (client != INVALID_SOCKET) {
        (void)closesocket(client);
    }
    papacc_tcp_socket_win32_close(&listener);
    papacc_tcp_platform_shutdown(&platform);
    return result;
}
