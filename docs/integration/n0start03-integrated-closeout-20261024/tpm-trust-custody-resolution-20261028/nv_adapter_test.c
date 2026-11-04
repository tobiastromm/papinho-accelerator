#include "nv_adapter.h"
#include <stdio.h>
#include <string.h>
static TP_OWNER owner,uncertain;
static unsigned checks,submits,boundaries,mode;static unsigned char vectors[316];
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%u checks=%u\n",(unsigned)__LINE__,checks);return 2;}}while(0)
static int boundary(TP_OWNER *o,const char *stage){(void)stage;boundaries++;if(mode==1){o->stop=1;return 0;}if(mode==2&&boundaries==2)return 0;return 1;}
static unsigned long send_mock(void *u,void *c,const unsigned char *p,unsigned n,unsigned char *r,unsigned *rn){(void)u;if(c!=(void *)1||n!=14||tp_u32(p+6)!=0x169)return 99;submits++;if(mode==3)return 0x80284001;if(mode==4){memset(r,0,10);tp_w16(r,0x8001);tp_w32(r+2,10);tp_w32(r+6,0x922);*rn=10;return 0;}memcpy(r,mode==6?vectors+254:vectors,62);*rn=mode==5?9:62;return 0;}
static void reset(void){owner.frozen=owner.stop=owner.retained=0;owner.first=owner.first_stage="NONE";owner.first_native=0;owner.calls=0;owner.context=(void *)1;owner.transport.send=send_mock;owner.boundary=boundary;submits=boundaries=mode=0;}
static TBS_RESULT WINAPI api_mock(TBS_HCONTEXT c,TBS_COMMAND_LOCALITY l,TBS_COMMAND_PRIORITY pr,PCBYTE p,UINT32 n,PBYTE r,PUINT32 rn){(void)c;(void)l;(void)pr;(void)p;(void)n;submits++;memcpy(r,vectors,62);*rn=62;return TBS_SUCCESS;}
static DWORD WINAPI foreign(LPVOID u){unsigned char a[14],n[34];(void)u;return q4_nv_public(&owner,0,a,n)?99:0;}
static unsigned long close_unknown(void *u,void *c){(void)u;(void)c;return 0x80284001;}
int main(int argc,char **argv){FILE *f=NULL;unsigned char a[14],nm[34],cmd[99],key[32],auth[32],caller[32],prior[32],name[34],bad[93],pubcmd[14],reply[4096];unsigned i,b,n;ULONGLONG value;HANDLE t;DWORD exitcode;const char *first;
 C(argc==2);C(fopen_s(&f,argv[1],"rb")==0&&f);C(fread(vectors,1,sizeof(vectors),f)==sizeof(vectors));C(fgetc(f)==EOF);C(fclose(f)==0);
 tp_init(&owner);C(tp_crypto_init(&owner));reset();C(q4_nv_public(&owner,0,a,nm));C(submits==1&&boundaries==2&&owner.calls==1);C(tp_eq(a,vectors+12,14)&&tp_eq(nm,vectors+28,34));
 reset();mode=6;C(q4_nv_public(&owner,1,a,nm));C(tp_eq(nm,vectors+282,34));reset();mode=6;C(!q4_nv_public(&owner,0,a,nm));
 memset(key,'a',32);memset(auth,'b',32);memset(caller,'c',32);memset(prior,'t',32);memset(name,'n',34);tp_w16(name,11);
 reset();C(q4_nv_read_packet(&owner,0x02000001,key,auth,name,caller,prior,cmd));C(tp_eq(cmd,vectors+62,99));C(submits==0);value=0;C(q4_nv_read_result(&owner,vectors+161,93,key,auth,caller,&value)&&value==42);
 for(i=0;i<93;i++)for(b=0;b<8;b++){reset();memcpy(bad,vectors+161,93);bad[i]^=(unsigned char)(1U<<b);value=777;C(!q4_nv_read_result(&owner,bad,93,key,auth,caller,&value));C(value==777&&owner.frozen);}
 for(i=0;i<93;i++){reset();value=777;C(!q4_nv_read_result(&owner,vectors+161,i,key,auth,caller,&value));C(value==777);}
 reset();memset(bad,0,10);tp_w16(bad,0x8001);tp_w32(bad+2,10);tp_w32(bad+6,0x98e);value=777;C(!q4_nv_read_result(&owner,bad,10,key,auth,caller,&value));C(value==777&&owner.first_native==0x98e);
 for(i=0;i<62;i++){unsigned char saved=vectors[i];reset();vectors[i]^=1;C(!q4_nv_public(&owner,0,a,nm));C(owner.frozen);first=owner.first;C(!q4_nv_public(&owner,0,a,nm)&&submits==1&&owner.first==first);vectors[i]=saved;}
 for(i=1;i<=5;i++){reset();mode=i;memset(a,0xcc,14);C(!q4_nv_public(&owner,0,a,nm));C(a[0]==0xcc);C(submits==(i==1?0U:1U));C(owner.frozen);if(i==4)C(owner.first_native==0x922);}
 reset();InterlockedExchange(&owner.busy,1);C(!q4_nv_public(&owner,0,a,nm)&&owner.frozen&&submits==0);InterlockedExchange(&owner.busy,0);
 reset();t=CreateThread(NULL,0,foreign,NULL,0,NULL);C(t!=NULL);C(WaitForSingleObject(t,INFINITE)==WAIT_OBJECT_0);C(GetExitCodeThread(t,&exitcode)&&exitcode==0);C(CloseHandle(t));C(!owner.frozen&&submits==0);
 reset();tp_w16(pubcmd,0x8001);tp_w32(pubcmd+2,14);tp_w32(pubcmd+6,0x169);tp_w32(pubcmd+10,Q4_NV_INDEX);n=sizeof(reply);C(q4_tbs_public_only(api_mock,(void *)1,pubcmd,14,reply,&n)==0&&n==62);
 for(i=0;i<14;i++){unsigned char saved=pubcmd[i];unsigned before=submits;pubcmd[i]^=1;n=sizeof(reply);C(q4_tbs_public_only(api_mock,(void *)1,pubcmd,14,reply,&n)!=0&&submits==before);pubcmd[i]=saved;}
 for(i=0;i<3;i++){unsigned codes[3]={0x12a,0x134,0x122};unsigned before=submits;tp_w32(pubcmd+6,codes[i]);n=sizeof(reply);C(q4_tbs_public_only(api_mock,(void *)1,pubcmd,14,reply,&n)!=0&&submits==before);}
 reset();owner.context=NULL;C(tp_finish(&owner));C(owner.controlled_refs==0&&!owner.retained);
 tp_init(&uncertain);C(tp_crypto_init(&uncertain));uncertain.context=(void *)1;uncertain.controlled_refs++;uncertain.transport.close=close_unknown;C(!tp_finish(&uncertain));C(uncertain.retained&&uncertain.context==(void *)1);C(!tp_finish(&uncertain));
 printf("PASS checks=%u; CNG_REAL; TPM_API_CALLS=0; TBS_ABI_MOCK_ONLY; UNKNOWN_RETAINS_SYNTHETIC_CONTEXT_AND_CNG_UNTIL_FIXTURE_PROCESS_EXIT; NO_AUTHORITY\n",checks);return 0;}
