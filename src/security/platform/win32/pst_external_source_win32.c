#include "pst_external_source_win32.h"

#include "papinho_secure_transport_win32.h"

PAPACC_RESULT papacc_pst_external_source_win32_create(
    PAPACC_SIZE native_socket_value, pst_external_source **out_source)
{
    PST_RESULT result;
    if (out_source == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *out_source = NULL;
    result = pst_win32_socket_external_source_create(native_socket_value,
        out_source);
    if (result == PST_RESULT_OK) return PAPACC_RESULT_OK;
    if (result == PST_RESULT_INVALID_ARGUMENT)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (result == PST_RESULT_OUT_OF_MEMORY)
        return PAPACC_RESULT_OUT_OF_MEMORY;
    if (result == PST_RESULT_UNSUPPORTED)
        return PAPACC_RESULT_NOT_SUPPORTED;
    return PAPACC_RESULT_INTERNAL_ERROR;
}
