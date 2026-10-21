/* TEST_ONLY portable ED11 candidate policy; no product wire or PST results. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define CORE ((size_t)524288)
#define SESS ((size_t)16384)
#define APP ((size_t)1048576)
#define CAP ((size_t)2097152)
typedef struct Entry { unsigned op,gen,session,state; unsigned char *core,*app; size_t bytes; unsigned callbacks; } Entry;
typedef struct Budget {
 Entry slots[2]; unsigned char *sessions[2]; unsigned session_users[2],session_generation[2],session_callbacks[2];
 size_t charged,peak,limit,host_available,host_reserve,physical_available,process_available,host_reserved;
 unsigned alloc_calls,fail_at,effects,denied,freed,cancel_reserve,last_op;
 int provider_ready,kernel_ready,measurement_ok,envelope_known;
} Budget;
static int assertions,failures;
static void expect(int ok,const char *name) { ++assertions;if(!ok)++failures;printf("{\"test\":\"%s\",\"result\":\"%s\",\"class\":\"ISOLATED_CANDIDATE_POLICY\"}\n",name,ok?"PASS":"FAIL"); }
static void start(Budget *b) { memset(b,0,sizeof(*b));b->charged=sizeof(*b);b->peak=b->charged;b->limit=CAP;b->host_available=64*CAP;b->physical_available=64*CAP;b->process_available=64*CAP;b->host_reserve=CAP;b->provider_ready=b->kernel_ready=b->measurement_ok=b->envelope_known=1; }
static void *allocate(Budget *b,size_t n) { ++b->alloc_calls;if(b->fail_at==b->alloc_calls)return NULL;return calloc(1,n); }
static Entry *reserve(Budget *b,unsigned op,unsigned gen,unsigned session,int browser) {
 Entry *e;unsigned i;size_t cost,extra,all;unsigned char *ss,*core,*app;int fresh;
 if(!op||op<=b->last_op||!gen||session>=2||!b->measurement_ok||!b->provider_ready||!b->kernel_ready||!b->envelope_known){++b->denied;return NULL;}
 e=NULL;for(i=0;i<2;++i){if(b->slots[i].state&&b->slots[i].session==session){++b->denied;return NULL;}if(!b->slots[i].state&&!e)e=&b->slots[i];}
 if(!e){++b->denied;return NULL;}
 cost=CORE;if(browser){if(cost>(size_t)-1-APP){++b->denied;return NULL;}cost+=APP;}
 fresh=b->sessions[session]==NULL;if(fresh&&b->session_generation[session]==UINT_MAX){++b->denied;return NULL;}extra=fresh?SESS:0;
 if(cost>(size_t)-1-extra){++b->denied;return NULL;}all=cost+extra;
 /* A real host adapter must supply conservative available capacity and reserve.
    Test values are explicit synthetic gate inputs, not measured kernel ceilings. */
 if(b->charged>b->limit||all>b->limit-b->charged||b->host_available<b->host_reserve||b->host_reserved>b->host_available-b->host_reserve||all>b->host_available-b->host_reserve-b->host_reserved||b->physical_available<b->host_reserve||b->host_reserved>b->physical_available-b->host_reserve||all>b->physical_available-b->host_reserve-b->host_reserved||b->process_available<b->host_reserve||b->host_reserved>b->process_available-b->host_reserve||all>b->process_available-b->host_reserve-b->host_reserved){++b->denied;return NULL;}
 b->charged+=all;b->host_reserved+=all;if(b->charged>b->peak)b->peak=b->charged;
 ss=NULL;core=NULL;app=NULL;
 if(fresh){ss=(unsigned char*)allocate(b,SESS);if(!ss)goto rollback;}
 core=(unsigned char*)allocate(b,CORE);if(!core)goto rollback;
 if(browser){app=(unsigned char*)allocate(b,APP);if(!app)goto rollback;}
 if(b->cancel_reserve)goto rollback;
 memset(core,0xa5,CORE);if(app)memset(app,0x5a,APP);
 if(fresh){b->sessions[session]=ss;++b->session_generation[session];}
 ++b->session_users[session];e->op=op;e->gen=gen;e->session=session;e->state=1;e->core=core;e->app=app;e->bytes=cost;b->last_op=op;++b->effects;return e;
