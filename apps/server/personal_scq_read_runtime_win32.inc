/* K3-K6 same-pipe P projection and result I/O. No endpoint/peer acquisition,
 * new OVERLAPPED, impersonation, authority getter or fallback is introduced. */
#define SCQK_P_SEND 128U
#define SCQK_P_READ 256U
#define SCQK_RESULT_SEND 512U
#define SCQK_RESULT_READ 1024U
#define SCQK_PROJECTION_BYTES 144U
static DWORD scqkr_fail(PAPACC_SCQ_READ_RECIPIENT *r,DWORD e)
{if(r){if(!r->first)r->first=e;r->phase=4U;}return e;}
static DWORD scqkr_current(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!c||!scqt_owned(c,owner)||!c->profile||!c->dispatch||!c->runtime||!proof||!stop||c->phase!=3U||c->profile->pre_readiness!=1U||c->profile->read_diagnostic!=1U||!c->runtime||c->runtime->read_diagnostic!=1U||
  c->dispatch->purpose!=PAPACC_SCQ_PURPOSE_READ||!c->transport.query_refs3||c->transport.query_only||
  !c->transport.resource_scope||!c->transport.global_until||c->transport.global_until!=c->transport.resource_scope->global_until||c->transport.phase!=1U||c->transport.cancelled||!proof->controller_consumed||
  scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p)||p.purpose!=PAPACC_SCQ_PURPOSE_READ)return ERROR_ACCESS_DENIED;
 e=scqt_current(c,owner,c->role==1U?&p.administrator:&p.runtime,c->role==1U?&p.runtime:&p.administrator,p.refs,proof,stop);if(e)return e;
 e=scqt_resource_effect(c,owner,stop,0);return e?scqt_fail(c,e):0;
}
static DWORD scqkr_prepare(PAPACC_SCQ_READ_RECIPIENT *r,const void *owner,PAPACC_SCQ_RECIPIENT_ORIGIN *origin,
 PAPACC_SCQ_SESSION_DRIVER *c,PAPACC_PERSONAL_SCQ_INPUTS *held_inputs,const BYTE public_id[32],volatile LONG *stop)
{
 unsigned i,any=0;DWORD e;
 if(!r||r->phase||r->self||!origin||!c||c->read_recipient||!held_inputs||!public_id||c->role!=2U||!c->receiver||c->receiver->original!=&origin->value||
  c->receiver->phase!=2U||!origin->value.consumed||!origin->value.borrowed||origin->value.retained)return ERROR_ACCESS_DENIED;
 e=scqkr_current(c,owner,&origin->value,stop);if(e)return e;
 e=scqr_current(origin,owner,c->profile,held_inputs,c->runtime,c->dispatch,stop);if(e)return e;
 for(i=0;i<32U;i++)any|=public_id[i];if(!any)return ERROR_INVALID_DATA;
 r->self=r;r->owner=owner;r->origin=origin;r->session=c;r->inputs=held_inputs;r->thread=GetCurrentThreadId();r->phase=1U;memcpy(r->public_id,public_id,32);c->read_recipient=r;
 return 0; /* awaiting original pipe read; not P admission */
}
static DWORD scqkr_local(PAPACC_SCQ_READ_RECIPIENT *r,const void *owner,volatile LONG *stop)
{
 DWORD e;
 if(!r||r->self!=r||r->owner!=owner||r->thread!=GetCurrentThreadId()||!r->phase||r->phase>3U||r->first||!r->origin||!r->session||r->session->read_recipient!=r||!r->inputs)return ERROR_INVALID_STATE;
 e=scqkr_current(r->session,owner,&r->origin->value,stop);if(e)return scqkr_fail(r,e);
 e=scqr_current(r->origin,owner,r->session->profile,r->inputs,r->session->runtime,r->session->dispatch,stop);return e?scqkr_fail(r,e):0;
}
static DWORD scqkr_projection(PAPACC_SCQ_READ_DIAGNOSTIC *k,PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 DWORD e=scqk_current(k,owner,stop);if(e)return e;
 if(k->phase!=3U||!k->consumed||!c||c->role!=1U||c->source!=k->source||c->transport.pending||c->transport.operation||c->transport.total||
  (c->transport.legs&SCQK_P_SEND))return ERROR_INVALID_STATE;
 e=scqkr_current(c,owner,&k->source->value,stop);if(e)return e;
 c->read_act=k;memcpy(c->buffer,"SKP1",4);scqk_u32_put(c->buffer+4,144U);scqk_u32_put(c->buffer+8,PAPACC_SCQ_PURPOSE_READ);scqk_u32_put(c->buffer+12,1U);
 memcpy(c->buffer+16,k->attempt,32);memcpy(c->buffer+48,k->request,32);memcpy(c->buffer+80,k->decision,32);memcpy(c->buffer+112,k->key_public,32);return 0;
}
/* One original native I/O per call; pending is polled only by the held owner.
 * Partial completion advances this SAME leg, never a resubmission/retry act. */
