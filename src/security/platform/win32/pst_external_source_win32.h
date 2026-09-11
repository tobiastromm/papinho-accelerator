#ifndef PAPACC_PST_EXTERNAL_SOURCE_WIN32_H
#define PAPACC_PST_EXTERNAL_SOURCE_WIN32_H

#include "papacc/types.h"
#include "papinho_secure_transport.h"

/* The native socket remains borrowed and is never closed by this wrapper. */
PAPACC_RESULT papacc_pst_external_source_win32_create(
    PAPACC_SIZE native_socket_value, pst_external_source **out_source);

#endif
