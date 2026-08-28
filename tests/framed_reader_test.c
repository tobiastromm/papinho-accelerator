#include "framed_reader.h"

#include <string.h>

typedef struct PAPACC_TEST_STREAM {
    const PAPACC_U8 *data;
    PAPACC_SIZE length;
    PAPACC_SIZE offset;
    const PAPACC_SIZE *chunks;
    PAPACC_SIZE chunk_count;
    PAPACC_SIZE chunk_index;
    PAPACC_U32 read_count;
    PAPACC_U32 close_count;
    PAPACC_BOOL would_block;
    PAPACC_BOOL fatal_error;
} PAPACC_TEST_STREAM;

static PAPACC_RESULT papacc_test_read(
    void *opaque, PAPACC_U8 *buffer, PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TEST_STREAM *stream = (PAPACC_TEST_STREAM *)opaque;
    PAPACC_SIZE available;
    PAPACC_SIZE amount;
    ++stream->read_count;
    if (stream->fatal_error == PAPACC_TRUE) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    if (stream->would_block == PAPACC_TRUE) {
        stream->would_block = PAPACC_FALSE;
        *out_transferred = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (stream->offset == stream->length) {
        *out_transferred = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    available = stream->length - stream->offset;
    amount = available < capacity ? available : capacity;
    if (stream->chunk_index < stream->chunk_count &&
        amount > stream->chunks[stream->chunk_index]) {
        amount = stream->chunks[stream->chunk_index];
    }
    ++stream->chunk_index;
    memcpy(buffer, &stream->data[stream->offset], amount);
    stream->offset += amount;
    *out_transferred = amount;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_test_write(
    void *opaque, const PAPACC_U8 *buffer, PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    (void)opaque;
    (void)buffer;
    (void)length;
    *out_transferred = 0;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}

static void papacc_test_close(void *opaque)
{
    PAPACC_TEST_STREAM *stream = (PAPACC_TEST_STREAM *)opaque;
    ++stream->close_count;
}

static PAPACC_TRANSPORT_CONNECTION papacc_test_transport(
    PAPACC_TEST_STREAM *stream)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    transport.context = stream;
    transport.read_fn = papacc_test_read;
    transport.write_fn = papacc_test_write;
    transport.close_fn = papacc_test_close;
    return transport;
}

static void papacc_test_frame(
    PAPACC_U8 *output, PAPACC_U16 type,
    const PAPACC_U8 *payload, PAPACC_U32 payload_length)
{
    PAPACC_FRAME_HEADER header = {
        PAPACC_FRAME_ENVELOPE_MAJOR, PAPACC_FRAME_ENVELOPE_MINOR,
        PAPACC_FRAME_BASE_HEADER_SIZE, type, 0, payload_length
    };
    PAPACC_SIZE written = 0;
    (void)papacc_frame_header_encode(&header, output, 16, &written);
    if (payload_length > 0 && payload != NULL) {
        memcpy(&output[16], payload, payload_length);
    }
}

static int papacc_test_partial_and_one_byte(void)
{
    PAPACC_U8 frame[19];
    PAPACC_U8 scratch[19];
    PAPACC_U8 one_scratch[1];
    const PAPACC_U8 payload[3] = { 0xAA, 0xBB, 0xCC };
    const PAPACC_SIZE chunks[3] = { 3, 4, 12 };
    PAPACC_TEST_STREAM stream = {
        frame, sizeof(frame), 0, chunks, 3, 0, 0, 0,
        PAPACC_FALSE, PAPACC_FALSE
    };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&stream);
    PAPACC_FRAMED_READER reader = PAPACC_FRAMED_READER_INITIALIZER;
    PAPACC_FRAMED_READER_STATUS status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_U32 payload_events = 0;

    papacc_test_frame(frame, 0x1234, payload, 3);
    if (papacc_framed_reader_init(
            &reader, &transport, scratch, sizeof(scratch), 3) !=
            PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_NEED_MORE_DATA ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_NEED_MORE_DATA ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_EVENT ||
        event.type != PAPACC_FRAME_PARSER_EVENT_HEADER_READY ||
        stream.read_count != 3 ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        event.type != PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE ||
        event.payload_length != 3 || event.payload < scratch ||
        event.payload + event.payload_length > scratch + sizeof(scratch) ||
        memcmp(event.payload, payload, 3) != 0 || stream.read_count != 3) {
        return 1;
    }
    papacc_framed_reader_shutdown(&reader);
    stream.offset = 0;
    stream.chunks = NULL;
    stream.chunk_count = 0;
    stream.chunk_index = 0;
    stream.read_count = 0;
    if (papacc_framed_reader_init(
            &reader, &transport, one_scratch, 1, 3) != PAPACC_RESULT_OK) {
        return 2;
    }
    for (;;) {
        if (papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_OK) {
            return 3;
        }
        if (status == PAPACC_FRAMED_READER_STATUS_EVENT) {
            ++payload_events;
            if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
                break;
            }
        }
    }
    if (stream.read_count != sizeof(frame) || payload_events != 4) {
        return 4;
    }
    return 0;
}

