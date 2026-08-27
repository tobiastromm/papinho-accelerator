#ifndef PAPACC_TRANSPORT_CONNECTION_H
#define PAPACC_TRANSPORT_CONNECTION_H

#include <papacc/types.h>

typedef void (*PAPACC_TRANSPORT_CLOSE_FN)(void *context);

/*
 * Minimal ownership handle for an established transport resource. The
 * context is opaque to portable callers. While valid, the owner must keep it
 * alive and close_fn must close the resource and reset/destroy its context.
 */
typedef struct PAPACC_TRANSPORT_CONNECTION {
    void *context;
    PAPACC_TRANSPORT_CLOSE_FN close_fn;
} PAPACC_TRANSPORT_CONNECTION;

#define PAPACC_TRANSPORT_CONNECTION_INITIALIZER { NULL, NULL }

PAPACC_BOOL papacc_transport_connection_is_valid(
    const PAPACC_TRANSPORT_CONNECTION *transport);

/* Closes at most once and restores the ownership handle to its initializer. */
void papacc_transport_connection_close(PAPACC_TRANSPORT_CONNECTION *transport);

#endif
