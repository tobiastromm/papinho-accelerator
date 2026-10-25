#ifndef PAPACC_N0_PROTECTED_ROOT_CORE_H
#define PAPACC_N0_PROTECTED_ROOT_CORE_H
/* Private engineering procedure, not an Accelerator admission/credit owner. */
#include <windows.h>
typedef struct N0_ROOT_OPS {
 void *context;
 int (*stop)(void *);
 DWORD (*parent)(void *,unsigned);
 DWORD (*absent)(void *,unsigned);
 DWORD (*create)(void *,unsigned);
 DWORD (*open)(void *,unsigned);
 DWORD (*verify)(void *,unsigned);
 DWORD (*finish)(void *,unsigned);
 DWORD (*close)(void *);
} N0_ROOT_OPS;
typedef struct N0_ROOT_FLOW {
 unsigned used,created,verified,finished;
 DWORD first,close_error;
 const char *stage;
} N0_ROOT_FLOW;
static DWORD n0_root_run(N0_ROOT_FLOW *s,const N0_ROOT_OPS *o)
{
 unsigned i,j;DWORD e=0;DWORD (*steps[6])(void *,unsigned);
 static const char *names[]={"PARENT","ABSENCE","CREATE","OPEN_ORIGINAL","VERIFY","CLOSE_ACQUISITION"};
 if(s->used)return ERROR_INVALID_STATE;
 s->used=1;steps[0]=o->parent;steps[1]=o->absent;steps[2]=o->create;
 steps[3]=o->open;steps[4]=o->verify;steps[5]=o->finish;
 for(i=0;i<3&&!e;i++)for(j=0;j<6;j++){
  s->stage=names[j];if(o->stop(o->context)){e=ERROR_CANCELLED;break;}
  e=steps[j](o->context,i);if(e)break;
  if(j==2)s->created|=1U<<i;
  if(j==4)s->verified|=1U<<i;
  if(j==5)s->finished|=1U<<i;
 }
 s->first=e;s->close_error=o->close(o->context);
 if(!s->first&&s->close_error){s->first=s->close_error;s->stage="CLOSE_RETAINED";}
 return s->first;
}
#endif
