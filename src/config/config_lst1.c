#include "config_lst1.h"
#include <stdint.h>
#include <string.h>
static int span(const void *p,PAPACC_SIZE n)
{return p&&n&&n-1U<=UINTPTR_MAX-(uintptr_t)p;}
static int overlap(const void *a,PAPACC_SIZE an,const void *b,PAPACC_SIZE bn)
{uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;return x<=y?y-x<an:x-y<bn;}
static unsigned u16(const PAPACC_U8 *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static PAPACC_U32 u32(const PAPACC_U8 *p)
{return (PAPACC_U32)p[0]|((PAPACC_U32)p[1]<<8)|((PAPACC_U32)p[2]<<16)|((PAPACC_U32)p[3]<<24);}
static PAPACC_U64 u64(const PAPACC_U8 *p)
{PAPACC_U64 v=0;unsigned i;for(i=0;i<8U;++i)v|=(PAPACC_U64)p[i]<<(8U*i);return v;}
static int zero(const PAPACC_U8 *p,PAPACC_SIZE n)
{PAPACC_SIZE i;for(i=0;i<n;++i)if(p[i])return 0;return 1;}
static int alpha(unsigned c){return (c>='a'&&c<='z')||(c>='A'&&c<='Z');}
static int ident(const PAPACC_U8 *p,unsigned n)
{unsigned i;if(!n||n>64U||!alpha(p[0]))return 0;
 for(i=1;i<n;++i)if(!alpha(p[i])&&!(p[i]>='0'&&p[i]<='9')&&p[i]!='_'&&p[i]!='.'&&p[i]!='-')return 0;
 return zero(p+n,64U-n);}
static int utf8(const PAPACC_U8 *p,unsigned n)
{unsigned i=0,c,v,need,min,k;if(!n||n>64U)return 0;
 while(i<n){c=p[i++];if(c<128U){if(c<32U||c==127U)return 0;continue;}
  if(c>=194U&&c<=223U){need=1U;v=c&31U;min=128U;}
  else if(c>=224U&&c<=239U){need=2U;v=c&15U;min=2048U;}
  else if(c>=240U&&c<=244U){need=3U;v=c&7U;min=65536U;}else return 0;
  if(n-i<need)return 0;
  for(k=0;k<need;++k){if((p[i]&192U)!=128U)return 0;v=(v<<6)|(p[i++]&63U);}
  if(v<min||v>1114111U||(v>=55296U&&v<=57343U)||v==65279U)return 0;
 }return zero(p+n,64U-n);}
PC_LST1_RESULT pc_lst1_record_shape(const PAPACC_U8 *q,PAPACC_SIZE n)
{
 unsigned flags,profile,admission,binding,auth,id,sec,ar,pol,ids,j,k;int unsupported=0;
 if(!span(q,n)||n!=512U)return PC_LST1_INVALID;
 flags=u32(q);profile=q[4];admission=q[5];binding=q[6];auth=q[7];
  id=u16(q+10);sec=u16(q+12);ar=u16(q+14);pol=u16(q+16);ids=u16(q+18);
  if((flags&63U)!=63U||(flags&~255U)||(profile!=1U&&profile!=2U)||
   (admission!=1U&&admission!=2U)||(binding!=1U&&binding!=2U)||auth>1U||!u16(q+8)||
   !zero(q+20,4)||!ident(q+24,id)||!ident(q+216,pol)||zero(q+280,32)||zero(q+312,32)||
   !zero(q+344,40)||ids>8U||(binding==1U?ids!=0U:ids==0U))return PC_LST1_INVALID;
  if(profile==1U){if(!(flags&64U)||!utf8(q+88,sec))return PC_LST1_INVALID;}
  else if((flags&64U)||sec||!zero(q+88,64))return PC_LST1_INVALID;
  if(!auth){if((flags&128U)||ar||!zero(q+152,64))return PC_LST1_INVALID;}
  else if(!(flags&128U)||!utf8(q+152,ar))return PC_LST1_INVALID;
  if(admission==2U||auth==1U)unsupported=1;
  for(j=0;j<8U;++j){const PAPACC_U8 *entry=q+384U+16U*j;
   if(j>=ids){if(!zero(entry,16))return PC_LST1_INVALID;}
   else {if(entry[0]!=1U||!zero(entry+1,7))return PC_LST1_INVALID;
    for(k=0;k<j;++k)if(u64(entry+8)==u64(q+392U+16U*k))return PC_LST1_INVALID;}}
 return unsupported?PC_LST1_UNSUPPORTED:PC_LST1_OK;
}
static PC_LST1_RESULT structural(const PAPACC_U8 *p,PAPACC_SIZE n)
{
 unsigned count,i,j,flags,id;
 const PAPACC_U8 *q,*previous;PAPACC_U64 r;int unsupported=0;
 if(n<PC_LST1_HEADER||memcmp(p,"PACLST01",8)||u16(p+8)!=1U||u16(p+10)!=128U||
  u16(p+18)!=512U||u16(p+96)!=1U||!zero(p+98,30))return PC_LST1_INVALID;
 count=u16(p+16);if(count>4U||n!=128U+512U*count||u32(p+12)!=n+32U)return PC_LST1_INVALID;
 flags=u32(p+20);r=u64(p+56);
 if((flags!=15U&&flags!=31U)||zero(p+24,32)||!r||!u64(p+64)||!u64(p+72)||
  u64(p+88)!=r-1U||((flags&16U)?!u64(p+80):!zero(p+80,8)))return PC_LST1_INVALID;
 for(i=0;i<count;++i){q=p+128U+512U*i;
  {PC_LST1_RESULT shape=pc_lst1_record_shape(q,512U);
   if(shape==PC_LST1_INVALID)return shape;
   if(shape==PC_LST1_UNSUPPORTED)unsupported=1;}
  id=u16(q+10);
  for(j=0;j<i;++j){previous=p+128U+512U*j;
   if(id==u16(previous+10)&&!memcmp(q+24,previous+24,id))return PC_LST1_INVALID;}
 }
 return unsupported?PC_LST1_UNSUPPORTED:PC_LST1_OK;
}
PC_LST1_RESULT pc_lst1_encode(const PC_LST1_MODEL *m,PAPACC_U8 *out,PAPACC_SIZE cap,
 PAPACC_SIZE *used,PC_LST1_HASH hash,void *ctx)
{
 PAPACC_SIZE n;PAPACC_U8 digest[32];PC_LST1_RESULT result;
 if(!span(m,sizeof(*m))||!span(out,cap)||!span(used,sizeof(*used))||
  overlap(m,sizeof(*m),out,cap)||overlap(m,sizeof(*m),used,sizeof(*used))||
  overlap(out,cap,used,sizeof(*used)))return PC_LST1_INVALID;
 /* Capacity precondition: no writes if destination cannot hold even header. */
 if(cap<160U)return PC_LST1_INVALID;
 if(u16(m->fields+16)>4U){*used=0U;return PC_LST1_INVALID;}
 n=128U+512U*u16(m->fields+16);
 if(cap<n+32U)return PC_LST1_INVALID;
 *used=0U;
 result=structural(m->fields,n);if(result!=PC_LST1_OK)return result;
 if(!zero(m->fields+n,PC_LST1_MODEL_BYTES-n))return PC_LST1_INVALID;
 if(!hash||!hash(ctx,m->fields,n,digest))return PC_LST1_HASH_FAILED;
 memcpy(out,m->fields,n);memcpy(out+n,digest,32);*used=n+32U;return PC_LST1_OK;
}
PC_LST1_RESULT pc_lst1_decode(const PAPACC_U8 *in,PAPACC_SIZE n,PC_LST1_MODEL *out,
 int *complete,PC_LST1_HASH hash,void *ctx)
{
 PAPACC_U8 digest[32];PC_LST1_RESULT result;
 if(!span(in,n)||!span(out,sizeof(*out))||!span(complete,sizeof(*complete))||
  overlap(in,n,out,sizeof(*out))||overlap(in,n,complete,sizeof(*complete))||
  overlap(out,sizeof(*out),complete,sizeof(*complete)))return PC_LST1_INVALID;
 *complete=0;memset(out,0,sizeof(*out));
 if(n<160U||n>PC_LST1_MAX_BYTES)return PC_LST1_INVALID;
 result=structural(in,n-32U);if(result==PC_LST1_INVALID)return result;
 if(!hash||!hash(ctx,in,n-32U,digest))return PC_LST1_HASH_FAILED;
 if(memcmp(digest,in+n-32U,32))return PC_LST1_INVALID;
 if(result!=PC_LST1_OK)return result;
 memcpy(out->fields,in,n-32U);*complete=1;return PC_LST1_OK;
}
