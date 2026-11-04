#include "personal_scq_recipient_win32.h"
static DWORD scqr_fail(PAPACC_SCQ_RECIPIENT_ORIGIN *r,DWORD e)
{if(!r->first)r->first=e?e:ERROR_ACCESS_DENIED;r->phase=4;r->value.retained=1;return r->first;}
static int scqr_inputs_scope(const PAPACC_PERSONAL_SCQ_INPUTS *inputs,const PAPACC_SCQ_PLAN_DATA *p)
{
 unsigned i;
 if(!inputs||!inputs->content_verified||inputs->content_hash.retained||inputs->content_hash.active||inputs->content_hash.hash||inputs->content_hash.algorithm)return 0;
 for(i=0;i<3;i++)if(!inputs->reader[i]||!scqa_original_identity(&inputs->identity[i],&p->refs[i])||
 memcmp(inputs->digest[i],p->refs[i].digest,32)||memcmp(inputs->descriptor_digest[i],p->refs[i].descriptor,32))return 0;
 return 1;
}
static int scqr_operator_scope(const PAPACC_PERSONAL_SCQ_ORIGINAL *v,const PAPACC_SCQ_PEER_DATA *p)
{return !memcmp(&v->operator_statistics.TokenId,p->token_id,sizeof(LUID))&&!memcmp(&v->operator_statistics.AuthenticationId,p->authentication,sizeof(LUID))&&
 !memcmp(&v->operator_statistics.ModifiedId,p->modified,sizeof(LUID));}
/* Called only after original read-only files have been independently acquired
 * and CNG content/descriptor checks returned. No remote pointer is copied.
 * The immutable receiver registration is independent of the DATA projection. */
static DWORD scqr_acquire_mode(PAPACC_SCQ_RECIPIENT_ORIGIN *r,const void *owner,
 const PAPACC_SCQ_OWNER_DECISION *registered,PAPACC_SCQ_PROFILE_DATA *profile,
 PAPACC_PERSONAL_SCQ_INPUTS *inputs,const PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,
 const PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,HANDLE original_controller,HANDLE original_token,PAPACC_SCQ_SESSION_DRIVER *capture,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_PERSONAL_SCQ_ORIGINAL *v;DWORD bytes,e;FILETIME exit,kernel,user;unsigned i;
 if(!r||r->phase||!owner||!registered||!profile||!inputs||!runtime||!dispatch||!original_controller||!stop)return ERROR_NOT_FOUND;
 if(!scqp_owned(profile,owner)||profile->role!=2U||profile->phase!=1U||registered->read_diagnostic!=profile->read_diagnostic||scqp_decode_fields(profile->record,sizeof(profile->record),&p)||!scqa_scope(registered,&p)||
 !scqs_runtime_snapshot((PAPACC_PERSONAL_RUNTIME_IDENTITY *)runtime,&p.runtime)||!scqr_inputs_scope(inputs,&p))return ERROR_ACCESS_DENIED;
 e=personal_scq_inputs_check(inputs,(PSID)p.administrator.sid,stop,0);if(e)return e;
 r->self=r;r->owner=owner;r->registration=registered;r->thread=GetCurrentThreadId();r->phase=1;v=&r->value;
 /* This pointer identifies the LOCAL peer-observation backing, never a
  * pointer received from the administrative process or another grant. */
 v->original=v;v->controller=r;v->recipient=owner;v->purpose=p.purpose;
 if(capture){e=scqt_resource_effect(capture,owner,stop,1);if(e)return scqr_fail(r,e);}
 if(!DuplicateHandle(GetCurrentProcess(),original_controller,GetCurrentProcess(),&v->operator_process,PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,0))return scqr_fail(r,GetLastError());
 if(GetProcessId(v->operator_process)!=p.administrator.pid||!GetProcessTimes(v->operator_process,&v->operator_creation,&exit,&kernel,&user)||
 memcmp(&v->operator_creation,p.administrator.creation,sizeof(FILETIME))||WaitForSingleObject(v->operator_process,0)!=WAIT_TIMEOUT)return scqr_fail(r,ERROR_ACCESS_DENIED);
 if(original_token){
  if(!capture)return scqr_fail(r,ERROR_ACCESS_DENIED);
  e=scqt_resource_effect(capture,owner,stop,1);if(e)return scqr_fail(r,e);
  if(!DuplicateHandle(GetCurrentProcess(),original_token,GetCurrentProcess(),&v->operator_token,TOKEN_QUERY,FALSE,0))return scqr_fail(r,GetLastError());
 }else if(!OpenProcessToken(v->operator_process,TOKEN_QUERY,&v->operator_token))return scqr_fail(r,GetLastError());
 if(!GetTokenInformation(v->operator_token,TokenStatistics,&v->operator_statistics,sizeof(v->operator_statistics),&bytes)||bytes!=sizeof(v->operator_statistics))return scqr_fail(r,GetLastError());
 if(!scqr_operator_scope(v,&p.administrator))return scqr_fail(r,ERROR_ACCESS_DENIED);
 v->operator_pid=p.administrator.pid;v->operator_sid_bytes=p.administrator.sid_bytes;memcpy(v->operator_sid,p.administrator.sid,68);
 v->request=inputs->reader[0];v->decision=inputs->reader[1];v->image=inputs->reader[2];
 v->request_id=inputs->identity[0];v->decision_id=inputs->identity[1];v->image_id=inputs->identity[2];
 v->runtime_pid=runtime->pid;v->runtime_creation=runtime->creation;v->runtime_token=runtime->statistics;memcpy(v->association,p.attempt,32);
 for(i=0;i<112;i++)v->key[i]=(WCHAR)(BYTE)p.key[i];v->deadline=((ULONGLONG)p.valid_until[1]<<32)|p.valid_until[0];
 if(!personal_scq_original_refs(v)||!scqa_operator(v,&p.administrator)||!personal_scq_original_binding(v,dispatch,runtime))return scqr_fail(r,ERROR_ACCESS_DENIED);
 return 0; /* QUERY/READ originals only: not consumption, authority or admission */
}
static DWORD scqr_acquire(PAPACC_SCQ_RECIPIENT_ORIGIN *r,const void *owner,
 const PAPACC_SCQ_OWNER_DECISION *registered,PAPACC_SCQ_PROFILE_DATA *profile,
 PAPACC_PERSONAL_SCQ_INPUTS *inputs,const PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,
 const PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,HANDLE original_controller,volatile LONG *stop)
{return scqr_acquire_mode(r,owner,registered,profile,inputs,runtime,dispatch,original_controller,NULL,NULL,stop);}
/* Initial receiver composition in the EXISTING closed packet. runtime and
 * inputs must already be native-acquired and independently verified; caller
 * DATA or a remote numeric HANDLE is not an acquisition. No endpoint is
 * created/connected, no grant is installed and no readiness is reported.
 * Failure never resets a partially acquired source for retry. */
