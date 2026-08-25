#include "ip_address.h"

#define PAPACC_IPV4_TEXT_CAPACITY 16
#define PAPACC_IPV6_TEXT_CAPACITY 40

static PAPACC_BOOL papacc_ip_bytes_are_zero(
    const PAPACC_U8 *bytes,
    PAPACC_SIZE count)
{
    PAPACC_SIZE index;

    for (index = 0; index < count; ++index) {
        if (bytes[index] != 0) {
            return PAPACC_FALSE;
        }
    }

    return PAPACC_TRUE;
}

static void papacc_ip_write_ipv4_byte(
    char *buffer,
    PAPACC_SIZE *offset,
    PAPACC_U8 value)
{
    if (value >= 100) {
        buffer[(*offset)++] = (char)('0' + value / 100);
        value = (PAPACC_U8)(value % 100);
        buffer[(*offset)++] = (char)('0' + value / 10);
    } else if (value >= 10) {
        buffer[(*offset)++] = (char)('0' + value / 10);
    }

    buffer[(*offset)++] = (char)('0' + value % 10);
}

PAPACC_RESULT papacc_ip_address_set_ipv4(
    PAPACC_IP_ADDRESS *address,
    PAPACC_U8 a,
    PAPACC_U8 b,
    PAPACC_U8 c,
    PAPACC_U8 d)
{
    PAPACC_SIZE index;

    if (address == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    address->family = PAPACC_IP_FAMILY_IPV4;
    address->bytes[0] = a;
    address->bytes[1] = b;
    address->bytes[2] = c;
    address->bytes[3] = d;
    for (index = 4; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
        address->bytes[index] = 0;
    }

    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_ip_address_set_ipv6(
    PAPACC_IP_ADDRESS *address,
    const PAPACC_U8 bytes[PAPACC_IP_ADDRESS_BYTE_COUNT])
{
    PAPACC_SIZE index;

    if (address == NULL || bytes == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    address->family = PAPACC_IP_FAMILY_IPV6;
    for (index = 0; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
        address->bytes[index] = bytes[index];
    }

    return PAPACC_RESULT_OK;
}

PAPACC_BOOL papacc_ip_address_equal(
    const PAPACC_IP_ADDRESS *left,
    const PAPACC_IP_ADDRESS *right)
{
    PAPACC_SIZE count;
    PAPACC_SIZE index;

    if (left == NULL || right == NULL || left->family != right->family) {
        return PAPACC_FALSE;
    }

    if (left->family == PAPACC_IP_FAMILY_UNSPECIFIED) {
        return PAPACC_TRUE;
    }
    if (left->family == PAPACC_IP_FAMILY_IPV4) {
        count = 4;
    } else if (left->family == PAPACC_IP_FAMILY_IPV6) {
        count = PAPACC_IP_ADDRESS_BYTE_COUNT;
    } else {
        return PAPACC_FALSE;
    }

    for (index = 0; index < count; ++index) {
        if (left->bytes[index] != right->bytes[index]) {
            return PAPACC_FALSE;
        }
    }

    return PAPACC_TRUE;
}

PAPACC_BOOL papacc_ip_address_is_unspecified(
    const PAPACC_IP_ADDRESS *address)
{
    if (address == NULL) {
        return PAPACC_FALSE;
    }
    if (address->family == PAPACC_IP_FAMILY_UNSPECIFIED) {
        return PAPACC_TRUE;
    }
    if (address->family == PAPACC_IP_FAMILY_IPV4) {
        return papacc_ip_bytes_are_zero(address->bytes, 4);
    }
    if (address->family == PAPACC_IP_FAMILY_IPV6) {
        return papacc_ip_bytes_are_zero(
            address->bytes,
            PAPACC_IP_ADDRESS_BYTE_COUNT);
    }

    return PAPACC_FALSE;
}

PAPACC_BOOL papacc_ip_address_is_loopback(
    const PAPACC_IP_ADDRESS *address)
{
    PAPACC_SIZE index;

    if (address == NULL) {
        return PAPACC_FALSE;
    }
    if (address->family == PAPACC_IP_FAMILY_IPV4) {
        return (address->bytes[0] == 127) ? PAPACC_TRUE : PAPACC_FALSE;
    }
    if (address->family != PAPACC_IP_FAMILY_IPV6) {
        return PAPACC_FALSE;
    }

    for (index = 0; index < 15; ++index) {
        if (address->bytes[index] != 0) {
            return PAPACC_FALSE;
        }
    }

    return (address->bytes[15] == 1) ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_ip_address_format(
    const PAPACC_IP_ADDRESS *address,
    char *buffer,
    PAPACC_SIZE buffer_size)
{
    const char hex_digits[] = "0123456789abcdef";
    PAPACC_SIZE offset = 0;
    PAPACC_SIZE index;

    if (address == NULL || buffer == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (address->family == PAPACC_IP_FAMILY_IPV4) {
        if (buffer_size < PAPACC_IPV4_TEXT_CAPACITY) {
            if (buffer_size > 0) {
                buffer[0] = '\0';
            }
            return PAPACC_RESULT_LIMIT_EXCEEDED;
        }

        for (index = 0; index < 4; ++index) {
            if (index != 0) {
                buffer[offset++] = '.';
            }
            papacc_ip_write_ipv4_byte(buffer, &offset, address->bytes[index]);
        }
        buffer[offset] = '\0';
        return PAPACC_RESULT_OK;
    }
    if (address->family == PAPACC_IP_FAMILY_IPV6) {
        if (buffer_size < PAPACC_IPV6_TEXT_CAPACITY) {
            if (buffer_size > 0) {
                buffer[0] = '\0';
            }
            return PAPACC_RESULT_LIMIT_EXCEEDED;
        }

        for (index = 0; index < PAPACC_IP_ADDRESS_BYTE_COUNT; ++index) {
            if (index != 0 && (index % 2) == 0) {
                buffer[offset++] = ':';
            }
            buffer[offset++] = hex_digits[address->bytes[index] >> 4];
            buffer[offset++] = hex_digits[address->bytes[index] & 0x0f];
        }
        buffer[offset] = '\0';
        return PAPACC_RESULT_OK;
    }

    if (buffer_size > 0) {
        buffer[0] = '\0';
    }
    return PAPACC_RESULT_INVALID_STATE;
}
