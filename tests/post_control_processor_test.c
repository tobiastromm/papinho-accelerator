#include "post_control_processor.h"

#include <string.h>

typedef struct TEST_IO {
    PAPACC_U8 input[128]; PAPACC_SIZE input_length; PAPACC_SIZE input_offset;
    PAPACC_SIZE read_limit; PAPACC_BOOL read_would_block; PAPACC_BOOL read_error;
    PAPACC_U8 output[128]; PAPACC_SIZE output_length; PAPACC_SIZE write_limit;
    PAPACC_BOOL write_would_block; PAPACC_BOOL write_error; PAPACC_BOOL write_eof;
    PAPACC_U32 close_count;
} TEST_IO;

typedef struct TEST_GENERATOR {
    PAPACC_U32 calls; PAPACC_BOOL fail; PAPACC_U8 seed;
} TEST_GENERATOR;

typedef struct TEST_FIXTURE {
    PAPACC_CONNECTION_MANAGER connections;
    PAPACC_SESSION_MANAGER sessions;
    PAPACC_CHANNEL_MANAGER channels;
    PAPACC_DATA_ASSOCIATION_MANAGER associations;
    PAPACC_CONNECTION connection_storage[2]; PAPACC_SESSION session_storage[2];
    PAPACC_CHANNEL channel_storage[2]; PAPACC_DATA_ASSOCIATION_ENTRY entries[2];
    TEST_IO io[2]; TEST_GENERATOR generator;
} TEST_FIXTURE;

static PAPACC_RESULT test_read(void *context, PAPACC_U8 *buffer,
    PAPACC_SIZE capacity, PAPACC_SIZE *out_count,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    TEST_IO *io = (TEST_IO *)context; PAPACC_SIZE count;
    if (io->read_error) return PAPACC_RESULT_INTERNAL_ERROR;
    if (io->read_would_block) { io->read_would_block = PAPACC_FALSE;
        *out_count = 0; *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK; }
    if (io->input_offset == io->input_length) { *out_count = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK; }
    count = io->input_length - io->input_offset;
    if (count > capacity) count = capacity;
    if (count > io->read_limit) count = io->read_limit;
    memcpy(buffer, &io->input[io->input_offset], count);
    io->input_offset += count; *out_count = count;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS; return PAPACC_RESULT_OK;
}

static PAPACC_RESULT test_write(void *context, const PAPACC_U8 *buffer,
    PAPACC_SIZE length, PAPACC_SIZE *out_count,
    PAPACC_TRANSPORT_IO_STATUS *out_status)
{
    TEST_IO *io = (TEST_IO *)context; PAPACC_SIZE count = length;
    if (io->write_error) return PAPACC_RESULT_INTERNAL_ERROR;
    if (io->write_eof) { *out_count = 0;
        *out_status = PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;
        return PAPACC_RESULT_OK; }
    if (io->write_would_block) { io->write_would_block = PAPACC_FALSE;
        *out_count = 0; *out_status = PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;
        return PAPACC_RESULT_OK; }
    if (count > io->write_limit) count = io->write_limit;
    memcpy(&io->output[io->output_length], buffer, count);
    io->output_length += count; *out_count = count;
    *out_status = PAPACC_TRANSPORT_IO_STATUS_PROGRESS; return PAPACC_RESULT_OK;
}
static void test_close(void *context) { ++((TEST_IO *)context)->close_count; }

static PAPACC_RESULT test_generate(void *context,
    PAPACC_DATA_ASSOCIATION_TICKET *out_ticket)
{
    TEST_GENERATOR *g = (TEST_GENERATOR *)context; PAPACC_SIZE i;
    ++g->calls; if (g->fail) return PAPACC_RESULT_INTERNAL_ERROR;
    for (i = 0; i < 16; ++i) out_ticket->bytes[i] = (PAPACC_U8)(g->seed + i);
    ++g->seed; return PAPACC_RESULT_OK;
}

