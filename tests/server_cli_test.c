#include "server_cli.h"

static int config_is_initializer(const PAPACC_SERVER_CONFIG *config)
{
    return config->control_port == 0 &&
        config->allow_network_egress == PAPACC_FALSE &&
        config->bind_selection.mode == PAPACC_BIND_SELECTION_UNSPECIFIED &&
        config->bind_selection.interface_persistent_ids == NULL &&
        config->bind_selection.interface_persistent_id_count == 0;
}

static int expect_error(
    int argc,
    const char *const *argv,
    PAPACC_RESULT expected)
{
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID storage[4];
    PAPACC_SERVER_CONFIG config;
    PAPACC_SIZE required = 99;

    config.control_port = 99;
    config.allow_network_egress = PAPACC_TRUE;
    if (papacc_server_cli_parse(
            argc, argv, storage, 4, &config, &required) != expected ||
        required != 0 || !config_is_initializer(&config)) {
        return 1;
    }
    return 0;
}

static int test_empty_and_all(void)
{
    const char *empty[] = { "papacc_server.exe" };
    const char *all[] = {
        "papacc_server.exe", "--port", "4433", "--all-interfaces"
    };
    const char *all_egress[] = {
        "papacc_server.exe", "--allow-network-egress", "--all-interfaces",
        "--port", "4433"
    };
    PAPACC_SERVER_CONFIG config;
    PAPACC_SIZE required;

    if (expect_error(1, empty, PAPACC_RESULT_INVALID_STATE) != 0) {
        return 1;
    }
    if (papacc_server_cli_parse(
            4, all, NULL, 0, &config, &required) != PAPACC_RESULT_OK ||
        config.control_port != 4433 ||
        config.allow_network_egress != PAPACC_FALSE ||
        config.bind_selection.mode != PAPACC_BIND_SELECTION_ALL_INTERFACES ||
        config.bind_selection.interface_persistent_ids != NULL ||
        config.bind_selection.interface_persistent_id_count != 0 ||
        required != 0) {
        return 2;
    }
    if (papacc_server_cli_parse(
            5, all_egress, NULL, 0, &config, &required) != PAPACC_RESULT_OK ||
        config.allow_network_egress != PAPACC_TRUE || required != 0) {
        return 3;
    }
    return 0;
}

static int test_selected(void)
{
    const char *single[] = {
        "papacc_server.exe", "--port", "4433", "--interface-id", "123"
    };
    const char *multiple[] = {
        "papacc_server.exe", "--interface-id", "30", "--port", "4433",
        "--interface-id", "10", "--interface-id", "20"
    };
    const char *zero[] = {
        "papacc_server.exe", "--port", "4433", "--interface-id", "0"
    };
    const char *maximum[] = {
        "papacc_server.exe", "--port", "4433", "--interface-id",
        "18446744073709551615"
    };
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID storage[3];
    PAPACC_SERVER_CONFIG config;
    PAPACC_SIZE required;

    if (papacc_server_cli_parse(
            5, single, storage, 1, &config, &required) != PAPACC_RESULT_OK ||
        config.bind_selection.mode !=
            PAPACC_BIND_SELECTION_SELECTED_INTERFACES ||
        config.bind_selection.interface_persistent_ids != storage ||
        config.bind_selection.interface_persistent_id_count != 1 ||
        storage[0].is_valid != PAPACC_TRUE || storage[0].value != 123 ||
        required != 1) {
        return 1;
    }
    if (papacc_server_cli_parse(
            9, multiple, storage, 3, &config, &required) != PAPACC_RESULT_OK ||
        required != 3 || storage[0].value != 30 || storage[1].value != 10 ||
        storage[2].value != 20) {
        return 2;
    }
    if (papacc_server_cli_parse(
            5, zero, storage, 1, &config, &required) != PAPACC_RESULT_OK ||
        storage[0].is_valid != PAPACC_TRUE || storage[0].value != 0) {
        return 3;
    }
    if (papacc_server_cli_parse(
            5, maximum, storage, 1, &config, &required) != PAPACC_RESULT_OK ||
        storage[0].value != UINT64_MAX) {
        return 4;
    }
    return 0;
}

static int test_invalid_numbers(void)
{
    const char *values[] = {
        "abc", "12x", "-1", "+1", "0x10", "18446744073709551616"
    };
    PAPACC_SIZE index;

    for (index = 0; index < sizeof(values) / sizeof(values[0]); ++index) {
        const char *argv[] = {
            "papacc_server.exe", "--port", "4433", "--interface-id", NULL
        };
        argv[4] = values[index];
        if (expect_error(5, argv, PAPACC_RESULT_INVALID_ARGUMENT) != 0) {
            return 1;
        }
    }
    return 0;
}

