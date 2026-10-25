/* Isolated resource experiment. NO SCM, operational intent, LAB paths or product entry. */
#define WIN32_LEAN_AND_MEAN 1
#define _WIN32_WINNT 0x0603
#include <windows.h>
#include <processsnapshot.h>
#include <tlhelp32.h>
#include <aclapi.h>
#include <psapi.h>
#include <winternl.h>
#include <sddl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "../../../../tools/qualification/scm-n0-private-stdout-native.h"
#define wmain composition_body
static DWORD composition_checkpoint(const char *name);
static BOOL WINAPI composition_create(LPCWSTR,LPWSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,BOOL,DWORD,LPVOID,LPCWSTR,LPSTARTUPINFOW,LPPROCESS_INFORMATION);
static DWORD WINAPI composition_tcp(PVOID,PDWORD,BOOL,ULONG,TCP_TABLE_CLASS,ULONG);
static DWORD WINAPI composition_udp(PVOID,PDWORD,BOOL,ULONG,UDP_TABLE_CLASS,ULONG);
#ifndef FIXTURE_SELECTED
#error Choose profile explicitly
#endif
static struct {DWORD thread;unsigned retained;HANDLE image;PSID service_sid;} native;
static void papacc_server_console_win32_request_stop(void);
#include "../../../../tools/qualification/scm-n0-private-stdout-adapter.inc"
static unsigned stop_requested,inject_close,aux_close_calls;
static int papacc_server_console_win32_stop_requested(void){return stop_requested!=0;}
static void papacc_server_console_win32_request_stop(void){stop_requested=1;}
static BOOL WINAPI fixture_aux_close(HANDLE h);
#include "../../../../tools/qualification/scm-aux-reservation-core.inc"
#define CloseHandle fixture_aux_close
#include "../../../../tools/qualification/scm-aux-reservation-bridge.inc"
#undef CloseHandle
static BOOL WINAPI fixture_aux_close(HANDLE h)
{unsigned i;LARGE_INTEGER stamp;++aux_close_calls;QueryPerformanceCounter(&stamp);for(i=0;i<2;i++)if(lab_aux_owner.handle[i]==h)printf("A1_CLOSE slot=%u generation=%llu handle=%p qpc=%llu injected=%u\n",i,*a1_ticket(&lab_aux_owner,i),(void *)h,(ULONGLONG)stamp.QuadPart,inject_close);if(inject_close){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return CloseHandle(h);}

typedef struct {HANDLE h;const char *name,*kind;unsigned generation,extra;ULONGLONG acquired,last_use;} REF;
static REF ref[64];
static WCHAR root[1024],paths[32][1200];
static DWORD error(void);
static HANDLE lab_token;static TOKEN_STATISTICS lab_stats;static const char *cycle_stage;
static DWORD lab_error(void){return error();}
static unsigned seq,peak_named,peak_app,raw_peak,first,retained,with_pss,child_started,child_quiescent;
static HPSS snapshot;static HPSSWALK marker;static PROCESS_INFORMATION child;
static const char *phase="INIT",*scenario;
static DWORD fixture_pid;static unsigned external_ack;
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
 if(composition_checkpoint(name))return fail(ERROR_INVALID_DATA);
 if(!GetProcessHandleCount(GetCurrentProcess(),&handles))return fail(error());
 for(i=0;i<64;i++)if(ref[i].h){++own;if(ref[i].extra)++extra;}
 for(i=0;i<2;i++)if(lab_aux_owner.handle[i])++aux;
 if(ref[29].h)child_metric=(unsigned)(GetProcessHandleCount(ref[29].h,&child_handles)!=0);
 own+=aux;if(own>peak_named)peak_named=own;if(own-extra>peak_app)peak_app=own-extra;if(handles>raw_peak)raw_peak=handles;
 printf("SAMPLE seq=%u phase=%s tick=%llu qpc=%llu pid=%lu handles=%lu named=%u application_named=%u fixture_only=%u aux=%u snapshot=%u marker=%u first=%u retained=%u child_metric=%u child_handles=%lu\n",++seq,phase,GetTickCount64(),now(),(unsigned long)fixture_pid,(unsigned long)handles,own,own-extra,extra,aux,snapshot!=NULL,marker!=NULL,first,retained||native.retained,child_metric,(unsigned long)child_handles);fflush(stdout);if(external_ack&&getchar()!='\n')return fail(ERROR_BROKEN_PIPE);return 0;
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
 printf("A1_LEASE slot=%u generation=%llu handle=%p qpc=%llu acquisition=REAL\n",a.slot,a.ticket,(void *)lab_aux_owner.handle[a.slot],now());if(sample("B_A1_ONE"))return first;
 if(!strcmp(mode,"close-unknown")||!strcmp(mode,"saturation")){
  if((e=lab_aux_file(&b,paths[0],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT)))return fail(e);
  printf("A1_LEASE slot=%u generation=%llu handle=%p qpc=%llu acquisition=REAL\n",b.slot,b.ticket,(void *)lab_aux_owner.handle[b.slot],now());
  {HANDLE ah=NULL,bh=NULL;BY_HANDLE_FILE_INFORMATION ia,ib;DWORD z=lab_aux_borrow_pair(a,b,&ah,&bh);if(z)return fail(z);if(!GetFileInformationByHandle(ah,&ia)||!GetFileInformationByHandle(bh,&ib)){z=error();(void)lab_aux_borrow_end(z);return fail(z);}
   printf("A1_PAIR_IDENTITIES type=File a=%lu:%lu:%lu b=%lu:%lu:%lu same_opened_object=%u qpc=%llu\n",(unsigned long)ia.dwVolumeSerialNumber,(unsigned long)ia.nFileIndexHigh,(unsigned long)ia.nFileIndexLow,(unsigned long)ib.dwVolumeSerialNumber,(unsigned long)ib.nFileIndexHigh,(unsigned long)ib.nFileIndexLow,(unsigned)(!z&&ia.dwVolumeSerialNumber==ib.dwVolumeSerialNumber&&ia.nFileIndexHigh==ib.nFileIndexHigh&&ia.nFileIndexLow==ib.nFileIndexLow),now());if(lab_aux_borrow_end(z))return fail(z?z:ERROR_INVALID_DATA);}
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
 printf("A1_BORROW real_file=1 busy=%u slot=%u generation=%llu native=%lu:%lu:%lu qpc=%llu\n",(unsigned)((lab_aux_owner.state&A1_BUSY)!=0),a.slot,a.ticket,(unsigned long)id.dwVolumeSerialNumber,(unsigned long)id.nFileIndexHigh,(unsigned long)id.nFileIndexLow,now());
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
  {MEMORY_BASIC_INFORMATION mbi;ULONG_PTR at=(ULONG_PTR)t.ContextRecord->Rsp,allocation;unsigned regions=0;ULONGLONG committed=0;
   if(!at||!VirtualQueryEx(target,(void *)at,&mbi,sizeof(mbi))||mbi.Type!=MEM_PRIVATE||!mbi.AllocationBase)return fail(ERROR_INVALID_DATA);
   allocation=(ULONG_PTR)mbi.AllocationBase;at=allocation;
   for(;;){if(++regions>1024||!VirtualQueryEx(target,(void *)at,&mbi,sizeof(mbi)))return fail(ERROR_INVALID_DATA);if((ULONG_PTR)mbi.AllocationBase!=allocation)break;
    if(mbi.Type!=MEM_PRIVATE||!mbi.RegionSize||at+mbi.RegionSize<=at)return fail(ERROR_INVALID_DATA);
    if(mbi.State==MEM_COMMIT){if(committed>~(ULONGLONG)0-mbi.RegionSize)return fail(ERROR_ARITHMETIC_OVERFLOW);committed+=mbi.RegionSize;}else if(mbi.State!=MEM_RESERVE)return fail(ERROR_INVALID_DATA);at+=mbi.RegionSize;}
   printf("COMPOSED_STACK tid=%lu regions=%u committed_va=%llu MEMORY_METADATA_ONLY\n",(unsigned long)t.ThreadId,regions,committed);
  }
  if(sample("PSS_THREAD"))return first;if(close_ref(55))return first;++threads;
 }
 e=PssWalkMarkerFree(marker);if(e){retained=1;return fail(e);}marker=NULL;if(sample("PSS_MARKER_CLOSED"))return first;
 e=PssFreeSnapshot(GetCurrentProcess(),snapshot);if(e){retained=1;return fail(e);}snapshot=NULL;
 if(!GetProcessHandleCount(GetCurrentProcess(),&after))return fail(error());
 printf("PSS_RESULT site=%s target_pid=%lu threads=%u before=%lu after=%lu capture_flags=THREADS_THREAD_CONTEXT context=CONTROL opaque_bytes=UNKNOWN\n",where,(unsigned long)target_pid,threads,(unsigned long)before,(unsigned long)after);
 return sample(where);
}
static DWORD census(void)
{HANDLE h;PROCESSENTRY32W p;unsigned seen=0;h=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);if(h==INVALID_HANDLE_VALUE)return fail(error());if(adopt(54,h,"census","ToolhelpSnapshot",0))return first;ZeroMemory(&p,sizeof(p));p.dwSize=sizeof(p);if(!Process32FirstW(use(54),&p))return fail(error());do{if(p.th32ProcessID==fixture_pid)seen=1;}while(Process32NextW(use(54),&p));if(GetLastError()!=ERROR_NO_MORE_FILES||!seen)return fail(ERROR_INVALID_DATA);if(sample("C_CENSUS_HELD"))return first;return close_ref(54);}
static HANDLE cycle_file,cycle_source_file,lab_parent,lab_config,cycle_process,cycle_token,cycle_access,cycle_threads,cycle_thread,cycle_dependencies[2];
static PSECURITY_DESCRIPTOR cycle_sd;static LPWSTR cycle_sid_retained;
static BY_HANDLE_FILE_INFORMATION cycle_id;static unsigned cycle_effect;
static WCHAR cycle_path[1200],lab_parent_path[1200];static char cycle_sha[65];
static int lab_append(WCHAR *o,size_t n,const WCHAR *t){size_t a=wcslen(o),b=wcslen(t);if(a+b>=n)return 0;memcpy(o+a,t,(b+1)*sizeof(WCHAR));return 1;}
static int ps_image_same(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b){return a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow;}
/* Harness content check; exact synthetic source/copy bytes, not product hash/provenance. */
static DWORD lab_hash(HANDLE h,const char *expected){char a[128],b[128];DWORD na,nb;LARGE_INTEGER z;z.QuadPart=0;(void)expected;if(!SetFilePointerEx(h,z,NULL,FILE_BEGIN)||!ReadFile(h,a,sizeof(a),&na,NULL)||!SetFilePointerEx(ref[0].h,z,NULL,FILE_BEGIN)||!ReadFile(ref[0].h,b,sizeof(b),&nb,NULL)||na!=nb||memcmp(a,b,na))return ERROR_INVALID_DATA;return 0;}
/* Relative to original synthetic parent, exclusive create; no product leaf/name. */
static void caller_point(const char *api,HANDLE added,HANDLE removed);
static BOOL WINAPI trace_close(HANDLE h);
static DWORD cycle_open_relative(int create){
 typedef NTSTATUS (NTAPI *FN)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
 FN fn;FARPROC proc=GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtCreateFile");UNICODE_STRING name;OBJECT_ATTRIBUTES oa;IO_STATUS_BLOCK io;NTSTATUS st;WCHAR leaf[]=L"fixture-28.dat";DWORD e=lab_aux_gate();if(e)return e;if(cycle_file||!proc)return ERROR_INVALID_STATE;memcpy(&fn,&proc,sizeof(fn));ZeroMemory(&oa,sizeof(oa));ZeroMemory(&io,sizeof(io));name.Buffer=leaf;name.Length=sizeof(leaf)-sizeof(WCHAR);name.MaximumLength=sizeof(leaf);oa.Length=sizeof(oa);oa.RootDirectory=lab_parent;oa.ObjectName=&name;oa.Attributes=OBJ_CASE_INSENSITIVE|0x1000U;oa.SecurityDescriptor=create?cycle_sd:NULL;
 st=fn(&cycle_file,GENERIC_READ|READ_CONTROL|SYNCHRONIZE|(create?GENERIC_WRITE:0),&oa,&io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,create?2U:1U,0x00200060U,NULL,0);if(st<0){if(cycle_file)retained=1;return st==(NTSTATUS)0xc0000035L?ERROR_FILE_EXISTS:ERROR_ACCESS_DENIED;}caller_point(create?"NtCreateFile_CREATE":"NtCreateFile_OPEN",cycle_file,NULL);return 0;
}
static DWORD cycle_files(void){BY_HANDLE_FILE_INFORMATION id;if(!GetFileInformationByHandle(cycle_file,&id)||!ps_image_same(&cycle_id,&id))return ERROR_INVALID_DATA;return lab_hash(cycle_file,cycle_sha);}
#define CloseHandle trace_close
#include "cycle-copy.UNMODIFIED.inc"
#undef CloseHandle
static DWORD copy_image(void){DWORD e;cycle_source_file=ref[0].h;lab_parent=ref[9].h;native.image=ref[0].h;e=cycle_copy();if(cycle_file&&!ref[28].h){HANDLE h=cycle_file;cycle_file=NULL;if(adopt(28,h,"actual_cycle_copy_reader","File",0))return first;cycle_file=h;}printf("ACTUAL_CYCLE_COPY code=%lu effect=%u native_relative=1 content_comparison=SYNTHETIC_NO_PRODUCT_HASH\n",(unsigned long)e,cycle_effect);return e?fail(e):sample("E_IMAGE_REOPENED");}
#define CreateProcessW composition_create
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

