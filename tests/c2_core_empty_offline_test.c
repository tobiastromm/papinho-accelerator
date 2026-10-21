/* Empty real C2 manager and invalid identities only; no granted Session. */
#include <windows.h>
#include "c2_operation_win32.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static PAPACC_C2_OPERATION_MANAGER manager;
static unsigned policy_calls,capacity_calls;
static PAPACC_RESULT deny_policy(void *c,const PAPACC_C2_SESSION *s,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_BOOL *out)
{(void)c;(void)s;(void)p;(void)n;++policy_calls;*out=PAPACC_FALSE;return PAPACC_RESULT_OK;}
static PAPACC_RESULT deny_capacity(void *c,PAPACC_SIZE n,PAPACC_BOOL *out)
{(void)c;(void)n;++capacity_calls;*out=PAPACC_FALSE;return PAPACC_RESULT_OK;}
int main(void)
{
 PAPACC_C2_SESSION absent;PAPACC_PRINCIPAL invalid;PAPACC_C2_OPERATION *operation=NULL;
 PAPACC_U8 result[108]={0},payload[56]={0};unsigned i;
 memset(&absent,0,sizeof(absent));memset(&invalid,0,sizeof(invalid));
 CHECK(papacc_c2_operation_manager_init(&manager,GetCurrentThreadId(),deny_policy,deny_capacity,NULL)==PAPACC_RESULT_OK);
 CHECK(papacc_c2_operation_manager_init(&manager,GetCurrentThreadId(),deny_policy,deny_capacity,NULL)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_c2_operation_session_attach(&manager,&absent,&invalid)!=PAPACC_RESULT_OK);
 CHECK(papacc_c2_operation_open(&manager,&absent,payload,sizeof(payload),result,&operation)!=PAPACC_RESULT_OK&&operation==NULL);
 CHECK(papacc_c2_operation_bind(&manager,&absent,&invalid,1U,payload,sizeof(payload),&operation)!=PAPACC_RESULT_OK&&operation==NULL);
 CHECK(papacc_c2_operation_tick(&manager,1U)==PAPACC_RESULT_OK);
 CHECK(papacc_c2_operation_stop(&manager)==PAPACC_RESULT_OK&&manager.stopping);
 CHECK(papacc_c2_operation_stop(&manager)==PAPACC_RESULT_OK);
 CHECK(papacc_c2_operation_session_attach(&manager,&absent,&invalid)!=PAPACC_RESULT_OK);
 CHECK(papacc_c2_operation_release_slot(&manager,&manager.operations[0])!=PAPACC_RESULT_OK);
 for(i=0;i<PAPACC_C2_OPERATION_CAPACITY;++i)CHECK(manager.operations[i].state==PAPACC_C2_OP_EMPTY);
 for(i=0;i<PAPACC_C2_SESSION_CAPACITY;++i)CHECK(manager.sessions[i].session==NULL);
 CHECK(policy_calls==0&&capacity_calls==0);
 puts("PASS_OFFLINE_REAL_EMPTY_MANAGER NO_AUTHENTICATED_SESSION_OR_EPOCH_CREATED");return 0;
}