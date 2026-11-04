#ifndef PAPACC_PERSONAL_INSTALL_PATHS_WIN32_H
#define PAPACC_PERSONAL_INSTALL_PATHS_WIN32_H
#include <windows.h>
#define PAPACC_PERSONAL_PATHS_READY 2U
#define PAPACC_PERSONAL_PATHS_NEW 1U
#define PAPACC_PERSONAL_PATHS_EXISTING 2U
#define PAPACC_PERSONAL_PATHS_BINARY_ONLY 3U /* private DATA, never full NEW_INSTALL */
#define PAPACC_PERSONAL_PATHS_SELECTED_OBJECT 4U /* P0-P7 candidate; not a proof */
typedef struct PAPACC_SCQ_EXISTING_SELECTION {
 DWORD identity[9],architecture; /* ProgramFiles / binary parent / image */
 BYTE provenance[32];
} PAPACC_SCQ_EXISTING_SELECTION;
/* Private frozen DATA in canonical packet; never an original-object proof. */
typedef struct PAPACC_PERSONAL_INSTALL_PATHS {
 DWORD thread,phase,mode,error,read_diagnostic;
 WCHAR binary[MAX_PATH],parent[MAX_PATH],data[MAX_PATH],leaf[65];
 PAPACC_SCQ_EXISTING_SELECTION selected;
 BYTE read_public[32]; /* K3 DATA, bound into original target; never authority */
 const void *original_chain;DWORD selected_role;
} PAPACC_PERSONAL_INSTALL_PATHS;
DWORD papacc_personal_paths_new(PAPACC_PERSONAL_INSTALL_PATHS *,volatile LONG *);
DWORD papacc_personal_paths_binary(PAPACC_PERSONAL_INSTALL_PATHS *,volatile LONG *);
DWORD papacc_personal_paths_binary_current(const PAPACC_PERSONAL_INSTALL_PATHS *,volatile LONG *);
DWORD papacc_personal_paths_existing(PAPACC_PERSONAL_INSTALL_PATHS *,const WCHAR *,const WCHAR *,const WCHAR *);
DWORD papacc_personal_paths_current(const PAPACC_PERSONAL_INSTALL_PATHS *,volatile LONG *);
/* K3 typed locator only; acquisition and original authority remain separate. */
DWORD papacc_personal_paths_selected_machine_read(PAPACC_PERSONAL_INSTALL_PATHS *,const PAPACC_SCQ_EXISTING_SELECTION *,DWORD,const void *,volatile LONG *,const BYTE[32]);
DWORD papacc_personal_paths_selected(PAPACC_PERSONAL_INSTALL_PATHS *,const PAPACC_SCQ_EXISTING_SELECTION *,DWORD,const void *,volatile LONG *);
#endif
