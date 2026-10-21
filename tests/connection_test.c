#include "connection.h"

#include <stdio.h>
#include <string.h>

typedef struct PAPACC_TEST_TRANSPORT_CONTEXT {
    PAPACC_BOOL owned;
    PAPACC_U32 close_count;
} PAPACC_TEST_TRANSPORT_CONTEXT;

static void papacc_test_transport_close(void *opaque_context)
{
    PAPACC_TEST_TRANSPORT_CONTEXT *context =
        (PAPACC_TEST_TRANSPORT_CONTEXT *)opaque_context;
    ++context->close_count;
    context->owned = PAPACC_FALSE;
}

static PAPACC_RESULT papacc_test_transport_read(
    void *opaque_context, PAPACC_U8 *buffer, PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    (void)opaque_context;
    (void)buffer;
    (void)capacity;
    *out_transferred = 0;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_test_transport_write(
    void *opaque_context, const PAPACC_U8 *buffer, PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    (void)opaque_context;
    (void)buffer;
    (void)length;
    *out_transferred = 0;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}

static PAPACC_TRANSPORT_CONNECTION papacc_test_transport(
    PAPACC_TEST_TRANSPORT_CONTEXT *context)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    context->owned = PAPACC_TRUE;
    transport.context = context;
    transport.read_fn = papacc_test_transport_read;
    transport.write_fn = papacc_test_transport_write;
    transport.close_fn = papacc_test_transport_close;
    return transport;
}

static PAPACC_RESULT papacc_test_endpoints(
    PAPACC_NETWORK_ENDPOINT *local_endpoint,
    PAPACC_NETWORK_ENDPOINT *remote_endpoint)
{
    if (papacc_ip_address_set_ipv4(
            &local_endpoint->address, 127, 0, 0, 1) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(
            &remote_endpoint->address, 127, 0, 0, 1) != PAPACC_RESULT_OK) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    local_endpoint->port = 41000;
    remote_endpoint->port = 41001;
    return PAPACC_RESULT_OK;
}

static int papacc_test_transport_lifecycle(void)
{
    PAPACC_TEST_TRANSPORT_CONTEXT context = { PAPACC_FALSE, 0 };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_transport(&context);

    if (papacc_transport_connection_is_valid(&transport) != PAPACC_TRUE) {
        return 1;
    }
    papacc_transport_connection_close(&transport);
    papacc_transport_connection_close(&transport);
    if (context.close_count != 1 || context.owned != PAPACC_FALSE ||
        transport.context != NULL || transport.read_fn != NULL ||
        transport.write_fn != NULL || transport.close_fn != NULL) {
        return 2;
    }
    return 0;
}

