/* Private local stdout, no SCM/admission/intent APIs. All resources persistent.
 * Failures preserve first error. Uncertain close/I/O freezes the context forever.
 * Export calls are thread-affine and serialized; no reset/reconnect/relaunch API. */
#define WIN32_LEAN_AND_MEAN 1
#include "scm-n0-private-stdout-native.h"
#include <sddl.h>
#include <aclapi.h>
#include <bcrypt.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
typedef struct {
 HANDLE pipe,event,token[2],parent; PROCESS_INFORMATION child; OVERLAPPED ov;
 PSECURITY_DESCRIPTOR sd,observed_sd; LPWSTR sid; FILE *stream; int fd;
 WCHAR name[96]; BYTE buffer[4096]; DWORD first,frozen,pending,used,phase,thread,eof,released;
 volatile LONG busy; ULONGLONG creation;
} PACC_TRANSPORT;
static PACC_TRANSPORT t;
static DWORD failure(DWORD e,int unknown){if(!e)e=ERROR_GEN_FAILURE;if(!t.first)t.first=e;if(unknown)t.frozen=1;t.phase=6;return e;}
static DWORD enter(void){if(InterlockedCompareExchange(&t.busy,1,0))return ERROR_BUSY;if(t.thread&&t.thread!=GetCurrentThreadId()){InterlockedExchange(&t.busy,0);return ERROR_INVALID_THREAD_ID;}t.thread=GetCurrentThreadId();if(t.frozen){InterlockedExchange(&t.busy,0);return ERROR_INVALID_STATE;}return 0;}
static DWORD leave(DWORD e){InterlockedExchange(&t.busy,0);return e;}
static DWORD close_ref(HANDLE *h){if(!*h)return 0;if(!CloseHandle(*h))return failure(GetLastError(),1);*h=NULL;return 0;}
static ULONGLONG birth(HANDLE p){FILETIME c,e,k,u;if(!GetProcessTimes(p,&c,&e,&k,&u))return 0;return ((ULONGLONG)c.dwHighDateTime<<32)|c.dwLowDateTime;}
static int live(HANDLE p){return WaitForSingleObject(p,0)==WAIT_TIMEOUT;}
static int no_impersonation(void){if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&t.token[0])){if(close_ref(&t.token[0]))return 0;return 0;}return GetLastError()==ERROR_NO_TOKEN;}
static int same_token(HANDLE p){BYTE a[256],b[256];TOKEN_STATISTICS sa,sb;DWORD n,ia,ib;int ok=0;
 if(!no_impersonation())return 0;
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&t.token[0])||!OpenProcessToken(p,TOKEN_QUERY,&t.token[1]))goto done;
 if(!GetTokenInformation(t.token[0],TokenUser,a,sizeof(a),&n)||!GetTokenInformation(t.token[1],TokenUser,b,sizeof(b),&n)||!GetTokenInformation(t.token[0],TokenStatistics,&sa,sizeof(sa),&n)||!GetTokenInformation(t.token[1],TokenStatistics,&sb,sizeof(sb),&n)||!GetTokenInformation(t.token[0],TokenSessionId,&ia,sizeof(ia),&n)||!GetTokenInformation(t.token[1],TokenSessionId,&ib,sizeof(ib),&n))goto done;
 ok=EqualSid(((TOKEN_USER *)a)->User.Sid,((TOKEN_USER *)b)->User.Sid)&&sa.TokenType==TokenPrimary&&sb.TokenType==TokenPrimary&&ia==ib&&sa.AuthenticationId.LowPart==sb.AuthenticationId.LowPart&&sa.AuthenticationId.HighPart==sb.AuthenticationId.HighPart;
 done:if(close_ref(&t.token[1])||close_ref(&t.token[0]))ok=0;return ok;
}
static int endpoint(const WCHAR *n){static const WCHAR prefix[]=L"\\\\.\\pipe\\papacc-n0-stdout-";size_t i,z=wcslen(prefix);if(!n||wcslen(n)!=z+32||wcsncmp(n,prefix,z))return 0;for(i=z;i<z+32;i++)if(!((n[i]>=L'0'&&n[i]<=L'9')||(n[i]>=L'a'&&n[i]<=L'f')))return 0;return 1;}
static DWORD reset_io(void){if(t.pending||t.frozen)return failure(ERROR_INVALID_STATE,1);ZeroMemory(&t.ov,sizeof(t.ov));t.ov.hEvent=t.event;if(!ResetEvent(t.event))return failure(GetLastError(),1);return 0;}
/* Pending buffer belongs to t, never caller/GC memory. Cancel is not completion. */
static DWORD complete(BOOL ok,DWORD *n,int bounded){DWORD e,elapsed=0;if(ok){t.pending=0;if(!GetOverlappedResult(t.pipe,&t.ov,n,FALSE))return failure(GetLastError(),0);return 0;}e=GetLastError();if(e!=ERROR_IO_PENDING){t.pending=0;return (!bounded&&e==ERROR_BROKEN_PIPE)?e:failure(e,0);}t.pending=1;
 for(;;){e=WaitForSingleObject(t.event,250);if(e==WAIT_OBJECT_0)break;if(e!=WAIT_TIMEOUT)return failure(GetLastError(),1);elapsed+=250;if((bounded&&elapsed>=10000)||(!bounded&&t.child.hProcess&&!live(t.child.hProcess))){
 if(!CancelIoEx(t.pipe,&t.ov)&&GetLastError()!=ERROR_NOT_FOUND)return failure(GetLastError(),1);
 if(WaitForSingleObject(t.event,5000)!=WAIT_OBJECT_0)return failure(ERROR_TIMEOUT,1);
 if(!GetOverlappedResult(t.pipe,&t.ov,n,FALSE)){e=GetLastError();t.pending=0;if(e!=ERROR_OPERATION_ABORTED&&e!=ERROR_BROKEN_PIPE)return failure(e,0);return bounded?failure(ERROR_TIMEOUT,0):ERROR_BROKEN_PIPE;}t.pending=0;return 0;}}
 ok=GetOverlappedResult(t.pipe,&t.ov,n,FALSE);e=ok?0:GetLastError();t.pending=0;return e?((!bounded&&e==ERROR_BROKEN_PIPE)?e:failure(e,0)):0;
}
static DWORD io(int write,DWORD bytes,DWORD *n,int bounded){BOOL ok;DWORD e=reset_io();if(e)return e;*n=0;ok=write?WriteFile(t.pipe,t.buffer,bytes,NULL,&t.ov):ReadFile(t.pipe,t.buffer,bytes,NULL,&t.ov);return complete(ok,n,bounded);}
DWORD WINAPI PaccTransportOpen(void){DWORD e=enter(),n,i;BYTE user[256],random[16];WCHAR descriptor[512];SECURITY_ATTRIBUTES sa;PACL dacl;SECURITY_DESCRIPTOR_CONTROL control;DWORD revision;BYTE system[SECURITY_MAX_SID_SIZE];DWORD system_size=sizeof(system);if(e)return e;
 if(t.used)return leave(ERROR_INVALID_STATE);t.used=1;
 if(!no_impersonation()){e=failure(ERROR_ACCESS_DENIED,0);goto done;}
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&t.token[0])||!GetTokenInformation(t.token[0],TokenUser,user,sizeof(user),&n)||!ConvertSidToStringSidW(((TOKEN_USER *)user)->User.Sid,&t.sid)){e=failure(GetLastError(),0);goto done;}
 if(_snwprintf_s(descriptor,512,_TRUNCATE,L"D:P(A;;GA;;;SY)(A;;0x00120183;;;%ls)",t.sid)<0){e=failure(ERROR_INVALID_DATA,0);goto done;}
 if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(descriptor,SDDL_REVISION_1,&t.sd,NULL)){e=failure(GetLastError(),0);goto done;}
 if(BCryptGenRandom(NULL,random,16,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0){e=failure(ERROR_GEN_FAILURE,0);goto done;}
 wcscpy_s(t.name,96,L"\\\\.\\pipe\\papacc-n0-stdout-");n=(DWORD)wcslen(t.name);for(i=0;i<16;i++){t.name[n+2*i]=L"0123456789abcdef"[random[i]>>4];t.name[n+2*i+1]=L"0123456789abcdef"[random[i]&15];}t.name[n+32]=0;
 sa.nLength=sizeof(sa);sa.lpSecurityDescriptor=t.sd;sa.bInheritHandle=FALSE;
 t.pipe=CreateNamedPipeW(t.name,PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED|FILE_FLAG_FIRST_PIPE_INSTANCE,PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT|PIPE_REJECT_REMOTE_CLIENTS,1,4096,4096,0,&sa);if(t.pipe==INVALID_HANDLE_VALUE){t.pipe=NULL;e=failure(GetLastError(),0);goto done;}
 e=GetSecurityInfo(t.pipe,SE_KERNEL_OBJECT,DACL_SECURITY_INFORMATION,NULL,NULL,&dacl,NULL,&t.observed_sd);if(e){e=failure(e,0);goto done;}
 if(!GetSecurityDescriptorControl(t.observed_sd,&control,&revision)||!(control&SE_DACL_PROTECTED)||!dacl||dacl->AceCount!=2||!CreateWellKnownSid(WinLocalSystemSid,NULL,system,&system_size)){e=failure(ERROR_INVALID_ACL,0);goto done;}
 for(i=0;i<2;i++){ACCESS_ALLOWED_ACE *ace=NULL;if(!GetAce(dacl,i,(void **)&ace)||ace->Header.AceType!=ACCESS_ALLOWED_ACE_TYPE||ace->Header.AceFlags||ace->Mask!=(DWORD)(i?0x00120183:FILE_ALL_ACCESS)||!EqualSid(&ace->SidStart,i?((TOKEN_USER *)user)->User.Sid:(PSID)system)){e=failure(ERROR_INVALID_ACL,0);goto done;}}
 t.event=CreateEventW(NULL,TRUE,FALSE,NULL);if(!t.event){e=failure(GetLastError(),0);goto done;}
 if((e=reset_io()))goto done;
 if(!ConnectNamedPipe(t.pipe,&t.ov)){e=GetLastError();if(e==ERROR_IO_PENDING){t.pending=1;e=0;}else{e=failure(e,0);goto done;}}
 t.phase=1;e=0;
 done:if(t.observed_sd){if(LocalFree(t.observed_sd))e=failure(ERROR_INVALID_HANDLE,1);else t.observed_sd=NULL;}if(t.sd){if(LocalFree(t.sd))e=failure(ERROR_INVALID_HANDLE,1);else t.sd=NULL;}if(t.sid){if(LocalFree(t.sid))e=failure(ERROR_INVALID_HANDLE,1);else t.sid=NULL;}if(close_ref(&t.token[0]))e=t.first;return leave(e);
}
DWORD WINAPI PaccTransportCreate(const WCHAR *exe,const WCHAR *arguments){DWORD e=enter();WCHAR command[4096];STARTUPINFOW si;ULONGLONG parent_creation;if(e)return e;if(t.phase!=1)return leave(ERROR_INVALID_STATE);
 parent_creation=birth(GetCurrentProcess());if(!parent_creation||!exe||!arguments||wcschr(exe,L'"')||_snwprintf_s(command,4096,_TRUNCATE,L"\"%ls\" %ls --private-stdout %ls %lu %llu",exe,arguments,t.name,(unsigned long)GetCurrentProcessId(),parent_creation)<0)return leave(failure(ERROR_INVALID_PARAMETER,0));
 ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);
 if(!CreateProcessW(exe,command,NULL,NULL,FALSE,0,NULL,NULL,&si,&t.child)){e=failure(GetLastError(),0);if(t.child.hProcess||t.child.hThread||t.child.dwProcessId||t.child.dwThreadId)e=failure(e,1);return leave(e);}
 t.creation=birth(t.child.hProcess);if(!t.creation)return leave(failure(GetLastError(),1));if((e=close_ref(&t.child.hThread)))return leave(e);t.phase=2;return leave(0);
}
DWORD WINAPI PaccTransportAuthenticate(void){DWORD e=enter(),n=0,pid=0;if(e)return e;if(t.phase!=2)return leave(ERROR_INVALID_STATE);
 if(t.pending){if(WaitForSingleObject(t.event,10000)!=WAIT_OBJECT_0)return leave(failure(ERROR_TIMEOUT,1));if(!GetOverlappedResult(t.pipe,&t.ov,&n,FALSE)){e=GetLastError();t.pending=0;return leave(failure(e,0));}t.pending=0;}
 if(!live(t.child.hProcess)||birth(t.child.hProcess)!=t.creation||!GetNamedPipeClientProcessId(t.pipe,&pid)||pid!=t.child.dwProcessId||!same_token(t.child.hProcess))return leave(failure(ERROR_ACCESS_DENIED,0));
 e=io(0,1,&n,1);if(e)return leave(e);if(n!=1||t.buffer[0]!='H'||!live(t.child.hProcess)||birth(t.child.hProcess)!=t.creation||!no_impersonation())return leave(failure(ERROR_ACCESS_DENIED,0));
 t.released=1;t.buffer[0]='A';e=io(1,1,&n,1);if(e)return leave(e);if(n!=1)return leave(failure(ERROR_WRITE_FAULT,0));t.phase=3;return leave(0);
}
DWORD WINAPI PaccTransportRead(BYTE *out,DWORD capacity,DWORD *length){DWORD e=enter(),n=0;if(e)return e;if(!out||!length||!capacity||capacity>sizeof(t.buffer)||t.phase!=3)return leave(ERROR_INVALID_PARAMETER);*length=0;if(t.eof)return leave(0);
 e=io(0,capacity,&n,0);if(e==ERROR_BROKEN_PIPE){t.eof=1;return leave(0);}if(e)return leave(e);memcpy(out,t.buffer,n);*length=n;if(!n)t.eof=1;return leave(0);
}
DWORD WINAPI PaccTransportWait(DWORD *code){DWORD e=enter();if(e)return e;if(!code||!t.child.hProcess)return leave(ERROR_INVALID_STATE);if(WaitForSingleObject(t.child.hProcess,10000)!=WAIT_OBJECT_0)return leave(failure(ERROR_TIMEOUT,1));if(!GetExitCodeProcess(t.child.hProcess,code))return leave(failure(GetLastError(),1));t.phase=4;return leave(0);}
DWORD WINAPI PaccTransportClose(void){DWORD e=enter(),n=0;if(e)return e;if(t.child.hProcess&&live(t.child.hProcess)&&(t.released||t.phase!=6))return leave(failure(ERROR_BUSY,1));
 if(t.pending){if(!CancelIoEx(t.pipe,&t.ov)&&GetLastError()!=ERROR_NOT_FOUND)return leave(failure(GetLastError(),1));if(WaitForSingleObject(t.event,5000)!=WAIT_OBJECT_0)return leave(failure(ERROR_TIMEOUT,1));if(!GetOverlappedResult(t.pipe,&t.ov,&n,FALSE)&&GetLastError()!=ERROR_OPERATION_ABORTED)return leave(failure(GetLastError(),1));t.pending=0;}
 if(close_ref(&t.pipe)||close_ref(&t.event))return leave(t.first);if(t.child.hProcess&&WaitForSingleObject(t.child.hProcess,10000)!=WAIT_OBJECT_0)return leave(failure(ERROR_TIMEOUT,1));if(close_ref(&t.child.hThread)||close_ref(&t.child.hProcess))return leave(t.first);t.phase=5;return leave(0);
}
DWORD WINAPI PaccTransportStatus(DWORD *pid,ULONGLONG *creation,DWORD *first,DWORD *retained){if(!pid||!creation||!first||!retained)return ERROR_INVALID_PARAMETER;*pid=t.child.dwProcessId;*creation=t.creation;*first=t.first;*retained=t.frozen;return 0;}
DWORD WINAPI PaccTransportClient(const WCHAR *name,DWORD pid,ULONGLONG creation){DWORD e=enter(),n,server,flags;char ack=0;if(e)return e;if(t.used)return leave(ERROR_INVALID_STATE);t.used=1;t.fd=-1;if(!endpoint(name)||!pid||!creation)return leave(failure(ERROR_INVALID_PARAMETER,0));
 if(!no_impersonation())return leave(failure(ERROR_ACCESS_DENIED,0));t.parent=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,pid);if(!t.parent)return leave(failure(GetLastError(),0));
 if(birth(t.parent)!=creation||!live(t.parent)||!same_token(t.parent))return leave(failure(ERROR_ACCESS_DENIED,0));
 t.pipe=CreateFileW(name,0x00120183,0,NULL,OPEN_EXISTING,SECURITY_SQOS_PRESENT|SECURITY_ANONYMOUS,NULL);if(t.pipe==INVALID_HANDLE_VALUE){t.pipe=NULL;return leave(failure(GetLastError(),0));}
 if(!GetNamedPipeServerProcessId(t.pipe,&server)||server!=pid||birth(t.parent)!=creation||!live(t.parent)||!GetHandleInformation(t.pipe,&flags)||(flags&HANDLE_FLAG_INHERIT))return leave(failure(ERROR_ACCESS_DENIED,0));
 if(!WriteFile(t.pipe,"H",1,&n,NULL)||n!=1||!ReadFile(t.pipe,&ack,1,&n,NULL)||n!=1||ack!='A')return leave(failure(ERROR_INVALID_DATA,0));
 if(!no_impersonation()||birth(t.parent)!=creation||!live(t.parent))return leave(failure(ERROR_ACCESS_DENIED,0));if((e=close_ref(&t.parent)))return leave(e);
 if(!SetStdHandle(STD_OUTPUT_HANDLE,t.pipe))return leave(failure(GetLastError(),0));
 t.fd=_open_osfhandle((intptr_t)t.pipe,_O_WRONLY|_O_BINARY|_O_NOINHERIT);if(t.fd<0)return leave(failure(ERROR_INVALID_HANDLE,1));t.stream=_fdopen(t.fd,"wb");if(!t.stream)return leave(failure(ERROR_NOT_ENOUGH_MEMORY,1));
 if(setvbuf(t.stream,NULL,_IONBF,0)||!GetHandleInformation(t.pipe,&flags)||(flags&HANDLE_FLAG_INHERIT))return leave(failure(ERROR_INVALID_DATA,1));t.phase=3;return leave(0);
}
static DWORD crt_failure(void){unsigned long original=0;_get_doserrno(&original);return failure(original?(DWORD)original:ERROR_WRITE_FAULT,1);}
int PaccTransportVPrintf(const char *format,va_list args){int n;DWORD e=enter();if(e)return -1;if(!t.stream)return (int)leave((DWORD)-1);_set_doserrno(0);n=vfprintf(t.stream,format,args);if(n<0)crt_failure();leave(0);return n;}
int PaccTransportPrintf(const char *format,...){int n;va_list args;va_start(args,format);n=PaccTransportVPrintf(format,args);va_end(args);return n;}
int PaccTransportPuts(const char *s){DWORD e=enter();if(e)return EOF;if(!t.stream){leave(0);return EOF;}_set_doserrno(0);if(fputs(s,t.stream)==EOF||fputc('\n',t.stream)==EOF){crt_failure();leave(0);return EOF;}leave(0);return 0;}
int PaccTransportFlush(void){DWORD e=enter();if(e)return EOF;if(!t.stream){leave(0);return EOF;}_set_doserrno(0);if(fflush(t.stream)){crt_failure();leave(0);return EOF;}leave(0);return 0;}
DWORD PaccTransportClientClose(void){DWORD e=enter();if(e)return e;if(!t.stream){if(t.fd>=0)return leave(failure(ERROR_INVALID_STATE,1));if(close_ref(&t.pipe)||close_ref(&t.parent))return leave(t.first);return leave(0);}_set_doserrno(0);if(fflush(t.stream))return leave(crt_failure());if(!SetStdHandle(STD_OUTPUT_HANDLE,NULL))return leave(failure(GetLastError(),1));_set_doserrno(0);if(fclose(t.stream))return leave(crt_failure());t.stream=NULL;t.pipe=NULL;t.fd=-1;t.phase=5;return leave(0);}
