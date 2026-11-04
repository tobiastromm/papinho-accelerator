#define _WIN32_WINNT 0x0600
#include "server_ca_key_win32.h"
#include <bcrypt.h>
#include <sddl.h>
#include <aclapi.h>
#include <string.h>
#include <stdio.h>
/* No logger/allocator/API callbacks. First receipt is sticky for this owner.
 * Wrong-thread calls cannot mutate even diagnostic owner storage. */
static SECURITY_STATUS ca_fail(PAPACC_CA_KEY *o,DWORD fn,DWORD stage,DWORD origin,
 DWORD api,SECURITY_STATUS raw,SECURITY_STATUS result,DWORD win32) {
 PAPACC_CA_FAILURE *d;DWORD saved_last_error=GetLastError();
 if(o&&(!o->thread||o->thread==GetCurrentThreadId())&&!o->first_failure.present) {
  d=&o->first_failure;d->function=fn;d->stage=stage;d->origin=origin;d->native_api=api;
  d->original_code=raw;d->backend_code=result;d->win32_error=win32;
  d->state=(DWORD)o->state;d->created=(DWORD)o->created;
  d->finalization=o->diagnostic_finalization;d->key_owned=o->key!=0;
  d->provider_owned=o->provider!=0;
  d->ownership=o->state==PAPACC_CA_KEY_RETAINED?2U:
   o->state==PAPACC_CA_KEY_CLOSED?3U:(o->key||o->provider||o->retained_token?1U:0U);
  d->present=1;
 }
 SetLastError(saved_last_error);return result;
}
static SECURITY_STATUS ca_local(PAPACC_CA_KEY *o,DWORD fn,DWORD stage,SECURITY_STATUS s) {
 return ca_fail(o,fn,stage,CA_D_LOCAL_GUARD,CA_API_NONE,s,s,0);
}
static SECURITY_STATUS ca_native(PAPACC_CA_KEY *o,DWORD fn,DWORD stage,DWORD api,SECURITY_STATUS s) {
 if(s!=ERROR_SUCCESS)(void)ca_fail(o,fn,stage,CA_D_NATIVE_NCRYPT,api,s,s,0);
 return s;
}
static SECURITY_STATUS ca_win32(PAPACC_CA_KEY *o,DWORD fn,DWORD stage,DWORD raw,SECURITY_STATUS mapped) {
 return ca_fail(o,fn,stage,CA_D_LOCAL_GUARD,CA_API_NONE,(SECURITY_STATUS)raw,mapped,raw);
}
#if defined(_MSC_VER)
/* This owner is also source-included by isolated identity/probe models. */
#pragma comment(lib,"psapi.lib")
#endif
#include "server_personal_resource_sample_win32.inc"
/* Accepted fixture F0-F5: own entry128/guard188/cap192, commit3GiB.
 * K7 READ is not selected here. Native/provider costs remain UNKNOWN.
 * Every acquisition/property/public call refreshes; cleanup runs once even
 * after Stop or budget failure, so an error can never rearm this budget. */
