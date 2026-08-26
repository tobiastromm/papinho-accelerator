#include <stdlib.h>
#include <stdio.h>

#include "papacc/version.h"
#include "server_cli.h"
#include "server_cli_interfaces.h"
#include "server_console_win32.h"
#include "server_network.h"

static const char *papacc_server_result_name(PAPACC_RESULT result)
{
    switch (result) {
    case PAPACC_RESULT_OK:
        return "OK";
    case PAPACC_RESULT_INVALID_ARGUMENT:
        return "INVALID_ARGUMENT";
    case PAPACC_RESULT_OUT_OF_MEMORY:
        return "OUT_OF_MEMORY";
    case PAPACC_RESULT_NOT_SUPPORTED:
        return "NOT_SUPPORTED";
    case PAPACC_RESULT_INVALID_STATE:
        return "INVALID_STATE";
    case PAPACC_RESULT_LIMIT_EXCEEDED:
        return "LIMIT_EXCEEDED";
    case PAPACC_RESULT_INTERNAL_ERROR:
        return "INTERNAL_ERROR";
    default:
        return "UNKNOWN";
    }
}

static PAPACC_RESULT papacc_server_print_listeners(
    const PAPACC_SERVER_NETWORK *network)
{
    PAPACC_SIZE index;

    if (network == NULL || network->is_active == PAPACC_FALSE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    puts("PapinhoAccelerator Server");
    printf("Software version %s\n\n", papacc_version_string());
    puts("Server network started.\n");
    puts("Listening:");
    for (index = 0; index < network->listener_set.count; ++index) {
        const PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry =
            &network->listener_set.entries[index];
        char address_text[40];
        PAPACC_RESULT result;

        if (entry->socket.is_listening == PAPACC_FALSE) {
            continue;
        }
        result = papacc_ip_address_format(
            &entry->target.address, address_text, sizeof(address_text));
        if (result != PAPACC_RESULT_OK) {
            return result;
        }
        if (entry->target.address.family == PAPACC_IP_FAMILY_IPV6) {
            printf("  IPv6 [%s]:%u", address_text,
                   (unsigned int)network->listener_set.bound_port);
            if (entry->target.scope_id != 0) {
                printf("  Scope ID: %lu",
                       (unsigned long)entry->target.scope_id);
            }
        } else {
            printf("  IPv4 %s:%u", address_text,
                   (unsigned int)network->listener_set.bound_port);
        }
        if (entry->target.interface_instance_id != 0) {
            printf("  Runtime Interface ID: %lu",
                   (unsigned long)entry->target.interface_instance_id);
        }
        putchar('\n');
    }
    puts("\nPress Ctrl+C to stop.");
    return PAPACC_RESULT_OK;
}

/* Takes ownership of CLI storage; Server Network retains no config views. */
static int papacc_server_run(
    const PAPACC_SERVER_CONFIG *config,
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *cli_id_storage)
{
    PAPACC_SERVER_CONSOLE_WIN32 console =
        PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER;
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_RESULT result;
    PAPACC_RESULT uninstall_result;

    result = papacc_server_console_win32_install(&console);
    if (result != PAPACC_RESULT_OK) {
        fprintf(stderr, "Console control failed: %s\n",
                papacc_server_result_name(result));
        free(cli_id_storage);
        return 1;
    }
    result = papacc_server_network_start(&network, config);
    free(cli_id_storage);
    if (result == PAPACC_RESULT_OK) {
        result = papacc_server_print_listeners(&network);
    }
    if (result != PAPACC_RESULT_OK) {
        fprintf(stderr, "Server startup failed: %s\n",
                papacc_server_result_name(result));
        papacc_server_network_shutdown(&network);
        (void)papacc_server_console_win32_uninstall(&console);
        return 1;
    }

    papacc_server_console_win32_wait_for_stop();
    puts("Stopping PapinhoAccelerator Server...");
    papacc_server_network_shutdown(&network);
    uninstall_result = papacc_server_console_win32_uninstall(&console);
    if (uninstall_result != PAPACC_RESULT_OK) {
        fprintf(stderr, "Console cleanup failed: %s\n",
                papacc_server_result_name(uninstall_result));
        return 1;
    }
    puts("Server stopped.");
    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 1) {
        PAPACC_SERVER_CLI_REQUEST request =
            PAPACC_SERVER_CLI_REQUEST_INITIALIZER;
        PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *id_storage = NULL;
        PAPACC_SIZE required_ids = 0;
        PAPACC_RESULT result = papacc_server_cli_request_parse(
            argc, (const char *const *)argv, NULL, 0, &request,
            &required_ids);

        if (result == PAPACC_RESULT_LIMIT_EXCEEDED && required_ids != 0) {
            if (required_ids > SIZE_MAX / sizeof(*id_storage)) {
                result = PAPACC_RESULT_LIMIT_EXCEEDED;
            } else {
                id_storage =
                    (PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *)malloc(
                        required_ids * sizeof(*id_storage));
                if (id_storage == NULL) {
                    result = PAPACC_RESULT_OUT_OF_MEMORY;
                } else {
                    result = papacc_server_cli_request_parse(
                        argc, (const char *const *)argv, id_storage,
                        required_ids, &request, &required_ids);
                }
            }
        }
        if (result != PAPACC_RESULT_OK) {
            fprintf(stderr, "Invalid server command: %s\n",
                    papacc_server_result_name(result));
            free(id_storage);
            return 1;
        }
        if (request.action == PAPACC_SERVER_CLI_ACTION_LIST_INTERFACES) {
            free(id_storage);
            return papacc_server_cli_list_interfaces(stdout) == PAPACC_RESULT_OK
                       ? 0
                       : 1;
        }
        return papacc_server_run(&request.config, id_storage);
    }

    puts("PapinhoAccelerator Server");
    puts("Foundation build");
    printf("Software version %s\n", papacc_version_string());

    return 0;
}
