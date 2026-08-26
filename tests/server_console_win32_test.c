#include "server_console_win32.h"

int main(void)
{
    PAPACC_SERVER_CONSOLE_WIN32 first =
        PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER;
    PAPACC_SERVER_CONSOLE_WIN32 second =
        PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER;

    if (papacc_server_console_win32_install(NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_console_win32_uninstall(NULL) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_server_console_win32_uninstall(&first) != PAPACC_RESULT_OK ||
        papacc_server_console_win32_install(&first) != PAPACC_RESULT_OK ||
        papacc_server_console_win32_stop_requested() != PAPACC_FALSE ||
        papacc_server_console_win32_install(&first) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_server_console_win32_install(&second) !=
            PAPACC_RESULT_INVALID_STATE) {
        (void)papacc_server_console_win32_uninstall(&first);
        return 1;
    }

    papacc_server_console_win32_request_stop();
    if (papacc_server_console_win32_stop_requested() != PAPACC_TRUE) {
        (void)papacc_server_console_win32_uninstall(&first);
        return 2;
    }
    papacc_server_console_win32_wait_for_stop();
    if (papacc_server_console_win32_uninstall(&first) != PAPACC_RESULT_OK ||
        first.is_installed != PAPACC_FALSE ||
        papacc_server_console_win32_stop_requested() != PAPACC_FALSE ||
        papacc_server_console_win32_install(&second) != PAPACC_RESULT_OK ||
        papacc_server_console_win32_uninstall(&second) != PAPACC_RESULT_OK) {
        return 3;
    }
    return 0;
}
