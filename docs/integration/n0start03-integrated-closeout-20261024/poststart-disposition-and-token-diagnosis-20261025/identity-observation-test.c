/* Native own-process and substituted-API fixtures; no product authority. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
static unsigned mode,queries,stop,native_mode;
static BYTE model_sid[SECURITY_MAX_SID_SIZE],other[SECURITY_MAX_SID_SIZE];
static BOOL WINAPI query(HANDLE,TOKEN_INFORMATION_CLASS,LPVOID,DWORD,PDWORD);
static DWORD WINAPI pid_of(HANDLE);
static BOOL WINAPI times_of(HANDLE,LPFILETIME,LPFILETIME,LPFILETIME,LPFILETIME);
static DWORD WINAPI wait_of(HANDLE,DWORD);
#define PT_QUERY query
#define PT_PID pid_of
#define PT_TIMES times_of
#define PT_WAIT wait_of
#define PT_STOP() (stop||(mode==15&&queries>=2))
#include "identity-observation.NOT-INTEGRATED.h"
static BOOL WINAPI query(HANDLE token,TOKEN_INFORMATION_CLASS cls,LPVOID out,DWORD cap,PDWORD used){queries++;if(native_mode)return GetTokenInformation(token,cls,out,cap,used);
 if(mode==20&&cls==TokenStatistics){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(cls==TokenStatistics){TOKEN_STATISTICS *s=out;if(cap<sizeof(*s))return FALSE;ZeroMemory(s,sizeof(*s));s->TokenType=mode==10?TokenImpersonation:TokenPrimary;s->TokenId.LowPart=1;s->AuthenticationId.LowPart=2;s->ModifiedId.LowPart=(mode==11||(mode==12&&queries==4))?4:3;*used=sizeof(*s);return TRUE;}
 if(cls==TokenHasRestrictions){if(mode==7){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}*(DWORD *)out=mode==23?512U:(mode==6||mode==17||mode==22);*used=mode==18?2:(mode==22?1:sizeof(DWORD));return TRUE;}
 if(cls==TokenRestrictedSids){TOKEN_GROUPS *g=out;BYTE *next=(BYTE *)out+FIELD_OFFSET(TOKEN_GROUPS,Groups)+sizeof(SID_AND_ATTRIBUTES);if(mode==8){*used=1024;SetLastError(ERROR_INSUFFICIENT_BUFFER);return FALSE;}if(cap<128)return FALSE;ZeroMemory(out,cap);g->GroupCount=mode==6||mode==19;if(g->GroupCount){g->Groups[0].Sid=next;g->Groups[0].Attributes=0;CopySid(SECURITY_MAX_SID_SIZE,next,model_sid);}*used=(DWORD)(next-(BYTE *)out)+GetLengthSid(model_sid);if(mode==19)g->Groups[0].Sid=(BYTE *)out+cap;return TRUE;}
 SetLastError(ERROR_INVALID_PARAMETER);return FALSE;
}
static DWORD WINAPI pid_of(HANDLE h){if(native_mode)return GetProcessId(h);if(mode==34){SetLastError(ERROR_ACCESS_DENIED);return 0;}return mode==32?201:200;}
static BOOL WINAPI times_of(HANDLE h,LPFILETIME c,LPFILETIME e,LPFILETIME k,LPFILETIME u){if(native_mode)return GetProcessTimes(h,c,e,k,u);ZeroMemory(c,sizeof(*c));ZeroMemory(e,sizeof(*e));ZeroMemory(k,sizeof(*k));ZeroMemory(u,sizeof(*u));c->dwLowDateTime=mode==33?301:300;return TRUE;}
static DWORD WINAPI wait_of(HANDLE h,DWORD t){if(native_mode)return WaitForSingleObject(h,t);if(mode==35){SetLastError(ERROR_INVALID_HANDLE);return WAIT_FAILED;}return mode==30?WAIT_OBJECT_0:WAIT_TIMEOUT;}
#define T(x) do{if(!(x)){printf("FAIL line=%d mode=%u error=%lu RETAINED_FIXTURE_REFS_NO_RETRY\n",__LINE__,mode,(unsigned long)GetLastError());return 1;}}while(0)
static HANDLE token;static PROCESS_INFORMATION child;
int main(int argc,char **argv){unsigned m;DWORD n,un,gn;TOKEN_STATISTICS stats;union {TOKEN_USER aligned;BYTE bytes[256];} user;union {TOKEN_GROUPS aligned;BYTE bytes[4096];} groups;PT_META x;PT_ROLE role;DWORD sn=sizeof(model_sid),on=sizeof(other);FILETIME ct,et,kt,ut;ULONGLONG birth,tick;WCHAR exe[512],cmd[600];STARTUPINFOW si;
 if(argc==2&&!strcmp(argv[1],"--exit-fixture-child"))return 0;
 T(CreateWellKnownSid(WinLocalServiceSid,NULL,model_sid,&sn)&&CreateWellKnownSid(WinNetworkServiceSid,NULL,other,&on));ZeroMemory(&stats,sizeof(stats));stats.TokenType=TokenPrimary;stats.TokenId.LowPart=1;stats.AuthenticationId.LowPart=2;stats.ModifiedId.LowPart=3;
 for(m=0;m<24;m++){BYTE *next;mode=m;queries=stop=0;ZeroMemory(&user,sizeof(user));user.aligned.User.Sid=user.bytes+sizeof(TOKEN_USER);CopySid(SECURITY_MAX_SID_SIZE,user.aligned.User.Sid,mode==9?other:model_sid);un=sizeof(TOKEN_USER)+sn;if(mode==21)user.aligned.User.Sid=user.bytes+sizeof(user);
  ZeroMemory(&groups,sizeof(groups));groups.aligned.GroupCount=mode==1?0:(mode==16?2:1);next=groups.bytes+FIELD_OFFSET(TOKEN_GROUPS,Groups)+2*sizeof(SID_AND_ATTRIBUTES);for(n=0;n<groups.aligned.GroupCount;n++){groups.aligned.Groups[n].Sid=next;groups.aligned.Groups[n].Attributes=mode==2?10:(mode==3?16:(mode==4?0:(mode==5?20:4)));CopySid(SECURITY_MAX_SID_SIZE,next,model_sid);next+=sn;}gn=(DWORD)(next-groups.bytes);if(mode==13)groups.aligned.Groups[0].Sid=groups.bytes+sizeof(groups);if(mode==14)stop=1;
  SetLastError(12345);x=pt_token((HANDLE)(ULONG_PTR)10,&stats,user.bytes,un,groups.bytes,gn,model_sid);T(GetLastError()==12345);
  if(m<=6||m==16||m==17||m==22||m==23){T(x.complete&&!x.error&&x.known==15);T(x.matches==(m==1?0U:(m==16?2U:1U)));if(m==2)T(x.attributes==10);if(m==3)T(x.attributes==16);if(m==5)T(x.attributes==20);if(m==6)T(x.filtered_ever&&x.restricting_count==1);if(m==17)T(x.filtered_ever&&!x.restricting_count);if(m==22)T(x.filter_bytes==1&&x.filtered_ever==1);if(m==23)T(x.filter_bytes==4&&x.filtered_ever==512);}
  else{T(x.error&&!x.complete);if(m==7)T(!(x.known&PT_FILTER));if(m==8||m==19)T(!(x.known&PT_RESTRICT));if(m==14)T(!queries);if(m==15)T(queries==2);if(m==9||m==21)T(x.error==ERROR_INVALID_OWNER);}
 }
 stop=0;for(m=30;m<=39;m++){mode=m;queries=0;SetLastError(12345);role=pt_role((HANDLE)(ULONG_PTR)20,200,300,2,15,m==36?15:11,m==37?ERROR_ACCESS_DENIED:0,100,110,m==38?99:120);T(GetLastError()==12345);if(m==30)T(role.result==PT_MISSING_SIGNALED_AFTER_SNAPSHOT);if(m==31)T(role.result==PT_MISSING_LIVE_AFTER_SNAPSHOT);if(m==32||m==33)T(role.result==PT_IDENTITY_DIVERGENCE);if(m==34||m==35||m==37)T(role.result==PT_QUERY_FAILED);if(m==36)T(role.result==PT_PRESENT);if(m==38)T(role.result==PT_TIME_INCONCLUSIVE);if(m==39){stop=1;role=pt_role(NULL,200,300,2,15,11,0,100,110,120);T(role.result==PT_STOPPED);stop=0;}}
 native_mode=1;mode=100;
 T(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token));
 T(GetTokenInformation(token,TokenStatistics,&stats,sizeof(stats),&n));
 T(GetTokenInformation(token,TokenUser,&user,sizeof(user),&un));
 T(GetTokenInformation(token,TokenGroups,&groups,sizeof(groups),&gn));
 {DWORD flag=0,bytes=0;BOOL ok=GetTokenInformation(token,TokenHasRestrictions,&flag,sizeof(flag),&bytes);printf("HAS_RESTRICTIONS_QUERY success=%u bytes=%lu raw=%lu error=%lu\n",(unsigned)ok,(unsigned long)bytes,(unsigned long)flag,(unsigned long)(ok?0:GetLastError()));}
 x=pt_token(token,&stats,user.bytes,un,groups.bytes,gn,user.aligned.User.Sid);
 printf("NATIVE_METADATA error=%lu known=%lu complete=%lu\n",(unsigned long)x.error,(unsigned long)x.known,(unsigned long)x.complete);T(x.complete&&x.known==15);printf("NATIVE_OWN_TOKEN known=%lu filtered_ever=%lu restricting_sid_count=%lu NOT_SERVICE_TOKEN\n",(unsigned long)x.known,(unsigned long)x.filtered_ever,(unsigned long)x.restricting_count);T(CloseHandle(token));token=NULL;
 T(GetProcessTimes(GetCurrentProcess(),&ct,&et,&kt,&ut));birth=((ULONGLONG)ct.dwHighDateTime<<32)|ct.dwLowDateTime;tick=GetTickCount64();role=pt_role(GetCurrentProcess(),GetCurrentProcessId(),birth,2,15,11,0,tick,tick,tick);T(role.result==PT_MISSING_LIVE_AFTER_SNAPSHOT);
 T(GetModuleFileNameW(NULL,exe,512)>0);T(_snwprintf_s(cmd,600,_TRUNCATE,L"\"%s\" --exit-fixture-child",exe)>0);ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);T(CreateProcessW(exe,cmd,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&si,&child));T(CloseHandle(child.hThread));child.hThread=NULL;T(GetProcessTimes(child.hProcess,&ct,&et,&kt,&ut));birth=((ULONGLONG)ct.dwHighDateTime<<32)|ct.dwLowDateTime;T(WaitForSingleObject(child.hProcess,5000)==WAIT_OBJECT_0);tick=GetTickCount64();role=pt_role(child.hProcess,child.dwProcessId,birth,2,15,11,0,tick,tick,tick);T(role.result==PT_MISSING_SIGNALED_AFTER_SNAPSHOT);T(CloseHandle(child.hProcess));child.hProcess=NULL;
 printf("IDENTITY_OBSERVATION=PASS token_models=24 role_models=10 native_token=1 native_role_lifetime=2 metadata_bytes=%zu role_bytes=%zu NO_LAB_NO_SCM_NO_ADMISSION\n",sizeof(PT_META),sizeof(PT_ROLE));return 0;}
