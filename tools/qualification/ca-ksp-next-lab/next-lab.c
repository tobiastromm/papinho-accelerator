/* Standalone LAB qualifier of the EXISTING custody owner; no product entry.
 * Preparation only. --describe does not query a token, provider or key.
 * --execute-new-lab requires a separate exact owner/executor approval. */
#define _WIN32_WINNT 0x0600
#include "server_ca_key_win32.h"
#include <sddl.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static PAPACC_CA_KEY owner;
static volatile LONG stop_requested;
static unsigned lab_holder;
static const BYTE authority[16]={0x23,0x45,0xe0,0x7a,0xdf,0x71,0x40,0x45,0xa2,0x0d,0x1c,0x8b,0x6f,0xdf,0x5a,0xaf};
static const BYTE generation[16]={0xa2,0xbc,0xd6,0xd2,0xed,0x30,0x43,0xd0,0x83,0xe7,0xe5,0x03,0x42,0xd6,0xfa,0xb0};
static const wchar_t expected_name[]=L"PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0";
static const char expected_sid[]="S-1-5-21-785699550-1440458426-1682014700-1108";
static BOOL WINAPI on_control(DWORD event)
{if(event!=CTRL_C_EVENT&&event!=CTRL_BREAK_EVENT)return FALSE;InterlockedExchange(&stop_requested,1);return TRUE;}
int papacc_lab_stopped(void){return InterlockedCompareExchange(&stop_requested,0,0)!=0;}
static int actor(BYTE *admin,BYTE *service)
{
 union {TOKEN_USER aligned;BYTE data[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];} u;
 HANDLE token=NULL,thread=NULL;DWORD n=0,z=SECURITY_MAX_SID_SIZE;TOKEN_ELEVATION e;TOKEN_ELEVATION_TYPE t;TOKEN_STATISTICS stats;
 BYTE group[SECURITY_MAX_SID_SIZE];BOOL member=FALSE;LPSTR text=NULL;int ok=0;
 owner.thread=GetCurrentThreadId();
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&thread)){
  if(!CloseHandle(thread)){owner.retained_token=thread;owner.state=PAPACC_CA_KEY_RETAINED;}return 0;}
 if(GetLastError()!=ERROR_NO_TOKEN||!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return 0;
 if(!GetTokenInformation(token,TokenUser,&u,sizeof(u),&n)||!IsValidSid(u.aligned.User.Sid)||
  !GetTokenInformation(token,TokenElevation,&e,sizeof(e),&n)||!e.TokenIsElevated||
  !GetTokenInformation(token,TokenElevationType,&t,sizeof(t),&n)||t==TokenElevationTypeLimited||
  !GetTokenInformation(token,TokenStatistics,&stats,sizeof(stats),&n)||
  !CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,group,&z)||!CheckTokenMembership(NULL,group,&member)||!member||
  !ConvertSidToStringSidA(u.aligned.User.Sid,&text))goto done;
 printf("LAB_OPERATOR sid=%s elevated=%lu type=%u token=%ld:%lu auth=%ld:%lu modified=%ld:%lu\n",text,e.TokenIsElevated,(unsigned)t,
  stats.TokenId.HighPart,stats.TokenId.LowPart,stats.AuthenticationId.HighPart,stats.AuthenticationId.LowPart,stats.ModifiedId.HighPart,stats.ModifiedId.LowPart);
 if(strcmp(text,expected_sid)||!CopySid(SECURITY_MAX_SID_SIZE,admin,u.aligned.User.Sid))goto done;
 z=SECURITY_MAX_SID_SIZE;if(!CreateWellKnownSid(WinLocalServiceSid,NULL,service,&z))goto done;
 ok=1;
done:
 if(text&&LocalFree(text)){owner.retained_local[0]=text;owner.state=PAPACC_CA_KEY_RETAINED;ok=0;}
 if(!CloseHandle(token)){owner.retained_token=token;owner.state=PAPACC_CA_KEY_RETAINED;ok=0;}
 return ok;
}
static void report(SECURITY_STATUS status,const char *phase)
{printf("LAB_RESULT phase=%s status=%08lx state=%u created=%d finalized=%lu key_owned=%u provider_owned=%u close_confirmed=%d first_function=%lu first_stage=%lu first_api=%lu first_code=%08lx\n",
 phase,(unsigned long)status,(unsigned)owner.state,owner.created,owner.diagnostic_finalization,owner.key!=0,owner.provider!=0,owner.close_confirmed,
 owner.first_failure.function,owner.first_failure.stage,owner.first_failure.native_api,(unsigned long)owner.first_failure.original_code);}
