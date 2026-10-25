#ifndef PAPACC_RT_OPENSSL_H
#define PAPACC_RT_OPENSSL_H
#include "roughtime.h"
/* Offline public verification adapter to the already pinned OpenSSL3.5.8.
 * Caller owns/protects explicit OSSL_LIB_CTX and4096-byte scratch through
 * synchronous return. No default context, autoload config, private operations,
 * thread, native I/O or operational trust. Not VC6/NT4-qualified. */
typedef struct PAPACC_RT_OPENSSL { void *libctx; PAPACC_U8 *scratch; PAPACC_SIZE bytes; } PAPACC_RT_OPENSSL;
int papacc_rt_openssl_hash(void *,const PAPACC_RT_BYTES *,unsigned,PAPACC_U8 *);
int papacc_rt_openssl_verify(void *,const PAPACC_U8 *,const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *);
#endif
