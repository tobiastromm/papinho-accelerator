#include <string.h>

#include "ip_address.h"

static int papacc_test_ipv4(void)
{
    PAPACC_IP_ADDRESS address = PAPACC_IP_ADDRESS_INITIALIZER;
    PAPACC_IP_ADDRESS equal = PAPACC_IP_ADDRESS_INITIALIZER;
    PAPACC_IP_ADDRESS different = PAPACC_IP_ADDRESS_INITIALIZER;
    char text[40];
    PAPACC_SIZE index;

    if (papacc_ip_address_set_ipv4(&address, 192, 168, 1, 120) !=
        PAPACC_RESULT_OK) {
        return 1;
    }
    if (address.family != PAPACC_IP_FAMILY_IPV4 ||
        address.bytes[0] != 192 || address.bytes[1] != 168 ||
        address.bytes[2] != 1 || address.bytes[3] != 120) {
        return 2;
    }
    for (index = 4; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
        if (address.bytes[index] != 0) {
            return 3;
        }
    }

    papacc_ip_address_set_ipv4(&equal, 192, 168, 1, 120);
    papacc_ip_address_set_ipv4(&different, 192, 168, 1, 121);
    if (papacc_ip_address_equal(&address, &equal) != PAPACC_TRUE ||
        papacc_ip_address_equal(&address, &different) != PAPACC_FALSE) {
        return 4;
    }
    if (papacc_ip_address_format(&address, text, sizeof(text)) !=
            PAPACC_RESULT_OK ||
        strcmp(text, "192.168.1.120") != 0) {
        return 5;
    }

    papacc_ip_address_set_ipv4(&address, 0, 0, 0, 0);
    if (papacc_ip_address_is_unspecified(&address) != PAPACC_TRUE) {
        return 6;
    }
    papacc_ip_address_set_ipv4(&address, 127, 0, 0, 1);
    if (papacc_ip_address_is_loopback(&address) != PAPACC_TRUE) {
        return 7;
    }
    papacc_ip_address_set_ipv4(&address, 127, 10, 20, 30);
    if (papacc_ip_address_is_loopback(&address) != PAPACC_TRUE) {
        return 8;
    }
    papacc_ip_address_set_ipv4(&address, 126, 255, 255, 255);
    if (papacc_ip_address_is_loopback(&address) != PAPACC_FALSE) {
        return 9;
    }
    papacc_ip_address_set_ipv4(&address, 128, 0, 0, 1);
    if (papacc_ip_address_is_loopback(&address) != PAPACC_FALSE) {
        return 10;
    }

    return 0;
}

static int papacc_test_ipv6(void)
{
    const PAPACC_U8 example_bytes[PAPACC_IP_ADDRESS_BYTE_COUNT] = {
        0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
    };
    PAPACC_U8 other_bytes[PAPACC_IP_ADDRESS_BYTE_COUNT] = {0};
    PAPACC_IP_ADDRESS address = PAPACC_IP_ADDRESS_INITIALIZER;
    PAPACC_IP_ADDRESS equal = PAPACC_IP_ADDRESS_INITIALIZER;
    PAPACC_IP_ADDRESS different = PAPACC_IP_ADDRESS_INITIALIZER;
    char text[40];

    if (papacc_ip_address_set_ipv6(&address, example_bytes) !=
            PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv6(&equal, example_bytes) !=
            PAPACC_RESULT_OK) {
        return 1;
    }
    other_bytes[15] = 2;
    papacc_ip_address_set_ipv6(&different, other_bytes);
    if (papacc_ip_address_equal(&address, &equal) != PAPACC_TRUE ||
        papacc_ip_address_equal(&address, &different) != PAPACC_FALSE) {
        return 2;
    }
    if (papacc_ip_address_format(&address, text, sizeof(text)) !=
            PAPACC_RESULT_OK ||
        strcmp(text, "2001:0db8:0000:0000:0000:0000:0000:0001") != 0) {
        return 3;
    }

    other_bytes[15] = 0;
    papacc_ip_address_set_ipv6(&address, other_bytes);
    if (papacc_ip_address_is_unspecified(&address) != PAPACC_TRUE) {
        return 4;
    }
    other_bytes[15] = 1;
    papacc_ip_address_set_ipv6(&address, other_bytes);
    if (papacc_ip_address_is_loopback(&address) != PAPACC_TRUE) {
        return 5;
    }

    return 0;
}

static int papacc_test_contract(void)
{
    PAPACC_IP_ADDRESS address = PAPACC_IP_ADDRESS_INITIALIZER;
    PAPACC_IP_ADDRESS other = PAPACC_IP_ADDRESS_INITIALIZER;
    PAPACC_U8 ipv6[PAPACC_IP_ADDRESS_BYTE_COUNT] = {0};
    char small[4] = {'x', 'x', 'x', '\0'};
    char text[40];
    PAPACC_SIZE index;

    if (address.family != PAPACC_IP_FAMILY_UNSPECIFIED ||
        papacc_ip_address_is_unspecified(&address) != PAPACC_TRUE ||
        papacc_ip_address_equal(&address, &other) != PAPACC_TRUE) {
        return 1;
    }
    for (index = 0; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
        if (address.bytes[index] != 0) {
            return 2;
        }
    }
    if (papacc_ip_address_format(&address, text, sizeof(text)) !=
        PAPACC_RESULT_INVALID_STATE) {
        return 3;
    }
    papacc_ip_address_set_ipv4(&address, 192, 168, 1, 120);
    if (papacc_ip_address_format(&address, small, sizeof(small)) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        small[0] != '\0') {
        return 4;
    }
    if (papacc_ip_address_set_ipv4(NULL, 1, 2, 3, 4) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_ip_address_set_ipv6(NULL, ipv6) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_ip_address_set_ipv6(&address, NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 5;
    }
    if (papacc_ip_address_equal(NULL, &address) != PAPACC_FALSE ||
        papacc_ip_address_is_unspecified(NULL) != PAPACC_FALSE ||
        papacc_ip_address_is_loopback(NULL) != PAPACC_FALSE) {
        return 6;
    }
    if (papacc_ip_address_format(NULL, text, sizeof(text)) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_ip_address_format(&address, NULL, sizeof(text)) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 7;
    }

    return 0;
}

int main(void)
{
    int result;

    result = papacc_test_ipv4();
    if (result != 0) {
        return 10 + result;
    }
    result = papacc_test_ipv6();
    if (result != 0) {
        return 30 + result;
    }
    result = papacc_test_contract();
    if (result != 0) {
        return 50 + result;
    }

    return 0;
}
