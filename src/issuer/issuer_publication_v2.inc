/* Included after the shared v1 primitives. Explicit DATA version, no grants. */
static int image_v2(const PAPACC_U8 *b){return b&&!memcmp(b,"PACIS602",8)&&r32(b+8)==2;}
static PAPACC_SIZE publication_last(const PAPACC_U8 *b,unsigned count,const PAPACC_U8 id[16]){
 PAPACC_SIZE p=256,last=0;unsigned i;for(i=0;i<count;i++){if(!memcmp(b+p+48,id,16))last=p;p+=r32(b+p+8);}return last;
}
static void publication_decode(const PAPACC_U8 *e,PAPACC_ISSUER_PUBLICATION_FACTS *f){
 f->policy_epoch=r64(e+8);f->revision=r64(e+16);f->issuer_revision=r64(e+24);f->administrative_epoch=r64(e+32);f->intent_ordinal=r64(e+40);
 memcpy(f->intent_digest,e+48,32);memcpy(f->correlation,e+80,32);
}
static void publication_encode(PAPACC_U8 *e,unsigned kind,const PAPACC_ISSUER_PUBLICATION_FACTS *f){
 memset(e,0,112);w32(e,kind);if(!f)return;
 w64(e+8,f->policy_epoch);w64(e+16,f->revision);w64(e+24,f->issuer_revision);w64(e+32,f->administrative_epoch);w64(e+40,f->intent_ordinal);
 memcpy(e+48,f->intent_digest,32);memcpy(e+80,f->correlation,32);
}
static int publication_transition(const PAPACC_U8 *b,unsigned count,const PAPACC_ISSUER_RECORD *n,const PAPACC_U8 *e,PAPACC_U64 *high){
 unsigned kind=r32(e);PAPACC_SIZE p;PAPACC_ISSUER_RECORD old;PAPACC_U8 a[276],d[276];
 if(!kind)return zero(e,112)&&n->state!=PAPACC_ISSUER_PUBLISHED&&n->state!=PAPACC_ISSUER_PUBLICATION_INTENT&&transition(b,count,n,high);
 if(kind>2||!zero(e+4,4)||!r64(e+8)||zero(e+80,32))return 0;
 p=publication_last(b,count,n->request.id);if(!p)return 0;record_decode(b+p+48,&old);
 record_encode(a,&old);record_encode(d,n);
 /* Every immutable field, cert, serial and P survives both phases. */
 if(memcmp(a,d,260)||memcmp(a+268,d+268,4)||r64(e+8)!=n->approved_epoch)return 0;
 if(kind==1)return old.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED&&n->state==PAPACC_ISSUER_PUBLICATION_INTENT&&!n->published_epoch&&
  zero(e+16,24)&&r64(e+40)==count+1U&&zero(e+48,32);
 return old.state==PAPACC_ISSUER_PUBLICATION_INTENT&&n->state==PAPACC_ISSUER_PUBLISHED&&r32(b+p+324)==1&&
  r64(e+16)&&r64(e+16)==r64(e+24)&&r64(e+24)==r64(b+24)+r64(b+p)&&r64(e+32)==n->published_epoch&&n->published_epoch&&
  r64(e+40)==r64(b+p)&&!memcmp(e+48,b+p+r32(b+p+8)-32,32)&&!memcmp(e+80,b+p+404,32)&&r64(e+8)==r64(b+p+332);
}
static PAPACC_ISSUER_RESULT image_validate_v2(const PAPACC_U8 *b,PAPACC_ISSUER_VIEW *v,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_U8 d[32],chain[32]={0};PAPACC_SIZE p=256,len,der;unsigned i,n;PAPACC_U64 high=1,base;PAPACC_ISSUER_RECORD r;
 memset(v,0,sizeof(*v));
 if(!image_v2(b)||r32(b+12)<256||r32(b+12)>FOOT||!zero(b+20,4)||zero(b+32,32)||zero(b+64,16)||zero(b+80,16)||zero(b+96,8)||(b[96]&128U)||zero(b+104,32)||zero(b+136,32)||zero(b+192,32))return PAPACC_ISSUER_INVALID;
 if(!hash(ctx,b,FOOT,d))return PAPACC_ISSUER_UNCERTAIN;if(memcmp(d,b+FOOT,32))return PAPACC_ISSUER_INVALID;
 base=r64(b+24);n=r32(b+16);if(!base||n>(FOOT-256)/468U||base>(PAPACC_U64)-1-n||r64(b+184)!=base+n||!r64(b+176))return PAPACC_ISSUER_INVALID;
 for(i=0;i<n;i++){
  if(p>FOOT-468U)return PAPACC_ISSUER_INVALID;len=r32(b+p+8);der=r32(b+p+12);
  if(len<468U||len>FOOT-p||der!=len-468U||r64(b+p)!=i+1U||memcmp(b+p+16,chain,32))return PAPACC_ISSUER_INVALID;
  if(!hash(ctx,b+p,len-32,d))return PAPACC_ISSUER_UNCERTAIN;if(memcmp(d,b+p+len-32,32))return PAPACC_ISSUER_INVALID;
  record_decode(b+p+48,&r);if(!publication_transition(b,i,&r,b+p+324,&high))return PAPACC_ISSUER_INVALID;
  if(r.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED){if(der!=r.certificate_size||!hash(ctx,b+p+436,der,d)||memcmp(d,r.certificate_digest,32))return PAPACC_ISSUER_INVALID;}else if(der)return PAPACC_ISSUER_INVALID;
  memcpy(chain,b+p+len-32,32);p+=len;
 }
 if(p!=r32(b+12)||high!=r64(b+168)||memcmp(chain,b+224,32)||!zero(b+p,FOOT-p))return PAPACC_ISSUER_INVALID;
 memcpy(v->root,b+32,32);memcpy(v->authority,b+64,16);memcpy(v->generation,b+80,16);memcpy(v->prefix,b+96,8);memcpy(v->policy,b+104,32);memcpy(v->ca_spki_digest,b+136,32);
 v->revision=r64(b+184);v->high_water=high;v->ca_not_after=r64(b+176);return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT image_append_v2(PAPACC_U8 *b,const PAPACC_ISSUER_RECORD *r,const PAPACC_U8 *der,PAPACC_SIZE size,unsigned kind,const PAPACC_ISSUER_PUBLICATION_FACTS *f,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RESULT s;PAPACC_SIZE p,len;PAPACC_U64 high;PAPACC_U32 n;PAPACC_U8 ext[112],digest[32];
 if(!b||!r||!hash||(!der&&size)||!image_v2(b))return PAPACC_ISSUER_INVALID;s=image_validate_v2(b,&v,hash,ctx);if(s)return s;
 p=r32(b+12);n=r32(b+16);if(size>FOOT||468U+size>FOOT-p||v.revision==(PAPACC_U64)-1)return PAPACC_ISSUER_LIMIT;
 publication_encode(ext,kind,f);high=v.high_water;if(!publication_transition(b,n,r,ext,&high))return PAPACC_ISSUER_CONFLICT;
 if(r->state==PAPACC_ISSUER_ISSUED_UNPUBLISHED){if(size!=r->certificate_size||!hash(ctx,der,size,digest)||memcmp(digest,r->certificate_digest,32))return PAPACC_ISSUER_INVALID;}else if(size)return PAPACC_ISSUER_INVALID;
 len=468U+size;w64(b+p,n+1U);w32(b+p+8,(PAPACC_U32)len);w32(b+p+12,(PAPACC_U32)size);memcpy(b+p+16,b+224,32);record_encode(b+p+48,r);memcpy(b+p+324,ext,112);if(size)memcpy(b+p+436,der,size);
 if(!hash(ctx,b+p,len-32,b+p+len-32))return PAPACC_ISSUER_UNCERTAIN;
 memcpy(b+224,b+p+len-32,32);w32(b+12,(PAPACC_U32)(p+len));w32(b+16,n+1);w64(b+168,high);w64(b+184,v.revision+1);
 if(!hash(ctx,b,FOOT,b+FOOT))return PAPACC_ISSUER_UNCERTAIN;return PAPACC_ISSUER_OK;
}
PAPACC_ISSUER_RESULT papacc_issuer_image_intent(PAPACC_U8 *b,const PAPACC_ISSUER_RECORD *next,const PAPACC_U8 correlation[32],PAPACC_U64 P,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_RECORD r;PAPACC_ISSUER_PUBLICATION_FACTS f;PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RESULT s;
 if(!b||!next||!correlation||!hash||next->state!=PAPACC_ISSUER_PUBLISHED||next->published_epoch||!image_v2(b))return PAPACC_ISSUER_INVALID;
 s=image_validate_v2(b,&v,hash,ctx);if(s)return s;
 /* Reserve both finite events before the first durable effect. Never purge
  * a historical ack or leave a predictably unrepresentable continuation. */
 if(FOOT-r32(b+12)<2U*468U||v.revision>(PAPACC_U64)-1-2U)return PAPACC_ISSUER_LIMIT;
 memset(&f,0,sizeof(f));f.policy_epoch=P;f.intent_ordinal=r32(b+16)+1U;memcpy(f.correlation,correlation,32);r=*next;r.state=PAPACC_ISSUER_PUBLICATION_INTENT;
 return image_append_v2(b,&r,NULL,0,1,&f,hash,ctx);
}
PAPACC_ISSUER_RESULT papacc_issuer_image_ack(PAPACC_U8 *b,const PAPACC_ISSUER_RECORD *next,const PAPACC_ISSUER_PUBLICATION_FACTS *f,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_RECORD r;if(!next||!f||next->state!=PAPACC_ISSUER_PUBLISHED||next->published_epoch)return PAPACC_ISSUER_INVALID;r=*next;r.published_epoch=f->administrative_epoch;
 return image_append_v2(b,&r,NULL,0,2,f,hash,ctx);
}
PAPACC_ISSUER_RESULT papacc_issuer_image_publication(const PAPACC_U8 *b,const PAPACC_U8 id[16],PAPACC_ISSUER_PUBLICATION_FACTS *f,PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_VIEW v;PAPACC_SIZE p=256;PAPACC_ISSUER_RESULT s;if(!b||!id||!f||!hash)return PAPACC_ISSUER_INVALID;memset(f,0,sizeof(*f));if(!image_v2(b))return PAPACC_ISSUER_UNSUPPORTED;
 s=image_validate_v2(b,&v,hash,ctx);if(s)return s;
 while(p<r32(b+12)){if(!memcmp(b+p+48,id,16)&&r32(b+p+324)==2){publication_decode(b+p+324,f);return PAPACC_ISSUER_OK;}p+=r32(b+p+8);}return PAPACC_ISSUER_ABSENT;
}
PAPACC_ISSUER_RESULT papacc_issuer_image_init_v2(PAPACC_U8 *b,const PAPACC_ISSUER_VIEW *v,const PAPACC_U8 base[32],PAPACC_ISSUER_HASH hash,void *ctx){
 PAPACC_ISSUER_RESULT s=papacc_issuer_image_init(b,v,base,hash,ctx);PAPACC_ISSUER_VIEW check;if(s)return s;memcpy(b,"PACIS602",8);w32(b+8,2);
 if(!hash(ctx,b,FOOT,b+FOOT))return PAPACC_ISSUER_UNCERTAIN;return image_validate_v2(b,&check,hash,ctx);
}