#undef CreateProcessW
static void caller_point(const char *api,HANDLE added,HANDLE removed){
 HANDLE v[80];unsigned n=0,i,j,extra=0;DWORD raw=0;
 HANDLE fixed[]={cycle_file,cycle_token,cycle_access,cycle_threads,cycle_thread};
 for(i=0;i<64;i++)if(ref[i].h&&ref[i].h!=removed){v[n++]=ref[i].h;if(ref[i].extra)++extra;}
 for(i=0;i<sizeof(fixed)/sizeof(fixed[0]);i++)if(fixed[i]&&fixed[i]!=removed&&fixed[i]!=INVALID_HANDLE_VALUE){for(j=0;j<n;j++)if(v[j]==fixed[i])break;if(j==n)v[n++]=fixed[i];}
 if(added&&added!=INVALID_HANDLE_VALUE){for(j=0;j<n;j++)if(v[j]==added)break;if(j==n)v[n++]=added;}
 if(!GetProcessHandleCount(GetCurrentProcess(),&raw)){fail(error());return;}
 if(raw>raw_peak)raw_peak=raw;if(n>peak_named)peak_named=n;if(n-extra>peak_app)peak_app=n-extra;
 printf("CALLER_SAMPLE api=%s phase=%s handles=%lu named=%u application_named=%u tick=%llu qpc=%llu added=%p removed=%p\n",api,phase,(unsigned long)raw,n,n-extra,GetTickCount64(),now(),(void *)added,(void *)removed);
}
static HANDLE WINAPI trace_snapshot(DWORD flags,DWORD target){HANDLE h=CreateToolhelp32Snapshot(flags,target);DWORD e=GetLastError();caller_point("CreateToolhelp32Snapshot",h,NULL);SetLastError(e);return h;}
static BOOL WINAPI trace_token(HANDLE h,DWORD access,PHANDLE out){BOOL r=OpenProcessToken(h,access,out);DWORD e=GetLastError();caller_point("OpenProcessToken",r?*out:NULL,NULL);SetLastError(e);return r;}
static BOOL WINAPI trace_duplicate(HANDLE h,SECURITY_IMPERSONATION_LEVEL level,PHANDLE out){BOOL r=DuplicateToken(h,level,out);DWORD e=GetLastError();caller_point("DuplicateToken",r?*out:NULL,NULL);SetLastError(e);return r;}
static HANDLE WINAPI trace_thread(DWORD access,BOOL inherit,DWORD id){HANDLE h=OpenThread(access,inherit,id);DWORD e=GetLastError();caller_point("OpenThread",h,NULL);SetLastError(e);return h;}
static BOOL WINAPI trace_close(HANDLE h){BOOL r=CloseHandle(h);DWORD e=GetLastError();caller_point("CloseHandle",NULL,r?h:NULL);SetLastError(e);return r;}

