#include "framed_writer.h"

#include <string.h>

typedef enum PAPACC_TEST_WRITE_MODE {
    PAPACC_TEST_WRITE_PROGRESS = 0,
    PAPACC_TEST_WRITE_WOULD_BLOCK = 1,
    PAPACC_TEST_WRITE_END_OF_STREAM = 2,
    PAPACC_TEST_WRITE_ERROR = 3
} PAPACC_TEST_WRITE_MODE;

typedef struct PAPACC_TEST_SINK {
    PAPACC_U8 bytes[256];
    PAPACC_SIZE length;
    PAPACC_SIZE max_write;
    PAPACC_U32 write_count;
    PAPACC_U32 close_count;
    PAPACC_TEST_WRITE_MODE next_mode;
} PAPACC_TEST_SINK;

static PAPACC_RESULT papacc_test_read(
    void *opaque, PAPACC_U8 *buffer, PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    (void)opaque;
    (void)buffer;
    (void)capacity;
    *out_transferred = 0;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_test_write(
    void *opaque, const PAPACC_U8 *buffer, PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TEST_SINK *sink = (PAPACC_TEST_SINK *)opaque;
    PAPACC_SIZE amount = length;
    PAPACC_TEST_WRITE_MODE mode = sink->next_mode;
    ++sink->write_count;
    sink->next_mode = PAPACC_TEST_WRITE_PROGRESS;
    if (mode == PAPACC_TEST_WRITE_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    if (mode == PAPACC_TEST_WRITE_WOULD_BLOCK) {
        *out_transferred = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (mode == PAPACC_TEST_WRITE_END_OF_STREAM) {
        *out_transferred = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (amount > sink->max_write) {
        amount = sink->max_write;
    }
    memcpy(&sink->bytes[sink->length], buffer, amount);
    sink->length += amount;
    *out_transferred = amount;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

static void papacc_test_close(void *opaque)
{
    PAPACC_TEST_SINK *sink = (PAPACC_TEST_SINK *)opaque;
    ++sink->close_count;
}

static PAPACC_TRANSPORT_CONNECTION papacc_test_transport(PAPACC_TEST_SINK *sink)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    transport.context = sink;
    transport.read_fn = papacc_test_read;
    transport.write_fn = papacc_test_write;
    transport.close_fn = papacc_test_close;
    return transport;
}

static PAPACC_FRAME_HEADER papacc_test_header(
    PAPACC_U16 type, PAPACC_U32 payload_length)
{
    PAPACC_FRAME_HEADER header = {
        1, 0, 16, type, 0, payload_length
    };
    return header;
}

static int papacc_test_golden_and_partial_header(void)
{
    static const PAPACC_U8 golden[19] = {
        0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
        0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
        0xAA, 0xBB, 0xCC
    };
    const PAPACC_U8 payload[3] = { 0xAA, 0xBB, 0xCC };
    PAPACC_TEST_SINK sink = { { 0 }, 0, 5, 0, 0,
                              PAPACC_TEST_WRITE_PROGRESS };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&sink);
    PAPACC_FRAMED_WRITER writer = PAPACC_FRAMED_WRITER_INITIALIZER;
    PAPACC_FRAME_HEADER header = papacc_test_header(0x1234, 3);
    PAPACC_SIZE consumed;
    PAPACC_FRAMED_WRITER_STATUS status;
    PAPACC_U32 step_count = 0;

    if (papacc_framed_writer_init(&writer, &transport) != PAPACC_RESULT_OK ||
        papacc_framed_writer_init(&writer, &transport) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        sink.write_count != 0) {
        return 1;
    }
    header.message_type = 0;
    while (writer.state == PAPACC_FRAMED_WRITER_STATE_WRITING_HEADER) {
        PAPACC_U32 before = sink.write_count;
        if (papacc_framed_writer_step(
                &writer, payload, 3, &consumed, &status) != PAPACC_RESULT_OK ||
            consumed != 0 || sink.write_count != before + 1 ||
            papacc_framed_writer_begin_frame(&writer, &header) !=
                PAPACC_RESULT_INVALID_STATE) {
            return 2;
        }
        ++step_count;
    }
    if (step_count != 4 ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD ||
        papacc_framed_writer_step(
            &writer, payload, 3, &consumed, &status) != PAPACC_RESULT_OK ||
        consumed != 3 || status != PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_IDLE ||
        sink.write_count != 5 || sink.length != sizeof(golden) ||
        memcmp(sink.bytes, golden, sizeof(golden)) != 0) {
        return 3;
    }
    return 0;
}

static int papacc_test_zero_payload_and_invalid(void)
{
    static const PAPACC_U8 golden[16] = {
        0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    PAPACC_TEST_SINK sink = { { 0 }, 0, 16, 0, 0,
                              PAPACC_TEST_WRITE_PROGRESS };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&sink);
    PAPACC_FRAMED_WRITER writer = PAPACC_FRAMED_WRITER_INITIALIZER;
    PAPACC_FRAME_HEADER header = papacc_test_header(0, 0);
    PAPACC_SIZE consumed = 99;
    PAPACC_FRAMED_WRITER_STATUS status = PAPACC_FRAMED_WRITER_STATUS_PROGRESS;

    if (papacc_framed_writer_init(&writer, &transport) != PAPACC_RESULT_OK ||
        papacc_framed_writer_begin_frame(&writer, &header) !=
            PAPACC_RESULT_PROTOCOL_ERROR || sink.write_count != 0 ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_IDLE) {
        return 10;
    }
    header = papacc_test_header(1, 0);
    header.flags = 1;
    if (papacc_framed_writer_begin_frame(&writer, &header) !=
            PAPACC_RESULT_PROTOCOL_ERROR || writer.state !=
            PAPACC_FRAMED_WRITER_STATE_IDLE) {
        return 11;
    }
    header = papacc_test_header(1, 0);
    header.envelope_major = 2;
    if (papacc_framed_writer_begin_frame(&writer, &header) !=
            PAPACC_RESULT_NOT_SUPPORTED || sink.write_count != 0 ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_IDLE) {
        return 12;
    }
    header = papacc_test_header(1, 0);
    if (papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) != PAPACC_RESULT_OK ||
        consumed != 0 || status != PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE ||
        sink.write_count != 1 || sink.length != 16 ||
        memcmp(sink.bytes, golden, 16) != 0) {
        return 13;
    }
    return 0;
}

static int papacc_test_payload_streaming(void)
{
    const PAPACC_U8 chunk_a[2] = { 1, 2 };
    const PAPACC_U8 chunk_b[1] = { 3 };
    const PAPACC_U8 chunk_c[10] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
    PAPACC_TEST_SINK sink = { { 0 }, 0, 16, 0, 0,
                              PAPACC_TEST_WRITE_PROGRESS };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&sink);
    PAPACC_FRAMED_WRITER writer = PAPACC_FRAMED_WRITER_INITIALIZER;
    PAPACC_FRAME_HEADER header = papacc_test_header(0xBEEF, 6);
    PAPACC_SIZE consumed;
    PAPACC_FRAMED_WRITER_STATUS status;
    PAPACC_U32 writes;

    if (papacc_framed_writer_init(&writer, &transport) != PAPACC_RESULT_OK ||
        papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, chunk_a, 2, &consumed, &status) != PAPACC_RESULT_OK ||
        consumed != 0 || writer.state !=
            PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD) {
        return 20;
    }
    writes = sink.write_count;
    if (papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_NEED_PAYLOAD || consumed != 0 ||
        sink.write_count != writes ||
        papacc_framed_writer_step(
            &writer, chunk_a, 2, &consumed, &status) != PAPACC_RESULT_OK ||
        consumed != 2 ||
        papacc_framed_writer_step(
            &writer, chunk_b, 1, &consumed, &status) != PAPACC_RESULT_OK ||
        consumed != 1 ||
        papacc_framed_writer_step(
            &writer, chunk_c, 10, &consumed, &status) != PAPACC_RESULT_OK ||
        consumed != 3 || status != PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_IDLE || sink.length != 22 ||
        memcmp(&sink.bytes[16], "\x01\x02\x03\x04\x05\x06", 6) != 0) {
        return 21;
    }
    return 0;
}

