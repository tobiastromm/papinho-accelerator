/* Simulated public APIs and borrowed objects only; never a PST Connection. */
#include <stdio.h>
#include "owner-provider-readonly-probe.h"
static unsigned guards,calls,freeze_at,fail_at,malformed,mismatch,initialized=1U,disposition;
static int current(void *p){(void)p;++guards;return guards!=freeze_at;}
static PST_RESULT fill(PST_PROVIDER_INFO *v){++calls;if(calls==fail_at)return (PST_RESULT)77;v->available=1;v->initialized=initialized;memcpy(v->provider_id,"openssl",8U);if(malformed)memset(v->provider_id,'x',sizeof(v->provider_id));return PST_RESULT_OK;}
static PST_RESULT PST_CALL rp(const pst_runtime *r,pst_size i,PST_PROVIDER_INFO *v){(void)r;(void)i;return fill(v);}
static PST_RESULT PST_CALL cp(const pst_connection *c,PST_PROVIDER_INFO *v){PST_RESULT rc;(void)c;rc=fill(v);if(mismatch)v->provider_id[0]='X';return rc;}
static PST_RESULT PST_CALL rel(const pst_connection *c,PST_RELEASE_INFO *v){(void)c;++calls;if(calls==fail_at)return (PST_RESULT)77;v->disposition=disposition;return PST_RESULT_OK;}
static unsigned checks;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
int main(void){ONE_OP_QUERY_API api;ONE_OP_PROVIDER_SNAPSHOT out;unsigned i;char runtime_object,connection_object;
 const pst_runtime *r=(const pst_runtime *)&runtime_object;const pst_connection *c=(const pst_connection *)&connection_object;
 api.runtime_provider=rp;api.connection_provider=cp;api.release_info=rel;api.existing_borrow_current=current;
 one_op_provider_snapshot(&api,NULL,r,c,0,&out);CHECK(out.first==0&&out.complete&&out.initialized_reported&&out.calls==3U);
 for(i=1;i<=4U;i++){calls=guards=0;freeze_at=i;one_op_provider_snapshot(&api,NULL,r,c,0,&out);CHECK(out.first==PST_RESULT_INVALID_STATE&&!out.complete&&calls==i-1U);}
 freeze_at=0;
 for(i=1;i<=3U;i++){calls=guards=0;fail_at=i;one_op_provider_snapshot(&api,NULL,r,c,0,&out);CHECK(out.first==77&&!out.complete&&calls==i);}
 fail_at=0;calls=guards=0;initialized=0;one_op_provider_snapshot(&api,NULL,r,c,0,&out);CHECK(out.complete&&!out.initialized_reported);initialized=1;
 for(i=1;i<=3U;i++){calls=guards=0;disposition=i;one_op_provider_snapshot(&api,NULL,r,c,0,&out);if(i==2U){CHECK(out.first==PST_RESULT_INVALID_STATE&&!out.complete);}else{CHECK(out.complete&&out.retained_reported);}}
 disposition=0;calls=guards=0;malformed=1;one_op_provider_snapshot(&api,NULL,r,c,0,&out);CHECK(out.first==PST_RESULT_INVALID_STATE&&calls==2U);malformed=0;
 calls=guards=0;mismatch=1;one_op_provider_snapshot(&api,NULL,r,c,0,&out);CHECK(out.first==PST_RESULT_INVALID_STATE&&!out.complete);mismatch=0;
 calls=guards=0;one_op_provider_snapshot(&api,NULL,r,NULL,0,&out);CHECK(out.first==PST_RESULT_INVALID_ARGUMENT&&calls==0U);
 one_op_provider_snapshot(&api,NULL,r,c,8,&out);CHECK(out.first==PST_RESULT_INVALID_ARGUMENT&&calls==0U);
 one_op_provider_snapshot(NULL,NULL,r,c,0,&out);CHECK(out.first==PST_RESULT_INVALID_ARGUMENT&&calls==0U);
 printf("PASS checks=%u snapshot_bytes=%u; APIs_SIMULATED; NO_PROVIDER_INITIALIZATION_NO_NETWORK_NO_CREDIT\n",checks,(unsigned)sizeof(out));return 0;}
