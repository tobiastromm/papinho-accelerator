#include "suite.h"
static const unsigned char sp_domain[]="PAPACC-COMPLETE-MANUAL-V2";
SP_RESULT sp_suite1_kdf_shape(SP_SLICE password,SP_SLICE salt,unsigned long rounds)
{
 size_t i;
 if(!password.data || password.size!=SP_SUITE1_KEY_BYTES || !salt.data ||
    salt.size!=SP_SUITE1_SALT_BYTES || rounds!=SP_SUITE1_ITERATIONS)return SP_INVALID;
 for(i=0;i<password.size;++i)if(!((password.data[i]>='A' && password.data[i]<='Z') ||
    (password.data[i]>='2' && password.data[i]<='7')))return SP_INVALID;
 return SP_DATA_ONLY;
}
SP_RESULT sp_suite1_aead_shape(size_t key,size_t nonce,size_t aad,size_t cipher,size_t tag)
{
 if(key!=SP_SUITE1_KEY_BYTES || nonce!=SP_SUITE1_NONCE_BYTES ||
    aad>SP_SUITE1_AAD_MAX || !cipher || cipher>SP_SUITE1_CIPHER_MAX ||
    tag!=SP_SUITE1_TAG_BYTES)return SP_INVALID;
 return SP_DATA_ONLY;
}
SP_RESULT sp_suite1_signed_parts(const unsigned char *p,size_t n,
 SP_SIGNED_PARTS *out,SP_STOP stop,void *ctx)
{
 SP_ENVELOPE e;SP_SIGNED_PARTS v;SP_RESULT result;size_t a,b,limit=(size_t)-1;
 if(!p || !out || !n || n>SP_INPUT_MAX)return SP_INVALID;
 /* Same reviewed flat-address precondition as structure.c; no pointer order. */
 a=(size_t)p;b=(size_t)out;
 if(a>limit-n || b>limit-sizeof(*out) ||
    (a<b+sizeof(*out) && b<a+n))return SP_INVALID;
 result=sp_envelope_inspect(p,n,&e,stop,ctx);
 if(result!=SP_DATA_ONLY)return result;
 v.domain.data=sp_domain;v.domain.size=sizeof(sp_domain);
 v.prefix.data=p;v.prefix.size=n-SP_SUITE1_SIGNATURE_BYTES;
 v.spki=e.spki;v.signature=e.signature;
 if(stop && stop(ctx))return SP_STOPPED;
 *out=v;return SP_DATA_ONLY;
}
