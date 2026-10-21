#include "manual_package_validation_internal.h"
/* No public source/callback injection. These missing source bindings CANNOT
 * become trusted by caller booleans, fixture SPKI, password or enum PACKAGE.
 * Concrete policy/attestation acquisition and full approved metadata/profile
 * binding require exact existing-owner integration before positive use. */
static int native_step(void *ctx,MP_AUTH_STAGE s,MP_AUTH_OPERATION *o,const MP_ENVELOPE *e,const MP_PAYLOAD *v){
 MP_CRYPTO *c=ctx;const MP_AUTH_INPUT *i=o->input;int result;size_t used=0;
 switch(s){
 case MP_AUTH_SIGNATURE:
  result=mp_crypto_verify_signature(c,i->package.data,i->package.size);
  return result==MP_CRYPTO_ONLY?MP_AUTH_STEP_OK:result==MP_STOPPED?MP_AUTH_STOPPED:result==MP_CRYPTO_UNAVAILABLE?MP_AUTH_STEP_PROVIDER:MP_AUTH_STEP_INVALID;
 case MP_AUTH_TRUST:case MP_AUTH_RECHECK_TRUST:
  return MP_AUTH_STEP_SOURCE_ABSENT;
 case MP_AUTH_KDF:
  result=mp_crypto_derive(c,i->password.data,i->password.size,i->package.data+28,16,o->key,i->stopped,i->stop_context);
  return result==MP_CRYPTO_ONLY?MP_AUTH_STEP_OK:result==MP_STOPPED?MP_AUTH_STOPPED:result==MP_CRYPTO_UNAVAILABLE?MP_AUTH_STEP_PROVIDER:MP_AUTH_STEP_INVALID;
 case MP_AUTH_AEAD:
  result=mp_crypto_unseal(c,o->key,i->package.data+44,12,i->package.data,92+e->spki.size,
   e->ciphertext.data,e->ciphertext.size,e->tag.data,e->tag.size,
   i->quarantine,i->quarantine_capacity,i->staging,i->staging_capacity,&used,i->stopped,i->stop_context);
  if(result!=MP_CRYPTO_ONLY)return result==MP_STOPPED?MP_AUTH_STOPPED:result==MP_CRYPTO_UNAVAILABLE?MP_AUTH_STEP_PROVIDER:MP_AUTH_STEP_INVALID;o->staging_size=used;return MP_AUTH_STEP_OK;
 case MP_AUTH_CONTENT:
  if(!v||mp_crypto_key_match(c,v->member[1].data,v->member[1].size,v->member[2].data,v->member[2].size)!=MP_CRYPTO_ONLY)return MP_AUTH_STEP_INVALID;
  return MP_AUTH_STEP_SOURCE_ABSENT; /* no approved identity/time/issuer metadata context */
 case MP_AUTH_RELEASE:return MP_AUTH_STEP_OK;
 default:return MP_AUTH_STEP_INVALID;
 }
}
static void native_wipe(void *ctx,unsigned char *p,size_t n){(void)ctx;mp_crypto_wipe(p,n);}
static int native_quiescent(void *ctx){(void)ctx;return 1;} /* current calls synchronous; no source handle acquired */
const MP_AUTH_PORT mp_auth_native_port={native_step,native_wipe,native_quiescent};