static int papacc_test_publication_and_ids(void)
{
    PAPACC_CONNECTION_MANAGER manager =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage[2];
    PAPACC_CONNECTION *first = NULL;
    PAPACC_CONNECTION *second = NULL;
    PAPACC_NETWORK_ENDPOINT local_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_TEST_TRANSPORT_CONTEXT contexts[2] = {
        { PAPACC_FALSE, 0 }, { PAPACC_FALSE, 0 }
    };
    PAPACC_TRANSPORT_CONNECTION first_transport;
    PAPACC_TRANSPORT_CONNECTION second_transport;
    PAPACC_U64 first_id;

    if (papacc_test_endpoints(&local_endpoint, &remote_endpoint) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, storage, 2) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, storage, 2) !=
            PAPACC_RESULT_INVALID_STATE) {
        return 10;
    }
    manager.next_instance_id = (PAPACC_U64)-1;
    first_transport = papacc_test_transport(&contexts[0]);
    if (papacc_connection_manager_publish(
            &manager, &first_transport, &local_endpoint, &remote_endpoint,
            &first) != PAPACC_RESULT_OK ||
        first == NULL || first->connection_instance_id == 0 ||
        first->state != PAPACC_CONNECTION_STATE_PENDING ||
        first_transport.context != NULL || manager.count != 1 ||
        papacc_network_endpoint_equal(
            &first->local_endpoint, &local_endpoint) != PAPACC_TRUE ||
        papacc_network_endpoint_equal(
            &first->remote_endpoint, &remote_endpoint) != PAPACC_TRUE) {
        return 11;
    }
    first_id = first->connection_instance_id;
    if (papacc_connection_mark_associated(first) != PAPACC_RESULT_OK ||
        first->state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        papacc_connection_mark_associated(first) !=
            PAPACC_RESULT_INVALID_STATE) {
        return 12;
    }
    second_transport = papacc_test_transport(&contexts[1]);
    if (papacc_connection_manager_publish(
            &manager, &second_transport, &local_endpoint, &remote_endpoint,
            &second) != PAPACC_RESULT_OK ||
        second == NULL || second->connection_instance_id == 0 ||
        second->connection_instance_id == first_id ||
        papacc_connection_manager_find(&manager, first_id) != first ||
        papacc_connection_manager_find(&manager, 0) != NULL ||
        papacc_connection_manager_find(&manager, 42) != NULL) {
        return 13;
    }
    papacc_connection_close(first);
    papacc_connection_close(first);
    if (first->state != PAPACC_CONNECTION_STATE_CLOSED ||
        first->connection_instance_id != first_id ||
        contexts[0].close_count != 1 || manager.count != 2) {
        return 14;
    }
    if (papacc_connection_manager_remove(&manager, first_id) !=
            PAPACC_RESULT_OK ||
        storage[0].state != PAPACC_CONNECTION_STATE_UNINITIALIZED ||
        manager.count != 1 || contexts[0].close_count != 1) {
        return 15;
    }
    papacc_connection_manager_shutdown(&manager);
    papacc_connection_manager_shutdown(&manager);
    if (contexts[1].close_count != 1 || manager.initialized != PAPACC_FALSE ||
        manager.count != 0 ||
        storage[1].state != PAPACC_CONNECTION_STATE_UNINITIALIZED) {
        return 16;
    }
    return 0;
}

static int papacc_test_failures_reuse_and_shutdown(void)
{
    PAPACC_CONNECTION_MANAGER manager =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION invalid_manager_storage[1];
    PAPACC_CONNECTION storage[1];
    PAPACC_CONNECTION *published = (PAPACC_CONNECTION *)storage;
    PAPACC_NETWORK_ENDPOINT local_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT invalid_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_TEST_TRANSPORT_CONTEXT contexts[3] = {
        { PAPACC_FALSE, 0 }, { PAPACC_FALSE, 0 }, { PAPACC_FALSE, 0 }
    };
    PAPACC_TRANSPORT_CONNECTION transports[3];
    PAPACC_U64 first_id;

    if (papacc_test_endpoints(&local_endpoint, &remote_endpoint) !=
        PAPACC_RESULT_OK) {
        return 20;
    }
    transports[0] = papacc_test_transport(&contexts[0]);
    if (papacc_connection_manager_publish(
            &manager, &transports[0], &local_endpoint, &remote_endpoint,
            &published) != PAPACC_RESULT_INVALID_STATE ||
        published != NULL || contexts[0].owned != PAPACC_TRUE) {
        return 21;
    }
    papacc_transport_connection_close(&transports[0]);
    if (papacc_connection_manager_init(&manager, storage, 1) !=
        PAPACC_RESULT_OK) {
        return 22;
    }
    transports[0] = papacc_test_transport(&contexts[0]);
    published = (PAPACC_CONNECTION *)invalid_manager_storage;
    if (papacc_connection_manager_publish(
            &manager, &transports[0], &invalid_endpoint, &remote_endpoint,
            &published) != PAPACC_RESULT_INVALID_ARGUMENT ||
        published != NULL || contexts[0].owned != PAPACC_TRUE) {
        return 23;
    }
    papacc_transport_connection_close(&transports[0]);
    transports[0] = papacc_test_transport(&contexts[0]);
    if (papacc_connection_manager_publish(
            &manager, &transports[0], &local_endpoint, &remote_endpoint,
            &published) != PAPACC_RESULT_OK) {
        return 24;
    }
    first_id = published->connection_instance_id;
    transports[1] = papacc_test_transport(&contexts[1]);
    published = (PAPACC_CONNECTION *)storage;
    if (papacc_connection_manager_publish(
            &manager, &transports[1], &local_endpoint, &remote_endpoint,
            &published) != PAPACC_RESULT_LIMIT_EXCEEDED ||
        published != NULL || contexts[1].owned != PAPACC_TRUE ||
        contexts[1].close_count != 0) {
        return 25;
    }
    papacc_transport_connection_close(&transports[1]);
    if (papacc_connection_manager_remove(&manager, first_id) !=
            PAPACC_RESULT_OK || contexts[0].close_count != 3) {
        return 26;
    }
    transports[2] = papacc_test_transport(&contexts[2]);
    if (papacc_connection_manager_publish(
            &manager, &transports[2], &local_endpoint, &remote_endpoint,
            &published) != PAPACC_RESULT_OK ||
        published->connection_instance_id == first_id) {
        return 27;
    }
    papacc_connection_manager_shutdown(&manager);
    if (contexts[2].close_count != 1) {
        return 28;
    }
    return 0;
}

