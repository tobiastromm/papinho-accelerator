#include "server_acceptor_win32.h"

static PAPACC_RESULT papacc_server_acceptor_win32_validate_network(
    const PAPACC_SERVER_NETWORK *network)
{
    PAPACC_SIZE index;

    if (network == NULL || network->is_active != PAPACC_TRUE ||
        network->tcp_platform.initialized != PAPACC_TRUE ||
        network->listener_set.is_active != PAPACC_TRUE ||
        network->listener_set.count == 0 ||
        network->listener_set.entries == NULL ||
        network->listener_set.count > network->listener_set.capacity) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    if (network->listener_set.count > (PAPACC_SIZE)FD_SETSIZE) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    for (index = 0; index < network->listener_set.count; ++index) {
        const PAPACC_TCP_SOCKET_WIN32 *socket_context =
            &network->listener_set.entries[index].socket;
        if (socket_context->is_open != PAPACC_TRUE ||
            socket_context->is_bound != PAPACC_TRUE ||
            socket_context->is_listening != PAPACC_TRUE ||
            socket_context->native_socket == INVALID_SOCKET ||
            (socket_context->family != PAPACC_IP_FAMILY_IPV4 &&
             socket_context->family != PAPACC_IP_FAMILY_IPV6)) {
            return PAPACC_RESULT_INVALID_STATE;
        }
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_server_acceptor_win32_select_error(
    int native_error)
{
    if (native_error == WSAEAFNOSUPPORT ||
        native_error == WSAEPROTONOSUPPORT ||
        native_error == WSAEOPNOTSUPP) {
        return PAPACC_RESULT_NOT_SUPPORTED;
    }
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT
    *papacc_server_acceptor_win32_find_context(
        PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor)
{
    PAPACC_SIZE index;

    for (index = 0; index < acceptor->transport_context_capacity; ++index) {
        PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context =
            &acceptor->transport_context_storage[index];
        if (context->native_socket == INVALID_SOCKET &&
            context->owns_socket == PAPACC_FALSE) {
            return context;
        }
    }
    return NULL;
}

PAPACC_CONNECTION_MANAGER *papacc_server_acceptor_win32_connection_manager(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor)
{
    if (acceptor == NULL || acceptor->initialized != PAPACC_TRUE ||
        acceptor->connection_manager.initialized != PAPACC_TRUE) {
        return NULL;
    }
    return &acceptor->connection_manager;
}

PAPACC_RESULT papacc_server_acceptor_win32_init(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
    PAPACC_SERVER_NETWORK *server_network,
    PAPACC_CONNECTION *connection_storage,
    PAPACC_SIZE connection_capacity,
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT
        *transport_context_storage,
    PAPACC_SIZE transport_context_capacity)
{
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    if (acceptor == NULL || server_network == NULL ||
        connection_capacity != transport_context_capacity ||
        (connection_capacity > 0 && connection_storage == NULL) ||
        (transport_context_capacity > 0 &&
         transport_context_storage == NULL)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (acceptor->initialized == PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_server_acceptor_win32_validate_network(server_network);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    for (index = 0; index < transport_context_capacity; ++index) {
        if (transport_context_storage[index].native_socket != INVALID_SOCKET ||
            transport_context_storage[index].owns_socket != PAPACC_FALSE) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
    }
    result = papacc_connection_manager_init(
        &acceptor->connection_manager, connection_storage,
        connection_capacity);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    acceptor->server_network = server_network;
    acceptor->transport_context_storage = transport_context_storage;
    acceptor->transport_context_capacity = transport_context_capacity;
    acceptor->next_listener_index = 0;
    acceptor->initialized = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_acceptor_win32_poll_once(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
    PAPACC_U32 timeout_ms,
    PAPACC_CONNECTION **out_connection)
{
    PAPACC_TCP_LISTENER_SET_WIN32 *listener_set;
    struct timeval timeout;
    fd_set read_set;
    PAPACC_SIZE offset;
    PAPACC_SIZE selected_index = 0;
    int ready_count;
    PAPACC_RESULT result;

    if (out_connection != NULL) {
        *out_connection = NULL;
    }
    if (acceptor == NULL || out_connection == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (acceptor->initialized != PAPACC_TRUE ||
        acceptor->connection_manager.initialized != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_server_acceptor_win32_validate_network(
        acceptor->server_network);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    listener_set = &acceptor->server_network->listener_set;
    FD_ZERO(&read_set);
    for (offset = 0; offset < listener_set->count; ++offset) {
        FD_SET(listener_set->entries[offset].socket.native_socket, &read_set);
    }
    timeout.tv_sec = (long)(timeout_ms / 1000U);
    timeout.tv_usec = (long)((timeout_ms % 1000U) * 1000U);
    ready_count = select(0, &read_set, NULL, NULL, &timeout);
    if (ready_count == SOCKET_ERROR) {
        return papacc_server_acceptor_win32_select_error(WSAGetLastError());
    }
    if (ready_count == 0) {
        return PAPACC_RESULT_OK;
    }
    if (acceptor->next_listener_index >= listener_set->count) {
        acceptor->next_listener_index = 0;
    }
    for (offset = 0; offset < listener_set->count; ++offset) {
        PAPACC_SIZE index =
            (acceptor->next_listener_index + offset) % listener_set->count;
        SOCKET native_socket = listener_set->entries[index].socket.native_socket;
        if (FD_ISSET(native_socket, &read_set)) {
            selected_index = index;
            break;
        }
    }
    if (offset == listener_set->count) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    acceptor->next_listener_index =
        (selected_index + 1U) % listener_set->count;
    return papacc_server_acceptor_win32_accept_ready(
        acceptor, selected_index, out_connection);
}

PAPACC_RESULT papacc_server_acceptor_win32_accept_ready(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor,
    PAPACC_SIZE listener_index,
    PAPACC_CONNECTION **out_connection)
{
    PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted =
        PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
    PAPACC_TRANSPORT_CONNECTION transport =
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT *context;
    PAPACC_NETWORK_ENDPOINT local_endpoint;
    PAPACC_NETWORK_ENDPOINT remote_endpoint;
    PAPACC_TCP_LISTENER_SET_WIN32 *listener_set;
    PAPACC_RESULT result;

    if (out_connection != NULL) {
        *out_connection = NULL;
    }
    if (acceptor == NULL || out_connection == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (acceptor->initialized != PAPACC_TRUE ||
        acceptor->connection_manager.initialized != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_server_acceptor_win32_validate_network(
        acceptor->server_network);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    listener_set = &acceptor->server_network->listener_set;
    if (listener_index >= listener_set->count) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    result = papacc_tcp_socket_win32_accept(
        &listener_set->entries[listener_index].socket, &accepted);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    result = papacc_tcp_accepted_socket_win32_set_nonblocking(
        &accepted, PAPACC_TRUE);
    if (result != PAPACC_RESULT_OK) {
        papacc_tcp_accepted_socket_win32_close(&accepted);
        return result;
    }
    local_endpoint = accepted.local_endpoint;
    remote_endpoint = accepted.remote_endpoint;
    context = papacc_server_acceptor_win32_find_context(acceptor);
    if (acceptor->connection_manager.count >=
            acceptor->connection_manager.capacity ||
        context == NULL) {
        papacc_tcp_accepted_socket_win32_close(&accepted);
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    result = papacc_tcp_connection_transport_win32_move(
        &accepted, context, &transport);
    if (result != PAPACC_RESULT_OK) {
        papacc_tcp_accepted_socket_win32_close(&accepted);
        return result;
    }
    result = papacc_connection_manager_publish(
        &acceptor->connection_manager, &transport, &local_endpoint,
        &remote_endpoint, out_connection);
    if (result != PAPACC_RESULT_OK) {
        papacc_transport_connection_close(&transport);
        *out_connection = NULL;
    }
    return result;
}

void papacc_server_acceptor_win32_shutdown(
    PAPACC_SERVER_ACCEPTOR_WIN32 *acceptor)
{
    if (acceptor == NULL) {
        return;
    }
    papacc_connection_manager_shutdown(&acceptor->connection_manager);
    *acceptor = (PAPACC_SERVER_ACCEPTOR_WIN32)
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
}
