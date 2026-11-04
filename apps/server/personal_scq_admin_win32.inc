#include "personal_scq_admin_win32.h"
/* Local controller source. Never serialized, never reconstructed in a peer.
 * Real peer delivery remains a separate, unauthorized native endpoint gate. */
static int scqa_scope(const PAPACC_SCQ_OWNER_DECISION *a,const PAPACC_SCQ_PLAN_DATA *p)
{
 return a&&p&&a->read_diagnostic<=1U&&(!a->read_diagnostic||p->purpose==PAPACC_SCQ_PURPOSE_READ)&&a->purpose==p->purpose&&scqp_static(p)&&scqp_window(p)&&
 !memcmp(a->valid_from,p->valid_from,sizeof(a->valid_from))&&!memcmp(a->valid_until,p->valid_until,sizeof(a->valid_until))&&
 a->controller_pid==p->administrator.pid&&!memcmp(a->controller_creation,p->administrator.creation,sizeof(FILETIME))&&
 !memcmp(a->controller_token,p->administrator.token_id,sizeof(LUID))&&!memcmp(a->controller_authentication,p->administrator.authentication,sizeof(LUID))&&
 !memcmp(a->controller_modified,p->administrator.modified,sizeof(LUID))&&
 !memcmp(a->request,p->request,32)&&!memcmp(a->decision,p->decision,32)&&
 !memcmp(a->attempt,p->attempt,32)&&!memcmp(a->policy,p->policy,32)&&!memcmp(a->target,p->target,32)&&
 !memcmp(a->image,p->refs[2].digest,32)&&!memcmp(a->originals,p->refs,sizeof(a->originals))&&!memcmp(a->key,p->key,112)&&
 a->administrator_bytes==p->administrator.sid_bytes&&a->runtime_bytes==p->runtime.sid_bytes&&
 !memcmp(a->administrator,p->administrator.sid,68)&&!memcmp(a->runtime,p->runtime.sid,68)&&
 !memcmp(p->request,p->refs[0].digest,32)&&!memcmp(p->decision,p->refs[1].digest,32);
}
static DWORD scqa_fail(PAPACC_SCQ_ADMIN_SOURCE *c,DWORD e)
{if(!c->first)c->first=e;c->phase=7;return e;}
static int scqa_operator(const PAPACC_PERSONAL_SCQ_ORIGINAL *s,const PAPACC_SCQ_PEER_DATA *p)
{
 union {TOKEN_GROUPS align;BYTE bytes[8192];} groups,restricted;
 TOKEN_STATISTICS t;TOKEN_ELEVATION e;TOKEN_ELEVATION_TYPE type;DWORD n,i,admins=0,interactive=0,attrs=0,session;
 TOKEN_GROUPS *g=(TOKEN_GROUPS *)groups.bytes,*r=(TOKEN_GROUPS *)restricted.bytes;
 if(!GetTokenInformation(s->operator_token,TokenStatistics,&t,sizeof(t),&n)||n!=sizeof(t)||t.TokenType!=TokenPrimary||
  !GetTokenInformation(s->operator_token,TokenGroups,groups.bytes,sizeof(groups),&n)||n>sizeof(groups)||n<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||
  g->GroupCount!=t.GroupCount||g->GroupCount!=p->groups||p->enumerated!=p->groups||g->GroupCount>(n-(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES))return 0;
 for(i=0;i<g->GroupCount;i++){
  if(!pri_sid_inside(groups.bytes,n,g->Groups[i].Sid))return 0;
  if(IsWellKnownSid(g->Groups[i].Sid,WinServiceSid))return 0;
  if(IsWellKnownSid(g->Groups[i].Sid,WinBuiltinAdministratorsSid)){admins++;attrs=g->Groups[i].Attributes;}
  if(IsWellKnownSid(g->Groups[i].Sid,WinInteractiveSid)){
   if(!(g->Groups[i].Attributes&SE_GROUP_ENABLED)||(g->Groups[i].Attributes&SE_GROUP_USE_FOR_DENY_ONLY))return 0;interactive++;}
 }
 if(admins!=1U||interactive!=1U||p->matches!=1U||p->attributes!=attrs||p->administrator_attributes!=attrs||
  !(attrs&SE_GROUP_ENABLED)||(attrs&SE_GROUP_USE_FOR_DENY_ONLY))return 0;
 if(!GetTokenInformation(s->operator_token,TokenRestrictedSids,restricted.bytes,sizeof(restricted),&n)||n>sizeof(restricted)||
  n<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||r->GroupCount||IsTokenRestricted(s->operator_token)||p->restricted)return 0;
 if(!GetTokenInformation(s->operator_token,TokenElevation,&e,sizeof(e),&n)||n!=sizeof(e)||!e.TokenIsElevated||p->elevated!=e.TokenIsElevated||
  !GetTokenInformation(s->operator_token,TokenElevationType,&type,sizeof(type),&n)||n!=sizeof(type)||p->elevation_type!=(DWORD)type||type==TokenElevationTypeLimited||
  !GetTokenInformation(s->operator_token,TokenSessionId,&session,sizeof(session),&n)||n!=sizeof(session)||!session||p->session!=session||p->logon!=2U)return 0;
 return 1;
}
static int scqa_original_identity(const BY_HANDLE_FILE_INFORMATION *v,const PAPACC_SCQ_REF_DATA *p)
{return v->dwVolumeSerialNumber==p->volume&&v->nFileIndexHigh==p->high&&v->nFileIndexLow==p->low&&
 v->nFileSizeHigh==p->size_high&&v->nFileSizeLow==p->size_low&&v->dwFileAttributes==p->attributes&&v->nNumberOfLinks==p->links;}
static DWORD scqa_current(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;unsigned i;
 if(!c||c->self!=c||c->controller!=owner||c->thread!=GetCurrentThreadId()||c->phase<1U||c->phase>4U||!stop)return ERROR_INVALID_STATE;
 if(InterlockedCompareExchange(stop,0,0))return scqa_fail(c,ERROR_CANCELLED);
 if(!c->registration||c->registration->controller!=owner||c->registration->source!=c||!c->plan||!scqp_owned(c->plan,owner)||c->plan->role!=1U||c->plan->first||c->plan->phase<1U||c->plan->phase>2U||
  c->registration->read_diagnostic!=c->plan->read_diagnostic||
  scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p)||!scqa_scope(c->registration,&p)||
  !scqa_original_identity(&c->value.request_id,&p.refs[0])||!scqa_original_identity(&c->value.decision_id,&p.refs[1])||!scqa_original_identity(&c->value.image_id,&p.refs[2])||
  !personal_scq_original_refs(&c->value)||!scqa_operator(&c->value,&p.administrator)||c->value.retained||
  c->value.purpose!=p.purpose||c->value.deadline!=(((ULONGLONG)p.valid_until[1]<<32)|p.valid_until[0])||
  c->value.operator_pid!=p.administrator.pid||memcmp(&c->value.operator_creation,p.administrator.creation,sizeof(FILETIME))||
  c->value.operator_sid_bytes!=p.administrator.sid_bytes||memcmp(c->value.operator_sid,p.administrator.sid,c->value.operator_sid_bytes)||
  memcmp(&c->value.operator_statistics.TokenId,p.administrator.token_id,sizeof(LUID))||
  memcmp(&c->value.operator_statistics.AuthenticationId,p.administrator.authentication,sizeof(LUID))||
  memcmp(&c->value.operator_statistics.ModifiedId,p.administrator.modified,sizeof(LUID)))return scqa_fail(c,ERROR_ACCESS_DENIED);
 for(i=0;i<112U;i++)if(c->value.key[i]!=(WCHAR)(BYTE)p.key[i])return scqa_fail(c,ERROR_ACCESS_DENIED);
 return 0;
}
static DWORD scqa_capture(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,
 const PAPACC_SCQ_OWNER_DECISION *registered,const PAPACC_SCQ_PROFILE_DATA *plan,
 const PAPACC_PERSONAL_SCQ_ORIGINAL *original,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;unsigned i;
 if(!c||c->phase||!owner||!plan||!original||!stop)return ERROR_INVALID_STATE;
 /* The production caller obtains this only from the immutable native
  * registration, never from a CLI/IPC record, boolean or hash getter. */
 if(!registered)return ERROR_NOT_FOUND;
 c->self=c;c->controller=owner;c->thread=GetCurrentThreadId();c->phase=1;c->registration=registered;c->plan=plan;
 if(registered->controller!=owner||registered->source!=c||!scqp_owned(plan,owner)||plan->role!=1U||plan->phase!=1U||
  registered->read_diagnostic!=plan->read_diagnostic||
  scqp_decode_fields(plan->record,sizeof(plan->record),&p)||!scqa_scope(registered,&p)||
  original->controller!=owner||original->purpose!=p.purpose||original->controller_consumed||original->consumed||original->borrowed||original->retained||
  original->operator_pid!=p.administrator.pid||memcmp(&original->operator_creation,p.administrator.creation,sizeof(FILETIME))||
  original->operator_sid_bytes!=p.administrator.sid_bytes||memcmp(original->operator_sid,p.administrator.sid,original->operator_sid_bytes)||
  memcmp(&original->operator_statistics.TokenId,p.administrator.token_id,sizeof(LUID))||
  memcmp(&original->operator_statistics.AuthenticationId,p.administrator.authentication,sizeof(LUID))||
  memcmp(&original->operator_statistics.ModifiedId,p.administrator.modified,sizeof(LUID)))return scqa_fail(c,ERROR_ACCESS_DENIED);
 c->value=*original;c->value.original=&c->value;
 for(i=0;i<112U;i++)c->value.key[i]=(WCHAR)(BYTE)p.key[i];
 return scqa_current(c,owner,stop);
}
static DWORD scqa_present(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{DWORD e;if(!c||c->phase!=1U)return ERROR_INVALID_STATE;e=scqa_current(c,owner,stop);if(e)return e;c->phase=2;return 0;}
static DWORD scqa_confirm(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,const char *reply,size_t bytes,volatile LONG *stop)
{
 static const char prefix[]="OWNER_SCQ ",hex[]="0123456789abcdef";PAPACC_SCQ_PLAN_DATA p;unsigned i;DWORD e;
 if(!c||c->phase!=2U)return ERROR_INVALID_STATE;e=scqa_current(c,owner,stop);if(e)return e;
 if(!reply||bytes!=sizeof(prefix)-1U+64U||memcmp(reply,prefix,sizeof(prefix)-1U)||
  scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p))return scqa_fail(c,ERROR_ACCESS_DENIED);
 for(i=0;i<32;i++)if(reply[sizeof(prefix)-1U+2U*i]!=hex[p.attempt[i]>>4]||reply[sizeof(prefix)+2U*i]!=hex[p.attempt[i]&15U])return scqa_fail(c,ERROR_ACCESS_DENIED);
 c->phase=3;return 0;
}
static DWORD scqa_take(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 DWORD e;if(!c||c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqa_current(c,owner,stop);if(e)return e;
 /* Monotonic controller consumption before Start/any protected effect;
  * a later denied check never rearms this original. */
 c->phase=4;if(InterlockedCompareExchange(&c->value.controller_consumed,1,0))return scqa_fail(c,ERROR_ALREADY_EXISTS);
 e=scqa_current(c,owner,stop);return e;
}
static PAPACC_PERSONAL_SCQ_ORIGINAL *scqa_borrow(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,
 const PAPACC_PERSONAL_SCQ_DISPATCH *d,const PAPACC_PERSONAL_RUNTIME_IDENTITY *r,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;size_t i;
 if(!c||c->phase!=4U||!c->value.controller_consumed||scqa_current(c,owner,stop)||!r||!d||
  papacc_personal_runtime_current((PAPACC_PERSONAL_RUNTIME_IDENTITY *)r)||
  scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p)||r->sid_bytes!=p.runtime.sid_bytes||memcmp(r->sid,p.runtime.sid,r->sid_bytes)||
  EqualSid((PSID)r->sid,c->value.operator_sid)||r->pid==c->value.operator_pid||memcmp(d->association,p.attempt,32))return NULL;
 if(c->value.purpose!=p.purpose||d->purpose!=p.purpose)return NULL;
 if(!c->value.runtime_pid){
  c->value.runtime_pid=r->pid;c->value.runtime_creation=r->creation;c->value.runtime_token=r->statistics;
  memcpy(c->value.association,p.attempt,32);for(i=0;i<112;i++)c->value.key[i]=(WCHAR)(BYTE)p.key[i];
 }
 return personal_scq_original_binding(&c->value,d,r)?&c->value:NULL;
}
/* SSO-B: controller-local original before the runtime exists. This is a
 * borrow of the already consumed live source, NOT delivery to ServiceMain,
 * lifecycle permission, a remote pointer or readiness. Never fill future PID. */