static int fixture_init(TEST_FIXTURE *f, PAPACC_SIZE association_capacity)
{
    memset(f, 0, sizeof(*f)); f->connections = (PAPACC_CONNECTION_MANAGER)
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    f->sessions = (PAPACC_SESSION_MANAGER)PAPACC_SESSION_MANAGER_INITIALIZER;
    f->channels = (PAPACC_CHANNEL_MANAGER)PAPACC_CHANNEL_MANAGER_INITIALIZER;
    f->associations = (PAPACC_DATA_ASSOCIATION_MANAGER)
        PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER;
    f->generator.seed = 0;
    return papacc_connection_manager_init(&f->connections,
        f->connection_storage, 2) == PAPACC_RESULT_OK &&
        papacc_session_manager_init(&f->sessions, f->session_storage, 2) ==
            PAPACC_RESULT_OK &&
        papacc_channel_manager_init(&f->channels, f->channel_storage, 2,
            &f->connections, &f->sessions) == PAPACC_RESULT_OK &&
        papacc_data_association_manager_init(&f->associations, f->entries,
            association_capacity, &f->sessions, &f->channels, test_generate,
            &f->generator, 100) == PAPACC_RESULT_OK ? 0 : 1;
}

static int fixture_publish(TEST_FIXTURE *f, PAPACC_SIZE index,
    PAPACC_SESSION **out_session, PAPACC_CHANNEL **out_channel)
{
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_CONNECTION *connection = NULL; TEST_IO *io = &f->io[index];
    io->read_limit = sizeof(io->input); io->write_limit = sizeof(io->output);
    (void)papacc_ip_address_set_ipv4(&endpoint.address, 127, 0, 0, 1);
    endpoint.port = (PAPACC_U16)(4000 + index);
    transport.context = io; transport.read_fn = test_read;
    transport.write_fn = test_write; transport.close_fn = test_close;
    if (papacc_connection_manager_publish(&f->connections, &transport,
            &endpoint, &endpoint, &connection) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&f->sessions, out_session) !=
            PAPACC_RESULT_OK || papacc_channel_manager_bind(&f->channels,
            (*out_session)->session_instance_id,
            connection->connection_instance_id, PAPACC_CHANNEL_ROLE_CONTROL,
            out_channel) != PAPACC_RESULT_OK ||
        papacc_session_activate(*out_session) != PAPACC_RESULT_OK) return 1;
    return 0;
}

static void set_request(TEST_IO *io, PAPACC_U16 type, PAPACC_U32 length)
{
    PAPACC_FRAME_HEADER header = papacc_data_ticket_request_frame_header();
    PAPACC_SIZE written = 0; header.message_type = type;
    header.payload_length = length;
    (void)papacc_frame_header_encode(&header, io->input, 16, &written);
    memset(&io->input[16], 0xA5, length); io->input_length = 16 + length;
}

static int run_until_write(PAPACC_POST_CONTROL_PROCESSOR *p, PAPACC_U64 now)
{
    PAPACC_POST_CONTROL_STEP_STATUS status;
    while (p->state == PAPACC_POST_CONTROL_PROCESSOR_STATE_READY) {
        PAPACC_RESULT r = papacc_post_control_processor_read_once(p, now, &status);
        if (r != PAPACC_RESULT_OK) return 50 + (int)r;
    }
    return p->state == PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET
        ? 0 : 70 + (int)p->state;
}
static int finish_write(PAPACC_POST_CONTROL_PROCESSOR *p)
{
    PAPACC_POST_CONTROL_STEP_STATUS status = PAPACC_POST_CONTROL_STEP_STATUS_UNSPECIFIED;
    while (p->state == PAPACC_POST_CONTROL_PROCESSOR_STATE_WRITING_TICKET)
        if (papacc_post_control_processor_write_once(p, &status) !=
            PAPACC_RESULT_OK) return 1;
    return status == PAPACC_POST_CONTROL_STEP_STATUS_RESPONSE_COMPLETE ? 0 : 1;
}

