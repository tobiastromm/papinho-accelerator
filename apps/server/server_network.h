#ifndef PAPACC_SERVER_NETWORK_H
#define PAPACC_SERVER_NETWORK_H

#include "server_config.h"
#include "tcp_listener_set_win32.h"

typedef struct PAPACC_SERVER_NETWORK {
    PAPACC_TCP_PLATFORM tcp_platform;
    PAPACC_TCP_LISTENER_SET_WIN32 listener_set;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *listener_storage;
    PAPACC_SIZE listener_storage_capacity;
    PAPACC_BOOL is_active;
} PAPACC_SERVER_NETWORK;

#define PAPACC_SERVER_NETWORK_INITIALIZER \
    { PAPACC_TCP_PLATFORM_INITIALIZER, \
      PAPACC_TCP_LISTENER_SET_WIN32_INITIALIZER, NULL, 0, PAPACC_FALSE }

/*
 * Application composition only: validates an origin-agnostic Server Config,
 * discovers the current snapshot, resolves persistent/runtime bind intent,
 * then acquires WinSock and starts the Listener Set. Network egress policy is
 * retained by the config but is not operationally consumed in this phase.
 *
 * The caller owns the aggregate. On successful start, the aggregate owns its
 * heap-allocated listener_storage and the live sockets stored in listener_set;
 * shutdown releases both and WinSock. The config and its persistent-ID view
 * are borrowed only for the duration of start. Discovery/presentation storage,
 * runtime IDs, and Bind Targets are temporary and are not retained after start.
 * A failed start publishes no partial ownership into the aggregate.
 */
PAPACC_RESULT papacc_server_network_start(
    PAPACC_SERVER_NETWORK *network,
    const PAPACC_SERVER_CONFIG *config);

void papacc_server_network_shutdown(PAPACC_SERVER_NETWORK *network);

#endif