static const PAPACC_PERSONAL_SCQ_ORIGINAL *scqa_startup_original(PAPACC_SCQ_ADMIN_SOURCE *c,
 const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;
 if(!c||c->phase!=4U||!c->value.controller_consumed||c->value.consumed||c->value.borrowed||
  scqa_current(c,owner,stop)||!c->plan->waiting_runtime||c->plan->phase!=2U||!c->plan->taken||c->plan->used||
  scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p)||!scqp_unbound(&p.runtime)||
  c->value.runtime_pid||c->value.runtime_creation.dwLowDateTime||c->value.runtime_creation.dwHighDateTime)return NULL;
 return &c->value;
}
/* SSO-B S4: a distinct attributed local act for ONE diagnostic Start. The
 * immutable SCQ request/decision/image and frozen target are borrowed from
 * this original source; the operation-specific confirmation is independent.
 * Neither function calls SCM or transfers lifecycle authority to the peer. */
/* Closed lifecycle actors accept only the original TO scope or the explicit
 * K0 local READ pair. Purpose alone, ordinary READ and copied DATA are denied. */
static int scqa_closed_diagnostic_scope(const PAPACC_SCQ_ADMIN_SOURCE *c,const PAPACC_SCQ_PLAN_DATA *p)
{
 if(!c||!p||!c->plan||!c->registration||c->plan->pre_readiness!=1U||c->plan->read_diagnostic!=c->registration->read_diagnostic)return 0;
 return p->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?!c->plan->read_diagnostic:
  p->purpose==PAPACC_SCQ_PURPOSE_READ&&c->plan->read_diagnostic==1U;
}
static DWORD scqa_start_fail(PAPACC_SCQ_ADMIN_SOURCE *c,DWORD e)
{if(!c->start_first)c->start_first=e;c->start_phase=4U;c->value.retained=1;return e;}
static DWORD scqa_start_current(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!c||!stop||c->start_phase<1U||c->start_phase>3U||c->start_first)return ERROR_INVALID_STATE;
 e=scqa_current(c,owner,stop);if(e)return scqa_start_fail(c,e);
 if(!c->plan->pre_readiness||scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p)||
  !scqa_closed_diagnostic_scope(c,&p)||!c->registration->binary||!c->registration->parent||!c->registration->data||
  (c->start_phase==3U&&!c->start_consumed)||(c->start_phase!=3U&&c->start_consumed))return scqa_start_fail(c,ERROR_ACCESS_DENIED);
 return 0;
}
static DWORD scqa_start_present(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!c||c->start_phase||c->start_consumed||c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqa_current(c,owner,stop);if(e)return e;
 if(!c->plan->waiting_runtime||!c->plan->pre_readiness||scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p)||
  !scqa_closed_diagnostic_scope(c,&p)||!scqp_unbound(&p.runtime)||c->value.runtime_pid)return ERROR_ACCESS_DENIED;
 c->start_phase=1U;return scqa_start_current(c,owner,stop);
}
static DWORD scqa_start_confirm(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,const char *reply,size_t bytes,volatile LONG *stop)
{
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ",hex[]="0123456789abcdef";
 PAPACC_SCQ_PLAN_DATA p;DWORD e;unsigned i;
 if(!c||c->start_phase!=1U||c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqa_start_current(c,owner,stop);if(e)return e;
 if(!reply||bytes!=sizeof(prefix)-1U+64U||memcmp(reply,prefix,sizeof(prefix)-1U)||
  scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p))return scqa_start_fail(c,ERROR_ACCESS_DENIED);
 for(i=0;i<32U;i++)if(reply[sizeof(prefix)-1U+2U*i]!=hex[p.attempt[i]>>4]||reply[sizeof(prefix)+2U*i]!=hex[p.attempt[i]&15U])return scqa_start_fail(c,ERROR_ACCESS_DENIED);
 c->start_phase=2U;return 0;
}
static DWORD scqa_start_take(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 DWORD e;
 if(!c||c->start_phase!=2U)return ERROR_INVALID_STATE;
 e=scqa_start_current(c,owner,stop);if(e)return e;
 if(!scqa_startup_original(c,owner,stop))return scqa_start_fail(c,ERROR_ACCESS_DENIED);
 /* Both independent acts are consumed before the lifecycle caller can act.
  * No runtime PID/token is invented; no retry after an uncertain Start. */
 if(InterlockedCompareExchange(&c->start_consumed,1,0))return scqa_start_fail(c,ERROR_ALREADY_EXISTS);
 c->start_phase=3U;return scqa_start_current(c,owner,stop);
}
/* Original-selection producer: borrow ONLY from the existing attributed live
 * Start act. No registration setter, DATA reconstruction or permission for
 * endpoint effects. The borrower cannot use this as another Start attempt. */
