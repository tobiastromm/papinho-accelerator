#ifndef PAPACC_SERVER_ISSUER_X509_WIN32_H
#define PAPACC_SERVER_ISSUER_X509_WIN32_H
#include "server_issuer_windows_validation.h"
#include <wincrypt.h>
/* Public data-only builder. No signature/authority is produced by TBS encoding.
 * All inputs and output are disjoint immutable/owned caller storage. ca is the
 * public CA from the same root; caller retains its backing decoded storage. */
PAPACC_ISSUER_RESULT papacc_issuer_x509_tbs(const PAPACC_ISSUER_VIEW *,const PAPACC_ISSUER_RECORD *,
 const CERT_INFO *,const CERT_PUBLIC_KEY_INFO *,BYTE *,DWORD,DWORD *);
/* Exact accepted profile plus public signature check; no system trust change.
 * expected public key/issuer identity comes from same-root record, not CSR name.
 * Caller provides disjoint workspace; no allocated decode/handle is retained. */
PAPACC_ISSUER_RESULT papacc_issuer_x509_verify(const PAPACC_ISSUER_VIEW *,const PAPACC_ISSUER_RECORD *,
 const CERT_INFO *,const BYTE *,DWORD,PAPACC_ISSUER_WINDOWS_WORKSPACE *,BYTE[32]);
/* Existing core ports: no new root, scheduler or key owner. All borrowed inputs
 * outlive this descriptor and any retained lease. Serialized owner-thread API.
 * initialize is registration only, not authority. Actual root proofs, protected
 * key identity, admin callback and complete credit precede any native signature. */
typedef struct PAPACC_ISSUER_CNG_BACKEND {
 PAPACC_ISSUER_BACKEND ports;
 PAPACC_ISSUER_ROOT_PORT root;
 const PAPACC_ISSUER_BACKEND *administrative;
 PAPACC_CA_KEY *key;PAPACC_ISSUER_CORE *owner;const CERT_INFO *ca;
 PAPACC_ISSUER_WINDOWS_WORKSPACE workspace;
 PAPACC_ISSUER_VIEW view;PAPACC_ISSUER_RECORD record;
 PAPACC_SIZE reserved;DWORD thread;SECURITY_STATUS native_error;
 int initialized,active,retained,stopped;
} PAPACC_ISSUER_CNG_BACKEND;
PAPACC_ISSUER_RESULT papacc_issuer_cng_backend_init(PAPACC_ISSUER_CNG_BACKEND *,
 const PAPACC_ISSUER_ROOT_PORT *,const PAPACC_ISSUER_BACKEND *,PAPACC_ISSUER_CORE *,PAPACC_CA_KEY *,const CERT_INFO *,void *,PAPACC_SIZE,PAPACC_SIZE);
void papacc_issuer_cng_backend_stop(PAPACC_ISSUER_CNG_BACKEND *);
#endif
