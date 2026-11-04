#include <stdio.h>
#include <stddef.h>
typedef unsigned long DWORD;typedef unsigned long PAPACC_U32;typedef unsigned __int64 PAPACC_U64;typedef unsigned char PAPACC_U8;typedef int PAPACC_C8_RESOURCE_REASON;
#define PAPACC_C8_RESOURCE_OK 0
#define PAPACC_C8_RESOURCE_STATE_ERROR 1
#define PAPACC_C8_RESOURCE_UNKNOWN 2
typedef struct {int unused;} PAPACC_C8_TARGET_PRODUCER;
typedef struct {int configured,current;PAPACC_U64 invocation,owner,revision,epoch;int stopping;} PAPACC_C8_TARGET_BINDING;
static int raw_result;static PAPACC_C8_RESOURCE_REASON papacc_c8_target_validate_raw(const PAPACC_C8_TARGET_PRODUCER*s,const PAPACC_C8_TARGET_BINDING*b,const PAPACC_U8*t,PAPACC_U64 now){(void)s;(void)b;(void)t;(void)now;return raw_result;}
static DWORD tick;static DWORD GetTickCount(void){return tick;}
static long c2_managed_bytes;static struct {int provider_known,kernel_known,host_approved;DWORD measured_at,fresh_ms;size_t available_credit;} c2_target;
static int c2_capacity(void) {
 return c2_target.provider_known && c2_target.kernel_known && c2_target.host_approved &&
  c2_target.fresh_ms && (DWORD)(GetTickCount()-c2_target.measured_at)<c2_target.fresh_ms &&
  c2_target.available_credit>=(size_t)c2_managed_bytes;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_target_admission(const PAPACC_C8_TARGET_PRODUCER *s,const PAPACC_C8_TARGET_BINDING *b,const PAPACC_U8 *target,PAPACC_U64 now)
{
 PAPACC_C8_RESOURCE_REASON result=papacc_c8_target_validate_raw(s,b,target,now);
 if(result!=PAPACC_C8_RESOURCE_OK)return result;
 if(!b->configured||!b->current||!b->invocation||!b->owner||!b->revision||!b->epoch||b->stopping)return PAPACC_C8_RESOURCE_STATE_ERROR;
 /* No evidence-backed provider/kernel recipe or empirical approval is installed.
  * Raw process totals and pool observations cannot authorize native effects. */
 return PAPACC_C8_RESOURCE_UNKNOWN;
}
static unsigned checks;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL %u\n",checks);return 1;}}while(0)
int main(void){unsigned mask;PAPACC_C8_TARGET_PRODUCER s={0};PAPACC_U8 t=0;PAPACC_C8_TARGET_BINDING b={1,1,1,1,1,1,0};
 CHECK(!c2_capacity());
 for(mask=0;mask<32;++mask){c2_target.provider_known=(mask&1)!=0;c2_target.kernel_known=(mask&2)!=0;c2_target.host_approved=(mask&4)!=0;c2_target.fresh_ms=(mask&8)?10:0;c2_target.measured_at=100;c2_target.available_credit=(mask&16)?100:99;c2_managed_bytes=100;tick=100;CHECK(c2_capacity()==(mask==31));}
 tick=110;CHECK(!c2_capacity());tick=109;CHECK(c2_capacity());tick=99;CHECK(!c2_capacity());
 raw_result=7;CHECK(papacc_c8_target_admission(&s,&b,&t,100)==7);raw_result=0;CHECK(papacc_c8_target_admission(&s,&b,&t,100)==PAPACC_C8_RESOURCE_UNKNOWN);
 b.stopping=1;CHECK(papacc_c8_target_admission(&s,&b,&t,100)==PAPACC_C8_RESOURCE_STATE_ERROR);b.stopping=0;b.current=0;CHECK(papacc_c8_target_admission(&s,&b,&t,100)==PAPACC_C8_RESOURCE_STATE_ERROR);
 printf("PASS checks=%u ACTUAL_EXTRACTED_BOUNDARIES SYNTHETIC_RAW_CLOCK_ONLY NO_NETWORK_NO_AUTHORITY\n",checks);return 0;}
