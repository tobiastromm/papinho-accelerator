#include "roughtime.h"
#include <string.h>
#include <stdio.h>
#ifdef RT_OPENSSL_TEST
#include "roughtime_openssl.h"
#include <stdlib.h>
#include <openssl/provider.h>
#include <openssl/crypto.h>
#include "roughtime_public_vector.inc"
#endif
static unsigned checks,failures;
static void ck(int yes,int line){++checks;if(!yes){++failures;printf("FAIL line %d\n",line);}}
#define CK(x) ck((x),__LINE__)
/* Deliberately failing primitive stub, NEVER synthetic crypto success. */
static int unavailable_hash(void *u,const PAPACC_RT_BYTES *p,unsigned n,PAPACC_U8 *o){(void)u;(void)p;(void)n;(void)o;return 0;}
static int bad_sig(void *u,const PAPACC_U8 *k,const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *d,PAPACC_SIZE m,const PAPACC_U8 *s){(void)u;(void)k;(void)p;(void)n;(void)d;(void)m;(void)s;return 0;}
static void events(void){
 PAPACC_RT_EXCHANGE e,save;PAPACC_U64 n=(PAPACC_U64)1000000000UL;
 memset(&e,0,sizeof(e));CK(papacc_rt_exchange_start_data(&e,10,10)==PAPACC_RT_DATA_OK);
 CK(papacc_rt_exchange_sent_data(&e,20)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_received_data(&e,30)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_verified_data(&e,40)==PAPACC_RT_DATA_OK);
 CK(papacc_rt_exchange_received_data(&e,50)==PAPACC_RT_INVALID); /* replay */
 memset(&e,0,sizeof(e));CK(papacc_rt_exchange_start_data(&e,0,0)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_sent_data(&e,n)==PAPACC_RT_EXPIRED);save=e;
 papacc_rt_exchange_stop_data(&e);CK(e.phase==save.phase&&e.start==save.start);CK(papacc_rt_exchange_received_data(&e,n)==PAPACC_RT_RETAINED);
 memset(&e,0,sizeof(e));CK(papacc_rt_exchange_start_data(&e,0,0)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_sent_data(&e,1)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_received_data(&e,1+3*n)==PAPACC_RT_EXPIRED);
 memset(&e,0,sizeof(e));CK(papacc_rt_exchange_start_data(&e,~(PAPACC_U64)0,~(PAPACC_U64)0)==PAPACC_RT_INVALID);
 memset(&e,0,sizeof(e));CK(papacc_rt_exchange_start_data(&e,0,10)==PAPACC_RT_DATA_OK);CK(papacc_rt_exchange_sent_data(&e,9)==PAPACC_RT_INVALID);
 CK(papacc_rt_backoff_data(1)==1);CK(papacc_rt_backoff_data(2)==2);CK(papacc_rt_backoff_data(1000)==86400);
}
static void bounds(void){
 PAPACC_UTC_OBSERVATION q,qualities[6];PAPACC_RT_REPLY r,replies[6];PAPACC_U64 l=7,u=8;unsigned i;
 memset(&q,0,sizeof(q));r.midpoint=100;r.radius=1;
 CK(papacc_rt_interval_data(&r,&q,1,&l,&u)==PAPACC_RT_NOT_QUALIFIED);CK(l==7&&u==8);
 q.version=1;q.present=PAPACC_UTC_DATA_PRESENT;q.rho_den=1000000;q.rho_num=10;q.pal_error=100;q.deadline=1000000000;
 CK(papacc_rt_interval_data(&r,&q,100,&l,&u)==PAPACC_RT_DATA_OK);CK(l==(PAPACC_U64)99*1000000000UL);CK(u>(PAPACC_U64)101*1000000000UL);
 for(i=0;i<6;i++){qualities[i]=q;replies[i]=r;}
 CK(papacc_rt_hull_data(replies,qualities,100,&l,&u)==PAPACC_RT_DATA_OK);qualities[1].context.revision=1;CK(papacc_rt_hull_data(replies,qualities,100,&l,&u)==PAPACC_RT_NOT_QUALIFIED);
 r.midpoint=~(PAPACC_U64)0;CK(papacc_rt_interval_data(&r,&q,100,&l,&u)!=PAPACC_RT_DATA_OK);
 r.midpoint=0;CK(papacc_rt_interval_data(&r,&q,100,&l,&u)==PAPACC_RT_INVALID);
 CK(papacc_issuer_qualified_utc_read(NULL,&q)==PAPACC_RESULT_NOT_SUPPORTED);
}
#ifdef RT_OPENSSL_TEST
static int hex(unsigned char *out,const char *s){unsigned x,y;int count=0;while(s[0]&&s[1]){x=s[0]>='a'?(unsigned)(s[0]-'a'+10):(unsigned)(s[0]-'0');y=s[1]>='a'?(unsigned)(s[1]-'a'+10):(unsigned)(s[1]-'0');if(x>15||y>15)return 0;out[count++]=(unsigned char)(16*x+y);s+=2;}return *s?0:count;}
typedef union RT_ALLOC_HEAD { size_t n; long double align; void *pointer; } RT_ALLOC_HEAD;
static size_t rt_live,rt_peak;
static void *rt_malloc(size_t n,const char *file,int line){RT_ALLOC_HEAD *p;(void)file;(void)line;if(n>(size_t)-1-sizeof(*p))return NULL;p=(RT_ALLOC_HEAD *)malloc(n+sizeof(*p));if(!p)return NULL;p->n=n+sizeof(*p);rt_live+=p->n;if(rt_live>rt_peak)rt_peak=rt_live;return p+1;}
static void rt_free(void *v,const char *file,int line){RT_ALLOC_HEAD *p;(void)file;(void)line;if(!v)return;p=(RT_ALLOC_HEAD *)v-1;rt_live-=p->n;free(p);}
static void *rt_realloc(void *v,size_t n,const char *file,int line){RT_ALLOC_HEAD *p;void *out;size_t old;if(!v)return rt_malloc(n,file,line);if(!n){rt_free(v,file,line);return NULL;}p=(RT_ALLOC_HEAD *)v-1;old=p->n-sizeof(*p);out=rt_malloc(n,file,line);if(!out)return NULL;memcpy(out,v,old<n?old:n);rt_free(v,file,line);return out;}
static unsigned verify_calls;
static int counted_verify(void *c,const PAPACC_U8 *k,const PAPACC_U8 *p,PAPACC_SIZE pn,const PAPACC_U8 *d,PAPACC_SIZE n,const PAPACC_U8 *sig){++verify_calls;return papacc_rt_openssl_verify(c,k,p,pn,d,n,sig);}
static void real_crypto(void){
 OSSL_LIB_CTX *lib;OSSL_PROVIDER *provider;PAPACC_RT_OPENSSL backend;PAPACC_RT_CRYPTO c;
 unsigned char scratch[4096],key[32],sig[64],nonce[32],request[2048],response[2048],prior[2048],hash[32];
 PAPACC_RT_BYTES v,req,resp,proof;
 static const unsigned char dc[]="Roughtime v1 delegation signature",rc[]="Roughtime v1 response signature";PAPACC_RT_REPLY out;unsigned i;
 CK(CRYPTO_set_mem_functions(rt_malloc,rt_realloc,rt_free));printf("openssl_version=%lx\n",OpenSSL_version_num());lib=OSSL_LIB_CTX_new();CK(lib!=NULL);if(!lib)return;
 provider=OSSL_PROVIDER_load(lib,"default");CK(provider!=NULL);if(!provider){OSSL_LIB_CTX_free(lib);return;}
 backend.libctx=lib;backend.scratch=scratch;backend.bytes=sizeof(scratch);c.user=&backend;c.hash=papacc_rt_openssl_hash;c.verify=counted_verify;
 /* RFC8032 section7.1 test1: public empty-message verification, no keygen/sign. */
 CK(hex(key,"d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a")==32);
 CK(hex(sig,"e5564300c360ac729086e2cc806e828a84877f1eb8e5d974d873e065224901555fb8821590a33bacc61e39701cf9b46bd25bf5f0595bbe24655141438e7a100b")==64);
 CK(c.verify(c.user,key,(const unsigned char *)"",0,(const unsigned char *)"",0,sig));sig[0]^=1;CK(!c.verify(c.user,key,(const unsigned char *)"",0,(const unsigned char *)"",0,sig));
 v.p=(const unsigned char *)"";v.n=0;CK(c.hash(c.user,&v,1,hash));CK(hash[0]==0xcf&&hash[1]==0x83);
 req.p=vector_request;req.n=sizeof(vector_request);proof.p=NULL;proof.n=0;
 CK(papacc_rt_merkle_data(&c,req,proof,0,vector_root)==PAPACC_RT_DATA_OK);
 CK(papacc_rt_merkle_data(&c,req,proof,1,vector_root)==PAPACC_RT_INVALID);
 memset(scratch,51,64);proof.p=scratch;proof.n=64;CK(papacc_rt_merkle_data(&c,req,proof,2,math_two_node_root)==PAPACC_RT_DATA_OK);
 scratch[0]^=1;CK(papacc_rt_merkle_data(&c,req,proof,2,math_two_node_root)==PAPACC_RT_INVALID);
 proof.n=1025;CK(papacc_rt_merkle_data(&c,req,proof,0,vector_root)==PAPACC_RT_INVALID);
 v.p=vector_response;v.n=sizeof(vector_response);CK(papacc_rt_packet_data(v)==PAPACC_RT_DATA_OK);
 for(i=0;i<sizeof(vector_response);i++){v.n=i;CK(papacc_rt_packet_data(v)==PAPACC_RT_INVALID);}
 memset(nonce,7,sizeof(nonce));CK(papacc_rt_request_data(&c,key,nonce,request)==PAPACC_RT_DATA_OK);CK(!memcmp(request,"ROUGHTIM",8)&&request[8]==0xf4&&request[9]==7&&request[12]==5);CK(!memcmp(request+88,nonce,32));
 memcpy(prior,request,2048);CK(papacc_rt_request_data(&c,key,request+88,request)==PAPACC_RT_INVALID);CK(!memcmp(prior,request,2048));
 v.p=request;v.n=2048;CK(papacc_rt_packet_data(v)==PAPACC_RT_DATA_OK);request[24]=1;CK(papacc_rt_packet_data(v)==PAPACC_RT_INVALID);memcpy(request,prior,2048);
 req.p=request;req.n=2048;resp.p=response;resp.n=2048;memset(response,0,sizeof(response));memcpy(response,"ROUGHTIM",8);response[8]=0xf4;response[9]=7;
 memset(&out,0x55,sizeof(out));CK(papacc_rt_reply_data(&c,req,resp,key,&out)==PAPACC_RT_INVALID);
 for(i=0;i<2048;i++){resp.n=i;CK(papacc_rt_reply_data(&c,req,resp,key,&out)==PAPACC_RT_INVALID);}
 CK(out.midpoint==(PAPACC_U64)0x55555555UL*(((PAPACC_U64)1<<32)+1));
 CK(!c.verify(c.user,vector_key,dc,sizeof(dc),vector_dele,sizeof(vector_dele),vector_dele_sig)); /* archived RFC example is not a positive vector */
 CK(!c.verify(c.user,vector_pub,rc,sizeof(rc),vector_srep,sizeof(vector_srep),vector_srep_sig));
 CK(papacc_rt_request_data(&c,vector_key,vector_nonce,request)==PAPACC_RT_DATA_OK);
 req.p=request;req.n=2048;resp.p=vector_response;resp.n=sizeof(vector_response);verify_calls=0;
 CK(papacc_rt_reply_data(&c,req,resp,vector_key,&out)==PAPACC_RT_INVALID);CK(verify_calls==1); /* invalid delegation rejected, no synthetic success */
 request[88]^=1;verify_calls=0;CK(papacc_rt_reply_data(&c,req,resp,vector_key,&out)==PAPACC_RT_INVALID);CK(verify_calls==0);
 OSSL_PROVIDER_unload(provider);OSSL_LIB_CTX_free(lib);printf("crypto_test_CRT_peak=%u live_before_cleanup=%u\n",(unsigned)rt_peak,(unsigned)rt_live);
}
#endif
int main(void){
 PAPACC_RT_CRYPTO c;PAPACC_U8 key[32],nonce[32],out[2048];memset(&c,0,sizeof(c));memset(key,0,32);memset(nonce,0,32);memset(out,0x55,sizeof(out));
 CK(papacc_rt_request_data(&c,key,nonce,out)==PAPACC_RT_CRYPTO_UNAVAILABLE);c.hash=unavailable_hash;c.verify=bad_sig;CK(papacc_rt_request_data(&c,key,nonce,out)==PAPACC_RT_CRYPTO_UNAVAILABLE);CK(out[0]==0x55);
 events();bounds();
#ifdef RT_OPENSSL_TEST
 real_crypto();
#endif
 printf("checks=%u failures=%u sizeof_exchange=%u sizeof_reply=%u\n",checks,failures,(unsigned)sizeof(PAPACC_RT_EXCHANGE),(unsigned)sizeof(PAPACC_RT_REPLY));return failures?1:0;
}
