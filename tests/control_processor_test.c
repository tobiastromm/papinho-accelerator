#include "control_processor.h"

#include <string.h>

typedef struct PAPACC_TEST_IO {
    PAPACC_U8 input[64];
    PAPACC_SIZE input_length;
    PAPACC_SIZE input_offset;
    PAPACC_SIZE read_limit;
    PAPACC_U8 output[64];
    PAPACC_SIZE output_length;
    PAPACC_SIZE write_limit;
    PAPACC_BOOL write_would_block;
    PAPACC_BOOL write_error;
    PAPACC_BOOL write_eof;
    PAPACC_U32 close_count;
} PAPACC_TEST_IO;

typedef struct PAPACC_TEST_FIXTURE {
    PAPACC_CONNECTION_MANAGER connections;
    PAPACC_SESSION_MANAGER sessions;
    PAPACC_CHANNEL_MANAGER channels;
    PAPACC_CONNECTION connection_storage[2];
    PAPACC_SESSION session_storage[2];
    PAPACC_CHANNEL channel_storage[2];
    PAPACC_TEST_IO io[2];
} PAPACC_TEST_FIXTURE;

static PAPACC_RESULT papacc_test_read(
    void *opaque, PAPACC_U8 *buffer, PAPACC_SIZE capacity,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TEST_IO *io = (PAPACC_TEST_IO *)opaque;
    PAPACC_SIZE amount;
    if (io->input_offset == io->input_length) {
        *out_transferred = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    amount = io->input_length - io->input_offset;
    if (amount > capacity) amount = capacity;
    if (amount > io->read_limit) amount = io->read_limit;
    memcpy(buffer, &io->input[io->input_offset], amount);
    io->input_offset += amount;
    *out_transferred = amount;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_test_write(
    void *opaque, const PAPACC_U8 *buffer, PAPACC_SIZE length,
    PAPACC_SIZE *out_transferred, PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    PAPACC_TEST_IO *io = (PAPACC_TEST_IO *)opaque;
    PAPACC_SIZE amount = length;
    if (io->write_error) return PAPACC_RESULT_INTERNAL_ERROR;
    if (io->write_eof) {
        *out_transferred = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK;
    }
    if (io->write_would_block) {
        io->write_would_block = PAPACC_FALSE;
        *out_transferred = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK;
    }
    if (amount > io->write_limit) amount = io->write_limit;
    memcpy(&io->output[io->output_length], buffer, amount);
    io->output_length += amount;
    *out_transferred = amount;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS;
    return PAPACC_RESULT_OK;
}
static void papacc_test_close(void *opaque)
{ ++((PAPACC_TEST_IO *)opaque)->close_count; }

static PAPACC_RESULT papacc_test_init(PAPACC_TEST_FIXTURE *fixture)
{
    memset(fixture, 0, sizeof(*fixture));
    fixture->connections = (PAPACC_CONNECTION_MANAGER)
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    fixture->sessions = (PAPACC_SESSION_MANAGER)PAPACC_SESSION_MANAGER_INITIALIZER;
    fixture->channels = (PAPACC_CHANNEL_MANAGER)PAPACC_CHANNEL_MANAGER_INITIALIZER;
    return papacc_connection_manager_init(
        &fixture->connections, fixture->connection_storage, 2) == PAPACC_RESULT_OK &&
        papacc_session_manager_init(
        &fixture->sessions, fixture->session_storage, 2) == PAPACC_RESULT_OK &&
        papacc_channel_manager_init(&fixture->channels, fixture->channel_storage,
        2, &fixture->connections, &fixture->sessions) == PAPACC_RESULT_OK
        ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_CONNECTION *papacc_test_publish(
    PAPACC_TEST_FIXTURE *fixture, PAPACC_SIZE index)
{
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT local = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT remote = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL;
    PAPACC_TEST_IO *io = &fixture->io[index];
    io->read_limit = sizeof(io->input);
    io->write_limit = sizeof(io->output);
    (void)papacc_ip_address_set_ipv4(&local.address, 127,0,0,1);
    (void)papacc_ip_address_set_ipv4(&remote.address, 127,0,0,1);
    local.port = (PAPACC_U16)(4000 + index);
    remote.port = (PAPACC_U16)(5000 + index);
    transport.context = io;
    transport.read_fn = papacc_test_read;
    transport.write_fn = papacc_test_write;
    transport.close_fn = papacc_test_close;
    if (papacc_connection_manager_publish(
        &fixture->connections, &transport, &local, &remote, &connection) !=
        PAPACC_RESULT_OK) return NULL;
    return connection;
}

static void papacc_test_open(PAPACC_TEST_IO *io, PAPACC_U16 type,
                             PAPACC_U32 length, PAPACC_U16 major,
                             PAPACC_U16 minor)
{
    PAPACC_FRAME_HEADER header = papacc_control_open_frame_header();
    PAPACC_SIZE written;
    header.message_type = type;
    header.payload_length = length;
    (void)papacc_frame_header_encode(&header, io->input, 16, &written);
    io->input[16] = (PAPACC_U8)(major >> 8);
    io->input[17] = (PAPACC_U8)major;
    io->input[18] = (PAPACC_U8)(minor >> 8);
    io->input[19] = (PAPACC_U8)minor;
    io->input_length = 16 + length;
}

static int papacc_test_success(void)
{
    static const PAPACC_U8 accept[20] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,2,0,0,0,0,0,4,0,1,0,0 };
    PAPACC_TEST_FIXTURE f;
    PAPACC_CONNECTION *connection;
    PAPACC_CONTROL_PROCESSOR p = PAPACC_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_U8 scratch[5];
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS status;
    PAPACC_SESSION *session;
    if (papacc_test_init(&f) != PAPACC_RESULT_OK ||
        (connection = papacc_test_publish(&f, 0)) == NULL) return 1;
    papacc_test_open(&f.io[0], PAPACC_MESSAGE_TYPE_CONTROL_OPEN, 4, 1, 0);
    f.io[0].read_limit = 1;
    f.io[0].write_limit = 3;
    if (papacc_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, connection->connection_instance_id, scratch, sizeof(scratch),
        4, 100) != PAPACC_RESULT_OK ||
        !papacc_control_processor_wants_read(&p) ||
        papacc_control_processor_wants_write(&p)) return 2;
    while (p.state != PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT) {
        if (papacc_control_processor_read_once(&p, &status) != PAPACC_RESULT_OK)
            return 3;
        if (f.sessions.count != 0 && f.io[0].input_offset < 20) return 4;
    }
    session = papacc_session_manager_find(&f.sessions, p.session_instance_id);
    if (f.sessions.count != 1 || f.channels.count != 1 ||
        connection->state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        session == NULL || session->state != PAPACC_SESSION_STATE_ESTABLISHING ||
        papacc_control_processor_wants_read(&p) ||
        !papacc_control_processor_wants_write(&p)) return 5;
    f.io[0].write_would_block = PAPACC_TRUE;
    if (papacc_control_processor_write_once(&p, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_CONTROL_PROCESSOR_STEP_STATUS_WOULD_BLOCK ||
        session->state != PAPACC_SESSION_STATE_ESTABLISHING) return 6;
    while (p.state == PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT) {
        if (papacc_control_processor_write_once(&p, &status) != PAPACC_RESULT_OK ||
            (p.state != PAPACC_CONTROL_PROCESSOR_STATE_ESTABLISHED &&
             session->state != PAPACC_SESSION_STATE_ESTABLISHING)) return 7;
    }
    if (status != PAPACC_CONTROL_PROCESSOR_STEP_STATUS_ESTABLISHED ||
        session->state != PAPACC_SESSION_STATE_ACTIVE ||
        memcmp(f.io[0].output, accept, 20) != 0 ||
        papacc_control_processor_read_once(&p, &status) !=
            PAPACC_RESULT_INVALID_STATE) return 8;
    papacc_control_processor_shutdown(&p);
    if (session->state != PAPACC_SESSION_STATE_ACTIVE ||
        connection->state != PAPACC_CONNECTION_STATE_ASSOCIATED ||
        f.channels.storage[0].state != PAPACC_CHANNEL_STATE_BOUND) return 9;
    papacc_channel_manager_shutdown(&f.channels);
    papacc_session_manager_shutdown(&f.sessions);
    papacc_connection_manager_shutdown(&f.connections);
    return 0;
}

static int papacc_test_wrong_and_deadline(void)
{
    PAPACC_TEST_FIXTURE f;
    PAPACC_CONNECTION *a;
    PAPACC_CONNECTION *b;
    PAPACC_CONTROL_PROCESSOR pa = PAPACC_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_CONTROL_PROCESSOR pb = PAPACC_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_U8 sa[8], sb[8];
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS status;
    if (papacc_test_init(&f) != PAPACC_RESULT_OK ||
        (a = papacc_test_publish(&f, 0)) == NULL ||
        (b = papacc_test_publish(&f, 1)) == NULL) return 20;
    papacc_test_open(&f.io[0], 0xBEEF, 4, 1, 0);
    papacc_test_open(&f.io[1], PAPACC_MESSAGE_TYPE_CONTROL_OPEN, 4, 1, 0);
    if (papacc_control_processor_init(&pa, &f.connections, &f.sessions,
        &f.channels, a->connection_instance_id, sa, 8, 4, 10) != PAPACC_RESULT_OK ||
        papacc_control_processor_init(&pb, &f.connections, &f.sessions,
        &f.channels, b->connection_instance_id, sb, 8, 4, 10) != PAPACC_RESULT_OK)
        return 21;
    while (pa.state != PAPACC_CONTROL_PROCESSOR_STATE_ERROR) {
        PAPACC_RESULT r = papacc_control_processor_read_once(&pa, &status);
        if (r != PAPACC_RESULT_OK && r != PAPACC_RESULT_PROTOCOL_ERROR) return 22;
    }
    if (a->state != PAPACC_CONNECTION_STATE_CLOSED || f.sessions.count != 0 ||
        papacc_control_processor_check_deadline(&pb, 9, &status) !=
            PAPACC_RESULT_OK || b->state != PAPACC_CONNECTION_STATE_PENDING ||
        papacc_control_processor_check_deadline(&pb, 10, &status) !=
            PAPACC_RESULT_OK || status != PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED ||
        b->state != PAPACC_CONNECTION_STATE_CLOSED) return 23;
    papacc_control_processor_shutdown(&pa);
    papacc_control_processor_shutdown(&pb);
    papacc_channel_manager_shutdown(&f.channels);
    papacc_session_manager_shutdown(&f.sessions);
    papacc_connection_manager_shutdown(&f.connections);
    return 0;
}

static int papacc_test_capacity_rollback(void)
{
    PAPACC_TEST_FIXTURE f;
    PAPACC_CONNECTION *c;
    PAPACC_CONTROL_PROCESSOR p = PAPACC_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_U8 scratch[20];
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS status;
    if (papacc_test_init(&f) != PAPACC_RESULT_OK ||
        (c = papacc_test_publish(&f, 0)) == NULL) return 30;
    f.channels.capacity = 0;
    papacc_test_open(&f.io[0], PAPACC_MESSAGE_TYPE_CONTROL_OPEN, 4, 1, 0);
    if (papacc_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, c->connection_instance_id, scratch, 20, 4, 100) !=
        PAPACC_RESULT_OK) return 31;
    while (p.state != PAPACC_CONTROL_PROCESSOR_STATE_ERROR) {
        PAPACC_RESULT r = papacc_control_processor_read_once(&p, &status);
        if (r != PAPACC_RESULT_OK && r != PAPACC_RESULT_LIMIT_EXCEEDED) return 32;
    }
    if (f.sessions.count != 0 || f.channels.count != 0 ||
        c->state != PAPACC_CONNECTION_STATE_CLOSED) return 33;
    papacc_control_processor_shutdown(&p);
    f.channels.capacity = 2;
    papacc_channel_manager_shutdown(&f.channels);
    papacc_session_manager_shutdown(&f.sessions);
    papacc_connection_manager_shutdown(&f.connections);
    return 0;
}

static int papacc_test_eof_and_writer_failures(void)
{
    PAPACC_TEST_FIXTURE f;
    PAPACC_CONNECTION *c;
    PAPACC_CONTROL_PROCESSOR p = PAPACC_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_U8 scratch[20];
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS status;
    PAPACC_SESSION *session;
    if (papacc_test_init(&f) != PAPACC_RESULT_OK ||
        (c = papacc_test_publish(&f, 0)) == NULL ||
        papacc_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, c->connection_instance_id, scratch, 20, 4, 100) !=
        PAPACC_RESULT_OK ||
        papacc_control_processor_read_once(&p, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED ||
        c->state != PAPACC_CONNECTION_STATE_CLOSED || f.sessions.count != 0)
        return 40;
    papacc_control_processor_shutdown(&p);
    papacc_channel_manager_shutdown(&f.channels);
    papacc_session_manager_shutdown(&f.sessions);
    papacc_connection_manager_shutdown(&f.connections);

    if (papacc_test_init(&f) != PAPACC_RESULT_OK ||
        (c = papacc_test_publish(&f, 0)) == NULL) return 41;
    papacc_test_open(&f.io[0], PAPACC_MESSAGE_TYPE_CONTROL_OPEN, 4, 1, 0);
    if (papacc_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, c->connection_instance_id, scratch, 20, 4, 100) !=
        PAPACC_RESULT_OK) return 42;
    while (p.state != PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT)
        if (papacc_control_processor_read_once(&p, &status) != PAPACC_RESULT_OK)
            return 43;
    session = papacc_session_manager_find(&f.sessions, p.session_instance_id);
    f.io[0].write_eof = PAPACC_TRUE;
    if (papacc_control_processor_write_once(&p, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED ||
        session->state != PAPACC_SESSION_STATE_CLOSED ||
        c->state != PAPACC_CONNECTION_STATE_CLOSED) return 44;
    papacc_control_processor_shutdown(&p);
    papacc_channel_manager_shutdown(&f.channels);
    papacc_session_manager_shutdown(&f.sessions);
    papacc_connection_manager_shutdown(&f.connections);

    if (papacc_test_init(&f) != PAPACC_RESULT_OK ||
        (c = papacc_test_publish(&f, 0)) == NULL) return 45;
    papacc_test_open(&f.io[0], PAPACC_MESSAGE_TYPE_CONTROL_OPEN, 4, 1, 0);
    if (papacc_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, c->connection_instance_id, scratch, 20, 4, 100) !=
        PAPACC_RESULT_OK) return 46;
    while (p.state != PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT)
        if (papacc_control_processor_read_once(&p, &status) != PAPACC_RESULT_OK)
            return 47;
    session = papacc_session_manager_find(&f.sessions, p.session_instance_id);
    f.io[0].write_error = PAPACC_TRUE;
    if (papacc_control_processor_write_once(&p, &status) !=
            PAPACC_RESULT_INTERNAL_ERROR ||
        p.state != PAPACC_CONTROL_PROCESSOR_STATE_ERROR ||
        session->state != PAPACC_SESSION_STATE_CLOSED ||
        c->state != PAPACC_CONNECTION_STATE_CLOSED) return 48;
    papacc_control_processor_shutdown(&p);
    papacc_channel_manager_shutdown(&f.channels);
    papacc_session_manager_shutdown(&f.sessions);
    papacc_connection_manager_shutdown(&f.connections);
    return 0;
}

