#include "server_secure_io_win32.h"

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
#include <windows.h>
#include <stdio.h>
PAPACC_RESULT papacc_server_secure_io_win32_test_candidate_close(
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *, PAPACC_BOOL *);
static int p11_slot_guard(void)
{
    PAPACC_SERVER_SECURE_IO_WIN32 owner =
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidate =
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER;
    PAPACC_SERVER_IO_LOOP_WIN32 loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 slot =
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_BOOL closed = PAPACC_FALSE;
    PAPACC_RESULT result;
    owner.owner_thread_id = (PAPACC_U32)GetCurrentThreadId();
    owner.protocol_loop = &loop;
    owner.candidates = &candidate;
    owner.candidate_capacity = 1U;
    loop.processor_slots = &slot;
    loop.processor_capacity = 1U;
    loop.connection_manager = &manager;
    loop.initialized = PAPACC_TRUE;
    manager.initialized = PAPACC_TRUE;
    candidate.owner = &owner;
    candidate.state = PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL;
    candidate.connection_instance_id = 7U;
    candidate.close_requested = PAPACC_TRUE;
    candidate.transport_adapter.closed = PAPACC_TRUE;
    slot.in_use = PAPACC_TRUE;
    slot.connection_instance_id = 7U;
    result = papacc_server_secure_io_win32_test_candidate_close(
        &candidate, &closed);
    fprintf(stderr, "P11_SLOT_PRESENT result=%d closed=%d state=%d connection=7 slot=7 acquired_native=0\n",
        (int)result, (int)closed, (int)candidate.state);
    if (closed == PAPACC_TRUE ||
        candidate.state == PAPACC_SERVER_SECURE_CANDIDATE_CLOSED)
        return 101;
    slot.in_use = PAPACC_FALSE;
    result = papacc_server_secure_io_win32_test_candidate_close(
        &candidate, &closed);
    if (result != PAPACC_RESULT_OK || closed != PAPACC_TRUE ||
        candidate.state != PAPACC_SERVER_SECURE_CANDIDATE_CLOSED)
        return 102;
    fprintf(stderr, "P11_SLOT_ABSENT=PASS acquired_native=0\n");
    return 0;
}
static PAPACC_U32 p11_close_count;
static void p11_observe_close(void *context)
{
    (void)context;
    ++p11_close_count;
}
static PAPACC_RESULT p11_read(void *context, PAPACC_U8 *buffer,
    PAPACC_SIZE size, PAPACC_SIZE *transferred,
    PAPACC_TRANSPORT_IO_STATUS *status)
{
    (void)context; (void)buffer; (void)size;
    *transferred = 0U; *status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}
static PAPACC_RESULT p11_write(void *context, const PAPACC_U8 *buffer,
    PAPACC_SIZE size, PAPACC_SIZE *transferred,
    PAPACC_TRANSPORT_IO_STATUS *status)
{
    (void)context; (void)buffer; (void)size;
    *transferred = 0U; *status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}