static DWORD scqr_first_prepare(PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_OWNER_DECISION *registered,PAPACC_SCQ_PROFILE_DATA *profile,
 PAPACC_PERSONAL_SCQ_INPUTS *inputs,PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,
 PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,PAPACC_PERSONAL_SCQ_RECEIVER *receiver,
 const PAPACC_SCQ_PLAN_DATA *plan,HANDLE original_controller,volatile LONG *stop)
{
 WCHAR empty[112]={0};DWORD e;
 if(!r||!c||!owner||!registered||!profile||!inputs||!runtime||!dispatch||!receiver||!plan||!original_controller||!stop)return ERROR_NOT_FOUND;
 if(r->phase||c->phase||profile->phase||dispatch->phase||receiver->phase||runtime->closed_preparation!=1U||
  (registered->read_diagnostic?(registered->read_diagnostic!=1U||plan->purpose!=PAPACC_SCQ_PURPOSE_READ||runtime->read_diagnostic!=1U||!c->transport.read_diagnostic||!c->transport.query_refs3):plan->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)||plan->runtime.service_state!=SERVICE_START_PENDING||
  !scqa_scope(registered,plan)||!scqs_runtime_snapshot(runtime,&plan->runtime)||!scqr_inputs_scope(inputs,plan))return ERROR_ACCESS_DENIED;
 if(c->transport.query_refs3&&(!scqt_owned(c,owner)||c->transport.phase!=3U||c->transport.capture_query!=2U||c->transport.capture_valid!=7U||
  c->transport.capture_registration!=registered||c->transport.capture_controller!=original_controller||!c->transport.capture_token||
  !c->transport.resource_scope||!c->transport.resource_scope->p1_handles))return ERROR_ACCESS_DENIED;
 if(InterlockedCompareExchange(stop,0,0))return ERROR_CANCELLED;
 if(registered->read_diagnostic){unsigned j;for(j=0;j<112U;j++)empty[j]=(WCHAR)(BYTE)plan->key[j];}
 e=(registered->read_diagnostic?papacc_personal_scq_prepare:papacc_personal_scq_prepare_transport)(dispatch,runtime,plan->attempt,empty);if(e)goto failed;
 e=(registered->read_diagnostic?papacc_scq_profile_prepare_machine_read_data:papacc_scq_profile_prepare_transport_data)(profile,owner,2U,plan,&plan->runtime);if(e)goto failed;
 e=scqr_acquire_mode(r,owner,registered,profile,inputs,runtime,dispatch,original_controller,
  c->transport.query_refs3?c->transport.capture_token:NULL,c->transport.query_refs3?c:NULL,stop);if(e)goto failed;
 e=scqs_prepare(c,owner,2U,profile,NULL,receiver,dispatch,runtime);if(e)goto failed;
 return 0; /* Still query-only, unconsumed, START_PENDING, not bilateral. */
failed:
 return scqr_fail(r,e);
}
static DWORD scqr_current(PAPACC_SCQ_RECIPIENT_ORIGIN *r,const void *owner,PAPACC_SCQ_PROFILE_DATA *profile,
 PAPACC_PERSONAL_SCQ_INPUTS *inputs,const PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,const PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!r||r->self!=r||r->owner!=owner||r->thread!=GetCurrentThreadId()||r->phase<1U||r->phase>2U||!r->registration||r->value.retained)return ERROR_INVALID_STATE;
 if(!scqp_owned(profile,owner)||profile->role!=2U||!r->registration||r->registration->read_diagnostic!=profile->read_diagnostic||scqp_decode_fields(profile->record,sizeof(profile->record),&p)||!scqa_scope(r->registration,&p)||!scqr_inputs_scope(inputs,&p)||!scqr_operator_scope(&r->value,&p.administrator)||
 !personal_scq_original_refs(&r->value)||!scqa_operator(&r->value,&p.administrator)||!personal_scq_original_binding(&r->value,dispatch,runtime))return scqr_fail(r,ERROR_ACCESS_DENIED);
 e=personal_scq_inputs_check(inputs,(PSID)p.administrator.sid,stop,0);return e?scqr_fail(r,e):0;
}
/* The query constructor never stamps a consumed act. Only this transition,
 * after independently checked originals AND a read from the correlated original
 * administrative endpoint, can bind its one scoped offer to the local origin.
 * No candidate metadata, caller boolean or SQR1 echo enters this path. */
