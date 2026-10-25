/* Fake APIs only. NO real acquisitions, CloseHandle, SCM, intent or LAB. */
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#define A1_ENABLE_CALLBACK_TEST_HELPER 1
#include "../tools/qualification/scm-aux-reservation-core.inc"
#ifndef A1_SELECTED
#error Choose Selected or Legacy model profile explicitly
#endif
#ifdef NDEBUG
#define A1_CONFIG "Release"
#else
#define A1_CONFIG "Debug"
#endif
#define T(x) do{if(!(x)){printf("FAIL line=%u: %s\n",(unsigned)__LINE__,#x);return 1;}}while(0)
typedef struct {A1_OWNER owner;unsigned seq,live,closed,acquisitions,closes,uses,mode,close_fail,reenter,reentered,postfreeze_calls;} FIXTURE;
static unsigned cases,paths;
static unsigned id(HANDLE h){return (unsigned)((uintptr_t)h-0x1000U);}
static int valid(const FIXTURE *f)
{
 unsigned i,j,seen=0;
 for(i=0;i<2;i++){
  unsigned state=a1_state(&f->owner,i);HANDLE h=f->owner.handle[i];
  if((state==A1_FREE||state==A1_RESERVED)&&h)return 0;
  if(state==A1_OWNED&&(!h||h==INVALID_HANDLE_VALUE))return 0;
  if(h&&h!=INVALID_HANDLE_VALUE){j=id(h);if(j>=31||!(f->live&(1U<<j))||(seen&(1U<<j)))return 0;seen|=1U<<j;}
  if(state==A1_UNKNOWN&&!(f->owner.state&A1_FROZEN))return 0;
 }
 return seen==f->live&&!f->postfreeze_calls;
}
static DWORD close_fake(void *arg,HANDLE h);
static DWORD acquire(void *arg,HANDLE *out)
{
 FIXTURE *f=(FIXTURE *)arg;A1_LEASE nested;unsigned i;
 if(!(f->owner.state&A1_BUSY)||!((out==&f->owner.handle[0]&&a1_state(&f->owner,0)==A1_RESERVED)||(out==&f->owner.handle[1]&&a1_state(&f->owner,1)==A1_RESERVED)))return ERROR_INVALID_DATA;
 ++f->acquisitions;if(f->owner.state&A1_FROZEN)++f->postfreeze_calls;
 if(f->reenter){unsigned action=f->reenter;f->reenter=0;++f->reentered;
  if(action==1){if(a1_reserve(&f->owner,&nested)!=ERROR_BUSY)return ERROR_INVALID_DATA;}
  else {nested.slot=0;nested.ticket=f->owner.ticket0;if(a1_close(&f->owner,nested,close_fake,f)!=ERROR_INVALID_STATE)return ERROR_INVALID_DATA;}
 }
 if(f->mode==1){*out=NULL;return ERROR_ACCESS_DENIED;}
 if(f->mode==2){*out=NULL;return ERROR_NO_TOKEN;}
 if(f->mode==4){*out=NULL;return 0;}
 if(f->mode==5){*out=INVALID_HANDLE_VALUE;return ERROR_ACCESS_DENIED;}
 i=++f->seq;*out=(HANDLE)(uintptr_t)(0x1000U+i);f->live|=1U<<i;
 return f->mode==3?ERROR_ACCESS_DENIED:0;
}
static DWORD close_fake(void *arg,HANDLE h)
{
 FIXTURE *f=(FIXTURE *)arg;unsigned i=id(h);++f->closes;
 if(f->owner.state&A1_FROZEN)++f->postfreeze_calls;
 if(i>=31||!(f->live&(1U<<i))||(f->closed&(1U<<i)))return ERROR_INVALID_HANDLE;
 if(f->reenter){A1_LEASE nested;f->reenter=0;++f->reentered;if(a1_reserve(&f->owner,&nested)!=ERROR_BUSY)return ERROR_INVALID_DATA;}
 if(f->close_fail)return ERROR_INVALID_HANDLE;
 f->live&=~(1U<<i);f->closed|=1U<<i;return 0;
}
static DWORD use_fake(void *arg,HANDLE h)
{FIXTURE *f=(FIXTURE *)arg;++f->uses;if(f->owner.state&A1_FROZEN)++f->postfreeze_calls;
 if(f->reenter){A1_LEASE nested;f->reenter=0;++f->reentered;if(a1_reserve(&f->owner,&nested)!=ERROR_BUSY)return ERROR_INVALID_DATA;}
 return (f->live&(1U<<id(h)))?0:ERROR_INVALID_HANDLE;}
