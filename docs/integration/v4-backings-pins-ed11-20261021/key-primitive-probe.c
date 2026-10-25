#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <string.h>
#include <stdio.h>
int main(void){OSSL_LIB_CTX *lib;EVP_PKEY *key;EVP_PKEY_CTX *ctx;unsigned char raw[32];int i,check; if(OpenSSL_version_num()!=0x30500080UL)return 2;lib=OSSL_LIB_CTX_new();if(!lib)return 3;
for(i=0;i<3;i++){memset(raw,i==1?255:0,32);if(i==2)raw[0]=1;key=EVP_PKEY_new_raw_public_key_ex(lib,"ED25519","provider=default",raw,32);ctx=key?EVP_PKEY_CTX_new_from_pkey(lib,key,"provider=default"):NULL;check=ctx?EVP_PKEY_public_check(ctx):-99;printf("PUBLIC_BYTES_CASE=%d IMPORT=%d PUBLIC_CHECK=%d NO_KEYGEN_NO_SIGNATURE_NO_AUTHORITY\n",i,key!=NULL,check);EVP_PKEY_CTX_free(ctx);EVP_PKEY_free(key);}OSSL_LIB_CTX_free(lib);return 0;}
