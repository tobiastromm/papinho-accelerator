/* Actual ig_approval body, simulated file/hash APIs; NO FILES/SCM/AUTHORITY. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define PAPACC_N0START03_PRIVATE_POSITIVE 1
#include "../../../../tools/qualification/scm-n0-private-stdout-profile.h"
static HANDLE ig_approval_file;static unsigned cases,opens,reads,invalid_path;static DWORD gate_error,hash_error,attributes,size,links,high,read_error,info_error;static int missing,short_read;static const char *record;
static DWORD lab_aux_gate(void){return gate_error;}
static DWORD lab_error(void){return GetLastError();}
static DWORD lab_hash(HANDLE h,const char *expected){(void)h;(void)expected;return hash_error;}
static HANDLE WINAPI open_mock(LPCWSTR path,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES security,DWORD disposition,DWORD flags,HANDLE template_file){(void)access;(void)share;(void)security;(void)disposition;(void)flags;(void)template_file;opens++;if(!wcsstr(path,L"t10-t14-positive-integration\\EXACT-POSITIVE-OPERATION-APPROVED.json"))invalid_path=1;if(missing){SetLastError(ERROR_FILE_NOT_FOUND);return INVALID_HANDLE_VALUE;}return (HANDLE)(ULONG_PTR)123;}
static BOOL WINAPI info_mock(HANDLE h,LPBY_HANDLE_FILE_INFORMATION i){(void)h;if(info_error){SetLastError(info_error);return FALSE;}ZeroMemory(i,sizeof(*i));i->nNumberOfLinks=links;i->dwFileAttributes=attributes;i->nFileSizeLow=size;i->nFileSizeHigh=high;return TRUE;}
static DWORD WINAPI seek_mock(HANDLE h,LONG low,PLONG high_part,DWORD method){(void)h;(void)low;(void)high_part;(void)method;return 0;}
static BOOL WINAPI read_mock(HANDLE h,LPVOID out,DWORD bytes,LPDWORD done,LPOVERLAPPED ov){size_t n=strlen(record);(void)h;(void)ov;reads++;if(read_error){SetLastError(read_error);return FALSE;}if(n>bytes)n=bytes;if(short_read)n=1;memcpy(out,record,n);*done=(DWORD)n;return TRUE;}
#define CreateFileW open_mock
#define GetFileInformationByHandle info_mock
#define SetFilePointer seek_mock
#define ReadFile read_mock
#include "approval-reader.CURRENT.inc"
#undef CreateFileW
#undef GetFileInformationByHandle
#undef SetFilePointer
#undef ReadFile
#define T(x) do{cases++;if(!(x)){printf("APPROVAL_FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
static void reset(void){ig_approval_file=NULL;opens=reads=invalid_path=0;gate_error=hash_error=attributes=high=read_error=info_error=0;size=1024;links=1;missing=short_read=0;record=IG_PRIVATE_POSITIVE_APPROVAL_PREFIX;SetLastError(0);}
int main(void){const char *hash="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
 reset();T(ig_approval(hash)==0&&opens==1&&reads==1&&!invalid_path&&ig_approval_file);
 reset();T(ig_approval(NULL)==ERROR_ACCESS_DENIED&&!opens);reset();T(ig_approval("")==ERROR_ACCESS_DENIED&&!opens);
 reset();gate_error=ERROR_INVALID_STATE;T(ig_approval(hash)==gate_error&&!opens);
 reset();missing=1;T(ig_approval(hash)==ERROR_FILE_NOT_FOUND&&opens==1&&!reads&&!ig_approval_file);
 reset();hash_error=ERROR_CRC;T(ig_approval(hash)==ERROR_CRC&&opens==1&&!reads&&ig_approval_file);
 reset();record="{\n  \"status\": \"EXACT_MANUAL_READONLY_OWNER_APPROVED\",\n";T(ig_approval(hash)==ERROR_ACCESS_DENIED&&reads==1&&ig_approval_file);
 reset();short_read=1;T(ig_approval(hash)==ERROR_ACCESS_DENIED&&ig_approval_file);
 reset();attributes=FILE_ATTRIBUTE_REPARSE_POINT;T(ig_approval(hash)==ERROR_INVALID_DATA&&!reads);
 reset();attributes=FILE_ATTRIBUTE_DIRECTORY;T(ig_approval(hash)==ERROR_INVALID_DATA&&!reads);
 reset();links=2;T(ig_approval(hash)==ERROR_INVALID_DATA&&!reads);
 reset();size=16385;T(ig_approval(hash)==ERROR_INVALID_DATA&&!reads);
 reset();size=0;T(ig_approval(hash)==ERROR_INVALID_DATA&&!reads);
 reset();read_error=ERROR_IO_DEVICE;T(ig_approval(hash)==ERROR_IO_DEVICE&&ig_approval_file);
 reset();info_error=ERROR_INVALID_HANDLE;T(ig_approval(hash)==ERROR_INVALID_HANDLE&&!reads&&ig_approval_file);
 reset();high=1;T(ig_approval(hash)==ERROR_INVALID_DATA&&!reads);
 printf("ACTUAL_POSITIVE_APPROVAL_READER=PASS cases=%u API_SIMULATION_ONLY NO_FILE_NO_SCM\n",cases);return 0;}
