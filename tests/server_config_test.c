#include "server_config.h"

static void set_all(PAPACC_SERVER_CONFIG *config, PAPACC_U16 port)
{
    *config = (PAPACC_SERVER_CONFIG)PAPACC_SERVER_CONFIG_INITIALIZER;
    config->control_port = port;
    config->bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
}

static PAPACC_RESULT validate_all_port(
    PAPACC_U16 port,
    PAPACC_BOOL allow_network_egress)
{
    PAPACC_SERVER_CONFIG config;
    set_all(&config, port);
    config.allow_network_egress = allow_network_egress;
    return papacc_server_config_validate(&config);
}

static int test_initializer_and_all(void)
{
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;

    if (config.control_port != 0 ||
        config.allow_network_egress != PAPACC_FALSE ||
        config.bind_selection.mode != PAPACC_BIND_SELECTION_UNSPECIFIED ||
        config.bind_selection.interface_persistent_ids != NULL ||
        config.bind_selection.interface_persistent_id_count != 0) {
        return 1;
    }
    if (papacc_server_config_validate(&config) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_server_config_validate(NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 2;
    }
    set_all(&config, 4433);
    if (papacc_server_config_validate(&config) != PAPACC_RESULT_OK) {
        return 3;
    }
    config.allow_network_egress = PAPACC_TRUE;
    if (papacc_server_config_validate(&config) != PAPACC_RESULT_OK) {
        return 4;
    }
    return 0;
}

static int test_selected(void)
{
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID ids[2] = {
        { PAPACC_TRUE, 10 }, { PAPACC_TRUE, 20 }
    };
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;
    PAPACC_SERVER_CONFIG copied;

    config.control_port = 4433;
    config.bind_selection.mode = PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
    config.bind_selection.interface_persistent_ids = ids;
    config.bind_selection.interface_persistent_id_count = 2;
    if (papacc_server_config_validate(&config) != PAPACC_RESULT_OK) {
        return 1;
    }
    copied = config;
    if (copied.control_port != config.control_port ||
        copied.allow_network_egress != config.allow_network_egress ||
        copied.bind_selection.interface_persistent_ids != ids ||
        copied.bind_selection.interface_persistent_id_count != 2) {
        return 2;
    }
    ids[0].value = 0;
    if (papacc_server_config_validate(&config) != PAPACC_RESULT_OK) {
        return 3;
    }
    ids[1] = ids[0];
    if (papacc_server_config_validate(&config) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 4;
    }
    ids[1].value = 20;
    ids[0].is_valid = PAPACC_FALSE;
    if (papacc_server_config_validate(&config) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 5;
    }
    return 0;
}

static int test_validation_order_and_ports(void)
{
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;

    config.control_port = 4433;
    if (papacc_server_config_validate(&config) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 1;
    }
    set_all(&config, 0);
    if (papacc_server_config_validate(&config) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 2;
    }
    if (validate_all_port(1, PAPACC_FALSE) != PAPACC_RESULT_OK ||
        validate_all_port(80, PAPACC_FALSE) != PAPACC_RESULT_OK ||
        validate_all_port(443, PAPACC_TRUE) != PAPACC_RESULT_OK ||
        validate_all_port(UINT16_MAX, PAPACC_TRUE) != PAPACC_RESULT_OK) {
        return 3;
    }
    set_all(&config, 80);
    config.allow_network_egress = (PAPACC_BOOL)2;
    if (papacc_server_config_validate(&config) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 4;
    }

    config.bind_selection.mode = PAPACC_BIND_SELECTION_UNSPECIFIED;
    config.bind_selection.interface_persistent_ids =
        (const PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *)&config;
    config.bind_selection.interface_persistent_id_count = 1;
    if (papacc_server_config_validate(&config) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 5;
    }
    return 0;
}

int main(void)
{
    int result = test_initializer_and_all();
    if (result != 0) {
        return 10 + result;
    }
    result = test_selected();
    if (result != 0) {
        return 30 + result;
    }
    result = test_validation_order_and_ports();
    return (result == 0) ? 0 : 50 + result;
}