DWORD papacc_ca_key_fixture_budget_begin(PAPACC_CA_KEY *o)
{
 PAPACC_CA_FIXTURE_BUDGET *b;DWORD e;ULONGLONG now;
 if(!o||o->state!=PAPACC_CA_KEY_EMPTY||o->diagnostic_only||o->provider||o->key)return ERROR_INVALID_STATE;
 b=&o->diagnostic_budget;if(b->active||b->self||b->first||b->observation.samples||b->observation.attempts||b->observation.first_error||b->observation.sink)return ERROR_INVALID_STATE;
 b->self=b;b->thread=GetCurrentThreadId();b->active=1U;
 e=personal_resource_collect(&b->observation,1U,0);if(e){b->first=e;return e;}
 now=GetTickCount64();if(now>((ULONGLONG)-1)-60000U){b->first=ERROR_ARITHMETIC_OVERFLOW;return b->first;}
 b->until=now+60000U;
 if(b->observation.first.handles>128U||b->observation.first.physical_available<7239172096ULL||
  b->observation.first.commit_available<3221225472ULL){b->first=ERROR_NOT_ENOUGH_QUOTA;return b->first;}
 return 0;
}
DWORD papacc_ca_key_fixture_budget_check(PAPACC_CA_KEY *o,volatile LONG *stop)
{
 PAPACC_CA_FIXTURE_BUDGET *b;PAPACC_SCM_RESOURCE_SAMPLE *v;DWORD e;LARGE_INTEGER now;
 if(!o||!stop)return ERROR_INVALID_PARAMETER;b=&o->diagnostic_budget;
 if(!b->active||b->self!=b||b->thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if(b->first)return b->first;
 if(InterlockedCompareExchange(stop,0,0)){b->first=ERROR_CANCELLED;return b->first;}
 if(!b->until||GetTickCount64()>=b->until){b->first=ERROR_TIMEOUT;return b->first;}
 e=personal_resource_collect(&b->observation,2U,0);if(e){b->first=e;return e;}v=&b->observation.last;
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0||(ULONGLONG)now.QuadPart<v->end||
  v->frequency>((ULONGLONG)-1)/100U||((ULONGLONG)now.QuadPart-v->begin)>v->frequency/10U||GetTickCount64()>=b->until){b->first=ERROR_TIMEOUT;return b->first;}
 if(v->handles>=188U||v->handles>192U||v->physical_available<7239172096ULL||v->commit_available<3221225472ULL||
  (v->private_bytes>b->observation.first.private_bytes&&v->private_bytes-b->observation.first.private_bytes>PAPACC_CA_FIXTURE_GROWTH_BYTES)||
  (v->working_set>b->observation.first.working_set&&v->working_set-b->observation.first.working_set>PAPACC_CA_FIXTURE_GROWTH_BYTES)){b->first=ERROR_NOT_ENOUGH_QUOTA;return b->first;}
 if(InterlockedCompareExchange(stop,0,0)){b->first=ERROR_CANCELLED;return b->first;}return 0;
}
#include "personal_scq_session_win32.h"
#include "personal_scq_start_assurance_win32.h"
static DWORD ca_machine_read_check(PAPACC_CA_KEY *,volatile LONG *);
static SECURITY_STATUS ca_fixture_guard(PAPACC_CA_KEY *o)
{
 DWORD e;
 if(!o->diagnostic_only)return 0;
 if(!o->diagnostic_stop||InterlockedCompareExchange(o->diagnostic_stop,0,0))return ca_local(o,CA_D_VALIDATE,CA_D_ENTRY,NTE_USER_CANCELLED);
 if(o->read_session){e=ca_machine_read_check(o,o->diagnostic_stop);return e?ca_local(o,CA_D_VALIDATE,CA_D_ENTRY,e==ERROR_CANCELLED?NTE_USER_CANCELLED:NTE_NO_MEMORY):0;}
 if(!o->diagnostic_budget.active)return 0; /* component primitive: caller still owes external admission */
 e=papacc_ca_key_fixture_budget_check(o,o->diagnostic_stop);
 return e?ca_local(o,CA_D_VALIDATE,CA_D_ENTRY,e==ERROR_CANCELLED?NTE_USER_CANCELLED:NTE_NO_MEMORY):0;
}
#define CA_FIXTURE_CALL(o,call) (ca_fixture_guard((o))?((o)->first_failure.present?(o)->first_failure.original_code:NTE_FAIL):(call))
static int ca_nonzero(const BYTE *p) {
 unsigned i;BYTE v=0;for(i=0;i<16;i++)v=(BYTE)(v|p[i]);return v!=0;
}
static SECURITY_STATUS ca_identity(PAPACC_CA_KEY *o,PSID admin,PSID service) {
 BYTE storage[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];DWORD n=0,e;
 HANDLE token=NULL;PTOKEN_USER user=(PTOKEN_USER)storage;int same,info;
 if(!IsValidSid(admin)||!IsValidSid(service)||EqualSid(admin,service))return ca_local(o,CA_D_IDENTITY,CA_D_SID_VALID,NTE_PERM);
 if(!OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&token)) {
  e=GetLastError();if(e!=ERROR_NO_TOKEN)return ca_win32(o,CA_D_IDENTITY,CA_D_THREAD_TOKEN,e,NTE_PERM);
  if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return ca_win32(o,CA_D_IDENTITY,CA_D_PROCESS_TOKEN,GetLastError(),NTE_PERM);
 }
 info=GetTokenInformation(token,TokenUser,storage,sizeof(storage),&n);
 if(!info)(void)ca_win32(o,CA_D_IDENTITY,CA_D_TOKEN_INFO,GetLastError(),NTE_PERM);
 same=info&&EqualSid(user->User.Sid,admin);
 if(!CloseHandle(token)){e=GetLastError();o->retained_token=token;o->state=PAPACC_CA_KEY_RETAINED;o->error=NTE_FAIL;return ca_win32(o,CA_D_IDENTITY,CA_D_TOKEN_CLOSE,e,NTE_FAIL);}
 if(!same)return ca_local(o,CA_D_IDENTITY,CA_D_IDENTITY_EQUAL,NTE_PERM);
 return ERROR_SUCCESS;
}
void papacc_ca_key_name_data(wchar_t *out,const BYTE *a,const BYTE *g) {
 static const wchar_t digits[]=L"0123456789abcdef";
 const wchar_t *prefix=L"PapinhoAccelerator.CA.";unsigned i,n=0;
 while(*prefix)out[n++]=*prefix++;
 for(i=0;i<16;i++){out[n++]=digits[a[i]>>4];out[n++]=digits[a[i]&15];}
 out[n++]=L'.';
 for(i=0;i<16;i++){out[n++]=digits[g[i]>>4];out[n++]=digits[g[i]&15];}out[n]=0;
}
static SECURITY_STATUS ca_property(PAPACC_CA_KEY *o,LPCWSTR name,DWORD expected,DWORD stage) {
 DWORD value=0,n=0;SECURITY_STATUS s=ca_native(o,CA_D_PROPERTY,stage,CA_API_GET_PROPERTY,
  CA_FIXTURE_CALL(o,NCryptGetProperty(o->key,name,(PBYTE)&value,sizeof(value),&n,0)));
 if(s!=ERROR_SUCCESS)return s;
 return n==sizeof(value)&&value==expected?ERROR_SUCCESS:ca_local(o,CA_D_PROPERTY,stage==CA_D_GET_LENGTH?CA_D_LENGTH_VALUE:stage==CA_D_GET_EXPORT?CA_D_EXPORT_VALUE:CA_D_USAGE_VALUE,NTE_BAD_KEY_STATE);
}
static SECURITY_STATUS ca_acl(PAPACC_CA_KEY *o) {
 BYTE descriptor[1024];DWORD n=0,i;PACL acl=NULL;BOOL present=FALSE,def=FALSE;
 DWORD admin_allow=0,service_deny=0;BYTE system[SECURITY_MAX_SID_SIZE];DWORD z=sizeof(system);
 SECURITY_STATUS s=ca_native(o,CA_D_ACL,CA_D_GET_DACL,CA_API_GET_PROPERTY,
  CA_FIXTURE_CALL(o,NCryptGetProperty(o->key,NCRYPT_SECURITY_DESCR_PROPERTY,descriptor,sizeof(descriptor),&n,DACL_SECURITY_INFORMATION|(o->diagnostic_only?OWNER_SECURITY_INFORMATION:0))));
 if(s!=ERROR_SUCCESS)return s;
 if(!n||!IsValidSecurityDescriptor(descriptor)||!GetSecurityDescriptorDacl(descriptor,&present,&acl,&def)||!present||!acl||
    !CreateWellKnownSid(WinLocalSystemSid,NULL,system,&z))return ca_local(o,CA_D_ACL,CA_D_SD_VALID,NTE_BAD_KEY_STATE);
 if(o->diagnostic_only){PSID key_owner=NULL;SECURITY_DESCRIPTOR_CONTROL control;DWORD revision;
  if(!GetSecurityDescriptorOwner(descriptor,&key_owner,&def)||!key_owner||!EqualSid(key_owner,o->operator_sid)||
   !GetSecurityDescriptorControl(descriptor,&control,&revision)||!(control&SE_DACL_PROTECTED)||acl->AceCount!=3U)return ca_local(o,CA_D_ACL,CA_D_SD_VALID,NTE_PERM);
 }
 for(i=0;i<acl->AceCount;i++) {
  PACE_HEADER h;ACCESS_ALLOWED_ACE *ace;PSID sid;
  if(!GetAce(acl,i,(LPVOID *)&h))return ca_local(o,CA_D_ACL,CA_D_ACE_READ,NTE_BAD_KEY_STATE);
  if(o->diagnostic_only&&h->AceFlags)return ca_local(o,CA_D_ACL,CA_D_ACE_FLAGS,NTE_PERM);
  if(h->AceFlags&INHERIT_ONLY_ACE)return ca_local(o,CA_D_ACL,CA_D_ACE_FLAGS,NTE_BAD_KEY_STATE);
  if(h->AceType!=ACCESS_ALLOWED_ACE_TYPE&&h->AceType!=ACCESS_DENIED_ACE_TYPE)return ca_local(o,CA_D_ACL,CA_D_ACE_TYPE,NTE_BAD_KEY_STATE);
  ace=(ACCESS_ALLOWED_ACE *)h;sid=(PSID)&ace->SidStart;
  if(!IsValidSid(sid))return ca_local(o,CA_D_ACL,CA_D_ACE_SID,NTE_BAD_KEY_STATE);
  /* S1: two exact coherent readbacks only; setter and ordinary CA unchanged. */
  if(o->diagnostic_only&&((ace->Mask!=GENERIC_ALL&&ace->Mask!=0xd01f01ffU)||(i>0U&&ace->Mask!=service_deny)||
   (i==0U?(h->AceType!=ACCESS_DENIED_ACE_TYPE||!EqualSid(sid,o->service_sid)):
    h->AceType!=ACCESS_ALLOWED_ACE_TYPE||!EqualSid(sid,i==1U?o->operator_sid:system))))return ca_local(o,CA_D_ACL,CA_D_ACE_MASK,NTE_PERM);
  if(h->AceType==ACCESS_ALLOWED_ACE_TYPE) {
   if(!EqualSid(sid,o->operator_sid)&&!EqualSid(sid,system))return ca_local(o,CA_D_ACL,CA_D_ACE_ALLOWED,NTE_PERM);
   if(EqualSid(sid,o->operator_sid))admin_allow|=ace->Mask;
  } else if(EqualSid(sid,o->service_sid))service_deny|=ace->Mask;
 }
 if(!(admin_allow&GENERIC_ALL)||!(service_deny&GENERIC_ALL))return ca_local(o,CA_D_ACL,CA_D_ACE_MASK,NTE_PERM);
 return ERROR_SUCCESS;
}
SECURITY_STATUS papacc_ca_key_validate(PAPACC_CA_KEY *o) {
 BYTE blob[512];BCRYPT_RSAKEY_BLOB *b=(BCRYPT_RSAKEY_BLOB *)blob;DWORD n=0;
 SECURITY_STATUS s;
 if(o&&o->diagnostic_only&&(!o->diagnostic_stop||InterlockedCompareExchange(o->diagnostic_stop,0,0)))return ca_local(o,CA_D_VALIDATE,CA_D_ENTRY,NTE_USER_CANCELLED);
 if(!o||!o->key||o->thread!=GetCurrentThreadId()||o->state==PAPACC_CA_KEY_RETAINED||o->state==PAPACC_CA_KEY_CLOSED)return ca_local(o,CA_D_VALIDATE,CA_D_ENTRY,NTE_BAD_KEY_STATE);
 s=ca_identity(o,o->operator_sid,o->service_sid);if(s!=ERROR_SUCCESS)return s;
 s=ca_property(o,NCRYPT_LENGTH_PROPERTY,2048,CA_D_GET_LENGTH);if(s!=ERROR_SUCCESS)return s;
 s=ca_property(o,NCRYPT_EXPORT_POLICY_PROPERTY,0,CA_D_GET_EXPORT);if(s!=ERROR_SUCCESS)return s;
 s=ca_property(o,NCRYPT_KEY_USAGE_PROPERTY,NCRYPT_ALLOW_SIGNING_FLAG,CA_D_GET_USAGE);if(s!=ERROR_SUCCESS)return s;
 s=ca_native(o,CA_D_VALIDATE,CA_D_EXPORT_PUBLIC,CA_API_EXPORT_PUBLIC,CA_FIXTURE_CALL(o,NCryptExportKey(o->key,0,BCRYPT_RSAPUBLIC_BLOB,NULL,blob,sizeof(blob),&n,0)));
 if(s!=ERROR_SUCCESS)return s;
 if(n!=sizeof(*b)+3+256||b->Magic!=BCRYPT_RSAPUBLIC_MAGIC||b->BitLength!=2048||b->cbPublicExp!=3||b->cbModulus!=256||
    blob[sizeof(*b)]!=1||blob[sizeof(*b)+1]!=0||blob[sizeof(*b)+2]!=1)return ca_local(o,CA_D_VALIDATE,CA_D_PUBLIC_VALUE,NTE_BAD_KEY_STATE);
 return ca_acl(o);
}
SECURITY_STATUS papacc_ca_key_borrow(PAPACC_CA_KEY *o,const void *holder) {
 if(!o||!holder||o->diagnostic_only||o->thread!=GetCurrentThreadId()||o->state!=PAPACC_CA_KEY_READY||!o->key||!o->provider||o->borrower||o->close_attempted)
  return ca_local(o,CA_D_VALIDATE,CA_D_ENTRY,NTE_BAD_KEY_STATE);
 o->borrower=holder;return ERROR_SUCCESS;
}
SECURITY_STATUS papacc_ca_key_return(PAPACC_CA_KEY *o,const void *holder) {
 if(!o||!holder||o->diagnostic_only||o->thread!=GetCurrentThreadId()||o->state!=PAPACC_CA_KEY_READY||o->borrower!=holder)
  return ca_local(o,CA_D_CLOSE,CA_D_ENTRY,NTE_BAD_KEY_STATE);
 o->borrower=NULL;return ERROR_SUCCESS;
}
SECURITY_STATUS papacc_ca_key_transfer_borrow(PAPACC_CA_KEY *o,const void *from,const void *to) {
 if(!o||!from||!to||o->diagnostic_only||from==to||o->thread!=GetCurrentThreadId()||o->state!=PAPACC_CA_KEY_READY||
  !o->key||!o->provider||o->close_attempted||o->borrower!=from)
  return ca_local(o,CA_D_VALIDATE,CA_D_ENTRY,NTE_BAD_KEY_STATE);
 o->borrower=to;return ERROR_SUCCESS;
}
SECURITY_STATUS papacc_ca_key_close(PAPACC_CA_KEY *o) {
 SECURITY_STATUS s;
 if(!o||o->thread!=GetCurrentThreadId()||o->borrower)return ca_local(o,CA_D_CLOSE,CA_D_ENTRY,NTE_BAD_KEY_STATE);
 if(o->state==PAPACC_CA_KEY_RETAINED)return ca_local(o,CA_D_CLOSE,CA_D_ALREADY_RETAINED,o->error?o->error:NTE_FAIL);
 if(o->state==PAPACC_CA_KEY_CLOSED)return ERROR_SUCCESS;
 o->close_attempted=1;
 if(o->diagnostic_reopen){s=ca_native(o,CA_D_CLOSE,CA_D_FREE_KEY,CA_API_FREE_OBJECT,NCryptFreeObject(o->diagnostic_reopen));if(s!=ERROR_SUCCESS)goto retained;o->diagnostic_reopen=0;}
 if(o->key){s=ca_native(o,CA_D_CLOSE,CA_D_FREE_KEY,CA_API_FREE_OBJECT,NCryptFreeObject(o->key));if(s!=ERROR_SUCCESS)goto retained;o->key=0;}
 if(o->provider){s=ca_native(o,CA_D_CLOSE,CA_D_FREE_PROVIDER,CA_API_FREE_OBJECT,NCryptFreeObject(o->provider));if(s!=ERROR_SUCCESS)goto retained;o->provider=0;}
 if(o->retained_token){s=ca_local(o,CA_D_CLOSE,CA_D_RETAINED_TOKEN,NTE_FAIL);goto retained;}
 o->state=PAPACC_CA_KEY_CLOSED;o->close_confirmed=1;return ERROR_SUCCESS;
retained:
 o->state=PAPACC_CA_KEY_RETAINED;o->error=s;return s;
}
static SECURITY_STATUS ca_begin(PAPACC_CA_KEY *o,const BYTE *a,const BYTE *g,PSID admin,PSID service,int create) {
 SECURITY_STATUS s;DWORD fn=create?CA_D_BEGIN_CREATE:CA_D_BEGIN_OPEN;
#ifndef PAPACC_CA_READ_ONLY_DIAGNOSTIC
 DWORD n;BYTE sd[1024];LPWSTR as=NULL,ss=NULL;
 PSECURITY_DESCRIPTOR converted=NULL;wchar_t text[512];int written;
#else
 /* Diagnostic build cannot create or set key properties, even via this API. */
 if(create)return ca_local(o,fn,CA_D_ENTRY,NTE_NOT_SUPPORTED);
#endif
 if(!o||!a||!g||!admin||!service||o->state!=PAPACC_CA_KEY_EMPTY||o->key||o->provider||o->retained_token||
    !ca_nonzero(a)||!ca_nonzero(g))return ca_local(o,fn,CA_D_ENTRY,NTE_INVALID_PARAMETER);
 o->thread=GetCurrentThreadId();s=ca_identity(o,admin,service);if(s!=ERROR_SUCCESS)goto failure;
 o->operator_size=GetLengthSid(admin);o->service_size=GetLengthSid(service);
 if(o->operator_size>sizeof(o->operator_sid)||o->service_size>sizeof(o->service_sid)){s=ca_local(o,fn,CA_D_SID_SIZE,NTE_INVALID_PARAMETER);goto failure;}
 if(!CopySid(sizeof(o->operator_sid),o->operator_sid,admin)||!CopySid(sizeof(o->service_sid),o->service_sid,service)){s=ca_win32(o,fn,CA_D_SID_COPY,GetLastError(),NTE_FAIL);goto failure;}
 papacc_ca_key_name_data(o->name,a,g);
 s=ca_native(o,fn,CA_D_OPEN_PROVIDER,CA_API_OPEN_PROVIDER,CA_FIXTURE_CALL(o,NCryptOpenStorageProvider(&o->provider,MS_KEY_STORAGE_PROVIDER,0)));if(s!=ERROR_SUCCESS)goto failure;
 if(!create){s=ca_native(o,fn,CA_D_OPEN_KEY,CA_API_OPEN_KEY,CA_FIXTURE_CALL(o,NCryptOpenKey(o->provider,&o->key,o->name,0,NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG)));if(s!=ERROR_SUCCESS)goto failure;}
#ifndef PAPACC_CA_READ_ONLY_DIAGNOSTIC
 else {
  /* No NCRYPT_OVERWRITE_KEY_FLAG. Existing key is never replaced. */
  s=ca_native(o,fn,CA_D_CREATE_KEY,CA_API_CREATE_KEY,CA_FIXTURE_CALL(o,NCryptCreatePersistedKey(o->provider,&o->key,NCRYPT_RSA_ALGORITHM,o->name,0,NCRYPT_MACHINE_KEY_FLAG)));
  if(s!=ERROR_SUCCESS)goto failure;o->created=1;
  n=2048;s=ca_native(o,fn,CA_D_SET_LENGTH,CA_API_SET_PROPERTY,CA_FIXTURE_CALL(o,NCryptSetProperty(o->key,NCRYPT_LENGTH_PROPERTY,(PBYTE)&n,sizeof(n),0)));if(s!=ERROR_SUCCESS)goto failure;
  n=0;s=ca_native(o,fn,CA_D_SET_EXPORT,CA_API_SET_PROPERTY,CA_FIXTURE_CALL(o,NCryptSetProperty(o->key,NCRYPT_EXPORT_POLICY_PROPERTY,(PBYTE)&n,sizeof(n),0)));if(s!=ERROR_SUCCESS)goto failure;
  n=NCRYPT_ALLOW_SIGNING_FLAG;s=ca_native(o,fn,CA_D_SET_USAGE,CA_API_SET_PROPERTY,CA_FIXTURE_CALL(o,NCryptSetProperty(o->key,NCRYPT_KEY_USAGE_PROPERTY,(PBYTE)&n,sizeof(n),0)));if(s!=ERROR_SUCCESS)goto failure;
  if(!ConvertSidToStringSidW(admin,&as)||!ConvertSidToStringSidW(service,&ss)){s=ca_win32(o,fn,CA_D_SID_TEXT,GetLastError(),NTE_FAIL);goto failure;}
  written=o->diagnostic_only?
   _snwprintf_s(text,sizeof(text)/sizeof(text[0]),_TRUNCATE,L"O:%sG:SYD:P(D;;GA;;;%s)(A;;GA;;;%s)(A;;GA;;;SY)",as,ss,as):
   _snwprintf_s(text,sizeof(text)/sizeof(text[0]),_TRUNCATE,L"D:P(D;;GA;;;%s)(A;;GA;;;%s)(A;;GA;;;SY)",ss,as);
  if(written<0){s=ca_local(o,fn,CA_D_SD_FORMAT,NTE_FAIL);goto failure;}
  if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(text,SDDL_REVISION_1,&converted,&n)){s=ca_win32(o,fn,CA_D_SD_PARSE,GetLastError(),NTE_FAIL);goto failure;}
  if(n>sizeof(sd)){s=ca_local(o,fn,CA_D_SD_SIZE,NTE_FAIL);goto failure;}
  memcpy(sd,converted,n);
  s=ca_native(o,fn,CA_D_SET_DACL,CA_API_SET_PROPERTY,CA_FIXTURE_CALL(o,NCryptSetProperty(o->key,NCRYPT_SECURITY_DESCR_PROPERTY,sd,n,DACL_SECURITY_INFORMATION|NCRYPT_PERSIST_FLAG|(o->diagnostic_only?OWNER_SECURITY_INFORMATION:0))));if(s!=ERROR_SUCCESS)goto failure;
  o->diagnostic_finalization=1;
  s=ca_native(o,fn,CA_D_FINALIZE,CA_API_FINALIZE,CA_FIXTURE_CALL(o,NCryptFinalizeKey(o->key,NCRYPT_SILENT_FLAG)));if(s!=ERROR_SUCCESS)goto failure;
  o->diagnostic_finalization=2;
 }
