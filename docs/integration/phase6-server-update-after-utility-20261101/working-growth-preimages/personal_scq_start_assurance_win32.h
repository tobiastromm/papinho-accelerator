#ifndef PAPACC_PERSONAL_SCQ_START_ASSURANCE_WIN32_H
#define PAPACC_PERSONAL_SCQ_START_ASSURANCE_WIN32_H
#include "personal_scq_admin_win32.h"
#include "personal_scm_registration_win32.h"
#include "personal_scq_handle_policy_win32.h"
/* Accepted P1-only administrative risk floor; never inferred from DATA. */
#define PAPACC_SCQ_P1_COMMIT_FLOOR 3221225472ULL
/* Private live G3 plan DATA, not a grant, persisted schema or imported envelope. */
typedef struct PAPACC_SCQ_START_LIMITS {
 ULONGLONG private_growth,working_growth,physical_floor,commit_floor;
 DWORD max_handles,freshness_ms,duration_ms;
} PAPACC_SCQ_START_LIMITS;
typedef struct PAPACC_SCQ_START_ASSURANCE {
 struct PAPACC_SCQ_START_ASSURANCE *self;const void *owner;PAPACC_SCQ_ADMIN_SOURCE *source;
 DWORD thread,phase,first,presented; /*1 frozen plan,2 READY,3 denied,4 UNKNOWN,5 QUERY resources only*/
 DWORD p1_handles,initial_known,initial_handles; /* local policy, never peer-selected */
 PAPACC_SCQ_START_LIMITS limits;
 BYTE attempt[32],policy[32],request[32],decision[32],target[32],image[32];
 PAPACC_SCM_RESOURCE_SAMPLE ready;
 PAPACC_SCM_RESOURCE_OBSERVATION observations;
 ULONGLONG global_until; /* boot-monotonic, shared, never renewed by receiver */
} PAPACC_SCQ_START_ASSURANCE;
#endif
