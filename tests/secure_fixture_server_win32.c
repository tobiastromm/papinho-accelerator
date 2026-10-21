#include "server_secure_io_win32.h"
#include "security_test_support.h"
#include "pst_provider_bootstrap_win32.h"
#include "egress_runtime_config.h"
#include "pst_outbound_tls.h"
#include "papinho_secure_transport_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIXTURE_CAPACITY 8U

typedef enum FIXTURE_SCENARIO {
    FIXTURE_SCENARIO_POSITIVE = 0,
    FIXTURE_SCENARIO_STALL,
    FIXTURE_SCENARIO_DATA_FAILURE,
    FIXTURE_SCENARIO_MODE_B_EGRESS,
    FIXTURE_SCENARIO_MODE_B_TLS_NAME_FAILURE,
    FIXTURE_SCENARIO_TCP_PLAIN_EGRESS,
    FIXTURE_SCENARIO_MIXED_EGRESS,
    FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE,
    FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE,
    FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_COMPLETE,
    FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_INCOMPLETE,
    FIXTURE_SCENARIO_TLS_TRUNCATED_CLOSE_DELIMITED,
    FIXTURE_SCENARIO_TLS_CLEAN_CLOSE,
    FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE_MULTI,
    FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE_MULTI,
    FIXTURE_SCENARIO_SECURE_V2_UNAVAILABLE_V1_FALLBACK,
    FIXTURE_SCENARIO_SECURE_V2_PROTOCOL_ERROR_NO_FALLBACK,
    FIXTURE_SCENARIO_SECURE_V2_AUTH_FAILURE_NO_FALLBACK,
    FIXTURE_SCENARIO_SECURE_V2_TRANSPORT_FAILURE_NO_FALLBACK,
    FIXTURE_SCENARIO_SECURE_V2_EGRESS_STARTED_NO_FALLBACK
} FIXTURE_SCENARIO;

typedef struct FIXTURE_POLICY_CONTEXT {
    FIXTURE_SCENARIO scenario;
    PAPACC_SIZE associate_data_authorization_count;
} FIXTURE_POLICY_CONTEXT;

typedef struct FIXTURE_LOG_CONTEXT {
    FIXTURE_SCENARIO scenario;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slots;
    PAPACC_SIZE slot_capacity;
    PAPACC_SIZE protocol_fault_armed_count;
    PAPACC_SIZE valid_v2_offer_decoded_count;
    PAPACC_SIZE protocol_fault_realized_count;
    PAPACC_SIZE auth_control_published_count;
    PAPACC_SIZE transport_control_published_count;
} FIXTURE_LOG_CONTEXT;

static PAPACC_RESULT fixture_protocol_error_after_decode(void *context,
    const PAPACC_CAPABILITY_SET *client_supported,
    const PAPACC_CAPABILITY_SET *client_preference)
{
    FIXTURE_LOG_CONTEXT *fixture = (FIXTURE_LOG_CONTEXT *)context;
    if (fixture == NULL || client_supported == NULL ||
        client_preference == NULL || client_supported->count != 2U ||
        client_preference->count != 2U ||
        papacc_capability_set_contains(client_supported,
            &PAPACC_CAPABILITY_NETWORK_EGRESS_V2) != PAPACC_TRUE ||
        papacc_capability_set_contains(client_supported,
            &PAPACC_CAPABILITY_TLS_OFFLOAD_V1) != PAPACC_TRUE ||
        papacc_capability_set_contains(client_preference,
            &PAPACC_CAPABILITY_NETWORK_EGRESS_V2) != PAPACC_TRUE ||
        papacc_capability_set_contains(client_preference,
            &PAPACC_CAPABILITY_TLS_OFFLOAD_V1) != PAPACC_TRUE)
        return PAPACC_RESULT_OK;
    ++fixture->valid_v2_offer_decoded_count;
    puts("VALID_V2_OFFER_DECODED=YES");
    fflush(stdout);
    return PAPACC_RESULT_PROTOCOL_ERROR;
}

typedef struct FIXTURE_EGRESS {
    PAPACC_NETWORK_ENDPOINT endpoint;
    PAPACC_NETWORK_ENDPOINT plain_endpoint;
    PAPACC_SIZE resolve_calls;
} FIXTURE_EGRESS;

typedef struct FIXTURE_HTTPS_TARGET {
    SOCKET listener;
    pst_runtime *runtime;
    pst_credentials *credentials;
    HANDLE stop_event;
    PAPACC_BOOL expect_tls_failure;
    PAPACC_BOOL use_tls;
    PAPACC_BOOL require_alpn;
    const PAPACC_U8 *response;
    PAPACC_SIZE response_size;
    PAPACC_BOOL omit_close_notify;
    PAPACC_BOOL fragment_response;
    PAPACC_BOOL completed;
    PAPACC_SIZE accepted_count;
    volatile LONG response_count;
    /* Optional synchronization/observations for the lifetime regression only. */
    HANDLE read_wait_event;
    HANDLE shutdown_wait_event;
    HANDLE worker_published_event;
    HANDLE *observed_worker_handle;
    pst_u32 observed_shutdown_operation;
    PST_RESULT observed_shutdown_source;
    volatile LONG graceful_count;
    volatile LONG cancelled_shutdown_count;
} FIXTURE_HTTPS_TARGET;

typedef struct FIXTURE_HTTPS_CONNECTION {
    FIXTURE_HTTPS_TARGET *fixture;
    SOCKET socket;
    /* Worker owns create/use/destroy. Parent may only signal the documented
       cross-thread wake, under this publication/lifetime lock. */
    CRITICAL_SECTION wake_lock;
    pst_wait_set *wait_set;
} FIXTURE_HTTPS_CONNECTION;

static PST_RESULT fixture_signal_worker(FIXTURE_HTTPS_CONNECTION *worker)
{
    PST_RESULT result = PST_RESULT_OK;
    SetEvent(worker->fixture->stop_event);
    EnterCriticalSection(&worker->wake_lock);
    if (worker->wait_set != NULL) result = pst_wait_set_wake(worker->wait_set);
    LeaveCriticalSection(&worker->wake_lock);
    return result;
}

static DWORD fixture_join_thread(HANDLE *thread, DWORD timeout,
    HANDLE stop_event, pst_wait_set *wait_set)
{
    DWORD code = 1U, waited;
    int timed_out = 0;
    if (*thread == NULL) return 0U;
    waited = WaitForSingleObject(*thread, timeout);
    if (waited != WAIT_OBJECT_0) {
        timed_out = 1;
        fprintf(stderr, "FIXTURE_JOIN_FAILURE wait=%lu HANDLE_RETAINED=YES\n",
            (unsigned long)waited);
        if (stop_event != NULL) SetEvent(stop_event);
        if (wait_set != NULL) (void)pst_wait_set_wake(wait_set);
        /* Recovery is cooperative, not TerminateThread. Never unwind storage
           under a live worker, even if a native operation is broken. */
        if (WaitForSingleObject(*thread, 5000U) != WAIT_OBJECT_0) {
            fputs("FIXTURE_UNRECOVERABLE_JOIN=FAIL PROCESS_FAIL_CLOSED=74\n", stderr);
            fflush(stderr);
            ExitProcess(74U);
        }
    }
    if (!GetExitCodeThread(*thread, &code)) code = 1U;
    CloseHandle(*thread);
    *thread = NULL;
    if (code != 0U) {
        fprintf(stderr, "FIXTURE_THREAD_EXIT=%lu\n", (unsigned long)code);
        return code;
    }
    return timed_out ? 4U : 0U;
}

/* 1 = readiness, 0 = failure/deadline, -1 = explicit cancellation.
   Stop is latched and the parent wakes PST, so there is no polling clock. */
static int fixture_tls_wait(FIXTURE_HTTPS_CONNECTION *worker,
    ULONGLONG deadline, const char *phase)
{
    PST_WAIT_EVENT event;
    PST_WAIT_SET_RESULT observed;
    PST_RESULT result;
    ULONGLONG now;
    if (WaitForSingleObject(worker->fixture->stop_event, 0U) == WAIT_OBJECT_0) {
        fprintf(stderr, "FIXTURE_WAIT_CANCEL phase=%s before_wait=YES\n", phase);
        return -1;
    }
    now = GetTickCount64();
    if (now >= deadline) {
        fprintf(stderr, "FIXTURE_WAIT_DEADLINE phase=%s now=%llu deadline=%llu\n",
            phase, (unsigned long long)now, (unsigned long long)deadline);
        return 0;
    }
    printf("FIXTURE_WAIT_PHASE=%s\n", phase); fflush(stdout);
    if (strcmp(phase, "READ") == 0 && worker->fixture->read_wait_event != NULL)
        SetEvent(worker->fixture->read_wait_event);
    if (strcmp(phase, "TLS_SHUTDOWN") == 0 && worker->fixture->shutdown_wait_event != NULL)
        SetEvent(worker->fixture->shutdown_wait_event);
    result = pst_wait_set_wait(worker->wait_set, (pst_u32)(deadline - now),
        &event, 1U, &observed);
    if (result != PST_RESULT_OK && result != PST_RESULT_WAIT_WOKEN &&
        result != PST_RESULT_WAIT_TIMEOUT) {
        fprintf(stderr, "FIXTURE_WAIT_FAILURE phase=%s result=%lu wsa=%d\n",
            phase, (unsigned long)result, WSAGetLastError());
        return 0;
    }
    if (WaitForSingleObject(worker->fixture->stop_event, 0U) == WAIT_OBJECT_0)
        return -1;
    return observed.timed_out ? 0 : 1;
}

static const char fixture_http_response[] =
    "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nConnection: close\r\n\r\n"
    "papacc-mode-b-ok\n";
static const char fixture_plain_http_response[] =
    "HTTP/1.1 200 OK\r\nContent-Length: 20\r\nConnection: close\r\n\r\n"
    "papacc-tcp-plain-ok\n";
static const char fixture_cl_complete[] =
    "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nConnection: close\r\n\r\n"
    "papacc-complete!\n";
static const char fixture_cl_incomplete[] =
    "HTTP/1.1 200 OK\r\nContent-Length: 23\r\nConnection: close\r\n\r\n"
    "papacc-incomplete\n";
static const char fixture_chunked_complete[] =
    "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n"
    "7\r\npapacc-\r\n9\r\nchunked!\n\r\n0\r\n\r\n";
static const char fixture_chunked_incomplete[] =
    "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nConnection: close\r\n\r\n"
    "7\r\npapacc-\r\n9\r\nchunked!\n\r\n";
static const char fixture_close_delimited[] =
    "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\npapacc-close-delimited\n";

typedef struct FIXTURE_HTTP_SCENARIO_METADATA {
    FIXTURE_SCENARIO scenario;
    const char *response;
    const char *expected_body;
    const char *framing;
    unsigned int declared_body_length;
    unsigned int delivered_body_length;
    PAPACC_BOOL http_complete;
    PAPACC_BOOL clean_close_notify;
    PAPACC_BOOL multi_frame;
} FIXTURE_HTTP_SCENARIO_METADATA;

