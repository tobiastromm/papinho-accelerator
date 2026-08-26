#ifndef PAPACC_NETWORK_PERSISTENT_BIND_SELECTION_H
#define PAPACC_NETWORK_PERSISTENT_BIND_SELECTION_H

#include "bind_selection.h"

typedef struct PAPACC_PERSISTENT_BIND_SELECTION {
    PAPACC_BIND_SELECTION_MODE mode;
    const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *interface_persistent_ids;
    PAPACC_SIZE interface_persistent_id_count;
} PAPACC_PERSISTENT_BIND_SELECTION;

#define PAPACC_PERSISTENT_BIND_SELECTION_INITIALIZER \
    { PAPACC_BIND_SELECTION_UNSPECIFIED, NULL, 0 }

/*
 * This model is persistable in semantics but owns no storage. The caller keeps
 * the persistent ID array alive. Persistent interface IDs are machine-local:
 * they may identify an interface across discoveries on the same host, but are
 * not universal hardware identity, Wire Protocol identity, remote-interface
 * identity, or configuration portable between computers.
 */
PAPACC_RESULT papacc_persistent_bind_selection_validate(
    const PAPACC_PERSISTENT_BIND_SELECTION *selection);

/*
 * Resolves persistent IDs only through snapshot.interfaces and preserves the
 * administrative order. The runtime selection is a non-owning view over
 * caller-provided instance ID storage. All semantic matches are validated
 * before capacity is checked. On INVALID_STATE, out_required remains zero; on
 * LIMIT_EXCEEDED it reports the complete requirement. Errors never publish a
 * partial runtime selection or write partial IDs.
 */
PAPACC_RESULT papacc_persistent_bind_selection_resolve(
    const PAPACC_PERSISTENT_BIND_SELECTION *persistent_selection,
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,
    PAPACC_U32 *interface_instance_id_storage,
    PAPACC_SIZE interface_instance_id_capacity,
    PAPACC_BIND_SELECTION *out_runtime_selection,
    PAPACC_SIZE *out_required);

#endif
