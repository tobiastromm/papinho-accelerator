#include "persistent_bind_selection.h"
#include "bind_target.h"

static PAPACC_NETWORK_INTERFACE_PERSISTENT_ID persistent_id(
    PAPACC_BOOL is_valid,
    PAPACC_U64 value)
{
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID id = { is_valid, value };
    return id;
}

static PAPACC_NETWORK_INTERFACE interface_record(
    PAPACC_U32 instance_id,
    PAPACC_BOOL persistent_is_valid,
    PAPACC_U64 persistent_value,
    PAPACC_BOOL is_up,
    PAPACC_BOOL is_loopback)
{
    PAPACC_NETWORK_INTERFACE record = PAPACC_NETWORK_INTERFACE_INITIALIZER;
    record.interface_instance_id = instance_id;
    record.persistent_id =
        persistent_id(persistent_is_valid, persistent_value);
    record.is_up = is_up;
    record.is_loopback = is_loopback;
    return record;
}

static int test_validation(void)
{
    PAPACC_PERSISTENT_BIND_SELECTION selection =
        PAPACC_PERSISTENT_BIND_SELECTION_INITIALIZER;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID ids[3];

    if (selection.mode != PAPACC_BIND_SELECTION_UNSPECIFIED ||
        selection.interface_persistent_ids != NULL ||
        selection.interface_persistent_id_count != 0 ||
        papacc_persistent_bind_selection_validate(NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_persistent_bind_selection_validate(&selection) !=
            PAPACC_RESULT_INVALID_STATE) {
        return 1;
    }
    ids[0] = persistent_id(PAPACC_TRUE, 0);
    selection.interface_persistent_ids = ids;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 2;
    }
    selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    selection.interface_persistent_ids = NULL;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_OK) {
        return 3;
    }
    selection.interface_persistent_ids = ids;
    selection.interface_persistent_id_count = 1;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 4;
    }
    selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    selection.interface_persistent_ids = NULL;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 5;
    }
    selection.interface_persistent_ids = ids;
    selection.interface_persistent_id_count = 0;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 6;
    }
    selection.interface_persistent_id_count = 1;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_OK) {
        return 7;
    }
    ids[0] = persistent_id(PAPACC_FALSE, 99);
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 8;
    }
    ids[0] = persistent_id((PAPACC_BOOL)2, 99);
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 9;
    }
    ids[0] = persistent_id(PAPACC_TRUE, 10);
    ids[1] = persistent_id(PAPACC_TRUE, 20);
    ids[2] = persistent_id(PAPACC_TRUE, 10);
    selection.interface_persistent_id_count = 3;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 10;
    }
    selection.mode = (PAPACC_BIND_SELECTION_MODE)99;
    if (papacc_persistent_bind_selection_validate(&selection) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 11;
    }
    return 0;
}

static int test_all(void)
{
    PAPACC_PERSISTENT_BIND_SELECTION persistent =
        PAPACC_PERSISTENT_BIND_SELECTION_INITIALIZER;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_BIND_SELECTION runtime = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_SIZE required = 99;

    persistent.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    if (papacc_persistent_bind_selection_resolve(
            &persistent, &snapshot, NULL, 0, &runtime, &required) !=
            PAPACC_RESULT_OK ||
        runtime.mode != PAPACC_BIND_SELECTION_ALL_INTERFACES ||
        runtime.interface_instance_ids != NULL ||
        runtime.interface_instance_count != 0 || required != 0) {
        return 1;
    }
    return 0;
}

