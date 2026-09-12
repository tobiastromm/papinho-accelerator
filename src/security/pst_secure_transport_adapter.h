#ifndef PAPACC_PST_SECURE_TRANSPORT_ADAPTER_H
#define PAPACC_PST_SECURE_TRANSPORT_ADAPTER_H

#include "pst_secure_processor.h"
#include "pst_secure_scheduler.h"
#include "transport_connection.h"

typedef struct PAPACC_PST_SECURE_TRANSPORT_ADAPTER {
    PAPACC_PST_SECURE_PROCESSOR *processor;
    PAPACC_PST_SECURE_SCHEDULER *scheduler;
    PAPACC_BOOL registered;
    PAPACC_BOOL closed;
} PAPACC_PST_SECURE_TRANSPORT_ADAPTER;

#define PAPACC_PST_SECURE_TRANSPORT_ADAPTER_INITIALIZER \
    { NULL, NULL, PAPACC_FALSE, PAPACC_FALSE }

PAPACC_RESULT papacc_pst_secure_transport_adapter_init(
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter,
    PAPACC_PST_SECURE_PROCESSOR *processor,
    PAPACC_PST_SECURE_SCHEDULER *scheduler,
    PAPACC_TRANSPORT_CONNECTION *out_transport);

#endif
