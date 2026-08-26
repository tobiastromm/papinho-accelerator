#ifndef PAPACC_SERVER_CONFIG_H
#define PAPACC_SERVER_CONFIG_H

#include "persistent_bind_selection.h"

typedef struct PAPACC_SERVER_CONFIG {
    PAPACC_U16 control_port;
    PAPACC_BOOL allow_network_egress;
    PAPACC_PERSISTENT_BIND_SELECTION bind_selection;
} PAPACC_SERVER_CONFIG;

/*
 * Port zero means unspecified, not an official or usable control port.
 * Network egress is denied and bind intent is UNSPECIFIED by default. These
 * initialization defaults are safe and deterministic, but intentionally not
 * operationally valid until the application supplies both decisions.
 *
 * The model is configuration-source agnostic: CLI, GUI, files, Registry,
 * firmware, tests, or programmatic APIs may construct the same structure.
 * It owns no heap and is copyable by value. For SELECTED_INTERFACES, copying
 * the config copies only the persistent-ID view; the source/caller owns that
 * array and keeps it alive while the config is consulted. The config never
 * modifies, reallocates, or frees it.
 */
#define PAPACC_SERVER_CONFIG_INITIALIZER \
    { 0, PAPACC_FALSE, PAPACC_PERSISTENT_BIND_SELECTION_INITIALIZER }

/*
 * Validates the in-memory model independently of any future configuration
 * source and without consulting discovery state. Validation order is boolean
 * representation, persistent bind selection, then operational port. Result
 * categories from persistent selection validation are preserved.
 */
PAPACC_RESULT papacc_server_config_validate(
    const PAPACC_SERVER_CONFIG *config);

#endif
