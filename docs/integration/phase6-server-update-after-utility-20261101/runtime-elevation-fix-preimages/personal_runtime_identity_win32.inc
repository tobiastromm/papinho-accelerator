#include "personal_runtime_identity_win32.h"
#include <string.h>
#include <stddef.h>
#include <wchar.h>
/* Diagnostic numeric source site; no new query, reference or authority. */
#include <stdio.h>
static DWORD pri_fail_at(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,DWORD e,unsigned site)
{if(!e)e=ERROR_INVALID_DATA;if(!r->first_error){if(r->closed_preparation==1U)(void)fprintf(stderr,"SCQ_START_RUNTIME_FIRST_CAUSE site=%u error=%lu; ORIGINAL_FAILED_CALL_ONLY; NO_NEW_QUERY; METADATA_NOT_AUTHORITY\n",site,(unsigned long)e);r->first_error=e;}r->phase=2;return e;}
#define pri_fail(r,e) pri_fail_at((r),(e),__LINE__)
static DWORD pri_capture_result(unsigned closed,DWORD e,unsigned site)
{if(closed==1U&&e)(void)fprintf(stderr,"SCQ_START_RUNTIME_CAPTURE_RETURN site=%u error=%lu; ORIGINAL_RETURN_ONLY; METADATA_NOT_AUTHORITY\n",site,(unsigned long)e);return e;}
static int pri_sid_inside(const BYTE *base,DWORD n,PSID sid)
{
 ULONG_PTR b=(ULONG_PTR)base,s=(ULONG_PTR)sid;DWORD z;
 if(s<b||s-b>n||n-(DWORD)(s-b)<8U)return 0;
 z=8U+4U*((const BYTE *)sid)[1];
 return z<=n-(DWORD)(s-b)&&IsValidSid(sid)&&GetLengthSid(sid)==z;
}
static DWORD pri_query(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,HANDLE token,TOKEN_STATISTICS *statistics,DWORD *service_logon)
{
 union {TOKEN_USER align;BYTE bytes[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];} user;
 union {TOKEN_GROUPS align;BYTE bytes[8192];} groups,restricted;
 DWORD n,i,matches=0,attributes=0,service_matches=0,service_attributes=0;TOKEN_ELEVATION elevation;TOKEN_ELEVATION_TYPE elevation_type;
 TOKEN_USER *u=(TOKEN_USER *)user.bytes;TOKEN_GROUPS *g=(TOKEN_GROUPS *)groups.bytes,*s=(TOKEN_GROUPS *)restricted.bytes;
 if(!GetTokenInformation(token,TokenStatistics,statistics,sizeof(*statistics),&n)||n!=sizeof(*statistics))return pri_fail(r,GetLastError());
 if(statistics->TokenType!=TokenPrimary)return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!GetTokenInformation(token,TokenUser,user.bytes,sizeof(user),&n))return pri_fail(r,GetLastError());
 if(n<sizeof(TOKEN_USER)||n>sizeof(user)||!pri_sid_inside(user.bytes,n,u->User.Sid)||!EqualSid(u->User.Sid,r->sid))return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!GetTokenInformation(token,TokenElevation,&elevation,sizeof(elevation),&n)||n!=sizeof(elevation))return pri_fail(r,GetLastError());
 if(elevation.TokenIsElevated)return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!GetTokenInformation(token,TokenElevationType,&elevation_type,sizeof(elevation_type),&n)||n!=sizeof(elevation_type))return pri_fail(r,GetLastError());
 if(elevation_type!=TokenElevationTypeDefault)return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!GetTokenInformation(token,TokenGroups,groups.bytes,sizeof(groups),&n))return pri_fail(r,GetLastError());
 if(n>sizeof(groups)||n<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||g->GroupCount!=statistics->GroupCount||g->GroupCount>(n-(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES))return pri_fail(r,ERROR_INVALID_DATA);
 for(i=0;i<g->GroupCount;i++){
  if(!pri_sid_inside(groups.bytes,n,g->Groups[i].Sid))return pri_fail(r,ERROR_INVALID_DATA);
  if(IsWellKnownSid(g->Groups[i].Sid,WinBuiltinAdministratorsSid)&&(g->Groups[i].Attributes&SE_GROUP_ENABLED))return pri_fail(r,ERROR_ACCESS_DENIED);
  if(IsWellKnownSid(g->Groups[i].Sid,WinServiceSid)){service_matches++;service_attributes=g->Groups[i].Attributes;}
  if(EqualSid(g->Groups[i].Sid,r->sid)){matches++;attributes=g->Groups[i].Attributes;}
 }
 if(matches>1U||(matches==1U&&(!(attributes&SE_GROUP_ENABLED)||(attributes&SE_GROUP_USE_FOR_DENY_ONLY))))return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!GetTokenInformation(token,TokenRestrictedSids,restricted.bytes,sizeof(restricted),&n))return pri_fail(r,GetLastError());
 if(n>sizeof(restricted)||n<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||s->GroupCount>(n-(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES))return pri_fail(r,ERROR_INVALID_DATA);
 for(i=0;i<s->GroupCount;i++)if(!pri_sid_inside(restricted.bytes,n,s->Groups[i].Sid))return pri_fail(r,ERROR_INVALID_DATA);
 /* Restricted/filtered variants require additional effective-access proof;
  * none is manufactured by this identity-only adapter. */
 if(s->GroupCount||IsTokenRestricted(token))return pri_fail(r,ERROR_ACCESS_DENIED);
 if(service_logon)*service_logon=service_matches==1U&&(service_attributes&SE_GROUP_ENABLED)&&!(service_attributes&SE_GROUP_USE_FOR_DENY_ONLY)?1U:0U;
 r->group_count=g->GroupCount;r->specific_matches=matches;r->specific_attributes=attributes;return ERROR_SUCCESS;
}
static DWORD pri_process(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,int initial)
{
 FILETIME created,exit_time,kernel,user;DWORD wait,code,pid=GetProcessId(r->process);
 if(!pid||!GetProcessTimes(r->process,&created,&exit_time,&kernel,&user)||!GetExitCodeProcess(r->process,&code))return pri_fail(r,GetLastError());
 wait=WaitForSingleObject(r->process,0);
 if(wait!=WAIT_TIMEOUT)return pri_fail(r,wait==WAIT_FAILED?GetLastError():ERROR_PROCESS_ABORTED);
 if(code!=STILL_ACTIVE)return pri_fail(r,ERROR_PROCESS_ABORTED);
 if(initial){r->pid=pid;r->creation=created;return 0;}
 return pid==r->pid&&!memcmp(&created,&r->creation,sizeof(created))?0:pri_fail(r,ERROR_ACCESS_DENIED);
}
static DWORD pri_scm(PAPACC_PERSONAL_RUNTIME_IDENTITY *r)
{
 union {QUERY_SERVICE_CONFIGW align;BYTE bytes[8192];} config;
 QUERY_SERVICE_CONFIGW *c=(QUERY_SERVICE_CONFIGW *)config.bytes;SERVICE_SID_INFO sid_type;
 SERVICE_STATUS_PROCESS status;DWORD n;ULONG_PTR offset;
 if(r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL)return 0;
 if(!QueryServiceConfigW(r->service,c,sizeof(config),&n))return pri_fail(r,GetLastError());
 if(n<sizeof(*c)||n>sizeof(config)||(ULONG_PTR)c->lpServiceStartName<(ULONG_PTR)config.bytes)return pri_fail(r,ERROR_INVALID_DATA);
 offset=(ULONG_PTR)c->lpServiceStartName-(ULONG_PTR)config.bytes;
 if(offset>=n||offset%sizeof(WCHAR)||!wmemchr(c->lpServiceStartName,0,(n-(DWORD)offset)/sizeof(WCHAR))||
 wcscmp(c->lpServiceStartName,L"NT SERVICE\\PapinhoAccelerator")||c->dwServiceType!=SERVICE_WIN32_OWN_PROCESS)return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!QueryServiceConfig2W(r->service,SERVICE_CONFIG_SERVICE_SID_INFO,(LPBYTE)&sid_type,sizeof(sid_type),&n))return pri_fail(r,GetLastError());
 if(n!=sizeof(sid_type)||sid_type.dwServiceSidType!=r->expected_sid_type)return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!QueryServiceStatusEx(r->service,SC_STATUS_PROCESS_INFO,(LPBYTE)&status,sizeof(status),&n))return pri_fail(r,GetLastError());
 if(n!=sizeof(status)||status.dwServiceType!=SERVICE_WIN32_OWN_PROCESS||status.dwCurrentState!=(r->closed_preparation==1U?(DWORD)SERVICE_START_PENDING:(DWORD)SERVICE_RUNNING)||status.dwProcessId!=r->pid)return pri_fail(r,ERROR_ACCESS_DENIED);
 return 0;
}
static DWORD pri_capture(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,HANDLE process,PSID sid,unsigned profile,DWORD sid_type,unsigned closed)
{
 BYTE resolved[SECURITY_MAX_SID_SIZE];DWORD bytes=sizeof(resolved),domain_bytes=32,e;WCHAR domain[32];SID_NAME_USE use;
 SID_IDENTIFIER_AUTHORITY nt=SECURITY_NT_AUTHORITY;
 if(!r||r->phase||r->closed_preparation||!process||!IsValidSid(sid)||GetLengthSid(sid)>sizeof(r->sid))return pri_capture_result(closed,ERROR_INVALID_PARAMETER,__LINE__);
 if(profile!=PAPACC_PERSONAL_RUNTIME_SAM&&profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL)return pri_capture_result(closed,ERROR_INVALID_PARAMETER,__LINE__);
 if((profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL&&(sid_type!=0U&&sid_type!=1U))||(profile==PAPACC_PERSONAL_RUNTIME_SAM&&sid_type!=0U))return pri_capture_result(closed,ERROR_NOT_SUPPORTED,__LINE__);
 if(memcmp(GetSidIdentifierAuthority(sid),&nt,sizeof(nt))||*GetSidSubAuthorityCount(sid)!=(profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL?6U:5U)||*GetSidSubAuthority(sid,0)!=(profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL?80U:21U))return pri_capture_result(closed,ERROR_ACCESS_DENIED,__LINE__);
 /* Native name resolution is an additional binding, never token evidence. */
 if(profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL){
  if(!LookupAccountNameW(NULL,L"NT SERVICE\\PapinhoAccelerator",resolved,&bytes,domain,&domain_bytes,&use))return pri_capture_result(closed,GetLastError(),__LINE__);
  if(bytes>sizeof(resolved)||!IsValidSid(resolved)||!EqualSid(resolved,sid)||(use!=SidTypeUser&&use!=SidTypeWellKnownGroup))return pri_capture_result(closed,ERROR_ACCESS_DENIED,__LINE__);
 }
 if(closed&&profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL)return pri_capture_result(closed,ERROR_INVALID_PARAMETER,__LINE__);
 r->closed_preparation=closed;
 r->thread=GetCurrentThreadId();r->profile=profile;r->expected_sid_type=sid_type;r->phase=1;
 r->sid_bytes=GetLengthSid(sid);if(!CopySid(sizeof(r->sid),r->sid,sid))return pri_capture_result(closed,pri_fail(r,GetLastError()),__LINE__);
 if(!DuplicateHandle(GetCurrentProcess(),process,GetCurrentProcess(),&r->process,PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,0))return pri_capture_result(closed,pri_fail(r,GetLastError()),__LINE__);
 e=pri_process(r,1);if(e)return pri_capture_result(closed,e,__LINE__);
 if(profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL){
  r->manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT);if(!r->manager)return pri_capture_result(closed,pri_fail(r,GetLastError()),__LINE__);
  r->service=OpenServiceW(r->manager,L"PapinhoAccelerator",SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS);if(!r->service)return pri_capture_result(closed,pri_fail(r,GetLastError()),__LINE__);
  e=pri_scm(r);if(e)return pri_capture_result(closed,e,__LINE__);
 }
 if(!OpenProcessToken(r->process,TOKEN_QUERY,&r->token))return pri_capture_result(closed,pri_fail(r,GetLastError()),__LINE__);
 e=pri_query(r,r->token,&r->statistics,NULL);if(e)return pri_capture_result(closed,e,__LINE__);
 return pri_capture_result(closed,papacc_personal_runtime_current(r),__LINE__);
}
DWORD papacc_personal_runtime_capture(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,HANDLE process,PSID sid,unsigned profile,DWORD sid_type)
{return pri_capture(r,process,sid,profile,sid_type,0U);}
DWORD papacc_personal_runtime_capture_closed(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,HANDLE process,PSID sid,DWORD sid_type)
{return pri_capture(r,process,sid,PAPACC_PERSONAL_RUNTIME_VIRTUAL,sid_type,1U);}
DWORD papacc_personal_runtime_current(PAPACC_PERSONAL_RUNTIME_IDENTITY *r)
{
 TOKEN_STATISTICS current;DWORD e,close_error;
 if(!r||r->closed_preparation>2U||r->thread!=GetCurrentThreadId()||r->phase!=1U||r->check_token)return ERROR_INVALID_STATE;
 e=pri_process(r,0);if(e)return e;
 e=pri_scm(r);if(e)return e;
 if(!OpenProcessToken(r->process,TOKEN_QUERY,&r->check_token))return pri_fail(r,GetLastError());
 e=pri_query(r,r->check_token,&current,NULL);
 if(!CloseHandle(r->check_token)){close_error=GetLastError();(void)pri_fail(r,close_error);r->phase=4U;return close_error;}r->check_token=NULL;
 if(e)return e;
 if(memcmp(&current.TokenId,&r->statistics.TokenId,sizeof(LUID))||memcmp(&current.AuthenticationId,&r->statistics.AuthenticationId,sizeof(LUID))||memcmp(&current.ModifiedId,&r->statistics.ModifiedId,sizeof(LUID)))return pri_fail(r,ERROR_ACCESS_DENIED);
 e=pri_process(r,0);return e?e:pri_scm(r);
}
/* Only projects facts revalidated using the retained native references. No
 * path/PID reopening, endpoint, image-name inference or authority construction. */
