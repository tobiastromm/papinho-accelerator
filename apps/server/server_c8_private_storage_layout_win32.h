#ifndef PAPACC_C8_PRIVATE_STORAGE_LAYOUT_WIN32_H
#define PAPACC_C8_PRIVATE_STORAGE_LAYOUT_WIN32_H
#ifndef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#error Private_C8_storage_layout_requires_its_explicit_exclusive_type_profile
#endif
#include "server_launcher_owner_win32.h"
#include "server_c2_owner_win32.h"
#include "server_c8_stage_pool_win32.h"
#include "server_c8_policy_adapter_win32.h"
#include "server_c2_participant_win32.h"
/* Future C8 durable containment type only; no functional C8 is installed by this
 * declaration. Do not link its differently laid out loop with old objects. */
typedef struct PAPACC_C8_PRIVATE_STORAGE_LAYOUT {
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 launcher;
 PAPACC_C8_POLICY_ADAPTER policy_adapter;
 PAPACC_SERVER_C2_OWNER_WIN32 c2;
 PAPACC_SERVER_C2_PARTICIPANT participant; /* contains the only manager */
 PAPACC_C3_BRIDGE bridges[2];
 PAPACC_CONNECTION_MANAGER site_manager;
 PAPACC_CONNECTION site_connections[2];
 PAPACC_C8_STAGE_POOL stages;
 /* Persistent by-value leases/tickets for every potentially pending stage. */
 PAPACC_C8_STAGE_LEASE stage_leases[PAPACC_C8_STAGE_COUNT];
 PAPACC_C8_STAGE_BORROW stage_borrows[PAPACC_C8_STAGE_COUNT];
} PAPACC_C8_PRIVATE_STORAGE_LAYOUT;
#define PAPACC_C8_BASE_ARRAY_BYTES (PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY*(sizeof(PAPACC_CONNECTION)+sizeof(PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)+sizeof(PAPACC_SESSION)+sizeof(PAPACC_CHANNEL)+sizeof(PAPACC_DATA_ASSOCIATION_ENTRY)+sizeof(PAPACC_SERVER_PROTOCOL_SLOT_WIN32)))
#define PAPACC_C8_SECURE_ARRAY_BYTES (PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY*sizeof(PAPACC_SERVER_SECURE_CANDIDATE_WIN32))
#define PAPACC_C8_SCHED_ARRAY_BYTES ((PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY+PAPACC_SERVER_NETWORK_STREAM_CAPACITY+1U)*(sizeof(PAPACC_PST_SCHEDULER_MEMBER)+sizeof(PST_WAIT_EVENT))+sizeof(pst_external_source*)+sizeof(pst_wait_token))
#define PAPACC_C8_BIND_LISTENER_BYTES (2U*sizeof(PAPACC_TCP_LISTENER_ENTRY_WIN32)+sizeof(PAPACC_NETWORK_INTERFACE_PERSISTENT_ID))
/* One listener and ID, exactly the preceding direct-composition census.
 * Variable listener/ID counts and future configuration allocations must be
 * checked against remaining credit; opaque/provider/kernel are NOT zero. */
#define PAPACC_C8_KNOWN_MANAGED_BYTES (PAPACC_PENDING_STACK_BYTES+PAPACC_CHECKPOINT_STACK_BYTES+sizeof(PAPACC_C8_PRIVATE_STORAGE_LAYOUT)+PAPACC_C8_BASE_ARRAY_BYTES+PAPACC_C8_SECURE_ARRAY_BYTES+PAPACC_C8_SCHED_ARRAY_BYTES+PAPACC_C8_BIND_LISTENER_BYTES)
typedef char papacc_c8_known_storage_fits[(PAPACC_C8_KNOWN_MANAGED_BYTES<=PAPACC_C8_STORAGE_LIMIT)?1:-1];
#endif
