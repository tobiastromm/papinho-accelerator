#include <stdlib.h>
#include <string.h>

#include "network_interface.h"

static int snapshot_is_empty(const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot)
{
    return snapshot->interfaces == NULL && snapshot->interface_count == 0 &&
        snapshot->addresses == NULL && snapshot->address_count == 0;
}

int main(void)
{
    PAPACC_NETWORK_INTERFACE *interfaces = NULL;
    PAPACC_NETWORK_INTERFACE_ADDRESS *addresses = NULL;
    char *presentation = NULL;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_SIZE interface_required = 0;
    PAPACC_SIZE address_required = 0;
    PAPACC_SIZE presentation_required = 0;
    PAPACC_SIZE interface_capacity = 0;
    PAPACC_SIZE address_capacity = 0;
    PAPACC_SIZE presentation_capacity = 0;
    PAPACC_SIZE index;
    PAPACC_RESULT result = PAPACC_RESULT_INTERNAL_ERROR;
    int attempt;
    int exit_code = 0;

    if (papacc_network_discover_local_snapshot(
            NULL, 0, NULL, 0, NULL, 0, NULL,
            &interface_required, &address_required, &presentation_required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_network_discover_local_snapshot(
            NULL, 1, NULL, 0, NULL, 0, &snapshot,
            &interface_required, &address_required, &presentation_required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_network_discover_local_snapshot(
            NULL, 0, NULL, 1, NULL, 0, &snapshot,
            &interface_required, &address_required, &presentation_required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_network_discover_local_snapshot(
            NULL, 0, NULL, 0, NULL, 1, &snapshot,
            &interface_required, &address_required, &presentation_required) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 1;
    }

    result = papacc_network_discover_local_snapshot(
        NULL, 0, NULL, 0, NULL, 0, &snapshot,
        &interface_required, &address_required, &presentation_required);
    if ((interface_required == 0 && address_required == 0 &&
         presentation_required == 0 && result != PAPACC_RESULT_OK) ||
        ((interface_required != 0 || address_required != 0 ||
          presentation_required != 0) &&
         result != PAPACC_RESULT_LIMIT_EXCEEDED) ||
        !snapshot_is_empty(&snapshot)) {
        return 2;
    }

    for (attempt = 0; attempt < 4; ++attempt) {
        free(interfaces);
        free(addresses);
        free(presentation);
        interfaces = NULL;
        addresses = NULL;
        presentation = NULL;
        interface_capacity = interface_required;
        address_capacity = address_required;
        presentation_capacity = presentation_required;

        if (interface_capacity > SIZE_MAX / sizeof(*interfaces) ||
            address_capacity > SIZE_MAX / sizeof(*addresses)) {
            exit_code = 3;
            goto cleanup;
        }
        if (interface_capacity != 0) {
            interfaces = (PAPACC_NETWORK_INTERFACE *)malloc(
                interface_capacity * sizeof(*interfaces));
        }
        if (address_capacity != 0) {
            addresses = (PAPACC_NETWORK_INTERFACE_ADDRESS *)malloc(
                address_capacity * sizeof(*addresses));
        }
        if (presentation_capacity != 0) {
            presentation = (char *)malloc(presentation_capacity);
        }
        if ((interface_capacity != 0 && interfaces == NULL) ||
            (address_capacity != 0 && addresses == NULL) ||
            (presentation_capacity != 0 && presentation == NULL)) {
            exit_code = 4;
            goto cleanup;
        }

        result = papacc_network_discover_local_snapshot(
            interfaces, interface_capacity, addresses, address_capacity,
            presentation, presentation_capacity, &snapshot,
            &interface_required, &address_required, &presentation_required);
        if (result == PAPACC_RESULT_OK &&
            interface_required == interface_capacity &&
            address_required == address_capacity &&
            presentation_required == presentation_capacity) {
            break;
        }
        if ((result != PAPACC_RESULT_LIMIT_EXCEEDED &&
             result != PAPACC_RESULT_OK) ||
            (result == PAPACC_RESULT_LIMIT_EXCEEDED &&
             !snapshot_is_empty(&snapshot))) {
            exit_code = 5;
            goto cleanup;
        }
    }

    if (result != PAPACC_RESULT_OK || attempt == 4 ||
        snapshot.interfaces != interfaces ||
        snapshot.interface_count != interface_required ||
        snapshot.addresses != addresses ||
        snapshot.address_count != address_required) {
        exit_code = 6;
        goto cleanup;
    }

    if (presentation_capacity != 0) {
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT insufficient =
            PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
        PAPACC_SIZE next_interfaces = 0;
        PAPACC_SIZE next_addresses = 0;
        PAPACC_SIZE next_presentation = 0;

        result = papacc_network_discover_local_snapshot(
            interfaces, interface_capacity, addresses, address_capacity,
            presentation, presentation_capacity - 1, &insufficient,
            &next_interfaces, &next_addresses, &next_presentation);
        if (result != PAPACC_RESULT_LIMIT_EXCEEDED ||
            !snapshot_is_empty(&insufficient) ||
            next_presentation <= presentation_capacity - 1) {
            exit_code = 7;
            goto cleanup;
        }
    }

    for (index = 0; index < snapshot.interface_count; ++index) {
        const PAPACC_NETWORK_INTERFACE *interface_record =
            &snapshot.interfaces[index];
        const PAPACC_NETWORK_INTERFACE_PRESENTATION_NAME *name =
            &interface_record->presentation_name;

        if (interface_record->interface_instance_id != (PAPACC_U32)(index + 1) ||
            (interface_record->persistent_id.is_valid != PAPACC_FALSE &&
             interface_record->persistent_id.is_valid != PAPACC_TRUE) ||
            (interface_record->persistent_id.is_valid == PAPACC_FALSE &&
             interface_record->persistent_id.value != 0) ||
            (interface_record->is_up != PAPACC_FALSE &&
             interface_record->is_up != PAPACC_TRUE) ||
            (interface_record->is_loopback != PAPACC_FALSE &&
             interface_record->is_loopback != PAPACC_TRUE) ||
            (name->is_available != PAPACC_FALSE &&
             name->is_available != PAPACC_TRUE)) {
            exit_code = 8;
            goto cleanup;
        }
        if (name->is_available == PAPACC_FALSE) {
            if (name->utf8 != NULL || name->length != 0) {
                exit_code = 9;
                goto cleanup;
            }
        } else {
            const uintptr_t arena_begin = (uintptr_t)(const void *)presentation;
            const uintptr_t arena_end = arena_begin + presentation_capacity;
            const uintptr_t name_begin = (uintptr_t)(const void *)name->utf8;
            if (name->utf8 == NULL || name->length == 0 ||
                arena_end < arena_begin || name_begin < arena_begin ||
                name_begin >= arena_end ||
                name->length >= (PAPACC_SIZE)(arena_end - name_begin) ||
                name->utf8[name->length] != '\0' ||
                strlen(name->utf8) != name->length) {
                exit_code = 10;
                goto cleanup;
            }
        }
    }

    for (index = 0; index < snapshot.address_count; ++index) {
        const PAPACC_NETWORK_INTERFACE_ADDRESS *address =
            &snapshot.addresses[index];
        const PAPACC_NETWORK_INTERFACE *interface_record;

        if ((address->address.family != PAPACC_IP_FAMILY_IPV4 &&
             address->address.family != PAPACC_IP_FAMILY_IPV6) ||
            address->interface_instance_id == 0 ||
            address->interface_instance_id > snapshot.interface_count ||
            address->interface_index == 0 ||
            (address->address.family == PAPACC_IP_FAMILY_IPV4 &&
             address->scope_id != 0)) {
            exit_code = 11;
            goto cleanup;
        }
        interface_record =
            &snapshot.interfaces[address->interface_instance_id - 1];
        if (address->interface_persistent_id.is_valid !=
                interface_record->persistent_id.is_valid ||
            address->interface_persistent_id.value !=
                interface_record->persistent_id.value ||
            address->interface_is_up != interface_record->is_up ||
            address->interface_is_loopback != interface_record->is_loopback) {
            exit_code = 12;
            goto cleanup;
        }
    }

    {
        PAPACC_SIZE legacy_count = 0;
        PAPACC_SIZE legacy_required = 0;
        result = papacc_network_discover_local_addresses(
            NULL, 0, &legacy_count, &legacy_required);
        if ((legacy_required == 0 && result != PAPACC_RESULT_OK) ||
            (legacy_required != 0 && result != PAPACC_RESULT_LIMIT_EXCEEDED) ||
            legacy_count != 0) {
            exit_code = 13;
            goto cleanup;
        }
    }

cleanup:
    free(interfaces);
    free(addresses);
    free(presentation);
    return exit_code;
}