static int test_success_reissue_expiry(void)
{
    static const PAPACC_U8 golden[32] = {0x50,0x41,0x43,0x43,1,0,0,16,
        0,4,0,0,0,0,0,16,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c;
    PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_U8 scratch[3]; PAPACC_U64 deadline; int read_result;
    if (fixture_init(&f, 2) || fixture_publish(&f, 0, &s, &c)) return 1;
    set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0);
    f.io[0].read_limit = 1; f.io[0].write_limit = 2;
    if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, s->session_instance_id,
        c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
        !papacc_post_control_processor_wants_read(&p) ||
        papacc_post_control_processor_wants_write(&p))
        return 2;
    read_result = run_until_write(&p, 10); if (read_result) return read_result;
    if (f.associations.count != 1 || f.generator.calls != 1) return 6;
    if (p.ticket_payload_offset != 0) return 7;
    if (finish_write(&p)) return 8;
    if (memcmp(f.io[0].output, golden, 32) != 0) return 9;
    if (f.associations.count != 1) return 15;
    deadline = f.entries[0].deadline_ns;
    set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0);
    f.io[0].input_offset = 0; f.io[0].output_length = 0;
    if (run_until_write(&p, 20) || finish_write(&p) || f.generator.calls != 1 ||
        f.entries[0].deadline_ns != deadline || memcmp(f.io[0].output, golden, 32))
        return 3;
    set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0);
    f.io[0].input_offset = 0; f.io[0].output_length = 0;
    if (run_until_write(&p, deadline) || finish_write(&p) ||
        f.generator.calls != 2 || f.io[0].output[17] != 2) return 4;
    papacc_post_control_processor_shutdown(&p); return 0;
}

static int test_would_block_and_eof(void)
{
    TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c;
    PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
    PAPACC_POST_CONTROL_STEP_STATUS status; PAPACC_U8 scratch[16];
    if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c)) return 10;
    set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0);
    if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, s->session_instance_id,
        c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
        run_until_write(&p, 0)) return 11;
    f.io[0].write_would_block = PAPACC_TRUE;
    if (papacc_post_control_processor_write_once(&p, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_POST_CONTROL_STEP_STATUS_WOULD_BLOCK ||
        p.ticket_payload_offset != 0) return 12;
    f.io[0].write_eof = PAPACC_TRUE;
    if (papacc_post_control_processor_write_once(&p, &status) != PAPACC_RESULT_OK ||
        status != PAPACC_POST_CONTROL_STEP_STATUS_CLOSED ||
        p.state != PAPACC_POST_CONTROL_PROCESSOR_STATE_CLOSED ||
        f.associations.count != 0 || s->state != PAPACC_SESSION_STATE_CLOSED)
        return 13;
    return 0;
}

static int test_invalid_input_and_failures(void)
{
    const PAPACC_U16 types[] = {1,2,4,5,6,0xBEEF}; PAPACC_SIZE i;
    for (i = 0; i < sizeof(types)/sizeof(types[0]); ++i) {
        TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c; PAPACC_U8 scratch[16];
        PAPACC_POST_CONTROL_STEP_STATUS status;
        PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
        if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c)) return 20;
        set_request(&f.io[0], types[i], 0);
        if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
            &f.channels, &f.associations, s->session_instance_id,
            c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
            papacc_post_control_processor_read_once(&p, 0, &status) !=
                PAPACC_RESULT_PROTOCOL_ERROR ||
            p.state != PAPACC_POST_CONTROL_PROCESSOR_STATE_ERROR ||
            s->state != PAPACC_SESSION_STATE_CLOSED) return 21;
    }
    return 0;
}

static int test_capacity_generator_shutdown(void)
{
    TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c; PAPACC_U8 scratch[16];
    PAPACC_POST_CONTROL_STEP_STATUS status;
    PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
    if (fixture_init(&f, 0) || fixture_publish(&f, 0, &s, &c)) return 30;
    set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0);
    if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, s->session_instance_id,
        c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
        papacc_post_control_processor_read_once(&p, 0, &status) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || s->state != PAPACC_SESSION_STATE_CLOSED)
        return 31;
    if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c)) return 32;
    f.generator.fail = PAPACC_TRUE;
    set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0);
    p = (PAPACC_POST_CONTROL_PROCESSOR)PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
    if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, s->session_instance_id,
        c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
        papacc_post_control_processor_read_once(&p, 0, &status) !=
            PAPACC_RESULT_INTERNAL_ERROR || f.associations.count != 0) return 33;
    papacc_post_control_processor_shutdown(&p);
    if (p.state != PAPACC_POST_CONTROL_PROCESSOR_STATE_UNINITIALIZED ||
        f.connections.initialized != PAPACC_TRUE ||
        f.sessions.initialized != PAPACC_TRUE || f.channels.initialized != PAPACC_TRUE ||
        f.associations.initialized != PAPACC_TRUE) return 34;
    return 0;
}

