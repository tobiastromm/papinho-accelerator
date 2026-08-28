#ifndef PAPACC_SERVER_IO_LOOP_WIN32_H
#define PAPACC_SERVER_IO_LOOP_WIN32_H

#include "control_processor.h"
#include "server_acceptor_win32.h"

#define PAPACC_SERVER_CONTROL_READ_BUFFER_SIZE ((PAPACC_SIZE)64U)

typedef struct PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 {
    PAPACC_BOOL in_use;
    PAPACC_U64 connection_instance_id;
    PAPACC_CONTROL_PROCESSOR processor;
    PAPACC_U8 read_scratch[PAPACC_SERVER_CONTROL_READ_BUFFER_SIZE];
} PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32;

#define PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32_INITIALIZER \
    { PAPACC_FALSE, 0, PAPACC_CONTROL_PROCESSOR_INITIALIZER, { 0 } }

typedef struct PAPACC_SERVER_IO_LOOP_WIN32 {
    PAPACC_SERVER_NETWORK *network;
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor;
    PAPACC_CONNECTION_MANAGER *connection_manager;
    PAPACC_SESSION_MANAGER session_manager;
    PAPACC_CHANNEL_MANAGER channel_manager;
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *processor_slots;
    PAPACC_SIZE processor_capacity;
    PAPACC_SIZE next_listener_index;
    PAPACC_SIZE next_processor_index;
    PAPACC_U64 establishment_timeout_ns;
    PAPACC_BOOL initialized;
} PAPACC_SERVER_IO_LOOP_WIN32;

#define PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER \
    { NULL, NULL, NULL, PAPACC_SESSION_MANAGER_INITIALIZER, \
      PAPACC_CHANNEL_MANAGER_INITIALIZER, NULL, 0, 0, 0, 0, PAPACC_FALSE }

PAPACC_RESULT papacc_server_io_loop_win32_init(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    PAPACC_SERVER_NETWORK *network,
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
    PAPACC_SESSION *session_storage,
    PAPACC_SIZE session_capacity,
    PAPACC_CHANNEL *channel_storage,
    PAPACC_SIZE channel_capacity,
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *processor_slots,
    PAPACC_SIZE processor_capacity,
    PAPACC_U64 establishment_timeout_ns);

/* One combined select, at most one accept, and one action per ready processor. */
PAPACC_RESULT papacc_server_io_loop_win32_poll_once(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    PAPACC_U32 timeout_ms);

/* Deterministic Win32 scheduler introspection; performs no I/O. */
PAPACC_RESULT papacc_server_io_loop_win32_processor_interest(
    const PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_SIZE processor_index,
    PAPACC_BOOL *out_read_interest, PAPACC_BOOL *out_write_interest);
PAPACC_RESULT papacc_server_io_loop_win32_processor_scan_index(
    const PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_SIZE scan_offset,
    PAPACC_SIZE *out_processor_index);

void papacc_server_io_loop_win32_shutdown(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop);

#endif
