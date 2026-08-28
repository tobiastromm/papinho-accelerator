#include <string.h>

#include "frame.h"

static const PAPACC_U8 papacc_test_golden[19] = {
    0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
    0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0xAA, 0xBB, 0xCC
};

static PAPACC_FRAME_HEADER papacc_test_header(
    PAPACC_U16 message_type,
    PAPACC_U32 payload_length)
{
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    header.envelope_major = PAPACC_FRAME_ENVELOPE_MAJOR;
    header.envelope_minor = PAPACC_FRAME_ENVELOPE_MINOR;
    header.header_length = PAPACC_FRAME_BASE_HEADER_SIZE;
    header.message_type = message_type;
    header.flags = 0;
    header.payload_length = payload_length;
    return header;
}

static int papacc_test_encode_decode(void)
{
    PAPACC_FRAME_HEADER header = papacc_test_header(0x1234, 3);
    PAPACC_FRAME_HEADER decoded = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_U8 output[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE written = 99;

    if (papacc_frame_header_encode(
            &header, output, sizeof(output), &written) != PAPACC_RESULT_OK ||
        written != PAPACC_FRAME_BASE_HEADER_SIZE ||
        memcmp(output, papacc_test_golden, sizeof(output)) != 0 ||
        papacc_frame_header_decode(
            output, sizeof(output), 3, &decoded) != PAPACC_RESULT_OK ||
        decoded.envelope_major != 1 || decoded.envelope_minor != 0 ||
        decoded.header_length != 16 || decoded.message_type != 0x1234 ||
        decoded.flags != 0 || decoded.payload_length != 3) {
        return 1;
    }
    header = papacc_test_header(0xBEEF, UINT32_MAX);
    if (papacc_frame_header_encode(
            &header, output, sizeof(output), &written) != PAPACC_RESULT_OK ||
        output[8] != 0xBE || output[9] != 0xEF ||
        output[12] != 0xFF || output[13] != 0xFF ||
        output[14] != 0xFF || output[15] != 0xFF ||
        papacc_frame_header_decode(
            output, sizeof(output), UINT32_MAX, &decoded) != PAPACC_RESULT_OK ||
        decoded.message_type != 0xBEEF ||
        decoded.payload_length != UINT32_MAX) {
        return 2;
    }
    return 0;
}

static int papacc_test_validation_and_atomicity(void)
{
    PAPACC_FRAME_HEADER header = papacc_test_header(1, 0);
    PAPACC_FRAME_HEADER decoded = papacc_test_header(9, 9);
    PAPACC_U8 output[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_U8 before[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE written = 99;
    PAPACC_SIZE index;

    memset(output, 0xA5, sizeof(output));
    memcpy(before, output, sizeof(output));
    if (papacc_frame_header_validate(NULL, 0) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_frame_header_encode(NULL, output, sizeof(output), &written) !=
            PAPACC_RESULT_INVALID_ARGUMENT || written != 0 ||
        memcmp(output, before, sizeof(output)) != 0 ||
        papacc_frame_header_encode(&header, NULL, sizeof(output), &written) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_frame_header_encode(&header, output, sizeof(output), NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_frame_header_encode(
            &header, output, sizeof(output) - 1U, &written) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || written != 0 ||
        memcmp(output, before, sizeof(output)) != 0 ||
        papacc_frame_header_decode(NULL, 0, 0, &decoded) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        decoded.message_type != 0 ||
        papacc_frame_header_decode(output, 15, 0, &decoded) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || decoded.message_type != 0 ||
        papacc_frame_header_decode(output, 16, 0, NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 10;
    }
    for (index = 0; index < PAPACC_FRAME_MAGIC_SIZE; ++index) {
        PAPACC_U8 malformed[PAPACC_FRAME_BASE_HEADER_SIZE];
        memcpy(malformed, papacc_test_golden, sizeof(malformed));
        malformed[index] ^= 0x01;
        if (papacc_frame_header_decode(
                malformed, sizeof(malformed), 3, &decoded) !=
                PAPACC_RESULT_PROTOCOL_ERROR || decoded.message_type != 0) {
            return 11;
        }
    }
    return 0;
}

static int papacc_test_field_errors(void)
{
    PAPACC_U8 bytes[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_FRAME_HEADER decoded = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_SIZE written;
    PAPACC_FRAME_HEADER header = papacc_test_header(1, 0);
    PAPACC_U16 invalid_lengths[3] = { 0, 15, 17 };
    PAPACC_U16 invalid_flags[3] = { 1, 0x8000, 0xFFFF };
    PAPACC_SIZE index;

    (void)papacc_frame_header_encode(&header, bytes, sizeof(bytes), &written);
    bytes[4] = 2;
    if (papacc_frame_header_decode(bytes, sizeof(bytes), 0, &decoded) !=
        PAPACC_RESULT_NOT_SUPPORTED) {
        return 20;
    }
    bytes[4] = 1;
    bytes[5] = 1;
    if (papacc_frame_header_decode(bytes, sizeof(bytes), 0, &decoded) !=
        PAPACC_RESULT_NOT_SUPPORTED) {
        return 21;
    }
    for (index = 0; index < 3; ++index) {
        header = papacc_test_header(1, 0);
        header.header_length = invalid_lengths[index];
        if (papacc_frame_header_validate(&header, 0) !=
            PAPACC_RESULT_PROTOCOL_ERROR) {
            return 22;
        }
        header = papacc_test_header(1, 0);
        header.flags = invalid_flags[index];
        if (papacc_frame_header_validate(&header, 0) !=
            PAPACC_RESULT_PROTOCOL_ERROR) {
            return 23;
        }
    }
    header = papacc_test_header(0, 0);
    if (papacc_frame_header_validate(&header, 0) !=
            PAPACC_RESULT_PROTOCOL_ERROR ||
        papacc_frame_header_encode(
            &header, bytes, sizeof(bytes), &written) !=
            PAPACC_RESULT_PROTOCOL_ERROR || written != 0) {
        return 24;
    }
    header = papacc_test_header(0xBEEF, 101);
    if (papacc_frame_header_validate(&header, 100) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        papacc_frame_header_validate(&header, 101) != PAPACC_RESULT_OK) {
        return 25;
    }
    return 0;
}

int main(void)
{
    PAPACC_FRAME_HEADER initializer = PAPACC_FRAME_HEADER_INITIALIZER;
    int result;
    if (initializer.envelope_major != 0 || initializer.envelope_minor != 0 ||
        initializer.header_length != 0 || initializer.message_type != 0 ||
        initializer.flags != 0 || initializer.payload_length != 0 ||
        PAPACC_FRAME_MAGIC_SIZE != 4 ||
        PAPACC_FRAME_BASE_HEADER_SIZE != 16 ||
        PAPACC_FRAME_ENVELOPE_MAJOR != 1 ||
        PAPACC_FRAME_ENVELOPE_MINOR != 0) {
        return 30;
    }
    result = papacc_test_encode_decode();
    if (result == 0) {
        result = papacc_test_validation_and_atomicity();
    }
    if (result == 0) {
        result = papacc_test_field_errors();
    }
    return result;
}
