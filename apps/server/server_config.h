#ifndef PAPACC_SERVER_CONFIG_H
#define PAPACC_SERVER_CONFIG_H

#include "papacc/types.h"

typedef struct PAPACC_SERVER_CONFIG {
    PAPACC_U16 control_port;
    PAPACC_BOOL allow_network_egress;
} PAPACC_SERVER_CONFIG;

/*
 * Port zero means unspecified, not an official or usable control port.
 * Network egress is denied by default. These initialization defaults are safe
 * and deterministic, but intentionally not operationally valid until the
 * application supplies a control port.
 */
#define PAPACC_SERVER_CONFIG_INITIALIZER \
    { 0, PAPACC_FALSE }

/*
 * Validates the in-memory model independently of any future configuration
 * source. An unspecified control port returns PAPACC_RESULT_INVALID_STATE;
 * invalid pointers or boolean representations return INVALID_ARGUMENT.
 */
PAPACC_RESULT papacc_server_config_validate(
    const PAPACC_SERVER_CONFIG *config);

#endif