static int test_selected_and_capacity(void)
{
    PAPACC_NETWORK_INTERFACE interfaces[3];
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID ids[2];
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_PERSISTENT_BIND_SELECTION persistent =
        PAPACC_PERSISTENT_BIND_SELECTION_INITIALIZER;
    PAPACC_BIND_SELECTION runtime = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_U32 storage[2] = {99, 99};
    PAPACC_SIZE required = 99;

    interfaces[0] = interface_record(10, PAPACC_TRUE, 100, PAPACC_TRUE,
                                     PAPACC_FALSE);
    interfaces[1] = interface_record(20, PAPACC_TRUE, 200, PAPACC_TRUE,
                                     PAPACC_FALSE);
    interfaces[2] = interface_record(30, PAPACC_TRUE, 300, PAPACC_FALSE,
                                     PAPACC_TRUE);
    ids[0] = persistent_id(PAPACC_TRUE, 300);
    ids[1] = persistent_id(PAPACC_TRUE, 100);
    snapshot.interfaces = interfaces;
    snapshot.interface_count = 3;
    persistent.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    persistent.interface_persistent_ids = ids;
    persistent.interface_persistent_id_count = 2;

    if (papacc_persistent_bind_selection_resolve(
            &persistent, &snapshot, NULL, 0, &runtime, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || required != 2 ||
        runtime.mode != PAPACC_BIND_SELECTION_UNSPECIFIED) {
        return 1;
    }
    if (papacc_persistent_bind_selection_resolve(
            &persistent, &snapshot, storage, 1, &runtime, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || required != 2 ||
        storage[0] != 99 || storage[1] != 99 ||
        runtime.mode != PAPACC_BIND_SELECTION_UNSPECIFIED) {
        return 2;
    }
    if (papacc_persistent_bind_selection_resolve(
            &persistent, &snapshot, storage, 2, &runtime, &required) !=
            PAPACC_RESULT_OK || required != 2 || storage[0] != 30 ||
        storage[1] != 10 ||
        runtime.mode != PAPACC_BIND_SELECTION_SELECTED_INTERFACES ||
        runtime.interface_instance_ids != storage ||
        runtime.interface_instance_count != 2) {
        return 3;
    }
    return 0;
}

static int expect_invalid_state(
    const PAPACC_PERSISTENT_BIND_SELECTION *persistent,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot)
{
    PAPACC_BIND_SELECTION runtime;
    PAPACC_U32 storage = 99;
    PAPACC_SIZE required = 99;

    runtime.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    runtime.interface_instance_ids = &storage;
    runtime.interface_instance_count = 1;
    if (papacc_persistent_bind_selection_resolve(
            persistent, snapshot, &storage, 1, &runtime, &required) !=
            PAPACC_RESULT_INVALID_STATE || required != 0 || storage != 99 ||
        runtime.mode != PAPACC_BIND_SELECTION_UNSPECIFIED ||
        runtime.interface_instance_ids != NULL ||
        runtime.interface_instance_count != 0) {
        return 1;
    }
    return 0;
}

static int test_invalid_snapshots(void)
{
    PAPACC_NETWORK_INTERFACE interfaces[2];
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID id =
        { PAPACC_TRUE, 100 };
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_PERSISTENT_BIND_SELECTION persistent = {
        PAPACC_BIND_SELECTION_SELECTED_INTERFACES, &id, 1
    };
    PAPACC_BIND_SELECTION runtime = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_SIZE required;

    interfaces[0] = interface_record(7, PAPACC_TRUE, 200, PAPACC_TRUE,
                                     PAPACC_FALSE);
    snapshot.interfaces = interfaces;
    snapshot.interface_count = 1;
    if (expect_invalid_state(&persistent, &snapshot) != 0) {
        return 1;
    }
    interfaces[0].persistent_id.is_valid = PAPACC_FALSE;
    interfaces[0].persistent_id.value = 100;
    if (expect_invalid_state(&persistent, &snapshot) != 0) {
        return 2;
    }
    interfaces[0] = interface_record(2, PAPACC_TRUE, 100, PAPACC_TRUE,
                                     PAPACC_FALSE);
    interfaces[1] = interface_record(5, PAPACC_TRUE, 100, PAPACC_TRUE,
                                     PAPACC_FALSE);
    snapshot.interface_count = 2;
    if (expect_invalid_state(&persistent, &snapshot) != 0) {
        return 3;
    }
    interfaces[0] = interface_record(0, PAPACC_TRUE, 100, PAPACC_TRUE,
                                     PAPACC_FALSE);
    snapshot.interface_count = 1;
    if (expect_invalid_state(&persistent, &snapshot) != 0) {
        return 4;
    }
    interfaces[0] = interface_record(7, PAPACC_TRUE, 100, PAPACC_TRUE,
                                     PAPACC_FALSE);
    interfaces[1] = interface_record(7, PAPACC_TRUE, 200, PAPACC_TRUE,
                                     PAPACC_FALSE);
    snapshot.interface_count = 2;
    if (expect_invalid_state(&persistent, &snapshot) != 0) {
        return 5;
    }
    id.value = 0;
    interfaces[0] = interface_record(7, PAPACC_TRUE, 0, PAPACC_FALSE,
                                     PAPACC_TRUE);
    snapshot.interface_count = 1;
    if (papacc_persistent_bind_selection_resolve(
            &persistent, &snapshot, &interfaces[1].interface_instance_id, 1,
            &runtime, &required) != PAPACC_RESULT_OK ||
        interfaces[1].interface_instance_id != 7) {
        return 6;
    }
    if (papacc_persistent_bind_selection_resolve(
            &persistent, NULL, NULL, 0, &runtime, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_persistent_bind_selection_resolve(
            &persistent, &snapshot, NULL, 1, &runtime, &required) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 7;
    }
    return 0;
}

static int test_interface_without_address(void)
{
    PAPACC_NETWORK_INTERFACE interface_value =
        PAPACC_NETWORK_INTERFACE_INITIALIZER;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID id = { PAPACC_TRUE, 42 };
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_PERSISTENT_BIND_SELECTION persistent = {
        PAPACC_BIND_SELECTION_SELECTED_INTERFACES, &id, 1
    };
    PAPACC_BIND_SELECTION runtime = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_U32 storage = 0;
    PAPACC_SIZE count = 99;
    PAPACC_SIZE required = 99;

    interface_value = interface_record(7, PAPACC_TRUE, 42, PAPACC_FALSE,
                                       PAPACC_TRUE);
    snapshot.interfaces = &interface_value;
    snapshot.interface_count = 1;
    if (papacc_persistent_bind_selection_resolve(
            &persistent, &snapshot, &storage, 1, &runtime, &required) !=
            PAPACC_RESULT_OK || storage != 7 || required != 1) {
        return 1;
    }
    if (papacc_bind_targets_resolve(
            &runtime, &snapshot, &target, 1, &count, &required) !=
            PAPACC_RESULT_INVALID_STATE || count != 0 || required != 0) {
        return 2;
    }
    return 0;
}

int main(void)
{
    int result = test_validation();
    if (result != 0) return 10 + result;
    result = test_all();
    if (result != 0) return 30 + result;
    result = test_selected_and_capacity();
    if (result != 0) return 40 + result;
    result = test_invalid_snapshots();
    if (result != 0) return 50 + result;
    result = test_interface_without_address();
    return (result == 0) ? 0 : 70 + result;
}
