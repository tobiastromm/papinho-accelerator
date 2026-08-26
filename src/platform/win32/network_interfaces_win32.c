#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <stddef.h>
#include <stdlib.h>

#include "network_interface.h"

#define PAPACC_ADAPTER_BUFFER_INITIAL_SIZE ((ULONG)(15U * 1024U))
#define PAPACC_ADAPTER_BUFFER_MAX_ATTEMPTS 4

static PAPACC_NETWORK_INTERFACE_PERSISTENT_ID
papacc_network_adapter_persistent_id(const IP_ADAPTER_ADDRESSES *adapter)
{
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID persistent_id =
        PAPACC_NETWORK_INTERFACE_PERSISTENT_ID_INITIALIZER;
    const size_t luid_end = offsetof(IP_ADAPTER_ADDRESSES, Luid) +
                            sizeof(adapter->Luid);

    if ((size_t)adapter->Length >= luid_end) {
        persistent_id.is_valid = PAPACC_TRUE;
        persistent_id.value = (PAPACC_U64)adapter->Luid.Value;
    }
    return persistent_id;
}

static PAPACC_BOOL papacc_network_unicast_is_valid(
    const IP_ADAPTER_UNICAST_ADDRESS *unicast)
{
    const SOCKADDR *socket_address;

    if (unicast == NULL || unicast->Address.lpSockaddr == NULL ||
        unicast->Address.iSockaddrLength < (INT)sizeof(SOCKADDR)) {
        return PAPACC_FALSE;
    }
    socket_address = unicast->Address.lpSockaddr;
    if (socket_address->sa_family == AF_INET) {
        return (unicast->Address.iSockaddrLength >= (INT)sizeof(SOCKADDR_IN))
                   ? PAPACC_TRUE
                   : PAPACC_FALSE;
    }
    if (socket_address->sa_family == AF_INET6) {
        return (unicast->Address.iSockaddrLength >= (INT)sizeof(SOCKADDR_IN6))
                   ? PAPACC_TRUE
                   : PAPACC_FALSE;
    }
    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_network_get_adapters(
    IP_ADAPTER_ADDRESSES **out_adapters)
{
    const ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST |
                        GAA_FLAG_SKIP_DNS_SERVER;
    IP_ADAPTER_ADDRESSES *adapters = NULL;
    ULONG buffer_size = PAPACC_ADAPTER_BUFFER_INITIAL_SIZE;
    ULONG native_result = ERROR_BUFFER_OVERFLOW;
    int attempt;

    for (attempt = 0;
         attempt < PAPACC_ADAPTER_BUFFER_MAX_ATTEMPTS &&
         native_result == ERROR_BUFFER_OVERFLOW;
         ++attempt) {
        free(adapters);
        adapters = NULL;
        if (buffer_size == 0) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        adapters = (IP_ADAPTER_ADDRESSES *)malloc((size_t)buffer_size);
        if (adapters == NULL) {
            return PAPACC_RESULT_OUT_OF_MEMORY;
        }
        native_result = GetAdaptersAddresses(
            AF_UNSPEC, flags, NULL, adapters, &buffer_size);
    }
    if (native_result == ERROR_NO_DATA) {
        free(adapters);
        *out_adapters = NULL;
        return PAPACC_RESULT_OK;
    }
    if (native_result != NO_ERROR) {
        free(adapters);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }
    *out_adapters = adapters;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_network_count_snapshot(
    const IP_ADAPTER_ADDRESSES *adapters,
    PAPACC_SIZE *out_interface_count,
    PAPACC_SIZE *out_address_count,
    PAPACC_SIZE *out_presentation_size)
{
    const IP_ADAPTER_ADDRESSES *adapter;
    PAPACC_SIZE interface_count = 0;
    PAPACC_SIZE address_count = 0;
    PAPACC_SIZE presentation_size = 0;

    for (adapter = adapters; adapter != NULL; adapter = adapter->Next) {
        const IP_ADAPTER_UNICAST_ADDRESS *unicast;
        const size_t friendly_name_end =
            offsetof(IP_ADAPTER_ADDRESSES, FriendlyName) +
            sizeof(adapter->FriendlyName);
        if (interface_count == SIZE_MAX || interface_count == UINT32_MAX) {
            return PAPACC_RESULT_LIMIT_EXCEEDED;
        }
        ++interface_count;
        if ((size_t)adapter->Length >= friendly_name_end &&
            adapter->FriendlyName != NULL && adapter->FriendlyName[0] != L'\0') {
            int native_size = WideCharToMultiByte(
                CP_UTF8, 0, adapter->FriendlyName, -1,
                NULL, 0, NULL, NULL);
            if (native_size > 1) {
                if ((PAPACC_SIZE)native_size > SIZE_MAX - presentation_size) {
                    return PAPACC_RESULT_LIMIT_EXCEEDED;
                }
                presentation_size += (PAPACC_SIZE)native_size;
            }
        }
        for (unicast = adapter->FirstUnicastAddress;
             unicast != NULL;
             unicast = unicast->Next) {
            if (papacc_network_unicast_is_valid(unicast) == PAPACC_TRUE) {
                if (address_count == SIZE_MAX) {
                    return PAPACC_RESULT_LIMIT_EXCEEDED;
                }
                ++address_count;
            }
        }
    }
    *out_interface_count = interface_count;
    *out_address_count = address_count;
    *out_presentation_size = presentation_size;
    return PAPACC_RESULT_OK;
}

static PAPACC_NETWORK_INTERFACE papacc_network_convert_interface(
    const IP_ADAPTER_ADDRESSES *adapter,
    PAPACC_U32 interface_instance_id,
    char **presentation_cursor)
{
    PAPACC_NETWORK_INTERFACE interface_record =
        PAPACC_NETWORK_INTERFACE_INITIALIZER;

    interface_record.interface_instance_id = interface_instance_id;
    interface_record.persistent_id =
        papacc_network_adapter_persistent_id(adapter);
    if (presentation_cursor != NULL) {
        const size_t friendly_name_end =
            offsetof(IP_ADAPTER_ADDRESSES, FriendlyName) +
            sizeof(adapter->FriendlyName);
        if ((size_t)adapter->Length >= friendly_name_end &&
            adapter->FriendlyName != NULL && adapter->FriendlyName[0] != L'\0') {
            int native_size = WideCharToMultiByte(
                CP_UTF8, 0, adapter->FriendlyName, -1,
                NULL, 0, NULL, NULL);
            if (native_size > 1 &&
                WideCharToMultiByte(
                    CP_UTF8, 0, adapter->FriendlyName, -1,
                    *presentation_cursor, native_size, NULL, NULL) ==
                    native_size) {
                interface_record.presentation_name.is_available = PAPACC_TRUE;
                interface_record.presentation_name.utf8 = *presentation_cursor;
                interface_record.presentation_name.length =
                    (PAPACC_SIZE)(native_size - 1);
                *presentation_cursor += native_size;
            }
        }
    }
    interface_record.is_up =
        (adapter->OperStatus == IfOperStatusUp) ? PAPACC_TRUE : PAPACC_FALSE;
    interface_record.is_loopback =
        (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
            ? PAPACC_TRUE
            : PAPACC_FALSE;
    return interface_record;
}

static PAPACC_RESULT papacc_network_convert_address(
    const IP_ADAPTER_ADDRESSES *adapter,
    const IP_ADAPTER_UNICAST_ADDRESS *unicast,
    PAPACC_U32 interface_instance_id,
    PAPACC_NETWORK_INTERFACE_ADDRESS *entry)
{
    const SOCKADDR *socket_address = unicast->Address.lpSockaddr;
    const PAPACC_U8 *native_bytes;
    PAPACC_U8 ipv6_bytes[PAPACC_IP_ADDRESS_BYTE_COUNT];
    PAPACC_SIZE index;
    PAPACC_RESULT result;

    *entry = (PAPACC_NETWORK_INTERFACE_ADDRESS)
        PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER;
    if (socket_address->sa_family == AF_INET) {
        const SOCKADDR_IN *ipv4 = (const SOCKADDR_IN *)socket_address;
        native_bytes = (const PAPACC_U8 *)&ipv4->sin_addr;
        result = papacc_ip_address_set_ipv4(
            &entry->address, native_bytes[0], native_bytes[1],
            native_bytes[2], native_bytes[3]);
        entry->interface_index = (PAPACC_U32)adapter->IfIndex;
        entry->scope_id = 0;
    } else {
        const SOCKADDR_IN6 *ipv6 = (const SOCKADDR_IN6 *)socket_address;
        native_bytes = (const PAPACC_U8 *)&ipv6->sin6_addr;
        for (index = 0; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
            ipv6_bytes[index] = native_bytes[index];
        }
        result = papacc_ip_address_set_ipv6(&entry->address, ipv6_bytes);
        entry->interface_index = (PAPACC_U32)adapter->Ipv6IfIndex;
        entry->scope_id = (PAPACC_U32)ipv6->sin6_scope_id;
    }
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    entry->interface_instance_id = interface_instance_id;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_network_write_snapshot(
    const IP_ADAPTER_ADDRESSES *adapters,
    PAPACC_NETWORK_INTERFACE *interfaces,
    PAPACC_NETWORK_INTERFACE_ADDRESS *addresses,
    char *presentation_storage)
{
    const IP_ADAPTER_ADDRESSES *adapter;
    PAPACC_SIZE interface_index = 0;
    PAPACC_SIZE address_index = 0;
    char *presentation_cursor = presentation_storage;

    for (adapter = adapters; adapter != NULL; adapter = adapter->Next) {
        const IP_ADAPTER_UNICAST_ADDRESS *unicast;
        PAPACC_NETWORK_INTERFACE interface_record =
            papacc_network_convert_interface(
                adapter, (PAPACC_U32)(interface_index + 1),
                (presentation_storage != NULL) ? &presentation_cursor : NULL);
        if (interfaces != NULL) {
            interfaces[interface_index] = interface_record;
        }
        ++interface_index;
        for (unicast = adapter->FirstUnicastAddress;
             unicast != NULL;
             unicast = unicast->Next) {
            PAPACC_RESULT result;
            if (papacc_network_unicast_is_valid(unicast) == PAPACC_FALSE) {
                continue;
            }
            result = papacc_network_convert_address(
                adapter, unicast, interface_record.interface_instance_id,
                &addresses[address_index]);
            if (result != PAPACC_RESULT_OK) {
                return result;
            }
            ++address_index;
        }
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_network_discover_local_snapshot(
    PAPACC_NETWORK_INTERFACE *interface_storage,
    PAPACC_SIZE interface_capacity,
    PAPACC_NETWORK_INTERFACE_ADDRESS *address_storage,
    PAPACC_SIZE address_capacity,
    char *presentation_storage,
    PAPACC_SIZE presentation_capacity,
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT *out_snapshot,
    PAPACC_SIZE *out_interface_required,
    PAPACC_SIZE *out_address_required,
    PAPACC_SIZE *out_presentation_required)
{
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT empty_snapshot =
        PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
    IP_ADAPTER_ADDRESSES *adapters = NULL;
    PAPACC_SIZE interface_required = 0;
    PAPACC_SIZE address_required = 0;
    PAPACC_SIZE presentation_required = 0;
    PAPACC_RESULT result;

    if (out_snapshot == NULL || out_interface_required == NULL ||
        out_address_required == NULL || out_presentation_required == NULL ||
        (interface_storage == NULL && interface_capacity != 0) ||
        (address_storage == NULL && address_capacity != 0) ||
        (presentation_storage == NULL && presentation_capacity != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    *out_snapshot = empty_snapshot;
    *out_interface_required = 0;
    *out_address_required = 0;
    *out_presentation_required = 0;
    result = papacc_network_get_adapters(&adapters);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    result = papacc_network_count_snapshot(
        adapters, &interface_required, &address_required,
        &presentation_required);
    if (result != PAPACC_RESULT_OK) {
        free(adapters);
        return result;
    }
    *out_interface_required = interface_required;
    *out_address_required = address_required;
    *out_presentation_required = presentation_required;
    if (interface_capacity < interface_required ||
        address_capacity < address_required ||
        presentation_capacity < presentation_required) {
        free(adapters);
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    result = papacc_network_write_snapshot(
        adapters, interface_storage, address_storage, presentation_storage);
    free(adapters);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    out_snapshot->interfaces = interface_storage;
    out_snapshot->interface_count = interface_required;
    out_snapshot->addresses = address_storage;
    out_snapshot->address_count = address_required;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_network_discover_local_addresses(
    PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_count,
    PAPACC_SIZE *out_required)
{
    IP_ADAPTER_ADDRESSES *adapters = NULL;
    PAPACC_SIZE interface_required = 0;
    PAPACC_SIZE address_required = 0;
    PAPACC_SIZE presentation_required = 0;
    PAPACC_RESULT result;

    if (out_count == NULL || out_required == NULL ||
        (entries == NULL && capacity != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    *out_count = 0;
    *out_required = 0;
    result = papacc_network_get_adapters(&adapters);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    result = papacc_network_count_snapshot(
        adapters, &interface_required, &address_required,
        &presentation_required);
    if (result != PAPACC_RESULT_OK) {
        free(adapters);
        return result;
    }
    *out_required = address_required;
    if (capacity < address_required) {
        free(adapters);
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    result = papacc_network_write_snapshot(adapters, NULL, entries, NULL);
    free(adapters);
    if (result == PAPACC_RESULT_OK) {
        *out_count = address_required;
    }
    return result;
}
