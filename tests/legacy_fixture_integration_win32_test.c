#include <papacc/pacc/capability_negotiation_protocol.h>
#include <papacc/pacc/control_protocol.h>
#include <papacc/pacc/data_association_protocol.h>
#include <papacc/pacc/frame.h>
#include <papacc/pacc/message_types.h>

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int send_all(SOCKET socket_value, const unsigned char *data, int size)
{
    int offset = 0;
    while (offset < size) {
        int sent = send(socket_value, (const char *)data + offset,
            size - offset, 0);
        if (sent <= 0) return 0;
        offset += sent;
    }
    return 1;
}

static int receive_all(SOCKET socket_value, unsigned char *data, int size)
{
    int offset = 0;
    while (offset < size) {
        int received = recv(socket_value, (char *)data + offset,
            size - offset, 0);
        if (received <= 0) return 0;
        offset += received;
    }
    return 1;
}

static SOCKET connect_loopback(unsigned short port)
{
    struct sockaddr_in address;
    SOCKET value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (value == INVALID_SOCKET) return INVALID_SOCKET;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(value, (const struct sockaddr *)&address, sizeof(address)) ==
            SOCKET_ERROR) {
        closesocket(value);
        return INVALID_SOCKET;
    }
    return value;
}

static int encode_frame(PAPACC_FRAME_HEADER header, const PAPACC_U8 *payload,
    PAPACC_SIZE payload_size, PAPACC_U8 *output, PAPACC_SIZE capacity,
    PAPACC_SIZE *written)
{
    PAPACC_SIZE header_size = 0U;
    if (capacity < PAPACC_FRAME_BASE_HEADER_SIZE + payload_size ||
        papacc_frame_header_encode(&header, output, capacity, &header_size) !=
            PAPACC_RESULT_OK || header_size != PAPACC_FRAME_BASE_HEADER_SIZE)
        return 0;
    if (payload_size != 0U)
        memcpy(output + header_size, payload, payload_size);
    *written = header_size + payload_size;
    return 1;
}

static int run_session(unsigned short port)
{
    const PAPACC_CAPABILITY_KEY network =
        PAPACC_PACC_NETWORK_EGRESS_V1_KEY_INITIALIZER;
    const PAPACC_CAPABILITY_KEY tls =
        PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY client_storage[2], preference_storage[2];
    PAPACC_CAPABILITY_KEY result_storage[2];
    PAPACC_CAPABILITY_SET client = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET preference = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET negotiated = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CONTROL_PROTOCOL_VERSION version = { 1U, 0U };
    PAPACC_DATA_ASSOCIATION_TICKET ticket =
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_FRAME_HEADER header;
    PAPACC_U8 payload[32], frame[64], response[64];
    PAPACC_SIZE payload_size = 0U, frame_size = 0U;
    SOCKET control = INVALID_SOCKET, data = INVALID_SOCKET;
    int ok = 0;

    control = connect_loopback(port);
    if (control == INVALID_SOCKET ||
        papacc_control_open_encode(&version, payload, sizeof(payload),
            &payload_size) != PAPACC_RESULT_OK ||
        !encode_frame(papacc_control_open_frame_header(), payload, payload_size,
            frame, sizeof(frame), &frame_size) ||
        !send_all(control, frame, (int)frame_size) ||
        !receive_all(control, response, 20) ||
        papacc_frame_header_decode(response, 16U, 4U, &header) !=
            PAPACC_RESULT_OK ||
        header.message_type != PAPACC_MESSAGE_TYPE_CONTROL_ACCEPT ||
        papacc_control_accept_decode(response + 16U, 4U, &version) !=
            PAPACC_RESULT_OK)
        goto cleanup;
    if (papacc_capability_set_init(&client, client_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_init(&preference, preference_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_init(&negotiated, result_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&client, &network) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&client, &tls) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&preference, &network) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&preference, &tls) != PAPACC_RESULT_OK ||
        papacc_capability_negotiate_encode(&client, &preference, payload,
            sizeof(payload), &payload_size) != PAPACC_RESULT_OK ||
        !encode_frame(papacc_capability_negotiate_frame_header(
            (PAPACC_U32)payload_size), payload, payload_size, frame,
            sizeof(frame), &frame_size) ||
        !send_all(control, frame, (int)frame_size) ||
        !receive_all(control, response, 24) ||
        papacc_frame_header_decode(response, 16U, 8U, &header) !=
            PAPACC_RESULT_OK ||
        header.message_type != PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT ||
        header.payload_length != 8U ||
        papacc_capability_negotiate_result_decode(response + 16U, 8U,
            &negotiated) != PAPACC_RESULT_OK ||
        papacc_capability_set_contains(&negotiated, &network) == PAPACC_TRUE ||
        papacc_capability_set_contains(&negotiated, &tls) != PAPACC_TRUE)
        goto cleanup;
    header = papacc_data_ticket_request_frame_header();
    if (!encode_frame(header, NULL, 0U, frame, sizeof(frame), &frame_size) ||
        !send_all(control, frame, (int)frame_size) ||
        !receive_all(control, response, 32) ||
        papacc_frame_header_decode(response, 16U, 16U, &header) !=
            PAPACC_RESULT_OK ||
        header.message_type != PAPACC_MESSAGE_TYPE_DATA_TICKET ||
        papacc_data_ticket_decode(response + 16U, 16U, &ticket) !=
            PAPACC_RESULT_OK)
        goto cleanup;
    data = connect_loopback(port);
    if (data == INVALID_SOCKET ||
        papacc_data_attach_encode(&ticket, payload, sizeof(payload),
            &payload_size) != PAPACC_RESULT_OK ||
        !encode_frame(papacc_data_attach_frame_header(), payload, payload_size,
            frame, sizeof(frame), &frame_size) ||
        !send_all(data, frame, (int)frame_size) ||
        !receive_all(data, response, 16) ||
        papacc_frame_header_decode(response, 16U, 0U, &header) !=
            PAPACC_RESULT_OK ||
        header.message_type != PAPACC_MESSAGE_TYPE_DATA_ACCEPT)
        goto cleanup;
    ok = control != data;
