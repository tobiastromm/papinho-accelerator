#ifndef PAPACC_SCQ_HANDLE_POLICY_WIN32_H
#define PAPACC_SCQ_HANDLE_POLICY_WIN32_H
#include "personal_install_paths_win32.h"
/* Private compiled policy; neither DATA nor max_handles selects P1. */
#define PAPACC_SCQ_P1_CAP 192U
#define PAPACC_SCQ_P1_GUARD 188U
#define PAPACC_SCQ_P1_ENTRY 128U
#define PAPACC_SCQ_P1_PATHS(p) ((p) && (p)->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT && \
 (p)->phase==PAPACC_PERSONAL_PATHS_READY && (p)->thread==GetCurrentThreadId() && \
 ((p)->selected_role==1U||(p)->selected_role==2U) && (p)->original_chain)
/* Scope matching only, never original-object proof or authority. READ needs
 * its explicit K3 locator; ordinary READ/NEW_INSTALL cannot reuse this path. */
#define PAPACC_SCQ_EXISTING_PATH_PURPOSE(p,x) ((p) && \
 (((x)==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY && !(p)->read_diagnostic) || \
  ((x)==PAPACC_SCQ_PURPOSE_READ && (p)->read_diagnostic==1U && PAPACC_SCQ_P1_PATHS(p))))
#define PAPACC_SCQ_HANDLE_GUARD(p1) ((p1)?PAPACC_SCQ_P1_GUARD:124U)
#define PAPACC_SCQ_HANDLE_CAP(p1) ((p1)?PAPACC_SCQ_P1_CAP:128U)
#endif
