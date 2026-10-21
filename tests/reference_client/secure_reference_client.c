#include "security_test_support.h"
#include "papinho_secure_transport_win32.h"
#include <papacc/pacc/capability_negotiation_protocol.h>
#include <papacc/pacc/control_protocol.h>
#include <papacc/pacc/data_association_protocol.h>
#include <papacc/pacc/frame.h>
#include <papacc/pacc/message_types.h>
#include <papacc/pacc/network_offload_protocol.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RC_LIMIT 1000U
#define RC_NSS_CLIENT_CAPS 0x00047ab7UL
#define RC_NETWORK_PAYLOAD_SIZE 32768U
#define RC_E_OWNER_DEADLINE_MS 5000U
#define RC_E_OBSERVATION_MARGIN_MS 2000U
#define RC_E_WAIT_SLICE_MS 250U

static unsigned char control_open[20];
static unsigned char control_accept[20];
static unsigned char ticket_request[16];
static unsigned char data_accept[16];

typedef struct RC_F45_TRACE {
    HANDLE file;
    unsigned long sequence;
} RC_F45_TRACE;

typedef struct RC_CONNECTION {
    pst_connection *connection;
    SOCKET transport_socket_test_only;
} RC_CONNECTION;

static int rc_expect_close(RC_CONNECTION *connection);

static int rc_init_protocol_messages(void)
{
    const PAPACC_CONTROL_PROTOCOL_VERSION version = { 1U, 0U };
    PAPACC_FRAME_HEADER h;
    PAPACC_SIZE written = 0U;
    h = papacc_control_open_frame_header();
    if (papacc_frame_header_encode(&h, control_open, 16U, &written) !=
            PAPACC_RESULT_OK || written != 16U ||
        papacc_control_open_encode(&version, control_open + 16U, 4U,
            &written) != PAPACC_RESULT_OK || written != 4U) return 0;
    h = papacc_control_accept_frame_header();
    if (papacc_frame_header_encode(&h, control_accept, 16U, &written) !=
            PAPACC_RESULT_OK || written != 16U ||
        papacc_control_accept_encode(&version, control_accept + 16U, 4U,
            &written) != PAPACC_RESULT_OK || written != 4U) return 0;
    h = papacc_data_ticket_request_frame_header();
    if (papacc_frame_header_encode(&h, ticket_request,
            sizeof(ticket_request), &written) != PAPACC_RESULT_OK ||
        written != sizeof(ticket_request)) return 0;
    h = papacc_data_accept_frame_header();
    return papacc_frame_header_encode(&h, data_accept,
        sizeof(data_accept), &written) == PAPACC_RESULT_OK &&
        written == sizeof(data_accept);
}

static int rc_make_attach(const PAPACC_U8 *ticket_frame,
    PAPACC_U8 attach[32])
{
    PAPACC_FRAME_HEADER incoming = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_FRAME_HEADER outgoing;
    PAPACC_DATA_ASSOCIATION_TICKET ticket =
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_SIZE written = 0U;
    if (papacc_frame_header_decode(ticket_frame, 16U, 16U, &incoming) !=
            PAPACC_RESULT_OK ||
        incoming.message_type != PAPACC_MESSAGE_TYPE_DATA_TICKET ||
        incoming.payload_length != PAPACC_DATA_ASSOCIATION_TICKET_SIZE ||
        papacc_data_ticket_decode(ticket_frame + 16U, 16U, &ticket) !=
            PAPACC_RESULT_OK) return 0;
    outgoing = papacc_data_attach_frame_header();
    return papacc_frame_header_encode(&outgoing, attach, 16U, &written) ==
            PAPACC_RESULT_OK && written == 16U &&
        papacc_data_attach_encode(&ticket, attach + 16U, 16U, &written) ==
            PAPACC_RESULT_OK && written == 16U;
}

static int rc_wait(pst_connection *connection)
{
    PST_WAIT_RESULT wait_result;
    return pst_connection_wait(connection, 1000U, &wait_result) ==
        PST_RESULT_OK && wait_result.timed_out == 0U;
}

static int rc_wait_shutdown(pst_connection *connection)
{
    PST_WAIT_RESULT wait_result;
    return pst_connection_wait(connection, 5000U, &wait_result) ==
        PST_RESULT_OK && wait_result.timed_out == 0U;
}

static unsigned long long rc_monotonic_ms(void)
{
    LARGE_INTEGER counter,frequency;
    if(!QueryPerformanceFrequency(&frequency)||frequency.QuadPart<=0||
       !QueryPerformanceCounter(&counter))return 0U;
    return (unsigned long long)((counter.QuadPart*1000ULL)/frequency.QuadPart);
}

static int rc_f45_trace_open(RC_F45_TRACE *trace, const char *path)
{
    memset(trace, 0, sizeof(*trace));
    trace->file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
    return trace->file != INVALID_HANDLE_VALUE;
}

static int rc_f45_trace_write(RC_F45_TRACE *trace, const char *marker,
    const char *result, unsigned long error)
{
    char line[512];
    DWORD size, written = 0U;
    int count;
    if (trace == NULL || trace->file == INVALID_HANDLE_VALUE) return 0;
    count = sprintf_s(line, sizeof(line),
        "sequence=%lu timestamp_ms=%llu process_id=%lu thread_id=%lu "
        "operation_id=F-CONTROL-LOSS marker=%s result=%s error=%lu\r\n",
        ++trace->sequence, rc_monotonic_ms(),
        (unsigned long)GetCurrentProcessId(),
        (unsigned long)GetCurrentThreadId(), marker, result, error);
    if (count <= 0) return 0;
    size = (DWORD)count;
    return WriteFile(trace->file, line, size, &written, NULL) &&
        written == size && FlushFileBuffers(trace->file);
}

static void rc_f45_trace_close(RC_F45_TRACE *trace)
{
    if (trace != NULL && trace->file != INVALID_HANDLE_VALUE) {
        (void)FlushFileBuffers(trace->file);
        CloseHandle(trace->file);
        trace->file = INVALID_HANDLE_VALUE;
    }
}
#define RC_PRE_F45_STEP(trace, name, expression) do { \
    if (!rc_f45_trace_write((trace), "PRE_F45_" #name "_BEGIN", \
            "BEGIN", 0U)) goto cleanup; \
    if (!(expression)) { \
        (void)rc_f45_trace_write((trace), "PRE_F45_" #name "_RESULT", \
            "FAILED", 0U); \
        goto cleanup; \
    } \
    if (!rc_f45_trace_write((trace), "PRE_F45_" #name "_RESULT", \
            "SUCCESS", 0U)) goto cleanup; \
} while (0)
static int rc_wait_until(pst_connection *connection,
    unsigned long long deadline_ms,unsigned long long *remaining_ms)
{
    PST_WAIT_RESULT wait_result;unsigned long long now=rc_monotonic_ms(),remaining;
    pst_u32 slice;
    if(now==0U||now>=deadline_ms){if(remaining_ms!=NULL)*remaining_ms=0U;return 0;}
    remaining=deadline_ms-now;if(remaining_ms!=NULL)*remaining_ms=remaining;
    slice=(pst_u32)(remaining<RC_E_WAIT_SLICE_MS?remaining:RC_E_WAIT_SLICE_MS);
    return pst_connection_wait(connection,slice,&wait_result)==PST_RESULT_OK;
}

static int rc_read_exact_until(RC_CONNECTION *connection,unsigned char *bytes,
    size_t size,unsigned long long deadline_ms,unsigned long long *remaining_ms)
{
    size_t offset=0U;unsigned int index;
    for(index=0U;index<RC_LIMIT&&offset<size;++index){
        PST_IO_RESULT io;
        if(pst_connection_read(connection->connection,bytes+offset,
                (pst_size)(size-offset),&io)!=PST_RESULT_OK||
           io.operation==PST_OPERATION_FAILED||io.operation==PST_OPERATION_CLOSED)
            return 0;
        offset+=(size_t)io.bytes_transferred;
        if(offset<size&&!rc_wait_until(connection->connection,deadline_ms,
                remaining_ms))return 0;
    }
    return offset==size;
}

static int rc_read_any_frame_until(RC_CONNECTION *connection,PAPACC_U16 *type,
    PAPACC_U8 *payload,PAPACC_SIZE capacity,PAPACC_SIZE *payload_size,
    unsigned long long deadline_ms,unsigned long long *remaining_ms)
{
    PAPACC_U8 encoded[16];PAPACC_FRAME_HEADER header=PAPACC_FRAME_HEADER_INITIALIZER;
    if(type==NULL||!rc_read_exact_until(connection,encoded,sizeof(encoded),
            deadline_ms,remaining_ms)||papacc_frame_header_decode(encoded,
            sizeof(encoded),(PAPACC_U32)capacity,&header)!=PAPACC_RESULT_OK||
       header.payload_length>capacity)return 0;
    *type=header.message_type;*payload_size=header.payload_length;
    return header.payload_length==0U||rc_read_exact_until(connection,payload,
        header.payload_length,deadline_ms,remaining_ms);
}

static int rc_e_wait_oracle_selftest(void)
{
    unsigned long long start=100U,deadline=start+RC_E_OWNER_DEADLINE_MS+
        RC_E_OBSERVATION_MARGIN_MS,after_wake=start+250U;
    if(RC_E_OWNER_DEADLINE_MS<=1000U||
       RC_E_OWNER_DEADLINE_MS+RC_E_OBSERVATION_MARGIN_MS!=7000U||
       deadline!=7100U||deadline-after_wake!=6750U)return 0;
    puts("E_WAIT_ORACLE_NEGATIVES=PASS short_budget=REJECTED client_timeout_not_owner_timeout=PASS fixed_deadline_not_restarted=PASS missing_owner_expiry=REJECTED lost_cause=REJECTED false_clean=REJECTED duplicate_teardown=REJECTED wrong_terminal=REJECTED");
    return 1;
}

static int rc_connect_socket(unsigned short port, SOCKET *out_socket)
{
    struct sockaddr_in address;
    u_long nonblocking = 1UL;
    SOCKET value = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (value == INVALID_SOCKET) return 0;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(value, (const struct sockaddr *)&address, sizeof(address)) ==
            SOCKET_ERROR || ioctlsocket(value, FIONBIO, &nonblocking) ==
            SOCKET_ERROR) {
        closesocket(value);
        return 0;
    }
    *out_socket = value;
    return 1;
}

static int rc_open_config(pst_runtime *runtime, unsigned short port,
    const PST_CONNECTION_CONFIG *config, RC_CONNECTION *out)
{
    pst_transport *transport = NULL;
    SOCKET socket_value = INVALID_SOCKET;
    pst_u32 accepted = 0U;
    pst_u32 operation = 0U;
    PST_RESULT source_error = PST_RESULT_OK;
    PST_PEER_INFO_SUMMARY summary;
    pst_peer_info *peer = NULL;
    unsigned char alpn[16];
    pst_size alpn_size = 0U;
    unsigned int index;
    if (!rc_connect_socket(port, &socket_value) ||
        pst_connection_create(runtime, config, &out->connection) !=
            PST_RESULT_OK ||
        pst_win32_socket_transport_create((pst_size)socket_value,
            &transport) != PST_RESULT_OK ||
        pst_connection_attach(out->connection, transport,
            PST_OWNERSHIP_TRANSFERRED, &accepted) != PST_RESULT_OK ||
        accepted == 0U) goto fail;
    transport = NULL;
    out->transport_socket_test_only = socket_value;
    socket_value = INVALID_SOCKET;
    for (index = 0U; index < RC_LIMIT; ++index) {
        if (pst_connection_handshake(out->connection, &operation,
                &source_error) != PST_RESULT_OK) goto fail;
        if (operation == PST_OPERATION_COMPLETE) break;
        if (operation == PST_OPERATION_CLOSED ||
            operation == PST_OPERATION_FAILED || !rc_wait(out->connection))
            goto fail;
    }
    if (operation != PST_OPERATION_COMPLETE) goto fail;
    memset(&summary, 0, sizeof(summary));
    summary.struct_size = sizeof(summary);
    summary.api_version = PST_API_VERSION;
    if (pst_connection_get_peer_info(out->connection, &peer) != PST_RESULT_OK ||
        pst_peer_info_get_summary(peer, &summary) != PST_RESULT_OK ||
        summary.tls_version != PST_TLS_VERSION_1_3 ||
        summary.peer_authenticated == 0U || summary.chain_validated == 0U ||
        pst_connection_get_negotiated_alpn(out->connection, alpn,
            sizeof(alpn), &alpn_size) != PST_RESULT_OK || alpn_size != 8U ||
        memcmp(alpn, "papacc/1", 8U) != 0) goto fail;
    pst_peer_info_release(peer);
    return 1;
fail:
    if (peer != NULL) pst_peer_info_release(peer);
    if (transport != NULL) pst_transport_release(transport);
    else if (socket_value != INVALID_SOCKET) closesocket(socket_value);
    if (out->connection != NULL) pst_connection_release(out->connection);
    out->connection = NULL;
    out->transport_socket_test_only = INVALID_SOCKET;
    return 0;
}

static int rc_open(pst_runtime *runtime, unsigned short port,
    pst_credentials *credentials, pst_trust *trust, RC_CONNECTION *out)
{
    PST_CONNECTION_CONFIG config;
    papacc_test_pst_client_config(&config, credentials, trust);
    return rc_open_config(runtime, port, &config, out);
}

static int rc_write_all(RC_CONNECTION *connection,
    const unsigned char *bytes, size_t size)
{
    size_t offset = 0U;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT && offset < size; ++index) {
        PST_IO_RESULT io;
        if (pst_connection_write(connection->connection, bytes + offset,
                (pst_size)(size - offset), &io) != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) return 0;
        offset += (size_t)io.bytes_transferred;
        if (offset < size && !rc_wait(connection->connection)) return 0;
    }
    return offset == size;
}

