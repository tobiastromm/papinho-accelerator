#ifndef PAPACC_SERVER_ISSUER_X509_WIN32_H
#define PAPACC_SERVER_ISSUER_X509_WIN32_H
#include "server_issuer_windows_validation.h"
#include <wincrypt.h>
#include <ncrypt.h>
/* Public export only; canonical RSA AlgorithmIdentifier, never private material. */
PAPACC_ISSUER_RESULT papacc_issuer_x509_key_public(NCRYPT_KEY_HANDLE,BYTE *,DWORD,CERT_PUBLIC_KEY_INFO **);
/* Borrowed public profile versus same-root authority/generation/prefix/SPKI.
 * No self-signature/trust/original-source claim. keyid is disjoint output. */
/* Unsigned initial CA TBS, serial 1 and high-water 1. No prior CURRENT/CA,
 * private operation, certificate authenticity or initialization authority. */
PAPACC_ISSUER_RESULT papacc_issuer_x509_ca_tbs(const PAPACC_ISSUER_VIEW *,PAPACC_U64,const CERT_PUBLIC_KEY_INFO *,BYTE *,DWORD,DWORD *);
PAPACC_ISSUER_RESULT papacc_issuer_x509_ca_matches(const PAPACC_ISSUER_VIEW *,const CERT_INFO *,BYTE[32]);
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
/* OFFLINE candidate-only SERVER preparation/check, not signer authority.
 * Explicit exact DNS ASCII name is caller-owned immutable public data. No CSR
 * name, CURRENT, approval or persistent binding is inferred. CN is display only;
 * the sole dNSName SAN and serverAuth EKU are required by the exact verifier.
 * These functions are NOT connected to the signer or product configuration. */
PAPACC_ISSUER_RESULT papacc_issuer_x509_server_tbs(const PAPACC_ISSUER_VIEW *,const PAPACC_ISSUER_RECORD *,
 const CERT_INFO *,const CERT_PUBLIC_KEY_INFO *,const char *,DWORD,BYTE *,DWORD,DWORD *);
PAPACC_ISSUER_RESULT papacc_issuer_x509_server_verify(const PAPACC_ISSUER_VIEW *,const PAPACC_ISSUER_RECORD *,
 const CERT_INFO *,const char *,DWORD,const BYTE *,DWORD,PAPACC_ISSUER_WINDOWS_WORKSPACE *,BYTE[32]);
/* Existing core ports: no new root, scheduler or key owner. All borrowed inputs
 * outlive this descriptor and any retained lease. Serialized owner-thread API.
 * initialize is registration only, not authority. Actual root proofs, protected
 * key identity, admin callback and complete credit precede any native signature. */
enum {PAPACC_CA_SOURCE_NONE=0,PAPACC_CA_SOURCE_NEW_INSTALL=1,PAPACC_CA_SOURCE_EXISTING_CURRENT=2};
typedef struct PAPACC_ISSUER_CNG_BACKEND {
 PAPACC_ISSUER_BACKEND ports;
 PAPACC_ISSUER_ROOT_PORT root;
 const PAPACC_ISSUER_BACKEND *administrative;
 PAPACC_CA_KEY *key;PAPACC_ISSUER_CORE *owner;const CERT_INFO *ca;
 PAPACC_ISSUER_WINDOWS_WORKSPACE workspace;
 PAPACC_ISSUER_VIEW view;PAPACC_ISSUER_RECORD record;
 PAPACC_SIZE reserved;NCRYPT_KEY_HANDLE leased_key;DWORD thread;SECURITY_STATUS native_error;DWORD native_stage,win32_error;
 /* Latched ONLY by the original ceremony handoff. The same root object
  * outlives backend custody even after its worker packet is reused. */
 const void *original_source_root;
 PAPACC_U8 original_attempt[32],original_authority[16],original_generation[16];
 unsigned source_kind;
 int initialized,active,retained,stopped,custody;
} PAPACC_ISSUER_CNG_BACKEND;
PAPACC_ISSUER_RESULT papacc_issuer_cng_backend_init(PAPACC_ISSUER_CNG_BACKEND *,
 const PAPACC_ISSUER_ROOT_PORT *,const PAPACC_ISSUER_BACKEND *,PAPACC_ISSUER_CORE *,PAPACC_CA_KEY *,const CERT_INFO *,void *,PAPACC_SIZE,PAPACC_SIZE);
/* Validate existing NORMAL root/key/certificate inputs before custody transfer.
 * Does not create proofs/borrow/key/cert or change root. Output is DATA only.
 * Same owner thread and caller-held immutable backing required. */
PAPACC_ISSUER_RESULT papacc_issuer_cng_backend_normal_source(PAPACC_ISSUER_CNG_BACKEND *,PAPACC_ISSUER_VIEW *);
void papacc_issuer_cng_backend_stop(PAPACC_ISSUER_CNG_BACKEND *);
/* Returns the transferred lifetime borrow only after factual core finish,
 * or backend Stop with a never-initialized, idle core.
 * Does not close the native key or make a custody/authority claim. */
PAPACC_ISSUER_RESULT papacc_issuer_cng_backend_release_custody(PAPACC_ISSUER_CNG_BACKEND *);
#endif
