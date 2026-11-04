#include "server_configuration_commit_win32.h"
#include <sddl.h>
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT model;
static const char *predicate(DWORD p)
{switch(p){case 0:return "NONE_OR_UNINSTRUMENTED";case 1:return "NATIVE_PHASE_OR_STOP";case 2:return "NO_IMPERSONATION_TOKEN";case 3:return "ADMIN_ENABLED";case 4:return "INTERACTIVE_ENABLED";case 5:return "SERVICE_GROUP_ABSENT";case 6:return "PRIMARY_TOKEN_OPEN";case 7:return "TOKEN_STATISTICS_QUERY";case 8:return "PRIMARY_TOKEN_TYPE";case 9:return "ELEVATION_QUERY";case 10:return "ELEVATED_REQUIRED";case 11:return "ELEVATION_TYPE_QUERY";case 12:return "NOT_LIMITED_TOKEN";case 13:return "TOKEN_SESSION_QUERY";case 14:return "NONZERO_SESSION";case 15:return "PROCESS_SESSION_QUERY";case 16:return "ACTIVE_CONSOLE_SESSION";case 17:return "TOKEN_USER_QUERY";case 18:return "VALID_OPERATOR_SID";case 19:return "SID_COPY";case 20:return "CURRENT_TOKEN_OPEN";case 21:return "CURRENT_TOKEN_CLOSE";case 22:return "CURRENT_TOKEN_CONTINUITY";case 23:return "ORIGINAL_TOKEN_CONTINUITY";case 30:return "DOS_DEVICE_QUERY";case 31:return "LOCAL_DEVICE_REQUIRED";case 32:return "ORIGINAL_PARENT_OPEN";case 33:return "PARENT_IDENTITY_QUERY";case 34:return "PARENT_DIRECTORY_NO_REPARSE";case 35:return "PARENT_SECURITY_QUERY";case 36:return "VALID_PARENT_DESCRIPTOR";case 37:return "PARENT_DACL_QUERY";case 38:return "PARENT_DACL_PRESENT";case 39:return "OPERATOR_SID_FORMAT";default:return "UNMAPPED";}}
int main(void)
{
 char path[MAX_PATH],host[256]="UNKNOWN";DWORD n,first,error,kind,size=sizeof(host),used;PAPACC_RESULT r,closed;HANDLE token=NULL;
 TOKEN_ELEVATION elevation={0};TOKEN_ELEVATION_TYPE type=TokenElevationTypeDefault;
 union {TOKEN_USER alignment;BYTE bytes[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];} user;
 LPSTR sid=NULL;int elevation_known=0,type_known=0,sid_known=0,observer_closed=1,parent_opened;DWORD observer_error=0,admin_attributes=0,token_session=0,process_session=0,active_session=WTSGetActiveConsoleSessionId();int groups_known=0,admin_present=0,session_known=0;
 union {TOKEN_GROUPS alignment;BYTE bytes[4096];} groups;BYTE admin_sid[SECURITY_MAX_SID_SIZE];DWORD admin_bytes=sizeof(admin_sid),index;
 model.writer=GetCurrentThreadId();
 if(papacc_configuration_personal_native_current(&model)==PAPACC_RESULT_OK)return 1;
 n=GetTempPathA(MAX_PATH,path);if(!n||n>=MAX_PATH)return 2;
 r=papacc_configuration_personal_native_open(&model,path);
 first=model.pending_commit.personal_native.diagnostic_first;error=model.pending_commit.personal_native.diagnostic_error;kind=model.pending_commit.personal_native.diagnostic_kind;
 parent_opened=model.pending_commit.bootstrap_parent!=NULL;
 /* Independent metadata query AFTER first failure. Never feeds the guard. */
 if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token)){
  elevation_known=GetTokenInformation(token,TokenElevation,&elevation,sizeof(elevation),&used)!=0;
  type_known=GetTokenInformation(token,TokenElevationType,&type,sizeof(type),&used)!=0;
  if(GetTokenInformation(token,TokenUser,&user,sizeof(user),&used)&&IsValidSid(((TOKEN_USER *)&user)->User.Sid))sid_known=ConvertSidToStringSidA(((TOKEN_USER *)&user)->User.Sid,&sid)!=0;
  session_known=GetTokenInformation(token,TokenSessionId,&token_session,sizeof(token_session),&used)!=0;
  (void)ProcessIdToSessionId(GetCurrentProcessId(),&process_session);
  groups_known=GetTokenInformation(token,TokenGroups,&groups,sizeof(groups),&used)!=0;
  if(groups_known&&CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admin_sid,&admin_bytes)){
   TOKEN_GROUPS *g=(TOKEN_GROUPS *)&groups;for(index=0;index<g->GroupCount;index++)if(EqualSid(admin_sid,g->Groups[index].Sid)){admin_present=1;admin_attributes=g->Groups[index].Attributes;}
  }
  if(!CloseHandle(token)){observer_closed=0;observer_error=GetLastError();}
 }else observer_error=GetLastError();
 (void)GetComputerNameA(host,&size);
 closed=papacc_configuration_personal_native_close(&model);
 printf("{\"observation\":\"INDEPENDENT_AFTER_FIRST_FAILURE\",\"admin_group_query_known\":%s,\"admin_sid_present\":%s,\"admin_group_attributes\":%lu,\"session_known\":%s,\"token_session\":%lu,\"process_session\":%lu,\"active_console_session\":%lu,\"authority\":\"DENY\"}\n",groups_known?"true":"false",admin_present?"true":"false",admin_attributes,session_known?"true":"false",token_session,process_session,active_session);
 printf("{\"first_predicate\":\"%s\",\"predicate_id\":%lu,\"failure_kind\":%lu,\"native_error\":%lu,\"preflight_result\":%u,\"operator_sid\":\"%s\",\"elevation_known\":%s,\"elevated\":%s,\"elevation_type_known\":%s,\"elevation_type\":%u,\"pid\":%lu,\"host_metadata\":\"%s\",\"uptime_ms\":%llu,\"boot_identity\":\"NOT_ESTABLISHED_BY_UPTIME\",\"parent_scope\":\"TEMP_DIRECTORY_PROBE_NOT_INSTALLATION_TARGET\",\"parent_opened\":%s,\"native_close_result\":%u,\"observer_closed\":%s,\"observer_error\":%lu,\"authority\":\"DENY\"}\n",
 predicate(first),first,kind,error,(unsigned)r,sid_known?sid:"UNKNOWN",elevation_known?"true":"false",elevation.TokenIsElevated?"true":"false",type_known?"true":"false",(unsigned)type,GetCurrentProcessId(),host,GetTickCount64(),parent_opened?"true":"false",(unsigned)closed,observer_closed?"true":"false",observer_error);
 if(sid&&LocalFree(sid))return 9;if(!observer_closed||closed!=PAPACC_RESULT_OK)return 3;
 if(r!=PAPACC_RESULT_OK){puts("NATIVE_READ_ONLY_PREFLIGHT_DENIED; EXACT_FIRST_PREDICATE_REPORTED; POSITIVE_NOT_QUALIFIED");return 0;}
 if(model.pending_commit.bootstrap_parent||model.pending_commit.bootstrap_token)return 8;
 puts("NATIVE_READ_ONLY_PREFLIGHT_ELIGIBLE_PARENT_TOKEN_REVALIDATED_CLOSED_NO_CREATION");return 0;
}
