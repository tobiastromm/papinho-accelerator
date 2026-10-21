#ifndef PAPACC_CAPABILITY_NEGOTIATION_PROTOCOL_H
#define PAPACC_CAPABILITY_NEGOTIATION_PROTOCOL_H

#include "frame.h"
#include "message_types.h"
#include <papacc/pacc/capability_wire.h>

#define PAPACC_CAPABILITY_WIRE_KEY_SIZE 6U
#define PAPACC_CAPABILITY_NEGOTIATION_HEADER_SIZE 4U

PAPACC_RESULT papacc_capability_negotiate_encode(
    const PAPACC_CAPABILITY_SET *, const PAPACC_CAPABILITY_SET *,
    PAPACC_U8 *, PAPACC_SIZE, PAPACC_SIZE *);
PAPACC_RESULT papacc_capability_negotiate_decode(
    const PAPACC_U8 *, PAPACC_SIZE, PAPACC_CAPABILITY_SET *,
    PAPACC_CAPABILITY_SET *);
PAPACC_RESULT papacc_capability_negotiate_result_encode(
    const PAPACC_CAPABILITY_SET *, PAPACC_U8 *, PAPACC_SIZE, PAPACC_SIZE *);
PAPACC_RESULT papacc_capability_negotiate_result_decode(
    const PAPACC_U8 *, PAPACC_SIZE, PAPACC_CAPABILITY_SET *);
PAPACC_FRAME_HEADER papacc_capability_negotiate_frame_header(PAPACC_U32);
PAPACC_FRAME_HEADER papacc_capability_negotiate_result_frame_header(PAPACC_U32);

#endif
