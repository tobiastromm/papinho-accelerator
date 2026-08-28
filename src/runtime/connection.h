#ifndef PAPACC_CONNECTION_H
#define PAPACC_CONNECTION_H

#include "network_endpoint.h"
#include "transport_connection.h"

typedef enum PAPACC_CONNECTION_STATE {
    PAPACC_CONNECTION_STATE_UNINITIALIZED = 0,
    PAPACC_CONNECTION_STATE_PENDING = 1,
    PAPACC_CONNECTION_STATE_ASSOCIATED = 2,
    PAPACC_CONNECTION_STATE_CLOSING = 3,
    PAPACC_CONNECTION_STATE_CLOSED = 4
} PAPACC_CONNECTION_STATE;

/*
 * A runtime Transport Connection, never a Session. PENDING means transport
 * established, role unclassified, unauthenticated, and not Session-bound.
 */
typedef struct PAPACC_CONNECTION {
    PAPACC_U64 connection_instance_id;
    PAPACC_CONNECTION_STATE state;
    PAPACC_NETWORK_ENDPOINT local_endpoint;
    PAPACC_NETWORK_ENDPOINT remote_endpoint;
    PAPACC_TRANSPORT_CONNECTION transport;
} PAPACC_CONNECTION;

#define PAPACC_CONNECTION_INITIALIZER \
    { 0, PAPACC_CONNECTION_STATE_UNINITIALIZED, \
      PAPACC_NETWORK_ENDPOINT_INITIALIZER, \
      PAPACC_NETWORK_ENDPOINT_INITIALIZER, \
      PAPACC_TRANSPORT_CONNECTION_INITIALIZER }

/* Keeps published identity/endpoints for diagnostics and enters CLOSED. */
void papacc_connection_close(PAPACC_CONNECTION *connection);

/* Higher relationship layer claim only: PENDING -> ASSOCIATED. */
PAPACC_RESULT papacc_connection_mark_associated(
    PAPACC_CONNECTION *connection);

typedef struct PAPACC_CONNECTION_MANAGER {
    PAPACC_CONNECTION *storage;
    PAPACC_SIZE capacity;
    PAPACC_SIZE count;
    PAPACC_U64 next_instance_id;
    PAPACC_BOOL initialized;
} PAPACC_CONNECTION_MANAGER;

/*
 * Temporary Phase 2.A3 ownership baseline: the manager owns every published
 * Connection. A future Session/channel layer may add an explicit transfer;
 * no such association or role exists in this model.
 */

#define PAPACC_CONNECTION_MANAGER_INITIALIZER \
    { NULL, 0, 0, 1, PAPACC_FALSE }

/* Capacity zero is valid but cannot publish. Storage remains caller-owned. */
PAPACC_RESULT papacc_connection_manager_init(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_CONNECTION *storage,
    PAPACC_SIZE capacity);

/*
 * Fixed slots do not move while published, so returned pointers remain stable
 * until remove/shutdown. Success moves transport; every failure leaves it
 * owned by the caller and publishes NULL.
 */
PAPACC_RESULT papacc_connection_manager_publish(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_TRANSPORT_CONNECTION *transport,
    const PAPACC_NETWORK_ENDPOINT *local_endpoint,
    const PAPACC_NETWORK_ENDPOINT *remote_endpoint,
    PAPACC_CONNECTION **out_connection);

PAPACC_CONNECTION *papacc_connection_manager_find(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_U64 connection_instance_id);

PAPACC_RESULT papacc_connection_manager_remove(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_U64 connection_instance_id);

/* Closes/resets all slots; a new init is required before reuse. */
void papacc_connection_manager_shutdown(PAPACC_CONNECTION_MANAGER *manager);

#endif
