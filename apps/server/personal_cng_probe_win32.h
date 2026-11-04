#ifndef PAPACC_PERSONAL_CNG_PROBE_WIN32_H
#define PAPACC_PERSONAL_CNG_PROBE_WIN32_H
#include <windows.h>
#include <ncrypt.h>
#include "personal_runtime_identity_win32.h"
/* Diagnostic facts only; no result is an authorization or CA borrow. */
enum {PAPACC_CNG_PROBE_NONE=0,PAPACC_CNG_PROBE_LOCAL_DENY,
 PAPACC_CNG_PROBE_QUERY_FAILURE,PAPACC_CNG_PROBE_PROVIDER_FAILURE,
 PAPACC_CNG_PROBE_PERMISSION_OBSERVED,PAPACC_CNG_PROBE_NOT_FOUND,
 PAPACC_CNG_PROBE_UNEXPECTED_OPEN,PAPACC_CNG_PROBE_STOPPED,
 PAPACC_CNG_PROBE_CLOSE_UNKNOWN};
typedef struct PAPACC_PERSONAL_CNG_PROBE {
 NCRYPT_PROV_HANDLE provider;NCRYPT_KEY_HANDLE key;HANDLE unexpected_thread;
 DWORD thread,phase,classification,first_stage,first_origin;
 SECURITY_STATUS first_error,provider_result,open_result,close_error;
 unsigned provider_calls,open_calls,close_attempted,close_calls,close_api;
 const struct PAPACC_SCQ_READ_RECIPIENT *read_scope; /* local admitted P only */
 WCHAR name[112];
} PAPACC_PERSONAL_CNG_PROBE;
/* I1 DATA classification only. All original authority/close gates remain separate. */
static __inline int papacc_personal_cng_inconclusive(const PAPACC_PERSONAL_CNG_PROBE *p)
{
 return p&&p->phase==3U&&p->classification==PAPACC_CNG_PROBE_NOT_FOUND&&
  p->provider_calls==1U&&!p->provider_result&&p->open_calls==1U&&p->open_result==NTE_BAD_KEYSET&&
  p->first_stage==4U&&p->first_origin==3U&&p->first_error==NTE_BAD_KEYSET&&
  p->close_attempted==1U&&p->close_calls==1U&&!p->close_error&&!p->provider&&!p->key&&!p->unexpected_thread;
}
/* Single-shot passive fixture access probe; never create/sign/export/delete.
 * Exact existing CA-owner name format; caller must bind the original fixture.
 * Names alone never establish that a key is a fixture or grant access. */
DWORD papacc_personal_cng_probe(PAPACC_PERSONAL_CNG_PROBE *,PAPACC_PERSONAL_RUNTIME_IDENTITY *,const WCHAR *,volatile LONG *);
DWORD papacc_personal_cng_probe_close(PAPACC_PERSONAL_CNG_PROBE *);
#endif
