#include "capability_negotiation_protocol.h"
#include "network_data_state.h"
#include "frame_parser.h"
#include "message_types.h"
#include <string.h>

#define CHECK(x,n) do { if (!(x)) return (n); } while (0)

static int test_registry(void)
{
    const PAPACC_U16 ids[]={PAPACC_MESSAGE_TYPE_CONTROL_OPEN,
        PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT,PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST,
        PAPACC_MESSAGE_TYPE_DATA_TICKET,PAPACC_MESSAGE_TYPE_DATA_ATTACH,
        PAPACC_MESSAGE_TYPE_DATA_ACCEPT,PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE,
        PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT,
        PAPACC_MESSAGE_TYPE_NETWORK_OPEN,PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
        PAPACC_MESSAGE_TYPE_NETWORK_DATA,PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE,
        PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,PAPACC_MESSAGE_TYPE_NETWORK_ABORT};
    PAPACC_SIZE i,j;
    for(i=0U;i<sizeof(ids)/sizeof(ids[0]);++i){
        CHECK(ids[i]==(PAPACC_U16)(i+1U),1);
        for(j=i+1U;j<sizeof(ids)/sizeof(ids[0]);++j)CHECK(ids[i]!=ids[j],2);
    }
    return 0;
}

static int test_capability_malformed(void)
{
    PAPACC_CAPABILITY_KEY a_store[3],b_store[3];
    PAPACC_CAPABILITY_SET a=PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET b=PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_U8 wire[40]={0};
    CHECK(papacc_capability_set_init(&a,a_store,3U)==PAPACC_RESULT_OK&&
        papacc_capability_set_init(&b,b_store,3U)==PAPACC_RESULT_OK,10);
    wire[1]=2U;wire[4]=0U;wire[5]=0U;wire[6]=0U;wire[7]=1U;
    wire[9]=1U;wire[10]=0U;wire[11]=0U;wire[12]=0U;wire[13]=1U;
    wire[15]=1U;
    CHECK(papacc_capability_negotiate_decode(wire,16U,&a,&b)==
        PAPACC_RESULT_PROTOCOL_ERROR&&a.count==0U&&b.count==0U,11);
    wire[7]=2U;wire[13]=1U;
    CHECK(papacc_capability_negotiate_decode(wire,16U,&a,&b)==
        PAPACC_RESULT_PROTOCOL_ERROR,12);
    memset(wire,0,sizeof(wire));wire[1]=1U;
    CHECK(papacc_capability_negotiate_decode(wire,10U,&a,&b)==
        PAPACC_RESULT_PROTOCOL_ERROR,13);
    wire[7]=1U;wire[9]=1U;
    CHECK(papacc_capability_negotiate_decode(wire,10U,&a,&b)==
        PAPACC_RESULT_OK&&a.count==1U,14);
    return 0;
}

static int test_open_limits_and_state(void)
{
    PAPACC_NETWORK_OPEN open=PAPACC_NETWORK_OPEN_INITIALIZER,decoded;
    PAPACC_NETWORK_DATA_PROTOCOL state=PAPACC_NETWORK_DATA_PROTOCOL_INITIALIZER;
    PAPACC_U8 wire[PAPACC_NETWORK_OPEN_MAX_PAYLOAD+1U];
    PAPACC_SIZE i,written=0U;
    open.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    open.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;open.port=443U;
    open.destination_size=PAPACC_NETWORK_DESTINATION_CAPACITY;
    memset(open.destination,'a',open.destination_size);
    open.alpn_count=PAPACC_NETWORK_ALPN_COUNT_MAX;
    for(i=0U;i<open.alpn_count;++i){open.alpn[i].size=
        PAPACC_NETWORK_ALPN_PROTOCOL_CAPACITY;
        memset(open.alpn[i].bytes,(int)('A'+i),open.alpn[i].size);}
    CHECK(papacc_network_open_encode(&open,wire,sizeof(wire),&written)==
        PAPACC_RESULT_OK&&written==PAPACC_NETWORK_OPEN_MAX_PAYLOAD,20);
    CHECK(papacc_network_open_decode(wire,written,&decoded)==PAPACC_RESULT_OK&&
        decoded.port==443U&&decoded.alpn_count==PAPACC_NETWORK_ALPN_COUNT_MAX,21);
    CHECK(papacc_network_data_protocol_init(&state,7U,8U)==PAPACC_RESULT_OK&&
        state.state==PAPACC_NETWORK_DATA_WAIT_OPEN,22);
    CHECK(papacc_network_data_protocol_accept(&state,
        PAPACC_MESSAGE_TYPE_NETWORK_DATA,wire,1U)==PAPACC_RESULT_PROTOCOL_ERROR&&
        state.state==PAPACC_NETWORK_DATA_WAIT_OPEN,23);
    CHECK(papacc_network_data_protocol_accept(&state,
        PAPACC_MESSAGE_TYPE_NETWORK_OPEN,wire,written)==PAPACC_RESULT_OK&&
        state.state==PAPACC_NETWORK_DATA_ESTABLISHING,24);
    CHECK(papacc_network_data_protocol_accept(&state,
        PAPACC_MESSAGE_TYPE_NETWORK_OPEN,wire,written)==PAPACC_RESULT_PROTOCOL_ERROR&&
        state.state==PAPACC_NETWORK_DATA_ESTABLISHING,25);
    wire[1]=0U;
    state=(PAPACC_NETWORK_DATA_PROTOCOL)PAPACC_NETWORK_DATA_PROTOCOL_INITIALIZER;
    CHECK(papacc_network_data_protocol_init(&state,7U,8U)==PAPACC_RESULT_OK&&
        papacc_network_data_protocol_accept(&state,PAPACC_MESSAGE_TYPE_NETWORK_OPEN,
            wire,written)==PAPACC_RESULT_PROTOCOL_ERROR&&
        state.state==PAPACC_NETWORK_DATA_WAIT_OPEN,26);
    CHECK(papacc_network_data_validate(PAPACC_NETWORK_DATA_MAX_PAYLOAD)==
        PAPACC_RESULT_OK&&papacc_network_data_validate(
            PAPACC_NETWORK_DATA_MAX_PAYLOAD+1U)==PAPACC_RESULT_LIMIT_EXCEEDED,27);
    CHECK(papacc_network_terminal_validate(PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,0U)==
        PAPACC_RESULT_OK&&papacc_network_terminal_validate(
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,1U)==PAPACC_RESULT_PROTOCOL_ERROR,28);
    return 0;
}

