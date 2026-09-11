#include "pst_secure_scheduler.h"

static PAPACC_RESULT papacc_pst_scheduler_map_result(PST_RESULT result)
{
    switch (result) {
    case PST_RESULT_OK: return PAPACC_RESULT_OK;
    case PST_RESULT_INVALID_ARGUMENT: return PAPACC_RESULT_INVALID_ARGUMENT;
    case PST_RESULT_INVALID_STATE: return PAPACC_RESULT_INVALID_STATE;
    case PST_RESULT_UNSUPPORTED: return PAPACC_RESULT_NOT_SUPPORTED;
    case PST_RESULT_OUT_OF_MEMORY: return PAPACC_RESULT_OUT_OF_MEMORY;
    case PST_RESULT_INSUFFICIENT_CAPACITY:
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    case PST_RESULT_ALREADY_REGISTERED:
    case PST_RESULT_NOT_REGISTERED:
    case PST_RESULT_CONCURRENT_OPERATION:
        return PAPACC_RESULT_INVALID_STATE;
    default: return PAPACC_RESULT_INTERNAL_ERROR;
    }
}

static PAPACC_PST_SCHEDULER_MEMBER *papacc_pst_scheduler_free_member(
    PAPACC_PST_SECURE_SCHEDULER *scheduler)
{
    PAPACC_SIZE index;
    for (index = 0U; index < scheduler->member_capacity; ++index) {
        if (scheduler->members[index].registered == PAPACC_FALSE)
            return &scheduler->members[index];
    }
    return NULL;
}

static pst_wait_token papacc_pst_scheduler_next_token(
    PAPACC_PST_SECURE_SCHEDULER *scheduler)
{
    pst_wait_token token = scheduler->next_token++;
    if (scheduler->next_token == 0U) scheduler->next_token = 1U;
    return token;
}

static void papacc_pst_scheduler_clear_member(
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    PAPACC_PST_SCHEDULER_MEMBER *member)
{
    *member = (PAPACC_PST_SCHEDULER_MEMBER)
        PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    --scheduler->member_count;
}

