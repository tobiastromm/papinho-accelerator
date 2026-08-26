#include <stdlib.h>

#include "server_network.h"

#define PAPACC_SERVER_NETWORK_DISCOVERY_ATTEMPTS 4

typedef struct PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE {
    PAPACC_NETWORK_INTERFACE *interfaces;
    PAPACC_NETWORK_INTERFACE_ADDRESS *addresses;
    char *presentation;
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot;
} PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE;

#define PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE_INITIALIZER \
    { NULL, NULL, NULL, PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER }

static void papacc_server_network_snapshot_free(
    PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE *storage)
{
    free(storage->interfaces);
    free(storage->addresses);
    free(storage->presentation);
    *storage = (PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE)
        PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE_INITIALIZER;
}

static PAPACC_RESULT papacc_server_network_snapshot_discover(
    PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE *storage)
{
    PAPACC_SIZE interface_required = 0;
    PAPACC_SIZE address_required = 0;
    PAPACC_SIZE presentation_required = 0;
    PAPACC_RESULT result;
    int attempt;

    result = papacc_network_discover_local_snapshot(
        NULL, 0, NULL, 0, NULL, 0, &storage->snapshot,
        &interface_required, &address_required, &presentation_required);
    if (result != PAPACC_RESULT_OK &&
        result != PAPACC_RESULT_LIMIT_EXCEEDED) {
        return result;
    }
    for (attempt = 0; attempt < PAPACC_SERVER_NETWORK_DISCOVERY_ATTEMPTS;
         ++attempt) {
        papacc_server_network_snapshot_free(storage);
        if (interface_required > SIZE_MAX / sizeof(*storage->interfaces) ||
            address_required > SIZE_MAX / sizeof(*storage->addresses)) {
            return PAPACC_RESULT_LIMIT_EXCEEDED;
        }
        if (interface_required != 0) {
            storage->interfaces = (PAPACC_NETWORK_INTERFACE *)malloc(
                interface_required * sizeof(*storage->interfaces));
        }
        if (address_required != 0) {
            storage->addresses = (PAPACC_NETWORK_INTERFACE_ADDRESS *)malloc(
                address_required * sizeof(*storage->addresses));
        }
        if (presentation_required != 0) {
            storage->presentation = (char *)malloc(presentation_required);
        }
        if ((interface_required != 0 && storage->interfaces == NULL) ||
            (address_required != 0 && storage->addresses == NULL) ||
            (presentation_required != 0 && storage->presentation == NULL)) {
            papacc_server_network_snapshot_free(storage);
            return PAPACC_RESULT_OUT_OF_MEMORY;
        }
        result = papacc_network_discover_local_snapshot(
            storage->interfaces, interface_required,
            storage->addresses, address_required,
            storage->presentation, presentation_required, &storage->snapshot,
            &interface_required, &address_required, &presentation_required);
        if (result != PAPACC_RESULT_LIMIT_EXCEEDED) {
            return result;
        }
    }
    papacc_server_network_snapshot_free(storage);
    return PAPACC_RESULT_LIMIT_EXCEEDED;
}

void papacc_server_network_shutdown(PAPACC_SERVER_NETWORK *network)
{
    if (network == NULL) {
        return;
    }
    papacc_tcp_listener_set_win32_shutdown(&network->listener_set);
    free(network->listener_storage);
    network->listener_storage = NULL;
    network->listener_storage_capacity = 0;
    papacc_tcp_platform_shutdown(&network->tcp_platform);
    *network = (PAPACC_SERVER_NETWORK)PAPACC_SERVER_NETWORK_INITIALIZER;
}

