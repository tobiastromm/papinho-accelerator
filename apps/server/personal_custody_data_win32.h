#ifndef PAPACC_PERSONAL_CUSTODY_DATA_WIN32_H
#define PAPACC_PERSONAL_CUSTODY_DATA_WIN32_H
#include "server_ca_key_win32.h"
/* Synchronous metadata query only. ABSENT means no observation in this owner,
 * never filesystem/key-store absence. DATA is not factual custody/authority.
 * Caller serializes on the canonical writer and keeps any CA owner protected
 * for this call. No reference escapes, borrow, native call or effect occurs. */
#define PAPACC_PERSONAL_CUSTODY_ABSENT 0U
#define PAPACC_PERSONAL_CUSTODY_METADATA 1U
#define PAPACC_PERSONAL_CUSTODY_RETAINED 2U
#define PAPACC_PERSONAL_CUSTODY_UNAVAILABLE 3U
typedef struct PAPACC_PERSONAL_CUSTODY_DATA {
 unsigned target,ca,ceremony_phase,effect_denied;
} PAPACC_PERSONAL_CUSTODY_DATA;
#endif
