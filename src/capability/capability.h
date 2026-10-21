#ifndef PAPACC_CAPABILITY_H
#define PAPACC_CAPABILITY_H

#include <papacc/types.h>
#include <papacc/pacc/capability_wire.h>

#define PAPACC_CAPABILITY_NAME_CAPACITY 48U

typedef struct PAPACC_CAPABILITY_DESCRIPTOR {
    PAPACC_CAPABILITY_KEY key;
    char name[PAPACC_CAPABILITY_NAME_CAPACITY];
} PAPACC_CAPABILITY_DESCRIPTOR;

typedef struct PAPACC_CAPABILITY_REGISTRY {
    PAPACC_CAPABILITY_DESCRIPTOR *storage;
    PAPACC_SIZE capacity;
    PAPACC_SIZE count;
    PAPACC_BOOL initialized;
} PAPACC_CAPABILITY_REGISTRY;

#define PAPACC_CAPABILITY_REGISTRY_INITIALIZER { NULL, 0U, 0U, PAPACC_FALSE }

typedef enum PAPACC_CAPABILITY_POLICY_DECISION {
    PAPACC_CAPABILITY_POLICY_DENY = 0,
    PAPACC_CAPABILITY_POLICY_ALLOW = 1,
    PAPACC_CAPABILITY_POLICY_ERROR = 2
} PAPACC_CAPABILITY_POLICY_DECISION;

typedef PAPACC_RESULT (*PAPACC_CAPABILITY_POLICY_FN)(void *,
    const PAPACC_CAPABILITY_KEY *, PAPACC_CAPABILITY_POLICY_DECISION *);

typedef struct PAPACC_CAPABILITY_EVALUATION_INPUTS {
    const PAPACC_CAPABILITY_REGISTRY *registry;
    const PAPACC_CAPABILITY_SET *server_supported;
    const PAPACC_CAPABILITY_SET *server_enabled;
    PAPACC_CAPABILITY_POLICY_FN principal_policy;
    void *principal_policy_context;
    const PAPACC_CAPABILITY_SET *client_supported;
    const PAPACC_CAPABILITY_SET *client_preference;
} PAPACC_CAPABILITY_EVALUATION_INPUTS;

typedef struct PAPACC_SESSION_CAPABILITY_SNAPSHOT {
    PAPACC_U64 session_instance_id;
    PAPACC_CAPABILITY_SET effective;
    PAPACC_BOOL published;
} PAPACC_SESSION_CAPABILITY_SNAPSHOT;

#define PAPACC_SESSION_CAPABILITY_SNAPSHOT_INITIALIZER \
    { 0U, PAPACC_CAPABILITY_SET_INITIALIZER, PAPACC_FALSE }

PAPACC_RESULT papacc_capability_registry_init(PAPACC_CAPABILITY_REGISTRY *registry,
    PAPACC_CAPABILITY_DESCRIPTOR *storage, PAPACC_SIZE capacity);
PAPACC_RESULT papacc_capability_registry_register(PAPACC_CAPABILITY_REGISTRY *registry,
    const PAPACC_CAPABILITY_KEY *key, const char *name);
PAPACC_RESULT papacc_capability_registry_find(const PAPACC_CAPABILITY_REGISTRY *registry,
    const PAPACC_CAPABILITY_KEY *key,
    const PAPACC_CAPABILITY_DESCRIPTOR **out_descriptor);
void papacc_capability_registry_release(PAPACC_CAPABILITY_REGISTRY *registry);

PAPACC_RESULT papacc_capability_evaluate(
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *inputs,
    PAPACC_CAPABILITY_SET *out_effective);
PAPACC_RESULT papacc_session_capability_snapshot_publish(
    PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot, PAPACC_U64 session_instance_id,
    PAPACC_CAPABILITY_KEY *storage, PAPACC_SIZE capacity,
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *inputs);
void papacc_session_capability_snapshot_release(
    PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot);
PAPACC_BOOL papacc_session_capability_snapshot_contains(
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    const PAPACC_CAPABILITY_KEY *key);
PAPACC_BOOL papacc_capability_operation_allowed(
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    const PAPACC_CAPABILITY_KEY *key,
    const PAPACC_CAPABILITY_SET *server_enabled_now,
    PAPACC_CAPABILITY_POLICY_FN contextual_policy,
    void *contextual_policy_context);

#endif
