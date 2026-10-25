/* Isolated adversarial/console integration boundary test. NO LAB/SCM. */
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <processsnapshot.h>
#include <stdio.h>
#include <string.h>
static void test_window(void *);
#define NX_BEFORE_CREATE(c) test_window(c)
#include "../tools/qualification/scm-explicit-inheritance-fixture.c"
static WCHAR fixture[MAX_PATH];static DWORD first,matched,window_calls;static int inherit_mode;
__declspec(noreturn) static void uncertain(DWORD error){if(!first)first=error;printf("BOUNDARY_UNKNOWN first=%lu; ORIGINAL_REFERENCES_RETAINED_NO_RETRY_NO_KILL\n",(unsigned long)first);fflush(stdout);for(;;)Sleep(1000);}
static int close_checked(HANDLE h){if(h&&!CloseHandle(h)){uncertain(GetLastError());}return 1;}
static DWORD WINAPI competing_creator(void *arg)
{
 NX_CONTEXT *ctx=(NX_CONTEXT *)arg;STARTUPINFOW si;PROCESS_INFORMATION pi;WCHAR cmd[1024];
 HPSS snap=NULL;HPSSWALK marker=NULL;PSS_HANDLE_ENTRY item;HANDLE ref=NULL;DWORD e=0,z,i,n=0;HANDLE targets[3];
 typedef BOOL(WINAPI *CMP)(HANDLE,HANDLE);CMP compare;FARPROC a;FILETIME birth,x,k,u;
 targets[0]=ctx->write_pipe;targets[1]=ctx->input;targets[2]=ctx->error;
 ZeroMemory(&si,sizeof(si));si.cb=sizeof(si);ZeroMemory(&pi,sizeof(pi));
 _snwprintf_s(cmd,1024,_TRUNCATE,L"\"%ls\" --isolated-observer-self delay-child",fixture);
 if(!CreateProcessW(fixture,cmd,NULL,NULL,inherit_mode?TRUE:FALSE,CREATE_SUSPENDED,NULL,NULL,&si,&pi)){first=GetLastError();return first;}
 if(!GetProcessTimes(pi.hProcess,&birth,&x,&k,&u)){e=GetLastError();goto done;}
 printf("COMPETITOR pid=%lu creation=%lu:%lu inherits=%d; ORIGINAL_PROCESS_HANDLE\n",(unsigned long)pi.dwProcessId,(unsigned long)birth.dwHighDateTime,(unsigned long)birth.dwLowDateTime,inherit_mode);
 e=PssCaptureSnapshot(pi.hProcess,(PSS_CAPTURE_FLAGS)(PSS_CAPTURE_HANDLES|PSS_CAPTURE_HANDLE_BASIC_INFORMATION|PSS_CAPTURE_HANDLE_NAME_INFORMATION|PSS_CAPTURE_HANDLE_TYPE_SPECIFIC_INFORMATION),0,&snap);if(e)goto done;
 e=PssWalkMarkerCreate(NULL,&marker);if(e)goto done;
 a=GetProcAddress(GetModuleHandleW(L"kernelbase.dll"),"CompareObjectHandles");if(!a){e=ERROR_NOT_SUPPORTED;goto done;}memcpy(&compare,&a,sizeof(compare));
 for(;;){ZeroMemory(&item,sizeof(item));e=PssWalkSnapshot(snap,PSS_WALK_HANDLES,marker,&item,sizeof(item));if(e==ERROR_NO_MORE_ITEMS){e=0;break;}if(e)break;
  if(!DuplicateHandle(pi.hProcess,item.Handle,GetCurrentProcess(),&ref,0,FALSE,DUPLICATE_SAME_ACCESS)){e=GetLastError();break;}
  for(i=0;i<3;i++){SetLastError(0);if(compare(ref,targets[i])){++matched;printf("PRIVATE_TRANSFER_EXPOSED slot=%lu competitor_value=%llu SAME_OBJECT\n",(unsigned long)i,(ULONGLONG)(ULONG_PTR)item.Handle);}else{z=GetLastError();if(z&&z!=ERROR_NOT_SAME_OBJECT){e=z;break;}}}
  if(!close_checked(ref)){e=first;ref=NULL;break;}ref=NULL;if(e)break;if(++n>4096){e=ERROR_INSUFFICIENT_BUFFER;break;}
 }
 done:
 if(marker){z=PssWalkMarkerFree(marker);if(z)uncertain(z);}
 if(snap){z=PssFreeSnapshot(GetCurrentProcess(),snap);if(z)uncertain(z);}
 /* Suspended only for deterministic observation: resume once and natural exit, never kill. */
 if(ResumeThread(pi.hThread)==(DWORD)-1)uncertain(GetLastError());
 if(WaitForSingleObject(pi.hProcess,30000)!=WAIT_OBJECT_0)uncertain(ERROR_TIMEOUT);
 close_checked(pi.hThread);close_checked(pi.hProcess);if(e&&!first)first=e;return first;
}
static void test_window(void *ctx)
{
 HANDLE thread;DWORD id;if(inherit_mode<0)return;++window_calls;
 thread=CreateThread(NULL,0,competing_creator,ctx,0,&id);if(!thread){first=GetLastError();return;}
 if(WaitForSingleObject(thread,30000)!=WAIT_OBJECT_0)uncertain(ERROR_TIMEOUT);close_checked(thread);
}
static void sample(const char *phase){DWORD handles=0;if(!GetProcessHandleCount(GetCurrentProcess(),&handles)){if(!first)first=GetLastError();return;}printf("BOUNDARY_SAMPLE phase=%s handles=%lu tick=%llu; OWN_PROCESS_ONLY_INTERNAL_COST_UNKNOWN\n",phase,(unsigned long)handles,GetTickCount64());}
static int concurrency_test(void)
{
 NX_CONTEXT ctx;HANDLE in,err;DWORD rc,code;char buf[4096];BOOL ok;
 sample("BEFORE_SYNTHETIC_ACQUISITIONS");ZeroMemory(&ctx,sizeof(ctx));in=CreateEventW(NULL,TRUE,FALSE,NULL);err=CreateEventW(NULL,TRUE,FALSE,NULL);if(!in||!err)return 71;
 if(NX_Init(&ctx,sizeof(ctx),0))return 72;
 rc=NX_Start(&ctx,fixture,L"--isolated-observer-self delay-child",in,err);if(rc)return 73;
 /* Drain fixture-only small output through owner reference. */
 do{ok=ReadFile(ctx.read_pipe,buf,sizeof(buf),&code,NULL);}while(ok&&code);
 if(!ok&&GetLastError()!=ERROR_BROKEN_PIPE)return 74;
 if(NX_Wait(&ctx,30000)||NX_Close(&ctx))return 75;
 close_checked(in);close_checked(err);sample("AFTER_CONCURRENCY_CLEANUP");
 printf("CONCURRENCY_RESULT first=%lu matched=%lu window_calls=%lu owner_refs=%u expected=%u; NO_GLOBAL_EXCLUSION_PROOF\n",(unsigned long)first,(unsigned long)matched,(unsigned long)window_calls,nx_owned(&ctx),inherit_mode?3U:0U);
 return first||matched!=(inherit_mode?3U:0U)||window_calls!=1?76:0;
}
static int inject_text(HANDLE input,const WCHAR *text)
{
 INPUT_RECORD record;DWORD n;ZeroMemory(&record,sizeof(record));record.EventType=KEY_EVENT;record.Event.KeyEvent.bKeyDown=TRUE;record.Event.KeyEvent.wRepeatCount=1;
 while(*text){record.Event.KeyEvent.uChar.UnicodeChar=*text;record.Event.KeyEvent.wVirtualKeyCode=(*text==L'\r')?VK_RETURN:0;if(!WriteConsoleInputW(input,&record,1,&n)||n!=1)return 0;++text;}return 1;
}
static int console_test(void)
{
 NX_CONTEXT ctx;HANDLE input,error;DWORD mode,rc,n,waited=0;char buf[4096];int ready=0;INPUT_RECORD key;HWND window=GetConsoleWindow();
 if(!window)return 81;printf("PARENT_CONSOLE_WINDOW value=%llu\n",(ULONGLONG)(ULONG_PTR)window);sample("CONSOLE_BEFORE_ACQUISITIONS");
 input=CreateFileW(L"CONIN$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
 error=CreateFileW(L"CONOUT$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
 if(input==INVALID_HANDLE_VALUE||error==INVALID_HANDLE_VALUE)return 82;
 if(!GetConsoleMode(input,&mode)||!FlushConsoleInputBuffer(input))return 83;
 ZeroMemory(&ctx,sizeof(ctx));if(NX_Init(&ctx,sizeof(ctx),0))return 84;
 rc=NX_Start(&ctx,fixture,L"--isolated-observer-self console-child",input,error);if(rc)return 85;
 while(waited<10000){if(!PeekNamedPipe(ctx.read_pipe,NULL,0,NULL,&n,NULL))return 86;if(n){if(!ReadFile(ctx.read_pipe,buf,sizeof(buf)-1,&n,NULL))return 87;buf[n]=0;printf("CHILD:%s",buf);if(strstr(buf,"CONSOLE_CONFIRM_READY")){ready=1;break;}}Sleep(10);waited+=10;}
 if(!ready||!inject_text(input,L"CONFIRM_CONSOLE\r"))return 88;
 ZeroMemory(&key,sizeof(key));key.EventType=KEY_EVENT;key.Event.KeyEvent.bKeyDown=TRUE;key.Event.KeyEvent.wRepeatCount=1;key.Event.KeyEvent.wVirtualKeyCode=VK_ESCAPE;
 if(!WriteConsoleInputW(input,&key,1,&n)||n!=1)return 89;
 do{rc=ReadFile(ctx.read_pipe,buf,sizeof(buf)-1,&n,NULL);if(rc&&n){buf[n]=0;printf("CHILD:%s",buf);}}while(rc&&n);
 if(!rc&&GetLastError()!=ERROR_BROKEN_PIPE)return 90;
 if(NX_Wait(&ctx,30000)||ctx.exit_code||NX_Close(&ctx))return 91;
 if(!FlushConsoleInputBuffer(input))return 92;
 key.Event.KeyEvent.wVirtualKeyCode=VK_F10;if(!WriteConsoleInputW(input,&key,1,&n)||!ReadConsoleInputW(input,&key,1,&n)||key.Event.KeyEvent.wVirtualKeyCode!=VK_F10)return 93;
 close_checked(input);close_checked(error);sample("CONSOLE_AFTER_CLEANUP");
 printf("CONSOLE_RESULT first=%lu original_window_same=%u child_exit=0 owner_refs=%u; SYNTHETIC_KEY_EVENTS_REAL_CONSOLE_NOT_HUMAN_OR_LAB\n",(unsigned long)first,GetConsoleWindow()==window,nx_owned(&ctx));return first?94:0;
}
int wmain(int argc,WCHAR **argv)
{
 unsigned iteration;int code;
 if(argc!=3)return 64;if(wcslen(argv[1])>=MAX_PATH)return 65;wcscpy_s(fixture,MAX_PATH,argv[1]);
 inherit_mode=!wcscmp(argv[2],L"global")?1:(!wcscmp(argv[2],L"none")?0:-1);
 for(iteration=0;iteration<3;iteration++){first=0;matched=0;window_calls=0;printf("BOUNDARY_ITERATION index=%u\n",iteration);code=inherit_mode<0?console_test():concurrency_test();if(code)return code;}return 0;
}
