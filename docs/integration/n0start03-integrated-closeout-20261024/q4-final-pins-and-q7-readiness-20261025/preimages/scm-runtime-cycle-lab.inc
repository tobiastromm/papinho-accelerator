#include "scm-lab-attempt-binding.h"
/* One-shot manual LAB cycle in the SAME existing SCM adapter/owner.
 * Included only by the isolated registration qualification build. */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iphlpapi.h>
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
static unsigned ig_n0_positive; /* existing mode state, before helpers that use it */
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
static const WCHAR cycle_source[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\q4-binding-20261025\\Selected\\Release\\papacc_server_lab_host.exe";
#else
static const WCHAR cycle_source[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\successor-exact-binding-20261024-runtime\\Selected\\Release\\papacc_server_lab_host.exe";
#endif
static const WCHAR cycle_path[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc_server_lab_host_ntstatus_" PACC_LAB_ATTEMPT_W L".exe";
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
#include "scm-q4-image-pin.h"
static const char cycle_sha[]=PS_BD_N0_IMAGE_SHA;
#else
static const char cycle_sha[]="44d8597469bdd4f40b317f467029ee96b3ad69fdc270bcdbf5e126788f623e5c";
#endif
#else
static const WCHAR cycle_source[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\windows-service-protected-runtime-20261023\\Selected\\Release\\papacc_server_lab_host.exe";
static const WCHAR cycle_path[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc_server_lab_host.exe";
static const char cycle_sha[]="62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716";
#endif
static HANDLE cycle_source_file,cycle_file,cycle_process,cycle_token,cycle_access,cycle_threads,cycle_thread;
static PSECURITY_DESCRIPTOR cycle_sd;static LPWSTR cycle_sid_retained;
static BY_HANDLE_FILE_INFORMATION cycle_id;
static unsigned cycle_effect,cycle_start_requested,cycle_stop_requested,cycle_closed,cycle_quiescent;
static HANDLE cycle_dependencies[2];static BYTE cycle_dependency_sd[2][32],cycle_image_sd[32];static unsigned cycle_image_sd_set;
static const char *cycle_stage="ENTRY";
/* Native relative FILE_CREATE, never OPEN_IF/OVERWRITE. Protected parent retained. */
static DWORD cycle_open_relative(int create)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 typedef NTSTATUS (NTAPI *CREATE)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
 CREATE fn;FARPROC proc;UNICODE_STRING str;OBJECT_ATTRIBUTES oa;IO_STATUS_BLOCK io;NTSTATUS status;HMODULE module;
 #ifdef PAPACC_SCM_INSTRUMENTED_LAB
 static WCHAR leaf[]=L"papacc_server_lab_host_ntstatus_" PACC_LAB_ATTEMPT_W L".exe";
#else
 static WCHAR leaf[]=L"papacc_server_lab_host.exe";
#endif
 if(cycle_file)return ERROR_INVALID_STATE;
 module=GetModuleHandleW(L"ntdll.dll");if(!module)return ERROR_NOT_SUPPORTED;
 proc=GetProcAddress(module,"NtCreateFile");if(!proc)return ERROR_NOT_SUPPORTED;memcpy(&fn,&proc,sizeof(fn));
 ZeroMemory(&oa,sizeof(oa));ZeroMemory(&io,sizeof(io));str.Buffer=leaf;str.Length=(USHORT)(sizeof(leaf)-sizeof(WCHAR));str.MaximumLength=sizeof(leaf);
 oa.Length=sizeof(oa);oa.RootDirectory=lab_parent;oa.ObjectName=&str;oa.Attributes=OBJ_CASE_INSENSITIVE|0x1000U;oa.SecurityDescriptor=create?cycle_sd:NULL;
 status=fn(&cycle_file,GENERIC_READ|READ_CONTROL|SYNCHRONIZE|(create?GENERIC_WRITE:0),&oa,&io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,create?2U:1U,0x00200060U,NULL,0);
 if(status<0){if(cycle_file)native.retained=1;return status==(NTSTATUS)0xc0000035L?ERROR_FILE_EXISTS:ERROR_ACCESS_DENIED;}
 return ERROR_SUCCESS;
}
static DWORD cycle_dependencies_current_ex(int initial,int content)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 static const WCHAR *paths[]={L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\libcrypto-3-x64.dll",L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\libssl-3-x64.dll"};
 static const char *hashes[]={"09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044","3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed"};
 static const BYTE expected_sd[2][32]={{0x10,0x95,0x71,0x3b,0x09,0xc3,0xc1,0x46,0x27,0xe3,0x26,0x4e,0xd2,0xfc,0x73,0x1a,0x61,0x11,0x0a,0x0a,0x25,0x30,0x56,0xff,0xe9,0xd7,0xd3,0xe8,0x9d,0x27,0x68,0xa0}, {0x10,0x95,0x71,0x3b,0x09,0xc3,0xc1,0x46,0x27,0xe3,0x26,0x4e,0xd2,0xfc,0x73,0x1a,0x61,0x11,0x0a,0x0a,0x25,0x30,0x56,0xff,0xe9,0xd7,0xd3,0xe8,0x9d,0x27,0x68,0xa0}};
 static const DWORD lows[]={1120009,1119998};unsigned i;DWORD e=0,rev,n;SECURITY_DESCRIPTOR_CONTROL control;BYTE digest[32];
 for(i=0;i<2;i++){
 if(initial){cycle_dependencies[i]=CreateFileW(paths[i],GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(cycle_dependencies[i]==INVALID_HANDLE_VALUE){cycle_dependencies[i]=NULL;return lab_error();}}
 if(!cycle_dependencies[i]||(e=lab_id(cycle_dependencies[i],393216,lows[i],0))||(content&&(e=lab_hash(cycle_dependencies[i],hashes[i]))))return e?e:ERROR_INVALID_HANDLE;
 if(cycle_sd)return ERROR_INVALID_STATE;
 e=GetSecurityInfo(cycle_dependencies[i],SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&cycle_sd);if(e)return e;
 n=GetSecurityDescriptorLength(cycle_sd);
 if(!n||n>65536||!GetSecurityDescriptorControl(cycle_sd,&control,&rev)||!(control&SE_DACL_PROTECTED)||!scm_descriptor(cycle_sd,(BYTE *)lab_sid)||!pacc_storage_hash_callback(NULL,(BYTE *)cycle_sd,n,digest)||memcmp(digest,expected_sd[i],32)||(!initial&&memcmp(digest,cycle_dependency_sd[i],32)))e=ERROR_ACCESS_DENIED;
 if(initial&&!e)memcpy(cycle_dependency_sd[i],digest,32);
 if(LocalFree(cycle_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_sd=NULL;if(e)return e;
 }
 return ERROR_SUCCESS;
}
static DWORD cycle_dependencies_current(int initial){return cycle_dependencies_current_ex(initial,1);}
static DWORD cycle_files(void)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 DWORD e;BY_HANDLE_FILE_INFORMATION now;
 if((e=lab_bound(1))||(e=cycle_dependencies_current(0)))return e;
 if(!cycle_file||!GetFileInformationByHandle(cycle_file,&now)||!ps_image_same(&now,&cycle_id))return ERROR_INVALID_DATA;
 if((e=lab_hash(cycle_file,cycle_sha)))return e;
 /* Same descriptor predicate, with protected DACL required. */
 if(cycle_sd)return ERROR_INVALID_STATE;
 e=GetSecurityInfo(cycle_file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&cycle_sd);
 if(!e){SECURITY_DESCRIPTOR_CONTROL c;DWORD rev;
 DWORD bytes;BYTE digest[32];
 if(!GetSecurityDescriptorControl(cycle_sd,&c,&rev)||!(c&SE_DACL_PROTECTED)||!scm_descriptor(cycle_sd,(BYTE *)lab_sid))e=ERROR_ACCESS_DENIED;
 bytes=GetSecurityDescriptorLength(cycle_sd);
 if(!e&&(!bytes||bytes>65536||!pacc_storage_hash_callback(NULL,(BYTE *)cycle_sd,bytes,digest)||(cycle_image_sd_set&&memcmp(digest,cycle_image_sd,32))))e=ERROR_ACCESS_DENIED;
 if(!e&&!cycle_image_sd_set){memcpy(cycle_image_sd,digest,32);cycle_image_sd_set=1;}}
 if(cycle_sd){if(LocalFree(cycle_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_sd=NULL;}
 return e;
}
static DWORD cycle_status(SERVICE_STATUS_PROCESS *s)
{
 DWORD n=0;ZeroMemory(s,sizeof(*s));
 if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(BYTE *)s,sizeof(*s),&n))return lab_error();
 return n==sizeof(*s)&&s->dwServiceType==SERVICE_WIN32_OWN_PROCESS?ERROR_SUCCESS:ERROR_INVALID_DATA;
}
static DWORD cycle_wait(DWORD desired,DWORD pending,SERVICE_STATUS_PROCESS *s)
{
 DWORD e;ULONGLONG began=GetTickCount64();
 for(;;){if((e=cycle_status(s)))return e;
 if(s->dwCurrentState==desired)return ERROR_SUCCESS;
 if(s->dwCurrentState!=pending&&!(desired==SERVICE_STOPPED&&pending==SERVICE_STOP_PENDING&&cycle_stop_requested&&s->dwCurrentState==SERVICE_RUNNING)){printf("LAB_UNEXPECTED_STATUS state=%lu pid=%lu win32=%lu specific=%lu\n",(unsigned long)s->dwCurrentState,(unsigned long)s->dwProcessId,(unsigned long)s->dwWin32ExitCode,(unsigned long)s->dwServiceSpecificExitCode);return ERROR_INVALID_STATE;}
 if(GetTickCount64()-began>=30000U)return ERROR_TIMEOUT; /* UNKNOWN; no kill/retry */
 Sleep(50);}
}
static DWORD cycle_readback(DWORD startup,int candidate)
{
 WCHAR saved[1050];DWORD e;memcpy(saved,native.command,sizeof(saved));
 if(candidate){native.command[0]=0;if(!lab_append(native.command,1050,L"\"")||!lab_append(native.command,1050,cycle_path)||!lab_append(native.command,1050,L"\" service host"))return ERROR_INVALID_DATA;}
 e=ps_virtual_readback_start(startup);
 memcpy(native.command,saved,sizeof(saved));
 if(!e&&native.query.aligned.dwErrorControl!=SERVICE_ERROR_NORMAL)e=ERROR_INVALID_DATA;
 return e;
}
static DWORD cycle_confirm(const char *attempt)
{
 int borrowed=0;HANDLE in=NULL,out=NULL;A1_LEASE in_lease,out_lease;DWORD im,om,written,n,e=0;WCHAR plan[4096],reply[128],expected[96],wa[33];unsigned k;
 if((e=lab_aux_console(&in_lease,&out_lease)))return e;
 if((e=lab_aux_borrow_pair(in_lease,out_lease,&in,&out)))goto done;borrowed=1;
 if(!GetConsoleMode(in,&im)||!GetConsoleMode(out,&om)||(im&(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT))!=(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT)){e=ERROR_ACCESS_DENIED;goto done;}
 for(k=0;k<32;k++)wa[k]=(WCHAR)(unsigned char)attempt[k];wa[32]=0;plan[0]=0;expected[0]=0;
 if(!lab_append(expected,96,L"CYCLE_SCM_LAB ")||!lab_append(expected,96,wa)||
 !lab_append(plan,4096,L"LAB ONLY; EXISTING SERVICE=PapinhoAcceleratorLab20261023\r\nACCOUNT=NT SERVICE\\PapinhoAcceleratorLab20261023; PASSWORD=NULL\r\nOPERATOR=S-1-5-21-785699550-1440458426-1682014700-1108\r\nPARENT=417614100:1966080:79426; CONFIG=417614100:393216:1120240\r\nSOURCE=")||!lab_append(plan,4096,cycle_source)||
 !lab_append(plan,4096,L"\r\nNEW EXCLUSIVE PROTECTED IMAGE=")||!lab_append(plan,4096,cycle_path)||
 !lab_append(plan,4096,L"\r\nSHA256=62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716\r\nCONFIG_SHA256=a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44\r\nDLL native IDs/hash/protected descriptors retained and revalidated. No ACL remediation.\r\nCreate ONE candidate with copied approved original-image owner/DACL; no overwrite/delete.\r\nRebind ONLY LAB image to quoted new path + service host; DISABLED -> DEMAND. Account/SID unchanged.\r\nONE Start -> RUNNING/status -> actual token/groups/privileges/access/resources/tables -> ONE Stop -> STOPPED/process exit.\r\nOne safety Stop is authorized if evidence capture fails after RUNNING, preserving first error.\r\nSuccess leaves DEMAND/STOPPED and candidate retained. Partial result/timeout is UNKNOWN; no restore/delete/retry/kill.\r\nUnknown live service or close failure retains qualifier process and refs; do not close/retry automatically.\r\nNo listener/network initialization, CA/key/cert/bootstrap/CURRENT/signature. KSP access NOT tested. ED11 PARTIAL.\r\nType ENTIRE exact line then Enter (visible input; Esc/mismatch cancels before effect):\r\n")||!lab_append(plan,4096,expected)||!lab_append(plan,4096,L"\r\n> ")){e=ERROR_INVALID_DATA;goto done;}
 if(!WriteConsoleW(out,plan,(DWORD)wcslen(plan),&written,NULL)||written!=(DWORD)wcslen(plan)||!ReadConsoleW(in,reply,127,&n,NULL)){e=lab_error();goto done;}
 while(n&&(reply[n-1]==L'\r'||reply[n-1]==L'\n'))n--;reply[n]=0;if(wcscmp(reply,expected)||papacc_server_console_win32_stop_requested())e=ERROR_CANCELLED;
 done:if(borrowed)e=lab_aux_borrow_end(e);e=lab_aux_close_preserving(in_lease,e);return lab_aux_close_preserving(out_lease,e);
}
static DWORD cycle_intent(const char *attempt)
{
 HANDLE h=NULL;A1_LEASE lease;DWORD n,e;char text[128];int count;
 if((e=lab_aux_file(&lease,L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\first-native-lab-service-start-stop-20261023\\manual-native-intent.txt",GENERIC_WRITE,0,CREATE_NEW,FILE_ATTRIBUTE_NORMAL)))return e;
 if((e=lab_aux_borrow(lease,&h)))return lab_aux_close_preserving(lease,e);
 count=_snprintf_s(text,sizeof(text),_TRUNCATE,"ATTEMPT=%s; CYCLE_LAB_EFFECTS_REQUESTED; NO_RETRY; UNKNOWN_UNTIL_RECEIPT\r\n",attempt);
 if(count<=0||count>=(int)sizeof(text)||!WriteFile(h,text,(DWORD)count,&n,NULL)||n!=(DWORD)count||!FlushFileBuffers(h)){e=lab_error();return lab_aux_close_preserving(lease,lab_aux_borrow_end(e));}
 return lab_aux_close_preserving(lease,lab_aux_borrow_end(0));
}
static DWORD cycle_copy(void)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 DWORD e,n,w;BYTE block[4096];LARGE_INTEGER zero;BY_HANDLE_FILE_INFORMATION before,after;
 cycle_stage="EXCLUSIVE_IMAGE_CREATE";
 e=GetSecurityInfo(native.image,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&cycle_sd);if(e)return e;
 if(!GetFileInformationByHandle(cycle_source_file,&before))return lab_error();
 if((e=cycle_open_relative(1)))return e;cycle_effect=1;
 if(LocalFree(cycle_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_sd=NULL;
 zero.QuadPart=0;if(!SetFilePointerEx(cycle_source_file,zero,NULL,FILE_BEGIN))return lab_error();
 for(;;){if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;
 if(!ReadFile(cycle_source_file,block,sizeof(block),&n,NULL))return lab_error();if(!n)break;
 if(!WriteFile(cycle_file,block,n,&w,NULL)||w!=n)return lab_error();}
 if(!FlushFileBuffers(cycle_file)||!GetFileInformationByHandle(cycle_file,&cycle_id)||!GetFileInformationByHandle(cycle_source_file,&after))return lab_error();
 if(!ps_image_same(&before,&after))return ERROR_INVALID_DATA;
 if((e=lab_hash(cycle_file,cycle_sha)))return e;
 /* Drop writer, reopen relative with read-only share, compare same native object. */
 if(!CloseHandle(cycle_file)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_file=NULL;
 if((e=cycle_open_relative(0)))return e;return cycle_files();
}
static DWORD cycle_access_file(HANDLE file,unsigned index)
{
 DWORD e,n=1024,granted=0;BOOL allowed=FALSE;BYTE privileges[1024];
 GENERIC_MAPPING map={FILE_GENERIC_READ,FILE_GENERIC_WRITE,FILE_GENERIC_EXECUTE,FILE_ALL_ACCESS};
 if(cycle_sd)return ERROR_INVALID_STATE;
 e=GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&cycle_sd);
 if(!e&&(!AccessCheck(cycle_sd,cycle_access,MAXIMUM_ALLOWED,&map,(PRIVILEGE_SET *)privileges,&n,&granted,&allowed)||!allowed||
 (granted&(FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|DELETE|WRITE_DAC|WRITE_OWNER|FILE_DELETE_CHILD))||
 (granted&(FILE_READ_DATA|(index==2?FILE_EXECUTE:0)))!=(DWORD)(FILE_READ_DATA|(index==2?FILE_EXECUTE:0))))e=ERROR_ACCESS_DENIED;
 printf("LAB_EFFECTIVE_ACL index=%u granted=%08lx status=%lu\n",index,(unsigned long)granted,(unsigned long)e);
 if(cycle_sd){if(LocalFree(cycle_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_sd=NULL;}return e;
}
static int cycle_sid_inside(BYTE *b,DWORD n,PSID p)
{
 ULONG_PTR a=(ULONG_PTR)b,v=(ULONG_PTR)p;DWORD z;
 if(v<a||v-a>n||n-(DWORD)(v-a)<8U)return 0;z=8U+4U*((BYTE *)p)[1];return z<=n-(DWORD)(v-a)&&IsValidSid(p)&&GetLengthSid(p)==z;
}
static DWORD cycle_no_endpoints(DWORD pid)
{
 union {MIB_TCPTABLE_OWNER_PID align;BYTE bytes[65536];}tcp;
 union {MIB_UDPTABLE_OWNER_PID align;BYTE bytes[65536];}udp;
 union {MIB_TCP6TABLE_OWNER_PID align;BYTE bytes[65536];}tcp6;
 union {MIB_UDP6TABLE_OWNER_PID align;BYTE bytes[65536];}udp6;
 DWORD n=sizeof(tcp),e,i;
 e=GetExtendedTcpTable(&tcp,&n,FALSE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0);if(e)return e;
 if(n>sizeof(tcp)||n<sizeof(DWORD)||tcp.align.dwNumEntries>(n-FIELD_OFFSET(MIB_TCPTABLE_OWNER_PID,table))/sizeof(MIB_TCPROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<tcp.align.dwNumEntries;i++)if(tcp.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 n=sizeof(udp);e=GetExtendedUdpTable(&udp,&n,FALSE,AF_INET,UDP_TABLE_OWNER_PID,0);if(e)return e;
 if(n>sizeof(udp)||n<sizeof(DWORD)||udp.align.dwNumEntries>(n-FIELD_OFFSET(MIB_UDPTABLE_OWNER_PID,table))/sizeof(MIB_UDPROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<udp.align.dwNumEntries;i++)if(udp.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 n=sizeof(tcp6);e=GetExtendedTcpTable(&tcp6,&n,FALSE,AF_INET6,TCP_TABLE_OWNER_PID_ALL,0);if(e)return e;
 if(n>sizeof(tcp6)||n<sizeof(DWORD)||tcp6.align.dwNumEntries>(n-FIELD_OFFSET(MIB_TCP6TABLE_OWNER_PID,table))/sizeof(MIB_TCP6ROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<tcp6.align.dwNumEntries;i++)if(tcp6.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 n=sizeof(udp6);e=GetExtendedUdpTable(&udp6,&n,FALSE,AF_INET6,UDP_TABLE_OWNER_PID,0);if(e)return e;
 if(n>sizeof(udp6)||n<sizeof(DWORD)||udp6.align.dwNumEntries>(n-FIELD_OFFSET(MIB_UDP6TABLE_OWNER_PID,table))/sizeof(MIB_UDP6ROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<udp6.align.dwNumEntries;i++)if(udp6.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 puts("LAB_ENDPOINT_TABLES=NO_IPV4_IPV6_TCP_UDP_ROWS_FOR_PID_SNAPSHOT; NO_SOCKET_OPENED_BY_QUERY");return ERROR_SUCCESS;
}
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
/* Last-use release of named live-validation owners, never auxiliary adoption.
 * Preserve the original reference on uncertainty and freeze before any reuse. */
static DWORD cycle_live_close_last(HANDLE *owned)
{
 DWORD e;if(lab_aux_owner.state&A1_FROZEN)return lab_aux_sync(ERROR_INVALID_STATE);
 if(!owned||!*owned)return ERROR_INVALID_HANDLE;
 if(!CloseHandle(*owned)){e=lab_error();a1_retain(&lab_aux_owner,e);return lab_aux_sync(e);}
 *owned=NULL;return 0;
}
#endif
static DWORD cycle_live(DWORD pid)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 union {TOKEN_USER align;BYTE b[256];}u;
 union {TOKEN_GROUPS align;BYTE b[8192];}g;
 union {TOKEN_PRIVILEGES align;BYTE b[4096];}p;
 TOKEN_STATISTICS stats;DWORD n,i,e=0,handles,threads=0;LPWSTR sid=NULL;THREADENTRY32 te;PROCESS_MEMORY_COUNTERS_EX mem;WCHAR image[1024];DWORD len=1024;TOKEN_ELEVATION elev;
 if(!cycle_process)cycle_process=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,pid);if(!cycle_process)return lab_error();
 if(!QueryFullProcessImageNameW(cycle_process,0,image,&len)||_wcsicmp(image,cycle_path)||!OpenProcessToken(cycle_process,TOKEN_QUERY|TOKEN_DUPLICATE,&cycle_token))return ERROR_ACCESS_DENIED;
 {MODULEENTRY32W me;unsigned seen=0;WCHAR expected[1024];
 cycle_threads=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);if(cycle_threads==INVALID_HANDLE_VALUE){cycle_threads=NULL;return lab_error();}
 ZeroMemory(&me,sizeof(me));me.dwSize=sizeof(me);if(!Module32FirstW(cycle_threads,&me))return lab_error();
 do{
 printf("LAB_MODULE=%ls; PATH=%ls\n",me.szModule,me.szExePath);
 if(!_wcsicmp(me.szModule,L"libcrypto-3-x64.dll")||!_wcsicmp(me.szModule,L"libssl-3-x64.dll")){
 expected[0]=0;if(!lab_append(expected,1024,lab_parent_path)||!lab_append(expected,1024,L"\\")||!lab_append(expected,1024,me.szModule)||_wcsicmp(expected,me.szExePath))return ERROR_INVALID_NAME;
 seen|=!_wcsicmp(me.szModule,L"libcrypto-3-x64.dll")?1U:2U;
 }
 }while(Module32NextW(cycle_threads,&me));if(GetLastError()!=ERROR_NO_MORE_FILES||seen!=3)return ERROR_INVALID_DATA;
 if(!CloseHandle(cycle_threads)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_threads=NULL;
 puts("LAB_IMPORTED_DLL_PATHS=MATCH_PROTECTED_INSTALLED_FILES; MAPPED_PAGE_ATTESTATION=NOT_CLAIMED");
 }

 if(!GetTokenInformation(cycle_token,TokenStatistics,&stats,sizeof(stats),&n)||stats.TokenType!=TokenPrimary||
 !GetTokenInformation(cycle_token,TokenUser,&u,sizeof(u),&n)||!cycle_sid_inside(u.b,n,u.align.User.Sid)||!EqualSid(u.align.User.Sid,native.service_sid)||
 !ConvertSidToStringSidW(u.align.User.Sid,&sid))return ERROR_ACCESS_DENIED;
 printf("LAB_TOKEN_USER=%ls; PRIMARY=1; PID=%lu\n",sid,(unsigned long)pid);if(LocalFree(sid)){cycle_sid_retained=sid;native.retained=1;return ERROR_INVALID_HANDLE;}sid=NULL;
 if(!GetTokenInformation(cycle_token,TokenGroups,&g,sizeof(g),&n)||n<(DWORD)FIELD_OFFSET(TOKEN_GROUPS,Groups)||g.align.GroupCount>(n-FIELD_OFFSET(TOKEN_GROUPS,Groups))/sizeof(SID_AND_ATTRIBUTES))return ERROR_INVALID_DATA;
 for(i=0;i<g.align.GroupCount;i++){
 if(!cycle_sid_inside(g.b,n,g.align.Groups[i].Sid)||!ConvertSidToStringSidW(g.align.Groups[i].Sid,&sid))return ERROR_INVALID_SID;
 printf("LAB_TOKEN_GROUP=%ls; ATTR=%08lx\n",sid,(unsigned long)g.align.Groups[i].Attributes);if(LocalFree(sid)){cycle_sid_retained=sid;native.retained=1;return ERROR_INVALID_HANDLE;}sid=NULL;}
 if(!GetTokenInformation(cycle_token,TokenPrivileges,&p,sizeof(p),&n)||n<(DWORD)FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges)||p.align.PrivilegeCount>(n-FIELD_OFFSET(TOKEN_PRIVILEGES,Privileges))/sizeof(LUID_AND_ATTRIBUTES))return ERROR_INVALID_DATA;
 for(i=0;i<p.align.PrivilegeCount;i++)printf("LAB_TOKEN_PRIVILEGE=%ld:%lu; ATTR=%08lx\n",(long)p.align.Privileges[i].Luid.HighPart,(unsigned long)p.align.Privileges[i].Luid.LowPart,(unsigned long)p.align.Privileges[i].Attributes);
 if(!GetTokenInformation(cycle_token,TokenElevation,&elev,sizeof(elev),&n))return lab_error();printf("LAB_TOKEN_ELEVATED=%lu; TOKEN_ID=%ld:%lu; AUTH_ID=%ld:%lu; MODIFIED=%ld:%lu\n",(unsigned long)elev.TokenIsElevated,(long)stats.TokenId.HighPart,(unsigned long)stats.TokenId.LowPart,(long)stats.AuthenticationId.HighPart,(unsigned long)stats.AuthenticationId.LowPart,(long)stats.ModifiedId.HighPart,(unsigned long)stats.ModifiedId.LowPart);
 if(!DuplicateToken(cycle_token,SecurityImpersonation,&cycle_access))return lab_error();
 if((e=cycle_access_file(lab_parent,0))||(e=cycle_access_file(lab_config,1))||(e=cycle_access_file(cycle_file,2))||(e=cycle_access_file(cycle_dependencies[0],2))||(e=cycle_access_file(cycle_dependencies[1],2))||(e=cycle_no_endpoints(pid)))return e;
 cycle_threads=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);if(cycle_threads==INVALID_HANDLE_VALUE){cycle_threads=NULL;return lab_error();}
 ZeroMemory(&te,sizeof(te));te.dwSize=sizeof(te);if(!Thread32First(cycle_threads,&te))return lab_error();
 do{if(te.th32OwnerProcessID==pid){A1_LEASE token;DWORD token_error;
 cycle_thread=OpenThread(THREAD_QUERY_INFORMATION,FALSE,te.th32ThreadID);if(!cycle_thread)return lab_error();
 token_error=lab_aux_token(&token,cycle_thread,1,TOKEN_QUERY);if(!token_error)return lab_aux_close_preserving(token,ERROR_ACCESS_DENIED);
 if(token_error!=ERROR_NO_TOKEN)return token_error;threads++;if(!CloseHandle(cycle_thread))return ERROR_INVALID_HANDLE;cycle_thread=NULL;}
 }while(Thread32Next(cycle_threads,&te));if(GetLastError()!=ERROR_NO_MORE_FILES)return lab_error();
 ZeroMemory(&mem,sizeof(mem));mem.cb=sizeof(mem);
 if(!GetProcessHandleCount(cycle_process,&handles)||!GetProcessMemoryInfo(cycle_process,(PROCESS_MEMORY_COUNTERS *)&mem,sizeof(mem)))return lab_error();
 printf("LAB_PROCESS_OBSERVATION threads=%lu handles=%lu working_set=%llu private_bytes=%llu peak_working_set=%llu; IMPERSONATION=NONE_OBSERVED_SNAPSHOT; STACK_KERNEL_PROVIDER=UNKNOWN; ED11=PARTIAL\n",(unsigned long)threads,(unsigned long)handles,(unsigned long long)mem.WorkingSetSize,(unsigned long long)mem.PrivateUsage,(unsigned long long)mem.PeakWorkingSetSize);
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
 /* All successful last uses above have completed; process/image remain owned. */
 if(ig_n0_positive){
  if((e=cycle_live_close_last(&cycle_threads))||(e=cycle_live_close_last(&cycle_access))||(e=cycle_live_close_last(&cycle_token)))return e;
 }
#endif
 return ERROR_SUCCESS;
}
static int cycle_can_retire(void){return !cycle_start_requested||cycle_quiescent;}
static DWORD cycle_cleanup(void)
{
 HANDLE *v[]={&cycle_dependencies[0],&cycle_dependencies[1],&cycle_thread,&cycle_threads,&cycle_access,&cycle_token,&cycle_process,&cycle_file,&cycle_source_file,&lab_config,&lab_parent,&lab_token};unsigned i;DWORD e;
 /* Handles may be closed while persistent effects remain. Never delete/kill. */
 if(native.retained||lab_aux_count||lab_pending_sd||cycle_sid_retained)return ERROR_INVALID_STATE;
 if(cycle_sd){if(LocalFree(cycle_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_sd=NULL;}
 for(i=0;i<sizeof(v)/sizeof(v[0]);i++)if(*v[i]){if(!CloseHandle(*v[i]))return ERROR_INVALID_HANDLE;*v[i]=NULL;}
 if(lab_sid){if(LocalFree(lab_sid))return ERROR_INVALID_HANDLE;lab_sid=NULL;}
 e=ps_close();if(e||lab_aux_count||lab_pending_sd)return e?e:ERROR_INVALID_HANDLE;cycle_closed=1;return ERROR_SUCCESS;
}
#ifdef PAPACC_SCM_INSTRUMENTED_LAB
#include "scm-instrumented-cycle-lab.inc"
#else
int main(int argc,char **argv)
{
 DWORD e=0,close_error,n,k;SERVICE_STATUS_PROCESS status;SERVICE_STATUS stopped;int positive=0;char attempt[33];
 if(argc==2&&!strcmp(argv[1],"--check"))positive=0;
 else if(argc==3&&!strcmp(argv[1],"--cycle-lab")&&strlen(argv[2])==32){positive=1;for(k=0;k<32;k++)if(!((argv[2][k]>='0'&&argv[2][k]<='9')||(argv[2][k]>='a'&&argv[2][k]<='f')))return 2;memcpy(attempt,argv[2],33);}
 else {puts("LAB_CYCLE=ARGUMENT_DENY; SCM_EFFECT=NONE");return 2;}
 setvbuf(stdout,NULL,_IONBF,0);native.thread=GetCurrentThreadId();
 cycle_stage="OPERATOR";if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&lab_sid)){e=lab_error();goto done;}
 if(papacc_server_console_win32_install(&native.console)!=PAPACC_RESULT_OK){e=ERROR_INVALID_STATE;goto done;}
 if(positive&&(e=lab_operator()))goto done;
 cycle_stage="PROTECTED_OLD_FILES";
 lab_parent=CreateFileW(lab_parent_path,READ_CONTROL|FILE_READ_ATTRIBUTES|FILE_TRAVERSE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(lab_parent==INVALID_HANDLE_VALUE){lab_parent=NULL;e=lab_error();goto done;}
 lab_config=CreateFileW(lab_config_path,GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(lab_config==INVALID_HANDLE_VALUE){lab_config=NULL;e=lab_error();goto done;}
 native.image=CreateFileW(lab_image_path,GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(native.image==INVALID_HANDLE_VALUE){native.image=NULL;e=lab_error();goto done;}
 if((e=lab_bound(0))||(e=cycle_dependencies_current(1)))goto done;
 cycle_source_file=CreateFileW(cycle_source,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(cycle_source_file==INVALID_HANDLE_VALUE){cycle_source_file=NULL;e=lab_error();goto done;}
 {BY_HANDLE_FILE_INFORMATION a;if(!GetFileInformationByHandle(cycle_source_file,&a)||(a.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))){e=ERROR_INVALID_DATA;goto done;}}
 if((e=lab_hash(cycle_source_file,cycle_sha)))goto done;
 native.command[0]=0;if(!lab_append(native.command,1050,L"\"")||!lab_append(native.command,1050,lab_image_path)||!lab_append(native.command,1050,L"\" service host")){e=ERROR_INVALID_DATA;goto done;}
 cycle_stage="EXISTING_LAB_READBACK";native.manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT);if(!native.manager){e=lab_error();goto done;}
 native.service=OpenServiceW(native.manager,ps_name,SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS|(positive?(SERVICE_CHANGE_CONFIG|SERVICE_START|SERVICE_STOP):0));if(!native.service){e=lab_error();goto done;}
 if((e=cycle_readback(SERVICE_DISABLED,0))||(e=cycle_status(&status)))goto done;
 if(status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId){e=ERROR_INVALID_STATE;goto done;}
 if(!positive){puts("LAB_CYCLE=PREPARED_READ_ONLY; OLD_REGISTERED_IMAGE_BOUND; SERVICE=DISABLED_STOPPED; CANDIDATE_HASH=PASS; EFFECTS=NONE; START_NOT_RUN");goto done;}
 cycle_stage="CONFIRMATION";if((e=cycle_confirm(attempt)))goto done;
 cycle_stage="REVALIDATE";if((e=lab_operator_current())||(e=lab_bound(1))||(e=cycle_readback(SERVICE_DISABLED,0))||(e=cycle_status(&status)))goto done;
 if(status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId||papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
 cycle_stage="ONE_SHOT_INTENT";if((e=cycle_intent(attempt)))goto done;
 if((e=cycle_copy()))goto done;
 printf("LAB_IMAGE_CREATED=%lu:%lu:%lu; SHA256=%s\n",(unsigned long)cycle_id.dwVolumeSerialNumber,(unsigned long)cycle_id.nFileIndexHigh,(unsigned long)cycle_id.nFileIndexLow,cycle_sha);
 cycle_stage="EXPLICIT_REBIND";if((e=lab_operator_current())||(e=cycle_files())||(e=cycle_readback(SERVICE_DISABLED,0))||(e=cycle_status(&status)))goto done;
 if(status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId||papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
 {WCHAR command[1050];command[0]=0;if(!lab_append(command,1050,L"\"")||!lab_append(command,1050,cycle_path)||!lab_append(command,1050,L"\" service host")){e=ERROR_INVALID_DATA;goto done;}
 cycle_effect=2; /* request may be uncertain, never rollback */
 if(!ChangeServiceConfigW(native.service,SERVICE_NO_CHANGE,SERVICE_DEMAND_START,SERVICE_NO_CHANGE,command,NULL,NULL,NULL,NULL,NULL,NULL)){e=lab_error();goto done;}}
 if((e=cycle_readback(SERVICE_DEMAND_START,1))||(e=cycle_files())||(e=cycle_status(&status)))goto done;
 if(status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId||papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}
 cycle_stage="START_ONCE";cycle_start_requested=1;if(!StartServiceW(native.service,0,NULL)){e=lab_error();goto done;}
 cycle_stage="RUNNING_READINESS";if((e=cycle_wait(SERVICE_RUNNING,SERVICE_START_PENDING,&status)))goto done;
 if(!status.dwProcessId){e=ERROR_INVALID_DATA;goto done;}n=status.dwProcessId;puts("LAB_RUNNING=OBSERVED; PROTECTED_CLOSED_RUNTIME_READINESS=GATED; LISTENER_DISABLED=BUILD_SOURCE_CONTRACT");
 cycle_stage="LIVE_TOKEN_ACCESS_RESOURCES";e=cycle_live(n);if(e)printf("LAB_FIRST_FAILURE STAGE=LIVE_TOKEN_ACCESS_RESOURCES ERROR=%lu\n",(unsigned long)e);
 /* STOP is part of this approved cycle even when evidence capture fails;
  * one safety Stop request only, with original error preserved. */
 cycle_stage="STOP_ONCE";cycle_stop_requested=1;
 if(!ControlService(native.service,SERVICE_CONTROL_STOP,&stopped)){if(!e)e=lab_error();goto done;}
 {DWORD q=cycle_wait(SERVICE_STOPPED,SERVICE_STOP_PENDING,&status);if(q){if(!e)e=q;goto done;}}
 if(status.dwProcessId||status.dwWin32ExitCode||status.dwServiceSpecificExitCode){if(!e)e=ERROR_INVALID_DATA;goto done;}
 if(!cycle_process||WaitForSingleObject(cycle_process,30000)!=WAIT_OBJECT_0){if(!e)e=ERROR_TIMEOUT;goto done;}
 cycle_quiescent=1;
 if(!e&&(e=cycle_files()))goto done;
 if(!e)puts("LAB_CYCLE=RUNNING_TOKEN_ACCESS_STOP_QUIESCENCE_PASS; STOPPED_EXIT0_PROCESS_SIGNALED; NO_CA_NO_CURRENT_NO_AUTHORITY; KSP_ACCESS_NOT_TESTED");
 done:
 if(!cycle_can_retire()){printf("LAB_CYCLE_RETAINED STAGE=%s ERROR=%lu; LIVE_DISPOSITION=UNKNOWN; NO_CLOSE_NO_RETRY_NO_KILL; references retained in qualifier process\n",cycle_stage,(unsigned long)e);fflush(stdout);Sleep(INFINITE);return 2;}
 close_error=cycle_cleanup();
 if(close_error){printf("LAB_CYCLE_RETAINED CLOSE=%lu; NO_SECOND_CLOSE; references retained\n",(unsigned long)close_error);fflush(stdout);Sleep(INFINITE);return 2;}
 printf("LAB_CYCLE_FINAL STAGE=%s ERROR=%lu CLOSE=%lu CLOSED=%u EFFECT=%u START_REQUESTED=%u STOP_REQUESTED=%u; NO_RETRY_NO_DELETE_NO_ROLLBACK; ED11_PARTIAL\n",cycle_stage,(unsigned long)e,(unsigned long)close_error,cycle_closed,cycle_effect,cycle_start_requested,cycle_stop_requested);
 fflush(stdout);return e||close_error?2:0;
}

#endif /* historical or instrumented entry; same owner/helpers */