static unsigned module_step;
static BOOL WINAPI fixture_module_first(HANDLE h,LPMODULEENTRY32W m){MODULEENTRY32W real;ZeroMemory(&real,sizeof(real));real.dwSize=sizeof(real);if(!Module32FirstW(h,&real))return FALSE;module_step=0;wcscpy_s(m->szModule,256,L"libcrypto-3-x64.dll");wcscpy_s(m->szExePath,MAX_PATH,lab_parent_path);wcscat_s(m->szExePath,MAX_PATH,L"\\libcrypto-3-x64.dll");return TRUE;}
static BOOL WINAPI fixture_module_next(HANDLE h,LPMODULEENTRY32W m){(void)h;if(module_step++){SetLastError(ERROR_NO_MORE_FILES);return FALSE;}wcscpy_s(m->szModule,256,L"libssl-3-x64.dll");wcscpy_s(m->szExePath,MAX_PATH,lab_parent_path);wcscat_s(m->szExePath,MAX_PATH,L"\\libssl-3-x64.dll");return TRUE;}
/* Only LAB-specific effective access / endpoint assertion substituted. Actual
 module snapshot, token/group/privilege reads, DuplicateToken and threads stay real. */
#define cycle_access_file fixture_actual_access_file
#include "cycle-access.UNMODIFIED.inc"
#undef cycle_access_file
static DWORD cycle_access_file(HANDLE h,unsigned index){return !strcmp(scenario,"access-denied")?fixture_actual_access_file(h,index):(h?0:ERROR_INVALID_HANDLE);}
#define GetExtendedTcpTable composition_tcp
#define GetExtendedUdpTable composition_udp
#include "cycle-endpoints.UNMODIFIED.inc"
#undef GetExtendedTcpTable
#undef GetExtendedUdpTable
static int cycle_sid_inside(BYTE *b,DWORD n,PSID p)
{
 ULONG_PTR a=(ULONG_PTR)b,v=(ULONG_PTR)p;DWORD z;
 if(v<a||v-a>n||n-(DWORD)(v-a)<8U)return 0;z=8U+4U*((BYTE *)p)[1];return z<=n-(DWORD)(v-a)&&IsValidSid(p)&&GetLengthSid(p)==z;
}

