#ifndef PAPACC_PERSONAL_SCQ_COLD_START_WIN32_H
#define PAPACC_PERSONAL_SCQ_COLD_START_WIN32_H
#include "personal_scq_start_assurance_win32.h"
#include "personal_install_paths_win32.h"
/* Same-process candidate selectors only. Paths/pins/numbers never approve
 * an operation. Native originals + attributed SCQ/endpoint/Start remain required. */
typedef struct PAPACC_SCQ_COLD_START_INPUT {
 const WCHAR *request_path,*decision_path;
 BYTE pins[3][32];PAPACC_SCQ_START_LIMITS limits;DWORD authorization_ms;
 const PAPACC_SCQ_EXISTING_SELECTION *existing; /* copied to owned packet; DATA only */
 DWORD existing_console_data; /* only explicit TO-existing entry; never approval */
} PAPACC_SCQ_COLD_START_INPUT;
typedef struct PAPACC_SCQ_READ_COLD_INPUT { PAPACC_SCQ_COLD_START_INPUT cold;char key[112];BYTE public_id[32]; } PAPACC_SCQ_READ_COLD_INPUT;
/* Private read-only interval observation, never an original-source proof.
 * Samples: known/count/error/owned-slot flags; no native references exported. */
typedef struct PAPACC_SCQ_INTERVAL_OBSERVATION {
 DWORD sample[9][4],first_cut,first_error,identity_checked;
 DWORD hash_status,hash_cleanup_status,hash_called,final_guard;
 BY_HANDLE_FILE_INFORMATION identity;
 WCHAR module[MAX_PATH],drive[4];BYTE descriptor_digest[32];
} PAPACC_SCQ_INTERVAL_OBSERVATION;
#endif
