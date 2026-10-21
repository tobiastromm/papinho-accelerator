#ifndef PAPACC_SERVER_CA_KEY_WIN32_H
#define PAPACC_SERVER_CA_KEY_WIN32_H
/* Private administrative key owner. No startup hook, signing API or TLS use.
 * Caller supplies zeroed durable storage; no copies/aliases while initialized.
 * Public issuer signing is deliberately unavailable without root reservation. */
#include <windows.h>
#include <ncrypt.h>
typedef enum PAPACC_CA_KEY_STATE {
 PAPACC_CA_KEY_EMPTY=0,PAPACC_CA_KEY_READY,PAPACC_CA_KEY_RETAINED,PAPACC_CA_KEY_CLOSED
} PAPACC_CA_KEY_STATE;
typedef struct PAPACC_CA_KEY {
 NCRYPT_PROV_HANDLE provider;
 NCRYPT_KEY_HANDLE key;
 HANDLE retained_token;
 HLOCAL retained_local[3];
 DWORD thread;
 PAPACC_CA_KEY_STATE state;
 SECURITY_STATUS error;
 wchar_t name[96];
 BYTE operator_sid[SECURITY_MAX_SID_SIZE],service_sid[SECURITY_MAX_SID_SIZE];
 DWORD operator_size,service_size;
 int created,close_attempted,close_confirmed;
} PAPACC_CA_KEY;
/* Explicit operation only: no overwrite, fallback, deletion or regeneration.
 * operator/service are actual configured SIDs, not display names. Native ACL
 * validation is mandatory. Unknown close result retains owner for diagnosis. */
SECURITY_STATUS papacc_ca_key_create(PAPACC_CA_KEY *,const BYTE[16],const BYTE[16],PSID,PSID);
SECURITY_STATUS papacc_ca_key_open(PAPACC_CA_KEY *,const BYTE[16],const BYTE[16],PSID,PSID);
SECURITY_STATUS papacc_ca_key_validate(PAPACC_CA_KEY *);
SECURITY_STATUS papacc_ca_key_close(PAPACC_CA_KEY *);
#endif