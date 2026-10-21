/* Offline fault model: every NCrypt call is replaced before backend inclusion.
 * This executable must import no NCrypt API. Token queries are read-only.
 * No private material, native key creation, DACL update or certificate. */
#define _WIN32_WINNT 0x0600
#include "server_ca_key_win32.h"
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
static DWORD fail_stage,trace[64],trace_count,cleanup_fail,local_fault,passed;
static SECURITY_STATUS injected;
static BYTE sd_bytes[1024];static DWORD sd_size;
static SECURITY_STATUS model(DWORD stage) {
 if(trace_count<64)trace[trace_count++]=stage;
 if(stage==fail_stage)return injected;
 if(stage==CA_D_FREE_KEY&&cleanup_fail)return NTE_BAD_KEYSET;
 return ERROR_SUCCESS;
}
SECURITY_STATUS WINAPI fake_provider(NCRYPT_PROV_HANDLE *out,LPCWSTR name,DWORD flags) {
 SECURITY_STATUS s=model(CA_D_OPEN_PROVIDER);(void)name;(void)flags;if(!s)*out=0x101;return s;
}
SECURITY_STATUS WINAPI fake_create(NCRYPT_PROV_HANDLE p,NCRYPT_KEY_HANDLE *out,LPCWSTR algorithm,LPCWSTR name,DWORD spec,DWORD flags) {
 SECURITY_STATUS s=model(CA_D_CREATE_KEY);(void)p;(void)algorithm;(void)name;(void)spec;(void)flags;if(!s)*out=0x202;return s;
}
SECURITY_STATUS WINAPI fake_open(NCRYPT_PROV_HANDLE p,NCRYPT_KEY_HANDLE *out,LPCWSTR name,DWORD spec,DWORD flags) {
 SECURITY_STATUS s=model(CA_D_OPEN_KEY);(void)p;(void)name;(void)spec;(void)flags;if(!s)*out=0x202;return s;
}
SECURITY_STATUS WINAPI fake_set(NCRYPT_HANDLE key,LPCWSTR name,PBYTE bytes,DWORD size,DWORD flags) {
 DWORD stage;SECURITY_STATUS s;(void)key;(void)flags;
 stage=!wcscmp(name,NCRYPT_LENGTH_PROPERTY)?CA_D_SET_LENGTH:!wcscmp(name,NCRYPT_EXPORT_POLICY_PROPERTY)?CA_D_SET_EXPORT:
  !wcscmp(name,NCRYPT_KEY_USAGE_PROPERTY)?CA_D_SET_USAGE:CA_D_SET_DACL;
 s=model(stage);if(s)return s;
 if(stage==CA_D_SET_DACL){if(size>sizeof(sd_bytes))return NTE_BAD_LEN;memcpy(sd_bytes,bytes,size);sd_size=size;}
 return ERROR_SUCCESS;
}
SECURITY_STATUS WINAPI fake_finalize(NCRYPT_KEY_HANDLE key,DWORD flags) {
 (void)key;(void)flags;return model(CA_D_FINALIZE);
}
SECURITY_STATUS WINAPI fake_get(NCRYPT_HANDLE key,LPCWSTR name,PBYTE out,DWORD cap,DWORD *size,DWORD flags) {
 DWORD stage,value;SECURITY_STATUS s;(void)key;(void)flags;
 stage=!wcscmp(name,NCRYPT_LENGTH_PROPERTY)?CA_D_GET_LENGTH:!wcscmp(name,NCRYPT_EXPORT_POLICY_PROPERTY)?CA_D_GET_EXPORT:
  !wcscmp(name,NCRYPT_KEY_USAGE_PROPERTY)?CA_D_GET_USAGE:CA_D_GET_DACL;
 s=model(stage);if(s)return s;
 if(stage==CA_D_GET_DACL){
  PACL acl;BOOL present,def;DWORD i;
  if(cap<sd_size)return NTE_BUFFER_TOO_SMALL;memcpy(out,sd_bytes,sd_size);*size=sd_size;
  if(local_fault==2&&GetSecurityDescriptorDacl(out,&present,&acl,&def)) {
   for(i=0;i<acl->AceCount;i++){ACCESS_ALLOWED_ACE *ace;if(GetAce(acl,i,(LPVOID *)&ace)&&ace->Header.AceType==ACCESS_ALLOWED_ACE_TYPE)ace->Mask=0;}
  }
 }else{
  value=stage==CA_D_GET_LENGTH?(local_fault==1?1024U:2048U):stage==CA_D_GET_EXPORT?0U:NCRYPT_ALLOW_SIGNING_FLAG;
  if(cap<sizeof(value))return NTE_BUFFER_TOO_SMALL;memcpy(out,&value,sizeof(value));*size=sizeof(value);
 }
 return ERROR_SUCCESS;
}
SECURITY_STATUS WINAPI fake_export(NCRYPT_KEY_HANDLE key,NCRYPT_KEY_HANDLE wrap,LPCWSTR type,NCryptBufferDesc *params,
 PBYTE out,DWORD cap,DWORD *size,DWORD flags) {
 SECURITY_STATUS s=model(CA_D_EXPORT_PUBLIC);BCRYPT_RSAKEY_BLOB *b;
 (void)key;(void)wrap;(void)type;(void)params;(void)flags;if(s)return s;
 *size=(DWORD)sizeof(*b)+3+256;if(cap<*size)return NTE_BUFFER_TOO_SMALL;
 memset(out,0,*size);b=(BCRYPT_RSAKEY_BLOB *)out;b->Magic=BCRYPT_RSAPUBLIC_MAGIC;
 b->BitLength=2048;b->cbPublicExp=3;b->cbModulus=256;out[sizeof(*b)]=1;out[sizeof(*b)+2]=(BYTE)(local_fault==3?3:1);
 return ERROR_SUCCESS;
}
SECURITY_STATUS WINAPI fake_free(NCRYPT_HANDLE key) {
 return model(key==0x202?CA_D_FREE_KEY:CA_D_FREE_PROVIDER);
}
#define NCryptOpenStorageProvider fake_provider
#define NCryptCreatePersistedKey fake_create
#define NCryptOpenKey fake_open
#define NCryptSetProperty fake_set
#define NCryptFinalizeKey fake_finalize
#define NCryptGetProperty fake_get
#define NCryptExportKey fake_export
#define NCryptFreeObject fake_free
#include "../apps/server/server_ca_key_win32.c"
#define CHECK(x) do{if(!(x)){printf("FAIL line=%u expression=%s\n",(unsigned)__LINE__,#x);return 1;}passed++;}while(0)
static void reset(void) {fail_stage=0;trace_count=0;cleanup_fail=0;local_fault=0;sd_size=0;injected=NTE_PERM;}
SECURITY_STATUS ca_baseline_run(const BYTE[16],const BYTE[16],PSID,PSID,DWORD[8]);
static int compare_preimage(const BYTE a[16],const BYTE g[16],PSID admin,PSID service) {
 PAPACC_CA_KEY o;DWORD prior=fail_stage,cf=cleanup_fail,lf=local_fault,old[8],now[8],saved[64],count,i;
 SECURITY_STATUS code=injected,s,b;
 memset(&o,0,sizeof(o));s=papacc_ca_key_create(&o,a,g,admin,service);
 if(s==ERROR_SUCCESS)s=papacc_ca_key_close(&o);
 now[0]=(DWORD)o.state;now[1]=(DWORD)o.created;now[2]=(DWORD)o.close_attempted;now[3]=(DWORD)o.close_confirmed;
 now[4]=(DWORD)o.error;now[5]=o.key!=0;now[6]=o.provider!=0;now[7]=o.retained_token!=0;
 count=trace_count;memcpy(saved,trace,sizeof(saved));
 reset();fail_stage=prior;cleanup_fail=cf;local_fault=lf;injected=code;
 b=ca_baseline_run(a,g,admin,service,old);
 CHECK(s==b);CHECK(memcmp(now,old,sizeof(now))==0);CHECK(count==trace_count);
 for(i=0;i<count;i++)CHECK(saved[i]==trace[i]);
 return 0;
}
int main(void) {
 BYTE storage[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE],service[SECURITY_MAX_SID_SIZE];
 DWORD bytes=sizeof(service),got=0,i,j;HANDLE token=NULL;PSID admin;BYTE a[16]={1},g[16]={2};
 PAPACC_CA_KEY o;PAPACC_CA_FAILURE first;SECURITY_STATUS s;
 static const DWORD stages[]={CA_D_OPEN_PROVIDER,CA_D_CREATE_KEY,CA_D_SET_LENGTH,CA_D_SET_EXPORT,CA_D_SET_USAGE,
 CA_D_SET_DACL,CA_D_FINALIZE,CA_D_GET_LENGTH,CA_D_GET_EXPORT,CA_D_GET_USAGE,CA_D_EXPORT_PUBLIC,CA_D_GET_DACL};
 CHECK(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token));
 CHECK(GetTokenInformation(token,TokenUser,storage,sizeof(storage),&got));admin=((PTOKEN_USER)storage)->User.Sid;
 CHECK(CloseHandle(token));CHECK(CreateWellKnownSid(WinLocalServiceSid,NULL,service,&bytes));CHECK(!EqualSid(admin,service));
 /* Each native stage shares NTE_PERM but must retain exact origin/API/stage. */
 for(i=0;i<sizeof(stages)/sizeof(stages[0]);i++){
  reset();fail_stage=stages[i];memset(&o,0,sizeof(o));
  s=papacc_ca_key_create(&o,a,g,admin,service);
  CHECK(s==NTE_PERM);CHECK(o.first_failure.present);CHECK(o.first_failure.origin==CA_D_NATIVE_NCRYPT);
  CHECK(o.first_failure.stage==stages[i]);
  CHECK(o.first_failure.native_api==(DWORD)(i==0?CA_API_OPEN_PROVIDER:i==1?CA_API_CREATE_KEY:i<=5?CA_API_SET_PROPERTY:i==6?CA_API_FINALIZE:i==10?CA_API_EXPORT_PUBLIC:CA_API_GET_PROPERTY));
  CHECK(o.first_failure.function==(DWORD)(i<=6?CA_D_BEGIN_CREATE:i<=9?CA_D_PROPERTY:i==10?CA_D_VALIDATE:CA_D_ACL));
  CHECK(o.first_failure.key_owned==(i>1?1U:0U));CHECK(o.first_failure.provider_owned==(i>0?1U:0U));
  CHECK(o.first_failure.ownership==(i>0?1U:0U));CHECK(o.first_failure.state==PAPACC_CA_KEY_EMPTY);
  CHECK(o.first_failure.original_code==NTE_PERM);CHECK(o.first_failure.backend_code==s);
  CHECK(o.first_failure.created==(i>1?1U:0U));
  CHECK(o.first_failure.finalization==(i<6?0U:i==6?1U:2U));
  CHECK(o.state==PAPACC_CA_KEY_CLOSED);CHECK(!o.key&&!o.provider);
  for(j=0;j<=i;j++)CHECK(trace[j]==stages[j]);
  first=o.first_failure;CHECK(papacc_ca_key_validate(&o)==NTE_BAD_KEY_STATE);
  CHECK(memcmp(&first,&o.first_failure,sizeof(first))==0);
 }
 /* Original native status is not normalized to NTE_PERM. */
 reset();fail_stage=CA_D_SET_EXPORT;injected=NTE_BAD_FLAGS;memset(&o,0,sizeof(o));
 CHECK(papacc_ca_key_create(&o,a,g,admin,service)==NTE_BAD_FLAGS);CHECK(o.first_failure.original_code==NTE_BAD_FLAGS);
 /* Later cleanup failure must not overwrite first failure or change retention. */
 reset();fail_stage=CA_D_SET_DACL;cleanup_fail=1;memset(&o,0,sizeof(o));
 CHECK(papacc_ca_key_create(&o,a,g,admin,service)==NTE_PERM);CHECK(o.state==PAPACC_CA_KEY_RETAINED);
 CHECK(o.key==0x202&&o.provider==0x101);CHECK(o.first_failure.stage==CA_D_SET_DACL);
 CHECK(o.first_failure.original_code==NTE_PERM);first=o.first_failure;
 CHECK(papacc_ca_key_close(&o)==NTE_BAD_KEYSET);CHECK(memcmp(&first,&o.first_failure,sizeof(first))==0);
 /* First failure during close captures native ownership before retention. */
 reset();memset(&o,0,sizeof(o));CHECK(papacc_ca_key_create(&o,a,g,admin,service)==ERROR_SUCCESS);
 cleanup_fail=1;CHECK(papacc_ca_key_close(&o)==NTE_BAD_KEYSET);
 CHECK(o.first_failure.stage==CA_D_FREE_KEY&&o.first_failure.function==CA_D_CLOSE);
 CHECK(o.first_failure.origin==CA_D_NATIVE_NCRYPT&&o.first_failure.native_api==CA_API_FREE_OBJECT);
 CHECK(o.first_failure.original_code==NTE_BAD_KEYSET&&o.first_failure.state==PAPACC_CA_KEY_READY);
 CHECK(o.first_failure.key_owned&&o.first_failure.provider_owned&&o.first_failure.ownership==1);
 CHECK(o.state==PAPACC_CA_KEY_RETAINED);
 /* Read/open failure and provider-close failure have distinct native receipts. */
 reset();fail_stage=CA_D_OPEN_KEY;memset(&o,0,sizeof(o));
 CHECK(papacc_ca_key_open(&o,a,g,admin,service)==NTE_PERM);
 CHECK(o.first_failure.stage==CA_D_OPEN_KEY&&o.first_failure.native_api==CA_API_OPEN_KEY);
 CHECK(o.first_failure.function==CA_D_BEGIN_OPEN&&o.first_failure.origin==CA_D_NATIVE_NCRYPT);
 CHECK(!o.first_failure.created&&!o.first_failure.key_owned&&o.first_failure.provider_owned);
 reset();memset(&o,0,sizeof(o));CHECK(papacc_ca_key_create(&o,a,g,admin,service)==ERROR_SUCCESS);
 fail_stage=CA_D_FREE_PROVIDER;injected=NTE_BAD_FLAGS;
 CHECK(papacc_ca_key_close(&o)==NTE_BAD_FLAGS);
 CHECK(o.first_failure.stage==CA_D_FREE_PROVIDER&&o.first_failure.native_api==CA_API_FREE_OBJECT);
 CHECK(o.first_failure.original_code==NTE_BAD_FLAGS&&!o.first_failure.key_owned&&o.first_failure.provider_owned);
 CHECK(o.state==PAPACC_CA_KEY_RETAINED&&!o.key&&o.provider==0x101);
 /* Local identity denial, zero CNG calls; no SID or policy changes. */
 reset();memset(&o,0,sizeof(o));CHECK(papacc_ca_key_create(&o,a,g,admin,admin)==NTE_PERM);
 CHECK(!trace_count);CHECK(o.first_failure.origin==CA_D_LOCAL_GUARD);
 CHECK(o.first_failure.stage==CA_D_SID_VALID);CHECK(o.first_failure.native_api==CA_API_NONE);
 /* Local DACL denial and key/value mismatches after synthetic native success. */
 for(i=1;i<=3;i++){
  reset();local_fault=i;memset(&o,0,sizeof(o));s=papacc_ca_key_create(&o,a,g,admin,service);
  CHECK(s==(SECURITY_STATUS)(i==2?NTE_PERM:NTE_BAD_KEY_STATE));CHECK(o.first_failure.origin==CA_D_LOCAL_GUARD);
  CHECK(o.first_failure.native_api==CA_API_NONE);
  CHECK(o.first_failure.stage==(DWORD)(i==1?CA_D_LENGTH_VALUE:i==2?CA_D_ACE_MASK:CA_D_PUBLIC_VALUE));
 }
 /* Null/invalid owner has no fabricated receipt; wrong thread cannot write. */
 reset();CHECK(papacc_ca_key_validate(NULL)==NTE_BAD_KEY_STATE);CHECK(!trace_count);
 memset(&o,0,sizeof(o));o.thread=GetCurrentThreadId()+1;first=o.first_failure;
 CHECK(papacc_ca_key_validate(&o)==NTE_BAD_KEY_STATE);CHECK(memcmp(&first,&o.first_failure,sizeof(first))==0);
 /* Success, close exactly once and guard return codes unchanged. */
 reset();memset(&o,0,sizeof(o));CHECK(papacc_ca_key_create(&o,a,g,admin,service)==ERROR_SUCCESS);
 CHECK(o.state==PAPACC_CA_KEY_READY&&!o.first_failure.present);CHECK(o.diagnostic_finalization==2);
 CHECK(papacc_ca_key_close(&o)==ERROR_SUCCESS);i=trace_count;CHECK(papacc_ca_key_close(&o)==ERROR_SUCCESS);CHECK(trace_count==i);
 /* Direct receipt helper is diagnostic-only and first failure is immutable. */
 memset(&o,0,sizeof(o));CHECK(ca_win32(&o,CA_D_IDENTITY,CA_D_PROCESS_TOKEN,ERROR_ACCESS_DENIED,NTE_PERM)==NTE_PERM);
 CHECK(o.first_failure.original_code==ERROR_ACCESS_DENIED);CHECK(o.first_failure.backend_code==NTE_PERM);
 CHECK(o.first_failure.origin==CA_D_LOCAL_GUARD&&o.first_failure.win32_error==ERROR_ACCESS_DENIED);
 memset(&o,0,sizeof(o));SetLastError(0x1357);
 CHECK(ca_local(&o,CA_D_VALIDATE,CA_D_ENTRY,NTE_BAD_KEY_STATE)==NTE_BAD_KEY_STATE);
 CHECK(GetLastError()==0x1357);first=o.first_failure;
 CHECK(ca_native(&o,CA_D_BEGIN_CREATE,CA_D_CREATE_KEY,CA_API_CREATE_KEY,NTE_PERM)==NTE_PERM);
 CHECK(memcmp(&first,&o.first_failure,sizeof(first))==0);
 /* Byte-exact preserved implementation, same doubles: compare returns,
  * resource ownership/dispositions and full NCrypt call-stage trace. */
 for(i=0;i<sizeof(stages)/sizeof(stages[0]);i++){reset();fail_stage=stages[i];CHECK(compare_preimage(a,g,admin,service)==0);}
 reset();CHECK(compare_preimage(a,g,admin,service)==0);
 reset();fail_stage=CA_D_SET_DACL;cleanup_fail=1;CHECK(compare_preimage(a,g,admin,service)==0);
 reset();fail_stage=CA_D_SET_EXPORT;injected=NTE_BAD_FLAGS;CHECK(compare_preimage(a,g,admin,service)==0);
 reset();CHECK(compare_preimage(a,g,admin,admin)==0);
 for(i=1;i<=3;i++){reset();local_fault=i;CHECK(compare_preimage(a,g,admin,service)==0);}
 printf("PASS assertions=%lu; preimage_cases=19; mode=SYNTHETIC_NCRYPT_NO_NATIVE_KEYS; owner=%zu; receipt=%zu\n",(unsigned long)passed,sizeof(o),sizeof(o.first_failure));
 return 0;
}