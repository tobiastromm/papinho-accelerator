#include "bind_selection.h"

PAPACC_RESULT papacc_bind_selection_validate(
    const PAPACC_BIND_SELECTION *selection)
{
    PAPACC_SIZE index;
    PAPACC_SIZE other_index;

    if (selection == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    if (selection->mode == PAPACC_BIND_SELECTION_UNSPECIFIED) {
        if (selection->interface_instance_ids != NULL ||
            selection->interface_instance_count != 0) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        return PAPACC_RESULT_INVALID_STATE;
    }

    if (selection->mode == PAPACC_BIND_SELECTION_ALL_INTERFACES) {
        return (selection->interface_instance_ids == NULL &&
                selection->interface_instance_count == 0)
                   ? PAPACC_RESULT_OK
                   : PAPACC_RESULT_INVALID_ARGUMENT;
    }

    if (selection->mode != PAPACC_BIND_SELECTION_SELECTED_INTERFACES ||
        selection->interface_instance_ids == NULL ||
        selection->interface_instance_count == 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    for (index = 0;
         index < selection->interface_instance_count;
         ++index) {
        if (selection->interface_instance_ids[index] == 0) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }

        for (other_index = index + 1;
             other_index < selection->interface_instance_count;
             ++other_index) {
            if (selection->interface_instance_ids[index] ==
                selection->interface_instance_ids[other_index]) {
                return PAPACC_RESULT_INVALID_ARGUMENT;
            }
        }
    }

    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_bind_selection_validate_snapshot(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot)
{
    PAPACC_SIZE selected_index;
    PAPACC_SIZE interface_index;
    PAPACC_RESULT result;

    result = papacc_bind_selection_validate(selection);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (snapshot == NULL ||
        (snapshot->interfaces == NULL && snapshot->interface_count != 0) ||
        (snapshot->addresses == NULL && snapshot->address_count != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (selection->mode == PAPACC_BIND_SELECTION_ALL_INTERFACES) {
        return PAPACC_RESULT_OK;
    }

    for (selected_index = 0;
         selected_index < selection->interface_instance_count;
         ++selected_index) {
        PAPACC_SIZE match_count = 0;
        for (interface_index = 0;
             interface_index < snapshot->interface_count;
             ++interface_index) {
            if (selection->interface_instance_ids[selected_index] ==
                snapshot->interfaces[interface_index].interface_instance_id) {
                ++match_count;
            }
        }
        if (match_count != 1) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    }

    return PAPACC_RESULT_OK;
}
