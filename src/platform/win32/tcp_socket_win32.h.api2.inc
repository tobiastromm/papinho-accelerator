#ifndef PAPACC_TCP_SOCKET_WIN32_H
#define PAPACC_TCP_SOCKET_WIN32_H

#include <winsock2.h>

#include "bind_target.h"
#include "tcp_platform.h"

typedef struct PAPACC_TCP_SOCKET_WIN32 {
    SOCKET native_socket;
    PAPACC_BOOL is_open;
    PAPACC_BOOL is_bound;
    PAPACC_BOOL is_listening;
    PAPACC_IP_FAMILY family;
} PAPACC_TCP_SOCKET_WIN32;

#define PAPACC_TCP_SOCKET_WIN32_INITIALIZER \
    { INVALID_SOCKET, PAPACC_FALSE, PAPACC_FALSE, PAPACC_FALSE, \
      PAPACC_IP_FAMILY_UNSPECIFIED }

/*
 * Port zero is valid here and requests an ephemeral port from WinSock. This
 * low-level socket rule is intentionally independent from application
 * configuration, where a zero control port represents incomplete input.
 */
PAPACC_RESULT papacc_tcp_socket_win32_bind(
    const PAPACC_TCP_PLATFORM *platform,
    const PAPACC_BIND_TARGET *target,
    PAPACC_U16 port,
    PAPACC_TCP_SOCKET_WIN32 *socket_context);

/*
 * Promotes an open, bound socket to a listener. SOMAXCONN is the initial
 * Win32 backlog policy, not a permanent application-level contract. If the
 * native listen operation fails, the caller retains the open, bound socket.
 */
PAPACC_RESULT papacc_tcp_socket_win32_listen(
    PAPACC_TCP_SOCKET_WIN32 *socket_context);

void papacc_tcp_socket_win32_close(
    PAPACC_TCP_SOCKET_WIN32 *socket_context);

#endif