DWORD papacc_personal_runtime_observe_data(PAPACC_PERSONAL_RUNTIME_IDENTITY *r,PAPACC_SCQ_RUNTIME_OBSERVATION *out,volatile LONG *stop)
{
 PAPACC_SCQ_RUNTIME_OBSERVATION v;TOKEN_STATISTICS st;DWORD e,service_logon=0,session=0,bytes=0;ULONG_PTR a=(ULONG_PTR)out,b=(ULONG_PTR)r,t=(ULONG_PTR)stop;
 if(!out||!r||!stop)return ERROR_INVALID_PARAMETER;
 /* Reject aliasing before clearing output: never overwrite held originals. */
 if((a>=b?a-b<sizeof(*r):b-a<sizeof(*out))||
    (a>=t?a-t<sizeof(*stop):t-a<sizeof(*out)))return ERROR_INVALID_PARAMETER;
 memset(out,0,sizeof(*out));memset(&v,0,sizeof(v));
 if(InterlockedCompareExchange(stop,0,0))return ERROR_CANCELLED;
 if(r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL)return ERROR_NOT_SUPPORTED;
 e=papacc_personal_runtime_current(r);if(e)return e;
 e=pri_query(r,r->token,&st,&service_logon);if(e)return e;
 if(memcmp(&st.TokenId,&r->statistics.TokenId,sizeof(LUID))||memcmp(&st.AuthenticationId,&r->statistics.AuthenticationId,sizeof(LUID))||
  memcmp(&st.ModifiedId,&r->statistics.ModifiedId,sizeof(LUID)))return pri_fail(r,ERROR_ACCESS_DENIED);
 if(!GetTokenInformation(r->token,TokenSessionId,&session,sizeof(session),&bytes)||bytes!=sizeof(session))return pri_fail(r,GetLastError());
 if(session||!service_logon)return pri_fail(r,ERROR_ACCESS_DENIED);
 if(r->sid_bytes>sizeof(v.peer.sid))return ERROR_INVALID_DATA;
 v.peer.pid=r->pid;v.peer.creation[0]=r->creation.dwLowDateTime;v.peer.creation[1]=r->creation.dwHighDateTime;
 v.peer.token_id[0]=r->statistics.TokenId.LowPart;v.peer.token_id[1]=(DWORD)r->statistics.TokenId.HighPart;
 v.peer.authentication[0]=r->statistics.AuthenticationId.LowPart;v.peer.authentication[1]=(DWORD)r->statistics.AuthenticationId.HighPart;
 v.peer.modified[0]=r->statistics.ModifiedId.LowPart;v.peer.modified[1]=(DWORD)r->statistics.ModifiedId.HighPart;
 v.peer.groups=r->group_count;v.peer.enumerated=r->group_count;
 v.peer.matches=r->specific_matches;v.peer.attributes=r->specific_attributes;
 v.peer.sid_bytes=r->sid_bytes;memcpy(v.peer.sid,r->sid,r->sid_bytes);
 /* These constants represent predicates checked by current(), not defaults
  * for fields that current() does not observe. */
 v.peer.token_type=TokenPrimary;v.peer.elevation_type=TokenElevationTypeDefault;
 v.peer.wait_status=WAIT_TIMEOUT;v.peer.exit_code=STILL_ACTIVE;
 v.peer.service_type=SERVICE_WIN32_OWN_PROCESS;v.peer.service_state=r->closed_preparation==1U?SERVICE_START_PENDING:SERVICE_RUNNING;
 v.peer.service_sid_type=r->expected_sid_type;v.peer.session=session;v.peer.logon=service_logon;
 e=papacc_personal_runtime_current(r);if(e)return e;
 if(InterlockedCompareExchange(stop,0,0))return ERROR_CANCELLED;
 v.known=PAPACC_SCQ_OBS_PROCESS|PAPACC_SCQ_OBS_TOKEN|PAPACC_SCQ_OBS_SCM|PAPACC_SCQ_OBS_SESSION_LOGON;
 /* Original image remains zero with its known bit absent. The
  * strict SCQ profile rejects this partial peer; callers must not patch it. */
 *out=v;return ERROR_SUCCESS;
}
DWORD papacc_personal_runtime_close(PAPACC_PERSONAL_RUNTIME_IDENTITY *r)
{
 HANDLE *refs[3];unsigned i;
 if(!r||r->thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if(r->phase==3U)return 0;
 /* A failed close is retained and cannot be retried by this adapter. */
 if(r->phase==4U)return r->first_error?r->first_error:ERROR_INVALID_STATE;
 refs[0]=&r->check_token;refs[1]=&r->token;refs[2]=&r->process;
 for(i=0;i<3;i++)if(*refs[i]){if(!CloseHandle(*refs[i])){DWORD e=GetLastError();(void)pri_fail(r,e);r->phase=4U;return e;}*refs[i]=NULL;}
 /* SCM references use their own close API; no handle-type aliasing. */
 if(r->service){if(!CloseServiceHandle(r->service)){DWORD e=GetLastError();(void)pri_fail(r,e);r->phase=4U;return e;}r->service=NULL;}
 if(r->manager){if(!CloseServiceHandle(r->manager)){DWORD e=GetLastError();(void)pri_fail(r,e);r->phase=4U;return e;}r->manager=NULL;}
 r->phase=3U;return 0;
}


#undef pri_fail