static int papacc_test_multiple_frames_and_eof(void)
{
    PAPACC_U8 bytes[51];
    PAPACC_U8 scratch[sizeof(bytes)];
    const PAPACC_U8 payload_a[1] = { 0xA1 };
    const PAPACC_U8 payload_c[2] = { 0xC1, 0xC2 };
    PAPACC_TEST_STREAM stream = {
        bytes, sizeof(bytes), 0, NULL, 0, 0, 0, 0,
        PAPACC_FALSE, PAPACC_FALSE
    };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&stream);
    PAPACC_FRAMED_READER reader = PAPACC_FRAMED_READER_INITIALIZER;
    PAPACC_FRAMED_READER_STATUS status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_U32 events = 0;

    papacc_test_frame(bytes, 1, payload_a, 1);
    papacc_test_frame(&bytes[17], 2, NULL, 0);
    papacc_test_frame(&bytes[33], 3, payload_c, 2);
    if (papacc_framed_reader_init(
            &reader, &transport, scratch, sizeof(scratch), 2) !=
            PAPACC_RESULT_OK) {
        return 10;
    }
    while (events < 5) {
        if (papacc_framed_reader_next(&reader, &status, &event) !=
                PAPACC_RESULT_OK ||
            status != PAPACC_FRAMED_READER_STATUS_EVENT) {
            return 11;
        }
        ++events;
        if (stream.read_count != 1) {
            return 12;
        }
    }
    if (papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_END_OF_STREAM ||
        stream.read_count != 2 ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_END_OF_STREAM ||
        stream.read_count != 2) {
        return 13;
    }
    return 0;
}

static int papacc_test_would_block_reset_and_shutdown(void)
{
    PAPACC_U8 frame[16];
    PAPACC_U8 scratch[8];
    PAPACC_TEST_STREAM stream = {
        frame, sizeof(frame), 0, NULL, 0, 0, 0, 0,
        PAPACC_TRUE, PAPACC_FALSE
    };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&stream);
    PAPACC_FRAMED_READER reader = PAPACC_FRAMED_READER_INITIALIZER;
    PAPACC_FRAMED_READER_STATUS status;
    PAPACC_FRAME_PARSER_EVENT event;

    papacc_test_frame(frame, 1, NULL, 0);
    if (papacc_framed_reader_init(&reader, &transport, scratch, 8, 0) !=
            PAPACC_RESULT_OK ||
        papacc_framed_reader_init(&reader, &transport, scratch, 8, 0) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_NEED_MORE_DATA ||
        papacc_framed_reader_reset(&reader) != PAPACC_RESULT_OK) {
        return 20;
    }
    stream.offset = 0;
    do {
        if (papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_OK) {
            return 21;
        }
    } while (status != PAPACC_FRAMED_READER_STATUS_EVENT);
    if (event.type != PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
        return 22;
    }
    if (papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_END_OF_STREAM ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_END_OF_STREAM) {
        return 23;
    }
    papacc_framed_reader_shutdown(&reader);
    papacc_framed_reader_shutdown(&reader);
    if (stream.close_count != 0 ||
        papacc_transport_connection_is_valid(&transport) != PAPACC_TRUE ||
        reader.state != PAPACC_FRAMED_READER_STATE_UNINITIALIZED) {
        return 24;
    }
    stream.length = 0;
    stream.offset = 0;
    if (papacc_framed_reader_init(&reader, &transport, scratch, 8, 0) !=
            PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_READER_STATUS_END_OF_STREAM) {
        return 25;
    }
    papacc_framed_reader_shutdown(&reader);
    return 0;
}

