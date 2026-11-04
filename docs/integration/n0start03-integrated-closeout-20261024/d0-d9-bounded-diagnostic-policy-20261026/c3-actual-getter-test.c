#include <stdio.h>
#include <string.h>
#include "c3_bridge_win32.h"

static unsigned calls,mode,checks;static char fake;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
static PAPACC_C3_BRIDGE state;
static int owner(const PAPACC_C3_BRIDGE*b){return b!=NULL&&b->initialized&&b->owner_thread==GetCurrentThreadId();}

PST_RESULT PST_CALL pst_connection_get_provider_info(const pst_connection *c,PST_PROVIDER_INFO *v){
 (void)c;++calls;if(mode==1)return (PST_RESULT)77;v->available=1;v->initialized=mode==2?0:1;memcpy(v->provider_id,"openssl",8);
 if(mode==3)memset(v->provider_id,'x',sizeof(v->provider_id));if(mode==4)v->provider_id[0]='X';if(mode==5)state.retained=1;if(mode==6)++state.generation;if(mode==7)state.child_acquired=0;if(mode==8)++state.owner_thread;return PST_RESULT_OK;
}
PST_RESULT papacc_c3_bridge_provider_snapshot(const PAPACC_C3_BRIDGE *b,
 PAPACC_U64 operation,PAPACC_U64 generation,PST_PROVIDER_INFO *out)
{
 PST_PROVIDER_INFO f;PST_RESULT rc;const pst_connection *connection;
 ULONG_PTR a=(ULONG_PTR)b,z=(ULONG_PTR)out;
 if(!b||!out)return PST_RESULT_INVALID_ARGUMENT;
 if((z>=a&&z-a<sizeof(*b))||(z<a&&a-z<sizeof(*out)))return PST_RESULT_INVALID_ARGUMENT;
 if(!owner(b)||b->revoked||b->retained||b->child_destroyed||!b->child_acquired||
    !operation||operation!=b->operation||generation!=b->generation||!b->processor.connection)
  return PST_RESULT_INVALID_STATE;
 connection=b->processor.connection;memset(&f,0,sizeof(f));f.struct_size=sizeof(f);f.api_version=PST_API_VERSION;
 rc=pst_connection_get_provider_info(connection,&f);
 if(rc!=PST_RESULT_OK)return rc;
 if(!owner(b)||b->revoked||b->retained||b->child_destroyed||!b->child_acquired||b->processor.connection!=connection||
    operation!=b->operation||generation!=b->generation||
    !memchr(f.provider_id,0,sizeof(f.provider_id))||strcmp(f.provider_id,"openssl")||
    f.available!=1U||f.initialized>1U)return PST_RESULT_INVALID_STATE;
 *out=f;return PST_RESULT_OK;
}

static void reset(void){memset(&state,0,sizeof(state));state.initialized=1;state.owner_thread=GetCurrentThreadId();state.child_acquired=1;state.operation=10;state.generation=20;state.processor.connection=(pst_connection*)&fake;state.first_failure=(PST_RESULT)91;calls=0;mode=0;}
int main(void){PST_PROVIDER_INFO out,old;unsigned i;reset();memset(&old,0x6a,sizeof(old));out=old;
 CHECK(papacc_c3_bridge_provider_snapshot(&state,10,20,&out)==0&&out.initialized==1&&calls==1);
 for(i=1;i<=8;i++){reset();mode=i;out=old;CHECK(papacc_c3_bridge_provider_snapshot(&state,10,20,&out)==(i==1?77:(i==2?0:PST_RESULT_INVALID_STATE)));CHECK(state.first_failure==91);if(i!=2)CHECK(!memcmp(&out,&old,sizeof(out)));}
 reset();state.child_destroyed=1;CHECK(papacc_c3_bridge_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();state.revoked=1;CHECK(papacc_c3_bridge_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();state.retained=1;CHECK(papacc_c3_bridge_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();state.owner_thread++;CHECK(papacc_c3_bridge_provider_snapshot(&state,10,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 reset();CHECK(papacc_c3_bridge_provider_snapshot(&state,9,20,&out)==PST_RESULT_INVALID_STATE&&!calls);
 CHECK(papacc_c3_bridge_provider_snapshot(&state,10,19,&out)==PST_RESULT_INVALID_STATE&&!calls);
 CHECK(papacc_c3_bridge_provider_snapshot(&state,10,20,(PST_PROVIDER_INFO*)&state)==PST_RESULT_INVALID_ARGUMENT&&!calls);
 CHECK(papacc_c3_bridge_provider_snapshot(NULL,10,20,&out)==PST_RESULT_INVALID_ARGUMENT);
 printf("PASS C3_actual_function checks=%u provider_info=%u; APIS_SIMULATED_NO_SOCKET_NO_TLS\n",checks,(unsigned)sizeof(out));return 0;}
