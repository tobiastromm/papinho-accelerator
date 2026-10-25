/* Synthetic API fixtures only. NEVER executes qualifier main, preflight or SCM. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <sddl.h>
#include "../apps/server/server_service_admission_win32.h"
#include "../tools/qualification/scm-instrumented-policy.h"
#define H(n) ((HANDLE)(ULONG_PTR)(n))
#define T(x) do{if(!(x)){printf("FAIL line=%d case=%u\n",__LINE__,cases);return 1;}}while(0)
static PS_BD_DESCRIPTOR bd_fixture;
static unsigned cases;
#ifndef PS_BD_SOURCE_ONLY
static unsigned mode,queries,closes,context_calls;
static ULONGLONG fixture_clock=100000;static DWORD context_delay;
static ULONGLONG WINAPI model_clock(void){return fixture_clock;}
static BOOL WINAPI denied_intent_write(HANDLE h,LPCVOID bytes,DWORD size,LPDWORD written,LPOVERLAPPED overlap){(void)h;(void)bytes;(void)size;(void)overlap;*written=0;SetLastError(ERROR_WRITE_FAULT);return FALSE;}static WCHAR confirmation[192];
#endif
static void descriptor(void)
{
 PSID sid=NULL;ZeroMemory(&bd_fixture,sizeof(bd_fixture));
 bd_fixture.actor.handle=H(0x10);bd_fixture.actor.pid=42424242;bd_fixture.actor.creation=55;bd_fixture.actor.session=1;
 bd_fixture.control_thread=H(0x20);bd_fixture.control_tid=123;bd_fixture.configuration=H(0x50);bd_fixture.image=H(0x60);
 bd_fixture.owner_pid=GetCurrentProcessId();bd_fixture.owner_creation=77;bd_fixture.boot=236441;bd_fixture.challenge=100;
 bd_fixture.statistics.TokenId.LowPart=1;bd_fixture.statistics.AuthenticationId.LowPart=2;bd_fixture.statistics.ModifiedId.LowPart=3;bd_fixture.statistics.TokenType=TokenPrimary;
 if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&sid))ExitProcess(3);
 bd_fixture.sid_size=GetLengthSid(sid);CopySid(sizeof(bd_fixture.sid),bd_fixture.sid,sid);LocalFree(sid);
 strcpy_s(bd_fixture.attempt,33,IG_ATTEMPT);memset(bd_fixture.plan,'a',64);memset(bd_fixture.config_hash,'b',64);memset(bd_fixture.image_hash,'c',64);memset(bd_fixture.policy,'d',64);memset(bd_fixture.host,'e',64);
 bd_fixture.config_id.dwVolumeSerialNumber=1;bd_fixture.config_id.nFileIndexLow=2;bd_fixture.config_id.nNumberOfLinks=1;bd_fixture.image_id=bd_fixture.config_id;bd_fixture.image_id.nFileIndexLow=3;
}
static PS_BD_INVENTORY inventory(const PS_BD_DESCRIPTOR *d)
{
 PS_BD_INVENTORY v;ZeroMemory(&v,sizeof(v));v.count=3;v.present[0]=v.present[1]=v.present[3]=1;
 v.role[0]=d->actor;v.role[1].handle=H(0x90);v.role[1].pid=d->owner_pid;v.role[1].creation=d->owner_creation;v.role[1].session=1;
 v.role[3].handle=H(0x80);v.role[3].pid=42424243;v.role[3].creation=56;v.role[3].session=1;
 v.console.actor=d->actor;v.console.console=v.role[3];v.console.known=PS_BD_PROOF_ALL;v.console.source=0x4d4f444cUL;v.console.boot=d->boot;v.console.generation=d->challenge;v.console.observed=100;return v;
}
#ifndef PS_BD_SOURCE_ONLY
static const WCHAR psa_name[]=L"UNEXECUTED_BD_MODEL",psa_path[]=L"UNEXECUTED_BD_MODEL",psa_launch_path[]=L"UNEXECUTED_BD_MODEL";
static BY_HANDLE_FILE_INFORMATION psa_identity;
static struct {PS_ADMISSION admission;} native;
static BOOL WINAPI m_duplicate(HANDLE a,HANDLE b,HANDLE c,LPHANDLE out,DWORD rights,BOOL inherit,DWORD options)
{(void)a;(void)c;(void)rights;(void)inherit;(void)options;*out=b==GetCurrentProcess()?H(0x91):b==H(0x10)?H(0x11):b==H(0x80)?H(0x81):b==H(0x82)?H(0x83):H(0x21);return TRUE;}
static BOOL WINAPI m_times(HANDLE h,LPFILETIME a,LPFILETIME b,LPFILETIME c,LPFILETIME d)
{ZeroMemory(a,sizeof(*a));ZeroMemory(b,sizeof(*b));ZeroMemory(c,sizeof(*c));ZeroMemory(d,sizeof(*d));a->dwLowDateTime=h==GetCurrentProcess()?77:h==H(0x81)?56:(mode==2?99:55);return TRUE;}
static DWORD WINAPI m_pid(HANDLE h){return h==H(0x81)?bd_fixture.auxiliary.instance.pid:bd_fixture.actor.pid;}
static DWORD WINAPI m_tid(HANDLE h){(void)h;return bd_fixture.control_tid;}
static BOOL WINAPI m_session(DWORD pid,DWORD *s){(void)pid;*s=mode==7?2:1;return TRUE;}
static DWORD WINAPI m_wait(HANDLE h,DWORD timeout){(void)h;(void)timeout;return mode==8?WAIT_OBJECT_0:WAIT_TIMEOUT;}
static BOOL WINAPI m_image(HANDLE h,DWORD flags,LPWSTR text,PDWORD n)
{(void)flags;if(h==H(0x81)){wcscpy_s(text,*n,L"C:\\Windows\\System32\\conhost.exe");return TRUE;}wcscpy_s(text,*n,mode==9?L"C:\\unrelated.exe":L"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe");return TRUE;}
static BOOL WINAPI m_thread_token(HANDLE h,DWORD rights,BOOL self,PHANDLE token)
{(void)h;(void)rights;(void)self;if(mode==4){*token=H(0x31);return TRUE;}SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI m_process_token(HANDLE h,DWORD rights,PHANDLE token){(void)h;(void)rights;*token=H(0x30);return TRUE;}
static BOOL WINAPI m_token(HANDLE h,TOKEN_INFORMATION_CLASS kind,LPVOID out,DWORD cap,PDWORD used)
{
 (void)h;++queries;
 if(kind==TokenSessionId){if(cap<sizeof(DWORD))return FALSE;*(DWORD *)out=mode==23?2:1;*used=sizeof(DWORD);return TRUE;}
 if(kind==TokenUser){TOKEN_USER *u=(TOKEN_USER *)out;DWORD n=(DWORD)sizeof(*u)+bd_fixture.sid_size;if(cap<n)return FALSE;*used=n;u->User.Sid=(BYTE *)out+sizeof(*u);CopySid(bd_fixture.sid_size,u->User.Sid,bd_fixture.sid);return TRUE;}
 if(kind==TokenElevation){if(cap<sizeof(TOKEN_ELEVATION))return FALSE;((TOKEN_ELEVATION *)out)->TokenIsElevated=mode!=5;*used=sizeof(TOKEN_ELEVATION);return TRUE;}
 if(kind==TokenStatistics){TOKEN_STATISTICS *s=(TOKEN_STATISTICS *)out;if(cap<sizeof(*s))return FALSE;*s=bd_fixture.statistics;if(mode==1)s->ModifiedId.LowPart++;*used=sizeof(*s);return TRUE;}return FALSE;
}
static BOOL WINAPI m_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION id)
{*id=h==H(0x50)?bd_fixture.config_id:h==H(0x83)?bd_fixture.auxiliary.file_id:bd_fixture.image_id;if(mode==6||mode==22)id->nFileIndexLow++;if(mode==25)id->nNumberOfLinks++;return TRUE;}
static HANDLE WINAPI m_create(LPCWSTR path,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES sa,DWORD disp,DWORD flags,HANDLE template_file)
{(void)access;(void)share;(void)sa;(void)disp;(void)flags;(void)template_file;if(mode==10){SetLastError(ERROR_INVALID_HANDLE);return INVALID_HANDLE_VALUE;}return !wcscmp(path,L"CONIN$")?H(0x70):H(0x71);}
static BOOL WINAPI m_console(HANDLE h,LPDWORD bits){(void)h;*bits=ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT;return TRUE;}
static BOOL WINAPI m_write(HANDLE h,const void *text,DWORD n,LPDWORD used,LPVOID reserved)
{const WCHAR *at=wcsstr((const WCHAR *)text,L"BIND_BD_READ_ONLY"),*end;(void)h;(void)reserved;if(!at){if(wcsstr((const WCHAR *)text,L"IDENTIFIED_AUXILIARY_CANDIDATE")){*used=n;return TRUE;}return FALSE;}end=wcschr(at,'\r');if(!end||end-at>=190)return FALSE;wcsncpy_s(confirmation,192,at,(size_t)(end-at));*used=n;return TRUE;}
static BOOL WINAPI m_read(HANDLE h,LPVOID text,DWORD n,LPDWORD used,PCONSOLE_READCONSOLE_CONTROL control)
{(void)h;(void)control;if(mode==3)wcscpy_s((WCHAR *)text,n,L"WRONG CONFIRMATION");else wcscpy_s((WCHAR *)text,n,confirmation);*used=(DWORD)wcslen((WCHAR *)text);if(mode==11)mode=1;return TRUE;}
static BOOL WINAPI m_close(HANDLE h){++closes;if(mode==12&&h==H(0x21)){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}
static BOOL WINAPI m_release(HANDLE h){(void)h;return TRUE;}
static BOOL WINAPI m_info(HANDLE h,LPDWORD bits){(void)h;*bits=mode==21?HANDLE_FLAG_INHERIT:0;return TRUE;}
static DWORD WINAPI m_final(HANDLE h,LPWSTR out,DWORD n,DWORD flags){(void)h;(void)flags;wcscpy_s(out,n,L"\\\\?\\C:\\Windows\\System32\\conhost.exe");return (DWORD)wcslen(out);}
static DWORD WINAPI m_clients(LPDWORD ids,DWORD cap){if(cap<2)return 3;ids[0]=bd_fixture.actor.pid;ids[1]=bd_fixture.owner_pid;return mode==24?3:2;}
static DWORD context(const PS_BD_DESCRIPTOR *d)
{++context_calls;fixture_clock+=context_delay;if(mode==20)return ERROR_ACCESS_DENIED;return (d->n0&&(strcmp(d->auxiliary.file_hash,bd_fixture.auxiliary.file_hash)||strcmp(d->auxiliary.security_hash,bd_fixture.auxiliary.security_hash)||strcmp(d->auxiliary.provenance_hash,bd_fixture.auxiliary.provenance_hash)))||strcmp(d->plan,bd_fixture.plan)||strcmp(d->config_hash,bd_fixture.config_hash)||strcmp(d->image_hash,bd_fixture.image_hash)||strcmp(d->policy,bd_fixture.policy)||strcmp(d->host,bd_fixture.host)||d->boot!=bd_fixture.boot?ERROR_INVALID_DATA:0;}
#define GetHandleInformation m_info
#define GetFinalPathNameByHandleW m_final
#define GetConsoleProcessList m_clients
#define DuplicateHandle m_duplicate
#define GetProcessTimes m_times
#define GetProcessId m_pid
#define GetProcessIdOfThread m_pid
#define GetThreadId m_tid
#define ProcessIdToSessionId m_session
#define WaitForSingleObject m_wait
#define QueryFullProcessImageNameW m_image
#define OpenThreadToken m_thread_token
#define OpenProcessToken m_process_token
#define GetTokenInformation m_token
#define GetFileInformationByHandle m_file
#define CreateFileW m_create
#define GetConsoleMode m_console
#define WriteConsoleW m_write
#define ReadConsoleW m_read
#define CloseHandle m_close
#define ReleaseMutex m_release
#define GetTickCount64 model_clock
#define WriteFile denied_intent_write
#define PS_ADMISSION_TEST 1
#include "../apps/server/server_service_admission_win32.inc"
static void owner(void)
{ZeroMemory(&native,sizeof(native));native.admission.phase=1;native.admission.owned=1;native.admission.thread=GetCurrentThreadId();native.admission.mutex=H(0x40);mode=queries=closes=context_calls=context_delay=0;fixture_clock=100000;}
#endif
static void n0_descriptor(void)
{
 descriptor();bd_fixture.n0=1;strcpy_s(bd_fixture.policy,65,PS_BD_N0_POLICY);
 strcpy_s(bd_fixture.config_hash,65,"a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44");
 strcpy_s(bd_fixture.image_hash,65,"eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b");
 strcpy_s(bd_fixture.host,65,"2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff");
 bd_fixture.auxiliary.instance.handle=H(0x80);bd_fixture.auxiliary.instance.pid=42424243;bd_fixture.auxiliary.instance.creation=56;bd_fixture.auxiliary.instance.session=1;
 bd_fixture.auxiliary.parent=bd_fixture.actor.pid;bd_fixture.auxiliary.file=H(0x82);bd_fixture.auxiliary.file_id=bd_fixture.image_id;
 bd_fixture.auxiliary.file_id.nFileIndexLow=4;memset(bd_fixture.auxiliary.file_hash,'1',64);memset(bd_fixture.auxiliary.security_hash,'2',64);memset(bd_fixture.auxiliary.provenance_hash,'3',64);
}
#ifdef PS_BD_SOURCE_ONLY
static DWORD forged_native_source(const PS_BD_DESCRIPTOR *d,ULONGLONG now){(void)d;(void)now;return 0;}
#endif
int main(void)
{
 PS_BD_INVENTORY v;
#ifndef PS_BD_SOURCE_ONLY
 PS_BD_COST c[4];unsigned i;IG_SAMPLE sample;DWORD e;
#endif
 (void)ig_metric;(void)ig_preflight;(void)ig_final;(void)ig_consume;(void)ig_start;(void)ig_monitor;(void)ig_stop;(void)ig_retire;
 descriptor();v=inventory(&bd_fixture);
#ifdef PS_BD_SOURCE_ONLY
 T(ps_bd_inventory(&bd_fixture,&v,100)==ERROR_NOT_SUPPORTED);++cases;
 v.console.source=1;T(ps_bd_inventory(&bd_fixture,&v,100)==ERROR_NOT_SUPPORTED);++cases;
 v.console.known=0;T(ps_bd_inventory(&bd_fixture,&v,100)==ERROR_ACCESS_DENIED);++cases;
 n0_descriptor();v=inventory(&bd_fixture);v.console.known=PS_BD_N0_KNOWN;v.console.source=PS_BD_N0_SOURCE;v.console.validate=forged_native_source;
 T(ps_bd_inventory(&bd_fixture,&v,100)==ERROR_NOT_SUPPORTED);++cases;
 v.console.source=0x4d4f444cUL;T(ps_bd_inventory(&bd_fixture,&v,100)==ERROR_NOT_SUPPORTED);++cases;
 v.console.known|=PS_BD_FACTUAL_RELATION;T(ps_bd_inventory(&bd_fixture,&v,100)==ERROR_ACCESS_DENIED);++cases;
 printf("BD_NATIVE_SOURCE_GATE=DENY cases=%u; NO_NATIVE_CONSOLE_PROVIDER_NO_PREFLIGHT_NO_HANDLE_MEASUREMENT\n",cases);return 0;
#else
 T(!ps_bd_inventory(&bd_fixture,&v,100));++cases;
 for(i=1;i<=128;i<<=1){PS_BD_INVENTORY x=v;x.console.known^=i;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;}
 {PS_BD_INVENTORY x=v;x.console.console.creation++;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;x=v;x.console.actor.pid++;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;x=v;x.console.console.session++;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;x=v;x.console.source=0;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;x=v;x.count=5;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;x=v;x.role[3].pid=x.role[0].pid;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;x=v;x.present[2]=1;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;}
 {PS_BD_INVENTORY x=v;x.count=4;x.present[PS_BD_SERVICE]=1;x.role[PS_BD_SERVICE].handle=H(0x88);x.role[PS_BD_SERVICE].pid=42424244;x.role[PS_BD_SERVICE].creation=57;x.role[PS_BD_SERVICE].session=1;T(!ps_bd_inventory(&bd_fixture,&x,100));T(ps_bd_census(&bd_fixture,&x,100,9999,42424244,0)==ERROR_BUSY);++cases;}
 T(ps_bd_inventory(&bd_fixture,&v,60101)!=0);++cases;
 T(ps_bd_census(&bd_fixture,&v,100,9999,bd_fixture.actor.pid,0)==ERROR_BUSY);++cases;
 T(ps_bd_census(&bd_fixture,&v,100,9999,0,1)==ERROR_BUSY);++cases;
 T(!ps_bd_census(&bd_fixture,&v,100,v.role[3].pid,0,0));++cases;
 ZeroMemory(c,sizeof(c));for(i=0;i<4;i++){c[i].private_bytes=4*PS_BD_MIB;c[i].commit=4*PS_BD_MIB;c[i].working_set=8*PS_BD_MIB;c[i].handles=64;c[i].threads=4;c[i].stack=128*1024;}
 T(!ps_bd_costs(&v,c));++cases;c[0].handles=900;c[3].handles=125;T(ps_bd_costs(&v,c)!=0);++cases;c[0].handles=c[3].handles=64;c[1].handles=129;T(ps_bd_costs(&v,c)!=0);++cases;c[1].handles=64;
 c[0].commit=200*PS_BD_MIB;c[3].commit=57*PS_BD_MIB;T(ps_bd_costs(&v,c)!=0);++cases;c[0].commit=c[3].commit=4*PS_BD_MIB;
 {PS_BD_COST saved=c[0],other=c[3];
 c[0].private_bytes=250*PS_BD_MIB;c[3].private_bytes=7*PS_BD_MIB;T(ps_bd_costs(&v,c)!=0);++cases;c[0]=saved;c[3]=other;
 c[0].working_set=250*PS_BD_MIB;c[3].working_set=7*PS_BD_MIB;T(ps_bd_costs(&v,c)!=0);++cases;c[0]=saved;c[3]=other;
 c[0].stack=30*PS_BD_MIB;c[3].stack=3*PS_BD_MIB;T(ps_bd_costs(&v,c)!=0);++cases;c[0]=saved;c[3]=other;
 c[0].threads=60;c[3].threads=5;T(ps_bd_costs(&v,c)!=0);++cases;c[0]=saved;c[3]=other;
 c[3].error=ERROR_NOT_SUPPORTED;T(ps_bd_costs(&v,c)==ERROR_NOT_SUPPORTED);++cases;c[3]=other;
 }
 ZeroMemory(&sample,sizeof(sample));sample.bd=1;sample.binding=bd_fixture;sample.inventory=v;sample.count=v.count;sample.identity=IG_ID_ALL;sample.oldest=100;sample.physical=5*IG_GIB;sample.commit=9*IG_GIB;
 for(i=0;i<4;i++){sample.process[i].private_bytes=c[i].private_bytes;sample.process[i].commit=c[i].commit;sample.process[i].working_set=c[i].working_set;sample.process[i].handles=c[i].handles;sample.process[i].threads=c[i].threads;sample.process[i].stack=c[i].stack;}
 T(!ig_metric(&sample,1));++cases;sample.process[1].handles=129;T(ig_metric(&sample,1)!=0);++cases;sample.process[1].handles=64;sample.inventory.console.known=0;T(ig_metric(&sample,1)!=0);++cases;
 owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(native.admission.bd.bound&&native.admission.launcher&&native.admission.token);T(!papacc_server_service_admission_bd_revalidate(context));T(papacc_server_service_admission_consume()==ERROR_INVALID_STATE);T(papacc_server_service_admission_bd_bind(&bd_fixture,context)==ERROR_INVALID_STATE);T(!papacc_server_service_admission_finish(1));T(!native.admission.launcher&&!native.admission.token&&!native.admission.bd.thread&&!native.admission.bd.binding.actor.handle&&!native.admission.bd.inventory.role[0].handle);++cases;
 for(i=1;i<=11;i++){owner();mode=i;e=papacc_server_service_admission_bd_bind(&bd_fixture,context);T(e!=0&&!native.admission.bd.bound);T(papacc_server_service_admission_bd_bind(&bd_fixture,context)==ERROR_INVALID_STATE);mode=0;T(!papacc_server_service_admission_finish(1));++cases;}
 owner();{PS_BD_DESCRIPTOR bad=bd_fixture;bad.plan[0]='f';T(papacc_server_service_admission_bd_bind(&bad,context)!=0);T(!papacc_server_service_admission_finish(1));}++cases;
 owner();{PS_BD_DESCRIPTOR bad=bd_fixture;bad.sid[bad.sid_size-1]^=1;T(papacc_server_service_admission_bd_bind(&bad,context)==ERROR_INVALID_OWNER);T(!papacc_server_service_admission_finish(1));}++cases;
 owner();T(papacc_server_service_admission_bd_bind(&bd_fixture,NULL)==ERROR_NOT_SUPPORTED);T(!papacc_server_service_admission_finish(1));++cases;
 owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));
 native.admission.consumed=1;native.admission.intent=H(0x99);native.admission.bd.inventory=inventory(&native.admission.bd.binding);
 native.admission.bd.inventory.console.observed=GetTickCount64();T(!papacc_server_service_admission_effect());
 native.admission.bd.inventory.console.known=0;T(papacc_server_service_admission_effect()!=0);T(!papacc_server_service_admission_finish(1));++cases;
 owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(!papacc_server_service_admission_stop());T(papacc_server_service_admission_bd_revalidate(context)!=0);T(!papacc_server_service_admission_finish(1));++cases;
 owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(papacc_server_service_admission_finish(0)==ERROR_INVALID_STATE&&native.admission.phase==3&&native.admission.launcher&&native.admission.bd.thread);T(papacc_server_service_admission_finish(1)==ERROR_INVALID_STATE);T(papacc_server_service_admission_bd_close(1)==ERROR_INVALID_STATE);/* fake handles only: fixture release below is not a native cleanup receipt */native.admission.phase=1;mode=0;T(!papacc_server_service_admission_finish(1));++cases;
 owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));mode=12;T(papacc_server_service_admission_finish(1)==ERROR_INVALID_HANDLE&&native.admission.phase==3&&native.admission.bd.thread);T(papacc_server_service_admission_finish(1)==ERROR_INVALID_STATE);native.admission.phase=1;mode=0;T(!papacc_server_service_admission_finish(1));++cases;
 n0_descriptor();v=inventory(&bd_fixture);v.console.known=PS_BD_N0_KNOWN;
 T(!ps_bd_inventory(&bd_fixture,&v,100));++cases;
 {PS_BD_DESCRIPTOR bad=bd_fixture;bad.policy[0]='0';T(ps_bd_descriptor(&bad)!=0);++cases;bad=bd_fixture;bad.auxiliary.provenance_hash[0]='z';T(ps_bd_descriptor(&bad)!=0);++cases;bad=bd_fixture;bad.auxiliary.parent++;T(ps_bd_descriptor(&bad)!=0);++cases;bad=bd_fixture;bad.auxiliary.instance.creation=bad.actor.creation;T(ps_bd_descriptor(&bad)!=0);++cases;bad=bd_fixture;bad.boot++;T(ps_bd_descriptor(&bad)!=0);++cases;}
 {PS_BD_INVENTORY x=v;x.present[2]=1;x.count=4;x.role[2].handle=H(0x88);x.role[2].pid=42424244;x.role[2].creation=58;x.role[2].session=0;T(!ps_bd_inventory(&bd_fixture,&x,100));++cases;x.role[2].session=1;T(ps_bd_inventory(&bd_fixture,&x,100)!=0);++cases;}
 T(ps_bd_census(&bd_fixture,&v,100,v.role[3].pid,0,0)!=0);++cases;
 T(!ps_bd_census(&bd_fixture,&v,100,v.role[3].pid,bd_fixture.actor.pid,0));++cases;
 owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(native.admission.bd.aux_bound&&native.admission.bd.auxiliary&&native.admission.bd.file&&native.admission.bd.aux_token);
 T(native.admission.bd.inventory.count==3&&native.admission.bd.inventory.present[3]);T(!papacc_server_service_admission_bd_revalidate(context));
 T(!ps_bd_inventory(&native.admission.bd.binding,&native.admission.bd.inventory,GetTickCount64()));
 native.admission.consumed=1;native.admission.intent=H(0x99);T(papacc_server_service_admission_effect()==ERROR_TIMEOUT);
 ZeroMemory(c,sizeof(c));for(i=0;i<4;i++){c[i].private_bytes=c[i].commit=4*PS_BD_MIB;c[i].working_set=8*PS_BD_MIB;c[i].handles=64;c[i].threads=4;c[i].stack=128*1024;}
 T(!papacc_server_service_admission_bd_metrics(c,GetTickCount64()));T(!papacc_server_service_admission_effect());++cases;
 c[1].handles=129;T(papacc_server_service_admission_bd_metrics(c,GetTickCount64())==ERROR_NOT_ENOUGH_QUOTA);T(papacc_server_service_admission_effect()==ERROR_TIMEOUT);c[1].handles=64;
 T(!papacc_server_service_admission_finish(1));T(!native.admission.bd.auxiliary&&!native.admission.bd.file&&!native.admission.bd.aux_token);++cases;
 for(i=20;i<=25;i++){owner();mode=i;T(papacc_server_service_admission_bd_bind(&bd_fixture,context)!=0);mode=0;T(!papacc_server_service_admission_finish(1));++cases;}
 owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(papacc_server_service_admission_finish(0)==ERROR_INVALID_STATE);T(native.admission.bd.auxiliary&&native.admission.bd.file&&native.admission.bd.aux_token);T(papacc_server_service_admission_finish(1)==ERROR_INVALID_STATE);native.admission.phase=1;T(!papacc_server_service_admission_finish(1));++cases;
 n0_descriptor();bd_fixture.auxiliary.file_id.nNumberOfLinks=2;T(!ps_bd_descriptor(&bd_fixture));owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(!papacc_server_service_admission_bd_revalidate(context));mode=25;T(papacc_server_service_admission_bd_revalidate(context)!=0);mode=0;T(!papacc_server_service_admission_finish(1));++cases;
 owner();native.admission.bd.context.dossier=H(0x999);T(papacc_server_service_admission_finish(1)==ERROR_INVALID_STATE&&native.admission.phase==3&&native.admission.mutex&&native.admission.bd.context.dossier);T(papacc_server_service_admission_finish(1)==ERROR_INVALID_STATE);/* reset synthetic handles ONLY */native.admission.bd.context.dossier=NULL;native.admission.phase=1;T(!papacc_server_service_admission_finish(1));++cases;
 owner();T(papacc_server_service_admission_n0_arm()==ERROR_INVALID_STATE);++cases;
 n0_descriptor();owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(papacc_server_service_admission_n0_arm()==ERROR_TIMEOUT);++cases;
 ZeroMemory(c,sizeof(c));for(i=0;i<4;i++){c[i].private_bytes=c[i].commit=4*PS_BD_MIB;c[i].working_set=8*PS_BD_MIB;c[i].handles=64;c[i].threads=4;c[i].stack=128*1024;}
 T(!papacc_server_service_admission_bd_metrics(c,GetTickCount64()));T(!papacc_server_service_admission_n0_arm()&&native.admission.lab);T(papacc_server_service_admission_n0_arm()==ERROR_INVALID_STATE);++cases;
 native.admission.consumed=1;native.admission.intent=H(0x99);T(!papacc_server_service_admission_effect());T(!papacc_server_service_admission_stop());T(!papacc_server_service_admission_bd_revalidate(context));T(papacc_server_service_admission_effect()==ERROR_INVALID_STATE);T(!papacc_server_service_admission_finish(1));++cases;
 n0_descriptor();owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(!papacc_server_service_admission_stop());T(papacc_server_service_admission_n0_arm()==ERROR_INVALID_STATE);T(!papacc_server_service_admission_finish(1));++cases;
 /* Slow COMPLETE protected validation happens BEFORE this new sample. */
 n0_descriptor();owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));context_delay=700;{ULONGLONG before=fixture_clock;T(!papacc_server_service_admission_bd_revalidate(context)&&fixture_clock>before+1000);}context_delay=0;
 ZeroMemory(c,sizeof(c));for(i=0;i<4;i++){c[i].private_bytes=c[i].commit=4*PS_BD_MIB;c[i].working_set=8*PS_BD_MIB;c[i].handles=64;c[i].threads=4;c[i].stack=128*1024;}
 T(!papacc_server_service_admission_bd_metrics(c,fixture_clock));fixture_clock+=1000;T(!papacc_server_service_admission_n0_arm());T(!papacc_server_service_admission_finish(1));++cases;
 n0_descriptor();owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(!papacc_server_service_admission_bd_metrics(c,fixture_clock));fixture_clock+=1001;T(papacc_server_service_admission_n0_arm()==ERROR_TIMEOUT&&!native.admission.consumed);T(!papacc_server_service_admission_finish(1));++cases;
 n0_descriptor();owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(!papacc_server_service_admission_bd_metrics(c,fixture_clock));mode=1;T(papacc_server_service_admission_n0_arm()!=0&&!native.admission.consumed);mode=0;T(!papacc_server_service_admission_finish(1));++cases;
 n0_descriptor();owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(!papacc_server_service_admission_bd_metrics(c,fixture_clock));mode=6;T(papacc_server_service_admission_n0_arm()!=0&&!native.admission.consumed);mode=0;T(!papacc_server_service_admission_finish(1));++cases;
 n0_descriptor();owner();T(!papacc_server_service_admission_bd_bind(&bd_fixture,context));T(!papacc_server_service_admission_bd_metrics(c,fixture_clock));T(!papacc_server_service_admission_n0_arm());T(papacc_server_service_admission_consume()==ERROR_WRITE_FAULT&&native.admission.consumed&&native.admission.phase==3);T(papacc_server_service_admission_consume()==ERROR_INVALID_STATE);T(papacc_server_service_admission_finish(1)==ERROR_INVALID_STATE);/* discard/reset FAKE model refs only, never native cleanup */owner();++cases;
 printf("BD_CORE_MODEL=PASS cases=%u; SYNTHETIC_PROOF_ONLY; OPERATOR_TOKEN_CREATION_CONFIRM_REPLAY_ROLES_ADMIN_BUDGET_129_STOP_RETENTION_UNKNOWN; NO_PREFLIGHT_NO_SCM_NO_START\n",cases);
 printf("BD_STORAGE descriptor=%llu inventory=%llu state=%llu admission=%llu sample=%llu; FIXED_STORAGE_NO_ADDED_THREAD\n",(ULONGLONG)sizeof(PS_BD_DESCRIPTOR),(ULONGLONG)sizeof(PS_BD_INVENTORY),(ULONGLONG)sizeof(PS_BD_STATE),(ULONGLONG)sizeof(PS_ADMISSION),(ULONGLONG)sizeof(IG_SAMPLE));return 0;
#endif
}

