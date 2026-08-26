#include <string.h>

#include "server_cli.h"

static PAPACC_RESULT papacc_server_cli_parse_u64(
    const char *text,
    PAPACC_U64 maximum,
    PAPACC_U64 *out_value)
{
    PAPACC_U64 value = 0;
    PAPACC_SIZE index;

    if (text == NULL || text[0] == '\0' || out_value == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    for (index = 0; text[index] != '\0'; ++index) {
        PAPACC_U64 digit;
        if (text[index] < '0' || text[index] > '9') {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        digit = (PAPACC_U64)(text[index] - '0');
        if (value > (maximum - digit) / 10) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        value = value * 10 + digit;
    }
    *out_value = value;
    return PAPACC_RESULT_OK;
}

static PAPACC_BOOL papacc_server_cli_has_prior_interface_id(
    int current_index,
    const char *const *argv,
    PAPACC_U64 value)
{
    int index;

    for (index = 1; index < current_index; ++index) {
        if (strcmp(argv[index], "--interface-id") == 0 &&
            index + 1 < current_index) {
            PAPACC_U64 prior_value;
            if (papacc_server_cli_parse_u64(
                    argv[index + 1], UINT64_MAX, &prior_value) ==
                    PAPACC_RESULT_OK &&
                prior_value == value) {
                return PAPACC_TRUE;
            }
            ++index;
        }
    }
    return PAPACC_FALSE;
}

static PAPACC_RESULT papacc_server_cli_scan(
    int argc,
    const char *const *argv,
    PAPACC_U16 *out_port,
    PAPACC_BOOL *out_allow_egress,
    PAPACC_BIND_SELECTION_MODE *out_bind_mode,
    PAPACC_SIZE *out_id_count)
{
    PAPACC_BOOL has_port = PAPACC_FALSE;
    PAPACC_BOOL has_all = PAPACC_FALSE;
    PAPACC_BOOL has_egress = PAPACC_FALSE;
    PAPACC_SIZE id_count = 0;
    int index;

    for (index = 1; index < argc; ++index) {
        PAPACC_U64 value;
        if (argv[index] == NULL) {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
        if (strcmp(argv[index], "--port") == 0) {
            if (has_port == PAPACC_TRUE || index + 1 >= argc ||
                argv[index + 1] == NULL ||
                papacc_server_cli_parse_u64(
                    argv[index + 1], UINT16_MAX, &value) != PAPACC_RESULT_OK) {
                return PAPACC_RESULT_INVALID_ARGUMENT;
            }
            has_port = PAPACC_TRUE;
            *out_port = (PAPACC_U16)value;
            ++index;
        } else if (strcmp(argv[index], "--all-interfaces") == 0) {
            if (has_all == PAPACC_TRUE || id_count != 0) {
                return PAPACC_RESULT_INVALID_ARGUMENT;
            }
            has_all = PAPACC_TRUE;
        } else if (strcmp(argv[index], "--interface-id") == 0) {
            if (has_all == PAPACC_TRUE || index + 1 >= argc ||
                argv[index + 1] == NULL ||
                papacc_server_cli_parse_u64(
                    argv[index + 1], UINT64_MAX, &value) != PAPACC_RESULT_OK ||
                papacc_server_cli_has_prior_interface_id(
                    index, argv, value) == PAPACC_TRUE ||
                id_count == SIZE_MAX) {
                return PAPACC_RESULT_INVALID_ARGUMENT;
            }
            ++id_count;
            ++index;
        } else if (strcmp(argv[index], "--allow-network-egress") == 0) {
            if (has_egress == PAPACC_TRUE) {
                return PAPACC_RESULT_INVALID_ARGUMENT;
            }
            has_egress = PAPACC_TRUE;
            *out_allow_egress = PAPACC_TRUE;
        } else {
            return PAPACC_RESULT_INVALID_ARGUMENT;
        }
    }
    *out_bind_mode = has_all == PAPACC_TRUE
                         ? PAPACC_BIND_SELECTION_ALL_INTERFACES
                         : (id_count != 0
                                ? PAPACC_BIND_SELECTION_SELECTED_INTERFACES
                                : PAPACC_BIND_SELECTION_UNSPECIFIED);
    *out_id_count = id_count;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_server_cli_parse(
    int argc,
    const char *const *argv,
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *id_storage,
    PAPACC_SIZE id_capacity,
    PAPACC_SERVER_CONFIG *out_config,
    PAPACC_SIZE *out_required_ids)
{
    PAPACC_SERVER_CONFIG config = PAPACC_SERVER_CONFIG_INITIALIZER;
    PAPACC_SIZE required = 0;
    PAPACC_SIZE storage_index = 0;
    PAPACC_BIND_SELECTION_MODE bind_mode;
    PAPACC_RESULT result;
    int index;

    if (out_config == NULL || out_required_ids == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    *out_config = config;
    *out_required_ids = 0;
    if (argc < 1 || argv == NULL || argv[0] == NULL ||
        (id_storage == NULL && id_capacity != 0)) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    result = papacc_server_cli_scan(
        argc, argv, &config.control_port, &config.allow_network_egress,
        &bind_mode, &required);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (id_capacity < required) {
        *out_required_ids = required;
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }

    for (index = 1; index < argc; ++index) {
        if (strcmp(argv[index], "--port") == 0) {
            ++index;
        } else if (strcmp(argv[index], "--interface-id") == 0) {
            PAPACC_U64 value;
            result = papacc_server_cli_parse_u64(
                argv[index + 1], UINT64_MAX, &value);
            if (result != PAPACC_RESULT_OK) {
                return result;
            }
            id_storage[storage_index].is_valid = PAPACC_TRUE;
            id_storage[storage_index].value = value;
            ++storage_index;
            ++index;
        }
    }
    config.bind_selection.mode = bind_mode;
    if (bind_mode == PAPACC_BIND_SELECTION_SELECTED_INTERFACES) {
        config.bind_selection.interface_persistent_ids = id_storage;
        config.bind_selection.interface_persistent_id_count = required;
    }
    result = papacc_server_config_validate(&config);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    *out_config = config;
    *out_required_ids = required;
    return PAPACC_RESULT_OK;
}
