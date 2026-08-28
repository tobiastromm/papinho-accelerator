#include "channel.h"

#define PAPACC_TEST_CAPACITY 8

typedef struct PAPACC_TEST_TRANSPORT_CONTEXT {
    PAPACC_U32 close_count;
} PAPACC_TEST_TRANSPORT_CONTEXT;

typedef struct PAPACC_TEST_FIXTURE {
    PAPACC_CONNECTION_MANAGER connections;
    PAPACC_SESSION_MANAGER sessions;
    PAPACC_CHANNEL_MANAGER channels;
    PAPACC_CONNECTION connection_storage[PAPACC_TEST_CAPACITY];
    PAPACC_SESSION session_storage[PAPACC_TEST_CAPACITY];
    PAPACC_CHANNEL channel_storage[PAPACC_TEST_CAPACITY];
    PAPACC_TEST_TRANSPORT_CONTEXT contexts[PAPACC_TEST_CAPACITY];
} PAPACC_TEST_FIXTURE;

static void papacc_test_transport_close(void *opaque_context)
{
    PAPACC_TEST_TRANSPORT_CONTEXT *context =
        (PAPACC_TEST_TRANSPORT_CONTEXT *)opaque_context;
    ++context->close_count;
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

static PAPACC_RESULT papacc_test_fixture_init(
    PAPACC_TEST_FIXTURE *fixture,
    PAPACC_SIZE channel_capacity)
{
    PAPACC_SIZE index;
    fixture->connections = (PAPACC_CONNECTION_MANAGER)
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    fixture->sessions = (PAPACC_SESSION_MANAGER)
        PAPACC_SESSION_MANAGER_INITIALIZER;
    fixture->channels = (PAPACC_CHANNEL_MANAGER)
        PAPACC_CHANNEL_MANAGER_INITIALIZER;
    for (index = 0; index < PAPACC_TEST_CAPACITY; ++index) {
        fixture->contexts[index].close_count = 0;
    }
    if (papacc_connection_manager_init(
            &fixture->connections, fixture->connection_storage,
            PAPACC_TEST_CAPACITY) != PAPACC_RESULT_OK ||
        papacc_session_manager_init(
            &fixture->sessions, fixture->session_storage,
            PAPACC_TEST_CAPACITY) != PAPACC_RESULT_OK) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    return papacc_channel_manager_init(
        &fixture->channels,
        channel_capacity > 0 ? fixture->channel_storage : NULL,
        channel_capacity, &fixture->connections, &fixture->sessions);
}

static void papacc_test_fixture_shutdown(PAPACC_TEST_FIXTURE *fixture)
{
    papacc_channel_manager_shutdown(&fixture->channels);
    papacc_session_manager_shutdown(&fixture->sessions);
    papacc_connection_manager_shutdown(&fixture->connections);
}

static PAPACC_RESULT papacc_test_publish_connection(
    PAPACC_TEST_FIXTURE *fixture,
    PAPACC_SIZE context_index,
    PAPACC_CONNECTION **out_connection)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT local_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote_endpoint =
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;

    if (papacc_ip_address_set_ipv4(
            &local_endpoint.address, 127, 0, 0, 1) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(
            &remote_endpoint.address, 127, 0, 0, 1) != PAPACC_RESULT_OK) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    local_endpoint.port = (PAPACC_U16)(40000U + context_index);
    remote_endpoint.port = (PAPACC_U16)(41000U + context_index);
    transport.context = &fixture->contexts[context_index];
    transport.read_fn = papacc_test_transport_read;
    transport.write_fn = papacc_test_transport_write;
    transport.close_fn = papacc_test_transport_close;
    return papacc_connection_manager_publish(
        &fixture->connections, &transport, &local_endpoint, &remote_endpoint,
        out_connection);
}

