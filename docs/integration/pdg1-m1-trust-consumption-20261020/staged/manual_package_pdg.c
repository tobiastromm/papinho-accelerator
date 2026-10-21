#include "manual_package_pdg_internal.h"
#include "../issuer/issuer_core.h"
#include <string.h>
static unsigned u16(const unsigned char *p){return (unsigned)p[0]|((unsigned)p[1]<<8);}
static int stopped(const MP_AUTH_INPUT *i){return i->stopped&&i->stopped(i->stop_context);}
static int hash(void *ctx,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 out[32]){
 return mp_crypto_public_sha256(ctx,p,n,out)==MP_CRYPTO_ONLY;
}
int mp_auth_pdg_candidate(MP_AUTH_OPERATION *o,const MP_ENVELOPE *e){
 const MP_AUTH_INPUT *i=o->input;const MP_PDG_INPUT *p=i->pdg;PC_ROOT8_RESULT result;unsigned char d[32];
 if(!p)return MP_AUTH_STEP_SOURCE_ABSENT;
 if(stopped(i))return MP_AUTH_STOPPED;
 if(mp_crypto_public_sha256(i->crypto,i->package.data,i->package.size,o->pdg_package_digest)!=MP_CRYPTO_ONLY)return MP_AUTH_STEP_PROVIDER;
 if(memcmp(o->pdg_package_digest,p->package_digest,32))return MP_AUTH_STEP_INVALID;
 result=pc_root8_v3_pdg_select_data(p->root.data,p->root.size,p->listener_ordinal,&o->pdg_data,hash,i->crypto,p->data_component,p->data_context);
 if(stopped(i))return MP_AUTH_STOPPED;
 if(result==PC_ROOT8_INCOMPLETE||result==PC_ROOT8_UNSUPPORTED)return MP_AUTH_STEP_SOURCE_ABSENT;
 if(result==PC_ROOT8_HASH_FAILED)return MP_AUTH_STEP_PROVIDER;if(result!=PC_ROOT8_OK)return MP_AUTH_STEP_INVALID;
 if(u16(o->pdg_data.row+18)!=1U)return MP_AUTH_STEP_WRONG_PURPOSE;
 if(u16(o->pdg_data.row+16)!=1U||o->pdg_data.certificate_state==PAPACC_ISSUER_REVOKED)return MP_AUTH_STEP_REVOKED;
 if(mp_crypto_public_sha256(i->crypto,e->spki.data,e->spki.size,d)!=MP_CRYPTO_ONLY)return MP_AUTH_STEP_PROVIDER;
 if(memcmp(d,o->pdg_data.row+144,32))return MP_AUTH_STEP_INVALID;
 /* Even full DATA match, historical epochs, valid certificate/role and RSA
  * cannot prove live policy/CURRENT/pin or independent Browser confirmation.
  * No factual source acquired: fail closed. Do NOT turn this into STEP_OK. */
 return MP_AUTH_STEP_SOURCE_ABSENT;
}
