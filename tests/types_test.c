#include <stddef.h>

#include "papacc/types.h"

int main(void)
{
    PAPACC_SIZE size = (PAPACC_SIZE)0;
    PAPACC_BOOL boolean = PAPACC_FALSE;
    PAPACC_RESULT result = PAPACC_RESULT_OK;

    if (sizeof(PAPACC_U8) != 1 || sizeof(PAPACC_I8) != 1) {
        return 1;
    }
    if (sizeof(PAPACC_U16) != 2 || sizeof(PAPACC_I16) != 2) {
        return 2;
    }
    if (sizeof(PAPACC_U32) != 4 || sizeof(PAPACC_I32) != 4) {
        return 3;
    }
    if (sizeof(PAPACC_U64) != 8 || sizeof(PAPACC_I64) != 8) {
        return 4;
    }
    if (PAPACC_FALSE != 0 || PAPACC_TRUE == PAPACC_FALSE) {
        return 5;
    }
    if (sizeof(PAPACC_SIZE) != sizeof(size_t) || size != 0) {
        return 6;
    }
    if (result != PAPACC_RESULT_OK || PAPACC_RESULT_OK != 0) {
        return 7;
    }
    if (PAPACC_RESULT_INVALID_ARGUMENT == PAPACC_RESULT_OK ||
        PAPACC_RESULT_OUT_OF_MEMORY == PAPACC_RESULT_OK ||
        PAPACC_RESULT_NOT_SUPPORTED == PAPACC_RESULT_OK ||
        PAPACC_RESULT_INVALID_STATE == PAPACC_RESULT_OK ||
        PAPACC_RESULT_LIMIT_EXCEEDED == PAPACC_RESULT_OK ||
        PAPACC_RESULT_INTERNAL_ERROR == PAPACC_RESULT_OK) {
        return 8;
    }

    boolean = PAPACC_TRUE;
    if (boolean != PAPACC_TRUE) {
        return 9;
    }

    return 0;
}