#endif
 s=papacc_ca_key_validate(o);if(s!=ERROR_SUCCESS)goto failure;
 o->state=PAPACC_CA_KEY_READY;
failure:
#ifndef PAPACC_CA_READ_ONLY_DIAGNOSTIC
 if(as&&LocalFree(as)){o->retained_local[0]=(HLOCAL)as;o->state=PAPACC_CA_KEY_RETAINED;s=ca_local(o,fn,CA_D_LOCAL_FREE_ADMIN,NTE_FAIL);}
 if(ss&&LocalFree(ss)){o->retained_local[1]=(HLOCAL)ss;o->state=PAPACC_CA_KEY_RETAINED;s=ca_local(o,fn,CA_D_LOCAL_FREE_SERVICE,NTE_FAIL);}
 if(converted&&LocalFree(converted)){o->retained_local[2]=(HLOCAL)converted;o->state=PAPACC_CA_KEY_RETAINED;s=ca_local(o,fn,CA_D_LOCAL_FREE_SD,NTE_FAIL);}
#endif
 if(s!=ERROR_SUCCESS){o->error=s;if(o->retained_token)o->state=PAPACC_CA_KEY_RETAINED;else if(o->state!=PAPACC_CA_KEY_RETAINED)(void)papacc_ca_key_close(o);}
 return s;
}
SECURITY_STATUS papacc_ca_key_create(PAPACC_CA_KEY *o,const BYTE a[16],const BYTE g[16],PSID admin,PSID service) {
 return ca_begin(o,a,g,admin,service,1);
}
SECURITY_STATUS papacc_ca_key_open(PAPACC_CA_KEY *o,const BYTE a[16],const BYTE g[16],PSID admin,PSID service) {
 if(o&&(o->diagnostic_only||o->diagnostic_budget.active))return NTE_INVALID_PARAMETER;
 return ca_begin(o,a,g,admin,service,0);
}

