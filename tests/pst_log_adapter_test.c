#include <string.h>

#include "pst_log_adapter.h"

#define CHECK(condition, code) do { if (!(condition)) return (code); } while (0)

typedef struct TEST_SINK {
    PAPACC_SIZE count;
    PAPACC_LOG_RECORD record;
    char message[64];
} TEST_SINK;

static void test_sink(void *context, const PAPACC_LOG_RECORD *record)
{
    TEST_SINK *sink = (TEST_SINK *)context;
    PAPACC_SIZE size = strlen(record->message);
    if (size >= sizeof(sink->message)) size = sizeof(sink->message) - 1U;
    ++sink->count;
    sink->record = *record;
    memcpy(sink->message, record->message, size);
    sink->message[size] = '\0';
}

static PST_LOG_EVENT make_event(PST_LOG_LEVEL level, pst_u32 event_id)
{
    PST_LOG_EVENT event;
    memset(&event, 0, sizeof(event));
    event.struct_size = (pst_u32)sizeof(event);
    event.api_version = PST_API_VERSION;
    event.level = level;
    event.event_id = event_id;
    event.category = PST_LOG_CATEGORY_TLS;
    event.normalized_result = PST_RESULT_OK;
    event.operation = PST_DIAGNOSTIC_OPERATION_HANDSHAKE;
    event.role = PST_CONNECTION_ROLE_CLIENT;
    event.peer_auth_fact = PST_KNOWN_TRUE;
    event.policy_fact = PST_KNOWN_FALSE;
    memcpy(event.backend_id, "openssl", 8U);
    return event;
}

