#include "personal_scq_read_diagnostic_win32.h"
#include "personal_scq_start_assurance_win32.inc"
#include "personal_scq_start_plan_data_win32.inc"
/* No phase here is inferred from Start, DATA, a digest or transport receipt.
 * Runtime admission of P must still come from its original registered source.
 * The ordinary source getter remains DENY until that binding is implemented. */
static DWORD scqk_fail(PAPACC_SCQ_READ_DIAGNOSTIC *k,DWORD e)
{if(k&&!k->first)k->first=e;if(k)k->phase=4U;return e;}
static DWORD scqk_current(PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;unsigned i;
 if(!k||k->self!=k||k->owner!=owner||!owner||k->thread!=GetCurrentThreadId()||!k->source||!stop||k->first||!k->phase||k->phase>3U)return ERROR_INVALID_STATE;
 e=scqa_current(k->source,owner,stop);if(e)return scqk_fail(k,e);
 if(k->source->plan->read_diagnostic!=1U||k->source->registration->read_diagnostic!=1U)return scqk_fail(k,ERROR_ACCESS_DENIED);
 if(scqp_decode_fields(k->source->plan->record,sizeof(k->source->plan->record),&p)||p.purpose!=PAPACC_SCQ_PURPOSE_READ||
  memcmp(k->attempt,p.attempt,32)||memcmp(k->policy,p.policy,32)||memcmp(k->request,p.request,32)||memcmp(k->decision,p.decision,32)||
  k->until!=(((ULONGLONG)p.valid_until[1]<<32)|p.valid_until[0])||GetTickCount64()>=k->until)return scqk_fail(k,ERROR_ACCESS_DENIED);
 for(i=0;i<112U;i++)if(k->key[i]!=(WCHAR)(BYTE)p.key[i])return scqk_fail(k,ERROR_ACCESS_DENIED);
 return 0;
}
static DWORD scqk_prepare(PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,PAPACC_SCQ_ADMIN_SOURCE *source,const BYTE key_public[32],volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;unsigned i,any=0;DWORD e;
 if(!k||k->phase||k->self||!source||!key_public||!stop)return ERROR_INVALID_STATE;
 e=scqa_current(source,owner,stop);if(e)return e;
 if(source->plan->read_diagnostic!=1U||source->registration->read_diagnostic!=1U||source->phase!=3U||scqp_decode_fields(source->plan->record,sizeof(source->plan->record),&p)||p.purpose!=PAPACC_SCQ_PURPOSE_READ)return ERROR_ACCESS_DENIED;
 for(i=0;i<32U;i++)any|=key_public[i];if(!any)return ERROR_INVALID_DATA;
 k->self=k;k->owner=owner;k->source=source;k->thread=GetCurrentThreadId();k->phase=1U;
 memcpy(k->attempt,p.attempt,32);memcpy(k->policy,p.policy,32);memcpy(k->request,p.request,32);memcpy(k->decision,p.decision,32);memcpy(k->key_public,key_public,32);
 for(i=0;i<112U;i++)k->key[i]=(WCHAR)(BYTE)p.key[i];k->until=((ULONGLONG)p.valid_until[1]<<32)|p.valid_until[0];
 return scqk_current(k,owner,stop);
}
static DWORD scqk_confirm(PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,const char *reply,size_t bytes,volatile LONG *stop)
{
 static const char prefix[]="OWNER_SCQ_MACHINE_READ ",hex[]="0123456789abcdef";DWORD e;unsigned i;
 if(!k||k->phase!=1U)return ERROR_INVALID_STATE;e=scqk_current(k,owner,stop);if(e)return e;
 if(!reply||bytes!=sizeof(prefix)-1U+64U||memcmp(reply,prefix,sizeof(prefix)-1U))return scqk_fail(k,ERROR_ACCESS_DENIED);
 for(i=0;i<32U;i++)if(reply[sizeof(prefix)-1U+2U*i]!=hex[k->attempt[i]>>4]||reply[sizeof(prefix)+2U*i]!=hex[k->attempt[i]&15U])return scqk_fail(k,ERROR_ACCESS_DENIED);
 k->phase=2U;return 0;
}
static DWORD scqk_take(PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,volatile LONG *stop)
{
 DWORD e;if(!k||k->phase!=2U)return ERROR_INVALID_STATE;e=scqk_current(k,owner,stop);if(e)return e;
 if(InterlockedCompareExchange(&k->consumed,1,0))return scqk_fail(k,ERROR_ALREADY_EXISTS);k->phase=3U;return 0;
}
/* Separate result DATA, never SQR/SQC or readiness. Produces no native query. */
static DWORD scqk_result(const PAPACC_PERSONAL_CNG_PROBE *p,int admin_control)
{
 if(!p||!admin_control||p->phase!=3U||p->close_error||p->provider||p->key||p->unexpected_thread||
  p->provider_calls!=1U||p->provider_result||p->open_calls!=1U)return 0;
 return p->classification==PAPACC_CNG_PROBE_PERMISSION_OBSERVED&&p->open_result==NTE_PERM?1U:
  p->classification==PAPACC_CNG_PROBE_UNEXPECTED_OPEN?2U:0U;
}

