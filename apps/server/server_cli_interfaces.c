#include <inttypes.h>
#include <stdlib.h>

#include "server_cli_interfaces.h"

#define PAPACC_SERVER_CLI_DISCOVERY_ATTEMPTS 4
#define PAPACC_SERVER_CLI_ADDRESS_TEXT_SIZE 64

static PAPACC_RESULT papacc_server_cli_print_name(
    const PAPACC_NETWORK_INTERFACE *interface_record,
    FILE *output)
{
    if (interface_record->presentation_name.is_available == PAPACC_TRUE &&
        interface_record->presentation_name.utf8 != NULL) {
        if (fwrite(
                interface_record->presentation_name.utf8, 1,
                interface_record->presentation_name.length, output) !=
            interface_record->presentation_name.length) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
    } else if (fputs("(unnamed interface)", output) == EOF) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_cli_print_interface_snapshot(
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,
    FILE *output)
{
    PAPACC_SIZE interface_index;

    if (snapshot == NULL || output == NULL ||
        (snapshot->interfaces == NULL && snapshot->interface_count != 0) ||
        (snapshot->addresses == NULL && snapshot->address_count != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    for (interface_index = 0;
         interface_index < snapshot->interface_count;
         ++interface_index) {
        const PAPACC_NETWORK_INTERFACE *interface_record =
            &snapshot->interfaces[interface_index];
        PAPACC_SIZE address_index;
        PAPACC_SIZE address_count = 0;
        PAPACC_RESULT result;

        if (fputs("Interface: ", output) == EOF) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        result = papacc_server_cli_print_name(interface_record, output);
        if (result != PAPACC_RESULT_OK) {
            return result;
        }
        if (fprintf(
                output, "\n  Runtime ID: %" PRIu32 "\n",
                interface_record->interface_instance_id) < 0) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        if (interface_record->persistent_id.is_valid == PAPACC_TRUE) {
            if (fprintf(
                    output, "  Persistent ID: %" PRIu64 "\n",
                    interface_record->persistent_id.value) < 0) {
                return PAPACC_RESULT_INTERNAL_ERROR;
            }
        } else if (fputs("  Persistent ID: unavailable\n", output) == EOF) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        if (fprintf(
                output, "  State: %s\n  Loopback: %s\n",
                interface_record->is_up == PAPACC_TRUE ? "UP" : "DOWN",
                interface_record->is_loopback == PAPACC_TRUE ? "yes" : "no") <
            0) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }

        for (address_index = 0;
             address_index < snapshot->address_count;
             ++address_index) {
            const PAPACC_NETWORK_INTERFACE_ADDRESS *address =
                &snapshot->addresses[address_index];
            char text[PAPACC_SERVER_CLI_ADDRESS_TEXT_SIZE];
            const char *label;

            if (address->interface_instance_id !=
                interface_record->interface_instance_id) {
                continue;
            }
            if (address->address.family == PAPACC_IP_FAMILY_IPV4) {
                label = "IPv4";
            } else if (address->address.family == PAPACC_IP_FAMILY_IPV6) {
                label = "IPv6";
            } else {
                continue;
            }
            result = papacc_ip_address_format(
                &address->address, text, sizeof(text));
            if (result != PAPACC_RESULT_OK) {
                return result;
            }
            if (fprintf(output, "  %s: %s", label, text) < 0) {
                return PAPACC_RESULT_INTERNAL_ERROR;
            }
            if (address->address.family == PAPACC_IP_FAMILY_IPV6 &&
                address->scope_id != 0 &&
                fprintf(output, "    Scope ID: %" PRIu32, address->scope_id) <
                    0) {
                return PAPACC_RESULT_INTERNAL_ERROR;
            }
            if (fputc('\n', output) == EOF) {
                return PAPACC_RESULT_INTERNAL_ERROR;
            }
            ++address_count;
        }
        if (address_count == 0 && fputs("  Addresses: none\n", output) == EOF) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        if (fputc('\n', output) == EOF) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
    }
    if (fputs(
            "Use an interface with:\n"
            "  --interface-id <Persistent ID>\n\n"
            "Use all interfaces with:\n"
            "  --all-interfaces\n",
            output) == EOF) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_cli_list_interfaces(FILE *output)
{
    PAPACC_NETWORK_INTERFACE *interfaces = NULL;
    PAPACC_NETWORK_INTERFACE_ADDRESS *addresses = NULL;
    char *presentation = NULL;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_SIZE interface_required = 0;
    PAPACC_SIZE address_required = 0;
    PAPACC_SIZE presentation_required = 0;
    PAPACC_RESULT result;
    int attempt;

    if (output == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    result = papacc_network_discover_local_snapshot(
        NULL, 0, NULL, 0, NULL, 0, &snapshot, &interface_required,
        &address_required, &presentation_required);
    if (result != PAPACC_RESULT_OK &&
        result != PAPACC_RESULT_LIMIT_EXCEEDED) {
        return result;
    }

    for (attempt = 0; attempt < PAPACC_SERVER_CLI_DISCOVERY_ATTEMPTS;
         ++attempt) {
        free(interfaces);
        free(addresses);
        free(presentation);
        interfaces = NULL;
        addresses = NULL;
        presentation = NULL;
        if (interface_required > SIZE_MAX / sizeof(*interfaces) ||
            address_required > SIZE_MAX / sizeof(*addresses)) {
            result = PAPACC_RESULT_LIMIT_EXCEEDED;
            break;
        }
        if (interface_required != 0) {
            interfaces = (PAPACC_NETWORK_INTERFACE *)malloc(
                interface_required * sizeof(*interfaces));
        }
        if (address_required != 0) {
            addresses = (PAPACC_NETWORK_INTERFACE_ADDRESS *)malloc(
                address_required * sizeof(*addresses));
        }
        if (presentation_required != 0) {
            presentation = (char *)malloc(presentation_required);
        }
        if ((interface_required != 0 && interfaces == NULL) ||
            (address_required != 0 && addresses == NULL) ||
            (presentation_required != 0 && presentation == NULL)) {
            result = PAPACC_RESULT_OUT_OF_MEMORY;
            break;
        }
        result = papacc_network_discover_local_snapshot(
            interfaces, interface_required, addresses, address_required,
            presentation, presentation_required, &snapshot,
            &interface_required, &address_required, &presentation_required);
        if (result != PAPACC_RESULT_LIMIT_EXCEEDED) {
            break;
        }
    }
    if (result == PAPACC_RESULT_OK) {
        result = papacc_server_cli_print_interface_snapshot(&snapshot, output);
    }
    free(interfaces);
    free(addresses);
    free(presentation);
    return result;
}
