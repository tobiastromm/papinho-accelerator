/* Private LAB producer, inside SAME SCM owner. No creation or authority. */
#include <aclapi.h>
#include <sddl.h>
#include <winternl.h>
#include "server_io_loop_win32.h"
#include "../../src/config/config_source.h"
#define PAPACC_SCM_DESCRIPTOR_TEST 1
#include "server_launcher_scm_prepare_win32.inc"
#undef PAPACC_SCM_DESCRIPTOR_TEST
static const WCHAR psr_parent[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release";
static const char psr_config_sha[]="a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44";
static DWORD psr_close(void)
{
 HANDLE *v[7];unsigned i;
 v[0]=&native.runtime.relative_image;v[1]=&native.runtime.configuration;v[2]=&native.runtime.access_token;
 v[3]=&native.runtime.unexpected;v[4]=&native.runtime.check_token;v[5]=&native.runtime.token;v[6]=&native.runtime.parent;
 if(host.terminal_receipt==2U||host.life.phase==PS_RETAINED||(host.run_active&&host.terminal_receipt!=1U))return PSRT_RETURN(ERROR_BUSY,"original_branch");
#ifdef PSR_FILE_ENABLED
 if(!psrd_quiescent())return ERROR_BUSY;
 if(psrf_close(&native.runtime.diagnostic.file))return ERROR_BUSY;
#endif
 if(native.runtime.retained)return PSRT_RETURN(ERROR_INVALID_STATE,"original_branch"); /* first failure retained; never retry */
 if(native.runtime.descriptor){if(LocalFree(native.runtime.descriptor)){native.runtime.retained=1U;return PSRT_RETURN(ERROR_INVALID_HANDLE,"original_branch");}native.runtime.descriptor=NULL;}
 if(native.runtime.approved_sid){if(LocalFree(native.runtime.approved_sid)){native.runtime.retained=1U;return PSRT_RETURN(ERROR_INVALID_HANDLE,"original_branch");}native.runtime.approved_sid=NULL;}
 for(i=0;i<7;i++)if(*v[i]){if(!CloseHandle(*v[i])){native.runtime.retained=1U;return ps_last_error();}*v[i]=NULL;}
 native.runtime.ready=0U;native.runtime.managers_ready=0U;SecureZeroMemory(native.runtime.bytes,sizeof(native.runtime.bytes));return ERROR_SUCCESS;
}
static int psr_inside_sid(const BYTE *b,DWORD n,PSID sid)
{
 ULONG_PTR start=(ULONG_PTR)b,p=(ULONG_PTR)sid;DWORD size;
 if(p<start||p-start>n||n-(DWORD)(p-start)<8U)return 0;
 size=8U+4U*((BYTE *)sid)[1];return size<=n-(DWORD)(p-start)&&IsValidSid(sid)&&GetLengthSid(sid)==size;
}
#ifdef PSR_FILE_ENABLED
#include "server_service_psr_file_win32.inc"
#define PSR_IMAGE_LEAF L"papacc_server_lab_host_ntstatus.exe"
#else
#define PSR_IMAGE_LEAF L"papacc_server_lab_host.exe"
#endif
/* Snapshot contents are factual query output, not normalized synthetic grants. */
static DWORD psr_token(void)
{
 DWORD n,i,gn,pn;TOKEN_STATISTICS stats;TOKEN_ELEVATION elev;
 TOKEN_USER *u=(TOKEN_USER *)native.runtime.user.bytes;TOKEN_GROUPS *g=(TOKEN_GROUPS *)native.runtime.groups.bytes;
 TOKEN_PRIVILEGES *priv=(TOKEN_PRIVILEGES *)native.runtime.privileges.bytes;
 BYTE admin[SECURITY_MAX_SID_SIZE],service[SECURITY_MAX_SID_SIZE];DWORD an=sizeof(admin),sn=sizeof(service);int service_group=0,specific_group=0;
 PSRT_CONTEXT("TOKEN_PROCESS_AND_THREAD");
 if(native.runtime.unexpected||native.runtime.token)return PSRT_RETURN(ERROR_INVALID_STATE,"original_branch");
 if(PSRT_BOOL("OpenThreadToken",(TOKEN_QUERY),OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&native.runtime.unexpected)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"unexpected_thread_impersonation_token");
 if(GetLastError()!=ERROR_NO_TOKEN)return PSRT_RETURN(ps_last_error(),"native_api_failure");
 if(!PSRT_BOOL("OpenProcessToken",(TOKEN_QUERY|TOKEN_DUPLICATE),OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_DUPLICATE,&native.runtime.token)))return PSRT_RETURN(ps_last_error(),"native_api_failure");
 if(!PSRT_TOKEN("GetTokenInformation:TokenStatistics",TokenStatistics,&stats,sizeof(stats),&n,GetTokenInformation(native.runtime.token,TokenStatistics,&stats,sizeof(stats),&n))||!PSRT_PRED("token.statistics.length",n==sizeof(stats))||!PSRT_PRED("token.primary",stats.TokenType==TokenPrimary)||
 !PSRT_TOKEN("GetTokenInformation:TokenUser",TokenUser,u,sizeof(native.runtime.user.bytes),&n,GetTokenInformation(native.runtime.token,TokenUser,u,sizeof(native.runtime.user.bytes),&n))||!PSRT_PRED("token.user.length",n<=sizeof(native.runtime.user.bytes))||
 !PSRT_PRED("token.user.sid.bounds",psr_inside_sid(native.runtime.user.bytes,n,u->User.Sid))||!PSRT_PRED("token.user.sid.matches_service",EqualSid(u->User.Sid,native.service_sid)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"authorization_or_native_mapping");
 if(!PSRT_TOKEN("GetTokenInformation:TokenGroups",TokenGroups,g,sizeof(native.runtime.groups.bytes),&gn,GetTokenInformation(native.runtime.token,TokenGroups,g,sizeof(native.runtime.groups.bytes),&gn))||gn>sizeof(native.runtime.groups.bytes)||
 gn<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||g->GroupCount>(gn-FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES)||
 !PSRT_BOOL("CreateWellKnownSid",(0),CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admin,&an))||!PSRT_BOOL("CreateWellKnownSid",(0),CreateWellKnownSid(WinServiceSid,NULL,service,&sn)))return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 for(i=0;i<g->GroupCount;i++){
  if(!psr_inside_sid(native.runtime.groups.bytes,gn,g->Groups[i].Sid))return PSRT_RETURN(ERROR_INVALID_SID,"original_branch");
  if((g->Groups[i].Attributes&SE_GROUP_ENABLED)&&!(g->Groups[i].Attributes&SE_GROUP_USE_FOR_DENY_ONLY)){
   if(!PSRT_PRED("token.no_enabled_administrators",!EqualSid(g->Groups[i].Sid,admin)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"authorization_or_native_mapping");
   if(EqualSid(g->Groups[i].Sid,service))service_group=1;
   if(EqualSid(g->Groups[i].Sid,native.service_sid))specific_group=1;
  }
 }
 if(!PSRT_PRED("token.service_group.enabled",service_group)||!PSRT_PRED("token.specific_group.enabled",specific_group))return PSRT_RETURN(ERROR_ACCESS_DENIED,"authorization_or_native_mapping");
 if(!PSRT_TOKEN("GetTokenInformation:TokenPrivileges",TokenPrivileges,priv,sizeof(native.runtime.privileges.bytes),&pn,GetTokenInformation(native.runtime.token,TokenPrivileges,priv,sizeof(native.runtime.privileges.bytes),&pn))||pn>sizeof(native.runtime.privileges.bytes)||
 pn<(DWORD)FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges)||priv->PrivilegeCount>(pn-FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges))/sizeof(LUID_AND_ATTRIBUTES)||
 !PSRT_TOKEN("GetTokenInformation:TokenElevation",TokenElevation,&elev,sizeof(elev),&n,GetTokenInformation(native.runtime.token,TokenElevation,&elev,sizeof(elev),&n))||n!=sizeof(elev))return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 native.runtime.statistics=stats;native.runtime.group_bytes=gn;native.runtime.privilege_bytes=pn;native.runtime.elevated=elev.TokenIsElevated;
 /* Keep full group/privilege entries/attributes through runtime quiescence. */
 if(!PSRT_BOOL("DuplicateToken",(TOKEN_DUPLICATE),DuplicateToken(native.runtime.token,SecurityImpersonation,&native.runtime.access_token)))return PSRT_RETURN(ps_last_error(),"native_api_failure");
 return ERROR_SUCCESS;
}
typedef NTSTATUS (NTAPI *PSR_CREATE)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
static DWORD psr_open(HANDLE parent,const WCHAR *name,HANDLE *out,int directory)
{
 UNICODE_STRING str;OBJECT_ATTRIBUTES attr;IO_STATUS_BLOCK io;HMODULE m;FARPROC proc;PSR_CREATE create;NTSTATUS status;size_t len;
 if(*out||!name)return PSRT_RETURN(ERROR_INVALID_STATE,"original_branch");len=wcslen(name);if(!len||len>512U)return PSRT_RETURN(ERROR_INVALID_NAME,"original_branch");
 PSRT_CONTEXT(directory?"PARENT":(!wcscmp(name,L"papacc.conf")?"CONFIG":"IMAGE"));
 m=GetModuleHandleW(L"ntdll.dll");if(!m)return PSRT_RETURN(ERROR_NOT_SUPPORTED,"original_branch");proc=GetProcAddress(m,"NtCreateFile");if(!proc)return PSRT_RETURN(ERROR_NOT_SUPPORTED,"original_branch");memcpy(&create,&proc,sizeof(create));
 ZeroMemory(&attr,sizeof(attr));ZeroMemory(&io,sizeof(io));str.Buffer=(WCHAR *)name;str.Length=(USHORT)(len*sizeof(WCHAR));str.MaximumLength=str.Length;
 attr.Length=sizeof(attr);attr.RootDirectory=parent;attr.ObjectName=&str;attr.Attributes=OBJ_CASE_INSENSITIVE|0x1000U;
 status=create(out,(directory?(FILE_TRAVERSE|FILE_READ_ATTRIBUTES):GENERIC_READ)|READ_CONTROL|SYNCHRONIZE,&attr,&io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1U,directory?0x00200021U:0x00200060U,NULL,0U);
 (void)PSRT_NT("NtCreateFile",(directory?(FILE_TRAVERSE|FILE_READ_ATTRIBUTES):GENERIC_READ)|READ_CONTROL|SYNCHRONIZE,status);
 if(status<0){if(*out)native.runtime.retained=1U;return PSRT_RETURN(ERROR_ACCESS_DENIED,"authorization_or_native_mapping");}return ERROR_SUCCESS;
}
static DWORD psr_acl(HANDLE file,unsigned slot,int image)
{
 PSECURITY_DESCRIPTOR sd=NULL;PSID approved=NULL;DWORD e,n,got=sizeof(native.runtime.access_privileges.bytes),granted=0;BOOL allowed=FALSE;int ok;SECURITY_DESCRIPTOR_CONTROL control;DWORD revision;
 unsigned char hash[32];
 GENERIC_MAPPING mapping={FILE_GENERIC_READ,FILE_GENERIC_WRITE,FILE_GENERIC_EXECUTE,FILE_ALL_ACCESS};
 PSRT_CONTEXT(slot==0?"PARENT_ACL":slot==1?"CONFIG_ACL":"IMAGE_ACL");
 if(slot>=3U||native.runtime.descriptor||native.runtime.approved_sid)return PSRT_RETURN(ERROR_INVALID_STATE,"original_branch");
 if(!PSRT_BOOL("ConvertStringSidToSidW",(0),ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&approved)))return PSRT_RETURN(ps_last_error(),"native_api_failure");native.runtime.approved_sid=approved;
 e=PSRT_STATUS("GetSecurityInfo",READ_CONTROL,GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&sd));native.runtime.descriptor=sd;if(e){if(sd)native.runtime.retained=1U;return PSRT_RETURN(e,"propagate_original");}
 n=GetSecurityDescriptorLength(sd);ok=PSRT_PRED("sd.length",n&&n<=65536U)&&PSRT_BOOL("GetSecurityDescriptorControl",(0),GetSecurityDescriptorControl(sd,&control,&revision))&&PSRT_PRED("sd.protected",(control&SE_DACL_PROTECTED)!=0)&&PSRT_PRED("sd.scm_descriptor",scm_descriptor(sd,(BYTE *)approved));
 if(ok)ok=PSRT_BOOL("AccessCheck",(MAXIMUM_ALLOWED),AccessCheck(sd,native.runtime.access_token,MAXIMUM_ALLOWED,&mapping,(PRIVILEGE_SET *)native.runtime.access_privileges.bytes,&got,&granted,&allowed))&&PSRT_PRED("accesscheck.privilege_buffer_bound",got<=sizeof(native.runtime.access_privileges.bytes))&&PSRT_PRED("accesscheck.access_status",allowed);
 PSRT_VALUES(granted,got);
 if(ok)ok=PSRT_PRED("acl.no_write_delete_owner",(granted&(FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|DELETE|WRITE_DAC|WRITE_OWNER|FILE_DELETE_CHILD))==0)&&
 PSRT_PRED("acl.required_read_execute",(granted&(FILE_READ_DATA|(image?FILE_EXECUTE:0)))==(DWORD)(FILE_READ_DATA|(image?FILE_EXECUTE:0)));
 if(ok)ok=PSRT_PRED("acl.hash",pacc_storage_hash_callback(NULL,(const unsigned char *)sd,n,hash))&&
 PSRT_PRED("acl.immutable",(!(native.runtime.acl_captured&(1U<<slot))||!memcmp(hash,native.runtime.acl_hash[slot],32)));
 if(ok&&!(native.runtime.acl_captured&(1U<<slot))){memcpy(native.runtime.acl_hash[slot],hash,32);native.runtime.acl_captured|=1U<<slot;}
 if(LocalFree(sd)){native.runtime.retained=1U;return PSRT_RETURN(ERROR_INVALID_HANDLE,"original_branch");}native.runtime.descriptor=NULL;
 if(LocalFree(approved)){native.runtime.retained=1U;return PSRT_RETURN(ERROR_INVALID_HANDLE,"original_branch");}native.runtime.approved_sid=NULL;
 return PSRT_RETURN(ok?ERROR_SUCCESS:ERROR_ACCESS_DENIED,"acl.policy_or_effective_access");
}
static DWORD psr_current(void)
{
 BY_HANDLE_FILE_INFORMATION a;TOKEN_STATISTICS stats;DWORD n,read;LARGE_INTEGER zero;unsigned char digest[32];char hex[65];unsigned i;
 PSRT_CONTEXT("CURRENT_SOURCE_AND_TOKEN");
 if(native.runtime.retained||papacc_server_console_win32_stop_requested()||!native.runtime.parent||!native.runtime.configuration||!native.runtime.token)return PSRT_RETURN(ERROR_OPERATION_ABORTED,"original_branch");
 if(native.runtime.unexpected||native.runtime.check_token)return PSRT_RETURN(ERROR_INVALID_STATE,"original_branch");
 if(PSRT_BOOL("OpenThreadToken",(TOKEN_QUERY),OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&native.runtime.unexpected)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"authorization_or_native_mapping");
 if(GetLastError()!=ERROR_NO_TOKEN||!PSRT_BOOL("OpenProcessToken",(TOKEN_QUERY),OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&native.runtime.check_token)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"authorization_or_native_mapping");
 {
 int same=PSRT_TOKEN("GetTokenInformation:TokenStatistics",TokenStatistics,&stats,sizeof(stats),&n,GetTokenInformation(native.runtime.check_token,TokenStatistics,&stats,sizeof(stats),&n))&&n==sizeof(stats)&&
 !memcmp(&stats.TokenId,&native.runtime.statistics.TokenId,sizeof(LUID))&&
 !memcmp(&stats.AuthenticationId,&native.runtime.statistics.AuthenticationId,sizeof(LUID))&&!memcmp(&stats.ModifiedId,&native.runtime.statistics.ModifiedId,sizeof(LUID));
 if(!CloseHandle(native.runtime.check_token)){native.runtime.retained=1U;return PSRT_RETURN(ERROR_INVALID_HANDLE,"original_branch");}native.runtime.check_token=NULL;
 if(!PSRT_PRED("token.same_id_auth_modified",same))return PSRT_RETURN(ERROR_ACCESS_DENIED,"authorization_or_native_mapping");
 }
 if(!PSRT_BOOL("GetFileInformationByHandle",(FILE_READ_ATTRIBUTES),GetFileInformationByHandle(native.runtime.parent,&a))||a.dwVolumeSerialNumber!=417614100UL||a.nFileIndexHigh!=1966080UL||a.nFileIndexLow!=79426UL||
 !(a.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(a.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 if(!PSRT_BOOL("GetFileInformationByHandle",(FILE_READ_ATTRIBUTES),GetFileInformationByHandle(native.runtime.relative_image,&a))||!ps_image_same(&a,&native.image_id)||
 (a.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 if(!PSRT_BOOL("GetFileInformationByHandle",(FILE_READ_ATTRIBUTES),GetFileInformationByHandle(native.runtime.configuration,&a))||a.dwVolumeSerialNumber!=417614100UL||a.nFileIndexHigh!=393216UL||a.nFileIndexLow!=1120240UL||
 a.nFileSizeHigh||a.nFileSizeLow!=116U||(a.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 zero.QuadPart=0;if(!PSRT_BOOL("SetFilePointerEx",(FILE_READ_DATA),SetFilePointerEx(native.runtime.configuration,zero,NULL,FILE_BEGIN))||!PSRT_BOOL("ReadFile",(FILE_READ_DATA),ReadFile(native.runtime.configuration,native.runtime.bytes,sizeof(native.runtime.bytes),&read,NULL))||read!=116U)return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 if(!pacc_storage_hash_callback(NULL,native.runtime.bytes,read,digest))return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 for(i=0;i<32;i++){hex[2*i]="0123456789abcdef"[digest[i]>>4];hex[2*i+1]="0123456789abcdef"[digest[i]&15];}hex[64]=0;
 if(strcmp(hex,psr_config_sha))return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 {DWORD e;if((e=psr_acl(native.runtime.parent,0U,0))||(e=psr_acl(native.runtime.configuration,1U,0))||(e=psr_acl(native.runtime.relative_image,2U,1)))return PSRT_RETURN(e,"propagate_original");}
 return ERROR_SUCCESS;
}
static DWORD psr_prepare(void)
{
 WCHAR device[256],path[512];size_t a,b;DWORD e;SERVICE_STATUS_PROCESS status;DWORD got=0;
 PC_DOCUMENT document;PC_DIAGNOSTIC diagnostic;PC_TARGET target={1U,0};
#ifdef PSR_FILE_ENABLED
 e=psrd_bind();if(e)return e;
#endif
 PSRT_CONTEXT("PREPARE");
 if(native.runtime.ready||native.runtime.retained||native.manager||native.service||native.image)return PSRT_RETURN(ERROR_INVALID_STATE,"original_branch");
 e=ps_image_open();if(e)return PSRT_RETURN(e,"propagate_original");
 native.manager=PSRT_HANDLE("OpenSCManagerW",(SC_MANAGER_CONNECT),OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT));if(!native.manager)return PSRT_RETURN(ps_last_error(),"native_api_failure");
 native.service=PSRT_HANDLE("OpenServiceW",(SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS),OpenServiceW(native.manager,ps_name,SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS));if(!native.service)return PSRT_RETURN(ps_last_error(),"native_api_failure");
 e=ps_virtual_readback_start(SERVICE_DEMAND_START);if(e)return PSRT_RETURN(e,"propagate_original");
 ZeroMemory(&status,sizeof(status));if(!PSRT_BOOL("QueryServiceStatusEx",(SERVICE_QUERY_STATUS),QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(BYTE *)&status,sizeof(status),&got))||got!=sizeof(status)||
 status.dwProcessId!=GetCurrentProcessId()||status.dwCurrentState!=SERVICE_START_PENDING)return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
 e=psr_token();if(e)return PSRT_RETURN(e,"propagate_original");
 a=wcslen(psr_parent);if(wcsncmp(native.image_path,psr_parent,a)||native.image_path[a]!=L'\\'||wcscmp(native.image_path+a+1,PSR_IMAGE_LEAF))return PSRT_RETURN(ERROR_INVALID_NAME,"original_branch");
 if(!PSRT_COUNT("QueryDosDeviceW",QueryDosDeviceW(L"C:",device,256))||wcsncmp(device,L"\\Device\\",8))return PSRT_RETURN(ERROR_INVALID_NAME,"original_branch");
 a=wcslen(device);b=wcslen(psr_parent+2);if(a+b>=512)return PSRT_RETURN(ERROR_INVALID_NAME,"original_branch");memcpy(path,device,a*sizeof(WCHAR));memcpy(path+a,psr_parent+2,(b+1)*sizeof(WCHAR));
 if((e=psr_open(NULL,path,&native.runtime.parent,1))||
 (e=psr_open(native.runtime.parent,L"papacc.conf",&native.runtime.configuration,0))||
 (e=psr_open(native.runtime.parent,PSR_IMAGE_LEAF,&native.runtime.relative_image,0)))return PSRT_RETURN(e,"propagate_original");
 if((e=psr_current()))return PSRT_RETURN(e,"propagate_current");
 if(pc_parse(native.runtime.bytes,116U,&document,&diagnostic)!=PC_OK||pc_validate(&document,&target,&diagnostic)!=PC_OK)return PSRT_RETURN(ERROR_INVALID_DATA,"original_branch");
#ifdef PSR_FILE_ENABLED
 return psrd_prepare_complete();
#else
 native.runtime.ready=1U;PSRT_PREPARED();return ERROR_SUCCESS;
#endif
}

int papacc_server_service_qualification_managers_ready(const struct PAPACC_SERVER_IO_LOOP_WIN32 *l)
{
 if(!papacc_server_service_qualification_active()||!l||!l->initialized||!l->network||l->network->is_active||l->network->tcp_platform.initialized||
 l->network->listener_set.count||l->network->listener_set.is_active||l->network->listener_storage||l->acceptor||!l->connection_manager||!l->connection_manager->initialized||l->connection_manager->count||
 !l->session_manager.initialized||l->session_manager.count||!l->channel_manager.initialized||l->channel_manager.count||
 !l->network_job_manager.initialized||l->network_job_manager.count||l->egress_configured||l->dns_resolver_configured||!l->network_stream_manager.initialized||l->network_stream_manager.count||l->enabled_capabilities.count||
 l->c2_owner||psr_current()!=ERROR_SUCCESS)return 0;
 native.runtime.managers_ready=1U;return papacc_server_service_runtime_ready();
}
