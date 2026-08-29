#include <stdlib.h>
#include <stdio.h>

#include "papacc/version.h"
#include "server_cli.h"
#include "server_cli_interfaces.h"
#include "server_console_win32.h"
#include "server_network.h"
#include "server_run_win32.h"
#include "log.h"

static const char *papacc_server_log_level_name(PAPACC_LOG_LEVEL level)
{
    switch (level) {
    case PAPACC_LOG_DEBUG: return "DEBUG";
    case PAPACC_LOG_INFO: return "INFO";
    case PAPACC_LOG_WARNING: return "WARN";
    case PAPACC_LOG_ERROR: return "ERROR";
    case PAPACC_LOG_LEVEL_OFF: return "OFF";
    default: return "UNKNOWN";
    }
}

static void papacc_server_console_log_sink(
    void *context, const PAPACC_LOG_RECORD *record)
{
    FILE *output = (FILE *)context;
    fprintf(output, "[%s] %s: %s\n",
            papacc_server_log_level_name(record->level),
            record->component, record->message);
    (void)fflush(output);
}

static void papacc_server_print_help(void)
{
    puts("PapinhoAccelerator Server");
    puts("Usage:");
    puts("  papacc_server.exe --help");
    puts("  papacc_server.exe --list-interfaces");
    puts("  papacc_server.exe --port <port> --all-interfaces [--log-level <level>]");
    puts("  papacc_server.exe --port <port> --interface-id <id> [--log-level <level>]");
    puts("Log levels: off, error, warn, info (default), debug");
    puts("  off disables all PAPACC_LOGGER output");
    puts("Example: papacc_server.exe --port 39999 --all-interfaces --log-level info");
}

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
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *cli_id_storage,
    PAPACC_LOG_LEVEL log_level)
{
    PAPACC_SERVER_CONSOLE_WIN32 console =
        PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER;
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_RESULT result;
    PAPACC_RESULT uninstall_result;
    PAPACC_LOGGER logger;

    if (papacc_logger_init(&logger, papacc_server_console_log_sink, stderr,
                           log_level) != PAPACC_RESULT_OK) {
        free(cli_id_storage);
        return 1;
    }

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

    result = papacc_server_run_win32(&network, stdout, stderr, &logger);
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
        if (request.action == PAPACC_SERVER_CLI_ACTION_HELP) {
            free(id_storage);
            papacc_server_print_help();
            return 0;
        }
        return papacc_server_run(
            &request.config, id_storage, request.log_level);
    }

    puts("PapinhoAccelerator Server");
    puts("Foundation build");
    printf("Software version %s\n", papacc_version_string());

    return 0;
}
