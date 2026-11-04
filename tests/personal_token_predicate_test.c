#include "server_configuration_commit_win32.h"
#include <sddl.h>
#include <stdio.h>
#include <string.h>
static unsigned fault,checks;static PSID fixture_sid;static PAPACC_CONFIG_PENDING packet;
static BOOL WINAPI thread_token(HANDLE thread,DWORD rights,BOOL self,PHANDLE out)
{(void)thread;(void)rights;(void)self;if(fault==2){*out=(HANDLE)3;return TRUE;}SetLastError(fault==24?ERROR_ACCESS_DENIED:ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI membership(HANDLE token,PSID group,PBOOL yes)
{(void)token;if(fault==25){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(IsWellKnownSid(group,WinBuiltinAdministratorsSid))*yes=fault!=3;
 else if(IsWellKnownSid(group,WinInteractiveSid))*yes=fault!=4;
 else *yes=fault==5;return TRUE;}
static BOOL WINAPI process_token(HANDLE process,DWORD rights,PHANDLE out)
{(void)process;(void)rights;if(fault==6||fault==20){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}*out=packet.bootstrap_token?(HANDLE)2:(HANDLE)1;return TRUE;}
static BOOL WINAPI information(HANDLE token,TOKEN_INFORMATION_CLASS kind,LPVOID out,DWORD cap,PDWORD used)
{(void)cap;*used=0;
 if((fault==7&&kind==TokenStatistics)||(fault==9&&kind==TokenElevation)||(fault==11&&kind==TokenElevationType)||(fault==13&&kind==TokenSessionId)||(fault==17&&kind==TokenUser)) {SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(kind==TokenStatistics){TOKEN_STATISTICS *t=out;memset(t,0,sizeof(*t));t->TokenType=fault==8?TokenImpersonation:TokenPrimary;t->TokenId.LowPart=(fault==22&&token==(HANDLE)2)?2:1;t->ModifiedId.LowPart=3;}
 if(kind==TokenElevation)((TOKEN_ELEVATION *)out)->TokenIsElevated=fault!=10;
 if(kind==TokenElevationType)*(TOKEN_ELEVATION_TYPE *)out=fault==12?TokenElevationTypeLimited:TokenElevationTypeFull;
 if(kind==TokenSessionId)*(DWORD *)out=fault==14?0:1;
 if(kind==TokenUser)((TOKEN_USER *)out)->User.Sid=fault==18?NULL:fixture_sid;
 return TRUE;}
static BOOL WINAPI query_session(DWORD process,DWORD *out){(void)process;*out=1;if(fault==15){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}
static DWORD WINAPI active(void){return fault==16?2:1;}
static BOOL WINAPI close_reference(HANDLE handle){(void)handle;if(fault==21){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}
#define OpenThreadToken thread_token
#define CheckTokenMembership membership
#define OpenProcessToken process_token
#define GetTokenInformation information
#define ProcessIdToSessionId query_session
#define WTSGetActiveConsoleSessionId active
#define CloseHandle close_reference
#include "../apps/server/server_personal_token_checks_win32.inc"
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL fault=%u line=%u stage=%lu kind=%lu\n",fault,(unsigned)__LINE__,packet.personal_native.diagnostic_first,packet.personal_native.diagnostic_kind);return 1;}}while(0)
int main(void)
{unsigned i;DWORD first;static const unsigned cases[]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,24,25};
 CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&fixture_sid));
 for(i=0;i<sizeof(cases)/sizeof(cases[0]);i++){
  fault=cases[i];memset(&packet,0,sizeof(packet));packet.personal_native.phase=1;
  CHECK(!personal_token(&packet,1));first=packet.personal_native.diagnostic_first;
  CHECK(first==(fault==24?2:fault==25?3:fault));
  if(fault==24||fault==25||fault==6||fault==7||fault==9||fault==11||fault==13||fault==15||fault==17)CHECK(packet.personal_native.diagnostic_kind==1&&packet.personal_native.diagnostic_error==ERROR_ACCESS_DENIED);
  else CHECK(packet.personal_native.diagnostic_kind==2&&!packet.personal_native.diagnostic_error);
  fault=0;(void)personal_deny(&packet,99,99,1);CHECK(packet.personal_native.diagnostic_first==first);
 }
 for(i=20;i<=23;i++){
  fault=0;memset(&packet,0,sizeof(packet));packet.personal_native.phase=1;CHECK(personal_token(&packet,1));
  fault=i;if(fault==23)packet.personal_native.token.ModifiedId.LowPart++;
  CHECK(!personal_token(&packet,0)&&packet.personal_native.diagnostic_first==i);
  if(i==21)CHECK(packet.personal_native.phase==2&&packet.personal_native.check_token);
 }
 fault=0;memset(&packet,0,sizeof(packet));packet.personal_native.phase=1;CHECK(personal_token(&packet,1)&&personal_token(&packet,0)&&!packet.personal_native.diagnostic_first);
 CHECK(!LocalFree(fixture_sid));printf("PASS TOKEN_PREDICATE_FAULTS checks=%u REAL_SID_HELPERS TEST_ONLY_TOKEN_APIS SAME_GUARDS NO_AUTHORITY\n",checks);return 0;
}
