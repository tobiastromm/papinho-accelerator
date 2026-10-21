#include "../../src/core/ed11_process_profile.h"
#include "server_c8_policy_adapter_win32.h"
#include <windows.h>
#include <string.h>
#define C8_LIMIT PAPACC_ED11_PROCESS_LIMIT
#define C8_PREVIOUS_GRAPH 1551024U
static const PAPACC_CAPABILITY_KEY keys[2] = {{2U,1U},{3U,1U}};
static int owned(const PAPACC_C8_POLICY_ADAPTER *a)
{
    return a != NULL && a->initialized == PAPACC_TRUE &&
        a->manager != NULL && a->manager->initialized == PAPACC_TRUE &&
        a->manager->owner == (PAPACC_U32)GetCurrentThreadId();
}
static PAPACC_C8_POLICY_BINDING *binding(PAPACC_C8_POLICY_ADAPTER *a,
    const PAPACC_C2_SESSION *s)
{
    unsigned i;
    for (i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i)
        if (a->bindings[i].session == s) return &a->bindings[i];
    return NULL;
}
static int principal_bound(PAPACC_C8_POLICY_ADAPTER *a,
    const PAPACC_C8_POLICY_BINDING *b)
{
    unsigned i;
    if (b->connection_security.state != PAPACC_SECURITY_CONTEXT_AUTHORIZED ||
        b->session_security.published != PAPACC_TRUE ||
        !papacc_principal_equal(&b->connection_security.principal,
            &b->session_security.principal)) return 0;
    for (i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i)
        if (a->manager->sessions[i].session == b->session)
            return papacc_principal_equal(&a->manager->sessions[i].principal,
                &b->session_security.principal) == PAPACC_TRUE;
    return 0;
}
static int set_valid(const PAPACC_CAPABILITY_SET *s)
{
    return s!=NULL && s->initialized==PAPACC_TRUE && s->count<=s->capacity &&
        (s->count==0U || s->storage!=NULL);
}
static int configuration_valid(const PAPACC_CAPABILITY_EVALUATION_INPUTS *e)
{
    return e!=NULL && e->registry!=NULL && e->registry->initialized==PAPACC_TRUE &&
        e->registry->count<=e->registry->capacity &&
        (e->registry->count==0U || e->registry->storage!=NULL) &&
        set_valid(e->server_supported) && set_valid(e->server_enabled);
}
static int allowed(PAPACC_C8_POLICY_ADAPTER *a, PAPACC_C8_POLICY_BINDING *b)
{
    unsigned i;
    const PAPACC_CAPABILITY_DESCRIPTOR *descriptor;
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *e=a->effective;
    if (!owned(a) || !a->enabled || a->stopped || a->manager->stopping ||
        b==NULL || b->revoked || b->session==NULL || b->effective_epoch==NULL ||
        b->snapshot==NULL || !configuration_valid(e) ||
        !set_valid(&b->snapshot->effective) || !principal_bound(a,b)) return 0;
    if (*b->effective_epoch==0U || *b->effective_epoch!=b->admitted_epoch ||
        b->session->policy_epoch!=b->admitted_epoch ||
        b->snapshot->session_instance_id!=b->admitted_session ||
        b->session->incarnation!=b->admitted_session ||
        b->session->assurance!=1U ||
        !papacc_c2_session_authorized(b->session,a->manager->owner,
            b->session->incarnation,b->session->principal,b->admitted_epoch)) return 0;
    for (i=0U;i<2U;++i) {
        if (papacc_capability_registry_find(e->registry,&keys[i],&descriptor)!=PAPACC_RESULT_OK ||
            !papacc_capability_set_contains(e->server_supported,&keys[i]) ||
            !papacc_capability_operation_allowed(b->snapshot,&keys[i],
                e->server_enabled,b->contextual_policy,b->policy_context)) return 0;
    }
    return 1;
}
static PAPACC_RESULT policy(void *context,const PAPACC_C2_SESSION *s,
    const PAPACC_U8 *payload,PAPACC_SIZE size,PAPACC_BOOL *permit)
{
    PAPACC_C8_POLICY_ADAPTER *a=context;
    PAPACC_RESULT result;
    PAPACC_BOOL decision=PAPACC_FALSE;
    if (permit==NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *permit=PAPACC_FALSE;
    if (!owned(a) || a->busy || s==NULL) return PAPACC_RESULT_INVALID_STATE;
    a->capacity_session=NULL;
    a->busy=PAPACC_TRUE;
    if (!allowed(a,binding(a,s))) { a->busy=PAPACC_FALSE; return PAPACC_RESULT_OK; }
    result=a->operation_policy(a->operation_context,s,payload,size,&decision);
    /* Recheck after a trusted callback; a callback error never grants. */
    if (result==PAPACC_RESULT_OK && decision==PAPACC_TRUE &&
        allowed(a,binding(a,s))) { *permit=PAPACC_TRUE; a->capacity_session=s; }
    a->busy=PAPACC_FALSE;
    return result;
}
static PAPACC_RESULT capacity(void *context,PAPACC_SIZE bytes,PAPACC_BOOL *permit)
{
    PAPACC_C8_POLICY_ADAPTER *a=context;
    PAPACC_BOOL decision=PAPACC_FALSE;
    PAPACC_RESULT result;
    PAPACC_C8_POLICY_BINDING *b;
    if (permit==NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *permit=PAPACC_FALSE;
    if (!owned(a) || a->busy || !a->enabled || a->stopped ||
        a->reserved_bytes>C8_LIMIT) return PAPACC_RESULT_INVALID_STATE;
    b=binding(a,a->capacity_session);a->capacity_session=NULL;
    a->busy=PAPACC_TRUE;
    if (!allowed(a,b)) { a->busy=PAPACC_FALSE; return PAPACC_RESULT_OK; }
    result=a->capacity(a->operation_context,bytes,&decision);
    if (result==PAPACC_RESULT_OK && decision==PAPACC_TRUE &&
        allowed(a,b)) *permit=PAPACC_TRUE;
    a->busy=PAPACC_FALSE;
    return result;
}
PAPACC_RESULT papacc_c8_policy_adapter_init(PAPACC_C8_POLICY_ADAPTER *a,
    PAPACC_C2_OPERATION_MANAGER *m,const PAPACC_CAPABILITY_EVALUATION_INPUTS *e,
    PAPACC_C2_OPEN_POLICY p,PAPACC_C2_CAPACITY_GATE c,void *context,
    PAPACC_SIZE reservation)
{
    PAPACC_RESULT result;
    if (a==NULL || m==NULL || e==NULL || p==NULL || c==NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (a->initialized || m->initialized) return PAPACC_RESULT_INVALID_STATE;
    if (reservation<C8_PREVIOUS_GRAPH+sizeof(*a) || reservation>C8_LIMIT)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    if (!configuration_valid(e))
        return PAPACC_RESULT_INVALID_STATE;
    result=papacc_c2_operation_manager_init(m,(PAPACC_U32)GetCurrentThreadId(),
        policy,capacity,a);
    if (result!=PAPACC_RESULT_OK) return result;
    memset(a,0,sizeof(*a));a->manager=m;a->effective=e;
    a->operation_policy=p;a->capacity=c;a->operation_context=context;
    a->reserved_bytes=reservation;a->initialized=PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_policy_adapter_bind(PAPACC_C8_POLICY_ADAPTER *a,
    PAPACC_C2_SESSION *s,const PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot,
    const PAPACC_U64 *epoch,const PAPACC_CONNECTION_SECURITY_CONTEXT *connection,
    const PAPACC_SESSION_SECURITY_CONTEXT *security,PAPACC_SERVER_LISTENER_PROFILE profile,
    PAPACC_CAPABILITY_POLICY_FN p,void *context)
{
    PAPACC_C8_POLICY_BINDING candidate;
    unsigned i;
    if (!owned(a) || a->busy || a->stopped || a->manager->stopping)
        return PAPACC_RESULT_INVALID_STATE;
    if (s==NULL || snapshot==NULL || epoch==NULL || connection==NULL ||
        security==NULL || p==NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL ||
        s->owner!=a->manager->owner || s->assurance!=1U ||
        snapshot->published!=PAPACC_TRUE || snapshot->session_instance_id!=s->incarnation ||
        *epoch==0U || s->policy_epoch!=*epoch || binding(a,s)!=NULL)
        return PAPACC_RESULT_INVALID_STATE;
    memset(&candidate,0,sizeof(candidate));candidate.session=s;
    candidate.snapshot=snapshot;candidate.effective_epoch=epoch;
    candidate.admitted_epoch=*epoch;candidate.admitted_session=s->incarnation;
    candidate.connection_security=*connection;
    candidate.session_security=*security;candidate.contextual_policy=p;
    candidate.policy_context=context;
    if (!principal_bound(a,&candidate)) return PAPACC_RESULT_INVALID_STATE;
    for (i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i) if (a->bindings[i].session==NULL) {
        a->bindings[i]=candidate;return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_LIMIT_EXCEEDED;
}
PAPACC_RESULT papacc_c8_policy_adapter_enable(PAPACC_C8_POLICY_ADAPTER *a)
{
    unsigned i;int found=0;
    if (!owned(a) || a->busy || a->stopped || a->manager->stopping)
        return PAPACC_RESULT_INVALID_STATE;
    for (i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i)
        if (a->bindings[i].session!=NULL) found=1;
    if (!found) return PAPACC_RESULT_INVALID_STATE;
    a->enabled=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_policy_adapter_reconcile(PAPACC_C8_POLICY_ADAPTER *a)
{
    unsigned i,j;
    if (!owned(a) || a->busy) return PAPACC_RESULT_INVALID_STATE;
    /* Revocation is sticky. Re-enabling old policy cannot revive an operation.
     * No epoch rewrite, token claim, PST release or native close occurs here. */
    for (i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i) {
        PAPACC_C8_POLICY_BINDING *b=&a->bindings[i];
        if (b->session==NULL) continue;
        a->busy=PAPACC_TRUE;
        j=allowed(a,b)?1U:0U;
        a->busy=PAPACC_FALSE;
        if (a->stopped || !j) {
            b->revoked=PAPACC_TRUE;
            for (j=0U;j<PAPACC_C2_OPERATION_CAPACITY;++j) {
                PAPACC_C2_OPERATION *o=&a->manager->operations[j];
                if (o->session==b->session && o->state!=PAPACC_C2_OP_EMPTY &&
                    o->state!=PAPACC_C2_OP_RELEASED && o->state!=PAPACC_C2_OP_RETAINED && !o->revoked)
                    (void)papacc_c2_operation_revoke(a->manager,o,2U);
            }
        }
    }
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_policy_adapter_stop(PAPACC_C8_POLICY_ADAPTER *a)
{
    if (!owned(a) || a->busy) return PAPACC_RESULT_INVALID_STATE;
    a->stopped=PAPACC_TRUE;a->enabled=PAPACC_FALSE;
    return papacc_c2_operation_stop(a->manager);
}
PAPACC_BOOL papacc_c8_policy_adapter_authorized(PAPACC_C8_POLICY_ADAPTER *a,
    PAPACC_C2_SESSION *s,const PAPACC_PRINCIPAL *p,PAPACC_U64 id,PAPACC_U64 generation,
    unsigned role,PAPACC_U64 channel)
{
    PAPACC_C8_POLICY_BINDING *b;
    unsigned i;
    if (!owned(a) || a->busy || s==NULL || p==NULL || id==0U || generation==0U ||
        role<1U || role>2U || channel==0U) return PAPACC_FALSE;
    b=binding(a,s);
    if (b==NULL || !papacc_principal_equal(p,&b->session_security.principal))
        return PAPACC_FALSE;
    for (i=0U;i<PAPACC_C2_OPERATION_CAPACITY;++i) {
        PAPACC_C2_OPERATION *o=&a->manager->operations[i];
        if (o->session!=s || o->id!=id || o->generation!=generation) continue;
        if (o->state==PAPACC_C2_OP_EMPTY || o->state==PAPACC_C2_OP_RETAINED ||
            o->state==PAPACC_C2_OP_TERMINATING || o->state==PAPACC_C2_OP_RELEASED ||
            !o->tokens.grant_written || !o->tokens.claimed[role-1U] ||
            !o->tokens.ack_written[role-1U] || o->tokens.channels[role-1U]!=channel)
            return PAPACC_FALSE;
        return papacc_c2_operation_authorized(a->manager,o);
    }
    return PAPACC_FALSE;
}