/* K2/K7: bound local lifetime, never a callback or metadata grant. The native
 * root must validate full originals/both peers before and after this owner.
 * Every provider/property/public call refreshes the SAME original G3 baseline. */
static DWORD ca_machine_read_check(PAPACC_CA_KEY *o,volatile LONG *stop)
{
 const PAPACC_SCQ_SESSION_DRIVER *c=o->read_session;const PAPACC_SCQ_READ_DIAGNOSTIC *k=o->read_act;
 PAPACC_SCQ_START_ASSURANCE *g;PAPACC_SCM_RESOURCE_SAMPLE *s;LARGE_INTEGER now;DWORD e;
 if(!stop||*stop)return ERROR_CANCELLED;
 if(!c||!k||!o->read_owner||c->self!=c||c->owner!=o->read_owner||c->thread!=GetCurrentThreadId()||c->role!=1U||c->phase!=3U||c->first||c->peer_lost||
  !c->profile||c->profile->read_diagnostic!=1U||c->profile->pre_readiness!=1U||!c->runtime||c->runtime->read_diagnostic!=1U||c->runtime->closed_preparation!=1U||
  k->self!=k||k->owner!=o->read_owner||k->thread!=GetCurrentThreadId()||k->phase!=3U||k->first||k->consumed!=1||k->source!=c->source||
  !k->source||k->source->phase!=4U||!k->source->value.controller_consumed||!k->source->endpoint_consumed||!k->source->start_consumed||
  !k->source->registration||k->source->registration->read_diagnostic!=1U||k->source->registration->purpose!=PAPACC_SCQ_PURPOSE_READ||
  c->transport.phase!=1U||!c->transport.query_refs3||c->transport.query_only||!c->transport.read_diagnostic||
  !(c->transport.legs&1U)||!(c->transport.legs&4096U)||!c->transport.pipe||!c->transport.event||o->diagnostic_budget.active)return ERROR_ACCESS_DENIED;
 g=c->transport.resource_scope;
 if(!g||g->self!=g||g->owner!=o->read_owner||g->source!=k->source||g->thread!=GetCurrentThreadId()||g->phase!=2U||g->first||!g->read_diagnostic||
  !g->p1_handles||!g->initial_known||g->initial_handles>128U||g->limits.max_handles!=192U||g->limits.private_growth!=1994944U||g->limits.working_growth!=3145728U||
  g->limits.physical_floor!=7239172096ULL||g->limits.commit_floor!=3221225472ULL||g->limits.freshness_ms!=100U||g->limits.duration_ms!=60000U||
  !g->global_until||g->global_until!=c->transport.global_until||g->global_until>k->until||GetTickCount64()>=g->global_until)return ERROR_ACCESS_DENIED;
 e=personal_resource_collect(&g->observations,2U,0);if(e)return e;s=&g->observations.last;
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0||!s->frequency||s->frequency!=g->ready.frequency||s->begin>s->end||(ULONGLONG)now.QuadPart<s->end||
  (ULONGLONG)now.QuadPart-s->begin>s->frequency/10U||s->begin<g->ready.end||GetTickCount64()>=g->global_until)return ERROR_TIMEOUT;
 if(s->handles>=188U||s->handles>192U||s->physical_available<g->limits.physical_floor||s->commit_available<g->limits.commit_floor||
  (s->private_bytes>g->ready.private_bytes&&s->private_bytes-g->ready.private_bytes>g->limits.private_growth)||
  (s->working_set>g->ready.working_set&&s->working_set-g->ready.working_set>g->limits.working_growth))return ERROR_NOT_ENOUGH_QUOTA;
 return *stop?ERROR_CANCELLED:0;
}
DWORD papacc_ca_key_machine_read_bind(PAPACC_CA_KEY *o,const PAPACC_SCQ_SESSION_DRIVER *c,const PAPACC_SCQ_READ_DIAGNOSTIC *k,const void *owner,volatile LONG *stop)
{
 DWORD e;if(!o||o->state!=PAPACC_CA_KEY_EMPTY||o->diagnostic_only||o->read_session||o->provider||o->key||o->diagnostic_budget.active)return ERROR_INVALID_STATE;
 o->read_session=c;o->read_act=k;o->read_owner=owner;o->diagnostic_stop=stop;o->diagnostic_only=1U;
 e=ca_machine_read_check(o,stop);if(e)o->error=NTE_BAD_KEY_STATE;return e; /* never rearmed */
}
SECURITY_STATUS papacc_ca_key_fixture_read_open(PAPACC_CA_KEY *o,const BYTE a[16],const BYTE g[16],PSID admin,PSID service)
{
 if(!o||!o->diagnostic_only||!o->read_session||o->error||o->created||o->thread||o->provider||o->key||ca_machine_read_check(o,o->diagnostic_stop))return NTE_BAD_KEY_STATE;
 return ca_begin(o,a,g,admin,service,0);
}
/* K1 fixture is explicitly test-only even after a partial create. No API here
 * creates a certificate/root, signs, deletes or changes an existing key. */
