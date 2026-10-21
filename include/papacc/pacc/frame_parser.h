#ifndef PAPACC_FRAME_PARSER_H
#define PAPACC_FRAME_PARSER_H

#include "frame.h"

typedef enum PAPACC_FRAME_PARSER_STATE {
    PAPACC_FRAME_PARSER_STATE_UNINITIALIZED = 0,
    PAPACC_FRAME_PARSER_STATE_READING_HEADER = 1,
    PAPACC_FRAME_PARSER_STATE_READING_PAYLOAD = 2,
    PAPACC_FRAME_PARSER_STATE_ERROR = 3
} PAPACC_FRAME_PARSER_STATE;

typedef enum PAPACC_FRAME_PARSER_EVENT_TYPE {
    PAPACC_FRAME_PARSER_EVENT_NONE = 0,
    PAPACC_FRAME_PARSER_EVENT_HEADER_READY = 1,
    PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK = 2,
    PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE = 3
} PAPACC_FRAME_PARSER_EVENT_TYPE;

typedef struct PAPACC_FRAME_PARSER_EVENT {
    PAPACC_FRAME_PARSER_EVENT_TYPE type;
    PAPACC_FRAME_HEADER header;
    const PAPACC_U8 *payload;
    PAPACC_SIZE payload_length;
} PAPACC_FRAME_PARSER_EVENT;

#define PAPACC_FRAME_PARSER_EVENT_INITIALIZER \
    { PAPACC_FRAME_PARSER_EVENT_NONE, PAPACC_FRAME_HEADER_INITIALIZER, \
      NULL, 0 }

typedef struct PAPACC_FRAME_PARSER {
    PAPACC_U8 header_bytes[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE header_bytes_received;
    PAPACC_FRAME_HEADER header;
    PAPACC_U32 payload_bytes_received;
    PAPACC_U32 max_payload_length;
    PAPACC_FRAME_PARSER_STATE state;
} PAPACC_FRAME_PARSER;

#define PAPACC_FRAME_PARSER_INITIALIZER \
    { { 0 }, 0, PAPACC_FRAME_HEADER_INITIALIZER, 0, 0, \
      PAPACC_FRAME_PARSER_STATE_UNINITIALIZED }

PAPACC_RESULT papacc_frame_parser_init(
    PAPACC_FRAME_PARSER *parser,
    PAPACC_U32 max_payload_length);

/* Reset discards partial/error state and preserves max_payload_length. */
void papacc_frame_parser_reset(PAPACC_FRAME_PARSER *parser);

/*
 * Consumes at most through one event. PAYLOAD_CHUNK points into this call's
 * input only. FRAME_COMPLETE carries the final payload chunk when non-empty;
 * the parser retains no input pointer and is already ready for the next header.
 */
PAPACC_RESULT papacc_frame_parser_feed(
    PAPACC_FRAME_PARSER *parser,
    const PAPACC_U8 *input,
    PAPACC_SIZE input_length,
    PAPACC_SIZE *out_consumed,
    PAPACC_FRAME_PARSER_EVENT *out_event);

/* Explicit EOF: clean only when exactly between frames. */
PAPACC_RESULT papacc_frame_parser_finish(PAPACC_FRAME_PARSER *parser);

#endif
