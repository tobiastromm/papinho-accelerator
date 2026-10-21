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
    !security->composition_inputs.secure_principal_alpn||
    security->composition_inputs.secure_principal_alpn_size!=PAPACC_SECURITY_ALPN_PAPACC_1_SIZE||
    memcmp(security->composition_inputs.secure_principal_alpn,PAPACC_SECURITY_ALPN_PAPACC_1,PAPACC_SECURITY_ALPN_PAPACC_1_SIZE)||
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
 out->issuer_present=pin->issuer_present?1U:0U;
 out->issuer_revision=pin->issuer_present?pin->issuer_revision:0U;out->resolved=1U;
 if(diag)diag->reason="resolved value snapshot; publication and operational authority remain denied";
 return PC_OK;
}

#ifdef PAPACC_OFFLINE_D3_WORKER
PC_RESULT papacc_server_config_initial_bootstrap_resolve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,
 const PC_DOCUMENT *doc,PAPACC_BOOTSTRAP_RESOLUTION *out,PC_DIAGNOSTIC *diag)
{
 PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_CONFIGURATION_COMMIT *c;
 const PAPACC_SERVER_CONFIG *models[1];PAPACC_ROOT_POLICY_REPORT report;PC_RESULT result;
 if(!out)return PC_INVALID;memset(out,0,sizeof(*out));
 if(!o||GetCurrentThreadId()!=o->owner_thread||!o->invocation)return deny(&out->binding,diag,"initial owner/invocation absent");
 c=&o->configuration_commit;t=&o->configuration_preparation;
 if(c->published||c->committed_bytes||c->committed_pending||c->recovery_selected||c->recovery_guard||
    t->administrative_published||t->normative_committed||t->invocation!=o->invocation||
    o->network.is_active||papacc_server_console_win32_stop_requested()||
    papacc_configuration_bootstrap_custody_ready(c)!=PAPACC_TRUE)
  return deny(&out->binding,diag,"initial mode requires native administrative custody; update/recovery forbidden");
 if(!pc_view(doc)||doc->listener_count!=1U||t->state!=PAPACC_ROOT_PREPARATION_PARTIAL||
    papacc_server_security_source_validate(&t->source)!=PAPACC_RESULT_OK||
    papacc_server_launcher_configuration_policy_report(o,&report)!=PAPACC_RESULT_OK||
    !report.prepared||!report.model_source||report.retained)
  return deny(&out->binding,diag,"initial governed participants/policy/source unavailable");
 if(t->model.config.bind_selection.mode!=PAPACC_BIND_SELECTION_ALL_INTERFACES)
  return deny(&out->binding,diag,"SELECTED topology unqualified");
 if(!t->source.reference.valid||t->source.reference.length!=t->model.config.security_configuration_ref_length||
    memcmp(t->source.reference.value,t->model.config.security_configuration_ref,t->source.reference.length))
  return deny(&out->binding,diag,"initial security reference mismatch");
 if(t->combined_reserved>2097152U||sizeof(*out)+2048U>2097152U-t->combined_reserved)
  return deny(&out->binding,diag,"initial resolver reservation insufficient");
 models[0]=&t->model.config;result=pc_bind_existing_models(doc,models,1U,&out->binding,diag);
 if(result!=PC_OK)return result;
 if(papacc_configuration_bootstrap_custody_ready(c)!=PAPACC_TRUE||papacc_server_console_win32_stop_requested())
  return deny(&out->binding,diag,"Stop or custody invalidated during initial resolution");
 /* Do not set the CURRENT-oriented binding.resolved/revision/epochs fields. */
 memcpy(out->binding.root,c->domain,32U);out->invocation=o->invocation;
 out->initial_resolved=1U;out->epochs_present=0U;
 if(diag)diag->reason="initial references resolved; no CURRENT, epoch, grants or commit";
 return PC_OK;
}
#endif
