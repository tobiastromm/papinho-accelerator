#define _WIN32_WINNT 0x0600
#include <windows.h>
static DWORD WINAPI read_ca_model_thread(void){return 7U;}
#define GetCurrentThreadId read_ca_model_thread
#define papacc_ca_key_create read_owner_create
#define papacc_ca_key_open read_owner_open
#define papacc_ca_key_validate read_owner_validate
#define papacc_ca_key_close read_owner_close
#define papacc_ca_key_borrow read_owner_borrow
#define papacc_ca_key_return read_owner_return
#define papacc_ca_key_transfer_borrow read_owner_transfer_borrow
#define papacc_ca_key_name_data read_owner_name_data
#define papacc_ca_key_fixture_budget_check read_owner_fixture_budget_check
#define papacc_ca_key_fixture_budget_begin read_owner_fixture_budget_begin
#define papacc_ca_key_fixture_create read_owner_fixture_create
#define papacc_ca_key_fixture_control read_owner_fixture_control
#define papacc_ca_key_machine_read_bind read_owner_machine_read_bind
#define papacc_ca_key_fixture_read_open read_owner_fixture_read_open
/* Separate translation unit: actual CA owner, ALL token/CNG/resource APIs modeled. */
#define PAPACC_MACHINE_FIXTURE_ENTRY preserved_read_ca_fixture_component
#include "scq_machine_fixture_test.c"
int read_ca_model_begin(PSID administrator,PSID runtime,unsigned fault,unsigned observed_sd)
{
 LPWSTR a=NULL,r=NULL;PSECURITY_DESCRIPTOR sd=NULL;WCHAR text[512];DWORD n;
 fixture_sd_fault=observed_sd?5U:0U;fixture_sd_custom=fixture_sd_shape=fixture_generic_model=0;
 admin_sid=administrator;virtual_sid=runtime;calls=creates=opens=finals=properties=exports=freed=0;
 fail_at=fault==1U?1U:0U;free_fail=fault==3U;mismatch=fault==2U;stop_after=0;fixture_stop=0;read_open_model=1U;
 fb_count=fault==4U?188U:100U;fb_query_fault=fb_stale=fb_stop=fb_qpc_calls=0;fb_tick=100;fb_private=fb_working=4096;
 if(!ConvertSidToStringSidW(administrator,&a)||!ConvertSidToStringSidW(runtime,&r))return 0;
 _snwprintf_s(text,512,_TRUNCATE,L"O:%sG:SYD:P(D;;GA;;;%s)(A;;GA;;;%s)(A;;GA;;;SY)",a,r,a);
 if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(text,1,&sd,&n)||n>sizeof(fixture_descriptor))return 0;
 memcpy(fixture_descriptor,sd,n);fixture_descriptor_bytes=n;LocalFree(sd);LocalFree(a);LocalFree(r);return 1;
}
DWORD read_ca_model_creates(void){return creates;}
DWORD read_ca_model_opens(void){return opens;}
DWORD read_ca_model_calls(void){return calls;}
