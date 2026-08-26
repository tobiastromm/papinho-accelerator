#include "bind_selection.h"

static int papacc_test_structure(void)
{
    const PAPACC_U32 valid_ids[] = {2, 4};
    const PAPACC_U32 zero_ids[] = {2, 0};
    const PAPACC_U32 duplicate_ids[] = {2, 4, 2};
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;

    if (selection.mode != PAPACC_BIND_SELECTION_UNSPECIFIED ||
        selection.interface_instance_ids != NULL ||
        selection.interface_instance_count != 0) {
        return 1;
    }
    if (papacc_bind_selection_validate(&selection) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_bind_selection_validate(NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 2;
    }

    selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    if (papacc_bind_selection_validate(&selection) != PAPACC_RESULT_OK) {
        return 3;
    }
    selection.interface_instance_ids = valid_ids;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 4;
    }
    selection.interface_instance_count = 1;
    selection.interface_instance_ids = NULL;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 5;
    }

    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = valid_ids;
    selection.interface_instance_count = 2;
    if (papacc_bind_selection_validate(&selection) != PAPACC_RESULT_OK) {
        return 6;
    }
    selection.interface_instance_count = 0;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 7;
    }
    selection.interface_instance_ids = zero_ids;
    selection.interface_instance_count = 2;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 8;
    }
    selection.interface_instance_ids = duplicate_ids;
    selection.interface_instance_count = 3;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 9;
    }
    selection.mode = (PAPACC_BIND_SELECTION_MODE)99;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 10;
    }
    return 0;
}

static int papacc_test_snapshot(void)
{
    const PAPACC_U32 selected_ids[] = {2, 3};
    const PAPACC_U32 missing_id[] = {7};
    const PAPACC_U32 duplicate_id[] = {2};
    PAPACC_NETWORK_INTERFACE interfaces[4] = {
        PAPACC_NETWORK_INTERFACE_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_INITIALIZER
    };
    PAPACC_NETWORK_INTERFACE_ADDRESS address =
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;

    interfaces[0].interface_instance_id = 1;
    interfaces[1].interface_instance_id = 2;
    interfaces[1].is_up = PAPACC_FALSE;
    interfaces[2].interface_instance_id = 3;
    interfaces[2].is_loopback = PAPACC_TRUE;
    address.interface_instance_id = 2;
    papacc_ip_address_set_ipv4(&address.address, 192, 168, 1, 2);
    snapshot.interfaces = interfaces;
    snapshot.interface_count = 3;
    snapshot.addresses = &address;
    snapshot.address_count = 1;

    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = selected_ids;
    selection.interface_instance_count = 2;
    if (papacc_bind_selection_validate_snapshot(&selection, &snapshot) !=
        PAPACC_RESULT_OK) {
        return 1;
    }

    selection.interface_instance_ids = missing_id;
    selection.interface_instance_count = 1;
    if (papacc_bind_selection_validate_snapshot(&selection, &snapshot) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 2;
    }

    interfaces[3].interface_instance_id = 2;
    snapshot.interface_count = 4;
    selection.interface_instance_ids = duplicate_id;
    if (papacc_bind_selection_validate_snapshot(&selection, &snapshot) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 3;
    }

    selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    selection.interface_instance_ids = NULL;
    selection.interface_instance_count = 0;
    snapshot = (PAPACC_NETWORK_DISCOVERY_SNAPSHOT)
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    if (papacc_bind_selection_validate_snapshot(&selection, &snapshot) !=
        PAPACC_RESULT_OK) {
        return 4;
    }
    if (papacc_bind_selection_validate_snapshot(&selection, NULL) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 5;
    }
    snapshot.interface_count = 1;
    if (papacc_bind_selection_validate_snapshot(&selection, &snapshot) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 6;
    }
    return 0;
}

int main(void)
{
    int result = papacc_test_structure();
    if (result != 0) {
        return 10 + result;
    }
    result = papacc_test_snapshot();
    return (result == 0) ? 0 : 30 + result;
}
