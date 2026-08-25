#include "network_interface.h"

int main(void)
{
    PAPACC_NETWORK_INTERFACE_ADDRESS entry =
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    PAPACC_SIZE index;

    if (entry.address.family != PAPACC_IP_FAMILY_UNSPECIFIED ||
        entry.interface_instance_id != 0 || entry.interface_index != 0 ||
        entry.scope_id != 0 ||
        entry.interface_is_up != PAPACC_FALSE ||
        entry.interface_is_loopback != PAPACC_FALSE) {
        return 1;
    }
    for (index = 0; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
        if (entry.address.bytes[index] != 0) {
            return 2;
        }
    }

    return 0;
}
