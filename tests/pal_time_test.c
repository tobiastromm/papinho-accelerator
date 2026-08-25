#include "pal_time.h"

int main(void)
{
    PAPACC_U64 previous;
    PAPACC_U64 current;
    PAPACC_SIZE index;

    if (papacc_pal_monotonic_time_ns(NULL) !=
        PAPACC_RESULT_INVALID_ARGUMENT) {
        return 1;
    }

    if (papacc_pal_monotonic_time_ns(&previous) != PAPACC_RESULT_OK) {
        return 2;
    }

    for (index = 0; index < (PAPACC_SIZE)1000; ++index) {
        if (papacc_pal_monotonic_time_ns(&current) != PAPACC_RESULT_OK) {
            return 3;
        }
        if (current < previous) {
            return 4;
        }
        previous = current;
    }

    return 0;
}
