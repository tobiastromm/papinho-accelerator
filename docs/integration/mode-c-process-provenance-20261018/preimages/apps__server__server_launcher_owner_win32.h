#ifndef PAPACC_SERVER_LAUNCHER_OWNER_WIN32_H
#define PAPACC_SERVER_LAUNCHER_OWNER_WIN32_H
/* Application-private. Reuse requires confirmed quiescent normal teardown.
 * A retained invocation permanently consumes this bounded process root. */
#include "server_run_win32.h"
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#include "server_configuration_commit_win32.h"
#include "bind_target.h"
#include "server_c8_resource_admission_win32.h"
#include "server_c8_target_evidence_win32.h"
#include "tcp_accept_win32.h"
#endif
#include "server_egress_source_win32.h"
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
#include "pst_log_adapter.h"
#include "server_secure_io_win32.h"
#include "server_security_source_win32.h"
#include "server_outbound_tls_source_win32.h"
#endif
#define PAPACC_LAUNCHER_MANAGED_LIMIT 2097152U
#define PAPACC_LAUNCHER_ACTIVE 1L
#define PAPACC_LAUNCHER_RETAINED 2L
#define PAPACC_LAUNCHER_FINISHED 3L

#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
/* One bounded candidate owned by the existing root. Preparation states are
 * never CONFIG_COMMITTED, authentication or provider readiness facts. */
typedef enum PAPACC_ROOT_PREPARATION_STATE {
    PAPACC_ROOT_PREPARATION_EMPTY=0, PAPACC_ROOT_PREPARATION_PARTIAL,
    PAPACC_ROOT_PREPARATION_ROLLED_BACK, PAPACC_ROOT_PREPARATION_RETAINED,
    PAPACC_ROOT_PREPARATION_CLOSED
} PAPACC_ROOT_PREPARATION_STATE;
/* Private bounded negative-policy preparation, not an administrative schema.
 * Caller input is candidate intent, NEVER proof of current authority. Positive
 * rules/unsupported predicates are rejected until their real source is bound. */
