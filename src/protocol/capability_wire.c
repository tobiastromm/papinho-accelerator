#include <papacc/pacc/capability_wire.h>
#include <string.h>

PAPACC_BOOL papacc_capability_key_valid(const PAPACC_CAPABILITY_KEY *key)
{
    return key != NULL && key->id != 0U && key->major != 0U ?
        PAPACC_TRUE : PAPACC_FALSE;
}

int papacc_capability_key_compare(const PAPACC_CAPABILITY_KEY *a,
    const PAPACC_CAPABILITY_KEY *b)
{
    if (a->id < b->id) return -1;
    if (a->id > b->id) return 1;
    if (a->major < b->major) return -1;
    if (a->major > b->major) return 1;
    return 0;
}

PAPACC_BOOL papacc_capability_key_equal(const PAPACC_CAPABILITY_KEY *a,
    const PAPACC_CAPABILITY_KEY *b)
{
    if (papacc_capability_key_valid(a) != PAPACC_TRUE ||
        papacc_capability_key_valid(b) != PAPACC_TRUE) return PAPACC_FALSE;
    return papacc_capability_key_compare(a, b) == 0 ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_SIZE papacc_capability_position(const PAPACC_CAPABILITY_KEY *storage,
    PAPACC_SIZE count, const PAPACC_CAPABILITY_KEY *key, PAPACC_BOOL *found)
{
    PAPACC_SIZE position = 0U;
    while (position < count &&
        papacc_capability_key_compare(&storage[position], key) < 0) ++position;
    *found = position < count &&
        papacc_capability_key_compare(&storage[position], key) == 0 ?
        PAPACC_TRUE : PAPACC_FALSE;
    return position;
}

PAPACC_RESULT papacc_capability_set_init(PAPACC_CAPABILITY_SET *set,
    PAPACC_CAPABILITY_KEY *storage, PAPACC_SIZE capacity)
{
    if (set == NULL || (capacity != 0U && storage == NULL))
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (set->initialized == PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    set->storage = storage; set->capacity = capacity; set->count = 0U;
    set->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_capability_set_insert(PAPACC_CAPABILITY_SET *set,
    const PAPACC_CAPABILITY_KEY *key)
{
    PAPACC_BOOL found; PAPACC_SIZE position;
    if (set == NULL || set->initialized != PAPACC_TRUE ||
        papacc_capability_key_valid(key) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    position = papacc_capability_position(set->storage, set->count, key, &found);
    if (found == PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    if (set->count == set->capacity) return PAPACC_RESULT_LIMIT_EXCEEDED;
    if (position < set->count)
        memmove(&set->storage[position + 1U], &set->storage[position],
            (set->count - position) * sizeof(set->storage[0]));
    set->storage[position] = *key; ++set->count;
    return PAPACC_RESULT_OK;
}

PAPACC_BOOL papacc_capability_set_contains(const PAPACC_CAPABILITY_SET *set,
    const PAPACC_CAPABILITY_KEY *key)
{
    PAPACC_BOOL found;
    if (set == NULL || set->initialized != PAPACC_TRUE ||
        papacc_capability_key_valid(key) != PAPACC_TRUE) return PAPACC_FALSE;
    (void)papacc_capability_position(set->storage, set->count, key, &found);
    return found;
}

void papacc_capability_set_release(PAPACC_CAPABILITY_SET *set)
{
    if (set == NULL) return;
    set->storage = NULL;
    set->capacity = 0U;
    set->count = 0U;
    set->initialized = PAPACC_FALSE;
}
