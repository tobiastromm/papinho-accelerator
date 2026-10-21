/* TEST_ONLY: same external C1 process sampler, now handle-bound Mode C evidence.
 * No product launch, Stop, termination, OpenProcess(PID), pipe or admission. */
#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <bcrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
static HANDLE output=INVALID_HANDLE_VALUE;
static int record(const char *format,...) {
 char line[8192];int n;DWORD written;va_list args;
 va_start(args,format);n=vsnprintf(line,sizeof(line),format,args);va_end(args);
 if(n<0||(size_t)n>=sizeof(line))return 0;
 return WriteFile(output,line,(DWORD)n,&written,NULL)&&written==(DWORD)n;
}
static int number(const char *s,unsigned __int64 *v) {
 char *end;unsigned __int64 n;
 if(!s||!*s||*s=='-'||*s=='+')return 0;
 errno=0;n=_strtoui64(s,&end,10);if(errno||*end)return 0;*v=n;return 1;
}
static int label(const char *s) {
 size_t i,n=strlen(s);if(!n||n>127)return 0;
 for(i=0;i<n;++i)if(!((s[i]>='a'&&s[i]<='z')||(s[i]>='A'&&s[i]<='Z')||(s[i]>='0'&&s[i]<='9')||s[i]=='_'||s[i]=='-'||s[i]=='.'))return 0;
 return 1;
}
static unsigned __int64 birth(FILETIME c) {return ((unsigned __int64)c.dwHighDateTime<<32)|c.dwLowDateTime;}
static int close_once(HANDLE *h,const char *name) {
 HANDLE owned=*h;BOOL ok;DWORD error;
 if(!owned||owned==INVALID_HANDLE_VALUE)return 1;
 /* Attempt once. On failure retain the value and report unresolved ownership. */
 ok=CloseHandle(owned);error=ok?0:GetLastError();
 if(ok)*h=NULL;
 (void)record("{\"event\":\"OBSERVER_CLOSE\",\"resource\":\"%s\",\"closed\":%s,\"error\":%lu,\"retained\":%s}\n",name,ok?"true":"false",(unsigned long)error,ok?"false":"true");
 return ok?1:0;
}
static int hash_file(HANDLE file,unsigned char digest[32]) {
 BCRYPT_ALG_HANDLE alg=NULL;BCRYPT_HASH_HANDLE hash=NULL;ULONG size=0,got=0;DWORD read;
 unsigned char object[1024],buffer[65536];LARGE_INTEGER zero;int ok=0;
 zero.QuadPart=0;
 if(!SetFilePointerEx(file,zero,NULL,FILE_BEGIN))return 0;
 if(BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,NULL,0)<0)return 0;
 if(BCryptGetProperty(alg,BCRYPT_OBJECT_LENGTH,(PUCHAR)&size,sizeof(size),&got,0)<0||size>sizeof(object))goto done;
 if(BCryptCreateHash(alg,&hash,object,size,NULL,0,0)<0)goto done;
 for(;;){if(!ReadFile(file,buffer,sizeof(buffer),&read,NULL))goto done;if(!read)break;if(BCryptHashData(hash,buffer,read,0)<0)goto done;}
 if(BCryptFinishHash(hash,digest,32,0)<0)goto done;ok=1;
 done:
 if(hash&&BCryptDestroyHash(hash)<0)ok=0;
 if(BCryptCloseAlgorithmProvider(alg,0)<0)ok=0;
 return ok;
}
static DWORD query_error(void) {DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
static int sample(HANDLE process,unsigned seq,DWORD pid,unsigned __int64 created) {
 PROCESS_MEMORY_COUNTERS_EX m;MEMORYSTATUSEX g;DWORD handles;FILETIME c,e,k,u;
 unsigned __int64 start=GetTickCount64(),end;DWORD errors[4]={0,0,0,0};
 /* Durable start record makes a missing return explicit. No child waits on it. */
 if(!record("{\"event\":\"QUERY_BEGIN\",\"seq\":%u,\"observer_ms\":%llu}\n",seq,start))return 0;
 memset(&m,0,sizeof(m));m.cb=sizeof(m);memset(&g,0,sizeof(g));g.dwLength=sizeof(g);handles=0;
 if(!GetProcessMemoryInfo(process,(PROCESS_MEMORY_COUNTERS*)&m,sizeof(m)))errors[0]=query_error();
 if(!GetProcessHandleCount(process,&handles))errors[1]=query_error();
 if(!GlobalMemoryStatusEx(&g))errors[2]=query_error();
 if(!GetProcessTimes(process,&c,&e,&k,&u))errors[3]=query_error();
 else if(birth(c)!=created)errors[3]=ERROR_INVALID_DATA;
 end=GetTickCount64();
 if(errors[0]||errors[1]||errors[2]||errors[3]||end<start){
  (void)record("{\"event\":\"QUERY_INCOMPLETE\",\"seq\":%u,\"errors\":[%lu,%lu,%lu,%lu],\"observer_ms\":%llu}\n",seq,(unsigned long)errors[0],(unsigned long)errors[1],(unsigned long)errors[2],(unsigned long)errors[3],end);return 0;
 }
 return record("{\"event\":\"EXTERNAL_C1_SAMPLE\",\"seq\":%u,\"pid\":%lu,\"creation\":%llu,\"observed_ms\":%llu,\"completed_ms\":%llu,\"private_usage\":%llu,\"working_set\":%llu,\"peak_working_set\":%llu,\"pagefile_usage\":%llu,\"peak_pagefile_usage\":%llu,\"paged_pool_quota_usage\":%llu,\"nonpaged_pool_quota_usage\":%llu,\"handles\":%lu,\"physical_available\":%llu,\"commit_available\":%llu,\"phase_association\":\"INCOMPLETE\",\"provider\":null,\"kernel_max\":null}\n",seq,(unsigned long)pid,created,start,end,(unsigned __int64)m.PrivateUsage,(unsigned __int64)m.WorkingSetSize,(unsigned __int64)m.PeakWorkingSetSize,(unsigned __int64)m.PagefileUsage,(unsigned __int64)m.PeakPagefileUsage,(unsigned __int64)m.QuotaPagedPoolUsage,(unsigned __int64)m.QuotaNonPagedPoolUsage,(unsigned long)handles,(unsigned __int64)g.ullAvailPhys,(unsigned __int64)g.ullAvailPageFile);
}
int main(int argc,char **argv) {
 HANDLE process=NULL,image=INVALID_HANDLE_VALUE;unsigned __int64 h,pid,created,count,interval;
 FILETIME c,e,k,u;DWORD actual,n,wait,flags;char path[MAX_PATH],hexpath[MAX_PATH*2+1],hashhex[65];
 unsigned char first[32],last[32];BY_HANDLE_FILE_INFORMATION before,after;unsigned i;int ok=0,closed=1;
 /* Handle must be an inherited, observer-owned duplicate from a governed launcher.
  * Values are decimal; count/interval are bounded tool inputs, NOT ED11 policy. */
 if(argc!=9||strcmp(argv[1],"--handle")||!number(argv[2],&h)||!number(argv[3],&pid)||!number(argv[4],&created)||!number(argv[7],&count)||!number(argv[8],&interval)||!h||h>(unsigned __int64)(ULONG_PTR)-1||!pid||pid>MAXDWORD||!created||!count||count>4096||interval>60000||!label(argv[5]))return 64;
 process=(HANDLE)(ULONG_PTR)h;
 if(!GetHandleInformation(process,&flags)||GetFileType(process)!=FILE_TYPE_UNKNOWN)return 65;
 output=CreateFileA(argv[6],GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
 if(output==INVALID_HANDLE_VALUE){BOOL released=CloseHandle(process);(void)fprintf(stderr,"OBSERVER_OUTPUT_UNAVAILABLE process_handle_closed=%u error=%lu\n",released?1U:0U,(unsigned long)(released?0:GetLastError()));return released?66:70;}
 if(!record("{\"event\":\"OBSERVER_BEGIN\",\"run\":\"%s\",\"configuration\":null,\"configuration_ref\":null,\"clock\":\"OBSERVER_GETTICKCOUNT64\",\"observer_pid\":%lu,\"lifecycle\":\"PENDING\",\"phase_association\":\"INCOMPLETE\",\"admission\":\"DENY\"}\n",argv[5],(unsigned long)GetCurrentProcessId()))goto done;
 /* An inherited object handle, not PID reopening, establishes lifetime continuity. */
 actual=GetProcessId(process);
 if(actual!=(DWORD)pid||!GetProcessTimes(process,&c,&e,&k,&u)||birth(c)!=created){(void)record("{\"event\":\"INVALID_IDENTITY\"}\n");goto done;}
 wait=WaitForSingleObject(process,0);
 if(wait==WAIT_OBJECT_0){(void)record("{\"event\":\"PROCESS_EXIT_OBSERVED\",\"pid\":%lu,\"creation\":%llu,\"local_teardown\":null}\n",(unsigned long)actual,created);goto done;}
 if(wait!=WAIT_TIMEOUT)goto done;
 n=MAX_PATH;if(!QueryFullProcessImageNameA(process,0,path,&n)||!n||n>=MAX_PATH)goto done;
 for(i=0;i<n;++i)(void)sprintf_s(hexpath+2*i,3,"%02x",(unsigned char)path[i]);hexpath[2*n]=0;
 /* Hash bytes at the OS-observed image path, denying write/delete until closure.
  * Association is NOT attestation of mapped pages or all provider modules. */
 image=CreateFileA(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 if(image==INVALID_HANDLE_VALUE||!GetFileInformationByHandle(image,&before)||!hash_file(image,first))goto done;
 for(i=0;i<32;++i)(void)sprintf_s(hashhex+2*i,3,"%02x",first[i]);hashhex[64]=0;
 if(!record("{\"event\":\"PROCESS_IDENTITY\",\"pid\":%lu,\"creation\":%llu,\"image_path_acp_hex\":\"%s\",\"observed_file_sha256\":\"%s\",\"image_limit\":\"PATH_FILE_NOT_PAGES_OR_DLL_ATTESTATION\"}\n",(unsigned long)actual,created,hexpath,hashhex))goto done;
 for(i=1;i<=(unsigned)count;++i){
  wait=WaitForSingleObject(process,0);
  if(wait==WAIT_OBJECT_0){(void)record("{\"event\":\"PROCESS_EXIT_OBSERVED\",\"local_teardown\":null}\n");break;}
  if(wait!=WAIT_TIMEOUT||!sample(process,i,actual,created))goto done;
  if(i<(unsigned)count&&interval)Sleep((DWORD)interval);
 }
 if(!GetFileInformationByHandle(image,&after)||before.dwVolumeSerialNumber!=after.dwVolumeSerialNumber||before.nFileIndexHigh!=after.nFileIndexHigh||before.nFileIndexLow!=after.nFileIndexLow||before.nFileSizeHigh!=after.nFileSizeHigh||before.nFileSizeLow!=after.nFileSizeLow||CompareFileTime(&before.ftLastWriteTime,&after.ftLastWriteTime)||!hash_file(image,last)||memcmp(first,last,32))goto done;
 ok=1;
 done:
 closed=close_once(&image,"observed_image_file")&&closed;
 closed=close_once(&process,"observer_process_handle")&&closed;
 (void)record("{\"event\":\"OBSERVER_END\",\"capture_complete\":%s,\"lifecycle\":\"%s\",\"phase_association\":\"INCOMPLETE\",\"ED11_PASS\":false,\"native_qualified\":false,\"admission\":\"DENY\"}\n",ok?"true":"false",closed?"HANDLE_CLOSURES_CONFIRMED":"RETAINED_CLOSE_UNCONFIRMED");
 /* Closing observer output is also factual; failure is nonzero, never retried. */
 if(!CloseHandle(output))return 70;
 return ok&&closed?0:1;
}