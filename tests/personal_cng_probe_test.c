/* Source-linked identity and NCrypt boundaries modeled; no real key/SCM. */
#define main preserved_identity_main
#include "personal_runtime_identity_test.c"
#undef main
static unsigned qfault,qopens,qfrees,qproviders;static volatile LONG qstop;
static const WCHAR fixture_name[]=L"PapinhoAccelerator.CA.11111111111111111111111111111111.22222222222222222222222222222222";
static DWORD WINAPI self_pid(void){return qfault==1?88:77;}
static SECURITY_STATUS WINAPI qprovider(NCRYPT_PROV_HANDLE *out,LPCWSTR provider,DWORD flags)
{qproviders++;if(wcscmp(provider,MS_KEY_STORAGE_PROVIDER)||flags)return NTE_INVALID_PARAMETER;
 if(qfault==2)return NTE_PROVIDER_DLL_FAIL;if(qfault==11)return 0;*out=301;return qfault==12?NTE_FAIL:0;}
static SECURITY_STATUS WINAPI qopen(NCRYPT_PROV_HANDLE provider,NCRYPT_KEY_HANDLE *key,LPCWSTR name,DWORD legacy,DWORD flags)
{qopens++;if(provider!=301||wcscmp(name,fixture_name)||legacy||flags!=(NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG))return NTE_INVALID_PARAMETER;
 if(qfault==3)return NTE_BAD_KEYSET;if(qfault==4)return NTE_FAIL;
 if(qfault==5||qfault==13){*key=302;return 0;}if(qfault==6){*key=302;return NTE_PERM;}
 if(qfault==7)InterlockedExchange(&qstop,1);if(qfault==8)fault=11;
 return NTE_PERM;}
static SECURITY_STATUS WINAPI qfree(NCRYPT_HANDLE value)
{qfrees++;if(qfault==9|| (qfault==10&&value==301)||(qfault==13&&value==302))return NTE_FAIL;return 0;}
static BOOL WINAPI qwinclose(HANDLE value)
{if(qfault==16){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return model_close(value);}
static BOOL WINAPI qthreadtoken(HANDLE a,DWORD b,BOOL c,PHANDLE out)
{(void)a;(void)b;(void)c;if(qfault==14||qfault==16){*out=(HANDLE)303;return TRUE;}SetLastError(qfault==15?ERROR_ACCESS_DENIED:ERROR_NO_TOKEN);return FALSE;}
#define GetCurrentProcessId self_pid
#define NCryptOpenStorageProvider qprovider
#define NCryptOpenKey qopen
#define NCryptFreeObject qfree
#undef CloseHandle
#define CloseHandle qwinclose
#undef OpenThreadToken
#define OpenThreadToken qthreadtoken
#include "../apps/server/personal_cng_probe_win32.inc"
#ifndef PAPACC_CNG_PROBE_NO_MAIN
int main(void)
{
 PAPACC_PERSONAL_RUNTIME_IDENTITY r;PAPACC_PERSONAL_CNG_PROBE p;unsigned i,before;DWORD result;
 const DWORD outcomes[]={PAPACC_CNG_PROBE_PERMISSION_OBSERVED,PAPACC_CNG_PROBE_LOCAL_DENY,
 PAPACC_CNG_PROBE_PROVIDER_FAILURE,PAPACC_CNG_PROBE_NOT_FOUND,PAPACC_CNG_PROBE_PROVIDER_FAILURE,
 PAPACC_CNG_PROBE_UNEXPECTED_OPEN,PAPACC_CNG_PROBE_UNEXPECTED_OPEN,PAPACC_CNG_PROBE_STOPPED,
 PAPACC_CNG_PROBE_QUERY_FAILURE,PAPACC_CNG_PROBE_CLOSE_UNKNOWN,PAPACC_CNG_PROBE_CLOSE_UNKNOWN,
 PAPACC_CNG_PROBE_PROVIDER_FAILURE,PAPACC_CNG_PROBE_PROVIDER_FAILURE,PAPACC_CNG_PROBE_CLOSE_UNKNOWN,
 PAPACC_CNG_PROBE_LOCAL_DENY,PAPACC_CNG_PROBE_QUERY_FAILURE,PAPACC_CNG_PROBE_CLOSE_UNKNOWN};
 checks=0;C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<=16;i++){
  reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);
  memset(&p,0,sizeof(p));qfault=i;qopens=qfrees=qproviders=0;qstop=0;
  result=papacc_personal_cng_probe(&p,&r,fixture_name,&qstop);C(result==outcomes[i]);
  if(i==0)C(p.open_calls==1&&p.provider_calls==1&&p.first_stage==4&&p.open_result==NTE_PERM&&p.phase==3&&!p.key&&!p.provider);
  if(i==1)C(qopens==0&&qproviders==0&&p.first_stage==2);
  if(i==2)C(qopens==0&&p.first_stage==3);
  if(i==5||i==6)C(qfrees==2&&!p.key&&!p.provider);
  if(i==9||i==10||i==13){C(p.phase==4&&p.provider);before=qfrees;qfault=0;C(papacc_personal_cng_probe_close(&p)!=0&&qfrees==before);}
  if(i==11)C(qopens==0&&!p.provider);
  if(i==12)C(qfrees==1&&!p.provider);
  if(i==14||i==15)C(qopens==0&&qproviders==0&&!p.unexpected_thread);
  if(i==16){C(p.unexpected_thread&&p.phase==4&&p.close_api==2);qfault=0;C(papacc_personal_cng_probe_close(&p)!=0);}
  before=qopens;C(papacc_personal_cng_probe(&p,&r,fixture_name,&qstop)!=PAPACC_CNG_PROBE_PERMISSION_OBSERVED&&qopens==before);
  fault=0;C(papacc_personal_runtime_close(&r)==0);
 }
 reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);memset(&p,0,sizeof(p));qfault=0;qstop=1;qopens=qproviders=0;
 C(papacc_personal_cng_probe(&p,&r,fixture_name,&qstop)==PAPACC_CNG_PROBE_STOPPED&&qproviders==0);
 memset(&p,0,sizeof(p));qstop=0;fault=17;
 C(papacc_personal_cng_probe(&p,&r,fixture_name,&qstop)==PAPACC_CNG_PROBE_QUERY_FAILURE&&qproviders==0);fault=0;
 memset(&p,0,sizeof(p));C(papacc_personal_cng_probe(&p,&r,L"PapinhoAccelerator.CA.live",&qstop)==ERROR_INVALID_PARAMETER&&!p.phase&&qproviders==0);
 memset(&p,0,sizeof(p));thread=8;C(papacc_personal_cng_probe(&p,&r,fixture_name,&qstop)==ERROR_INVALID_STATE&&!p.phase);thread=7;
 C(papacc_personal_runtime_close(&r)==0);C(LocalFree(virtual_sid)==NULL);C(LocalFree(sam_sid)==NULL);
 printf("PASS CNG_PROBE checks=%u MODEL_APIS ACTUAL_BINDING_AND_PROBE_SOURCE NO_REAL_CNG NO_PRIVATE_OPERATIONS bytes=%zu\n",checks,sizeof(p));return 0;
}
#endif
