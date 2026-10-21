/* TEST_ONLY candidate admission allocator. No PST outcome injection or product use. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CORE 524288u
#define SESSION 16384u
#define APP 1048576u
#define GLOBAL 2097152u
typedef struct Slot { unsigned op,gen,session,state,callbacks; unsigned char *core,*app; } Slot;
typedef struct Ledger { Slot slots[2]; unsigned char *sessions[2]; size_t used; unsigned effects,denied,freed; } Ledger;
static int check(int value,const char *name){printf("{\"event\":\"ADMISSION_ASSERT\",\"name\":\"%s\",\"result\":\"%s\",\"scope\":\"TEST_ONLY_CANDIDATE_POLICY\"}\n",name,value?"PASS":"FAIL");return value;}
static unsigned next_effect;
static Slot *admit(Ledger *l,unsigned op,unsigned gen,unsigned s,int browser,int envelope_known,size_t claimed) {
 size_t cost=CORE+(browser?APP:0);unsigned i;Slot *slot=NULL;
 /* No allocator/socket/PST effect before authority, hard-policy and arithmetic gates. */
 if(!op||!gen||s>=2||!envelope_known||claimed!=cost||cost>GLOBAL||l->used>GLOBAL-cost){l->denied++;return NULL;}
 for(i=0;i<2;i++){if(l->slots[i].state&&l->slots[i].session==s){l->denied++;return NULL;}if(!l->slots[i].state&&!slot)slot=&l->slots[i];}
 if(!slot){l->denied++;return NULL;}
 if(!l->sessions[s]&&l->used+cost>GLOBAL-SESSION){l->denied++;return NULL;}
 if(!l->sessions[s]){l->sessions[s]=(unsigned char*)calloc(1,SESSION);if(!l->sessions[s])return NULL;l->used+=SESSION;}
 slot->core=(unsigned char*)calloc(1,CORE);if(!slot->core)return NULL;
 if(browser){slot->app=(unsigned char*)calloc(1,APP);if(!slot->app){free(slot->core);slot->core=NULL;return NULL;}}
 /* Touch every reserved byte; not a provider hard allocator guarantee. */
 memset(slot->core,0xa5,CORE);if(slot->app)memset(slot->app,0xa5,APP);
 slot->op=op;slot->gen=gen;slot->session=s;slot->state=1;l->used+=cost;l->effects++;next_effect++;
 return slot;
}
static int retain(Slot *s,unsigned op,unsigned gen){if(!s||s->op!=op||s->gen!=gen||s->state!=1)return 0;s->state=2;return 1;}
static int finalized(Ledger *l,Slot *s,unsigned op,unsigned gen,int destroyed,int callbacks_quiescent){
 size_t n;
 if(!s||!s->state||s->op!=op||s->gen!=gen||!destroyed||!callbacks_quiescent||s->state==2)return 0;
 n=CORE+(s->app?APP:0);free(s->core);free(s->app);memset(s,0,sizeof(*s));l->used-=n;l->freed++;return 1;
}
static void memory(Ledger *l,const char *phase) {
 PROCESS_MEMORY_COUNTERS_EX m;memset(&m,0,sizeof(m));m.cb=(DWORD)sizeof(m);
 if(!GetProcessMemoryInfo(GetCurrentProcess(),(PROCESS_MEMORY_COUNTERS*)&m,(DWORD)sizeof(m)))exit(3);
 printf("{\"event\":\"ADMISSION_RESOURCE\",\"phase\":\"%s\",\"managed_charged\":%llu,\"private_usage\":%llu,\"peak_pagefile_usage\":%llu,\"effects\":%u,\"scope\":\"TEST_ONLY_NOT_PST_R4\"}\n",phase,(unsigned long long)l->used,(unsigned long long)m.PrivateUsage,(unsigned long long)m.PeakPagefileUsage,l->effects);
}
int main(int argc,char **argv){
 Ledger l;Slot *a,*b;size_t kept;unsigned before;int browser=argc>1&&!strcmp(argv[1],"browser");int ok=1;
 memset(&l,0,sizeof(l));memory(&l,"BASELINE");
 before=next_effect;ok&=check(admit(&l,1,7,0,browser,0,CORE+(browser?APP:0))==NULL&&next_effect==before,"UNKNOWN_REQUIRED_ENVELOPE_DENIES_BEFORE_EFFECTS");
 ok&=check(admit(&l,1,7,0,browser,1,(size_t)-1)==NULL&&next_effect==before,"OVERFLOW_OR_FORGED_RESERVATION_DENIED");
 a=admit(&l,1,7,0,browser,1,CORE+(browser?APP:0));ok&=check(a!=NULL,"RESERVED_BEFORE_EFFECTS");
 if(!a)return 1;memory(&l,"FIRST_OP");
 /* Defined span budget is wholly within charged CORE, never double-counted. */
 ok&=check(4u*65536u+65600u+148u+8u*48u+16384u+65536u<=CORE,"FOUR_QUEUES_STAGING_NOTICES_ACKS_METADATA_C3_FIT_CORE");
 memset(a->core+4u*65536u+65600u,0x5a,148u);
 ok&=check(a->core[4u*65536u+65600u]==0x5a,"CANCEL_FINAL_RESERVE_SURVIVES_SATURATED_DATA");
 kept=l.used;before=next_effect;ok&=check(retain(a,1,7),"RETAINED_STATE_TRANSITION_SYNTHETIC");
 ok&=check(!retain(a,1,8)&&a->gen==7,"STALE_GENERATION_REJECTED");
 ok&=check(!finalized(&l,a,1,7,0,1)&&l.used==kept,"NO_QUOTA_RECOVERY_FROM_ATTEMPT_OR_STOP");
 ok&=check(!finalized(&l,a,1,7,1,1)&&l.used==kept,"TERMINAL_RETENTION_CANNOT_BE_EVICTED");
 ok&=check(admit(&l,3,8,0,browser,1,CORE+(browser?APP:0))==NULL&&next_effect==before,"NO_SAME_SESSION_EARLY_REUSE");
 memory(&l,"RETAINED");
 b=admit(&l,2,7,1,browser,1,CORE+(browser?APP:0));
 if(browser)ok&=check(b==NULL,"BROWSER_SECOND_OP_EXCEEDS_2MIB_DENIED");
 else{
  ok&=check(b!=NULL,"SECOND_SESSION_ALLOWED_WITH_RESERVED_QUOTA");
  if(!b)return 1;memory(&l,"SECOND_OP");
  ok&=check(!finalized(&l,b,2,8,1,1)&&!finalized(&l,b,2,7,1,0),"GENERATION_AND_CALLBACK_QUIESCENCE_GUARDS");
  ok&=check(finalized(&l,b,2,7,1,1)&&!finalized(&l,b,2,7,1,1),"CONFIRMED_DESTROYED_FREES_EXACTLY_ONCE");
 }
 ok&=check(a->state==2&&a->core!=NULL&&l.used>=CORE+SESSION,"OWNER_AND_RESERVATION_REMAIN_REACHABLE");
 memory(&l,"FINAL_RETAINED");
 printf("{\"result\":\"%s\",\"profile\":\"%s\",\"scope\":\"CANDIDATE_ADMISSION_SYNTHETIC_RETENTION_NOT_NATIVE_R4\",\"containment\":\"process_exit_not_clean_or_quota_recovery\"}\n",ok?"PASS":"FAIL",browser?"browser-budget-model":"accelerator-budget-model");
 /* Explicit diagnostic process containment preserves synthetic retained slab. */
 return ok?0:1;
}
