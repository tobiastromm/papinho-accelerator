/* LAB ONLY. Private keys are ephemeral EVP objects, never exported.
 * Output: raw RFC packets, public keys and public nonce-chain randomness.
 * No network, OS-clock observation/change, CA, X509, CURRENT or authority. */
#include "roughtime_openssl.h"
#include <openssl/evp.h>
#include <openssl/provider.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct KV { const char *tag; const unsigned char *p; size_t n; } KV;
static void le32(unsigned char *p,unsigned long v){unsigned i;for(i=0;i<4;i++){p[i]=(unsigned char)v;v>>=8;}}
static void le64(unsigned char *p,PAPACC_U64 v){unsigned i;for(i=0;i<8;i++){p[i]=(unsigned char)v;v>>=8;}}
static unsigned long tagnum(const char *p){return (unsigned long)(unsigned char)p[0]|((unsigned long)(unsigned char)p[1]<<8)|((unsigned long)(unsigned char)p[2]<<16)|((unsigned long)(unsigned char)p[3]<<24);}
static size_t pack(unsigned char *out,KV *items,unsigned n){unsigned i,j;size_t off=0;KV t;
 for(i=0;i<n;i++)for(j=i+1;j<n;j++)if(tagnum(items[i].tag)>tagnum(items[j].tag)){t=items[i];items[i]=items[j];items[j]=t;}
 le32(out,n);for(i=0;i<n;i++){if(i)le32(out+4*i,(unsigned long)off);memcpy(out+4*n+4*i,items[i].tag,4);memcpy(out+8*n+off,items[i].p,items[i].n);off+=items[i].n;}return 8*n+off;}
static int sign(OSSL_LIB_CTX *ctx,EVP_PKEY *key,const unsigned char *prefix,size_t pn,const unsigned char *data,size_t n,unsigned char *sig){
 unsigned char message[4096];EVP_MD_CTX *md;size_t sn=64;int ok=0;
 if(pn>64||n>2048)return 0;memcpy(message,prefix,pn);memcpy(message+pn,data,n);md=EVP_MD_CTX_new();
 if(md&&EVP_DigestSignInit_ex(md,NULL,NULL,ctx,"provider=default",key,NULL)==1)ok=EVP_DigestSign(md,sig,&sn,message,pn+n)==1&&sn==64;
 EVP_MD_CTX_free(md);OPENSSL_cleanse(message,sizeof(message));return ok;}
static int write_file(const char *directory,const char *name,const void *data,size_t n){char path[2048];FILE *f;int close_result;
 if(strlen(directory)+strlen(name)+2>=sizeof(path))return 0;sprintf(path,"%s/%s",directory,name);f=fopen(path,"rb");if(f){fclose(f);return 0;}
 f=fopen(path,"wb");if(!f)return 0;if(fwrite(data,1,n,f)!=n){fclose(f);return 0;}close_result=fclose(f);return close_result==0;}
