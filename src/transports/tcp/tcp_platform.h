#ifndef PAPACC_TCP_PLATFORM_H
#define PAPACC_TCP_PLATFORM_H

#include "papacc/types.h"

/*
 * Caller-owned lifecycle state for the platform-specific TCP foundation.
 * A context may be initialized again after shutdown. Concurrent access to the
 * same context is outside this contract and requires external synchronization.
 */
typedef struct PAPACC_TCP_PLATFORM {
    PAPACC_BOOL initialized;
} PAPACC_TCP_PLATFORM;

#define PAPACC_TCP_PLATFORM_INITIALIZER { PAPACC_FALSE }

PAPACC_RESULT papacc_tcp_platform_init(PAPACC_TCP_PLATFORM *platform);
void papacc_tcp_platform_shutdown(PAPACC_TCP_PLATFORM *platform);

#endif