static int papacc_test_initializer_init_and_zero(void)
{
    PAPACC_CHANNEL channel = PAPACC_CHANNEL_INITIALIZER;
    PAPACC_CHANNEL_MANAGER manager = PAPACC_CHANNEL_MANAGER_INITIALIZER;
    PAPACC_CONNECTION_MANAGER connections =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_SESSION_MANAGER sessions = PAPACC_SESSION_MANAGER_INITIALIZER;

    if (channel.channel_instance_id != 0 ||
        channel.state != PAPACC_CHANNEL_STATE_UNINITIALIZED ||
        channel.role != PAPACC_CHANNEL_ROLE_UNSPECIFIED ||
        channel.session_instance_id != 0 ||
        channel.connection_instance_id != 0 ||
        papacc_channel_manager_init(
            NULL, NULL, 0, &connections, &sessions) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_channel_manager_init(
            &manager, NULL, 0, &connections, &sessions) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_connection_manager_init(&connections, NULL, 0) !=
            PAPACC_RESULT_OK ||
        papacc_session_manager_init(&sessions, NULL, 0) != PAPACC_RESULT_OK ||
        papacc_channel_manager_init(
            &manager, NULL, 0, &connections, &sessions) != PAPACC_RESULT_OK) {
        return 1;
    }
    papacc_channel_manager_shutdown(&manager);
    if (connections.initialized != PAPACC_TRUE ||
        sessions.initialized != PAPACC_TRUE) {
        return 2;
    }
    papacc_session_manager_shutdown(&sessions);
    papacc_connection_manager_shutdown(&connections);
    return 0;
}