static DWORD scqkr_io(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,DWORD operation,PAPACC_PERSONAL_SCQ_ORIGINAL *proof,PAPACC_SCQ_READ_DIAGNOSTIC *controller,PAPACC_SCQ_READ_RECIPIENT *recipient,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;DWORD e,n=0,bytes;BOOL ok;int writing;
 e=scqkr_current(c,owner,proof,stop);if(e)return e;t=&c->transport;
 if(operation!=SCQK_P_SEND&&operation!=SCQK_P_READ&&operation!=SCQK_RESULT_SEND&&operation!=SCQK_RESULT_READ)return ERROR_INVALID_PARAMETER;
 writing=operation==SCQK_P_SEND||operation==SCQK_RESULT_SEND;
 if((c->role==1U)!=(operation==SCQK_P_SEND||operation==SCQK_RESULT_READ)||t->legs&operation)return ERROR_INVALID_STATE;
 if(c->role==1U){
  e=scqk_current(controller,owner,stop);if(e)return e;
  if(controller->phase!=3U||!controller->consumed||c->source!=controller->source||proof!=&controller->source->value)return ERROR_ACCESS_DENIED;
  if(operation==SCQK_RESULT_READ&&!(t->legs&SCQK_P_SEND))return ERROR_ACCESS_DENIED;
  if(operation==SCQK_P_SEND&&(memcmp(c->buffer,"SKP1",4)||scqk_u32_get(c->buffer+4)!=144U||scqk_u32_get(c->buffer+8)!=PAPACC_SCQ_PURPOSE_READ||scqk_u32_get(c->buffer+12)!=1U||
   memcmp(c->buffer+16,controller->attempt,32)||memcmp(c->buffer+48,controller->request,32)||memcmp(c->buffer+80,controller->decision,32)||memcmp(c->buffer+112,controller->key_public,32)))return ERROR_ACCESS_DENIED;
 }else{
  e=scqkr_local(recipient,owner,stop);if(e)return e;
  if(recipient->session!=c||proof!=&recipient->origin->value)return ERROR_ACCESS_DENIED;
  if(operation==SCQK_P_READ&&recipient->phase!=1U)return ERROR_ACCESS_DENIED;
  if(operation==SCQK_RESULT_SEND){BYTE expected[128];PAPACC_SCQ_PLAN_DATA plan;
   if(recipient->phase!=3U||!recipient->probe_used||!recipient->probe||scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&plan)||
    scqk_result_encode(recipient->probe,plan.attempt,recipient->public_id,expected)||memcmp(c->buffer,expected,128))return ERROR_ACCESS_DENIED;
  }
 }
 bytes=operation==SCQK_P_SEND||operation==SCQK_P_READ?144U:128U;
 if(t->operation&&t->operation!=operation)return ERROR_INVALID_STATE;
 if(!t->operation){if(t->pending||t->total)return ERROR_INVALID_STATE;t->operation=operation;t->total=bytes;t->offset=0;}
 if(t->total!=bytes||t->offset>=bytes)return ERROR_INVALID_STATE;
 if(t->pending)return scqt_poll(c,owner);
 e=scqt_resource_effect(c,owner,stop,0);if(e)return scqt_fail(c,e);
 if(!ResetEvent(t->event))return scqt_fail(c,GetLastError());memset(&t->ov,0,sizeof(t->ov));t->ov.hEvent=t->event;t->pending=1U;
 ok=writing?WriteFile(t->pipe,c->buffer+t->offset,bytes-t->offset,&n,&t->ov):ReadFile(t->pipe,c->buffer+t->offset,bytes-t->offset,&n,&t->ov);
 if(!ok){e=GetLastError();if(e==ERROR_IO_PENDING)return e;t->pending=0;return scqt_fail(c,e);}
 t->pending=0;return scqt_io_result(c,n);
}
static DWORD scqkr_accept(PAPACC_SCQ_READ_RECIPIENT *r,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_TRANSPORT *t;BYTE *frame;DWORD e=scqkr_local(r,owner,stop);if(e)return e;
 t=&r->session->transport;frame=r->session->buffer;
 if(r->phase!=1U||r->probe_used||t->pending||t->operation!=SCQK_P_READ||t->total!=144U||t->offset!=144U||t->legs&SCQK_P_READ)return ERROR_INVALID_STATE;
 if(scqp_decode_fields(r->session->profile->record,sizeof(r->session->profile->record),&p)||memcmp(frame,"SKP1",4)||scqk_u32_get(frame+4)!=144U||
  scqk_u32_get(frame+8)!=PAPACC_SCQ_PURPOSE_READ||scqk_u32_get(frame+12)!=1U||memcmp(frame+16,p.attempt,32)||
  memcmp(frame+48,p.request,32)||memcmp(frame+80,p.decision,32)||memcmp(frame+112,r->public_id,32))return scqkr_fail(r,ERROR_INVALID_DATA);
 t->legs|=SCQK_P_READ;t->operation=t->total=t->offset=0;r->phase=2U;return 0;
}
static DWORD scqkr_probe(PAPACC_SCQ_READ_RECIPIENT *r,const void *owner,PAPACC_PERSONAL_CNG_PROBE *probe,volatile LONG *stop)
{
 DWORD e=scqkr_local(r,owner,stop);if(e)return e;
 if(r->phase!=2U||r->probe_used||!probe||probe->phase||r->session->transport.pending||r->session->transport.operation||r->session->runtime->transport_only)return ERROR_INVALID_STATE;
 e=scqt_resource_effect(r->session,owner,stop,0);if(e)return scqkr_fail(r,e);
 r->probe_used=1U;r->probe=probe;probe->read_scope=r; /* original P consumed before any provider call */
 (void)pcp_probe_mode(probe,r->session->runtime,r->session->dispatch->name,stop,1);
 if(probe->phase!=3U)return scqkr_fail(r,ERROR_INVALID_STATE);
 r->phase=3U;return 0; /* only observation; NOT permission/readiness */
}

