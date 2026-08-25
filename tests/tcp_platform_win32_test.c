#include "tcp_platform.h"

int main(void)
{
    PAPACC_TCP_PLATFORM first = PAPACC_TCP_PLATFORM_INITIALIZER;
    PAPACC_TCP_PLATFORM second = PAPACC_TCP_PLATFORM_INITIALIZER;
    int test_result = 0;

    papacc_tcp_platform_shutdown(NULL);
    papacc_tcp_platform_shutdown(&first);

    if (first.initialized != PAPACC_FALSE) {
        return 1;
    }
    if (papacc_tcp_platform_init(NULL) != PAPACC_RESULT_INVALID_ARGUMENT) {
        return 2;
    }
    if (papacc_tcp_platform_init(&first) != PAPACC_RESULT_OK ||
        first.initialized != PAPACC_TRUE) {
        return 3;
    }
    if (papacc_tcp_platform_init(&first) != PAPACC_RESULT_INVALID_STATE ||
        first.initialized != PAPACC_TRUE) {
        test_result = 4;
        goto cleanup;
    }
    if (papacc_tcp_platform_init(&second) != PAPACC_RESULT_OK ||
        second.initialized != PAPACC_TRUE) {
        test_result = 5;
        goto cleanup;
    }

    papacc_tcp_platform_shutdown(&first);
    if (first.initialized != PAPACC_FALSE ||
        second.initialized != PAPACC_TRUE) {
        test_result = 6;
        goto cleanup;
    }

    papacc_tcp_platform_shutdown(&first);
    if (first.initialized != PAPACC_FALSE) {
        test_result = 7;
        goto cleanup;
    }

    papacc_tcp_platform_shutdown(&second);
    if (second.initialized != PAPACC_FALSE) {
        test_result = 8;
        goto cleanup;
    }

    if (papacc_tcp_platform_init(&first) != PAPACC_RESULT_OK ||
        first.initialized != PAPACC_TRUE) {
        test_result = 9;
        goto cleanup;
    }

cleanup:
    papacc_tcp_platform_shutdown(&first);
    papacc_tcp_platform_shutdown(&second);
    return test_result;
}
