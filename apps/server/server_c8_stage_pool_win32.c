#include "server_c8_stage_pool_win32.h"
#include <windows.h>
#include <string.h>
static int pool_owner(const PAPACC_C8_STAGE_POOL*p)
{return p!=NULL&&p->initialized==PAPACC_TRUE&&p->finalized==PAPACC_FALSE&&p->thread==GetCurrentThreadId();}
static int key_valid(const PAPACC_C8_STAGE_KEY*k)
{
 if(k==NULL||!k->owner||!k->session||!k->connection||k->role>2U)return 0;
 if(!k->operation||!k->generation)return !k->operation&&!k->generation&&!k->role;
 return k->role==1U||k->role==2U;
}
static int key_equal(const PAPACC_C8_STAGE_KEY*a,const PAPACC_C8_STAGE_KEY*b)
{return a->owner==b->owner&&a->session==b->session&&a->connection==b->connection&&a->operation==b->operation&&a->generation==b->generation&&a->role==b->role;}
static PAPACC_C8_STAGE* find_stage(PAPACC_C8_STAGE_POOL*p,const PAPACC_C8_STAGE_LEASE*h)
{
 PAPACC_C8_STAGE*s;
 if(!pool_owner(p)||h==NULL||h->root!=p->root||h->index>=PAPACC_C8_STAGE_COUNT||!h->serial)return NULL;
 s=&p->stages[h->index];
 if(s->state==PAPACC_C8_STAGE_FREE||s->serial!=h->serial||!key_equal(&s->key,&h->key))return NULL;
 return s;
}
PAPACC_RESULT papacc_c8_stage_pool_init(PAPACC_C8_STAGE_POOL*p,PAPACC_U64 root,PAPACC_SIZE credit)
{
 if(p==NULL||root==0U)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(p->initialized!=PAPACC_FALSE)return PAPACC_RESULT_INVALID_STATE;
 if(credit<sizeof(*p)||credit>PAPACC_C8_STORAGE_LIMIT)return PAPACC_RESULT_LIMIT_EXCEEDED;
 memset(p,0,sizeof(*p));p->root=root;p->next_serial=1U;p->thread=GetCurrentThreadId();
 p->reserved_bytes=sizeof(*p);p->initialized=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_stage_acquire(PAPACC_C8_STAGE_POOL*p,const PAPACC_C8_STAGE_KEY*k,PAPACC_C8_STAGE_LEASE*out)
{
 unsigned i;PAPACC_C8_STAGE*s;
 if(out==NULL||!key_valid(k))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!pool_owner(p)||p->stopping)return PAPACC_RESULT_INVALID_STATE;
 /* A live connection has one RX stage, irrespective of an attempted new key.
  * Never overwrite another generation/owner's pending or retained bytes. */
 for(i=0U;i<PAPACC_C8_STAGE_COUNT;++i){s=&p->stages[i];
  if(s->state!=PAPACC_C8_STAGE_FREE&&s->key.connection==k->connection)return PAPACC_RESULT_INVALID_STATE;}
 if(p->next_serial==(PAPACC_U64)-1||p->used>=PAPACC_C8_STAGE_COUNT)return PAPACC_RESULT_LIMIT_EXCEEDED;
 for(i=0U;i<PAPACC_C8_STAGE_COUNT;++i){s=&p->stages[i];if(s->state==PAPACC_C8_STAGE_FREE){
  memset(s,0,sizeof(*s));s->key=*k;s->serial=p->next_serial++;s->state=PAPACC_C8_STAGE_ACTIVE;
  out->root=p->root;out->serial=s->serial;out->key=*k;out->index=i;
  ++p->used;if(p->used>p->high_water)p->high_water=p->used;return PAPACC_RESULT_OK;}}
 return PAPACC_RESULT_LIMIT_EXCEEDED;
}
PAPACC_RESULT papacc_c8_stage_borrow(PAPACC_C8_STAGE_POOL*p,const PAPACC_C8_STAGE_LEASE*h,PAPACC_C8_STAGE_BORROW*ticket,PAPACC_U8**out,PAPACC_SIZE*capacity)
{
 PAPACC_C8_STAGE*s;
 if(out==NULL||capacity==NULL||ticket==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 *out=NULL;*capacity=0U;s=find_stage(p,h);
 if(s==NULL||p->stopping||s->state!=PAPACC_C8_STAGE_ACTIVE)return PAPACC_RESULT_INVALID_STATE;
 if(s->borrowers)return PAPACC_RESULT_INVALID_STATE;
 if(s->borrow_ticket==(PAPACC_U64)-1)return PAPACC_RESULT_LIMIT_EXCEEDED;
 ++s->borrow_ticket;s->borrowers=1U;ticket->lease=*h;ticket->ticket=s->borrow_ticket;
 *out=s->bytes;*capacity=sizeof(s->bytes);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_stage_return(PAPACC_C8_STAGE_POOL*p,const PAPACC_C8_STAGE_BORROW*h)
{
 PAPACC_C8_STAGE*s;
 if(h==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 s=find_stage(p,&h->lease);
 if(s==NULL||s->borrowers!=1U||!h->ticket||s->borrow_ticket!=h->ticket)return PAPACC_RESULT_INVALID_STATE;
 /* Completing a preexisting borrow never returns retained capacity to admission. */
 --s->borrowers;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_stage_cancel(PAPACC_C8_STAGE_POOL*p,const PAPACC_C8_STAGE_LEASE*h)
{
 PAPACC_C8_STAGE*s=find_stage(p,h);
 if(s==NULL||s->state==PAPACC_C8_STAGE_RETAINED)return PAPACC_RESULT_INVALID_STATE;
 s->state=PAPACC_C8_STAGE_QUIESCING;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_stage_retain(PAPACC_C8_STAGE_POOL*p,const PAPACC_C8_STAGE_LEASE*h)
{
 PAPACC_C8_STAGE*s=find_stage(p,h);
 if(s==NULL)return PAPACC_RESULT_INVALID_STATE;
 s->state=PAPACC_C8_STAGE_RETAINED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_stage_release(PAPACC_C8_STAGE_POOL*p,const PAPACC_C8_STAGE_LEASE*h,PAPACC_BOOL users_finalized)
{
 PAPACC_C8_STAGE*s=find_stage(p,h);volatile PAPACC_U8*bytes;PAPACC_SIZE i;
 if(s==NULL||s->state!=PAPACC_C8_STAGE_QUIESCING||s->borrowers||users_finalized!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 bytes=s->bytes;for(i=0U;i<sizeof(s->bytes);++i)bytes[i]=0U;
 memset(&s->key,0,sizeof(s->key));s->serial=0U;s->state=PAPACC_C8_STAGE_FREE;--p->used;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_stage_stop(PAPACC_C8_STAGE_POOL*p)
{
 unsigned i;if(!pool_owner(p))return PAPACC_RESULT_INVALID_STATE;
 p->stopping=PAPACC_TRUE;
 for(i=0U;i<PAPACC_C8_STAGE_COUNT;++i)if(p->stages[i].state==PAPACC_C8_STAGE_ACTIVE)p->stages[i].state=PAPACC_C8_STAGE_QUIESCING;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_stage_pool_finish(PAPACC_C8_STAGE_POOL*p)
{
 if(!pool_owner(p)||!p->stopping||p->used!=0U)return PAPACC_RESULT_INVALID_STATE;
 p->finalized=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
