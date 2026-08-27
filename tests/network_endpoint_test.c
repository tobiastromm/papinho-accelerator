#include "network_endpoint.h"

static int test_initializer_and_null(void)
{
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_SIZE index;

    if (endpoint.address.family != PAPACC_IP_FAMILY_UNSPECIFIED ||
        endpoint.port != 0 || endpoint.scope_id != 0 ||
        papacc_network_endpoint_validate(&endpoint) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_network_endpoint_validate(NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_network_endpoint_equal(NULL, &endpoint) != PAPACC_FALSE ||
        papacc_network_endpoint_equal(&endpoint, NULL) != PAPACC_FALSE ||
        papacc_network_endpoint_equal(NULL, NULL) != PAPACC_FALSE) {
        return 1;
    }
    for (index = 0; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
        if (endpoint.address.bytes[index] != 0) {
            return 2;
        }
    }
    return 0;
}

static int test_ipv4(void)
{
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT same = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT different = PAPACC_NETWORK_ENDPOINT_INITIALIZER;

    if (papacc_ip_address_set_ipv4(
            &endpoint.address, 127, 0, 0, 1) != PAPACC_RESULT_OK) {
        return 10;
    }
    endpoint.port = 4433;
    same = endpoint;
    different = endpoint;
    if (papacc_network_endpoint_validate(&endpoint) != PAPACC_RESULT_OK ||
        papacc_network_endpoint_equal(&endpoint, &same) != PAPACC_TRUE) {
        return 11;
    }
    different.port = 4434;
    if (papacc_network_endpoint_equal(&endpoint, &different) != PAPACC_FALSE) {
        return 12;
    }
    different = endpoint;
    if (papacc_ip_address_set_ipv4(
            &different.address, 127, 0, 0, 2) != PAPACC_RESULT_OK ||
        papacc_network_endpoint_equal(&endpoint, &different) != PAPACC_FALSE) {
        return 13;
    }
    endpoint.port = 0;
    if (papacc_network_endpoint_validate(&endpoint) != PAPACC_RESULT_OK) {
        return 14;
    }
    endpoint.scope_id = 5;
    if (papacc_network_endpoint_validate(&endpoint) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 15;
    }
    return 0;
}

static int test_ipv6(void)
{
    const PAPACC_U8 global_bytes[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1
    };
    const PAPACC_U8 link_local_bytes[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0xfe, 0x80, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1
    };
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT other = PAPACC_NETWORK_ENDPOINT_INITIALIZER;

    if (papacc_ip_address_set_ipv6(&endpoint.address, global_bytes) !=
            PAPACC_RESULT_OK) {
        return 20;
    }
    endpoint.port = 4433;
    if (papacc_network_endpoint_validate(&endpoint) != PAPACC_RESULT_OK) {
        return 21;
    }
    if (papacc_ip_address_set_ipv6(&endpoint.address, link_local_bytes) !=
            PAPACC_RESULT_OK) {
        return 22;
    }
    endpoint.scope_id = 12;
    if (papacc_network_endpoint_validate(&endpoint) != PAPACC_RESULT_OK) {
        return 23;
    }
    other = endpoint;
    other.scope_id = 13;
    if (papacc_network_endpoint_equal(&endpoint, &other) != PAPACC_FALSE) {
        return 24;
    }
    endpoint.port = 0;
    if (papacc_network_endpoint_validate(&endpoint) != PAPACC_RESULT_OK) {
        return 25;
    }
    return 0;
}

static int test_unknown_family(void)
{
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;

    endpoint.address.family = (PAPACC_IP_FAMILY)99;
    return papacc_network_endpoint_validate(&endpoint) ==
                   PAPACC_RESULT_INVALID_ARGUMENT
               ? 0
               : 30;
}

int main(void)
{
    int result = test_initializer_and_null();
    if (result != 0) {
        return result;
    }
    result = test_ipv4();
    if (result != 0) {
        return result;
    }
    result = test_ipv6();
    if (result != 0) {
        return result;
    }
    return test_unknown_family();
}
