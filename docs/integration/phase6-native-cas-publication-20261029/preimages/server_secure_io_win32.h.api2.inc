#ifndef PAPACC_SERVER_SECURE_IO_WIN32_H
#define PAPACC_SERVER_SECURE_IO_WIN32_H

#include "server_io_loop_win32.h"
#include "pst_external_source_win32.h"
#include "pst_peer_evidence.h"
#include "pst_secure_transport_adapter.h"
#include "secure_data_association.h"
#include "server_listener_configuration.h"

typedef enum PAPACC_SERVER_SECURE_CANDIDATE_STATE {
    PAPACC_SERVER_SECURE_CANDIDATE_UNUSED = 0,
    PAPACC_SERVER_SECURE_CANDIDATE_HANDSHAKING,
    PAPACC_SERVER_SECURE_CANDIDATE_AUTHORIZED,
    PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL,
    PAPACC_SERVER_SECURE_CANDIDATE_SHUTTING_DOWN,
    PAPACC_SERVER_SECURE_CANDIDATE_FAILED,
    PAPACC_SERVER_SECURE_CANDIDATE_CLOSED
} PAPACC_SERVER_SECURE_CANDIDATE_STATE;

struct PAPACC_SERVER_SECURE_IO_WIN32;

typedef enum PAPACC_SERVER_SECURE_STOP_STATE {
    PAPACC_SERVER_SECURE_STOP_RUNNING = 0,
    PAPACC_SERVER_SECURE_STOP_REQUESTED,
    PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING,
    PAPACC_SERVER_SECURE_STOP_DRAINING,
    PAPACC_SERVER_SECURE_STOP_STOPPED,
    PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL
} PAPACC_SERVER_SECURE_STOP_STATE;

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
typedef enum PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_MODE {
    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_OFF = 0,
    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_ONCE,
    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_PERSISTENT
} PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_MODE;

typedef struct PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION {
    PAPACC_U64 connection_instance_id;
    PAPACC_U64 operation_id;
    PAPACC_U64 stream_generation;
    PAPACC_U32 owner_thread_id;
    PAPACC_U32 remove_attempt_count;
    PAPACC_U32 injected_failure_count;
    PAPACC_U32 real_remove_call_count;
    PAPACC_U32 real_remove_success_count;
    PAPACC_U32 normal_remove_attempt_count;
    PAPACC_U32 normal_remove_success_count;
    PAPACC_U32 stop_remove_attempt_count;
    PAPACC_U32 target_stop_remove_attempt_count;
    PAPACC_U32 target_injected_failure_count;
    PAPACC_U32 target_real_remove_call_count;
    PAPACC_U32 target_real_remove_success_count;
    PAPACC_U32 other_stop_remove_attempt_count;
    PAPACC_BOOL dispatch_active_at_attempt;
} PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION;
#endif

typedef struct PAPACC_SERVER_OUTBOUND_READY_WIN32 {
    pst_connection *connection;
    pst_wait_token wait_token;
    PAPACC_U64 stream_instance_id;
    PAPACC_BOOL registered;
} PAPACC_SERVER_OUTBOUND_READY_WIN32;

#define PAPACC_SERVER_OUTBOUND_READY_WIN32_INITIALIZER \
    { NULL, 0U, 0U, PAPACC_FALSE }

typedef struct PAPACC_SERVER_SECURE_CANDIDATE_WIN32 {
    struct PAPACC_SERVER_SECURE_IO_WIN32 *owner;
    PAPACC_SERVER_SECURE_CANDIDATE_STATE state;
    PAPACC_PST_SECURE_PROCESSOR secure_processor;
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER transport_adapter;
    PAPACC_CONNECTION_SECURITY_CONTEXT connection_security;
    PAPACC_SESSION_SECURITY_CONTEXT session_security;
    PAPACC_NETWORK_ENDPOINT local_endpoint;
    PAPACC_NETWORK_ENDPOINT remote_endpoint;
    pst_wait_token wait_token;
    PAPACC_U64 connection_instance_id;
    PAPACC_BOOL control_authorized;
    PAPACC_BOOL issuance_gate_installed;
    PAPACC_BOOL data_gate_installed;
    PAPACC_BOOL control_close_handler_installed;
    PAPACC_CONTROL_CLOSE_RESULT control_close_result;
    PAPACC_NETWORK_STREAM_TERMINAL_CAUSE control_stream_cause;
    PAPACC_U32 control_cascade_count;
    PAPACC_BOOL close_requested;
} PAPACC_SERVER_SECURE_CANDIDATE_WIN32;

