#include <windows.h>

#include "server_console_win32.h"

/*
 * SetConsoleCtrlHandler supplies no application context. These two process-
 * local atomics are therefore the minimal bridge between its callback thread
 * and the single server main thread; no config, network, or socket is global.
 */
static volatile LONG papacc_server_console_stop_requested = 0;
static volatile LONG papacc_server_console_handler_installed = 0;

void papacc_server_console_win32_request_stop(void)
{
    (void)InterlockedExchange(&papacc_server_console_stop_requested, 1);
}

PAPACC_BOOL papacc_server_console_win32_stop_requested(void)
{
    return InterlockedCompareExchange(
               &papacc_server_console_stop_requested, 0, 0) != 0
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

static BOOL WINAPI papacc_server_console_win32_handler(DWORD event_type)
{
    if (event_type != CTRL_C_EVENT && event_type != CTRL_BREAK_EVENT) {
        return FALSE;
    }
    papacc_server_console_win32_request_stop();
    return TRUE;
}

PAPACC_RESULT papacc_server_console_win32_install(
    PAPACC_SERVER_CONSOLE_WIN32 *console)
{
    if (console == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (console->is_installed == PAPACC_TRUE ||
        InterlockedCompareExchange(
            &papacc_server_console_handler_installed, 1, 0) != 0) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    (void)InterlockedExchange(&papacc_server_console_stop_requested, 0);
    if (SetConsoleCtrlHandler(
            papacc_server_console_win32_handler, TRUE) == FALSE) {
        (void)InterlockedExchange(
            &papacc_server_console_handler_installed, 0);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    console->is_installed = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_console_win32_uninstall(
    PAPACC_SERVER_CONSOLE_WIN32 *console)
{
    if (console == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (console->is_installed == PAPACC_FALSE) {
        return PAPACC_RESULT_OK;
    }
    if (SetConsoleCtrlHandler(
            papacc_server_console_win32_handler, FALSE) == FALSE) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    console->is_installed = PAPACC_FALSE;
    (void)InterlockedExchange(&papacc_server_console_stop_requested, 0);
    (void)InterlockedExchange(
        &papacc_server_console_handler_installed, 0);
    return PAPACC_RESULT_OK;
}

void papacc_server_console_win32_wait_for_stop(void)
{
    while (papacc_server_console_win32_stop_requested() == PAPACC_FALSE) {
        Sleep(50);
    }
}
