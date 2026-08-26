#include <stdio.h>

#include "papacc/version.h"
#include "server_cli.h"
#include "server_cli_interfaces.h"

int main(int argc, char **argv)
{
    if (argc > 1) {
        PAPACC_SERVER_CLI_REQUEST request =
            PAPACC_SERVER_CLI_REQUEST_INITIALIZER;
        PAPACC_SIZE required_ids = 0;
        PAPACC_RESULT result = papacc_server_cli_request_parse(
            argc, (const char *const *)argv, NULL, 0, &request,
            &required_ids);

        if (result != PAPACC_RESULT_OK) {
            return 1;
        }
        if (request.action == PAPACC_SERVER_CLI_ACTION_LIST_INTERFACES) {
            return papacc_server_cli_list_interfaces(stdout) == PAPACC_RESULT_OK
                       ? 0
                       : 1;
        }
        return 1;
    }

    puts("PapinhoAccelerator Server");
    puts("Foundation build");
    printf("Software version %s\n", papacc_version_string());

    return 0;
}
