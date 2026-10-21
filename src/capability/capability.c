#include "capability.h"

#include <string.h>

PAPACC_RESULT papacc_capability_registry_init(PAPACC_CAPABILITY_REGISTRY *registry,
    PAPACC_CAPABILITY_DESCRIPTOR *storage, PAPACC_SIZE capacity)
{
    if (registry == NULL || (capacity != 0U && storage == NULL))
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (registry->initialized == PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    registry->storage = storage;
    registry->capacity = capacity;
    registry->count = 0U;
    registry->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_capability_registry_register(PAPACC_CAPABILITY_REGISTRY *registry,
    const PAPACC_CAPABILITY_KEY *key, const char *name)
{
    PAPACC_SIZE position = 0U, length;
    if (registry == NULL || registry->initialized != PAPACC_TRUE ||
        papacc_capability_key_valid(key) != PAPACC_TRUE || name == NULL ||
        name[0] == '\0') return PAPACC_RESULT_INVALID_ARGUMENT;
    length = strlen(name);
    if (length >= PAPACC_CAPABILITY_NAME_CAPACITY)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    while (position < registry->count &&
        papacc_capability_key_compare(&registry->storage[position].key, key) < 0)
        ++position;
    if (position < registry->count &&
        papacc_capability_key_compare(&registry->storage[position].key, key) == 0)
        return PAPACC_RESULT_INVALID_STATE;
    if (registry->count == registry->capacity)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    if (position < registry->count)
        memmove(&registry->storage[position + 1U], &registry->storage[position],
            (registry->count - position) * sizeof(registry->storage[0]));
    registry->storage[position].key = *key;
    memcpy(registry->storage[position].name, name, length + 1U);
    ++registry->count;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_capability_registry_find(const PAPACC_CAPABILITY_REGISTRY *registry,
    const PAPACC_CAPABILITY_KEY *key,
    const PAPACC_CAPABILITY_DESCRIPTOR **out_descriptor)
{
    PAPACC_SIZE i;
    if (out_descriptor != NULL) *out_descriptor = NULL;
    if (registry == NULL || registry->initialized != PAPACC_TRUE ||
        papacc_capability_key_valid(key) != PAPACC_TRUE || out_descriptor == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    for (i = 0U; i < registry->count; ++i) {
        int comparison = papacc_capability_key_compare(&registry->storage[i].key, key);
        if (comparison == 0) { *out_descriptor = &registry->storage[i]; return PAPACC_RESULT_OK; }
        if (comparison > 0) break;
    }
    return PAPACC_RESULT_NOT_SUPPORTED;
}

void papacc_capability_registry_release(PAPACC_CAPABILITY_REGISTRY *registry)
{
    if (registry == NULL) return;
    registry->storage = NULL;
    registry->capacity = 0U;
    registry->count = 0U;
    registry->initialized = PAPACC_FALSE;
}

static PAPACC_BOOL papacc_capability_policy_allows(PAPACC_CAPABILITY_POLICY_FN policy,
    void *context, const PAPACC_CAPABILITY_KEY *key)
{
    PAPACC_CAPABILITY_POLICY_DECISION decision = PAPACC_CAPABILITY_POLICY_DENY;
    if (policy == NULL || policy(context, key, &decision) != PAPACC_RESULT_OK)
        return PAPACC_FALSE;
    return decision == PAPACC_CAPABILITY_POLICY_ALLOW ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_capability_evaluate(
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *inputs,
    PAPACC_CAPABILITY_SET *out_effective)
{
    PAPACC_SIZE i; PAPACC_RESULT result;
    if (inputs == NULL || out_effective == NULL ||
        out_effective->initialized != PAPACC_TRUE || inputs->registry == NULL ||
        inputs->registry->initialized != PAPACC_TRUE ||
        inputs->server_supported == NULL || inputs->server_enabled == NULL ||
        inputs->client_supported == NULL || inputs->client_preference == NULL ||
        inputs->principal_policy == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    out_effective->count = 0U;
    for (i = 0U; i < inputs->registry->count; ++i) {
        const PAPACC_CAPABILITY_KEY *key = &inputs->registry->storage[i].key;
        if (papacc_capability_set_contains(inputs->server_supported, key) &&
            papacc_capability_set_contains(inputs->server_enabled, key) &&
            papacc_capability_policy_allows(inputs->principal_policy,
                inputs->principal_policy_context, key) &&
            papacc_capability_set_contains(inputs->client_supported, key) &&
            papacc_capability_set_contains(inputs->client_preference, key)) {
            result = papacc_capability_set_insert(out_effective, key);
            if (result != PAPACC_RESULT_OK) { out_effective->count = 0U; return result; }
        }
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_session_capability_snapshot_publish(
    PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot, PAPACC_U64 session_instance_id,
    PAPACC_CAPABILITY_KEY *storage, PAPACC_SIZE capacity,
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *inputs)
{
    PAPACC_RESULT result;
    if (snapshot == NULL || session_instance_id == 0U || inputs == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (snapshot->published == PAPACC_TRUE || snapshot->effective.initialized == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_capability_set_init(&snapshot->effective, storage, capacity);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_capability_evaluate(inputs, &snapshot->effective);
    if (result != PAPACC_RESULT_OK) { papacc_capability_set_release(&snapshot->effective); return result; }
    snapshot->session_instance_id = session_instance_id;
    snapshot->published = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_session_capability_snapshot_release(PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot)
{
    if (snapshot == NULL) return;
    papacc_capability_set_release(&snapshot->effective);
    snapshot->session_instance_id = 0U;
    snapshot->published = PAPACC_FALSE;
}

PAPACC_BOOL papacc_session_capability_snapshot_contains(
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    const PAPACC_CAPABILITY_KEY *key)
{
    if (snapshot == NULL || snapshot->published != PAPACC_TRUE ||
        snapshot->session_instance_id == 0U) return PAPACC_FALSE;
    return papacc_capability_set_contains(&snapshot->effective, key);
}

PAPACC_BOOL papacc_capability_operation_allowed(
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    const PAPACC_CAPABILITY_KEY *key,
    const PAPACC_CAPABILITY_SET *server_enabled_now,
    PAPACC_CAPABILITY_POLICY_FN contextual_policy,
    void *contextual_policy_context)
{
    if (papacc_session_capability_snapshot_contains(snapshot, key) != PAPACC_TRUE ||
        papacc_capability_set_contains(server_enabled_now, key) != PAPACC_TRUE)
        return PAPACC_FALSE;
    return papacc_capability_policy_allows(contextual_policy,
        contextual_policy_context, key);
}
