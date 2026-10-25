/* Isolated diagnostic candidate: borrowed originals only, never admission. */
#ifndef PT_OBSERVATION_H
#define PT_OBSERVATION_H
#include <windows.h>
#include <string.h>
#ifndef PT_QUERY
#define PT_QUERY GetTokenInformation
#endif
#ifndef PT_PID
#define PT_PID GetProcessId
#endif
#ifndef PT_TIMES
#define PT_TIMES GetProcessTimes
#endif
#ifndef PT_WAIT
#define PT_WAIT WaitForSingleObject
#endif
#ifndef PT_STOP
#define PT_STOP() 0
#endif
#define PT_ID 1U
#define PT_GROUP 2U
#define PT_FILTER 4U
#define PT_RESTRICT 8U
typedef struct PT_META {DWORD error,known,matches,attributes,filtered_ever,filter_bytes,restricting_count,complete;} PT_META;
static int pt_inside(const BYTE *b,DWORD n,PSID sid){ULONG_PTR s=(ULONG_PTR)b,v=(ULONG_PTR)sid;DWORD z;if(v<s||v-s>n||n-(DWORD)(v-s)<8U)return 0;z=8U+4U*((BYTE *)sid)[1];return z<=n-(DWORD)(v-s)&&IsValidSid(sid)&&GetLengthSid(sid)==z;}
static int pt_same(const TOKEN_STATISTICS *a,const TOKEN_STATISTICS *b){return a->TokenType==TokenPrimary&&b->TokenType==TokenPrimary&&!memcmp(&a->TokenId,&b->TokenId,sizeof(LUID))&&!memcmp(&a->AuthenticationId,&b->AuthenticationId,sizeof(LUID))&&!memcmp(&a->ModifiedId,&b->ModifiedId,sizeof(LUID));}
static DWORD pt_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
/* TokenUser/groups/baseline are caller-owned snapshots; changed generation denies
 * completeness. Query failures remain UNKNOWN, never absent/zero evidence. */
