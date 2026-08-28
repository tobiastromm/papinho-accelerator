#include "data_attach_processor.h"

#include <string.h>

static void papacc_data_attach_close_scope(
    PAPACC_DATA_ATTACH_PROCESSOR *processor)
{
    if (processor->data_channel_instance_id != 0)
        (void)papacc_channel_manager_close(processor->channel_manager,
            processor->data_channel_instance_id);
    else if (processor->connection != NULL)
        papacc_connection_close(processor->connection);
}

static PAPACC_RESULT papacc_data_attach_fail(
    PAPACC_DATA_ATTACH_PROCESSOR *processor, PAPACC_RESULT result)
{
    papacc_data_attach_close_scope(processor);
    processor->state = PAPACC_DATA_ATTACH_PROCESSOR_STATE_ERROR;
    return result;
}

static void papacc_data_attach_close(PAPACC_DATA_ATTACH_PROCESSOR *processor)
{
    papacc_data_attach_close_scope(processor);
    processor->state = PAPACC_DATA_ATTACH_PROCESSOR_STATE_CLOSED;
}

PAPACC_RESULT papacc_data_attach_processor_init_from_reader(
    PAPACC_DATA_ATTACH_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 connection_instance_id,
    const PAPACC_FRAME_HEADER *first_header,
    PAPACC_FRAMED_READER *reader,
    PAPACC_U64 establishment_deadline_ns)
{
    PAPACC_CONNECTION *connection;
    PAPACC_RESULT result;
    if (processor == NULL || connection_manager == NULL ||
        session_manager == NULL || channel_manager == NULL ||
        association_manager == NULL || first_header == NULL || reader == NULL ||
        connection_instance_id == 0)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_DATA_ATTACH_PROCESSOR_STATE_UNINITIALIZED ||
        reader->state != PAPACC_FRAMED_READER_STATE_READY ||
        channel_manager->initialized != PAPACC_TRUE ||
        channel_manager->connection_manager != connection_manager ||
        channel_manager->session_manager != session_manager ||
        association_manager->initialized != PAPACC_TRUE ||
        association_manager->session_manager != session_manager ||
        association_manager->channel_manager != channel_manager)
        return PAPACC_RESULT_INVALID_STATE;
    connection = papacc_connection_manager_find(
        connection_manager, connection_instance_id);
    if (connection == NULL || connection->state != PAPACC_CONNECTION_STATE_PENDING ||
        papacc_channel_manager_find_by_connection(channel_manager,
            connection_instance_id) != NULL ||
        papacc_framed_reader_transport(reader) != &connection->transport ||
        first_header->message_type != PAPACC_MESSAGE_TYPE_DATA_ATTACH ||
        first_header->payload_length != PAPACC_DATA_ASSOCIATION_TICKET_SIZE ||
        reader->parser.state != PAPACC_FRAME_PARSER_STATE_READING_PAYLOAD ||
        reader->parser.header.message_type != first_header->message_type ||
        reader->parser.header.payload_length != first_header->payload_length)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_writer_init(&processor->writer,
        &connection->transport);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_framed_reader_move(&processor->reader, reader);
    if (result != PAPACC_RESULT_OK) {
        papacc_framed_writer_shutdown(&processor->writer);
        return result;
    }
    processor->connection_manager = connection_manager;
    processor->session_manager = session_manager;
    processor->channel_manager = channel_manager;
    processor->association_manager = association_manager;
    processor->connection = connection;
    processor->connection_instance_id = connection_instance_id;
    processor->establishment_deadline_ns = establishment_deadline_ns;
    processor->state = PAPACC_DATA_ATTACH_PROCESSOR_STATE_READING_DATA_ATTACH;
    return PAPACC_RESULT_OK;
}

