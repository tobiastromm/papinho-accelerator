#include "control_protocol.h"

static PAPACC_RESULT papacc_control_version_validate(
    const PAPACC_CONTROL_PROTOCOL_VERSION *version)
{
    if (version == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    return (version->major == PAPACC_CONTROL_PROTOCOL_MAJOR &&
            version->minor == PAPACC_CONTROL_PROTOCOL_MINOR)
        ? PAPACC_RESULT_OK : PAPACC_RESULT_NOT_SUPPORTED;
}

static PAPACC_RESULT papacc_control_version_encode(
    const PAPACC_CONTROL_PROTOCOL_VERSION *version, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written)
{
    PAPACC_RESULT result;
    if (out_written != NULL) *out_written = 0;
    if (version == NULL || output == NULL || out_written == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    result = papacc_control_version_validate(version);
    if (result != PAPACC_RESULT_OK) return result;
    if (capacity < PAPACC_CONTROL_VERSION_PAYLOAD_SIZE)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    output[0] = (PAPACC_U8)(version->major >> 8);
    output[1] = (PAPACC_U8)version->major;
    output[2] = (PAPACC_U8)(version->minor >> 8);
    output[3] = (PAPACC_U8)version->minor;
    *out_written = PAPACC_CONTROL_VERSION_PAYLOAD_SIZE;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_control_version_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_CONTROL_PROTOCOL_VERSION *out_version)
{
    PAPACC_CONTROL_PROTOCOL_VERSION version =
        PAPACC_CONTROL_PROTOCOL_VERSION_INITIALIZER;
    PAPACC_RESULT result;
    if (out_version != NULL) *out_version = version;
    if (input == NULL || out_version == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (length != PAPACC_CONTROL_VERSION_PAYLOAD_SIZE)
        return PAPACC_RESULT_PROTOCOL_ERROR;
    version.major = (PAPACC_U16)(((PAPACC_U16)input[0] << 8) | input[1]);
    version.minor = (PAPACC_U16)(((PAPACC_U16)input[2] << 8) | input[3]);
    result = papacc_control_version_validate(&version);
    if (result != PAPACC_RESULT_OK) return result;
    *out_version = version;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_control_open_encode(
    const PAPACC_CONTROL_PROTOCOL_VERSION *version, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written)
{ return papacc_control_version_encode(version, output, capacity, out_written); }
PAPACC_RESULT papacc_control_open_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_CONTROL_PROTOCOL_VERSION *out_version)
{ return papacc_control_version_decode(input, length, out_version); }
PAPACC_RESULT papacc_control_accept_encode(
    const PAPACC_CONTROL_PROTOCOL_VERSION *version, PAPACC_U8 *output,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_written)
{ return papacc_control_version_encode(version, output, capacity, out_written); }
PAPACC_RESULT papacc_control_accept_decode(
    const PAPACC_U8 *input, PAPACC_SIZE length,
    PAPACC_CONTROL_PROTOCOL_VERSION *out_version)
{ return papacc_control_version_decode(input, length, out_version); }

static PAPACC_FRAME_HEADER papacc_control_frame_header(PAPACC_U16 type)
{
    PAPACC_FRAME_HEADER header = {
        PAPACC_FRAME_ENVELOPE_MAJOR, PAPACC_FRAME_ENVELOPE_MINOR,
        PAPACC_FRAME_BASE_HEADER_SIZE, type, 0,
        PAPACC_CONTROL_VERSION_PAYLOAD_SIZE
    };
    return header;
}
PAPACC_FRAME_HEADER papacc_control_open_frame_header(void)
{ return papacc_control_frame_header(PAPACC_MESSAGE_TYPE_CONTROL_OPEN); }
PAPACC_FRAME_HEADER papacc_control_accept_frame_header(void)
{ return papacc_control_frame_header(PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT); }
