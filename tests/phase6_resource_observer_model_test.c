/* Native API fault model only. Never opens SCM/processes or creates endpoints.
 * Each case models an independent collector invocation; retained refs are fake. */
#include <winsock2.h>
#include <windows.h>
#include <winsvc.h>
#include <psapi.h>
#include <stdio.h>
#include <string.h>
static unsigned fault,opens,process_closes,service_closes,query_calls,time_calls,bad_rights;
static LONGLONG ticks;
static SC_HANDLE WINAPI m_manager(LPCWSTR host,LPCWSTR db,DWORD rights)
{opens++;if(host||db||rights!=SC_MANAGER_CONNECT)bad_rights++;if(fault==1U){SetLastError(ERROR_ACCESS_DENIED);return NULL;}return (SC_HANDLE)11;}
static SC_HANDLE WINAPI m_service(SC_HANDLE manager,LPCWSTR name,DWORD rights)
{opens++;if(manager!=(SC_HANDLE)11||wcscmp(name,L"PapinhoAccelerator")||rights!=SERVICE_QUERY_STATUS)bad_rights++;if(fault==2U){SetLastError(ERROR_SERVICE_DOES_NOT_EXIST);return NULL;}return (SC_HANDLE)12;}
static BOOL WINAPI m_status(SC_HANDLE service,SC_STATUS_TYPE type,LPBYTE bytes,DWORD cap,LPDWORD n)
{SERVICE_STATUS_PROCESS *s=(SERVICE_STATUS_PROCESS *)bytes;query_calls++;
 if(service!=(SC_HANDLE)12||type!=SC_STATUS_PROCESS_INFO||cap!=sizeof(*s))return FALSE;
 if(fault==3U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}memset(s,0,sizeof(*s));*n=sizeof(*s);
 s->dwServiceType=fault==5U?SERVICE_WIN32_SHARE_PROCESS:SERVICE_WIN32_OWN_PROCESS;s->dwCurrentState=fault==13U&&query_calls>3U?SERVICE_STOP_PENDING:SERVICE_RUNNING;
 s->dwProcessId=fault==4U?0U:fault==7U&&query_calls>1U?78U:77U;return TRUE;}
static HANDLE WINAPI m_process(DWORD rights,BOOL inherit,DWORD pid)
{opens++;if(rights!=(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE)||inherit||pid!=77U)bad_rights++;if(fault==6U){SetLastError(ERROR_ACCESS_DENIED);return NULL;}return (HANDLE)13;}
static DWORD WINAPI m_pid(HANDLE h){return h==(HANDLE)13?77U:99U;}
static BOOL WINAPI m_times(HANDLE h,LPFILETIME c,LPFILETIME e,LPFILETIME k,LPFILETIME u)
{(void)h;memset(c,0,sizeof(*c));memset(e,0,sizeof(*e));memset(k,0,sizeof(*k));memset(u,0,sizeof(*u));time_calls++;c->dwLowDateTime=fault==8U&&time_calls>2U?100U:99U;return TRUE;}
static DWORD WINAPI m_wait(HANDLE h,DWORD ms){(void)h;(void)ms;return fault==14U?WAIT_OBJECT_0:WAIT_TIMEOUT;}
static BOOL WINAPI m_close(HANDLE h){if(h!=(HANDLE)13)bad_rights++;process_closes++;if(fault==10U){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static BOOL WINAPI m_close_service(SC_HANDLE h){if(h!=(SC_HANDLE)11&&h!=(SC_HANDLE)12)bad_rights++;service_closes++;if(fault==11U){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static BOOL WINAPI m_handles(HANDLE h,PDWORD n){(void)h;if(fault==9U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}*n=5U;return TRUE;}
static BOOL WINAPI m_memory(HANDLE h,PPROCESS_MEMORY_COUNTERS p,DWORD n)
{PROCESS_MEMORY_COUNTERS_EX *e=(PROCESS_MEMORY_COUNTERS_EX *)p;(void)h;memset(p,0,n);e->cb=n;e->PrivateUsage=4096;e->WorkingSetSize=8192;return TRUE;}
static BOOL WINAPI m_performance(PPERFORMANCE_INFORMATION p,DWORD n)
{memset(p,0,n);p->cb=n;p->PageSize=4096;p->CommitTotal=256;p->CommitLimit=512;p->PhysicalAvailable=256;p->PhysicalTotal=512;return TRUE;}
static BOOL WINAPI m_frequency(LARGE_INTEGER *v){if(fault==12U){SetLastError(ERROR_GEN_FAILURE);return FALSE;}v->QuadPart=1000000;return TRUE;}
static BOOL WINAPI m_counter(LARGE_INTEGER *v){v->QuadPart=(ticks+=100);return TRUE;}
static ULONGLONG WINAPI m_tick(void){return 100U;}
#define OpenSCManagerW m_manager
#define OpenServiceW m_service
#define QueryServiceStatusEx m_status
#define OpenProcess m_process
#define GetProcessId m_pid
#define GetProcessTimes m_times
#define WaitForSingleObject m_wait
#define CloseHandle m_close
#define CloseServiceHandle m_close_service
#define GetProcessHandleCount m_handles
#undef GetProcessMemoryInfo
#define GetProcessMemoryInfo m_memory
#undef GetPerformanceInfo
#define GetPerformanceInfo m_performance
#define QueryPerformanceFrequency m_frequency
#define QueryPerformanceCounter m_counter
#define GetTickCount64 m_tick
#define QOC_METADATA_MODEL 1
#define main preserved_census_main
#include "phase6_memory_census_test.c"
#undef main
#define C(x) do{if(!(x)){printf("FAIL RESOURCE_OBSERVER case=%u line=%u\n",fault,(unsigned)__LINE__);return 1;}}while(0)
int main(void)
{
 unsigned before,result;
 for(fault=0;fault<15U;fault++){
  /* Model process-lifetime reset only; never a real retained-ref release. */
  metadata_manager=metadata_service=NULL;metadata_process=NULL;metadata_pid=0;memset(&metadata_creation,0,sizeof(metadata_creation));
  opens=process_closes=service_closes=query_calls=time_calls=bad_rights=0;ticks=0;
  result=(unsigned)service_observation();C(!bad_rights&&opens<=3U);
  if(!fault){C(!result&&process_closes==1U&&service_closes==2U&&!metadata_manager&&!metadata_service&&!metadata_process);}
  else{C(result==2U);
   if(fault==10U){C(metadata_process&&metadata_service&&metadata_manager&&process_closes==1U&&!service_closes);before=opens;C(service_observation()==2&&before==opens&&process_closes==1U);}
   else if(fault==11U){C(!metadata_process&&metadata_service&&metadata_manager&&process_closes==1U&&service_closes==1U);before=opens;C(service_observation()==2&&before==opens&&service_closes==1U);}
   else C(!metadata_manager&&!metadata_service&&!metadata_process);
  }
 }
 puts("PASS RESOURCE_OBSERVER_MODEL cases=15 MINIMUM_RIGHTS NO_SCM_NATIVE NO_ENDPOINT NO_KSP RETAIN_ON_CLOSE_UNKNOWN NO_RETRY");return 0;
}
