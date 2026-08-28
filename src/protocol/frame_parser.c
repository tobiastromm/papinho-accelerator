#include "frame_parser.h"

#include <string.h>

static void papacc_frame_parser_clear_frame(PAPACC_FRAME_PARSER *parser)
{
    memset(parser->header_bytes, 0, sizeof(parser->header_bytes));
    parser->header_bytes_received = 0;
    parser->header = (PAPACC_FRAME_HEADER)PAPACC_FRAME_HEADER_INITIALIZER;
    parser->payload_bytes_received = 0;
    parser->state = PAPACC_FRAME_PARSER_STATE_READING_HEADER;
}

PAPACC_RESULT papacc_frame_parser_init(
    PAPACC_FRAME_PARSER *parser,
    PAPACC_U32 max_payload_length)
{
    if (parser == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (parser->state != PAPACC_FRAME_PARSER_STATE_UNINITIALIZED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    parser->max_payload_length = max_payload_length;
    papacc_frame_parser_clear_frame(parser);
    return PAPACC_RESULT_OK;
}

void papacc_frame_parser_reset(PAPACC_FRAME_PARSER *parser)
{
    if (parser == NULL ||
        parser->state == PAPACC_FRAME_PARSER_STATE_UNINITIALIZED) {
        return;
    }
    papacc_frame_parser_clear_frame(parser);
}

PAPACC_RESULT papacc_frame_parser_feed(
    PAPACC_FRAME_PARSER *parser,
    const PAPACC_U8 *input,
    PAPACC_SIZE input_length,
    PAPACC_SIZE *out_consumed,
    PAPACC_FRAME_PARSER_EVENT *out_event)
{
    PAPACC_FRAME_PARSER_EVENT event =
        PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
    PAPACC_RESULT result;

    if (out_consumed != NULL) {
        *out_consumed = 0;
    }
    if (out_event != NULL) {
        *out_event = event;
    }
    if (parser == NULL || out_consumed == NULL || out_event == NULL ||
        (input_length > 0 && input == NULL)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (parser->state == PAPACC_FRAME_PARSER_STATE_UNINITIALIZED ||
        parser->state == PAPACC_FRAME_PARSER_STATE_ERROR) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (input_length == 0) {
        return PAPACC_RESULT_OK;
    }
    if (parser->state == PAPACC_FRAME_PARSER_STATE_READING_HEADER) {
        PAPACC_SIZE needed = PAPACC_FRAME_BASE_HEADER_SIZE -
            parser->header_bytes_received;
        PAPACC_SIZE consumed = input_length < needed ? input_length : needed;
        memcpy(&parser->header_bytes[parser->header_bytes_received],
               input, consumed);
        parser->header_bytes_received += consumed;
        *out_consumed = consumed;
        if (parser->header_bytes_received < PAPACC_FRAME_BASE_HEADER_SIZE) {
            return PAPACC_RESULT_OK;
        }
        result = papacc_frame_header_decode(
            parser->header_bytes, PAPACC_FRAME_BASE_HEADER_SIZE,
            parser->max_payload_length, &parser->header);
        if (result != PAPACC_RESULT_OK) {
            parser->state = PAPACC_FRAME_PARSER_STATE_ERROR;
            return result;
        }
        event.header = parser->header;
        if (parser->header.payload_length == 0) {
            event.type = PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE;
            papacc_frame_parser_clear_frame(parser);
        } else {
            event.type = PAPACC_FRAME_PARSER_EVENT_HEADER_READY;
            parser->state = PAPACC_FRAME_PARSER_STATE_READING_PAYLOAD;
        }
        *out_event = event;
        return PAPACC_RESULT_OK;
    }
    if (parser->state == PAPACC_FRAME_PARSER_STATE_READING_PAYLOAD) {
        PAPACC_U32 remaining =
            parser->header.payload_length - parser->payload_bytes_received;
        PAPACC_SIZE consumed = input_length;
        if (consumed > (PAPACC_SIZE)remaining) {
            consumed = (PAPACC_SIZE)remaining;
        }
        event.header = parser->header;
        event.payload = input;
        event.payload_length = consumed;
        parser->payload_bytes_received += (PAPACC_U32)consumed;
        *out_consumed = consumed;
        if (parser->payload_bytes_received == parser->header.payload_length) {
            event.type = PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE;
            papacc_frame_parser_clear_frame(parser);
        } else {
            event.type = PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK;
        }
        *out_event = event;
        return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

PAPACC_RESULT papacc_frame_parser_finish(PAPACC_FRAME_PARSER *parser)
{
    if (parser == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (parser->state == PAPACC_FRAME_PARSER_STATE_UNINITIALIZED ||
        parser->state == PAPACC_FRAME_PARSER_STATE_ERROR) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (parser->state == PAPACC_FRAME_PARSER_STATE_READING_HEADER &&
        parser->header_bytes_received == 0) {
        return PAPACC_RESULT_OK;
    }
    parser->state = PAPACC_FRAME_PARSER_STATE_ERROR;
    return PAPACC_RESULT_PROTOCOL_ERROR;
}
