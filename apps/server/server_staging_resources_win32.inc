/* Necessary observations plus independent D0-D7 admission. Legacy numeric diagnostic stops remain conservative until a complete policy is qualified. */
#include "server_personal_resource_sample_win32.inc"
typedef NTSTATUS (NTAPI *STAGING_QUERY_VOLUME)(HANDLE,PIO_STATUS_BLOCK,PVOID,ULONG,ULONG);
static DWORD staging_resource_fail(DWORD error)
{
 PAPACC_STAGING_RESOURCES *r=&staging_act()->resources;
 if(!error)error=ERROR_INVALID_DATA;if(!r->first)r->first=error;return staging_fail(error);
}
static DWORD staging_observe(DWORD phase)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();DWORD error;FILE *sink;
 if(!staging_owned()||s->resources.first)return ERROR_INVALID_STATE;
 sink=s->resources.observation.sink;if(s->selected_plan&&phase==10U)s->resources.observation.sink=NULL;
 error=s->selected_plan?personal_resource_collect(&s->resources.observation,phase,0):personal_resource_sample(&s->resources.observation,phase);
 s->resources.observation.sink=sink;
 if(error)return staging_resource_fail(error);
 if(s->selected_plan&&(s->resources.observation.last.physical_available<6296461312ULL||s->resources.observation.last.commit_available<8479985664ULL))return staging_resource_fail(ERROR_NOT_ENOUGH_MEMORY);
 s->resources.missing&=~PAPACC_STAGE_MISSING_OBSERVATION;return 0;
}
/* D0-D7: independent staging policy, not a LAB/TPM/SCQ quota alias.
 * Missing native policy sources cannot be supplied by caller-owned DATA.
 * In particular clearing the diagnostic mask never produces admission. */
static DWORD staging_policy_missing(void)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();PAPACC_STAGING_RESOURCES *r=&s->resources;
 DWORD missing=PAPACC_STAGE_MISSING_NATIVE_ENVELOPE|PAPACC_STAGE_MISSING_HOST_MARGIN|PAPACC_STAGE_MISSING_FS_METADATA;
 /* Single owner/invocation, no SCQ peer or future service. Controller totals
  * remain charged through native envelope/host margin, never a 1024 credit.
  * Retained native/FS costs remain in those material classes, not assumed zero. */
 if(!staging_owned())missing|=PAPACC_STAGE_MISSING_ADMIN_COHORT;
 if(s->phase==8U||root.configuration_commit.unresolved||state==PAPACC_LAUNCHER_RETAINED)
  missing|=PAPACC_STAGE_MISSING_RETENTION;
 if(!r->observation.samples||r->observation.first_error||!r->observation.last.physical_available||
 !r->observation.last.commit_available)missing|=PAPACC_STAGE_MISSING_OBSERVATION;
 if(!r->volume_known||!r->unit_bytes||r->caller_free<r->payload_allocated)missing|=PAPACC_STAGE_MISSING_VOLUME;
 return missing;
}
/* B0-B8 records a reduced guarantee, NOT a positive capacity source. */
static DWORD staging_policy_blockers(void)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();DWORD missing=staging_policy_missing();
 if(s->assurance_profile==PAPACC_STAGING_LIMITED_QUALIFICATION){
  /* Unknown opaque costs remain in missing/report. Only their integral
   * pre-proof is specialized. B3 plan/margins/authority are still absent. */
  missing&=~(PAPACC_STAGE_MISSING_NATIVE_ENVELOPE|PAPACC_STAGE_MISSING_FS_METADATA);
  if(!s->selected_plan||!s->resources.ready_frozen)missing|=PAPACC_STAGE_MISSING_EXACT_LIMITED_PLAN;
  else missing&=~PAPACC_STAGE_MISSING_HOST_MARGIN; /* checked by original selected criteria, not absent opaque proof */
 }
 return missing;
}
/* Pure necessary-predicate evaluator: all thresholds are immutable plan DATA,
 * never normative defaults. Does not acquire objects or authorize effects. */
