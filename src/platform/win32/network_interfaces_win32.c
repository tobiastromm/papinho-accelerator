#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <stdlib.h>

#include "network_interface.h"

#define PAPACC_ADAPTER_BUFFER_INITIAL_SIZE ((ULONG)(15U * 1024U))
#define PAPACC_ADAPTER_BUFFER_MAX_ATTEMPTS 4

static PAPACC_BOOL papacc_network_unicast_is_valid(
    const IP_ADAPTER_UNICAST_ADDRESS *unicast)
{
    const SOCKADDR *socket_address;

    if (unicast == NULL || unicast->Address.lpSockaddr == NULL) {
        return PAPACC_FALSE;
    }
    if (unicast->Address.iSockaddrLength < (INT)sizeof(SOCKADDR)) {
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

static PAPACC_RESULT papacc_network_count_addresses(
    const IP_ADAPTER_ADDRESSES *adapters,
    PAPACC_SIZE *out_required)
{
    const IP_ADAPTER_ADDRESSES *adapter;
    const IP_ADAPTER_UNICAST_ADDRESS *unicast;
    PAPACC_SIZE required = 0;

    for (adapter = adapters; adapter != NULL; adapter = adapter->Next) {
        for (unicast = adapter->FirstUnicastAddress;
             unicast != NULL;
             unicast = unicast->Next) {
            if (papacc_network_unicast_is_valid(unicast) == PAPACC_TRUE) {
                if (required == SIZE_MAX) {
                    return PAPACC_RESULT_LIMIT_EXCEEDED;
                }
                ++required;
            }
        }
    }

    *out_required = required;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_network_convert_address(
    const IP_ADAPTER_ADDRESSES *adapter,
    const IP_ADAPTER_UNICAST_ADDRESS *unicast,
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
            &entry->address,
            native_bytes[0],
            native_bytes[1],
            native_bytes[2],
            native_bytes[3]);
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

    entry->interface_is_up =
        (adapter->OperStatus == IfOperStatusUp) ? PAPACC_TRUE : PAPACC_FALSE;
    entry->interface_is_loopback =
        (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
            ? PAPACC_TRUE
            : PAPACC_FALSE;

    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_network_write_addresses(
    const IP_ADAPTER_ADDRESSES *adapters,
    PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE *out_count)
{
    const IP_ADAPTER_ADDRESSES *adapter;
    const IP_ADAPTER_UNICAST_ADDRESS *unicast;
    PAPACC_SIZE count = 0;
    PAPACC_RESULT result;

    for (adapter = adapters; adapter != NULL; adapter = adapter->Next) {
        for (unicast = adapter->FirstUnicastAddress;
             unicast != NULL;
             unicast = unicast->Next) {
            if (papacc_network_unicast_is_valid(unicast) == PAPACC_FALSE) {
                continue;
            }

            result = papacc_network_convert_address(
                adapter,
                unicast,
                &entries[count]);
            if (result != PAPACC_RESULT_OK) {
                return result;
            }
            ++count;
        }
    }

    *out_count = count;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_network_discover_local_addresses(
    PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_count,
    PAPACC_SIZE *out_required)
{
    const ULONG flags =
        GAA_FLAG_SKIP_ANYCAST |
        GAA_FLAG_SKIP_MULTICAST |
        GAA_FLAG_SKIP_DNS_SERVER |
        GAA_FLAG_SKIP_FRIENDLY_NAME;
    IP_ADAPTER_ADDRESSES *adapters = NULL;
    ULONG buffer_size = PAPACC_ADAPTER_BUFFER_INITIAL_SIZE;
    ULONG native_result = ERROR_BUFFER_OVERFLOW;
    int attempt;
    PAPACC_SIZE required = 0;
    PAPACC_RESULT result;

    if (out_count == NULL || out_required == NULL ||
        (entries == NULL && capacity != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    *out_count = 0;
    *out_required = 0;

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
            AF_UNSPEC,
            flags,
            NULL,
            adapters,
            &buffer_size);
    }

    if (native_result == ERROR_NO_DATA) {
        free(adapters);
        return PAPACC_RESULT_OK;
    }
    if (native_result != NO_ERROR) {
        free(adapters);
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    result = papacc_network_count_addresses(adapters, &required);
    if (result != PAPACC_RESULT_OK) {
        free(adapters);
        return result;
    }
    *out_required = required;

    if (capacity < required) {
        free(adapters);
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }
    if (required == 0) {
        free(adapters);
        return PAPACC_RESULT_OK;
    }

    result = papacc_network_write_addresses(adapters, entries, out_count);
    free(adapters);
    return result;
}
