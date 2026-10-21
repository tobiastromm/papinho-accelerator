#ifndef PAPACC_SERVER_C2_OWNER_WIN32_H
#define PAPACC_SERVER_C2_OWNER_WIN32_H
#include "server_secure_io_win32.h"
#include "c2_operation_win32.h"
#include "c3_bridge_win32.h"
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#include "server_configuration_commit_win32.h"
#include "server_security_source_win32.h"
#define PAPACC_C2_CONFIG_EMPTY 0U
#define PAPACC_C2_CONFIG_APPLIED 1U
#define PAPACC_C2_CONFIG_CURRENT 2U
#define PAPACC_C2_CONFIG_RETAINED 3U
#define PAPACC_C2_CONFIG_CLOSED 4U
typedef struct PAPACC_C2_DORMANT_CONFIGURATION {
 PAPACC_CONFIG_VIEW view;
 PAPACC_SECURITY_SOURCE_TRANSACTION *source;
 const PAPACC_SECURITY_COMPOSITION *secure_lease;
 PAPACC_CAPABILITY_KEY enabled[3];PAPACC_SIZE enabled_count,reserved;
 PAPACC_U64 source_ticket,epoch;
 PAPACC_U32 owner,state;
 PAPACC_BOOL io_attached;
} PAPACC_C2_DORMANT_CONFIGURATION;
#endif
typedef enum PAPACC_C2_PUBLICATION_EVENT_KIND {
 PAPACC_C2_PUBLICATION_ATTEMPT=1,PAPACC_C2_PUBLICATION_AUTHORIZED,
 PAPACC_C2_PUBLICATION_LOWER_ACCEPTED,PAPACC_C2_PUBLICATION_REJECTED,
 PAPACC_C2_PUBLICATION_FRAME_COMPLETE,
 PAPACC_C2_E10_DATA_BEFORE,PAPACC_C2_E10_DATA_AFTER,
 PAPACC_C2_E10_OWNER_SNAPSHOT,PAPACC_C2_E10_HISTORY_RETIRING,
 PAPACC_C2_E10_STOP_RETURN,PAPACC_C2_E10_ROLE_RETURN,
 PAPACC_C2_E10_CONTROL_BEFORE,PAPACC_C2_E10_CONTROL_RETURN,PAPACC_C2_E10_RELEASE_RETURN,
 PAPACC_C2_E10_HISTORY_RETIRED,PAPACC_C2_E10_HISTORY_REUSED,
 PAPACC_C2_E10_PST_CALL_OBSERVED,PAPACC_C2_E10_PST_RETURN,
 PAPACC_C2_E10_MODEL_E_DISPOSITION,PAPACC_C2_E10_DESTROYED_CONFIRMED,
 PAPACC_C2_E10_RETAINED_CONFIRMED,
 PAPACC_C2_E10_BIND_ATTEMPT,PAPACC_C2_E10_BIND_RETURN,PAPACC_C2_E10_BIND_ACK_QUEUE,PAPACC_C2_E10_BIND_HEADER_RETURN
} PAPACC_C2_PUBLICATION_EVENT_KIND;
/* Private diagnostic value snapshot. No payload, token, pointer or grant authority.
 * Consumer must copy synchronously into pre-reserved bounded memory, never do
 * file/network I/O, re-enter owner, keep this pointer or mutate owner state. */
