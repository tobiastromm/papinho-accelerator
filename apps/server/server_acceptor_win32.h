#ifndef PAPACC_SERVER_ACCEPTOR_WIN32_H
#define PAPACC_SERVER_ACCEPTOR_WIN32_H

#include "connection.h"
#include "server_network.h"
#include "tcp_connection_transport_win32.h"

typedef struct PAPACC_SERVER_ACCEPTOR_WIN32 {
    PAPACC_SERVER_NETWORK *server_network;
    PAPACC_CONNECTION_MANAGER connection_manager;
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT
        *transport_context_storage;
    PAPACC_SIZE transport_context_capacity;
    PAPACC_SIZE next_listener_index;
    PAPACC_BOOL initialized;
} PAPACC_SERVER_ACCEPTOR_WIN32;

#define PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER \
    { NULL, PAPACC_CONNECTION_MANAGER_INITIALIZER, NULL, 0, 0, PAPACC_FALSE }

/*
 * Borrows the active Server Network and caller-owned fixed storages. Connection
 * and context capacities must be equal; zero is a valid reject-only capacity.
 * The Acceptor must be shut down before its Server Network.
 */
PAPACC_RESULT papacc_server_acceptor_win32_init(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
    PAPACC_SERVER_NETWORK *server_network,
    PAPACC_CONNECTION *connection_storage,
    PAPACC_SIZE connection_capacity,
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT
        *transport_context_storage,
    PAPACC_SIZE transport_context_capacity);

/*
 * Finite timeout in milliseconds; zero is non-blocking. Each call publishes
 * at most one PENDING Connection. A ready connection that cannot be published
 * due to capacity is accepted, closed, and reported as LIMIT_EXCEEDED. Each
 * event is independently atomic; earlier Connections are never rolled back.
 */
PAPACC_RESULT papacc_server_acceptor_win32_poll_once(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
    PAPACC_U32 timeout_ms,
    PAPACC_CONNECTION **out_connection);

/* Closes Connections/contexts but never shuts down the borrowed network. */
void papacc_server_acceptor_win32_shutdown(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor);

#endif
