/* Read-only extraction of existing Browser port of Accelerator M2/M3.
 * Provenance/transform mapping frozen with this task. Not protocol PACC. */
#include "structure.h"
#include <string.h>
static const char *const sp_local_names[5]={"ca.der","cliente.der","cliente.pk8","config.bin","issuance.bin"};
static const size_t sp_local_caps[5]={16384U,16384U,8192U,4096U,4096U};
static int sp_local_span(const void *p,size_t n){return p&&n&&n-1U<=((size_t)-1)-(size_t)p;}
static int sp_local_overlap(const void *p,size_t n,const void *q,size_t m)
{size_t a=(size_t)p,b=(size_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);}
static unsigned sp_local_le16(const unsigned char *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static SP_U32 sp_local_le32(const unsigned char *p){return (SP_U32)p[0]|((SP_U32)p[1]<<8)|((SP_U32)p[2]<<16)|((SP_U32)p[3]<<24);}
static unsigned sp_local_be16(const unsigned char *p){return ((unsigned)p[0]<<8)|p[1];}
static SP_U32 sp_local_be32(const unsigned char *p){return ((SP_U32)p[0]<<24)|((SP_U32)p[1]<<16)|((SP_U32)p[2]<<8)|p[3];}
static int sp_local_zero(const unsigned char *p,size_t n){size_t i;for(i=0;i<n;i++)if(p[i])return 0;return 1;}
/* ZIP IEEE CRC32 only; never a cryptographic authentication primitive. */
static SP_U32 sp_local_crc32_zip(const unsigned char *p,size_t n)
{SP_U32 c=0xffffffffU;size_t i;unsigned j;for(i=0;i<n;i++){c^=p[i];for(j=0;j<8U;j++)c=(c>>1)^((c&1U)?0xedb88320U:0U);}return c^0xffffffffU;}
static int sp_local_text(const unsigned char *p,size_t n)
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
static int sp_local_dns(const unsigned char *p,size_t n)
{size_t i;unsigned c,label=0;int nonnumeric=0;if(!n||n>253U)return 0;
 for(i=0;i<n;i++){c=p[i];if(c=='.'){if(!label||p[i-1]=='-')return 0;label=0;}
 else{if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='-')||label==63U||(!label&&c=='-'))return 0;
 label++;if(c<'0'||c>'9')nonnumeric=1;}}return label&&p[n-1]!='-'&&nonnumeric;}
static int sp_local_metadata(SP_SLICE s,int issuance)
{
 const unsigned char *p=s.data,*v,*times[3]={NULL,NULL,NULL};size_t off=12,n;
 unsigned tag,count=issuance?5U:16U;
 if(s.size<12U||s.size>4096U||memcmp(p,issuance?"PACISS02":"PACCFG02",8)||sp_local_be16(p+8)!=2U||sp_local_be16(p+10)!=count)return 0;
 for(tag=1;tag<=count;tag++){
 if(s.size-off<6U||sp_local_be16(p+off)!=tag)return 0;n=sp_local_be32(p+off+2);off+=6;if(n>s.size-off)return 0;v=p+off;
 if(issuance){
 if((tag==1U||tag==2U)&&n!=32U)return 0;
 if(tag==3U&&(!n||n>20U||!v[0]))return 0;
 if(tag==4U&&!sp_local_text(v,n))return 0;
 if(tag==5U&&n!=0U&&n!=32U)return 0;
 }else switch(tag){
 case 1:if(!sp_local_dns(v,n))return 0;break;
 case 2:if(n!=2U||!sp_local_be16(v))return 0;break;
 case 3:case 4:case 12:if(n!=32U)return 0;break;
 case 5:case 7:case 11:if(n!=32U||sp_local_zero(v,n))return 0;break;
 case 6:case 8:if(n!=8U||sp_local_zero(v,n))return 0;break;
 case 9:if(n!=8U||memcmp(v,"papacc/1",8))return 0;break;
 case 10:if(n!=2U||sp_local_be16(v)!=1U)return 0;break;
 case 13:if(!sp_local_text(v,n))return 0;break;
 default:if(n!=8U)return 0;times[tag-14U]=v;break;
 }
 off+=n;
 }return off==s.size&&(issuance||(memcmp(times[0],times[1],8)<=0&&memcmp(times[1],times[2],8)<0));
}
static int sp_local_parts(const SP_PAYLOAD *p)
{unsigned i;for(i=0;i<5U;i++)if(!sp_local_span(p->member[i].data,p->member[i].size)||p->member[i].size>sp_local_caps[i])return 0;
 return sp_local_metadata(p->member[3],0)&&sp_local_metadata(p->member[4],1);}
