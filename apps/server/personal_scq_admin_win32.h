#ifndef PAPACC_PERSONAL_SCQ_ADMIN_WIN32_H
#define PAPACC_PERSONAL_SCQ_ADMIN_WIN32_H
#include "personal_scq_handoff_win32.h"
#include "personal_scq_profile_win32.h"
#include "server_service_data.h"
/* Private immutable registration from an actual owner operational decision.
 * Not accepted from argv/files/IPC, not a generic grant or new persistence.
 * Native registration is EMPTY until that independent decision exists. */
typedef struct PAPACC_SCQ_OWNER_DECISION {
 const WCHAR *binary,*parent,*data; /* frozen approved target, not a path reopen */
 const void *controller;const struct PAPACC_SCQ_ADMIN_SOURCE *source; /* original live packet only */
 DWORD read_diagnostic; /* K0 local typed binding, never serialized or inferred from purpose */
 DWORD purpose,controller_pid,controller_creation[2],controller_token[2],controller_authentication[2],controller_modified[2];
 DWORD valid_from[2],valid_until[2]; /* exact original window, never carrier-selected */
 BYTE request[32],decision[32],attempt[32],policy[32],target[32],image[32];
 PAPACC_SCQ_REF_DATA originals[3];
 BYTE administrator[68],runtime[68];DWORD administrator_bytes,runtime_bytes;char key[112];
} PAPACC_SCQ_OWNER_DECISION;
typedef struct PAPACC_SCQ_ADMIN_SOURCE {
 struct PAPACC_SCQ_ADMIN_SOURCE *self;const void *controller;
 const PAPACC_SCQ_OWNER_DECISION *registration;const PAPACC_SCQ_PROFILE_DATA *plan;
 DWORD thread,phase,first; /*1 captured,2 shown,3 confirmed,4 taken,5 closed,6 UNKNOWN,7 invalid*/
 /* Independent, live Start-only act in this same source. Not serialized,
  * restored after restart, or implied by SCQ/SCM registration confirmation. */
 DWORD start_phase,start_first; /*1 presented,2 attributed,3 taken,4 UNKNOWN*/
 volatile LONG start_consumed;
 DWORD endpoint_phase,endpoint_first; /* independent one-use endpoint act; same original source */
 volatile LONG endpoint_consumed;
 PAPACC_PERSONAL_SCQ_ORIGINAL value;
} PAPACC_SCQ_ADMIN_SOURCE;
#endif
