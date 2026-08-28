#ifndef PAPACC_CONTROL_PROTOCOL_H
#define PAPACC_CONTROL_PROTOCOL_H

#include "frame.h"

#define PAPACC_MESSAGE_TYPE_CONTROL_OPEN ((PAPACC_U16)0x0001U)
#define PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT ((PAPACC_U16)0x0002U)
#define PAPACC_CONTROL_PROTOCOL_MAJOR ((PAPACC_U16)1U)
#define PAPACC_CONTROL_PROTOCOL_MINOR ((PAPACC_U16)0U)
#define PAPACC_CONTROL_VERSION_PAYLOAD_SIZE 4U

typedef struct PAPACC_CONTROL_PROTOCOL_VERSION {
    PAPACC_U16 major;
    PAPACC_U16 minor;
} PAPACC_CONTROL_PROTOCOL_VERSION;

#define PAPACC_CONTROL_PROTOCOL_VERSION_INITIALIZER { 0, 0 }

PAPACC_RESULT papacc_control_open_encode(
    const PAPACC_CONTROL_PROTOCOL_VERSION *version, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written);
PAPACC_RESULT papacc_control_open_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_CONTROL_PROTOCOL_VERSION *out_version);
PAPACC_RESULT papacc_control_accept_encode(
    const PAPACC_CONTROL_PROTOCOL_VERSION *version, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written);
PAPACC_RESULT papacc_control_accept_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_CONTROL_PROTOCOL_VERSION *out_version);
PAPACC_FRAME_HEADER papacc_control_open_frame_header(void);
PAPACC_FRAME_HEADER papacc_control_accept_frame_header(void);

#endif
