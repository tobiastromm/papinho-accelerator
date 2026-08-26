#include "tcp_listener_set_win32.h"

#include <ws2tcpip.h>

static void papacc_tcp_listener_entry_win32_reset(
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry)
{
    PAPACC_BIND_TARGET empty_target = PAPACC_BIND_TARGET_INITIALIZER;

    papacc_tcp_socket_win32_close(&entry->socket);
    entry->target = empty_target;
}

static PAPACC_RESULT papacc_tcp_listener_win32_get_port(
    const PAPACC_TCP_SOCKET_WIN32 *socket_context,
    PAPACC_U16 *out_port)
{
    if (socket_context->family == PAPACC_IP_FAMILY_IPV4) {
        struct sockaddr_in address;
        int address_size = (int)sizeof(address);

        if (getsockname(
                socket_context->native_socket,
                (struct sockaddr *)&address,
                &address_size) == SOCKET_ERROR) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        *out_port = (PAPACC_U16)ntohs(address.sin_port);
    } else if (socket_context->family == PAPACC_IP_FAMILY_IPV6) {
        struct sockaddr_in6 address;
        int address_size = (int)sizeof(address);

        if (getsockname(
                socket_context->native_socket,
                (struct sockaddr *)&address,
                &address_size) == SOCKET_ERROR) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        *out_port = (PAPACC_U16)ntohs(address.sin6_port);
    } else {
        return PAPACC_RESULT_INVALID_STATE;
    }

    return (*out_port != 0) ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
}

static void papacc_tcp_listener_entries_win32_rollback(
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *entries,
    PAPACC_SIZE count)
{
    while (count > 0) {
        --count;
        papacc_tcp_listener_entry_win32_reset(&entries[count]);
    }
}

PAPACC_RESULT papacc_tcp_listener_set_win32_start(
    const PAPACC_TCP_PLATFORM *platform,
    const PAPACC_BIND_TARGET *targets,
    PAPACC_SIZE target_count,
    PAPACC_U16 port,
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry_storage,
    PAPACC_SIZE entry_capacity,
    PAPACC_TCP_LISTENER_SET_WIN32 *listener_set)
{
    PAPACC_SIZE target_index;
    PAPACC_SIZE active_count = 0;
    PAPACC_SIZE unsupported_count = 0;
    PAPACC_U16 effective_port = port;

    if (platform == NULL || targets == NULL || entry_storage == NULL ||
        listener_set == NULL || target_count == 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (platform->initialized != PAPACC_TRUE ||
        listener_set->is_active == PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (entry_capacity < target_count) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }

    listener_set->entries = NULL;
    listener_set->capacity = 0;
    listener_set->count = 0;
    listener_set->unsupported_target_count = 0;
    listener_set->bound_port = 0;
    listener_set->is_active = PAPACC_FALSE;

    for (target_index = 0; target_index < target_count; ++target_index) {
        PAPACC_TCP_LISTENER_ENTRY_WIN32 empty_entry =
            PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
        entry_storage[target_index] = empty_entry;
    }

    for (target_index = 0; target_index < target_count; ++target_index) {
        PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
            PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
        PAPACC_RESULT result;

        entry.target = targets[target_index];
        result = papacc_tcp_socket_win32_bind(
            platform, &entry.target, effective_port, &entry.socket);
        if (result == PAPACC_RESULT_NOT_SUPPORTED) {
            ++unsupported_count;
            continue;
        }
        if (result != PAPACC_RESULT_OK) {
            papacc_tcp_listener_entries_win32_rollback(
                entry_storage, active_count);
            return result;
        }

        result = papacc_tcp_socket_win32_listen(&entry.socket);
        if (result != PAPACC_RESULT_OK) {
            papacc_tcp_socket_win32_close(&entry.socket);
            papacc_tcp_listener_entries_win32_rollback(
                entry_storage, active_count);
            return result;
        }

        if (effective_port == 0) {
            result = papacc_tcp_listener_win32_get_port(
                &entry.socket, &effective_port);
            if (result != PAPACC_RESULT_OK) {
                papacc_tcp_socket_win32_close(&entry.socket);
                papacc_tcp_listener_entries_win32_rollback(
                    entry_storage, active_count);
                return result;
            }
        }

        entry_storage[active_count] = entry;
        ++active_count;
    }

    if (active_count == 0) {
        listener_set->unsupported_target_count = unsupported_count;
        return PAPACC_RESULT_NOT_SUPPORTED;
    }

    listener_set->entries = entry_storage;
    listener_set->capacity = entry_capacity;
    listener_set->count = active_count;
    listener_set->unsupported_target_count = unsupported_count;
    listener_set->bound_port = effective_port;
    listener_set->is_active = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_tcp_listener_set_win32_shutdown(
    PAPACC_TCP_LISTENER_SET_WIN32 *listener_set)
{
    if (listener_set == NULL) {
        return;
    }

    if (listener_set->entries != NULL) {
        papacc_tcp_listener_entries_win32_rollback(
            listener_set->entries, listener_set->count);
    }

    listener_set->entries = NULL;
    listener_set->capacity = 0;
    listener_set->count = 0;
    listener_set->unsupported_target_count = 0;
    listener_set->bound_port = 0;
    listener_set->is_active = PAPACC_FALSE;
}
