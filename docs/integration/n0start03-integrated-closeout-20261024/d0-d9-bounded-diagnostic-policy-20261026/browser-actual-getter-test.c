#include <stdio.h>
#include <string.h>
#include "papinho_pst_owner.h"

static unsigned calls,mode,checks;static char fake;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
static papinho_pst_owner state;static papinho_pst_owner_registry registry;

PST_RESULT PST_CALL pst_connection_get_provider_info(const pst_connection *c,PST_PROVIDER_INFO *v){(void)c;++calls;if(mode==1)return (PST_RESULT)77;v->available=1;v->initialized=mode==2?0:1;memcpy(v->provider_id,"retrozilla-nss",14);if(mode==3)memset(v->provider_id,'x',sizeof(v->provider_id));if(mode==4)v->provider_id[0]='X';if(mode==5)state.terminal_retained=1;if(mode==6)++state.generation;if(mode==7)++state.thread_id;if(mode==8)state.release_attempts=1;return PST_RESULT_OK;}
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

static void reset(void){memset(&state,0,sizeof(state));state.registry=&registry;state.thread_id=GetCurrentThreadId();state.connection=(pst_connection*)&fake;state.identity=10;state.generation=20;state.preclose_result=(PST_RESULT)91;registry.admission_closed=0;mode=0;calls=0;}
int main(void){PST_PROVIDER_INFO out,old;unsigned i;InitializeCriticalSection(&registry.lock);reset();memset(&old,0x6a,sizeof(old));out=old;
 CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==0&&out.initialized==1&&calls==1);
 for(i=1;i<=8;i++){reset();mode=i;out=old;CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==(i==1?77:(i==2?0:PST_RESULT_INVALID_STATE)));CHECK(state.preclose_result==91);if(i!=2)CHECK(!memcmp(&out,&old,sizeof(out)));}
 reset();state.closing=1;CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();state.finalized=1;CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();state.terminal_retained=1;CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();registry.admission_closed=1;CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();state.release_attempts=1;CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();state.thread_id++;CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();CHECK(papinho_pst_owner_provider_snapshot(&state,9,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 CHECK(papinho_pst_owner_provider_snapshot(&state,10,19,&out)==PST_RESULT_INVALID_STATE&&!calls);
 CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,(PST_PROVIDER_INFO*)&state)==PST_RESULT_INVALID_ARGUMENT&&!calls);
 CHECK(papinho_pst_owner_provider_snapshot(&state,10,20,(PST_PROVIDER_INFO*)&registry)==PST_RESULT_INVALID_ARGUMENT&&!calls);
 CHECK(papinho_pst_owner_provider_snapshot(NULL,10,20,&out)==PST_RESULT_INVALID_ARGUMENT);
 DeleteCriticalSection(&registry.lock);printf("PASS Browser_actual_function checks=%u provider_info=%u owner=%u; APIS_SIMULATED_NO_SOCKET_NO_TLS\n",checks,(unsigned)sizeof(out),(unsigned)sizeof(state));return 0;}
