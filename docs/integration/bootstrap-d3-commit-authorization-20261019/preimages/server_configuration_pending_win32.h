#ifndef PAPACC_CONFIGURATION_PENDING_WIN32_H
#define PAPACC_CONFIGURATION_PENDING_WIN32_H
#include "../../src/issuer/issuer_transaction.h"
/* Embedded scratch in the SAME durable root. Worker owns no root pointer,
 * lock, Session, lease, publication callback or operational authority.
 * Inputs frozen before CreateThread; outputs read only after thread signal.
 * 64KiB reserved stack, one thread, no queue, no retry or forced termination. */
#define PAPACC_PENDING_STACK_BYTES 65536U
/* Additional serialized owner-side encode/hash scratch reservation. */
#define PAPACC_CHECKPOINT_STACK_BYTES 8192U
typedef struct PAPACC_CONFIG_PENDING {
 unsigned char previous[PAPACC_ISSUER_LINKED_ROOT_BYTES],next[PAPACC_ISSUER_LINKED_ROOT_BYTES],observed[PAPACC_ISSUER_LINKED_ROOT_BYTES];
 char path[MAX_PATH];DWORD previous_bytes,next_bytes,native_error,close_error;
 int checkpoint_only;
 /* Explicit packet discriminator; native identity is never taken from D3 R. */
 unsigned packet_format;DWORD expected_volume,expected_index_high,expected_index_low;
 /* Same worker owns these namespace leases until signaled and explicitly closed.
  * Parent/container native identities are never serialized or CURRENT. */
 HANDLE bootstrap_parent,bootstrap_container;
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
