#ifndef PAPACC_CHANNEL_H
#define PAPACC_CHANNEL_H

#include "connection.h"
#include "session.h"

typedef enum PAPACC_CHANNEL_ROLE {
    PAPACC_CHANNEL_ROLE_UNSPECIFIED = 0,
    PAPACC_CHANNEL_ROLE_CONTROL = 1,
    PAPACC_CHANNEL_ROLE_DATA = 2
} PAPACC_CHANNEL_ROLE;

typedef enum PAPACC_CHANNEL_STATE {
    PAPACC_CHANNEL_STATE_UNINITIALIZED = 0,
    PAPACC_CHANNEL_STATE_BOUND = 1,
    PAPACC_CHANNEL_STATE_CLOSING = 2,
    PAPACC_CHANNEL_STATE_CLOSED = 3
} PAPACC_CHANNEL_STATE;

/*
 * Runtime relationship between exactly one Connection and one Session.
 * It owns neither transport nor lower entity and stores IDs, not pointers.
 * channel_instance_id is runtime-only, never a future Wire Channel ID.
 */
typedef struct PAPACC_CHANNEL {
    PAPACC_U64 channel_instance_id;
    PAPACC_CHANNEL_STATE state;
    PAPACC_CHANNEL_ROLE role;
    PAPACC_U64 session_instance_id;
    PAPACC_U64 connection_instance_id;
} PAPACC_CHANNEL;

#define PAPACC_CHANNEL_INITIALIZER \
    { 0, PAPACC_CHANNEL_STATE_UNINITIALIZED, \
      PAPACC_CHANNEL_ROLE_UNSPECIFIED, 0, 0 }

typedef struct PAPACC_CHANNEL_MANAGER {
    PAPACC_CHANNEL *storage;
    PAPACC_SIZE capacity;
    PAPACC_SIZE count;
    PAPACC_U64 next_instance_id;
    PAPACC_CONNECTION_MANAGER *connection_manager;
    PAPACC_SESSION_MANAGER *session_manager;
    PAPACC_BOOL initialized;
} PAPACC_CHANNEL_MANAGER;

#define PAPACC_CHANNEL_MANAGER_INITIALIZER \
    { NULL, 0, 0, 1, NULL, NULL, PAPACC_FALSE }

/*
 * Dependencies and fixed storage are non-owning and must outlive the Channel
 * Manager. Shut Channels down before removing bound Sessions/Connections.
 */
PAPACC_RESULT papacc_channel_manager_init(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_CHANNEL *storage,
    PAPACC_SIZE capacity,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager);

/*
 * Internal structural binding only. A DATA bind is not secure remote
 * association, and knowledge of any Session ID grants no attachment authority.
 */
PAPACC_RESULT papacc_channel_manager_bind(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 session_instance_id,
    PAPACC_U64 connection_instance_id,
    PAPACC_CHANNEL_ROLE role,
    PAPACC_CHANNEL **out_channel);

PAPACC_CHANNEL *papacc_channel_manager_find(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 channel_instance_id);
PAPACC_CHANNEL *papacc_channel_manager_find_by_connection(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 connection_instance_id);
PAPACC_CHANNEL *papacc_channel_manager_find_control(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 session_instance_id);

PAPACC_RESULT papacc_channel_manager_close(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 channel_instance_id);
PAPACC_RESULT papacc_channel_manager_remove(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 channel_instance_id);

/* Resets Channels only; dependency managers/storage remain externally owned. */
void papacc_channel_manager_shutdown(PAPACC_CHANNEL_MANAGER *manager);

#endif
