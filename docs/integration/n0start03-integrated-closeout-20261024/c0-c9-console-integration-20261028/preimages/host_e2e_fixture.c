/* Full production entry control flow; compile-time authority/endpoint substitutions.
 * No actual TBS implementation is compiled here. This binary cannot prove TPM possession. */
#include "host_platform.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static wchar_t fixture_folder[MAX_PATH];
static int fixture_token(TP_PLATFORM *),fixture_files(TP_PLATFORM *,const char *,const char *),fixture_admin(TP_PLATFORM *),fixture_certificate(TP_PLATFORM *),fixture_boundary(TP_OWNER *,const char *);
static void fixture_init(TP_PLATFORM *);
#define TP_HOST_FIXTURE 1
#define hp_init fixture_init
#define hp_token fixture_token
#define hp_files fixture_files
#define hp_admin fixture_admin
#define hp_load_certificate fixture_certificate
#define wmain fixture_production_entry
#include "native_host.c"
#undef wmain
#undef hp_init
#undef hp_token
#undef hp_files
#undef hp_admin
#undef hp_load_certificate
/* Reuse exact previously qualified mock protocol responder; no duplicate crypto. */
#define main reusable_mock_tests_not_run
#include "proof_test.c"
#undef main
static MOCK endpoint;
static void fixture_init(TP_PLATFORM *p){hp_init(p);p->proof.boundary=fixture_boundary;memset(&endpoint,0,sizeof(endpoint));endpoint.owner=&p->proof;}
static int fixture_token(TP_PLATFORM *p){HANDLE token=NULL;TOKEN_STATISTICS statistics;DWORD bytes=0;unsigned char ids[24];
 /* Real token resource/lifetime, simulated authority decision only. */
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return tp_fail(&p->proof,"MODEL_TOKEN_OPEN",GetLastError());
 p->proof.controlled_refs++;if(!GetTokenInformation(token,TokenStatistics,&statistics,sizeof(statistics),&bytes)||bytes!=sizeof(statistics)){p->token=token;return tp_fail(&p->proof,"MODEL_TOKEN_QUERY",GetLastError());}
 if(!tp_sample(&p->proof,"MODEL_TOKEN_HELD")){p->token=token;return 0;}memcpy(ids,&statistics.TokenId,8);memcpy(ids+8,&statistics.AuthenticationId,8);memcpy(ids+16,&statistics.ModifiedId,8);
 if(!CloseHandle(token)){p->token=token;p->proof.retained=1;return tp_fail(&p->proof,"MODEL_TOKEN_CLOSE_UNKNOWN",GetLastError());}p->proof.controlled_refs--;
 if(p->token_bound&&!tp_eq(ids,p->token_ids,24))return tp_fail(&p->proof,"MODEL_TOKEN_CHANGED",0);memcpy(p->token_ids,ids,24);p->token_bound=1;return 1;
}
static int fixture_files(TP_PLATFORM *p,const char *exe,const char *approval){unsigned i;DWORD wrote;wchar_t path[MAX_PATH];HANDLE f;char model[1024];int n;unsigned char digest[32];
 wcscpy_s(p->folder,MAX_PATH,fixture_folder);strcpy_s(p->exe_hash,65,exe);strcpy_s(p->approval_hash,65,approval);memset(p->request_hash,'c',64);p->request_hash[64]=0;
 for(i=0;i<TP_FILES;i++){
 if(swprintf_s(path,MAX_PATH,L"%sresource-%u.MODEL",p->folder,i)<0)return 0;
 f=CreateFileW(path,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);if(f==INVALID_HANDLE_VALUE)return tp_fail(&p->proof,"MODEL_FILE_CREATE",GetLastError());
 if(!WriteFile(f,"SYNTHETIC_RESOURCE_ONLY",23,&wrote,NULL)||wrote!=23||!FlushFileBuffers(f)){p->token=f;p->proof.controlled_refs++;return tp_fail(&p->proof,"MODEL_FILE_WRITE",GetLastError());}
 if(!CloseHandle(f)){p->token=f;p->proof.controlled_refs++;p->proof.retained=1;return tp_fail(&p->proof,"MODEL_FILE_CLOSE_UNKNOWN",GetLastError());}
 p->files[i].handle=CreateFileW(path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(p->files[i].handle==INVALID_HANDLE_VALUE)return tp_fail(&p->proof,"MODEL_FILE_PIN",GetLastError());p->proof.controlled_refs++;p->files[i].bytes=23;
 if(!GetFileInformationByHandle(p->files[i].handle,&p->files[i].id)||!tp_hash(&p->proof,(const unsigned char *)"SYNTHETIC_RESOURCE_ONLY",23,digest))return 0;
 }
 n=_snprintf_s(model,sizeof(model),_TRUNCATE,"{\"kind\":\"OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY\",\"request_sha256\":\"%s\",\"executable_sha256\":\"%s\",\"one_manual_execution\":true,\"ack_unknown_revocation\":true,\"ack_no_hardware_admission\":true,\"ack_internal_tpm_accounting_unknown\":true,\"UNIT_TEST_MODEL_ONLY\":true}",p->request_hash,p->exe_hash);
 if(n<0)return 0;memcpy(p->approval.bytes,model,(size_t)n);if(!tj_parse(&p->approval,(size_t)n)||!hp_approval_contract(p))return 0;return tp_sample(&p->proof,"MODEL_ARTIFACTS_PINNED");
}
static int fixture_admin(TP_PLATFORM *p){DWORD ids[8],start=GetTickCount(),count,aux=0,used=0,handles;unsigned char basic[48];ULONGLONG parent;FILETIME pc,sc,e,k,u;TP_OWNER *o=&p->proof;
 typedef LONG(NTAPI *QUERY_BASIC)(HANDLE,ULONG,PVOID,ULONG,PULONG);QUERY_BASIC query;
 while((count=GetConsoleProcessList(ids,8))!=2&&GetTickCount()-start<3000)Sleep(5);
 if(count!=2)return tp_fail(o,"MODEL_CONSOLE_CLIENT_COUNT",0);
 p->observer_attempted=1;p->observer_known=GetProcessHandleCount(GetCurrentProcess(),&p->observer_count)!=0;p->observer_error=p->observer_known?0:GetLastError();
 if(!p->observer_known)return tp_fail(o,"NATIVE_OBSERVER_QUERY_FAILED",p->observer_error);if(p->observer_count>124)return tp_fail(o,"NATIVE_OBSERVER_HEADROOM",0);
 query=(QUERY_BASIC)(void *)GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtQueryInformationProcess");if(!query||query(GetCurrentProcess(),0,basic,48,&used)||used!=48)return tp_fail(o,"MODEL_PARENT_QUERY",0);memcpy(&parent,basic+40,8);
 if(!parent||parent>0xffffffffULL)return tp_fail(o,"MODEL_PARENT_PID",0);
 GetWindowThreadProcessId(GetConsoleWindow(),&aux);p->topology_parent=(DWORD)parent;p->topology_console=aux;p->topology_count=count;memcpy(p->topology_ids,ids,count*sizeof(DWORD));
 /* This test environment's pseudo-console HWND belongs to the client. The
  * production guard correctly denies that topology and is NOT relaxed.
  * Borrow self solely as an object-type/resource stand-in; DO NOT sum its
  * count into the administrative budget or report it as an admitted role. */
 p->parent=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,(DWORD)parent);if(!p->parent)return tp_fail(o,"MODEL_PARENT_OPEN",GetLastError());o->controlled_refs++;
 p->console=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,GetCurrentProcessId());if(!p->console)return tp_fail(o,"MODEL_OBSERVER_OPEN",GetLastError());o->controlled_refs++;
 if(!GetProcessTimes(p->parent,&pc,&e,&k,&u)||!GetProcessTimes(p->console,&sc,&e,&k,&u)||!GetProcessHandleCount(p->parent,&handles)||!GetProcessHandleCount(p->console,&handles))return tp_fail(o,"MODEL_PROCESS_OBSERVATION",GetLastError());
 if(!tp_sample(o,"MODEL_OBSERVER_REFS_NO_ADMIN_CREDIT"))return 0;
 if(!CloseHandle(p->console)){o->retained=1;return tp_fail(o,"MODEL_OBSERVER_CLOSE_UNKNOWN",GetLastError());}p->console=NULL;o->controlled_refs--;
 if(!CloseHandle(p->parent)){o->retained=1;return tp_fail(o,"MODEL_PARENT_CLOSE_UNKNOWN",GetLastError());}p->parent=NULL;o->controlled_refs--;return 1;
}
static int fixture_certificate(TP_PLATFORM *p){unsigned char blob[512];ULONG n=0;BCRYPT_RSAKEY_BLOB *head=(BCRYPT_RSAKEY_BLOB *)blob;
 if(BCryptGenerateKeyPair(p->proof.rsa,&endpoint.private_key,2048,0)<0||BCryptFinalizeKeyPair(endpoint.private_key,0)<0)return tp_fail(&p->proof,"MODEL_PRIVATE_KEY",0);
 if(BCryptExportKey(endpoint.private_key,NULL,BCRYPT_RSAPUBLIC_BLOB,blob,sizeof(blob),&n,0)<0||head->cbModulus!=256||head->cbPublicExp!=3)return tp_fail(&p->proof,"MODEL_PUBLIC_EXPORT",0);
 return tp_rsa_import(&p->proof,blob+sizeof(*head)+3);
}
static int fixture_boundary(TP_OWNER *o,const char *phase){TP_PLATFORM *p=o->guard_data;return hp_console_stop(p)&&fixture_token(p)&&fixture_admin(p)&&hp_files_current(p)&&hp_context_current(p,phase);}
static unsigned long real_open(void *r,void **out){RUNTIME *x=r;x->native_open_calls++;return mock_open(&endpoint,out);}
static unsigned long real_send(void *r,void *context,const unsigned char *b,unsigned n,unsigned char *out,unsigned *size){RUNTIME *x=r;x->native_submit_calls++;return mock_send(&endpoint,context,b,n,out,size);}
static unsigned long real_close(void *r,void *context){RUNTIME *x=r;x->native_close_calls++;return mock_close(&endpoint,context);}
static void fixture_before_finish(RUNTIME *r){if(endpoint.private_key&&!r->platform.proof.retained&&(!r->platform.proof.session_attempted||r->platform.proof.session_ended)){if(BCryptDestroyKey(endpoint.private_key)<0){tp_fail(&r->platform.proof,"MODEL_PRIVATE_CLOSE_UNKNOWN",0);r->platform.proof.retained=1;}else{endpoint.private_key=NULL;puts("MODEL_ENDPOINT_PRIVATE_KEY_CLOSED_BEFORE_PROVIDER");}}}
static int child(const wchar_t *folder){wchar_t out[MAX_PATH],a[65],b[65];FILE *file=NULL;int result;wchar_t *argv[4]={L"FIXTURE_ONLY",L"--owner-manual-proof",a,b};
 if(wcslen(folder)+32>=MAX_PATH)return 3;swprintf_s(fixture_folder,MAX_PATH,L"%s\\",folder);swprintf_s(out,MAX_PATH,L"%sresult.txt",fixture_folder);
 if(_wfreopen_s(&file,out,L"wx",stdout)!=0||!file)return 3;
 wmemset(a,L'a',64);a[64]=0;wmemset(b,L'b',64);b[64]=0;puts("FIXTURE AUTHORITY_AND_ADMIN_ROLE_TOPOLOGY_MODEL; REAL_CONSOLE_AND_CLIENT_COUNTS; ADMIN_BUDGET_NOT_OBSERVED");result=fixture_production_entry(4,argv);
 puts("FIXTURE_ONLY_AUTHORITY_MODEL_NO_TPM_NO_REAL_APPROVAL");if(fclose(file)!=0)return 3;return result;
}
static int controller(const wchar_t *base,const wchar_t *mode){STARTUPINFOW si;PROCESS_INFORMATION pi;wchar_t exe[MAX_PATH],folder[MAX_PATH],cmd[3*MAX_PATH],line[100];HANDLE input;INPUT_RECORD records[100];DWORD i,len,wrote,start,exitCode=999;int attached=0;BOOL created;DWORD attach_error=0;
 if(!FreeConsole()&&GetLastError()!=ERROR_INVALID_HANDLE){printf("MODEL_DETACH_DENY error=%lu\n",GetLastError());return 1;}
 if(!GetModuleFileNameW(NULL,exe,MAX_PATH))return 1;if(swprintf_s(folder,MAX_PATH,L"%s\\%s",base,mode)<0||!CreateDirectoryW(folder,NULL))return 1;
 memset(&si,0,sizeof(si));memset(&pi,0,sizeof(pi));si.cb=sizeof(si);si.dwFlags=STARTF_USESHOWWINDOW;si.wShowWindow=SW_HIDE;
 if(swprintf_s(cmd,3*MAX_PATH,L"\"%s\" --fixture-child \"%s\"",exe,folder)<0)return 1;
 created=CreateProcessW(exe,cmd,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);if(!created)return 1;
 start=GetTickCount();while(GetTickCount()-start<3000){if(AttachConsole(pi.dwProcessId)){attached=1;break;}attach_error=GetLastError();Sleep(5);}
 if(!attached){printf("UNKNOWN fixture child attach pid=%lu error=%lu no kill\n",pi.dwProcessId,attach_error);return 1;}
 input=CreateFileW(L"CONIN$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);if(input==INVALID_HANDLE_VALUE)return 1;
 if(!wcscmp(mode,L"wrong"))wcscpy_s(line,100,L"WRONG\r");else{wcscpy_s(line,100,L"PROVE_EK_SESSION ");{size_t prefix=wcslen(line);wmemset(line+prefix,L'c',64);line[prefix+64]=L'\r';line[prefix+65]=0;}}
 len=(DWORD)wcslen(line);memset(records,0,sizeof(records));for(i=0;i<len;i++){records[i].EventType=KEY_EVENT;records[i].Event.KeyEvent.bKeyDown=TRUE;records[i].Event.KeyEvent.wRepeatCount=1;records[i].Event.KeyEvent.uChar.UnicodeChar=line[i];records[i].Event.KeyEvent.wVirtualKeyCode=line[i]==L'\r'?VK_RETURN:0;}
 if(!wcscmp(mode,L"escape")){records[len].EventType=KEY_EVENT;records[len].Event.KeyEvent.bKeyDown=TRUE;records[len].Event.KeyEvent.wRepeatCount=1;records[len].Event.KeyEvent.wVirtualKeyCode=VK_ESCAPE;records[len].Event.KeyEvent.uChar.UnicodeChar=27;len++;}
 if(!WriteConsoleInputW(input,records,len,&wrote)||wrote!=len||!CloseHandle(input))return 1;
 if(WaitForSingleObject(pi.hProcess,15000)!=WAIT_OBJECT_0){printf("UNKNOWN fixture child retained pid=%lu no kill\n",pi.dwProcessId);return 1;}
 if(!GetExitCodeProcess(pi.hProcess,&exitCode)||!CloseHandle(pi.hThread)||!CloseHandle(pi.hProcess))return 1;
 if(!FreeConsole())return 1;printf("FIXTURE mode=%ls exit=%lu; child refs closed; no TBS\n",mode,exitCode);
 return exitCode==(!wcscmp(mode,L"clean")?0UL:2UL)?0:1;
}
int wmain(int argc,wchar_t **argv){if(argc==3&&!wcscmp(argv[1],L"--fixture-child"))return child(argv[2]);if(argc!=3||wcscmp(argv[1],L"--fixture-controller"))return 2;
 if(controller(argv[2],L"clean")||controller(argv[2],L"wrong")||controller(argv[2],L"escape"))return 1;puts("PASS FULL_CONTROL_FLOW_PRIVATE_CONSOLE_MOCK_TPM_ONLY");return 0;}
