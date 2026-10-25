/* Isolated resource experiment. NO SCM, operational intent, LAB paths or product entry. */
#define WIN32_LEAN_AND_MEAN 1
#define _WIN32_WINNT 0x0603
#include <windows.h>
#include <processsnapshot.h>
#include <tlhelp32.h>
#include <aclapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#ifndef FIXTURE_SELECTED
#error Choose profile explicitly
#endif
static struct {DWORD thread;unsigned retained;} native;
static unsigned stop_requested,inject_close,aux_close_calls;
static int papacc_server_console_win32_stop_requested(void){return stop_requested!=0;}
static void papacc_server_console_win32_request_stop(void){stop_requested=1;}
static BOOL WINAPI fixture_aux_close(HANDLE h);
#include "../../../tools/qualification/scm-aux-reservation-core.inc"
#define CloseHandle fixture_aux_close
#include "../../../tools/qualification/scm-aux-reservation-bridge.inc"
#undef CloseHandle
static BOOL WINAPI fixture_aux_close(HANDLE h)
{++aux_close_calls;if(inject_close){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return CloseHandle(h);}

typedef struct {HANDLE h;const char *name,*kind;unsigned generation,extra;ULONGLONG acquired,last_use;} REF;
static REF ref[64];
static WCHAR root[1024],paths[32][1200];
static unsigned seq,peak_named,peak_app,raw_peak,first,retained,with_pss,child_started,child_quiescent;
static HPSS snapshot;static HPSSWALK marker;static PROCESS_INFORMATION child;
static const char *phase="INIT",*scenario;
static DWORD pid;static unsigned external_ack;
static ULONGLONG now(void){LARGE_INTEGER n;QueryPerformanceCounter(&n);return (ULONGLONG)n.QuadPart;}
static DWORD error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
static DWORD fail(DWORD e){if(!first)first=e?e:ERROR_GEN_FAILURE;return first;}
static void event(const char *op,unsigned i)
{REF *r=&ref[i];printf("REF seq=%u tick=%llu qpc=%llu phase=%s op=%s slot=%u generation=%u name=%s kind=%s handle=%p extra=%u acquired=%llu last_use=%llu\n",++seq,GetTickCount64(),now(),phase,op,i,r->generation,r->name,r->kind,(void *)r->h,r->extra,r->acquired,r->last_use);}
static DWORD identity(unsigned i)
{
 REF *r=&ref[i];FILETIME ct,et,kt,ut;
 if(!strcmp(r->kind,"File")||!strcmp(r->kind,"Directory")){
  BY_HANDLE_FILE_INFORMATION f;if(GetFileType(r->h)!=FILE_TYPE_DISK||!GetFileInformationByHandle(r->h,&f)||(f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return fail(error());
  printf("IDENTITY slot=%u generation=%u type=File disk=1 native=%lu:%lu:%lu directory=%u links=%lu\n",i,r->generation,(unsigned long)f.dwVolumeSerialNumber,(unsigned long)f.nFileIndexHigh,(unsigned long)f.nFileIndexLow,(unsigned)((f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0),(unsigned long)f.nNumberOfLinks);
 }else if(!strcmp(r->kind,"Process")){
  if(!GetProcessTimes(r->h,&ct,&et,&kt,&ut)||!GetProcessId(r->h))return fail(error());
  printf("IDENTITY slot=%u generation=%u type=Process pid=%lu creation=%lu:%lu\n",i,r->generation,(unsigned long)GetProcessId(r->h),(unsigned long)ct.dwHighDateTime,(unsigned long)ct.dwLowDateTime);
 }else if(!strcmp(r->kind,"Thread")){
  if(!GetThreadTimes(r->h,&ct,&et,&kt,&ut)||!GetThreadId(r->h))return fail(error());
  printf("IDENTITY slot=%u generation=%u type=Thread tid=%lu pid=%lu creation=%lu:%lu\n",i,r->generation,(unsigned long)GetThreadId(r->h),(unsigned long)GetProcessIdOfThread(r->h),(unsigned long)ct.dwHighDateTime,(unsigned long)ct.dwLowDateTime);
 }else if(!strcmp(r->kind,"Token")){
  TOKEN_STATISTICS s;DWORD n;if(!GetTokenInformation(r->h,TokenStatistics,&s,sizeof(s),&n))return fail(error());
  printf("IDENTITY slot=%u generation=%u type=Token token_id=%ld:%lu token_type=%u\n",i,r->generation,(long)s.TokenId.HighPart,(unsigned long)s.TokenId.LowPart,(unsigned)s.TokenType);
 }else printf("IDENTITY slot=%u generation=%u type=%s source=DOCUMENTED_CREATOR_API_NOT_NTQUERYOBJECT\n",i,r->generation,r->kind);
 return 0;
}
static DWORD adopt(unsigned i,HANDLE h,const char *name,const char *kind,unsigned extra)
{
 if(i>=64||ref[i].h||!h||h==INVALID_HANDLE_VALUE){retained=1;return fail(ERROR_INVALID_STATE);}
 ref[i].h=h;ref[i].name=name;ref[i].kind=kind;ref[i].extra=extra;++ref[i].generation;ref[i].acquired=ref[i].last_use=now();event("ACQUIRE",i);return identity(i);
}
static HANDLE use(unsigned i){ref[i].last_use=now();return ref[i].h;}
static DWORD close_ref(unsigned i)
{if(!ref[i].h)return 0;if(retained||native.retained)return fail(ERROR_INVALID_STATE);event("CLOSE_ATTEMPT",i);if(!CloseHandle(ref[i].h)){retained=1;return fail(error());}event("CLOSED",i);ref[i].h=NULL;return 0;}
static DWORD sample(const char *name)
{
 DWORD handles=0,child_handles=0;unsigned i,own=0,extra=0,aux=0,child_metric=0;
 phase=name;
 if(!GetProcessHandleCount(GetCurrentProcess(),&handles))return fail(error());
 for(i=0;i<64;i++)if(ref[i].h){++own;if(ref[i].extra)++extra;}
 for(i=0;i<2;i++)if(lab_aux_owner.handle[i])++aux;
 if(ref[29].h)child_metric=(unsigned)(GetProcessHandleCount(ref[29].h,&child_handles)!=0);
 own+=aux;if(own>peak_named)peak_named=own;if(own-extra>peak_app)peak_app=own-extra;if(handles>raw_peak)raw_peak=handles;
 printf("SAMPLE seq=%u phase=%s tick=%llu qpc=%llu pid=%lu handles=%lu named=%u application_named=%u fixture_only=%u aux=%u snapshot=%u marker=%u first=%u retained=%u child_metric=%u child_handles=%lu\n",++seq,phase,GetTickCount64(),now(),(unsigned long)pid,(unsigned long)handles,own,own-extra,extra,aux,snapshot!=NULL,marker!=NULL,first,retained||native.retained,child_metric,(unsigned long)child_handles);fflush(stdout);if(external_ack&&getchar()!='\n')return fail(ERROR_BROKEN_PIPE);return 0;
}
static DWORD make_file(unsigned i,unsigned p,const char *name)
{
 DWORD n;HANDLE h;const char data[]="SYNTHETIC_ONLY_NO_AUTHORITY_NO_LAB\r\n";
 h=CreateFileW(paths[p],GENERIC_READ|GENERIC_WRITE|READ_CONTROL,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(h==INVALID_HANDLE_VALUE)return fail(error());
 if(adopt(i,h,name,"File",0))return first;
 if(!WriteFile(use(i),data,sizeof(data),&n,NULL)||n!=sizeof(data)||!FlushFileBuffers(use(i)))return fail(error());
 return 0;
}
static DWORD dup(unsigned i,HANDLE h,const char *name,const char *kind)
{HANDLE out=NULL;if(!DuplicateHandle(GetCurrentProcess(),h,GetCurrentProcess(),&out,0,FALSE,DUPLICATE_SAME_ACCESS))return fail(error());return adopt(i,out,name,kind,0);}
static DWORD token(unsigned i,HANDLE process,const char *name,int duplicate)
{HANDLE h=NULL;if(duplicate){if(!DuplicateToken(process,SecurityImpersonation,&h))return fail(error());}else if(!OpenProcessToken(process,TOKEN_QUERY|TOKEN_DUPLICATE,&h))return fail(error());return adopt(i,h,name,"Token",0);}
static DWORD validate_file(unsigned i)
{
 PSECURITY_DESCRIPTOR a=NULL,b=NULL;BY_HANDLE_FILE_INFORMATION before,after;DWORD e=0,n;SECURITY_DESCRIPTOR_CONTROL control;DWORD revision;
 if(!GetFileInformationByHandle(use(i),&before))return fail(error());
 e=GetSecurityInfo(use(i),SE_FILE_OBJECT,7,NULL,NULL,NULL,NULL,&a);if(e)return fail(e);
 n=GetSecurityDescriptorLength(a);
 e=GetSecurityInfo(use(i),SE_FILE_OBJECT,7,NULL,NULL,NULL,NULL,&b);
 if(!e&&(!IsValidSecurityDescriptor(a)||!GetSecurityDescriptorControl(a,&control,&revision)||!(control&SE_SELF_RELATIVE)||n!=GetSecurityDescriptorLength(b)||memcmp(a,b,n)||!GetFileInformationByHandle(use(i),&after)||before.dwVolumeSerialNumber!=after.dwVolumeSerialNumber||before.nFileIndexHigh!=after.nFileIndexHigh||before.nFileIndexLow!=after.nFileIndexLow))e=ERROR_INVALID_DATA;
 printf("RAW_REVALIDATION slot=%u same_original_handle=1 descriptor_bytes=%lu result=%lu fixture_policy_not_product_protection\n",i,(unsigned long)n,(unsigned long)e);
 if(LocalFree(a)){retained=1;return fail(ERROR_INVALID_HANDLE);}if(b&&LocalFree(b)){retained=1;return fail(ERROR_INVALID_HANDLE);}return e?fail(e):0;
}
static DWORD a1_probe(const char *mode)
{
 A1_LEASE a,b,c;HANDLE borrowed=NULL;DWORD e;unsigned before;BY_HANDLE_FILE_INFORMATION id;
 if(!strcmp(mode,"acquire-fail")){
  e=lab_aux_file(&a,paths[31],GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT);
  printf("A1_ACQUIRE_FAILURE code=%lu owned=%u finish=%u\n",(unsigned long)e,lab_aux_owner.handle[0]!=NULL,a1_finish(&lab_aux_owner));
  return e==ERROR_FILE_NOT_FOUND&&a1_finish(&lab_aux_owner)?fail(e):fail(ERROR_INVALID_DATA);
 }
 e=lab_aux_file(&a,paths[0],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT);if(e)return fail(e);
 printf("A1_LEASE slot=%u generation=%llu acquisition=REAL\n",a.slot,a.ticket);if(sample("B_A1_ONE"))return first;
 if(!strcmp(mode,"close-unknown")||!strcmp(mode,"saturation")){
  if((e=lab_aux_file(&b,paths[0],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT)))return fail(e);
  if(sample("B_A1_TWO"))return first;
  if(!strcmp(mode,"saturation")){
   e=a1_reserve(&lab_aux_owner,&c);if(e!=ERROR_TOO_MANY_OPEN_FILES)return fail(ERROR_INVALID_DATA);
   (void)lab_aux_sync(e);if(lab_aux_close_preserving(a,0)!=e||lab_aux_close_preserving(b,0)!=e||!a1_finish(&lab_aux_owner))return fail(ERROR_INVALID_DATA);
   return fail(e);
  }
  inject_close=1;e=lab_aux_close_preserving(a,1816);before=aux_close_calls;
  if(e!=1816||!native.retained||a1_state(&lab_aux_owner,a.slot)!=A1_UNKNOWN||!lab_aux_owner.handle[a.slot]||a1_state(&lab_aux_owner,b.slot)!=A1_OWNED)return fail(ERROR_INVALID_DATA);
  e=lab_aux_token(&c,GetCurrentProcess(),0,TOKEN_QUERY);
  (void)lab_aux_close_preserving(b,0);
  if(e!=1816||aux_close_calls!=before)return fail(ERROR_INVALID_DATA);
  printf("A1_UNKNOWN injected_close_not_called=1 first=1816 first_close=%lu no_second_close=1 remaining_owned=1 freeze=1\n",(unsigned long)lab_aux_owner.first_close_error);
  retained=1;return fail(1816);
 }
 if((e=lab_aux_borrow(a,&borrowed)))return fail(e);
 if(!GetFileInformationByHandle(borrowed,&id)){e=error();(void)lab_aux_borrow_end(e);return fail(lab_aux_close_preserving(a,e));}
 printf("A1_BORROW real_file=1 busy=%u slot=%u generation=%llu\n",(unsigned)((lab_aux_owner.state&A1_BUSY)!=0),a.slot,a.ticket);
 e=lab_aux_borrow_end(0);e=lab_aux_close_preserving(a,e);if(e)return fail(e);return sample("B_A1_CLOSED");
}
static DWORD observe_pss(HANDLE target,DWORD target_pid,const char *where)
{
 DWORD e,before=0,after=0;PSS_THREAD_ENTRY t;unsigned threads=0;
 if(!with_pss)return 0;
 if(snapshot||marker||ref[55].h||retained||native.retained)return fail(ERROR_INVALID_STATE);
 phase=where;if(!GetProcessHandleCount(GetCurrentProcess(),&before))return fail(error());
 e=PssCaptureSnapshot(target,(PSS_CAPTURE_FLAGS)(PSS_CAPTURE_THREADS|PSS_CAPTURE_THREAD_CONTEXT),CONTEXT_CONTROL,&snapshot);
 if(e){if(snapshot)retained=1;return fail(e);}if(sample("PSS_CAPTURED"))return first;
 e=PssWalkMarkerCreate(NULL,&marker);if(e)return fail(e);if(sample("PSS_MARKER"))return first;
 for(;;){FILETIME ct,et,kt,ut;HANDLE h;
  e=PssWalkSnapshot(snapshot,PSS_WALK_THREADS,marker,&t,sizeof(t));if(e==ERROR_NO_MORE_ITEMS)break;if(e)return fail(e);
  if(t.ExitStatus!=STILL_ACTIVE)continue;
  if(t.ProcessId!=target_pid||!t.ContextRecord||t.SizeOfContextRecord<sizeof(CONTEXT))return fail(ERROR_INVALID_DATA);
  h=OpenThread(THREAD_QUERY_INFORMATION|SYNCHRONIZE,FALSE,t.ThreadId);if(!h)return fail(error());
  if(adopt(55,h,"pss_thread","Thread",0))return first;
  if(GetProcessIdOfThread(use(55))!=target_pid||!GetThreadTimes(use(55),&ct,&et,&kt,&ut)||memcmp(&ct,&t.CreateTime,sizeof(ct))||WaitForSingleObject(use(55),0)!=WAIT_TIMEOUT)return fail(ERROR_INVALID_DATA);
  if(sample("PSS_THREAD"))return first;if(close_ref(55))return first;++threads;
 }
 e=PssWalkMarkerFree(marker);if(e){retained=1;return fail(e);}marker=NULL;if(sample("PSS_MARKER_CLOSED"))return first;
 e=PssFreeSnapshot(GetCurrentProcess(),snapshot);if(e){retained=1;return fail(e);}snapshot=NULL;
 if(!GetProcessHandleCount(GetCurrentProcess(),&after))return fail(error());
 printf("PSS_RESULT site=%s target_pid=%lu threads=%u before=%lu after=%lu capture_flags=THREADS_THREAD_CONTEXT context=CONTROL opaque_bytes=UNKNOWN\n",where,(unsigned long)target_pid,threads,(unsigned long)before,(unsigned long)after);
 return sample(where);
}
static DWORD census(void)
{HANDLE h;PROCESSENTRY32W p;unsigned seen=0;h=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);if(h==INVALID_HANDLE_VALUE)return fail(error());if(adopt(54,h,"census","ToolhelpSnapshot",0))return first;ZeroMemory(&p,sizeof(p));p.dwSize=sizeof(p);if(!Process32FirstW(use(54),&p))return fail(error());do{if(p.th32ProcessID==pid)seen=1;}while(Process32NextW(use(54),&p));if(GetLastError()!=ERROR_NO_MORE_FILES||!seen)return fail(ERROR_INVALID_DATA);if(sample("C_CENSUS_HELD"))return first;return close_ref(54);}
static DWORD copy_image(void)
{
 char data[128];DWORD n,w;LARGE_INTEGER zero;BY_HANDLE_FILE_INFORMATION old,newid;HANDLE h;
 if(make_file(28,28,"image_writer"))return first;
 zero.QuadPart=0;if(!SetFilePointerEx(use(0),zero,NULL,FILE_BEGIN)||!SetFilePointerEx(use(28),zero,NULL,FILE_BEGIN)||!ReadFile(use(0),data,sizeof(data),&n,NULL)||!WriteFile(use(28),data,n,&w,NULL)||n!=w||!FlushFileBuffers(use(28))||!GetFileInformationByHandle(use(28),&old))return fail(error());
 if(sample("E_IMAGE_WRITER"))return first;if(close_ref(28))return first;
 h=CreateFileW(paths[28],GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(h==INVALID_HANDLE_VALUE)return fail(error());
 if(adopt(28,h,"image_reader","File",0))return first;
 if(!GetFileInformationByHandle(use(28),&newid)||old.dwVolumeSerialNumber!=newid.dwVolumeSerialNumber||old.nFileIndexHigh!=newid.nFileIndexHigh||old.nFileIndexLow!=newid.nFileIndexLow)return fail(ERROR_INVALID_DATA);
 if(validate_file(28))return first;return sample("E_IMAGE_REOPENED");
}
static DWORD supervise(void)
{
 WCHAR exe[1200],command[1600];SECURITY_ATTRIBUTES sa={sizeof(sa),NULL,TRUE};STARTUPINFOW startup;HANDLE h;DWORD n=GetModuleFileNameW(NULL,exe,1200);
 if(!n||n>=1200)return fail(error());
 h=CreateEventW(&sa,TRUE,FALSE,NULL);if(!h)return fail(error());if(adopt(40,h,"fixture_stop_event","Event",1))return first;
 ZeroMemory(&startup,sizeof(startup));startup.cb=sizeof(startup);ZeroMemory(&child,sizeof(child));
 h=CreateEventW(&sa,TRUE,FALSE,NULL);if(!h)return fail(error());if(adopt(42,h,"fixture_ready_event","Event",1))return first;
 if(swprintf_s(command,1600,L"\"%s\" --worker %llu %llu",exe,(ULONGLONG)(ULONG_PTR)ref[40].h,(ULONGLONG)(ULONG_PTR)ref[42].h)<0)return fail(ERROR_INVALID_DATA);
 if(!CreateProcessW(exe,command,NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,root,&startup,&child))return fail(error());child_started=1;
 if(adopt(29,child.hProcess,"synthetic_process","Process",0)||adopt(41,child.hThread,"fixture_creation_thread","Thread",1))return first;
 if(sample("G_PROCESS_CREATED"))return first;
 if(WaitForSingleObject(use(42),5000)!=WAIT_OBJECT_0){retained=1;return fail(ERROR_TIMEOUT);}
 if(close_ref(42)||close_ref(41))return first;
 if(WaitForSingleObject(use(29),0)!=WAIT_TIMEOUT)return fail(ERROR_INVALID_STATE);
 printf("PROCESS_ALIVE pid=%lu signaled=0 termination=NOT_CLAIMED\n",(unsigned long)child.dwProcessId);return sample("G_SUPERVISED");
}
static DWORD live_validation(void)
{
 HANDLE h;MODULEENTRY32W m;THREADENTRY32 t;unsigned found=0;
 if(WaitForSingleObject(use(29),0)!=WAIT_TIMEOUT)return fail(ERROR_PROCESS_ABORTED);
 h=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,child.dwProcessId);if(h==INVALID_HANDLE_VALUE)return fail(error());
 if(adopt(54,h,"module_snapshot","ToolhelpSnapshot",0))return first;ZeroMemory(&m,sizeof(m));m.dwSize=sizeof(m);if(!Module32FirstW(use(54),&m)){DWORD z=error();printf("NATIVE_FAILURE api=Module32FirstW code=%lu\n",(unsigned long)z);return fail(z);}if(sample("H_MODULE_HELD"))return first;if(close_ref(54))return first;
 if(token(30,use(29),"process_token",0)||token(31,use(30),"access_token",1))return first;
 if(validate_file(28))return first;
 h=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);if(h==INVALID_HANDLE_VALUE)return fail(error());if(adopt(32,h,"thread_snapshot","ToolhelpSnapshot",0))return first;
 ZeroMemory(&t,sizeof(t));t.dwSize=sizeof(t);if(!Thread32First(use(32),&t))return fail(error());
 do{if(t.th32OwnerProcessID==child.dwProcessId){h=OpenThread(THREAD_QUERY_INFORMATION|SYNCHRONIZE,FALSE,t.th32ThreadID);if(!h)return fail(error());if(adopt(54,h,"live_thread","Thread",0))return first;if(sample("H_LIVE_THREAD"))return first;if(close_ref(54))return first;found=1;break;}}while(Thread32Next(use(32),&t));
 if(!found)return fail(ERROR_INVALID_DATA);
 /* Same current lifetime shape as source: token/access/snapshot remain OWNED to J. */
 return sample("H_LIVE_RETURN_THREE_RETAINED");
}
static DWORD stop_child(void)
{
 DWORD code;if(!child_started)return 0;
 printf("SYNTHETIC_STOP requested=1 accepted=NOT_A_SERVICE_CONTROL\n");
 if(!SetEvent(use(40)))return fail(error());
 if(WaitForSingleObject(use(29),5000)!=WAIT_OBJECT_0){retained=1;return fail(ERROR_TIMEOUT);}
 if(!GetExitCodeProcess(use(29),&code)||code!=0){retained=1;return fail(ERROR_INVALID_DATA);}
 child_quiescent=1;printf("SYNTHETIC_STOP completed=1 original_process_handle_signaled=1 exit=%lu service_quiescence=NOT_CLAIMED\n",(unsigned long)code);return sample("I_SYNTHETIC_STOPPED");
}
static int all_closed(void)
{unsigned i;if(retained||native.retained||snapshot||marker||!a1_finish(&lab_aux_owner))return 0;for(i=0;i<64;i++)if(ref[i].h)return 0;return !child_started||child_quiescent;}
static DWORD cleanup(void)
{unsigned i;if(retained||native.retained||snapshot||marker||!a1_finish(&lab_aux_owner))return fail(ERROR_INVALID_STATE);if(child_started&&!child_quiescent)return fail(ERROR_INVALID_STATE);phase="J_CLOSE";for(i=64;i>0;i--)if(close_ref(i-1))return first;return sample("J_CLOSED");}
static int worker(const WCHAR *number,const WCHAR *ready_number)
{HANDLE h=(HANDLE)(ULONG_PTR)_wcstoui64(number,NULL,10),ready=(HANDLE)(ULONG_PTR)_wcstoui64(ready_number,NULL,10);DWORD wait;
 if(!SetEvent(ready)||!CloseHandle(ready))return 14;
 wait=WaitForSingleObject(h,15000);if(!CloseHandle(h))return 13;return wait==WAIT_OBJECT_0?0:12;}
int wmain(int argc,WCHAR **argv)
{
 unsigned i;DWORD e=0,base=0,after=0;LARGE_INTEGER freq;char chosen[64];
 if(argc==4&&!wcscmp(argv[1],L"--worker"))return worker(argv[2],argv[3]);
 if(argc!=4||WideCharToMultiByte(CP_UTF8,0,argv[2],-1,chosen,sizeof(chosen),NULL,NULL)<=0)return 64;
 external_ack=GetEnvironmentVariableW(L"NATIVE_FIXTURE_ACK",NULL,0)>0;scenario=chosen;with_pss=(unsigned)(_wtoi(argv[3])!=0);pid=GetCurrentProcessId();native.thread=GetCurrentThreadId();QueryPerformanceFrequency(&freq);
 {static const WCHAR prefix[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\native-c-positive-lifetime-20261024\\runs\\";
  DWORD length=GetFullPathNameW(argv[1],1024,root,NULL);
  if(!length||length>=1024||_wcsnicmp(root,prefix,wcslen(prefix))||GetFileAttributesW(root)!=FILE_ATTRIBUTE_DIRECTORY)return 65;
 }
 printf("FIXTURE selected=%u config=%s pid=%lu scenario=%s pss=%u qpc_frequency=%llu NO_SCM_NO_LAB_NO_OPERATIONAL_AUTHORITY\n",(unsigned)FIXTURE_SELECTED,
#ifdef NDEBUG
 "Release",
#else
 "Debug",
#endif
 (unsigned long)pid,scenario,with_pss,(ULONGLONG)freq.QuadPart);fflush(stdout);
 for(i=0;i<32;i++)if(swprintf_s(paths[i],1200,L"%s\\fixture-%02u.dat",root,i)<0)return 66;
 (void)lab_aux_console;(void)lab_aux_borrow_pair;(void)a1_cancel;(void)a1_retain;
 if(sample("A_BASELINE"))goto done;
 if(!GetProcessHandleCount(GetCurrentProcess(),&base)){e=fail(error());goto done;}for(i=0;i<128;i++)if(!GetProcessHandleCount(GetCurrentProcess(),&after)){e=fail(error());goto done;}
 printf("OBSERVER_READS count=128 before=%lu after=%lu self_handle_acquisitions=0 external_observer_handles=SEPARATE_TABLE\n",(unsigned long)base,(unsigned long)after);
 for(i=0;i<6;i++)if(make_file(i,i,"common_file"))goto done;
 if(token(6,GetCurrentProcess(),"base_operator_token",0)||make_file(7,7,"approval_synthetic"))goto done;
 {HANDLE h=CreateMutexW(NULL,FALSE,NULL);if(!h){fail(error());goto done;}if(adopt(8,h,"fixture_unnamed_mutex","Mutex",0))goto done;}
 if(sample("A_COMMON_NINE"))goto done;
 phase="B_BIND";
 if(!strcmp(scenario,"acquire-fail")||!strcmp(scenario,"close-unknown")||!strcmp(scenario,"saturation")){(void)a1_probe(scenario);goto done;}
 for(i=9;i<14;i++){HANDLE h=CreateFileW(root,FILE_READ_ATTRIBUTES|READ_CONTROL,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(h==INVALID_HANDLE_VALUE){fail(error());goto done;}if(adopt(i,h,"synthetic_root_alias","Directory",0)||validate_file(i))goto done;}
 for(i=14;i<20;i++)if(make_file(i,i,"context_input_synthetic"))goto done;
 if(dup(45,GetCurrentProcess(),"original_actor","Process")||dup(46,GetCurrentThread(),"original_control","Thread")||dup(47,GetCurrentProcess(),"original_auxiliary","Process")||dup(48,use(0),"original_os_file_substitute","File")||token(49,GetCurrentProcess(),"original_actor_token",0))goto done;
 if(dup(20,use(45),"bound_actor","Process")||dup(21,use(46),"bound_control","Thread")||dup(22,use(47),"bound_auxiliary","Process")||dup(23,use(48),"bound_file","File")||dup(24,GetCurrentProcess(),"bound_self","Process")||token(25,GetCurrentProcess(),"bound_actor_token",0)||token(26,GetCurrentProcess(),"bound_auxiliary_token",0))goto done;
 if(a1_probe("clean"))goto done;
 {A1_LEASE a,b;DWORD z;if((z=lab_aux_file(&a,paths[0],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL))||(z=lab_aux_file(&b,paths[0],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL))){fail(z);goto done;}if(sample("B_HANDOFF_TWO_AUX"))goto done;if((z=lab_aux_close_preserving(a,0))||(z=lab_aux_close_preserving(b,0))){fail(z);goto done;}}
 for(i=45;i<50;i++)if(close_ref(i))goto done;
 if(sample("B_BOUND_27")||census()||observe_pss(GetCurrentProcess(),pid,"C_SELF_PSS_CLOSED")||sample("C_ADMITTED_SYNTHETIC_NOT_AUTHORITY"))goto done;
 if(!strcmp(scenario,"stop")){A1_LEASE lease;stop_requested=1;e=lab_aux_file(&lease,paths[27],GENERIC_WRITE,0,CREATE_NEW,FILE_ATTRIBUTE_NORMAL);printf("STOP_BEFORE_SYNTHETIC_EFFECT code=%lu created=%u\n",(unsigned long)e,GetFileAttributesW(paths[27])!=INVALID_FILE_ATTRIBUTES);if(e!=ERROR_OPERATION_ABORTED){fail(ERROR_INVALID_DATA);goto done;}fail(e);goto done;}
 if(!strcmp(scenario,"stale")){ULONGLONG tick=GetTickCount64(),old=tick>=1001?tick-1001:0;e=(tick-old>1000)?ERROR_TIMEOUT:ERROR_INVALID_DATA;printf("STALE_TEST synthetic_oldest_tick=%llu current_tick=%llu code=%lu thresholds_60s_1s_UNCHANGED\n",old,tick,(unsigned long)e);fail(e);goto done;}
 phase="D_SYNTHETIC_INTENT";if(make_file(27,27,"intent_synthetic")||sample("D_SYNTHETIC_INTENT_HELD"))goto done;
 phase="E_COPY";if(copy_image())goto done;
 phase="F_SINK";if(make_file(33,29,"sink_synthetic")||sample("F_SINK_HELD")||close_ref(33)||sample("F_SINK_CLOSED"))goto done;
 if(sample("G_READBACK_NO_SCM")||observe_pss(GetCurrentProcess(),pid,"H_FINAL_PREPROCESS_SAMPLE")||supervise())goto done;
 if(!strcmp(scenario,"terminated")){if(stop_child())goto done;if(WaitForSingleObject(use(29),0)!=WAIT_OBJECT_0){fail(ERROR_INVALID_STATE);goto done;}printf("TERMINATED_PROCESS_MONITOR_DENIED code=%u no_new_token=1\n",ERROR_PROCESS_ABORTED);fail(ERROR_PROCESS_ABORTED);goto done;}
 if(observe_pss(use(29),child.dwProcessId,"H_FIRST_MONITOR")||live_validation())goto done;
 if(!strcmp(scenario,"alive")){printf("ALIVE_REFERENCE_OBLIGATION retained_until_factual_exit=1 state=%lu\n",(unsigned long)WaitForSingleObject(use(29),0));if(WaitForSingleObject(use(29),0)!=WAIT_TIMEOUT){fail(ERROR_INVALID_STATE);goto done;}}
 if(stop_child())goto done;
 done:
 if(first&&!retained&&!native.retained&&child_started&&!child_quiescent)(void)stop_child();
 if(retained||native.retained||snapshot||marker){retained=1;(void)sample("J_RETAINED_UNKNOWN");printf("RESULT scenario=%s first=%u owned_peak=%u app_peak=%u raw_peak=%u retained=1 close=UNPROVEN process_exit_containment=1 NOT_NATIVE_CLOSE_SUCCESS\n",scenario,first,peak_named,peak_app,raw_peak);fflush(stdout);return 2;}
 if(cleanup())e=first;
 printf("RESULT scenario=%s first=%u owned_peak=%u app_peak=%u raw_peak=%u retained=%u own_references_closed=%u child_started=%u child_quiescent=%u service_quiescence=NOT_CLAIMED\n",scenario,first,peak_named,peak_app,raw_peak,retained,(unsigned)all_closed(),child_started,child_quiescent);fflush(stdout);
 if(!all_closed())return 2;
 if(!strcmp(scenario,"clean")||!strcmp(scenario,"alive"))return first?1:0;
 if(!strcmp(scenario,"acquire-fail"))return first==ERROR_FILE_NOT_FOUND?0:1;
 if(!strcmp(scenario,"saturation"))return first==ERROR_TOO_MANY_OPEN_FILES?0:1;
 if(!strcmp(scenario,"stop"))return first==ERROR_OPERATION_ABORTED?0:1;
 if(!strcmp(scenario,"stale"))return first==ERROR_TIMEOUT?0:1;
 if(!strcmp(scenario,"terminated"))return first==ERROR_PROCESS_ABORTED?0:1;
 return e?1:0;
}