static int papacc_test_session_capacity(void)
{
    PAPACC_TEST_FIXTURE f;
    PAPACC_CONNECTION *c;
    PAPACC_SESSION *s1, *s2;
    PAPACC_CONTROL_PROCESSOR p = PAPACC_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_U8 scratch[20];
    PAPACC_CONTROL_PROCESSOR_STEP_STATUS status;
    if (papacc_test_init(&f) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&f.sessions, &s1) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&f.sessions, &s2) != PAPACC_RESULT_OK ||
        (c = papacc_test_publish(&f, 0)) == NULL) return 50;
    papacc_test_open(&f.io[0], PAPACC_MESSAGE_TYPE_CONTROL_OPEN, 4, 1, 0);
    if (papacc_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, c->connection_instance_id, scratch, 20, 4, 100) !=
        PAPACC_RESULT_OK) return 51;
    while (p.state != PAPACC_CONTROL_PROCESSOR_STATE_ERROR) {
        PAPACC_RESULT r = papacc_control_processor_read_once(&p, &status);
        if (r != PAPACC_RESULT_OK && r != PAPACC_RESULT_LIMIT_EXCEEDED) return 52;
    }
    if (f.sessions.count != 2 || f.channels.count != 0 ||
        c->state != PAPACC_CONNECTION_STATE_CLOSED) return 53;
    papacc_control_processor_shutdown(&p);
    papacc_channel_manager_shutdown(&f.channels);
    papacc_session_manager_shutdown(&f.sessions);
    papacc_connection_manager_shutdown(&f.connections);
    return 0;
}

int main(void)
{
    int r = papacc_test_success();
    if (r == 0) r = papacc_test_wrong_and_deadline();
    if (r == 0) r = papacc_test_capacity_rollback();
    if (r == 0) r = papacc_test_eof_and_writer_failures();
    if (r == 0) r = papacc_test_session_capacity();
    return r;
}
