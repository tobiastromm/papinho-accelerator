/* Actual caller/API-simulated qualification. NEVER execute qualifier entry. */
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <winternl.h>
#include <aclapi.h>
#include <sddl.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <processsnapshot.h>
#include <winevt.h>
#include <wintrust.h>
#include <stdio.h>
#include <string.h>
#include "../apps/server/server_console_win32.h"
static unsigned mode,stop_flag,opens,queries,anchor_closes,aux_closes,anchor_live,close_failure,cases;
static BYTE sid_bytes[12]={1,1,0,0,0,0,0,5,21,0,0,0};
static PAPACC_BOOL fx_stop(void){return stop_flag?PAPACC_TRUE:PAPACC_FALSE;}
static void fx_request(void){stop_flag=1;}
static BOOL WINAPI fx_thread(HANDLE h,DWORD access,BOOL self,PHANDLE out){(void)h;(void)access;(void)self;++queries;if(mode==8){*out=(HANDLE)(ULONG_PTR)0xb000;return TRUE;}*out=NULL;SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI fx_process(HANDLE h,DWORD access,PHANDLE out);
static BOOL WINAPI fx_token(HANDLE h,TOKEN_INFORMATION_CLASS kind,LPVOID p,DWORD cap,PDWORD used);
static BOOL WINAPI fx_close(HANDLE h){if(h==(HANDLE)(ULONG_PTR)0x9001){++anchor_closes;if(close_failure){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}if(!anchor_live){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}anchor_live=0;return TRUE;}++aux_closes;return TRUE;}
static BOOL WINAPI fx_sid(WELL_KNOWN_SID_TYPE t,PSID domain,PSID out,DWORD *n){(void)t;(void)domain;if(*n<sizeof(sid_bytes))return FALSE;memcpy(out,sid_bytes,sizeof(sid_bytes));*n=sizeof(sid_bytes);return TRUE;}
static BOOL WINAPI fx_member(HANDLE t,PSID sid,PBOOL member){(void)t;(void)sid;*member=mode!=5;return TRUE;}
#define OpenThreadToken fx_thread
#define OpenProcessToken fx_process
#define GetTokenInformation fx_token
#define CloseHandle fx_close
#define CreateWellKnownSid fx_sid
#define CheckTokenMembership fx_member
#define papacc_server_console_win32_stop_requested fx_stop
#define papacc_server_console_win32_request_stop fx_request
#define main NEVER_EXECUTE_QUALIFIER_ENTRY
#include "../tools/qualification/scm-registration-lab.c"
#undef main
#include "../docs/integration/premonitor-self-token-lifetime-20261024/applied-branch.UNMODIFIED.inc"
static BOOL WINAPI fx_process(HANDLE h,DWORD access,PHANDLE out){(void)h;(void)access;++opens;if(out==&lab_token){anchor_live=1;*out=(HANDLE)(ULONG_PTR)0x9001;}else *out=(HANDLE)(ULONG_PTR)(0xa000+opens);return TRUE;}
static BOOL WINAPI fx_token(HANDLE h,TOKEN_INFORMATION_CLASS kind,LPVOID p,DWORD cap,PDWORD used)
{
 if(kind==TokenUser){TOKEN_USER *u=(TOKEN_USER *)p;if(cap<sizeof(*u))return FALSE;u->User.Sid=sid_bytes;*used=sizeof(*u);return mode!=1;}
 if(kind==TokenElevation){((TOKEN_ELEVATION *)p)->TokenIsElevated=mode!=2;*used=sizeof(TOKEN_ELEVATION);return TRUE;}
 if(kind==TokenElevationType){*(TOKEN_ELEVATION_TYPE *)p=mode==3?TokenElevationTypeLimited:TokenElevationTypeDefault;*used=sizeof(TOKEN_ELEVATION_TYPE);return TRUE;}
 if(kind==TokenStatistics){TOKEN_STATISTICS *s=(TOKEN_STATISTICS *)p;if(mode==4&&h==(HANDLE)(ULONG_PTR)0x9001)return FALSE;ZeroMemory(s,sizeof(*s));s->TokenId.LowPart=17;s->AuthenticationId.LowPart=18;s->ModifiedId.LowPart=19;s->TokenType=TokenPrimary;if(h!=(HANDLE)(ULONG_PTR)0x9001){if(mode==10)++s->TokenId.LowPart;if(mode==11)++s->AuthenticationId.LowPart;if(mode==12)++s->ModifiedId.LowPart;}*used=sizeof(*s);return TRUE;}
 return FALSE;
}
#define T(x) do{++cases;if(!(x)){printf("FAIL %u %s\n",(unsigned)__LINE__,#x);return 1;}}while(0)
static void reset(void){ZeroMemory(&native,sizeof(native));ZeroMemory(&lab_aux_owner,sizeof(lab_aux_owner));ZeroMemory(&lab_stats,sizeof(lab_stats));lab_token=NULL;lab_sid=sid_bytes;mode=stop_flag=opens=queries=anchor_closes=aux_closes=anchor_live=close_failure=0;native.thread=GetCurrentThreadId();}
int main(void)
{
 unsigned positive,n0,i,before,aq;DWORD e;TOKEN_STATISTICS saved;
 for(positive=0;positive<2;positive++)for(n0=0;n0<2;n0++){
  reset();T(lab_operator()==0);saved=lab_stats;T(anchor_live&&lab_token);T(test_after_initial_success((int)positive,(int)n0)==0);T(!memcmp(&saved,&lab_stats,sizeof(saved)));
  T(anchor_closes==(positive&&n0?1U:0U));T((lab_token==NULL)==(positive&&n0));
  for(i=0;i<32;i++)T(lab_operator_current()==0);
  if(positive&&n0){before=anchor_closes;T(test_after_initial_success(1,1)==ERROR_INVALID_STATE);T(anchor_closes==before);}else{T(fx_close(lab_token));lab_token=NULL;}
  T(anchor_closes==1&&anchor_live==0&&a1_finish(&lab_aux_owner));
 }
 for(i=10;i<=12;i++){reset();T(lab_operator()==0);T(test_after_initial_success(1,1)==0);mode=i;T(lab_operator_current()==ERROR_ACCESS_DENIED);T(!lab_token&&anchor_closes==1&&a1_finish(&lab_aux_owner));}
 reset();T(lab_operator()==0);T(test_after_initial_success(1,1)==0);mode=8;T(lab_operator_current()==ERROR_ACCESS_DENIED);T(aux_closes==1&&anchor_closes==1&&a1_finish(&lab_aux_owner));
 reset();T(lab_operator()==0);T(test_after_initial_success(1,1)==0);stop_flag=1;before=opens;aq=queries;T(lab_operator_current()!=0);T(opens==before&&queries==aq&&anchor_closes==1);
 reset();T(lab_operator()==0);saved=lab_stats;close_failure=1;T(test_after_initial_success(1,1)==ERROR_ACCESS_DENIED);T(anchor_live&&lab_token==(HANDLE)(ULONG_PTR)0x9001&&anchor_closes==1);T(native.retained&&stop_flag&&(lab_aux_owner.state&A1_FROZEN));T(!memcmp(&saved,&lab_stats,sizeof(saved)));before=opens;aq=queries;
 T(lab_operator_current()==ERROR_ACCESS_DENIED);T(ig_project_census()==ERROR_ACCESS_DENIED);T(ig_cleanup()!=0);T(cycle_cleanup()!=0);T(opens==before&&queries==aq&&anchor_closes==1&&anchor_live);T(lab_aux_owner.first_error==ERROR_ACCESS_DENIED);
 /* Fault contexts are synthetic, reset ONLY fake storage between cases. */
 for(i=1;i<=5;i++){reset();mode=i;e=lab_operator();T(e!=0);T(anchor_closes==0);if(!e)(void)test_after_initial_success(1,1);T(anchor_closes==0);if(lab_token){T(fx_close(lab_token));lab_token=NULL;}}
 reset();mode=8;e=lab_operator();T(e==ERROR_ACCESS_DENIED);T(!lab_token&&opens==0&&anchor_closes==0);
 reset();T(lab_operator()==0);e=ERROR_ACCESS_DENIED; /* boot validation failure prevents branch */
 if(!e)(void)test_after_initial_success(1,1);T(anchor_closes==0&&anchor_live);T(fx_close(lab_token));lab_token=NULL;
 puts("ACTUAL_CALLERS=lab_operator,lab_operator_current; APPLIED_BRANCH_EXACT; FOUR_ROUTE_COMBINATIONS; INITIAL_FAILURES; TOKENID_AUTHID_MODIFIEDID; IMPERSONATION; STOP_FREEZE_RETENTION; NO_SECOND_CLOSE; NO_OPERATIONAL_ENTRY");
 printf("SELF_TOKEN_LIFETIME=PASS checks=%u; API_SIMULATED_NOT_NATIVE_ADMISSION\n",cases);return 0;
}
