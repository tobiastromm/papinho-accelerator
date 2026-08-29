#include "server_run_win32.h"

#include <fcntl.h>
#include <io.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <ws2tcpip.h>

typedef struct PAPACC_TEST_RUN_CONTEXT {
    PAPACC_SERVER_NETWORK *network;
    FILE *output;
    PAPACC_LOGGER *logger;
    PAPACC_RESULT result;
} PAPACC_TEST_RUN_CONTEXT;

typedef struct PAPACC_TEST_LOG_CAPTURE {
    char text[4096];
    PAPACC_SIZE length;
} PAPACC_TEST_LOG_CAPTURE;

static void papacc_test_log_sink(
    void *context, const PAPACC_LOG_RECORD *record)
{
    PAPACC_TEST_LOG_CAPTURE *capture = (PAPACC_TEST_LOG_CAPTURE *)context;
    PAPACC_SIZE available = sizeof(capture->text) - capture->length;
    int count;
    if (available <= 1) return;
    count = snprintf(&capture->text[capture->length], available,
                     "%s: %s\n", record->component, record->message);
    if (count > 0 && (PAPACC_SIZE)count < available)
        capture->length += (PAPACC_SIZE)count;
}

static DWORD WINAPI papacc_test_run_thread(void *opaque_context)
{
    PAPACC_TEST_RUN_CONTEXT *context =
        (PAPACC_TEST_RUN_CONTEXT *)opaque_context;
    context->result = papacc_server_run_win32(
        context->network, context->output, context->output, context->logger);
    return 0;
}

static PAPACC_RESULT papacc_test_network_start(
    PAPACC_SERVER_NETWORK *network,
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry,
    PAPACC_U16 *out_port)
{
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    struct sockaddr_in address;
    int address_length = (int)sizeof(address);

    *network = (PAPACC_SERVER_NETWORK)PAPACC_SERVER_NETWORK_INITIALIZER;
    *entry = (PAPACC_TCP_LISTENER_ENTRY_WIN32)
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    if (papacc_tcp_platform_init(&network->tcp_platform) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(
            &network->tcp_platform, &target, 0, &entry->socket) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&entry->socket) != PAPACC_RESULT_OK ||
        getsockname(entry->socket.native_socket,
                    (struct sockaddr *)&address,
                    &address_length) == SOCKET_ERROR) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    entry->target = target;
    *out_port = (PAPACC_U16)ntohs(address.sin_port);
    network->listener_set.entries = entry;
    network->listener_set.capacity = 1;
    network->listener_set.count = 1;
    network->listener_set.bound_port = *out_port;
    network->listener_set.is_active = PAPACC_TRUE;
    network->listener_storage = entry;
    network->listener_storage_capacity = 1;
    network->is_active = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

static void papacc_test_network_shutdown(
    PAPACC_SERVER_NETWORK *network,
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry)
{
    papacc_tcp_socket_win32_close(&entry->socket);
    papacc_tcp_platform_shutdown(&network->tcp_platform);
    *network = (PAPACC_SERVER_NETWORK)PAPACC_SERVER_NETWORK_INITIALIZER;
}

static SOCKET papacc_test_connect(PAPACC_U16 port)
{
    struct sockaddr_in address;
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
    return client;
}

