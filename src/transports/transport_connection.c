#include "transport_connection.h"

PAPACC_BOOL papacc_transport_connection_is_valid(
    const PAPACC_TRANSPORT_CONNECTION *transport)
{
    return (transport != NULL && transport->context != NULL &&
            transport->close_fn != NULL)
               ? PAPACC_TRUE
               : PAPACC_FALSE;
}

void papacc_transport_connection_close(PAPACC_TRANSPORT_CONNECTION *transport)
{
    PAPACC_TRANSPORT_CLOSE_FN close_fn;
    void *context;

    if (papacc_transport_connection_is_valid(transport) != PAPACC_TRUE) {
        return;
    }
    close_fn = transport->close_fn;
    context = transport->context;
    *transport = (PAPACC_TRANSPORT_CONNECTION)
        PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    close_fn(context);
}