static const FIXTURE_HTTP_SCENARIO_METADATA fixture_http_scenarios[] = {
    { FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE,
      fixture_cl_complete, "papacc-complete!", "CONTENT_LENGTH",
      17U, 17U, PAPACC_TRUE, PAPACC_FALSE, PAPACC_FALSE },
    { FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE,
      fixture_cl_incomplete, "papacc-incomplete", "CONTENT_LENGTH",
      23U, 18U, PAPACC_FALSE, PAPACC_FALSE, PAPACC_FALSE },
    { FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_COMPLETE,
      fixture_chunked_complete, "papacc-chunked!", "CHUNKED",
      0U, 16U, PAPACC_TRUE, PAPACC_FALSE, PAPACC_FALSE },
    { FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_INCOMPLETE,
      fixture_chunked_incomplete, "papacc-chunked!", "CHUNKED",
      0U, 16U, PAPACC_FALSE, PAPACC_FALSE, PAPACC_FALSE },
    { FIXTURE_SCENARIO_TLS_TRUNCATED_CLOSE_DELIMITED,
      fixture_close_delimited, "papacc-close-delimited", "CLOSE_DELIMITED",
      0U, 23U, PAPACC_FALSE, PAPACC_FALSE, PAPACC_FALSE },
    { FIXTURE_SCENARIO_TLS_CLEAN_CLOSE,
      fixture_cl_complete, "papacc-complete!", "CONTENT_LENGTH",
      17U, 17U, PAPACC_TRUE, PAPACC_TRUE, PAPACC_FALSE },
    { FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE_MULTI,
      fixture_cl_complete, "papacc-complete!", "CONTENT_LENGTH",
      17U, 17U, PAPACC_TRUE, PAPACC_FALSE, PAPACC_TRUE },
    { FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE_MULTI,
      fixture_cl_incomplete, "papacc-incomplete", "CONTENT_LENGTH",
      23U, 18U, PAPACC_FALSE, PAPACC_FALSE, PAPACC_TRUE }
};

static const FIXTURE_HTTP_SCENARIO_METADATA *fixture_http_metadata(
    FIXTURE_SCENARIO scenario)
{
    PAPACC_SIZE index;
    for (index = 0U; index < sizeof(fixture_http_scenarios) /
            sizeof(fixture_http_scenarios[0]); ++index)
        if (fixture_http_scenarios[index].scenario == scenario)
            return &fixture_http_scenarios[index];
    return NULL;
}

static int fixture_http_metadata_consistent(
    const FIXTURE_HTTP_SCENARIO_METADATA *metadata)
{
    const char *body;
    char content_length[64];
    PAPACC_SIZE body_size;
    if (metadata == NULL || metadata->response == NULL ||
        metadata->expected_body == NULL || metadata->framing == NULL ||
        metadata->delivered_body_length != strlen(metadata->expected_body) + 1U)
        return 0;
    body = strstr(metadata->response, "\r\n\r\n");
    if (body == NULL) return 0;
    body += 4;
    if (strcmp(metadata->framing, "CONTENT_LENGTH") == 0) {
        if (sprintf_s(content_length, sizeof(content_length),
                "Content-Length: %u\r\n", metadata->declared_body_length) < 0 ||
            strstr(metadata->response, content_length) == NULL)
            return 0;
        body_size = strlen(body);
        if (body_size != metadata->delivered_body_length ||
            memcmp(body, metadata->expected_body,
                strlen(metadata->expected_body)) != 0 ||
            body[body_size - 1U] != '\n' ||
            (metadata->http_complete == PAPACC_TRUE ?
                metadata->declared_body_length != body_size :
                metadata->declared_body_length <= body_size))
            return 0;
    } else if (strcmp(metadata->framing, "CHUNKED") == 0) {
        if (metadata->declared_body_length != 0U ||
            strstr(body, "7\r\npapacc-\r\n9\r\nchunked!\n\r\n") != body ||
            (metadata->http_complete == PAPACC_TRUE) !=
                (strstr(body, "0\r\n\r\n") != NULL))
            return 0;
    } else if (strcmp(metadata->framing, "CLOSE_DELIMITED") == 0) {
        body_size = strlen(body);
        if (metadata->declared_body_length != 0U ||
            metadata->http_complete != PAPACC_FALSE ||
            body_size != metadata->delivered_body_length ||
            memcmp(body, metadata->expected_body,
                strlen(metadata->expected_body)) != 0 ||
            body[body_size - 1U] != '\n')
            return 0;
    } else return 0;
    return metadata->clean_close_notify == PAPACC_TRUE ?
        metadata->http_complete == PAPACC_TRUE : 1;
}

static int fixture_operation_needs_wait(pst_u32 operation)
{
    return operation == PST_OPERATION_NEED_READ ||
        operation == PST_OPERATION_NEED_WRITE ||
        operation == PST_OPERATION_NEED_READ_WRITE;
}

static PAPACC_EGRESS_DECISION allow_fixture_host(void *context,
    const char *host, PAPACC_SIZE size, PAPACC_U16 port)
{
    FIXTURE_EGRESS *fixture = (FIXTURE_EGRESS *)context;
    if (((size == 18U && memcmp(host, "papacc-test-server", 18U) == 0) ||
         (size == 13U && memcmp(host, "wrong.example", 13U) == 0)) &&
        port == fixture->endpoint.port) return PAPACC_EGRESS_ALLOW;
    if (size == 24U &&
        memcmp(host, "papacc-plain-test-server", 24U) == 0 &&
        port == fixture->plain_endpoint.port) return PAPACC_EGRESS_ALLOW;
    return PAPACC_EGRESS_DENY;
}

static PAPACC_EGRESS_DECISION allow_fixture_endpoint(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint, PAPACC_U32 sensitive_classes)
{
    FIXTURE_EGRESS *fixture = (FIXTURE_EGRESS *)context;
    (void)sensitive_classes;
    return papacc_network_endpoint_equal(&fixture->endpoint, endpoint) ==
            PAPACC_TRUE ||
        papacc_network_endpoint_equal(&fixture->plain_endpoint, endpoint) ==
            PAPACC_TRUE ? PAPACC_EGRESS_ALLOW : PAPACC_EGRESS_DENY;
}

