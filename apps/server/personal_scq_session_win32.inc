#include "personal_scq_session_win32.h"
/* DATA composition; transport custody never grants KSP admission. */
static DWORD scqt_close(PAPACC_SCQ_SESSION_DRIVER *,const void *);
static DWORD scqt_stop(PAPACC_SCQ_SESSION_DRIVER *,const void *);
static int scqs_owned(const PAPACC_SCQ_SESSION_DRIVER *c,const void *owner)
{return c&&c->self==c&&c->owner==owner&&c->thread==GetCurrentThreadId();}
static DWORD scqs_fail(PAPACC_SCQ_SESSION_DRIVER *c,DWORD e)
{
 int pending=(c->profile&&c->profile->used)||(c->source&&c->source->value.controller_consumed);
 if(!c->first)c->first=e;
 if(c->receiver&&c->receiver->original)pending=1;
 c->phase=pending?6U:7U;
 if(pending){
  if(c->source)(void)scqa_close(c->source,c->owner,0);
  if(c->receiver&&c->receiver->original&&c->receiver->phase!=4U)(void)personal_scq_receive_finish(c->receiver,c->owner,0);
 }return e;
}
static DWORD scqs_prepare(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,DWORD role,
 PAPACC_SCQ_PROFILE_DATA *profile,PAPACC_SCQ_ADMIN_SOURCE *source,
 PAPACC_PERSONAL_SCQ_RECEIVER *receiver,PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime)
{
 if(!c||c->phase||!owner||!profile||!receiver||!dispatch||!runtime||(role!=1U&&role!=2U))return ERROR_INVALID_PARAMETER;
 c->self=c;c->owner=owner;c->thread=GetCurrentThreadId();c->role=role;c->phase=1;
 c->profile=profile;c->source=source;c->receiver=receiver;c->dispatch=dispatch;c->runtime=runtime;
 if(!scqp_owned(profile,owner)||profile->role!=role||profile->phase!=1U||profile->first||receiver->phase)return scqs_fail(c,ERROR_INVALID_STATE);
 if(role==1U&&(!source||source->self!=source||source->controller!=owner||source->plan!=profile||source->phase!=3U))return scqs_fail(c,ERROR_NOT_FOUND);
 if(role==2U&&(source||runtime->phase!=1U||dispatch->phase!=1U))return scqs_fail(c,ERROR_INVALID_STATE);
 return 0;
}
static DWORD scqs_current(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 DWORD e;PAPACC_SCQ_PLAN_DATA plan;
 if(!scqs_owned(c,owner)||c->phase<1U||c->phase>4U||!stop)return ERROR_INVALID_STATE;
 if(scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&plan)||
  (c->dispatch->phase&&c->dispatch->purpose!=plan.purpose))return scqs_fail(c,ERROR_ACCESS_DENIED);
 if(c->peer_lost||InterlockedCompareExchange(stop,0,0))return scqs_fail(c,ERROR_CANCELLED);
 e=scqp_current(c->profile,owner,local,remote,refs,stop);if(e)return scqs_fail(c,e);
 if(c->role==1U){e=scqa_current(c->source,owner,stop);if(e)return scqs_fail(c,e);}
 else if(c->receiver->original){const PAPACC_PERSONAL_SCQ_ORIGINAL *v=c->receiver->original;
  if(v->retained||!v->controller_consumed||!personal_scq_original_refs(v)||!scqa_operator(v,remote)||!personal_scq_original_binding(v,c->dispatch,c->runtime))return scqs_fail(c,ERROR_ACCESS_DENIED);}
 return 0;
}
static DWORD scqs_take(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *administrator,const PAPACC_SCQ_PEER_DATA *runtime,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 DWORD e;
 if(c&&(c->transport.pending||((c->transport.operation||c->transport.total)&&!c->transport.applying)))return ERROR_INVALID_STATE;
 if(!scqs_owned(c,owner)||c->role!=1U||c->phase!=1U)return ERROR_INVALID_STATE;
 e=scqs_current(c,owner,administrator,runtime,refs,stop);if(e)return e;
 e=scqa_take(c->source,owner,stop);if(e)return scqs_fail(c,e);
 e=papacc_scq_profile_take_data(c->profile,owner,administrator,runtime,refs,stop);if(e)return scqs_fail(c,e);
 if(c->profile->waiting_runtime&&!scqa_startup_original(c->source,owner,stop))return scqs_fail(c,ERROR_ACCESS_DENIED);
 c->phase=2;return 0; /* controller consumed; NO Start or other effect here */
}
static int scqs_runtime_snapshot(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,const PAPACC_SCQ_PEER_DATA *p)
{
 return p&&r&&r->profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL&&!papacc_personal_runtime_current(r)&&
 p->pid==r->pid&&!memcmp(p->creation,&r->creation,sizeof(FILETIME))&&p->sid_bytes==r->sid_bytes&&!memcmp(p->sid,r->sid,r->sid_bytes)&&
 !memcmp(p->token_id,&r->statistics.TokenId,sizeof(LUID))&&!memcmp(p->authentication,&r->statistics.AuthenticationId,sizeof(LUID))&&
 !memcmp(p->modified,&r->statistics.ModifiedId,sizeof(LUID))&&p->groups==r->group_count&&p->enumerated==r->group_count&&
 p->matches==r->specific_matches&&p->attributes==r->specific_attributes&&p->token_type==(DWORD)r->statistics.TokenType&&
 p->service_sid_type==r->expected_sid_type&&p->service_type==SERVICE_WIN32_OWN_PROCESS&&p->service_state==(r->closed_preparation?(DWORD)SERVICE_START_PENDING:(DWORD)SERVICE_RUNNING);
}
static DWORD scqs_offer(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *administrator,const PAPACC_SCQ_PEER_DATA *runtime,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 DWORD e;
 if(c&&(c->transport.pending||((c->transport.operation||c->transport.total)&&!c->transport.applying)))return ERROR_INVALID_STATE;
 if(!scqs_owned(c,owner)||c->role!=1U||c->phase!=2U)return ERROR_INVALID_STATE;
 /* An explicitly selected lifecycle act must have been independently taken.
  * Generic SCQ exchange does not imply permission to Start a service. */
 if(c->source->start_phase&&(!c->source->start_consumed||scqa_start_current(c->source,owner,stop)))return scqs_fail(c,ERROR_ACCESS_DENIED);
 if(!scqs_runtime_snapshot(c->runtime,runtime))return scqs_fail(c,ERROR_ACCESS_DENIED);
 if(c->profile->waiting_runtime){e=papacc_scq_profile_bind_runtime_data(c->profile,owner,administrator,runtime,refs,stop);if(e)return scqs_fail(c,e);}
 e=scqs_current(c,owner,administrator,runtime,refs,stop);if(e)return e;
 if(!scqa_borrow(c->source,owner,c->dispatch,c->runtime,stop))return scqs_fail(c,ERROR_ACCESS_DENIED);
 e=papacc_scq_profile_offer_data(c->profile,owner,administrator,runtime,refs,c->buffer,PAPACC_SCQ_RECORD_BYTES,stop);if(e)return scqs_fail(c,e);
 c->bytes=PAPACC_SCQ_RECORD_BYTES;c->phase=3;return 0;
}
static DWORD scqs_receive(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *runtime,const PAPACC_SCQ_PEER_DATA *administrator,const PAPACC_SCQ_REF_DATA refs[3],
 const BYTE *frame,DWORD n,PAPACC_PERSONAL_SCQ_ORIGINAL *local_original_proof,volatile LONG *stop)
{
 DWORD e;
 if(c&&(c->transport.pending||((c->transport.operation||c->transport.total)&&!c->transport.applying)))return ERROR_INVALID_STATE;
 if(!scqs_owned(c,owner)||c->role!=2U||c->phase!=1U)return ERROR_INVALID_STATE;
 /* This local original is acquired by the native origin adapter, NOT decoded
  * from frame or serialized across processes. Fixture passes a model only. */
 if(!local_original_proof)return scqs_fail(c,ERROR_NOT_FOUND);
 e=scqs_current(c,owner,runtime,administrator,refs,stop);if(e)return e;
 if(!scqs_runtime_snapshot(c->runtime,runtime))return scqs_fail(c,ERROR_ACCESS_DENIED);
 if(!scqa_original_identity(&local_original_proof->request_id,&refs[0])||!scqa_original_identity(&local_original_proof->decision_id,&refs[1])||
  !scqa_original_identity(&local_original_proof->image_id,&refs[2])||local_original_proof->operator_pid!=administrator->pid||
  memcmp(&local_original_proof->operator_creation,administrator->creation,sizeof(FILETIME))||
  local_original_proof->operator_sid_bytes!=administrator->sid_bytes||memcmp(local_original_proof->operator_sid,administrator->sid,administrator->sid_bytes)||
  memcmp(&local_original_proof->operator_statistics.TokenId,administrator->token_id,sizeof(LUID))||
  memcmp(&local_original_proof->operator_statistics.AuthenticationId,administrator->authentication,sizeof(LUID))||
  memcmp(&local_original_proof->operator_statistics.ModifiedId,administrator->modified,sizeof(LUID))||!scqa_operator(local_original_proof,administrator))return scqs_fail(c,ERROR_ACCESS_DENIED);
 e=personal_scq_receive_original(c->receiver,local_original_proof,owner,c->dispatch,c->runtime,stop);if(e)return scqs_fail(c,e);
 e=papacc_scq_profile_receive_data(c->profile,owner,runtime,administrator,refs,frame,n,stop);if(e)return scqs_fail(c,e);
 /* Separate recipient use of the already-consumed controller act. No body,
  * KSP call or administrative authority is dispatched by this DATA driver. */
 e=personal_scq_receive_consume(c->receiver,owner,stop);if(e)return scqs_fail(c,e);
 memcpy(c->buffer,frame,n);c->bytes=n;c->phase=3;return 0;
}
static DWORD scqs_receipt(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *runtime,const PAPACC_SCQ_PEER_DATA *administrator,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 DWORD e;
 if(c&&(c->transport.pending||((c->transport.operation||c->transport.total)&&!c->transport.applying)))return ERROR_INVALID_STATE;
 if(!scqs_owned(c,owner)||c->role!=2U||c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqs_current(c,owner,runtime,administrator,refs,stop);if(e)return e;
 if(!c->receiver->original||c->receiver->phase!=2U||!c->receiver->original->consumed)return scqs_fail(c,ERROR_ACCESS_DENIED);
 e=papacc_scq_profile_receipt_data(c->profile,owner,c->buffer,PAPACC_SCQ_RECEIPT_BYTES);if(e)return scqs_fail(c,e);
 c->bytes=PAPACC_SCQ_RECEIPT_BYTES;c->phase=4;return 0; /* DATA receipt, NOT authenticated ack */
}
static DWORD scqs_ack(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *administrator,const PAPACC_SCQ_PEER_DATA *runtime,const PAPACC_SCQ_REF_DATA refs[3],const BYTE *receipt,DWORD n,volatile LONG *stop)
{
 DWORD e;
 if(c&&(c->transport.pending||((c->transport.operation||c->transport.total)&&!c->transport.applying)))return ERROR_INVALID_STATE;
 if(!scqs_owned(c,owner)||c->role!=1U||c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqs_current(c,owner,administrator,runtime,refs,stop);if(e)return e;
 if(!scqs_runtime_snapshot(c->runtime,runtime))return scqs_fail(c,ERROR_ACCESS_DENIED);
 e=papacc_scq_profile_ack_data(c->profile,owner,administrator,runtime,refs,receipt,n,stop);if(e)return scqs_fail(c,e);
 memcpy(c->buffer,receipt,n);c->bytes=n;c->phase=4;return 0; /* validated DATA only */
}
static DWORD scqs_peer_lost(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner)
{if(!scqs_owned(c,owner)||c->phase==5U||c->phase==6U)return ERROR_INVALID_STATE;InterlockedExchange(&c->peer_lost,1);if(c->transport.phase&&c->transport.phase!=5U)return scqt_stop(c,owner);return scqs_fail(c,ERROR_PROCESS_ABORTED);}
static DWORD scqs_close(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,int quiescent_data)
{
 DWORD e;
 if(!c||!c->phase)return 0;if(!scqs_owned(c,owner)||c->phase==6U)return ERROR_INVALID_STATE;if(c->phase==5U)return 0;
 /* Once a diagnostic Start act was consumed, a caller's DATA quiescence
  * assertion cannot replace the original transport close/return evidence. */
 if(c->source&&c->source->start_consumed&&(!c->transport.close_accepted||!(c->transport.legs&16U)))return scqs_fail(c,ERROR_INVALID_STATE);
 if(c->transport.phase){e=scqt_close(c,owner);if(e)return scqs_fail(c,e);}
 e=papacc_scq_profile_close_data(c->profile,owner,quiescent_data);if(e){c->phase=6;if(!c->first)c->first=e;
  if(c->source)(void)scqa_close(c->source,owner,0);if(c->receiver->original)(void)personal_scq_receive_finish(c->receiver,owner,0);return e;}
 if(c->receiver->original){e=personal_scq_receive_finish(c->receiver,owner,quiescent_data);if(e){c->phase=6;if(!c->first)c->first=e;return e;}}
 if(c->source){e=scqa_close(c->source,owner,quiescent_data);if(e){c->phase=6;if(!c->first)c->first=e;return e;}}
 c->phase=5;return 0;
}
