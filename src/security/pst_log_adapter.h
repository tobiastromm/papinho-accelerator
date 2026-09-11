#ifndef PAPACC_PST_LOG_ADAPTER_H
#define PAPACC_PST_LOG_ADAPTER_H

#include "log.h"
#include "papinho_secure_transport.h"

typedef struct PAPACC_PST_LOG_ADAPTER {
    PAPACC_LOGGER logger;
    PAPACC_BOOL ready;
} PAPACC_PST_LOG_ADAPTER;

#define PAPACC_PST_LOG_ADAPTER_INITIALIZER \
    { { NULL, NULL, PAPACC_LOG_LEVEL_OFF }, PAPACC_FALSE }

PAPACC_RESULT papacc_pst_log_adapter_init(PAPACC_PST_LOG_ADAPTER *adapter,
    const PAPACC_LOGGER *logger);

void papacc_pst_log_adapter_release(PAPACC_PST_LOG_ADAPTER *adapter);

PAPACC_RESULT papacc_pst_log_adapter_make_config(
    PAPACC_PST_LOG_ADAPTER *adapter, PST_LOG_CONFIG *config);

void PST_CALL papacc_pst_log_adapter_callback(void *user_context,
    const PST_LOG_EVENT *event);

#endif