SECURITY_STATUS papacc_ca_key_fixture_create(PAPACC_CA_KEY *o,const BYTE a[16],const BYTE g[16],PSID admin,PSID service,volatile LONG *stop)
{
 if(!o||!stop||o->diagnostic_only||o->state!=PAPACC_CA_KEY_EMPTY||o->provider||o->key||o->diagnostic_reopen||o->borrower)return NTE_INVALID_PARAMETER;
 o->diagnostic_only=1U;o->diagnostic_stop=stop;
 if(InterlockedCompareExchange(stop,0,0))return ca_local(o,CA_D_BEGIN_CREATE,CA_D_ENTRY,NTE_USER_CANCELLED);

 return ca_begin(o,a,g,admin,service,1);
}
/* Retain original key while positively reopening the same MACHINE name.
 * Return only public identity bytes. Caller hashes/binds them with existing
 * bounded SHA256 machinery; no public bytes alone become an authorization. */
SECURITY_STATUS papacc_ca_key_fixture_control(PAPACC_CA_KEY *o,BYTE *public_blob,DWORD capacity,DWORD *bytes)
{
 BYTE original[283],observed[283];DWORD n=0,m=0;SECURITY_STATUS s,closed;
 if(!o||!public_blob||!bytes||capacity<sizeof(original)||!o->diagnostic_only||o->state!=PAPACC_CA_KEY_READY||
  o->thread!=GetCurrentThreadId()||o->borrower||o->diagnostic_reopen||o->diagnostic_control_attempted||o->close_attempted)return NTE_INVALID_PARAMETER;
 *bytes=0;o->diagnostic_control_attempted=1U;s=papacc_ca_key_validate(o);if(s)return s;
 s=ca_native(o,CA_D_VALIDATE,CA_D_EXPORT_PUBLIC,CA_API_EXPORT_PUBLIC,CA_FIXTURE_CALL(o,NCryptExportKey(o->key,0,BCRYPT_RSAPUBLIC_BLOB,NULL,original,sizeof(original),&n,0)));
 if(s)return s;if(n!=sizeof(original))return NTE_BAD_KEY_STATE;
 s=ca_native(o,CA_D_BEGIN_OPEN,CA_D_OPEN_KEY,CA_API_OPEN_KEY,CA_FIXTURE_CALL(o,NCryptOpenKey(o->provider,&o->diagnostic_reopen,o->name,0,NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG)));
 if(s||!o->diagnostic_reopen){if(o->diagnostic_reopen){o->state=PAPACC_CA_KEY_RETAINED;o->error=s?s:NTE_INVALID_HANDLE;}return s?s:NTE_INVALID_HANDLE;}
 s=ca_native(o,CA_D_VALIDATE,CA_D_EXPORT_PUBLIC,CA_API_EXPORT_PUBLIC,CA_FIXTURE_CALL(o,NCryptExportKey(o->diagnostic_reopen,0,BCRYPT_RSAPUBLIC_BLOB,NULL,observed,sizeof(observed),&m,0)));
 if(!s&&(m!=n||memcmp(original,observed,n)))s=ca_local(o,CA_D_VALIDATE,CA_D_PUBLIC_VALUE,NTE_BAD_KEY_STATE);
 closed=ca_native(o,CA_D_CLOSE,CA_D_FREE_KEY,CA_API_FREE_OBJECT,NCryptFreeObject(o->diagnostic_reopen));
 if(closed){o->state=PAPACC_CA_KEY_RETAINED;o->error=closed;return s?s:closed;}
 o->diagnostic_reopen=0;if(s)return s;
 s=papacc_ca_key_validate(o);if(s)return s;
 memcpy(public_blob,original,n);*bytes=n;o->diagnostic_control_confirmed=1U;return ERROR_SUCCESS;
}

#undef CA_FIXTURE_CALL
