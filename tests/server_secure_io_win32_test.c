#include "server_secure_io_win32.h"

#define CHECK(condition, code) do { if (!(condition)) return (code); } while (0)

int main(void)
{
    PAPACC_SERVER_SECURE_IO_WIN32 secure_io =
        PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
    PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidate =
        PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER;
    PAPACC_SERVER_IO_LOOP_WIN32 protocol_loop =
        PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 slot =
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    PAPACC_CONNECTION_MANAGER connections =
        PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION connection = PAPACC_CONNECTION_INITIALIZER;

    secure_io.initialized = PAPACC_TRUE;
    secure_io.stop_requested = PAPACC_TRUE;
    secure_io.candidates = &candidate;
    secure_io.candidate_capacity = 1U;
    secure_io.protocol_loop = &protocol_loop;
    protocol_loop.initialized = PAPACC_TRUE;
    protocol_loop.processor_slots = &slot;
    protocol_loop.processor_capacity = 1U;
    protocol_loop.connection_manager = &connections;
    connections.storage = &connection;
    connections.capacity = 1U;
    connections.initialized = PAPACC_TRUE;

    candidate.state = PAPACC_SERVER_SECURE_CANDIDATE_SHUTTING_DOWN;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 1);

    candidate.state = PAPACC_SERVER_SECURE_CANDIDATE_CLOSED;
    candidate.connection_instance_id = 7U;
    slot.in_use = PAPACC_TRUE;
    slot.connection_instance_id = 7U;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 2);

    slot = (PAPACC_SERVER_PROTOCOL_SLOT_WIN32)
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32_INITIALIZER;
    connections.count = 1U;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 3);

    connections.count = 0U;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_TRUE, 4);
    secure_io.stop_requested = PAPACC_FALSE;
    CHECK(papacc_server_secure_io_win32_is_stopped(&secure_io) ==
        PAPACC_FALSE, 5);
    return 0;
}
