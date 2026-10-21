#include "builtin_capabilities.h"

const PAPACC_CAPABILITY_KEY PAPACC_CAPABILITY_NETWORK_EGRESS_V1 = { 1U, 1U };
const PAPACC_CAPABILITY_KEY PAPACC_CAPABILITY_NETWORK_EGRESS_V2 = { 1U, 2U };
const PAPACC_CAPABILITY_KEY PAPACC_CAPABILITY_TLS_OFFLOAD_V1 = { 2U, 1U };

PAPACC_RESULT papacc_builtin_capabilities_register(
    PAPACC_CAPABILITY_REGISTRY *registry)
{
    PAPACC_RESULT result = papacc_capability_registry_register(registry,
        &PAPACC_CAPABILITY_NETWORK_EGRESS_V1, "NETWORK_EGRESS");
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_capability_registry_register(registry,
        &PAPACC_CAPABILITY_NETWORK_EGRESS_V2, "NETWORK_EGRESS");
    if (result != PAPACC_RESULT_OK) return result;
    return papacc_capability_registry_register(registry,
        &PAPACC_CAPABILITY_TLS_OFFLOAD_V1, "TLS_OFFLOAD");
}
