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

PAPACC_RESULT papacc_transport_connection_prepare_close(
    PAPACC_TRANSPORT_CONNECTION *transport,
    PAPACC_BOOL *out_ready)
{
    PAPACC_BOOL ready = PAPACC_FALSE;
    PAPACC_RESULT result;

    if (out_ready != NULL) {
        *out_ready = PAPACC_FALSE;
    }
    if (out_ready == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (transport->pre_close_fn == NULL) {
        *out_ready = PAPACC_TRUE;
        return PAPACC_RESULT_OK;
    }
    result = transport->pre_close_fn(transport->context, &ready);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    *out_ready = ready == PAPACC_TRUE ? PAPACC_TRUE : PAPACC_FALSE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_transport_connection_try_close(
    PAPACC_TRANSPORT_CONNECTION *transport, PAPACC_BOOL *out_closed)
{
    PAPACC_RESULT result;
    PAPACC_BOOL closed = PAPACC_FALSE;
    if (out_closed == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *out_closed = PAPACC_FALSE;
    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (transport->try_close_fn != NULL) {
        result = transport->try_close_fn(transport->context, &closed);
        if (result != PAPACC_RESULT_OK || closed != PAPACC_TRUE)
            return result;
    } else {
        /* Existing non-PST infallible transports retain their contract.
         * PST adapters always install the factual completion hook. */
        transport->close_fn(transport->context);
    }
    *transport = (PAPACC_TRANSPORT_CONNECTION)
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    *out_closed = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}
void papacc_transport_connection_close(PAPACC_TRANSPORT_CONNECTION *transport)
{
    PAPACC_BOOL closed;
    (void)papacc_transport_connection_try_close(transport, &closed);
}