static PAPACC_RESULT select_fixture_profile(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint, PAPACC_NETWORK_TRANSPORT_MODE mode,
    PAPACC_U32 *profile_id)
{
    FIXTURE_EGRESS *fixture = (FIXTURE_EGRESS *)context;
    if (endpoint == NULL || profile_id == NULL ||
        (mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED &&
         mode != PAPACC_NETWORK_TRANSPORT_TCP_PLAIN) ||
        papacc_network_endpoint_equal(&fixture->endpoint, endpoint) !=
            PAPACC_TRUE &&
        papacc_network_endpoint_equal(&fixture->plain_endpoint, endpoint) !=
            PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *profile_id = 1U;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT resolve_fixture(void *context, const char *host,
    PAPACC_SIZE size, PAPACC_U16 port, PAPACC_NETWORK_ENDPOINT *endpoints,
    PAPACC_SIZE capacity, PAPACC_SIZE *count)
{
    FIXTURE_EGRESS *fixture = (FIXTURE_EGRESS *)context;
    if (count != NULL) *count = 0U;
    if (host == NULL || endpoints == NULL || count == NULL || capacity == 0U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (size == 24U &&
        memcmp(host, "papacc-plain-test-server", 24U) == 0 &&
        port == fixture->plain_endpoint.port)
        endpoints[0] = fixture->plain_endpoint;
    else if (((size == 18U &&
              memcmp(host, "papacc-test-server", 18U) == 0) ||
             (size == 13U && memcmp(host, "wrong.example", 13U) == 0)) &&
             port == fixture->endpoint.port)
        endpoints[0] = fixture->endpoint;
    else return PAPACC_RESULT_INVALID_ARGUMENT;
    ++fixture->resolve_calls;
    *count = 1U;
    return PAPACC_RESULT_OK;
}

static const PAPACC_U8 fixture_principal[16] = {
    0x50,0x41,0x50,0x41,0x43,0x43,0x2d,0x49,
    0x41,0x36,0x2d,0x50,0,0,0,1 };
static const PAPACC_U8 browser_certificate_sha256[32] = {
    0x35,0xc5,0xe9,0x8e,0x70,0x1d,0x8b,0x18,
    0xe5,0xfb,0x85,0xea,0xf1,0xeb,0x5d,0x21,
    0x57,0x20,0x4c,0xe7,0x35,0xab,0x14,0xbf,
    0x7d,0x9e,0x2d,0x25,0xb8,0x30,0x2a,0xc2 };

static void fixture_log(void *context, const PAPACC_LOG_RECORD *record)
{
    FIXTURE_LOG_CONTEXT *fixture = (FIXTURE_LOG_CONTEXT *)context;
    PAPACC_SIZE index;
    if (fixture == NULL || record == NULL) return;
    if (record->event_id == PAPACC_LOG_EVENT_CONTROL_ESTABLISHED &&
        fixture->scenario ==
            FIXTURE_SCENARIO_SECURE_V2_AUTH_FAILURE_NO_FALLBACK)
        ++fixture->auth_control_published_count;
    if (record->event_id == PAPACC_LOG_EVENT_CONTROL_ESTABLISHED &&
        fixture->scenario ==
            FIXTURE_SCENARIO_SECURE_V2_TRANSPORT_FAILURE_NO_FALLBACK)
        ++fixture->transport_control_published_count;
    if (fixture->scenario !=
            FIXTURE_SCENARIO_SECURE_V2_PROTOCOL_ERROR_NO_FALLBACK)
        return;
    if (record->event_id == PAPACC_LOG_EVENT_CONTROL_ESTABLISHED) {
        for (index = 0U; index < fixture->slot_capacity; ++index) {
            PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot = &fixture->slots[index];
            if (slot->in_use == PAPACC_TRUE && slot->kind ==
                    PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL &&
                papacc_post_control_processor_set_capability_test_gate(
                    &slot->post_control_processor,
                    fixture_protocol_error_after_decode, fixture) ==
                        PAPACC_RESULT_OK) {
                ++fixture->protocol_fault_armed_count;
                puts("PROTOCOL_ERROR_ARMED_BEFORE_REQUEST_PROCESSING=YES");
                fflush(stdout);
                return;
            }
        }
    }
    if (record->event_id == PAPACC_LOG_EVENT_PROTOCOL_REJECTED &&
        record->result_valid == PAPACC_TRUE &&
        record->result == PAPACC_RESULT_PROTOCOL_ERROR &&
        fixture->protocol_fault_realized_count <
            fixture->protocol_fault_armed_count) {
        ++fixture->protocol_fault_realized_count;
        puts("PROTOCOL_ERROR_REALIZED=YES");
        puts("NORMAL_CAPABILITY_RESULT_EMITTED=NO");
        fflush(stdout);
    }
}

static PAPACC_RESULT resolve_browser_principal(void *context,
    const PAPACC_PEER_EVIDENCE *evidence,
    PAPACC_PRINCIPAL_RESOLUTION *resolution, PAPACC_PRINCIPAL *principal)
{
    (void)context;
    if (evidence == NULL || resolution == NULL || principal == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *resolution = PAPACC_PRINCIPAL_NOT_ENROLLED;
    if (evidence->certificate_present != PAPACC_TRUE ||
        evidence->chain_validated != PAPACC_TRUE ||
        evidence->peer_authenticated != PAPACC_TRUE ||
        evidence->certificate_sha256_valid != PAPACC_TRUE ||
        memcmp(evidence->certificate_sha256, browser_certificate_sha256,
            sizeof(browser_certificate_sha256)) != 0)
        return PAPACC_RESULT_OK;
    principal->valid = PAPACC_TRUE;
    memcpy(principal->value, fixture_principal, sizeof(fixture_principal));
    *resolution = PAPACC_PRINCIPAL_RESOLVED;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT authorize_browser_principal(void *context,
    const PAPACC_PRINCIPAL *principal, PAPACC_AUTHORIZATION_ACTION action,
    PAPACC_AUTHORIZATION_DECISION *decision)
{
    FIXTURE_POLICY_CONTEXT *policy = (FIXTURE_POLICY_CONTEXT *)context;
    if (principal == NULL || decision == NULL ||
        principal->valid != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *decision = memcmp(principal->value, fixture_principal,
        sizeof(fixture_principal)) == 0 ? PAPACC_AUTHORIZATION_ALLOW :
        PAPACC_AUTHORIZATION_DENY;
    if (*decision == PAPACC_AUTHORIZATION_ALLOW && policy != NULL &&
        policy->scenario == FIXTURE_SCENARIO_DATA_FAILURE &&
        action == PAPACC_AUTHORIZATION_ASSOCIATE_DATA) {
        ++policy->associate_data_authorization_count;
        if (policy->associate_data_authorization_count > 1U) {
            *decision = PAPACC_AUTHORIZATION_DENY;
            puts("FIXTURE_DATA_FAILURE_POINT=DATA_ASSOCIATION_AUTHORIZATION_DENIED_BEFORE_DATA_ACCEPT");
            fflush(stdout);
        }
    }
    return PAPACC_RESULT_OK;
}

static int parse_scenario(const char *text, FIXTURE_SCENARIO *scenario)
{
    if (text == NULL || scenario == NULL) return 0;
    if (strcmp(text, "positive") == 0)
        *scenario = FIXTURE_SCENARIO_POSITIVE;
    else if (strcmp(text, "stall") == 0)
        *scenario = FIXTURE_SCENARIO_STALL;
    else if (strcmp(text, "data-failure") == 0)
        *scenario = FIXTURE_SCENARIO_DATA_FAILURE;
    else if (strcmp(text, "mode-b-egress") == 0)
        *scenario = FIXTURE_SCENARIO_MODE_B_EGRESS;
    else if (strcmp(text, "mode-b-tls-name-failure") == 0)
        *scenario = FIXTURE_SCENARIO_MODE_B_TLS_NAME_FAILURE;
    else if (strcmp(text, "tcp-plain-egress") == 0)
        *scenario = FIXTURE_SCENARIO_TCP_PLAIN_EGRESS;
    else if (strcmp(text, "mixed-egress") == 0)
        *scenario = FIXTURE_SCENARIO_MIXED_EGRESS;
    else if (strcmp(text, "tls-truncated-content-length-complete") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE;
    else if (strcmp(text, "tls-truncated-content-length-incomplete") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE;
    else if (strcmp(text, "tls-truncated-chunked-complete") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_COMPLETE;
    else if (strcmp(text, "tls-truncated-chunked-incomplete") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_INCOMPLETE;
    else if (strcmp(text, "tls-truncated-close-delimited") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_TRUNCATED_CLOSE_DELIMITED;
    else if (strcmp(text, "tls-clean-close") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_CLEAN_CLOSE;
    else if (strcmp(text,
            "tls-truncated-content-length-complete-multi-frame") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE_MULTI;
    else if (strcmp(text,
            "tls-truncated-content-length-incomplete-multi-frame") == 0)
        *scenario = FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE_MULTI;
    else if (strcmp(text, "secure-v2-unavailable-v1-fallback") == 0)
        *scenario = FIXTURE_SCENARIO_SECURE_V2_UNAVAILABLE_V1_FALLBACK;
    else if (strcmp(text, "secure-v2-protocol-error-no-fallback") == 0)
        *scenario = FIXTURE_SCENARIO_SECURE_V2_PROTOCOL_ERROR_NO_FALLBACK;
    else if (strcmp(text, "secure-v2-auth-failure-no-fallback") == 0)
        *scenario = FIXTURE_SCENARIO_SECURE_V2_AUTH_FAILURE_NO_FALLBACK;
    else if (strcmp(text, "secure-v2-transport-failure-no-fallback") == 0)
        *scenario = FIXTURE_SCENARIO_SECURE_V2_TRANSPORT_FAILURE_NO_FALLBACK;
    else if (strcmp(text, "secure-v2-egress-started-no-fallback") == 0)
        *scenario = FIXTURE_SCENARIO_SECURE_V2_EGRESS_STARTED_NO_FALLBACK;
    else
        return 0;
    return 1;
}

static const char *scenario_name(FIXTURE_SCENARIO scenario)
{
    if (scenario == FIXTURE_SCENARIO_STALL) return "stall";
    if (scenario == FIXTURE_SCENARIO_DATA_FAILURE) return "data-failure";
    if (scenario == FIXTURE_SCENARIO_MODE_B_EGRESS) return "mode-b-egress";
    if (scenario == FIXTURE_SCENARIO_MODE_B_TLS_NAME_FAILURE)
        return "mode-b-tls-name-failure";
    if (scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS)
        return "tcp-plain-egress";
    if (scenario == FIXTURE_SCENARIO_MIXED_EGRESS) return "mixed-egress";
    if (scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE)
        return "tls-truncated-content-length-complete";
    if (scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE)
        return "tls-truncated-content-length-incomplete";
    if (scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_COMPLETE)
        return "tls-truncated-chunked-complete";
    if (scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_INCOMPLETE)
        return "tls-truncated-chunked-incomplete";
    if (scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CLOSE_DELIMITED)
        return "tls-truncated-close-delimited";
    if (scenario == FIXTURE_SCENARIO_TLS_CLEAN_CLOSE)
        return "tls-clean-close";
    if (scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE_MULTI)
        return "tls-truncated-content-length-complete-multi-frame";
    if (scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE_MULTI)
        return "tls-truncated-content-length-incomplete-multi-frame";
    if (scenario == FIXTURE_SCENARIO_SECURE_V2_UNAVAILABLE_V1_FALLBACK)
        return "secure-v2-unavailable-v1-fallback";
    if (scenario == FIXTURE_SCENARIO_SECURE_V2_PROTOCOL_ERROR_NO_FALLBACK)
        return "secure-v2-protocol-error-no-fallback";
    if (scenario == FIXTURE_SCENARIO_SECURE_V2_AUTH_FAILURE_NO_FALLBACK)
        return "secure-v2-auth-failure-no-fallback";
    if (scenario == FIXTURE_SCENARIO_SECURE_V2_TRANSPORT_FAILURE_NO_FALLBACK)
        return "secure-v2-transport-failure-no-fallback";
    if (scenario == FIXTURE_SCENARIO_SECURE_V2_EGRESS_STARTED_NO_FALLBACK)
        return "secure-v2-egress-started-no-fallback";
    return "positive";
}

static PAPACC_BOOL is_tls_http_scenario(FIXTURE_SCENARIO scenario)
{
    return scenario == FIXTURE_SCENARIO_MODE_B_EGRESS ||
        scenario == FIXTURE_SCENARIO_MODE_B_TLS_NAME_FAILURE ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_COMPLETE ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CHUNKED_INCOMPLETE ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CLOSE_DELIMITED ||
        scenario == FIXTURE_SCENARIO_TLS_CLEAN_CLOSE ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE_MULTI ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE_MULTI ||
        scenario == FIXTURE_SCENARIO_SECURE_V2_UNAVAILABLE_V1_FALLBACK ||
        scenario == FIXTURE_SCENARIO_SECURE_V2_EGRESS_STARTED_NO_FALLBACK ?
        PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_BOOL is_fallback_scenario(FIXTURE_SCENARIO scenario)
{
    return scenario >= FIXTURE_SCENARIO_SECURE_V2_UNAVAILABLE_V1_FALLBACK ?
        PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_BOOL secure_authentication_completed(
    const PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidates,
    PAPACC_SIZE capacity)
{
    PAPACC_SIZE index;
    for (index = 0U; index < capacity; ++index) {
        if (candidates[index].state == PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL &&
            candidates[index].connection_security.state ==
                PAPACC_SECURITY_CONTEXT_AUTHORIZED)
            return PAPACC_TRUE;
    }
    return PAPACC_FALSE;
}

static PAPACC_BOOL authenticated_transport_ready_for_close(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidates, PAPACC_SIZE capacity)
{
    PAPACC_SIZE index;
    for (index = 0U; index < capacity; ++index) {
        if ((candidates[index].state == PAPACC_SERVER_SECURE_CANDIDATE_AUTHORIZED ||
             candidates[index].state == PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL) &&
            candidates[index].connection_security.state ==
                PAPACC_SECURITY_CONTEXT_AUTHORIZED) {
            return PAPACC_TRUE;
        }
    }
    return PAPACC_FALSE;
}

static void target_server_config(PST_CONNECTION_CONFIG *configuration,
    pst_credentials *credentials,PAPACC_BOOL require_alpn)
{
    static const PST_ALPN_PROTOCOL alpn = {
        (const pst_u8 *)"papacc/1", 8U };
    memset(configuration, 0, sizeof(*configuration));
    configuration->struct_size = sizeof(*configuration);
    configuration->api_version = PST_API_VERSION;
    configuration->role = PST_CONNECTION_ROLE_SERVER;
    configuration->provider_selection.struct_size =
        sizeof(configuration->provider_selection);
    configuration->provider_selection.api_version = PST_API_VERSION;
    configuration->provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    configuration->provider_selection.exact_provider_id = "openssl";
    configuration->provider_selection.required_capabilities =
        PST_CAP_TLS_1_3 | PST_CAP_ROLE_SERVER | PST_CAP_LOCAL_IDENTITY |
        PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT |
        PST_CAP_GRACEFUL_SHUTDOWN;
    if(require_alpn==PAPACC_TRUE)
        configuration->provider_selection.required_capabilities|=
            PST_CAP_ALPN_SERVER;
    configuration->local_identity.struct_size =
        sizeof(configuration->local_identity);
    configuration->local_identity.api_version = PST_API_VERSION;
    configuration->local_identity.credentials = credentials;
    configuration->peer_authentication.struct_size =
        sizeof(configuration->peer_authentication);
    configuration->peer_authentication.api_version = PST_API_VERSION;
    configuration->peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_DISABLED;
    configuration->tls.struct_size = sizeof(configuration->tls);
    configuration->tls.api_version = PST_API_VERSION;
    configuration->tls.minimum_version = PST_TLS_VERSION_1_3;
    configuration->tls.maximum_version = PST_TLS_VERSION_1_3;
    configuration->tls.resumption = PST_FEATURE_DISABLED;
    configuration->tls.early_data = PST_FEATURE_DISABLED;
    configuration->tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    configuration->alpn.struct_size = sizeof(configuration->alpn);
    configuration->alpn.api_version = PST_API_VERSION;
    configuration->alpn.mode = require_alpn==PAPACC_TRUE?
        PST_FEATURE_REQUIRED:PST_FEATURE_DISABLED;
    configuration->alpn.protocols = require_alpn==PAPACC_TRUE?&alpn:NULL;
    configuration->alpn.protocol_count = require_alpn==PAPACC_TRUE?1U:0U;
}

static int has_http_headers(const PAPACC_U8 *bytes, PAPACC_SIZE size)
{
    PAPACC_SIZE index;
    for (index = 3U; index < size; ++index)
        if (bytes[index - 3U] == '\r' && bytes[index - 2U] == '\n' &&
            bytes[index - 1U] == '\r' && bytes[index] == '\n') return 1;
    return 0;
}

static int serve_https_connection(FIXTURE_HTTPS_CONNECTION *worker)
{
    FIXTURE_HTTPS_TARGET *fixture = worker->fixture;
    SOCKET accepted = worker->socket;
    PAPACC_U8 bytes[32768];
    PAPACC_SIZE size = 0U, offset, index;
    pst_transport *transport = NULL;
    pst_connection *connection = NULL;
    PST_CONNECTION_CONFIG configuration;
    pst_u32 ownership = 0U, operation = PST_OPERATION_NEED_READ;
    PST_RESULT source = PST_RESULT_OK;
    u_long nonblocking = 1UL;
    int http = 0;
    int success = 0;
    int registered = 0, waited = 0;
    unsigned int failure_line = 0U;
    ULONGLONG deadline = GetTickCount64() + 5000U;
    if (fixture->use_tls != PAPACC_TRUE) {
        int received;
        const char *response = fixture_plain_http_response;
        PAPACC_SIZE response_size = sizeof(fixture_plain_http_response) - 1U;
        u_long nonblocking_plain = 1UL;
        if (ioctlsocket(accepted, FIONBIO, &nonblocking_plain) == SOCKET_ERROR)
            { failure_line = __LINE__; goto cleanup; }
        while (size < sizeof(bytes) && !has_http_headers(bytes, size)) {
            if (WaitForSingleObject(fixture->stop_event, 0U) == WAIT_OBJECT_0 ||
                GetTickCount64() >= deadline) { failure_line = __LINE__; goto cleanup; }
            received = recv(accepted, (char *)bytes + size,
                (int)(sizeof(bytes) - size), 0);
            if (received > 0) size += (PAPACC_SIZE)received;
            else if (received == 0) { failure_line = __LINE__; goto cleanup; }
            else if (WSAGetLastError() == WSAEWOULDBLOCK) Sleep(1U);
            else { failure_line = __LINE__; goto cleanup; }
        }
        if (size < 4U || memcmp(bytes, "GET ", 4U) != 0) { failure_line = __LINE__; goto cleanup; }
        for (offset = 0U; offset < response_size;) {
            PAPACC_SIZE chunk = response_size - offset > 19U ? 19U :
                response_size - offset;
            int written;
            if (WaitForSingleObject(fixture->stop_event, 0U) == WAIT_OBJECT_0 ||
                GetTickCount64() >= deadline) { failure_line = __LINE__; goto cleanup; }
            written = send(accepted, response + offset, (int)chunk, 0);
            if (written > 0) offset += (PAPACC_SIZE)written;
            else if (written == SOCKET_ERROR &&
                WSAGetLastError() == WSAEWOULDBLOCK) Sleep(1U);
            else { failure_line = __LINE__; goto cleanup; }
        }
        shutdown(accepted, SD_SEND);
        (void)InterlockedIncrement(&fixture->response_count);
        success = 1;
        { failure_line = __LINE__; goto cleanup; }
    }
    target_server_config(&configuration,fixture->credentials,
        fixture->require_alpn);
    if (ioctlsocket(accepted, FIONBIO, &nonblocking) == SOCKET_ERROR ||
        pst_connection_create(fixture->runtime, &configuration,
            &connection) != PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)accepted,
            &transport) != PST_RESULT_OK ||
        pst_connection_attach(connection, transport,
            PST_OWNERSHIP_TRANSFERRED, &ownership) != PST_RESULT_OK ||
        ownership == 0U) { failure_line = __LINE__; goto cleanup; }
    accepted = INVALID_SOCKET;
    transport = NULL;
    if (pst_wait_set_add_connection(worker->wait_set, connection, 1U) !=
            PST_RESULT_OK) { failure_line = __LINE__; goto cleanup; }
    registered = 1;
    for (index = 0U; index < 5000U; ++index) {
        if (pst_connection_handshake(connection, &operation, &source) !=
                PST_RESULT_OK || operation == PST_OPERATION_FAILED ||
            operation == PST_OPERATION_CLOSED) { failure_line = __LINE__; goto cleanup; }
        if (operation == PST_OPERATION_COMPLETE) break;
        if (fixture_operation_needs_wait(operation) &&
            fixture_tls_wait(worker, deadline, "HANDSHAKE") != 1) { failure_line = __LINE__; goto cleanup; }
    }
    if (operation != PST_OPERATION_COMPLETE) { failure_line = __LINE__; goto cleanup; }
    deadline = GetTickCount64() + 5000U;
    while (size < sizeof(bytes)) {
        PST_IO_RESULT io;
        if (pst_connection_read(connection, bytes + size,
                sizeof(bytes) - size, &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) { failure_line = __LINE__; goto cleanup; }
        size += io.bytes_transferred;
        if (size >= 4U && memcmp(bytes, "GET ", 4U) == 0) http = 1;
        if ((http && has_http_headers(bytes, size)) ||
            (!http && size == sizeof(bytes))) break;
        if (fixture_operation_needs_wait(io.operation) &&
            fixture_tls_wait(worker, deadline, "READ") != 1) { failure_line = __LINE__; goto cleanup; }
    }
    if (http) {
        const PAPACC_U8 *response = fixture->response != NULL ?
            fixture->response : (const PAPACC_U8 *)fixture_http_response;
        PAPACC_SIZE response_size = fixture->response != NULL ?
            fixture->response_size : sizeof(fixture_http_response) - 1U;
        for (offset = 0U; offset < response_size;) {
            PST_IO_RESULT io;
            PAPACC_SIZE chunk = response_size - offset;
            if (fixture->fragment_response == PAPACC_TRUE && chunk > 23U)
                chunk = 23U;
            if (pst_connection_write(connection, response + offset, chunk,
                    &io) != PST_RESULT_OK ||
                io.operation == PST_OPERATION_FAILED ||
                io.operation == PST_OPERATION_CLOSED) { failure_line = __LINE__; goto cleanup; }
            offset += io.bytes_transferred;
            if (offset < response_size &&
                fixture_operation_needs_wait(io.operation) &&
                fixture_tls_wait(worker, deadline, "WRITE") != 1) { failure_line = __LINE__; goto cleanup; }
        }
    } else {
        for (offset = 0U; offset < size;) {
            PST_IO_RESULT io;
            if (pst_connection_write(connection, bytes + offset,
                    size - offset, &io) != PST_RESULT_OK ||
                io.operation == PST_OPERATION_FAILED ||
                io.operation == PST_OPERATION_CLOSED) { failure_line = __LINE__; goto cleanup; }
            offset += io.bytes_transferred;
            if (offset < size && fixture_operation_needs_wait(io.operation) &&
                fixture_tls_wait(worker, deadline, "WRITE") != 1) { failure_line = __LINE__; goto cleanup; }
        }
    }
    (void)InterlockedIncrement(&fixture->response_count);
    success = 1;
    puts("FIXTURE_APPLICATION_RESPONSE_SENT=YES"); fflush(stdout);
    if (fixture->omit_close_notify == PAPACC_TRUE) { failure_line = __LINE__; goto cleanup; }
    /* Preserve the existing response delivery window, but make stop wake it. */
    (void)WaitForSingleObject(fixture->stop_event, 500U);
    deadline = GetTickCount64() + 5000U;
    operation = PST_OPERATION_NEED_READ;
    for (index = 0U; index < 5000U; ++index) {
        if (pst_connection_shutdown(connection, &operation, &source) !=
                PST_RESULT_OK || operation == PST_OPERATION_FAILED) {
            success = 0;
            fprintf(stderr, "FIXTURE_TLS_TERMINAL=FAILED source=%lu\n",
                (unsigned long)source);
            break;
        }
        if (operation == PST_OPERATION_COMPLETE ||
            operation == PST_OPERATION_CLOSED) {
            fixture->observed_shutdown_operation = operation;
            fixture->observed_shutdown_source = source;
            puts("FIXTURE_TLS_TERMINAL=GRACEFUL"); fflush(stdout);
            (void)InterlockedIncrement(&fixture->graceful_count);
            break;
        }
        waited = fixture_tls_wait(worker, deadline, "TLS_SHUTDOWN");
        if (waited != 1) {
            printf("FIXTURE_TLS_TERMINAL=%s\n", waited == -1 ?
                "CANCELLED_BY_STOP_NOT_CLOSE_NOTIFY" : "DEADLINE_OR_WAIT_FAILURE");
            fflush(stdout);
            /* Delivered application bytes remain delivered. Explicit fixture
               cancellation is not a claim of reciprocal TLS shutdown. */
            if (waited != -1) success = 0;
            else (void)InterlockedIncrement(&fixture->cancelled_shutdown_count);
            break;
        }
    }
    if (index == 5000U) success = 0;
cleanup:
    if (!success)
        fprintf(stderr, "FIXTURE_SERVER_FAILURE line=%u registered=%d wsa=%d\n",
            failure_line, registered, WSAGetLastError());
    if (fixture->expect_tls_failure == PAPACC_TRUE)
        success = 1;
    if (!success)
        fprintf(stderr,
            "FIXTURE_HTTPS_CONNECTION_FAILURE operation=%lu source=%lu bytes=%zu http=%d\n",
            (unsigned long)operation, (unsigned long)source, (size_t)size,
            http);
    if (registered) (void)pst_wait_set_remove_connection(worker->wait_set, connection);
    if (connection != NULL) pst_connection_release(connection);
    if (transport != NULL) pst_transport_release(transport);
    if (accepted != INVALID_SOCKET) closesocket(accepted);
    return success;
}

static DWORD WINAPI https_connection_thread(void *value)
{
    FIXTURE_HTTPS_CONNECTION *connection =
        (FIXTURE_HTTPS_CONNECTION *)value;
    pst_wait_set *wait_set = NULL;
    DWORD result;
    if (pst_wait_set_create(&wait_set) != PST_RESULT_OK) {
        closesocket(connection->socket);
        return 3U;
    }
    EnterCriticalSection(&connection->wake_lock);
    connection->wait_set = wait_set;
    LeaveCriticalSection(&connection->wake_lock);
    printf("WAITSET_CREATE_THREAD=%lu\n", (unsigned long)GetCurrentThreadId());
    result = serve_https_connection(connection) ? 0U : 1U;
    /* Revoke the wake reference before destroying. The lock is never held
       across TLS I/O or a blocking wait. A stop before publication is latched. */
    EnterCriticalSection(&connection->wake_lock);
    connection->wait_set = NULL;
    LeaveCriticalSection(&connection->wake_lock);
    if (pst_wait_set_destroy(wait_set) != PST_RESULT_OK) result = 4U;
    printf("WAITSET_DESTROY_THREAD=%lu RESULT=%lu\n",
        (unsigned long)GetCurrentThreadId(), (unsigned long)result);
    fflush(stdout);
    return result;
}

static int fixture_accept_event_failed(const WSANETWORKEVENTS *events)
{
    /* WSAEnumNetworkEvents leaves error slots for absent events untouched. */
    return (events->lNetworkEvents & FD_ACCEPT) != 0 &&
        events->iErrorCode[FD_ACCEPT_BIT] != 0;
}

static DWORD WINAPI https_target_thread(void *value)
{
    FIXTURE_HTTPS_TARGET *fixture = (FIXTURE_HTTPS_TARGET *)value;
    HANDLE workers[FIXTURE_CAPACITY];
    FIXTURE_HTTPS_CONNECTION connections[FIXTURE_CAPACITY];
    PAPACC_SIZE count = 0U, index;
    DWORD result = 0U;
    WSAEVENT listener_event = WSACreateEvent();
    HANDLE waits[2];
    memset(workers, 0, sizeof(workers));
    memset(connections, 0, sizeof(connections));
    if (listener_event == WSA_INVALID_EVENT) return 1U;
    if (WSAEventSelect(fixture->listener, listener_event, FD_ACCEPT) != 0) {
        WSACloseEvent(listener_event);
        return 1U;
    }
    waits[0] = fixture->stop_event; waits[1] = listener_event;
    while (WaitForSingleObject(fixture->stop_event, 0U) == WAIT_TIMEOUT) {
        SOCKET accepted = accept(fixture->listener, NULL, NULL);
        if (accepted == INVALID_SOCKET) {
            WSANETWORKEVENTS events;
            DWORD ready;
            int enum_result;
            if (WSAGetLastError() != WSAEWOULDBLOCK) {
                fprintf(stderr, "FIXTURE_ACCEPT_FAILURE wsa=%d\n", WSAGetLastError());
                result = 1U; break;
            }
            ready = WaitForMultipleObjects(2U, waits, FALSE, INFINITE);
            if (ready == WAIT_OBJECT_0) break;
            memset(&events, 0, sizeof(events));
            enum_result = ready == WAIT_OBJECT_0 + 1U ?
                WSAEnumNetworkEvents(fixture->listener, listener_event, &events) : SOCKET_ERROR;
            if (enum_result != 0 || fixture_accept_event_failed(&events)) {
                fprintf(stderr, "FIXTURE_ENUM_FAILURE ready=%lu enum=%d events=%ld accept_error=%d\n",
                    (unsigned long)ready, enum_result,
                    enum_result == 0 ? events.lNetworkEvents : 0L,
                    enum_result == 0 ? events.iErrorCode[FD_ACCEPT_BIT] : 0);
                result = 1U; break;
            }
            continue;
        }
        /* An accepted socket inherits event selection; PST owns its own
           readiness. Remove the listener association before transfer. */
        if (WSAEventSelect(accepted, NULL, 0) != 0) {
            closesocket(accepted); result = 1U; break;
        }
        if (count == FIXTURE_CAPACITY) {
            closesocket(accepted);
            result = 2U;
            break;
        }
        connections[count].fixture = fixture;
        connections[count].socket = accepted;
        InitializeCriticalSection(&connections[count].wake_lock);
        workers[count] = CreateThread(NULL, 0U, https_connection_thread,
            &connections[count], 0U, NULL);
        if (workers[count] == NULL) {
            closesocket(accepted);
            DeleteCriticalSection(&connections[count].wake_lock);
            result = 3U;
            break;
        }
        ++count;
        /* Optional test observation: duplicate kernel thread handle, never
           transfer a PST handle. Observer closes its copy after target join. */
        if (fixture->observed_worker_handle != NULL && count == 1U) {
            if (!DuplicateHandle(GetCurrentProcess(), workers[count - 1U],
                    GetCurrentProcess(), fixture->observed_worker_handle,
                    SYNCHRONIZE | THREAD_QUERY_INFORMATION, FALSE, 0U)) {
                result = 4U;
                break;
            }
            SetEvent(fixture->worker_published_event);
        }
        ++fixture->accepted_count;
        fixture->completed = PAPACC_TRUE;
    }
    if (result != 0U)
        fprintf(stderr, "FIXTURE_LISTENER_FAILURE result=%lu wsa=%d\n",
            (unsigned long)result, WSAGetLastError());
    /* Wake every worker before joining any one of them. Stable wait-set
       ownership covers a stop racing registration or an already exited worker. */
    SetEvent(fixture->stop_event);
    for (index = 0U; index < count; ++index)
        if (fixture_signal_worker(&connections[index]) != PST_RESULT_OK)
            result = 4U;
    for (index = 0U; index < count; ++index) {
        if (fixture_join_thread(&workers[index], 10000U, fixture->stop_event,
                NULL) != 0U)
            result = 4U;
        DeleteCriticalSection(&connections[index].wake_lock);
    }
    (void)WSAEventSelect(fixture->listener, NULL, 0);
    WSACloseEvent(listener_event);
    return result;
}

static int create_target_listener_on_port(SOCKET *listener, PAPACC_U16 requested,
    PAPACC_U16 *port, PAPACC_NETWORK_ENDPOINT *endpoint)
{
    struct sockaddr_in address;
    int length = (int)sizeof(address);
    u_long nonblocking = 1UL;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(requested);
    *listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*listener == INVALID_SOCKET || bind(*listener,
            (const struct sockaddr *)&address, (int)sizeof(address)) ==
            SOCKET_ERROR || listen(*listener, 4) == SOCKET_ERROR ||
        getsockname(*listener, (struct sockaddr *)&address, &length) ==
            SOCKET_ERROR ||
        ioctlsocket(*listener, FIONBIO, &nonblocking) == SOCKET_ERROR ||
        papacc_ip_address_set_ipv4(&endpoint->address,
            127U, 0U, 0U, 1U) != PAPACC_RESULT_OK)
        return 0;
    *port = (PAPACC_U16)ntohs(address.sin_port);
    endpoint->port = *port;
    return 1;
}

static int create_target_listener(SOCKET *listener, PAPACC_U16 *port,
    PAPACC_NETWORK_ENDPOINT *endpoint)
{
    return create_target_listener_on_port(listener, 0U, port, endpoint);
}

static int publish_ready(const char *ready_path, PAPACC_U16 port,
    PAPACC_U16 target_port, PAPACC_U16 plain_target_port,
    const char *stop_event_name,
    FIXTURE_SCENARIO scenario)
{
    char temporary[MAX_PATH * 2];
    char ca_path[MAX_PATH * 2];
    char certificate_path[MAX_PATH * 2];
    char key_path[MAX_PATH * 2];
    char alternate_certificate_path[MAX_PATH * 2];
    char alternate_key_path[MAX_PATH * 2];
    FILE *file = NULL;
    const FIXTURE_HTTP_SCENARIO_METADATA *metadata =
        fixture_http_metadata(scenario);
    if (ready_path == NULL || stop_event_name == NULL ||
        sprintf_s(temporary, sizeof(temporary), "%s.tmp.%lu", ready_path,
            (unsigned long)GetCurrentProcessId()) < 0 ||
        GetFullPathNameA(PAPACC_TEST_PKI_DIR "/ca.der.b64", sizeof(ca_path),
            ca_path, NULL) == 0U ||
        GetFullPathNameA(PAPACC_TEST_PKI_DIR "/client-a.der.b64",
            sizeof(certificate_path), certificate_path, NULL) == 0U ||
        GetFullPathNameA(PAPACC_TEST_PKI_DIR "/client-a.pk8.b64",
            sizeof(key_path), key_path, NULL) == 0U ||
        GetFullPathNameA(PAPACC_TEST_PKI_DIR "/client-b.der.b64",
            sizeof(alternate_certificate_path), alternate_certificate_path,
            NULL) == 0U ||
        GetFullPathNameA(PAPACC_TEST_PKI_DIR "/client-b.pk8.b64",
            sizeof(alternate_key_path), alternate_key_path, NULL) == 0U ||
        fopen_s(&file, temporary, "wb") != 0 || file == NULL)
        return 0;
    if (metadata != NULL) {
        if (!fixture_http_metadata_consistent(metadata)) {
            fclose(file);
            DeleteFileA(temporary);
            return 0;
        }
        if (fprintf(file,
            "fixture_version=3\nfixture_scenario=%s\n"
            "CONTROL_host=127.0.0.1\nCONTROL_port=%u\n"
            "DATA_host=127.0.0.1\nDATA_port=%u\n"
            "expected_server_name=papacc-test-server\n"
            "ALPN=papacc/1\nTLS_version=1.3\nmTLS=required\n"
            "test_CA_public_path=%s\n"
            "browser_certificate_path=%s\n"
            "browser_private_key_path=%s\n"
            "capability_policy_expectation=NETWORK_EGRESS,TLS_OFFLOAD\n"
            "NETWORK_EGRESS_major=2\nTLS_OFFLOAD_granted=YES\n"
            "target_host=papacc-test-server\ntarget_port=%u\n"
            "target_scheme=https\nnetwork_mode=TLS_REQUIRED\nTLS_REQUIRED=1\n"
            "resolver_policy=controlled\nsite_tls_verification=required\n"
            "expected_HTTP_status=200\nexpected_HTTP_body=%s\n"
            "expected_HTTP_body_trailing_lf=YES\n"
            "expected_HTTP_body_length=%u\n"
            "sequential_request_count=1\nHTTP_framing=%s\n"
            "declared_content_length=%u\ndelivered_body_length=%u\n"
            "HTTP_complete=%s\nclean_close_notify=%s\n"
            "expected_terminal=%s\nexpected_terminal_reason=%s\n"
            "pacc_fragmentation=%s\nterminal_after_final_NETWORK_DATA=YES\n"
            "fragmentation_variants_available=YES\nstop_event=%s\n",
            scenario_name(scenario), (unsigned int)port, (unsigned int)port,
            ca_path, certificate_path, key_path, (unsigned int)target_port,
            metadata->expected_body, metadata->delivered_body_length,
            metadata->framing, metadata->declared_body_length,
            metadata->delivered_body_length,
            metadata->http_complete == PAPACC_TRUE ? "YES" : "NO",
            metadata->clean_close_notify == PAPACC_TRUE ? "YES" : "NO",
            metadata->clean_close_notify == PAPACC_TRUE ?
                "NETWORK_CLOSE" : "NETWORK_ABORT_REASON",
            metadata->clean_close_notify == PAPACC_TRUE ?
                "NONE" : "TLS_TRUNCATED",
            metadata->multi_frame == PAPACC_TRUE ? "MULTI_FRAME" : "ONE_FRAME",
            stop_event_name) < 0 ||
            fclose(file) != 0) {
            if (file != NULL) fclose(file);
            DeleteFileA(temporary);
            return 0;
        }
        file = NULL;
    } else if (is_fallback_scenario(scenario) == PAPACC_TRUE) {
        const char *failure_class = "NONE";
        const char *fallback_permitted = "NO";
        unsigned int runner_target_port = target_port != 0U ?
            (unsigned int)target_port : (unsigned int)port;
        unsigned int expected_sessions = 1U;
        unsigned int expected_control_connections = 1U;
        unsigned int second_major = 0U;
        const char *expected_operation = "NO";
        const char *failure_stage = "CAPABILITY_NEGOTIATION";
        const char *egress_expected = "NO";
        if (scenario == FIXTURE_SCENARIO_SECURE_V2_UNAVAILABLE_V1_FALLBACK) {
            fallback_permitted = "YES";
            expected_sessions = 2U;
            expected_control_connections = 2U;
            second_major = 1U;
            expected_operation = "PASS";
        } else if (scenario ==
                FIXTURE_SCENARIO_SECURE_V2_PROTOCOL_ERROR_NO_FALLBACK)
            failure_class = "PROTOCOL_ERROR";
        else if (scenario ==
                FIXTURE_SCENARIO_SECURE_V2_AUTH_FAILURE_NO_FALLBACK) {
            failure_class = "AUTHENTICATION_FAILURE";
            failure_stage = "PRINCIPAL_RESOLUTION_BEFORE_CONTROL";
            expected_control_connections = 0U;
        }
        else if (scenario ==
                FIXTURE_SCENARIO_SECURE_V2_TRANSPORT_FAILURE_NO_FALLBACK) {
            failure_class = "TRANSPORT_FAILURE";
            failure_stage = "AUTHENTICATED_BEFORE_CONTROL";
        }
        else {
            failure_class = "POST_EGRESS_TLS_TRUNCATED";
            failure_stage = "NETWORK_STREAM_ACTIVE";
            egress_expected = "YES";
            expected_operation = "STARTED_THEN_FAILED";
        }
        if (fprintf(file,
            "fixture_version=4\nfixture_scenario=%s\n"
            "CONTROL_host=127.0.0.1\nCONTROL_port=%u\n"
            "DATA_host=127.0.0.1\nDATA_port=%u\n"
            "expected_server_name=papacc-test-server\n"
            "ALPN=papacc/1\nTLS_version=1.3\nmTLS=required\n"
            "test_CA_public_path=%s\n"
            "browser_certificate_path=%s\n"
            "browser_private_key_path=%s\n"
            "negative_client_certificate_path=%s\n"
            "negative_client_private_key_path=%s\n"
            "capability_policy_expectation=NETWORK_EGRESS,TLS_OFFLOAD\n"
            "target_host=papacc-test-server\ntarget_port=%u\n"
            "target_scheme=https\nnetwork_mode=TLS_REQUIRED\nTLS_REQUIRED=1\n"
            "resolver_policy=controlled\nsite_tls_verification=required\n"
            "expected_HTTP_status=200\nexpected_HTTP_body=papacc-mode-b-ok\n"
            "sequential_request_count=1\n"
            "expected_secure_session_count=%u\n"
            "expected_control_connection_count=%u\n"
            "expected_CONTROL_connection_count=%u\n"
            "expected_first_offer_network_egress_major=2\n"
            "expected_second_offer_network_egress_major=%u\n"
            "fallback_permitted=%s\nfallback_legal=%s\n"
            "expected_failure_class=%s\nexpected_failure_stage=%s\n"
            "expected_v1_offer_count=0\n"
            "egress_expected_to_start=%s\n"
            "expected_v1_operation=%s\n"
            "in_place_downgrade=NO\nlegacy_connection_count=0\n"
            "secure_to_legacy_fallback=NO\nTLS_downgrade=NO\n"
            "malformed_capability_fixture_available=NO\n"
            "malformed_capability_fixture_gap=canonical_server_processor_always_emits_codec_valid_result\n"
            "stop_event=%s\n",
            scenario_name(scenario), (unsigned int)port, (unsigned int)port,
            ca_path, certificate_path, key_path, alternate_certificate_path,
            alternate_key_path, runner_target_port,
            expected_sessions, expected_control_connections,
            expected_control_connections, second_major,
            fallback_permitted, fallback_permitted, failure_class,
            failure_stage, egress_expected, expected_operation,
            stop_event_name) < 0 || fclose(file) != 0) {
            if (file != NULL) fclose(file);
            DeleteFileA(temporary);
            return 0;
        }
        file = NULL;
    } else if (scenario == FIXTURE_SCENARIO_MIXED_EGRESS) {
        if (fprintf(file,
            "fixture_version=2\nfixture_scenario=mixed-egress\n"
            "CONTROL_host=127.0.0.1\nCONTROL_port=%u\n"
            "DATA_host=127.0.0.1\nDATA_port=%u\n"
            "expected_server_name=papacc-test-server\n"
            "ALPN=papacc/1\nTLS_version=1.3\nmTLS=required\n"
            "test_CA_public_path=%s\n"
            "browser_certificate_path=%s\n"
            "browser_private_key_path=%s\n"
            "expected_capabilities=NETWORK_EGRESS,TLS_OFFLOAD\n"
            "resolver_policy=controlled\nsequential_operation_count=3\n"
            "target_host=papacc-test-server\ntarget_port=%u\n"
            "target_scheme=https\nnetwork_mode=MIXED\nTLS_REQUIRED=1\n"
            "expected_HTTP_body=papacc-mode-b-ok\n"
            "sequential_request_count=3\n"
            "tls_target_host=papacc-test-server\ntls_target_port=%u\n"
            "tls_target_scheme=https\ntls_target_network_mode=TLS_REQUIRED\n"
            "tls_expected_HTTP_status=200\n"
            "tls_expected_HTTP_body=papacc-mode-b-ok\n"
            "plain_target_host=papacc-plain-test-server\n"
            "plain_target_port=%u\nplain_target_scheme=http\n"
            "plain_target_network_mode=TCP_PLAIN\n"
            "plain_expected_HTTP_status=200\n"
            "plain_expected_HTTP_body=papacc-tcp-plain-ok\n"
            "stop_event=%s\n", (unsigned int)port, (unsigned int)port,
            ca_path, certificate_path, key_path, (unsigned int)target_port,
            (unsigned int)target_port,
            (unsigned int)plain_target_port, stop_event_name) < 0 ||
            fclose(file) != 0) {
            if (file != NULL) fclose(file);
            DeleteFileA(temporary);
            return 0;
        }
        file = NULL;
    } else if (fprintf(file,
            "fixture_version=2\n"
            "fixture_scenario=%s\n"
            "CONTROL_host=127.0.0.1\nCONTROL_port=%u\n"
            "DATA_host=127.0.0.1\nDATA_port=%u\n"
            "expected_server_name=papacc-test-server\n"
            "ALPN=papacc/1\nTLS_version=1.3\nmTLS=required\n"
            "test_CA_public_path=%s\n"
            "browser_certificate_path=%s\n"
            "browser_private_key_path=%s\n"
            "capability_policy_expectation=NETWORK_EGRESS,TLS_OFFLOAD\n"
            "target_host=%s\ntarget_port=%u\ntarget_scheme=%s\n"
            "network_mode=%s\nTLS_REQUIRED=%u\nresolver_policy=controlled\n"
            "site_tls_verification=%s\n"
            "expected_HTTP_status=200\n"
            "expected_HTTP_body=%s\n"
            "sequential_request_count=%u\n"
            "stop_event=%s\n", scenario_name(scenario), (unsigned int)port,
            (unsigned int)port, ca_path, certificate_path, key_path,
            scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ?
                "papacc-plain-test-server" :
                (scenario == FIXTURE_SCENARIO_MODE_B_TLS_NAME_FAILURE ?
                    "wrong.example" : "papacc-test-server"),
            (unsigned int)target_port,
            scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ? "http" : "https",
            scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ?
                "TCP_PLAIN" : "TLS_REQUIRED",
            scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ? 0U : 1U,
            scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ?
                "not-applicable" : "required",
            scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ?
                "papacc-tcp-plain-ok" : "papacc-mode-b-ok",
            scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ? 3U : 1U,
            stop_event_name) < 0 ||
        fclose(file) != 0) {
        if (file != NULL) fclose(file);
        DeleteFileA(temporary);
        return 0;
    }
    if (!MoveFileExA(temporary, ready_path,
            MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        DeleteFileA(temporary);
        return 0;
    }
    return 1;
}

static int run_controlled_targets_on_ports(const char *ready_path,
    const char *stop_event_name, PAPACC_U16 requested_http_port,
    PAPACC_U16 requested_https_port)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture=
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime=NULL;
    HANDLE stop_event=NULL,tls_thread=NULL,plain_thread=NULL;
    SOCKET tls_listener=INVALID_SOCKET,plain_listener=INVALID_SOCKET;
    PAPACC_U16 tls_port=0U,plain_port=0U;
    PAPACC_NETWORK_ENDPOINT tls_endpoint=PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT plain_endpoint=PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    FIXTURE_HTTPS_TARGET tls_target,plain_target;
    int result=1;
    memset(&tls_target,0,sizeof(tls_target));
    memset(&plain_target,0,sizeof(plain_target));
    DeleteFileA(ready_path);
    if(WSAStartup(MAKEWORD(2,2),&winsock)!=0||
        papacc_pst_provider_bootstrap_win32(NULL)!=PAPACC_RESULT_OK||
        !papacc_test_security_fixture_init(&fixture))goto cleanup;
    memset(&options,0,sizeof(options));options.struct_size=sizeof(options);
    options.api_version=PST_API_VERSION;
    if(pst_runtime_create(&options,&runtime)!=PST_RESULT_OK||
        !create_target_listener_on_port(&tls_listener,requested_https_port,
            &tls_port,&tls_endpoint)||
        !create_target_listener_on_port(&plain_listener,requested_http_port,
            &plain_port,&plain_endpoint))
        goto cleanup;
    stop_event=CreateEventA(NULL,TRUE,FALSE,stop_event_name);
    if(stop_event==NULL)goto cleanup;
    tls_target.listener=tls_listener;tls_target.runtime=runtime;
    tls_target.credentials=fixture.server.credentials;
    tls_target.stop_event=stop_event;tls_target.use_tls=PAPACC_TRUE;
    plain_target.listener=plain_listener;plain_target.stop_event=stop_event;
    plain_target.use_tls=PAPACC_FALSE;
    tls_thread=CreateThread(NULL,0U,https_target_thread,&tls_target,0U,NULL);
    plain_thread=CreateThread(NULL,0U,https_target_thread,&plain_target,0U,NULL);
    if(tls_thread==NULL||plain_thread==NULL||!publish_ready(ready_path,0U,
        tls_port,plain_port,stop_event_name,FIXTURE_SCENARIO_MIXED_EGRESS))
        goto cleanup;
    (void)WaitForSingleObject(stop_event,INFINITE);
    if(fixture_join_thread(&tls_thread,20000U,stop_event,NULL)!=0U||
        fixture_join_thread(&plain_thread,20000U,stop_event,NULL)!=0U||
        tls_target.response_count<1U||plain_target.response_count<1U)
        goto cleanup;
    result=0;
cleanup:
    if(stop_event!=NULL)SetEvent(stop_event);
    if(fixture_join_thread(&tls_thread,20000U,stop_event,NULL)!=0U)result=1;
    if(fixture_join_thread(&plain_thread,20000U,stop_event,NULL)!=0U)result=1;
    if(tls_listener!=INVALID_SOCKET)closesocket(tls_listener);
    if(plain_listener!=INVALID_SOCKET)closesocket(plain_listener);
    if(stop_event!=NULL)CloseHandle(stop_event);
    if(runtime!=NULL)pst_runtime_release(runtime);
    papacc_test_security_fixture_release(&fixture);
    DeleteFileA(ready_path);WSACleanup();return result;
}

static int run_controlled_targets(const char *ready_path,
    const char *stop_event_name)
{
    return run_controlled_targets_on_ports(ready_path,stop_event_name,0U,0U);
}

static int configure_http_target(FIXTURE_HTTPS_TARGET *target,
    FIXTURE_SCENARIO scenario)
{
    const FIXTURE_HTTP_SCENARIO_METADATA *metadata;
    if (target == NULL) return 0;
    metadata = fixture_http_metadata(scenario);
    if (metadata != NULL) {
        if (!fixture_http_metadata_consistent(metadata)) return 0;
        target->response = (const PAPACC_U8 *)metadata->response;
    }
    if (target->response != NULL && target->response_size == 0U)
        target->response_size = (PAPACC_SIZE)strlen((const char *)target->response);
    target->omit_close_notify = scenario != FIXTURE_SCENARIO_TLS_CLEAN_CLOSE &&
        scenario >= FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE ?
        PAPACC_TRUE : PAPACC_FALSE;
    target->fragment_response =
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_COMPLETE_MULTI ||
        scenario == FIXTURE_SCENARIO_TLS_TRUNCATED_CONTENT_LENGTH_INCOMPLETE_MULTI ?
        PAPACC_TRUE : PAPACC_FALSE;
    return 1;
}

static int configure_v1_only_capabilities(PAPACC_SERVER_IO_LOOP_WIN32 *loop)
{
    PAPACC_RESULT result;
    if (loop == NULL) return 0;
    papacc_capability_set_release(&loop->enabled_capabilities);
    result = papacc_capability_set_init(&loop->enabled_capabilities,
        loop->enabled_capability_keys, PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS);
    if (result == PAPACC_RESULT_OK)
        result = papacc_capability_set_insert(&loop->enabled_capabilities,
            &PAPACC_CAPABILITY_NETWORK_EGRESS_V1);
    if (result == PAPACC_RESULT_OK)
        result = papacc_capability_set_insert(&loop->enabled_capabilities,
            &PAPACC_CAPABILITY_TLS_OFFLOAD_V1);
    return result == PAPACC_RESULT_OK;
}

static int closed_comparison_ports(PAPACC_U16 *control, PAPACC_U16 *target)
{
    char value[32], *end;
    unsigned long first, second;
    DWORD length = GetEnvironmentVariableA(
        "PAPACC_TEST_CLOSED_COMPARISON_PORTS", value, sizeof(value));
    *control = *target = 0U;
    if (length == 0U) return 1;
    if (length >= sizeof(value) || value[0] < '0' || value[0] > '9')
        return 0;
    first = strtoul(value, &end, 10);
    if (*end != ',' || end[1] < '0' || end[1] > '9') return 0;
    second = strtoul(end + 1, &end, 10);
    if (*end != '\0' || first == 0UL || first > 65535UL ||
        second == 0UL || second > 65535UL || first == second) return 0;
    *control = (PAPACC_U16)first;
    *target = (PAPACC_U16)second;
    return 1;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture =
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_CONNECTION connections[FIXTURE_CAPACITY];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT contexts[FIXTURE_CAPACITY];
    PAPACC_SESSION sessions[FIXTURE_CAPACITY];
    PAPACC_CHANNEL channels[FIXTURE_CAPACITY];
    PAPACC_DATA_ASSOCIATION_ENTRY associations[FIXTURE_CAPACITY];
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 slots[FIXTURE_CAPACITY];
    PAPACC_SERVER_IO_LOOP_WIN32 protocol_loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_SECURITY_COMPOSITION composition =
        PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION security =
        PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION_INITIALIZER;
    PAPACC_SECURITY_DER_ITEM server_certificate;
    PAPACC_SECURITY_DER_ITEM trust_anchor;
    PAPACC_LOGGER logger;
    PAPACC_SERVER_LISTENER_CONFIGURATION listener =
        PAPACC_SERVER_LISTENER_CONFIGURATION_INITIALIZER;
    PAPACC_SERVER_LISTENER_CONFIGURATION_SET listener_set = { &listener, 1U };
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidates[FIXTURE_CAPACITY];
    PAPACC_PST_SCHEDULER_MEMBER members[FIXTURE_CAPACITY + 1U +
        PAPACC_SERVER_NETWORK_STREAM_CAPACITY];
    PST_WAIT_EVENT events[FIXTURE_CAPACITY + 1U +
        PAPACC_SERVER_NETWORK_STREAM_CAPACITY];
    pst_external_source *listener_sources[1];
    pst_wait_token listener_tokens[1];
    PAPACC_SERVER_SECURE_IO_WIN32 secure_io =
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    struct sockaddr_in native_address;
    int native_length = sizeof(native_address);
    PAPACC_U16 port = 0U;
    PAPACC_U16 target_port = 0U;
    PAPACC_U16 plain_target_port = 0U;
    PAPACC_U16 comparison_control_port = 0U, comparison_target_port = 0U;
    PAPACC_SIZE index;
    HANDLE stop_event = NULL;
    HANDLE target_thread = NULL;
    HANDLE plain_target_thread = NULL;
    SOCKET target_listener = INVALID_SOCKET;
    SOCKET plain_target_listener = INVALID_SOCKET;
    FIXTURE_EGRESS egress = { PAPACC_NETWORK_ENDPOINT_INITIALIZER,
        PAPACC_NETWORK_ENDPOINT_INITIALIZER, 0U };
    FIXTURE_HTTPS_TARGET https_target;
    FIXTURE_HTTPS_TARGET plain_target;
    PAPACC_EGRESS_POLICY egress_policy = PAPACC_EGRESS_POLICY_INITIALIZER;
    PAPACC_EGRESS_PROFILE egress_profile = PAPACC_EGRESS_PROFILE_INITIALIZER;
    PAPACC_EGRESS_RUNTIME_CONFIG egress_config =
        PAPACC_EGRESS_RUNTIME_CONFIG_INITIALIZER;
    PAPACC_DNS_RESOLVER resolver = PAPACC_DNS_RESOLVER_INITIALIZER;
    PAPACC_OUTBOUND_TLS_POLICY outbound_policy;
    FIXTURE_POLICY_CONTEXT policy;
    FIXTURE_LOG_CONTEXT log_context;
    PAPACC_BOOL stall_engaged = PAPACC_FALSE;
    PAPACC_BOOL transport_fault_injected = PAPACC_FALSE;
    int result = 1;

    if(argc==4&&strcmp(argv[1],"--controlled-egress-targets")==0)
        return run_controlled_targets(argv[2],argv[3]);

    policy.scenario = FIXTURE_SCENARIO_POSITIVE;
    policy.associate_data_authorization_count = 0U;
    if (argc != 5 || strcmp(argv[1], "--browser-fixture") != 0 ||
        !parse_scenario(argv[2], &policy.scenario)) {
        fprintf(stderr, "usage: %s --browser-fixture <scenario> <ready-file> <stop-event>\n",
            argc > 0 ? argv[0] : "papacc_secure_fixture");
        return 2;
    }
    if (!closed_comparison_ports(&comparison_control_port,
            &comparison_target_port) ||
        (comparison_control_port != 0U &&
            policy.scenario != FIXTURE_SCENARIO_TLS_CLEAN_CLOSE)) return 2;
    log_context.scenario = policy.scenario;
    log_context.slots = slots;
    log_context.slot_capacity = FIXTURE_CAPACITY;
    log_context.protocol_fault_armed_count = 0U;
    log_context.valid_v2_offer_decoded_count = 0U;
    log_context.protocol_fault_realized_count = 0U;
    log_context.auth_control_published_count = 0U;
    log_context.transport_control_published_count = 0U;
    DeleteFileA(argv[3]);
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0 ||
        !papacc_test_security_fixture_init(&fixture) ||
        papacc_logger_init(&logger, fixture_log, &log_context,
            PAPACC_LOG_TRACE) != PAPACC_RESULT_OK)
        goto cleanup;
    server_certificate.data = fixture.server.certificate.data;
    server_certificate.size = fixture.server.certificate.size;
    trust_anchor.data = fixture.ca.data;
    trust_anchor.size = fixture.ca.size;
    security.valid = PAPACC_TRUE;
    security.composition_inputs.local_certificate_chain = &server_certificate;
    security.composition_inputs.local_certificate_count = 1U;
    security.composition_inputs.local_private_key_pkcs8_der =
        fixture.server.private_key.data;
    security.composition_inputs.local_private_key_pkcs8_der_size =
        fixture.server.private_key.size;
    security.composition_inputs.peer_trust_anchors = &trust_anchor;
    security.composition_inputs.peer_trust_anchor_count = 1U;
    security.composition_inputs.secure_principal_alpn =
        (const PAPACC_U8 *)PAPACC_SECURITY_ALPN_PAPACC_1;
    security.composition_inputs.secure_principal_alpn_size =
        PAPACC_SECURITY_ALPN_PAPACC_1_SIZE;
    security.composition_inputs.provider_id = "openssl";
    security.composition_inputs.provider_bootstrap =
        papacc_pst_provider_bootstrap_win32;
    security.composition_inputs.logger = &logger;
    security.principal_resolve = resolve_browser_principal;
    security.authorize = authorize_browser_principal;
    security.authorization_context = &policy;
    if (papacc_security_composition_init(&composition,
            &security.composition_inputs) != PAPACC_RESULT_OK ||
        papacc_tcp_platform_init(&network.tcp_platform) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&target.address, 127U, 0U, 0U, 1U) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(&network.tcp_platform, &target,
            comparison_control_port,
            &entry.socket) != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&entry.socket) != PAPACC_RESULT_OK)
        goto cleanup;
    memset(&native_address, 0, sizeof(native_address));
    if (getsockname(entry.socket.native_socket,
            (struct sockaddr *)&native_address, &native_length) == SOCKET_ERROR)
        goto cleanup;
    port = (PAPACC_U16)ntohs(native_address.sin_port);
    entry.target = target;
    network.listener_set.entries = &entry;
    network.listener_set.capacity = network.listener_set.count = 1U;
    network.listener_set.bound_port = port;
    network.listener_set.is_active = PAPACC_TRUE;
    network.listener_storage = &entry;
    network.listener_storage_capacity = 1U;
    network.is_active = PAPACC_TRUE;
    for (index = 0U; index < FIXTURE_CAPACITY; ++index)
        contexts[index] = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
            PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    if (papacc_server_acceptor_win32_init(&acceptor, &network, connections,
            FIXTURE_CAPACITY, contexts, FIXTURE_CAPACITY) != PAPACC_RESULT_OK ||
        papacc_server_io_loop_win32_init(&protocol_loop, &network, &acceptor,
            sessions, FIXTURE_CAPACITY, channels, FIXTURE_CAPACITY,
            associations, FIXTURE_CAPACITY, slots, FIXTURE_CAPACITY,
            5000000000ULL) != PAPACC_RESULT_OK)
        goto cleanup;
    papacc_server_io_loop_win32_set_logger(&protocol_loop, &logger);
    if (policy.scenario == FIXTURE_SCENARIO_SECURE_V2_UNAVAILABLE_V1_FALLBACK &&
        !configure_v1_only_capabilities(&protocol_loop))
        goto cleanup;
    stop_event = CreateEventA(NULL, TRUE, FALSE, argv[4]);
    if (stop_event == NULL) goto cleanup;
    memset(&https_target, 0, sizeof(https_target));
    memset(&plain_target, 0, sizeof(plain_target));
    memset(&outbound_policy, 0, sizeof(outbound_policy));
    if (is_tls_http_scenario(policy.scenario) == PAPACC_TRUE ||
        policy.scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ||
        policy.scenario == FIXTURE_SCENARIO_MIXED_EGRESS) {
        if (!create_target_listener_on_port(&target_listener,
                comparison_target_port, &target_port, &egress.endpoint))
            goto cleanup;
        if (comparison_control_port != 0U)
            printf("CLOSED_COMPARISON_ENDPOINTS control=127.0.0.1:%u "
                "target=127.0.0.1:%u\n", (unsigned int)port,
                (unsigned int)target_port);
        egress.plain_endpoint = egress.endpoint;
        if (policy.scenario == FIXTURE_SCENARIO_MIXED_EGRESS &&
            !create_target_listener(&plain_target_listener,
                &plain_target_port, &egress.plain_endpoint)) goto cleanup;
        if (policy.scenario != FIXTURE_SCENARIO_MIXED_EGRESS)
            plain_target_port = target_port;
        egress_policy.master_enabled = PAPACC_TRUE;
        egress_policy.explicitly_allowed_sensitive_classes =
            PAPACC_EGRESS_SENSITIVE_LOOPBACK;
        egress_policy.host_policy = allow_fixture_host;
        egress_policy.endpoint_policy = allow_fixture_endpoint;
        egress_policy.context = &egress;
        egress_profile.profile_id = 1U;
        memcpy(egress_profile.name, "browser-fixture", 16U);
        egress_profile.use_default_route = PAPACC_TRUE;
        egress_profile.available = PAPACC_TRUE;
        egress_config.allow_network_egress = PAPACC_TRUE;
        egress_config.policy = &egress_policy;
        egress_config.profiles = &egress_profile;
        egress_config.profile_count = 1U;
        egress_config.select_profile = select_fixture_profile;
        egress_config.select_profile_context = &egress;
        resolver.context = &egress;
        resolver.resolve = resolve_fixture;
        outbound_policy.runtime = composition.runtime;
        outbound_policy.trust = fixture.trust;
        outbound_policy.provider_id = "openssl";
        outbound_policy.alpn = (const PAPACC_U8 *)"papacc/1";
        outbound_policy.alpn_size = 8U;
        outbound_policy.alpn_required = PAPACC_TRUE;
        if (papacc_server_io_loop_win32_set_egress_config(&protocol_loop,
                &egress_config) != PAPACC_RESULT_OK ||
            papacc_server_io_loop_win32_set_dns_resolver(&protocol_loop,
                &resolver) != PAPACC_RESULT_OK ||
            papacc_server_io_loop_win32_set_outbound_tls(&protocol_loop,
                &outbound_policy) != PAPACC_RESULT_OK)
            goto cleanup;
        https_target.listener = target_listener;
        https_target.runtime = composition.runtime;
        https_target.credentials = fixture.server.credentials;
        https_target.stop_event = stop_event;
        https_target.expect_tls_failure = policy.scenario ==
            FIXTURE_SCENARIO_MODE_B_TLS_NAME_FAILURE ?
            PAPACC_TRUE : PAPACC_FALSE;
        https_target.use_tls = policy.scenario ==
            FIXTURE_SCENARIO_TCP_PLAIN_EGRESS ? PAPACC_FALSE : PAPACC_TRUE;
        https_target.require_alpn=PAPACC_TRUE;
        if (!configure_http_target(&https_target, policy.scenario))
            goto cleanup;
        target_thread = CreateThread(NULL, 0U, https_target_thread,
            &https_target, 0U, NULL);
        if (target_thread == NULL) goto cleanup;
        if (policy.scenario == FIXTURE_SCENARIO_MIXED_EGRESS) {
            plain_target.listener = plain_target_listener;
            plain_target.stop_event = stop_event;
            plain_target.use_tls = PAPACC_FALSE;
            plain_target_thread = CreateThread(NULL, 0U,
                https_target_thread, &plain_target, 0U, NULL);
            if (plain_target_thread == NULL) goto cleanup;
        }
    }
    listener.bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    listener.port = port;
    listener.transport_profile =
        PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL;
    if (papacc_security_configuration_ref_set(
            &listener.security_configuration_ref,
            (const PAPACC_U8 *)"browser-fixture", 15U) != PAPACC_RESULT_OK ||
        papacc_server_secure_io_win32_init(&secure_io, &network, &listener_set,
            &protocol_loop, &composition, &security, candidates,
            FIXTURE_CAPACITY, members, events, FIXTURE_CAPACITY + 1U +
                PAPACC_SERVER_NETWORK_STREAM_CAPACITY,
            listener_sources, listener_tokens, 1U, 5000000000ULL) !=
            PAPACC_RESULT_OK)
        goto cleanup;
    if (!publish_ready(argv[3], port, target_port, plain_target_port, argv[4],
            policy.scenario))
        goto cleanup;
    while (WaitForSingleObject(stop_event, 0U) == WAIT_TIMEOUT) {
        if (transport_fault_injected == PAPACC_TRUE &&
            papacc_server_secure_io_win32_is_stopped(&secure_io) == PAPACC_TRUE) {
            (void)WaitForSingleObject(stop_event, 25U);
            continue;
        }
        if (stall_engaged == PAPACC_TRUE) {
            (void)WaitForSingleObject(stop_event, 25U);
            continue;
        }
        if (papacc_server_secure_io_win32_poll_once(&secure_io, 25U) !=
                PAPACC_RESULT_OK)
            goto cleanup;
        if (policy.scenario ==
                FIXTURE_SCENARIO_SECURE_V2_TRANSPORT_FAILURE_NO_FALLBACK &&
            transport_fault_injected != PAPACC_TRUE &&
            authenticated_transport_ready_for_close(candidates,
                FIXTURE_CAPACITY) == PAPACC_TRUE &&
            papacc_server_secure_io_win32_request_stop(&secure_io) ==
                PAPACC_RESULT_OK) {
            transport_fault_injected = PAPACC_TRUE;
            puts("TRANSPORT_FAILURE_REALIZED=YES");
            fflush(stdout);
        }
        if (policy.scenario == FIXTURE_SCENARIO_STALL &&
            secure_authentication_completed(candidates, FIXTURE_CAPACITY) ==
                PAPACC_TRUE) {
            stall_engaged = PAPACC_TRUE;
            puts("FIXTURE_STALL_BARRIER=SECURE_AUTHENTICATED_BEFORE_CONTROL_PROGRESS");
            fflush(stdout);
        }
    }
    if (papacc_server_secure_io_win32_is_stopped(&secure_io) != PAPACC_TRUE &&
        papacc_server_secure_io_win32_request_stop(&secure_io) != PAPACC_RESULT_OK)
        goto cleanup;
    for (index = 0U; index < 400U &&
            papacc_server_secure_io_win32_is_stopped(&secure_io) !=
                PAPACC_TRUE; ++index) {
        if (papacc_server_secure_io_win32_poll_once(&secure_io, 25U) !=
                PAPACC_RESULT_OK)
            goto cleanup;
    }
    if (papacc_server_secure_io_win32_is_stopped(&secure_io) != PAPACC_TRUE)
        goto cleanup;
    /* Parent budget covers the child's bounded join plus cooperative recovery.
       This is not the fix for the wait: stop wakes the actual PST wait above. */
    result = (int)fixture_join_thread(&target_thread, 20000U, stop_event, NULL);
    if (result != 0) goto cleanup;
    result = (int)fixture_join_thread(&plain_target_thread, 20000U, stop_event, NULL);
    if (result != 0) goto cleanup;
    result = 1;
    if (policy.scenario == FIXTURE_SCENARIO_MODE_B_EGRESS &&
        https_target.response_count < 1U) goto cleanup;
    if (policy.scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS &&
        (https_target.accepted_count < 3U ||
         https_target.response_count < 3U))
        goto cleanup;
    if (policy.scenario == FIXTURE_SCENARIO_TCP_PLAIN_EGRESS) {
        printf("FIXTURE_TCP_PLAIN_TARGET_ACCEPT_COUNT=%zu\n",
            (size_t)https_target.accepted_count);
        printf("FIXTURE_TCP_PLAIN_TARGET_RESPONSE_COUNT=%zu\n",
            (size_t)https_target.response_count);
        puts("FIXTURE_SITE_TLS_PST_USED=NO");
        fflush(stdout);
    }
    if (policy.scenario == FIXTURE_SCENARIO_MIXED_EGRESS &&
        (https_target.accepted_count < 2U ||
         https_target.response_count < 2U ||
         plain_target.accepted_count < 1U ||
         plain_target.response_count < 1U)) goto cleanup;
    if (policy.scenario ==
            FIXTURE_SCENARIO_SECURE_V2_PROTOCOL_ERROR_NO_FALLBACK &&
        (log_context.protocol_fault_armed_count == 0U ||
         log_context.valid_v2_offer_decoded_count !=
            log_context.protocol_fault_armed_count ||
         log_context.protocol_fault_realized_count !=
            log_context.protocol_fault_armed_count))
        goto cleanup;
    if ((policy.scenario ==
            FIXTURE_SCENARIO_SECURE_V2_AUTH_FAILURE_NO_FALLBACK &&
         log_context.auth_control_published_count != 0U) ||
        (policy.scenario ==
            FIXTURE_SCENARIO_SECURE_V2_TRANSPORT_FAILURE_NO_FALLBACK &&
         log_context.transport_control_published_count != 0U))
        goto cleanup;
    result = 0;

cleanup:
    DeleteFileA(argv != NULL && argc > 3 ? argv[3] : "");
    if (stop_event != NULL) SetEvent(stop_event);
    {
        DWORD joined = fixture_join_thread(&target_thread, 20000U, stop_event, NULL);
        if (joined != 0U) result = (int)joined;
        joined = fixture_join_thread(&plain_target_thread, 20000U, stop_event, NULL);
        if (joined != 0U) result = (int)joined;
    }
    if (target_listener != INVALID_SOCKET) closesocket(target_listener);
    if (plain_target_listener != INVALID_SOCKET)
        closesocket(plain_target_listener);
    if (stop_event != NULL) CloseHandle(stop_event);
    papacc_server_secure_io_win32_shutdown(&secure_io);
    papacc_server_io_loop_win32_shutdown(&protocol_loop);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    papacc_tcp_socket_win32_close(&entry.socket);
    papacc_tcp_platform_shutdown(&network.tcp_platform);
    papacc_security_composition_release(&composition);
    papacc_test_security_fixture_release(&fixture);
    WSACleanup();
    return result;
}
