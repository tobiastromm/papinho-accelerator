#ifndef PAPACC_SERVER_C8_POLICY_ADAPTER_WIN32_H
#define PAPACC_SERVER_C8_POLICY_ADAPTER_WIN32_H
/* Private composition, not a provider, login protocol or wire API. */
#include "c2_operation_win32.h"
#include "capability.h"
#include "server_config.h"
typedef struct PAPACC_C8_POLICY_BINDING {
    PAPACC_C2_SESSION *session;
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;
    const PAPACC_U64 *effective_epoch;
    PAPACC_U64 admitted_epoch, admitted_session;
    PAPACC_CONNECTION_SECURITY_CONTEXT connection_security;
    PAPACC_SESSION_SECURITY_CONTEXT session_security;
    PAPACC_CAPABILITY_POLICY_FN contextual_policy;
    void *policy_context;
    PAPACC_BOOL revoked;
} PAPACC_C8_POLICY_BINDING;
typedef struct PAPACC_C8_POLICY_ADAPTER {
    PAPACC_C2_OPERATION_MANAGER *manager;
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *effective;
    PAPACC_C2_OPEN_POLICY operation_policy;
    PAPACC_C2_CAPACITY_GATE capacity;
    void *operation_context;
    PAPACC_C8_POLICY_BINDING bindings[PAPACC_C2_SESSION_CAPACITY];
    const PAPACC_C2_SESSION *capacity_session;
    PAPACC_SIZE reserved_bytes;
    PAPACC_BOOL initialized, enabled, busy, stopped;
} PAPACC_C8_POLICY_ADAPTER;
/* References MUST be durable configuration/snapshot/context leases in the
 * owner's retained graph. No epoch is created or incremented here. Storage
 * must be zeroed and never reused after retention. Combined reservation must
 * include this object before enable; it is not a total ED11 certificate. */
PAPACC_RESULT papacc_c8_policy_adapter_init(PAPACC_C8_POLICY_ADAPTER *,
    PAPACC_C2_OPERATION_MANAGER *, const PAPACC_CAPABILITY_EVALUATION_INPUTS *,
    PAPACC_C2_OPEN_POLICY, PAPACC_C2_CAPACITY_GATE, void *, PAPACC_SIZE);
PAPACC_RESULT papacc_c8_policy_adapter_bind(PAPACC_C8_POLICY_ADAPTER *,
    PAPACC_C2_SESSION *, const PAPACC_SESSION_CAPABILITY_SNAPSHOT *,
    const PAPACC_U64 *, const PAPACC_CONNECTION_SECURITY_CONTEXT *,
    const PAPACC_SESSION_SECURITY_CONTEXT *, PAPACC_SERVER_LISTENER_PROFILE,
    PAPACC_CAPABILITY_POLICY_FN, void *);
PAPACC_RESULT papacc_c8_policy_adapter_enable(PAPACC_C8_POLICY_ADAPTER *);
PAPACC_RESULT papacc_c8_policy_adapter_reconcile(PAPACC_C8_POLICY_ADAPTER *);
PAPACC_RESULT papacc_c8_policy_adapter_stop(PAPACC_C8_POLICY_ADAPTER *);
/* Query never claims a role token or advances DATA. Existing BIND/ACK and
 * manager authorization remain mandatory; role/channel must be factual. */
PAPACC_BOOL papacc_c8_policy_adapter_authorized(PAPACC_C8_POLICY_ADAPTER *,
    PAPACC_C2_SESSION *, const PAPACC_PRINCIPAL *, PAPACC_U64, PAPACC_U64,
    unsigned, PAPACC_U64);
#endif