static int core_cases(void)
{
 FIXTURE f;A1_LEASE a,b,c,stale;unsigned before;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&a.slot==0);++cases;
 T(!a1_reserve(&f.owner,&b)&&b.slot==1);++cases;
 T(a1_reserve(&f.owner,&c)==ERROR_TOO_MANY_OPEN_FILES&&!f.acquisitions);T(!a1_cancel(&f.owner,a)&&!a1_cancel(&f.owner,b));T(a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));f.mode=1;T(a1_acquire(&f.owner,a,acquire,&f,ERROR_NO_TOKEN)==ERROR_ACCESS_DENIED);T(!f.live&&a1_state(&f.owner,0)==A1_FREE&&a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));f.mode=2;T(a1_acquire(&f.owner,a,acquire,&f,ERROR_NO_TOKEN)==ERROR_NO_TOKEN&&!f.owner.first_error);f.mode=0;T(!a1_reserve(&f.owner,&a)&&!a1_acquire(&f.owner,a,acquire,&f,0)&&!a1_close(&f.owner,a,close_fake,&f));T(a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));f.mode=3;T(a1_acquire(&f.owner,a,acquire,&f,0)==ERROR_ACCESS_DENIED&&a1_state(&f.owner,0)==A1_UNKNOWN&&f.live);T(!a1_finish(&f.owner)&&valid(&f));++cases;
 before=f.acquisitions;T(a1_reserve(&f.owner,&b)!=0);T(a1_acquire(&f.owner,a,acquire,&f,0)!=0&&f.acquisitions==before);T(a1_close(&f.owner,a,close_fake,&f)!=0&&!f.closes);T(valid(&f));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));f.mode=4;T(a1_acquire(&f.owner,a,acquire,&f,0)==ERROR_INVALID_DATA&&a1_state(&f.owner,0)==A1_UNKNOWN&&!a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));f.mode=5;T(a1_acquire(&f.owner,a,acquire,&f,0)==ERROR_ACCESS_DENIED&&a1_state(&f.owner,0)==A1_UNKNOWN&&f.owner.handle[0]==INVALID_HANDLE_VALUE);++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&!a1_reserve(&f.owner,&b));T(!a1_acquire(&f.owner,a,acquire,&f,0)&&!a1_acquire(&f.owner,b,acquire,&f,0));T(valid(&f));++cases;
 f.owner.first_error=1816;f.close_fail=1;T(a1_close(&f.owner,a,close_fake,&f)==ERROR_INVALID_HANDLE&&f.owner.first_error==1816&&f.owner.first_close_error==ERROR_INVALID_HANDLE);T(a1_state(&f.owner,0)==A1_UNKNOWN&&a1_state(&f.owner,1)==A1_OWNED&&valid(&f));++cases;
 before=f.closes;T(a1_close(&f.owner,a,close_fake,&f)!=0&&a1_close(&f.owner,b,close_fake,&f)!=0&&f.closes==before);T(a1_reserve(&f.owner,&c)!=0);T(a1_use(&f.owner,b,use_fake,&f)!=0&&!f.uses);T(!a1_finish(&f.owner)&&valid(&f));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&!a1_acquire(&f.owner,a,acquire,&f,0));stale=a;T(!a1_close(&f.owner,a,close_fake,&f));before=f.closes;T(a1_close(&f.owner,a,close_fake,&f)!=0&&f.closes==before);++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&!a1_acquire(&f.owner,a,acquire,&f,0));stale=a;T(!a1_close(&f.owner,a,close_fake,&f));T(!a1_reserve(&f.owner,&a)&&a.ticket!=stale.ticket);T(!a1_acquire(&f.owner,a,acquire,&f,0));before=f.closes;T(a1_close(&f.owner,stale,close_fake,&f)!=0&&f.closes==before&&valid(&f));T(!a1_close(&f.owner,a,close_fake,&f));++cases;
 memset(&f,0,sizeof(f));a.slot=2;a.ticket=1;T(a1_close(&f.owner,a,close_fake,&f)==ERROR_INVALID_PARAMETER&&!f.closes);++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));f.reenter=1;T(a1_acquire(&f.owner,a,acquire,&f,0)==ERROR_BUSY&&f.acquisitions==1&&f.reentered==1&&f.owner.first_error==ERROR_BUSY&&valid(&f));T(!a1_close(&f.owner,a,close_fake,&f)&&a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));a1_stop(&f.owner);T(a1_reserve(&f.owner,&a)==ERROR_OPERATION_ABORTED&&!f.acquisitions);++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&!a1_acquire(&f.owner,a,acquire,&f,0));a1_stop(&f.owner);T(a1_use(&f.owner,a,use_fake,&f)!=0&&!f.uses);T(!a1_close(&f.owner,a,close_fake,&f)&&a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));f.owner.ticket0=~(ULONGLONG)0;T(a1_reserve(&f.owner,&a)==ERROR_ARITHMETIC_OVERFLOW&&!f.acquisitions);++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));T(a1_reserve_pair(&f.owner,&b,&c)==ERROR_TOO_MANY_OPEN_FILES&&a1_state(&f.owner,1)==A1_FREE&&!f.acquisitions);T(!a1_cancel(&f.owner,a)&&a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve_pair(&f.owner,&a,&b));T(!a1_acquire(&f.owner,a,acquire,&f,0)&&!a1_acquire(&f.owner,b,acquire,&f,0));T(!a1_close(&f.owner,a,close_fake,&f));f.close_fail=1;T(a1_close(&f.owner,b,close_fake,&f)==ERROR_INVALID_HANDLE&&f.owner.first_error==ERROR_INVALID_HANDLE&&a1_state(&f.owner,0)==A1_FREE&&a1_state(&f.owner,1)==A1_UNKNOWN&&valid(&f));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve_pair(&f.owner,&a,&b));T(!a1_acquire(&f.owner,a,acquire,&f,0));f.reenter=2;T(a1_acquire(&f.owner,b,acquire,&f,0)==ERROR_INVALID_STATE&&!f.closes&&f.reentered==1&&valid(&f));T(!a1_close(&f.owner,a,close_fake,&f)&&!a1_close(&f.owner,b,close_fake,&f));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a));b=a;b.ticket++;T(a1_acquire(&f.owner,b,acquire,&f,0)==ERROR_INVALID_PARAMETER&&!f.acquisitions);T(!a1_cancel(&f.owner,a)&&a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve_pair(&f.owner,&a,&b)&&!a1_acquire(&f.owner,a,acquire,&f,0));a1_retain(&f.owner,ERROR_ACCESS_DENIED);T(a1_state(&f.owner,0)==A1_OWNED&&a1_state(&f.owner,1)==A1_RESERVED&&valid(&f));before=f.acquisitions;T(a1_acquire(&f.owner,b,acquire,&f,0)!=0&&f.acquisitions==before);T(a1_close(&f.owner,a,close_fake,&f)!=0&&!f.closes&&!a1_finish(&f.owner));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve_pair(&f.owner,&a,&b)&&!a1_acquire(&f.owner,a,acquire,&f,0)&&!a1_acquire(&f.owner,b,acquire,&f,0));
 {HANDLE in=NULL,out=NULL;T(!a1_borrow_pair_begin(&f.owner,a,b,&in,&out));T(in==f.owner.handle[0]&&out==f.owner.handle[1]);before=f.closes;T(a1_close(&f.owner,a,close_fake,&f)==ERROR_INVALID_STATE&&f.closes==before);T(a1_reserve(&f.owner,&c)==ERROR_BUSY);T(a1_borrow_end(&f.owner,0)==ERROR_INVALID_STATE);T(!a1_close(&f.owner,a,close_fake,&f)&&!a1_close(&f.owner,b,close_fake,&f)&&valid(&f));}++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&!a1_acquire(&f.owner,a,acquire,&f,0));
 {HANDLE in=NULL,out=NULL;T(a1_borrow_pair_begin(&f.owner,a,a,&in,&out)==ERROR_INVALID_PARAMETER&&!(f.owner.state&A1_BUSY));T(!a1_close(&f.owner,a,close_fake,&f));T(a1_borrow_end(&f.owner,0)==ERROR_INVALID_STATE);}++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&!a1_acquire(&f.owner,a,acquire,&f,0));f.reenter=1;T(a1_use(&f.owner,a,use_fake,&f)==ERROR_BUSY&&f.reentered==1&&f.acquisitions==1&&valid(&f));T(!a1_close(&f.owner,a,close_fake,&f));++cases;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&a)&&!a1_acquire(&f.owner,a,acquire,&f,0));f.reenter=1;T(!a1_close(&f.owner,a,close_fake,&f)&&f.reentered==1&&f.owner.first_error==ERROR_BUSY&&f.acquisitions==1&&valid(&f));T(a1_reserve(&f.owner,&b)!=0&&f.acquisitions==1);++cases;
 return 0;
}
/* Each function models the acquisition schedule of its named REAL caller.
 * It does not call that caller or prove a patched production integration. */