static int papacc_test_partial_payload(void)
{
    const PAPACC_U8 payload[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    PAPACC_TEST_SINK sink = { { 0 }, 0, 16, 0, 0,
                              PAPACC_TEST_WRITE_PROGRESS };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&sink);
    PAPACC_FRAMED_WRITER writer = PAPACC_FRAMED_WRITER_INITIALIZER;
    PAPACC_FRAME_HEADER header = papacc_test_header(1, 10);
    PAPACC_SIZE consumed;
    PAPACC_SIZE offset = 0;
    PAPACC_FRAMED_WRITER_STATUS status;
    PAPACC_U32 payload_steps = 0;

    if (papacc_framed_writer_init(&writer, &transport) != PAPACC_RESULT_OK ||
        papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, payload, 10, &consumed, &status) != PAPACC_RESULT_OK) {
        return 30;
    }
    sink.max_write = 3;
    while (writer.state == PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD) {
        if (papacc_framed_writer_step(
                &writer, &payload[offset], 10 - offset,
                &consumed, &status) != PAPACC_RESULT_OK || consumed == 0 ||
            consumed > 3) {
            return 31;
        }
        offset += consumed;
        ++payload_steps;
    }
    if (offset != 10 || payload_steps != 4 ||
        status != PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE ||
        memcmp(&sink.bytes[16], payload, 10) != 0) {
        return 32;
    }
    return 0;
}

