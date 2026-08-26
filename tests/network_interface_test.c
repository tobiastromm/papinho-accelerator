#include "network_interface.h"

int main(void)
{
    PAPACC_NETWORK_INTERFACE_ADDRESS entry =
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    PAPACC_NETWORK_INTERFACE interface_record =
        PAPACC_NETWORK_INTERFACE_INITIALIZER;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID valid = { PAPACC_TRUE, 42 };
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID same = { PAPACC_TRUE, 42 };
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID different = { PAPACC_TRUE, 43 };
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID valid_zero = { PAPACC_TRUE, 0 };
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID invalid =
        PAPACC_NETWORK_INTERFACE_PERSISTENT_ID_INITIALIZER;
    static const char utf8_name[] = "Ol\xc3\xa1";
    PAPACC_SIZE index;

    if (interface_record.interface_instance_id != 0 ||
        interface_record.persistent_id.is_valid != PAPACC_FALSE ||
        interface_record.persistent_id.value != 0 ||
        interface_record.presentation_name.is_available != PAPACC_FALSE ||
        interface_record.presentation_name.utf8 != NULL ||
        interface_record.presentation_name.length != 0 ||
        interface_record.is_up != PAPACC_FALSE ||
        interface_record.is_loopback != PAPACC_FALSE ||
        snapshot.interfaces != NULL || snapshot.interface_count != 0 ||
        snapshot.addresses != NULL || snapshot.address_count != 0) {
        return 1;
    }

    interface_record.presentation_name.is_available = PAPACC_TRUE;
    interface_record.presentation_name.utf8 = utf8_name;
    interface_record.presentation_name.length = sizeof(utf8_name) - 1;
    if (interface_record.presentation_name.length != 4 ||
        interface_record.presentation_name.utf8[4] != '\0') {
        return 2;
    }

    snapshot.interfaces = &interface_record;
    snapshot.interface_count = 1;
    if (snapshot.interface_count != 1 || snapshot.address_count != 0) {
        return 3;
    }

    if (entry.address.family != PAPACC_IP_FAMILY_UNSPECIFIED ||
        entry.interface_instance_id != 0 || entry.interface_index != 0 ||
        entry.interface_persistent_id.is_valid != PAPACC_FALSE ||
        entry.interface_persistent_id.value != 0 ||
        entry.scope_id != 0 ||
        entry.interface_is_up != PAPACC_FALSE ||
        entry.interface_is_loopback != PAPACC_FALSE) {
        return 4;
    }
    for (index = 0; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
        if (entry.address.bytes[index] != 0) {
            return 5;
        }
    }

    if (papacc_network_interface_persistent_id_equal(&valid, &same) !=
            PAPACC_TRUE ||
        papacc_network_interface_persistent_id_equal(&valid, &different) !=
            PAPACC_FALSE ||
        papacc_network_interface_persistent_id_equal(&invalid, &valid) !=
            PAPACC_FALSE ||
        papacc_network_interface_persistent_id_equal(&invalid, &invalid) !=
            PAPACC_FALSE ||
        papacc_network_interface_persistent_id_equal(&valid_zero, &valid_zero) !=
            PAPACC_TRUE ||
        papacc_network_interface_persistent_id_equal(NULL, &valid) !=
            PAPACC_FALSE) {
        return 6;
    }

    return 0;
}
