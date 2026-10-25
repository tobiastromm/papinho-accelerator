#ifndef PAPACC_PRIVATE_STRICT_ED25519_H
#define PAPACC_PRIVATE_STRICT_ED25519_H
#include <stddef.h>
/* Public immutable bytes borrowed synchronously; no retention/allocation.
 * VALID is mathematical DATA validity, never administrative authority.
 * Caller owns Stop gating and retains inputs until synchronous return. */
#define PAPACC_ED25519_VALID 1
#define PAPACC_ED25519_INVALID 0
#define PAPACC_ED25519_INTERNAL_ERROR (-1)
#define PAPACC_ED25519_BACKEND_UNAVAILABLE (-2)
int papacc_time_source_ed25519_check(const unsigned char *key, size_t bytes);
#endif
