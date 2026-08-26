#ifndef PAPACC_NETWORK_NETWORK_INTERFACE_H
#define PAPACC_NETWORK_NETWORK_INTERFACE_H

#include "ip_address.h"

typedef struct PAPACC_NETWORK_INTERFACE_PERSISTENT_ID {
    PAPACC_BOOL is_valid;
    PAPACC_U64 value;
} PAPACC_NETWORK_INTERFACE_PERSISTENT_ID;

#define PAPACC_NETWORK_INTERFACE_PERSISTENT_ID_INITIALIZER \
    { PAPACC_FALSE, 0 }

typedef struct PAPACC_NETWORK_INTERFACE {
    PAPACC_U32 interface_instance_id;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID persistent_id;
    PAPACC_BOOL is_up;
    PAPACC_BOOL is_loopback;
} PAPACC_NETWORK_INTERFACE;

#define PAPACC_NETWORK_INTERFACE_INITIALIZER \
    { 0, PAPACC_NETWORK_INTERFACE_PERSISTENT_ID_INITIALIZER, \
      PAPACC_FALSE, PAPACC_FALSE }

typedef struct PAPACC_NETWORK_INTERFACE_ADDRESS {
    PAPACC_IP_ADDRESS address;
    PAPACC_U32 interface_instance_id;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID interface_persistent_id;
    PAPACC_U32 interface_index;
    PAPACC_U32 scope_id;
    PAPACC_BOOL interface_is_up;
    PAPACC_BOOL interface_is_loopback;
} PAPACC_NETWORK_INTERFACE_ADDRESS;

#define PAPACC_NETWORK_INTERFACE_ADDRESS_INITIALIZER \
    { PAPACC_IP_ADDRESS_INITIALIZER, 0, \
      PAPACC_NETWORK_INTERFACE_PERSISTENT_ID_INITIALIZER, \
      0, 0, PAPACC_FALSE, PAPACC_FALSE }

typedef struct PAPACC_NETWORK_DISCOVERY_SNAPSHOT {
    PAPACC_NETWORK_INTERFACE *interfaces;
    PAPACC_SIZE interface_count;
    PAPACC_NETWORK_INTERFACE_ADDRESS *addresses;
    PAPACC_SIZE address_count;
} PAPACC_NETWORK_DISCOVERY_SNAPSHOT;

#define PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER \
    { NULL, 0, NULL, 0 }

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
 * PAPACC_NETWORK_INTERFACE owns persistent identity and interface state.
 * Their copies in an address record are temporary projection metadata kept
 * for compatibility with the current Bind Selection pipeline and must match
 * the corresponding interface record in a discovery snapshot.
 *
 * persistent_id is an opaque, machine-local token used to find the
 * same interface in later discovery snapshots. It is not globally unique, an
 * IP address, an instance ID, an interface index, a presentation name, a
 * universal device identity, or a Wire Protocol identity. Only is_valid
 * determines availability; value zero alone has no validity meaning. Other
 * platform backends may provide a compatible token or leave it unavailable.
 * A discovery snapshot is a non-owning view over caller-provided interface
 * and address storage. Both projections come from one logical enumeration.
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

PAPACC_RESULT papacc_network_discover_local_snapshot(
    PAPACC_NETWORK_INTERFACE *interface_storage,
    PAPACC_SIZE interface_capacity,
    PAPACC_NETWORK_INTERFACE_ADDRESS *address_storage,
    PAPACC_SIZE address_capacity,
    PAPACC_NETWORK_DISCOVERY_SNAPSHOT *out_snapshot,
    PAPACC_SIZE *out_interface_required,
    PAPACC_SIZE *out_address_required);

PAPACC_BOOL papacc_network_interface_persistent_id_equal(
    const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *left,
    const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *right);

#endif