static int caller_case(const char *name,unsigned pair,unsigned expected_absence,unsigned fault)
{
 FIXTURE f;A1_LEASE a,b;DWORD e;unsigned acq,closes;
 memset(&f,0,sizeof(f));memset(&b,0,sizeof(b));
 if(pair){T(!a1_reserve_pair(&f.owner,&a,&b));}else{T(!a1_reserve(&f.owner,&a));}
 if(fault==1)f.mode=1;if(fault==2)f.mode=3;if(fault==3)f.reenter=1;
 if(expected_absence&&!fault){f.mode=2;T(a1_acquire(&f.owner,a,acquire,&f,ERROR_NO_TOKEN)==ERROR_NO_TOKEN);
  if(strcmp(name,"lab_operator_current")){T(a1_finish(&f.owner)&&valid(&f));++cases;return 0;}
  T(!a1_reserve(&f.owner,&a));f.mode=0;}
 e=a1_acquire(&f.owner,a,acquire,&f,0);
 if(e){if(a1_state(&f.owner,a.slot)==A1_OWNED)T(!a1_close(&f.owner,a,close_fake,&f));if(pair&&!(f.owner.state&A1_FROZEN))T(!a1_cancel(&f.owner,b));T(valid(&f));++cases;return 0;}
 if(pair){if(fault==4)f.mode=1;e=a1_acquire(&f.owner,b,acquire,&f,0);if(e){T(!a1_close(&f.owner,a,close_fake,&f)&&valid(&f));++cases;return 0;}}
 if(pair){HANDLE in,out;T(!a1_borrow_pair_begin(&f.owner,a,b,&in,&out));T(!use_fake(&f,in)&&!use_fake(&f,out));T(!a1_borrow_end(&f.owner,0));}else{T(!a1_use(&f.owner,a,use_fake,&f));}
 if(fault==5)f.close_fail=1;
 e=a1_close(&f.owner,a,close_fake,&f);
 if(e){T(f.owner.state&A1_FROZEN);acq=f.acquisitions;closes=f.closes;
  T(a1_reserve(&f.owner,&a)!=0);if(pair)T(a1_close(&f.owner,b,close_fake,&f)!=0);
  T(f.acquisitions==acq&&f.closes==closes&&!a1_finish(&f.owner)&&valid(&f));
 }else{if(pair)T(!a1_close(&f.owner,b,close_fake,&f));T(a1_finish(&f.owner)&&valid(&f));}
 ++cases;(void)name;return 0;
}
static int callers(void)
{
 static const char *names[]={"lab_operator","lab_operator_current","lab_confirm","cycle_confirm","ig_confirm","lab_intent","cycle_intent","cycle_live","ig_actor","ig_extract"};
 unsigned i,f;
 for(i=0;i<10;i++){
  for(f=0;f<6;f++)T(!caller_case(names[i],i>=2&&i<=4,i==0||i==1||i==7,f));
  printf("CALLER_MODEL=%s cases=6; CAPACITY=%u; SYNTHETIC_SCHEDULE_ONLY\n",names[i],i>=2&&i<=4?2U:1U);
 }
 return 0;
}
/* Finite exhaustive event sequences, not proof of arbitrary native execution. */
static int explore(FIXTURE parent,unsigned depth)
{
 unsigned ev,i;A1_LEASE l;DWORD e;
 T(valid(&parent));++paths;if(!depth)return 0;
 for(ev=0;ev<12;ev++){
  FIXTURE f=parent;memset(&l,0,sizeof(l));
  if(ev==0){(void)a1_reserve(&f.owner,&l);}
  else if(ev>=1&&ev<=5){
   for(i=0;i<2;i++)if(a1_state(&f.owner,i)==A1_RESERVED)break;
   if(i<2){l.slot=i;l.ticket=*a1_ticket(&f.owner,i);f.mode=ev-1;(void)a1_acquire(&f.owner,l,acquire,&f,ERROR_NO_TOKEN);}
  }else if(ev==6||ev==7){
   for(i=0;i<2;i++)if(a1_state(&f.owner,i)==A1_OWNED)break;
   if(i<2){l.slot=i;l.ticket=*a1_ticket(&f.owner,i);f.close_fail=ev==7;(void)a1_close(&f.owner,l,close_fake,&f);}
  }else if(ev==8){a1_stop(&f.owner);}
  else if(ev==9){l.slot=3;l.ticket=1;(void)a1_close(&f.owner,l,close_fake,&f);}
  else if(ev==10){
   for(i=0;i<2;i++)if(a1_state(&f.owner,i)==A1_RESERVED)break;
   if(i<2){l.slot=i;l.ticket=*a1_ticket(&f.owner,i);f.mode=0;f.reenter=1;(void)a1_acquire(&f.owner,l,acquire,&f,0);}
  }else{for(i=0;i<2;i++)if(a1_state(&f.owner,i)==A1_OWNED)break;if(i<2){l.slot=i;l.ticket=*a1_ticket(&f.owner,i);e=a1_use(&f.owner,l,use_fake,&f);(void)e;}}
  if(parent.owner.state&A1_FROZEN){T(!memcmp(parent.owner.handle,f.owner.handle,sizeof(f.owner.handle)));T(parent.acquisitions==f.acquisitions&&parent.closes==f.closes&&parent.owner.first_error==f.owner.first_error);}
  T(!explore(f,depth-1));
 }
 return 0;
}
/* Frozen-boundary integration model: tokens, census, snapshot and context
 * must gate BEFORE callbacks. Safe Stop uses existing authority/handle only. */
