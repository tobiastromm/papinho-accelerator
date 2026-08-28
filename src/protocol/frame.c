#include "frame.h"

#include <string.h>

static PAPACC_U16 papacc_frame_read_u16_be(const PAPACC_U8 *input)
{
    return (PAPACC_U16)(((PAPACC_U16)input[0] << 8) |
                        (PAPACC_U16)input[1]);
}

static PAPACC_U32 papacc_frame_read_u32_be(const PAPACC_U8 *input)
{
    return ((PAPACC_U32)input[0] << 24) |
           ((PAPACC_U32)input[1] << 16) |
           ((PAPACC_U32)input[2] << 8) |
           (PAPACC_U32)input[3];
}

static void papacc_frame_write_u16_be(PAPACC_U8 *output, PAPACC_U16 value)
{
    output[0] = (PAPACC_U8)(value >> 8);
    output[1] = (PAPACC_U8)value;
}

static void papacc_frame_write_u32_be(PAPACC_U8 *output, PAPACC_U32 value)
{
    output[0] = (PAPACC_U8)(value >> 24);
    output[1] = (PAPACC_U8)(value >> 16);
    output[2] = (PAPACC_U8)(value >> 8);
    output[3] = (PAPACC_U8)value;
}

PAPACC_RESULT papacc_frame_header_validate(
    const PAPACC_FRAME_HEADER *header,
    PAPACC_U32 max_payload_length)
{
    if (header == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (header->envelope_major != PAPACC_FRAME_ENVELOPE_MAJOR ||
        header->envelope_minor != PAPACC_FRAME_ENVELOPE_MINOR) {
        return PAPACC_RESULT_NOT_SUPPORTED;
    }
    if (header->header_length != PAPACC_FRAME_BASE_HEADER_SIZE ||
        header->message_type == 0 || header->flags != 0) {
        return PAPACC_RESULT_PROTOCOL_ERROR;
    }
    if (header->payload_length > max_payload_length) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_frame_header_encode(
    const PAPACC_FRAME_HEADER *header,
    PAPACC_U8 *output,
    PAPACC_SIZE output_capacity,
    PAPACC_SIZE *out_written)
{
    PAPACC_U8 encoded[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_RESULT result;

    if (out_written != NULL) {
        *out_written = 0;
    }
    if (header == NULL || output == NULL || out_written == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    result = papacc_frame_header_validate(header, UINT32_MAX);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (output_capacity < PAPACC_FRAME_BASE_HEADER_SIZE) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    encoded[0] = 0x50;
    encoded[1] = 0x41;
    encoded[2] = 0x43;
    encoded[3] = 0x43;
    encoded[4] = header->envelope_major;
    encoded[5] = header->envelope_minor;
    papacc_frame_write_u16_be(&encoded[6], header->header_length);
    papacc_frame_write_u16_be(&encoded[8], header->message_type);
    papacc_frame_write_u16_be(&encoded[10], header->flags);
    papacc_frame_write_u32_be(&encoded[12], header->payload_length);
    memcpy(output, encoded, PAPACC_FRAME_BASE_HEADER_SIZE);
    *out_written = PAPACC_FRAME_BASE_HEADER_SIZE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_frame_header_decode(
    const PAPACC_U8 *input,
    PAPACC_SIZE input_length,
    PAPACC_U32 max_payload_length,
    PAPACC_FRAME_HEADER *out_header)
{
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_RESULT result;

    if (out_header != NULL) {
        *out_header = header;
    }
    if (input == NULL || out_header == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (input_length < PAPACC_FRAME_BASE_HEADER_SIZE) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    if (input[0] != 0x50 || input[1] != 0x41 ||
        input[2] != 0x43 || input[3] != 0x43) {
        return PAPACC_RESULT_PROTOCOL_ERROR;
    }
    header.envelope_major = input[4];
    header.envelope_minor = input[5];
    header.header_length = papacc_frame_read_u16_be(&input[6]);
    header.message_type = papacc_frame_read_u16_be(&input[8]);
    header.flags = papacc_frame_read_u16_be(&input[10]);
    header.payload_length = papacc_frame_read_u32_be(&input[12]);
    result = papacc_frame_header_validate(&header, max_payload_length);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    *out_header = header;
    return PAPACC_RESULT_OK;
}
