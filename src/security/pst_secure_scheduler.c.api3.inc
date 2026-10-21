#include "pst_secure_scheduler.h"
#include <stdio.h>
#include <stdlib.h>

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
    scheduler->wait_sets[0] = scheduler->wait_set;
    scheduler->wait_set_count = 1U;
    scheduler->active_wait_set = scheduler->wait_set;
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
    result = pst_wait_set_add_connection(scheduler->active_wait_set, connection,
        token);
    if (result != PST_RESULT_OK) return papacc_pst_scheduler_map_result(result);
    member->registered = PAPACC_TRUE;
    member->kind = PAPACC_PST_SCHEDULER_MEMBER_CONNECTION;
    member->token = token;
    member->source.connection = connection;
    member->membership_wait_set = scheduler->active_wait_set;
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
    result = pst_wait_set_add_external_source(scheduler->active_wait_set,
        external_source, interest, token);
    if (result != PST_RESULT_OK) return papacc_pst_scheduler_map_result(result);
    member->registered = PAPACC_TRUE;
    member->kind = PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL;
    member->token = token;
    member->source.external_source = external_source;
    member->membership_wait_set = scheduler->active_wait_set;
    member->external_interest = interest;
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
            result = pst_wait_set_remove_connection(member->membership_wait_set,
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
            result = pst_wait_set_remove_external_source(member->membership_wait_set,
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
    if (scheduler->partition_failed) return PAPACC_RESULT_INVALID_STATE;
    result = pst_wait_set_wait(scheduler->active_wait_set, timeout_ms,
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
        if (!member->registered || member->productive_suspended) continue;
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
    {
        PAPACC_SIZE index;
        for(index=0U;index<scheduler->wait_set_count;++index){
            result=pst_wait_set_wake(scheduler->wait_sets[index]);
            if(result!=PST_RESULT_OK&&result!=PST_RESULT_WAIT_WOKEN)
                return papacc_pst_scheduler_map_result(result);
        }
    }
    return PAPACC_RESULT_OK;
}

void papacc_pst_secure_scheduler_release(
    PAPACC_PST_SECURE_SCHEDULER *scheduler)
{
    PAPACC_SIZE index;
    if (scheduler == NULL || scheduler->initialized == PAPACC_FALSE) return;
    if (scheduler->partition_failed) return;
    for (index = 0U; index < scheduler->member_capacity; ++index) {
        PAPACC_PST_SCHEDULER_MEMBER *member = &scheduler->members[index];
        if (!member->registered) continue;
        if (member->kind == PAPACC_PST_SCHEDULER_MEMBER_CONNECTION)
            {if(pst_wait_set_remove_connection(member->membership_wait_set,
                member->source.connection)!=PST_RESULT_OK)return;}
        else if (member->kind == PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL)
            {if(pst_wait_set_remove_external_source(member->membership_wait_set,
                member->source.external_source)!=PST_RESULT_OK)return;}
        *member = (PAPACC_PST_SCHEDULER_MEMBER)
            PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER;
    }
    if(scheduler->partition_failed)return;
    for(index=0U;index<scheduler->wait_set_count;++index){
        if(scheduler->wait_sets[index]==NULL)continue;
        if(pst_wait_set_destroy(scheduler->wait_sets[index])!=PST_RESULT_OK)return;
        scheduler->wait_sets[index]=NULL;
    }
    *scheduler = (PAPACC_PST_SECURE_SCHEDULER)
        PAPACC_PST_SECURE_SCHEDULER_INITIALIZER;
}

PAPACC_RESULT papacc_pst_secure_scheduler_enable_partition(PAPACC_PST_SECURE_SCHEDULER*s){
 PAPACC_SIZE i;PST_RESULT r;
 if(!s||!s->initialized||s->stopping)return PAPACC_RESULT_INVALID_STATE;
 if(s->partition_enabled)return PAPACC_RESULT_OK;
 for(i=1U;i<3U;++i){r=pst_wait_set_create(&s->wait_sets[i]);if(r!=PST_RESULT_OK){
  while(i>1U){--i;if(pst_wait_set_destroy(s->wait_sets[i])!=PST_RESULT_OK){s->partition_failed=PAPACC_TRUE;return PAPACC_RESULT_INVALID_STATE;}s->wait_sets[i]=NULL;--s->wait_set_count;}return papacc_pst_scheduler_map_result(r);}
  ++s->wait_set_count;}
 s->wait_set_count=3U;s->partition_enabled=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_pst_secure_scheduler_suspend_complete(PAPACC_PST_SECURE_SCHEDULER*s,pst_connection*c){
 PAPACC_SIZE i,j;PAPACC_PST_SCHEDULER_MEMBER*target=NULL;pst_wait_set*next=NULL;PST_RESULT r,rollback;const char*trace;
 if(!s||!s->initialized||!s->partition_enabled||s->partition_failed||s->stopping)return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<s->member_capacity;++i)if(s->members[i].registered&&s->members[i].kind==PAPACC_PST_SCHEDULER_MEMBER_CONNECTION&&s->members[i].source.connection==c){target=&s->members[i];break;}
 if(!target)return PAPACC_RESULT_INVALID_STATE;if(target->productive_suspended)return PAPACC_RESULT_OK;
 for(i=0;i<s->wait_set_count;++i){PAPACC_BOOL empty=PAPACC_TRUE;
  for(j=0;j<s->member_capacity;++j)if(s->members[j].registered&&s->members[j].membership_wait_set==s->wait_sets[i])empty=PAPACC_FALSE;
  if(empty){next=s->wait_sets[i];break;}}
 if(!next)return PAPACC_RESULT_LIMIT_EXCEEDED;
 /* Mark the factual completed child; NEVER move/remove/release this member. */
 target->productive_suspended=PAPACC_TRUE;
 for(i=0;i<s->member_capacity;++i){PAPACC_PST_SCHEDULER_MEMBER*m=&s->members[i];pst_wait_set*old;
  if(!m->registered||m->productive_suspended)continue;old=m->membership_wait_set;
  r=m->kind==PAPACC_PST_SCHEDULER_MEMBER_CONNECTION?pst_wait_set_remove_connection(old,m->source.connection):pst_wait_set_remove_external_source(old,m->source.external_source);
  if(r!=PST_RESULT_OK){s->partition_failed=PAPACC_TRUE;return papacc_pst_scheduler_map_result(r);}
  r=m->kind==PAPACC_PST_SCHEDULER_MEMBER_CONNECTION?pst_wait_set_add_connection(next,m->source.connection,m->token):pst_wait_set_add_external_source(next,m->source.external_source,m->external_interest,m->token);
  if(r!=PST_RESULT_OK){
   rollback=m->kind==PAPACC_PST_SCHEDULER_MEMBER_CONNECTION?pst_wait_set_add_connection(old,m->source.connection,m->token):pst_wait_set_add_external_source(old,m->source.external_source,m->external_interest,m->token);
   if(rollback!=PST_RESULT_OK)m->membership_wait_set=NULL;s->partition_failed=PAPACC_TRUE;return papacc_pst_scheduler_map_result(r);
  }
  m->membership_wait_set=next;
 }
 s->active_wait_set=next;
#if defined(_MSC_VER)
 {size_t needed=0U;char value[2]={0,0};trace=NULL;
  if(getenv_s(&needed,NULL,0U,"PAPACC_C7_METRICS")==0&&needed==2U&&
     getenv_s(&needed,value,sizeof(value),"PAPACC_C7_METRICS")==0&&value[0]=='1')trace="1";}
#else
 trace=getenv("PAPACC_C7_METRICS");
#endif
 if(trace&&trace[0]=='1'&&trace[1]==0)
  fprintf(stderr,"ADR0037_SCHEDULING_SUSPENDED wait_membership=%llu child=%p original_wait_set=%p active_wait_set=%p child_membership_preserved=1 removal_attempt=0 release_attempt=0 productive_retry=0\n",(unsigned long long)target->token,(void*)c,(void*)target->membership_wait_set,(void*)next);
 return PAPACC_RESULT_OK;
}
