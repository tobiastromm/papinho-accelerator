#include "pst_provider_bootstrap_win32.h"

#include "papinho_secure_transport_win32.h"

PAPACC_RESULT papacc_pst_provider_bootstrap_win32(void *context)
{
    static int providers_registered = 0;
    PST_RESULT result;
    (void)context;
    if (providers_registered) return PAPACC_RESULT_OK;
    result = pst_win32_register_builtin_providers();
    if (result == PST_RESULT_OK) {
        providers_registered = 1;
        return PAPACC_RESULT_OK;
    }
    if (result == PST_RESULT_UNSUPPORTED)
        return PAPACC_RESULT_NOT_SUPPORTED;
    if (result == PST_RESULT_OUT_OF_MEMORY)
        return PAPACC_RESULT_OUT_OF_MEMORY;
    return PAPACC_RESULT_INTERNAL_ERROR;
}
