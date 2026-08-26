#ifndef PAPACC_SERVER_CONSOLE_WIN32_H
#define PAPACC_SERVER_CONSOLE_WIN32_H

#include "papacc/types.h"

typedef struct PAPACC_SERVER_CONSOLE_WIN32 {
    PAPACC_BOOL is_installed;
} PAPACC_SERVER_CONSOLE_WIN32;

#define PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER { PAPACC_FALSE }

PAPACC_RESULT papacc_server_console_win32_install(
    PAPACC_SERVER_CONSOLE_WIN32 *console);

PAPACC_RESULT papacc_server_console_win32_uninstall(
    PAPACC_SERVER_CONSOLE_WIN32 *console);

PAPACC_BOOL papacc_server_console_win32_stop_requested(void);
void papacc_server_console_win32_request_stop(void);
void papacc_server_console_win32_wait_for_stop(void);

#endif
