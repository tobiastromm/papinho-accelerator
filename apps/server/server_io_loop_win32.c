#include "server_io_loop_win32.h"

#include <limits.h>

#include "pal_time.h"

static PAPACC_BOOL papacc_server_io_loop_network_valid(
    const PAPACC_SERVER_NETWORK *network)
{
    return network != NULL && network->is_active == PAPACC_TRUE &&
        network->listener_set.is_active == PAPACC_TRUE &&
        network->listener_set.entries != NULL &&
        network->listener_set.count > 0 &&
        network->listener_set.count <= network->listener_set.capacity
        ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_U64 papacc_server_io_loop_deadline(
    PAPACC_U64 now_ns, PAPACC_U64 timeout_ns)
{
    const PAPACC_U64 maximum = (PAPACC_U64)(~(PAPACC_U64)0);
    return now_ns > maximum - timeout_ns ? maximum : now_ns + timeout_ns;
}

static PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32
    *papacc_server_io_loop_find_free_slot(PAPACC_SERVER_IO_LOOP_WIN32 *loop)
{
    PAPACC_SIZE index;
    for (index = 0; index < loop->processor_capacity; ++index) {
        if (loop->processor_slots[index].in_use == PAPACC_FALSE)
            return &loop->processor_slots[index];
    }
    return NULL;
}

static void papacc_server_io_loop_reclaim(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *slot)
{
    PAPACC_U64 connection_id = slot->connection_instance_id;
    PAPACC_CHANNEL *channel = papacc_channel_manager_find_by_connection(
        &loop->channel_manager, connection_id);
    PAPACC_U64 channel_id = 0;
    PAPACC_U64 session_id = 0;

    if (channel != NULL) {
        channel_id = channel->channel_instance_id;
        session_id = channel->session_instance_id;
    }
    papacc_control_processor_shutdown(&slot->processor);
    if (channel_id != 0)
        (void)papacc_channel_manager_remove(&loop->channel_manager, channel_id);
    if (session_id != 0)
        (void)papacc_session_manager_remove(&loop->session_manager, session_id);
    if (connection_id != 0)
        (void)papacc_connection_manager_remove(
            loop->connection_manager, connection_id);
    *slot = (PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32)
        PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32_INITIALIZER;
}

static PAPACC_RESULT papacc_server_io_loop_attach(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_CONNECTION *connection,
    PAPACC_U64 now_ns)
{
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *slot =
        papacc_server_io_loop_find_free_slot(loop);
    PAPACC_RESULT result;

    if (slot == NULL) {
        (void)papacc_connection_manager_remove(
            loop->connection_manager, connection->connection_instance_id);
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    *slot = (PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32)
        PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32_INITIALIZER;
    result = papacc_control_processor_init(
        &slot->processor, loop->connection_manager, &loop->session_manager,
        &loop->channel_manager, connection->connection_instance_id,
        slot->read_scratch, PAPACC_SERVER_CONTROL_READ_BUFFER_SIZE, 4U,
        papacc_server_io_loop_deadline(
            now_ns, loop->establishment_timeout_ns));
    if (result != PAPACC_RESULT_OK) {
        (void)papacc_connection_manager_remove(
            loop->connection_manager, connection->connection_instance_id);
        *slot = (PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32)
            PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32_INITIALIZER;
        return result;
    }
    slot->connection_instance_id = connection->connection_instance_id;
    slot->in_use = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

static PAPACC_BOOL papacc_server_io_loop_is_peer_failure(PAPACC_RESULT result)
{
    return result == PAPACC_RESULT_PROTOCOL_ERROR ||
        result == PAPACC_RESULT_NOT_SUPPORTED ||
        result == PAPACC_RESULT_LIMIT_EXCEEDED ||
        result == PAPACC_RESULT_INTERNAL_ERROR
        ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_server_io_loop_win32_init(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_SERVER_NETWORK *network,
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
    PAPACC_SESSION *session_storage, PAPACC_SIZE session_capacity,
    PAPACC_CHANNEL *channel_storage, PAPACC_SIZE channel_capacity,
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *processor_slots,
    PAPACC_SIZE processor_capacity, PAPACC_U64 establishment_timeout_ns)
{
    PAPACC_CONNECTION_MANAGER *connection_manager;
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (loop == NULL || network == NULL || acceptor == NULL ||
        (session_capacity > 0 && session_storage == NULL) ||
        (channel_capacity > 0 && channel_storage == NULL) ||
        (processor_capacity > 0 && processor_slots == NULL) ||
        establishment_timeout_ns == 0) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (loop->initialized == PAPACC_TRUE) return PAPACC_RESULT_INVALID_STATE;
    if (papacc_server_io_loop_network_valid(network) != PAPACC_TRUE ||
        acceptor->initialized != PAPACC_TRUE ||
        acceptor->server_network != network) return PAPACC_RESULT_INVALID_STATE;
    connection_manager =
        papacc_server_acceptor_win32_connection_manager(acceptor);
    if (connection_manager == NULL ||
        session_capacity > connection_manager->capacity ||
        channel_capacity > connection_manager->capacity ||
        processor_capacity > connection_manager->capacity)
        return PAPACC_RESULT_INVALID_STATE;
    result = papacc_session_manager_init(
        &loop->session_manager, session_storage, session_capacity);
    if (result != PAPACC_RESULT_OK) return result;
    result = papacc_channel_manager_init(
        &loop->channel_manager, channel_storage, channel_capacity,
        connection_manager, &loop->session_manager);
    if (result != PAPACC_RESULT_OK) {
        papacc_session_manager_shutdown(&loop->session_manager);
        return result;
    }
    for (index = 0; index < processor_capacity; ++index)
        processor_slots[index] =
            (PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32)
                PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32_INITIALIZER;
    loop->network = network;
    loop->acceptor = acceptor;
    loop->connection_manager = connection_manager;
    loop->processor_slots = processor_slots;
    loop->processor_capacity = processor_capacity;
    loop->next_listener_index = 0;
    loop->next_processor_index = 0;
    loop->establishment_timeout_ns = establishment_timeout_ns;
    loop->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_io_loop_win32_poll_once(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_U32 timeout_ms)
{
    fd_set read_set;
    fd_set write_set;
    struct timeval timeout;
    PAPACC_SIZE read_count;
    PAPACC_SIZE write_count = 0;
    PAPACC_BOOL has_buffered_work = PAPACC_FALSE;
    PAPACC_SIZE index;
    PAPACC_SIZE offset;
    PAPACC_U64 now_ns;
    int ready;
    PAPACC_RESULT result;

    if (loop == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (loop->initialized != PAPACC_TRUE ||
        papacc_server_io_loop_network_valid(loop->network) != PAPACC_TRUE ||
        papacc_server_acceptor_win32_connection_manager(loop->acceptor) !=
            loop->connection_manager) return PAPACC_RESULT_INVALID_STATE;
    read_count = loop->network->listener_set.count;
    for (index = 0; index < loop->processor_capacity; ++index) {
        PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *slot =
            &loop->processor_slots[index];
        if (slot->in_use == PAPACC_TRUE &&
            papacc_control_processor_wants_read(&slot->processor) == PAPACC_TRUE)
            ++read_count;
        if (slot->in_use == PAPACC_TRUE &&
            slot->processor.reader.buffered_length > 0)
            has_buffered_work = PAPACC_TRUE;
        if (slot->in_use == PAPACC_TRUE &&
            papacc_control_processor_wants_write(&slot->processor) == PAPACC_TRUE)
            ++write_count;
    }
    if (read_count > (PAPACC_SIZE)FD_SETSIZE ||
        write_count > (PAPACC_SIZE)FD_SETSIZE)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    if (papacc_pal_monotonic_time_ns(&now_ns) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INTERNAL_ERROR;
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    for (index = 0; index < loop->network->listener_set.count; ++index)
        FD_SET(loop->network->listener_set.entries[index].socket.native_socket,
               &read_set);
    for (index = 0; index < loop->processor_capacity; ++index) {
        PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *slot =
            &loop->processor_slots[index];
        SOCKET native_socket;
        if (slot->in_use != PAPACC_TRUE ||
            (papacc_control_processor_wants_read(&slot->processor) != PAPACC_TRUE &&
             papacc_control_processor_wants_write(&slot->processor) != PAPACC_TRUE))
            continue;
        result = papacc_tcp_connection_transport_win32_get_native_socket(
            &slot->processor.connection->transport, &native_socket);
        if (result != PAPACC_RESULT_OK) return result;
        if (papacc_control_processor_wants_read(&slot->processor) == PAPACC_TRUE)
            FD_SET(native_socket, &read_set);
        if (papacc_control_processor_wants_write(&slot->processor) == PAPACC_TRUE)
            FD_SET(native_socket, &write_set);
    }
    timeout.tv_sec = has_buffered_work == PAPACC_TRUE
        ? 0L : (long)(timeout_ms / 1000U);
    timeout.tv_usec = has_buffered_work == PAPACC_TRUE
        ? 0L : (long)((timeout_ms % 1000U) * 1000U);
    ready = select(0, &read_set, &write_set, NULL, &timeout);
    if (ready == SOCKET_ERROR) return PAPACC_RESULT_INTERNAL_ERROR;

    if (loop->next_listener_index >= loop->network->listener_set.count)
        loop->next_listener_index = 0;
    for (offset = 0; offset < loop->network->listener_set.count; ++offset) {
        index = (loop->next_listener_index + offset) %
            loop->network->listener_set.count;
        if (FD_ISSET(loop->network->listener_set.entries[index].socket.native_socket,
                     &read_set)) {
            PAPACC_CONNECTION *connection = NULL;
            loop->next_listener_index =
                (index + 1U) % loop->network->listener_set.count;
            result = papacc_server_acceptor_win32_accept_ready(
                loop->acceptor, index, &connection);
            if (result == PAPACC_RESULT_OK && connection != NULL)
                result = papacc_server_io_loop_attach(loop, connection, now_ns);
            if (result != PAPACC_RESULT_OK &&
                result != PAPACC_RESULT_LIMIT_EXCEEDED) return result;
            break;
        }
    }
    if (loop->processor_capacity > 0 &&
        loop->next_processor_index >= loop->processor_capacity)
        loop->next_processor_index = 0;
    for (offset = 0; offset < loop->processor_capacity; ++offset) {
        PAPACC_CONTROL_PROCESSOR_STEP_STATUS status;
        PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *slot;
        SOCKET native_socket;
        index = (loop->next_processor_index + offset) % loop->processor_capacity;
        slot = &loop->processor_slots[index];
        if (slot->in_use != PAPACC_TRUE ||
            papacc_tcp_connection_transport_win32_get_native_socket(
                &slot->processor.connection->transport, &native_socket) !=
                PAPACC_RESULT_OK) continue;
        result = PAPACC_RESULT_OK;
        if (papacc_control_processor_wants_read(&slot->processor) == PAPACC_TRUE &&
            (slot->processor.reader.buffered_length > 0 ||
             FD_ISSET(native_socket, &read_set)))
            result = papacc_control_processor_read_once(&slot->processor, &status);
        else if (papacc_control_processor_wants_write(&slot->processor) == PAPACC_TRUE &&
                 FD_ISSET(native_socket, &write_set))
            result = papacc_control_processor_write_once(&slot->processor, &status);
        else continue;
        if (result != PAPACC_RESULT_OK ||
            status == PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED ||
            slot->processor.state == PAPACC_CONTROL_PROCESSOR_STATE_ERROR) {
            if (result != PAPACC_RESULT_OK &&
                papacc_server_io_loop_is_peer_failure(result) != PAPACC_TRUE)
                return result;
            papacc_server_io_loop_reclaim(loop, slot);
        }
    }
    if (loop->processor_capacity > 0)
        loop->next_processor_index =
            (loop->next_processor_index + 1U) % loop->processor_capacity;
    if (papacc_pal_monotonic_time_ns(&now_ns) != PAPACC_RESULT_OK)
        return PAPACC_RESULT_INTERNAL_ERROR;
    for (index = 0; index < loop->processor_capacity; ++index) {
        PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32 *slot =
            &loop->processor_slots[index];
        PAPACC_CONTROL_PROCESSOR_STEP_STATUS status;
        if (slot->in_use != PAPACC_TRUE ||
            papacc_control_processor_is_established(&slot->processor) == PAPACC_TRUE)
            continue;
        result = papacc_control_processor_check_deadline(
            &slot->processor, now_ns, &status);
        if (result != PAPACC_RESULT_OK ||
            status == PAPACC_CONTROL_PROCESSOR_STEP_STATUS_CLOSED)
            papacc_server_io_loop_reclaim(loop, slot);
    }
    return PAPACC_RESULT_OK;
}

void papacc_server_io_loop_win32_shutdown(PAPACC_SERVER_IO_LOOP_WIN32 *loop)
{
    PAPACC_SIZE index;
    if (loop == NULL || loop->initialized != PAPACC_TRUE) return;
    for (index = 0; index < loop->processor_capacity; ++index) {
        if (loop->processor_slots[index].in_use == PAPACC_TRUE)
            papacc_control_processor_shutdown(
                &loop->processor_slots[index].processor);
    }
    papacc_channel_manager_shutdown(&loop->channel_manager);
    papacc_session_manager_shutdown(&loop->session_manager);
    for (index = 0; index < loop->processor_capacity; ++index)
        loop->processor_slots[index] =
            (PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32)
                PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32_INITIALIZER;
    *loop = (PAPACC_SERVER_IO_LOOP_WIN32)PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
}