/* K5 private SKR1 observation DATA: 16 little-endian DWORDs + attempt +
 * public-key digest. Never a receipt, close proof, authority or wire API. */
#define PAPACC_SCQ_READ_RESULT_BYTES 128U
static void scqk_u32_put(BYTE *p,DWORD v)
{unsigned i;for(i=0;i<4U;i++)p[i]=(BYTE)(v>>(8U*i));}
static DWORD scqk_u32_get(const BYTE *p)
{return (DWORD)p[0]|((DWORD)p[1]<<8)|((DWORD)p[2]<<16)|((DWORD)p[3]<<24);}
static DWORD scqk_result_encode(const PAPACC_PERSONAL_CNG_PROBE *p,const BYTE attempt[32],const BYTE public_id[32],BYTE out[128])
{
 DWORD fields[14];unsigned i,any=0;
 if(!p||!attempt||!public_id||!out||p->phase!=3U||p->provider||p->key||p->unexpected_thread||!p->close_attempted||p->close_error||
  p->provider_calls>1U||p->open_calls>1U||p->close_calls>2U)return ERROR_INVALID_STATE;
 for(i=0;i<32U;i++)any|=attempt[i];if(!any)return ERROR_INVALID_DATA;any=0;for(i=0;i<32U;i++)any|=public_id[i];if(!any)return ERROR_INVALID_DATA;
 fields[0]=PAPACC_SCQ_PURPOSE_READ;fields[1]=1U;fields[2]=p->classification;fields[3]=p->provider_calls;fields[4]=p->open_calls;
 fields[5]=(DWORD)p->provider_result;fields[6]=(DWORD)p->open_result;fields[7]=p->first_stage;fields[8]=p->first_origin;
 fields[9]=(DWORD)p->first_error;fields[10]=(DWORD)p->close_error;fields[11]=p->phase;fields[12]=p->close_attempted;fields[13]=p->close_calls;
 memcpy(out,"SKR1",4);scqk_u32_put(out+4,128U);for(i=0;i<14U;i++)scqk_u32_put(out+8U+4U*i,fields[i]);
 memcpy(out+64,attempt,32);memcpy(out+96,public_id,32);return 0;
}
static DWORD scqk_result_decode(const BYTE *frame,DWORD bytes,const BYTE attempt[32],const BYTE public_id[32],PAPACC_PERSONAL_CNG_PROBE *out)
{
 PAPACC_PERSONAL_CNG_PROBE p;unsigned i,any=0;
 if(!frame||!attempt||!public_id||!out||bytes!=128U||memcmp(frame,"SKR1",4)||scqk_u32_get(frame+4)!=128U||
  scqk_u32_get(frame+8)!=PAPACC_SCQ_PURPOSE_READ||scqk_u32_get(frame+12)!=1U||memcmp(frame+64,attempt,32)||memcmp(frame+96,public_id,32))return ERROR_INVALID_DATA;
 for(i=0;i<32U;i++)any|=attempt[i];if(!any)return ERROR_INVALID_DATA;any=0;for(i=0;i<32U;i++)any|=public_id[i];if(!any)return ERROR_INVALID_DATA;
 memset(&p,0,sizeof(p));p.classification=scqk_u32_get(frame+16);p.provider_calls=scqk_u32_get(frame+20);p.open_calls=scqk_u32_get(frame+24);
 p.provider_result=(SECURITY_STATUS)scqk_u32_get(frame+28);p.open_result=(SECURITY_STATUS)scqk_u32_get(frame+32);
 p.first_stage=scqk_u32_get(frame+36);p.first_origin=scqk_u32_get(frame+40);p.first_error=(SECURITY_STATUS)scqk_u32_get(frame+44);
 p.close_error=(SECURITY_STATUS)scqk_u32_get(frame+48);p.phase=scqk_u32_get(frame+52);p.close_attempted=scqk_u32_get(frame+56);p.close_calls=scqk_u32_get(frame+60);
 if(!p.classification||p.classification>PAPACC_CNG_PROBE_CLOSE_UNKNOWN||p.provider_calls>1U||p.open_calls>1U||p.close_calls>2U||
  p.phase!=3U||p.close_attempted!=1U||p.close_error||p.first_origin>4U||p.first_stage>6U||
  (p.open_calls&&(!p.provider_calls||p.provider_result)))return ERROR_INVALID_DATA;
 if(p.classification==PAPACC_CNG_PROBE_PERMISSION_OBSERVED&&(!scqk_result(&p,1)||p.first_stage!=4U||p.first_origin!=3U||p.first_error!=NTE_PERM||p.close_calls!=1U))return ERROR_INVALID_DATA;
 *out=p;return 0; /* correlated DATA only; native source/current gates remain mandatory */
}

