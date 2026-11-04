#include "personal_machine_fixture_win32.h"
#ifndef PAPACC_MACHINE_FIXTURE_DATA_HASH
#define PAPACC_MACHINE_FIXTURE_DATA_HASH papacc_store_hash
#endif
/* Same CA key and hash owners; no callbacks, signer, new process or channel.
 * A future native fixture act must precede key creation; this helper cannot
 * create/adopt a key or turn its public digest into authority. */
static DWORD scqkf_identity_fail(PAPACC_MACHINE_FIXTURE_CONTROL *f,DWORD stage,DWORD e)
{if(!f->first){f->first=e?e:ERROR_INVALID_DATA;f->stage=stage;}f->phase=3U;return f->first;}
static DWORD scqkf_identity_core(PAPACC_MACHINE_FIXTURE_CONTROL *f,const void *owner,volatile LONG *stop)
{
 SECURITY_STATUS s;BYTE digest[32];
 if(!f||!owner||!stop||f->self||f->phase||f->used||f->hash.active||f->hash.retained||f->hash.hash||f->hash.algorithm||
  f->key.thread!=GetCurrentThreadId()||f->key.state!=PAPACC_CA_KEY_READY||!f->key.diagnostic_only||(!f->key.created&&!f->key.read_session)||
  !f->key.provider||!f->key.key||f->key.borrower||f->key.close_attempted||f->key.diagnostic_control_attempted)return ERROR_ACCESS_DENIED;
 f->self=f;f->owner=owner;f->thread=GetCurrentThreadId();f->phase=1U;
 if(InterlockedCompareExchange(&f->used,1,0))return scqkf_identity_fail(f,1U,ERROR_ALREADY_EXISTS);
 f->original_key=f->key.key;f->original_provider=f->key.provider;
 if(*stop||f->key.diagnostic_stop!=stop)return scqkf_identity_fail(f,1U,ERROR_CANCELLED);
 s=papacc_ca_key_fixture_control(&f->key,f->public_blob,sizeof(f->public_blob),&f->public_bytes);
 if(s)return scqkf_identity_fail(f,2U,(DWORD)s);
 if(*stop||f->key.key!=f->original_key||f->key.provider!=f->original_provider||f->public_bytes!=sizeof(f->public_blob)||
  !f->key.diagnostic_control_confirmed||f->key.diagnostic_reopen)return scqkf_identity_fail(f,3U,ERROR_INVALID_STATE);
 if(!PAPACC_MACHINE_FIXTURE_DATA_HASH(&f->hash,f->public_blob,f->public_bytes,digest))return scqkf_identity_fail(f,4U,ERROR_INVALID_DATA);
 if(*stop||papacc_store_hash_blocked(&f->hash))return scqkf_identity_fail(f,5U,ERROR_CANCELLED);
 if(f->key.key!=f->original_key||f->key.provider!=f->original_provider)return scqkf_identity_fail(f,5U,ERROR_REVISION_MISMATCH);
 s=papacc_ca_key_validate(&f->key);if(s)return scqkf_identity_fail(f,6U,(DWORD)s);
 if(*stop||f->key.key!=f->original_key||f->key.provider!=f->original_provider)return scqkf_identity_fail(f,6U,ERROR_CANCELLED);
 memcpy(f->public_id,digest,32);f->phase=2U;return 0; /* identity DATA only */
}
static DWORD scqkf_identity(PAPACC_MACHINE_FIXTURE_CONTROL *f,const void *owner,volatile LONG *stop)
{if(!f||!f->key.created||f->key.read_session)return ERROR_ACCESS_DENIED;return scqkf_identity_core(f,owner,stop);}
static DWORD scqkf_read_identity(PAPACC_MACHINE_FIXTURE_CONTROL *f,const void *owner,const struct PAPACC_SCQ_READ_DIAGNOSTIC *k,volatile LONG *stop)
{if(!f||f->key.created||!f->key.read_session||f->key.read_act!=k||f->key.read_owner!=owner)return ERROR_ACCESS_DENIED;return scqkf_identity_core(f,owner,stop);}
static DWORD scqkf_identity_current(PAPACC_MACHINE_FIXTURE_CONTROL *f,const void *owner,volatile LONG *stop)
{
 SECURITY_STATUS s;
 if(!f||!owner||!stop||f->self!=f||f->owner!=owner||f->thread!=GetCurrentThreadId()||f->phase!=2U||f->first||!f->used||
  *stop||f->key.diagnostic_stop!=stop||!f->key.diagnostic_only||f->key.state!=PAPACC_CA_KEY_READY||f->key.close_attempted||
  !f->key.diagnostic_control_confirmed||f->key.diagnostic_reopen||f->key.key!=f->original_key||f->key.provider!=f->original_provider||
  papacc_store_hash_blocked(&f->hash))return ERROR_ACCESS_DENIED;
 s=papacc_ca_key_validate(&f->key);return s?scqkf_identity_fail(f,6U,(DWORD)s):0;
}

static DWORD scqkf_close(PAPACC_MACHINE_FIXTURE_CONTROL *f,const void *owner)
{
 SECURITY_STATUS s;
 if(!f||!owner)return ERROR_INVALID_PARAMETER;
 if(!f->key.diagnostic_only&&!f->self&&!f->phase&&!f->hash.active&&!f->hash.retained&&!f->hash.hash&&!f->hash.algorithm)return 0;
 if(f->self&&(f->self!=f||f->owner!=owner||f->thread!=GetCurrentThreadId()))return ERROR_ACCESS_DENIED;
 if(papacc_store_hash_blocked(&f->hash)||f->key.state==PAPACC_CA_KEY_RETAINED)return ERROR_IO_INCOMPLETE;
 if(f->key.diagnostic_only&&f->key.state==PAPACC_CA_KEY_EMPTY&&!f->key.thread&&!f->key.provider&&!f->key.key&&!f->key.diagnostic_reopen&&
  !f->key.retained_token&&!f->key.retained_local[0]&&!f->key.retained_local[1]&&!f->key.retained_local[2]&&!f->key.created){f->phase=4U;return 0;}

 if(!f->key.diagnostic_only||f->key.thread!=GetCurrentThreadId())return ERROR_ACCESS_DENIED;
 s=papacc_ca_key_close(&f->key);if(s)return scqkf_identity_fail(f,7U,(DWORD)s);
 f->phase=4U;return 0; /* persistent fixture remains; no delete/rollback */
}

#undef PAPACC_MACHINE_FIXTURE_DATA_HASH
