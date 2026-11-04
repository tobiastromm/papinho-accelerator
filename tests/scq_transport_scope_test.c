#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
static PSID admin_sid,virtual_sid;
static unsigned mode,closes,token_queries;
static BOOL WINAPI scope_thread(HANDLE a,DWORD b,BOOL c,PHANDLE out)
{(void)a;(void)b;(void)c;(void)out;SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI scope_open(HANDLE process,DWORD rights,PHANDLE out)
{(void)rights;*out=process==GetCurrentProcess()?(HANDLE)1:(HANDLE)2;return TRUE;}
static BOOL WINAPI scope_info(HANDLE token,TOKEN_INFORMATION_CLASS kind,LPVOID out,DWORD capacity,PDWORD bytes)
{
 int peer=token==(HANDLE)2;token_queries++;
 if(kind==TokenUser){TOKEN_USER *u=out;PSID sid=peer&&mode==1?virtual_sid:admin_sid;DWORD n=GetLengthSid(sid);
  if(capacity<sizeof(*u)+n)return FALSE;u->User.Sid=(BYTE *)out+sizeof(*u);CopySid(n,u->User.Sid,sid);u->User.Attributes=0;*bytes=(DWORD)sizeof(*u)+n;return TRUE;}
 if(kind==TokenStatistics){TOKEN_STATISTICS *s=out;memset(s,0,sizeof(*s));s->TokenType=TokenPrimary;s->AuthenticationId.LowPart=peer&&mode==2?2:1;*bytes=sizeof(*s);return TRUE;}
 if(kind==TokenSessionId){*(DWORD *)out=peer&&mode==3?0:1;*bytes=sizeof(DWORD);return TRUE;}return FALSE;
}
static BOOL WINAPI scope_close(HANDLE h){(void)h;closes++;return TRUE;}
#define OpenThreadToken scope_thread
#define OpenProcessToken scope_open
#define GetTokenInformation scope_info
#define CloseHandle scope_close
/* Exercise the EXISTING actual transport predicate; never call Open/Launch/
 * Connect/Read/Write. No IPC endpoint, process or authority is created. */
#include "../tools/qualification/scm-n0-private-stdout-native.c"
#undef CloseHandle
int main(void)
{
 unsigned i;
 if(!ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&admin_sid)||
 !ConvertStringSidToSidA("S-1-5-80-1-2-3-4-5",&virtual_sid))return 1;
 for(i=0;i<4;i++){
  mode=i;closes=token_queries=0;
  if(same_token((HANDLE)44)!=(i==0)||closes!=2||token_queries!=6||t.pipe||t.used||t.token[0]||t.token[1])return 2;
 }
 if(LocalFree(admin_sid)||LocalFree(virtual_sid))return 3;
 puts("PASS SCQ_TRANSPORT_SCOPE cases=4 EXISTING_SAME_TOKEN_PREDICATE REAL_SID_HELPERS MODELED_TOKEN_APIS PIPE_CALLS=0 NO_AUTHORITY");return 0;
}
