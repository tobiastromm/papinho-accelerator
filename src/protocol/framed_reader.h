#ifndef PAPACC_FRAMED_READER_H
#define PAPACC_FRAMED_READER_H

#include "frame_parser.h"
#include "transport_connection.h"

typedef enum PAPACC_FRAMED_READER_STATE {
    PAPACC_FRAMED_READER_STATE_UNINITIALIZED = 0,
    PAPACC_FRAMED_READER_STATE_READY = 1,
    PAPACC_FRAMED_READER_STATE_END_OF_STREAM = 2,
    PAPACC_FRAMED_READER_STATE_ERROR = 3
} PAPACC_FRAMED_READER_STATE;

typedef enum PAPACC_FRAMED_READER_STATUS {
    PAPACC_FRAMED_READER_STATUS_UNSPECIFIED = 0,
    PAPACC_FRAMED_READER_STATUS_EVENT = 1,
    PAPACC_FRAMED_READER_STATUS_NEED_MORE_DATA = 2,
    PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK = 3,
    PAPACC_FRAMED_READER_STATUS_END_OF_STREAM = 4
} PAPACC_FRAMED_READER_STATUS;

typedef struct PAPACC_FRAMED_READER {
    PAPACC_TRANSPORT_CONNECTION *transport;
    PAPACC_FRAME_PARSER parser;
    PAPACC_U8 *read_buffer;
    PAPACC_SIZE read_buffer_capacity;
    PAPACC_SIZE buffered_offset;
    PAPACC_SIZE buffered_length;
    PAPACC_FRAMED_READER_STATE state;
} PAPACC_FRAMED_READER;

#define PAPACC_FRAMED_READER_INITIALIZER \
    { NULL, PAPACC_FRAME_PARSER_INITIALIZER, NULL, 0, 0, 0, \
      PAPACC_FRAMED_READER_STATE_UNINITIALIZED }

PAPACC_RESULT papacc_framed_reader_init(
    PAPACC_FRAMED_READER *reader,
    PAPACC_TRANSPORT_CONNECTION *transport,
    PAPACC_U8 *read_buffer,
    PAPACC_SIZE read_buffer_capacity,
    PAPACC_U32 max_payload_length);

/*
 * May block when it needs the underlying transport. Performs at most one
 * transport read and returns at most one framing event. A payload pointer in
 * out_event is valid until the next next(), reset(), or shutdown() call.
 */
PAPACC_RESULT papacc_framed_reader_next(
    PAPACC_FRAMED_READER *reader,
    PAPACC_FRAMED_READER_STATUS *out_status,
    PAPACC_FRAME_PARSER_EVENT *out_event);

/* Discards buffered/parser state but never reopens or resynchronizes a stream. */
PAPACC_RESULT papacc_framed_reader_reset(PAPACC_FRAMED_READER *reader);

/* Releases no resources and does not close the non-owned transport. */
void papacc_framed_reader_shutdown(PAPACC_FRAMED_READER *reader);

#endif
