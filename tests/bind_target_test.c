#include "bind_target.h"

static void papacc_test_set_ipv4(
    PAPACC_NETWORK_INTERFACE_ADDRESS *entry,
    PAPACC_U32 instance_id,
    PAPACC_U8 a,
    PAPACC_U8 b,
    PAPACC_U8 c,
    PAPACC_U8 d)
{
    *entry = (PAPACC_NETWORK_INTERFACE_ADDRESS)
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    entry->interface_instance_id = instance_id;
    papacc_ip_address_set_ipv4(&entry->address, a, b, c, d);
}

static void papacc_test_set_ipv6(
    PAPACC_NETWORK_INTERFACE_ADDRESS *entry,
    PAPACC_U32 instance_id,
    const PAPACC_U8 bytes[PAPACC_IP_ADDRESS_BYTE_COUNT],
    PAPACC_U32 scope_id)
{
    *entry = (PAPACC_NETWORK_INTERFACE_ADDRESS)
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    entry->interface_instance_id = instance_id;
    entry->scope_id = scope_id;
    papacc_ip_address_set_ipv6(&entry->address, bytes);
}

static int papacc_test_initializer_and_all(void)
{
    PAPACC_BIND_TARGET initialized = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_BIND_TARGET targets[2];
    PAPACC_SIZE count;
    PAPACC_SIZE required;

    if (initialized.address.family != PAPACC_IP_FAMILY_UNSPECIFIED ||
        initialized.scope_id != 0 || initialized.interface_instance_id != 0) {
        return 1;
    }

    selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    if (papacc_bind_targets_resolve(
            &selection, NULL, 0, NULL, 0, &count, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        count != 0 || required != 2) {
        return 2;
    }
    if (papacc_bind_targets_resolve(
            &selection, NULL, 0, targets, 2, &count, &required) !=
            PAPACC_RESULT_OK ||
        count != 2 || required != 2) {
        return 3;
    }
    if (targets[0].address.family != PAPACC_IP_FAMILY_IPV4 ||
        papacc_ip_address_is_unspecified(&targets[0].address) != PAPACC_TRUE ||
        targets[0].scope_id != 0 || targets[0].interface_instance_id != 0) {
        return 4;
    }
    if (targets[1].address.family != PAPACC_IP_FAMILY_IPV6 ||
        papacc_ip_address_is_unspecified(&targets[1].address) != PAPACC_TRUE ||
        targets[1].scope_id != 0 || targets[1].interface_instance_id != 0) {
        return 5;
    }

    return 0;
}

static int papacc_test_selected(void)
{
    const PAPACC_U32 selected_ids[] = {2, 5};
    const PAPACC_U8 ipv6_bytes[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    };
    PAPACC_NETWORK_INTERFACE_ADDRESS entries[9];
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_BIND_TARGET targets[4];
    PAPACC_SIZE count;
    PAPACC_SIZE required;

    papacc_test_set_ipv4(&entries[0], 1, 10, 0, 0, 1);
    papacc_test_set_ipv4(&entries[1], 2, 192, 168, 1, 120);
    papacc_test_set_ipv6(&entries[2], 2, ipv6_bytes, 12);
    papacc_test_set_ipv4(&entries[3], 2, 0, 0, 0, 0);
    papacc_test_set_ipv4(&entries[4], 5, 127, 0, 0, 1);
    entries[4].interface_is_loopback = PAPACC_TRUE;
    entries[4].interface_is_up = PAPACC_FALSE;
    papacc_test_set_ipv4(&entries[5], 2, 192, 168, 1, 120);
    papacc_test_set_ipv6(&entries[6], 2, ipv6_bytes, 12);
    papacc_test_set_ipv6(&entries[7], 2, ipv6_bytes, 13);
    entries[8] = (PAPACC_NETWORK_INTERFACE_ADDRESS)
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    entries[8].interface_instance_id = 2;

    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = selected_ids;
    selection.interface_instance_count = 2;

    if (papacc_bind_targets_resolve(
            &selection, entries, 9, NULL, 0, &count, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        count != 0 || required != 4) {
        return 1;
    }
    if (papacc_bind_targets_resolve(
            &selection, entries, 9, targets, 4, &count, &required) !=
            PAPACC_RESULT_OK ||
        count != 4 || required != 4) {
        return 2;
    }
    if (targets[0].address.family != PAPACC_IP_FAMILY_IPV4 ||
        targets[0].address.bytes[0] != 192 ||
        targets[0].interface_instance_id != 2 || targets[0].scope_id != 0) {
        return 3;
    }
    if (targets[1].address.family != PAPACC_IP_FAMILY_IPV6 ||
        targets[1].address.bytes[0] != 0xfe || targets[1].scope_id != 12 ||
        targets[1].interface_instance_id != 2) {
        return 4;
    }
    if (targets[2].address.family != PAPACC_IP_FAMILY_IPV4 ||
        papacc_ip_address_is_loopback(&targets[2].address) != PAPACC_TRUE ||
        targets[2].interface_instance_id != 5) {
        return 5;
    }
    if (targets[3].address.family != PAPACC_IP_FAMILY_IPV6 ||
        targets[3].scope_id != 13 || targets[3].interface_instance_id != 2) {
        return 6;
    }

    return 0;
}

static int papacc_test_failures(void)
{
    const PAPACC_U32 selected_id[] = {7};
    PAPACC_NETWORK_INTERFACE_ADDRESS entry =
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SIZE count = 99;
    PAPACC_SIZE required = 99;

    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = selected_id;
    selection.interface_instance_count = 1;
    entry.interface_instance_id = 7;

    if (papacc_bind_targets_resolve(
            &selection, &entry, 1, &target, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_STATE ||
        count != 0 || required != 0) {
        return 1;
    }

    papacc_ip_address_set_ipv4(&entry.address, 192, 168, 1, 10);
    target.interface_instance_id = 99;
    if (papacc_bind_targets_resolve(
            &selection, &entry, 1, &target, 0, &count, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        count != 0 || required != 1 || target.interface_instance_id != 99) {
        return 2;
    }
    if (papacc_bind_targets_resolve(
            NULL, &entry, 1, &target, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_bind_targets_resolve(
            &selection, NULL, 1, &target, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_bind_targets_resolve(
            &selection, &entry, 1, NULL, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_bind_targets_resolve(
            &selection, &entry, 1, &target, 1, NULL, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 3;
    }

    return 0;
}

int main(void)
{
    int result;

    result = papacc_test_initializer_and_all();
    if (result != 0) {
        return 10 + result;
    }
    result = papacc_test_selected();
    if (result != 0) {
        return 30 + result;
    }
    result = papacc_test_failures();
    if (result != 0) {
        return 50 + result;
    }

    return 0;
}
