#include "log.h"

#include "pal_time.h"

static PAPACC_BOOL papacc_log_level_is_valid(PAPACC_LOG_LEVEL level)
{
    return (level >= PAPACC_LOG_DEBUG && level <= PAPACC_LOG_ERROR)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

PAPACC_RESULT papacc_logger_init(
    PAPACC_LOGGER *logger,
    PAPACC_LOG_SINK_FN sink,
    void *sink_context,
    PAPACC_LOG_LEVEL minimum_level)
{
    if (logger == NULL || sink == NULL ||
        papacc_log_level_is_valid(minimum_level) == PAPACC_FALSE) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    logger->sink = sink;
    logger->sink_context = sink_context;
    logger->minimum_level = minimum_level;

    return PAPACC_RESULT_OK;
}

void papacc_log(
    const PAPACC_LOGGER *logger,
    PAPACC_LOG_LEVEL level,
    const char *component,
    const char *message)
{
    PAPACC_LOG_RECORD record;
    PAPACC_RESULT time_result;

    if (logger == NULL || logger->sink == NULL || component == NULL ||
        message == NULL ||
        papacc_log_level_is_valid(level) == PAPACC_FALSE ||
        level < logger->minimum_level) {
        return;
    }

    record.level = level;
    record.component = component;
    record.message = message;
    record.monotonic_timestamp_ns = 0;

    time_result = papacc_pal_monotonic_time_ns(
        &record.monotonic_timestamp_ns);
    record.monotonic_timestamp_valid =
        (time_result == PAPACC_RESULT_OK) ? PAPACC_TRUE : PAPACC_FALSE;

    logger->sink(logger->sink_context, &record);
}
