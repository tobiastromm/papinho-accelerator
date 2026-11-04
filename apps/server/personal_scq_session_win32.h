#ifndef PAPACC_PERSONAL_SCQ_SESSION_WIN32_H
#define PAPACC_PERSONAL_SCQ_SESSION_WIN32_H
#include "personal_scq_admin_win32.h"
#include "personal_scq_transport_win32.h"
#include "personal_scq_read_diagnostic_win32.h"
/* Embedded sequence and bounded native-IO custody, never authority.
 * DATA and private close stages stay separate; buffer lifetime spans pending IO. */
typedef struct PAPACC_SCQ_SESSION_DRIVER {
 struct PAPACC_SCQ_SESSION_DRIVER *self;const void *owner;
 PAPACC_SCQ_PROFILE_DATA *profile;PAPACC_SCQ_ADMIN_SOURCE *source;
 PAPACC_PERSONAL_SCQ_RECEIVER *receiver;PAPACC_PERSONAL_SCQ_DISPATCH *dispatch;
 PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime;
 DWORD thread,role,phase,first,bytes; /*1 ready,2 taken,3 delivered,4 DATA ack/receipt,5 closed,6 UNKNOWN,7 invalid*/
 PAPACC_SCQ_READ_DIAGNOSTIC *read_act;PAPACC_SCQ_READ_RECIPIENT *read_recipient; /* local originals only */
 PAPACC_SCQ_TRANSPORT transport; /* bounded native IO and original-reference custody */
 volatile LONG peer_lost;BYTE buffer[PAPACC_SCQ_BOOTSTRAP_READ_BYTES];
} PAPACC_SCQ_SESSION_DRIVER;
#endif
