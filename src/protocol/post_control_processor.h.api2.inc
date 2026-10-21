#ifndef PAPACC_POST_CONTROL_PROCESSOR_H
#define PAPACC_POST_CONTROL_PROCESSOR_H

#include "connection.h"
#include "data_association.h"
#include "framed_reader.h"
#include "framed_writer.h"
#include "capability_negotiation_protocol.h"
#include "session_capability_store.h"

typedef enum PAPACC_POST_CONTROL_PROCESSOR_STATE {
    PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED = 0,
    PAPACC_POST_CONTROL_PROCESSOR_STATE_READY = 1,
    PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET = 2,
    PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_CAPABILITIES = 5,
    PAPACC_POST_CONTROL_PROCESSOR_STATE_CLOSED = 3,
    PAPACC_POST_CONTROL_PROCESSOR_STATE_ERROR = 4
} PAPACC_POST_CONTROL_PROCESSOR_STATE;

typedef enum PAPACC_POST_CONTROL_STEP_STATUS {
    PAPACC_POST_CONTROL_STEP_STATUS_UNSPECIFIED = 0,
    PAPACC_POST_CONTROL_STEP_STATUS_PROGRESS = 1,
    PAPACC_POST_CONTROL_STEP_STATUS_WOULD_BLOCK = 2,
    PAPACC_POST_CONTROL_STEP_STATUS_RESPONSE_COMPLETE = 3,
    PAPACC_POST_CONTROL_STEP_STATUS_CLOSED = 4
} PAPACC_POST_CONTROL_STEP_STATUS;

typedef enum PAPACC_CONTROL_CLOSE_ORIGIN {
    PAPACC_CONTROL_CLOSE_ORIGIN_NONE = 0,
    PAPACC_CONTROL_CLOSE_ORIGIN_NORMAL,
    PAPACC_CONTROL_CLOSE_ORIGIN_PACC_PROTOCOL_FAILURE,
    PAPACC_CONTROL_CLOSE_ORIGIN_OWNER_FAILURE
} PAPACC_CONTROL_CLOSE_ORIGIN;

typedef struct PAPACC_CONTROL_CLOSE_RESULT {
    PAPACC_CONTROL_CLOSE_ORIGIN origin;
    PAPACC_RESULT pacc_result;
    PAPACC_U64 session_instance_id;
    PAPACC_U64 control_channel_instance_id;
} PAPACC_CONTROL_CLOSE_RESULT;

#define PAPACC_CONTROL_CLOSE_RESULT_INITIALIZER \
    { PAPACC_CONTROL_CLOSE_ORIGIN_NONE, PAPACC_RESULT_OK, 0U, 0U }

typedef PAPACC_RESULT (*PAPACC_CONTROL_CLOSE_FN)(void *,
    const PAPACC_CONTROL_CLOSE_RESULT *);

typedef PAPACC_RESULT (*PAPACC_DATA_TICKET_ISSUE_GATE_FN)(void *context,
    PAPACC_U64 session_instance_id, PAPACC_U64 now_ns,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket, PAPACC_U64 *out_deadline_ns);

typedef PAPACC_RESULT (*PAPACC_CAPABILITY_PROCESSING_TEST_GATE_FN)(
    void *context, const PAPACC_CAPABILITY_SET *client_supported,
    const PAPACC_CAPABILITY_SET *client_preference);

