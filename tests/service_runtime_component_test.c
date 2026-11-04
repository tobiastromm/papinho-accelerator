#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <aclapi.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
#include "server_console_win32.h"
#include "server_service_identity_win32.h"
static unsigned model_mode,model_closes,model_next,model_stop_flag,model_stop_on_running,model_reports;
static DWORD model_states[8];
static BYTE model_sid[SECURITY_MAX_SID_SIZE],model_admin[SECURITY_MAX_SID_SIZE],model_service[SECURITY_MAX_SID_SIZE];

#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
static BYTE model_relative_sd[2048];
#endif
static SECURITY_DESCRIPTOR model_sd;static BYTE model_acl_bytes[1024];
static const char model_config[]="format_version 1\r\nclient_admission DISABLED\r\nsite_tls_admission DENY\r\ned11_admission DENY\r\nnetwork_egress DISABLED\r\n";
#ifdef PAPACC_SERVICE_RUNTIME_MODEL
/* Inject only NtCreateFile lookup: no native file or SCM mutation. */
static NTSTATUS model_nt_status;static unsigned model_nt_handle;static ACCESS_MASK model_nt_access;
static NTSTATUS NTAPI m_ntcreate(PHANDLE out,ACCESS_MASK access,POBJECT_ATTRIBUTES attr,PIO_STATUS_BLOCK io,PLARGE_INTEGER size,ULONG flags,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_size)
{
 (void)attr;(void)io;(void)size;(void)flags;(void)share;(void)disposition;(void)options;(void)ea;(void)ea_size;
 model_nt_access=access;if(model_nt_handle)*out=(HANDLE)(ULONG_PTR)1001;return model_nt_status;
}
static FARPROC WINAPI m_proc(HMODULE module,LPCSTR name)
{
 FARPROC out;NTSTATUS (NTAPI *fn)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG)=m_ntcreate;
 if(strcmp(name,"NtCreateFile"))return GetProcAddress(module,name);memcpy(&out,&fn,sizeof(out));return out;
}
#define GetProcAddress m_proc
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
static HANDLE model_r4_duplicate,model_r4_original;
static unsigned model_r4_fault,model_r4_query_count,model_r4_duplicate_stats,model_r4_calls_at_stop;
static DWORD psr_r4_current(void);
static DWORD psr_current(void);
static BOOL WINAPI m_r4_member(HANDLE token,PSID sid,PBOOL member){(void)token;(void)sid;if(model_r4_fault==2U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}if(model_r4_fault==13U){model_stop_flag=1;model_r4_calls_at_stop=model_r4_query_count;}if(model_r4_fault==16U)(void)psr_current();*member=model_r4_fault==1U?FALSE:TRUE;return TRUE;}
#define CheckTokenMembership m_r4_member
#endif
static BOOL WINAPI m_process(HANDLE process,DWORD rights,PHANDLE token){(void)process;(void)rights;*token=(HANDLE)(ULONG_PTR)(++model_next);
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 if(!model_r4_original)model_r4_original=*token;
#endif
 return TRUE;}