static DWORD scqkr_finish_send(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,DWORD operation,PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;DWORD e=scqkr_current(c,owner,proof,stop);if(e)return e;t=&c->transport;
 if((operation!=SCQK_P_SEND&&operation!=SCQK_RESULT_SEND)||t->pending||t->operation!=operation||t->offset!=t->total||
  t->total!=(operation==SCQK_P_SEND?144U:128U)||t->legs&operation)return ERROR_INVALID_STATE;
 t->legs|=operation;t->operation=t->total=t->offset=0;return 0;
}
static DWORD scqkr_result_ready(PAPACC_SCQ_READ_RECIPIENT *r,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e=scqkr_local(r,owner,stop);if(e)return e;
 if(r->phase!=3U||!r->probe_used||!r->probe||r->session->transport.pending||r->session->transport.operation||
  scqp_decode_fields(r->session->profile->record,sizeof(r->session->profile->record),&p))return ERROR_INVALID_STATE;
 return scqk_result_encode(r->probe,p.attempt,r->public_id,r->session->buffer);
}
static DWORD scqkr_result_accept(PAPACC_SCQ_READ_DIAGNOSTIC *k,PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_PERSONAL_CNG_PROBE *out,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;DWORD e=scqk_current(k,owner,stop);if(e)return e;
 if(k->phase!=3U||!k->consumed||!c||c->role!=1U||c->source!=k->source)return ERROR_ACCESS_DENIED;
 e=scqkr_current(c,owner,&k->source->value,stop);if(e)return e;t=&c->transport;
 if(!(t->legs&SCQK_P_SEND)||t->legs&SCQK_RESULT_READ||t->pending||t->operation!=SCQK_RESULT_READ||t->total!=128U||t->offset!=128U)return ERROR_INVALID_STATE;
 e=scqk_result_decode(c->buffer,128,k->attempt,k->key_public,out);if(e)return scqt_fail(c,e);
 t->legs|=SCQK_RESULT_READ;t->operation=t->total=t->offset=0;return 0; /* DATA, never original control proof */
}

