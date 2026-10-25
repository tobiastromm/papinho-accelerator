#include "roughtime.h"
#include <string.h>
#define RT_MAX (~(PAPACC_U64)0)
#define RT_NS ((PAPACC_U64)1000000000UL)
typedef struct RT_MAP { PAPACC_RT_BYTES b; PAPACC_SIZE base; unsigned count; } RT_MAP;
static PAPACC_U32 u32(const PAPACC_U8 *p){return (PAPACC_U32)p[0]|((PAPACC_U32)p[1]<<8)|((PAPACC_U32)p[2]<<16)|((PAPACC_U32)p[3]<<24);}
static PAPACC_U64 u64(const PAPACC_U8 *p){return (PAPACC_U64)u32(p)|((PAPACC_U64)u32(p+4)<<32);}
static void w32(PAPACC_U8 *p,PAPACC_U32 x){unsigned i;for(i=0;i<4;i++){p[i]=(PAPACC_U8)x;x>>=8;}}
static int add(PAPACC_U64 a,PAPACC_U64 b,PAPACC_U64 *out){if(a>RT_MAX-b)return 0;*out=a+b;return 1;}
static int disjoint(const void *a,PAPACC_SIZE n,const void *b,PAPACC_SIZE m){
 const PAPACC_U8 *x=(const PAPACC_U8 *)a,*y=(const PAPACC_U8 *)b;PAPACC_SIZE i;
 if(!n||!m)return 1;
 for(i=0;i<n;i++)if(x+i==y)return 0;
 for(i=0;i<m;i++)if(y+i==x)return 0;
 return 1;
}
static int map(PAPACC_RT_BYTES b,RT_MAP *m){
 PAPACC_U32 count,prev=0,off=0,v;unsigned i;
 if(!b.p||b.n<8)return 0;
 count=u32(b.p);if(!count||count>32||b.n<(PAPACC_SIZE)count*8U)return 0;
 m->b=b;m->count=(unsigned)count;m->base=(PAPACC_SIZE)count*8U;
 for(i=0;i<m->count;i++){
  v=u32(b.p+4U*m->count+4U*i);if(i&&v<=prev)return 0;prev=v;
  if(i){v=u32(b.p+4U*i);if(v%4U||v<off||v>b.n-m->base)return 0;off=v;}
 }
 return 1;
}
static int field(const RT_MAP *m,const char *tag,PAPACC_SIZE exact,PAPACC_RT_BYTES *out){
 PAPACC_U32 t=u32((const PAPACC_U8 *)tag);PAPACC_SIZE a,b;unsigned i;
 for(i=0;i<m->count;i++)if(u32(m->b.p+4U*m->count+4U*i)==t){
  a=i?u32(m->b.p+4U*i):0;b=i+1U<m->count?u32(m->b.p+4U*(i+1U)):m->b.n-m->base;
  out->p=m->b.p+m->base+a;out->n=b-a;return exact==(PAPACC_SIZE)-1||out->n==exact;
 }
 return 0;
}
static int packet(PAPACC_RT_BYTES b,RT_MAP *m){
 PAPACC_RT_BYTES body;
 if(!b.p||b.n<20||b.n>PAPACC_RT_PACKET||memcmp(b.p,"ROUGHTIM",8)||u32(b.p+8)!=b.n-12)return 0;
 body.p=b.p+12;body.n=b.n-12;return map(body,m);
}
PAPACC_RT_RESULT papacc_rt_packet_data(PAPACC_RT_BYTES b){RT_MAP m;return packet(b,&m)?PAPACC_RT_DATA_OK:PAPACC_RT_INVALID;}
static int hash(const PAPACC_RT_CRYPTO *c,const PAPACC_U8 *a,PAPACC_SIZE an,const PAPACC_U8 *b,PAPACC_SIZE bn,const PAPACC_U8 *d,PAPACC_SIZE dn,PAPACC_U8 *out){
 PAPACC_RT_BYTES v[3];v[0].p=a;v[0].n=an;v[1].p=b;v[1].n=bn;v[2].p=d;v[2].n=dn;return c->hash(c->user,v,3,out);
}
PAPACC_RT_RESULT papacc_rt_merkle_data(const PAPACC_RT_CRYPTO *c,PAPACC_RT_BYTES request,PAPACC_RT_BYTES path,PAPACC_U32 index,const PAPACC_U8 *root){
 PAPACC_U8 digest[32],next[32],prefix;unsigned i;
 if(!c||!root||!request.p||request.n>2048||!request.n||path.n>1024||path.n%32||(path.n&&!path.p))return PAPACC_RT_INVALID;
 if(!c->hash)return PAPACC_RT_CRYPTO_UNAVAILABLE;
 prefix=0;if(!hash(c,&prefix,1,request.p,request.n,NULL,0,digest))return PAPACC_RT_CRYPTO_UNAVAILABLE;
 prefix=1;
 for(i=0;i<path.n/32;i++){
  const PAPACC_U8 *node=path.p+32*i;
  if(!hash(c,&prefix,1,index&1U?node:digest,32,index&1U?digest:node,32,next))return PAPACC_RT_CRYPTO_UNAVAILABLE;
  memcpy(digest,next,32);index>>=1;
 }
 return index||memcmp(digest,root,32)?PAPACC_RT_INVALID:PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_request_data(const PAPACC_RT_CRYPTO *c,const PAPACC_U8 *key,const PAPACC_U8 *nonce,PAPACC_U8 *out){
 PAPACC_U8 srv[32],prefix=255;unsigned i;const char *tags[5]={"VER\0","SRV\0","NONC","TYPE","ZZZZ"};PAPACC_U32 offsets[4]={4,36,68,72};
 if(!c||!key||!nonce||!out||!disjoint(out,2048,key,32)||!disjoint(out,2048,nonce,32))return PAPACC_RT_INVALID;
 if(!c->hash)return PAPACC_RT_CRYPTO_UNAVAILABLE;
 if(!hash(c,&prefix,1,key,32,NULL,0,srv))return PAPACC_RT_CRYPTO_UNAVAILABLE;
 memset(out,0,2048);memcpy(out,"ROUGHTIM",8);w32(out+8,2036);w32(out+12,5);
 for(i=0;i<4;i++)w32(out+16+4*i,offsets[i]);
 for(i=0;i<5;i++)memcpy(out+32+4*i,tags[i],4);
 w32(out+52,1);memcpy(out+56,srv,32);memcpy(out+88,nonce,32);return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_reply_data(const PAPACC_RT_CRYPTO *c,PAPACC_RT_BYTES request,PAPACC_RT_BYTES response,const PAPACC_U8 *key,PAPACC_RT_REPLY *out){
 RT_MAP req,top,srep,cert,dele;PAPACC_RT_BYTES pad,n,t,v,sig,path,sv,cv,dv,cs,pub,lo,hi,mid,rad,vers,root,idx,qn,qs;
 PAPACC_U8 digest[32],prefix;PAPACC_U32 last=0,x;unsigned i;PAPACC_RT_REPLY result;
 static const PAPACC_U8 dc[]="Roughtime v1 delegation signature",rc[]="Roughtime v1 response signature";
 if(!c||!key||!out||!request.p||request.n!=2048||!response.p||response.n>2048||!disjoint(out,sizeof(*out),request.p,request.n)||!disjoint(out,sizeof(*out),response.p,response.n)||!disjoint(out,sizeof(*out),key,32))return PAPACC_RT_INVALID;
 if(!c->hash||!c->verify)return PAPACC_RT_CRYPTO_UNAVAILABLE;
 if(request.n!=2048||!packet(request,&req)||!packet(response,&top))return PAPACC_RT_INVALID;
 if(!field(&req,"VER\0",4,&v)||u32(v.p)!=1||!field(&req,"TYPE",4,&t)||u32(t.p)||!field(&req,"NONC",32,&qn)||!field(&req,"SRV\0",32,&qs))return PAPACC_RT_INVALID;
 if(req.count!=5||!field(&req,"ZZZZ",1924,&pad))return PAPACC_RT_INVALID;
 for(i=0;i<pad.n;i++)if(pad.p[i])return PAPACC_RT_INVALID;
 prefix=255;if(!hash(c,&prefix,1,key,32,NULL,0,digest))return PAPACC_RT_CRYPTO_UNAVAILABLE;
 if(memcmp(qs.p,digest,32))return PAPACC_RT_INVALID;
 if(!field(&top,"NONC",32,&n)||memcmp(n.p,qn.p,32)||!field(&top,"TYPE",4,&t)||u32(t.p)!=1||
 !field(&top,"SIG\0",64,&sig)||!field(&top,"PATH",(PAPACC_SIZE)-1,&path)||path.n%32||path.n>1024||
 !field(&top,"SREP",(PAPACC_SIZE)-1,&sv)||!field(&top,"CERT",(PAPACC_SIZE)-1,&cv)||!field(&top,"INDX",4,&idx)||
 !map(sv,&srep)||!map(cv,&cert)||!field(&cert,"SIG\0",64,&cs)||!field(&cert,"DELE",(PAPACC_SIZE)-1,&dv)||!map(dv,&dele))return PAPACC_RT_INVALID;
 if(!field(&srep,"VER\0",4,&v)||u32(v.p)!=1||!field(&srep,"RADI",4,&rad)||!u32(rad.p)||!field(&srep,"MIDP",8,&mid)||
 !field(&srep,"VERS",(PAPACC_SIZE)-1,&vers)||!vers.n||vers.n%4||vers.n>128||!field(&srep,"ROOT",32,&root)||
 !field(&dele,"PUBK",32,&pub)||!field(&dele,"MINT",8,&lo)||!field(&dele,"MAXT",8,&hi))return PAPACC_RT_INVALID;
 result.midpoint=u64(mid.p);result.radius=u32(rad.p);
 if(u64(lo.p)>result.midpoint||result.midpoint>u64(hi.p))return PAPACC_RT_INVALID;
 x=0;for(i=0;i<vers.n/4;i++){PAPACC_U32 z=u32(vers.p+4*i);if(i&&z<=last)return PAPACC_RT_INVALID;last=z;if(z==1)x=1;}
 if(!x)return PAPACC_RT_INVALID;
 if(!c->verify(c->user,key,dc,sizeof(dc),dv.p,dv.n,cs.p)||!c->verify(c->user,pub.p,rc,sizeof(rc),sv.p,sv.n,sig.p))return PAPACC_RT_INVALID;
 if(papacc_rt_merkle_data(c,request,path,u32(idx.p),root.p)!=PAPACC_RT_DATA_OK)return PAPACC_RT_INVALID;
 *out=result;return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_campaign_data(const PAPACC_RT_CRYPTO *c,const PAPACC_RT_BYTES *req,const PAPACC_RT_BYTES *resp,const PAPACC_U8 *keys,const PAPACC_U8 *randoms,PAPACC_RT_REPLY *out){
 PAPACC_RT_REPLY values[6];PAPACC_U8 nonce[32];unsigned i,j;PAPACC_RT_RESULT result;PAPACC_U64 lower,upper;
 if(!c||!req||!resp||!keys||!randoms||!out)return PAPACC_RT_INVALID;
 for(i=0;i<6;i++)if(!req[i].p||req[i].n!=2048||!resp[i].p||resp[i].n>2048)return PAPACC_RT_INVALID;
 for(i=0;i<6;i++)if(!disjoint(out,sizeof(values),req[i].p,req[i].n)||!disjoint(out,sizeof(values),resp[i].p,resp[i].n))return PAPACC_RT_INVALID;
 if(!disjoint(out,sizeof(values),req,6U*sizeof(*req))||!disjoint(out,sizeof(values),resp,6U*sizeof(*resp))||!disjoint(out,sizeof(values),keys,96)||!disjoint(out,sizeof(values),randoms,192))return PAPACC_RT_INVALID;
 if(!c->hash)return PAPACC_RT_CRYPTO_UNAVAILABLE;
 for(i=0;i<6;i++){
  for(j=0;j<i;j++)if(!memcmp(randoms+32*i,randoms+32*j,32))return PAPACC_RT_INVALID;
  if(!i)memcpy(nonce,randoms,32);
  else if(!hash(c,resp[i-1].p,resp[i-1].n,randoms+32*i,32,NULL,0,nonce))return PAPACC_RT_CRYPTO_UNAVAILABLE;
  if(!req[i].p||req[i].n!=2048||memcmp(req[i].p+88,nonce,32))return PAPACC_RT_INVALID;
  result=papacc_rt_reply_data(c,req[i],resp[i],keys+32*(i%3),&values[i]);if(result!=PAPACC_RT_DATA_OK)return result;
  if(values[i].midpoint<values[i].radius||!add(values[i].midpoint,values[i].radius,&upper))return PAPACC_RT_LIMIT;
  for(j=0;j<i;j++){lower=values[j].midpoint-values[j].radius;if(lower>upper)return PAPACC_RT_INVALID;}
 }
 memcpy(out,values,sizeof(values));return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_interval_data(const PAPACC_RT_REPLY *r,const PAPACC_UTC_OBSERVATION *quality,PAPACC_U64 m,PAPACC_U64 *lower,PAPACC_U64 *upper){
 PAPACC_UTC_OBSERVATION o;PAPACC_U64 ls,us,l,u;PAPACC_UTC_DATA_RESULT result;
 if(!r||!quality||quality->m0>quality->m1||quality->m1>m||!lower||!upper||!disjoint(lower,sizeof(*lower),upper,sizeof(*upper))||!disjoint(lower,sizeof(*lower),quality,sizeof(*quality))||!disjoint(upper,sizeof(*upper),quality,sizeof(*quality))||!disjoint(lower,sizeof(*lower),r,sizeof(*r))||!disjoint(upper,sizeof(*upper),r,sizeof(*r))||r->midpoint<r->radius||!r->radius||!add(r->midpoint,r->radius,&us))return PAPACC_RT_INVALID;
 ls=r->midpoint-r->radius;if(ls>RT_MAX/RT_NS||us>RT_MAX/RT_NS)return PAPACC_RT_LIMIT;
 o=*quality;o.lower=0;o.upper=0;o.m1=o.m0;
 result=papacc_utc_data_propagate(&o,&o.context,m,&l,&u);
 if(result!=PAPACC_UTC_DATA_OK)return result==PAPACC_UTC_DATA_LIMIT?PAPACC_RT_LIMIT:PAPACC_RT_NOT_QUALIFIED;
 /* Elapsed lower is deliberately not used: g may be anywhere in bracket. */
 ls*=RT_NS;us*=RT_NS;if(!add(us,u,&us))return PAPACC_RT_LIMIT;
 if(us-ls>30U*RT_NS)return PAPACC_RT_LIMIT;
 *lower=ls;*upper=us;return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_hull_data(const PAPACC_RT_REPLY *replies,const PAPACC_UTC_OBSERVATION *quality,PAPACC_U64 m,PAPACC_U64 *lower,PAPACC_U64 *upper){
 PAPACC_U64 l,u,lo=0,hi=0;PAPACC_RT_RESULT result;unsigned i;
 if(!replies||!quality||!lower||!upper||!disjoint(lower,sizeof(*lower),upper,sizeof(*upper))||
 !disjoint(lower,sizeof(*lower),replies,6U*sizeof(*replies))||!disjoint(upper,sizeof(*upper),replies,6U*sizeof(*replies))||
 !disjoint(lower,sizeof(*lower),quality,6U*sizeof(*quality))||!disjoint(upper,sizeof(*upper),quality,6U*sizeof(*quality)))return PAPACC_RT_INVALID;
 for(i=0;i<6;i++){
  /* Shared Q3 checks context correlation; still DATA, never provenance. */
  if(papacc_utc_data_propagate(&quality[i],&quality[0].context,m,&l,&u)!=PAPACC_UTC_DATA_OK)return PAPACC_RT_NOT_QUALIFIED;
  if(quality[i].rho_num!=quality[0].rho_num||quality[i].rho_den!=quality[0].rho_den||quality[i].pal_error!=quality[0].pal_error||quality[i].deadline!=quality[0].deadline)return PAPACC_RT_NOT_QUALIFIED;
  result=papacc_rt_interval_data(&replies[i],&quality[i],m,&l,&u);if(result!=PAPACC_RT_DATA_OK)return result;
  if(!i||l<lo)lo=l;if(!i||u>hi)hi=u;
 }
 if(hi-lo>30U*RT_NS)return PAPACC_RT_LIMIT;
 *lower=lo;*upper=hi;return PAPACC_RT_DATA_OK;
}
static PAPACC_RT_RESULT check(PAPACC_RT_EXCHANGE *e,PAPACC_U64 now){
 if(!e)return PAPACC_RT_INVALID;
 if(e->stopped)return PAPACC_RT_RETAINED;
 if(e->invalid||now<e->last){e->invalid=1;return PAPACC_RT_INVALID;}
 e->last=now;
 if(now<e->campaign||now-e->campaign>=60U*RT_NS){e->invalid=1;return PAPACC_RT_EXPIRED;}
 return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_exchange_start_data(PAPACC_RT_EXCHANGE *e,PAPACC_U64 campaign,PAPACC_U64 now){
 if(!e||e->phase||e->stopped||e->invalid||now<campaign||now-campaign>=60U*RT_NS||campaign>RT_MAX-60U*RT_NS)return PAPACC_RT_INVALID;
 e->campaign=campaign;e->start=now;e->last=now;e->phase=1;return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_exchange_sent_data(PAPACC_RT_EXCHANGE *e,PAPACC_U64 now){
 PAPACC_RT_RESULT r=check(e,now);if(r!=PAPACC_RT_DATA_OK)return r;
 if(e->phase!=1){e->invalid=1;return PAPACC_RT_INVALID;}
 if(now-e->start>=RT_NS){e->invalid=1;return PAPACC_RT_EXPIRED;}
 e->sent=now;e->phase=2;return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_exchange_received_data(PAPACC_RT_EXCHANGE *e,PAPACC_U64 now){
 PAPACC_RT_RESULT r=check(e,now);if(r!=PAPACC_RT_DATA_OK)return r;
 if(e->phase!=2){e->invalid=1;return PAPACC_RT_INVALID;}
 if(now-e->start>=4U*RT_NS||now-e->sent>=3U*RT_NS){e->invalid=1;return PAPACC_RT_EXPIRED;}
 e->phase=3;return PAPACC_RT_DATA_OK;
}
PAPACC_RT_RESULT papacc_rt_exchange_verified_data(PAPACC_RT_EXCHANGE *e,PAPACC_U64 now){
 PAPACC_RT_RESULT r=check(e,now);if(r!=PAPACC_RT_DATA_OK)return r;
 if(e->phase!=3){e->invalid=1;return PAPACC_RT_INVALID;}
 if(now-e->start>=6U*RT_NS){e->invalid=1;return PAPACC_RT_EXPIRED;}
 e->phase=4;return PAPACC_RT_DATA_OK;
}
void papacc_rt_exchange_stop_data(PAPACC_RT_EXCHANGE *e){if(e){e->stopped=1;e->invalid=1;}}
PAPACC_U32 papacc_rt_backoff_data(unsigned failures){PAPACC_U32 n=1;while(failures>1U&&n<86400U){n=n>43200U?86400U:n*2U;--failures;}return n;}