static unsigned ig_n0_positive=1,last_close_calls,last_close_injected;
static HANDLE last_close_original;
static BOOL WINAPI last_use_close(HANDLE h){unsigned wanted=0;++last_close_calls;
 if(!strcmp(scenario,"last-snapshot-unknown"))wanted=1;
 if(!strcmp(scenario,"last-access-unknown"))wanted=2;
 if(!strcmp(scenario,"last-token-unknown"))wanted=3;
 if(wanted==last_close_calls){last_close_original=h;last_close_injected=1;SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 return trace_close(h);
}
#define CloseHandle last_use_close
#include "live-close-helper.CURRENT.inc"
#undef CloseHandle
#define PAPACC_SCM_INSTRUMENTED_LAB 1
#define CreateToolhelp32Snapshot trace_snapshot
#define OpenProcessToken trace_token
#define DuplicateToken trace_duplicate
#define OpenThread trace_thread
#define CloseHandle trace_close
#define Module32FirstW fixture_module_first
#define Module32NextW fixture_module_next
#include "cycle-live.CURRENT.inc"
#undef PAPACC_SCM_INSTRUMENTED_LAB
#undef Module32FirstW
#undef Module32NextW
#undef CreateToolhelp32Snapshot
#undef OpenProcessToken
#undef DuplicateToken
#undef OpenThread
#undef CloseHandle
#include "operator-current.UNMODIFIED.inc"
static BOOL WINAPI self_close(HANDLE h){if(inject_close){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return CloseHandle(h);}
#define CloseHandle self_close
#include "self-token-branch.UNMODIFIED.inc"
#undef CloseHandle
static DWORD live_validation(void){DWORD e;unsigned i;cycle_process=ref[29].h;lab_parent=ref[9].h;lab_config=ref[1].h;cycle_file=ref[28].h;cycle_dependencies[0]=ref[2].h;cycle_dependencies[1]=ref[3].h;wcscpy_s(lab_parent_path,1200,root);if(!GetModuleFileNameW(NULL,cycle_path,1200))return fail(error());phase="H_ACTUAL_CYCLE_LIVE";e=cycle_live(child.dwProcessId);
 if(last_close_injected){unsigned before=last_close_calls;HANDLE *target=cycle_threads?&cycle_threads:(cycle_access?&cycle_access:&cycle_token);DWORD check=cycle_live_close_last(target);
  if(!native.retained||!(lab_aux_owner.state&A1_FROZEN)||!stop_requested||lab_aux_owner.first_error!=ERROR_ACCESS_DENIED||last_close_calls!=before||check!=ERROR_ACCESS_DENIED||*target!=last_close_original)return fail(ERROR_INVALID_DATA);
  printf("LAST_USE_UNKNOWN first=5 calls=%u original_retained=1 freeze=1 stop=1 no_retry=1 NO_NEW_ACQUISITION\n",last_close_calls);retained=1;return fail(e);}
 if(!e&&ig_n0_positive&&(cycle_token||cycle_access||cycle_threads))return fail(ERROR_INVALID_DATA);
 {HANDLE *v[]={&cycle_token,&cycle_access,&cycle_threads,&cycle_thread};const char *names[]={"actual_live_token","actual_live_access","actual_live_snapshot","actual_live_thread"};const char *kinds[]={"Token","Token","ToolhelpSnapshot","Thread"};for(i=0;i<4;i++)if(*v[i]){if(adopt(i==3?54:30+i,*v[i],names[i],kinds[i],0))return first;}}
 printf("ACTUAL_CYCLE_LIVE code=%lu policy_stubs=MODULE_METADATA_ACCESS_ENDPOINTS NOT_SECURITY_QUALIFICATION\n",(unsigned long)e);return e?fail(e):sample("H_LIVE_RETURN");}
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
{unsigned i;if(retained||native.retained||snapshot||marker||!a1_finish(&lab_aux_owner))return fail(ERROR_INVALID_STATE);if(child_started&&!child_quiescent)return fail(ERROR_INVALID_STATE);phase="J_CLOSE";if(native.service_sid){if(LocalFree(native.service_sid))return fail(ERROR_INVALID_HANDLE);native.service_sid=NULL;}for(i=64;i>0;i--)if(close_ref(i-1))return first;return sample("J_CLOSED");}
static int worker(const WCHAR *number,const WCHAR *ready_number)
{HANDLE h=(HANDLE)(ULONG_PTR)_wcstoui64(number,NULL,10),ready=(HANDLE)(ULONG_PTR)_wcstoui64(ready_number,NULL,10);DWORD wait;
 if(!SetEvent(ready)||!CloseHandle(ready))return 14;
 wait=WaitForSingleObject(h,15000);if(!CloseHandle(h))return 13;return wait==WAIT_OBJECT_0?0:12;}
int wmain(int argc,WCHAR **argv)
{
 unsigned i;DWORD e=0,base=0,after=0;LARGE_INTEGER freq;char chosen[64];
 if(argc==4&&!wcscmp(argv[1],L"--worker"))return worker(argv[2],argv[3]);
 if(argc!=4||WideCharToMultiByte(CP_UTF8,0,argv[2],-1,chosen,sizeof(chosen),NULL,NULL)<=0)return 64;
 external_ack=GetEnvironmentVariableW(L"NATIVE_FIXTURE_ACK",NULL,0)>0;scenario=chosen;with_pss=(unsigned)(_wtoi(argv[3])!=0);fixture_pid=GetCurrentProcessId();native.thread=GetCurrentThreadId();QueryPerformanceFrequency(&freq);
 {static const WCHAR prefix[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\n0start03-positive-resource-requalification-20261024\\runs\\";
  DWORD length=GetFullPathNameW(argv[1],1024,root,NULL);
  if(!length||length>=1024||_wcsnicmp(root,prefix,wcslen(prefix))||GetFileAttributesW(root)!=FILE_ATTRIBUTE_DIRECTORY)return 65;
 }
 printf("FIXTURE selected=%u config=%s pid=%lu scenario=%s pss=%u qpc_frequency=%llu NO_SCM_NO_LAB_NO_OPERATIONAL_AUTHORITY\n",(unsigned)FIXTURE_SELECTED,
#ifdef NDEBUG
 "Release",
#else
 "Debug",
#endif
 (unsigned long)fixture_pid,scenario,with_pss,(ULONGLONG)freq.QuadPart);fflush(stdout);
 for(i=0;i<32;i++)if(swprintf_s(paths[i],1200,L"%s\\fixture-%02u.dat",root,i)<0)return 66;
 (void)lab_aux_console;(void)lab_aux_borrow_pair;(void)a1_cancel;(void)a1_retain;
 if(sample("A_BASELINE"))goto done;
 if(!GetProcessHandleCount(GetCurrentProcess(),&base)){e=fail(error());goto done;}for(i=0;i<128;i++)if(!GetProcessHandleCount(GetCurrentProcess(),&after)){e=fail(error());goto done;}
 printf("OBSERVER_READS count=128 before=%lu after=%lu self_handle_acquisitions=0 external_observer_handles=SEPARATE_TABLE\n",(unsigned long)base,(unsigned long)after);
 for(i=0;i<6;i++)if(make_file(i,i,"common_file"))goto done;
 if(token(6,GetCurrentProcess(),"base_operator_token",0)||make_file(7,7,"approval_synthetic"))goto done;
 {HANDLE h=CreateMutexW(NULL,FALSE,NULL);if(!h){fail(error());goto done;}if(adopt(8,h,"fixture_unnamed_mutex","Mutex",0))goto done;}
 if(sample("A_COMMON_NINE"))goto done;
 lab_token=ref[6].h;if(!GetTokenInformation(lab_token,TokenStatistics,&lab_stats,sizeof(lab_stats),&base)){fail(error());goto done;}
 {union {TOKEN_USER u;BYTE b[512];}u;DWORD used;if(!GetTokenInformation(lab_token,TokenUser,&u,sizeof(u),&used)){fail(error());goto done;}native.service_sid=LocalAlloc(LMEM_FIXED,GetLengthSid(u.u.User.Sid));if(!native.service_sid||!CopySid(GetLengthSid(u.u.User.Sid),native.service_sid,u.u.User.Sid)){fail(error());goto done;}}
 if(!strcmp(scenario,"self-unknown"))inject_close=1;
 e=test_after_initial_success(1,1);if(e){fail(e);goto done;}event("CLOSED_BY_ACTUAL_SELF_BRANCH",6);ref[6].h=NULL;
 if(sample("A_SELF_TOKEN_CLOSED"))goto done;
 if(!strcmp(scenario,"token")){++lab_stats.ModifiedId.LowPart;e=lab_operator_current();printf("TOKEN_DIVERGENCE code=%lu before_effect=1\n",(unsigned long)e);fail(e?e:ERROR_INVALID_DATA);goto done;}
 if((e=lab_operator_current())){fail(e);goto done;}
 phase="B_BIND";
 if(!strcmp(scenario,"acquire-fail")||!strcmp(scenario,"close-unknown")||!strcmp(scenario,"saturation")){(void)a1_probe(scenario);goto done;}
 for(i=9;i<14;i++){HANDLE h=CreateFileW(root,FILE_READ_ATTRIBUTES|READ_CONTROL,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(h==INVALID_HANDLE_VALUE){fail(error());goto done;}if(adopt(i,h,"synthetic_root_alias","Directory",0)||validate_file(i))goto done;}
 for(i=14;i<20;i++)if(make_file(i,i,"context_input_synthetic"))goto done;
 if(dup(45,GetCurrentProcess(),"original_actor","Process")||dup(46,GetCurrentThread(),"original_control","Thread")||dup(47,GetCurrentProcess(),"original_auxiliary","Process")||dup(48,use(0),"original_os_file_substitute","File")||token(49,GetCurrentProcess(),"original_actor_token",0))goto done;
 if(dup(20,use(45),"bound_actor","Process")||dup(21,use(46),"bound_control","Thread")||dup(22,use(47),"bound_auxiliary","Process")||dup(23,use(48),"bound_file","File")||dup(24,GetCurrentProcess(),"bound_self","Process")||token(25,GetCurrentProcess(),"bound_actor_token",0)||token(26,GetCurrentProcess(),"bound_auxiliary_token",0))goto done;
 if(a1_probe("clean"))goto done;
 {A1_LEASE a,b;DWORD z;if((z=lab_aux_file(&a,paths[0],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL))||(z=lab_aux_file(&b,paths[0],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL))){fail(z);goto done;}printf("A1_HANDOFF pair_a=%u:%llu:%p pair_b=%u:%llu:%p qpc=%llu\n",a.slot,a.ticket,(void *)lab_aux_owner.handle[a.slot],b.slot,b.ticket,(void *)lab_aux_owner.handle[b.slot],now());if(sample("B_HANDOFF_TWO_AUX"))goto done;if((z=lab_aux_close_preserving(a,0))||(z=lab_aux_close_preserving(b,0))){fail(z);goto done;}}
 for(i=45;i<50;i++)if(close_ref(i))goto done;
 if(sample("B_BOUND_26")||census()||observe_pss(GetCurrentProcess(),fixture_pid,"C_SELF_PSS_CLOSED")||sample("C_ADMITTED_SYNTHETIC_NOT_AUTHORITY"))goto done;
 if(!strcmp(scenario,"stop")){A1_LEASE lease;stop_requested=1;e=lab_aux_file(&lease,paths[27],GENERIC_WRITE,0,CREATE_NEW,FILE_ATTRIBUTE_NORMAL);printf("STOP_BEFORE_SYNTHETIC_EFFECT code=%lu created=%u\n",(unsigned long)e,GetFileAttributesW(paths[27])!=INVALID_FILE_ATTRIBUTES);if(e!=ERROR_OPERATION_ABORTED){fail(ERROR_INVALID_DATA);goto done;}fail(e);goto done;}
 if(!strcmp(scenario,"stale")||!strcmp(scenario,"replay")){ULONGLONG tick=GetTickCount64(),old=tick>=1001?tick-1001:0;e=!strcmp(scenario,"replay")?ERROR_INVALID_STATE:((tick-old>1000)?ERROR_TIMEOUT:ERROR_INVALID_DATA);printf("STALE_TEST synthetic_oldest_tick=%llu current_tick=%llu code=%lu thresholds_60s_1s_UNCHANGED\n",old,tick,(unsigned long)e);fail(e);goto done;}
 phase="D_SYNTHETIC_INTENT";if(make_file(27,27,"intent_synthetic")||sample("D_SYNTHETIC_INTENT_HELD"))goto done;
 phase="E_COPY";if(copy_image())goto done;
 phase="F_SINK";if(make_file(33,29,"sink_synthetic")||sample("F_SINK_HELD")||close_ref(33)||sample("F_SINK_CLOSED"))goto done;
 if(sample("G_READBACK_NO_SCM")||observe_pss(GetCurrentProcess(),fixture_pid,"H_FINAL_PREPROCESS_SAMPLE")||supervise())goto done;
 if(!strcmp(scenario,"terminated")){if(stop_child())goto done;if(WaitForSingleObject(use(29),0)!=WAIT_OBJECT_0){fail(ERROR_INVALID_STATE);goto done;}printf("TERMINATED_PROCESS_MONITOR_DENIED code=%u no_new_token=1\n",ERROR_PROCESS_ABORTED);fail(ERROR_PROCESS_ABORTED);goto done;}
 if(observe_pss(use(29),child.dwProcessId,"H_FIRST_MONITOR")||live_validation()||observe_pss(use(29),child.dwProcessId,"H_POST_LIVE_PSS_CLOSED"))goto done;
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
 if(!strcmp(scenario,"replay"))return first==ERROR_INVALID_STATE?0:1;
 if(!strcmp(scenario,"token"))return first==ERROR_ACCESS_DENIED?0:1;
 if(!strcmp(scenario,"terminated"))return first==ERROR_PROCESS_ABORTED?0:1;
 if(!strcmp(scenario,"access-denied"))return first==ERROR_ACCESS_DENIED?0:1;
 return e?1:0;
}

#undef wmain
#undef printf
#undef puts
#undef fflush
