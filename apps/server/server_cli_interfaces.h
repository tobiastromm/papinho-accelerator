#ifndef PAPACC_SERVER_CLI_INTERFACES_H
#define PAPACC_SERVER_CLI_INTERFACES_H

#include <stdio.h>

#include "network_interface.h"

PAPACC_RESULT papacc_server_cli_print_interface_snapshot(
    const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,
    FILE *output);

PAPACC_RESULT papacc_server_cli_list_interfaces(FILE *output);

#endif
