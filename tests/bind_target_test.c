#include "bind_target.h"

static void set_interface(
    PAPACC_NETWORK_INTERFACE *interface_record,
    PAPACC_U32 instance_id,
    PAPACC_BOOL is_up,
    PAPACC_BOOL is_loopback)
{
    *interface_record =
        (PAPACC_NETWORK_INTERFACE)PAPACC_NETWORK_INTERFACE_INITIALIZER;
    interface_record->interface_instance_id = instance_id;
    interface_record->is_up = is_up;
    interface_record->is_loopback = is_loopback;
}

static void set_ipv4(
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

static void set_ipv6(
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

static int test_all_interfaces(void)
{
    PAPACC_BIND_TARGET initialized = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_BIND_TARGET targets[2];
    PAPACC_SIZE count;
    PAPACC_SIZE required;

    if (initialized.address.family != PAPACC_IP_FAMILY_UNSPECIFIED ||
        initialized.scope_id != 0 || initialized.interface_instance_id != 0) {
        return 1;
    }
    selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    if (papacc_bind_targets_resolve(
            &selection, &snapshot, NULL, 0, &count, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || count != 0 || required != 2) {
        return 2;
    }
    if (papacc_bind_targets_resolve(
            &selection, &snapshot, targets, 2, &count, &required) !=
            PAPACC_RESULT_OK || count != 2 || required != 2) {
        return 3;
    }
    if (targets[0].address.family != PAPACC_IP_FAMILY_IPV4 ||
        papacc_ip_address_is_unspecified(&targets[0].address) != PAPACC_TRUE ||
        targets[0].scope_id != 0 || targets[0].interface_instance_id != 0 ||
        targets[1].address.family != PAPACC_IP_FAMILY_IPV6 ||
        papacc_ip_address_is_unspecified(&targets[1].address) != PAPACC_TRUE ||
        targets[1].scope_id != 0 || targets[1].interface_instance_id != 0) {
        return 4;
    }
    return 0;
}

static int test_selected(void)
{
    const PAPACC_U32 selected_ids[] = {2, 5};
    const PAPACC_U8 ipv6[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    };
    PAPACC_NETWORK_INTERFACE interfaces[3];
    PAPACC_NETWORK_INTERFACE_ADDRESS addresses[9];
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_BIND_TARGET targets[4];
    PAPACC_SIZE count;
    PAPACC_SIZE required;

    set_interface(&interfaces[0], 1, PAPACC_TRUE, PAPACC_FALSE);
    set_interface(&interfaces[1], 2, PAPACC_FALSE, PAPACC_FALSE);
    set_interface(&interfaces[2], 5, PAPACC_TRUE, PAPACC_TRUE);
    set_ipv4(&addresses[0], 1, 10, 0, 0, 1);
    set_ipv4(&addresses[1], 2, 192, 168, 1, 120);
    set_ipv6(&addresses[2], 2, ipv6, 12);
    set_ipv4(&addresses[3], 2, 0, 0, 0, 0);
    set_ipv4(&addresses[4], 5, 127, 0, 0, 1);
    set_ipv4(&addresses[5], 2, 192, 168, 1, 120);
    set_ipv6(&addresses[6], 2, ipv6, 12);
    set_ipv6(&addresses[7], 2, ipv6, 13);
    addresses[8] = (PAPACC_NETWORK_INTERFACE_ADDRESS)
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    addresses[8].interface_instance_id = 2;
    snapshot.interfaces = interfaces;
    snapshot.interface_count = 3;
    snapshot.addresses = addresses;
    snapshot.address_count = 9;
    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = selected_ids;
    selection.interface_instance_count = 2;

    if (papacc_bind_targets_resolve(
            &selection, &snapshot, NULL, 0, &count, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || count != 0 || required != 4) {
        return 1;
    }
    if (papacc_bind_targets_resolve(
            &selection, &snapshot, targets, 4, &count, &required) !=
            PAPACC_RESULT_OK || count != 4 || required != 4) {
        return 2;
    }
    if (targets[0].address.family != PAPACC_IP_FAMILY_IPV4 ||
        targets[0].address.bytes[0] != 192 ||
        targets[0].interface_instance_id != 2 || targets[0].scope_id != 0 ||
        targets[1].address.family != PAPACC_IP_FAMILY_IPV6 ||
        targets[1].scope_id != 12 || targets[1].interface_instance_id != 2 ||
        papacc_ip_address_is_loopback(&targets[2].address) != PAPACC_TRUE ||
        targets[2].interface_instance_id != 5 ||
        targets[3].address.family != PAPACC_IP_FAMILY_IPV6 ||
        targets[3].scope_id != 13 || targets[3].interface_instance_id != 2) {
        return 3;
    }
    return 0;
}

static int expect_invalid_state(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot)
{
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SIZE count = 99;
    PAPACC_SIZE required = 99;

    target.interface_instance_id = 99;
    if (papacc_bind_targets_resolve(
            selection, snapshot, &target, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_STATE ||
        count != 0 || required != 0 || target.interface_instance_id != 99) {
        return 1;
    }
    return 0;
}

static int test_failures(void)
{
    const PAPACC_U32 one_id[] = {7};
    const PAPACC_U32 two_ids[] = {7, 8};
    PAPACC_NETWORK_INTERFACE interfaces[2];
    PAPACC_NETWORK_INTERFACE_ADDRESS addresses[2];
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_BIND_SELECTION selection = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SIZE count = 99;
    PAPACC_SIZE required = 99;

    set_interface(&interfaces[0], 7, PAPACC_TRUE, PAPACC_FALSE);
    set_interface(&interfaces[1], 8, PAPACC_TRUE, PAPACC_FALSE);
    addresses[0] = (PAPACC_NETWORK_INTERFACE_ADDRESS)
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    addresses[0].interface_instance_id = 7;
    set_ipv4(&addresses[1], 7, 192, 168, 1, 10);
    snapshot.interfaces = interfaces;
    snapshot.interface_count = 2;
    snapshot.addresses = addresses;
    snapshot.address_count = 1;
    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_instance_ids = one_id;
    selection.interface_instance_count = 1;

    if (expect_invalid_state(&selection, &snapshot) != 0) {
        return 1;
    }
    snapshot.address_count = 2;
    target.interface_instance_id = 99;
    if (papacc_bind_targets_resolve(
            &selection, &snapshot, &target, 0, &count, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || count != 0 || required != 1 ||
        target.interface_instance_id != 99) {
        return 2;
    }

    selection.interface_instance_ids = two_ids;
    selection.interface_instance_count = 2;
    if (expect_invalid_state(&selection, &snapshot) != 0) {
        return 3;
    }
    if (papacc_bind_targets_resolve(
            NULL, &snapshot, &target, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_bind_targets_resolve(
            &selection, NULL, &target, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_bind_targets_resolve(
            &selection, &snapshot, NULL, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_bind_targets_resolve(
            &selection, &snapshot, &target, 1, NULL, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 4;
    }
    return 0;
}

int main(void)
{
    int result = test_all_interfaces();
    if (result != 0) {
        return 10 + result;
    }
    result = test_selected();
    if (result != 0) {
        return 30 + result;
    }
    result = test_failures();
    return (result == 0) ? 0 : 50 + result;
}
