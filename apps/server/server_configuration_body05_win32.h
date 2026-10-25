#ifndef PAPACC_PRIVATE_BODY05_H
#define PAPACC_PRIVATE_BODY05_H
#include "server_configuration_body04_win32.h"
/* hash context stays caller-owned through failure/retention; never stack-local
 * if a failed native cleanup may outlive the call. No implicit retry/timeout. */
PAPACC_RESULT papacc_configuration_body05_encode(PAPACC_STORE_HASH *,const PAPACC_CONFIG_VIEW *,PAPACC_U8 *,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_body05_decode(PAPACC_STORE_HASH *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_CONFIG_VIEW *);
#endif
