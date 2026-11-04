#include "proof_core.h"
#include "json_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static unsigned checks;
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%u %s\n",(unsigned)__LINE__,#x);return 0;}}while(0)
typedef struct MOCK {TP_OWNER *owner;BCRYPT_KEY_HANDLE private_key;HANDLE event;unsigned mode,opens,sends,closes;unsigned char key[32],start_nonce[32],tpm_nonce[32],caller[32];} MOCK;
static TP_OWNER owner,kept[7];static MOCK mock,kept_mock[7];static unsigned kept_count;
static unsigned char binding[32];
static unsigned long mock_open(void *v,void **ctx){MOCK *m=v;m->opens++;if(m->mode==1)return 5;m->event=CreateEventW(NULL,TRUE,FALSE,NULL);if(!m->event)return GetLastError();*ctx=m->event;return m->mode==2?5:0;}
static void response_header(unsigned char *r,unsigned n,unsigned tag){memset(r,0,n);tp_w16(r,tag);tp_w32(r+2,n);}
static int public_reply(MOCK *m,unsigned char *r){unsigned char *p=r+12,h[32];response_header(r,398,0x8001);tp_w16(r+10,314);tp_w16(p,1);tp_w16(p+2,11);tp_w32(p+4,0x300b2);tp_w16(p+8,32);memset(p+10,6,32);tp_w16(p+42,6);tp_w16(p+44,128);tp_w16(p+46,0x43);tp_w16(p+48,0x10);tp_w16(p+50,2048);tp_w32(p+52,0);tp_w16(p+56,256);memcpy(p+58,m->owner->modulus,256);
 if(m->mode==16||(m->mode==7&&m->sends>3))p[58]^=1;
 if(!tp_hash(m->owner,p,314,h))return 0;tp_w16(r+326,34);tp_w16(r+328,11);memcpy(r+330,h,32);tp_w16(r+362,34);tp_w16(r+364,11);memset(r+366,9,32);return 1;}
static int proof_reply(TP_OWNER *o,const unsigned char key[32],const unsigned char caller[32],unsigned size,unsigned char *r){unsigned char nonce[32],parameters[34],rpdata[42],rp[32],auth[97],mac[32],material[32],plain[32];unsigned j,a;
 memset(nonce,7,32);parameters[0]=0;parameters[1]=(unsigned char)size;for(j=0;j<size;j++)parameters[j+2]=(unsigned char)(j+20);
 if(size==32){memset(plain,8,32);if(!tp_kdf(o,key,"CFB",nonce,32,caller,32,material)||!tp_cfb_fixture(o,material,material+16,plain,32,parameters+2))return 0;}
 tp_w32(rpdata,0);tp_w32(rpdata+4,0x17b);memcpy(rpdata+8,parameters,size+2);if(!tp_hash(o,rpdata,10+size,rp))return 0;
 memcpy(auth,rp,32);memcpy(auth+32,nonce,32);memcpy(auth+64,caller,32);auth[96]=0x40;if(!tp_mac(o,key,32,auth,97,mac))return 0;
 response_header(r,85+size,0x8002);tp_w32(r+10,size+2);memcpy(r+14,parameters,size+2);a=16+size;tp_w16(r+a,32);memcpy(r+a+2,nonce,32);r[a+34]=0x40;tp_w16(r+a+35,32);memcpy(r+a+37,mac,32);SecureZeroMemory(material,32);return 1;}
