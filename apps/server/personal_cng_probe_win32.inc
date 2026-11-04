#include "personal_cng_probe_win32.h"
#include <wchar.h>
#include <string.h>
/* origin:1 local predicate,2 Win32,3 NCrypt,4 original identity binding. */
static void pcp_first(PAPACC_PERSONAL_CNG_PROBE *p,DWORD stage,SECURITY_STATUS raw,DWORD origin)
{if(!p->first_stage){p->first_stage=stage;p->first_error=raw;p->first_origin=origin;}}
static int pcp_effective_token(PAPACC_PERSONAL_CNG_PROBE *p)
{
 DWORD e;
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&p->unexpected_thread)){
  p->classification=PAPACC_CNG_PROBE_LOCAL_DENY;pcp_first(p,2,ERROR_ACCESS_DENIED,1);return 0;
 }
 e=GetLastError();if(e==ERROR_NO_TOKEN&&!p->unexpected_thread)return 1;
 p->classification=PAPACC_CNG_PROBE_QUERY_FAILURE;pcp_first(p,2,(SECURITY_STATUS)e,2);return 0;
}
static int pcp_name(const WCHAR *name,size_t *bytes)
{
 static const WCHAR prefix[]=L"PapinhoAccelerator.CA.";
 size_t n=0,i,k=(sizeof(prefix)/sizeof(prefix[0]))-1U;unsigned nonzero;
 if(!name)return 0;while(n<112U&&name[n])n++;
 if(n!=k+65U||wcsncmp(name,prefix,k)||name[k+32U]!=L'.')return 0;
 for(i=0;i<2U;i++){
  size_t j;nonzero=0;
  for(j=0;j<32U;j++){WCHAR c=name[k+i*33U+j];if(!((c>=L'0'&&c<=L'9')||(c>=L'a'&&c<=L'f')))return 0;nonzero|=c!=L'0';}
  if(!nonzero)return 0;
 }
 *bytes=(n+1U)*sizeof(WCHAR);return 1;
}
DWORD papacc_personal_cng_probe_close(PAPACC_PERSONAL_CNG_PROBE *p)
{
 SECURITY_STATUS e;
 if(!p||p->thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if(p->phase==3U)return 0;
 if(p->close_attempted)return ERROR_INVALID_STATE;
 p->close_attempted=1;
 if(p->key){p->close_api=3;p->close_calls++;e=NCryptFreeObject(p->key);if(e){p->close_error=e;goto retained;}p->key=0;}
 if(p->provider){p->close_api=3;p->close_calls++;e=NCryptFreeObject(p->provider);if(e){p->close_error=e;goto retained;}p->provider=0;}
 if(p->unexpected_thread){p->close_api=2;p->close_calls++;if(!CloseHandle(p->unexpected_thread)){e=(SECURITY_STATUS)GetLastError();p->close_error=e;goto retained;}p->unexpected_thread=NULL;}
 p->phase=3U;return 0;
retained:
 pcp_first(p,6,e,p->close_api);p->classification=PAPACC_CNG_PROBE_CLOSE_UNKNOWN;p->phase=4U;return ERROR_INVALID_STATE;
}
#include "personal_scq_session_win32.h"
#include "personal_scq_start_assurance_win32.h"
/* Same fresh G3 observation and immediate pre-acquisition count pattern as
 * native transport effects. No provider call when scope/sample is UNKNOWN. */
static DWORD pcp_read_resource(PAPACC_PERSONAL_CNG_PROBE *p,PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,volatile LONG *stop)
{
 const PAPACC_SCQ_READ_RECIPIENT *r=p->read_scope;const PAPACC_SCQ_SESSION_DRIVER *c;const PAPACC_SCQ_START_ASSURANCE *g;const PAPACC_SCM_RESOURCE_SAMPLE *s;LARGE_INTEGER now;DWORD count,error;
 if(!r||r->self!=r||r->thread!=GetCurrentThreadId()||r->phase!=2U||r->first||r->probe!=p||r->probe_used!=1U||!r->origin||!r->origin->value.controller_consumed||!r->origin->value.consumed||!r->origin->value.borrowed||r->origin->value.retained)return ERROR_ACCESS_DENIED;
 c=r->session;if(!c||c->self!=c||c->owner!=r->owner||c->read_recipient!=r||c->runtime!=runtime||c->role!=2U||c->phase!=3U||c->first||c->peer_lost||!c->profile||c->profile->read_diagnostic!=1U||!(c->transport.legs&256U)||!(c->transport.legs&2048U))return ERROR_ACCESS_DENIED;
 g=c->transport.resource_scope;if(!g||g->self!=g||g->owner!=r->owner||g->thread!=GetCurrentThreadId()||g->phase!=5U||g->source||g->first||g->read_diagnostic!=1U||!g->p1_handles||!g->initial_known||g->initial_handles>128U||g->limits.max_handles!=192U||g->limits.private_growth!=1994944U||g->limits.working_growth!=3145728U||g->limits.physical_floor!=7239172096ULL||g->limits.commit_floor!=3221225472ULL||g->limits.freshness_ms!=100U||g->limits.duration_ms!=60000U||!g->global_until||g->global_until!=c->transport.global_until||g->global_until>r->origin->value.deadline||GetTickCount64()>=g->global_until)return ERROR_ACCESS_DENIED;
 s=&g->observations.last;if(!s->frequency||s->frequency!=g->ready.frequency||s->begin>s->end||!QueryPerformanceCounter(&now)||now.QuadPart<0||(ULONGLONG)now.QuadPart<s->end||(ULONGLONG)now.QuadPart-s->begin>s->frequency/10U)return ERROR_TIMEOUT;
 if(!GetProcessHandleCount(GetCurrentProcess(),&count)){error=GetLastError();return error?error:ERROR_GEN_FAILURE;}
 if(count>=188U||s->physical_available<g->limits.physical_floor||s->commit_available<g->limits.commit_floor||
  (s->private_bytes>g->ready.private_bytes&&s->private_bytes-g->ready.private_bytes>g->limits.private_growth)||
  (s->working_set>g->ready.working_set&&s->working_set-g->ready.working_set>g->limits.working_growth))return ERROR_NOT_ENOUGH_QUOTA;
 return *stop?ERROR_CANCELLED:0;
}
static DWORD pcp_probe_mode(PAPACC_PERSONAL_CNG_PROBE *p,PAPACC_PERSONAL_RUNTIME_IDENTITY *r,const WCHAR *name,volatile LONG *stop,int closed_read)
{
 SECURITY_STATUS raw;DWORD e;size_t bytes;ULONG_PTR a,b;
 if(!p||!r||!stop||p->phase||!pcp_name(name,&bytes))return ERROR_INVALID_PARAMETER;
 a=(ULONG_PTR)p;b=(ULONG_PTR)name;
 if((b>=a&&b-a<sizeof(*p))||(a>=b&&a-b<bytes))return ERROR_INVALID_PARAMETER;
 if(r->thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 p->thread=GetCurrentThreadId();p->phase=1U;memcpy(p->name,name,bytes);
 if(!pcp_name(p->name,&bytes)){p->classification=PAPACC_CNG_PROBE_LOCAL_DENY;pcp_first(p,2,ERROR_INVALID_DATA,1);goto close;}
 if(InterlockedCompareExchange(stop,0,0)){p->classification=PAPACC_CNG_PROBE_STOPPED;pcp_first(p,1,ERROR_OPERATION_ABORTED,1);goto close;}
 if(!pcp_effective_token(p))goto close;
 /* Never impersonate or treat an admin observer's runtime handle as self. */
 if(r->transport_only||(r->closed_preparation&&!(closed_read==1&&r->closed_preparation==1U&&!r->transport_only))||
  (closed_read&&(closed_read!=1||r->closed_preparation!=1U||r->transport_only||r->read_diagnostic!=1U))||r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||r->pid!=GetCurrentProcessId()){
  p->classification=PAPACC_CNG_PROBE_LOCAL_DENY;pcp_first(p,2,ERROR_ACCESS_DENIED,1);goto close;
 }
 e=papacc_personal_runtime_current(r);
 if(e){p->classification=PAPACC_CNG_PROBE_QUERY_FAILURE;pcp_first(p,2,(SECURITY_STATUS)e,4);goto close;}
 if(InterlockedCompareExchange(stop,0,0)){p->classification=PAPACC_CNG_PROBE_STOPPED;pcp_first(p,1,ERROR_OPERATION_ABORTED,1);goto close;}
 if(closed_read){e=pcp_read_resource(p,r,stop);if(e){p->classification=PAPACC_CNG_PROBE_QUERY_FAILURE;pcp_first(p,2,(SECURITY_STATUS)e,4);goto close;}}
 p->provider_calls++;raw=NCryptOpenStorageProvider(&p->provider,MS_KEY_STORAGE_PROVIDER,0);p->provider_result=raw;
 if(raw){p->classification=PAPACC_CNG_PROBE_PROVIDER_FAILURE;pcp_first(p,3,raw,3);goto close;}
 if(!p->provider){p->classification=PAPACC_CNG_PROBE_PROVIDER_FAILURE;pcp_first(p,3,NTE_INVALID_HANDLE,1);goto close;}
 if(InterlockedCompareExchange(stop,0,0)){p->classification=PAPACC_CNG_PROBE_STOPPED;pcp_first(p,1,ERROR_OPERATION_ABORTED,1);goto close;}
 if(closed_read){if(!pcp_effective_token(p))goto close;e=papacc_personal_runtime_current(r);if(!e)e=pcp_read_resource(p,r,stop);if(e){p->classification=PAPACC_CNG_PROBE_QUERY_FAILURE;pcp_first(p,2,(SECURITY_STATUS)e,4);goto close;}}
 p->open_calls++;raw=NCryptOpenKey(p->provider,&p->key,p->name,0,NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG);p->open_result=raw;
 /* Any returned key handle is an unexpected capability, even on error. */
 if(!raw||p->key){p->classification=PAPACC_CNG_PROBE_UNEXPECTED_OPEN;pcp_first(p,4,raw,3);}
 else if(raw==NTE_PERM)p->classification=PAPACC_CNG_PROBE_PERMISSION_OBSERVED;
 else if(raw==NTE_BAD_KEYSET)p->classification=PAPACC_CNG_PROBE_NOT_FOUND;
 else p->classification=PAPACC_CNG_PROBE_PROVIDER_FAILURE;
 if(raw)pcp_first(p,4,raw,3);
 if(InterlockedCompareExchange(stop,0,0)){p->classification=PAPACC_CNG_PROBE_STOPPED;pcp_first(p,1,ERROR_OPERATION_ABORTED,1);goto close;}
 e=papacc_personal_runtime_current(r);
 if(e){p->classification=PAPACC_CNG_PROBE_QUERY_FAILURE;pcp_first(p,5,(SECURITY_STATUS)e,4);}
 if(!p->unexpected_thread)(void)pcp_effective_token(p);
 if(InterlockedCompareExchange(stop,0,0)){p->classification=PAPACC_CNG_PROBE_STOPPED;pcp_first(p,1,ERROR_OPERATION_ABORTED,1);}
close:
 (void)papacc_personal_cng_probe_close(p);
 return p->classification; /* diagnostic enum, never an access grant */
}

/* Existing ordinary READ entry never receives the K4 closed-read exception. */
DWORD papacc_personal_cng_probe(PAPACC_PERSONAL_CNG_PROBE *p,PAPACC_PERSONAL_RUNTIME_IDENTITY *r,const WCHAR *name,volatile LONG *stop)
{return pcp_probe_mode(p,r,name,stop,0);}
