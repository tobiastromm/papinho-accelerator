#include <stdlib.h>
#include <stdio.h>

#include "papacc/version.h"
#include "server_cli.h"
#include "server_cli_interfaces.h"
#include "server_console_win32.h"
#include "server_network.h"
#include "server_run_win32.h"

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
    if (result != PAPACC_RESULT_OK) {
        fprintf(stderr, "Server startup failed: %s\n",
                papacc_server_result_name(result));
        papacc_server_network_shutdown(&network);
        (void)papacc_server_console_win32_uninstall(&console);
        return 1;
    }

    result = papacc_server_run_win32(&network, stdout, stderr);
    puts("Stopping PapinhoAccelerator Server...");
    papacc_server_network_shutdown(&network);
    uninstall_result = papacc_server_console_win32_uninstall(&console);
    if (uninstall_result != PAPACC_RESULT_OK) {
        fprintf(stderr, "Console cleanup failed: %s\n",
                papacc_server_result_name(uninstall_result));
        return 1;
    }
    puts("Server stopped.");
    return result == PAPACC_RESULT_OK ? 0 : 1;
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
