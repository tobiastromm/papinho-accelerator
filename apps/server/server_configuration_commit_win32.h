#ifndef PAPACC_SERVER_CONFIGURATION_COMMIT_WIN32_H
#define PAPACC_SERVER_CONFIGURATION_COMMIT_WIN32_H
/* Private modern-Windows diagnostic component; not a configuration service.
 * Zeroed durable owner storage required. No network, PST or secret storage. */
#include <windows.h>
#include "server_store_hash_win32.h"
#include "server_config.h"
#define PAPACC_CONFIG_RECORD_BYTES 512U
#define PAPACC_CONFIG_STORE_BYTES 8192U
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#include "../../src/issuer/issuer_codec.h"
#include "server_configuration_pending_win32.h"
#define PAPACC_CONFIG_STORAGE_BYTES PAPACC_ISSUER_LINKED_ROOT_BYTES
#define PAPACC_CONFIG_COMMITTED_BYTES PAPACC_CONFIG_TRANSACTION_BYTES
#else
#define PAPACC_CONFIG_STORAGE_BYTES PAPACC_CONFIG_STORE_BYTES
#define PAPACC_CONFIG_COMMITTED_BYTES PAPACC_CONFIG_STORAGE_BYTES
#endif
#define PAPACC_CONFIG_SLOTS 3U
#define PAPACC_CONFIG_IDS 8U
typedef struct PAPACC_CONFIG_VIEW {
    PAPACC_SERVER_CONFIG config;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID ids[PAPACC_CONFIG_IDS];
    unsigned char domain[32];
    PAPACC_U64 revision, predecessor, policy_epoch;
} PAPACC_CONFIG_VIEW;
typedef struct PAPACC_CONFIG_SLOT {
    PAPACC_CONFIG_VIEW view;
    unsigned long readers;
    PAPACC_U64 serial;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    PAPACC_U64 context_epoch; /* A: same-root post-commit administrative context. */
    PAPACC_U64 operational_epoch; /* P: independently bound current C2 policy. */
    PAPACC_U64 issuer_revision; /* I: absent unless issuer_snapshot_valid. */
#endif
    int retained;
} PAPACC_CONFIG_SLOT;
typedef struct PAPACC_CONFIG_PIN {
    const void *issuer;
    unsigned index;
    PAPACC_U64 serial, ticket;
} PAPACC_CONFIG_PIN;
/* Private by-value v3/v4 DATA backing, same slots/tickets; never CURRENT or trust.
 * Full image owns SNT/PDG/issuer/BODY/policy references. CA is public DER only.
 * Selected Windows process reserves all three slots even when dormant. */
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ED11_WINDOWS_3MIB)
#include "../../src/config/config_root8_v5.h"
#define PAPACC_CONFIG_V3_CA_BYTES 8192U
#define PAPACC_CONFIG_V4_CAPTURE_WORKSPACE_MAX 32768U
typedef struct PAPACC_CONFIG_V3_BACKING {
 unsigned char packet[PC_ROOT8_V5_MAX],ca[PAPACC_CONFIG_V3_CA_BYTES];
 unsigned char packet_hash[32],ca_hash[32];
 PAPACC_SIZE packet_bytes,ca_bytes;
 PAPACC_U64 serial; /* exact existing slot generation, not an epoch */
} PAPACC_CONFIG_V3_BACKING;
#endif
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ED11_WINDOWS_3MIB)
/* Same owner, one unresolved UPDATE; real predecessor ticket, no candidate pin. */
#define PAPACC_V3_UPDATE_EMPTY 0U
#define PAPACC_V3_UPDATE_HELD 1U
#define PAPACC_V3_UPDATE_RETAINED 2U
#define PAPACC_V3_ACT_EMPTY 0U
#define PAPACC_V3_ACT_CAPTURED 1U
#define PAPACC_V3_ACT_INVALID 2U
#define PAPACC_V3_ACT_RETAINED 3U
#define PAPACC_V3_STATUS_SOURCE_ABSENT 1U
/* Native ephemeral act, not serialized, a lease or a status authority. */
typedef struct PAPACC_V3_UPDATE_ACT {
 BYTE sid[SECURITY_MAX_SID_SIZE]; TOKEN_STATISTICS statistics;
 BYTE candidate_hash[32],predecessor_hash[32]; HANDLE retained_token,retained_process_token;
 DWORD sid_bytes,native_error; unsigned state,status;
} PAPACC_V3_UPDATE_ACT;
typedef struct PAPACC_V3_UPDATE_COHORT {
 PAPACC_CONFIG_PIN predecessor_pin;
 PAPACC_U64 candidate_serial,source_ticket,invocation;
 PAPACC_SIZE reserved;
 unsigned phase,candidate_index,prior_pending,data_active;
 PAPACC_V3_UPDATE_ACT act;
} PAPACC_V3_UPDATE_COHORT;
#endif
typedef struct PAPACC_CONFIGURATION_COMMIT {
    CRITICAL_SECTION lock;
    PAPACC_STORE_HASH store_hash;
    PAPACC_CONFIG_SLOT slots[PAPACC_CONFIG_SLOTS];
    PAPACC_CONFIG_PIN tickets[32];
    PAPACC_U64 next_ticket;
    unsigned char committed[PAPACC_CONFIG_COMMITTED_BYTES];
    char path[MAX_PATH];
    unsigned char domain[32];
    HANDLE retained_file, retained_transaction;
    DWORD writer;
    unsigned current, pending;
    PAPACC_U64 next_serial;
    int initialized, prepared, committed_pending, published, stopped, unresolved;
    int store02, recovery_selected;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    PAPACC_CONFIG_PENDING pending_commit;
    DWORD committed_bytes; /* actual same-file size, not allocated capacity */
    int authority_lost; /* CAS conflict/unknown is not old CURRENT. */
    int recovery_guard; /* Exclusive native read lease, not Model E destruction. */
    unsigned char issuer_staged[PAPACC_ISSUER_IMAGE_BYTES];
    void *issuer_hash_alg,*issuer_hash;
    LONG issuer_hash_error;
    int issuer_stage_prepared;
    /* Immutable issuer bytes belong to the existing bounded config slots.
     * A pin protects the matching slot serial, never the mutable latest image. */
    unsigned char issuer_snapshots[PAPACC_CONFIG_SLOTS][PAPACC_ISSUER_IMAGE_BYTES];
    int issuer_snapshot_valid[PAPACC_CONFIG_SLOTS];
#if defined(PAPACC_ED11_WINDOWS_3MIB)
    PAPACC_CONFIG_V3_BACKING v3_backings[PAPACC_CONFIG_SLOTS];
    PAPACC_V3_UPDATE_COHORT v3_update;
#endif
#endif
} PAPACC_CONFIGURATION_COMMIT;
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_ED11_WINDOWS_3MIB)
#define PAPACC_V3_UPDATE_BUSY(o) ((o)->v3_update.phase!=PAPACC_V3_UPDATE_EMPTY)
#else
#define PAPACC_V3_UPDATE_BUSY(o) 0
#endif
/* init only creates an in-memory diagnostic domain; CREATE_NEW at first commit
 * prevents replacing any existing installation. No recovery authorization. */
