/* Independent memory-wire oracle over real Connection, queue and framed writer. */
typedef struct QUEUE_CASE {
    PAPACC_SERVER_IO_LOOP_WIN32 loop;
    PAPACC_CONNECTION_MANAGER manager;
    PAPACC_CONNECTION connections[1],*connection;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 slot;
    PAPACC_NETWORK_STREAM streams[2],*stream;
    PAPACC_U8 tx[128],rx[128];
    TEST_LOG log;TEST_SINK sink;
    PAPACC_U64 operation,generation;
} QUEUE_CASE;
static int queue_case_init(QUEUE_CASE*q)
{
    PAPACC_NETWORK_ENDPOINT endpoint=PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport;
    PAPACC_NETWORK_STREAM_IO io=PAPACC_NETWORK_STREAM_IO_INITIALIZER;
    memset(q,0,sizeof(*q));
    q->loop=(PAPACC_SERVER_IO_LOOP_WIN32)PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    q->manager=(PAPACC_CONNECTION_MANAGER)PAPACC_CONNECTION_MANAGER_INITIALIZER;
    q->slot=(PAPACC_SERVER_PROTOCOL_SLOT_WIN32)PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    if(papacc_ip_address_set_ipv4(&endpoint.address,127,0,0,1)!=PAPACC_RESULT_OK)return 0;
    endpoint.port=41000U;
    transport=make_transport(&q->sink);
    if(papacc_connection_manager_init(&q->manager,q->connections,1U)!=PAPACC_RESULT_OK||
       papacc_connection_manager_publish(&q->manager,&transport,&endpoint,&endpoint,&q->connection)!=PAPACC_RESULT_OK)return 0;
    q->loop.connection_manager=&q->manager;
    io.context=q;io.read=stream_read;io.write=stream_write;
    io.half_close=stream_half_close;io.close=stream_close;
    if(papacc_network_stream_manager_init(&q->loop.network_stream_manager,
        q->streams,q->tx,q->rx,2U,2U,64U)!=PAPACC_RESULT_OK||
       papacc_server_io_loop_win32_test_set_owner_terminal_observer(&q->loop,observe,&q->log)!=PAPACC_RESULT_OK)return 0;
    q->operation=101U;
    if(reserve_stream(&q->loop,&io,q->operation,&q->stream)!=PAPACC_RESULT_OK)return 0;
    q->generation=q->stream->stream_instance_id;
    q->slot.in_use=PAPACC_TRUE;q->slot.kind=PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH;
    q->slot.connection_instance_id=q->connection->connection_instance_id;
    q->slot.network_data_protocol.channel_instance_id=q->operation;
    q->slot.network_stream_generation=q->generation;
    q->slot.establishment.loop=&q->loop;q->slot.establishment.reservation=q->stream;
    return papacc_framed_writer_init(&q->slot.data_attach_processor.writer,
        &q->connection->transport)==PAPACC_RESULT_OK&&
        papacc_server_io_loop_win32_test_inject_owner_terminal(&q->loop,q->operation,
            q->generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)==PAPACC_RESULT_OK;
}
static PAPACC_RESULT queue_case_enqueue(QUEUE_CASE*q)
{
    return papacc_server_io_loop_win32_test_queue_owner_terminal(&q->loop,&q->slot,
        q->operation,q->generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE);
}
static PAPACC_RESULT queue_case_drain(QUEUE_CASE*q)
{
    return papacc_server_io_loop_win32_test_drain_owner_terminal(&q->loop,&q->slot,
        q->operation,q->generation);
}
typedef struct QUEUE_CANCEL_THREAD {
    QUEUE_CASE*q;HANDLE barrier;PAPACC_RESULT result;
} QUEUE_CANCEL_THREAD;
static DWORD WINAPI queue_after_cancel_thread(void*context)
{
    QUEUE_CANCEL_THREAD*c=(QUEUE_CANCEL_THREAD*)context;
    if(WaitForSingleObject(c->barrier,5000U)!=WAIT_OBJECT_0)return 1U;
    c->result=queue_case_drain(c->q);return 0U;
}
static int publication_authority_cases(void)
{
    QUEUE_CASE q;PAPACC_SERVER_PROTOCOL_SLOT_WIN32 saved;
    QUEUE_CANCEL_THREAD cross;HANDLE thread;PAPACC_U32 i;
    for(i=0U;i<10U;++i){
        if(!queue_case_init(&q))return 101;
        if(i==3U)papacc_connection_close(q.connection);
        if(i==4U){
            if(papacc_server_io_loop_win32_test_queue_owner_terminal(&q.loop,&q.slot,
                q.operation,q.generation+1U,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_STATE)return 104;
        }else if(i==5U){
            if(papacc_server_io_loop_win32_test_queue_owner_terminal(&q.loop,&q.slot,
                q.operation+1U,q.generation,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_STATE)return 105;
        }else if(i==6U){
            if(papacc_server_io_loop_win32_test_queue_owner_terminal(&q.loop,&q.slot,
                0U,0U,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_ARGUMENT)return 106;
        }else if(i==3U){
            if(queue_case_enqueue(&q)!=PAPACC_RESULT_INVALID_STATE)return 103;
        }else{
            if(queue_case_enqueue(&q)!=PAPACC_RESULT_OK)return 102;
            saved=q.slot;
            if(i==2U){
                papacc_connection_close(q.connection);
                if(queue_case_drain(&q)!=PAPACC_RESULT_INVALID_STATE||
                   memcmp(&saved,&q.slot,sizeof(saved))!=0)return 112;
            }else if(i==8U){
                cross.q=&q;cross.result=PAPACC_RESULT_OK;
                cross.barrier=CreateEventA(NULL,TRUE,FALSE,NULL);
                if(cross.barrier==NULL)return 118;
                thread=CreateThread(NULL,0U,queue_after_cancel_thread,&cross,0U,NULL);
                if(thread==NULL)return 119;
                papacc_connection_close(q.connection);
                if(!SetEvent(cross.barrier)||WaitForSingleObject(thread,5000U)!=WAIT_OBJECT_0)return 120;
                CloseHandle(thread);CloseHandle(cross.barrier);
                if(cross.result!=PAPACC_RESULT_INVALID_STATE||queue_case_drain(&q)!=PAPACC_RESULT_INVALID_STATE)return 121;
            }else{
                if(queue_case_drain(&q)!=PAPACC_RESULT_OK)return 110;
                if(i==7U&&queue_case_enqueue(&q)!=PAPACC_RESULT_INVALID_STATE)return 117;
                if(q.sink.length!=16U||q.sink.bytes[0]!=0x50U||q.sink.bytes[3]!=0x43U||
                   q.sink.bytes[9]!=0x0dU||q.sink.writes!=1U)return 111;
            }
        }
        if(i==2U||i==3U||i==4U||i==5U||i==6U||i==8U){
            if(q.sink.length!=0U||q.sink.writes!=0U||
               count_kind(&q.log,PAPACC_OWNER_TERMINAL_TEST_EVENT_PUBLICATION_ATTEMPT)!=0)return 122;
        }
        papacc_network_stream_manager_shutdown(&q.loop.network_stream_manager);
        papacc_connection_manager_shutdown(&q.manager);
    }
    puts("PUBLICATION_AUTHORITY_A=PASS B_ACTIVE_AND_REVOKED=PASS C=PASS D=PASS E=PASS F=PASS G=PASS K_OWNER_SERIALIZATION=PASS L_WIRE=PASS CASES=10_OF_10");
    return 0;
}
