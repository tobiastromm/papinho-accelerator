#ifndef PAPACC_TCP_ACCEPT_WIN32_H
#define PAPACC_TCP_ACCEPT_WIN32_H

#include "network_endpoint.h"
#include "tcp_socket_win32.h"

typedef struct PAPACC_TCP_ACCEPTED_SOCKET_WIN32 {
    SOCKET native_socket;
    PAPACC_BOOL is_open;
    PAPACC_IP_FAMILY family;
    PAPACC_NETWORK_ENDPOINT local_endpoint;
    PAPACC_NETWORK_ENDPOINT remote_endpoint;
} PAPACC_TCP_ACCEPTED_SOCKET_WIN32;

#define PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER \
    { INVALID_SOCKET, PAPACC_FALSE, PAPACC_IP_FAMILY_UNSPECIFIED, \
      PAPACC_NETWORK_ENDPOINT_INITIALIZER, \
      PAPACC_NETWORK_ENDPOINT_INITIALIZER }

/*
 * Accepts one transport from an open/bound/listening socket. This operation
 * may block until a pending connection exists. It does not classify a channel,
 * create a Connection/Session, or alter ownership/state of the listener.
 *
 * out_accepted must be exactly initialized/closed. The backend owns the native
 * accepted socket until both endpoints have been obtained, converted, and
 * validated. Only successful publication transfers ownership to the caller.
 */
PAPACC_RESULT papacc_tcp_socket_win32_accept(
    const PAPACC_TCP_SOCKET_WIN32 *listener,
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *out_accepted);

/* Closes only the published accepted socket and restores the initializer. */
void papacc_tcp_accepted_socket_win32_close(
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *accepted);

#endif
