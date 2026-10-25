#include "personal_plan_data.h"
#include <string.h>
static int overlaps(const PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *s,size_t n)
{
 const unsigned char *base=(const unsigned char *)p;size_t i;
 for(i=0U;i<sizeof(*p);i++)if(s==base+i||s+n-1U==base+i)return 1;
 return 0;
}
static int match(const PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *id,const unsigned char *plan,size_t n)
{return p&&id&&plan&&n&&n<=PAPACC_PERSONAL_PLAN_MAX&&n==p->bytes&&!memcmp(id,p->attempt,32U)&&!memcmp(plan,p->plan,n);}
int papacc_personal_data_capture(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *id,const unsigned char *plan,size_t n)
{
 size_t i;unsigned any=0U;
 if(!p||p->phase||!id||!plan||!n||n>PAPACC_PERSONAL_PLAN_MAX)return 0;
 /* Aliased input is rejected before copying: no destructive partial capture. */
 if(overlaps(p,id,32U)||overlaps(p,plan,n))return 0;
 for(i=0U;i<32U;i++)any|=id[i];
 if(!any)return 0;
 memcpy(p->attempt,id,32U);memcpy(p->plan,plan,n);p->bytes=n;p->consumed=0U;p->effectful=0U;p->phase=PAPACC_PERSONAL_DATA_CAPTURED;return 1;
}
int papacc_personal_data_present(PAPACC_PERSONAL_PLAN_DATA *p)
{if(!p||p->phase!=PAPACC_PERSONAL_DATA_CAPTURED)return 0;p->phase=PAPACC_PERSONAL_DATA_PRESENTED;return 1;}
int papacc_personal_data_confirm(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *id,const unsigned char *plan,size_t n)
{
 if(!p||p->effectful||p->phase!=PAPACC_PERSONAL_DATA_PRESENTED)return 0;
 if(!match(p,id,plan,n)){p->phase=PAPACC_PERSONAL_DATA_INVALID;return 0;}
 p->phase=PAPACC_PERSONAL_DATA_CONFIRMED;return 1;
}
int papacc_personal_data_consume(PAPACC_PERSONAL_PLAN_DATA *p,const unsigned char *id,const unsigned char *plan,size_t n)
{
 if(!p||p->effectful||p->phase!=PAPACC_PERSONAL_DATA_CONFIRMED||p->consumed)return 0;
 if(!match(p,id,plan,n)){p->phase=PAPACC_PERSONAL_DATA_INVALID;return 0;}
 p->consumed=1U;p->phase=PAPACC_PERSONAL_DATA_CONSUMED;return 1;
}
void papacc_personal_data_invalidate(PAPACC_PERSONAL_PLAN_DATA *p)
{if(p&&p->phase&&p->phase!=PAPACC_PERSONAL_DATA_FINISHED)p->phase=PAPACC_PERSONAL_DATA_INVALID;}
int papacc_personal_data_retire(PAPACC_PERSONAL_PLAN_DATA *p)
{
 volatile unsigned char *b;size_t i;
 if(!p||p->phase!=PAPACC_PERSONAL_DATA_INVALID)return 0;
 b=p->plan;for(i=0U;i<sizeof(p->plan);i++)b[i]=0U;
 p->bytes=0U;p->phase=PAPACC_PERSONAL_DATA_FINISHED;return 1;
}

#include "personal_effect_plan_data.inc"
