#include "network_interface.h"

PAPACC_BOOL papacc_network_interface_persistent_id_equal(
    const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *left,
    const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *right)
{
    if (left == NULL || right == NULL ||
        left->is_valid != PAPACC_TRUE || right->is_valid != PAPACC_TRUE) {
        return PAPACC_FALSE;
    }

    return (left->value == right->value) ? PAPACC_TRUE : PAPACC_FALSE;
}
