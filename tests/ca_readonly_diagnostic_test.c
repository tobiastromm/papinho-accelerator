/* All reachable NCrypt APIs replaced; no native key access. */
#define PAPACC_CA_READ_ONLY_DIAGNOSTIC 1
#define PAPACC_CA_DIAGNOSTIC_NO_MAIN 1
#define _WIN32_WINNT 0x0600
#include "server_ca_key_win32.h"
#include <bcrypt.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
static unsigned calls,checks;static int close_fail,open_fail,request_stop;
static volatile LONG stopping;static PSECURITY_DESCRIPTOR model_descriptor;static DWORD descriptor_size;
static SECURITY_STATUS WINAPI model_provider(NCRYPT_PROV_HANDLE *p,LPCWSTR name,DWORD flags){
 (void)name;(void)flags;calls++;*p=11;return ERROR_SUCCESS;
}
static SECURITY_STATUS WINAPI model_open(NCRYPT_PROV_HANDLE p,NCRYPT_KEY_HANDLE *k,LPCWSTR name,DWORD spec,DWORD flags){
 (void)p;(void)name;(void)spec;calls++;if(flags!=(NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG))return NTE_INVALID_PARAMETER;
 if(request_stop)InterlockedExchange(&stopping,1);
 if(open_fail)return NTE_PERM;*k=22;return ERROR_SUCCESS;
}
static SECURITY_STATUS WINAPI model_get(NCRYPT_HANDLE k,LPCWSTR name,PBYTE out,DWORD cap,DWORD *n,DWORD flags){
 DWORD v;(void)k;(void)flags;calls++;
 if(!wcscmp(name,NCRYPT_SECURITY_DESCR_PROPERTY)){
  if(cap<descriptor_size)return NTE_BUFFER_TOO_SMALL;memcpy(out,model_descriptor,descriptor_size);*n=descriptor_size;return ERROR_SUCCESS;
 }
 v=!wcscmp(name,NCRYPT_LENGTH_PROPERTY)?2048:!wcscmp(name,NCRYPT_EXPORT_POLICY_PROPERTY)?0:NCRYPT_ALLOW_SIGNING_FLAG;
 if(cap<sizeof(v))return NTE_BUFFER_TOO_SMALL;memcpy(out,&v,sizeof(v));*n=sizeof(v);return ERROR_SUCCESS;
}
static SECURITY_STATUS WINAPI model_export(NCRYPT_KEY_HANDLE k,NCRYPT_KEY_HANDLE wrap,LPCWSTR kind,NCryptBufferDesc *params,PBYTE out,DWORD cap,DWORD *n,DWORD flags){
 BCRYPT_RSAKEY_BLOB *b=(BCRYPT_RSAKEY_BLOB *)out;(void)k;(void)wrap;(void)params;(void)flags;calls++;
 /* Public only, exact existing backend validation contract. */
 if(wcscmp(kind,BCRYPT_RSAPUBLIC_BLOB))return NTE_PERM;
 *n=(DWORD)sizeof(*b)+259;if(cap<*n)return NTE_BUFFER_TOO_SMALL;memset(out,0,*n);
 b->Magic=BCRYPT_RSAPUBLIC_MAGIC;b->BitLength=2048;b->cbPublicExp=3;b->cbModulus=256;
 out[sizeof(*b)]=1;out[sizeof(*b)+2]=1;return ERROR_SUCCESS;
}
static SECURITY_STATUS WINAPI model_free(NCRYPT_HANDLE k){calls++;if(k==22&&close_fail)return NTE_PERM;return ERROR_SUCCESS;}
#define NCryptOpenStorageProvider model_provider
#define NCryptOpenKey model_open
#define NCryptGetProperty model_get
#define NCryptExportKey model_export
#define NCryptFreeObject model_free
#include "../apps/server/server_ca_key_win32.c"
#include "ca_key_readonly_diagnostic_win32.c"
#define CHECK(x) do {checks++;if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
int main(void){
 PAPACC_CA_KEY o;PAPACC_CA_FAILURE first;BYTE a[16]={1},g[16]={2},tokenbuf[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];
 BYTE service[SECURITY_MAX_SID_SIZE];DWORD n=0,z=sizeof(service);HANDLE token=NULL;PSID admin;LPSTR admin_text=NULL;
 char sddl[512];SECURITY_STATUS s;unsigned before;
 char *args[]={"test","--observe-existing","01000000000000000000000000000000","02000000000000000000000000000000",
 "PapinhoAccelerator.CA.01000000000000000000000000000000.02000000000000000000000000000000","S-1-5-19","S-1-5-20"};
 CHECK(diag_identity(7,args,a,g));CHECK(!diag_identity(6,args,a,g));
 args[4]="PapinhoAccelerator.CA.wrong";CHECK(!diag_identity(7,args,a,g));
 args[1]="--native-key";CHECK(!diag_identity(7,args,a,g));
 CHECK(!diag_hex("00000000000000000000000000000000",a));a[0]=1;
 CHECK(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token));
 CHECK(GetTokenInformation(token,TokenUser,tokenbuf,sizeof(tokenbuf),&n));admin=((PTOKEN_USER)tokenbuf)->User.Sid;
 CHECK(CloseHandle(token));CHECK(CreateWellKnownSid(WinLocalServiceSid,NULL,service,&z));
 CHECK(ConvertSidToStringSidA(admin,&admin_text));
 CHECK(_snprintf_s(sddl,sizeof(sddl),_TRUNCATE,"D:P(D;;GA;;;S-1-5-19)(A;;GA;;;%s)(A;;GA;;;SY)",admin_text)>0);
 CHECK(ConvertStringSecurityDescriptorToSecurityDescriptorA(sddl,SDDL_REVISION_1,&model_descriptor,&descriptor_size));
 CHECK(LocalFree(admin_text)==NULL);
 memset(&o,0,sizeof(o));before=calls;
 CHECK(papacc_ca_key_create(&o,a,g,admin,service)==NTE_NOT_SUPPORTED);CHECK(calls==before);
 CHECK(o.first_failure.origin==CA_D_LOCAL_GUARD&&o.first_failure.stage==CA_D_ENTRY);CHECK(!o.created&&!o.key&&!o.provider);
 memset(&o,0,sizeof(o));stopping=1;before=calls;CHECK(diag_observe(&o,a,g,admin,service,&stopping)==NTE_USER_CANCELLED);CHECK(calls==before);
 memset(&o,0,sizeof(o));stopping=0;before=calls;
 CHECK(diag_observe(&o,a,g,service,admin,&stopping)==NTE_PERM);CHECK(calls==before);
 CHECK(o.first_failure.origin==CA_D_LOCAL_GUARD&&o.first_failure.stage==CA_D_IDENTITY_EQUAL);
 memset(&o,0,sizeof(o));CHECK(diag_observe(&o,a,g,admin,admin,&stopping)==NTE_PERM);CHECK(o.first_failure.stage==CA_D_SID_VALID);
 memset(&o,0,sizeof(o));CHECK(diag_observe(&o,a,g,admin,service,&stopping)==ERROR_SUCCESS);
 CHECK(o.state==PAPACC_CA_KEY_CLOSED&&o.close_confirmed&&!o.key&&!o.provider&&!o.created&&!o.diagnostic_finalization);
 before=calls;CHECK(papacc_ca_key_close(&o)==ERROR_SUCCESS);CHECK(calls==before);
 memset(&o,0,sizeof(o));request_stop=1;
 CHECK(diag_observe(&o,a,g,admin,service,&stopping)==NTE_USER_CANCELLED);CHECK(o.close_confirmed);request_stop=0;stopping=0;
 memset(&o,0,sizeof(o));open_fail=1;CHECK(diag_observe(&o,a,g,admin,service,&stopping)==NTE_PERM);
 CHECK(o.first_failure.origin==CA_D_NATIVE_NCRYPT&&o.first_failure.native_api==CA_API_OPEN_KEY&&o.first_failure.original_code==NTE_PERM);
 first=o.first_failure;CHECK(papacc_ca_key_validate(&o)==NTE_BAD_KEY_STATE);CHECK(!memcmp(&first,&o.first_failure,sizeof(first)));open_fail=0;
 memset(&o,0,sizeof(o));close_fail=1;s=diag_observe(&o,a,g,admin,service,&stopping);
 CHECK(s==NTE_PERM&&o.state==PAPACC_CA_KEY_RETAINED&&o.key==22&&o.provider==11&&!o.close_confirmed);
 CHECK(o.first_failure.stage==CA_D_FREE_KEY&&o.first_failure.origin==CA_D_NATIVE_NCRYPT);
 before=calls;CHECK(papacc_ca_key_close(&o)==NTE_PERM);CHECK(calls==before); /* no destructive retry */
 CHECK(LocalFree(model_descriptor)==NULL);
 printf("PASS readonly_checks=%u; native_key_operations=NOT_RUN; retention=SYNTHETIC; owner_bytes=%zu\n",checks,sizeof(o));return 0;
}