int main(void)
{
    static const PST_LOG_LEVEL pst_levels[] = {
        PST_LOG_LEVEL_ERROR, PST_LOG_LEVEL_WARN, PST_LOG_LEVEL_INFO,
        PST_LOG_LEVEL_DEBUG, PST_LOG_LEVEL_TRACE
    };
    static const PAPACC_LOG_LEVEL papacc_levels[] = {
        PAPACC_LOG_ERROR, PAPACC_LOG_WARNING, PAPACC_LOG_INFO,
        PAPACC_LOG_DEBUG, PAPACC_LOG_TRACE
    };
    static const PAPACC_LOG_LEVEL configured_levels[] = {
        PAPACC_LOG_LEVEL_OFF, PAPACC_LOG_ERROR, PAPACC_LOG_WARNING,
        PAPACC_LOG_INFO, PAPACC_LOG_DEBUG, PAPACC_LOG_TRACE
    };
    static const PST_LOG_LEVEL configured_pst_levels[] = {
        PST_LOG_LEVEL_OFF, PST_LOG_LEVEL_ERROR, PST_LOG_LEVEL_WARN,
        PST_LOG_LEVEL_INFO, PST_LOG_LEVEL_DEBUG, PST_LOG_LEVEL_TRACE
    };
    static const pst_u32 pst_events[] = {
        PST_LOG_EVENT_RUNTIME_READY, PST_LOG_EVENT_RUNTIME_FAILURE,
        PST_LOG_EVENT_CONNECTION_SECURE, PST_LOG_EVENT_CONNECTION_FAILURE,
        PST_LOG_EVENT_AUTHENTICATION_FAILURE, PST_LOG_EVENT_CONNECTION_CLOSED,
        PST_LOG_EVENT_STATE_TRANSITION, PST_LOG_EVENT_OPERATION_PROGRESS
    };
    static const PAPACC_LOG_EVENT_ID papacc_events[] = {
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_RUNTIME_READY,
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_RUNTIME_FAILURE,
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_CONNECTION_SECURE,
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_CONNECTION_FAILURE,
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_AUTHENTICATION_FAILURE,
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_CONNECTION_CLOSED,
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_STATE_TRANSITION,
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_OPERATION_PROGRESS
    };
    static const PST_RESULT exact_source[] = {
        PST_RESULT_OK, PST_RESULT_INVALID_ARGUMENT, PST_RESULT_INVALID_STATE,
        PST_RESULT_UNSUPPORTED, PST_RESULT_OUT_OF_MEMORY
    };
    static const PAPACC_RESULT exact_result[] = {
        PAPACC_RESULT_OK, PAPACC_RESULT_INVALID_ARGUMENT,
        PAPACC_RESULT_INVALID_STATE, PAPACC_RESULT_NOT_SUPPORTED,
        PAPACC_RESULT_OUT_OF_MEMORY
    };
    static const pst_u32 pst_categories[] = {
        PST_LOG_CATEGORY_RUNTIME, PST_LOG_CATEGORY_BACKEND,
        PST_LOG_CATEGORY_CONNECTION, PST_LOG_CATEGORY_TLS,
        PST_LOG_CATEGORY_AUTHENTICATION, PST_LOG_CATEGORY_IO,
        PST_LOG_CATEGORY_READINESS, PST_LOG_CATEGORY_SHUTDOWN
    };
    static const PAPACC_LOG_CATEGORY papacc_categories[] = {
        PAPACC_LOG_CATEGORY_RUNTIME, PAPACC_LOG_CATEGORY_BACKEND,
        PAPACC_LOG_CATEGORY_SECURITY, PAPACC_LOG_CATEGORY_SECURITY,
        PAPACC_LOG_CATEGORY_SECURITY, PAPACC_LOG_CATEGORY_SECURITY,
        PAPACC_LOG_CATEGORY_SECURITY, PAPACC_LOG_CATEGORY_SECURITY
    };
    static const pst_u32 pst_operations[] = {
        PST_DIAGNOSTIC_OPERATION_RUNTIME,
        PST_DIAGNOSTIC_OPERATION_CONFIGURATION,
        PST_DIAGNOSTIC_OPERATION_TRANSPORT,
        PST_DIAGNOSTIC_OPERATION_CONNECTION,
        PST_DIAGNOSTIC_OPERATION_HANDSHAKE,
        PST_DIAGNOSTIC_OPERATION_AUTHENTICATION,
        PST_DIAGNOSTIC_OPERATION_READ, PST_DIAGNOSTIC_OPERATION_WRITE,
        PST_DIAGNOSTIC_OPERATION_WAIT, PST_DIAGNOSTIC_OPERATION_SHUTDOWN,
        PST_DIAGNOSTIC_OPERATION_PEER_INFO
    };
    static const PAPACC_LOG_OPERATION papacc_operations[] = {
        PAPACC_LOG_OPERATION_STARTUP, PAPACC_LOG_OPERATION_CONFIGURE,
        PAPACC_LOG_OPERATION_TRANSPORT, PAPACC_LOG_OPERATION_CONNECTION,
        PAPACC_LOG_OPERATION_HANDSHAKE, PAPACC_LOG_OPERATION_AUTHENTICATION,
        PAPACC_LOG_OPERATION_READ, PAPACC_LOG_OPERATION_WRITE,
        PAPACC_LOG_OPERATION_WAIT, PAPACC_LOG_OPERATION_SHUTDOWN,
        PAPACC_LOG_OPERATION_CONNECTION
    };
    TEST_SINK sink;
    PAPACC_LOGGER logger;
    PAPACC_PST_LOG_ADAPTER adapter = PAPACC_PST_LOG_ADAPTER_INITIALIZER;
    PST_LOG_CONFIG config;
    PST_LOG_EVENT event;
    PAPACC_SIZE index;

    memset(&sink, 0, sizeof(sink));
    for (index = 0U;
         index < sizeof(configured_levels) / sizeof(configured_levels[0]);
         ++index) {
        CHECK(papacc_logger_init(&logger, test_sink, &sink,
            configured_levels[index]) == PAPACC_RESULT_OK, 22);
        CHECK(papacc_pst_log_adapter_init(&adapter, &logger) ==
            PAPACC_RESULT_OK, 23);
        CHECK(papacc_pst_log_adapter_make_config(&adapter, &config) ==
            PAPACC_RESULT_OK && config.level == configured_pst_levels[index],
            24);
        papacc_pst_log_adapter_release(&adapter);
    }
    CHECK(papacc_logger_init(&logger, test_sink, &sink, PAPACC_LOG_TRACE) ==
        PAPACC_RESULT_OK, 1);
    CHECK(papacc_pst_log_adapter_init(&adapter, &logger) == PAPACC_RESULT_OK, 2);
    CHECK(papacc_pst_log_adapter_make_config(&adapter, &config) ==
        PAPACC_RESULT_OK && config.callback == papacc_pst_log_adapter_callback &&
        config.user_context == &adapter && config.level == PST_LOG_LEVEL_TRACE,
        3);

    for (index = 0U; index < sizeof(pst_levels) / sizeof(pst_levels[0]);
         ++index) {
        event = make_event(pst_levels[index], PST_LOG_EVENT_CONNECTION_SECURE);
        papacc_pst_log_adapter_callback(&adapter, &event);
        CHECK(sink.record.level == papacc_levels[index], 4);
    }
    for (index = 0U; index < sizeof(pst_events) / sizeof(pst_events[0]);
         ++index) {
        event = make_event(PST_LOG_LEVEL_INFO, pst_events[index]);
        papacc_pst_log_adapter_callback(&adapter, &event);
        CHECK(sink.record.event_id == papacc_events[index], 5);
    }
    for (index = 0U;
         index < sizeof(pst_categories) / sizeof(pst_categories[0]); ++index) {
        event = make_event(PST_LOG_LEVEL_INFO, PST_LOG_EVENT_CONNECTION_SECURE);
        event.category = pst_categories[index];
        papacc_pst_log_adapter_callback(&adapter, &event);
        CHECK(sink.record.category == papacc_categories[index], 25);
    }
    for (index = 0U;
         index < sizeof(pst_operations) / sizeof(pst_operations[0]); ++index) {
        event = make_event(PST_LOG_LEVEL_INFO, PST_LOG_EVENT_OPERATION_PROGRESS);
        event.operation = pst_operations[index];
        papacc_pst_log_adapter_callback(&adapter, &event);
        CHECK(sink.record.operation == papacc_operations[index], 26);
    }

    event = make_event(PST_LOG_LEVEL_INFO, 0xffffffffUL);
    event.category = 0xffffffffUL;
    event.operation = 0xffffffffUL;
    event.role = 0xffffffffUL;
    event.peer_auth_fact = 0xffffffffUL;
    event.policy_fact = 0xffffffffUL;
    memset(event.backend_id, 'X', sizeof(event.backend_id));
    papacc_pst_log_adapter_callback(&adapter, &event);
    CHECK(sink.record.event_id ==
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_UNRECOGNIZED, 16);
    CHECK(sink.record.category == PAPACC_LOG_CATEGORY_UNSPECIFIED, 17);
    CHECK(sink.record.operation == PAPACC_LOG_OPERATION_UNSPECIFIED, 18);
    CHECK(sink.record.context.secure_transport_role ==
        PAPACC_LOG_SECURE_TRANSPORT_ROLE_UNKNOWN, 19);
    CHECK(sink.record.context.peer_auth_fact == PAPACC_LOG_KNOWN_FACT_UNKNOWN &&
        sink.record.context.policy_fact == PAPACC_LOG_KNOWN_FACT_UNKNOWN, 20);
    CHECK(sink.record.context.provider_id[
        PAPACC_LOG_PROVIDER_ID_CAPACITY - 1U] == '\0', 21);
    memset(event.backend_id, 0, sizeof(event.backend_id));
    CHECK(strcmp(sink.record.context.provider_id,
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX") == 0, 7);

    event = make_event(PST_LOG_LEVEL_INFO, PST_LOG_EVENT_CONNECTION_SECURE);
    event.role = PST_CONNECTION_ROLE_SERVER;
    event.peer_auth_fact = PST_KNOWN_UNSUPPORTED;
    event.policy_fact = PST_KNOWN_NOT_APPLICABLE;
    papacc_pst_log_adapter_callback(&adapter, &event);
    CHECK(sink.record.context.secure_transport_role ==
        PAPACC_LOG_SECURE_TRANSPORT_ROLE_SERVER &&
        sink.record.context.peer_auth_fact ==
            PAPACC_LOG_KNOWN_FACT_UNSUPPORTED &&
        sink.record.context.policy_fact ==
            PAPACC_LOG_KNOWN_FACT_NOT_APPLICABLE &&
        strcmp(sink.record.context.provider_id, "openssl") == 0, 8);

    for (index = 0U; index < sizeof(exact_source) / sizeof(exact_source[0]);
         ++index) {
        event.normalized_result = exact_source[index];
        papacc_pst_log_adapter_callback(&adapter, &event);
        CHECK(sink.record.result_valid == PAPACC_TRUE &&
            sink.record.result == exact_result[index] &&
            sink.record.context.source_result == exact_source[index], 9);
    }
    event.normalized_result = PST_RESULT_AUTH_FAILURE;
    papacc_pst_log_adapter_callback(&adapter, &event);
    CHECK(sink.record.result_valid == PAPACC_FALSE &&
        sink.record.context.source_result == PST_RESULT_AUTH_FAILURE, 10);
    CHECK(strcmp(sink.message,
        "Secure Transport connection established") == 0, 11);

    papacc_pst_log_adapter_release(&adapter);
    CHECK(papacc_logger_init(&logger, test_sink, &sink, PAPACC_LOG_INFO) ==
        PAPACC_RESULT_OK, 27);
    adapter = (PAPACC_PST_LOG_ADAPTER)PAPACC_PST_LOG_ADAPTER_INITIALIZER;
    CHECK(papacc_pst_log_adapter_init(&adapter, &logger) == PAPACC_RESULT_OK, 28);
    sink.count = 0U;
    event = make_event(PST_LOG_LEVEL_DEBUG, PST_LOG_EVENT_OPERATION_PROGRESS);
    papacc_pst_log_adapter_callback(&adapter, &event);
    event.level = PST_LOG_LEVEL_TRACE;
    papacc_pst_log_adapter_callback(&adapter, &event);
    CHECK(sink.count == 0U, 29);
    event.level = PST_LOG_LEVEL_INFO;
    papacc_pst_log_adapter_callback(&adapter, &event);
    CHECK(sink.count == 1U, 30);

    papacc_pst_log_adapter_release(&adapter);
    logger.minimum_level = PAPACC_LOG_LEVEL_OFF;
    adapter = (PAPACC_PST_LOG_ADAPTER)PAPACC_PST_LOG_ADAPTER_INITIALIZER;
    CHECK(papacc_pst_log_adapter_init(&adapter, &logger) == PAPACC_RESULT_OK, 12);
    CHECK(papacc_pst_log_adapter_make_config(&adapter, &config) ==
        PAPACC_RESULT_OK && config.level == PST_LOG_LEVEL_OFF, 13);
    sink.count = 0U;
    event = make_event(PST_LOG_LEVEL_ERROR, PST_LOG_EVENT_RUNTIME_FAILURE);
    papacc_pst_log_adapter_callback(&adapter, &event);
    CHECK(sink.count == 0U, 14);
    papacc_pst_log_adapter_release(&adapter);
    papacc_pst_log_adapter_callback(&adapter, &event);
    CHECK(sink.count == 0U, 15);
    return 0;
}
