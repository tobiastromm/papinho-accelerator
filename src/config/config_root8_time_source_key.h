#ifndef PAPACC_ROOT8_TIME_SOURCE_KEY_H
#define PAPACC_ROOT8_TIME_SOURCE_KEY_H
#include "config_root8.h"
/* Private kind9 DATA adapter. Caller protects immutable row384/header512
 * until synchronous return; no references escape, no owner/pin/authority.
 * Called only after codec structure/integrity checks, never on Stop path. */
PC_ROOT8_RESULT pc_root8_time_source_key_data(void *,unsigned,const PAPACC_U8 *,
 PAPACC_SIZE,const PAPACC_U8 *,const PAPACC_U8 *);
#endif
