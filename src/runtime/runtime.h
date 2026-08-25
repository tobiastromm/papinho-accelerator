#ifndef PAPACC_RUNTIME_RUNTIME_H
#define PAPACC_RUNTIME_RUNTIME_H

#include "log.h"

typedef enum PAPACC_RUNTIME_STATE {
    PAPACC_RUNTIME_UNINITIALIZED = 0,
    PAPACC_RUNTIME_READY = 1,
    PAPACC_RUNTIME_SHUTDOWN = 2
} PAPACC_RUNTIME_STATE;

typedef struct PAPACC_RUNTIME_OPTIONS {
    PAPACC_LOGGER *logger;
} PAPACC_RUNTIME_OPTIONS;

typedef struct PAPACC_RUNTIME {
    PAPACC_RUNTIME_STATE state;
    PAPACC_LOGGER *logger;
} PAPACC_RUNTIME;

#define PAPACC_RUNTIME_INITIALIZER \
    { PAPACC_RUNTIME_UNINITIALIZED, NULL }

/*
 * The caller owns PAPACC_RUNTIME storage and must construct it with
 * PAPACC_RUNTIME_INITIALIZER before first use. options may be NULL; logging is
 * optional. A supplied logger is borrowed and must remain valid through the
 * matching shutdown call. The runtime never frees or destroys it.
 *
 * Initialization commits the READY state only after validation succeeds.
 * Failed initialization leaves an already valid object unchanged. A runtime
 * cannot be initialized twice or reinitialized after shutdown.
 */
PAPACC_RESULT papacc_runtime_init(
    PAPACC_RUNTIME *runtime,
    const PAPACC_RUNTIME_OPTIONS *options);

/*
 * Shutdown is deterministic and idempotent. NULL, uninitialized, and already
 * shut down runtimes are safe. No concurrent access guarantee is provided in
 * this phase.
 */
void papacc_runtime_shutdown(PAPACC_RUNTIME *runtime);

#endif
