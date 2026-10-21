#ifndef PAPACC_MANUAL_PACKAGE_CRYPTO_H
#define PAPACC_MANUAL_PACKAGE_CRYPTO_H
#include "manual_package.h"
/* OFFLINE ONLY: crypto success is not trust, X509 validity, RESOLVED or CURRENT.
 * Caller serializes each operation; all buffers are disjoint and immutable
 * inputs remain owned until return. Stop callback may observe a synchronized
 * flag, never frees/reenters. PBKDF2 is synchronous and not interruptible inside
 * the provider: never call on an owner Stop path. No production linkage.
 * Provider allocations are opaque/unqualified; runtime activation is denied. */
typedef struct MP_CRYPTO MP_CRYPTO;
typedef int (*MP_CANCEL)(void *);
#define MP_CRYPTO_ONLY 2
MP_CRYPTO *mp_crypto_open(void);
void mp_crypto_close(MP_CRYPTO *);
int mp_password_normalize(const unsigned char *,size_t,unsigned char [32]);
int mp_crypto_derive(MP_CRYPTO *,const unsigned char *,size_t,
 const unsigned char *,size_t,unsigned char [32],MP_CANCEL,void *);
int mp_crypto_seal(MP_CRYPTO *,const unsigned char [32],const unsigned char *,size_t,
 const unsigned char *,size_t,const unsigned char *,size_t,
 unsigned char *,size_t,unsigned char [16]);
/* quarantine is private caller workspace; it MUST NOT be observed concurrently.
 * No plaintext output is written until final tag verification succeeds.
 * quarantine is wiped on every exit after accepted argument validation. */
int mp_crypto_unseal(MP_CRYPTO *,const unsigned char [32],const unsigned char *,size_t,
 const unsigned char *,size_t,const unsigned char *,size_t,const unsigned char *,size_t,
 unsigned char *,size_t,unsigned char *,size_t,size_t *,MP_CANCEL,void *);
/* approved_spki must come from independent approved signer trust, not the
 * package. This verifies the exact M1 signed domain, never establishes trust. */
int mp_crypto_verify_package(MP_CRYPTO *,const unsigned char *,size_t,
 const unsigned char *,size_t);
/* Mathematical signature only; untrusted embedded SPKI never grants trust.
 * Caller must pass D3 independently before any KDF/content release. */
int mp_crypto_verify_signature(MP_CRYPTO *,const unsigned char *,size_t);
/* Reviewed provider erasure; only on owned quiescent memory. */
void mp_crypto_wipe(unsigned char *,size_t);
/* Correspondence only, NOT certificate profile/chain/validity authorization. */
int mp_crypto_key_match(MP_CRYPTO *,const unsigned char *,size_t,
 const unsigned char *,size_t);
/* Public direct-chain profile check. Expected IDs and reserved serial are
 * immutable caller data from the existing administrative/issuer owner, not
 * authority created here. Approved CA is independent of package contents.
 * UTC must be supplied by its qualified clock; this API never selects time. */
int mp_crypto_client_profile(MP_CRYPTO *,const unsigned char *,size_t,
 const unsigned char *,size_t,const unsigned char [16],const unsigned char [16],
 const unsigned char [16],unsigned long long);
#endif
