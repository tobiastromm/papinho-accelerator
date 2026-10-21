#include "connection.h"

static PAPACC_BOOL papacc_connection_manager_is_valid(
    const PAPACC_CONNECTION_MANAGER *manager)
{
    return (manager != NULL && manager->initialized == PAPACC_TRUE &&
            manager->count <= manager->capacity &&
            (manager->capacity == 0 || manager->storage != NULL))
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

static PAPACC_BOOL papacc_connection_id_exists(
    const PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_U64 instance_id)
{
    PAPACC_SIZE index;

    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state !=
                PAPACC_CONNECTION_STATE_UNINITIALIZED &&
            manager->storage[index].connection_instance_id == instance_id) {
            return PAPACC_TRUE;
        }
    }
    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_connection_next_id(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_U64 *out_instance_id)
{
    PAPACC_U64 candidate = manager->next_instance_id;
    PAPACC_SIZE collisions = 0;

    if (candidate == 0) {
        candidate = 1;
    }
    while (papacc_connection_id_exists(manager, candidate) == PAPACC_TRUE) {
        ++collisions;
        if (collisions > manager->count) {
            return PAPACC_RESULT_LIMIT_EXCEEDED;
        }
        ++candidate;
        if (candidate == 0) {
            candidate = 1;
        }
    }
    *out_instance_id = candidate;
    ++candidate;
    manager->next_instance_id = (candidate == 0) ? 1 : candidate;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_connection_try_close(
    PAPACC_CONNECTION *connection,
    PAPACC_BOOL *out_closed)
{
    PAPACC_BOOL ready = PAPACC_FALSE;
    PAPACC_RESULT result;

    if (out_closed != NULL) {
        *out_closed = PAPACC_FALSE;
    }
    if (connection == NULL || out_closed == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (connection->state == PAPACC_CONNECTION_STATE_CLOSED) {
        *out_closed = PAPACC_TRUE;
        return PAPACC_RESULT_OK;
    }
    if (connection->state == PAPACC_CONNECTION_STATE_UNINITIALIZED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    connection->state = PAPACC_CONNECTION_STATE_CLOSING;
    result = papacc_transport_connection_prepare_close(
        &connection->transport, &ready);
    if (result != PAPACC_RESULT_OK || ready != PAPACC_TRUE) {
        return result;
    }
    result = papacc_transport_connection_try_close(&connection->transport, out_closed);
    if (result != PAPACC_RESULT_OK || *out_closed != PAPACC_TRUE) return result;
    connection->state = PAPACC_CONNECTION_STATE_CLOSED;
    *out_closed = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_connection_close(PAPACC_CONNECTION *connection)
{
    PAPACC_BOOL closed;

    (void)papacc_connection_try_close(connection, &closed);
}

PAPACC_RESULT papacc_connection_mark_associated(
    PAPACC_CONNECTION *connection)
{
    if (connection == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (connection->state != PAPACC_CONNECTION_STATE_PENDING ||
        connection->connection_instance_id == 0 ||
        papacc_transport_connection_is_valid(&connection->transport) !=
            PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    connection->state = PAPACC_CONNECTION_STATE_ASSOCIATED;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_connection_manager_init(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_CONNECTION *storage,
    PAPACC_SIZE capacity)
{
    PAPACC_SIZE index;

    if (manager == NULL || (capacity > 0 && storage == NULL)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (manager->initialized == PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    for (index = 0; index < capacity; ++index) {
        storage[index] = (PAPACC_CONNECTION)PAPACC_CONNECTION_INITIALIZER;
    }
    manager->storage = storage;
    manager->capacity = capacity;
    manager->count = 0;
    manager->next_instance_id = 1;
    manager->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_connection_manager_publish(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_TRANSPORT_CONNECTION *transport,
    const PAPACC_NETWORK_ENDPOINT *local_endpoint,
    const PAPACC_NETWORK_ENDPOINT *remote_endpoint,
    PAPACC_CONNECTION **out_connection)
{
    PAPACC_CONNECTION *slot = NULL;
    PAPACC_U64 instance_id;
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (out_connection != NULL) {
        *out_connection = NULL;
    }
    if (manager == NULL || transport == NULL || local_endpoint == NULL ||
        remote_endpoint == NULL || out_connection == NULL ||
        papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_connection_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (papacc_network_endpoint_validate(local_endpoint) != PAPACC_RESULT_OK ||
        papacc_network_endpoint_validate(remote_endpoint) != PAPACC_RESULT_OK ||
        local_endpoint->address.family != remote_endpoint->address.family) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (manager->count >= manager->capacity) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state ==
            PAPACC_CONNECTION_STATE_UNINITIALIZED) {
            slot = &manager->storage[index];
            break;
        }
    }
    if (slot == NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_connection_next_id(manager, &instance_id);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    slot->connection_instance_id = instance_id;
    slot->state = PAPACC_CONNECTION_STATE_PENDING;
    slot->local_endpoint = *local_endpoint;
    slot->remote_endpoint = *remote_endpoint;
    slot->transport = *transport;
    *transport = (PAPACC_TRANSPORT_CONNECTION)
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    ++manager->count;
    *out_connection = slot;
    return PAPACC_RESULT_OK;
}

PAPACC_CONNECTION *papacc_connection_manager_find(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_U64 connection_instance_id)
{
    PAPACC_SIZE index;

    if (papacc_connection_manager_is_valid(manager) != PAPACC_TRUE ||
        connection_instance_id == 0) {
        return NULL;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state !=
                PAPACC_CONNECTION_STATE_UNINITIALIZED &&
            manager->storage[index].connection_instance_id ==
                connection_instance_id) {
            return &manager->storage[index];
        }
    }
    return NULL;
}

PAPACC_RESULT papacc_connection_manager_try_remove(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_U64 connection_instance_id,
    PAPACC_BOOL *out_removed)
{
    PAPACC_CONNECTION *connection;
    PAPACC_BOOL closed = PAPACC_FALSE;
    PAPACC_RESULT result;

    if (out_removed != NULL) {
        *out_removed = PAPACC_FALSE;
    }
    if (manager == NULL || connection_instance_id == 0 ||
        out_removed == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_connection_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    connection = papacc_connection_manager_find(manager, connection_instance_id);
    if (connection == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    result = papacc_connection_try_close(connection, &closed);
    if (result != PAPACC_RESULT_OK || closed != PAPACC_TRUE) {
        return result;
    }
    *connection = (PAPACC_CONNECTION)PAPACC_CONNECTION_INITIALIZER;
    --manager->count;
    *out_removed = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_connection_manager_remove(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_U64 connection_instance_id)
{
    PAPACC_BOOL removed = PAPACC_FALSE;
    PAPACC_RESULT result = papacc_connection_manager_try_remove(
        manager, connection_instance_id, &removed);

    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    return removed == PAPACC_TRUE
        ? PAPACC_RESULT_OK : PAPACC_RESULT_INVALID_STATE;
}

PAPACC_RESULT papacc_connection_manager_try_shutdown(
    PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_BOOL *out_shutdown)
{
    PAPACC_SIZE index;

    if (out_shutdown != NULL) {
        *out_shutdown = PAPACC_FALSE;
    }
    if (manager == NULL || out_shutdown == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_connection_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    for (index = 0; index < manager->capacity; ++index) {
        PAPACC_BOOL closed = PAPACC_FALSE;
        PAPACC_RESULT result;

        if (manager->storage[index].state ==
            PAPACC_CONNECTION_STATE_UNINITIALIZED) {
            continue;
        }
        result = papacc_connection_try_close(
            &manager->storage[index], &closed);
        if (result != PAPACC_RESULT_OK || closed != PAPACC_TRUE) {
            return result;
        }
    }
    for (index = 0; index < manager->capacity; ++index) {
        manager->storage[index] =
            (PAPACC_CONNECTION)PAPACC_CONNECTION_INITIALIZER;
    }
    *manager = (PAPACC_CONNECTION_MANAGER)
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    *out_shutdown = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_connection_manager_shutdown(PAPACC_CONNECTION_MANAGER *manager)
{
    PAPACC_BOOL shutdown;

    (void)papacc_connection_manager_try_shutdown(manager, &shutdown);
}
