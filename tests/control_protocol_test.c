#include "control_protocol.h"

#include <string.h>

int main(void)
{
    static const PAPACC_U8 open_golden[20] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,1,0,0,0,0,0,4,0,1,0,0 };
    static const PAPACC_U8 accept_golden[20] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,2,0,0,0,0,0,4,0,1,0,0 };
    PAPACC_CONTROL_PROTOCOL_VERSION version = { 1, 0 };
    PAPACC_CONTROL_PROTOCOL_VERSION decoded =
        PAPACC_CONTROL_PROTOCOL_VERSION_INITIALIZER;
    PAPACC_FRAME_HEADER header;
    PAPACC_U8 bytes[20] = { 0 };
    PAPACC_SIZE written = 0;

    header = papacc_control_open_frame_header();
    if (papacc_frame_header_encode(&header, bytes, 16, &written) !=
            PAPACC_RESULT_OK ||
        papacc_control_open_encode(&version, &bytes[16], 4, &written) !=
            PAPACC_RESULT_OK || memcmp(bytes, open_golden, 20) != 0 ||
        papacc_control_open_decode(&bytes[16], 4, &decoded) != PAPACC_RESULT_OK ||
        decoded.major != 1 || decoded.minor != 0) return 1;
    header = papacc_control_accept_frame_header();
    if (papacc_frame_header_encode(&header, bytes, 16, &written) !=
            PAPACC_RESULT_OK ||
        papacc_control_accept_encode(&version, &bytes[16], 4, &written) !=
            PAPACC_RESULT_OK || memcmp(bytes, accept_golden, 20) != 0 ||
        papacc_control_accept_decode(&bytes[16], 4, &decoded) != PAPACC_RESULT_OK)
        return 2;
    if (papacc_control_open_decode(bytes, 3, &decoded) !=
            PAPACC_RESULT_PROTOCOL_ERROR ||
        papacc_control_open_decode(bytes, 5, &decoded) !=
            PAPACC_RESULT_PROTOCOL_ERROR) return 3;
    bytes[16] = 0; bytes[17] = 2; bytes[18] = 0; bytes[19] = 0;
    if (papacc_control_open_decode(&bytes[16], 4, &decoded) !=
            PAPACC_RESULT_NOT_SUPPORTED) return 4;
    bytes[16] = 0; bytes[17] = 1; bytes[18] = 0; bytes[19] = 1;
    if (papacc_control_accept_decode(&bytes[16], 4, &decoded) !=
            PAPACC_RESULT_NOT_SUPPORTED ||
        papacc_control_open_decode(NULL, 4, &decoded) !=
            PAPACC_RESULT_INVALID_ARGUMENT) return 5;
    return 0;
}
