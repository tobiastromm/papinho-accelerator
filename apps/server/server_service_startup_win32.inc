#include "server_service_startup_win32.h"
#include "personal_scq_inputs_win32.h"
/* F0/F4/F6 are local live acts; LAB preparation is read-only; a post-RUNNING
 * observation cannot authorize its own startup. SSO-B is accepted, but no original operational decision is registered
 * or independently delivered to this pre-callback SCM owner. Do not
 * manufacture one from metadata/flags or add an approval loader/registry. */
DWORD papacc_server_service_startup_source_status(PAPACC_SERVICE_STARTUP_SOURCE_REPORT *out)
{
 PAPACC_SERVICE_STARTUP_SOURCE_REPORT r;
 if(!out)return ERROR_INVALID_PARAMETER;
 memset(&r,0,sizeof(r));
 r.first_reason=(DWORD)InterlockedCompareExchange(&host.startup_first_source,0,0);
 r.missing_source_fields=PS_STARTUP_NEEDS_ORIGINAL|PS_STARTUP_NEEDS_INSTALLATION|
 PS_STARTUP_NEEDS_SCOPE|PS_STARTUP_NEEDS_LIFETIME|PS_STARTUP_NEEDS_READINESS_EVIDENCE;
 /* Availability of stored observations is explicitly NOT freshness, original
  * provenance, custody, readiness or approval. Never dereference these values
  * here to adopt/reopen an object or perform an effect while source is absent. */
 if(native.image&&native.image_ready)r.available_metadata|=PS_STARTUP_IMAGE_METADATA;
 if(native.service&&native.readback)r.available_metadata|=PS_STARTUP_SCM_METADATA;
 if(host.handle)r.available_metadata|=PS_STARTUP_HOST_METADATA;
 if(host.life.phase==PS_RUNNING)r.available_metadata|=PS_STARTUP_RUNNING_METADATA;
 if(native.retained||host.life.phase==PS_RETAINED||host.terminal_receipt==2U)r.reason=PS_STARTUP_RETAINED_UNKNOWN;
 else if((host.thread&&host.thread!=GetCurrentThreadId())||(native.thread&&native.thread!=GetCurrentThreadId()))r.reason=PS_STARTUP_WRONG_THREAD;
 else if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested())r.reason=PS_STARTUP_STOPPED;
#ifdef PAPACC_SCM_REGISTRATION_LAB
 else r.reason=PS_STARTUP_WRONG_LAB_SCOPE;
#else
 else if(host.run_active||host.life.phase==PS_RUNNING)r.reason=PS_STARTUP_POST_RUNNING_CIRCULAR;
 else r.reason=PS_STARTUP_ORIGINAL_UNAVAILABLE;
