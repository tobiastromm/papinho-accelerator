#ifndef PAPACC_SERVER_CREDENTIAL_REVOCATION_WIN32_H
#define PAPACC_SERVER_CREDENTIAL_REVOCATION_WIN32_H
#include <papacc/types.h>
/* Facts from existing owners only; never a grant or a close receipt. */
typedef struct PAPACC_CREDENTIAL_REVOCATION_RECEIPT {
 PAPACC_U64 revision,administrative_epoch,policy_epoch;
 PAPACC_U8 request[16],credential[32];
 unsigned sessions_invalidated,operations_stop_requested,controls_stop_requested;
 unsigned wake_requested;
 unsigned uncertain,quiescence_proven,close_proven;
 PAPACC_RESULT first_error;
} PAPACC_CREDENTIAL_REVOCATION_RECEIPT;
#endif
