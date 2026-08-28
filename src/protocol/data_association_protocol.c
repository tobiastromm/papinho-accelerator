#include "data_association_protocol.h"

#include <string.h>

PAPACC_BOOL papacc_data_association_ticket_is_zero(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket)
{
    PAPACC_SIZE index;
    if (ticket == NULL) return PAPACC_FALSE;
    for (index = 0; index < PAPACC_DATA_ASSOCIATION_TICKET_SIZE; ++index)
        if (ticket->bytes[index] != 0) return PAPACC_FALSE;
    return PAPACC_TRUE;
}

PAPACC_BOOL papacc_data_association_ticket_is_valid(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket)
{
    return ticket != NULL &&
        papacc_data_association_ticket_is_zero(ticket) == PAPACC_FALSE
        ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_BOOL papacc_data_association_ticket_equal(
    const PAPACC_DATA_ASSOCIATION_TICKET *left,
    const PAPACC_DATA_ASSOCIATION_TICKET *right)
{
    if (left == NULL || right == NULL) return PAPACC_FALSE;
    return memcmp(left->bytes, right->bytes,
                  PAPACC_DATA_ASSOCIATION_TICKET_SIZE) == 0
        ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_FRAME_HEADER papacc_data_frame_header(
    PAPACC_U16 type, PAPACC_U32 payload_length)
{
    PAPACC_FRAME_HEADER header = {
        PAPACC_FRAME_ENVELOPE_MAJOR, PAPACC_FRAME_ENVELOPE_MINOR,
        PAPACC_FRAME_BASE_HEADER_SIZE, type, 0, payload_length
    };
    return header;
}

PAPACC_FRAME_HEADER papacc_data_ticket_request_frame_header(void)
{ return papacc_data_frame_header(PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0); }
PAPACC_FRAME_HEADER papacc_data_ticket_frame_header(void)
{ return papacc_data_frame_header(PAPACC_MESSAGE_TYPE_DATA_TICKET,
    PAPACC_DATA_ASSOCIATION_TICKET_SIZE); }
PAPACC_FRAME_HEADER papacc_data_attach_frame_header(void)
{ return papacc_data_frame_header(PAPACC_MESSAGE_TYPE_DATA_ATTACH,
    PAPACC_DATA_ASSOCIATION_TICKET_SIZE); }
PAPACC_FRAME_HEADER papacc_data_accept_frame_header(void)
{ return papacc_data_frame_header(PAPACC_MESSAGE_TYPE_DATA_ACCEPT, 0); }

static PAPACC_RESULT papacc_data_ticket_payload_encode(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written)
{
    if (out_written != NULL) *out_written = 0;
    if (ticket == NULL || output == NULL || out_written == NULL ||
        papacc_data_association_ticket_is_valid(ticket) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (capacity < PAPACC_DATA_ASSOCIATION_TICKET_SIZE)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    memcpy(output, ticket->bytes, PAPACC_DATA_ASSOCIATION_TICKET_SIZE);
    *out_written = PAPACC_DATA_ASSOCIATION_TICKET_SIZE;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_data_ticket_payload_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket)
{
    PAPACC_DATA_ASSOCIATION_TICKET ticket =
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    if (out_ticket != NULL) *out_ticket = ticket;
    if (input == NULL || out_ticket == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (length != PAPACC_DATA_ASSOCIATION_TICKET_SIZE)
        return PAPACC_RESULT_PROTOCOL_ERROR;
    memcpy(ticket.bytes, input, PAPACC_DATA_ASSOCIATION_TICKET_SIZE);
    if (papacc_data_association_ticket_is_valid(&ticket) != PAPACC_TRUE)
        return PAPACC_RESULT_PROTOCOL_ERROR;
    *out_ticket = ticket;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_data_ticket_encode(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written)
{ return papacc_data_ticket_payload_encode(ticket, output, capacity, out_written); }
PAPACC_RESULT papacc_data_ticket_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket)
{ return papacc_data_ticket_payload_decode(input, length, out_ticket); }
PAPACC_RESULT papacc_data_attach_encode(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written)
{ return papacc_data_ticket_payload_encode(ticket, output, capacity, out_written); }
PAPACC_RESULT papacc_data_attach_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket)
{ return papacc_data_ticket_payload_decode(input, length, out_ticket); }
