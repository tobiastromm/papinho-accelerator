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
