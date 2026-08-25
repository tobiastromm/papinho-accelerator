#include "server_config.h"

static PAPACC_RESULT papacc_test_port(
    PAPACC_U16 port,
    PAPACC_BOOL allow_network_egress)
{
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;

    config.control_port = port;
    config.allow_network_egress = allow_network_egress;

    return papacc_server_config_validate(&config);
}

int main(void)
{
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;

    if (config.control_port != 0 ||
        config.allow_network_egress != PAPACC_FALSE) {
        return 1;
    }
    if (papacc_server_config_validate(&config) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 2;
    }
    if (papacc_server_config_validate(NULL) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 3;
    }

    if (papacc_test_port(1, PAPACC_FALSE) != PAPACC_RESULT_OK) {
        return 4;
    }
    if (papacc_test_port(80, PAPACC_FALSE) != PAPACC_RESULT_OK) {
        return 5;
    }
    if (papacc_test_port(443, PAPACC_TRUE) != PAPACC_RESULT_OK) {
        return 6;
    }
    if (papacc_test_port(UINT16_MAX, PAPACC_TRUE) != PAPACC_RESULT_OK) {
        return 7;
    }
    if (papacc_test_port(0, PAPACC_FALSE) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 8;
    }
    if (papacc_test_port(80, (PAPACC_BOOL)2) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 9;
    }

    return 0;
}