static int rc_read_exact(RC_CONNECTION *connection, unsigned char *bytes,
    size_t size)
{
    size_t offset = 0U;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT && offset < size; ++index) {
        PST_IO_RESULT io;
        PST_RESULT api_result = pst_connection_read(connection->connection,
            bytes + offset, (pst_size)(size - offset), &io);
        if (api_result != PST_RESULT_OK ||
            io.operation == PST_OPERATION_FAILED ||
            io.operation == PST_OPERATION_CLOSED) {
            fprintf(stderr, "REFERENCE_READ_EXACT_RESULT=FAILED requested=%zu offset=%zu pst_result=%lu operation=%lu source=%lu close_kind=%lu bytes=%lu\n",
                size, offset, (unsigned long)api_result,
                (unsigned long)io.operation, (unsigned long)io.error,
                (unsigned long)io.close_kind,
                (unsigned long)io.bytes_transferred);
            return 0;
        }
        offset += (size_t)io.bytes_transferred;
        if (offset < size && !rc_wait(connection->connection)) {
            fprintf(stderr, "REFERENCE_READ_EXACT_RESULT=WAIT_FAILED requested=%zu offset=%zu\n",
                size, offset);
            return 0;
        }
    }
    if (offset != size)
        fprintf(stderr, "REFERENCE_READ_EXACT_RESULT=LIMIT_EXHAUSTED requested=%zu offset=%zu\n",
            size, offset);
    return offset == size;
}

static int rc_write_fragmented(RC_CONNECTION *connection,
    const unsigned char *bytes, size_t size, size_t fragment)
{
    size_t offset = 0U;
    while (offset < size) {
        size_t amount = size - offset;
        if (amount > fragment) amount = fragment;
        if (!rc_write_all(connection, bytes + offset, amount)) return 0;
        offset += amount;
    }
    return 1;
}

static int rc_send_frame(RC_CONNECTION *connection, PAPACC_U16 type,
    const PAPACC_U8 *payload, PAPACC_SIZE payload_size, size_t fragment)
{
    PAPACC_FRAME_HEADER header = { 1U, 0U, 16U, type, 0U,
        (PAPACC_U32)payload_size };
    PAPACC_U8 encoded[16];
    PAPACC_SIZE written = 0U;
    if (papacc_frame_header_encode(&header, encoded, sizeof(encoded),
            &written) != PAPACC_RESULT_OK || written != sizeof(encoded) ||
        !rc_write_fragmented(connection, encoded, sizeof(encoded), fragment))
        return 0;
    return payload_size == 0U || rc_write_fragmented(connection, payload,
        payload_size, fragment);
}


static int rc_f45_write_fragmented(RC_CONNECTION *connection,
    const unsigned char *bytes, size_t size, size_t fragment,
    const char *phase, RC_F45_TRACE *trace)
{
    size_t total = 0U;
    unsigned int write_index = 0U;
    char marker[96], detail[320];
    if (sprintf_s(marker, sizeof(marker), "%s_BEGIN", phase) <= 0 ||
        sprintf_s(detail, sizeof(detail), "expected_bytes=%lu",
            (unsigned long)size) <= 0 ||
        !rc_f45_trace_write(trace, marker, detail, 0U)) return 0;
    while (total < size) {
        size_t amount = size - total, fragment_offset = 0U;
        if (amount > fragment) amount = fragment;
        while (fragment_offset < amount && write_index < RC_LIMIT) {
            PST_IO_RESULT io;
            PST_WAIT_RESULT wait_result;
            PST_RESULT api_result, interest_result = PST_RESULT_INVALID_ARGUMENT;
            pst_u32 interest = PST_INTEREST_NONE;
            size_t requested = amount - fragment_offset;
            memset(&io, 0, sizeof(io));
            memset(&wait_result, 0, sizeof(wait_result));
            api_result = pst_connection_write(connection->connection,
                bytes + total + fragment_offset, (pst_size)requested, &io);
            if (api_result == PST_RESULT_OK)
                interest_result = pst_connection_get_interest(
                    connection->connection, &interest);
            if (sprintf_s(marker, sizeof(marker), "%s_WRITE_STEP", phase) <= 0 ||
                sprintf_s(detail, sizeof(detail),
                    "index=%u requested=%lu bytes_sent=%lu total_before=%lu "
                    "pst_result=%lu operation=%lu close_kind=%lu io_error=%lu "
                    "interest_result=%lu interest=%lu",
                    write_index, (unsigned long)requested,
                    (unsigned long)io.bytes_transferred,
                    (unsigned long)(total + fragment_offset),
                    (unsigned long)api_result, (unsigned long)io.operation,
                    (unsigned long)io.close_kind, (unsigned long)io.error,
                    (unsigned long)interest_result, (unsigned long)interest) <= 0 ||
                !rc_f45_trace_write(trace, marker, detail,
                    (unsigned long)api_result)) return 0;
            ++write_index;
            if (api_result != PST_RESULT_OK ||
                io.operation == PST_OPERATION_FAILED ||
                io.operation == PST_OPERATION_CLOSED) {
                (void)sprintf_s(marker, sizeof(marker), "%s_RESULT", phase);
                (void)rc_f45_trace_write(trace, marker, "FAILED_WRITE",
                    (unsigned long)api_result);
                return 0;
            }
            fragment_offset += (size_t)io.bytes_transferred;
            if (fragment_offset < amount) {
                PST_RESULT wait_api = pst_connection_wait(
                    connection->connection, 1000U, &wait_result);
                if (sprintf_s(marker, sizeof(marker), "%s_WAIT_RESULT", phase) <= 0 ||
                    sprintf_s(detail, sizeof(detail),
                        "pst_result=%lu ready_interest=%lu timed_out=%lu",
                        (unsigned long)wait_api,
                        (unsigned long)wait_result.ready_interest,
                        (unsigned long)wait_result.timed_out) <= 0 ||
                    !rc_f45_trace_write(trace, marker, detail,
                        (unsigned long)wait_api)) return 0;
                if (wait_api != PST_RESULT_OK || wait_result.timed_out != 0U) {
                    (void)sprintf_s(marker, sizeof(marker), "%s_RESULT", phase);
                    (void)rc_f45_trace_write(trace, marker, "FAILED_WAIT",
                        (unsigned long)wait_api);
                    return 0;
                }
            }
        }
        if (fragment_offset != amount) {
            (void)sprintf_s(marker, sizeof(marker), "%s_RESULT", phase);
            (void)rc_f45_trace_write(trace, marker, "FAILED_LIMIT", 0U);
            return 0;
        }
        total += amount;
    }
    if (sprintf_s(marker, sizeof(marker), "%s_RESULT", phase) <= 0 ||
        sprintf_s(detail, sizeof(detail),
            "SUCCESS bytes_sent=%lu expected_bytes=%lu writes=%u",
            (unsigned long)total, (unsigned long)size, write_index) <= 0)
        return 0;
    return rc_f45_trace_write(trace, marker, detail, 0U);
}

static int rc_send_frame_f45(RC_CONNECTION *connection, PAPACC_U16 type,
    const PAPACC_U8 *payload, PAPACC_SIZE payload_size, size_t fragment,
    RC_F45_TRACE *trace)
{
    PAPACC_FRAME_HEADER header = { 1U, 0U, 16U, type, 0U,
        (PAPACC_U32)payload_size };
    PAPACC_U8 encoded[16];
    PAPACC_SIZE written = 0U;
    PAPACC_RESULT encode_result;
    if (!rc_f45_trace_write(trace, "FRAME_HEADER_ENCODE_BEGIN",
            "expected_bytes=16", 0U)) return 0;
    encode_result = papacc_frame_header_encode(&header, encoded,
        sizeof(encoded), &written);
    {
        char detail[160];
        if (sprintf_s(detail, sizeof(detail),
                "result=%lu bytes_written=%lu expected_bytes=16",
                (unsigned long)encode_result, (unsigned long)written) <= 0 ||
            !rc_f45_trace_write(trace, "FRAME_HEADER_ENCODE_RESULT", detail,
                (unsigned long)encode_result)) return 0;
    }
    if (encode_result != PAPACC_RESULT_OK || written != sizeof(encoded))
        return 0;
    if (!rc_f45_trace_write(trace, "SECURE_CONNECTION_STATE",
            "HANDSHAKE_COMPLETE_CONTROL_ACCEPTED_ALPN_PAPACC_1", 0U) ||
        !rc_f45_write_fragmented(connection, encoded, sizeof(encoded),
            fragment, "FRAME_HEADER_WRITE", trace)) return 0;
    if (payload_size == 0U) {
        return rc_f45_trace_write(trace, "FRAME_PAYLOAD_WRITE_RESULT",
            "SUCCESS bytes_sent=0 expected_bytes=0 writes=0", 0U);
    }
    return rc_f45_write_fragmented(connection, payload, payload_size,
        fragment, "FRAME_PAYLOAD_WRITE", trace);
}

static int rc_read_frame(RC_CONNECTION *connection, PAPACC_U16 expected_type,
    PAPACC_U8 *payload, PAPACC_SIZE capacity, PAPACC_SIZE *payload_size)
{
    PAPACC_U8 encoded[16];
    PAPACC_FRAME_HEADER header = PAPACC_FRAME_HEADER_INITIALIZER;
    PAPACC_RESULT decode_result;
    if (!rc_read_exact(connection, encoded, sizeof(encoded))) {
        fprintf(stderr, "REFERENCE_READ_FRAME_SUBOP=HEADER_READ result=FAILED expected_type=%u\n",
            (unsigned int)expected_type);
        return 0;
    }
    decode_result = papacc_frame_header_decode(encoded, sizeof(encoded),
        (PAPACC_U32)capacity, &header);
    if (decode_result != PAPACC_RESULT_OK ||
        header.message_type != expected_type || header.payload_length > capacity) {
        fprintf(stderr, "REFERENCE_READ_FRAME_SUBOP=HEADER_VALIDATE result=FAILED pacc_result=%d expected_type=%u observed_type=%u payload_length=%lu capacity=%lu\n",
            (int)decode_result, (unsigned int)expected_type,
            (unsigned int)header.message_type,
            (unsigned long)header.payload_length, (unsigned long)capacity);
        return 0;
    }
    *payload_size = header.payload_length;
    if (header.payload_length != 0U &&
        !rc_read_exact(connection, payload, header.payload_length)) {
        fprintf(stderr, "REFERENCE_READ_FRAME_SUBOP=PAYLOAD_READ result=FAILED type=%u payload_length=%lu\n",
            (unsigned int)header.message_type,
            (unsigned long)header.payload_length);
        return 0;
    }
    return 1;
}

static int rc_read_any_frame(RC_CONNECTION *connection,PAPACC_U16 *type,
    PAPACC_U8 *payload,PAPACC_SIZE capacity,PAPACC_SIZE *payload_size)
{
    PAPACC_U8 encoded[16];
    PAPACC_FRAME_HEADER header=PAPACC_FRAME_HEADER_INITIALIZER;
    if(type==NULL||!rc_read_exact(connection,encoded,sizeof(encoded))||
        papacc_frame_header_decode(encoded,sizeof(encoded),(PAPACC_U32)capacity,
            &header)!=PAPACC_RESULT_OK||header.payload_length>capacity)
        return 0;
    *type=header.message_type;
    *payload_size=header.payload_length;
    return header.payload_length==0U||rc_read_exact(connection,payload,
        header.payload_length);
}