#define PAPACC_ROOT_POLICY_RULES 4U
#define PAPACC_ROOT_POLICY_KEYS 3U
typedef enum PAPACC_ROOT_POLICY_ADMISSION {
    PAPACC_ROOT_POLICY_OPEN=0, PAPACC_ROOT_POLICY_CLOSED=1
} PAPACC_ROOT_POLICY_ADMISSION;
typedef struct PAPACC_ROOT_POLICY_RULE {
    PAPACC_CAPABILITY_KEY key;
    PAPACC_CAPABILITY_POLICY_DECISION decision;
    PAPACC_U32 unsupported_conditions;
} PAPACC_ROOT_POLICY_RULE;
typedef struct PAPACC_ROOT_POLICY_INTENT {
    PAPACC_SERVER_LISTENER_PROFILE profile;
    PAPACC_U16 listener_port;
    PAPACC_ROOT_POLICY_ADMISSION admission;
    PAPACC_SIZE enabled_count, rule_count;
    PAPACC_CAPABILITY_KEY enabled[PAPACC_ROOT_POLICY_KEYS];
    PAPACC_ROOT_POLICY_RULE rules[PAPACC_ROOT_POLICY_RULES];
} PAPACC_ROOT_POLICY_INTENT;
typedef struct PAPACC_ROOT_POLICY_PREPARATION {
    PAPACC_ROOT_POLICY_INTENT intent;
    PAPACC_SERVER_CONFIG configured_model;
    PAPACC_CAPABILITY_SET snapshot;
    PAPACC_CAPABILITY_KEY snapshot_keys[PAPACC_ROOT_POLICY_KEYS];
    PAPACC_U64 invocation;
    PAPACC_BOOL configured, prepared, from_model;
} PAPACC_ROOT_POLICY_PREPARATION;
typedef struct PAPACC_ROOT_POLICY_REPORT {
    PAPACC_U64 invocation, config_revision, c2_policy_epoch;
    PAPACC_SIZE configured_enabled, prepared_enabled, rules;
    PAPACC_SERVER_LISTENER_PROFILE profile;
    PAPACC_ROOT_POLICY_ADMISSION admission;
    PAPACC_U16 listener_port;
    PAPACC_BOOL configured, prepared, committed, current, retained, model_source;
} PAPACC_ROOT_POLICY_REPORT;
/* Dormant transaction participants. No live listener/Session/grant exists. */
#define PAPACC_ROOT_BIND_TARGETS 8U
typedef struct PAPACC_ROOT_TRANSACTION_PARTICIPANTS {
    PAPACC_SERVER_LISTENER_CONFIGURATION listener;
    PAPACC_BIND_TARGET targets[PAPACC_ROOT_BIND_TARGETS];
    PAPACC_U32 runtime_ids[PAPACC_CONFIG_IDS];
    PAPACC_SIZE target_count;
    PAPACC_U64 invocation;
    PAPACC_BOOL listener_prepared, capabilities_prepared, c2_prepared;
} PAPACC_ROOT_TRANSACTION_PARTICIPANTS;
typedef struct PAPACC_ROOT_CONFIGURATION_PREPARATION {
    PAPACC_CONFIG_VIEW model;
    PAPACC_SECURITY_SOURCE_TRANSACTION source;
    PAPACC_CAPABILITY_REGISTRY registry;
    PAPACC_CAPABILITY_DESCRIPTOR descriptors[PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS];
    PAPACC_CAPABILITY_SET supported, enabled;
    PAPACC_CAPABILITY_KEY supported_keys[PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS];
    PAPACC_CAPABILITY_KEY enabled_keys[PAPACC_SERVER_GLOBAL_CAPABILITY_KEYS];
    /* Same capability framework, rooted references; NULL policy is DENY.
     * Backend readiness never installs an authorization callback or grant. */
    PAPACC_CAPABILITY_EVALUATION_INPUTS policy_inputs;
    PAPACC_ROOT_POLICY_PREPARATION policy;
    PAPACC_ROOT_TRANSACTION_PARTICIPANTS participants;
    PAPACC_BOOL dormant_applied, normative_committed, administrative_published;
    PAPACC_U64 current_policy_epoch;
    PST_PROVIDER_INFO runtime_provider;
    PAPACC_SIZE runtime_credit;
    PAPACC_RESULT runtime_result;
    PAPACC_BOOL runtime_attempted;
    /* Local provider configuration only: never attached to a transport. */
    PST_CONNECTION_CONFIG provider_config;
    pst_connection *provider_connection;
    PST_RELEASE_INFO provider_release;
    PST_DIAGNOSTIC_INFO provider_diagnostic;
    PST_RESULT provider_result, provider_release_result;
    PAPACC_BOOL provider_attempted, provider_release_attempted;
    PAPACC_U64 invocation;
    PAPACC_SIZE combined_reserved, material_credit;
    PAPACC_RESULT preparation_result;
    PAPACC_ROOT_PREPARATION_STATE state;
} PAPACC_ROOT_CONFIGURATION_PREPARATION;
#endif

#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
typedef struct PAPACC_ROOT_CURRENT_PIN {
 PAPACC_CONFIG_PIN pin;PAPACC_CONFIG_VIEW view;PAPACC_U64 epoch,invocation;
} PAPACC_ROOT_CURRENT_PIN;
/* Private stage facts, not a published socket or an admission/grant ticket. */
typedef enum PAPACC_ROOT_LISTENER_STATE {
 PAPACC_ROOT_LISTENER_EMPTY=0, PAPACC_ROOT_LISTENER_PREPARED,
 PAPACC_ROOT_LISTENER_VALIDATED, PAPACC_ROOT_LISTENER_READY,
 PAPACC_ROOT_LISTENER_CLOSED, PAPACC_ROOT_LISTENER_RETAINED
} PAPACC_ROOT_LISTENER_STATE;
typedef struct PAPACC_ROOT_LISTENER_COMPOSITION {
 PAPACC_ROOT_CURRENT_PIN pin;
 PAPACC_U64 invocation;
 PAPACC_SIZE reserved;
 PAPACC_ROOT_LISTENER_STATE state;
} PAPACC_ROOT_LISTENER_COMPOSITION;
typedef struct PAPACC_ROOT_LISTENER_REPORT {
 PAPACC_U8 domain[32];
 PAPACC_U64 invocation,revision,predecessor,epoch;
 PAPACC_SIZE reserved;
 PAPACC_U32 state,profile,admission;
 PAPACC_BOOL ready,published,publication_allowed,retained;
} PAPACC_ROOT_LISTENER_REPORT;
#endif
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
typedef struct PAPACC_C8_ACCEPT_LIFETIME {
 PAPACC_C8_RESOURCE_CONTEXT context;
 PAPACC_C8_RESOURCE_KEY ticket;
 PAPACC_TCP_ACCEPTED_SOCKET_WIN32 socket;
 pst_transport *unaccepted_transport;
 PAPACC_U64 protocol_connection;
 PAPACC_U32 phase, socket_transferred, native_close_confirmed, scheduler_borrow, protocol_borrow;
} PAPACC_C8_ACCEPT_LIFETIME;
#endif

