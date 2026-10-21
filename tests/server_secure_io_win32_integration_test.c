#include "server_secure_io_win32.h"
#include "security_test_support.h"
#include "pst_provider_bootstrap_win32.h"
#include "papinho_secure_transport_win32.h"
#include "egress_runtime_config.h"
#include "destination.h"
#include "pst_outbound_tls.h"
#include "pal_time.h"
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
#include "pst_test_only_socket_correlation_observer.h"
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
static int p10_fixture_bulk_drain=0;
#endif
#define CAPACITY 8U
#define CHECK(x, n) do { if (!(x)) { \
    fprintf(stderr, "check=%d line=%d\n", (n), __LINE__); \
    result = (n); goto cleanup; } } while (0)

static const PAPACC_U8 control_open[20] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,1,0,0,0,0,0,4,0,1,0,0 };
static const PAPACC_U8 control_accept[20] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,2,0,0,0,0,0,4,0,1,0,0 };
static const PAPACC_U8 data_ticket_request[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,3,0,0,0,0,0,0 };
static const PAPACC_U8 data_accept[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,6,0,0,0,0,0,0 };
static const PAPACC_U8 principal_bytes[16] = {
    0x50,0x41,0x50,0x41,0x43,0x43,0x2d,0x33,
    0x45,0x2d,0x50,0x2d,0,0,0,1 };

typedef struct TEST_POLICY {
    PAPACC_SIZE resolve_calls;
    PAPACC_SIZE authorize_calls;
    PAPACC_BOOL principal_a_known;
    PAPACC_U8 principal_a_sha256[PAPACC_CREDENTIAL_SHA256_SIZE];
    PAPACC_BOOL principal_b_known;
    PAPACC_U8 principal_b_sha256[PAPACC_CREDENTIAL_SHA256_SIZE];
    PAPACC_PRINCIPAL_RESOLUTION forced_resolution;
    PAPACC_AUTHORIZATION_DECISION forced_decision;
} TEST_POLICY;

typedef struct TEST_EGRESS {
    PAPACC_NETWORK_ENDPOINT endpoint;
    PAPACC_SIZE resolve_calls;
} TEST_EGRESS;

typedef enum TEST_OUTBOUND_MODE {
    TEST_OUTBOUND_VALID = 0,
    TEST_OUTBOUND_TLS12,
    TEST_OUTBOUND_WRONG_TRUST,
    TEST_OUTBOUND_WRONG_NAME,
    TEST_OUTBOUND_WRONG_ALPN,
    TEST_OUTBOUND_STALL,
    TEST_OUTBOUND_RESET
} TEST_OUTBOUND_MODE;

static PAPACC_EGRESS_DECISION allow_fixture_host(void *context,
    const char *host, PAPACC_SIZE size, PAPACC_U16 port)
{
    TEST_EGRESS *fixture = (TEST_EGRESS *)context;
    return ((size == 18U && memcmp(host, "papacc-test-server", 18U) == 0) ||
        (size == 13U && memcmp(host, "wrong.example", 13U) == 0)) &&
        port == fixture->endpoint.port ? PAPACC_EGRESS_ALLOW : PAPACC_EGRESS_DENY;
}

static PAPACC_EGRESS_DECISION allow_fixture_endpoint(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint, PAPACC_U32 sensitive_classes)
{
    TEST_EGRESS *fixture = (TEST_EGRESS *)context;
    (void)sensitive_classes;
    return papacc_network_endpoint_equal(&fixture->endpoint, endpoint) ==
        PAPACC_TRUE ? PAPACC_EGRESS_ALLOW : PAPACC_EGRESS_DENY;
}