typedef struct PAPACC_C2_E10_SLOT_SNAPSHOT {
 PAPACC_U64 session,operation,generation,epoch,channels[2];
 PAPACC_U64 admitted[4],downstream[4],submitted[4],escaped[4];
 PAPACC_SIZE queued[4],callbacks,borrowers,grant_count;
 PAPACC_CAPABILITY_KEY grants[2];PAPACC_BOOL principal_valid;
 PAPACC_U8 principal_public[PAPACC_PRINCIPAL_VALUE_SIZE];
 PAPACC_U32 owner,state,session_state;PAPACC_U16 cause,ownership;
 PAPACC_BOOL grant_written,claimed[2],ack_written[2],site_ready,tls_ready;
 PAPACC_BOOL revoked,final_queued,final_written,cleanup_confirmed;
 PAPACC_BOOL release_attempted,bridge_present,bridge_retained,membership;
 PAPACC_BOOL child_destroyed,ordered_eof;
 PAPACC_U32 release_result,release_disposition;PAPACC_U64 pst_calls,pst_returns;
 PAPACC_BOOL pst_count_overflow,pst_synthetic;
} PAPACC_C2_E10_SLOT_SNAPSHOT;
/* S1 private by-value facts; presence bits, NOT public protocol enums. */
#define PAPACC_S1_ELIGIBILITY 1U
#define PAPACC_S1_ELIGIBLE 2U
#define PAPACC_S1_AUTHORITY 4U
#define PAPACC_S1_CREDIT 8U
#define PAPACC_S1_IO_CALL 16U
#define PAPACC_S1_IO_RESULT 32U
#define PAPACC_S1_IO_STATUS 64U
#define PAPACC_S1_STATE_BEFORE 128U
#define PAPACC_S1_STATE_AFTER 256U
#define PAPACC_S1_CREDIT_GRANTED 512U
#define PAPACC_S1_STEP_RETURN 26U
/* Exactly one fact per completed valid-owner step, not one snapshot per phase. */
typedef struct PAPACC_C2_S1_FACT {
 PAPACC_U32 phases,authority,credits_before,credits_after;
 PAPACC_RESULT io_result;PAPACC_U32 io_status;
 PAPACC_U64 offered,accepted,offset_before,offset_after;
 PAPACC_U32 state_before,state_after;
} PAPACC_C2_S1_FACT;
typedef struct PAPACC_C2_PUBLICATION_EVENT {
 PAPACC_C2_S1_FACT s1;
 PAPACC_U64 progress_prefix;
 PAPACC_U64 bind_channel;PAPACC_U32 bind_stage;PAPACC_BOOL bind_principal_valid;
 PAPACC_U8 bind_principal_public[PAPACC_PRINCIPAL_VALUE_SIZE];
 PAPACC_C2_PUBLICATION_EVENT_KIND kind;PAPACC_U64 connection,operation,generation;
 PAPACC_U16 type;PAPACC_SIZE accepted;PAPACC_RESULT result;
 PAPACC_BOOL e10_snapshot_valid;PAPACC_U64 incoming_operation,incoming_generation;
 PAPACC_U64 session,run,sequence,offset,target_request,target_epoch;PAPACC_U32 process,owner;
 PAPACC_BOOL cancel_authority_prevalidated;PAPACC_SIZE data_length;unsigned incoming_role,incoming_direction,target_kind;PAPACC_C2_E10_SLOT_SNAPSHOT e10_slots[2];
} PAPACC_C2_PUBLICATION_EVENT;
/* Embedded, fixed memory charged by sizeof(owner) before enabling. Never wrap. */
#ifdef PAPACC_ENABLE_C2_DIAGNOSTIC
#define PAPACC_C2_E10_EVENT_CAPACITY 768U
#else
#define PAPACC_C2_E10_EVENT_CAPACITY 128U
#endif
/* Bounded public TLS_READY/FINAL facts; never tokens or DATA payload.
 * Same-thread harness consumption only; not remote receipt/replay authority. */
