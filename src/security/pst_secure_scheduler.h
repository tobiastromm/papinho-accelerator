#ifndef PAPACC_PST_SECURE_SCHEDULER_H
#define PAPACC_PST_SECURE_SCHEDULER_H

#include "papacc/types.h"
#include "papinho_secure_transport.h"

typedef enum PAPACC_PST_SCHEDULER_MEMBER_KIND {
    PAPACC_PST_SCHEDULER_MEMBER_NONE = 0,
    PAPACC_PST_SCHEDULER_MEMBER_CONNECTION,
    PAPACC_PST_SCHEDULER_MEMBER_EXTERNAL
} PAPACC_PST_SCHEDULER_MEMBER_KIND;

typedef enum PAPACC_PST_WAIT_OUTCOME {
    PAPACC_PST_WAIT_READY = 0,
    PAPACC_PST_WAIT_TIMEOUT,
    PAPACC_PST_WAIT_WOKEN
} PAPACC_PST_WAIT_OUTCOME;

typedef struct PAPACC_PST_SCHEDULER_MEMBER {
    PAPACC_BOOL registered;
    PAPACC_PST_SCHEDULER_MEMBER_KIND kind;
    pst_wait_token token;
    union {
        pst_connection *connection;
        pst_external_source *external_source;
    } source;
} PAPACC_PST_SCHEDULER_MEMBER;

#define PAPACC_PST_SCHEDULER_MEMBER_INITIALIZER \
    { PAPACC_FALSE, PAPACC_PST_SCHEDULER_MEMBER_NONE, 0U, { NULL } }

typedef struct PAPACC_PST_READY_EVENT {
    pst_wait_token token;
    PAPACC_PST_SCHEDULER_MEMBER_KIND kind;
    PAPACC_BOOL read_ready;
    PAPACC_BOOL write_ready;
    PAPACC_BOOL terminal;
    PAPACC_BOOL external;
    PST_RESULT source_result;
} PAPACC_PST_READY_EVENT;

typedef void (*PAPACC_PST_READY_FN)(void *context,
    const PAPACC_PST_READY_EVENT *event);

typedef struct PAPACC_PST_SECURE_SCHEDULER {
    pst_wait_set *wait_set;
    PAPACC_PST_SCHEDULER_MEMBER *members;
    PAPACC_SIZE member_capacity;
    PAPACC_SIZE member_count;
    PST_WAIT_EVENT *events;
    PAPACC_SIZE event_capacity;
    PAPACC_SIZE event_count;
    PAPACC_SIZE next_member_index;
    pst_wait_token next_token;
    PAPACC_BOOL stopping;
    PAPACC_BOOL initialized;
} PAPACC_PST_SECURE_SCHEDULER;

#define PAPACC_PST_SECURE_SCHEDULER_INITIALIZER \
    { NULL, NULL, 0U, 0U, NULL, 0U, 0U, 0U, 1U, PAPACC_FALSE, PAPACC_FALSE }

PAPACC_RESULT papacc_pst_secure_scheduler_init(
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    PAPACC_PST_SCHEDULER_MEMBER *members, PAPACC_SIZE member_capacity,
    PST_WAIT_EVENT *events, PAPACC_SIZE event_capacity);

PAPACC_RESULT papacc_pst_secure_scheduler_add_connection(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, pst_connection *connection,
    pst_wait_token *out_token);

PAPACC_RESULT papacc_pst_secure_scheduler_add_external(
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    pst_external_source *external_source, PAPACC_BOOL read_interest,
    PAPACC_BOOL write_interest, pst_wait_token *out_token);

PAPACC_RESULT papacc_pst_secure_scheduler_remove_connection(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, pst_connection *connection);

PAPACC_RESULT papacc_pst_secure_scheduler_remove_external(
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    pst_external_source *external_source);

PAPACC_RESULT papacc_pst_secure_scheduler_wait(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, PAPACC_U32 timeout_ms,
    PAPACC_PST_WAIT_OUTCOME *outcome, PAPACC_SIZE *out_ready_count);

PAPACC_RESULT papacc_pst_secure_scheduler_dispatch_ready(
    PAPACC_PST_SECURE_SCHEDULER *scheduler, PAPACC_PST_READY_FN callback,
    void *callback_context, PAPACC_SIZE *out_dispatched);

PAPACC_RESULT papacc_pst_secure_scheduler_request_stop(
    PAPACC_PST_SECURE_SCHEDULER *scheduler);

PAPACC_RESULT papacc_pst_secure_scheduler_wake(
    PAPACC_PST_SECURE_SCHEDULER *scheduler);

void papacc_pst_secure_scheduler_release(
    PAPACC_PST_SECURE_SCHEDULER *scheduler);

#endif
