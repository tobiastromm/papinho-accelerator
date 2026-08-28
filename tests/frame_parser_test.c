#include <string.h>

#include "frame_parser.h"

static PAPACC_RESULT papacc_test_encode(
    PAPACC_U16 type,
    PAPACC_U32 payload_length,
    PAPACC_U8 output[PAPACC_FRAME_BASE_HEADER_SIZE])
{
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_SIZE written;
    header.envelope_major = 1;
    header.envelope_minor = 0;
    header.header_length = 16;
    header.message_type = type;
    header.payload_length = payload_length;
    return papacc_frame_header_encode(
        &header, output, PAPACC_FRAME_BASE_HEADER_SIZE, &written);
}

static int papacc_test_golden_and_partial(void)
{
    PAPACC_U8 frame[19] = {
        0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
        0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
        0xAA, 0xBB, 0xCC
    };
    PAPACC_FRAME_PARSER parser = PAPACC_FRAME_PARSER_INITIALIZER;
    PAPACC_FRAME_PARSER_EVENT event = PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
    PAPACC_SIZE consumed;

    if (papacc_frame_parser_init(&parser, 3) != PAPACC_RESULT_OK ||
        papacc_frame_parser_feed(
            &parser, frame, 5, &consumed, &event) != PAPACC_RESULT_OK ||
        consumed != 5 || event.type != PAPACC_FRAME_PARSER_EVENT_NONE ||
        papacc_frame_parser_feed(
            &parser, frame + 5, 3, &consumed, &event) != PAPACC_RESULT_OK ||
        consumed != 3 || event.type != PAPACC_FRAME_PARSER_EVENT_NONE ||
        papacc_frame_parser_feed(
            &parser, frame + 8, 11, &consumed, &event) != PAPACC_RESULT_OK ||
        consumed != 8 ||
        event.type != PAPACC_FRAME_PARSER_EVENT_HEADER_READY ||
        event.header.message_type != 0x1234 ||
        event.header.payload_length != 3 ||
        papacc_frame_parser_feed(
            &parser, frame + 16, 3, &consumed, &event) != PAPACC_RESULT_OK ||
        consumed != 3 ||
        event.type != PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE ||
        event.payload_length != 3 ||
        memcmp(event.payload, frame + 16, 3) != 0 ||
        parser.state != PAPACC_FRAME_PARSER_STATE_READING_HEADER ||
        papacc_frame_parser_finish(&parser) != PAPACC_RESULT_OK) {
        return 1;
    }
    return 0;
}

static int papacc_test_fragmentation_and_multiple(void)
{
    static const PAPACC_SIZE chunks[] = { 2, 5, 1, 4, 3, 7 };
    PAPACC_U8 stream[52];
    PAPACC_FRAME_PARSER parser = PAPACC_FRAME_PARSER_INITIALIZER;
    PAPACC_FRAME_PARSER_EVENT event = PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
    PAPACC_SIZE offset = 0;
    PAPACC_SIZE consumed;
    PAPACC_SIZE complete_count = 0;
    PAPACC_SIZE chunk_index = 0;
    PAPACC_U8 observed[4];
    PAPACC_SIZE observed_count = 0;

    (void)papacc_test_encode(1, 0, stream);
    (void)papacc_test_encode(2, 3, stream + 16);
    stream[32] = 0xAA;
    stream[33] = 0xBB;
    stream[34] = 0xCC;
    (void)papacc_test_encode(3, 1, stream + 35);
    stream[51] = 0xDD;
    if (papacc_frame_parser_init(&parser, 3) != PAPACC_RESULT_OK) {
        return 10;
    }
    while (offset < sizeof(stream)) {
        PAPACC_SIZE offered = chunks[chunk_index %
            (sizeof(chunks) / sizeof(chunks[0]))];
        PAPACC_SIZE local_offset = 0;
        if (offered > sizeof(stream) - offset) {
            offered = sizeof(stream) - offset;
        }
        while (local_offset < offered) {
            if (papacc_frame_parser_feed(
                    &parser, stream + offset + local_offset,
                    offered - local_offset, &consumed, &event) !=
                    PAPACC_RESULT_OK || consumed == 0) {
                return 11;
            }
            if (event.type == PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK ||
                (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE &&
                 event.payload_length > 0)) {
                if (observed_count + event.payload_length > sizeof(observed)) {
                    return 12;
                }
                memcpy(observed + observed_count,
                       event.payload, event.payload_length);
                observed_count += event.payload_length;
            }
            if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
                ++complete_count;
            }
            local_offset += consumed;
        }
        offset += offered;
        ++chunk_index;
    }
    if (complete_count != 3 || observed_count != 4 ||
        observed[0] != 0xAA || observed[1] != 0xBB ||
        observed[2] != 0xCC || observed[3] != 0xDD ||
        papacc_frame_parser_finish(&parser) != PAPACC_RESULT_OK) {
        return 13;
    }
    return 0;
}

