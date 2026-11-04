#include "personal_scq_inputs_win32.h"
/* Accepted SQF1 F0-F7. Only negative DATA: never ack/close/admission. */
#define PAPACC_SCQ_FAILURE_BYTES 56U
#define PAPACC_SCQ_FAILURE_STAGE_MAX 24U
static DWORD scqf_u32(const BYTE *p)
{return (DWORD)p[0]|((DWORD)p[1]<<8)|((DWORD)p[2]<<16)|((DWORD)p[3]<<24);}
static void scqf_put(BYTE *p,DWORD v)
{unsigned i;for(i=0;i<4U;i++)p[i]=(BYTE)(v>>(8U*i));}
static int scqf_frame(const BYTE *p,DWORD bytes,const BYTE attempt[32])
{
 return p&&attempt&&bytes==PAPACC_SCQ_FAILURE_BYTES&&!memcmp(p,"SQF1",4)&&
 scqf_u32(p+4)==PAPACC_SCQ_FAILURE_BYTES&&scqf_u32(p+8)>0U&&
 scqf_u32(p+8)<=PAPACC_SCQ_FAILURE_STAGE_MAX&&scqf_u32(p+12)>0U&&
 scqf_u32(p+16)<=2U&&(scqf_u32(p+16)||!scqf_u32(p+20))&&!memcmp(p+24,attempt,32);
}
static void scqf_freeze(PAPACC_SCQ_TRANSPORT *t,DWORD stage,DWORD error)
{
 if(!t||t->failure_stage)return;
 t->failure_stage=stage;t->failure_error=error?error:ERROR_ACCESS_DENIED;
 /* Composite validation returns are not invented native statuses. */
 t->failure_kind=t->failure_raw=0;
 memcpy(t->failure_frame,"SQF1",4);scqf_put(t->failure_frame+4,56U);
 scqf_put(t->failure_frame+8,t->failure_stage);scqf_put(t->failure_frame+12,t->failure_error);
 scqf_put(t->failure_frame+16,0U);scqf_put(t->failure_frame+20,0U);
 memcpy(t->failure_frame+24,t->capture_carrier.attempt,32);
}
/* F1 catalog refinement, same SQF1/56 bytes and same original cached returns.
 * 20 own-count query;21/22/23 ReOpenFile(request/decision/image);24 final check.
 * No native call, no new cursor or inference from error5 alone. */
static void scqf_freeze_inputs(PAPACC_SCQ_TRANSPORT *t,DWORD stage,DWORD error,const PAPACC_PERSONAL_SCQ_INPUTS *in)
{
 DWORD refined=stage,kind=0U,raw=0U;
 if(!t||t->failure_stage)return;
 if(stage==10U&&in){
  if(in->first==31U){refined=20U;
   /* The same substage also synthesizes quota1816: not a native query error. */
   if(in->error&&in->error==error&&error!=ERROR_NOT_ENOUGH_QUOTA){kind=2U;raw=in->error;}}
  else if(in->first==15U){
   /* Readers are acquired in order; the failed slot is cleared immediately.
    * Reject inconsistent bookkeeping rather than inventing a file index. */
   if(!in->reader[0]&&!in->reader[1]&&!in->reader[2])refined=21U;
   else if(in->reader[0]&&!in->reader[1]&&!in->reader[2])refined=22U;
   else if(in->reader[0]&&in->reader[1]&&!in->reader[2])refined=23U;
   if(refined!=stage&&in->error&&in->error==error){kind=2U;raw=in->error;}
  }else if(error==ERROR_ACCESS_DENIED&&in->first)refined=24U;
 }
 scqf_freeze(t,refined,error);
 t->failure_kind=kind;t->failure_raw=raw;
 scqf_put(t->failure_frame+16,kind);scqf_put(t->failure_frame+20,raw);
}
static DWORD scqf_received(PAPACC_SCQ_SESSION_DRIVER *c)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_TRANSPORT *t=&c->transport;
 if(c->role!=1U||!t->query_refs3||!t->resource_scope||!t->resource_scope->p1_handles||
 !c->profile||!c->profile->pre_readiness||c->phase!=3U||!t->delivery_process||
 t->failure_reported||t->first||c->first||t->phase!=1U||t->pending||t->operation!=2U||t->offset!=56U||
 scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p)||
 p.purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||!scqf_frame(c->buffer,56U,p.attempt)||
 !scqt_process(t->delivery_process,&p.runtime)||!scqt_process(c->runtime->process,&p.runtime))
 return scqt_fail(c,ERROR_INVALID_DATA);
 /* start_io/continuation already validate original pipe/runtime; repeat its
  * kernel peer correlation here before accepting negative metadata. */
 {DWORD pid;if(!GetNamedPipeClientProcessId(t->pipe,&pid)||pid!=p.runtime.pid)return scqt_fail(c,ERROR_ACCESS_DENIED);}
 t->failure_reported=1U;t->failure_stage=scqf_u32(c->buffer+8);
 t->failure_error=scqf_u32(c->buffer+12);t->failure_kind=scqf_u32(c->buffer+16);t->failure_raw=scqf_u32(c->buffer+20);
 (void)fprintf(stderr,"RECEIVER_REPORTED_FIRST_FAILURE stage=%lu win32=%lu native_kind=%lu raw=0x%08lx; NEGATIVE_DATA_ONLY; NOT_CONTROLLER_NATIVE_ERROR; NO_RECEIPT_NO_CLOSE_NO_AUTHORITY\n",
 (unsigned long)t->failure_stage,(unsigned long)t->failure_error,(unsigned long)t->failure_kind,(unsigned long)t->failure_raw);
 return scqt_fail(c,ERROR_ACCESS_DENIED); /* remote code is DATA, never loop control */
}