typedef struct PAPACC_C2_E10_PUBLICATION_FACT {
 PAPACC_U64 sequence,connection,operation,generation;
 PAPACC_SIZE length;PAPACC_U8 bytes[287];PAPACC_BOOL valid;
} PAPACC_C2_E10_PUBLICATION_FACT;
typedef struct PAPACC_C2_E10_INTERVAL {
 PAPACC_U64 first_sequence,last_sequence,count,bytes,prefix_first,prefix_last;
} PAPACC_C2_E10_INTERVAL;
typedef struct PAPACC_C2_E10_COLLECTOR {
 PAPACC_C2_PUBLICATION_EVENT events[PAPACC_C2_E10_EVENT_CAPACITY];
 PAPACC_C2_E10_INTERVAL intervals[PAPACC_C2_E10_EVENT_CAPACITY];
 PAPACC_SIZE used;PAPACC_U64 run,next_sequence,retired_operation[2],retired_generation[2];
 PAPACC_BOOL enabled,sealed,overflow,reentrant,busy,reservation_verified;
 /* Input observations, conservative merges and lost inputs; saturating.
  * Derived HISTORY_REUSED records are counted separately via event kinds. */
 PAPACC_U64 observations,aggregated_inputs,dropped_inputs;
 PAPACC_SIZE reserved_owner_bytes;
 PAPACC_C2_E10_PUBLICATION_FACT tls_ready_fact[2],final_fact[2];
} PAPACC_C2_E10_COLLECTOR;
typedef void (*PAPACC_C2_PUBLICATION_OBSERVER)(void*,const PAPACC_C2_PUBLICATION_EVENT*);
typedef struct PAPACC_C2_OWNER_WRITE {
 PAPACC_U8 bytes[16400];PAPACC_SIZE length,offset;
 PAPACC_U64 connection,operation,generation;PAPACC_U16 type;
 PAPACC_C2_OPERATION *record;PAPACC_C2_SESSION *denial_session;PAPACC_BOOL pending,secret_borrow,transport_attempted,quarantined;
} PAPACC_C2_OWNER_WRITE;
typedef struct PAPACC_SERVER_C2_OWNER_WIN32 {
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 PAPACC_C2_DORMANT_CONFIGURATION configuration;
#endif
 PAPACC_SERVER_IO_LOOP_WIN32 *loop;PAPACC_SERVER_SECURE_IO_WIN32 *secure;
 PAPACC_C2_OPERATION_MANAGER *manager;PAPACC_C3_BRIDGE *bridges[2];
 PAPACC_C2_SESSION sessions[4];
 pst_runtime *site_runtime;PST_CONNECTION_CONFIG site_template;
 PAPACC_C3_BRIDGE *site_storage;PAPACC_CONNECTION_MANAGER *site_manager;
 PAPACC_C2_CAPACITY_GATE site_capacity;void *site_capacity_context;
 PST_ALPN_PROTOCOL site_alpn[2][8];
 PAPACC_U8 plaintext_pending[2][16356];PAPACC_SIZE plaintext_pending_size[2];
 unsigned next_bridge,wake_credits,next_output_role[2];
 PAPACC_BOOL wake_progress_done;
 PAPACC_BOOL site_configured;
 HANDLE c7_stall_event;unsigned c7_stall_flow;PAPACC_U64 c7_stall_operation,c7_stall_generation;
 PAPACC_U64 control_connections[2];
 PAPACC_BOOL child_release_attempted[2],initialized;
 /* Capture only during dispatch. Native work waits for owner quiescence. */
 PAPACC_PST_READY_EVENT child_ready[2],peer_ready[2];
 PAPACC_BOOL child_ready_pending[2],peer_ready_pending[2];
 PAPACC_C2_OWNER_WRITE writes[6];
 PAPACC_C2_E10_COLLECTOR e10;
 PAPACC_C2_PUBLICATION_OBSERVER observer;void *observer_context;
} PAPACC_SERVER_C2_OWNER_WIN32;
PAPACC_RESULT papacc_server_c2_owner_win32_install(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_SERVER_IO_LOOP_WIN32*,PAPACC_SERVER_SECURE_IO_WIN32*,PAPACC_C2_OPERATION_MANAGER*);
PAPACC_RESULT papacc_server_c2_owner_win32_queue(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_C2_OPERATION*,PAPACC_U64,PAPACC_U64,PAPACC_U16,const PAPACC_U8*,PAPACC_SIZE);
PAPACC_RESULT papacc_server_c2_owner_win32_step(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64);
PAPACC_RESULT papacc_server_c2_owner_win32_bind_bridge(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_C2_OPERATION*,PAPACC_C3_BRIDGE*);
PAPACC_RESULT papacc_server_c2_owner_win32_stop(PAPACC_SERVER_C2_OWNER_WIN32*);
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
PAPACC_RESULT papacc_server_c2_owner_win32_retire_sessions(PAPACC_SERVER_C2_OWNER_WIN32 *);
#endif
PAPACC_BOOL papacc_server_c2_owner_win32_has_dependencies(const PAPACC_SERVER_C2_OWNER_WIN32*);
PAPACC_RESULT papacc_server_c2_owner_win32_adopt_session(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64);
PAPACC_RESULT papacc_server_c2_owner_win32_control(void*,PAPACC_U64,PAPACC_U16,const PAPACC_U8*,PAPACC_SIZE,PAPACC_BOOL);
PAPACC_BOOL papacc_server_c2_owner_win32_wants_write(const PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64);
PAPACC_RESULT papacc_server_c2_owner_win32_session_lost(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64,PAPACC_U16);
PAPACC_BOOL papacc_server_c2_owner_win32_has_session(const PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64);
PAPACC_RESULT papacc_server_c2_owner_win32_role(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64,PAPACC_U64,PAPACC_U64,PAPACC_U16,const PAPACC_U8*,PAPACC_SIZE);
PAPACC_RESULT papacc_server_c2_owner_win32_role_header(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64,PAPACC_U64,PAPACC_U16);
PAPACC_RESULT papacc_server_c2_owner_win32_role_lost(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64,PAPACC_U64,PAPACC_U16);
PAPACC_RESULT papacc_server_c2_owner_win32_capability_policy(void*,const PAPACC_CAPABILITY_KEY*,PAPACC_CAPABILITY_POLICY_DECISION*);
PAPACC_RESULT papacc_server_c2_owner_win32_collect_ready(PAPACC_SERVER_C2_OWNER_WIN32*,const PAPACC_PST_READY_EVENT*,PAPACC_BOOL*);
/* Private diagnostic configuration. Dependencies and fixed bridge storage stay
 * borrowed from this durable owner through every retained lifetime. */
