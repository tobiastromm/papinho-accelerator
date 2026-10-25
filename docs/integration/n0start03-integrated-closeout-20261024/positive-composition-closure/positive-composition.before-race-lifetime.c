/* Isolated composition: no product main, no operational mutex/intent/SCM.
 * Existing synthetic A-J body + actual private transport + PSS/stack metadata.
 * All product-policy substitutions stay explicit in the included body. */
#define WIN32_LEAN_AND_MEAN 1
#define _WIN32_WINNT 0x0603
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "positive-composition-body.inc"
static void composition_native_point(const char *api,DWORD before,DWORD after,DWORD code)
{if(after>raw_peak)raw_peak=after;PaccTransportPrintf("COMPOSED_NATIVE api=%s before=%lu after=%lu delta=%ld code=%lu phase=%s SAMPLED_NOT_UNIVERSAL_PEAK\n",api,(unsigned long)before,(unsigned long)after,(long)after-(long)before,(unsigned long)code,phase);}
static BOOL WINAPI composition_create(LPCWSTR app,LPWSTR cmd,LPSECURITY_ATTRIBUTES a,LPSECURITY_ATTRIBUTES b,BOOL inherit,DWORD flags,LPVOID env,LPCWSTR dir,LPSTARTUPINFOW startup,LPPROCESS_INFORMATION pi)
{DWORD before=0,after=0,e;BOOL ok;GetProcessHandleCount(GetCurrentProcess(),&before);ok=CreateProcessW(app,cmd,a,b,inherit,flags,env,dir,startup,pi);e=GetLastError();GetProcessHandleCount(GetCurrentProcess(),&after);composition_native_point("CreateProcessW_synthetic_worker",before,after,ok?0:e);SetLastError(e);return ok;}
static DWORD WINAPI composition_tcp(PVOID table,PDWORD bytes,BOOL order,ULONG family,TCP_TABLE_CLASS type,ULONG reserved)
{DWORD before=0,after=0,e;GetProcessHandleCount(GetCurrentProcess(),&before);e=GetExtendedTcpTable(table,bytes,order,family,type,reserved);GetProcessHandleCount(GetCurrentProcess(),&after);composition_native_point(family==AF_INET?"GetExtendedTcpTable_v4":"GetExtendedTcpTable_v6",before,after,e);return e;}
static DWORD WINAPI composition_udp(PVOID table,PDWORD bytes,BOOL order,ULONG family,UDP_TABLE_CLASS type,ULONG reserved)
{DWORD before=0,after=0,e;GetProcessHandleCount(GetCurrentProcess(),&before);e=GetExtendedUdpTable(table,bytes,order,family,type,reserved);GetProcessHandleCount(GetCurrentProcess(),&after);composition_native_point(family==AF_INET?"GetExtendedUdpTable_v4":"GetExtendedUdpTable_v6",before,after,e);return e;}
static unsigned race_enabled;
typedef BOOL(WINAPI *SAME_OBJECT)(HANDLE,HANDLE);
typedef struct {WCHAR exe[1200];HANDLE private_pipe;DWORD result;} COMPOSITION_RACE;
static int competitor(void){Sleep(20);return 0;}
static DWORD audit_competitor(HANDLE process,HANDLE pipe)
{
 HPSS snap=NULL;HPSSWALK walk=NULL;PSS_HANDLE_ENTRY row;DWORD e,z,found=0;HANDLE candidate=NULL;
 SAME_OBJECT compare;FARPROC addr=GetProcAddress(GetModuleHandleW(L"kernelbase.dll"),"CompareObjectHandles");
 if(!addr)return ERROR_NOT_SUPPORTED;memcpy(&compare,&addr,sizeof(compare));
 e=PssCaptureSnapshot(process,(PSS_CAPTURE_FLAGS)(PSS_CAPTURE_HANDLES|PSS_CAPTURE_HANDLE_BASIC_INFORMATION|PSS_CAPTURE_HANDLE_NAME_INFORMATION|PSS_CAPTURE_HANDLE_TYPE_SPECIFIC_INFORMATION),0,&snap);if(e)return e;
 e=PssWalkMarkerCreate(NULL,&walk);if(e)return e;
 for(;;){ZeroMemory(&row,sizeof(row));e=PssWalkSnapshot(snap,PSS_WALK_HANDLES,walk,&row,sizeof(row));if(e==ERROR_NO_MORE_ITEMS){e=0;break;}if(e)break;
  if(!DuplicateHandle(process,row.Handle,GetCurrentProcess(),&candidate,0,FALSE,DUPLICATE_SAME_ACCESS)){e=error();break;}
  if(compare(candidate,pipe))++found;
  if(!CloseHandle(candidate))return ERROR_INVALID_HANDLE;candidate=NULL;
 }
 z=PssWalkMarkerFree(walk);if(z)return z;z=PssFreeSnapshot(GetCurrentProcess(),snap);if(z)return z;
 if(e)return e;return found?ERROR_ACCESS_DENIED:0;
}
static DWORD WINAPI competing_creates(void *arg)
{
 COMPOSITION_RACE *r=(COMPOSITION_RACE *)arg;unsigned i;
 for(i=0;i<5;i++){STARTUPINFOW si;PROCESS_INFORMATION pi;WCHAR cmd[1400];DWORD e;
  ZeroMemory(&si,sizeof(si));ZeroMemory(&pi,sizeof(pi));si.cb=sizeof(si);_snwprintf_s(cmd,1400,_TRUNCATE,L"\"%ls\" --competitor",r->exe);
  if(!CreateProcessW(r->exe,cmd,NULL,NULL,TRUE,CREATE_SUSPENDED,NULL,NULL,&si,&pi)){r->result=error();return r->result;}
  e=audit_competitor(pi.hProcess,r->private_pipe);
  if(ResumeThread(pi.hThread)==MAXDWORD||WaitForSingleObject(pi.hProcess,10000)!=WAIT_OBJECT_0||!CloseHandle(pi.hThread)||!CloseHandle(pi.hProcess)){r->result=ERROR_INVALID_STATE;return r->result;}
  if(e){r->result=e;return e;}
 }
 return 0;
}
static DWORD composition_checkpoint(const char *name)
{
 COMPOSITION_RACE racers[2];HANDLE threads[2]={NULL,NULL};unsigned i;DWORD e=0;
 if(!race_enabled||strcmp(name,"E_IMAGE_REOPENED"))return 0;
 for(i=0;i<2;i++){ZeroMemory(&racers[i],sizeof(racers[i]));racers[i].private_pipe=GetStdHandle(STD_OUTPUT_HANDLE);if(!GetModuleFileNameW(NULL,racers[i].exe,1200))return error();threads[i]=CreateThread(NULL,0,competing_creates,&racers[i],0,NULL);if(!threads[i])return error();}
 for(i=0;i<2;i++){if(WaitForSingleObject(threads[i],20000)!=WAIT_OBJECT_0||!CloseHandle(threads[i]))return ERROR_INVALID_STATE;if(racers[i].result)e=racers[i].result;}
 if(PaccTransportPrintf("COMPOSED_RACE phase=E_IMAGE_REOPENED competitors=10 private_object_matches=0 result=%lu fixture_observer_cost_NOT_PRODUCT_PEAK\n",(unsigned long)e)<0)return ERROR_WRITE_FAULT;
 return e;
}
int wmain(int argc,WCHAR **argv)
{
 WCHAR exe[1200],args[2400];DWORD e,n,exit_code=0,first_error=0,kept=0,pid=0;ULONGLONG creation=0;BYTE buffer[4096];unsigned final_seen=0;char tail[8192];size_t used=0;
 (void)ig_transport_puts;
 if(argc==2&&!wcscmp(argv[1],L"--competitor"))return competitor();
 if(argc==4&&!wcscmp(argv[1],L"--worker"))return composition_body(argc,argv);
 if(argc==9&&!wcscmp(argv[1],L"--composition-child")&&!wcscmp(argv[5],L"--private-stdout")){
  WCHAR *body_argv[4]={argv[0],argv[2],argv[3],argv[4]};int result;
  e=PaccTransportClient(argv[6],wcstoul(argv[7],NULL,10),_wcstoui64(argv[8],NULL,10));if(e){fprintf(stderr,"COMPOSITION_AUTH_DENIED %lu\n",(unsigned long)e);return 80;}
  race_enabled=(unsigned)(!wcscmp(argv[3],L"race"));if(race_enabled)body_argv[2]=L"clean";
  result=composition_body(4,body_argv);
  e=PaccTransportClientClose();if(e){fprintf(stderr,"COMPOSITION_TRANSPORT_CLOSE_UNKNOWN %lu\n",(unsigned long)e);return 81;}return result;
 }
 if(argc!=4||!GetModuleFileNameW(NULL,exe,1200))return 64;
 if(GetEnvironmentVariableW(L"NATIVE_FIXTURE_ACK",NULL,0))return 65; /* never ReadFile from operational stdin */
 _snwprintf_s(args,2400,_TRUNCATE,L"--composition-child \"%ls\" %ls %ls",argv[1],argv[2],argv[3]);
 if((e=PaccTransportOpen())||(e=PaccTransportCreate(exe,args))||(e=PaccTransportAuthenticate())){fprintf(stderr,"COMPOSITION_PARENT_FAILED first=%lu\n",(unsigned long)e);return 82;}
 do{e=PaccTransportRead(buffer,sizeof(buffer),&n);if(e){fprintf(stderr,"COMPOSITION_READ_FAILED first=%lu NO_RETRY\n",(unsigned long)e);return 83;}
  if(n){size_t keep=n;if(used+keep>=sizeof(tail)){used=0;}memcpy(tail+used,buffer,keep);used+=keep;tail[used]=0;if(strstr(tail,"RESULT scenario="))final_seen=1;if(fwrite(buffer,1,n,stdout)!=n)return 84;}
 }while(n);
 if(PaccTransportWait(&exit_code)||PaccTransportStatus(&pid,&creation,&first_error,&kept)||PaccTransportClose())return 85;
 printf("COMPOSITION_PARENT_CLOSED child=%lu creation=%llu exit=%lu result_received=%u first=%lu retained=%lu NO_OPERATIONAL_AUTHORITY\n",(unsigned long)pid,creation,(unsigned long)exit_code,final_seen,(unsigned long)first_error,(unsigned long)kept);
 return final_seen&&!first_error&&!kept?(int)exit_code:86;
}