static int papacc_test_control_data_and_cascade(void)
{
    PAPACC_TEST_FIXTURE fixture;
    PAPACC_SESSION *session = NULL;
    PAPACC_CONNECTION *connections[5] = { NULL, NULL, NULL, NULL, NULL };
    PAPACC_CHANNEL *control = NULL;
    PAPACC_CHANNEL *data_a = NULL;
    PAPACC_CHANNEL *data_b = NULL;
    PAPACC_CHANNEL *data_c = NULL;
    PAPACC_CHANNEL *failed = (PAPACC_CHANNEL *)connections;
    PAPACC_U64 control_id;
    PAPACC_U64 data_a_id;
    PAPACC_SIZE index;
    int result = 0;

    if (papacc_test_fixture_init(&fixture, PAPACC_TEST_CAPACITY) !=
            PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&fixture.sessions, &session) !=
            PAPACC_RESULT_OK) {
        return 10;
    }
    for (index = 0; index < 5; ++index) {
        if (papacc_test_publish_connection(
                &fixture, index, &connections[index]) != PAPACC_RESULT_OK) {
            result = 11;
            goto cleanup;
        }
    }
    if (papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[0]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_UNSPECIFIED, &failed) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        failed != NULL || connections[0]->state !=
            PAPACC_CONNECTION_STATE_PENDING ||
        papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[0]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_CONTROL, &control) != PAPACC_RESULT_OK) {
        result = 12;
        goto cleanup;
    }
    control_id = control->channel_instance_id;
    if (control_id == 0 || control->state != PAPACC_CHANNEL_STATE_BOUND ||
        control->role != PAPACC_CHANNEL_ROLE_CONTROL ||
        control->session_instance_id != session->session_instance_id ||
        control->connection_instance_id !=
            connections[0]->connection_instance_id ||
        connections[0]->state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        session->state != PAPACC_SESSION_STATE_ESTABLISHING ||
        papacc_channel_manager_find(&fixture.channels, control_id) != control ||
        papacc_channel_manager_find_by_connection(
            &fixture.channels, connections[0]->connection_instance_id) !=
            control ||
        papacc_channel_manager_find_control(
            &fixture.channels, session->session_instance_id) != control) {
        result = 13;
        goto cleanup;
    }
    if (papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[1]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_CONTROL, &failed) !=
            PAPACC_RESULT_INVALID_STATE ||
        failed != NULL || connections[1]->state !=
            PAPACC_CONNECTION_STATE_PENDING ||
        papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[1]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &failed) !=
            PAPACC_RESULT_INVALID_STATE ||
        connections[1]->state != PAPACC_CONNECTION_STATE_PENDING ||
        papacc_session_activate(session) != PAPACC_RESULT_OK) {
        result = 14;
        goto cleanup;
    }
    if (papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[1]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &data_a) != PAPACC_RESULT_OK ||
        papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[2]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &data_b) != PAPACC_RESULT_OK ||
        data_a->channel_instance_id == data_b->channel_instance_id ||
        connections[1]->state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        connections[2]->state != PAPACC_CONNECTION_STATE_ASSOCIATED) {
        result = 15;
        goto cleanup;
    }
    data_a_id = data_a->channel_instance_id;
    if (papacc_channel_manager_close(&fixture.channels, data_a_id) !=
            PAPACC_RESULT_OK ||
        papacc_channel_manager_close(&fixture.channels, data_a_id) !=
            PAPACC_RESULT_OK ||
        data_a->state != PAPACC_CHANNEL_STATE_CLOSED ||
        data_a->channel_instance_id != data_a_id ||
        connections[1]->state != PAPACC_CONNECTION_STATE_CLOSED ||
        fixture.contexts[1].close_count != 1 ||
        session->state != PAPACC_SESSION_STATE_ACTIVE ||
        control->state != PAPACC_CHANNEL_STATE_BOUND) {
        result = 16;
        goto cleanup;
    }
    if (papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[3]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &data_c) != PAPACC_RESULT_OK ||
        papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[3]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &failed) !=
            PAPACC_RESULT_INVALID_STATE || failed != NULL ||
        papacc_channel_manager_remove(
            &fixture.channels, data_c->channel_instance_id) !=
            PAPACC_RESULT_OK ||
        connections[3]->state != PAPACC_CONNECTION_STATE_CLOSED ||
        session->state != PAPACC_SESSION_STATE_ACTIVE ||
        papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            connections[4]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &data_c) != PAPACC_RESULT_OK ||
        papacc_channel_manager_close(&fixture.channels, control_id) !=
            PAPACC_RESULT_OK) {
        result = 17;
        goto cleanup;
    }
    if (session->state != PAPACC_SESSION_STATE_CLOSED ||
        control->state != PAPACC_CHANNEL_STATE_CLOSED ||
        data_b->state != PAPACC_CHANNEL_STATE_CLOSED ||
        connections[0]->state != PAPACC_CONNECTION_STATE_CLOSED ||
        connections[2]->state != PAPACC_CONNECTION_STATE_CLOSED ||
        connections[4]->state != PAPACC_CONNECTION_STATE_CLOSED ||
        fixture.contexts[0].close_count != 1 ||
        fixture.contexts[2].close_count != 1 ||
        fixture.contexts[4].close_count != 1) {
        result = 18;
        goto cleanup;
    }
    if (papacc_channel_manager_remove(&fixture.channels, control_id) !=
            PAPACC_RESULT_OK ||
        control->state != PAPACC_CHANNEL_STATE_UNINITIALIZED ||
        data_b->state != PAPACC_CHANNEL_STATE_CLOSED ||
        fixture.channels.count != 3 ||
        papacc_channel_manager_remove(&fixture.channels, data_a_id) !=
            PAPACC_RESULT_OK) {
        result = 19;
    }

cleanup:
    papacc_test_fixture_shutdown(&fixture);
    return result;
}

