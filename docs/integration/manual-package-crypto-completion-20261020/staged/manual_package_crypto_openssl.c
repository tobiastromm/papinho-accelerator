#include "manual_package_crypto.h"
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/core_names.h>
#include <openssl/params.h>
#include <openssl/provider.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/x509v3.h>
#include <time.h>
struct MP_CRYPTO { OSSL_LIB_CTX *lib; OSSL_PROVIDER *provider; };
static int canonical_cert(X509 *,const unsigned char *,size_t);
static int cancelled(MP_CANCEL f,void *u){return f && f(u);}
static int overlap(const void *a,size_t n,const void *b,size_t m){
 uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
 if(!n||!m)return 0;
 if(x>UINTPTR_MAX-n||y>UINTPTR_MAX-m)return 1;
 return x<y+m&&y<x+n;
}
MP_CRYPTO *mp_crypto_open(void){
 MP_CRYPTO *c;
 if(OpenSSL_version_num()!=0x30500080UL)return NULL;
 c=OPENSSL_zalloc(sizeof(*c));if(!c)return NULL;
 c->lib=OSSL_LIB_CTX_new();
 if(c->lib)c->provider=OSSL_PROVIDER_load(c->lib,"default");
 if(!c->provider){mp_crypto_close(c);return NULL;}return c;
}
void mp_crypto_close(MP_CRYPTO *c){if(c){OSSL_PROVIDER_unload(c->provider);
 OSSL_LIB_CTX_free(c->lib);OPENSSL_clear_free(c,sizeof(*c));}}