SP_RESULT sp_payload_inspect(const unsigned char *p,size_t bytes,SP_PAYLOAD *out,SP_STOP stop,void *context)
{
 SP_PAYLOAD v;size_t off=0,start[5],n,len,dir;SP_U32 crc[5];unsigned i;
 if(!sp_local_span(p,bytes)||!sp_local_span(out,sizeof(*out))||sp_local_overlap(p,bytes,out,sizeof(*out)))return SP_INVALID;
 if(stop && stop(context))return SP_STOPPED;if(bytes<502U||bytes>49654UL)return SP_LIMIT;
 for(i=0;i<5U;i++){
 if(stop && stop(context))return SP_STOPPED;
 n=strlen(sp_local_names[i]);if(bytes-off<30U+n)return SP_INVALID;start[i]=off;
 if(sp_local_le32(p+off)!=0x04034b50U||sp_local_le16(p+off+4)!=20U||sp_local_le16(p+off+6)||sp_local_le16(p+off+8)||sp_local_le16(p+off+10)||sp_local_le16(p+off+12)!=0x21U||sp_local_le16(p+off+26)!=n||sp_local_le16(p+off+28)||memcmp(p+off+30,sp_local_names[i],n))return SP_INVALID;
 crc[i]=sp_local_le32(p+off+14);len=sp_local_le32(p+off+18);if(!len||len>sp_local_caps[i]||len!=sp_local_le32(p+off+22))return SP_INVALID;off+=30U+n;if(len>bytes-off)return SP_INVALID;
 v.member[i].data=p+off;v.member[i].size=len;if(sp_local_crc32_zip(p+off,len)!=crc[i])return SP_INVALID;off+=len;
 }
 dir=off;for(i=0;i<5U;i++){
 if(stop && stop(context))return SP_STOPPED;
 n=strlen(sp_local_names[i]);if(bytes-off<46U+n)return SP_INVALID;
 if(sp_local_le32(p+off)!=0x02014b50U||sp_local_le16(p+off+4)!=20U||sp_local_le16(p+off+6)!=20U||sp_local_le16(p+off+8)||sp_local_le16(p+off+10)||sp_local_le16(p+off+12)||sp_local_le16(p+off+14)!=0x21U||sp_local_le32(p+off+16)!=crc[i]||sp_local_le32(p+off+20)!=v.member[i].size||sp_local_le32(p+off+24)!=v.member[i].size||sp_local_le16(p+off+28)!=n||!sp_local_zero(p+off+30,12)||sp_local_le32(p+off+42)!=start[i]||memcmp(p+off+46,sp_local_names[i],n))return SP_INVALID;
 off+=46U+n;
 }
 if(bytes-off!=22U||sp_local_le32(p+off)!=0x06054b50U||sp_local_le32(p+off+4)||sp_local_le16(p+off+8)!=5U||sp_local_le16(p+off+10)!=5U||sp_local_le32(p+off+12)!=off-dir||sp_local_le32(p+off+16)!=dir||sp_local_le16(p+off+20)||!sp_local_parts(&v))return SP_INVALID;
 if(stop && stop(context))return SP_STOPPED;
 *out=v;return SP_DATA_ONLY;
}

SP_SLICE sp_payload_field(const SP_PAYLOAD *v,unsigned issuance,unsigned tag)
{
    SP_SLICE empty,s;
    size_t off=12,n;unsigned t,count=issuance?5U:16U;
    empty.data=NULL;empty.size=0;
    if(!v || issuance>1U || tag<1 || tag>count)return empty;
    s=v->member[issuance?4:3];
    if(!sp_local_span(s.data,s.size) || !sp_local_metadata(s,issuance))return empty;
    for(t=1;t<=count;++t){n=sp_local_be32(s.data+off+2);off+=6;
        if(t==tag){empty.data=s.data+off;empty.size=n;return empty;}off+=n;}
    return empty;
}

SP_RESULT sp_metadata_validate(SP_SLICE s,unsigned kind)
{
 if(kind>1U || !sp_local_span(s.data,s.size))return SP_INVALID;
 return sp_local_metadata(s,(int)kind)?SP_DATA_ONLY:SP_INVALID;
}
SP_RESULT sp_envelope_inspect(const unsigned char *p,size_t n,SP_ENVELOPE *out,SP_STOP stop,void *context)
{
 size_t s,c,off;SP_ENVELOPE v;
 if(!sp_local_span(p,n)||!out||sp_local_overlap(p,n,out,sizeof(*out)))return SP_INVALID;
 if(stop && stop(context))return SP_STOPPED;
 if(n<366U||n>SP_INPUT_MAX)return SP_LIMIT;
 if(memcmp(p,"PACCMP02",8)||sp_local_be16(p+8)!=2U||sp_local_be16(p+10)!=1U||sp_local_be32(p+12)||sp_local_be32(p+16)!=n||sp_local_be32(p+24)!=600000U)return SP_INVALID;
 s=sp_local_be32(p+88);c=sp_local_be32(p+20);if(!s||s>1024U||!c||c>65536U||n!=364U+s+c)return SP_INVALID;
 off=92;v.spki.data=p+off;v.spki.size=s;off+=s;v.ciphertext.data=p+off;v.ciphertext.size=c;off+=c;v.tag.data=p+off;v.tag.size=16;off+=16;v.signature.data=p+off;v.signature.size=256;
 if(stop && stop(context))return SP_STOPPED;*out=v;return SP_DATA_ONLY;
}
static void sp_local_wipe(void *value,size_t n)
{
 volatile unsigned char *p=(volatile unsigned char *)value;
 while(n!=0U){*p++=0U;--n;}
}
SP_RESULT sp_password_normalize(const unsigned char *input,size_t n,
    unsigned char out[32])
{
    unsigned char normalized[32];
    size_t i,count=0;
    SP_RESULT result=SP_INVALID;
    if(!out || sp_local_overlap(input,n,out,32))return result;
    sp_local_wipe(out,32);memset(normalized,0,sizeof(normalized));
    if(!input || n>128)goto done;
    for(i=0;i<n;++i) {
        unsigned char v=input[i];
        if(v==' ' || v=='-')continue;
        if(v>='a' && v<='z')v=(unsigned char)(v-'a'+'A');
        if(!((v>='A' && v<='Z') || (v>='2' && v<='7')) || count>=32)goto done;
        normalized[count++]=v;
    }
    if(count!=32)goto done;
    memcpy(out,normalized,32);result=SP_DATA_ONLY;
done:
    sp_local_wipe(normalized,sizeof(normalized));return result;
}
