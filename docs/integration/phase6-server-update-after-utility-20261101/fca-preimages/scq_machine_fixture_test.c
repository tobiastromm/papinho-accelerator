/* Actual same CA key owner; no real key/provider/token/SCM effects. */
#define _WIN32_WINNT 0x0600
#include "server_ca_key_win32.h"
#include <bcrypt.h>
#include <psapi.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
static unsigned read_open_model,fixture_sd_fault,fixture_sd_custom,fixture_sd_shape,fixture_generic_model;
static DWORD fixture_sd_masks[3];
static unsigned checks,calls,fail_at,free_fail,creates,opens,finals,properties,exports,freed,mismatch;
static volatile LONG fixture_stop;static unsigned stop_after;
static PSID admin_sid,virtual_sid;static BYTE fixture_descriptor[1024];static DWORD fixture_descriptor_bytes;
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%u calls=%u fail=%u\n",(unsigned)__LINE__,calls,fail_at);return 1;}}while(0)
static SECURITY_STATUS api(void){calls++;if(stop_after==calls)fixture_stop=1;return fail_at==calls?NTE_FAIL:0;}
static BOOL WINAPI mt(HANDLE a,DWORD b,BOOL c,PHANDLE d){(void)a;(void)b;(void)c;(void)d;SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI mp(HANDLE a,DWORD b,PHANDLE out){(void)a;(void)b;*out=(HANDLE)1;return TRUE;}
static BOOL WINAPI mi(HANDLE a,TOKEN_INFORMATION_CLASS k,LPVOID out,DWORD cap,PDWORD n)
{PTOKEN_USER u=out;DWORD z=GetLengthSid(admin_sid);(void)a;if(k!=TokenUser||cap<sizeof(*u)+z)return FALSE;u->User.Sid=(BYTE*)out+sizeof(*u);u->User.Attributes=0;CopySid(z,u->User.Sid,admin_sid);*n=sizeof(*u)+z;return TRUE;}
static BOOL WINAPI mc(HANDLE a){(void)a;return TRUE;}
static SECURITY_STATUS WINAPI op(NCRYPT_PROV_HANDLE *p,LPCWSTR name,DWORD flags)
{SECURITY_STATUS s=api();if(wcscmp(name,MS_KEY_STORAGE_PROVIDER)||flags)return NTE_INVALID_PARAMETER;if(!s)*p=11;return s;}
static SECURITY_STATUS WINAPI cr(NCRYPT_PROV_HANDLE p,NCRYPT_KEY_HANDLE *k,LPCWSTR alg,LPCWSTR name,DWORD legacy,DWORD flags)
{SECURITY_STATUS s=api();creates++;if(p!=11||wcscmp(alg,NCRYPT_RSA_ALGORITHM)||wcsncmp(name,L"PapinhoAccelerator.CA.",21)||legacy||flags!=NCRYPT_MACHINE_KEY_FLAG)return NTE_INVALID_PARAMETER;if(!s)*k=22;return s;}
static SECURITY_STATUS WINAPI ok(NCRYPT_PROV_HANDLE p,NCRYPT_KEY_HANDLE *k,LPCWSTR name,DWORD legacy,DWORD flags)
{SECURITY_STATUS s=api();opens++;if(p!=11||!name||legacy||flags!=(NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG))return NTE_INVALID_PARAMETER;if(!s)*k=read_open_model&&opens==1U?22:33;return s;}
static SECURITY_STATUS WINAPI sp(NCRYPT_HANDLE k,LPCWSTR name,PBYTE data,DWORD n,DWORD flags)
{SECURITY_STATUS s=api();properties++;if(k!=22)return NTE_INVALID_HANDLE;
 if(!wcscmp(name,NCRYPT_SECURITY_DESCR_PROPERTY)){if(n>sizeof(fixture_descriptor)||flags!=(DWORD)(DACL_SECURITY_INFORMATION|NCRYPT_PERSIST_FLAG|(fixture_generic_model?0U:OWNER_SECURITY_INFORMATION)))return NTE_INVALID_PARAMETER;memcpy(fixture_descriptor,data,n);fixture_descriptor_bytes=n;}
 else if(n!=4||flags)return NTE_INVALID_PARAMETER;return s;}
static SECURITY_STATUS WINAPI fk(NCRYPT_KEY_HANDLE k,DWORD flags)
{finals++;if(k!=22||flags!=NCRYPT_SILENT_FLAG)return NTE_INVALID_PARAMETER;return api();}
static SECURITY_STATUS WINAPI gp(NCRYPT_HANDLE k,LPCWSTR name,PBYTE out,DWORD cap,PDWORD n,DWORD flags)
{DWORD v;SECURITY_STATUS s=api();if(s)return s;if(k!=22)return NTE_INVALID_HANDLE;
 if(!wcscmp(name,NCRYPT_SECURITY_DESCR_PROPERTY)){if(flags!=(DWORD)(DACL_SECURITY_INFORMATION|(fixture_generic_model?0U:OWNER_SECURITY_INFORMATION))||cap<fixture_descriptor_bytes)return NTE_INVALID_PARAMETER;if(fixture_sd_fault==1U)return NTE_PERM;
 memcpy(out,fixture_descriptor,fixture_descriptor_bytes);*n=fixture_descriptor_bytes;
 if(fixture_sd_fault==2U)((SECURITY_DESCRIPTOR_RELATIVE*)out)->Control&=(SECURITY_DESCRIPTOR_CONTROL)~SE_DACL_PROTECTED;
 if(fixture_sd_fault==3U||fixture_sd_fault==4U){PACL acl;BOOL present,def;PACE_HEADER ace;
  if(!GetSecurityDescriptorDacl(out,&present,&acl,&def)||!present||!GetAce(acl,0,(LPVOID*)&ace))return NTE_FAIL;
  if(fixture_sd_fault==3U)ace->AceFlags=INHERITED_ACE;else ((ACCESS_ALLOWED_ACE*)ace)->Mask=0x000f003fU;
 }
 if(fixture_sd_fault==5U){PACL acl;BOOL present,def;DWORD index;PACE_HEADER ace;
  if(!GetSecurityDescriptorDacl(out,&present,&acl,&def)||!present)return NTE_FAIL;
  for(index=0;index<acl->AceCount;index++){if(!GetAce(acl,index,(LPVOID*)&ace))return NTE_FAIL;((ACCESS_ALLOWED_ACE*)ace)->Mask=0xd01f01ffU;}
 }
 if(fixture_sd_custom||fixture_sd_shape){PACL acl;BOOL present,def;DWORD index;PACE_HEADER ace;
  if(!GetSecurityDescriptorDacl(out,&present,&acl,&def)||!present||acl->AceCount!=3U)return NTE_FAIL;
  if(fixture_sd_custom)for(index=0;index<3U;index++){if(!GetAce(acl,index,(LPVOID*)&ace))return NTE_FAIL;((ACCESS_ALLOWED_ACE*)ace)->Mask=fixture_sd_masks[index];}
  if(fixture_sd_shape==1U){PSID owner;BYTE sys[SECURITY_MAX_SID_SIZE];DWORD bytes=sizeof(sys);
   if(!GetSecurityDescriptorOwner(out,&owner,&def)||!owner||!CreateWellKnownSid(WinLocalSystemSid,NULL,sys,&bytes)||bytes>GetLengthSid(owner))return NTE_FAIL;memcpy(owner,sys,bytes);}
  if(fixture_sd_shape==2U)((SECURITY_DESCRIPTOR_RELATIVE*)out)->Control&=(SECURITY_DESCRIPTOR_CONTROL)~SE_DACL_PROTECTED;
  if(fixture_sd_shape==3U)acl->AceCount=4U;if(fixture_sd_shape==4U)acl->AceCount=2U;
  if(fixture_sd_shape==5U||fixture_sd_shape==8U||fixture_sd_shape==9U){if(!GetAce(acl,0,(LPVOID*)&ace))return NTE_FAIL;
   if(fixture_sd_shape==5U)ace->AceType=ACCESS_ALLOWED_ACE_TYPE;if(fixture_sd_shape==8U)ace->AceType=ACCESS_DENIED_CALLBACK_ACE_TYPE;if(fixture_sd_shape==9U)ace->AceFlags=CONTAINER_INHERIT_ACE;}
  if(fixture_sd_shape==6U||fixture_sd_shape==7U){LPSTR actor=NULL,runtime=NULL;PSECURITY_DESCRIPTOR changed=NULL;char text[1024];DWORD bytes;
   if(!ConvertSidToStringSidA(admin_sid,&actor)||!ConvertSidToStringSidA(virtual_sid,&runtime))return NTE_FAIL;
   if(fixture_sd_shape==6U)sprintf_s(text,sizeof(text),"O:%sG:SYD:P(D;;GA;;;%s)(A;;GA;;;SY)(A;;GA;;;%s)",actor,runtime,actor);
   else sprintf_s(text,sizeof(text),"O:%sG:SYD:P(D;;GA;;;BA)(A;;GA;;;%s)(A;;GA;;;SY)",actor,actor);
   if(!ConvertStringSecurityDescriptorToSecurityDescriptorA(text,1,&changed,&bytes)||bytes>cap)return NTE_FAIL;
   memcpy(out,changed,bytes);*n=bytes;LocalFree(changed);LocalFree(actor);LocalFree(runtime);
  }
 }
 return 0;}
 v=!wcscmp(name,NCRYPT_LENGTH_PROPERTY)?2048:!wcscmp(name,NCRYPT_EXPORT_POLICY_PROPERTY)?0:NCRYPT_ALLOW_SIGNING_FLAG;if(flags||cap<4)return NTE_INVALID_PARAMETER;memcpy(out,&v,4);*n=4;return 0;}
static SECURITY_STATUS WINAPI ek(NCRYPT_KEY_HANDLE k,NCRYPT_KEY_HANDLE exportkey,LPCWSTR type,NCryptBufferDesc *params,PBYTE out,DWORD cap,PDWORD n,DWORD flags)
{BCRYPT_RSAKEY_BLOB *b=(BCRYPT_RSAKEY_BLOB*)out;SECURITY_STATUS s=api();exports++;if(s)return s;
 if((k!=22&&k!=33)||exportkey||wcscmp(type,BCRYPT_RSAPUBLIC_BLOB)||params||cap<283||flags)return NTE_INVALID_PARAMETER;
 memset(out,0,283);b->Magic=BCRYPT_RSAPUBLIC_MAGIC;b->BitLength=2048;b->cbPublicExp=3;b->cbModulus=256;out[sizeof(*b)]=1;out[sizeof(*b)+2]=1;out[282]=(BYTE)(k==33&&mismatch?2:1);*n=283;return 0;}
static SECURITY_STATUS WINAPI fr(NCRYPT_HANDLE k){(void)k;freed++;return free_fail?NTE_FAIL:0;}
#define OpenThreadToken mt
#define OpenProcessToken mp
#define GetTokenInformation mi
#define CloseHandle mc
#define NCryptOpenStorageProvider op
#define NCryptCreatePersistedKey cr
#define NCryptOpenKey ok
#define NCryptSetProperty sp
#define NCryptFinalizeKey fk
#define NCryptGetProperty gp
#define NCryptExportKey ek
#define NCryptFreeObject fr
static ULONGLONG fb_commit=17175674880ULL;static SIZE_T fb_page=4096;static unsigned fb_overflow;
static DWORD fb_count=100,fb_query_fault;static ULONGLONG fb_private=4096,fb_working=4096,fb_tick=100;static unsigned fb_qpc_calls,fb_stale,fb_stop;
static BOOL WINAPI fb_handles(HANDLE h,PDWORD n){(void)h;if(fb_query_fault){SetLastError(fb_query_fault==2U?0:ERROR_INVALID_DATA);return FALSE;}*n=fb_count;return TRUE;}
static BOOL WINAPI fb_memory(HANDLE h,PPROCESS_MEMORY_COUNTERS p,DWORD n)
{PROCESS_MEMORY_COUNTERS_EX *x=(PROCESS_MEMORY_COUNTERS_EX*)p;(void)h;memset(p,0,n);p->cb=n;p->WorkingSetSize=(SIZE_T)fb_working;x->PrivateUsage=(SIZE_T)fb_private;return TRUE;}
static BOOL WINAPI fb_global(LPMEMORYSTATUSEX p){p->ullTotalPhys=21474836480ULL;p->ullAvailPhys=10737418240ULL;return TRUE;}
static BOOL WINAPI fb_performance(PPERFORMANCE_INFORMATION p,DWORD n){memset(p,0,n);p->cb=n;p->PageSize=fb_page;p->CommitLimit=(SIZE_T)(21474836480ULL/fb_page);p->CommitTotal=p->CommitLimit-(SIZE_T)(fb_commit/fb_page);if(fb_overflow){p->PageSize=(SIZE_T)-1;p->CommitLimit=2;p->CommitTotal=0;}if(fb_stop)fixture_stop=1;return TRUE;}
static BOOL WINAPI fb_frequency(LARGE_INTEGER *v){v->QuadPart=1000;return TRUE;}
static BOOL WINAPI fb_counter(LARGE_INTEGER *v){fb_qpc_calls++;v->QuadPart=fb_stale&&fb_qpc_calls%3U==2U?300:100;return TRUE;}
static ULONGLONG WINAPI fb_clock(void){return fb_tick;}
#define GetProcessHandleCount fb_handles
#define GetProcessMemoryInfo fb_memory
#define GlobalMemoryStatusEx fb_global
#define GetPerformanceInfo fb_performance
#define QueryPerformanceCounter fb_counter
#define QueryPerformanceFrequency fb_frequency
#define GetTickCount64 fb_clock
#include "../apps/server/server_ca_key_win32.c"
/* Hash APIs are simulated too; no provider or key is touched by this test. */
static unsigned fh_fault,fh_calls;static BYTE fh_blob[283];
static NTSTATUS fh_step(void){fh_calls++;return fh_fault==fh_calls?(NTSTATUS)0xc0000001L:0;}
static NTSTATUS WINAPI fh_open(BCRYPT_ALG_HANDLE *h,LPCWSTR name,LPCWSTR impl,ULONG flags)
{NTSTATUS s=fh_step();(void)impl;if(wcscmp(name,BCRYPT_SHA256_ALGORITHM)||flags)return (NTSTATUS)0xc000000dL;if(!s)*h=(BCRYPT_ALG_HANDLE)101;return s;}
static NTSTATUS WINAPI fh_create(BCRYPT_ALG_HANDLE a,BCRYPT_HASH_HANDLE *h,PUCHAR obj,ULONG bytes,PUCHAR secret,ULONG secret_bytes,ULONG flags)
{NTSTATUS s=fh_step();(void)a;if(obj||bytes||secret||secret_bytes||flags)return (NTSTATUS)0xc000000dL;if(!s)*h=(BCRYPT_HASH_HANDLE)102;return s;}
static NTSTATUS WINAPI fh_data(BCRYPT_HASH_HANDLE h,PUCHAR b,ULONG n,ULONG flags)
{NTSTATUS s=fh_step();(void)h;if(n!=283||flags)return (NTSTATUS)0xc000000dL;if(!s)memcpy(fh_blob,b,n);return s;}
static NTSTATUS WINAPI fh_final(BCRYPT_HASH_HANDLE h,PUCHAR b,ULONG n,ULONG flags)
{NTSTATUS s=fh_step();(void)h;if(n!=32||flags)return (NTSTATUS)0xc000000dL;if(!s)memset(b,0x71,32);return s;}
static NTSTATUS WINAPI fh_destroy(BCRYPT_HASH_HANDLE h){(void)h;return fh_step();}
static NTSTATUS WINAPI fh_close(BCRYPT_ALG_HANDLE h,ULONG flags){(void)h;(void)flags;return fh_step();}
#define BCryptOpenAlgorithmProvider fh_open
#define BCryptCreateHash fh_create
#define BCryptHashData fh_data
#define BCryptFinishHash fh_final
#define BCryptDestroyHash fh_destroy
#define BCryptCloseAlgorithmProvider fh_close
#include "../apps/server/server_store_hash_win32.inc"
#include "../apps/server/personal_machine_fixture_win32.inc"
#ifndef PAPACC_MACHINE_FIXTURE_ENTRY
#define PAPACC_MACHINE_FIXTURE_ENTRY main
#endif
int PAPACC_MACHINE_FIXTURE_ENTRY(void)
{
 PAPACC_CA_KEY o;BYTE a[16]={1},g[16]={2},blob[283];DWORD n;unsigned i,positive_calls;SECURITY_STATUS s;
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&admin_sid));C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 memset(&o,0,sizeof(o));C(!papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop));positive_calls=calls;
 C(o.diagnostic_only&&creates==1&&finals==1&&properties==4&&o.state==PAPACC_CA_KEY_READY);
 C(papacc_ca_key_borrow(&o,&o)!=0&&!o.borrower);C(!papacc_ca_key_fixture_control(&o,blob,sizeof(blob),&n)&&n==283&&opens==1&&!o.diagnostic_reopen);
 C(papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop)!=0&&creates==1);C(papacc_ca_key_fixture_control(&o,blob,sizeof(blob),&n)!=0&&opens==1);C(!papacc_ca_key_close(&o));
 for(i=1;i<=positive_calls;i++){
  memset(&o,0,sizeof(o));calls=creates=finals=properties=exports=freed=0;fail_at=i;
  s=papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop);C(s!=0&&o.diagnostic_only&&o.first_failure.present&&creates<=1&&finals<=1);
  C(papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop)!=0); /* no retry even pre-create */
 }
 fail_at=0;calls=0;memset(&o,0,sizeof(o));C(!papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop));mismatch=1;
 C(papacc_ca_key_fixture_control(&o,blob,sizeof(blob),&n)!=0&&n==0&&!o.diagnostic_reopen);C(!papacc_ca_key_close(&o));mismatch=0;
 memset(&o,0,sizeof(o));C(!papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop));free_fail=1;
 C(papacc_ca_key_fixture_control(&o,blob,sizeof(blob),&n)!=0&&o.state==PAPACC_CA_KEY_RETAINED&&o.diagnostic_reopen);
 i=freed;C(papacc_ca_key_close(&o)!=0&&freed==i);free_fail=0;
 for(i=1;i<positive_calls;i++){
  memset(&o,0,sizeof(o));calls=creates=opens=finals=properties=exports=freed=0;fixture_stop=0;stop_after=i;
  C(papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop)!=0&&o.diagnostic_only&&o.first_failure.present);
  C(o.first_failure.origin==CA_D_LOCAL_GUARD&&o.first_failure.original_code==NTE_USER_CANCELLED&&calls==i);
 }
 stop_after=0;fixture_stop=0;

 {PAPACC_MACHINE_FIXTURE_CONTROL f;unsigned j,before;
  for(j=0;j<7U;j++){
   memset(&f,0,sizeof(f));calls=creates=opens=finals=properties=exports=freed=0;fail_at=free_fail=mismatch=stop_after=0;fixture_stop=0;
   fh_fault=j;fh_calls=0;C(!papacc_ca_key_fixture_create(&f.key,a,g,admin_sid,virtual_sid,&fixture_stop));
   s=(SECURITY_STATUS)scqkf_identity(&f,&f,&fixture_stop);
   if(!j){C(!s&&f.phase==2U&&f.public_id[0]==0x71&&f.public_bytes==283&&!memcmp(fh_blob,f.public_blob,283));
    C(!scqkf_identity_current(&f,&f,&fixture_stop));before=fh_calls;C(scqkf_identity(&f,&f,&fixture_stop)!=0&&fh_calls==before);}
   else {C(s!=0&&f.used&&f.phase==3U&&f.stage==4U&&f.public_id[0]==0);before=fh_calls;
    C(scqkf_identity(&f,&f,&fixture_stop)!=0&&fh_calls==before);}
   before=freed;s=(SECURITY_STATUS)scqkf_close(&f,&f);
   if(j==5U||j==6U){C(s!=0&&f.hash.retained&&freed==before);C(scqkf_close(&f,&f)!=0&&freed==before);}
   else C(!s&&f.key.state==PAPACC_CA_KEY_CLOSED&&f.phase==4U);
  }
  memset(&f,0,sizeof(f));calls=0;fh_fault=fh_calls=0;fixture_stop=0;
  C(!papacc_ca_key_fixture_create(&f.key,a,g,admin_sid,virtual_sid,&fixture_stop));fixture_stop=1;
  C(scqkf_identity(&f,&f,&fixture_stop)!=0&&f.used&&!fh_calls);C(!scqkf_close(&f,&f));fixture_stop=0;
 }

 for(i=0;i<7U;i++){
  memset(&o,0,sizeof(o));calls=creates=opens=fail_at=free_fail=mismatch=stop_after=0;fixture_stop=0;
  fb_private=fb_working=4096;fb_count=100;fb_query_fault=fb_qpc_calls=fb_stale=fb_stop=0;fb_tick=100;
  C(!papacc_ca_key_fixture_budget_begin(&o));
  if(i==1U)fb_count=188;if(i==2U)fb_tick=60100;if(i==3U)fb_query_fault=1;if(i==4U)fb_stale=1;if(i==5U)fb_stop=1;
  if(i==6U)fb_private=fb_working=1024; /* a decrease is not growth/underflow */
  s=papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop);
  if(i==0U||i==6U){C(!s&&creates==1&&o.diagnostic_budget.observation.samples>1);C(!papacc_ca_key_close(&o));}
  else {C(s!=0&&o.diagnostic_only&&!creates&&!opens&&o.diagnostic_budget.first);C(papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop)!=0);}
 }

 /* F1 boundaries use the real budget owner with only native queries mocked. */
 {static const DWORD entry[]={127U,128U,129U};static const DWORD next[]={187U,188U,189U,191U,192U,193U};unsigned j;
  fb_query_fault=fb_stale=fb_stop=fb_qpc_calls=0;fixture_stop=0;fb_tick=100;fb_private=fb_working=4096;
  for(j=0;j<3U;j++){memset(&o,0,sizeof(o));fb_count=entry[j];C((papacc_ca_key_fixture_budget_begin(&o)==0)==(entry[j]<=128U));}
  for(j=0;j<6U;j++){memset(&o,0,sizeof(o));fb_count=100;C(!papacc_ca_key_fixture_budget_begin(&o));fb_count=next[j];C((papacc_ca_key_fixture_budget_check(&o,&fixture_stop)==0)==(next[j]<188U));}
  fb_page=1;for(j=0;j<3U;j++){memset(&o,0,sizeof(o));fb_count=100;fb_commit=j==0U?3221225471ULL:j==2U?3221225473ULL:3221225472ULL;C((papacc_ca_key_fixture_budget_begin(&o)==0)==(j!=0U));}
  fb_commit=17175674880ULL;fb_page=4096;
 }
 /* R0: exact independent and combined positive-growth boundaries; original baseline never rebased. */
 {unsigned j,k,attempts;DWORD e;ULONGLONG growth[3]={16777215ULL,16777216ULL,16777217ULL};
  fb_query_fault=fb_overflow=fb_stale=fb_stop=0;fixture_stop=0;fb_count=100;fb_tick=100;
  for(j=0;j<3U;j++)for(k=0;k<3U;k++){
   memset(&o,0,sizeof(o));fb_private=fb_working=4096;C(!papacc_ca_key_fixture_budget_begin(&o));
   fb_private=4096+growth[j];fb_working=4096+growth[k];e=papacc_ca_key_fixture_budget_check(&o,&fixture_stop);
   C((e==0)==(j<2U&&k<2U));C(o.diagnostic_budget.observation.first.private_bytes==4096&&o.diagnostic_budget.observation.first.working_set==4096);
   if(e){attempts=o.diagnostic_budget.observation.attempts;fb_private=fb_working=4096;C(papacc_ca_key_fixture_budget_check(&o,&fixture_stop)==e);C(o.diagnostic_budget.observation.attempts==attempts);}
  }
  for(j=0;j<2U;j++)for(k=0;k<3U;k++){
   memset(&o,0,sizeof(o));fb_private=fb_working=4096;C(!papacc_ca_key_fixture_budget_begin(&o));
   if(j)fb_working+=growth[k];else fb_private+=growth[k];C((papacc_ca_key_fixture_budget_check(&o,&fixture_stop)==0)==(k<2U));
  }
  memset(&o,0,sizeof(o));fb_private=fb_working=(ULONGLONG)-1;C(!papacc_ca_key_fixture_budget_begin(&o));fb_private=fb_working=0;C(!papacc_ca_key_fixture_budget_check(&o,&fixture_stop));
  memset(&o,0,sizeof(o));C(!papacc_ca_key_fixture_budget_begin(&o));fb_private=(ULONGLONG)-1;C(papacc_ca_key_fixture_budget_check(&o,&fixture_stop)==ERROR_NOT_ENOUGH_QUOTA);
  fb_private=fb_working=4096;memset(&o,0,sizeof(o));fb_overflow=1;C(papacc_ca_key_fixture_budget_begin(&o)==ERROR_ARITHMETIC_OVERFLOW);fb_overflow=0;
  memset(&o,0,sizeof(o));fb_query_fault=2;C(papacc_ca_key_fixture_budget_begin(&o)==ERROR_GEN_FAILURE);fb_query_fault=0;
 }
 fb_count=100;fb_query_fault=fb_stale=fb_stop=0;fixture_stop=0;fb_tick=100;
 /* The same sample count can mean native Security Descr denial OR a local strict-SD rejection.
  * Six source samples are reproduced using the existing budget, not by inventing a native trace. */
 {unsigned j,k;DWORD samples;for(j=0;j<=5U;j++){
  memset(&o,0,sizeof(o));calls=creates=opens=finals=properties=exports=freed=fail_at=free_fail=mismatch=stop_after=0;
  fixture_stop=0;fb_count=116;fb_private=fb_working=4096;fb_query_fault=fb_overflow=fb_stale=fb_stop=0;fb_tick=100;fixture_sd_fault=j;
  C(!papacc_ca_key_fixture_budget_begin(&o));for(k=0;k<5U;k++)C(!papacc_ca_key_fixture_budget_check(&o,&fixture_stop));
  s=papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop);samples=o.diagnostic_budget.observation.samples;
  C(samples==18U&&o.created&&o.diagnostic_finalization==2U&&o.diagnostic_budget.first==0);
  if(!j||j==5U){C(!s&&!o.first_failure.present);C(!papacc_ca_key_close(&o));}
  else {C(s==NTE_PERM&&o.first_failure.present&&o.first_failure.function==CA_D_ACL&&o.state==PAPACC_CA_KEY_CLOSED);
   C(o.first_failure.stage==(DWORD)(j==1U?CA_D_GET_DACL:j==2U?CA_D_SD_VALID:j==3U?CA_D_ACE_FLAGS:CA_D_ACE_MASK));
   C(o.first_failure.origin==(DWORD)(j==1U?CA_D_NATIVE_NCRYPT:CA_D_LOCAL_GUARD));
   C(o.first_failure.original_code==NTE_PERM);}
 }fixture_sd_fault=0;printf("PASS FIXTURE_SD_BOUNDARY_MODEL cases=6 OBSERVED_D01F01FF_ACCEPTED_ONLY_AS_UNIFORM_TEMPLATE samples=18 NATIVE_OR_LOCAL_NOT_DISTINGUISHED_BY_COUNTERS NO_NATIVE_KSP\n");}
 /* S1: all eight mixed-template combinations, every single-bit mutation of both forms,
  * and structural negatives run through the actual create/validate owner. Setter remains GA. */
 {unsigned j,k,mask_cases=0;PACL acl;BOOL present,def;PACE_HEADER ace;BYTE control_blob[283];DWORD bytes;
  for(j=0;j<8U;j++){
   memset(&o,0,sizeof(o));calls=creates=opens=finals=properties=exports=freed=fail_at=free_fail=mismatch=stop_after=0;fixture_stop=0;
   fixture_sd_fault=fixture_sd_shape=fixture_generic_model=0;fixture_sd_custom=1;
   for(k=0;k<3U;k++)fixture_sd_masks[k]=(j&(1U<<k))?0xd01f01ffU:GENERIC_ALL;
   s=papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop);C((s==0)==(j==0U||j==7U));mask_cases++;
   if(!s){C(!papacc_ca_key_fixture_control(&o,control_blob,sizeof(control_blob),&bytes)&&bytes==283&&o.diagnostic_control_confirmed&&opens==1);
    C(papacc_ca_key_fixture_control(&o,control_blob,sizeof(control_blob),&bytes)!=0&&opens==1);
    C(GetSecurityDescriptorDacl(fixture_descriptor,&present,&acl,&def)&&present&&acl->AceCount==3U);
    for(k=0;k<3U;k++){C(GetAce(acl,k,(LPVOID*)&ace));C(((ACCESS_ALLOWED_ACE*)ace)->Mask==GENERIC_ALL);}C(!papacc_ca_key_close(&o));
   }else {C(o.first_failure.stage==CA_D_ACE_MASK&&o.first_failure.origin==CA_D_LOCAL_GUARD);C(papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop)!=0&&creates==1);}
  }
  for(j=0;j<64U;j++){
   memset(&o,0,sizeof(o));calls=creates=opens=finals=properties=exports=freed=0;fixture_sd_custom=1;fixture_sd_shape=fixture_sd_fault=0;
   for(k=0;k<3U;k++)fixture_sd_masks[k]=(j<32U?GENERIC_ALL:0xd01f01ffU)^((DWORD)1U<<(j%32U));
   C(papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop)!=0&&o.first_failure.stage==CA_D_ACE_MASK);mask_cases++;
  }
  fixture_sd_custom=0;for(j=1;j<=9U;j++){
   memset(&o,0,sizeof(o));calls=creates=opens=finals=properties=exports=freed=0;fixture_sd_fault=0;fixture_sd_shape=j;
   C(papacc_ca_key_fixture_create(&o,a,g,admin_sid,virtual_sid,&fixture_stop)!=0&&o.first_failure.present);mask_cases++;
  }
  fixture_sd_shape=fixture_sd_custom=0;fixture_generic_model=1;
  for(j=0;j<2U;j++){
   memset(&o,0,sizeof(o));calls=creates=opens=finals=properties=exports=freed=0;fixture_sd_fault=j?5U:0U;
   C(!papacc_ca_key_create(&o,a,g,admin_sid,virtual_sid)&&!o.diagnostic_only);C(!papacc_ca_key_close(&o));
  }
  fixture_generic_model=fixture_sd_fault=fixture_sd_custom=fixture_sd_shape=0;
  printf("PASS S1_EXACT_SD_TEMPLATES cases=%u UNIFORM_GA_OR_D01F01FF_ONLY ORIGINAL_GA_SETTER_SAME_KEY_POSITIVE_CONTROL ORDINARY_CA_UNCHANGED NO_REAL_KSP\n",mask_cases);
 }
 C(LocalFree(admin_sid)==NULL);C(LocalFree(virtual_sid)==NULL);
 printf("PASS MACHINE_FIXTURE checks=%u create_faults=%u SAME_OWNER_EXCLUSIVE_TEST_ONLY_PUBLIC_CONTROL_SUBSTITUTION_CLOSE_UNKNOWN NO_REAL_NCRYPT\n",checks,positive_calls);return 0;
}