typedef struct PAPACC_SERVER_LAUNCHER_OWNER_WIN32 {
    PAPACC_SERVER_NETWORK network;
    PAPACC_SERVER_NETWORK return_network;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *listener_copy;
    PAPACC_U64 invocation;
    PAPACC_SERVER_CONFIG config;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    PAPACC_CONFIGURATION_COMMIT configuration_commit;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    PAPACC_ROOT_CONFIGURATION_PREPARATION configuration_preparation;
    PAPACC_ROOT_CURRENT_PIN attachment_pin;
    PAPACC_U32 attachment_state;
    PAPACC_ROOT_LISTENER_COMPOSITION listener_composition;
    PAPACC_C8_RESOURCE_ADMISSION resource_admission;
    PAPACC_C8_TARGET_PRODUCER target_producer;
    PAPACC_C8_ACCEPT_LIFETIME accept_lifetimes[PAPACC_C8_RESOURCE_SLOTS];
    PAPACC_U64 accept_generation;
    PAPACC_C8_ACCEPT_LIFETIME activation_lifetime;
#endif
#endif
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *bind_ids;
    PAPACC_LOGGER logger, forward_logger;
    PAPACC_U32 owner_thread;
    PAPACC_SIZE reserved_bytes;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor;
    PAPACC_SERVER_IO_LOOP_WIN32 io_loop;
    PAPACC_CONNECTION *connection_storage;
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context_storage;
    PAPACC_SESSION *session_storage;
    PAPACC_CHANNEL *channel_storage;
    PAPACC_DATA_ASSOCIATION_ENTRY *association_storage;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *processor_storage;
    PAPACC_SERVER_EGRESS_SOURCE_WIN32 egress_source;
#ifdef PAPACC_ENABLE_PST_OUTBOUND_TLS
    PST_RUNTIME_OPTIONS pst_options;
    PST_LOG_CONFIG pst_log_config;
    PST_TRUST_SOURCE pst_trust_source;
    PST_DER_ITEM pst_trust_anchor;
    PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE resolved_outbound_tls;
    PAPACC_PST_LOG_ADAPTER pst_log_adapter;
    pst_runtime *pst_runtime_instance;
    pst_trust *pst_system_trust;
    PAPACC_OUTBOUND_TLS_POLICY outbound_tls_policy;
    PAPACC_SERVER_LISTENER_CONFIGURATION secure_listener;
    PAPACC_SERVER_LISTENER_CONFIGURATION_SET secure_listener_set;
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION resolved_security;
    PAPACC_SECURITY_COMPOSITION security_composition;
    PAPACC_SERVER_SECURE_IO_WIN32 secure_io;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *secure_candidates;
    PAPACC_PST_SCHEDULER_MEMBER *secure_members;
    PST_WAIT_EVENT *secure_events;
    pst_external_source **listener_sources;
    pst_wait_token *listener_tokens;
#endif
} PAPACC_SERVER_LAUNCHER_OWNER_WIN32;
/* Only application-private calls. No PST/network calls in acquire/finish.
 * Acquire moves the network aggregate. Finish returns it only after all users
 * have been removed; on retention the caller stays empty (no WSACleanup). */
PAPACC_RESULT papacc_server_launcher_owner_acquire(
    PAPACC_SERVER_NETWORK *,const PAPACC_SERVER_CONFIG *,const PAPACC_LOGGER *,
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 **);
PAPACC_RESULT papacc_server_launcher_owner_finish(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_SERVER_NETWORK *,PAPACC_BOOL);
PAPACC_U32 papacc_server_launcher_owner_state(void);
/* Existing CLI lacks committed administrative input. Validate then deny before
 * any network effect; this is not a publication/admission ticket. */
PAPACC_RESULT papacc_server_launcher_bootstrap_cli_preflight(const PAPACC_SERVER_CONFIG *);
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
/* Exact durable-root object identity, including dormant/retained/Stop states.
 * Generic I/O is never the C8 ticketed Secure accept transaction. */