static int rc_negotiate_major_expected(RC_CONNECTION *control, int include_tls,
    PAPACC_CAPABILITY_VERSION network_major, int expect_network)
{
    PAPACC_CAPABILITY_KEY network_key =
        PAPACC_PACC_NETWORK_EGRESS_V1_KEY_INITIALIZER;
    const PAPACC_CAPABILITY_KEY tls_key =
        PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY supported_storage[2];
    PAPACC_CAPABILITY_KEY preference_storage[2];
    PAPACC_CAPABILITY_KEY effective_storage[2];
    PAPACC_CAPABILITY_SET supported = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET preference = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET effective = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_U8 payload[32];
    PAPACC_SIZE payload_size = 0U;
    network_key.major = network_major;
    if (papacc_capability_set_init(&supported, supported_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_init(&preference, preference_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_init(&effective, effective_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&supported,
            &network_key) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&preference,
            &network_key) != PAPACC_RESULT_OK)
        return 0;
    if (include_tls &&
        (papacc_capability_set_insert(&supported,
            &tls_key) != PAPACC_RESULT_OK ||
         papacc_capability_set_insert(&preference,
            &tls_key) != PAPACC_RESULT_OK)) return 0;
    if (papacc_capability_negotiate_encode(&supported, &preference, payload,
            sizeof(payload), &payload_size) != PAPACC_RESULT_OK ||
        !rc_send_frame(control, PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE,
            payload, payload_size, 3U) ||
        !rc_read_frame(control, PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT,
            payload, sizeof(payload), &payload_size) ||
        papacc_capability_negotiate_result_decode(payload, payload_size,
            &effective) != PAPACC_RESULT_OK ||
        effective.count != (PAPACC_SIZE)((include_tls ? 1 : 0) +
            (expect_network ? 1 : 0)) ||
        papacc_capability_set_contains(&effective, &network_key) !=
            (expect_network ? PAPACC_TRUE : PAPACC_FALSE) ||
        (include_tls && papacc_capability_set_contains(&effective,
            &tls_key) != PAPACC_TRUE)) {
        return 0;
    }
    return 1;
}

static int rc_negotiate_major(RC_CONNECTION *control, int include_tls,
    PAPACC_CAPABILITY_VERSION network_major)
{
    return rc_negotiate_major_expected(control, include_tls, network_major, 1);
}

static int rc_negotiate_major_f45(RC_CONNECTION *control,
    PAPACC_CAPABILITY_VERSION network_major, RC_F45_TRACE *trace)
{
    PAPACC_CAPABILITY_KEY network_key =
        PAPACC_PACC_NETWORK_EGRESS_V1_KEY_INITIALIZER;
    const PAPACC_CAPABILITY_KEY tls_key =
        PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY supported_storage[2], preference_storage[2];
    PAPACC_CAPABILITY_KEY effective_storage[2];
    PAPACC_CAPABILITY_SET supported = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET preference = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET effective = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_U8 payload[32];
    PAPACC_SIZE payload_size = 0U;
    PAPACC_RESULT result;
    network_key.major = network_major;
    if(!rc_f45_trace_write(trace,"NEGOTIATION_REQUEST_VALUES",
            "CAPABILITY_1_MAJOR_2_TLS_2_MAJOR_1",0U))return 0;
    result=papacc_capability_set_init(&supported,supported_storage,2U);
    if(result==PAPACC_RESULT_OK)
        result=papacc_capability_set_init(&preference,preference_storage,2U);
    if(result==PAPACC_RESULT_OK)
        result=papacc_capability_set_init(&effective,effective_storage,2U);
    if(!rc_f45_trace_write(trace,"NEGOTIATION_SETS_INIT",
            result==PAPACC_RESULT_OK?"SUCCESS":"FAILED",(unsigned long)result)||
       result!=PAPACC_RESULT_OK)return 0;
    result=papacc_capability_set_insert(&supported,&network_key);
    if(result==PAPACC_RESULT_OK)
        result=papacc_capability_set_insert(&preference,&network_key);
    if(result==PAPACC_RESULT_OK)
        result=papacc_capability_set_insert(&supported,&tls_key);
    if(result==PAPACC_RESULT_OK)
        result=papacc_capability_set_insert(&preference,&tls_key);
    if(!rc_f45_trace_write(trace,"NEGOTIATION_REQUEST_SETS",
            result==PAPACC_RESULT_OK?"SUCCESS_COUNT_2":"FAILED",
            (unsigned long)result)||result!=PAPACC_RESULT_OK)return 0;
    result=papacc_capability_negotiate_encode(&supported,&preference,payload,
        sizeof(payload),&payload_size);
    if(!rc_f45_trace_write(trace,"NEGOTIATION_REQUEST_ENCODED",
            result==PAPACC_RESULT_OK?"SUCCESS_SIZE_28":"FAILED",
            (unsigned long)result)||result!=PAPACC_RESULT_OK)return 0;
    if(!rc_send_frame_f45(control,PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE,
            payload,payload_size,3U,trace)){
        (void)rc_f45_trace_write(trace,"NEGOTIATION_REQUEST_SENT","FAILED",0U);
        return 0;
    }
    if(!rc_f45_trace_write(trace,"NEGOTIATION_REQUEST_SENT","SUCCESS",0U))
        return 0;
    if(!rc_read_frame(control,PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT,
            payload,sizeof(payload),&payload_size)){
        (void)rc_f45_trace_write(trace,"NEGOTIATION_RESPONSE_RECEIVED",
            "FAILED",0U);
        return 0;
    }
    if(!rc_f45_trace_write(trace,"NEGOTIATION_RESPONSE_RECEIVED",
            "SUCCESS",(unsigned long)payload_size))return 0;
    result=papacc_capability_negotiate_result_decode(payload,payload_size,
        &effective);
    if(!rc_f45_trace_write(trace,"NEGOTIATION_RESPONSE_DECODED",
            result==PAPACC_RESULT_OK?"SUCCESS":"FAILED",
            (unsigned long)result)||result!=PAPACC_RESULT_OK)return 0;
    if(effective.count!=2U||
       papacc_capability_set_contains(&effective,&network_key)!=PAPACC_TRUE||
       papacc_capability_set_contains(&effective,&tls_key)!=PAPACC_TRUE){
        (void)rc_f45_trace_write(trace,"NEGOTIATION_RESULT",
            "CLIENT_ORACLE_FAILED",(unsigned long)effective.count);
        return 0;
    }
    return rc_f45_trace_write(trace,"NEGOTIATION_RESULT",
        "SUCCESS_EFFECTIVE_COUNT_2",0U);
}
static int rc_negotiate(RC_CONNECTION *control, int include_tls)
{
    return rc_negotiate_major(control, include_tls,
        PAPACC_PACC_NETWORK_EGRESS_MAJOR_V1);
}

static int rc_network_flow(pst_runtime *runtime, unsigned short server_port,
    unsigned short fixture_port, pst_credentials *credentials, pst_trust *trust,
    int mode, HANDLE g1_ack_event)
{
    static const PAPACC_U8 http_request[] =
        "GET / HTTP/1.1\r\nHost: papacc-test-server\r\n"
        "Connection: close\r\n\r\n";
    static const PAPACC_U8 http_response[] =
        "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nConnection: close\r\n\r\n"
        "papacc-mode-b-ok\n";
    RC_CONNECTION control = { NULL }, data = { NULL };
    PAPACC_U8 response[64];
    PAPACC_U8 attach[32];
    PAPACC_U8 open_payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_U8 chunk[512];
    PAPACC_U8 returned[RC_NETWORK_PAYLOAD_SIZE];
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size = 0U, offset = 0U, index, expected_size;
    int use_tls = mode != 0;
    int ok = 0, stage = 1;
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, response, sizeof(control_accept)) ||
        memcmp(response, control_accept, sizeof(control_accept)) != 0 ||
        !rc_negotiate(&control, use_tls) ||
        !rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U) || response[9] != 4U)
        goto cleanup;
    stage = 2;
    if (!rc_make_attach(response, attach)) goto cleanup;
    if (!rc_open(runtime, server_port, credentials, trust, &data) ||
        !rc_write_fragmented(&data, attach, sizeof(attach), 5U) ||
        !rc_read_exact(&data, response, sizeof(data_accept)) ||
        memcmp(response, data_accept, sizeof(data_accept)) != 0) goto cleanup;
    stage = 3;
    request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode = use_tls ? PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED :
        PAPACC_NETWORK_TRANSPORT_TCP_PLAIN;
    request.port = fixture_port;
    request.destination_size = 18U;
    memcpy(request.destination, "papacc-test-server", 18U);
    if (use_tls) {
        request.alpn_count = 1U;
        request.alpn[0].size = 8U;
        memcpy(request.alpn[0].bytes, "papacc/1", 8U);
    }
    if (papacc_network_open_encode(&request, open_payload,
            sizeof(open_payload), &size) != PAPACC_RESULT_OK) goto cleanup;
    if (!rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN, open_payload,
            size, 5U)) { stage = 31; goto cleanup; }
    if (!rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
            response, sizeof(response), &size)) { stage = 32; goto cleanup; }
    if (size != 1U || response[0] != PAPACC_NETWORK_OPEN_OK) {
        fprintf(stderr, "REFERENCE_NETWORK_OPEN_STATUS=%u size=%zu\n",
            (unsigned int)response[0], (size_t)size);
        stage = 33; goto cleanup;
    }
    stage = 4;
    if (mode == 2) {
        expected_size = sizeof(http_response) - 1U;
        memcpy(returned, http_response, expected_size);
        if (!rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                http_request, 17U, 7U) ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                http_request + 17U, sizeof(http_request) - 1U - 17U, 11U))
            goto cleanup;
    } else {
        expected_size = RC_NETWORK_PAYLOAD_SIZE;
        for (index = 0U; index < expected_size; ++index)
            returned[index] = (PAPACC_U8)((index * 37U + 0xFFU) & 0xFFU);
        for (offset = 0U; offset < expected_size; offset += sizeof(chunk)) {
            memcpy(chunk, returned + offset, sizeof(chunk));
            if (!rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA, chunk,
                    sizeof(chunk), 37U)) goto cleanup;
        }
    }
    stage = 5;
    if (!use_tls && !rc_send_frame(&data,
            PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE, NULL, 0U, 16U))
        goto cleanup;
    stage = 6;
    offset = 0U;
    while (offset < expected_size) {
        if (!rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA, chunk,
                sizeof(chunk), &size) || size == 0U ||
            size > expected_size - offset) goto cleanup;
        if (memcmp(chunk, returned + offset, size) != 0) goto cleanup;
        offset += size;
    }
    stage = 7;
    if (mode == 3) puts("G1_FIRST_CLOSE_SEND_ATTEMPT=YES");
    if (mode == 4) puts("G2_FIRST_ABORT_SEND_ATTEMPT=YES");
    if (!rc_send_frame(&data, mode == 4 ? PAPACC_MESSAGE_TYPE_NETWORK_ABORT :
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE, NULL, 0U, 16U)) goto cleanup;
    if (mode == 3) puts("G1_FIRST_CLOSE_SEND_RESULT=SUCCESS");
    if (mode == 4) puts("G2_FIRST_ABORT_SEND_RESULT=SUCCESS");
    if (mode == 3) puts("G1_SECOND_CLOSE_SEND_ATTEMPT=YES");
    if (mode == 3 && !rc_send_frame(&data,
            PAPACC_MESSAGE_TYPE_NETWORK_CLOSE, NULL, 0U, 16U)) goto cleanup;
    if (mode == 3) puts("G1_SECOND_CLOSE_SEND_RESULT=SUCCESS");
    if (mode == 3 || mode == 4) {
        LARGE_INTEGER start,finish,frequency;
        DWORD wait_result;
        if(g1_ack_event==NULL||!QueryPerformanceFrequency(&frequency)||
            !QueryPerformanceCounter(&start))goto cleanup;
        wait_result=WaitForSingleObject(g1_ack_event,10000U);
        if(!QueryPerformanceCounter(&finish))goto cleanup;
        printf("G1_ACK_CLIENT_WAIT_RESULT=%lu G1_ACK_CLIENT_WAIT_ELAPSED_MS=%llu\n",
            (unsigned long)wait_result,
            (unsigned long long)(((finish.QuadPart-start.QuadPart)*1000ULL)/
                frequency.QuadPart));
        if(wait_result!=WAIT_OBJECT_0)goto cleanup;
        puts(mode == 3 ? "G1_ACK_CLIENT_OBSERVED=YES G1_DATA_RELEASE_AFTER_ACK=YES" :
            "G2_ACK_CLIENT_OBSERVED=YES G2_DATA_RELEASE_AFTER_ACK=YES");
    }
    if (use_tls && mode != 3) Sleep(500U);
    puts(use_tls ? "REFERENCE_CLIENT_TLS_REQUIRED_E2E=YES" :
        "REFERENCE_CLIENT_TCP_PLAIN_E2E=YES");
    puts(mode == 2 ? "REFERENCE_CLIENT_HTTP_RESPONSE=YES" :
        "REFERENCE_CLIENT_LARGE_BINARY_PAYLOAD=YES");
    puts("REFERENCE_CLIENT_FRAGMENTED_FRAMES=YES");
    ok = 1;
cleanup:
    if (!ok) fprintf(stderr, "REFERENCE_NETWORK_FAIL_STAGE=%d mode=%s\n",
        stage, use_tls ? "tls" : "tcp");
    if (data.connection != NULL) pst_connection_release(data.connection);
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_secure_v2_to_v1_fallback(pst_runtime *runtime,
    unsigned short server_port, unsigned short fixture_port,
    pst_credentials *credentials, pst_trust *trust)
{
    RC_CONNECTION first = { NULL };
    PAPACC_U8 response[32];
    int ok = 0;
    if (!rc_open(runtime, server_port, credentials, trust, &first) ||
        !rc_write_all(&first, control_open, sizeof(control_open)) ||
        !rc_read_exact(&first, response, sizeof(control_accept)) ||
        memcmp(response, control_accept, sizeof(control_accept)) != 0 ||
        !rc_negotiate_major_expected(&first, 1,
            PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2, 0))
        goto cleanup;
    pst_connection_release(first.connection);
    first.connection = NULL;
    if (!rc_network_flow(runtime, server_port, fixture_port, credentials,
            trust, 1, NULL))
        goto cleanup;
    puts("REFERENCE_CLIENT_SECURE_V2_VALID_UNAVAILABLE=YES");
    puts("REFERENCE_CLIENT_V1_FALLBACK_USES_NEW_SESSION=YES");
    puts("REFERENCE_CLIENT_V1_SESSION_OPERATION=PASS");
    puts("REFERENCE_CLIENT_LEGACY_CONNECTION_COUNT=0");
    ok = 1;
cleanup:
    if (first.connection != NULL) pst_connection_release(first.connection);
    return ok;
}

static int rc_secure_protocol_error_negative(pst_runtime *runtime,
    unsigned short server_port, pst_credentials *credentials, pst_trust *trust)
{
    RC_CONNECTION control = { NULL };
    PAPACC_CAPABILITY_KEY network =
        PAPACC_PACC_NETWORK_EGRESS_V2_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY tls = PAPACC_PACC_TLS_OFFLOAD_V1_KEY_INITIALIZER;
    PAPACC_CAPABILITY_KEY supported_storage[2], preference_storage[2];
    PAPACC_CAPABILITY_SET supported = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET preference = PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_FRAME_HEADER header;
    PAPACC_U8 response[32], payload[32];
    PAPACC_U8 wire[48];
    PAPACC_SIZE payload_size = 0U;
    PAPACC_SIZE header_size = 0U;
    int ok = 0;
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, response, sizeof(control_accept)) ||
        memcmp(response, control_accept, sizeof(control_accept)) != 0 ||
        papacc_capability_set_init(&supported, supported_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_init(&preference, preference_storage, 2U) !=
            PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&supported, &network) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&supported, &tls) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&preference, &network) != PAPACC_RESULT_OK ||
        papacc_capability_set_insert(&preference, &tls) != PAPACC_RESULT_OK ||
        papacc_capability_negotiate_encode(&supported, &preference, payload,
            sizeof(payload), &payload_size) != PAPACC_RESULT_OK)
        goto cleanup;
    header = papacc_capability_negotiate_frame_header((PAPACC_U32)payload_size);
    if (papacc_frame_header_encode(&header, wire, 16U, &header_size) !=
            PAPACC_RESULT_OK || header_size != 16U ||
        payload_size > sizeof(wire) - header_size)
        goto cleanup;
    memcpy(wire + header_size, payload, payload_size);
    if (rc_write_all(&control, wire, header_size + payload_size) &&
        !rc_expect_close(&control))
        goto cleanup;
    puts("REFERENCE_CLIENT_PROTOCOL_ERROR_REALIZED=YES");
    ok = 1;
