#include "server_run_win32.h"

#include <stdlib.h>

#include "papacc/version.h"

static PAPACC_RESULT papacc_server_print_listeners(
    FILE *output,
    const PAPACC_SERVER_NETWORK *network)
{
    PAPACC_SIZE index;

    fputs("PapinhoAccelerator Server\n", output);
    fprintf(output, "Software version %s\n\n", papacc_version_string());
    fputs("Server network started.\n\nListening:\n", output);
    for (index = 0; index < network->listener_set.count; ++index) {
        const PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry =
            &network->listener_set.entries[index];
        char address_text[40];
        PAPACC_RESULT result = papacc_ip_address_format(
            &entry->target.address, address_text, sizeof(address_text));
        if (result != PAPACC_RESULT_OK) {
            return result;
        }
        if (entry->target.address.family == PAPACC_IP_FAMILY_IPV6) {
            fprintf(output, "  IPv6 [%s]:%u", address_text,
                    (unsigned int)network->listener_set.bound_port);
            if (entry->target.scope_id != 0) {
                fprintf(output, "  Scope ID: %lu",
                        (unsigned long)entry->target.scope_id);
            }
        } else {
            fprintf(output, "  IPv4 %s:%u", address_text,
                    (unsigned int)network->listener_set.bound_port);
        }
        if (entry->target.interface_instance_id != 0) {
            fprintf(output, "  Runtime Interface ID: %lu",
                    (unsigned long)entry->target.interface_instance_id);
        }
        fputc('\n', output);
    }
    fputs("\nPress Ctrl+C to stop.\n", output);
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_server_print_endpoint(
    FILE *output,
    const char *label,
    const PAPACC_NETWORK_ENDPOINT *endpoint)
{
    char address_text[40];
    PAPACC_RESULT result = papacc_ip_address_format(
        &endpoint->address, address_text, sizeof(address_text));

    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (endpoint->address.family == PAPACC_IP_FAMILY_IPV6) {
        fprintf(output, "  %s: [%s]:%u", label, address_text,
                (unsigned int)endpoint->port);
        if (endpoint->scope_id != 0) {
            fprintf(output, "  Scope ID: %lu",
                    (unsigned long)endpoint->scope_id);
        }
        fputc('\n', output);
    } else {
        fprintf(output, "  %s: %s:%u\n", label, address_text,
                (unsigned int)endpoint->port);
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_server_print_connection(
    FILE *output,
    const PAPACC_CONNECTION *connection)
{
    PAPACC_RESULT result;

    fprintf(output, "Connection ID: %llu\n",
            (unsigned long long)connection->connection_instance_id);
    result = papacc_server_print_endpoint(
        output, "Remote", &connection->remote_endpoint);
    if (result == PAPACC_RESULT_OK) {
        result = papacc_server_print_endpoint(
            output, "Local", &connection->local_endpoint);
    }
    if (result == PAPACC_RESULT_OK) {
        fputs("  State: PENDING\n", output);
        (void)fflush(output);
    }
    return result;
}

PAPACC_RESULT papacc_server_run_win32(
    PAPACC_SERVER_NETWORK *server_network,
    FILE *output,
    FILE *error_output)
{
    const PAPACC_SIZE capacity =
        (PAPACC_SIZE)PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_CONNECTION *connection_storage = NULL;
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context_storage = NULL;
    PAPACC_RESULT result = PAPACC_RESULT_OK;
    PAPACC_SIZE index;

    if (server_network == NULL || output == NULL || error_output == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (capacity > SIZE_MAX / sizeof(*connection_storage) ||
        capacity > SIZE_MAX / sizeof(*context_storage)) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    connection_storage = (PAPACC_CONNECTION *)malloc(
        capacity * sizeof(*connection_storage));
    context_storage =
        (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *)malloc(
            capacity * sizeof(*context_storage));
    if (connection_storage == NULL || context_storage == NULL) {
        result = PAPACC_RESULT_OUT_OF_MEMORY;
        goto cleanup;
    }
    for (index = 0; index < capacity; ++index) {
        context_storage[index] =
            (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
                PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    }
    result = papacc_server_acceptor_win32_init(
        &acceptor, server_network, connection_storage, capacity,
        context_storage, capacity);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }
    result = papacc_server_print_listeners(output, server_network);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }
    fputs("Accepting transport connections.\n"
          "Protocol processing is not yet enabled.\n",
          output);
    (void)fflush(output);
    while (papacc_server_console_win32_stop_requested() == PAPACC_FALSE) {
        PAPACC_CONNECTION *new_connection = NULL;
        result = papacc_server_acceptor_win32_poll_once(
            &acceptor, PAPACC_SERVER_ACCEPTOR_POLL_TIMEOUT_MS,
            &new_connection);
        if (result == PAPACC_RESULT_LIMIT_EXCEEDED) {
            fputs("Connection rejected: connection capacity reached.\n",
                  error_output);
            (void)fflush(error_output);
            result = PAPACC_RESULT_OK;
            continue;
        }
        if (result != PAPACC_RESULT_OK) {
            fprintf(error_output, "Acceptor poll failed: %d\n", (int)result);
            (void)fflush(error_output);
            break;
        }
        if (new_connection != NULL) {
            result = papacc_server_print_connection(output, new_connection);
            if (result != PAPACC_RESULT_OK) {
                fprintf(error_output,
                        "Connection presentation failed: %d\n", (int)result);
                (void)fflush(error_output);
                break;
            }
        }
    }

cleanup:
    papacc_server_acceptor_win32_shutdown(&acceptor);
    free(context_storage);
    free(connection_storage);
    return result;
}
