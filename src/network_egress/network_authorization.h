#ifndef PAPACC_NETWORK_AUTHORIZATION_H
#define PAPACC_NETWORK_AUTHORIZATION_H

#include "builtin_capabilities.h"
#include "network_offload_protocol.h"

PAPACC_RESULT papacc_network_open_authorize(
    PAPACC_BOOL secure_principal_authenticated,
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    const PAPACC_CAPABILITY_SET *server_enabled_now,
    PAPACC_CAPABILITY_POLICY_FN contextual_policy,
    void *contextual_policy_context,
    PAPACC_NETWORK_TRANSPORT_MODE mode);

#endif
