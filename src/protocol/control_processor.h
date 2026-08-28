#ifndef PAPACC_CONTROL_PROCESSOR_H
#define PAPACC_CONTROL_PROCESSOR_H

#include "channel.h"
#include "control_protocol.h"
#include "framed_reader.h"
#include "framed_writer.h"

typedef enum PAPACC_CONTROL_PROCESSOR_STATE {
    PAPACC_CONTROL_PROCESSOR_STATE_UNINITIALIZED = 0,
    PAPACC_CONTROL_PROCESSOR_STATE_WAITING_CONTROL_OPEN_HEADER = 1,
    PAPACC_CONTROL_PROCESSOR_STATE_READING_CONTROL_OPEN = 2,
    PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT = 3,
    PAPACC_CONTROL_PROCESSOR_STATE_ESTABLISHED = 4,
    PAPACC_CONTROL_PROCESSOR_STATE_CLOSED = 5,
    PAPACC_CONTROL_PROCESSOR_STATE_ERROR = 6
} PAPACC_CONTROL_PROCESSOR_STATE;

typedef enum PAPACC_CONTROL_PROCESSOR_STEP_STATUS {
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS_UNSPECIFIED = 0,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS_PROGRESS = 1,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS_WOULD_BLOCK = 2,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS_ESTABLISHED = 3,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED = 4
} PAPACC_CONTROL_PROCESSOR_STEP_STATUS;

typedef struct PAPACC_CONTROL_PROCESSOR {
    PAPACC_CONNECTION_MANAGER *connection_manager;
    PAPACC_SESSION_MANAGER *session_manager;
    PAPACC_CHANNEL_MANAGER *channel_manager;
    PAPACC_CONNECTION *connection;
    PAPACC_FRAMED_READER reader;
    PAPACC_FRAMED_WRITER writer;
    PAPACC_U8 open_payload[4];
    PAPACC_SIZE open_payload_received;
    PAPACC_U8 accept_payload[4];
    PAPACC_SIZE accept_payload_offset;
    PAPACC_U64 session_instance_id;
    PAPACC_U64 control_channel_instance_id;
    PAPACC_U64 establishment_deadline_ns;
    PAPACC_CONTROL_PROCESSOR_STATE state;
} PAPACC_CONTROL_PROCESSOR;

#define PAPACC_CONTROL_PROCESSOR_INITIALIZER \
    { NULL, NULL, NULL, NULL, PAPACC_FRAMED_READER_INITIALIZER, \
      PAPACC_FRAMED_WRITER_INITIALIZER, { 0 }, 0, { 0 }, 0, 0, 0, 0, \
      PAPACC_CONTROL_PROCESSOR_STATE_UNINITIALIZED }

PAPACC_RESULT papacc_control_processor_init(
    PAPACC_CONTROL_PROCESSOR *processor,
    PAPACC_CONNECTION_MANAGER *connection_manager,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_U64 connection_instance_id, PAPACC_U8 *scratch,
    PAPACC_SIZE scratch_capacity, PAPACC_U32 max_payload_length,
    PAPACC_U64 establishment_deadline_ns);
PAPACC_BOOL papacc_control_processor_wants_read(
    const PAPACC_CONTROL_PROCESSOR *processor);
PAPACC_BOOL papacc_control_processor_wants_write(
    const PAPACC_CONTROL_PROCESSOR *processor);
PAPACC_BOOL papacc_control_processor_is_established(
    const PAPACC_CONTROL_PROCESSOR *processor);
PAPACC_RESULT papacc_control_processor_read_once(
    PAPACC_CONTROL_PROCESSOR *processor,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS *out_status);
PAPACC_RESULT papacc_control_processor_write_once(
    PAPACC_CONTROL_PROCESSOR *processor,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS *out_status);
PAPACC_RESULT papacc_control_processor_check_deadline(
    PAPACC_CONTROL_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS *out_status);
void papacc_control_processor_shutdown(PAPACC_CONTROL_PROCESSOR *processor);

#endif