int mp_password_normalize(const unsigned char *p,size_t n,unsigned char out[32]){
 unsigned char tmp[32];size_t i,j=0;int ok=1;
 if(!p||!out||n>128||overlap(p,n,out,32))return MP_INVALID;
 memset(tmp,0,sizeof(tmp));
 for(i=0;i<n;i++){unsigned char v=p[i];if(v==' '||v=='-')continue;
 if(v>='a'&&v<='z')v=(unsigned char)(v-'a'+'A');
 if(!((v>='A'&&v<='Z')||(v>='2'&&v<='7'))||j>=32){ok=0;break;}tmp[j++]=v;}
 if(j!=32)ok=0;if(ok)memcpy(out,tmp,32);else OPENSSL_cleanse(out,32);
 OPENSSL_cleanse(tmp,sizeof(tmp));return ok?MP_CRYPTO_ONLY:MP_INVALID;
}
int mp_crypto_derive(MP_CRYPTO *c,const unsigned char *p,size_t pn,
 const unsigned char *s,size_t sn,unsigned char out[32],MP_CANCEL stop,void *u){
 EVP_KDF *k=NULL;EVP_KDF_CTX *x=NULL;OSSL_PARAM a[6];
 unsigned char pw[32],key[32];unsigned int iterations=600000;int ok=0,pkcs5=0;
 if(!c||!p||!s||!out||sn!=16||overlap(out,32,p,pn)||overlap(out,32,s,sn))return MP_INVALID;
 memset(pw,0,32);memset(key,0,32);OPENSSL_cleanse(out,32);
 if(cancelled(stop,u)){ok=MP_STOPPED;goto done;}
 if(mp_password_normalize(p,pn,pw)!=MP_CRYPTO_ONLY){ok=MP_INVALID;goto done;}
 k=EVP_KDF_fetch(c->lib,"PBKDF2","provider=default");if(!k)goto done;
 x=EVP_KDF_CTX_new(k);if(!x)goto done;
 a[0]=OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_PASSWORD,pw,32);
 a[1]=OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT,(void *)s,16);
 a[2]=OSSL_PARAM_construct_uint(OSSL_KDF_PARAM_ITER,&iterations);
 a[3]=OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST,"SHA256",0);
 a[4]=OSSL_PARAM_construct_int(OSSL_KDF_PARAM_PKCS5,&pkcs5);a[5]=OSSL_PARAM_construct_end();
 if(EVP_KDF_derive(x,key,32,a)!=1)goto done;
 if(cancelled(stop,u)){ok=MP_STOPPED;goto done;}
 memcpy(out,key,32);ok=MP_CRYPTO_ONLY;
 done:OPENSSL_cleanse(pw,32);OPENSSL_cleanse(key,32);
 EVP_KDF_CTX_free(x);EVP_KDF_free(k);return ok?ok:MP_CRYPTO_UNAVAILABLE;
}
int mp_crypto_seal(MP_CRYPTO *c,const unsigned char key[32],const unsigned char *nonce,size_t nn,
 const unsigned char *aad,size_t an,const unsigned char *plain,size_t pn,
 unsigned char *out,size_t cap,unsigned char tag[16]){
 EVP_CIPHER *alg=NULL;EVP_CIPHER_CTX *x=NULL;int n=0,t=0,ok=0;
 if(!c||!key||!nonce||nn!=12||!aad||an>1116||!plain||!pn||pn>65536||!out||cap<pn||!tag||
 overlap(out,pn,plain,pn)||overlap(out,pn,key,32)||overlap(out,pn,nonce,nn)||
 overlap(out,pn,aad,an)||overlap(out,pn,tag,16)||overlap(tag,16,key,32)||
 overlap(tag,16,plain,pn)||overlap(tag,16,nonce,nn)||overlap(tag,16,aad,an))return MP_INVALID;
 alg=EVP_CIPHER_fetch(c->lib,"CHACHA20-POLY1305","provider=default");x=EVP_CIPHER_CTX_new();
 if(!alg||!x)goto done;
 if(EVP_EncryptInit_ex2(x,alg,key,nonce,NULL)!=1||EVP_EncryptUpdate(x,NULL,&n,aad,(int)an)!=1||
 EVP_EncryptUpdate(x,out,&n,plain,(int)pn)!=1||n!=(int)pn||
 EVP_EncryptFinal_ex(x,out+n,&t)!=1||t!=0||
 EVP_CIPHER_CTX_ctrl(x,EVP_CTRL_AEAD_GET_TAG,16,tag)!=1)goto done;ok=1;
 done:if(!ok){OPENSSL_cleanse(out,pn);OPENSSL_cleanse(tag,16);}
 EVP_CIPHER_CTX_free(x);EVP_CIPHER_free(alg);return ok?MP_CRYPTO_ONLY:MP_CRYPTO_UNAVAILABLE;
}
int mp_crypto_unseal(MP_CRYPTO *c,const unsigned char key[32],const unsigned char *nonce,size_t nn,
 const unsigned char *aad,size_t an,const unsigned char *cipher,size_t cn,const unsigned char *tag,size_t tn,
 unsigned char *q,size_t qc,unsigned char *out,size_t oc,size_t *used,MP_CANCEL stop,void *u){
 EVP_CIPHER *alg=NULL;EVP_CIPHER_CTX *x=NULL;int n=0,t=0,ok=MP_INVALID;
 if(!c||!key||!nonce||nn!=12||!aad||an>1116||!cipher||!cn||cn>65536||!tag||tn!=16||
 !q||qc<cn||!out||oc<cn||!used||overlap(q,cn,out,cn)||
 overlap(q,cn,key,32)||overlap(q,cn,nonce,nn)||overlap(q,cn,aad,an)||overlap(q,cn,cipher,cn)||overlap(q,cn,tag,tn)||
 overlap(out,cn,key,32)||overlap(out,cn,nonce,nn)||overlap(out,cn,aad,an)||overlap(out,cn,cipher,cn)||overlap(out,cn,tag,tn)||
 overlap(used,sizeof(*used),out,cn)||overlap(used,sizeof(*used),q,cn)||
 overlap(used,sizeof(*used),key,32)||overlap(used,sizeof(*used),cipher,cn)||
 overlap(used,sizeof(*used),nonce,nn)||overlap(used,sizeof(*used),aad,an)||overlap(used,sizeof(*used),tag,tn))return MP_INVALID;
 *used=0;if(cancelled(stop,u)){ok=MP_STOPPED;goto done;}
 ok=MP_CRYPTO_UNAVAILABLE;
 alg=EVP_CIPHER_fetch(c->lib,"CHACHA20-POLY1305","provider=default");x=EVP_CIPHER_CTX_new();
 if(!alg||!x)goto done;
 if(EVP_DecryptInit_ex2(x,alg,key,nonce,NULL)!=1||EVP_DecryptUpdate(x,NULL,&n,aad,(int)an)!=1||
 EVP_DecryptUpdate(x,q,&n,cipher,(int)cn)!=1||n!=(int)cn||
 EVP_CIPHER_CTX_ctrl(x,EVP_CTRL_AEAD_SET_TAG,16,(void *)tag)!=1)goto done;
 ok=MP_INVALID;if(EVP_DecryptFinal_ex(x,q+n,&t)!=1||t!=0)goto done;
 if(cancelled(stop,u)){ok=MP_STOPPED;goto done;}
 memcpy(out,q,cn);*used=cn;ok=MP_CRYPTO_ONLY;
 done:OPENSSL_cleanse(q,cn);EVP_CIPHER_CTX_free(x);EVP_CIPHER_free(alg);return ok;
}
static int rsa_profile(EVP_PKEY *p){BIGNUM *e=NULL;int ok;
 if(!p||!EVP_PKEY_is_a(p,"RSA")||EVP_PKEY_get_bits(p)!=2048)return 0;
 ok=EVP_PKEY_get_bn_param(p,OSSL_PKEY_PARAM_RSA_E,&e)==1&&BN_is_word(e,65537);
 BN_free(e);return ok;
}
int mp_crypto_verify_package(MP_CRYPTO *c,const unsigned char *raw,size_t rn,
 const unsigned char *approved,size_t sn){
 MP_ENVELOPE env;const unsigned char *p;EVP_PKEY *key=NULL;EVP_MD_CTX *md=NULL;
 EVP_PKEY_CTX *pc=NULL;unsigned char digest[32];size_t dn=0;int ok=0;
 static const unsigned char domain[]="PAPACC-COMPLETE-MANUAL-V2";
 if(!c||!approved||!sn||sn>1024||mp_envelope_inspect(raw,rn,&env,0)!=MP_DATA_ONLY)return MP_INVALID;
 if(env.spki.size!=sn||CRYPTO_memcmp(approved,env.spki.data,sn))return MP_INVALID;
 if(EVP_Q_digest(c->lib,"SHA256","provider=default",approved,sn,digest,&dn)!=1||dn!=32||
 CRYPTO_memcmp(digest,raw+56,32))return MP_INVALID;
 p=approved;key=d2i_PUBKEY_ex(NULL,&p,(long)sn,c->lib,"provider=default");
 if(!key||p!=approved+sn||!rsa_profile(key))goto done;
 {unsigned char *canonical=NULL;int len=i2d_PUBKEY(key,&canonical);
  int exact=len>0&&(size_t)len==sn&&!CRYPTO_memcmp(canonical,approved,sn);
  OPENSSL_free(canonical);if(!exact)goto done;}
 md=EVP_MD_CTX_new();if(!md)goto done;
 if(EVP_DigestVerifyInit_ex(md,&pc,"SHA256",c->lib,"provider=default",key,NULL)!=1||
 EVP_PKEY_CTX_set_rsa_padding(pc,RSA_PKCS1_PADDING)<=0||
 EVP_DigestVerifyUpdate(md,domain,sizeof(domain))!=1||
 EVP_DigestVerifyUpdate(md,raw,rn-256)!=1)goto done;
 ok=EVP_DigestVerifyFinal(md,env.signature.data,env.signature.size)==1;
 done:EVP_MD_CTX_free(md);EVP_PKEY_free(key);return ok?MP_CRYPTO_ONLY:MP_INVALID;
}
int mp_crypto_key_match(MP_CRYPTO *c,const unsigned char *cert,size_t cn,
 const unsigned char *private_der,size_t pn){
 const unsigned char *a=cert,*b=private_der;X509 *x=NULL;PKCS8_PRIV_KEY_INFO *info=NULL;
 EVP_PKEY *pub=NULL,*key=NULL;int ok=0;
 if(!c||!cert||!cn||cn>16384||!private_der||!pn||pn>8192)return MP_INVALID;
 x=X509_new_ex(c->lib,"provider=default");if(!x)goto done;
 if(!d2i_X509(&x,&a,(long)cn)||a!=cert+cn||!canonical_cert(x,cert,cn))goto done;
 /* PKCS8v0 only: canonical outer DER length and exact version INTEGER 0. */
 {size_t h=2,len;if(pn<5||private_der[0]!=0x30)goto done;
  if(private_der[1]<128)len=private_der[1];
  else if(private_der[1]==0x81&&pn>=6&&private_der[2]>=128){h=3;len=private_der[2];}
  else if(private_der[1]==0x82&&pn>=7&&private_der[2]){h=4;len=(size_t)private_der[2]*256+private_der[3];}
  else goto done;
  if(len!=pn-h||len<3||private_der[h]!=2||private_der[h+1]!=1||private_der[h+2]!=0)goto done;}
 info=d2i_PKCS8_PRIV_KEY_INFO(NULL,&b,(long)pn);if(!info||b!=private_der+pn)goto done;
 {unsigned char *canonical=NULL;int len=i2d_PKCS8_PRIV_KEY_INFO(info,&canonical);
  int exact=len>0&&(size_t)len==pn&&!CRYPTO_memcmp(canonical,private_der,pn);
  OPENSSL_clear_free(canonical,len>0?(size_t)len:0);if(!exact)goto done;}
 key=EVP_PKCS82PKEY_ex(info,c->lib,"provider=default");pub=X509_get_pubkey(x);
 if(!rsa_profile(key)||!rsa_profile(pub))goto done;
 ok=EVP_PKEY_eq(pub,key)==1;
 done:EVP_PKEY_free(pub);EVP_PKEY_free(key);PKCS8_PRIV_KEY_INFO_free(info);X509_free(x);
 return ok?MP_CRYPTO_ONLY:MP_INVALID;
}