static PAPACC_RESULT select_fixture_profile(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint, PAPACC_NETWORK_TRANSPORT_MODE mode,
    PAPACC_U32 *profile_id)
{
    TEST_EGRESS *fixture = (TEST_EGRESS *)context;
    if (profile_id == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *profile_id = 0U;
    if ((mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED &&
        mode != PAPACC_NETWORK_TRANSPORT_TCP_PLAIN) ||
        papacc_network_endpoint_equal(&fixture->endpoint, endpoint) !=
            PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    *profile_id = 1U;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT resolve_fixture(void *context, const char *host,
    PAPACC_SIZE size, PAPACC_U16 port, PAPACC_NETWORK_ENDPOINT *endpoints,
    PAPACC_SIZE capacity, PAPACC_SIZE *count)
{
    TEST_EGRESS *fixture = (TEST_EGRESS *)context;
    if (count == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *count = 0U;
    ++fixture->resolve_calls;
    if (host == NULL || endpoints == NULL || capacity == 0U ||
        !((size == 18U && memcmp(host, "papacc-test-server", 18U) == 0) ||
        (size == 13U && memcmp(host, "wrong.example", 13U) == 0)) ||
        port != fixture->endpoint.port) return PAPACC_RESULT_INVALID_ARGUMENT;
    endpoints[0] = fixture->endpoint;
    *count = 1U;
    return PAPACC_RESULT_OK;
}

static void outbound_server_config(PST_CONNECTION_CONFIG *configuration,
    pst_credentials *credentials, TEST_OUTBOUND_MODE mode)
{
    static const PST_ALPN_PROTOCOL alpn = {
        (const pst_u8 *)"papacc/1", 8U };
    static const PST_ALPN_PROTOCOL wrong_alpn = {
        (const pst_u8 *)"wrong/1", 7U };
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
        (mode == TEST_OUTBOUND_TLS12 ? PST_CAP_TLS_1_2 : PST_CAP_TLS_1_3) |
        PST_CAP_ROLE_SERVER | PST_CAP_LOCAL_IDENTITY |
        PST_CAP_ALPN_SERVER | PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT |
        PST_CAP_GRACEFUL_SHUTDOWN;
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
    configuration->tls.minimum_version = mode == TEST_OUTBOUND_TLS12 ?
        PST_TLS_VERSION_1_2 : PST_TLS_VERSION_1_3;
    configuration->tls.maximum_version = configuration->tls.minimum_version;
    configuration->tls.resumption = PST_FEATURE_DISABLED;
    configuration->tls.early_data = PST_FEATURE_DISABLED;
    configuration->tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    configuration->alpn.struct_size = sizeof(configuration->alpn);
    configuration->alpn.api_version = PST_API_VERSION;
    configuration->alpn.mode = PST_FEATURE_REQUIRED;
    configuration->alpn.protocols = mode == TEST_OUTBOUND_WRONG_ALPN ?
        &wrong_alpn : &alpn;
    configuration->alpn.protocol_count = 1U;
}

typedef struct REFERENCE_FIXTURE_THREAD {
    SOCKET listener;
    pst_runtime *runtime;
    pst_credentials *credentials;
    PAPACC_BOOL tls;
    PAPACC_BOOL truncate_after_write;
    PAPACC_BOOL reset_after_read;
    PAPACC_BOOL peer_close_after_read;
    PAPACC_BOOL handshake_completed;
    PAPACC_BOOL rst_injected;
    PAPACC_BOOL peer_close_started;
    PAPACC_BOOL abort_mode;
    PAPACC_BOOL transport_close_observed;
    PAPACC_BOOL tls_close_notify_observed;
    PAPACC_BOOL abort_wait_timed_out;
    pst_u32 abort_operation;
    PST_RESULT abort_source;
    PAPACC_SIZE response_size;
    PAPACC_SIZE expected_size;
    HANDLE hold_open_until_event;
    PAPACC_BOOL hold_open_wait_started;
    DWORD hold_open_wait_result;
    volatile LONG diagnostic_listener_ready;
    volatile LONG diagnostic_thread_started;
    volatile LONG diagnostic_accept_completed;
    volatile LONG diagnostic_handshake_completed;
    volatile LONG diagnostic_application_data_completed;
    volatile LONG diagnostic_shutdown_started;
    volatile LONG diagnostic_shutdown_operation;
    volatile LONG diagnostic_shutdown_result;
    volatile LONG diagnostic_shutdown_source;
    volatile LONG diagnostic_transport_terminal_observed;
    volatile LONG diagnostic_thread_exit;
    volatile LONG diagnostic_thread_exit_code;
    HANDLE p10_marker_received_event;
    HANDLE p10_owner_stop_event;
    volatile LONG p10_sequence;
    volatile LONG p10_marker_received;
    volatile LONG p10_marker_validated;
    volatile LONG p10_marker_receive_sequence;
    volatile LONG p10_marker_signal_attempts;
    volatile LONG p10_marker_signal_successes;
    volatile LONG p10_marker_signal_error;
    volatile LONG p10_marker_signal_sequence;
    volatile LONG p10_owner_stop_wait_result;
    volatile LONG p10_owner_stop_wait_error;
    volatile LONG p10_owner_stop_observed;
    volatile LONG p10_owner_stop_observed_sequence;
    PAPACC_U64 p10_post_ack_bytes;
    volatile LONG p10_post_ack_loop_iterations;
    volatile LONG p10_post_ack_read_result;
    volatile LONG p10_post_ack_read_operation;
    volatile LONG p10_post_ack_read_source;
    volatile LONG p10_post_ack_interest_result;
    volatile LONG p10_post_ack_interest_flags;
    volatile LONG p10_post_ack_wait_result;
    volatile LONG p10_post_ack_wait_ready_interest;
    volatile LONG p10_post_ack_wait_timed_out;
    PAPACC_U64 p10_post_ack_wait_elapsed_ns;
    PAPACC_U32 p10_post_ack_wait_call_count;
    PAPACC_U64 p10_post_ack_wait_total_elapsed_ns;
    PAPACC_U64 p10_post_ack_monotonic_start_ns;
    PAPACC_U64 p10_post_ack_monotonic_now_ns;
    PAPACC_U64 p10_post_ack_monotonic_deadline_ns;
    volatile LONG p10_post_ack_socket_select_result;
    volatile LONG p10_post_ack_socket_readiness;
    volatile LONG p10_post_ack_socket_error;
    volatile LONG p10_post_ack_exit_reason;
    PAPACC_U64 p10_connection_instance_id;
    PAPACC_U64 p10_stream_generation;
    char p10_run_id[64];
    int result;
} REFERENCE_FIXTURE_THREAD;

static int reference_fixture_initialization_selftest(void)
{
    REFERENCE_FIXTURE_THREAD fixture;
    memset(&fixture, 0, sizeof(fixture));
    return fixture.truncate_after_write == PAPACC_FALSE &&
        fixture.reset_after_read == PAPACC_FALSE &&
        fixture.peer_close_after_read == PAPACC_FALSE &&
        fixture.abort_mode == PAPACC_FALSE &&
        fixture.hold_open_until_event == NULL &&
        fixture.result == 0;
}

static DWORD WINAPI reference_fixture_thread(void *value)
{
    REFERENCE_FIXTURE_THREAD *fixture = (REFERENCE_FIXTURE_THREAD *)value;
    PAPACC_U8 bytes[32768];
    PAPACC_SIZE offset = 0U;
    SOCKET accepted = INVALID_SOCKET;
    u_long blocking = 0UL;
    PAPACC_SIZE index;
    InterlockedExchange(&fixture->diagnostic_thread_started, 1L);
    fixture->result = 0;
    for (index = 0U; index < 5000U && accepted == INVALID_SOCKET; ++index) {
        accepted = accept(fixture->listener, NULL, NULL);
        if (accepted == INVALID_SOCKET) Sleep(1U);
    }
    if (accepted == INVALID_SOCKET) {
        InterlockedExchange(&fixture->diagnostic_thread_exit_code, 1L);
        InterlockedExchange(&fixture->diagnostic_thread_exit, 1L);
        return 1U;
    }
    InterlockedExchange(&fixture->diagnostic_accept_completed, 1L);
    if (fixture->tls != PAPACC_TRUE) {
        if (ioctlsocket(accepted, FIONBIO, &blocking) == SOCKET_ERROR) {
            closesocket(accepted); return 2U;
        }
        if (fixture->expected_size == 0U) {
            char byte;
            while (recv(accepted, &byte, 1, 0) > 0) { }
            closesocket(accepted);
            fixture->result = 1;
            return 0U;
        }
        while (offset < fixture->expected_size) {
            int amount = recv(accepted, (char *)bytes + offset,
                (int)(fixture->expected_size - offset), 0);
            if (amount <= 0) { closesocket(accepted); return 3U; }
            offset += (PAPACC_SIZE)amount;
        }
        offset = 0U;
        while (offset < fixture->expected_size) {
            int amount = send(accepted, (const char *)bytes + offset,
                (int)(fixture->expected_size - offset), 0);
            if (amount <= 0) { closesocket(accepted); return 4U; }
            offset += (PAPACC_SIZE)amount;
        }
        shutdown(accepted, SD_SEND);
        closesocket(accepted);
        fixture->result = 1;
        return 0U;
    } else {
        pst_transport *transport = NULL;
        pst_connection *connection = NULL;
        PST_CONNECTION_CONFIG configuration;
        pst_u32 ownership = 0U, operation = 0U;
        SOCKET transport_socket_alias = accepted;
        struct linger abortive = { 1, 0 };
        PST_RESULT source = PST_RESULT_OK;
        outbound_server_config(&configuration, fixture->credentials,
            TEST_OUTBOUND_VALID);
        blocking = 1UL;
        if (ioctlsocket(accepted, FIONBIO, &blocking) == SOCKET_ERROR ||
            pst_connection_create(fixture->runtime, &configuration,
                &connection) != PST_RESULT_OK ||
            pst_win32_socket_transport_create((pst_size)accepted,
                &transport) != PST_RESULT_OK ||
            pst_connection_attach(connection, transport,
                PST_OWNERSHIP_TRANSFERRED, &ownership) != PST_RESULT_OK ||
            ownership == 0U) goto tls_cleanup;
        accepted = INVALID_SOCKET;
        transport = NULL;
        for (index = 0U; index < 5000U; ++index) {
            PST_WAIT_RESULT wait_result;
            if (pst_connection_handshake(connection, &operation, &source) !=
                    PST_RESULT_OK || operation == PST_OPERATION_FAILED ||
                    operation == PST_OPERATION_CLOSED) goto tls_cleanup;
            if (operation == PST_OPERATION_COMPLETE) break;
            if (pst_connection_wait(connection, 1000U, &wait_result) !=
                    PST_RESULT_OK) goto tls_cleanup;
        }
        if (operation != PST_OPERATION_COMPLETE) goto tls_cleanup;
        fixture->handshake_completed = PAPACC_TRUE;
        InterlockedExchange(&fixture->diagnostic_handshake_completed, 1L);
        if (fixture->reset_after_read == PAPACC_TRUE) {
            PAPACC_U8 marker = 0U; PAPACC_SIZE marker_size = 0U;
            for (index = 0U; index < 5000U && marker_size == 0U; ++index) {
                PST_IO_RESULT io; PST_WAIT_RESULT wait_result;
                if (pst_connection_read(connection, &marker, 1U, &io) !=
                        PST_RESULT_OK || io.operation == PST_OPERATION_FAILED ||
                        io.operation == PST_OPERATION_CLOSED) goto tls_cleanup;
                marker_size += io.bytes_transferred;
                if (marker_size == 0U && pst_connection_wait(connection, 1000U,
                        &wait_result) != PST_RESULT_OK) goto tls_cleanup;
            }
            if (marker_size != 1U ||
                setsockopt(transport_socket_alias, SOL_SOCKET, SO_LINGER,
                    (const char *)&abortive, (int)sizeof(abortive)) != 0 ||
                closesocket(transport_socket_alias) != 0) goto tls_cleanup;
            fixture->rst_injected = PAPACC_TRUE; fixture->result = 1;
            puts("TLS_REQUIRED_PATH=YES PST_CONNECTION_ESTABLISHED=YES");
            puts("TCP_RST_INJECTED=YES RST_AFTER_TLS_APPLICATION_BYTE=YES");
            goto tls_cleanup;
        }
        if (fixture->peer_close_after_read == PAPACC_TRUE) {
            PAPACC_U8 marker = 0U; PAPACC_SIZE marker_size = 0U;
            PST_RESULT shutdown_result;
            for (index = 0U; index < 5000U && marker_size == 0U; ++index) {
                PST_IO_RESULT io; PST_WAIT_RESULT wait_result;
                if (pst_connection_read(connection, &marker, 1U, &io) != PST_RESULT_OK ||
                    io.operation == PST_OPERATION_FAILED || io.operation == PST_OPERATION_CLOSED) goto tls_cleanup;
                marker_size += io.bytes_transferred;
                if (marker_size == 0U && pst_connection_wait(connection, 1000U, &wait_result) != PST_RESULT_OK) goto tls_cleanup;
            }
            if (marker_size != 1U) goto tls_cleanup;
            InterlockedExchange(
                &fixture->diagnostic_application_data_completed, 1L);
            if(fixture->p10_marker_received_event!=NULL){
                InterlockedExchange(&fixture->p10_marker_received,1L);
                InterlockedExchange(&fixture->p10_marker_receive_sequence,
                    InterlockedIncrement(&fixture->p10_sequence));
                if(marker!=0x44U)goto tls_cleanup;
                InterlockedExchange(&fixture->p10_marker_validated,1L);
                InterlockedIncrement(&fixture->p10_marker_signal_attempts);
                if(SetEvent(fixture->p10_marker_received_event)==0){
                    InterlockedExchange(&fixture->p10_marker_signal_error,
                        (LONG)GetLastError());
                    goto tls_cleanup;
                }
                InterlockedIncrement(&fixture->p10_marker_signal_successes);
                InterlockedExchange(&fixture->p10_marker_signal_sequence,
                    InterlockedIncrement(&fixture->p10_sequence));
                if(fixture->p10_owner_stop_event==NULL)goto tls_cleanup;
                fixture->p10_owner_stop_wait_result=(LONG)WaitForSingleObject(
                    fixture->p10_owner_stop_event,5000U);
                if((DWORD)fixture->p10_owner_stop_wait_result==WAIT_FAILED)
                    fixture->p10_owner_stop_wait_error=(LONG)GetLastError();
                if((DWORD)fixture->p10_owner_stop_wait_result!=WAIT_OBJECT_0)
                    goto tls_cleanup;
                InterlockedExchange(&fixture->p10_owner_stop_observed,1L);
                InterlockedExchange(
                    &fixture->p10_owner_stop_observed_sequence,
                    InterlockedIncrement(&fixture->p10_sequence));
                (void)papacc_pal_monotonic_time_ns(
                    &fixture->p10_post_ack_monotonic_start_ns);
                for(index=0U;index<
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                        (p10_fixture_bulk_drain?1024U:100U)
#else
                        100U
#endif
                        ;++index){
                    PAPACC_U8 drain_buffer[16384];PAPACC_SIZE drain_capacity=1U;
                    PST_IO_RESULT io;
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                    if(p10_fixture_bulk_drain)drain_capacity=sizeof(drain_buffer);
#endif
                    PST_WAIT_RESULT wait_result;
                    PST_RESULT read_result,interest_result,wait_call_result;
                    pst_u32 interest=PST_INTEREST_NONE;
                    PAPACC_U64 wait_start_ns=0U,wait_end_ns=0U;
                    fd_set read_set,error_set;
                    struct timeval zero_timeout={0,0};
                    int select_result,socket_error=0;
                    int socket_error_size=(int)sizeof(socket_error);
                    memset(&io,0,sizeof(io));
                    memset(&wait_result,0,sizeof(wait_result));
                    InterlockedExchange(&fixture->p10_post_ack_loop_iterations,
                        (LONG)(index+1U));
                    read_result=pst_connection_read(connection,drain_buffer,drain_capacity,&io);
                    fixture->p10_post_ack_bytes+=io.bytes_transferred;
                    InterlockedExchange(&fixture->p10_post_ack_read_result,
                        (LONG)read_result);
                    InterlockedExchange(&fixture->p10_post_ack_read_operation,
                        (LONG)io.operation);
                    InterlockedExchange(&fixture->p10_post_ack_read_source,
                        (LONG)io.error);
                    InterlockedExchange(&fixture->diagnostic_shutdown_operation,
                        (LONG)io.operation);
                    InterlockedExchange(&fixture->diagnostic_shutdown_source,
                        (LONG)io.error);
                    if(read_result!=PST_RESULT_OK){
                        InterlockedExchange(&fixture->p10_post_ack_exit_reason,1L);
                        goto tls_cleanup;
                    }
                    interest_result=pst_connection_get_interest(connection,
                        &interest);
                    InterlockedExchange(&fixture->p10_post_ack_interest_result,
                        (LONG)interest_result);
                    InterlockedExchange(&fixture->p10_post_ack_interest_flags,
                        (LONG)interest);
                    if(io.operation==PST_OPERATION_CLOSED||
                       io.operation==PST_OPERATION_FAILED){
                        InterlockedExchange(
                            &fixture->diagnostic_transport_terminal_observed,
                            1L);
                        InterlockedExchange(&fixture->p10_post_ack_exit_reason,2L);
                        fixture->result=1;
                        goto tls_cleanup;
                    }
                    (void)papacc_pal_monotonic_time_ns(&wait_start_ns);
                    wait_call_result=pst_connection_wait(connection,100U,
                        &wait_result);
                    (void)papacc_pal_monotonic_time_ns(&wait_end_ns);
                    fixture->p10_post_ack_wait_elapsed_ns=
                        wait_end_ns>=wait_start_ns?
                        wait_end_ns-wait_start_ns:0U;
                    ++fixture->p10_post_ack_wait_call_count;
                    fixture->p10_post_ack_wait_total_elapsed_ns+=
                        fixture->p10_post_ack_wait_elapsed_ns;
                    fixture->p10_post_ack_monotonic_now_ns=wait_end_ns;
                    InterlockedExchange(&fixture->p10_post_ack_wait_result,
                        (LONG)wait_call_result);
                    InterlockedExchange(
                        &fixture->p10_post_ack_wait_ready_interest,
                        (LONG)wait_result.ready_interest);
                    InterlockedExchange(&fixture->p10_post_ack_wait_timed_out,
                        (LONG)wait_result.timed_out);
                    FD_ZERO(&read_set);FD_ZERO(&error_set);
                    FD_SET(transport_socket_alias,&read_set);
                    FD_SET(transport_socket_alias,&error_set);
                    select_result=select(0,&read_set,NULL,&error_set,
                        &zero_timeout);
                    InterlockedExchange(
                        &fixture->p10_post_ack_socket_select_result,
                        (LONG)select_result);
                    InterlockedExchange(&fixture->p10_post_ack_socket_readiness,
                        select_result>0&&FD_ISSET(transport_socket_alias,
                            &read_set)?1L:0L);
                    if(getsockopt(transport_socket_alias,SOL_SOCKET,SO_ERROR,
                            (char *)&socket_error,&socket_error_size)==0)
                        InterlockedExchange(
                            &fixture->p10_post_ack_socket_error,
                            (LONG)socket_error);
                    if(wait_call_result!=PST_RESULT_OK){
                        InterlockedExchange(&fixture->p10_post_ack_exit_reason,3L);
                        goto tls_cleanup;
                    }
                }
                InterlockedExchange(&fixture->p10_post_ack_exit_reason,4L);
                goto tls_cleanup;
            }
            InterlockedExchange(&fixture->diagnostic_shutdown_started, 1L);
            shutdown_result = pst_connection_shutdown(connection, &operation,
                &source);
            InterlockedExchange(&fixture->diagnostic_shutdown_result,
                (LONG)shutdown_result);
            InterlockedExchange(&fixture->diagnostic_shutdown_operation,
                (LONG)operation);
            InterlockedExchange(&fixture->diagnostic_shutdown_source,
                (LONG)source);
            if (shutdown_result != PST_RESULT_OK ||
                operation != PST_OPERATION_NEED_READ) goto tls_cleanup;
            fixture->peer_close_started = PAPACC_TRUE; fixture->result = 1;
            puts("D_PEER_CLOSE_NOTIFY=YES D_FIXTURE_SHUTDOWN_NEED_READ=YES");
            for (index = 0U; index < 100U; ++index) {
                PST_WAIT_RESULT wait_result;
                if (pst_connection_wait(connection, 100U, &wait_result) != PST_RESULT_OK) break;
                shutdown_result = pst_connection_shutdown(connection,
                    &operation, &source);
                InterlockedExchange(&fixture->diagnostic_shutdown_result,
                    (LONG)shutdown_result);
                InterlockedExchange(&fixture->diagnostic_shutdown_operation,
                    (LONG)operation);
                InterlockedExchange(&fixture->diagnostic_shutdown_source,
                    (LONG)source);
                if (shutdown_result != PST_RESULT_OK ||
                    operation == PST_OPERATION_FAILED ||
                    operation == PST_OPERATION_CLOSED ||
                    operation == PST_OPERATION_COMPLETE) {
                    if (operation == PST_OPERATION_CLOSED ||
                        operation == PST_OPERATION_COMPLETE)
                        InterlockedExchange(
                            &fixture->diagnostic_transport_terminal_observed,
                            1L);
                    break;
                }
            }
            goto tls_cleanup;
        }
        if(fixture->truncate_after_write==PAPACC_TRUE){
            for(index=0U;index<fixture->response_size;++index)
                bytes[index]=(PAPACC_U8)(index*37U+11U);
            offset=0U;
            while(offset<fixture->response_size){
                PST_IO_RESULT io;
                PST_WAIT_RESULT wait_result;
                if(pst_connection_write(connection,bytes+offset,
                        fixture->response_size-offset,&io)!=PST_RESULT_OK||
                    io.operation==PST_OPERATION_FAILED||
                    io.operation==PST_OPERATION_CLOSED)goto tls_cleanup;
                offset+=io.bytes_transferred;
                if(offset<fixture->response_size&&
                    pst_connection_wait(connection,1000U,&wait_result)!=
                        PST_RESULT_OK)goto tls_cleanup;
            }
            fixture->result=1;
            goto tls_cleanup;
        }
        if (fixture->expected_size == 0U) {
            Sleep(500U);
            fixture->result = 1;
            goto tls_cleanup;
        }
        offset = 0U;
        while (offset < sizeof(bytes)) {
            PST_IO_RESULT io;
            PST_WAIT_RESULT wait_result;
            if (pst_connection_read(connection, bytes + offset,
                    sizeof(bytes) - offset, &io) != PST_RESULT_OK ||
                io.operation == PST_OPERATION_FAILED ||
                io.operation == PST_OPERATION_CLOSED) goto tls_cleanup;
            offset += io.bytes_transferred;
            if (offset < sizeof(bytes) && pst_connection_wait(connection,
                    1000U, &wait_result) != PST_RESULT_OK) goto tls_cleanup;
        }
        offset = 0U;
        while (offset < sizeof(bytes)) {
            PST_IO_RESULT io;
            PST_WAIT_RESULT wait_result;
            if (pst_connection_write(connection, bytes + offset,
                    sizeof(bytes) - offset, &io) != PST_RESULT_OK ||
                io.operation == PST_OPERATION_FAILED ||
                io.operation == PST_OPERATION_CLOSED) goto tls_cleanup;
            offset += io.bytes_transferred;
            if (offset < sizeof(bytes) && pst_connection_wait(connection,
                    1000U, &wait_result) != PST_RESULT_OK) goto tls_cleanup;
        }
        if (fixture->hold_open_until_event != NULL) {
            fixture->hold_open_wait_started = PAPACC_TRUE;
            fixture->hold_open_wait_result = WaitForSingleObject(
                fixture->hold_open_until_event, 10000U);
            if (fixture->hold_open_wait_result != WAIT_OBJECT_0)
                goto tls_cleanup;
        }
        if(fixture->abort_mode==PAPACC_TRUE){
            PAPACC_U64 start_ns=0U,now_ns=0U;
            PAPACC_U8 ignored_byte=0U;
            if(papacc_pal_monotonic_time_ns(&start_ns)!=PAPACC_RESULT_OK)
                goto tls_cleanup;
            for(;;){
                PST_IO_RESULT io;
                PST_WAIT_RESULT wait_result;
                memset(&io,0,sizeof(io));
                if(pst_connection_read(connection,&ignored_byte,1U,&io)!=
                        PST_RESULT_OK)goto tls_cleanup;
                fixture->abort_operation=io.operation;
                fixture->abort_source=io.error;
                if(io.operation==PST_OPERATION_CLOSED||
                   io.operation==PST_OPERATION_FAILED){
                    fixture->transport_close_observed=PAPACC_TRUE;
                    fixture->tls_close_notify_observed=
                        io.close_kind==PST_CLOSE_CLEAN?PAPACC_TRUE:PAPACC_FALSE;
                    fixture->result=1;
                    goto tls_cleanup;
                }
                if(papacc_pal_monotonic_time_ns(&now_ns)!=PAPACC_RESULT_OK)
                    goto tls_cleanup;
                if(now_ns-start_ns>=5000000000ULL){
                    fixture->abort_wait_timed_out=PAPACC_TRUE;
                    goto tls_cleanup;
                }
                if(pst_connection_wait(connection,100U,&wait_result)!=
                        PST_RESULT_OK)goto tls_cleanup;
            }
        }
        fixture->result = 1;
        operation = PST_OPERATION_NEED_READ;
        for (index = 0U; index < 5000U; ++index) {
            PST_WAIT_RESULT wait_result;
            if (pst_connection_shutdown(connection, &operation, &source) !=
                    PST_RESULT_OK || operation == PST_OPERATION_FAILED)
                break;
            if (operation == PST_OPERATION_COMPLETE ||
                operation == PST_OPERATION_CLOSED) break;
            if (pst_connection_wait(connection, 1000U, &wait_result) !=
                    PST_RESULT_OK) break;
        }
tls_cleanup:
        if (connection != NULL) pst_connection_release(connection);
        if (transport != NULL) pst_transport_release(transport);
        if (accepted != INVALID_SOCKET) closesocket(accepted);
        InterlockedExchange(&fixture->diagnostic_thread_exit_code,
            fixture->result ? 0L : 5L);
        InterlockedExchange(&fixture->diagnostic_thread_exit, 1L);
        return fixture->result ? 0U : 5U;
    }
}

static void discard_log(void *context, const PAPACC_LOG_RECORD *record)
{
    (void)context;
    (void)record;
}

static PAPACC_RESULT resolve_principal(void *context,
    const PAPACC_PEER_EVIDENCE *evidence,
    PAPACC_PRINCIPAL_RESOLUTION *resolution, PAPACC_PRINCIPAL *principal)
{
    TEST_POLICY *policy = (TEST_POLICY *)context;
    ++policy->resolve_calls;
    if (evidence == NULL || evidence->certificate_present != PAPACC_TRUE ||
        evidence->chain_validated != PAPACC_TRUE ||
        evidence->peer_authenticated != PAPACC_TRUE ||
        evidence->certificate_sha256_valid != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (policy->forced_resolution != PAPACC_PRINCIPAL_RESOLVED) {
        *resolution = policy->forced_resolution;
        return PAPACC_RESULT_OK;
    }
    *resolution = PAPACC_PRINCIPAL_RESOLVED;
    principal->valid = PAPACC_TRUE;
    memcpy(principal->value, principal_bytes, sizeof(principal_bytes));
    if (policy->principal_a_known != PAPACC_TRUE) {
        memcpy(policy->principal_a_sha256, evidence->certificate_sha256,
            sizeof(policy->principal_a_sha256));
        policy->principal_a_known = PAPACC_TRUE;
    } else if (memcmp(policy->principal_a_sha256,
            evidence->certificate_sha256,
            sizeof(policy->principal_a_sha256)) != 0) {
        if (policy->principal_b_known != PAPACC_TRUE) {
            memcpy(policy->principal_b_sha256, evidence->certificate_sha256,
                sizeof(policy->principal_b_sha256));
            policy->principal_b_known = PAPACC_TRUE;
        } else if (memcmp(policy->principal_b_sha256,
                evidence->certificate_sha256,
                sizeof(policy->principal_b_sha256)) != 0) {
            *resolution = PAPACC_PRINCIPAL_NOT_ENROLLED;
            principal->valid = PAPACC_FALSE;
            return PAPACC_RESULT_OK;
        }
        principal->value[sizeof(principal->value) - 1U] = 2U;
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT authorize(void *context,
    const PAPACC_PRINCIPAL *principal, PAPACC_AUTHORIZATION_ACTION action,
    PAPACC_AUTHORIZATION_DECISION *decision)
{
    TEST_POLICY *policy = (TEST_POLICY *)context;
    (void)action;
    ++policy->authorize_calls;
    if (principal == NULL || principal->valid != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *decision = policy->forced_decision;
    if (action == PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION &&
        principal->value[sizeof(principal->value) - 1U] == 2U)
        *decision = PAPACC_AUTHORIZATION_DENY;
    return PAPACC_RESULT_OK;
}

static SOCKET connect_loopback(PAPACC_U16 port)
{
    struct sockaddr_in address;
    u_long nonblocking = 1UL;
    SOCKET socket_value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_value == INVALID_SOCKET) return INVALID_SOCKET;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(socket_value, (const struct sockaddr *)&address,
            sizeof(address)) == SOCKET_ERROR ||
        ioctlsocket(socket_value, FIONBIO, &nonblocking) == SOCKET_ERROR) {
        closesocket(socket_value);
        return INVALID_SOCKET;
    }
    return socket_value;
}

static int establish_client(PAPACC_U16 port,
    PAPACC_SECURITY_COMPOSITION *composition,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, pst_credentials *credentials,
    pst_trust *trust, PAPACC_PST_SECURE_PROCESSOR *client)
{
    SOCKET socket_value = INVALID_SOCKET;
    pst_transport *transport = NULL;
    PST_CONNECTION_CONFIG configuration;
    PAPACC_BOOL ownership = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP step = { 0 };
    PAPACC_SIZE index = 0U;
    PAPACC_SIZE diagnostic_index;
    PAPACC_RESULT processor_result;
    PST_RESULT transport_result;
    PAPACC_RESULT attach_result;
    PAPACC_RESULT handshake_result = PAPACC_RESULT_OK;
    PAPACC_RESULT poll_result = PAPACC_RESULT_OK;
    socket_value = connect_loopback(port);
    if (socket_value == INVALID_SOCKET) {
        fprintf(stderr, "CHECK101_ESTABLISH stage=CONNECT error=%lu\n",
            (unsigned long)WSAGetLastError());
        return 0;
    }
    papacc_test_pst_client_config(&configuration, credentials, trust);
    processor_result = papacc_pst_secure_processor_init(client,
        composition->runtime, &configuration, ~(PAPACC_U64)0);
    if (processor_result != PAPACC_RESULT_OK) {
        fprintf(stderr, "CHECK101_ESTABLISH stage=PROCESSOR_INIT result=%d\n",
            (int)processor_result);
        closesocket(socket_value);
        return 0;
    }
    transport_result = pst_win32_socket_transport_create(
        (pst_size)socket_value, &transport);
    if (transport_result != PST_RESULT_OK) {
        fprintf(stderr, "CHECK101_ESTABLISH stage=TRANSPORT_CREATE result=%d\n",
            (int)transport_result);
        closesocket(socket_value);
        return 0;
    }
    attach_result = papacc_pst_secure_processor_attach(client, transport,
        &ownership);
    if (attach_result != PAPACC_RESULT_OK || ownership != PAPACC_TRUE) {
        fprintf(stderr,
            "CHECK101_ESTABLISH stage=ATTACH result=%d ownership=%d\n",
            (int)attach_result, (int)ownership);
        if (transport != NULL) pst_transport_release(transport);
        else closesocket(socket_value);
        return 0;
    }
    transport = NULL;
    socket_value = INVALID_SOCKET;
    for (index = 0U; index < 500U && client->state !=
            PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED; ++index) {
        handshake_result = papacc_pst_secure_processor_handshake_once(client,
            &step);
        if (handshake_result != PAPACC_RESULT_OK) {
            fprintf(stderr,
                "CHECK101_ESTABLISH stage=HANDSHAKE iteration=%lu result=%d state=%d step=%d\n",
                (unsigned long)index, (int)handshake_result,
                (int)client->state, (int)step);
            return 0;
        }
        poll_result = papacc_server_secure_io_win32_poll_once(secure_io, 10U);
        if (poll_result != PAPACC_RESULT_OK) {
            fprintf(stderr,
                "CHECK101_ESTABLISH stage=SERVER_POLL iteration=%lu result=%d state=%d step=%d stop_state=%d scheduler_members=%lu\n",
                (unsigned long)index, (int)poll_result, (int)client->state,
                (int)step, (int)secure_io->stop_state,
                (unsigned long)secure_io->scheduler.member_count);
            fprintf(stderr, "CHECK101_CANDIDATES capacity=%lu states=",
                (unsigned long)secure_io->candidate_capacity);
            for (diagnostic_index = 0U; diagnostic_index <
                    secure_io->candidate_capacity; ++diagnostic_index)
                fprintf(stderr, "%s%d", diagnostic_index == 0U ? "" : ",",
                    (int)secure_io->candidates[diagnostic_index].state);
            fprintf(stderr, "\n");
            return 0;
        }
    }
    if (client->state != PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED)
        fprintf(stderr,
            "CHECK101_ESTABLISH stage=LOOP_EXHAUSTED iterations=%lu state=%d step=%d stop_state=%d scheduler_members=%lu\n",
            (unsigned long)index, (int)client->state, (int)step,
            (int)secure_io->stop_state,
            (unsigned long)secure_io->scheduler.member_count);
    return client->state == PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED;
}

static int write_all(PAPACC_PST_SECURE_PROCESSOR *client,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, const PAPACC_U8 *bytes,
    PAPACC_SIZE size, PAPACC_SIZE fragment_size)
{
    PAPACC_PST_SECURE_STEP step;
    PAPACC_SIZE offset = 0U;
    PAPACC_SIZE transferred;
    PAPACC_SIZE requested;
    PAPACC_SIZE index;
    for (index = 0U; index < 1000U && offset < size; ++index) {
        requested = size - offset;
        if (fragment_size != 0U && requested > fragment_size)
            requested = fragment_size;
        if (papacc_pst_secure_processor_write_once(client, bytes + offset,
                requested, &transferred, &step) != PAPACC_RESULT_OK ||
            papacc_server_secure_io_win32_poll_once(secure_io, 10U) !=
                PAPACC_RESULT_OK) return 0;
        offset += transferred;
    }
    return offset == size;
}

static int read_exact(PAPACC_PST_SECURE_PROCESSOR *client,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U8 *bytes,
    PAPACC_SIZE size)
{
    PAPACC_PST_SECURE_STEP step;
    PAPACC_SIZE offset = 0U;
    PAPACC_SIZE transferred;
    PAPACC_SIZE index;
    for (index = 0U; index < 1000U && offset < size; ++index) {
        if (papacc_server_secure_io_win32_poll_once(secure_io, 10U) !=
                PAPACC_RESULT_OK ||
            papacc_pst_secure_processor_read_once(client, bytes + offset,
                size - offset, &transferred, &step) != PAPACC_RESULT_OK)
            return 0;
        offset += transferred;
    }
    return offset == size;
}

static int read_network_payload(PAPACC_PST_SECURE_PROCESSOR *client,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U8 *bytes,
    PAPACC_SIZE size)
{
    PAPACC_SIZE offset = 0U;
    while (offset < size) {
        PAPACC_U8 header[16];
        PAPACC_SIZE payload_size;
        if (!read_exact(client, secure_io, header, sizeof(header)) ||
            header[8] != 0U || header[9] != PAPACC_MESSAGE_TYPE_NETWORK_DATA)
            return 0;
        payload_size = ((PAPACC_SIZE)header[12] << 24) |
            ((PAPACC_SIZE)header[13] << 16) |
            ((PAPACC_SIZE)header[14] << 8) | header[15];
        if (payload_size == 0U || payload_size > size - offset ||
            !read_exact(client, secure_io, bytes + offset, payload_size))
            return 0;
        offset += payload_size;
    }
    return 1;
}

static int productive_tls_scenario(TEST_OUTBOUND_MODE mode,
    PAPACC_U16 server_port, PAPACC_SECURITY_COMPOSITION *composition,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,
    PAPACC_PST_SECURE_PROCESSOR *control, pst_credentials *client_credentials,
    pst_trust *client_trust, pst_credentials *fixture_credentials,
    pst_trust *wrong_trust, SOCKET fixture_listener,
    TEST_EGRESS *egress_fixture, PAPACC_OUTBOUND_TLS_POLICY *outbound_policy)
{
    PAPACC_PST_SECURE_PROCESSOR data =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_U8 ticket[32];
    PAPACC_U8 attach[32] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
    PAPACC_U8 accepted_frame[16];
    PAPACC_U8 open_frame[64] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,9,0,0,0,0,0,0 };
    PAPACC_U8 open_result[17];
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE payload_size = 0U;
    PAPACC_SIZE baseline_streams =
        secure_io->protocol_loop->network_stream_manager.count;
    PAPACC_SIZE baseline_connections =
        secure_io->protocol_loop->connection_manager->count;
    PAPACC_SIZE index = 0U;
    PAPACC_BOOL saw_job = PAPACC_FALSE;
    SOCKET fixture_socket = INVALID_SOCKET;
    pst_transport *transport = NULL;
    pst_connection *fixture_connection = NULL;
    PST_CONNECTION_CONFIG fixture_configuration;
    pst_u32 ownership = 0U;
    pst_u32 operation = PST_OPERATION_NEED_READ;
    PST_RESULT source = PST_RESULT_OK;
    u_long nonblocking = 1UL;
    struct linger abortive = { 1, 0 };
    int success = 0;
    int diagnostic_stage = 0;
    PAPACC_RESULT diagnostic_poll_result = PAPACC_RESULT_OK;
    const char *hostname = mode == TEST_OUTBOUND_WRONG_NAME ?
        "wrong.example" : "papacc-test-server";
    PAPACC_SIZE hostname_size = mode == TEST_OUTBOUND_WRONG_NAME ? 13U : 18U;

    diagnostic_stage = 1;
    outbound_policy->trust = mode == TEST_OUTBOUND_WRONG_TRUST ?
        wrong_trust : client_trust;
    if (papacc_server_io_loop_win32_set_outbound_tls(secure_io->protocol_loop,
            outbound_policy) != PAPACC_RESULT_OK ||
        !write_all(control, secure_io, data_ticket_request,
            sizeof(data_ticket_request), 3U) ||
        !read_exact(control, secure_io, ticket, sizeof(ticket))) goto cleanup;
    diagnostic_stage = 20;
    memcpy(attach + 16U, ticket + 16U, 16U);
    if (!establish_client(server_port, composition, secure_io,
            client_credentials, client_trust, &data)) goto cleanup;
    diagnostic_stage = 21;
    if (!write_all(&data, secure_io, attach, sizeof(attach), 3U))
        goto cleanup;
    diagnostic_stage = 22;
    if (!read_exact(&data, secure_io, accepted_frame,
            sizeof(accepted_frame))) goto cleanup;
    diagnostic_stage = 23;
    if (memcmp(accepted_frame, data_accept, sizeof(data_accept)) != 0)
        goto cleanup;

    diagnostic_stage = 3;
    request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode = PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port = egress_fixture->endpoint.port;
    request.destination_size = (PAPACC_U16)hostname_size;
    memcpy(request.destination, hostname, hostname_size);
    request.alpn_count = 1U;
    request.alpn[0].size = 8U;
    memcpy(request.alpn[0].bytes, "papacc/1", 8U);
    if (papacc_network_open_encode(&request, open_frame + 16U,
            sizeof(open_frame) - 16U, &payload_size) != PAPACC_RESULT_OK)
        goto cleanup;
    open_frame[12] = (PAPACC_U8)(payload_size >> 24);
    open_frame[13] = (PAPACC_U8)(payload_size >> 16);
    open_frame[14] = (PAPACC_U8)(payload_size >> 8);
    open_frame[15] = (PAPACC_U8)payload_size;
    diagnostic_stage = 4;
    if (!write_all(&data, secure_io, open_frame, 16U + payload_size, 5U))
        goto cleanup;
    outbound_server_config(&fixture_configuration, fixture_credentials, mode);

    diagnostic_stage = 5;
    for (index = 0U; index < 1600U; ++index) {
        diagnostic_poll_result = papacc_server_secure_io_win32_poll_once(
            secure_io, 5U);
        if (diagnostic_poll_result != PAPACC_RESULT_OK) goto cleanup;
        if (secure_io->protocol_loop->network_job_manager.count != 0U)
            saw_job = PAPACC_TRUE;
        if (fixture_socket == INVALID_SOCKET && fixture_connection == NULL) {
            fixture_socket = accept(fixture_listener, NULL, NULL);
            if (fixture_socket != INVALID_SOCKET) {
                if (mode == TEST_OUTBOUND_RESET) {
                    (void)setsockopt(fixture_socket, SOL_SOCKET, SO_LINGER,
                        (const char *)&abortive, (int)sizeof(abortive));
                    (void)closesocket(fixture_socket);
                    fixture_socket = INVALID_SOCKET;
                } else if (mode != TEST_OUTBOUND_STALL) {
                    if (ioctlsocket(fixture_socket, FIONBIO, &nonblocking) ==
                            SOCKET_ERROR ||
                        pst_connection_create(composition->runtime,
                            &fixture_configuration, &fixture_connection) !=
                            PST_RESULT_OK ||
                        pst_win32_socket_transport_create(
                            (pst_size)fixture_socket, &transport) !=
                            PST_RESULT_OK ||
                        pst_connection_attach(fixture_connection, transport,
                            PST_OWNERSHIP_TRANSFERRED, &ownership) !=
                            PST_RESULT_OK || ownership == 0U) goto cleanup;
                    fixture_socket = INVALID_SOCKET;
                    transport = NULL;
                }
            }
        }
        if (fixture_connection != NULL && operation != PST_OPERATION_COMPLETE &&
                operation != PST_OPERATION_FAILED) {
            if (pst_connection_handshake(fixture_connection, &operation,
                    &source) != PST_RESULT_OK) operation = PST_OPERATION_FAILED;
        }
        if (saw_job == PAPACC_TRUE &&
            secure_io->protocol_loop->network_job_manager.count == 0U &&
            ((mode == TEST_OUTBOUND_VALID &&
                secure_io->protocol_loop->network_stream_manager.count ==
                    baseline_streams + 1U) ||
             (mode != TEST_OUTBOUND_VALID &&
                secure_io->protocol_loop->network_stream_manager.count ==
                    baseline_streams))) break;
    }
    diagnostic_stage = index == 1600U ? 6 : 7;
    if (index == 1600U || !read_exact(&data, secure_io, open_result,
            sizeof(open_result)) || open_result[9] != 10U) goto cleanup;
    if (mode == TEST_OUTBOUND_VALID) {
        if (open_result[16] != PAPACC_NETWORK_OPEN_OK) goto cleanup;
    } else if (open_result[16] == PAPACC_NETWORK_OPEN_OK) goto cleanup;
    success = 1;

cleanup:
    if (!success && mode == TEST_OUTBOUND_VALID) {
        pst_u32 diagnostic_interest = PST_INTEREST_NONE;
        PST_RESULT diagnostic_interest_result = PST_RESULT_INVALID_STATE;
        if (fixture_connection != NULL)
            diagnostic_interest_result = pst_connection_get_interest(
                fixture_connection, &diagnostic_interest);
        fprintf(stderr,
            "CHECK101_DIAG stage=%d poll=%d loop=%zu saw_job=%d "
            "jobs=%zu streams=%zu baseline_streams=%zu connections=%zu "
            "baseline_connections=%zu stop_state=%d scheduler_members=%zu "
            "pst_operation=%lu pst_source=%lu interest_result=%lu "
            "interest=%lu fixture_connection=%d open_status=%u\n",
            diagnostic_stage, (int)diagnostic_poll_result, (size_t)index,
            (int)saw_job,
            (size_t)secure_io->protocol_loop->network_job_manager.count,
            (size_t)secure_io->protocol_loop->network_stream_manager.count,
            (size_t)baseline_streams,
            (size_t)secure_io->protocol_loop->connection_manager->count,
            (size_t)baseline_connections, (int)secure_io->stop_state,
            (size_t)secure_io->scheduler.member_count,
            (unsigned long)operation, (unsigned long)source,
            (unsigned long)diagnostic_interest_result,
            (unsigned long)diagnostic_interest,
            fixture_connection != NULL ? 1 : 0,
            (unsigned int)open_result[16]);
        (void)fflush(stderr);
    }
    papacc_pst_secure_processor_release(&data);
    for (index = 0U; index < 500U &&
            (secure_io->protocol_loop->network_job_manager.count != 0U ||
             secure_io->protocol_loop->network_stream_manager.count !=
                baseline_streams ||
             secure_io->protocol_loop->connection_manager->count !=
                baseline_connections); ++index)
        {
            if (papacc_server_secure_io_win32_poll_once(secure_io, 1U) !=
                    PAPACC_RESULT_OK) success = 0;
        }
    if (fixture_connection != NULL) pst_connection_release(fixture_connection);
    if (transport != NULL) pst_transport_release(transport);
    if (fixture_socket != INVALID_SOCKET) closesocket(fixture_socket);
    if (secure_io->protocol_loop->network_job_manager.count != 0U ||
        secure_io->protocol_loop->network_stream_manager.count !=
            baseline_streams ||
        secure_io->protocol_loop->connection_manager->count !=
            baseline_connections) success = 0;
    return success;
}

static int raw_candidate_rejected(PAPACC_U16 port,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, const PAPACC_U8 *bytes,
    PAPACC_SIZE size)
{
    SOCKET socket_value = connect_loopback(port);
    PAPACC_SIZE index;
    int sent;
    if (socket_value == INVALID_SOCKET) return 0;
    sent = send(socket_value, (const char *)bytes, (int)size, 0);
    if (sent != (int)size) {
        closesocket(socket_value);
        return 0;
    }
    for (index = 0U; index < 100U; ++index) {
        PAPACC_RESULT poll_result =
            papacc_server_secure_io_win32_poll_once(secure_io, 1U);
        if (poll_result != PAPACC_RESULT_OK) {
            fprintf(stderr, "raw poll result=%d round=%zu\n",
                (int)poll_result, (size_t)index);
            closesocket(socket_value);
            return 0;
        }
    }
    closesocket(socket_value);
    return 1;
}

static int tls_candidate_rejected(PAPACC_U16 port,
    PAPACC_SECURITY_COMPOSITION *composition,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,
    const PST_CONNECTION_CONFIG *configuration)
{
    SOCKET socket_value = INVALID_SOCKET;
    pst_transport *transport = NULL;
    PAPACC_PST_SECURE_PROCESSOR client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_BOOL ownership = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_RESULT client_result;
    PAPACC_SIZE index;
    int ok = 1;
    socket_value = connect_loopback(port);
    if (socket_value == INVALID_SOCKET ||
        papacc_pst_secure_processor_init(&client, composition->runtime,
            configuration, ~(PAPACC_U64)0) != PAPACC_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)socket_value, &transport) !=
            PST_RESULT_OK ||
        papacc_pst_secure_processor_attach(&client, transport, &ownership) !=
            PAPACC_RESULT_OK || ownership != PAPACC_TRUE) {
        ok = 0;
        goto cleanup;
    }
    transport = NULL;
    socket_value = INVALID_SOCKET;
    for (index = 0U; index < 200U; ++index) {
        client_result = papacc_pst_secure_processor_handshake_once(
            &client, &step);
        if (papacc_server_secure_io_win32_poll_once(secure_io, 5U) !=
                PAPACC_RESULT_OK) {
            ok = 0;
            break;
        }
        if (client_result != PAPACC_RESULT_OK ||
            step == PAPACC_PST_SECURE_STEP_FAILED ||
            step == PAPACC_PST_SECURE_STEP_CLOSED) break;
    }
cleanup:
    papacc_pst_secure_processor_release(&client);
    if (transport != NULL) pst_transport_release(transport);
    if (socket_value != INVALID_SOCKET) closesocket(socket_value);
    for (index = 0U; index < 50U; ++index) {
        if (papacc_server_secure_io_win32_poll_once(secure_io, 1U) !=
                PAPACC_RESULT_OK) ok = 0;
    }
    return ok;
}

typedef struct AI_RESOURCE_OBSERVATION {
    const char *type;
    PAPACC_U32 test_id;
    PAPACC_U32 acquired_count;
    PAPACC_U32 release_attempt_count;
    PAPACC_U32 release_success_count;
    PAPACC_BOOL reference_remaining;
} AI_RESOURCE_OBSERVATION;

typedef struct AI_CASE_C_OBSERVER {
    PAPACC_NETWORK_STREAM_IO delegate;
    PAPACC_NETWORK_STREAM *stream;
    PAPACC_OUTBOUND_TLS *tls;
    void *original_context;
    PAPACC_U64 stream_instance_id;
    PAPACC_U64 channel_instance_id;
    PAPACC_U64 connection_instance_id;
    DWORD owner_thread_id;
    AI_RESOURCE_OBSERVATION pst;
    AI_RESOURCE_OBSERVATION socket_value;
    AI_RESOURCE_OBSERVATION stream_value;
    AI_RESOURCE_OBSERVATION data;
    PAPACC_U32 terminal_publications;
    PAPACC_U32 original_close_calls;
    PAPACC_BOOL installed;
    PAPACC_BOOL terminal_truncated;
    PAPACC_BOOL context_before_original;
    PAPACC_BOOL context_during_unchanged;
    PAPACC_BOOL context_after_owner_release;
    PAPACC_BOOL thread_violation;
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io;
    PAPACC_BOOL observe_shutdown,cancel_on_pending,shutdown_started,shutdown_pending;
    PAPACC_BOOL shutdown_write_interest,cancel_after_pending;
    PAPACC_BOOL cancel_cause_preserved,false_clean_success;
    PAPACC_RESULT cancel_result;
    PAPACC_U32 delegate_call_count;
    PAPACC_RESULT delegate_result;
    PAPACC_BOOL delegate_done;
    PAPACC_PST_SECURE_PROCESSOR_STATE delegate_state_before;
    PAPACC_PST_SECURE_PROCESSOR_STATE delegate_state_after;
    PAPACC_RESULT delegate_interest_result;
    PAPACC_BOOL delegate_interest_read;
    PAPACC_BOOL delegate_interest_write;
    PST_RESULT delegate_source;
    PAPACC_BOOL pst_close_clean_observed,stop_delivered_to_owner;
    PAPACC_BOOL owner_stopped,wire_terminal_absent,deadline_armed,deadline_cause_observed;
    PAPACC_U32 terminal_publication_attempts,terminal_publication_successes;
    PAPACC_U16 terminal_type;
    PAPACC_BOOL signal_control_loss,control_loss_signaled;
    PAPACC_BOOL control_loss_acknowledged,control_loss_cause_observed;
    PAPACC_BOOL control_loss_owner_observed;
    PAPACC_U32 control_loss_cascade_count;
    volatile LONG control_loss_signal_attempts;
    volatile LONG control_loss_signal_successes;
    volatile LONG control_loss_signal_error;
    HANDLE control_loss_event;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    PAPACC_U32 owner_events_received,owner_events_validated;
    PAPACC_U32 owner_terminals_accepted,owner_terminals_suppressed;
    PAPACC_U32 owner_publication_attempts,owner_publication_successes;
    PAPACC_U32 owner_publication_failures;
    PAPACC_SIZE owner_publication_attempt_sequence;
    PAPACC_OWNER_TERMINAL_TEST_EVENT owner_publication_attempt_event;
    PAPACC_SIZE owner_publication_success_sequence;
    PAPACC_OWNER_TERMINAL_TEST_EVENT owner_publication_success_event;
    PAPACC_OWNER_TERMINAL_TEST_EVENT owner_publication_failure_event;
    PAPACC_U32 owner_publication_attempt_remove_successes;
    PAPACC_U32 owner_publication_success_remove_successes;
    PAPACC_U32 owner_publication_attempt_release_attempts;
    PAPACC_U32 owner_publication_success_release_attempts;
    PAPACC_PST_SECURE_PROCESSOR_STATE owner_publication_pst_state;
    PAPACC_NETWORK_DATA_STATE owner_publication_data_state;
    PAPACC_BOOL owner_publication_remote_half_closed;
    PAPACC_U16 owner_effective_terminal;
    PAPACC_NETWORK_STREAM_TERMINAL_CAUSE owner_terminal_cause;
    PAPACC_OWNER_TERMINAL_TEST_EVENT owner_event_log[64];
    PAPACC_SIZE owner_event_count;
    PAPACC_BOOL p9_real_waitset_membership;
    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_MODE p10_remove_mode;
    PAPACC_BOOL p10_normal_only;
    PAPACC_BOOL p10_seam_configured;
    PAPACC_RESULT p10_first_stop_result;
    PAPACC_SIZE p10_membership_before;
    PAPACC_SIZE p10_membership_after_failure;
    PAPACC_SIZE p10_membership_after_normal;
    PAPACC_BOOL p10_selected_membership_after_normal;
    PAPACC_U64 p10_deadline_before;
    PAPACC_U64 p10_deadline_after_failure;
    PAPACC_NETWORK_STREAM_TERMINAL_CAUSE p10_cause_after_failure;
    PAPACC_BOOL p10_connection_preserved;
    PAPACC_BOOL p10_processor_preserved;
    PAPACC_U32 p10_release_attempts_after_failure;
    PAPACC_U32 p10_destructive_close_attempts_after_failure;
    PAPACC_SERVER_SECURE_STOP_STATE p10_stop_state_after_failure;
    REFERENCE_FIXTURE_THREAD *p10_fixture;
    DWORD p10_marker_ack_wait_result;
    DWORD p10_marker_ack_wait_error;
    PAPACC_BOOL p10_marker_ack_observed;
    PAPACC_BOOL p10_stop_request_after_ack;
    LONG p10_marker_ack_sequence;
    LONG p10_stop_request_sequence;
    LONG p10_owner_stop_ack_signal_sequence;
    PAPACC_U32 p10_owner_stop_ack_signal_attempts;
    PAPACC_U32 p10_owner_stop_ack_signal_successes;
    DWORD p10_owner_stop_ack_signal_error;
    PAPACC_SERVER_SECURE_STOP_STATE p10_stop_state_at_ack;
    PAPACC_BOOL p10_target_membership_at_ack;
    char p10_run_id[64];
    char p10_trace_path[MAX_PATH];
    PAPACC_U32 p10_owner_poll_call_count;
    PAPACC_RESULT p10_owner_poll_result;
    PAPACC_SERVER_SECURE_STOP_STATE p10_stop_state_before;
    PAPACC_SERVER_SECURE_STOP_STATE p10_stop_state_after;
    PAPACC_BOOL p10_trace_finalized;
    const char *p10_check115_first_failed;
    char p10_check115_expected[64];
    char p10_check115_observed[64];
    HANDLE g1_site_release_event;
    HANDLE g1_client_ack_event;
    PAPACC_BOOL g1_site_release_signaled;
    DWORD g1_site_release_error;
    PAPACC_U32 g1_ack_signal_attempts;
    PAPACC_U32 g1_ack_signal_successes;
    DWORD g1_ack_signal_error;
    PAPACC_BOOL g1_owner_injection_requested;
    PAPACC_BOOL g2_mode;
    PAPACC_BOOL g1_owner_injection_performed;
    PAPACC_RESULT g1_owner_injection_result;
    PAPACC_BOOL g1_data_terminal_after_first;
    PAPACC_BOOL g1_data_read_disabled_after_first;
    REFERENCE_FIXTURE_THREAD *g1_fixture;
    char g1_run_id[64];
    char g1_trace_path[MAX_PATH];
    PAPACC_BOOL g1_trace_created;
    PAPACC_BOOL g1_trace_finalized;
#endif
} AI_CASE_C_OBSERVER;

#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
#include "p10_normal_native_observer.inc"
#endif
static AI_CASE_C_OBSERVER *ai_active_observer = NULL;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
static void ai_p10_write_trace(AI_CASE_C_OBSERVER *observer,
    const char *exit_reason)
{
    FILE *trace=NULL;
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION removal;
    if(observer==NULL||observer->p10_remove_mode==
            PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF||
        observer->p10_trace_path[0]=='\0')return;
    memset(&removal,0,sizeof(removal));
    (void)papacc_server_secure_io_win32_test_get_remove_observation(
        observer->secure_io,&removal);
    if(fopen_s(&trace,observer->p10_trace_path,"wb")!=0||trace==NULL)return;
    fprintf(trace,"P10_RUN_ID=%s\n",observer->p10_run_id);
    fprintf(trace,"P10_OWNER_POLL_CALL_COUNT=%lu\n",
        (unsigned long)observer->p10_owner_poll_call_count);
    fprintf(trace,"P10_OWNER_POLL_RESULT=%d\n",
        (int)observer->p10_owner_poll_result);
    fprintf(trace,"P10_FIRST_NON_OK_FUNCTION=%s\n",
        observer->secure_io->test_last_non_ok_function!=NULL?
        observer->secure_io->test_last_non_ok_function:"NOT_RECORDED");
    fprintf(trace,"P10_STOP_STATE_BEFORE=%u\n",
        (unsigned)observer->p10_stop_state_before);
    fprintf(trace,"P10_STOP_STATE_AFTER=%u\n",
        (unsigned)observer->p10_stop_state_after);
    fprintf(trace,"P10_REMOVE_ATTEMPTS=%lu\n",
        (unsigned long)removal.remove_attempt_count);
    fprintf(trace,"P10_INJECTED_FAILURES=%lu\n",
        (unsigned long)removal.injected_failure_count);
    fprintf(trace,"P10_REAL_REMOVE_CALLS=%lu\n",
        (unsigned long)removal.real_remove_call_count);
    fprintf(trace,"P10_REAL_REMOVE_SUCCESSES=%lu\n",
        (unsigned long)removal.real_remove_success_count);
    fprintf(trace,"P10_NORMAL_REMOVE_ATTEMPTS=%lu\n",
        (unsigned long)removal.normal_remove_attempt_count);
    fprintf(trace,"P10_NORMAL_REMOVE_SUCCESSES=%lu\n",
        (unsigned long)removal.normal_remove_success_count);
    fprintf(trace,"P10_STOP_REMOVE_ATTEMPTS=%lu\n",
        (unsigned long)removal.stop_remove_attempt_count);
    fprintf(trace,"P10_CONNECTION_ID=%llu\n",
        (unsigned long long)removal.connection_instance_id);
    fprintf(trace,"P10_STREAM_GENERATION=%llu\n",
        (unsigned long long)removal.stream_generation);
    fprintf(trace,"P10_WAITSET_MEMBERSHIP=%lu\n",
        (unsigned long)observer->secure_io->scheduler.member_count);
    fprintf(trace,"P10_PROCESSOR_STATE=%u\n",
        observer->tls!=NULL?(unsigned)observer->tls->processor.state:0U);
    fprintf(trace,"P10_REMOVE_ORIGIN=%s\n",
        removal.stop_remove_attempt_count!=0U?"STOP_REMOVE_ROUND":
        (removal.normal_remove_attempt_count!=0U?"NORMAL_REMOVE":"NOT_OBSERVED"));
    fprintf(trace,"P10_SEAM_ARMED=%s\n",
        observer->p10_seam_configured==PAPACC_TRUE?"YES":"NO");
    fprintf(trace,"P10_SEAM_ELIGIBLE=%s\n",
        removal.stop_remove_attempt_count!=0U?"YES":"NO");
    fprintf(trace,"P10_SEAM_CONSUMED=%s\n",
        removal.injected_failure_count!=0U?"YES":"NO");
    fprintf(trace,"P10_MEMBERSHIP_BEFORE=%lu\n",
        (unsigned long)observer->p10_membership_before);
    fprintf(trace,"P10_MEMBERSHIP_AFTER=%lu\n",
        (unsigned long)observer->secure_io->scheduler.member_count);
    fprintf(trace,"P10_MARKER_RECEIVED=%s\n",
        observer->p10_fixture!=NULL&&
        observer->p10_fixture->p10_marker_received!=0L?"YES":"NO");
    fprintf(trace,"P10_MARKER_VALIDATED=%s\n",
        observer->p10_fixture!=NULL&&
        observer->p10_fixture->p10_marker_validated!=0L?"YES":"NO");
    fprintf(trace,"P10_MARKER_ACK_SIGNAL_RESULT=%s\n",
        observer->p10_fixture!=NULL&&
        observer->p10_fixture->p10_marker_signal_successes==1L?
        "SUCCESS":"NOT_SUCCESSFUL");
    fprintf(trace,"P10_MARKER_ACK_WAIT_RESULT=%lu\n",
        (unsigned long)observer->p10_marker_ack_wait_result);
    fprintf(trace,"P10_MARKER_RECEIVE_SEQUENCE=%ld\n",
        observer->p10_fixture!=NULL?
        observer->p10_fixture->p10_marker_receive_sequence:0L);
    fprintf(trace,"P10_MARKER_SIGNAL_SEQUENCE=%ld\n",
        observer->p10_fixture!=NULL?
        observer->p10_fixture->p10_marker_signal_sequence:0L);
    fprintf(trace,"P10_MARKER_ACK_SEQUENCE=%ld\n",
        observer->p10_marker_ack_sequence);
    fprintf(trace,"P10_STOP_REQUEST_SEQUENCE=%ld\n",
        observer->p10_stop_request_sequence);
    fprintf(trace,"P10_STOP_STATE_AT_ACK=%u\n",
        (unsigned)observer->p10_stop_state_at_ack);
    fprintf(trace,"P10_TARGET_MEMBERSHIP_AT_ACK=%s\n",
        observer->p10_target_membership_at_ack==PAPACC_TRUE?"YES":"NO");
    fprintf(trace,"P10_OWNER_STOP_ACK_SIGNAL_ATTEMPTS=%lu\n",
        (unsigned long)observer->p10_owner_stop_ack_signal_attempts);
    fprintf(trace,"P10_OWNER_STOP_ACK_SIGNAL_SUCCESSES=%lu\n",
        (unsigned long)observer->p10_owner_stop_ack_signal_successes);
    fprintf(trace,"P10_OWNER_STOP_ACK_SIGNAL_ERROR=%lu\n",
        (unsigned long)observer->p10_owner_stop_ack_signal_error);
    fprintf(trace,"P10_OWNER_STOP_ACK_SIGNAL_SEQUENCE=%ld\n",
        observer->p10_owner_stop_ack_signal_sequence);
    fprintf(trace,"P10_FIXTURE_STOP_ACK_WAIT_RESULT=%ld\n",
        observer->p10_fixture!=NULL?
        observer->p10_fixture->p10_owner_stop_wait_result:0L);
    fprintf(trace,"P10_FIXTURE_STOP_ACK_OBSERVED_SEQUENCE=%ld\n",
        observer->p10_fixture!=NULL?
        observer->p10_fixture->p10_owner_stop_observed_sequence:0L);
    fprintf(trace,"P10_STOP_REQUESTED=%s\n",
        observer->p10_first_stop_result!=PAPACC_RESULT_OK||
        observer->secure_io->stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING?
        "YES":"NO");
    fprintf(trace,"P10_FIRST_STOP_RESULT=%d\n",
        (int)observer->p10_first_stop_result);
    fprintf(trace,"P10_CONNECTION_PRESERVED=%s\n",
        observer->p10_connection_preserved==PAPACC_TRUE?"YES":"NO");
    fprintf(trace,"P10_PROCESSOR_PRESERVED=%s\n",
        observer->p10_processor_preserved==PAPACC_TRUE?"YES":"NO");
    fprintf(trace,"P10_PST_RELEASE_ATTEMPTS=%lu\n",
        (unsigned long)observer->pst.release_attempt_count);
    fprintf(trace,"P10_PST_RELEASE_ATTEMPTS_AFTER_FAILURE=%lu\n",
        (unsigned long)observer->p10_release_attempts_after_failure);
    fprintf(trace,"P10_DESTRUCTIVE_CLOSE_ATTEMPTS=%lu\n",
        (unsigned long)observer->original_close_calls);
    fprintf(trace,"P10_DESTRUCTIVE_CLOSE_ATTEMPTS_AFTER_FAILURE=%lu\n",
        (unsigned long)observer->p10_destructive_close_attempts_after_failure);
    fprintf(trace,"P10_STOP_STATE_AFTER_FAILURE=%u\n",
        (unsigned)observer->p10_stop_state_after_failure);
    fprintf(trace,"P10_ORIGINAL_CAUSE=%u\n",
        (unsigned)observer->p10_cause_after_failure);
    fprintf(trace,"P10_FINAL_CAUSE=%u\n",
        (unsigned)observer->secure_io->stop_cause);
    fprintf(trace,"P10_ORIGINAL_DEADLINE=%llu\n",
        (unsigned long long)observer->p10_deadline_after_failure);
    fprintf(trace,"P10_FINAL_DEADLINE=%llu\n",
        (unsigned long long)observer->secure_io->shutdown_deadline_ns);
    fprintf(trace,"P10_RELEASE_AFTER_REMOVE=%s\n",
        observer->pst.release_attempt_count!=0U&&
        removal.real_remove_success_count!=0U?"YES":"NO_OR_NOT_OBSERVED");
    fprintf(trace,"PUBLICATION_CALL_SITE=progress_network_control\n");
    fprintf(trace,"PUBLICATION_TRIGGER=TLS_PEER_CLEAN_CLOSE_REMOTE_HALF_CLOSED\n");
    fprintf(trace,"PUBLICATION_OPERATION_ID=%llu\n",
        (unsigned long long)observer->owner_publication_attempt_event.operation_id);
    fprintf(trace,"PUBLICATION_STREAM_GENERATION=%llu\n",
        (unsigned long long)observer->owner_publication_attempt_event.stream_generation);
    fprintf(trace,"PUBLICATION_CONNECTION_ID=%llu\n",
        (unsigned long long)observer->connection_instance_id);
    fprintf(trace,"PUBLICATION_TERMINAL_TYPE=%u\n",
        (unsigned)observer->owner_publication_attempt_event.terminal_type);
    fprintf(trace,"PUBLICATION_CAUSE=%u\n",
        (unsigned)observer->owner_publication_attempt_event.terminal_cause);
    fprintf(trace,"PUBLICATION_PACC_RESULT=%d\n",
        (int)observer->owner_publication_success_event.publication_result);
    fprintf(trace,"PUBLICATION_WRITER_STATUS=%u\n",
        (unsigned)observer->owner_publication_success_event.writer_status);
    fprintf(trace,"PUBLICATION_ATTEMPTS=%lu\n",
        (unsigned long)observer->owner_publication_attempts);
    fprintf(trace,"PUBLICATION_SUCCESSES=%lu\n",
        (unsigned long)observer->owner_publication_successes);
    fprintf(trace,"PUBLICATION_FAILURES=%lu\n",
        (unsigned long)observer->owner_publication_failures);
    fprintf(trace,"PUBLICATION_ATTEMPT_SEQUENCE=%lu\n",
        (unsigned long)observer->owner_publication_attempt_sequence);
    fprintf(trace,"PUBLICATION_SUCCESS_SEQUENCE=%lu\n",
        (unsigned long)observer->owner_publication_success_sequence);
    fprintf(trace,"PUBLICATION_REMOTE_HALF_CLOSED=%s\n",
        observer->owner_publication_remote_half_closed==PAPACC_TRUE?"YES":"NO");
    fprintf(trace,"PUBLICATION_PST_STATE=%u\n",
        (unsigned)observer->owner_publication_pst_state);
    fprintf(trace,"PUBLICATION_DATA_STATE=%u\n",
        (unsigned)observer->owner_publication_data_state);
    fprintf(trace,"PUBLICATION_ATTEMPT_REMOVE_SUCCESSES=%lu\n",
        (unsigned long)observer->owner_publication_attempt_remove_successes);
    fprintf(trace,"PUBLICATION_SUCCESS_REMOVE_SUCCESSES=%lu\n",
        (unsigned long)observer->owner_publication_success_remove_successes);
    fprintf(trace,"PUBLICATION_ATTEMPT_RELEASE_ATTEMPTS=%lu\n",
        (unsigned long)observer->owner_publication_attempt_release_attempts);
    fprintf(trace,"PUBLICATION_SUCCESS_RELEASE_ATTEMPTS=%lu\n",
        (unsigned long)observer->owner_publication_success_release_attempts);
    fprintf(trace,"P10_TERMINAL_EXPECTED=NETWORK_CLOSE\n");
    fprintf(trace,"P10_TERMINAL_OBSERVED=%u\n",
        (unsigned)observer->owner_effective_terminal);
    fprintf(trace,"P10_TERMINAL_COUNT=%lu\n",
        (unsigned long)observer->owner_terminals_accepted);
    fprintf(trace,"P10_EXTRA_TERMINAL_COUNT=%lu\n",
        (unsigned long)observer->owner_terminals_suppressed);
    fprintf(trace,"CHECK115_FIRST_FAILED_SUBCONDITION=%s\n",
        observer->p10_check115_first_failed!=NULL?
        observer->p10_check115_first_failed:"NONE");
    fprintf(trace,"CHECK115_EXPECTED=%s\n",
        observer->p10_check115_expected[0]!='\0'?
        observer->p10_check115_expected:"ALL_TRUE");
    fprintf(trace,"CHECK115_OBSERVED=%s\n",
        observer->p10_check115_observed[0]!='\0'?
        observer->p10_check115_observed:"ALL_TRUE");
    fprintf(trace,"P10_REFERENCE_CLIENT_FAIL_STAGE=NOT_REACHED\n");
    fprintf(trace,"P10_REFERENCE_CLIENT_EXIT_REASON=%s\n",exit_reason);
    (void)fflush(trace);fclose(trace);observer->p10_trace_finalized=PAPACC_TRUE;
}

static void ai_p10_write_fixture_trace(
    const REFERENCE_FIXTURE_THREAD *fixture,
    const AI_CASE_C_OBSERVER *observer,DWORD wait_result,DWORD wait_error)
{
    char path[MAX_PATH];
    FILE *trace=NULL;
    if(fixture==NULL||observer==NULL||observer->p10_run_id[0]=='\0'||
        sprintf_s(path,sizeof(path),"papacc-p10-fixture-%s.trace",
            observer->p10_run_id)<0)return;
    if(fopen_s(&trace,path,"wb")!=0||trace==NULL)return;
    fprintf(trace,"P10_RUN_ID=%s\n",observer->p10_run_id);
    fprintf(trace,"FIXTURE_LISTENER_READY=%s\n",
        fixture->diagnostic_listener_ready!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_THREAD_STARTED=%s\n",
        fixture->diagnostic_thread_started!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_ACCEPT_COMPLETED=%s\n",
        fixture->diagnostic_accept_completed!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_TLS_HANDSHAKE_COMPLETED=%s\n",
        fixture->diagnostic_handshake_completed!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_APPLICATION_DATA_COMPLETED=%s\n",
        fixture->diagnostic_application_data_completed!=0L?"YES":"NO");
    fprintf(trace,"P10_MARKER_RECEIVED=%s\n",
        fixture->p10_marker_received!=0L?"YES":"NO");
    fprintf(trace,"P10_MARKER_VALIDATED=%s\n",
        fixture->p10_marker_validated!=0L?"YES":"NO");
    fprintf(trace,"P10_MARKER_ACK_SIGNAL_ATTEMPTS=%ld\n",
        fixture->p10_marker_signal_attempts);
    fprintf(trace,"P10_MARKER_ACK_SIGNAL_SUCCESSES=%ld\n",
        fixture->p10_marker_signal_successes);
    fprintf(trace,"P10_MARKER_ACK_SIGNAL_ERROR=%ld\n",
        fixture->p10_marker_signal_error);
    fprintf(trace,"P10_CONNECTION_ID=%llu\n",
        (unsigned long long)fixture->p10_connection_instance_id);
    fprintf(trace,"P10_STREAM_GENERATION=%llu\n",
        (unsigned long long)fixture->p10_stream_generation);
    fprintf(trace,"P10_OWNER_STOP_ACK_WAIT_RESULT=%ld\n",
        fixture->p10_owner_stop_wait_result);
    fprintf(trace,"P10_OWNER_STOP_ACK_WAIT_ERROR=%ld\n",
        fixture->p10_owner_stop_wait_error);
    fprintf(trace,"P10_OWNER_STOP_ACK_OBSERVED=%s\n",
        fixture->p10_owner_stop_observed!=0L?"YES":"NO");
    fprintf(trace,"P10_OWNER_STOP_ACK_OBSERVED_SEQUENCE=%ld\n",
        fixture->p10_owner_stop_observed_sequence);
    fprintf(trace,"P10_FIXTURE_POST_ACK_LOOP_ITERATIONS=%ld\n",
        fixture->p10_post_ack_loop_iterations);
    fprintf(trace,"P10_FIXTURE_PST_READ_RESULT=%ld\n",
        fixture->p10_post_ack_read_result);
    fprintf(trace,"P10_FIXTURE_PST_READ_OPERATION=%ld\n",
        fixture->p10_post_ack_read_operation);
    fprintf(trace,"P10_FIXTURE_PST_READ_SOURCE=%ld\n",
        fixture->p10_post_ack_read_source);
    fprintf(trace,"P10_FIXTURE_PST_INTEREST_RESULT=%ld\n",
        fixture->p10_post_ack_interest_result);
    fprintf(trace,"P10_FIXTURE_PST_INTEREST_FLAGS=%ld\n",
        fixture->p10_post_ack_interest_flags);
    fprintf(trace,"P10_FIXTURE_WAIT_RESULT=%ld\n",
        fixture->p10_post_ack_wait_result);
    fprintf(trace,"P10_FIXTURE_WAIT_SOURCE=NOT_AVAILABLE_PUBLIC_API\n");
    fprintf(trace,"P10_FIXTURE_WAIT_READY_INTEREST=%ld\n",
        fixture->p10_post_ack_wait_ready_interest);
    fprintf(trace,"P10_FIXTURE_WAIT_TIMED_OUT=%ld\n",
        fixture->p10_post_ack_wait_timed_out);
    fprintf(trace,"P10_FIXTURE_WAIT_ELAPSED_MS=%llu\n",
        (unsigned long long)(fixture->p10_post_ack_wait_elapsed_ns/
            1000000ULL));
    fprintf(trace,"P10_FIXTURE_WAIT_CALLS=%lu\n",
        (unsigned long)fixture->p10_post_ack_wait_call_count);
    fprintf(trace,"P10_FIXTURE_WAIT_TOTAL_ELAPSED_MS=%llu\n",
        (unsigned long long)(fixture->p10_post_ack_wait_total_elapsed_ns/
            1000000ULL));
    fprintf(trace,"P10_FIXTURE_MONOTONIC_START=%llu\n",
        (unsigned long long)fixture->p10_post_ack_monotonic_start_ns);
    fprintf(trace,"P10_FIXTURE_MONOTONIC_NOW=%llu\n",
        (unsigned long long)fixture->p10_post_ack_monotonic_now_ns);
    fprintf(trace,"P10_FIXTURE_MONOTONIC_DEADLINE=%llu\n",
        (unsigned long long)fixture->p10_post_ack_monotonic_deadline_ns);
    fprintf(trace,"P10_FIXTURE_MONOTONIC_DEADLINE_REACHED=%s\n",
        fixture->p10_post_ack_monotonic_deadline_ns!=0U&&
        fixture->p10_post_ack_monotonic_now_ns>=
            fixture->p10_post_ack_monotonic_deadline_ns?"YES":"NO");
    fprintf(trace,"P10_FIXTURE_SOCKET_SELECT_RESULT=%ld\n",
        fixture->p10_post_ack_socket_select_result);
    fprintf(trace,"P10_FIXTURE_SOCKET_READINESS=%s\n",
        fixture->p10_post_ack_socket_readiness!=0L?"READABLE":"NOT_READABLE");
    fprintf(trace,"P10_FIXTURE_SOCKET_EOF=NOT_OBSERVED_NO_OUT_OF_BAND_RECV\n");
    fprintf(trace,"P10_FIXTURE_SOCKET_ERROR=%ld\n",
        fixture->p10_post_ack_socket_error);
    fprintf(trace,"P10_FIXTURE_TERMINAL_CLASSIFICATION=%s\n",
        fixture->diagnostic_transport_terminal_observed!=0L?
        "PST_OPERATION_TERMINAL":"NOT_TERMINAL");
    fprintf(trace,"P10_FIXTURE_EXIT_REASON=%s\n",
        fixture->p10_post_ack_exit_reason==1L?"PST_READ_NON_OK":
        (fixture->p10_post_ack_exit_reason==2L?"PST_READ_TERMINAL":
        (fixture->p10_post_ack_exit_reason==3L?"PST_WAIT_NON_OK":
        (fixture->p10_post_ack_exit_reason==4L?"LOOP_EXHAUSTED":
        "NOT_REACHED"))));
    fprintf(trace,"FIXTURE_PEER_CLOSE_OBSERVED=%s\n",
        fixture->diagnostic_transport_terminal_observed!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_SHUTDOWN_STARTED=%s\n",
        fixture->diagnostic_shutdown_started!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_SHUTDOWN_OPERATION=%ld\n",
        fixture->diagnostic_shutdown_operation);
    fprintf(trace,"FIXTURE_SHUTDOWN_RESULT=%ld\n",
        fixture->diagnostic_shutdown_result);
    fprintf(trace,"FIXTURE_SHUTDOWN_SOURCE=%ld\n",
        fixture->diagnostic_shutdown_source);
    fprintf(trace,"FIXTURE_TRANSPORT_TERMINAL_OBSERVED=%s\n",
        fixture->diagnostic_transport_terminal_observed!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_THREAD_EXIT=%s\n",
        fixture->diagnostic_thread_exit!=0L?"YES":"NO");
    fprintf(trace,"FIXTURE_THREAD_EXIT_CODE=%ld\n",
        fixture->diagnostic_thread_exit_code);
    fprintf(trace,"FIXTURE_RESULT=%d\n",fixture->result);
    fprintf(trace,"CHECK114_EXPECTED=WAIT_OBJECT_0_AND_RESULT_TRUE\n");
    fprintf(trace,"CHECK114_WAIT_RESULT=%lu\n",(unsigned long)wait_result);
    fprintf(trace,"CHECK114_WAIT_ERROR=%lu\n",(unsigned long)wait_error);
    fprintf(trace,"CHECK114_OBSERVED=%s\n",
        wait_result==WAIT_OBJECT_0&&fixture->result?
        "WAIT_OBJECT_0_AND_RESULT_TRUE":
        (wait_result==WAIT_TIMEOUT?"WAIT_TIMEOUT":
        (wait_result==WAIT_FAILED?"WAIT_FAILED":"THREAD_EXIT_RESULT_FALSE")));
    (void)fflush(trace);
    fclose(trace);
    printf("P10_FIXTURE_TRACE_PATH=%s P10_FIXTURE_TRACE_CREATED=YES\n",path);
}

#else
static void ai_p10_write_trace(AI_CASE_C_OBSERVER *observer,
    const char *exit_reason)
{
    (void)observer;
    (void)exit_reason;
}
#endif

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
static void ai_owner_terminal_observe(void *context,
    const PAPACC_OWNER_TERMINAL_TEST_EVENT *event)
{
    AI_CASE_C_OBSERVER *observer=(AI_CASE_C_OBSERVER *)context;
    if(observer==NULL||event==NULL)return;
    if(observer->owner_event_count<64U)
        observer->owner_event_log[observer->owner_event_count++]=*event;
    if(event->kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_RECEIVED)
        observer->owner_events_received++;
    else if(event->kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_VALIDATED)
        observer->owner_events_validated++;
    else if(event->kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_ACCEPTED){
        observer->owner_terminals_accepted++;
        observer->owner_effective_terminal=event->terminal_type;
        observer->owner_terminal_cause=event->terminal_cause;
        if(event->internal_injection!=PAPACC_TRUE&&
            (event->terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_CLOSE||
             (observer->g2_mode==PAPACC_TRUE&&
              event->terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_ABORT))){
            observer->g1_owner_injection_requested=PAPACC_TRUE;
            if(observer->g2_mode==PAPACC_TRUE&&observer->secure_io!=NULL&&
                observer->g1_owner_injection_performed!=PAPACC_TRUE){
                observer->g1_owner_injection_performed=PAPACC_TRUE;
                observer->g1_owner_injection_result=
                    papacc_server_io_loop_win32_test_inject_owner_terminal(
                        observer->secure_io->protocol_loop,event->operation_id,
                        event->stream_generation,PAPACC_MESSAGE_TYPE_NETWORK_ABORT);
            }
        }
    }else if(event->kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_SUPPRESSED){
        observer->owner_terminals_suppressed++;
        if((event->terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_CLOSE||
             (observer->g2_mode==PAPACC_TRUE&&
              event->terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_ABORT))&&
            event->internal_injection==PAPACC_TRUE&&
            observer->g1_site_release_event!=NULL&&
            observer->channel_instance_id!=0U&&
            event->operation_id==observer->channel_instance_id&&
            event->stream_generation==observer->stream_instance_id){
            if(SetEvent(observer->g1_site_release_event)!=0)
                observer->g1_site_release_signaled=PAPACC_TRUE;
            else observer->g1_site_release_error=GetLastError();
            observer->g1_ack_signal_attempts++;
            if(observer->g1_client_ack_event!=NULL&&
                SetEvent(observer->g1_client_ack_event)!=0)
                observer->g1_ack_signal_successes++;
            else observer->g1_ack_signal_error=GetLastError();
        }
    }
    else if(event->kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_ATTEMPT){
        PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION removal;
        observer->owner_publication_attempts++;
        observer->owner_publication_attempt_sequence=observer->owner_event_count;
        observer->owner_publication_attempt_event=*event;
        memset(&removal,0,sizeof(removal));
        if(observer->secure_io!=NULL&&
            papacc_server_secure_io_win32_test_get_remove_observation(
                observer->secure_io,&removal)==PAPACC_RESULT_OK)
            observer->owner_publication_attempt_remove_successes=
                removal.real_remove_success_count;
        observer->owner_publication_attempt_release_attempts=
            observer->pst.release_attempt_count;
        if(observer->tls!=NULL)
            observer->owner_publication_pst_state=observer->tls->processor.state;
        if(observer->stream!=NULL)
            observer->owner_publication_remote_half_closed=
                observer->stream->remote_half_closed;
        if(observer->secure_io!=NULL&&observer->secure_io->protocol_loop!=NULL){
            PAPACC_SIZE index;
            for(index=0U;index<observer->secure_io->protocol_loop->processor_capacity;
                    ++index){
                PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot=
                    &observer->secure_io->protocol_loop->processor_slots[index];
                if(slot->in_use&&
                    slot->kind==PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH&&
                    slot->network_data_protocol.channel_instance_id==
                        event->operation_id){
                    observer->owner_publication_data_state=
                        slot->network_data_protocol.state;
                    break;
                }
            }
        }
    }else if(event->kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS){
        PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION removal;
        observer->owner_publication_successes++;
        observer->owner_publication_success_sequence=observer->owner_event_count;
        observer->owner_publication_success_event=*event;
        memset(&removal,0,sizeof(removal));
        if(observer->secure_io!=NULL&&
            papacc_server_secure_io_win32_test_get_remove_observation(
                observer->secure_io,&removal)==PAPACC_RESULT_OK)
            observer->owner_publication_success_remove_successes=
                removal.real_remove_success_count;
        observer->owner_publication_success_release_attempts=
            observer->pst.release_attempt_count;
    }
    else if(event->kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE){
        observer->owner_publication_failures++;
        observer->owner_publication_failure_event=*event;
    }
}
#endif

static PAPACC_RESULT ai_shutdown(void *context, PAPACC_BOOL *done)
{
    AI_CASE_C_OBSERVER *o=ai_active_observer; PAPACC_RESULT r;
    PAPACC_BOOL rd=PAPACC_FALSE,wr=PAPACC_FALSE;
    if(o==NULL||o->installed!=PAPACC_TRUE||context!=o->original_context||done==NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if(GetCurrentThreadId()!=o->owner_thread_id)o->thread_violation=PAPACC_TRUE;
    o->context_during_unchanged=o->stream->io.context==o->original_context?PAPACC_TRUE:PAPACC_FALSE;
    o->pst_close_clean_observed=o->tls->clean_peer_close;
    o->shutdown_started=PAPACC_TRUE;
    o->delegate_call_count++;
    o->delegate_state_before=o->tls->processor.state;
    r=o->delegate.shutdown(context,done);
    o->delegate_result=r;
    o->delegate_done=*done;
    o->delegate_state_after=o->tls->processor.state;
    o->delegate_source=o->tls->processor.source_result;
    if(r!=PAPACC_RESULT_OK||*done==PAPACC_TRUE){if(*done==PAPACC_TRUE)o->false_clean_success=PAPACC_TRUE;return r;}
    o->delegate_interest_result=papacc_outbound_tls_interest(o->tls,&rd,&wr);
    o->delegate_interest_read=rd;
    o->delegate_interest_write=wr;
    if(o->delegate_interest_result!=PAPACC_RESULT_OK||wr!=PAPACC_TRUE)return PAPACC_RESULT_INTERNAL_ERROR;
    o->shutdown_pending=PAPACC_TRUE;o->shutdown_write_interest=PAPACC_TRUE;
    o->deadline_armed=o->stream->close_deadline_ns!=0U?PAPACC_TRUE:PAPACC_FALSE;
    if(o->signal_control_loss==PAPACC_TRUE&&
       o->control_loss_signaled!=PAPACC_TRUE){
        (void)InterlockedIncrement(&o->control_loss_signal_attempts);
        if(o->control_loss_event==NULL){
            (void)InterlockedExchange(&o->control_loss_signal_error,
                (LONG)ERROR_INVALID_HANDLE);
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        if(!SetEvent(o->control_loss_event)){
            (void)InterlockedExchange(&o->control_loss_signal_error,
                (LONG)GetLastError());
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        (void)InterlockedIncrement(&o->control_loss_signal_successes);
        o->control_loss_signaled=PAPACC_TRUE;
    }
    if(o->cancel_on_pending!=PAPACC_TRUE)return r;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    if(o->p10_remove_mode!=PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF&&
       o->p10_normal_only!=PAPACC_TRUE&&
       o->p10_marker_ack_observed!=PAPACC_TRUE)return r;
    if(o->p10_remove_mode!=PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF&&
       o->p10_normal_only!=PAPACC_TRUE&&
       o->secure_io->stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING)return r;
    o->p10_deadline_before=o->secure_io->shutdown_deadline_ns;
#endif
    o->cancel_result=papacc_server_secure_io_win32_request_stop(o->secure_io);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    o->p10_first_stop_result=o->cancel_result;
#endif
    if(o->cancel_result==PAPACC_RESULT_OK){o->cancel_after_pending=PAPACC_TRUE;
        o->stop_delivered_to_owner=o->secure_io->stop_state != PAPACC_SERVER_SECURE_STOP_RUNNING;
        o->cancel_cause_preserved=o->stop_delivered_to_owner;}
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    else if(o->p10_remove_mode!=PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF){
        o->p10_membership_after_failure=o->secure_io->scheduler.member_count;
        o->p10_deadline_after_failure=o->secure_io->shutdown_deadline_ns;
        o->p10_cause_after_failure=o->secure_io->stop_cause;
        o->p10_connection_preserved=o->tls->processor.connection!=NULL?
            PAPACC_TRUE:PAPACC_FALSE;
        o->p10_processor_preserved=o->tls->processor.state!=
            PAPACC_PST_SECURE_PROCESSOR_CLOSED?PAPACC_TRUE:PAPACC_FALSE;
        o->secure_io->stop_retry_not_before_ns=0U;
        return r;
    }
#endif
    return o->cancel_result;
}

static void ai_resource_init(AI_RESOURCE_OBSERVATION *resource,
    const char *type, PAPACC_U32 test_id)
{
    memset(resource, 0, sizeof(*resource));
    resource->type = type;
    resource->test_id = test_id;
}

static void ai_close(void *context)
{
    AI_CASE_C_OBSERVER *observer = ai_active_observer;
    if (observer == NULL || observer->installed != PAPACC_TRUE ||
        context != observer->original_context) return;
    if (GetCurrentThreadId() != observer->owner_thread_id)
        observer->thread_violation = PAPACC_TRUE;
    observer->context_during_unchanged =
        observer->stream->io.context == observer->original_context ?
        PAPACC_TRUE : PAPACC_FALSE;
    if(observer->stream->terminal_cause==PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SHUTDOWN_DEADLINE)
        observer->deadline_cause_observed=PAPACC_TRUE;
    if(observer->stream->terminal_cause==
            PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_CONTROL_TRANSPORT_LOSS)
        observer->control_loss_cause_observed=PAPACC_TRUE;
    observer->pst.release_attempt_count++;
    observer->socket_value.release_attempt_count++;
    observer->stream_value.release_attempt_count++;
    if (observer->tls->truncated == PAPACC_TRUE)
        observer->terminal_truncated = PAPACC_TRUE;
    observer->original_close_calls++;
    observer->delegate.close(context);
    observer->context_after_owner_release =
        observer->stream->io.context == observer->original_context ?
        PAPACC_TRUE : PAPACC_FALSE;
    if (observer->tls->processor.connection == NULL) {
        observer->pst.release_success_count++;
        observer->pst.reference_remaining = PAPACC_FALSE;
        /* Socket ownership was transferred to this PST connection. The PST
         * release is its authoritative public close root. */
        observer->socket_value.release_success_count++;
        observer->socket_value.reference_remaining = PAPACC_FALSE;
    }
}

static void ai_observer_init(AI_CASE_C_OBSERVER *observer)
{
    memset(observer, 0, sizeof(*observer));
    ai_resource_init(&observer->pst, "PST_CONNECTION", 1U);
    ai_resource_init(&observer->socket_value, "SITE_SOCKET", 2U);
    ai_resource_init(&observer->stream_value, "NETWORK_STREAM", 3U);
    ai_resource_init(&observer->data, "DATA_ATTACHMENT", 4U);
}

static int ai_install_close_observer(AI_CASE_C_OBSERVER *observer,
    PAPACC_NETWORK_STREAM *stream)
{
    void *before;
    if (observer == NULL || stream == NULL || stream->io.context == NULL ||
        stream->io.close == NULL || ai_active_observer != observer)
        return 0;
    before = stream->io.context;
    observer->delegate = stream->io;
    observer->stream = stream;
    observer->tls = (PAPACC_OUTBOUND_TLS *)before;
    observer->original_context = before;
    observer->stream_instance_id = stream->stream_instance_id;
    observer->channel_instance_id = stream->data_channel_instance_id;
    observer->owner_thread_id = GetCurrentThreadId();
    observer->context_before_original = PAPACC_TRUE;
    stream->io.close = ai_close;
    if (observer->observe_shutdown == PAPACC_TRUE) stream->io.shutdown = ai_shutdown;
    if (stream->io.context != before || stream->io.read != observer->delegate.read ||
        stream->io.write != observer->delegate.write ||
        stream->io.half_close != observer->delegate.half_close ||
        (observer->observe_shutdown != PAPACC_TRUE && stream->io.shutdown != observer->delegate.shutdown) ||
        stream->io.terminal_reason != observer->delegate.terminal_reason) {
        stream->io = observer->delegate;
        return 0;
    }
    observer->installed = PAPACC_TRUE;
    return 1;
}

static void ai_observe(PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,
    AI_CASE_C_OBSERVER *observer)
{
    PAPACC_NETWORK_STREAM_MANAGER *manager;
    PAPACC_SIZE index;
    PAPACC_BOOL data_present = PAPACC_FALSE;
    if (secure_io == NULL || observer == NULL) return;
    if (observer->installed == PAPACC_TRUE &&
        GetCurrentThreadId() != observer->owner_thread_id)
        observer->thread_violation = PAPACC_TRUE;
    observer->secure_io = secure_io;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    if(observer->p10_stop_request_after_ack==PAPACC_TRUE&&
       observer->p10_cause_after_failure==
            PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SERVER_STOP_REQUESTED&&
       secure_io->stop_cause==observer->p10_cause_after_failure&&
       secure_io->stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING)
        observer->cancel_cause_preserved=PAPACC_TRUE;
#endif
    if(observer->observe_shutdown==PAPACC_TRUE&&papacc_server_secure_io_win32_is_stopped(secure_io)==PAPACC_TRUE)
        observer->owner_stopped=PAPACC_TRUE;
    manager = &secure_io->protocol_loop->network_stream_manager;
    if (observer->installed != PAPACC_TRUE) {
        for (index = 0U; index < manager->capacity; ++index) {
            PAPACC_NETWORK_STREAM *stream = &manager->storage[index];
            PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot = NULL;
            PAPACC_SIZE slot_index;
            if (stream->state != PAPACC_NETWORK_STREAM_OPEN ||
                stream->transport_mode != PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)
                continue;
            for (slot_index = 0U;
                    slot_index < secure_io->protocol_loop->processor_capacity;
                    ++slot_index) {
                PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *candidate =
                    &secure_io->protocol_loop->processor_slots[slot_index];
                if (candidate->in_use == PAPACC_TRUE &&
                    candidate->kind ==
                        PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH &&
                    candidate->network_data_protocol.channel_instance_id ==
                        stream->data_channel_instance_id) {
                    slot = candidate;
                    break;
                }
            }
            if (slot == NULL || stream->io.context == NULL) continue;
            observer->tls = (PAPACC_OUTBOUND_TLS *)stream->io.context;
            if (observer->tls->processor.connection == NULL ||
                observer->tls->processor.state <
                    PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED)
                continue;
            if (!ai_install_close_observer(observer, stream)) continue;
            observer->connection_instance_id = slot->connection_instance_id;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            if(observer->p10_fixture!=NULL){
                observer->p10_fixture->p10_connection_instance_id=
                    observer->connection_instance_id;
                observer->p10_fixture->p10_stream_generation=
                    observer->stream_instance_id;
            }
            for (slot_index = 0U;
                    slot_index < PAPACC_SERVER_NETWORK_STREAM_CAPACITY;
                    ++slot_index) {
                PAPACC_SERVER_OUTBOUND_READY_WIN32 *ready =
                    &secure_io->outbound_ready[slot_index];
                if (ready->registered == PAPACC_TRUE &&
                    ready->stream_instance_id == stream->stream_instance_id &&
                    ready->connection == observer->tls->processor.connection) {
                    observer->p9_real_waitset_membership = PAPACC_TRUE;
                    break;
                }
            }
            {
                char p10_mode[32];
                DWORD p10_length=GetEnvironmentVariableA(
                    "PAPACC_ADR0032_P10",p10_mode,(DWORD)sizeof(p10_mode));
                if((p10_length==6U&&memcmp(p10_mode,"normal",6U)==0)||
                   (p10_length==11U&&memcmp(p10_mode,"normal-once",11U)==0)){
                    observer->p10_remove_mode=
                        PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE;
                    observer->p10_normal_only=PAPACC_TRUE;
                }else if(p10_length==17U&&
                         memcmp(p10_mode,"normal-persistent",17U)==0){
                    observer->p10_remove_mode=
                        PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT;
                    observer->p10_normal_only=PAPACC_TRUE;
                }else if(p10_length==4U&&memcmp(p10_mode,"once",4U)==0)
                    observer->p10_remove_mode=
                        PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE;
                else if(p10_length==10U&&memcmp(p10_mode,"persistent",10U)==0)
                    observer->p10_remove_mode=
                        PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT;
                if(observer->p10_remove_mode!=
                        PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF){
                    LARGE_INTEGER p10_counter;
                    (void)QueryPerformanceCounter(&p10_counter);
                    observer->p10_membership_before=
                        secure_io->scheduler.member_count;
                    (void)sprintf_s(observer->p10_run_id,
                        sizeof(observer->p10_run_id),"p10-%lu-%llu",
                        (unsigned long)GetCurrentProcessId(),
                        (unsigned long long)p10_counter.QuadPart);
                    (void)sprintf_s(observer->p10_trace_path,
                        sizeof(observer->p10_trace_path),
                        "papacc-p10-owner-poll-%s.trace",
                        observer->p10_run_id);
                    if(observer->p10_fixture!=NULL)
                        (void)strcpy_s(observer->p10_fixture->p10_run_id,
                            sizeof(observer->p10_fixture->p10_run_id),
                            observer->p10_run_id);
                    if(papacc_server_secure_io_win32_test_set_remove_fault(
                            secure_io,observer->tls->processor.connection,
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                            p10_native_mode==0?PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF:
#endif
                            observer->p10_remove_mode,
                            observer->connection_instance_id,
                            observer->channel_instance_id,
                            observer->stream_instance_id)==PAPACC_RESULT_OK){
                        observer->p10_seam_configured=PAPACC_TRUE;
                    }
                }
            }
#endif
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
            if(p10_native_mode>=0)p10_native_checkpoint(observer,1U);
#endif
            observer->pst.acquired_count = 1U;
            observer->pst.reference_remaining = PAPACC_TRUE;
            observer->socket_value.acquired_count = 1U;
            observer->socket_value.reference_remaining = PAPACC_TRUE;
            observer->stream_value.acquired_count = 1U;
            observer->stream_value.reference_remaining = PAPACC_TRUE;
            observer->data.acquired_count = 1U;
            observer->data.reference_remaining = PAPACC_TRUE;
            break;
        }
    }
    if (observer->installed != PAPACC_TRUE) return;
    if (observer->stream_value.reference_remaining == PAPACC_TRUE &&
        observer->stream->state == PAPACC_NETWORK_STREAM_UNUSED) {
        observer->stream_value.release_success_count++;
        observer->stream_value.reference_remaining = PAPACC_FALSE;
    }
    for (index = 0U;
            index < secure_io->protocol_loop->processor_capacity; ++index) {
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot =
            &secure_io->protocol_loop->processor_slots[index];
        if (slot->in_use == PAPACC_TRUE &&
            slot->kind == PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH &&
            slot->network_data_protocol.channel_instance_id ==
                observer->channel_instance_id) {
            data_present = PAPACC_TRUE;
            break;
        }
    }
    if (observer->data.reference_remaining == PAPACC_TRUE &&
        data_present != PAPACC_TRUE) {
        observer->data.release_attempt_count++;
        observer->data.release_success_count++;
        observer->data.reference_remaining = PAPACC_FALSE;
    }
    for (index = 0U; index < secure_io->candidate_capacity; ++index) {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate =
            &secure_io->candidates[index];
        if (candidate->control_cascade_count == 1U &&
            candidate->control_close_result.origin ==
                PAPACC_CONTROL_CLOSE_ORIGIN_OWNER_FAILURE &&
            candidate->control_stream_cause ==
                PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_CONTROL_TRANSPORT_LOSS) {
            observer->control_loss_owner_observed = PAPACC_TRUE;
            observer->control_loss_cascade_count =
                candidate->control_cascade_count;
        }
    }
}

static int ai_resource_valid(const AI_RESOURCE_OBSERVATION *resource)
{
    if (resource->acquired_count == 0U)
        return resource->release_attempt_count == 0U &&
            resource->release_success_count == 0U &&
            resource->reference_remaining != PAPACC_TRUE;
    return resource->acquired_count == 1U &&
        resource->release_attempt_count == 1U &&
        resource->release_success_count == 1U &&
        resource->reference_remaining != PAPACC_TRUE;
}

static const char *ai_observed_terminal_cause(
    const AI_CASE_C_OBSERVER *observer)
{
    unsigned int cause_count = 0U;
    const char *cause = NULL;

    if (observer == NULL) return NULL;
    if (observer->deadline_cause_observed == PAPACC_TRUE) {
        cause = "SHUTDOWN_DEADLINE";
        cause_count++;
    }
    if (observer->cancel_cause_preserved == PAPACC_TRUE) {
        cause = "SERVER_STOP_REQUESTED";
        cause_count++;
    }
    if (observer->terminal_truncated == PAPACC_TRUE) {
        cause = "TLS_TRUNCATED";
        cause_count++;
    }
    if (observer->control_loss_cause_observed == PAPACC_TRUE) {
        cause = "CONTROL_TRANSPORT_LOSS";
        cause_count++;
    }
    return cause_count == 1U ? cause : NULL;
}

static int ai_observed_terminal_cause_is(
    const AI_CASE_C_OBSERVER *observer, const char *expected)
{
    const char *observed = ai_observed_terminal_cause(observer);
    return observed != NULL && expected != NULL &&
        strcmp(observed, expected) == 0;
}

static int ai_observer_valid(const AI_CASE_C_OBSERVER *observer)
{
    return observer->installed == PAPACC_TRUE &&
        observer->terminal_truncated == PAPACC_TRUE &&
        ai_observed_terminal_cause_is(observer, "TLS_TRUNCATED") &&
        observer->context_before_original == PAPACC_TRUE &&
        observer->context_during_unchanged == PAPACC_TRUE &&
        observer->context_after_owner_release == PAPACC_TRUE &&
        observer->thread_violation != PAPACC_TRUE &&
        observer->original_close_calls == 1U &&
        ai_resource_valid(&observer->pst) &&
        ai_resource_valid(&observer->socket_value) &&
        ai_resource_valid(&observer->stream_value) &&
        ai_resource_valid(&observer->data);
}

static int ai_d_observer_valid(const AI_CASE_C_OBSERVER *o)
{
    return o->installed==PAPACC_TRUE&&o->shutdown_started==PAPACC_TRUE&&o->shutdown_pending==PAPACC_TRUE&&
        o->shutdown_write_interest==PAPACC_TRUE&&o->pst_close_clean_observed==PAPACC_TRUE&&
        o->cancel_after_pending==PAPACC_TRUE&&o->stop_delivered_to_owner==PAPACC_TRUE&&
        o->owner_stopped==PAPACC_TRUE&&o->cancel_cause_preserved==PAPACC_TRUE&&
        ai_observed_terminal_cause_is(o,"SERVER_STOP_REQUESTED")&&
        o->wire_terminal_absent==PAPACC_TRUE&&o->terminal_truncated!=PAPACC_TRUE&&
        o->terminal_publications==0U&&o->terminal_publication_attempts==0U&&
        o->terminal_publication_successes==0U&&o->terminal_type==0U&&
        o->false_clean_success!=PAPACC_TRUE&&
        o->context_before_original==PAPACC_TRUE&&o->context_during_unchanged==PAPACC_TRUE&&
        o->context_after_owner_release==PAPACC_TRUE&&o->thread_violation!=PAPACC_TRUE&&
        o->original_close_calls==1U&&ai_resource_valid(&o->pst)&&ai_resource_valid(&o->socket_value)&&
        ai_resource_valid(&o->stream_value)&&ai_resource_valid(&o->data);
}

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
static int ai_g1_publication_oracle_valid(const AI_CASE_C_OBSERVER *o)
{
    const PAPACC_OWNER_TERMINAL_TEST_EVENT *attempt;
    const PAPACC_OWNER_TERMINAL_TEST_EVENT *failure;
    if(o==NULL)return 0;
    attempt=&o->owner_publication_attempt_event;
    failure=&o->owner_publication_failure_event;
    return o->owner_terminals_accepted==1U&&
        o->owner_terminals_suppressed==1U&&
        o->owner_effective_terminal==PAPACC_MESSAGE_TYPE_NETWORK_CLOSE&&
        o->owner_terminal_cause==PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE&&
        o->owner_publication_attempts==1U&&
        o->owner_publication_successes==0U&&
        o->owner_publication_failures==1U&&
        o->owner_publication_attempt_sequence>6U&&
        attempt->operation_id==o->channel_instance_id&&
        attempt->stream_generation==o->stream_instance_id&&
        attempt->terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_CLOSE&&
        attempt->terminal_cause==PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE&&
        attempt->publication_result==PAPACC_RESULT_OK&&
        attempt->internal_injection!=PAPACC_TRUE&&
        failure->operation_id==attempt->operation_id&&
        failure->stream_generation==attempt->stream_generation&&
        failure->terminal_type==attempt->terminal_type&&
        failure->publication_result==PAPACC_RESULT_INVALID_STATE&&
        failure->internal_injection!=PAPACC_TRUE&&
        o->owner_publication_pst_state==PAPACC_PST_SECURE_PROCESSOR_CLOSED&&
        o->owner_publication_data_state==PAPACC_NETWORK_DATA_TERMINAL&&
        o->owner_publication_remote_half_closed==PAPACC_TRUE;
}

static int ai_g1_publication_oracle_negatives(void)
{
    AI_CASE_C_OBSERVER baseline={0},candidate;
    baseline.channel_instance_id=2U;
    baseline.stream_instance_id=1U;
    baseline.owner_terminals_accepted=1U;
    baseline.owner_terminals_suppressed=1U;
    baseline.owner_effective_terminal=PAPACC_MESSAGE_TYPE_NETWORK_CLOSE;
    baseline.owner_terminal_cause=PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE;
    baseline.owner_publication_attempts=1U;
    baseline.owner_publication_failures=1U;
    baseline.owner_publication_attempt_sequence=7U;
    baseline.owner_publication_attempt_event.operation_id=2U;
    baseline.owner_publication_attempt_event.stream_generation=1U;
    baseline.owner_publication_attempt_event.terminal_type=
        PAPACC_MESSAGE_TYPE_NETWORK_CLOSE;
    baseline.owner_publication_attempt_event.publication_result=
        PAPACC_RESULT_OK;
    baseline.owner_publication_failure_event=
        baseline.owner_publication_attempt_event;
    baseline.owner_publication_failure_event.publication_result=
        PAPACC_RESULT_INVALID_STATE;
    baseline.owner_publication_pst_state=PAPACC_PST_SECURE_PROCESSOR_CLOSED;
    baseline.owner_publication_data_state=PAPACC_NETWORK_DATA_TERMINAL;
    baseline.owner_publication_remote_half_closed=PAPACC_TRUE;
    if(!ai_g1_publication_oracle_valid(&baseline))return 0;
    candidate=baseline;
    candidate.owner_publication_attempt_event.internal_injection=PAPACC_TRUE;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_publication_successes=2U;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_publication_attempt_event.operation_id=3U;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_publication_attempt_event.stream_generation=2U;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_publication_attempt_event.terminal_type=
        PAPACC_MESSAGE_TYPE_NETWORK_ABORT;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_terminal_cause=
        PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_PACC_PROTOCOL_FAILURE;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_terminals_accepted=2U;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_publication_remote_half_closed=PAPACC_FALSE;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    candidate=baseline;candidate.owner_publication_successes=1U;
    if(ai_g1_publication_oracle_valid(&candidate))return 0;
    return 1;
}

typedef struct AI_P9_DUMMY_IO {
    PAPACC_U32 close_count;
} AI_P9_DUMMY_IO;

typedef struct AI_P9_EVIDENCE {
    PAPACC_U64 operation_id;
    PAPACC_U64 old_generation;
    PAPACC_U64 current_generation;
    PAPACC_U64 connection_instance_id;
    PAPACC_RESULT late_generation_result;
    PAPACC_RESULT duplicate_result;
    PAPACC_RESULT after_teardown_result;
    PAPACC_RESULT other_operation_result;
    PAPACC_RESULT old_after_new_result;
    PAPACC_U32 publication_attempts_before;
    PAPACC_U32 publication_attempts_after;
    PAPACC_U32 publication_successes_before;
    PAPACC_U32 publication_successes_after;
    PAPACC_U32 pst_release_attempts_before;
    PAPACC_U32 pst_release_attempts_after;
    PAPACC_SIZE pst_connections_before;
    PAPACC_SIZE pst_connections_after;
    PAPACC_SIZE waitset_members_before;
    PAPACC_SIZE waitset_members_after;
    PAPACC_SIZE connections_before;
    PAPACC_SIZE connections_after;
    PAPACC_SIZE streams_before;
    PAPACC_SIZE streams_after;
    PAPACC_U32 accepted_before;
    PAPACC_U32 accepted_after;
    PAPACC_U32 suppressed_before;
    PAPACC_U32 suppressed_after;
    PAPACC_BOOL real_pst_connection;
    PAPACC_BOOL real_waitset_membership;
    PAPACC_BOOL stream_terminal_preserved;
    PAPACC_BOOL data_terminal_preserved;
    PAPACC_BOOL new_generation_preserved;
    PAPACC_BOOL dummy_closed_once;
} AI_P9_EVIDENCE;

static PAPACC_RESULT ai_p9_dummy_read(void *context,PAPACC_U8 *buffer,
    PAPACC_SIZE capacity,PAPACC_SIZE *read_size,PAPACC_BOOL *eof)
{
    (void)context;(void)buffer;(void)capacity;
    if(read_size==NULL||eof==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    *read_size=0U;*eof=PAPACC_FALSE;return PAPACC_RESULT_OK;
}

static PAPACC_RESULT ai_p9_dummy_write(void *context,const PAPACC_U8 *buffer,
    PAPACC_SIZE size,PAPACC_SIZE *written)
{
    (void)context;(void)buffer;
    if(written==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    *written=size;return PAPACC_RESULT_OK;
}

static PAPACC_RESULT ai_p9_dummy_half_close(void *context)
{
    (void)context;return PAPACC_RESULT_OK;
}

static void ai_p9_dummy_close(void *context)
{
    AI_P9_DUMMY_IO *dummy=(AI_P9_DUMMY_IO *)context;
    if(dummy!=NULL)dummy->close_count++;
}

static PAPACC_SIZE ai_p9_pst_connection_count(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io)
{
    PAPACC_SIZE index,count=0U;
    if(secure_io==NULL)return 0U;
    for(index=0U;index<PAPACC_SERVER_NETWORK_STREAM_CAPACITY;++index){
        const PAPACC_SERVER_OUTBOUND_READY_WIN32 *ready=
            &secure_io->outbound_ready[index];
        if(ready->registered==PAPACC_TRUE&&ready->connection!=NULL)count++;
    }
    return count;
}

static PAPACC_BOOL ai_p9_requested(void)
{
    char value[8];
    DWORD length=GetEnvironmentVariableA("PAPACC_ADR0032_P9",value,
        (DWORD)sizeof(value));
    return length==1U&&value[0]=='1'?PAPACC_TRUE:PAPACC_FALSE;
}

static int ai_p9_evidence_valid(const AI_P9_EVIDENCE *e)
{
    return e!=NULL&&e->operation_id!=0U&&e->old_generation!=0U&&
        e->current_generation!=0U&&e->current_generation!=e->old_generation&&
        e->connection_instance_id!=0U&&
        e->late_generation_result==PAPACC_RESULT_INVALID_STATE&&
        e->duplicate_result==PAPACC_RESULT_INVALID_STATE&&
        e->after_teardown_result==PAPACC_RESULT_INVALID_STATE&&
        e->other_operation_result==PAPACC_RESULT_INVALID_STATE&&
        e->old_after_new_result==PAPACC_RESULT_INVALID_STATE&&
        e->publication_attempts_after==e->publication_attempts_before&&
        e->publication_successes_after==e->publication_successes_before&&
        e->pst_release_attempts_after==e->pst_release_attempts_before&&
        e->pst_connections_after==e->pst_connections_before&&
        e->waitset_members_after==e->waitset_members_before&&
        e->connections_after==e->connections_before&&
        e->streams_after==e->streams_before&&
        e->accepted_after==e->accepted_before&&
        e->suppressed_after==e->suppressed_before&&
        e->real_pst_connection==PAPACC_TRUE&&
        e->real_waitset_membership==PAPACC_TRUE&&
        e->stream_terminal_preserved==PAPACC_TRUE&&
        e->data_terminal_preserved==PAPACC_TRUE&&
        e->new_generation_preserved==PAPACC_TRUE&&
        e->dummy_closed_once==PAPACC_TRUE;
}

static int ai_p9_oracle_negatives(const AI_P9_EVIDENCE *baseline)
{
    AI_P9_EVIDENCE candidate;
    if(!ai_p9_evidence_valid(baseline))return 0;
    candidate=*baseline;candidate.old_after_new_result=PAPACC_RESULT_OK;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.duplicate_result=PAPACC_RESULT_OK;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.after_teardown_result=PAPACC_RESULT_OK;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.other_operation_result=PAPACC_RESULT_OK;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.publication_attempts_after++;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.stream_terminal_preserved=PAPACC_FALSE;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.pst_release_attempts_after++;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.waitset_members_after++;
    if(ai_p9_evidence_valid(&candidate))return 0;
    candidate=*baseline;candidate.current_generation=baseline->old_generation;
    if(ai_p9_evidence_valid(&candidate))return 0;
    return 1;
}

static int ai_p9_run(PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,
    PAPACC_SERVER_IO_LOOP_WIN32 *loop,AI_CASE_C_OBSERVER *observer)
{
    AI_P9_EVIDENCE evidence;
    AI_P9_DUMMY_IO dummy={0U};
    PAPACC_NETWORK_STREAM_IO io=PAPACC_NETWORK_STREAM_IO_INITIALIZER;
    PAPACC_NETWORK_STREAM *new_stream=NULL;
    PAPACC_RESULT result;
    PAPACC_SIZE event_count_before;
    if(secure_io==NULL||loop==NULL||observer==NULL||
        observer->channel_instance_id==0U||observer->stream_instance_id==0U||
        observer->connection_instance_id==0U)return 0;
    memset(&evidence,0,sizeof(evidence));
    evidence.operation_id=observer->channel_instance_id;
    evidence.old_generation=observer->stream_instance_id;
    evidence.connection_instance_id=observer->connection_instance_id;
    evidence.publication_attempts_before=observer->owner_publication_attempts;
    evidence.publication_successes_before=observer->owner_publication_successes;
    evidence.pst_release_attempts_before=observer->pst.release_attempt_count;
    evidence.pst_connections_before=ai_p9_pst_connection_count(secure_io);
    evidence.waitset_members_before=secure_io->scheduler.member_count;
    evidence.connections_before=loop->connection_manager->count;
    evidence.streams_before=loop->network_stream_manager.count;
    evidence.accepted_before=observer->owner_terminals_accepted;
    evidence.suppressed_before=observer->owner_terminals_suppressed;
    evidence.real_pst_connection=observer->pst.acquired_count==1U&&
        observer->pst.release_success_count==1U?PAPACC_TRUE:PAPACC_FALSE;
    evidence.real_waitset_membership=observer->p9_real_waitset_membership;
    event_count_before=observer->owner_event_count;
    evidence.late_generation_result=
        papacc_server_io_loop_win32_test_inject_owner_terminal(loop,
            evidence.operation_id,evidence.old_generation,
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
    evidence.duplicate_result=
        papacc_server_io_loop_win32_test_inject_owner_terminal(loop,
            evidence.operation_id,evidence.old_generation,
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
    evidence.after_teardown_result=
        papacc_server_io_loop_win32_test_inject_owner_terminal(loop,
            evidence.operation_id,evidence.old_generation,
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
    evidence.other_operation_result=
        papacc_server_io_loop_win32_test_inject_owner_terminal(loop,
            evidence.operation_id+0x100000U,evidence.old_generation,
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
    result=papacc_network_stream_reserve(&loop->network_stream_manager,
        0x9001U,evidence.operation_id,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,
        &new_stream);
    if(result!=PAPACC_RESULT_OK||new_stream==NULL)return 0;
    io.context=&dummy;io.read=ai_p9_dummy_read;io.write=ai_p9_dummy_write;
    io.half_close=ai_p9_dummy_half_close;io.close=ai_p9_dummy_close;
    result=papacc_network_stream_publish(&loop->network_stream_manager,
        new_stream,&io);
    if(result!=PAPACC_RESULT_OK){
        papacc_network_stream_release_reservation(
            &loop->network_stream_manager,new_stream);return 0;
    }
    evidence.current_generation=new_stream->stream_instance_id;
    evidence.old_after_new_result=
        papacc_server_io_loop_win32_test_inject_owner_terminal(loop,
            evidence.operation_id,evidence.old_generation,
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
    evidence.new_generation_preserved=
        new_stream->state==PAPACC_NETWORK_STREAM_OPEN&&
        new_stream->stream_instance_id==evidence.current_generation&&
        new_stream->accepted_terminal_type==0U?PAPACC_TRUE:PAPACC_FALSE;
    if(papacc_network_stream_close_channel(&loop->network_stream_manager,
            evidence.operation_id)!=PAPACC_RESULT_OK)return 0;
    evidence.dummy_closed_once=dummy.close_count==1U?PAPACC_TRUE:PAPACC_FALSE;
    evidence.publication_attempts_after=observer->owner_publication_attempts;
    evidence.publication_successes_after=observer->owner_publication_successes;
    evidence.pst_release_attempts_after=observer->pst.release_attempt_count;
    evidence.pst_connections_after=ai_p9_pst_connection_count(secure_io);
    evidence.waitset_members_after=secure_io->scheduler.member_count;
    evidence.connections_after=loop->connection_manager->count;
    evidence.streams_after=loop->network_stream_manager.count;
    evidence.accepted_after=observer->owner_terminals_accepted;
    evidence.suppressed_after=observer->owner_terminals_suppressed;
    evidence.stream_terminal_preserved=
        loop->network_stream_manager.count==evidence.streams_before?
        PAPACC_TRUE:PAPACC_FALSE;
    evidence.data_terminal_preserved=observer->data.reference_remaining!=
        PAPACC_TRUE?PAPACC_TRUE:PAPACC_FALSE;
    if(observer->owner_event_count<event_count_before+6U)return 0;
    if(!ai_p9_evidence_valid(&evidence)||
        !ai_p9_oracle_negatives(&evidence))return 0;
    printf("P9_REAL_OWNER=YES P9_REAL_NETWORK_STREAM=YES P9_REAL_PST_CONNECTION=YES P9_REAL_WAITSET_MEMBERSHIP=YES\n");
    printf("P9_OPERATION_ID=%llu P9_STREAM_GENERATION=%llu P9_CONNECTION_ID=%llu P9_CURRENT_GENERATION=%llu OWNER_THREAD=%lu\n",
        (unsigned long long)evidence.operation_id,
        (unsigned long long)evidence.old_generation,
        (unsigned long long)evidence.connection_instance_id,
        (unsigned long long)evidence.current_generation,
        (unsigned long)observer->owner_thread_id);
    printf("P9_PUBLICATION_BEFORE=%lu P9_PUBLICATION_AFTER=%lu P9_PST_CONNECTION_COUNT_BEFORE=%lu P9_PST_CONNECTION_COUNT_AFTER=%lu P9_PST_RELEASE_ATTEMPTS_BEFORE=%lu P9_PST_RELEASE_ATTEMPTS_AFTER=%lu P9_WAITSET_MEMBERSHIP_BEFORE=%lu P9_WAITSET_MEMBERSHIP_AFTER=%lu\n",
        (unsigned long)evidence.publication_attempts_before,
        (unsigned long)evidence.publication_attempts_after,
        (unsigned long)evidence.pst_connections_before,
        (unsigned long)evidence.pst_connections_after,
        (unsigned long)evidence.pst_release_attempts_before,
        (unsigned long)evidence.pst_release_attempts_after,
        (unsigned long)evidence.waitset_members_before,
        (unsigned long)evidence.waitset_members_after);
    printf("P9_LATE_CALLBACK_REJECTED=YES P9_PUBLICATION_DELTA=%ld P9_PST_CONNECTION_DELTA=%ld P9_PST_RELEASE_DELTA=%ld P9_WAITSET_MEMBERSHIP_DELTA=%ld P9_NO_RESURRECTION=YES\n",
        (long)evidence.publication_attempts_after-(long)evidence.publication_attempts_before,
        (long)evidence.pst_connections_after-(long)evidence.pst_connections_before,
        (long)evidence.pst_release_attempts_after-(long)evidence.pst_release_attempts_before,
        (long)evidence.waitset_members_after-(long)evidence.waitset_members_before);
    puts("P9_CALLBACKS=OLD_GENERATION_REJECTED,DUPLICATE_REJECTED,AFTER_TEARDOWN_REJECTED,OTHER_OPERATION_REJECTED,OLD_AFTER_NEW_GENERATION_REJECTED");
    puts("P9_NEGATIVES=9_OF_9_PASS P9_QUALIFIED=YES");
    return 1;
}
static int ai_p10_normal_evidence_valid(const AI_CASE_C_OBSERVER *o,
    const PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *observation)
{
    return o!=NULL&&observation!=NULL&&o->secure_io!=NULL&&
        o->p10_normal_only==PAPACC_TRUE&&
        o->p10_seam_configured==PAPACC_TRUE&&
        (o->p10_remove_mode==PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE||
         o->p10_remove_mode==
            PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT)&&
        o->secure_io->test_remove_fault_mode==o->p10_remove_mode&&
        o->secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_RUNNING&&
        observation->connection_instance_id==o->connection_instance_id&&
        observation->operation_id==o->channel_instance_id&&
        observation->stream_generation==o->stream_instance_id&&
        observation->owner_thread_id==o->owner_thread_id&&
        observation->injected_failure_count==0U&&
        observation->stop_remove_attempt_count==0U&&
        observation->normal_remove_attempt_count>0U&&
        observation->normal_remove_success_count==
            observation->normal_remove_attempt_count&&
        observation->real_remove_call_count==
            observation->normal_remove_attempt_count&&
        observation->real_remove_success_count==
            observation->normal_remove_success_count&&
        observation->dispatch_active_at_attempt!=PAPACC_TRUE&&
        o->p10_membership_before>o->p10_membership_after_normal&&
        o->p10_selected_membership_after_normal!=PAPACC_TRUE&&
        o->owner_terminals_accepted==0U&&
        o->owner_terminals_suppressed==0U&&
        o->owner_publication_attempts==1U&&
        o->owner_publication_successes==1U&&
        o->owner_publication_failures==0U&&
        o->owner_publication_attempt_event.operation_id==o->channel_instance_id&&
        o->owner_publication_attempt_event.stream_generation==
            o->stream_instance_id&&
        o->owner_publication_attempt_event.terminal_type==
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE&&
        o->owner_publication_attempt_event.terminal_cause==
            PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE&&
        o->owner_publication_success_event.operation_id==o->channel_instance_id&&
        o->owner_publication_success_event.stream_generation==
            o->stream_instance_id&&
        o->owner_publication_success_event.terminal_type==
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE&&
        o->owner_publication_success_event.terminal_cause==
            PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE&&
        o->owner_publication_success_event.publication_result==PAPACC_RESULT_OK&&
        o->owner_publication_success_event.writer_status==
            PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE&&
        o->owner_publication_attempt_sequence>0U&&
        o->owner_publication_success_sequence>
            o->owner_publication_attempt_sequence&&
        o->owner_publication_pst_state==PAPACC_PST_SECURE_PROCESSOR_CLOSED&&
        o->owner_publication_remote_half_closed==PAPACC_TRUE&&
        o->owner_publication_attempt_remove_successes>0U&&
        o->owner_publication_success_remove_successes==
            o->owner_publication_attempt_remove_successes&&
        o->owner_publication_attempt_release_attempts==0U&&
        o->owner_publication_success_release_attempts==0U&&
        o->pst.release_attempt_count==1U&&
        ai_resource_valid(&o->pst)&&ai_resource_valid(&o->socket_value)&&
        ai_resource_valid(&o->stream_value)&&ai_resource_valid(&o->data);
}

static int ai_p10_normal_evidence_diagnose(AI_CASE_C_OBSERVER *o,
    const PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *observation)
{
    int all_valid=1;
#define AI_P10_CHECK(name,condition,expected,observed_format,observed_value) \
    do { int evaluated=(condition)?1:0; \
        printf("CHECK115_SUBCONDITION=%s EXPECTED=%s OBSERVED=" \
            observed_format " EVALUATED=YES RESULT=%s\n",name,expected, \
            observed_value,evaluated?"PASS":"FAIL"); \
        if(!evaluated&&o->p10_check115_first_failed==NULL){ \
            o->p10_check115_first_failed=name; \
            (void)sprintf_s(o->p10_check115_expected, \
                sizeof(o->p10_check115_expected),"%s",expected); \
            (void)sprintf_s(o->p10_check115_observed, \
                sizeof(o->p10_check115_observed),observed_format, \
                observed_value); } \
        if(!evaluated)all_valid=0; } while(0)
    if(o==NULL||observation==NULL)return 0;
    o->p10_check115_first_failed=NULL;
    o->p10_check115_expected[0]='\0';
    o->p10_check115_observed[0]='\0';
    AI_P10_CHECK("OBSERVER_SECURE_IO_PRESENT",o->secure_io!=NULL,"YES","%s",o->secure_io!=NULL?"YES":"NO");
    AI_P10_CHECK("NORMAL_ONLY",o->p10_normal_only==PAPACC_TRUE,"TRUE","%u",(unsigned)o->p10_normal_only);
    AI_P10_CHECK("SEAM_CONFIGURED",o->p10_seam_configured==PAPACC_TRUE,"TRUE","%u",(unsigned)o->p10_seam_configured);
    AI_P10_CHECK("REMOVE_MODE_ALLOWED",o->p10_remove_mode==PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE||o->p10_remove_mode==PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT,"ONCE_OR_PERSISTENT","%u",(unsigned)o->p10_remove_mode);
    AI_P10_CHECK("SEAM_MODE_MATCH",o->secure_io!=NULL&&o->secure_io->test_remove_fault_mode==o->p10_remove_mode,"MATCH","%u",o->secure_io!=NULL?(unsigned)o->secure_io->test_remove_fault_mode:0U);
    AI_P10_CHECK("STOP_STATE_RUNNING",o->secure_io!=NULL&&o->secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_RUNNING,"RUNNING","%u",o->secure_io!=NULL?(unsigned)o->secure_io->stop_state:0U);
    AI_P10_CHECK("CONNECTION_ID_MATCH",observation->connection_instance_id==o->connection_instance_id,"MATCH","%llu",(unsigned long long)observation->connection_instance_id);
    AI_P10_CHECK("OPERATION_ID_MATCH",observation->operation_id==o->channel_instance_id,"MATCH","%llu",(unsigned long long)observation->operation_id);
    AI_P10_CHECK("STREAM_GENERATION_MATCH",observation->stream_generation==o->stream_instance_id,"MATCH","%llu",(unsigned long long)observation->stream_generation);
    AI_P10_CHECK("OWNER_THREAD_MATCH",observation->owner_thread_id==o->owner_thread_id,"MATCH","%lu",(unsigned long)observation->owner_thread_id);
    AI_P10_CHECK("INJECTED_FAILURES_ZERO",observation->injected_failure_count==0U,"0","%lu",(unsigned long)observation->injected_failure_count);
    AI_P10_CHECK("STOP_REMOVE_ATTEMPTS_ZERO",observation->stop_remove_attempt_count==0U,"0","%lu",(unsigned long)observation->stop_remove_attempt_count);
    AI_P10_CHECK("NORMAL_REMOVE_ATTEMPTS_POSITIVE",observation->normal_remove_attempt_count>0U,">0","%lu",(unsigned long)observation->normal_remove_attempt_count);
    AI_P10_CHECK("NORMAL_REMOVE_SUCCESSES_MATCH_ATTEMPTS",observation->normal_remove_success_count==observation->normal_remove_attempt_count,"ATTEMPTS","%lu",(unsigned long)observation->normal_remove_success_count);
    AI_P10_CHECK("REAL_REMOVE_CALLS_MATCH_NORMAL_ATTEMPTS",observation->real_remove_call_count==observation->normal_remove_attempt_count,"NORMAL_ATTEMPTS","%lu",(unsigned long)observation->real_remove_call_count);
    AI_P10_CHECK("REAL_REMOVE_SUCCESSES_MATCH_NORMAL_SUCCESSES",observation->real_remove_success_count==observation->normal_remove_success_count,"NORMAL_SUCCESSES","%lu",(unsigned long)observation->real_remove_success_count);
    AI_P10_CHECK("NO_DISPATCH_MUTATION",observation->dispatch_active_at_attempt!=PAPACC_TRUE,"FALSE","%u",(unsigned)observation->dispatch_active_at_attempt);
    AI_P10_CHECK("MEMBERSHIP_DECREASED",o->p10_membership_before>o->p10_membership_after_normal,"BEFORE>AFTER","%lu",(unsigned long)o->p10_membership_after_normal);
    AI_P10_CHECK("SELECTED_MEMBERSHIP_CLEARED",o->p10_selected_membership_after_normal!=PAPACC_TRUE,"FALSE","%u",(unsigned)o->p10_selected_membership_after_normal);
    AI_P10_CHECK("OWNER_TERMINAL_ACCEPTED_ZERO",o->owner_terminals_accepted==0U,"0","%lu",(unsigned long)o->owner_terminals_accepted);
    AI_P10_CHECK("OWNER_TERMINAL_SUPPRESSED_ZERO",o->owner_terminals_suppressed==0U,"0","%lu",(unsigned long)o->owner_terminals_suppressed);
    AI_P10_CHECK("PUBLICATION_ATTEMPTS_ONE",o->owner_publication_attempts==1U,"1","%lu",(unsigned long)o->owner_publication_attempts);
    AI_P10_CHECK("PUBLICATION_SUCCESSES_ONE",o->owner_publication_successes==1U,"1","%lu",(unsigned long)o->owner_publication_successes);
    AI_P10_CHECK("PUBLICATION_FAILURES_ZERO",o->owner_publication_failures==0U,"0","%lu",(unsigned long)o->owner_publication_failures);
    AI_P10_CHECK("PUBLICATION_OPERATION_MATCH",o->owner_publication_attempt_event.operation_id==o->channel_instance_id,"MATCH","%llu",(unsigned long long)o->owner_publication_attempt_event.operation_id);
    AI_P10_CHECK("PUBLICATION_GENERATION_MATCH",o->owner_publication_attempt_event.stream_generation==o->stream_instance_id,"MATCH","%llu",(unsigned long long)o->owner_publication_attempt_event.stream_generation);
    AI_P10_CHECK("PUBLICATION_TYPE_NETWORK_CLOSE",o->owner_publication_attempt_event.terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,"NETWORK_CLOSE","%u",(unsigned)o->owner_publication_attempt_event.terminal_type);
    AI_P10_CHECK("PUBLICATION_CAUSE_NONE",o->owner_publication_attempt_event.terminal_cause==PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE,"NONE","%u",(unsigned)o->owner_publication_attempt_event.terminal_cause);
    AI_P10_CHECK("PUBLICATION_SUCCESS_IDENTITY",o->owner_publication_success_event.operation_id==o->channel_instance_id&&o->owner_publication_success_event.stream_generation==o->stream_instance_id,"MATCH","%llu",(unsigned long long)o->owner_publication_success_event.operation_id);
    AI_P10_CHECK("PUBLICATION_RESULT_OK",o->owner_publication_success_event.publication_result==PAPACC_RESULT_OK,"OK","%d",(int)o->owner_publication_success_event.publication_result);
    AI_P10_CHECK("PUBLICATION_FRAME_COMPLETE",o->owner_publication_success_event.writer_status==PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE,"FRAME_COMPLETE","%u",(unsigned)o->owner_publication_success_event.writer_status);
    AI_P10_CHECK("PST_CLOSED_BEFORE_PUBLICATION",o->owner_publication_pst_state==PAPACC_PST_SECURE_PROCESSOR_CLOSED,"CLOSED","%u",(unsigned)o->owner_publication_pst_state);
    AI_P10_CHECK("WAITSET_REMOVED_BEFORE_PUBLICATION",o->owner_publication_attempt_remove_successes>0U&&o->owner_publication_success_remove_successes==o->owner_publication_attempt_remove_successes,">0_STABLE","%lu",(unsigned long)o->owner_publication_success_remove_successes);
    AI_P10_CHECK("PUBLICATION_SEQUENCE_ORDER",o->owner_publication_attempt_sequence>0U&&o->owner_publication_success_sequence>o->owner_publication_attempt_sequence,"ATTEMPT<SUCCESS","%lu",(unsigned long)o->owner_publication_success_sequence);
    AI_P10_CHECK("RELEASE_AFTER_PUBLICATION",o->owner_publication_attempt_release_attempts==0U&&o->owner_publication_success_release_attempts==0U&&o->pst.release_attempt_count==1U,"0,0,1","%lu",(unsigned long)o->pst.release_attempt_count);
    AI_P10_CHECK("PST_RESOURCE_VALID",ai_resource_valid(&o->pst),"TRUE","%u",(unsigned)ai_resource_valid(&o->pst));
    AI_P10_CHECK("SOCKET_RESOURCE_VALID",ai_resource_valid(&o->socket_value),"TRUE","%u",(unsigned)ai_resource_valid(&o->socket_value));
    AI_P10_CHECK("STREAM_RESOURCE_VALID",ai_resource_valid(&o->stream_value),"TRUE","%u",(unsigned)ai_resource_valid(&o->stream_value));
    AI_P10_CHECK("DATA_RESOURCE_VALID",ai_resource_valid(&o->data),"TRUE","%u",(unsigned)ai_resource_valid(&o->data));
#undef AI_P10_CHECK
    return all_valid;
}
static int ai_p10_normal_valid(AI_CASE_C_OBSERVER *o,
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *out_observation)
{
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION observation;
    if(o==NULL||o->secure_io==NULL||out_observation==NULL||
        papacc_server_secure_io_win32_test_get_remove_observation(
            o->secure_io,&observation)!=PAPACC_RESULT_OK)return 0;
    *out_observation=observation;
    {
        int valid=ai_p10_normal_evidence_diagnose(o,&observation);
        ai_p10_write_trace(o,valid?"CHECK115_PASS":"CHECK115_FAIL");
        printf("P10_RUN_ID=%s P10_TRACE_PATH=%s CHECK115_FIRST_FAILED_SUBCONDITION=%s CHECK115_EXPECTED=%s CHECK115_OBSERVED=%s\n",
            o->p10_run_id,o->p10_trace_path,
            o->p10_check115_first_failed!=NULL?
                o->p10_check115_first_failed:"NONE",
            o->p10_check115_expected[0]!='\0'?
                o->p10_check115_expected:"ALL_TRUE",
            o->p10_check115_observed[0]!='\0'?
                o->p10_check115_observed:"ALL_TRUE");
        return valid;
    }
}

static int ai_p10_normal_negatives(
    const PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *baseline,
    const AI_CASE_C_OBSERVER *observer)
{
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION candidate;
    AI_CASE_C_OBSERVER mutated;
    PAPACC_U32 passed=0U;
#define AI_P10_NORMAL_REJECT(statement) do { candidate=*baseline; \
    mutated=*observer; statement; \
    if(ai_p10_normal_evidence_valid(&mutated,&candidate))return 0; \
    ++passed; } while(0)
    AI_P10_NORMAL_REJECT(mutated.owner_terminals_accepted=1U);
    AI_P10_NORMAL_REJECT(mutated.owner_publication_successes=2U);
    AI_P10_NORMAL_REJECT(mutated.owner_publication_attempt_event.terminal_type=
        PAPACC_MESSAGE_TYPE_NETWORK_ABORT);
    AI_P10_NORMAL_REJECT(++mutated.owner_publication_attempt_event.operation_id);
    AI_P10_NORMAL_REJECT(mutated.owner_publication_attempt_remove_successes=0U;
        mutated.owner_publication_success_remove_successes=0U);
    AI_P10_NORMAL_REJECT(mutated.owner_publication_success_release_attempts=1U);
    AI_P10_NORMAL_REJECT(candidate.injected_failure_count=1U);
    AI_P10_NORMAL_REJECT(
        mutated.p10_selected_membership_after_normal=PAPACC_TRUE);
    AI_P10_NORMAL_REJECT(mutated.owner_publication_success_event.writer_status=
        PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK);
    AI_P10_NORMAL_REJECT(mutated.owner_terminals_suppressed=1U);
#undef AI_P10_NORMAL_REJECT
    return passed==10U;
}
static int ai_p10_normal_reporting_valid(const AI_CASE_C_OBSERVER *observer,
    const PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *observation,
    PAPACC_BOOL oracle_passed,PAPACC_BOOL assigned_after_oracle)
{
    return oracle_passed==PAPACC_TRUE&&assigned_after_oracle==PAPACC_TRUE&&
        ai_p10_normal_evidence_valid(observer,observation);
}

static int ai_p10_normal_reporting_negatives(
    const PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *baseline,
    const AI_CASE_C_OBSERVER *observer)
{
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION candidate;
    AI_CASE_C_OBSERVER mutated;
    PAPACC_U32 passed=0U;
#define AI_P10_REPORT_REJECT(statement,oracle_passed) do { \
    candidate=*baseline;mutated=*observer;statement; \
    if(ai_p10_normal_reporting_valid(&mutated,&candidate,oracle_passed, \
            PAPACC_TRUE))return 0;++passed; } while(0)
    AI_P10_REPORT_REJECT(mutated.owner_publication_attempts=0U,PAPACC_FALSE);
    AI_P10_REPORT_REJECT(mutated.owner_publication_attempt_event.terminal_cause=
        PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_PACC_PROTOCOL_FAILURE,PAPACC_FALSE);
    AI_P10_REPORT_REJECT(++mutated.owner_publication_attempt_event.operation_id,
        PAPACC_FALSE);
    AI_P10_REPORT_REJECT(mutated.owner_publication_successes=2U,PAPACC_FALSE);
    AI_P10_REPORT_REJECT(mutated.owner_publication_attempt_remove_successes=0U;
        mutated.owner_publication_success_remove_successes=0U,PAPACC_FALSE);
    AI_P10_REPORT_REJECT(mutated.owner_publication_success_event.writer_status=
        PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK,PAPACC_FALSE);
    AI_P10_REPORT_REJECT((void)0,PAPACC_FALSE);
#undef AI_P10_REPORT_REJECT
    return passed==7U;
}
static int ai_p10_once_evidence_valid(const AI_CASE_C_OBSERVER *o,
    const PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *observation)
{
    const REFERENCE_FIXTURE_THREAD *fixture;
    if(o==NULL||observation==NULL||o->secure_io==NULL||
        o->p10_fixture==NULL)return 0;
    fixture=o->p10_fixture;
    return o->p10_remove_mode==PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE&&
        o->p10_seam_configured==PAPACC_TRUE&&
        fixture->p10_marker_received==1L&&
        fixture->p10_marker_validated==1L&&
        fixture->p10_marker_signal_attempts==1L&&
        fixture->p10_marker_signal_successes==1L&&
        fixture->p10_marker_signal_error==0L&&
        fixture->p10_connection_instance_id==o->connection_instance_id&&
        fixture->p10_stream_generation==o->stream_instance_id&&
        strcmp(fixture->p10_run_id,o->p10_run_id)==0&&
        fixture->p10_marker_receive_sequence>0L&&
        fixture->p10_marker_signal_sequence>
            fixture->p10_marker_receive_sequence&&
        o->p10_marker_ack_sequence>fixture->p10_marker_signal_sequence&&
        o->p10_stop_request_sequence>o->p10_marker_ack_sequence&&
        o->p10_owner_stop_ack_signal_sequence>
            o->p10_stop_request_sequence&&
        fixture->p10_owner_stop_observed_sequence>
            o->p10_owner_stop_ack_signal_sequence&&
        o->p10_marker_ack_wait_result==WAIT_OBJECT_0&&
        o->p10_marker_ack_wait_error==ERROR_SUCCESS&&
        o->p10_marker_ack_observed==PAPACC_TRUE&&
        o->p10_stop_request_after_ack==PAPACC_TRUE&&
        o->p10_first_stop_result==PAPACC_RESULT_INTERNAL_ERROR&&
        o->p10_stop_state_at_ack==
            PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING&&
        o->p10_target_membership_at_ack==PAPACC_TRUE&&
        o->p10_owner_stop_ack_signal_attempts==1U&&
        o->p10_owner_stop_ack_signal_successes==1U&&
        o->p10_owner_stop_ack_signal_error==ERROR_SUCCESS&&
        fixture->p10_owner_stop_wait_result==(LONG)WAIT_OBJECT_0&&
        fixture->p10_owner_stop_wait_error==ERROR_SUCCESS&&
        fixture->p10_owner_stop_observed==1L&&
        fixture->diagnostic_shutdown_started==0L&&
        o->p10_stop_state_after_failure==
            PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING&&
        o->p10_membership_before==o->p10_membership_after_failure&&
        o->p10_membership_before>0U&&
        o->p10_connection_preserved==PAPACC_TRUE&&
        o->p10_processor_preserved==PAPACC_TRUE&&
        o->p10_release_attempts_after_failure==0U&&
        o->p10_destructive_close_attempts_after_failure==0U&&
        o->p10_deadline_after_failure!=0U&&
        o->secure_io->shutdown_deadline_ns==o->p10_deadline_after_failure&&
        o->p10_cause_after_failure==
            PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SERVER_STOP_REQUESTED&&
        o->secure_io->stop_cause==o->p10_cause_after_failure&&
        o->secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_STOPPED&&
        observation->connection_instance_id==o->connection_instance_id&&
        observation->operation_id==o->channel_instance_id&&
        observation->stream_generation==o->stream_instance_id&&
        observation->owner_thread_id==o->owner_thread_id&&
        observation->remove_attempt_count==
            observation->normal_remove_attempt_count+
            observation->stop_remove_attempt_count&&
        observation->real_remove_call_count==
            observation->remove_attempt_count-
            observation->injected_failure_count&&
        observation->real_remove_success_count==
            observation->real_remove_call_count&&
        observation->normal_remove_success_count==
            observation->normal_remove_attempt_count&&
        observation->target_stop_remove_attempt_count==2U&&
        observation->target_injected_failure_count==1U&&
        observation->target_real_remove_call_count==1U&&
        observation->target_real_remove_success_count==1U&&
        observation->other_stop_remove_attempt_count==0U&&
        observation->dispatch_active_at_attempt!=PAPACC_TRUE&&
        fixture->diagnostic_transport_terminal_observed==1L&&
        fixture->diagnostic_thread_exit==1L&&fixture->result!=0&&
        ai_resource_valid(&o->pst)&&ai_resource_valid(&o->socket_value)&&
        ai_resource_valid(&o->stream_value)&&ai_resource_valid(&o->data);
}

static int ai_p10_once_valid(const AI_CASE_C_OBSERVER *o,
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *out_observation)
{
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION observation;
    if(o==NULL||o->secure_io==NULL||out_observation==NULL||
        papacc_server_secure_io_win32_test_get_remove_observation(
            o->secure_io,&observation)!=PAPACC_RESULT_OK)return 0;
    *out_observation=observation;
    return ai_p10_once_evidence_valid(o,&observation);
}

static int ai_p10_negatives(
    const PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *baseline,
    const AI_CASE_C_OBSERVER *observer)
{
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION candidate;
    AI_CASE_C_OBSERVER mutated;
    REFERENCE_FIXTURE_THREAD fixture;
    PAPACC_U32 passed=0U;
#define AI_P10_REJECT(statement) do { candidate=*baseline;mutated=*observer; \
    fixture=*observer->p10_fixture;mutated.p10_fixture=&fixture;statement; \
    if(ai_p10_once_evidence_valid(&mutated,&candidate))return 0;++passed; \
    } while(0)
    AI_P10_REJECT(++candidate.target_stop_remove_attempt_count;
        ++candidate.normal_remove_attempt_count);
    AI_P10_REJECT(++candidate.other_stop_remove_attempt_count);
    AI_P10_REJECT(++candidate.stream_generation);
    AI_P10_REJECT(candidate.target_real_remove_call_count=0U;
        candidate.target_real_remove_success_count=0U);
    AI_P10_REJECT(candidate.target_real_remove_success_count=0U);
    AI_P10_REJECT(--mutated.p10_membership_after_failure);
    AI_P10_REJECT(mutated.p10_release_attempts_after_failure=1U);
    AI_P10_REJECT(mutated.p10_cause_after_failure=
        PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_PACC_PROTOCOL_FAILURE);
    AI_P10_REJECT(++mutated.p10_deadline_after_failure);
    AI_P10_REJECT(mutated.p10_stop_state_after_failure=
        PAPACC_SERVER_SECURE_STOP_STOPPED);
    AI_P10_REJECT(++candidate.remove_attempt_count);
    AI_P10_REJECT(mutated.stream_value.reference_remaining=PAPACC_TRUE);
#undef AI_P10_REJECT
    return passed==12U;
}
static int ai_g2_observer_valid(const AI_CASE_C_OBSERVER *o)
{
    PAPACC_BOOL publication_authorized=o->owner_publication_attempts==0U||
        (o->owner_publication_attempts==1U&&
         o->owner_publication_attempt_event.internal_injection!=PAPACC_TRUE&&
         o->owner_publication_attempt_event.operation_id==o->channel_instance_id&&
         o->owner_publication_attempt_event.stream_generation==o->stream_instance_id&&
         o->owner_publication_attempt_event.terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_ABORT);
    return o->installed==PAPACC_TRUE&&o->g2_mode==PAPACC_TRUE&&
        o->owner_events_received==2U&&o->owner_events_validated==2U&&
        o->owner_terminals_accepted==1U&&o->owner_terminals_suppressed==1U&&
        o->owner_effective_terminal==PAPACC_MESSAGE_TYPE_NETWORK_ABORT&&
        o->owner_terminal_cause==PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE&&
        publication_authorized&&o->owner_publication_successes<=1U&&
        o->g1_owner_injection_performed==PAPACC_TRUE&&
        o->g1_owner_injection_result==PAPACC_RESULT_INVALID_STATE&&
        o->owner_event_log[0].internal_injection!=PAPACC_TRUE&&
        o->owner_event_log[1].internal_injection!=PAPACC_TRUE&&
        o->owner_event_log[2].internal_injection!=PAPACC_TRUE&&
        o->owner_event_log[3].internal_injection==PAPACC_TRUE&&
        o->owner_event_log[4].internal_injection==PAPACC_TRUE&&
        o->owner_event_log[5].internal_injection==PAPACC_TRUE&&
        o->g1_ack_signal_successes==1U&&o->g1_site_release_signaled==PAPACC_TRUE&&
        o->context_before_original==PAPACC_TRUE&&
        o->context_during_unchanged==PAPACC_TRUE&&
        o->context_after_owner_release==PAPACC_TRUE&&
        o->thread_violation!=PAPACC_TRUE&&o->original_close_calls==1U&&
        ai_resource_valid(&o->pst)&&ai_resource_valid(&o->socket_value)&&
        ai_resource_valid(&o->stream_value)&&ai_resource_valid(&o->data);
}
static int ai_g1_observer_valid(const AI_CASE_C_OBSERVER *o)
{
    return o->installed==PAPACC_TRUE&&o->owner_events_received==2U&&
        o->owner_events_validated==2U&&o->owner_terminals_accepted==1U&&
        o->owner_terminals_suppressed==1U&&
        o->owner_effective_terminal==PAPACC_MESSAGE_TYPE_NETWORK_CLOSE&&
        ai_g1_publication_oracle_valid(o)&&
        o->g1_ack_signal_attempts==1U&&o->g1_ack_signal_successes==1U&&
        o->g1_owner_injection_performed==PAPACC_TRUE&&
        o->g1_owner_injection_result==PAPACC_RESULT_INVALID_STATE&&
        o->g1_data_terminal_after_first==PAPACC_TRUE&&
        o->g1_data_read_disabled_after_first==PAPACC_TRUE&&
        o->owner_event_log[0].internal_injection!=PAPACC_TRUE&&
        o->owner_event_log[1].internal_injection!=PAPACC_TRUE&&
        o->owner_event_log[2].internal_injection!=PAPACC_TRUE&&
        o->owner_event_log[3].internal_injection==PAPACC_TRUE&&
        o->owner_event_log[4].internal_injection==PAPACC_TRUE&&
        o->owner_event_log[5].internal_injection==PAPACC_TRUE&&
        o->g1_site_release_signaled==PAPACC_TRUE&&o->g1_fixture!=NULL&&
        o->g1_fixture->hold_open_wait_started==PAPACC_TRUE&&
        o->g1_fixture->hold_open_wait_result==WAIT_OBJECT_0&&
        o->context_before_original==PAPACC_TRUE&&
        o->context_during_unchanged==PAPACC_TRUE&&
        o->context_after_owner_release==PAPACC_TRUE&&
        o->thread_violation!=PAPACC_TRUE&&o->original_close_calls==1U&&
        ai_resource_valid(&o->pst)&&ai_resource_valid(&o->socket_value)&&
        ai_resource_valid(&o->stream_value)&&ai_resource_valid(&o->data);
}
#endif

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
static void ai_g1_write_diagnostics(FILE *out,const AI_CASE_C_OBSERVER *o,
    const PAPACC_SERVER_IO_LOOP_WIN32 *loop)
{
    PAPACC_SIZE event_index;
    const char *failed="NONE",*expected="PASS",*observed="PASS";
    PAPACC_BOOL first_received=o->owner_event_count>=1U&&o->owner_event_log[0].kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_RECEIVED;
    PAPACC_BOOL first_validated=o->owner_event_count>=2U&&o->owner_event_log[1].kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_VALIDATED&&o->owner_event_log[1].validation_result==PAPACC_RESULT_OK;
    PAPACC_BOOL first_accepted=o->owner_event_count>=3U&&o->owner_event_log[2].kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_ACCEPTED;
    PAPACC_BOOL second_received=o->owner_event_count>=4U&&o->owner_event_log[3].kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_RECEIVED;
    PAPACC_BOOL second_validated=o->owner_event_count>=5U&&o->owner_event_log[4].kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_VALIDATED&&o->owner_event_log[4].validation_result==PAPACC_RESULT_OK;
    PAPACC_BOOL second_suppressed=o->owner_event_count>=6U&&o->owner_event_log[5].kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_SUPPRESSED;
    PAPACC_BOOL no_resurrection=loop->network_stream_manager.count==0U&&
        o->pst.reference_remaining!=PAPACC_TRUE&&o->socket_value.reference_remaining!=PAPACC_TRUE&&
        o->stream_value.reference_remaining!=PAPACC_TRUE&&o->data.reference_remaining!=PAPACC_TRUE;
    fprintf(out,"RUN_ID=%s TRACE_PATH=%s TRACE_CREATION_RESULT=%s TRACE_FINALIZATION_RESULT=%s\n",
        o->g1_run_id,o->g1_trace_path,
        o->g1_trace_created==PAPACC_TRUE?"YES":"NO",
        o->g1_trace_finalized==PAPACC_TRUE?"YES":"PENDING");
    fprintf(out,"SITE_TLS_LIFETIME_CONTROL=%s SITE_TLS_WAIT_RESULT=%lu SITE_RELEASE_SIGNAL_ERROR=%lu SITE_TLS_RESULT=%d\n",
        o->g1_fixture!=NULL&&o->g1_fixture->hold_open_wait_started==PAPACC_TRUE?"ARMED":"NOT_ARMED",
        (unsigned long)(o->g1_fixture!=NULL?o->g1_fixture->hold_open_wait_result:WAIT_FAILED),
        (unsigned long)o->g1_site_release_error,
        o->g1_fixture!=NULL?o->g1_fixture->result:0);
    fprintf(out,"G2_FIXTURE_ABORT_MODE=%s G2_FIXTURE_TRANSPORT_CLOSE_OBSERVED=%s G2_FIXTURE_TLS_CLOSE_NOTIFY_OBSERVED=%s G2_FIXTURE_TIMEOUT=%s G2_FIXTURE_SHUTDOWN_OPERATION=%lu G2_FIXTURE_SHUTDOWN_SOURCE=%ld\n",
        o->g1_fixture!=NULL&&o->g1_fixture->abort_mode==PAPACC_TRUE?"YES":"NO",
        o->g1_fixture!=NULL&&o->g1_fixture->transport_close_observed==PAPACC_TRUE?"YES":"NO",
        o->g1_fixture!=NULL&&o->g1_fixture->tls_close_notify_observed==PAPACC_TRUE?"YES":"NO",
        o->g1_fixture!=NULL&&o->g1_fixture->abort_wait_timed_out==PAPACC_TRUE?"YES":"NO",
        (unsigned long)(o->g1_fixture!=NULL?o->g1_fixture->abort_operation:0U),
        (long)(o->g1_fixture!=NULL?o->g1_fixture->abort_source:PST_RESULT_OK));
    fprintf(out,"G1_OPERATION_ID=%llu G1_DATA_CHANNEL_ID=%llu G1_STREAM_GENERATION=%llu G1_OWNER_THREAD=%lu G1_INTERNAL_INJECTION=%s\n",
        (unsigned long long)(o->owner_event_count?o->owner_event_log[0].operation_id:0U),
        (unsigned long long)o->channel_instance_id,
        (unsigned long long)o->stream_instance_id,
        (unsigned long)(o->owner_event_count?o->owner_event_log[0].owner_thread_id:0U),
        o->owner_event_count&&o->owner_event_log[0].internal_injection==PAPACC_TRUE?"YES":"NO");
    fprintf(out,"G1_FIRST_CLOSE_RECEIVED=%s G1_FIRST_CLOSE_VALIDATED=%s G1_FIRST_CLOSE_ACCEPTED=%s\n",
        first_received?"YES":"NO",first_validated?"YES":"NO",first_accepted?"YES":"NO");
    fprintf(out,"G1_SECOND_CLOSE_RECEIVED=%s G1_SECOND_CLOSE_VALIDATED=%s G1_SECOND_CLOSE_SUPPRESSED=%s\n",
        second_received?"YES":"NO",second_validated?"YES":"NO",second_suppressed?"YES":"NO");
    fprintf(out,"G1_WIRE_DATA_TERMINAL=%s G1_WIRE_READ_DISABLED=%s G1_OWNER_DUPLICATE_INJECTED=%s G1_OWNER_INJECTION_RESULT=%d\n",
        o->g1_data_terminal_after_first==PAPACC_TRUE?"YES":"NO",
        o->g1_data_read_disabled_after_first==PAPACC_TRUE?"YES":"NO",
        o->g1_owner_injection_performed==PAPACC_TRUE?"YES":"NO",
        (int)o->g1_owner_injection_result);
    fprintf(out,"ACK_SIGNAL_ATTEMPT=%lu ACK_SIGNAL_SUCCESS=%lu ACK_SIGNAL_ERROR=%lu\n",
        (unsigned long)o->g1_ack_signal_attempts,
        (unsigned long)o->g1_ack_signal_successes,
        (unsigned long)o->g1_ack_signal_error);
    fprintf(out,"OWNER_TERMINAL_EVENTS=%lu OWNER_STREAM_STATE=%u OWNER_STREAM_COUNT=%lu OWNER_DATA_REFERENCE_REMAINING=%s\n",
        (unsigned long)o->owner_event_count,
        (unsigned)(o->owner_event_count?o->owner_event_log[o->owner_event_count-1U].owner_state:0U),
        (unsigned long)loop->network_stream_manager.count,
        o->data.reference_remaining==PAPACC_TRUE?"YES":"NO");
    fprintf(out,"G1_TERMINAL_ACCEPTED_COUNT=%lu G1_TERMINAL_SUPPRESSED_COUNT=%lu G1_PUBLICATION_ATTEMPTS=%lu G1_PUBLICATION_SUCCESSES=%lu G1_EFFECTIVE_TERMINAL=%s G1_TERMINAL_CAUSE=%u\n",
        (unsigned long)o->owner_terminals_accepted,(unsigned long)o->owner_terminals_suppressed,
        (unsigned long)o->owner_publication_attempts,(unsigned long)o->owner_publication_successes,
        o->owner_effective_terminal==PAPACC_MESSAGE_TYPE_NETWORK_CLOSE?"NETWORK_CLOSE":"OTHER",
        (unsigned)o->owner_terminal_cause);
    for(event_index=0U;event_index<o->owner_event_count;++event_index){
        const PAPACC_OWNER_TERMINAL_TEST_EVENT *event=&o->owner_event_log[event_index];
        fprintf(out,"OWNER_EVENT_SEQUENCE=%lu KIND=%u OPERATION_ID=%llu STREAM_GENERATION=%llu OWNER_THREAD=%lu OWNER_STATE=%u TERMINAL_TYPE=%u TERMINAL_CAUSE=%u VALIDATION_RESULT=%d PUBLICATION_RESULT=%d WRITER_STATUS=%u TRANSPORT_STATUS_AVAILABLE=%s TRANSPORT_STATUS=%u INTERNAL_INJECTION=%s\n",
            (unsigned long)(event_index+1U),(unsigned)event->kind,
            (unsigned long long)event->operation_id,
            (unsigned long long)event->stream_generation,
            (unsigned long)event->owner_thread_id,(unsigned)event->owner_state,
            (unsigned)event->terminal_type,(unsigned)event->terminal_cause,
            (int)event->validation_result,(int)event->publication_result,
            (unsigned)event->writer_status,
            event->transport_status_available==PAPACC_TRUE?"YES":"NO",
            (unsigned)event->transport_status,
            event->internal_injection==PAPACC_TRUE?"YES":"NO");
    }
    fprintf(out,"PUBLICATION_ATTEMPT_SEQUENCE=%lu PUBLICATION_CALL_SITE=progress_network_control PUBLICATION_OPERATION_ID=%llu PUBLICATION_STREAM_GENERATION=%llu PUBLICATION_OWNER_THREAD=%lu PUBLICATION_TERMINAL_TYPE=%u PUBLICATION_CAUSE=%u PUBLICATION_PACC_RESULT=%d PUBLICATION_WRITER_STATUS=%u PUBLICATION_TRANSPORT_STATUS_AVAILABLE=%s PUBLICATION_TRANSPORT_STATUS=%u\n",
        (unsigned long)o->owner_publication_attempt_sequence,
        (unsigned long long)o->owner_publication_attempt_event.operation_id,
        (unsigned long long)o->owner_publication_attempt_event.stream_generation,
        (unsigned long)o->owner_publication_attempt_event.owner_thread_id,
        (unsigned)o->owner_publication_attempt_event.terminal_type,
        (unsigned)o->owner_publication_attempt_event.terminal_cause,
        (int)o->owner_publication_attempt_event.publication_result,
        (unsigned)o->owner_publication_attempt_event.writer_status,
        o->owner_publication_attempt_event.transport_status_available==PAPACC_TRUE?"YES":"NO",
        (unsigned)o->owner_publication_attempt_event.transport_status);
    fprintf(out,"PUBLICATION_FAILURE_COUNT=%lu PUBLICATION_FAILURE_PACC_RESULT=%d PUBLICATION_FAILURE_WRITER_STATUS=%u PUBLICATION_FAILURE_TRANSPORT_STATUS_AVAILABLE=%s PUBLICATION_FAILURE_TRANSPORT_STATUS=%u PST_STATE_AT_ATTEMPT=%u DATA_STATE_AT_ATTEMPT=%u REMOTE_HALF_CLOSED_AT_ATTEMPT=%s\n",
        (unsigned long)o->owner_publication_failures,
        (int)o->owner_publication_failure_event.publication_result,
        (unsigned)o->owner_publication_failure_event.writer_status,
        o->owner_publication_failure_event.transport_status_available==PAPACC_TRUE?"YES":"NO",
        (unsigned)o->owner_publication_failure_event.transport_status,
        (unsigned)o->owner_publication_pst_state,
        (unsigned)o->owner_publication_data_state,
        o->owner_publication_remote_half_closed==PAPACC_TRUE?"YES":"NO");
    fprintf(out,"G1_PST_RELEASE_COUNT=%lu G1_SOCKET_CLOSE_COUNT=%lu G1_STREAM_DESTROY_COUNT=%lu G1_DATA_DETACH_COUNT=%lu G1_NO_RESURRECTION=%s\n",
        (unsigned long)o->pst.release_success_count,(unsigned long)o->socket_value.release_success_count,
        (unsigned long)o->stream_value.release_success_count,(unsigned long)o->data.release_success_count,
        no_resurrection?"YES":"NO");
    if(o->installed!=PAPACC_TRUE){failed="OBSERVER_INSTALLED";observed="NO";}
    else if(o->g1_fixture==NULL||o->g1_fixture->hold_open_wait_started!=PAPACC_TRUE){failed="SITE_TLS_WAIT_ARMED";observed="NO";}
    else if(!first_received){failed="FIRST_CLOSE_RECEIVED";observed="NO";}
    else if(!first_validated){failed="FIRST_CLOSE_VALIDATED";observed="NO";}
    else if(!first_accepted){failed="FIRST_CLOSE_ACCEPTED";observed="NO";}
    else if(!second_received){failed="SECOND_CLOSE_RECEIVED";observed="NO";}
    else if(!second_validated){failed="SECOND_CLOSE_VALIDATED";observed="NO";}
    else if(!second_suppressed){failed="SECOND_CLOSE_SUPPRESSED";observed="NO";}
    else if(o->g1_site_release_signaled!=PAPACC_TRUE){failed="SITE_RELEASE_SIGNALED";observed="NO";}
    else if(o->g1_fixture->hold_open_wait_result!=WAIT_OBJECT_0){failed="SITE_TLS_WAIT_RESULT";expected="WAIT_OBJECT_0";observed="OTHER";}
    else if(o->owner_terminals_accepted!=1U){failed="TERMINAL_ACCEPTED_COUNT";expected="1";observed="NOT_1";}
    else if(o->owner_terminals_suppressed!=1U){failed="TERMINAL_SUPPRESSED_COUNT";expected="1";observed="NOT_1";}
    else if(o->owner_effective_terminal!=PAPACC_MESSAGE_TYPE_NETWORK_CLOSE){failed="EFFECTIVE_TERMINAL";expected="NETWORK_CLOSE";observed="OTHER";}
    else if(!ai_g1_publication_oracle_valid(o)){failed="PUBLICATION_AUTHORITY";expected="PERMITTED_SITE_CLEAN_CLOSE_ATTEMPT";observed="INVALID";}
    else if(o->context_before_original!=PAPACC_TRUE){failed="IO_CONTEXT_BEFORE";observed="CHANGED";}
    else if(o->context_during_unchanged!=PAPACC_TRUE){failed="IO_CONTEXT_DURING";observed="CHANGED";}
    else if(o->context_after_owner_release!=PAPACC_TRUE){failed="IO_CONTEXT_AFTER";observed="CHANGED";}
    else if(o->thread_violation==PAPACC_TRUE){failed="OWNER_THREAD_AFFINITY";observed="VIOLATED";}
    else if(o->original_close_calls!=1U){failed="ORIGINAL_CLOSE_CALLS";expected="1";observed="NOT_1";}
    else if(!ai_resource_valid(&o->pst)){failed="PST_RELEASE";observed="INVALID";}
    else if(!ai_resource_valid(&o->socket_value)){failed="SOCKET_CLOSE";observed="INVALID";}
    else if(!ai_resource_valid(&o->stream_value)){failed="STREAM_DESTROY";observed="INVALID";}
    else if(!ai_resource_valid(&o->data)){failed="DATA_DETACH";observed="INVALID";}
    else if(!no_resurrection){failed="NO_RESURRECTION";observed="NO";}
    fprintf(out,"FIRST_FAILED_G1_SUBCONDITION=%s EXPECTED_VALUE=%s OBSERVED_VALUE=%s EVIDENCE_SOURCE=OWNER_HOOK_AND_RESOURCE_OBSERVER\n",failed,expected,observed);
    fflush(out);
}

static void ai_g1_emit_diagnostics(AI_CASE_C_OBSERVER *o,
    const PAPACC_SERVER_IO_LOOP_WIN32 *loop)
{
    FILE *trace=NULL;
    if(fopen_s(&trace,o->g1_trace_path,"wb")==0&&trace!=NULL){
        o->g1_trace_created=PAPACC_TRUE;
        ai_g1_write_diagnostics(trace,o,loop);
        if(fflush(trace)==0){
            o->g1_trace_finalized=PAPACC_TRUE;
            fprintf(trace,"TRACE_FINALIZATION_RESULT=YES\n");
            (void)fflush(trace);
        }
        (void)fclose(trace);
    }
    ai_g1_write_diagnostics(stdout,o,loop);
}
#endif

static int ai_f_observer_valid(const AI_CASE_C_OBSERVER *o)
{
    return o->installed==PAPACC_TRUE&&o->shutdown_started==PAPACC_TRUE&&
        o->shutdown_pending==PAPACC_TRUE&&o->shutdown_write_interest==PAPACC_TRUE&&
        o->pst_close_clean_observed==PAPACC_TRUE&&o->control_loss_signaled==PAPACC_TRUE&&
        o->control_loss_acknowledged==PAPACC_TRUE&&
        o->control_loss_cause_observed==PAPACC_TRUE&&
        o->control_loss_owner_observed==PAPACC_TRUE&&
        o->control_loss_cascade_count==1U&&
        ai_observed_terminal_cause_is(o,"CONTROL_TRANSPORT_LOSS")&&
        o->false_clean_success!=PAPACC_TRUE&&o->terminal_truncated!=PAPACC_TRUE&&
        o->context_before_original==PAPACC_TRUE&&o->context_during_unchanged==PAPACC_TRUE&&
        o->context_after_owner_release==PAPACC_TRUE&&o->thread_violation!=PAPACC_TRUE&&
        o->original_close_calls==1U&&ai_resource_valid(&o->pst)&&
        ai_resource_valid(&o->socket_value)&&ai_resource_valid(&o->stream_value)&&
        ai_resource_valid(&o->data);
}

static int ai_e_observer_valid(const AI_CASE_C_OBSERVER *o)
{
    return o->installed==PAPACC_TRUE&&o->shutdown_started==PAPACC_TRUE&&
        o->shutdown_pending==PAPACC_TRUE&&o->shutdown_write_interest==PAPACC_TRUE&&
        o->pst_close_clean_observed==PAPACC_TRUE&&o->deadline_armed==PAPACC_TRUE&&
        o->deadline_cause_observed==PAPACC_TRUE&&
        ai_observed_terminal_cause_is(o,"SHUTDOWN_DEADLINE")&&
        o->cancel_after_pending!=PAPACC_TRUE&&o->stop_delivered_to_owner!=PAPACC_TRUE&&
        o->false_clean_success!=PAPACC_TRUE&&o->terminal_truncated!=PAPACC_TRUE&&
        o->context_before_original==PAPACC_TRUE&&o->context_during_unchanged==PAPACC_TRUE&&
        o->context_after_owner_release==PAPACC_TRUE&&o->thread_violation!=PAPACC_TRUE&&
        o->original_close_calls==1U&&ai_resource_valid(&o->pst)&&
        ai_resource_valid(&o->socket_value)&&ai_resource_valid(&o->stream_value)&&
        ai_resource_valid(&o->data);
}

static void ai_print_resource(const AI_RESOURCE_OBSERVATION *resource,
    const char *owner, const char *terminal)
{
    printf("RESOURCE_TYPE=%s RESOURCE_ID_TEST_ONLY=%lu ACQUIRED_COUNT=%lu "
        "RELEASE_ATTEMPT_COUNT=%lu RELEASE_SUCCESS_COUNT=%lu OWNER=%s "
        "TERMINAL_CAUSE=%s REFERENCE_REMAINING=%s\n", resource->type,
        (unsigned long)resource->test_id,
        (unsigned long)resource->acquired_count,
        (unsigned long)resource->release_attempt_count,
        (unsigned long)resource->release_success_count, owner, terminal,
        resource->reference_remaining == PAPACC_TRUE ? "YES" : "NO");
}

static void ai_selftest_close(void *context)
{
    (void)context;
}

static int ai_oracle_selftest(void)
{
    AI_CASE_C_OBSERVER observer;
    AI_RESOURCE_OBSERVATION never_acquired;
    PAPACC_NETWORK_STREAM stream = PAPACC_NETWORK_STREAM_INITIALIZER;
    PAPACC_OUTBOUND_TLS tls = PAPACC_OUTBOUND_TLS_INITIALIZER;
    ai_observer_init(&observer);
    stream.io.context = &tls;
    stream.io.close = ai_selftest_close;
    ai_active_observer = &observer;
    if (!ai_install_close_observer(&observer, &stream) ||
        stream.io.context != &tls || observer.original_context != &tls)
        return 200;
    ai_active_observer = NULL;
    observer.terminal_truncated = PAPACC_TRUE;
    observer.context_during_unchanged = PAPACC_TRUE;
    observer.context_after_owner_release = PAPACC_TRUE;
    observer.original_close_calls = 1U;
    observer.pst.acquired_count = observer.socket_value.acquired_count = 1U;
    observer.stream_value.acquired_count = observer.data.acquired_count = 1U;
    observer.pst.release_attempt_count =
        observer.socket_value.release_attempt_count = 1U;
    observer.stream_value.release_attempt_count =
        observer.data.release_attempt_count = 1U;
    observer.pst.release_success_count =
        observer.socket_value.release_success_count = 1U;
    observer.stream_value.release_success_count =
        observer.data.release_success_count = 1U;
    if (!ai_observer_valid(&observer)) return 201;
    observer.pst.release_attempt_count = 2U;
    if (ai_observer_valid(&observer)) return 202;
    observer.pst.release_attempt_count = 1U;
    observer.socket_value.release_success_count = 0U;
    if (ai_observer_valid(&observer)) return 203;
    observer.socket_value.release_success_count = 1U;
    ai_resource_init(&never_acquired, "NEVER_ACQUIRED", 5U);
    if (!ai_resource_valid(&never_acquired)) return 204;
    never_acquired.release_attempt_count = 1U;
    if (ai_resource_valid(&never_acquired)) return 205;
    observer.terminal_publications = 2U;
    if (observer.terminal_publications <= 1U) return 206;
    observer.terminal_publications = 1U;
    observer.data.release_attempt_count = 0U;
    if (ai_observer_valid(&observer)) return 207;
    observer.data.release_attempt_count = 1U;
    observer.data.reference_remaining = PAPACC_TRUE;
    if (ai_observer_valid(&observer)) return 208;
    observer.data.reference_remaining = PAPACC_FALSE;
    observer.context_during_unchanged = PAPACC_FALSE;
    if (ai_observer_valid(&observer)) return 209;
    ai_observer_init(&observer);
    observer.installed=observer.shutdown_started=observer.shutdown_pending=PAPACC_TRUE;
    observer.shutdown_write_interest=observer.pst_close_clean_observed=PAPACC_TRUE;
    observer.cancel_after_pending=observer.stop_delivered_to_owner=PAPACC_TRUE;
    observer.owner_stopped=observer.cancel_cause_preserved=PAPACC_TRUE;
    observer.wire_terminal_absent=observer.context_before_original=PAPACC_TRUE;
    observer.context_during_unchanged=observer.context_after_owner_release=PAPACC_TRUE;
    observer.original_close_calls=1U;
    observer.pst.acquired_count=observer.socket_value.acquired_count=1U;
    observer.stream_value.acquired_count=observer.data.acquired_count=1U;
    observer.pst.release_attempt_count=observer.socket_value.release_attempt_count=1U;
    observer.stream_value.release_attempt_count=observer.data.release_attempt_count=1U;
    observer.pst.release_success_count=observer.socket_value.release_success_count=1U;
    observer.stream_value.release_success_count=observer.data.release_success_count=1U;
    if(!ai_d_observer_valid(&observer))return 210;
    observer.cancel_cause_preserved=PAPACC_FALSE;if(ai_d_observer_valid(&observer))return 211;observer.cancel_cause_preserved=PAPACC_TRUE;
    observer.terminal_truncated=PAPACC_TRUE;if(ai_d_observer_valid(&observer))return 212;observer.terminal_truncated=PAPACC_FALSE;
    observer.stop_delivered_to_owner=PAPACC_FALSE;if(ai_d_observer_valid(&observer))return 213;observer.stop_delivered_to_owner=PAPACC_TRUE;
    observer.terminal_publication_attempts=2U;observer.terminal_publication_successes=2U;if(ai_d_observer_valid(&observer))return 214;observer.terminal_publication_attempts=observer.terminal_publication_successes=0U;
    observer.terminal_type=PAPACC_MESSAGE_TYPE_NETWORK_CLOSE;observer.terminal_publication_attempts=1U;if(ai_d_observer_valid(&observer))return 215;observer.terminal_type=0U;observer.terminal_publication_attempts=0U;
    observer.terminal_publications=1U;if(ai_d_observer_valid(&observer))return 216;observer.terminal_publications=0U;
    observer.pst_close_clean_observed=PAPACC_FALSE;if(ai_d_observer_valid(&observer))return 217;observer.pst_close_clean_observed=PAPACC_TRUE;
    puts("D_ORACLE_NEGATIVES=PASS missing_cause=DETECTED reused_C_cause=DETECTED owner_not_reached=DETECTED duplicate_terminal=DETECTED expected_terminal_missing=DETECTED resource_close_as_terminal=DETECTED close_clean_missing=DETECTED");
    ai_observer_init(&observer);
    if(ai_observed_terminal_cause(&observer)!=NULL)return 218;
    observer.cancel_cause_preserved=PAPACC_TRUE;
    if(ai_observed_terminal_cause_is(&observer,"SHUTDOWN_DEADLINE"))return 219;
    observer.deadline_cause_observed=PAPACC_TRUE;
    if(ai_observed_terminal_cause(&observer)!=NULL)return 220;
    observer.cancel_cause_preserved=PAPACC_FALSE;
    if(!ai_observed_terminal_cause_is(&observer,"SHUTDOWN_DEADLINE"))return 221;
    observer.terminal_truncated=PAPACC_TRUE;
    if(ai_observed_terminal_cause(&observer)!=NULL)return 222;
    puts("E_RESOURCE_CAUSE_ORACLE_NEGATIVES=PASS missing=DETECTED inherited_D=DETECTED divergent=DETECTED owner_mismatch=DETECTED");
    puts("RESOURCE_ORACLE_NEGATIVES=PASS duplicate_release=DETECTED "
        "missing_release=DETECTED never_acquired=EXCLUDED "
        "duplicate_terminal=DETECTED stale_counter=DETECTED "
        "remaining_reference=DETECTED context_mutation=DETECTED");
    puts("IO_CONTEXT_REGRESSION=PASS before=ORIGINAL during=UNCHANGED");
    return 0;
}
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
PAPACC_RESULT papacc_server_secure_io_win32_test_probe_stop_retention(
    PAPACC_SERVER_SECURE_IO_WIN32 *,pst_connection *,pst_wait_token,PAPACC_U64);
static PAPACC_U32 p10_retention_checks;
static PAPACC_BOOL p10_persistent_verified;
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
static pst_test_only_correlation_snapshot p10_initial_native;
static PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION p10_initial_removal;

static int ai_p10_native_snapshot(const AI_CASE_C_OBSERVER *o,
    pst_test_only_correlation_snapshot *out)
{
    pst_test_correlation_token token;
    if(o==NULL||o->tls==NULL||o->tls->test_correlation_associated!=PAPACC_TRUE)
        return 0;
    token.high=o->tls->test_correlation_token_high;
    token.low=o->tls->test_correlation_token_low;
    memset(out,0,sizeof(*out));
    return pst_test_only_correlation_snapshot_by_token(token,out)==PST_RESULT_OK&&
        out->associated!=0U&&out->connection_id!=0U&&out->socket_lifetime_id!=0U;
}

static int ai_p10_native_retained(
    const pst_test_only_correlation_snapshot *s,
    const pst_test_only_correlation_snapshot *baseline)
{
    return s->token.high==baseline->token.high&&s->token.low==baseline->token.low&&
        s->connection_id==baseline->connection_id&&
        s->provider_connection_id==baseline->provider_connection_id&&
        s->socket_lifetime_id==baseline->socket_lifetime_id&&
        s->waitset_remove_attempts==baseline->waitset_remove_attempts&&
        s->waitset_remove_successes==baseline->waitset_remove_successes&&
        s->release_attempts==0U&&s->release_completions==0U&&
        s->provider_destroy_attempts==0U&&s->provider_destroy_completions==0U&&
        s->native_close_attempts==0U&&s->native_close_successes==0U&&
        s->native_close_failures==0U&&s->transport_destroy_attempts==0U&&
        s->transport_destroy_completions==0U&&s->released==0U&&s->native_closed==0U&&
        s->associated!=0U;
}

static int ai_p10_retention_negatives(
    const pst_test_only_correlation_snapshot *baseline)
{
    pst_test_only_correlation_snapshot s;
    PAPACC_U32 passed=0U;
#define P10_NATIVE_REJECT(change) do { s=*baseline;change; \
    if(ai_p10_native_retained(&s,baseline))return 0;++passed; } while(0)
    P10_NATIVE_REJECT(++s.token.low);
    P10_NATIVE_REJECT(++s.connection_id);
    P10_NATIVE_REJECT(++s.provider_connection_id);
    P10_NATIVE_REJECT(++s.socket_lifetime_id);
    P10_NATIVE_REJECT(++s.waitset_remove_successes);
    P10_NATIVE_REJECT(++s.release_attempts);
    P10_NATIVE_REJECT(++s.provider_destroy_attempts);
    P10_NATIVE_REJECT(++s.native_close_attempts);
    P10_NATIVE_REJECT(++s.native_close_failures);
    P10_NATIVE_REJECT(++s.transport_destroy_attempts);
    P10_NATIVE_REJECT(s.released=1U);
    P10_NATIVE_REJECT(s.native_closed=1U);
#undef P10_NATIVE_REJECT
    return passed==12U;
}

static int ai_p10_retained(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *owner=o->secure_io;
    PAPACC_SIZE i;
    PAPACC_BOOL membership=PAPACC_FALSE,ready=PAPACC_FALSE;
    pst_test_only_correlation_snapshot native;
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION removal;
    if(owner==NULL||
       papacc_server_secure_io_win32_test_get_remove_observation(owner,&removal)!=
            PAPACC_RESULT_OK||
       removal.normal_remove_attempt_count!=p10_initial_removal.normal_remove_attempt_count||
       removal.normal_remove_success_count!=p10_initial_removal.normal_remove_success_count||
       removal.target_real_remove_call_count!=0U||
       removal.target_real_remove_success_count!=0U||o->tls->processor.connection==NULL||
       owner->test_remove_selected_connection!=o->tls->processor.connection||
       o->stream->stream_instance_id!=o->stream_instance_id||
       owner->shutdown_deadline_ns!=o->p10_deadline_after_failure||
       owner->stop_cause!=o->p10_cause_after_failure||
       owner->stop_cause!=PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SERVER_STOP_REQUESTED||
       owner->scheduler.member_count!=o->p10_membership_after_failure||
       o->pst.release_attempt_count!=0U||o->original_close_calls!=0U||
       o->stream->state!=PAPACC_NETWORK_STREAM_OPEN||
       o->tls->processor.state!=PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED||
       owner->dispatch_active==PAPACC_TRUE||
       (owner->stop_state!=PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING&&
        owner->stop_state!=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL)||
       !ai_p10_native_snapshot(o,&native)||
       !ai_p10_native_retained(&native,&p10_initial_native))return 0;
    for(i=0U;i<owner->scheduler.member_capacity;++i){
        PAPACC_PST_SCHEDULER_MEMBER *m=&owner->scheduler.members[i];
        if(m->registered==PAPACC_TRUE&&
           m->kind==PAPACC_PST_SCHEDULER_MEMBER_CONNECTION&&
           m->source.connection==o->tls->processor.connection){
            PAPACC_SIZE j;
            membership=PAPACC_TRUE;
            for(j=0U;j<PAPACC_SERVER_NETWORK_STREAM_CAPACITY;++j)
                if(owner->outbound_ready[j].registered==PAPACC_TRUE&&
                   owner->outbound_ready[j].connection==m->source.connection&&
                   owner->outbound_ready[j].stream_instance_id==o->stream_instance_id&&
                   owner->outbound_ready[j].wait_token==m->token)ready=PAPACC_TRUE;
        }
    }
    if(membership!=PAPACC_TRUE||ready!=PAPACC_TRUE)return 0;
    ++p10_retention_checks;
    return 1;
}

static int ai_p10_retention_state_negatives(AI_CASE_C_OBSERVER *o)
{
    AI_CASE_C_OBSERVER mutated;
    PAPACC_SERVER_SECURE_IO_WIN32 owner;
    PAPACC_U32 passed=0U;
#define P10_STATE_REJECT(change) do { mutated=*o;owner=*o->secure_io; \
    mutated.secure_io=&owner;change; \
    if(ai_p10_retained(&mutated))return 0;++passed; } while(0)
    P10_STATE_REJECT(++owner.shutdown_deadline_ns);
    P10_STATE_REJECT(owner.stop_cause=PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE);
    P10_STATE_REJECT(++mutated.stream_instance_id);
    P10_STATE_REJECT(owner.stop_state=PAPACC_SERVER_SECURE_STOP_STOPPED);
    P10_STATE_REJECT(++owner.scheduler.member_count);
    P10_STATE_REJECT(owner.dispatch_active=PAPACC_TRUE);
#undef P10_STATE_REJECT
    return passed==6U;
}

static int ai_p10_persistent_terminal_valid(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION removal;
    PAPACC_U64 now=0U;
    if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK||
       now<o->secure_io->shutdown_deadline_ns||
       o->secure_io->stop_state!=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL||
       papacc_server_secure_io_win32_test_get_remove_observation(
            o->secure_io,&removal)!=PAPACC_RESULT_OK)return 0;
    return removal.target_stop_remove_attempt_count>1U&&
        removal.target_stop_remove_attempt_count<=
            o->secure_io->establishment_timeout_ns/10000000U+1U&&
        removal.target_injected_failure_count==
            removal.target_stop_remove_attempt_count&&
        ai_p10_retained(o)&&ai_p10_retention_state_negatives(o);
}

#endif
#ifndef PAPACC_ENABLE_PST_TEST_CORRELATION
static int ai_p10_consumer_pending(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION r;
    PAPACC_SERVER_SECURE_IO_WIN32 *owner=o->secure_io;
    PAPACC_CONNECTION *data;
    PAPACC_SIZE i;
    if(o->p10_stop_request_after_ack!=PAPACC_TRUE||o->tls==NULL||
       o->tls->processor.connection==NULL||
       owner->dispatch_active==PAPACC_TRUE||
       owner->stop_cause!=o->p10_cause_after_failure||
       owner->shutdown_deadline_ns!=o->p10_deadline_after_failure||
       o->pst.release_attempt_count!=0U||o->original_close_calls!=0U||
       papacc_server_secure_io_win32_test_get_remove_observation(owner,&r)!=
            PAPACC_RESULT_OK||r.connection_instance_id!=o->connection_instance_id||
       r.stream_generation!=o->stream_instance_id||
       r.target_injected_failure_count==0U||r.target_real_remove_success_count!=0U)
        return 0;
    data=papacc_connection_manager_find(owner->protocol_loop->connection_manager,
        o->connection_instance_id);
    if(data==NULL||data->state!=PAPACC_CONNECTION_STATE_CLOSING)return 0;
    for(i=0U;i<PAPACC_SERVER_NETWORK_STREAM_CAPACITY;++i)
        if(owner->outbound_ready[i].registered==PAPACC_TRUE&&
           owner->outbound_ready[i].connection==o->tls->processor.connection&&
           owner->outbound_ready[i].stream_instance_id==o->stream_instance_id)
            return 1;
    return 0;
}
#endif

static int ai_p10_begin_pre_close(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *owner=o->secure_io;
    PAPACC_CONNECTION *c=papacc_connection_manager_find(
        owner->protocol_loop->connection_manager,o->connection_instance_id);
    PAPACC_TRANSPORT_CONNECTION transport;
    PAPACC_BOOL removed=PAPACC_FALSE;
    PAPACC_RESULT result;
    PAPACC_SIZE count=owner->protocol_loop->connection_manager->count;
    if(c==NULL||owner->dispatch_active==PAPACC_TRUE||
       owner->stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING)return 0;
    transport=c->transport;
    /* TEST_ONLY active-dispatch injection exercises the real pre-close root.
     * No processor/provider result is mocked. Restore the flag before Stop. */
    owner->dispatch_active=PAPACC_TRUE;
    result=papacc_connection_manager_try_remove(
        owner->protocol_loop->connection_manager,o->connection_instance_id,&removed);
    owner->dispatch_active=PAPACC_FALSE;
    if(result!=PAPACC_RESULT_INVALID_STATE||removed==PAPACC_TRUE||
       c->state!=PAPACC_CONNECTION_STATE_CLOSING||
       memcmp(&transport,&c->transport,sizeof(transport))!=0||
       owner->protocol_loop->connection_manager->count!=count)return 0;
    printf("P10_PRE_CLOSE_BEFORE_STOP=FAILED_RETAINED CONNECTION_INSTANCE_ID=%llu "
        "P10_DISPATCH_INJECTION=TEST_ONLY\n",
        (unsigned long long)o->connection_instance_id);
    return 1;
}

#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
static int ai_p10_native_completed(
    const pst_test_only_correlation_snapshot *s,
    const pst_test_only_correlation_snapshot *baseline)
{
    return s->associated!=0U&&s->token.high==baseline->token.high&&
        s->token.low==baseline->token.low&&s->connection_id==baseline->connection_id&&
        s->provider_connection_id==baseline->provider_connection_id&&
        s->socket_lifetime_id==baseline->socket_lifetime_id&&
        s->waitset_remove_successes>baseline->waitset_remove_successes&&
        s->release_attempts==1U&&s->release_completions==1U&&
        s->provider_destroy_attempts==1U&&s->provider_destroy_completions==1U&&
        s->native_close_attempts==1U&&s->native_close_successes==1U&&
        s->native_close_failures==0U&&s->transport_destroy_attempts==1U&&
        s->transport_destroy_completions==1U&&s->released!=0U&&
        s->native_closed!=0U&&s->order_valid!=0U;
}

static int ai_p10_completed_negatives(
    const pst_test_only_correlation_snapshot *baseline,
    const pst_test_only_correlation_snapshot *completed)
{
    pst_test_only_correlation_snapshot s;
    PAPACC_U32 passed=0U;
#define P10_COMPLETED_REJECT(change) do { s=*completed;change; \
    if(ai_p10_native_completed(&s,baseline))return 0;++passed; } while(0)
    P10_COMPLETED_REJECT(++s.token.low);
    P10_COMPLETED_REJECT(++s.connection_id);
    P10_COMPLETED_REJECT(++s.socket_lifetime_id);
    P10_COMPLETED_REJECT(s.waitset_remove_successes=baseline->waitset_remove_successes);
    P10_COMPLETED_REJECT(s.release_attempts=2U);
    P10_COMPLETED_REJECT(s.release_completions=0U);
    P10_COMPLETED_REJECT(s.provider_destroy_attempts=2U);
    P10_COMPLETED_REJECT(s.native_close_attempts=2U);
    P10_COMPLETED_REJECT(s.native_close_successes=0U);
    P10_COMPLETED_REJECT(s.native_close_failures=1U);
    P10_COMPLETED_REJECT(s.transport_destroy_attempts=2U);
    P10_COMPLETED_REJECT(s.order_valid=0U);
#undef P10_COMPLETED_REJECT
    return passed==12U;
}

static int ai_p10_check_completed(AI_CASE_C_OBSERVER *o)
{
    pst_test_only_correlation_snapshot s,after;
    pst_test_correlation_token token=p10_initial_native.token;
    PAPACC_SIZE i;
    if(pst_test_only_correlation_snapshot_by_token(token,&s)!=PST_RESULT_OK||
       !ai_p10_native_completed(&s,&p10_initial_native)||
       !ai_p10_completed_negatives(&p10_initial_native,&s))return 0;
    if(o->p10_stop_request_after_ack==PAPACC_TRUE)
        for(i=0U;i<3U;++i)
            if(papacc_server_secure_io_win32_request_stop(o->secure_io)!=
                    PAPACC_RESULT_OK)return 0;
    if(pst_test_only_correlation_snapshot_by_token(token,&after)!=PST_RESULT_OK||
       !ai_p10_native_completed(&after,&p10_initial_native))return 0;
    printf("P10_TOKEN=%08lX%08lX PST_CONNECTION_ID=%lu SOCKET_LIFETIME_ID=%lu "
        "P10_RELEASE=1 P10_PROVIDER_DESTROY=1 P10_NATIVE_CLOSE=1 "
        "P10_TRANSPORT_DESTROY=1 P10_ORDER_VALID=YES "
        "P10_NO_DOUBLE_RELEASE=PASS P10_COMPLETED_NEGATIVES=12_OF_12_PASS\n",
        (unsigned long)token.high,(unsigned long)token.low,
        (unsigned long)s.connection_id,(unsigned long)s.socket_lifetime_id);
    return 1;
}

static int ai_p10_probe_retention(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *owner=o->secure_io;
    PAPACC_SIZE i;
    PAPACC_CONNECTION *c=papacc_connection_manager_find(
        owner->protocol_loop->connection_manager,o->connection_instance_id);
    PAPACC_TRANSPORT_CONNECTION transport;
    PAPACC_BOOL removed=PAPACC_FALSE;
    PAPACC_RESULT expected=owner->stop_last_remove_result;
    PAPACC_SIZE manager_count=owner->protocol_loop->connection_manager->count;
    pst_wait_token token=0U;
    if(c==NULL||expected==PAPACC_RESULT_OK)return 0;
    transport=c->transport;
    for(i=0U;i<PAPACC_SERVER_NETWORK_STREAM_CAPACITY;++i)
        if(owner->outbound_ready[i].registered==PAPACC_TRUE&&
           owner->outbound_ready[i].connection==o->tls->processor.connection)
            token=owner->outbound_ready[i].wait_token;
    if(token==0U||papacc_server_secure_io_win32_test_probe_stop_retention(
            owner,o->tls->processor.connection,token,o->connection_instance_id)!=
            PAPACC_RESULT_OK)return 0;
    if(papacc_connection_manager_try_remove(
            owner->protocol_loop->connection_manager,o->connection_instance_id,
            &removed)!=expected||removed==PAPACC_TRUE||
       papacc_connection_manager_find(owner->protocol_loop->connection_manager,
            o->connection_instance_id)!=c||
       owner->protocol_loop->connection_manager->count!=manager_count||
       memcmp(&transport,&c->transport,sizeof(transport))!=0||
       c->state==PAPACC_CONNECTION_STATE_CLOSED)return 0;
    for(i=0U;i<3U;++i)
        if(papacc_server_secure_io_win32_request_stop(owner)!=expected||
           !ai_p10_retained(o))return 0;
    if(papacc_server_secure_io_win32_try_shutdown(owner)==PAPACC_RESULT_OK)
        return 0;
    papacc_server_secure_io_win32_shutdown(owner);
    return ai_p10_retained(o);
}
#endif
#endif

static int run_reference_client(PAPACC_U16 port,
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, const char *scenario,
    PAPACC_BOOL stop_for_close, PAPACC_U16 fixture_port)
{
    char executable[MAX_PATH];
    char command[(MAX_PATH * 2) + 256];
    char event_name[96];
    char trace_path[MAX_PATH];
    char ack_event_name[96];
    char *separator;
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    DWORD exit_code = STILL_ACTIVE;
    PAPACC_SIZE rounds = 0U;
    PAPACC_BOOL stop_requested = PAPACC_FALSE;
    HANDLE shutdown_event;
    HANDLE shutdown_ack_event;
    if (GetModuleFileNameA(NULL, executable, sizeof(executable)) == 0U)
        return 0;
    separator = strrchr(executable, '\\');
    if (separator == NULL) return 0;
    strcpy_s(separator + 1U,
        sizeof(executable) - (size_t)(separator + 1U - executable),
        "papacc_secure_reference_client.exe");
    trace_path[0] = '\0';
    if(strcmp(scenario,"network-tls-control-loss")==0){
        strcpy_s(trace_path,sizeof(trace_path),executable);
        {
            char *trace_separator=strrchr(trace_path,'\\');
            if(trace_separator==NULL||sprintf_s(trace_separator+1U,
                    sizeof(trace_path)-(size_t)(trace_separator+1U-trace_path),
                    "papacc-f45-%lu-%lu.trace",
                    (unsigned long)GetCurrentProcessId(),
                    (unsigned long)GetTickCount())<0)return 0;
        }
        (void)DeleteFileA(trace_path);
    }
    if (sprintf_s(event_name, sizeof(event_name),
            "Local\\papacc-3f-%lu-%lu", (unsigned long)GetCurrentProcessId(),
            (unsigned long)GetTickCount()) < 0) return 0;
    shutdown_event = CreateEventA(NULL, TRUE, FALSE, event_name);
    if (shutdown_event == NULL) return 0;
    if (ai_active_observer != NULL &&
        strcmp(scenario, "network-tls-control-loss") == 0)
        ai_active_observer->control_loss_event = shutdown_event;
    if (sprintf_s(ack_event_name, sizeof(ack_event_name), "%s-ack",
            event_name) < 0) {
        CloseHandle(shutdown_event);
        return 0;
    }
    shutdown_ack_event = CreateEventA(NULL, TRUE, FALSE, ack_event_name);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    if(ai_active_observer!=NULL&&
        (strcmp(scenario,"network-g1-duplicate-close")==0||
         strcmp(scenario,"network-g2-duplicate-abort")==0))
        ai_active_observer->g1_client_ack_event=shutdown_ack_event;
#endif
    if (shutdown_ack_event == NULL ||
        (strcmp(scenario,"network-tls-control-loss")==0 ?
        sprintf_s(command,sizeof(command),"\"%s\" %u %s %s %s %u \"%s\"",
            executable,(unsigned int)port,event_name,ack_event_name,scenario,
            (unsigned int)fixture_port,trace_path) : fixture_port == 0U ?
        sprintf_s(command, sizeof(command), "\"%s\" %u %s %s %s", executable,
            (unsigned int)port, event_name, ack_event_name, scenario) :
        sprintf_s(command, sizeof(command), "\"%s\" %u %s %s %s %u",
            executable, (unsigned int)port, event_name, ack_event_name,
            scenario, (unsigned int)fixture_port)) < 0) {
        if (shutdown_ack_event != NULL) CloseHandle(shutdown_ack_event);
        CloseHandle(shutdown_event);
        return 0;
    }
    if(trace_path[0]!='\0')printf("F45_TRACE_FILE=%s\n",trace_path);
    memset(&startup, 0, sizeof(startup));
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    startup.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    memset(&process, 0, sizeof(process));
    if (!CreateProcessA(executable, command, NULL, NULL, TRUE,
            CREATE_NO_WINDOW, NULL, NULL, &startup, &process)) return 0;
    while (rounds++ < 5000U) {
        PAPACC_RESULT owner_poll_result;
        if(exit_code!=STILL_ACTIVE){
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            if(ai_active_observer==NULL||
               ai_active_observer->p10_remove_mode!=
                    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT||
               ai_active_observer->p10_stop_request_after_ack!=PAPACC_TRUE||
               secure_io->stop_state!=PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING)
#endif
                break;
        }
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        if(ai_active_observer!=NULL){
            ++ai_active_observer->p10_owner_poll_call_count;
            ai_active_observer->p10_stop_state_before=secure_io->stop_state;
        }
#endif
        owner_poll_result=papacc_server_secure_io_win32_poll_once(
            secure_io,10U);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        if(ai_active_observer!=NULL){
            ai_active_observer->p10_owner_poll_result=owner_poll_result;
            ai_active_observer->p10_stop_state_after=secure_io->stop_state;
        }
#endif
        {
            PAPACC_BOOL expected_pending=PAPACC_FALSE;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
            if(ai_active_observer!=NULL&&
               ai_active_observer->p10_stop_request_after_ack==PAPACC_TRUE&&
               (secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING||
                secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL)&&
               owner_poll_result==secure_io->stop_last_remove_result&&
               owner_poll_result!=PAPACC_RESULT_OK&&
               ai_p10_retained(ai_active_observer)){
                expected_pending=PAPACC_TRUE;
                if(secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL){
                    if(ai_active_observer->p10_remove_mode==
                            PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT&&
                       ai_p10_probe_retention(ai_active_observer)&&
                       ai_p10_retention_negatives(&p10_initial_native)&&
                       ai_p10_persistent_terminal_valid(ai_active_observer)){
                        p10_persistent_verified=PAPACC_TRUE;
                        ai_p10_write_trace(ai_active_observer,"ERROR_TERMINAL_RETAINED");
                    }
                    TerminateProcess(process.hProcess,252U);
                    (void)WaitForSingleObject(process.hProcess,5000U);
                    exit_code=252U;
                    break;
                }
            }
#else
            if(ai_active_observer!=NULL&&
               (secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING||
                secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL)&&
               owner_poll_result==secure_io->stop_last_remove_result&&
               owner_poll_result!=PAPACC_RESULT_OK&&
               ai_p10_consumer_pending(ai_active_observer)){
                expected_pending=PAPACC_TRUE;
                if(secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL){
                    ai_p10_write_trace(ai_active_observer,"CONSUMER_ERROR_TERMINAL_RETAINED");
                    puts("P10_CONSUMER_RETENTION=OBSERVED NATIVE_ORACLE=UNAVAILABLE P10_QUALIFIED=NO");
                    TerminateProcess(process.hProcess,252U);
                    (void)WaitForSingleObject(process.hProcess,5000U);
                    exit_code=252U;
                    break;
                }
            }
#endif
#endif
        if ((owner_poll_result != PAPACC_RESULT_OK&&
                expected_pending!=PAPACC_TRUE) ||
            !GetExitCodeProcess(process.hProcess, &exit_code)) {
            if(ai_active_observer!=NULL)
                ai_p10_write_trace(ai_active_observer,
                    owner_poll_result!=PAPACC_RESULT_OK?
                    "OWNER_POLL_NON_OK":"GET_EXIT_CODE_FAILED");
            TerminateProcess(process.hProcess, 255U);
            exit_code = 255U;
        }
        }
        if (ai_active_observer != NULL)
            ai_observe(secure_io, ai_active_observer);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
        if(ai_active_observer!=NULL&&
           ai_active_observer->p10_normal_only==PAPACC_TRUE&&
           p10_initial_native.associated==0U)
            (void)ai_p10_native_snapshot(ai_active_observer,&p10_initial_native);
#endif
        if(ai_active_observer!=NULL&&
           ai_active_observer->p10_remove_mode!=
                PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF&&
           ai_active_observer->p10_normal_only!=PAPACC_TRUE&&
           ai_active_observer->p10_stop_request_after_ack!=PAPACC_TRUE&&
           ai_active_observer->p10_fixture!=NULL&&
           ai_active_observer->p10_fixture->p10_marker_received_event!=NULL){
            DWORD marker_wait=WaitForSingleObject(
                ai_active_observer->p10_fixture->p10_marker_received_event,0U);
            ai_active_observer->p10_marker_ack_wait_result=marker_wait;
            if(marker_wait==WAIT_FAILED)
                ai_active_observer->p10_marker_ack_wait_error=GetLastError();
            else if(marker_wait==WAIT_OBJECT_0){
                AI_CASE_C_OBSERVER *p10=ai_active_observer;
                p10->p10_marker_ack_observed=PAPACC_TRUE;
                p10->p10_marker_ack_sequence=InterlockedIncrement(
                    &p10->p10_fixture->p10_sequence);
                p10->p10_deadline_before=secure_io->shutdown_deadline_ns;
                if(
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                    (p10_native_mode>=0&&!p10_native_begin_pending(p10))||
#endif
                    !ai_p10_begin_pre_close(p10)){
                    TerminateProcess(process.hProcess,253U);
                    (void)WaitForSingleObject(process.hProcess,5000U);
                    exit_code=253U;
                    break;
                }
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                if(p10_native_mode>=0)p10_native_checkpoint(p10,3U);
#endif
                p10->p10_first_stop_result=
                    papacc_server_secure_io_win32_request_stop(secure_io);
                p10->p10_stop_request_after_ack=PAPACC_TRUE;
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                if(p10_native_mode>=0&&!p10_native_queue_after_stop(p10)){
                    TerminateProcess(process.hProcess,254U);
                    (void)WaitForSingleObject(process.hProcess,5000U);exit_code=254U;break;
                }
#endif
                p10->p10_stop_request_sequence=InterlockedIncrement(
                    &p10->p10_fixture->p10_sequence);
                p10->p10_membership_after_failure=
                    secure_io->scheduler.member_count;
                p10->p10_deadline_after_failure=
                    secure_io->shutdown_deadline_ns;
                p10->p10_cause_after_failure=secure_io->stop_cause;
                p10->p10_connection_preserved=
                    p10->tls->processor.connection!=NULL?
                    PAPACC_TRUE:PAPACC_FALSE;
                p10->p10_processor_preserved=
                    p10->tls->processor.state!=
                    PAPACC_PST_SECURE_PROCESSOR_CLOSED?
                    PAPACC_TRUE:PAPACC_FALSE;
                p10->p10_release_attempts_after_failure=
                    p10->pst.release_attempt_count;
                p10->p10_destructive_close_attempts_after_failure=
                    p10->original_close_calls;
                p10->p10_stop_state_after_failure=secure_io->stop_state;
                p10->p10_stop_state_at_ack=secure_io->stop_state;
                {
                    PAPACC_SIZE p10_index;
                    for(p10_index=0U;
                            p10_index<PAPACC_SERVER_NETWORK_STREAM_CAPACITY;
                            ++p10_index){
                        PAPACC_SERVER_OUTBOUND_READY_WIN32 *ready=
                            &secure_io->outbound_ready[p10_index];
                        if(ready->registered==PAPACC_TRUE&&
                           ready->connection==p10->tls->processor.connection&&
                           ready->stream_instance_id==p10->stream_instance_id){
                            p10->p10_target_membership_at_ack=PAPACC_TRUE;
                            break;
                        }
                    }
                }
                ++p10->p10_owner_stop_ack_signal_attempts;
                if(
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                    ((p10_native_mode==0&&p10->p10_first_stop_result==PAPACC_RESULT_OK)||
                    (
#endif
                    p10->p10_first_stop_result==
                        PAPACC_RESULT_INTERNAL_ERROR&&
                   p10->p10_stop_state_at_ack==
                        PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING&&
                   p10->p10_connection_preserved==PAPACC_TRUE&&
                   p10->p10_target_membership_at_ack==PAPACC_TRUE&&
                   p10->p10_release_attempts_after_failure==0U&&
                   p10->p10_destructive_close_attempts_after_failure==0U
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
                   ))
#endif
                   &&p10->p10_fixture->p10_owner_stop_event!=NULL&&
                   SetEvent(p10->p10_fixture->p10_owner_stop_event)!=0){
                    ++p10->p10_owner_stop_ack_signal_successes;
                    p10->p10_owner_stop_ack_signal_sequence=
                        InterlockedIncrement(&p10->p10_fixture->p10_sequence);
                }else{
                    p10->p10_owner_stop_ack_signal_error=GetLastError();
                }
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
                if(papacc_server_secure_io_win32_test_get_remove_observation(
                        secure_io,&p10_initial_removal)!=PAPACC_RESULT_OK||
                   !ai_p10_native_snapshot(p10,&p10_initial_native)||
                   !ai_p10_native_retained(&p10_initial_native,&p10_initial_native)||
                   !ai_p10_probe_retention(p10)){
                    TerminateProcess(process.hProcess,253U);
                    (void)WaitForSingleObject(process.hProcess,5000U);
                    exit_code=253U;
                    break;
                }
#endif
            }
        }
        if(ai_active_observer!=NULL&&
            (strcmp(scenario,"network-g1-duplicate-close")==0||
             strcmp(scenario,"network-g2-duplicate-abort")==0)&&
            ai_active_observer->g1_owner_injection_requested==PAPACC_TRUE&&
            ai_active_observer->g1_owner_injection_performed!=PAPACC_TRUE){
            PAPACC_SIZE slot_index;
            PAPACC_BOOL read_interest=PAPACC_TRUE,write_interest=PAPACC_FALSE;
            for(slot_index=0U;slot_index<secure_io->protocol_loop->processor_capacity;
                    ++slot_index){
                PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot=
                    &secure_io->protocol_loop->processor_slots[slot_index];
                if(slot->in_use==PAPACC_TRUE&&
                    slot->kind==PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH&&
                    slot->network_data_protocol.channel_instance_id==
                        ai_active_observer->channel_instance_id){
                    ai_active_observer->g1_data_terminal_after_first=
                        slot->network_data_protocol.state==
                        PAPACC_NETWORK_DATA_TERMINAL?PAPACC_TRUE:PAPACC_FALSE;
                    if(papacc_server_io_loop_win32_processor_interest(
                            secure_io->protocol_loop,slot_index,&read_interest,
                            &write_interest)==PAPACC_RESULT_OK&&
                        read_interest!=PAPACC_TRUE)
                        ai_active_observer->g1_data_read_disabled_after_first=
                            PAPACC_TRUE;
                    break;
                }
            }
            ai_active_observer->g1_owner_injection_result=
                papacc_server_io_loop_win32_test_inject_owner_terminal(
                    secure_io->protocol_loop,
                    ai_active_observer->channel_instance_id,
                    ai_active_observer->stream_instance_id,
                    ai_active_observer->g2_mode==PAPACC_TRUE?
                        PAPACC_MESSAGE_TYPE_NETWORK_ABORT:
                        PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
            ai_active_observer->g1_owner_injection_performed=PAPACC_TRUE;
        }
#endif
        if (stop_for_close == PAPACC_TRUE &&
            stop_requested != PAPACC_TRUE &&
            WaitForSingleObject(shutdown_event, 0U) == WAIT_OBJECT_0) {
            if (papacc_server_secure_io_win32_request_stop(secure_io) !=
                    PAPACC_RESULT_OK) {
                TerminateProcess(process.hProcess, 254U);
                exit_code = 254U;
            } else {
                stop_requested = PAPACC_TRUE;
                (void)SetEvent(shutdown_ack_event);
            }
        }
    }
    if(ai_active_observer!=NULL&&ai_active_observer->observe_shutdown==PAPACC_TRUE&&
        strcmp(scenario,"network-tls-shutdown-cancel")==0&&exit_code==0U)
        ai_active_observer->wire_terminal_absent=PAPACC_TRUE;
    if(ai_active_observer!=NULL&&
       strcmp(scenario,"network-tls-control-loss")==0){
        printf("F45_LOW_INTERFERENCE installed=%d clean_close=%d "
            "shutdown_started=%d shutdown_pending=%d write_interest=%d "
            "delegate_calls=%lu delegate_result=%d delegate_done=%d "
            "state_before=%d state_after=%d interest_result=%d "
            "interest_read=%d interest_write=%d source=%lu "
            "signal_attempts=%ld signal_successes=%ld signal_error=%ld "
            "event_signaled=%d\\n",
            (int)ai_active_observer->installed,
            (int)ai_active_observer->pst_close_clean_observed,
            (int)ai_active_observer->shutdown_started,
            (int)ai_active_observer->shutdown_pending,
            (int)ai_active_observer->shutdown_write_interest,
            (unsigned long)ai_active_observer->delegate_call_count,
            (int)ai_active_observer->delegate_result,
            (int)ai_active_observer->delegate_done,
            (int)ai_active_observer->delegate_state_before,
            (int)ai_active_observer->delegate_state_after,
            (int)ai_active_observer->delegate_interest_result,
            (int)ai_active_observer->delegate_interest_read,
            (int)ai_active_observer->delegate_interest_write,
            (unsigned long)ai_active_observer->delegate_source,
            (long)ai_active_observer->control_loss_signal_attempts,
            (long)ai_active_observer->control_loss_signal_successes,
            (long)ai_active_observer->control_loss_signal_error,
            (int)ai_active_observer->control_loss_signaled);
        fflush(stdout);
        DWORD ack_result;
        PAPACC_U64 ack_now=0U;
        (void)papacc_pal_monotonic_time_ns(&ack_now);
        printf("F45_ACK_OBSERVATION=START operation=WaitForSingleObject_zero_timeout expected=SIGNALED timestamp_ns=%llu operation_id=F-CONTROL-LOSS\n",
            (unsigned long long)ack_now);
        ack_result=WaitForSingleObject(shutdown_ack_event,0U);
        (void)papacc_pal_monotonic_time_ns(&ack_now);
        printf("F45_ACK_RECEIVED=%s observed=%lu timestamp_ns=%llu operation_id=F-CONTROL-LOSS\n",
            ack_result==WAIT_OBJECT_0?"YES":"NO",(unsigned long)ack_result,
            (unsigned long long)ack_now);
        if(ack_result==WAIT_OBJECT_0)
            ai_active_observer->control_loss_acknowledged=PAPACC_TRUE;
    }
    if (ai_active_observer != NULL &&
        secure_io->stop_state!=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL) {
        PAPACC_SIZE cleanup_round;
        for (cleanup_round = 0U; cleanup_round < 100U &&
                !ai_observer_valid(ai_active_observer); ++cleanup_round) {
            if (papacc_server_secure_io_win32_poll_once(secure_io, 1U) !=
                    PAPACC_RESULT_OK) break;
            ai_observe(secure_io, ai_active_observer);
        }
    }
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    CloseHandle(shutdown_event);
    CloseHandle(shutdown_ack_event);
    if (exit_code != 0U)
        fprintf(stderr, "reference client exit=%lu rounds=%zu connections=%zu "
            "sessions=%zu channels=%zu jobs=%zu streams=%zu\n",
            (unsigned long)exit_code, (size_t)rounds,
            (size_t)secure_io->protocol_loop->connection_manager->count,
            (size_t)secure_io->protocol_loop->session_manager.count,
            (size_t)secure_io->protocol_loop->channel_manager.count,
            (size_t)secure_io->protocol_loop->network_job_manager.count,
            (size_t)secure_io->protocol_loop->network_stream_manager.count);
    return exit_code == 0U;
}

static PAPACC_BOOL stage13_stop_after_check80_requested(void)
{
    char value[2];
    DWORD length;
    SetLastError(ERROR_SUCCESS);
    length = GetEnvironmentVariableA("PAPACC_TEST_STOP_AFTER_CHECK80", value,
        (DWORD)sizeof(value));
    if (length == 0U || length >= (DWORD)sizeof(value))
        return PAPACC_FALSE;
    return length == 1U && value[0] == '1' ? PAPACC_TRUE : PAPACC_FALSE;
}

static int stage13_env_gate_selftest(void)
{
    static const char oversized[] = "11111111111111111111111111111111";
    if (!SetEnvironmentVariableA("PAPACC_TEST_STOP_AFTER_CHECK80", NULL) ||
        stage13_stop_after_check80_requested() != PAPACC_FALSE)
        return 201;
    if (!SetEnvironmentVariableA("PAPACC_TEST_STOP_AFTER_CHECK80", "") ||
        stage13_stop_after_check80_requested() != PAPACC_FALSE)
        return 202;
    if (!SetEnvironmentVariableA("PAPACC_TEST_STOP_AFTER_CHECK80", "1") ||
        stage13_stop_after_check80_requested() != PAPACC_TRUE)
        return 203;
    if (!SetEnvironmentVariableA("PAPACC_TEST_STOP_AFTER_CHECK80", "other") ||
        stage13_stop_after_check80_requested() != PAPACC_FALSE)
        return 204;
    if (!SetEnvironmentVariableA("PAPACC_TEST_STOP_AFTER_CHECK80", oversized) ||
        stage13_stop_after_check80_requested() != PAPACC_FALSE)
        return 205;
    puts("STAGE13_ENV_GATE_SELFTEST=PASS absent=REJECTED empty=REJECTED expected=ACCEPTED other=REJECTED oversized=REJECTED");
    return 0;
}

int main(int argc, char **argv)
{    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture =
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_CONNECTION connections[CAPACITY];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT contexts[CAPACITY];
    PAPACC_SESSION sessions[CAPACITY];
    PAPACC_CHANNEL channels[CAPACITY];
    PAPACC_DATA_ASSOCIATION_ENTRY associations[CAPACITY];
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 protocol_slots[CAPACITY];
    PAPACC_SERVER_IO_LOOP_WIN32 protocol_loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    TEST_EGRESS egress_fixture = {
        PAPACC_NETWORK_ENDPOINT_INITIALIZER, 0U };
    PAPACC_EGRESS_POLICY egress_policy = PAPACC_EGRESS_POLICY_INITIALIZER;
    PAPACC_EGRESS_PROFILE egress_profile = PAPACC_EGRESS_PROFILE_INITIALIZER;
    PAPACC_EGRESS_RUNTIME_CONFIG egress_config =
        PAPACC_EGRESS_RUNTIME_CONFIG_INITIALIZER;
    PAPACC_DNS_RESOLVER resolver = PAPACC_DNS_RESOLVER_INITIALIZER;
    PAPACC_OUTBOUND_TLS_POLICY outbound_policy;
    PAPACC_SECURITY_COMPOSITION composition =
        PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION security =
        PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION_INITIALIZER;
    PAPACC_SECURITY_DER_ITEM server_certificate;
    PAPACC_SECURITY_DER_ITEM trust_anchor;
    PAPACC_LOGGER logger;
    TEST_POLICY policy = { 0U, 0U, PAPACC_FALSE, { 0 }, PAPACC_FALSE, { 0 },
        PAPACC_PRINCIPAL_RESOLVED, PAPACC_AUTHORIZATION_ALLOW };
    PAPACC_SERVER_LISTENER_CONFIGURATION listener =
        PAPACC_SERVER_LISTENER_CONFIGURATION_INITIALIZER;
    PAPACC_SERVER_LISTENER_CONFIGURATION_SET listener_set = { &listener, 1U };
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidates[CAPACITY];
    PAPACC_PST_SCHEDULER_MEMBER scheduler_members[CAPACITY + 1U +
        PAPACC_SERVER_NETWORK_STREAM_CAPACITY];
    PST_WAIT_EVENT scheduler_events[CAPACITY + 1U +
        PAPACC_SERVER_NETWORK_STREAM_CAPACITY];
    pst_external_source *listener_sources[1];
    pst_wait_token listener_tokens[1];
    PAPACC_SERVER_SECURE_IO_WIN32 secure_io =
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    struct sockaddr_in native_address;
    int native_length = sizeof(native_address);
    PAPACC_U16 port;
    SOCKET fixture_listener = INVALID_SOCKET;
    SOCKET fixture_socket = INVALID_SOCKET;
    pst_transport *fixture_transport = NULL;
    pst_connection *fixture_connection = NULL;
    pst_trust *wrong_trust = NULL;
    SOCKET client_socket = INVALID_SOCKET;
    pst_transport *client_transport = NULL;
    PAPACC_PST_SECURE_PROCESSOR client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR data_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR replay_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR foreign_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR matched_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PAPACC_PST_SECURE_PROCESSOR bad_protocol_client =
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    PST_CONNECTION_CONFIG client_config;
    PAPACC_BOOL ownership = PAPACC_FALSE;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_RESULT server_result;
    PAPACC_SIZE offset = 0U;
    PAPACC_SIZE transferred;
    PAPACC_U8 response[sizeof(control_accept)];
    PAPACC_SIZE response_size = 0U;
    PAPACC_U8 ticket_frame[32];
    PAPACC_SIZE ticket_frame_size = 0U;
    PAPACC_U8 data_attach[32] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
    PAPACC_U8 data_response[sizeof(data_accept)];
    PAPACC_SIZE data_response_size = 0U;
    PAPACC_SIZE index;
    PAPACC_BOOL control_shutdown = PAPACC_FALSE;
    PAPACC_BOOL data_shutdown = PAPACC_FALSE;
    PAPACC_BOOL matched_shutdown = PAPACC_FALSE;
    int result = 0;
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
    if(argc==2){
        if(strcmp(argv[1],"--isolated-p10-normal-need-write")==0){p10_native_mode=0;p10_native_need_write=1;p10_fixture_bulk_drain=1;}
        else if(strcmp(argv[1],"--isolated-p10-normal-queued-stop")==0){p10_native_mode=1;p10_native_queued_probe=1;}
        else if(strcmp(argv[1],"--isolated-p10-normal-stop")==0)p10_native_mode=0;
        else if(strcmp(argv[1],"--isolated-p10-normal-once")==0)p10_native_mode=1;
        else if(strcmp(argv[1],"--isolated-p10-normal-persistent")==0)p10_native_mode=2;
        if(p10_native_mode>=0){
            if(!SetEnvironmentVariableA("PAPACC_ADR0032_P10",p10_native_mode==2?"persistent":"once"))return 124;
            argv[1]="--isolated-tls-shutdown-cancel";
        }
    }
#endif

    if (argc == 2 && strcmp(argv[1], "--stage13-env-gate-selftest") == 0)
        return stage13_env_gate_selftest();
    if (argc == 2 && strcmp(argv[1], "--resource-oracle-selftest") == 0)
        return ai_oracle_selftest();
    if (argc == 2 && strcmp(argv[1],
            "--reference-fixture-initialization-selftest") == 0) {
        int passed = reference_fixture_initialization_selftest();
        puts(passed ?
            "REFERENCE_FIXTURE_INITIALIZATION_SELFTEST=PASS OPTIONAL_FLAGS_ZERO=YES OPTIONAL_HANDLES_NULL=YES" :
            "REFERENCE_FIXTURE_INITIALIZATION_SELFTEST=FAIL");
        return passed ? 0 : 1;
    }
    CHECK(WSAStartup(MAKEWORD(2, 2), &winsock) == 0, 1);
    CHECK(papacc_test_security_fixture_init(&fixture), 2);
    {
        PST_TRUST_SOURCE wrong_trust_source;
        memset(&wrong_trust_source, 0, sizeof(wrong_trust_source));
        wrong_trust_source.struct_size = sizeof(wrong_trust_source);
        wrong_trust_source.api_version = PST_API_VERSION;
        wrong_trust_source.kind = PST_TRUST_SOURCE_SYSTEM;
        CHECK(pst_trust_create(&wrong_trust_source, &wrong_trust) ==
            PST_RESULT_OK, 92);
    }
    CHECK(papacc_logger_init(&logger, discard_log, NULL,
        PAPACC_LOG_LEVEL_OFF) == PAPACC_RESULT_OK, 3);
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
    security.principal_resolve = resolve_principal;
    security.principal_resolve_context = &policy;
    security.authorize = authorize;
    security.authorization_context = &policy;
    CHECK(papacc_security_composition_init(&composition,
        &security.composition_inputs) == PAPACC_RESULT_OK, 4);

    CHECK(papacc_tcp_platform_init(&network.tcp_platform) == PAPACC_RESULT_OK &&
        papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1) ==
            PAPACC_RESULT_OK &&
        papacc_tcp_socket_win32_bind(&network.tcp_platform, &target, 0,
            &entry.socket) == PAPACC_RESULT_OK &&
        papacc_tcp_socket_win32_listen(&entry.socket) == PAPACC_RESULT_OK &&
        getsockname(entry.socket.native_socket,
            (struct sockaddr *)&native_address, &native_length) != SOCKET_ERROR,
        5);
    port = (PAPACC_U16)ntohs(native_address.sin_port);
    entry.target = target;
    network.listener_set.entries = &entry;
    network.listener_set.capacity = network.listener_set.count = 1U;
    network.listener_set.bound_port = port;
    network.listener_set.is_active = PAPACC_TRUE;
    network.listener_storage = &entry;
    network.listener_storage_capacity = 1U;
    network.is_active = PAPACC_TRUE;
    for (index = 0U; index < CAPACITY; ++index)
        contexts[index] = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
            PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    CHECK(papacc_server_acceptor_win32_init(&acceptor, &network, connections,
        CAPACITY, contexts, CAPACITY) == PAPACC_RESULT_OK, 6);
    CHECK(papacc_server_io_loop_win32_init(&protocol_loop, &network, &acceptor,
        sessions, CAPACITY, channels, CAPACITY, associations, CAPACITY,
        protocol_slots, CAPACITY, 5000000000ULL) == PAPACC_RESULT_OK, 7);
    {
        struct sockaddr_in fixture_address;
        int fixture_length = (int)sizeof(fixture_address);
        u_long nonblocking = 1UL;
        memset(&fixture_address, 0, sizeof(fixture_address));
        fixture_address.sin_family = AF_INET;
        fixture_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        fixture_listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        CHECK(fixture_listener != INVALID_SOCKET && bind(fixture_listener,
            (const struct sockaddr *)&fixture_address,
            (int)sizeof(fixture_address)) != SOCKET_ERROR &&
            listen(fixture_listener, 1) != SOCKET_ERROR &&
            getsockname(fixture_listener, (struct sockaddr *)&fixture_address,
                &fixture_length) != SOCKET_ERROR &&
            ioctlsocket(fixture_listener, FIONBIO, &nonblocking) != SOCKET_ERROR,
            68);
        CHECK(papacc_ip_address_set_ipv4(&egress_fixture.endpoint.address,
            127U, 0U, 0U, 1U) == PAPACC_RESULT_OK, 69);
        egress_fixture.endpoint.port =
            (PAPACC_U16)ntohs(fixture_address.sin_port);
    }
    egress_policy.master_enabled = PAPACC_TRUE;
    egress_policy.explicitly_allowed_sensitive_classes =
        PAPACC_EGRESS_SENSITIVE_LOOPBACK;
    egress_policy.host_policy = allow_fixture_host;
    egress_policy.endpoint_policy = allow_fixture_endpoint;
    egress_policy.context = &egress_fixture;
    egress_profile.profile_id = 1U;
    memcpy(egress_profile.name, "network-fixture", 16U);
    egress_profile.use_default_route = PAPACC_TRUE;
    egress_profile.available = PAPACC_TRUE;
    egress_config.allow_network_egress = PAPACC_TRUE;
    egress_config.policy = &egress_policy;
    egress_config.profiles = &egress_profile;
    egress_config.profile_count = 1U;
    egress_config.select_profile = select_fixture_profile;
    egress_config.select_profile_context = &egress_fixture;
    resolver.context = &egress_fixture;
    resolver.resolve = resolve_fixture;
    memset(&outbound_policy, 0, sizeof(outbound_policy));
    outbound_policy.runtime = composition.runtime;
    outbound_policy.trust = fixture.trust;
    outbound_policy.provider_id = "openssl";
    outbound_policy.alpn = (const PAPACC_U8 *)"papacc/1";
    outbound_policy.alpn_size = 8U;
    outbound_policy.alpn_required = PAPACC_TRUE;
    CHECK(papacc_server_io_loop_win32_set_egress_config(&protocol_loop,
            &egress_config) == PAPACC_RESULT_OK &&
        papacc_server_io_loop_win32_set_dns_resolver(&protocol_loop,
            &resolver) == PAPACC_RESULT_OK &&
        papacc_server_io_loop_win32_set_outbound_tls(&protocol_loop,
            &outbound_policy) == PAPACC_RESULT_OK, 70);
    listener.bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    listener.port = port;
    listener.transport_profile =
        PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL;
    CHECK(papacc_security_configuration_ref_set(
        &listener.security_configuration_ref, (const PAPACC_U8 *)"test", 4U) ==
        PAPACC_RESULT_OK, 8);
    CHECK(papacc_server_secure_io_win32_init(&secure_io, &network,
        &listener_set, &protocol_loop, &composition, &security, candidates,
        CAPACITY, scheduler_members, scheduler_events, CAPACITY + 1U +
            PAPACC_SERVER_NETWORK_STREAM_CAPACITY,
        listener_sources, listener_tokens, 1U, 5000000000ULL) ==
        PAPACC_RESULT_OK, 9);

    if(argc==2&&strcmp(argv[1],"--isolated-tls-truncated-matrix")==0){
        const char*scenarios[2]={"network-tls-truncated-v1",
            "network-tls-truncated-v2"};
        PAPACC_SIZE response_sizes[2]={100U,600U};
        PAPACC_SIZE fixture_index;
        for(fixture_index=0U;fixture_index<2U;++fixture_index){
            REFERENCE_FIXTURE_THREAD isolated_fixture;
            HANDLE isolated_thread;
            memset(&isolated_fixture,0,sizeof(isolated_fixture));
            isolated_fixture.listener=fixture_listener;
            isolated_fixture.runtime=composition.runtime;
            isolated_fixture.credentials=fixture.server.credentials;
            isolated_fixture.tls=PAPACC_TRUE;
            isolated_fixture.truncate_after_write=PAPACC_TRUE;
            isolated_fixture.response_size=response_sizes[fixture_index];
            isolated_thread=CreateThread(NULL,0U,reference_fixture_thread,
                &isolated_fixture,0U,NULL);
            CHECK(isolated_thread!=NULL&&run_reference_client(port,&secure_io,
                scenarios[fixture_index],PAPACC_FALSE,
                egress_fixture.endpoint.port),115);
            for(index=0U;index<5000U&&WaitForSingleObject(isolated_thread,0U)==
                    WAIT_TIMEOUT;++index)
                CHECK(papacc_server_secure_io_win32_poll_once(&secure_io,1U)==
                    PAPACC_RESULT_OK,116);
            CHECK(WaitForSingleObject(isolated_thread,5000U)==WAIT_OBJECT_0&&
                isolated_fixture.result,117);
            CloseHandle(isolated_thread);
        }
        result=0;
        goto cleanup;
    }
    if (argc == 2 && strncmp(argv[1], "--isolated-", 11U) == 0) {
        const char *client_scenario = NULL;
        HANDLE isolated_fixture_threads[3] = { NULL, NULL, NULL };
        REFERENCE_FIXTURE_THREAD isolated_fixtures[3];
        PAPACC_SIZE isolated_fixture_count = 0U, fixture_index;
        AI_CASE_C_OBSERVER ai_observer = {0};
        HANDLE g1_site_release_event = NULL;
        HANDLE p10_marker_received_event = NULL;
        HANDLE p10_owner_stop_event = NULL;
        PAPACC_BOOL ai_enabled = PAPACC_FALSE;
        PAPACC_BOOL isolated_client_result = PAPACC_FALSE;
        if (strcmp(argv[1], "--isolated-negative-no-negotiation") == 0)
            client_scenario = "network-negative-no-negotiation";
        else if (strcmp(argv[1], "--isolated-negative-missing-tls") == 0)
            client_scenario = "network-negative-missing-tls";
        else if (strcmp(argv[1], "--isolated-negative-destination") == 0)
            client_scenario = "network-negative-destination";
        else if (strcmp(argv[1], "--isolated-negative-tls-name") == 0)
            client_scenario = "network-negative-tls-name";
        else if (strcmp(argv[1], "--isolated-network-tls-transfer") == 0)
            client_scenario = "network-tls";
        else if (strcmp(argv[1], "--isolated-p10-normal-remove") == 0)
            client_scenario = "network-tls";
        else if (strcmp(argv[1], "--isolated-tcp-abort") == 0)
            client_scenario = "network-tcp-abort";
        else if (strcmp(argv[1], "--isolated-tls-half-close") == 0)
            client_scenario = "network-tls-half-close";
        else if (strcmp(argv[1], "--isolated-tls-abort") == 0)
            client_scenario = "network-tls-abort";
        else if (strcmp(argv[1], "--isolated-tls-reset") == 0)
            client_scenario = "network-tls-reset";
        else if (strcmp(argv[1], "--isolated-tls-shutdown-cancel") == 0)
            client_scenario = "network-tls-shutdown-cancel";
        else if (strcmp(argv[1], "--isolated-tls-shutdown-deadline") == 0)
            client_scenario = "network-tls-shutdown-deadline";
        else if (strcmp(argv[1], "--isolated-tls-control-loss") == 0)
            client_scenario = "network-tls-control-loss";
        else if (strcmp(argv[1], "--isolated-g1-duplicate-close") == 0)
            client_scenario = "network-g1-duplicate-close";
        else if (strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0)
            client_scenario = "network-g2-duplicate-abort";
        else if (strcmp(argv[1], "--isolated-multiple-recovery") == 0)
            client_scenario = "network-multiple-recovery";
        CHECK(client_scenario != NULL, 110);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        if((strcmp(argv[1],"--isolated-g1-duplicate-close")==0||
               strcmp(argv[1],"--isolated-g2-duplicate-abort")==0)){
            if(strcmp(argv[1],"--isolated-g1-duplicate-close")==0){
                CHECK(ai_g1_publication_oracle_negatives(),119);
                puts("G1_ORACLE_NEGATIVES=9_OF_9_PASS duplicate_trigger=DETECTED duplicate_success=DETECTED operation_mismatch=DETECTED generation_mismatch=DETECTED terminal_type_mismatch=DETECTED cause_overwrite=DETECTED second_acceptance=DETECTED unauthorized_attempt=DETECTED effective_publication_after_data_terminal=DETECTED");
            }
        }
#endif
        memset(isolated_fixtures, 0, sizeof(isolated_fixtures));
        if ((strcmp(argv[1], "--isolated-g1-duplicate-close") == 0 ||
            strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0)) {
            g1_site_release_event = CreateEventA(NULL, TRUE, FALSE, NULL);
            CHECK(g1_site_release_event != NULL, 111);
        }
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        if(strcmp(argv[1],"--isolated-tls-shutdown-cancel")==0){
            char p10_mode[32];
            DWORD p10_length=GetEnvironmentVariableA(
                "PAPACC_ADR0032_P10",p10_mode,(DWORD)sizeof(p10_mode));
            if((p10_length==4U&&memcmp(p10_mode,"once",4U)==0)||
               (p10_length==10U&&memcmp(p10_mode,"persistent",10U)==0)){
                p10_marker_received_event=CreateEventA(NULL,TRUE,FALSE,NULL);
                p10_owner_stop_event=CreateEventA(NULL,TRUE,FALSE,NULL);
                CHECK(p10_marker_received_event!=NULL&&
                    p10_owner_stop_event!=NULL,111);
            }
        }
#endif
        if (strcmp(argv[1], "--isolated-multiple-recovery") == 0)
            isolated_fixture_count = 3U;
        if (strcmp(argv[1], "--isolated-network-tls-transfer") == 0 ||
            strcmp(argv[1], "--isolated-p10-normal-remove") == 0 ||
            strcmp(argv[1], "--isolated-negative-tls-name") == 0 ||
            strcmp(argv[1], "--isolated-tcp-abort") == 0 ||
            strcmp(argv[1], "--isolated-tls-half-close") == 0 ||
            strcmp(argv[1], "--isolated-tls-abort") == 0 ||
            strcmp(argv[1], "--isolated-tls-reset") == 0 ||
            strcmp(argv[1], "--isolated-tls-shutdown-cancel") == 0 ||
            strcmp(argv[1], "--isolated-tls-shutdown-deadline") == 0 ||
            strcmp(argv[1], "--isolated-tls-control-loss") == 0 ||
            (strcmp(argv[1], "--isolated-g1-duplicate-close") == 0 ||
            strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0))
            isolated_fixture_count = 1U;
        for (fixture_index = 0U; fixture_index < isolated_fixture_count;
                ++fixture_index) {
            isolated_fixtures[fixture_index].listener = fixture_listener;
            isolated_fixtures[fixture_index].runtime = composition.runtime;
            isolated_fixtures[fixture_index].credentials =
                fixture.server.credentials;
            isolated_fixtures[fixture_index].tls =
                strcmp(argv[1], "--isolated-network-tls-transfer") == 0 ||
                strcmp(argv[1], "--isolated-p10-normal-remove") == 0 ||
                strcmp(argv[1], "--isolated-negative-tls-name") == 0 ||
                strcmp(argv[1], "--isolated-tls-half-close") == 0 ||
                strcmp(argv[1], "--isolated-tls-abort") == 0 ||
                strcmp(argv[1], "--isolated-tls-reset") == 0 ||
                strcmp(argv[1], "--isolated-tls-shutdown-cancel") == 0 ||
                strcmp(argv[1], "--isolated-tls-shutdown-deadline") == 0 ||
                strcmp(argv[1], "--isolated-tls-control-loss") == 0 ||
                (strcmp(argv[1], "--isolated-g1-duplicate-close") == 0 ||
            strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0) ?
                PAPACC_TRUE : PAPACC_FALSE;
            isolated_fixtures[fixture_index].reset_after_read =
                strcmp(argv[1], "--isolated-tls-reset") == 0 ?
                PAPACC_TRUE : PAPACC_FALSE;
            isolated_fixtures[fixture_index].peer_close_after_read =
                strcmp(argv[1], "--isolated-tls-shutdown-cancel") == 0 ||
                strcmp(argv[1], "--isolated-tls-shutdown-deadline") == 0 ||
                strcmp(argv[1], "--isolated-tls-control-loss") == 0 ? PAPACC_TRUE : PAPACC_FALSE;
            isolated_fixtures[fixture_index].abort_mode =
                strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0 ?
                PAPACC_TRUE : PAPACC_FALSE;
            isolated_fixtures[fixture_index].expected_size =
                (strcmp(argv[1], "--isolated-network-tls-transfer") == 0 ||
                 strcmp(argv[1], "--isolated-p10-normal-remove") == 0) ?
                32768U : (strcmp(argv[1], "--isolated-g1-duplicate-close") == 0 ||
            strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0) ?
                32768U : strcmp(argv[1], "--isolated-multiple-recovery") == 0 ?
                512U : 0U;
            isolated_fixtures[fixture_index].hold_open_until_event =
                (strcmp(argv[1], "--isolated-g1-duplicate-close") == 0 ||
            strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0) ?
                g1_site_release_event : NULL;
            isolated_fixtures[fixture_index].p10_marker_received_event=
                p10_marker_received_event;
            isolated_fixtures[fixture_index].p10_owner_stop_event=
                p10_owner_stop_event;
            InterlockedExchange(
                &isolated_fixtures[fixture_index].diagnostic_listener_ready,
                1L);
            isolated_fixture_threads[fixture_index] = CreateThread(NULL, 0U,
                reference_fixture_thread, &isolated_fixtures[fixture_index],
                0U, NULL);
            CHECK(isolated_fixture_threads[fixture_index] != NULL, 111);
        }
        if (strcmp(argv[1], "--isolated-tls-reset") == 0 ||
            strcmp(argv[1], "--isolated-p10-normal-remove") == 0 ||
            strcmp(argv[1], "--isolated-tls-shutdown-cancel") == 0 ||
            strcmp(argv[1], "--isolated-tls-shutdown-deadline") == 0 ||
            strcmp(argv[1], "--isolated-tls-control-loss") == 0 ||
            (strcmp(argv[1], "--isolated-g1-duplicate-close") == 0 ||
            strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0)) {
            ai_observer_init(&ai_observer);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            ai_observer.p10_fixture=&isolated_fixtures[0];
            if ((strcmp(argv[1], "--isolated-g1-duplicate-close") == 0 ||
            strcmp(argv[1], "--isolated-g2-duplicate-abort") == 0)) {
                LARGE_INTEGER g1_counter;
                (void)QueryPerformanceCounter(&g1_counter);
                ai_observer.g1_site_release_event = g1_site_release_event;
                ai_observer.g2_mode = strcmp(argv[1],
                    "--isolated-g2-duplicate-abort") == 0 ?
                    PAPACC_TRUE : PAPACC_FALSE;
                ai_observer.secure_io = &secure_io;
                ai_observer.g1_fixture = &isolated_fixtures[0];
                (void)sprintf_s(ai_observer.g1_run_id,
                    sizeof(ai_observer.g1_run_id),"g1-%lu-%llu",
                    (unsigned long)GetCurrentProcessId(),
                    (unsigned long long)g1_counter.QuadPart);
                (void)sprintf_s(ai_observer.g1_trace_path,
                    sizeof(ai_observer.g1_trace_path),
                    "papacc-g1-oracle-%s.trace",ai_observer.g1_run_id);
            }
#endif
            ai_observer.observe_shutdown =
                strcmp(argv[1], "--isolated-tls-reset") != 0 &&
                strcmp(argv[1], "--isolated-p10-normal-remove") != 0 &&
                strcmp(argv[1], "--isolated-g1-duplicate-close") != 0 &&
                strcmp(argv[1], "--isolated-g2-duplicate-abort") != 0 ?
                PAPACC_TRUE : PAPACC_FALSE;
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
            if(p10_native_mode>=0)ai_observer.observe_shutdown=PAPACC_FALSE;
#endif
            ai_observer.cancel_on_pending = strcmp(argv[1], "--isolated-tls-shutdown-cancel") == 0 ? PAPACC_TRUE : PAPACC_FALSE;
            ai_observer.signal_control_loss = strcmp(argv[1],
                "--isolated-tls-control-loss") == 0 ? PAPACC_TRUE : PAPACC_FALSE;
            ai_active_observer = &ai_observer;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            if((strcmp(argv[1],"--isolated-g1-duplicate-close")==0||
               strcmp(argv[1],"--isolated-g2-duplicate-abort")==0||
               strcmp(argv[1],"--isolated-p10-normal-remove")==0
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
               ||p10_native_mode>=0
#endif
               ))
                CHECK(papacc_server_io_loop_win32_test_set_owner_terminal_observer(
                    &protocol_loop,ai_owner_terminal_observe,&ai_observer)==
                    PAPACC_RESULT_OK,117);
#endif
            ai_enabled = PAPACC_TRUE;
        }
        isolated_client_result=run_reference_client(port,&secure_io,
            client_scenario,PAPACC_FALSE,egress_fixture.endpoint.port)?
            PAPACC_TRUE:PAPACC_FALSE;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
        if(ai_observer.p10_remove_mode==
                PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT&&
           ai_observer.p10_normal_only!=PAPACC_TRUE){
            CHECK(p10_persistent_verified==PAPACC_TRUE,123);
            printf("P10_FAIL_PERSISTENT=PASS P10_FINAL_STATE=ERROR_TERMINAL "
                "P10_RETENTION_CHECKS=%lu P10_NO_NORMAL_REMOVE_BYPASS=PASS "
                "P10_LATE_CALLBACK=REJECTED P10_MANAGER_REMOVE=REJECTED "
                "P10_REPEATED_STOP=ERROR_PRESERVED P10_NO_RESURRECTION=PASS "
                "P10_NATIVE_NEGATIVES=12_OF_12_PASS P10_STATE_NEGATIVES=6_OF_6_PASS\n",
                (unsigned long)p10_retention_checks);
            printf("P10_TOKEN=%08lX%08lX PST_CONNECTION_ID=%lu SOCKET_LIFETIME_ID=%lu "
                "P10_RELEASE=0 P10_PROVIDER_DESTROY=0 P10_NATIVE_CLOSE=0 "
                "P10_TRANSPORT_DESTROY=0 P10_CAUSE_PRESERVED=YES "
                "P10_DEADLINE_PRESERVED=YES P10_OWNER_STRUCTURES_RETAINED=YES\n",
                (unsigned long)p10_initial_native.token.high,
                (unsigned long)p10_initial_native.token.low,
                (unsigned long)p10_initial_native.connection_id,
                (unsigned long)p10_initial_native.socket_lifetime_id);
            /* ERROR_TERMINAL retains all owners until process exit (ADR-0030).
             * The reference subprocess was cancelled; it is not a wire PASS. */
            return 0;
        }
#endif
        if(ai_observer.p10_normal_only==PAPACC_TRUE){
            PAPACC_SIZE p10_index;
            ai_observer.p10_membership_after_normal=
                secure_io.scheduler.member_count;
            for(p10_index=0U;
                    p10_index<PAPACC_SERVER_NETWORK_STREAM_CAPACITY;
                    ++p10_index)
                if(secure_io.outbound_ready[p10_index].registered==PAPACC_TRUE&&
                   secure_io.outbound_ready[p10_index].connection==
                    secure_io.test_remove_selected_connection)
                    ai_observer.p10_selected_membership_after_normal=
                        PAPACC_TRUE;
        }
        if(ai_observer.p10_remove_mode!=
                PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF&&
            isolated_client_result!=PAPACC_TRUE)
            ai_p10_write_trace(&ai_observer,"REFERENCE_CLIENT_STAGE41");
#endif
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
        if(strcmp(argv[1],"--isolated-g2-duplicate-abort")==0)
            ai_g1_emit_diagnostics(&ai_observer,&protocol_loop);
#endif
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
        if(p10_native_mode==2){
            CHECK(p10_native_consumer_final(&ai_observer),125);
            return 0; /* ERROR_TERMINAL owners retained until process boundary. */
        }
#endif
        CHECK(isolated_client_result==PAPACC_TRUE,112);
        for (fixture_index = 0U; fixture_index < isolated_fixture_count;
                ++fixture_index) {
            DWORD fixture_wait_result;
            DWORD fixture_wait_error = ERROR_SUCCESS;
            for (index = 0U; index < 5000U && WaitForSingleObject(
                    isolated_fixture_threads[fixture_index], 0U) ==
                    WAIT_TIMEOUT; ++index)
                CHECK(papacc_server_secure_io_win32_poll_once(&secure_io,
                    1U) == PAPACC_RESULT_OK, 113);
            if (ai_enabled == PAPACC_TRUE)
                ai_observe(&secure_io, &ai_observer);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            if((strcmp(argv[1],"--isolated-g1-duplicate-close")==0||
               strcmp(argv[1],"--isolated-g2-duplicate-abort")==0))
                ai_g1_emit_diagnostics(&ai_observer,&protocol_loop);
#endif
            fixture_wait_result=WaitForSingleObject(
                isolated_fixture_threads[fixture_index],5000U);
            if(fixture_wait_result==WAIT_FAILED)
                fixture_wait_error=GetLastError();
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            if(ai_observer.p10_remove_mode!=
                    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF){
                ai_p10_write_trace(&ai_observer,"PRE_CHECK114");
                ai_p10_write_fixture_trace(
                    &isolated_fixtures[fixture_index],&ai_observer,
                    fixture_wait_result,fixture_wait_error);
            }
#endif
            CHECK(fixture_wait_result==WAIT_OBJECT_0&&
                (strcmp(argv[1], "--isolated-negative-tls-name") == 0 ||
                isolated_fixtures[fixture_index].result),114);
            CloseHandle(isolated_fixture_threads[fixture_index]);
        }
        if(p10_marker_received_event!=NULL){
            CloseHandle(p10_marker_received_event);
            p10_marker_received_event=NULL;
        }
        if(p10_owner_stop_event!=NULL){
            CloseHandle(p10_owner_stop_event);
            p10_owner_stop_event=NULL;
        }
#ifdef PAPACC_P10_NORMAL_NATIVE_OBSERVER
        if(p10_native_mode>=0){
            CHECK(p10_native_consumer_final(&ai_observer),125);
            CHECK(ai_p9_run(&secure_io,&protocol_loop,&ai_observer),126);
            ai_active_observer=NULL;
            result=0;
            goto cleanup;
        }
#endif
        if (ai_enabled == PAPACC_TRUE) {
            const char *resource_cause=ai_observed_terminal_cause(&ai_observer);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            if((strcmp(argv[1],"--isolated-g1-duplicate-close")==0||
               strcmp(argv[1],"--isolated-g2-duplicate-abort")==0)){
                CHECK(ai_observer.g2_mode==PAPACC_TRUE?
                    ai_g2_observer_valid(&ai_observer):
                    ai_g1_observer_valid(&ai_observer),115);
                resource_cause="NONE";
                if(ai_p9_requested()==PAPACC_TRUE)
                    CHECK(ai_observer.g2_mode!=PAPACC_TRUE&&
                        ai_p9_run(&secure_io,&protocol_loop,&ai_observer),120);
            }else
#endif
            {
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
                if(ai_observer.p10_normal_only==PAPACC_TRUE){
                    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION p10_observation;
                    CHECK(ai_p10_normal_valid(&ai_observer,&p10_observation),115);
                    CHECK(ai_p10_normal_negatives(
                        &p10_observation,&ai_observer),120);
                    CHECK(ai_p10_normal_reporting_negatives(
                        &p10_observation,&ai_observer),121);
                    CHECK(ai_p10_normal_reporting_valid(&ai_observer,
                        &p10_observation,PAPACC_TRUE,PAPACC_TRUE),122);
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
                    CHECK(ai_p10_check_completed(&ai_observer),124);
#endif
                    resource_cause="NONE";
                    printf("P10_NORMAL_REMOVE_UNAFFECTED=PASS P10_NORMAL_REMOVE_MODE=%s P10_NORMAL_REMOVE_ATTEMPTS=%lu P10_NORMAL_REMOVE_SUCCESSES=%lu P10_INJECTED_FAILURES=%lu\n",
                        ai_observer.p10_remove_mode==
                            PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT?
                            "PERSISTENT":"ONCE",
                        (unsigned long)p10_observation.normal_remove_attempt_count,
                        (unsigned long)p10_observation.normal_remove_success_count,
                        (unsigned long)p10_observation.injected_failure_count);
                    printf("P10_MEMBERSHIP_BEFORE=%lu P10_MEMBERSHIP_AFTER=%lu P10_SELECTED_MEMBERSHIP_AFTER=NO P10_NORMAL_NEGATIVES=10_OF_10_PASS P10_REPORTING_NEGATIVES=7_OF_7_PASS\n",
                        (unsigned long)ai_observer.p10_membership_before,
                        (unsigned long)ai_observer.p10_membership_after_normal);
                }else if(ai_observer.p10_remove_mode==
                        PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE){
                    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION p10_observation;
                    CHECK(ai_p10_once_valid(&ai_observer,&p10_observation),115);
                    CHECK(ai_p10_negatives(&p10_observation,&ai_observer),120);
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
                    CHECK(ai_p10_check_completed(&ai_observer),124);
#endif
                    printf("P10_SEAM_STOP_SCOPED=PASS P10_FAIL_ONCE=PASS P10_IDENTITY_MATCH=PASS P10_OWNER_THREAD=PASS P10_DISPATCH_SAFETY=PASS\n");
                    printf("P10_GLOBAL_REMOVE_ATTEMPTS=%lu P10_GLOBAL_REAL_REMOVE_CALLS=%lu P10_GLOBAL_REAL_REMOVE_SUCCESSES=%lu P10_NORMAL_REMOVE_ATTEMPTS=%lu\n",
                        (unsigned long)p10_observation.remove_attempt_count,
                        (unsigned long)p10_observation.real_remove_call_count,
                        (unsigned long)p10_observation.real_remove_success_count,
                        (unsigned long)p10_observation.normal_remove_attempt_count);
                    printf("P10_TARGET_STOP_REMOVE_ATTEMPTS=%lu P10_TARGET_INJECTED_FAILURES=%lu P10_TARGET_REAL_REMOVE_CALLS=%lu P10_TARGET_REAL_REMOVE_SUCCESSES=%lu P10_OTHER_STOP_REMOVE_ATTEMPTS=%lu\n",
                        (unsigned long)p10_observation.target_stop_remove_attempt_count,
                        (unsigned long)p10_observation.target_injected_failure_count,
                        (unsigned long)p10_observation.target_real_remove_call_count,
                        (unsigned long)p10_observation.target_real_remove_success_count,
                        (unsigned long)p10_observation.other_stop_remove_attempt_count);
                    puts("P10_TARGET_MEMBERSHIP_PRESERVED_AFTER_FAILURE=YES P10_TARGET_MEMBERSHIP_CLEARED_AFTER_RECOVERY=YES P10_CONNECTION_PRESERVED=YES P10_PROCESSOR_PRESERVED=YES P10_PST_RELEASE_AFTER_REMOVE=YES P10_DESTRUCTIVE_CLOSE_AFTER_REMOVE=YES P10_CAUSE_PRESERVED=YES P10_DEADLINE_PRESERVED=YES P10_FINAL_STOP_STATE=STOPPED P10_NO_RESURRECTION=YES P10_NEGATIVES=12_OF_12_PASS");
                }else
#endif
                CHECK(strcmp(argv[1], "--isolated-tls-shutdown-deadline") == 0 ?
                    ai_e_observer_valid(&ai_observer) :
                    (strcmp(argv[1], "--isolated-tls-control-loss") == 0 ?
                        ai_f_observer_valid(&ai_observer) :
                        (ai_observer.observe_shutdown == PAPACC_TRUE ?
                            ai_d_observer_valid(&ai_observer) :
                            ai_observer_valid(&ai_observer))), 115);
                CHECK(resource_cause != NULL, 116);
            }
            ai_print_resource(&ai_observer.pst, "NETWORK_STREAM_TLS_IO",
                resource_cause);
            ai_print_resource(&ai_observer.socket_value, "PST_TRANSFERRED",
                resource_cause);
            ai_print_resource(&ai_observer.stream_value,
                "NETWORK_STREAM_MANAGER",
                resource_cause);
            ai_print_resource(&ai_observer.data, "SESSION_DATA_CHANNEL",
                resource_cause);
            puts("PST_RELEASE_EXACTLY_ONCE=PASS");
            puts("SOCKET_CLOSE_EXACTLY_ONCE=PASS");
            puts("STREAM_DESTROY_EXACTLY_ONCE=PASS");
            puts("DATA_DETACH_EXACTLY_ONCE=PASS");
            if((strcmp(argv[1],"--isolated-g1-duplicate-close")==0||
               strcmp(argv[1],"--isolated-g2-duplicate-abort")==0)){
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
                printf("G1_REAL_TLS_REQUIRED=YES G1_TERMINAL_EVENTS_RECEIVED=%lu G1_TERMINAL_EVENTS_VALIDATED=%lu G1_TERMINAL_ACCEPTED_COUNT=%lu G1_TERMINAL_SUPPRESSED_COUNT=%lu G1_PUBLICATION_ATTEMPTS=%lu G1_PUBLICATION_SUCCESSES=%lu G1_EFFECTIVE_TERMINAL=NETWORK_CLOSE G1_TERMINAL_CAUSE=NONE\n",
                    (unsigned long)ai_observer.owner_events_received,
                    (unsigned long)ai_observer.owner_events_validated,
                    (unsigned long)ai_observer.owner_terminals_accepted,
                    (unsigned long)ai_observer.owner_terminals_suppressed,
                    (unsigned long)ai_observer.owner_publication_attempts,
                    (unsigned long)ai_observer.owner_publication_successes);
                puts("G1_DUPLICATE_CLOSE=PASS G1_EXACTLY_ONCE_TEARDOWN=PASS G1_NO_RESURRECTION=YES");
#endif
            }else if(strcmp(argv[1],"--isolated-tls-control-loss")==0){
                puts("F_REAL_TLS_REQUIRED=YES F_PST_CLOSE_CLEAN=YES F_REAL_NEED_WRITE=YES F_INTEREST_WRITE=YES");
                puts("F_CONTROL_LOSS_AFTER_PENDING=YES F_OWNER_CAUSE=CONTROL_TRANSPORT_LOSS F_CASCADE_COUNT=1");
                puts("F_DATA_DEPENDENT_TEARDOWN=PASS F_NO_RESURRECTION=YES F_EXACTLY_ONCE_TEARDOWN=PASS");
            }else if(strcmp(argv[1],"--isolated-tls-shutdown-deadline")==0){
                puts("E_PST_CLOSE_CLEAN=YES E_REAL_NEED_WRITE=YES E_INTEREST_WRITE=YES");
                puts("E_OWNER_DEADLINE_ARMED=YES E_DEADLINE_EXPIRED=YES E_PENDING_AT_EXPIRY=YES E_TERMINAL_CAUSE=SHUTDOWN_DEADLINE");
                puts("E_OWNER_FINAL_STATE=TERMINAL E_FALSE_CLEAN_SUCCESS=NO E_EXACTLY_ONCE_TEARDOWN=PASS");
            }
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            else if(ai_observer.p10_normal_only==PAPACC_TRUE){
                puts("P10_NORMAL_CLOSE=PASS");
            }else if(ai_observer.p10_remove_mode!=
                        PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF){
                puts("P10_STOP_REMOVE_RECOVERY=PASS P10_PRE_CLOSE_PENDING=EXERCISED_REAL_ROOT");
            }
#endif
            else if(ai_observer.observe_shutdown==PAPACC_TRUE){
                puts("D_PST_CLOSE_CLEAN_OBSERVED=YES PST_OPERATION_CLOSED=YES PST_CLOSE_CLEAN=YES");
                puts("D_REAL_WRITE_BLOCKED=YES D_PST_NEED_WRITE=YES D_PST_INTEREST_WRITE=YES");
                puts("D_STOP_AFTER_PENDING=YES D_STOP_DELIVERED_TO_OWNER=YES D_OWNER_CANCEL_STATE=STOPPED");
                puts("D_TERMINAL_CAUSE=SERVER_STOP_REQUESTED D_FALSE_CLEAN_SUCCESS=NO");
                puts("D_TERMINAL_PUBLICATION_ATTEMPTS=0 D_TERMINAL_PUBLICATION_SUCCESSES=0 D_TERMINAL_TYPE=NONE D_TERMINAL_COUNT=0");
                puts("D_EXACTLY_ONCE_TEARDOWN=PASS");
            }else puts("C_EXACTLY_ONCE_TEARDOWN=PASS");
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
            if((strcmp(argv[1],"--isolated-g1-duplicate-close")==0||
               strcmp(argv[1],"--isolated-g2-duplicate-abort")==0))
                CHECK(papacc_server_io_loop_win32_test_set_owner_terminal_observer(
                    &protocol_loop,NULL,NULL)==PAPACC_RESULT_OK,118);
#endif
            ai_active_observer = NULL;
        }
        if (g1_site_release_event != NULL) {
            CloseHandle(g1_site_release_event);
            g1_site_release_event = NULL;
        }
        if (strcmp(argv[1], "--isolated-network-tls-transfer") == 0) {
            /* The transfer oracle precedes Stop. Complete owner reconciliation
             * for the acquired CONTROL candidate before generic harness cleanup. */
            CHECK(papacc_server_secure_io_win32_request_stop(&secure_io) ==
                PAPACC_RESULT_OK, 126);
            for (index = 0U; index < 5000U &&
                    papacc_server_secure_io_win32_is_stopped(&secure_io) !=
                        PAPACC_TRUE; ++index)
                CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
                    PAPACC_RESULT_OK, 127);
            CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
                PAPACC_TRUE && protocol_loop.connection_manager->count == 0U,
                128);
            for (index = 0U; index < secure_io.candidate_capacity; ++index)
                CHECK(candidates[index].state ==
                        PAPACC_SERVER_SECURE_CANDIDATE_UNUSED ||
                    (candidates[index].state ==
                        PAPACC_SERVER_SECURE_CANDIDATE_CLOSED &&
                     candidates[index].transport_adapter.registered ==
                        PAPACC_FALSE &&
                     candidates[index].secure_processor.connection == NULL),
                    129);
            CHECK(papacc_server_secure_io_win32_try_shutdown(&secure_io) ==
                PAPACC_RESULT_OK, 130);
            puts("P11_P12_NORMAL_TRANSFER_FINAL_OWNER_RECONCILIATION=PASS");
        }
        result = 0;
        goto cleanup;
    }
    CHECK(argc == 1, 109);

    client_socket = connect_loopback(port);
    CHECK(client_socket != INVALID_SOCKET, 10);
    papacc_test_pst_client_config(&client_config,
        fixture.client_a.credentials, fixture.trust);
    CHECK(papacc_pst_secure_processor_init(&client, composition.runtime,
        &client_config, ~(PAPACC_U64)0) == PAPACC_RESULT_OK, 11);
    CHECK(pst_win32_socket_transport_create((pst_size)client_socket,
        &client_transport) == PST_RESULT_OK, 12);
    CHECK(papacc_pst_secure_processor_attach(&client, client_transport,
        &ownership) == PAPACC_RESULT_OK && ownership == PAPACC_TRUE, 13);
    client_transport = NULL;
    client_socket = INVALID_SOCKET;
    for (index = 0U; index < 500U && client.state !=
            PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED; ++index) {
        CHECK(papacc_pst_secure_processor_handshake_once(&client, &step) ==
            PAPACC_RESULT_OK, 14);
        server_result = papacc_server_secure_io_win32_poll_once(
            &secure_io, 10U);
        if (server_result != PAPACC_RESULT_OK)
            fprintf(stderr, "server_result=%d round=%zu candidate=%d members=%zu\n",
                (int)server_result, (size_t)index, (int)candidates[0].state,
                (size_t)secure_io.scheduler.member_count);
        CHECK(server_result == PAPACC_RESULT_OK, 15);
    }
    CHECK(client.state == PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED &&
        policy.resolve_calls == 1U && policy.authorize_calls >= 1U, 16);

    for (index = 0U; index < 500U && offset < sizeof(control_open); ++index) {
        CHECK(papacc_pst_secure_processor_write_once(&client,
            control_open + offset, 1U,
            &transferred, &step) == PAPACC_RESULT_OK, 17);
        offset += transferred;
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 18);
    }
    CHECK(offset == sizeof(control_open), 19);
    for (index = 0U; index < 500U && response_size < sizeof(response); ++index) {
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 20);
        CHECK(papacc_pst_secure_processor_read_once(&client,
            response + response_size, sizeof(response) - response_size,
            &transferred, &step) == PAPACC_RESULT_OK, 21);
        response_size += transferred;
    }
    if (!(response_size == sizeof(response) &&
        memcmp(response, control_accept, sizeof(response)) == 0 &&
        protocol_loop.session_manager.count == 1U &&
        candidates[0].session_security.published == PAPACC_TRUE))
        fprintf(stderr, "response=%zu sessions=%zu published=%d kind=%d\n",
            (size_t)response_size,
            (size_t)protocol_loop.session_manager.count,
            (int)candidates[0].session_security.published,
            (int)protocol_slots[0].kind);
    CHECK(response_size == sizeof(response) &&
        memcmp(response, control_accept, sizeof(response)) == 0 &&
        protocol_loop.session_manager.count == 1U &&
        candidates[0].session_security.published == PAPACC_TRUE, 22);

    {
        const PAPACC_U8 negotiation[44] = {
            0x50,0x41,0x43,0x43,1,0,0,16,0,7,0,0,0,0,0,28,
            0,2,0,2,
            0,0,0,1,0,1, 0,0,0,2,0,1,
            0,0,0,1,0,1, 0,0,0,2,0,1 };
        PAPACC_U8 negotiation_result[30];
        /* One TLS write reproduces HEADER_READY with the complete capability
           payload already buffered inside the production protocol reader. */
        CHECK(write_all(&client, &secure_io, negotiation,
            sizeof(negotiation), sizeof(negotiation)), 71);
        CHECK(read_exact(&client, &secure_io, negotiation_result,
            sizeof(negotiation_result)) && negotiation_result[8] == 0U &&
            negotiation_result[9] == 8U && negotiation_result[16] == 0U &&
            negotiation_result[17] == 2U, 72);
    }

    offset = 0U;
    for (index = 0U; index < 500U && offset < sizeof(data_ticket_request);
            ++index) {
        CHECK(papacc_pst_secure_processor_write_once(&client,
            data_ticket_request + offset,
            sizeof(data_ticket_request) - offset, &transferred, &step) ==
            PAPACC_RESULT_OK, 23);
        offset += transferred;
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 24);
    }
    CHECK(offset == sizeof(data_ticket_request), 25);
    for (index = 0U; index < 500U && ticket_frame_size < sizeof(ticket_frame);
            ++index) {
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 26);
        CHECK(papacc_pst_secure_processor_read_once(&client,
            ticket_frame + ticket_frame_size,
            sizeof(ticket_frame) - ticket_frame_size, &transferred, &step) ==
            PAPACC_RESULT_OK, 27);
        ticket_frame_size += transferred;
    }
    CHECK(ticket_frame_size == sizeof(ticket_frame) &&
        ticket_frame[0] == 0x50 && ticket_frame[8] == 0x00 &&
        ticket_frame[9] == 0x04 && ticket_frame[15] == 0x10, 28);
    memcpy(data_attach + 16U, ticket_frame + 16U, 16U);

    client_socket = connect_loopback(port);
    CHECK(client_socket != INVALID_SOCKET, 29);
    papacc_test_pst_client_config(&client_config,
        fixture.client_a.credentials, fixture.trust);
    CHECK(papacc_pst_secure_processor_init(&data_client, composition.runtime,
        &client_config, ~(PAPACC_U64)0) == PAPACC_RESULT_OK, 30);
    CHECK(pst_win32_socket_transport_create((pst_size)client_socket,
        &client_transport) == PST_RESULT_OK, 31);
    CHECK(papacc_pst_secure_processor_attach(&data_client, client_transport,
        &ownership) == PAPACC_RESULT_OK && ownership == PAPACC_TRUE, 32);
    client_transport = NULL;
    client_socket = INVALID_SOCKET;
    for (index = 0U; index < 500U && data_client.state !=
            PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED; ++index) {
        CHECK(papacc_pst_secure_processor_handshake_once(&data_client, &step) ==
            PAPACC_RESULT_OK, 33);
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 34);
    }
    CHECK(data_client.state == PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED, 35);

    offset = 0U;
    for (index = 0U; index < 500U && offset < sizeof(data_attach); ++index) {
        CHECK(papacc_pst_secure_processor_write_once(&data_client,
            data_attach + offset, sizeof(data_attach) - offset,
            &transferred, &step) == PAPACC_RESULT_OK, 36);
        offset += transferred;
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 37);
    }
    CHECK(offset == sizeof(data_attach), 38);
    for (index = 0U; index < 500U &&
            data_response_size < sizeof(data_response); ++index) {
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 39);
        CHECK(papacc_pst_secure_processor_read_once(&data_client,
            data_response + data_response_size,
            sizeof(data_response) - data_response_size,
            &transferred, &step) == PAPACC_RESULT_OK, 40);
        data_response_size += transferred;
    }
    if (!(data_response_size == sizeof(data_response) &&
        memcmp(data_response, data_accept, sizeof(data_response)) == 0 &&
        protocol_loop.channel_manager.count == 2U &&
        protocol_loop.association_manager.count == 0U &&
        candidates[1].connection_security.state ==
            PAPACC_SECURITY_CONTEXT_AUTHORIZED &&
        candidates[1].data_gate_installed == PAPACC_TRUE))
        fprintf(stderr, "data_response=%zu channels=%zu associations=%zu "
            "security_state=%d data_gate=%d kind=%d candidate_state=%d\n",
            (size_t)data_response_size,
            (size_t)protocol_loop.channel_manager.count,
            (size_t)protocol_loop.association_manager.count,
            (int)candidates[1].connection_security.state,
            (int)candidates[1].data_gate_installed,
            (int)protocol_slots[1].kind, (int)candidates[1].state);
    CHECK(data_response_size == sizeof(data_response) &&
        memcmp(data_response, data_accept, sizeof(data_response)) == 0 &&
        protocol_loop.channel_manager.count == 2U &&
        protocol_loop.association_manager.count == 0U &&
        candidates[1].connection_security.state ==
            PAPACC_SECURITY_CONTEXT_AUTHORIZED &&
        candidates[1].data_gate_installed == PAPACC_TRUE, 41);

    /* Full secure PACC -> productive TLS_REQUIRED -> PST server fixture. */
    {
        PAPACC_NETWORK_OPEN open_request = PAPACC_NETWORK_OPEN_INITIALIZER;
        PAPACC_U8 open_frame[64] = {
            0x50,0x41,0x43,0x43,1,0,0,16,0,9,0,0,0,0,0,0 };
        PAPACC_SIZE open_payload_size = 0U;
        PST_CONNECTION_CONFIG fixture_configuration;
        pst_u32 accepted = 0U;
        pst_u32 fixture_operation = PST_OPERATION_NEED_READ;
        PST_RESULT fixture_source = PST_RESULT_OK;
        PAPACC_BOOL fixture_established = PAPACC_FALSE;
        u_long nonblocking = 1UL;

        open_request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
        open_request.transport_mode = PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
        open_request.port = egress_fixture.endpoint.port;
        open_request.destination_size = 18U;
        memcpy(open_request.destination, "papacc-test-server", 18U);
        open_request.alpn_count = 1U;
        open_request.alpn[0].size = 8U;
        memcpy(open_request.alpn[0].bytes, "papacc/1", 8U);
        CHECK(papacc_network_open_encode(&open_request, open_frame + 16U,
            sizeof(open_frame) - 16U, &open_payload_size) == PAPACC_RESULT_OK,
            73);
        open_frame[12] = (PAPACC_U8)(open_payload_size >> 24);
        open_frame[13] = (PAPACC_U8)(open_payload_size >> 16);
        open_frame[14] = (PAPACC_U8)(open_payload_size >> 8);
        open_frame[15] = (PAPACC_U8)open_payload_size;
        CHECK(write_all(&data_client, &secure_io, open_frame,
            16U + open_payload_size, 5U), 74);
        outbound_server_config(&fixture_configuration,
            fixture.server.credentials, TEST_OUTBOUND_VALID);
        for (index = 0U; index < 1000U &&
                (fixture_established != PAPACC_TRUE ||
                protocol_loop.network_job_manager.count != 0U); ++index) {
            CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 5U) ==
                PAPACC_RESULT_OK, 75);
            if (fixture_socket == INVALID_SOCKET) {
                fixture_socket = accept(fixture_listener, NULL, NULL);
                if (fixture_socket != INVALID_SOCKET) {
                    CHECK(ioctlsocket(fixture_socket, FIONBIO, &nonblocking) !=
                        SOCKET_ERROR, 76);
                    CHECK(pst_connection_create(composition.runtime,
                        &fixture_configuration, &fixture_connection) ==
                        PST_RESULT_OK, 77);
                    CHECK(pst_win32_socket_transport_create(
                        (pst_size)fixture_socket, &fixture_transport) ==
                        PST_RESULT_OK, 78);
                    CHECK(pst_connection_attach(fixture_connection,
                        fixture_transport, PST_OWNERSHIP_TRANSFERRED,
                        &accepted) == PST_RESULT_OK && accepted != 0U, 79);
                    fixture_transport = NULL;
                    fixture_socket = INVALID_SOCKET;
                }
            }
            if (fixture_connection != NULL && fixture_established !=
                    PAPACC_TRUE) {
                CHECK(pst_connection_handshake(fixture_connection,
                    &fixture_operation, &fixture_source) == PST_RESULT_OK &&
                    fixture_operation != PST_OPERATION_FAILED, 80);
                if (fixture_operation == PST_OPERATION_COMPLETE)
                    fixture_established = PAPACC_TRUE;
            }
        }
        if (!(fixture_established == PAPACC_TRUE &&
            protocol_loop.network_stream_manager.count == 1U &&
            protocol_loop.network_job_manager.count == 0U &&
            egress_fixture.resolve_calls == 1U))
            fprintf(stderr, "tls open fixture=%d op=%lu source=%ld streams=%zu jobs=%zu resolves=%zu\n",
                (int)fixture_established, (unsigned long)fixture_operation,
                (long)fixture_source,
                (size_t)protocol_loop.network_stream_manager.count,
                (size_t)protocol_loop.network_job_manager.count,
                (size_t)egress_fixture.resolve_calls);
        CHECK(fixture_established == PAPACC_TRUE &&
            protocol_loop.network_stream_manager.count == 1U &&
            protocol_loop.network_job_manager.count == 0U &&
            egress_fixture.resolve_calls == 1U, 81);
        {
            PAPACC_U8 open_result[17];
            CHECK(read_exact(&data_client, &secure_io, open_result,
                sizeof(open_result)) && open_result[9] == 10U &&
                open_result[16] == PAPACC_NETWORK_OPEN_OK, 82);
        }
        for (index = 0U; index < 64U; ++index) {
            PAPACC_U8 frame[528] = {
                0x50,0x41,0x43,0x43,1,0,0,16,0,11,0,0,0,0,2,0 };
            PAPACC_U8 returned[512];
            PAPACC_U8 observed[512];
            PAPACC_SIZE observed_size = 0U;
            PAPACC_SIZE written_size = 0U;
            PAPACC_SIZE byte_index;
            PST_IO_RESULT io_result;
            for (byte_index = 0U; byte_index < sizeof(observed); ++byte_index)
                frame[16U + byte_index] = (PAPACC_U8)
                    ((byte_index * 37U + index * 13U) & 0xFFU);
            frame[16] = 0U;
            frame[17] = 0xFFU;
            CHECK(write_all(&data_client, &secure_io, frame,
                sizeof(frame), 37U), 83);
            /* Deliberately let bounded Accelerator buffers absorb pressure. */
            CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
                    PAPACC_RESULT_OK &&
                papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
                    PAPACC_RESULT_OK, 84);
            while (observed_size < sizeof(observed)) {
                memset(&io_result, 0, sizeof(io_result));
                CHECK(pst_connection_read(fixture_connection,
                    observed + observed_size, sizeof(observed) - observed_size,
                    &io_result) == PST_RESULT_OK &&
                    io_result.operation != PST_OPERATION_FAILED, 85);
                observed_size += (PAPACC_SIZE)io_result.bytes_transferred;
                CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
                    PAPACC_RESULT_OK, 86);
            }
            CHECK(memcmp(observed, frame + 16U, sizeof(observed)) == 0, 87);
            while (written_size < sizeof(observed)) {
                memset(&io_result, 0, sizeof(io_result));
                CHECK(pst_connection_write(fixture_connection,
                    observed + written_size, sizeof(observed) - written_size,
                    &io_result) == PST_RESULT_OK &&
                    io_result.operation != PST_OPERATION_FAILED, 88);
                written_size += (PAPACC_SIZE)io_result.bytes_transferred;
                CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
                    PAPACC_RESULT_OK, 89);
            }
            CHECK(read_network_payload(&data_client, &secure_io, returned,
                sizeof(returned)) &&
                memcmp(returned, observed, sizeof(observed)) == 0, 90);
        }
        CHECK(protocol_loop.network_stream_storage[0].to_remote_size <=
                protocol_loop.network_stream_storage[0].buffer_capacity &&
            protocol_loop.network_stream_storage[0].to_client_size <=
                protocol_loop.network_stream_storage[0].buffer_capacity &&
            protocol_loop.network_stream_storage[0].bytes_to_remote == 32768U &&
            protocol_loop.network_stream_storage[0].bytes_to_client == 32768U,
            91);
    }

    /* Productive failure matrix: each case traverses secure PACC and Job. */
    CHECK(productive_tls_scenario(TEST_OUTBOUND_TLS12, port, &composition,
        &secure_io, &client, fixture.client_a.credentials, fixture.trust,
        fixture.server.credentials, wrong_trust, fixture_listener,
        &egress_fixture, &outbound_policy), 95);
    CHECK(productive_tls_scenario(TEST_OUTBOUND_WRONG_TRUST, port,
        &composition, &secure_io, &client, fixture.client_a.credentials,
        fixture.trust, fixture.server.credentials, wrong_trust,
        fixture_listener, &egress_fixture, &outbound_policy), 96);
    CHECK(productive_tls_scenario(TEST_OUTBOUND_WRONG_NAME, port, &composition,
        &secure_io, &client, fixture.client_a.credentials, fixture.trust,
        fixture.server.credentials, wrong_trust, fixture_listener,
        &egress_fixture, &outbound_policy), 97);
    CHECK(productive_tls_scenario(TEST_OUTBOUND_WRONG_ALPN, port, &composition,
        &secure_io, &client, fixture.client_a.credentials, fixture.trust,
        fixture.server.credentials, wrong_trust, fixture_listener,
        &egress_fixture, &outbound_policy), 98);
    CHECK(productive_tls_scenario(TEST_OUTBOUND_STALL, port, &composition,
        &secure_io, &client, fixture.client_a.credentials, fixture.trust,
        fixture.server.credentials, wrong_trust, fixture_listener,
        &egress_fixture, &outbound_policy), 99);
    CHECK(productive_tls_scenario(TEST_OUTBOUND_RESET, port, &composition,
        &secure_io, &client, fixture.client_a.credentials, fixture.trust,
        fixture.server.credentials, wrong_trust, fixture_listener,
        &egress_fixture, &outbound_policy), 100);
    CHECK(productive_tls_scenario(TEST_OUTBOUND_VALID, port, &composition,
        &secure_io, &client, fixture.client_a.credentials, fixture.trust,
        fixture.server.credentials, wrong_trust, fixture_listener,
        &egress_fixture, &outbound_policy), 101);

    /* NETWORK_CLOSE drives reciprocal PST close_notify before teardown. */
    {
        static const PAPACC_U8 network_close[16] = {
            0x50,0x41,0x43,0x43,1,0,0,16,0,13,0,0,0,0,0,0 };
        pst_u32 close_operation = PST_OPERATION_NEED_READ;
        PST_RESULT close_source = PST_RESULT_OK;
        CHECK(write_all(&data_client, &secure_io, network_close,
            sizeof(network_close), 2U), 102);
        for(index=0U;index<2000U&&
                (protocol_loop.network_stream_manager.count!=0U||
                 close_operation!=PST_OPERATION_COMPLETE);++index){
            CHECK(papacc_server_secure_io_win32_poll_once(&secure_io,1U)==
                PAPACC_RESULT_OK,103);
            if(close_operation!=PST_OPERATION_COMPLETE&&
               close_operation!=PST_OPERATION_FAILED)
                CHECK(pst_connection_shutdown(fixture_connection,
                    &close_operation,&close_source)==PST_RESULT_OK,104);
        }
        CHECK(index<2000U&&close_operation==PST_OPERATION_COMPLETE&&
            protocol_loop.network_stream_manager.count==0U,105);
        pst_connection_release(fixture_connection);
        fixture_connection=NULL;
    }

    /* A consumed ticket is rejected without disturbing the CONTROL Session. */
    CHECK(establish_client(port, &composition, &secure_io,
        fixture.client_a.credentials, fixture.trust, &replay_client), 42);
    CHECK(write_all(&replay_client, &secure_io, data_attach,
        sizeof(data_attach), 3U), 43);
    for (index = 0U; index < 100U; ++index)
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
            PAPACC_RESULT_OK, 44);
    CHECK(protocol_loop.session_manager.count == 1U &&
        protocol_loop.channel_manager.count == 1U &&
        protocol_loop.association_manager.count == 0U, 45);
    papacc_pst_secure_processor_release(&replay_client);

    /* Obtain a fresh ticket, reject Principal B, then accept Principal A. */
    CHECK(write_all(&client, &secure_io, data_ticket_request,
        sizeof(data_ticket_request), 2U), 46);
    ticket_frame_size = 0U;
    CHECK(read_exact(&client, &secure_io, ticket_frame,
        sizeof(ticket_frame)), 47);
    memcpy(data_attach + 16U, ticket_frame + 16U, 16U);
    CHECK(protocol_loop.association_manager.count == 1U, 48);

    CHECK(establish_client(port, &composition, &secure_io,
        fixture.client_b.credentials, fixture.trust, &foreign_client), 49);
    CHECK(write_all(&foreign_client, &secure_io, data_attach,
        sizeof(data_attach), 1U), 50);
    for (index = 0U; index < 100U; ++index)
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
            PAPACC_RESULT_OK, 51);
    CHECK(protocol_loop.session_manager.count == 1U &&
        protocol_loop.channel_manager.count == 1U &&
        protocol_loop.association_manager.count == 1U, 52);
    papacc_pst_secure_processor_release(&foreign_client);

    CHECK(establish_client(port, &composition, &secure_io,
        fixture.client_a.credentials, fixture.trust, &matched_client), 53);
    CHECK(write_all(&matched_client, &secure_io, data_attach,
        sizeof(data_attach), 1U), 54);
    data_response_size = 0U;
    CHECK(read_exact(&matched_client, &secure_io, data_response,
        sizeof(data_response)), 55);
    CHECK(memcmp(data_response, data_accept, sizeof(data_response)) == 0 &&
        protocol_loop.session_manager.count == 1U &&
        protocol_loop.channel_manager.count == 2U &&
        protocol_loop.association_manager.count == 0U, 56);

    /* Pre-TLS garbage and plaintext PACC never reach the classifier. */
    {
        static const PAPACC_U8 malformed_tls[9] =
            { 0x16, 0x03, 0x03, 0x00, 0x04, 0xde, 0xad, 0xbe, 0xef };
        CHECK(raw_candidate_rejected(port, &secure_io, malformed_tls,
            sizeof(malformed_tls)), 57);
        CHECK(raw_candidate_rejected(port, &secure_io, control_open,
            sizeof(control_open)), 58);
        CHECK(raw_candidate_rejected(port, &secure_io, data_attach,
            sizeof(data_attach)), 59);
        CHECK(protocol_loop.session_manager.count == 1U &&
            protocol_loop.channel_manager.count == 2U &&
            protocol_loop.association_manager.count == 0U &&
            listener.transport_profile ==
                PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL, 60);
    }

    {
        static const PAPACC_U8 wrong_alpn_bytes[] = "wrong/1";
        static const PST_ALPN_PROTOCOL wrong_alpn = {
            wrong_alpn_bytes, sizeof(wrong_alpn_bytes) - 1U };
        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        policy.forced_resolution = PAPACC_PRINCIPAL_NOT_ENROLLED;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 68);
        policy.forced_resolution = PAPACC_PRINCIPAL_RESOLVED;

        policy.forced_decision = PAPACC_AUTHORIZATION_DENY;
        papacc_test_pst_client_config(&client_config,
            fixture.client_a.credentials, fixture.trust);
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 69);
        policy.forced_decision = PAPACC_AUTHORIZATION_ALLOW;

        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        client_config.alpn.protocols = &wrong_alpn;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 70);

        papacc_test_pst_client_config(&client_config, NULL, fixture.trust);
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 71);

        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        client_config.tls.minimum_version = PST_TLS_VERSION_1_2;
        client_config.tls.maximum_version = PST_TLS_VERSION_1_2;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 73);

        papacc_test_pst_client_config(&client_config,
            fixture.client_c.credentials, fixture.trust);
        client_config.alpn.mode = PST_FEATURE_DISABLED;
        client_config.alpn.protocols = NULL;
        client_config.alpn.protocol_count = 0U;
        CHECK(tls_candidate_rejected(port, &composition, &secure_io,
            &client_config), 74);

        {
            static const PAPACC_U8 invalid_first_frame[16] = {
                0x50,0x41,0x43,0x43,1,0,0,16,0,0x7f,0,0,0,0,0,0 };
            CHECK(establish_client(port, &composition, &secure_io,
                fixture.client_a.credentials, fixture.trust,
                &bad_protocol_client), 75);
            CHECK(write_all(&bad_protocol_client, &secure_io,
                invalid_first_frame, sizeof(invalid_first_frame), 1U), 76);
            for (index = 0U; index < 50U; ++index)
                CHECK(papacc_server_secure_io_win32_poll_once(
                    &secure_io, 1U) == PAPACC_RESULT_OK, 77);
            papacc_pst_secure_processor_release(&bad_protocol_client);
        }
        CHECK(protocol_loop.session_manager.count == 1U &&
            protocol_loop.channel_manager.count == 2U &&
            protocol_loop.connection_manager->count == 2U &&
            listener.transport_profile ==
                PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL, 72);
    }

    CHECK(run_reference_client(port, &secure_io, "tls12", PAPACC_FALSE, 0U), 83);
    CHECK(run_reference_client(port, &secure_io, "wrong-alpn",
        PAPACC_FALSE, 0U), 84);
    CHECK(run_reference_client(port, &secure_io, "missing-credential",
        PAPACC_FALSE, 0U), 85);
    CHECK(run_reference_client(port, &secure_io, "not-enrolled",
        PAPACC_FALSE, 0U), 86);
    CHECK(run_reference_client(port, &secure_io, "deny", PAPACC_FALSE, 0U), 87);
    {
        REFERENCE_FIXTURE_THREAD reference_fixture;
        HANDLE fixture_thread;
        DWORD fixture_exit = 1U;
        memset(&reference_fixture, 0, sizeof(reference_fixture));
        CHECK(reference_fixture_initialization_selftest(), 100);
        puts("CHECK100_SCENARIO_ID=NETWORK_TLS_TRANSFER_REFERENCE_FIXTURE FIXTURE_ZERO_INITIALIZED=YES OPTIONAL_FLAGS_ZERO=YES OPTIONAL_HANDLES_NULL=YES");
        reference_fixture.listener = fixture_listener;
        reference_fixture.runtime = composition.runtime;
        reference_fixture.credentials = fixture.server.credentials;
        reference_fixture.tls = PAPACC_FALSE;
        reference_fixture.expected_size = 32768U;
        reference_fixture.result = 0;
        fixture_thread = CreateThread(NULL, 0U, reference_fixture_thread,
            &reference_fixture, 0U, NULL);
        CHECK(fixture_thread != NULL && run_reference_client(port, &secure_io,
            "network-tcp", PAPACC_FALSE, egress_fixture.endpoint.port), 97);
        for (index = 0U; index < 5000U &&
                WaitForSingleObject(fixture_thread, 0U) == WAIT_TIMEOUT;
                ++index)
            CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
                PAPACC_RESULT_OK, 97);
        CHECK(WaitForSingleObject(fixture_thread, 5000U) == WAIT_OBJECT_0 &&
            GetExitCodeThread(fixture_thread, &fixture_exit) &&
            fixture_exit == 0U && reference_fixture.result, 98);
        CloseHandle(fixture_thread);

        reference_fixture.tls = PAPACC_TRUE;
        reference_fixture.expected_size = 32768U;
        reference_fixture.result = 0;
        fixture_thread = CreateThread(NULL, 0U, reference_fixture_thread,
            &reference_fixture, 0U, NULL);
        CHECK(fixture_thread != NULL && run_reference_client(port, &secure_io,
            "network-tls", PAPACC_FALSE, egress_fixture.endpoint.port), 99);
        for (index = 0U; index < 5000U &&
                WaitForSingleObject(fixture_thread, 0U) == WAIT_TIMEOUT;
                ++index)
            CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
                PAPACC_RESULT_OK, 99);
        if (!GetExitCodeThread(fixture_thread, &fixture_exit) ||
            fixture_exit != 0U || !reference_fixture.result)
            fprintf(stderr, "reference TLS fixture exit=%lu result=%d "
                "rounds=%zu streams=%zu\n", (unsigned long)fixture_exit,
                reference_fixture.result, (size_t)index,
                (size_t)protocol_loop.network_stream_manager.count);
        CHECK(WaitForSingleObject(fixture_thread, 5000U) == WAIT_OBJECT_0 &&
            GetExitCodeThread(fixture_thread, &fixture_exit) &&
            fixture_exit == 0U && reference_fixture.result, 100);
        CloseHandle(fixture_thread);
    }

    papacc_pst_secure_processor_release(&matched_client);
    papacc_pst_secure_processor_release(&data_client);
    papacc_pst_secure_processor_release(&client);
    control_shutdown = data_shutdown = matched_shutdown = PAPACC_TRUE;
    for (index = 0U; index < 200U &&
            protocol_loop.connection_manager->count != 0U; ++index)
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 1U) ==
            PAPACC_RESULT_OK, 81);
    CHECK(protocol_loop.connection_manager->count == 0U, 82);
    CHECK(run_reference_client(port, &secure_io, "main", PAPACC_TRUE, 0U), 80);
    if (stage13_stop_after_check80_requested() == PAPACC_TRUE) {
        result = 0;
        goto cleanup;
    }
    CHECK(papacc_server_secure_io_win32_request_stop(&secure_io) ==
        PAPACC_RESULT_OK, 61);
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_TRUE, 62);
    for (index = 0U; index < 500U &&
            papacc_server_secure_io_win32_is_stopped(&secure_io) !=
                PAPACC_TRUE; ++index) {
        if (control_shutdown != PAPACC_TRUE) {
            CHECK(papacc_pst_secure_processor_shutdown_once(&client, &step) ==
                PAPACC_RESULT_OK, 63);
            if (step == PAPACC_PST_SECURE_STEP_COMPLETE ||
                step == PAPACC_PST_SECURE_STEP_CLOSED)
                control_shutdown = PAPACC_TRUE;
        }
        if (data_shutdown != PAPACC_TRUE) {
            CHECK(papacc_pst_secure_processor_shutdown_once(
                &data_client, &step) == PAPACC_RESULT_OK, 64);
            if (step == PAPACC_PST_SECURE_STEP_COMPLETE ||
                step == PAPACC_PST_SECURE_STEP_CLOSED)
                data_shutdown = PAPACC_TRUE;
        }
        if (matched_shutdown != PAPACC_TRUE) {
            CHECK(papacc_pst_secure_processor_shutdown_once(
                &matched_client, &step) == PAPACC_RESULT_OK, 65);
            if (step == PAPACC_PST_SECURE_STEP_COMPLETE ||
                step == PAPACC_PST_SECURE_STEP_CLOSED)
                matched_shutdown = PAPACC_TRUE;
        }
        CHECK(papacc_server_secure_io_win32_poll_once(&secure_io, 10U) ==
            PAPACC_RESULT_OK, 66);
    }
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_TRUE && protocol_loop.connection_manager->count == 0U &&
        protocol_loop.session_manager.count == 0U &&
        protocol_loop.channel_manager.count == 0U, 67);

