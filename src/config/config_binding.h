#ifndef PAPACC_CONFIG_BINDING_H
#define PAPACC_CONFIG_BINDING_H
#include "config_source.h"
#include "server_config.h"
#include "bind_target.h"
/* Private read-only bridge to existing canonical model, not a registry.
 * MODEL_BOUND is never RESOLVED or CURRENT. Only the native root adapter can
 * additionally establish factual pin/source/currency. No callback or allocation.
 * Caller owns immutable inputs for the synchronous call; output owns its bytes. */
typedef struct PC_BOUND_LISTENER {
 char id[PC_TEXT_MAX+1U];
 unsigned profile,admission,port;
 PAPACC_U8 security_ref[64],policy_identity[32];
 PAPACC_SIZE security_length,target_count;
 PAPACC_BIND_TARGET targets[2];
} PC_BOUND_LISTENER;
typedef struct PC_MODEL_BINDING {
 unsigned model_bound,resolved,count,issuer_present;
 PC_BOUND_LISTENER listeners[PC_LISTENERS_MAX];
 PAPACC_U8 root[32];PAPACC_U64 revision,administrative_epoch,policy_epoch,issuer_revision;
} PC_MODEL_BINDING;
PC_RESULT pc_bind_existing_models(const PC_DOCUMENT *,
 const PAPACC_SERVER_CONFIG *const *,PAPACC_SIZE,PC_MODEL_BINDING *,PC_DIAGNOSTIC *);
/* Pure binding against one immutable borrowed discovery snapshot. Its storage
 * and the models remain caller-owned for the entire call, with no concurrent
 * mutation/Stop-driven destruction. This API cannot prove producer identity,
 * administrative revision or CURRENT and NEVER sets resolved. Missing snapshot
 * rejects SELECTED; the old no-snapshot entry therefore remains fail-closed. */
PC_RESULT pc_bind_existing_models_snapshot(const PC_DOCUMENT *,
 const PAPACC_SERVER_CONFIG *const *,PAPACC_SIZE,
 const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *,PC_MODEL_BINDING *,PC_DIAGNOSTIC *);
/* Value snapshot only. No secret handles, pins or source borrows escape. */
#endif
