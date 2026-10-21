#include "config_root8_v2.h"
#include "config_root8_internal.h"
#include "../issuer/issuer_codec.h"
#include <stdint.h>
#include <string.h>
static int range(const void *p,PAPACC_SIZE n){return p&&n&&n-1U<=UINTPTR_MAX-(uintptr_t)p;}
static int cross(const void *a,PAPACC_SIZE n,const void *b,PAPACC_SIZE m)
{uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;return x<=y?y-x<n:x-y<m;}
static unsigned u16(const PAPACC_U8 *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static PAPACC_U32 u32(const PAPACC_U8 *p){return (PAPACC_U32)p[0]|((PAPACC_U32)p[1]<<8)|((PAPACC_U32)p[2]<<16)|((PAPACC_U32)p[3]<<24);}
static PAPACC_U64 u64(const PAPACC_U8 *p){PAPACC_U64 v=0;unsigned i;for(i=0;i<8U;i++)v|=(PAPACC_U64)p[i]<<(8U*i);return v;}
static int zero(const PAPACC_U8 *p,PAPACC_SIZE n){PAPACC_SIZE i;for(i=0;i<n;i++)if(p[i])return 0;return 1;}
static int digest(const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *expected,PC_LST1_HASH hash,void *ctx)
{PAPACC_U8 d[32];if(!hash||!hash(ctx,p,n,d))return -1;return !memcmp(d,expected,32);}
static int dns(const PAPACC_U8 *p,unsigned n)
{
 unsigned i,label=0,c;int nonnumeric=0;if(!n||n>253U||!zero(p+n,256U-n))return 0;
 for(i=0;i<n;i++){c=p[i];if(c=='.'){if(!label||p[i-1]=='-')return 0;label=0;}
 else{if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='-')||label==63U||(!label&&c=='-'))return 0;label++;if(c<'0'||c>'9')nonnumeric=1;}}
 return label&&p[n-1]!='-'&&nonnumeric;
}
/* Header checked before any computed offset. n is prefix size, NOT a file. */
static PC_ROOT8_RESULT header(const PAPACC_U8 *p,PAPACC_SIZE n)
{
 unsigned N,K,B;PAPACC_SIZE O,S,L,Q;PAPACC_U64 R,pre;
 if(!range(p,n)||n<PC_ROOT8_V2_MIN-32U||n>PC_ROOT8_V2_FIELDS_MAX||memcmp(p,"PACROOT8",8)||u16(p+8)!=2U||
 u16(p+10)!=512U||u32(p+12)!=n+32U||u32(p+16)!=3U||u16(p+22)!=512U||u16(p+24)!=1024U||
 u16(p+26)!=1U||u16(p+28)!=512U||u16(p+30)!=1U||u32(p+128)!=31U||u16(p+132)!=5U||u16(p+134)!=1U||
 u32(p+168)!=15U||!zero(p+172,4)||!zero(p+258,254)||u16(p+242)!=1U||u16(p+254)!=768U||u16(p+256)!=1U||
 (u16(p+240)!=1U&&u16(p+240)!=2U)||zero(p+72,32))return PC_ROOT8_INVALID;
 N=u16(p+20);K=u16(p+252);L=u32(p+44);
 if(!N||N>4U||!K||K>N||L<672U||L>2208U||(L-160U)%512U)return PC_ROOT8_INVALID;
 B=(unsigned)((L-160U)/512U);if(!B||B>4U)return PC_ROOT8_INVALID;
 O=512U+1536U*N;S=96U+768U*K;Q=O+S+L+16384U;
 if(Q+512U!=n||u32(p+32)!=512U||u32(p+36)!=1536U*N||u32(p+40)!=O+S||
 u32(p+48)!=O+S+L||u32(p+52)!=16384U||u32(p+56)!=Q||u32(p+60)!=512U||u32(p+64)!=Q+512U||
 u32(p+68)!=32U||u32(p+244)!=O||u32(p+248)!=S)return PC_ROOT8_INVALID;
 R=u64(p+104);pre=u64(p+112);
 if(!R||!pre||pre==(PAPACC_U64)-1||R!=pre+1U||u64(p+136)!=pre||!u64(p+120)||!u64(p+144)||!u64(p+152)||
 !u64(p+160)||u64(p+120)<u64(p+160))return PC_ROOT8_INVALID;
 return PC_ROOT8_OK;
}
static PC_ROOT8_RESULT descriptors(const PAPACC_U8 *p)
{
 PAPACC_U8 record[512];const PAPACC_U8 *q,*old;unsigned i,j;PC_LST1_RESULT s;
 for(i=0;i<u16(p+20);i++){q=p+512U+1536U*i;
 if(memcmp(q+344,"PACCD001",8)||!zero(q+352,32))return PC_ROOT8_INVALID;
 memcpy(record,q,512);memset(record+344,0,40);s=pc_lst1_record_shape(record,512);
 if(s==PC_LST1_UNSUPPORTED)return PC_ROOT8_UNSUPPORTED;if(s!=PC_LST1_OK)return PC_ROOT8_INVALID;
 for(j=0;j<i;j++){old=p+512U+1536U*j;if(u16(q+10)==u16(old+10)&&!memcmp(q+24,old+24,u16(q+10)))return PC_ROOT8_INVALID;}}
 return PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_snt_data(const PAPACC_U8 *p,int integrity,PC_LST1_HASH hash,void *ctx)
{
 const PAPACC_U8 *s=p+u32(p+244),*r,*q,*old;unsigned i,j,ordinal,stage,seen=0,secure=0;int h;
 PAPACC_SIZE S=u32(p+248);unsigned K=u16(p+252);PC_ROOT8_RESULT result=descriptors(p);if(result)return result;
 if(memcmp(s,"PACSNT01",8)||u16(s+8)!=1U||u16(s+10)!=64U||u32(s+12)!=S||u16(s+16)!=K||
 u16(s+18)!=768U||u32(s+20)||memcmp(s+24,p+72,32)||u64(s+56)!=u64(p+104))return PC_ROOT8_INVALID;
 for(i=0;i<K;i++){r=s+64U+768U*i;ordinal=u16(r+16);stage=r[22];
 if(memcmp(r,"PACSNI01",8)||u16(r+8)!=1U||u16(r+10)!=768U||ordinal>=u16(p+20)||
 (i&&ordinal<=u16(r-768U+16U))||r[18]!=1U||r[19]!=1U||u16(r+20)!=1U||r[23]||u16(r+30)||
 !u64(r+32)||u64(r+32)>u64(p+104)||zero(r+40,16)||zero(r+56,16)||zero(r+568,32)||!zero(r+600,168)||
 !dns(r+312,u16(r+28)))return PC_ROOT8_INVALID;
 if(stage<1U||stage>3U||u32(r+12)!=(stage==1U?15U:stage==2U?31U:63U))return PC_ROOT8_INVALID;
 if(stage==1U){if(!zero(r+72,112))return PC_ROOT8_INVALID;}
 else if(zero(r+72,16)||zero(r+88,32)||zero(r+120,32)||(stage==2U?!zero(r+152,32):zero(r+152,32)))return PC_ROOT8_INVALID;
 q=p+512U+1536U*ordinal;
 if(q[4]!=1U||u16(r+24)!=u16(q+10)||u16(r+26)!=u16(q+12)||memcmp(r+184,q+24,64)||memcmp(r+248,q+88,64))return PC_ROOT8_INVALID;
 seen|=1U<<ordinal;
 for(j=0;j<i;j++){old=s+64U+768U*j;
 if(u16(r+26)==u16(old+26)&&!memcmp(r+248,old+248,u16(r+26))&&
 (memcmp(r+18,old+18,5)||u16(r+28)!=u16(old+28)||memcmp(r+312,old+312,256)||memcmp(r+40,old+40,144)||memcmp(r+568,old+568,32)))return PC_ROOT8_INVALID;}
 }
 for(i=0;i<u16(p+20);i++)if(p[512U+1536U*i+4U]==1U)secure|=1U<<i;
 if(secure!=seen)return PC_ROOT8_INVALID;
 if(integrity){h=digest(s,S-32U,s+S-32U,hash,ctx);if(h<0)return PC_ROOT8_HASH_FAILED;if(!h)return PC_ROOT8_INVALID;}
 return PC_ROOT8_OK;
}
static PC_ROOT8_RESULT checkpoint(const PAPACC_U8 *p,int integrity,PC_LST1_HASH hash,void *ctx)
{
 PAPACC_SIZE Q=u32(p+56);const PAPACC_U8 *c=p+Q;int h;
 if(zero(c,512))return PC_ROOT8_OK;
 if(memcmp(c,"PACR8CP2",8)||u16(c+8)!=2U||u16(c+10)!=512U||u32(c+12)!=1U||memcmp(c+16,p+72,32)||
 u64(c+48)!=u64(p+104)||u64(c+56)!=u64(p+112)||!u64(c+64)||!u64(c+72)||u64(c+80)!=u64(p+120)||
 memcmp(c+120,p+176,64)||!zero(c+184,296))return PC_ROOT8_INVALID;
 h=digest(p,Q,c+88,hash,ctx);if(h<0)return PC_ROOT8_HASH_FAILED;if(!h)return PC_ROOT8_INVALID;
 if(integrity){h=digest(c,480,c+480,hash,ctx);if(h<0)return PC_ROOT8_HASH_FAILED;if(!h)return PC_ROOT8_INVALID;}
 return PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_components_data(const PAPACC_U8 *p,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;PC_LST1_MODEL lst;PC_LST1_RESULT lr;int complete,h;unsigned i;const PAPACC_U8 *q,*table;
 lr=pc_lst1_decode(p+u32(p+40),u32(p+44),&lst,&complete,hash,hc);
 if(lr==PC_LST1_HASH_FAILED)return PC_ROOT8_HASH_FAILED;if(lr==PC_LST1_UNSUPPORTED)return PC_ROOT8_UNSUPPORTED;
 if(lr!=PC_LST1_OK||!complete||memcmp(lst.fields+24,p+72,32)||u64(lst.fields+56)!=u64(p+136)||u64(lst.fields+64)!=u64(p+144)||
 u64(lst.fields+72)!=u64(p+152)||u64(lst.fields+80)!=u64(p+160)||u32(lst.fields+20)!=31U)return PC_ROOT8_INVALID;
 if(!component)return PC_ROOT8_INCOMPLETE;
 for(i=0;i<u16(p+20);i++){q=p+512U+1536U*i;h=digest(q+512,1024,q+312,hash,hc);
 if(h<0)return PC_ROOT8_HASH_FAILED;if(!h)return PC_ROOT8_INVALID;s=component(cc,5U,q+512,1024,q,p);if(s)return s;}
 s=component(cc,6U,p+u32(p+48),16384,NULL,p);if(s)return s;
 table=p+u32(p+244);for(i=0;i<u16(p+252);i++){q=table+64U+768U*i;s=component(cc,7U,p+u32(p+48),16384,q,p);if(s)return s;}
 return PC_ROOT8_OK;
}
static PC_ROOT8_RESULT shape(const PAPACC_U8 *p,PAPACC_SIZE n,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;s=header(p,n);if(s)return s;s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;
 s=checkpoint(p,1,hash,hc);if(s)return s;return pc_root8_components_data(p,hash,hc,component,cc);
}
PC_ROOT8_RESULT pc_root8_v2_validate(const PAPACC_U8 *p,PAPACC_SIZE n,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 int h;if(!range(p,n)||n<PC_ROOT8_V2_MIN||n>PC_ROOT8_V2_MAX)return PC_ROOT8_INVALID;
 h=digest(p,n-32U,p+n-32U,hash,hc);if(h<0)return PC_ROOT8_HASH_FAILED;if(!h)return PC_ROOT8_INVALID;
 return shape(p,n-32U,hash,hc,component,cc);
}
static int outputs(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used)
{return range(p,n)&&range(out,cap)&&range(used,sizeof(*used))&&!cross(p,n,out,cap)&&!cross(p,n,used,sizeof(*used))&&!cross(out,cap,used,sizeof(*used));}
PC_ROOT8_RESULT pc_root8_v2_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,
 PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;PAPACC_U8 d[32];if(!outputs(p,n,out,cap,used)||n>PC_ROOT8_V2_FIELDS_MAX||cap<n+32U)return PC_ROOT8_INVALID;
 *used=0;s=shape(p,n,hash,hc,component,cc);if(s)return s;if(!hash||!hash(hc,p,n,d))return PC_ROOT8_HASH_FAILED;
 memcpy(out,p,n);memcpy(out+n,d,32);*used=n+32U;return PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_v2_decode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,int *complete,
 PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;if(!outputs(p,n,out,cap,used)||!range(complete,sizeof(*complete))||cross(complete,sizeof(*complete),p,n)||
 cross(complete,sizeof(*complete),out,cap)||cross(complete,sizeof(*complete),used,sizeof(*used)))return PC_ROOT8_INVALID;
 *used=0;*complete=0;if(n<PC_ROOT8_V2_MIN||n>PC_ROOT8_V2_MAX||cap<n-32U)return PC_ROOT8_INVALID;
 s=pc_root8_v2_validate(p,n,hash,hc,component,cc);if(s)return s;memcpy(out,p,n-32U);*used=n-32U;*complete=1;return PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_v2_snt_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{
 PC_ROOT8_RESULT s;PAPACC_SIZE S;PAPACC_U8 d[32];if(!outputs(p,n,out,cap,used))return PC_ROOT8_INVALID;*used=0;
 s=header(p,n);if(s)return s;S=u32(p+248);if(cap<S)return PC_ROOT8_INVALID;s=pc_root8_snt_data(p,0,hash,hc);if(s)return s;
 if(!hash||!hash(hc,p+u32(p+244),S-32U,d))return PC_ROOT8_HASH_FAILED;memcpy(out,p+u32(p+244),S-32U);memcpy(out+S-32U,d,32);*used=S;return PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_v2_cp_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{
 PC_ROOT8_RESULT s;PAPACC_U8 d[32];const PAPACC_U8 *cp;if(!outputs(p,n,out,cap,used)||cap<512U)return PC_ROOT8_INVALID;*used=0;
 s=header(p,n);if(s)return s;s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;s=checkpoint(p,0,hash,hc);if(s)return s;cp=p+u32(p+56);
 if(zero(cp,512)){memset(out,0,512);*used=512;return PC_ROOT8_OK;}
 if(!hash||!hash(hc,cp,480,d))return PC_ROOT8_HASH_FAILED;memcpy(out,cp,480);memcpy(out+480,d,32);*used=512;return PC_ROOT8_OK;
}

/* No native store, pin or authority is created by this DATA relation. */
PC_ROOT8_RESULT pc_root8_v2_base_link(const PAPACC_U8 *next,PAPACC_SIZE nn,
 const PAPACC_U8 *base,PAPACC_SIZE bn,PC_LST1_HASH hash,void *hc,
 PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;PAPACC_U8 d[32];const PAPACC_U8 *cp,*lst,*a,*b;
 PAPACC_SIZE bq,bo,no;unsigned i,N,K;PAPACC_ISSUER_CHECKPOINT_REPORT report;
 PAPACC_ISSUER_RESULT ir;
 if(!range(next,nn)||nn<PC_ROOT8_V2_MIN||nn>PC_ROOT8_V2_MAX)return PC_ROOT8_INVALID;
 if(!base)return bn?PC_ROOT8_INVALID:PC_ROOT8_INCOMPLETE;
 if(!range(base,bn)||bn<PC_ROOT8_MIN||bn>PC_ROOT8_V2_MAX||cross(next,nn,base,bn))return PC_ROOT8_INVALID;
 s=pc_root8_v2_validate(next,nn,hash,hc,component,cc);if(s)return s;
 if(!memcmp(base,"PACROOT8",8)&&u16(base+8)==1U)return PC_ROOT8_INCOMPLETE;
 s=pc_root8_v2_validate(base,bn,hash,hc,component,cc);if(s)return s;
 if(u16(next+240)!=2U||memcmp(next+72,base+72,32)||u64(next+112)!=u64(base+104)||
    u64(next+136)!=u64(base+104)||memcmp(next+208,base+bn-32U,32))return PC_ROOT8_INVALID;
 bq=u32(base+56);cp=base+bq;
 if(zero(cp,512))return PC_ROOT8_INCOMPLETE;
 if(u64(next+144)!=u64(cp+64)||u64(next+152)!=u64(cp+72)||u64(next+160)!=u64(cp+80))return PC_ROOT8_INVALID;
 if(!hash||!hash(hc,base,bq,d))return PC_ROOT8_HASH_FAILED;
 if(memcmp(d,next+176,32))return PC_ROOT8_INVALID;
 N=u16(base+20);lst=next+u32(next+40);
 if(u32(next+44)!=160U+512U*N)return PC_ROOT8_INVALID;
 for(i=0;i<N;i++){
  a=base+512U+1536U*i;b=lst+128U+512U*i;
  if(memcmp(a,b,344)||!zero(b+344,40)||memcmp(a+384,b+384,128))return PC_ROOT8_INVALID;
 }
 bo=u32(base+48);no=u32(next+48);
 if(memcmp(base+bo,next+no,8192))return PC_ROOT8_INVALID;
 ir=papacc_issuer_checkpoint_assess(next+no+8192,8192,base+bo+8192,8192,hash,hc,0,&report);
 if(ir==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(ir!=PAPACC_ISSUER_DENIED||report.state!=PAPACC_ISSUER_CHECKPOINT_DATA_VALID_RECOVERY_DENIED)return PC_ROOT8_INVALID;
 /* Full approval rows are NOT carried by base LST1. No silent replacement or
  * reconstruction: accept identical rows as DATA, defer any changed history. */
 K=u16(base+252);
 if(u16(next+252)!=K)return PC_ROOT8_INCOMPLETE;
 a=base+u32(base+244)+64U;b=next+u32(next+244)+64U;
 if(memcmp(a,b,768U*K))return PC_ROOT8_INCOMPLETE;
 return PC_ROOT8_OK;
}
