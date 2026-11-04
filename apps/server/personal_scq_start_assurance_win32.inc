#ifndef PAPACC_SCQ_START_ASSURANCE_DEFINED
#define PAPACC_SCQ_START_ASSURANCE_DEFINED
#include "personal_scq_start_assurance_win32.h"
static void scqg_selected(PAPACC_SCQ_START_LIMITS *v)
{
 memset(v,0,sizeof(*v));v->private_growth=v->working_growth=1994944U;
 v->physical_floor=7239172096ULL;v->commit_floor=9823326208ULL;
 v->max_handles=128U;v->freshness_ms=100U;v->duration_ms=60000U;
}
static int scqg_selected_exact_legacy(const PAPACC_SCQ_START_LIMITS *v,DWORD window)
{
 PAPACC_SCQ_START_LIMITS s;scqg_selected(&s);
 return v&&window==240000U&&v->private_growth==s.private_growth&&v->working_growth==s.working_growth&&
 v->physical_floor==s.physical_floor&&v->commit_floor==s.commit_floor&&v->max_handles==s.max_handles&&
 v->freshness_ms==s.freshness_ms&&v->duration_ms==s.duration_ms;
}
static void scqg_selected_p1(PAPACC_SCQ_START_LIMITS *v)
{scqg_selected(v);v->max_handles=PAPACC_SCQ_P1_CAP;v->commit_floor=PAPACC_SCQ_P1_COMMIT_FLOOR;v->working_growth=PAPACC_SCQ_P1_WORKING_GROWTH;}
static int scqg_selected_exact_mode(const PAPACC_SCQ_START_LIMITS *v,DWORD window,int p1)
{PAPACC_SCQ_START_LIMITS copy;if(!v)return 0;copy=*v;if(p1){if(copy.max_handles!=PAPACC_SCQ_P1_CAP||copy.commit_floor!=PAPACC_SCQ_P1_COMMIT_FLOOR||copy.working_growth!=PAPACC_SCQ_P1_WORKING_GROWTH)return 0;copy.max_handles=128U;copy.commit_floor=9823326208ULL;copy.working_growth=1994944U;}return scqg_selected_exact_legacy(&copy,window);}
static int scqg_selected_exact(const PAPACC_SCQ_START_LIMITS *v,DWORD window)
{return scqg_selected_exact_mode(v,window,0);}
/* G1 never makes opaque costs zero. This actor establishes only limited
 * eligibility from original acts + a shown G3 plan + fresh native observations. */
