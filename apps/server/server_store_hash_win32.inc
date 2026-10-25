/* Existing BCrypt API only; no Win10 one-shot/pseudo-handle dependency.
 * Test-only macro substitution occurs in an isolated fixture translation unit. */
#include "server_store_hash_win32.h"
#include <string.h>
int papacc_store_hash_blocked(const PAPACC_STORE_HASH *c)
{return c&&(c->active||c->retained||c->algorithm||c->hash);}
int papacc_store_hash(PAPACC_STORE_HASH *c,const unsigned char *p,unsigned n,unsigned char out[32])
{
 NTSTATUS status;unsigned char value[32];int ok=0;
 if(!c||!out||(!p&&n)||c->stopped||c->retained||c->algorithm||c->hash||
    InterlockedCompareExchange(&c->active,1,0)!=0)return 0;
 c->operation_status=0;c->cleanup_status=0;
 status=BCryptOpenAlgorithmProvider(&c->algorithm,BCRYPT_SHA256_ALGORITHM,NULL,0);
 if(status!=0||!c->algorithm){c->operation_status=status?status:(NTSTATUS)0xc0000001L;
  if(c->algorithm)c->retained=1;goto done;}
 if(c->stopped)goto cleanup;
 status=BCryptCreateHash(c->algorithm,&c->hash,NULL,0,NULL,0,0);
 if(status!=0||!c->hash){c->operation_status=status?status:(NTSTATUS)0xc0000001L;
  /* An error with nonnull output gives no qualified object state. */
  if(c->hash){c->retained=1;goto done;}goto cleanup;}
 if(c->stopped)goto cleanup;
 status=BCryptHashData(c->hash,(PUCHAR)p,n,0);
 if(status!=0){c->operation_status=status;goto cleanup;}
 if(c->stopped)goto cleanup;
 status=BCryptFinishHash(c->hash,value,32,0);
 if(status!=0){c->operation_status=status;goto cleanup;}ok=1;
cleanup:
 if(c->hash){status=BCryptDestroyHash(c->hash);
  if(status!=0){c->cleanup_status=status;c->retained=1;ok=0;goto done;}c->hash=NULL;}
 if(c->algorithm){status=BCryptCloseAlgorithmProvider(c->algorithm,0);
  if(status!=0){c->cleanup_status=status;c->retained=1;ok=0;goto done;}c->algorithm=NULL;}
done:
 if(c->stopped&&!c->operation_status)c->operation_status=(NTSTATUS)0xc0000120L;
 if(c->retained||c->stopped)ok=0;
 if(ok)memcpy(out,value,32);
 InterlockedExchange(&c->active,0);return ok;
}
