#ifndef PAPACC_NETWORK_NETWORK_INTERFACE_H
#define PAPACC_NETWORK_NETWORK_INTERFACE_H

#include "ip_address.h"

typedef struct PAPACC_NETWORK_INTERFACE_ADDRESS {
    PAPACC_IP_ADDRESS address;
    PAPACC_U32 interface_instance_id;
    PAPACC_U32 interface_index;
    PAPACC_U32 scope_id;
    PAPACC_BOOL interface_is_up;
    PAPACC_BOOL interface_is_loopback;
} PAPACC_NETWORK_INTERFACE_ADDRESS;

#define PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER \
    { PAPACC_IP_ADDRESS_INITIALIZER, 0, 0, 0, PAPACC_FALSE, PAPACC_FALSE }

/*
 * interface_instance_id groups addresses produced from the same interface in
 * one discovery snapshot. Zero means not associated with a discovered
 * interface. The ID is valid only within that single returned snapshot: it
 * must not be persisted or assumed stable across discovery calls, reboots,
 * adapter state changes, topology changes, or platforms.
 *
 * interface_index identifies the interface at runtime and is not persistent
 * across boots or adapter changes. It remains separate because IPv4 IfIndex
 * and IPv6 Ipv6IfIndex have technical family-specific meaning and are not the
 * cross-family grouping identity. IPv4 entries always have scope_id zero;
 * IPv6 scope IDs are kept outside PAPACC_IP_ADDRESS.
 *
 * Discovery reports facts and applies no address selection or ranking policy.
 * On success, out_count and out_required both describe the complete snapshot.
 * If capacity is insufficient, no entries are written, out_count is zero,
 * out_required reports the needed capacity, and LIMIT_EXCEEDED is returned.
 * Passing entries == NULL and capacity == 0 is the sizing query form. It
 * returns OK when no addresses exist, otherwise LIMIT_EXCEEDED.
 */
PAPACC_RESULT papacc_network_discover_local_addresses(
    PAPACC_NETWORK_INTERFACE_ADDRESS *entries,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_count,
    PAPACC_SIZE *out_required);

#endif
