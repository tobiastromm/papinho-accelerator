#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <stdio.h>
static HANDLE injected_close_handle;static DWORD injected_closes;static int injected_pid,inject_token,token_calls,inject_fclose;
static BOOL WINAPI fixture_close(HANDLE h){if(h==injected_close_handle&&h){injected_closes++;SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return CloseHandle(h);}
static BOOL WINAPI fixture_client_pid(HANDLE h,PULONG pid){BOOL ok=GetNamedPipeClientProcessId(h,pid);if(ok&&injected_pid)(*pid)++;return ok;}
static BOOL WINAPI fixture_token(HANDLE h,TOKEN_INFORMATION_CLASS c,LPVOID v,DWORD n,PDWORD used){BOOL ok=GetTokenInformation(h,c,v,n,used);if(ok&&inject_token&&c==TokenStatistics&&token_calls++==0)((TOKEN_STATISTICS *)v)->AuthenticationId.LowPart^=1;return ok;}
static int fixture_fclose(FILE *f){if(inject_fclose){injected_closes++;return EOF;}return fclose(f);}
#define CloseHandle fixture_close
#define GetNamedPipeClientProcessId fixture_client_pid
#define GetTokenInformation fixture_token
#define fclose fixture_fclose
#include "../tools/qualification/scm-n0-private-stdout-native.c"
#undef CloseHandle
#undef GetNamedPipeClientProcessId
#undef GetTokenInformation
#undef fclose
static struct {unsigned retained;} fixture_runtime;static unsigned fixture_stop;
static void papacc_server_console_win32_request_stop(void){fixture_stop=1;}
#define native fixture_runtime
#include "../tools/qualification/scm-n0-private-stdout-adapter.inc"
#undef printf
#undef puts
#undef fflush
#undef native
#include <processsnapshot.h>
#include <stdlib.h>
static DWORD handles(void){DWORD n=0;if(!GetProcessHandleCount(GetCurrentProcess(),&n))return MAXDWORD;return n;}
static int competitor(void){Sleep(250);return 0;}
static int sentinel_matches(DWORD parent,WCHAR **values){HANDLE p,h=NULL;DWORD i,flags,matches=0,present[8];typedef BOOL(WINAPI *COMPARE)(HANDLE,HANDLE);COMPARE compare;FARPROC v=GetProcAddress(GetModuleHandleW(L"kernelbase.dll"),"CompareObjectHandles");if(!v)return -1;memcpy(&compare,&v,sizeof(compare));for(i=0;i<8;i++)present[i]=(DWORD)GetHandleInformation((HANDLE)(ULONG_PTR)_wcstoui64(values[i],NULL,10),&flags);p=OpenProcess(PROCESS_DUP_HANDLE,FALSE,parent);if(!p)return -1;for(i=0;i<8;i++){HANDLE claimed=(HANDLE)(ULONG_PTR)_wcstoui64(values[i],NULL,10);if(!present[i])continue;if(!DuplicateHandle(p,claimed,GetCurrentProcess(),&h,0,FALSE,DUPLICATE_SAME_ACCESS))return -1;if(compare(claimed,h))matches++;if(!CloseHandle(h))return -1;h=NULL;}if(!CloseHandle(p))return -1;return (int)matches;}
static int child(int argc,WCHAR **argv){DWORD e,n,mode;unsigned i;WCHAR text[64];HANDLE input=NULL,error=NULL;INPUT_RECORD key;
 if(argc!=7&&argc!=15)return 10;
 if(!wcscmp(argv[2],L"delayed"))Sleep(750);
 printf("FIXTURE_CHILD_PRE=%lu\n",(unsigned long)handles());
 e=PaccTransportClient(argv[argc-3],wcstoul(argv[argc-2],NULL,10),_wcstoui64(argv[argc-1],NULL,10));if(e){fprintf(stderr,"CLIENT_DENY %lu\n",(unsigned long)e);return 11;}
 if(!wcscmp(argv[2],L"console")||!wcscmp(argv[2],L"positive-console")){input=CreateFileW(L"CONIN$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);error=CreateFileW(L"CONOUT$",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);if(input==INVALID_HANDLE_VALUE||error==INVALID_HANDLE_VALUE||!GetConsoleMode(input,&mode))return 12;if(!WriteConsoleW(error,L"TRANSPORT_STDERR\r\n",18,&n,NULL))return 13;if(!ReadConsoleW(input,text,63,&n,NULL)||n>=64)return 14;text[n]=0;if(wcscmp(text,L"CONFIRM_TRANSPORT\r\n"))return 15;do{if(!ReadConsoleInputW(input,&key,1,&n))return 16;}while(key.EventType!=KEY_EVENT||!key.Event.KeyEvent.bKeyDown||key.Event.KeyEvent.wVirtualKeyCode!=VK_ESCAPE);if(!CloseHandle(input)||!CloseHandle(error))return 17;}
 if(argc==15){int matches=sentinel_matches(wcstoul(argv[argc-2],NULL,10),argv+3);ig_transport_printf("SENTINEL_MATCHES=%d\n",matches);if(matches!=0)return 63;}
 if(!wcscmp(argv[2],L"write-fail")){int wrote;unsigned long original=0;Sleep(300);wrote=PaccTransportPrintf("BROKEN_STDOUT\n");_get_doserrno(&original);fprintf(stderr,"WRITE_FAULT_OBSERVED dos=%lu first=%lu frozen=%lu\n",original,(unsigned long)t.first,(unsigned long)t.frozen);if(wrote>=0||!t.frozen||!t.stream||t.first!=(original?(DWORD)original:ERROR_WRITE_FAULT)||PaccTransportOpen()!=ERROR_INVALID_STATE)return 65;fprintf(stderr,"WRITE_FAULT_UNKNOWN_RETAINED no_new_acquisition NO_RETRY; isolated_exit_only\n");ExitProcess(0);}
 ig_transport_printf("FIXTURE_CHILD_CONNECTED=%lu\n",(unsigned long)handles());
 ig_transport_printf("FIXTURE_STDOUT\n\nUTF16_HEX=03a9\n");
 if(!wcscmp(argv[2],L"large")){for(i=0;i<160000;i++)if(ig_transport_printf("x")<0)return 18;ig_transport_printf("\n");}
 if(!wcscmp(argv[2],L"positive")||!wcscmp(argv[2],L"positive-console"))ig_transport_printf("IG_FINAL stage=FIXTURE first=0 close=0 starts=1 stops=1 quiescent=1 intent_consumed=1 effect=2 diagnostic_created=1; SYNTHETIC_NOT_OPERATIONAL\n");
 else if(!wcscmp(argv[2],L"positive-partial"))ig_transport_printf("IG_FINAL stage=FIXTURE first=1816 close=0 starts=0 stops=0 quiescent=0 intent_consumed=1 effect=2 diagnostic_created=1; SYNTHETIC_NOT_OPERATIONAL\n");
 else if(!wcscmp(argv[2],L"positive-unknown"))ig_transport_printf("IG_FINAL stage=FIXTURE first=5 close=0 starts=1 stops=0 quiescent=0 intent_consumed=1 effect=2 diagnostic_created=1; SYNTHETIC_NOT_OPERATIONAL\n");
 else if(!wcscmp(argv[2],L"positive-inconsistent"))ig_transport_printf("IG_FINAL stage=FIXTURE first=0 close=0 starts=0 stops=1 quiescent=1 intent_consumed=0 effect=0 diagnostic_created=0; SYNTHETIC_NOT_OPERATIONAL\n");
 else if(wcscmp(argv[2],L"missing-final"))ig_transport_printf("IG_FINAL stage=FIXTURE first=0 close=0 starts=0 stops=0 quiescent=0 intent_consumed=0 effect=0 diagnostic_created=0; SYNTHETIC_NOT_OPERATIONAL\n");
 ig_transport_puts("FIXTURE_PUTS");if(ig_transport_flush(stdout))return 68;
 e=PaccTransportClientClose();return e?19:(!wcscmp(argv[2],L"positive-partial")?2:0);
}
static int audit(HANDLE process){HPSS snapshot=NULL;HPSSWALK walk=NULL;PSS_HANDLE_ENTRY row;DWORD e,matches=0;HANDLE h=NULL;typedef BOOL(WINAPI *COMPARE)(HANDLE,HANDLE);COMPARE compare;FARPROC ptr=GetProcAddress(GetModuleHandleW(L"kernelbase.dll"),"CompareObjectHandles");if(!ptr)return 20;memcpy(&compare,&ptr,sizeof(compare));
 e=PssCaptureSnapshot(process,(PSS_CAPTURE_FLAGS)(PSS_CAPTURE_HANDLES|PSS_CAPTURE_HANDLE_BASIC_INFORMATION|PSS_CAPTURE_HANDLE_NAME_INFORMATION|PSS_CAPTURE_HANDLE_TYPE_SPECIFIC_INFORMATION),0,&snapshot);if(e)return 21;if(PssWalkMarkerCreate(NULL,&walk))return 22;
 for(;;){ZeroMemory(&row,sizeof(row));e=PssWalkSnapshot(snapshot,PSS_WALK_HANDLES,walk,&row,sizeof(row));if(e==ERROR_NO_MORE_ITEMS)break;if(e)return 23;if(!DuplicateHandle(process,row.Handle,GetCurrentProcess(),&h,0,FALSE,DUPLICATE_SAME_ACCESS))return 24;if(compare(h,t.pipe)||compare(h,t.event)||compare(h,t.child.hProcess))matches++;if(!CloseHandle(h))return 25;h=NULL;}
 if(PssWalkMarkerFree(walk)||PssFreeSnapshot(GetCurrentProcess(),snapshot))return 26;printf("COMPETITOR_PRIVATE_MATCHES=%lu\n",(unsigned long)matches);return matches?27:0;}
typedef struct {WCHAR exe[MAX_PATH];DWORD error;} RACE;
static DWORD WINAPI race_creator(void *arg){RACE *r=(RACE *)arg;unsigned i;for(i=0;i<5;i++){STARTUPINFOW si;PROCESS_INFORMATION p;WCHAR cmd[1024];int e;ZeroMemory(&si,sizeof(si));ZeroMemory(&p,sizeof(p));si.cb=sizeof(si);_snwprintf_s(cmd,1024,_TRUNCATE,L"\"%ls\" --competitor",r->exe);if(!CreateProcessW(r->exe,cmd,NULL,NULL,TRUE,CREATE_SUSPENDED,NULL,NULL,&si,&p)){r->error=70;return 70;}e=audit(p.hProcess);if(ResumeThread(p.hThread)==MAXDWORD||WaitForSingleObject(p.hProcess,10000)!=WAIT_OBJECT_0||!CloseHandle(p.hThread)||!CloseHandle(p.hProcess)){r->error=71;return 71;}if(e){r->error=(DWORD)e;return (DWORD)e;}}return 0;}
static int server(const WCHAR *exe,const WCHAR *mode){WCHAR args[128],command[1024];DWORD e,n,code,base=handles(),peak=base,now;BYTE out[4096];STARTUPINFOW si;PROCESS_INFORMATION other,intruder;int found=0;char assembled[8192];size_t used=0;HANDLE racing[2]={NULL,NULL};RACE race[2];unsigned j;
 e=PaccTransportOpen();if(e){printf("OPEN_ERROR=%lu\n",(unsigned long)e);return 30;}
 if(!wcscmp(mode,L"reentrant")){t.busy=1;e=PaccTransportOpen();t.busy=0;if(e!=ERROR_BUSY)return 31;}
 if(!wcscmp(mode,L"create-fail")){e=PaccTransportCreate(L"C:\\papacc-fixture-missing-exe.exe",L"");if(e!=ERROR_FILE_NOT_FOUND)return 32;if(PaccTransportClose())return 33;printf("CREATE_FAIL_CLOSED first=%lu\n",(unsigned long)t.first);return 0;}
 _snwprintf_s(args,128,_TRUNCATE,L"--fixture-child %ls",!wcscmp(mode,L"spoof-replay")?L"delayed":mode);if((e=PaccTransportCreate(exe,args)))return 34;
 ZeroMemory(&intruder,sizeof(intruder));if(!wcscmp(mode,L"spoof-replay")){ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);_snwprintf_s(command,1024,_TRUNCATE,L"\"%ls\" --spoof-client %ls",exe,t.name);if(!CreateProcessW(exe,command,NULL,NULL,FALSE,0,NULL,NULL,&si,&intruder))return 74;}
 ZeroMemory(&si,sizeof(si));ZeroMemory(&other,sizeof(other));si.cb=sizeof(si);_snwprintf_s(command,1024,_TRUNCATE,L"\"%ls\" --competitor",exe);
 if(!CreateProcessW(exe,command,NULL,NULL,TRUE,CREATE_SUSPENDED,NULL,NULL,&si,&other))return 35;
 e=(DWORD)audit(other.hProcess);if(ResumeThread(other.hThread)==MAXDWORD)return 36;if(WaitForSingleObject(other.hProcess,10000)!=WAIT_OBJECT_0)return 37;if(!CloseHandle(other.hThread)||!CloseHandle(other.hProcess))return 38;if(e)return (int)e;
 if(!wcscmp(mode,L"wrong-peer"))injected_pid=1;if(!wcscmp(mode,L"token-deny"))inject_token=1;
 if((e=PaccTransportAuthenticate())){if(injected_pid||inject_token||intruder.hProcess){if(e!=ERROR_ACCESS_DENIED||PaccTransportClose()||t.child.hProcess||t.pipe||t.event)return 43;if(intruder.hProcess){DWORD intruder_code;if(WaitForSingleObject(intruder.hProcess,10000)!=WAIT_OBJECT_0||!GetExitCodeProcess(intruder.hProcess,&intruder_code)||intruder_code||!CloseHandle(intruder.hThread)||!CloseHandle(intruder.hProcess))return 75;printf("REAL_WRONG_PROCESS_REPLAY_DENIED no_ack no_payload ORIGINAL_CHILD_DISTINCT\n");}printf("AUTH_DENIED_CLOSED first=%lu released=%lu\n",(unsigned long)t.first,(unsigned long)t.released);return 0;}printf("AUTH_ERROR=%lu\n",(unsigned long)e);return 39;}
 if(!wcscmp(mode,L"write-fail")){if(!CloseHandle(t.pipe))return 66;t.pipe=NULL;if(PaccTransportWait(&code)||code||PaccTransportClose())return 67;printf("SERVER_WRITE_FAULT_CONTAINMENT_PASS child_UNKNOWN_not_product_cleanup\n");return 0;}
 if(!wcscmp(mode,L"race")){for(j=0;j<2;j++){ZeroMemory(&race[j],sizeof(race[j]));wcscpy_s(race[j].exe,MAX_PATH,exe);racing[j]=CreateThread(NULL,0,race_creator,&race[j],0,NULL);if(!racing[j])return 72;}}
 do{e=PaccTransportRead(out,sizeof(out),&n);if(e)return 40;now=handles();if(now>peak)peak=now;if(used+n<sizeof(assembled)){memcpy(assembled+used,out,n);used+=n;assembled[used]=0;}if(n&&strstr(assembled,"IG_FINAL"))found=1;}while(n);
 for(j=0;j<2;j++)if(racing[j]){if(WaitForSingleObject(racing[j],20000)!=WAIT_OBJECT_0||race[j].error||!CloseHandle(racing[j]))return 73;racing[j]=NULL;}
 if(PaccTransportWait(&code)||code||PaccTransportClose())return 41;
 printf("CAPTURED_CONNECTED=%u\n",(unsigned)(strstr(assembled,"FIXTURE_CHILD_CONNECTED=")!=NULL));
 printf("INTEGRATED_CORE_PASS base=%lu sampled_peak=%lu after=%lu final=%d state_bytes=%zu NO_OPERATION\n",(unsigned long)base,(unsigned long)peak,(unsigned long)handles(),found,sizeof(t));return found&&!t.pipe&&!t.event&&!t.child.hProcess&&!t.child.hThread&&!t.pending?0:42;}
static int faults(const WCHAR *mode){DWORD e,flags,n;ULONGLONG when=birth(GetCurrentProcess());
 if(!wcscmp(mode,L"name-deny")){e=PaccTransportClient(L"\\\\remote.invalid\\pipe\\bad",GetCurrentProcessId(),when);return e==ERROR_INVALID_PARAMETER&&!t.parent&&!t.pipe?0:50;}
 if(!wcscmp(mode,L"birth-deny")){e=PaccTransportClient(L"\\\\.\\pipe\\papacc-n0-stdout-00000000000000000000000000000000",GetCurrentProcessId(),when+1);return e==ERROR_ACCESS_DENIED&&!PaccTransportClientClose()&&!t.parent&&!t.pipe?0:51;}
 if(!wcscmp(mode,L"impersonation")){if(!ImpersonateSelf(SecurityImpersonation))return 52;e=PaccTransportOpen();if(!RevertToSelf())return 53;return e==ERROR_ACCESS_DENIED&&!t.pipe&&!t.token[0]?0:54;}
 if(!wcscmp(mode,L"close-unknown")){if(PaccTransportOpen())return 55;injected_close_handle=t.event;e=PaccTransportClose();if(e!=ERROR_ACCESS_DENIED||!t.frozen||t.event!=injected_close_handle||t.first!=ERROR_ACCESS_DENIED||injected_closes!=1||!GetHandleInformation(t.event,&flags))return 56;if(PaccTransportOpen()!=ERROR_INVALID_STATE||PaccTransportClose()!=ERROR_INVALID_STATE||injected_closes!=1)return 57;fprintf(stderr,"UNKNOWN_RETAINED one_attempt no_new_acquisition NO_RETRY; fixture_process_exit_is_not_product_cleanup\n");return 0;}
 if(!wcscmp(mode,L"fclose-unknown")){t.used=1;t.fd=-1;t.pipe=CreateFileW(L"NUL",GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);if(t.pipe==INVALID_HANDLE_VALUE)return 58;t.fd=_open_osfhandle((intptr_t)t.pipe,_O_NOINHERIT|_O_WRONLY);if(t.fd<0)return 59;t.stream=_fdopen(t.fd,"wb");if(!t.stream)return 60;inject_fclose=1;e=PaccTransportClientClose();n=injected_closes;if(e!=ERROR_WRITE_FAULT||!t.frozen||!t.stream||!GetHandleInformation(t.pipe,&flags)||PaccTransportClientClose()!=ERROR_INVALID_STATE||injected_closes!=n)return 61;fprintf(stderr,"FILE_UNKNOWN_RETAINED no_double_close NO_RETRY\n");return 0;}
 return 62;
}
int wmain(int argc,WCHAR **argv){WCHAR exe[MAX_PATH];if(argc==3&&!wcscmp(argv[1],L"--spoof-client")){HANDLE h;DWORD n;char ack=0;BOOL ok;h=CreateFileW(argv[2],0x00120183,0,NULL,OPEN_EXISTING,SECURITY_SQOS_PRESENT|SECURITY_ANONYMOUS,NULL);if(h==INVALID_HANDLE_VALUE)return 76;if(!WriteFile(h,"HOLD_SESSION_STDOUT_AND_FINAL",28,&n,NULL)||n!=28)return 77;ok=ReadFile(h,&ack,1,&n,NULL);if(!CloseHandle(h))return 78;return ok&&n?79:0;}if(argc==11&&!wcscmp(argv[1],L"--legacy-baseline")){DWORD before=handles();int matches=sentinel_matches(wcstoul(argv[2],NULL,10),argv+3);printf("LEGACY_CHILD_BASELINE=%lu SENTINEL_MATCHES=%d\n",(unsigned long)before,matches);return matches==8?0:64;}if(argc>1&&!wcscmp(argv[1],L"--fixture-child"))return child(argc,argv);if(argc==2&&!wcscmp(argv[1],L"--competitor"))return competitor();if(argc==3&&!wcscmp(argv[1],L"--fault"))return faults(argv[2]);if(!GetModuleFileNameW(NULL,exe,MAX_PATH))return 1;return server(exe,argc==2?argv[1]:L"clean");}
