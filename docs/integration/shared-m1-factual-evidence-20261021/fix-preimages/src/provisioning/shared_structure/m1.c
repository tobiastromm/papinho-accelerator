#include "m1_internal.h"
#include <stddef.h>
static void failure(SP_M1_STATE *s,int code)
{
 if(!s->first_failure_code){s->first_failure_stage=s->stage;s->first_failure_code=code;}
}
static int abort_operation(SP_M1_STATE *s,const SP_M1_BINDING *b,void *owner,int result,int code)
{
 failure(s,code);
 if(code==SP_M1_UNCERTAIN_DETAIL || b->quiescent(owner)!=1){s->state=2;return SP_M1_RETAINED;}
 b->wipe(owner);s->state=3;return result;
}
static int valid(const SP_M1_BINDING *b)
{
 return b && b->perform && b->stopped && b->quiescent && b->wipe && b->publish;
}
int sp_m1_execute(SP_M1_STATE *s,const SP_M1_BINDING *b,void *owner)
{
 static const SP_M1_STAGE order[]={SP_M1_SIGNATURE,SP_M1_TRUST,SP_M1_KDF,
  SP_M1_AEAD,SP_M1_CONTENT,SP_M1_RECHECK_TRUST,SP_M1_RELEASE};
 size_t j;int result,detail;
 if(!s || !owner || !valid(b) || s->state)return SP_M1_INVALID;
 s->owner_identity=owner;s->binding_identity=b;
 s->state=1;s->stage=SP_M1_STRUCTURE;s->first_failure_stage=SP_M1_NONE;s->first_failure_code=0;
 s->kdf_invocations=s->aead_invocations=s->content_invocations=0;
 detail=SP_M1_INVALID_DETAIL;result=b->perform(owner,s->stage,&detail);
 if(s->state!=1 || s->stage!=SP_M1_STRUCTURE){s->stage=SP_M1_STRUCTURE;failure(s,SP_M1_INVALID_DETAIL);s->state=3;return SP_M1_INVALID;}
 if(result!=SP_M1_COMPLETE){failure(s,detail);s->state=3;return result;}
 s->stage=SP_M1_BUDGET;detail=SP_M1_INVALID_DETAIL;result=b->perform(owner,s->stage,&detail);
 if(s->state!=1 || s->stage!=SP_M1_BUDGET){s->stage=SP_M1_BUDGET;failure(s,SP_M1_INVALID_DETAIL);s->state=3;return SP_M1_INVALID;}
 if(result!=SP_M1_COMPLETE){failure(s,detail);s->state=3;return result;}
 /* Workspaces acquired only after structural and budget validation. */
 b->wipe(owner);
 for(j=0;j<sizeof(order)/sizeof(order[0]);++j){
  s->stage=order[j];
  if(s->stage==SP_M1_CONTENT){
   detail=SP_M1_INVALID_DETAIL;result=b->perform(owner,SP_M1_CONTENT_PARSE,&detail);
   if(s->state!=1 || s->stage!=SP_M1_CONTENT){s->state=1;s->stage=SP_M1_CONTENT;return abort_operation(s,b,owner,SP_M1_DENIED,SP_M1_INVALID_DETAIL);}
   if(result!=SP_M1_COMPLETE)return abort_operation(s,b,owner,result,detail);
  }
  if(b->stopped(owner))return abort_operation(s,b,owner,SP_M1_STOPPED,SP_M1_STOPPED);
  if(s->stage==SP_M1_KDF)++s->kdf_invocations;
  if(s->stage==SP_M1_AEAD)++s->aead_invocations;
  if(s->stage==SP_M1_CONTENT)++s->content_invocations;
  detail=SP_M1_INVALID_DETAIL;result=b->perform(owner,s->stage,&detail);
  /* Callbacks cannot advance/reset a running machine. */
  if(s->state!=1 || s->stage!=order[j]){
   s->state=1;s->stage=order[j];return abort_operation(s,b,owner,SP_M1_DENIED,SP_M1_INVALID_DETAIL);
  }
  if(result!=SP_M1_COMPLETE)return abort_operation(s,b,owner,result,detail);
  if(b->stopped(owner))return abort_operation(s,b,owner,SP_M1_STOPPED,SP_M1_STOPPED);
 }
 if(b->quiescent(owner)!=1)return abort_operation(s,b,owner,SP_M1_RETAINED,SP_M1_UNCERTAIN_DETAIL);
 if(b->stopped(owner))return abort_operation(s,b,owner,SP_M1_STOPPED,SP_M1_STOPPED);
 /* Sole controlled publish. Private binding, not an authority callback API. */
 b->publish(owner);b->wipe(owner);s->state=3;return SP_M1_COMPLETE;
}
int sp_m1_finish(SP_M1_STATE *s,const SP_M1_BINDING *b,void *owner)
{
 if(!s || !owner || !valid(b) || s->state!=2 || s->owner_identity!=owner || s->binding_identity!=b)return SP_M1_INVALID;
 if(b->quiescent(owner)!=1)return SP_M1_RETAINED;
 b->wipe(owner);s->state=3;return SP_M1_ABORTED;
}
