#ifndef PAPACC_SERVER_CLI_H
#define PAPACC_SERVER_CLI_H

#include "server_config.h"
#include "log.h"

typedef enum PAPACC_SERVER_CLI_ACTION {
    PAPACC_SERVER_CLI_ACTION_RUN_SERVER = 0,
    PAPACC_SERVER_CLI_ACTION_LIST_INTERFACES = 1,
    PAPACC_SERVER_CLI_ACTION_HELP = 2
} PAPACC_SERVER_CLI_ACTION;

typedef struct PAPACC_SERVER_CLI_REQUEST {
    PAPACC_SERVER_CLI_ACTION action;
    PAPACC_SERVER_CONFIG config;
    PAPACC_LOG_LEVEL log_level;
} PAPACC_SERVER_CLI_REQUEST;

#define PAPACC_SERVER_CLI_REQUEST_INITIALIZER \
    { PAPACC_SERVER_CLI_ACTION_RUN_SERVER, PAPACC_SERVER_CONFIG_INITIALIZER, \
      PAPACC_LOG_INFO }

/*
 * argv[0] is ignored as the executable name. The parser owns no storage and
 * produces only PAPACC_SERVER_CONFIG. For SELECTED_INTERFACES, the resulting
 * config views caller-provided id_storage, which must remain alive while used.
 * NULL/zero storage is a sizing query. Syntax and model errors leave required
 * zero; insufficient capacity reports the complete requirement. All errors
 * leave out_config at PAPACC_SERVER_CONFIG_INITIALIZER.
 */
PAPACC_RESULT papacc_server_cli_parse(
    int argc,
    const char *const *argv,
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *id_storage,
    PAPACC_SIZE id_capacity,
    PAPACC_SERVER_CONFIG *out_config,
    PAPACC_SIZE *out_required_ids);

PAPACC_RESULT papacc_server_cli_request_parse(
    int argc,
    const char *const *argv,
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID *id_storage,
    PAPACC_SIZE id_capacity,
    PAPACC_SERVER_CLI_REQUEST *out_request,
    PAPACC_SIZE *out_required_ids);

#endif
