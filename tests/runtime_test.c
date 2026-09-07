#include <string.h>

#include "runtime.h"

typedef struct PAPACC_RUNTIME_TEST_SINK {
    PAPACC_SIZE call_count;
    PAPACC_LOG_COMPONENT_ID last_component;
    PAPACC_LOG_EVENT_ID last_event;
    const char *last_message;
} PAPACC_RUNTIME_TEST_SINK;

static void papacc_runtime_test_sink(
    void *context,
    const PAPACC_LOG_RECORD *record)
{
    PAPACC_RUNTIME_TEST_SINK *sink =
        (PAPACC_RUNTIME_TEST_SINK *)context;

    ++sink->call_count;
    sink->last_component = record->component_id;
    sink->last_event = record->event_id;
    sink->last_message = record->message;
}

int main(void)
{
    PAPACC_RUNTIME_TEST_SINK sink = {0};
    PAPACC_LOGGER logger;
    PAPACC_RUNTIME_OPTIONS options;
    PAPACC_RUNTIME runtime = PAPACC_RUNTIME_INITIALIZER;
    PAPACC_RUNTIME without_logger = PAPACC_RUNTIME_INITIALIZER;

    if (papacc_runtime_init(NULL, NULL) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 1;
    }
    if (papacc_logger_init(
            &logger,
            papacc_runtime_test_sink,
            &sink,
            PAPACC_LOG_DEBUG) != PAPACC_RESULT_OK) {
        return 2;
    }

    options.logger = &logger;
    if (papacc_runtime_init(&runtime, &options) != PAPACC_RESULT_OK) {
        return 3;
    }
    if (runtime.state != PAPACC_RUNTIME_READY || runtime.logger != &logger) {
        return 4;
    }
    if (sink.call_count != (PAPACC_SIZE)1 ||
        sink.last_component != PAPACC_LOG_COMPONENT_RUNTIME ||
        sink.last_event != PAPACC_LOG_EVENT_RUNTIME_STARTED ||
        strcmp(sink.last_message, "Runtime initialized") != 0) {
        return 5;
    }

    if (papacc_runtime_init(&runtime, &options) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 6;
    }
    if (runtime.state != PAPACC_RUNTIME_READY || runtime.logger != &logger ||
        sink.call_count != (PAPACC_SIZE)1) {
        return 7;
    }

    papacc_runtime_shutdown(&runtime);
    if (runtime.state != PAPACC_RUNTIME_SHUTDOWN || runtime.logger != NULL) {
        return 8;
    }
    if (sink.call_count != (PAPACC_SIZE)2 ||
        sink.last_event != PAPACC_LOG_EVENT_RUNTIME_STOPPED ||
        strcmp(sink.last_message, "Runtime shutdown") != 0) {
        return 9;
    }

    papacc_runtime_shutdown(&runtime);
    if (runtime.state != PAPACC_RUNTIME_SHUTDOWN ||
        sink.call_count != (PAPACC_SIZE)2) {
        return 10;
    }
    if (papacc_runtime_init(&runtime, &options) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 11;
    }
    if (runtime.state != PAPACC_RUNTIME_SHUTDOWN) {
        return 12;
    }

    if (papacc_runtime_init(&without_logger, NULL) != PAPACC_RESULT_OK) {
        return 13;
    }
    if (without_logger.state != PAPACC_RUNTIME_READY ||
        without_logger.logger != NULL) {
        return 14;
    }
    papacc_runtime_shutdown(&without_logger);
    papacc_runtime_shutdown(&without_logger);
    papacc_runtime_shutdown(NULL);

    return 0;
}