PAPACC_BOOL papacc_server_launcher_generic_io_is_c8(
    const PAPACC_SERVER_NETWORK *,const PAPACC_SERVER_ACCEPTOR_WIN32 *,
    const PAPACC_SERVER_IO_LOOP_WIN32 *);
/* Resource observation only; accepts a durable retained owner on its own thread.
 * No lease dereference or operational currency is required after Stop. */
PAPACC_RESULT papacc_server_launcher_target_observation_binding(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_C8_TARGET_BINDING *);
PAPACC_RESULT papacc_server_launcher_target_refresh(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_activate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_network_reserve(PAPACC_SERVER_NETWORK *);
PAPACC_BOOL papacc_server_launcher_owns_listener(const PAPACC_TCP_SOCKET_WIN32 *);
PAPACC_BOOL papacc_server_launcher_owns_secure(const PAPACC_SERVER_SECURE_IO_WIN32 *);
PAPACC_RESULT papacc_server_launcher_accept_prepare(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *,PAPACC_U64);
PAPACC_RESULT papacc_server_launcher_accept_borrow(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *,PAPACC_U32,PAPACC_U64);
PAPACC_RESULT papacc_server_launcher_accept_bind_connection(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *,const PAPACC_CONNECTION *);
PAPACC_RESULT papacc_server_launcher_accept_observe(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *,const PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *,PAPACC_BOOL);
PAPACC_RESULT papacc_server_launcher_accept_retain_transport(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *,pst_transport *);
PAPACC_RESULT papacc_server_launcher_accept_complete(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *);
PAPACC_RESULT papacc_server_launcher_accept_close_socket(PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *,PAPACC_TCP_ACCEPTED_SOCKET_WIN32 *);
PAPACC_RESULT papacc_server_launcher_client_resource_reserve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_C8_RESOURCE_KEY *);
PAPACC_RESULT papacc_server_launcher_resource_stop(PAPACC_SERVER_SECURE_IO_WIN32 *);
PAPACC_BOOL papacc_server_launcher_resource_dependencies(const PAPACC_SERVER_SECURE_IO_WIN32 *);
/* Presence/validation facts, never apply confirmations or client grants. */
#define PAPACC_CFG_MODEL 1U
#define PAPACC_CFG_LISTENER_INTENT 2U
#define PAPACC_CFG_SECURITY_LEASE 4U
#define PAPACC_CFG_PRINCIPAL_RESOLVER 8U
#define PAPACC_CFG_AUTHORIZATION_PROVIDER 16U
#define PAPACC_CFG_REGISTRY 32U
#define PAPACC_CFG_CAPABILITY_SETS 64U
#define PAPACC_CFG_C2_OWNER_PRESENT 128U
#define PAPACC_CFG_CURRENT_COMMIT 256U
#define PAPACC_CFG_POLICY_ASSOCIATION 512U
typedef struct PAPACC_CONFIG_PARTICIPANT_REPORT {
    PAPACC_U32 validated, missing;
    PAPACC_RESULT model_result, listener_result, security_result;
    PAPACC_BOOL publication_eligible;
} PAPACC_CONFIG_PARTICIPANT_REPORT;
/* Owner-thread only; no mutation, callbacks, resolver invocation, file/network
 * I/O or release. Missing policy association cannot be inferred from Session. */
/* Fresh inactive real root only. Selection/model copy can succeed internally;
 * return remains NOT_SUPPORTED until excluded mandatory participants exist.
 * This never replaces root.config, grants capabilities or starts listeners. */
PAPACC_RESULT papacc_server_launcher_configuration_recover(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const char *);
PAPACC_RESULT papacc_server_launcher_configuration_preflight(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_CONFIG_PARTICIPANT_REPORT *);
/* Inactive real root only. Complete private layout, stack/scratch and material
 * credit are reserved before loader effects. No bootstrap, grant, epoch,
 * store commit, listener or operational publication is performed. */
PAPACC_RESULT papacc_server_launcher_configuration_prepare_source(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_SIZE,PAPACC_SIZE);
/* Explicit local/offline preparation, no Connection/wait-set/socket/handshake.
 * Reuses existing composition. No runtime/source lease is exported to children;
 * normal rollback is limited to this never-attached zero-child graph. */
/* Trusted application owner submits by-value CANDIDATE intent; no caller
 * callback/reference becomes authority. Only exact-key DENY rules supported.
 * Preparation never creates a Session snapshot, committed view, epoch or grant. */
