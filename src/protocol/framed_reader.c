#include "framed_reader.h"

static PAPACC_RESULT papacc_framed_reader_fail(
    PAPACC_FRAMED_READER *reader,
    PAPACC_RESULT result)
{
    reader->state = PAPACC_FRAMED_READER_STATE_ERROR;
    return result;
}

static PAPACC_RESULT papacc_framed_reader_process_buffer(
    PAPACC_FRAMED_READER *reader,
    PAPACC_FRAMED_READER_STATUS *out_status,
    PAPACC_FRAME_PARSER_EVENT *out_event)
{
    PAPACC_SIZE consumed = 0;
    PAPACC_RESULT result = papacc_frame_parser_feed(
        &reader->parser, &reader->read_buffer[reader->buffered_offset],
        reader->buffered_length, &consumed, out_event);

    if (result != PAPACC_RESULT_OK) {
        *out_event = (PAPACC_FRAME_PARSER_EVENT)
            PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
        return papacc_framed_reader_fail(reader, result);
    }
    if (consumed == 0 &&
        out_event->type == PAPACC_FRAME_PARSER_EVENT_NONE) {
        return papacc_framed_reader_fail(
            reader, PAPACC_RESULT_INTERNAL_ERROR);
    }
    if (consumed > reader->buffered_length) {
        *out_event = (PAPACC_FRAME_PARSER_EVENT)
            PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
        return papacc_framed_reader_fail(
            reader, PAPACC_RESULT_INTERNAL_ERROR);
    }
    reader->buffered_offset += consumed;
    reader->buffered_length -= consumed;
    if (reader->buffered_length == 0) {
        reader->buffered_offset = 0;
    }
    if (out_event->type != PAPACC_FRAME_PARSER_EVENT_NONE) {
        *out_status = PAPACC_FRAMED_READER_STATUS_EVENT;
    } else {
        *out_status = PAPACC_FRAMED_READER_STATUS_NEED_MORE_DATA;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_framed_reader_init(
    PAPACC_FRAMED_READER *reader,
    PAPACC_TRANSPORT_CONNECTION *transport,
    PAPACC_U8 *read_buffer,
    PAPACC_SIZE read_buffer_capacity,
    PAPACC_U32 max_payload_length)
{
    PAPACC_RESULT result;

    if (reader == NULL || transport == NULL || read_buffer == NULL ||
        read_buffer_capacity == 0) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (reader->state != PAPACC_FRAMED_READER_STATE_UNINITIALIZED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_frame_parser_init(&reader->parser, max_payload_length);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    reader->transport = transport;
    reader->read_buffer = read_buffer;
    reader->read_buffer_capacity = read_buffer_capacity;
    reader->buffered_offset = 0;
    reader->buffered_length = 0;
    reader->state = PAPACC_FRAMED_READER_STATE_READY;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_framed_reader_next(
    PAPACC_FRAMED_READER *reader,
    PAPACC_FRAMED_READER_STATUS *out_status,
    PAPACC_FRAME_PARSER_EVENT *out_event)
{
    PAPACC_SIZE transferred = 0;
    PAPACC_TRANSPORT_IO_STATUS io_status =
        PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED;
    PAPACC_RESULT result;

    if (out_status != NULL) {
        *out_status = PAPACC_FRAMED_READER_STATUS_UNSPECIFIED;
    }
    if (out_event != NULL) {
        *out_event = (PAPACC_FRAME_PARSER_EVENT)
            PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
    }
    if (reader == NULL || out_status == NULL || out_event == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (reader->state == PAPACC_FRAMED_READER_STATE_END_OF_STREAM) {
        *out_status = PAPACC_FRAMED_READER_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (reader->state != PAPACC_FRAMED_READER_STATE_READY) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (reader->buffered_offset > reader->read_buffer_capacity ||
        reader->buffered_length >
            reader->read_buffer_capacity - reader->buffered_offset) {
        return papacc_framed_reader_fail(
            reader, PAPACC_RESULT_INTERNAL_ERROR);
    }
    if (reader->buffered_length > 0) {
        return papacc_framed_reader_process_buffer(
            reader, out_status, out_event);
    }
    result = papacc_transport_connection_read(
        reader->transport, reader->read_buffer, reader->read_buffer_capacity,
        &transferred, &io_status);
    if (result != PAPACC_RESULT_OK) {
        return papacc_framed_reader_fail(reader, result);
    }
    if (io_status == PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (io_status == PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM) {
        result = papacc_frame_parser_finish(&reader->parser);
        if (result != PAPACC_RESULT_OK) {
            return papacc_framed_reader_fail(reader, result);
        }
        reader->state = PAPACC_FRAMED_READER_STATE_END_OF_STREAM;
        *out_status = PAPACC_FRAMED_READER_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (io_status != PAPACC_TRANSPORT_IO_STATUS_PROGRESS || transferred == 0 ||
        transferred > reader->read_buffer_capacity) {
        return papacc_framed_reader_fail(
            reader, PAPACC_RESULT_INTERNAL_ERROR);
    }
    reader->buffered_offset = 0;
    reader->buffered_length = transferred;
    return papacc_framed_reader_process_buffer(
        reader, out_status, out_event);
}

PAPACC_RESULT papacc_framed_reader_reset(PAPACC_FRAMED_READER *reader)
{
    if (reader == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (reader->state == PAPACC_FRAMED_READER_STATE_UNINITIALIZED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    papacc_frame_parser_reset(&reader->parser);
    reader->buffered_offset = 0;
    reader->buffered_length = 0;
    reader->state = PAPACC_FRAMED_READER_STATE_READY;
    return PAPACC_RESULT_OK;
}

void papacc_framed_reader_shutdown(PAPACC_FRAMED_READER *reader)
{
    if (reader == NULL) {
        return;
    }
    *reader = (PAPACC_FRAMED_READER)PAPACC_FRAMED_READER_INITIALIZER;
}
