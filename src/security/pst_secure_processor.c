#include "pst_secure_processor.h"

static PAPACC_RESULT papacc_pst_processor_map_result(PST_RESULT result)
{
    switch (result) {
    case PST_RESULT_OK: return PAPACC_RESULT_OK;
    case PST_RESULT_INVALID_ARGUMENT: return PAPACC_RESULT_INVALID_ARGUMENT;
    case PST_RESULT_INVALID_STATE: return PAPACC_RESULT_INVALID_STATE;
    case PST_RESULT_UNSUPPORTED: return PAPACC_RESULT_NOT_SUPPORTED;
    case PST_RESULT_OUT_OF_MEMORY: return PAPACC_RESULT_OUT_OF_MEMORY;
    default: return PAPACC_RESULT_INTERNAL_ERROR;
    }
}

static PAPACC_PST_SECURE_STEP papacc_pst_processor_step(pst_u32 operation)
{
    switch (operation) {
    case PST_OPERATION_COMPLETE: return PAPACC_PST_SECURE_STEP_COMPLETE;
    case PST_OPERATION_NEED_READ: return PAPACC_PST_SECURE_STEP_NEED_READ;
    case PST_OPERATION_NEED_WRITE: return PAPACC_PST_SECURE_STEP_NEED_WRITE;
    case PST_OPERATION_NEED_READ_WRITE:
        return PAPACC_PST_SECURE_STEP_NEED_READ_WRITE;
    case PST_OPERATION_CLOSED: return PAPACC_PST_SECURE_STEP_CLOSED;
    default: return PAPACC_PST_SECURE_STEP_FAILED;
    }
}

