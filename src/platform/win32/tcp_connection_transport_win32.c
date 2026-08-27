#include "tcp_connection_transport_win32.h"

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
        out_transport->context != NULL || out_transport->close_fn != NULL) {
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
    transport.close_fn = papacc_tcp_connection_transport_win32_close;
    *accepted = (PAPACC_TCP_ACCEPTED_SOCKET_WIN32)
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    *out_transport = transport;
    return PAPACC_RESULT_OK;
}
