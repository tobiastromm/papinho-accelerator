#include "manual_package.h"
#include <stdint.h>
#include <string.h>
static const char *const names[5]={"ca.der","cliente.der","cliente.pk8","config.bin","issuance.bin"};
static const size_t caps[5]={16384U,16384U,8192U,4096U,4096U};
static int span(const void *p,size_t n){return p&&n&&n-1U<=UINTPTR_MAX-(uintptr_t)p;}
static int overlap(const void *p,size_t n,const void *q,size_t m)
{uintptr_t a=(uintptr_t)p,b=(uintptr_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);}
static unsigned le16(const unsigned char *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static uint32_t le32(const unsigned char *p){return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static unsigned be16(const unsigned char *p){return ((unsigned)p[0]<<8)|p[1];}
static uint32_t be32(const unsigned char *p){return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
static void w16(unsigned char *p,unsigned n){p[0]=(unsigned char)n;p[1]=(unsigned char)(n>>8);}
static void w32(unsigned char *p,uint32_t n){unsigned i;for(i=0;i<4U;i++)p[i]=(unsigned char)(n>>(8U*i));}
static int zero(const unsigned char *p,size_t n){size_t i;for(i=0;i<n;i++)if(p[i])return 0;return 1;}
/* ZIP IEEE CRC32 only; never a cryptographic authentication primitive. */
static uint32_t crc32_zip(const unsigned char *p,size_t n)
{uint32_t c=0xffffffffU;size_t i;unsigned j;for(i=0;i<n;i++){c^=p[i];for(j=0;j<8U;j++)c=(c>>1)^((c&1U)?0xedb88320U:0U);}return c^0xffffffffU;}
static int text(const unsigned char *p,size_t n)
{
 size_t i=0;unsigned c,v,k,need,min;
 if(!n||n>128U)return 0;
 while(i<n){c=p[i++];if(c<128U){if(c<32U||c==127U)return 0;continue;}
 if(c>=194U&&c<=223U){v=c&31U;need=1;min=128;}
 else if(c>=224U&&c<=239U){v=c&15U;need=2;min=2048;}
 else if(c>=240U&&c<=244U){v=c&7U;need=3;min=65536;}else return 0;
 if(n-i<need)return 0;for(k=0;k<need;k++){c=p[i++];if((c&192U)!=128U)return 0;v=(v<<6)|(c&63U);}
 if(v<min||v>1114111U||(v>=55296U&&v<=57343U)||(v>=128U&&v<=159U))return 0;
 }return 1;
}
static int dns(const unsigned char *p,size_t n)
{size_t i;unsigned c,label=0;int nonnumeric=0;if(!n||n>253U)return 0;
 for(i=0;i<n;i++){c=p[i];if(c=='.'){if(!label||p[i-1]=='-')return 0;label=0;}
 else{if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='-')||label==63U||(!label&&c=='-'))return 0;
 label++;if(c<'0'||c>'9')nonnumeric=1;}}return label&&p[n-1]!='-'&&nonnumeric;}
static int metadata(MP_SLICE s,int issuance)
{
 const unsigned char *p=s.data,*v,*times[3]={NULL,NULL,NULL};size_t off=12,n;
 unsigned tag,count=issuance?5U:16U;
 if(s.size<12U||s.size>4096U||memcmp(p,issuance?"PACISS02":"PACCFG02",8)||be16(p+8)!=2U||be16(p+10)!=count)return 0;
 for(tag=1;tag<=count;tag++){
 if(s.size-off<6U||be16(p+off)!=tag)return 0;n=be32(p+off+2);off+=6;if(n>s.size-off)return 0;v=p+off;
 if(issuance){
 if((tag==1U||tag==2U)&&n!=32U)return 0;
 if(tag==3U&&(!n||n>20U||!v[0]))return 0;
 if(tag==4U&&!text(v,n))return 0;
 if(tag==5U&&n!=0U&&n!=32U)return 0;
 }else switch(tag){
 case 1:if(!dns(v,n))return 0;break;
 case 2:if(n!=2U||!be16(v))return 0;break;
 case 3:case 4:case 12:if(n!=32U)return 0;break;
 case 5:case 7:case 11:if(n!=32U||zero(v,n))return 0;break;
 case 6:case 8:if(n!=8U||zero(v,n))return 0;break;
 case 9:if(n!=8U||memcmp(v,"papacc/1",8))return 0;break;
 case 10:if(n!=2U||be16(v)!=1U)return 0;break;
 case 13:if(!text(v,n))return 0;break;
 default:if(n!=8U)return 0;times[tag-14U]=v;break;
 }
 off+=n;
 }return off==s.size&&(issuance||(memcmp(times[0],times[1],8)<=0&&memcmp(times[1],times[2],8)<0));
}
static int parts(const MP_PAYLOAD *p)
{unsigned i;for(i=0;i<5U;i++)if(!span(p->member[i].data,p->member[i].size)||p->member[i].size>caps[i])return 0;
 return metadata(p->member[3],0)&&metadata(p->member[4],1);}
MP_RESULT mp_payload_prepare(const MP_PAYLOAD *p,unsigned char *out,size_t cap,size_t *used,int stopped)
{
 size_t off=0,dir,start[5],n,total=502U;unsigned i;uint32_t crc[5];MP_PAYLOAD local;
 if(!span(p,sizeof(*p))||!span(out,cap)||!span(used,sizeof(*used))||overlap(p,sizeof(*p),out,cap)||overlap(p,sizeof(*p),used,sizeof(*used))||overlap(out,cap,used,sizeof(*used)))return MP_INVALID;
 for(i=0;i<5U;i++)if(!span(p->member[i].data,p->member[i].size)||overlap(p->member[i].data,p->member[i].size,out,cap)||overlap(p->member[i].data,p->member[i].size,used,sizeof(*used)))return MP_INVALID;
 *used=0;if(stopped)return MP_STOPPED;if(!parts(p))return MP_INVALID;
 for(i=0;i<5U;i++)total+=p->member[i].size;if(total>MP_ARCHIVE_MAX||cap<total)return MP_LIMIT;
 for(i=0;i<5U;i++){
 n=strlen(names[i]);start[i]=off;crc[i]=crc32_zip(p->member[i].data,p->member[i].size);memset(out+off,0,30U);w32(out+off,0x04034b50U);w16(out+off+4,20);w16(out+off+12,0x21);w32(out+off+14,crc[i]);w32(out+off+18,(uint32_t)p->member[i].size);w32(out+off+22,(uint32_t)p->member[i].size);w16(out+off+26,(unsigned)n);memcpy(out+off+30,names[i],n);off+=30U+n;memcpy(out+off,p->member[i].data,p->member[i].size);off+=p->member[i].size;
 }
 dir=off;for(i=0;i<5U;i++){
 n=strlen(names[i]);memset(out+off,0,46U);w32(out+off,0x02014b50U);w16(out+off+4,20);w16(out+off+6,20);w16(out+off+14,0x21);w32(out+off+16,crc[i]);w32(out+off+20,(uint32_t)p->member[i].size);w32(out+off+24,(uint32_t)p->member[i].size);w16(out+off+28,(unsigned)n);w32(out+off+42,(uint32_t)start[i]);memcpy(out+off+46,names[i],n);off+=46U+n;
 }
 memset(out+off,0,22U);w32(out+off,0x06054b50U);w16(out+off+8,5);w16(out+off+10,5);w32(out+off+12,(uint32_t)(off-dir));w32(out+off+16,(uint32_t)dir);off+=22U;
 if(off!=total||mp_payload_inspect(out,off,&local,0)!=MP_DATA_ONLY)return MP_INVALID;
 *used=off;return MP_DATA_ONLY;
}
MP_RESULT mp_payload_inspect(const unsigned char *p,size_t bytes,MP_PAYLOAD *out,int stopped)
{
 MP_PAYLOAD v;size_t off=0,start[5],n,len,dir;uint32_t crc[5];unsigned i;
 if(!span(p,bytes)||!span(out,sizeof(*out))||overlap(p,bytes,out,sizeof(*out)))return MP_INVALID;
 memset(out,0,sizeof(*out));if(stopped)return MP_STOPPED;if(bytes<502U||bytes>MP_ARCHIVE_MAX)return MP_LIMIT;
 for(i=0;i<5U;i++){
 n=strlen(names[i]);if(bytes-off<30U+n)return MP_INVALID;start[i]=off;
 if(le32(p+off)!=0x04034b50U||le16(p+off+4)!=20U||le16(p+off+6)||le16(p+off+8)||le16(p+off+10)||le16(p+off+12)!=0x21U||le16(p+off+26)!=n||le16(p+off+28)||memcmp(p+off+30,names[i],n))return MP_INVALID;
 crc[i]=le32(p+off+14);len=le32(p+off+18);if(!len||len>caps[i]||len!=le32(p+off+22))return MP_INVALID;off+=30U+n;if(len>bytes-off)return MP_INVALID;
 v.member[i].data=p+off;v.member[i].size=len;if(crc32_zip(p+off,len)!=crc[i])return MP_INVALID;off+=len;
 }
 dir=off;for(i=0;i<5U;i++){
 n=strlen(names[i]);if(bytes-off<46U+n)return MP_INVALID;
 if(le32(p+off)!=0x02014b50U||le16(p+off+4)!=20U||le16(p+off+6)!=20U||le16(p+off+8)||le16(p+off+10)||le16(p+off+12)||le16(p+off+14)!=0x21U||le32(p+off+16)!=crc[i]||le32(p+off+20)!=v.member[i].size||le32(p+off+24)!=v.member[i].size||le16(p+off+28)!=n||!zero(p+off+30,12)||le32(p+off+42)!=start[i]||memcmp(p+off+46,names[i],n))return MP_INVALID;
 off+=46U+n;
 }
 if(bytes-off!=22U||le32(p+off)!=0x06054b50U||le32(p+off+4)||le16(p+off+8)!=5U||le16(p+off+10)!=5U||le32(p+off+12)!=off-dir||le32(p+off+16)!=dir||le16(p+off+20)||!parts(&v))return MP_INVALID;
 *out=v;return MP_DATA_ONLY;
}
MP_RESULT mp_envelope_inspect(const unsigned char *p,size_t n,MP_ENVELOPE *out,int stopped)
{
 size_t s,c,off;if(!span(p,n)||!span(out,sizeof(*out))||overlap(p,n,out,sizeof(*out)))return MP_INVALID;
 memset(out,0,sizeof(*out));if(stopped)return MP_STOPPED;if(n<366U||n>MP_INPUT_MAX)return MP_LIMIT;
 if(memcmp(p,"PACCMP02",8)||be16(p+8)!=2U||be16(p+10)!=1U||be32(p+12)||be32(p+16)!=n||be32(p+24)!=600000U)return MP_INVALID;
 s=be32(p+88);c=be32(p+20);if(!s||s>1024U||!c||c>65536U||n!=364U+s+c)return MP_INVALID;
 off=92;out->spki.data=p+off;out->spki.size=s;off+=s;out->ciphertext.data=p+off;out->ciphertext.size=c;off+=c;out->tag.data=p+off;out->tag.size=16;off+=16;out->signature.data=p+off;out->signature.size=256;return MP_DATA_ONLY;
}
MP_RESULT mp_envelope_prepare(const unsigned char *header,const MP_ENVELOPE *e,unsigned char *out,size_t cap,size_t *used,int stopped)
{
 MP_SLICE v[4];MP_ENVELOPE check;size_t n,off;unsigned i;
 if(!span(header,92)||!span(e,sizeof(*e))||!span(out,cap)||!span(used,sizeof(*used))||overlap(header,92,out,cap)||overlap(header,92,used,sizeof(*used))||overlap(e,sizeof(*e),out,cap)||overlap(e,sizeof(*e),used,sizeof(*used))||overlap(out,cap,used,sizeof(*used)))return MP_INVALID;
 v[0]=e->spki;v[1]=e->ciphertext;v[2]=e->tag;v[3]=e->signature;
 for(i=0;i<4U;i++)if(!span(v[i].data,v[i].size)||overlap(v[i].data,v[i].size,out,cap)||overlap(v[i].data,v[i].size,used,sizeof(*used)))return MP_INVALID;
 *used=0;if(stopped)return MP_STOPPED;
 if(!e->spki.size||e->spki.size>1024U||!e->ciphertext.size||e->ciphertext.size>65536U||e->tag.size!=16U||e->signature.size!=256U)return MP_INVALID;
 n=364U+e->spki.size+e->ciphertext.size;if(cap<n)return MP_LIMIT;
 if(memcmp(header,"PACCMP02",8)||be16(header+8)!=2U||be16(header+10)!=1U||be32(header+12)||be32(header+16)!=n||be32(header+20)!=e->ciphertext.size||be32(header+24)!=600000U||be32(header+88)!=e->spki.size)return MP_INVALID;
 memcpy(out,header,92);off=92;for(i=0;i<4U;i++){memcpy(out+off,v[i].data,v[i].size);off+=v[i].size;}
 if(mp_envelope_inspect(out,n,&check,0)!=MP_DATA_ONLY)return MP_INVALID;*used=n;return MP_DATA_ONLY;
}
MP_RESULT mp_package_validate(const unsigned char *p,size_t n,int stopped)
{MP_ENVELOPE e;MP_RESULT r=mp_envelope_inspect(p,n,&e,stopped);return r==MP_DATA_ONLY?MP_CRYPTO_UNAVAILABLE:r;}
