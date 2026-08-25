#include "runtime.h"

PAPACC_RESULT papacc_runtime_init(
    PAPACC_RUNTIME *runtime,
    const PAPACC_RUNTIME_OPTIONS *options)
{
    PAPACC_RUNTIME initialized;

    if (runtime == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (runtime->state != PAPACC_RUNTIME_UNINITIALIZED) {
        return PAPACC_RESULT_INVALID_STATE;
    }

    initialized.state = PAPACC_RUNTIME_READY;
    initialized.logger = (options != NULL) ? options->logger : NULL;
    *runtime = initialized;

    if (runtime->logger != NULL) {
        papacc_log(
            runtime->logger,
            PAPACC_LOG_INFO,
            "runtime",
            "Runtime initialized");
    }

    return PAPACC_RESULT_OK;
}

void papacc_runtime_shutdown(PAPACC_RUNTIME *runtime)
{
    if (runtime == NULL || runtime->state == PAPACC_RUNTIME_SHUTDOWN) {
        return;
    }

    if (runtime->state == PAPACC_RUNTIME_READY && runtime->logger != NULL) {
        papacc_log(
            runtime->logger,
            PAPACC_LOG_INFO,
            "runtime",
            "Runtime shutdown");
    }

    runtime->logger = NULL;
    runtime->state = PAPACC_RUNTIME_SHUTDOWN;
}
