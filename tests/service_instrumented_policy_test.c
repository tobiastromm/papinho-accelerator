#ifdef NDEBUG
#undef NDEBUG
#endif
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include "../tools/qualification/scm-instrumented-policy.h"
static IG_SAMPLE good(void)
{
 IG_SAMPLE s;unsigned i;ZeroMemory(&s,sizeof(s));s.oldest=100;s.identity=IG_ID_ALL;s.count=3;s.physical=5*IG_GIB;s.commit=9*IG_GIB;
 for(i=0;i<3;i++){s.process[i].powershell=i<2;s.process[i].private_bytes=4*IG_MIB;s.process[i].commit=4*IG_MIB;s.process[i].working_set=8*IG_MIB;s.process[i].stack=128*1024;s.process[i].handles=80;s.process[i].threads=4;}return s;
}
int main(void)
{
 IG_SAMPLE s;IG_GATE g;unsigned bit,cases=0;
 ZeroMemory(&s,sizeof(s));ZeroMemory(&g,sizeof(g));assert(ig_preflight(&g,&s,0)!=0&&!g.preflight_valid&&!g.consumed);cases++;
 ZeroMemory(&g,sizeof(g));assert(ig_consume(&g)!=0&&!g.consumed);cases++; /* no factual budget/admission => no consumption */
 for(bit=1;bit<=256;bit<<=1){s=good();s.identity^=bit;assert(ig_metric(&s,1)!=0);cases++;} /* host boot service hash config dependencies operator native lock */
 s=good();assert(ig_metric(&s,1)==0);s.physical--;assert(ig_metric(&s,1)!=0);s=good();s.commit--;assert(ig_metric(&s,1)!=0);cases+=3;
 s=good();s.count=5;assert(ig_metric(&s,1)!=0);s=good();s.process[2].handles=129;assert(ig_metric(&s,1)!=0);s=good();s.process[0].threads=65;assert(ig_metric(&s,1)!=0);s=good();s.process[2].stack=8*IG_MIB+1;assert(ig_metric(&s,1)!=0);cases+=4;
 s=good();s.process[1].commit=256*IG_MIB+1;assert(ig_metric(&s,1)!=0);s=good();s.process[2].error=ERROR_ACCESS_DENIED;assert(ig_metric(&s,1)==ERROR_ACCESS_DENIED);cases+=2;
 ZeroMemory(&g,sizeof(g));s=good();assert(ig_preflight(&g,&s,60101)==ERROR_TIMEOUT);assert(g.first==ERROR_TIMEOUT);assert(ig_preflight(&g,&s,99)==ERROR_TIMEOUT);cases+=2;
 ZeroMemory(&g,sizeof(g));s=good();assert(!ig_preflight(&g,&s,100));assert(ig_preflight(&g,&s,100)!=0);cases++;
 ZeroMemory(&g,sizeof(g));s=good();assert(!ig_preflight(&g,&s,100));assert(ig_final(&g,&s,1101)==ERROR_TIMEOUT);assert(!g.consumed);cases++;
 ZeroMemory(&g,sizeof(g));s=good();assert(!ig_preflight(&g,&s,100));assert(!ig_final(&g,&s,1100));assert(!ig_consume(&g));assert(ig_consume(&g)!=0);assert(!ig_start(&g,&s,1100));assert(ig_start(&g,&s,1100)!=0);assert(!ig_retire(&g));assert(!ig_stop(&g));assert(!ig_retire(&g));assert(ig_stop(&g)!=0);g.quiescent=1;assert(ig_retire(&g));g.retained=1;assert(!ig_retire(&g));cases+=8;
 ZeroMemory(&g,sizeof(g));s=good();assert(!ig_preflight(&g,&s,100));assert(!ig_consume(&g));assert(ig_start(&g,&s,1101)==ERROR_TIMEOUT);assert(!g.starts);cases++;
 ZeroMemory(&g,sizeof(g));s=good();assert(!ig_preflight(&g,&s,100));assert(!ig_consume(&g));assert(!ig_start(&g,&s,100));s.oldest=1200;assert(ig_monitor(&g,&s,1200)==ERROR_TIMEOUT);assert(g.first==ERROR_TIMEOUT);cases++;
 ZeroMemory(&g,sizeof(g));s=good();assert(!ig_preflight(&g,&s,100));assert(!ig_consume(&g));assert(!ig_start(&g,&s,100));s.oldest=200;s.physical=4*IG_GIB;s.commit=8*IG_GIB;assert(!ig_monitor(&g,&s,200));s.physical--;assert(ig_monitor(&g,&s,300)==ERROR_NOT_ENOUGH_MEMORY);assert(!ig_retire(&g));cases+=2;
 printf("INSTRUMENTED_POLICY_MODEL=PASS cases=%u; IDENTITY_HASH_BOOT_CONFIG_SERVICE_RESOURCES_FRESHNESS_CONSUMED_STOP_RETENTION_UNKNOWN; NO_FILE_NO_SCM_NO_START\n",cases);return 0;
}