static int canonical_cert(X509 *x,const unsigned char *der,size_t n){
 unsigned char *b=NULL;int len,ok;
 if(i2d_re_X509_tbs(x,NULL)<=0)return 0;len=i2d_X509(x,&b);
 ok=len>0&&(size_t)len==n&&CRYPTO_memcmp(b,der,n)==0;OPENSSL_free(b);return ok;
}
static int cn_matches(X509 *x,const char *prefix,const unsigned char id[16]){
 static const char digits[]="0123456789abcdef";char expected[80];size_t n=strlen(prefix),i;
 X509_NAME *name=X509_get_subject_name(x);X509_NAME_ENTRY *entry;ASN1_STRING *value;int at;
 if(n+32>=sizeof(expected))return 0;memcpy(expected,prefix,n);
 for(i=0;i<16;i++){expected[n+2*i]=digits[id[i]>>4];expected[n+2*i+1]=digits[id[i]&15];}
 at=X509_NAME_get_index_by_NID(name,NID_commonName,-1);if(at<0||X509_NAME_get_index_by_NID(name,NID_commonName,at)>=0)return 0;
 entry=X509_NAME_get_entry(name,at);value=X509_NAME_ENTRY_get_data(entry);
 return ASN1_STRING_length(value)==(int)(n+32)&&CRYPTO_memcmp(ASN1_STRING_get0_data(value),expected,n+32)==0;
}
static int cert_algorithm(X509 *x){
 return X509_get_version(x)==2&&X509_get_signature_nid(x)==NID_sha256WithRSAEncryption;
}
static int constraints(X509 *x,int ca){
 BASIC_CONSTRAINTS *bc=NULL;ASN1_BIT_STRING *ku=NULL;EXTENDED_KEY_USAGE *eku=NULL;
 int critical=-1,ok=0,i;bc=X509_get_ext_d2i(x,NID_basic_constraints,&critical,NULL);
 if(!bc||critical!=1||((bc->ca!=0)!=(ca!=0)))goto done;
 if(ca){if(!bc->pathlen||ASN1_INTEGER_get(bc->pathlen)!=0)goto done;}
 else if(bc->pathlen)goto done;
 critical=-1;ku=X509_get_ext_d2i(x,NID_key_usage,&critical,NULL);
 if(!ku||critical!=1)goto done;
 for(i=0;i<ASN1_STRING_length(ku)*8;i++){
  int expected=ca?(i==5||i==6):(i==0);
  if(ASN1_BIT_STRING_get_bit(ku,i)!=expected)goto done;
 }
 if(ASN1_STRING_length(ku)<1)goto done;
 critical=-1;eku=X509_get_ext_d2i(x,NID_ext_key_usage,&critical,NULL);
 if(ca){if(eku||critical!=-1)goto done;}
 else if(!eku||critical!=0||sk_ASN1_OBJECT_num(eku)!=1||OBJ_obj2nid(sk_ASN1_OBJECT_value(eku,0))!=NID_client_auth)goto done;
 /* No unknown critical semantics may be ignored. */
 for(i=0;i<X509_get_ext_count(x);i++){
  X509_EXTENSION *e=X509_get_ext(x,i);int nid=OBJ_obj2nid(X509_EXTENSION_get_object(e));
  if(X509_EXTENSION_get_critical(e)&&nid!=NID_basic_constraints&&nid!=NID_key_usage)goto done;
 }
 ok=1;
 done:BASIC_CONSTRAINTS_free(bc);ASN1_BIT_STRING_free(ku);EXTENDED_KEY_USAGE_free(eku);return ok;
}
int mp_crypto_client_profile(MP_CRYPTO *c,const unsigned char *cert,size_t cn,
 const unsigned char *approved_ca,size_t an,const unsigned char device[16],const unsigned char authority[16],
 const unsigned char serial[16],unsigned long long utc){
 const unsigned char *p;X509 *leaf=NULL,*ca=NULL;EVP_PKEY *lk=NULL,*ck=NULL;
 ASN1_OCTET_STRING *ski=NULL,*lski=NULL;AUTHORITY_KEYID *aki=NULL;BIGNUM *bn=NULL;
 unsigned char serial_bytes[16];int days=0,seconds=0,critical=-1,ok=0;time_t now;
 if(!c||!cert||!cn||cn>16384||!approved_ca||!an||an>16384||!device||!authority||!serial||!utc)return MP_INVALID;
 now=(time_t)utc;if(now<0||(unsigned long long)now!=utc)return MP_INVALID;
 leaf=X509_new_ex(c->lib,"provider=default");ca=X509_new_ex(c->lib,"provider=default");if(!leaf||!ca)goto done;
 p=cert;if(!d2i_X509(&leaf,&p,(long)cn)||p!=cert+cn||!canonical_cert(leaf,cert,cn))goto done;
 p=approved_ca;if(!d2i_X509(&ca,&p,(long)an)||p!=approved_ca+an||!canonical_cert(ca,approved_ca,an))goto done;
 if(!cert_algorithm(leaf)||!cert_algorithm(ca)||!constraints(leaf,0)||!constraints(ca,1)||
 !cn_matches(leaf,"Papinho Client ",device)||!cn_matches(ca,"Papinho CA ",authority))goto done;
 lk=X509_get_pubkey(leaf);ck=X509_get_pubkey(ca);
 if(!rsa_profile(lk)||!rsa_profile(ck)||EVP_PKEY_eq(lk,ck)!=0||
 X509_NAME_cmp(X509_get_subject_name(ca),X509_get_issuer_name(ca))||
 X509_NAME_cmp(X509_get_issuer_name(leaf),X509_get_subject_name(ca))||
 X509_verify(ca,ck)!=1||X509_verify(leaf,ck)!=1)goto done;
 if(X509_cmp_time(X509_get0_notBefore(ca),&now)!=-1||X509_cmp_time(X509_get0_notAfter(ca),&now)!=1||
 X509_cmp_time(X509_get0_notBefore(leaf),&now)!=-1||X509_cmp_time(X509_get0_notAfter(leaf),&now)!=1)goto done;
 if(!ASN1_TIME_diff(&days,&seconds,X509_get0_notBefore(ca),X509_get0_notAfter(ca))||days!=1825||seconds!=0)goto done;
 if(!ASN1_TIME_diff(&days,&seconds,X509_get0_notBefore(leaf),X509_get0_notAfter(leaf))||days<0||days>90||seconds<0||
 (days==0&&seconds==0)||(days==90&&seconds!=0))goto done;
 if(ASN1_TIME_compare(X509_get0_notBefore(leaf),X509_get0_notBefore(ca))<0||
 ASN1_TIME_compare(X509_get0_notAfter(leaf),X509_get0_notAfter(ca))>0)goto done;
 bn=ASN1_INTEGER_to_BN(X509_get0_serialNumber(leaf),NULL);
 if(!bn||BN_is_negative(bn)||BN_is_zero(bn)||BN_num_bytes(bn)>16||BN_bn2binpad(bn,serial_bytes,16)!=16||
 CRYPTO_memcmp(serial_bytes,serial,16))goto done;
 ski=X509_get_ext_d2i(ca,NID_subject_key_identifier,&critical,NULL);if(!ski||critical<0)goto done;
 critical=-1;lski=X509_get_ext_d2i(leaf,NID_subject_key_identifier,&critical,NULL);if(!lski||critical<0)goto done;
 critical=-1;aki=X509_get_ext_d2i(leaf,NID_authority_key_identifier,&critical,NULL);
 if(!aki||critical<0||!aki->keyid||ASN1_STRING_length(ski)<=0||ASN1_STRING_length(lski)<=0||
 ASN1_STRING_cmp(ski,aki->keyid))goto done;
 ok=1;
 done:BN_free(bn);AUTHORITY_KEYID_free(aki);ASN1_OCTET_STRING_free(ski);ASN1_OCTET_STRING_free(lski);
 EVP_PKEY_free(lk);EVP_PKEY_free(ck);X509_free(leaf);X509_free(ca);return ok?MP_CRYPTO_ONLY:MP_INVALID;
}
