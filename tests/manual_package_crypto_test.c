#include "manual_package_crypto.h"
#ifdef MP_AUTH_NATIVE_INTEGRATION_TEST
#include "manual_package_validation.h"
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <windows.h>
#include <bcrypt.h>
#include <malloc.h>
#include <stdint.h>
/* Isolated serial test process only. Actual CRT allocation bytes include our
 * 16-byte accounting header. Kernel/provider internals remain unqualified. */
typedef struct ALLOCATION {size_t requested,actual;} ALLOCATION;
static size_t live=0,peak=0;
static void *tracked_alloc(size_t n,const char *file,int line){
 ALLOCATION *a;(void)file;(void)line;if(n>SIZE_MAX-sizeof(*a))return NULL;
 a=(ALLOCATION *)malloc(n+sizeof(*a));if(!a)return NULL;
 a->requested=n;a->actual=_msize(a);live+=a->actual;if(live>peak)peak=live;return a+1;
}
static void tracked_free(void *p,const char *file,int line){
 ALLOCATION *a;(void)file;(void)line;if(!p)return;a=(ALLOCATION *)p-1;live-=a->actual;free(a);
}
static void *tracked_realloc(void *p,size_t n,const char *file,int line){
 ALLOCATION *a;void *next;if(!p)return tracked_alloc(n,file,line);
 if(!n){tracked_free(p,file,line);return NULL;}a=(ALLOCATION *)p-1;
 next=tracked_alloc(n,file,line);if(!next)return NULL;
 memcpy(next,p,a->requested<n?a->requested:n);tracked_free(p,file,line);return next;
}
static void phase(const char *name){printf("memory_phase=%s peak_crt_bytes=%zu live_crt_bytes=%zu\n",name,peak,live);}
static int checks=0;
#define CHECK(x) do{checks++;if(!(x)){fprintf(stderr,"failure line %d\n",__LINE__);return 1;}}while(0)
#ifdef MP_AUTH_NATIVE_INTEGRATION_TEST
#include "manual_package_pdg_fixture.inc"
#endif
static int hex(const char *s,unsigned char *out){unsigned int v;int n=0;
 while(*s){if(sscanf(s,"%2x",&v)!=1)return 0;out[n++]=(unsigned char)v;s+=2;}return n;}
