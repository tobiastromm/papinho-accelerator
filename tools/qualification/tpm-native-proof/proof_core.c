#include "proof_core.h"
#include <string.h>
#include <limits.h>
#include <psapi.h>
static int current(const TP_OWNER *o){return o&&o->owner_thread==GetCurrentThreadId();}
void tp_init(TP_OWNER *o){memset(o,0,sizeof(*o));o->owner_thread=GetCurrentThreadId();o->stage="ENTRY";o->first="NONE";o->first_stage="NONE";}
int tp_fail(TP_OWNER *o,const char *s,unsigned long n){if(!strcmp(o->first,"NONE")){o->first=s;o->first_native=n;o->first_stage=o->stage;}o->frozen=1;return 0;}
int tp_sample(TP_OWNER *o,const char *phase){TP_SAMPLE *s;DWORD n=0;const char *previous;PROCESS_MEMORY_COUNTERS_EX memory;
 if(!current(o)||o->frozen)return 0;previous=o->stage;o->stage=phase;
 if(o->sample_count==TP_MAX_SAMPLES)return tp_fail(o,"SAMPLE_STORAGE_EXHAUSTED",0);
 s=&o->samples[o->sample_count++];s->phase=phase;s->tick=GetTickCount64();s->own_refs=o->controlled_refs;
 memset(&memory,0,sizeof(memory));memory.cb=sizeof(memory);s->memory_known=GetProcessMemoryInfo(GetCurrentProcess(),(PROCESS_MEMORY_COUNTERS *)&memory,sizeof(memory))!=0;s->memory_error=s->memory_known?0:GetLastError();s->private_bytes=memory.PrivateUsage;s->working_set=memory.WorkingSetSize;
 s->known=GetProcessHandleCount(GetCurrentProcess(),&n)!=0;s->error=s->known?0:GetLastError();s->count=n;
 if(!s->known)return tp_fail(o,"NATIVE_HANDLE_QUERY_FAILED",s->error);
 if(!s->memory_known)return tp_fail(o,"NATIVE_MEMORY_QUERY_FAILED",s->memory_error);
 if(n>o->peak)o->peak=n;if(n>TP_NATIVE_LIMIT)return tp_fail(o,"NATIVE_HANDLE_GATE",0);o->stage=previous;return 1;
}
static int alg(TP_OWNER *o,BCRYPT_ALG_HANDLE *h,LPCWSTR id,ULONG flags){NTSTATUS s;if(o->frozen||o->closed)return 0;
 s=BCryptOpenAlgorithmProvider(h,id,NULL,flags);if(*h)o->controlled_refs++;
 if(s!=0||!*h){if(*h)o->retained=1;return tp_fail(o,"CNG_OPEN",(unsigned long)s);}return 1;
}
static int object_length(TP_OWNER *o,BCRYPT_ALG_HANDLE h,DWORD *n){DWORD got=0;NTSTATUS s=BCryptGetProperty(h,BCRYPT_OBJECT_LENGTH,(PUCHAR)n,sizeof(*n),&got,0);
 return s==0&&got==sizeof(*n)&&*n&&*n<=sizeof(o->hash_object)?1:tp_fail(o,"CNG_OBJECT_LENGTH",(unsigned long)s);
}
int tp_crypto_init(TP_OWNER *o){if(!current(o)||o->sha||o->hmac||o->frozen||o->closed)return 0;
 return alg(o,&o->sha,BCRYPT_SHA256_ALGORITHM,0)&&object_length(o,o->sha,&o->sha_object)&&alg(o,&o->hmac,BCRYPT_SHA256_ALGORITHM,BCRYPT_ALG_HANDLE_HMAC_FLAG)&&object_length(o,o->hmac,&o->hmac_object)&&alg(o,&o->rsa,BCRYPT_RSA_ALGORITHM,0)&&tp_sample(o,"DEPENDENCIES_READY");
}
static int digest(TP_OWNER *o,int mac,const unsigned char *key,size_t kn,const unsigned char *p,size_t n,unsigned char out[32]){NTSTATUS s;int ok;
 if(!current(o)||o->closed||o->frozen||o->work_hash||n>ULONG_MAX||kn>ULONG_MAX||(!p&&n)||(!key&&kn))return 0;
 if(++o->hash_calls==o->inject_hash_failure)return tp_fail(o,"INJECTED_HASH_FAILURE",0);
 s=BCryptCreateHash(mac?o->hmac:o->sha,&o->work_hash,o->hash_object,mac?o->hmac_object:o->sha_object,(PUCHAR)key,(ULONG)kn,0);
 if(o->work_hash)o->controlled_refs++;if(s!=0||!o->work_hash){if(o->work_hash)o->retained=1;return tp_fail(o,"HASH_CREATE",(unsigned long)s);}
 ok=tp_sample(o,mac?"HMAC_HELD":"HASH_HELD");
 if(ok){s=BCryptHashData(o->work_hash,(PUCHAR)p,(ULONG)n,0);if(s!=0)ok=tp_fail(o,"HASH_DATA",(unsigned long)s);}
 if(ok){s=BCryptFinishHash(o->work_hash,out,32,0);if(s!=0)ok=tp_fail(o,"HASH_FINISH",(unsigned long)s);}
 s=BCryptDestroyHash(o->work_hash);if(s!=0){o->retained=1;return tp_fail(o,"HASH_CLOSE_UNKNOWN",(unsigned long)s);}o->work_hash=NULL;o->controlled_refs--;SecureZeroMemory(o->hash_object,sizeof(o->hash_object));return ok;
}
int tp_hash(TP_OWNER *o,const unsigned char *p,size_t n,unsigned char out[32]){return digest(o,0,NULL,0,p,n,out);}
int tp_mac(TP_OWNER *o,const unsigned char *k,size_t kn,const unsigned char *p,size_t n,unsigned char out[32]){return digest(o,1,k,kn,p,n,out);}
unsigned tp_u16(const unsigned char *p){return ((unsigned)p[0]<<8)|p[1];}
unsigned tp_u32(const unsigned char *p){return ((unsigned)p[0]<<24)|((unsigned)p[1]<<16)|((unsigned)p[2]<<8)|p[3];}
void tp_w16(unsigned char *p,unsigned n){p[0]=(unsigned char)(n>>8);p[1]=(unsigned char)n;}
void tp_w32(unsigned char *p,unsigned n){p[0]=(unsigned char)(n>>24);p[1]=(unsigned char)(n>>16);p[2]=(unsigned char)(n>>8);p[3]=(unsigned char)n;}
int tp_eq(const unsigned char *a,const unsigned char *b,size_t n){size_t j;unsigned x=0;for(j=0;j<n;j++)x|=(unsigned)(a[j]^b[j]);return x==0;}
int tp_kdf(TP_OWNER *o,const unsigned char key[32],const char *label,const unsigned char *u,size_t un,const unsigned char *v,size_t vn,unsigned char out[32]){unsigned char b[160];size_t l=strlen(label),n;
 if(l>32||un>48||vn>48)return tp_fail(o,"KDF_SHAPE",0);tp_w32(b,1);memcpy(b+4,label,l+1);n=5+l;memcpy(b+n,u,un);n+=un;memcpy(b+n,v,vn);n+=vn;tp_w32(b+n,256);n+=4;return tp_mac(o,key,32,b,n,out);
}
int tp_rsa_import(TP_OWNER *o,const unsigned char modulus[256]){unsigned char b[sizeof(BCRYPT_RSAKEY_BLOB)+3+256];BCRYPT_RSAKEY_BLOB head;NTSTATUS s;
 if(!current(o)||o->closed||o->frozen||o->public_key)return 0;memset(&head,0,sizeof(head));head.Magic=BCRYPT_RSAPUBLIC_MAGIC;head.BitLength=2048;head.cbPublicExp=3;head.cbModulus=256;
 memcpy(b,&head,sizeof(head));b[sizeof(head)]=1;b[sizeof(head)+1]=0;b[sizeof(head)+2]=1;memcpy(b+sizeof(head)+3,modulus,256);
 s=BCryptImportKeyPair(o->rsa,NULL,BCRYPT_RSAPUBLIC_BLOB,&o->public_key,b,sizeof(b),0);if(o->public_key)o->controlled_refs++;
 if(s!=0||!o->public_key){if(o->public_key)o->retained=1;return tp_fail(o,"RSA_PUBLIC_IMPORT",(unsigned long)s);}memcpy(o->modulus,modulus,256);return tp_sample(o,"RSA_PUBLIC_READY");
}
int tp_cfb_fixture(TP_OWNER *o,const unsigned char key[16],const unsigned char iv[16],const unsigned char *plain,unsigned n,unsigned char *out){DWORD got=0,size=0,block=16;unsigned char local[16];NTSTATUS s;
 if(!current(o)||o->closed||o->frozen||o->aes||o->work_key||!n||n>32||n%16)return 0;
 if(!alg(o,&o->aes,BCRYPT_AES_ALGORITHM,0))return 0;
 s=BCryptSetProperty(o->aes,BCRYPT_CHAINING_MODE,(PUCHAR)BCRYPT_CHAIN_MODE_CFB,sizeof(BCRYPT_CHAIN_MODE_CFB),0);if(s!=0)return tp_fail(o,"AES_MODE",(unsigned long)s);
 if(!object_length(o,o->aes,&size))return 0;
 s=BCryptGenerateSymmetricKey(o->aes,&o->work_key,o->key_object,size,(PUCHAR)key,16,0);if(o->work_key)o->controlled_refs++;
 if(s!=0||!o->work_key){if(o->work_key)o->retained=1;return tp_fail(o,"AES_KEY",(unsigned long)s);}
 s=BCryptSetProperty(o->work_key,BCRYPT_MESSAGE_BLOCK_LENGTH,(PUCHAR)&block,sizeof(block),0);if(s!=0)return tp_fail(o,"AES_FEEDBACK",(unsigned long)s);
 memcpy(local,iv,16);s=BCryptEncrypt(o->work_key,(PUCHAR)plain,n,NULL,local,16,out,n,&got,0);if(s!=0||got!=n)return tp_fail(o,"AES_ENCRYPT",(unsigned long)s);
 if(BCryptDestroyKey(o->work_key)!=0){o->retained=1;return tp_fail(o,"AES_KEY_CLOSE",0);}o->work_key=NULL;o->controlled_refs--;
 if(BCryptCloseAlgorithmProvider(o->aes,0)!=0){o->retained=1;return tp_fail(o,"AES_PROVIDER_CLOSE",0);}o->aes=NULL;o->controlled_refs--;SecureZeroMemory(o->key_object,sizeof(o->key_object));return 1;
}
int tp_capability(const unsigned char *r,unsigned n,unsigned hs[8],unsigned *count){unsigned i,c;
 if(!r||n<19||tp_u16(r)!=0x8001||tp_u32(r+2)!=n||tp_u32(r+6)||r[10]||tp_u32(r+11)!=1)return 0;c=tp_u32(r+15);if(c>8||n!=19+4*c)return 0;
 for(i=0;i<c;i++){hs[i]=tp_u32(r+19+4*i);if((hs[i]&0xff000000U)!=0x81000000U||(i&&hs[i]<=hs[i-1]))return 0;}*count=c;return 1;
}
int tp_public(TP_OWNER *o,const unsigned char *r,unsigned n,const unsigned char mod[256],unsigned char area[314],unsigned char name[34]){const unsigned char *p;unsigned ex;unsigned char hash[32];
 if(!r||n<20||n>1024||tp_u16(r)!=0x8001||tp_u32(r+2)!=n||tp_u32(r+6))return -1;
 if(tp_u16(r+10)!=314||n!=398)return 0;p=r+12;
 if(tp_u16(p)!=1||tp_u16(p+2)!=11||tp_u32(p+4)!=0x300b2||tp_u16(p+8)!=32||tp_u16(p+42)!=6||tp_u16(p+44)!=128||tp_u16(p+46)!=0x43||tp_u16(p+48)!=0x10||tp_u16(p+50)!=2048||tp_u16(p+56)!=256)return 0;
 ex=tp_u32(p+52);if(ex&&ex!=65537)return 0;if(!tp_eq(p+58,mod,256))return 0;
 if(tp_u16(r+326)!=34||tp_u16(r+362)!=34||tp_u16(r+364)!=11)return -1;
 if(!tp_hash(o,p,314,hash)||tp_u16(r+328)!=11||!tp_eq(hash,r+330,32))return -1;
 memcpy(area,p,314);memcpy(name,r+328,34);return 1;
}
int tp_start_packet(unsigned h,const unsigned char nonce[32],const unsigned char cipher[256],unsigned char out[319]){
 if((h&0xff000000U)!=0x81000000U)return 0;memset(out,0,319);tp_w16(out,0x8001);tp_w32(out+2,319);tp_w32(out+6,0x176);tp_w32(out+10,h);tp_w32(out+14,0x40000007);tp_w16(out+18,32);memcpy(out+20,nonce,32);tp_w16(out+52,256);memcpy(out+54,cipher,256);out[310]=0;tp_w16(out+311,6);tp_w16(out+313,128);tp_w16(out+315,0x43);tp_w16(out+317,11);return 1;
}
int tp_start_response(const unsigned char *r,unsigned n,unsigned *session,unsigned char nonce[32]){
 if(!r||n!=48||tp_u16(r)!=0x8001||tp_u32(r+2)!=48||tp_u32(r+6)||(tp_u32(r+10)&0xff000000U)!=0x02000000U||tp_u16(r+14)!=32)return 0;
 *session=tp_u32(r+10);memcpy(nonce,r+16,32);return 1;
}
int tp_query(TP_OWNER *o,unsigned s,const unsigned char key[32],const unsigned char caller[32],const unsigned char tpm[32],unsigned char out[89]){unsigned char params[6],cp[32],message[97],mac[32];
 if((s&0xff000000U)!=0x02000000U)return 0;tp_w32(params,0x17b);tp_w16(params+4,32);if(!tp_hash(o,params,6,cp))return 0;
 memcpy(message,cp,32);memcpy(message+32,caller,32);memcpy(message+64,tpm,32);message[96]=0x40;if(!tp_mac(o,key,32,message,97,mac))return 0;
 memset(out,0,89);tp_w16(out,0x8002);tp_w32(out+2,89);tp_w32(out+6,0x17b);tp_w32(out+10,73);tp_w32(out+14,s);tp_w16(out+18,32);memcpy(out+20,caller,32);out[52]=0x40;tp_w16(out+53,32);memcpy(out+55,mac,32);tp_w16(out+87,32);SecureZeroMemory(mac,32);return 1;
}
int tp_verify(TP_OWNER *o,const unsigned char *r,unsigned n,const unsigned char key[32],const unsigned char caller[32]){unsigned length,a;unsigned char message[42],rp[32],auth[97],mac[32];
 if(!r||n<85||n>117||tp_u16(r)!=0x8002||tp_u32(r+2)!=n||tp_u32(r+6)||r[14])return 0;
 length=r[15];a=16+length;if(length>32||n!=85+length||tp_u32(r+10)!=length+2||tp_u16(r+a)!=32||r[a+34]!=0x40||tp_u16(r+a+35)!=32)return 0;
 tp_w32(message,0);tp_w32(message+4,0x17b);memcpy(message+8,r+14,length+2);if(!tp_hash(o,message,10+length,rp))return 0;
 memcpy(auth,rp,32);memcpy(auth+32,r+a+2,32);memcpy(auth+64,caller,32);auth[96]=0x40;if(!tp_mac(o,key,32,auth,97,mac))return 0;return tp_eq(mac,r+a+37,32);
}
static int boundary(TP_OWNER *o,const char *phase){o->stage=phase;if(o->frozen)return 0;if(InterlockedCompareExchange(&o->stop,0,0))return tp_fail(o,"OWNER_STOP",0);
 if(!o->boundary||!o->boundary(o,phase))return o->frozen?0:tp_fail(o,"BOUNDARY_DENY",0);return tp_sample(o,phase);
}
static int core_send(TP_OWNER *o,unsigned n,unsigned *rn,const char *phase){unsigned code;unsigned long rc;
 if(!boundary(o,phase))return 0;code=tp_u32(o->command+6);if(code!=0x17a&&code!=0x173&&code!=0x176&&code!=0x17b)return tp_fail(o,"COMMAND_NOT_ALLOWED",code);
 if(o->calls==12)return tp_fail(o,"CALL_LIMIT",0);o->calls++;if(code==0x176)o->session_attempted=1;
 *rn=TP_MAX_RESPONSE;rc=o->transport.send(o->transport.user,o->context,o->command,n,o->response,rn);
 if(rc)return tp_fail(o,"TBS_SUBMIT",rc);if(*rn<10||*rn>TP_MAX_RESPONSE||tp_u32(o->response+2)!=*rn)return tp_fail(o,"RESPONSE_FRAME_UNKNOWN",0);
 if(tp_u32(o->response+6))return tp_fail(o,"TPM_ERROR",tp_u32(o->response+6));return 1;
}
int tp_run(TP_OWNER *o,const unsigned char binding[32]){unsigned hs[8],count=0,i,rn=0,selected=0;unsigned char area[314],name[34],cipher[256];ULONG written=0;NTSTATUS s;unsigned long rc;int match,ok=0;BCRYPT_OAEP_PADDING_INFO pad;
 if(!current(o)||InterlockedCompareExchange(&o->busy,1,0))return 0;
 if(o->used||o->closed||o->frozen){InterlockedExchange(&o->busy,0);return 0;}o->used=1;memcpy(o->context_binding,binding,32);
 if(!o->public_key||!o->transport.open||!o->transport.send||!o->transport.close){tp_fail(o,"INCOMPLETE_DEPENDENCIES",0);goto done;}
 if(!boundary(o,"BEFORE_CONTEXT"))goto done;o->context_attempted=1;rc=o->transport.open(o->transport.user,&o->context);
 if(o->context)o->controlled_refs++;if(rc||!o->context){if(o->context)o->retained=1;tp_fail(o,"TBS_CONTEXT_OPEN",rc);goto done;}o->context_opened=1;
 memset(o->command,0,22);tp_w16(o->command,0x8001);tp_w32(o->command+2,22);tp_w32(o->command+6,0x17a);tp_w32(o->command+10,1);tp_w32(o->command+14,0x81000000);tp_w32(o->command+18,8);
 if(!core_send(o,22,&rn,"CAPABILITY")||!tp_capability(o->response,rn,hs,&count)){if(!o->frozen)tp_fail(o,"CAPABILITY_INCOMPLETE",0);goto done;}
 for(i=0;i<count;i++){
 tp_w16(o->command,0x8001);tp_w32(o->command+2,14);tp_w32(o->command+6,0x173);tp_w32(o->command+10,hs[i]);if(!core_send(o,14,&rn,"PUBLIC_CANDIDATE"))goto done;
 match=tp_public(o,o->response,rn,o->modulus,area,name);if(match<0){tp_fail(o,"PUBLIC_FRAME_NAME_DENY",0);goto done;}if(match){if(selected){tp_fail(o,"MULTIPLE_MATCHES",0);goto done;}selected=hs[i];memcpy(o->public_area,area,314);memcpy(o->public_name,name,34);}}
 if(!selected){tp_fail(o,"NO_MATCHING_EK_NO_FALLBACK",0);goto done;}
 if(!boundary(o,"CHALLENGE"))goto done;
 s=BCryptGenRandom(NULL,o->salt,32,BCRYPT_USE_SYSTEM_PREFERRED_RNG);if(s==0)s=BCryptGenRandom(NULL,o->start_nonce,32,BCRYPT_USE_SYSTEM_PREFERRED_RNG);if(s==0)s=BCryptGenRandom(NULL,o->proof_nonce,32,BCRYPT_USE_SYSTEM_PREFERRED_RNG);if(s!=0){tp_fail(o,"FRESH_RANDOM",(unsigned long)s);goto done;}
 pad.pszAlgId=BCRYPT_SHA256_ALGORITHM;pad.pbLabel=(PUCHAR)"SECRET";pad.cbLabel=7;
 s=BCryptEncrypt(o->public_key,o->salt,32,&pad,NULL,0,cipher,sizeof(cipher),&written,BCRYPT_PAD_OAEP);if(s!=0||written!=256){tp_fail(o,"RSA_OAEP_SECRET",(unsigned long)s);goto done;}
 if(!tp_sample(o,"RSA_CHALLENGE_READY")||!tp_start_packet(selected,o->start_nonce,cipher,o->command)||!core_send(o,319,&rn,"START_SESSION"))goto done;
 if(!tp_start_response(o->response,rn,&o->session,o->tpm_nonce)){tp_fail(o,"START_RESPONSE_UNKNOWN",0);goto done;}
 if(!tp_kdf(o,o->salt,"ATH",o->tpm_nonce,32,o->start_nonce,32,o->session_key))goto done;
 if(!tp_query(o,o->session,o->session_key,o->proof_nonce,o->tpm_nonce,o->command)||!core_send(o,89,&rn,"PROOF"))goto done;
 if(!tp_verify(o,o->response,rn,o->session_key,o->proof_nonce)){tp_fail(o,"PROOF_HMAC_UNKNOWN",0);goto done;}o->session_ended=1;
 tp_w16(o->command,0x8001);tp_w32(o->command+2,14);tp_w32(o->command+6,0x173);tp_w32(o->command+10,selected);
 if(!core_send(o,14,&rn,"FINAL_PUBLIC"))goto done;
 if(tp_public(o,o->response,rn,o->modulus,area,name)!=1||!tp_eq(area,o->public_area,314)||!tp_eq(name,o->public_name,34)){tp_fail(o,"FINAL_EK_CHANGED",0);goto done;}
 o->proof=1;ok=1;
done:SecureZeroMemory(cipher,sizeof(cipher));InterlockedExchange(&o->busy,0);return ok;
}
static int close_key(TP_OWNER *o,BCRYPT_KEY_HANDLE *h){NTSTATUS s;if(!*h)return 1;if(o->inject_close_failure==2){o->retained=1;return tp_fail(o,"KEY_CLOSE_UNKNOWN",0);}s=BCryptDestroyKey(*h);if(s!=0){o->retained=1;return tp_fail(o,"KEY_CLOSE_UNKNOWN",(unsigned long)s);}*h=NULL;o->controlled_refs--;return 1;}
static int close_alg(TP_OWNER *o,BCRYPT_ALG_HANDLE *h){NTSTATUS s;if(!*h)return 1;s=BCryptCloseAlgorithmProvider(*h,0);if(s!=0){o->retained=1;return tp_fail(o,"ALG_CLOSE_UNKNOWN",(unsigned long)s);}*h=NULL;o->controlled_refs--;return 1;}
int tp_finish(TP_OWNER *o){unsigned long rc;
 if(!current(o)||o->closed||InterlockedCompareExchange(&o->busy,0,0))return 0;
 if(o->retained)return 0;if(o->context){if(o->session_attempted&&!o->session_ended){o->retained=1;return tp_fail(o,"SESSION_END_UNKNOWN",0);}o->stage="CONTEXT_CLOSE";
 rc=o->transport.close(o->transport.user,o->context);if(rc){o->retained=1;return tp_fail(o,"CONTEXT_CLOSE_UNKNOWN",rc);}o->context=NULL;o->context_closed=1;o->controlled_refs--;}
 o->stage="CNG_CLOSE";
 if(!close_key(o,&o->work_key)||!close_key(o,&o->public_key)||!close_alg(o,&o->aes)||!close_alg(o,&o->rsa)||!close_alg(o,&o->hmac)||!close_alg(o,&o->sha))return 0;
 SecureZeroMemory(o->salt,32);SecureZeroMemory(o->start_nonce,32);SecureZeroMemory(o->proof_nonce,32);SecureZeroMemory(o->session_key,32);SecureZeroMemory(o->tpm_nonce,32);o->closed=1;
 if(!o->frozen)tp_sample(o,"CORE_CLOSED");return 1;
}
