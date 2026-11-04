/* Same native composition, all object/token/layout/console acquisitions modeled.
 * Real SD/SID codecs and SHA256 math only. Never opens a real directory. */
#define WIN32_LEAN_AND_MEAN 1
#include "server_launcher_owner_win32.h"
#include "server_c8_private_storage_layout_win32.h"
#include <winternl.h>
#include <aclapi.h>
#include <sddl.h>
#include <stdio.h>
#include "../tools/qualification/storage-hash-capsule/storage_sha256.h"
static PAPACC_SERVER_LAUNCHER_OWNER_WIN32 root;
static volatile LONG state;
static PAPACC_U64 next_invocation;
static unsigned scenario,opens,closes,infos,checks,free_calls,token_opens,sd_queries;
static PSECURITY_DESCRIPTOR fixture;
static PSID fixture_operator_sid;
static unsigned closed[16];
size_t papacc_server_service_storage_bytes(void){return 0;}
static PAPACC_QUALIFICATION_STAGING *staging_act(void)
{return &root.configuration_commit.pending_commit.personal_native.staging;}
static DWORD model_stop(void){return scenario==10U&&opens>=2U;}
static PAPACC_RESULT model_install(PAPACC_SERVER_CONSOLE_WIN32 *c){c->is_installed=1;return 0;}
static PAPACC_RESULT model_uninstall(PAPACC_SERVER_CONSOLE_WIN32 *c){c->is_installed=0;return scenario==14U?PAPACC_RESULT_INVALID_STATE:0;}
static BOOL WINAPI model_host(LPWSTR name,LPDWORD count)
{(void)count;wcscpy_s(name,64,scenario==12U?L"OTHER":L"TCORP-TOBIAS");return TRUE;}
static BOOL WINAPI model_thread(HANDLE t,DWORD access,BOOL self,PHANDLE out)
{(void)t;(void)self;if(access!=TOKEN_QUERY)return FALSE;if(scenario==13U){*out=(HANDLE)11;return TRUE;}SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI model_token(HANDLE p,DWORD access,PHANDLE out)
{(void)p;if(access!=TOKEN_QUERY)return FALSE;token_opens++;*out=(HANDLE)(ULONG_PTR)(token_opens==1U?10U:12U);return TRUE;}
static BOOL WINAPI model_token_info(HANDLE h,TOKEN_INFORMATION_CLASS cls,LPVOID data,DWORD bytes,PDWORD used)
{
 if(cls==TokenUser){TOKEN_USER *u=data;DWORD n=GetLengthSid(fixture_operator_sid);if(bytes<sizeof(*u)+n)return FALSE;
  u->User.Sid=(BYTE *)data+sizeof(*u);u->User.Attributes=0;memcpy(u->User.Sid,fixture_operator_sid,n);*used=(DWORD)sizeof(*u)+n;return TRUE;}
 if(cls==TokenStatistics){TOKEN_STATISTICS *s=data;if(bytes<sizeof(*s))return FALSE;ZeroMemory(s,sizeof(*s));s->TokenId.LowPart=scenario==19U&&h==(HANDLE)12?8:7;s->ModifiedId.LowPart=scenario==11U&&opens?2:1;*used=sizeof(*s);return TRUE;}
 return FALSE;
}
static PAPACC_RESULT model_layout(PAPACC_CONFIGURATION_COMMIT *c)
{wcscpy_s(c->pending_commit.personal_native.install_paths.binary,MAX_PATH,scenario==15U?L"D:\\Other":L"C:\\Program Files\\Papinho\\Accelerator");return 0;}
static PAPACC_RESULT model_current(PAPACC_CONFIGURATION_COMMIT *c){(void)c;return scenario==16U?PAPACC_RESULT_INVALID_STATE:0;}
static DWORD WINAPI model_dos(LPCWSTR drive,LPWSTR out,DWORD cap)
{if(wcscmp(drive,L"C:"))return 0;wcscpy_s(out,cap,L"\\Device\\HarddiskVolume3");return 24;}
static NTSTATUS NTAPI model_nt(PHANDLE out,ACCESS_MASK rights,POBJECT_ATTRIBUTES a,PIO_STATUS_BLOCK io,PLARGE_INTEGER allocation,ULONG attr,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_size)
{
 (void)allocation;(void)ea;(void)ea_size;opens++;
 if(rights!=(READ_CONTROL|FILE_READ_ATTRIBUTES|SYNCHRONIZE)||share!=FILE_SHARE_READ||disposition!=1U||
  options!=0x00200021U||attr!=FILE_ATTRIBUTE_DIRECTORY||a->SecurityDescriptor||!(a->Attributes&0x1000U))return (NTSTATUS)0xc0000022L;
 if(opens>1U&&a->RootDirectory!=(HANDLE)(ULONG_PTR)(opens-1U))return (NTSTATUS)0xc0000022L;
 if(scenario==24U&&opens==1U)return (NTSTATUS)0xc0000034L;
 if(scenario==1U&&opens==3U)return (NTSTATUS)0xc0000034L;
 if(scenario==2U&&opens==4U)return (NTSTATUS)0xc000003aL;
 if(scenario==8U&&opens==2U)return (NTSTATUS)0xc0000022L;
 *out=(HANDLE)(ULONG_PTR)opens;io->Information=1;
 if(scenario==9U&&opens==2U){io->Status=(NTSTATUS)0x103L;return (NTSTATUS)0x103L;}
 return 0;
}
static HMODULE WINAPI model_module(LPCWSTR n){(void)n;return (HMODULE)1;}
static FARPROC WINAPI model_proc(HMODULE m,LPCSTR name)
{FARPROC out;NTSTATUS (NTAPI *fn)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG)=model_nt;(void)m;if(strcmp(name,"NtCreateFile"))return NULL;memcpy(&out,&fn,sizeof(out));return out;}
static BOOL WINAPI model_info(HANDLE h,LPBY_HANDLE_FILE_INFORMATION f)
{infos++;if(scenario==23U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}ZeroMemory(f,sizeof(*f));f->dwVolumeSerialNumber=42;f->nFileIndexLow=(DWORD)(ULONG_PTR)h;f->dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;
 if(scenario==6U&&h==(HANDLE)2)f->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;
 if(scenario==7U&&infos>4U)f->nFileIndexLow++;return TRUE;}
