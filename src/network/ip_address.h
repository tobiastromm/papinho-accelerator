#ifndef PAPACC_NETWORK_IP_ADDRESS_H
#define PAPACC_NETWORK_IP_ADDRESS_H

#include "papacc/types.h"

#define PAPACC_IP_ADDRESS_BYTE_COUNT 16

typedef enum PAPACC_IP_FAMILY {
    PAPACC_IP_FAMILY_UNSPECIFIED = 0,
    PAPACC_IP_FAMILY_IPV4 = 1,
    PAPACC_IP_FAMILY_IPV6 = 2
} PAPACC_IP_FAMILY;

typedef struct PAPACC_IP_ADDRESS {
    PAPACC_IP_FAMILY family;
    PAPACC_U8 bytes[PAPACC_IP_ADDRESS_BYTE_COUNT];
} PAPACC_IP_ADDRESS;

#define PAPACC_IP_ADDRESS_INITIALIZER \
    { PAPACC_IP_FAMILY_UNSPECIFIED, {0} }

/*
 * Address bytes use conventional network byte order. IPv4 uses bytes 0..3;
 * bytes 4..15 are always zeroed by the constructor. IPv6 uses all 16 bytes.
 * No native integer representation or host endianness is implied.
 *
 * PAPACC_IP_ADDRESS represents only an IP address. It is not a network
 * endpoint (address + port) or a network interface. IPv6 scope/zone IDs belong
 * to a future endpoint or interface representation and are not hidden here.
 */
PAPACC_RESULT papacc_ip_address_set_ipv4(
    PAPACC_IP_ADDRESS *address,
    PAPACC_U8 a,
    PAPACC_U8 b,
    PAPACC_U8 c,
    PAPACC_U8 d);

PAPACC_RESULT papacc_ip_address_set_ipv6(
    PAPACC_IP_ADDRESS *address,
    const PAPACC_U8 bytes[PAPACC_IP_ADDRESS_BYTE_COUNT]);

PAPACC_BOOL papacc_ip_address_equal(
    const PAPACC_IP_ADDRESS *left,
    const PAPACC_IP_ADDRESS *right);

PAPACC_BOOL papacc_ip_address_is_unspecified(
    const PAPACC_IP_ADDRESS *address);

PAPACC_BOOL papacc_ip_address_is_loopback(
    const PAPACC_IP_ADDRESS *address);

/*
 * Formats IPv4 as dotted decimal and IPv6 as eight lowercase, four-digit
 * hexadecimal groups. On success the caller buffer is NUL-terminated. An
 * insufficient buffer returns PAPACC_RESULT_LIMIT_EXCEEDED and is cleared when
 * it has at least one byte.
 */
PAPACC_RESULT papacc_ip_address_format(
    const PAPACC_IP_ADDRESS *address,
    char *buffer,
    PAPACC_SIZE buffer_size);

#endif