#endif
 *out=r;return ERROR_ACCESS_DENIED;
}
DWORD papacc_server_service_diagnostic_start_status(PAPACC_SERVICE_DIAGNOSTIC_START_REPORT *out)
{
 PAPACC_SERVICE_STARTUP_SOURCE_REPORT source;PAPACC_SERVICE_DIAGNOSTIC_START_REPORT r;
 if(!out)return ERROR_INVALID_PARAMETER;
 memset(&r,0,sizeof(r));
 (void)papacc_server_service_startup_source_status(&source);
 r.reason=source.reason;r.available_metadata=source.available_metadata;
 /* Exact controlled storage only, NOT native physical/commit provision.
  * Carrier is embedded in this storage: the component sizes below must not
  * be added again. SCM process/loader/RPC, CRT/provider/kernel transients and
  * retained work have no demonstrated native bound in the accepted SCQ plan. */
 r.scm_compiled_storage_bytes=sizeof(native)+sizeof(host);
 r.carrier_binary_bytes=PS_SCQ_CARRIER_BYTES;r.carrier_text_characters=PS_SCQ_CARRIER_CHARS;
 r.scq_input_storage_bytes=sizeof(PAPACC_PERSONAL_SCQ_INPUTS);
 r.scq_image_chain_storage_bytes=sizeof(PAPACC_SCQ_IMAGE_CHAIN);
 r.controller_plan_storage_bytes=sizeof(PAPACC_SCQ_PLAN_DATA);
 r.initial_receiver_source_unavailable=1U; /* default DENY; original predicates below must prove eligibility */
 r.scq_image_chain_handle_slots=PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS+1U;
 r.resource_pre_missing=PS_START_RESOURCE_PROCESS_ENVELOPE|PS_START_RESOURCE_RPC_TRANSIENT|
  PS_START_RESOURCE_HOST_MARGIN|PS_START_RESOURCE_CLOCK_POLICY|
  PS_START_RESOURCE_RETENTION_ENVELOPE|PS_START_RESOURCE_PARTICIPANT_HANDLES;
 r.resource_effect_obligations=PS_START_RESOURCE_EFFECT_RUNTIME|PS_START_RESOURCE_EFFECT_ENDPOINT|
  PS_START_RESOURCE_EFFECT_STOP_CLOSE;
 /* No positive producer is installed here. In particular, SCQA's local
  * startup borrow is neither lifecycle permission nor cross-process delivery.
  * R0-R9 resource acceptance applies to registration, not this Start. */
 r.missing_pre_start=PS_DIAGNOSTIC_START_NEEDS_LIFECYCLE_ORIGINAL|
 PS_DIAGNOSTIC_START_NEEDS_CONTROLLER_ORIGINAL|
 PS_DIAGNOSTIC_START_NEEDS_DELIVERY_CONTINUATION|
 PS_DIAGNOSTIC_START_NEEDS_RESOURCE_PLAN;
 r.later_obligations=PS_DIAGNOSTIC_START_FRESH_RUNTIME_IDENTITY|
 PS_DIAGNOSTIC_START_FRESH_BILATERAL|PS_DIAGNOSTIC_START_FRESH_CLOSED_READINESS|
 PS_DIAGNOSTIC_START_FRESH_ORIGINAL_CLOSE;
#ifndef PAPACC_SCM_REGISTRATION_LAB
 {PS_SCQ_CARRIER original;
  /* These queries inspect the existing original actor, not the received DATA
   * carrier. Independent native capacity is still UNKNOWN: never clear it
   * by possession of a carrier, namespace or a modeled memory census. */
  r.original_checked=1U;r.original_error=papacc_server_launcher_scq_start_carrier(&original);
  if(!r.original_error){
   r.missing_pre_start&=~(PS_DIAGNOSTIC_START_NEEDS_LIFECYCLE_ORIGINAL|PS_DIAGNOSTIC_START_NEEDS_CONTROLLER_ORIGINAL);
  }
 }
#endif
#ifndef PAPACC_SCM_REGISTRATION_LAB
 if(!(r.missing_pre_start&(PS_DIAGNOSTIC_START_NEEDS_LIFECYCLE_ORIGINAL|PS_DIAGNOSTIC_START_NEEDS_CONTROLLER_ORIGINAL))){
  r.resource_checked=1U;r.resource_error=papacc_server_launcher_scq_start_resource_current();
  if(!r.resource_error){
  /* Explicit owner-approved binding9637246a…: original controller acquisition,
   * three attributed acts and native selected G3 are required by these native
   * getters. This recognizes compiled QUERY/SQB2 continuation, not a future
   * authenticated runtime. Never clear missing native evidence below. */
  r.initial_receiver_source_unavailable=0U;
  if(!papacc_server_launcher_scq_start_namespace_current())
   r.missing_pre_start&=~PS_DIAGNOSTIC_START_NEEDS_DELIVERY_CONTINUATION;
  r.limited_assurance=1U;r.limited_eligible=1U;
  /* Resource UNKNOWN stays visible. G1 allows only limited eligibility,
   * not a rewritten proof of integral native provision. */
  *out=r;return (r.missing_pre_start&~PS_DIAGNOSTIC_START_NEEDS_RESOURCE_PLAN)?ERROR_ACCESS_DENIED:ERROR_SUCCESS;
  }
 }
#endif
 *out=r;return ERROR_ACCESS_DENIED;
}
#ifndef PAPACC_SCM_REGISTRATION_LAB
static void ps_registration_view_bind(void)
{
 PAPACC_SERVICE_REGISTRATION_VIEW *v=&native.registration_view;
 memset(v,0,sizeof(*v));v->recipient=&native;v->image.handle=native.image;v->image.identity=native.image_id;
 v->image.path=native.image_path;v->image.sha256=native.image_sha256;v->image.owner_thread=native.thread;
 v->command=native.command;v->service=PAPACC_SCM_NAME_LITERAL;v->account=PAPACC_SCM_ACCOUNT_LITERAL;
 v->start_type=PAPACC_SCM_START_MODE;v->sid_type=SERVICE_SID_TYPE_UNRESTRICTED;
}
#endif
DWORD papacc_server_service_registration_source_status(PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT *out)
{
 PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT r;
 if(!out)return ERROR_INVALID_PARAMETER;
 memset(&r,0,sizeof(r));r.selected_profile=native.install_profile;
 r.resource_missing=PAPACC_SCM_RESOURCE_MISSING_ALL;
 r.resource_pre_missing=PAPACC_SCM_RESOURCE_PRECONDITIONS;r.resource_effect_missing=PAPACC_SCM_RESOURCE_EFFECT_EVIDENCE;
 r.missing_source_fields=PS_REGISTRATION_NEEDS_ORIGINAL|PS_REGISTRATION_NEEDS_TARGET_IMAGE|
 PS_REGISTRATION_NEEDS_EXACT_SCOPE|PS_REGISTRATION_NEEDS_LIVE_ACT|PS_REGISTRATION_NEEDS_RESOURCES;
 r.image_metadata=native.image&&native.image_ready?1U:0U;
 if(native.retained)r.reason=PS_REGISTRATION_RETAINED_UNKNOWN;
 else if(native.thread&&native.thread!=GetCurrentThreadId())r.reason=PS_REGISTRATION_WRONG_THREAD;
 else if(papacc_server_console_win32_stop_requested())r.reason=PS_REGISTRATION_STOPPED;
 else if(native.install_effect)r.reason=PS_REGISTRATION_CONSUMED;
 else if(native.install_profile!=PS_INSTALL_PRODUCT&&native.install_profile!=PS_INSTALL_SCQ_CLOSED)r.reason=PS_REGISTRATION_INVALID_PROFILE;
#ifdef PAPACC_SCM_REGISTRATION_LAB
 else if(native.install_profile==PS_INSTALL_SCQ_CLOSED)r.reason=PS_REGISTRATION_WRONG_LAB_SCOPE;
#endif
 else if(native.install_profile==PS_INSTALL_SCQ_CLOSED&&native.registration_active&&
 papacc_server_launcher_scm_registration_current(&native.registration_view)==ERROR_SUCCESS){
  PAPACC_SCM_RESOURCE_REPORT observation;
  r.missing_source_fields=PS_REGISTRATION_NEEDS_RESOURCES;r.reason=PS_REGISTRATION_RESOURCES_UNQUALIFIED;
  if(papacc_server_launcher_scm_registration_resource_report(&native.registration_view,&observation)==ERROR_SUCCESS){
   r.resource_observations=observation.samples;r.resource_error=observation.first_error;r.resource_missing=observation.missing;
   r.resource_pre_missing=observation.pre_missing;r.resource_effect_missing=observation.effect_missing;
   if(r.resource_error)r.reason=PS_REGISTRATION_RESOURCE_OBSERVATION_FAILED;
   else if(observation.assurance==PAPACC_SCM_ASSURANCE_LIMITED_R0_R9&&observation.limited_eligible){
    /* Accepted R0-R9 limited risk, NOT proof that opaque missing costs passed.
     * Original current() above and fresh per-effect gates remain mandatory. */
    r.reason=PS_REGISTRATION_LIMITED_ELIGIBLE;r.missing_source_fields=0;*out=r;return ERROR_SUCCESS;
   }
  }
 }else r.reason=PS_REGISTRATION_ORIGINAL_UNAVAILABLE;
 /* Confirmed original provenance does not prove native quantitative capacity.
  * No installed process/RUNNING prerequisite, no permissive resource flag. */
 *out=r;return ERROR_ACCESS_DENIED;
}
static int ps_registration_binding_ready(void)
{
 PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT r;
 return papacc_server_service_registration_source_status(&r)==ERROR_SUCCESS;
}
static int ps_binding_ready(void)
{
 PAPACC_SERVICE_STARTUP_SOURCE_REPORT r;DWORD e;
 e=papacc_server_service_startup_source_status(&r);
 if(e){(void)InterlockedCompareExchange(&host.startup_first_source,(LONG)r.reason,0);return 0;}
 /* There is deliberately no positive original source built from metadata.
  * The accepted SSO-B contract still requires the original delivered producer,
  * and independently separate lifecycle effects before returning success. */
 return 1;
}
