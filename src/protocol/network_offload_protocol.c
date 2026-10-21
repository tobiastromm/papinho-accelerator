#include "network_offload_protocol.h"
#include <string.h>

static PAPACC_BOOL open_valid(const PAPACC_NETWORK_OPEN *o)
{
    PAPACC_SIZE i,j;
    if(o==NULL||o->port==0U||o->destination_size==0U||
       o->destination_size>PAPACC_NETWORK_DESTINATION_CAPACITY||
       o->alpn_count>PAPACC_NETWORK_ALPN_COUNT_MAX)return PAPACC_FALSE;
    if(o->destination_kind<PAPACC_NETWORK_DESTINATION_DNS_ASCII||
       o->destination_kind>PAPACC_NETWORK_DESTINATION_IPV6)return PAPACC_FALSE;
    if(o->transport_mode!=PAPACC_NETWORK_TRANSPORT_TCP_PLAIN&&
       o->transport_mode!=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)return PAPACC_FALSE;
    if(o->transport_mode==PAPACC_NETWORK_TRANSPORT_TCP_PLAIN&&o->alpn_count!=0U)
       return PAPACC_FALSE;
    for(i=0U;i<o->alpn_count;++i){
        if(o->alpn[i].size==0U||o->alpn[i].size>PAPACC_NETWORK_ALPN_PROTOCOL_CAPACITY)
            return PAPACC_FALSE;
        for(j=0U;j<i;++j)if(o->alpn[j].size==o->alpn[i].size&&
            memcmp(o->alpn[j].bytes,o->alpn[i].bytes,o->alpn[i].size)==0)
            return PAPACC_FALSE;
    }
    return PAPACC_TRUE;
}
PAPACC_RESULT papacc_network_open_encode(const PAPACC_NETWORK_OPEN *o,
    PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *written)
{
    PAPACC_SIZE i,pos,need=8U;
    if(written!=NULL)*written=0U;
    if(out==NULL||written==NULL||open_valid(o)!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_ARGUMENT;
    for(i=0U;i<o->alpn_count;++i)need+=1U+o->alpn[i].size;
    need+=o->destination_size;if(cap<need)return PAPACC_RESULT_LIMIT_EXCEEDED;
    out[0]=1U;out[1]=(PAPACC_U8)o->destination_kind;out[2]=(PAPACC_U8)o->transport_mode;
    out[3]=o->alpn_count;out[4]=(PAPACC_U8)(o->port>>8);out[5]=(PAPACC_U8)o->port;
    out[6]=(PAPACC_U8)(o->destination_size>>8);out[7]=(PAPACC_U8)o->destination_size;
    memcpy(out+8,o->destination,o->destination_size);pos=8U+o->destination_size;
    for(i=0U;i<o->alpn_count;++i){out[pos++]=o->alpn[i].size;
        memcpy(out+pos,o->alpn[i].bytes,o->alpn[i].size);pos+=o->alpn[i].size;}
    *written=pos;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_network_open_decode(const PAPACC_U8 *in,PAPACC_SIZE len,
    PAPACC_NETWORK_OPEN *o)
{
    PAPACC_NETWORK_OPEN v=PAPACC_NETWORK_OPEN_INITIALIZER;PAPACC_SIZE i,pos;
    if(o!=NULL)*o=v;
    if(in==NULL||o==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(len<8U||in[0]!=1U)return PAPACC_RESULT_PROTOCOL_ERROR;
    v.destination_kind=(PAPACC_NETWORK_DESTINATION_KIND)in[1];
    v.transport_mode=(PAPACC_NETWORK_TRANSPORT_MODE)in[2];v.alpn_count=in[3];
    v.port=(PAPACC_U16)(((PAPACC_U16)in[4]<<8)|in[5]);
    v.destination_size=(PAPACC_U16)(((PAPACC_U16)in[6]<<8)|in[7]);
    if(v.destination_size>PAPACC_NETWORK_DESTINATION_CAPACITY||
       8U+v.destination_size>len||v.alpn_count>PAPACC_NETWORK_ALPN_COUNT_MAX)
       return PAPACC_RESULT_PROTOCOL_ERROR;
    memcpy(v.destination,in+8,v.destination_size);pos=8U+v.destination_size;
    for(i=0U;i<v.alpn_count;++i){if(pos>=len)return PAPACC_RESULT_PROTOCOL_ERROR;
        v.alpn[i].size=in[pos++];if(v.alpn[i].size>PAPACC_NETWORK_ALPN_PROTOCOL_CAPACITY||
          pos+v.alpn[i].size>len)return PAPACC_RESULT_PROTOCOL_ERROR;
        memcpy(v.alpn[i].bytes,in+pos,v.alpn[i].size);pos+=v.alpn[i].size;}
    if(pos!=len||open_valid(&v)!=PAPACC_TRUE)return PAPACC_RESULT_PROTOCOL_ERROR;
    *o=v;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_network_open_result_encode(PAPACC_NETWORK_OPEN_STATUS s,
    PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *written)
{if(written!=NULL)*written=0U;if(out==NULL||written==NULL||s<PAPACC_NETWORK_OPEN_OK||s>PAPACC_NETWORK_OPEN_FAILED)return PAPACC_RESULT_INVALID_ARGUMENT;if(cap<1U)return PAPACC_RESULT_LIMIT_EXCEEDED;out[0]=(PAPACC_U8)s;*written=1U;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_network_open_result_decode(const PAPACC_U8 *in,PAPACC_SIZE len,
    PAPACC_NETWORK_OPEN_STATUS *s)
{if(in==NULL||s==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;if(len!=1U||in[0]>PAPACC_NETWORK_OPEN_FAILED)return PAPACC_RESULT_PROTOCOL_ERROR;*s=(PAPACC_NETWORK_OPEN_STATUS)in[0];return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_network_data_validate(PAPACC_SIZE n)
{return n<=PAPACC_NETWORK_DATA_MAX_PAYLOAD?PAPACC_RESULT_OK:PAPACC_RESULT_LIMIT_EXCEEDED;}
PAPACC_RESULT papacc_network_terminal_validate(PAPACC_U16 type,PAPACC_SIZE n)
{if(type==PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON)return n==2U?PAPACC_RESULT_OK:PAPACC_RESULT_PROTOCOL_ERROR;if(type!=PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE&&type!=PAPACC_MESSAGE_TYPE_NETWORK_CLOSE&&type!=PAPACC_MESSAGE_TYPE_NETWORK_ABORT)return PAPACC_RESULT_INVALID_ARGUMENT;return n==0U?PAPACC_RESULT_OK:PAPACC_RESULT_PROTOCOL_ERROR;}
PAPACC_RESULT papacc_network_abort_reason_encode(
    const PAPACC_NETWORK_ABORT_REASON_MESSAGE *message,PAPACC_U8 *out,
    PAPACC_SIZE capacity,PAPACC_SIZE *written)
{
    if(written!=NULL)*written=0U;
    if(message==NULL||out==NULL||written==NULL||
       message->reason!=PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if(capacity<2U)return PAPACC_RESULT_LIMIT_EXCEEDED;
    out[0]=(PAPACC_U8)(message->reason>>8);
    out[1]=(PAPACC_U8)message->reason;
    *written=2U;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_network_abort_reason_decode(const PAPACC_U8 *in,
    PAPACC_SIZE length,PAPACC_NETWORK_ABORT_REASON_MESSAGE *message)
{
    PAPACC_NETWORK_ABORT_REASON_MESSAGE value=
        PAPACC_NETWORK_ABORT_REASON_MESSAGE_INITIALIZER;
    if(message!=NULL)*message=value;
    if(in==NULL||message==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(length!=2U)return PAPACC_RESULT_PROTOCOL_ERROR;
    value.reason=(PAPACC_NETWORK_TERMINAL_REASON)
        (((PAPACC_U16)in[0]<<8)|in[1]);
    if(value.reason==PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED)
        return PAPACC_RESULT_PROTOCOL_ERROR;
    *message=value;return PAPACC_RESULT_OK;
}
PAPACC_FRAME_HEADER papacc_network_frame_header(PAPACC_U16 type,PAPACC_U32 n)
{PAPACC_FRAME_HEADER h={1U,0U,16U,0U,0U,0U};
 h.message_type=type;h.payload_length=n;return h;}
