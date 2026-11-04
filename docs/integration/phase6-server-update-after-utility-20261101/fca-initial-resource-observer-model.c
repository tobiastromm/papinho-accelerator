#define _WIN32_WINNT 0x0600
#include "personal_scm_registration_win32.h"
#include <psapi.h>
#include <string.h>
static DWORD model_handles=128,model_fail;
static ULONGLONG model_physical=7239172096ULL,model_limit=10,model_total=7,model_page=1073741824ULL;
static BOOL WINAPI qf(LARGE_INTEGER *v){v->QuadPart=1000;return TRUE;}
static BOOL WINAPI qc(LARGE_INTEGER *v){v->QuadPart=100;return TRUE;}
static BOOL WINAPI hc(HANDLE h,PDWORD v){(void)h;*v=model_handles;return TRUE;}
static BOOL WINAPI mi(HANDLE h,PPROCESS_MEMORY_COUNTERS v,DWORD n){PROCESS_MEMORY_COUNTERS_EX *m=(PROCESS_MEMORY_COUNTERS_EX *)v;(void)h;(void)n;m->PrivateUsage=1;m->WorkingSetSize=1;return TRUE;}
static BOOL WINAPI gm(LPMEMORYSTATUSEX v){v->ullTotalPhys=16ULL*1073741824ULL;v->ullAvailPhys=model_physical;return TRUE;}
static BOOL WINAPI pi(PPERFORMANCE_INFORMATION v,DWORD n){(void)n;if(model_fail)return FALSE;v->CommitLimit=(SIZE_T)model_limit;v->CommitTotal=(SIZE_T)model_total;v->PageSize=(SIZE_T)model_page;return TRUE;}
static DWORD WINAPI ge(void){return 5;}
#define QueryPerformanceFrequency qf
#define QueryPerformanceCounter qc
#define GetProcessHandleCount hc
#define GetProcessMemoryInfo mi
#define GlobalMemoryStatusEx gm
#define GetPerformanceInfo pi
#define GetLastError ge
#include "server_personal_resource_sample_win32.inc"
#define C(x) do{if(!(x))return __LINE__;}while(0)
int main(void){
 PAPACC_SCM_RESOURCE_OBSERVATION o;DWORD e;
 memset(&o,0,sizeof(o));e=personal_resource_collect(&o,1,0);C(!e&&o.samples==1&&o.first.handles==128&&o.first.physical_available==7239172096ULL&&o.first.commit_available==3221225472ULL);
 model_handles=129;model_physical--;model_total=8;memset(&o,0,sizeof(o));C(!personal_resource_collect(&o,1,0)&&o.first.handles==129&&o.first.physical_available==7239172095ULL&&o.first.commit_available==2147483648ULL);
 model_fail=1;memset(&o,0,sizeof(o));C(personal_resource_collect(&o,1,0)==5&&o.error_stage==6&&o.samples==0);C(personal_resource_collect(&o,1,0)==5&&o.attempts==1);
 model_fail=0;model_total=11;memset(&o,0,sizeof(o));C(personal_resource_collect(&o,1,0)==ERROR_INVALID_DATA&&o.error_stage==8);
 model_total=0;model_limit=~0ULL;model_page=4096;memset(&o,0,sizeof(o));C(personal_resource_collect(&o,1,0)==ERROR_ARITHMETIC_OVERFLOW);
 puts("FCA_RESOURCE_REAL_COLLECTOR_MODEL 6 scenarios PASS; MOCKS_ONLY_NO_NATIVE_COLLECTION");return 0;
}