static int stop(void *u){int *n=(int *)u;return --*n<=0;}
static void be32(unsigned char *p,size_t n){p[0]=(unsigned char)(n>>24);p[1]=(unsigned char)(n>>16);p[2]=(unsigned char)(n>>8);p[3]=(unsigned char)n;}
static size_t fixture(const char *path,unsigned char *out,size_t cap){
 FILE *f=fopen(path,"rb");char b[24000];size_t n,i,j=0;int decoded,pad=0;
 if(!f)return 0;n=fread(b,1,sizeof(b),f);fclose(f);
 for(i=0;i<n;i++)if(b[i]!='\r'&&b[i]!='\n'&&b[i]!=' ')b[j++]=b[i];
 if(j<4||j%4||j/4*3>cap)return 0;
 if(b[j-1]=='=')pad++;if(b[j-2]=='=')pad++;
 decoded=EVP_DecodeBlock(out,(const unsigned char *)b,(int)j);
 return decoded>=pad?(size_t)(decoded-pad):0;
}
int main(int argc,char **argv){
 BCRYPT_ALG_HANDLE bcrypt=NULL;unsigned char native_key[32];
 MP_CRYPTO *c;unsigned char key[32],expected[256],salt[16],nonce[12],aad[12];
 unsigned char cipher[256],tag[16],q[256],out[256],pw[32],wrong[32];
 const unsigned char plain[]="Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
 size_t n=sizeof(plain)-1,used=9,i;int count;
 unsigned char cert[16384],private_der[8192],other[8192],ids[16]={0};size_t cn,pn,on;
 FILE *f;BIO *bio;char pem[8192];size_t pemn;EVP_PKEY *signer;EVP_MD_CTX *md;EVP_PKEY_CTX *pc=NULL;
 unsigned char spki[1024],*sp=spki,raw[2048],header[92],signature[256],hash[32];
 size_t sn,siglen=256,hn=0,rn=0;MP_ENVELOPE env;
 static const unsigned char domain[]="PAPACC-COMPLETE-MANUAL-V2";
 CHECK(argc==5);CHECK(CRYPTO_set_mem_functions(tracked_alloc,tracked_realloc,tracked_free)==1);c=mp_crypto_open();CHECK(c!=NULL);
 phase("private_provider_open");memset(pw,'A',32);for(i=0;i<16;i++)salt[i]=(unsigned char)i;
 CHECK(mp_crypto_derive(c,pw,32,salt,16,key,NULL,NULL)==MP_CRYPTO_ONLY);
 CHECK(hex("cd9ead6a37c37227e0f1f521cfcb321504eaab5dc335824da45c2bb8f264a92e",expected)==32);
 CHECK(!memcmp(key,expected,32));phase("exact_kdf_before_native_comparison");
 CHECK(BCryptOpenAlgorithmProvider(&bcrypt,BCRYPT_SHA256_ALGORITHM,NULL,BCRYPT_ALG_HANDLE_HMAC_FLAG)==0);
 CHECK(BCryptDeriveKeyPBKDF2(bcrypt,pw,32,salt,16,600000,native_key,32,0)==0);
 CHECK(!memcmp(native_key,key,32));CHECK(BCryptCloseAlgorithmProvider(bcrypt,0)==0);
 memset(nonce,0,12);memset(aad,0,12);
 CHECK(mp_crypto_seal(c,key,nonce,12,aad,12,plain,n,cipher,sizeof(cipher),tag)==MP_CRYPTO_ONLY);
 memset(wrong,'B',32);CHECK(mp_crypto_derive(c,wrong,32,salt,16,key,NULL,NULL)==MP_CRYPTO_ONLY);
 CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,NULL,NULL)==MP_INVALID);
 CHECK(used==0);
 memset(pw,'A',32);CHECK(mp_crypto_derive(c,pw,32,salt,16,key,NULL,NULL)==MP_CRYPTO_ONLY);
 CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,NULL,NULL)==MP_CRYPTO_ONLY);
 CHECK(used==n&&!memcmp(out,plain,n));
 CHECK(mp_crypto_derive(c,pw,32,salt,15,key,NULL,NULL)==MP_INVALID);
 count=1;CHECK(mp_crypto_derive(c,pw,32,salt,16,key,stop,&count)==MP_STOPPED);
 for(i=0;i<32;i++)CHECK(key[i]==0);
 count=2;CHECK(mp_crypto_derive(c,pw,32,salt,16,key,stop,&count)==MP_STOPPED);
 for(i=0;i<32;i++)CHECK(key[i]==0);
 CHECK(mp_password_normalize((const unsigned char *)"aaaa-aaaa aaaa-aaaa aaaa-aaaa aaaa-aaaa",39,wrong)==MP_CRYPTO_ONLY);
 CHECK(!memcmp(pw,wrong,32));wrong[0]='0';CHECK(mp_password_normalize(wrong,32,pw)==MP_INVALID);
 hex("808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f",key);
 hex("070000004041424344454647",nonce);hex("50515253c0c1c2c3c4c5c6c7",aad);
 CHECK(mp_crypto_seal(c,key,nonce,12,aad,12,plain,n,cipher,sizeof(cipher),tag)==MP_CRYPTO_ONLY);
 CHECK(hex("d31a8d34648e60db7b86afbc53ef7ec2a4aded51296e08fea9e2b5a736ee62d63dbea45e8ca9671282fafb69da92728b1a71de0a9e060b2905d6a5b67ecd3b3692ddbd7f2d778b8c9803aee328091b58fab324e4fad675945585808b4831d7bc3ff4def08e4b7a9de576d26586cec64b6116",expected)==(int)n);
 CHECK(!memcmp(cipher,expected,n));hex("1ae10b594f09e26a7e902ecbd0600691",expected);CHECK(!memcmp(tag,expected,16));
 CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,NULL,NULL)==MP_CRYPTO_ONLY);
 CHECK(used==n&&!memcmp(out,plain,n));for(i=0;i<n;i++)CHECK(q[i]==0);
 /* Corruption of every ciphertext byte and tag byte must publish nothing. */
 for(i=0;i<n+16;i++){
  if(i<n)cipher[i]^=1;else tag[i-n]^=1;
  memset(out,0xa5,sizeof(out));memset(q,0xa5,sizeof(q));used=99;
  CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,NULL,NULL)==MP_INVALID);
  CHECK(used==0&&out[0]==0xa5&&out[n-1]==0xa5&&q[0]==0&&q[n-1]==0);
  if(i<n)cipher[i]^=1;else tag[i-n]^=1;
 }
 nonce[0]^=1;CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,NULL,NULL)==MP_INVALID);nonce[0]^=1;
 CHECK(mp_crypto_unseal(c,key,nonce,11,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,NULL,NULL)==MP_INVALID);
 key[0]^=1;CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,NULL,NULL)==MP_INVALID);key[0]^=1;
 count=2;CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),out,sizeof(out),&used,stop,&count)==MP_STOPPED);
 CHECK(used==0&&q[0]==0);
 CHECK(mp_crypto_unseal(c,key,nonce,12,aad,12,cipher,n,tag,16,q,sizeof(q),q,sizeof(q),&used,NULL,NULL)==MP_INVALID);
 phase("kdf_aead_stop_and_corruption_complete");
 cn=fixture(argv[2],cert,sizeof(cert));pn=fixture(argv[3],private_der,sizeof(private_der));on=fixture(argv[4],other,sizeof(other));
 CHECK(cn&&pn&&on);CHECK(mp_crypto_key_match(c,cert,cn,private_der,pn)==MP_CRYPTO_ONLY);
 CHECK(mp_crypto_key_match(c,cert,cn,other,on)==MP_INVALID);
 /* Historical mTLS fixture is NOT the newly accepted profile1 identity.
  * It must never pass by being an otherwise usable client certificate. */
 CHECK(mp_crypto_client_profile(c,cert,cn,cert,cn,ids,ids,ids,1792454400ULL)==MP_INVALID);
 CHECK(mp_crypto_key_match(c,cert,cn-1,private_der,pn)==MP_INVALID);
 CHECK(mp_crypto_key_match(c,cert,cn,private_der,pn-1)==MP_INVALID);
 phase("client_material_checks_complete");
 /* Public upstream RSA TEST KEY, never a delegated production signer. */
 f=fopen(argv[1],"rb");CHECK(f!=NULL);pemn=fread(pem,1,sizeof(pem),f);fclose(f);CHECK(pemn>0&&pemn<sizeof(pem));
 bio=BIO_new_mem_buf(pem,(int)pemn);CHECK(bio!=NULL);
 signer=PEM_read_bio_PrivateKey(bio,NULL,NULL,NULL);BIO_free(bio);OPENSSL_cleanse(pem,sizeof(pem));CHECK(signer!=NULL);
 sn=(size_t)i2d_PUBKEY(signer,&sp);CHECK(sn<=1024);
 memset(header,0,sizeof(header));memcpy(header,"PACCMP02",8);header[9]=2;header[11]=1;
 be32(header+16,364+sn+1);be32(header+20,1);be32(header+24,600000);be32(header+88,sn);
 CHECK(EVP_Q_digest(NULL,"SHA256",NULL,spki,sn,hash,&hn)==1&&hn==32);memcpy(header+56,hash,32);
 memset(signature,0,sizeof(signature));memset(tag,0,sizeof(tag));
 env.spki.data=spki;env.spki.size=sn;env.ciphertext.data=cipher;env.ciphertext.size=1;
 env.tag.data=tag;env.tag.size=16;env.signature.data=signature;env.signature.size=256;
 CHECK(mp_envelope_prepare(header,&env,raw,sizeof(raw),&rn,0)==MP_DATA_ONLY);
 md=EVP_MD_CTX_new();CHECK(md!=NULL);
 CHECK(EVP_DigestSignInit(md,&pc,EVP_sha256(),NULL,signer)==1);
 CHECK(EVP_PKEY_CTX_set_rsa_padding(pc,RSA_PKCS1_PADDING)>0);
 CHECK(EVP_DigestSignUpdate(md,domain,sizeof(domain))==1);
 CHECK(EVP_DigestSignUpdate(md,raw,rn-256)==1);
 CHECK(EVP_DigestSignFinal(md,raw+rn-256,&siglen)==1&&siglen==256);
 EVP_MD_CTX_free(md);EVP_PKEY_free(signer);phase("test_only_global_signing_context_complete");
 CHECK(mp_crypto_verify_package(c,raw,rn,spki,sn)==MP_CRYPTO_ONLY);
