#define wmain observation_entry
#include "ConsoleRoles.c"
#undef wmain
static unsigned checks;static DWORD controller_peak;static OBS retained_fault_model;
static int controller_count(int reserve){DWORD n=0;if(!GetProcessHandleCount(GetCurrentProcess(),&n))return 0;if(n>controller_peak)controller_peak=n;printf("CONTROLLER_HANDLES=%lu reserve=%d\n",n,reserve);return n<=(reserve?124U:128U);}
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL line=%u %s error=%lu\n",(unsigned)__LINE__,#x,GetLastError());return 1;}}while(0)
static int spawn(const wchar_t *exe,const wchar_t *file,int pseudo,PROCESS_INFORMATION *pi,HPCON *pc,HANDLE pipes[4]){
 STARTUPINFOEXW s;wchar_t cmd[2048];SIZE_T bytes=0;COORD size={100,40};HRESULT result;memset(&s,0,sizeof(s));memset(pi,0,sizeof(*pi));s.StartupInfo.cb=pseudo?sizeof(s):sizeof(s.StartupInfo);
 if(!controller_count(1))return 0;
 if(swprintf_s(cmd,2048,L"\"%s\" --fixture-output \"%s\"",exe,file)<0)return 0;
 if(pseudo){if(!CreatePipe(&pipes[0],&pipes[1],NULL,0)||!CreatePipe(&pipes[2],&pipes[3],NULL,0))return 0;
 result=CreatePseudoConsole(size,pipes[0],pipes[3],0,pc);if(FAILED(result)){printf("PSEUDO_CREATE=%08lx\n",(unsigned long)result);return 0;}
 InitializeProcThreadAttributeList(NULL,1,0,&bytes);s.lpAttributeList=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,bytes);if(!s.lpAttributeList)return 0;
 if(!InitializeProcThreadAttributeList(s.lpAttributeList,1,0,&bytes)||!UpdateProcThreadAttribute(s.lpAttributeList,0,PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,*pc,sizeof(*pc),NULL,NULL))return 0;}
 if(!CreateProcessW(exe,cmd,NULL,NULL,FALSE,pseudo?EXTENDED_STARTUPINFO_PRESENT:CREATE_NEW_CONSOLE,NULL,NULL,&s.StartupInfo,pi))return 0;
 if(s.lpAttributeList){DeleteProcThreadAttributeList(s.lpAttributeList);if(!HeapFree(GetProcessHeap(),0,s.lpAttributeList))return 0;}return 1;
}
static DWORD WINAPI drain(LPVOID value){HANDLE h=value;unsigned char b[1024];DWORD n;while(ReadFile(h,b,sizeof(b),&n,NULL)&&n){}return GetLastError()==ERROR_BROKEN_PIPE?0:1;}
static int wait_close(PROCESS_INFORMATION *pi,HPCON pc,HANDLE pipes[4]){DWORD result,code=0,i;HANDLE reader=NULL;
 if(pc){reader=CreateThread(NULL,0,drain,pipes[2],0,NULL);if(!reader)return 0;}
 if(!controller_count(0))return 0;result=WaitForSingleObject(pi->hProcess,15000);if(result!=WAIT_OBJECT_0){puts("CHILD_NOT_FINISHED_UNKNOWN_NO_KILL");return 0;}
 if(!GetExitCodeProcess(pi->hProcess,&code)||code)return 0;{NODE ended;OBS state;memset(&ended,0,sizeof(ended));init(&state);ended.process=pi->hProcess;metrics(&state,&ended);if(ended.stable||ended.exit_code!=0)return 0;puts("ACTUAL_ENDED_PROCESS_NOT_LIVE_PASS");}if(!CloseHandle(pi->hThread)||!CloseHandle(pi->hProcess))return 0;
 if(pc){ClosePseudoConsole(pc);puts("CLOSE_PSEUDOCONSOLE_RETURNED_INTERNAL_CLOSE_NOT_OBSERVED");for(i=0;i<4;i++)if(i!=2&&pipes[i]){if(!CloseHandle(pipes[i]))return 0;pipes[i]=NULL;}
 if(WaitForSingleObject(reader,5000)!=WAIT_OBJECT_0){puts("DRAIN_NOT_FINISHED_UNKNOWN_NO_KILL");return 0;}if(!GetExitCodeThread(reader,&code)||code||!CloseHandle(reader)||!CloseHandle(pipes[2]))return 0;}
 return 1;}
