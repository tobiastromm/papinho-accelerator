#include "connection.h"

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

static PAPACC_TRANSPORT_CONNECTION papacc_test_transport(
    PAPACC_TEST_TRANSPORT_CONTEXT *context)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    context->owned = PAPACC_TRUE;
    transport.context = context;
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
        transport.context != NULL || transport.close_fn != NULL) {
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
    second_transport = papacc_test_transport(&contexts[1]);
    if (papacc_connection_manager_publish(
            &manager, &second_transport, &local_endpoint, &remote_endpoint,
            &second) != PAPACC_RESULT_OK ||
        second == NULL || second->connection_instance_id == 0 ||
        second->connection_instance_id == first_id ||
        papacc_connection_manager_find(&manager, first_id) != first ||
        papacc_connection_manager_find(&manager, 0) != NULL ||
        papacc_connection_manager_find(&manager, 42) != NULL) {
        return 12;
    }
    papacc_connection_close(first);
    papacc_connection_close(first);
    if (first->state != PAPACC_CONNECTION_STATE_CLOSED ||
        first->connection_instance_id != first_id ||
        contexts[0].close_count != 1 || manager.count != 2) {
        return 13;
    }
    if (papacc_connection_manager_remove(&manager, first_id) !=
            PAPACC_RESULT_OK ||
        storage[0].state != PAPACC_CONNECTION_STATE_UNINITIALIZED ||
        manager.count != 1 || contexts[0].close_count != 1) {
        return 14;
    }
    papacc_connection_manager_shutdown(&manager);
    papacc_connection_manager_shutdown(&manager);
    if (contexts[1].close_count != 1 || manager.initialized != PAPACC_FALSE ||
        manager.count != 0 ||
        storage[1].state != PAPACC_CONNECTION_STATE_UNINITIALIZED) {
        return 15;
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

int main(void)
{
    int result = papacc_test_transport_lifecycle();
    if (result == 0) {
        result = papacc_test_publication_and_ids();
    }
    if (result == 0) {
        result = papacc_test_failures_reuse_and_shutdown();
    }
    return result;
}