PAPACC_RESULT papacc_server_launcher_configuration_policy_configure(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_ROOT_POLICY_INTENT *);
PAPACC_RESULT papacc_server_launcher_configuration_policy_prepare(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
/* Application source is root.config.policy, not a caller policy callback. */
PAPACC_RESULT papacc_server_launcher_configuration_policy_prepare_model(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_configuration_policy_preview(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_ADMIN_POLICY_CONTEXT *,
    const PAPACC_CAPABILITY_KEY *,PAPACC_U32 *,PAPACC_SIZE *);
/* Trusted local catalog input; only resolved targets are copied by value.
 * Synthetic catalog tests do not prove OS interface presence or bind success. */
PAPACC_RESULT papacc_server_launcher_configuration_prepare_listener(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *);
PAPACC_RESULT papacc_server_launcher_configuration_prepare_capabilities(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_configuration_prepare_c2(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
typedef struct PAPACC_ROOT_TRANSACTION_REPORT {
    PAPACC_U64 invocation;
    PAPACC_SIZE targets, supported, enabled, c2_sessions, c2_operations;
    PAPACC_BOOL listener_prepared, capabilities_prepared, c2_prepared;
    PAPACC_BOOL c2_installed, apply_confirmed, committed, current, retained;
} PAPACC_ROOT_TRANSACTION_REPORT;
PAPACC_RESULT papacc_server_launcher_configuration_transaction_report(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_ROOT_TRANSACTION_REPORT *);
/* Rejects before storage effects while mandatory apply facts are absent. */
PAPACC_RESULT papacc_server_launcher_configuration_transaction_commit(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
/* Administrative transaction, no operational attachment. First commit only;
 * subsequent live reload/reconstruction remains a separate qualified gate. */
PAPACC_RESULT papacc_server_launcher_configuration_apply_dormant(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*);
PAPACC_RESULT papacc_server_launcher_configuration_transaction_prepare_store(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*,const char*,PAPACC_U64);
PAPACC_RESULT papacc_server_launcher_configuration_transaction_publish(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*);
PAPACC_RESULT papacc_server_launcher_configuration_transaction_abort(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*);
PAPACC_RESULT papacc_server_launcher_configuration_transaction_close(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*);
struct PAPACC_C2_SESSION;
PAPACC_RESULT papacc_server_launcher_c2_open_policy(void *,const struct PAPACC_C2_SESSION *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_BOOL *);
PAPACC_RESULT papacc_server_launcher_c2_capacity(void *,PAPACC_SIZE,PAPACC_BOOL *);
PAPACC_RESULT papacc_server_launcher_dispatch_preflight(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_BOOL *,PAPACC_BOOL *);
/* Administrative read-only metadata; this never grants operational authority. */
PAPACC_RESULT papacc_server_launcher_site_profile(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,
 const PAPACC_ROOT_CURRENT_PIN *,PAPACC_U32,PAPACC_U64,PAPACC_SITE_TLS_CATALOG *);
struct PAPACC_C2_SESSION;
PAPACC_RESULT papacc_server_launcher_c2_site_template(const struct PAPACC_C2_SESSION *,
 PAPACC_U32,const PST_CONNECTION_CONFIG *,PAPACC_SITE_TLS_CATALOG *);
/* Reader calls use the existing commit lock and full scoped identity. Caller
 * must establish thread/callback quiescence before final owner finish. */
PAPACC_RESULT papacc_server_launcher_configuration_current_pin(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*,PAPACC_ROOT_CURRENT_PIN*);
PAPACC_RESULT papacc_server_launcher_configuration_current_unpin(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*,const PAPACC_ROOT_CURRENT_PIN*);
PAPACC_BOOL papacc_server_launcher_configuration_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*,const PAPACC_ROOT_CURRENT_PIN*);
PAPACC_RESULT papacc_server_launcher_configuration_recover03(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const char *);
/* Offline normal-custody profile only. Select remains query-only until guarded
 * original identity and all same-root dormant participants are reconstructed. */
PAPACC_RESULT papacc_server_launcher_configuration_restart_guard03(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_NORMAL_RESTART_WITNESS *);
PAPACC_RESULT papacc_server_launcher_configuration_restart_publish03(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
/* By-value offline facts only, never authentication/grants/publication authority.
 * Stored in already charged caller scratch; no borrowed pointer survives. */
#define PAPACC_ADMISSION_MODEL 1U
#define PAPACC_ADMISSION_CURRENT 2U
#define PAPACC_ADMISSION_CAPABILITY 4U
#define PAPACC_ADMISSION_EPOCH 8U
#define PAPACC_ADMISSION_LEASE 16U
#define PAPACC_ADMISSION_PROVIDER 32U
#define PAPACC_ADMISSION_LIFECYCLE 64U
#define PAPACC_ADMISSION_RESOURCE 128U
#define PAPACC_ADMISSION_ACCESS 256U
#define PAPACC_ADMISSION_ALL 511U
typedef struct PAPACC_ADMISSION_PREFLIGHT {
 PAPACC_U64 invocation, revision, epoch;
 PAPACC_SIZE reserved;
 PAPACC_U32 checked, missing, profile, admission;
 PAPACC_BOOL offline_ready, publication_allowed, operation_authorized;
} PAPACC_ADMISSION_PREFLIGHT;
PAPACC_BOOL papacc_server_launcher_active_cohort_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_admission_preflight(
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_ROOT_CURRENT_PIN *,
 PAPACC_ADMISSION_PREFLIGHT *);
PAPACC_RESULT papacc_server_launcher_listener_prepare(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_listener_validate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_listener_ready(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_listener_report(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_ROOT_LISTENER_REPORT *);
PAPACC_RESULT papacc_server_launcher_listener_abort(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
/* Future publication gate stays denied; no socket/publication implementation. */
PAPACC_RESULT papacc_server_launcher_listener_publication_preflight(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_attach_closed(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*,PAPACC_SIZE);
PAPACC_RESULT papacc_server_launcher_attachment_wake(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*);
PAPACC_RESULT papacc_server_launcher_detach_closed(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*);
#ifdef PAPACC_C8_TEST_ATTACHMENT_FAULTS
PAPACC_RESULT papacc_server_launcher_attachment_test_fault(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*,PAPACC_U32);
#endif
PAPACC_RESULT papacc_server_launcher_configuration_policy_report(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_ROOT_POLICY_REPORT *);
/* Current-context evaluation always DENY before normative commit/epoch; key
 * and context are not new authentication or attachment authority. */
PAPACC_RESULT papacc_server_launcher_configuration_policy_decide(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_CAPABILITY_KEY *,
    PAPACC_SERVER_LISTENER_PROFILE,PAPACC_U16,PAPACC_ROOT_POLICY_ADMISSION,
    PAPACC_CAPABILITY_POLICY_DECISION *);
PAPACC_RESULT papacc_server_launcher_configuration_prepare_runtime(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
/* Initializes the selected provider using a retained unbound SERVER object.
 * No attach/wait-set/handshake/IO/socket. Rollback uses exactly one Model E
 * release; any non-DESTROYED result retains this entire existing root. */
PAPACC_RESULT papacc_server_launcher_configuration_prepare_provider(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_configuration_preparation_report(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_CONFIG_PARTICIPANT_REPORT *);
PAPACC_RESULT papacc_server_launcher_configuration_preparation_rollback(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_configuration_preparation_retain(
    PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
#endif
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ENABLE_PST_OUTBOUND_TLS)
PAPACC_RESULT papacc_server_launcher_configuration_recover04(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const char *);
PAPACC_RESULT papacc_server_launcher_configuration_restart_guard04(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_NORMAL_RESTART_WITNESS *);
PAPACC_RESULT papacc_server_launcher_configuration_restart_publish04(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
#endif
#if defined(PAPACC_C8_SUPERVISED_DIAGNOSTIC)
#include "server_c8_supervision_win32.h"
PAPACC_RESULT papacc_server_launcher_supervision_prepare(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,const PAPACC_C8_SV_PLAN *);
PAPACC_BOOL papacc_server_launcher_supervision_loop_control_only(const PAPACC_SERVER_IO_LOOP_WIN32 *);
PAPACC_BOOL papacc_server_launcher_supervision_control_only(const PAPACC_SERVER_SECURE_IO_WIN32 *);
PAPACC_RESULT papacc_server_launcher_supervision_finish(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_supervision_status(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,PAPACC_C8_SV_STATE *);
PAPACC_RESULT papacc_server_launcher_supervision_cancel(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_supervision_step(PAPACC_SERVER_SECURE_IO_WIN32 *,PAPACC_U64);
#endif
#endif