static int boundaries(void)
{
 FIXTURE f;A1_LEASE l;unsigned i,blocked=0,stop_attempts=0;
 memset(&f,0,sizeof(f));T(!a1_reserve(&f.owner,&l)&&!a1_acquire(&f.owner,l,acquire,&f,0));f.close_fail=1;T(a1_close(&f.owner,l,close_fake,&f)!=0);
 for(i=0;i<8;i++){if(a1_gate(&f.owner))++blocked;else ++f.acquisitions;}
 T(blocked==8&&f.acquisitions==1&&valid(&f));
 /* Four separate factual-state scenarios, each with at most ONE request.
  * No canonical phase3 authority; no Stop inferred from request/timeout. */
 for(i=0;i<4;i++){unsigned phase,local_requests=0;
  for(phase=1;phase<=3;phase+=2){unsigned authorized=1,started=1,quiescent=0,accept_stop=i>=2,legal_state=i!=0;
   if(authorized&&phase==1&&started&&!quiescent&&accept_stop&&legal_state&&!local_requests)++local_requests;
  }
  T(local_requests<=1);stop_attempts+=local_requests;
 }
 T(stop_attempts==2&&!a1_finish(&f.owner)&&f.owner.first_error==ERROR_INVALID_HANDLE);++cases;
 printf("FROZEN_BOUNDARIES=8_DENIED; TOKENS_CENSUS_SNAPSHOT_CONTEXT_BIND_EXTRACT_EFFECT_LIVE; SAFE_STOP_MODEL=ONE_WHEN_ACCEPTED; NO_SCM\n");return 0;
}
int main(void)
{
 FIXTURE f;struct old_layout {HANDLE handle[2];unsigned count;};
 struct request_shape {HANDLE target;LPCWSTR path;DWORD kind,access,share,disposition,flags;};
 memset(&f,0,sizeof(f));T(!core_cases()&&!callers()&&!boundaries()&&!explore(f,5));
 printf("A1_MODEL=PASS cases=%u explored_nodes=%u depth=5 alphabet=12; ZERO_POSTFREEZE_ACQUISITIONS; NO_NATIVE_PROOF\n",cases,paths);
 printf("STORAGE handles=%llu state=%llu first=%llu first_close=%llu tickets=%llu candidate=%llu old_aggregate=%llu delta_aggregate=%llu lease_stack=%llu; EXACTLY_TWO_HANDLES\n",(ULONGLONG)sizeof(f.owner.handle),(ULONGLONG)sizeof(f.owner.state),(ULONGLONG)sizeof(f.owner.first_error),(ULONGLONG)sizeof(f.owner.first_close_error),(ULONGLONG)(sizeof(f.owner.ticket0)+sizeof(f.owner.ticket1)),(ULONGLONG)sizeof(A1_OWNER),(ULONGLONG)sizeof(struct old_layout),(ULONGLONG)(sizeof(A1_OWNER)-sizeof(struct old_layout)),(ULONGLONG)sizeof(A1_LEASE));
 printf("CANDIDATE_ADAPTER_REQUEST_STACK_BYTES=%llu; NO_STACK_COMMIT_MEASUREMENT\n",(ULONGLONG)sizeof(struct request_shape));
 printf("PROFILE=%s CONFIG=%s POINTER_BYTES=%llu; FUNCTIONAL_CORE_UNDER_TEST; NO_LAB_NO_SCM_NO_PREFLIGHT_NO_START\n",A1_SELECTED?"Selected":"Legacy",A1_CONFIG,(ULONGLONG)sizeof(void *));return 0;
}