typedef struct P11_WORKER {
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *candidate;
    PAPACC_RESULT close_result;
    PAPACC_RESULT stop_result;
    PAPACC_BOOL closed;
} P11_WORKER;
static DWORD WINAPI p11_non_owner(LPVOID context)
{
    P11_WORKER *worker = (P11_WORKER *)context;
    worker->close_result = papacc_server_secure_io_win32_test_candidate_close(
        worker->candidate, &worker->closed);
    worker->stop_result = papacc_server_secure_io_win32_request_stop(
        worker->candidate->owner);
    return 0U;
}
static int p11_wrong_connection(void)
{
    PAPACC_SERVER_SECURE_IO_WIN32 owner =
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidates[2] = {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER,
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER };
    PAPACC_SERVER_IO_LOOP_WIN32 loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_CONNECTION_MANAGER manager =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION connection = PAPACC_CONNECTION_INITIALIZER;
    PAPACC_BOOL closed = PAPACC_FALSE;
    PAPACC_RESULT result;
    owner.owner_thread_id = (PAPACC_U32)GetCurrentThreadId();
    owner.protocol_loop = &loop;
    owner.candidates = candidates;
    owner.candidate_capacity = 2U;
    loop.connection_manager = &manager;
    manager.initialized = PAPACC_TRUE;
    manager.storage = &connection;
    manager.capacity = 1U;
    manager.count = 1U;
    connection.connection_instance_id = 8U;
    connection.state = PAPACC_CONNECTION_STATE_ASSOCIATED;
    connection.transport.context = &candidates[0].transport_adapter;
    connection.transport.read_fn = p11_read;
    connection.transport.write_fn = p11_write;
    connection.transport.close_fn = p11_observe_close;
    candidates[0].owner = &owner;
    candidates[0].state = PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL;
    candidates[0].connection_instance_id = 8U;
    candidates[1].owner = &owner;
    candidates[1].state = PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL;
    candidates[1].connection_instance_id = 8U;
    p11_close_count = 0U;
    result = papacc_server_secure_io_win32_test_candidate_close(
        &candidates[1], &closed);
    fprintf(stderr, "P11_WRONG_CANDIDATE result=%d closed=%d connection_state=%d root_calls=%u acquired_native=0\n",
        (int)result, (int)closed, (int)connection.state,
        (unsigned)p11_close_count);
    if (result != PAPACC_RESULT_INVALID_STATE || closed != PAPACC_FALSE ||
        p11_close_count != 0U ||
        connection.state != PAPACC_CONNECTION_STATE_ASSOCIATED)
        return 201;
    {
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 duplicate = candidates[1];
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32 before;
        P11_WORKER worker;
        HANDLE thread;
        result = papacc_server_secure_io_win32_test_candidate_close(
            &duplicate, &closed);
        if (result != PAPACC_RESULT_INVALID_STATE || closed != PAPACC_FALSE)
            return 203;
        candidates[1].connection_instance_id = 0U;
        result = papacc_server_secure_io_win32_test_candidate_close(
            &candidates[1], &closed);
        if (result != PAPACC_RESULT_INVALID_STATE || closed != PAPACC_FALSE)
            return 204;
        candidates[1].connection_instance_id = 7U;
        candidates[1].transport_adapter.registered = PAPACC_TRUE;
        result = papacc_server_secure_io_win32_test_candidate_close(
            &candidates[1], &closed);
        if (result != PAPACC_RESULT_INVALID_STATE || closed != PAPACC_FALSE)
            return 205;
        candidates[1].transport_adapter.registered = PAPACC_FALSE;
        candidates[1].state = PAPACC_SERVER_SECURE_CANDIDATE_FAILED;
        result = papacc_server_secure_io_win32_test_candidate_close(
            &candidates[1], &closed);
        if (result != PAPACC_RESULT_INVALID_STATE || closed != PAPACC_FALSE)
            return 206;
        candidates[1].state = PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL;
        candidates[1].transport_adapter.closed = PAPACC_TRUE;
        owner.dispatch_active = PAPACC_TRUE;
        result = papacc_server_secure_io_win32_test_candidate_close(
            &candidates[1], &closed);
        if (result != PAPACC_RESULT_OK || closed != PAPACC_FALSE ||
            candidates[1].state != PAPACC_SERVER_SECURE_CANDIDATE_PROTOCOL)
            return 207;
        owner.dispatch_active = PAPACC_FALSE;
        owner.stop_state = PAPACC_SERVER_SECURE_STOP_REMOVE_PENDING;
        owner.stop_last_remove_result = PAPACC_RESULT_INTERNAL_ERROR;
        owner.shutdown_deadline_ns = 123456U;
        result = papacc_server_secure_io_win32_test_candidate_close(
            &candidates[1], &closed);
        if (result != PAPACC_RESULT_INTERNAL_ERROR || closed != PAPACC_FALSE ||
            owner.shutdown_deadline_ns != 123456U)
            return 208;
        owner.stop_state = PAPACC_SERVER_SECURE_STOP_RUNNING;
        owner.initialized = PAPACC_TRUE;
        before = candidates[1];
        worker.candidate = &candidates[1];
        worker.close_result = worker.stop_result = PAPACC_RESULT_OK;
        worker.closed = PAPACC_FALSE;
        thread = CreateThread(NULL, 0U, p11_non_owner, &worker, 0U, NULL);
        if (thread == NULL) return 209;
        if (WaitForSingleObject(thread, 3000U) != WAIT_OBJECT_0) return 210;
        CloseHandle(thread);
        if (worker.close_result != PAPACC_RESULT_INVALID_STATE ||
            worker.stop_result != PAPACC_RESULT_INVALID_STATE ||
            worker.closed != PAPACC_FALSE ||
            memcmp(&before, &candidates[1], sizeof(before)) != 0 ||
            owner.stop_state != PAPACC_SERVER_SECURE_STOP_RUNNING ||
            owner.shutdown_deadline_ns != 123456U || p11_close_count != 0U)
            return 211;
        fprintf(stderr, "P11_DUPLICATE_REVOKED_MEMBERSHIP_FAILED_DISPATCH_PENDING_NONOWNER_STOP=PASS root_calls=0\n");
    }
    candidates[1].connection_instance_id = 7U;
    candidates[1].transport_adapter.closed = PAPACC_TRUE;
    result = papacc_server_secure_io_win32_test_candidate_close(
        &candidates[1], &closed);
    if (result != PAPACC_RESULT_OK || closed != PAPACC_TRUE ||
        connection.state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        connection.connection_instance_id != 8U || p11_close_count != 0U)
        return 202;
    fprintf(stderr, "P11_OLD7_NEW8=PASS distinct_lifetime_ids root_calls=0\n");
    result = papacc_server_secure_io_win32_test_candidate_close(
        &candidates[1], &closed);
    if (result != PAPACC_RESULT_OK || closed != PAPACC_TRUE ||
        p11_close_count != 0U) return 212;
    fprintf(stderr, "P11_CLOSED_REPEAT=PASS root_calls=0\n");
    return 0;
}
#endif