static DWORD scqr_open(PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 PAPACC_PERSONAL_SCQ_INPUTS *inputs,const WCHAR *name,const PAPACC_SCQ_PEER_DATA *local,
 const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 DWORD e;
 if(!r||r->phase!=1U||r->value.controller_consumed||!scqs_owned(c,owner)||c->role!=2U||c->phase!=1U||c->transport.phase)return ERROR_INVALID_STATE;
 e=scqr_current(r,owner,c->profile,inputs,c->runtime,c->dispatch,stop);if(e)return e;
 c->transport.query_only=1;
 e=scqt_open_local(c,owner,name,local,remote,refs,&r->value,stop);
 if(e){(void)scqr_fail(r,e);(void)scqs_fail(c,e);}return e;
}
static DWORD scqr_admit_offer(PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 PAPACC_PERSONAL_SCQ_INPUTS *inputs,const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,
 const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 DWORD e;PAPACC_SCQ_TRANSPORT *t=&c->transport;
 if(!r||r->phase!=1U||r->value.controller_consumed||!scqt_owned(c,owner)||c->role!=2U||c->phase!=1U||!t->query_only||
 t->pending||t->operation!=2U||t->total!=PAPACC_SCQ_RECORD_BYTES||t->offset!=t->total)return ERROR_INVALID_STATE;
 e=scqr_current(r,owner,c->profile,inputs,c->runtime,c->dispatch,stop);if(e)return scqt_fail(c,e);
 e=scqt_current(c,owner,local,remote,refs,&r->value,stop);if(e)return e;
 if(memcmp(c->buffer,c->profile->record,PAPACC_SCQ_RECORD_BYTES))return scqt_fail(c,ERROR_INVALID_DATA);
 /* Native peer identity, independent immutable decision scope, content/SD
  * custody and the existing producer's take-before-offer ordering are all
  * required, not the DATA frame in isolation. No KSP/body executes here. */
 r->value.controller_consumed=1;r->phase=2;t->query_only=0;
 return 0;
}
/* Closed diagnostic readiness evidence only. No SetServiceStatus, productive
 * callback, KSP dispatch or grant is performed. Independent local originals
 * AND the native authenticated delivery transition must already exist. */