int main(int argc,char **argv)
{
 BYTE admin[SECURITY_MAX_SID_SIZE],service[SECURITY_MAX_SID_SIZE];SECURITY_STATUS status;int borrowed=0;
 if(argc==2&&!strcmp(argv[1],"--describe")){
  puts("PREPARED_ONLY PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0 MACHINE RSA2048 NO_PRODUCT_AUTHORITY EXECUTION_REQUIRES_SEPARATE_APPROVAL");return 0;}
 if(argc==2&&!strcmp(argv[1],"--check-context")){
  DWORD mode=0;if(!GetConsoleWindow()||!GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),&mode)||!actor(admin,service)){puts("LAB_CONTEXT_DENY_NO_KEY_CALL");return 2;}
  puts("LAB_CONTEXT_ELIGIBLE_NO_KEY_CALL");return 0;}
 if(argc!=2||strcmp(argv[1],"--execute-new-lab")){puts("DENY_NO_APPROVED_LAB_OPERATION");return 2;}
 if(!GetConsoleWindow()||!SetConsoleCtrlHandler(on_control,TRUE)||!actor(admin,service)||papacc_lab_stopped()){report(NTE_PERM,"ACTOR_OR_STOP_DENY");return 2;}
 /* Same original backend sequence, no overwrite or parameter experimentation. */
 status=papacc_ca_key_create(&owner,authority,generation,admin,service);report(status,"CREATE_FINALIZE");
 if(status!=ERROR_SUCCESS)goto retain;
 if(papacc_lab_stopped()){status=NTE_USER_CANCELLED;goto retain;}
 if(wcscmp(owner.name,expected_name)||!owner.created||owner.state!=PAPACC_CA_KEY_READY||owner.diagnostic_finalization!=2U){status=NTE_BAD_KEY_STATE;goto retain;}
 status=papacc_ca_key_validate(&owner);if(status!=ERROR_SUCCESS)goto retain;
 if(papacc_lab_stopped()){status=NTE_USER_CANCELLED;goto retain;}
 status=papacc_ca_key_borrow(&owner,&lab_holder);if(status!=ERROR_SUCCESS)goto retain;borrowed=1;
 if(papacc_ca_key_close(&owner)!=NTE_BAD_KEY_STATE||!owner.key||!owner.provider){status=NTE_BAD_KEY_STATE;goto retain;}
 puts("LAB_EXPECTED_BORROW_CLOSE_DENIED");
 status=papacc_ca_key_return(&owner,&lab_holder);if(status!=ERROR_SUCCESS)goto retain;borrowed=0;
 if(papacc_lab_stopped()){status=NTE_USER_CANCELLED;goto retain;}
 /* No DeleteKey: successful finalization leaves a LAB-only persisted key. */
 status=papacc_ca_key_close(&owner);report(status,"FINAL_CLOSE");
 if(status!=ERROR_SUCCESS||!owner.close_confirmed||owner.key||owner.provider)goto retain;
 if(!SetConsoleCtrlHandler(on_control,FALSE))return 3;
 puts("LAB_PERSISTED_KEY=PRESENT_FINALIZED_LAB_ONLY; HANDLES_CLOSED; NO_DELETION");
 puts("LAB_CUSTODY_PASS_NO_CERT_NO_SIGNATURE_NO_CURRENT_NO_AUTHORITY");return 0;
retain:
 if(owner.key||owner.provider||owner.retained_token||owner.retained_local[0]||owner.retained_local[1]||owner.retained_local[2]){owner.state=PAPACC_CA_KEY_RETAINED;owner.error=status;}
 report(status,"STOP_UNKNOWN_NO_RETRY");printf("LAB_BORROW_RETAINED=%d; PERSISTENT_DISPOSITION_UNKNOWN; PROCESS_EXIT_IS_NOT_DESTRUCTION\n",borrowed);return 3;
}
