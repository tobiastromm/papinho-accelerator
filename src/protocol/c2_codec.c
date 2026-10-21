/* Internal portable C2 grammar, frozen ADR-0034 encoding1; no implicit grants. */
#include "c2_codec.h"
#include <string.h>
int papacc_c2_compare(const void *,const void *,size_t);
static unsigned u16(const PAPACC_U8*p){return ((unsigned)p[0]<<8)|p[1];}
static PAPACC_U32 u32(const PAPACC_U8*p){return ((PAPACC_U32)p[0]<<24)|((PAPACC_U32)p[1]<<16)|((PAPACC_U32)p[2]<<8)|p[3];}
static int nz(const PAPACC_U8*p,PAPACC_SIZE n){unsigned v=0;while(n--!=0U)v|=*p++;return v!=0U;}
static int zeros(const PAPACC_U8*p,PAPACC_SIZE n){return !nz(p,n);}
static int ids(const PAPACC_U8*p,unsigned n){unsigned i;for(i=0;i<n;++i)if(!nz(p+8U*i,8U))return 0;return 1;}
static int prefix_can_add(const PAPACC_U8*p,PAPACC_SIZE amount){int i;PAPACC_SIZE carry=amount;for(i=7;i>=0;--i){carry+=p[i];carry>>=8;}return carry==0U;}
static int role(unsigned r){return r==1U||r==2U;}
static PAPACC_RESULT bounds(PAPACC_U16 t,PAPACC_SIZE n){
 static const PAPACC_SIZE minlen[12]={42,108,68,28,32,32,29,32,32,32,40,76};
 static const PAPACC_SIZE maxlen[12]={803,108,68,28,32,287,16384,32,32,32,40,76};
 if(t<16U||t>27U)return PAPACC_RESULT_PROTOCOL_ERROR;
 return n>=minlen[t-16U]&&n<=maxlen[t-16U]?PAPACC_RESULT_OK:PAPACC_RESULT_PROTOCOL_ERROR;
}
static PAPACC_RESULT envelope(const PAPACC_U8*p,PAPACC_SIZE*n,PAPACC_U16*t){
 if(memcmp(p,"PACC",4)||p[4]!=1U||p[5]!=0U||u16(p+6)!=16U||u16(p+10)!=0U)return PAPACC_RESULT_PROTOCOL_ERROR;
 *t=(PAPACC_U16)u16(p+8);*n=(PAPACC_SIZE)u32(p+12);return bounds(*t,*n);
}
static PAPACC_RESULT context(PAPACC_U16 t,PAPACC_C2_CHANNEL channel,PAPACC_C2_SENDER sender){
 if(channel!=PAPACC_C2_CONTROL&&channel!=PAPACC_C2_ROLE_CHANNEL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(sender!=PAPACC_C2_BROWSER&&sender!=PAPACC_C2_ACCELERATOR)return PAPACC_RESULT_INVALID_ARGUMENT;
 if((t==16U||t==17U||t==20U||t==21U||t==26U||t==27U)!=(channel==PAPACC_C2_CONTROL))return PAPACC_RESULT_PROTOCOL_ERROR;
 if((t==16U||t==18U||t==20U||t==25U)&&sender!=PAPACC_C2_BROWSER)return PAPACC_RESULT_PROTOCOL_ERROR;
 if((t==17U||t==19U||t==21U||t==27U)&&sender!=PAPACC_C2_ACCELERATOR)return PAPACC_RESULT_PROTOCOL_ERROR;
 return PAPACC_RESULT_OK;
}
static int dns(const PAPACC_U8*p,PAPACC_SIZE a,PAPACC_SIZE c){
 PAPACC_SIZE i,j,label=0U;unsigned ch;int numeric=1,dot=0;
 if(a<1U||a>254U||c<1U||c>253U||!(a==c||(a==c+1U&&p[a-1U]=='.')))return 0;
 for(i=0U;i<c;++i){
  ch=p[i];if(ch>='A'&&ch<='Z')ch+='a'-'A';if(ch!=p[a+i])return 0;
  if(ch=='.'){if(!label||label>63U||p[a+i-1U]=='-')return 0;label=0;dot=1;}
  else {if(!((ch>='a'&&ch<='z')||(ch>='0'&&ch<='9')||ch=='-'))return 0;if(!label&&ch=='-')return 0;++label;}
  if(ch!='.'&&!(ch>='0'&&ch<='9'))numeric=0;
 }
 if(!label||label>63U||p[a+c-1U]=='-'||(numeric&&dot))return 0;
 /* Original final byte is either corresponding ASCII or the one trailing dot. */
 for(j=0U;j<a;++j)if(p[j]<33U||p[j]>126U)return 0;
 return 1;
}
static int alpn(const PAPACC_U8*p,PAPACC_SIZE n,unsigned count){
 PAPACC_SIZE i=0U,start[8],size[8],k;unsigned c,j;
 if(count>8U||n>256U||((count==0U)!=(n==0U)))return 0;
 for(c=0U;c<count;++c){
  if(i>=n||p[i]==0U)return 0;k=p[i++];if(k>n-i)return 0;
  start[c]=i;size[c]=k;
  for(j=0U;j<c;++j)if(size[j]==k&&memcmp(p+start[j],p+i,k)==0)return 0;
  i+=k;
 }
 return i==n;
}
PAPACC_RESULT papacc_c2_validate_payload(PAPACC_U16 t,const PAPACC_U8*p,PAPACC_SIZE n,PAPACC_C2_CHANNEL channel,PAPACC_C2_SENDER sender){
 unsigned a,c,al,status,kind,fact,action,outcome,tls,owned,dir;
 int ok=0;
 if(p==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(bounds(t,n)!=PAPACC_RESULT_OK)return PAPACC_RESULT_PROTOCOL_ERROR;
 if(context(t,channel,sender)!=PAPACC_RESULT_OK)return context(t,channel,sender);
 switch(t){
 case 16U:
  a=u16(p+24);c=u16(p+26);al=u16(p+34);
  ok=ids(p,1)&&nz(p+8,4)&&nz(p+12,4)&&p[16]==1U&&p[17]==1U&&u16(p+18)!=0U&&zeros(p+36,4)&&p[32]<=1U;
  if(!ok||a>254U||c>253U||al>256U||n!=40U+(PAPACC_SIZE)a+c+al)return PAPACC_RESULT_PROTOCOL_ERROR;
  ok=dns(p+40,a,c)&&alpn(p+40+a+c,al,p[33])&&(!p[32]||p[33]);break;
 case 17U:
  status=u16(p+40);ok=ids(p,1)&&nz(p+32,4)&&nz(p+36,4)&&status<=5U&&role(p[42])&&p[43]==0U;
  if(status==0U)ok=ok&&ids(p+8,3)&&nz(p+44,32)&&nz(p+76,32)&&papacc_c2_compare(p+44,p+76,32)!=0;
  else ok=ok&&zeros(p+8,24)&&zeros(p+44,64);break;
 case 18U:ok=ids(p,3)&&nz(p+24,4)&&nz(p+28,4)&&role(p[32])&&zeros(p+33,3)&&nz(p+36,32);break;
 case 19U:ok=ids(p,3)&&role(p[24])&&p[25]==0U&&zeros(p+26,2);break;
 case 20U:ok=ids(p,3)&&nz(p+24,4)&&nz(p+28,4);break;
 case 21U:al=u16(p+28);ok=ids(p,3)&&u16(p+24)==0x0304U&&u16(p+26)==7U&&zeros(p+30,2)&&al<=255U&&n==32U+al;break;
 case 22U:ok=ids(p,2)&&role(p[16])&&p[17]==(PAPACC_U8)sender&&zeros(p+18,2)&&prefix_can_add(p+20,n-28U);break;
 case 23U:ok=ids(p,2)&&role(p[16])&&p[17]==(PAPACC_U8)sender&&zeros(p+18,2)&&u16(p+28)==1U&&zeros(p+30,2);break;
 case 24U:ok=ids(p,2)&&role(p[16])&&p[17]==(PAPACC_U8)(1U-(unsigned)sender)&&zeros(p+18,2)&&u16(p+28)==0U&&zeros(p+30,2);break;
 case 25U:kind=u16(p+28);dir=p[17];ok=ids(p,2)&&p[16]==2U&&zeros(p+18,2)&&zeros(p+30,2)&&kind>=1U&&kind<=4U&&dir==(kind==4U?1U:0U);break;
 case 26U:
  kind=u16(p);fact=u16(p+2);action=u16(p+4);
  ok=(kind==1U||kind==2U)&&fact>=1U&&fact<=3U&&action==1U&&zeros(p+6,2);
  if(kind==1U)ok=ok&&nz(p+8,8)&&zeros(p+16,24);
  else ok=ok&&zeros(p+8,8)&&ids(p+16,3);
  if(fact==2U&&sender!=PAPACC_C2_ACCELERATOR)ok=0;break;
 case 27U:
  fact=u16(p+32);action=u16(p+34);outcome=u16(p+36);tls=u16(p+38);owned=u16(p+40);
  ok=ids(p,4)&&fact<=14U&&tls<=4U&&owned<=3U&&(p[42]&0xF0U)==0U&&role(p[43]);
  if(outcome==1U)ok=ok&&fact==0U&&action==3U&&tls==2U&&owned==1U&&p[42]==15U;
  else if(outcome==2U)ok=ok&&fact>=1U&&action==1U&&owned<=1U;
  else if(outcome==3U)ok=ok&&fact>=1U&&action==2U&&owned>=2U;
  else ok=0;break;
 default:break;
 }
 return ok?PAPACC_RESULT_OK:PAPACC_RESULT_PROTOCOL_ERROR;
}
PAPACC_RESULT papacc_c2_encode_frame(PAPACC_U16 t,const PAPACC_U8*p,PAPACC_SIZE n,PAPACC_C2_CHANNEL c,PAPACC_C2_SENDER s,PAPACC_U8*out,PAPACC_SIZE cap,PAPACC_SIZE*written){
 PAPACC_RESULT result;
 if(written!=NULL)*written=0;
 if(out==NULL||written==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 result=papacc_c2_validate_payload(t,p,n,c,s);if(result!=0)return result;
 if(cap<16U||n>cap-16U)return PAPACC_RESULT_LIMIT_EXCEEDED;
 memmove(out+16,p,n);memcpy(out,"PACC",4);out[4]=1;out[5]=0;out[6]=0;out[7]=16;out[8]=(PAPACC_U8)(t>>8);out[9]=(PAPACC_U8)t;out[10]=out[11]=0;
 out[12]=(PAPACC_U8)(n>>24);out[13]=(PAPACC_U8)(n>>16);out[14]=(PAPACC_U8)(n>>8);out[15]=(PAPACC_U8)n;*written=16U+n;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_decode_frame(const PAPACC_U8*p,PAPACC_SIZE n,PAPACC_C2_CHANNEL c,PAPACC_C2_SENDER s,PAPACC_C2_VIEW*out){
 PAPACC_SIZE size;PAPACC_U16 type;PAPACC_RESULT result;
 if(out==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 out->type=0;out->payload=NULL;out->length=0;
 if(p==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(n<16U)return PAPACC_RESULT_PROTOCOL_ERROR;
 result=envelope(p,&size,&type);if(result!=0)return result;
 if(size!=n-16U)return PAPACC_RESULT_PROTOCOL_ERROR;
 result=papacc_c2_validate_payload(type,p+16,size,c,s);if(result!=0)return result;
 out->type=type;out->payload=p+16;out->length=size;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_reader_init(PAPACC_C2_READER*r,PAPACC_U8*p,PAPACC_SIZE n,PAPACC_C2_CHANNEL c,PAPACC_C2_SENDER s){
 if(r==NULL||p==NULL||n<16U||(c!=PAPACC_C2_CONTROL&&c!=PAPACC_C2_ROLE_CHANNEL)||(s!=PAPACC_C2_BROWSER&&s!=PAPACC_C2_ACCELERATOR))return PAPACC_RESULT_INVALID_ARGUMENT;
 r->storage=p;r->capacity=n;r->used=0;r->expected=16;r->channel=c;r->sender=s;r->failed=PAPACC_FALSE;r->complete=PAPACC_FALSE;return PAPACC_RESULT_OK;
}
void papacc_c2_reader_reset(PAPACC_C2_READER*r){
 volatile PAPACC_U8 *p;PAPACC_SIZE n;
 if(r==NULL)return;
 /* Caller has ended all view borrows. Erase owned token-bearing input even
  * when the parser is terminal; erasure never restores stream authority. */
 p=r->storage;n=r->used;while(n--!=0U)*p++=0;
 if(r->failed!=PAPACC_TRUE){r->used=0;r->expected=16;r->complete=PAPACC_FALSE;}
}
PAPACC_RESULT papacc_c2_reader_feed(PAPACC_C2_READER*r,const PAPACC_U8*p,PAPACC_SIZE n,PAPACC_SIZE*consumed,PAPACC_BOOL*complete,PAPACC_C2_VIEW*out){
 PAPACC_SIZE take,size;PAPACC_U16 type;PAPACC_RESULT result;
 if(consumed!=NULL)*consumed=0;if(complete!=NULL)*complete=PAPACC_FALSE;
 if(r==NULL||consumed==NULL||complete==NULL||out==NULL||(n!=0U&&p==NULL))return PAPACC_RESULT_INVALID_ARGUMENT;
 out->type=0;out->payload=NULL;out->length=0;
 if(r->failed||r->complete||r->storage==NULL)return PAPACC_RESULT_INVALID_STATE;
 while(*consumed<n){
  take=r->expected-r->used;if(take>n-*consumed)take=n-*consumed;
  memcpy(r->storage+r->used,p+*consumed,take);r->used+=take;*consumed+=take;
  if(r->used==16U&&r->expected==16U){
   result=envelope(r->storage,&size,&type);
   if(result==0)result=context(type,r->channel,r->sender);
   if(result!=0||size>r->capacity-16U){r->failed=PAPACC_TRUE;return result!=0?result:PAPACC_RESULT_LIMIT_EXCEEDED;}
   r->expected=16U+size;
  }
  if(r->used==r->expected){
   result=papacc_c2_decode_frame(r->storage,r->used,r->channel,r->sender,out);
   if(result!=0){r->failed=PAPACC_TRUE;return result;}
   r->complete=PAPACC_TRUE;*complete=PAPACC_TRUE;return PAPACC_RESULT_OK;
  }
 }
 return PAPACC_RESULT_OK;
}
