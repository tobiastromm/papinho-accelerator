/* One prospective read-only observation, never an administrative executor.
 * Build only unless separately authorized. No arguments, fallback or retry. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <sddl.h>
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"bcrypt.lib")
typedef NTSTATUS (NTAPI *OPEN_FN)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
static const WCHAR path[]=L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\phase6-server-update-after-utility-20261101\\FIRST-P1-SESSION-REQUEST.ORIGINAL.md";
#ifndef OBSERVER_MODEL
static volatile LONG stopped;
static BOOL WINAPI stop_handler(DWORD kind){(void)kind;InterlockedExchange(&stopped,1);return TRUE;}
#endif
static int accepted_result(NTSTATUS status,const IO_STATUS_BLOCK *io,HANDLE h)
{return status==0&&io->Status==0&&io->Information==1&&h!=NULL;}
static int uncertain_result(NTSTATUS status,const IO_STATUS_BLOCK *io,HANDLE h)
{return status>0||io->Status==(NTSTATUS)0x103L||h!=NULL;}
static int may_open(int known,DWORD count,int stop){return known&&count<124UL&&!stop;}
static void prepare_name(WCHAR name[MAX_PATH+4],UNICODE_STRING *unicode,OBJECT_ATTRIBUTES *attributes)
{
 wcscpy_s(name,MAX_PATH+4,L"\\??\\");wcscat_s(name,MAX_PATH+4,path);
 memset(attributes,0,sizeof(*attributes));unicode->Buffer=name;unicode->Length=(USHORT)(wcslen(name)*sizeof(WCHAR));unicode->MaximumLength=unicode->Length;
 attributes->Length=sizeof(*attributes);attributes->ObjectName=unicode;attributes->Attributes=0x1040UL;
}
static NTSTATUS invoke(OPEN_FN fn,HANDLE *h,OBJECT_ATTRIBUTES *attributes,IO_STATUS_BLOCK *io)
{return fn(h,0x00120089UL,attributes,io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1U,0x00200060U,NULL,0);}
#ifdef OBSERVER_MODEL
static int parameters_match;
static NTSTATUS NTAPI modeled_open(PHANDLE h,ACCESS_MASK access,POBJECT_ATTRIBUTES a,PIO_STATUS_BLOCK io,PLARGE_INTEGER size,ULONG attr,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_size)
{
 WCHAR expected[MAX_PATH+4];wcscpy_s(expected,MAX_PATH+4,L"\\??\\");wcscat_s(expected,MAX_PATH+4,path);
 parameters_match=access==0x00120089UL&&a->Length==sizeof(*a)&&!a->RootDirectory&&!a->SecurityDescriptor&&!a->SecurityQualityOfService&&
  a->Attributes==0x1040UL&&!wcscmp(a->ObjectName->Buffer,expected)&&a->ObjectName->Length==wcslen(expected)*sizeof(WCHAR)&&a->ObjectName->MaximumLength==a->ObjectName->Length&&
  !size&&attr==FILE_ATTRIBUTE_NORMAL&&share==FILE_SHARE_READ&&disposition==1U&&options==0x00200060U&&!ea&&!ea_size;
 *h=NULL;memset(io,0,sizeof(*io));return (NTSTATUS)0xc0000043L;
}
int main(void)
{
 IO_STATUS_BLOCK io;unsigned checks=0;HANDLE h=(HANDLE)(ULONG_PTR)1;NTSTATUS bad[]={ (NTSTATUS)0xc0000022L,(NTSTATUS)0xc0000043L,(NTSTATUS)0xc000050bL };unsigned i;WCHAR name[MAX_PATH+4];UNICODE_STRING unicode;OBJECT_ATTRIBUTES a;
#define CHECK(x) do{++checks;if(!(x))return 2;}while(0)
 memset(&io,0,sizeof(io));io.Information=1;
 CHECK(accepted_result(0,&io,h));CHECK(!accepted_result(0,&io,NULL));
 for(i=0;i<3;i++){CHECK(!accepted_result(bad[i],&io,NULL));CHECK(!uncertain_result(bad[i],&io,NULL));CHECK(uncertain_result(bad[i],&io,h));}
 io.Status=(NTSTATUS)0xc0000022L;CHECK(!accepted_result(0,&io,h));io.Status=(NTSTATUS)0x103L;CHECK(uncertain_result(0,&io,NULL));
 io.Status=0;io.Information=0;CHECK(!accepted_result(0,&io,h));CHECK(uncertain_result(0x103,&io,NULL));
 CHECK(may_open(1,113,0));CHECK(may_open(1,123,0));CHECK(!may_open(1,124,0));CHECK(!may_open(1,128,0));CHECK(!may_open(0,113,0));CHECK(!may_open(1,113,1));
 prepare_name(name,&unicode,&a);CHECK(invoke(modeled_open,&h,&a,&io)==(NTSTATUS)0xc0000043L);CHECK(parameters_match);
 printf("RAW_OPEN_MODEL checks=%u; NO_NATIVE_OPEN_NO_TOKEN_NO_SCM\n",checks);return 0;
}
#else
static int context(void)
{
 HANDLE token=NULL,thread=NULL;BYTE storage[1024];DWORD bytes=0;TOKEN_ELEVATION elevation;PSID expected=NULL;int ok=0;WCHAR host[64];DWORD size=64;
 if(!GetComputerNameW(host,&size)||wcscmp(host,L"TCORP-TOBIAS"))return 0;
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&thread)){CloseHandle(thread);return 0;}
 if(GetLastError()!=ERROR_NO_TOKEN)return 0;
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return 0;
 if(GetTokenInformation(token,TokenUser,storage,sizeof(storage),&bytes)&&
    ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&expected)&&
    EqualSid(((TOKEN_USER *)storage)->User.Sid,expected)&&
    GetTokenInformation(token,TokenElevation,&elevation,sizeof(elevation),&bytes)&&elevation.TokenIsElevated)ok=1;
 if(expected&&!LocalFree(expected))expected=NULL;
 if(expected)ok=0;if(!CloseHandle(token))ok=0;
 return ok;
}
static int verify(HANDLE h)
{
 BY_HANDLE_FILE_INFORMATION before,after;BYTE bytes[15530],digest[32];DWORD got=0;BCRYPT_HASH_HANDLE hash=NULL;unsigned i;int ok=0;
 static const BYTE expected[32]={0x86,0x30,0x3a,0xac,0x1b,0xf3,0xb0,0x40,0xa5,0x94,0x9e,0x89,0x47,0xa2,0xbe,0xe2,0xcb,0x15,0x7f,0xdb,0xde,0x47,0x2f,0x31,0xd5,0xf2,0x83,0x0b,0x46,0x9a,0x2a,0x34};
 if(!GetFileInformationByHandle(h,&before))return 0;
 printf("RAW_OPEN_OBJECT volume=%lu high=%lu low=%lu bytes=%lu; CURRENT_NOT_HISTORICAL\n",before.dwVolumeSerialNumber,before.nFileIndexHigh,before.nFileIndexLow,before.nFileSizeLow);
 if(before.dwVolumeSerialNumber!=417614100UL||before.nFileIndexHigh!=8978432UL||before.nFileIndexLow!=356716UL||before.nFileSizeHigh||before.nFileSizeLow!=15529UL||
    (before.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))return 0;
 if(stopped||!ReadFile(h,bytes,sizeof(bytes),&got,NULL)||got!=15529UL)return 0;
 if(BCryptCreateHash(BCRYPT_SHA256_ALG_HANDLE,&hash,NULL,0,NULL,0,0)!=0)return 0;
 if(BCryptHashData(hash,bytes,got,0)==0&&BCryptFinishHash(hash,digest,sizeof(digest),0)==0&&
    !memcmp(digest,expected,sizeof(digest))&&GetFileInformationByHandle(h,&after)&&
    before.dwVolumeSerialNumber==after.dwVolumeSerialNumber&&before.nFileIndexHigh==after.nFileIndexHigh&&before.nFileIndexLow==after.nFileIndexLow&&
    before.nFileSizeHigh==after.nFileSizeHigh&&before.nFileSizeLow==after.nFileSizeLow)ok=1;
 if(BCryptDestroyHash(hash)!=0)ok=0;
 if(ok){printf("RAW_OPEN_SHA256=");for(i=0;i<sizeof(digest);i++)printf("%02x",digest[i]);printf("; HASH_NOT_AUTHORITY\n");}
 SecureZeroMemory(bytes,sizeof(bytes));return ok;
}
int wmain(int argc,WCHAR **argv)
{
 OPEN_FN open_file;HMODULE module;FARPROC address;WCHAR name[MAX_PATH+4];UNICODE_STRING unicode;OBJECT_ATTRIBUTES attributes;IO_STATUS_BLOCK io;HANDLE h=NULL;DWORD count;NTSTATUS status;int match=0,closed=1;WCHAR confirm[64];
 (void)argv;if(argc!=1)return 2;
 if(!SetConsoleCtrlHandler(stop_handler,TRUE)||!context())return 2;
 if(GetDriveTypeW(L"C:\\")!=DRIVE_FIXED)return 2;
 printf("ONE_READONLY_RAW_OPEN; DIFFERENT_PROCESS_TOKEN_NOT_HISTORICAL_EQUIVALENCE; NO_RETRY_NO_AUTHORITY\nType OBSERVE_P1_REQUEST_RAW_ONCE to authorize only this observation:\n");
 if(!fgetws(confirm,64,stdin)||wcscmp(confirm,L"OBSERVE_P1_REQUEST_RAW_ONCE\n"))return 2;
 module=GetModuleHandleW(L"ntdll.dll");address=module?GetProcAddress(module,"NtCreateFile"):NULL;if(!address)return 2;memcpy(&open_file,&address,sizeof(open_file));
 memset(&io,0,sizeof(io));prepare_name(name,&unicode,&attributes);
 if(!GetProcessHandleCount(GetCurrentProcess(),&count))return 2;
 if(!may_open(1,count,(int)stopped)){printf("RAW_OPEN_NOT_ATTEMPTED handles=%lu guard=124 cap=128\n",count);return 2;}
 /* Exactly the original syscall parameters, including NULL RootDirectory,
  * no SecurityQualityOfService and no fallback on DOS/reparse rejection. */
 status=invoke(open_file,&h,&attributes,&io);
 printf("RAW_OPEN_RESULT attempted=1 handles_before=%lu ntstatus=0x%08lx iosb_status_snapshot=0x%08lx iosb_information_snapshot=%llu returned_handle=%u; INITIALIZED_IOSB_NOT_PROVEN_WRITTEN_ON_FAILURE\n",count,(DWORD)status,(DWORD)io.Status,(unsigned long long)io.Information,h?1U:0U);
 if(!accepted_result(status,&io,h)){
  if(uncertain_result(status,&io,h)){printf("RAW_OPEN_CLOSE=UNKNOWN; PENDING_OR_UNEXPECTED_HANDLE; NO_WAIT_NO_RETRY; OS_QUIESCENCE=UNKNOWN\n");return 2;}
  printf("RAW_OPEN_NO_HANDLE_RETURNED; HISTORICAL_CAUSE=UNKNOWN; AUTHORITY=DENY\n");return 0;
 }
 if(!stopped)match=verify(h);closed=CloseHandle(h)?1:0;
 printf("RAW_OPEN_CURRENT_MATCH=%d close_known=%d; HISTORICAL_CAUSE=UNKNOWN; AUTHORITY=DENY; OS_QUIESCENCE=UNKNOWN; NO_RETRY\n",match,closed);
 return match&&closed?0:2;
}
#endif
