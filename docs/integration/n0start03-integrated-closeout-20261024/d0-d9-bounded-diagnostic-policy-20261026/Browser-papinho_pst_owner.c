#include "papinho_pst_owner.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* Browser-private API3 owner. Production API2 never compiles this unit. */
static int owner_finish_dependencies(papinho_pst_owner *o) {
    if (o->winsock_owned) {
        if (o->winsock_cleanup_attempted) return 0;
        o->winsock_cleanup_attempted = 1;
        o->winsock_cleanup_result = WSACleanup();
        if (o->winsock_cleanup_result != 0) {
            o->winsock_cleanup_error = WSAGetLastError();
            o->terminal_retained = 1; /* Browser retention, not a PST disposition */
            o->registry->admission_closed = 1;
            return 0;
        }
        o->winsock_owned = 0;
    }
    /* D's callback only drops this owner's group lease; it does not destroy
     * the group or its resources. Keep that lease on WinSock cleanup failure. */
    if (o->dependencies.finalize)
        o->dependencies.finalize(o->dependencies.context);
    ZeroMemory(&o->dependencies, sizeof(o->dependencies));
    o->finalized = 1;
    ++o->finalizations;
    return 1;
}
void papinho_pst_owner_registry_init(papinho_pst_owner_registry *r) {
    ZeroMemory(r, sizeof(*r));
    InitializeCriticalSection(&r->lock);
}

papinho_pst_owner *papinho_pst_owner_create(papinho_pst_owner_registry *r,
    pst_connection *connection, unsigned long identity, unsigned long generation,
    unsigned long role, const papinho_pst_owner_dependencies *dependencies,
    const papinho_pst_owner_api *api) {
    papinho_pst_owner *o, *p;
    WSADATA data;
    if (!r || !identity || !generation || !dependencies)
        return NULL;
    EnterCriticalSection(&r->lock);
    if (r->admission_closed) { LeaveCriticalSection(&r->lock); return NULL; }
    for (p = r->head; p; p = p->next) {
        /* Tombstones prevent token reuse even after finalization. */
        if (p->identity == identity) { LeaveCriticalSection(&r->lock); return NULL; }
    }
    o = (papinho_pst_owner *)calloc(1, sizeof(*o));
    if (!o) { LeaveCriticalSection(&r->lock); return NULL; }
    o->api.release = api ? api->release : pst_connection_try_release;
    o->api.remove = api ? api->remove : pst_wait_set_remove_connection;
    if (!o->api.release || !o->api.remove ||
        pst_release_info_init(&o->release_info, sizeof(o->release_info)) != PST_RESULT_OK) {
        free(o); LeaveCriticalSection(&r->lock); return NULL;
    }
    if (WSAStartup(MAKEWORD(1,1), &data) != 0) {
        free(o); LeaveCriticalSection(&r->lock); return NULL;
    }
    o->winsock_owned = 1;
    o->release_eligible = 1;
    o->registry = r; o->connection = connection; o->dependencies = *dependencies;
    o->identity = identity; o->generation = generation; o->role = role;
    o->thread_id = GetCurrentThreadId();

    o->next = r->head; r->head = o;
    LeaveCriticalSection(&r->lock);
    return o;
}

int papinho_pst_owner_callback_allowed(papinho_pst_owner *o,
    unsigned long identity, unsigned long generation) {
    int allowed;
    if (!o) return 0;
    EnterCriticalSection(&o->registry->lock);
    allowed = o->identity == identity && o->generation == generation &&
        !o->registry->admission_closed &&
        !o->closing && !o->finalized && !o->terminal_retained &&
        o->thread_id == GetCurrentThreadId();
    LeaveCriticalSection(&o->registry->lock);
    return allowed;
}

int papinho_pst_owner_remove_membership(papinho_pst_owner *o) {
    PST_RESULT rc;
    if (!o || o->thread_id != GetCurrentThreadId() ||
        o->terminal_retained || o->finalized) return 0;
    if (!o->wait_set) return 1;
    rc = o->api.remove(o->wait_set, o->connection);
    o->preclose_result = rc;
    if (rc != PST_RESULT_OK) return 0;
    o->wait_set = NULL;
    o->release_eligible = 1; /* factual owner-thread precondition correction */
    return 1;
}