static int models(void){OBS o;HANDLE event;FILETIME c,e,k,u;NODE *n;DWORD raw_before;unsigned i;DWORD candidate=0;NODE raw;
 memset(&raw,0,sizeof(raw));raw.host_bytes=8;raw.host_raw=20145;CHECK(host_candidate(&raw,&candidate)&&candidate==20144);raw.host_raw=17206;CHECK(!host_candidate(&raw,&candidate));raw.host_raw=0;CHECK(!host_candidate(&raw,&candidate));raw.host_raw=20145;raw.host_bytes=4;CHECK(!host_candidate(&raw,&candidate));raw.host_bytes=8;raw.host_status=(LONG)0xc0000003;CHECK(!host_candidate(&raw,&candidate));raw.host_status=0;raw.host_raw=0x100000001ULL;CHECK(!host_candidate(&raw,&candidate));
 init(&o);CHECK(bind_role(&o,GetCurrentProcessId(),R_SEED)>=0);raw_before=o.n;CHECK(bind_role(&o,GetCurrentProcessId(),R_WINDOW)==0&&o.n==raw_before);n=&o.nodes[0];CHECK(n->roles==(R_SEED|R_WINDOW));CHECK(GetProcessTimes(n->process,&c,&e,&k,&u));n->creation++;CHECK(bind_role(&o,GetCurrentProcessId(),R_CLIENT)<0&&!strcmp(o.first,"ROLE_IDENTITY_CHANGED"));CHECK(o.n==raw_before);n->creation=filetime(c);for(i=0;i<o.n;i++)CHECK(close_ref(&o,&o.nodes[i].process));if(o.sha)CHECK(BCryptCloseAlgorithmProvider(o.sha,0)==0);
 init(&o);o.reserved=1;CHECK(!reserve_temp(&o)&&!strcmp(o.first,"TEMP_BUSY"));CHECK(!sample(&o,0));
 init(&retained_fault_model);event=CreateEventW(NULL,TRUE,FALSE,NULL);CHECK(event!=NULL);retained_fault_model.temporary=event;retained_fault_model.test_close_failure=1;CHECK(!close_ref(&retained_fault_model,&retained_fault_model.temporary)&&retained_fault_model.temporary==event&&retained_fault_model.retained&&retained_fault_model.close_calls==1&&!strcmp(retained_fault_model.first,"CLOSE_UNKNOWN"));CHECK(!close_ref(&retained_fault_model,&retained_fault_model.temporary)&&retained_fault_model.close_calls==1);CHECK(!reserve_temp(&retained_fault_model));CHECK(retained_fault_model.temporary==event&&!strcmp(retained_fault_model.first,"CLOSE_UNKNOWN"));puts("MODEL_UNKNOWN_REFERENCE_RETAINED_UNTIL_FIXTURE_PROCESS_LOSS_NOT_CONFIRMED_CLOSE");
 init(&o);o.n=CAP;CHECK(bind_role(&o,GetCurrentProcessId(),R_SEED)<0&&!strcmp(o.first,"NODE_CAP")&&o.n==CAP);CHECK(!reserve_temp(&o)&&!o.temporary);
 init(&o);InterlockedExchange(&stopped,1);CHECK(bind_role(&o,GetCurrentProcessId(),R_SEED)<0&&!strcmp(o.first,"STOP")&&o.n==0);InterlockedExchange(&stopped,0);
 printf("MODEL_CHECKS=%u identity/dedup/generation/borrow/Stop/UNKNOWN noTPM\n",checks);return 0;}
int wmain(int argc,wchar_t **argv){wchar_t exe[MAX_PATH],file[MAX_PATH];PROCESS_INFORMATION pi[2];HPCON pc=NULL;HANDLE pipes[4]={NULL,NULL,NULL,NULL};DWORD code;unsigned i;
 if(argc==3&&!wcscmp(argv[1],L"--fixture-output"))return observation_entry(argc,argv);
 if(argc!=2)return 2;CHECK(GetModuleFileNameW(NULL,exe,MAX_PATH));CHECK(models()==0);
 CHECK(swprintf_s(file,MAX_PATH,L"%s\\classic.json",argv[1])>0);CHECK(spawn(exe,file,0,&pi[0],&pc,pipes));CHECK(wait_close(&pi[0],pc,pipes));puts("CLASSIC_REAL_API_FINITE_CHILD_PASS");
 CHECK(swprintf_s(file,MAX_PATH,L"%s\\pseudo.json",argv[1])>0);CHECK(spawn(exe,file,1,&pi[0],&pc,pipes));CHECK(wait_close(&pi[0],pc,pipes));puts("PSEUDOCONSOLE_REAL_API_FINITE_CHILD_PASS");
 for(i=0;i<2;i++){HPCON no_pc=NULL;HANDLE no_pipes[4]={0};CHECK(swprintf_s(file,MAX_PATH,L"%s\\concurrent%u.json",argv[1],i)>0);CHECK(spawn(exe,file,0,&pi[i],&no_pc,no_pipes));}
 for(i=0;i<2;i++){HANDLE no_pipes[4]={0};CHECK(wait_close(&pi[i],NULL,no_pipes));}puts("CONCURRENT_DISTINCT_CONSOLE_OBSERVERS_PASS");
 {HANDLE dead=CreateEventW(NULL,TRUE,FALSE,NULL);CHECK(dead!=NULL);CHECK(CloseHandle(dead));}
 code=GetCurrentProcessId();printf("PASS total_checks=%u fixture_pid=%lu controller_peak=%lu no_TPM_no_authority\n",checks,code,controller_peak);return 0;}