static const PAPACC_SCQ_OWNER_DECISION *scqa_start_selection(PAPACC_SCQ_ADMIN_SOURCE *c,
 const void *owner,volatile LONG *stop)
{
 if(!c||!stop||(c->start_phase!=2U&&c->start_phase!=3U)||
  (c->phase!=3U&&c->phase!=4U)||scqa_start_current(c,owner,stop))return NULL;
 return c->registration; /* exact original, same owner/thread/lifetime */
}
static DWORD scqa_endpoint_fail(PAPACC_SCQ_ADMIN_SOURCE *c,DWORD e)
{if(!c->endpoint_first)c->endpoint_first=e;c->endpoint_phase=4U;c->value.retained=1;return e;}
static DWORD scqa_endpoint_current(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!c||!stop||c->endpoint_phase<1U||c->endpoint_phase>3U||c->endpoint_first)return ERROR_INVALID_STATE;
 e=scqa_current(c,owner,stop);if(e)return scqa_endpoint_fail(c,e);
 if(!c->plan->pre_readiness||scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p)||
  !scqa_closed_diagnostic_scope(c,&p)||!c->registration->binary||!c->registration->parent||!c->registration->data||
  (c->endpoint_phase==3U&&!c->endpoint_consumed)||(c->endpoint_phase!=3U&&c->endpoint_consumed))return scqa_endpoint_fail(c,ERROR_ACCESS_DENIED);
 return 0;
}
static DWORD scqa_endpoint_present(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!c||c->endpoint_phase||c->endpoint_consumed||c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqa_current(c,owner,stop);if(e)return e;
 if(!c->plan->waiting_runtime||!c->plan->pre_readiness||scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p)||
  !scqa_closed_diagnostic_scope(c,&p)||!scqp_unbound(&p.runtime)||c->value.runtime_pid)return ERROR_ACCESS_DENIED;
 c->endpoint_phase=1U;return scqa_endpoint_current(c,owner,stop);
}
static DWORD scqa_endpoint_confirm(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,const char *reply,size_t bytes,volatile LONG *stop)
{
 static const char prefix[]="OWNER_SCQ_PRIVATE_ENDPOINT ",hex[]="0123456789abcdef";
 PAPACC_SCQ_PLAN_DATA p;DWORD e;unsigned i;
 if(!c||c->endpoint_phase!=1U||c->phase!=3U)return ERROR_INVALID_STATE;
 e=scqa_endpoint_current(c,owner,stop);if(e)return e;
 if(!reply||bytes!=sizeof(prefix)-1U+64U||memcmp(reply,prefix,sizeof(prefix)-1U)||
  scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p))return scqa_endpoint_fail(c,ERROR_ACCESS_DENIED);
 for(i=0;i<32U;i++)if(reply[sizeof(prefix)-1U+2U*i]!=hex[p.attempt[i]>>4]||reply[sizeof(prefix)+2U*i]!=hex[p.attempt[i]&15U])return scqa_endpoint_fail(c,ERROR_ACCESS_DENIED);
 c->endpoint_phase=2U;return 0;
}
static DWORD scqa_endpoint_take(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,volatile LONG *stop)
{
 DWORD e;
 if(!c||c->endpoint_phase!=2U)return ERROR_INVALID_STATE;
 e=scqa_endpoint_current(c,owner,stop);if(e)return e;
 if(!scqa_startup_original(c,owner,stop))return scqa_endpoint_fail(c,ERROR_ACCESS_DENIED);
 /* The endpoint act is consumed before its first endpoint effect.
  * No runtime PID/token is invented; no retry after uncertain creation. */
 if(InterlockedCompareExchange(&c->endpoint_consumed,1,0))return scqa_endpoint_fail(c,ERROR_ALREADY_EXISTS);
 c->endpoint_phase=3U;return scqa_endpoint_current(c,owner,stop);
}
/* Original-selection producer: borrow ONLY from the existing attributed live
 * endpoint act. No registration setter, DATA reconstruction or permission for
 * Start effects. The borrower cannot use this as another endpoint attempt. */