typedef struct PAPACC_TEST_CLOSE_GATE {
    PAPACC_U32 prepare_count;
    PAPACC_U32 close_count;
    PAPACC_RESULT result;
    PAPACC_BOOL ready;
    PAPACC_U32 cause;
} PAPACC_TEST_CLOSE_GATE;

static PAPACC_RESULT papacc_test_gate_prepare(
    void *opaque_context, PAPACC_BOOL *out_ready)
{
    PAPACC_TEST_CLOSE_GATE *gate =
        (PAPACC_TEST_CLOSE_GATE *)opaque_context;
    ++gate->prepare_count;
    *out_ready = gate->ready;
    return gate->result;
}

static void papacc_test_gate_close(void *opaque_context)
{
    PAPACC_TEST_CLOSE_GATE *gate =
        (PAPACC_TEST_CLOSE_GATE *)opaque_context;
    ++gate->close_count;
}

static PAPACC_TRANSPORT_CONNECTION papacc_test_gate_transport(
    PAPACC_TEST_CLOSE_GATE *gate)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    transport.context = gate;
    transport.read_fn = papacc_test_transport_read;
    transport.write_fn = papacc_test_transport_write;
    transport.pre_close_fn = papacc_test_gate_prepare;
    transport.close_fn = papacc_test_gate_close;
    return transport;
}

