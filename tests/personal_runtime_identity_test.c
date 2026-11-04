#define _WIN32_WINNT 0x0600
#include "personal_runtime_identity_win32.h"
#include "server_ca_key_win32.h"
#include <sddl.h>
#include <stdio.h>
#include <string.h>
static PSID virtual_sid,sam_sid,service_sid;static int observation_service;static unsigned fault,closes,opens,checks;static DWORD thread=7;
static int use_native_thread;
static DWORD shape_elevation;static PSID shape_admin_sid;
static unsigned observation_open_target,observation_fault;static volatile LONG *observation_stop;
static BOOL native_thread_close(HANDLE h){return CloseHandle(h);}
static DWORD native_wait(HANDLE h,DWORD ms){return WaitForSingleObject(h,ms);}
static DWORD WINAPI model_wait(HANDLE h,DWORD ms){if(h!=(HANDLE)11)return native_wait(h,ms);if(fault==29){SetLastError(ERROR_ACCESS_DENIED);return WAIT_FAILED;}return fault==28?WAIT_OBJECT_0:WAIT_TIMEOUT;}
static DWORD WINAPI model_thread(void){return use_native_thread?GetCurrentThreadId():thread;}
static BOOL WINAPI model_duplicate(HANDLE a,HANDLE b,HANDLE c,LPHANDLE out,DWORD rights,BOOL inherit,DWORD options)
{(void)a;(void)b;(void)c;if(rights!=(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE)||inherit||options)return FALSE;*out=(HANDLE)11;return TRUE;}
static DWORD WINAPI model_pid(HANDLE p){(void)p;return fault==10?88:77;}
static BOOL WINAPI model_times(HANDLE p,LPFILETIME c,LPFILETIME e,LPFILETIME k,LPFILETIME u)
{(void)p;memset(c,0,sizeof(*c));c->dwLowDateTime=fault==11?100:99;memset(e,0,sizeof(*e));memset(k,0,sizeof(*k));memset(u,0,sizeof(*u));return TRUE;}
static BOOL WINAPI model_exit(HANDLE p,LPDWORD code){(void)p;*code=fault==12?0:STILL_ACTIVE;return TRUE;}
static BOOL WINAPI model_open(HANDLE p,DWORD rights,PHANDLE out)
{(void)p;(void)rights;opens++;if(observation_open_target&&opens==observation_open_target){fault=observation_fault;if(observation_stop)InterlockedExchange(observation_stop,1);}
 if(fault==13){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}*out=(HANDLE)(ULONG_PTR)(100+opens);return TRUE;}