static PT_META pt_token_body(HANDLE token,const TOKEN_STATISTICS *baseline,const BYTE *user,DWORD un,const BYTE *groups,DWORD gn,PSID expected){
 PT_META o;TOKEN_STATISTICS a,b;DWORD n,i,flag=0;const TOKEN_USER *u=(const TOKEN_USER *)user;const TOKEN_GROUPS *g=(const TOKEN_GROUPS *)groups;union {TOKEN_GROUPS aligned;BYTE bytes[512];} restricting;
 ZeroMemory(&o,sizeof(o));if(PT_STOP()){o.error=ERROR_CANCELLED;return o;}
 if(!token||!baseline||!user||un<sizeof(TOKEN_USER)||!pt_inside(user,un,u->User.Sid)||!expected||!IsValidSid(expected)||!EqualSid(u->User.Sid,expected)){o.error=ERROR_INVALID_OWNER;return o;}
 if(!PT_QUERY(token,TokenStatistics,&a,sizeof(a),&n)){o.error=pt_error();return o;}if(n!=sizeof(a)||!pt_same(&a,baseline)){o.error=ERROR_INVALID_DATA;return o;}o.known|=PT_ID;
 if(!groups||gn<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||g->GroupCount>(gn-FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES)){o.error=ERROR_INVALID_DATA;return o;}
 for(i=0;i<g->GroupCount;i++){if(!pt_inside(groups,gn,g->Groups[i].Sid)){o.error=ERROR_INVALID_SID;return o;}if(EqualSid(g->Groups[i].Sid,expected)){o.matches++;o.attributes|=g->Groups[i].Attributes;}}o.known|=PT_GROUP;
 if(PT_STOP()){o.error=ERROR_CANCELLED;return o;}if(!PT_QUERY(token,TokenHasRestrictions,&flag,sizeof(flag),&n)){o.error=pt_error();return o;}if(n!=sizeof(flag)&&n!=sizeof(BYTE)){o.error=ERROR_INVALID_DATA;return o;}o.filtered_ever=flag;o.filter_bytes=n;o.known|=PT_FILTER;
 if(PT_STOP()){o.error=ERROR_CANCELLED;return o;}if(!PT_QUERY(token,TokenRestrictedSids,&restricting,sizeof(restricting),&n)){o.error=pt_error();return o;}if(n>sizeof(restricting)||n<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||restricting.aligned.GroupCount>(n-FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES)){o.error=ERROR_INVALID_DATA;return o;}
 for(i=0;i<restricting.aligned.GroupCount;i++)if(!pt_inside(restricting.bytes,n,restricting.aligned.Groups[i].Sid)){o.error=ERROR_INVALID_SID;return o;}o.restricting_count=restricting.aligned.GroupCount;o.known|=PT_RESTRICT;
 if(PT_STOP()){o.error=ERROR_CANCELLED;return o;}if(!PT_QUERY(token,TokenStatistics,&b,sizeof(b),&n)){o.error=pt_error();return o;}if(n!=sizeof(b)||!pt_same(&a,&b)){o.error=ERROR_INVALID_DATA;return o;}o.complete=1;return o;
}
/* No enum value authorizes an exception to the four-role census. */
enum {PT_PRESENT=1,PT_MISSING_SIGNALED_AFTER_SNAPSHOT,PT_MISSING_LIVE_AFTER_SNAPSHOT,PT_IDENTITY_DIVERGENCE,PT_QUERY_FAILED,PT_TIME_INCONCLUSIVE,PT_STOPPED};
typedef struct PT_ROLE {DWORD result,error,role,pid,wait_result;ULONGLONG creation,snapshot_begin,snapshot_end,observed_at;} PT_ROLE;
static PT_ROLE pt_role_body(HANDLE original,DWORD pid,ULONGLONG birth,unsigned role,unsigned expected,unsigned seen,DWORD enum_error,ULONGLONG begin,ULONGLONG end,ULONGLONG now){
 PT_ROLE o;FILETIME ct,et,kt,ut;DWORD actual;ZeroMemory(&o,sizeof(o));o.role=role;o.snapshot_begin=begin;o.snapshot_end=end;o.observed_at=now;
 if(PT_STOP()){o.result=PT_STOPPED;o.error=ERROR_CANCELLED;return o;}if(enum_error){o.result=PT_QUERY_FAILED;o.error=enum_error;return o;}
 if(role>=4U||(expected&~15U)||(seen&~expected)||!(expected&(1U<<role))||end<begin||now<end||now-end>1000U){o.result=PT_TIME_INCONCLUSIVE;o.error=ERROR_INVALID_DATA;return o;}
 if(seen&(1U<<role)){o.result=PT_PRESENT;return o;}if(!original){o.result=PT_QUERY_FAILED;o.error=ERROR_INVALID_HANDLE;return o;}
 actual=PT_PID(original);if(!actual){o.result=PT_QUERY_FAILED;o.error=pt_error();return o;}o.pid=actual;if(!PT_TIMES(original,&ct,&et,&kt,&ut)){o.result=PT_QUERY_FAILED;o.error=pt_error();return o;}
 o.creation=((ULONGLONG)ct.dwHighDateTime<<32)|ct.dwLowDateTime;if(actual!=pid||o.creation!=birth){o.result=PT_IDENTITY_DIVERGENCE;o.error=ERROR_INVALID_DATA;return o;}
 o.wait_result=PT_WAIT(original,0);if(o.wait_result==WAIT_OBJECT_0)o.result=PT_MISSING_SIGNALED_AFTER_SNAPSHOT;else if(o.wait_result==WAIT_TIMEOUT)o.result=PT_MISSING_LIVE_AFTER_SNAPSHOT;else{o.result=PT_QUERY_FAILED;o.error=o.wait_result==WAIT_FAILED?pt_error():ERROR_INVALID_DATA;}return o;
}
/* Diagnostic queries cannot clobber the caller's original Win32 first error. */
static PT_META pt_token(HANDLE token,const TOKEN_STATISTICS *baseline,const BYTE *user,DWORD un,const BYTE *groups,DWORD gn,PSID expected){DWORD saved=GetLastError();PT_META o=pt_token_body(token,baseline,user,un,groups,gn,expected);SetLastError(saved);return o;}
static PT_ROLE pt_role(HANDLE original,DWORD pid,ULONGLONG birth,unsigned role,unsigned expected,unsigned seen,DWORD enum_error,ULONGLONG begin,ULONGLONG end,ULONGLONG now){DWORD saved=GetLastError();PT_ROLE o=pt_role_body(original,pid,birth,role,expected,seen,enum_error,begin,end,now);SetLastError(saved);return o;}
#endif
