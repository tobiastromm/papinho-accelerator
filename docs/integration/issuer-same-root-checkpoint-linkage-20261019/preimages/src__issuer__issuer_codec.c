#include "issuer_codec.h"
#include <string.h>
#define FOOT (PAPACC_ISSUER_IMAGE_BYTES-32U)
static void w32(PAPACC_U8 *p,PAPACC_U32 x){unsigned i;for(i=0;i<4;i++){p[3-i]=(PAPACC_U8)x;x>>=8;}}
static PAPACC_U32 r32(const PAPACC_U8 *p){unsigned i;PAPACC_U32 x=0;for(i=0;i<4;i++)x=(x<<8)|p[i];return x;}
static void w64(PAPACC_U8 *p,PAPACC_U64 x){unsigned i;for(i=0;i<8;i++){p[7-i]=(PAPACC_U8)x;x>>=8;}}
static PAPACC_U64 r64(const PAPACC_U8 *p){unsigned i;PAPACC_U64 x=0;for(i=0;i<8;i++)x=(x<<8)|p[i];return x;}
static int zero(const PAPACC_U8 *p,PAPACC_SIZE n){PAPACC_SIZE i;for(i=0;i<n;i++)if(p[i])return 0;return 1;}
static void record_encode(PAPACC_U8 *p,const PAPACC_ISSUER_RECORD *r){
 memcpy(p,r->request.id,16);memcpy(p+16,r->request.device,16);memcpy(p+32,r->request.csr_digest,32);memcpy(p+64,r->request.key_digest,32);memcpy(p+96,r->request.predecessor,16);
 w32(p+112,r->request.purpose);w64(p+116,r->request.not_before);w64(p+124,r->request.not_after);
 memcpy(p+132,r->authority,16);memcpy(p+148,r->generation,16);memcpy(p+164,r->policy,32);memcpy(p+196,r->serial,16);memcpy(p+212,r->certificate_digest,32);
 w64(p+244,r->counter);w64(p+252,r->approved_epoch);w64(p+260,r->published_epoch);w32(p+268,(PAPACC_U32)r->certificate_size);w32(p+272,(PAPACC_U32)r->state);
}
static void record_decode(const PAPACC_U8 *p,PAPACC_ISSUER_RECORD *r){
 memset(r,0,sizeof(*r));memcpy(r->request.id,p,16);memcpy(r->request.device,p+16,16);memcpy(r->request.csr_digest,p+32,32);memcpy(r->request.key_digest,p+64,32);memcpy(r->request.predecessor,p+96,16);
 r->request.purpose=r32(p+112);r->request.not_before=r64(p+116);r->request.not_after=r64(p+124);
 memcpy(r->authority,p+132,16);memcpy(r->generation,p+148,16);memcpy(r->policy,p+164,32);memcpy(r->serial,p+196,16);memcpy(r->certificate_digest,p+212,32);
 r->counter=r64(p+244);r->approved_epoch=r64(p+252);r->published_epoch=r64(p+260);r->certificate_size=r32(p+268);r->state=(PAPACC_ISSUER_STATE)r32(p+272);
}
static int latest(const PAPACC_U8 *b,unsigned until,const PAPACC_U8 id[16],PAPACC_ISSUER_RECORD *r){
 unsigned i;PAPACC_SIZE p=256;int found=0;for(i=0;i<until;i++){if(!memcmp(b+p+48,id,16)){record_decode(b+p+48,r);found=1;}p+=r32(b+p+8);}return found;
}
static int transition(const PAPACC_U8 *b,unsigned count,const PAPACC_ISSUER_RECORD *n,PAPACC_U64 *high){
 PAPACC_ISSUER_RECORD old,prev;PAPACC_U8 a[276],d[276],sn[16];PAPACC_U64 x;unsigned i;int found=latest(b,count,n->request.id,&old);
 if(zero(n->request.id,16)||zero(n->request.device,16)||zero(n->request.csr_digest,32)||zero(n->request.key_digest,32)||n->request.purpose<1||n->request.purpose>3||
  n->request.not_after<=n->request.not_before||n->request.not_after-n->request.not_before>(PAPACC_U64)90U*86400U||n->request.not_after>r64(b+176)||!n->approved_epoch||
  memcmp(n->authority,b+64,16)||memcmp(n->generation,b+80,16)||memcmp(n->policy,b+104,32)||n->certificate_size>FOOT)return 0;
 if(!found){
  if(n->state!=PAPACC_ISSUER_PREPARED||n->counter||!zero(n->serial,16)||n->certificate_size||!zero(n->certificate_digest,32)||n->published_epoch)return 0;
  if(!zero(n->request.predecessor,16)){
   if(!memcmp(n->request.id,n->request.predecessor,16)||!latest(b,count,n->request.predecessor,&prev)||
    (prev.state!=PAPACC_ISSUER_PUBLISHED&&prev.state!=PAPACC_ISSUER_REVOKED)||memcmp(prev.request.device,n->request.device,16)||prev.request.purpose!=n->request.purpose||!memcmp(prev.request.key_digest,n->request.key_digest,32))return 0;
  }return 1;
 }
 record_encode(a,&old);record_encode(d,n);
 if(memcmp(a,d,196)||old.approved_epoch!=n->approved_epoch)return 0;
 if(old.state==PAPACC_ISSUER_PREPARED&&n->state==PAPACC_ISSUER_SERIAL_RESERVED){
  if(*high==(PAPACC_U64)-1||n->counter!=*high+1)return 0;x=n->counter;memcpy(sn,b+96,8);for(i=0;i<8;i++){sn[15-i]=(PAPACC_U8)x;x>>=8;}
  if(memcmp(sn,n->serial,16))return 0;*high=n->counter;
 }else{
  if(old.counter!=n->counter||memcmp(old.serial,n->serial,16))return 0;
  if(!((old.state==PAPACC_ISSUER_PREPARED&&n->state==PAPACC_ISSUER_ABANDONED)||
   (old.state==PAPACC_ISSUER_SERIAL_RESERVED&&(n->state==PAPACC_ISSUER_SIGN_REQUESTED||n->state==PAPACC_ISSUER_ABANDONED))||
   (old.state==PAPACC_ISSUER_SIGN_REQUESTED&&(n->state==PAPACC_ISSUER_ISSUED_UNPUBLISHED||n->state==PAPACC_ISSUER_RECONCILIATION_REQUIRED))||
   (old.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED&&n->state==PAPACC_ISSUER_PUBLISHED)||
   (old.state==PAPACC_ISSUER_PUBLISHED&&n->state==PAPACC_ISSUER_REVOKED)))return 0;
 }
 if(n->state==PAPACC_ISSUER_ISSUED_UNPUBLISHED){if(!n->certificate_size||zero(n->certificate_digest,32)||n->published_epoch)return 0;}
 else if(n->certificate_size!=old.certificate_size||memcmp(n->certificate_digest,old.certificate_digest,32))return 0;
 if(n->state==PAPACC_ISSUER_PUBLISHED){if(!n->published_epoch)return 0;}else if(n->published_epoch!=old.published_epoch)return 0;
 return 1;
}
PAPACC_ISSUER_RESULT papacc_issuer_image_validate(const PAPACC_U8 *b,PAPACC_ISSUER_VIEW *v,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_U8 d[32],chain[32]={0};PAPACC_SIZE p=256,len,der;unsigned i,n;PAPACC_U64 high=1,base;PAPACC_ISSUER_RECORD r;
 if(!b||!v||!hash)return PAPACC_ISSUER_INVALID;memset(v,0,sizeof(*v));
 if(memcmp(b,"PACIS601",8)||r32(b+8)!=1||r32(b+12)<256||r32(b+12)>FOOT||!zero(b+20,4)||zero(b+32,32)||zero(b+64,16)||zero(b+80,16)||zero(b+96,8)||(b[96]&128U)||zero(b+104,32)||zero(b+136,32)||zero(b+192,32))return PAPACC_ISSUER_INVALID;
 if(!hash(ctx,b,FOOT,d))return PAPACC_ISSUER_UNCERTAIN;if(memcmp(d,b+FOOT,32))return PAPACC_ISSUER_INVALID;
 base=r64(b+24);n=r32(b+16);if(!base||n>(FOOT-256)/PAPACC_ISSUER_EVENT_FIXED||base>(PAPACC_U64)-1-n||r64(b+184)!=base+n||!r64(b+176))return PAPACC_ISSUER_INVALID;
 for(i=0;i<n;i++){
  if(p>FOOT-PAPACC_ISSUER_EVENT_FIXED)return PAPACC_ISSUER_INVALID;len=r32(b+p+8);der=r32(b+p+12);
  if(len<PAPACC_ISSUER_EVENT_FIXED||len>FOOT-p||der!=len-PAPACC_ISSUER_EVENT_FIXED||r64(b+p)!=i+1U||memcmp(b+p+16,chain,32))return PAPACC_ISSUER_INVALID;
  if(!hash(ctx,b+p,len-32,d))return PAPACC_ISSUER_UNCERTAIN;if(memcmp(d,b+p+len-32,32))return PAPACC_ISSUER_INVALID;
  record_decode(b+p+48,&r);if(!transition(b,i,&r,&high))return PAPACC_ISSUER_INVALID;
  if(r.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED){if(der!=r.certificate_size||!hash(ctx,b+p+324,der,d))return PAPACC_ISSUER_INVALID;if(memcmp(d,r.certificate_digest,32))return PAPACC_ISSUER_INVALID;}
  else if(der)return PAPACC_ISSUER_INVALID;
  memcpy(chain,b+p+len-32,32);p+=len;
 }
 if(p!=r32(b+12)||high!=r64(b+168)||memcmp(chain,b+224,32)||!zero(b+p,FOOT-p))return PAPACC_ISSUER_INVALID;
 memcpy(v->root,b+32,32);memcpy(v->authority,b+64,16);memcpy(v->generation,b+80,16);memcpy(v->prefix,b+96,8);memcpy(v->policy,b+104,32);memcpy(v->ca_spki_digest,b+136,32);
 v->revision=r64(b+184);v->high_water=high;v->ca_not_after=r64(b+176);/* epoch/proofs remain absent */return PAPACC_ISSUER_OK;
}
PAPACC_ISSUER_RESULT papacc_issuer_image_init(PAPACC_U8 *b,const PAPACC_ISSUER_VIEW *v,const PAPACC_U8 base_hash[32],PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_VIEW check;if(!b||!v||!base_hash||!hash||v->high_water!=1)return PAPACC_ISSUER_INVALID;
 memset(b,0,PAPACC_ISSUER_IMAGE_BYTES);memcpy(b,"PACIS601",8);w32(b+8,1);w32(b+12,256);w64(b+24,v->revision);
 memcpy(b+32,v->root,32);memcpy(b+64,v->authority,16);memcpy(b+80,v->generation,16);memcpy(b+96,v->prefix,8);memcpy(b+104,v->policy,32);memcpy(b+136,v->ca_spki_digest,32);
 w64(b+168,1);w64(b+176,v->ca_not_after);w64(b+184,v->revision);memcpy(b+192,base_hash,32);
 if(!hash(ctx,b,FOOT,b+FOOT))return PAPACC_ISSUER_UNCERTAIN;return papacc_issuer_image_validate(b,&check,hash,ctx);
}
PAPACC_ISSUER_RESULT papacc_issuer_image_find(const PAPACC_U8 *b,const PAPACC_U8 id[16],PAPACC_ISSUER_RECORD *r,PAPACC_ISSUER_HASH hash,void *ctx){PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RESULT s;if(!id||!r)return PAPACC_ISSUER_INVALID;s=papacc_issuer_image_validate(b,&v,hash,ctx);if(s)return s;return latest(b,r32(b+16),id,r)?PAPACC_ISSUER_OK:PAPACC_ISSUER_ABSENT;}
PAPACC_ISSUER_RESULT papacc_issuer_image_append(PAPACC_U8 *b,const PAPACC_ISSUER_RECORD *r,const PAPACC_U8 *der,PAPACC_SIZE size,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RESULT s;PAPACC_SIZE p,len;PAPACC_U64 high;PAPACC_U32 n;
 if(!r||(!der&&size))return PAPACC_ISSUER_INVALID;s=papacc_issuer_image_validate(b,&v,hash,ctx);if(s)return s;
 p=r32(b+12);n=r32(b+16);if(size>FOOT||PAPACC_ISSUER_EVENT_FIXED+size>FOOT-p||v.revision==(PAPACC_U64)-1)return PAPACC_ISSUER_LIMIT;
 high=v.high_water;if(!transition(b,n,r,&high))return PAPACC_ISSUER_CONFLICT;
 if(r->state==PAPACC_ISSUER_ISSUED_UNPUBLISHED){PAPACC_U8 d[32];if(size!=r->certificate_size||!hash(ctx,der,size,d)||memcmp(d,r->certificate_digest,32))return PAPACC_ISSUER_INVALID;}else if(size)return PAPACC_ISSUER_INVALID;
 len=PAPACC_ISSUER_EVENT_FIXED+size;w64(b+p,n+1U);w32(b+p+8,(PAPACC_U32)len);w32(b+p+12,(PAPACC_U32)size);memcpy(b+p+16,b+224,32);record_encode(b+p+48,r);if(size)memcpy(b+p+324,der,size);
 if(!hash(ctx,b+p,len-32,b+p+len-32))return PAPACC_ISSUER_UNCERTAIN;
 memcpy(b+224,b+p+len-32,32);w32(b+12,(PAPACC_U32)(p+len));w32(b+16,n+1);w64(b+168,high);w64(b+184,v.revision+1);
 if(!hash(ctx,b,FOOT,b+FOOT))return PAPACC_ISSUER_UNCERTAIN;return PAPACC_ISSUER_OK;
}
PAPACC_ISSUER_RESULT papacc_issuer_image_certificate(const PAPACC_U8 *b,const PAPACC_U8 id[16],PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *size,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_RECORD r;PAPACC_SIZE p=256,n;PAPACC_ISSUER_RESULT s;if(!out||!size)return PAPACC_ISSUER_INVALID;s=papacc_issuer_image_find(b,id,&r,hash,ctx);if(s)return s;
 while(p<r32(b+12)){n=r32(b+p+12);if(n&&!memcmp(b+p+48,id,16)){*size=n;if(cap<n)return PAPACC_ISSUER_LIMIT;memcpy(out,b+p+324,n);return PAPACC_ISSUER_OK;}p+=r32(b+p+8);}return PAPACC_ISSUER_ABSENT;
}
/* Validate before reading offsets. No API can grant recovery authority here. */
PAPACC_ISSUER_RESULT papacc_issuer_checkpoint_assess(
 const PAPACC_U8 *image,PAPACC_SIZE size,const PAPACC_U8 *floor,PAPACC_SIZE floor_size,
 PAPACC_ISSUER_HASH hash,void *ctx,int stopped,PAPACC_ISSUER_CHECKPOINT_REPORT *out){
 PAPACC_ISSUER_VIEW candidate,prior;PAPACC_ISSUER_RESULT s;PAPACC_SIZE used;
 if(!out)return PAPACC_ISSUER_INVALID;
 memset(out,0,sizeof(*out));out->version=1;out->state=PAPACC_ISSUER_CHECKPOINT_INVALID;
 if(stopped){out->state=PAPACC_ISSUER_CHECKPOINT_STOPPED;return PAPACC_ISSUER_STOPPED;}
 if(!image||size!=PAPACC_ISSUER_IMAGE_BYTES||!hash||(!floor&&floor_size)||
  (floor&&floor_size!=PAPACC_ISSUER_IMAGE_BYTES))return PAPACC_ISSUER_INVALID;
 s=papacc_issuer_image_validate(image,&candidate,hash,ctx);
 if(s!=PAPACC_ISSUER_OK){if(s==PAPACC_ISSUER_UNCERTAIN)out->state=PAPACC_ISSUER_CHECKPOINT_INCOMPLETE;return s;}
 out->data=candidate;out->events=r32(image+16);
 if(!floor){out->state=PAPACC_ISSUER_CHECKPOINT_INCOMPLETE;return PAPACC_ISSUER_DENIED;}
 s=papacc_issuer_image_validate(floor,&prior,hash,ctx);
 if(s!=PAPACC_ISSUER_OK){out->state=PAPACC_ISSUER_CHECKPOINT_INCOMPLETE;return s;}
 /* Immutable image origin includes root/CA/generation/prefix/policy/key/base.
  * This data-only comparison deliberately refuses a changed identity/policy. */
 if(memcmp(image+24,floor+24,144)||memcmp(image+176,floor+176,8)||
  memcmp(image+192,floor+192,32)){
  out->state=PAPACC_ISSUER_CHECKPOINT_DIVERGENT;return PAPACC_ISSUER_CONFLICT;
 }
 if(candidate.revision<prior.revision||candidate.high_water<prior.high_water||
  out->events<r32(floor+16)){
  out->state=PAPACC_ISSUER_CHECKPOINT_ROLLBACK;return PAPACC_ISSUER_CONFLICT;
 }
 /* Compare the complete old event prefix, not just counter/revision/digest.
  * A newer fork may have equal/higher high-water while discarding revocation. */
 used=r32(floor+12);
 if(r32(image+12)<used||memcmp(image+256,floor+256,used-256)){
  out->state=PAPACC_ISSUER_CHECKPOINT_DIVERGENT;return PAPACC_ISSUER_CONFLICT;
 }
 out->state=PAPACC_ISSUER_CHECKPOINT_DATA_VALID_RECOVERY_DENIED;
 return PAPACC_ISSUER_DENIED;
}
