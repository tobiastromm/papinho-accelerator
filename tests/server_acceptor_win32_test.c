#include "server_acceptor_win32.h"

#include <ws2tcpip.h>

#define PAPACC_TEST_LISTENER_COUNT 2

typedef struct PAPACC_TEST_NETWORK_FIXTURE {
    PAPACC_SERVER_NETWORK network;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entries[PAPACC_TEST_LISTENER_COUNT];
    PAPACC_U16 ports[PAPACC_TEST_LISTENER_COUNT];
} PAPACC_TEST_NETWORK_FIXTURE;

static PAPACC_RESULT papacc_test_fixture_start(
    PAPACC_TEST_NETWORK_FIXTURE *fixture)
{
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SIZE index;

    fixture->network = (PAPACC_SERVER_NETWORK)
        PAPACC_SERVER_NETWORK_INITIALIZER;
    for (index = 0; index < PAPACC_TEST_LISTENER_COUNT; ++index) {
        fixture->entries[index] = (PAPACC_TCP_LISTENER_ENTRY_WIN32)
            PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
        fixture->ports[index] = 0;
    }
    if (papacc_tcp_platform_init(&fixture->network.tcp_platform) !=
            PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1) !=
            PAPACC_RESULT_OK) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    for (index = 0; index < PAPACC_TEST_LISTENER_COUNT; ++index) {
        struct sockaddr_in address;
        int address_length = (int)sizeof(address);
        if (papacc_tcp_socket_win32_bind(
                &fixture->network.tcp_platform, &target, 0,
                &fixture->entries[index].socket) != PAPACC_RESULT_OK ||
            papacc_tcp_socket_win32_listen(
                &fixture->entries[index].socket) != PAPACC_RESULT_OK ||
            getsockname(
                fixture->entries[index].socket.native_socket,
                (struct sockaddr *)&address, &address_length) == SOCKET_ERROR) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        fixture->ports[index] = (PAPACC_U16)ntohs(address.sin_port);
        fixture->entries[index].target = target;
    }
    fixture->network.listener_set.entries = fixture->entries;
    fixture->network.listener_set.capacity = PAPACC_TEST_LISTENER_COUNT;
    fixture->network.listener_set.count = PAPACC_TEST_LISTENER_COUNT;
    fixture->network.listener_set.is_active = PAPACC_TRUE;
    fixture->network.listener_storage = fixture->entries;
    fixture->network.listener_storage_capacity = PAPACC_TEST_LISTENER_COUNT;
    fixture->network.is_active = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

static void papacc_test_fixture_shutdown(PAPACC_TEST_NETWORK_FIXTURE *fixture)
{
    PAPACC_SIZE index;
    for (index = 0; index < PAPACC_TEST_LISTENER_COUNT; ++index) {
        papacc_tcp_socket_win32_close(&fixture->entries[index].socket);
    }
    papacc_tcp_platform_shutdown(&fixture->network.tcp_platform);
    fixture->network = (PAPACC_SERVER_NETWORK)
        PAPACC_SERVER_NETWORK_INITIALIZER;
}

