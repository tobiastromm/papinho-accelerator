#include <stdlib.h>
#include <string.h>

#include "server_cli_interfaces.h"

static int read_output(FILE *file, char *buffer, PAPACC_SIZE capacity)
{
    long length;

    if (fflush(file) != 0 || fseek(file, 0, SEEK_END) != 0) {
        return 1;
    }
    length = ftell(file);
    if (length < 0 || (PAPACC_SIZE)length >= capacity ||
        fseek(file, 0, SEEK_SET) != 0) {
        return 1;
    }
    if (fread(buffer, 1, (PAPACC_SIZE)length, file) != (PAPACC_SIZE)length) {
        return 1;
    }
    buffer[length] = '\0';
    return 0;
}

static int test_synthetic_snapshot(void)
{
    static const char ethernet[] = "Ethernet";
    static const char utf8_name[] = "Conex\xc3\xa3o";
    const PAPACC_U8 ipv6[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    };
    PAPACC_NETWORK_INTERFACE interfaces[3] = {
        PAPACC_NETWORK_INTERFACE_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_INITIALIZER
    };
    PAPACC_NETWORK_INTERFACE_ADDRESS addresses[3] = {
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER,
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER
    };
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot = {
        interfaces, 3, addresses, 3
    };
    char output[4096];
    FILE *file = tmpfile();

    if (file == NULL) {
        return 1;
    }
    interfaces[0].interface_instance_id = 1;
    interfaces[0].persistent_id.is_valid = PAPACC_TRUE;
    interfaces[0].persistent_id.value = 123;
    interfaces[0].presentation_name.is_available = PAPACC_TRUE;
    interfaces[0].presentation_name.utf8 = ethernet;
    interfaces[0].presentation_name.length = sizeof(ethernet) - 1;
    interfaces[0].is_up = PAPACC_TRUE;

    interfaces[1].interface_instance_id = 2;

    interfaces[2].interface_instance_id = 3;
    interfaces[2].persistent_id.is_valid = PAPACC_TRUE;
    interfaces[2].persistent_id.value = 0;
    interfaces[2].presentation_name.is_available = PAPACC_TRUE;
    interfaces[2].presentation_name.utf8 = utf8_name;
    interfaces[2].presentation_name.length = sizeof(utf8_name) - 1;
    interfaces[2].is_up = PAPACC_TRUE;
    interfaces[2].is_loopback = PAPACC_TRUE;

    addresses[0].interface_instance_id = 1;
    papacc_ip_address_set_ipv4(&addresses[0].address, 192, 168, 1, 50);
    addresses[1].interface_instance_id = 1;
    papacc_ip_address_set_ipv6(&addresses[1].address, ipv6);
    addresses[1].scope_id = 12;
    addresses[2].interface_instance_id = 3;
    papacc_ip_address_set_ipv4(&addresses[2].address, 127, 0, 0, 1);

    if (papacc_server_cli_print_interface_snapshot(&snapshot, file) !=
            PAPACC_RESULT_OK ||
        read_output(file, output, sizeof(output)) != 0) {
        fclose(file);
        return 2;
    }
    fclose(file);
    if (strstr(output, "Interface: Ethernet") == NULL ||
        strstr(output, "Runtime ID: 1") == NULL ||
        strstr(output, "Persistent ID: 123") == NULL ||
        strstr(output, "State: UP") == NULL ||
        strstr(output, "Loopback: no") == NULL ||
        strstr(output, "IPv4: 192.168.1.50") == NULL ||
        strstr(output, "IPv6: 2001:0db8:0000:0000:0000:0000:0000:0001") ==
            NULL ||
        strstr(output, "Scope ID: 12") == NULL ||
        strstr(output, "Interface: (unnamed interface)") == NULL ||
        strstr(output, "Persistent ID: unavailable") == NULL ||
        strstr(output, "State: DOWN") == NULL ||
        strstr(output, "Addresses: none") == NULL ||
        strstr(output, utf8_name) == NULL ||
        strstr(output, "Persistent ID: 0") == NULL ||
        strstr(output, "Loopback: yes") == NULL ||
        strstr(output, "IPv4: 127.0.0.1") == NULL) {
        return 3;
    }
    return 0;
}

static int test_real_discovery(void)
{
    FILE *file = tmpfile();
    PAPACC_RESULT result;

    if (file == NULL) {
        return 1;
    }
    result = papacc_server_cli_list_interfaces(file);
    fclose(file);
    return result == PAPACC_RESULT_OK ? 0 : 2;
}

int main(void)
{
    int result = test_synthetic_snapshot();
    if (result != 0) {
        return 10 + result;
    }
    result = test_real_discovery();
    return result == 0 ? 0 : 20 + result;
}
