#ifndef PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_H
#define PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_H

#include "tcp_accept_win32.h"
#include "transport_connection.h"

typedef struct PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT {
    SOCKET native_socket;
    PAPACC_BOOL owns_socket;
} PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT;

#define PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER \
    { INVALID_SOCKET, PAPACC_FALSE }

/*
 * Moves only the accepted socket into caller-provided context. On success the
 * source accepted object is reset. On every failure it remains the sole owner.
 * The context must outlive the abstract transport/Connection that references
 * it. Closing this transport never closes a listener or WinSock globally.
 * Accepted Connections must therefore be closed before listeners/server
 * network and before the TCP Platform performs global WinSock cleanup.
 */
PAPACC_RESULT papacc_tcp_connection_transport_win32_move(
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *accepted,
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context,
    PAPACC_TRANSPORT_CONNECTION *out_transport);

#endif
