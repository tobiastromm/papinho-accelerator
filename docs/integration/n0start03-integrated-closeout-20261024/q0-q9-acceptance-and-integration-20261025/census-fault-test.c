/* Actual census body, simulated observations only. No process/service launch. */
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#define PAPACC_SINGLE_LAB_ADMISSION 1
#define PAPACC_N0_N9_NATIVE_OWNER 1
#define PS_BD_ROLES 4
static struct {struct {struct {unsigned bound,used;struct {unsigned n0;} binding;struct {unsigned present[4];struct {DWORD pid;HANDLE handle;ULONGLONG creation;} role[4];} inventory;void *check;} bd;} admission;unsigned retained;} native;
static struct {DWORD pid[4];} ig_observer;
static HANDLE ig_guard_snapshot;
static const char *cycle_stage="ISOLATED_START_PENDING";
static unsigned mode,index_now,opens,closes,first,seen_mask,expected_mask;
static const char *predicate;
static unsigned correlated;static DWORD correlation_result,correlation_error;
static DWORD lab_aux_gate(void){return mode==6?ERROR_CANCELLED:0;}
static DWORD lab_error(void){return GetLastError();}
static DWORD papacc_server_service_admission_bd_revalidate(void *x){(void)x;return mode==4?ERROR_READ_FAULT:0;}
static DWORD ps_bd_inventory(void *a,void *b,ULONGLONG tick){(void)a;(void)b;(void)tick;return 0;}
static DWORD ps_bd_census(void *a,void *b,ULONGLONG tick,DWORD pid,DWORD parent,unsigned prefix){(void)a;(void)b;(void)tick;(void)parent;(void)prefix;return mode==7&&pid==102?ERROR_INVALID_DATA:0;}
static HANDLE WINAPI m_snapshot(DWORD flags,DWORD pid){(void)flags;(void)pid;opens++;if(mode==3){SetLastError(ERROR_ACCESS_DENIED);return INVALID_HANDLE_VALUE;}return (HANDLE)(ULONG_PTR)20;}
static BOOL fill(LPPROCESSENTRY32W pe){while(index_now<4){unsigned j=index_now++;if((mode==1&&j==2)||(mode==2&&j==1))continue;ZeroMemory(pe,sizeof(*pe));pe->dwSize=sizeof(*pe);pe->th32ProcessID=100+j;return TRUE;}SetLastError(ERROR_NO_MORE_FILES);return FALSE;}
static BOOL WINAPI m_first(HANDLE h,LPPROCESSENTRY32W pe){(void)h;index_now=0;return fill(pe);}
static BOOL WINAPI m_next(HANDLE h,LPPROCESSENTRY32W pe){(void)h;return fill(pe);}
static BOOL WINAPI m_close(HANDLE h){(void)h;closes++;if(mode==5){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static HANDLE ig_diag_handle(const char *s,const char *a,const char *r,HANDLE h){(void)s;(void)a;(void)r;return h;}
static BOOL ig_diag_bool(const char *s,const char *a,const char *r,BOOL v){(void)s;(void)a;(void)r;return v;}
static BOOL ig_diag_next(const char *s,const char *a,const char *r,BOOL v){(void)s;(void)a;(void)r;return v;}
static void ig_diag_note(const char *s,const char *a,const char *r,DWORD e,DWORD n,DWORD pid,unsigned role){(void)s;(void)r;(void)n;(void)pid;(void)role;if(!first){first=e;predicate=a;}}
static int note(const char *fmt,...){va_list args;va_start(args,fmt);if(strstr(fmt,"IG_CENSUS_ROLE_MASK")){expected_mask=va_arg(args,unsigned);seen_mask=va_arg(args,unsigned);}if(strstr(fmt,"IG_CENSUS_ORIGINAL")){unsigned role=va_arg(args,unsigned);unsigned long expected_pid=va_arg(args,unsigned long);ULONGLONG birth=va_arg(args,ULONGLONG);unsigned long actual_pid=va_arg(args,unsigned long);ULONGLONG actual_birth=va_arg(args,ULONGLONG);correlation_result=va_arg(args,unsigned long);correlation_error=va_arg(args,unsigned long);correlated++;if(role!=(mode==1?2U:1U)||expected_pid!=100U+role||actual_pid!=expected_pid||birth!=123||actual_birth!=birth)correlation_error=ERROR_INVALID_DATA;}va_end(args);return 0;}

#define CreateToolhelp32Snapshot m_snapshot
#define Process32FirstW m_first
#define Process32NextW m_next
#define CloseHandle m_close
static DWORD WINAPI q_pid(HANDLE h){return (DWORD)(ULONG_PTR)h;}
static BOOL WINAPI q_times(HANDLE h,LPFILETIME c,LPFILETIME e,LPFILETIME k,LPFILETIME u){(void)h;ZeroMemory(c,sizeof(*c));ZeroMemory(e,sizeof(*e));ZeroMemory(k,sizeof(*k));ZeroMemory(u,sizeof(*u));c->dwLowDateTime=123;return TRUE;}
static DWORD WINAPI q_wait(HANDLE h,DWORD ms){(void)h;(void)ms;return mode==1?WAIT_OBJECT_0:WAIT_TIMEOUT;}
#define PT_PID q_pid
#define PT_TIMES q_times
#define PT_WAIT q_wait
#define PT_STOP() (lab_aux_gate()!=0)
#include "../../../../apps/server/server_service_observation_win32.h"
#define printf note
#include "census-body.CURRENT.inc"
#undef printf
#define T(x) do{if(!(x)){printf("FAIL line=%d mode=%u\n",__LINE__,mode);return 1;}}while(0)
int main(void){unsigned i,j;DWORD e;for(i=0;i<8;i++){mode=i;correlated=correlation_result=correlation_error=0;index_now=opens=closes=first=seen_mask=expected_mask=0;predicate=NULL;ig_guard_snapshot=NULL;ZeroMemory(&native,sizeof(native));native.admission.bd.used=native.admission.bd.bound=native.admission.bd.binding.n0=1;for(j=0;j<4;j++){native.admission.bd.inventory.present[j]=1;native.admission.bd.inventory.role[j].pid=100+j;native.admission.bd.inventory.role[j].handle=(HANDLE)(ULONG_PTR)(100+j);native.admission.bd.inventory.role[j].creation=123;}e=ig_project_census_body();if(!i)T(!e&&opens==1&&closes==1);else T(e!=0);if(i==1||i==2){T(e==ERROR_INVALID_DATA&&first==ERROR_INVALID_DATA&&!strcmp(predicate,"n0_original_roles_missing"));T(expected_mask==15&&seen_mask==(i==1?11U:13U));T(correlated==1&&!correlation_error&&correlation_result==(DWORD)(i==1?PT_MISSING_SIGNALED_AFTER_SNAPSHOT:PT_MISSING_LIVE_AFTER_SNAPSHOT));}if(i==3)T(opens==1&&!closes);if(i==4||i==6)T(!opens&&!closes);if(i==5)T(native.retained&&ig_guard_snapshot);else T(!native.retained);}
 puts("ACTUAL_CENSUS_FAULTS=PASS cases=8 SIMULATED_QUERY_ONLY NO_ADMISSION_NO_START");return 0;}
