#include "config_root8_v3.h"
#include "config_root8_internal.h"
#include "../issuer/issuer_codec.h"
#include <stdint.h>
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
 unsigned N,K,D,B;PAPACC_SIZE O,S,U,V,L,Q;PAPACC_U64 pre;
 if(!range(p,n)||n<PC_ROOT8_V3_MIN-32U||n>PC_ROOT8_V3_FIELDS_MAX||memcmp(p,"PACROOT8",8)||u16(p+8)!=3U||
 u16(p+10)!=512U||u32(p+12)!=n+32U||u32(p+16)!=3U||u16(p+22)!=512U||u16(p+24)!=1024U||u16(p+26)!=1U||
 u16(p+28)!=512U||u16(p+30)!=1U||zero(p+72,32)||u32(p+128)!=31U||u16(p+132)!=5U||u16(p+134)!=1U||
 u32(p+168)!=15U||!zero(p+172,4)||(u16(p+240)!=2U&&u16(p+240)!=3U)||u16(p+242)!=1U||u16(p+254)!=768U||
 u16(p+256)!=1U||u16(p+258)!=1U||u16(p+270)!=384U||u16(p+272)!=1U||!zero(p+274,238))return PC_ROOT8_INVALID;
 N=u16(p+20);K=u16(p+252);D=u16(p+268);L=u32(p+44);
 if(!N||N>4U||!K||K>N||D>K||L<672U||L>2208U||(L-160U)%512U)return PC_ROOT8_INVALID;
 B=(unsigned)((L-160U)/512U);if(!B||B>4U)return PC_ROOT8_INVALID;
 O=512U+1536U*N;S=96U+768U*K;U=O+S;V=96U+384U*D;Q=U+V+L+16384U;
 if(Q+512U!=n||u32(p+32)!=512U||u32(p+36)!=1536U*N||u32(p+40)!=U+V||u32(p+48)!=U+V+L||
 u32(p+52)!=16384U||u32(p+56)!=Q||u32(p+60)!=512U||u32(p+64)!=Q+512U||u32(p+68)!=32U||u32(p+244)!=O||
 u32(p+248)!=S||u32(p+260)!=U||u32(p+264)!=V)return PC_ROOT8_INVALID;
 pre=u64(p+112);if(!pre||pre==(PAPACC_U64)-1||u64(p+104)!=pre+1U||u64(p+136)!=pre||
 !u64(p+144)||!u64(p+152)||!u64(p+160)||!u64(p+120)||u64(p+120)<u64(p+160))return PC_ROOT8_INVALID;
 return PC_ROOT8_OK;
}
static const PAPACC_U8 *association(const PAPACC_U8 *p,unsigned ordinal)
{unsigned j;const PAPACC_U8 *s=p+u32(p+244)+64U;for(j=0;j<u16(p+252);j++,s+=768U)if(u16(s+16)==ordinal)return s;return NULL;}
static PC_ROOT8_RESULT pdg(const PAPACC_U8 *p,int integrity,PC_LST1_HASH hash,void *hc)
{
 const PAPACC_U8 *s=p+u32(p+260),*a,*q,*sn;unsigned j,ordinal,len;PAPACC_SIZE V=u32(p+264);
 if(memcmp(s,"PACPDG01",8)||u16(s+8)!=1U||u16(s+10)!=64U||u32(s+12)!=V||u16(s+16)!=u16(p+268)||
 u16(s+18)!=384U||u32(s+20)||memcmp(s+24,p+72,32)||u64(s+56)!=u64(p+104))return PC_ROOT8_INVALID;
 for(j=0;j<u16(s+16);j++){
 a=s+64U+384U*j;ordinal=u16(a+22);len=u16(a+192);
 if(memcmp(a,"PACPDL01",8)||u16(a+8)!=1U||u16(a+10)!=384U||u32(a+12)!=1U||
 (u16(a+16)!=1U&&u16(a+16)!=2U)||u16(a+18)!=1U||u16(a+20)!=1U||ordinal>=u16(p+20)||
 (j&&ordinal<=u16(a-384U+22U))||!u64(a+24)||u64(a+24)>u64(p+104)||zero(a+32,32)||zero(a+64,16)||
 zero(a+80,16)||zero(a+96,16)||zero(a+112,32)||zero(a+144,32)||!u64(a+176)||u64(a+184)<=u64(a+176)||
 !len||len>64U||!zero(a+194,2)||!zero(a+196+len,64U-len)||zero(a+260,32)||!zero(a+292,92))return PC_ROOT8_INVALID;
 q=p+512U+1536U*ordinal;sn=association(p,ordinal);
 if(q[4]!=1U||!sn||u16(q+10)!=len||memcmp(q+24,a+196,64)||memcmp(q+280,a+32,32)||
 memcmp(sn+40,a+64,32)||memcmp(sn+568,a+260,32))return PC_ROOT8_INVALID;
 }
 return integrity?digest(s,V-32U,s+V-32U,hash,hc):PC_ROOT8_OK;
}
static PC_ROOT8_RESULT checkpoint(const PAPACC_U8 *p,int integrity,PC_LST1_HASH hash,void *hc)
{
 const PAPACC_U8 *c=p+u32(p+56);PC_ROOT8_RESULT s;if(zero(c,512))return PC_ROOT8_OK;
 if(memcmp(c,"PACR8CP3",8)||u16(c+8)!=3U||u16(c+10)!=512U||u32(c+12)!=1U||memcmp(c+16,p+72,32)||
 u64(c+48)!=u64(p+104)||u64(c+56)!=u64(p+112)||!u64(c+64)||!u64(c+72)||u64(c+80)!=u64(p+120)||
 memcmp(c+120,p+176,64)||!zero(c+184,296))return PC_ROOT8_INVALID;
 s=digest(p,u32(p+56),c+88,hash,hc);if(s)return s;return integrity?digest(c,480,c+480,hash,hc):PC_ROOT8_OK;
}
static PC_ROOT8_RESULT records(const PAPACC_U8 *p,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 const PAPACC_U8 *carrier=p+u32(p+48),*a;PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD r;PAPACC_ISSUER_RESULT ir;
 PC_ROOT8_RESULT s;unsigned j;if(!u16(p+268))return PC_ROOT8_OK;
 ir=papacc_issuer_image_validate(carrier+8192,&v,hash,hc);
 if(ir==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(ir!=PAPACC_ISSUER_OK||memcmp(v.root,p+72,32)||v.revision!=u64(p+120))return PC_ROOT8_INVALID;
 for(j=0;j<u16(p+268);j++){
 a=p+u32(p+260)+64U+384U*j;
 ir=papacc_issuer_image_find(carrier+8192,a+96,&r,hash,hc);
 if(ir==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(ir!=PAPACC_ISSUER_OK||r.request.purpose!=PAPACC_ISSUER_PACKAGE||memcmp(a+64,v.authority,16)||memcmp(a+80,v.generation,16)||
 memcmp(a+260,v.ca_spki_digest,32)||memcmp(a+112,r.certificate_digest,32)||memcmp(a+144,r.request.key_digest,32)||!r.certificate_size||
 (r.state!=PAPACC_ISSUER_ISSUED_UNPUBLISHED&&r.state!=PAPACC_ISSUER_PUBLISHED&&r.state!=PAPACC_ISSUER_REVOKED)||
 u64(a+176)<r.request.not_before||u64(a+184)>r.request.not_after||u64(a+184)>v.ca_not_after)return PC_ROOT8_INVALID;
 /* Full certificate/profile/SPKI/CA validity source is mandatory, not inferred
  * from digests, enum PACKAGE, policy ID or mathematical RSA. DATA ONLY. */
 s=component(cc,8U,carrier,16384,a,p);if(s)return s;
 }
 return PC_ROOT8_OK;
}
static PC_ROOT8_RESULT shape(const PAPACC_U8 *p,PAPACC_SIZE n,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;s=header(p,n);if(s)return s;s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;
 s=pdg(p,1,hash,hc);if(s)return s;s=checkpoint(p,1,hash,hc);if(s)return s;
 s=pc_root8_components_data(p,hash,hc,component,cc);if(s)return s;return records(p,hash,hc,component,cc);
}
static int outputs(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used)
{return range(p,n)&&range(out,cap)&&range(used,sizeof(*used))&&!cross(p,n,out,cap)&&!cross(p,n,used,sizeof(*used))&&!cross(out,cap,used,sizeof(*used));}
PC_ROOT8_RESULT pc_root8_v3_validate(const PAPACC_U8 *p,PAPACC_SIZE n,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{PC_ROOT8_RESULT s;if(!range(p,n)||n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V3_MAX)return PC_ROOT8_INVALID;s=header(p,n-32U);if(s)return s;
 s=digest(p,n-32U,p+n-32U,hash,hc);if(s)return s;return shape(p,n-32U,hash,hc,component,cc);}
PC_ROOT8_RESULT pc_root8_v3_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{PAPACC_U8 d[32];PC_ROOT8_RESULT s;if(!outputs(p,n,out,cap,used)||n>PC_ROOT8_V3_FIELDS_MAX||cap<n+32U)return PC_ROOT8_INVALID;
 *used=0;s=shape(p,n,hash,hc,component,cc);if(s)return s;if(!hash||!hash(hc,p,n,d))return PC_ROOT8_HASH_FAILED;
 memcpy(out,p,n);memcpy(out+n,d,32);*used=n+32U;return PC_ROOT8_OK;}
PC_ROOT8_RESULT pc_root8_v3_decode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,int *complete,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{PC_ROOT8_RESULT s;if(!outputs(p,n,out,cap,used)||!range(complete,sizeof(*complete))||cross(complete,sizeof(*complete),p,n)||cross(complete,sizeof(*complete),out,cap)||cross(complete,sizeof(*complete),used,sizeof(*used)))return PC_ROOT8_INVALID;
 *used=0;*complete=0;if(n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V3_MAX||cap<n-32U)return PC_ROOT8_INVALID;
 s=pc_root8_v3_validate(p,n,hash,hc,component,cc);if(s)return s;memcpy(out,p,n-32U);*used=n-32U;*complete=1;return PC_ROOT8_OK;}
/* No owned view escapes the synchronous fragment call. */
static PC_ROOT8_RESULT fragment(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc,unsigned kind)
{
 PC_ROOT8_RESULT s;PAPACC_SIZE off,len;PAPACC_U8 d[32];if(!outputs(p,n,out,cap,used))return PC_ROOT8_INVALID;*used=0;
 s=header(p,n);if(s)return s;
 if(kind==1U){off=u32(p+244);len=u32(p+248);s=pc_root8_snt_data(p,0,hash,hc);}
 else if(kind==2U){s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;off=u32(p+260);len=u32(p+264);s=pdg(p,0,hash,hc);}
 else{s=pc_root8_snt_data(p,1,hash,hc);if(s)return s;s=pdg(p,1,hash,hc);if(s)return s;off=u32(p+56);len=512;s=checkpoint(p,0,hash,hc);}
 if(s)return s;if(cap<len)return PC_ROOT8_INVALID;if(kind==3U&&zero(p+off,512)){memset(out,0,512);*used=512;return PC_ROOT8_OK;}
 if(!hash||!hash(hc,p+off,len-32U,d))return PC_ROOT8_HASH_FAILED;memcpy(out,p+off,len-32U);memcpy(out+len-32U,d,32);*used=len;return PC_ROOT8_OK;
}
PC_ROOT8_RESULT pc_root8_v3_snt_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{return fragment(p,n,out,cap,used,hash,hc,1U);}
PC_ROOT8_RESULT pc_root8_v3_pdg_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{return fragment(p,n,out,cap,used,hash,hc,2U);}
PC_ROOT8_RESULT pc_root8_v3_cp_encode(const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *used,PC_LST1_HASH hash,void *hc)
{return fragment(p,n,out,cap,used,hash,hc,3U);}
PC_ROOT8_RESULT pc_root8_v3_base_link(const PAPACC_U8 *next,PAPACC_SIZE nn,const PAPACC_U8 *base,PAPACC_SIZE bn,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;const PAPACC_U8 *cp,*a,*b,*lst;PAPACC_U8 d[32];unsigned version,j,N,K,D;PAPACC_SIZE bo,no;
 PAPACC_ISSUER_CHECKPOINT_REPORT report;PAPACC_ISSUER_RESULT ir;
 if(!range(next,nn)||nn<PC_ROOT8_V3_MIN||nn>PC_ROOT8_V3_MAX)return PC_ROOT8_INVALID;
 if(!base)return bn?PC_ROOT8_INVALID:PC_ROOT8_INCOMPLETE;
 if(!range(base,bn)||bn<PC_ROOT8_V2_MIN||bn>PC_ROOT8_V3_MAX||cross(next,nn,base,bn))return PC_ROOT8_INVALID;
 s=pc_root8_v3_validate(next,nn,hash,hc,component,cc);if(s)return s;
 if(memcmp(base,"PACROOT8",8))return PC_ROOT8_INVALID;version=u16(base+8);
 if(version==2U)s=pc_root8_v2_validate(base,bn,hash,hc,component,cc);
 else if(version==3U)s=pc_root8_v3_validate(base,bn,hash,hc,component,cc);else return PC_ROOT8_UNSUPPORTED;
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
 D=version==3U?u16(base+268):0;if(D!=u16(next+268))return PC_ROOT8_INCOMPLETE;
 if(D&&memcmp(base+u32(base+260)+64U,next+u32(next+260)+64U,384U*D))return PC_ROOT8_INCOMPLETE;
 return PC_ROOT8_OK;
}

PC_ROOT8_RESULT pc_root8_v3_pdg_select_data(const PAPACC_U8 *p,PAPACC_SIZE n,unsigned ordinal,
 PC_PDG1_SELECTION *out,PC_LST1_HASH hash,void *hc,PC_ROOT8_COMPONENT component,void *cc)
{
 PC_ROOT8_RESULT s;unsigned j;const PAPACC_U8 *row,*cp;PAPACC_ISSUER_RECORD record;PAPACC_ISSUER_RESULT ir;
 if(!range(p,n)||!range(out,sizeof(*out))||cross(p,n,out,sizeof(*out)))return PC_ROOT8_INVALID;
 memset(out,0,sizeof(*out));s=pc_root8_v3_validate(p,n,hash,hc,component,cc);if(s)return s;
 if(ordinal>=u16(p+20))return PC_ROOT8_INVALID;
 for(j=0;j<u16(p+268);j++){
 row=p+u32(p+260)+64U+384U*j;if(u16(row+22)!=ordinal)continue;
 ir=papacc_issuer_image_find(p+u32(p+48)+8192,row+96,&record,hash,hc);
 if(ir==PAPACC_ISSUER_UNCERTAIN)return PC_ROOT8_HASH_FAILED;
 if(ir!=PAPACC_ISSUER_OK)return PC_ROOT8_INVALID;
 memcpy(out->row,row,384);memcpy(out->root,p+72,32);memcpy(out->whole_digest,p+n-32U,32);
 out->revision=u64(p+104);out->issuer_revision=u64(p+120);out->certificate_state=(unsigned)record.state;
 cp=p+u32(p+56);if(!zero(cp,512)){out->administrative_epoch=u64(cp+64);out->policy_epoch=u64(cp+72);}
 return PC_ROOT8_OK;
 }
 return PC_ROOT8_INCOMPLETE;
}
