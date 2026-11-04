#ifndef PAPACC_PERSONAL_SCQ_DISPATCH_WIN32_H
#define PAPACC_PERSONAL_SCQ_DISPATCH_WIN32_H
#include "personal_cng_probe_win32.h"
#include "personal_scq_profile_win32.h"
/* Denial reason only; caller-written values cannot grant authority. */
#define PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE 1U
#define PAPACC_SCQ_GATE_ORIGINAL_PROVENANCE_UNAVAILABLE 2U
/* One-shot synchronous dispatch DATA. No authorization bit, intent or handle. */
typedef struct PAPACC_PERSONAL_SCQ_DISPATCH {
 DWORD thread,phase,pid,result,gate_result,gate_reason,purpose;FILETIME creation;
 TOKEN_STATISTICS token;BYTE association[32];WCHAR name[112];
} PAPACC_PERSONAL_SCQ_DISPATCH;
DWORD papacc_personal_scq_prepare(PAPACC_PERSONAL_SCQ_DISPATCH *,const PAPACC_PERSONAL_RUNTIME_IDENTITY *,const BYTE[32],const WCHAR *);
DWORD papacc_personal_scq_prepare_transport(PAPACC_PERSONAL_SCQ_DISPATCH *,const PAPACC_PERSONAL_RUNTIME_IDENTITY *,const BYTE[32],const WCHAR[112]);
#endif
