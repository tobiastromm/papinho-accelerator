#ifndef PAPACC_TCP_LISTENER_SET_WIN32_H
#define PAPACC_TCP_LISTENER_SET_WIN32_H

#include "tcp_socket_win32.h"

typedef struct PAPACC_TCP_LISTENER_ENTRY_WIN32 {
    PAPACC_BIND_TARGET target;
    PAPACC_TCP_SOCKET_WIN32 socket;
} PAPACC_TCP_LISTENER_ENTRY_WIN32;

#define PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER \
    { PAPACC_BIND_TARGET_INITIALIZER, PAPACC_TCP_SOCKET_WIN32_INITIALIZER }

typedef struct PAPACC_TCP_LISTENER_SET_WIN32 {
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *entries;
    PAPACC_SIZE capacity;
    PAPACC_SIZE count;
    PAPACC_SIZE unsupported_target_count;
    PAPACC_U16 bound_port;
    PAPACC_BOOL is_active;
} PAPACC_TCP_LISTENER_SET_WIN32;

#define PAPACC_TCP_LISTENER_SET_WIN32_INITIALIZER \
    { NULL, 0, 0, 0, 0, PAPACC_FALSE }

/*
 * The caller owns entry_storage and must keep it alive while the set is
 * active. The set owns the live socket resources stored there; callers must
 * not close individual entries. Target metadata, including the ephemeral
 * interface_instance_id, is copied for runtime use only and is not persisted.
 * Port zero selects one ephemeral port which is then shared by every listener.
 */
PAPACC_RESULT papacc_tcp_listener_set_win32_start(
    const PAPACC_TCP_PLATFORM *platform,
    const PAPACC_BIND_TARGET *targets,
    PAPACC_SIZE target_count,
    PAPACC_U16 port,
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry_storage,
    PAPACC_SIZE entry_capacity,
    PAPACC_TCP_LISTENER_SET_WIN32 *listener_set);

void papacc_tcp_listener_set_win32_shutdown(
    PAPACC_TCP_LISTENER_SET_WIN32 *listener_set);

#endif