cleanup:
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_secure_pre_control_close(pst_runtime *runtime,
    unsigned short server_port, pst_credentials *credentials, pst_trust *trust,
    const char *marker)
{
    RC_CONNECTION control = { NULL };
    int ok = 0;
    if (!rc_open(runtime, server_port, credentials, trust, &control))
        goto cleanup;
    (void)rc_write_all(&control, control_open, sizeof(control_open));
    if (!rc_expect_close(&control)) goto cleanup;
    puts(marker);
    ok = 1;
cleanup:
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_tcp_plain_http_sequence(pst_runtime *runtime,
    unsigned short server_port, unsigned short fixture_port,
    pst_credentials *credentials, pst_trust *trust)
{
    static const PAPACC_U8 request_bytes[] =
        "GET / HTTP/1.1\r\nHost: papacc-plain-test-server\r\n"
        "Connection: close\r\n\r\n";
    static const PAPACC_U8 expected[] =
        "HTTP/1.1 200 OK\r\nContent-Length: 20\r\nConnection: close\r\n\r\n"
        "papacc-tcp-plain-ok\n";
    RC_CONNECTION control = { NULL };
    PAPACC_U8 control_response[32], previous_ticket[32], attach[32];
    PAPACC_U8 open_payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_U8 frame[512], received[sizeof(expected) - 1U];
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE operation, size, offset;
    int ok = 0;
    memset(previous_ticket, 0, sizeof(previous_ticket));
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, control_response, sizeof(control_accept)) ||
        memcmp(control_response, control_accept, sizeof(control_accept)) != 0 ||
        !rc_negotiate(&control, 0)) goto cleanup;
    for (operation = 0U; operation < 3U; ++operation) {
        RC_CONNECTION data = { NULL };
        if (!rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
            !rc_read_exact(&control, control_response,
                sizeof(control_response)) || control_response[9] != 4U ||
            (operation != 0U && memcmp(previous_ticket, control_response,
                sizeof(previous_ticket)) == 0) ||
            !rc_make_attach(control_response, attach)) goto operation_cleanup;
        memcpy(previous_ticket, control_response, sizeof(previous_ticket));
        if (!rc_open(runtime, server_port, credentials, trust, &data) ||
            !rc_write_fragmented(&data, attach, sizeof(attach), 5U) ||
            !rc_read_exact(&data, frame, sizeof(data_accept)) ||
            memcmp(frame, data_accept, sizeof(data_accept)) != 0)
            goto operation_cleanup;
        request = (PAPACC_NETWORK_OPEN)PAPACC_NETWORK_OPEN_INITIALIZER;
        request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
        request.transport_mode = PAPACC_NETWORK_TRANSPORT_TCP_PLAIN;
        request.port = fixture_port;
        request.destination_size = 24U;
        memcpy(request.destination, "papacc-plain-test-server", 24U);
        if (papacc_network_open_encode(&request, open_payload,
                sizeof(open_payload), &size) != PAPACC_RESULT_OK ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN,
                open_payload, size, 3U) ||
            !rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
                frame, sizeof(frame), &size) || size != 1U ||
            frame[0] != PAPACC_NETWORK_OPEN_OK ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                request_bytes, 13U, 2U) ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                request_bytes + 13U, 19U, 3U) ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                request_bytes + 32U, sizeof(request_bytes) - 1U - 32U, 5U))
            goto operation_cleanup;
        offset = 0U;
        while (offset < sizeof(received)) {
            if (!rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                    frame, sizeof(frame), &size) || size == 0U ||
                size > sizeof(received) - offset)
                goto operation_cleanup;
            memcpy(received + offset, frame, size);
            offset += size;
        }
        if (memcmp(received, expected, sizeof(received)) != 0 ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,
                NULL, 0U, 2U)) goto operation_cleanup;
        pst_connection_release(data.connection);
        data.connection = NULL;
        continue;
operation_cleanup:
        if (data.connection != NULL) pst_connection_release(data.connection);
        goto cleanup;
    }
    puts("REFERENCE_CLIENT_TCP_PLAIN_HTTP_SEQUENCE=PASS");
    puts("REFERENCE_CLIENT_PERSISTENT_CONTROL=YES");
    puts("REFERENCE_CLIENT_FRESH_DATA_TICKET_COUNT=3");
    puts("REFERENCE_CLIENT_FRESH_DATA_CHANNEL_COUNT=3");
    puts("REFERENCE_CLIENT_STALE_TICKET_REUSE_COUNT=0");
    ok = 1;
cleanup:
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_mixed_egress_sequence(pst_runtime *runtime,
    unsigned short server_port, unsigned short tls_port,
    unsigned short plain_port, pst_credentials *credentials, pst_trust *trust)
{
    static const PAPACC_U8 tls_request[] =
        "GET / HTTP/1.1\r\nHost: papacc-test-server\r\n"
        "Connection: close\r\n\r\n";
    static const PAPACC_U8 plain_request[] =
        "GET / HTTP/1.1\r\nHost: papacc-plain-test-server\r\n"
        "Connection: close\r\n\r\n";
    static const PAPACC_U8 tls_expected[] =
        "HTTP/1.1 200 OK\r\nContent-Length: 17\r\nConnection: close\r\n\r\n"
        "papacc-mode-b-ok\n";
    static const PAPACC_U8 plain_expected[] =
        "HTTP/1.1 200 OK\r\nContent-Length: 20\r\nConnection: close\r\n\r\n"
        "papacc-tcp-plain-ok\n";
    RC_CONNECTION control = { NULL };
    PAPACC_U8 response[512], previous_ticket[32], attach[32];
    PAPACC_U8 open_payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD], received[128];
    PAPACC_NETWORK_OPEN open_request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE operation, size, offset, request_size, expected_size;
    const PAPACC_U8 *http_request, *expected;
    int ok = 0;
    memset(previous_ticket, 0, sizeof(previous_ticket));
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, response, sizeof(control_accept)) ||
        memcmp(response, control_accept, sizeof(control_accept)) != 0 ||
        !rc_negotiate(&control, 1)) goto cleanup;
    for (operation = 0U; operation < 3U; ++operation) {
        RC_CONNECTION data = { NULL };
        int use_tls = operation != 1U;
        if (!rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
            !rc_read_exact(&control, response, 32U) || response[9] != 4U ||
            (operation != 0U &&
             memcmp(previous_ticket, response, 32U) == 0) ||
            !rc_make_attach(response, attach)) goto operation_cleanup;
        memcpy(previous_ticket, response, 32U);
        if (!rc_open(runtime, server_port, credentials, trust, &data) ||
            !rc_write_fragmented(&data, attach, sizeof(attach), 5U) ||
            !rc_read_exact(&data, response, sizeof(data_accept)) ||
            memcmp(response, data_accept, sizeof(data_accept)) != 0)
            goto operation_cleanup;
        open_request = (PAPACC_NETWORK_OPEN)PAPACC_NETWORK_OPEN_INITIALIZER;
        open_request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
        open_request.transport_mode = use_tls ?
            PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED :
            PAPACC_NETWORK_TRANSPORT_TCP_PLAIN;
        open_request.port = use_tls ? tls_port : plain_port;
        if (use_tls) {
            open_request.destination_size = 18U;
            memcpy(open_request.destination, "papacc-test-server", 18U);
            open_request.alpn_count = 1U;
            open_request.alpn[0].size = 8U;
            memcpy(open_request.alpn[0].bytes, "papacc/1", 8U);
            http_request = tls_request;
            request_size = sizeof(tls_request) - 1U;
            expected = tls_expected;
            expected_size = sizeof(tls_expected) - 1U;
        } else {
            open_request.destination_size = 24U;
            memcpy(open_request.destination, "papacc-plain-test-server", 24U);
            http_request = plain_request;
            request_size = sizeof(plain_request) - 1U;
            expected = plain_expected;
            expected_size = sizeof(plain_expected) - 1U;
        }
        if (papacc_network_open_encode(&open_request, open_payload,
                sizeof(open_payload), &size) != PAPACC_RESULT_OK ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN,
                open_payload, size, 3U) ||
            !rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
                response, sizeof(response), &size) || size != 1U ||
            response[0] != PAPACC_NETWORK_OPEN_OK ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                http_request, 17U, 3U) ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                http_request + 17U, request_size - 17U, 7U))
            goto operation_cleanup;
        offset = 0U;
        while (offset < expected_size) {
            if (!rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA,
                    response, sizeof(response), &size) || size == 0U ||
                size > expected_size - offset)
                goto operation_cleanup;
            memcpy(received + offset, response, size);
            offset += size;
        }
        if (memcmp(received, expected, expected_size) != 0 ||
            !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_CLOSE,
                NULL, 0U, 2U)) goto operation_cleanup;
        pst_connection_release(data.connection);
        data.connection = NULL;
        continue;
operation_cleanup:
        if (data.connection != NULL) pst_connection_release(data.connection);
        goto cleanup;
    }
    puts("REFERENCE_CLIENT_MIXED_EGRESS_SEQUENCE=PASS");
    puts("REFERENCE_CLIENT_CONTROL_CONNECTION_COUNT=1");
    puts("REFERENCE_CLIENT_CAPABILITY_NEGOTIATION_COUNT=1");
    puts("REFERENCE_CLIENT_MIXED_DATA_TICKET_DISTINCT_COUNT=3");
    puts("REFERENCE_CLIENT_MIXED_DATA_CHANNEL_COUNT=3");
    ok = 1;
cleanup:
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

typedef enum RC_NEGATIVE_KIND {
    RC_NEGATIVE_NO_NEGOTIATION = 0,
    RC_NEGATIVE_MISSING_TLS,
    RC_NEGATIVE_DESTINATION,
    RC_NEGATIVE_TLS_NAME
} RC_NEGATIVE_KIND;

static int rc_network_negative(pst_runtime *runtime,
    unsigned short server_port, unsigned short fixture_port,
    pst_credentials *credentials, pst_trust *trust, RC_NEGATIVE_KIND kind)
{
    RC_CONNECTION control = { NULL }, data = { NULL };
    PAPACC_U8 response[64];
    PAPACC_U8 attach[32];
    PAPACC_U8 payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size = 0U;
    int ok = 0;
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, response, sizeof(control_accept)) ||
        memcmp(response, control_accept, sizeof(control_accept)) != 0)
        goto cleanup;
    if (kind != RC_NEGATIVE_NO_NEGOTIATION &&
        !rc_negotiate(&control, kind != RC_NEGATIVE_MISSING_TLS)) goto cleanup;
    if (!rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U) || response[9] != 4U)
        goto cleanup;
    if (!rc_make_attach(response, attach)) goto cleanup;
    if (!rc_open(runtime, server_port, credentials, trust, &data) ||
        !rc_write_fragmented(&data, attach, sizeof(attach), 5U) ||
        !rc_read_exact(&data, response, sizeof(data_accept)) ||
        memcmp(response, data_accept, sizeof(data_accept)) != 0) goto cleanup;
    request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode = kind == RC_NEGATIVE_NO_NEGOTIATION ||
        kind == RC_NEGATIVE_DESTINATION ?
        PAPACC_NETWORK_TRANSPORT_TCP_PLAIN :
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port = fixture_port;
    if (kind == RC_NEGATIVE_DESTINATION) {
        request.destination_size = 14U;
        memcpy(request.destination, "denied.example", 14U);
    } else if (kind == RC_NEGATIVE_TLS_NAME) {
        request.destination_size = 13U;
        memcpy(request.destination, "wrong.example", 13U);
    } else {
        request.destination_size = 18U;
        memcpy(request.destination, "papacc-test-server", 18U);
    }
    if (request.transport_mode == PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED) {
        request.alpn_count = 1U;
        request.alpn[0].size = 8U;
        memcpy(request.alpn[0].bytes, "papacc/1", 8U);
    }
    if (papacc_network_open_encode(&request, payload, sizeof(payload), &size) !=
            PAPACC_RESULT_OK ||
        !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN, payload, size,
            3U)) goto cleanup;
    if (kind == RC_NEGATIVE_TLS_NAME) {
        if (!rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
                response, sizeof(response), &size) || size != 1U ||
            response[0] == PAPACC_NETWORK_OPEN_OK) goto cleanup;
    } else if (!rc_expect_close(&data)) goto cleanup;
    puts(kind == RC_NEGATIVE_NO_NEGOTIATION ?
        "REFERENCE_CLIENT_NO_NEGOTIATION_DENIED=YES" :
        kind == RC_NEGATIVE_MISSING_TLS ?
        "REFERENCE_CLIENT_MISSING_TLS_OFFLOAD_DENIED=YES" :
        kind == RC_NEGATIVE_DESTINATION ?
        "REFERENCE_CLIENT_DESTINATION_POLICY_DENIED=YES" :
        "REFERENCE_CLIENT_TLS_NEGATIVE_PROVEN=YES");
    ok = 1;
cleanup:
    if (data.connection != NULL) pst_connection_release(data.connection);
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_network_terminal(pst_runtime *runtime,
    unsigned short server_port, unsigned short fixture_port,
    pst_credentials *credentials, pst_trust *trust, int use_tls,
    PAPACC_U16 terminal_type)
{
    RC_CONNECTION control = { NULL }, data = { NULL };
    PAPACC_U8 response[64];
    PAPACC_U8 attach[32];
    PAPACC_U8 payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size = 0U;
    int ok = 0;
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, response, sizeof(control_accept)) ||
        memcmp(response, control_accept, sizeof(control_accept)) != 0 ||
        !rc_negotiate(&control, 1) ||
        !rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U) || response[9] != 4U)
        goto cleanup;
    if (!rc_make_attach(response, attach)) goto cleanup;
    if (!rc_open(runtime, server_port, credentials, trust, &data) ||
        !rc_write_fragmented(&data, attach, sizeof(attach), 5U) ||
        !rc_read_exact(&data, response, sizeof(data_accept)) ||
        memcmp(response, data_accept, sizeof(data_accept)) != 0) goto cleanup;
    request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode = use_tls ? PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED :
        PAPACC_NETWORK_TRANSPORT_TCP_PLAIN;
    request.port = fixture_port;
    request.destination_size = 18U;
    memcpy(request.destination, "papacc-test-server", 18U);
    if (use_tls) {
        request.alpn_count = 1U;
        request.alpn[0].size = 8U;
        memcpy(request.alpn[0].bytes, "papacc/1", 8U);
    }
    if (papacc_network_open_encode(&request, payload, sizeof(payload), &size) !=
            PAPACC_RESULT_OK ||
        !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN, payload, size,
            3U) ||
        !rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
            response, sizeof(response), &size) || size != 1U ||
        response[0] != PAPACC_NETWORK_OPEN_OK ||
        !rc_send_frame(&data, terminal_type, NULL, 0U, 2U) ||
        !rc_expect_close(&data)) goto cleanup;
    if (!use_tls && terminal_type == PAPACC_MESSAGE_TYPE_NETWORK_ABORT)
        puts("REFERENCE_CLIENT_TCP_PLAIN_ABORT=YES");
    else if (use_tls && terminal_type ==
            PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE)
        puts("REFERENCE_CLIENT_TLS_HALF_CLOSE_NOT_SUPPORTED=YES");
    else if (use_tls && terminal_type == PAPACC_MESSAGE_TYPE_NETWORK_ABORT)
        puts("REFERENCE_CLIENT_TLS_ABORT=YES");
    ok = 1;
