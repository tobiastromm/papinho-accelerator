/* Actual caller bodies; all acquisition APIs below are simulated. Never run entry. */
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
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string.h>
#include "../apps/server/server_console_win32.h"
static unsigned stop_flag,opens,closes,fail_open,fail_close,checks,reenter,busy_seen;
static WCHAR reply_text[128];
static unsigned live_mode,module_index,thread_token_present;
static BYTE fake_sid[12]={1,1,0,0,0,0,0,5,21,0,0,0};
static LPCWSTR fx_live_path(void);
static PAPACC_BOOL fx_stop(void){return stop_flag?PAPACC_TRUE:PAPACC_FALSE;}
static void fx_request(void){stop_flag=1;}
static void fx_check(void);
static void fx_reenter(void);
static HANDLE fx_handle(void){++opens;fx_check();if(fail_open==opens){SetLastError(ERROR_ACCESS_DENIED);return NULL;}return (HANDLE)(ULONG_PTR)(0xa000U+opens);}
static BOOL WINAPI fx_thread(HANDLE h,DWORD a,BOOL s,PHANDLE o){(void)h;(void)a;(void)s;fx_check();if(thread_token_present){*o=fx_handle();return *o!=NULL;}*o=NULL;SetLastError(ERROR_NO_TOKEN);return FALSE;}
static BOOL WINAPI fx_token(HANDLE h,DWORD a,PHANDLE o){(void)h;(void)a;*o=fx_handle();return *o!=NULL;}
static HANDLE WINAPI fx_file(LPCWSTR p,DWORD a,DWORD s,LPSECURITY_ATTRIBUTES sa,DWORD d,DWORD f,HANDLE t){HANDLE h;(void)p;(void)a;(void)s;(void)sa;(void)d;(void)f;(void)t;h=fx_handle();return h?h:INVALID_HANDLE_VALUE;}
static BOOL WINAPI fx_close(HANDLE h){(void)h;++closes;if(closes==fail_close){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}
static BOOL WINAPI fx_info(HANDLE h,TOKEN_INFORMATION_CLASS c,LPVOID p,DWORD n,PDWORD used){(void)h;fx_check();if(c==TokenStatistics&&n>=sizeof(TOKEN_STATISTICS)){ZeroMemory(p,n);((TOKEN_STATISTICS *)p)->TokenType=TokenPrimary;*used=sizeof(TOKEN_STATISTICS);return TRUE;}if(live_mode&&c==TokenUser&&n>=sizeof(TOKEN_USER)+sizeof(fake_sid)){TOKEN_USER *u=(TOKEN_USER *)p;ZeroMemory(p,n);u->User.Sid=(BYTE *)p+sizeof(TOKEN_USER);memcpy(u->User.Sid,fake_sid,sizeof(fake_sid));*used=sizeof(TOKEN_USER)+sizeof(fake_sid);return TRUE;}
 if(live_mode&&c==TokenGroups){ZeroMemory(p,n);*used=FIELD_OFFSET(TOKEN_GROUPS,Groups);return TRUE;}
 if(live_mode&&c==TokenPrivileges){ZeroMemory(p,n);*used=FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges);return TRUE;}
 if(live_mode&&c==TokenElevation){((TOKEN_ELEVATION *)p)->TokenIsElevated=0;*used=sizeof(TOKEN_ELEVATION);return TRUE;}
 SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
static BOOL WINAPI fx_mode(HANDLE h,LPDWORD m){(void)h;*m=ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT;fx_check();return TRUE;}
static BOOL WINAPI fx_writeconsole(HANDLE h,const VOID *p,DWORD n,LPDWORD out,LPVOID r){(void)h;(void)p;(void)r;fx_check();*out=n;return TRUE;}
static BOOL WINAPI fx_readconsole(HANDLE h,LPVOID p,DWORD n,LPDWORD out,LPVOID r){size_t len=wcslen(reply_text);(void)h;(void)r;fx_check();if(reenter)fx_reenter();if(len>=n)return FALSE;memcpy(p,reply_text,(len+1)*sizeof(WCHAR));*out=(DWORD)len;return TRUE;}
static BOOL WINAPI fx_write(HANDLE h,LPCVOID p,DWORD n,LPDWORD out,LPOVERLAPPED o){(void)h;(void)p;(void)o;fx_check();*out=n;return TRUE;}
static BOOL WINAPI fx_flush(HANDLE h){(void)h;fx_check();return TRUE;}

static HANDLE WINAPI fx_process(DWORD a,BOOL b,DWORD pid){(void)a;(void)b;(void)pid;return fx_handle();}
static BOOL WINAPI fx_times(HANDLE h,LPFILETIME c,LPFILETIME e,LPFILETIME k,LPFILETIME u){(void)h;ZeroMemory(c,sizeof(*c));c->dwLowDateTime=55;ZeroMemory(e,sizeof(*e));ZeroMemory(k,sizeof(*k));ZeroMemory(u,sizeof(*u));return TRUE;}
static BOOL WINAPI fx_image(HANDLE h,DWORD f,LPWSTR p,PDWORD n){static const WCHAR name[]=L"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";(void)h;(void)f;if(*n<sizeof(name)/sizeof(WCHAR))return FALSE;wcscpy_s(p,*n,live_mode?fx_live_path():name);*n=(DWORD)wcslen(p);return TRUE;}
static BOOL WINAPI fx_fileinfo(HANDLE h,LPBY_HANDLE_FILE_INFORMATION p){(void)h;ZeroMemory(p,sizeof(*p));p->dwVolumeSerialNumber=1;p->nFileIndexLow=2;return TRUE;}
static BOOL WINAPI fx_size(HANDLE h,PLARGE_INTEGER p){(void)h;p->QuadPart=2048;return TRUE;}
static BOOL WINAPI fx_seek(HANDLE h,LARGE_INTEGER p,PLARGE_INTEGER o,DWORD m){(void)h;(void)p;(void)o;(void)m;return TRUE;}
static BOOL WINAPI fx_read(HANDLE h,LPVOID p,DWORD n,LPDWORD out,LPOVERLAPPED o);

static HANDLE WINAPI fx_snapshot(DWORD f,DWORD p){(void)f;(void)p;module_index=0;return fx_handle();}
static BOOL WINAPI fx_module(HANDLE h,LPMODULEENTRY32W m){(void)h;wcscpy_s(m->szModule,256,module_index?L"libssl-3-x64.dll":L"libcrypto-3-x64.dll");wcscpy_s(m->szExePath,MAX_PATH,L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\");wcscat_s(m->szExePath,MAX_PATH,m->szModule);return TRUE;}
static BOOL WINAPI fx_module_next(HANDLE h,LPMODULEENTRY32W m){if(++module_index==1)return fx_module(h,m);SetLastError(ERROR_NO_MORE_FILES);return FALSE;}
static BOOL WINAPI fx_sidtext(PSID p,LPWSTR *out){(void)p;*out=L"S-1-5-21";return TRUE;}
static HLOCAL WINAPI fx_free(HLOCAL p){(void)p;return NULL;}
static BOOL WINAPI fx_duplicate(HANDLE h,SECURITY_IMPERSONATION_LEVEL l,PHANDLE o){(void)h;(void)l;*o=fx_handle();return *o!=NULL;}
static DWORD WINAPI fx_security(HANDLE h,SE_OBJECT_TYPE t,SECURITY_INFORMATION m,PSID *o,PSID *g,PACL *d,PACL *a,PSECURITY_DESCRIPTOR *sd){(void)h;(void)t;(void)m;(void)o;(void)g;(void)d;(void)a;*sd=(PSECURITY_DESCRIPTOR)fake_sid;return 0;}
static BOOL WINAPI fx_access(PSECURITY_DESCRIPTOR sd,HANDLE t,DWORD want,PGENERIC_MAPPING m,PPRIVILEGE_SET p,LPDWORD n,LPDWORD granted,LPBOOL allowed){(void)sd;(void)t;(void)want;(void)m;(void)p;(void)n;*granted=FILE_READ_DATA|FILE_EXECUTE;*allowed=TRUE;return TRUE;}
static DWORD WINAPI fx_tcp(PVOID p,PDWORD n,BOOL b,ULONG family,TCP_TABLE_CLASS c,ULONG r){(void)b;(void)family;(void)c;(void)r;ZeroMemory(p,32);*n=32;return 0;}
static DWORD WINAPI fx_udp(PVOID p,PDWORD n,BOOL b,ULONG family,UDP_TABLE_CLASS c,ULONG r){(void)b;(void)family;(void)c;(void)r;ZeroMemory(p,32);*n=32;return 0;}
static BOOL WINAPI fx_threadfirst(HANDLE h,LPTHREADENTRY32 p){(void)h;p->th32OwnerProcessID=123;p->th32ThreadID=777;return TRUE;}
static BOOL WINAPI fx_threadnext(HANDLE h,LPTHREADENTRY32 p){(void)h;(void)p;SetLastError(ERROR_NO_MORE_FILES);return FALSE;}
static HANDLE WINAPI fx_openthread(DWORD a,BOOL b,DWORD p){(void)a;(void)b;(void)p;return fx_handle();}
static BOOL WINAPI fx_count(HANDLE h,PDWORD n){(void)h;*n=64;return TRUE;}
static BOOL WINAPI fx_memory(HANDLE h,PPROCESS_MEMORY_COUNTERS p,DWORD n){(void)h;ZeroMemory(p,n);return TRUE;}
#define CreateToolhelp32Snapshot fx_snapshot
#define Module32FirstW fx_module
#define Module32NextW fx_module_next
#define ConvertSidToStringSidW fx_sidtext
#define LocalFree fx_free
#define DuplicateToken fx_duplicate
#define GetSecurityInfo fx_security
#define AccessCheck fx_access
#define GetExtendedTcpTable fx_tcp
#define GetExtendedUdpTable fx_udp
#define Thread32First fx_threadfirst
#define Thread32Next fx_threadnext
#define OpenThread fx_openthread
#define GetProcessHandleCount fx_count
#undef GetProcessMemoryInfo
#define GetProcessMemoryInfo fx_memory
#define OpenProcess fx_process
#define GetProcessTimes fx_times
#define QueryFullProcessImageNameW fx_image
#define GetFileInformationByHandle fx_fileinfo
#define GetFileSizeEx fx_size
#define SetFilePointerEx fx_seek
#define ReadFile fx_read
#define OpenThreadToken fx_thread
#define OpenProcessToken fx_token
#define CreateFileW fx_file
#define CloseHandle fx_close
#define GetTokenInformation fx_info
#define GetConsoleMode fx_mode
#define WriteConsoleW fx_writeconsole
#define ReadConsoleW fx_readconsole
#define WriteFile fx_write
#define FlushFileBuffers fx_flush
#define papacc_server_console_win32_stop_requested fx_stop
#define papacc_server_console_win32_request_stop fx_request
#define main forbidden_operational_entry
#include "../tools/qualification/scm-registration-lab.c"
#undef main

static LPCWSTR fx_live_path(void){
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
 return cycle_path;
#else
 return L"UNUSED";
#endif
}
static BOOL WINAPI fx_read(HANDLE h,LPVOID p,DWORD n,LPDWORD out,LPOVERLAPPED o){(void)h;(void)o;
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
 if(n==sizeof(PSRF_FRAME)){PSRF_FRAME *f=(PSRF_FRAME *)p;ZeroMemory(f,sizeof(*f));memcpy(f->magic,psrf_magic,16);memcpy(f->attempt,IG_ATTEMPT,32);f->version=1;f->kind=1;f->length=1;f->text[0]='X';f->check=psrf_checksum(f);*out=n;return TRUE;}
#endif
 (void)p;(void)n;*out=0;SetLastError(ERROR_INVALID_DATA);return FALSE;}
#define T(x) do{++checks;if(!(x)){printf("FAIL %u %s\n",(unsigned)__LINE__,#x);return 1;}}while(0)
static void fx_check(void){if(lab_aux_owner.state&A1_BUSY)++busy_seen;}
static void fx_reenter(void){unsigned before=opens;DWORD e=lab_operator_current();if(!e||opens!=before)++opens;}
static void reset(void){ZeroMemory(&native,sizeof(native));ZeroMemory(&lab_aux_owner,sizeof(lab_aux_owner));ZeroMemory(&lab_stats,sizeof(lab_stats));stop_flag=opens=closes=fail_open=fail_close=reenter=busy_seen=live_mode=thread_token_present=0;reply_text[0]=0;}
static DWORD confirm(void){
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
 return ig_confirm();
#else
 return lab_confirm("0123456789abcdef0123456789abcdef");
#endif
}
static void expected(void){wcscpy_s(reply_text,128,
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
 L"INSTRUMENT_SCM_LAB 0b1de869e11493b7cb08fb329578f6ce"
#else
 L"REGISTER_SCM_LAB 0123456789abcdef0123456789abcdef"
#endif
 );}
int main(void){unsigned before;A1_LEASE a,b,c;DWORD e;
 reset();T(lab_operator_current()==0);T(opens==1&&closes==1&&a1_finish(&lab_aux_owner));T(busy_seen>0);
 reset();expected();T(confirm()==0);T(opens==2&&closes==2&&a1_finish(&lab_aux_owner));T(busy_seen>=5);
 reset();expected();fail_close=1;T(confirm()==ERROR_ACCESS_DENIED);T(closes==1&&native.retained&&stop_flag);T(a1_state(&lab_aux_owner,0)==A1_UNKNOWN&&a1_state(&lab_aux_owner,1)==A1_OWNED);before=opens;T(lab_operator_current()!=0);T(opens==before&&closes==1);
 reset();expected();fail_close=2;T(confirm()==ERROR_ACCESS_DENIED);T(closes==2&&native.retained&&a1_state(&lab_aux_owner,1)==A1_UNKNOWN);
 reset();expected();fail_open=2;T(confirm()==ERROR_ACCESS_DENIED);T(opens==2&&closes==1&&a1_finish(&lab_aux_owner));
 reset();wcscpy_s(reply_text,128,L"WRONG");T(confirm()==ERROR_CANCELLED);T(closes==2&&stop_flag&&a1_finish(&lab_aux_owner));
 reset();expected();reenter=1;T(confirm()!=0);T(opens==2&&closes==2&&a1_finish(&lab_aux_owner));
 reset();T(a1_reserve_pair(&lab_aux_owner,&a,&b)==0);T(a1_reserve(&lab_aux_owner,&c)!=0);T(confirm()!=0);T(opens==0);
 reset();stop_flag=1;T(lab_operator_current()!=0);T(opens==0);
 reset();fail_open=1;T(lab_operator()!=0);T(opens==1);
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
 reset();wcscpy_s(reply_text,128,L"CYCLE_SCM_LAB 0123456789abcdef0123456789abcdef");T(cycle_confirm("0123456789abcdef0123456789abcdef")==0);T(opens==2&&closes==2);
 reset();T(cycle_intent("0123456789abcdef0123456789abcdef")==0);T(opens==1&&closes==1);
 reset();fail_open=1;T(ig_extract()!=0);T(opens==1&&closes==0);
 reset();fx_fileinfo(NULL,&ig_diag.identity);T(ig_extract()==0);T(opens==1&&closes==1);
 reset();fx_fileinfo(NULL,&ig_diag.identity);fail_close=1;T(ig_extract()==ERROR_ACCESS_DENIED);T(native.retained&&closes==1);
 reset();ZeroMemory(&ig_observer,sizeof(ig_observer));T(ig_actor(0,123,55)==ERROR_INVALID_OWNER);T(opens==2&&closes==1&&a1_finish(&lab_aux_owner));
 reset();ZeroMemory(&ig_observer,sizeof(ig_observer));fail_close=1;T(ig_actor(0,123,55)==ERROR_INVALID_OWNER);T(native.retained&&closes==1&&lab_aux_owner.first_error==ERROR_INVALID_OWNER);
 reset();cycle_process=NULL;fail_open=1;T(cycle_live(123)!=0);T(opens==1);
 reset();cycle_process=NULL;cycle_sd=NULL;live_mode=1;memcpy(native.service_sid,fake_sid,sizeof(fake_sid));T(cycle_live(123)==0);T(a1_finish(&lab_aux_owner));
 reset();cycle_process=NULL;cycle_sd=NULL;live_mode=1;thread_token_present=1;fail_close=2;memcpy(native.service_sid,fake_sid,sizeof(fake_sid));T(cycle_live(123)==ERROR_ACCESS_DENIED);T(native.retained&&closes==2);T(a1_state(&lab_aux_owner,0)==A1_UNKNOWN);before=opens;T(cycle_cleanup()!=0);T(opens==before&&closes==2);
 reset();a1_retain(&lab_aux_owner,1816);before=opens;T(ig_actor(0,123,55)!=0);T(cycle_live(123)!=0);T(ig_project_census()!=0);T(ig_create_intent()!=0);T(ig_create_diagnostic()!=0);T(ig_extract()!=0);T(opens==before);T(lab_aux_owner.first_error==1816);
 T(ig_observe()!=0);T(ig_measure_process(0,NULL,0,NULL,NULL)!=0);T(ig_n0_roots_current(1)!=0);T(ig_n0_current(NULL)!=0);T(ig_n0_bind(0,0,0,0,0)!=0);T(psa_bd_current()!=0);T(psa_bd_aux_current(NULL,0)!=0);T(papacc_server_service_admission_bd_bind(NULL,NULL)!=0);T(ig_absence_name(0)!=0);T(opens==before);
#else
 reset();T(lab_intent("0123456789abcdef0123456789abcdef")==0);T(opens==1&&closes==1);
#endif
 reset();native.thread=GetCurrentThreadId()+1;T(lab_operator_current()==ERROR_ACCESS_DENIED);T(opens==0&&lab_aux_owner.state==0);
 reset();T(lab_aux_token(&a,GetCurrentProcess(),0,TOKEN_QUERY)==0);b=a;++b.ticket;e=lab_aux_close_preserving(b,0);T(e!=0&&closes==0);T(lab_aux_close_preserving(a,0)!=0);T(closes==1); /* first error stays, known owner closes once */
 printf("ACTUAL_CALLER_API_SIMULATION checks=%u owner=%zu lease=%zu request=%zu; NO_LAB\n",checks,sizeof(A1_OWNER),sizeof(A1_LEASE),sizeof(LAB_AUX_REQUEST));return 0;
}
