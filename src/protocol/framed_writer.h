#ifndef PAPACC_FRAMED_WRITER_H
#define PAPACC_FRAMED_WRITER_H

#include "frame.h"
#include "transport_connection.h"

typedef enum PAPACC_FRAMED_WRITER_STATE {
    PAPACC_FRAMED_WRITER_STATE_UNINITIALIZED = 0,
    PAPACC_FRAMED_WRITER_STATE_IDLE = 1,
    PAPACC_FRAMED_WRITER_STATE_WRITING_HEADER = 2,
    PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD = 3,
    PAPACC_FRAMED_WRITER_STATE_END_OF_STREAM = 4,
    PAPACC_FRAMED_WRITER_STATE_ERROR = 5
} PAPACC_FRAMED_WRITER_STATE;

typedef enum PAPACC_FRAMED_WRITER_STATUS {
    PAPACC_FRAMED_WRITER_STATUS_UNSPECIFIED = 0,
    PAPACC_FRAMED_WRITER_STATUS_PROGRESS = 1,
    PAPACC_FRAMED_WRITER_STATUS_NEED_PAYLOAD = 2,
    PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK = 3,
    PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE = 4,
    PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM = 5
} PAPACC_FRAMED_WRITER_STATUS;

typedef struct PAPACC_FRAMED_WRITER {
    PAPACC_TRANSPORT_CONNECTION *transport;
    PAPACC_U8 encoded_header[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE header_offset;
    PAPACC_U32 payload_remaining;
    PAPACC_FRAMED_WRITER_STATE state;
} PAPACC_FRAMED_WRITER;

#define PAPACC_FRAMED_WRITER_INITIALIZER \
    { NULL, { 0 }, 0, 0, PAPACC_FRAMED_WRITER_STATE_UNINITIALIZED }

PAPACC_RESULT papacc_framed_writer_init(
    PAPACC_FRAMED_WRITER *writer,
    PAPACC_TRANSPORT_CONNECTION *transport);

/* Validates and stores the encoded header, but performs no transport I/O. */
PAPACC_RESULT papacc_framed_writer_begin_frame(
    PAPACC_FRAMED_WRITER *writer,
    const PAPACC_FRAME_HEADER *header);

/*
 * May block in one underlying transport write. out_payload_consumed never
 * includes header bytes. The writer never retains payload or its suffix.
 */
PAPACC_RESULT papacc_framed_writer_step(
    PAPACC_FRAMED_WRITER *writer,
    const PAPACC_U8 *payload,
    PAPACC_SIZE payload_available,
    PAPACC_SIZE *out_payload_consumed,
    PAPACC_FRAMED_WRITER_STATUS *out_status);

/*
 * Discards local state only. If bytes were written, reset cannot undo them or
 * resynchronize the peer; production normally abandons that transport.
 */
PAPACC_RESULT papacc_framed_writer_reset(PAPACC_FRAMED_WRITER *writer);

/* Does not close the non-owned transport or release caller payload storage. */
void papacc_framed_writer_shutdown(PAPACC_FRAMED_WRITER *writer);

#endif