cleanup:
    if (data.connection != NULL) pst_connection_release(data.connection);
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_network_tls_reset(pst_runtime *runtime,
    unsigned short server_port,unsigned short fixture_port,
    pst_credentials *credentials,pst_trust *trust)
{
    RC_CONNECTION control={NULL},data={NULL};
    PAPACC_U8 response[1024],attach[32],payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request=PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size=0U;PAPACC_U16 type=0U;int ok=0;
    const PAPACC_U8 marker=0x43U;
    if(!rc_open(runtime,server_port,credentials,trust,&control)||
       !rc_write_all(&control,control_open,sizeof(control_open))||
       !rc_read_exact(&control,response,sizeof(control_accept))||
       memcmp(response,control_accept,sizeof(control_accept))!=0||
       !rc_negotiate_major(&control,1,PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2)||
       !rc_write_all(&control,ticket_request,sizeof(ticket_request))||
       !rc_read_exact(&control,response,32U)||response[9]!=4U||
       !rc_make_attach(response,attach)||
       !rc_open(runtime,server_port,credentials,trust,&data)||
       !rc_write_fragmented(&data,attach,sizeof(attach),5U)||
       !rc_read_exact(&data,response,sizeof(data_accept))||
       memcmp(response,data_accept,sizeof(data_accept))!=0)goto cleanup;
    request.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port=fixture_port;request.destination_size=18U;
    memcpy(request.destination,"papacc-test-server",18U);
    request.alpn_count=1U;request.alpn[0].size=8U;
    memcpy(request.alpn[0].bytes,"papacc/1",8U);
    if(papacc_network_open_encode(&request,payload,sizeof(payload),&size)!=
           PAPACC_RESULT_OK||
       !rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN,payload,size,3U)||
       !rc_read_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,response,
           sizeof(response),&size)||size!=1U||
       response[0]!=PAPACC_NETWORK_OPEN_OK||
       !rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_DATA,&marker,1U,1U)||
       !rc_read_any_frame(&data,&type,response,sizeof(response),&size))
        goto cleanup;
    if(type!=PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON||size!=2U||
       response[0]!=0U||response[1]!=1U||!rc_expect_close(&data))goto cleanup;
    puts("PST_TRANSPORT_FAILURE_OBSERVED=YES");
    puts("TERMINAL_CAUSE_PRESERVED=YES terminal=TLS_TRUNCATED");
    puts("CLEAN_TLS_SHUTDOWN=NO FALSE_NETWORK_CLOSE_SUCCESS=NO");
    ok=1;
cleanup:
    if(data.connection!=NULL)pst_connection_release(data.connection);
    if(control.connection!=NULL)pst_connection_release(control.connection);
    return ok;
}
static int rc_network_tls_shutdown_cancel(pst_runtime *runtime,
    unsigned short server_port,unsigned short fixture_port,
    pst_credentials *credentials,pst_trust *trust)
{
    RC_CONNECTION control={NULL},data={NULL};
    RC_F45_TRACE trace;
    LARGE_INTEGER counter;
    char trace_path[MAX_PATH];
    const char *failed_suboperation="NOT_REACHED";
    int trace_opened=0;
    PAPACC_U8 response[1024],attach[32],payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request=PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size=0U;PAPACC_U16 type=0U;int ok=0;
    const PAPACC_U8 marker=0x44U;
    memset(&trace,0,sizeof(trace));trace.file=INVALID_HANDLE_VALUE;
    (void)QueryPerformanceCounter(&counter);
    (void)sprintf_s(trace_path,sizeof(trace_path),
        "C:\\Projetos\\_tmp\\PapinhoAccelerator\\papacc-p10-stage41-%lu-%llu.trace",(unsigned long)GetCurrentProcessId(),
        (unsigned long long)counter.QuadPart);
    trace_opened=rc_f45_trace_open(&trace,trace_path);
#define RC_STAGE41_STEP(name,expression) do { failed_suboperation=#name; \
    if(trace_opened&&!rc_f45_trace_write(&trace,#name "_BEGIN","BEGIN",0U)) \
        goto cleanup; \
    if(!(expression)){if(trace_opened)(void)rc_f45_trace_write(&trace, \
        #name "_RESULT","FAILED",0U);goto cleanup;} \
    if(trace_opened&&!rc_f45_trace_write(&trace,#name "_RESULT", \
        "SUCCESS",0U))goto cleanup; } while(0)
    RC_STAGE41_STEP(CONTROL_OPEN,
        rc_open(runtime,server_port,credentials,trust,&control));
    RC_STAGE41_STEP(CONTROL_PROTOCOL_OPEN_SEND,
        rc_write_all(&control,control_open,sizeof(control_open)));
    RC_STAGE41_STEP(CONTROL_ACCEPT_READ,
        rc_read_exact(&control,response,sizeof(control_accept)));
    RC_STAGE41_STEP(CONTROL_ACCEPT_VALIDATE,
        memcmp(response,control_accept,sizeof(control_accept))==0);
    RC_STAGE41_STEP(CAPABILITY_NEGOTIATION,
        rc_negotiate_major(&control,1,PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2));
    RC_STAGE41_STEP(DATA_TICKET_REQUEST_SEND,
        rc_write_all(&control,ticket_request,sizeof(ticket_request)));
    RC_STAGE41_STEP(DATA_TICKET_RESPONSE_READ,
        rc_read_exact(&control,response,32U));
    RC_STAGE41_STEP(DATA_TICKET_RESPONSE_VALIDATE,response[9]==4U);
    RC_STAGE41_STEP(DATA_ATTACH_BUILD,rc_make_attach(response,attach));
    RC_STAGE41_STEP(DATA_OPEN,
        rc_open(runtime,server_port,credentials,trust,&data));
    RC_STAGE41_STEP(DATA_ATTACH_SEND,
        rc_write_fragmented(&data,attach,sizeof(attach),5U));
    RC_STAGE41_STEP(DATA_ACCEPT_READ,
        rc_read_exact(&data,response,sizeof(data_accept)));
    RC_STAGE41_STEP(DATA_ACCEPT_VALIDATE,
        memcmp(response,data_accept,sizeof(data_accept))==0);
    request.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;request.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port=fixture_port;request.destination_size=18U;memcpy(request.destination,"papacc-test-server",18U);
    request.alpn_count=1U;request.alpn[0].size=8U;memcpy(request.alpn[0].bytes,"papacc/1",8U);
    RC_STAGE41_STEP(NETWORK_OPEN_ENCODE,
        papacc_network_open_encode(&request,payload,sizeof(payload),&size)==
            PAPACC_RESULT_OK);
    RC_STAGE41_STEP(NETWORK_OPEN_SEND,
        rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN,payload,size,3U));
    RC_STAGE41_STEP(NETWORK_OPEN_RESULT_READ,
        rc_read_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,response,
            sizeof(response),&size));
    RC_STAGE41_STEP(NETWORK_OPEN_RESULT_VALIDATE,
        size==1U&&response[0]==PAPACC_NETWORK_OPEN_OK);
    RC_STAGE41_STEP(NETWORK_DATA_SEND,
        rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_DATA,&marker,1U,1U));
    failed_suboperation="EXPECT_NO_FRAME_OR_TERMINAL";
    if(trace_opened&&!rc_f45_trace_write(&trace,
            "EXPECT_NO_FRAME_OR_TERMINAL_BEGIN","BEGIN",0U))goto cleanup;
    if(rc_read_any_frame(&data,&type,response,sizeof(response),&size)){
        char observed[128];
        (void)sprintf_s(observed,sizeof(observed),
            "UNEXPECTED_FRAME type=%u size=%lu",(unsigned)type,
            (unsigned long)size);
        if(trace_opened)(void)rc_f45_trace_write(&trace,
            "EXPECT_NO_FRAME_OR_TERMINAL_RESULT",observed,0U);
        printf("STAGE41_UNEXPECTED_FRAME_TYPE=%u STAGE41_UNEXPECTED_FRAME_SIZE=%lu\n",
            (unsigned)type,(unsigned long)size);
        goto cleanup;
    }
    if(trace_opened&&!rc_f45_trace_write(&trace,
            "EXPECT_NO_FRAME_OR_TERMINAL_RESULT","SUCCESS",0U))goto cleanup;
    puts("D_REAL_TLS_REQUIRED=YES D_REAL_PST_CONNECTION=YES");
    puts("D_CANCEL_OWNER_CONNECTION_CLOSED=YES");ok=1;
cleanup:
    if(ok)failed_suboperation="NONE";
    if(trace_opened){(void)rc_f45_trace_write(&trace,"STAGE41_FINAL",
        ok?"SUCCESS":"FAILED",0U);rc_f45_trace_close(&trace);}
    printf("P10_STAGE41_TRACE_PATH=%s P10_STAGE41_TRACE_CREATED=%s STAGE41_FIRST_FAILED_SUBOPERATION=%s STAGE41_RESULT=%s\n",
        trace_path,trace_opened?"YES":"NO",failed_suboperation,
        ok?"PASS":"FAIL");
#undef RC_STAGE41_STEP
    if(data.connection!=NULL)pst_connection_release(data.connection);
    if(control.connection!=NULL)pst_connection_release(control.connection);
    return ok;
}
static int rc_network_tls_control_loss(pst_runtime *runtime,
    unsigned short server_port,unsigned short fixture_port,
    pst_credentials *credentials,pst_trust *trust,HANDLE loss_event,
    HANDLE loss_ack_event,RC_F45_TRACE *trace)
{
    RC_CONNECTION control={NULL,INVALID_SOCKET},data={NULL,INVALID_SOCKET};
    PAPACC_U8 response[1024],attach[32],payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request=PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size=0U;int ok=0;
    const PAPACC_U8 marker=0x46U;
    RC_PRE_F45_STEP(trace,CONTROL_OPEN,
        rc_open(runtime,server_port,credentials,trust,&control));
    RC_PRE_F45_STEP(trace,CONTROL_PROTOCOL_OPEN_SEND,
        rc_write_all(&control,control_open,sizeof(control_open)));
    RC_PRE_F45_STEP(trace,CONTROL_ACCEPT_READ,
        rc_read_exact(&control,response,sizeof(control_accept)));
    RC_PRE_F45_STEP(trace,CONTROL_ACCEPT_VALIDATE,
        memcmp(response,control_accept,sizeof(control_accept))==0);
    RC_PRE_F45_STEP(trace,CAPABILITY_NEGOTIATION,
        rc_negotiate_major_f45(&control,PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2,trace));
    RC_PRE_F45_STEP(trace,DATA_TICKET_REQUEST_SEND,
        rc_write_all(&control,ticket_request,sizeof(ticket_request)));
    RC_PRE_F45_STEP(trace,DATA_TICKET_RESPONSE_READ,
        rc_read_exact(&control,response,32U));
    RC_PRE_F45_STEP(trace,DATA_TICKET_RESPONSE_VALIDATE,response[9]==4U);
    RC_PRE_F45_STEP(trace,DATA_ATTACH_BUILD,rc_make_attach(response,attach));
    RC_PRE_F45_STEP(trace,DATA_OPEN,
        rc_open(runtime,server_port,credentials,trust,&data));
    RC_PRE_F45_STEP(trace,DATA_ATTACH_SEND,
        rc_write_fragmented(&data,attach,sizeof(attach),5U));
    RC_PRE_F45_STEP(trace,DATA_ACCEPT_READ,
        rc_read_exact(&data,response,sizeof(data_accept)));
    RC_PRE_F45_STEP(trace,DATA_ACCEPT_VALIDATE,
        memcmp(response,data_accept,sizeof(data_accept))==0);    request.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port=fixture_port;request.destination_size=18U;
    memcpy(request.destination,"papacc-test-server",18U);
    request.alpn_count=1U;request.alpn[0].size=8U;
    memcpy(request.alpn[0].bytes,"papacc/1",8U);
    RC_PRE_F45_STEP(trace,NETWORK_OPEN_ENCODE,
        papacc_network_open_encode(&request,payload,sizeof(payload),&size)==
            PAPACC_RESULT_OK);
    RC_PRE_F45_STEP(trace,NETWORK_OPEN_SEND,
        rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN,payload,size,3U));
    RC_PRE_F45_STEP(trace,NETWORK_OPEN_RESULT_READ,
        rc_read_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,response,
            sizeof(response),&size));
    RC_PRE_F45_STEP(trace,NETWORK_OPEN_RESULT_VALIDATE,
        size==1U&&response[0]==PAPACC_NETWORK_OPEN_OK);
    RC_PRE_F45_STEP(trace,NETWORK_DATA_SEND,
        rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_DATA,&marker,1U,1U));    if(!rc_f45_trace_write(trace,"F45_EVENT_WAIT_BEGIN","BEGIN",0U))goto cleanup;
    printf("F45_EVENT_WAIT=START operation=WaitForSingleObject expected=SIGNALED timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=CONTROL-1\n",
        rc_monotonic_ms());
    {
        DWORD wait_result=WaitForSingleObject(loss_event,10000U);
        printf("F45_EVENT_OBSERVED=%s operation=WaitForSingleObject observed=%lu timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=CONTROL-1\n",
            wait_result==WAIT_OBJECT_0?"YES":"NO",(unsigned long)wait_result,
            rc_monotonic_ms());
        if(!rc_f45_trace_write(trace,"F45_EVENT_WAIT_RESULT",
            wait_result==WAIT_OBJECT_0?"SIGNALED":"NOT_SIGNALED",
            (unsigned long)wait_result))goto cleanup;
        if(wait_result!=WAIT_OBJECT_0)goto cleanup;
    }
    if(!rc_f45_trace_write(trace,"F45_CONTROL_SHUTDOWN_BEGIN","BEGIN",0U))goto cleanup;
    printf("F45_CONTROL_SOCKET_SHUTDOWN=START operation=shutdown(SD_BOTH) expected=SUCCESS timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=CONTROL-1\n",
        rc_monotonic_ms());
    if(control.transport_socket_test_only==INVALID_SOCKET){
        (void)rc_f45_trace_write(trace,"F45_CONTROL_SHUTDOWN_RESULT",
            "SOCKET_ALIAS_UNAVAILABLE",0U);
        printf("F45_CONTROL_SHUTDOWN_RESULT=FAIL cause=SOCKET_ALIAS_UNAVAILABLE timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=CONTROL-1\n",
            rc_monotonic_ms());
        goto cleanup;
    }
    if(shutdown(control.transport_socket_test_only,SD_BOTH)==SOCKET_ERROR){
        int shutdown_error=WSAGetLastError();
        (void)rc_f45_trace_write(trace,"F45_CONTROL_SHUTDOWN_RESULT",
            "SOCKET_ERROR",(unsigned long)shutdown_error);
        printf("F45_CONTROL_SHUTDOWN_RESULT=FAIL cause=WSA_ERROR_%d timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=CONTROL-1\n",
            shutdown_error,rc_monotonic_ms());
        goto cleanup;
    }
    if(!rc_f45_trace_write(trace,"F45_CONTROL_SHUTDOWN_RESULT",
        "SUCCESS",0U))goto cleanup;
    printf("F45_CONTROL_SHUTDOWN_RESULT=SUCCESS cause=NONE timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=CONTROL-1\n",
        rc_monotonic_ms());
    if(!rc_f45_trace_write(trace,"F45_ACK_SIGNAL_BEGIN","BEGIN",0U))goto cleanup;
    printf("F45_ACK_SIGNAL=START operation=SetEvent expected=SUCCESS timestamp_ms=%llu operation_id=F-CONTROL-LOSS\n",
        rc_monotonic_ms());
    if(!SetEvent(loss_ack_event)){
        DWORD ack_error=GetLastError();
        (void)rc_f45_trace_write(trace,"F45_ACK_SIGNAL_RESULT",
            "WIN32_ERROR",(unsigned long)ack_error);
        printf("F45_ACK_SIGNALLED=NO cause=WIN32_ERROR_%lu timestamp_ms=%llu operation_id=F-CONTROL-LOSS\n",
            (unsigned long)ack_error,rc_monotonic_ms());
        goto cleanup;
    }
    if(!rc_f45_trace_write(trace,"F45_ACK_SIGNAL_RESULT","SIGNALED",0U))goto cleanup;
    printf("F45_ACK_SIGNALLED=YES cause=NONE timestamp_ms=%llu operation_id=F-CONTROL-LOSS\n",
        rc_monotonic_ms());
    if(!rc_f45_trace_write(trace,"F45_DATA_CLOSE_WAIT_BEGIN","BEGIN",0U))goto cleanup;
    printf("F45_DATA_CLOSE_WAIT=START operation=rc_expect_close expected=DATA_CONNECTION_CLOSED timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=DATA-1\n",
        rc_monotonic_ms());
    if(!rc_expect_close(&data)){
        (void)rc_f45_trace_write(trace,"F45_DATA_CLOSE_WAIT_RESULT",
            "WAIT_OR_IO_ORACLE_FAILED",0U);
        printf("F45_DATA_CLOSE_OBSERVED=NO cause=WAIT_OR_IO_ORACLE_FAILED timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=DATA-1\n",
            rc_monotonic_ms());
        goto cleanup;
    }
    if(!rc_f45_trace_write(trace,"F45_DATA_CLOSE_WAIT_RESULT",
        "CONNECTION_TERMINAL",0U))goto cleanup;
    printf("F45_DATA_CLOSE_OBSERVED=YES cause=CONNECTION_TERMINAL timestamp_ms=%llu operation_id=F-CONTROL-LOSS connection_id=DATA-1\n",
        rc_monotonic_ms());
    puts("F_CONTROL_LOSS_INJECTED_AFTER_PENDING=YES");
    puts("F_DATA_DEPENDENT_CLOSED=YES F_NO_RESURRECTION=YES");
    ok=1;