#define PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER \
    { NULL, PAPACC_SERVER_SECURE_CANDIDATE_UNUSED, \
      PAPACC_PST_SECURE_PROCESSOR_INITIALIZER, \
      PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER, \
      PAPACC_CONNECTION_SECURITY_CONTEXT_INITIALIZER, \
      PAPACC_SESSION_SECURITY_CONTEXT_INITIALIZER, \
      PAPACC_NETWORK_ENDPOINT_INITIALIZER, PAPACC_NETWORK_ENDPOINT_INITIALIZER, \
      0U, 0U, PAPACC_FALSE, PAPACC_FALSE, PAPACC_FALSE, PAPACC_FALSE, \
      PAPACC_CONTROL_CLOSE_RESULT_INITIALIZER, \
      PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE, 0U, PAPACC_FALSE }

typedef struct PAPACC_SERVER_SECURE_IO_WIN32 {
    PAPACC_SERVER_NETWORK *network;
    const PAPACC_SERVER_LISTENER_CONFIGURATION_SET *listener_configuration;
    PAPACC_SERVER_IO_LOOP_WIN32 *protocol_loop;
    PAPACC_SECURITY_COMPOSITION *security_composition;
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *security_configuration;
    PAPACC_PST_SECURE_SCHEDULER scheduler;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidates;
    PAPACC_SIZE candidate_capacity;
    pst_external_source **listener_sources;
    pst_wait_token *listener_tokens;
    PAPACC_SIZE listener_count;
    PAPACC_SERVER_OUTBOUND_READY_WIN32 outbound_ready[
        PAPACC_SERVER_NETWORK_STREAM_CAPACITY];
    PAPACC_U64 establishment_timeout_ns;
    PAPACC_U64 shutdown_deadline_ns;
    PAPACC_U64 stop_retry_not_before_ns;
    PAPACC_U64 stop_first_pending_stream_instance_id;
    PAPACC_U32 stop_remove_attempt_count;
    PAPACC_U32 owner_thread_id;
    PAPACC_RESULT stop_last_remove_result;
    PAPACC_NETWORK_STREAM_TERMINAL_CAUSE stop_cause;
    PAPACC_SERVER_SECURE_STOP_STATE stop_state;
    PAPACC_BOOL dispatch_active;
    PAPACC_BOOL initialized;
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    pst_connection *test_remove_selected_connection;
    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_MODE test_remove_fault_mode;
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION test_remove_observation;
    const char *test_last_non_ok_function;
#endif
} PAPACC_SERVER_SECURE_IO_WIN32;

#define PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER \
    { NULL, NULL, NULL, NULL, NULL, PAPACC_PST_SECURE_SCHEDULER_INITIALIZER, NULL, \
      0U, NULL, NULL, 0U, {{NULL,0U,0U,PAPACC_FALSE}}, 0U, 0U, 0U, 0U, \
      0U, 0U, PAPACC_RESULT_OK, PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE, \
      PAPACC_SERVER_SECURE_STOP_RUNNING, PAPACC_FALSE, PAPACC_FALSE }

PAPACC_RESULT papacc_server_secure_io_win32_init(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io,
    PAPACC_SERVER_NETWORK *network,
    const PAPACC_SERVER_LISTENER_CONFIGURATION_SET *listener_configuration,
    PAPACC_SERVER_IO_LOOP_WIN32 *protocol_loop,
    PAPACC_SECURITY_COMPOSITION *security_composition,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *security_configuration,
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidates,
    PAPACC_SIZE candidate_capacity,
    PAPACC_PST_SCHEDULER_MEMBER *scheduler_members,
    PST_WAIT_EVENT *scheduler_events, PAPACC_SIZE scheduler_capacity,
    pst_external_source **listener_sources, pst_wait_token *listener_tokens,
    PAPACC_SIZE listener_capacity, PAPACC_U64 establishment_timeout_ns);

PAPACC_RESULT papacc_server_secure_io_win32_poll_once(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io, PAPACC_U32 timeout_ms);

PAPACC_RESULT papacc_server_secure_io_win32_request_stop(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io);

PAPACC_BOOL papacc_server_secure_io_win32_is_stopped(
    const PAPACC_SERVER_SECURE_IO_WIN32 *secure_io);

PAPACC_RESULT papacc_server_secure_io_win32_try_shutdown(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io);
#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
PAPACC_RESULT papacc_server_secure_io_win32_test_set_remove_fault(
    PAPACC_SERVER_SECURE_IO_WIN32 *,pst_connection *,
    PAPACC_SERVER_SECURE_TEST_REMOVE_FAULT_MODE,PAPACC_U64,PAPACC_U64,
    PAPACC_U64);
PAPACC_RESULT papacc_server_secure_io_win32_test_get_remove_observation(
    const PAPACC_SERVER_SECURE_IO_WIN32 *,
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION *);
#endif

void papacc_server_secure_io_win32_shutdown(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io);

#endif
