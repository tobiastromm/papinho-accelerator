#include "papinho_c2_target_evidence.inc"
/* Borrow only the original first-reached slot. No resampling or expiry renewal. */
static int c2_resource_view(const papinho_c2_s6_facts *f,unsigned phase,unsigned metric,C2_TARGET_VIEW *v){
 const papinho_candidate_resource_sample *r;
 ZeroMemory(v,sizeof(*v));if(phase>=8U || metric>5U || !(f->resource_mask&(1UL<<phase)))return 0;
 r=&f->resource[phase].sample;v->version=1U;v->bytes=sizeof(*v);v->origin=1U;v->clock=1U;
 v->metric=metric;v->unit=metric==2U?2U:1U;v->scope=metric<3U?1U:(metric==3U?2U:3U);v->phase=phase;
 v->process=r->process;v->creation=r->creation;v->mapping=r->mapped_base;
 v->thread=f->owner_thread;v->thread_creation=f->native.thread_creation;
 v->root=f->native.root_identity;v->root_state=f->native.root_state;v->witness=f->resource[phase].witness;
 v->request=f->request;v->operation=f->operation;v->generation=f->generation;v->epoch=f->epoch;
 v->begin=r->begin_tick;v->end=r->end_tick;
 v->lifetime=f->resource[phase].retained?2U:(f->resource[phase].teardown?4U:(f->resource[phase].stopped?3U:1U));
 if(c2_resource_identity(f,r) && !r->identity_error && (r->presence&128U))v->presence|=1U;
 if(r->presence&(2UL<<metric))v->presence|=2U;
 if(metric==0U)v->value=r->private_bytes;else if(metric==1U)v->value=r->commit_bytes;
 else if(metric==2U)v->value=r->handles;else if(metric==3U)v->value=r->os_peak_commit;
 else if(metric==4U)v->value=r->available_physical;else v->value=r->available_pagefile;
 return 1;
}
