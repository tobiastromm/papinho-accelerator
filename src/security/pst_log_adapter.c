#include "pst_log_adapter.h"

#include <string.h>

static PAPACC_RESULT papacc_pst_log_level_from_papacc(
    PAPACC_LOG_LEVEL level, PST_LOG_LEVEL *out_level)
{
    if (out_level == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    switch (level) {
    case PAPACC_LOG_LEVEL_OFF: *out_level = PST_LOG_LEVEL_OFF; break;
    case PAPACC_LOG_ERROR: *out_level = PST_LOG_LEVEL_ERROR; break;
    case PAPACC_LOG_WARNING: *out_level = PST_LOG_LEVEL_WARN; break;
    case PAPACC_LOG_INFO: *out_level = PST_LOG_LEVEL_INFO; break;
    case PAPACC_LOG_DEBUG: *out_level = PST_LOG_LEVEL_DEBUG; break;
    case PAPACC_LOG_TRACE: *out_level = PST_LOG_LEVEL_TRACE; break;
    default: return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_BOOL papacc_pst_log_level_to_papacc(PST_LOG_LEVEL level,
    PAPACC_LOG_LEVEL *out_level)
{
    if (out_level == NULL) return PAPACC_FALSE;
    switch (level) {
    case PST_LOG_LEVEL_ERROR: *out_level = PAPACC_LOG_ERROR; break;
    case PST_LOG_LEVEL_WARN: *out_level = PAPACC_LOG_WARNING; break;
    case PST_LOG_LEVEL_INFO: *out_level = PAPACC_LOG_INFO; break;
    case PST_LOG_LEVEL_DEBUG: *out_level = PAPACC_LOG_DEBUG; break;
    case PST_LOG_LEVEL_TRACE: *out_level = PAPACC_LOG_TRACE; break;
    default: return PAPACC_FALSE;
    }
    return PAPACC_TRUE;
}

static PAPACC_LOG_EVENT_ID papacc_pst_log_event_id(pst_u32 event_id,
    const char **message)
{
    switch (event_id) {
    case PST_LOG_EVENT_RUNTIME_READY:
        *message = "Secure Transport runtime ready";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_RUNTIME_READY;
    case PST_LOG_EVENT_RUNTIME_FAILURE:
        *message = "Secure Transport runtime failure";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_RUNTIME_FAILURE;
    case PST_LOG_EVENT_CONNECTION_SECURE:
        *message = "Secure Transport connection established";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_CONNECTION_SECURE;
    case PST_LOG_EVENT_CONNECTION_FAILURE:
        *message = "Secure Transport connection failure";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_CONNECTION_FAILURE;
    case PST_LOG_EVENT_AUTHENTICATION_FAILURE:
        *message = "Secure Transport authentication failure";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_AUTHENTICATION_FAILURE;
    case PST_LOG_EVENT_CONNECTION_CLOSED:
        *message = "Secure Transport connection closed";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_CONNECTION_CLOSED;
    case PST_LOG_EVENT_STATE_TRANSITION:
        *message = "Secure Transport state transition";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_STATE_TRANSITION;
    case PST_LOG_EVENT_OPERATION_PROGRESS:
        *message = "Secure Transport operation progress";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_OPERATION_PROGRESS;
    default:
        *message = "Unrecognized Secure Transport event";
        return PAPACC_LOG_EVENT_SECURE_TRANSPORT_UNRECOGNIZED;
    }
}

static PAPACC_LOG_CATEGORY papacc_pst_log_category(pst_u32 category)
{
    switch (category) {
    case PST_LOG_CATEGORY_RUNTIME: return PAPACC_LOG_CATEGORY_RUNTIME;
    case PST_LOG_CATEGORY_BACKEND: return PAPACC_LOG_CATEGORY_BACKEND;
    case PST_LOG_CATEGORY_CONNECTION:
    case PST_LOG_CATEGORY_TLS:
    case PST_LOG_CATEGORY_AUTHENTICATION:
    case PST_LOG_CATEGORY_IO:
    case PST_LOG_CATEGORY_READINESS:
    case PST_LOG_CATEGORY_SHUTDOWN:
        return PAPACC_LOG_CATEGORY_SECURITY;
    default: return PAPACC_LOG_CATEGORY_UNSPECIFIED;
    }
}

static PAPACC_LOG_OPERATION papacc_pst_log_operation(pst_u32 operation)
{
    switch (operation) {
    case PST_DIAGNOSTIC_OPERATION_RUNTIME: return PAPACC_LOG_OPERATION_STARTUP;
    case PST_DIAGNOSTIC_OPERATION_CONFIGURATION:
        return PAPACC_LOG_OPERATION_CONFIGURE;
    case PST_DIAGNOSTIC_OPERATION_TRANSPORT:
        return PAPACC_LOG_OPERATION_TRANSPORT;
    case PST_DIAGNOSTIC_OPERATION_CONNECTION:
        return PAPACC_LOG_OPERATION_CONNECTION;
    case PST_DIAGNOSTIC_OPERATION_HANDSHAKE:
        return PAPACC_LOG_OPERATION_HANDSHAKE;
    case PST_DIAGNOSTIC_OPERATION_AUTHENTICATION:
        return PAPACC_LOG_OPERATION_AUTHENTICATION;
    case PST_DIAGNOSTIC_OPERATION_READ: return PAPACC_LOG_OPERATION_READ;
    case PST_DIAGNOSTIC_OPERATION_WRITE: return PAPACC_LOG_OPERATION_WRITE;
    case PST_DIAGNOSTIC_OPERATION_WAIT: return PAPACC_LOG_OPERATION_WAIT;
    case PST_DIAGNOSTIC_OPERATION_SHUTDOWN:
        return PAPACC_LOG_OPERATION_SHUTDOWN;
    case PST_DIAGNOSTIC_OPERATION_PEER_INFO:
        return PAPACC_LOG_OPERATION_CONNECTION;
    default: return PAPACC_LOG_OPERATION_UNSPECIFIED;
    }
}

static PAPACC_LOG_SECURE_TRANSPORT_ROLE papacc_pst_log_role(pst_u32 role)
{
    if (role == PST_CONNECTION_ROLE_CLIENT)
        return PAPACC_LOG_SECURE_TRANSPORT_ROLE_CLIENT;
    if (role == PST_CONNECTION_ROLE_SERVER)
        return PAPACC_LOG_SECURE_TRANSPORT_ROLE_SERVER;
    return PAPACC_LOG_SECURE_TRANSPORT_ROLE_UNKNOWN;
}

static PAPACC_LOG_KNOWN_FACT papacc_pst_log_fact(pst_u32 fact)
{
    switch (fact) {
    case PST_KNOWN_FALSE: return PAPACC_LOG_KNOWN_FACT_FALSE;
    case PST_KNOWN_TRUE: return PAPACC_LOG_KNOWN_FACT_TRUE;
    case PST_KNOWN_UNSUPPORTED: return PAPACC_LOG_KNOWN_FACT_UNSUPPORTED;
    case PST_KNOWN_NOT_APPLICABLE:
        return PAPACC_LOG_KNOWN_FACT_NOT_APPLICABLE;
    default: return PAPACC_LOG_KNOWN_FACT_UNKNOWN;
    }
}

static PAPACC_BOOL papacc_pst_log_result(PST_RESULT source,
    PAPACC_RESULT *result)
{
    switch (source) {
    case PST_RESULT_OK: *result = PAPACC_RESULT_OK; break;
    case PST_RESULT_INVALID_ARGUMENT:
        *result = PAPACC_RESULT_INVALID_ARGUMENT; break;
    case PST_RESULT_INVALID_STATE:
        *result = PAPACC_RESULT_INVALID_STATE; break;
    case PST_RESULT_UNSUPPORTED:
        *result = PAPACC_RESULT_NOT_SUPPORTED; break;
    case PST_RESULT_OUT_OF_MEMORY:
        *result = PAPACC_RESULT_OUT_OF_MEMORY; break;
    default:
        *result = PAPACC_RESULT_INTERNAL_ERROR;
        return PAPACC_FALSE;
    }
    return PAPACC_TRUE;
}

static void papacc_pst_log_copy_provider(char *destination,
    PAPACC_SIZE capacity, const char *source)
{
    PAPACC_SIZE index = 0U;
    if (capacity == 0U) return;
    while (index + 1U < capacity &&
        index < PST_DIAGNOSTIC_BACKEND_ID_CAPACITY && source[index] != '\0') {
        destination[index] = source[index];
        ++index;
    }
    destination[index] = '\0';
}

PAPACC_RESULT papacc_pst_log_adapter_init(PAPACC_PST_LOG_ADAPTER *adapter,
    const PAPACC_LOGGER *logger)
{
    PST_LOG_LEVEL ignored;
    if (adapter == NULL || logger == NULL || logger->sink == NULL ||
        papacc_pst_log_level_from_papacc(logger->minimum_level, &ignored) !=
            PAPACC_RESULT_OK)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (adapter->ready == PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    adapter->logger = *logger;
    adapter->ready = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_pst_log_adapter_release(PAPACC_PST_LOG_ADAPTER *adapter)
{
    if (adapter == NULL) return;
    adapter->logger.sink = NULL;
    adapter->logger.sink_context = NULL;
    adapter->logger.minimum_level = PAPACC_LOG_LEVEL_OFF;
    adapter->ready = PAPACC_FALSE;
}

PAPACC_RESULT papacc_pst_log_adapter_make_config(
    PAPACC_PST_LOG_ADAPTER *adapter, PST_LOG_CONFIG *config)
{
    PST_RESULT pst_result;
    PAPACC_RESULT result;
    if (adapter == NULL || config == NULL || adapter->ready != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    pst_result = pst_log_config_init(config);
    if (pst_result != PST_RESULT_OK) return PAPACC_RESULT_INTERNAL_ERROR;
    result = papacc_pst_log_level_from_papacc(
        adapter->logger.minimum_level, &config->level);
    if (result != PAPACC_RESULT_OK) return result;
    config->callback = papacc_pst_log_adapter_callback;
    config->user_context = adapter;
    return PAPACC_RESULT_OK;
}

void PST_CALL papacc_pst_log_adapter_callback(void *user_context,
    const PST_LOG_EVENT *event)
{
    PAPACC_PST_LOG_ADAPTER *adapter =
        (PAPACC_PST_LOG_ADAPTER *)user_context;
    PAPACC_LOG_CONTEXT context = PAPACC_LOG_CONTEXT_INITIALIZER;
    PAPACC_LOG_LEVEL level;
    PAPACC_LOG_EVENT_ID event_id;
    PAPACC_LOG_CATEGORY category;
    PAPACC_LOG_OPERATION operation;
    PAPACC_RESULT result;
    PAPACC_BOOL result_valid;
    const char *message;
    if (adapter == NULL || adapter->ready != PAPACC_TRUE || event == NULL ||
        event->struct_size < PST_LOG_EVENT_MIN_SIZE ||
        event->api_version != PST_API_VERSION ||
        papacc_pst_log_level_to_papacc(event->level, &level) == PAPACC_FALSE)
        return;
    event_id = papacc_pst_log_event_id(event->event_id, &message);
    category = papacc_pst_log_category(event->category);
    operation = papacc_pst_log_operation(event->operation);
    context.fields = PAPACC_LOG_CONTEXT_SECURE_TRANSPORT_ROLE |
        PAPACC_LOG_CONTEXT_PROVIDER_ID | PAPACC_LOG_CONTEXT_PEER_AUTH_FACT |
        PAPACC_LOG_CONTEXT_POLICY_FACT | PAPACC_LOG_CONTEXT_SOURCE_RESULT;
    context.secure_transport_role = papacc_pst_log_role(event->role);
    papacc_pst_log_copy_provider(context.provider_id,
        sizeof(context.provider_id), event->backend_id);
    context.peer_auth_fact = papacc_pst_log_fact(event->peer_auth_fact);
    context.policy_fact = papacc_pst_log_fact(event->policy_fact);
    context.source_result = event->normalized_result;
    result_valid = papacc_pst_log_result(event->normalized_result, &result);
    papacc_log_event(&adapter->logger, level, event_id, category,
        PAPACC_LOG_COMPONENT_SECURE_TRANSPORT, operation, result_valid, result,
        &context, message);
}
