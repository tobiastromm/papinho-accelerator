#include "tcp_socket_win32.h"

#include <ws2tcpip.h>
#include <string.h>

static int papacc_test_bound_socket(
    PAPACC_TCP_SOCKET_WIN32 *socket_context,
    PAPACC_IP_FAMILY expected_family)
{
    int exclusive = 0;
    int exclusive_size = (int)sizeof(exclusive);

    if (socket_context->is_open != PAPACC_TRUE ||
        socket_context->is_bound != PAPACC_TRUE ||
        socket_context->family != expected_family ||
        socket_context->native_socket == INVALID_SOCKET) {
        return 1;
    }

    if (getsockopt(
            socket_context->native_socket,
            SOL_SOCKET,
            SO_EXCLUSIVEADDRUSE,
            (char *)&exclusive,
            &exclusive_size) == SOCKET_ERROR ||
        exclusive == 0) {
        return 2;
    }

    if (expected_family == PAPACC_IP_FAMILY_IPV4) {
        struct sockaddr_in address;
        int address_size = (int)sizeof(address);

        if (getsockname(
                socket_context->native_socket,
                (struct sockaddr *)&address,
                &address_size) == SOCKET_ERROR ||
            address.sin_port == 0) {
            return 3;
        }
    } else {
        struct sockaddr_in6 address;
        int address_size = (int)sizeof(address);

        if (getsockname(
                socket_context->native_socket,
                (struct sockaddr *)&address,
                &address_size) == SOCKET_ERROR ||
            address.sin6_port == 0) {
            return 4;
        }
    }

    return 0;
}

int main(void)
{
    static const PAPACC_U8 ipv6_loopback[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
    };
    PAPACC_TCP_PLATFORM platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_PLATFORM stopped_platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_SOCKET_WIN32 socket_context =
        PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_RESULT result;
    SOCKET original_socket;
    int test_result = 0;

    papacc_tcp_socket_win32_close(NULL);
    papacc_tcp_socket_win32_close(&socket_context);

    if (papacc_ip_address_set_ipv4(
            &target.address, 127, 0, 0, 1) != PAPACC_RESULT_OK) {
        return 1;
    }
    if (papacc_tcp_socket_win32_bind(
            &stopped_platform, &target, 0, &socket_context) !=
            PAPACC_RESULT_INVALID_STATE ||
        socket_context.is_open != PAPACC_FALSE) {
        return 2;
    }

    target.address.family = PAPACC_IP_FAMILY_UNSPECIFIED;
    if (papacc_tcp_platform_init(&platform) != PAPACC_RESULT_OK) {
        return 3;
    }
    if (papacc_tcp_socket_win32_bind(
            &platform, &target, 0, &socket_context) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        test_result = 4;
        goto cleanup;
    }

    if (papacc_ip_address_set_ipv4(
            &target.address, 127, 0, 0, 1) != PAPACC_RESULT_OK) {
        test_result = 5;
        goto cleanup;
    }
    target.scope_id = 1;
    if (papacc_tcp_socket_win32_bind(
            &platform, &target, 0, &socket_context) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        test_result = 6;
        goto cleanup;
    }
    target.scope_id = 0;

    if (papacc_tcp_socket_win32_bind(
            &platform, &target, 0, &socket_context) != PAPACC_RESULT_OK ||
        papacc_test_bound_socket(
            &socket_context, PAPACC_IP_FAMILY_IPV4) != 0) {
        test_result = 7;
        goto cleanup;
    }

    original_socket = socket_context.native_socket;
    if (papacc_tcp_socket_win32_bind(
            &platform, &target, 0, &socket_context) !=
            PAPACC_RESULT_INVALID_STATE ||
        socket_context.native_socket != original_socket) {
        test_result = 8;
        goto cleanup;
    }

    papacc_tcp_socket_win32_close(&socket_context);
    papacc_tcp_socket_win32_close(&socket_context);
    if (socket_context.native_socket != INVALID_SOCKET ||
        socket_context.is_open != PAPACC_FALSE ||
        socket_context.is_bound != PAPACC_FALSE ||
        socket_context.family != PAPACC_IP_FAMILY_UNSPECIFIED) {
        test_result = 9;
        goto cleanup;
    }

    memset(target.address.bytes, 0, sizeof(target.address.bytes));
    target.address.family = PAPACC_IP_FAMILY_IPV4;
    if (papacc_tcp_socket_win32_bind(
            &platform, &target, 0, &socket_context) != PAPACC_RESULT_OK) {
        test_result = 10;
        goto cleanup;
    }
    papacc_tcp_socket_win32_close(&socket_context);

    if (papacc_ip_address_set_ipv6(
            &target.address, ipv6_loopback) != PAPACC_RESULT_OK) {
        test_result = 11;
        goto cleanup;
    }
    result = papacc_tcp_socket_win32_bind(
        &platform, &target, 0, &socket_context);
    if (result == PAPACC_RESULT_OK) {
        if (papacc_test_bound_socket(
                &socket_context, PAPACC_IP_FAMILY_IPV6) != 0) {
            test_result = 12;
            goto cleanup;
        }
        papacc_tcp_socket_win32_close(&socket_context);
    } else if (result != PAPACC_RESULT_NOT_SUPPORTED) {
        test_result = 13;
        goto cleanup;
    }

    if (papacc_ip_address_set_ipv4(
            &target.address, 127, 0, 0, 1) != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(
            &platform, &target, 0, &socket_context) != PAPACC_RESULT_OK) {
        test_result = 14;
    }

cleanup:
    papacc_tcp_socket_win32_close(&socket_context);
    papacc_tcp_platform_shutdown(&platform);
    return test_result;
}
