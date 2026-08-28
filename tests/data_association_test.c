#include "data_association.h"

typedef struct PAPACC_TEST_GENERATOR {
    PAPACC_DATA_ASSOCIATION_TICKET values[40];
    PAPACC_SIZE count;
    PAPACC_SIZE index;
    PAPACC_RESULT result;
} PAPACC_TEST_GENERATOR;

static PAPACC_RESULT papacc_test_generate(
    void *context, PAPACC_DATA_ASSOCIATION_TICKET *out_ticket)
{
    PAPACC_TEST_GENERATOR *generator = (PAPACC_TEST_GENERATOR *)context;
    if (generator->result != PAPACC_RESULT_OK) return generator->result;
    if (generator->index < generator->count)
        *out_ticket = generator->values[generator->index++];
    else
        *out_ticket = (PAPACC_DATA_ASSOCIATION_TICKET)
            PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_test_read(
    void *context, PAPACC_U8 *buffer, PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    (void)context; (void)buffer; (void)capacity;
    *out_transferred = 0; *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}
static PAPACC_RESULT papacc_test_write(
    void *context, const PAPACC_U8 *buffer, PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    (void)context; (void)buffer; (void)length;
    *out_transferred = 0; *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
    return PAPACC_RESULT_OK;
}
static void papacc_test_close(void *context) { (void)context; }

static void papacc_test_set_ticket(
    PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U8 value)
{
    PAPACC_SIZE index;
    *ticket = (PAPACC_DATA_ASSOCIATION_TICKET)
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    for (index = 0; index < 16; ++index)
        ticket->bytes[index] = (PAPACC_U8)(value + index);
}

int main(void)
{
    PAPACC_CONNECTION connections[2];
    PAPACC_SESSION sessions[2];
    PAPACC_CHANNEL channels[2];
    PAPACC_CONNECTION_MANAGER connection_manager =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_SESSION_MANAGER session_manager = PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_CHANNEL_MANAGER channel_manager = PAPACC_CHANNEL_MANAGER_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_ENTRY entries[2];
    PAPACC_DATA_ASSOCIATION_MANAGER manager =
        PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER;
    PAPACC_TEST_GENERATOR generator = { { { { 0 } } }, 0, 0, PAPACC_RESULT_OK };
    PAPACC_SESSION *session[2] = { NULL, NULL };
    PAPACC_CONNECTION *connection;
    PAPACC_CHANNEL *channel;
    PAPACC_TRANSPORT_CONNECTION transport;
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_TICKET issued;
    PAPACC_DATA_ASSOCIATION_TICKET second;
    PAPACC_U64 deadline = 0;
    PAPACC_U64 session_id = 0;
    PAPACC_SIZE expired = 0;
    PAPACC_SIZE index;

    if (papacc_ip_address_set_ipv4(&endpoint.address, 127,0,0,1) !=
            PAPACC_RESULT_OK ||
        papacc_connection_manager_init(&connection_manager, connections, 2) !=
            PAPACC_RESULT_OK ||
        papacc_session_manager_init(&session_manager, sessions, 2) !=
            PAPACC_RESULT_OK ||
        papacc_channel_manager_init(&channel_manager, channels, 2,
            &connection_manager, &session_manager) != PAPACC_RESULT_OK)
        return 1;
    endpoint.port = 1;
    for (index = 0; index < 2; ++index) {
        int context = 0;
        transport.context = &context;
        transport.read_fn = papacc_test_read;
        transport.write_fn = papacc_test_write;
        transport.close_fn = papacc_test_close;
        if (papacc_connection_manager_publish(
                &connection_manager, &transport, &endpoint, &endpoint,
                &connection) != PAPACC_RESULT_OK ||
            papacc_session_manager_publish(&session_manager, &session[index]) !=
                PAPACC_RESULT_OK ||
            papacc_channel_manager_bind(&channel_manager,
                session[index]->session_instance_id,
                connection->connection_instance_id,
                PAPACC_CHANNEL_ROLE_CONTROL, &channel) != PAPACC_RESULT_OK ||
            papacc_session_activate(session[index]) != PAPACC_RESULT_OK)
            return 2;
    }
    papacc_test_set_ticket(&generator.values[1], 1);
    papacc_test_set_ticket(&generator.values[2], 1); /* collision */
    papacc_test_set_ticket(&generator.values[3], 33);
    papacc_test_set_ticket(&generator.values[4], 65);
    generator.count = 5;
    if (papacc_data_association_manager_init(
            &manager, entries, 2, &session_manager, &channel_manager,
            papacc_test_generate, &generator, 10) != PAPACC_RESULT_OK ||
        papacc_data_association_manager_issue(&manager,
            session[0]->session_instance_id, 100, &issued, &deadline) !=
            PAPACC_RESULT_OK || deadline != 110 || manager.count != 1 ||
        generator.index != 2)
        return 3;
    if (papacc_data_association_manager_issue(&manager,
            session[0]->session_instance_id, 109, &second, &session_id) !=
            PAPACC_RESULT_OK ||
        papacc_data_association_ticket_equal(&issued, &second) != PAPACC_TRUE ||
        session_id != 110 || generator.index != 2 || manager.count != 1)
        return 4;
    if (papacc_data_association_manager_issue(&manager,
            session[1]->session_instance_id, 100, &second, &deadline) !=
            PAPACC_RESULT_OK || generator.index != 4 || manager.count != 2 ||
        papacc_data_association_ticket_equal(&issued, &second) == PAPACC_TRUE)
        return 5;
    if (papacc_data_association_manager_consume(
            &manager, &issued, 109, &session_id) != PAPACC_RESULT_OK ||
        session_id != session[0]->session_instance_id || manager.count != 1 ||
        papacc_data_association_manager_consume(
            &manager, &issued, 109, &session_id) != PAPACC_RESULT_INVALID_STATE ||
        session_id != 0)
        return 6;
    if (papacc_data_association_manager_issue(&manager,
            session[0]->session_instance_id, 200, &issued, &deadline) !=
            PAPACC_RESULT_OK || generator.index != 5 || deadline != 210 ||
        papacc_data_association_manager_consume(
            &manager, &issued, 210, &session_id) != PAPACC_RESULT_INVALID_STATE ||
        manager.count != 1)
        return 7;
    if (papacc_data_association_manager_invalidate_session(
            &manager, session[1]->session_instance_id) != PAPACC_RESULT_OK ||
        papacc_data_association_manager_invalidate_session(
            &manager, session[1]->session_instance_id) != PAPACC_RESULT_OK ||
        manager.count != 0)
        return 8;
    papacc_data_association_manager_shutdown(&manager);
    papacc_test_set_ticket(&generator.values[generator.index], 80);
    ++generator.count;
    if (papacc_data_association_manager_init(
            &manager, entries, 1, &session_manager, &channel_manager,
            papacc_test_generate, &generator, 10) != PAPACC_RESULT_OK ||
        papacc_data_association_manager_issue(&manager,
            session[0]->session_instance_id, 1, &issued, &deadline) !=
            PAPACC_RESULT_OK ||
        papacc_data_association_manager_issue(&manager,
            session[1]->session_instance_id, 1, &second, &deadline) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || manager.count != 1 ||
        papacc_data_association_manager_consume(
            &manager, &issued, 1, &session_id) != PAPACC_RESULT_OK)
        return 9;
    papacc_data_association_manager_shutdown(&manager);
    papacc_test_set_ticket(&generator.values[generator.index], 90);
    ++generator.count;
    if (papacc_data_association_manager_init(
            &manager, entries, 2, &session_manager, &channel_manager,
            papacc_test_generate, &generator, 10) != PAPACC_RESULT_OK)
        return 10;
    if (papacc_data_association_manager_issue(&manager,
            session[0]->session_instance_id, (PAPACC_U64)(~(PAPACC_U64)0) - 5,
            &issued, &deadline) != PAPACC_RESULT_OK ||
        deadline != (PAPACC_U64)(~(PAPACC_U64)0) ||
        papacc_channel_manager_close(&channel_manager,
            channels[0].channel_instance_id) != PAPACC_RESULT_OK ||
        papacc_data_association_manager_expire(&manager, 0, &expired) !=
            PAPACC_RESULT_OK || expired != 1 || manager.count != 0)
        return 11;
    papacc_data_association_manager_shutdown(&manager);
    papacc_data_association_manager_shutdown(&manager);
    generator.result = PAPACC_RESULT_INTERNAL_ERROR;
    if (papacc_data_association_manager_init(
            &manager, entries, 2, &session_manager, &channel_manager,
            papacc_test_generate, &generator, 10) != PAPACC_RESULT_OK ||
        papacc_data_association_manager_issue(&manager,
            session[1]->session_instance_id, 1, &issued, &deadline) !=
            PAPACC_RESULT_INTERNAL_ERROR || manager.count != 0)
        return 12;
    papacc_data_association_manager_shutdown(&manager);
    generator.result = PAPACC_RESULT_OK;
    generator.count = generator.index;
    if (papacc_data_association_manager_init(
            &manager, entries, 2, &session_manager, &channel_manager,
            papacc_test_generate, &generator, 10) != PAPACC_RESULT_OK ||
        papacc_data_association_manager_issue(&manager,
            session[1]->session_instance_id, 1, &issued, &deadline) !=
            PAPACC_RESULT_INTERNAL_ERROR || manager.count != 0)
        return 13;
    papacc_data_association_manager_shutdown(&manager);
    if (manager.initialized != PAPACC_FALSE) return 14;
    papacc_channel_manager_shutdown(&channel_manager);
    papacc_session_manager_shutdown(&session_manager);
    papacc_connection_manager_shutdown(&connection_manager);
    return 0;
}
