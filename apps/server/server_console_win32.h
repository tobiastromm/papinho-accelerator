#ifndef PAPACC_SERVER_CONSOLE_WIN32_H
#define PAPACC_SERVER_CONSOLE_WIN32_H

#include "papacc/types.h"

typedef struct PAPACC_SERVER_CONSOLE_WIN32 {
    PAPACC_BOOL is_installed;
} PAPACC_SERVER_CONSOLE_WIN32;

#define PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER { PAPACC_FALSE }

/*
 * Application-private Win32 console lifecycle for one active server runner per
 * process. The caller owns the small state object. Install registers the
 * process handler; uninstall removes it deterministically and is idempotent.
 * The handler only publishes an interlocked stop flag. Waiting and all network
 * cleanup remain on the application's main thread.
 */
PAPACC_RESULT papacc_server_console_win32_install(
    PAPACC_SERVER_CONSOLE_WIN32 *console);

PAPACC_RESULT papacc_server_console_win32_uninstall(
    PAPACC_SERVER_CONSOLE_WIN32 *console);

PAPACC_BOOL papacc_server_console_win32_stop_requested(void);
void papacc_server_console_win32_request_stop(void);
void papacc_server_console_win32_wait_for_stop(void);
/* Synchronous ORIGINAL handle borrows for closed diagnostic progress. No
 * handle ownership or authority transfer. Busy/error means retain, not close.
 * A Stop callback can only signal the event during this bounded wait. */
PAPACC_RESULT papacc_server_console_win32_wait_original(void *,void *,unsigned long,unsigned long *);
#define PAPACC_CONSOLE_BRIDGE_STORAGE_BYTES (4U*sizeof(long)+sizeof(void *))

#endif
