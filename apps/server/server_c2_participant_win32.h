#include "../../src/core/ed11_process_profile.h"
#ifndef PAPACC_SERVER_C2_PARTICIPANT_H
#define PAPACC_SERVER_C2_PARTICIPANT_H
#include "server_config.h"
#include "c2_operation_win32.h"
#include "capability.h"
#define PAPACC_C2_PARTICIPANT_IDS 8U
#define PAPACC_C2_PARTICIPANT_LIMIT PAPACC_ED11_PROCESS_LIMIT
typedef enum PAPACC_C2_PARTICIPANT_STATE {
 PAPACC_C2_PARTICIPANT_EMPTY=0, PAPACC_C2_PARTICIPANT_READY,
 PAPACC_C2_PARTICIPANT_CLOSED, PAPACC_C2_PARTICIPANT_RETAINED
} PAPACC_C2_PARTICIPANT_STATE;
/* Root-owned durable storage. Noncopyable after prepare; never an IO owner.
 * All arrays are charged once by sizeof, before manager initialization.
 * Root identity is correlation, not a commit, grant or operational authority.
 * Caller keeps this object and root identity alive until CLOSED, or indefinitely
 * when RETAINED. No API transfers ownership or permits reinitialization. */
typedef struct PAPACC_SERVER_C2_PARTICIPANT {
 const struct PAPACC_SERVER_C2_PARTICIPANT *self;
 const void *root;
 PAPACC_U64 invocation;
 PAPACC_U32 thread;
 PAPACC_C2_PARTICIPANT_STATE state;
 PAPACC_SIZE reserved, combined;
 PAPACC_SERVER_CONFIG model;
 PAPACC_NETWORK_INTERFACE_PERSISTENT_ID ids[PAPACC_C2_PARTICIPANT_IDS];
 PAPACC_CAPABILITY_KEY enabled[2]; PAPACC_SIZE enabled_count;
 PAPACC_C2_OPERATION_MANAGER manager;
} PAPACC_SERVER_C2_PARTICIPANT;
PAPACC_RESULT papacc_server_c2_participant_prepare(PAPACC_SERVER_C2_PARTICIPANT *,
 const void *,PAPACC_U64,const PAPACC_SERVER_CONFIG *,
 const PAPACC_CAPABILITY_EVALUATION_INPUTS *,PAPACC_SIZE);
PAPACC_RESULT papacc_server_c2_participant_validate(const PAPACC_SERVER_C2_PARTICIPANT *,const void *,PAPACC_U64);
PAPACC_RESULT papacc_server_c2_participant_rollback(PAPACC_SERVER_C2_PARTICIPANT *,const void *,PAPACC_U64);
#ifdef PAPACC_ENABLE_CANONICAL_C8_ROOT
struct PAPACC_SERVER_C2_OWNER_WIN32;
/* Separate from rollback: the existing consumer must be detached, configuration
 * CLOSED and every Session/operation/write/bridge reference factually retired. */
PAPACC_RESULT papacc_server_c2_participant_retire(PAPACC_SERVER_C2_PARTICIPANT *,
 const void *,PAPACC_U64,const struct PAPACC_SERVER_C2_OWNER_WIN32 *);
#endif
/* Stop is rollback only for a never-consumed empty participant. */
PAPACC_RESULT papacc_server_c2_participant_stop(PAPACC_SERVER_C2_PARTICIPANT *,const void *,PAPACC_U64);
#endif