/* Private deterministic storage DATA. Existing context remains the sole owner.
 * Old uncertain BCrypt references are neither reset nor released here. */
#include "../../tools/qualification/storage-hash-capsule/storage_sha256.h"
static int store_data_hash(PAPACC_STORE_HASH *c,const unsigned char *p,unsigned n,unsigned char out[32])
{
 PACC_STORAGE_SHA256 hash;unsigned char value[32];unsigned left=n,chunk;int ok=0;
 if(!c||!out||(!p&&n)||(n&&(ULONG_PTR)p>MAXULONG_PTR-(n-1U))||
 (ULONG_PTR)out>MAXULONG_PTR-31U||
 ((ULONG_PTR)out<=(ULONG_PTR)c?(ULONG_PTR)c-(ULONG_PTR)out<32U:(ULONG_PTR)out-(ULONG_PTR)c<sizeof(*c))||c->stopped||c->retained||c->algorithm||c->hash||
 InterlockedCompareExchange(&c->active,1,0)!=0)return 0;
 memset(&hash,0,sizeof(hash));
 if(pacc_storage_sha_init(&hash)!=PACC_HASH_OK)goto done;
 while(left){
  if(c->stopped)goto done;
  chunk=left>PACC_STORAGE_HASH_CHUNK?PACC_STORAGE_HASH_CHUNK:left;
  if(pacc_storage_sha_update(&hash,p,chunk,NULL,NULL)!=PACC_HASH_OK)goto done;
  p+=chunk;left-=chunk;
 }
 if(c->stopped||pacc_storage_sha_final(&hash,value,sizeof(value),NULL,NULL)!=PACC_HASH_OK||c->stopped)goto done;
 memcpy(out,value,32);ok=1;
done:
 /* Synchronous quiescence: no input/output/native borrow survives. Preserve
  * prior native statuses/references and the monotonic Stop flag. */
 memset(&hash,0,sizeof(hash));memset(value,0,sizeof(value));
 InterlockedExchange(&c->active,0);return ok;
}
static int storage_digest(PAPACC_STORE_HASH *c,unsigned char *record,int verify)
{
 unsigned char value[32];
 if(!store_data_hash(c,record,480U,value))return 0;
 if(verify)return memcmp(value,record+480U,32U)==0;
 memcpy(record+480U,value,32U);return 1;
}
int papacc_configuration_storage_hash(void *ctx,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 out[32])
{
 PAPACC_CONFIGURATION_COMMIT *o=ctx;
 if(!owner(o)||o->stopped||o->unresolved||o->authority_lost||!p||!out||n>ULONG_MAX||
 papacc_store_hash_blocked(&o->store_hash)||o->issuer_hash||o->issuer_hash_alg)return 0;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 if(papacc_store_hash_blocked(&o->pending_commit.store_hash))return 0;
#endif
 return store_data_hash(&o->store_hash,p,(unsigned)n,out);
}