static unsigned long mock_send(void *v,void *ctx,const unsigned char *cmd,unsigned n,unsigned char *r,unsigned *rn){MOCK *m=v;TP_OWNER *o=m->owner;unsigned code=tp_u32(cmd+6);ULONG written=0;unsigned char salt[256],nonce[32];NTSTATUS status;BCRYPT_OAEP_PADDING_INFO pad;
 if(ctx!=m->event)return 87;m->sends++;
 if(m->mode==13){*rn=4097;return 0;}if(m->mode==3){response_header(r,10,0x8001);tp_w32(r+6,0x101);*rn=10;return 0;}
 if(code==0x17a){if(m->mode==18){unsigned i;*rn=51;response_header(r,*rn,0x8001);tp_w32(r+11,1);tp_w32(r+15,8);for(i=0;i<8;i++)tp_w32(r+19+4*i,0x81000001+i);return 0;}*rn=m->mode==15?27:23;response_header(r,*rn,0x8001);r[10]=(unsigned char)(m->mode==14);tp_w32(r+11,1);tp_w32(r+15,m->mode==15?2:1);tp_w32(r+19,0x81000001);if(m->mode==15)tp_w32(r+23,0x81000002);return 0;}
 if(code==0x173){unsigned mode=m->mode;if(mode==18&&tp_u32(cmd+10)!=0x81000008)m->mode=16;if(n!=14||!public_reply(m,r))return 87;m->mode=mode;*rn=398;return 0;}
 if(code==0x176){if(m->mode==4)return 0x8028400fUL;if(n!=319)return 87;pad.pszAlgId=BCRYPT_SHA256_ALGORITHM;pad.pbLabel=(PUCHAR)"SECRET";pad.cbLabel=7;
 status=BCryptDecrypt(m->private_key,(PUCHAR)cmd+54,256,&pad,NULL,0,salt,sizeof(salt),&written,BCRYPT_PAD_OAEP);if(status<0||written!=32)return 87;memcpy(m->start_nonce,cmd+20,32);memset(nonce,6,32);memcpy(m->tpm_nonce,nonce,32);
 if(!tp_kdf(o,salt,"ATH",nonce,32,cmd+20,32,m->key))return 87;SecureZeroMemory(salt,sizeof(salt));response_header(r,48,0x8001);tp_w32(r+10,0x02000001);tp_w16(r+14,32);memcpy(r+16,nonce,32);*rn=48;if(m->mode==5)r[15]=31;return 0;}
 if(code==0x17b){unsigned char query[89];if(n!=89||!tp_query(o,0x02000001,m->key,cmd+20,m->tpm_nonce,query)||!tp_eq(query,cmd,89))return 87;memcpy(m->caller,cmd+20,32);if(!proof_reply(o,m->key,cmd+20,32,r))return 87;*rn=117;if(m->mode==6)r[116]^=1;return 0;}return 87;
}
static unsigned long mock_close(void *v,void *ctx){MOCK *m=v;m->closes++;if(m->mode==8)return 5;if(ctx!=m->event||!CloseHandle(m->event))return 87;m->event=NULL;
 if(m->private_key){if(BCryptDestroyKey(m->private_key)<0)return 87;m->private_key=NULL;}return 0;}
static int mock_boundary(TP_OWNER *o,const char *phase){MOCK *m=o->transport.user;
 if(m->mode==9&&!strcmp(phase,"CHALLENGE")){InterlockedExchange(&o->stop,1);return tp_fail(o,"OWNER_STOP",0);}
 if(m->mode==10)return tp_fail(o,"NATIVE_OBSERVER_HEADROOM",0);
 if(m->mode==11)C(!tp_run(o,binding));
 if(m->mode==12&&!strcmp(phase,"START_SESSION"))return tp_fail(o,"TOKEN_CONTINUITY_DENY",0);
 if(m->mode==17&&!strcmp(phase,"PROOF"))return tp_fail(o,"OWNER_STOP",0);
 return 1;}
static int setup(TP_OWNER *o,MOCK *m,unsigned mode){unsigned char blob[1024];ULONG n;BCRYPT_RSAKEY_BLOB *head=(BCRYPT_RSAKEY_BLOB *)blob;
 memset(m,0,sizeof(*m));m->mode=mode;m->owner=o;tp_init(o);C(tp_crypto_init(o));C(BCryptGenerateKeyPair(o->rsa,&m->private_key,2048,0)>=0);C(BCryptFinalizeKeyPair(m->private_key,0)>=0);
 C(BCryptExportKey(m->private_key,NULL,BCRYPT_RSAPUBLIC_BLOB,blob,sizeof(blob),&n,0)>=0);C(head->cbModulus==256&&head->cbPublicExp==3);C(tp_rsa_import(o,blob+sizeof(*head)+3));
 o->transport.user=m;o->transport.open=mock_open;o->transport.send=mock_send;o->transport.close=mock_close;o->boundary=mock_boundary;return 1;}