#define SCQK_READY_SEND 2048U
#define SCQK_READY_READ 4096U
/* SKA1 is a sequencing watermark from the SAME original admitted recipient,
 * not SQR/SQC, authority, native authentication or productive readiness.
 * Both endpoints still independently revalidate their full native originals. */
static DWORD scqkr_ready_io(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_SCQ_READ_DIAGNOSTIC *k,PAPACC_SCQ_READ_RECIPIENT *r,PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD e,n=0,operation=c->role==1U?SCQK_READY_READ:SCQK_READY_SEND;BOOL ok;BYTE expected[100];const BYTE *public_id;
 e=scqkr_current(c,owner,proof,stop);if(e)return e;
 if(c->role==1U){e=scqk_current(k,owner,stop);if(e)return e;if(k->phase!=2U||k->consumed||k->source!=c->source)return ERROR_ACCESS_DENIED;public_id=k->key_public;}
 else {e=scqkr_local(r,owner,stop);if(e)return e;if(r->phase!=1U||r->probe_used||r->session!=c)return ERROR_ACCESS_DENIED;public_id=r->public_id;}
 if(t->legs&operation)return ERROR_INVALID_STATE;
 if(scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p))return ERROR_INVALID_DATA;
 memcpy(expected,"SKA1",4);memcpy(expected+4,p.attempt,32);memcpy(expected+36,p.target,32);memcpy(expected+68,public_id,32);
 if(!t->operation){if(t->pending||t->total)return ERROR_INVALID_STATE;t->operation=operation;t->total=100U;t->offset=0;if(c->role==2U)memcpy(c->buffer,expected,100);}
 if(t->operation!=operation||t->total!=100U||t->offset>100U)return ERROR_INVALID_STATE;
 if(t->pending){e=scqt_poll(c,owner);if(e)return e;}
 else if(t->offset<100U){
  e=scqt_resource_effect(c,owner,stop,0);if(e)return e;
  if(!ResetEvent(t->event))return scqt_fail(c,GetLastError());memset(&t->ov,0,sizeof(t->ov));t->ov.hEvent=t->event;t->pending=1U;
  ok=c->role==1U?ReadFile(t->pipe,c->buffer+t->offset,100U-t->offset,&n,&t->ov):WriteFile(t->pipe,c->buffer+t->offset,100U-t->offset,&n,&t->ov);
  if(!ok){e=GetLastError();if(e==ERROR_IO_PENDING)return e;t->pending=0;return scqt_fail(c,e);}
  t->pending=0;e=scqt_io_result(c,n);if(e)return e;
 }
 if(t->offset!=100U)return ERROR_MORE_DATA;
 if(memcmp(c->buffer,expected,100))return scqt_fail(c,ERROR_ACCESS_DENIED);
 t->legs|=operation;t->operation=t->total=t->offset=0;return 0;
}
