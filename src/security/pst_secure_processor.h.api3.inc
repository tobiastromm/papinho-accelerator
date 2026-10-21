#ifndef PAPACC_PST_SECURE_PROCESSOR_H
#define PAPACC_PST_SECURE_PROCESSOR_H

#include "papacc/types.h"
#include "papinho_secure_transport.h"

typedef enum PAPACC_PST_SECURE_PROCESSOR_STATE {
    PAPACC_PST_SECURE_PROCESSOR_UNINITIALIZED = 0,
    PAPACC_PST_SECURE_PROCESSOR_CREATED,
    PAPACC_PST_SECURE_PROCESSOR_ATTACHED,
    PAPACC_PST_SECURE_PROCESSOR_HANDSHAKING,
    PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED,
    PAPACC_PST_SECURE_PROCESSOR_PEER_CLOSED,
    PAPACC_PST_SECURE_PROCESSOR_SHUTTING_DOWN,
    PAPACC_PST_SECURE_PROCESSOR_CLOSED,
    PAPACC_PST_SECURE_PROCESSOR_FAILED,
    PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL,
    PAPACC_PST_SECURE_PROCESSOR_RELEASE_PENDING
} PAPACC_PST_SECURE_PROCESSOR_STATE;

typedef enum PAPACC_PST_SECURE_STEP {
    PAPACC_PST_SECURE_STEP_COMPLETE = 0,
    PAPACC_PST_SECURE_STEP_NEED_READ,
    PAPACC_PST_SECURE_STEP_NEED_WRITE,
    PAPACC_PST_SECURE_STEP_NEED_READ_WRITE,
    PAPACC_PST_SECURE_STEP_CLOSED,
    PAPACC_PST_SECURE_STEP_FAILED
} PAPACC_PST_SECURE_STEP;

typedef struct PAPACC_PST_SECURE_PROCESSOR {
    pst_connection *connection;
    PAPACC_PST_SECURE_PROCESSOR_STATE state;
    PAPACC_U64 deadline_ns;
    PST_RESULT source_result;
    pst_u32 last_close_kind,last_io_operation;
    PST_RELEASE_INFO release_info;
    PAPACC_U64 e10_release_calls,e10_release_returns;
    PAPACC_BOOL e10_release_overflow,e10_release_synthetic;
} PAPACC_PST_SECURE_PROCESSOR;

#define PAPACC_PST_SECURE_PROCESSOR_INITIALIZER \
    { NULL, PAPACC_PST_SECURE_PROCESSOR_UNINITIALIZED, 0U, PST_RESULT_OK, 0U, 0U, {0}, 0U, 0U, PAPACC_FALSE, PAPACC_FALSE }

PAPACC_RESULT papacc_pst_secure_processor_init(
    PAPACC_PST_SECURE_PROCESSOR *processor, pst_runtime *runtime,
    const PST_CONNECTION_CONFIG *config, PAPACC_U64 deadline_ns);

PAPACC_RESULT papacc_pst_secure_processor_attach(
    PAPACC_PST_SECURE_PROCESSOR *processor, pst_transport *transport,
    PAPACC_BOOL *ownership_accepted);

PAPACC_RESULT papacc_pst_secure_processor_interest(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_BOOL *read_interest,
    PAPACC_BOOL *write_interest);

PAPACC_RESULT papacc_pst_secure_processor_handshake_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_PST_SECURE_STEP *step);

PAPACC_RESULT papacc_pst_secure_processor_read_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_U8 *buffer,
    PAPACC_SIZE capacity, PAPACC_SIZE *transferred,
    PAPACC_PST_SECURE_STEP *step);

PAPACC_RESULT papacc_pst_secure_processor_write_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, const PAPACC_U8 *buffer,
    PAPACC_SIZE length, PAPACC_SIZE *transferred,
    PAPACC_PST_SECURE_STEP *step);

PAPACC_RESULT papacc_pst_secure_processor_shutdown_once(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_PST_SECURE_STEP *step);

PAPACC_RESULT papacc_pst_secure_processor_check_deadline(
    PAPACC_PST_SECURE_PROCESSOR *processor, PAPACC_U64 now_ns,
    PAPACC_BOOL *expired);

PAPACC_RESULT papacc_pst_secure_processor_release(
    PAPACC_PST_SECURE_PROCESSOR *processor);

PAPACC_RESULT papacc_pst_secure_processor_query_release(
    const PAPACC_PST_SECURE_PROCESSOR *, PST_RELEASE_INFO *);

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
void papacc_pst_secure_processor_test_release_fault(pst_connection *, pst_u32);
#endif

#endif