static int papacc_test_result_aware_close(void)
{
    PAPACC_CONNECTION_MANAGER manager =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage[1];
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_TEST_CLOSE_GATE gate =
        { 0, 0, PAPACC_RESULT_OK, PAPACC_FALSE, 0U };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_gate_transport(&gate);
    PAPACC_U64 connection_id;
    PAPACC_BOOL completed = PAPACC_TRUE;

    if (papacc_test_endpoints(&local_endpoint, &remote_endpoint) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, storage, 1) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_publish(
            &manager, &transport, &local_endpoint, &remote_endpoint,
            &connection) != PAPACC_RESULT_OK || connection == NULL) {
        return 30;
    }
    connection_id = connection->connection_instance_id;
    if (papacc_connection_manager_try_remove(
            &manager, connection_id, &completed) != PAPACC_RESULT_OK ||
        completed != PAPACC_FALSE || gate.prepare_count != 1 ||
        gate.close_count != 0 || manager.count != 1 ||
        connection->state != PAPACC_CONNECTION_STATE_CLOSING ||
        connection->connection_instance_id != connection_id ||
        connection->transport.context != &gate) {
        return 31;
    }
    gate.result = PAPACC_RESULT_INTERNAL_ERROR;
    gate.ready = PAPACC_TRUE;
    if (papacc_connection_try_close(connection, &completed) !=
            PAPACC_RESULT_INTERNAL_ERROR || completed != PAPACC_FALSE ||
        gate.prepare_count != 2 || gate.close_count != 0 ||
        manager.count != 1 || connection->connection_instance_id !=
            connection_id || connection->transport.context != &gate) {
        return 32;
    }
    gate.result = PAPACC_RESULT_OK;
    if (papacc_connection_manager_try_remove(
            &manager, connection_id, &completed) != PAPACC_RESULT_OK ||
        completed != PAPACC_TRUE || gate.prepare_count != 3 ||
        gate.close_count != 1 || manager.count != 0 ||
        storage[0].state != PAPACC_CONNECTION_STATE_UNINITIALIZED) {
        return 33;
    }
    return 0;
}
typedef struct PAPACC_TEST_P3_EVIDENCE {
    PAPACC_U64 expected_connection_id;
    PAPACC_U64 observed_connection_id;
    PAPACC_U32 expected_cause;
    PAPACC_U32 observed_cause;
    PAPACC_U32 prepare_count;
    PAPACC_U32 close_count;
    PAPACC_SIZE manager_count;
    PAPACC_CONNECTION_STATE state;
    PAPACC_RESULT result;
    PAPACC_BOOL ready;
    PAPACC_BOOL handle_preserved;
    PAPACC_BOOL transport_preserved;
    PAPACC_BOOL connection_published;
} PAPACC_TEST_P3_EVIDENCE;

static PAPACC_BOOL papacc_test_p3_evidence_valid(
    const PAPACC_TEST_P3_EVIDENCE *evidence)
{
    if (evidence == NULL || evidence->expected_connection_id == 0U ||
        evidence->observed_connection_id != evidence->expected_connection_id ||
        evidence->expected_cause != evidence->observed_cause ||
        evidence->prepare_count != 1U || evidence->close_count != 0U ||
        evidence->manager_count != 1U ||
        evidence->state != PAPACC_CONNECTION_STATE_CLOSING ||
        evidence->result != PAPACC_RESULT_OK ||
        evidence->ready != PAPACC_FALSE ||
        evidence->handle_preserved != PAPACC_TRUE ||
        evidence->transport_preserved != PAPACC_TRUE ||
        evidence->connection_published != PAPACC_TRUE) {
        return PAPACC_FALSE;
    }
    return PAPACC_TRUE;
}

static int papacc_test_p3_deferred(void)
{
    PAPACC_CONNECTION_MANAGER manager = PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION storage[1];
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_NETWORK_ENDPOINT local_endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote_endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_TEST_CLOSE_GATE gate =
        { 0U, 0U, PAPACC_RESULT_OK, PAPACC_FALSE, 73U };
    PAPACC_TRANSPORT_CONNECTION transport = papacc_test_gate_transport(&gate);
    PAPACC_TEST_P3_EVIDENCE evidence;
    PAPACC_TEST_P3_EVIDENCE mutation;
    PAPACC_U64 connection_id;
    PAPACC_BOOL removed = PAPACC_TRUE;
    PAPACC_U32 rejected = 0U;

    if (papacc_test_endpoints(&local_endpoint, &remote_endpoint) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&manager, storage, 1U) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_publish(
            &manager, &transport, &local_endpoint, &remote_endpoint,
            &connection) != PAPACC_RESULT_OK || connection == NULL) {
        return 40;
    }
    connection_id = connection->connection_instance_id;
    if (papacc_connection_manager_try_remove(
            &manager, connection_id, &removed) != PAPACC_RESULT_OK ||
        removed != PAPACC_FALSE) {
        return 41;
    }

    evidence.expected_connection_id = connection_id;
    evidence.observed_connection_id = connection->connection_instance_id;
    evidence.expected_cause = 73U;
    evidence.observed_cause = gate.cause;
    evidence.prepare_count = gate.prepare_count;
    evidence.close_count = gate.close_count;
    evidence.manager_count = manager.count;
    evidence.state = connection->state;
    evidence.result = PAPACC_RESULT_OK;
    evidence.ready = removed;
    evidence.handle_preserved = connection->transport.context == &gate
        ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.transport_preserved =
        connection->transport.pre_close_fn == papacc_test_gate_prepare &&
        connection->transport.close_fn == papacc_test_gate_close
            ? PAPACC_TRUE : PAPACC_FALSE;
    evidence.connection_published =
        papacc_connection_manager_find(&manager, connection_id) == connection
            ? PAPACC_TRUE : PAPACC_FALSE;
    if (papacc_test_p3_evidence_valid(&evidence) != PAPACC_TRUE) {
        return 42;
    }

