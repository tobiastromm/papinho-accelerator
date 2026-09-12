#include "job.h"

typedef struct TEST_EXEC {
    int steps_left;
    int fail;
    int stall;
    int cancel;
} TEST_EXEC;

typedef struct TEST_BACKEND {
    TEST_EXEC executions[16];
    PAPACC_SIZE used;
    int prepare_fail;
    int starts;
    int steps;
    int cancels;
    int destroys;
} TEST_BACKEND;

typedef struct TEST_WORK { int steps; int fail; int stall; } TEST_WORK;

static PAPACC_RESULT policy(void *context, const PAPACC_CAPABILITY_KEY *key,
    PAPACC_CAPABILITY_POLICY_DECISION *decision)
{
    (void)key;
    *decision = (*(int *)context != 0) ? PAPACC_CAPABILITY_POLICY_ALLOW :
        PAPACC_CAPABILITY_POLICY_DENY;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT prepare(void *context, void *work, void **out)
{
    TEST_BACKEND *b = (TEST_BACKEND *)context;
    TEST_WORK *w = (TEST_WORK *)work;
    TEST_EXEC *e;
    *out = NULL;
    if (b->prepare_fail) return PAPACC_RESULT_INTERNAL_ERROR;
    if (b->used >= 16U) return PAPACC_RESULT_LIMIT_EXCEEDED;
    e = &b->executions[b->used++];
    e->steps_left = w->steps; e->fail = w->fail;
    e->stall = w->stall; e->cancel = 0; *out = e;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT advance(TEST_BACKEND *b, TEST_EXEC *e,
    PAPACC_COMPUTE_STEP_STATUS *status)
{
    (void)b;
    if (e->cancel) { *status = PAPACC_COMPUTE_STEP_CANCELLED; return PAPACC_RESULT_OK; }
    if (e->stall) { *status = PAPACC_COMPUTE_STEP_WOULD_BLOCK; return PAPACC_RESULT_OK; }
    if (e->fail) { *status = PAPACC_COMPUTE_STEP_FAILED; return PAPACC_RESULT_OK; }
    if (e->steps_left == 0) *status = PAPACC_COMPUTE_STEP_COMPLETED;
    else { --e->steps_left; *status = PAPACC_COMPUTE_STEP_PROGRESS; }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT start(void *context, void *execution,
    PAPACC_COMPUTE_STEP_STATUS *status)
{
    TEST_BACKEND *b = (TEST_BACKEND *)context; ++b->starts;
    return advance(b, (TEST_EXEC *)execution, status);
}
static PAPACC_RESULT step(void *context, void *execution,
    PAPACC_COMPUTE_STEP_STATUS *status)
{
    TEST_BACKEND *b = (TEST_BACKEND *)context; ++b->steps;
    return advance(b, (TEST_EXEC *)execution, status);
}
static PAPACC_RESULT cancel(void *context, void *execution)
{
    TEST_BACKEND *b = (TEST_BACKEND *)context; ++b->cancels;
    ((TEST_EXEC *)execution)->cancel = 1; return PAPACC_RESULT_OK;
}
static void destroy(void *context, void *execution)
{
    TEST_BACKEND *b = (TEST_BACKEND *)context; (void)execution; ++b->destroys;
}

static PAPACC_RESULT setup_snapshot(PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    PAPACC_CAPABILITY_KEY *snapshot_storage, PAPACC_CAPABILITY_REGISTRY *registry,
    PAPACC_CAPABILITY_DESCRIPTOR *descriptors, PAPACC_CAPABILITY_SET *sets,
    PAPACC_CAPABILITY_KEY storage[][1], PAPACC_CAPABILITY_KEY key, int *allow)
{
    PAPACC_CAPABILITY_EVALUATION_INPUTS inputs;
    PAPACC_SIZE i;
    PAPACC_RESULT result = papacc_capability_registry_init(registry, descriptors, 1U);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_capability_registry_register(registry, &key, "TEST_COMPUTE");
    if (result != PAPACC_RESULT_OK) return result;
    for (i = 0U; i < 4U; ++i) {
        result = papacc_capability_set_init(&sets[i], storage[i], 1U);
        if (result != PAPACC_RESULT_OK) return result;
        result = papacc_capability_set_insert(&sets[i], &key);
        if (result != PAPACC_RESULT_OK) return result;
    }
    inputs.registry = registry; inputs.server_supported = &sets[0];
    inputs.server_enabled = &sets[1]; inputs.principal_policy = policy;
    inputs.principal_policy_context = allow; inputs.client_supported = &sets[2];
    inputs.client_preference = &sets[3];
    return papacc_session_capability_snapshot_publish(snapshot, 1U,
        snapshot_storage, 1U, &inputs);
}

int main(void)
{
    PAPACC_SESSION sessions[2]; PAPACC_SESSION_MANAGER sm = PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_SESSION *s1, *s2;
    PAPACC_JOB jobs[4]; PAPACC_JOB_MANAGER jm = PAPACC_JOB_MANAGER_INITIALIZER;
    PAPACC_JOB small_jobs[1];
    PAPACC_JOB_MANAGER small_manager = PAPACC_JOB_MANAGER_INITIALIZER;
    PAPACC_CAPABILITY_KEY key = { 10U, 1U }, snap_store[1], set_store[4][1];
    PAPACC_CAPABILITY_DESCRIPTOR desc[1]; PAPACC_CAPABILITY_REGISTRY reg;
    PAPACC_CAPABILITY_SET sets[4];
    PAPACC_SESSION_CAPABILITY_SNAPSHOT snap = PAPACC_SESSION_CAPABILITY_SNAPSHOT_INITIALIZER;
    TEST_BACKEND tb = { { { 0, 0, 0, 0 } }, 0U, 0, 0, 0, 0, 0 };
    PAPACC_COMPUTE_BACKEND backend = { "test", &tb, prepare, start, step, cancel, destroy };
    TEST_WORK immediate = { 0, 0, 0 }, slow = { 2, 0, 0 }, failing = { 0, 1, 0 };
    TEST_WORK stalled = { 0, 0, 1 };
    PAPACC_JOB_ADMISSION a;
    PAPACC_JOB *j1, *j2, *selected;
    PAPACC_U64 id1; int allow = 1; int destroy_before;

    if (papacc_session_manager_init(&sm, sessions, 2U) != PAPACC_RESULT_OK) return 1;
    if (papacc_session_manager_publish(&sm, &s1) != PAPACC_RESULT_OK ||
        papacc_session_activate(s1) != PAPACC_RESULT_OK) return 2;
    if (papacc_session_manager_publish(&sm, &s2) != PAPACC_RESULT_OK ||
        papacc_session_activate(s2) != PAPACC_RESULT_OK) return 3;
    if (setup_snapshot(&snap, snap_store, &reg, desc, sets, set_store, key,
        &allow) != PAPACC_RESULT_OK) return 4;
    if (papacc_job_manager_init(&jm, jobs, 4U, 2U, &sm) != PAPACC_RESULT_OK) return 5;
    a.session_instance_id = s1->session_instance_id; a.snapshot = &snap;
    a.required_capability = &key; a.server_enabled_now = &sets[1];
    a.contextual_policy = policy; a.contextual_policy_context = &allow;
    a.backend = &backend; a.workload_context = &immediate;
    a.now_ns = 10U; a.deadline_ns = 100U;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_OK ||
        j1 == NULL || j1->job_instance_id == 0U ||
        j1->state != PAPACC_JOB_STATE_QUEUED) return 6;
    if (papacc_job_manager_inspect_at(&jm, 0U) != j1) return 24;
    id1 = j1->job_instance_id;
    if (papacc_job_manager_find_for_session(&jm, s2->session_instance_id, id1) != NULL) return 7;
    if (papacc_job_manager_progress_one(&jm, 11U, &selected) != PAPACC_RESULT_OK ||
        selected != j1 || j1->state != PAPACC_JOB_STATE_COMPLETED || tb.destroys != 1) return 8;
    if (papacc_job_manager_cancel(&jm, id1) != PAPACC_RESULT_OK ||
        j1->state != PAPACC_JOB_STATE_COMPLETED) return 9;
    if (papacc_job_manager_reap(&jm, id1) != PAPACC_RESULT_OK || jm.count != 0U) return 10;

    a.workload_context = &slow;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_OK ||
        papacc_job_manager_admit(&jm, &a, &j2) != PAPACC_RESULT_OK ||
        j1->job_instance_id == j2->job_instance_id) return 11;
    if (papacc_job_manager_admit(&jm, &a, &selected) != PAPACC_RESULT_LIMIT_EXCEEDED) return 12;
    if (papacc_job_manager_cancel(&jm, j2->job_instance_id) != PAPACC_RESULT_OK ||
        j2->state != PAPACC_JOB_STATE_CANCELLED) return 13;
    if (papacc_job_manager_progress_one(&jm, 12U, &selected) != PAPACC_RESULT_OK ||
        selected != j1 || j1->state != PAPACC_JOB_STATE_RUNNING) return 14;
    if (papacc_job_manager_cancel(&jm, j1->job_instance_id) != PAPACC_RESULT_OK ||
        papacc_job_manager_progress_one(&jm, 13U, &selected) != PAPACC_RESULT_OK ||
        j1->state != PAPACC_JOB_STATE_CANCELLED) return 15;
    (void)papacc_job_manager_reap(&jm, j1->job_instance_id);
    (void)papacc_job_manager_reap(&jm, j2->job_instance_id);

    /* Multi-step completion uses exactly one backend opportunity per call. */
    snap.session_instance_id = s1->session_instance_id;
    a.session_instance_id = s1->session_instance_id;
    a.workload_context = &slow;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_OK) return 25;
    if (papacc_job_manager_progress_one(&jm, 14U, &selected) != PAPACC_RESULT_OK ||
        j1->progress_count != 1U || j1->state != PAPACC_JOB_STATE_RUNNING) return 26;
    if (papacc_job_manager_progress_one(&jm, 15U, &selected) != PAPACC_RESULT_OK ||
        j1->progress_count != 2U || j1->state != PAPACC_JOB_STATE_RUNNING) return 27;
    if (papacc_job_manager_progress_one(&jm, 16U, &selected) != PAPACC_RESULT_OK ||
        j1->state != PAPACC_JOB_STATE_COMPLETED) return 28;
    (void)papacc_job_manager_reap(&jm, j1->job_instance_id);

    /* Consecutive opportunities prefer different Sessions when both run. */
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_OK) return 29;
    snap.session_instance_id = s2->session_instance_id;
    a.session_instance_id = s2->session_instance_id;
    if (papacc_job_manager_admit(&jm, &a, &j2) != PAPACC_RESULT_OK) return 30;
    if (papacc_job_manager_progress_one(&jm, 17U, &selected) != PAPACC_RESULT_OK)
        return 31;
    id1 = selected->owner_session_instance_id;
    if (papacc_job_manager_progress_one(&jm, 18U, &selected) != PAPACC_RESULT_OK ||
        selected->owner_session_instance_id == id1) return 32;
    (void)papacc_job_manager_cancel(&jm, j1->job_instance_id);
    (void)papacc_job_manager_cancel(&jm, j2->job_instance_id);
    (void)papacc_job_manager_progress_one(&jm, 19U, &selected);
    (void)papacc_job_manager_progress_one(&jm, 19U, &selected);
    (void)papacc_job_manager_reap(&jm, j1->job_instance_id);
    (void)papacc_job_manager_reap(&jm, j2->job_instance_id);

    allow = 0; destroy_before = tb.destroys;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_INVALID_STATE ||
        jm.count != 0U || tb.destroys != destroy_before) return 16;
    allow = 1; tb.prepare_fail = 1;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_INTERNAL_ERROR || jm.count != 0U) return 17;
    tb.prepare_fail = 0; a.snapshot = NULL;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_INVALID_STATE) return 18;
    a.snapshot = &snap; a.session_instance_id = s2->session_instance_id;
    snap.session_instance_id = s2->session_instance_id;
    a.workload_context = &failing;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_OK ||
        papacc_job_manager_progress_one(&jm, 20U, &selected) != PAPACC_RESULT_OK ||
        j1->state != PAPACC_JOB_STATE_FAILED) return 19;
    (void)papacc_job_manager_reap(&jm, j1->job_instance_id);

    a.workload_context = &stalled; a.deadline_ns = 22U;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_OK ||
        papacc_job_manager_progress_one(&jm, 21U, &selected) != PAPACC_RESULT_OK ||
        papacc_job_manager_progress_one(&jm, 22U, &selected) != PAPACC_RESULT_OK ||
        j1->state != PAPACC_JOB_STATE_CANCELLED ||
        j1->terminal_reason != PAPACC_JOB_REASON_DEADLINE) return 20;
    (void)papacc_job_manager_reap(&jm, j1->job_instance_id);

    a.deadline_ns = 100U; a.workload_context = &slow;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_OK) return 21;
    papacc_session_close(s2);
    if (papacc_job_manager_cancel_session(&jm, s2->session_instance_id) != PAPACC_RESULT_OK ||
        j1->state != PAPACC_JOB_STATE_CANCELLED || j1->suppress_result != PAPACC_TRUE) return 22;
    (void)papacc_job_manager_reap(&jm, j1->job_instance_id);
    snap.session_instance_id = s1->session_instance_id;
    a.session_instance_id = s1->session_instance_id;
    a.workload_context = &immediate; a.deadline_ns = 100U;
    a.backend = NULL;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_INVALID_ARGUMENT ||
        jm.count != 0U) return 33;
    a.backend = &backend; a.session_instance_id = 999U;
    if (papacc_job_manager_admit(&jm, &a, &j1) != PAPACC_RESULT_INVALID_STATE ||
        jm.count != 0U) return 34;
    a.session_instance_id = s1->session_instance_id;
    if (papacc_job_manager_init(&small_manager, small_jobs, 1U, 2U, &sm) !=
        PAPACC_RESULT_OK) return 35;
    if (papacc_job_manager_admit(&small_manager, &a, &j1) != PAPACC_RESULT_OK ||
        papacc_job_manager_admit(&small_manager, &a, &j2) !=
        PAPACC_RESULT_LIMIT_EXCEEDED || small_manager.count != 1U) return 36;
    (void)papacc_job_manager_cancel(&small_manager, j1->job_instance_id);
    (void)papacc_job_manager_reap(&small_manager, j1->job_instance_id);
    papacc_job_manager_shutdown(&small_manager);
    papacc_job_manager_shutdown(&jm);
    if (jm.initialized != PAPACC_FALSE || tb.destroys != (int)tb.used) return 23;
    return 0;
}
