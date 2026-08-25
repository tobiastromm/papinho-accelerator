#include <string.h>

#include "log.h"
#include "pal_time.h"

typedef struct PAPACC_TEST_SINK {
    PAPACC_SIZE call_count;
    void *received_context;
    PAPACC_LOG_RECORD record;
} PAPACC_TEST_SINK;

static void papacc_test_sink_callback(
    void *context,
    const PAPACC_LOG_RECORD *record)
{
    PAPACC_TEST_SINK *sink = (PAPACC_TEST_SINK *)context;

    ++sink->call_count;
    sink->received_context = context;
    sink->record = *record;
}

int main(void)
{
    PAPACC_TEST_SINK sink = {0};
    PAPACC_LOGGER logger;
    PAPACC_U64 before;
    PAPACC_U64 after;

    if (papacc_logger_init(
            &logger,
            papacc_test_sink_callback,
            &sink,
            PAPACC_LOG_INFO) != PAPACC_RESULT_OK) {
        return 1;
    }

    if (papacc_pal_monotonic_time_ns(&before) != PAPACC_RESULT_OK) {
        return 2;
    }

    papacc_log(&logger, PAPACC_LOG_WARNING, "test", "delivery");

    if (papacc_pal_monotonic_time_ns(&after) != PAPACC_RESULT_OK) {
        return 3;
    }
    if (sink.call_count != (PAPACC_SIZE)1) {
        return 4;
    }
    if (sink.received_context != &sink) {
        return 5;
    }
    if (sink.record.level != PAPACC_LOG_WARNING) {
        return 6;
    }
    if (strcmp(sink.record.component, "test") != 0) {
        return 7;
    }
    if (strcmp(sink.record.message, "delivery") != 0) {
        return 8;
    }
    if (sink.record.monotonic_timestamp_valid != PAPACC_TRUE ||
        sink.record.monotonic_timestamp_ns < before ||
        sink.record.monotonic_timestamp_ns > after) {
        return 9;
    }

    papacc_log(&logger, PAPACC_LOG_DEBUG, "test", "filtered");
    if (sink.call_count != (PAPACC_SIZE)1) {
        return 10;
    }

    papacc_log(&logger, PAPACC_LOG_INFO, "test", "at threshold");
    papacc_log(&logger, PAPACC_LOG_ERROR, "test", "above threshold");
    if (sink.call_count != (PAPACC_SIZE)3) {
        return 11;
    }

    papacc_log(NULL, PAPACC_LOG_INFO, "test", "invalid logger");
    papacc_log(&logger, (PAPACC_LOG_LEVEL)99, "test", "invalid level");
    papacc_log(&logger, PAPACC_LOG_INFO, NULL, "invalid component");
    papacc_log(&logger, PAPACC_LOG_INFO, "test", NULL);
    if (sink.call_count != (PAPACC_SIZE)3) {
        return 12;
    }

    if (papacc_logger_init(
            NULL,
            papacc_test_sink_callback,
            &sink,
            PAPACC_LOG_INFO) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 13;
    }
    if (papacc_logger_init(
            &logger,
            NULL,
            &sink,
            PAPACC_LOG_INFO) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 14;
    }
    if (papacc_logger_init(
            &logger,
            papacc_test_sink_callback,
            &sink,
            (PAPACC_LOG_LEVEL)99) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 15;
    }

    return 0;
}
