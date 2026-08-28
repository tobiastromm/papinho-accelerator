#include "transport_connection.h"

PAPACC_BOOL papacc_transport_connection_is_valid(
    const PAPACC_TRANSPORT_CONNECTION *transport)
{
    return (transport != NULL && transport->context != NULL &&
            transport->read_fn != NULL && transport->write_fn != NULL &&
            transport->close_fn != NULL)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

static PAPACC_RESULT papacc_transport_connection_check_io_result(
    PAPACC_RESULT result,
    PAPACC_SIZE requested,
    PAPACC_SIZE transferred,
    PAPACC_TRANSPORT_IO_STATUS status)
{
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (status == PAPACC_TRANSPORT_IO_STATUS_PROGRESS) {
        return (transferred > 0 && transferred <= requested)
            ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
    }
    if (status == PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK ||
        status == PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM) {
        return transferred == 0
            ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

PAPACC_RESULT papacc_transport_connection_read(
    PAPACC_TRANSPORT_CONNECTION *transport,
    PAPACC_U8 *buffer,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_SIZE transferred = 0;
    PAPACC_TRANSPORT_IO_STATUS status =
        PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED;
    PAPACC_RESULT result;

    if (out_transferred != NULL) {
        *out_transferred = 0;
    }
    if (out_status != NULL) {
        *out_status = PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED;
    }
    if (out_transferred == NULL || out_status == NULL ||
        (capacity > 0 && buffer == NULL)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (capacity == 0) {
        return PAPACC_RESULT_OK;
    }
    result = transport->read_fn(
        transport->context, buffer, capacity, &transferred, &status);
    result = papacc_transport_connection_check_io_result(
        result, capacity, transferred, status);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    *out_transferred = transferred;
    *out_status = status;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_transport_connection_write(
    PAPACC_TRANSPORT_CONNECTION *transport,
    const PAPACC_U8 *buffer,
    PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_SIZE transferred = 0;
    PAPACC_TRANSPORT_IO_STATUS status =
        PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED;
    PAPACC_RESULT result;

    if (out_transferred != NULL) {
        *out_transferred = 0;
    }
    if (out_status != NULL) {
        *out_status = PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED;
    }
    if (out_transferred == NULL || out_status == NULL ||
        (length > 0 && buffer == NULL)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (length == 0) {
        return PAPACC_RESULT_OK;
    }
    result = transport->write_fn(
        transport->context, buffer, length, &transferred, &status);
    result = papacc_transport_connection_check_io_result(
        result, length, transferred, status);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    *out_transferred = transferred;
    *out_status = status;
    return PAPACC_RESULT_OK;
}

void papacc_transport_connection_close(PAPACC_TRANSPORT_CONNECTION *transport)
{
    PAPACC_TRANSPORT_CLOSE_FN close_fn;
    void *context;

    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return;
    }
    close_fn = transport->close_fn;
    context = transport->context;
    *transport = (PAPACC_TRANSPORT_CONNECTION)
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    close_fn(context);
}
