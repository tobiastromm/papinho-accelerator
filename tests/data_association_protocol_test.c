#include "data_association_protocol.h"

#include <string.h>

static int papacc_test_frame(
    PAPACC_FRAME_HEADER header, const PAPACC_U8 *payload,
    PAPACC_SIZE payload_length, const PAPACC_U8 *expected,
    PAPACC_SIZE expected_length)
{
    PAPACC_U8 encoded[32];
    PAPACC_SIZE written = 0;
    if (papacc_frame_header_encode(
            &header, encoded, sizeof(encoded), &written) != PAPACC_RESULT_OK ||
        written != 16) return 1;
    if (payload_length > 0) memcpy(&encoded[written], payload, payload_length);
    return written + payload_length == expected_length &&
        memcmp(encoded, expected, expected_length) == 0 ? 0 : 2;
}

int main(void)
{
    static const PAPACC_U8 request[16] = {
        0x50,0x41,0x43,0x43,1,0,0,0x10,0,3,0,0,0,0,0,0 };
    static const PAPACC_U8 accept[16] = {
        0x50,0x41,0x43,0x43,1,0,0,0x10,0,6,0,0,0,0,0,0 };
    static const PAPACC_U8 ticket_frame[32] = {
        0x50,0x41,0x43,0x43,1,0,0,0x10,0,4,0,0,0,0,0,0x10,
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    static const PAPACC_U8 attach_frame[32] = {
        0x50,0x41,0x43,0x43,1,0,0,0x10,0,5,0,0,0,0,0,0x10,
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    PAPACC_DATA_ASSOCIATION_TICKET zero =
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_TICKET ticket =
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_TICKET decoded =
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_U8 payload[16];
    PAPACC_SIZE written = 99;
    PAPACC_SIZE index;

    if (PAPACC_MESSAGE_TYPE_CONTROL_OPEN != 1 ||
        PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT != 2 ||
        PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST != 3 ||
        PAPACC_MESSAGE_TYPE_DATA_TICKET != 4 ||
        PAPACC_MESSAGE_TYPE_DATA_ATTACH != 5 ||
        PAPACC_MESSAGE_TYPE_DATA_ACCEPT != 6 || sizeof(ticket.bytes) != 16)
        return 1;
    for (index = 0; index < 16; ++index) ticket.bytes[index] = (PAPACC_U8)index;
    if (papacc_data_association_ticket_is_zero(&zero) != PAPACC_TRUE ||
        papacc_data_association_ticket_is_valid(&zero) != PAPACC_FALSE ||
        papacc_data_association_ticket_is_valid(&ticket) != PAPACC_TRUE ||
        papacc_data_association_ticket_equal(&ticket, &ticket) != PAPACC_TRUE ||
        papacc_data_association_ticket_equal(NULL, &ticket) != PAPACC_FALSE ||
        papacc_data_association_ticket_is_zero(NULL) != PAPACC_FALSE)
        return 2;
    decoded = ticket;
    decoded.bytes[15] ^= 1;
    if (papacc_data_association_ticket_equal(&decoded, &ticket) != PAPACC_FALSE)
        return 3;
    if (papacc_data_ticket_encode(
            &ticket, payload, sizeof(payload), &written) != PAPACC_RESULT_OK ||
        written != 16 || memcmp(payload, ticket.bytes, 16) != 0 ||
        papacc_data_ticket_decode(payload, 16, &decoded) != PAPACC_RESULT_OK ||
        papacc_data_association_ticket_equal(&decoded, &ticket) != PAPACC_TRUE ||
        papacc_data_attach_encode(
            &ticket, payload, sizeof(payload), &written) != PAPACC_RESULT_OK ||
        papacc_data_attach_decode(payload, 16, &decoded) != PAPACC_RESULT_OK)
        return 4;
    if (papacc_data_ticket_decode(payload, 15, &decoded) !=
            PAPACC_RESULT_PROTOCOL_ERROR ||
        papacc_data_ticket_decode(payload, 17, &decoded) !=
            PAPACC_RESULT_PROTOCOL_ERROR ||
        papacc_data_attach_decode(zero.bytes, 16, &decoded) !=
            PAPACC_RESULT_PROTOCOL_ERROR ||
        papacc_data_ticket_encode(
            &zero, payload, sizeof(payload), &written) !=
            PAPACC_RESULT_INVALID_ARGUMENT || written != 0)
        return 5;
    if (papacc_test_frame(papacc_data_ticket_request_frame_header(), NULL, 0,
                          request, sizeof(request)) != 0 ||
        papacc_test_frame(papacc_data_ticket_frame_header(), ticket.bytes, 16,
                          ticket_frame, sizeof(ticket_frame)) != 0 ||
        papacc_test_frame(papacc_data_attach_frame_header(), ticket.bytes, 16,
                          attach_frame, sizeof(attach_frame)) != 0 ||
        papacc_test_frame(papacc_data_accept_frame_header(), NULL, 0,
                          accept, sizeof(accept)) != 0)
        return 6;
    return 0;
}
