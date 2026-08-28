#include "connection_classifier.h"

static PAPACC_RESULT papacc_classifier_fail(
    PAPACC_CONNECTION_CLASSIFIER *classifier, PAPACC_RESULT result)
{
    papacc_connection_close(classifier->connection);
    classifier->state = PAPACC_CONNECTION_CLASSIFIER_STATE_ERROR;
    return result;
}

PAPACC_RESULT papacc_connection_classifier_init(
    PAPACC_CONNECTION_CLASSIFIER *classifier,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_U64 connection_instance_id, PAPACC_U8 *read_scratch,
    PAPACC_SIZE read_scratch_capacity, PAPACC_U32 max_payload_length,
    PAPACC_U64 establishment_deadline_ns)
{
    PAPACC_CONNECTION *connection;
    PAPACC_RESULT result;
    if (classifier == NULL || connection_manager == NULL ||
        connection_instance_id == 0 || read_scratch == NULL ||
        read_scratch_capacity == 0)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (classifier->state != PAPACC_CONNECTION_CLASSIFIER_STATE_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    connection = papacc_connection_manager_find(
        connection_manager, connection_instance_id);
    if (connection == NULL || connection->state != PAPACC_CONNECTION_STATE_PENDING ||
        papacc_transport_connection_is_valid(&connection->transport) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_init(&classifier->reader,
        &connection->transport, read_scratch, read_scratch_capacity,
        max_payload_length);
    if (result != PAPACC_RESULT_OK) return result;
    classifier->connection_manager = connection_manager;
    classifier->connection = connection;
    classifier->establishment_deadline_ns = establishment_deadline_ns;
    classifier->state = PAPACC_CONNECTION_CLASSIFIER_STATE_WAITING_FIRST_HEADER;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_connection_classifier_read_once(
    PAPACC_CONNECTION_CLASSIFIER *classifier,
    PAPACC_CONNECTION_CLASSIFIER_STATUS *out_status)
{
    PAPACC_FRAMED_READER_STATUS reader_status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_UNSPECIFIED;
    if (classifier == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (classifier->state !=
        PAPACC_CONNECTION_CLASSIFIER_STATE_WAITING_FIRST_HEADER)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_next(
        &classifier->reader, &reader_status, &event);
    if (result != PAPACC_RESULT_OK)
        return papacc_classifier_fail(classifier, result);
    if (reader_status == PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (reader_status == PAPACC_FRAMED_READER_STATUS_END_OF_STREAM) {
        papacc_connection_close(classifier->connection);
        classifier->state = PAPACC_CONNECTION_CLASSIFIER_STATE_CLOSED;
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (reader_status != PAPACC_FRAMED_READER_STATUS_EVENT) {
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    if (event.type != PAPACC_FRAME_PARSER_EVENT_HEADER_READY)
        return papacc_classifier_fail(classifier,
            event.type == PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE
                ? PAPACC_RESULT_PROTOCOL_ERROR : PAPACC_RESULT_INTERNAL_ERROR);
    if (event.header.message_type == PAPACC_MESSAGE_TYPE_CONTROL_OPEN &&
        event.header.payload_length == 4) {
        classifier->first_header = event.header;
        classifier->state =
            PAPACC_CONNECTION_CLASSIFIER_STATE_CLASSIFIED_CONTROL;
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_CLASSIFIED_CONTROL;
        return PAPACC_RESULT_OK;
    }
    if (event.header.message_type == PAPACC_MESSAGE_TYPE_DATA_ATTACH &&
        event.header.payload_length == PAPACC_DATA_ASSOCIATION_TICKET_SIZE) {
        classifier->first_header = event.header;
        classifier->state = PAPACC_CONNECTION_CLASSIFIER_STATE_CLASSIFIED_DATA;
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_CLASSIFIED_DATA;
        return PAPACC_RESULT_OK;
    }
    return papacc_classifier_fail(classifier, PAPACC_RESULT_PROTOCOL_ERROR);
}

PAPACC_RESULT papacc_connection_classifier_check_deadline(
    PAPACC_CONNECTION_CLASSIFIER *classifier, PAPACC_U64 now_ns,
    PAPACC_CONNECTION_CLASSIFIER_STATUS *out_status)
{
    if (out_status != NULL)
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_UNSPECIFIED;
    if (classifier == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (classifier->state !=
            PAPACC_CONNECTION_CLASSIFIER_STATE_WAITING_FIRST_HEADER &&
        classifier->state !=
            PAPACC_CONNECTION_CLASSIFIER_STATE_CLASSIFIED_CONTROL &&
        classifier->state != PAPACC_CONNECTION_CLASSIFIER_STATE_CLASSIFIED_DATA)
        return PAPACC_RESULT_INVALID_STATE;
    if (now_ns < classifier->establishment_deadline_ns) {
        *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    papacc_connection_close(classifier->connection);
    classifier->state = PAPACC_CONNECTION_CLASSIFIER_STATE_CLOSED;
    *out_status = PAPACC_CONNECTION_CLASSIFIER_STATUS_CLOSED;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_connection_classifier_take(
    PAPACC_CONNECTION_CLASSIFIER *classifier,
    PAPACC_CONNECTION_CLASSIFICATION *out_classification,
    PAPACC_FRAME_HEADER *out_first_header, PAPACC_FRAMED_READER *out_reader,
    PAPACC_U64 *out_establishment_deadline_ns)
{
    PAPACC_CONNECTION_CLASSIFICATION classification =
        PAPACC_CONNECTION_CLASSIFICATION_UNSPECIFIED;
    PAPACC_RESULT result;
    if (out_classification != NULL) *out_classification = classification;
    if (out_first_header != NULL)
        *out_first_header = (PAPACC_FRAME_HEADER)PAPACC_FRAME_HEADER_INITIALIZER;
    if (out_establishment_deadline_ns != NULL)
        *out_establishment_deadline_ns = 0;
    if (classifier == NULL || out_classification == NULL ||
        out_first_header == NULL || out_reader == NULL ||
        out_establishment_deadline_ns == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (classifier->state ==
        PAPACC_CONNECTION_CLASSIFIER_STATE_CLASSIFIED_CONTROL)
        classification = PAPACC_CONNECTION_CLASSIFICATION_CONTROL;
    else if (classifier->state ==
        PAPACC_CONNECTION_CLASSIFIER_STATE_CLASSIFIED_DATA)
        classification = PAPACC_CONNECTION_CLASSIFICATION_DATA;
    else return PAPACC_RESULT_INVALID_STATE;
    if (out_reader->state != PAPACC_FRAMED_READER_STATE_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_move(out_reader, &classifier->reader);
    if (result != PAPACC_RESULT_OK) return result;
    *out_classification = classification;
    *out_first_header = classifier->first_header;
    *out_establishment_deadline_ns = classifier->establishment_deadline_ns;
    *classifier = (PAPACC_CONNECTION_CLASSIFIER)
        PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
    return PAPACC_RESULT_OK;
}

void papacc_connection_classifier_shutdown(
    PAPACC_CONNECTION_CLASSIFIER *classifier)
{
    if (classifier == NULL || classifier->state ==
        PAPACC_CONNECTION_CLASSIFIER_STATE_UNINITIALIZED) return;
    papacc_connection_close(classifier->connection);
    papacc_framed_reader_shutdown(&classifier->reader);
    *classifier = (PAPACC_CONNECTION_CLASSIFIER)
        PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
}