static SOCKET papacc_test_connect(
    PAPACC_U16 port,
    PAPACC_NETWORK_ENDPOINT *out_client_endpoint)
{
    struct sockaddr_in address;
    int address_length;
    const PAPACC_U8 *bytes;
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (client == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(client, (const struct sockaddr *)&address,
                (int)sizeof(address)) == SOCKET_ERROR) {
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    address_length = (int)sizeof(address);
    if (getsockname(client, (struct sockaddr *)&address, &address_length) ==
        SOCKET_ERROR) {
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    *out_client_endpoint = (PAPACC_NETWORK_ENDPOINT)
        PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    bytes = (const PAPACC_U8 *)&address.sin_addr;
    if (papacc_ip_address_set_ipv4(
            &out_client_endpoint->address,
            bytes[0], bytes[1], bytes[2], bytes[3]) != PAPACC_RESULT_OK) {
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    out_client_endpoint->port = (PAPACC_U16)ntohs(address.sin_port);
    return client;
}

static int papacc_test_init_timeout_and_guards(
    PAPACC_TEST_NETWORK_FIXTURE *fixture)
{
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_SERVER_NETWORK inactive = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_CONNECTION storage[1];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT contexts[1] = {
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER
    };
    PAPACC_CONNECTION *connection = (PAPACC_CONNECTION *)storage;
    PAPACC_SIZE saved_count;
    PAPACC_SIZE saved_capacity;

    papacc_server_acceptor_win32_shutdown(NULL);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    if (papacc_server_acceptor_win32_init(
            NULL, &fixture->network, storage, 1, contexts, 1) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_acceptor_win32_init(
            &acceptor, NULL, storage, 1, contexts, 1) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_acceptor_win32_init(
            &acceptor, &inactive, storage, 1, contexts, 1) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, NULL, 1, contexts, 1) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, storage, 1, NULL, 1) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, storage, 1, contexts, 0) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 1;
    }
    if (papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, storage, 1, contexts, 1) !=
            PAPACC_RESULT_OK ||
        acceptor.initialized != PAPACC_TRUE ||
        acceptor.connection_manager.count != 0 ||
        papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, storage, 1, contexts, 1) !=
            PAPACC_RESULT_INVALID_STATE) {
        return 2;
    }
    if (papacc_server_acceptor_win32_poll_once(
            &acceptor, 0, &connection) != PAPACC_RESULT_OK ||
        connection != NULL ||
        papacc_server_acceptor_win32_poll_once(
            &acceptor, 10, &connection) != PAPACC_RESULT_OK ||
        connection != NULL) {
        return 3;
    }
    saved_count = fixture->network.listener_set.count;
    saved_capacity = fixture->network.listener_set.capacity;
    fixture->network.listener_set.count = (PAPACC_SIZE)FD_SETSIZE + 1U;
    fixture->network.listener_set.capacity = (PAPACC_SIZE)FD_SETSIZE + 1U;
    if (papacc_server_acceptor_win32_poll_once(
            &acceptor, 0, &connection) != PAPACC_RESULT_LIMIT_EXCEEDED ||
        connection != NULL) {
        return 4;
    }
    fixture->network.listener_set.count = saved_count;
    fixture->network.listener_set.capacity = saved_capacity;
    fixture->network.is_active = PAPACC_FALSE;
    if (papacc_server_acceptor_win32_poll_once(
            &acceptor, 0, &connection) != PAPACC_RESULT_INVALID_STATE) {
        return 5;
    }
    fixture->network.is_active = PAPACC_TRUE;
    papacc_server_acceptor_win32_shutdown(&acceptor);
    if (papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, NULL, 0, NULL, 0) !=
            PAPACC_RESULT_OK) {
        return 6;
    }
    papacc_server_acceptor_win32_shutdown(&acceptor);
    return 0;
}

static int papacc_test_round_robin_and_shutdown(
    PAPACC_TEST_NETWORK_FIXTURE *fixture)
{
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_CONNECTION storage[2];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT contexts[2] = {
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER,
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER
    };
    PAPACC_NETWORK_ENDPOINT client_endpoints[2];
    PAPACC_CONNECTION *connections[2] = { NULL, NULL };
    SOCKET clients[2] = { INVALID_SOCKET, INVALID_SOCKET };
    int result = 0;

    if (papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, storage, 2, contexts, 2) !=
        PAPACC_RESULT_OK) {
        return 10;
    }
    clients[0] = papacc_test_connect(
        fixture->ports[0], &client_endpoints[0]);
    clients[1] = papacc_test_connect(
        fixture->ports[1], &client_endpoints[1]);
    if (clients[0] == INVALID_SOCKET || clients[1] == INVALID_SOCKET ||
        papacc_server_acceptor_win32_poll_once(
            &acceptor, 100, &connections[0]) != PAPACC_RESULT_OK ||
        papacc_server_acceptor_win32_poll_once(
            &acceptor, 100, &connections[1]) != PAPACC_RESULT_OK) {
        result = 11;
        goto cleanup;
    }
    if (connections[0] == NULL || connections[1] == NULL ||
        connections[0]->state != PAPACC_CONNECTION_STATE_PENDING ||
        connections[1]->state != PAPACC_CONNECTION_STATE_PENDING ||
        connections[0]->connection_instance_id == 0 ||
        connections[1]->connection_instance_id == 0 ||
        connections[0]->connection_instance_id ==
            connections[1]->connection_instance_id ||
        connections[0]->local_endpoint.port != fixture->ports[0] ||
        connections[1]->local_endpoint.port != fixture->ports[1] ||
        papacc_network_endpoint_equal(
            &connections[0]->remote_endpoint, &client_endpoints[0]) !=
            PAPACC_TRUE ||
        papacc_network_endpoint_equal(
            &connections[1]->remote_endpoint, &client_endpoints[1]) !=
            PAPACC_TRUE ||
        acceptor.connection_manager.count != 2 ||
        fixture->entries[0].socket.is_listening != PAPACC_TRUE ||
        fixture->entries[1].socket.is_listening != PAPACC_TRUE) {
        result = 12;
        goto cleanup;
    }
    papacc_server_acceptor_win32_shutdown(&acceptor);
    if (acceptor.initialized != PAPACC_FALSE ||
        contexts[0].owns_socket != PAPACC_FALSE ||
        contexts[1].owns_socket != PAPACC_FALSE ||
        fixture->network.is_active != PAPACC_TRUE ||
        fixture->entries[0].socket.is_listening != PAPACC_TRUE ||
        fixture->entries[1].socket.is_listening != PAPACC_TRUE ||
        papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, storage, 2, contexts, 2) !=
            PAPACC_RESULT_OK) {
        result = 13;
    }

