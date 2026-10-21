#include "network_authorization.h"

PAPACC_RESULT papacc_network_open_authorize(
    PAPACC_BOOL secure_principal_authenticated,
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    const PAPACC_CAPABILITY_SET *server_enabled_now,
    PAPACC_CAPABILITY_POLICY_FN contextual_policy,
    void *contextual_policy_context,
    PAPACC_NETWORK_TRANSPORT_MODE mode)
{
    if (secure_principal_authenticated != PAPACC_TRUE || snapshot == NULL ||
        snapshot->published != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (mode != PAPACC_NETWORK_TRANSPORT_TCP_PLAIN &&
        mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_capability_operation_allowed(snapshot,
        &PAPACC_CAPABILITY_NETWORK_EGRESS_V2,server_enabled_now,
        contextual_policy,contextual_policy_context) != PAPACC_TRUE &&
        papacc_capability_operation_allowed(snapshot,
        &PAPACC_CAPABILITY_NETWORK_EGRESS_V1,server_enabled_now,
        contextual_policy,contextual_policy_context) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (mode == PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED &&
        papacc_capability_operation_allowed(snapshot,
        &PAPACC_CAPABILITY_TLS_OFFLOAD_V1,server_enabled_now,
        contextual_policy,contextual_policy_context) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    return PAPACC_RESULT_OK;
}
