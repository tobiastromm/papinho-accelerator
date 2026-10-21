#ifndef PAPACC_SERVER_C8_TARGET_EVIDENCE_WIN32_H
#define PAPACC_SERVER_C8_TARGET_EVIDENCE_WIN32_H
#include "server_c8_resource_admission_win32.h"
/* Raw provenance is not configuration authority or empirical approval. */
typedef struct PAPACC_C8_TARGET_BINDING {
 PAPACC_U64 invocation,owner,revision,epoch,managed,charged;
 PAPACC_U8 domain[32];
 PAPACC_U32 profile,phase,configured,current,concurrency,retained,stopping;
} PAPACC_C8_TARGET_BINDING;
typedef struct PAPACC_C8_TARGET_OBSERVATION {
 PAPACC_U64 started_ms,finished_ms,creation,physical,commit,private_bytes,working_set,paged,nonpaged;
 PAPACC_U32 process,thread,handles,errors[5],success,version_ok;
} PAPACC_C8_TARGET_OBSERVATION;
/* Finite diagnostic history. Exhaustion makes capture incomplete; no overwrite. */
#define PAPACC_C8_TARGET_HISTORY_CAPACITY 8U
typedef enum PAPACC_C8_OBSERVATION_PHASE {
 PAPACC_C8_OBSERVATION_PREPARED=1,PAPACC_C8_OBSERVATION_ACTIVE=2,
 PAPACC_C8_OBSERVATION_DRAINING=3,PAPACC_C8_OBSERVATION_TEARDOWN=4,
 PAPACC_C8_OBSERVATION_RETAINED=5,PAPACC_C8_OBSERVATION_SHUTDOWN_PENDING=6,
 PAPACC_C8_OBSERVATION_BASELINE=7
} PAPACC_C8_OBSERVATION_PHASE;
typedef struct PAPACC_C8_TARGET_HISTORY {
 PAPACC_C8_TARGET_BINDING binding;
 PAPACC_C8_TARGET_EVIDENCE evidence;
 PAPACC_U64 creation,series_id,first_sequence,last_sequence,started_ms;
 PAPACC_U32 process,thread,status;
} PAPACC_C8_TARGET_HISTORY;
typedef struct PAPACC_C8_TARGET_PRODUCER {
 PAPACC_C8_TARGET_BINDING binding;
 PAPACC_C8_TARGET_EVIDENCE evidence;
 PAPACC_U64 creation,sequence,last_attempt_ms,working_set;
 PAPACC_U32 process,thread,errors[5],status;
 PAPACC_U64 series_id,series_first_sequence,series_started_ms;
 PAPACC_U32 history_count,history_overflow,last_attempt_origin;
 PAPACC_C8_TARGET_HISTORY history[PAPACC_C8_TARGET_HISTORY_CAPACITY];
} PAPACC_C8_TARGET_PRODUCER;
/* Private diagnostic result, independent of provenance and sample contents. */
#define PAPACC_C8_COLLECT_OK 0U
#define PAPACC_C8_COLLECT_RATE 1U
#define PAPACC_C8_COLLECT_CLOCK 2U
#define PAPACC_C8_COLLECT_BUSY 3U
#define PAPACC_C8_COLLECT_IDENTITY 4U
#define PAPACC_C8_COLLECT_FAILED 5U
#define PAPACC_C8_COLLECT_OVERFLOW 6U
#define PAPACC_C8_COLLECTION_CLOCK_INCOMPLETE 1U
#define PAPACC_C8_COLLECTION_OVERFLOW_INCOMPLETE 2U
#define PAPACC_C8_COLLECTION_STORAGE 128U
#define PAPACC_C8_COLLECTION_RESERVATION 144U /* bank plus conservative alignment */
PAPACC_RESULT papacc_c8_target_collect_diagnostic(PAPACC_C8_TARGET_PRODUCER *,const PAPACC_C8_TARGET_BINDING *,PAPACC_U32 *,PAPACC_U32 *);
/* Record a factual clock fault without collecting or advancing cadence.
 * unavailable poisons unknown completion; known regression stays sticky. */
PAPACC_RESULT papacc_c8_collection_clock_fault(const PAPACC_C8_TARGET_BINDING *,PAPACC_BOOL);
PAPACC_RESULT papacc_c8_collection_health(const PAPACC_C8_TARGET_BINDING *,PAPACC_U32 *);
/* No callbacks or loop. One own-process sample; rate limited to 100ms.
 * Caller must own this by-value state and be outside active dispatch. */
PAPACC_RESULT papacc_c8_target_collect(PAPACC_C8_TARGET_PRODUCER *,const PAPACC_C8_TARGET_BINDING *);
/* Explicit synthetic reducer for offline negatives; never native origin. */
PAPACC_RESULT papacc_c8_target_observe_offline(PAPACC_C8_TARGET_PRODUCER *,const PAPACC_C8_TARGET_BINDING *,const PAPACC_C8_TARGET_OBSERVATION *);
PAPACC_C8_RESOURCE_REASON papacc_c8_target_validate_raw(const PAPACC_C8_TARGET_PRODUCER *,const PAPACC_C8_TARGET_BINDING *,const PAPACC_U8 *,PAPACC_U64);
PAPACC_C8_RESOURCE_REASON papacc_c8_target_admission(const PAPACC_C8_TARGET_PRODUCER *,const PAPACC_C8_TARGET_BINDING *,const PAPACC_U8 *,PAPACC_U64);
#endif
