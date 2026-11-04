/* Isolated public API runtime core only. No Connection/transport/trust/key/network. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <papinho_secure_transport.h>
#include <papinho_secure_transport_win32.h>
typedef unsigned __int64 U64;
typedef struct QMEM {DWORD cb,fault;SIZE_T peak_ws,ws,peak_paged,paged,peak_nonpaged,nonpaged,commit,peak_commit,private_bytes;} QMEM;
typedef BOOL (WINAPI *QINFO)(HANDLE,void *,DWORD);
typedef BOOL (WINAPI *QHANDLES)(HANDLE,DWORD *);
static QINFO info;static QHANDLES handles;static unsigned seq;static DWORD first;
static int sample(const char *phase,unsigned cycle){QMEM m;DWORD h,begin,end;FILETIME c,e,k,u;U64 creation;
 begin=GetTickCount();ZeroMemory(&m,sizeof(m));m.cb=sizeof(m);
 if(!info(GetCurrentProcess(),&m,sizeof(m))||!handles(GetCurrentProcess(),&h)||!GetProcessTimes(GetCurrentProcess(),&c,&e,&k,&u)){first=GetLastError();if(!first)first=ERROR_GEN_FAILURE;return 0;}end=GetTickCount();if(end<begin){first=ERROR_INVALID_DATA;return 0;}
 creation=((U64)c.dwHighDateTime<<32)|c.dwLowDateTime;
 printf("S,%u,%u,%s,%lu,%I64u,%lu,%lu,%I64u,%I64u,%I64u,%I64u,%I64u,%I64u,%lu\n",++seq,cycle,phase,(unsigned long)GetCurrentProcessId(),creation,(unsigned long)begin,(unsigned long)end,(U64)m.private_bytes,(U64)m.ws,(U64)m.commit,(U64)m.paged,(U64)m.nonpaged,(U64)m.peak_commit,(unsigned long)h);return 1;}
int main(void){char path[MAX_PATH];UINT n;HMODULE psapi=NULL,kernel;FARPROC f;unsigned cycle;PST_RUNTIME_OPTIONS opt;pst_runtime *runtime=NULL;PST_RUNTIME_INFO ri;PST_PROVIDER_INFO pi;PST_RESULT result;pst_size i;int retained=0;
 n=GetSystemDirectoryA(path,MAX_PATH);if(!n||n+11U>=MAX_PATH)return 2;memcpy(path+n,"\\psapi.dll",11U);psapi=LoadLibraryA(path);if(!psapi)return 3;
 f=GetProcAddress(psapi,"GetProcessMemoryInfo");memcpy(&info,&f,sizeof(info));kernel=GetModuleHandleA("kernel32.dll");f=GetProcAddress(kernel,"GetProcessHandleCount");memcpy(&handles,&f,sizeof(handles));if(!info||!handles){first=ERROR_PROC_NOT_FOUND;goto done;}
 printf("ABI pointer=%u qmem=%u runtime_options=%u provider_info=%u\n",(unsigned)sizeof(void*),(unsigned)sizeof(QMEM),(unsigned)sizeof(PST_RUNTIME_OPTIONS),(unsigned)sizeof(PST_PROVIDER_INFO));
 puts("format=RUNTIME_CORE_OBSERVATION_V1; units=bytes/count; clock=local_GetTickCount_u32; no_cross_process_clock; observer=psapi_loaded_stdout_captured; NO_CONNECTION_NO_NETWORK_NO_PROVIDER_INITIALIZATION_REQUEST");

#ifdef QUANT_NSS
 result=pst_win32_register_retrozilla_nss();
#else
 result=pst_win32_register_builtin_providers();
#endif
 if(result!=PST_RESULT_OK){first=ERROR_INVALID_DATA;goto done;}
 if(!sample("BASELINE",0))goto done;
 for(cycle=1;cycle<=12U;cycle++){
 ZeroMemory(&opt,sizeof(opt));opt.struct_size=sizeof(opt);opt.api_version=PST_API_VERSION;result=pst_runtime_create(&opt,&runtime);if(result!=PST_RESULT_OK||!runtime){first=ERROR_INVALID_DATA;goto done;}
 ZeroMemory(&ri,sizeof(ri));ri.struct_size=sizeof(ri);ri.api_version=PST_API_VERSION;if(pst_runtime_get_info(runtime,&ri)!=PST_RESULT_OK||!ri.provider_count||ri.provider_count>8U){first=ERROR_INVALID_DATA;retained=1;goto done;}
 for(i=0;i<ri.provider_count;i++){ZeroMemory(&pi,sizeof(pi));pi.struct_size=sizeof(pi);pi.api_version=PST_API_VERSION;if(pst_runtime_get_provider_info(runtime,i,&pi)!=PST_RESULT_OK||pi.provider_id[sizeof(pi.provider_id)-1]){first=ERROR_INVALID_DATA;retained=1;goto done;}printf("P,%u,%lu,%s,%lu,%lu\n",cycle,(unsigned long)i,pi.provider_id,(unsigned long)pi.available,(unsigned long)pi.initialized);if(pi.initialized){first=ERROR_INVALID_DATA;retained=1;goto done;}}
 if(!sample("CORE_LIVE_PROVIDER_UNINITIALIZED",cycle))goto done;
 pst_runtime_release(runtime);runtime=NULL;
 if(!sample("PUBLIC_RELEASE_RETURNED",cycle))goto done;
 }
 done:
 if(runtime&&!retained){pst_runtime_release(runtime);runtime=NULL;}
 if(psapi&&!retained){if(!FreeLibrary(psapi)){if(!first)first=GetLastError();if(!first)first=ERROR_GEN_FAILURE;retained=1;}else psapi=NULL;}
 printf("FINAL first=%lu runtime_ref=%u module_ref=%u retained=%u; release_void_not_backend_quiescence; NO_RETRY_NO_CAPACITY_GRANT\n",(unsigned long)first,(unsigned)(runtime!=NULL),(unsigned)(psapi!=NULL),(unsigned)retained);return first?1:0;}
