#include "connection_classifier.h"
#include "control_processor.h"
#include "post_control_processor.h"

#include <string.h>

typedef struct TEST_STREAM {
    PAPACC_U8 bytes[96]; PAPACC_SIZE length; PAPACC_SIZE offset;
    PAPACC_SIZE limit; PAPACC_U32 reads; PAPACC_BOOL would_block;
    PAPACC_U32 closes;
} TEST_STREAM;

static PAPACC_RESULT test_read(void *context, PAPACC_U8 *buffer,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_count,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    TEST_STREAM *s = (TEST_STREAM *)context; PAPACC_SIZE count;
    ++s->reads;
    if (s->would_block) { s->would_block = PAPACC_FALSE; *out_count = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK; return PAPACC_RESULT_OK; }
    if (s->offset == s->length) { *out_count = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM; return PAPACC_RESULT_OK; }
    count = s->length - s->offset; if (count > capacity) count = capacity;
    if (count > s->limit) count = s->limit;
    memcpy(buffer, &s->bytes[s->offset], count); s->offset += count;
    *out_count = count; *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}
static PAPACC_RESULT test_write(void *context, const PAPACC_U8 *buffer,
    PAPACC_SIZE length, PAPACC_SIZE *out_count,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{ (void)context; (void)buffer; *out_count = length;
  *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS; return PAPACC_RESULT_OK; }
static void test_close(void *context) { ++((TEST_STREAM *)context)->closes; }

static PAPACC_CONNECTION *publish(PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_CONNECTION *storage, TEST_STREAM *stream)
{
    PAPACC_TRANSPORT_CONNECTION t = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT e = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    *manager = (PAPACC_CONNECTION_MANAGER)PAPACC_CONNECTION_MANAGER_INITIALIZER;
    (void)papacc_connection_manager_init(manager, storage, 1);
    (void)papacc_ip_address_set_ipv4(&e.address,127,0,0,1); e.port=1234;
    t.context=stream; t.read_fn=test_read; t.write_fn=test_write; t.close_fn=test_close;
    (void)papacc_connection_manager_publish(manager,&t,&e,&e,&connection);
    stream->limit=sizeof(stream->bytes); return connection;
}
static void frame(TEST_STREAM *s, PAPACC_U16 type, PAPACC_U32 length)
{
    PAPACC_FRAME_HEADER h = papacc_control_open_frame_header(); PAPACC_SIZE n;
    h.message_type=type; h.payload_length=length;
    (void)papacc_frame_header_encode(&h,s->bytes,16,&n);
    for(n=0;n<length;++n)s->bytes[16+n]=(PAPACC_U8)n;
    s->length=16+length;
}
static int classify(PAPACC_CONNECTION_CLASSIFIER *c,
    PAPACC_CONNECTION_CLASSIFIER_STATUS expected)
{
    PAPACC_CONNECTION_CLASSIFIER_STATUS status =
        PAPACC_CONNECTION_CLASSIFIER_STATUS_UNSPECIFIED;
    while(c->state==PAPACC_CONNECTION_CLASSIFIER_STATE_WAITING_FIRST_HEADER) {
        PAPACC_RESULT r=papacc_connection_classifier_read_once(c,&status);
        if(r!=PAPACC_RESULT_OK)return (int)r+10;
    }
    return status==expected?0:20;
}
static int test_control_and_data(void)
{
    PAPACC_U16 types[2]={PAPACC_MESSAGE_TYPE_CONTROL_OPEN,
        PAPACC_MESSAGE_TYPE_DATA_ATTACH}; PAPACC_U32 lengths[2]={4,16};
    PAPACC_CONNECTION_CLASSIFIER_STATUS statuses[2]={
        PAPACC_CONNECTION_CLASSIFIER_STATUS_CLASSIFIED_CONTROL,
        PAPACC_CONNECTION_CLASSIFIER_STATUS_CLASSIFIED_DATA}; PAPACC_SIZE i;
    for(i=0;i<2;++i){
        PAPACC_CONNECTION_MANAGER m; PAPACC_CONNECTION storage[1]; TEST_STREAM s;
        PAPACC_CONNECTION *connection; PAPACC_U8 scratch[64];
        PAPACC_CONNECTION_CLASSIFIER c=PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
        PAPACC_FRAMED_READER reader=PAPACC_FRAMED_READER_INITIALIZER;
        PAPACC_CONNECTION_CLASSIFICATION kind; PAPACC_FRAME_HEADER header;
        PAPACC_U64 deadline; PAPACC_FRAMED_READER_STATUS rs;
        PAPACC_FRAME_PARSER_EVENT event; memset(&s,0,sizeof(s));
        connection=publish(&m,storage,&s); frame(&s,types[i],lengths[i]);
        if(connection==NULL || papacc_connection_classifier_init(&c,&m,
            connection->connection_instance_id,scratch,sizeof(scratch),32,100)!=
            PAPACC_RESULT_OK || classify(&c,statuses[i]) || s.reads!=1 ||
            papacc_connection_classifier_take(&c,&kind,&header,&reader,&deadline)!=
            PAPACC_RESULT_OK || deadline!=100 || header.message_type!=types[i] ||
            papacc_framed_reader_next(&reader,&rs,&event)!=PAPACC_RESULT_OK ||
            event.type!=PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE ||
            event.payload_length!=lengths[i] || s.reads!=1 ||
            papacc_connection_classifier_take(&c,&kind,&header,&reader,&deadline)!=
                PAPACC_RESULT_INVALID_STATE || connection->state!=
                PAPACC_CONNECTION_STATE_PENDING) return 1+(int)i;
        papacc_connection_classifier_shutdown(&c);
        if(connection->state!=PAPACC_CONNECTION_STATE_PENDING)return 3;
    }
    return 0;
}
static int test_invalid_and_deadline(void)
{
    PAPACC_U16 types[]={PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT,
        PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST,PAPACC_MESSAGE_TYPE_DATA_TICKET,
        PAPACC_MESSAGE_TYPE_DATA_ACCEPT,0xBEEF}; PAPACC_SIZE i;
    for(i=0;i<sizeof(types)/sizeof(types[0]);++i){
        PAPACC_CONNECTION_MANAGER m; PAPACC_CONNECTION storage[1]; TEST_STREAM s;
        PAPACC_CONNECTION *connection; PAPACC_U8 scratch[20];
        PAPACC_CONNECTION_CLASSIFIER c=PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
        PAPACC_CONNECTION_CLASSIFIER_STATUS status; memset(&s,0,sizeof(s));
        connection=publish(&m,storage,&s); frame(&s,types[i],0);
        if(papacc_connection_classifier_init(&c,&m,connection->connection_instance_id,
            scratch,sizeof(scratch),32,10)!=PAPACC_RESULT_OK ||
            papacc_connection_classifier_read_once(&c,&status)!=
                PAPACC_RESULT_PROTOCOL_ERROR || connection->state!=
                PAPACC_CONNECTION_STATE_CLOSED) return 10;
    }
    {
        PAPACC_CONNECTION_MANAGER m; PAPACC_CONNECTION storage[1]; TEST_STREAM s;
        PAPACC_CONNECTION *connection; PAPACC_U8 scratch[20];
        PAPACC_CONNECTION_CLASSIFIER c=PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
        PAPACC_CONNECTION_CLASSIFIER_STATUS status; memset(&s,0,sizeof(s));
        connection=publish(&m,storage,&s); s.would_block=PAPACC_TRUE;
        if(papacc_connection_classifier_init(&c,&m,connection->connection_instance_id,
            scratch,sizeof(scratch),32,10)!=PAPACC_RESULT_OK ||
            papacc_connection_classifier_read_once(&c,&status)!=PAPACC_RESULT_OK ||
            status!=PAPACC_CONNECTION_CLASSIFIER_STATUS_WOULD_BLOCK ||
            papacc_connection_classifier_check_deadline(&c,10,&status)!=
                PAPACC_RESULT_OK || status!=PAPACC_CONNECTION_CLASSIFIER_STATUS_CLOSED)
            return 11;
    }
    return 0;
}

static PAPACC_RESULT generate_ticket(void *context,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket)
{
    PAPACC_SIZE i; (void)context;
    for(i=0;i<16;++i)out_ticket->bytes[i]=(PAPACC_U8)(i+1);
    return PAPACC_RESULT_OK;
}

static int test_control_to_post_control_pipeline(void)
{
    PAPACC_CONNECTION_MANAGER connections; PAPACC_CONNECTION connection_storage[1];
    PAPACC_SESSION_MANAGER sessions=PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_SESSION session_storage[1];
    PAPACC_CHANNEL_MANAGER channels=PAPACC_CHANNEL_MANAGER_INITIALIZER;
    PAPACC_CHANNEL channel_storage[1];
    PAPACC_DATA_ASSOCIATION_MANAGER associations=
        PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_ENTRY entries[1]; TEST_STREAM stream;
    PAPACC_CONNECTION *connection; PAPACC_U8 scratch[64]; PAPACC_SIZE written;
    PAPACC_FRAME_HEADER request=papacc_data_ticket_request_frame_header();
    PAPACC_CONNECTION_CLASSIFIER classifier=PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
    PAPACC_CONNECTION_CLASSIFIER_STATUS classifier_status;
    PAPACC_CONNECTION_CLASSIFICATION classification;
    PAPACC_FRAME_HEADER first_header; PAPACC_U64 deadline;
    PAPACC_FRAMED_READER classified_reader=PAPACC_FRAMED_READER_INITIALIZER;
    PAPACC_FRAMED_READER established_reader=PAPACC_FRAMED_READER_INITIALIZER;
    PAPACC_CONTROL_PROCESSOR control=PAPACC_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS control_status;
    PAPACC_POST_CONTROL_PROCESSOR post=PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_POST_CONTROL_STEP_STATUS post_status;
    memset(&stream,0,sizeof(stream)); connection=publish(&connections,
        connection_storage,&stream);
    frame(&stream,PAPACC_MESSAGE_TYPE_CONTROL_OPEN,4);
    stream.bytes[16]=0; stream.bytes[17]=1; stream.bytes[18]=0; stream.bytes[19]=0;
    (void)papacc_frame_header_encode(&request,&stream.bytes[20],16,&written);
    stream.length=36;
    if(connection==NULL || papacc_session_manager_init(&sessions,session_storage,1)!=
            PAPACC_RESULT_OK || papacc_channel_manager_init(&channels,channel_storage,1,
            &connections,&sessions)!=PAPACC_RESULT_OK ||
        papacc_data_association_manager_init(&associations,entries,1,&sessions,
            &channels,generate_ticket,NULL,100)!=PAPACC_RESULT_OK ||
        papacc_connection_classifier_init(&classifier,&connections,
            connection->connection_instance_id,scratch,sizeof(scratch),16,100)!=
            PAPACC_RESULT_OK ||
        papacc_connection_classifier_read_once(&classifier,&classifier_status)!=
            PAPACC_RESULT_OK || classifier_status!=
            PAPACC_CONNECTION_CLASSIFIER_STATUS_CLASSIFIED_CONTROL || stream.reads!=1 ||
        papacc_connection_classifier_take(&classifier,&classification,&first_header,
            &classified_reader,&deadline)!=PAPACC_RESULT_OK || classification!=
            PAPACC_CONNECTION_CLASSIFICATION_CONTROL ||
        papacc_control_processor_init_from_reader(&control,&connections,&sessions,
            &channels,connection->connection_instance_id,&first_header,
            &classified_reader,deadline)!=PAPACC_RESULT_OK) return 30;
    while(control.state!=PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT)
        if(papacc_control_processor_read_once(&control,&control_status)!=
            PAPACC_RESULT_OK)return 31;
    if(stream.reads!=1)return 32;
    while(control.state==PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT)
        if(papacc_control_processor_write_once(&control,&control_status)!=
            PAPACC_RESULT_OK)return 33;
    if(control.state!=PAPACC_CONTROL_PROCESSOR_STATE_ESTABLISHED ||
        papacc_control_processor_handoff_reader(&control,&established_reader)!=
            PAPACC_RESULT_OK || sessions.storage[0].state!=PAPACC_SESSION_STATE_ACTIVE ||
        channels.storage[0].state!=PAPACC_CHANNEL_STATE_BOUND ||
        connection->state!=PAPACC_CONNECTION_STATE_ASSOCIATED ||
        papacc_post_control_processor_init_from_reader(&post,&connections,&sessions,
            &channels,&associations,sessions.storage[0].session_instance_id,
            channels.storage[0].channel_instance_id,&established_reader)!=
            PAPACC_RESULT_OK ||
        papacc_post_control_processor_read_once(&post,10,&post_status)!=
            PAPACC_RESULT_OK || post.state!=
            PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET || stream.reads!=1 ||
        associations.count!=1)return 34;
    papacc_post_control_processor_shutdown(&post); return 0;
}

static int test_wrong_lengths_and_fragmentation(void)
{
    PAPACC_U32 control_lengths[]={0,3,5,16};
    PAPACC_U32 data_lengths[]={0,15,17}; PAPACC_SIZE group,index;
    for(group=0;group<2;++group){
        PAPACC_U32 *lengths=group==0?control_lengths:data_lengths;
        PAPACC_SIZE count=group==0?4:3;
        for(index=0;index<count;++index){
            PAPACC_CONNECTION_MANAGER m; PAPACC_CONNECTION storage[1]; TEST_STREAM s;
            PAPACC_CONNECTION *connection; PAPACC_U8 scratch[64];
            PAPACC_CONNECTION_CLASSIFIER c=PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
            PAPACC_CONNECTION_CLASSIFIER_STATUS status; memset(&s,0,sizeof(s));
            connection=publish(&m,storage,&s); frame(&s,group==0?
                PAPACC_MESSAGE_TYPE_CONTROL_OPEN:PAPACC_MESSAGE_TYPE_DATA_ATTACH,
                lengths[index]);
            if(papacc_connection_classifier_init(&c,&m,
                connection->connection_instance_id,scratch,sizeof(scratch),32,100)!=
                    PAPACC_RESULT_OK ||
                papacc_connection_classifier_read_once(&c,&status)!=
                    PAPACC_RESULT_PROTOCOL_ERROR || connection->state!=
                    PAPACC_CONNECTION_STATE_CLOSED)return 40;
        }
    }
    {
        PAPACC_CONNECTION_MANAGER m; PAPACC_CONNECTION storage[1]; TEST_STREAM s;
        PAPACC_CONNECTION *connection; PAPACC_U8 scratch[4];
        PAPACC_CONNECTION_CLASSIFIER c=PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;
        memset(&s,0,sizeof(s)); connection=publish(&m,storage,&s);
        frame(&s,PAPACC_MESSAGE_TYPE_CONTROL_OPEN,4); s.limit=1;
        if(papacc_connection_classifier_init(&c,&m,connection->connection_instance_id,
            scratch,sizeof(scratch),16,100)!=PAPACC_RESULT_OK || classify(&c,
            PAPACC_CONNECTION_CLASSIFIER_STATUS_CLASSIFIED_CONTROL) || s.reads!=16)
            return 41;
    }
    return 0;
}

int main(void)
{
    int r=test_control_and_data(); if(r)return r;
    r=test_invalid_and_deadline(); if(r)return r;
    r=test_control_to_post_control_pipeline(); if(r)return r;
    return test_wrong_lengths_and_fragmentation();
}
