#include "server_c8_target_evidence_win32.h"
#include "pal_time.h"
#include "papinho_secure_transport.h"
#include <windows.h>
#include <psapi.h>
#include <string.h>
#if PST_API_VERSION != 0x00030000UL || PST_LIBRARY_VERSION != 0x00000605UL
#error C8_target_requires_governed_PST065_API3
#endif
/* Canonical sorted 47-file normal SDK inventory; see evidence manifest.
 * SPI4 is verified from that governed package, not a fabricated runtime fact. */
static const PAPACC_U8 sdk_identity[32]={0xdd,0x9b,0xca,0x6d,0x0a,0x6b,0xc8,0x9d,0xcd,0xf9,0xf5,0x45,0xfc,0xca,0x76,0xa8,0x47,0x46,0xe2,0xa0,0x2e,0x96,0x33,0x7c,0x31,0x2b,0x9b,0xf5,0xb3,0xef,0xbb,0xe5};
static const PAPACC_U8 recipe_identity[32]={0x02,0xa7,0x09,0x4a,0x78,0xd1,0x25,0xf2,0xa3,0x5f,0xb6,0xf5,0x21,0x33,0xd3,0xd7,0x99,0x35,0xe8,0x92,0xbb,0xd7,0x6b,0x74,0xe4,0x1d,0x45,0xdc,0x8a,0x06,0x39,0x8b};
static void put32(PAPACC_U8 *p,PAPACC_U32 v){unsigned i;for(i=0U;i<4U;++i)p[i]=(PAPACC_U8)(v>>(8U*i));}
static void put64(PAPACC_U8 *p,PAPACC_U64 v){unsigned i;for(i=0U;i<8U;++i)p[i]=(PAPACC_U8)(v>>(8U*i));}
static int binding_equal(const PAPACC_C8_TARGET_BINDING *a,const PAPACC_C8_TARGET_BINDING *b)
{
 return a->invocation==b->invocation&&a->owner==b->owner&&a->revision==b->revision&&a->epoch==b->epoch&&
 a->managed==b->managed&&a->charged==b->charged&&!memcmp(a->domain,b->domain,32U)&&
 a->profile==b->profile&&a->phase==b->phase&&a->configured==b->configured&&a->current==b->current&&
 a->concurrency==b->concurrency&&a->retained==b->retained&&a->stopping==b->stopping;
}
static PAPACC_RESULT archive_series(PAPACC_C8_TARGET_PRODUCER *s)
{
 PAPACC_C8_TARGET_HISTORY *h;
 if(!s->status)return PAPACC_RESULT_OK;
 if(s->history_count>=PAPACC_C8_TARGET_HISTORY_CAPACITY){
  s->history_overflow=1U;s->status=0U;return PAPACC_RESULT_LIMIT_EXCEEDED;
 }
 h=&s->history[s->history_count++];memset(h,0,sizeof(*h));
 h->binding=s->binding;h->evidence=s->evidence;h->creation=s->creation;
 h->series_id=s->series_id;h->first_sequence=s->series_first_sequence;
 h->last_sequence=s->sequence;h->started_ms=s->series_started_ms;
 h->process=s->process;h->thread=s->thread;h->status=s->status;
 return PAPACC_RESULT_OK;
}
static PAPACC_RESULT reduce(PAPACC_C8_TARGET_PRODUCER *s,const PAPACC_C8_TARGET_BINDING *b,const PAPACC_C8_TARGET_OBSERVATION *o,PAPACC_U32 origin)
{
 PAPACC_C8_TARGET_EVIDENCE *e;PAPACC_U64 baseline,peak;PAPACC_U8 target[32];unsigned i;int same,valid;
 if(!s||!b||!o)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(s->history_overflow||s->history_count>PAPACC_C8_TARGET_HISTORY_CAPACITY||
    s->sequence==(PAPACC_U64)-1){s->status=0U;s->history_overflow=1U;return PAPACC_RESULT_LIMIT_EXCEEDED;}
 put32(target,o->process);put64(target+4U,o->creation);put32(target+12U,o->thread);
 put64(target+16U,b->invocation);put32(target+24U,(PAPACC_U32)sizeof(void*));put32(target+28U,1U);
 valid=o->success&&o->version_ok&&o->process&&o->thread&&o->creation&&
  (origin==1U||origin==2U)&&o->finished_ms>=o->started_ms&&
  o->finished_ms-o->started_ms<=100U&&o->started_ms<=(PAPACC_U64)-1-1000U;
 for(i=0U;i<5U;++i)if(o->errors[i])valid=0;
 /* Native and synthetic histories are NEVER interchangeable. A gap, target,
  * SDK/recipe, process/thread, configuration/phase/quota or origin change
  * closes this finite series; its immutable by-value history stays separate. */
 same=valid&&s->status==1U&&s->series_id&&s->evidence.origin==origin&&
  s->process==o->process&&s->thread==o->thread&&s->creation==o->creation&&
  binding_equal(&s->binding,b)&&!memcmp(s->evidence.target,target,32U)&&
  !memcmp(s->evidence.sdk_sha256,sdk_identity,32U)&&!memcmp(s->evidence.provenance,recipe_identity,32U)&&
  s->evidence.version==0x000605U&&s->evidence.api==0x030000U&&s->evidence.spi==0x0400U&&
  o->started_ms>=s->last_attempt_ms&&o->started_ms<=s->evidence.expires_ms;
 baseline=same?s->evidence.baseline:o->private_bytes;peak=same?s->evidence.peak:o->private_bytes;
 if(peak<o->private_bytes)peak=o->private_bytes;
 if(!same){
  if(archive_series(s)!=PAPACC_RESULT_OK)return PAPACC_RESULT_LIMIT_EXCEEDED;
  if(valid){
   if(s->series_id==(PAPACC_U64)-1){s->history_overflow=1U;s->status=0U;return PAPACC_RESULT_LIMIT_EXCEEDED;}
   ++s->series_id;s->series_first_sequence=s->sequence+1U;s->series_started_ms=o->started_ms;
  }
 }
 ++s->sequence;s->binding=*b;s->process=o->process;s->thread=o->thread;s->creation=o->creation;
 s->last_attempt_ms=o->finished_ms;s->last_attempt_origin=origin;s->working_set=o->working_set;memcpy(s->errors,o->errors,sizeof(s->errors));
 e=&s->evidence;memset(e,0,sizeof(*e));s->status=0U;
 for(i=0U;i<PAPACC_C8_RESOURCE_METRICS;++i)e->metrics[i].kind=i+1U;
 if(!valid)return PAPACC_RESULT_INTERNAL_ERROR;
 e->version=0x000605U;e->api=0x030000U;e->spi=0x0400U;e->origin=origin;
 memcpy(e->target,target,32U);memcpy(e->sdk_sha256,sdk_identity,32U);memcpy(e->provenance,recipe_identity,32U);
 e->source_id=o->creation;e->profile=b->profile;e->phase=b->phase;e->sampled_ms=o->started_ms;
 e->expires_ms=o->started_ms+1000U;e->baseline=baseline;e->peak=peak;e->final_value=o->private_bytes;e->final_class=1U;
 e->concurrency=b->concurrency;e->retained=b->retained;
 e->metrics[0].value=o->physical;e->metrics[1].value=o->commit;e->metrics[2].value=o->private_bytes;e->metrics[3].value=o->handles;
 for(i=0U;i<4U;++i)e->metrics[i].classification=PAPACC_C8_DIRECTLY_MEASURED;
 /* Process pool observations do not bound kernel or provider per Connection. */
 e->metrics[6].value=o->paged;e->metrics[7].value=o->nonpaged;
 e->metrics[6].classification=e->metrics[7].classification=PAPACC_C8_DIRECTLY_MEASURED;
 s->status=1U;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c8_target_observe_offline(PAPACC_C8_TARGET_PRODUCER *s,const PAPACC_C8_TARGET_BINDING *b,const PAPACC_C8_TARGET_OBSERVATION *o)
{return reduce(s,b,o,1U);}
#include "server_c8_collection_gate_win32.inc"
PAPACC_RESULT papacc_c8_target_collect_diagnostic(PAPACC_C8_TARGET_PRODUCER *s,const PAPACC_C8_TARGET_BINDING *b,PAPACC_U32 *reason,PAPACC_U32 *incomplete)
{
 PAPACC_C8_TARGET_OBSERVATION o;MEMORYSTATUSEX memory;PROCESS_MEMORY_COUNTERS_EX counters;
 FILETIME creation,exit_time,kernel,user;DWORD handles=0U;PAPACC_U64 now,end;PST_VERSION_INFO version;HANDLE self;PAPACC_RESULT result;
 if(!reason||!incomplete)return PAPACC_RESULT_INVALID_ARGUMENT;
 *reason=PAPACC_C8_COLLECT_FAILED;*incomplete=0U;
 if(!s||!b)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(s->history_overflow||s->history_count>PAPACC_C8_TARGET_HISTORY_CAPACITY||s->sequence==(PAPACC_U64)-1)return PAPACC_RESULT_LIMIT_EXCEEDED;
 if(b->owner&&b->owner!=(PAPACC_U64)GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK){
  *reason=PAPACC_C8_COLLECT_CLOCK;*incomplete=PAPACC_C8_COLLECTION_CLOCK_INCOMPLETE;
  (void)papacc_c8_collection_clock_fault(b,PAPACC_TRUE);
  if(archive_series(s)!=PAPACC_RESULT_OK)return PAPACC_RESULT_LIMIT_EXCEEDED;
  s->status=0U;return PAPACC_RESULT_INTERNAL_ERROR;
 }
 now/=1000000U;
 *reason=collection_begin(b,now,incomplete);
 if(*reason!=PAPACC_C8_COLLECT_OK)return PAPACC_RESULT_INVALID_STATE;
 memset(&o,0,sizeof(o));o.started_ms=now;o.process=GetCurrentProcessId();o.thread=GetCurrentThreadId();self=GetCurrentProcess();
 memset(&memory,0,sizeof(memory));memory.dwLength=sizeof(memory);
 if(!GlobalMemoryStatusEx(&memory))o.errors[0]=GetLastError()?GetLastError():ERROR_GEN_FAILURE;
 else {o.physical=memory.ullAvailPhys;o.commit=memory.ullAvailPageFile;}
 memset(&counters,0,sizeof(counters));counters.cb=sizeof(counters);
 if(!GetProcessMemoryInfo(self,(PROCESS_MEMORY_COUNTERS*)&counters,sizeof(counters)))o.errors[1]=GetLastError()?GetLastError():ERROR_GEN_FAILURE;
 else {o.private_bytes=counters.PrivateUsage;o.working_set=counters.WorkingSetSize;o.paged=counters.QuotaPagedPoolUsage;o.nonpaged=counters.QuotaNonPagedPoolUsage;}
 if(!GetProcessHandleCount(self,&handles))o.errors[2]=GetLastError()?GetLastError():ERROR_GEN_FAILURE;else o.handles=handles;
 if(!GetProcessTimes(self,&creation,&exit_time,&kernel,&user))o.errors[3]=GetLastError()?GetLastError():ERROR_GEN_FAILURE;
 else o.creation=((PAPACC_U64)creation.dwHighDateTime<<32)|creation.dwLowDateTime;
 memset(&version,0,sizeof(version));version.struct_size=sizeof(version);version.api_version=PST_API_VERSION;
 if(pst_get_version(&version)!=PST_RESULT_OK)o.errors[4]=ERROR_REVISION_MISMATCH;
 else o.version_ok=version.library_major==0U&&version.library_minor==6U&&version.library_patch==5U&&version.api_major==3U&&version.api_minor==0U&&version.api_patch==0U;
 if(papacc_pal_monotonic_time_ns(&end)!=PAPACC_RESULT_OK){
  o.success=0U;o.finished_ms=now;result=reduce(s,b,&o,2U);
  collection_end(now,0,reason,incomplete);return result;
 }
 o.finished_ms=end/1000000U;o.success=1U;result=reduce(s,b,&o,2U);
 if(result!=PAPACC_RESULT_OK)*reason=PAPACC_C8_COLLECT_FAILED;
 collection_end(o.finished_ms,1,reason,incomplete);return result;
}
PAPACC_RESULT papacc_c8_target_collect(PAPACC_C8_TARGET_PRODUCER *s,const PAPACC_C8_TARGET_BINDING *b)
{
 PAPACC_U32 reason,incomplete;return papacc_c8_target_collect_diagnostic(s,b,&reason,&incomplete);
}
PAPACC_C8_RESOURCE_REASON papacc_c8_target_validate_raw(const PAPACC_C8_TARGET_PRODUCER *s,const PAPACC_C8_TARGET_BINDING *b,const PAPACC_U8 *target,PAPACC_U64 now)
{
 unsigned i;PAPACC_U8 actual[32];
 if(!s||!b||!target||!s->status||!s->sequence||!s->series_id||s->history_overflow||
 s->history_count>PAPACC_C8_TARGET_HISTORY_CAPACITY||s->series_first_sequence>s->sequence||
 s->series_started_ms>s->evidence.sampled_ms||!binding_equal(&s->binding,b))return PAPACC_C8_RESOURCE_STATE_ERROR;
 put32(actual,s->process);put64(actual+4U,s->creation);put32(actual+12U,s->thread);
 put64(actual+16U,b->invocation);put32(actual+24U,(PAPACC_U32)sizeof(void*));put32(actual+28U,1U);
 if(memcmp(actual,s->evidence.target,32U)||(b->owner&&b->owner!=s->thread)||s->evidence.source_id!=s->creation||
 b->configured>1U||b->current>1U||b->retained>b->concurrency)return PAPACC_C8_RESOURCE_STATE_ERROR;
 if(s->evidence.origin!=2U||s->last_attempt_origin!=2U||!s->process||!s->creation||!s->thread||memcmp(s->evidence.target,target,32U)||
 memcmp(s->evidence.sdk_sha256,sdk_identity,32U)||memcmp(s->evidence.provenance,recipe_identity,32U)||
 s->evidence.version!=0x000605U||s->evidence.api!=0x030000U||s->evidence.spi!=0x0400U||
 s->evidence.profile!=b->profile||s->evidence.phase!=b->phase||s->evidence.concurrency!=b->concurrency||s->evidence.retained!=b->retained)
 return PAPACC_C8_RESOURCE_STATE_ERROR;
 for(i=0U;i<5U;++i)if(s->errors[i])return PAPACC_C8_RESOURCE_UNKNOWN;
 if(now<s->evidence.sampled_ms||now>s->evidence.expires_ms||s->evidence.expires_ms<s->evidence.sampled_ms||
 s->evidence.expires_ms-s->evidence.sampled_ms>1000U)return PAPACC_C8_RESOURCE_STALE;
 if(b->managed>PAPACC_C8_RESOURCE_LIMIT||b->charged>PAPACC_C8_RESOURCE_LIMIT-b->managed)return PAPACC_C8_RESOURCE_CAPACITY;
 return PAPACC_C8_RESOURCE_OK;
}
PAPACC_C8_RESOURCE_REASON papacc_c8_target_admission(const PAPACC_C8_TARGET_PRODUCER *s,const PAPACC_C8_TARGET_BINDING *b,const PAPACC_U8 *target,PAPACC_U64 now)
{
 PAPACC_C8_RESOURCE_REASON result=papacc_c8_target_validate_raw(s,b,target,now);
 if(result!=PAPACC_C8_RESOURCE_OK)return result;
 if(!b->configured||!b->current||!b->invocation||!b->owner||!b->revision||!b->epoch||b->stopping)return PAPACC_C8_RESOURCE_STATE_ERROR;
 /* No evidence-backed provider/kernel recipe or empirical approval is installed.
  * Raw process totals and pool observations cannot authorize native effects. */
 return PAPACC_C8_RESOURCE_UNKNOWN;
}

/* Same own-process producer; reference capture is outside dispatch and only
 * before activation. Stop observations continue via the existing read-only API.
 * Paths identify the OS-selected mapped image, NOT byte attestation of pages.
 * SDK identity is the compiled normal inventory plus actual public version;
 * expected package hashes are never described as measured loaded-module hashes. */
PAPACC_RESULT papacc_c8_target_process_provenance(const PAPACC_C8_TARGET_PRODUCER*s,const PAPACC_C8_TARGET_BINDING*b,PAPACC_C8_PROCESS_PROVENANCE*out){
 PAPACC_C8_PROCESS_PROVENANCE f;WCHAR process_path[MAX_PATH];
 FILETIME c,e,k,u;PAPACC_U64 creation,now;PAPACC_U32 health;DWORD n;ULONG_PTR a=(ULONG_PTR)s,z=(ULONG_PTR)out,t=(ULONG_PTR)b;
 if(!s||!b||!out)return PAPACC_RESULT_INVALID_ARGUMENT;
 if((z>=a&&z-a<sizeof(*s))||(z<a&&a-z<sizeof(*out))||
    (z>=t&&z-t<sizeof(*b))||(z<t&&t-z<sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!s->status||!s->sequence||s->history_overflow||s->evidence.origin!=2U||
    !binding_equal(&s->binding,b)||b->stopping||
    (b->phase!=PAPACC_C8_OBSERVATION_BASELINE&&b->phase!=PAPACC_C8_OBSERVATION_PREPARED)||
    s->process!=GetCurrentProcessId()||s->thread!=GetCurrentThreadId()||b->owner!=s->thread)
  return PAPACC_RESULT_INVALID_STATE;
 if(papacc_c8_collection_health(b,&health)!=PAPACC_RESULT_OK||health)return PAPACC_RESULT_INVALID_STATE;
 if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INTERNAL_ERROR;
 now/=1000000U;
 if(now<s->evidence.sampled_ms||now>s->evidence.expires_ms||s->evidence.expires_ms<s->evidence.sampled_ms)
  return PAPACC_RESULT_INVALID_STATE;
 if(!GetProcessTimes(GetCurrentProcess(),&c,&e,&k,&u))return PAPACC_RESULT_INTERNAL_ERROR;
 creation=((PAPACC_U64)c.dwHighDateTime<<32)|c.dwLowDateTime;
 if(!creation||creation!=s->creation)return PAPACC_RESULT_INVALID_STATE;
 memset(&f,0,sizeof(f));memset(process_path,0,sizeof(process_path));
 n=GetModuleFileNameW(NULL,f.image_path,MAX_PATH);if(!n||n>=MAX_PATH)return PAPACC_RESULT_LIMIT_EXCEEDED;
 n=GetMappedFileNameW(GetCurrentProcess(),GetModuleHandleW(NULL),f.mapped_path,MAX_PATH);
 if(!n||n>=MAX_PATH)return PAPACC_RESULT_LIMIT_EXCEEDED;
 n=GetProcessImageFileNameW(GetCurrentProcess(),process_path,MAX_PATH);
 if(!n||n>=MAX_PATH||lstrcmpiW(process_path,f.mapped_path))return PAPACC_RESULT_INVALID_STATE;
 f.process=s->process;f.creation=creation;f.thread=s->thread;f.invocation=b->invocation;
 f.sequence=s->sequence;f.revision=b->revision;f.epoch=b->epoch;f.configured=b->configured;f.current=b->current;f.phase=b->phase;
 /* Copy the existing producer observation, existing validation instant and
  * recorded expiry exactly. Clock1 denotes the existing local PAL ns/ms
  * conversion, scoped by actual process/creation; not a new clock identity.
  * No Mode C applicability/approval is inferred from recorded C8 validity. */
 f.observed_ms=s->evidence.sampled_ms;f.validated_ms=now;f.valid_until_ms=s->evidence.expires_ms;
 f.time_presence=7U;f.clock_source=1U;
 f.build_debug=PAPACC_OBSERVED_BUILD_DEBUG;
 memcpy(f.domain,b->domain,32U);memcpy(f.sdk_inventory,s->evidence.sdk_sha256,32U);
 *out=f;return PAPACC_RESULT_OK;
}