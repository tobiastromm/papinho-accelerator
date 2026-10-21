/* TEST_ONLY repeated normal-package lifecycle; not C2 production or Browser. */
#define main prior_single_probe_main
#include "../ed11_resource_probe/ed11_resource_probe.c"
#undef main
#include <tlhelp32.h>
static int thread_sample(unsigned cycle,const char *phase) {
 HANDLE snap;THREADENTRY32 t;DWORD pid;unsigned count;
 pid=GetCurrentProcessId();count=0;snap=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
 if(snap==INVALID_HANDLE_VALUE)return 0;memset(&t,0,sizeof(t));t.dwSize=(DWORD)sizeof(t);
 if(!Thread32First(snap,&t)){CloseHandle(snap);return 0;}
 do {if(t.th32OwnerProcessID==pid)++count;} while(Thread32Next(snap,&t));
 if(!CloseHandle(snap))return 0;
 printf("{\"event\":\"THREAD_SAMPLE\",\"cycle\":%u,\"phase\":\"%s\",\"threads\":%u,\"pid\":%lu,\"scope\":\"MODERN_X64_OBSERVER_NOT_BROWSER_API\"}\n",cycle,phase,count,(unsigned long)pid);return 1;
}
int main(int argc,char **argv) {
 Probe p,second;WSADATA wd;unsigned cycle,n;int two;
 if(argc!=3)return 64;n=(unsigned)strtoul(argv[2],NULL,10);if(!n||n>512)return 64;
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
 memset(payload,0x5a,sizeof(payload));
 measure(NULL,"LOOP_PROCESS_BASELINE");if(!thread_sample(0,"BASELINE"))return 80;
 if(pst_api_version()!=PST_API_VERSION||pst_library_version()!=PST_LIBRARY_VERSION)return 63;

 {const char *names[2]={"libssl-3-x64.dll","libcrypto-3-x64.dll"};unsigned i;int j;char path[MAX_PATH];HMODULE h;
 for(i=0;i<2;i++){h=GetModuleHandleA(names[i]);if(!h||!GetModuleFileNameA(h,path,MAX_PATH))return 62;for(j=0;path[j];j++)if(path[j]=='\\')path[j]='/';
 printf("{\"event\":\"ACTUAL_LOADED_MODULE\",\"module\":\"%s\",\"path\":\"%s\"}\n",names[i],path);}}
 if(WSAStartup(MAKEWORD(2,2),&wd))return 65;
 if(pst_win32_register_builtin_providers()!=PST_RESULT_OK)return 66;
 for(cycle=0;cycle<n;++cycle){
  MEMORYSTATUSEX margin;memset(&margin,0,sizeof(margin));margin.dwLength=(DWORD)sizeof(margin);
  if(!GlobalMemoryStatusEx(&margin))return 83;
  /* Standalone fixture safety guard, not an accepted production headroom policy. */
  if(margin.ullAvailPhys<64ULL*1024*1024||margin.ullAvailPageFile<64ULL*1024*1024){printf("{\"result\":\"DENIED_HOST_MARGIN_NOT_PASS\"}\n");WSACleanup();return 84;}
  ba_peak=ab_peak=0;two=(cycle%16)==0;
  printf("{\"event\":\"CYCLE_BEGIN\",\"cycle\":%u,\"concurrent_operations\":%u}\n",cycle,two?2u:1u);
  if(!init(&p,10000+cycle*2,argv[1],"openssl",NULL)||!handshake(&p))return 67;
  if(two){if(!init(&second,10001+cycle*2,argv[1],"openssl",p.rt)||!handshake(&second))return 68;}
  if(!thread_sample(cycle,"ACTIVE"))return 81;
  /* Normal package actual membership precondition every cycle.
     No synthetic R4 or native close-error injection. */
  if(!pressure(&p)||!close_probe(&p,1,0))return 69;
  if(two){if(!pressure(&second)||!close_probe(&second,1,0)||!cleanup(&second))return 70;}
  if(!cleanup(&p))return 71;
  /* p retained only as owned stack trace metadata; all PST children destroyed.
     Shared runtime freed after second operation; no postfree PST query. */
  p.rt=NULL;p.ci=NULL;p.si=NULL;p.trust=NULL;
  measure(&p,"LOOP_POST_ELIGIBLE_DESTRUCTION");
  if(!thread_sample(cycle,"POST_CLOSE"))return 82;
  printf("{\"event\":\"CYCLE_END\",\"cycle\":%u,\"op\":%u,\"generation\":7,\"result\":\"PASS\",\"scope\":\"NORMAL_PUBLIC_PST_DESTROYED_AND_PRECONDITION\"}\n",cycle,p.op);
 }
 if(WSACleanup())return 72;
 measure(NULL,"LOOP_FINAL_AFTER_WINSOCK");if(measure_failed)return 73;
 printf("{\"result\":\"PASS\",\"cycles\":%u,\"scope\":\"REPEATED_NORMAL_OPENSSL_FIXTURE_NOT_GLOBAL_C2\"}\n",n);return 0;
}