static DWORD scqr_closed_terminal_current(PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_SCQ_SESSION_DRIVER *c,
 const void *owner,PAPACC_PERSONAL_SCQ_INPUTS *inputs,volatile LONG *stop,unsigned inconclusive)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(inconclusive>1U)return ERROR_INVALID_PARAMETER;
 if(!r||!c||!stop||!c->receiver||!c->profile||!c->runtime||!c->dispatch||r->phase!=2U||!scqt_owned(c,owner)||c->role!=2U||c->first||c->peer_lost||
  c->transport.first||c->transport.pending||c->transport.operation||c->transport.query_only||!(c->transport.legs&2U)||
  !r->value.controller_consumed||!r->value.consumed||!r->value.borrowed||
  c->receiver->original!=&r->value||c->transport.original!=&r->value||
  scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p))return ERROR_INVALID_STATE;
 /* The closed session completes DATA receipt AND its separately produced
  * native close before changing SCM state. Otherwise a fresh RUNNING peer
  * would disagree with the original Pending binding during receipt/close. */
 if((p.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||(c->read_recipient||c->transport.query_refs3)&&p.purpose==PAPACC_SCQ_PURPOSE_READ)&&(!c->transport.close_confirmed||!(c->transport.legs&32U)))return ERROR_INVALID_STATE;
 if(inconclusive&&(!c->profile->pre_readiness||c->profile->read_diagnostic!=1U||p.purpose!=PAPACC_SCQ_PURPOSE_READ||!c->read_recipient||!papacc_personal_cng_inconclusive(c->read_recipient->probe)))return ERROR_INVALID_STATE;
 if(c->profile->pre_readiness){
  if(p.purpose==PAPACC_SCQ_PURPOSE_READ&&(c->read_recipient||c->transport.query_refs3)&&(!scqt_read_result_closed(c)||!c->read_recipient||!c->read_recipient->probe||(!inconclusive&&c->read_recipient->probe->classification!=PAPACC_CNG_PROBE_PERMISSION_OBSERVED)))return ERROR_INVALID_STATE;
  if(c->phase!=4U||!c->profile->ack||!(c->transport.legs&1U)||
   !(c->transport.legs&4U)||c->transport.close_produced!=3U)return ERROR_INVALID_STATE;
 }else if(c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqr_current(r,owner,c->profile,inputs,c->runtime,c->dispatch,stop);if(e)return e;
 return scqt_current(c,owner,&p.runtime,&p.administrator,p.refs,&r->value,stop);
}
static DWORD scqr_closed_ready_current(PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_PERSONAL_SCQ_INPUTS *inputs,volatile LONG *stop)
{return scqr_closed_terminal_current(r,c,owner,inputs,stop,0U);}
/* Arm only after the actual closed delivery evidence. This changes an expected
 * SCM observation, not authority; before a factual RUNNING report current()
 * will fail closed. A failed report therefore cannot restore/retry this state. */
static DWORD scqr_arm_closed_running(PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_SCQ_SESSION_DRIVER *c,
 const void *owner,PAPACC_PERSONAL_SCQ_INPUTS *inputs,volatile LONG *stop)
{
 DWORD e;
 if(!c||!c->profile||!c->runtime||!c->profile->pre_readiness||c->runtime->closed_preparation!=1U)return ERROR_INVALID_STATE;
 e=scqr_closed_ready_current(r,c,owner,inputs,stop);if(e)return e;
 c->runtime->closed_preparation=2U;return 0;
}
static DWORD scqr_close(PAPACC_SCQ_RECIPIENT_ORIGIN *r,const void *owner)
{
 if(!r||!r->phase)return 0;if(r->self!=r||r->owner!=owner||r->thread!=GetCurrentThreadId()||r->phase==4U||r->value.borrowed)return ERROR_INVALID_STATE;
 if(r->phase==3U)return 0;
 if(r->value.retained){r->phase=4;return ERROR_INVALID_STATE;}
 if(r->value.operator_token){if(!CloseHandle(r->value.operator_token))return scqr_fail(r,GetLastError());r->value.operator_token=NULL;}
 if(r->value.operator_process){if(!CloseHandle(r->value.operator_process))return scqr_fail(r,GetLastError());r->value.operator_process=NULL;}
 r->phase=3;return 0; /* file custody remains with existing inputs owner */
}