static BOOL WINAPI m_thread(HANDLE thread,DWORD rights,BOOL self,PHANDLE token){(void)thread;(void)rights;(void)self;if(model_mode==3U){*token=(HANDLE)(ULONG_PTR)(++model_next);return TRUE;}SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI m_duplicate(HANDLE h,SECURITY_IMPERSONATION_LEVEL level,PHANDLE out){(void)h;if(level!=SecurityImpersonation)return FALSE;*out=(HANDLE)(ULONG_PTR)(++model_next);
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 model_r4_duplicate=*out;
#endif
 return TRUE;}
#ifdef PAPACC_PSR_FILE_RUNTIME_LAB
#include "service_runtime_file_fixture.inc"
#endif
static BOOL WINAPI m_close(HANDLE h){
#ifdef PAPACC_PSR_FILE_RUNTIME_LAB
 if(h==(HANDLE)(ULONG_PTR)2001||h==(HANDLE)(ULONG_PTR)2002)return dm_close(h);
#endif
 (void)h;model_closes++;if(model_mode==9U){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static BOOL WINAPI m_token(HANDLE h,TOKEN_INFORMATION_CLASS kind,LPVOID b,DWORD capacity,PDWORD used)
{
 DWORD n=0;(void)h;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 model_r4_query_count++;
 if((model_r4_fault==8U&&kind==TokenRestrictedSids)||(model_r4_fault==9U&&kind==TokenHasRestrictions)||(model_r4_fault==10U&&h==model_r4_duplicate&&kind==TokenStatistics)){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
#endif
 if(kind==TokenStatistics){TOKEN_STATISTICS *v=b;n=sizeof(*v);if(capacity<n)return FALSE;ZeroMemory(v,n);v->TokenType=model_mode==17U?TokenImpersonation:TokenPrimary;v->TokenId.LowPart=1;v->AuthenticationId.LowPart=2;v->ModifiedId.LowPart=model_mode==10U?2:1;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 if(h==model_r4_duplicate){v->TokenType=TokenImpersonation;v->TokenId.LowPart=77;model_r4_duplicate_stats++;if(model_r4_fault==3U)v->AuthenticationId.LowPart=99;if(model_r4_fault==4U&&model_r4_duplicate_stats>=3U)v->ModifiedId.LowPart++;}
 if(model_r4_fault==12U&&h!=model_r4_duplicate&&h!=model_r4_original)v->TokenId.LowPart++;
#endif
}
 else if(kind==TokenUser){TOKEN_USER *v=b;PSID sid=model_mode==1U?model_admin:model_sid;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 if(model_r4_fault==11U&&h==model_r4_duplicate)sid=model_admin;
#endif
n=sizeof(*v)+GetLengthSid(sid);if(capacity<n)return FALSE;ZeroMemory(b,capacity);v->User.Sid=(BYTE *)b+sizeof(*v);CopySid(GetLengthSid(sid),v->User.Sid,sid);}
 else if(kind==TokenGroups){TOKEN_GROUPS *v=b;if(model_mode==16U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}BYTE *next=(BYTE *)b+FIELD_OFFSET(TOKEN_GROUPS,Groups)+3U*sizeof(SID_AND_ATTRIBUTES);unsigned i;PSID sids[3];sids[0]=model_service;sids[1]=model_sid;sids[2]=model_admin;ZeroMemory(b,capacity);v->GroupCount=model_mode==2U?3U:(model_mode==13U?1U:2U);for(i=0;i<v->GroupCount;i++){v->Groups[i].Sid=next;v->Groups[i].Attributes=SE_GROUP_ENABLED;if(model_mode==4U&&i==1U)v->Groups[i].Attributes=SE_GROUP_USE_FOR_DENY_ONLY;if(model_mode==14U&&i==1U)v->Groups[i].Attributes=0;if(model_mode==15U&&i==1U)v->Groups[i].Attributes=SE_GROUP_ENABLED|SE_GROUP_USE_FOR_DENY_ONLY;if(model_mode==18U&&i==1U)v->Groups[i].Attributes=SE_GROUP_ENABLED_BY_DEFAULT|SE_GROUP_OWNER;CopySid(GetLengthSid(sids[i]),next,sids[i]);next+=GetLengthSid(sids[i]);}n=(DWORD)(next-(BYTE *)b);if(model_mode==5U)v->Groups[0].Sid=(BYTE *)b+capacity;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 if(model_r4_fault==6U)n=(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)-1U;
 if(model_r4_fault==7U){v->GroupCount=3U;v->Groups[2].Sid=(BYTE *)v->Groups[1].Sid+GetLengthSid(model_sid);v->Groups[2].Attributes=SE_GROUP_ENABLED;CopySid(GetLengthSid(model_sid),v->Groups[2].Sid,model_sid);n=(DWORD)((BYTE *)v->Groups[2].Sid+GetLengthSid(model_sid)-(BYTE *)b);}
#endif
}
 else if(kind==TokenPrivileges){TOKEN_PRIVILEGES *v=b;ZeroMemory(b,capacity);v->PrivilegeCount=0;n=(DWORD)FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges);}
 else if(kind==TokenHasRestrictions){if(model_mode==19U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}if(capacity<sizeof(DWORD))return FALSE;*(DWORD *)b=1;n=1;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 *(DWORD *)b=model_r4_fault==14U?1U:0U;
#endif
}
 else if(kind==TokenRestrictedSids){TOKEN_GROUPS *v=b;ZeroMemory(b,capacity);v->GroupCount=0;n=(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups);
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
 if(model_r4_fault==15U){v->GroupCount=1;v->Groups[0].Sid=(BYTE *)b+FIELD_OFFSET(TOKEN_GROUPS,Groups)+sizeof(SID_AND_ATTRIBUTES);CopySid(GetLengthSid(model_service),v->Groups[0].Sid,model_service);n=(DWORD)((BYTE *)v->Groups[0].Sid+GetLengthSid(model_service)-(BYTE *)b);}
#endif
}
 else if(kind==TokenElevation){TOKEN_ELEVATION *v=b;v->TokenIsElevated=0;n=sizeof(*v);}
 else return FALSE;*used=n;return TRUE;
}
static BOOL WINAPI m_info(HANDLE h,LPBY_HANDLE_FILE_INFORMATION i)
{
#ifdef PAPACC_PSR_FILE_RUNTIME_LAB
 if(h==(HANDLE)(ULONG_PTR)2001||h==(HANDLE)(ULONG_PTR)2002)return dm_info(h,i);
#endif
 ZeroMemory(i,sizeof(*i));i->dwVolumeSerialNumber=417614100UL;if(model_mode==12U)return FALSE;
 if(h==(HANDLE)(ULONG_PTR)1001){i->nFileIndexHigh=1966080UL;i->nFileIndexLow=79426UL;i->dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;}
 else if(h==(HANDLE)(ULONG_PTR)1002){i->nFileIndexHigh=393216UL;i->nFileIndexLow=model_mode==7U?1120241UL:1120240UL;i->nFileSizeLow=116U;}
 else if(h==(HANDLE)(ULONG_PTR)1003){i->nFileIndexHigh=262144UL;i->nFileIndexLow=1119989UL;}
 else return FALSE;return TRUE;
}
static BOOL WINAPI m_seek(HANDLE h,LARGE_INTEGER offset,PLARGE_INTEGER result,DWORD method){(void)h;(void)offset;(void)result;(void)method;return TRUE;}
static BOOL WINAPI m_read(HANDLE h,LPVOID b,DWORD n,LPDWORD used,LPOVERLAPPED ov){
#ifdef PAPACC_PSR_FILE_RUNTIME_LAB
 if(h==(HANDLE)(ULONG_PTR)2002)return dm_read(h,b,n,used,ov);
#endif
(void)h;(void)ov;if(n<116U||model_mode==11U)return FALSE;memcpy(b,model_config,116U);if(model_mode==6U)((BYTE *)b)[0]='X';*used=116U;return TRUE;}
static DWORD WINAPI m_security(HANDLE h,SE_OBJECT_TYPE type,SECURITY_INFORMATION info,PSID *owner,PSID *group,PACL *dacl,PACL *sacl,PSECURITY_DESCRIPTOR *sd){(void)h;(void)type;(void)info;(void)owner;(void)group;(void)dacl;(void)sacl;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
{DWORD size=sizeof(model_relative_sd);if(!MakeSelfRelativeSD(&model_sd,model_relative_sd,&size))return GetLastError();*sd=model_relative_sd;}
#else
*sd=&model_sd;
#endif
return ERROR_SUCCESS;}
static HLOCAL WINAPI m_free(HLOCAL v){if(v==&model_sd)return NULL;
#ifdef PAPACC_R0R9_VIRTUAL_USER_LAB
if(v==model_relative_sd)return NULL;
#endif
return LocalFree(v);}
static BOOL WINAPI m_access(PSECURITY_DESCRIPTOR sd,HANDLE token,DWORD desired,PGENERIC_MAPPING mapping,PPRIVILEGE_SET privileges,LPDWORD size,LPDWORD granted,LPBOOL allowed){(void)sd;(void)token;(void)mapping;(void)privileges;if(desired!=MAXIMUM_ALLOWED)return FALSE;*size=sizeof(PRIVILEGE_SET);*granted=FILE_GENERIC_READ|FILE_GENERIC_EXECUTE;if(model_mode==8U)*granted|=FILE_WRITE_DATA;*allowed=TRUE;return TRUE;}
static PAPACC_BOOL m_stopped(void){return model_stop_flag?PAPACC_TRUE:PAPACC_FALSE;}
static void m_stop(void){model_stop_flag=1U;}
static BOOL WINAPI m_report(SERVICE_STATUS_HANDLE h,LPSERVICE_STATUS status){(void)h;if(model_reports>=8U)return FALSE;model_states[model_reports++]=status->dwCurrentState;if(model_stop_on_running&&status->dwCurrentState==SERVICE_RUNNING)model_stop_flag=1U;return TRUE;}
#define papacc_server_console_win32_stop_requested m_stopped
#define papacc_server_console_win32_request_stop m_stop
#define SetServiceStatus m_report
#define OpenProcessToken m_process
#define OpenThreadToken m_thread
#define GetTokenInformation m_token
#define DuplicateToken m_duplicate
#define CloseHandle m_close
#define GetFileInformationByHandle m_info
#define SetFilePointerEx m_seek
#define ReadFile m_read
#define GetSecurityInfo m_security
#define LocalFree m_free
#define AccessCheck m_access
#ifdef PAPACC_PSR_FILE_RUNTIME_LAB
#define CreateFileW dm_create
#define GetFinalPathNameByHandleW dm_name
#define GetKernelObjectSecurity dm_security
#define GetFileSizeEx dm_size_fn
#define GetFileType dm_type
#define WriteFile dm_write
#define FlushFileBuffers dm_flush
#endif
#endif
#define PAPACC_SCM_REGISTRATION_LAB 1
#define PAPACC_SCM_RUNTIME_LAB 1
#include "server_service_win32.h"
/* Compile-time negative boundary, never an installable product callback. */
static DWORD model_no_start_carrier(PS_SCQ_CARRIER *out){(void)out;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_take(const PAPACC_SERVICE_IMAGE_VIEW *view){(void)view;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_progress(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_wait(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_terminal(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_finish(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_namespace(void){return ERROR_ACCESS_DENIED;}
static DWORD model_no_first_controller(void){return ERROR_ACCESS_DENIED;}
static DWORD model_prestart_close(void){return 0;}
static DWORD model_no_start_resource(void){return ERROR_ACCESS_DENIED;}
#define papacc_server_launcher_scq_start_resource_current model_no_start_resource
static DWORD model_no_namespace_prepare(void){return ERROR_ACCESS_DENIED;}
#define papacc_server_launcher_scq_start_namespace_prepare model_no_namespace_prepare
#define papacc_server_launcher_scq_first_controller model_no_first_controller
#define papacc_server_launcher_scq_prestart_close model_prestart_close
#define papacc_server_launcher_scq_start_carrier model_no_start_carrier
#define papacc_server_launcher_scq_start_take model_no_start_take
#define papacc_server_launcher_scq_start_progress model_no_start_progress
#define papacc_server_launcher_scq_start_wait model_no_start_wait
#define papacc_server_launcher_scq_start_wait_terminal model_no_start_terminal
#define papacc_server_launcher_scq_start_finish model_no_start_finish
#define papacc_server_launcher_scq_start_namespace_current model_no_start_namespace
#include "../apps/server/server_service_win32.c"
#define T(x) do{if(!(x)){printf("FAIL line=%d mode=%u\n",__LINE__,model_mode);return 1;}}while(0)
static int ids(void)
{
 PSID p=NULL;DWORD n=sizeof(model_admin),sn=sizeof(model_service);
 if(!ConvertStringSidToSidW(L"S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451",&p))return 0;
 if(!CopySid(sizeof(model_sid),model_sid,p)||LocalFree(p))return 0;
 if(!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,model_admin,&n)||!CreateWellKnownSid(WinServiceSid,NULL,model_service,&sn))return 0;
 memcpy(native.service_sid,model_sid,GetLengthSid(model_sid));native.sid_size=GetLengthSid(model_sid);return 1;
}
int main(void)
{
 T(ids());
#ifdef PAPACC_SERVICE_RUNTIME_MODEL
 {
 /* Values copied from Windows SDK ntstatus.h, not Win32 mappings. */
 const LONG statuses[]={(LONG)0xc0000022UL,(LONG)0xc0000034UL,(LONG)0xc000003aUL,(LONG)0xc0000043UL,(LONG)0xc0000035UL,(LONG)0xc000000dUL,(LONG)0xe1234567UL};
 unsigned j,with_handle;DWORD e;
 for(j=0;j<sizeof(statuses)/sizeof(statuses[0]);j++)for(with_handle=0;with_handle<2;with_handle++){
  ZeroMemory(&native.runtime,sizeof(native.runtime));model_nt_status=statuses[j];model_nt_handle=with_handle;
#ifdef PAPACC_PSR_TRACE_LAB
  ZeroMemory(&psrt_record,sizeof(psrt_record));
#endif
  e=psr_open(NULL,L"\\Device\\MODEL_ONLY",&native.runtime.parent,1);
  T(e==ERROR_ACCESS_DENIED);T(model_nt_access==(FILE_TRAVERSE|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE));
  T(native.runtime.retained==with_handle);T((native.runtime.parent!=NULL)==(with_handle!=0));
#ifdef PAPACC_PSR_TRACE_LAB
  T(psrt_record.native_valid&&psrt_record.ntstatus==statuses[j]&&psrt_record.emitted&&psrt_record.returned==e);
  T(PSRT_RETURN(e,"prepare_propagation")==e&&psrt_record.ntstatus==statuses[j]);
#endif
  /* Model fixture only: no real handle exists; do not infer native release. */
 }
 ZeroMemory(&native.runtime,sizeof(native.runtime));model_nt_handle=0;
 }
#endif

#ifdef PAPACC_SERVICE_RUNTIME_MODEL
 {
 unsigned i;DWORD e;PSID approved=NULL;
 T(sizeof(model_config)-1==116U);T(ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&approved));
 T(InitializeSecurityDescriptor(&model_sd,SECURITY_DESCRIPTOR_REVISION));T(SetSecurityDescriptorOwner(&model_sd,approved,FALSE));T(SetSecurityDescriptorGroup(&model_sd,model_admin,FALSE));
 T(InitializeAcl((PACL)model_acl_bytes,sizeof(model_acl_bytes),ACL_REVISION));T(AddAccessAllowedAce((PACL)model_acl_bytes,ACL_REVISION,FILE_GENERIC_READ|FILE_GENERIC_EXECUTE,model_sid));
 T(SetSecurityDescriptorDacl(&model_sd,TRUE,(PACL)model_acl_bytes,FALSE));T(SetSecurityDescriptorControl(&model_sd,SE_DACL_PROTECTED,SE_DACL_PROTECTED));
 for(i=0;i<=5;i++){
  ZeroMemory(&native.runtime,sizeof(native.runtime));model_mode=i;
  e=psr_token();T(i==0U?e==ERROR_SUCCESS:e!=ERROR_SUCCESS);
#ifdef PAPACC_PSR_TRACE_LAB
  if(i==0U)T(psrt_record.value_a==1U&&psrt_record.value_b==SE_GROUP_ENABLED);
#endif
  T(psr_close()==ERROR_SUCCESS);
 }
 /* Missing, disabled, contradictory enabled+deny-only, and query failure.
  * These are API-substituted facts, not a reconstruction of the exited service. */
 for(i=13U;i<=18U;i++){
  ZeroMemory(&native.runtime,sizeof(native.runtime));model_mode=i;
#ifdef PAPACC_PSR_TRACE_LAB
  ZeroMemory(&psrt_record,sizeof(psrt_record));
#endif
  T(psr_token()!=ERROR_SUCCESS);
#ifdef PAPACC_PSR_TRACE_LAB
  if(i<16U){T(psrt_record.emitted&&psrt_record.returned==ERROR_ACCESS_DENIED&&!psrt_record.native_valid);
   T(psrt_record.value_a==(i==13U?0U:1U));T(psrt_record.value_b==(i==15U?(SE_GROUP_ENABLED|SE_GROUP_USE_FOR_DENY_ONLY):0U));
   T(psrt_record.predicate&&!strcmp(psrt_record.predicate,"token.specific_group.enabled"));}
#endif
  T(psr_close()==ERROR_SUCCESS);
 }
 ZeroMemory(&native.runtime,sizeof(native.runtime));model_mode=0;T(psr_token()==ERROR_SUCCESS);
 native.runtime.parent=(HANDLE)(ULONG_PTR)1001;native.runtime.configuration=(HANDLE)(ULONG_PTR)1002;native.runtime.relative_image=(HANDLE)(ULONG_PTR)1003;
 T(m_info(native.runtime.relative_image,&native.image_id));T(psr_current()==ERROR_SUCCESS);
 model_mode=6;T(psr_current()!=ERROR_SUCCESS);model_mode=7;T(psr_current()!=ERROR_SUCCESS);
 model_mode=8;T(psr_current()!=ERROR_SUCCESS);model_mode=10;T(psr_current()!=ERROR_SUCCESS);model_mode=11;T(psr_current()!=ERROR_SUCCESS);model_mode=12;T(psr_current()!=ERROR_SUCCESS);
 model_mode=0;T(psr_current()==ERROR_SUCCESS);T(psr_close()==ERROR_SUCCESS);
 {
 static PAPACC_SERVER_NETWORK net=PAPACC_SERVER_NETWORK_INITIALIZER;
 static PAPACC_SERVER_IO_LOOP_WIN32 loop=PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
 static PAPACC_CONNECTION_MANAGER cm=PAPACC_CONNECTION_MANAGER_INITIALIZER;
 PAPACC_BOOL done=PAPACC_FALSE;unsigned scenario;
 for(scenario=0U;scenario<3U;scenario++){
  ZeroMemory(&native.runtime,sizeof(native.runtime));ZeroMemory(&host,sizeof(host));model_stop_flag=model_stop_on_running=model_reports=0U;
  T(psr_token()==ERROR_SUCCESS);native.runtime.parent=(HANDLE)(ULONG_PTR)1001;native.runtime.configuration=(HANDLE)(ULONG_PTR)1002;native.runtime.relative_image=(HANDLE)(ULONG_PTR)1003;T(psr_current()==ERROR_SUCCESS);native.runtime.ready=1U;
#ifdef PSR_FILE_ENABLED
  native.runtime.diagnostic.receipt=2U; /* model-only prior capture receipt */
#endif
  host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)9;host.thread=GetCurrentThreadId();host.run_active=1U;T(ps_begin(&host.life));
  native.runtime.managers_ready=1U;host.thread++;i=model_next;T(!papacc_server_service_runtime_ready()&&model_next==i);host.thread--;native.runtime.managers_ready=0U;
  T(papacc_connection_manager_init(&cm,NULL,0U)==PAPACC_RESULT_OK);
  T(papacc_server_io_loop_win32_init_closed(&loop,&net,NULL,NULL,0U,NULL,0U,NULL,0U,NULL,0U,1U,&cm)==PAPACC_RESULT_OK);
  if(scenario==0U)T(ps_handler(SERVICE_CONTROL_STOP,0,NULL,NULL)==NO_ERROR);
  if(scenario==1U)model_stop_on_running=1U;
  if(scenario<2U){T(!papacc_server_service_qualification_managers_ready(&loop));if(scenario==0U)T(model_reports==0U);else T(model_reports==2U&&model_states[0]==SERVICE_RUNNING&&model_states[1]==SERVICE_STOP_PENDING);}
  else{T(papacc_server_service_qualification_managers_ready(&loop));T(model_reports==1U&&model_states[0]==SERVICE_RUNNING);T(ps_handler(SERVICE_CONTROL_STOP,0,NULL,NULL)==NO_ERROR);}
  i=model_closes;T(psr_close()!=ERROR_SUCCESS&&model_closes==i);T(!host.terminal_receipt);T(papacc_server_service_runtime_stopping());
  papacc_server_io_loop_win32_shutdown(&loop);T(!loop.initialized);T(papacc_connection_manager_try_shutdown(&cm,&done)==PAPACC_RESULT_OK&&done);
  papacc_server_service_runtime_closed();T(host.terminal_receipt==1U&&host.life.phase==PS_STOPPED);
  T(native.runtime.parent!=NULL);T(psr_close()==ERROR_SUCCESS);
 }
 model_stop_flag=0U;
 }

 ZeroMemory(&host,sizeof(host));ZeroMemory(&native.runtime,sizeof(native.runtime));model_mode=0;T(psr_token()==ERROR_SUCCESS);model_mode=9;
 T(psr_close()!=ERROR_SUCCESS&&native.runtime.retained);i=model_closes;T(psr_close()!=ERROR_SUCCESS&&model_closes==i);
 /* End isolated model lifetime; do not reset a real retained owner. */
 T(LocalFree(approved)==NULL);
 printf("CONFIG_TOKEN_MODEL=PASS; DIVERGENCE_REPLACEMENT_ACL_STOP_RETENTION_NOT_AUTHORITY; PS_RUNTIME=%lu PS_NATIVE=%lu\n",(unsigned long)sizeof(PS_RUNTIME),(unsigned long)sizeof(PS_NATIVE));
 }
#else
 {
 DWORD e=psr_token();T(e!=ERROR_SUCCESS);T(psr_close()==ERROR_SUCCESS);
 puts("NATIVE_EFFECTIVE_TOKEN=INELIGIBLE_DENIED_NO_SUBSTITUTION; READ_ONLY_HANDLES_CLOSED; SERVICE_NOT_STARTED");
 }
#endif
 return 0;
}
