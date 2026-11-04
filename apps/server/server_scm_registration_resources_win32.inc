#include "server_personal_resource_sample_win32.inc"
static DWORD scm_register_observe(PAPACC_SCM_REGISTRATION_ACT *a,DWORD phase)
{
 if(a->self!=a||a->thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 /* R3 applies ONLY to this explicit act. Shared SCQ/LAB/TPM callers retain
  * personal_resource_sample and its original guard. No inferred H_READY+2. */
 if(a->assurance==PAPACC_SCM_ASSURANCE_LIMITED_R0_R9)
  return personal_resource_collect(&a->resources,phase,0);
 return personal_resource_sample(&a->resources,phase);
}
static int scm_register_limited_resources(PAPACC_SCM_REGISTRATION_ACT *a)
{
 PAPACC_SCM_RESOURCE_OBSERVATION *o=&a->resources;
 return a->assurance==PAPACC_SCM_ASSURANCE_LIMITED_R0_R9&&
  root.reserved_bytes&&root.reserved_bytes<=PAPACC_LAUNCHER_MANAGED_LIMIT&&
  !o->first_error&&o->samples&&o->last.physical_available>=PAPACC_LAUNCHER_MANAGED_LIMIT&&
  o->last.commit_available>=PAPACC_LAUNCHER_MANAGED_LIMIT;
}
static DWORD scm_register_effect_begin(PAPACC_SCM_REGISTRATION_ACT *a,unsigned effect)
{
 LARGE_INTEGER now,frequency;DWORD e;
 if(effect>1U||a->effect_state[effect]||
  (effect&&(!a->consumed||a->effect_state[0]!=2U||a->effect_result[0])))return ERROR_INVALID_STATE;
 e=scm_register_observe(a,effect?PAPACC_SCM_RESOURCE_PRE_CHANGE:PAPACC_SCM_RESOURCE_PRE_CREATE);if(e)return e;
 if(!scm_register_limited_resources(a))return ERROR_NOT_ENOUGH_MEMORY;
 if(!QueryPerformanceFrequency(&frequency)||!QueryPerformanceCounter(&now)||
  frequency.QuadPart<=0||(ULONGLONG)frequency.QuadPart!=a->resources.last.frequency||
  now.QuadPart<0||(ULONGLONG)now.QuadPart<a->resources.last.end)return ERROR_INVALID_DATA;
 if(papacc_server_console_win32_stop_requested()||root.configuration_commit.pending_commit.stop)return ERROR_CANCELLED;
 a->effect_begin[effect]=(ULONGLONG)now.QuadPart;a->effect_state[effect]=1U;
 return 0;
}
DWORD papacc_server_launcher_scm_registration_resource_report(const PAPACC_SERVICE_REGISTRATION_VIEW *v,PAPACC_SCM_RESOURCE_REPORT *out)
{
 PAPACC_SCM_REGISTRATION_ACT *a=&root.configuration_commit.pending_commit.personal_native.scm_registration;
 PAPACC_SCM_RESOURCE_OBSERVATION *o=&a->resources;
 if(!out)return ERROR_INVALID_PARAMETER;memset(out,0,sizeof(*out));
 out->missing=PAPACC_SCM_RESOURCE_MISSING_PRE_ENTRY|PAPACC_SCM_RESOURCE_MISSING_TRANSITIVE|
 PAPACC_SCM_RESOURCE_MISSING_HOST_POLICY|PAPACC_SCM_RESOURCE_MISSING_RETENTION_ENVELOPE|
 PAPACC_SCM_RESOURCE_MISSING_ADMIN_COHORT|PAPACC_SCM_RESOURCE_MISSING_SCM_ACTIVE|PAPACC_SCM_RESOURCE_MISSING_OBSERVATION;
 if(root.owner_thread!=GetCurrentThreadId()||a->self!=a||a->original!=v||a->invocation!=root.invocation)return ERROR_INVALID_STATE;
 out->samples=o->samples;out->first_error=o->first_error;out->peak_handles=o->peak_handles;
 out->peak_private=o->peak_private;out->minimum_physical=o->min_physical;out->minimum_commit=o->min_commit;
 out->frequency=o->first.frequency;out->begin=o->first.begin;out->end=o->last.end;
 if(o->samples&&!o->first_error)out->missing&=~PAPACC_SCM_RESOURCE_MISSING_OBSERVATION;
 out->pre_missing=out->missing&PAPACC_SCM_RESOURCE_PRECONDITIONS;
 out->effect_missing=out->missing&PAPACC_SCM_RESOURCE_EFFECT_EVIDENCE;
 out->assurance=a->assurance;out->controlled_bytes=root.reserved_bytes;out->host_floor=PAPACC_LAUNCHER_MANAGED_LIMIT;
 out->limited_eligible=a->phase==3U&&!a->consumed&&a->package_captured&&
  o->last.phase==PAPACC_SCM_RESOURCE_CONFIRMED&&scm_register_limited_resources(a);
 return 0; /* report generated, NOT admission or freshness renewal */
}