static int papacc_test_data_without_control_and_capacity(void)
{
    PAPACC_TEST_FIXTURE zero_fixture;
    PAPACC_TEST_FIXTURE fixture;
    PAPACC_SESSION *session = NULL;
    PAPACC_CONNECTION *control_connection = NULL;
    PAPACC_CONNECTION *data_connection = NULL;
    PAPACC_CHANNEL *control = NULL;
    PAPACC_CHANNEL *failed = (PAPACC_CHANNEL *)&fixture;
    int result = 0;

    if (papacc_test_fixture_init(&zero_fixture, 0) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&zero_fixture.sessions, &session) !=
            PAPACC_RESULT_OK ||
        papacc_test_publish_connection(
            &zero_fixture, 0, &control_connection) != PAPACC_RESULT_OK ||
        papacc_channel_manager_bind(
            &zero_fixture.channels, session->session_instance_id,
            control_connection->connection_instance_id,
            PAPACC_CHANNEL_ROLE_CONTROL, &failed) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        failed != NULL || control_connection->state !=
            PAPACC_CONNECTION_STATE_PENDING) {
        papacc_test_fixture_shutdown(&zero_fixture);
        return 20;
    }
    papacc_test_fixture_shutdown(&zero_fixture);
    session = NULL;
    control_connection = NULL;
    failed = (PAPACC_CHANNEL *)&fixture;
    if (papacc_test_fixture_init(&fixture, 1) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&fixture.sessions, &session) !=
            PAPACC_RESULT_OK ||
        papacc_test_publish_connection(
            &fixture, 0, &control_connection) != PAPACC_RESULT_OK ||
        papacc_test_publish_connection(
            &fixture, 1, &data_connection) != PAPACC_RESULT_OK ||
        papacc_session_activate(session) != PAPACC_RESULT_OK) {
        return 21;
    }
    if (papacc_channel_manager_bind(
            &fixture.channels, 9999, control_connection->connection_instance_id,
            PAPACC_CHANNEL_ROLE_CONTROL, &failed) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        failed != NULL || control_connection->state !=
            PAPACC_CONNECTION_STATE_PENDING ||
        papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id, 9999,
            PAPACC_CHANNEL_ROLE_CONTROL, &failed) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        failed != NULL || session->state !=
            PAPACC_SESSION_STATE_ACTIVE) {
        result = 22;
        goto cleanup;
    }
    if (papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            data_connection->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &failed) !=
            PAPACC_RESULT_INVALID_STATE ||
        failed != NULL || data_connection->state !=
            PAPACC_CONNECTION_STATE_PENDING) {
        result = 23;
        goto cleanup;
    }
    session->state = PAPACC_SESSION_STATE_ESTABLISHING;
    if (papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            control_connection->connection_instance_id,
            PAPACC_CHANNEL_ROLE_CONTROL, &control) != PAPACC_RESULT_OK ||
        papacc_session_activate(session) != PAPACC_RESULT_OK ||
        papacc_channel_manager_bind(
            &fixture.channels, session->session_instance_id,
            data_connection->connection_instance_id,
            PAPACC_CHANNEL_ROLE_DATA, &failed) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        failed != NULL || data_connection->state !=
            PAPACC_CONNECTION_STATE_PENDING ||
        fixture.channels.count != 1) {
        result = 24;
    }

cleanup:
    papacc_test_fixture_shutdown(&fixture);
    return result;
}