static DWORD WINAPI model_sd(HANDLE h,SE_OBJECT_TYPE type,SECURITY_INFORMATION flags,PSID *owner,PSID *group,PACL *dacl,PACL *sacl,PSECURITY_DESCRIPTOR *out)
{
 DWORD size=GetSecurityDescriptorLength(fixture);sd_queries++;(void)h;(void)owner;(void)group;(void)dacl;(void)sacl;
 if(type!=SE_FILE_OBJECT||flags!=(OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION))return ERROR_ACCESS_DENIED;
 if(scenario==21U)return ERROR_ACCESS_DENIED;
 *out=LocalAlloc(LPTR,size);if(!*out)return ERROR_OUTOFMEMORY;memcpy(*out,fixture,size);if(scenario==22U&&sd_queries>4U)((BYTE *)*out)[size-1U]^=1;return 0;
}
static BOOL WINAPI model_close(HANDLE h)
{unsigned i=(unsigned)(ULONG_PTR)h;closes++;if(i>=16U||closed[i])return FALSE;if(scenario==17U&&closes==1U)return FALSE;closed[i]=1;return TRUE;}
static HLOCAL WINAPI model_free(HLOCAL h)
{free_calls++;if(scenario==18U&&free_calls==2U)return h;return LocalFree(h);}
/* shared original classifier, not a test-specific positive replacement */
#define PAPACC_SCM_DESCRIPTOR_TEST 1
#include "../apps/server/server_launcher_scm_prepare_win32.inc"
#include "../apps/server/server_staging_descriptor_win32.inc"
typedef NTSTATUS (NTAPI *STAGING_NT_CREATE)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
#define GetComputerNameW model_host
#define OpenThreadToken model_thread
#define OpenProcessToken model_token
#define GetTokenInformation model_token_info
#define papacc_configuration_personal_layout_binary model_layout
#define papacc_configuration_personal_layout_binary_current model_current
#define QueryDosDeviceW model_dos
#define GetModuleHandleW model_module
#define GetProcAddress model_proc
#define GetFileInformationByHandle model_info
#define GetSecurityInfo model_sd
#define CloseHandle model_close
#define LocalFree model_free
#define papacc_server_console_win32_stop_requested model_stop
#define papacc_server_console_win32_install model_install
#define papacc_server_console_win32_uninstall model_uninstall
#include "../apps/server/server_launcher_chain_metadata_win32.inc"
#undef LocalFree
#define T(x) do{checks++;if(!(x)){printf("FAIL chain scenario=%u line=%u first=%lu opens=%u closes=%u state=%ld\n",scenario,(unsigned)__LINE__,(unsigned long)staging_act()->first,opens,closes,(long)state);return 1;}}while(0)
int main(void)
{
 unsigned i;FILE *out=tmpfile();DWORD ace,mask;PAPACC_QUALIFICATION_STAGING *s;
 if(!out||!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&fixture_operator_sid))return 2;
 for(i=0;i<=25U;i++){
  int result;const WCHAR *sd=L"O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)";
  ZeroMemory(&root,sizeof(root));ZeroMemory(closed,sizeof(closed));state=0;scenario=i;opens=closes=infos=free_calls=token_opens=sd_queries=0;
  if(i==3U)sd=L"O:BAG:BAD:P(A;;0x4;;;AU)(A;;FA;;;BA)";
  if(i==4U)sd=L"O:BAG:BAD:PAI(A;OICIIO;GW;;;AU)(A;ID;GR;;;BU)(A;;FA;;;BA)";
  if(i==25U)sd=L"O:BAG:BAD:P(OA;;GR;11111111-1111-1111-1111-111111111111;;AU)";
  if(i==20U)sd=L"O:BAG:BAD:NO_ACCESS_CONTROL";
  if(i==5U)sd=L"O:WDG:BAD:P(A;;FA;;;BA)";
  if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(sd,SDDL_REVISION_1,&fixture,NULL))return 2;
  result=papacc_server_launcher_chain_metadata(out,out);s=staging_act();
  if(i<=5U){T(result==0);T(state==0&&s->phase==6U);T(opens==(i==1U?3U:4U));T(closes==(i==1U?4U:i==2U?5U:6U));}
  else {T(result==2);T(s->first!=0);}
  T(!s->effects&&!s->consumed);T(!s->objects[0].created);
  if(i==1U)T(s->ancestor_count==2U);
  if(i==2U)T(s->ancestor_count==3U);
  if(i==9U||i==14U||i==17U||i==18U)T(state==(LONG)PAPACC_LAUNCHER_RETAINED);
  if(i==3U||i==5U){DWORD reason;ConvertStringSidToSidW(L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464",&s->trusted_installer);
   reason=staging_descriptor_classify(fixture,1,&ace,&mask);T(reason==(DWORD)(i==3U?STAGE_SD_FOREIGN_WRITE:STAGE_SD_OWNER_UNTRUSTED));if(i==3U){T(mask==4U);T(staging_anchor_classify(fixture,fixture,&ace,&mask)!=STAGE_SD_VALID);}}
  /* Explicit fixture allocator disposal after assertions, not product cleanup. */
  if(s->scratch)LocalFree(s->scratch);if(s->trusted_installer)LocalFree(s->trusted_installer);
  if(root.configuration_commit.pending_commit.personal_native.sid_text)LocalFree(root.configuration_commit.pending_commit.personal_native.sid_text);
  LocalFree(fixture);fixture=NULL;
 }
 /* Existing owner rejects competing entry, no API acquisition. */
 state=PAPACC_LAUNCHER_RETAINED;opens=0;T(papacc_server_launcher_chain_metadata(out,out)==2&&!opens);
 LocalFree(fixture_operator_sid);fclose(out);printf("PASS root chain: 26 scenarios, %u checks; API acquisition modeled; no real directory query or effects\n",checks);return 0;
}
