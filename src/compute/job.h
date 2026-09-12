#ifndef PAPACC_JOB_H
#define PAPACC_JOB_H

#include <papacc/types.h>
#include "capability.h"
#include "session.h"

typedef enum PAPACC_JOB_STATE {
    PAPACC_JOB_STATE_UNUSED = 0,
    PAPACC_JOB_STATE_QUEUED = 1,
    PAPACC_JOB_STATE_RUNNING = 2,
    PAPACC_JOB_STATE_COMPLETED = 3,
    PAPACC_JOB_STATE_FAILED = 4,
    PAPACC_JOB_STATE_CANCELLED = 5
} PAPACC_JOB_STATE;

typedef enum PAPACC_JOB_TERMINAL_REASON {
    PAPACC_JOB_REASON_NONE = 0,
    PAPACC_JOB_REASON_SUCCESS = 1,
    PAPACC_JOB_REASON_BACKEND_FAILURE = 2,
    PAPACC_JOB_REASON_CANCELLED = 3,
    PAPACC_JOB_REASON_DEADLINE = 4,
    PAPACC_JOB_REASON_SESSION_LOST = 5
} PAPACC_JOB_TERMINAL_REASON;

typedef enum PAPACC_COMPUTE_STEP_STATUS {
    PAPACC_COMPUTE_STEP_PROGRESS = 1,
    PAPACC_COMPUTE_STEP_WOULD_BLOCK = 2,
    PAPACC_COMPUTE_STEP_COMPLETED = 3,
    PAPACC_COMPUTE_STEP_FAILED = 4,
    PAPACC_COMPUTE_STEP_CANCELLED = 5
} PAPACC_COMPUTE_STEP_STATUS;

typedef struct PAPACC_COMPUTE_BACKEND {
    const char *diagnostic_name;
    void *context;
    PAPACC_RESULT (*prepare)(void *, void *, void **);
    PAPACC_RESULT (*start)(void *, void *, PAPACC_COMPUTE_STEP_STATUS *);
    PAPACC_RESULT (*step)(void *, void *, PAPACC_COMPUTE_STEP_STATUS *);
    PAPACC_RESULT (*request_cancel)(void *, void *);
    void (*destroy)(void *, void *);
} PAPACC_COMPUTE_BACKEND;

typedef struct PAPACC_JOB {
    PAPACC_U64 job_instance_id;
    PAPACC_U64 owner_session_instance_id;
    PAPACC_CAPABILITY_KEY required_capability;
    PAPACC_JOB_STATE state;
    PAPACC_JOB_TERMINAL_REASON terminal_reason;
    PAPACC_BOOL cancel_requested;
    PAPACC_BOOL suppress_result;
    PAPACC_U64 created_at_ns;
    PAPACC_U64 started_at_ns;
    PAPACC_U64 deadline_ns;
    PAPACC_U64 progress_count;
    const PAPACC_COMPUTE_BACKEND *backend;
    void *backend_execution;
} PAPACC_JOB;

#define PAPACC_JOB_INITIALIZER { 0U, 0U, PAPACC_CAPABILITY_KEY_INITIALIZER, \
    PAPACC_JOB_STATE_UNUSED, PAPACC_JOB_REASON_NONE, PAPACC_FALSE, \
    PAPACC_FALSE, 0U, 0U, 0U, 0U, NULL, NULL }

typedef struct PAPACC_JOB_ADMISSION {
    PAPACC_U64 session_instance_id;
    const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;
    const PAPACC_CAPABILITY_KEY *required_capability;
    const PAPACC_CAPABILITY_SET *server_enabled_now;
    PAPACC_CAPABILITY_POLICY_FN contextual_policy;
    void *contextual_policy_context;
    const PAPACC_COMPUTE_BACKEND *backend;
    void *workload_context;
    PAPACC_U64 now_ns;
    PAPACC_U64 deadline_ns;
} PAPACC_JOB_ADMISSION;

typedef struct PAPACC_JOB_MANAGER {
    PAPACC_JOB *storage;
    PAPACC_SIZE capacity;
    PAPACC_SIZE count;
    PAPACC_SIZE per_session_limit;
    PAPACC_SIZE cursor;
    PAPACC_U64 last_session_id;
    PAPACC_U64 next_instance_id;
    PAPACC_SESSION_MANAGER *session_manager;
    PAPACC_BOOL initialized;
} PAPACC_JOB_MANAGER;

#define PAPACC_JOB_MANAGER_INITIALIZER \
    { NULL, 0U, 0U, 0U, 0U, 0U, 1U, NULL, PAPACC_FALSE }

PAPACC_BOOL papacc_job_state_terminal(PAPACC_JOB_STATE state);
PAPACC_RESULT papacc_job_manager_init(PAPACC_JOB_MANAGER *, PAPACC_JOB *,
    PAPACC_SIZE, PAPACC_SIZE, PAPACC_SESSION_MANAGER *);
PAPACC_RESULT papacc_job_manager_admit(PAPACC_JOB_MANAGER *,
    const PAPACC_JOB_ADMISSION *, PAPACC_JOB **);
PAPACC_JOB *papacc_job_manager_find(PAPACC_JOB_MANAGER *, PAPACC_U64);
PAPACC_JOB *papacc_job_manager_find_for_session(PAPACC_JOB_MANAGER *,
    PAPACC_U64, PAPACC_U64);
const PAPACC_JOB *papacc_job_manager_inspect_at(const PAPACC_JOB_MANAGER *,
    PAPACC_SIZE);
PAPACC_RESULT papacc_job_manager_cancel(PAPACC_JOB_MANAGER *, PAPACC_U64);
PAPACC_RESULT papacc_job_manager_cancel_session(PAPACC_JOB_MANAGER *,
    PAPACC_U64);
PAPACC_RESULT papacc_job_manager_progress_one(PAPACC_JOB_MANAGER *,
    PAPACC_U64, PAPACC_JOB **);
PAPACC_RESULT papacc_job_manager_reap(PAPACC_JOB_MANAGER *, PAPACC_U64);
void papacc_job_manager_shutdown(PAPACC_JOB_MANAGER *);

#endif
