#ifndef PAPACC_PERSONAL_SCQ_RECIPIENT_WIN32_H
#define PAPACC_PERSONAL_SCQ_RECIPIENT_WIN32_H
#include "personal_scq_admin_win32.h"
#include "personal_scq_inputs_win32.h"
/* Receiver-local origin custody in the same packet, not a second authority. */
typedef struct PAPACC_SCQ_RECIPIENT_ORIGIN {
 struct PAPACC_SCQ_RECIPIENT_ORIGIN *self;const void *owner;
 const PAPACC_SCQ_OWNER_DECISION *registration;
 DWORD thread,phase,first;PAPACC_PERSONAL_SCQ_ORIGINAL value;
} PAPACC_SCQ_RECIPIENT_ORIGIN;
#endif
