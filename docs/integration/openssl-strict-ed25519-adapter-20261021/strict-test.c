#include "papacc_strict_ed25519.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/provider.h>
#include <openssl/crypto.h>
static unsigned checks, failures;
static size_t alloc_calls;
static void *count_malloc(size_t n,const char *f,int l) { (void)f;(void)l;++alloc_calls;return malloc(n); }
static void *count_realloc(void *p,size_t n,const char *f,int l) { (void)f;(void)l;++alloc_calls;return realloc(p,n); }
static void count_free(void *p,const char *f,int l) { (void)f;(void)l;free(p); }
#define CK(x) do { ++checks; if (!(x)) { ++failures; printf("FAIL line=%u\n",(unsigned)__LINE__); } } while(0)
static int unhex(unsigned char *p,const char *s) { unsigned a,b; int n=0; while(s[0]&&s[1]) { a=(unsigned)(s[0]>='a'?s[0]-'a'+10:s[0]-'0'); b=(unsigned)(s[1]>='a'?s[1]-'a'+10:s[1]-'0'); if(a>15||b>15)return -1; p[n++]=(unsigned char)(a*16+b);s+=2;}return *s?-1:n; }
extern int ossl_ed25519_verify(const unsigned char *,size_t,const unsigned char *,const unsigned char *,const unsigned char,const unsigned char,const unsigned char,const unsigned char *,size_t,OSSL_LIB_CTX *,const char *);
static void key_tests(void) {
 unsigned char k[64], saved[64]; size_t n; unsigned sign,j;
 CK(unhex(k,"d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a")==32);memset(k+32,0x55,32);memcpy(saved,k,64);
 CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_VALID);
 for(n=0;n<32;++n)CK(papacc_ossl358_ed25519_point_check(k,n)==PAPACC_ED25519_INVALID);
 for(n=33;n<=64;++n)CK(papacc_ossl358_ed25519_point_check(k,n)==PAPACC_ED25519_INVALID);
 CK(papacc_ossl358_ed25519_point_check(k,(size_t)-1)==PAPACC_ED25519_INVALID);
 for(n=0;n<=64;++n)CK(papacc_ossl358_ed25519_point_check(NULL,n)==PAPACC_ED25519_INVALID);
 CK(!memcmp(k,saved,64));
 for(j=0;j<100;++j)CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_VALID);
 CK(!memcmp(k,saved,64));
 for(sign=0;sign<2;++sign) {
  memset(k,0,32);k[0]=1;k[31]=(unsigned char)(sign*128);CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0xff,32);k[0]=0xec;k[31]=(unsigned char)(0x7f+sign*128);CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0,32);k[31]=(unsigned char)(sign*128);CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0,32);k[0]=2;k[31]=(unsigned char)(sign*128);CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0x99,32);k[0]=0x95;k[31]=(unsigned char)(0x19+sign*128);CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_INVALID);
  for(j=0;j<19;++j) {memset(k,0xff,32);k[0]=(unsigned char)(0xed+j);k[31]=(unsigned char)(0x7f+sign*128);CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_INVALID);}
  memset(k,0x66,32);k[0]=0x58;k[31]=(unsigned char)(0x66+sign*128);CK(papacc_ossl358_ed25519_point_check(k,32)==PAPACC_ED25519_VALID);
 }
}
static void signature_regression(void) {
 OSSL_LIB_CTX *ctx;OSSL_PROVIDER *provider;EVP_PKEY *key;EVP_MD_CTX *md;
 unsigned char pub[32],sig[64];int rc;
 CK(OpenSSL_version_num()==0x30500080UL);
 CK(unhex(pub,"d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a")==32);
 CK(unhex(sig,"e5564300c360ac729086e2cc806e828a84877f1eb8e5d974d873e065224901555fb8821590a33bacc61e39701cf9b46bd25bf5f0595bbe24655141438e7a100b")==64);
 ctx=OSSL_LIB_CTX_new();CK(ctx!=NULL);if(!ctx)return;
 provider=OSSL_PROVIDER_load(ctx,"default");CK(provider!=NULL);if(!provider){OSSL_LIB_CTX_free(ctx);return;}
 key=EVP_PKEY_new_raw_public_key_ex(ctx,"ED25519","provider=default",pub,32);md=EVP_MD_CTX_new();CK(key!=NULL&&md!=NULL);
 if(key&&md) {rc=EVP_DigestVerifyInit_ex(md,NULL,NULL,ctx,"provider=default",key,NULL);CK(rc==1);if(rc==1)CK(EVP_DigestVerify(md,sig,64,(const unsigned char *)"",0)==1);}
 CK(ossl_ed25519_verify((const unsigned char *)"",0,sig,pub,0,0,0,NULL,0,ctx,"provider=default")==1);
 sig[0]^=1;CK(ossl_ed25519_verify((const unsigned char *)"",0,sig,pub,0,0,0,NULL,0,ctx,"provider=default")==0);
 EVP_MD_CTX_free(md);EVP_PKEY_free(key);OSSL_PROVIDER_unload(provider);OSSL_LIB_CTX_free(ctx);
}
int main(void) {CK(CRYPTO_set_mem_functions(count_malloc,count_realloc,count_free)==1);key_tests();CK(alloc_calls==0);printf("point_checks_OpenSSL_allocation_calls=%u\n",(unsigned)alloc_calls);signature_regression();printf("checks=%u failures=%u TEST_ONLY_NO_AUTHORITY\n",checks,failures);return failures?1:0;}
