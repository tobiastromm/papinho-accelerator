/* Synthetic CONTROL ONLY. No crypto, trust receipt, key, package or private data. */
#include "m1_internal.h"
#include <stdio.h>
#include <string.h>
static unsigned checks;
#define C(x) do{++checks;if(!(x)){printf("FAIL %u\n",(unsigned)__LINE__);return 1;}}while(0)
typedef struct MODEL {
 SP_M1_STATE flow;unsigned seen[10],order[10],count,published,wiped;
 SP_M1_STAGE fail,stop_at,jump;int after,code,result,quiet,source,current,pin,revoked;
} MODEL;
static int perform(void *p,SP_M1_STAGE s,int *detail){
 MODEL *m=p;if(s==SP_M1_CONTENT_PARSE){if(m->stop_at==SP_M1_CONTENT && !m->after){*detail=-3;return SP_M1_STOPPED;}return SP_M1_COMPLETE;}m->order[m->count++]=(unsigned)s;++m->seen[s];
 if(s==SP_M1_TRUST && (!m->source || !m->current || !m->pin || m->revoked)){*detail=m->revoked?-104:-102;return SP_M1_DENIED;}
 if(s==m->jump)m->flow.stage=SP_M1_RELEASE;
 if(s==m->fail){*detail=m->code;return m->result;}
 *detail=1;return SP_M1_COMPLETE;
}
static int stop(void *p){MODEL *m=p;return m->stop_at && m->flow.stage==m->stop_at && (!m->after || m->seen[m->stop_at]);}
static int quiet(void *p){return ((MODEL *)p)->quiet;}
static void wipe(void *p){++((MODEL *)p)->wiped;}
static void publish(void *p){++((MODEL *)p)->published;}
static const SP_M1_BINDING binding={perform,stop,quiet,wipe,publish};
static void reset(MODEL *m){memset(m,0,sizeof(*m));m->quiet=m->source=m->current=m->pin=1;m->code=-101;m->result=SP_M1_DENIED;}
int main(void){
 MODEL m,other;unsigned j,k;int result;
 reset(&m);C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_COMPLETE);
 C(m.count==9 && m.published==1 && m.flow.state==3);
 for(j=0;j<9;++j)C(m.order[j]==j+1);
 C(m.flow.kdf_invocations==1 && m.flow.aead_invocations==1);
 C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_INVALID);
 C(sp_m1_finish(&m.flow,&binding,&m)==SP_M1_INVALID);
 /* No source/CURRENT/pin/revocation is a model, never native authority. */
 for(j=0;j<4;++j){reset(&m);if(j==0)m.source=0;if(j==1)m.current=0;if(j==2)m.pin=0;if(j==3)m.revoked=1;
 C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_DENIED);C(m.flow.first_failure_stage==SP_M1_TRUST);
 C(!m.seen[SP_M1_KDF] && !m.seen[SP_M1_AEAD] && !m.published && m.flow.state==3);
 C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_INVALID);}
 for(j=SP_M1_SIGNATURE;j<=SP_M1_RELEASE;++j){
  reset(&m);m.fail=(SP_M1_STAGE)j;C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_DENIED);
  C(m.flow.first_failure_stage==(SP_M1_STAGE)j && !m.published && m.flow.state==3);
  for(k=j+1;k<=SP_M1_RELEASE;++k)C(!m.seen[k]);
  reset(&m);m.stop_at=(SP_M1_STAGE)j;C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_STOPPED);C(!m.seen[j] && !m.published);
  reset(&m);m.stop_at=(SP_M1_STAGE)j;m.after=1;C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_STOPPED);C(m.seen[j]==1 && !m.published);
  reset(&m);m.jump=(SP_M1_STAGE)j;if(j!=SP_M1_RELEASE){C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_DENIED);C(!m.published);}
  reset(&m);m.fail=(SP_M1_STAGE)j;m.code=-103;m.quiet=0;
  C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_RETAINED);C(m.flow.state==2 && m.wiped==1 && !m.published);
  C(sp_m1_finish(&m.flow,&binding,&m)==SP_M1_RETAINED);C(m.wiped==1);
  reset(&other);C(sp_m1_finish(&m.flow,&binding,&other)==SP_M1_INVALID);
  m.quiet=1;result=m.flow.first_failure_code;C(sp_m1_finish(&m.flow,&binding,&m)==SP_M1_ABORTED);
  C(m.flow.first_failure_code==result && m.flow.state==3 && m.wiped==2 && !m.published && m.seen[j]==1);
  C(sp_m1_finish(&m.flow,&binding,&m)==SP_M1_INVALID);
 }
 for(j=SP_M1_STRUCTURE;j<=SP_M1_BUDGET;++j){reset(&m);m.fail=(SP_M1_STAGE)j;m.result=j==SP_M1_BUDGET?SP_M1_LIMIT:SP_M1_INVALID;
 C(sp_m1_execute(&m.flow,&binding,&m)==m.result);C(!m.wiped && !m.published && m.flow.state==3);}
 reset(&m);m.quiet=0;C(sp_m1_execute(&m.flow,&binding,&m)==SP_M1_RETAINED);C(!m.published);
 m.quiet=1;C(sp_m1_finish(&m.flow,&binding,&m)==SP_M1_ABORTED && !m.published);
 printf("PASS %u shared M1 CONTROL_ONLY checks state_size=%u binding_size=%u; no factual trust\n",checks,(unsigned)sizeof(SP_M1_STATE),(unsigned)sizeof(binding));return 0;
}
