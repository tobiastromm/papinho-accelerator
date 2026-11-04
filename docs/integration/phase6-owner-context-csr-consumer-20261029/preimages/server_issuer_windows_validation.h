#ifndef PAPACC_SERVER_ISSUER_WINDOWS_VALIDATION_H
#define PAPACC_SERVER_ISSUER_WINDOWS_VALIDATION_H
#include "server_ca_key_win32.h"
#include "issuer_core.h"
/* Read-only public preparation, never a signer/authority. No owned handles.
 * Caller serializes calls/Stop and supplies aligned disjoint scratch, retained
 * until return. Providers may allocate opaque workspace: not ED11 qualified.
 * Nothing here is attached to product startup or enabled by configuration. */
typedef struct PAPACC_ISSUER_WINDOWS_WORKSPACE {
 void *bytes;PAPACC_SIZE capacity;DWORD native_error;
 int stopped;
} PAPACC_ISSUER_WINDOWS_WORKSPACE;
/* Compatible with the existing backend.verify_request port. Checks exact DER,
 * RSA2048/e65537/SHA256 proof of possession and SHA256(CSR/full DER SPKI).
 * Does not accept CSR identity/extensions as administrative authority.
 * Attributes are unsupported in this first bounded preparation slice.
 * Caller owns immutable record/CSR and must keep both disjoint from workspace. */
PAPACC_ISSUER_RESULT papacc_issuer_windows_verify_request(void *,const PAPACC_ISSUER_RECORD *,const PAPACC_U8 *,PAPACC_SIZE);
/* Classification only; preserves key owner and original error receipt. Never
 * creates, opens, signs, frees or retries. BAD_KEYSET is not global absence. */
PAPACC_ISSUER_RESULT papacc_issuer_windows_key_result(const PAPACC_CA_KEY *,SECURITY_STATUS);
#endif