#define PAPACC_TEST_P3_REJECT(statement) \
    do { \
        mutation = evidence; \
        statement; \
        if (papacc_test_p3_evidence_valid(&mutation) != PAPACC_FALSE) \
            return 43; \
        ++rejected; \
    } while (0)

    PAPACC_TEST_P3_REJECT(mutation.ready = PAPACC_TRUE);
    PAPACC_TEST_P3_REJECT(mutation.handle_preserved = PAPACC_FALSE);
    PAPACC_TEST_P3_REJECT(mutation.connection_published = PAPACC_FALSE);
    PAPACC_TEST_P3_REJECT(mutation.close_count = 1U);
    PAPACC_TEST_P3_REJECT(++mutation.observed_connection_id);
    PAPACC_TEST_P3_REJECT(mutation.prepare_count = 2U);
    PAPACC_TEST_P3_REJECT(++mutation.observed_cause);
#undef PAPACC_TEST_P3_REJECT

    if (rejected != 7U) {
        return 44;
    }

    gate.ready = PAPACC_TRUE;
    if (papacc_connection_manager_try_remove(
            &manager, connection_id, &removed) != PAPACC_RESULT_OK ||
        removed != PAPACC_TRUE || gate.prepare_count != 2U ||
        gate.close_count != 1U || manager.count != 0U ||
        storage[0].state != PAPACC_CONNECTION_STATE_UNINITIALIZED ||
        papacc_connection_manager_find(&manager, connection_id) != NULL) {
        return 45;
    }
    if (papacc_connection_manager_remove(&manager, connection_id) !=
            PAPACC_RESULT_INVALID_ARGUMENT || gate.close_count != 1U) {
        return 46;
    }

    (void)printf(
        "P3_GENERIC_REAL_CONNECTION_PATH=PASS "
        "P3_DEFERRED_RESULT=OK_READY_FALSE "
        "P3_CONNECTION_PRESERVED=YES P3_HANDLE_PRESERVED=YES "
        "P3_DESTRUCTIVE_CLOSE_BLOCKED=YES "
        "P3_MANAGER_REMOVE_BLOCKED=YES P3_IDENTITY_PRESERVED=YES "
        "P3_CAUSE_PRESERVED=YES P3_RETRY_READY=YES "
        "P3_FINAL_CLOSE=ONCE P3_NEGATIVES=%lu_OF_7_PASS\n",
        (unsigned long)rejected);
    return 0;
}
int main(int argc, char **argv)
{
    int result;

    if (argc == 2 && strcmp(argv[1], "--adr0032-p3") == 0) {
        return papacc_test_p3_deferred();
    }
    if (argc != 1) {
        return 47;
    }
    result = papacc_test_transport_lifecycle();
    if (result == 0) {
        result = papacc_test_publication_and_ids();
    }
    if (result == 0) {
        result = papacc_test_failures_reuse_and_shutdown();
    }
    if (result == 0) {
        result = papacc_test_result_aware_close();
    }
    if (result == 0) {
        result = papacc_test_p3_deferred();
    }
    return result;
}
