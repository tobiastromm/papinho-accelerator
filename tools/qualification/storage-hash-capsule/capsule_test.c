#include "storage_sha256.h"
#include <stdio.h>
#include <string.h>
#include "vectors.h"
static unsigned checks,failures;static unsigned char block[1024];
static void ck(int yes,unsigned line){checks++;if(!yes){failures++;printf("FAIL line=%u\n",line);}}
#define CK(x) ck((x),__LINE__)
static void zero_context(PACC_STORAGE_SHA256 *c){memset(c,0,sizeof(*c));}
static volatile unsigned stop_token;
static unsigned char octet(unsigned long pos,const VEC *v){if(v->prefix&&pos<8)return (unsigned char)v->prefix[pos];return (unsigned char)(v->kind==1?"abc"[pos%3]:v->kind==2?'a':(unsigned char)(pos*17UL+3UL));}
static void vector_run(const VEC *v,unsigned step){
 PACC_STORAGE_SHA256 c;unsigned char out[32];unsigned long at=0;unsigned n,j;
 zero_context(&c);memset(out,0x55,sizeof(out));CK(pacc_storage_sha_init(&c)==0);
 CK(pacc_storage_sha_update(&c,NULL,0,NULL,NULL)==0);
 while(at<v->length){n=(unsigned)(v->length-at);if(n>step)n=step;
  for(j=0;j<n;j++)block[j]=octet(at+j,v);
  CK(pacc_storage_sha_update(&c,block,n,NULL,NULL)==0);at+=n;
 }
 CK(pacc_storage_sha_final(&c,out,sizeof(out),NULL,NULL)==0);CK(!memcmp(out,v->expected,32));
 CK(c.state==PACC_HASH_DONE&&!c.busy);for(j=0;j<sizeof(c.hash);j++)CK(((unsigned char *)&c.hash)[j]==0);
 CK(pacc_storage_sha_update(&c,block,1,NULL,NULL)==PACC_HASH_STATE);
 CK(pacc_storage_sha_final(&c,out,sizeof(out),NULL,NULL)==PACC_HASH_STATE);
}
int main(void){
 unsigned i,j;unsigned char out[32],saved[32];PACC_STORAGE_SHA256 a,b;
 PACC_STORAGE_HASH_CALL call;uint64_t limit=(((uint64_t)1U)<<61)-1U;
 for(i=0;i<VECTOR_COUNT;i++){vector_run(&vectors[i],1024);if(vectors[i].length<40000UL){vector_run(&vectors[i],1);vector_run(&vectors[i],7);vector_run(&vectors[i],64);}}
 CK(pacc_storage_sha_init(NULL)==PACC_HASH_INVALID);zero_context(&a);CK(pacc_storage_sha_init(&a)==0);CK(pacc_storage_sha_init(&a)==PACC_HASH_STATE);
 memset(out,0x55,32);memcpy(saved,out,32);CK(pacc_storage_sha_final(&a,out,31,NULL,NULL)==PACC_HASH_INVALID);CK(!memcmp(out,saved,32));CK(a.state==PACC_HASH_FAILED);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);CK(pacc_storage_sha_update(&a,NULL,1,NULL,NULL)==PACC_HASH_INVALID);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);CK(pacc_storage_sha_update(&a,&a,sizeof(a),NULL,NULL)==PACC_HASH_INVALID);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);CK(pacc_storage_sha_update(&a,(const void *)((size_t)-8),16,NULL,NULL)==PACC_HASH_INVALID);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);a.hash.count=limit;CK(pacc_storage_sha_update(&a,block,1,NULL,NULL)==PACC_HASH_LIMIT);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);a.hash.count=limit+1U;CK(pacc_storage_sha_update(&a,NULL,0,NULL,NULL)==PACC_HASH_LIMIT);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);CK(pacc_storage_sha_final(&a,NULL,32,NULL,NULL)==PACC_HASH_INVALID);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);CK(pacc_storage_sha_final(&a,(unsigned char *)&a,32,NULL,NULL)==PACC_HASH_INVALID);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);stop_token=1;
 CK(pacc_storage_sha_update(&a,block,1024,&stop_token,NULL)==PACC_HASH_STOPPED);CK(a.state==PACC_HASH_CANCELLED&&!a.busy);
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);stop_token=0;CK(pacc_storage_sha_update(&a,block,64,&stop_token,NULL)==0);stop_token=1;
 CK(pacc_storage_sha_update(&a,block,64,&stop_token,NULL)==PACC_HASH_STOPPED);CK(!memcmp(out,saved,32));
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);CK(pacc_storage_sha_final(&a,out,32,&stop_token,NULL)==PACC_HASH_STOPPED);CK(!memcmp(out,saved,32));
 zero_context(&a);CK(pacc_storage_sha_init(&a)==0);a.busy=1;CK(pacc_storage_sha_update(&a,block,1,NULL,NULL)==PACC_HASH_BUSY);CK(a.busy==1);a.busy=0;
 zero_context(&a);zero_context(&b);CK(pacc_storage_sha_init(&a)==0&&pacc_storage_sha_init(&b)==0);
 CK(pacc_storage_sha_update(&a,"a",1,NULL,NULL)==0);CK(pacc_storage_sha_update(&b,"abc",3,NULL,NULL)==0);CK(pacc_storage_sha_update(&a,"bc",2,NULL,NULL)==0);
 CK(pacc_storage_sha_final(&b,out,32,NULL,NULL)==0&&!memcmp(out,vectors[1].expected,32));CK(pacc_storage_sha_final(&a,out,32,NULL,NULL)==0&&!memcmp(out,vectors[1].expected,32));
 CK(pacc_storage_hash_callback(NULL,(const unsigned char *)"abc",3,out)&&!memcmp(out,vectors[1].expected,32));
 memset(out,0x55,32);call.stop=&stop_token;call.context=NULL;stop_token=1;CK(!pacc_storage_hash_callback(&call,block,100,out));CK(!memcmp(out,saved,32));
 /* No secret/lease remains: cancelled/failed hash state is physically cleared. */
 for(j=0;j<sizeof(a.hash);j++)CK(((unsigned char *)&a.hash)[j]==0);
 printf("checks=%u failures=%u hash_context=%u capsule_context=%u two_contexts=%u chunk=%u LAB_ONLY_NO_AUTHORITY\n",checks,failures,(unsigned)sizeof(br_sha256_context),(unsigned)sizeof(a),(unsigned)(sizeof(a)+sizeof(b)),PACC_STORAGE_HASH_CHUNK);
 return failures?1:0;
}
