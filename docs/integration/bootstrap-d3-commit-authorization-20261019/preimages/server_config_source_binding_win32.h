#ifndef PAPACC_SERVER_CONFIG_SOURCE_BINDING_WIN32_H
#define PAPACC_SERVER_CONFIG_SOURCE_BINDING_WIN32_H
#include "server_launcher_owner_win32.h"
#include "config_binding.h"
/* Existing owner thread, caller-held current pin and exclusive output required.
 * No new pin/borrow acquired. Caller retains existing pin/source graph until
 * return. Output owns value copies only; never a future admission ticket. */
PC_RESULT papacc_server_config_source_resolve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,
 const PAPACC_ROOT_CURRENT_PIN *,PAPACC_U64,const PC_DOCUMENT *,PC_MODEL_BINDING *,PC_DIAGNOSTIC *);
/* Initial result is a bound candidate, never a historical CURRENT pin/epoch.
 * root identity is copied, revision/epochs remain explicitly unavailable. */
typedef struct PAPACC_BOOTSTRAP_RESOLUTION {
 PC_MODEL_BINDING binding;
 PAPACC_U64 invocation;
 unsigned initial_resolved,epochs_present;
} PAPACC_BOOTSTRAP_RESOLUTION;
PC_RESULT papacc_server_config_initial_bootstrap_resolve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *,
 const PC_DOCUMENT *,PAPACC_BOOTSTRAP_RESOLUTION *,PC_DIAGNOSTIC *);
#endif