cleanup:
    papacc_server_secure_io_win32_shutdown(&secure_io);
    papacc_pst_secure_processor_release(&bad_protocol_client);
    papacc_pst_secure_processor_release(&matched_client);
    papacc_pst_secure_processor_release(&foreign_client);
    papacc_pst_secure_processor_release(&replay_client);
    papacc_pst_secure_processor_release(&data_client);
    papacc_pst_secure_processor_release(&client);
    if (client_transport != NULL) pst_transport_release(client_transport);
    if (client_socket != INVALID_SOCKET) closesocket(client_socket);
    if (fixture_connection != NULL) pst_connection_release(fixture_connection);
    if (fixture_transport != NULL) pst_transport_release(fixture_transport);
    if (fixture_socket != INVALID_SOCKET) closesocket(fixture_socket);
    if (fixture_listener != INVALID_SOCKET) closesocket(fixture_listener);
    if (wrong_trust != NULL) pst_trust_release(wrong_trust);
    papacc_server_io_loop_win32_shutdown(&protocol_loop);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    papacc_tcp_socket_win32_close(&entry.socket);
    papacc_tcp_platform_shutdown(&network.tcp_platform);
    papacc_security_composition_release(&composition);
    papacc_test_security_fixture_release(&fixture);
    WSACleanup();
    return result;
}
