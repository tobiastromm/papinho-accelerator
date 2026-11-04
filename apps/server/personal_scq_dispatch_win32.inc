#include "personal_scq_dispatch_win32.h"
DWORD papacc_personal_scq_prepare(PAPACC_PERSONAL_SCQ_DISPATCH *d,const PAPACC_PERSONAL_RUNTIME_IDENTITY *r,const BYTE association[32],const WCHAR *name)
{
 PAPACC_PERSONAL_SCQ_DISPATCH copy;size_t bytes;unsigned i,any=0;ULONG_PTR a,b;
 if(!d||!r||!association||d->phase||r->phase!=1U||r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||r->thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if(!pcp_name(name,&bytes))return ERROR_INVALID_NAME;
 a=(ULONG_PTR)d;b=(ULONG_PTR)association;if((b>=a&&b-a<sizeof(*d))||(a>=b&&a-b<32U))return ERROR_INVALID_PARAMETER;
 b=(ULONG_PTR)name;if((b>=a&&b-a<sizeof(*d))||(a>=b&&a-b<bytes))return ERROR_INVALID_PARAMETER;
 memset(&copy,0,sizeof(copy));memcpy(copy.association,association,32U);
 for(i=0;i<32U;i++)any|=copy.association[i];if(!any)return ERROR_INVALID_PARAMETER;
 memcpy(copy.name,name,bytes);if(!pcp_name(copy.name,&bytes))return ERROR_INVALID_NAME;
 copy.purpose=PAPACC_SCQ_PURPOSE_READ;
 copy.thread=GetCurrentThreadId();copy.phase=1;copy.pid=r->pid;copy.creation=r->creation;copy.token=r->statistics;
 *d=copy;return 0; /* association DATA only, not an approval or native source */
}
/* Full bounded absence, including bytes following a terminator. Never an authority. */
static int scqd_key_binding(DWORD purpose,const WCHAR key[112])
{
 unsigned i;size_t bytes;
 if(!key)return 0;
 if(purpose==PAPACC_SCQ_PURPOSE_READ)return pcp_name(key,&bytes);
 if(purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)return 0;
 for(i=0;i<112U;i++)if(key[i])return 0;return 1;
}
DWORD papacc_personal_scq_prepare_transport(PAPACC_PERSONAL_SCQ_DISPATCH *d,const PAPACC_PERSONAL_RUNTIME_IDENTITY *r,const BYTE association[32],const WCHAR key[112])
{
 PAPACC_PERSONAL_SCQ_DISPATCH copy;unsigned i,any=0;ULONG_PTR a,b;
 if(!d||!r||!association||d->phase||r->phase!=1U||r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||r->thread!=GetCurrentThreadId()||r->closed_preparation!=1U)return ERROR_INVALID_STATE;
 if(!scqd_key_binding(PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY,key))return ERROR_INVALID_NAME;
 a=(ULONG_PTR)d;b=(ULONG_PTR)association;if((b>=a&&b-a<sizeof(*d))||(a>=b&&a-b<32U))return ERROR_INVALID_PARAMETER;
 b=(ULONG_PTR)key;if((b>=a&&b-a<sizeof(*d))||(a>=b&&a-b<112U*sizeof(WCHAR)))return ERROR_INVALID_PARAMETER;
 memset(&copy,0,sizeof(copy));memcpy(copy.association,association,32U);
 for(i=0;i<32U;i++)any|=copy.association[i];if(!any)return ERROR_INVALID_PARAMETER;
 copy.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;copy.thread=GetCurrentThreadId();copy.phase=1U;copy.pid=r->pid;copy.creation=r->creation;copy.token=r->statistics;
 *d=copy;return 0; /* typed DATA, never admission or provider permission */
}
/* Called only AFTER native admission by the canonical owner. The isolated
 * component test exercises this body without pretending app admission passed.
 * Synchronous: never transfer handle ownership to another thread. */
static DWORD personal_scq_dispatch_body(PAPACC_PERSONAL_SCQ_DISPATCH *d,PAPACC_PERSONAL_RUNTIME_IDENTITY *r,PAPACC_PERSONAL_CNG_PROBE *probe,const BYTE association[32],volatile LONG *stop)
{
 DWORD result;
 if(!d||!r||!probe||!association||!stop||d->thread!=GetCurrentThreadId()||d->phase!=1U)return ERROR_INVALID_STATE;
 /* Every direct KSP dispatch rejects transport before touching the provider,
  * even if name/closed metadata is subsequently substituted. */
 if(d->purpose!=PAPACC_SCQ_PURPOSE_READ){d->result=PAPACC_CNG_PROBE_LOCAL_DENY;return d->result;}
 d->phase=2U;
 if(InterlockedCompareExchange(stop,0,0)){d->result=PAPACC_CNG_PROBE_STOPPED;d->phase=3;return d->result;}
 if(memcmp(d->association,association,32U)||r->phase!=1U||r->pid!=d->pid||memcmp(&d->creation,&r->creation,sizeof(FILETIME))||
 memcmp(&d->token.TokenId,&r->statistics.TokenId,sizeof(LUID))||memcmp(&d->token.AuthenticationId,&r->statistics.AuthenticationId,sizeof(LUID))||
 memcmp(&d->token.ModifiedId,&r->statistics.ModifiedId,sizeof(LUID))){d->result=PAPACC_CNG_PROBE_QUERY_FAILURE;d->phase=3;return d->result;}
 result=papacc_personal_cng_probe(probe,r,d->name,stop);
 d->result=probe->phase?probe->classification:PAPACC_CNG_PROBE_LOCAL_DENY;
 d->phase=probe->phase==4U||r->phase==4U?4U:3U;
 return result;
}
