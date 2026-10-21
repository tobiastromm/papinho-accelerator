#include "tcp_accept_win32.h"

#include <ws2tcpip.h>

static PAPACC_BOOL papacc_tcp_accepted_socket_win32_is_empty(
    const PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *accepted)
{
    PAPACC_NETWORK_ENDPOINT empty_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;

    return (accepted->native_socket == INVALID_SOCKET &&
            accepted->is_open == PAPACC_FALSE &&
            accepted->family == PAPACC_IP_FAMILY_UNSPECIFIED &&
            papacc_network_endpoint_equal(
                &accepted->local_endpoint, &empty_endpoint) == PAPACC_TRUE &&
            papacc_network_endpoint_equal(
                &accepted->remote_endpoint, &empty_endpoint) == PAPACC_TRUE)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

static PAPACC_RESULT papacc_tcp_win32_accept_error(int native_error)
{
    if (native_error == WSAEAFNOSUPPORT ||
        native_error == WSAEPROTONOSUPPORT ||
        native_error == WSAEOPNOTSUPP) {
        return PAPACC_RESULT_NOT_SUPPORTED;
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT papacc_tcp_win32_endpoint_from_native(
    const struct sockaddr *native_address,
    int native_length,
    PAPACC_NETWORK_ENDPOINT *out_endpoint)
{
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_RESULT result;

    if (native_address == NULL || out_endpoint == NULL ||
        native_length < (int)sizeof(struct sockaddr)) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    if (native_address->sa_family == AF_INET) {
        const struct sockaddr_in *ipv4;
        const PAPACC_U8 *bytes;

        if (native_length < (int)sizeof(struct sockaddr_in)) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        ipv4 = (const struct sockaddr_in *)native_address;
        bytes = (const PAPACC_U8 *)&ipv4->sin_addr;
        result = papacc_ip_address_set_ipv4(
            &endpoint.address, bytes[0], bytes[1], bytes[2], bytes[3]);
        endpoint.port = (PAPACC_U16)ntohs(ipv4->sin_port);
        endpoint.scope_id = 0;
    } else if (native_address->sa_family == AF_INET6) {
        const struct sockaddr_in6 *ipv6;
        const PAPACC_U8 *bytes;

        if (native_length < (int)sizeof(struct sockaddr_in6)) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        ipv6 = (const struct sockaddr_in6 *)native_address;
        bytes = (const PAPACC_U8 *)&ipv6->sin6_addr;
        result = papacc_ip_address_set_ipv6(&endpoint.address, bytes);
        endpoint.port = (PAPACC_U16)ntohs(ipv6->sin6_port);
        endpoint.scope_id = (PAPACC_U32)ipv6->sin6_scope_id;
    } else {
        return PAPACC_RESULT_NOT_SUPPORTED;
    }
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    result = papacc_network_endpoint_validate(&endpoint);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    *out_endpoint = endpoint;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_tcp_socket_win32_accept(
    const PAPACC_TCP_SOCKET_WIN32 *listener,
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *out_accepted)
{
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted =
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    struct sockaddr_storage remote_address;
    struct sockaddr_storage local_address;
    int remote_length = (int)sizeof(remote_address);
    int local_length = (int)sizeof(local_address);
    PAPACC_RESULT result;

    if (listener == NULL || out_accepted == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (out_accepted->is_open == PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (papacc_tcp_accepted_socket_win32_is_empty(out_accepted) ==
        PAPACC_FALSE) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    *out_accepted = accepted;
    if (listener->is_open != PAPACC_TRUE ||
        listener->is_bound != PAPACC_TRUE ||
        listener->is_listening != PAPACC_TRUE ||
        listener->native_socket == INVALID_SOCKET ||
        (listener->family != PAPACC_IP_FAMILY_IPV4 &&
         listener->family != PAPACC_IP_FAMILY_IPV6)) {
        return PAPACC_RESULT_INVALID_STATE;
    }

    accepted.native_socket = accept(
        listener->native_socket,
        (struct sockaddr *)&remote_address,
        &remote_length);
    if (accepted.native_socket == INVALID_SOCKET) {
        return papacc_tcp_win32_accept_error(WSAGetLastError());
    }
    result = papacc_tcp_win32_endpoint_from_native(
        (const struct sockaddr *)&remote_address,
        remote_length,
        &accepted.remote_endpoint);
    if (result != PAPACC_RESULT_OK) {
        goto failure;
    }
    if (getsockname(
            accepted.native_socket,
            (struct sockaddr *)&local_address,
            &local_length) == SOCKET_ERROR) {
        result = papacc_tcp_win32_accept_error(WSAGetLastError());
        goto failure;
    }
    result = papacc_tcp_win32_endpoint_from_native(
        (const struct sockaddr *)&local_address,
        local_length,
        &accepted.local_endpoint);
    if (result != PAPACC_RESULT_OK) {
        goto failure;
    }
    if (accepted.local_endpoint.address.family !=
        accepted.remote_endpoint.address.family) {
        result = PAPACC_RESULT_INTERNAL_ERROR;
        goto failure;
    }

    accepted.is_open = PAPACC_TRUE;
    accepted.family = accepted.remote_endpoint.address.family;
    *out_accepted = accepted;
    return PAPACC_RESULT_OK;

failure:
    (void)closesocket(accepted.native_socket);
    *out_accepted = (PAPACC_TCP_ACCEPTED_SOCKET_WIN32)
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    return result;
}

PAPACC_RESULT papacc_tcp_accepted_socket_win32_set_nonblocking(
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *accepted,
    PAPACC_BOOL enabled)
{
    u_long native_mode;

    if (accepted == NULL ||
        (enabled != PAPACC_FALSE && enabled != PAPACC_TRUE)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (accepted->is_open != PAPACC_TRUE ||
        accepted->native_socket == INVALID_SOCKET ||
        (accepted->family != PAPACC_IP_FAMILY_IPV4 &&
         accepted->family != PAPACC_IP_FAMILY_IPV6)) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    native_mode = enabled == PAPACC_TRUE ? 1UL : 0UL;
    if (ioctlsocket(accepted->native_socket, FIONBIO, &native_mode) ==
        SOCKET_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    return PAPACC_RESULT_OK;
}

void papacc_tcp_accepted_socket_win32_close(
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *accepted)
{
    if (accepted == NULL) {
        return;
    }
    if (accepted->is_open == PAPACC_TRUE &&
        accepted->native_socket != INVALID_SOCKET) {
        (void)closesocket(accepted->native_socket);
    }
    *accepted = (PAPACC_TCP_ACCEPTED_SOCKET_WIN32)
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
}
