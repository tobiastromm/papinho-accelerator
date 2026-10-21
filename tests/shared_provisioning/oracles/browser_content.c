/* Read-only M3 port from Accelerator manual_package.c, origin/hash in checkpoint.
 * Exact accepted format, no writer/producer/authority imported. */
#include "papinho_papacc_content.h"
#include <string.h>
#include <limits.h>
typedef char pc_flat_pointer[(sizeof(size_t)==sizeof(void *))?1:-1];
#if ULONG_MAX != 0xffffffffUL
#error Win32 unsigned long32 required
#endif
static const char *const pc_names[5]={"ca.der","cliente.der","cliente.pk8","config.bin","issuance.bin"};
static const size_t pc_caps[5]={16384U,16384U,8192U,4096U,4096U};
static int pc_span(const void *p,size_t n){return p&&n&&n-1U<=((size_t)-1)-(size_t)p;}
static int pc_overlap(const void *p,size_t n,const void *q,size_t m)
{size_t a=(size_t)p,b=(size_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);}
static unsigned pc_le16(const unsigned char *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static unsigned long pc_le32(const unsigned char *p){return (unsigned long)p[0]|((unsigned long)p[1]<<8)|((unsigned long)p[2]<<16)|((unsigned long)p[3]<<24);}
static unsigned pc_be16(const unsigned char *p){return ((unsigned)p[0]<<8)|p[1];}
static unsigned long pc_be32(const unsigned char *p){return ((unsigned long)p[0]<<24)|((unsigned long)p[1]<<16)|((unsigned long)p[2]<<8)|p[3];}
static int pc_zero(const unsigned char *p,size_t n){size_t i;for(i=0;i<n;i++)if(p[i])return 0;return 1;}
/* ZIP IEEE CRC32 only; never a cryptographic authentication primitive. */
static unsigned long pc_crc32_zip(const unsigned char *p,size_t n)
{unsigned long c=0xffffffffU;size_t i;unsigned j;for(i=0;i<n;i++){c^=p[i];for(j=0;j<8U;j++)c=(c>>1)^((c&1U)?0xedb88320U:0U);}return c^0xffffffffU;}
static int pc_text(const unsigned char *p,size_t n)
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
static int pc_dns(const unsigned char *p,size_t n)
{size_t i;unsigned c,label=0;int nonnumeric=0;if(!n||n>253U)return 0;
 for(i=0;i<n;i++){c=p[i];if(c=='.'){if(!label||p[i-1]=='-')return 0;label=0;}
 else{if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='-')||label==63U||(!label&&c=='-'))return 0;
 label++;if(c<'0'||c>'9')nonnumeric=1;}}return label&&p[n-1]!='-'&&nonnumeric;}
static int pc_metadata(papinho_papacc_slice s,int issuance)
{
 const unsigned char *p=s.data,*v,*times[3]={NULL,NULL,NULL};size_t off=12,n;
 unsigned tag,count=issuance?5U:16U;
 if(s.size<12U||s.size>4096U||memcmp(p,issuance?"PACISS02":"PACCFG02",8)||pc_be16(p+8)!=2U||pc_be16(p+10)!=count)return 0;
 for(tag=1;tag<=count;tag++){
 if(s.size-off<6U||pc_be16(p+off)!=tag)return 0;n=pc_be32(p+off+2);off+=6;if(n>s.size-off)return 0;v=p+off;
 if(issuance){
 if((tag==1U||tag==2U)&&n!=32U)return 0;
 if(tag==3U&&(!n||n>20U||!v[0]))return 0;
 if(tag==4U&&!pc_text(v,n))return 0;
 if(tag==5U&&n!=0U&&n!=32U)return 0;
 }else switch(tag){
 case 1:if(!pc_dns(v,n))return 0;break;
 case 2:if(n!=2U||!pc_be16(v))return 0;break;
 case 3:case 4:case 12:if(n!=32U)return 0;break;
 case 5:case 7:case 11:if(n!=32U||pc_zero(v,n))return 0;break;
 case 6:case 8:if(n!=8U||pc_zero(v,n))return 0;break;
 case 9:if(n!=8U||memcmp(v,"papacc/1",8))return 0;break;
 case 10:if(n!=2U||pc_be16(v)!=1U)return 0;break;
 case 13:if(!pc_text(v,n))return 0;break;
 default:if(n!=8U)return 0;times[tag-14U]=v;break;
 }
 off+=n;
 }return off==s.size&&(issuance||(memcmp(times[0],times[1],8)<=0&&memcmp(times[1],times[2],8)<0));
}
static int pc_parts(const papinho_papacc_content_view *p)
{unsigned i;for(i=0;i<5U;i++)if(!pc_span(p->member[i].data,p->member[i].size)||p->member[i].size>pc_caps[i])return 0;
 return pc_metadata(p->member[3],0)&&pc_metadata(p->member[4],1);}
