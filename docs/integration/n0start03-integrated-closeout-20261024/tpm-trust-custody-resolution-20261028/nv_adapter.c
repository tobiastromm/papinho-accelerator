#include "nv_adapter.h"
#include <string.h>
static int owned(TP_OWNER *o){return o&&o->owner_thread==GetCurrentThreadId();}
static int usable(TP_OWNER *o){return owned(o)&&!o->frozen&&!o->closed&&!o->stop;}
static int bad(TP_OWNER *o,const char *why,unsigned long rc){if(owned(o))return tp_fail(o,why,rc);return 0;}
static int public_packet(const unsigned char *c,unsigned n){return c&&n==14&&tp_u16(c)==0x8001&&tp_u32(c+2)==14&&tp_u32(c+6)==0x169&&tp_u32(c+10)==Q4_NV_INDEX;}
unsigned long q4_tbs_public_only(Q4_TBS_SUBMIT api,void *original,const unsigned char *c,unsigned n,unsigned char *r,unsigned *rn){
 UINT32 count;TBS_RESULT rc;
 if(!api||!original||!r||!rn||*rn<10||*rn>TP_MAX_RESPONSE||!public_packet(c,n))return (TBS_RESULT)TBS_E_BAD_PARAMETER;
 count=*rn;rc=api((TBS_HCONTEXT)original,TBS_COMMAND_LOCALITY_ZERO,TBS_COMMAND_PRIORITY_NORMAL,c,n,r,&count);*rn=count;return rc;
}
static int profile(TP_OWNER *o,const unsigned char *r,unsigned n,int written,unsigned char area[14],unsigned char name[34]){
 unsigned char digest[32];const unsigned char *a;
 if(n!=62||tp_u16(r)!=0x8001||tp_u32(r+2)!=62||tp_u32(r+6)||tp_u16(r+10)!=14)return bad(o,"NV_PUBLIC_FRAME",0);
 a=r+12;
 if(tp_u32(a)!=Q4_NV_INDEX||tp_u16(a+4)!=11||tp_u32(a+6)!=(0x40014U|(written?0x20000000U:0))||tp_u16(a+10)||tp_u16(a+12)!=8||tp_u16(r+26)!=34||tp_u16(r+28)!=11)return bad(o,"NV_PUBLIC_PROFILE",0);
 if(!tp_hash(o,a,14,digest))return 0;
 if(!tp_eq(digest,r+30,32))return bad(o,"NV_PUBLIC_NAME",0);
 memcpy(area,a,14);memcpy(name,r+28,34);return 1;
}
int q4_nv_public(TP_OWNER *o,int written,unsigned char area[14],unsigned char name[34]){
 unsigned char c[14],r[TP_MAX_RESPONSE],a[14],nm[34];unsigned n=sizeof(r);unsigned long rc;void *original;int ok=0;
 if(!usable(o)||!area||!name||(written!=0&&written!=1))return 0;
 if(InterlockedCompareExchange(&o->busy,1,0))return bad(o,"NV_REENTRANT",0);
 original=o->context;
 if(!original||!o->transport.send||!o->boundary){bad(o,"NV_OWNER_DEPENDENCIES",0);goto done;}
 if(!o->boundary(o,"NV_PUBLIC_BEFORE")||!usable(o)||o->context!=original){bad(o,"NV_BOUNDARY",0);goto done;}
 tp_w16(c,0x8001);tp_w32(c+2,14);tp_w32(c+6,0x169);tp_w32(c+10,Q4_NV_INDEX);
 if(o->calls>=12){bad(o,"NV_CALL_LIMIT",0);goto done;}o->calls++;
 rc=o->transport.send(o->transport.user,original,c,sizeof(c),r,&n);
 if(rc){bad(o,"NV_TBS",rc);goto done;}
 if(n<10||n>sizeof(r)||tp_u32(r+2)!=n){bad(o,"NV_RESPONSE_LENGTH",0);goto done;}
 if(tp_u32(r+6)){bad(o,"NV_TPM_RC",tp_u32(r+6));goto done;}
 if(!profile(o,r,n,written,a,nm))goto done;
 if(!o->boundary(o,"NV_PUBLIC_AFTER")||!usable(o)||o->context!=original){bad(o,"NV_BOUNDARY",0);goto done;}
 memcpy(area,a,14);memcpy(name,nm,34);ok=1;
 done:SecureZeroMemory(r,sizeof(r));InterlockedExchange(&o->busy,0);return ok;
}
static int mac_auth(TP_OWNER *o,const unsigned char key[32],const unsigned char auth[32],const unsigned char digest[32],const unsigned char newer[32],const unsigned char older[32],unsigned char mac[32]){
 unsigned char k[64],msg[97];unsigned n=32;int ok;
 while(n&&auth[n-1]==0)n--;memcpy(k,key,32);memcpy(k+32,auth,n);memcpy(msg,digest,32);memcpy(msg+32,newer,32);memcpy(msg+64,older,32);msg[96]=0;
 ok=tp_mac(o,k,32+n,msg,sizeof(msg),mac);SecureZeroMemory(k,sizeof(k));SecureZeroMemory(msg,sizeof(msg));return ok;
}
int q4_nv_read_packet(TP_OWNER *o,unsigned session,const unsigned char key[32],const unsigned char auth[32],const unsigned char name[34],const unsigned char caller[32],const unsigned char previous[32],unsigned char out[99]){
 unsigned char cp[76],digest[32],mac[32],packet[99];int ok=0;
 if(!usable(o)||!key||!auth||!name||!caller||!previous||!out||(session&0xff000000U)!=0x02000000U||tp_u16(name)!=11)return 0;
 if(InterlockedCompareExchange(&o->busy,1,0))return bad(o,"NV_REENTRANT",0);
 tp_w32(cp,0x14e);memcpy(cp+4,name,34);memcpy(cp+38,name,34);tp_w16(cp+72,8);tp_w16(cp+74,0);
 if(!tp_hash(o,cp,sizeof(cp),digest)||!mac_auth(o,key,auth,digest,caller,previous,mac))goto done;
 tp_w16(packet,0x8002);tp_w32(packet+2,99);tp_w32(packet+6,0x14e);tp_w32(packet+10,Q4_NV_INDEX);tp_w32(packet+14,Q4_NV_INDEX);tp_w32(packet+18,73);tp_w32(packet+22,session);tp_w16(packet+26,32);memcpy(packet+28,caller,32);packet[60]=0;tp_w16(packet+61,32);memcpy(packet+63,mac,32);tp_w16(packet+95,8);tp_w16(packet+97,0);memcpy(out,packet,99);ok=1;
 done:SecureZeroMemory(mac,sizeof(mac));SecureZeroMemory(packet,sizeof(packet));InterlockedExchange(&o->busy,0);return ok;
}
int q4_nv_read_result(TP_OWNER *o,const unsigned char *p,unsigned n,const unsigned char key[32],const unsigned char auth[32],const unsigned char caller[32],ULONGLONG *value){
 unsigned char rp[18],digest[32],mac[32];ULONGLONG v=0;unsigned i;int ok=0;
 if(!usable(o)||!p||!key||!auth||!caller||!value)return 0;
 if(InterlockedCompareExchange(&o->busy,1,0))return bad(o,"NV_REENTRANT",0);
 if(n==10&&tp_u16(p)==0x8001&&tp_u32(p+2)==10&&tp_u32(p+6)){bad(o,"NV_READ_TPM_RC",tp_u32(p+6));goto done;}
 if(n!=93||tp_u16(p)!=0x8002||tp_u32(p+2)!=93||tp_u32(p+6)||tp_u32(p+10)!=10||tp_u16(p+14)!=8||tp_u16(p+24)!=32||p[58]!=0||tp_u16(p+59)!=32){bad(o,"NV_READ_FRAME",0);goto done;}
 tp_w32(rp,0);tp_w32(rp+4,0x14e);memcpy(rp+8,p+14,10);
 if(!tp_hash(o,rp,sizeof(rp),digest)||!mac_auth(o,key,auth,digest,p+26,caller,mac))goto done;
 if(!tp_eq(mac,p+61,32)){bad(o,"NV_READ_HMAC",0);goto done;}
 for(i=0;i<8;i++)v=(v<<8)|p[16+i];*value=v;ok=1;
 done:SecureZeroMemory(mac,sizeof(mac));InterlockedExchange(&o->busy,0);return ok;
}
