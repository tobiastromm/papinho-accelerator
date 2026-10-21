#include "post_control_processor.h"
#include <string.h>

static void papacc_post_control_close_scope(
    PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    (void)papacc_data_association_manager_invalidate_session(
        processor->association_manager, processor->session_instance_id);
    (void)papacc_channel_manager_close(
        processor->channel_manager, processor->control_channel_instance_id);
}

static PAPACC_RESULT papacc_post_control_dispatch_close(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_CONTROL_CLOSE_ORIGIN origin, PAPACC_RESULT result)
{
    PAPACC_RESULT close_result = PAPACC_RESULT_OK;
    if (processor->close_dispatched == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    processor->close_result.origin = origin;
    processor->close_result.pacc_result = result;
    processor->close_result.session_instance_id =
        processor->session_instance_id;
    processor->close_result.control_channel_instance_id =
        processor->control_channel_instance_id;
    if (processor->close_fn != NULL)
        close_result = processor->close_fn(processor->close_context,
            &processor->close_result);
    else
        papacc_post_control_close_scope(processor);
    if (close_result == PAPACC_RESULT_OK)
        processor->close_dispatched = PAPACC_TRUE;
    return close_result;
}

static PAPACC_RESULT papacc_post_control_fail(
    PAPACC_POST_CONTROL_PROCESSOR *processor, PAPACC_RESULT result)
{
    PAPACC_CONTROL_CLOSE_ORIGIN origin =
        result == PAPACC_RESULT_PROTOCOL_ERROR ?
        PAPACC_CONTROL_CLOSE_ORIGIN_PACC_PROTOCOL_FAILURE :
        PAPACC_CONTROL_CLOSE_ORIGIN_OWNER_FAILURE;
    PAPACC_RESULT close_result = papacc_post_control_dispatch_close(
        processor, origin, result);
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_ERROR;
    return close_result == PAPACC_RESULT_OK ? result : close_result;
}

static PAPACC_RESULT papacc_post_control_close(
    PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    PAPACC_RESULT result = papacc_post_control_dispatch_close(processor,
        PAPACC_CONTROL_CLOSE_ORIGIN_NORMAL, PAPACC_RESULT_OK);
    if (result == PAPACC_RESULT_OK)
        processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_CLOSED;
    return result;
}

PAPACC_RESULT papacc_post_control_processor_init(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 session_instance_id,
    PAPACC_U64 control_channel_instance_id,
    PAPACC_U8 *read_scratch, PAPACC_SIZE read_scratch_capacity)
{
    PAPACC_SESSION *session;
    PAPACC_CHANNEL *channel;
    PAPACC_CONNECTION *connection;
    PAPACC_RESULT result;
    if (processor == NULL || connection_manager == NULL ||
        session_manager == NULL || channel_manager == NULL ||
        association_manager == NULL || session_instance_id == 0 ||
        control_channel_instance_id == 0 || read_scratch == NULL ||
        read_scratch_capacity == 0)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    if (channel_manager->initialized != PAPACC_TRUE ||
        channel_manager->connection_manager != connection_manager ||
        channel_manager->session_manager != session_manager ||
        association_manager->initialized != PAPACC_TRUE ||
        association_manager->session_manager != session_manager ||
        association_manager->channel_manager != channel_manager)
        return PAPACC_RESULT_INVALID_STATE;
    session = papacc_session_manager_find(session_manager, session_instance_id);
    channel = papacc_channel_manager_find(
        channel_manager, control_channel_instance_id);
    if (session == NULL || session->state != PAPACC_SESSION_STATE_ACTIVE ||
        channel == NULL || channel->state != PAPACC_CHANNEL_STATE_BOUND ||
        channel->role != PAPACC_CHANNEL_ROLE_CONTROL ||
        channel->session_instance_id != session_instance_id)
        return PAPACC_RESULT_INVALID_STATE;
    connection = papacc_connection_manager_find(
        connection_manager, channel->connection_instance_id);
    if (connection == NULL ||
        connection->state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        papacc_transport_connection_is_valid(&connection->transport) !=
            PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_init(
        &processor->reader, &connection->transport, read_scratch,
        read_scratch_capacity, 28U);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_framed_writer_init(&processor->writer,
        &connection->transport);
    if (result != PAPACC_RESULT_OK) {
        papacc_framed_reader_shutdown(&processor->reader);
        return result;
    }
    processor->connection_manager = connection_manager;
    processor->session_manager = session_manager;
    processor->channel_manager = channel_manager;
    processor->association_manager = association_manager;
    processor->connection = connection;
    processor->session_instance_id = session_instance_id;
    processor->control_channel_instance_id = control_channel_instance_id;
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_READY;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_init_from_reader(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 session_instance_id,
    PAPACC_U64 control_channel_instance_id,
    PAPACC_FRAMED_READER *reader)
{
    PAPACC_SESSION *session;
    PAPACC_CHANNEL *channel;
    PAPACC_CONNECTION *connection;
    PAPACC_RESULT result;
    if (processor == NULL || connection_manager == NULL ||
        session_manager == NULL || channel_manager == NULL ||
        association_manager == NULL || reader == NULL ||
        session_instance_id == 0 || control_channel_instance_id == 0)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED ||
        reader->state != PAPACC_FRAMED_READER_STATE_READY ||
        channel_manager->initialized != PAPACC_TRUE ||
        channel_manager->connection_manager != connection_manager ||
        channel_manager->session_manager != session_manager ||
        association_manager->initialized != PAPACC_TRUE ||
        association_manager->session_manager != session_manager ||
        association_manager->channel_manager != channel_manager)
        return PAPACC_RESULT_INVALID_STATE;
    session = papacc_session_manager_find(session_manager, session_instance_id);
    channel = papacc_channel_manager_find(
        channel_manager, control_channel_instance_id);
    if (session == NULL || session->state != PAPACC_SESSION_STATE_ACTIVE ||
        channel == NULL || channel->state != PAPACC_CHANNEL_STATE_BOUND ||
        channel->role != PAPACC_CHANNEL_ROLE_CONTROL ||
        channel->session_instance_id != session_instance_id)
        return PAPACC_RESULT_INVALID_STATE;
    connection = papacc_connection_manager_find(
        connection_manager, channel->connection_instance_id);
    if (connection == NULL || connection->state !=
            PAPACC_CONNECTION_STATE_ASSOCIATED ||
        papacc_framed_reader_transport(reader) != &connection->transport)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_writer_init(&processor->writer,
        &connection->transport);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_framed_reader_move(&processor->reader, reader);
    if (result != PAPACC_RESULT_OK) {
        papacc_framed_writer_shutdown(&processor->writer);
        return result;
    }
    processor->connection_manager = connection_manager;
    processor->reader.parser.max_payload_length = 28U;
    processor->session_manager = session_manager;
    processor->channel_manager = channel_manager;
    processor->association_manager = association_manager;
    processor->connection = connection;
    processor->session_instance_id = session_instance_id;
    processor->control_channel_instance_id = control_channel_instance_id;
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_READY;
    return PAPACC_RESULT_OK;
}

PAPACC_BOOL papacc_post_control_processor_wants_read(
    const PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    return processor != NULL &&
        processor->state == PAPACC_POST_CONTROL_PROCESSOR_STATE_READY
        ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_BOOL papacc_post_control_processor_wants_write(
    const PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    return processor != NULL && (processor->state ==
        PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET || processor->state ==
        PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_CAPABILITIES)
        ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_post_control_processor_set_capability_gate(
    PAPACC_POST_CONTROL_PROCESSOR *p, PAPACC_SESSION_CAPABILITY_STORE *store,
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *inputs)
{
    if(p==NULL||store==NULL||inputs==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(p->state!=PAPACC_POST_CONTROL_PROCESSOR_STATE_READY||p->capability_store!=NULL||
       !store->initialized||inputs->registry==NULL||inputs->server_supported==NULL||
       inputs->server_enabled==NULL||inputs->principal_policy==NULL)
        return PAPACC_RESULT_INVALID_STATE;
    p->capability_store=store;p->capability_inputs=*inputs;return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_set_close_handler(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_CONTROL_CLOSE_FN close_fn, void *context)
{
    if (processor == NULL || close_fn == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_POST_CONTROL_PROCESSOR_STATE_READY ||
        processor->close_fn != NULL ||
        processor->close_dispatched == PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    processor->close_fn = close_fn;
    processor->close_context = context;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_set_capability_test_gate(
    PAPACC_POST_CONTROL_PROCESSOR *p,
    PAPACC_CAPABILITY_PROCESSING_TEST_GATE_FN gate_fn, void *context)
{
    if (p == NULL || gate_fn == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (p->state != PAPACC_POST_CONTROL_PROCESSOR_STATE_READY ||
        p->capability_test_gate_fn != NULL)
        return PAPACC_RESULT_INVALID_STATE;
    p->capability_test_gate_fn = gate_fn;
    p->capability_test_gate_context = context;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_post_control_prepare_capabilities(
    PAPACC_POST_CONTROL_PROCESSOR *p)
{
    PAPACC_CAPABILITY_KEY client_keys[2],preference_keys[2];
    PAPACC_CAPABILITY_SET client=PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET preference=PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_EVALUATION_INPUTS inputs=p->capability_inputs;
    PAPACC_FRAME_HEADER header;PAPACC_RESULT r;
    if(p->capability_store==NULL||papacc_session_capability_store_find(
       p->capability_store,p->session_instance_id)!=NULL)return PAPACC_RESULT_INVALID_STATE;
    r=papacc_capability_set_init(&client,client_keys,2U);
    if(r==PAPACC_RESULT_OK)r=papacc_capability_set_init(&preference,preference_keys,2U);
    if(r==PAPACC_RESULT_OK)r=papacc_capability_negotiate_decode(p->capability_payload,
       p->capability_payload_received,&client,&preference);
    inputs.client_supported=&client;inputs.client_preference=&preference;
    if(r==PAPACC_RESULT_OK&&p->capability_test_gate_fn!=NULL)
        r=p->capability_test_gate_fn(p->capability_test_gate_context,
            &client,&preference);
    if(r==PAPACC_RESULT_OK)r=papacc_session_capability_store_publish(
       p->capability_store,p->session_instance_id,&inputs);
    if(r==PAPACC_RESULT_OK)r=papacc_capability_negotiate_result_encode(
       &papacc_session_capability_store_find(p->capability_store,
       p->session_instance_id)->effective,p->capability_result,
       sizeof(p->capability_result),&p->capability_result_length);
    if(r!=PAPACC_RESULT_OK){if(papacc_session_capability_store_find(p->capability_store,
       p->session_instance_id)!=NULL)(void)papacc_session_capability_store_remove(
       p->capability_store,p->session_instance_id);return r;}
    header=papacc_capability_negotiate_result_frame_header(
       (PAPACC_U32)p->capability_result_length);
    r=papacc_framed_writer_begin_frame(&p->writer,&header);
    if(r!=PAPACC_RESULT_OK){(void)papacc_session_capability_store_remove(
       p->capability_store,p->session_instance_id);return r;}
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    p->capability_result_written=PAPACC_FALSE;
#endif
    p->capability_result_offset=0U;
    p->state=PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_CAPABILITIES;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_post_control_prepare_ticket(
    PAPACC_POST_CONTROL_PROCESSOR *processor, PAPACC_U64 now_ns)
{
    PAPACC_FRAME_HEADER header;
    PAPACC_SIZE written = 0;
    PAPACC_U64 deadline_ns = 0;
    PAPACC_RESULT result;
    if (processor->issue_gate_fn != NULL)
        result = processor->issue_gate_fn(processor->issue_gate_context,
            processor->session_instance_id, now_ns,
            &processor->pending_ticket, &deadline_ns);
    else
        result = papacc_data_association_manager_issue(
            processor->association_manager, processor->session_instance_id,
            now_ns, &processor->pending_ticket, &deadline_ns);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    result = papacc_data_ticket_encode(
        &processor->pending_ticket, processor->ticket_payload,
        sizeof(processor->ticket_payload), &written);
    if (result == PAPACC_RESULT_OK &&
        written != PAPACC_DATA_ASSOCIATION_TICKET_SIZE)
        result = PAPACC_RESULT_INTERNAL_ERROR;
    header = papacc_data_ticket_frame_header();
    if (result == PAPACC_RESULT_OK)
        result = papacc_framed_writer_begin_frame(&processor->writer, &header);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    processor->ticket_payload_offset = 0;
    processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_set_ticket_issue_gate(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_DATA_TICKET_ISSUE_GATE_FN issue_gate_fn, void *issue_gate_context)
{
    if (processor == NULL || issue_gate_fn == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (processor->state != PAPACC_POST_CONTROL_PROCESSOR_STATE_READY ||
        processor->issue_gate_fn != NULL)
        return PAPACC_RESULT_INVALID_STATE;
    processor->issue_gate_fn = issue_gate_fn;
    processor->issue_gate_context = issue_gate_context;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_read_once(
    PAPACC_POST_CONTROL_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_POST_CONTROL_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_READER_STATUS reader_status;
    PAPACC_FRAME_PARSER_EVENT event;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_post_control_processor_wants_read(processor) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_framed_reader_next(
        &processor->reader, &reader_status, &event);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    if (reader_status == PAPACC_FRAMED_READER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (reader_status == PAPACC_FRAMED_READER_STATUS_END_OF_STREAM) {
        result = papacc_post_control_close(processor);
        if (result != PAPACC_RESULT_OK) return result;
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (reader_status != PAPACC_FRAMED_READER_STATUS_EVENT) {
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
#if defined(PAPACC_ENABLE_C2_DIAGNOSTIC) || defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE)
    if(event.header.message_type==16U||event.header.message_type==20U||event.header.message_type==26U){
        PAPACC_SIZE maximum=event.header.message_type==16U?803U:event.header.message_type==20U?32U:40U;
        if(processor->c2_control==NULL||event.header.payload_length>maximum||event.header.payload_length<(event.header.message_type==16U?42U:maximum))return papacc_post_control_fail(processor,PAPACC_RESULT_PROTOCOL_ERROR);
        result=processor->c2_control(processor->c2_context,processor->session_instance_id,event.header.message_type,NULL,event.header.payload_length,PAPACC_TRUE);
        if(result!=0)return papacc_post_control_fail(processor,result);
        if(event.payload_length>sizeof(processor->c2_payload)-processor->c2_received)return papacc_post_control_fail(processor,PAPACC_RESULT_PROTOCOL_ERROR);
        if(event.payload_length){memcpy(processor->c2_payload+processor->c2_received,event.payload,event.payload_length);processor->c2_received+=event.payload_length;}
        if(event.type==PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE){
            result=processor->c2_control(processor->c2_context,processor->session_instance_id,event.header.message_type,processor->c2_payload,processor->c2_received,PAPACC_FALSE);
            {volatile PAPACC_U8*erase=processor->c2_payload;PAPACC_SIZE count=processor->c2_received;while(count--!=0U)*erase++=0;}
            processor->c2_received=0U;if(result!=0)return papacc_post_control_fail(processor,result);
        }
        *out_status=PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;return PAPACC_RESULT_OK;
    }
#endif
    if(event.header.message_type==PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE){
        if(processor->capability_store==NULL||event.header.payload_length>
           sizeof(processor->capability_payload)||processor->capability_payload_received+
           event.payload_length>sizeof(processor->capability_payload))
            return papacc_post_control_fail(processor,PAPACC_RESULT_PROTOCOL_ERROR);
        if(event.payload_length!=0U){memcpy(&processor->capability_payload[
           processor->capability_payload_received],event.payload,event.payload_length);
           processor->capability_payload_received+=event.payload_length;}
        if(event.type!=PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE){*out_status=
           PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;return PAPACC_RESULT_OK;}
        result=papacc_post_control_prepare_capabilities(processor);
        processor->capability_payload_received=0U;
        if(result!=PAPACC_RESULT_OK)return papacc_post_control_fail(processor,result);
        *out_status=PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;return PAPACC_RESULT_OK;
    }
    if (event.header.message_type != PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST ||
        event.header.payload_length != 0 || event.type !=
        PAPACC_FRAME_PARSER_EVENT_FRAME_COMPLETE)
        return papacc_post_control_fail(processor,PAPACC_RESULT_PROTOCOL_ERROR);
    result = papacc_post_control_prepare_ticket(processor, now_ns);
    if (result != PAPACC_RESULT_OK) return result;
    *out_status = PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_post_control_processor_write_once(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_POST_CONTROL_STEP_STATUS *out_status)
{
    PAPACC_FRAMED_WRITER_STATUS writer_status;
    PAPACC_SIZE consumed = 0;
    PAPACC_SIZE remaining;
    PAPACC_RESULT result;
    if (out_status != NULL)
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_UNSPECIFIED;
    if (processor == NULL || out_status == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_post_control_processor_wants_write(processor) != PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    if(processor->state==PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_CAPABILITIES){
        remaining=processor->capability_result_length-processor->capability_result_offset;
        result=papacc_framed_writer_step(&processor->writer,
          &processor->capability_result[processor->capability_result_offset],remaining,
          &consumed,&writer_status);
        if(result!=PAPACC_RESULT_OK)return papacc_post_control_fail(processor,result);
        processor->capability_result_offset+=consumed;
        if(writer_status==PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE){
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
        processor->capability_result_written=PAPACC_TRUE;
#endif
        processor->state=
          PAPACC_POST_CONTROL_PROCESSOR_STATE_READY;*out_status=
          PAPACC_POST_CONTROL_STEP_STATUS_RESPONSE_COMPLETE;return PAPACC_RESULT_OK;}
        *out_status=writer_status==PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK?
          PAPACC_POST_CONTROL_STEP_STATUS_WOULD_BLOCK:PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;
        return PAPACC_RESULT_OK;
    }
    remaining = PAPACC_DATA_ASSOCIATION_TICKET_SIZE - processor->ticket_payload_offset;
    result = papacc_framed_writer_step(
        &processor->writer,
        &processor->ticket_payload[processor->ticket_payload_offset],
        remaining, &consumed, &writer_status);
    if (result != PAPACC_RESULT_OK)
        return papacc_post_control_fail(processor, result);
    if (consumed > remaining)
        return papacc_post_control_fail(
            processor, PAPACC_RESULT_INTERNAL_ERROR);
    processor->ticket_payload_offset += consumed;
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_WOULD_BLOCK) {
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_END_OF_STREAM) {
        result = papacc_post_control_close(processor);
        if (result != PAPACC_RESULT_OK) return result;
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_CLOSED;
        return PAPACC_RESULT_OK;
    }
    if (writer_status == PAPACC_FRAMED_WRITER_STATUS_FRAME_COMPLETE) {
        if (processor->ticket_payload_offset !=
            PAPACC_DATA_ASSOCIATION_TICKET_SIZE)
            return papacc_post_control_fail(
                processor, PAPACC_RESULT_INTERNAL_ERROR);
        processor->state = PAPACC_POST_CONTROL_PROCESSOR_STATE_READY;
        *out_status = PAPACC_POST_CONTROL_STEP_STATUS_RESPONSE_COMPLETE;
        return PAPACC_RESULT_OK;
    }
    *out_status = PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

void papacc_post_control_processor_shutdown(
    PAPACC_POST_CONTROL_PROCESSOR *processor)
{
    if (processor == NULL || processor->state ==
        PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED)
        return;
    papacc_post_control_close_scope(processor);
    papacc_framed_reader_shutdown(&processor->reader);
    papacc_framed_writer_shutdown(&processor->writer);
    *processor = (PAPACC_POST_CONTROL_PROCESSOR)
        PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
}

#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
/* Complete RESULT is an immutable write fact. A subsequent ticket writer is
 * not revocation. This predicate alone never grants Session/DATA authority. */
PAPACC_BOOL papacc_post_control_processor_c8_result_complete(const PAPACC_POST_CONTROL_PROCESSOR *p)
{
 if(!p||p->close_dispatched||!p->capability_result_written||
    !p->capability_result_length||p->capability_result_offset!=p->capability_result_length)return PAPACC_FALSE;
 return p->state==PAPACC_POST_CONTROL_PROCESSOR_STATE_READY||
  p->state==PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET?PAPACC_TRUE:PAPACC_FALSE;
}
#endif
