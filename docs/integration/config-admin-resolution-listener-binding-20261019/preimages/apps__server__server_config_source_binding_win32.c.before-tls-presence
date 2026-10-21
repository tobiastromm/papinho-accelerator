#include "server_config_source_binding_win32.h"
#include <string.h>
static PC_RESULT deny(PC_MODEL_BINDING *out,PC_DIAGNOSTIC *d,const char *why)
{if(out)memset(out,0,sizeof(*out));if(d){d->code=PC_REFERENCE;d->line=d->column=0U;d->reason=why;}return PC_REFERENCE;}
PC_RESULT papacc_server_config_source_resolve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,
 const PAPACC_ROOT_CURRENT_PIN *pin,PAPACC_U64 expected_revision,const PC_DOCUMENT *doc,
 PC_MODEL_BINDING *out,PC_DIAGNOSTIC *diag)
{
 const PAPACC_SERVER_CONFIG *models[1];PAPACC_SECURITY_SOURCE_TRANSACTION *source;PC_RESULT result;
 const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *security;
 PAPACC_SIZE caller_reserved=sizeof(PC_DOCUMENT)+PC_SOURCE_MAX+sizeof(PC_MODEL_BINDING)+sizeof(PC_DIAGNOSTIC)+sizeof(models)+16384U;
 if(!out)return PC_INVALID;memset(out,0,sizeof(*out));
 /* Existing function checks exact root object, held pin provenance and CURRENT.
  * No caller flag or PC_OK can stand in for this check. */
 if(!o||!pin||!expected_revision||papacc_server_launcher_configuration_current(o,pin)!=PAPACC_TRUE)
  return deny(out,diag,"no factual current administrative root pin");
 if(GetCurrentThreadId()!=o->owner_thread||pin->view.revision!=expected_revision||
    o->network.is_active||o->configuration_commit.stopped||o->configuration_commit.unresolved||
    o->configuration_commit.authority_lost||papacc_server_console_win32_stop_requested()==PAPACC_TRUE)
  return deny(out,diag,"wrong thread, stale revision, Stop, active runtime or retained authority");
 if(caller_reserved>PAPACC_LAUNCHER_MANAGED_LIMIT||o->configuration_preparation.combined_reserved>PAPACC_LAUNCHER_MANAGED_LIMIT-caller_reserved)
  return deny(out,diag,"managed reservation insufficient for input, AST, output and resolution stack allowance");
 if(!pc_view(doc)||doc->listener_count!=1U)return deny(out,diag,"canonical root currently supports exactly one logical listener; no truncation");
 source=&o->configuration_preparation.source;security=&source->configuration;
 if(source->state!=PAPACC_SECURITY_SOURCE_PUBLISHED||papacc_server_security_source_validate(source)!=PAPACC_RESULT_OK||
    !source->reference.valid||source->reference.length==0U||source->reference.length>64U||!security->principal_resolve||!security->authorize||
    source->reference.length!=pin->view.config.security_configuration_ref_length||
    memcmp(source->reference.value,pin->view.config.security_configuration_ref,source->reference.length))
  return deny(out,diag,"existing published Secure source unavailable or divergent");
 models[0]=&pin->view.config;result=pc_bind_existing_models(doc,models,1U,out,diag);
 if(result!=PC_OK)return result;
 /* Pure sync read/copy above invokes no provider callbacks or I/O. Revalidate
  * before marking RESOLVED; Stop never waits for any new worker or observer. */
 if(papacc_server_launcher_configuration_current(o,pin)!=PAPACC_TRUE||
    o->configuration_commit.stopped||papacc_server_console_win32_stop_requested()==PAPACC_TRUE||source->state!=PAPACC_SECURITY_SOURCE_PUBLISHED||
    papacc_server_security_source_validate(source)!=PAPACC_RESULT_OK)
  return deny(out,diag,"root/source changed during resolution");
 memcpy(out->root,pin->view.domain,32U);out->revision=pin->view.revision;
 out->administrative_epoch=pin->administrative_epoch;out->policy_epoch=pin->epoch;
 out->issuer_revision=pin->issuer_present?pin->issuer_revision:0U;out->resolved=1U;
 if(diag)diag->reason="resolved value snapshot; publication and operational authority remain denied";
 return PC_OK;
}
