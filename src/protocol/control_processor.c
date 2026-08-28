#include "control_processor.h"

#include <string.h>

static void papacc_control_processor_close_scope(
    PAPACC_CONTROL_PROCESSOR *processor)
{
    if (processor->control_channel_instance_id != 0) {
        (void)papacc_channel_manager_close(
            processor->channel_manager, processor->control_channel_instance_id);
    } else if (processor->connection != NULL) {
        papacc_connection_close(processor->connection);
    }
}

static PAPACC_RESULT papacc_control_processor_fail(
    PAPACC_CONTROL_PROCESSOR *processor, PAPACC_RESULT result)
{
    papacc_control_processor_close_scope(processor);
    processor->state = PAPACC_CONTROL_PROCESSOR_STATE_ERROR;
    return result;
}

static PAPACC_RESULT papacc_control_processor_commit(
    PAPACC_CONTROL_PROCESSOR *processor)
{
    PAPACC_CONTROL_PROTOCOL_VERSION version =
        PAPACC_CONTROL_PROTOCOL_VERSION_INITIALIZER;
    PAPACC_CONTROL_PROTOCOL_VERSION selected = {
        PAPACC_CONTROL_PROTOCOL_MAJOR, PAPACC_CONTROL_PROTOCOL_MINOR
    };
    PAPACC_SESSION *session = NULL;
    PAPACC_CHANNEL *channel = NULL;
    PAPACC_FRAME_HEADER accept_header;
    PAPACC_SIZE written = 0;
    PAPACC_RESULT result;

    if (processor->open_payload_received != 4)
        return papacc_control_processor_fail(
            processor, PAPACC_RESULT_PROTOCOL_ERROR);
    result = papacc_control_open_decode(
        processor->open_payload, 4, &version);
    if (result != PAPACC_RESULT_OK)
        return papacc_control_processor_fail(processor, result);
    result = papacc_session_manager_publish(processor->session_manager, &session);
    if (result != PAPACC_RESULT_OK)
        return papacc_control_processor_fail(processor, result);
    result = papacc_channel_manager_bind(
        processor->channel_manager, session->session_instance_id,
        processor->connection->connection_instance_id,
        PAPACC_CHANNEL_ROLE_CONTROL, &channel);
    if (result != PAPACC_RESULT_OK) {
        PAPACC_U64 session_id = session->session_instance_id;
        (void)papacc_session_manager_remove(
            processor->session_manager, session_id);
        return papacc_control_processor_fail(processor, result);
    }
    processor->session_instance_id = session->session_instance_id;
    processor->control_channel_instance_id = channel->channel_instance_id;
    result = papacc_control_accept_encode(
        &selected, processor->accept_payload,
        sizeof(processor->accept_payload), &written);
    if (result == PAPACC_RESULT_OK && written != 4)
        result = PAPACC_RESULT_INTERNAL_ERROR;
    accept_header = papacc_control_accept_frame_header();
    if (result == PAPACC_RESULT_OK)
        result = papacc_framed_writer_begin_frame(
            &processor->writer, &accept_header);
    if (result != PAPACC_RESULT_OK)
        return papacc_control_processor_fail(processor, result);
    processor->accept_payload_offset = 0;
    processor->state =
        PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_control_processor_init(
    PAPACC_CONTROL_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_U64 connection_instance_id, PAPACC_U8 *scratch,
    PAPACC_SIZE scratch_capacity, PAPACC_U32 max_payload_length,
    PAPACC_U64 establishment_deadline_ns)
{
    PAPACC_CONNECTION *connection;
    PAPACC_RESULT result;
    if (processor == NULL || connection_manager == NULL ||
        session_manager == NULL || channel_manager == NULL || scratch == NULL ||
        scratch_capacity == 0 || connection_instance_id == 0)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_CONTROL_PROCESSOR_STATE_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    if (channel_manager->initialized != PAPACC_TRUE ||
        channel_manager->connection_manager != connection_manager ||
        channel_manager->session_manager != session_manager)
        return PAPACC_RESULT_INVALID_STATE;
    connection = papacc_connection_manager_find(
        connection_manager, connection_instance_id);
    if (connection == NULL || connection->state != PAPACC_CONNECTION_STATE_PENDING ||
        papacc_transport_connection_is_valid(&connection->transport) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_init(
        &processor->reader, &connection->transport, scratch, scratch_capacity,
        max_payload_length);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_framed_writer_init(&processor->writer, &connection->transport);
    if (result != PAPACC_RESULT_OK) {
        papacc_framed_reader_shutdown(&processor->reader);
        return result;
    }
    processor->connection_manager = connection_manager;
    processor->session_manager = session_manager;
    processor->channel_manager = channel_manager;
    processor->connection = connection;
    processor->establishment_deadline_ns = establishment_deadline_ns;
    processor->state =
        PAPACC_CONTROL_PROCESSOR_STATE_WAITING_CONTROL_OPEN_HEADER;
    return PAPACC_RESULT_OK;
}

PAPACC_BOOL papacc_control_processor_wants_read(
    const PAPACC_CONTROL_PROCESSOR *processor)
{
    return processor != NULL &&
        (processor->state ==
             PAPACC_CONTROL_PROCESSOR_STATE_WAITING_CONTROL_OPEN_HEADER ||
         processor->state == PAPACC_CONTROL_PROCESSOR_STATE_READING_CONTROL_OPEN)
        ? PAPACC_TRUE : PAPACC_FALSE;
}
PAPACC_BOOL papacc_control_processor_wants_write(
    const PAPACC_CONTROL_PROCESSOR *processor)
{ return processor != NULL && processor->state ==
    PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT
    ? PAPACC_TRUE : PAPACC_FALSE; }
PAPACC_BOOL papacc_control_processor_is_established(
    const PAPACC_CONTROL_PROCESSOR *processor)
{ return processor != NULL && processor->state ==
    PAPACC_CONTROL_PROCESSOR_STATE_ESTABLISHED ? PAPACC_TRUE : PAPACC_FALSE; }

static PAPACC_RESULT papacc_control_processor_copy_payload(
    PAPACC_CONTROL_PROCESSOR *processor,
    const PAPACC_FRAME_PARSER_EVENT *event)
{
    if (event->payload_length >
        sizeof(processor->open_payload) - processor->open_payload_received)
        return PAPACC_RESULT_PROTOCOL_ERROR;
    if (event->payload_length > 0) {
        memcpy(&processor->open_payload[processor->open_payload_received],
               event->payload, event->payload_length);
        processor->open_payload_received += event->payload_length;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_control_processor_read_once(
    PAPACC_CONTROL_PROCESSOR *processor,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_READER_STATUS reader_status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_control_processor_wants_read(processor) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_next(
        &processor->reader, &reader_status, &event);
    if (result != PAPACC_RESULT_OK)
        return papacc_control_processor_fail(processor, result);
    if (reader_status == PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (reader_status == PAPACC_FRAMED_READER_STATUS_END_OF_STREAM) {
        papacc_control_processor_close_scope(processor);
        processor->state = PAPACC_CONTROL_PROCESSOR_STATE_CLOSED;
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (reader_status != PAPACC_FRAMED_READER_STATUS_EVENT) {
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    if (event.type == PAPACC_FRAME_PARSER_EVENT_HEADER_READY) {
        if (processor->state !=
                PAPACC_CONTROL_PROCESSOR_STATE_WAITING_CONTROL_OPEN_HEADER ||
            event.header.message_type != PAPACC_MESSAGE_TYPE_CONTROL_OPEN ||
            event.header.payload_length != 4)
            return papacc_control_processor_fail(
                processor, PAPACC_RESULT_PROTOCOL_ERROR);
        processor->open_payload_received = 0;
        processor->state =
            PAPACC_CONTROL_PROCESSOR_STATE_READING_CONTROL_OPEN;
    } else if (event.type == PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK ||
               event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
        if (processor->state !=
            PAPACC_CONTROL_PROCESSOR_STATE_READING_CONTROL_OPEN)
            return papacc_control_processor_fail(
                processor, PAPACC_RESULT_PROTOCOL_ERROR);
        result = papacc_control_processor_copy_payload(processor, &event);
        if (result != PAPACC_RESULT_OK)
            return papacc_control_processor_fail(processor, result);
        if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
            result = papacc_control_processor_commit(processor);
            if (result != PAPACC_RESULT_OK) return result;
        }
    } else {
        return papacc_control_processor_fail(
            processor, PAPACC_RESULT_INTERNAL_ERROR);
    }
    *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_control_processor_write_once(
    PAPACC_CONTROL_PROCESSOR *processor,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_WRITER_STATUS writer_status;
    PAPACC_SIZE consumed = 0;
    PAPACC_SESSION *session;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state !=
        PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_writer_step(
        &processor->writer,
        &processor->accept_payload[processor->accept_payload_offset],
        4 - processor->accept_payload_offset, &consumed, &writer_status);
    if (result != PAPACC_RESULT_OK)
        return papacc_control_processor_fail(processor, result);
    if (consumed > 4 - processor->accept_payload_offset)
        return papacc_control_processor_fail(
            processor, PAPACC_RESULT_INTERNAL_ERROR);
    processor->accept_payload_offset += consumed;
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM) {
        papacc_control_processor_close_scope(processor);
        processor->state = PAPACC_CONTROL_PROCESSOR_STATE_CLOSED;
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE) {
        if (processor->accept_payload_offset != 4)
            return papacc_control_processor_fail(
                processor, PAPACC_RESULT_INTERNAL_ERROR);
        session = papacc_session_manager_find(
            processor->session_manager, processor->session_instance_id);
        result = papacc_session_activate(session);
        if (result != PAPACC_RESULT_OK)
            return papacc_control_processor_fail(processor, result);
        processor->state = PAPACC_CONTROL_PROCESSOR_STATE_ESTABLISHED;
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_ESTABLISHED;
        return PAPACC_RESULT_OK;
    }
    *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_control_processor_check_deadline(
    PAPACC_CONTROL_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS *out_status)
{
    if (out_status != NULL)
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state == PAPACC_CONTROL_PROCESSOR_STATE_ESTABLISHED)
        return PAPACC_RESULT_OK;
    if (processor->state !=
            PAPACC_CONTROL_PROCESSOR_STATE_WAITING_CONTROL_OPEN_HEADER &&
        processor->state != PAPACC_CONTROL_PROCESSOR_STATE_READING_CONTROL_OPEN &&
        processor->state !=
            PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT)
        return PAPACC_RESULT_INVALID_STATE;
    if (now_ns < processor->establishment_deadline_ns) {
        *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    papacc_control_processor_close_scope(processor);
    processor->state = PAPACC_CONTROL_PROCESSOR_STATE_CLOSED;
    *out_status = PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED;
    return PAPACC_RESULT_OK;
}

void papacc_control_processor_shutdown(PAPACC_CONTROL_PROCESSOR *processor)
{
    if (processor == NULL ||
        processor->state == PAPACC_CONTROL_PROCESSOR_STATE_UNINITIALIZED)
        return;
    if (processor->state != PAPACC_CONTROL_PROCESSOR_STATE_ESTABLISHED &&
        processor->state != PAPACC_CONTROL_PROCESSOR_STATE_CLOSED &&
        processor->state != PAPACC_CONTROL_PROCESSOR_STATE_ERROR)
        papacc_control_processor_close_scope(processor);
    papacc_framed_reader_shutdown(&processor->reader);
    papacc_framed_writer_shutdown(&processor->writer);
    *processor = (PAPACC_CONTROL_PROCESSOR)
        PAPACC_CONTROL_PROCESSOR_INITIALIZER;
}
