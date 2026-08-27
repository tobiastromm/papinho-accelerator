#ifndef PAPACC_SERVER_RUN_WIN32_H
#define PAPACC_SERVER_RUN_WIN32_H

#include <stdio.h>

#include "server_acceptor_win32.h"
#include "server_console_win32.h"

/*
 * Application-private Phase 2 baseline. This is not a configuration or
 * protocol contract; a future resource policy will replace it.
 */
#define PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY 64U
#define PAPACC_SERVER_ACCEPTOR_POLL_TIMEOUT_MS 50U

/*
 * Runs the single-threaded accept/stop loop for an already-active network.
 * Published PENDING Connections remain idle and owned until shutdown because
 * protocol processing, Session establishment, and handshake timeout do not
 * exist yet. The caller still owns and shuts down server_network afterwards.
 */
PAPACC_RESULT papacc_server_run_win32(
    PAPACC_SERVER_NETWORK *server_network,
    FILE *output,
    FILE *error_output);

#endif
