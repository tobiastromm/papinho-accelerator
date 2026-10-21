/* TEST_ONLY external modern observer of immutable C1 x86 fixture.
   No measurement APIs are added to Browser. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
static int sample(HANDLE process,unsigned seq,DWORD pid) {
 PROCESS_MEMORY_COUNTERS_EX m;MEMORYSTATUSEX g;DWORD handles;
 memset(&m,0,sizeof(m));m.cb=(DWORD)sizeof(m);memset(&g,0,sizeof(g));g.dwLength=(DWORD)sizeof(g);
 if(!GetProcessMemoryInfo(process,(PROCESS_MEMORY_COUNTERS*)&m,(DWORD)sizeof(m)))return 0;
 if(!GetProcessHandleCount(process,&handles)||!GlobalMemoryStatusEx(&g))return 0;
 printf("{\"event\":\"EXTERNAL_C1_SAMPLE\",\"seq\":%u,\"pid\":%lu,\"private_usage\":%llu,\"working_set\":%llu,\"peak_working_set\":%llu,\"pagefile_usage\":%llu,\"peak_pagefile_usage\":%llu,\"paged_pool_quota_usage\":%llu,\"nonpaged_pool_quota_usage\":%llu,\"handles\":%lu,\"physical_available\":%llu,\"commit_available\":%llu,\"scope\":\"EXISTING_C1_X86_FIXTURE_CURRENT_HOST_NOT_BROWSER_C2\"}\n",seq,(unsigned long)pid,(unsigned long long)m.PrivateUsage,(unsigned long long)m.WorkingSetSize,(unsigned long long)m.PeakWorkingSetSize,(unsigned long long)m.PagefileUsage,(unsigned long long)m.PeakPagefileUsage,(unsigned long long)m.QuotaPagedPoolUsage,(unsigned long long)m.QuotaNonPagedPoolUsage,(unsigned long)handles,(unsigned long long)g.ullAvailPhys,(unsigned long long)g.ullAvailPageFile);fflush(stdout);
 if(seq==1||seq%64==0){HANDLE snap;THREADENTRY32 t;unsigned threads=0;int valid=0;snap=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
 if(snap!=INVALID_HANDLE_VALUE){memset(&t,0,sizeof(t));t.dwSize=(DWORD)sizeof(t);if(Thread32First(snap,&t)){valid=1;do{if(t.th32OwnerProcessID==pid)++threads;}while(Thread32Next(snap,&t));}CloseHandle(snap);}
 printf("{\"event\":\"EXTERNAL_C1_THREAD_SAMPLE\",\"seq\":%u,\"pid\":%lu,\"valid\":%d,\"threads\":%u}\n",seq,(unsigned long)pid,valid,threads);}
 return 1;
}
int main(int argc,char **argv) {
 STARTUPINFOA si;PROCESS_INFORMATION pi;DWORD start,wait,code;unsigned seq,missed;
 if(argc!=2)return 64;memset(&si,0,sizeof(si));memset(&pi,0,sizeof(pi));si.cb=(DWORD)sizeof(si);
 si.dwFlags=STARTF_USESTDHANDLES;si.hStdOutput=GetStdHandle(STD_OUTPUT_HANDLE);si.hStdError=GetStdHandle(STD_ERROR_HANDLE);si.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
 if(!CreateProcessA(argv[1],NULL,NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi))return 65;
 start=GetTickCount();seq=0;missed=0;
 for(;;){
  if(sample(pi.hProcess,seq+1,pi.dwProcessId))++seq;else ++missed;
  wait=WaitForSingleObject(pi.hProcess,1);
  if(wait==WAIT_OBJECT_0)break;if(wait!=WAIT_TIMEOUT)return 66;
  if((DWORD)(GetTickCount()-start)>60000){printf("{\"result\":\"FAIL_TIMEOUT_NOT_PASS\"}\n");TerminateProcess(pi.hProcess,99);WaitForSingleObject(pi.hProcess,5000);CloseHandle(pi.hThread);CloseHandle(pi.hProcess);return 67;}
 }
 if(!GetExitCodeProcess(pi.hProcess,&code))return 68;
 CloseHandle(pi.hThread);CloseHandle(pi.hProcess);
 printf("{\"event\":\"EXTERNAL_C1_RESULT\",\"child_exit\":%lu,\"samples\":%u,\"missed\":%u,\"result\":\"%s\",\"final_kind\":\"LAST_SAMPLE_BEFORE_PROCESS_EXIT_NOT_PHASE_BASELINE\"}\n",(unsigned long)code,seq,missed,code==0&&seq?"PASS":"FAIL");
 return code==0&&seq?0:1;
}
