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
        PAPACC_RESULT_INVALID_STATE) {
        return 2;
    }
    if (papacc_bind_selection_validate(NULL) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 3;
    }

    selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    if (papacc_bind_selection_validate(&selection) != PAPACC_RESULT_OK) {
        return 4;
    }
    selection.interface_instance_ids = valid_ids;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 5;
    }
    selection.interface_instance_count = 1;
    selection.interface_instance_ids = NULL;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 6;
    }

    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = valid_ids;
    selection.interface_instance_count = 2;
    if (papacc_bind_selection_validate(&selection) != PAPACC_RESULT_OK) {
        return 7;
    }
    selection.interface_instance_count = 0;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 8;
    }
    selection.interface_instance_ids = NULL;
    selection.interface_instance_count = 1;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 9;
    }
    selection.interface_instance_ids = zero_ids;
    selection.interface_instance_count = 2;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 10;
    }
    selection.interface_instance_ids = duplicate_ids;
    selection.interface_instance_count = 3;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 11;
    }
    selection.mode = (PAPACC_BIND_SELECTION_MODE)99;
    if (papacc_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 12;
    }

    return 0;
}

static int papacc_test_snapshot(void)
{
    const PAPACC_U32 selected_ids[] = {2, 3};
    const PAPACC_U32 missing_id[] = {7};
    const PAPACC_U32 loopback_id[] = {3};
    const PAPACC_U32 down_id[] = {2};
    PAPACC_NETWORK_INTERFACE_ADDRESS entries[4] = {
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER
    };
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;

    entries[0].interface_instance_id = 1;
    entries[1].interface_instance_id = 2;
    entries[1].interface_is_up = PAPACC_FALSE;
    entries[2].interface_instance_id = 3;
    entries[2].address.family = PAPACC_IP_FAMILY_IPV4;
    entries[2].interface_is_loopback = PAPACC_TRUE;
    entries[3].interface_instance_id = 3;
    entries[3].address.family = PAPACC_IP_FAMILY_IPV6;
    entries[3].interface_is_loopback = PAPACC_TRUE;

    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = selected_ids;
    selection.interface_instance_count = 2;
    if (papacc_bind_selection_validate_snapshot(
            &selection, entries, 4) != PAPACC_RESULT_OK) {
        return 1;
    }

    selection.interface_instance_ids = missing_id;
    selection.interface_instance_count = 1;
    if (papacc_bind_selection_validate_snapshot(
            &selection, entries, 4) != PAPACC_RESULT_INVALID_STATE) {
        return 2;
    }
    selection.interface_instance_ids = loopback_id;
    if (papacc_bind_selection_validate_snapshot(
            &selection, entries, 4) != PAPACC_RESULT_OK) {
        return 3;
    }
    selection.interface_instance_ids = down_id;
    if (papacc_bind_selection_validate_snapshot(
            &selection, entries, 4) != PAPACC_RESULT_OK) {
        return 4;
    }

    selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    selection.interface_instance_ids = NULL;
    selection.interface_instance_count = 0;
    if (papacc_bind_selection_validate_snapshot(
            &selection, NULL, 0) != PAPACC_RESULT_OK) {
        return 5;
    }
    if (papacc_bind_selection_validate_snapshot(
            &selection, NULL, 1) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 6;
    }

    selection.mode = PAPACC_BIND_SELECTION_UNSPECIFIED;
    if (papacc_bind_selection_validate_snapshot(
            &selection, entries, 4) != PAPACC_RESULT_INVALID_STATE) {
        return 7;
    }

    return 0;
}

int main(void)
{
    int result;

    result = papacc_test_structure();
    if (result != 0) {
        return 10 + result;
    }
    result = papacc_test_snapshot();
    if (result != 0) {
        return 30 + result;
    }

    return 0;
}
