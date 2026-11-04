#include <windows.h>

#include "server_console_win32.h"

/*
 * SetConsoleCtrlHandler supplies no application context. These two process-
 * local atomics are therefore the minimal bridge between its callback thread
 * and the single server main thread; no config, network, or socket is global.
 */
static volatile LONG papacc_server_console_stop_requested = 0;
static volatile LONG papacc_server_console_handler_installed = 0;
static PVOID volatile papacc_server_console_wait_event = NULL;
static volatile LONG papacc_server_console_wait_users = 0;
static volatile LONG papacc_server_console_wait_error = 0;
typedef char papacc_console_census_exact[(PAPACC_CONSOLE_BRIDGE_STORAGE_BYTES==
 sizeof(papacc_server_console_stop_requested)+sizeof(papacc_server_console_handler_installed)+
 sizeof(papacc_server_console_wait_event)+sizeof(papacc_server_console_wait_users)+sizeof(papacc_server_console_wait_error))?1:-1];

void papacc_server_console_win32_request_stop(void)
{
    HANDLE wake;
    (void)InterlockedExchange(&papacc_server_console_stop_requested, 1);
    (void)InterlockedIncrement(&papacc_server_console_wait_users);
    wake=(HANDLE)InterlockedCompareExchangePointer(&papacc_server_console_wait_event,NULL,NULL);
    if(wake&&!SetEvent(wake))(void)InterlockedCompareExchange(&papacc_server_console_wait_error,1,0);
    (void)InterlockedDecrement(&papacc_server_console_wait_users);
}
PAPACC_RESULT papacc_server_console_win32_wait_original(void *event,void *peer,unsigned long timeout,unsigned long *observed)
{
    HANDLE handles[2];DWORD flags,result,count=peer?2U:1U;
    if(!event||!observed||event==peer||!GetHandleInformation((HANDLE)event,&flags)||(flags&HANDLE_FLAG_INHERIT))return PAPACC_RESULT_INVALID_ARGUMENT;
    *observed=WAIT_FAILED;
    if(InterlockedCompareExchange(&papacc_server_console_wait_error,0,0)||
       InterlockedCompareExchangePointer(&papacc_server_console_wait_event,event,NULL))return PAPACC_RESULT_INVALID_STATE;
    handles[0]=(HANDLE)event;handles[1]=(HANDLE)peer;
    if(papacc_server_console_win32_stop_requested())result=WAIT_OBJECT_0;
    else result=WaitForMultipleObjects(count,handles,FALSE,timeout);
    (void)InterlockedCompareExchangePointer(&papacc_server_console_wait_event,NULL,event);
    /* A callback that already borrowed the event may still be using it. Do
     * not spin, close it or infer completion; caller retains on this result. */
    if(InterlockedCompareExchange(&papacc_server_console_wait_users,0,0)||InterlockedCompareExchange(&papacc_server_console_wait_error,0,0))return PAPACC_RESULT_INVALID_STATE;
    *observed=result;return result==WAIT_FAILED?PAPACC_RESULT_INTERNAL_ERROR:PAPACC_RESULT_OK;
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