static int papacc_test_reuse_wrap_shutdown_and_violation(void)
{
    PAPACC_TEST_FIXTURE fixture;
    PAPACC_SESSION *sessions[3] = { NULL, NULL, NULL };
    PAPACC_CONNECTION *connections[3] = { NULL, NULL, NULL };
    PAPACC_CHANNEL *channels[3] = { NULL, NULL, NULL };
    PAPACC_U64 first_id;
    PAPACC_SIZE index;
    int result = 0;

    if (papacc_test_fixture_init(&fixture, 3) != PAPACC_RESULT_OK) {
        return 30;
    }
    fixture.channels.next_instance_id = (PAPACC_U64)-1;
    for (index = 0; index < 3; ++index) {
        if (papacc_session_manager_publish(
                &fixture.sessions, &sessions[index]) != PAPACC_RESULT_OK ||
            papacc_test_publish_connection(
                &fixture, index, &connections[index]) != PAPACC_RESULT_OK ||
            papacc_channel_manager_bind(
                &fixture.channels, sessions[index]->session_instance_id,
                connections[index]->connection_instance_id,
                PAPACC_CHANNEL_ROLE_CONTROL, &channels[index]) !=
                PAPACC_RESULT_OK) {
            result = 31;
            goto cleanup;
        }
        if (index == 1) {
            fixture.channels.next_instance_id = (PAPACC_U64)-1;
        }
    }
    if (channels[0]->channel_instance_id != (PAPACC_U64)-1 ||
        channels[1]->channel_instance_id == 0 ||
        channels[2]->channel_instance_id == 0 ||
        channels[0]->channel_instance_id == channels[1]->channel_instance_id ||
        channels[0]->channel_instance_id == channels[2]->channel_instance_id ||
        channels[1]->channel_instance_id == channels[2]->channel_instance_id) {
        result = 32;
        goto cleanup;
    }
    first_id = channels[0]->channel_instance_id;
    if (papacc_channel_manager_remove(&fixture.channels, first_id) !=
            PAPACC_RESULT_OK ||
        fixture.channel_storage[0].state !=
            PAPACC_CHANNEL_STATE_UNINITIALIZED) {
        result = 33;
        goto cleanup;
    }
    if (papacc_session_manager_publish(&fixture.sessions, &sessions[0]) !=
            PAPACC_RESULT_OK ||
        papacc_test_publish_connection(&fixture, 3, &connections[0]) !=
            PAPACC_RESULT_OK ||
        papacc_channel_manager_bind(
            &fixture.channels, sessions[0]->session_instance_id,
            connections[0]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_CONTROL, &channels[0]) != PAPACC_RESULT_OK ||
        channels[0]->channel_instance_id == first_id) {
        result = 34;
        goto cleanup;
    }
    papacc_channel_manager_shutdown(&fixture.channels);
    papacc_channel_manager_shutdown(&fixture.channels);
    if (fixture.connections.initialized != PAPACC_TRUE ||
        fixture.sessions.initialized != PAPACC_TRUE ||
        fixture.channels.initialized != PAPACC_FALSE ||
        fixture.channel_storage[0].state !=
            PAPACC_CHANNEL_STATE_UNINITIALIZED ||
        fixture.channel_storage[1].state !=
            PAPACC_CHANNEL_STATE_UNINITIALIZED ||
        fixture.channel_storage[2].state !=
            PAPACC_CHANNEL_STATE_UNINITIALIZED) {
        result = 35;
        goto dependencies_cleanup;
    }
    if (papacc_channel_manager_init(
            &fixture.channels, fixture.channel_storage, 3,
            &fixture.connections, &fixture.sessions) != PAPACC_RESULT_OK) {
        result = 36;
        goto dependencies_cleanup;
    }
    /* Bind a fresh relationship, then deliberately violate lower lifetime. */
    if (papacc_session_manager_publish(&fixture.sessions, &sessions[0]) !=
            PAPACC_RESULT_OK ||
        papacc_test_publish_connection(&fixture, 4, &connections[0]) !=
            PAPACC_RESULT_OK ||
        papacc_channel_manager_bind(
            &fixture.channels, sessions[0]->session_instance_id,
            connections[0]->connection_instance_id,
            PAPACC_CHANNEL_ROLE_CONTROL, &channels[0]) != PAPACC_RESULT_OK) {
        result = 37;
        goto cleanup;
    }
    first_id = channels[0]->channel_instance_id;
    if (papacc_connection_manager_remove(
            &fixture.connections, connections[0]->connection_instance_id) !=
            PAPACC_RESULT_OK ||
        papacc_channel_manager_close(&fixture.channels, first_id) !=
            PAPACC_RESULT_INVALID_STATE) {
        result = 38;
    }

cleanup:
    papacc_channel_manager_shutdown(&fixture.channels);
dependencies_cleanup:
    papacc_session_manager_shutdown(&fixture.sessions);
    papacc_connection_manager_shutdown(&fixture.connections);
    return result;
}

int main(void)
{
    int result = papacc_test_initializer_init_and_zero();
    if (result == 0) {
        result = papacc_test_control_data_and_cascade();
    }
    if (result == 0) {
        result = papacc_test_data_without_control_and_capacity();
    }
    if (result == 0) {
        result = papacc_test_reuse_wrap_shutdown_and_violation();
    }
    return result;
}
