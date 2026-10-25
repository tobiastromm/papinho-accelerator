#ifndef PAPACC_STRICT_ED25519_H
#define PAPACC_STRICT_ED25519_H
#include <stddef.h>
/* Mathematical DATA only. Caller protects immutable key for synchronous call.
 * No key ownership transfer, allocation, I/O, retention or temporal authority.
 * Interface is C89; OpenSSL implementation is modern-target specific. */
#define PAPACC_ED25519_INVALID 0
#define PAPACC_ED25519_VALID 1
#define PAPACC_ED25519_INTERNAL_ERROR (-1)
#define PAPACC_ED25519_BACKEND_UNAVAILABLE (-2)
#ifdef __cplusplus
extern "C" {
#endif
int papacc_ossl358_ed25519_point_check(const unsigned char *key, size_t bytes);
#ifdef __cplusplus
}
#endif
#endif