static int test_ports(void)
{
    const char *zero[] = {
        "papacc_server.exe", "--port", "0", "--all-interfaces"
    };
    const char *one[] = {
        "papacc_server.exe", "--port", "1", "--all-interfaces"
    };
    const char *maximum[] = {
        "papacc_server.exe", "--port", "65535", "--all-interfaces"
    };
    const char *overflow[] = {
        "papacc_server.exe", "--port", "65536", "--all-interfaces"
    };
    PAPACC_SERVER_CONFIG config;
    PAPACC_SIZE required;

    if (expect_error(4, zero, PAPACC_RESULT_INVALID_STATE) != 0 ||
        papacc_server_cli_parse(4, one, NULL, 0, &config, &required) !=
            PAPACC_RESULT_OK ||
        papacc_server_cli_parse(4, maximum, NULL, 0, &config, &required) !=
            PAPACC_RESULT_OK ||
        expect_error(4, overflow, PAPACC_RESULT_INVALID_ARGUMENT) != 0) {
        return 1;
    }
    return 0;
}

static int test_conflicts_and_arguments(void)
{
    const char *cases[][7] = {
        { "x", "--all-interfaces", "--interface-id", "5", "--port", "1" },
        { "x", "--interface-id", "5", "--all-interfaces", "--port", "1" },
        { "x", "--port", "1000", "--port", "2000", "--all-interfaces" },
        { "x", "--all-interfaces", "--all-interfaces", "--port", "1" },
        { "x", "--allow-network-egress", "--allow-network-egress",
          "--port", "1", "--all-interfaces" },
        { "x", "--interface-id", "5", "--interface-id", "5", "--port", "1" }
    };
    const int counts[] = { 6, 6, 7, 5, 6, 7 };
    const char *unknown[] = { "x", "--banana" };
    const char *short_option[] = { "x", "-p", "4433" };
    const char *equals_form[] = { "x", "--port=4433" };
    const char *help[] = { "x", "--help" };
    const char *missing_port[] = { "x", "--port" };
    const char *missing_id[] = { "x", "--interface-id" };
    const char *positional[] = { "x", "hello" };
    PAPACC_SIZE index;

    for (index = 0; index < sizeof(counts) / sizeof(counts[0]); ++index) {
        if (expect_error(
                counts[index], cases[index], PAPACC_RESULT_INVALID_ARGUMENT) !=
            0) {
            return 1;
        }
    }
    if (expect_error(2, unknown, PAPACC_RESULT_INVALID_ARGUMENT) != 0 ||
        expect_error(3, short_option, PAPACC_RESULT_INVALID_ARGUMENT) != 0 ||
        expect_error(2, equals_form, PAPACC_RESULT_INVALID_ARGUMENT) != 0 ||
        expect_error(2, help, PAPACC_RESULT_INVALID_ARGUMENT) != 0 ||
        expect_error(2, missing_port, PAPACC_RESULT_INVALID_ARGUMENT) != 0 ||
        expect_error(2, missing_id, PAPACC_RESULT_INVALID_ARGUMENT) != 0 ||
        expect_error(2, positional, PAPACC_RESULT_INVALID_ARGUMENT) != 0) {
        return 2;
    }
    return 0;
}

static int test_sizing(void)
{
    const char *argv[] = {
        "x", "--port", "4433", "--interface-id", "30",
        "--interface-id", "10", "--interface-id", "20"
    };
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID storage[3] = {
        { PAPACC_FALSE, 99 }, { PAPACC_FALSE, 99 }, { PAPACC_FALSE, 99 }
    };
    PAPACC_SERVER_CONFIG config;
    PAPACC_SIZE required;

    if (papacc_server_cli_parse(
            9, argv, NULL, 0, &config, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || required != 3 ||
        !config_is_initializer(&config)) {
        return 1;
    }
    if (papacc_server_cli_parse(
            9, argv, storage, 2, &config, &required) !=
            PAPACC_RESULT_LIMIT_EXCEEDED || required != 3 ||
        storage[0].is_valid != PAPACC_FALSE || storage[0].value != 99 ||
        !config_is_initializer(&config)) {
        return 2;
    }
    if (papacc_server_cli_parse(
            9, argv, storage, 3, &config, &required) != PAPACC_RESULT_OK ||
        required != 3) {
        return 3;
    }
    return 0;
}

int main(void)
{
    int result = test_empty_and_all();
    if (result != 0) return 10 + result;
    result = test_selected();
    if (result != 0) return 20 + result;
    result = test_invalid_numbers();
    if (result != 0) return 30 + result;
    result = test_ports();
    if (result != 0) return 40 + result;
    result = test_conflicts_and_arguments();
    if (result != 0) return 50 + result;
    result = test_sizing();
    return (result == 0) ? 0 : 60 + result;
}
