#include <stdlib.h>

#include "network_interface.h"

int main(void)
{
    PAPACC_NETWORK_INTERFACE_ADDRESS *entries = NULL;
    PAPACC_SIZE count = 0;
    PAPACC_SIZE required = 0;
    PAPACC_SIZE capacity = 0;
    PAPACC_SIZE index;
    PAPACC_U32 previous_instance_id = 0;
    PAPACC_RESULT result;
    int attempt;

    if (papacc_network_discover_local_addresses(
            NULL,
            0,
            NULL,
            &required) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 1;
    }
    if (papacc_network_discover_local_addresses(
            NULL,
            1,
            &count,
            &required) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 2;
    }

    result = papacc_network_discover_local_addresses(
        NULL,
        0,
        &count,
        &required);
    if (required == 0) {
        return (result == PAPACC_RESULT_OK && count == 0) ? 0 : 3;
    }
    if (result != PAPACC_RESULT_LIMIT_EXCEEDED || count != 0) {
        return 4;
    }

    for (attempt = 0; attempt < 4; ++attempt) {
        if (required > SIZE_MAX / sizeof(*entries)) {
            free(entries);
            return 5;
        }

        free(entries);
        capacity = required;
        entries = (PAPACC_NETWORK_INTERFACE_ADDRESS *)malloc(
            capacity * sizeof(*entries));
        if (entries == NULL) {
            return 6;
        }

        result = papacc_network_discover_local_addresses(
            entries,
            capacity,
            &count,
            &required);
        if (result != PAPACC_RESULT_LIMIT_EXCEEDED) {
            break;
        }
        if (count != 0 || required <= capacity) {
            free(entries);
            return 7;
        }
    }

    if (result != PAPACC_RESULT_OK || count != required ||
        count > capacity) {
        free(entries);
        return 8;
    }

    for (index = 0; index < count; ++index) {
        const PAPACC_NETWORK_INTERFACE_ADDRESS *entry = &entries[index];

        if (entry->address.family != PAPACC_IP_FAMILY_IPV4 &&
            entry->address.family != PAPACC_IP_FAMILY_IPV6) {
            free(entries);
            return 9;
        }
        if (entry->interface_instance_id == 0 ||
            entry->interface_index == 0 ||
            (entry->interface_is_up != PAPACC_FALSE &&
             entry->interface_is_up != PAPACC_TRUE) ||
            (entry->interface_is_loopback != PAPACC_FALSE &&
             entry->interface_is_loopback != PAPACC_TRUE)) {
            free(entries);
            return 10;
        }
        if ((index == 0 && entry->interface_instance_id != 1) ||
            (index != 0 &&
             entry->interface_instance_id != previous_instance_id &&
             entry->interface_instance_id != previous_instance_id + 1)) {
            free(entries);
            return 11;
        }
        previous_instance_id = entry->interface_instance_id;
        if (entry->address.family == PAPACC_IP_FAMILY_IPV4 &&
            entry->scope_id != 0) {
            free(entries);
            return 12;
        }
    }

    for (index = 0; index < count; ++index) {
        PAPACC_SIZE other_index;

        for (other_index = index + 1; other_index < count; ++other_index) {
            const PAPACC_NETWORK_INTERFACE_ADDRESS *left = &entries[index];
            const PAPACC_NETWORK_INTERFACE_ADDRESS *right =
                &entries[other_index];

            if (left->interface_instance_id == right->interface_instance_id &&
                left->address.family == right->address.family &&
                left->interface_index != right->interface_index) {
                free(entries);
                return 13;
            }
        }
    }

    free(entries);
    return 0;
}
