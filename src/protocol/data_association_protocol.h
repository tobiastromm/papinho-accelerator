#ifndef PAPACC_DATA_ASSOCIATION_PROTOCOL_H
#define PAPACC_DATA_ASSOCIATION_PROTOCOL_H

#include "frame.h"
#include "message_types.h"

#define PAPACC_DATA_ASSOCIATION_TICKET_SIZE 16U

typedef struct PAPACC_DATA_ASSOCIATION_TICKET {
    PAPACC_U8 bytes[PAPACC_DATA_ASSOCIATION_TICKET_SIZE];
} PAPACC_DATA_ASSOCIATION_TICKET;

#define PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER { { 0 } }

PAPACC_BOOL papacc_data_association_ticket_is_zero(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket);
PAPACC_BOOL papacc_data_association_ticket_is_valid(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket);
PAPACC_BOOL papacc_data_association_ticket_equal(
    const PAPACC_DATA_ASSOCIATION_TICKET *left,
    const PAPACC_DATA_ASSOCIATION_TICKET *right);

PAPACC_FRAME_HEADER papacc_data_ticket_request_frame_header(void);
PAPACC_FRAME_HEADER papacc_data_ticket_frame_header(void);
PAPACC_FRAME_HEADER papacc_data_attach_frame_header(void);
PAPACC_FRAME_HEADER papacc_data_accept_frame_header(void);

PAPACC_RESULT papacc_data_ticket_encode(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written);
PAPACC_RESULT papacc_data_ticket_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket);
PAPACC_RESULT papacc_data_attach_encode(
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written);
PAPACC_RESULT papacc_data_attach_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket);

#endif
