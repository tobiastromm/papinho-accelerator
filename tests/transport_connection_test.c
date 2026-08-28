#include "transport_connection.h"

#include <string.h>

typedef enum PAPACC_TEST_MODE {
    PAPACC_TEST_MODE_PROGRESS = 0,
    PAPACC_TEST_MODE_WOULD_BLOCK = 1,
    PAPACC_TEST_MODE_EOF = 2,
    PAPACC_TEST_MODE_ERROR = 3
} PAPACC_TEST_MODE;

typedef struct PAPACC_TEST_CONTEXT {
    PAPACC_U8 read_data[4];
    PAPACC_SIZE read_offset;
    PAPACC_U8 written[10];
    PAPACC_SIZE written_length;
    PAPACC_SIZE read_limit;
    PAPACC_SIZE write_limit;
    PAPACC_U32 read_calls;
    PAPACC_U32 write_calls;
    PAPACC_U32 close_calls;
    PAPACC_TEST_MODE read_mode;
    PAPACC_TEST_MODE write_mode;
} PAPACC_TEST_CONTEXT;

static PAPACC_RESULT papacc_test_complete(
    PAPACC_TEST_MODE mode, PAPACC_SIZE transferred,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    if (mode == PAPACC_TEST_MODE_ERROR) {
        *out_transferred = 9;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    *out_transferred = transferred;
    *out_status = mode == PAPACC_TEST_MODE_WOULD_BLOCK
        ? PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK
        : (mode == PAPACC_TEST_MODE_EOF
               ? PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM
               : PAPACC_TRANSPORT_IO_STATUS_PROGRESS);
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_test_read(
    void *opaque, PAPACC_U8 *buffer, PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TEST_CONTEXT *context = (PAPACC_TEST_CONTEXT *)opaque;
    PAPACC_SIZE available;
    PAPACC_SIZE transferred;
    ++context->read_calls;
    if (context->read_mode != PAPACC_TEST_MODE_PROGRESS) {
        return papacc_test_complete(
            context->read_mode, 0, out_transferred, out_status);
    }
    available = sizeof(context->read_data) - context->read_offset;
    transferred = capacity < available ? capacity : available;
    if (transferred > context->read_limit) {
        transferred = context->read_limit;
    }
    memcpy(buffer, &context->read_data[context->read_offset], transferred);
    context->read_offset += transferred;
    return papacc_test_complete(
        PAPACC_TEST_MODE_PROGRESS, transferred, out_transferred, out_status);
}

static PAPACC_RESULT papacc_test_write(
    void *opaque, const PAPACC_U8 *buffer, PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TEST_CONTEXT *context = (PAPACC_TEST_CONTEXT *)opaque;
    PAPACC_SIZE transferred = length < context->write_limit
        ? length : context->write_limit;
    ++context->write_calls;
    if (context->write_mode != PAPACC_TEST_MODE_PROGRESS) {
        return papacc_test_complete(
            context->write_mode, 0, out_transferred, out_status);
    }
    memcpy(&context->written[context->written_length], buffer, transferred);
    context->written_length += transferred;
    return papacc_test_complete(
        PAPACC_TEST_MODE_PROGRESS, transferred, out_transferred, out_status);
}

static void papacc_test_close(void *opaque)
{
    PAPACC_TEST_CONTEXT *context = (PAPACC_TEST_CONTEXT *)opaque;
    ++context->close_calls;
}

static PAPACC_TRANSPORT_CONNECTION papacc_test_make_transport(
    PAPACC_TEST_CONTEXT *context)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    transport.context = context;
    transport.read_fn = papacc_test_read;
    transport.write_fn = papacc_test_write;
    transport.close_fn = papacc_test_close;
    return transport;
}

static int papacc_test_partial_io(void)
{
    PAPACC_TEST_CONTEXT context = {
        { 0xAA, 0xBB, 0xCC, 0xDD }, 0, { 0 }, 0, 2, 3, 0, 0, 0,
        PAPACC_TEST_MODE_PROGRESS, PAPACC_TEST_MODE_PROGRESS
    };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_make_transport(&context);
    PAPACC_U8 output[10] = { 0 };
    const PAPACC_U8 input[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    PAPACC_SIZE transferred = 99;
    PAPACC_TRANSPORT_IO_STATUS status =
        PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;

    if (papacc_transport_connection_read(
            &transport, output, sizeof(output), &transferred, &status) !=
            PAPACC_RESULT_OK || transferred != 2 ||
        status != PAPACC_TRANSPORT_IO_STATUS_PROGRESS || output[0] != 0xAA ||
        output[1] != 0xBB ||
        papacc_transport_connection_read(
            &transport, &output[2], sizeof(output) - 2,
            &transferred, &status) != PAPACC_RESULT_OK || transferred != 2 ||
        output[2] != 0xCC || output[3] != 0xDD) {
        return 1;
    }
    if (papacc_transport_connection_write(
            &transport, input, sizeof(input), &transferred, &status) !=
            PAPACC_RESULT_OK || transferred != 3 ||
        status != PAPACC_TRANSPORT_IO_STATUS_PROGRESS ||
        context.write_calls != 1 || memcmp(context.written, input, 3) != 0) {
        return 2;
    }
    return 0;
}

static int papacc_test_status_zero_and_errors(void)
{
    PAPACC_TEST_CONTEXT context = {
        { 0 }, 0, { 0 }, 0, 1, 1, 0, 0, 0,
        PAPACC_TEST_MODE_WOULD_BLOCK, PAPACC_TEST_MODE_WOULD_BLOCK
    };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_make_transport(&context);
    PAPACC_U8 byte = 0;
    PAPACC_SIZE transferred = 99;
    PAPACC_TRANSPORT_IO_STATUS status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;

    if (papacc_transport_connection_read(
            &transport, &byte, 1, &transferred, &status) != PAPACC_RESULT_OK ||
        transferred != 0 || status != PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK ||
        papacc_transport_connection_write(
            &transport, &byte, 1, &transferred, &status) != PAPACC_RESULT_OK ||
        transferred != 0 || status != PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK) {
        return 10;
    }
    context.read_mode = PAPACC_TEST_MODE_EOF;
    if (papacc_transport_connection_read(
            &transport, &byte, 1, &transferred, &status) != PAPACC_RESULT_OK ||
        transferred != 0 || status != PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM) {
        return 11;
    }
    if (papacc_transport_connection_read(
            &transport, NULL, 0, &transferred, &status) != PAPACC_RESULT_OK ||
        transferred != 0 || status != PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED ||
        papacc_transport_connection_write(
            &transport, NULL, 0, &transferred, &status) != PAPACC_RESULT_OK ||
        transferred != 0 || status != PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED ||
        context.read_calls != 2 || context.write_calls != 1) {
        return 12;
    }
    context.read_mode = PAPACC_TEST_MODE_ERROR;
    context.write_mode = PAPACC_TEST_MODE_ERROR;
    if (papacc_transport_connection_read(
            &transport, &byte, 1, &transferred, &status) !=
            PAPACC_RESULT_INTERNAL_ERROR || transferred != 0 ||
        status != PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED ||
        papacc_transport_connection_write(
            &transport, &byte, 1, &transferred, &status) !=
            PAPACC_RESULT_INTERNAL_ERROR || transferred != 0 ||
        status != PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED) {
        return 13;
    }
    return 0;
}

static int papacc_test_close_and_validation(void)
{
    PAPACC_TEST_CONTEXT context = {
        { 0 }, 0, { 0 }, 0, 1, 1, 0, 0, 0,
        PAPACC_TEST_MODE_PROGRESS, PAPACC_TEST_MODE_PROGRESS
    };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_make_transport(&context);
    PAPACC_U8 byte = 0;
    PAPACC_SIZE transferred = 99;
    PAPACC_TRANSPORT_IO_STATUS status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;

    papacc_transport_connection_close(&transport);
    papacc_transport_connection_close(&transport);
    if (context.close_calls != 1 ||
        papacc_transport_connection_read(
            &transport, &byte, 1, &transferred, &status) !=
            PAPACC_RESULT_INVALID_STATE || transferred != 0 ||
        status != PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED ||
        papacc_transport_connection_write(
            &transport, &byte, 1, &transferred, &status) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_transport_connection_read(
            NULL, &byte, 1, &transferred, &status) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_transport_connection_write(
            &transport, NULL, 1, &transferred, &status) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 20;
    }
    return 0;
}

int main(void)
{
    int result = papacc_test_partial_io();
    if (result == 0) {
        result = papacc_test_status_zero_and_errors();
    }
    if (result == 0) {
        result = papacc_test_close_and_validation();
    }
    return result;
}