int main(void)
{
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    PAPACC_SERVER_CONSOLE_WIN32 console =
        PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER;
    PAPACC_LOGGER logger;
    PAPACC_TEST_LOG_CAPTURE log_capture = { { 0 }, 0 };
    PAPACC_TEST_RUN_CONTEXT run_context;
    PAPACC_U16 port = 0;
    HANDLE read_handle = NULL;
    HANDLE write_handle = NULL;
    HANDLE thread = NULL;
    FILE *output = NULL;
    SOCKET client = INVALID_SOCKET;
    SOCKET data_client = INVALID_SOCKET;
    static const unsigned char control_open[20] = {
        0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
        0x00, 0x01, 0x00, 0x00
    };
    static const unsigned char control_accept[20] = {
        0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
        0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
        0x00, 0x01, 0x00, 0x00
    };
    static const unsigned char ticket_request[16] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,3,0,0,0,0,0,0 };
    static const unsigned char data_accept[16] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,6,0,0,0,0,0,0 };
    char captured[4096];
    DWORD captured_size = 0;
    int output_fd = -1;
    int result = 0;

    captured[0] = '\0';
    if (papacc_test_network_start(&network, &entry, &port) !=
            PAPACC_RESULT_OK ||
        papacc_server_console_win32_install(&console) != PAPACC_RESULT_OK ||
        CreatePipe(&read_handle, &write_handle, NULL, 0) == FALSE) {
        result = 1;
        goto cleanup;
    }
    output_fd = _open_osfhandle((intptr_t)write_handle, _O_TEXT);
    if (output_fd == -1) {
        result = 2;
        goto cleanup;
    }
    write_handle = NULL;
    output = _fdopen(output_fd, "w");
    if (output == NULL) {
        result = 3;
        goto cleanup;
    }
    output_fd = -1;
    if (papacc_logger_init(&logger, papacc_test_log_sink, &log_capture,
                           PAPACC_LOG_INFO) != PAPACC_RESULT_OK) {
        result = 31;
        goto cleanup;
    }
    run_context.network = &network;
    run_context.output = output;
    run_context.logger = &logger;
    run_context.result = PAPACC_RESULT_INTERNAL_ERROR;
    thread = CreateThread(
        NULL, 0, papacc_test_run_thread, &run_context, 0, NULL);
    if (thread == NULL) {
        result = 4;
        goto cleanup;
    }
    client = papacc_test_connect(port);
    if (client == INVALID_SOCKET) {
        result = 5;
        goto cleanup;
    }
    {
        ULONGLONG deadline = GetTickCount64() + 3000U;
        while (strstr(captured, "Accepting Control establishment") == NULL &&
               GetTickCount64() < deadline) {
            DWORD available = 0;
            if (PeekNamedPipe(
                    read_handle, NULL, 0, NULL, &available, NULL) == FALSE) {
                result = 6;
                break;
            }
            if (available > 0 && captured_size < sizeof(captured) - 1U) {
                DWORD remaining =
                    (DWORD)(sizeof(captured) - 1U - captured_size);
                DWORD requested = available < remaining ? available : remaining;
                DWORD read_size = 0;
                if (ReadFile(read_handle, captured + captured_size, requested,
                             &read_size, NULL) == FALSE) {
                    result = 7;
                    break;
                }
                captured_size += read_size;
                captured[captured_size] = '\0';
            } else {
                Sleep(10);
            }
        }
    }
    if (result == 0 && strstr(captured,
            "Accepting Control establishment connections.") == NULL) {
        result = 8;
    }
    if (result == 0) {
        unsigned char received[20];
        int total = 0;
        if (send(client, (const char *)control_open,
                 (int)sizeof(control_open), 0) != (int)sizeof(control_open)) {
            result = 14;
        }
        while (result == 0 && total < (int)sizeof(received)) {
            int count = recv(client, (char *)&received[total],
                             (int)sizeof(received) - total, 0);
            if (count <= 0) result = 15;
            else total += count;
        }
        if (result == 0 && memcmp(
                received, control_accept, sizeof(control_accept)) != 0)
            result = 16;
    }
    if (result == 0) {
        unsigned char ticket_frame[32];
        unsigned char attach_frame[32] = {
            0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
        unsigned char received_accept[16];
        int total = 0;
        if (send(client, (const char *)ticket_request, 16, 0) != 16)
            result = 17;
        while (result == 0 && total < 32) {
            int count = recv(client, (char *)&ticket_frame[total], 32-total, 0);
            if (count <= 0) result = 18; else total += count;
        }
        if (result == 0 && (ticket_frame[8] != 0 || ticket_frame[9] != 4 ||
            ticket_frame[15] != 16)) result = 19;
        if (result == 0) {
            memcpy(&attach_frame[16], &ticket_frame[16], 16);
            data_client = papacc_test_connect(port);
            if (data_client == INVALID_SOCKET ||
                send(data_client, (const char *)attach_frame, 32, 0) != 32)
                result = 20;
        }
        total = 0;
        while (result == 0 && total < 16) {
            int count = recv(data_client, (char *)&received_accept[total],
                             16-total, 0);
            if (count <= 0) result = 21; else total += count;
        }
        if (result == 0 && memcmp(received_accept, data_accept, 16) != 0)
            result = 22;
    }
    papacc_server_console_win32_request_stop();
    if (thread != NULL && WaitForSingleObject(thread, 3000) != WAIT_OBJECT_0) {
        result = 9;
    }
    if (result == 0 && run_context.result != PAPACC_RESULT_OK) {
        result = 10;
    }
    if (result == 0 &&
        (strstr(log_capture.text, "Listener started") == NULL ||
         strstr(log_capture.text, "CONTROL accepted") == NULL ||
         strstr(log_capture.text, "from 127.0.0.1:") == NULL ||
         strstr(log_capture.text, "Session 1 CONTROL established") == NULL ||
         strstr(log_capture.text, "Session 1 DATA ticket issued") == NULL ||
         strstr(log_capture.text, "DATA connection accepted") == NULL ||
         strstr(log_capture.text, "Session 1 DATA successfully attached") == NULL ||
         strstr(log_capture.text, "Server stopping") == NULL)) {
        result = 32;
    }
    if (result == 0) {
        fd_set read_set;
        struct timeval timeout;
        char byte;
        int receive_result;
        FD_ZERO(&read_set);
        FD_SET(client, &read_set);
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        if (select(0, &read_set, NULL, NULL, &timeout) != 1) {
            result = 11;
        } else {
            receive_result = recv(client, &byte, 1, 0);
            if (receive_result > 0 ||
                (receive_result == SOCKET_ERROR &&
                 WSAGetLastError() != WSAECONNRESET &&
                 WSAGetLastError() != WSAENOTCONN &&
                 WSAGetLastError() != WSAESHUTDOWN)) {
                result = 12;
            }
        }
    }
    if (result == 0 &&
        (network.is_active != PAPACC_TRUE ||
         entry.socket.is_listening != PAPACC_TRUE)) {
        result = 13;
    }
    if (result == 0) {
        log_capture.length = 0;
        log_capture.text[0] = '\0';
        if (papacc_logger_init(&logger, papacc_test_log_sink, &log_capture,
                               PAPACC_LOG_LEVEL_OFF) != PAPACC_RESULT_OK ||
            papacc_server_run_win32(
                &network, output, output, &logger) != PAPACC_RESULT_OK ||
            log_capture.length != 0 || log_capture.text[0] != '\0') {
            result = 33;
        }
    }

cleanup:
    papacc_server_console_win32_request_stop();
    if (thread != NULL) {
        (void)WaitForSingleObject(thread, 3000);
        (void)CloseHandle(thread);
    }
    if (output != NULL) {
        (void)fclose(output);
    } else if (output_fd != -1) {
        (void)_close(output_fd);
    } else if (write_handle != NULL) {
        (void)CloseHandle(write_handle);
    }
    if (read_handle != NULL) {
        (void)CloseHandle(read_handle);
    }
    if (client != INVALID_SOCKET) {
        (void)closesocket(client);
    }
    if (data_client != INVALID_SOCKET) {
        (void)closesocket(data_client);
    }
    papacc_test_network_shutdown(&network, &entry);
    (void)papacc_server_console_win32_uninstall(&console);
    return result;
}
