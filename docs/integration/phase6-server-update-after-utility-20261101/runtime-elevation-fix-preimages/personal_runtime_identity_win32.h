#ifndef PAPACC_PERSONAL_RUNTIME_IDENTITY_WIN32_H
#define PAPACC_PERSONAL_RUNTIME_IDENTITY_WIN32_H
#include <windows.h>
#include "personal_scq_profile_win32.h"
/* Embedded substate of the canonical pending packet, NOT a root/authority.
 * Identity/SCM readback evidence only. Never CA-access/installation authority. */
#define PAPACC_PERSONAL_RUNTIME_SAM 1U
#define PAPACC_PERSONAL_RUNTIME_VIRTUAL 2U
typedef struct PAPACC_PERSONAL_RUNTIME_IDENTITY {
 HANDLE process,token,check_token,manager,service;
 DWORD thread,pid,profile,expected_sid_type,first_error;
 FILETIME creation;
 TOKEN_STATISTICS statistics;
 BYTE sid[SECURITY_MAX_SID_SIZE];DWORD sid_bytes;
 DWORD group_count,specific_matches,specific_attributes;
 unsigned closed_preparation; /* 1 START_PENDING,2 armed closed RUNNING; never productive authority */
 unsigned phase; /* 0 empty,1 live,2 denied,3 closed,4 close UNKNOWN retained */
} PAPACC_PERSONAL_RUNTIME_IDENTITY;
DWORD papacc_personal_runtime_capture(PAPACC_PERSONAL_RUNTIME_IDENTITY *,HANDLE,PSID,unsigned,DWORD);
DWORD papacc_personal_runtime_capture_closed(PAPACC_PERSONAL_RUNTIME_IDENTITY *,HANDLE,PSID,DWORD);
DWORD papacc_personal_runtime_current(PAPACC_PERSONAL_RUNTIME_IDENTITY *);
/* Partial native observation, never an admission/source or transferable ref.
 * Original image stays UNKNOWN; session and service-logon group are queried. */
#define PAPACC_SCQ_OBS_PROCESS 1U
#define PAPACC_SCQ_OBS_TOKEN 2U
#define PAPACC_SCQ_OBS_SCM 4U
#define PAPACC_SCQ_OBS_SESSION_LOGON 8U
#define PAPACC_SCQ_OBS_IMAGE 16U
#define PAPACC_SCQ_OBS_COMPLETE 31U
typedef struct PAPACC_SCQ_RUNTIME_OBSERVATION {
 DWORD known;PAPACC_SCQ_PEER_DATA peer;
} PAPACC_SCQ_RUNTIME_OBSERVATION;
DWORD papacc_personal_runtime_observe_data(PAPACC_PERSONAL_RUNTIME_IDENTITY *,PAPACC_SCQ_RUNTIME_OBSERVATION *,volatile LONG *);
DWORD papacc_personal_runtime_close(PAPACC_PERSONAL_RUNTIME_IDENTITY *);
#endif