static DWORD staging_limited_criteria_check(const PAPACC_STAGING_CRITERIA_DATA *p,const PAPACC_STAGING_RESOURCES *r,ULONGLONG now)
{
 const PAPACC_SCM_RESOURCE_SAMPLE *last;ULONGLONG elapsed,fresh,frequency,oldest;
 if(!p||!r)return ERROR_INVALID_PARAMETER;
 if(!p->max_handles||!p->max_private||!p->max_working||!p->min_physical||!p->min_commit||
 !p->min_caller_free||!p->min_actual_free||!p->max_elapsed_ms||!p->max_freshness_ms)return ERROR_NOT_SUPPORTED;
 if(p->max_freshness_ms>p->max_elapsed_ms)return ERROR_INVALID_DATA;
 if(r->first||r->observation.first_error||!r->observation.samples||!r->volume_known||!r->unit_bytes)return ERROR_INVALID_STATE;
 last=&r->observation.last;frequency=last->frequency;
 if((r->volume_begin&&(r->volume_end<r->volume_begin||r->volume_end>now))||!frequency||frequency!=r->observation.first.frequency||last->end<last->begin||
 last->begin<r->observation.first.begin||now<last->end)return ERROR_INVALID_DATA;
 if(p->max_elapsed_ms>((ULONGLONG)-1)/frequency||p->max_freshness_ms>((ULONGLONG)-1)/frequency)return ERROR_ARITHMETIC_OVERFLOW;
 elapsed=p->max_elapsed_ms*frequency/1000U;fresh=p->max_freshness_ms*frequency/1000U;
 oldest=last->begin;if(r->volume_begin&&r->volume_begin<oldest)oldest=r->volume_begin;
 if(r->validation_begin&&r->validation_begin<oldest)oldest=r->validation_begin;
 if(now-r->observation.first.begin>elapsed||now-oldest>fresh)return ERROR_TIMEOUT;
 if(last->handles>p->max_handles||last->private_bytes>p->max_private||last->working_set>p->max_working)return ERROR_NOT_ENOUGH_QUOTA;
 if(last->physical_available<p->min_physical||last->commit_available<p->min_commit)return ERROR_NOT_ENOUGH_MEMORY;
 if(p->min_caller_free<r->payload_allocated||p->min_actual_free<r->payload_allocated)return ERROR_INVALID_DATA;
 if(r->caller_free<p->min_caller_free||r->actual_free<p->min_actual_free)return ERROR_DISK_FULL;
 return 0; /* DATA fit, not admission; B3 original source remains mandatory */
}
static DWORD staging_limited_refs(void);
static DWORD staging_limited_refresh(void);
static DWORD staging_admit(void)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();
 if(!staging_owned()||s->phase!=3U||s->consumed||
 (s->assurance_profile!=PAPACC_STAGING_STRICT&&s->assurance_profile!=PAPACC_STAGING_LIMITED_QUALIFICATION))return ERROR_INVALID_STATE;
 if(papacc_server_console_win32_stop_requested()||root.configuration_commit.pending_commit.stop)return ERROR_CANCELLED;
 if(s->first)return s->first;if(s->resources.first)return s->resources.first;
 if(s->selected_plan){DWORD e=staging_limited_refresh();if(e)return e;}
 if(s->assurance_profile==PAPACC_STAGING_LIMITED_QUALIFICATION){
  LARGE_INTEGER now;DWORD data_error;
  if(!QueryPerformanceCounter(&now))return GetLastError()?GetLastError():ERROR_GEN_FAILURE;
  if(now.QuadPart<0)return ERROR_INVALID_DATA;
  data_error=staging_limited_criteria_check(&s->resources.criteria,&s->resources,(ULONGLONG)now.QuadPart);
  if(data_error)return data_error;
 }
 s->resources.missing=staging_policy_missing();
 /* No qualified source currently exists for native/host/FS sufficiency.
  * A future integration must prove them; neither handles=80 nor 132 is a grant. */
 if(staging_policy_blockers())return ERROR_NOT_SUPPORTED;
 return s->selected_plan&&s->resources.ready_frozen?0:ERROR_ACCESS_DENIED;
}
static DWORD staging_volume(void)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();PAPACC_STAGING_RESOURCES *r=&s->resources;
 STAGING_QUERY_VOLUME query;FARPROC proc;HMODULE module;NTSTATUS status;ULONGLONG unit,total,rounded,blocks,bytes;unsigned i;LARGE_INTEGER began,ended;
 if(!staging_owned()||!s->ancestor_count||(r->volume_attempted&&(!s->selected_plan||!r->volume_known||r->volume_io.Status!=0))||r->first)return staging_resource_fail(ERROR_INVALID_STATE);
 r->volume_attempted=1U;
 module=GetModuleHandleW(L"ntdll.dll");if(!module)return staging_resource_fail(ERROR_NOT_SUPPORTED);
 proc=GetProcAddress(module,"NtQueryVolumeInformationFile");if(!proc)return staging_resource_fail(ERROR_NOT_SUPPORTED);memcpy(&query,&proc,sizeof(query));
 if(!QueryPerformanceCounter(&began)||began.QuadPart<0)return staging_resource_fail(ERROR_INVALID_DATA);
 status=query(s->objects[s->ancestor_count-1U].handle,&r->volume_io,&r->volume,sizeof(r->volume),7U);
 if(status!=0){
  DWORD error=staging_resource_fail((DWORD)status);
  if(status>0){s->phase=8U;root.configuration_commit.unresolved=1;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);}return error;
 }
 if(r->volume_io.Status!=0){DWORD error=staging_resource_fail(ERROR_INVALID_DATA);s->phase=8U;root.configuration_commit.unresolved=1;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return error;}
 if(r->volume_io.Information!=sizeof(r->volume)||r->volume.total.QuadPart<0||
 r->volume.caller_available.QuadPart<0||r->volume.actual_available.QuadPart<0||
 r->volume.caller_available.QuadPart>r->volume.total.QuadPart||r->volume.caller_available.QuadPart>r->volume.actual_available.QuadPart||
 !r->volume.sectors_per_unit||!r->volume.bytes_per_sector)return staging_resource_fail(ERROR_INVALID_DATA);
 unit=(ULONGLONG)r->volume.sectors_per_unit*r->volume.bytes_per_sector;
 total=(ULONGLONG)r->volume.caller_available.QuadPart;
 if(total>((ULONGLONG)-1)/unit||(ULONGLONG)r->volume.actual_available.QuadPart>((ULONGLONG)-1)/unit)return staging_resource_fail(ERROR_ARITHMETIC_OVERFLOW);
 r->unit_bytes=unit;r->caller_free=total*unit;r->actual_free=(ULONGLONG)r->volume.actual_available.QuadPart*unit;
 total=rounded=0;
 for(i=0;i<3U;i++){
  BY_HANDLE_FILE_INFORMATION *f=&s->objects[8U+i].identity;
  bytes=((ULONGLONG)f->nFileSizeHigh<<32)|f->nFileSizeLow;
  if(total>((ULONGLONG)-1)-bytes)return staging_resource_fail(ERROR_ARITHMETIC_OVERFLOW);total+=bytes;
  blocks=bytes/unit+(bytes%unit?1U:0U);
  if(blocks>((ULONGLONG)-1)/unit||rounded>((ULONGLONG)-1)-blocks*unit)return staging_resource_fail(ERROR_ARITHMETIC_OVERFLOW);rounded+=blocks*unit;
 }
 if(!QueryPerformanceCounter(&ended)||ended.QuadPart<began.QuadPart)return staging_resource_fail(ERROR_INVALID_DATA);
 r->volume_begin=(ULONGLONG)began.QuadPart;r->volume_end=(ULONGLONG)ended.QuadPart;
 if(r->ready_frozen&&(unit!=r->frozen_unit||rounded!=r->frozen_payload))return staging_resource_fail(ERROR_INVALID_DATA);
 r->logical_bytes=total;r->payload_allocated=rounded;r->volume_known=1U;r->missing&=~PAPACC_STAGE_MISSING_VOLUME;
 /* Full payload cluster rounding is an uncompressed allocation planning
  * quantity, not a universal floor/ceiling, metadata bound or reservation.
  * Reject below it; never accept above it as sufficient capacity. */
 if(r->caller_free<rounded)return staging_resource_fail(ERROR_DISK_FULL);
 return 0;
}
static DWORD staging_limited_freeze(void)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();PAPACC_STAGING_RESOURCES *r=&s->resources;PAPACC_SCM_RESOURCE_SAMPLE *v=&r->observation.last;
 if(!staging_owned()||!s->selected_plan||s->assurance_profile!=PAPACC_STAGING_LIMITED_QUALIFICATION||r->ready_frozen||!r->volume_known||!r->observation.samples)return ERROR_INVALID_STATE;
 if(v->handles>MAXDWORD-5U||v->private_bytes>((ULONGLONG)-1)-9094144ULL||v->working_set>((ULONGLONG)-1)-9094144ULL||r->payload_allocated>((ULONGLONG)-1)/2U)return ERROR_ARITHMETIC_OVERFLOW;
 r->criteria.max_handles=v->handles+5U;r->criteria.max_private=v->private_bytes+9094144ULL;r->criteria.max_working=v->working_set+9094144ULL;
 r->criteria.min_physical=6296461312ULL;r->criteria.min_commit=8479985664ULL;
 r->criteria.min_caller_free=r->criteria.min_actual_free=r->payload_allocated;
 r->criteria.max_elapsed_ms=120000U;r->criteria.max_freshness_ms=500U;
 r->frozen_unit=r->unit_bytes;r->frozen_payload=r->payload_allocated;r->ready_frozen=1U;return 0;
}
static DWORD staging_limited_refresh(void)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();PAPACC_STAGING_RESOURCES *r=&s->resources;LARGE_INTEGER now;DWORD e;
 if(!staging_owned()||!s->selected_plan||!r->ready_frozen||s->phase<2U||s->phase>4U||s->first)return ERROR_INVALID_STATE;
 if(papacc_server_console_win32_stop_requested()||root.configuration_commit.pending_commit.stop)return staging_fail(ERROR_CANCELLED);
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0)return staging_resource_fail(ERROR_INVALID_DATA);
 r->validation_begin=(ULONGLONG)now.QuadPart;
 if((e=staging_limited_refs())!=0)return e;
 if((e=staging_volume())!=0)return e;
 if((e=staging_observe(10U))!=0)return e;
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0)return staging_resource_fail(ERROR_INVALID_DATA);
 e=staging_limited_criteria_check(&r->criteria,r,(ULONGLONG)now.QuadPart);
 if(e)return staging_resource_fail(e);
 if(!s->effects&&(r->caller_free<2U*r->frozen_payload||r->actual_free<2U*r->frozen_payload))return staging_resource_fail(ERROR_DISK_FULL);
 return 0;
}
static void staging_resource_report(FILE *out)
{
 PAPACC_QUALIFICATION_STAGING *s=staging_act();PAPACC_STAGING_RESOURCES *r=&s->resources;unsigned i,live=0;
 for(i=0;i<16U;i++)if(s->objects[i].handle)live++;
 fprintf(out,"STAGING_RESOURCES samples=%lu first=%lu missing=%lu peak_handles=%lu private_peak=%llu minimum_physical=%llu minimum_commit=%llu volume_known=%u cluster=%llu caller_free=%llu actual_free=%llu logical=%llu rounded_payload=%llu controlled_stage_refs=%u phase=%u consumed=%u effects=%u; APPLY_SCOPE=QUALIFICATION_ONLY_OR_DENIED; FS_METADATA=UNKNOWN; OBSERVED_PEAK_NOT_CEILING\n",
 (unsigned long)r->observation.samples,(unsigned long)(s->first?s->first:r->first),(unsigned long)r->missing,
 (unsigned long)r->observation.peak_handles,r->observation.peak_private,r->observation.min_physical,r->observation.min_commit,
 r->volume_known,r->unit_bytes,r->caller_free,r->actual_free,r->logical_bytes,r->payload_allocated,live,s->phase,s->consumed,s->effects);
 fprintf(out,"STAGING_DESCRIPTOR_PREDICATE reason=%lu ace_index=%lu access_mask=%lu; ZERO_MEANS_NOT_REJECTED_OR_NOT_REACHED; NO_RAW_SD_OR_SECRET; POLICY_UNCHANGED\n",(unsigned long)r->descriptor_reason,(unsigned long)r->descriptor_ace,(unsigned long)r->descriptor_mask);
 fprintf(out,"STAGING_DESCRIPTOR_BYTES parent=%lu new_protection=%lu scratch_peak_observed=%lu; NATIVE_ALLOCATOR_OVERHEAD=UNKNOWN; ALLOCATION_BEFORE_LENGTH_CHECK_NOT_BOUNDED_BY_VALIDATOR\n",
 (unsigned long)r->parent_descriptor_bytes,(unsigned long)r->protection_bytes,(unsigned long)r->scratch_peak_bytes);
 fprintf(out,"STAGING_CONTROLLED_CLOSE known=%u retained=%u; OS_INTERNAL_QUIESCENCE=UNKNOWN\n",r->close_known,s->phase==8U?1U:0U);
 fprintf(out,"STAGING_NATIVE_CONTEXT first_predicate=%lu native_error=%lu failure_kind=%lu; TOKEN_NOT_OPERATIONAL_AUTHORIZATION\n",
 (unsigned long)root.configuration_commit.pending_commit.personal_native.diagnostic_first,
 (unsigned long)root.configuration_commit.pending_commit.personal_native.diagnostic_error,
 (unsigned long)root.configuration_commit.pending_commit.personal_native.diagnostic_kind);
 fprintf(out,"STAGING_FIRST_LOCATION=%lu; CURRENT_LOCATION=%lu; CAPTURE_DIAGNOSTIC_NOT_AUTHORITY\n",(unsigned long)r->first_location,(unsigned long)r->location);
 if(!s->selected_plan)fprintf(out,"STAGING_HANDLE_GUARD query_attempted=%lu query_succeeded=%lu count_known=%lu own_count=%lu query_error=%lu reserve_guard=124 own_cap=128 required_controlled_refs=1 guard_denied=%lu object_index=%lu; ORIGINAL_PRE_OPEN_QUERY_NOT_A_NEW_QUERY; UNKNOWN_NOT_ZERO\n",
 (unsigned long)r->guard_attempted,(unsigned long)r->guard_succeeded,(unsigned long)r->guard_succeeded,(unsigned long)r->guard_count,
 (unsigned long)r->guard_error,(unsigned long)r->guard_denied,(unsigned long)r->guard_index);
 if(s->selected_plan)fprintf(out,"STAGING_SELECTED_LIMITS ready_frozen=%u handles=%lu private=%llu working=%llu physical_floor=%llu commit_floor=%llu R=%llu freshness_ms=%llu duration_ms=%llu; UNKNOWN_OPAQUE_NOT_ZERO; NO_PRODUCTIVE_PROMOTION\n",r->ready_frozen,(unsigned long)r->criteria.max_handles,r->criteria.max_private,r->criteria.max_working,r->criteria.min_physical,r->criteria.min_commit,r->frozen_payload,r->criteria.max_freshness_ms,r->criteria.max_elapsed_ms);
 fprintf(out,"STAGING_POST_ENTRY_HANDLES query_attempted=%lu query_succeeded=%lu count_known=%lu own_count=%lu query_error=%lu; SCOPE=AFTER_FIRST_FRAME_BEFORE_LAYOUT; NO_ATTRIBUTION_TO_LATER_PHASES\n",
 (unsigned long)r->post_entry_attempted,(unsigned long)r->post_entry_succeeded,(unsigned long)r->post_entry_succeeded,
 (unsigned long)r->post_entry_count,(unsigned long)r->post_entry_error);
}