cleanup:
    papacc_server_acceptor_win32_shutdown(&acceptor);
    if (clients[0] != INVALID_SOCKET) {
        (void)closesocket(clients[0]);
    }
    if (clients[1] != INVALID_SOCKET) {
        (void)closesocket(clients[1]);
    }
    return result;
}

static int papacc_test_capacity_recovery(
    PAPACC_TEST_NETWORK_FIXTURE *fixture)
{
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_CONNECTION storage[1];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT contexts[1] = {
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER
    };
    PAPACC_NETWORK_ENDPOINT client_endpoint;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_U64 first_id;
    SOCKET client = INVALID_SOCKET;
    int result = 0;

    if (papacc_server_acceptor_win32_init(
            &acceptor, &fixture->network, storage, 1, contexts, 1) !=
        PAPACC_RESULT_OK) {
        return 20;
    }
    client = papacc_test_connect(fixture->ports[0], &client_endpoint);
    if (client == INVALID_SOCKET ||
        papacc_server_acceptor_win32_poll_once(
            &acceptor, 100, &connection) != PAPACC_RESULT_OK ||
        connection == NULL) {
        result = 21;
        goto cleanup;
    }
    first_id = connection->connection_instance_id;
    (void)closesocket(client);
    client = papacc_test_connect(fixture->ports[0], &client_endpoint);
    connection = (PAPACC_CONNECTION *)storage;
    if (client == INVALID_SOCKET ||
        papacc_server_acceptor_win32_poll_once(
            &acceptor, 100, &connection) != PAPACC_RESULT_LIMIT_EXCEEDED ||
        connection != NULL || acceptor.connection_manager.count != 1 ||
        storage[0].connection_instance_id != first_id ||
        storage[0].state != PAPACC_CONNECTION_STATE_PENDING) {
        result = 22;
        goto cleanup;
    }
    (void)closesocket(client);
    client = INVALID_SOCKET;
    if (papacc_connection_manager_remove(
            &acceptor.connection_manager, first_id) != PAPACC_RESULT_OK ||
        contexts[0].owns_socket != PAPACC_FALSE) {
        result = 23;
        goto cleanup;
    }
    client = papacc_test_connect(fixture->ports[0], &client_endpoint);
    if (client == INVALID_SOCKET ||
        papacc_server_acceptor_win32_poll_once(
            &acceptor, 100, &connection) != PAPACC_RESULT_OK ||
        connection == NULL || connection->connection_instance_id == first_id ||
        contexts[0].owns_socket != PAPACC_TRUE) {
        result = 24;
    }

cleanup:
    papacc_server_acceptor_win32_shutdown(&acceptor);
    if (client != INVALID_SOCKET) {
        (void)closesocket(client);
    }
    return result;
}

int main(void)
{
    PAPACC_TEST_NETWORK_FIXTURE fixture;
    int result;

    if (papacc_test_fixture_start(&fixture) != PAPACC_RESULT_OK) {
        papacc_test_fixture_shutdown(&fixture);
        return 30;
    }
    result = papacc_test_init_timeout_and_guards(&fixture);
    if (result == 0) {
        result = papacc_test_round_robin_and_shutdown(&fixture);
    }
    if (result == 0) {
        result = papacc_test_capacity_recovery(&fixture);
    }
    papacc_test_fixture_shutdown(&fixture);
    return result;
}