static int release_known_mock(TP_OWNER *o,MOCK *m){if(m->private_key&&!o->context&&!o->retained){C(BCryptDestroyKey(m->private_key)>=0);m->private_key=NULL;}return 1;}
static DWORD WINAPI foreign_run(void *p){return tp_run((TP_OWNER *)p,binding)?1UL:0UL;}
static int native_pressure(void){HANDLE events[128];unsigned i,n=0;DWORD count=0;tp_init(&owner);C(GetProcessHandleCount(GetCurrentProcess(),&count));
 while(count<=128&&n<128){events[n]=CreateEventW(NULL,TRUE,FALSE,NULL);C(events[n]!=NULL);n++;C(GetProcessHandleCount(GetCurrentProcess(),&count));}
 C(count>128);C(!tp_sample(&owner,"NEGATIVE_PRESSURE_129")&&!strcmp(owner.first,"NATIVE_HANDLE_GATE")&&!owner.context_attempted);for(i=0;i<n;i++)C(CloseHandle(events[i]));C(tp_finish(&owner));puts("PASS REAL_SYNTHETIC_HANDLE_PRESSURE_REJECTED BEFORE_ANY_TPM");return 1;}
static int suite(void){unsigned mode,i,j,rn;unsigned char r[128],copy[128],key[32],caller[32],saltA[32];TP_OWNER *o;MOCK *m;unsigned kept_modes=0;
 memset(binding,5,32);for(mode=0;mode<=20;mode++){
 int expect=mode==0||mode==8||mode==11||mode==18;
 if(mode==2||mode==4||mode==5||mode==6||mode==8||mode==17||mode==19){/* Retain fixture dependencies permanently; no retry/guessed close. */
 if(kept_modes==7)continue;o=&kept[kept_modes];m=&kept_mock[kept_modes++];kept_count=kept_modes;
 }else{o=&owner;m=&mock;}
 C(setup(o,m,mode==19?1:mode));if(mode==19)o->inject_close_failure=2;if(mode==20)o->inject_hash_failure=1;if(mode==0){HANDLE t;DWORD code=999;t=CreateThread(NULL,0,foreign_run,o,0,NULL);C(t!=NULL);C(WaitForSingleObject(t,10000)==WAIT_OBJECT_0&&GetExitCodeThread(t,&code)&&code==0);C(CloseHandle(t));C(!o->used&&!o->frozen&&!m->opens);}
 C(tp_run(o,binding)==expect);C(o->calls<=12);C(!tp_run(o,binding));C(!InterlockedCompareExchange(&o->busy,0,0));
 if(mode==8){C(o->proof&&o->session_ended);C(!tp_finish(o));C(o->retained&&!o->closed);C(!strcmp(o->first,"CONTEXT_CLOSE_UNKNOWN"));{unsigned closes=m->closes;C(!tp_finish(o)&&m->closes==closes);}}
 else if(expect){if(mode==11)C(!tp_eq(saltA,o->salt,32));C(o->proof&&o->session_ended&&o->calls==(mode==18?12U:5U));memcpy(saltA,o->salt,32);C(tp_finish(o));C(o->closed&&o->context_closed&&!o->retained&&!o->controlled_refs);C(!tp_finish(o));C(!tp_run(o,binding));}
 else{const char *first=o->first;C(strcmp(first,"NONE")!=0);C(release_known_mock(o,m));tp_finish(o);C(o->first==first);if(o->retained){unsigned closes=m->closes;C(!tp_finish(o));C(closes==m->closes);}else C(o->closed&&!o->controlled_refs);}
 printf("CASE mode=%u peak=%u samples=%u retained=%u refs=%u first=%s\n",mode,o->peak,o->sample_count,o->retained,o->controlled_refs,o->first);
 }
 /* Parser/HMAC negative suite with one clean independent owner. */
 C(setup(&owner,&mock,0));for(i=0;i<32;i++){key[i]=(unsigned char)(i+1);caller[i]=(unsigned char)(i+3);}for(rn=0;rn<=32;rn++){
 owner.sample_count=0;C(proof_reply(&owner,key,caller,rn,r));C(tp_verify(&owner,r,85+rn,key,caller));for(j=0;j<85+rn;j++){owner.sample_count=0;memcpy(copy,r,85+rn);copy[j]^=1;C(!tp_verify(&owner,copy,85+rn,key,caller));}}
 for(i=0;i<32;i++)caller[i]^=1;C(!tp_verify(&owner,r,117,key,caller));C(release_known_mock(&owner,&mock));C(tp_finish(&owner));
 C(native_pressure());
 printf("PASS assertions=%u retained_models=%u no_real_TPM_no_authority owner_bytes=%Iu\n",checks,kept_count,sizeof(TP_OWNER));return 1;
}
static int vectors(const char *path){FILE *f=NULL;unsigned char all[4096],out[512],key[32],nonce[32],area[314],name[34],mod[256],material[32],plain[32],copy[512];const unsigned char *v[13];unsigned n[13],i,pos=8,total,hs[8],count=0,session=0;
 C(fopen_s(&f,path,"rb")==0&&f);total=(unsigned)fread(all,1,sizeof(all),f);C(fclose(f)==0);C(total<sizeof(all)&&total>=8&&!memcmp(all,"TPVECT01",8));
 for(i=0;i<13;i++){C(total-pos>=4);n[i]=tp_u32(all+pos);pos+=4;C(n[i]<=total-pos);v[i]=all+pos;pos+=n[i];}C(pos==total);
 C(n[0]==32&&n[1]==32&&n[2]==32&&n[3]==32&&n[4]==32&&n[5]==319&&n[6]==48&&n[7]==89&&n[8]==117&&n[9]==398&&n[10]==27&&n[11]==32&&n[12]==398);
 tp_init(&owner);C(tp_crypto_init(&owner));C(tp_kdf(&owner,v[0],"ATH",v[3],32,v[1],32,key)&&tp_eq(key,v[4],32));
 for(i=0;i<256;i++)out[i]=(unsigned char)i;C(tp_start_packet(0x81000001,v[1],out,copy)&&tp_eq(copy,v[5],319));C(tp_start_response(v[6],48,&session,nonce)&&session==0x02000001&&tp_eq(nonce,v[3],32));
 C(tp_query(&owner,session,key,v[2],v[3],out)&&tp_eq(out,v[7],89));C(tp_verify(&owner,v[8],117,key,v[2]));
 memcpy(mod,v[9]+70,256);C(tp_public(&owner,v[9],398,mod,area,name)==1);
 for(i=0;i<398;i++){owner.sample_count=0;memcpy(copy,v[9],398);copy[i]^=1;C((tp_public(&owner,copy,398,mod,area,name)==1)==(v[12][i]!=0));}
 C(tp_capability(v[10],27,hs,&count)&&count==2&&hs[0]==0x81000001&&hs[1]==0x81000002);
 owner.sample_count=0;C(tp_kdf(&owner,key,"CFB",v[8]+50,32,v[2],32,material));for(i=0;i<32;i++)plain[i]=(unsigned char)(9+i);C(tp_cfb_fixture(&owner,material,material+16,plain,32,out)&&tp_eq(out,v[11],32));C(tp_finish(&owner));
 printf("FINAL_ASSERTIONS=%u\n",checks);puts("PASS CROSS_LANGUAGE exact KDF/319/48/89/HMAC/public398/capability/AES-CFB and398 public mutation decisions");return 1;
}
int main(int argc,char **argv){if(!suite())return 1;if(argc==2&&!vectors(argv[1]))return 1;return 0;}