int papinho_papacc_content_inspect(const unsigned char *p,size_t bytes,papinho_papacc_content_view *out,papinho_papacc_stopped stop,void *context)
{
 papinho_papacc_content_view v;size_t off=0,start[5],n,len,dir;unsigned long crc[5];unsigned i;
 if(!pc_span(p,bytes)||!pc_span(out,sizeof(*out))||pc_overlap(p,bytes,out,sizeof(*out)))return PAPINHO_PAPACC_CONTENT_INVALID;
 if(stop && stop(context))return PAPINHO_PAPACC_CONTENT_STOPPED;if(bytes<502U||bytes>49654UL)return PAPINHO_PAPACC_CONTENT_INVALID;
 for(i=0;i<5U;i++){
 if(stop && stop(context))return PAPINHO_PAPACC_CONTENT_STOPPED;
 n=strlen(pc_names[i]);if(bytes-off<30U+n)return PAPINHO_PAPACC_CONTENT_INVALID;start[i]=off;
 if(pc_le32(p+off)!=0x04034b50U||pc_le16(p+off+4)!=20U||pc_le16(p+off+6)||pc_le16(p+off+8)||pc_le16(p+off+10)||pc_le16(p+off+12)!=0x21U||pc_le16(p+off+26)!=n||pc_le16(p+off+28)||memcmp(p+off+30,pc_names[i],n))return PAPINHO_PAPACC_CONTENT_INVALID;
 crc[i]=pc_le32(p+off+14);len=pc_le32(p+off+18);if(!len||len>pc_caps[i]||len!=pc_le32(p+off+22))return PAPINHO_PAPACC_CONTENT_INVALID;off+=30U+n;if(len>bytes-off)return PAPINHO_PAPACC_CONTENT_INVALID;
 v.member[i].data=p+off;v.member[i].size=len;if(pc_crc32_zip(p+off,len)!=crc[i])return PAPINHO_PAPACC_CONTENT_INVALID;off+=len;
 }
 dir=off;for(i=0;i<5U;i++){
 if(stop && stop(context))return PAPINHO_PAPACC_CONTENT_STOPPED;
 n=strlen(pc_names[i]);if(bytes-off<46U+n)return PAPINHO_PAPACC_CONTENT_INVALID;
 if(pc_le32(p+off)!=0x02014b50U||pc_le16(p+off+4)!=20U||pc_le16(p+off+6)!=20U||pc_le16(p+off+8)||pc_le16(p+off+10)||pc_le16(p+off+12)||pc_le16(p+off+14)!=0x21U||pc_le32(p+off+16)!=crc[i]||pc_le32(p+off+20)!=v.member[i].size||pc_le32(p+off+24)!=v.member[i].size||pc_le16(p+off+28)!=n||!pc_zero(p+off+30,12)||pc_le32(p+off+42)!=start[i]||memcmp(p+off+46,pc_names[i],n))return PAPINHO_PAPACC_CONTENT_INVALID;
 off+=46U+n;
 }
 if(bytes-off!=22U||pc_le32(p+off)!=0x06054b50U||pc_le32(p+off+4)||pc_le16(p+off+8)!=5U||pc_le16(p+off+10)!=5U||pc_le32(p+off+12)!=off-dir||pc_le32(p+off+16)!=dir||pc_le16(p+off+20)||!pc_parts(&v))return PAPINHO_PAPACC_CONTENT_INVALID;
 if(stop && stop(context))return PAPINHO_PAPACC_CONTENT_STOPPED;
 *out=v;return PAPINHO_PAPACC_CONTENT_DATA_ONLY;
}

papinho_papacc_slice papinho_papacc_content_field(const papinho_papacc_content_view *v,int issuance,unsigned tag)
{
    papinho_papacc_slice empty,s;
    size_t off=12,n;unsigned t,count=issuance?5U:16U;
    empty.data=NULL;empty.size=0;
    if(!v || tag<1 || tag>count)return empty;
    s=v->member[issuance?4:3];
    if(!pc_span(s.data,s.size) || !pc_metadata(s,issuance))return empty;
    for(t=1;t<=count;++t){n=pc_be32(s.data+off+2);off+=6;
        if(t==tag){empty.data=s.data+off;empty.size=n;return empty;}off+=n;}
    return empty;
}

int browser_metadata_valid(const unsigned char *p,size_t n,unsigned kind){papinho_papacc_slice s;s.data=p;s.size=n;return kind<=1U&&pc_metadata(s,(int)kind);}
