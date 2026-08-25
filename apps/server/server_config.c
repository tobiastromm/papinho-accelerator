#include "server_config.h"

PAPACC_RESULT papacc_server_config_validate(
    const PAPACC_SERVER_CONFIG *config)
{
    if (config == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (config->allow_network_egress != PAPACC_FALSE &&
        config->allow_network_egress != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (config->control_port == 0) {
        return PAPACC_RESULT_INVALID_STATE;
    }

    return PAPACC_RESULT_OK;
}
