#include "tcp_connection_transport_win32.h"

#include <limits.h>

static PAPACC_RESULT papacc_tcp_connection_transport_win32_read(
    void *opaque_context,
    PAPACC_U8 *buffer,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context =
        (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *)opaque_context;
    int requested = capacity > (PAPACC_SIZE)INT_MAX ? INT_MAX : (int)capacity;
    int received = recv(context->native_socket, (char *)buffer, requested, 0);

    if (received > 0) {
        *out_transferred = (PAPACC_SIZE)received;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    if (received == 0) {
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (WSAGetLastError() == WSAEWOULDBLOCK) {
        *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT papacc_tcp_connection_transport_win32_write(
    void *opaque_context,
    const PAPACC_U8 *buffer,
    PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context =
        (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *)opaque_context;
    int requested = length > (PAPACC_SIZE)INT_MAX ? INT_MAX : (int)length;
    int sent = send(context->native_socket, (const char *)buffer, requested, 0);

    if (sent > 0) {
        *out_transferred = (PAPACC_SIZE)sent;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    if (sent == 0) {
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (WSAGetLastError() == WSAEWOULDBLOCK) {
        *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static void papacc_tcp_connection_transport_win32_close(void *opaque_context)
{
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context =
        (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *)opaque_context;

    if (context->owns_socket == PAPACC_TRUE &&
        context->native_socket != INVALID_SOCKET) {
        (void)closesocket(context->native_socket);
    }
    *context = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
}

PAPACC_RESULT papacc_tcp_connection_transport_win32_move(
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *accepted,
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context,
    PAPACC_TRANSPORT_CONNECTION *out_transport)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;

    if (accepted == NULL || context == NULL || out_transport == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (context->owns_socket == PAPACC_TRUE ||
        context->native_socket != INVALID_SOCKET ||
        out_transport->context != NULL || out_transport->read_fn != NULL ||
        out_transport->write_fn != NULL || out_transport->close_fn != NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (accepted->is_open != PAPACC_TRUE ||
        accepted->native_socket == INVALID_SOCKET ||
        (accepted->family != PAPACC_IP_FAMILY_IPV4 &&
         accepted->family != PAPACC_IP_FAMILY_IPV6) ||
        papacc_network_endpoint_validate(&accepted->local_endpoint) !=
            PAPACC_RESULT_OK ||
        papacc_network_endpoint_validate(&accepted->remote_endpoint) !=
            PAPACC_RESULT_OK) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    context->native_socket = accepted->native_socket;
    context->owns_socket = PAPACC_TRUE;
    transport.context = context;
    transport.read_fn = papacc_tcp_connection_transport_win32_read;
    transport.write_fn = papacc_tcp_connection_transport_win32_write;
    transport.close_fn = papacc_tcp_connection_transport_win32_close;
    *accepted = (PAPACC_TCP_ACCEPTED_SOCKET_WIN32)
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    *out_transport = transport;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_tcp_connection_transport_win32_get_native_socket(
    const PAPACC_TRANSPORT_CONNECTION *transport,
    SOCKET *out_native_socket)
{
    const PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context;

    if (transport == NULL || out_native_socket == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (transport->context == NULL ||
        transport->read_fn != papacc_tcp_connection_transport_win32_read ||
        transport->write_fn != papacc_tcp_connection_transport_win32_write ||
        transport->close_fn != papacc_tcp_connection_transport_win32_close) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    context = (const PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *)
        transport->context;
    if (context->owns_socket != PAPACC_TRUE ||
        context->native_socket == INVALID_SOCKET) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    *out_native_socket = context->native_socket;
    return PAPACC_RESULT_OK;
}
