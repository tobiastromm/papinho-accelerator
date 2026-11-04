/* Isolated private candidate. No owner, handle, allocation, log or admission. */
#ifndef ONE_OP_OWNER_READONLY_PROBE_H
#define ONE_OP_OWNER_READONLY_PROBE_H
#include <string.h>
#include <papinho_secure_transport.h>
typedef struct ONE_OP_QUERY_API {
 PST_RESULT (PST_CALL *runtime_provider)(const pst_runtime *,pst_size,PST_PROVIDER_INFO *);
 PST_RESULT (PST_CALL *connection_provider)(const pst_connection *,PST_PROVIDER_INFO *);
 PST_RESULT (PST_CALL *release_info)(const pst_connection *,PST_RELEASE_INFO *);
 /* Must validate existing serialized owner borrow, original lifetime and no freeze.
  * A caller Boolean is NOT an implementation of this required integration hook. */
 int (*existing_borrow_current)(void *);
} ONE_OP_QUERY_API;
typedef struct ONE_OP_PROVIDER_SNAPSHOT {
 PST_RESULT first;
 unsigned calls,complete,initialized_reported,retained_reported;
 PST_PROVIDER_INFO runtime_provider,connection_provider;
 PST_RELEASE_INFO release;
} ONE_OP_PROVIDER_SNAPSHOT;
/* Calls only public getters under an already-held, non-reentrant owner borrow.
 * The caller retains ALL ownership. No release/query-after-destroy/retry.
 * Getter success says nothing about mapped image, bytes or kernel capacity. */
static void one_op_provider_snapshot(const ONE_OP_QUERY_API *api,void *borrow,
 const pst_runtime *runtime,const pst_connection *connection,pst_size index,
 ONE_OP_PROVIDER_SNAPSHOT *out)
{
 PST_RESULT rc;
 if(!out)return;
 memset(out,0,sizeof(*out));
 if(!api||!api->runtime_provider||!api->connection_provider||!api->release_info||
    !api->existing_borrow_current||!runtime||!connection||index>=8U){out->first=PST_RESULT_INVALID_ARGUMENT;return;}
 if(!api->existing_borrow_current(borrow)){out->first=PST_RESULT_INVALID_STATE;return;}
 out->runtime_provider.struct_size=sizeof(out->runtime_provider);out->runtime_provider.api_version=PST_API_VERSION;
 rc=api->runtime_provider(runtime,index,&out->runtime_provider);++out->calls;
 if(rc!=PST_RESULT_OK){out->first=rc;return;}
 if(!api->existing_borrow_current(borrow)){out->first=PST_RESULT_INVALID_STATE;return;}
 out->connection_provider.struct_size=sizeof(out->connection_provider);out->connection_provider.api_version=PST_API_VERSION;
 rc=api->connection_provider(connection,&out->connection_provider);++out->calls;
 if(rc!=PST_RESULT_OK){out->first=rc;return;}
 if(!api->existing_borrow_current(borrow)){out->first=PST_RESULT_INVALID_STATE;return;}
 if(!memchr(out->runtime_provider.provider_id,0,sizeof(out->runtime_provider.provider_id))||
    !memchr(out->connection_provider.provider_id,0,sizeof(out->connection_provider.provider_id))||
    !out->runtime_provider.provider_id[0]||
    strcmp(out->runtime_provider.provider_id,out->connection_provider.provider_id)||
    out->runtime_provider.available!=1U||out->connection_provider.available!=1U||
    out->runtime_provider.initialized>1U||out->connection_provider.initialized>1U||
    out->runtime_provider.initialized!=out->connection_provider.initialized){out->first=PST_RESULT_INVALID_STATE;return;}
 out->release.struct_size=sizeof(out->release);out->release.api_version=PST_API_VERSION;
 rc=api->release_info(connection,&out->release);++out->calls;
 if(rc!=PST_RESULT_OK){out->first=rc;return;}
 if(!api->existing_borrow_current(borrow)){out->first=PST_RESULT_INVALID_STATE;return;}
 if(out->release.disposition==PST_RELEASE_DISPOSITION_DESTROYED||out->release.disposition>3U){out->first=PST_RESULT_INVALID_STATE;return;}
 out->retained_reported=out->release.disposition==PST_RELEASE_DISPOSITION_PRECONDITION_RETAINED||out->release.disposition==PST_RELEASE_DISPOSITION_CLOSE_UNRESOLVED_RETAINED;
 out->initialized_reported=out->runtime_provider.initialized;out->complete=1U;
}
#endif
