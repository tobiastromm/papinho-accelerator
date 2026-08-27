#ifndef PAPACC_SESSION_H
#define PAPACC_SESSION_H

#include <papacc/types.h>

typedef enum PAPACC_SESSION_STATE {
    PAPACC_SESSION_STATE_UNINITIALIZED = 0,
    PAPACC_SESSION_STATE_ESTABLISHING = 1,
    PAPACC_SESSION_STATE_ACTIVE = 2,
    PAPACC_SESSION_STATE_CLOSING = 3,
    PAPACC_SESSION_STATE_CLOSED = 4
} PAPACC_SESSION_STATE;

/*
 * Portable logical client/session lifecycle entity. It is not a Connection,
 * socket, listener, or channel and owns none of those models in Phase 2.B1.
 * session_instance_id is runtime-only and is never a future Wire Session ID.
 */
typedef struct PAPACC_SESSION {
    PAPACC_U64 session_instance_id;
    PAPACC_SESSION_STATE state;
} PAPACC_SESSION;

#define PAPACC_SESSION_INITIALIZER \
    { 0, PAPACC_SESSION_STATE_UNINITIALIZED }

/* The only valid activation transition is ESTABLISHING -> ACTIVE. */
PAPACC_RESULT papacc_session_activate(PAPACC_SESSION *session);

/*
 * ESTABLISHING/ACTIVE -> CLOSING -> CLOSED. NULL, UNINITIALIZED, and CLOSED
 * are no-ops. A published CLOSED entity retains its runtime ID for diagnostics.
 */
void papacc_session_close(PAPACC_SESSION *session);

typedef struct PAPACC_SESSION_MANAGER {
    PAPACC_SESSION *storage;
    PAPACC_SIZE capacity;
    PAPACC_SIZE count;
    PAPACC_U64 next_instance_id;
    PAPACC_BOOL initialized;
} PAPACC_SESSION_MANAGER;

#define PAPACC_SESSION_MANAGER_INITIALIZER \
    { NULL, 0, 0, 1, PAPACC_FALSE }

/* Caller owns fixed storage. Capacity zero is valid and cannot publish. */
PAPACC_RESULT papacc_session_manager_init(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_SESSION *storage,
    PAPACC_SIZE capacity);

/* Publishes ESTABLISHING in a fixed slot; every failure publishes NULL. */
PAPACC_RESULT papacc_session_manager_publish(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_SESSION **out_session);

/* Returned pointers remain stable until remove or manager shutdown. */
PAPACC_SESSION *papacc_session_manager_find(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_U64 session_instance_id);

PAPACC_RESULT papacc_session_manager_remove(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_U64 session_instance_id);

/* Resets slots/manager without freeing caller storage; idempotent. */
void papacc_session_manager_shutdown(PAPACC_SESSION_MANAGER *manager);

#endif
