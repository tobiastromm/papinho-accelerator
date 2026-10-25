#include <aclapi.h>
#include <sddl.h>
static int personal_group(WELL_KNOWN_SID_TYPE kind,int expected)
{
 BYTE sid[SECURITY_MAX_SID_SIZE];DWORD bytes=sizeof(sid);BOOL member=FALSE;
 return CreateWellKnownSid(kind,NULL,sid,&bytes)&&CheckTokenMembership(NULL,sid,&member)&&(member?1:0)==expected;
}
static int personal_token(PAPACC_CONFIG_PENDING *p,int capture)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 union {TOKEN_USER alignment;BYTE bytes[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];} u;
 TOKEN_STATISTICS current_token;int same;
 TOKEN_STATISTICS t;TOKEN_ELEVATION e;TOKEN_ELEVATION_TYPE type;DWORD z,session,ps;TOKEN_USER *user=(TOKEN_USER *)&u;
 if(n->phase!=1U||p->stop||n->unexpected_thread)return 0;
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&n->unexpected_thread))return 0;
 if(GetLastError()!=ERROR_NO_TOKEN||!personal_group(WinBuiltinAdministratorsSid,1)||!personal_group(WinInteractiveSid,1)||!personal_group(WinServiceSid,0))return 0;
 if(capture&&!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&p->bootstrap_token))return 0;
 if(!p->bootstrap_token||!GetTokenInformation(p->bootstrap_token,TokenStatistics,&t,sizeof(t),&z)||t.TokenType!=TokenPrimary||
 !GetTokenInformation(p->bootstrap_token,TokenElevation,&e,sizeof(e),&z)||!e.TokenIsElevated||
 !GetTokenInformation(p->bootstrap_token,TokenElevationType,&type,sizeof(type),&z)||type==TokenElevationTypeLimited||
 !GetTokenInformation(p->bootstrap_token,TokenSessionId,&session,sizeof(session),&z)||!session||
 !ProcessIdToSessionId(GetCurrentProcessId(),&ps)||ps!=session||session!=WTSGetActiveConsoleSessionId()||
 !GetTokenInformation(p->bootstrap_token,TokenUser,&u,sizeof(u),&z)||!IsValidSid(user->User.Sid)||GetLengthSid(user->User.Sid)>sizeof(n->sid))return 0;
 if(capture){n->token=t;n->session=session;n->sid_bytes=GetLengthSid(user->User.Sid);return CopySid(sizeof(n->sid),n->sid,user->User.Sid)!=0;}
 if(n->check_token||!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&n->check_token))return 0;
 same=GetTokenInformation(n->check_token,TokenStatistics,&current_token,sizeof(current_token),&z)&&
 !memcmp(&current_token.TokenId,&t.TokenId,sizeof(LUID))&&!memcmp(&current_token.ModifiedId,&t.ModifiedId,sizeof(LUID));
 if(!CloseHandle(n->check_token)){n->phase=2U;return 0;}n->check_token=NULL;
 return same&&n->session==session&&n->sid_bytes==GetLengthSid(user->User.Sid)&&EqualSid(n->sid,user->User.Sid)&&
 !memcmp(&n->token.TokenId,&t.TokenId,sizeof(LUID))&&!memcmp(&n->token.AuthenticationId,&t.AuthenticationId,sizeof(LUID))&&
 !memcmp(&n->token.ModifiedId,&t.ModifiedId,sizeof(LUID));
}
PAPACC_RESULT papacc_configuration_personal_native_open(PAPACC_CONFIGURATION_COMMIT *o,const char *parent)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;unsigned i,k,len;char drive[3],device[512];
 WCHAR path[MAX_PATH+5];UNICODE_STRING name;OBJECT_ATTRIBUTES a;IO_STATUS_BLOCK io;HMODULE mod;FARPROC proc;BOOTSTRAP_NT_CREATE create;
 PACL dacl;BOOL present,defaulted;
 if(!o||o->writer!=GetCurrentThreadId()||o->initialized||!parent||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(n->phase||p->active||p->thread||p->bootstrap_parent||p->bootstrap_token)return PAPACC_RESULT_INVALID_STATE;
 len=(unsigned)strlen(parent);if(len<3U||len>=MAX_PATH||parent[1]!=':'||parent[2]!='\\'||strstr(parent,".."))return PAPACC_RESULT_INVALID_ARGUMENT;
 for(i=0;i<len;i++)if((unsigned char)parent[i]<32U||(unsigned char)parent[i]>126U||parent[i]=='/'||(i!=1U&&parent[i]==':'))return PAPACC_RESULT_INVALID_ARGUMENT;
 n->phase=1U;p->file=INVALID_HANDLE_VALUE;memcpy(p->path,parent,len+1U);
 if(!personal_token(p,1))return PAPACC_RESULT_INVALID_STATE;
 drive[0]=parent[0];drive[1]=':';drive[2]=0;
 if(!QueryDosDeviceA(drive,device,sizeof(device))||strncmp(device,"\\Device\\",8U))return PAPACC_RESULT_NOT_SUPPORTED;
 k=(unsigned)strlen(device);if(k+len-2U>=MAX_PATH+5U)return PAPACC_RESULT_INVALID_ARGUMENT;
 for(i=0;i<k;i++)path[i]=(WCHAR)(unsigned char)device[i];
 for(i=2U;i<=len;i++)path[k+i-2U]=(WCHAR)(unsigned char)parent[i];len=k+len-2U;
 mod=GetModuleHandleA("ntdll.dll");if(!mod)return PAPACC_RESULT_NOT_SUPPORTED;proc=GetProcAddress(mod,"NtCreateFile");if(!proc)return PAPACC_RESULT_NOT_SUPPORTED;memcpy(&create,&proc,sizeof(create));
 memset(&a,0,sizeof(a));memset(&io,0,sizeof(io));name.Buffer=path;name.Length=(USHORT)(len*2U);name.MaximumLength=name.Length;
 a.Length=sizeof(a);a.ObjectName=&name;a.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 if(create(&p->bootstrap_parent,FILE_READ_ATTRIBUTES|FILE_TRAVERSE|FILE_ADD_SUBDIRECTORY|READ_CONTROL|SYNCHRONIZE,&a,&io,NULL,
 FILE_ATTRIBUTE_DIRECTORY,FILE_SHARE_READ,1U,0x00200021U,NULL,0U)<0)return PAPACC_RESULT_INVALID_STATE;
 if(!GetFileInformationByHandle(p->bootstrap_parent,&n->parent)||!(n->parent.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||
 (n->parent.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return PAPACC_RESULT_INVALID_STATE;
 if(GetSecurityInfo(p->bootstrap_parent,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->descriptor)!=ERROR_SUCCESS||
 !IsValidSecurityDescriptor(n->descriptor)||!GetSecurityDescriptorDacl(n->descriptor,&present,&dacl,&defaulted)||!present||!dacl)return PAPACC_RESULT_INVALID_STATE;
 n->descriptor_bytes=GetSecurityDescriptorLength(n->descriptor);
 if(!ConvertSidToStringSidA(n->sid,&n->sid_text))return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_personal_native_current(o);
}
static PAPACC_RESULT personal_packet_current(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;BY_HANDLE_FILE_INFORMATION f;int same;
 if(InterlockedCompareExchange(&p->stop,0,0))return PAPACC_RESULT_INVALID_STATE;
 n=&p->personal_native;
 if(!personal_token(p,0)||!p->bootstrap_parent||!n->descriptor||n->check_descriptor||
 !GetFileInformationByHandle(p->bootstrap_parent,&f)||memcmp(&f.dwVolumeSerialNumber,&n->parent.dwVolumeSerialNumber,sizeof(DWORD))||
 f.nFileIndexHigh!=n->parent.nFileIndexHigh||f.nFileIndexLow!=n->parent.nFileIndexLow||
 !(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return PAPACC_RESULT_INVALID_STATE;
 if(GetSecurityInfo(p->bootstrap_parent,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->check_descriptor)!=ERROR_SUCCESS)return PAPACC_RESULT_INVALID_STATE;
 same=IsValidSecurityDescriptor(n->check_descriptor)&&n->descriptor_bytes==GetSecurityDescriptorLength(n->check_descriptor)&&
 !memcmp(n->descriptor,n->check_descriptor,n->descriptor_bytes);
 if(LocalFree(n->check_descriptor)){n->phase=2U;return PAPACC_RESULT_INTERNAL_ERROR;}n->check_descriptor=NULL;
 return same?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_personal_native_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_RESULT r;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 r=personal_packet_current(&o->pending_commit);
 if(o->pending_commit.personal_native.phase==2U)o->unresolved=1;
 return r;
}
PAPACC_RESULT papacc_configuration_personal_native_close(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;HANDLE *h[5];HLOCAL m[3];unsigned i;
 if(!o||o->writer!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;p=&o->pending_commit;n=&p->personal_native;
 if(p->personal_ca||p->active||p->thread||n->phase==2U)return PAPACC_RESULT_INVALID_STATE;
 if(n->phase==3U)return PAPACC_RESULT_OK;
 /* The child must close before its protecting parent, after worker signal.
  * This bit is disjoint from old handles and LocalFree attempt bits. */
 if(p->bootstrap_container){
  if(n->close_attempts&256U)goto retained;n->close_attempts|=256U;
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
  if(p->test_unknown==2){SetLastError(ERROR_INVALID_HANDLE);goto retained;}
#endif
  if(!CloseHandle(p->bootstrap_container))goto retained;p->bootstrap_container=NULL;
 }
 h[0]=&p->file;h[1]=&n->unexpected_thread;h[2]=&p->bootstrap_token;h[3]=&p->bootstrap_parent;h[4]=&n->check_token;
 for(i=0;i<5;i++)if(*h[i]&&*h[i]!=INVALID_HANDLE_VALUE){
 if(n->close_attempts&(1U<<i))goto retained;n->close_attempts|=1U<<i;
 if(!CloseHandle(*h[i]))goto retained;*h[i]=NULL;}
 m[0]=n->check_descriptor;m[1]=n->descriptor;m[2]=n->sid_text;
 for(i=0;i<3;i++)if(m[i]){if(n->close_attempts&(32U<<i))goto retained;n->close_attempts|=32U<<i;if(LocalFree(m[i]))goto retained;if(i==0U)n->check_descriptor=NULL;else if(i==1U)n->descriptor=NULL;else n->sid_text=NULL;}
 n->phase=3U;return PAPACC_RESULT_OK;
retained:n->error=GetLastError();n->phase=2U;o->unresolved=1;return PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT personal_packet_source(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;BY_HANDLE_FILE_INFORMATION f;LARGE_INTEGER zero;
 const unsigned char *configuration=p->personal_data.plan;size_t bytes=p->personal_data.bytes;
 if(InterlockedCompareExchange(&p->stop,0,0))return PAPACC_RESULT_INVALID_STATE;
 if(p->personal_data.effectful&&!papacc_personal_effect_source(p->personal_data.plan,p->personal_data.bytes,&configuration,&bytes))return PAPACC_RESULT_INVALID_STATE;
 n=&p->personal_native;zero.QuadPart=0;
 if(n->phase!=1U||!p->file||p->file==INVALID_HANDLE_VALUE||!GetFileInformationByHandle(p->file,&f)||
 f.dwVolumeSerialNumber!=n->source.dwVolumeSerialNumber||f.nFileIndexHigh!=n->source.nFileIndexHigh||f.nFileIndexLow!=n->source.nFileIndexLow||
 f.nFileSizeHigh||f.nFileSizeLow!=bytes||f.nFileSizeLow!=n->source.nFileSizeLow||
 !SetFilePointerEx(p->file,zero,NULL,FILE_BEGIN)||pending_compare_file(p,configuration,f.nFileSizeLow)!=1)return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_source_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 return personal_packet_source(&o->pending_commit);
}
/* Same pending_worker dispatch. All inputs belong to the frozen packet. Never
 * reads owner-only DATA phase/consumed while Stop may invalidate that phase.
 * Source bytes/attempt/native refs remain owned until factual quiescence. */
static void personal_handoff_worker(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 LONG result=PAPACC_PERSONAL_HANDOFF_DENIED;
 if(InterlockedCompareExchange(&n->handoff_state,0,0)!=PAPACC_PERSONAL_HANDOFF_SUBMITTED)return;
 if(!InterlockedCompareExchange(&p->stop,0,0)&&
    !memcmp(n->handoff_attempt,p->personal_data.attempt,32U)&&
    n->plan_leaf[0]&&memchr(n->plan_leaf,0,sizeof(n->plan_leaf))&&
    !memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf))&&
    personal_packet_current(p)==PAPACC_RESULT_OK&&
    personal_packet_source(p)==PAPACC_RESULT_OK&&
    !InterlockedCompareExchange(&p->stop,0,0))result=PAPACC_PERSONAL_HANDOFF_VERIFIED;
 if(n->phase==2U||p->close_error){result=PAPACC_PERSONAL_HANDOFF_RETAINED;p->result=PAPACC_CAS_UNKNOWN;}
 /* No FILE_CREATE, owner lookup, key borrow, commit or publication here.
  * Existing child creation remains separately denied by its native gate. */
 InterlockedExchange(&n->handoff_state,result);
}

/* Effect-capable sibling dispatch in the SAME worker. The current read-only
 * ceremony never authorizes it; the production gate is independently checked.
 * LAB93 tests mechanics only, never an eligible actor or administrative grant. */
static void personal_target_worker(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;int lab=0;
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
 lab=p->checkpoint_only==93;
#endif
 if(InterlockedCompareExchange(&n->handoff_state,0,0)!=PAPACC_PERSONAL_HANDOFF_SUBMITTED)return;
 p->result=PAPACC_COMMIT_FAILED;
 if(!lab&&!personal_native_ceremony_ready(p))goto denied;
 if(InterlockedCompareExchange(&p->stop,0,0)||
    memcmp(n->handoff_attempt,p->personal_data.attempt,32U)||
    !n->plan_leaf[0]||!memchr(n->plan_leaf,0,sizeof(n->plan_leaf))||
    memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf)))goto denied;
 if(!lab&&(personal_packet_current(p)!=PAPACC_RESULT_OK||personal_packet_source(p)!=PAPACC_RESULT_OK))goto denied;
 if(!bootstrap_create_child(p))goto denied;
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
 if(lab&&p->test_entered){
  if(!SetEvent(p->test_entered)||WaitForSingleObject(p->test_release,INFINITE)!=WAIT_OBJECT_0){p->result=PAPACC_CAS_UNKNOWN;goto denied;}
 }
 if(lab&&p->test_unknown==1){p->result=PAPACC_CAS_UNKNOWN;goto denied;}
#endif
 if(InterlockedCompareExchange(&p->stop,0,0))goto denied;
 if(!bootstrap_identity(p->bootstrap_parent,&p->bootstrap_parent_volume,&p->bootstrap_parent_high,&p->bootstrap_parent_low,0)||
    !bootstrap_identity(p->bootstrap_container,&p->bootstrap_volume,&p->bootstrap_high,&p->bootstrap_low,0)){
  p->result=PAPACC_CAS_UNKNOWN;goto denied;}
 /* Only the namespace fact. No CA reference/acquisition is fabricated. */
 p->result=PAPACC_CAS_SUCCESS;
 InterlockedExchange(&n->handoff_state,PAPACC_PERSONAL_HANDOFF_VERIFIED);return;
denied:
 InterlockedExchange(&n->handoff_state,p->result==PAPACC_CAS_UNKNOWN?PAPACC_PERSONAL_HANDOFF_RETAINED:PAPACC_PERSONAL_HANDOFF_DENIED);
}
