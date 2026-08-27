#include "network_endpoint.h"

PAPACC_RESULT papacc_network_endpoint_validate(
    const PAPACC_NETWORK_ENDPOINT *endpoint)
{
    if (endpoint == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (endpoint->address.family == PAPACC_IP_FAMILY_UNSPECIFIED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (endpoint->address.family == PAPACC_IP_FAMILY_IPV4) {
        return (endpoint->scope_id == 0)
                   ? PAPACC_RESULT_OK
                   : PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (endpoint->address.family == PAPACC_IP_FAMILY_IPV6) {
        return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_INVALID_ARGUMENT;
}

PAPACC_BOOL papacc_network_endpoint_equal(
    const PAPACC_NETWORK_ENDPOINT *left,
    const PAPACC_NETWORK_ENDPOINT *right)
{
    if (left == NULL || right == NULL) {
        return PAPACC_FALSE;
    }
    return (papacc_ip_address_equal(&left->address, &right->address) ==
                PAPACC_TRUE &&
            left->port == right->port &&
            left->scope_id == right->scope_id)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}
