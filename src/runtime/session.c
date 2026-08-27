#include "session.h"

static PAPACC_BOOL papacc_session_manager_is_valid(
    const PAPACC_SESSION_MANAGER *manager)
{
    return (manager != NULL && manager->initialized == PAPACC_TRUE &&
            manager->count <= manager->capacity &&
            (manager->capacity == 0 || manager->storage != NULL))
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

static PAPACC_BOOL papacc_session_id_exists(
    const PAPACC_SESSION_MANAGER *manager,
    PAPACC_U64 instance_id)
{
    PAPACC_SIZE index;
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state !=
                PAPACC_SESSION_STATE_UNINITIALIZED &&
            manager->storage[index].session_instance_id == instance_id) {
            return PAPACC_TRUE;
        }
    }
    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_session_next_id(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_U64 *out_instance_id)
{
    PAPACC_U64 candidate = manager->next_instance_id;
    PAPACC_SIZE collisions = 0;

    if (candidate == 0) {
        candidate = 1;
    }
    while (papacc_session_id_exists(manager, candidate) == PAPACC_TRUE) {
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

PAPACC_RESULT papacc_session_activate(PAPACC_SESSION *session)
{
    if (session == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (session->state != PAPACC_SESSION_STATE_ESTABLISHING ||
        session->session_instance_id == 0) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    session->state = PAPACC_SESSION_STATE_ACTIVE;
    return PAPACC_RESULT_OK;
}

void papacc_session_close(PAPACC_SESSION *session)
{
    if (session == NULL ||
        session->state == PAPACC_SESSION_STATE_UNINITIALIZED ||
        session->state == PAPACC_SESSION_STATE_CLOSED) {
        return;
    }
    if (session->state != PAPACC_SESSION_STATE_ESTABLISHING &&
        session->state != PAPACC_SESSION_STATE_ACTIVE &&
        session->state != PAPACC_SESSION_STATE_CLOSING) {
        return;
    }
    session->state = PAPACC_SESSION_STATE_CLOSING;
    session->state = PAPACC_SESSION_STATE_CLOSED;
}

PAPACC_RESULT papacc_session_manager_init(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_SESSION *storage,
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
        storage[index] = (PAPACC_SESSION)PAPACC_SESSION_INITIALIZER;
    }
    manager->storage = storage;
    manager->capacity = capacity;
    manager->count = 0;
    manager->next_instance_id = 1;
    manager->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_session_manager_publish(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_SESSION **out_session)
{
    PAPACC_SESSION *slot = NULL;
    PAPACC_U64 instance_id;
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (out_session != NULL) {
        *out_session = NULL;
    }
    if (manager == NULL || out_session == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_session_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (manager->count >= manager->capacity) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state ==
            PAPACC_SESSION_STATE_UNINITIALIZED) {
            slot = &manager->storage[index];
            break;
        }
    }
    if (slot == NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_session_next_id(manager, &instance_id);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    slot->session_instance_id = instance_id;
    slot->state = PAPACC_SESSION_STATE_ESTABLISHING;
    ++manager->count;
    *out_session = slot;
    return PAPACC_RESULT_OK;
}

PAPACC_SESSION *papacc_session_manager_find(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_U64 session_instance_id)
{
    PAPACC_SIZE index;

    if (papacc_session_manager_is_valid(manager) != PAPACC_TRUE ||
        session_instance_id == 0) {
        return NULL;
    }
    for (index = 0; index < manager->capacity; ++index) {
        if (manager->storage[index].state !=
                PAPACC_SESSION_STATE_UNINITIALIZED &&
            manager->storage[index].session_instance_id ==
                session_instance_id) {
            return &manager->storage[index];
        }
    }
    return NULL;
}

PAPACC_RESULT papacc_session_manager_remove(
    PAPACC_SESSION_MANAGER *manager,
    PAPACC_U64 session_instance_id)
{
    PAPACC_SESSION *session;

    if (manager == NULL || session_instance_id == 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_session_manager_is_valid(manager) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    session = papacc_session_manager_find(manager, session_instance_id);
    if (session == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    papacc_session_close(session);
    *session = (PAPACC_SESSION)PAPACC_SESSION_INITIALIZER;
    --manager->count;
    return PAPACC_RESULT_OK;
}

void papacc_session_manager_shutdown(PAPACC_SESSION_MANAGER *manager)
{
    PAPACC_SIZE index;

    if (papacc_session_manager_is_valid(manager) != PAPACC_TRUE) {
        return;
    }
    for (index = 0; index < manager->capacity; ++index) {
        papacc_session_close(&manager->storage[index]);
        manager->storage[index] =
            (PAPACC_SESSION)PAPACC_SESSION_INITIALIZER;
    }
    *manager = (PAPACC_SESSION_MANAGER)
        PAPACC_SESSION_MANAGER_INITIALIZER;
}
