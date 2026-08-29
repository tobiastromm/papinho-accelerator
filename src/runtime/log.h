#ifndef PAPACC_RUNTIME_LOG_H
#define PAPACC_RUNTIME_LOG_H

#include "papacc/types.h"

typedef enum PAPACC_LOG_LEVEL {
    PAPACC_LOG_DEBUG = 0,
    PAPACC_LOG_INFO = 1,
    PAPACC_LOG_WARNING = 2,
    PAPACC_LOG_ERROR = 3,
    PAPACC_LOG_LEVEL_OFF = 4
} PAPACC_LOG_LEVEL;

typedef struct PAPACC_LOG_RECORD {
    PAPACC_LOG_LEVEL level;
    const char *component;
    const char *message;
    PAPACC_U64 monotonic_timestamp_ns;
    PAPACC_BOOL monotonic_timestamp_valid;
} PAPACC_LOG_RECORD;

typedef void (*PAPACC_LOG_SINK_FN)(
    void *context,
    const PAPACC_LOG_RECORD *record);

typedef struct PAPACC_LOGGER {
    PAPACC_LOG_SINK_FN sink;
    void *sink_context;
    PAPACC_LOG_LEVEL minimum_level;
} PAPACC_LOGGER;

/*
 * The logger and sink context are supplied explicitly; no global logger is
 * used. Thread-safety and synchronization policy are intentionally undefined
 * until threading is introduced by the runtime.
 * PAPACC_LOG_LEVEL_OFF is a runtime disabled state: papacc_log returns before
 * timestamp acquisition or sink delivery for every message severity.
 */
PAPACC_RESULT papacc_logger_init(
    PAPACC_LOGGER *logger,
    PAPACC_LOG_SINK_FN sink,
    void *sink_context,
    PAPACC_LOG_LEVEL minimum_level);

/*
 * Logging is best-effort and never replaces the caller's original result.
 * Invalid arguments are ignored. If monotonic time cannot be read, the record
 * is still delivered with timestamp 0 and monotonic_timestamp_valid false.
 *
 * component and message are borrowed, read-only strings. Their validity is
 * guaranteed only for the duration of the sink callback. Neither logger nor
 * sink owns or frees them; a sink that retains text must make its own copy.
 */
void papacc_log(
    const PAPACC_LOGGER *logger,
    PAPACC_LOG_LEVEL level,
    const char *component,
    const char *message);

#endif
