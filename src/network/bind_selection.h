#ifndef PAPACC_NETWORK_BIND_SELECTION_H
#define PAPACC_NETWORK_BIND_SELECTION_H

#include "network_interface.h"

typedef enum PAPACC_BIND_SELECTION_MODE {
    PAPACC_BIND_SELECTION_UNSPECIFIED = 0,
    PAPACC_BIND_SELECTION_ALL_INTERFACES = 1,
    PAPACC_BIND_SELECTION_SELECTED_INTERFACES = 2
} PAPACC_BIND_SELECTION_MODE;

typedef struct PAPACC_BIND_SELECTION {
    PAPACC_BIND_SELECTION_MODE mode;
    const PAPACC_U32 *interface_instance_ids;
    PAPACC_SIZE interface_instance_count;
} PAPACC_BIND_SELECTION;

#define PAPACC_BIND_SELECTION_INITIALIZER \
    { PAPACC_BIND_SELECTION_UNSPECIFIED, NULL, 0 }

/*
 * This structure is a non-owning view. It never allocates, copies, or frees the
 * ID array; the caller must keep the array valid for every use of the view.
 * Interface instance IDs are snapshot-only, so SELECTED_INTERFACES selections
 * are transient runtime objects and must not be persisted as configuration.
 *
 * ALL_INTERFACES is a semantic mode, not 0.0.0.0 stored as configuration.
 * Snapshot validation uses the interface catalog as the sole authority for
 * interface existence; an interface need not have an address to be selected.
 * Translation to IPv4/IPv6 wildcard addresses belongs to bind-target
 * resolution and listener layers.
 */
PAPACC_RESULT papacc_bind_selection_validate(
    const PAPACC_BIND_SELECTION *selection);

PAPACC_RESULT papacc_bind_selection_validate_snapshot(
    const PAPACC_BIND_SELECTION *selection,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot);

#endif
