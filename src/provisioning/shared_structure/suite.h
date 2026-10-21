/* Private C89 suite policy. DATA ONLY: no crypto or authority acquisition. */
#ifndef PAPACC_SHARED_SUITE_H
#define PAPACC_SHARED_SUITE_H
#include "structure.h"
#define SP_SUITE1_ITERATIONS 600000UL
#define SP_SUITE1_KEY_BYTES 32U
#define SP_SUITE1_SALT_BYTES 16U
#define SP_SUITE1_NONCE_BYTES 12U
#define SP_SUITE1_TAG_BYTES 16U
#define SP_SUITE1_SIGNATURE_BYTES 256U
#define SP_SUITE1_AAD_MAX 1116U
#define SP_SUITE1_CIPHER_MAX 65536UL
/* No normalization here: reuse sp_password_normalize first. These functions
 * only check data/shape; they cannot authorize a backend KDF or AEAD call. */
SP_RESULT sp_suite1_kdf_shape(SP_SLICE,SP_SLICE,unsigned long);
SP_RESULT sp_suite1_aead_shape(size_t,size_t,size_t,size_t,size_t);
/* Borrowed public input plan for SHA256 + RSA2048/e65537 PKCS1-v1_5.
 * Domain includes its terminating zero. No copy of package, no DER/RSA/trust.
 * Input immutable/disjoint from output through all borrows; domain static const.
 * Output unchanged on rejection/Stop; caller owns every nonstatic borrow. */
typedef struct SP_SIGNED_PARTS {
 SP_SLICE domain,prefix,spki,signature;
} SP_SIGNED_PARTS;
SP_RESULT sp_suite1_signed_parts(const unsigned char *,size_t,
 SP_SIGNED_PARTS *,SP_STOP,void *);
#endif
