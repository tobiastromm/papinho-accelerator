#ifndef PAPACC_PRIVATE_STORE_HASH_WIN32_H
#define PAPACC_PRIVATE_STORE_HASH_WIN32_H
#include <windows.h>
#include <bcrypt.h>
/* Embedded in the existing owner/worker packet or explicit codec caller.
 * Zero-init once. No copy/reset/free while active or retained. No retry API.
 * Success is published only after factual hash/provider cleanup success.
 * References retained on uncertain cleanup stay owned; byte costs UNKNOWN. */
typedef struct PAPACC_STORE_HASH {
 BCRYPT_ALG_HANDLE algorithm;
 BCRYPT_HASH_HANDLE hash;
 NTSTATUS operation_status,cleanup_status;
 volatile LONG active,retained,stopped;
} PAPACC_STORE_HASH;
int papacc_store_hash(PAPACC_STORE_HASH *,const unsigned char *,unsigned,unsigned char [32]);
int papacc_store_hash_blocked(const PAPACC_STORE_HASH *);
#endif
