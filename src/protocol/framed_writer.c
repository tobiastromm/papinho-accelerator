#include "framed_writer.h"

#include <string.h>

static PAPACC_RESULT papacc_framed_writer_fail(
    PAPACC_FRAMED_WRITER *writer,
    PAPACC_RESULT result)
{
    writer->state = PAPACC_FRAMED_WRITER_STATE_ERROR;
    return result;
}

static PAPACC_RESULT papacc_framed_writer_apply_io(
    PAPACC_FRAMED_WRITER *writer,
    PAPACC_SIZE requested,
    PAPACC_SIZE transferred,
    PAPACC_TRANSPORT_IO_STATUS io_status,
    PAPACC_FRAMED_WRITER_STATUS *out_status)
{
    if (io_status == PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (io_status == PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM) {
        writer->state = PAPACC_FRAMED_WRITER_STATE_END_OF_STREAM;
        *out_status = PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (io_status != PAPACC_TRANSPORT_IO_STATUS_PROGRESS || transferred == 0 ||
        transferred > requested) {
        return papacc_framed_writer_fail(
            writer, PAPACC_RESULT_INTERNAL_ERROR);
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_framed_writer_init(
    PAPACC_FRAMED_WRITER *writer,
    PAPACC_TRANSPORT_CONNECTION *transport)
{
    if (writer == NULL || transport == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (writer->state != PAPACC_FRAMED_WRITER_STATE_UNINITIALIZED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    writer->transport = transport;
    writer->state = PAPACC_FRAMED_WRITER_STATE_IDLE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_framed_writer_begin_frame(
    PAPACC_FRAMED_WRITER *writer,
    const PAPACC_FRAME_HEADER *header)
{
    PAPACC_U8 encoded[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE written = 0;
    PAPACC_RESULT result;

    if (writer == NULL || header == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (writer->state != PAPACC_FRAMED_WRITER_STATE_IDLE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_frame_header_encode(
        header, encoded, sizeof(encoded), &written);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (written != PAPACC_FRAME_BASE_HEADER_SIZE) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    memcpy(writer->encoded_header, encoded, sizeof(encoded));
    writer->header_offset = 0;
    writer->payload_remaining = header->payload_length;
    writer->state = PAPACC_FRAMED_WRITER_STATE_WRITING_HEADER;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_framed_writer_step(
    PAPACC_FRAMED_WRITER *writer,
    const PAPACC_U8 *payload,
    PAPACC_SIZE payload_available,
    PAPACC_SIZE *out_payload_consumed,
    PAPACC_FRAMED_WRITER_STATUS *out_status)
{
    PAPACC_SIZE requested;
    PAPACC_SIZE transferred = 0;
    PAPACC_TRANSPORT_IO_STATUS io_status =
        PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED;
    PAPACC_RESULT result;

    if (out_payload_consumed != NULL) {
        *out_payload_consumed = 0;
    }
    if (out_status != NULL) {
        *out_status = PAPACC_FRAMED_WRITER_STATUS_UNSPECIFIED;
    }
    if (writer == NULL || out_payload_consumed == NULL || out_status == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (writer->state == PAPACC_FRAMED_WRITER_STATE_END_OF_STREAM) {
        *out_status = PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (writer->state != PAPACC_FRAMED_WRITER_STATE_WRITING_HEADER &&
        writer->state != PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (payload_available > 0 && payload == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (writer->state == PAPACC_FRAMED_WRITER_STATE_WRITING_HEADER) {
        if (writer->header_offset >= PAPACC_FRAME_BASE_HEADER_SIZE) {
            return papacc_framed_writer_fail(
                writer, PAPACC_RESULT_INTERNAL_ERROR);
        }
        requested = PAPACC_FRAME_BASE_HEADER_SIZE - writer->header_offset;
        result = papacc_transport_connection_write(
            writer->transport,
            &writer->encoded_header[writer->header_offset], requested,
            &transferred, &io_status);
        if (result != PAPACC_RESULT_OK) {
            return papacc_framed_writer_fail(writer, result);
        }
        result = papacc_framed_writer_apply_io(
            writer, requested, transferred, io_status, out_status);
        if (result != PAPACC_RESULT_OK ||
            io_status != PAPACC_TRANSPORT_IO_STATUS_PROGRESS) {
            return result;
        }
        writer->header_offset += transferred;
        if (writer->header_offset < PAPACC_FRAME_BASE_HEADER_SIZE) {
            *out_status = PAPACC_FRAMED_WRITER_STATUS_PROGRESS;
        } else if (writer->payload_remaining == 0) {
            writer->state = PAPACC_FRAMED_WRITER_STATE_IDLE;
            *out_status = PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE;
        } else {
            writer->state = PAPACC_FRAMED_WRITER_STATE_WRITING_PAYLOAD;
            *out_status = PAPACC_FRAMED_WRITER_STATUS_PROGRESS;
        }
        return PAPACC_RESULT_OK;
    }
    if (writer->payload_remaining == 0) {
        return papacc_framed_writer_fail(
            writer, PAPACC_RESULT_INTERNAL_ERROR);
    }
    if (payload_available == 0) {
        *out_status = PAPACC_FRAMED_WRITER_STATUS_NEED_PAYLOAD;
        return PAPACC_RESULT_OK;
    }
    requested = payload_available;
    if (requested > (PAPACC_SIZE)writer->payload_remaining) {
        requested = (PAPACC_SIZE)writer->payload_remaining;
    }
    result = papacc_transport_connection_write(
        writer->transport, payload, requested, &transferred, &io_status);
    if (result != PAPACC_RESULT_OK) {
        return papacc_framed_writer_fail(writer, result);
    }
    result = papacc_framed_writer_apply_io(
        writer, requested, transferred, io_status, out_status);
    if (result != PAPACC_RESULT_OK ||
        io_status != PAPACC_TRANSPORT_IO_STATUS_PROGRESS) {
        return result;
    }
    writer->payload_remaining -= (PAPACC_U32)transferred;
    *out_payload_consumed = transferred;
    if (writer->payload_remaining == 0) {
        writer->state = PAPACC_FRAMED_WRITER_STATE_IDLE;
        *out_status = PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE;
    } else {
        *out_status = PAPACC_FRAMED_WRITER_STATUS_PROGRESS;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_framed_writer_reset(PAPACC_FRAMED_WRITER *writer)
{
    if (writer == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (writer->state == PAPACC_FRAMED_WRITER_STATE_UNINITIALIZED) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    memset(writer->encoded_header, 0, sizeof(writer->encoded_header));
    writer->header_offset = 0;
    writer->payload_remaining = 0;
    writer->state = PAPACC_FRAMED_WRITER_STATE_IDLE;
    return PAPACC_RESULT_OK;
}

void papacc_framed_writer_shutdown(PAPACC_FRAMED_WRITER *writer)
{
    if (writer == NULL) {
        return;
    }
    *writer = (PAPACC_FRAMED_WRITER)PAPACC_FRAMED_WRITER_INITIALIZER;
}
