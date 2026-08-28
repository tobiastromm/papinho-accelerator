#ifndef PAPACC_FRAME_H
#define PAPACC_FRAME_H

#include <papacc/types.h>

#define PAPACC_FRAME_MAGIC_SIZE 4U
#define PAPACC_FRAME_BASE_HEADER_SIZE 16U
#define PAPACC_FRAME_ENVELOPE_MAJOR 1U
#define PAPACC_FRAME_ENVELOPE_MINOR 0U

typedef struct PAPACC_FRAME_HEADER {
    PAPACC_U8 envelope_major;
    PAPACC_U8 envelope_minor;
    PAPACC_U16 header_length;
    PAPACC_U16 message_type;
    PAPACC_U16 flags;
    PAPACC_U32 payload_length;
} PAPACC_FRAME_HEADER;

#define PAPACC_FRAME_HEADER_INITIALIZER { 0, 0, 0, 0, 0, 0 }

PAPACC_RESULT papacc_frame_header_validate(
    const PAPACC_FRAME_HEADER *header,
    PAPACC_U32 max_payload_length);

/* Encoder applies no receive-side payload policy and writes atomically. */
PAPACC_RESULT papacc_frame_header_encode(
    const PAPACC_FRAME_HEADER *header,
    PAPACC_U8 *output,
    PAPACC_SIZE output_capacity,
    PAPACC_SIZE *out_written);

/* Complete-header helper; input shorter than 16 returns LIMIT_EXCEEDED. */
PAPACC_RESULT papacc_frame_header_decode(
    const PAPACC_U8 *input,
    PAPACC_SIZE input_length,
    PAPACC_U32 max_payload_length,
    PAPACC_FRAME_HEADER *out_header);

#endif
