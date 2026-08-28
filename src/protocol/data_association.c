#include "data_association.h"

static PAPACC_BOOL papacc_data_association_manager_valid(
    const PAPACC_DATA_ASSOCIATION_MANAGER *manager)
{
    return manager != NULL && manager->initialized == PAPACC_TRUE &&
        manager->count <= manager->capacity &&
        (manager->capacity == 0 || manager->storage != NULL) &&
        manager->session_manager != NULL &&
        manager->session_manager->initialized == PAPACC_TRUE &&
        manager->channel_manager != NULL &&
        manager->channel_manager->initialized == PAPACC_TRUE &&
        manager->channel_manager->session_manager == manager->session_manager &&
        manager->generate_fn != NULL && manager->ticket_lifetime_ns > 0
        ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_BOOL papacc_data_association_lifecycle_valid(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 session_id)
{
    PAPACC_SESSION *session = papacc_session_manager_find(
        manager->session_manager, session_id);
    PAPACC_CHANNEL *control = papacc_channel_manager_find_control(
        manager->channel_manager, session_id);
    return session != NULL && session->state == PAPACC_SESSION_STATE_ACTIVE &&
        control != NULL && control->state == PAPACC_CHANNEL_STATE_BOUND
        ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_DATA_ASSOCIATION_ENTRY *papacc_data_association_find_session(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 session_id)
{
    PAPACC_SIZE index;
    for (index = 0; index < manager->capacity; ++index)
        if (manager->storage[index].in_use == PAPACC_TRUE &&
            manager->storage[index].session_instance_id == session_id)
            return &manager->storage[index];
    return NULL;
}

static PAPACC_DATA_ASSOCIATION_ENTRY *papacc_data_association_find_ticket(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket)
{
    PAPACC_SIZE index;
    for (index = 0; index < manager->capacity; ++index)
        if (manager->storage[index].in_use == PAPACC_TRUE &&
            papacc_data_association_ticket_equal(
                &manager->storage[index].ticket, ticket) == PAPACC_TRUE)
            return &manager->storage[index];
    return NULL;
}

static void papacc_data_association_clear(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager,
    PAPACC_DATA_ASSOCIATION_ENTRY *entry)
{
    *entry = (PAPACC_DATA_ASSOCIATION_ENTRY)
        PAPACC_DATA_ASSOCIATION_ENTRY_INITIALIZER;
    --manager->count;
}

static PAPACC_U64 papacc_data_association_deadline(
    PAPACC_U64 now_ns, PAPACC_U64 lifetime_ns)
{
    const PAPACC_U64 maximum = (PAPACC_U64)(~(PAPACC_U64)0);
    return now_ns > maximum - lifetime_ns ? maximum : now_ns + lifetime_ns;
}

PAPACC_RESULT papacc_data_association_manager_init(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager,
    PAPACC_DATA_ASSOCIATION_ENTRY *storage, PAPACC_SIZE capacity,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_TICKET_GENERATE_FN generate_fn, void *generate_context,
    PAPACC_U64 ticket_lifetime_ns)
{
    PAPACC_SIZE index;
    if (manager == NULL || session_manager == NULL || channel_manager == NULL ||
        generate_fn == NULL || ticket_lifetime_ns == 0 ||
        (capacity > 0 && storage == NULL)) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (manager->initialized == PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    if (session_manager->initialized != PAPACC_TRUE ||
        channel_manager->initialized != PAPACC_TRUE ||
        channel_manager->session_manager != session_manager ||
        channel_manager->connection_manager == NULL ||
        channel_manager->connection_manager->initialized != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    for (index = 0; index < capacity; ++index)
        storage[index] = (PAPACC_DATA_ASSOCIATION_ENTRY)
            PAPACC_DATA_ASSOCIATION_ENTRY_INITIALIZER;
    manager->storage = storage;
    manager->capacity = capacity;
    manager->count = 0;
    manager->session_manager = session_manager;
    manager->channel_manager = channel_manager;
    manager->generate_fn = generate_fn;
    manager->generate_context = generate_context;
    manager->ticket_lifetime_ns = ticket_lifetime_ns;
    manager->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_data_association_manager_issue(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 session_instance_id,
    PAPACC_U64 now_ns, PAPACC_DATA_ASSOCIATION_TICKET *out_ticket,
    PAPACC_U64 *out_deadline_ns)
{
    PAPACC_DATA_ASSOCIATION_TICKET empty =
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_ENTRY *entry;
    PAPACC_SIZE index;
    PAPACC_SIZE attempt;
    PAPACC_RESULT result;
    if (out_ticket != NULL) *out_ticket = empty;
    if (out_deadline_ns != NULL) *out_deadline_ns = 0;
    if (manager == NULL || session_instance_id == 0 || out_ticket == NULL ||
        out_deadline_ns == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_data_association_manager_valid(manager) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (papacc_data_association_lifecycle_valid(
            manager, session_instance_id) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    entry = papacc_data_association_find_session(manager, session_instance_id);
    if (entry != NULL && now_ns < entry->deadline_ns) {
        *out_ticket = entry->ticket;
        *out_deadline_ns = entry->deadline_ns;
        return PAPACC_RESULT_OK;
    }
    if (entry != NULL) papacc_data_association_clear(manager, entry);
    if (manager->count >= manager->capacity)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    entry = NULL;
    for (index = 0; index < manager->capacity; ++index)
        if (manager->storage[index].in_use == PAPACC_FALSE) {
            entry = &manager->storage[index];
            break;
        }
    if (entry == NULL) return PAPACC_RESULT_INVALID_STATE;
    for (attempt = 0; attempt < PAPACC_DATA_ASSOCIATION_GENERATION_ATTEMPTS;
         ++attempt) {
        PAPACC_DATA_ASSOCIATION_TICKET candidate =
            PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
        result = manager->generate_fn(manager->generate_context, &candidate);
        if (result != PAPACC_RESULT_OK) return result;
        if (papacc_data_association_ticket_is_valid(&candidate) == PAPACC_TRUE &&
            papacc_data_association_find_ticket(manager, &candidate) == NULL) {
            entry->in_use = PAPACC_TRUE;
            entry->session_instance_id = session_instance_id;
            entry->ticket = candidate;
            entry->deadline_ns = papacc_data_association_deadline(
                now_ns, manager->ticket_lifetime_ns);
            ++manager->count;
            *out_ticket = candidate;
            *out_deadline_ns = entry->deadline_ns;
            return PAPACC_RESULT_OK;
        }
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

PAPACC_RESULT papacc_data_association_manager_consume(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U64 now_ns,
    PAPACC_U64 *out_session_instance_id)
{
    PAPACC_DATA_ASSOCIATION_ENTRY *entry;
    PAPACC_U64 session_id;
    if (out_session_instance_id != NULL) *out_session_instance_id = 0;
    if (manager == NULL || ticket == NULL || out_session_instance_id == NULL ||
        papacc_data_association_ticket_is_valid(ticket) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_data_association_manager_valid(manager) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    entry = papacc_data_association_find_ticket(manager, ticket);
    if (entry == NULL) return PAPACC_RESULT_INVALID_STATE;
    session_id = entry->session_instance_id;
    if (now_ns >= entry->deadline_ns ||
        papacc_data_association_lifecycle_valid(manager, session_id) !=
            PAPACC_TRUE) {
        papacc_data_association_clear(manager, entry);
        return PAPACC_RESULT_INVALID_STATE;
    }
    papacc_data_association_clear(manager, entry);
    *out_session_instance_id = session_id;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_data_association_manager_invalidate_session(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 session_instance_id)
{
    PAPACC_DATA_ASSOCIATION_ENTRY *entry;
    if (manager == NULL || session_instance_id == 0)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_data_association_manager_valid(manager) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    entry = papacc_data_association_find_session(manager, session_instance_id);
    if (entry != NULL) papacc_data_association_clear(manager, entry);
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_data_association_manager_expire(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 now_ns,
    PAPACC_SIZE *out_expired_count)
{
    PAPACC_SIZE index;
    if (out_expired_count != NULL) *out_expired_count = 0;
    if (manager == NULL || out_expired_count == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_data_association_manager_valid(manager) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    for (index = 0; index < manager->capacity; ++index) {
        PAPACC_DATA_ASSOCIATION_ENTRY *entry = &manager->storage[index];
        if (entry->in_use == PAPACC_TRUE &&
            (now_ns >= entry->deadline_ns ||
             papacc_data_association_lifecycle_valid(
                 manager, entry->session_instance_id) != PAPACC_TRUE)) {
            papacc_data_association_clear(manager, entry);
            ++*out_expired_count;
        }
    }
    return PAPACC_RESULT_OK;
}

void papacc_data_association_manager_shutdown(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager)
{
    PAPACC_SIZE index;
    if (manager == NULL || manager->initialized != PAPACC_TRUE) return;
    for (index = 0; index < manager->capacity; ++index)
        manager->storage[index] = (PAPACC_DATA_ASSOCIATION_ENTRY)
            PAPACC_DATA_ASSOCIATION_ENTRY_INITIALIZER;
    *manager = (PAPACC_DATA_ASSOCIATION_MANAGER)
        PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER;
}
