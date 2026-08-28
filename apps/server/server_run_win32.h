#ifndef PAPACC_SERVER_RUN_WIN32_H
#define PAPACC_SERVER_RUN_WIN32_H

#include <stdio.h>

#include "server_acceptor_win32.h"
#include "server_console_win32.h"
#include "server_io_loop_win32.h"

/*
 * Application-private Phase 2 baseline. This is not a configuration or
 * protocol contract; a future resource policy will replace it.
 */
#define PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY 64U
#define PAPACC_SERVER_IO_LOOP_POLL_TIMEOUT_MS 50U
#define PAPACC_SERVER_ESTABLISHMENT_TIMEOUT_NS 15000000000ULL

/*
 * Runs the single-threaded combined listener/Control establishment loop.
 * The timeout is application-private policy, not a wire/configuration value.
 */
PAPACC_RESULT papacc_server_run_win32(
    PAPACC_SERVER_NETWORK *server_network,
    FILE *output,
    FILE *error_output);

#endif
