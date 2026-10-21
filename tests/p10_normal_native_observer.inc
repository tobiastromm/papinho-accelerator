/* TEST_ONLY consumer checkpoints; no normal PST modifications. */
#if !defined(PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS) || defined(PAPACC_ENABLE_PST_TEST_CORRELATION)
#error Normal SDK observer requires consumer hooks without correlation.
#endif
static int p10_native_mode=-1;
static int p10_native_need_write=0;
static int p10_native_queued_probe=0;
static PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *p10_native_queued_slot=NULL;
static PAPACC_U64 p10_native_packet[16];
__declspec(noinline) void papacc_p10_normal_checkpoint(PAPACC_U64 *packet)
{
    volatile PAPACC_U64 stage=packet[0];
    (void)stage;
}
static void p10_native_checkpoint(AI_CASE_C_OBSERVER *o,PAPACC_U64 stage)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *s=o->secure_io;
    p10_native_packet[0]=stage;
    p10_native_packet[1]=(PAPACC_U64)p10_native_mode;
    if(stage==1U)p10_native_packet[2]=(PAPACC_U64)(uintptr_t)o->tls->processor.connection;
    p10_native_packet[3]=o->connection_instance_id;
    p10_native_packet[4]=o->channel_instance_id;
    p10_native_packet[5]=o->stream_instance_id;
    p10_native_packet[6]=GetCurrentThreadId();
    p10_native_packet[7]=(PAPACC_U64)(uintptr_t)&s->test_remove_observation;
    p10_native_packet[8]=s->shutdown_deadline_ns;
    p10_native_packet[9]=(PAPACC_U64)s->stop_cause;
    p10_native_packet[10]=(PAPACC_U64)s->stop_state;
    p10_native_packet[11]=o->owner_publication_attempts;
    p10_native_packet[12]=o->terminal_publication_attempts;
    p10_native_packet[13]=o->tls->processor.state;
    p10_native_packet[14]=s->scheduler.member_count;
    p10_native_packet[15]=(PAPACC_U64)(uintptr_t)o->tls->processor.connection;
    papacc_p10_normal_checkpoint(p10_native_packet);
}
static int p10_native_begin_pending(AI_CASE_C_OBSERVER *o)
{
    PAPACC_BOOL done=PAPACC_TRUE,read=PAPACC_FALSE,write=PAPACC_FALSE;
    pst_connection *identity=o->tls->processor.connection;
    PAPACC_U64 now=0U;
    if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK)return 0;
    if(p10_native_queued_probe){
        PAPACC_SERVER_IO_LOOP_WIN32 *loop=o->secure_io->protocol_loop;
        p10_native_queued_slot=papacc_server_io_loop_win32_find_connection_slot(loop,o->connection_instance_id);
        if(p10_native_queued_slot==NULL||
           papacc_server_io_loop_win32_test_inject_owner_terminal(loop,o->channel_instance_id,o->stream_instance_id,
                PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_OK||
           papacc_server_io_loop_win32_test_queue_owner_terminal(loop,p10_native_queued_slot,
                o->channel_instance_id,o->stream_instance_id,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_OK)return 0;
        puts("P10_B_QUEUE_CREATED_BEFORE_STOP=YES ACTIVE_IDENTITY=CONFIRMED NO_WIRE_DISPATCH=YES");
    }
    if(p10_native_need_write){
        static const PAPACC_U8 payload[16384]={0x53U};
        PAPACC_SIZE transferred=0U,total=0U;
        PAPACC_PST_SECURE_STEP step=PAPACC_PST_SECURE_STEP_FAILED;
        PAPACC_U32 attempts;
        for(attempts=0U;attempts<1024U;++attempts){
            if(papacc_pst_secure_processor_write_once(&o->tls->processor,payload,
                    sizeof(payload),&transferred,&step)!=PAPACC_RESULT_OK)return 0;
            total+=transferred;
            if(step==PAPACC_PST_SECURE_STEP_NEED_WRITE)break;
            if(step!=PAPACC_PST_SECURE_STEP_COMPLETE)return 0;
        }
        printf("S7_BACKPRESSURE_WRITE_STEP=%u BYTES=%llu CALLS=%u\n",(unsigned)step,
            (unsigned long long)total,(unsigned)(attempts+1U));
        if(step!=PAPACC_PST_SECURE_STEP_NEED_WRITE)return 0;
    }
    if(o->secure_io->stop_state!=PAPACC_SERVER_SECURE_STOP_RUNNING||
       o->p10_fixture->p10_owner_stop_observed!=0L||
       o->owner_publication_attempts!=0U||o->terminal_publication_attempts!=0U||
       papacc_network_stream_request_close(o->stream,now+5000000000ULL)!=PAPACC_RESULT_OK||
       o->delegate.shutdown(o->delegate.context,&done)!=PAPACC_RESULT_OK||done!=PAPACC_FALSE||
       o->tls->processor.connection!=identity||
       o->tls->processor.state!=PAPACC_PST_SECURE_PROCESSOR_SHUTTING_DOWN||
       o->tls->processor.source_result!=PST_RESULT_OK||
       papacc_pst_secure_processor_interest(&o->tls->processor,&read,&write)!=PAPACC_RESULT_OK||
       (p10_native_need_write?(write!=PAPACC_TRUE):(read!=PAPACC_TRUE||write!=PAPACC_FALSE)))return 0;
    o->shutdown_started=o->shutdown_pending=PAPACC_TRUE;
    p10_native_checkpoint(o,2U);
    printf("P10_NORMAL_PENDING=%s CONNECTION_INSTANCE_ID=%llu GENERATION=%llu NO_PRE_STOP_TERMINAL=YES\n",
        p10_native_need_write?"REAL_NEED_WRITE":"REAL_NEED_READ",(unsigned long long)o->connection_instance_id,(unsigned long long)o->stream_instance_id);
    return 1;
}
static int p10_native_queue_after_stop(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_IO_LOOP_WIN32 *loop=o->secure_io->protocol_loop;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 saved;
    if(!p10_native_queued_probe)return 1;
    if(p10_native_queued_slot==NULL||o->secure_io->stop_state==PAPACC_SERVER_SECURE_STOP_RUNNING)return 0;
    saved=*p10_native_queued_slot;
    if(saved.network_control_send_pending!=PAPACC_TRUE||
       saved.network_control_connection_id!=o->connection_instance_id||
       saved.network_control_operation_id!=o->channel_instance_id||
       saved.network_control_generation!=o->stream_instance_id||
       papacc_server_io_loop_win32_test_drain_owner_terminal(loop,p10_native_queued_slot,
            o->channel_instance_id,o->stream_instance_id)!=PAPACC_RESULT_INVALID_STATE||
       papacc_server_io_loop_win32_test_queue_owner_terminal(loop,p10_native_queued_slot,
            o->channel_instance_id,o->stream_instance_id,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_STATE||
       memcmp(&saved,p10_native_queued_slot,sizeof(saved))!=0||o->owner_publication_attempts!=0U)return 0;
    printf("P10_B_DISPATCH_AFTER_OWNER_STOP=REJECTED_REVOKED_IDENTITY CONNECTION=%llu OPERATION=%llu GENERATION=%llu QUEUE_PRESERVED=YES P10_C_CREATE_AFTER_STOP=REJECTED\n",
        (unsigned long long)o->connection_instance_id,(unsigned long long)o->channel_instance_id,
        (unsigned long long)o->stream_instance_id);
    return 1;
}
static int p10_native_late_callbacks(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *s=o->secure_io;
    PAPACC_SERVER_IO_LOOP_WIN32 *loop=s->protocol_loop;
    PAPACC_PST_SECURE_PROCESSOR processor=o->tls->processor;
    PAPACC_NETWORK_STREAM stream=*o->stream;
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION before,after;
    PAPACC_SIZE members=s->scheduler.member_count,connections=loop->connection_manager->count;
    PAPACC_SIZE streams=loop->network_stream_manager.count;
    PAPACC_U32 release=o->pst.release_attempt_count,close=o->original_close_calls;
    PAPACC_U32 attempts=o->owner_publication_attempts,successes=o->owner_publication_successes;
    PAPACC_U64 deadline=s->shutdown_deadline_ns;
    PAPACC_NETWORK_STREAM_TERMINAL_CAUSE cause=s->stop_cause;
    if(papacc_server_secure_io_win32_test_get_remove_observation(s,&before)!=PAPACC_RESULT_OK)return 0;
    if(papacc_server_io_loop_win32_test_inject_owner_terminal(loop,o->channel_instance_id,
            o->stream_instance_id,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_STATE||
       papacc_server_io_loop_win32_test_inject_owner_terminal(loop,o->channel_instance_id,
            o->stream_instance_id,PAPACC_MESSAGE_TYPE_NETWORK_ABORT)!=PAPACC_RESULT_INVALID_STATE||
       papacc_server_io_loop_win32_test_inject_owner_terminal(loop,o->channel_instance_id,
            o->stream_instance_id+1U,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_STATE||
       papacc_server_io_loop_win32_test_inject_owner_terminal(loop,o->channel_instance_id+0x100000U,
            o->stream_instance_id,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_STATE||
       papacc_server_io_loop_win32_test_inject_owner_terminal(loop,0U,0U,
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=PAPACC_RESULT_INVALID_ARGUMENT)return 0;
    if(papacc_server_secure_io_win32_test_get_remove_observation(s,&after)!=PAPACC_RESULT_OK||
       memcmp(&before,&after,sizeof(before))!=0||memcmp(&processor,&o->tls->processor,sizeof(processor))!=0||
       memcmp(&stream,o->stream,sizeof(stream))!=0||members!=s->scheduler.member_count||
       connections!=loop->connection_manager->count||streams!=loop->network_stream_manager.count||
       release!=o->pst.release_attempt_count||close!=o->original_close_calls||
       attempts!=o->owner_publication_attempts||successes!=o->owner_publication_successes||
       deadline!=s->shutdown_deadline_ns||cause!=s->stop_cause)return 0;
    printf("P10_LATE_CALLBACK_INTEGRATED=PASS CONNECTION=%llu OPERATION=%llu GENERATION=%llu PROBES=5 NO_RESOURCE_OR_PUBLICATION_MUTATION=YES\n",
        (unsigned long long)o->connection_instance_id,(unsigned long long)o->channel_instance_id,
        (unsigned long long)o->stream_instance_id);
    p10_native_checkpoint(o,7U);
    return 1;
}
static int p10_native_consumer_final(AI_CASE_C_OBSERVER *o)
{
    PAPACC_SERVER_SECURE_IO_WIN32 *s=o->secure_io;
    PAPACC_SERVER_SECURE_TEST_REMOVE_OBSERVATION r;
    PAPACC_U64 deadline=s->shutdown_deadline_ns;
    PAPACC_NETWORK_STREAM_TERMINAL_CAUSE cause=s->stop_cause;
    PAPACC_U32 i;
    if(p10_native_need_write){
        printf("S7_POST_STOP_TLS_PROGRESS_BYTES=%llu TERMINAL_OPERATION=%ld SOURCE=%ld OWNER_STOP_ACK=%ld\n",
            (unsigned long long)o->p10_fixture->p10_post_ack_bytes,
            (long)o->p10_fixture->diagnostic_shutdown_operation,
            (long)o->p10_fixture->diagnostic_shutdown_source,
            (long)o->p10_fixture->p10_owner_stop_observed);
        if(o->p10_fixture->p10_post_ack_bytes==0U||
           o->p10_fixture->p10_owner_stop_observed!=1L||
           o->p10_fixture->diagnostic_transport_terminal_observed!=1L)return 0;
    }
    p10_native_checkpoint(o,4U);
    printf("P10_FINAL_PUBLICATION_ATTEMPTS=%lu SUCCESSES=%lu FAILURES=%lu EVENT_OPERATION=%llu EVENT_GENERATION=%llu FINAL_CAUSE=%u FINAL_DEADLINE=%llu\n",
        (unsigned long)o->owner_publication_attempts,(unsigned long)o->owner_publication_successes,
        (unsigned long)o->owner_publication_failures,
        (unsigned long long)o->owner_publication_attempt_event.operation_id,
        (unsigned long long)o->owner_publication_attempt_event.stream_generation,
        (unsigned)cause,(unsigned long long)deadline);
    if(o->shutdown_pending!=PAPACC_TRUE||o->p10_stop_request_after_ack!=PAPACC_TRUE||
       deadline==0U||deadline!=o->p10_deadline_after_failure||
       cause!=PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SERVER_STOP_REQUESTED||
       cause!=o->p10_cause_after_failure||o->owner_publication_attempts!=0U||
       o->terminal_publication_attempts!=0U)return 0;
    for(i=0U;i<3U;++i){
        PAPACC_RESULT result=papacc_server_secure_io_win32_request_stop(s);
        if((p10_native_mode==2?result==PAPACC_RESULT_OK:result!=PAPACC_RESULT_OK)||
           s->shutdown_deadline_ns!=deadline||s->stop_cause!=cause)return 0;
    }
    if(papacc_server_secure_io_win32_test_get_remove_observation(s,&r)!=PAPACC_RESULT_OK||
       r.connection_instance_id!=o->connection_instance_id||r.stream_generation!=o->stream_instance_id||
       r.operation_id!=o->channel_instance_id||r.dispatch_active_at_attempt==PAPACC_TRUE)return 0;
    if(p10_native_mode==2){
        if(s->stop_state!=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL||
           o->tls->processor.connection==NULL||r.target_injected_failure_count==0U||
           r.target_real_remove_success_count!=0U||o->original_close_calls!=0U||
           o->pst.release_attempt_count!=0U)return 0;
        p10_native_checkpoint(o,5U);
    }else{
        if(s->stop_state!=PAPACC_SERVER_SECURE_STOP_STOPPED||
           s->scheduler.member_count!=0U||o->tls->processor.connection!=NULL||
           r.target_real_remove_call_count!=1U||r.target_real_remove_success_count!=1U||
           r.target_injected_failure_count!=(PAPACC_U32)p10_native_mode||
           o->original_close_calls!=1U||o->pst.release_attempt_count!=1U)return 0;
        p10_native_checkpoint(o,6U);
    }
    if(!p10_native_late_callbacks(o))return 0;
    puts("P10_NORMAL_CONSUMER=PASS NATIVE_ORACLE_REQUIRED=EXTERNAL REPEATED_STOP=3_CAUSE_DEADLINE_PRESERVED");
    return 1;
}
