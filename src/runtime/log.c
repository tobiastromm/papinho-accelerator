#include "log.h"

#include "pal_time.h"

static PAPACC_BOOL papacc_log_level_is_valid(PAPACC_LOG_LEVEL level)
{
    return (level >= PAPACC_LOG_LEVEL_OFF && level <= PAPACC_LOG_TRACE)
               ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_logger_init(PAPACC_LOGGER *logger,
    PAPACC_LOG_SINK_FN sink, void *sink_context,
    PAPACC_LOG_LEVEL minimum_level)
{
    if (logger == NULL || sink == NULL ||
        papacc_log_level_is_valid(minimum_level) == PAPACC_FALSE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    logger->sink = sink;
    logger->sink_context = sink_context;
    logger->minimum_level = minimum_level;
    return PAPACC_RESULT_OK;
}

void papacc_log_event(const PAPACC_LOGGER *logger, PAPACC_LOG_LEVEL level,
    PAPACC_LOG_EVENT_ID event_id, PAPACC_LOG_CATEGORY category,
    PAPACC_LOG_COMPONENT_ID component_id, PAPACC_LOG_OPERATION operation,
    PAPACC_BOOL result_valid, PAPACC_RESULT result,
    const PAPACC_LOG_CONTEXT *context, const char *message)
{
    PAPACC_LOG_RECORD record;
    PAPACC_RESULT time_result;
    if (logger == NULL || logger->sink == NULL || message == NULL ||
        papacc_log_level_is_valid(level) == PAPACC_FALSE ||
        logger->minimum_level == PAPACC_LOG_LEVEL_OFF ||
        level == PAPACC_LOG_LEVEL_OFF || level > logger->minimum_level)
        return;
    record.level = level;
    record.event_id = event_id;
    record.category = category;
    record.component_id = component_id;
    record.operation = operation;
    record.result = result;
    record.result_valid = result_valid;
    record.message = message;
    record.context = context != NULL
        ? *context : (PAPACC_LOG_CONTEXT)PAPACC_LOG_CONTEXT_INITIALIZER;
    record.monotonic_timestamp_ns = 0;
    time_result = papacc_pal_monotonic_time_ns(&record.monotonic_timestamp_ns);
    record.monotonic_timestamp_valid =
        time_result == PAPACC_RESULT_OK ? PAPACC_TRUE : PAPACC_FALSE;
    logger->sink(logger->sink_context, &record);
}
