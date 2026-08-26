#include "persistent_bind_selection.h"

PAPACC_RESULT papacc_persistent_bind_selection_validate(
    const PAPACC_PERSISTENT_BIND_SELECTION *selection)
{
    PAPACC_SIZE index;
    PAPACC_SIZE other_index;

    if (selection == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (selection->mode == PAPACC_BIND_SELECTION_UNSPECIFIED) {
        if (selection->interface_persistent_ids != NULL ||
            selection->interface_persistent_id_count != 0) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (selection->mode == PAPACC_BIND_SELECTION_ALL_INTERFACES) {
        return (selection->interface_persistent_ids == NULL &&
                selection->interface_persistent_id_count == 0)
                   ? PAPACC_RESULT_OK
                   : PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (selection->mode != PAPACC_BIND_SELECTION_SELECTED_INTERFACES ||
        selection->interface_persistent_ids == NULL ||
        selection->interface_persistent_id_count == 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    for (index = 0;
         index < selection->interface_persistent_id_count;
         ++index) {
        const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *persistent_id =
            &selection->interface_persistent_ids[index];
        if (persistent_id->is_valid != PAPACC_FALSE &&
            persistent_id->is_valid != PAPACC_TRUE) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        if (persistent_id->is_valid == PAPACC_FALSE) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        for (other_index = index + 1;
             other_index < selection->interface_persistent_id_count;
             ++other_index) {
            if (papacc_network_interface_persistent_id_equal(
                    persistent_id,
                    &selection->interface_persistent_ids[other_index]) ==
                PAPACC_TRUE) {
                return PAPACC_RESULT_INVALID_ARGUMENT;
            }
        }
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_persistent_bind_find_instance_id(
    const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *persistent_id,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,
    PAPACC_U32 *out_instance_id)
{
    PAPACC_SIZE index;
    PAPACC_SIZE match_count = 0;
    PAPACC_U32 instance_id = 0;

    for (index = 0; index < snapshot->interface_count; ++index) {
        const PAPACC_NETWORK_INTERFACE *interface_record =
            &snapshot->interfaces[index];
        if (papacc_network_interface_persistent_id_equal(
                persistent_id, &interface_record->persistent_id) ==
            PAPACC_TRUE) {
            ++match_count;
            instance_id = interface_record->interface_instance_id;
        }
    }
    if (match_count != 1 || instance_id == 0) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    for (index = 0; index < snapshot->interface_count; ++index) {
        if (snapshot->interfaces[index].interface_instance_id == instance_id &&
            papacc_network_interface_persistent_id_equal(
                persistent_id,
                &snapshot->interfaces[index].persistent_id) == PAPACC_FALSE) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    }
    *out_instance_id = instance_id;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_persistent_bind_selection_resolve(
    const PAPACC_PERSISTENT_BIND_SELECTION *persistent_selection,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,
    PAPACC_U32 *interface_instance_id_storage,
    PAPACC_SIZE interface_instance_id_capacity,
    PAPACC_BIND_SELECTION *out_runtime_selection,
    PAPACC_SIZE *out_required)
{
    PAPACC_BIND_SELECTION empty = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (out_runtime_selection == NULL || out_required == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    *out_runtime_selection = empty;
    *out_required = 0;
    if (snapshot == NULL ||
        (snapshot->interfaces == NULL && snapshot->interface_count != 0) ||
        (interface_instance_id_storage == NULL &&
         interface_instance_id_capacity != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    result = papacc_persistent_bind_selection_validate(persistent_selection);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (persistent_selection->mode == PAPACC_BIND_SELECTION_ALL_INTERFACES) {
        out_runtime_selection->mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
        return PAPACC_RESULT_OK;
    }

    for (index = 0;
         index < persistent_selection->interface_persistent_id_count;
         ++index) {
        PAPACC_U32 ignored_instance_id;
        result = papacc_persistent_bind_find_instance_id(
            &persistent_selection->interface_persistent_ids[index],
            snapshot, &ignored_instance_id);
        if (result != PAPACC_RESULT_OK) {
            return result;
        }
    }

    *out_required = persistent_selection->interface_persistent_id_count;
    if (interface_instance_id_capacity < *out_required) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    for (index = 0;
         index < persistent_selection->interface_persistent_id_count;
         ++index) {
        result = papacc_persistent_bind_find_instance_id(
            &persistent_selection->interface_persistent_ids[index], snapshot,
            &interface_instance_id_storage[index]);
        if (result != PAPACC_RESULT_OK) {
            return result;
        }
    }
    out_runtime_selection->mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    out_runtime_selection->interface_instance_ids =
        interface_instance_id_storage;
    out_runtime_selection->interface_instance_count = *out_required;
    return PAPACC_RESULT_OK;
}