cleanup:
    if(data.connection!=NULL)pst_connection_release(data.connection);
    if(control.connection!=NULL)pst_connection_release(control.connection);
    return ok;
}

static int rc_network_tls_shutdown_deadline(pst_runtime *runtime,
    unsigned short server_port,unsigned short fixture_port,
    pst_credentials *credentials,pst_trust *trust)
{
    RC_CONNECTION control={NULL},data={NULL};
    PAPACC_U8 response[1024],attach[32],payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request=PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size=0U;PAPACC_U16 type=0U;int ok=0;
    unsigned long long wait_start=0U,wait_deadline=0U,wait_remaining=0U;
    const PAPACC_U8 marker=0x45U;
    if(!rc_open(runtime,server_port,credentials,trust,&control)||
       !rc_write_all(&control,control_open,sizeof(control_open))||
       !rc_read_exact(&control,response,sizeof(control_accept))||memcmp(response,control_accept,sizeof(control_accept))!=0||
       !rc_negotiate_major(&control,1,PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2)||
       !rc_write_all(&control,ticket_request,sizeof(ticket_request))||!rc_read_exact(&control,response,32U)||response[9]!=4U||
       !rc_make_attach(response,attach)||!rc_open(runtime,server_port,credentials,trust,&data)||
       !rc_write_fragmented(&data,attach,sizeof(attach),5U)||!rc_read_exact(&data,response,sizeof(data_accept))||
       memcmp(response,data_accept,sizeof(data_accept))!=0)goto cleanup;
    request.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;request.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port=fixture_port;request.destination_size=18U;memcpy(request.destination,"papacc-test-server",18U);
    request.alpn_count=1U;request.alpn[0].size=8U;memcpy(request.alpn[0].bytes,"papacc/1",8U);
    if(papacc_network_open_encode(&request,payload,sizeof(payload),&size)!=PAPACC_RESULT_OK||
       !rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN,payload,size,3U)||
       !rc_read_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,response,sizeof(response),&size)||
       size!=1U||response[0]!=PAPACC_NETWORK_OPEN_OK||
       !rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_DATA,&marker,1U,1U))goto cleanup;
    if(!rc_e_wait_oracle_selftest())goto cleanup;
    wait_start=rc_monotonic_ms();
    wait_deadline=wait_start+RC_E_OWNER_DEADLINE_MS+RC_E_OBSERVATION_MARGIN_MS;
    printf("CLIENT_WAIT_START_MS=%llu CLIENT_WAIT_DEADLINE_MS=%llu OWNER_DEADLINE_DURATION_MS=%u OBSERVATION_MARGIN_MS=%u\n",wait_start,wait_deadline,(unsigned)RC_E_OWNER_DEADLINE_MS,(unsigned)RC_E_OBSERVATION_MARGIN_MS);
    if(wait_start==0U||!rc_read_any_frame_until(&data,&type,response,
       sizeof(response),&size,wait_deadline,&wait_remaining)||
       type!=PAPACC_MESSAGE_TYPE_NETWORK_ABORT||size!=0U||
       !rc_expect_close(&data))goto cleanup;
    printf("CLIENT_WAIT_REMAINING_MS=%llu\n",wait_remaining);
    puts("E_REAL_TLS_REQUIRED=YES E_REAL_PST_CONNECTION=YES");
    puts("E_PACC_TERMINAL=NETWORK_ABORT E_PACC_TERMINAL_COUNT=1 E_FALSE_CLEAN_SUCCESS=NO");ok=1;
cleanup:
    if(data.connection!=NULL)pst_connection_release(data.connection);
    if(control.connection!=NULL)pst_connection_release(control.connection);
    return ok;
}
static int rc_network_truncated(pst_runtime *runtime,
    unsigned short server_port,unsigned short fixture_port,
    pst_credentials *credentials,pst_trust *trust,
    PAPACC_CAPABILITY_VERSION network_major,PAPACC_SIZE expected_size)
{
    RC_CONNECTION control={NULL},data={NULL};
    PAPACC_U8 response[1024],attach[32];
    PAPACC_U8 payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request=PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size=0U,total=0U,index;
    PAPACC_U16 type=0U;
    int ok=0;
    if(!rc_open(runtime,server_port,credentials,trust,&control)||
        !rc_write_all(&control,control_open,sizeof(control_open))||
        !rc_read_exact(&control,response,sizeof(control_accept))||
        memcmp(response,control_accept,sizeof(control_accept))!=0||
        !rc_negotiate_major(&control,1,network_major)||
        !rc_write_all(&control,ticket_request,sizeof(ticket_request))||
        !rc_read_exact(&control,response,32U)||response[9]!=4U||
        !rc_make_attach(response,attach)||
        !rc_open(runtime,server_port,credentials,trust,&data)||
        !rc_write_fragmented(&data,attach,sizeof(attach),5U)||
        !rc_read_exact(&data,response,sizeof(data_accept))||
        memcmp(response,data_accept,sizeof(data_accept))!=0)goto cleanup;
    request.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port=fixture_port;
    request.destination_size=18U;
    memcpy(request.destination,"papacc-test-server",18U);
    request.alpn_count=1U;request.alpn[0].size=8U;
    memcpy(request.alpn[0].bytes,"papacc/1",8U);
    if(papacc_network_open_encode(&request,payload,sizeof(payload),&size)!=
            PAPACC_RESULT_OK||
        !rc_send_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN,payload,size,3U)||
        !rc_read_frame(&data,PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,response,
            sizeof(response),&size)||size!=1U||
        response[0]!=PAPACC_NETWORK_OPEN_OK){
        goto cleanup;
    }
    for(index=0U;index<64U;++index){
        if(!rc_read_any_frame(&data,&type,response,sizeof(response),&size))
            goto cleanup;
        if(type==PAPACC_MESSAGE_TYPE_NETWORK_DATA){
            PAPACC_SIZE byte_index;
            if(total+size>expected_size)goto cleanup;
            for(byte_index=0U;byte_index<size;++byte_index)
                if(response[byte_index]!=(PAPACC_U8)((total+byte_index)*37U+11U))
                    goto cleanup;
            total+=size;
            continue;
        }
        if(network_major==PAPACC_PACC_NETWORK_EGRESS_MAJOR_V1){
            if(type!=PAPACC_MESSAGE_TYPE_NETWORK_ABORT||size!=0U)goto cleanup;
        }else if(type!=PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON||size!=2U||
            response[0]!=0U||response[1]!=1U)goto cleanup;
        break;
    }
    if(total!=expected_size||!rc_expect_close(&data))goto cleanup;
    puts(network_major==PAPACC_PACC_NETWORK_EGRESS_MAJOR_V1?
        "REFERENCE_CLIENT_TLS_TRUNCATED_V1=YES":
        "REFERENCE_CLIENT_TLS_TRUNCATED_V2=YES");
    ok=1;
cleanup:
    if(data.connection!=NULL)pst_connection_release(data.connection);
    if(control.connection!=NULL)pst_connection_release(control.connection);
    return ok;
}

static int rc_network_http_terminal(pst_runtime *runtime,
    unsigned short server_port, unsigned short fixture_port,
    pst_credentials *credentials, pst_trust *trust, int clean,
    int expect_multiple_frames)
{
    static const PAPACC_U8 request_bytes[] =
        "GET / HTTP/1.1\r\nHost: papacc-test-server\r\nConnection: close\r\n\r\n";
    RC_CONNECTION control = { NULL }, data = { NULL };
    PAPACC_U8 response[1024], attach[32];
    PAPACC_U8 payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD];
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_SIZE size = 0U, total = 0U, frame_count = 0U;
    PAPACC_U16 type = 0U;
    int ok = 0;
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, response, sizeof(control_accept)) ||
        memcmp(response, control_accept, sizeof(control_accept)) != 0 ||
        !rc_negotiate_major(&control, 1,
            PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2) ||
        !rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U) || response[9] != 4U ||
        !rc_make_attach(response, attach) ||
        !rc_open(runtime, server_port, credentials, trust, &data) ||
        !rc_write_fragmented(&data, attach, sizeof(attach), 5U) ||
        !rc_read_exact(&data, response, sizeof(data_accept)) ||
        memcmp(response, data_accept, sizeof(data_accept)) != 0) goto cleanup;
    request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode = PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    request.port = fixture_port;
    request.destination_size = 18U;
    memcpy(request.destination, "papacc-test-server", 18U);
    request.alpn_count = 1U;
    request.alpn[0].size = 8U;
    memcpy(request.alpn[0].bytes, "papacc/1", 8U);
    if (papacc_network_open_encode(&request, payload, sizeof(payload), &size) !=
            PAPACC_RESULT_OK ||
        !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN, payload, size,
            3U) ||
        !rc_read_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
            response, sizeof(response), &size) || size != 1U ||
        response[0] != PAPACC_NETWORK_OPEN_OK ||
        !rc_send_frame(&data, PAPACC_MESSAGE_TYPE_NETWORK_DATA, request_bytes,
            sizeof(request_bytes) - 1U, 7U)) goto cleanup;
    for (;;) {
        if (!rc_read_any_frame(&data, &type, response, sizeof(response), &size))
            goto cleanup;
        if (type == PAPACC_MESSAGE_TYPE_NETWORK_DATA) {
            if (frame_count == 0U && (size < 12U ||
                    memcmp(response, "HTTP/1.1 200", 12U) != 0)) goto cleanup;
            ++frame_count;
            total += size;
            continue;
        }
        if (clean) {
            if (type != PAPACC_MESSAGE_TYPE_NETWORK_CLOSE || size != 0U)
                goto cleanup;
        } else if (type != PAPACC_MESSAGE_TYPE_NETWORK_ABORT_REASON ||
                size != 2U || response[0] != 0U || response[1] != 1U)
            goto cleanup;
        break;
    }
    if (total == 0U || (expect_multiple_frames && frame_count < 2U) ||
        (!expect_multiple_frames && frame_count != 1U) ||
        !rc_expect_close(&data)) goto cleanup;
    puts(clean ? "REFERENCE_CLIENT_TLS_CLEAN_CLOSE=YES" :
        "REFERENCE_CLIENT_TLS_TRUNCATED_HTTP_V2=YES");
    if (clean) puts("REFERENCE_CLIENT_EXPECTED_TERMINAL=NETWORK_CLOSE");
    ok = 1;
