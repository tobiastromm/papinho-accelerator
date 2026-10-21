#ifndef PAPACC_PRIVATE_BODY05_H
#define PAPACC_PRIVATE_BODY05_H
#include "server_configuration_body04_win32.h"
PAPACC_RESULT papacc_configuration_body05_encode(const PAPACC_CONFIG_VIEW *,PAPACC_U8 *,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_body05_decode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_CONFIG_VIEW *);
#endif
