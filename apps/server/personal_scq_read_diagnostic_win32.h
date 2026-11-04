#ifndef PAPACC_SCQ_READ_DIAGNOSTIC_WIN32_H
#define PAPACC_SCQ_READ_DIAGNOSTIC_WIN32_H
#include "personal_scq_admin_win32.h"
#include "personal_scq_recipient_win32.h"
/* K0-K9 substate of the SAME owner. Never an IPC-decoded grant. */
typedef struct PAPACC_SCQ_READ_DIAGNOSTIC {
 struct PAPACC_SCQ_READ_DIAGNOSTIC *self;const void *owner;
 PAPACC_SCQ_ADMIN_SOURCE *source;
 DWORD thread,phase,first;volatile LONG consumed;
 BYTE attempt[32],policy[32],request[32],decision[32],key_public[32];
 WCHAR key[112];ULONGLONG until;
} PAPACC_SCQ_READ_DIAGNOSTIC;
/* Receiver-local custody. Never copied from a controller pointer/frame. */
typedef struct PAPACC_SCQ_READ_RECIPIENT {
 struct PAPACC_SCQ_READ_RECIPIENT *self;const void *owner;
 PAPACC_SCQ_RECIPIENT_ORIGIN *origin;
 struct PAPACC_SCQ_SESSION_DRIVER *session;
 PAPACC_PERSONAL_SCQ_INPUTS *inputs;
 PAPACC_PERSONAL_CNG_PROBE *probe;
 DWORD thread,phase,first,probe_used;
 BYTE public_id[32];
} PAPACC_SCQ_READ_RECIPIENT;
#endif
