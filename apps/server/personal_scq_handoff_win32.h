#ifndef PAPACC_PERSONAL_SCQ_HANDOFF_WIN32_H
#define PAPACC_PERSONAL_SCQ_HANDOFF_WIN32_H
#include "personal_scq_dispatch_win32.h"
/* Private borrowed original. No public constructor, approved bit or callback.
 * A decoded/caller-created view is never a source of administrative authority. */
typedef struct PAPACC_PERSONAL_SCQ_ORIGINAL {
 const struct PAPACC_PERSONAL_SCQ_ORIGINAL *original;
 const void *controller,*recipient;
 HANDLE request,decision,operator_process,operator_token,image;
 DWORD operator_pid;FILETIME operator_creation;
 BY_HANDLE_FILE_INFORMATION request_id,decision_id,image_id;
 TOKEN_STATISTICS operator_statistics;
 BYTE operator_sid[SECURITY_MAX_SID_SIZE];DWORD operator_sid_bytes;
 DWORD runtime_pid;FILETIME runtime_creation;TOKEN_STATISTICS runtime_token;
 BYTE association[32];WCHAR key[112];
 ULONGLONG deadline;
 volatile LONG consumed,borrowed,retained,controller_consumed;
 unsigned purpose; /* 1: SCQ read-only; never NEW_INSTALL/CA_INITIALIZE */
} PAPACC_PERSONAL_SCQ_ORIGINAL;


typedef struct PAPACC_PERSONAL_SCQ_RECEIVER {
 const struct PAPACC_PERSONAL_SCQ_RECEIVER *identity;
 const struct PAPACC_PERSONAL_SCQ_ORIGINAL *original;
 const void *owner;
 const PAPACC_PERSONAL_SCQ_DISPATCH *dispatch;
 const PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime;
 DWORD thread,phase,first,close_error;
 ULONGLONG deadline;
 BYTE association[32];
} PAPACC_PERSONAL_SCQ_RECEIVER;
enum {PAPACC_SCQ_RECEIVE_ABSENT=1,PAPACC_SCQ_RECEIVE_ORIGIN,
 PAPACC_SCQ_RECEIVE_IDENTITY,PAPACC_SCQ_RECEIVE_PURPOSE,
 PAPACC_SCQ_RECEIVE_EXPIRED,PAPACC_SCQ_RECEIVE_CONSUMED,
 PAPACC_SCQ_RECEIVE_STOP,PAPACC_SCQ_RECEIVE_CLOSE_UNKNOWN};
#endif
