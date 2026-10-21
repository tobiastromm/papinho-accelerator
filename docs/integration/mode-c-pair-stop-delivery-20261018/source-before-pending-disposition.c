/* Offline fixed self-child launcher qualification. No product, network, kill,
 * PID reopen or replacement supervisor. Links the existing server console path. */
#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <io.h>
#include "server_console_win32.h"
typedef struct CHILD {PROCESS_INFORMATION pi;unsigned __int64 creation;const wchar_t *log;} CHILD;
static wchar_t self_path[MAX_PATH];
static FILE *events;
static int open_log(const wchar_t *path) {
 if(_wfopen_s(&events,path,L"wx")||!events)return 0;
 if(!SetHandleInformation((HANDLE)_get_osfhandle(_fileno(events)),HANDLE_FLAG_INHERIT,0)){
  (void)fclose(events);events=NULL;return 0;
 }
 return 1;
}
static int log_event(const char *name) {return fprintf(events,"%s pid=%lu tid=%lu\n",name,(unsigned long)GetCurrentProcessId(),(unsigned long)GetCurrentThreadId())>0&&fflush(events)==0;}
static unsigned __int64 created(HANDLE h) {FILETIME c,e,k,u;if(!GetProcessTimes(h,&c,&e,&k,&u))return 0;return ((unsigned __int64)c.dwHighDateTime<<32)|c.dwLowDateTime;}
static int same_child(const CHILD *c) {
 wchar_t path[MAX_PATH];DWORD n=MAX_PATH;
 return c->pi.hProcess&&c->creation&&GetProcessId(c->pi.hProcess)==c->pi.dwProcessId&&created(c->pi.hProcess)==c->creation&&QueryFullProcessImageNameW(c->pi.hProcess,0,path,&n)&&n<MAX_PATH&&!_wcsicmp(path,self_path);
}
static int close_handle(HANDLE *h,const char *name) {
 BOOL ok;if(!*h)return 1;ok=CloseHandle(*h);
 (void)fprintf(events,"CLOSE resource=%s closed=%u error=%lu\n",name,ok?1U:0U,(unsigned long)(ok?0:GetLastError()));(void)fflush(events);
 if(ok)*h=NULL;return ok?1:0;
}
static LRESULT CALLBACK window_proc(HWND h,UINT m,WPARAM w,LPARAM l) {
 if(m==WM_CLOSE){(void)log_event("WM_CLOSE_RECEIVED");}
 if(m==WM_DESTROY){(void)log_event("WM_DESTROY_RECEIVED");PostQuitMessage(0);return 0;}
 if(m==WM_TIMER){(void)log_event("TIMEOUT_NOT_STOP");DestroyWindow(h);return 0;}
 return DefWindowProcW(h,m,w,l);
}
static int helper(int argc,wchar_t **argv) {
 HANDLE ready;int result=1;PAPACC_SERVER_CONSOLE_WIN32 console=PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER;
 if(argc<4)return 64;ready=(HANDLE)(ULONG_PTR)_wcstoui64(argv[3],NULL,10);
 if(!open_log(argv[2]))return 65;
 if(!wcscmp(argv[1],L"--console")){
  ULONGLONG start=GetTickCount64();
  if(papacc_server_console_win32_install(&console)!=PAPACC_RESULT_OK)goto done;
  if(!log_event("CONSOLE_READY")||!SetEvent(ready))goto done;
  while(!papacc_server_console_win32_stop_requested()&&GetTickCount64()-start<10000U)Sleep(1);
  if(papacc_server_console_win32_stop_requested()){if(log_event("STOP_RECEIVED_EXISTING_HANDLER"))result=0;}
  else (void)log_event("TIMEOUT_NOT_STOP");
  if(papacc_server_console_win32_uninstall(&console)!=PAPACC_RESULT_OK)result=1;
 }else if(!wcscmp(argv[1],L"--window")){
  WNDCLASSW wc;HWND h;MSG message;int got;
  memset(&wc,0,sizeof(wc));wc.lpfnWndProc=window_proc;wc.hInstance=GetModuleHandleW(NULL);wc.lpszClassName=L"PapinhoPairStopDiagnostic";
  if(!RegisterClassW(&wc))goto done;
  h=CreateWindowW(wc.lpszClassName,L"Diagnostic only",WS_OVERLAPPED,0,0,100,100,NULL,NULL,wc.hInstance,NULL);
  if(!h)goto done;
  if(!SetTimer(h,1,10000,NULL)||!log_event("WINDOW_READY")||!SetEvent(ready)){DestroyWindow(h);goto done;}
  while((got=(int)GetMessageW(&message,NULL,0,0))>0){TranslateMessage(&message);DispatchMessageW(&message);}
  result=got==0?0:1;
 }else if(!wcscmp(argv[1],L"--pending")&&argc==5){
  HANDLE release=(HANDLE)(ULONG_PTR)_wcstoui64(argv[4],NULL,10);
  if(log_event("SYNTHETIC_QUERY_PENDING")&&SetEvent(ready)&&WaitForSingleObject(release,10000)==WAIT_OBJECT_0){(void)log_event("SYNTHETIC_QUERY_RELEASED");result=0;}
  else (void)log_event("PENDING_TIMEOUT_NOT_COMPLETE");
  if(!close_handle(&release,"fixture_release"))result=1;
 }
 done:
 if(!close_handle(&ready,"fixture_ready"))result=1;
 if(fclose(events))result=1;events=NULL;return result;
}
static int launch(CHILD *c,const wchar_t *mode,const wchar_t *path,HANDLE release) {
 SECURITY_ATTRIBUTES sa={sizeof(sa),NULL,TRUE};HANDLE ready;STARTUPINFOW si;wchar_t command[2048];DWORD wait;
 memset(c,0,sizeof(*c));c->log=path;ready=CreateEventW(&sa,TRUE,FALSE,NULL);if(!ready)return 0;
 memset(&si,0,sizeof(si));si.cb=sizeof(si);si.dwFlags=STARTF_USESHOWWINDOW;si.wShowWindow=SW_HIDE;
 if(swprintf_s(command,2048,L"\"%s\" %s \"%s\" %llu %llu",self_path,mode,path,(unsigned __int64)(ULONG_PTR)ready,(unsigned __int64)(ULONG_PTR)release)<0){(void)close_handle(&ready,"ready_parent");return 0;}
 /* Same dedicated console, separate child process group. No CREATE_NO_WINDOW. */
 if(!CreateProcessW(self_path,command,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&c->pi)){(void)close_handle(&ready,"ready_parent");return 0;}
 c->creation=created(c->pi.hProcess);
 wait=WaitForSingleObject(ready,5000);
 (void)fprintf(events,"CHILD pid=%lu creation=%llu ready=%lu\n",(unsigned long)c->pi.dwProcessId,c->creation,(unsigned long)wait);(void)fflush(events);
 return close_handle(&ready,"ready_parent")&&wait==WAIT_OBJECT_0&&same_child(c);
}
static int finished(CHILD *c) {DWORD code;return WaitForSingleObject(c->pi.hProcess,5000)==WAIT_OBJECT_0&&GetExitCodeProcess(c->pi.hProcess,&code)&&code==0;}
static int contains(const wchar_t *path,const char *needle) {
 FILE *f;char data[4096];size_t n;if(_wfopen_s(&f,path,L"rb")||!f)return 0;n=fread(data,1,sizeof(data)-1,f);data[n]=0;if(fclose(f))return 0;return strstr(data,needle)!=NULL;
}
typedef struct WINDOW_SEARCH {DWORD pid;HWND hwnd;unsigned count;} WINDOW_SEARCH;
static BOOL CALLBACK find_window(HWND h,LPARAM value) {
 WINDOW_SEARCH *s=(WINDOW_SEARCH*)value;wchar_t cls[80];DWORD pid;
 if(!GetWindowThreadProcessId(h,&pid)||pid!=s->pid||GetWindow(h,GW_OWNER))return TRUE;
 if(GetClassNameW(h,cls,80)&&!wcscmp(cls,L"PapinhoPairStopDiagnostic")){s->hwnd=h;++s->count;}
 return TRUE;
}
static int bound_window(const CHILD *child,HWND hwnd) {
 DWORD pid=0,tid;wchar_t cls[80];
 if(!same_child(child)||WaitForSingleObject(child->pi.hProcess,0)!=WAIT_TIMEOUT)return 0;
 tid=GetWindowThreadProcessId(hwnd,&pid);
 return tid==child->pi.dwThreadId&&pid==child->pi.dwProcessId&&!GetWindow(hwnd,GW_OWNER)&&
  GetClassNameW(hwnd,cls,80)&&!wcscmp(cls,L"PapinhoPairStopDiagnostic");
}
static int controller(const wchar_t *dir) {
 wchar_t a[MAX_PATH],b[MAX_PATH],w[MAX_PATH],q[MAX_PATH],report[MAX_PATH];CHILD target,sentinel,window,pending;
 SECURITY_ATTRIBUTES sa={sizeof(sa),NULL,TRUE};HANDLE release=NULL;PAPACC_SERVER_CONSOLE_WIN32 own=PAPACC_SERVER_CONSOLE_WIN32_INITIALIZER;
 WINDOW_SEARCH search;DWORD pid,tid;int ok=0,closed=1;
 memset(&target,0,sizeof(target));memset(&sentinel,0,sizeof(sentinel));memset(&window,0,sizeof(window));memset(&pending,0,sizeof(pending));
 if(swprintf_s(a,MAX_PATH,L"%s/target.log",dir)<0||swprintf_s(b,MAX_PATH,L"%s/sentinel.log",dir)<0||swprintf_s(w,MAX_PATH,L"%s/window.log",dir)<0||swprintf_s(q,MAX_PATH,L"%s/pending.log",dir)<0||swprintf_s(report,MAX_PATH,L"%s/controller.log",dir)<0)return 64;
 if(!open_log(report))return 65;
 if(papacc_server_console_win32_install(&own)!=PAPACC_RESULT_OK)goto done;
 release=CreateEventW(&sa,TRUE,FALSE,NULL);if(!release)goto done;
 if(!launch(&pending,L"--pending",q,release)||!SetHandleInformation(release,HANDLE_FLAG_INHERIT,0))goto done;
 if(!launch(&target,L"--console",a,NULL)||!launch(&sentinel,L"--console",b,NULL))goto done;
 if(!same_child(&target)||!same_child(&sentinel)||target.pi.dwProcessId==sentinel.pi.dwProcessId)goto done;
 {CHILD stale=target;stale.creation++;if(same_child(&stale))goto done;
  stale=target;stale.pi.dwProcessId=sentinel.pi.dwProcessId;if(same_child(&stale))goto done;}
 (void)log_event("STALE_CREATION_AND_WRONG_PID_DENIED_WITHOUT_SIGNAL");
 if(!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,target.pi.dwProcessId)||!finished(&target)||!contains(a,"STOP_RECEIVED_EXISTING_HANDLER"))goto done;
 if(WaitForSingleObject(sentinel.pi.hProcess,0)!=WAIT_TIMEOUT||papacc_server_console_win32_stop_requested()||WaitForSingleObject(pending.pi.hProcess,0)!=WAIT_TIMEOUT)goto done;
 (void)log_event("TARGET_BREAK_RECEIVED_SENTINEL_CONTROLLER_PENDING_UNAFFECTED");
 if(!launch(&window,L"--window",w,NULL))goto done;
 memset(&search,0,sizeof(search));search.pid=window.pi.dwProcessId;
 if(!EnumWindows(find_window,(LPARAM)&search)||search.count!=1||!same_child(&window))goto done;
 tid=GetWindowThreadProcessId(search.hwnd,&pid);
 if(!tid||pid!=window.pi.dwProcessId||!bound_window(&window,search.hwnd)||bound_window(&target,search.hwnd))goto done;
 {CHILD stale=window;stale.creation++;if(bound_window(&stale,search.hwnd))goto done;}
 (void)log_event("HWND_WRONG_PROCESS_AND_STALE_LIFETIME_DENIED_WITHOUT_MESSAGE");
 (void)fprintf(events,"HWND_BOUND hwnd=%llu pid=%lu tid=%lu creation=%llu class=PapinhoPairStopDiagnostic\n",(unsigned __int64)(ULONG_PTR)search.hwnd,(unsigned long)pid,(unsigned long)tid,window.creation);(void)fflush(events);
 if(!PostMessageW(search.hwnd,WM_CLOSE,0,0)||!finished(&window)||!contains(w,"WM_CLOSE_RECEIVED")||!contains(w,"WM_DESTROY_RECEIVED"))goto done;
 if(WaitForSingleObject(pending.pi.hProcess,0)!=WAIT_TIMEOUT||papacc_server_console_win32_stop_requested())goto done;
 (void)log_event("FUNCTIONAL_WINDOW_CLOSE_WHILE_SYNTHETIC_OBSERVER_PENDING");
 if(!same_child(&sentinel)||!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,sentinel.pi.dwProcessId)||!finished(&sentinel)||!contains(b,"STOP_RECEIVED_EXISTING_HANDLER"))goto done;
 if(!SetEvent(release)||!finished(&pending))goto done;
 ok=1;
 done:
 /* Never retry delivery or kill helpers to turn a failed test into PASS.
  * On failure, unresolved helpers are PENDING and have their own finite timeout. */
 if(!ok)(void)log_event("FAIL_PENDING_NO_TERMINATION_NO_RETRY");
 closed=close_handle(&target.pi.hThread,"target_thread")&&closed;closed=close_handle(&target.pi.hProcess,"target_process")&&closed;
 closed=close_handle(&sentinel.pi.hThread,"sentinel_thread")&&closed;closed=close_handle(&sentinel.pi.hProcess,"sentinel_process")&&closed;
 closed=close_handle(&window.pi.hThread,"window_thread")&&closed;closed=close_handle(&window.pi.hProcess,"window_process")&&closed;
 closed=close_handle(&pending.pi.hThread,"pending_thread")&&closed;closed=close_handle(&pending.pi.hProcess,"pending_process")&&closed;
 closed=close_handle(&release,"release_parent")&&closed;
 if(own.is_installed&&papacc_server_console_win32_uninstall(&own)!=PAPACC_RESULT_OK)closed=0;
 (void)log_event(ok&&closed?"PASS_DIAGNOSTIC_ONLY":"INCOMPLETE_OR_RETAINED_NOT_PASS");
 if(fclose(events))closed=0;events=NULL;return ok&&closed?0:1;
}
int wmain(int argc,wchar_t **argv) {
 DWORD n=GetModuleFileNameW(NULL,self_path,MAX_PATH);if(!n||n>=MAX_PATH)return 64;
 if(argc==3&&!wcscmp(argv[1],L"--bootstrap")){
  STARTUPINFOW si;PROCESS_INFORMATION pi;wchar_t command[2048];DWORD wait,code;BOOL pclose,tclose;
  memset(&si,0,sizeof(si));memset(&pi,0,sizeof(pi));si.cb=sizeof(si);si.dwFlags=STARTF_USESHOWWINDOW;si.wShowWindow=SW_HIDE;
  if(swprintf_s(command,2048,L"\"%s\" --controller \"%s\"",self_path,argv[2])<0)return 64;
  if(!CreateProcessW(self_path,command,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))return 65;
  wait=WaitForSingleObject(pi.hProcess,20000);code=1;
  if(wait!=WAIT_OBJECT_0||!GetExitCodeProcess(pi.hProcess,&code))code=1;
  tclose=CloseHandle(pi.hThread);pclose=CloseHandle(pi.hProcess);
  (void)printf("BOOTSTRAP wait=%lu code=%lu thread_closed=%u process_closed=%u\n",(unsigned long)wait,(unsigned long)code,tclose?1U:0U,pclose?1U:0U);
  return code==0&&tclose&&pclose?0:1;
 }
 if(argc==3&&!wcscmp(argv[1],L"--controller"))return controller(argv[2]);
 return helper(argc,argv);
}