/* Bounded continuation of an ALREADY original-acquired/connected transport.
 * No endpoint/source constructor, callback grant, sleep, retry loop or KSP.
 * At most one IO submission/poll per call. 0 means this side's close leg,
 * NEVER productive readiness or peer/global quiescence. */
static DWORD scqr_continue_transport(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 PAPACC_SCQ_RECIPIENT_ORIGIN *origin,PAPACC_PERSONAL_SCQ_INPUTS *inputs,
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_TRANSPORT *t;const PAPACC_SCQ_PEER_DATA *local,*remote;DWORD e,operation;
 if(!c||!c->profile||!scqt_owned(c,owner)||!proof||!stop)return ERROR_INVALID_STATE;
 t=&c->transport;
 if(t->phase!=1U||t->cancelled||c->first||t->first||!c->profile->pre_readiness||
  scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p)||(p.purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&!scqt_read_result_closed(c)))return ERROR_INVALID_STATE;
 local=c->role==1U?&p.administrator:&p.runtime;remote=c->role==1U?&p.runtime:&p.administrator;
 if(InterlockedCompareExchange(stop,0,0))return scqt_stop(c,owner);
 e=scqt_current(c,owner,local,remote,p.refs,proof,stop);
 if(e){if(t->pending)(void)scqt_stop(c,owner);return e;}
 if(c->role==1U){
  if(origin||inputs||!t->delivery_process)return ERROR_NOT_READY;
  if(c->phase==3U)operation=(t->legs&1U)?2U:1U;
  else if(c->phase==4U){if(t->close_accepted&&(t->legs&8U)){if(t->legs&16U)return 0;operation=16U;}else operation=8U;}
  else return ERROR_INVALID_STATE;
 }else{
  if(!origin||!inputs||proof!=&origin->value)return ERROR_NOT_READY;
  e=scqr_current(origin,owner,c->profile,inputs,c->runtime,c->dispatch,stop);if(e)return scqt_fail(c,e);
  if(c->phase==1U)operation=2U;
  else if(c->phase==3U){e=scqs_receipt(c,owner,local,remote,p.refs,stop);return e?e:ERROR_MORE_DATA;}
  else if(c->phase==4U){
   if(!(t->legs&1U))operation=1U;
   else if(!t->close_produced){e=scqt_produce_close(c,owner,local,remote,p.refs,proof,stop);return e?e:ERROR_MORE_DATA;}
   else if(t->close_produced==3U&&(t->legs&4U)){if(t->close_confirmed&&(t->legs&32U))return 0;operation=32U;}
   else if(t->close_produced==1U)operation=4U;
   else return ERROR_INVALID_STATE;
  }else return ERROR_INVALID_STATE;
 }
 if(t->operation&&t->operation!=operation)return scqt_fail(c,ERROR_INVALID_STATE);
 if(t->pending)e=scqt_poll(c,owner);
 else if(!t->operation||t->offset<t->total)e=scqt_start_io(c,owner,operation,local,remote,p.refs,proof,stop);
 else e=0;
 if(e==ERROR_IO_PENDING||e==ERROR_MORE_DATA)return e;
 if(e)return e;
 if(c->role==2U&&operation==2U){
  if(t->query_only){e=scqr_admit_offer(origin,c,owner,inputs,local,remote,p.refs,stop);if(e)return e;}
  /* Separate legitimate delivery/adoption, never remote numeric IDs as handles. */
  if(!t->files[0]||!t->files[1]||!t->files[2]||!t->delivered_inputs[0]||!t->delivered_inputs[1]||!t->delivered_inputs[2])return ERROR_NOT_READY;
 }
 e=operation==4U||operation==8U||operation==16U||operation==32U?scqt_apply_close(c,owner,operation,local,remote,p.refs,proof,stop):scqt_apply(c,owner,operation,local,remote,p.refs,proof,stop);
 if(e)return e;
 return (c->role==1U&&t->close_accepted&&(t->legs&16U))||(c->role==2U&&t->close_confirmed&&(t->legs&32U))?0:ERROR_MORE_DATA;
}
