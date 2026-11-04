#ifndef PAPACC_CONFIGURATION_PENDING_WIN32_H
#define PAPACC_CONFIGURATION_PENDING_WIN32_H
#include "server_store_hash_win32.h"
#include "personal_plan_data.h"
#include "personal_native_win32.h"
#include "server_ca_key_win32.h"
#include "../../src/issuer/issuer_transaction.h"
#include "../../src/core/ed11_process_profile.h"
/* Storage capacity only: format/admission/authority guards remain independent.
 * Historical codec constants and non-selected targets are unchanged. */
#if defined(PAPACC_ED11_WINDOWS_3MIB)
#include "../../src/config/config_root8_v5.h"
#define PAPACC_CONFIG_TRANSACTION_BYTES PC_ROOT8_V5_MAX
#else
#define PAPACC_CONFIG_TRANSACTION_BYTES PAPACC_ISSUER_LINKED_ROOT_BYTES
#endif
/* Embedded scratch in the SAME durable root. Worker owns no root pointer,
 * lock, Session, lease, publication callback or operational authority.
 * Inputs frozen before CreateThread; outputs read only after thread signal.
 * 64KiB reserved stack, one thread, no queue, no retry or forced termination. */
#define PAPACC_PENDING_STACK_BYTES 65536U
/* Additional serialized owner-side encode/hash scratch reservation. */
#define PAPACC_CHECKPOINT_STACK_BYTES 8192U
typedef struct PAPACC_CONFIG_PENDING {
 PAPACC_PERSONAL_NATIVE_CONTEXT personal_native;
 /* Borrow only; canonical CA owner stays durable on the writer thread. */
 PAPACC_CA_KEY *personal_ca;
 PAPACC_CA_KEY *personal_ca_candidate; /* caller-owned native key, never copied */
 unsigned ca_initialize_phase; /* 0 untouched, 1 taken, 2 created, 3 borrowed, 4 retained */
 SECURITY_STATUS ca_initialize_first; /* first result, not operational permission */
 unsigned personal_ca_used; /* monotonic one borrow acquisition per attempt */
 PAPACC_PERSONAL_PLAN_DATA personal_data; /* owned DATA only, never native permission */
 PAPACC_STORE_HASH store_hash;
 unsigned char previous[PAPACC_CONFIG_TRANSACTION_BYTES],next[PAPACC_CONFIG_TRANSACTION_BYTES];
 /* Read-only comparison scratch, never exported/pinned or the CAS preimage.
  * A synchronous read relinquishes its OS borrow before this chunk is reused. */
 unsigned char observed[512];
 char path[MAX_PATH];DWORD previous_bytes,next_bytes,native_error,close_error;
 int checkpoint_only;
 /* Explicit packet discriminator; native identity is never taken from D3 R. */
 unsigned packet_format;DWORD expected_volume,expected_index_high,expected_index_low;
 /* Same worker owns these namespace leases until signaled and explicitly closed.
  * Parent/container native identities are never serialized or CURRENT. */
 HANDLE bootstrap_parent,bootstrap_container,bootstrap_token;
 DWORD bootstrap_parent_volume,bootstrap_parent_high,bootstrap_parent_low;
 DWORD bootstrap_volume,bootstrap_high,bootstrap_low;
 unsigned bootstrap_created,bootstrap_ready,bootstrap_admin,bootstrap_close_attempts;
 HANDLE thread,file,transaction;volatile LONG stop;
 PAPACC_ISSUER_CAS_RESULT result;int active,committed,thread_close_attempted;
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 HANDLE test_entered,test_release;int test_unknown;
#endif
} PAPACC_CONFIG_PENDING;
#endif
