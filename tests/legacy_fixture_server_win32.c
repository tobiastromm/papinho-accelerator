#include "server_io_loop_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define LEGACY_FIXTURE_CAPACITY 8U

static int publish_ready(const char *path, PAPACC_U16 port,
    const char *stop_event_name)
{
    char temporary[MAX_PATH * 2];
    FILE *file = NULL;
    if (sprintf_s(temporary, sizeof(temporary), "%s.tmp.%lu", path,
            (unsigned long)GetCurrentProcessId()) < 0 ||
        fopen_s(&file, temporary, "wb") != 0 || file == NULL)
        return 0;
    if (fprintf(file,
            "fixture_version=1\nprofile=legacy\n"
            "CONTROL_host=127.0.0.1\nCONTROL_port=%u\n"
            "DATA_host=127.0.0.1\nDATA_port=%u\n"
            "transport=plaintext-tcp\n"
            "capability_policy_expectation=TLS_OFFLOAD\n"
            "denied_capabilities=NETWORK_EGRESS\nstop_event=%s\n",
            (unsigned int)port, (unsigned int)port, stop_event_name) < 0 ||
        fclose(file) != 0) {
        if (file != NULL) fclose(file);
        DeleteFileA(temporary);
        return 0;
    }
    if (!MoveFileExA(temporary, path,
            MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        DeleteFileA(temporary);
        return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_CONNECTION connections[LEGACY_FIXTURE_CAPACITY];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT contexts[
        LEGACY_FIXTURE_CAPACITY];
    PAPACC_SESSION sessions[LEGACY_FIXTURE_CAPACITY];
    PAPACC_CHANNEL channels[LEGACY_FIXTURE_CAPACITY];
    PAPACC_DATA_ASSOCIATION_ENTRY associations[LEGACY_FIXTURE_CAPACITY];
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 slots[LEGACY_FIXTURE_CAPACITY];
    PAPACC_SERVER_IO_LOOP_WIN32 loop = PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    struct sockaddr_in native_address;
    int native_length = sizeof(native_address);
    PAPACC_U16 port = 0U;
    PAPACC_SIZE index;
    HANDLE stop_event = NULL;
    int result = 1;

    if (argc != 4 || strcmp(argv[1], "--legacy-browser-fixture") != 0) {
        fprintf(stderr,
            "usage: %s --legacy-browser-fixture <ready-file> <stop-event>\n",
            argc > 0 ? argv[0] : "papacc_legacy_browser_fixture");
        return 2;
    }
    DeleteFileA(argv[2]);
    if (WSAStartup(MAKEWORD(2, 2), &winsock) != 0 ||
        papacc_tcp_platform_init(&network.tcp_platform) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&target.address, 127U, 0U, 0U, 1U) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(&network.tcp_platform, &target, 0U,
            &entry.socket) != PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&entry.socket) != PAPACC_RESULT_OK)
        goto cleanup;
    memset(&native_address, 0, sizeof(native_address));
    if (getsockname(entry.socket.native_socket,
            (struct sockaddr *)&native_address, &native_length) == SOCKET_ERROR)
        goto cleanup;
    port = (PAPACC_U16)ntohs(native_address.sin_port);
    entry.target = target;
    network.listener_set.entries = &entry;
    network.listener_set.capacity = network.listener_set.count = 1U;
    network.listener_set.bound_port = port;
    network.listener_set.is_active = PAPACC_TRUE;
    network.listener_storage = &entry;
    network.listener_storage_capacity = 1U;
    network.is_active = PAPACC_TRUE;
    for (index = 0U; index < LEGACY_FIXTURE_CAPACITY; ++index)
        contexts[index] = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
            PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    if (papacc_server_acceptor_win32_init(&acceptor, &network, connections,
            LEGACY_FIXTURE_CAPACITY, contexts, LEGACY_FIXTURE_CAPACITY) !=
            PAPACC_RESULT_OK ||
        papacc_server_io_loop_win32_init(&loop, &network, &acceptor, sessions,
            LEGACY_FIXTURE_CAPACITY, channels, LEGACY_FIXTURE_CAPACITY,
            associations, LEGACY_FIXTURE_CAPACITY, slots,
            LEGACY_FIXTURE_CAPACITY, 5000000000ULL) != PAPACC_RESULT_OK)
        goto cleanup;

    /* Accepted Legacy v1 policy: TLS offload may be negotiated, while
       NETWORK_EGRESS remains unavailable. This changes test composition only. */
    loop.enabled_capabilities.count = 0U;
    if (papacc_capability_set_insert(&loop.enabled_capabilities,
            &PAPACC_CAPABILITY_TLS_OFFLOAD_V1) != PAPACC_RESULT_OK)
        goto cleanup;
    stop_event = CreateEventA(NULL, TRUE, FALSE, argv[3]);
    if (stop_event == NULL || !publish_ready(argv[2], port, argv[3]))
        goto cleanup;
    while (WaitForSingleObject(stop_event, 0U) == WAIT_TIMEOUT) {
        if (papacc_server_io_loop_win32_poll_once(&loop, 25U) !=
                PAPACC_RESULT_OK)
            goto cleanup;
    }
    result = 0;

cleanup:
    DeleteFileA(argc > 2 ? argv[2] : "");
    if (stop_event != NULL) CloseHandle(stop_event);
    papacc_server_io_loop_win32_shutdown(&loop);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    papacc_tcp_socket_win32_close(&entry.socket);
    papacc_tcp_platform_shutdown(&network.tcp_platform);
    WSACleanup();
    return result;
}
