#include "tcp_socket_win32.h"

#include <ws2tcpip.h>
#include <string.h>

static PAPACC_RESULT papacc_tcp_socket_win32_create(
    int address_family,
    SOCKET *out_socket)
{
    SOCKET native_socket;
    int native_error;

    native_socket = socket(address_family, SOCK_STREAM, IPPROTO_TCP);
    if (native_socket != INVALID_SOCKET) {
        *out_socket = native_socket;
        return PAPACC_RESULT_OK;
    }

    native_error = WSAGetLastError();
    if (native_error == WSAEAFNOSUPPORT ||
        native_error == WSAEPROTONOSUPPORT ||
        native_error == WSAESOCKTNOSUPPORT) {
        return PAPACC_RESULT_NOT_SUPPORTED;
    }

    return PAPACC_RESULT_INTERNAL_ERROR;
}

PAPACC_RESULT papacc_tcp_socket_win32_bind(
    const PAPACC_TCP_PLATFORM *platform,
    const PAPACC_BIND_TARGET *target,
    PAPACC_U16 port,
    PAPACC_TCP_SOCKET_WIN32 *socket_context)
{
    SOCKET native_socket = INVALID_SOCKET;
    PAPACC_RESULT result;
    int address_family;
    int exclusive = 1;
    int bind_result;

    if (platform == NULL || target == NULL || socket_context == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (platform->initialized != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (socket_context->is_open == PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (socket_context->is_open != PAPACC_FALSE ||
        socket_context->is_bound != PAPACC_FALSE ||
        socket_context->is_listening != PAPACC_FALSE ||
        socket_context->native_socket != INVALID_SOCKET ||
        socket_context->family != PAPACC_IP_FAMILY_UNSPECIFIED) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    if (target->address.family == PAPACC_IP_FAMILY_IPV4) {
        if (target->scope_id != 0) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        address_family = AF_INET;
    } else if (target->address.family == PAPACC_IP_FAMILY_IPV6) {
        address_family = AF_INET6;
    } else {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    result = papacc_tcp_socket_win32_create(address_family, &native_socket);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }

    if (setsockopt(
            native_socket,
            SOL_SOCKET,
            SO_EXCLUSIVEADDRUSE,
            (const char *)&exclusive,
            (int)sizeof(exclusive)) == SOCKET_ERROR) {
        (void)closesocket(native_socket);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    if (target->address.family == PAPACC_IP_FAMILY_IPV4) {
        struct sockaddr_in native_address;

        memset(&native_address, 0, sizeof(native_address));
        native_address.sin_family = AF_INET;
        native_address.sin_port = htons(port);
        memcpy(
            &native_address.sin_addr,
            target->address.bytes,
            sizeof(native_address.sin_addr));
        bind_result = bind(
            native_socket,
            (const struct sockaddr *)&native_address,
            (int)sizeof(native_address));
    } else {
        struct sockaddr_in6 native_address;

        memset(&native_address, 0, sizeof(native_address));
        native_address.sin6_family = AF_INET6;
        native_address.sin6_port = htons(port);
        native_address.sin6_scope_id = target->scope_id;
        memcpy(
            &native_address.sin6_addr,
            target->address.bytes,
            sizeof(native_address.sin6_addr));
        bind_result = bind(
            native_socket,
            (const struct sockaddr *)&native_address,
            (int)sizeof(native_address));
    }

    if (bind_result == SOCKET_ERROR) {
        (void)closesocket(native_socket);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    socket_context->native_socket = native_socket;
    socket_context->is_open = PAPACC_TRUE;
    socket_context->is_bound = PAPACC_TRUE;
    socket_context->is_listening = PAPACC_FALSE;
    socket_context->family = target->address.family;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_tcp_socket_win32_listen(
    PAPACC_TCP_SOCKET_WIN32 *socket_context)
{
    if (socket_context == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (socket_context->is_open != PAPACC_TRUE ||
        socket_context->is_bound != PAPACC_TRUE ||
        socket_context->is_listening != PAPACC_FALSE ||
        socket_context->native_socket == INVALID_SOCKET) {
        return PAPACC_RESULT_INVALID_STATE;
    }

    if (listen(socket_context->native_socket, SOMAXCONN) == SOCKET_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    socket_context->is_listening = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_tcp_socket_win32_close(
    PAPACC_TCP_SOCKET_WIN32 *socket_context)
{
    if (socket_context == NULL) {
        return;
    }

    if (socket_context->is_open == PAPACC_TRUE &&
        socket_context->native_socket != INVALID_SOCKET) {
        (void)closesocket(socket_context->native_socket);
    }

    socket_context->native_socket = INVALID_SOCKET;
    socket_context->is_open = PAPACC_FALSE;
    socket_context->is_bound = PAPACC_FALSE;
    socket_context->is_listening = PAPACC_FALSE;
    socket_context->family = PAPACC_IP_FAMILY_UNSPECIFIED;
}