cleanup:
    if (data != INVALID_SOCKET) closesocket(data);
    if (control != INVALID_SOCKET) closesocket(control);
    return ok;
}

static int wait_ready(const char *path, unsigned short *port)
{
    unsigned int attempt;
    for (attempt = 0U; attempt < 200U; ++attempt) {
        FILE *file = NULL;
        char line[256];
        if (fopen_s(&file, path, "rb") == 0 && file != NULL) {
            while (fgets(line, sizeof(line), file) != NULL) {
                if (strncmp(line, "CONTROL_port=", 13U) == 0)
                    *port = (unsigned short)strtoul(line + 13U, NULL, 10);
            }
            fclose(file);
            if (*port != 0U) return 1;
        }
        Sleep(25U);
    }
    return 0;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    char temporary[MAX_PATH], ready[MAX_PATH * 2], event_name[128];
    char command[MAX_PATH * 4];
    STARTUPINFOA startup;
    PROCESS_INFORMATION process;
    HANDLE stop_event = NULL;
    unsigned short port = 0U;
    int result = 1;
    if (argc != 2 || WSAStartup(MAKEWORD(2, 2), &winsock) != 0 ||
        GetTempPathA(sizeof(temporary), temporary) == 0U ||
        sprintf_s(ready, sizeof(ready), "%spapacc-legacy-%lu.ready",
            temporary, (unsigned long)GetCurrentProcessId()) < 0 ||
        sprintf_s(event_name, sizeof(event_name),
            "Local\\papacc-legacy-stop-%lu",
            (unsigned long)GetCurrentProcessId()) < 0 ||
        sprintf_s(command, sizeof(command),
            "\"%s\" --legacy-browser-fixture \"%s\" \"%s\"",
            argv[1], ready, event_name) < 0)
        return 2;
    DeleteFileA(ready);
    stop_event = CreateEventA(NULL, TRUE, FALSE, event_name);
    memset(&startup, 0, sizeof(startup));
    memset(&process, 0, sizeof(process));
    startup.cb = sizeof(startup);
    if (stop_event == NULL || !CreateProcessA(NULL, command, NULL, NULL, FALSE,
            CREATE_NO_WINDOW, NULL, NULL, &startup, &process) ||
        !wait_ready(ready, &port) || !run_session(port) || !run_session(port) ||
        !SetEvent(stop_event) ||
        WaitForSingleObject(process.hProcess, 10000U) != WAIT_OBJECT_0)
        goto cleanup;
    result = 0;
    puts("LEGACY_FIXTURE_CONTROL_OPEN_ACCEPT=PASS");
    puts("NETWORK_EGRESS_granted=NO");
    puts("TLS_OFFLOAD_granted=YES");
    puts("LEGACY_FIXTURE_DATA_TICKET=PASS");
    puts("LEGACY_FIXTURE_INDEPENDENT_DATA=PASS");
    puts("LEGACY_FIXTURE_DATA_ATTACH_ACCEPT=PASS");
    puts("LEGACY_FRESH_RECONNECT_SUPPORTED=YES");
cleanup:
    if (process.hProcess != NULL) {
        if (result != 0 && WaitForSingleObject(process.hProcess, 0U) ==
                WAIT_TIMEOUT) {
            if (stop_event != NULL) (void)SetEvent(stop_event);
            if (WaitForSingleObject(process.hProcess, 5000U) == WAIT_TIMEOUT)
                TerminateProcess(process.hProcess, 1U);
        }
        CloseHandle(process.hThread);
        CloseHandle(process.hProcess);
    }
    if (stop_event != NULL) CloseHandle(stop_event);
    DeleteFileA(ready);
    WSACleanup();
    return result;
}
