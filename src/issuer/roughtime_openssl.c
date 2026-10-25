#include "roughtime_openssl.h"
#include <string.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
static int separate(const PAPACC_U8 *a,PAPACC_SIZE n,const PAPACC_U8 *b,PAPACC_SIZE m){PAPACC_SIZE i;for(i=0;i<n;i++)if(a+i==b)return 0;for(i=0;i<m;i++)if(b+i==a)return 0;return 1;}
int papacc_rt_openssl_hash(void *user,const PAPACC_RT_BYTES *parts,unsigned count,PAPACC_U8 *out){
 PAPACC_RT_OPENSSL *c=(PAPACC_RT_OPENSSL *)user;EVP_MD *alg;EVP_MD_CTX *md;unsigned i,n=0;unsigned char digest[64];int ok=0;
 if(!c||!c->libctx||!parts||!out||count>3||OpenSSL_version_num()!=0x30500080UL)return 0;
 alg=EVP_MD_fetch((OSSL_LIB_CTX *)c->libctx,"SHA512","provider=default");md=EVP_MD_CTX_new();
 if(!alg||!md)goto done;
 if(EVP_DigestInit_ex2(md,alg,NULL)!=1)goto done;
 for(i=0;i<count;i++){if(parts[i].n>2048||(parts[i].n&&!parts[i].p))goto done;if(parts[i].n&&EVP_DigestUpdate(md,parts[i].p,parts[i].n)!=1)goto done;}
 if(EVP_DigestFinal_ex(md,digest,&n)!=1||n!=64)goto done;
 memcpy(out,digest,32);ok=1;
 done:EVP_MD_CTX_free(md);EVP_MD_free(alg);OPENSSL_cleanse(digest,sizeof(digest));return ok;
}
int papacc_rt_openssl_verify(void *user,const PAPACC_U8 *key,const PAPACC_U8 *prefix,PAPACC_SIZE pn,const PAPACC_U8 *data,PAPACC_SIZE n,const PAPACC_U8 *sig){
 PAPACC_RT_OPENSSL *c=(PAPACC_RT_OPENSSL *)user;EVP_PKEY *p=NULL;EVP_MD_CTX *md=NULL;int ok=0;
 if(!c||!c->libctx||!c->scratch||c->bytes!=4096||!key||!prefix||!data||!sig||pn>64||n>2048||OpenSSL_version_num()!=0x30500080UL)return 0;
 if(!separate(c->scratch,c->bytes,prefix,pn)||!separate(c->scratch,c->bytes,data,n)||!separate(c->scratch,c->bytes,key,32)||!separate(c->scratch,c->bytes,sig,64))return 0;
 memcpy(c->scratch,prefix,pn);memcpy(c->scratch+pn,data,n);
 p=EVP_PKEY_new_raw_public_key_ex((OSSL_LIB_CTX *)c->libctx,"ED25519","provider=default",key,32);md=EVP_MD_CTX_new();
 if(p&&md&&EVP_DigestVerifyInit_ex(md,NULL,NULL,(OSSL_LIB_CTX *)c->libctx,"provider=default",p,NULL)==1)
  ok=EVP_DigestVerify(md,sig,64,c->scratch,pn+n)==1;
 EVP_MD_CTX_free(md);EVP_PKEY_free(p);OPENSSL_cleanse(c->scratch,pn+n);return ok;
}