PAPACC_RESULT papacc_pst_secure_processor_init(
    PAPACC_PST_SECURE_PROCESSOR *processor, pst_runtime *runtime,
    const PST_CONNECTION_CONFIG *config, PAPACC_U64 deadline_ns)
{
    PST_RESULT result;
    if (processor == NULL || runtime == NULL || config == NULL ||
        deadline_ns == 0U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_PST_SECURE_PROCESSOR_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    result = pst_connection_create(runtime, config, &processor->connection);
    if (result != PST_RESULT_OK) {
        processor->source_result = result;
        return papacc_pst_processor_map_result(result);
    }
    processor->deadline_ns = deadline_ns;
    processor->source_result = PST_RESULT_OK;
    processor->state = PAPACC_PST_SECURE_PROCESSOR_CREATED;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_processor_attach(
    PAPACC_PST_SECURE_PROCESSOR *processor, pst_transport *transport,
    PAPACC_BOOL *ownership_accepted)
{
    pst_u32 accepted = 0U;
    PST_RESULT result;
    if (processor == NULL || transport == NULL || ownership_accepted == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *ownership_accepted = PAPACC_FALSE;
    if (processor->state != PAPACC_PST_SECURE_PROCESSOR_CREATED)
        return PAPACC_RESULT_INVALID_STATE;
    result = pst_connection_attach(processor->connection, transport,
        PST_OWNERSHIP_TRANSFERRED, &accepted);
    if (accepted != 0U) *ownership_accepted = PAPACC_TRUE;
    if (result != PST_RESULT_OK) {
        processor->source_result = result;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
        return papacc_pst_processor_map_result(result);
    }
    processor->state = PAPACC_PST_SECURE_PROCESSOR_ATTACHED;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_processor_interest(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_BOOL *read_interest,
    PAPACC_BOOL *write_interest)
{
    pst_u32 interest;
    PST_RESULT result;
    if (processor == NULL || read_interest == NULL || write_interest == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->connection == NULL)
        return PAPACC_RESULT_INVALID_STATE;
    result = pst_connection_get_interest(processor->connection, &interest);
    if (result != PST_RESULT_OK) return papacc_pst_processor_map_result(result);
    *read_interest = (interest & PST_INTEREST_READ) != 0U ?
        PAPACC_TRUE : PAPACC_FALSE;
    *write_interest = (interest & PST_INTEREST_WRITE) != 0U ?
        PAPACC_TRUE : PAPACC_FALSE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_processor_handshake_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_PST_SECURE_STEP *step)
{
    pst_u32 operation;
    PST_RESULT source_error = PST_RESULT_OK;
    PST_RESULT result;
    if (processor == NULL || step == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_PST_SECURE_PROCESSOR_ATTACHED &&
        processor->state != PAPACC_PST_SECURE_PROCESSOR_HANDSHAKING)
        return PAPACC_RESULT_INVALID_STATE;
    processor->state = PAPACC_PST_SECURE_PROCESSOR_HANDSHAKING;
    result = pst_connection_handshake(processor->connection, &operation,
        &source_error);
    if (result != PST_RESULT_OK) {
        processor->source_result = result;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
        *step = PAPACC_PST_SECURE_STEP_FAILED;
        return papacc_pst_processor_map_result(result);
    }
    *step = papacc_pst_processor_step(operation);
    if (*step == PAPACC_PST_SECURE_STEP_COMPLETE)
        processor->state = PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED;
    else if (*step == PAPACC_PST_SECURE_STEP_FAILED) {
        processor->source_result = source_error;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_pst_processor_io_result(
    PAPACC_PST_SECURE_PROCESSOR *processor, const PST_IO_RESULT *io,
    PAPACC_SIZE *transferred, PAPACC_PST_SECURE_STEP *step)
{
    *transferred = io->bytes_transferred;
    *step = papacc_pst_processor_step(io->operation);
    if (*step == PAPACC_PST_SECURE_STEP_CLOSED)
        processor->state = PAPACC_PST_SECURE_PROCESSOR_CLOSED;
    else if (*step == PAPACC_PST_SECURE_STEP_FAILED) {
        processor->source_result = io->error;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_processor_read_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_U8 *buffer,
    PAPACC_SIZE capacity, PAPACC_SIZE *transferred,
    PAPACC_PST_SECURE_STEP *step)
{
    PST_IO_RESULT io;
    PST_RESULT result;
    if (processor == NULL || buffer == NULL || capacity == 0U ||
        transferred == NULL || step == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED)
        return PAPACC_RESULT_INVALID_STATE;
    result = pst_connection_read(processor->connection, buffer, capacity, &io);
    if (result != PST_RESULT_OK) {
        processor->source_result = result;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
        return papacc_pst_processor_map_result(result);
    }
    return papacc_pst_processor_io_result(processor, &io, transferred, step);
}

PAPACC_RESULT papacc_pst_secure_processor_write_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, const PAPACC_U8 *buffer,
    PAPACC_SIZE length, PAPACC_SIZE *transferred,
    PAPACC_PST_SECURE_STEP *step)
{
    PST_IO_RESULT io;
    PST_RESULT result;
    if (processor == NULL || buffer == NULL || length == 0U ||
        transferred == NULL || step == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED)
        return PAPACC_RESULT_INVALID_STATE;
    result = pst_connection_write(processor->connection, buffer, length, &io);
    if (result != PST_RESULT_OK) {
        processor->source_result = result;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
        return papacc_pst_processor_map_result(result);
    }
    return papacc_pst_processor_io_result(processor, &io, transferred, step);
}

PAPACC_RESULT papacc_pst_secure_processor_shutdown_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_PST_SECURE_STEP *step)
{
    pst_u32 operation;
    PST_RESULT source_error = PST_RESULT_OK;
    PST_RESULT result;
    if (processor == NULL || step == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED &&
        processor->state != PAPACC_PST_SECURE_PROCESSOR_SHUTTING_DOWN)
        return PAPACC_RESULT_INVALID_STATE;
    processor->state = PAPACC_PST_SECURE_PROCESSOR_SHUTTING_DOWN;
    result = pst_connection_shutdown(processor->connection, &operation,
        &source_error);
    if (result != PST_RESULT_OK) {
        processor->source_result = result;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
        return papacc_pst_processor_map_result(result);
    }
    *step = papacc_pst_processor_step(operation);
    if (*step == PAPACC_PST_SECURE_STEP_COMPLETE ||
        *step == PAPACC_PST_SECURE_STEP_CLOSED)
        processor->state = PAPACC_PST_SECURE_PROCESSOR_CLOSED;
    else if (*step == PAPACC_PST_SECURE_STEP_FAILED) {
        processor->source_result = source_error;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_pst_secure_processor_check_deadline(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_BOOL *expired)
{
    if (processor == NULL || expired == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state == PAPACC_PST_SECURE_PROCESSOR_UNINITIALIZED ||
        processor->state == PAPACC_PST_SECURE_PROCESSOR_CLOSED)
        return PAPACC_RESULT_INVALID_STATE;
    *expired = (PAPACC_I64)(now_ns - processor->deadline_ns) >= 0 ?
        PAPACC_TRUE : PAPACC_FALSE;
    if (*expired && processor->state != PAPACC_PST_SECURE_PROCESSOR_FAILED) {
        processor->source_result = PST_RESULT_WAIT_TIMEOUT;
        processor->state = PAPACC_PST_SECURE_PROCESSOR_FAILED;
    }
    return PAPACC_RESULT_OK;
}

void papacc_pst_secure_processor_release(
    PAPACC_PST_SECURE_PROCESSOR *processor)
{
    if (processor == NULL) return;
    if (processor->connection != NULL)
        pst_connection_release(processor->connection);
    *processor = (PAPACC_PST_SECURE_PROCESSOR)
        PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
    processor->state = PAPACC_PST_SECURE_PROCESSOR_CLOSED;
}
