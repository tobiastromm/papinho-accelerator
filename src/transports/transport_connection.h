#ifndef PAPACC_TRANSPORT_CONNECTION_H
#define PAPACC_TRANSPORT_CONNECTION_H

#include <papacc/types.h>

typedef void (*PAPACC_TRANSPORT_CLOSE_FN)(void *context);

typedef enum PAPACC_TRANSPORT_IO_STATUS {
    PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED = 0,
    PAPACC_TRANSPORT_IO_STATUS_PROGRESS = 1,
    PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK = 2,
    PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM = 3
} PAPACC_TRANSPORT_IO_STATUS;

typedef PAPACC_RESULT (*PAPACC_TRANSPORT_READ_FN)(
    void *context,
    PAPACC_U8 *buffer,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status);

typedef PAPACC_RESULT (*PAPACC_TRANSPORT_WRITE_FN)(
    void *context,
    const PAPACC_U8 *buffer,
    PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status);

/*
 * Minimal ownership handle for an established transport resource. The
 * context is opaque to portable callers. While valid, the owner must keep it
 * alive and close_fn must close the resource and reset/destroy its context.
 */
typedef struct PAPACC_TRANSPORT_CONNECTION {
    void *context;
    PAPACC_TRANSPORT_READ_FN read_fn;
    PAPACC_TRANSPORT_WRITE_FN write_fn;
    PAPACC_TRANSPORT_CLOSE_FN close_fn;
} PAPACC_TRANSPORT_CONNECTION;

#define PAPACC_TRANSPORT_CONNECTION_INITIALIZER { NULL, NULL, NULL, NULL }

PAPACC_BOOL papacc_transport_connection_is_valid(
    const PAPACC_TRANSPORT_CONNECTION *transport);

/* Partial progress is normal. Zero capacity is OK + UNSPECIFIED, with no
 * callback invocation. The transport never retains the caller's buffer. */
PAPACC_RESULT papacc_transport_connection_read(
    PAPACC_TRANSPORT_CONNECTION *transport,
    PAPACC_U8 *buffer,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status);

/* The caller owns and must retain any unsent suffix. Zero length is an
 * OK + UNSPECIFIED no-op, with no callback invocation. */
PAPACC_RESULT papacc_transport_connection_write(
    PAPACC_TRANSPORT_CONNECTION *transport,
    const PAPACC_U8 *buffer,
    PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status);

/* Closes at most once and restores the ownership handle to its initializer. */
void papacc_transport_connection_close(PAPACC_TRANSPORT_CONNECTION *transport);

#endif