rollback:
 free(app);free(core);free(ss);b->charged-=all;b->host_reserved-=all;++b->denied;return NULL;
}
static int finish(Budget *b,Entry *e,unsigned op,unsigned gen,int destroyed) {
 unsigned s;
 if(!e||!e->state||e->op!=op||e->gen!=gen||e->state==2||!destroyed||e->callbacks)return 0;
 s=e->session;free(e->core);free(e->app);b->charged-=e->bytes;b->host_reserved-=e->bytes;memset(e,0,sizeof(*e));++b->freed;
 --b->session_users[s];return 1;
}
static int close_session(Budget *b,unsigned s,unsigned gen,int destroyed) {
 if(s>=2||!b->sessions[s]||b->session_generation[s]!=gen||!destroyed||b->session_users[s]||b->session_callbacks[s])return 0;
 free(b->sessions[s]);b->sessions[s]=NULL;b->charged-=SESS;b->host_reserved-=SESS;return 1;
}
static int retain(Entry *e,unsigned op,unsigned gen) { if(!e||e->state!=1||e->op!=op||e->gen!=gen)return 0;e->state=2;return 1; }
static int arithmetic(size_t a,size_t b,size_t *sum) { if(a>(size_t)-1-b)return 0;*sum=a+b;return 1; }
static void cases(void) {
 static Budget b;Entry *a,*e;unsigned n,i;size_t before,sum;
 for(n=1;n<=3;++n){start(&b);before=b.charged;b.fail_at=n;expect(reserve(&b,1,7,0,1)==NULL&&b.charged==before&&b.effects==0&&b.sessions[0]==NULL,"PARTIAL_ALLOCATION_ROLLBACK");}
 start(&b);before=b.charged;b.cancel_reserve=1;expect(reserve(&b,1,7,0,1)==NULL&&b.charged==before&&b.effects==0,"CANCEL_DURING_RESERVATION_ROLLBACK");
 start(&b);before=b.charged;b.measurement_ok=0;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0&&b.charged==before,"MEASUREMENT_FAILURE_DENIES_BEFORE_EFFECTS");
 start(&b);b.provider_ready=0;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"PROVIDER_UNAVAILABLE_DENIES");
 start(&b);b.kernel_ready=0;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"KERNEL_RESOURCE_UNAVAILABLE_DENIES");
 start(&b);b.envelope_known=0;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"UNKNOWN_OPAQUE_ENVELOPE_DENIES");
 start(&b);b.host_available=b.host_reserve;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"HOST_EXHAUSTED_DESPITE_INCREMENT_FITTING");
 start(&b);b.physical_available=b.host_reserve;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"PHYSICAL_EXHAUSTION_DESPITE_COMMIT_HEADROOM_DENIES");
 start(&b);b.process_available=b.host_reserve;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"PROCESS_CAPACITY_EXHAUSTED_DESPITE_HOST_AND_QUOTA_DENIES");
 start(&b);b.host_available=b.host_reserve+CORE+SESS;a=reserve(&b,1,7,0,0);expect(a!=NULL,"HOST_EXACT_RESERVATION_ACCEPTS");before=b.host_reserved;expect(reserve(&b,2,7,1,0)==NULL&&b.host_reserved==before,"HOST_SHARED_SNAPSHOT_CREDIT_NOT_DOUBLESPENT");if(a){expect(finish(&b,a,1,7,1)&&b.host_reserved==SESS,"SESSION_CONTROL_RESERVATION_PERSISTS_AT_ZERO_OPS");expect(close_session(&b,0,1,1)&&b.host_reserved==0,"HOST_CREDIT_RECOVERS_ONLY_ON_CONFIRMED_SESSION_FREE");}
 start(&b);b.host_available=b.host_reserve-1;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"HOST_HEADROOM_UNDERFLOW_DENIES");
 start(&b);b.limit=b.charged+CORE+SESS-1;expect(reserve(&b,1,7,0,0)==NULL&&b.alloc_calls==0,"EXACT_REQUEST_EXCEEDS_QUOTA_BY_ONE_BYTE");
 start(&b);b.limit=b.charged+CORE+SESS;a=reserve(&b,1,7,0,0);expect(a!=NULL&&b.charged==b.limit,"EXACT_LIMIT_ACCEPTS");
 expect(reserve(&b,2,7,1,0)==NULL,"LIMIT_PLUS_ONE_OR_MORE_DENIES");if(a){expect(finish(&b,a,1,7,1),"EXACT_LIMIT_OP_RELEASE");expect(close_session(&b,0,1,1),"EXACT_LIMIT_SESSION_RELEASE");}
 expect(!arithmetic((size_t)-1,1,&sum),"CHECKED_ARITHMETIC_OVERFLOW");
 expect(arithmetic(CORE,SESS,&sum)&&sum==CORE+SESS,"CHECKED_ARITHMETIC_EXACT");
 start(&b);a=reserve(&b,1,7,0,0);e=reserve(&b,2,7,1,0);expect(a&&e,"TWO_SESSIONS_WITH_TWO_OPS");
 expect(!close_session(&b,0,1,1),"LIVE_CHILD_PREVENTS_SESSION_FREE");
 expect(reserve(&b,3,8,0,0)==NULL&&b.effects==2,"REPEATED_ADMISSION_OR_THIRD_OP_DENIED");
 if(a){a->callbacks=1;expect(!finish(&b,a,1,7,1),"CALLBACK_BORROWER_PREVENTS_FREE");a->callbacks=0;expect(!finish(&b,a,1,8,1),"STALE_GENERATION");expect(!finish(&b,a,1,7,0),"ATTEMPT_NOT_DESTRUCTION");expect(finish(&b,a,1,7,1)&&!finish(&b,a,1,7,1),"DUPLICATE_RELEASE_DENIED");}
 expect(reserve(&b,1,7,0,0)==NULL,"NO_CLOSED_OPERATION_ID_RESURRECTION");
 a=reserve(&b,3,8,0,0);expect(a!=NULL,"FRESH_ID_REUSE_AFTER_QUIESCENCE");if(a){expect(!finish(&b,a,1,7,1),"LATE_CALLBACK_CANNOT_RETARGET_REUSED_SLOT");expect(finish(&b,a,3,8,1),"NEW_LIFETIME_RELEASE");}
 if(e)expect(finish(&b,e,2,7,1),"SECOND_SESSION_RELEASE");
 b.session_callbacks[0]=1;expect(!close_session(&b,0,1,1),"SESSION_CALLBACK_BORROWER_PREVENTS_FREE");b.session_callbacks[0]=0;
 expect(close_session(&b,0,1,1)&&!close_session(&b,0,1,1)&&close_session(&b,1,1,1),"SESSION_FREE_EXACTLY_ONCE_AFTER_CONTROL_AND_BORROWERS");
 a=reserve(&b,4,9,0,0);expect(a!=NULL&&b.session_generation[0]==2,"NEW_CONTROL_INCARNATION_AFTER_QUIESCENCE");if(a){expect(finish(&b,a,4,9,1)&&!close_session(&b,0,1,1),"OLD_SESSION_CALLBACK_CANNOT_FREE_NEW_INCARNATION");expect(close_session(&b,0,2,1),"NEW_CONTROL_INCARNATION_RELEASE");}
 expect(b.charged==sizeof(b)&&b.host_reserved==0,"ALL_MANAGED_AND_HOST_CREDITS_RECOVERED");
 start(&b);a=reserve(&b,9,7,0,0);expect(a!=NULL,"RETENTION_SETUP");before=b.charged;if(a){expect(retain(a,9,7),"TERMINAL_RETENTION_MODEL");for(i=0;i<1000;++i){if(finish(&b,a,9,7,1)||reserve(&b,10,8,0,0))break;}expect(i==1000&&b.charged==before&&b.effects==1&&a->core!=NULL,"PERSISTENT_RETENTION_NO_EVICTION_REUSE_OR_RESURRECTION");expect(!retain(a,9,8),"LATE_CALLBACK_STALE_GENERATION");expect(!close_session(&b,0,1,1)&&b.charged==before,"RETAINED_CHILD_PRESERVES_SESSION_AND_DEPENDENCY_CREDIT");}
 printf("{\"retained_charged\":%lu,\"retained_op\":9,\"generation\":7,\"class\":\"SYNTHETIC_NO_PST_OUTCOME\",\"containment\":\"process_exit_not_destroyed\"}\n",(unsigned long)b.charged);
 /* Deliberately retained modeled reservation survives until isolated process exit. */
}
static void fairness(void) {
 Budget b;Entry *a,*e;size_t remain[8];unsigned grants[8],cursor,wake,k,ix,control,visits,failed;unsigned char *q;
 start(&b);a=reserve(&b,21,7,0,0);e=reserve(&b,22,7,1,0);if(!a||!e){expect(0,"FAIRNESS_SETUP");return;}
 for(ix=0;ix<8;++ix){remain[ix]=65536;grants[ix]=0;q=(ix<4?a:e)->core+(ix%4)*65536;memset(q,0x3c,65536);}
 memset(b.sessions[0],0x43,803+819);memset(b.sessions[1],0x43,803+819);
 cursor=0;control=0;failed=0;
 for(wake=0;wake<16;++wake){
  /* One bounded CONTROL opportunity then seven round-robin DATA opportunities. */
  ++control;visits=1;
  for(k=0;k<7;++k){ix=cursor;cursor=(cursor+1)%8;++visits;if(remain[ix]){size_t used;used=remain[ix]<16384?remain[ix]:16384;remain[ix]-=used;++grants[ix];}}
  if(visits>8)failed=1;
 }
 for(ix=0;ix<8;++ix)if(remain[ix]||grants[ix]!=4)failed=1;
 expect(!failed&&control==16,"SATURATED_CONTROL_AND_DATA_NO_STARVATION_BOUNDED_8");
 expect(4*65536+65600+148+8*48+16384+65536<=CORE,"SPAN_LAYOUT_FITS_CORE");
 q=a->core+4*65536+65600;memset(q,0x6b,148);
 expect(q[0]==0x6b&&q[147]==0x6b,"CANCEL_FINAL_RESERVED_OUTSIDE_SATURATED_DATA");
 expect(finish(&b,a,21,7,1)&&finish(&b,e,22,7,1)&&close_session(&b,0,1,1)&&close_session(&b,1,1,1),"FAIRNESS_OWNER_RELEASE_EXACTLY_ONCE");
}
int main(void) { printf("{\"sizeof_budget\":%lu,\"sizeof_entry\":%lu,\"managed_limit\":%lu,\"core_reservation\":%lu,\"session_reservation\":%lu,\"app_reservation\":%lu}\n",(unsigned long)sizeof(Budget),(unsigned long)sizeof(Entry),(unsigned long)CAP,(unsigned long)CORE,(unsigned long)SESS,(unsigned long)APP);fairness();cases();printf("{\"assertions\":%d,\"failures\":%d,\"result\":\"%s\",\"scope\":\"C89_PORTABLE_MANAGED_ADMISSION_AND_FAIRNESS_CANDIDATE_NOT_GLOBAL_C2\"}\n",assertions,failures,failures?"FAIL":"PASS");return failures?1:0; }
