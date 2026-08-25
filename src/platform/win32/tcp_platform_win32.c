#include <winsock2.h>

#include "tcp_platform.h"

PAPACC_RESULT papacc_tcp_platform_init(PAPACC_TCP_PLATFORM *platform)
{
    WSADATA data;
    int startup_result;

    if (platform == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (platform->initialized == PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (platform->initialized != PAPACC_FALSE) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    startup_result = WSAStartup(MAKEWORD(2, 2), &data);
    if (startup_result != 0) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2) {
        (void)WSACleanup();
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    platform->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_tcp_platform_shutdown(PAPACC_TCP_PLATFORM *platform)
{
    if (platform == NULL || platform->initialized != PAPACC_TRUE) {
        return;
    }

    (void)WSACleanup();
    platform->initialized = PAPACC_FALSE;
}