#ifdef MP_AUTH_NATIVE_INTEGRATION_TEST
#include "manual_package_validation_native_test.inc"
#endif
 for(i=0;i<rn;i++){raw[i]^=1;CHECK(mp_crypto_verify_package(c,raw,rn,spki,sn)==MP_INVALID);raw[i]^=1;}
 CHECK(mp_crypto_verify_package(c,raw,rn-1,spki,sn)==MP_INVALID);
 spki[sn-1]^=1;CHECK(mp_crypto_verify_package(c,raw,rn,spki,sn)==MP_INVALID);
 phase("package_public_signature_checks_complete");
 CHECK(mp_package_validate(raw,rn,0)==MP_CRYPTO_UNAVAILABLE);
 OPENSSL_cleanse(private_der,sizeof(private_der));OPENSSL_cleanse(other,sizeof(other));
 OPENSSL_cleanse(key,sizeof(key));OPENSSL_cleanse(native_key,sizeof(native_key));mp_crypto_close(c);
 printf("provider_test_crt_peak_bytes=%zu live_before_process_cleanup=%zu caller_main_arrays_excluded\n",peak,live);
 printf("%d assertions: REAL OFFLINE CRYPTO, PUBLIC TEST FIXTURES; no trust/profile/operational authority\n",checks);return 0;
}
