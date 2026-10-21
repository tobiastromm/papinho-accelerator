#include "server_io_loop_win32.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define CHECK(x,n) do { if (!(x)) return (n); } while (0)
#define INVALID_TERMINAL ((PAPACC_U16)0xFFFFU)

typedef struct TEST_LOG {
    PAPACC_OWNER_TERMINAL_TEST_EVENT events[96];
    PAPACC_SIZE count;
} TEST_LOG;

typedef struct THREAD_INPUT {
    PAPACC_SERVER_IO_LOOP_WIN32 *loop;
    PAPACC_U64 operation;
    PAPACC_U64 generation;
    PAPACC_RESULT result;
} THREAD_INPUT;

typedef struct TEST_SINK {
    PAPACC_U8 bytes[64];
    PAPACC_SIZE length;
    PAPACC_BOOL fail;
    PAPACC_U32 writes;
} TEST_SINK;

static void observe(void *context,const PAPACC_OWNER_TERMINAL_TEST_EVENT *event)
{
    TEST_LOG *log=(TEST_LOG *)context;
    if(log!=NULL&&event!=NULL&&log->count<96U)log->events[log->count++]=*event;
}

static PAPACC_RESULT stream_read(void *c,PAPACC_U8 *b,PAPACC_SIZE z,
    PAPACC_SIZE *n,PAPACC_BOOL *e)
{(void)c;(void)b;(void)z;*n=0U;*e=PAPACC_FALSE;return PAPACC_RESULT_OK;}
static PAPACC_RESULT stream_write(void *c,const PAPACC_U8 *b,PAPACC_SIZE z,
    PAPACC_SIZE *n)
{(void)c;(void)b;*n=z;return PAPACC_RESULT_OK;}
static PAPACC_RESULT stream_half_close(void *c)
{(void)c;return PAPACC_RESULT_OK;}
static void stream_close(void *c){(void)c;}

static PAPACC_RESULT sink_read(void *c,PAPACC_U8 *b,PAPACC_SIZE z,
    PAPACC_SIZE *n,PAPACC_TRANSPORT_IO_STATUS *status)
{(void)c;(void)b;(void)z;*n=0U;*status=PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;return PAPACC_RESULT_OK;}
static PAPACC_RESULT sink_write(void *context,const PAPACC_U8 *buffer,
    PAPACC_SIZE length,PAPACC_SIZE *written,PAPACC_TRANSPORT_IO_STATUS *status)
{
    TEST_SINK *sink=(TEST_SINK *)context;
    ++sink->writes;
    if(sink->fail==PAPACC_TRUE)return PAPACC_RESULT_INTERNAL_ERROR;
    if(sink->length+length>sizeof(sink->bytes))return PAPACC_RESULT_LIMIT_EXCEEDED;
    memcpy(&sink->bytes[sink->length],buffer,length);sink->length+=length;
    *written=length;*status=PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}
static void sink_close(void *c){(void)c;}

static PAPACC_TRANSPORT_CONNECTION make_transport(TEST_SINK *sink)
{
    PAPACC_TRANSPORT_CONNECTION t=PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    t.context=sink;t.read_fn=sink_read;t.write_fn=sink_write;t.close_fn=sink_close;
    return t;
}

static int count_kind(const TEST_LOG *log,PAPACC_OWNER_TERMINAL_TEST_EVENT_KIND kind)
{
    PAPACC_SIZE i;int count=0;
    for(i=0U;i<log->count;++i)if(log->events[i].kind==kind)++count;
    return count;
}

static const PAPACC_OWNER_TERMINAL_TEST_EVENT *find_last_kind(
    const TEST_LOG *log,PAPACC_OWNER_TERMINAL_TEST_EVENT_KIND kind)
{
    PAPACC_SIZE i=log->count;
    while(i>0U){--i;if(log->events[i].kind==kind)return &log->events[i];}
    return NULL;
}

static int count_validation(const TEST_LOG *log,PAPACC_RESULT result)
{
    PAPACC_SIZE i;int count=0;
    for(i=0U;i<log->count;++i)
        if(log->events[i].kind==PAPACC_OWNER_TERMINAL_TEST_EVENT_VALIDATED&&
           log->events[i].validation_result==result)++count;
    return count;
}

