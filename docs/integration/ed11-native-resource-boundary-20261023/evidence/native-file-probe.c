#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
typedef struct OBS {
 const char *phase;LARGE_INTEGER tick;PROCESS_MEMORY_COUNTERS_EX mem;PERFORMANCE_INFORMATION host;MEMORYSTATUSEX available;IO_COUNTERS io;
 DWORD handles,mem_error,host_error,available_error,handles_error,io_error,stack_error;
 SIZE_T stack_committed,stack_reserved,stack_address_span,stack_pointer_distance;
} OBS;
static DWORD metadata_error;
static OBS samples[64];static unsigned count;static BYTE sd_buffer[1024];static WCHAR normalized[1024];
static void snap(const char *phase)
{
 DWORD saved=GetLastError();OBS *s;NT_TIB *tib=(NT_TIB *)NtCurrentTeb();MEMORY_BASIC_INFORMATION mbi;BYTE marker;ULONG_PTR at,top;unsigned regions=0;
 if(count>=64){SetLastError(saved);return;}s=&samples[count++];s->phase=phase;QueryPerformanceCounter(&s->tick);s->mem.cb=sizeof(s->mem);s->host.cb=sizeof(s->host);s->available.dwLength=sizeof(s->available);
 if(!GetProcessMemoryInfo(GetCurrentProcess(),(PROCESS_MEMORY_COUNTERS *)&s->mem,sizeof(s->mem)))s->mem_error=GetLastError();
 if(!GetPerformanceInfo(&s->host,sizeof(s->host)))s->host_error=GetLastError();
 if(!GlobalMemoryStatusEx(&s->available))s->available_error=GetLastError();
 if(!GetProcessHandleCount(GetCurrentProcess(),&s->handles))s->handles_error=GetLastError();
 if(!GetProcessIoCounters(GetCurrentProcess(),&s->io))s->io_error=GetLastError();
 top=(ULONG_PTR)tib->StackBase;s->stack_pointer_distance=(SIZE_T)(top-(ULONG_PTR)&marker);
 if(!VirtualQuery(&marker,&mbi,sizeof(mbi)))s->stack_error=GetLastError();
 else {
  at=(ULONG_PTR)mbi.AllocationBase;s->stack_address_span=(SIZE_T)(top-at);
  while(at<top&&regions++<64){
   if(!VirtualQuery((void *)at,&mbi,sizeof(mbi))){s->stack_error=GetLastError();break;}
   if(!mbi.RegionSize||at+mbi.RegionSize<=at||at+mbi.RegionSize>top){s->stack_error=ERROR_INVALID_DATA;break;}
   if(mbi.State==MEM_COMMIT)s->stack_committed+=mbi.RegionSize;else if(mbi.State==MEM_RESERVE)s->stack_reserved+=mbi.RegionSize;else{s->stack_error=ERROR_INVALID_DATA;break;}
   at+=mbi.RegionSize;
  }
  if(at!=top&&!s->stack_error)s->stack_error=ERROR_INVALID_DATA;
 }
 SetLastError(saved);
}
static HANDLE WINAPI observed_create(LPCWSTR path,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES security,DWORD creation,DWORD flags,HANDLE template_file)
{
 HANDLE h;DWORD saved,used=0;snap(creation==CREATE_NEW?"before_create_new":"before_open_writer");
 h=CreateFileW(path,access,share,security,creation,flags,template_file);saved=GetLastError();snap(creation==CREATE_NEW?"after_create_new":"after_open_writer");
 if(h!=INVALID_HANDLE_VALUE&&creation==OPEN_EXISTING){
  if(!GetKernelObjectSecurity(h,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,sd_buffer,sizeof(sd_buffer),&used)||!used||used>sizeof(sd_buffer)||!GetFinalPathNameByHandleW(h,normalized,1024,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS)){DWORD e=GetLastError();metadata_error=e?e:ERROR_INVALID_DATA;snap("metadata_query_failed");SetLastError(e);}else snap("after_bounded_sd_and_final_path");
 }
 SetLastError(saved);return h;
}
static BOOL WINAPI observed_write(HANDLE h,LPCVOID b,DWORD n,LPDWORD used,LPOVERLAPPED ov)
{BOOL ok;DWORD saved;snap("before_write");ok=WriteFile(h,b,n,used,ov);saved=GetLastError();snap("after_write");SetLastError(saved);return ok;}
static BOOL WINAPI observed_flush(HANDLE h)
{BOOL ok;DWORD saved;snap("before_flush");ok=FlushFileBuffers(h);saved=GetLastError();snap("after_flush");SetLastError(saved);return ok;}
static BOOL WINAPI observed_close(HANDLE h)
{BOOL ok;DWORD saved;snap("before_close");ok=CloseHandle(h);saved=GetLastError();snap("after_close");SetLastError(saved);return ok;}
static BOOL WINAPI observed_read(HANDLE h,LPVOID b,DWORD n,LPDWORD used,LPOVERLAPPED ov)
{BOOL ok;DWORD saved;snap("before_read_arm");ok=ReadFile(h,b,n,used,ov);saved=GetLastError();snap("after_read_arm");SetLastError(saved);return ok;}
#define CreateFileW observed_create
#define WriteFile observed_write
#define FlushFileBuffers observed_flush
#define CloseHandle observed_close
#define ReadFile observed_read
#define PAPACC_PSR_FILE_CAPSULE_LAB 1
#include "psr-file-capture-lab.inc"
#undef CreateFileW
#undef WriteFile
#undef FlushFileBuffers
#undef CloseHandle
#undef ReadFile
static PSRF_FILE preparer,writer;
static void emit(void)
{
 unsigned i;for(i=0;i<count;i++){
  OBS *s=&samples[i];
  printf("{\"phase\":\"%s\",\"qpc\":%lld,\"memory_error\":%lu,\"host_error\":%lu,\"available_error\":%lu,\"handles_error\":%lu,\"io_error\":%lu,\"stack_error\":%lu,",s->phase,s->tick.QuadPart,s->mem_error,s->host_error,s->available_error,s->handles_error,s->io_error,s->stack_error);
  printf("\"private_bytes\":%zu,\"working_set\":%zu,\"peak_working_set\":%zu,\"commit_charge\":%zu,\"peak_commit_charge\":%zu,\"process_quota_paged\":%zu,\"process_quota_peak_paged\":%zu,\"process_quota_nonpaged\":%zu,\"process_quota_peak_nonpaged\":%zu,\"handles\":%lu,",s->mem.PrivateUsage,s->mem.WorkingSetSize,s->mem.PeakWorkingSetSize,s->mem.PagefileUsage,s->mem.PeakPagefileUsage,s->mem.QuotaPagedPoolUsage,s->mem.QuotaPeakPagedPoolUsage,s->mem.QuotaNonPagedPoolUsage,s->mem.QuotaPeakNonPagedPoolUsage,s->handles);
  printf("\"page_bytes\":%zu,\"host_commit_pages\":%zu,\"host_commit_limit_pages\":%zu,\"host_commit_peak_pages\":%zu,\"host_cache_pages\":%zu,\"host_kernel_paged_pages\":%zu,\"host_kernel_nonpaged_pages\":%zu,\"host_handles\":%lu,\"host_processes\":%lu,\"host_threads\":%lu,\"host_available_physical\":%llu,\"process_available_virtual\":%llu,",s->host.PageSize,s->host.CommitTotal,s->host.CommitLimit,s->host.CommitPeak,s->host.SystemCache,s->host.KernelPaged,s->host.KernelNonpaged,s->host.HandleCount,s->host.ProcessCount,s->host.ThreadCount,s->available.ullAvailPhys,s->available.ullAvailVirtual);
  printf("\"io_read_ops\":%llu,\"io_write_ops\":%llu,\"io_read_bytes\":%llu,\"io_write_bytes\":%llu,\"stack_committed_va\":%zu,\"stack_reserved_va\":%zu,\"stack_address_span\":%zu,\"stack_pointer_distance\":%zu}\n",s->io.ReadOperationCount,s->io.WriteOperationCount,s->io.ReadTransferCount,s->io.WriteTransferCount,s->stack_committed,s->stack_reserved,s->stack_address_span,s->stack_pointer_distance);
 }
}
int wmain(int argc,WCHAR **argv)
{
 SECURITY_ATTRIBUTES sa={sizeof(sa),NULL,FALSE};PSECURITY_DESCRIPTOR sd=NULL;HANDLE parent=NULL,read=NULL;BY_HANDLE_FILE_INFORMATION identity;DWORD e=0,serial=0,flags=0,max_component=0,session=0,got=0;LARGE_INTEGER frequency;WCHAR filesystem[32]={0};PSRF_FRAME extracted;LARGE_INTEGER offset;SIZE_T prefix;unsigned close_failed=0;
 static const BYTE attempt[33]="ed110000000000000000000000000001";static const char record[]="PACC_PSR_FIRST_FAILURE api=NtCreateFile nt_valid=1 ntstatus=c0000022 stage=PREPARE measurement_fixture_only=1";
 if(argc!=3||GetConsoleWindow()!=NULL)return 2;prefix=wcslen(L"C:\\Projetos\\_tmp\\PapinhoAccelerator\\ed11-native-resource-boundary-20261023\\fixtures\\");
 if(wcsncmp(argv[1],L"C:\\Projetos\\_tmp\\PapinhoAccelerator\\ed11-native-resource-boundary-20261023\\fixtures\\",prefix)||wcsncmp(argv[2],argv[1],wcslen(argv[1]))||GetDriveTypeW(L"C:\\")!=DRIVE_FIXED)return 2;
 if(!QueryPerformanceFrequency(&frequency)||!ProcessIdToSessionId(GetCurrentProcessId(),&session))return 2;
 if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;OW)",SDDL_REVISION_1,&sd,NULL))return 2;sa.lpSecurityDescriptor=sd;
 ZeroMemory(samples,sizeof(samples));snap("cold_observer_baseline");snap("warm_observer_baseline");snap("stable_pre_file_baseline");
 parent=CreateFileW(argv[1],FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(parent==INVALID_HANDLE_VALUE){e=GetLastError();parent=NULL;goto finish;}
 if(!GetFileInformationByHandle(parent,&identity)||!(identity.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(identity.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||!GetVolumeInformationByHandleW(parent,NULL,0,&serial,&max_component,&flags,filesystem,32)){e=ERROR_INVALID_DATA;goto finish;}
 if(serial!=417614100UL||wcscmp(filesystem,L"NTFS")){e=ERROR_INVALID_DATA;goto finish;}
 snap("parent_pinned");e=psrf_create(&preparer,argv[2],&sa,attempt);if(e)goto finish;
 snap("arm_flushed_preparer_closed");e=psrf_capture(&writer,argv[2],&preparer.identity,attempt,record,sizeof(record)-1U,1,NULL);if(e)goto finish;
 snap("writer_flushed_closed");
 read=CreateFileW(argv[2],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(read==INVALID_HANDLE_VALUE){e=GetLastError();read=NULL;goto finish;}
 offset.QuadPart=1024;if(!SetFilePointerEx(read,offset,NULL,FILE_BEGIN)||!ReadFile(read,&extracted,sizeof(extracted),&got,NULL)||got!=sizeof(extracted)||!psrf_valid(&extracted,attempt,1)||memcmp(extracted.text,record,sizeof(record)-1U)){e=ERROR_INVALID_DATA;goto finish;}
 if(!CloseHandle(read)){e=GetLastError();close_failed=1;goto finish;}read=NULL;
 finish:
 if(!preparer.retained&&!writer.retained&&!close_failed){if(read){if(CloseHandle(read))read=NULL;else{if(!e)e=GetLastError();close_failed=1;}}if(parent&&!close_failed){if(CloseHandle(parent))parent=NULL;else{if(!e)e=GetLastError();close_failed=1;}}}if(metadata_error&&!e)e=metadata_error;
 snap("final_observation");emit();
 printf("{\"result\":%lu,\"frequency\":%lld,\"session\":%lu,\"sample_count\":%u,\"observer_array_bytes\":%zu,\"capsule_bytes\":%zu,\"preparer_closed\":%u,\"writer_closed\":%u,\"preparer_retained\":%u,\"writer_retained\":%u,\"parent_closed\":%s,\"volume_serial\":%lu,\"filesystem\":\"%ls\",\"retained_logical_file_bytes\":2048,\"kernel_attribution\":\"UNKNOWN\"}\n",e,frequency.QuadPart,session,count,sizeof(samples),sizeof(PSRF_FILE),preparer.closed,writer.closed,preparer.retained,writer.retained,parent?"false":"true",serial,serial?filesystem:L"UNAVAILABLE");
 if(preparer.retained||writer.retained||parent||read){fflush(stdout);Sleep(INFINITE);return 2;}
 if(sd&&LocalFree(sd))return 2;return e?2:0;
}
