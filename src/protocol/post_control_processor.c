#include "post_control_processor.h"

static void papacc_post_control_close_scope(
    PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    (void)papacc_data_association_manager_invalidate_session(
        processor->association_manager, processor->session_instance_id);
    (void)papacc_channel_manager_close(
        processor->channel_manager, processor->control_channel_instance_id);
}

static PAPACC_RESULT papacc_post_control_fail(
    PAPACC_POST_CONTROL_PROCESSOR *processor, PAPACC_RESULT result)
{
    papacc_post_control_close_scope(processor);
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_ERROR;
    return result;
}

static void papacc_post_control_close(PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    papacc_post_control_close_scope(processor);
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_CLOSED;
}

PAPACC_RESULT papacc_post_control_processor_init(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 session_instance_id,
    PAPACC_U64 control_channel_instance_id,
    PAPACC_U8 *read_scratch, PAPACC_SIZE read_scratch_capacity)
{
    PAPACC_SESSION *session;
    PAPACC_CHANNEL *channel;
    PAPACC_CONNECTION *connection;
    PAPACC_RESULT result;
    if (processor == NULL || connection_manager == NULL ||
        session_manager == NULL || channel_manager == NULL ||
        association_manager == NULL || session_instance_id == 0 ||
        control_channel_instance_id == 0 || read_scratch == NULL ||
        read_scratch_capacity == 0)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    if (channel_manager->initialized != PAPACC_TRUE ||
        channel_manager->connection_manager != connection_manager ||
        channel_manager->session_manager != session_manager ||
        association_manager->initialized != PAPACC_TRUE ||
        association_manager->session_manager != session_manager ||
        association_manager->channel_manager != channel_manager)
        return PAPACC_RESULT_INVALID_STATE;
    session = papacc_session_manager_find(session_manager, session_instance_id);
    channel = papacc_channel_manager_find(
        channel_manager, control_channel_instance_id);
    if (session == NULL || session->state != PAPACC_SESSION_STATE_ACTIVE ||
        channel == NULL || channel->state != PAPACC_CHANNEL_STATE_BOUND ||
        channel->role != PAPACC_CHANNEL_ROLE_CONTROL ||
        channel->session_instance_id != session_instance_id)
        return PAPACC_RESULT_INVALID_STATE;
    connection = papacc_connection_manager_find(
        connection_manager, channel->connection_instance_id);
    if (connection == NULL ||
        connection->state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        papacc_transport_connection_is_valid(&connection->transport) !=
            PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_init(
        &processor->reader, &connection->transport, read_scratch,
        read_scratch_capacity, PAPACC_DATA_ASSOCIATION_TICKET_SIZE);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_framed_writer_init(&processor->writer,
        &connection->transport);
    if (result != PAPACC_RESULT_OK) {
        papacc_framed_reader_shutdown(&processor->reader);
        return result;
    }
    processor->connection_manager = connection_manager;
    processor->session_manager = session_manager;
    processor->channel_manager = channel_manager;
    processor->association_manager = association_manager;
    processor->connection = connection;
    processor->session_instance_id = session_instance_id;
    processor->control_channel_instance_id = control_channel_instance_id;
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_READY;
    return PAPACC_RESULT_OK;
}

PAPACC_BOOL papacc_post_control_processor_wants_read(
    const PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    return processor != NULL &&
        processor->state == PAPACC_POST_CONTROL_PROCESSOR_STATE_READY
        ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_BOOL papacc_post_control_processor_wants_write(
    const PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    return processor != NULL && processor->state ==
        PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET
        ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_RESULT papacc_post_control_prepare_ticket(
    PAPACC_POST_CONTROL_PROCESSOR *processor, PAPACC_U64 now_ns)
{
    PAPACC_FRAME_HEADER header;
    PAPACC_SIZE written = 0;
    PAPACC_U64 deadline_ns = 0;
    PAPACC_RESULT result = papacc_data_association_manager_issue(
        processor->association_manager, processor->session_instance_id,
        now_ns, &processor->pending_ticket, &deadline_ns);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    result = papacc_data_ticket_encode(
        &processor->pending_ticket, processor->ticket_payload,
        sizeof(processor->ticket_payload), &written);
    if (result == PAPACC_RESULT_OK &&
        written != PAPACC_DATA_ASSOCIATION_TICKET_SIZE)
        result = PAPACC_RESULT_INTERNAL_ERROR;
    header = papacc_data_ticket_frame_header();
    if (result == PAPACC_RESULT_OK)
        result = papacc_framed_writer_begin_frame(&processor->writer, &header);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    processor->ticket_payload_offset = 0;
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_read_once(
    PAPACC_POST_CONTROL_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_POST_CONTROL_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_READER_STATUS reader_status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_post_control_processor_wants_read(processor) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_next(
        &processor->reader, &reader_status, &event);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    if (reader_status == PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (reader_status == PAPACC_FRAMED_READER_STATUS_END_OF_STREAM) {
        papacc_post_control_close(processor);
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (reader_status != PAPACC_FRAMED_READER_STATUS_EVENT) {
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    if (event.header.message_type !=
            PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST ||
        event.header.payload_length != 0 ||
        event.type != PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE)
        return papacc_post_control_fail(
            processor, PAPACC_RESULT_PROTOCOL_ERROR);
    result = papacc_post_control_prepare_ticket(processor, now_ns);
    if (result != PAPACC_RESULT_OK) return result;
    *out_status = PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_write_once(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_POST_CONTROL_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_WRITER_STATUS writer_status;
    PAPACC_SIZE consumed = 0;
    PAPACC_SIZE remaining;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_post_control_processor_wants_write(processor) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    remaining = PAPACC_DATA_ASSOCIATION_TICKET_SIZE -
        processor->ticket_payload_offset;
    result = papacc_framed_writer_step(
        &processor->writer,
        &processor->ticket_payload[processor->ticket_payload_offset],
        remaining, &consumed, &writer_status);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    if (consumed > remaining)
        return papacc_post_control_fail(
            processor, PAPACC_RESULT_INTERNAL_ERROR);
    processor->ticket_payload_offset += consumed;
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM) {
        papacc_post_control_close(processor);
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE) {
        if (processor->ticket_payload_offset !=
            PAPACC_DATA_ASSOCIATION_TICKET_SIZE)
            return papacc_post_control_fail(
                processor, PAPACC_RESULT_INTERNAL_ERROR);
        processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_READY;
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_RESPONSE_COMPLETE;
        return PAPACC_RESULT_OK;
    }
    *out_status = PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

void papacc_post_control_processor_shutdown(
    PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    if (processor == NULL || processor->state ==
        PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED)
        return;
    papacc_post_control_close_scope(processor);
    papacc_framed_reader_shutdown(&processor->reader);
    papacc_framed_writer_shutdown(&processor->writer);
    *processor = (PAPACC_POST_CONTROL_PROCESSOR)
        PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
}
