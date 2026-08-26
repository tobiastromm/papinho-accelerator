#include <stdlib.h>

#include "network_interface.h"

int main(void)
{
    PAPACC_NETWORK_INTERFACE *interfaces = NULL;
    PAPACC_NETWORK_INTERFACE_ADDRESS *addresses = NULL;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_SIZE interface_required = 0;
    PAPACC_SIZE address_required = 0;
    PAPACC_SIZE interface_capacity = 0;
    PAPACC_SIZE address_capacity = 0;
    PAPACC_SIZE index;
    PAPACC_RESULT result;
    int attempt;

    if (papacc_network_discover_local_snapshot(
            NULL, 0, NULL, 0, NULL,
            &interface_required, &address_required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_network_discover_local_snapshot(
            NULL, 1, NULL, 0, &snapshot,
            &interface_required, &address_required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_network_discover_local_snapshot(
            NULL, 0, NULL, 1, &snapshot,
            &interface_required, &address_required) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 1;
    }

    result = papacc_network_discover_local_snapshot(
        NULL, 0, NULL, 0, &snapshot,
        &interface_required, &address_required);
    if ((interface_required == 0 && address_required == 0 &&
         result != PAPACC_RESULT_OK) ||
        ((interface_required != 0 || address_required != 0) &&
         result != PAPACC_RESULT_LIMIT_EXCEEDED) ||
        snapshot.interfaces != NULL || snapshot.interface_count != 0 ||
        snapshot.addresses != NULL || snapshot.address_count != 0) {
        return 2;
    }

    for (attempt = 0; attempt < 4; ++attempt) {
        free(interfaces);
        free(addresses);
        interfaces = NULL;
        addresses = NULL;
        interface_capacity = interface_required;
        address_capacity = address_required;

        if (interface_capacity > SIZE_MAX / sizeof(*interfaces) ||
            address_capacity > SIZE_MAX / sizeof(*addresses)) {
            return 3;
        }
        if (interface_capacity != 0) {
            interfaces = (PAPACC_NETWORK_INTERFACE *)malloc(
                interface_capacity * sizeof(*interfaces));
            if (interfaces == NULL) {
                return 4;
            }
        }
        if (address_capacity != 0) {
            addresses = (PAPACC_NETWORK_INTERFACE_ADDRESS *)malloc(
                address_capacity * sizeof(*addresses));
            if (addresses == NULL) {
                free(interfaces);
                return 5;
            }
        }

        result = papacc_network_discover_local_snapshot(
            interfaces, interface_capacity,
            addresses, address_capacity,
            &snapshot, &interface_required, &address_required);
        if (result != PAPACC_RESULT_LIMIT_EXCEEDED) {
            break;
        }
        if (snapshot.interfaces != NULL || snapshot.interface_count != 0 ||
            snapshot.addresses != NULL || snapshot.address_count != 0 ||
            (interface_required <= interface_capacity &&
             address_required <= address_capacity)) {
            free(interfaces);
            free(addresses);
            return 6;
        }
    }

    if (result != PAPACC_RESULT_OK ||
        snapshot.interfaces != interfaces ||
        snapshot.interface_count != interface_required ||
        snapshot.addresses != addresses ||
        snapshot.address_count != address_required ||
        snapshot.interface_count > interface_capacity ||
        snapshot.address_count > address_capacity) {
        free(interfaces);
        free(addresses);
        return 7;
    }

    for (index = 0; index < snapshot.interface_count; ++index) {
        const PAPACC_NETWORK_INTERFACE *interface_record =
            &snapshot.interfaces[index];

        if (interface_record->interface_instance_id != (PAPACC_U32)(index + 1) ||
            (interface_record->persistent_id.is_valid != PAPACC_FALSE &&
             interface_record->persistent_id.is_valid != PAPACC_TRUE) ||
            (interface_record->persistent_id.is_valid == PAPACC_FALSE &&
             interface_record->persistent_id.value != 0) ||
            (interface_record->is_up != PAPACC_FALSE &&
             interface_record->is_up != PAPACC_TRUE) ||
            (interface_record->is_loopback != PAPACC_FALSE &&
             interface_record->is_loopback != PAPACC_TRUE)) {
            free(interfaces);
            free(addresses);
            return 8;
        }
    }

    for (index = 0; index < snapshot.address_count; ++index) {
        const PAPACC_NETWORK_INTERFACE_ADDRESS *address =
            &snapshot.addresses[index];
        const PAPACC_NETWORK_INTERFACE *interface_record;

        if (address->address.family != PAPACC_IP_FAMILY_IPV4 &&
            address->address.family != PAPACC_IP_FAMILY_IPV6) {
            free(interfaces);
            free(addresses);
            return 9;
        }
        if (address->interface_instance_id == 0 ||
            address->interface_instance_id > snapshot.interface_count ||
            address->interface_index == 0 ||
            (address->address.family == PAPACC_IP_FAMILY_IPV4 &&
             address->scope_id != 0)) {
            free(interfaces);
            free(addresses);
            return 10;
        }

        interface_record = &snapshot.interfaces[
            address->interface_instance_id - 1];
        if (address->interface_persistent_id.is_valid !=
                interface_record->persistent_id.is_valid ||
            address->interface_persistent_id.value !=
                interface_record->persistent_id.value ||
            address->interface_is_up != interface_record->is_up ||
            address->interface_is_loopback !=
                interface_record->is_loopback) {
            free(interfaces);
            free(addresses);
            return 11;
        }
    }

    {
        PAPACC_SIZE legacy_count = 0;
        PAPACC_SIZE legacy_required = 0;
        result = papacc_network_discover_local_addresses(
            NULL, 0, &legacy_count, &legacy_required);
        if ((legacy_required == 0 && result != PAPACC_RESULT_OK) ||
            (legacy_required != 0 &&
             result != PAPACC_RESULT_LIMIT_EXCEEDED) ||
            legacy_count != 0) {
            free(interfaces);
            free(addresses);
            return 12;
        }
    }

    free(interfaces);
    free(addresses);
    return 0;
}