static int test_invalid_init(void)
{
    TEST_FIXTURE f, other; PAPACC_SESSION *s; PAPACC_CHANNEL *c; PAPACC_U8 scratch[4];
    PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
    if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c) ||
        fixture_init(&other, 1)) return 40;
    if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, 999, c->channel_instance_id, scratch, 4) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &other.associations, s->session_instance_id,
        c->channel_instance_id, scratch, 4) != PAPACC_RESULT_INVALID_STATE ||
        papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, s->session_instance_id,
        c->channel_instance_id, scratch, 0) != PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, s->session_instance_id,
        c->channel_instance_id, scratch, 4) != PAPACC_RESULT_OK ||
        papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
        &f.channels, &f.associations, s->session_instance_id,
        c->channel_instance_id, scratch, 4) != PAPACC_RESULT_INVALID_STATE)
        return 41;
    papacc_post_control_processor_shutdown(&p); return 0;
}

static int test_lengths_and_io_lifecycle(void)
{
    PAPACC_U32 lengths[2] = { 1, 16 }; PAPACC_SIZE i;
    for (i = 0; i < 2; ++i) {
        TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c; PAPACC_U8 scratch[32];
        PAPACC_POST_CONTROL_STEP_STATUS status;
        PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
        if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c)) return 50;
        set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, lengths[i]);
        if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
            &f.channels, &f.associations, s->session_instance_id,
            c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
            papacc_post_control_processor_read_once(&p, 0, &status) !=
                PAPACC_RESULT_PROTOCOL_ERROR || f.associations.count != 0 ||
            s->state != PAPACC_SESSION_STATE_CLOSED) return 51;
    }
    {
        TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c; PAPACC_U8 scratch[16];
        PAPACC_POST_CONTROL_STEP_STATUS status;
        PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
        if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c)) return 52;
        f.io[0].read_error = PAPACC_TRUE;
        if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
            &f.channels, &f.associations, s->session_instance_id,
            c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
            papacc_post_control_processor_read_once(&p, 0, &status) !=
                PAPACC_RESULT_INTERNAL_ERROR ||
            p.state != PAPACC_POST_CONTROL_PROCESSOR_STATE_ERROR) return 53;
    }
    {
        TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c; PAPACC_U8 scratch[16];
        PAPACC_POST_CONTROL_STEP_STATUS status;
        PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
        if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c)) return 54;
        if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
            &f.channels, &f.associations, s->session_instance_id,
            c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
            papacc_post_control_processor_read_once(&p, 0, &status) !=
                PAPACC_RESULT_OK || status != PAPACC_POST_CONTROL_STEP_STATUS_CLOSED ||
            p.state != PAPACC_POST_CONTROL_PROCESSOR_STATE_CLOSED) return 55;
    }
    {
        TEST_FIXTURE f; PAPACC_SESSION *s; PAPACC_CHANNEL *c; PAPACC_U8 scratch[16];
        PAPACC_POST_CONTROL_STEP_STATUS status;
        PAPACC_POST_CONTROL_PROCESSOR p = PAPACC_POST_CONTROL_PROCESSOR_INITIALIZER;
        if (fixture_init(&f, 1) || fixture_publish(&f, 0, &s, &c)) return 56;
        set_request(&f.io[0], PAPACC_MESSAGE_TYPE_DATA_TICKET_REQUEST, 0);
        if (papacc_post_control_processor_init(&p, &f.connections, &f.sessions,
            &f.channels, &f.associations, s->session_instance_id,
            c->channel_instance_id, scratch, sizeof(scratch)) != PAPACC_RESULT_OK ||
            run_until_write(&p, 0)) return 57;
        f.io[0].write_error = PAPACC_TRUE;
        if (papacc_post_control_processor_write_once(&p, &status) !=
                PAPACC_RESULT_INTERNAL_ERROR || f.associations.count != 0 ||
            s->state != PAPACC_SESSION_STATE_CLOSED ||
            p.state != PAPACC_POST_CONTROL_PROCESSOR_STATE_ERROR) return 58;
    }
    return 0;
}

int main(void)
{
    int result = test_success_reissue_expiry(); if (result) return result;
    result = test_would_block_and_eof(); if (result) return result;
    result = test_invalid_input_and_failures(); if (result) return result;
    result = test_capacity_generator_shutdown(); if (result) return result;
    result = test_invalid_init(); if (result) return result;
    return test_lengths_and_io_lifecycle();
}
