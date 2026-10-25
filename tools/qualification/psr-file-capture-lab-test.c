#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
#ifdef PSRF_MODEL
static volatile LONG *capture_stop;
static BYTE disk[2048];static DWORD disk_size,position;static unsigned mode,create_calls,write_calls,close_calls;
static HANDLE WINAPI f_create(LPCWSTR path,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES sa,DWORD disposition,DWORD flags,HANDLE temp)
{
 (void)path;(void)sa;(void)temp;create_calls++;
 if(share||!(flags&FILE_FLAG_OPEN_REPARSE_POINT)||access!=(FILE_READ_DATA|FILE_WRITE_DATA|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE)){SetLastError(ERROR_INVALID_PARAMETER);return INVALID_HANDLE_VALUE;}
 if(mode==1U){SetLastError(ERROR_ACCESS_DENIED);return INVALID_HANDLE_VALUE;}
 if(disposition==CREATE_NEW){if(disk_size){SetLastError(ERROR_FILE_EXISTS);return INVALID_HANDLE_VALUE;}}
 else if(disposition!=OPEN_EXISTING){SetLastError(ERROR_INVALID_PARAMETER);return INVALID_HANDLE_VALUE;}
 position=0;return (HANDLE)(ULONG_PTR)77;
}
static BOOL WINAPI f_info(HANDLE h,LPBY_HANDLE_FILE_INFORMATION v){(void)h;ZeroMemory(v,sizeof(*v));v->dwVolumeSerialNumber=17;v->nFileIndexLow=mode==8U?99U:32U;v->nNumberOfLinks=1;v->dwFileAttributes=mode==9U?FILE_ATTRIBUTE_REPARSE_POINT:FILE_ATTRIBUTE_NORMAL;return TRUE;}
static BOOL WINAPI f_size(HANDLE h,PLARGE_INTEGER v){(void)h;v->QuadPart=mode==10U?12:disk_size;return TRUE;}
static DWORD WINAPI f_type(HANDLE h){(void)h;return FILE_TYPE_DISK;}
static BOOL WINAPI f_read(HANDLE h,LPVOID b,DWORD n,LPDWORD got,LPOVERLAPPED ov){(void)h;(void)ov;if(n>disk_size-position){SetLastError(ERROR_HANDLE_EOF);return FALSE;}memcpy(b,disk+position,n);position+=n;*got=n;if(mode==11U&&capture_stop)*capture_stop=1;return TRUE;}
static BOOL WINAPI f_write(HANDLE h,LPCVOID b,DWORD n,LPDWORD got,LPOVERLAPPED ov){(void)h;(void)ov;write_calls++;if(mode==4U){SetLastError(ERROR_DISK_FULL);return FALSE;}if(mode==3U)n--;if(position+n>sizeof(disk)){SetLastError(ERROR_DISK_FULL);return FALSE;}memcpy(disk+position,b,n);position+=n;disk_size=position;*got=n;return TRUE;}
static BOOL WINAPI f_flush(HANDLE h){(void)h;if(mode==5U){SetLastError(ERROR_WRITE_FAULT);return FALSE;}return TRUE;}
static BOOL WINAPI f_close(HANDLE h){(void)h;close_calls++;if(mode==6U){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
#define CreateFileW f_create
#define GetFileInformationByHandle f_info
#define GetFileSizeEx f_size
#define GetFileType f_type
#define ReadFile f_read
#define WriteFile f_write
#define FlushFileBuffers f_flush
#define CloseHandle f_close
#endif
#define PAPACC_PSR_FILE_CAPSULE_LAB 1
#include "psr-file-capture-lab.inc"
static void psrf_trace_sink(LPCSTR text,unsigned kind);
#define PSRT_OUTPUT psrf_trace_sink
#define PAPACC_SCM_RUNTIME_LAB 1
#define PAPACC_SCM_REGISTRATION_LAB 1
#include "../../apps/server/server_service_psr_trace_win32.inc"
#define T(x) do{if(!(x)){printf("FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
static const BYTE attempt[]="0123456789abcdef0123456789abcdef";
static const char original[]="PACC_PSR_FIRST_FAILURE api=NtCreateFile nt_valid=1 ntstatus=c0000022 request=001200a0 object=parent stage=PREPARE returned_win32=5";
static PSRF_FILE prep,sink;static SECURITY_DESCRIPTOR descriptor;static SECURITY_ATTRIBUTES security={sizeof(SECURITY_ATTRIBUTES),&descriptor,FALSE};
static const WCHAR *trace_path;static DWORD trace_result;
static void psrf_trace_sink(LPCSTR text,unsigned kind)
{
 if(kind&&!sink.consumed)
  trace_result=psrf_capture(&sink,trace_path,&prep.identity,attempt,text,(DWORD)strlen(text),kind,NULL);
}
static DWORD trace_case(LONG status)
{
 DWORD n=0;ZeroMemory(&psrt_record,sizeof(psrt_record));psrt_context("parent");
 (void)psrt_bool("BOOL",1,0,TRUE);(void)psrt_handle("HANDLE",2,0,(HANDLE)(ULONG_PTR)1);
 (void)psrt_status("STATUS",3,0,0);(void)psrt_count("COUNT",4,1);(void)psrt_pred("PREDICATE",TRUE);psrt_values(17,32);
 (void)psrt_token("TOKEN_UNAVAILABLE_FIXTURE",5,TokenUser,NULL,0,&n,FALSE);
 (void)psrt_nt("NtCreateFile",6,0x1200a0UL,status);SetLastError(12345);
 n=psrt_return(ERROR_ACCESS_DENIED,7,"prepare_propagation");psrt_prepared();return n;
}

#ifdef PSRF_MODEL
static int arm(void){ZeroMemory(&prep,sizeof(prep));ZeroMemory(&sink,sizeof(sink));ZeroMemory(disk,sizeof(disk));disk_size=position=0;mode=0;return psrf_create(&prep,L"MODEL_ONLY",&security,attempt)==0;}
int main(void)
{
 unsigned i;DWORD w,c,calls;volatile LONG stop=0;PSRF_FRAME saved;BYTE wrong[32];DWORD authorization=ERROR_ACCESS_DENIED;
 T(InitializeSecurityDescriptor(&descriptor,SECURITY_DESCRIPTOR_REVISION));T(arm());T(disk_size==1024&&prep.closed&&prep.created&&prep.flushed);memcpy(&saved,disk,1024);
 T(psrf_capture(&sink,L"MODEL_ONLY",&prep.identity,attempt,original,sizeof(original)-1,1,&stop)==0);T(disk_size==2048&&sink.closed&&sink.written&&sink.flushed);T(!memcmp(&saved,disk,1024));T(psrf_valid((PSRF_FRAME *)(disk+1024),attempt,1));T(!memcmp(((PSRF_FRAME *)(disk+1024))->text,original,sizeof(original)-1));
 calls=create_calls;T(psrf_capture(&sink,L"MODEL_ONLY",&prep.identity,attempt,original,sizeof(original)-1,1,&stop)==ERROR_INVALID_STATE&&create_calls==calls);
 ZeroMemory(&sink,sizeof(sink));w=write_calls;T(psrf_capture(&sink,L"MODEL_ONLY",&prep.identity,attempt,original,sizeof(original)-1,1,&stop)==ERROR_INVALID_DATA&&write_calls==w&&sink.retained);T(psrf_close(&sink)==ERROR_BUSY);
 T(arm());ZeroMemory(&sink,sizeof(sink));T(psrf_create(&sink,L"MODEL_ONLY",&security,attempt)==ERROR_FILE_EXISTS&&!sink.handle&&disk_size==1024);
 ZeroMemory(&prep,sizeof(prep));ZeroMemory(disk,sizeof(disk));disk_size=position=0;mode=1U;w=write_calls;
 T(psrf_create(&prep,L"MODEL_ONLY",&security,attempt)==ERROR_ACCESS_DENIED&&!prep.handle&&prep.consumed&&write_calls==w);
 T(arm());stop=1;calls=create_calls;T(psrf_capture(&sink,L"MODEL_ONLY",&prep.identity,attempt,original,sizeof(original)-1,1,&stop)==ERROR_CANCELLED&&create_calls==calls);stop=0;
 for(i=1;i<=11;i++)if(i!=2&&i!=7){T(arm());mode=i;stop=0;capture_stop=&stop;c=close_calls;w=write_calls;T(psrf_capture(&sink,L"MODEL_ONLY",&prep.identity,attempt,original,sizeof(original)-1,1,&stop)!=0);T(authorization==ERROR_ACCESS_DENIED);if(sink.handle){T(sink.retained);T(psrf_close(&sink)==ERROR_BUSY);T(close_calls==c+(i==6U?1U:0U));}if(i==8U||i==9U||i==10U||i==11U)T(write_calls==w);}
 stop=0;capture_stop=NULL;
 T(arm());memcpy(wrong,attempt,32);wrong[0]^=1;w=write_calls;T(psrf_capture(&sink,L"MODEL_ONLY",&prep.identity,wrong,original,sizeof(original)-1,1,&stop)==ERROR_INVALID_DATA&&write_calls==w);
 T(arm());disk[64]^=1;w=write_calls;T(psrf_capture(&sink,L"MODEL_ONLY",&prep.identity,attempt,original,sizeof(original)-1,1,&stop)==ERROR_INVALID_DATA&&write_calls==w);
 {
 const LONG statuses[]={(LONG)0xc0000022UL,(LONG)0xc0000034UL,(LONG)0xc000003aUL,(LONG)0xc0000043UL,(LONG)0xc0000035UL,(LONG)0xc000000dUL,(LONG)0xe1234567UL};
 for(i=0;i<sizeof(statuses)/sizeof(statuses[0]);i++){
  T(arm());trace_path=L"MODEL_ONLY";T(trace_case(statuses[i])==ERROR_ACCESS_DENIED);T(GetLastError()==12345);T(trace_result==0&&psrt_record.ntstatus==statuses[i]&&psrt_record.returned==ERROR_ACCESS_DENIED);T(disk_size==2048);
 }
 T(arm());mode=1U;T(trace_case((LONG)0xc0000022UL)==ERROR_ACCESS_DENIED);T(GetLastError()==12345);T(trace_result==ERROR_ACCESS_DENIED&&psrt_record.ntstatus==(LONG)0xc0000022UL);
 }
 printf("MODEL_PASS exclusive collision partial denied stop retained flush_unknown truncated identity reparse attempt checksum replay original_status authorization_unchanged state_bytes=%zu frame_bytes=%zu\n",sizeof(PSRF_FILE),sizeof(PSRF_FRAME));return 0;
}
#else
int wmain(int argc,WCHAR **argv)
{
 HANDLE read;PSRF_FRAME final;DWORD n=0;LARGE_INTEGER offset;PSECURITY_DESCRIPTOR sd=NULL;DWORD denied=ERROR_ACCESS_DENIED;
 T(argc==2);T(GetConsoleWindow()==NULL);T(wcsncmp(argv[1],L"C:\\Projetos\\_tmp\\PapinhoAccelerator\\session0-diagnostic-capture-20261023\\fixtures\\",(sizeof(L"C:\\Projetos\\_tmp\\PapinhoAccelerator\\session0-diagnostic-capture-20261023\\fixtures\\")/sizeof(WCHAR))-1)==0);
 T(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;OW)",SDDL_REVISION_1,&sd,NULL));security.lpSecurityDescriptor=sd;
 T(psrf_create(&prep,argv[1],&security,attempt)==0);T(prep.closed&&prep.flushed);
 T(psrf_create(&sink,argv[1],&security,attempt)==ERROR_FILE_EXISTS);ZeroMemory(&sink,sizeof(sink));
 trace_path=argv[1];T(trace_case((LONG)0xc0000022UL)==ERROR_ACCESS_DENIED);T(GetLastError()==12345);T(trace_result==0);T(sink.closed&&sink.flushed&&!sink.retained&&denied==ERROR_ACCESS_DENIED);
 read=CreateFileW(argv[1],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);T(read!=INVALID_HANDLE_VALUE);offset.QuadPart=1024;T(SetFilePointerEx(read,offset,NULL,FILE_BEGIN));T(ReadFile(read,&final,sizeof(final),&n,NULL)&&n==sizeof(final));T(psrf_valid(&final,attempt,1));T(final.length<PSRF_TEXT);final.text[final.length]=0;T(strstr((char *)final.text,"ntstatus=c0000022")!=NULL);T(strstr((char *)final.text,"api=NtCreateFile")!=NULL);T(CloseHandle(read));T(LocalFree(sd)==NULL);
 final.text[0]^=1;T(!psrf_valid(&final,attempt,1));T(!psrf_valid(&final,attempt,0));
 printf("NATIVE_PASS no_console create_new collision append_no_overwrite file_id exact_status flushed closed retained_file_bytes=2048 state_bytes=%zu\n",sizeof(PSRF_FILE));return 0;
}
#endif
