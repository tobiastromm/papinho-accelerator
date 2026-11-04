#include "config_root8_v5.h"
#include "config_root8_internal.h"
#include "../issuer/issuer_codec.h"
#include <stdint.h>
#include "../issuer/strict_ed25519.h"
#include <string.h>
static unsigned u16(const PAPACC_U8 *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static PAPACC_U32 u32(const PAPACC_U8 *p){return (PAPACC_U32)p[0]|((PAPACC_U32)p[1]<<8)|((PAPACC_U32)p[2]<<16)|((PAPACC_U32)p[3]<<24);}
static PAPACC_U64 u64(const PAPACC_U8 *p){PAPACC_U64 x=0;unsigned j;for(j=0;j<8U;j++)x|=(PAPACC_U64)p[j]<<(8U*j);return x;}
static int range(const void *p,PAPACC_SIZE n){return p&&n&&n-1U<=UINTPTR_MAX-(uintptr_t)p;}
static int cross(const void *a,PAPACC_SIZE n,const void *b,PAPACC_SIZE m){uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;return x<=y?y-x<n:x-y<m;}
static int zero(const PAPACC_U8 *p,PAPACC_SIZE n){PAPACC_SIZE j;for(j=0;j<n;j++)if(p[j])return 0;return 1;}
static PC_ROOT8_RESULT digest(const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *expected,PC_LST1_HASH hash,void *ctx)
{PAPACC_U8 d[32];if(!hash||!hash(ctx,p,n,d))return PC_ROOT8_HASH_FAILED;return memcmp(d,expected,32)?PC_ROOT8_INVALID:PC_ROOT8_OK;}
static PC_ROOT8_RESULT header(const PAPACC_U8 *p,PAPACC_SIZE n)
{
 unsigned N,K,D,B,H;PAPACC_SIZE O,S,U,V,L,Q,F;PAPACC_U64 pre;
 if(!range(p,n)||n<PC_ROOT8_V5_MIN-32U||n>PC_ROOT8_V5_FIELDS_MAX||memcmp(p,"PACROOT8",8)||u16(p+8)!=5U||
 u16(p+10)!=512U||u32(p+12)!=n+32U||u32(p+16)!=3U||u16(p+22)!=512U||u16(p+24)!=1024U||u16(p+26)!=1U||
 u16(p+28)!=512U||u16(p+30)!=1U||zero(p+72,32)||u32(p+128)!=31U||u16(p+132)!=5U||u16(p+134)!=1U||
 u32(p+168)!=15U||!zero(p+172,4)||(u16(p+240)!=4U&&u16(p+240)!=5U)||u16(p+242)!=1U||u16(p+254)!=768U||
 u16(p+256)!=1U||u16(p+258)!=1U||u16(p+270)!=384U||u16(p+272)!=1U||(u16(p+274)!=1U||u16(p+286)!=384U||u16(p+288)!=1U||u16(p+290)||u32(p+296)!=512U||u16(p+300)!=1U||u16(p+302)!=1U||!zero(p+304,208)))return PC_ROOT8_INVALID;
 N=u16(p+20);K=u16(p+252);D=u16(p+268);H=u16(p+284);L=u32(p+44);
 if(!N||N>4U||!K||K>N||D>K||H>3U||L<672U||L>2208U||(L-160U)%512U)return PC_ROOT8_INVALID;
 B=(unsigned)((L-160U)/512U);if(!B||B>4U)return PC_ROOT8_INVALID;
 O=512U+1536U*N;S=96U+768U*K;U=O+S;V=96U+384U*D;F=96U+384U*H;Q=U+V+F+L+16384U;
 if(Q+1024U!=n||u32(p+292)!=Q||u32(p+32)!=512U||u32(p+36)!=1536U*N||u32(p+40)!=U+V+F||u32(p+48)!=U+V+F+L||
 u32(p+52)!=16384U||u32(p+56)!=Q+512U||u32(p+60)!=512U||u32(p+64)!=Q+1024U||u32(p+68)!=32U||u32(p+244)!=O||
 u32(p+248)!=S||u32(p+260)!=U||u32(p+264)!=V||u32(p+276)!=U+V||u32(p+280)!=F)return PC_ROOT8_INVALID;
 pre=u64(p+112);if(!pre||pre==(PAPACC_U64)-1||u64(p+104)!=pre+1U||u64(p+136)!=pre||
 !u64(p+144)||!u64(p+152)||!u64(p+160)||!u64(p+120)||u64(p+120)<u64(p+160))return PC_ROOT8_INVALID;
 return PC_ROOT8_OK;
}
static int endpoint(const PAPACC_U8 *a)
{
 unsigned family=u16(a+22);const PAPACC_U8 *ip=a+152;
 if(u16(a+168)!=5319U)return 0;
 if(family==4U)return !zero(ip,4)&&ip[0]<224U&&zero(ip+4,12);
 if(family!=6U||zero(ip,16)||ip[0]==255U||(ip[0]==254U&&(ip[1]&192U)==128U))return 0;
 if(zero(ip,10)&&ip[10]==255U&&ip[11]==255U)return 0;
 return 1;
}
static PC_ROOT8_RESULT sources(const PAPACC_U8 *p,int integrity,PC_LST1_HASH hash,void *hc)
{
 const PAPACC_U8 *s=p+u32(p+276),*a;PAPACC_SIZE F=u32(p+280);PAPACC_U8 d[32];unsigned j;
 if(memcmp(s,"PACTSC01",8)||u16(s+8)!=1U||u16(s+10)!=64U||u32(s+12)!=F||u16(s+16)!=u16(p+284)||u16(s+18)!=384U||u32(s+20)||memcmp(s+24,p+72,32)||u64(s+56)!=u64(p+104))return PC_ROOT8_INVALID;
 for(j=0;j<u16(s+16);j++){
  a=s+64U+384U*j;
  if(memcmp(a,"PACTSR01",8)||u16(a+8)!=1U||u16(a+10)!=384U||u32(a+12)!=1U||u16(a+16)>3U||u16(a+18)!=1U||u16(a+20)!=1U||
   zero(a+24,32)||zero(a+56,32)||zero(a+88,32)||zero(a+120,32)||!endpoint(a)||!zero(a+170,6)||zero(a+176,32)||
   !u64(a+208)||u64(a+208)>u64(p+104)||!u64(a+216)||u64(a+224)<=u64(a+216)||zero(a+232,32)||zero(a+264,32)||zero(a+296,32)||!zero(a+328,56)||
   (j&&memcmp(a-384U+24U,a+24,32)>=0))return PC_ROOT8_INVALID;
  if(!hash||!hash(hc,a+88,32,d))return PC_ROOT8_HASH_FAILED;if(memcmp(d,a+120,32))return PC_ROOT8_INVALID;
 }
 return integrity?digest(s,F-32U,s+F-32U,hash,hc):PC_ROOT8_OK;
}
static PC_ROOT8_RESULT source_keys(const PAPACC_U8 *p,PC_ROOT8_COMPONENT component,void *cc)
{
 unsigned j;PC_ROOT8_RESULT s;const PAPACC_U8 *a;
 if(u16(p+284)&&!component)return PC_ROOT8_INCOMPLETE;
 for(j=0;j<u16(p+284);j++){
  a=p+u32(p+276)+64U+384U*j;
  /* Private DATA discriminator9, not wire/policy/authority. Adapter must prove
   * public Ed25519 key validity using an existing qualified backend. It must
   * never resolve operator/act/status/CURRENT or grant positive trust here. */
  s=component(cc,9U,a,384U,a,p);if(s)return s;
 }
 return PC_ROOT8_OK;
}
static PC_ROOT8_RESULT checkpoint(const PAPACC_U8 *p,int integrity,PC_LST1_HASH hash,void *hc)
{
 const PAPACC_U8 *c=p+u32(p+56);PC_ROOT8_RESULT s;if(zero(c,512))return PC_ROOT8_OK;
 if(memcmp(c,"PACR8CP5",8)||u16(c+8)!=5U||u16(c+10)!=512U||u32(c+12)!=1U||memcmp(c+16,p+72,32)||
 u64(c+48)!=u64(p+104)||u64(c+56)!=u64(p+112)||!u64(c+64)||!u64(c+72)||u64(c+80)!=u64(p+120)||
 memcmp(c+120,p+176,64)||!zero(c+184,296))return PC_ROOT8_INVALID;
 s=digest(p,u32(p+56),c+88,hash,hc);if(s)return s;return integrity?digest(c,480,c+480,hash,hc):PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_v5_key_data(void *ctx,unsigned kind,const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *h)
{
 int result;(void)ctx;
 if(kind!=9U)return PC_ROOT8_UNSUPPORTED;
 if(!p||n!=384U||p!=row||!h||memcmp(h,"PACROOT8",8)||h[8]!=5U||h[9]||memcmp(p,"PACTSR01",8)||p[8]!=1U||p[9]||p[10]!=128U||p[11]!=1U)return PC_ROOT8_INVALID;
 result=papacc_time_source_ed25519_check(p+88U,32U);
 return result==PAPACC_ED25519_VALID?PC_ROOT8_OK:result==PAPACC_ED25519_INVALID?PC_ROOT8_INVALID:result==PAPACC_ED25519_BACKEND_UNAVAILABLE?PC_ROOT8_UNSUPPORTED:PC_ROOT8_INCOMPLETE;
}
static PAPACC_U64 be64(const PAPACC_U8 *p)
{PAPACC_U64 v=0;unsigned j;for(j=0;j<8U;j++)v=(v<<8)|p[j];return v;}
PC_ROOT8_RESULT pc_root8_q4_data(const PAPACC_U8 *p,PAPACC_SIZE n,PC_LST1_HASH hash,void *hc)
{
 unsigned j;PAPACC_U64 first;
 if(!range(p,n)||n!=512U||memcmp(p,"PACQ4D01",8)||p[8]!=1U||p[9]<1U||p[9]>4U||!zero(p+10,2)||!zero(p+372,140))return PC_ROOT8_INVALID;
 first=be64(p+44);
 if(be64(p+12)==(PAPACC_U64)-1||be64(p+20)!=be64(p+12)+1U||!be64(p+28)||be64(p+28)>0xffffffffU||!be64(p+36)||
 (first>11U&&first!=255U)||(p[9]<4U&&first)||(p[9]==4U&&!first))return PC_ROOT8_INVALID;
 for(j=0;j<9U;j++)if(zero(p+52U+32U*j,32))return PC_ROOT8_INCOMPLETE;
 return digest(p,340U,p+340U,hash,hc);
}
/* No resumption/adoption: same original invocation and frozen manifests. */
static PC_ROOT8_RESULT q4_link(const PAPACC_U8 *next,const PAPACC_U8 *base,PAPACC_SIZE bn,unsigned version,PC_LST1_HASH hash,void *hc)
{
 const PAPACC_U8 *b=next+u32(next+292),*a;unsigned x,y;PAPACC_U8 whole[32];
 if(memcmp(b+52,next+72,32))return PC_ROOT8_INVALID;
 if(version==4U){if(!hash||!hash(hc,base,bn,whole))return PC_ROOT8_HASH_FAILED;return b[9]==1U&&!memcmp(b+84,whole,32)?PC_ROOT8_OK:PC_ROOT8_INVALID;}
 a=base+u32(base+292);x=a[9];y=b[9];
 if(memcmp(a+12,b+12,32)||memcmp(a+52,b+52,288))return PC_ROOT8_INVALID;
 if(!((x==1U&&(y==2U||y==4U))||(x==2U&&(y==3U||y==4U))))return PC_ROOT8_INVALID;
 return PC_ROOT8_OK;
}
/* BODY0.5 root lineage and its two checksums may change; configuration cannot. */
static PC_ROOT8_RESULT documentary_only(const PAPACC_U8 *a,const PAPACC_U8 *b)
{
 unsigned j,N=u16(a+20);const PAPACC_U8 *x,*y;
 if(N!=u16(b+20)||u64(a+120)!=u64(b+120)||memcmp(a+u32(a+48),b+u32(b+48),16384))return PC_ROOT8_INVALID;
 for(j=0;j<N;j++){
 x=a+512U+1536U*j;y=b+512U+1536U*j;
 if(memcmp(x,y,312)||memcmp(x+344,y+344,168)||memcmp(x+512,y+512,40)||
 memcmp(x+568,y+568,424)||memcmp(x+1024,y+1024,480))return PC_ROOT8_INVALID;
 }
 return PC_ROOT8_OK;
}
static PC_ROOT8_RESULT shape(const PAPACC_U8 *p,PAPACC_SIZE n,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;s=header(p,n);if(s)return s;s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;
 s=pc_root8_pdg_data(p,1,hash,hc);if(s)return s;s=sources(p,1,hash,hc);if(s)return s;s=checkpoint(p,1,hash,hc);if(s)return s;
 s=pc_root8_components_data(p,hash,hc,component,cc);if(s)return s;s=pc_root8_pdg_records_data(p,hash,hc,component,cc);if(s)return s;s=source_keys(p,component,cc);if(s)return s;s=pc_root8_q4_data(p+u32(p+292),512U,hash,hc);if(s)return s;return component?component(cc,10U,p+u32(p+292),512U,NULL,p):PC_ROOT8_INCOMPLETE;
}
static int outputs(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used)
{return range(p,n)&&range(out,cap)&&range(used,sizeof(*used))&&!cross(p,n,out,cap)&&!cross(p,n,used,sizeof(*used))&&!cross(out,cap,used,sizeof(*used));}
PC_ROOT8_RESULT pc_root8_v5_validate(const PAPACC_U8 *p,PAPACC_SIZE n,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{PC_ROOT8_RESULT s;if(!range(p,n)||n<PC_ROOT8_V5_MIN||n>PC_ROOT8_V5_MAX)return PC_ROOT8_INVALID;s=header(p,n-32U);if(s)return s;
 s=digest(p,n-32U,p+n-32U,hash,hc);if(s)return s;return shape(p,n-32U,hash,hc,component,cc);}
PC_ROOT8_RESULT pc_root8_v5_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{PAPACC_U8 d[32];PC_ROOT8_RESULT s;if(!outputs(p,n,out,cap,used)||n>PC_ROOT8_V5_FIELDS_MAX||cap<n+32U)return PC_ROOT8_INVALID;
 *used=0;s=shape(p,n,hash,hc,component,cc);if(s)return s;if(!hash||!hash(hc,p,n,d))return PC_ROOT8_HASH_FAILED;
 memcpy(out,p,n);memcpy(out+n,d,32);*used=n+32U;return PC_ROOT8_OK;}
PC_ROOT8_RESULT pc_root8_v5_decode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,int *complete,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{PC_ROOT8_RESULT s;if(!outputs(p,n,out,cap,used)||!range(complete,sizeof(*complete))||cross(complete,sizeof(*complete),p,n)||cross(complete,sizeof(*complete),out,cap)||cross(complete,sizeof(*complete),used,sizeof(*used)))return PC_ROOT8_INVALID;
 *used=0;*complete=0;if(n<PC_ROOT8_V5_MIN||n>PC_ROOT8_V5_MAX||cap<n-32U)return PC_ROOT8_INVALID;
 s=pc_root8_v5_validate(p,n,hash,hc,component,cc);if(s)return s;memcpy(out,p,n-32U);*used=n-32U;*complete=1;return PC_ROOT8_OK;}
/* No owned view escapes the synchronous fragment call. */
static PC_ROOT8_RESULT fragment(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc,unsigned kind)
{
 PC_ROOT8_RESULT s;PAPACC_SIZE off,len;PAPACC_U8 d[32];if(!outputs(p,n,out,cap,used))return PC_ROOT8_INVALID;*used=0;
 s=header(p,n);if(s)return s;
 if(kind==1U){off=u32(p+244);len=u32(p+248);s=pc_root8_snt_data(p,0,hash,hc);}
 else if(kind==2U){s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;off=u32(p+260);len=u32(p+264);s=pc_root8_pdg_data(p,0,hash,hc);}
 else if(kind==4U){s=sources(p,0,hash,hc);off=u32(p+276);len=u32(p+280);}
 else{s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;s=pc_root8_pdg_data(p,1,hash,hc);if(s)return s;s=sources(p,1,hash,hc);if(s)return s;off=u32(p+56);len=512;s=checkpoint(p,0,hash,hc);}
 if(s)return s;if(cap<len)return PC_ROOT8_INVALID;if(kind==3U&&zero(p+off,512)){memset(out,0,512);*used=512;return PC_ROOT8_OK;}
 if(!hash||!hash(hc,p+off,len-32U,d))return PC_ROOT8_HASH_FAILED;memcpy(out,p+off,len-32U);memcpy(out+len-32U,d,32);*used=len;return PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_v5_snt_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{return fragment(p,n,out,cap,used,hash,hc,1U);}
PC_ROOT8_RESULT pc_root8_v5_pdg_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{return fragment(p,n,out,cap,used,hash,hc,2U);}
PC_ROOT8_RESULT pc_root8_v5_cp_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{return fragment(p,n,out,cap,used,hash,hc,3U);}
PC_ROOT8_RESULT pc_root8_v5_tsc_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{return fragment(p,n,out,cap,used,hash,hc,4U);}
PC_ROOT8_RESULT pc_root8_v5_base_link(const PAPACC_U8 *next,PAPACC_SIZE nn,const PAPACC_U8 *base,PAPACC_SIZE bn,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;const PAPACC_U8 *cp,*a,*b,*lst;PAPACC_U8 d[32];unsigned version,j,N,K,D;PAPACC_SIZE bo,no;
 PAPACC_ISSUER_CHECKPOINT_REPORT report;PAPACC_ISSUER_RESULT ir;
 if(!range(next,nn)||nn<PC_ROOT8_V5_MIN||nn>PC_ROOT8_V5_MAX)return PC_ROOT8_INVALID;
 if(!base)return bn?PC_ROOT8_INVALID:PC_ROOT8_INCOMPLETE;
 if(!range(base,bn)||bn<PC_ROOT8_V4_MIN||bn>PC_ROOT8_V5_MAX||cross(next,nn,base,bn))return PC_ROOT8_INVALID;
 s=pc_root8_v5_validate(next,nn,hash,hc,component,cc);if(s)return s;
 if(memcmp(base,"PACROOT8",8))return PC_ROOT8_INVALID;version=u16(base+8);
 if(version==4U)s=pc_root8_v4_validate(base,bn,hash,hc,component,cc);
 else if(version==5U)s=pc_root8_v5_validate(base,bn,hash,hc,component,cc);else return PC_ROOT8_UNSUPPORTED;
 if(s)return s;if(u16(next+240)!=version||memcmp(next+72,base+72,32)||u64(next+112)!=u64(base+104)||u64(next+136)!=u64(base+104)||memcmp(next+208,base+bn-32U,32))return PC_ROOT8_INVALID;
 cp=base+u32(base+56);if(zero(cp,512))return PC_ROOT8_INCOMPLETE;
 if(u64(next+144)!=u64(cp+64)||u64(next+152)!=u64(cp+72)||u64(next+160)!=u64(cp+80))return PC_ROOT8_INVALID;
 if(!hash||!hash(hc,base,u32(base+56),d))return PC_ROOT8_HASH_FAILED;if(memcmp(d,next+176,32))return PC_ROOT8_INVALID;
 N=u16(base+20);lst=next+u32(next+40);if(u32(next+44)!=160U+512U*N)return PC_ROOT8_INVALID;
 for(j=0;j<N;j++){a=base+512U+1536U*j;b=lst+128U+512U*j;if(memcmp(a,b,344)||!zero(b+344,40)||memcmp(a+384,b+384,128))return PC_ROOT8_INVALID;}
 bo=u32(base+48);no=u32(next+48);if(memcmp(base+bo,next+no,8192))return PC_ROOT8_INVALID;
 ir=papacc_issuer_checkpoint_assess(next+no+8192,8192,base+bo+8192,8192,hash,hc,0,&report);
 if(ir==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(ir!=PAPACC_ISSUER_DENIED||report.state!=PAPACC_ISSUER_CHECKPOINT_DATA_VALID_RECOVERY_DENIED)return PC_ROOT8_INVALID;
 K=u16(base+252);if(u16(next+252)!=K)return PC_ROOT8_INCOMPLETE;
 if(memcmp(base+u32(base+244)+64U,next+u32(next+244)+64U,768U*K))return PC_ROOT8_INCOMPLETE;
 D=u16(base+268);if(D!=u16(next+268))return PC_ROOT8_INCOMPLETE;
 if(D&&memcmp(base+u32(base+260)+64U,next+u32(next+260)+64U,384U*D))return PC_ROOT8_INCOMPLETE;
 /* A change needs the separately protected administrative disposition source.
  * Identical bytes are DATA continuity, never that source or CURRENT. */
 j=u16(base+284);if(j!=u16(next+284))return PC_ROOT8_INCOMPLETE;
 if(j&&memcmp(base+u32(base+276)+64U,next+u32(next+276)+64U,384U*j))return PC_ROOT8_INCOMPLETE;
 s=documentary_only(base,next);if(s)return s;return q4_link(next,base,bn,version,hash,hc);
}

