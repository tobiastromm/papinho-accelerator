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
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE entry_count)
{
    PAPACC_SIZE selected_index;
    PAPACC_SIZE entry_index;
    PAPACC_BOOL found;
    PAPACC_RESULT result;

    result = papacc_bind_selection_validate(selection);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (entries == NULL && entry_count != 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (selection->mode == PAPACC_BIND_SELECTION_ALL_INTERFACES) {
        return PAPACC_RESULT_OK;
    }

    for (selected_index = 0;
         selected_index < selection->interface_instance_count;
         ++selected_index) {
        found = PAPACC_FALSE;
        for (entry_index = 0; entry_index < entry_count; ++entry_index) {
            if (selection->interface_instance_ids[selected_index] ==
                entries[entry_index].interface_instance_id) {
                found = PAPACC_TRUE;
                break;
            }
        }
        if (found == PAPACC_FALSE) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    }

    return PAPACC_RESULT_OK;
}
