/* Native C0 role graph; test-only authority adapter, no TBS compiled. */
#define TP_E2E_ENTRY prior_console_mock_main
#include "host_e2e_fixture.c"
#undef TP_E2E_ENTRY
#ifndef PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE
#define PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE 0x00020016
#endif
typedef void *FIXTURE_PTY;
typedef HRESULT (WINAPI *CREATE_PTY)(COORD,HANDLE,HANDLE,DWORD,FIXTURE_PTY *);
typedef void (WINAPI *CLOSE_PTY)(FIXTURE_PTY);
static CREATE_PTY create_pty;static CLOSE_PTY close_pty;static DWORD controller_peak;
static int controller_count(int reserve){DWORD n=0;if(!GetProcessHandleCount(GetCurrentProcess(),&n))return 0;if(n>controller_peak)controller_peak=n;printf("NATIVE_C0_CONTROLLER_HANDLES=%lu\n",n);return n<=(reserve?124U:128U);}
static int spawn(const wchar_t *exe,const wchar_t *file,int pseudo,PROCESS_INFORMATION *pi,FIXTURE_PTY *pc,HANDLE pipes[4]){
 STARTUPINFOEXW s;wchar_t cmd[2048];SIZE_T bytes=0;COORD size={100,40};HRESULT result;memset(&s,0,sizeof(s));memset(pi,0,sizeof(*pi));s.StartupInfo.cb=pseudo?sizeof(s):sizeof(s.StartupInfo);
 if(!controller_count(1))return 0;
 if(swprintf_s(cmd,2048,L"\"%s\" --c0-parent \"%s\"",exe,file)<0)return 0;
 if(pseudo){if(!CreatePipe(&pipes[0],&pipes[1],NULL,0)||!CreatePipe(&pipes[2],&pipes[3],NULL,0))return 0;
 result=create_pty(size,pipes[0],pipes[3],0,pc);if(FAILED(result)){printf("PSEUDO_CREATE=%08lx\n",(unsigned long)result);return 0;}
 InitializeProcThreadAttributeList(NULL,1,0,&bytes);s.lpAttributeList=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,bytes);if(!s.lpAttributeList)return 0;
 if(!InitializeProcThreadAttributeList(s.lpAttributeList,1,0,&bytes)||!UpdateProcThreadAttribute(s.lpAttributeList,0,PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,*pc,sizeof(*pc),NULL,NULL))return 0;}
 if(!CreateProcessW(exe,cmd,NULL,NULL,FALSE,pseudo?EXTENDED_STARTUPINFO_PRESENT:CREATE_NEW_CONSOLE,NULL,NULL,&s.StartupInfo,pi))return 0;
 if(s.lpAttributeList){DeleteProcThreadAttributeList(s.lpAttributeList);if(!HeapFree(GetProcessHeap(),0,s.lpAttributeList))return 0;}return 1;
}
static DWORD WINAPI drain(LPVOID value){HANDLE h=value;unsigned char b[1024];DWORD n;while(ReadFile(h,b,sizeof(b),&n,NULL)&&n){}return GetLastError()==ERROR_BROKEN_PIPE?0:1;}
static int wait_close(PROCESS_INFORMATION *pi,FIXTURE_PTY pc,HANDLE pipes[4]){DWORD result,code=0,i;HANDLE reader=NULL;
 if(pc){reader=CreateThread(NULL,0,drain,pipes[2],0,NULL);if(!reader)return 0;}
 if(!controller_count(0))return 0;result=WaitForSingleObject(pi->hProcess,15000);if(result!=WAIT_OBJECT_0){puts("CHILD_NOT_FINISHED_UNKNOWN_NO_KILL");return 0;}
 if(!GetExitCodeProcess(pi->hProcess,&code)||code)return 0;{TP_PLATFORM ended;FILETIME creation;hp_init(&ended);if(hp_alive(&ended,pi->hProcess,pi->dwProcessId,&creation)||strcmp(ended.proof.first,"ROLE_PROCESS_NOT_LIVE"))return 0;puts("PRODUCTION_ALIVE_REJECTS_ENDED_PROCESS");}if(!CloseHandle(pi->hThread)||!CloseHandle(pi->hProcess))return 0;
 if(pc){close_pty(pc);puts("CLOSE_PSEUDOCONSOLE_RETURNED_INTERNAL_CLOSE_NOT_OBSERVED");for(i=0;i<4;i++)if(i!=2&&pipes[i]){if(!CloseHandle(pipes[i]))return 0;pipes[i]=NULL;}
 if(WaitForSingleObject(reader,5000)!=WAIT_OBJECT_0){puts("DRAIN_NOT_FINISHED_UNKNOWN_NO_KILL");return 0;}if(!GetExitCodeThread(reader,&code)||code||!CloseHandle(reader)||!CloseHandle(pipes[2]))return 0;}
 return 1;}

