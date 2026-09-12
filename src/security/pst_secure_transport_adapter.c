#include "pst_secure_transport_adapter.h"

static PAPACC_RESULT papacc_pst_secure_adapter_status(
    PAPACC_PST_SECURE_STEP step, PAPACC_SIZE transferred,
    PAPACC_TRANSPORT_IO_STATUS *status)
{
    if (transferred != 0U) {
        *status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    if (step == PAPACC_PST_SECURE_STEP_NEED_READ ||
        step == PAPACC_PST_SECURE_STEP_NEED_WRITE ||
        step == PAPACC_PST_SECURE_STEP_NEED_READ_WRITE) {
        *status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (step == PAPACC_PST_SECURE_STEP_CLOSED) {
        *status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (step == PAPACC_PST_SECURE_STEP_COMPLETE) {
        *status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT papacc_pst_secure_adapter_read(void *context,
    PAPACC_U8 *buffer, PAPACC_SIZE capacity, PAPACC_SIZE *out_transferred,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter =
        (PAPACC_PST_SECURE_TRANSPORT_ADAPTER *)context;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_RESULT result;
    if (adapter == NULL || adapter->closed == PAPACC_TRUE ||
        adapter->processor == NULL) return PAPACC_RESULT_INVALID_STATE;
    result = papacc_pst_secure_processor_read_once(adapter->processor, buffer,
        capacity, out_transferred, &step);
    if (result != PAPACC_RESULT_OK) return result;
    return papacc_pst_secure_adapter_status(step, *out_transferred, out_status);
}

static PAPACC_RESULT papacc_pst_secure_adapter_write(void *context,
    const PAPACC_U8 *buffer, PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter =
        (PAPACC_PST_SECURE_TRANSPORT_ADAPTER *)context;
    PAPACC_PST_SECURE_STEP step;
    PAPACC_RESULT result;
    if (adapter == NULL || adapter->closed == PAPACC_TRUE ||
        adapter->processor == NULL) return PAPACC_RESULT_INVALID_STATE;
    result = papacc_pst_secure_processor_write_once(adapter->processor, buffer,
        length, out_transferred, &step);
    if (result != PAPACC_RESULT_OK) return result;
    return papacc_pst_secure_adapter_status(step, *out_transferred, out_status);
}

static void papacc_pst_secure_adapter_close(void *context)
{
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter =
        (PAPACC_PST_SECURE_TRANSPORT_ADAPTER *)context;
    if (adapter == NULL || adapter->closed == PAPACC_TRUE) return;
    if (adapter->registered == PAPACC_TRUE && adapter->scheduler != NULL &&
        adapter->processor != NULL && adapter->processor->connection != NULL) {
        (void)papacc_pst_secure_scheduler_remove_connection(
            adapter->scheduler, adapter->processor->connection);
    }
    adapter->registered = PAPACC_FALSE;
    if (adapter->processor != NULL)
        papacc_pst_secure_processor_release(adapter->processor);
    adapter->closed = PAPACC_TRUE;
}

PAPACC_RESULT papacc_pst_secure_transport_adapter_init(
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter,
    PAPACC_PST_SECURE_PROCESSOR *processor,
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    PAPACC_TRANSPORT_CONNECTION *out_transport)
{
    if (adapter == NULL || processor == NULL || scheduler == NULL ||
        out_transport == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (adapter->processor != NULL || adapter->closed == PAPACC_TRUE ||
        processor->state != PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED ||
        processor->connection == NULL || scheduler->initialized != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if (papacc_transport_connection_is_valid(out_transport) == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    adapter->processor = processor;
    adapter->scheduler = scheduler;
    adapter->registered = PAPACC_TRUE;
    out_transport->context = adapter;
    out_transport->read_fn = papacc_pst_secure_adapter_read;
    out_transport->write_fn = papacc_pst_secure_adapter_write;
    out_transport->close_fn = papacc_pst_secure_adapter_close;
    return PAPACC_RESULT_OK;
}
