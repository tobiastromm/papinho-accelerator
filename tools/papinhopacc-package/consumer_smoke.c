#include <papacc/pacc/frame.h>
#include <papacc/pacc/frame_parser.h>
#include <papacc/pacc/message_types.h>
#include <papacc/pacc/control_protocol.h>
#include <papacc/pacc/data_association_protocol.h>
#include <papacc/pacc/capability_negotiation_protocol.h>
#include <papacc/pacc/network_offload_protocol.h>
#include <papacc/pacc/version.h>

int main(void)
{
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_U8 bytes[PAPACC_FRAME_BASE_HEADER_SIZE];
    PAPACC_SIZE written = 0U;
    volatile unsigned int api_major = PAPACC_PACC_API_VERSION_MAJOR;
    volatile unsigned int api_minor = PAPACC_PACC_API_VERSION_MINOR;
    volatile unsigned int wire_generation = PAPACC_PACC_WIRE_CONTRACT_GENERATION;
    PAPACC_NETWORK_ABORT_REASON_MESSAGE message =
        PAPACC_NETWORK_ABORT_REASON_MESSAGE_INITIALIZER;
    PAPACC_NETWORK_ABORT_REASON_MESSAGE decoded =
        PAPACC_NETWORK_ABORT_REASON_MESSAGE_INITIALIZER;
    PAPACC_U8 reason_bytes[2];
    PAPACC_SIZE reason_written = 0U;
    if (api_major != 1U || api_minor != 1U || wire_generation != 2U) return 1;
    header.envelope_major = 1U;
    header.envelope_minor = 0U;
    header.header_length = PAPACC_FRAME_BASE_HEADER_SIZE;
    header.message_type = PAPACC_MESSAGE_TYPE_CONTROL_OPEN;
    if (papacc_frame_header_encode(&header, bytes, sizeof(bytes), &written) !=
        PAPACC_RESULT_OK || written != sizeof(bytes)) return 2;
    if(bytes[8]!=0U||bytes[9]!=1U)return 3;
    message.reason=PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if(papacc_network_abort_reason_encode(&message,reason_bytes,
           sizeof(reason_bytes),&reason_written)!=PAPACC_RESULT_OK||
       reason_written!=2U||
       papacc_network_abort_reason_decode(reason_bytes,reason_written,
           &decoded)!=PAPACC_RESULT_OK||
       decoded.reason!=PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED)return 4;
    reason_bytes[0]=0x12U;reason_bytes[1]=0x34U;
    if(papacc_network_abort_reason_decode(reason_bytes,sizeof(reason_bytes),
           &decoded)!=PAPACC_RESULT_OK||decoded.reason==
           PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED||decoded.reason==
           PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED)return 5;
    return 0;
}