static const PAPACC_SCQ_OWNER_DECISION *scqa_endpoint_selection(PAPACC_SCQ_ADMIN_SOURCE *c,
 const void *owner,volatile LONG *stop)
{
 if(!c||!stop||(c->endpoint_phase!=2U&&c->endpoint_phase!=3U)||
  (c->phase!=3U&&c->phase!=4U)||scqa_endpoint_current(c,owner,stop))return NULL;
 return c->registration; /* exact original, same owner/thread/lifetime */
}
/* Discovery projection only. Its possession is NEVER Start permission or an
 * original receiver reference. It can be fixed before any runtime PID exists.
 * Reuse the original one-use attempt as the endpoint selector; first-instance
 * creation still denies collisions, no reconnect/adoption is introduced. */
static int scqa_carrier_scope_variant(const PS_SCQ_CARRIER *v,const PAPACC_SCQ_PLAN_DATA *p,unsigned read_diagnostic)
{
 return v&&p&&read_diagnostic<=1U&&p->purpose==(read_diagnostic?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)&&v->controller_pid==p->administrator.pid&&
  !memcmp(v->controller_creation,p->administrator.creation,sizeof(FILETIME))&&!memcmp(v->until,p->valid_until,sizeof(v->until))&&
  v->runtime_sid_bytes==p->runtime.sid_bytes&&!memcmp(v->runtime_sid,p->runtime.sid,68)&&
  !memcmp(v->attempt,p->attempt,32)&&!memcmp(v->policy,p->policy,32)&&!memcmp(v->target,p->target,32)&&
  !memcmp(v->request,p->request,32)&&!memcmp(v->decision,p->decision,32)&&!memcmp(v->image,p->refs[2].digest,32)&&!memcmp(v->endpoint,p->attempt,16);
}
static int scqa_carrier_scope(const PS_SCQ_CARRIER *v,const PAPACC_SCQ_PLAN_DATA *p)
{return scqa_carrier_scope_variant(v,p,0U);}
static DWORD scqa_start_carrier(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,PS_SCQ_CARRIER *out,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!out||!c||!scqa_start_selection(c,owner,stop))return ERROR_INVALID_STATE;
 e=scqa_start_current(c,owner,stop);if(e)return e;
 if(scqp_decode_fields(c->plan->record,sizeof(c->plan->record),&p))return ERROR_INVALID_DATA;
 if(!scqp_nonzero(p.attempt,16))return ERROR_INVALID_DATA;
 memset(out,0,sizeof(*out));out->controller_pid=p.administrator.pid;
 out->controller_creation[0]=p.administrator.creation[0];out->controller_creation[1]=p.administrator.creation[1];
 out->until[0]=p.valid_until[0];out->until[1]=p.valid_until[1];out->runtime_sid_bytes=p.runtime.sid_bytes;
 memcpy(out->attempt,p.attempt,32);memcpy(out->policy,p.policy,32);memcpy(out->target,p.target,32);
 memcpy(out->request,p.request,32);memcpy(out->decision,p.decision,32);memcpy(out->image,p.refs[2].digest,32);
 memcpy(out->endpoint,p.attempt,16);memcpy(out->runtime_sid,p.runtime.sid,68);return 0;
}
static DWORD scqa_close(PAPACC_SCQ_ADMIN_SOURCE *c,const void *owner,int quiescent)
{
 if(!c||!c->phase)return 0;
 if(c->self!=c||c->controller!=owner||c->thread!=GetCurrentThreadId()||c->phase==6U)return ERROR_INVALID_STATE;
 if(c->phase==5U)return 0;
 if(!quiescent||c->value.borrowed){c->phase=6;c->value.retained=1;c->value.consumed=1;c->value.controller_consumed=1;if(c->start_phase)c->start_phase=4U;if(c->endpoint_phase)c->endpoint_phase=4U;return ERROR_INVALID_HANDLE;}
 c->phase=5;return 0; /* borrows only; original refs close in canonical packet */
}
