#include "job.h"

static PAPACC_BOOL manager_valid(const PAPACC_JOB_MANAGER *m)
{
    return (m != NULL && m->initialized == PAPACC_TRUE &&
        m->count <= m->capacity && (m->capacity == 0U || m->storage != NULL) &&
        m->session_manager != NULL &&
        m->session_manager->initialized == PAPACC_TRUE) ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_BOOL papacc_job_state_terminal(PAPACC_JOB_STATE state)
{
    return (state == PAPACC_JOB_STATE_COMPLETED ||
        state == PAPACC_JOB_STATE_FAILED ||
        state == PAPACC_JOB_STATE_CANCELLED) ? PAPACC_TRUE : PAPACC_FALSE;
}

static void destroy_execution(PAPACC_JOB *job)
{
    if (job->backend_execution != NULL && job->backend != NULL &&
        job->backend->destroy != NULL) {
        job->backend->destroy(job->backend->context, job->backend_execution);
    }
    job->backend_execution = NULL;
}

static void make_terminal(PAPACC_JOB *job, PAPACC_JOB_STATE state,
    PAPACC_JOB_TERMINAL_REASON reason)
{
    if (papacc_job_state_terminal(job->state) == PAPACC_TRUE) return;
    job->state = state;
    job->terminal_reason = reason;
    destroy_execution(job);
}

PAPACC_RESULT papacc_job_manager_init(PAPACC_JOB_MANAGER *m, PAPACC_JOB *storage,
    PAPACC_SIZE capacity, PAPACC_SIZE per_session_limit,
    PAPACC_SESSION_MANAGER *session_manager)
{
    PAPACC_SIZE i;
    if (m == NULL || session_manager == NULL ||
        session_manager->initialized != PAPACC_TRUE ||
        (capacity > 0U && storage == NULL) ||
        (capacity > 0U && per_session_limit == 0U))
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (m->initialized == PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    for (i = 0U; i < capacity; ++i)
        storage[i] = (PAPACC_JOB)PAPACC_JOB_INITIALIZER;
    m->storage = storage; m->capacity = capacity; m->count = 0U;
    m->per_session_limit = per_session_limit; m->cursor = 0U;
    m->last_session_id = 0U; m->next_instance_id = 1U;
    m->session_manager = session_manager; m->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_JOB *papacc_job_manager_find(PAPACC_JOB_MANAGER *m, PAPACC_U64 id)
{
    PAPACC_SIZE i;
    if (manager_valid(m) != PAPACC_TRUE || id == 0U) return NULL;
    for (i = 0U; i < m->capacity; ++i)
        if (m->storage[i].state != PAPACC_JOB_STATE_UNUSED &&
            m->storage[i].job_instance_id == id) return &m->storage[i];
    return NULL;
}

PAPACC_JOB *papacc_job_manager_find_for_session(PAPACC_JOB_MANAGER *m,
    PAPACC_U64 session_id, PAPACC_U64 job_id)
{
    PAPACC_JOB *job = papacc_job_manager_find(m, job_id);
    return (job != NULL && session_id != 0U &&
        job->owner_session_instance_id == session_id) ? job : NULL;
}

const PAPACC_JOB *papacc_job_manager_inspect_at(const PAPACC_JOB_MANAGER *m,
    PAPACC_SIZE index)
{
    if (manager_valid(m) != PAPACC_TRUE || index >= m->capacity ||
        m->storage[index].state == PAPACC_JOB_STATE_UNUSED) return NULL;
    return &m->storage[index];
}

static PAPACC_RESULT next_job_id(PAPACC_JOB_MANAGER *m, PAPACC_U64 *out_id)
{
    PAPACC_U64 candidate = m->next_instance_id;
    PAPACC_SIZE attempts = 0U;
    if (candidate == 0U) candidate = 1U;
    while (papacc_job_manager_find(m, candidate) != NULL) {
        if (++attempts > m->count) return PAPACC_RESULT_LIMIT_EXCEEDED;
        if (++candidate == 0U) candidate = 1U;
    }
    *out_id = candidate;
    if (++candidate == 0U) candidate = 1U;
    m->next_instance_id = candidate;
    return PAPACC_RESULT_OK;
}

static PAPACC_SIZE session_job_count(PAPACC_JOB_MANAGER *m, PAPACC_U64 id)
{
    PAPACC_SIZE i, count = 0U;
    for (i = 0U; i < m->capacity; ++i)
        if (m->storage[i].state != PAPACC_JOB_STATE_UNUSED &&
            m->storage[i].owner_session_instance_id == id) ++count;
    return count;
}

static PAPACC_BOOL backend_valid(const PAPACC_COMPUTE_BACKEND *b)
{
    return (b != NULL && b->prepare != NULL && b->start != NULL &&
        b->step != NULL && b->request_cancel != NULL && b->destroy != NULL)
        ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_job_manager_admit(PAPACC_JOB_MANAGER *m,
    const PAPACC_JOB_ADMISSION *a, PAPACC_JOB **out_job)
{
    PAPACC_SESSION *session;
    PAPACC_JOB *slot = NULL;
    void *execution = NULL;
    PAPACC_SIZE i;
    PAPACC_RESULT result;
    PAPACC_U64 job_id;
    if (out_job != NULL) *out_job = NULL;
    if (manager_valid(m) != PAPACC_TRUE || a == NULL || out_job == NULL ||
        a->session_instance_id == 0U || a->deadline_ns == 0U ||
        a->deadline_ns <= a->now_ns || backend_valid(a->backend) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    session = papacc_session_manager_find(m->session_manager,
        a->session_instance_id);
    if (session == NULL || session->state != PAPACC_SESSION_STATE_ACTIVE)
        return PAPACC_RESULT_INVALID_STATE;
    if (a->snapshot == NULL || a->snapshot->published != PAPACC_TRUE ||
        a->snapshot->session_instance_id != a->session_instance_id)
        return PAPACC_RESULT_INVALID_STATE;
    if (papacc_capability_operation_allowed(a->snapshot,
        a->required_capability, a->server_enabled_now,
        a->contextual_policy, a->contextual_policy_context) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (m->count >= m->capacity ||
        session_job_count(m, a->session_instance_id) >= m->per_session_limit)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    for (i = 0U; i < m->capacity; ++i)
        if (m->storage[i].state == PAPACC_JOB_STATE_UNUSED) { slot = &m->storage[i]; break; }
    if (slot == NULL) return PAPACC_RESULT_INVALID_STATE;
    result = a->backend->prepare(a->backend->context, a->workload_context,
        &execution);
    if (result != PAPACC_RESULT_OK || execution == NULL) {
        if (execution != NULL) a->backend->destroy(a->backend->context, execution);
        return (result == PAPACC_RESULT_OK) ? PAPACC_RESULT_INVALID_STATE : result;
    }
    result = next_job_id(m, &job_id);
    if (result != PAPACC_RESULT_OK) {
        a->backend->destroy(a->backend->context, execution);
        return result;
    }
    slot->job_instance_id = job_id;
    slot->owner_session_instance_id = a->session_instance_id;
    slot->required_capability = *a->required_capability;
    slot->state = PAPACC_JOB_STATE_QUEUED;
    slot->created_at_ns = a->now_ns; slot->deadline_ns = a->deadline_ns;
    slot->backend = a->backend; slot->backend_execution = execution;
    ++m->count; *out_job = slot;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_job_manager_cancel(PAPACC_JOB_MANAGER *m, PAPACC_U64 id)
{
    PAPACC_JOB *job = papacc_job_manager_find(m, id);
    PAPACC_RESULT result;
    if (job == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_job_state_terminal(job->state) == PAPACC_TRUE) return PAPACC_RESULT_OK;
    job->cancel_requested = PAPACC_TRUE;
    if (job->state == PAPACC_JOB_STATE_QUEUED) {
        make_terminal(job, PAPACC_JOB_STATE_CANCELLED, PAPACC_JOB_REASON_CANCELLED);
        return PAPACC_RESULT_OK;
    }
    result = job->backend->request_cancel(job->backend->context,
        job->backend_execution);
    return result;
}

PAPACC_RESULT papacc_job_manager_cancel_session(PAPACC_JOB_MANAGER *m,
    PAPACC_U64 session_id)
{
    PAPACC_SIZE i;
    if (manager_valid(m) != PAPACC_TRUE || session_id == 0U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    for (i = 0U; i < m->capacity; ++i) {
        PAPACC_JOB *job = &m->storage[i];
        if (job->state != PAPACC_JOB_STATE_UNUSED &&
            job->owner_session_instance_id == session_id &&
            papacc_job_state_terminal(job->state) != PAPACC_TRUE) {
            job->suppress_result = PAPACC_TRUE;
            if (job->state == PAPACC_JOB_STATE_QUEUED)
                make_terminal(job, PAPACC_JOB_STATE_CANCELLED,
                    PAPACC_JOB_REASON_SESSION_LOST);
            else {
                job->cancel_requested = PAPACC_TRUE;
                (void)job->backend->request_cancel(job->backend->context,
                    job->backend_execution);
            }
        }
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_JOB *select_runnable(PAPACC_JOB_MANAGER *m)
{
    PAPACC_SIZE pass, index;
    PAPACC_JOB *fallback = NULL;
    for (pass = 1U; pass <= m->capacity; ++pass) {
        index = (m->cursor + pass) % m->capacity;
        if (m->storage[index].state == PAPACC_JOB_STATE_QUEUED ||
            m->storage[index].state == PAPACC_JOB_STATE_RUNNING) {
            if (fallback == NULL) fallback = &m->storage[index];
            if (m->storage[index].owner_session_instance_id != m->last_session_id) {
                m->cursor = index; return &m->storage[index];
            }
        }
    }
    if (fallback != NULL) m->cursor = (PAPACC_SIZE)(fallback - m->storage);
    return fallback;
}

static void apply_step(PAPACC_JOB *job, PAPACC_COMPUTE_STEP_STATUS status)
{
    if (status == PAPACC_COMPUTE_STEP_PROGRESS) ++job->progress_count;
    else if (status == PAPACC_COMPUTE_STEP_COMPLETED)
        make_terminal(job, PAPACC_JOB_STATE_COMPLETED, PAPACC_JOB_REASON_SUCCESS);
    else if (status == PAPACC_COMPUTE_STEP_FAILED)
        make_terminal(job, PAPACC_JOB_STATE_FAILED, PAPACC_JOB_REASON_BACKEND_FAILURE);
    else if (status == PAPACC_COMPUTE_STEP_CANCELLED)
        make_terminal(job, PAPACC_JOB_STATE_CANCELLED,
            job->suppress_result ? PAPACC_JOB_REASON_SESSION_LOST :
            PAPACC_JOB_REASON_CANCELLED);
}

static PAPACC_BOOL step_status_valid(PAPACC_COMPUTE_STEP_STATUS status)
{
    return (status == PAPACC_COMPUTE_STEP_PROGRESS ||
        status == PAPACC_COMPUTE_STEP_WOULD_BLOCK ||
        status == PAPACC_COMPUTE_STEP_COMPLETED ||
        status == PAPACC_COMPUTE_STEP_FAILED ||
        status == PAPACC_COMPUTE_STEP_CANCELLED) ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_job_manager_progress_one(PAPACC_JOB_MANAGER *m,
    PAPACC_U64 now_ns, PAPACC_JOB **out_job)
{
    PAPACC_JOB *job;
    PAPACC_COMPUTE_STEP_STATUS status = PAPACC_COMPUTE_STEP_WOULD_BLOCK;
    PAPACC_RESULT result;
    PAPACC_BOOL deadline_expired = PAPACC_FALSE;
    if (out_job != NULL) *out_job = NULL;
    if (manager_valid(m) != PAPACC_TRUE || out_job == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    job = select_runnable(m);
    if (job == NULL) return PAPACC_RESULT_OK;
    *out_job = job; m->last_session_id = job->owner_session_instance_id;
    if (now_ns >= job->deadline_ns) {
        deadline_expired = PAPACC_TRUE;
        job->cancel_requested = PAPACC_TRUE;
        if (job->state == PAPACC_JOB_STATE_QUEUED) {
            make_terminal(job, PAPACC_JOB_STATE_CANCELLED, PAPACC_JOB_REASON_DEADLINE);
            return PAPACC_RESULT_OK;
        }
        (void)job->backend->request_cancel(job->backend->context,
            job->backend_execution);
    }
    if (job->state == PAPACC_JOB_STATE_QUEUED) {
        job->state = PAPACC_JOB_STATE_RUNNING; job->started_at_ns = now_ns;
        result = job->backend->start(job->backend->context,
            job->backend_execution, &status);
    } else {
        result = job->backend->step(job->backend->context,
            job->backend_execution, &status);
    }
    if (result != PAPACC_RESULT_OK) {
        make_terminal(job, PAPACC_JOB_STATE_FAILED, PAPACC_JOB_REASON_BACKEND_FAILURE);
        return result;
    }
    if (step_status_valid(status) != PAPACC_TRUE) {
        make_terminal(job, PAPACC_JOB_STATE_FAILED,
            PAPACC_JOB_REASON_BACKEND_FAILURE);
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (deadline_expired == PAPACC_TRUE) {
        make_terminal(job, PAPACC_JOB_STATE_CANCELLED,
            PAPACC_JOB_REASON_DEADLINE);
        return PAPACC_RESULT_OK;
    }
    apply_step(job, status);
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_job_manager_reap(PAPACC_JOB_MANAGER *m, PAPACC_U64 id)
{
    PAPACC_JOB *job = papacc_job_manager_find(m, id);
    if (job == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_job_state_terminal(job->state) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    destroy_execution(job); *job = (PAPACC_JOB)PAPACC_JOB_INITIALIZER; --m->count;
    return PAPACC_RESULT_OK;
}

void papacc_job_manager_shutdown(PAPACC_JOB_MANAGER *m)
{
    PAPACC_SIZE i;
    if (m == NULL || m->initialized != PAPACC_TRUE) return;
    for (i = 0U; i < m->capacity; ++i) {
        PAPACC_JOB *job = &m->storage[i];
        if (job->state != PAPACC_JOB_STATE_UNUSED) {
            if (papacc_job_state_terminal(job->state) != PAPACC_TRUE &&
                job->backend_execution != NULL)
                (void)job->backend->request_cancel(job->backend->context,
                    job->backend_execution);
            destroy_execution(job); *job = (PAPACC_JOB)PAPACC_JOB_INITIALIZER;
        }
    }
    *m = (PAPACC_JOB_MANAGER)PAPACC_JOB_MANAGER_INITIALIZER;
}