static int test_fragmentation_coalescing_and_reset(void)
{
    PAPACC_FRAME_PARSER parser=PAPACC_FRAME_PARSER_INITIALIZER;
    PAPACC_FRAME_PARSER_EVENT event=PAPACC_FRAME_PARSER_EVENT_INITIALIZER;
    PAPACC_FRAME_HEADER header=papacc_network_frame_header(
        PAPACC_MESSAGE_TYPE_NETWORK_OPEN,3U);
    PAPACC_U8 bytes[35],payload[3]={0x00U,0xA5U,0xFFU};
    PAPACC_SIZE written=0U,consumed=0U,offset=0U,events=0U;
    CHECK(papacc_frame_header_encode(&header,bytes,sizeof(bytes),&written)==
        PAPACC_RESULT_OK&&written==16U,30);
    memcpy(bytes+16U,payload,sizeof(payload));
    CHECK(papacc_frame_parser_init(&parser,PAPACC_NETWORK_OPEN_MAX_PAYLOAD)==
        PAPACC_RESULT_OK,31);
    while(offset<19U){CHECK(papacc_frame_parser_feed(&parser,bytes+offset,1U,
        &consumed,&event)==PAPACC_RESULT_OK&&consumed==1U,32);offset+=consumed;
        if(event.type!=PAPACC_FRAME_PARSER_EVENT_NONE)++events;}
    CHECK(events==4U&&event.type==PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE&&
        event.payload_length==1U&&event.payload[0]==0xFFU,33);
    header=papacc_network_frame_header(PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,0U);
    CHECK(papacc_frame_header_encode(&header,bytes,sizeof(bytes),&written)==
        PAPACC_RESULT_OK,34);
    header=papacc_network_frame_header(PAPACC_MESSAGE_TYPE_NETWORK_ABORT,0U);
    CHECK(papacc_frame_header_encode(&header,bytes+16U,sizeof(bytes)-16U,
        &written)==PAPACC_RESULT_OK,35);
    offset=events=0U;while(offset<32U){CHECK(papacc_frame_parser_feed(&parser,
        bytes+offset,32U-offset,&consumed,&event)==PAPACC_RESULT_OK&&consumed>0U,36);
        offset+=consumed;if(event.type==PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE)
            ++events;}
    CHECK(events==2U,37);
    CHECK(papacc_frame_parser_feed(&parser,bytes,7U,&consumed,&event)==
        PAPACC_RESULT_OK&&papacc_frame_parser_finish(&parser)==
        PAPACC_RESULT_PROTOCOL_ERROR,38);
    papacc_frame_parser_reset(&parser);
    CHECK(papacc_frame_parser_feed(&parser,bytes,16U,&consumed,&event)==
        PAPACC_RESULT_OK&&event.type==PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE,39);
    return 0;
}

int main(void)
{
    int result=test_registry();if(result!=0)return result;
    result=test_capability_malformed();if(result!=0)return result;
    result=test_open_limits_and_state();if(result!=0)return result;
    return test_fragmentation_coalescing_and_reset();
}
