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

PAPACC_RESULT papacc_server_run_win32(
    PAPACC_SERVER_NETWORK *server_network,
    FILE *output,
    FILE *error_output)
{
    const PAPACC_SIZE capacity =
        (PAPACC_SIZE)PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_SERVER_IO_LOOP_WIN32 io_loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_CONNECTION *connection_storage = NULL;
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context_storage = NULL;
    PAPACC_SESSION *session_storage = NULL;
    PAPACC_CHANNEL *channel_storage = NULL;
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *processor_storage = NULL;
    PAPACC_RESULT result = PAPACC_RESULT_OK;
    PAPACC_SIZE index;

    if (server_network == NULL || output == NULL || error_output == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (capacity > SIZE_MAX / sizeof(*connection_storage) ||
        capacity > SIZE_MAX / sizeof(*context_storage) ||
        capacity > SIZE_MAX / sizeof(*session_storage) ||
        capacity > SIZE_MAX / sizeof(*channel_storage) ||
        capacity > SIZE_MAX / sizeof(*processor_storage)) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    connection_storage = (PAPACC_CONNECTION *)malloc(
        capacity * sizeof(*connection_storage));
    context_storage =
        (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *)malloc(
            capacity * sizeof(*context_storage));
    session_storage = (PAPACC_SESSION *)malloc(
        capacity * sizeof(*session_storage));
    channel_storage = (PAPACC_CHANNEL *)malloc(
        capacity * sizeof(*channel_storage));
    processor_storage = (PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *)malloc(
        capacity * sizeof(*processor_storage));
    if (connection_storage == NULL || context_storage == NULL ||
        session_storage == NULL || channel_storage == NULL ||
        processor_storage == NULL) {
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
    result = papacc_server_io_loop_win32_init(
        &io_loop, server_network, &acceptor, session_storage, capacity,
        channel_storage, capacity, processor_storage, capacity,
        (PAPACC_U64)PAPACC_SERVER_ESTABLISHMENT_TIMEOUT_NS);
    if (result != PAPACC_RESULT_OK) goto cleanup;
    result = papacc_server_print_listeners(output, server_network);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }
    fputs("Accepting Control establishment connections.\n",
          output);
    (void)fflush(output);
    while (papacc_server_console_win32_stop_requested() == PAPACC_FALSE) {
        result = papacc_server_io_loop_win32_poll_once(
            &io_loop, PAPACC_SERVER_IO_LOOP_POLL_TIMEOUT_MS);
        if (result != PAPACC_RESULT_OK) {
            fprintf(error_output, "Server I/O Loop failed: %d\n", (int)result);
            (void)fflush(error_output);
            break;
        }
    }

cleanup:
    papacc_server_io_loop_win32_shutdown(&io_loop);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    free(processor_storage);
    free(channel_storage);
    free(session_storage);
    free(context_storage);
    free(connection_storage);
    return result;
}