PAPACC_RESULT papacc_server_c2_owner_win32_configure_site(PAPACC_SERVER_C2_OWNER_WIN32*,pst_runtime*,const PST_CONNECTION_CONFIG*,PAPACC_CONNECTION_MANAGER*,PAPACC_C3_BRIDGE*,PAPACC_C2_CAPACITY_GATE,void*);
PAPACC_RESULT papacc_server_c2_owner_win32_progress(PAPACC_SERVER_C2_OWNER_WIN32*);
PAPACC_BOOL papacc_server_c2_owner_win32_read_allowed(const PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64);
PAPACC_RESULT papacc_server_c2_owner_win32_begin_wake(PAPACC_SERVER_C2_OWNER_WIN32*);
PAPACC_BOOL papacc_server_c2_owner_win32_take_opportunity(PAPACC_SERVER_C2_OWNER_WIN32*);
/* Private source-only collector interface; no public package API. */
PAPACC_RESULT papacc_server_c2_e10_enable_reserved(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_U64,PAPACC_SIZE);
PAPACC_RESULT papacc_server_c2_e10_seal(PAPACC_SERVER_C2_OWNER_WIN32*);
PAPACC_RESULT papacc_server_c2_e10_copy(const PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_SIZE,PAPACC_C2_PUBLICATION_EVENT*);
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
/* SAME owner, administrative apply only: initialized stays FALSE; no IO. */
PAPACC_RESULT papacc_server_c2_configuration_apply(PAPACC_SERVER_C2_OWNER_WIN32*,
 PAPACC_C2_OPERATION_MANAGER*,const PAPACC_CONFIG_VIEW*,
 const PAPACC_CAPABILITY_EVALUATION_INPUTS*,PAPACC_SECURITY_SOURCE_TRANSACTION*,
 const PAPACC_SECURITY_COMPOSITION*,PAPACC_SIZE);
PAPACC_RESULT papacc_server_c2_configuration_validate(const PAPACC_SERVER_C2_OWNER_WIN32*);
PAPACC_RESULT papacc_server_c2_configuration_associate(PAPACC_SERVER_C2_OWNER_WIN32*,const PAPACC_CONFIG_VIEW*,PAPACC_U64);
PAPACC_RESULT papacc_server_c2_configuration_unapply(PAPACC_SERVER_C2_OWNER_WIN32*);
PAPACC_RESULT papacc_server_c2_configuration_retain(PAPACC_SERVER_C2_OWNER_WIN32*);
PAPACC_RESULT papacc_server_c2_owner_win32_attach_closed(PAPACC_SERVER_C2_OWNER_WIN32*,PAPACC_SERVER_IO_LOOP_WIN32*,PAPACC_SERVER_SECURE_IO_WIN32*);
PAPACC_RESULT papacc_server_c2_owner_win32_detach_closed(PAPACC_SERVER_C2_OWNER_WIN32*);
#endif
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
PAPACC_RESULT papacc_server_c2_configuration_authority_current(const PAPACC_SERVER_C2_OWNER_WIN32 *);
PAPACC_RESULT papacc_server_launcher_c2_adopt(PAPACC_SERVER_C2_OWNER_WIN32 *,PAPACC_U64);
PAPACC_BOOL papacc_server_launcher_c2_session_current(const PAPACC_C2_SESSION *);
PAPACC_RESULT papacc_server_launcher_c2_capability_policy(const PAPACC_POST_CONTROL_PROCESSOR *,const PAPACC_CAPABILITY_KEY *,PAPACC_CAPABILITY_POLICY_DECISION *);
#endif
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
PAPACC_RESULT papacc_server_c2_owner_win32_wire_control(PAPACC_SERVER_C2_OWNER_WIN32 *,PAPACC_POST_CONTROL_PROCESSOR *);
#endif
#endif