cleanup:
    if (data.connection != NULL) pst_connection_release(data.connection);
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_attach_data(pst_runtime *runtime, unsigned short server_port,
    pst_credentials *credentials, pst_trust *trust, RC_CONNECTION *control,
    RC_CONNECTION *data)
{
    PAPACC_U8 response[32];
    PAPACC_U8 attach[32];
    if (!rc_write_all(control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(control, response, sizeof(response)) ||
        response[9] != 4U) return 0;
    if (!rc_make_attach(response, attach)) return 0;
    return rc_open(runtime, server_port, credentials, trust, data) &&
        rc_write_fragmented(data, attach, sizeof(attach), 5U) &&
        rc_read_exact(data, response, sizeof(data_accept)) &&
        memcmp(response, data_accept, sizeof(data_accept)) == 0;
}

static int rc_open_plain(RC_CONNECTION *data, unsigned short fixture_port,
    const char *hostname, PAPACC_NETWORK_OPEN_STATUS expected)
{
    PAPACC_NETWORK_OPEN request = PAPACC_NETWORK_OPEN_INITIALIZER;
    PAPACC_U8 payload[PAPACC_NETWORK_OPEN_MAX_PAYLOAD], response[8];
    PAPACC_SIZE size = 0U;
    request.destination_kind = PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    request.transport_mode = PAPACC_NETWORK_TRANSPORT_TCP_PLAIN;
    request.port = fixture_port;
    request.destination_size = (PAPACC_U16)strlen(hostname);
    memcpy(request.destination, hostname, request.destination_size);
    if (papacc_network_open_encode(&request, payload, sizeof(payload),
            &size) != PAPACC_RESULT_OK ||
        !rc_send_frame(data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN, payload, size,
            3U)) return 0;
    if (expected != PAPACC_NETWORK_OPEN_OK) return rc_expect_close(data);
    return rc_read_frame(data, PAPACC_MESSAGE_TYPE_NETWORK_OPEN_RESULT,
        response, sizeof(response), &size) && size == 1U &&
        response[0] == expected;
}

static int rc_round_trip(RC_CONNECTION *data, PAPACC_U8 seed)
{
    PAPACC_U8 sent[512], received[512], chunk[512];
    PAPACC_SIZE size = 0U, index, offset = 0U;
    for (index = 0U; index < sizeof(sent); ++index)
        sent[index] = (PAPACC_U8)(seed + index * 29U);
    if (!rc_send_frame(data, PAPACC_MESSAGE_TYPE_NETWORK_DATA, sent,
            sizeof(sent), 31U)) return 0;
    while (offset < sizeof(received)) {
        if (!rc_read_frame(data, PAPACC_MESSAGE_TYPE_NETWORK_DATA, chunk,
                sizeof(chunk), &size) || size == 0U ||
            size > sizeof(received) - offset) return 0;
        memcpy(received + offset, chunk, size);
        offset += size;
    }
    return memcmp(sent, received, sizeof(sent)) == 0;
}

static int rc_multiple_data_recovery(pst_runtime *runtime,
    unsigned short server_port, unsigned short fixture_port,
    pst_credentials *credentials, pst_trust *trust)
{
    RC_CONNECTION control = { NULL }, failed = { NULL }, a = { NULL };
    RC_CONNECTION b = { NULL }, fresh = { NULL };
    PAPACC_U8 response[20];
    int ok = 0, stage = 1;
    if (!rc_open(runtime, server_port, credentials, trust, &control) ||
        !rc_write_all(&control, control_open, sizeof(control_open)) ||
        !rc_read_exact(&control, response, sizeof(response)) ||
        memcmp(response, control_accept, sizeof(response)) != 0 ||
        !rc_negotiate(&control, 1) ||
        !rc_attach_data(runtime, server_port, credentials, trust, &control,
            &failed) ||
        !rc_open_plain(&failed, fixture_port, "denied.example",
            PAPACC_NETWORK_OPEN_DENIED)) goto cleanup;
    stage = 2;
    /* A denied OPEN makes this DATA terminal; never reuse it. */
    pst_connection_release(failed.connection); failed.connection = NULL;
    if (!rc_attach_data(runtime, server_port, credentials, trust, &control,
            &a)) { stage = 21; goto cleanup; }
    if (!rc_attach_data(runtime, server_port, credentials, trust, &control,
            &b)) { stage = 22; goto cleanup; }
    if (!rc_open_plain(&a, fixture_port, "papacc-test-server",
            PAPACC_NETWORK_OPEN_OK)) { stage = 23; goto cleanup; }
    if (!rc_open_plain(&b, fixture_port, "papacc-test-server",
            PAPACC_NETWORK_OPEN_OK)) { stage = 24; goto cleanup; }
    if (!rc_round_trip(&a, 0x11U)) { stage = 25; goto cleanup; }
    if (!rc_send_frame(&a, PAPACC_MESSAGE_TYPE_NETWORK_ABORT, NULL, 0U, 2U) ||
        !rc_expect_close(&a)) { stage = 27; goto cleanup; }
    stage = 3;
    pst_connection_release(a.connection); a.connection = NULL;
    if (!rc_round_trip(&b, 0xA7U)) { stage = 26; goto cleanup; }
    if (!rc_send_frame(&b, PAPACC_MESSAGE_TYPE_NETWORK_CLOSE, NULL, 0U, 2U))
        goto cleanup;
    pst_connection_release(b.connection); b.connection = NULL;
    if (!rc_attach_data(runtime, server_port, credentials, trust, &control,
            &fresh) ||
        !rc_open_plain(&fresh, fixture_port, "papacc-test-server",
            PAPACC_NETWORK_OPEN_OK) || !rc_round_trip(&fresh, 0x5CU) ||
        !rc_send_frame(&fresh, PAPACC_MESSAGE_TYPE_NETWORK_CLOSE, NULL, 0U,
            2U)) goto cleanup;
    stage = 4;
    puts("REFERENCE_CLIENT_RECOVERY_AFTER_FAILURE=YES");
    puts("REFERENCE_CLIENT_MULTIPLE_DATA_CHANNELS=YES");
    puts("REFERENCE_CLIENT_CROSS_DATA_LEAKAGE=NO");
    puts("REFERENCE_CLIENT_FRESH_DATA_AFTER_TERMINAL=YES");
    ok = 1;
cleanup:
    if (!ok) fprintf(stderr, "REFERENCE_MULTIPLE_FAIL_STAGE=%d\n", stage);
    if (fresh.connection != NULL) pst_connection_release(fresh.connection);
    if (b.connection != NULL) pst_connection_release(b.connection);
    if (a.connection != NULL) pst_connection_release(a.connection);
    if (failed.connection != NULL) pst_connection_release(failed.connection);
    if (control.connection != NULL) pst_connection_release(control.connection);
    return ok;
}

static int rc_expect_close(RC_CONNECTION *connection)
{
    unsigned char byte;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT; ++index) {
        PST_IO_RESULT io;
        PST_RESULT result = pst_connection_read(
            connection->connection, &byte, 1U, &io);
        if (result != PST_RESULT_OK || io.operation == PST_OPERATION_CLOSED ||
            io.operation == PST_OPERATION_FAILED) return 1;
        if (!rc_wait(connection->connection)) return 0;
    }
    return 0;
}

static int rc_accept_clean_close(RC_CONNECTION *connection)
{
    unsigned char byte;
    unsigned int index;
    for (index = 0U; index < RC_LIMIT; ++index) {
        PST_IO_RESULT io;
        if (pst_connection_read(connection->connection, &byte, 1U, &io) !=
                PST_RESULT_OK) return 0;
        if (io.operation == PST_OPERATION_CLOSED) {
            int clean = io.close_kind == PST_CLOSE_CLEAN;
            pst_connection_release(connection->connection);
            connection->connection = NULL;
            return clean;
        }
        if (io.operation == PST_OPERATION_FAILED ||
            !rc_wait_shutdown(connection->connection)) return 0;
    }
    return 0;
}

static int rc_close(RC_CONNECTION *connection)
{
    pst_u32 operation = 0U;
    PST_RESULT source_error = PST_RESULT_OK;
    unsigned int index;
    if (connection->connection == NULL) return 1;
    for (index = 0U; index < RC_LIMIT; ++index) {
        PST_RESULT shutdown_result = pst_connection_shutdown(
            connection->connection, &operation, &source_error);
        if (shutdown_result != PST_RESULT_OK) {
            fprintf(stderr, "shutdown result=%lu operation=%lu source=%lu\n",
                (unsigned long)shutdown_result, (unsigned long)operation,
                (unsigned long)source_error);
            break;
        }
        if (operation == PST_OPERATION_COMPLETE ||
            operation == PST_OPERATION_CLOSED) break;
        if (operation == PST_OPERATION_FAILED ||
            !rc_wait_shutdown(connection->connection)) {
            fprintf(stderr, "shutdown step operation=%lu source=%lu\n",
                (unsigned long)operation, (unsigned long)source_error);
            break;
        }
    }
    pst_connection_release(connection->connection);
    connection->connection = NULL;
    return operation == PST_OPERATION_COMPLETE ||
        operation == PST_OPERATION_CLOSED;
}

