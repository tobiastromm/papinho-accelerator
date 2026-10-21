#include "../../src/core/ed11_process_profile.h"
#ifndef PAPACC_C8_STAGE_POOL_WIN32_H
#define PAPACC_C8_STAGE_POOL_WIN32_H
/* Private storage leases, NEVER authentication, grants, binding or PST authority.
 * Embed one pool in the same durable process root as its users. No shared
 * writable stage between independently live channels or retained lifetimes. */
#include "papacc/types.h"
#define PAPACC_C8_STAGE_COUNT 8U
#define PAPACC_C8_STAGE_BYTES 16384U
#define PAPACC_C8_STORAGE_LIMIT PAPACC_ED11_PROCESS_LIMIT
typedef enum PAPACC_C8_STAGE_STATE {
 PAPACC_C8_STAGE_FREE=0,PAPACC_C8_STAGE_ACTIVE,
 PAPACC_C8_STAGE_QUIESCING,PAPACC_C8_STAGE_RETAINED
} PAPACC_C8_STAGE_STATE;
typedef struct PAPACC_C8_STAGE_KEY {
 PAPACC_U64 owner,session,connection,operation,generation;
 unsigned role;
} PAPACC_C8_STAGE_KEY;
typedef struct PAPACC_C8_STAGE_LEASE {
 PAPACC_U64 root,serial;PAPACC_C8_STAGE_KEY key;unsigned index;
} PAPACC_C8_STAGE_LEASE;
typedef struct PAPACC_C8_STAGE_BORROW {
 PAPACC_C8_STAGE_LEASE lease;PAPACC_U64 ticket;
} PAPACC_C8_STAGE_BORROW;
typedef struct PAPACC_C8_STAGE {
 PAPACC_C8_STAGE_KEY key;PAPACC_U64 serial,borrow_ticket;PAPACC_U32 borrowers;
 PAPACC_C8_STAGE_STATE state;
 PAPACC_U8 bytes[PAPACC_C8_STAGE_BYTES];
} PAPACC_C8_STAGE;
typedef struct PAPACC_C8_STAGE_POOL {
 PAPACC_C8_STAGE stages[PAPACC_C8_STAGE_COUNT];
 PAPACC_U64 root,next_serial;PAPACC_U32 thread;
 PAPACC_SIZE reserved_bytes,used,high_water;
 PAPACC_BOOL initialized,stopping,finalized;
} PAPACC_C8_STAGE_POOL;
/* Fresh zeroed durable storage; root comes from a non-reused launcher invocation.
 * No reinitialization of an existing pool, including after terminal retention.
 * Caller must reserve the COMBINED graph first, not eight independent credits. */
PAPACC_RESULT papacc_c8_stage_pool_init(PAPACC_C8_STAGE_POOL*,PAPACC_U64,PAPACC_SIZE);
PAPACC_RESULT papacc_c8_stage_acquire(PAPACC_C8_STAGE_POOL*,const PAPACC_C8_STAGE_KEY*,PAPACC_C8_STAGE_LEASE*);
PAPACC_RESULT papacc_c8_stage_borrow(PAPACC_C8_STAGE_POOL*,const PAPACC_C8_STAGE_LEASE*,PAPACC_C8_STAGE_BORROW*,PAPACC_U8**,PAPACC_SIZE*);
PAPACC_RESULT papacc_c8_stage_return(PAPACC_C8_STAGE_POOL*,const PAPACC_C8_STAGE_BORROW*);
PAPACC_RESULT papacc_c8_stage_cancel(PAPACC_C8_STAGE_POOL*,const PAPACC_C8_STAGE_LEASE*);
PAPACC_RESULT papacc_c8_stage_retain(PAPACC_C8_STAGE_POOL*,const PAPACC_C8_STAGE_LEASE*);
/* users_finalized is a caller storage-lifetime postcondition, NOT a fabricated
 * PST disposition. No release until quiescing + all borrows returned. */
PAPACC_RESULT papacc_c8_stage_release(PAPACC_C8_STAGE_POOL*,const PAPACC_C8_STAGE_LEASE*,PAPACC_BOOL);
PAPACC_RESULT papacc_c8_stage_stop(PAPACC_C8_STAGE_POOL*);
PAPACC_RESULT papacc_c8_stage_pool_finish(PAPACC_C8_STAGE_POOL*);
#endif