static DWORD scqg_fail(PAPACC_SCQ_START_ASSURANCE *g,DWORD e)
{if(g&&!g->first)g->first=e;if(g)g->phase=3U;return e;}
static int scqg_scope(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_OWNER_DECISION const *r;
 if(!g||g->self!=g||g->owner!=owner||g->thread!=GetCurrentThreadId()||!stop||
  g->first||(g->phase!=1U&&g->phase!=2U)||!g->source||scqa_current(g->source,owner,stop))return 0;
 r=g->source->registration;
 return r&&g->read_diagnostic<=1U&&g->read_diagnostic==r->read_diagnostic&&(g->read_diagnostic?(r->purpose==PAPACC_SCQ_PURPOSE_READ&&g->source->plan->read_diagnostic==1U&&g->source->plan->pre_readiness==1U):(r->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&!g->source->plan->read_diagnostic))&&
 !memcmp(g->attempt,r->attempt,32)&&!memcmp(g->policy,r->policy,32)&&
 !memcmp(g->request,r->request,32)&&!memcmp(g->decision,r->decision,32)&&
 !memcmp(g->target,r->target,32)&&!memcmp(g->image,r->image,32);
}
static DWORD scqg_capture_variant(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,
 PAPACC_SCQ_ADMIN_SOURCE *source,const PAPACC_SCQ_START_LIMITS *limits,volatile LONG *stop,int p1,int read_diagnostic)
{
 PAPACC_SCQ_OWNER_DECISION const *r;
 if(!g||g->phase||g->self||!owner||!source||!limits||!stop||source->phase!=1U||
  scqa_current(source,owner,stop)||!source->plan->pre_readiness||!source->plan->waiting_runtime||
  (read_diagnostic?(!p1||source->plan->read_diagnostic!=1U||source->registration->read_diagnostic!=1U||source->registration->purpose!=PAPACC_SCQ_PURPOSE_READ):(source->plan->read_diagnostic||source->registration->read_diagnostic||source->registration->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)))return ERROR_ACCESS_DENIED;
 if(!limits->private_growth||!limits->working_growth||!limits->physical_floor||!limits->commit_floor||
  !limits->max_handles||limits->max_handles>PAPACC_SCQ_HANDLE_CAP(p1)||!limits->freshness_ms||!limits->duration_ms)return ERROR_INVALID_DATA;
 if(p1&&!scqg_selected_exact_mode(limits,240000U,1))return ERROR_INVALID_DATA;
 g->p1_handles=p1?1U:0U;g->read_diagnostic=read_diagnostic?1U:0U;r=source->registration;g->self=g;g->owner=owner;g->source=source;g->thread=GetCurrentThreadId();g->phase=1U;g->limits=*limits;
 memcpy(g->attempt,r->attempt,32);memcpy(g->policy,r->policy,32);memcpy(g->request,r->request,32);
 memcpy(g->decision,r->decision,32);memcpy(g->target,r->target,32);memcpy(g->image,r->image,32);
 return 0; /* frozen candidate; no attribution, consumption or resource grant */
}
static DWORD scqg_capture_mode(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,PAPACC_SCQ_ADMIN_SOURCE *source,const PAPACC_SCQ_START_LIMITS *limits,volatile LONG *stop,int p1)
{return scqg_capture_variant(g,owner,source,limits,stop,p1,0);}
static DWORD scqg_capture_machine_read(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,PAPACC_SCQ_ADMIN_SOURCE *source,const PAPACC_SCQ_START_LIMITS *limits,volatile LONG *stop)
{return scqg_capture_variant(g,owner,source,limits,stop,1,1);}
static DWORD scqg_capture(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,PAPACC_SCQ_ADMIN_SOURCE *source,const PAPACC_SCQ_START_LIMITS *limits,volatile LONG *stop)
{return scqg_capture_mode(g,owner,source,limits,stop,0);}
static DWORD scqg_presented(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,volatile LONG *stop)
{if(!scqg_scope(g,owner,stop)||g->phase!=1U)return ERROR_ACCESS_DENIED;g->presented=1U;return 0;}
static int scqg_within(ULONGLONG ticks,ULONGLONG frequency,DWORD ms)
{
 ULONGLONG seconds=ms/1000U,fraction;
 if(!frequency||frequency>((ULONGLONG)-1)/1000U)return 0;
 if(ticks/frequency<seconds)return 1;if(ticks/frequency>seconds)return 0;
 fraction=((ULONGLONG)(ms%1000U)*frequency)/1000U;
 return ticks%frequency<=fraction;
}
static DWORD scqg_sample(PAPACC_SCQ_START_ASSURANCE *g,const PAPACC_SCM_RESOURCE_SAMPLE *s,ULONGLONG now,int creating)
{
 if(g->p1_handles&&(!g->initial_known||g->initial_handles>PAPACC_SCQ_P1_ENTRY||g->limits.max_handles!=PAPACC_SCQ_P1_CAP||g->limits.commit_floor!=PAPACC_SCQ_P1_COMMIT_FLOOR||g->limits.working_growth!=PAPACC_SCQ_P1_WORKING_GROWTH))return ERROR_INVALID_DATA;
 if(!s||!s->frequency||s->begin>s->end||now<s->end||
  !scqg_within(now-s->begin,s->frequency,g->limits.freshness_ms))return ERROR_INVALID_DATA;
 if(s->handles>g->limits.max_handles||s->handles>PAPACC_SCQ_HANDLE_CAP(g->p1_handles)||(creating&&s->handles>=PAPACC_SCQ_HANDLE_GUARD(g->p1_handles)))return ERROR_NOT_ENOUGH_QUOTA;
 if(!s->private_bytes||!s->working_set||s->physical_available<g->limits.physical_floor||s->commit_available<g->limits.commit_floor)return ERROR_NOT_ENOUGH_MEMORY;
 return 0;
}
static DWORD scqg_ready(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,
 const PAPACC_SCM_RESOURCE_SAMPLE *s,ULONGLONG now,volatile LONG *stop)
{
 DWORD e;
 if(!scqg_scope(g,owner,stop)||g->phase!=1U||!g->presented||g->source->phase!=3U||
  g->source->value.controller_consumed||g->source->endpoint_consumed||g->source->start_consumed||
  !scqa_endpoint_selection(g->source,owner,stop)||!scqa_start_selection(g->source,owner,stop))return ERROR_ACCESS_DENIED;
 e=scqg_sample(g,s,now,1);if(e)return scqg_fail(g,e);
 g->ready=*s;g->phase=2U;return 0; /* G0 LIMITED_RISK_ELIGIBLE only */
}
static DWORD scqg_controller_deadline(PAPACC_SCQ_START_ASSURANCE *g,ULONGLONG tick)
{
 ULONGLONG original;
 if(!g||g->phase!=2U||g->global_until||!g->source||!g->source->registration)return ERROR_INVALID_STATE;
 original=((ULONGLONG)g->source->registration->valid_until[1]<<32)|g->source->registration->valid_until[0];
 if(tick>((ULONGLONG)-1)-60000U||original<=tick)return scqg_fail(g,ERROR_TIMEOUT);
 g->global_until=tick+60000U;if(g->global_until>original)g->global_until=original;return 0;
}
static DWORD scqg_admit(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,
 const PAPACC_SCM_RESOURCE_SAMPLE *s,ULONGLONG now,int creating,volatile LONG *stop)
{
 DWORD e;
 if(!scqg_scope(g,owner,stop)||g->phase!=2U||!g->presented||
  !scqa_endpoint_selection(g->source,owner,stop)||!scqa_start_selection(g->source,owner,stop))return ERROR_ACCESS_DENIED;
 e=scqg_sample(g,s,now,creating);if(e)return scqg_fail(g,e);
 if(s->frequency!=g->ready.frequency||s->begin<g->ready.end||now<g->ready.end||
  !scqg_within(now-g->ready.end,s->frequency,g->limits.duration_ms))return scqg_fail(g,ERROR_TIMEOUT);
 if((s->private_bytes>g->ready.private_bytes&&s->private_bytes-g->ready.private_bytes>g->limits.private_growth)||
  (s->working_set>g->ready.working_set&&s->working_set-g->ready.working_set>g->limits.working_growth))return scqg_fail(g,ERROR_NOT_ENOUGH_MEMORY);
 return 0; /* do not clear opaque missing evidence or assert OS quiescence */
}
/* Policy-only QUERY resource custody. No source/confirmation is constructed. */
static DWORD scqg_query_ready_mode(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,
 const PAPACC_SCM_RESOURCE_SAMPLE *s,ULONGLONG now,ULONGLONG tick,ULONGLONG until,volatile LONG *stop,int p1)
{
 DWORD e;
 if(!g||g->self||g->phase||!owner||!stop||*stop||until<=tick||until-tick>60000U)return ERROR_ACCESS_DENIED;
 g->self=g;g->owner=owner;g->thread=GetCurrentThreadId();g->phase=5U;g->global_until=until;g->p1_handles=p1?1U:0U;
 if(p1)scqg_selected_p1(&g->limits);else scqg_selected(&g->limits);
 g->initial_known=1U;g->initial_handles=s?s->handles:0U;
 if(p1&&(!s||s->handles>PAPACC_SCQ_P1_ENTRY))return scqg_fail(g,ERROR_NOT_ENOUGH_QUOTA);
 e=scqg_sample(g,s,now,1);if(e)return scqg_fail(g,e);g->ready=*s;return 0;
}
static DWORD scqg_query_ready(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,const PAPACC_SCM_RESOURCE_SAMPLE *s,ULONGLONG now,ULONGLONG tick,ULONGLONG until,volatile LONG *stop)
{return scqg_query_ready_mode(g,owner,s,now,tick,until,stop,0);}
static DWORD scqg_query_admit(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,
 const PAPACC_SCM_RESOURCE_SAMPLE *s,ULONGLONG now,ULONGLONG tick,int creating,volatile LONG *stop)
{
 DWORD e;
 if(!g||g->self!=g||g->owner!=owner||g->thread!=GetCurrentThreadId()||g->phase!=5U||g->source||
 !stop||*stop||!g->global_until||tick>=g->global_until)return ERROR_ACCESS_DENIED;
 e=scqg_sample(g,s,now,creating);if(e)return scqg_fail(g,e);
 if(s->frequency!=g->ready.frequency||s->begin<g->ready.end||
 (s->private_bytes>g->ready.private_bytes&&s->private_bytes-g->ready.private_bytes>g->limits.private_growth)||
 (s->working_set>g->ready.working_set&&s->working_set-g->ready.working_set>g->limits.working_growth))return scqg_fail(g,ERROR_NOT_ENOUGH_MEMORY);
 return 0;
}
#endif