int papinho_pst_owner_close(papinho_pst_owner *o,
    unsigned long identity, unsigned long generation) {
    PST_RESULT result;
    papinho_pst_owner_registry *r;
    if (!o) return 0;
    r = o->registry;
    EnterCriticalSection(&r->lock);
    if (o->identity != identity || o->generation != generation ||
        o->thread_id != GetCurrentThreadId()) {
        LeaveCriticalSection(&r->lock); return 0;
    }
    if (o->finalized) { LeaveCriticalSection(&r->lock); return 1; }
    if (o->terminal_retained) { LeaveCriticalSection(&r->lock); return 0; }
    o->closing = 1;
    if (!o->quiescence_confirmed) { LeaveCriticalSection(&r->lock); return 0; }
    /* PRE_CLOSE failure is not a fabricated PST release disposition. */
    if (!papinho_pst_owner_remove_membership(o)) {
        LeaveCriticalSection(&r->lock); return 0;
    }
    if (!o->release_eligible) { LeaveCriticalSection(&r->lock); return 0; }
    pst_release_info_init(&o->release_info, sizeof(o->release_info));
    ++o->release_attempts;
    if (o->connection == NULL) {
        /* Reserved owner whose PST creation never succeeded. No PST destruction
         * is claimed: NONE remains the release disposition and attempts is zero. */
        --o->release_attempts;
        if (!owner_finish_dependencies(o)) { LeaveCriticalSection(&r->lock); return 0; }
        LeaveCriticalSection(&r->lock); return 1;
    }
    result = o->api.release(o->connection, &o->release_info);
    if (result == PST_RESULT_OK && o->release_info.result == result &&
        o->release_info.disposition == PST_RELEASE_DISPOSITION_DESTROYED) {
        o->connection = NULL;
        if (!owner_finish_dependencies(o)) { LeaveCriticalSection(&r->lock); return 0; }
        LeaveCriticalSection(&r->lock); return 1;
    }
    if (result != PST_RESULT_INVALID_STATE ||
        o->release_info.result != result ||
        o->release_info.disposition != PST_RELEASE_DISPOSITION_PRECONDITION_RETAINED) {
        o->terminal_retained = 1;
        r->admission_closed = 1;
    } else {
        o->release_eligible = 0;
    }
    LeaveCriticalSection(&r->lock);
    return 0;
}

int papinho_pst_owner_reclaim(papinho_pst_owner *o) {
    papinho_pst_owner_registry *r;
    papinho_pst_owner **link;
    if (!o || !o->finalized || o->thread_id != GetCurrentThreadId()) return 0;
    r = o->registry;
    EnterCriticalSection(&r->lock);
    for (link = &r->head; *link && *link != o; link = &(*link)->next) {}
    if (*link != o) { LeaveCriticalSection(&r->lock); return 0; }
    *link = o->next;
    LeaveCriticalSection(&r->lock);
    free(o);
    return 1;
}

int papinho_pst_owner_registry_dispose(papinho_pst_owner_registry *r) {
    papinho_pst_owner *o, *next;
    if (!r) return 0;
    EnterCriticalSection(&r->lock);
    for (o = r->head; o; o = o->next) {
        if (!o->finalized) { LeaveCriticalSection(&r->lock); return 0; }
    }
    o = r->head; r->head = NULL;
    LeaveCriticalSection(&r->lock);
    while (o) { next = o->next; free(o); o = next; }
    /* Caller must have quiesced all callback sources before disposal. */
    DeleteCriticalSection(&r->lock);
    return 1;
}

/* No ownership transfer, provider init, release, logging or policy grant.
 * Caller holds the existing stable owner reference; registry serializes close. */
PST_RESULT papinho_pst_owner_provider_snapshot(papinho_pst_owner *o,
 unsigned long identity,unsigned long generation,PST_PROVIDER_INFO *out) {
 PST_PROVIDER_INFO f;PST_RESULT rc;papinho_pst_owner_registry *r;const pst_connection *connection;
 size_t a=(size_t)o,z=(size_t)out,t;
 if(!o||!out||!o->registry)return PST_RESULT_INVALID_ARGUMENT;
 r=o->registry;t=(size_t)r;
 if((z>=a&&z-a<sizeof(*o))||(z<a&&a-z<sizeof(*out))||
    (z>=t&&z-t<sizeof(*r))||(z<t&&t-z<sizeof(*out)))return PST_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&r->lock);
 if(o->identity!=identity||o->generation!=generation||!identity||!generation||
    r->admission_closed||o->closing||o->finalized||o->terminal_retained||
    o->release_attempts||!o->connection||o->thread_id!=GetCurrentThreadId()) {
  LeaveCriticalSection(&r->lock);return PST_RESULT_INVALID_STATE;
 }
 connection=o->connection;ZeroMemory(&f,sizeof(f));f.struct_size=sizeof(f);f.api_version=PST_API_VERSION;
 rc=pst_connection_get_provider_info(connection,&f);
 if(rc==PST_RESULT_OK&&(o->connection!=connection||o->closing||o->finalized||
    o->terminal_retained||o->release_attempts||o->thread_id!=GetCurrentThreadId()||
    o->registry!=r||r->admission_closed||o->identity!=identity||o->generation!=generation||
    !memchr(f.provider_id,0,sizeof(f.provider_id))||strcmp(f.provider_id,"retrozilla-nss")||
    f.available!=1U||f.initialized>1U))rc=PST_RESULT_INVALID_STATE;
 if(rc==PST_RESULT_OK)*out=f;
 LeaveCriticalSection(&r->lock);return rc;
}