PAPACC_BOOL papacc_data_attach_processor_wants_read(
    const PAPACC_DATA_ATTACH_PROCESSOR *processor)
{
    return processor != NULL && processor->state ==
        PAPACC_DATA_ATTACH_PROCESSOR_STATE_READING_DATA_ATTACH
        ? PAPACC_TRUE : PAPACC_FALSE;
}
PAPACC_BOOL papacc_data_attach_processor_wants_write(
    const PAPACC_DATA_ATTACH_PROCESSOR *processor)
{
    return processor != NULL && processor->state ==
        PAPACC_DATA_ATTACH_PROCESSOR_STATE_WRITING_DATA_ACCEPT
        ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_RESULT papacc_data_attach_commit(
    PAPACC_DATA_ATTACH_PROCESSOR *processor, PAPACC_U64 now_ns)
{
    PAPACC_CHANNEL *channel = NULL;
    PAPACC_FRAME_HEADER accept_header;
    PAPACC_RESULT result = papacc_data_attach_decode(
        processor->attach_payload, sizeof(processor->attach_payload),
        &processor->ticket);
    if (result != PAPACC_RESULT_OK)
        return papacc_data_attach_fail(processor, result);
    result = papacc_data_association_manager_consume(
        processor->association_manager, &processor->ticket, now_ns,
        &processor->session_instance_id);
    if (result != PAPACC_RESULT_OK)
        return papacc_data_attach_fail(processor, result);
    result = papacc_channel_manager_bind(processor->channel_manager,
        processor->session_instance_id, processor->connection_instance_id,
        PAPACC_CHANNEL_ROLE_DATA, &channel);
    if (result != PAPACC_RESULT_OK)
        return papacc_data_attach_fail(processor, result);
    processor->data_channel_instance_id = channel->channel_instance_id;
    accept_header = papacc_data_accept_frame_header();
    result = papacc_framed_writer_begin_frame(&processor->writer, &accept_header);
    if (result != PAPACC_RESULT_OK)
        return papacc_data_attach_fail(processor, result);
    processor->state = PAPACC_DATA_ATTACH_PROCESSOR_STATE_WRITING_DATA_ACCEPT;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_data_attach_processor_read_once(
    PAPACC_DATA_ATTACH_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_READER_STATUS reader_status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_data_attach_processor_wants_read(processor) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_next(&processor->reader,
        &reader_status, &event);
    if (result != PAPACC_RESULT_OK)
        return papacc_data_attach_fail(processor, result);
    if (reader_status == PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (reader_status == PAPACC_FRAMED_READER_STATUS_END_OF_STREAM) {
        papacc_data_attach_close(processor);
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (reader_status != PAPACC_FRAMED_READER_STATUS_EVENT) {
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    if (event.type != PAPACC_FRAME_PARSER_EVENT_PAYLOAD_CHUNK &&
        event.type != PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE)
        return papacc_data_attach_fail(processor, PAPACC_RESULT_INTERNAL_ERROR);
    if (event.payload_length > sizeof(processor->attach_payload) -
        processor->attach_payload_received)
        return papacc_data_attach_fail(processor, PAPACC_RESULT_PROTOCOL_ERROR);
    if (event.payload_length != 0) {
        memcpy(&processor->attach_payload[processor->attach_payload_received],
            event.payload, event.payload_length);
        processor->attach_payload_received += event.payload_length;
    }
    if (event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE) {
        if (processor->attach_payload_received !=
            PAPACC_DATA_ASSOCIATION_TICKET_SIZE)
            return papacc_data_attach_fail(processor,
                PAPACC_RESULT_PROTOCOL_ERROR);
        result = papacc_data_attach_commit(processor, now_ns);
        if (result != PAPACC_RESULT_OK) return result;
    }
    *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_data_attach_processor_write_once(
    PAPACC_DATA_ATTACH_PROCESSOR *processor,
    PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_WRITER_STATUS writer_status;
    PAPACC_SIZE consumed = 0;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_data_attach_processor_wants_write(processor) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_writer_step(
        &processor->writer, NULL, 0, &consumed, &writer_status);
    if (result != PAPACC_RESULT_OK)
        return papacc_data_attach_fail(processor, result);
    if (consumed != 0)
        return papacc_data_attach_fail(processor,
            PAPACC_RESULT_INTERNAL_ERROR);
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM) {
        papacc_data_attach_close(processor);
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE) {
        processor->state = PAPACC_DATA_ATTACH_PROCESSOR_STATE_ESTABLISHED;
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_ESTABLISHED;
        return PAPACC_RESULT_OK;
    }
    *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_data_attach_processor_check_deadline(
    PAPACC_DATA_ATTACH_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS *out_status)
{
    if (out_status != NULL)
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state == PAPACC_DATA_ATTACH_PROCESSOR_STATE_ESTABLISHED)
        return PAPACC_RESULT_OK;
    if (processor->state != PAPACC_DATA_ATTACH_PROCESSOR_STATE_READING_DATA_ATTACH &&
        processor->state != PAPACC_DATA_ATTACH_PROCESSOR_STATE_WRITING_DATA_ACCEPT)
        return PAPACC_RESULT_INVALID_STATE;
    if (now_ns < processor->establishment_deadline_ns) {
        *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    papacc_data_attach_close(processor);
    *out_status = PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_CLOSED;
    return PAPACC_RESULT_OK;
}

void papacc_data_attach_processor_shutdown(
    PAPACC_DATA_ATTACH_PROCESSOR *processor)
{
    if (processor == NULL || processor->state ==
        PAPACC_DATA_ATTACH_PROCESSOR_STATE_UNINITIALIZED) return;
    papacc_data_attach_close_scope(processor);
    papacc_framed_reader_shutdown(&processor->reader);
    papacc_framed_writer_shutdown(&processor->writer);
    *processor = (PAPACC_DATA_ATTACH_PROCESSOR)
        PAPACC_DATA_ATTACH_PROCESSOR_INITIALIZER;
}
