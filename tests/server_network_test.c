#include <stdlib.h>
#include <ws2tcpip.h>

#include "server_network.h"

static int network_is_inactive(const PAPACC_SERVER_NETWORK *network)
{
    return network->tcp_platform.initialized == PAPACC_FALSE &&
        network->listener_set.entries == NULL &&
        network->listener_set.count == 0 &&
        network->listener_set.is_active == PAPACC_FALSE &&
        network->listener_storage == NULL &&
        network->listener_storage_capacity == 0 &&
        network->is_active == PAPACC_FALSE;
}

static PAPACC_RESULT reserve_loopback_port(
    PAPACC_TCP_PLATFORM *platform,
    PAPACC_TCP_SOCKET_WIN32 *socket_context,
    PAPACC_U16 *out_port)
{
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    struct sockaddr_in native_address;
    int native_size = (int)sizeof(native_address);
    PAPACC_RESULT result;

    result = papacc_tcp_platform_init(platform);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1);
    result = papacc_tcp_socket_win32_bind(
        platform, &target, 0, socket_context);
    if (result != PAPACC_RESULT_OK) {
        papacc_tcp_platform_shutdown(platform);
        return result;
    }
    result = papacc_tcp_socket_win32_listen(socket_context);
    if (result != PAPACC_RESULT_OK ||
        getsockname(
            socket_context->native_socket,
            (struct sockaddr *)&native_address, &native_size) == SOCKET_ERROR) {
        papacc_tcp_socket_win32_close(socket_context);
        papacc_tcp_platform_shutdown(platform);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    *out_port = (PAPACC_U16)ntohs(native_address.sin_port);
    return *out_port != 0 ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT discover_test_ids(
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *out_loopback_id,
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *out_missing_id)
{
    PAPACC_NETWORK_INTERFACE *interfaces = NULL;
    PAPACC_NETWORK_INTERFACE_ADDRESS *addresses = NULL;
    char *presentation = NULL;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    PAPACC_SIZE interface_required = 0;
    PAPACC_SIZE address_required = 0;
    PAPACC_SIZE presentation_required = 0;
    PAPACC_RESULT result;
    int attempt;

    result = papacc_network_discover_local_snapshot(
        NULL, 0, NULL, 0, NULL, 0, &snapshot, &interface_required,
        &address_required, &presentation_required);
    if (result != PAPACC_RESULT_OK &&
        result != PAPACC_RESULT_LIMIT_EXCEEDED) {
        return result;
    }
    for (attempt = 0; attempt < 4; ++attempt) {
        PAPACC_SIZE index;
        free(interfaces);
        free(addresses);
        free(presentation);
        interfaces = NULL;
        addresses = NULL;
        presentation = NULL;
        if (interface_required > SIZE_MAX / sizeof(*interfaces) ||
            address_required > SIZE_MAX / sizeof(*addresses)) {
            result = PAPACC_RESULT_LIMIT_EXCEEDED;
            break;
        }
        if (interface_required != 0) {
            interfaces = (PAPACC_NETWORK_INTERFACE *)malloc(
                interface_required * sizeof(*interfaces));
        }
        if (address_required != 0) {
            addresses = (PAPACC_NETWORK_INTERFACE_ADDRESS *)malloc(
                address_required * sizeof(*addresses));
        }
        if (presentation_required != 0) {
            presentation = (char *)malloc(presentation_required);
        }
        if ((interface_required != 0 && interfaces == NULL) ||
            (address_required != 0 && addresses == NULL) ||
            (presentation_required != 0 && presentation == NULL)) {
            result = PAPACC_RESULT_OUT_OF_MEMORY;
            break;
        }
        result = papacc_network_discover_local_snapshot(
            interfaces, interface_required, addresses, address_required,
            presentation, presentation_required, &snapshot,
            &interface_required, &address_required, &presentation_required);
        if (result != PAPACC_RESULT_LIMIT_EXCEEDED) {
            if (result == PAPACC_RESULT_OK) {
                PAPACC_U64 candidate = 0;
                PAPACC_BOOL candidate_used;
                *out_loopback_id = (PAPACC_NETWORK_INTERFACE_PERSISTENT_ID)
                    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID_INITIALIZER;
                for (index = 0; index < snapshot.interface_count; ++index) {
                    PAPACC_SIZE address_index;
                    if (snapshot.interfaces[index].is_loopback != PAPACC_TRUE ||
                        snapshot.interfaces[index].persistent_id.is_valid !=
                            PAPACC_TRUE) {
                        continue;
                    }
                    for (address_index = 0;
                         address_index < snapshot.address_count;
                         ++address_index) {
                        if (snapshot.addresses[address_index]
                                    .interface_instance_id ==
                                snapshot.interfaces[index]
                                    .interface_instance_id &&
                            papacc_ip_address_is_unspecified(
                                &snapshot.addresses[address_index].address) ==
                                PAPACC_FALSE) {
                            *out_loopback_id =
                                snapshot.interfaces[index].persistent_id;
                            break;
                        }
                    }
                    if (out_loopback_id->is_valid == PAPACC_TRUE) {
                        break;
                    }
                }
                do {
                    candidate_used = PAPACC_FALSE;
                    for (index = 0; index < snapshot.interface_count; ++index) {
                        if (snapshot.interfaces[index].persistent_id.is_valid ==
                                PAPACC_TRUE &&
                            snapshot.interfaces[index].persistent_id.value ==
                                candidate) {
                            candidate_used = PAPACC_TRUE;
                            ++candidate;
                            break;
                        }
                    }
                } while (candidate_used == PAPACC_TRUE && candidate != 0);
                out_missing_id->is_valid = PAPACC_TRUE;
                out_missing_id->value = candidate;
            }
            break;
        }
    }
    free(interfaces);
    free(addresses);
    free(presentation);
    return result;
}

static int test_invalid_and_missing(void)
{
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID loopback_id;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID missing_id;

    papacc_server_network_shutdown(NULL);
    papacc_server_network_shutdown(&network);
    if (!network_is_inactive(&network) ||
        papacc_server_network_start(NULL, &config) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_network_start(&network, NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_network_start(&network, &config) !=
            PAPACC_RESULT_INVALID_STATE ||
        !network_is_inactive(&network)) {
        return 1;
    }
    if (discover_test_ids(&loopback_id, &missing_id) != PAPACC_RESULT_OK) {
        return 2;
    }
    config.control_port = 1;
    config.bind_selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    config.bind_selection.interface_persistent_ids = &missing_id;
    config.bind_selection.interface_persistent_id_count = 1;
    if (papacc_server_network_start(&network, &config) !=
            PAPACC_RESULT_INVALID_STATE ||
        !network_is_inactive(&network)) {
        return 3;
    }
    return 0;
}

static int test_all_rollback_and_reuse(void)
{
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;
    PAPACC_TCP_PLATFORM blocker_platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_SOCKET_WIN32 blocker = PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_U16 port;
    PAPACC_RESULT result;

    if (reserve_loopback_port(&blocker_platform, &blocker, &port) !=
        PAPACC_RESULT_OK) {
        return 1;
    }
    config.control_port = port;
    config.allow_network_egress = PAPACC_TRUE;
    config.bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    result = papacc_server_network_start(&network, &config);
    if (result == PAPACC_RESULT_OK || !network_is_inactive(&network)) {
        papacc_server_network_shutdown(&network);
        papacc_tcp_socket_win32_close(&blocker);
        papacc_tcp_platform_shutdown(&blocker_platform);
        return 2;
    }
    papacc_tcp_socket_win32_close(&blocker);
    papacc_tcp_platform_shutdown(&blocker_platform);

    if (papacc_server_network_start(&network, &config) != PAPACC_RESULT_OK ||
        network.is_active != PAPACC_TRUE ||
        network.tcp_platform.initialized != PAPACC_TRUE ||
        network.listener_set.is_active != PAPACC_TRUE ||
        network.listener_set.count == 0 ||
        network.listener_set.bound_port != port ||
        network.listener_storage == NULL) {
        papacc_server_network_shutdown(&network);
        return 3;
    }
    if (papacc_server_network_start(&network, &config) !=
            PAPACC_RESULT_INVALID_STATE ||
        network.is_active != PAPACC_TRUE) {
        papacc_server_network_shutdown(&network);
        return 4;
    }
    papacc_server_network_shutdown(&network);
    papacc_server_network_shutdown(&network);
    if (!network_is_inactive(&network)) {
        return 5;
    }
    if (papacc_server_network_start(&network, &config) != PAPACC_RESULT_OK) {
        return 6;
    }
    papacc_server_network_shutdown(&network);
    return !network_is_inactive(&network) ? 7 : 0;
}

static int test_selected_loopback(void)
{
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID loopback_id;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID missing_id;
    PAPACC_TCP_PLATFORM port_platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_SOCKET_WIN32 port_socket = PAPACC_TCP_SOCKET_WIN32_INITIALIZER;
    PAPACC_U16 port;
    PAPACC_RESULT result;

    result = discover_test_ids(&loopback_id, &missing_id);
    if (result != PAPACC_RESULT_OK) {
        return 1;
    }
    if (loopback_id.is_valid != PAPACC_TRUE) {
        return 0;
    }
    if (reserve_loopback_port(&port_platform, &port_socket, &port) !=
        PAPACC_RESULT_OK) {
        return 2;
    }
    papacc_tcp_socket_win32_close(&port_socket);
    papacc_tcp_platform_shutdown(&port_platform);
    config.control_port = port;
    config.bind_selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    config.bind_selection.interface_persistent_ids = &loopback_id;
    config.bind_selection.interface_persistent_id_count = 1;
    result = papacc_server_network_start(&network, &config);
    if (result != PAPACC_RESULT_OK || network.listener_set.count == 0 ||
        network.listener_set.bound_port != port) {
        papacc_server_network_shutdown(&network);
        return 3;
    }
    papacc_server_network_shutdown(&network);
    return 0;
}

int main(void)
{
    int result = test_invalid_and_missing();
    if (result != 0) return 10 + result;
    result = test_all_rollback_and_reuse();
    if (result != 0) return 30 + result;
    result = test_selected_loopback();
    return result == 0 ? 0 : 50 + result;
}
