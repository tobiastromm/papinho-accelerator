#include "tcp_listener_set_win32.h"

#include <ws2tcpip.h>

static PAPACC_RESULT papacc_test_socket_port(
    const PAPACC_TCP_SOCKET_WIN32 *socket_context,
    PAPACC_U16 *out_port)
{
    struct sockaddr_in address;
    int address_size = (int)sizeof(address);

    if (getsockname(
            socket_context->native_socket,
            (struct sockaddr *)&address,
            &address_size) == SOCKET_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    *out_port = (PAPACC_U16)ntohs(address.sin_port);
    return (*out_port != 0) ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT papacc_test_target_ipv4(
    PAPACC_BIND_TARGET *target,
    PAPACC_U8 last_byte,
    PAPACC_U32 interface_instance_id)
{
    PAPACC_BIND_TARGET empty_target = PAPACC_BIND_TARGET_INITIALIZER;

    *target = empty_target;
    target->interface_instance_id = interface_instance_id;
    return papacc_ip_address_set_ipv4(
        &target->address, 127, 0, 0, last_byte);
}

static PAPACC_BOOL papacc_test_entry_is_closed(
    const PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry)
{
    return (entry->socket.native_socket == INVALID_SOCKET &&
            entry->socket.is_open == PAPACC_FALSE &&
            entry->socket.is_bound == PAPACC_FALSE &&
            entry->socket.is_listening == PAPACC_FALSE)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

int main(void)
{
    static const PAPACC_U8 ipv6_loopback[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    };
    PAPACC_TCP_PLATFORM platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_PLATFORM stopped_platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_LISTENER_SET_WIN32 listener_set =
        PAPACC_TCP_LISTENER_SET_WIN32_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 storage[2] = {
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER,
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER
    };
    PAPACC_BIND_TARGET targets[2] = {
        PAPACC_BIND_TARGET_INITIALIZER,
        PAPACC_BIND_TARGET_INITIALIZER
    };
    PAPACC_TCP_SOCKET_WIN32 blocker = PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_U16 first_port;
    PAPACC_U16 second_port;
    PAPACC_U16 blocker_port;
    PAPACC_RESULT result;
    SOCKET original_socket;
    int test_result = 0;

    papacc_tcp_listener_set_win32_shutdown(NULL);
    papacc_tcp_listener_set_win32_shutdown(&listener_set);

    if (papacc_test_target_ipv4(&targets[0], 1, 101) != PAPACC_RESULT_OK ||
        papacc_test_target_ipv4(&targets[1], 2, 102) != PAPACC_RESULT_OK) {
        return 1;
    }
    if (papacc_tcp_listener_set_win32_start(
            &stopped_platform, targets, 1, 0, storage, 2,
            &listener_set) != PAPACC_RESULT_INVALID_STATE) {
        return 2;
    }
    if (papacc_tcp_platform_init(&platform) != PAPACC_RESULT_OK) {
        return 3;
    }

    if (papacc_tcp_listener_set_win32_start(
            &platform, targets, 2, 0, storage, 1,
            &listener_set) != PAPACC_RESULT_LIMIT_EXCEEDED ||
        listener_set.is_active != PAPACC_FALSE ||
        storage[0].socket.is_open != PAPACC_FALSE) {
        test_result = 4;
        goto cleanup;
    }

    if (papacc_tcp_listener_set_win32_start(
            &platform, targets, 1, 0, storage, 2,
            &listener_set) != PAPACC_RESULT_OK ||
        listener_set.entries != storage || listener_set.capacity != 2 ||
        listener_set.count != 1 ||
        listener_set.unsupported_target_count != 0 ||
        listener_set.bound_port == 0 ||
        listener_set.is_active != PAPACC_TRUE ||
        storage[0].socket.is_listening != PAPACC_TRUE ||
        storage[0].target.interface_instance_id != 101) {
        test_result = 5;
        goto cleanup;
    }
    if (papacc_test_socket_port(&storage[0].socket, &first_port) !=
            PAPACC_RESULT_OK ||
        first_port != listener_set.bound_port) {
        test_result = 6;
        goto cleanup;
    }

    original_socket = storage[0].socket.native_socket;
    if (papacc_tcp_listener_set_win32_start(
            &platform, targets, 1, 0, storage, 2,
            &listener_set) != PAPACC_RESULT_INVALID_STATE ||
        storage[0].socket.native_socket != original_socket ||
        listener_set.count != 1) {
        test_result = 7;
        goto cleanup;
    }

    papacc_tcp_listener_set_win32_shutdown(&listener_set);
    papacc_tcp_listener_set_win32_shutdown(&listener_set);
    if (listener_set.entries != NULL || listener_set.capacity != 0 ||
        listener_set.count != 0 || listener_set.bound_port != 0 ||
        listener_set.is_active != PAPACC_FALSE ||
        papacc_test_entry_is_closed(&storage[0]) != PAPACC_TRUE) {
        test_result = 8;
        goto cleanup;
    }

    if (papacc_tcp_listener_set_win32_start(
            &platform, targets, 2, 0, storage, 2,
            &listener_set) != PAPACC_RESULT_OK ||
        listener_set.count != 2 || listener_set.bound_port == 0 ||
        storage[0].socket.is_listening != PAPACC_TRUE ||
        storage[1].socket.is_listening != PAPACC_TRUE ||
        storage[0].target.interface_instance_id != 101 ||
        storage[1].target.interface_instance_id != 102) {
        test_result = 9;
        goto cleanup;
    }
    if (papacc_test_socket_port(&storage[0].socket, &first_port) !=
            PAPACC_RESULT_OK ||
        papacc_test_socket_port(&storage[1].socket, &second_port) !=
            PAPACC_RESULT_OK ||
        first_port != second_port || first_port != listener_set.bound_port) {
        test_result = 10;
        goto cleanup;
    }
    papacc_tcp_listener_set_win32_shutdown(&listener_set);

    if (papacc_tcp_socket_win32_bind(
            &platform, &targets[0], 0, &blocker) != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&blocker) != PAPACC_RESULT_OK ||
        papacc_test_socket_port(&blocker, &blocker_port) != PAPACC_RESULT_OK) {
        test_result = 11;
        goto cleanup;
    }

    if (papacc_tcp_listener_set_win32_start(
            &platform, &targets[1], 1, blocker_port, storage, 2,
            &listener_set) != PAPACC_RESULT_OK) {
        test_result = 12;
        goto cleanup;
    }
    papacc_tcp_listener_set_win32_shutdown(&listener_set);

    {
        PAPACC_BIND_TARGET rollback_targets[2];
        rollback_targets[0] = targets[1];
        rollback_targets[1] = targets[0];

        result = papacc_tcp_listener_set_win32_start(
            &platform, rollback_targets, 2, blocker_port, storage, 2,
            &listener_set);
        if (result != PAPACC_RESULT_INTERNAL_ERROR ||
            listener_set.is_active != PAPACC_FALSE ||
            listener_set.count != 0 ||
            papacc_test_entry_is_closed(&storage[0]) != PAPACC_TRUE ||
            papacc_test_entry_is_closed(&storage[1]) != PAPACC_TRUE) {
            test_result = 13;
            goto cleanup;
        }

        papacc_tcp_socket_win32_close(&blocker);
        if (papacc_tcp_listener_set_win32_start(
                &platform, rollback_targets, 2, blocker_port, storage, 2,
                &listener_set) != PAPACC_RESULT_OK ||
            listener_set.count != 2 ||
            listener_set.bound_port != blocker_port) {
            test_result = 14;
            goto cleanup;
        }
        papacc_tcp_listener_set_win32_shutdown(&listener_set);
    }

    if (papacc_ip_address_set_ipv6(
            &targets[0].address, ipv6_loopback) != PAPACC_RESULT_OK) {
        test_result = 15;
        goto cleanup;
    }
    targets[0].scope_id = 0;
    result = papacc_tcp_listener_set_win32_start(
        &platform, targets, 1, 0, storage, 2, &listener_set);
    if (result == PAPACC_RESULT_OK) {
        if (listener_set.count != 1 || listener_set.bound_port == 0 ||
            storage[0].socket.family != PAPACC_IP_FAMILY_IPV6 ||
            storage[0].socket.is_listening != PAPACC_TRUE) {
            test_result = 16;
            goto cleanup;
        }
    } else if (result != PAPACC_RESULT_NOT_SUPPORTED) {
        test_result = 17;
        goto cleanup;
    }

cleanup:
    papacc_tcp_listener_set_win32_shutdown(&listener_set);
    papacc_tcp_socket_win32_close(&blocker);
    papacc_tcp_platform_shutdown(&platform);
    return test_result;
}
