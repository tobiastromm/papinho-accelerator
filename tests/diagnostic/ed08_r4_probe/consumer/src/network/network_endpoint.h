#ifndef PAPACC_NETWORK_NETWORK_ENDPOINT_H
#define PAPACC_NETWORK_NETWORK_ENDPOINT_H

#include "ip_address.h"

typedef struct PAPACC_NETWORK_ENDPOINT {
    PAPACC_IP_ADDRESS address;
    PAPACC_U16 port;
    PAPACC_U32 scope_id;
} PAPACC_NETWORK_ENDPOINT;

#define PAPACC_NETWORK_ENDPOINT_INITIALIZER \
    { PAPACC_IP_ADDRESS_INITIALIZER, 0, 0 }

/*
 * An IP network endpoint value for future local/remote Connection metadata.
 * Port is semantic host-byte-order data; native backends perform byte-order
 * conversion at their boundaries. Port zero is representable and does not by
 * itself make the value invalid.
 *
 * IPv4 endpoints are canonical only with scope_id zero. IPv6 endpoints retain
 * any numeric scope_id without inferring whether one is operationally needed.
 * A scope ID is not an interface instance, persistent identity, or presentation
 * identity. This model owns no heap and carries no interface identity.
 *
 * PAPACC_NETWORK_ENDPOINT is distinct from PAPACC_BIND_TARGET: an endpoint is
 * local/remote IP metadata for an established transport, while a Bind Target
 * is runtime listening intent with interface association. It is also not a
 * universal endpoint for future non-IP transports such as LOCAL_PCI/LOCAL_ISA.
 * A future Win32 accept backend may convert native local/remote address
 * metadata into this model; no native transport representation is part of
 * this contract.
 */
PAPACC_RESULT papacc_network_endpoint_validate(
    const PAPACC_NETWORK_ENDPOINT *endpoint);

/* NULL compared with any value, including NULL, is false. */
PAPACC_BOOL papacc_network_endpoint_equal(
    const PAPACC_NETWORK_ENDPOINT *left,
    const PAPACC_NETWORK_ENDPOINT *right);

#endif
