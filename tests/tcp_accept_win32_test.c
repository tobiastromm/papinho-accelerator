#include "tcp_accept_win32.h"

#include <ws2tcpip.h>

static PAPACC_BOOL papacc_test_accepted_is_empty(
    const PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *accepted)
{
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;

    return (accepted->native_socket == INVALID_SOCKET &&
            accepted->is_open == PAPACC_FALSE &&
            accepted->family == PAPACC_IP_FAMILY_UNSPECIFIED &&
            papacc_network_endpoint_equal(
                &accepted->local_endpoint, &endpoint) == PAPACC_TRUE &&
            papacc_network_endpoint_equal(
                &accepted->remote_endpoint, &endpoint) == PAPACC_TRUE)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

static PAPACC_RESULT papacc_test_listener_port(
    const PAPACC_TCP_SOCKET_WIN32 *listener,
    PAPACC_U16 *out_port)
{
    struct sockaddr_storage address;
    int address_length = (int)sizeof(address);

    if (getsockname(
            listener->native_socket,
            (struct sockaddr *)&address,
            &address_length) == SOCKET_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    if (address.ss_family == AF_INET &&
        address_length >= (int)sizeof(struct sockaddr_in)) {
        *out_port = (PAPACC_U16)ntohs(
            ((const struct sockaddr_in *)&address)->sin_port);
    } else if (address.ss_family == AF_INET6 &&
               address_length >= (int)sizeof(struct sockaddr_in6)) {
        *out_port = (PAPACC_U16)ntohs(
            ((const struct sockaddr_in6 *)&address)->sin6_port);
    } else {
        return PAPACC_RESULT_NOT_SUPPORTED;
    }
    return (*out_port != 0) ? PAPACC_RESULT_OK
                            : PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT papacc_test_client_endpoint(
    SOCKET client,
    PAPACC_NETWORK_ENDPOINT *out_endpoint)
{
    struct sockaddr_storage address;
    int address_length = (int)sizeof(address);
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_RESULT result;

    if (getsockname(
            client, (struct sockaddr *)&address, &address_length) ==
        SOCKET_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    if (address.ss_family == AF_INET &&
        address_length >= (int)sizeof(struct sockaddr_in)) {
        const struct sockaddr_in *ipv4 =
            (const struct sockaddr_in *)&address;
        const PAPACC_U8 *bytes = (const PAPACC_U8 *)&ipv4->sin_addr;
        result = papacc_ip_address_set_ipv4(
            &endpoint.address, bytes[0], bytes[1], bytes[2], bytes[3]);
        endpoint.port = (PAPACC_U16)ntohs(ipv4->sin_port);
    } else if (address.ss_family == AF_INET6 &&
               address_length >= (int)sizeof(struct sockaddr_in6)) {
        const struct sockaddr_in6 *ipv6 =
            (const struct sockaddr_in6 *)&address;
        result = papacc_ip_address_set_ipv6(
            &endpoint.address, (const PAPACC_U8 *)&ipv6->sin6_addr);
        endpoint.port = (PAPACC_U16)ntohs(ipv6->sin6_port);
        endpoint.scope_id = (PAPACC_U32)ipv6->sin6_scope_id;
    } else {
        return PAPACC_RESULT_NOT_SUPPORTED;
    }
    if (result != PAPACC_RESULT_OK ||
        papacc_network_endpoint_validate(&endpoint) != PAPACC_RESULT_OK) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    *out_endpoint = endpoint;
    return PAPACC_RESULT_OK;
}

static SOCKET papacc_test_connect_ipv4(PAPACC_U16 port)
{
    struct sockaddr_in address;
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(
            client, (const struct sockaddr *)&address,
            (int)sizeof(address)) == SOCKET_ERROR) {
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    return client;
}

static SOCKET papacc_test_connect_ipv6(
    PAPACC_U16 port,
    int *out_native_error)
{
    struct sockaddr_in6 address;
    SOCKET client = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

    *out_native_error = 0;
    if (client == INVALID_SOCKET) {
        *out_native_error = WSAGetLastError();
        return INVALID_SOCKET;
    }
    address.sin6_family = AF_INET6;
    address.sin6_port = htons(port);
    address.sin6_flowinfo = 0;
    address.sin6_addr = in6addr_loopback;
    address.sin6_scope_id = 0;
    if (connect(
            client, (const struct sockaddr *)&address,
            (int)sizeof(address)) == SOCKET_ERROR) {
        *out_native_error = WSAGetLastError();
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    return client;
}

static int papacc_test_invalid_states(const PAPACC_TCP_PLATFORM *platform)
{
    PAPACC_TCP_SOCKET_WIN32 listener = PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted =
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;

    papacc_tcp_accepted_socket_win32_close(NULL);
    papacc_tcp_accepted_socket_win32_close(&accepted);
    if (papacc_tcp_socket_win32_accept(NULL, &accepted) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_tcp_socket_win32_accept(&listener, NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_test_accepted_is_empty(&accepted) != PAPACC_TRUE) {
        return 1;
    }

    accepted.native_socket = (SOCKET)0;
    if (papacc_tcp_socket_win32_accept(&listener, &accepted) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 2;
    }
    accepted = (PAPACC_TCP_ACCEPTED_SOCKET_WIN32)
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    if (papacc_ip_address_set_ipv4(
            &target.address, 127, 0, 0, 1) != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(
            platform, &target, 0, &listener) != PAPACC_RESULT_OK) {
        return 3;
    }
    if (papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_test_accepted_is_empty(&accepted) != PAPACC_TRUE ||
        listener.is_open != PAPACC_TRUE || listener.is_bound != PAPACC_TRUE ||
        listener.is_listening != PAPACC_FALSE) {
        papacc_tcp_socket_win32_close(&listener);
        return 4;
    }
    papacc_tcp_socket_win32_close(&listener);
    return 0;
}

static int papacc_test_ipv4_reaccept(const PAPACC_TCP_PLATFORM *platform)
{
    PAPACC_TCP_SOCKET_WIN32 listener = PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted =
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT expected_remote =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_U16 port = 0;
    SOCKET client = INVALID_SOCKET;
    SOCKET owned_socket;
    int result = 0;

    if (papacc_ip_address_set_ipv4(
            &target.address, 127, 0, 0, 1) != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(
            platform, &target, 0, &listener) != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&listener) != PAPACC_RESULT_OK ||
        papacc_test_listener_port(&listener, &port) != PAPACC_RESULT_OK) {
        result = 10;
        goto cleanup;
    }
    client = papacc_test_connect_ipv4(port);
    if (client == INVALID_SOCKET ||
        papacc_test_client_endpoint(client, &expected_remote) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_OK) {
        result = 11;
        goto cleanup;
    }
    if (accepted.is_open != PAPACC_TRUE ||
        accepted.family != PAPACC_IP_FAMILY_IPV4 ||
        accepted.local_endpoint.address.family != PAPACC_IP_FAMILY_IPV4 ||
        accepted.local_endpoint.port != port ||
        accepted.local_endpoint.scope_id != 0 ||
        accepted.remote_endpoint.port == 0 ||
        papacc_network_endpoint_equal(
            &accepted.remote_endpoint, &expected_remote) != PAPACC_TRUE ||
        papacc_ip_address_is_loopback(&accepted.local_endpoint.address) !=
            PAPACC_TRUE ||
        listener.is_listening != PAPACC_TRUE) {
        result = 12;
        goto cleanup;
    }
    owned_socket = accepted.native_socket;
    if (papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_INVALID_STATE ||
        accepted.native_socket != owned_socket ||
        accepted.is_open != PAPACC_TRUE) {
        result = 13;
        goto cleanup;
    }
    papacc_tcp_accepted_socket_win32_close(&accepted);
    papacc_tcp_accepted_socket_win32_close(&accepted);
    (void)closesocket(client);
    client = INVALID_SOCKET;
    if (papacc_test_accepted_is_empty(&accepted) != PAPACC_TRUE ||
        listener.is_listening != PAPACC_TRUE) {
        result = 14;
        goto cleanup;
    }

    client = papacc_test_connect_ipv4(port);
    if (client == INVALID_SOCKET ||
        papacc_test_client_endpoint(client, &expected_remote) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_OK ||
        papacc_network_endpoint_equal(
            &accepted.remote_endpoint, &expected_remote) != PAPACC_TRUE ||
        listener.is_listening != PAPACC_TRUE) {
        result = 15;
    }

cleanup:
    papacc_tcp_accepted_socket_win32_close(&accepted);
    if (client != INVALID_SOCKET) {
        (void)closesocket(client);
    }
    papacc_tcp_socket_win32_close(&listener);
    return result;
}

static int papacc_test_ipv6(const PAPACC_TCP_PLATFORM *platform)
{
    static const PAPACC_U8 loopback[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    };
    PAPACC_TCP_SOCKET_WIN32 listener = PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted =
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT expected_remote =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_U16 port = 0;
    PAPACC_RESULT bind_result;
    SOCKET client = INVALID_SOCKET;
    int native_error = 0;
    int result = 0;

    if (papacc_ip_address_set_ipv6(&target.address, loopback) !=
        PAPACC_RESULT_OK) {
        return 20;
    }
    bind_result = papacc_tcp_socket_win32_bind(
        platform, &target, 0, &listener);
    if (bind_result == PAPACC_RESULT_NOT_SUPPORTED) {
        return 0;
    }
    if (bind_result != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&listener) != PAPACC_RESULT_OK ||
        papacc_test_listener_port(&listener, &port) != PAPACC_RESULT_OK) {
        result = 21;
        goto cleanup;
    }
    client = papacc_test_connect_ipv6(port, &native_error);
    if (client == INVALID_SOCKET) {
        if (native_error != WSAEAFNOSUPPORT &&
            native_error != WSAEPROTONOSUPPORT &&
            native_error != WSAEADDRNOTAVAIL &&
            native_error != WSAENETUNREACH) {
            result = 22;
        }
        goto cleanup;
    }
    if (papacc_test_client_endpoint(client, &expected_remote) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_accept(&listener, &accepted) !=
            PAPACC_RESULT_OK ||
        accepted.family != PAPACC_IP_FAMILY_IPV6 ||
        accepted.local_endpoint.address.family != PAPACC_IP_FAMILY_IPV6 ||
        accepted.local_endpoint.port != port ||
        accepted.local_endpoint.scope_id != 0 ||
        papacc_network_endpoint_equal(
            &accepted.remote_endpoint, &expected_remote) != PAPACC_TRUE ||
        listener.is_listening != PAPACC_TRUE) {
        result = 23;
    }

cleanup:
    papacc_tcp_accepted_socket_win32_close(&accepted);
    if (client != INVALID_SOCKET) {
        (void)closesocket(client);
    }
    papacc_tcp_socket_win32_close(&listener);
    return result;
}

int main(void)
{
    PAPACC_TCP_PLATFORM platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    int result;

    if (papacc_tcp_platform_init(&platform) != PAPACC_RESULT_OK) {
        return 30;
    }
    result = papacc_test_invalid_states(&platform);
    if (result == 0) {
        result = papacc_test_ipv4_reaccept(&platform);
    }
    if (result == 0) {
        result = papacc_test_ipv6(&platform);
    }
    papacc_tcp_platform_shutdown(&platform);
    return result;
}
