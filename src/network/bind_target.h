#ifndef PAPACC_NETWORK_BIND_TARGET_H
#define PAPACC_NETWORK_BIND_TARGET_H

#include "bind_selection.h"

typedef struct PAPACC_BIND_TARGET {
    PAPACC_IP_ADDRESS address;
    PAPACC_U32 scope_id;
    PAPACC_U32 interface_instance_id;
} PAPACC_BIND_TARGET;

#define PAPACC_BIND_TARGET_INITIALIZER \
    { PAPACC_IP_ADDRESS_INITIALIZER, 0, 0 }

/*
 * Resolves transient bind intent into portable local-address targets. Targets
 * intentionally contain no service port. A NULL target buffer with capacity
 * zero is a sizing query. When capacity is insufficient, no target is written,
 * out_count remains zero, and out_required reports the complete requirement.
 * INVALID_STATE is atomic: when any explicitly selected interface has no
 * bindable address, both outputs remain zero and no partial plan is written.
 */
PAPACC_RESULT papacc_bind_targets_resolve(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,
    PAPACC_BIND_TARGET *targets,
    PAPACC_SIZE capacity,
    PAPACC_SIZE *out_count,
    PAPACC_SIZE *out_required);

#endif
