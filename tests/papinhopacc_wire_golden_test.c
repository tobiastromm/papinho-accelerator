#include <papacc/pacc/capability_negotiation_protocol.h>
#include <papacc/pacc/network_offload_protocol.h>
#include <string.h>

static int check_frame(PAPACC_U16 type, const PAPACC_U8 *payload,
    PAPACC_SIZE payload_size, const PAPACC_U8 *expected,
    PAPACC_SIZE expected_size)
{
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_U8 encoded[64];
    PAPACC_SIZE written = 0U;
    header.envelope_major = 1U;
    header.envelope_minor = 0U;
    header.header_length = 16U;
    header.message_type = type;
    header.payload_length = (PAPACC_U32)payload_size;
    if (expected_size != 16U + payload_size || expected_size > sizeof(encoded) ||
        papacc_frame_header_encode(&header, encoded, sizeof(encoded),
            &written) != PAPACC_RESULT_OK || written != 16U) return 0;
    if (payload_size > 0U) memcpy(encoded + 16U, payload, payload_size);
    return memcmp(encoded, expected, expected_size) == 0;
}

int main(void)
{
    static const PAPACC_U8 negotiate[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,7,0,0,0,0,0,16,
        0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1 };
    static const PAPACC_U8 negotiated[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,8,0,0,0,0,0,8,
        0,1,0,0,0,1,0,1 };
    static const PAPACC_U8 opened[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,9,0,0,0,0,0,9,
        1,1,1,0,1,0xBB,0,1,'a' };
    static const PAPACC_U8 open_result[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,10,0,0,0,0,0,1,0 };
    static const PAPACC_U8 network_data[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,11,0,0,0,0,0,3,0,0xA5,0xFF };
    static const PAPACC_U8 half_close[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,12,0,0,0,0,0,0 };
    static const PAPACC_U8 close_frame[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,13,0,0,0,0,0,0 };
    static const PAPACC_U8 abort_frame[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,14,0,0,0,0,0,0 };
    static const PAPACC_U8 abort_reason_frame[] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,15,0,0,0,0,0,2,0,1 };
    const PAPACC_CAPABILITY_KEY key = { 1U, 1U };
    PAPACC_CAPABILITY_KEY s_storage[1], p_storage[1];
    PAPACC_CAPABILITY_SET supported = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET preference = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_U8 payload[32];
    PAPACC_SIZE written = 0U;
    PAPACC_U8 data[3] = { 0U, 0xA5U, 0xFFU };
    PAPACC_NETWORK_ABORT_REASON_MESSAGE reason =
        PAPACC_NETWORK_ABORT_REASON_MESSAGE_INITIALIZER;
    const PAPACC_CAPABILITY_KEY v2 =
        PAPACC_PACC_NETWORK_EGRESS_V2_KEY_INITIALIZER;
    if (papacc_capability_set_init(&supported, s_storage, 1U) != PAPACC_RESULT_OK ||
        papacc_capability_set_init(&preference, p_storage, 1U) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&supported, &key) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&preference, &key) != PAPACC_RESULT_OK ||
        papacc_capability_negotiate_encode(&supported, &preference, payload,
            sizeof(payload), &written) != PAPACC_RESULT_OK ||
        !check_frame(PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE, payload,
            written, negotiate, sizeof(negotiate))) return 1;
    if (papacc_capability_negotiate_result_encode(&supported, payload,
            sizeof(payload), &written) != PAPACC_RESULT_OK ||
        !check_frame(PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT, payload,
            written, negotiated, sizeof(negotiated))) return 2;
    request.port = 443U;
    request.destination_size = 1U;
    request.destination[0] = 'a';
    if (papacc_network_open_encode(&request, payload, sizeof(payload),
            &written) != PAPACC_RESULT_OK ||
        !check_frame(PAPACC_MESSAGE_TYPE_NETWORK_OPEN, payload, written,
            opened, sizeof(opened))) return 3;
    if (papacc_network_open_result_encode(PAPACC_NETWORK_OPEN_OK, payload,
            sizeof(payload), &written) != PAPACC_RESULT_OK ||
        !check_frame(PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT, payload, written,
            open_result, sizeof(open_result))) return 4;
    if (papacc_network_data_validate(sizeof(data)) != PAPACC_RESULT_OK ||
        !check_frame(PAPACC_MESSAGE_TYPE_NETWORK_DATA, data, sizeof(data),
            network_data, sizeof(network_data))) return 5;
    if (papacc_network_terminal_validate(PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE,
            0U) != PAPACC_RESULT_OK ||
        !check_frame(PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE, NULL, 0U,
            half_close, sizeof(half_close)) ||
        !check_frame(PAPACC_MESSAGE_TYPE_NETWORK_CLOSE, NULL, 0U,
            close_frame, sizeof(close_frame)) ||
        !check_frame(PAPACC_MESSAGE_TYPE_NETWORK_ABORT, NULL, 0U,
            abort_frame, sizeof(abort_frame))) return 6;
    if(v2.id!=PAPACC_PACC_NETWORK_EGRESS_ID||
       v2.major!=PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2)return 7;
    reason.reason=PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if(papacc_network_abort_reason_encode(&reason,payload,sizeof(payload),
           &written)!=PAPACC_RESULT_OK||written!=2U||payload[0]!=0U||
       payload[1]!=1U||
       !check_frame(PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON,payload,written,
           abort_reason_frame,sizeof(abort_reason_frame)))return 8;
    reason.reason=PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED;
    if(papacc_network_abort_reason_decode(payload,2U,&reason)!=PAPACC_RESULT_OK||
       reason.reason!=PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED)return 9;
    payload[0]=0xFFU;payload[1]=0xFFU;
    if(papacc_network_abort_reason_decode(payload,2U,&reason)!=PAPACC_RESULT_OK||
       reason.reason!=(PAPACC_NETWORK_TERMINAL_REASON)0xFFFFU)return 10;
    payload[0]=0U;payload[1]=0U;
    if(papacc_network_abort_reason_decode(payload,2U,&reason)!=
           PAPACC_RESULT_PROTOCOL_ERROR||
       papacc_network_abort_reason_decode(payload,0U,&reason)!=
           PAPACC_RESULT_PROTOCOL_ERROR||
       papacc_network_abort_reason_decode(payload,1U,&reason)!=
           PAPACC_RESULT_PROTOCOL_ERROR||
       papacc_network_abort_reason_decode(payload,3U,&reason)!=
           PAPACC_RESULT_PROTOCOL_ERROR)return 11;
    reason.reason=PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED;
    if(papacc_network_abort_reason_encode(&reason,payload,sizeof(payload),
           &written)!=PAPACC_RESULT_INVALID_ARGUMENT||written!=0U)return 12;
    reason.reason=(PAPACC_NETWORK_TERMINAL_REASON)2U;
    if(papacc_network_abort_reason_encode(&reason,payload,sizeof(payload),
           &written)!=PAPACC_RESULT_INVALID_ARGUMENT||written!=0U)return 13;
    reason.reason=PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;
    if(papacc_network_abort_reason_encode(&reason,payload,1U,&written)!=
           PAPACC_RESULT_LIMIT_EXCEEDED||written!=0U)return 14;
    if(papacc_network_terminal_validate(PAPACC_MESSAGE_TYPE_NETWORK_ABORT,0U)!=
           PAPACC_RESULT_OK||
       papacc_network_terminal_validate(PAPACC_MESSAGE_TYPE_NETWORK_ABORT,2U)!=
           PAPACC_RESULT_PROTOCOL_ERROR||
       papacc_network_terminal_validate(
           PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON,2U)!=PAPACC_RESULT_OK||
       papacc_network_terminal_validate(
           PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON,0U)!=
           PAPACC_RESULT_PROTOCOL_ERROR||
       papacc_network_terminal_validate((PAPACC_U16)0xFFFFU,0U)!=
           PAPACC_RESULT_INVALID_ARGUMENT)return 15;
    return 0;
}
