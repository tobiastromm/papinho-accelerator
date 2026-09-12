#include "capability.h"

#include <string.h>

#define CHECK(expression, code) do { if (!(expression)) return (code); } while (0)

typedef struct TEST_POLICY {
    PAPACC_CAPABILITY_KEY allowed;
    PAPACC_CAPABILITY_POLICY_DECISION decision;
    PAPACC_RESULT result;
} TEST_POLICY;

static PAPACC_RESULT test_policy(void *context,
    const PAPACC_CAPABILITY_KEY *key,
    PAPACC_CAPABILITY_POLICY_DECISION *decision)
{
    TEST_POLICY *policy = (TEST_POLICY *)context;
    if (policy == NULL || decision == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *decision = papacc_capability_key_equal(&policy->allowed, key) == PAPACC_TRUE ?
        policy->decision : PAPACC_CAPABILITY_POLICY_DENY;
    return policy->result;
}

static int add(PAPACC_CAPABILITY_SET *set, PAPACC_CAPABILITY_KEY key)
{
    return papacc_capability_set_insert(set, &key) == PAPACC_RESULT_OK;
}

int main(void)
{
    const PAPACC_CAPABILITY_KEY invalid_id = { 0U, 1U };
    const PAPACC_CAPABILITY_KEY invalid_version = { 1U, 0U };
    const PAPACC_CAPABILITY_KEY a1 = { 10U, 1U };
    const PAPACC_CAPABILITY_KEY a2 = { 10U, 2U };
    const PAPACC_CAPABILITY_KEY b1 = { 20U, 1U };
    const PAPACC_CAPABILITY_KEY unknown = { 30U, 1U };
    PAPACC_CAPABILITY_DESCRIPTOR descriptors[3];
    PAPACC_CAPABILITY_REGISTRY registry = PAPACC_CAPABILITY_REGISTRY_INITIALIZER;
    const PAPACC_CAPABILITY_DESCRIPTOR *descriptor = NULL;
    PAPACC_CAPABILITY_KEY supported_storage[3], enabled_storage[3];
    PAPACC_CAPABILITY_KEY client_storage[3], preference_storage[3];
    PAPACC_CAPABILITY_KEY effective_storage[3], snapshot_storage[3];
    PAPACC_CAPABILITY_KEY second_snapshot_storage[3];
    PAPACC_CAPABILITY_SET supported = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET enabled = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET client = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET preference = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET effective = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_EVALUATION_INPUTS inputs;
    PAPACC_SESSION_CAPABILITY_SNAPSHOT snapshot =
        PAPACC_SESSION_CAPABILITY_SNAPSHOT_INITIALIZER;
    PAPACC_SESSION_CAPABILITY_SNAPSHOT second =
        PAPACC_SESSION_CAPABILITY_SNAPSHOT_INITIALIZER;
    TEST_POLICY principal_a = { { 10U, 1U }, PAPACC_CAPABILITY_POLICY_ALLOW,
        PAPACC_RESULT_OK };
    TEST_POLICY principal_b = { { 10U, 1U }, PAPACC_CAPABILITY_POLICY_DENY,
        PAPACC_RESULT_OK };
    TEST_POLICY runtime = { { 10U, 1U }, PAPACC_CAPABILITY_POLICY_ALLOW,
        PAPACC_RESULT_OK };
    PAPACC_CAPABILITY_DESCRIPTOR same_name[2];
    PAPACC_CAPABILITY_REGISTRY name_registry = PAPACC_CAPABILITY_REGISTRY_INITIALIZER;
    PAPACC_CAPABILITY_KEY one_storage[1];
    PAPACC_CAPABILITY_SET one = PAPACC_CAPABILITY_SET_INITIALIZER;

    CHECK(papacc_capability_key_valid(&a1) == PAPACC_TRUE &&
        papacc_capability_key_valid(&invalid_id) == PAPACC_FALSE &&
        papacc_capability_key_valid(&invalid_version) == PAPACC_FALSE, 1);
    CHECK(papacc_capability_registry_init(&registry, descriptors, 3U) ==
        PAPACC_RESULT_OK, 2);
    CHECK(papacc_capability_registry_register(&registry, &b1, "B") ==
        PAPACC_RESULT_OK &&
        papacc_capability_registry_register(&registry, &a2, "A-v2") ==
        PAPACC_RESULT_OK &&
        papacc_capability_registry_register(&registry, &a1, "A-v1") ==
        PAPACC_RESULT_OK, 3);
    CHECK(registry.storage[0].key.major == 1U &&
        registry.storage[1].key.major == 2U &&
        registry.storage[2].key.id == 20U, 4);
    CHECK(papacc_capability_registry_register(&registry, &a1, "duplicate") ==
        PAPACC_RESULT_INVALID_STATE &&
        papacc_capability_registry_register(&registry, &unknown, "full") ==
        PAPACC_RESULT_LIMIT_EXCEEDED, 5);
    CHECK(papacc_capability_registry_find(&registry, &unknown, &descriptor) ==
        PAPACC_RESULT_NOT_SUPPORTED && descriptor == NULL, 6);
    CHECK(papacc_capability_registry_find(&registry, &a1, &descriptor) ==
        PAPACC_RESULT_OK && strcmp(descriptor->name, "A-v1") == 0, 7);
    CHECK(papacc_capability_registry_init(&name_registry, same_name, 2U) ==
        PAPACC_RESULT_OK &&
        papacc_capability_registry_register(&name_registry, &a1, "same") ==
        PAPACC_RESULT_OK &&
        papacc_capability_registry_register(&name_registry, &b1, "same") ==
        PAPACC_RESULT_OK && name_registry.count == 2U, 8);

    CHECK(papacc_capability_set_init(&supported, supported_storage, 3U) ==
        PAPACC_RESULT_OK &&
        papacc_capability_set_init(&enabled, enabled_storage, 3U) ==
        PAPACC_RESULT_OK &&
        papacc_capability_set_init(&client, client_storage, 3U) ==
        PAPACC_RESULT_OK &&
        papacc_capability_set_init(&preference, preference_storage, 3U) ==
        PAPACC_RESULT_OK &&
        papacc_capability_set_init(&effective, effective_storage, 3U) ==
        PAPACC_RESULT_OK, 9);
    CHECK(add(&supported, b1) && add(&supported, a1) && add(&supported, a2) &&
        supported.storage[0].id == 10U && supported.storage[0].major == 1U, 10);
    CHECK(papacc_capability_set_insert(&supported, &a1) ==
        PAPACC_RESULT_INVALID_STATE, 11);
    CHECK(papacc_capability_set_init(&one, one_storage, 1U) == PAPACC_RESULT_OK &&
        add(&one, a1) && papacc_capability_set_insert(&one, &b1) ==
        PAPACC_RESULT_LIMIT_EXCEEDED, 12);
    CHECK(add(&enabled, a1) && add(&client, a1) && add(&preference, a1), 13);

    memset(&inputs, 0, sizeof(inputs));
    inputs.registry = &registry;
    inputs.server_supported = &supported;
    inputs.server_enabled = &enabled;
    inputs.principal_policy = test_policy;
    inputs.principal_policy_context = &principal_a;
    inputs.client_supported = &client;
    inputs.client_preference = &preference;
    CHECK(papacc_capability_evaluate(&inputs, &effective) == PAPACC_RESULT_OK &&
        effective.count == 1U && papacc_capability_set_contains(&effective, &a1) &&
        !papacc_capability_set_contains(&effective, &unknown), 14);
    CHECK(papacc_capability_evaluate(&inputs, &effective) == PAPACC_RESULT_OK &&
        effective.count == 1U, 15);

    inputs.principal_policy_context = &principal_b;
    CHECK(papacc_capability_evaluate(&inputs, &effective) == PAPACC_RESULT_OK &&
        effective.count == 0U, 16);
    principal_b.decision = PAPACC_CAPABILITY_POLICY_ERROR;
    CHECK(papacc_capability_evaluate(&inputs, &effective) == PAPACC_RESULT_OK &&
        effective.count == 0U, 17);
    principal_b.decision = PAPACC_CAPABILITY_POLICY_ALLOW;
    principal_b.result = PAPACC_RESULT_INTERNAL_ERROR;
    CHECK(papacc_capability_evaluate(&inputs, &effective) == PAPACC_RESULT_OK &&
        effective.count == 0U, 18);
    inputs.principal_policy_context = &principal_a;

    CHECK(papacc_session_capability_snapshot_publish(&snapshot, 101U,
        snapshot_storage, 3U, &inputs) == PAPACC_RESULT_OK &&
        papacc_session_capability_snapshot_contains(&snapshot, &a1), 19);
    CHECK(papacc_session_capability_snapshot_publish(&snapshot, 101U,
        snapshot_storage, 3U, &inputs) == PAPACC_RESULT_INVALID_STATE, 20);
    CHECK(papacc_capability_operation_allowed(&snapshot, &a1, &enabled,
        test_policy, &runtime) == PAPACC_TRUE, 21);
    enabled.count = 0U;
    CHECK(papacc_capability_operation_allowed(&snapshot, &a1, &enabled,
        test_policy, &runtime) == PAPACC_FALSE, 22);
    CHECK(add(&enabled, a1), 23);
    runtime.decision = PAPACC_CAPABILITY_POLICY_DENY;
    CHECK(papacc_capability_operation_allowed(&snapshot, &a1, &enabled,
        test_policy, &runtime) == PAPACC_FALSE, 24);
    runtime.decision = PAPACC_CAPABILITY_POLICY_ALLOW;
    CHECK(papacc_capability_operation_allowed(&snapshot, &a1, &enabled,
        test_policy, &runtime) == PAPACC_TRUE, 25);
    CHECK(add(&enabled, b1) &&
        papacc_capability_operation_allowed(&snapshot, &b1, &enabled,
            test_policy, &runtime) == PAPACC_FALSE, 26);
    CHECK(papacc_capability_operation_allowed(NULL, &a1, &enabled,
        test_policy, &runtime) == PAPACC_FALSE, 27);

    inputs.principal_policy_context = &principal_b;
    CHECK(papacc_session_capability_snapshot_publish(&second, 102U,
        second_snapshot_storage, 3U, &inputs) == PAPACC_RESULT_OK &&
        second.effective.count == 0U && snapshot.effective.count == 1U, 28);
    papacc_session_capability_snapshot_release(&snapshot);
    CHECK(snapshot.published == PAPACC_FALSE && snapshot.session_instance_id == 0U &&
        papacc_session_capability_snapshot_contains(&snapshot, &a1) ==
            PAPACC_FALSE, 29);
    papacc_session_capability_snapshot_release(&second);
    return 0;
}
