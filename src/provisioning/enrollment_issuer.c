#include "enrollment_issuer.h"
#include <string.h>
static int ei_overlap(const void *p,PAPACC_SIZE n,const void *q,PAPACC_SIZE m)
{PAPACC_SIZE a=(PAPACC_SIZE)p,b=(PAPACC_SIZE)q;if(n>(PAPACC_SIZE)-1-a||m>(PAPACC_SIZE)-1-b)return 1;return n&&m&&(a<=b?b-a<n:a-b<m);}
static int ei_buffer(const PAPACC_ISSUER_CORE *c,const EP_TRACE *t,const EP_PAIR *p,const void *buffer,PAPACC_SIZE n)
{return buffer&&n&&!ei_overlap(buffer,n,c,sizeof(*c))&&!ei_overlap(buffer,n,t,sizeof(*t))&&!ei_overlap(buffer,n,p,sizeof(*p));}
static int ei_request_equal(const PAPACC_ISSUER_REQUEST *a,const PAPACC_ISSUER_REQUEST *b)
{return !memcmp(a->id,b->id,16)&&!memcmp(a->device,b->device,16)&&!memcmp(a->csr_digest,b->csr_digest,32)&&!memcmp(a->key_digest,b->key_digest,32)&&!memcmp(a->predecessor,b->predecessor,16)&&a->purpose==b->purpose&&a->not_before==b->not_before&&a->not_after==b->not_after;}
static PAPACC_ISSUER_RESULT ei_candidate(PAPACC_ISSUER_CORE *c,const EP_TRACE *t,const EP_PAIR *p,PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now,EP_SLICE *csr)
{
 EP_RESULT r;
 if(!c||!t||!p||ei_overlap(c,sizeof(*c),t,sizeof(*t))||ei_overlap(c,sizeof(*c),p,sizeof(*p))||ei_overlap(t,sizeof(*t),p,sizeof(*p)))return PAPACC_ISSUER_INVALID;
 if(c->initialized!=1||c->continuity_profile!=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL)return PAPACC_ISSUER_DENIED;
 if(c->busy)return PAPACC_ISSUER_BUSY;if(c->stopped)return PAPACC_ISSUER_STOPPED;
 if(c->retained||c->blocked)return PAPACC_ISSUER_RETAINED;
 r=ep_trace_csr_candidate(t,p,connection,generation,now,csr);
 return r==EP_DATA_ONLY?PAPACC_ISSUER_OK:r==EP_INVALID?PAPACC_ISSUER_INVALID:r==EP_STOPPED?PAPACC_ISSUER_STOPPED:PAPACC_ISSUER_DENIED;
}
static PAPACC_ISSUER_RESULT ei_record(PAPACC_ISSUER_CORE *c,const EP_TRACE *t,PAPACC_ISSUER_RECORD *record)
{
 PAPACC_ISSUER_VIEW view;PAPACC_ISSUER_RECORD exact;int found=0;
 PAPACC_ISSUER_RESULT r=papacc_issuer_query(c,t->request,record);
 if(r)return r;
 if(record->request.purpose!=PAPACC_ISSUER_CLIENT||memcmp(record->request.id,t->request,16))return PAPACC_ISSUER_DENIED;
 /* An old row/received APPROVED is not current policy. Use only the original
  * core root port, in the same serialized owner turn; never a supplied grant. */
 r=c->root.read(c->root.ctx,t->request,&view,&exact,&found);if(r)return r;
 if(!found||!papacc_issuer_view_proofs_valid(&view,PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL)||
  view.revision<c->minimum_revision||view.high_water<c->minimum_counter||memcmp(view.root,c->root_id,32)||
  !view.administrative_epoch||memcmp(view.prefix,c->prefix,8)||memcmp(view.authority,c->authority,16)||memcmp(view.generation,c->generation,16)||memcmp(view.ca_spki_digest,c->ca_spki_digest,32)||
  view.epoch!=record->approved_epoch||memcmp(view.policy,record->policy,32))return PAPACC_ISSUER_DENIED;
 if(exact.state!=record->state||!ei_request_equal(&exact.request,&record->request)||
  memcmp(exact.authority,record->authority,16)||memcmp(exact.generation,record->generation,16)||memcmp(exact.serial,record->serial,16)||
  memcmp(exact.policy,record->policy,32)||memcmp(exact.certificate_digest,record->certificate_digest,32)||
  exact.counter!=record->counter||exact.approved_epoch!=record->approved_epoch||exact.published_epoch!=record->published_epoch||exact.certificate_size!=record->certificate_size)return PAPACC_ISSUER_CONFLICT;
 return PAPACC_ISSUER_OK;
}
PAPACC_ISSUER_RESULT ep_issuer_submit(PAPACC_ISSUER_CORE *c,const EP_TRACE *t,const EP_PAIR *p,PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now,PAPACC_U8 *quarantine,PAPACC_SIZE cap)
{
 EP_SLICE csr;PAPACC_ISSUER_RECORD record;PAPACC_ISSUER_RESULT r=ei_candidate(c,t,p,connection,generation,now,&csr);
 if(r)return r;if(!ei_buffer(c,t,p,quarantine,cap))return PAPACC_ISSUER_INVALID;
 if(c->pending.active)return PAPACC_ISSUER_BUSY;
 r=ei_record(c,t,&record);if(r)return r;
 /* No prepare/reserve/approval inferred from a wire APPROVED or a token. */
 if(record.state!=PAPACC_ISSUER_SERIAL_RESERVED)return PAPACC_ISSUER_CONFLICT;
 return papacc_issuer_issue(c,t->request,csr.data,csr.size,quarantine,cap);
}
PAPACC_ISSUER_RESULT ep_issuer_poll(PAPACC_ISSUER_CORE *c,const EP_TRACE *t,const EP_PAIR *p,PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now,PAPACC_U8 *quarantine,PAPACC_SIZE cap)
{
 EP_SLICE csr;PAPACC_ISSUER_RESULT r=ei_candidate(c,t,p,connection,generation,now,&csr);
 if(r==PAPACC_ISSUER_INVALID||r==PAPACC_ISSUER_BUSY)return r;
 if(r&&!(r==PAPACC_ISSUER_STOPPED&&c&&c->stopped&&c->pending.active&&p->connection==connection&&p->generation==generation&&
   !memcmp(t->request,p->request,16)&&!memcmp(t->client_nonce,p->client_nonce,32)&&!memcmp(t->server_nonce,p->server_nonce,32))){
  /* No implicit global Stop, release or retry for an expired/lost connection.
   * Explicit issuer Stop above permits ONLY its factual core poll below. */
  if(c&&c->pending.active)return PAPACC_ISSUER_RETAINED;return r;
 }
 if(!ei_buffer(c,t,p,quarantine,cap)||!c->pending.active||memcmp(c->pending.next.request.id,t->request,16))return PAPACC_ISSUER_INVALID;
 if(!c->pending.publish&&(c->pending.output!=quarantine||c->pending.output_capacity!=cap||
   (c->pending.csr&&(c->pending.csr!=t->csr||c->pending.csr_size!=t->csr_size))))return PAPACC_ISSUER_CONFLICT;
 return papacc_issuer_poll(c);
}
PAPACC_ISSUER_RESULT ep_issuer_publish(PAPACC_ISSUER_CORE *c,const EP_TRACE *t,const EP_PAIR *p,PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now)
{
 EP_SLICE csr;PAPACC_ISSUER_RECORD record;PAPACC_ISSUER_RESULT r=ei_candidate(c,t,p,connection,generation,now,&csr);
 if(r)return r;if(c->pending.active)return PAPACC_ISSUER_BUSY;
 r=ei_record(c,t,&record);if(r)return r;
 if(record.state!=PAPACC_ISSUER_ISSUED_UNPUBLISHED)return PAPACC_ISSUER_CONFLICT;
 /* Actual PUBLISH authorization and original ack remain in the core/root. */
 return papacc_issuer_transition(c,t->request,PAPACC_ISSUER_PUBLISHED);
}
PAPACC_ISSUER_RESULT ep_issuer_deliver(PAPACC_ISSUER_CORE *c,EP_TRACE *t,const EP_PAIR *p,PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now,PAPACC_U8 *quarantine,PAPACC_SIZE cap,PAPACC_U8 *out,PAPACC_SIZE outcap,PAPACC_SIZE *used)
{
 EP_SLICE csr;PAPACC_ISSUER_RECORD before,after;PAPACC_ISSUER_RESULT r=ei_candidate(c,t,p,connection,generation,now,&csr);
 if(r)return r;
 if(!used||!ei_buffer(c,t,p,quarantine,cap)||!ei_buffer(c,t,p,out,outcap)||!ei_buffer(c,t,p,used,sizeof(*used))||
  ei_overlap(quarantine,cap,out,outcap)||ei_overlap(used,sizeof(*used),out,outcap)||ei_overlap(used,sizeof(*used),quarantine,cap))return PAPACC_ISSUER_INVALID;
 if(c->pending.active)return PAPACC_ISSUER_RETAINED;
 r=ei_record(c,t,&before);if(r)return r;
 if(before.state!=PAPACC_ISSUER_PUBLISHED||!before.published_epoch)return PAPACC_ISSUER_DENIED;
 if(!before.certificate_size||before.certificate_size>cap||before.certificate_size>outcap)return PAPACC_ISSUER_LIMIT;
 if(!c->backend||!c->backend->verify_request_data)return PAPACC_ISSUER_UNSUPPORTED;
 r=c->backend->verify_request_data(c->backend->ctx,&before,csr.data,csr.size);if(r)return r;
 r=papacc_issuer_issue(c,t->request,csr.data,csr.size,quarantine,cap);if(r)return r; /* verified existing DER; never resign */
 r=ei_candidate(c,t,p,connection,generation,now,&csr);if(r)return r;
 r=ei_record(c,t,&after);if(r)return r;
 if(after.state!=PAPACC_ISSUER_PUBLISHED||!ei_request_equal(&after.request,&before.request)||after.counter!=before.counter||memcmp(after.policy,before.policy,32)||after.published_epoch!=before.published_epoch||after.certificate_size!=before.certificate_size||
  memcmp(after.certificate_digest,before.certificate_digest,32)||memcmp(after.authority,before.authority,16)||memcmp(after.generation,before.generation,16)||
  memcmp(after.serial,before.serial,16)||after.approved_epoch!=before.approved_epoch)return PAPACC_ISSUER_CONFLICT;
 memcpy(out,quarantine,before.certificate_size);*used=before.certificate_size;t->credential_released=1;return PAPACC_ISSUER_OK;
}
