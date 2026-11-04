/* Native API MODEL only: no Create/Change/Delete reaches Windows SCM. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "server_console_win32.h"
#include "server_service_win32.h"
#include "../apps/server/server_service_identity_win32.h"
#ifndef PAPACC_SCM_ORIGINAL_PRODUCER_MODEL
/* These are negative source stubs only. The separate producer model compiles
 * the actual canonical presenter/source instead, with native APIs modeled. */
static int no_registration_present(const PAPACC_SERVICE_REGISTRATION_VIEW *v,FILE *out,FILE *err){(void)v;(void)out;(void)err;return 2;}
static DWORD no_registration_current(const PAPACC_SERVICE_REGISTRATION_VIEW *v){(void)v;return ERROR_ACCESS_DENIED;}
static DWORD no_registration_take(const PAPACC_SERVICE_REGISTRATION_VIEW *v){(void)v;return ERROR_ACCESS_DENIED;}
static DWORD no_registration_effect(const PAPACC_SERVICE_REGISTRATION_VIEW *v,unsigned effect,unsigned returned,DWORD result)
{(void)v;(void)effect;(void)returned;(void)result;return ERROR_ACCESS_DENIED;}
static DWORD no_registration_finish(const PAPACC_SERVICE_REGISTRATION_VIEW *v,int closed){(void)v;(void)closed;return ERROR_ACCESS_DENIED;}
static DWORD no_registration_resource_report(const PAPACC_SERVICE_REGISTRATION_VIEW *v,PAPACC_SCM_RESOURCE_REPORT *out){(void)v;(void)out;return ERROR_ACCESS_DENIED;}
#define papacc_server_launcher_scm_registration_present no_registration_present
#define papacc_server_launcher_scm_registration_current no_registration_current
#define papacc_server_launcher_scm_registration_take no_registration_take
#define papacc_server_launcher_scm_registration_effect no_registration_effect
#define papacc_server_launcher_scm_registration_finish no_registration_finish
#define papacc_server_launcher_scm_registration_resource_report no_registration_resource_report
#endif
static unsigned mode,creates,changes,queries,lookups,closes,violations;
static unsigned forbidden_starts,forbidden_deletes;
static unsigned forbidden_managers,model_closed;
static int model_stopped;static unsigned reads,file_calls;
static BY_HANDLE_FILE_INFORMATION file_id;
static const wchar_t *model_command(void)
{return model_closed?L"\"C:\\Lab Space\\papacc_server.exe\" service host scq-closed":L"\"C:\\Lab Space\\papacc_server.exe\" service host";}
static PAPACC_BOOL model_stop(void){return model_stopped?PAPACC_TRUE:PAPACC_FALSE;}
static BOOL WINAPI model_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION f)
{(void)h;file_calls++;*f=file_id;if(mode==13||(mode==20&&file_calls>1))f->nFileIndexLow++;return TRUE;}
static DWORD WINAPI model_module(HMODULE m,LPWSTR out,DWORD cap)
{
 const wchar_t *v=mode==23?L"relative.exe":mode==24?L"C:\\Lab\\bad\"name.exe":mode==36?L"D:\\Controller\\new-controller.exe":L"C:\\Lab Space\\papacc_server.exe";
 size_t n=wcslen(v);(void)m;if(n+1>cap)return cap;memcpy(out,v,(n+1)*sizeof(wchar_t));return (DWORD)n;
}
static DWORD model_registration_target(WCHAR *out,size_t cap)
{
 const WCHAR *v=L"C:\\Lab Space\\papacc_server.exe";size_t n=wcslen(v);
 if(mode==23U)return ERROR_PATH_NOT_FOUND;
 if(n+1U>cap)return ERROR_INSUFFICIENT_BUFFER;memcpy(out,v,(n+1U)*sizeof(WCHAR));return 0;
}
static HANDLE WINAPI model_open_file(LPCWSTR name,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES sa,DWORD disposition,DWORD flags,HANDLE template_file)
{
 (void)name;if(access!=GENERIC_READ||share!=FILE_SHARE_READ||sa||disposition!=OPEN_EXISTING||flags!=FILE_FLAG_OPEN_REPARSE_POINT||template_file)violations++;
 if(mode==21){SetLastError(ERROR_ACCESS_DENIED);return INVALID_HANDLE_VALUE;}return (HANDLE)(ULONG_PTR)3;
}
static BOOL WINAPI model_read_file(HANDLE file,LPVOID out,DWORD cap,LPDWORD n,LPOVERLAPPED overlapped)
{
 (void)file;if(overlapped||cap<3)violations++;
 if(mode==22){SetLastError(ERROR_READ_FAULT);return FALSE;}
 *n=reads++?0:3;if(*n)memcpy(out,"abc",3);if(mode==25)model_stopped=1;return TRUE;
}
static SC_HANDLE WINAPI model_create(SC_HANDLE m,LPCWSTR name,LPCWSTR display,DWORD access,DWORD type,DWORD start,DWORD error,
 LPCWSTR command,LPCWSTR group,LPDWORD tag,LPCWSTR dep,LPCWSTR account,LPCWSTR password)
{
 creates++;
 if(m!=(SC_HANDLE)(ULONG_PTR)1||wcscmp(name,PAPACC_SCM_NAME_LITERAL)||wcscmp(display,name)||
 access!=(SERVICE_QUERY_CONFIG|SERVICE_CHANGE_CONFIG|SERVICE_QUERY_STATUS)||type!=SERVICE_WIN32_OWN_PROCESS||start!=PAPACC_SCM_START_MODE||
 error!=SERVICE_ERROR_NORMAL||wcscmp(command,model_command())||group||tag||dep||
 !account||wcscmp(account,PAPACC_SCM_ACCOUNT_LITERAL)||password)violations++;
 if(mode==1){SetLastError(ERROR_SERVICE_EXISTS);return NULL;}
 if(mode==2){SetLastError(ERROR_ACCESS_DENIED);return NULL;}
 if(mode==15)model_stopped=1;
 return (SC_HANDLE)(ULONG_PTR)2;
}
static BOOL WINAPI model_change(SC_HANDLE h,DWORD kind,LPVOID data)
{
 changes++;if(h!=(SC_HANDLE)(ULONG_PTR)2||kind!=SERVICE_CONFIG_SERVICE_SID_INFO||
 ((SERVICE_SID_INFO *)data)->dwServiceSidType!=SERVICE_SID_TYPE_UNRESTRICTED)violations++;
 if(mode==3){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;
}
static wchar_t *put_string(BYTE *b,DWORD *used,const wchar_t *s)
{
 wchar_t *p=(wchar_t *)(b+*used);size_t n=(wcslen(s)+1)*sizeof(wchar_t);memcpy(p,s,n);*used+=(DWORD)n;return p;
}
static BOOL WINAPI model_query(SC_HANDLE h,LPQUERY_SERVICE_CONFIGW q,DWORD cap,LPDWORD used)
{
 BYTE *b=(BYTE *)q;(void)h;queries++;
 if(mode==7){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(cap<8192){violations++;return FALSE;}
 ZeroMemory(b,cap);*used=sizeof(*q);q->dwServiceType=SERVICE_WIN32_OWN_PROCESS;q->dwStartType=PAPACC_SCM_START_MODE;
 q->dwErrorControl=mode==43U?SERVICE_ERROR_CRITICAL:SERVICE_ERROR_NORMAL;
 q->lpServiceStartName=put_string(b,used,mode==4?L"LocalSystem":PAPACC_SCM_ACCOUNT_LITERAL);
 q->lpBinaryPathName=put_string(b,used,mode==5?L"C:\\different.exe":mode==35?L"\"C:\\Lab Space\\papacc_server.exe\" service host":model_command());
 q->lpDependencies=put_string(b,used,L"");
 if(mode==8)q->lpServiceStartName=(wchar_t *)(ULONG_PTR)1;
 if(mode==9)*used=cap+2;
 if(mode==17)q->lpDependencies=NULL;
 return TRUE;
}
static BOOL WINAPI model_query2(SC_HANDLE h,DWORD kind,LPBYTE p,DWORD cap,LPDWORD used)
{
 SERVICE_SID_INFO *i=(SERVICE_SID_INFO *)p;(void)h;
 if(kind!=SERVICE_CONFIG_SERVICE_SID_INFO||cap!=sizeof(*i))violations++;
 if(mode==10){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 *used=mode==18?0:sizeof(*i);i->dwServiceSidType=mode==6?SERVICE_SID_TYPE_NONE:SERVICE_SID_TYPE_UNRESTRICTED;return TRUE;
}
static BOOL WINAPI model_lookup(LPCWSTR host,LPCWSTR name,PSID sid,LPDWORD n,LPWSTR domain,LPDWORD dn,PSID_NAME_USE use)
{
 SID_IDENTIFIER_AUTHORITY nt=SECURITY_NT_AUTHORITY;unsigned j;lookups++;
 if(host||wcscmp(name,PAPACC_SCM_ACCOUNT_LITERAL)||*n<32||*dn<11)violations++;
 if(mode==12){SetLastError(ERROR_NONE_MAPPED);return FALSE;}
 if(mode==30){*n=32;SetLastError(ERROR_INSUFFICIENT_BUFFER);return FALSE;}
 if(mode==31&&!creates){SetLastError(ERROR_TRUSTED_RELATIONSHIP_FAILURE);return FALSE;}
 if(!InitializeSid(sid,&nt,6))return FALSE;
 for(j=0;j<6;j++)*GetSidSubAuthority(sid,j)=j?j:80;
 if(mode==11)*GetSidSubAuthority(sid,0)=21;
 /* Real LookupAccountName may preserve capacity on success. */
 if(mode!=27&&mode!=33)*n=GetLengthSid(sid);
 if(mode==28)*n=31; /* valid SID, inconsistent/truncated reported capacity */
 if(mode==29)((BYTE *)sid)[0]=2; /* invalid revision */
 if(mode==32)*n=SECURITY_MAX_SID_SIZE+1;
 if(mode==33)((BYTE *)sid)[1]=15; /* wrong service-SID shape */
 if(mode==34)*n=0;
 memcpy(domain,L"NT SERVICE",sizeof(L"NT SERVICE"));*dn=10;*use=SidTypeWellKnownGroup;return TRUE;
}
static BOOL WINAPI model_close_service(SC_HANDLE h)
{(void)h;closes++;if(mode==16){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static BOOL WINAPI model_close_file(HANDLE h){(void)h;closes++;if(mode==26){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static BOOL WINAPI model_stopped_status(SC_HANDLE h,SC_STATUS_TYPE type,LPBYTE out,DWORD cap,LPDWORD used)
{
 SERVICE_STATUS_PROCESS *s=(SERVICE_STATUS_PROCESS *)out;(void)h;
 if(type!=SC_STATUS_PROCESS_INFO||cap<sizeof(*s)){violations++;return FALSE;}
 if(mode==38U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 ZeroMemory(s,sizeof(*s));s->dwCurrentState=mode==37U?SERVICE_RUNNING:SERVICE_STOPPED;
 *used=mode==39U?(DWORD)sizeof(*s)-1U:(DWORD)sizeof(*s);return TRUE;
}
static SC_HANDLE WINAPI model_manager(LPCWSTR machine,LPCWSTR database,DWORD rights)
{(void)machine;(void)database;(void)rights;forbidden_managers++;if(mode==42U)return (SC_HANDLE)(ULONG_PTR)1;SetLastError(ERROR_ACCESS_DENIED);return NULL;}
static SC_HANDLE WINAPI model_existing_service(SC_HANDLE m,LPCWSTR name,DWORD rights)
{
 if(m!=(SC_HANDLE)(ULONG_PTR)1||wcscmp(name,PAPACC_SCM_NAME_LITERAL)||rights!=SERVICE_QUERY_STATUS)violations++;
 if(mode==40U)return (SC_HANDLE)(ULONG_PTR)2;
 SetLastError(mode==41U?ERROR_ACCESS_DENIED:ERROR_SERVICE_DOES_NOT_EXIST);return NULL;
}
static PAPACC_RESULT model_console_install(PAPACC_SERVER_CONSOLE_WIN32 *c)
{c->is_installed=PAPACC_TRUE;return PAPACC_RESULT_OK;}
static PAPACC_RESULT model_console_uninstall(PAPACC_SERVER_CONSOLE_WIN32 *c)
{c->is_installed=PAPACC_FALSE;return PAPACC_RESULT_OK;}
#define OpenSCManagerW model_manager
#define OpenServiceW model_existing_service
#define QueryServiceStatusEx model_stopped_status
#define papacc_server_console_win32_install model_console_install
#define papacc_server_console_win32_uninstall model_console_uninstall
#define papacc_server_console_win32_stop_requested model_stop
#define GetModuleFileNameW model_module
#define papacc_server_launcher_scm_registration_target model_registration_target
#define CreateFileW model_open_file
#define ReadFile model_read_file
#define GetFileInformationByHandle model_file
static BOOL WINAPI model_forbidden_start(SC_HANDLE h,DWORD n,LPCWSTR *a){(void)h;(void)n;(void)a;forbidden_starts++;SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
static BOOL WINAPI model_forbidden_delete(SC_HANDLE h){(void)h;forbidden_deletes++;SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
#define StartServiceW model_forbidden_start
#define DeleteService model_forbidden_delete
#define CreateServiceW model_create
#define ChangeServiceConfig2W model_change
#define QueryServiceConfigW model_query
#define QueryServiceConfig2W model_query2
#define LookupAccountNameW model_lookup
#define CloseServiceHandle model_close_service
#define CloseHandle model_close_file
#include "server_service_win32.h"
/* Compile-time negative boundary, never an installable product callback. */
static DWORD model_no_start_carrier(PS_SCQ_CARRIER *out){(void)out;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_take(const PAPACC_SERVICE_IMAGE_VIEW *view){(void)view;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_progress(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_wait(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_terminal(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_finish(HANDLE process){(void)process;return ERROR_ACCESS_DENIED;}
static DWORD model_no_start_namespace(void){return ERROR_ACCESS_DENIED;}
static unsigned model_first_calls,model_preclose_calls;
static DWORD model_no_first_controller(void){model_first_calls++;return ERROR_ACCESS_DENIED;}
static DWORD model_prestart_close(void){model_preclose_calls++;return 0;}
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
#define T(x) do{if(!(x)){printf("FAIL line=%d mode=%u\n",__LINE__,mode);return 1;}}while(0)
static void reset_model(unsigned m)
{
 /* Different MODEL process. Never used to reset a real retained owner. */
 mode=m;reads=file_calls=0;creates=changes=queries=lookups=closes=violations=model_closed=0;model_stopped=m==14;
 ZeroMemory(&native,sizeof(native));ZeroMemory(&file_id,sizeof(file_id));file_id.nFileIndexLow=123;
 native.manager=(SC_HANDLE)(ULONG_PTR)1;native.image=(HANDLE)(ULONG_PTR)3;native.image_id=file_id;native.image_ready=1;
 memcpy(native.image_path,L"C:\\Lab Space\\papacc_server.exe",sizeof(L"C:\\Lab Space\\papacc_server.exe"));
 (void)ps_install_command(1);
}
int main(void)
{
 /* New controller image is not implicitly the stopped-service target. */
 DWORD e;unsigned m,n;const char *v[]={"p","service","install","--scq-closed"};FILE *o=tmpfile(),*err=tmpfile();
 PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT registration;
 if(!o||!err)return 1;
#ifndef PAPACC_SCM_REGISTRATION_LAB
 reset_model(36);native.image=NULL;native.image_ready=0;native.install_profile=PS_INSTALL_SCQ_CLOSED;model_closed=1;
 T(ps_image_open(NULL)==0&&!wcscmp(native.image_path,L"C:\\Lab Space\\papacc_server.exe"));
 T(ps_install_command(0)&&!wcsstr(native.command,L"new-controller.exe"));T(ps_close()==0);
#endif
 reset_model(0);T(ps_install_virtual()==0);T(creates==1&&changes==1&&queries==1&&lookups==1&&!violations);
 T(native.readback&&native.install_disposition==4&&native.service&&native.image);
 T(ps_install_virtual()==ERROR_INVALID_STATE&&creates==1);T(ps_close()==0&&closes==3&&!native.service&&!native.manager&&!native.image);
 T(native.install_effect&&native.install_disposition==4);InterlockedExchange(&cli_active,0);
 T(papacc_server_service_cli(3,v,o,err,NULL)==2&&creates==1); /* consumed, no retry even after factual closes */
 for(m=1;m<=18;m++){
  reset_model(m);e=ps_install_virtual();
  if(m==17){T(e==0&&native.readback&&native.install_disposition==4);T(ps_close()==0);continue;}
  if(m==16){T(e==0);T(ps_close()!=0&&native.retained&&native.service&&native.manager&&native.image);n=closes;
   InterlockedExchange(&cli_active,0);T(papacc_server_service_cli(3,v,o,err,NULL)==2&&closes==n);continue;}
  T(e!=0&&!violations);
  if(m==13||m==14){T(creates==0&&!native.install_effect);}
  else {T(creates==1&&native.install_effect);n=creates;T(ps_install_virtual()==ERROR_INVALID_STATE&&creates==n);}
  if(m==1)T(e==ERROR_SERVICE_EXISTS&&native.install_disposition==1&&!native.service);
  if(m==2)T(e==ERROR_ACCESS_DENIED&&native.install_disposition==2);
  if(m==3)T(changes==1&&queries==0&&native.install_disposition==3);
  if(m==15)T(changes==0&&native.install_disposition==3);
 }
 /* Same original image operations, diagnostic only: no source or query added. */
 {DWORD stage;
  reset_model(21);native.image=NULL;native.image_ready=0;stage=0;
  T(ps_image_open(&stage)==ERROR_ACCESS_DENIED&&stage==103U&&!native.image);T(ps_close()==0);
  reset_model(22);native.image=NULL;native.image_ready=0;stage=0;
  T(ps_image_open(&stage)==ERROR_READ_FAULT&&stage==107U&&native.image);T(ps_close()==0);
  reset_model(23);native.image=NULL;native.image_ready=0;native.start_selected=1U;stage=0;
  T(ps_image_open(&stage)==ERROR_PATH_NOT_FOUND&&stage==101U&&!native.image);T(ps_close()==0);
  reset_model(20);native.image=NULL;native.image_ready=0;native.start_selected=1U;stage=0;
  T(ps_image_open(&stage)==ERROR_INVALID_DATA&&stage==111U&&!native.image_ready);T(ps_close()==0);
  reset_model(19);stage=0;native.start_effect=1U;
  T(ps_close()==ERROR_INVALID_STATE&&native.retained&&native.image); /* no false close0 after submission */
 }
 for(m=19;m<=26;m++){
  static const BYTE abc[32]={0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
  reset_model(m);native.manager=NULL;native.image=NULL;native.image_ready=0;
  e=ps_image_open(NULL);T(!violations);
  if(m==19||m==26){T(e==0&&native.image_ready&&memcmp(native.image_sha256,abc,32)==0);T(!wcscmp(native.command,L"\"C:\\Lab Space\\papacc_server.exe\" service host"));}
  else T(e!=0&&!native.image_ready&&!native.install_effect);
  if(m==26){T(ps_close()!=0&&native.image&&native.retained);n=closes;InterlockedExchange(&cli_active,0);T(papacc_server_service_cli(3,v,o,err,NULL)==2&&closes==n);}
  else T(ps_close()==0&&!native.image);
 }
 for(m=27;m<=34;m++){
  reset_model(m);
  if(m==31){DWORD sn=sizeof(native.service_sid),dn=sizeof(native.sid_domain)/sizeof(wchar_t);SID_NAME_USE use;
   T(!model_lookup(NULL,PAPACC_SCM_ACCOUNT_LITERAL,native.service_sid,&sn,native.sid_domain,&dn,&use));
   T(GetLastError()==ERROR_TRUSTED_RELATIONSHIP_FAILURE&&creates==0);}
  e=ps_install_virtual();T(!violations);
  if(m==27||m==31){T(e==0&&native.readback&&native.sid_size==32&&native.install_disposition==4);}
  else {T(e!=0&&!native.readback);if(m==30)T(e==ERROR_INSUFFICIENT_BUFFER);else T(e==ERROR_INVALID_SID);}
  T(creates==1);n=creates;T(ps_install_virtual()==ERROR_INVALID_STATE&&creates==n);
  T(ps_close()==0); /* confirmed modeled cleanup, no actual SCM */
 }
 /* Explicit closed selection affects exactly the registered/read-back command.
  * No original authority is synthesized by the primitive model. */
 reset_model(0);native.install_profile=PS_INSTALL_SCQ_CLOSED;
#ifdef PAPACC_SCM_REGISTRATION_LAB
 T(!ps_install_command(1));T(ps_install_virtual()!=0&&!creates);
 T(papacc_server_service_registration_source_status(&registration)==ERROR_ACCESS_DENIED&&registration.reason==PS_REGISTRATION_WRONG_LAB_SCOPE);
#else
 model_closed=1U;T(ps_install_command(1));T(ps_install_virtual()==ERROR_ACCESS_DENIED&&!creates&&!native.install_effect);
 native.service=(SC_HANDLE)(ULONG_PTR)2;T(ps_virtual_readback()==0&&!violations);T(native.readback);T(ps_close()==0);
 reset_model(35);native.install_profile=PS_INSTALL_SCQ_CLOSED;model_closed=1U;T(ps_install_command(1));
 native.service=(SC_HANDLE)(ULONG_PTR)2;T(ps_virtual_readback()==ERROR_INVALID_DATA&&!creates&&!native.readback);
 T(ps_close()==0); /* mismatched productive readback never becomes confirmation */
#endif
 reset_model(0);native.install_profile=PS_INSTALL_SCQ_CLOSED;
 T(ps_install_virtual()==ERROR_INVALID_DATA&&!creates&&!native.install_effect); /* stale product command */
 reset_model(0);native.command[4]^=1;T(ps_install_virtual()==ERROR_INVALID_DATA&&!creates);
 reset_model(0);native.install_profile=99U;T(!ps_install_command(1));T(ps_install_virtual()==ERROR_INVALID_DATA&&!creates);
 reset_model(0);native.image_path[0]=0;T(!ps_install_command(1));
 reset_model(0);memset(native.image_path,'x',sizeof(native.image_path));T(!ps_install_command(1));
 reset_model(0);T(papacc_server_service_registration_source_status(NULL)==ERROR_INVALID_PARAMETER);
 T(papacc_server_service_registration_source_status(&registration)==ERROR_ACCESS_DENIED&&registration.reason==PS_REGISTRATION_ORIGINAL_UNAVAILABLE&&registration.image_metadata==1U&&registration.missing_source_fields==31U);
 T(!ps_registration_binding_ready());native.thread=GetCurrentThreadId()+1U;
 T(papacc_server_service_registration_source_status(&registration)==ERROR_ACCESS_DENIED&&registration.reason==PS_REGISTRATION_WRONG_THREAD);
 native.thread=0;model_stopped=1;T(papacc_server_service_registration_source_status(&registration)==ERROR_ACCESS_DENIED&&registration.reason==PS_REGISTRATION_STOPPED);
 model_stopped=0;native.install_effect=1;T(papacc_server_service_registration_source_status(&registration)==ERROR_ACCESS_DENIED&&registration.reason==PS_REGISTRATION_CONSUMED);
 native.install_effect=0;native.retained=1;T(papacc_server_service_registration_source_status(&registration)==ERROR_ACCESS_DENIED&&registration.reason==PS_REGISTRATION_RETAINED_UNKNOWN);
 /* Full CLI: never reaches SCM, regardless of selector or a forged RUNNING
  * metadata observation. Simulated image/console APIs, no real endpoint/token. */
 for(m=3;m<=4;m++){
  reset_model(0);native.manager=NULL;native.image=NULL;native.image_ready=0;
  host.run_active=1U;host.life.phase=PS_RUNNING;InterlockedExchange(&cli_active,0);
  T(papacc_server_service_cli((int)m,v,o,err,NULL)==2&&!creates&&!changes&&!queries&&!forbidden_managers);
 }
#ifndef PAPACC_SCM_REGISTRATION_LAB
 {const char *closed_start[]={"p","service","start","--scq-closed"};
  reset_model(0);native.manager=NULL;native.image=NULL;native.image_ready=0;
  memset(&host,0,sizeof(host));InterlockedExchange(&cli_active,0);model_first_calls=model_preclose_calls=0;
  T(papacc_server_service_cli(4,closed_start,o,err,NULL)==2);
  T(model_first_calls==1U&&model_preclose_calls==1U&&!native.retained);
  T(!creates&&!changes&&!queries&&!forbidden_managers&&!forbidden_starts);
 }
#endif
 T(forbidden_starts==0&&forbidden_deletes==0&&forbidden_managers==0);
 T(fclose(o)==0&&fclose(err)==0);
 printf("Virtual service MODEL contract, no password, readback mismatch, partial effects, Stop and no retry PASS; owner=%lu host=%lu total=%lu\n",
 (unsigned long)sizeof(native),(unsigned long)sizeof(host),(unsigned long)(sizeof(native)+sizeof(host)+sizeof(cli_active)));
 return 0;
}
