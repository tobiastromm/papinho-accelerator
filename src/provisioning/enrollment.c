#include "enrollment.h"
#include "../core/ed11_process_profile.h"
#include <string.h>
/* Existing reviewed OpenBSD comparator; no C2 token/owner/protocol reuse. */
int papacc_c2_compare(const void *,const void *,size_t);
static const char alphabet[]="0123456789ABCDEFGHJKMNPQRSTVWXYZ";
static unsigned get16(const PAPACC_U8 *p){return ((unsigned)p[0]<<8)|p[1];}
static PAPACC_U32 get32(const PAPACC_U8 *p){return ((PAPACC_U32)p[0]<<24)|((PAPACC_U32)p[1]<<16)|((PAPACC_U32)p[2]<<8)|p[3];}
static void put32(PAPACC_U8 *p,PAPACC_U32 n){p[0]=(PAPACC_U8)(n>>24);p[1]=(PAPACC_U8)(n>>16);p[2]=(PAPACC_U8)(n>>8);p[3]=(PAPACC_U8)n;}
static int span(const void *p,PAPACC_SIZE n){return p&&n&&n-1<=(PAPACC_SIZE)-1-(PAPACC_SIZE)p;}
static int cross(const void *p,PAPACC_SIZE n,const void *q,PAPACC_SIZE m){PAPACC_SIZE a=(PAPACC_SIZE)p,b=(PAPACC_SIZE)q;return n&&m&&(a<=b?b-a<n:a-b<m);}
static int chars(const PAPACC_U8 *p,PAPACC_SIZE n){PAPACC_SIZE i;for(i=0;i<n;i++)if(!p[i]||!strchr(alphabet,p[i]))return 0;return 1;}
static int utf8(const PAPACC_U8 *p,PAPACC_SIZE n){
 PAPACC_SIZE i=0;PAPACC_U32 c;unsigned more,j;
 while(i<n){c=p[i++];if(c<128U){if(c<32U||c==127U)return 0;continue;}
 if(c>=194U&&c<=223U){more=1;c&=31U;}else if(c>=224U&&c<=239U){more=2;c&=15U;}else if(c>=240U&&c<=244U){more=3;c&=7U;}else return 0;
 if(more>n-i)return 0;for(j=0;j<more;j++){if((p[i]&192U)!=128U)return 0;c=(c<<6)|(p[i++]&63U);}
 if((more==1&&c<128U)||(more==2&&c<2048U)||(more==3&&c<65536U)||c>1114111U||(c>=55296U&&c<=57343U)||c==65279U)return 0;
 }return 1;
}
static int payload(unsigned k,const PAPACC_U8 *p,PAPACC_SIZE n){
 unsigned a,b;PAPACC_SIZE i;
 switch(k){
 case 1:if(n<67U)return 0;a=get16(p+64);if(a<1U||a>253U||n!=66U+a)return 0;for(i=66;i<n;i++)if(p[i]<33U||p[i]>126U)return 0;return 1;
 case 2:return n==120U&&p[112]<=1U&&p[113]<=1U&&!p[114]&&!p[115]&&get32(p+116)>0U&&get32(p+116)<=300000U;
 case 3:if(n<62U||!chars(p+48,10))return 0;a=get16(p+58);b=get16(p+60);return a<=128U&&b<=128U&&n==62U+a+b&&utf8(p+62,a)&&utf8(p+62+a,b);
 case 4:return n==20U&&p[16]>=1U&&p[16]<=3U&&!p[17]&&!p[18]&&!p[19];
 case 5:return n>=53U&&get32(p+48)>=1U&&get32(p+48)<=4096U&&n==52U+get32(p+48);
 case 6:if(n<26U)return 0;return get32(p+16)>=1U&&get32(p+16)<=4096U&&get32(p+20)>=1U&&get32(p+20)<=4096U&&n==24U+get32(p+16)+get32(p+20);
 case 7:return n==48U;
 case 8:return n==20U&&get16(p+16)>=1U&&get16(p+16)<=7U&&!p[18]&&!p[19];
 default:return 0;
 }
}
static int direction(unsigned k,unsigned d){return d<=1U&&((d==EP_C2S&&(k==1U||k==3U||k==5U||k==7U))||(d==EP_S2C&&(k==2U||k==4U||k==6U||k==8U)));}
static EP_RESULT header(const PAPACC_U8 *p,unsigned d,PAPACC_SIZE *want){
 PAPACC_U32 n=get32(p+8),seq=get32(p+12);
 if(memcmp(p,"PAE1",4)||p[4]!=1U||p[5]||p[7]||!direction(p[6],d)||!seq||seq>8U||!n||n>EP_BODY_MAX)return EP_INVALID;
 *want=16U+n;return EP_DATA_ONLY;
}
EP_RESULT ep_frame_decode(const PAPACC_U8 *p,PAPACC_SIZE n,unsigned d,EP_FRAME *out){
 PAPACC_SIZE want;EP_FRAME f;if(!span(p,n)||!out||cross(p,n,out,sizeof(*out)))return EP_INVALID;
 if(n<16U)return EP_MORE;if(header(p,d,&want)!=EP_DATA_ONLY)return EP_INVALID;
 if(n<want)return EP_MORE;if(n!=want||!payload(p[6],p+16,n-16U))return EP_INVALID;
 f.kind=p[6];f.sequence=get32(p+12);f.payload.data=p+16;f.payload.size=n-16U;*out=f;return EP_DATA_ONLY;
}
EP_RESULT ep_frame_encode(unsigned k,unsigned seq,unsigned d,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used){
 if(!span(p,n)||!span(out,cap)||!used||n>EP_BODY_MAX||!seq||seq>8U||!direction(k,d)||!payload(k,p,n)||cross(p,n,out,cap)||cross(used,sizeof(*used),out,cap)||cross(used,sizeof(*used),p,n))return EP_INVALID;
 if(cap<16U+n)return EP_LIMIT;memcpy(out,"PAE1",4);out[4]=1;out[5]=0;out[6]=(PAPACC_U8)k;out[7]=0;put32(out+8,(PAPACC_U32)n);put32(out+12,seq);memcpy(out+16,p,n);*used=16U+n;return EP_DATA_ONLY;
}
EP_RESULT ep_reader_push(EP_READER *r,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U64 now,unsigned d,PAPACC_SIZE *used,EP_FRAME *out){
 PAPACC_SIZE take;EP_RESULT result;
 if(!r||!used||!out||!span(r->rx,r->capacity)||r->capacity>EP_FRAME_MAX||r->capacity<16U||r->have>r->capacity||r->want>r->capacity||(r->want&&r->have>r->want)||(n&&!span(p,n))||cross(r,sizeof(*r),r->rx,r->capacity)||cross(p,n,r->rx,r->capacity)||cross(p,n,r,sizeof(*r))||cross(out,sizeof(*out),r->rx,r->capacity)||cross(used,sizeof(*used),r->rx,r->capacity)||cross(out,sizeof(*out),r,sizeof(*r))||cross(used,sizeof(*used),r,sizeof(*r))||cross(out,sizeof(*out),used,sizeof(*used))||cross(p,n,out,sizeof(*out))||cross(p,n,used,sizeof(*used)))return EP_INVALID;
 *used=0;if(r->failed||r->ready)return EP_DENIED;if(r->stopped){r->failed=1;return EP_STOPPED;}
 if(r->started&&(now<r->last||now-r->first>=5000U)){r->failed=1;return EP_EXPIRED;}
 if(n&&!r->started){r->started=1;r->first=now;r->want=16;}r->last=now;
 while(n&&r->have<r->want){take=r->want-r->have;if(take>n)take=n;memcpy(r->rx+r->have,p,take);r->have+=take;*used+=take;p+=take;n-=take;
 if(r->have==16U&&r->want==16U){result=header(r->rx,d,&r->want);if(result!=EP_DATA_ONLY||r->want>r->capacity){r->failed=1;return EP_INVALID;}}
 }
 if(!r->started||r->have<r->want)return EP_MORE;result=ep_frame_decode(r->rx,r->have,d,out);if(result==EP_DATA_ONLY)r->ready=1;else r->failed=1;return result;
}
EP_RESULT ep_trace_observe(EP_TRACE *t,const EP_FRAME *f,unsigned d){
 unsigned k,s;PAPACC_SIZE n;const PAPACC_U8 *p;
 if(!t||!f||!span(f->payload.data,f->payload.size)||d>1U||cross(t,sizeof(*t),f,sizeof(*f))||cross(t,sizeof(*t),f->payload.data,f->payload.size))return EP_INVALID;
 if(t->stopped)return EP_STOPPED;if(t->terminal)return EP_DENIED;k=f->kind;p=f->payload.data;n=f->payload.size;
 if(!direction(k,d)||!payload(k,p,n)||f->sequence!=t->sequence[d]+1U||f->sequence>8U||t->bytes[d]>32768U||16U+n>32768U-t->bytes[d])goto bad;
 if(k==8U){PAPACC_U8 zero[16]={0};if(!t->phase||memcmp(p,t->phase>=2U?t->request:zero,16))goto bad;t->terminal=1;}
 else if(k==7U){if(t->phase<2U||memcmp(p,t->request,16)||memcmp(p+16,t->server_nonce,32))goto bad;t->terminal=1;}
 else if(t->phase==0U&&k==1U){memcpy(t->client_nonce,p,32);memcpy(t->server_fingerprint,p+32,32);t->phase=1;}
 else if(t->phase==1U&&k==2U){if(memcmp(p+16,t->client_nonce,32)||memcmp(p+80,t->server_fingerprint,32))goto bad;memcpy(t->request,p,16);memcpy(t->server_nonce,p+48,32);t->access=p[112];t->approval=p[113];t->phase=2;}
 else if(t->phase==2U&&k==3U){if(memcmp(p,t->request,16)||memcmp(p+16,t->server_nonce,32))goto bad;if(t->access?(!get16(p+58)||!get16(p+60)):(get16(p+58)||get16(p+60)))goto bad;t->phase=3;}
 else if(k==4U&&t->phase>=3U){if(memcmp(p,t->request,16))goto bad;s=p[16];if(s==1U&&t->phase==3U&&!t->wait_seen)t->wait_seen=1;else if(s==2U&&t->phase==3U)t->phase=4;else if(s==3U&&t->phase==5U&&!t->pending)t->pending=1;else goto bad;}
 else if(k==5U&&t->phase==4U){if(memcmp(p,t->request,16)||memcmp(p+16,t->server_nonce,32))goto bad;t->csr_size=get32(p+48);memcpy(t->csr,p+52,t->csr_size);t->phase=5;}
 else if(k==6U&&t->phase==5U&&!t->pending){if(memcmp(p,t->request,16))goto bad;t->terminal=1;}
 else goto bad;
 t->sequence[d]=f->sequence;t->bytes[d]+=16U+n;return EP_DATA_ONLY;
 bad:t->terminal=1;return EP_DENIED;
}
void ep_trace_stop(EP_TRACE *t){if(t){t->stopped=1;t->terminal=1;}}
EP_RESULT ep_code_format(const PAPACC_U8 r[7],PAPACC_U8 out[10]){
 unsigned i,j,v;if(!r||!out||cross(r,7,out,10))return EP_INVALID;
 for(i=0;i<10;i++){v=0;for(j=0;j<5;j++)v=(v<<1)|((r[(5U*i+j)/8U]>>(7U-(5U*i+j)%8U))&1U);out[i]=(PAPACC_U8)alphabet[v];}return EP_DATA_ONLY;
}
EP_RESULT ep_code_parse_local(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 out[10]){
 PAPACC_U8 code[10];unsigned i;if(!p||!out||cross(p,n,out,10)||(n!=10U&&n!=11U))return EP_INVALID;
 if(n==11U&&p[5]!='-')return EP_INVALID;for(i=0;i<10;i++)code[i]=p[i+(n==11U&&i>=5U?1U:0U)];if(!chars(code,10))return EP_INVALID;memcpy(out,code,10);return EP_DATA_ONLY;
}
EP_RESULT ep_pair_prepare_data(EP_PAIR *p,const PAPACC_U8 random[55],const PAPACC_U8 client[32],PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now,const EP_PAIR *live,PAPACC_SIZE count){
 PAPACC_SIZE i;
 if(!p||!random||!client||!connection||!generation||count>2U||(count&&!live)||p->initialized||p->stopped||cross(p,sizeof(*p),random,55)||cross(p,sizeof(*p),client,32)||cross(p,sizeof(*p),live,count*sizeof(*live)))return EP_INVALID;
 if(now>(PAPACC_U64)-1-300000U)return EP_LIMIT;
 if(!memcmp(random+23,client,32))return EP_DENIED;
 for(i=0;i<count;i++)if(live[i].initialized&&(!memcmp(random+7,live[i].request,16)||!memcmp(random+23,live[i].server_nonce,32)||!memcmp(random+23,live[i].client_nonce,32)||!memcmp(client,live[i].client_nonce,32)||!memcmp(client,live[i].server_nonce,32)))return EP_DENIED;
 memset(p,0,sizeof(*p));(void)ep_code_format(random,p->code);memcpy(p->request,random+7,16);memcpy(p->server_nonce,random+23,32);memcpy(p->client_nonce,client,32);p->connection=connection;p->generation=generation;p->start=now;p->last=now;p->deadline=now+300000U;p->initialized=1;return EP_DATA_ONLY;
}
EP_RESULT ep_pair_claim_data(EP_PAIR *p,const PAPACC_U8 *code,PAPACC_SIZE n,PAPACC_U64 connection,PAPACC_U64 generation,PAPACC_U64 now){
 if(!p||!p->initialized)return EP_INVALID;if(p->stopped)return EP_STOPPED;if(p->submitted||p->invalid)return EP_REPLAY;
 p->submitted=1;if(now<p->last||now>=p->deadline){p->invalid=1;return EP_EXPIRED;}p->last=now;
 if(connection!=p->connection||generation!=p->generation||!code||n!=10U||!chars(code,10)||papacc_c2_compare(code,p->code,10)){p->invalid=1;return EP_DENIED;}p->claimed=1;return EP_DATA_ONLY;
}
void ep_pair_stop(EP_PAIR *p){if(p){p->stopped=1;p->invalid=1;}}
EP_RESULT ep_origin_set(EP_ORIGIN *o,unsigned family,const PAPACC_U8 *a,PAPACC_U32 scope){
 static const PAPACC_U8 mapped[12]={0,0,0,0,0,0,0,0,0,0,255,255};EP_ORIGIN n;
 if(!o||!a||(family!=4U&&family!=6U)||cross(o,sizeof(*o),a,family==4U?4U:16U))return EP_INVALID;
 memset(&n,0,sizeof(n));if(family==6U&&!memcmp(a,mapped,12)){family=4;a+=12;scope=0;}n.family=family;n.scope=family==6U?scope:0;memcpy(n.address,a,family==4U?4U:16U);*o=n;return EP_DATA_ONLY;
}
static unsigned prune(PAPACC_U64 *p,unsigned n,PAPACC_U64 now){unsigned a,b=0;for(a=0;a<n;a++)if(now-p[a]<60000U)p[b++]=p[a];return b;}
static int origin_equal(const EP_ORIGIN *a,const EP_ORIGIN *b){return a->family==b->family&&a->scope==b->scope&&!memcmp(a->address,b->address,16);}
EP_RESULT ep_limiter_admit(EP_LIMITER *l,const EP_ORIGIN *o,PAPACC_U64 now,EP_TICKET *out){
 unsigned i,slot=16U,free_slot=16U,holder=2U;EP_ORIGIN_SLOT *s;
 if(!l||!o||!out||cross(l,sizeof(*l),o,sizeof(*o))||cross(l,sizeof(*l),out,sizeof(*out))||(o->family!=4U&&o->family!=6U))return EP_INVALID;
 if(l->stopped)return EP_STOPPED;if(l->started&&now<l->last){l->stopped=1;return EP_EXPIRED;}l->started=1;l->last=now;
 if(l->count>6U)return EP_INVALID;l->count=prune(l->global,l->count,now);if(l->count==6U||l->next==(PAPACC_U64)-1)return EP_LIMIT;
 for(i=0;i<2;i++)if(!l->held[i]&&holder==2U)holder=i;if(holder==2U)return EP_LIMIT;
 for(i=0;i<16;i++){s=&l->origins[i];if(s->count>3U)return EP_INVALID;s->count=prune(s->stamps,s->count,now);if(s->used&&origin_equal(&s->origin,o))slot=i;else if((!s->used||(!s->count&&!s->holds))&&free_slot==16U)free_slot=i;}
 if(slot==16U){slot=free_slot;if(slot==16U)return EP_LIMIT;s=&l->origins[slot];memset(s,0,sizeof(*s));s->origin=*o;s->used=1;}s=&l->origins[slot];if(s->count==3U)return EP_LIMIT;
 s->stamps[s->count++]=now;s->holds++;l->global[l->count++]=now;l->held[holder]=++l->next;l->held_slot[holder]=slot;out->slot=slot;out->generation=l->next;return EP_DATA_ONLY;
}
EP_RESULT ep_limiter_release_data(EP_LIMITER *l,EP_TICKET ticket){
 unsigned i;if(!l||ticket.slot>=16U||!ticket.generation)return EP_INVALID;
 for(i=0;i<2;i++)if(l->held[i]==ticket.generation&&l->held_slot[i]==ticket.slot){if(!l->origins[ticket.slot].holds)return EP_INVALID;l->origins[ticket.slot].holds--;l->held[i]=0;return EP_DATA_ONLY;}return EP_REPLAY;
}
void ep_limiter_stop(EP_LIMITER *l){if(l)l->stopped=1;}
EP_RESULT ep_enrollment_quote(PAPACC_SIZE other,PAPACC_SIZE local,PAPACC_SIZE provider,PAPACC_SIZE *out){
 PAPACC_SIZE total;if(!out||other==EP_UNKNOWN_BYTES||provider==EP_UNKNOWN_BYTES||local>262144U||provider>262144U-local)return EP_LIMIT;
 total=local+provider;if(other>PAPACC_ED11_PROCESS_LIMIT||total>PAPACC_ED11_PROCESS_LIMIT-other)return EP_LIMIT;*out=other+total;return EP_DATA_ONLY;
}