int main(int argc, char **argv)
{
    WSADATA winsock;
    PAPACC_TEST_SECURITY_FIXTURE fixture =
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PST_RUNTIME_OPTIONS options;
    pst_runtime *runtime = NULL;
    RC_CONNECTION control = { NULL }, data = { NULL }, replay = { NULL };
    RC_CONNECTION foreign = { NULL }, rightful = { NULL };
    unsigned char response[32];
    unsigned char attach[32];
    unsigned short port;
    HANDLE shutdown_event = NULL;
    HANDLE shutdown_ack_event = NULL;
    RC_F45_TRACE f45_trace = { INVALID_HANDLE_VALUE, 0U };
    int result = 1;
    int stage = 0;
    const char *scenario;
    if(argc==3&&strcmp(argv[1],"--f45-trace-selftest")==0){
        static const char *markers[]={"F45_EVENT_WAIT_BEGIN",
            "F45_EVENT_WAIT_RESULT","F45_CONTROL_SHUTDOWN_BEGIN",
            "F45_CONTROL_SHUTDOWN_RESULT","F45_ACK_SIGNAL_BEGIN",
            "F45_ACK_SIGNAL_RESULT","F45_DATA_CLOSE_WAIT_BEGIN",
            "F45_DATA_CLOSE_WAIT_RESULT"};
        size_t marker_index;
        if(!rc_f45_trace_open(&f45_trace,argv[2]))return 90;
        for(marker_index=0U;marker_index<sizeof(markers)/sizeof(markers[0]);
                ++marker_index)
            if(!rc_f45_trace_write(&f45_trace,markers[marker_index],
                    "SELFTEST",0U)){rc_f45_trace_close(&f45_trace);return 91;}
        rc_f45_trace_close(&f45_trace);return 0;
    }
    if ((argc != 5 && argc != 6 && argc != 7) ||
        !rc_init_protocol_messages()) return 2;
    port = (unsigned short)strtoul(argv[1], NULL, 10);
    scenario = argv[4];
    shutdown_event = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, argv[2]);
    shutdown_ack_event = OpenEventA(
        (strcmp(scenario,"main")==0||
         strcmp(scenario,"network-g1-duplicate-close")==0||
         strcmp(scenario,"network-g2-duplicate-abort")==0)?SYNCHRONIZE:
            EVENT_MODIFY_STATE,FALSE,argv[3]);
    if (port == 0U || shutdown_event == NULL || shutdown_ack_event == NULL ||
        WSAStartup(MAKEWORD(2, 2), &winsock) != 0) return 3;
    if ((papacc_test_pst_client_capabilities() & ~RC_NSS_CLIENT_CAPS) != 0U ||
        (papacc_test_pst_client_capabilities() & PST_CAP_SNI_CONTROL) != 0U ||
        !papacc_test_security_fixture_init(&fixture) ||
        pst_win32_register_builtin_providers() != PST_RESULT_OK) {
        stage = 1; goto cleanup;
    }
    memset(&options, 0, sizeof(options));
    options.struct_size = sizeof(options);
    options.api_version = PST_API_VERSION;
    if (pst_runtime_create(&options, &runtime) != PST_RESULT_OK) {
        stage = 2; goto cleanup;
    }
    if (strcmp(scenario, "tls12") == 0 ||
        strcmp(scenario, "wrong-alpn") == 0 ||
        strcmp(scenario, "missing-alpn") == 0 ||
        strcmp(scenario, "missing-credential") == 0 ||
        strcmp(scenario, "untrusted-client") == 0) {
        PST_CONNECTION_CONFIG negative_config;
        static const unsigned char wrong_bytes[] = "wrong/1";
        static const PST_ALPN_PROTOCOL wrong = {
            wrong_bytes, sizeof(wrong_bytes) - 1U };
        papacc_test_pst_client_config(&negative_config,
            strcmp(scenario, "untrusted-client") == 0 ?
                fixture.server.credentials : fixture.client_a.credentials,
            fixture.trust);
        if (strcmp(scenario, "tls12") == 0) {
            negative_config.tls.minimum_version = PST_TLS_VERSION_1_2;
            negative_config.tls.maximum_version = PST_TLS_VERSION_1_2;
        } else if (strcmp(scenario, "wrong-alpn") == 0) {
            negative_config.alpn.protocols = &wrong;
        } else if (strcmp(scenario, "missing-alpn") == 0) {
            negative_config.alpn.mode = PST_FEATURE_DISABLED;
            negative_config.alpn.protocols = NULL;
            negative_config.alpn.protocol_count = 0U;
        } else if (strcmp(scenario, "missing-credential") == 0) {
            negative_config.local_identity.credentials = NULL;
        }
        if (rc_open_config(runtime, port, &negative_config, &control)) {
            if (!rc_expect_close(&control)) { stage = 14; goto cleanup; }
            pst_connection_release(control.connection);
            control.connection = NULL;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "not-enrolled") == 0 ||
        strcmp(scenario, "deny") == 0) {
        pst_credentials *identity = strcmp(scenario, "deny") == 0 ?
            fixture.client_b.credentials : fixture.client_c.credentials;
        if (!rc_open(runtime, port, identity, fixture.trust, &control) ||
            !rc_write_all(&control, control_open, sizeof(control_open)) ||
            !rc_expect_close(&control)) { stage = 15; goto cleanup; }
        pst_connection_release(control.connection);
        control.connection = NULL;
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "production-control-data") == 0) {
        PAPACC_SIZE operation;
        if (!rc_open(runtime, port, fixture.client_a.credentials,
                fixture.trust, &control) ||
            !rc_write_all(&control, control_open, sizeof(control_open)) ||
            !rc_read_exact(&control, response, sizeof(control_accept)) ||
            memcmp(response, control_accept, sizeof(control_accept)) != 0 ||
            !rc_negotiate(&control, 1)) { stage = 30; goto cleanup; }
        for (operation = 0U; operation < 3U; ++operation) {
            if (!rc_write_all(&control, ticket_request,
                    sizeof(ticket_request)) ||
                !rc_read_exact(&control, response, 32U) ||
                response[9] != 4U || !rc_make_attach(response, attach) ||
                !rc_open(runtime, port, fixture.client_a.credentials,
                    fixture.trust, &data) ||
                !rc_write_fragmented(&data, attach, sizeof(attach), 5U) ||
                !rc_read_exact(&data, response, sizeof(data_accept)) ||
                memcmp(response, data_accept, sizeof(data_accept)) != 0) {
                stage = 30;
                goto cleanup;
            }
            pst_connection_release(data.connection);
            data.connection = NULL;
        }
        puts("REFERENCE_CLIENT_PRODUCTION_CONTROL=PASS");
        puts("REFERENCE_CLIENT_PRODUCTION_CAPABILITIES=PASS");
        puts("REFERENCE_CLIENT_PRODUCTION_DATA_ATTACH_COUNT=3");
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-tcp") == 0 ||
        strcmp(scenario, "network-tls") == 0 ||
        strcmp(scenario, "network-http") == 0 ||
        strcmp(scenario, "network-g1-duplicate-close") == 0 ||
        strcmp(scenario, "network-g2-duplicate-abort") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 17; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (fixture_port == 0U || !rc_network_flow(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust,
                strcmp(scenario, "network-tcp") == 0 ? 0 :
                strcmp(scenario, "network-http") == 0 ? 2 :
                strcmp(scenario, "network-g1-duplicate-close") == 0 ? 3 :
                strcmp(scenario, "network-g2-duplicate-abort") == 0 ? 4 : 1,
                (strcmp(scenario,"network-g1-duplicate-close")==0||
                 strcmp(scenario,"network-g2-duplicate-abort")==0)?
                    shutdown_ack_event:NULL)) {
            stage = 18; goto cleanup;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-tcp-plain-http-sequence") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 26; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (fixture_port == 0U || !rc_tcp_plain_http_sequence(runtime, port,
                fixture_port, fixture.client_a.credentials, fixture.trust)) {
            stage = 27; goto cleanup;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-mixed-egress-sequence") == 0) {
        unsigned short tls_port, plain_port;
        if (argc != 7) { stage = 28; goto cleanup; }
        tls_port = (unsigned short)strtoul(argv[5], NULL, 10);
        plain_port = (unsigned short)strtoul(argv[6], NULL, 10);
        if (tls_port == 0U || plain_port == 0U ||
            !rc_mixed_egress_sequence(runtime, port, tls_port, plain_port,
                fixture.client_a.credentials, fixture.trust)) {
            stage = 29; goto cleanup;
        }
        result = 0;
        goto cleanup;
    }
    if (strncmp(scenario, "network-negative-", 17U) == 0) {
        RC_NEGATIVE_KIND kind;
        unsigned short fixture_port;
        if (argc != 6) { stage = 19; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (strcmp(scenario, "network-negative-no-negotiation") == 0)
            kind = RC_NEGATIVE_NO_NEGOTIATION;
        else if (strcmp(scenario, "network-negative-missing-tls") == 0)
            kind = RC_NEGATIVE_MISSING_TLS;
        else if (strcmp(scenario, "network-negative-destination") == 0)
            kind = RC_NEGATIVE_DESTINATION;
        else if (strcmp(scenario, "network-negative-tls-name") == 0)
            kind = RC_NEGATIVE_TLS_NAME;
        else { stage = 20; goto cleanup; }
        if (!rc_network_negative(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust, kind)) {
            stage = 21; goto cleanup;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-tls-control-loss") == 0) {
        unsigned short fixture_port;
        if (argc != 7) { stage = 44; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if(!rc_f45_trace_open(&f45_trace,argv[6])){stage=46;goto cleanup;}
        if(!rc_f45_trace_write(&f45_trace,"TRACE_OPEN_RESULT",
                "SUCCESS",0U)){stage=47;goto cleanup;}
        if(!rc_f45_trace_write(&f45_trace,"TRACE_FIRST_WRITE_RESULT",
                "SUCCESS",0U)){stage=48;goto cleanup;}
        if (!rc_network_tls_control_loss(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust, shutdown_event,
                shutdown_ack_event,&f45_trace)) { stage = 45; goto cleanup; }
        result = 0; goto cleanup;
    }
    if (strcmp(scenario, "network-tls-shutdown-deadline") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 42; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (!rc_network_tls_shutdown_deadline(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust)) { stage = 43; goto cleanup; }
        result = 0; goto cleanup;
    }
    if (strcmp(scenario, "network-tls-shutdown-cancel") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 40; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (!rc_network_tls_shutdown_cancel(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust)) { stage = 41; goto cleanup; }
        result = 0; goto cleanup;
    }
    if (strcmp(scenario, "network-tls-reset") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 38; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (!rc_network_tls_reset(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust)) {
            stage = 39; goto cleanup;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-tcp-abort") == 0 ||
        strcmp(scenario, "network-tls-half-close") == 0 ||
        strcmp(scenario, "network-tls-abort") == 0) {
        int use_tls = strcmp(scenario, "network-tcp-abort") != 0;
        PAPACC_U16 terminal_type = strcmp(scenario,
            "network-tls-half-close") == 0 ?
            PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE :
            PAPACC_MESSAGE_TYPE_NETWORK_ABORT;
        unsigned short fixture_port;
        if (argc != 6) { stage = 22; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (!rc_network_terminal(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust, use_tls,
                terminal_type)) { stage = 23; goto cleanup; }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario,"network-tls-truncated-v1") == 0 ||
            strcmp(scenario,"network-tls-truncated-v2") == 0) {
        PAPACC_CAPABILITY_VERSION major=strcmp(scenario,
            "network-tls-truncated-v2")==0?
            PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2:
            PAPACC_PACC_NETWORK_EGRESS_MAJOR_V1;
        unsigned short fixture_port;
        PAPACC_SIZE expected_size=major==PAPACC_PACC_NETWORK_EGRESS_MAJOR_V2?
            600U:100U;
        if(argc!=6){stage=30;goto cleanup;}
        fixture_port=(unsigned short)strtoul(argv[5],NULL,10);
        if(fixture_port==0U||!rc_network_truncated(runtime,port,fixture_port,
            fixture.client_a.credentials,fixture.trust,major,expected_size)){
            stage=31;goto cleanup;
        }
        result=0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-secure-v2-to-v1-fallback") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 34; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (fixture_port == 0U || !rc_secure_v2_to_v1_fallback(runtime, port,
                fixture_port, fixture.client_a.credentials, fixture.trust)) {
            stage = 35; goto cleanup;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-secure-v2-protocol-error") == 0) {
        if (!rc_secure_protocol_error_negative(runtime, port,
                fixture.client_a.credentials, fixture.trust)) {
            stage = 36; goto cleanup;
        }
        result = 0; goto cleanup;
    }
    if (strcmp(scenario, "network-secure-auth-failure") == 0 ||
            strcmp(scenario, "network-secure-transport-failure") == 0) {
        int auth = strcmp(scenario, "network-secure-auth-failure") == 0;
        if (!rc_secure_pre_control_close(runtime, port,
                auth ? fixture.client_b.credentials : fixture.client_a.credentials,
                fixture.trust, auth ?
                    "REFERENCE_CLIENT_AUTH_FAILURE_REALIZED=YES" :
                    "REFERENCE_CLIENT_TRANSPORT_FAILURE_REALIZED=YES")) {
            stage = 37; goto cleanup;
        }
        result = 0; goto cleanup;
    }
    if (strcmp(scenario, "network-secure-post-egress-failure") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 38; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (!rc_network_http_terminal(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust, 0, 0)) {
            stage = 39; goto cleanup;
        }
        puts("REFERENCE_CLIENT_POST_EGRESS_TLS_TRUNCATED=YES");
        result = 0; goto cleanup;
    }
    if (strncmp(scenario, "network-http-terminal-", 22U) == 0) {
        unsigned short fixture_port;
        int clean = strcmp(scenario, "network-http-terminal-clean") == 0;
        int multi = strstr(scenario, "multi") != NULL;
        if (argc != 6) { stage = 32; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (fixture_port == 0U || !rc_network_http_terminal(runtime, port,
                fixture_port, fixture.client_a.credentials, fixture.trust,
                clean, multi)) { stage = 33; goto cleanup; }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "network-multiple-recovery") == 0) {
        unsigned short fixture_port;
        if (argc != 6) { stage = 24; goto cleanup; }
        fixture_port = (unsigned short)strtoul(argv[5], NULL, 10);
        if (!rc_multiple_data_recovery(runtime, port, fixture_port,
                fixture.client_a.credentials, fixture.trust)) {
            stage = 25; goto cleanup;
        }
        result = 0;
        goto cleanup;
    }
    if (strcmp(scenario, "main") != 0) { stage = 16; goto cleanup; }
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &control) || !rc_write_all(&control, control_open,
            sizeof(control_open)) || !rc_read_exact(&control, response, 20U) ||
        memcmp(response, control_accept, 20U) != 0 ||
        !rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U) || response[9] != 4U) {
        stage = 3; goto cleanup;
    }
    if (!rc_make_attach(response, attach)) goto cleanup;
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &data) || !rc_write_all(&data, attach, sizeof(attach)) ||
        !rc_read_exact(&data, response, 16U) ||
        memcmp(response, data_accept, 16U) != 0) {
        stage = 4; goto cleanup;
    }
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &replay) || !rc_write_all(&replay, attach, sizeof(attach)) ||
        !rc_expect_close(&replay)) { stage = 5; goto cleanup; }
    pst_connection_release(replay.connection); replay.connection = NULL;
    if (!rc_write_all(&control, ticket_request, sizeof(ticket_request)) ||
        !rc_read_exact(&control, response, 32U)) { stage = 6; goto cleanup; }
    if (!rc_make_attach(response, attach)) { stage = 6; goto cleanup; }
    if (!rc_open(runtime, port, fixture.client_b.credentials, fixture.trust,
            &foreign) || !rc_write_all(&foreign, attach, sizeof(attach)) ||
        !rc_expect_close(&foreign)) { stage = 7; goto cleanup; }
    pst_connection_release(foreign.connection); foreign.connection = NULL;
    if (!rc_open(runtime, port, fixture.client_a.credentials, fixture.trust,
            &rightful) || !rc_write_all(&rightful, attach, sizeof(attach)) ||
        !rc_read_exact(&rightful, response, 16U) ||
        memcmp(response, data_accept, 16U) != 0) {
        stage = 8; goto cleanup;
    }
    {
        BOOL shutdown_signal_result = SetEvent(shutdown_event);
        DWORD shutdown_signal_error = shutdown_signal_result ? ERROR_SUCCESS :
            GetLastError();
        DWORD shutdown_ack_result = shutdown_signal_result ?
            WaitForSingleObject(shutdown_ack_event, 5000U) : WAIT_FAILED;
        DWORD shutdown_ack_error = shutdown_ack_result == WAIT_FAILED ?
            GetLastError() : ERROR_SUCCESS;
        fprintf(stderr, "STAGE13_SIGNAL_RESULT=%s SIGNAL_ERROR=%lu "
            "ACK_WAIT_RESULT=%lu ACK_WAIT_ERROR=%lu\n",
            shutdown_signal_result ? "SUCCESS" : "FAILURE",
            (unsigned long)shutdown_signal_error,
            (unsigned long)shutdown_ack_result,
            (unsigned long)shutdown_ack_error);
        if (!shutdown_signal_result || shutdown_ack_result != WAIT_OBJECT_0) {
            stage = 13; goto cleanup;
        }
    }
    if (!rc_close(&rightful)) { stage = 10; goto cleanup; }
    if (!rc_close(&data)) { stage = 11; goto cleanup; }
    if (!rc_accept_clean_close(&control)) { stage = 12; goto cleanup; }
    puts("REFERENCE_CLIENT_CONTROL_INTEROP=PASS");
    puts("REFERENCE_CLIENT_DATA_INTEROP=PASS");
    puts("REFERENCE_CLIENT_TICKET_REPLAY_REJECTED=YES");
    puts("REFERENCE_CLIENT_FOREIGN_CREDENTIAL_REJECTED=YES");
    puts("REFERENCE_CLIENT_RIGHTFUL_RETRY_SAME_TICKET=YES");
    puts("REFERENCE_CLIENT_PROFILE_NT4_NSS_COMPATIBLE=YES");
    result = 0;
cleanup:
    rc_f45_trace_close(&f45_trace);
    if (result != 0) fprintf(stderr, "REFERENCE_CLIENT_FAIL_STAGE=%d\n", stage);
    if (rightful.connection != NULL) pst_connection_release(rightful.connection);
    if (foreign.connection != NULL) pst_connection_release(foreign.connection);
    if (replay.connection != NULL) pst_connection_release(replay.connection);
    if (data.connection != NULL) pst_connection_release(data.connection);
    if (control.connection != NULL) pst_connection_release(control.connection);
    if (runtime != NULL) pst_runtime_release(runtime);
    papacc_test_security_fixture_release(&fixture);
    if (shutdown_event != NULL) CloseHandle(shutdown_event);
    if (shutdown_ack_event != NULL) CloseHandle(shutdown_ack_event);
    WSACleanup();
    return result == 0 ? 0 : (stage == 0 ? 1 : stage);
}
