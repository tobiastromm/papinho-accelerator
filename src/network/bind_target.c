#include "bind_target.h"

static PAPACC_BOOL papacc_bind_selection_contains_id(
    const PAPACC_BIND_SELECTION *selection,
    PAPACC_U32 interface_instance_id)
{
    PAPACC_SIZE index;

    for (index = 0;
         index < selection->interface_instance_count;
         ++index) {
        if (selection->interface_instance_ids[index] == interface_instance_id) {
            return PAPACC_TRUE;
        }
    }

    return PAPACC_FALSE;
}

static PAPACC_BOOL papacc_bind_entry_is_applicable(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entry)
{
    if (papacc_bind_selection_contains_id(
            selection,
            entry->interface_instance_id) == PAPACC_FALSE) {
        return PAPACC_FALSE;
    }
    if (entry->address.family != PAPACC_IP_FAMILY_IPV4 &&
        entry->address.family != PAPACC_IP_FAMILY_IPV6) {
        return PAPACC_FALSE;
    }
    if (papacc_ip_address_is_unspecified(&entry->address) == PAPACC_TRUE) {
        return PAPACC_FALSE;
    }

    return PAPACC_TRUE;
}

static PAPACC_U32 papacc_bind_entry_scope(
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entry)
{
    return (entry->address.family == PAPACC_IP_FAMILY_IPV6)
               ? entry->scope_id
               : 0;
}

static PAPACC_BOOL papacc_bind_entry_has_prior_duplicate(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE entry_index)
{
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entry = &entries[entry_index];
    PAPACC_SIZE prior_index;

    for (prior_index = 0; prior_index < entry_index; ++prior_index) {
        const PAPACC_NETWORK_INTERFACE_ADDRESS *prior = &entries[prior_index];

        if (papacc_bind_entry_is_applicable(selection, prior) == PAPACC_TRUE &&
            papacc_bind_entry_scope(prior) == papacc_bind_entry_scope(entry) &&
            papacc_ip_address_equal(
                &prior->address,
                &entry->address) == PAPACC_TRUE) {
            return PAPACC_TRUE;
        }
    }

    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_bind_count_selected_targets(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE entry_count,
    PAPACC_SIZE *out_required)
{
    PAPACC_SIZE index;
    PAPACC_SIZE required = 0;

    for (index = 0; index < entry_count; ++index) {
        if (papacc_bind_entry_is_applicable(selection, &entries[index]) ==
                PAPACC_TRUE &&
            papacc_bind_entry_has_prior_duplicate(selection, entries, index) ==
                PAPACC_FALSE) {
            if (required == SIZE_MAX) {
                return PAPACC_RESULT_LIMIT_EXCEEDED;
            }
            ++required;
        }
    }

    *out_required = required;
    return PAPACC_RESULT_OK;
}

static void papacc_bind_write_selected_targets(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE entry_count,
    PAPACC_BIND_TARGET *targets,
    PAPACC_SIZE *out_count)
{
    PAPACC_SIZE index;
    PAPACC_SIZE count = 0;

    for (index = 0; index < entry_count; ++index) {
        if (papacc_bind_entry_is_applicable(selection, &entries[index]) ==
                PAPACC_FALSE ||
            papacc_bind_entry_has_prior_duplicate(selection, entries, index) ==
                PAPACC_TRUE) {
            continue;
        }

        targets[count].address = entries[index].address;
        targets[count].scope_id = papacc_bind_entry_scope(&entries[index]);
        targets[count].interface_instance_id =
            entries[index].interface_instance_id;
        ++count;
    }

    *out_count = count;
}

static void papacc_bind_write_all_targets(PAPACC_BIND_TARGET *targets)
{
    *targets = (PAPACC_BIND_TARGET)PAPACC_BIND_TARGET_INITIALIZER;
    papacc_ip_address_set_ipv4(&targets[0].address, 0, 0, 0, 0);

    targets[1] = (PAPACC_BIND_TARGET)PAPACC_BIND_TARGET_INITIALIZER;
    papacc_ip_address_set_ipv6(
        &targets[1].address,
        targets[1].address.bytes);
}

PAPACC_RESULT papacc_bind_targets_resolve(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE entry_count,
    PAPACC_BIND_TARGET *targets,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_count,
    PAPACC_SIZE *out_required)
{
    PAPACC_SIZE required;
    PAPACC_RESULT result;

    if (out_count == NULL || out_required == NULL ||
        (targets == NULL && capacity != 0) ||
        (entries == NULL && entry_count != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    *out_count = 0;
    *out_required = 0;

    result = papacc_bind_selection_validate(selection);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }

    if (selection->mode == PAPACC_BIND_SELECTION_ALL_INTERFACES) {
        required = 2;
    } else {
        result = papacc_bind_selection_validate_snapshot(
            selection,
            entries,
            entry_count);
        if (result != PAPACC_RESULT_OK) {
            return result;
        }

        result = papacc_bind_count_selected_targets(
            selection,
            entries,
            entry_count,
            &required);
        if (result != PAPACC_RESULT_OK) {
            return result;
        }
        if (required == 0) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    }

    *out_required = required;
    if (capacity < required) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }

    if (selection->mode == PAPACC_BIND_SELECTION_ALL_INTERFACES) {
        papacc_bind_write_all_targets(targets);
        *out_count = required;
    } else {
        papacc_bind_write_selected_targets(
            selection,
            entries,
            entry_count,
            targets,
            out_count);
    }

    return PAPACC_RESULT_OK;
}
