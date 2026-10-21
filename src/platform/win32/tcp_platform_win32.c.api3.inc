#include <winsock2.h>

#include "tcp_platform.h"
#include "native_teardown_win32.h"

PAPACC_RESULT papacc_tcp_platform_init(PAPACC_TCP_PLATFORM *platform)
{
    WSADATA data;
    int startup_result;

    if (platform == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (platform->initialized == PAPACC_TRUE || platform->close_unresolved != PAPACC_FALSE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (platform->initialized != PAPACC_FALSE) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    startup_result = WSAStartup(MAKEWORD(2, 2), &data);
    if (startup_result != 0) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    platform->initialized=PAPACC_TRUE;
    if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2) {
        (void)papacc_native_winsock_cleanup_once(&platform->initialized,&platform->close_unresolved);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    platform->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_tcp_platform_try_shutdown(PAPACC_TCP_PLATFORM *platform)
{
 if(!platform)return PAPACC_RESULT_INVALID_ARGUMENT;
 return papacc_native_winsock_cleanup_once(&platform->initialized,&platform->close_unresolved);
}
void papacc_tcp_platform_shutdown(PAPACC_TCP_PLATFORM *platform)
{(void)papacc_tcp_platform_try_shutdown(platform);}