static BOOL WINAPI model_thread_token(HANDLE a,DWORD b,BOOL c,PHANDLE d)
{(void)a;(void)b;(void)c;(void)d;SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI model_close(HANDLE h)
{(void)h;closes++;if(fault==14){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static BOOL WINAPI model_lookup(LPCWSTR system,LPCWSTR name,PSID sid,LPDWORD bytes,LPWSTR domain,LPDWORD count,PSID_NAME_USE use)
{(void)system;(void)domain;(void)count;if(wcscmp(name,L"NT SERVICE\\PapinhoAccelerator"))return FALSE;
 if(fault==15){SetLastError(ERROR_NONE_MAPPED);return FALSE;}*use=SidTypeWellKnownGroup;*bytes=GetLengthSid(virtual_sid);return CopySid(*bytes,sid,fault==16?sam_sid:virtual_sid);}
static BOOL WINAPI model_restricted(HANDLE h){(void)h;return fault==8;}
static SC_HANDLE WINAPI model_manager(LPCWSTR a,LPCWSTR b,DWORD rights)
{(void)a;(void)b;(void)rights;if(fault==21){SetLastError(ERROR_ACCESS_DENIED);return NULL;}return (SC_HANDLE)31;}
static SC_HANDLE WINAPI model_service(SC_HANDLE manager,LPCWSTR name,DWORD rights)
{(void)manager;(void)rights;if(wcscmp(name,L"PapinhoAccelerator"))return NULL;return (SC_HANDLE)32;}
static BOOL WINAPI model_config(SC_HANDLE h,LPQUERY_SERVICE_CONFIGW out,DWORD cap,LPDWORD n)
{
 const WCHAR *name=fault==22?L"LocalSystem":L"NT SERVICE\\PapinhoAccelerator";size_t bytes=(wcslen(name)+1)*sizeof(WCHAR);(void)h;(void)cap;
 memset(out,0,sizeof(*out));out->dwServiceType=SERVICE_WIN32_OWN_PROCESS;out->lpServiceStartName=(LPWSTR)((BYTE *)out+sizeof(*out));memcpy(out->lpServiceStartName,name,bytes);*n=(DWORD)(sizeof(*out)+bytes);
 if(fault==25)out->lpServiceStartName=NULL;return TRUE;
}
static BOOL WINAPI model_config2(SC_HANDLE h,DWORD level,LPBYTE out,DWORD cap,LPDWORD n)
{(void)h;(void)level;(void)cap;((SERVICE_SID_INFO *)out)->dwServiceSidType=fault==23?3:fault==26?0:1;*n=sizeof(SERVICE_SID_INFO);return TRUE;}
static DWORD runtime_service_state=SERVICE_RUNNING;
static BOOL WINAPI model_status(SC_HANDLE h,SC_STATUS_TYPE type,LPBYTE out,DWORD cap,LPDWORD n)
{SERVICE_STATUS_PROCESS *s=(SERVICE_STATUS_PROCESS *)out;(void)h;(void)type;(void)cap;memset(s,0,sizeof(*s));s->dwServiceType=SERVICE_WIN32_OWN_PROCESS;s->dwCurrentState=runtime_service_state;s->dwProcessId=fault==24?99:77;*n=sizeof(*s);return TRUE;}
static BOOL WINAPI model_service_close(SC_HANDLE h){if(fault==27){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return model_close(h);}
static PSID identity_expected;
static BOOL WINAPI model_info(HANDLE h,TOKEN_INFORMATION_CLASS kind,LPVOID out,DWORD cap,PDWORD n)
{
 BYTE *b=out;DWORD z;PSID sid=identity_expected;(void)cap;
 if(fault==1&&kind==TokenGroups){SetLastError(ERROR_INSUFFICIENT_BUFFER);return FALSE;}
 if(kind==TokenStatistics){TOKEN_STATISTICS *s=out;memset(s,0,sizeof(*s));s->TokenType=TokenPrimary;s->GroupCount=(observation_service||fault==4||fault==5||fault==6||fault==7||fault==17||fault==18||fault==20)?1:0;s->TokenId.LowPart=(fault==9&&(ULONG_PTR)h>101)?2:1;s->ModifiedId.LowPart=3;*n=sizeof(*s);return TRUE;}
 if(kind==TokenSessionId){*(DWORD *)out=fault==31?1U:0U;*n=sizeof(DWORD);return TRUE;}
 if(kind==TokenUser){TOKEN_USER *u=out;if(fault==2)sid=sid==virtual_sid?sam_sid:virtual_sid;z=GetLengthSid(sid);u->User.Sid=b+sizeof(*u);CopySid(z,u->User.Sid,sid);u->User.Attributes=0;*n=sizeof(*u)+z;return TRUE;}
 if(kind==TokenElevation){((TOKEN_ELEVATION *)out)->TokenIsElevated=shape_elevation?shape_elevation:(fault==3);*n=sizeof(TOKEN_ELEVATION);return TRUE;}
 if(kind==TokenElevationType){*(TOKEN_ELEVATION_TYPE *)out=fault==19?TokenElevationTypeLimited:TokenElevationTypeDefault;*n=sizeof(TOKEN_ELEVATION_TYPE);return TRUE;}
 if(kind==TokenGroups||kind==TokenRestrictedSids){TOKEN_GROUPS *g=out;int has=kind==TokenGroups?(observation_service||fault==4||fault==5||fault==6||fault==7||fault==17||fault==18):(fault==7);
  g->GroupCount=has?1:0;*n=FIELD_OFFSET(TOKEN_GROUPS,Groups);if(has){if(observation_service&&kind==TokenGroups&&fault!=2)sid=service_sid;if(shape_admin_sid&&kind==TokenGroups)sid=shape_admin_sid;z=GetLengthSid(sid);g->Groups[0].Sid=b+sizeof(TOKEN_GROUPS);g->Groups[0].Attributes=fault==4?0:fault==5?(SE_GROUP_ENABLED|SE_GROUP_USE_FOR_DENY_ONLY):fault==18?SE_GROUP_ENABLED_BY_DEFAULT:SE_GROUP_ENABLED;CopySid(z,g->Groups[0].Sid,sid);*n=sizeof(TOKEN_GROUPS)+z;}
  if(fault==17&&kind==TokenGroups)*n=FIELD_OFFSET(TOKEN_GROUPS,Groups);
  return TRUE;}
 SetLastError(ERROR_INVALID_PARAMETER);return FALSE;
}
#define WaitForSingleObject model_wait
#define GetCurrentThreadId model_thread
#define DuplicateHandle model_duplicate
#define GetProcessId model_pid
#define GetProcessTimes model_times
#define GetExitCodeProcess model_exit
#define OpenProcessToken model_open
#define OpenThreadToken model_thread_token
#define GetTokenInformation model_info
#define LookupAccountNameW model_lookup
#define IsTokenRestricted model_restricted
#define CloseHandle model_close
#define OpenSCManagerW model_manager
#define OpenServiceW model_service
#define QueryServiceConfigW model_config
#define QueryServiceConfig2W model_config2
#define QueryServiceStatusEx model_status
#define CloseServiceHandle model_service_close
#include "../apps/server/personal_runtime_identity_win32.inc"
#include "scq_profile_fixture.h"
#include "../apps/server/personal_scq_profile_win32.inc"
#include "../apps/server/server_ca_key_win32.c"
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%u fault=%u\n",(unsigned)__LINE__,fault);return 1;}}while(0)
static void reset(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,unsigned f,unsigned profile)
{memset(r,0,sizeof(*r));fault=f;opens=closes=0;thread=7;use_native_thread=0;observation_open_target=observation_fault=0;observation_stop=NULL;observation_service=0;shape_elevation=0;shape_admin_sid=NULL;identity_expected=profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL?virtual_sid:sam_sid;}
static DWORD WINAPI other_thread(LPVOID value)
{return papacc_personal_runtime_current((PAPACC_PERSONAL_RUNTIME_IDENTITY *)value);}
static int observation_zero(const PAPACC_SCQ_RUNTIME_OBSERVATION *v)
{const BYTE *p=(const BYTE *)v;size_t i;for(i=0;i<sizeof(*v);i++)if(p[i])return 0;return 1;}
static DWORD WINAPI observation_other_thread(LPVOID value)
{PAPACC_SCQ_RUNTIME_OBSERVATION v;volatile LONG stop=0;DWORD e;memset(&v,0xff,sizeof(v));
 e=papacc_personal_runtime_observe_data(value,&v,&stop);
 return e==ERROR_INVALID_STATE&&observation_zero(&v)?0:1;}
int main(void)
{
 PAPACC_PERSONAL_RUNTIME_IDENTITY r;PAPACC_CA_KEY ca;unsigned i,before;DWORD result;HANDLE worker;
 C(ConvertStringSidToSidA("S-1-5-6",&service_sid));C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=1;i<=25;i++){
  if(i==6||i==9||i==10||i==11||i==14)continue;
  reset(&r,i,PAPACC_PERSONAL_RUNTIME_VIRTUAL);result=papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,PAPACC_PERSONAL_RUNTIME_VIRTUAL,1);C(result!=0);
  if(r.phase){fault=0;C(papacc_personal_runtime_close(&r)==0&&r.phase==3&&!r.process&&!r.token&&!r.check_token);}
 }
 for(i=0;i<2;i++){
  reset(&r,0,i?PAPACC_PERSONAL_RUNTIME_SAM:PAPACC_PERSONAL_RUNTIME_VIRTUAL);
  C(papacc_personal_runtime_capture(&r,(HANDLE)1,identity_expected,r.profile=i?PAPACC_PERSONAL_RUNTIME_SAM:PAPACC_PERSONAL_RUNTIME_VIRTUAL,i?0:1)==0);
  C(r.pid==77&&r.creation.dwLowDateTime==99&&r.specific_matches==0&&r.phase==1);
  C(papacc_personal_runtime_current(&r)==0);
  C(papacc_personal_runtime_capture(&r,(HANDLE)1,identity_expected,PAPACC_PERSONAL_RUNTIME_VIRTUAL,1)!=0);
  thread=8;before=opens;C(papacc_personal_runtime_current(&r)!=0&&opens==before);C(papacc_personal_runtime_close(&r)!=0);thread=7;
  C(papacc_personal_runtime_close(&r)==0);before=closes;C(papacc_personal_runtime_close(&r)==0&&closes==before);
 }
 for(i=9;i<=11;i++){
  reset(&r,0,PAPACC_PERSONAL_RUNTIME_VIRTUAL);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);fault=i;C(papacc_personal_runtime_current(&r)!=0);fault=0;C(papacc_personal_runtime_close(&r)==0);
 }
 reset(&r,6,PAPACC_PERSONAL_RUNTIME_VIRTUAL);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);C(r.specific_matches==1&&r.specific_attributes==SE_GROUP_ENABLED);C(papacc_personal_runtime_close(&r)==0);
 reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,3)==ERROR_NOT_SUPPORTED&&opens==0);
 C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,1,0)!=0&&opens==0);
 reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);fault=14;C(papacc_personal_runtime_current(&r)!=0&&r.phase==4&&r.check_token);before=closes;fault=0;C(papacc_personal_runtime_close(&r)!=0&&closes==before);
 reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);fault=14;C(papacc_personal_runtime_close(&r)!=0&&r.phase==4);before=closes;fault=0;C(papacc_personal_runtime_close(&r)!=0&&closes==before);
 reset(&r,26,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,0)==0);C(papacc_personal_runtime_close(&r)==0);
 reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);fault=27;C(papacc_personal_runtime_close(&r)!=0&&r.phase==4&&r.service&&r.manager);before=closes;fault=0;C(papacc_personal_runtime_close(&r)!=0&&closes==before);
 for(i=28;i<=29;i++){reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);fault=i;
 C(papacc_personal_runtime_current(&r)!=0);fault=0;C(papacc_personal_runtime_close(&r)==0);}
 reset(&r,0,2);use_native_thread=1;C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);before=opens;
 worker=CreateThread(NULL,0,other_thread,&r,0,NULL);C(worker!=NULL);C(WaitForSingleObject(worker,10000)==WAIT_OBJECT_0);C(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE&&opens==before);C(papacc_personal_runtime_close(&r)==0);
 /* This is a real thread handle, not one of the modeled process references. */
 C(native_thread_close(worker));
 use_native_thread=0;thread=7;
 /* Bridge uses the actual adapter; process/token/SCM APIs remain modeled.
  * Partial observations cannot become an admissible SCQ peer. */
 {PAPACC_SCQ_RUNTIME_OBSERVATION v;PAPACC_SCQ_PLAN_DATA plan;PAPACC_SCQ_PROFILE_DATA profile;
  volatile LONG stop=0;DWORD saved;unsigned f;
  reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);
  observation_service=1;C(papacc_personal_runtime_observe_data(&r,&v,&stop)==0);
  C(v.known==15U&&v.known!=PAPACC_SCQ_OBS_COMPLETE&&v.peer.pid==77&&v.peer.creation[0]==99);
  C(v.peer.token_id[0]==r.statistics.TokenId.LowPart&&v.peer.modified[0]==3&&EqualSid(v.peer.sid,virtual_sid));
  C(v.peer.groups==r.group_count&&v.peer.enumerated==r.group_count&&v.peer.service_state==SERVICE_RUNNING);
  C(v.peer.logon==1U&&!v.peer.session&&!v.peer.image[0]&&!v.peer.image[1]&&!v.peer.image[2]);
  C(scq_fixture_plan(&plan));plan.runtime=v.peer;memset(&profile,0,sizeof(profile));
  C(papacc_scq_profile_prepare_data(&profile,&r,2,&plan,&v.peer)!=0);
  observation_service=0;memset(&v,0xff,sizeof(v));C(papacc_personal_runtime_observe_data(&r,&v,&stop)!=0&&observation_zero(&v));
  C(papacc_personal_runtime_close(&r)==0);reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);observation_service=1;
  saved=r.phase;C(papacc_personal_runtime_observe_data(&r,(PAPACC_SCQ_RUNTIME_OBSERVATION *)&r,&stop)==ERROR_INVALID_PARAMETER&&r.phase==saved);
  C(papacc_personal_runtime_observe_data(&r,&v,(volatile LONG *)&v)==ERROR_INVALID_PARAMETER);
  stop=1;memset(&v,0xff,sizeof(v));C(papacc_personal_runtime_observe_data(&r,&v,&stop)==ERROR_CANCELLED&&observation_zero(&v));stop=0;
  C(papacc_personal_runtime_close(&r)==0);memset(&v,0xff,sizeof(v));C(papacc_personal_runtime_observe_data(&r,&v,&stop)!=0&&observation_zero(&v));
  for(f=0;f<11;f++){
   static const unsigned failures[]={1,2,4,5,7,9,10,11,28,29,31};
   reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);fault=failures[f];observation_service=1;memset(&v,0xff,sizeof(v));
   C(papacc_personal_runtime_observe_data(&r,&v,&stop)!=0&&observation_zero(&v));fault=0;C(papacc_personal_runtime_close(&r)==0);
  }
  reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);
  observation_service=1;observation_open_target=opens+2;observation_fault=9;memset(&v,0xff,sizeof(v));
  C(papacc_personal_runtime_observe_data(&r,&v,&stop)!=0&&observation_zero(&v));fault=0;observation_open_target=0;C(papacc_personal_runtime_close(&r)==0);
  reset(&r,0,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);
  observation_service=1;observation_open_target=opens+2;observation_stop=&stop;memset(&v,0xff,sizeof(v));
  C(papacc_personal_runtime_observe_data(&r,&v,&stop)==ERROR_CANCELLED&&observation_zero(&v));stop=0;observation_open_target=0;observation_stop=NULL;C(papacc_personal_runtime_close(&r)==0);
  reset(&r,0,1);C(papacc_personal_runtime_capture(&r,(HANDLE)1,sam_sid,1,0)==0);
  memset(&v,0xff,sizeof(v));C(papacc_personal_runtime_observe_data(&r,&v,&stop)==ERROR_NOT_SUPPORTED&&observation_zero(&v));C(papacc_personal_runtime_close(&r)==0);
  reset(&r,0,2);use_native_thread=1;C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)==0);before=opens;
  worker=CreateThread(NULL,0,observation_other_thread,&r,0,NULL);C(worker!=NULL);C(WaitForSingleObject(worker,10000)==WAIT_OBJECT_0);
  C(GetExitCodeThread(worker,&result)&&result==0&&opens==before);C(native_thread_close(worker));C(papacc_personal_runtime_close(&r)==0);
  use_native_thread=0;thread=7;
 }
 /* Actual CA guard, modeled runtime TokenUser: rejects BEFORE any NCrypt call. */
 memset(&ca,0,sizeof(ca));ca.thread=thread;ca.state=PAPACC_CA_KEY_READY;ca.key=1;ca.provider=2;CopySid(sizeof(ca.operator_sid),ca.operator_sid,sam_sid);CopySid(sizeof(ca.service_sid),ca.service_sid,virtual_sid);identity_expected=virtual_sid;
 C(papacc_ca_key_validate(&ca)!=0);C(ca.first_failure.present&&ca.first_failure.stage==CA_D_IDENTITY_EQUAL);
 C(LocalFree(service_sid)==NULL);C(LocalFree(virtual_sid)==NULL);C(LocalFree(sam_sid)==NULL);
 printf("PASS PERSONAL_RUNTIME_IDENTITY checks=%u REAL_SID_PARSING REAL_OTHER_THREAD MODELED_PROCESS_TOKEN_LOOKUP_SCM NO_CNG_CALLS NO_REAL_SCM NO_AUTHORITY bytes=%zu\n",checks,sizeof(r));return 0;
}