PAPACC_RESULT papacc_configuration_commit_init(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare(PAPACC_CONFIGURATION_COMMIT *,
    const PAPACC_SERVER_CONFIG *,PAPACC_U64,PAPACC_U64);
PAPACC_RESULT papacc_configuration_commit(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_publish(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_abort(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_pin(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CONFIG_PIN *,PAPACC_CONFIG_VIEW *);
PAPACC_RESULT papacc_configuration_unpin(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,int);
PAPACC_BOOL papacc_configuration_current(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_VIEW *);
PAPACC_RESULT papacc_configuration_stop(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_finish(PAPACC_CONFIGURATION_COMMIT *);
/* Integrity inspection cannot authorize recovery. Output never contains a
 * usable view. Even a valid record returns NOT_SUPPORTED without continuity. */
PAPACC_RESULT papacc_configuration_recovery_inspect(PAPACC_CONFIGURATION_COMMIT *,const char *);
/* Opt-in protocol0.2 full server-model profile; no operational activation. */
PAPACC_RESULT papacc_configuration_commit_init02(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_recovery_init02(PAPACC_CONFIGURATION_COMMIT *,const char *);
/* Query-only candidate, NOT a published/current configuration or new grant. */
PAPACC_RESULT papacc_configuration_recovered_copy(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CONFIG_VIEW *);
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
/* Root must validate full CURRENT/projection before beginning publication checkpoint.
 * Same pending worker/CAS; this records historical facts, never recovery authority. */
PAPACC_RESULT papacc_configuration_checkpoint_begin07(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_checkpoint_poll07(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);
PAPACC_RESULT papacc_configuration_checkpoint_data07(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_LINK *);
PAPACC_RESULT papacc_configuration_issuer_begin06(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_issuer_poll06(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);
/* BODY0.3 is a COMPONENT store, not the application's normative commit.
 * No caller epoch accepted; zero records unavailable epoch, never a grant. */
PAPACC_RESULT papacc_configuration_commit_init03(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_recovery_init03(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare03(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SERVER_CONFIG *,PAPACC_U64);
/* Private isolated preserved-local-custody evidence. Integrity/file ID alone
 * do not establish freshness outside ADR0046's explicitly supervised scope. */
typedef struct PAPACC_NORMAL_RESTART_WITNESS {
 unsigned char domain[32], image_sha256[32];
 PAPACC_U64 revision, predecessor, volume, file_index;
} PAPACC_NORMAL_RESTART_WITNESS;
PAPACC_RESULT papacc_configuration_recovery_guard03(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_NORMAL_RESTART_WITNESS *);
PAPACC_RESULT papacc_configuration_recovery_validate03(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_recovery_publish03(PAPACC_CONFIGURATION_COMMIT *);
/* Private ROOT06 component, same file and owner. No normative/live CURRENT.
 * Caller must establish application dormancy; readers/retained slots deny.
 * Epoch and admission proofs are never inferred from stored issuer bytes. */
/* Storage math only; no issuer/status/authorization result. */
int papacc_configuration_storage_hash(void *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 [32]);
int papacc_configuration_issuer_hash06(void *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8[32]);
PAPACC_RESULT papacc_configuration_issuer_prepare06(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_U8 *,PAPACC_U64,PAPACC_U64);
PAPACC_RESULT papacc_configuration_issuer_copy06(PAPACC_CONFIGURATION_COMMIT *,PAPACC_U8 *);
/* Data-only copy using an existing live pin; no CURRENT/epoch proof returned. */
PAPACC_RESULT papacc_configuration_issuer_snapshot06(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,PAPACC_U8 *);
PAPACC_RESULT papacc_configuration_issuer_recovery06(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_pin_read(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,PAPACC_CONFIG_VIEW *,PAPACC_U64 *,PAPACC_U64 *);
/* W/L: private value-only preparation binding to the EXISTING root/pin.
 * Not a serialized ledger, native-file identity proof, grant or CURRENT token.
 * No owner growth, handles, acquisition, commit, NV command or new authority.
 * Output remains unchanged on failure and must be disjoint from root/pin. */
typedef struct PAPACC_Q4_ROOT_BINDING_DATA {
 unsigned version,scope_data_only;
 unsigned char domain[32];
 PAPACC_U64 revision,predecessor,administrative_epoch,policy_epoch,ticket,serial;
} PAPACC_Q4_ROOT_BINDING_DATA;
PAPACC_RESULT papacc_configuration_q4_binding_data(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,PAPACC_Q4_ROOT_BINDING_DATA *);
/* Revalidate exact DATA snapshot at point of use; no store/NV/admission effect. */
PAPACC_RESULT papacc_configuration_q4_binding_current(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,const PAPACC_Q4_ROOT_BINDING_DATA *);

/* TC0-TC9 DATA review only: caller facts cannot create authenticated authority.
 * Existing root/pin is revalidated. authority_denied ALWAYS1 and SOURCE_MISSING
 * remains set until an actual authenticated producer exists. No secret/handle/
 * serialized format/owner-storage growth or operational effect. */
#define PAPACC_Q4_TC_FACTS 12U
#define PAPACC_Q4_TC_VERIFIED 1U
#define PAPACC_Q4_TC_SOURCE_MISSING 0x80000000UL
/* facts order: anchor,chain,revocation,custodian,ownerAuth,backing,generation,
 * original index,current counter,ledger persistence,exclusive invocation,resources */
typedef struct PAPACC_Q4_TC_REVIEW {
 unsigned version,profile;
 PAPACC_Q4_ROOT_BINDING_DATA root;
 unsigned char ek_spki[32],certificate[32],anchor[32];
 unsigned char approved_generation[32],current_generation[32];
 unsigned char approved_custody[32],current_custody[32];
 unsigned char original_predecessor[32],current_predecessor[32];
 unsigned facts[PAPACC_Q4_TC_FACTS];
 PAPACC_U64 baseline,expected,observed;
 unsigned index,attributes,data_bytes,written;
} PAPACC_Q4_TC_REVIEW;
typedef struct PAPACC_Q4_TC_POLICY_DATA {
 unsigned version,scope_data_only,authority_denied;
 unsigned long missing;
} PAPACC_Q4_TC_POLICY_DATA;
PAPACC_RESULT papacc_configuration_q4_tc_policy_data(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,const PAPACC_Q4_TC_REVIEW *,PAPACC_Q4_TC_POLICY_DATA *);

/* Accepted custody/source designation DATA, not permission to fetch a value. */
typedef struct PAPACC_Q4_ST20_DESIGNATION_DATA {
 unsigned version,scope_data_only,custodian_designated,source_designated;
 unsigned source_type,index,credential_access_denied;
 char custodian_sid[80];
 unsigned char custodian_decision[32],source_decision[32],ek_spki[32],certificate[32];
 PAPACC_Q4_ROOT_BINDING_DATA root;
} PAPACC_Q4_ST20_DESIGNATION_DATA;
PAPACC_RESULT papacc_configuration_q4_st20_designation_data(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,PAPACC_Q4_ST20_DESIGNATION_DATA *);

/* Offline DATA inspection of a full historical carrier, not file adoption.
 * Caller owns immutable16384 input and disjoint workspace/output. No CURRENT,
 * epoch/proof bits, source lease, key binding or recovery authority returned.
 * Embedded file identity is checked for internal consistency only. */
PAPACC_RESULT papacc_configuration_storage_carrier_inspect(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_CONFIG_VIEW *,PAPACC_ISSUER_VIEW *);
PAPACC_RESULT papacc_configuration_issuer_carrier_inspect06(
 PAPACC_CONFIGURATION_COMMIT *,const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_CONFIG_VIEW *,PAPACC_ISSUER_VIEW *);
int papacc_configuration_issuer_projection06(const PAPACC_SERVER_CONFIG *,const PAPACC_SERVER_CONFIG *);
PAPACC_RESULT papacc_configuration_issuer_publish06(PAPACC_CONFIGURATION_COMMIT *);
/* Explicit BODY0.5 catalog profile; no old-format promotion. */
PAPACC_RESULT papacc_configuration_commit_init05(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare05(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SERVER_CONFIG *,PAPACC_U64);
/* Explicit typed0.4 profile;0.2/0.3 entry points remain strict. */
PAPACC_RESULT papacc_configuration_commit_init04(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_recovery_init04(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare04(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SERVER_CONFIG *,PAPACC_U64);
PAPACC_RESULT papacc_configuration_recovery_guard04(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_NORMAL_RESTART_WITNESS *);
PAPACC_RESULT papacc_configuration_recovery_validate04(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_recovery_publish04(PAPACC_CONFIGURATION_COMMIT *);
#endif
#ifdef PAPACC_OFFLINE_D3_WORKER
#include "personal_custody_data_win32.h"
/* DATA match never grants authority. Bind requires live native ceremony,
 * child/source custody and CA validation, then the existing single borrow.
 * CA owner must outlive this root borrow. No create/open/close/delete occurs.
 * Release is allowed after Stop only with factual worker quiescence. */
PAPACC_RESULT papacc_configuration_personal_ca_match_data(const PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CA_KEY *,const unsigned char[32],const unsigned char[16],const unsigned char[16]);
/* Explicit native entry, never startup. Consumed exact NEW_INSTALL plus
 * CA_INITIALIZE plan and original native references required. Existing native
 * admission stays closed; tests substitute the key API only in test code.
 * No cert signature, commit/publication, private export or retry. */
SECURITY_STATUS papacc_configuration_personal_ca_initialize(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CA_KEY *,const unsigned char[32],const unsigned char[16],const unsigned char[16],const unsigned char *,DWORD,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_personal_ca_bind(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CA_KEY *,const unsigned char[32],const unsigned char[16],const unsigned char[16]);
PAPACC_RESULT papacc_configuration_personal_ca_current(PAPACC_CONFIGURATION_COMMIT *,const unsigned char[32],const unsigned char[16],const unsigned char[16]);
PAPACC_RESULT papacc_configuration_personal_ca_release(PAPACC_CONFIGURATION_COMMIT *);
struct PAPACC_ISSUER_CNG_BACKEND;
/* Typed handoff to the existing issuer backend; source revalidated first.
 * Does not produce NORMAL proof flags, issue or publish anything. */
PAPACC_RESULT papacc_configuration_personal_ca_handoff(PAPACC_CONFIGURATION_COMMIT *,struct PAPACC_ISSUER_CNG_BACKEND *,const unsigned char[32],const unsigned char[16],const unsigned char[16]);
/* Activate ONLY the original source already transferred by handoff,
 * after the same root has become factually CURRENT. Never keep a consumed
 * ceremony packet open to fabricate issuer readiness. Registered backend only.
 * No key/cert/root creation, proof manufacture, signature or publication.
 * Prechecks keep the existing borrow untouched. Failure during/after init
 * retains the recipient and blocks another activation. */
PAPACC_ISSUER_RESULT papacc_configuration_personal_ca_activate_normal(PAPACC_CONFIGURATION_COMMIT *,struct PAPACC_ISSUER_CNG_BACKEND *,const unsigned char[32],const unsigned char[16],const unsigned char[16]);
/* EXISTING_CURRENT: already opened key, no NEW_INSTALL attempt restored.
 * Actual original CURRENT/history/proofs remain mandatory. No open/create here. */
PAPACC_ISSUER_RESULT papacc_configuration_existing_ca_bind(PAPACC_CONFIGURATION_COMMIT *,struct PAPACC_ISSUER_CNG_BACKEND *);
PAPACC_ISSUER_RESULT papacc_configuration_existing_ca_activate_normal(PAPACC_CONFIGURATION_COMMIT *,struct PAPACC_ISSUER_CNG_BACKEND *);
PAPACC_PERSONAL_CUSTODY_DATA papacc_configuration_personal_custody_data(const PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CA_KEY *);
/* One-shot, read-only handoff to the existing worker; no child/key effects.
 * Same owner keeps all inputs frozen until poll confirms signal AND close. */
PAPACC_RESULT papacc_configuration_personal_handoff_begin(PAPACC_CONFIGURATION_COMMIT *,const unsigned char[32],PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_personal_handoff_poll(PAPACC_CONFIGURATION_COMMIT *);
/* Separate effect entry: read-only consumed attempts never qualify. */
PAPACC_RESULT papacc_configuration_personal_target_begin(PAPACC_CONFIGURATION_COMMIT *,const unsigned char[32],PAPACC_SIZE);
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
/* Compile-isolated LAB mechanics, not native administrative authorization. */
PAPACC_RESULT papacc_configuration_personal_target_component_begin(PAPACC_CONFIGURATION_COMMIT *,const unsigned char[32],PAPACC_SIZE);
#endif
/* Native read-only preflight only; no root identity, child, key or CURRENT. */
PAPACC_RESULT papacc_configuration_personal_native_open(PAPACC_CONFIGURATION_COMMIT *,const char *);
/* FCA original preparation; necessary native facts, never publication. */
PAPACC_RESULT papacc_configuration_personal_first_open(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CA_KEY *,const char *,const char *);
PAPACC_RESULT papacc_configuration_personal_parent_open(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CA_KEY *,const char *,const char *);
/* FCR: current protected originals only; selected IDs never grant authority. */
PAPACC_RESULT papacc_configuration_personal_prepared_open(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CA_KEY *,const char *,const char *,const DWORD[9],const BYTE[16],const BYTE[16]);
PAPACC_RESULT papacc_configuration_personal_prepared_plan(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_prepared_validate(PAPACC_CONFIGURATION_COMMIT *,BYTE[32]);
PAPACC_RESULT papacc_configuration_personal_preparation_plan(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_first_confirm(PAPACC_CONFIGURATION_COMMIT *,const BYTE[32]);
/* Read-only identity DATA in same owner; no SCM/CA authority is acquired. */
PAPACC_RESULT papacc_configuration_personal_runtime_bind(PAPACC_CONFIGURATION_COMMIT *,HANDLE,PSID,unsigned,DWORD);
PAPACC_RESULT papacc_configuration_personal_runtime_current(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_observe_runtime_data(PAPACC_CONFIGURATION_COMMIT *,PAPACC_SCQ_RUNTIME_OBSERVATION *);
PAPACC_RESULT papacc_configuration_personal_runtime_cng_probe(PAPACC_CONFIGURATION_COMMIT *,const WCHAR *);
PAPACC_RESULT papacc_configuration_personal_layout_new(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_layout_binary(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_layout_binary_current(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_layout_existing(PAPACC_CONFIGURATION_COMMIT *,const WCHAR *,const WCHAR *,const WCHAR *);
PAPACC_RESULT papacc_configuration_personal_layout_current(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_prepare_data(PAPACC_CONFIGURATION_COMMIT *,const BYTE[32],const WCHAR *);
PAPACC_RESULT papacc_configuration_personal_scq_dispatch(PAPACC_CONFIGURATION_COMMIT *,const BYTE[32]);
/* No caller-made source/grant accepted. Native original acquisition required. */
PAPACC_RESULT papacc_configuration_personal_scq_receive_original(PAPACC_CONFIGURATION_COMMIT *);
/* Metadata/reference custody only; OK is NOT approval or diagnostic authority. */
PAPACC_RESULT papacc_configuration_personal_scq_observe_image_data(PAPACC_CONFIGURATION_COMMIT *,PAPACC_SCQ_RUNTIME_OBSERVATION *);
PAPACC_RESULT papacc_configuration_personal_scq_session_acquire_originals(PAPACC_CONFIGURATION_COMMIT *,const HANDLE [3]);
PAPACC_RESULT papacc_configuration_personal_scq_session_prepare(PAPACC_CONFIGURATION_COMMIT *,DWORD);
DWORD papacc_configuration_personal_scq_first_controller(PAPACC_CONFIGURATION_COMMIT *);
DWORD papacc_configuration_personal_machine_fixture_execute(PAPACC_CONFIGURATION_COMMIT *,FILE *);
DWORD papacc_configuration_personal_machine_fixture_source(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_MACHINE_FIXTURE_INPUT *);
DWORD papacc_configuration_personal_scq_cold_machine_read_controller(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SCQ_READ_COLD_INPUT *);
DWORD papacc_configuration_personal_scq_cold_controller(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SCQ_COLD_START_INPUT *);
/* Temporary read-only boundary observation: no originals, RNG, SCM or SCQ. */
DWORD papacc_configuration_personal_cold_handle_observe(PAPACC_CONFIGURATION_COMMIT *,DWORD [15]);
DWORD papacc_configuration_personal_request_interval_observe(PAPACC_CONFIGURATION_COMMIT *,PAPACC_SCQ_INTERVAL_OBSERVATION *);
PAPACC_RESULT papacc_configuration_personal_scq_start_plan_capture(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SCQ_START_LIMITS *);
DWORD papacc_configuration_personal_scq_start_resource_current(PAPACC_CONFIGURATION_COMMIT *,int);
PAPACC_RESULT papacc_configuration_personal_scq_endpoint_confirm_console(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_session_take_data(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_session_offer_data(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_session_receive_data(PAPACC_CONFIGURATION_COMMIT *,const BYTE *,DWORD);
PAPACC_RESULT papacc_configuration_personal_scq_session_receipt_data(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_session_ack_data(PAPACC_CONFIGURATION_COMMIT *,const BYTE *,DWORD);
const BYTE *papacc_configuration_personal_scq_session_output_data(PAPACC_CONFIGURATION_COMMIT *,DWORD *);
PAPACC_RESULT papacc_configuration_personal_scq_recipient_prepare(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SCQ_PLAN_DATA *,const HANDLE [3],HANDLE);
PAPACC_RESULT papacc_configuration_personal_scq_recipient_acquire(PAPACC_CONFIGURATION_COMMIT *,HANDLE);
DWORD papacc_configuration_personal_scq_transport_enter(PAPACC_CONFIGURATION_COMMIT *,DWORD,const WCHAR *);
DWORD papacc_configuration_personal_scq_transport_references(PAPACC_CONFIGURATION_COMMIT *,HANDLE [3]);
DWORD papacc_configuration_personal_scq_transport_data(PAPACC_CONFIGURATION_COMMIT *,DWORD,int);
DWORD papacc_configuration_personal_scq_transport_produce_close(PAPACC_CONFIGURATION_COMMIT *);
DWORD papacc_configuration_personal_scq_ingress_continue(PAPACC_CONFIGURATION_COMMIT *);
int papacc_configuration_personal_scq_initial_wait_current(PAPACC_CONFIGURATION_COMMIT *);
DWORD papacc_configuration_personal_scq_start_progress(PAPACC_CONFIGURATION_COMMIT *,HANDLE);
PAPACC_RESULT papacc_configuration_personal_scq_start_namespace_current(PAPACC_CONFIGURATION_COMMIT *);
DWORD papacc_configuration_personal_scq_transport_poll(PAPACC_CONFIGURATION_COMMIT *);
/* One bounded continuation step; 0 is local protocol close only, not readiness. */
DWORD papacc_configuration_personal_scq_transport_continue(PAPACC_CONFIGURATION_COMMIT *);
DWORD papacc_configuration_personal_scq_transport_stop(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_session_peer_lost(PAPACC_CONFIGURATION_COMMIT *);
/* Query existing original controller source before any runtime binding.
 * Not ServiceMain admission, readiness, delivery or lifecycle authority. */
PAPACC_RESULT papacc_configuration_personal_scq_startup_source_current(PAPACC_CONFIGURATION_COMMIT *);
/* Evidence query only; does not report SCM RUNNING or enable production. */
PAPACC_RESULT papacc_configuration_personal_scq_controller_inconclusive(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_closed_inconclusive_current(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_closed_ready_current(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_closed_arm_running(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_admin_prepare(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_admin_confirm_console(PAPACC_CONFIGURATION_COMMIT *);
/* Independent attributed Start event and single consumption; no SCM effect. */
PAPACC_RESULT papacc_configuration_personal_scq_start_confirm_console(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_start_take(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_start_carrier(PAPACC_CONFIGURATION_COMMIT *,PS_SCQ_CARRIER *);
PAPACC_RESULT papacc_configuration_personal_scq_start_take_image(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SERVICE_IMAGE_VIEW *);
PAPACC_RESULT papacc_configuration_personal_scq_carrier_capture(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_admin_take(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_acquire_contents(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_scq_capture_inputs(PAPACC_CONFIGURATION_COMMIT *,const HANDLE[3]);
PAPACC_RESULT papacc_configuration_personal_scq_inputs_current(PAPACC_CONFIGURATION_COMMIT *);
/* Closed typed DATA projection only. Native source/endpoint gates stay DENY. */
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_machine_read_data(PAPACC_CONFIGURATION_COMMIT *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_transport_data(PAPACC_CONFIGURATION_COMMIT *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_closed_data(PAPACC_CONFIGURATION_COMMIT *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
PAPACC_RESULT papacc_configuration_personal_scq_profile_prepare_data(PAPACC_CONFIGURATION_COMMIT *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
PAPACC_RESULT papacc_configuration_personal_runtime_close(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_native_current(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_source_current(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_native_close(PAPACC_CONFIGURATION_COMMIT *);
/* Read-only SCM preparation uses SAME parent/token/source owner, no NEW_INSTALL grant. */
PAPACC_RESULT papacc_configuration_scm_lab_files_open(PAPACC_CONFIGURATION_COMMIT *,const wchar_t *,const char *);
PAPACC_RESULT papacc_configuration_scm_registration_image_open(PAPACC_CONFIGURATION_COMMIT *,const wchar_t *);
PAPACC_RESULT papacc_configuration_scm_registration_dependency_open(PAPACC_CONFIGURATION_COMMIT *,unsigned);
PAPACC_RESULT papacc_configuration_scm_lab_files_current(PAPACC_CONFIGURATION_COMMIT *,const BY_HANDLE_FILE_INFORMATION *);
/* Independent immutable-plan DATA. No operator, CSPRNG, target or authority
 * is inferred. Owned by this root; no returned buffer borrow. */
PAPACC_RESULT papacc_configuration_personal_data_capture(PAPACC_CONFIGURATION_COMMIT *,const unsigned char[32],const unsigned char *,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_personal_data_retire(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_personal_effect_plan_data(PAPACC_CONFIGURATION_COMMIT *,const unsigned char[16],const unsigned char[16],const unsigned char *,PAPACC_SIZE);
/* Initial namespace phase only. Same worker/packet, no commit or CURRENT.
 * parent is an existing governed local directory, leaf a new single component.
 * explicit caller is administrative; native enabled-admin token required.
 * Retained leases block all other pending operations until stopped/closed. */
PAPACC_RESULT papacc_configuration_bootstrap_custody_begin(PAPACC_CONFIGURATION_COMMIT *,const char *,const char *,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_bootstrap_custody_poll(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);
PAPACC_BOOL papacc_configuration_bootstrap_custody_ready(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_bootstrap_custody_close(PAPACC_CONFIGURATION_COMMIT *);
/* Same worker, relative initial FILE_CREATE under retained custody. Durable
 * success is COMMITTED_PENDING only, never CURRENT. Scope/identity native proof
 * and token state are revalidated by worker before commit. */
PAPACC_BOOL papacc_configuration_bootstrap_authority_valid(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_bootstrap_commit_begin(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_U8 *,PAPACC_SIZE,const char *,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_bootstrap_commit_poll(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);
/* Offline update only. No bootstrap/recovery/publication; actual current pin
 * and native identity handle required. Last argument is factual already-reserved
 * composition including root, caller and codec pair; extra scratch is checked
 * before effects. Result packet remains root-owned; no credit is released. */
PAPACC_RESULT papacc_configuration_d3_begin08(PAPACC_CONFIGURATION_COMMIT *,
 const PAPACC_CONFIG_PIN *,const PAPACC_U8 *,PAPACC_SIZE,HANDLE,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_d3_poll08(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);
/* Factual native commit result, NOT CURRENT/recovery/authorization. Epochs
 * explicitly absent; caller owns full collection copy, no borrowed pointers. */
typedef struct PAPACC_D3_COMMITTED_RECEIPT {
 PAPACC_U8 root[32],commit_digest[32],whole_digest[32];
 PAPACC_U64 revision,predecessor;
 PAPACC_SIZE bytes;
 unsigned listener_count,epochs_present;
} PAPACC_D3_COMMITTED_RECEIPT;
/* Same owner/offline/no-reentry contract as begin08. already_reserved includes
 * the caller receipt+buffers; separate9408 scratch is checked. Original begin
 * input may be reused only after it returned: worker owns frozen copies. */
PAPACC_RESULT papacc_configuration_d3_committed08(PAPACC_CONFIGURATION_COMMIT *,
 PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PAPACC_D3_COMMITTED_RECEIPT *,PAPACC_SIZE);
#endif
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_OFFLINE_D3_WORKER) && defined(PAPACC_ED11_WINDOWS_3MIB)
#include "config_root8_v3.h"
#include "server_issuer_windows_validation.h"
typedef struct PAPACC_CONFIG_V3_BORROW {
 const PAPACC_U8 *packet,*snt,*pdg,*issuer_image,*ca;
 PAPACC_SIZE packet_bytes,snt_bytes,pdg_bytes,ca_bytes;
 PAPACC_U64 revision,issuer_revision;
} PAPACC_CONFIG_V3_BORROW;
/* DATA-only capture into a prepared free slot. Existing public codec ports are
 * synchronous/nonreentrant, never authority callbacks. Caller retains immutable
 * disjoint inputs and exclusive workspace until return, serializes owner/Stop.
 * Capture does NOT commit/publish/create a pin or mark issuer/CURRENT valid.
 * Missing native/profile/source validation remains its independent gate. */
PAPACC_RESULT papacc_configuration_v3_capture_data(PAPACC_CONFIGURATION_COMMIT *,
 const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_ISSUER_WINDOWS_WORKSPACE *,PC_ROOT8_COMPONENT,void *);
/* Borrow only under the SAME already-held canonical ticket. Caller must keep
 * that ticket unreleased for the entire use (including children/callbacks).
 * No extra acquisition or release API. Stop denies new borrows, preserves old
 * references; unpin(retained) and finish keep their factual-quiescence rules.
 * Output unchanged on failure. Root/slot/pin must outlive the complete borrow.
 * Raw DER has no CERT_INFO pointers; future decode owns its own accounted arena.
 * Historical pin protects memory, NEVER CURRENT/UTC/status/delegation/trust. */
PAPACC_RESULT papacc_configuration_v3_borrow_data(PAPACC_CONFIGURATION_COMMIT *,
 const PAPACC_CONFIG_PIN *,PAPACC_CONFIG_V3_BORROW *);
/* Same backing slots and canonical tickets; v4 never enters a v3 borrower.
 * No new owner/pin/worker or native commit support. All preprotected-caller
 * and Stop/retention rules above apply. Native key DATA adapter is required;
 * no decoded source bytes or model callback can establish source authority. */
typedef struct PAPACC_CONFIG_V4_BORROW {
 PAPACC_CONFIG_V3_BORROW base;
 const PAPACC_U8 *tsc;
 PAPACC_SIZE tsc_bytes;
} PAPACC_CONFIG_V4_BORROW;
PAPACC_RESULT papacc_configuration_v4_capture_data(PAPACC_CONFIGURATION_COMMIT *,
 const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_ISSUER_WINDOWS_WORKSPACE *,PC_ROOT8_COMPONENT,void *);
PAPACC_RESULT papacc_configuration_v4_borrow_data(PAPACC_CONFIGURATION_COMMIT *,
 const PAPACC_CONFIG_PIN *,PAPACC_CONFIG_V4_BORROW *);
/* Same-pin DATA equation only: stored packet/CP3 vs slot R/predecessor/A/P/I.
 * OK is NOT native commit, full-predecessor approval, CURRENT or E0 evidence.
 * No output borrow/pin/lease, mutation, allocation or publication. */
PAPACC_RESULT papacc_configuration_v3_checkpoint_match_data(
 PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *);

/* Synchronous lifetime/staging ONLY. On OK, consumes the existing pin by move
 * (caller pin cleared), owns a free G7 slot and copies complete input by value.
 * No validation/RESOLVED/commit/CURRENT. Root must separately prove authority.
 * Input remains caller-owned through return; reserved is lower-bound accounting,
 * never a grant. Abort releases only after factual cohort quiescence; retained
 * phase keeps the same pin/slot reachable. No candidate borrow is exported. */
/* Private synchronous DATA phase. Candidate views exist only inside validate;
 * no candidate pin or authority. Ports are nonreentrant; no pointer may escape. */
PAPACC_RESULT papacc_configuration_storage_borrow_data(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,PAPACC_CONFIG_V3_BORROW *,unsigned);
PAPACC_RESULT papacc_configuration_candidate_borrow_data(PAPACC_CONFIGURATION_COMMIT *,PAPACC_U64,PAPACC_U64,PAPACC_CONFIG_V3_BORROW *);
PAPACC_RESULT papacc_configuration_prepared_storage_validate_data(PAPACC_CONFIGURATION_COMMIT *,PC_ROOT8_COMPONENT,void *);
PAPACC_RESULT papacc_configuration_v4_update_hold_data(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CONFIG_PIN *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE,PAPACC_U64);
PAPACC_RESULT papacc_configuration_v3_update_hold_data(PAPACC_CONFIGURATION_COMMIT *,
 PAPACC_CONFIG_PIN *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE,PAPACC_U64);
/* Same held cohort only. Capture is an explicit local act, NOT resolution.
 * Native capture/revalidation never accepts a caller authorization flag.
 * Root must prove CURRENT/source before and after these private components. */
PAPACC_RESULT papacc_configuration_v3_update_act_capture(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_U8 *,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_v3_update_act_revalidate(PAPACC_CONFIGURATION_COMMIT *);
/* Pure correlation for diagnostics: never native identity/CURRENT/status. */
PAPACC_RESULT papacc_configuration_v3_update_act_match_data(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_v3_update_abort_data(PAPACC_CONFIGURATION_COMMIT *);

#endif
#endif