PAPACC_RESULT papacc_pst_secure_scheduler_init(
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    PAPACC_PST_SCHEDULER_MEMBER *members, PAPACC_SIZE member_capacity,
    PST_WAIT_EVENT *events, PAPACC_SIZE event_capacity)
{
    PAPACC_SIZE index;
    PST_RESULT result;
    if (scheduler == NULL || members == NULL || member_capacity == 0U ||
        events == NULL || event_capacity < member_capacity)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = pst_wait_set_create(&scheduler->wait_set);
    if (result != PST_RESULT_OK) return papacc_pst_scheduler_map_result(result);
    for (index = 0U; index < member_capacity; ++index)
        members[index] = (PAPACC_PST_SCHEDULER_MEMBER)
            PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    scheduler->members = members;
    scheduler->member_capacity = member_capacity;
    scheduler->events = events;
    scheduler->event_capacity = event_capacity;
    scheduler->next_token = 1U;
    scheduler->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_scheduler_add_connection(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, pst_connection *connection,
    pst_wait_token *out_token)
{
    PAPACC_PST_SCHEDULER_MEMBER *member;
    pst_wait_token token;
    PST_RESULT result;
    if (scheduler == NULL || connection == NULL || out_token == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE || scheduler->stopping)
        return PAPACC_RESULT_INVALID_STATE;
    member = papacc_pst_scheduler_free_member(scheduler);
    if (member == NULL) return PAPACC_RESULT_LIMIT_EXCEEDED;
    token = papacc_pst_scheduler_next_token(scheduler);
    result = pst_wait_set_add_connection(scheduler->wait_set, connection,
        token);
    if (result != PST_RESULT_OK) return papacc_pst_scheduler_map_result(result);
    member->registered = PAPACC_TRUE;
    member->kind = PAPACC_PST_SCHEDULER_MEMBER_CONNECTION;
    member->token = token;
    member->source.connection = connection;
    ++scheduler->member_count;
    *out_token = token;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_scheduler_add_external(
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    pst_external_source *external_source, PAPACC_BOOL read_interest,
    PAPACC_BOOL write_interest, pst_wait_token *out_token)
{
    PAPACC_PST_SCHEDULER_MEMBER *member;
    pst_wait_token token;
    pst_u32 interest = PST_INTEREST_NONE;
    PST_RESULT result;
    if (scheduler == NULL || external_source == NULL || out_token == NULL ||
        (read_interest == PAPACC_FALSE && write_interest == PAPACC_FALSE))
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE || scheduler->stopping)
        return PAPACC_RESULT_INVALID_STATE;
    member = papacc_pst_scheduler_free_member(scheduler);
    if (member == NULL) return PAPACC_RESULT_LIMIT_EXCEEDED;
    if (read_interest) interest |= PST_INTEREST_READ;
    if (write_interest) interest |= PST_INTEREST_WRITE;
    token = papacc_pst_scheduler_next_token(scheduler);
    result = pst_wait_set_add_external_source(scheduler->wait_set,
        external_source, interest, token);
    if (result != PST_RESULT_OK) return papacc_pst_scheduler_map_result(result);
    member->registered = PAPACC_TRUE;
    member->kind = PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL;
    member->token = token;
    member->source.external_source = external_source;
    ++scheduler->member_count;
    *out_token = token;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_scheduler_remove_connection(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, pst_connection *connection)
{
    PAPACC_SIZE index;
    PST_RESULT result;
    if (scheduler == NULL || connection == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE)
        return PAPACC_RESULT_INVALID_STATE;
    for (index = 0U; index < scheduler->member_capacity; ++index) {
        PAPACC_PST_SCHEDULER_MEMBER *member = &scheduler->members[index];
        if (member->registered &&
            member->kind == PAPACC_PST_SCHEDULER_MEMBER_CONNECTION &&
            member->source.connection == connection) {
            result = pst_wait_set_remove_connection(scheduler->wait_set,
                connection);
            if (result != PST_RESULT_OK)
                return papacc_pst_scheduler_map_result(result);
            papacc_pst_scheduler_clear_member(scheduler, member);
            return PAPACC_RESULT_OK;
        }
    }
    return PAPACC_RESULT_INVALID_STATE;
}

PAPACC_RESULT papacc_pst_secure_scheduler_remove_external(
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    pst_external_source *external_source)
{
    PAPACC_SIZE index;
    PST_RESULT result;
    if (scheduler == NULL || external_source == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE)
        return PAPACC_RESULT_INVALID_STATE;
    for (index = 0U; index < scheduler->member_capacity; ++index) {
        PAPACC_PST_SCHEDULER_MEMBER *member = &scheduler->members[index];
        if (member->registered &&
            member->kind == PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL &&
            member->source.external_source == external_source) {
            result = pst_wait_set_remove_external_source(scheduler->wait_set,
                external_source);
            if (result != PST_RESULT_OK)
                return papacc_pst_scheduler_map_result(result);
            papacc_pst_scheduler_clear_member(scheduler, member);
            return PAPACC_RESULT_OK;
        }
    }
    return PAPACC_RESULT_INVALID_STATE;
}

PAPACC_RESULT papacc_pst_secure_scheduler_wait(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, PAPACC_U32 timeout_ms,
    PAPACC_PST_WAIT_OUTCOME *outcome, PAPACC_SIZE *out_ready_count)
{
    PST_WAIT_SET_RESULT wait_result;
    PST_RESULT result;
    if (scheduler == NULL || outcome == NULL || out_ready_count == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE)
        return PAPACC_RESULT_INVALID_STATE;
    scheduler->event_count = 0U;
    wait_result.ready_count = 0U;
    wait_result.event_count = 0U;
    wait_result.timed_out = 0U;
    wait_result.woken = 0U;
    result = pst_wait_set_wait(scheduler->wait_set, timeout_ms,
        scheduler->events, scheduler->event_capacity, &wait_result);
    if (result == PST_RESULT_WAIT_TIMEOUT || wait_result.timed_out != 0U) {
        *outcome = PAPACC_PST_WAIT_TIMEOUT;
        *out_ready_count = 0U;
        return PAPACC_RESULT_OK;
    }
    if (result == PST_RESULT_WAIT_WOKEN || wait_result.woken != 0U) {
        *outcome = PAPACC_PST_WAIT_WOKEN;
        *out_ready_count = 0U;
        return PAPACC_RESULT_OK;
    }
    if (result != PST_RESULT_OK) return papacc_pst_scheduler_map_result(result);
    if (wait_result.event_count > scheduler->event_capacity)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    scheduler->event_count = wait_result.event_count;
    *outcome = PAPACC_PST_WAIT_READY;
    *out_ready_count = wait_result.ready_count;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_scheduler_dispatch_ready(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, PAPACC_PST_READY_FN callback,
    void *callback_context, PAPACC_SIZE *out_dispatched)
{
    PAPACC_SIZE offset;
    PAPACC_SIZE dispatched = 0U;
    if (scheduler == NULL || callback == NULL || out_dispatched == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE)
        return PAPACC_RESULT_INVALID_STATE;
    if (scheduler->stopping) {
        *out_dispatched = 0U;
        return PAPACC_RESULT_OK;
    }
    for (offset = 0U; offset < scheduler->member_capacity; ++offset) {
        PAPACC_SIZE member_index =
            (scheduler->next_member_index + offset) %
                scheduler->member_capacity;
        PAPACC_PST_SCHEDULER_MEMBER *member =
            &scheduler->members[member_index];
        PAPACC_SIZE event_index;
        if (!member->registered) continue;
        for (event_index = 0U; event_index < scheduler->event_count;
             ++event_index) {
            PST_WAIT_EVENT *source = &scheduler->events[event_index];
            PAPACC_PST_READY_EVENT event;
            if (source->token != member->token) continue;
            event.token = source->token;
            event.kind = member->kind;
            event.read_ready =
                (source->ready_interest & PST_INTEREST_READ) != 0U ?
                PAPACC_TRUE : PAPACC_FALSE;
            event.write_ready =
                (source->ready_interest & PST_INTEREST_WRITE) != 0U ?
                PAPACC_TRUE : PAPACC_FALSE;
            event.terminal =
                (source->flags & PST_WAIT_READY_TERMINAL) != 0U ?
                PAPACC_TRUE : PAPACC_FALSE;
            event.external =
                (source->flags & PST_WAIT_READY_EXTERNAL) != 0U ?
                PAPACC_TRUE : PAPACC_FALSE;
            event.source_result = source->result;
            callback(callback_context, &event);
            ++dispatched;
            break;
        }
    }
    if (scheduler->member_capacity != 0U)
        scheduler->next_member_index =
            (scheduler->next_member_index + 1U) % scheduler->member_capacity;
    *out_dispatched = dispatched;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_scheduler_request_stop(
    PAPACC_PST_SECURE_SCHEDULER *scheduler)
{
    if (scheduler == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE)
        return PAPACC_RESULT_INVALID_STATE;
    scheduler->stopping = PAPACC_TRUE;
    return papacc_pst_secure_scheduler_wake(scheduler);
}

PAPACC_RESULT papacc_pst_secure_scheduler_wake(
    PAPACC_PST_SECURE_SCHEDULER *scheduler)
{
    PST_RESULT result;
    if (scheduler == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (scheduler->initialized == PAPACC_FALSE)
        return PAPACC_RESULT_INVALID_STATE;
    result = pst_wait_set_wake(scheduler->wait_set);
    if (result == PST_RESULT_OK || result == PST_RESULT_WAIT_WOKEN)
        return PAPACC_RESULT_OK;
    return papacc_pst_scheduler_map_result(result);
}

void papacc_pst_secure_scheduler_release(
    PAPACC_PST_SECURE_SCHEDULER *scheduler)
{
    PAPACC_SIZE index;
    if (scheduler == NULL || scheduler->initialized == PAPACC_FALSE) return;
    for (index = 0U; index < scheduler->member_capacity; ++index) {
        PAPACC_PST_SCHEDULER_MEMBER *member = &scheduler->members[index];
        if (!member->registered) continue;
        if (member->kind == PAPACC_PST_SCHEDULER_MEMBER_CONNECTION)
            (void)pst_wait_set_remove_connection(scheduler->wait_set,
                member->source.connection);
        else if (member->kind == PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL)
            (void)pst_wait_set_remove_external_source(scheduler->wait_set,
                member->source.external_source);
        *member = (PAPACC_PST_SCHEDULER_MEMBER)
            PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    }
    (void)pst_wait_set_destroy(scheduler->wait_set);
    *scheduler = (PAPACC_PST_SECURE_SCHEDULER)
        PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
}