PAPACC_RESULT papacc_server_network_start(
    PAPACC_SERVER_NETWORK *network,
    const PAPACC_SERVER_CONFIG *config)
{
    PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE discovery =
        PAPACC_SERVER_NETWORK_SNAPSHOT_STORAGE_INITIALIZER;
    PAPACC_BIND_SELECTION runtime_selection = PAPACC_BIND_SELECTION_INITIALIZER;
    PAPACC_U32 *runtime_ids = NULL;
    PAPACC_SIZE runtime_required = 0;
    PAPACC_BIND_TARGET *targets = NULL;
    PAPACC_SIZE target_count = 0;
    PAPACC_SIZE target_required = 0;
    PAPACC_TCP_PLATFORM platform = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_LISTENER_SET_WIN32 listener_set =
        PAPACC_TCP_LISTENER_SET_WIN32_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 *listener_storage = NULL;
    PAPACC_RESULT result;

    if (network == NULL || config == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (network->is_active == PAPACC_TRUE ||
        network->tcp_platform.initialized != PAPACC_FALSE ||
        network->listener_set.is_active != PAPACC_FALSE ||
        network->listener_storage != NULL) {
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_server_config_validate(config);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    result = papacc_server_network_snapshot_discover(&discovery);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }

    result = papacc_persistent_bind_selection_resolve(
        &config->bind_selection, &discovery.snapshot, NULL, 0,
        &runtime_selection, &runtime_required);
    if (result == PAPACC_RESULT_LIMIT_EXCEEDED) {
        if (runtime_required > SIZE_MAX / sizeof(*runtime_ids)) {
            result = PAPACC_RESULT_LIMIT_EXCEEDED;
            goto cleanup;
        }
        runtime_ids = (PAPACC_U32 *)malloc(
            runtime_required * sizeof(*runtime_ids));
        if (runtime_ids == NULL) {
            result = PAPACC_RESULT_OUT_OF_MEMORY;
            goto cleanup;
        }
        result = papacc_persistent_bind_selection_resolve(
            &config->bind_selection, &discovery.snapshot,
            runtime_ids, runtime_required, &runtime_selection,
            &runtime_required);
    }
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }

    result = papacc_bind_targets_resolve(
        &runtime_selection, &discovery.snapshot, NULL, 0,
        &target_count, &target_required);
    if (result != PAPACC_RESULT_LIMIT_EXCEEDED) {
        if (result == PAPACC_RESULT_OK) {
            result = PAPACC_RESULT_INVALID_STATE;
        }
        goto cleanup;
    }
    if (target_required > SIZE_MAX / sizeof(*targets) ||
        target_required > SIZE_MAX / sizeof(*listener_storage)) {
        result = PAPACC_RESULT_LIMIT_EXCEEDED;
        goto cleanup;
    }
    targets = (PAPACC_BIND_TARGET *)malloc(
        target_required * sizeof(*targets));
    listener_storage = (PAPACC_TCP_LISTENER_ENTRY_WIN32 *)malloc(
        target_required * sizeof(*listener_storage));
    if (targets == NULL || listener_storage == NULL) {
        result = PAPACC_RESULT_OUT_OF_MEMORY;
        goto cleanup;
    }
    result = papacc_bind_targets_resolve(
        &runtime_selection, &discovery.snapshot,
        targets, target_required, &target_count, &target_required);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }

    result = papacc_tcp_platform_init(&platform);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }
    result = papacc_tcp_listener_set_win32_start(
        &platform, targets, target_count, config->control_port,
        listener_storage, target_count, &listener_set);
    if (result != PAPACC_RESULT_OK) {
        goto cleanup;
    }

    network->tcp_platform = platform;
    network->listener_set = listener_set;
    network->listener_storage = listener_storage;
    network->listener_storage_capacity = target_count;
    network->is_active = PAPACC_TRUE;
    listener_storage = NULL;
    platform = (PAPACC_TCP_PLATFORM)PAPACC_TCP_PLATFORM_INITIALIZER;
    listener_set = (PAPACC_TCP_LISTENER_SET_WIN32)
        PAPACC_TCP_LISTENER_SET_WIN32_INITIALIZER;
    result = PAPACC_RESULT_OK;

cleanup:
    papacc_tcp_listener_set_win32_shutdown(&listener_set);
    free(listener_storage);
    papacc_tcp_platform_shutdown(&platform);
    free(targets);
    free(runtime_ids);
    papacc_server_network_snapshot_free(&discovery);
    return result;
}
