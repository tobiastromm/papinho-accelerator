/* Negative interface/control tests. No producer, fabricated lease or authority.
 * Existing M1 model suite covers positive sequence CONTROL_ONLY separately.
 */
#include "trust.h"
#include "m1_internal.h"
#include <stdio.h>
#include <string.h>
static unsigned checks;
#define C(x) do{++checks;if(!(x)){printf("FAIL %u\n",(unsigned)__LINE__);return 1;}}while(0)
typedef struct OWNER {SP_M1_STATE flow;unsigned seen[11],wipe,publish;int stop,quiet;} OWNER;
static int step(void *p,SP_M1_STAGE stage,int *detail){
 OWNER *o=p;const SP_TRUST_LEASE *lease=NULL;SP_TRUST_RESULT r;
 ++o->seen[stage];
 if(stage==SP_M1_TRUST || stage==SP_M1_RECHECK_TRUST){
  r=sp_trust_acquire(NULL,NULL,NULL,&lease);
  *detail=-102;
  return r==SP_TRUST_DENIED && !lease?SP_M1_DENIED:SP_M1_INVALID;
 }
 *detail=1;return SP_M1_COMPLETE;
}
static int stopped(void *p){return ((OWNER *)p)->stop;}
static int quiet(void *p){return ((OWNER *)p)->quiet;}
static void wipe(void *p){++((OWNER *)p)->wipe;}
static void publish(void *p){++((OWNER *)p)->publish;}
static const SP_M1_BINDING binding={step,stopped,quiet,wipe,publish};
int main(void){
 OWNER o;const SP_TRUST_LEASE *lease=NULL;unsigned i;
 C(SP_TRUST_CONTRACT_REVISION==2);
 C(sp_trust_acquire(NULL,NULL,NULL,NULL)==SP_TRUST_INVALID);
 for(i=0;i<3;++i){
  C(sp_trust_acquire(NULL,NULL,NULL,&lease)==SP_TRUST_DENIED);
  C(lease==NULL);
 }
 C(sp_trust_revalidate(NULL,SP_TRUST_BEFORE_KDF)==SP_TRUST_INVALID);
 C(sp_trust_revalidate(NULL,SP_TRUST_BEFORE_RELEASE)==SP_TRUST_INVALID);
 C(sp_trust_revalidate(NULL,(SP_TRUST_PHASE)0)==SP_TRUST_INVALID);
 C(sp_trust_release(NULL)==SP_TRUST_RELEASE_INVALID);
 C(sp_trust_release_query(NULL)==SP_TRUST_RELEASE_INVALID);
 memset(&o,0,sizeof(o));o.quiet=1;
 C(sp_m1_execute(&o.flow,&binding,&o)==SP_M1_DENIED);
 C(o.flow.first_failure_stage==SP_M1_TRUST && o.flow.first_failure_code==-102);
 C(o.seen[SP_M1_SIGNATURE]==1 && o.seen[SP_M1_TRUST]==1);
 C(o.flow.kdf_invocations==0 && o.flow.aead_invocations==0 && o.flow.content_invocations==0);
 C(!o.publish && o.flow.state==3);
 C(sp_m1_execute(&o.flow,&binding,&o)==SP_M1_INVALID);
 memset(&o,0,sizeof(o));o.stop=1;o.quiet=1;
 C(sp_m1_execute(&o.flow,&binding,&o)==SP_M1_STOPPED);
 C(!o.seen[SP_M1_TRUST] && !o.flow.kdf_invocations && !o.publish);
 memset(&o,0,sizeof(o));
 C(sp_m1_execute(&o.flow,&binding,&o)==SP_M1_RETAINED);
 C(o.flow.state==2 && o.wipe==1 && !o.publish);
 C(sp_m1_finish(&o.flow,&binding,&o)==SP_M1_RETAINED);
 C(o.wipe==1 && o.seen[SP_M1_TRUST]==1);
 o.quiet=1;
 C(sp_m1_finish(&o.flow,&binding,&o)==SP_M1_ABORTED);
 C(o.wipe==2 && o.seen[SP_M1_TRUST]==1 && !o.publish && !o.flow.kdf_invocations);
 C(sp_m1_finish(&o.flow,&binding,&o)==SP_M1_INVALID);
 printf("PASS %u SOURCE_ABSENT checks; no positive lease or factual trust; state=%u owner=%u pointer=%u\n",checks,(unsigned)sizeof(SP_M1_STATE),(unsigned)sizeof(OWNER),(unsigned)sizeof(lease));
 return 0;
}