static int papacc_test_byte_by_byte(void)
{
    PAPACC_U8 frame[19];
    PAPACC_FRAME_PARSER parser = PAPACC_FRAME_PARSER_INITIALIZER;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_SIZE consumed;
    PAPACC_SIZE index;
    PAPACC_SIZE complete_count = 0;
    PAPACC_U8 observed[3];
    PAPACC_SIZE observed_count = 0;

    (void)papacc_test_encode(0xBEEF, 3, frame);
    frame[16] = 1;
    frame[17] = 2;
    frame[18] = 3;
    if (papacc_frame_parser_init(&parser, 3) != PAPACC_RESULT_OK) {
        return 20;
    }
    for (index = 0; index < sizeof(frame); ++index) {
        if (papacc_frame_parser_feed(
                &parser, &frame[index], 1, &consumed, &event) !=
                PAPACC_RESULT_OK || consumed != 1) {
            return 21;
        }
        if (event.type == PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK ||
            (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE &&
             event.payload_length != 0)) {
            observed[observed_count++] = event.payload[0];
        }
        if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
            ++complete_count;
        }
    }
    if (complete_count != 1 || observed_count != 3 ||
        memcmp(observed, frame + 16, 3) != 0) {
        return 22;
    }
    return 0;
}

static int papacc_test_limits_versions_and_terminal(void)
{
    PAPACC_U8 header[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_FRAME_PARSER parser = PAPACC_FRAME_PARSER_INITIALIZER;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_SIZE consumed;
    PAPACC_RESULT expected[2] = {
        PAPACC_RESULT_NOT_SUPPORTED, PAPACC_RESULT_NOT_SUPPORTED
    };
    PAPACC_SIZE index;

    for (index = 0; index < 2; ++index) {
        parser = (PAPACC_FRAME_PARSER)PAPACC_FRAME_PARSER_INITIALIZER;
        (void)papacc_test_encode(1, 0, header);
        header[4 + index] = (PAPACC_U8)(2 - index);
        if (papacc_frame_parser_init(&parser, 0) != PAPACC_RESULT_OK ||
            papacc_frame_parser_feed(
                &parser, header, sizeof(header), &consumed, &event) !=
                expected[index] ||
            parser.state != PAPACC_FRAME_PARSER_STATE_ERROR ||
            papacc_frame_parser_feed(
                &parser, header, sizeof(header), &consumed, &event) !=
                PAPACC_RESULT_INVALID_STATE || consumed != 0 ||
            papacc_frame_parser_finish(&parser) !=
                PAPACC_RESULT_INVALID_STATE) {
            return 30;
        }
    }
    parser = (PAPACC_FRAME_PARSER)PAPACC_FRAME_PARSER_INITIALIZER;
    (void)papacc_test_encode(1, 101, header);
    if (papacc_frame_parser_init(&parser, 100) != PAPACC_RESULT_OK ||
        papacc_frame_parser_feed(
            &parser, header, sizeof(header), &consumed, &event) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        parser.state != PAPACC_FRAME_PARSER_STATE_ERROR) {
        return 31;
    }
    papacc_frame_parser_reset(&parser);
    (void)papacc_test_encode(1, 100, header);
    if (papacc_frame_parser_feed(
            &parser, header, sizeof(header), &consumed, &event) !=
            PAPACC_RESULT_OK ||
        event.type != PAPACC_FRAME_PARSER_EVENT_HEADER_READY) {
        return 32;
    }
    return 0;
}

static int papacc_test_eof_reset_and_errors(void)
{
    PAPACC_U8 header[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_FRAME_PARSER parser;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_SIZE consumed;
    PAPACC_SIZE length;

    (void)papacc_test_encode(1, 0, header);
    for (length = 1; length < PAPACC_FRAME_BASE_HEADER_SIZE; ++length) {
        parser = (PAPACC_FRAME_PARSER)PAPACC_FRAME_PARSER_INITIALIZER;
        if (papacc_frame_parser_init(&parser, 0) != PAPACC_RESULT_OK ||
            papacc_frame_parser_feed(
                &parser, header, length, &consumed, &event) !=
                PAPACC_RESULT_OK ||
            papacc_frame_parser_finish(&parser) !=
                PAPACC_RESULT_PROTOCOL_ERROR ||
            parser.state != PAPACC_FRAME_PARSER_STATE_ERROR) {
            return 40;
        }
    }
    parser = (PAPACC_FRAME_PARSER)PAPACC_FRAME_PARSER_INITIALIZER;
    (void)papacc_test_encode(1, 3, header);
    if (papacc_frame_parser_init(&parser, 3) != PAPACC_RESULT_OK ||
        papacc_frame_parser_finish(&parser) != PAPACC_RESULT_OK ||
        papacc_frame_parser_feed(
            &parser, header, sizeof(header), &consumed, &event) !=
            PAPACC_RESULT_OK ||
        papacc_frame_parser_feed(
            &parser, header, 1, &consumed, &event) != PAPACC_RESULT_OK ||
        papacc_frame_parser_finish(&parser) !=
            PAPACC_RESULT_PROTOCOL_ERROR) {
        return 41;
    }
    papacc_frame_parser_reset(&parser);
    header[0] = 0;
    if (papacc_frame_parser_feed(
            &parser, header, sizeof(header), &consumed, &event) !=
            PAPACC_RESULT_PROTOCOL_ERROR ||
        parser.state != PAPACC_FRAME_PARSER_STATE_ERROR) {
        return 42;
    }
    papacc_frame_parser_reset(&parser);
    (void)papacc_test_encode(1, 0, header);
    if (papacc_frame_parser_feed(
            &parser, header, 8, &consumed, &event) != PAPACC_RESULT_OK) {
        return 43;
    }
    papacc_frame_parser_reset(&parser);
    if (papacc_frame_parser_feed(
            &parser, header, sizeof(header), &consumed, &event) !=
            PAPACC_RESULT_OK ||
        event.type != PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
        return 44;
    }
    return 0;
}

static int papacc_test_null_and_zero_input(void)
{
    PAPACC_FRAME_PARSER parser = PAPACC_FRAME_PARSER_INITIALIZER;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_SIZE consumed = 99;
    PAPACC_U8 byte = 0;

    papacc_frame_parser_reset(NULL);
    papacc_frame_parser_reset(&parser);
    if (papacc_frame_parser_init(NULL, 0) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_frame_parser_feed(
            NULL, NULL, 0, &consumed, &event) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_frame_parser_finish(NULL) != PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_frame_parser_init(&parser, 0) != PAPACC_RESULT_OK ||
        papacc_frame_parser_init(&parser, 0) != PAPACC_RESULT_INVALID_STATE ||
        papacc_frame_parser_feed(
            &parser, NULL, 0, &consumed, &event) != PAPACC_RESULT_OK ||
        consumed != 0 || event.type != PAPACC_FRAME_PARSER_EVENT_NONE ||
        papacc_frame_parser_feed(
            &parser, NULL, 1, &consumed, &event) !=
            PAPACC_RESULT_INVALID_ARGUMENT || consumed != 0 ||
        papacc_frame_parser_feed(
            &parser, &byte, 1, NULL, &event) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_frame_parser_feed(
            &parser, &byte, 1, &consumed, NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 50;
    }
    return 0;
}

int main(void)
{
    int result = papacc_test_golden_and_partial();
    if (result == 0) {
        result = papacc_test_fragmentation_and_multiple();
    }
    if (result == 0) {
        result = papacc_test_byte_by_byte();
    }
    if (result == 0) {
        result = papacc_test_limits_versions_and_terminal();
    }
    if (result == 0) {
        result = papacc_test_eof_reset_and_errors();
    }
    if (result == 0) {
        result = papacc_test_null_and_zero_input();
    }
    return result;
}
