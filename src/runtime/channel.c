#include "channel.h"

static PAPACC_BOOL papacc_channel_manager_is_valid(
    const PAPACC_CHANNEL_MANAGER *manager)
{
    return (manager != NULL && manager->initialized == PAPACC_TRUE &&
            manager->count <= manager->capacity &&
            (manager->capacity == 0 || manager->storage != NULL) &&
            manager->connection_manager != NULL &&
            manager->connection_manager->initialized == PAPACC_TRUE &&
            manager->session_manager != NULL &&
            manager->session_manager->initialized == PAPACC_TRUE)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

static PAPACC_BOOL papacc_channel_id_exists(
    const PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 instance_id)
{
    PAPACC_SIZE index;
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state !=
                PAPACC_CHANNEL_STATE_UNINITIALIZED &&
            manager->storage[index].channel_instance_id == instance_id) {
            return PAPACC_TRUE;
        }
    }
    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_channel_next_id(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 *out_instance_id)
{
    PAPACC_U64 candidate = manager->next_instance_id;
    PAPACC_SIZE collisions = 0;

    if (candidate == 0) {
        candidate = 1;
    }
    while (papacc_channel_id_exists(manager, candidate) == PAPACC_TRUE) {
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

PAPACC_RESULT papacc_channel_manager_init(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_CHANNEL *storage,
    PAPACC_SIZE capacity,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager)
{
    PAPACC_SIZE index;

    if (manager == NULL || connection_manager == NULL ||
        session_manager == NULL || (capacity > 0 && storage == NULL)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (manager->initialized == PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (connection_manager->initialized != PAPACC_TRUE ||
        session_manager->initialized != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    for (index = 0; index < capacity; ++index) {
        storage[index] = (PAPACC_CHANNEL)PAPACC_CHANNEL_INITIALIZER;
    }
    manager->storage = storage;
    manager->capacity = capacity;
    manager->count = 0;
    manager->next_instance_id = 1;
    manager->connection_manager = connection_manager;
    manager->session_manager = session_manager;
    manager->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_CHANNEL *papacc_channel_manager_find(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 channel_instance_id)
{
    PAPACC_SIZE index;
    if (papacc_channel_manager_is_valid(manager) != PAPACC_TRUE ||
        channel_instance_id == 0) {
        return NULL;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state !=
                PAPACC_CHANNEL_STATE_UNINITIALIZED &&
            manager->storage[index].channel_instance_id ==
                channel_instance_id) {
            return &manager->storage[index];
        }
    }
    return NULL;
}

PAPACC_CHANNEL *papacc_channel_manager_find_by_connection(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 connection_instance_id)
{
    PAPACC_SIZE index;
    if (papacc_channel_manager_is_valid(manager) != PAPACC_TRUE ||
        connection_instance_id == 0) {
        return NULL;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state !=
                PAPACC_CHANNEL_STATE_UNINITIALIZED &&
            manager->storage[index].connection_instance_id ==
                connection_instance_id) {
            return &manager->storage[index];
        }
    }
    return NULL;
}

PAPACC_CHANNEL *papacc_channel_manager_find_control(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 session_instance_id)
{
    PAPACC_SIZE index;
    if (papacc_channel_manager_is_valid(manager) != PAPACC_TRUE ||
        session_instance_id == 0) {
        return NULL;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state == PAPACC_CHANNEL_STATE_BOUND &&
            manager->storage[index].role == PAPACC_CHANNEL_ROLE_CONTROL &&
            manager->storage[index].session_instance_id ==
                session_instance_id) {
            return &manager->storage[index];
        }
    }
    return NULL;
}

PAPACC_RESULT papacc_channel_manager_bind(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 session_instance_id,
    PAPACC_U64 connection_instance_id,
    PAPACC_CHANNEL_ROLE role,
    PAPACC_CHANNEL **out_channel)
{
    PAPACC_CHANNEL *slot = NULL;
    PAPACC_CONNECTION *connection;
    PAPACC_SESSION *session;
    PAPACC_U64 channel_instance_id;
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (out_channel != NULL) {
        *out_channel = NULL;
    }
    if (manager == NULL || out_channel == NULL || session_instance_id == 0 ||
        connection_instance_id == 0 ||
        (role != PAPACC_CHANNEL_ROLE_CONTROL &&
         role != PAPACC_CHANNEL_ROLE_DATA)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_channel_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    session = papacc_session_manager_find(
        manager->session_manager, session_instance_id);
    connection = papacc_connection_manager_find(
        manager->connection_manager, connection_instance_id);
    if (session == NULL || connection == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (connection->state != PAPACC_CONNECTION_STATE_PENDING ||
        papacc_channel_manager_find_by_connection(
            manager, connection_instance_id) != NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (role == PAPACC_CHANNEL_ROLE_CONTROL) {
        if (session->state != PAPACC_SESSION_STATE_ESTABLISHING ||
            papacc_channel_manager_find_control(
                manager, session_instance_id) != NULL) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    } else if (session->state != PAPACC_SESSION_STATE_ACTIVE ||
               papacc_channel_manager_find_control(
                   manager, session_instance_id) == NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (manager->count >= manager->capacity) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state ==
            PAPACC_CHANNEL_STATE_UNINITIALIZED) {
            slot = &manager->storage[index];
            break;
        }
    }
    if (slot == NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_channel_next_id(manager, &channel_instance_id);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    result = papacc_connection_mark_associated(connection);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    slot->channel_instance_id = channel_instance_id;
    slot->state = PAPACC_CHANNEL_STATE_BOUND;
    slot->role = role;
    slot->session_instance_id = session_instance_id;
    slot->connection_instance_id = connection_instance_id;
    ++manager->count;
    *out_channel = slot;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_channel_validate_bound_connection(
    PAPACC_CHANNEL_MANAGER *manager,
    const PAPACC_CHANNEL *channel,
    PAPACC_CONNECTION **out_connection)
{
    PAPACC_CONNECTION *connection = papacc_connection_manager_find(
        manager->connection_manager, channel->connection_instance_id);
    if (connection == NULL ||
        connection->state != PAPACC_CONNECTION_STATE_ASSOCIATED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    *out_connection = connection;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_channel_manager_close(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 channel_instance_id)
{
    PAPACC_CHANNEL *channel;
    PAPACC_CONNECTION *connection;
    PAPACC_SESSION *session;
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (manager == NULL || channel_instance_id == 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_channel_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    channel = papacc_channel_manager_find(manager, channel_instance_id);
    if (channel == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (channel->state == PAPACC_CHANNEL_STATE_CLOSED) {
        return PAPACC_RESULT_OK;
    }
    if (channel->state != PAPACC_CHANNEL_STATE_BOUND) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_channel_validate_bound_connection(
        manager, channel, &connection);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    session = papacc_session_manager_find(
        manager->session_manager, channel->session_instance_id);
    if (session == NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (channel->role == PAPACC_CHANNEL_ROLE_DATA) {
        if (session->state != PAPACC_SESSION_STATE_ACTIVE) {
            return PAPACC_RESULT_INVALID_STATE;
        }
        channel->state = PAPACC_CHANNEL_STATE_CLOSING;
        papacc_connection_close(connection);
        channel->state = PAPACC_CHANNEL_STATE_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (channel->role != PAPACC_CHANNEL_ROLE_CONTROL ||
        (session->state != PAPACC_SESSION_STATE_ESTABLISHING &&
         session->state != PAPACC_SESSION_STATE_ACTIVE)) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    for (index = 0; index < manager->capacity; ++index) {
        PAPACC_CHANNEL *related = &manager->storage[index];
        PAPACC_CONNECTION *related_connection;
        if (related->state == PAPACC_CHANNEL_STATE_BOUND &&
            related->role == PAPACC_CHANNEL_ROLE_DATA &&
            related->session_instance_id == channel->session_instance_id &&
            papacc_channel_validate_bound_connection(
                manager, related, &related_connection) != PAPACC_RESULT_OK) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    }
    channel->state = PAPACC_CHANNEL_STATE_CLOSING;
    papacc_session_close(session);
    for (index = 0; index < manager->capacity; ++index) {
        PAPACC_CHANNEL *related = &manager->storage[index];
        if (related->state == PAPACC_CHANNEL_STATE_BOUND &&
            related->role == PAPACC_CHANNEL_ROLE_DATA &&
            related->session_instance_id == channel->session_instance_id) {
            PAPACC_CONNECTION *related_connection =
                papacc_connection_manager_find(
                    manager->connection_manager,
                    related->connection_instance_id);
            related->state = PAPACC_CHANNEL_STATE_CLOSING;
            papacc_connection_close(related_connection);
            related->state = PAPACC_CHANNEL_STATE_CLOSED;
        }
    }
    papacc_connection_close(connection);
    channel->state = PAPACC_CHANNEL_STATE_CLOSED;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_channel_manager_remove(
    PAPACC_CHANNEL_MANAGER *manager,
    PAPACC_U64 channel_instance_id)
{
    PAPACC_CHANNEL *channel;
    PAPACC_RESULT result;

    if (manager == NULL || channel_instance_id == 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_channel_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    channel = papacc_channel_manager_find(manager, channel_instance_id);
    if (channel == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    result = papacc_channel_manager_close(manager, channel_instance_id);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    *channel = (PAPACC_CHANNEL)PAPACC_CHANNEL_INITIALIZER;
    --manager->count;
    return PAPACC_RESULT_OK;
}

void papacc_channel_manager_shutdown(PAPACC_CHANNEL_MANAGER *manager)
{
    PAPACC_SIZE index;

    if (papacc_channel_manager_is_valid(manager) != PAPACC_TRUE) {
        return;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state == PAPACC_CHANNEL_STATE_BOUND &&
            manager->storage[index].role == PAPACC_CHANNEL_ROLE_CONTROL) {
            (void)papacc_channel_manager_close(
                manager, manager->storage[index].channel_instance_id);
        }
    }
    for (index = 0; index < manager->capacity; ++index) {
        PAPACC_CHANNEL *channel = &manager->storage[index];
        if (channel->state == PAPACC_CHANNEL_STATE_BOUND) {
            (void)papacc_channel_manager_close(
                manager, channel->channel_instance_id);
        }
        *channel = (PAPACC_CHANNEL)PAPACC_CHANNEL_INITIALIZER;
    }
    *manager = (PAPACC_CHANNEL_MANAGER)PAPACC_CHANNEL_MANAGER_INITIALIZER;
}