/* K7 selection requires the original READ actor, never a peer cap/purpose bit.
 * Reuse resource sampling arithmetic without extending the old TO selector. */
static DWORD scqk_resource_ready(PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,PAPACC_SCQ_START_ASSURANCE *g,
 const PAPACC_SCM_RESOURCE_SAMPLE *sample,ULONGLONG qpc,ULONGLONG tick,ULONGLONG until,volatile LONG *stop)
{
 DWORD e=scqk_current(k,owner,stop);if(e)return e;
 if(until>k->until)return ERROR_ACCESS_DENIED;
 {DWORD sampled=scqg_query_ready_mode(g,owner,sample,qpc,tick,until,stop,1);if(!sampled)g->read_diagnostic=1U;return sampled;}
}
static DWORD scqk_resource_admit(PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,PAPACC_SCQ_START_ASSURANCE *g,
 const PAPACC_SCM_RESOURCE_SAMPLE *sample,ULONGLONG qpc,ULONGLONG tick,int creating,volatile LONG *stop)
{
 DWORD e=scqk_current(k,owner,stop);if(e)return e;
 if(!g||g->global_until>k->until)return ERROR_ACCESS_DENIED;
 return scqg_query_admit(g,owner,sample,qpc,tick,creating,stop);
}
/* Pure policy DATA for cold/bootstrap producers; never P/source authority. */
static DWORD scqk_policy_preimage(PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,const PAPACC_SCQ_START_LIMITS *limits,
 BYTE out[PAPACC_SCQ_G3_PREIMAGE_BYTES],volatile LONG *stop)
{DWORD e=scqk_current(k,owner,stop);return e?e:scqk_policy_data(limits,out);}