static int admin_child(const wchar_t *file){TP_PLATFORM *p=&runtime.platform;FILE *out=NULL;int ok;FILETIME c,e,k,u;
 if(_wfopen_s(&out,file,L"wx"))return 2;if(fclose(out))return 2;if(_wfreopen_s(&out,file,L"w",stdout)||!out)return 2;
 wcscpy_s(fixture_folder,MAX_PATH,L"ISOLATED_C0_ROLE_AUTHORITY_MODEL");fixture_init(p);
 if(!GetProcessTimes(GetCurrentProcess(),&c,&e,&k,&u))return 2;p->pid=GetCurrentProcessId();p->creation=ft(&c);strcpy_s(p->live_binding,sizeof(p->live_binding),"NATIVE_C0_FIXTURE_NOT_OPERATION");
 puts("TEST_ONLY_ROLE_TOKEN_AUTHORITY_AND_PARENT_FILE_MODEL; REAL_ORIGINAL_PROCESS_CONSOLE_ABI_AUXFILE_METRICS");
 ok=tp_crypto_init(&p->proof)&&fixture_token(p)&&hp_admin(p)&&hp_admin(p);tp_finish(&p->proof);if(!p->proof.retained)hp_close(p);hp_print(p,"NATIVE_C0_ROLE_FIXTURE_NO_TPM");
 if(fclose(out))return 3;return ok&&!p->proof.retained&&!strcmp(p->proof.first,"NONE")?0:2;
}
static int operator_parent(const wchar_t *file){wchar_t exe[MAX_PATH],cmd[2048];STARTUPINFOW si;PROCESS_INFORMATION pi;DWORD code;
 if(!GetModuleFileNameW(NULL,exe,MAX_PATH)||swprintf_s(cmd,2048,L"\"%s\" --c0-child \"%s\"",exe,file)<0)return 2;memset(&si,0,sizeof(si));si.cb=sizeof(si);
 if(!CreateProcessW(exe,cmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))return 2;
 if(WaitForSingleObject(pi.hProcess,15000)!=WAIT_OBJECT_0)return 3;
 if(!GetExitCodeProcess(pi.hProcess,&code)||!CloseHandle(pi.hThread)||!CloseHandle(pi.hProcess))return 3;return (int)code;
}
int wmain(int argc,wchar_t **argv){wchar_t exe[MAX_PATH],file[MAX_PATH];PROCESS_INFORMATION pi[2];FIXTURE_PTY pc=NULL;HANDLE pipes[4]={0};unsigned i;
 if(argc==3&&!wcscmp(argv[1],L"--c0-child"))return admin_child(argv[2]);if(argc==3&&!wcscmp(argv[1],L"--c0-parent"))return operator_parent(argv[2]);if(argc!=2)return 2;
 create_pty=(CREATE_PTY)(void *)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),"CreatePseudoConsole");close_pty=(CLOSE_PTY)(void *)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),"ClosePseudoConsole");
 if(!create_pty||!close_pty||!GetModuleFileNameW(NULL,exe,MAX_PATH))return 2;
 if(swprintf_s(file,MAX_PATH,L"%s\\classic.json",argv[1])<0||!spawn(exe,file,0,&pi[0],&pc,pipes)||!wait_close(&pi[0],pc,pipes))return 1;puts("PRODUCTION_C0_CLASSIC_NATIVE_GRAPH_PASS");
 if(swprintf_s(file,MAX_PATH,L"%s\\pseudo.json",argv[1])<0||!spawn(exe,file,1,&pi[0],&pc,pipes)||!wait_close(&pi[0],pc,pipes))return 1;puts("PRODUCTION_C0_PSEUDO_NATIVE_GRAPH_PASS");
 for(i=0;i<2;i++){FIXTURE_PTY none=NULL;HANDLE no_pipes[4]={0};if(swprintf_s(file,MAX_PATH,L"%s\\concurrent%u.json",argv[1],i)<0||!spawn(exe,file,0,&pi[i],&none,no_pipes))return 1;}
 for(i=0;i<2;i++){HANDLE no_pipes[4]={0};if(!wait_close(&pi[i],NULL,no_pipes))return 1;}printf("PASS_C0_NATIVE_CLASSIC_PSEUDO_CONCURRENT_LIFETIME controller_peak=%lu NO_TPM\n",controller_peak);return 0;
}