int main(int argc,char **argv){
 OSSL_LIB_CTX *ctx=NULL;OSSL_PROVIDER *provider=NULL;EVP_PKEY *longkey[3]={NULL,NULL,NULL},*tempkey[3]={NULL,NULL,NULL};
 PAPACC_RT_OPENSSL adapter;PAPACC_RT_CRYPTO crypto;PAPACC_RT_BYTES pieces[3];PAPACC_RT_REPLY verified;
 unsigned char keys[96],randoms[192],requests[6][2048],responses[6][2048],pub[3][32],scratch[4096],nonce[32];size_t sizes[6],pn;
 unsigned char dummy[2048],dummy_nonce[32],leaves[4][32],parent[2][32],root[32],path[64],prefix;
 unsigned char dele[128],cert[224],srep[160],dsig[64],rsig[64],mint[8],maxt[8],mid[8],radi[4],one[4],type[4],index[4];
 size_t dn,cn,sn,bn;unsigned i,j,source;KV item[7];char name[64];int ok=0;
 static const unsigned char dc[]="Roughtime v1 delegation signature",rc[]="Roughtime v1 response signature";
 if(argc!=2)return 2;if(OpenSSL_version_num()!=0x30500080UL)return 3;
 if(!OPENSSL_init_crypto(OPENSSL_INIT_NO_LOAD_CONFIG,NULL))return 4;
 ctx=OSSL_LIB_CTX_new();if(!ctx)goto done;provider=OSSL_PROVIDER_load(ctx,"default");if(!provider)goto done;
 adapter.libctx=ctx;adapter.scratch=scratch;adapter.bytes=sizeof(scratch);crypto.user=&adapter;crypto.hash=papacc_rt_openssl_hash;crypto.verify=papacc_rt_openssl_verify;
 for(i=0;i<3;i++){
  longkey[i]=EVP_PKEY_Q_keygen(ctx,"provider=default","ED25519");tempkey[i]=EVP_PKEY_Q_keygen(ctx,"provider=default","ED25519");if(!longkey[i]||!tempkey[i])goto done;
  pn=32;if(EVP_PKEY_get_raw_public_key(longkey[i],keys+32*i,&pn)!=1||pn!=32)goto done;
  pn=32;if(EVP_PKEY_get_raw_public_key(tempkey[i],pub[i],&pn)!=1||pn!=32)goto done;
 }
 if(RAND_bytes_ex(ctx,randoms,sizeof(randoms),0)!=1)goto done;
 le64(mint,(PAPACC_U64)2000000000UL-3600U);le64(maxt,(PAPACC_U64)2000000000UL+3600U);le64(mid,(PAPACC_U64)2000000000UL);le32(radi,3);le32(one,1);le32(type,1);le32(index,2);
 for(i=0;i<6;i++){
  source=i%3;if(!i)memcpy(nonce,randoms,32);else{
   pieces[0].p=responses[i-1];pieces[0].n=sizes[i-1];pieces[1].p=randoms+32*i;pieces[1].n=32;if(!crypto.hash(crypto.user,pieces,2,nonce))goto done;}
  if(papacc_rt_request_data(&crypto,keys+32*source,nonce,requests[i])!=PAPACC_RT_DATA_OK)goto done;
  /* A four-leaf batch: actual request at index2, three other LAB requests.
   * All hashing uses existing provider primitives, no custom crypto. */
  prefix=0;for(j=0;j<4;j++){
   if(j==2){pieces[1].p=requests[i];}else{if(RAND_bytes_ex(ctx,dummy_nonce,32,0)!=1)goto done;if(papacc_rt_request_data(&crypto,keys+32*source,dummy_nonce,dummy)!=PAPACC_RT_DATA_OK)goto done;pieces[1].p=dummy;}
   pieces[0].p=&prefix;pieces[0].n=1;pieces[1].n=2048;if(!crypto.hash(crypto.user,pieces,2,leaves[j]))goto done;
  }
  prefix=1;for(j=0;j<2;j++){pieces[0].p=&prefix;pieces[0].n=1;pieces[1].p=leaves[2*j];pieces[1].n=32;pieces[2].p=leaves[2*j+1];pieces[2].n=32;if(!crypto.hash(crypto.user,pieces,3,parent[j]))goto done;}
  pieces[1].p=parent[0];pieces[2].p=parent[1];if(!crypto.hash(crypto.user,pieces,3,root))goto done;memcpy(path,leaves[3],32);memcpy(path+32,parent[0],32);
  item[0]=(KV){"PUBK",pub[source],32};item[1]=(KV){"MINT",mint,8};item[2]=(KV){"MAXT",maxt,8};dn=pack(dele,item,3);
  if(!sign(ctx,longkey[source],dc,sizeof(dc),dele,dn,dsig))goto done;
  item[0]=(KV){"SIG\0",dsig,64};item[1]=(KV){"DELE",dele,dn};cn=pack(cert,item,2);
  item[0]=(KV){"VER\0",one,4};item[1]=(KV){"RADI",radi,4};item[2]=(KV){"MIDP",mid,8};item[3]=(KV){"VERS",one,4};item[4]=(KV){"ROOT",root,32};sn=pack(srep,item,5);
  if(!sign(ctx,tempkey[source],rc,sizeof(rc),srep,sn,rsig))goto done;
  item[0]=(KV){"SIG\0",rsig,64};item[1]=(KV){"NONC",nonce,32};item[2]=(KV){"TYPE",type,4};item[3]=(KV){"PATH",path,64};item[4]=(KV){"SREP",srep,sn};item[5]=(KV){"CERT",cert,cn};item[6]=(KV){"INDX",index,4};
  memcpy(responses[i],"ROUGHTIM",8);bn=pack(responses[i]+12,item,7);le32(responses[i]+8,(unsigned long)bn);sizes[i]=bn+12;
  if(papacc_rt_reply_data(&crypto,(PAPACC_RT_BYTES){requests[i],2048},(PAPACC_RT_BYTES){responses[i],sizes[i]},keys+32*source,&verified)!=PAPACC_RT_DATA_OK)goto done;
 }
 for(i=0;i<6;i++){sprintf(name,"request-%u.bin",i);if(!write_file(argv[1],name,requests[i],2048))goto done;sprintf(name,"response-%u.bin",i);if(!write_file(argv[1],name,responses[i],sizes[i]))goto done;}
 if(!write_file(argv[1],"long-term-keys.bin",keys,96)||!write_file(argv[1],"nonce-randoms.bin",randoms,192))goto done;
 ok=1;printf("ROUGHTIME_TRANSCRIPT=LAB_ONLY pairs=6 request_bytes=2048 response_bytes=%u private_keys_exported=0 network=0 authority=0\n",(unsigned)sizes[0]);
 done:for(i=0;i<3;i++){EVP_PKEY_free(tempkey[i]);EVP_PKEY_free(longkey[i]);}OSSL_PROVIDER_unload(provider);OSSL_LIB_CTX_free(ctx);OPENSSL_cleanse(dummy_nonce,sizeof(dummy_nonce));OPENSSL_cleanse(scratch,sizeof(scratch));return ok?0:1;
}