#define CHECK(condition, code) do { if (!(condition)) return (code); } while (0)

int main(void)
{
    PAPACC_SERVER_SECURE_IO_WIN32 secure_io =
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidate =
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER;
    PAPACC_SERVER_IO_LOOP_WIN32 protocol_loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 slot =
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    PAPACC_CONNECTION_MANAGER connections =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION connection = PAPACC_CONNECTION_INITIALIZER;

#ifdef PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    {
        int p11_result = p11_slot_guard();
#define P11_OFFSET(type, field) printf("LAYOUT " #type "." #field "=%zu\n", offsetof(type, field))
#define P11_SIZE(type) printf("LAYOUT " #type ".size=%zu\n", sizeof(type))
        P11_SIZE(PAPACC_SERVER_SECURE_CANDIDATE_WIN32);
        P11_OFFSET(PAPACC_SERVER_SECURE_CANDIDATE_WIN32, owner);
        P11_OFFSET(PAPACC_SERVER_SECURE_CANDIDATE_WIN32, state);
        P11_OFFSET(PAPACC_SERVER_SECURE_CANDIDATE_WIN32, secure_processor);
        P11_OFFSET(PAPACC_SERVER_SECURE_CANDIDATE_WIN32, transport_adapter);
        P11_OFFSET(PAPACC_SERVER_SECURE_CANDIDATE_WIN32, connection_instance_id);
        P11_OFFSET(PAPACC_PST_SECURE_PROCESSOR, connection);
        P11_OFFSET(PAPACC_PST_SECURE_TRANSPORT_ADAPTER, registered);
        P11_OFFSET(PAPACC_PST_SECURE_TRANSPORT_ADAPTER, closed);
        P11_OFFSET(PAPACC_SERVER_SECURE_IO_WIN32, protocol_loop);
        P11_OFFSET(PAPACC_SERVER_SECURE_IO_WIN32, dispatch_active);
        P11_OFFSET(PAPACC_SERVER_SECURE_IO_WIN32, owner_thread_id);
        P11_OFFSET(PAPACC_SERVER_SECURE_IO_WIN32, stop_state);
        P11_OFFSET(PAPACC_SERVER_IO_LOOP_WIN32, connection_manager);
        P11_OFFSET(PAPACC_SERVER_IO_LOOP_WIN32, processor_slots);
        P11_OFFSET(PAPACC_SERVER_IO_LOOP_WIN32, processor_capacity);
        P11_SIZE(PAPACC_SERVER_PROTOCOL_SLOT_WIN32);
        P11_OFFSET(PAPACC_SERVER_PROTOCOL_SLOT_WIN32, in_use);
        P11_OFFSET(PAPACC_SERVER_PROTOCOL_SLOT_WIN32, connection_instance_id);
        P11_SIZE(PAPACC_CONNECTION);
        P11_OFFSET(PAPACC_CONNECTION, connection_instance_id);
        P11_OFFSET(PAPACC_CONNECTION, state);
        P11_OFFSET(PAPACC_CONNECTION, transport);
        P11_OFFSET(PAPACC_CONNECTION_MANAGER, storage);
        P11_OFFSET(PAPACC_CONNECTION_MANAGER, capacity);
        P11_OFFSET(PAPACC_CONNECTION_MANAGER, count);
#undef P11_OFFSET
#undef P11_SIZE
        if (p11_result != 0) return p11_result;
        p11_result = p11_wrong_connection();
        if (p11_result != 0) return p11_result;
    }
#endif
    secure_io.initialized = PAPACC_TRUE;
    secure_io.stop_state = PAPACC_SERVER_SECURE_STOP_DRAINING;
    secure_io.candidates = &candidate;
    secure_io.candidate_capacity = 1U;
    secure_io.protocol_loop = &protocol_loop;
    protocol_loop.initialized = PAPACC_TRUE;
    protocol_loop.processor_slots = &slot;
    protocol_loop.processor_capacity = 1U;
    protocol_loop.connection_manager = &connections;
    connections.storage = &connection;
    connections.capacity = 1U;
    connections.initialized = PAPACC_TRUE;

    candidate.state = PAPACC_SERVER_SECURE_CANDIDATE_SHUTTING_DOWN;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 1);

    candidate.state = PAPACC_SERVER_SECURE_CANDIDATE_CLOSED;
    candidate.connection_instance_id = 7U;
    slot.in_use = PAPACC_TRUE;
    slot.connection_instance_id = 7U;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 2);

    slot = (PAPACC_SERVER_PROTOCOL_SLOT_WIN32)
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    connections.count = 1U;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 3);

    connections.count = 0U;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 4);
    secure_io.stop_state = PAPACC_SERVER_SECURE_STOP_STOPPED;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_TRUE, 5);
    secure_io.stop_state = PAPACC_SERVER_SECURE_STOP_RUNNING;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 6);
    return 0;
}