static DWORD WINAPI inject_from_other_thread(void *context)
{
    THREAD_INPUT *input=(THREAD_INPUT *)context;
    input->result=papacc_server_io_loop_win32_test_inject_owner_terminal(
        input->loop,input->operation,input->generation,
        PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
    return 0U;
}

static PAPACC_RESULT reserve_stream(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    PAPACC_NETWORK_STREAM_IO *io,PAPACC_U64 operation,PAPACC_NETWORK_STREAM **out)
{
    PAPACC_RESULT r=papacc_network_stream_reserve(&loop->network_stream_manager,
        1U,operation,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,out);
    if(r==PAPACC_RESULT_OK)r=papacc_network_stream_publish(
        &loop->network_stream_manager,*out,io);
    return r;
}

#include "p10_publication_authority_cases.inc"

int main(void)
{
    PAPACC_SERVER_IO_LOOP_WIN32 loop=PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_NETWORK_STREAM storage[4],*stream;
    PAPACC_U8 tx[256],rx[256];
    PAPACC_NETWORK_STREAM_IO io=PAPACC_NETWORK_STREAM_IO_INITIALIZER;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 slot=PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    TEST_LOG log;TEST_SINK sink;PAPACC_TRANSPORT_CONNECTION transport;
    THREAD_INPUT input;HANDLE thread;PAPACC_U64 generation;PAPACC_SIZE before;
    int accepted_before,attempt_before,success_before;
    PAPACC_BOOL negatives[7]={PAPACC_FALSE,PAPACC_FALSE,PAPACC_FALSE,
        PAPACC_FALSE,PAPACC_FALSE,PAPACC_FALSE,PAPACC_FALSE};

    { int authority_result=publication_authority_cases();
      if(authority_result!=0){fprintf(stderr,"PUBLICATION_AUTHORITY_FAILURE=%d\n",authority_result);return authority_result;} }
    memset(&log,0,sizeof(log));memset(&sink,0,sizeof(sink));
    io.context=&loop;io.read=stream_read;io.write=stream_write;
    io.half_close=stream_half_close;io.close=stream_close;
    CHECK(papacc_network_stream_manager_init(&loop.network_stream_manager,
        storage,tx,rx,4U,4U,64U)==PAPACC_RESULT_OK,1);
    CHECK(papacc_server_io_loop_win32_test_set_owner_terminal_observer(
        &loop,observe,&log)==PAPACC_RESULT_OK,2);

    /* Invalid event reaches the real owner validation boundary and is rejected. */
    CHECK(reserve_stream(&loop,&io,10U,&stream)==PAPACC_RESULT_OK,3);
    generation=stream->stream_instance_id;accepted_before=count_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_ACCEPTED);
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,10U,
        generation,INVALID_TERMINAL)==PAPACC_RESULT_INVALID_ARGUMENT,4);
    CHECK(count_validation(&log,PAPACC_RESULT_INVALID_ARGUMENT)==1&&
        count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_ACCEPTED)==accepted_before&&
        stream->accepted_terminal_type==0U,5);
    negatives[0]=PAPACC_TRUE;
    CHECK(papacc_network_stream_close_channel(&loop.network_stream_manager,10U)==
        PAPACC_RESULT_OK,6);

    /* A real framed writer publishes exactly one PACC NETWORK_CLOSE header. */
    CHECK(reserve_stream(&loop,&io,11U,&stream)==PAPACC_RESULT_OK,7);
    generation=stream->stream_instance_id;
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,11U,
        generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)==PAPACC_RESULT_OK,8);
    transport=make_transport(&sink);
    CHECK(papacc_framed_writer_init(&slot.data_attach_processor.writer,&transport)==
        PAPACC_RESULT_OK,9);
    slot.network_data_protocol.channel_instance_id=11U;
    CHECK(papacc_server_io_loop_win32_test_publish_owner_terminal(&loop,&slot,
        11U,generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)==PAPACC_RESULT_OK,10);
    CHECK(sink.length==PAPACC_FRAME_BASE_HEADER_SIZE&&sink.writes==1U&&
        count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_ATTEMPT)==1&&
        count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS)==1&&
        count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE)==0,11);
    CHECK(find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS)!=NULL&&
        find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS)->publication_result==
            PAPACC_RESULT_OK&&find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS)->writer_status==
            PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE&&find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS)->transport_status_available==
            PAPACC_FALSE,36);
    attempt_before=count_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_ATTEMPT);
    success_before=count_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS);
    CHECK(papacc_server_io_loop_win32_test_publish_owner_terminal(&loop,&slot,
        11U,generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)==PAPACC_RESULT_INVALID_STATE,12);
    CHECK(count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_ATTEMPT)==
        attempt_before&&count_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS)==success_before,13);
    negatives[1]=PAPACC_TRUE;

    /* A transport error is an attempted, failed publication, never success. */
    slot=(PAPACC_SERVER_PROTOCOL_SLOT_WIN32)PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    memset(&sink,0,sizeof(sink));sink.fail=PAPACC_TRUE;
    CHECK(reserve_stream(&loop,&io,12U,&stream)==PAPACC_RESULT_OK,14);
    generation=stream->stream_instance_id;
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,12U,
        generation,PAPACC_MESSAGE_TYPE_NETWORK_ABORT)==PAPACC_RESULT_OK,15);
    transport=make_transport(&sink);
    CHECK(papacc_framed_writer_init(&slot.data_attach_processor.writer,&transport)==
        PAPACC_RESULT_OK,16);
    slot.network_data_protocol.channel_instance_id=12U;
    success_before=count_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS);
    CHECK(papacc_server_io_loop_win32_test_publish_owner_terminal(&loop,&slot,
        12U,generation,PAPACC_MESSAGE_TYPE_NETWORK_ABORT)!=PAPACC_RESULT_OK,17);
    CHECK(count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE)==1&&
        count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_SUCCESS)==
        success_before,18);
    CHECK(find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE)!=NULL&&
        find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE)->publication_result==
            PAPACC_RESULT_INTERNAL_ERROR&&find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE)->writer_status==
            PAPACC_FRAMED_WRITER_STATUS_UNSPECIFIED&&find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE)->transport_status==
            PAPACC_TRANSPORT_IO_STATUS_UNSPECIFIED&&find_last_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_FAILURE)->transport_status_available==
            PAPACC_FALSE,37);

    /* Disconnecting observation cannot silently satisfy the oracle. */
    CHECK(reserve_stream(&loop,&io,13U,&stream)==PAPACC_RESULT_OK,19);
    generation=stream->stream_instance_id;before=log.count;
    CHECK(papacc_server_io_loop_win32_test_set_owner_terminal_observer(
        &loop,NULL,NULL)==PAPACC_RESULT_OK,20);
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,13U,
        generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)==PAPACC_RESULT_OK,21);
    CHECK(log.count==before&&stream->accepted_terminal_type==
        PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,22);
    negatives[2]=PAPACC_TRUE;
    CHECK(papacc_server_io_loop_win32_test_set_owner_terminal_observer(
        &loop,observe,&log)==PAPACC_RESULT_OK,23);
    CHECK(papacc_network_stream_close_channel(&loop.network_stream_manager,13U)==
        PAPACC_RESULT_OK,24);

    /* Generation, owner-thread, overwrite and late-event negatives. */
    CHECK(reserve_stream(&loop,&io,14U,&stream)==PAPACC_RESULT_OK,25);
    generation=stream->stream_instance_id;
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,14U,
        generation+1U,PAPACC_MESSAGE_TYPE_NETWORK_ABORT)==PAPACC_RESULT_INVALID_STATE,26);
    negatives[3]=PAPACC_TRUE;
    input.loop=&loop;input.operation=14U;input.generation=generation;
    input.result=PAPACC_RESULT_OK;
    thread=CreateThread(NULL,0U,inject_from_other_thread,&input,0U,NULL);
    CHECK(thread!=NULL&&WaitForSingleObject(thread,5000U)==WAIT_OBJECT_0,27);
    CloseHandle(thread);CHECK(input.result==PAPACC_RESULT_INVALID_STATE,28);
    negatives[4]=PAPACC_TRUE;
    accepted_before=count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_ACCEPTED);
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,14U,
        generation,PAPACC_MESSAGE_TYPE_NETWORK_ABORT)==PAPACC_RESULT_OK,29);
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,14U,
        generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)==PAPACC_RESULT_INVALID_STATE,30);
    CHECK(stream->accepted_terminal_type==PAPACC_MESSAGE_TYPE_NETWORK_ABORT&&
        stream->accepted_terminal_generation==generation&&count_kind(&log,
        PAPACC_OWNER_TERMINAL_TEST_EVENT_ACCEPTED)==accepted_before+1,31);
    negatives[5]=PAPACC_TRUE;
    CHECK(papacc_network_stream_close_channel(&loop.network_stream_manager,14U)==
        PAPACC_RESULT_OK,32);
    CHECK(papacc_server_io_loop_win32_test_inject_owner_terminal(&loop,14U,
        generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)==PAPACC_RESULT_INVALID_STATE,33);
    CHECK(count_kind(&log,PAPACC_OWNER_TERMINAL_TEST_EVENT_LATE_AFTER_TEARDOWN)==1,34);
    negatives[6]=PAPACC_TRUE;

    CHECK(negatives[0]&&negatives[1]&&negatives[2]&&negatives[3]&&
        negatives[4]&&negatives[5]&&negatives[6],35);
    puts("OWNER_PUBLICATION_ATTEMPT=PASS");
    puts("OWNER_PUBLICATION_SUCCESS=PASS");
    puts("OWNER_PUBLICATION_FAILURE=PASS");
    puts("OWNER_PUBLICATION_SUPPRESSION=PASS");
    puts("OBSERVER_DISCONNECTION_NEGATIVE=PASS");
    puts("INVALID_EVENT_ACCEPTANCE_NEGATIVE=PASS");
    puts("DUPLICATE_PUBLICATION_NEGATIVE=PASS");
    puts("CAUSE_OVERWRITE_NEGATIVE=PASS");
    puts("GENERATION_NEGATIVE=PASS");
    puts("THREAD_AFFINITY_NEGATIVE=PASS");
    puts("LATE_EVENT_NEGATIVE=PASS");
    puts("ORACLE_NEGATIVES_EXECUTED=7_OF_7");
    puts("ORACLE_NEGATIVES=PASS");
    papacc_network_stream_manager_shutdown(&loop.network_stream_manager);
    return 0;
}