typedef struct PAPACC_POST_CONTROL_PROCESSOR {
    PAPACC_CONNECTION_MANAGER *connection_manager;
    PAPACC_SESSION_MANAGER *session_manager;
    PAPACC_CHANNEL_MANAGER *channel_manager;
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager;
    PAPACC_CONNECTION *connection;
    PAPACC_FRAMED_READER reader;
    PAPACC_FRAMED_WRITER writer;
    PAPACC_DATA_ASSOCIATION_TICKET pending_ticket;
    PAPACC_U8 ticket_payload[PAPACC_DATA_ASSOCIATION_TICKET_SIZE];
    PAPACC_SIZE ticket_payload_offset;
    PAPACC_U8 capability_payload[28U];
    PAPACC_SIZE capability_payload_received;
    PAPACC_U8 capability_result[16U];
    PAPACC_SIZE capability_result_length;
    PAPACC_SIZE capability_result_offset;
    PAPACC_SESSION_CAPABILITY_STORE *capability_store;
    PAPACC_CAPABILITY_EVALUATION_INPUTS capability_inputs;
    PAPACC_U64 session_instance_id;
    PAPACC_U64 control_channel_instance_id;
    PAPACC_DATA_TICKET_ISSUE_GATE_FN issue_gate_fn;
    void *issue_gate_context;
    PAPACC_CAPABILITY_PROCESSING_TEST_GATE_FN capability_test_gate_fn;
    void *capability_test_gate_context;
    PAPACC_CONTROL_CLOSE_FN close_fn;
    void *close_context;
    PAPACC_CONTROL_CLOSE_RESULT close_result;
    PAPACC_BOOL close_dispatched;
    PAPACC_POST_CONTROL_PROCESSOR_STATE state;
} PAPACC_POST_CONTROL_PROCESSOR;

#define PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER \
    { NULL, NULL, NULL, NULL, NULL, PAPACC_FRAMED_READER_INITIALIZER, \
      PAPACC_FRAMED_WRITER_INITIALIZER, \
      PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER, { 0 }, 0, {0},0,{0},0,0,NULL,{NULL,NULL,NULL,NULL,NULL,NULL,NULL},0,0,NULL,NULL,NULL,NULL,NULL,NULL,PAPACC_CONTROL_CLOSE_RESULT_INITIALIZER,PAPACC_FALSE, \
      PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED }

PAPACC_RESULT papacc_post_control_processor_init(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 session_instance_id,
    PAPACC_U64 control_channel_instance_id,
    PAPACC_U8 *read_scratch, PAPACC_SIZE read_scratch_capacity);
PAPACC_RESULT papacc_post_control_processor_init_from_reader(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 session_instance_id,
    PAPACC_U64 control_channel_instance_id,
    PAPACC_FRAMED_READER *reader);
PAPACC_RESULT papacc_post_control_processor_set_ticket_issue_gate(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_DATA_TICKET_ISSUE_GATE_FN issue_gate_fn, void *issue_gate_context);
PAPACC_RESULT papacc_post_control_processor_set_capability_gate(
    PAPACC_POST_CONTROL_PROCESSOR *, PAPACC_SESSION_CAPABILITY_STORE *,
    const PAPACC_CAPABILITY_EVALUATION_INPUTS *);
PAPACC_RESULT papacc_post_control_processor_set_capability_test_gate(
    PAPACC_POST_CONTROL_PROCESSOR *,
    PAPACC_CAPABILITY_PROCESSING_TEST_GATE_FN, void *);
PAPACC_RESULT papacc_post_control_processor_set_close_handler(
    PAPACC_POST_CONTROL_PROCESSOR *, PAPACC_CONTROL_CLOSE_FN, void *);
PAPACC_BOOL papacc_post_control_processor_wants_read(
    const PAPACC_POST_CONTROL_PROCESSOR *processor);
PAPACC_BOOL papacc_post_control_processor_wants_write(
    const PAPACC_POST_CONTROL_PROCESSOR *processor);
PAPACC_RESULT papacc_post_control_processor_read_once(
    PAPACC_POST_CONTROL_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_POST_CONTROL_STEP_STATUS *out_status);
PAPACC_RESULT papacc_post_control_processor_write_once(
    PAPACC_POST_CONTROL_PROCESSOR *processor,
    PAPACC_POST_CONTROL_STEP_STATUS *out_status);
void papacc_post_control_processor_shutdown(
    PAPACC_POST_CONTROL_PROCESSOR *processor);

#endif