static int papacc_test_failures(void)
{
    PAPACC_U8 bytes[18];
    PAPACC_U8 scratch[18];
    PAPACC_TEST_STREAM stream = {
        bytes, 16, 0, NULL, 0, 0, 0, 0, PAPACC_FALSE, PAPACC_FALSE
    };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&stream);
    PAPACC_FRAMED_READER reader = PAPACC_FRAMED_READER_INITIALIZER;
    PAPACC_FRAMED_READER_STATUS status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_RESULT result;

    papacc_test_frame(bytes, 1, NULL, 0);
    bytes[0] = 0;
    if (papacc_framed_reader_init(&reader, &transport, scratch, 18, 100) !=
            PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_PROTOCOL_ERROR ||
        reader.state != PAPACC_FRAMED_READER_STATE_ERROR ||
        status != PAPACC_FRAMED_READER_STATUS_UNSPECIFIED ||
        event.type != PAPACC_FRAME_PARSER_EVENT_NONE ||
        papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_framed_reader_reset(&reader) != PAPACC_RESULT_OK) {
        return 30;
    }
    papacc_test_frame(bytes, 1, NULL, 0);
    bytes[4] = 2;
    stream.offset = 0;
    if (papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_NOT_SUPPORTED ||
        papacc_framed_reader_reset(&reader) != PAPACC_RESULT_OK) {
        return 31;
    }
    papacc_test_frame(bytes, 1, NULL, 101);
    stream.offset = 0;
    if (papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_LIMIT_EXCEEDED) {
        return 32;
    }
    papacc_framed_reader_shutdown(&reader);
    stream.length = 5;
    stream.offset = 0;
    if (papacc_framed_reader_init(&reader, &transport, scratch, 18, 100) !=
            PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_PROTOCOL_ERROR) {
        return 33;
    }
    papacc_framed_reader_shutdown(&reader);
    papacc_test_frame(bytes, 1, NULL, 2);
    stream.length = 17;
    stream.offset = 0;
    if (papacc_framed_reader_init(&reader, &transport, scratch, 18, 100) !=
            PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) != PAPACC_RESULT_OK ||
        papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_PROTOCOL_ERROR) {
        return 34;
    }
    papacc_framed_reader_shutdown(&reader);
    stream.fatal_error = PAPACC_TRUE;
    stream.offset = 0;
    if (papacc_framed_reader_init(&reader, &transport, scratch, 18, 100) !=
            PAPACC_RESULT_OK) {
        return 35;
    }
    result = papacc_framed_reader_next(&reader, &status, &event);
    if (result != PAPACC_RESULT_INTERNAL_ERROR ||
        reader.state != PAPACC_FRAMED_READER_STATE_ERROR ||
        papacc_framed_reader_next(&reader, &status, &event) !=
            PAPACC_RESULT_INVALID_STATE) {
        return 36;
    }
    return 0;
}

int main(void)
{
    int result = papacc_test_partial_and_one_byte();
    if (result == 0) result = papacc_test_multiple_frames_and_eof();
    if (result == 0) result = papacc_test_would_block_reset_and_shutdown();
    if (result == 0) result = papacc_test_failures();
    return result;
}
