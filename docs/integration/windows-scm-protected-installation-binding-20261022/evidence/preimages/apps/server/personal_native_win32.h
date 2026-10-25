#ifndef PAPACC_PERSONAL_NATIVE_WIN32_H
#define PAPACC_PERSONAL_NATIVE_WIN32_H
#include <windows.h>
#include "server_console_win32.h"
/* Same canonical packet owns every handle/allocation through factual close. */
#define PAPACC_PERSONAL_HANDOFF_SUBMITTED 1L
#define PAPACC_PERSONAL_HANDOFF_VERIFIED 2L
#define PAPACC_PERSONAL_HANDOFF_DENIED 3L
#define PAPACC_PERSONAL_HANDOFF_RETAINED 4L
typedef struct PAPACC_PERSONAL_NATIVE_CONTEXT {
 /* Private read-only dispatch state; same packet, no authority or child ID. */
 volatile LONG handoff_state;unsigned char handoff_attempt[32];
 char plan_leaf[65]; /* fixed BEFORE presentation; never a child identity */
 TOKEN_STATISTICS token;BYTE sid[SECURITY_MAX_SID_SIZE];DWORD sid_bytes,session;
 BY_HANDLE_FILE_INFORMATION parent,source;
 HANDLE unexpected_thread,check_token;
 PSECURITY_DESCRIPTOR descriptor,check_descriptor;
 LPSTR sid_text;
 DWORD descriptor_bytes,error,close_attempts;
 unsigned phase; /*0 empty,1 live,2 retained,3 closed; never authority */
 PAPACC_SERVER_CONSOLE_WIN32 console;
} PAPACC_PERSONAL_NATIVE_CONTEXT;
#endif
