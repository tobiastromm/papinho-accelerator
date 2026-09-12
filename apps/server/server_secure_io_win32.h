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
} PAPACC_SERVER_SECURE_CANDIDATE_WIN32;

#define PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER \
    { NULL, PAPACC_SERVER_SECURE_CANDIDATE_UNUSED, \
      PAPACC_PST_SECURE_PROCESSOR_INITIALIZER, \
      PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER, \
      PAPACC_CONNECTION_SECURITY_CONTEXT_INITIALIZER, \
      PAPACC_SESSION_SECURITY_CONTEXT_INITIALIZER, \
      PAPACC_NETWORK_ENDPOINT_INITIALIZER, PAPACC_NETWORK_ENDPOINT_INITIALIZER, \
      0U, 0U, PAPACC_FALSE, PAPACC_FALSE, PAPACC_FALSE }

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
    PAPACC_U64 establishment_timeout_ns;
    PAPACC_U64 shutdown_deadline_ns;
    PAPACC_BOOL stop_requested;
    PAPACC_BOOL initialized;
} PAPACC_SERVER_SECURE_IO_WIN32;

#define PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER \
    { NULL, NULL, NULL, NULL, NULL, PAPACC_PST_SECURE_SCHEDULER_INITIALIZER, NULL, \
      0U, NULL, NULL, 0U, 0U, 0U, PAPACC_FALSE, PAPACC_FALSE }

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

void papacc_server_secure_io_win32_shutdown(
    PAPACC_SERVER_SECURE_IO_WIN32 *secure_io);

#endif
