#include "server_io_loop_win32.h"

#include <string.h>
#include <ws2tcpip.h>

#define PAPACC_TEST_CAPACITY 4U

static const PAPACC_U8 papacc_test_open[20] = {
    0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x01, 0x00, 0x00
};
static const PAPACC_U8 papacc_test_accept[20] = {
    0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x01, 0x00, 0x00
};

static SOCKET papacc_test_connect(PAPACC_U16 port)
{
    struct sockaddr_in address;
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == INVALID_SOCKET) return INVALID_SOCKET;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(client, (const struct sockaddr *)&address,
                (int)sizeof(address)) == SOCKET_ERROR) {
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    return client;
}

static PAPACC_RESULT papacc_test_drive(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_SIZE passes)
{
    PAPACC_SIZE index;
    for (index = 0; index < passes; ++index) {
        PAPACC_RESULT result = papacc_server_io_loop_win32_poll_once(loop, 10U);
        if (result != PAPACC_RESULT_OK) return result;
    }
    return PAPACC_RESULT_OK;
}

int main(void)
{
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_SERVER_IO_LOOP_WIN32 loop = PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_CONNECTION connections[PAPACC_TEST_CAPACITY];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT
        contexts[PAPACC_TEST_CAPACITY];
    PAPACC_SESSION sessions[PAPACC_TEST_CAPACITY];
    PAPACC_CHANNEL channels[PAPACC_TEST_CAPACITY];
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32
        processors[PAPACC_TEST_CAPACITY];
    struct sockaddr_in native_address;
    int native_length = (int)sizeof(native_address);
    PAPACC_U16 port = 0;
    SOCKET idle = INVALID_SOCKET;
    SOCKET valid = INVALID_SOCKET;
    SOCKET malformed = INVALID_SOCKET;
    SOCKET truncated = INVALID_SOCKET;
    SOCKET rejected = INVALID_SOCKET;
    SOCKET established = INVALID_SOCKET;
    SOCKET session_fail = INVALID_SOCKET;
    SOCKET channel_fail = INVALID_SOCKET;
    SOCKET round_robin_a = INVALID_SOCKET;
    SOCKET round_robin_b = INVALID_SOCKET;
    PAPACC_U8 received[20];
    PAPACC_SIZE index;
    int result = 0;
    PAPACC_BOOL read_interest = PAPACC_FALSE;
    PAPACC_BOOL write_interest = PAPACC_FALSE;

    for (index = 0; index < PAPACC_TEST_CAPACITY; ++index)
        contexts[index] = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
            PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    if (papacc_tcp_platform_init(&network.tcp_platform) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(
            &network.tcp_platform, &target, 0, &entry.socket) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&entry.socket) != PAPACC_RESULT_OK ||
        getsockname(entry.socket.native_socket,
                    (struct sockaddr *)&native_address, &native_length) ==
            SOCKET_ERROR) return 1;
    port = (PAPACC_U16)ntohs(native_address.sin_port);
    entry.target = target;
    network.listener_set.entries = &entry;
    network.listener_set.capacity = 1;
    network.listener_set.count = 1;
    network.listener_set.bound_port = port;
    network.listener_set.is_active = PAPACC_TRUE;
    network.listener_storage = &entry;
    network.listener_storage_capacity = 1;
    network.is_active = PAPACC_TRUE;
    if (papacc_server_acceptor_win32_init(
            &acceptor, &network, connections, PAPACC_TEST_CAPACITY,
            contexts, PAPACC_TEST_CAPACITY) != PAPACC_RESULT_OK ||
        papacc_server_io_loop_win32_init(
            &loop, &network, &acceptor, sessions, PAPACC_TEST_CAPACITY,
            channels, PAPACC_TEST_CAPACITY, processors,
            PAPACC_TEST_CAPACITY, 1000000000ULL) != PAPACC_RESULT_OK) {
        result = 2;
        goto cleanup;
    }

    /* Dedicated processor-capacity exhaustion and reuse. */
    loop.processor_capacity = 1;
    idle = papacc_test_connect(port);
    if (idle == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK || loop.connection_manager->count != 1 ||
        papacc_server_io_loop_win32_processor_interest(
            &loop, 0, &read_interest, &write_interest) != PAPACC_RESULT_OK ||
        read_interest != PAPACC_TRUE || write_interest != PAPACC_FALSE) {
        result = 10;
        goto cleanup;
    }
    rejected = papacc_test_connect(port);
    if (rejected == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK || loop.connection_manager->count != 1 ||
        processors[0].in_use != PAPACC_TRUE) {
        result = 11;
        goto cleanup;
    }
    processors[0].processor.establishment_deadline_ns = 0;
    if (papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 0 ||
        processors[0].in_use != PAPACC_FALSE) {
        result = 12;
        goto cleanup;
    }
    (void)closesocket(idle);
    idle = INVALID_SOCKET;
    (void)closesocket(rejected);
    rejected = INVALID_SOCKET;
    established = papacc_test_connect(port);
    if (established == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK ||
        send(established, (const char *)papacc_test_open, 20, 0) != 20) {
        result = 13;
        goto cleanup;
    }
    for (index = 0; index < 8 && processors[0].processor.state !=
            PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT; ++index) {
        if (papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK) {
            result = 14;
            goto cleanup;
        }
    }
    if (papacc_server_io_loop_win32_processor_interest(
            &loop, 0, &read_interest, &write_interest) != PAPACC_RESULT_OK ||
        read_interest != PAPACC_FALSE || write_interest != PAPACC_TRUE ||
        papacc_test_drive(&loop, 4) != PAPACC_RESULT_OK ||
        recv(established, (char *)received, 20, MSG_WAITALL) != 20 ||
        memcmp(received, papacc_test_accept, 20) != 0 ||
        papacc_server_io_loop_win32_processor_interest(
            &loop, 0, &read_interest, &write_interest) != PAPACC_RESULT_OK ||
        read_interest != PAPACC_FALSE || write_interest != PAPACC_FALSE) {
        result = 15;
        goto cleanup;
    }
    loop.processor_capacity = PAPACC_TEST_CAPACITY;

    /* Dedicated Session capacity isolation through the real loop. */
    loop.session_manager.capacity = 1;
    session_fail = papacc_test_connect(port);
    if (session_fail == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK ||
        send(session_fail, (const char *)papacc_test_open, 20, 0) != 20 ||
        papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
        loop.session_manager.count != 1 || loop.channel_manager.count != 1 ||
        loop.connection_manager->count != 1 ||
        sessions[0].state != PAPACC_SESSION_STATE_ACTIVE) {
        result = 16;
        goto cleanup;
    }
    loop.session_manager.capacity = PAPACC_TEST_CAPACITY;

    /* Dedicated Channel capacity rollback: no orphan Session. */
    loop.channel_manager.capacity = 1;
    channel_fail = papacc_test_connect(port);
    if (channel_fail == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK ||
        send(channel_fail, (const char *)papacc_test_open, 20, 0) != 20 ||
        papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
        loop.session_manager.count != 1 || loop.channel_manager.count != 1 ||
        loop.connection_manager->count != 1) {
        result = 17;
        goto cleanup;
    }
    loop.channel_manager.capacity = PAPACC_TEST_CAPACITY;

    idle = papacc_test_connect(port);
    valid = papacc_test_connect(port);
    if (idle == INVALID_SOCKET || valid == INVALID_SOCKET ||
        papacc_test_drive(&loop, 3) != PAPACC_RESULT_OK ||
        send(valid, (const char *)papacc_test_open, 5, 0) != 5 ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(valid, (const char *)&papacc_test_open[5], 7, 0) != 7 ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(valid, (const char *)&papacc_test_open[12], 8, 0) != 8 ||
        papacc_test_drive(&loop, 10) != PAPACC_RESULT_OK ||
        recv(valid, (char *)received, 20, MSG_WAITALL) != 20 ||
        memcmp(received, papacc_test_accept, 20) != 0 ||
        loop.session_manager.count != 2 || loop.channel_manager.count != 2 ||
        loop.connection_manager->count != 3 ||
        sessions[0].state != PAPACC_SESSION_STATE_ACTIVE) {
        result = 3;
        goto cleanup;
    }
    malformed = papacc_test_connect(port);
    if (malformed == INVALID_SOCKET ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(malformed, "BAD!", 4, 0) != 4 ||
        send(malformed, (const char *)&papacc_test_open[4], 16, 0) != 16 ||
        papacc_test_drive(&loop, 4) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 3 ||
        loop.session_manager.count != 2 || loop.channel_manager.count != 2) {
        result = 4;
        goto cleanup;
    }
    for (index = 0; index < PAPACC_TEST_CAPACITY; ++index) {
        if (processors[index].in_use == PAPACC_TRUE &&
            papacc_control_processor_is_established(
                &processors[index].processor) != PAPACC_TRUE)
            processors[index].processor.establishment_deadline_ns = 0;
    }
    if (papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 2) {
        result = 5;
        goto cleanup;
    }
    truncated = papacc_test_connect(port);
    if (truncated == INVALID_SOCKET ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(truncated, (const char *)papacc_test_open, 8, 0) != 8 ||
        shutdown(truncated, SD_SEND) != 0 ||
        papacc_test_drive(&loop, 5) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 2 ||
        loop.session_manager.count != 2 || loop.channel_manager.count != 2) {
        result = 6;
        goto cleanup;
    }
    {
        PAPACC_SIZE first_order[PAPACC_TEST_CAPACITY];
        PAPACC_SIZE second_order[PAPACC_TEST_CAPACITY];
        PAPACC_SIZE first_ready_position = PAPACC_TEST_CAPACITY;
        PAPACC_SIZE second_ready_position = PAPACC_TEST_CAPACITY;
        round_robin_a = papacc_test_connect(port);
        round_robin_b = papacc_test_connect(port);
        if (round_robin_a == INVALID_SOCKET ||
            round_robin_b == INVALID_SOCKET ||
            papacc_test_drive(&loop, 3) != PAPACC_RESULT_OK ||
            loop.connection_manager->count != 4 ||
            send(round_robin_a, "a", 1, 0) != 1 ||
            send(round_robin_b, "b", 1, 0) != 1) {
            result = 18;
            goto cleanup;
        }
        for (index = 0; index < PAPACC_TEST_CAPACITY; ++index) {
            if (papacc_server_io_loop_win32_processor_scan_index(
                    &loop, index, &first_order[index]) != PAPACC_RESULT_OK) {
                result = 19;
                goto cleanup;
            }
            if (papacc_control_processor_is_established(
                    &processors[first_order[index]].processor) != PAPACC_TRUE) {
                if (first_ready_position == PAPACC_TEST_CAPACITY)
                    first_ready_position = index;
                else if (second_ready_position == PAPACC_TEST_CAPACITY)
                    second_ready_position = index;
            }
        }
        if (first_ready_position == PAPACC_TEST_CAPACITY ||
            second_ready_position == PAPACC_TEST_CAPACITY ||
            papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK) {
            result = 20;
            goto cleanup;
        }
        for (index = 0; index < PAPACC_TEST_CAPACITY; ++index) {
            if (papacc_server_io_loop_win32_processor_scan_index(
                    &loop, index, &second_order[index]) != PAPACC_RESULT_OK ||
                second_order[index] !=
                    first_order[(index + 1U) % PAPACC_TEST_CAPACITY]) {
                result = 21;
                goto cleanup;
            }
        }
        (void)shutdown(round_robin_a, SD_SEND);
        (void)shutdown(round_robin_b, SD_SEND);
        if (papacc_test_drive(&loop, 4) != PAPACC_RESULT_OK ||
            loop.connection_manager->count != 2) {
            result = 22;
            goto cleanup;
        }
    }
    {
        PAPACC_SIZE saved_count = network.listener_set.count;
        PAPACC_SIZE saved_capacity = network.listener_set.capacity;
        PAPACC_SIZE connection_count = loop.connection_manager->count;
        PAPACC_SIZE session_count = loop.session_manager.count;
        PAPACC_SIZE channel_count = loop.channel_manager.count;
        network.listener_set.count = (PAPACC_SIZE)FD_SETSIZE + 1U;
        network.listener_set.capacity = (PAPACC_SIZE)FD_SETSIZE + 1U;
        if (papacc_server_io_loop_win32_poll_once(&loop, 0) !=
                PAPACC_RESULT_LIMIT_EXCEEDED ||
            loop.connection_manager->count != connection_count ||
            loop.session_manager.count != session_count ||
            loop.channel_manager.count != channel_count) {
            result = 23;
        }
        network.listener_set.count = saved_count;
        network.listener_set.capacity = saved_capacity;
    }

cleanup:
    papacc_server_io_loop_win32_shutdown(&loop);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    if (round_robin_b != INVALID_SOCKET) (void)closesocket(round_robin_b);
    if (round_robin_a != INVALID_SOCKET) (void)closesocket(round_robin_a);
    if (channel_fail != INVALID_SOCKET) (void)closesocket(channel_fail);
    if (session_fail != INVALID_SOCKET) (void)closesocket(session_fail);
    if (established != INVALID_SOCKET) (void)closesocket(established);
    if (rejected != INVALID_SOCKET) (void)closesocket(rejected);
    if (truncated != INVALID_SOCKET) (void)closesocket(truncated);
    if (malformed != INVALID_SOCKET) (void)closesocket(malformed);
    if (valid != INVALID_SOCKET) (void)closesocket(valid);
    if (idle != INVALID_SOCKET) (void)closesocket(idle);
    papacc_tcp_socket_win32_close(&entry.socket);
    papacc_tcp_platform_shutdown(&network.tcp_platform);
    return result;
}