static int papacc_test_statuses_and_errors(void)
{
    const PAPACC_U8 payload[1] = { 0xAA };
    PAPACC_TEST_SINK sink = { { 0 }, 0, 16, 0, 0,
                              PAPACC_TEST_WRITE_WOULD_BLOCK };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&sink);
    PAPACC_FRAMED_WRITER writer = PAPACC_FRAMED_WRITER_INITIALIZER;
    PAPACC_FRAME_HEADER header = papacc_test_header(1, 1);
    PAPACC_SIZE consumed;
    PAPACC_FRAMED_WRITER_STATUS status;
    PAPACC_U32 writes;

    if (papacc_framed_writer_init(&writer, &transport) != PAPACC_RESULT_OK ||
        papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, payload, 1, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK || consumed != 0 ||
        writer.header_offset != 0 ||
        papacc_framed_writer_step(
            &writer, payload, 1, &consumed, &status) != PAPACC_RESULT_OK ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD) {
        return 40;
    }
    sink.next_mode = PAPACC_TEST_WRITE_WOULD_BLOCK;
    if (papacc_framed_writer_step(
            &writer, payload, 1, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK || consumed != 0 ||
        writer.payload_remaining != 1 ||
        papacc_framed_writer_step(
            &writer, payload, 1, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE) {
        return 41;
    }
    header = papacc_test_header(1, 0);
    if (papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK) {
        return 42;
    }
    sink.next_mode = PAPACC_TEST_WRITE_END_OF_STREAM;
    if (papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_END_OF_STREAM) {
        return 43;
    }
    writes = sink.write_count;
    if (papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM ||
        sink.write_count != writes ||
        papacc_framed_writer_begin_frame(&writer, &header) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_framed_writer_reset(&writer) != PAPACC_RESULT_OK) {
        return 44;
    }
    header = papacc_test_header(1, 1);
    if (papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, payload, 1, &consumed, &status) != PAPACC_RESULT_OK ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD) {
        return 45;
    }
    sink.next_mode = PAPACC_TEST_WRITE_END_OF_STREAM;
    if (papacc_framed_writer_step(
            &writer, payload, 1, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM || consumed != 0 ||
        papacc_framed_writer_reset(&writer) != PAPACC_RESULT_OK) {
        return 46;
    }
    header = papacc_test_header(1, 0);
    if (papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK) {
        return 47;
    }
    sink.next_mode = PAPACC_TEST_WRITE_ERROR;
    if (papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) !=
            PAPACC_RESULT_INTERNAL_ERROR || consumed != 0 ||
        status != PAPACC_FRAMED_WRITER_STATUS_UNSPECIFIED ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_ERROR ||
        papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_framed_writer_reset(&writer) != PAPACC_RESULT_OK ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_IDLE) {
        return 48;
    }
    return 0;
}

static int papacc_test_reset_shutdown_and_consecutive(void)
{
    PAPACC_TEST_SINK sink = { { 0 }, 0, 5, 0, 0,
                              PAPACC_TEST_WRITE_PROGRESS };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&sink);
    PAPACC_FRAMED_WRITER writer = PAPACC_FRAMED_WRITER_INITIALIZER;
    PAPACC_FRAME_HEADER header = papacc_test_header(1, 0);
    PAPACC_SIZE consumed;
    PAPACC_FRAMED_WRITER_STATUS status;

    if (papacc_framed_writer_init(&writer, &transport) != PAPACC_RESULT_OK ||
        papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_reset(&writer) != PAPACC_RESULT_OK ||
        sink.write_count != 0 ||
        papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) != PAPACC_RESULT_OK ||
        papacc_framed_writer_reset(&writer) != PAPACC_RESULT_OK ||
        sink.write_count != 1 || writer.state != PAPACC_FRAMED_WRITER_STATE_IDLE) {
        return 50;
    }
    sink.max_write = 16;
    if (papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE ||
        papacc_framed_writer_begin_frame(&writer, &header) != PAPACC_RESULT_OK ||
        papacc_framed_writer_step(
            &writer, NULL, 0, &consumed, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE) {
        return 51;
    }
    papacc_framed_writer_shutdown(&writer);
    papacc_framed_writer_shutdown(&writer);
    if (sink.close_count != 0 ||
        papacc_transport_connection_is_valid(&transport) != PAPACC_TRUE ||
        writer.state != PAPACC_FRAMED_WRITER_STATE_UNINITIALIZED) {
        return 52;
    }
    return 0;
}

int main(void)
{
    int result = papacc_test_golden_and_partial_header();
    if (result == 0) result = papacc_test_zero_payload_and_invalid();
    if (result == 0) result = papacc_test_payload_streaming();
    if (result == 0) result = papacc_test_partial_payload();
    if (result == 0) result = papacc_test_statuses_and_errors();
    if (result == 0) result = papacc_test_reset_shutdown_and_consecutive();
    return result;
}
