#include "issuer_core.h"
#include <string.h>
#define I_MAX ((PAPACC_U64)~(PAPACC_U64)0)
static int nz(const PAPACC_U8 *p,PAPACC_SIZE n){PAPACC_U8 v=0;PAPACC_SIZE i;for(i=0;i<n;i++)v|=p[i];return v!=0;}
static int request_ok(const PAPACC_ISSUER_REQUEST *q){
 return q&&nz(q->id,16)&&nz(q->device,16)&&nz(q->csr_digest,32)&&nz(q->key_digest,32)&&q->purpose>=1&&q->purpose<=3&&
 q->not_after>q->not_before&&q->not_after-q->not_before<=(PAPACC_U64)90U*86400U;
}
static int same_request(const PAPACC_ISSUER_REQUEST *a,const PAPACC_ISSUER_REQUEST *b){
 return !memcmp(a->id,b->id,16)&&!memcmp(a->device,b->device,16)&&!memcmp(a->csr_digest,b->csr_digest,32)&&
 !memcmp(a->key_digest,b->key_digest,32)&&!memcmp(a->predecessor,b->predecessor,16)&&a->purpose==b->purpose&&a->not_before==b->not_before&&a->not_after==b->not_after;
}
static void serial(PAPACC_U8 out[16],const PAPACC_U8 prefix[8],PAPACC_U64 n){unsigned i;memcpy(out,prefix,8);for(i=0;i<8;i++){out[15-i]=(PAPACC_U8)n;n>>=8;}}
int papacc_issuer_view_proofs_valid(const PAPACC_ISSUER_VIEW *v,PAPACC_U32 profile){
 PAPACC_U32 required;
 if(!v||v->continuity_profile!=profile)return 0;
 if(profile==PAPACC_ISSUER_CONTINUITY_RECOVERY){
  required=PAPACC_ISSUER_ROOT_PROOFS;
  if(v->proofs&~PAPACC_ISSUER_ROOT_PROOFS)return 0;
 }else if(profile==PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL){
  required=PAPACC_ISSUER_NORMAL_PROOFS;
  if(v->proofs&~PAPACC_ISSUER_NORMAL_PROOFS)return 0;
 }else return 0;
 return (v->proofs&required)==required;
}
static int view_ok(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_VIEW *v){
 if(!nz(v->root,32)||!nz(v->authority,16)||!nz(v->generation,16)||!nz(v->policy,32)||!nz(v->ca_spki_digest,32)||!nz(v->prefix,8)||(v->prefix[0]&128U)||
  !v->revision||!v->epoch||!v->administrative_epoch||!v->high_water||!v->ca_not_after||!papacc_issuer_view_proofs_valid(v,c->continuity_profile))return 0;
 if(c->initialized&& (memcmp(c->root_id,v->root,32)||memcmp(c->authority,v->authority,16)||memcmp(c->generation,v->generation,16)||memcmp(c->prefix,v->prefix,8)||memcmp(c->ca_spki_digest,v->ca_spki_digest,32)||
  v->revision<c->minimum_revision||v->high_water<c->minimum_counter))return 0;
 return 1;
}
static int record_ok(const PAPACC_ISSUER_RECORD *r,const PAPACC_ISSUER_VIEW *v){
 PAPACC_U8 sn[16];
 if(r->state<PAPACC_ISSUER_PREPARED||r->state>PAPACC_ISSUER_RECONCILIATION_REQUIRED||!request_ok(&r->request)||
  memcmp(r->authority,v->authority,16)||memcmp(r->generation,v->generation,16)||!nz(r->policy,32)||!r->approved_epoch)return 0;
 if(r->state==PAPACC_ISSUER_PREPARED&&(r->counter||nz(r->serial,16)))return 0;
 if(r->counter){serial(sn,v->prefix,r->counter);if(r->counter<2||r->counter>v->high_water||memcmp(sn,r->serial,16))return 0;}
 else if(r->state!=PAPACC_ISSUER_PREPARED&&r->state!=PAPACC_ISSUER_ABANDONED)return 0;
 if(r->state>=PAPACC_ISSUER_ISSUED_UNPUBLISHED&&r->state<=PAPACC_ISSUER_REVOKED){
  if(!r->certificate_size||!nz(r->certificate_digest,32))return 0;
  if(r->state!=PAPACC_ISSUER_ISSUED_UNPUBLISHED&&!r->published_epoch)return 0;
 }return 1;
}
static PAPACC_ISSUER_RESULT load(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16],PAPACC_ISSUER_VIEW *v,PAPACC_ISSUER_RECORD *r,int *found){
 PAPACC_ISSUER_RESULT s;memset(v,0,sizeof(*v));memset(r,0,sizeof(*r));*found=0;
 s=c->root.read(c->root.ctx,id,v,r,found);if(s!=PAPACC_ISSUER_OK){if(s==PAPACC_ISSUER_UNCERTAIN)c->blocked=1;return s;}
 if(!view_ok(c,v)||(*found&&(!record_ok(r,v)||memcmp(id,r->request.id,16)))){c->blocked=1;return PAPACC_ISSUER_UNCERTAIN;}
 c->minimum_revision=v->revision;c->minimum_counter=v->high_water;return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT enter(PAPACC_ISSUER_CORE *c,int observation){
 if(!c||c->initialized!=1)return PAPACC_ISSUER_INVALID;
 if(c->busy)return PAPACC_ISSUER_BUSY;
 if(c->pending.active)return c->pending.active==1U?PAPACC_ISSUER_PENDING:PAPACC_ISSUER_RETAINED;
 if(!observation&&(c->stopped||c->retained||c->blocked))return c->retained?PAPACC_ISSUER_RETAINED:c->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_UNCERTAIN;
 c->busy=1;return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT leave(PAPACC_ISSUER_CORE *c,PAPACC_ISSUER_RESULT s){c->busy=0;return s;}
static PAPACC_ISSUER_RESULT authorized(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_REQUEST *r,unsigned action){
 PAPACC_ISSUER_RESULT s=c->backend->authorize(c->backend->ctx,v,r,action);
 return c->stopped?PAPACC_ISSUER_STOPPED:s;
}
/* CAS success is insufficient by itself: re-read exact committed facts before
 * permitting a subsequent side effect. No retries on conflict/uncertainty. */
static PAPACC_ISSUER_RESULT verify_store(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_RECORD *next,PAPACC_U64 high,int publish){
 PAPACC_ISSUER_VIEW after;PAPACC_ISSUER_RECORD actual;int found;PAPACC_ISSUER_RESULT s;
 s=load(c,next->request.id,&after,&actual,&found);
 if(s!=PAPACC_ISSUER_OK||!found||after.revision<=v->revision||after.high_water!=high||after.epoch!=v->epoch||
  !after.administrative_epoch||after.administrative_epoch==v->administrative_epoch||actual.state!=next->state||
  !same_request(&actual.request,&next->request)||memcmp(actual.serial,next->serial,16)||actual.counter!=next->counter||
  memcmp(actual.policy,next->policy,32)||actual.approved_epoch!=next->approved_epoch||memcmp(actual.certificate_digest,next->certificate_digest,32)||
  actual.certificate_size!=next->certificate_size||(!publish&&actual.published_epoch!=next->published_epoch)||(publish&&(!actual.published_epoch||actual.published_epoch!=after.administrative_epoch))){c->blocked=1;return PAPACC_ISSUER_UNCERTAIN;}
 return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT store(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_RECORD *old,const PAPACC_ISSUER_RECORD *next,PAPACC_U64 high,const PAPACC_U8 *der,PAPACC_SIZE size,int publish){
 PAPACC_ISSUER_PENDING_STORE *p=&c->pending;PAPACC_ISSUER_RESULT s;
 if(p->active)return PAPACC_ISSUER_PENDING;
 if(v->revision==I_MAX||high<v->high_water)return PAPACC_ISSUER_LIMIT;
 memset(p,0,sizeof(*p));p->before=*v;p->next=*next;if(old){p->previous=*old;p->had_previous=1;}
 p->high=high;p->der=der;p->der_size=size;p->publish=publish?1U:0U;p->active=1U;
 s=c->root.cas(c->root.ctx,&p->before,p->had_previous?&p->previous:NULL,&p->next,high,der,size,publish);
 if(s==PAPACC_ISSUER_PENDING){
  if(c->root.poll)return s;
  p->active=2U;c->retained=1;c->blocked=1;return PAPACC_ISSUER_UNCERTAIN;
 }
 if(s==PAPACC_ISSUER_RETAINED){p->active=2U;c->retained=1;c->blocked=1;return s;}
 p->active=0;
 if(s!=PAPACC_ISSUER_OK){if(s==PAPACC_ISSUER_UNCERTAIN)c->blocked=1;return s;}
 return verify_store(c,&p->before,&p->next,high,publish);
}
PAPACC_ISSUER_RESULT papacc_issuer_interval(PAPACC_U64 ca_end,PAPACC_U64 begin,PAPACC_U64 requested,PAPACC_U64 *selected){
 PAPACC_U64 duration=(PAPACC_U64)90U*86400U,limit;
 if(!selected||!begin||requested<=begin||ca_end<=begin)return PAPACC_ISSUER_INVALID;
 limit=begin>I_MAX-duration?I_MAX:begin+duration;if(ca_end<limit)limit=ca_end;
 *selected=requested<limit?requested:limit;
 return requested>*selected?PAPACC_ISSUER_CONFIRMATION_REQUIRED:PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT init_profile(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_ROOT_PORT *root,const PAPACC_ISSUER_BACKEND *b,PAPACC_U32 profile){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r;PAPACC_U8 id[16]={0};int found;PAPACC_ISSUER_RESULT s;
 if(!c||c->initialized||c->busy||c->retained||c->lease||c->retained_output||c->pending.active)return PAPACC_ISSUER_INVALID;
 if(!root||!root->ctx||!root->read||!root->cas||!b||!b->authorize||!b->utc)return PAPACC_ISSUER_UNSUPPORTED;
#ifndef PAPACC_ISSUER_OFFLINE_TEST
 if(root->test_only||b->test_only)return PAPACC_ISSUER_DENIED;
#endif
 c->busy=1;c->root=*root;c->backend=b;c->continuity_profile=profile;
 s=load(c,id,&v,&r,&found);if(s!=PAPACC_ISSUER_OK){c->busy=0;return s;}
 memcpy(c->root_id,v.root,32);memcpy(c->authority,v.authority,16);memcpy(c->generation,v.generation,16);memcpy(c->prefix,v.prefix,8);memcpy(c->ca_spki_digest,v.ca_spki_digest,32);
 c->initialized=1;c->busy=0;return PAPACC_ISSUER_OK;
}
PAPACC_ISSUER_RESULT papacc_issuer_init(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_ROOT_PORT *r,const PAPACC_ISSUER_BACKEND *b){return init_profile(c,r,b,PAPACC_ISSUER_CONTINUITY_RECOVERY);}
PAPACC_ISSUER_RESULT papacc_issuer_init_normal(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_ROOT_PORT *r,const PAPACC_ISSUER_BACKEND *b){return init_profile(c,r,b,PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL);}
PAPACC_ISSUER_RESULT papacc_issuer_prepare(PAPACC_ISSUER_CORE *c,const PAPACC_ISSUER_REQUEST *q){
 PAPACC_ISSUER_VIEW v,pv;PAPACC_ISSUER_RECORD r,prev;PAPACC_ISSUER_RESULT s;int found;PAPACC_U64 now,selected;
 s=enter(c,0);if(s)return s;
 if(!q||!nz(q->id,16)||!nz(q->device,16)||!nz(q->csr_digest,32)||!nz(q->key_digest,32)||q->purpose<1||q->purpose>3||q->not_after<=q->not_before){return leave(c,PAPACC_ISSUER_INVALID);}
 s=load(c,q->id,&v,&r,&found);if(s)return leave(c,s);
 s=authorized(c,&v,q,PAPACC_ISSUER_PREPARE);if(s)return leave(c,s);
 if(found)return leave(c,same_request(q,&r.request)?PAPACC_ISSUER_OK:PAPACC_ISSUER_CONFLICT);
 s=c->backend->utc(c->backend->ctx,&now);if(s)return leave(c,s);
 if(c->stopped)return leave(c,PAPACC_ISSUER_STOPPED);
 if(q->not_before<now)return leave(c,PAPACC_ISSUER_DENIED);
 s=papacc_issuer_interval(v.ca_not_after,q->not_before,q->not_after,&selected);if(s)return leave(c,s);
 if(nz(q->predecessor,16)){
  if(!memcmp(q->predecessor,q->id,16))return leave(c,PAPACC_ISSUER_INVALID);
  s=load(c,q->predecessor,&pv,&prev,&found);if(s)return leave(c,s);
  if(!found||(prev.state!=PAPACC_ISSUER_PUBLISHED&&prev.state!=PAPACC_ISSUER_REVOKED)||memcmp(prev.request.device,q->device,16)||
   prev.request.purpose!=q->purpose||!memcmp(prev.request.key_digest,q->key_digest,32))return leave(c,PAPACC_ISSUER_DENIED);
  if(pv.revision!=v.revision)return leave(c,PAPACC_ISSUER_CONFLICT);
 }
 memset(&r,0,sizeof(r));r.request=*q;r.state=PAPACC_ISSUER_PREPARED;r.approved_epoch=v.epoch;
 memcpy(r.authority,v.authority,16);memcpy(r.generation,v.generation,16);memcpy(r.policy,v.policy,32);
 return leave(c,store(c,&v,NULL,&r,v.high_water,NULL,0,0));
}
PAPACC_ISSUER_RESULT papacc_issuer_reserve(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16]){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r,n;PAPACC_ISSUER_RESULT s;int found;
 s=enter(c,0);if(s)return s;if(!id)return leave(c,PAPACC_ISSUER_INVALID);
 s=load(c,id,&v,&r,&found);if(s||!found)return leave(c,s?s:PAPACC_ISSUER_ABSENT);
 s=authorized(c,&v,&r.request,PAPACC_ISSUER_RESERVE);if(s)return leave(c,s);
 if(r.state!=PAPACC_ISSUER_PREPARED)return leave(c,r.state==PAPACC_ISSUER_SERIAL_RESERVED?PAPACC_ISSUER_OK:PAPACC_ISSUER_CONFLICT);
 if(memcmp(r.policy,v.policy,32))return leave(c,PAPACC_ISSUER_DENIED);
 if(v.high_water==I_MAX)return leave(c,PAPACC_ISSUER_LIMIT);
 n=r;n.state=PAPACC_ISSUER_SERIAL_RESERVED;n.counter=v.high_water+1;serial(n.serial,v.prefix,n.counter);
 return leave(c,store(c,&v,&r,&n,n.counter,NULL,0,0));
}
static PAPACC_ISSUER_RESULT release_lease(PAPACC_ISSUER_CORE *c,PAPACC_ISSUER_RESULT s){
 PAPACC_ISSUER_RESULT r;if(!c->lease)return s;
 r=c->backend->release(c->backend->ctx,c->lease);if(r!=PAPACC_ISSUER_OK){c->retained=1;return PAPACC_ISSUER_RETAINED;}
 c->lease=NULL;return s;
}
/* Continuation of THIS invocation only, after its durable SIGN_REQUESTED.
 * Never resume an unknown marker found by a new/restarted invocation. */
static PAPACC_ISSUER_RESULT issue_after_marker(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16],const PAPACC_U8 *csr,PAPACC_SIZE csr_size,PAPACC_U8 *out,PAPACC_SIZE cap){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r,n;PAPACC_ISSUER_RESULT s;PAPACC_U64 now;PAPACC_SIZE size=0;int found;
 s=load(c,id,&v,&r,&found);if(s)return s;
 if(!found||r.state!=PAPACC_ISSUER_SIGN_REQUESTED)return PAPACC_ISSUER_UNCERTAIN;
 if(c->stopped)return PAPACC_ISSUER_STOPPED;
 s=authorized(c,&v,&r.request,PAPACC_ISSUER_SIGN);if(s)return s;
 if(memcmp(r.policy,v.policy,32))return PAPACC_ISSUER_DENIED;
 /* The caller's immutable CSR crossed owner turns: recheck the exact request. */
 s=c->backend->verify_request(c->backend->ctx,&r,csr,csr_size);if(s)return s;
 if(c->stopped)return PAPACC_ISSUER_STOPPED;
 s=c->backend->acquire(c->backend->ctx,&v,&r,&c->lease);
 if(s||!c->lease){c->blocked=1;return release_lease(c,s?s:PAPACC_ISSUER_UNCERTAIN);}
 s=c->backend->utc(c->backend->ctx,&now);
 if(s||now<r.request.not_before||now>=r.request.not_after||c->stopped)return release_lease(c,s?s:c->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_DENIED);
 c->retained_output=out;c->output_capacity=cap;
 s=c->backend->sign(c->backend->ctx,c->lease,&r,csr,csr_size,out,cap,&size);
 if(s||!size||size>cap||c->stopped){c->blocked=1;return release_lease(c,s?s:c->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_UNCERTAIN);}
 n=r;n.state=PAPACC_ISSUER_ISSUED_UNPUBLISHED;n.certificate_size=size;
 s=c->backend->verify_certificate(c->backend->ctx,&r,out,size,n.certificate_digest);
 if(s||!nz(n.certificate_digest,32)||c->stopped){c->blocked=1;return release_lease(c,s?s:PAPACC_ISSUER_UNCERTAIN);}
 s=c->backend->utc(c->backend->ctx,&now);
 if(s||now<r.request.not_before||now>=r.request.not_after||c->stopped){c->blocked=1;return release_lease(c,s?s:c->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_DENIED);}
 s=store(c,&v,&r,&n,v.high_water,out,size,0);
 if(s==PAPACC_ISSUER_PENDING){c->pending.continuation=3U;c->pending.output=out;c->pending.output_capacity=cap;return s;}
 if(s==PAPACC_ISSUER_OK){c->retained_output=NULL;c->output_capacity=0;}else c->blocked=1;
 return release_lease(c,s);
}
PAPACC_ISSUER_RESULT papacc_issuer_issue(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16],const PAPACC_U8 *csr,PAPACC_SIZE csr_size,PAPACC_U8 *out,PAPACC_SIZE cap){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r,n;PAPACC_ISSUER_RESULT s;PAPACC_U64 now;PAPACC_SIZE size=0;int found;
 s=enter(c,0);if(s)return s;
 if(!id||!csr||!csr_size||!out||!cap)return leave(c,PAPACC_ISSUER_INVALID);
 if(!c->backend->verify_certificate)return leave(c,PAPACC_ISSUER_UNSUPPORTED);
 s=load(c,id,&v,&r,&found);if(s||!found)return leave(c,s?s:PAPACC_ISSUER_ABSENT);
 s=authorized(c,&v,&r.request,(r.state>=PAPACC_ISSUER_ISSUED_UNPUBLISHED&&r.state<=PAPACC_ISSUER_REVOKED)?PAPACC_ISSUER_QUERY:PAPACC_ISSUER_SIGN);if(s)return leave(c,s);
 if(r.state>=PAPACC_ISSUER_ISSUED_UNPUBLISHED&&r.state<=PAPACC_ISSUER_REVOKED){
  PAPACC_U8 digest[32];
  if(!c->root.copy_certificate)return leave(c,PAPACC_ISSUER_UNSUPPORTED);
  s=c->root.copy_certificate(c->root.ctx,id,r.certificate_digest,out,cap,&size);if(s)return leave(c,s);
  if(size!=r.certificate_size||size>cap)return leave(c,PAPACC_ISSUER_UNCERTAIN);
  s=c->backend->verify_certificate(c->backend->ctx,&r,out,size,digest);
  return leave(c,s?s:memcmp(digest,r.certificate_digest,32)?PAPACC_ISSUER_UNCERTAIN:PAPACC_ISSUER_OK);
 }
 if(!c->backend->verify_request||!c->backend->acquire||!c->backend->sign||!c->backend->release)return leave(c,PAPACC_ISSUER_UNSUPPORTED);
 if(r.state!=PAPACC_ISSUER_SERIAL_RESERVED)return leave(c,PAPACC_ISSUER_UNCERTAIN);
 if(memcmp(r.policy,v.policy,32))return leave(c,PAPACC_ISSUER_DENIED);
 s=c->backend->utc(c->backend->ctx,&now);if(s)return leave(c,s);
 if(now<r.request.not_before||now>=r.request.not_after||c->stopped)return leave(c,c->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_DENIED);
 s=c->backend->verify_request(c->backend->ctx,&r,csr,csr_size);if(s)return leave(c,s);
 if(c->stopped)return leave(c,PAPACC_ISSUER_STOPPED);
 n=r;n.state=PAPACC_ISSUER_SIGN_REQUESTED;s=store(c,&v,&r,&n,v.high_water,NULL,0,0);
 if(s==PAPACC_ISSUER_PENDING){
  c->pending.continuation=1U;c->pending.csr=csr;c->pending.csr_size=csr_size;
  c->pending.output=out;c->pending.output_capacity=cap;c->retained_output=out;c->output_capacity=cap;
  return leave(c,s);
 }
 if(s)return leave(c,s);
 return leave(c,issue_after_marker(c,id,csr,csr_size,out,cap));
}
PAPACC_ISSUER_RESULT papacc_issuer_query(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16],PAPACC_ISSUER_RECORD *out){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r;PAPACC_ISSUER_RESULT s;int found;
 s=enter(c,1);if(s)return s;if(!id||!out)return leave(c,PAPACC_ISSUER_INVALID);
 s=load(c,id,&v,&r,&found);if(s||!found)return leave(c,s?s:PAPACC_ISSUER_ABSENT);
 s=c->backend->authorize(c->backend->ctx,&v,&r.request,PAPACC_ISSUER_QUERY); /* Read-only after Stop; exactly one authorization. */
 if(!s)*out=r;return leave(c,s);
}
static PAPACC_ISSUER_RESULT revoke_after_commit(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16]){
 PAPACC_ISSUER_VIEW v,after;PAPACC_ISSUER_RECORD r,n;PAPACC_ISSUER_RESULT s;int found;
 s=load(c,id,&v,&r,&found);
 if(s||!found||r.state!=PAPACC_ISSUER_REVOKED){c->blocked=1;return PAPACC_ISSUER_UNCERTAIN;}
 s=c->root.revoke_dependents(c->root.ctx,&v,&r);
 if(s==PAPACC_ISSUER_OK){
  s=load(c,id,&after,&n,&found);
  if(s!=PAPACC_ISSUER_OK||!found||n.state!=PAPACC_ISSUER_REVOKED||after.revision!=v.revision||after.epoch!=v.epoch||after.administrative_epoch!=v.administrative_epoch||
     after.high_water!=v.high_water||!same_request(&r.request,&n.request)||memcmp(r.certificate_digest,n.certificate_digest,32))s=PAPACC_ISSUER_UNCERTAIN;
 }
 if(s!=PAPACC_ISSUER_OK){c->blocked=1;if(s==PAPACC_ISSUER_RETAINED)c->retained=1;}
 return s;
}
PAPACC_ISSUER_RESULT papacc_issuer_transition(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16],PAPACC_ISSUER_STATE target){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r,n;PAPACC_ISSUER_RESULT s;int found;unsigned action;PAPACC_U64 now;
 s=enter(c,0);if(s)return s;if(!id)return leave(c,PAPACC_ISSUER_INVALID);
 if(target!=PAPACC_ISSUER_PUBLISHED&&target!=PAPACC_ISSUER_REVOKED&&target!=PAPACC_ISSUER_ABANDONED)return leave(c,PAPACC_ISSUER_INVALID);
 s=load(c,id,&v,&r,&found);if(s||!found)return leave(c,s?s:PAPACC_ISSUER_ABSENT);
 action=target==PAPACC_ISSUER_PUBLISHED?PAPACC_ISSUER_PUBLISH:target==PAPACC_ISSUER_REVOKED?PAPACC_ISSUER_REVOKE:PAPACC_ISSUER_RECOVER;
 s=authorized(c,&v,&r.request,action);if(s)return leave(c,s);
 if(target==PAPACC_ISSUER_REVOKED&&c->continuity_profile==PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL&&!c->root.revoke_dependents)return leave(c,PAPACC_ISSUER_UNSUPPORTED);
 if(r.state==target){
  /* A persisted revoked row alone is not evidence of runtime fanout. */
  if(target!=PAPACC_ISSUER_REVOKED||c->continuity_profile!=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL)return leave(c,PAPACC_ISSUER_OK);
  return leave(c,revoke_after_commit(c,id));
 }
 if((target==PAPACC_ISSUER_PUBLISHED&&(r.state!=PAPACC_ISSUER_ISSUED_UNPUBLISHED||memcmp(r.policy,v.policy,32)))||
 (target==PAPACC_ISSUER_REVOKED&&r.state!=PAPACC_ISSUER_PUBLISHED)||
 (target==PAPACC_ISSUER_ABANDONED&&r.state!=PAPACC_ISSUER_PREPARED&&r.state!=PAPACC_ISSUER_SERIAL_RESERVED))return leave(c,PAPACC_ISSUER_DENIED);
 if(target==PAPACC_ISSUER_PUBLISHED){s=c->backend->utc(c->backend->ctx,&now);if(s||c->stopped||now<r.request.not_before||now>=r.request.not_after)return leave(c,s?s:c->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_DENIED);}
 n=r;n.state=target;s=store(c,&v,&r,&n,v.high_water,NULL,0,target==PAPACC_ISSUER_PUBLISHED);
 if(s==PAPACC_ISSUER_PENDING&&target==PAPACC_ISSUER_REVOKED&&c->continuity_profile==PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL)c->pending.continuation=2U;
 if(s||target!=PAPACC_ISSUER_REVOKED||c->continuity_profile!=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL)return leave(c,s);
 return leave(c,revoke_after_commit(c,id));
}
PAPACC_ISSUER_RESULT papacc_issuer_poll(PAPACC_ISSUER_CORE *c){
 PAPACC_ISSUER_PENDING_STORE *p;PAPACC_ISSUER_RESULT s;unsigned continuation;
 const PAPACC_U8 *csr;PAPACC_U8 *output;PAPACC_SIZE csr_size,capacity;PAPACC_U8 id[16];
 if(!c||c->initialized!=1)return PAPACC_ISSUER_INVALID;
 if(c->busy)return PAPACC_ISSUER_BUSY;p=&c->pending;
 if(p->active==2U)return PAPACC_ISSUER_RETAINED;
 if(p->active!=1U||!c->root.poll)return PAPACC_ISSUER_INVALID;
 c->busy=1;s=c->root.poll(c->root.ctx,&p->before,&p->next,c->stopped);
 if(s==PAPACC_ISSUER_PENDING)return leave(c,s);
 if(s!=PAPACC_ISSUER_OK){
  c->blocked=1;
  if(s==PAPACC_ISSUER_CONFLICT)p->active=0;
  else{p->active=2U;c->retained=1;}
  return leave(c,s);
 }
 s=verify_store(c,&p->before,&p->next,p->high,(int)p->publish);
 if(s){p->active=2U;c->retained=1;c->blocked=1;return leave(c,s);}
 continuation=p->continuation;csr=p->csr;csr_size=p->csr_size;output=p->output;capacity=p->output_capacity;
 memcpy(id,p->next.request.id,16);p->active=0;
 if(continuation==3U){
  c->retained_output=NULL;c->output_capacity=0;
  return leave(c,release_lease(c,c->stopped?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_OK));
 }
 if(c->stopped){
  if(continuation==2U){p->active=2U;c->retained=1;}
  c->blocked=1;c->retained_output=NULL;c->output_capacity=0;return leave(c,PAPACC_ISSUER_STOPPED);
 }
 if(continuation==1U){
  /* Root finished borrowing this marker. Its inputs now belong to the one
   * continuation, which may create a distinct pending DER store. */
  c->retained_output=NULL;c->output_capacity=0;
  return leave(c,issue_after_marker(c,id,csr,csr_size,output,capacity));
 }
 if(continuation==2U)return leave(c,revoke_after_commit(c,id));
 if(continuation==4U)return leave(c,PAPACC_ISSUER_UNCERTAIN);
 return leave(c,PAPACC_ISSUER_OK);
}
PAPACC_ISSUER_RESULT papacc_issuer_recover(PAPACC_ISSUER_CORE *c,const PAPACC_U8 id[16]){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r,n;PAPACC_ISSUER_RESULT s;int found;
 s=enter(c,1);if(s)return s;if(!id)return leave(c,PAPACC_ISSUER_INVALID);
 if(c->continuity_profile!=PAPACC_ISSUER_CONTINUITY_RECOVERY)return leave(c,PAPACC_ISSUER_DENIED);
 if(c->retained||c->lease||c->retained_output)return leave(c,PAPACC_ISSUER_RETAINED);
 s=load(c,id,&v,&r,&found);if(s)return leave(c,s);
 if(!found)return leave(c,PAPACC_ISSUER_ABSENT);
 s=authorized(c,&v,&r.request,PAPACC_ISSUER_RECOVER);if(s)return leave(c,s);
 if(r.state==PAPACC_ISSUER_SIGN_REQUESTED){n=r;n.state=PAPACC_ISSUER_RECONCILIATION_REQUIRED;s=store(c,&v,&r,&n,v.high_water,NULL,0,0);if(s==PAPACC_ISSUER_PENDING)c->pending.continuation=4U;return leave(c,s?s:PAPACC_ISSUER_UNCERTAIN);}
 if(r.state==PAPACC_ISSUER_RECONCILIATION_REQUIRED)return leave(c,PAPACC_ISSUER_UNCERTAIN);
 c->blocked=0;return leave(c,PAPACC_ISSUER_OK);
}
void papacc_issuer_stop(PAPACC_ISSUER_CORE *c){if(c&&c->initialized==1)c->stopped=1;}
PAPACC_ISSUER_RESULT papacc_issuer_finish(PAPACC_ISSUER_CORE *c){
 if(!c||!c->initialized)return PAPACC_ISSUER_INVALID;if(c->busy)return PAPACC_ISSUER_BUSY;
 if(c->pending.active)return PAPACC_ISSUER_RETAINED;
 if(c->lease||c->retained||c->retained_output)return PAPACC_ISSUER_RETAINED;
 if(!c->stopped)return PAPACC_ISSUER_DENIED;c->initialized=2;return PAPACC_ISSUER_OK;
}
