#ifndef PACC_LAB_STORAGE_SHA256_H
#define PACC_LAB_STORAGE_SHA256_H
#include <stddef.h>
#include "bearssl_hash.h"
/* Private LAB only. Caller owns zero-initialized context; serialized calls.
 * Inputs and Stop token/context are borrowed only during synchronous calls.
 * No saved borrow, handles, heap, worker or product authority. */
typedef const volatile unsigned *PACC_STORAGE_STOP;
typedef struct PACC_STORAGE_SHA256 {
 unsigned version,state,busy;
 br_sha256_context hash;
} PACC_STORAGE_SHA256;
enum { PACC_HASH_OK=0,PACC_HASH_INVALID=1,PACC_HASH_STATE=2,
 PACC_HASH_STOPPED=3,PACC_HASH_LIMIT=4,PACC_HASH_BUSY=5 };
enum { PACC_HASH_ACTIVE=1,PACC_HASH_DONE=2,PACC_HASH_FAILED=3,PACC_HASH_CANCELLED=4 };
#define PACC_STORAGE_HASH_CHUNK 1024U
int pacc_storage_sha_init(PACC_STORAGE_SHA256 *);
int pacc_storage_sha_update(PACC_STORAGE_SHA256 *,const void *,size_t,PACC_STORAGE_STOP,void *);
int pacc_storage_sha_final(PACC_STORAGE_SHA256 *,unsigned char *,size_t,PACC_STORAGE_STOP,void *);
typedef struct PACC_STORAGE_HASH_CALL { PACC_STORAGE_STOP stop;void *context; } PACC_STORAGE_HASH_CALL;
/* Hash callback shape matches portable storage codecs; math result only. */
int pacc_storage_hash_callback(void *,const unsigned char *,size_t,unsigned char [32]);
#endif
