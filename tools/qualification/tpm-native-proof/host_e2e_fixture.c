/* Full production entry control flow; compile-time authority/endpoint substitutions.
 * No actual TBS implementation is compiled here. This binary cannot prove TPM possession. */
#include "host_platform.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static wchar_t fixture_folder[MAX_PATH];static int fixture_maximum;
static int fixture_role_evidence(TP_PLATFORM *,HANDLE,unsigned);
#define HP_ADMIN_EVIDENCE fixture_role_evidence
#include "host_platform.c"
#undef HP_ADMIN_EVIDENCE

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
static void fixture_init(TP_PLATFORM *p){hp_init(p);p->proof.boundary=fixture_boundary;memset(&endpoint,0,sizeof(endpoint));endpoint.owner=&p->proof;endpoint.mode=fixture_maximum?18:0;}
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
#include "native_role_fixture.inc"

static int fixture_admin(TP_PLATFORM *p){DWORD ids[8],start=GetTickCount();
 /* Controller attachment synchronization belongs only to this isolated test. */
 while(GetConsoleProcessList(ids,8)!=2&&GetTickCount()-start<3000)Sleep(5);
 return hp_admin(p);
}

static int fixture_certificate(TP_PLATFORM *p){unsigned char blob[512];ULONG n=0;BCRYPT_RSAKEY_BLOB *head=(BCRYPT_RSAKEY_BLOB *)blob;
 if(BCryptGenerateKeyPair(p->proof.rsa,&endpoint.private_key,2048,0)<0||BCryptFinalizeKeyPair(endpoint.private_key,0)<0)return tp_fail(&p->proof,"MODEL_PRIVATE_KEY",0);
 if(BCryptExportKey(endpoint.private_key,NULL,BCRYPT_RSAPUBLIC_BLOB,blob,sizeof(blob),&n,0)<0||head->cbModulus!=256||head->cbPublicExp!=3)return tp_fail(&p->proof,"MODEL_PUBLIC_EXPORT",0);
 return tp_rsa_import(&p->proof,blob+sizeof(*head)+3);
}
static int fixture_boundary(TP_OWNER *o,const char *phase){TP_PLATFORM *p=o->guard_data;return hp_console_stop(p)&&fixture_token(p)&&fixture_admin(p)&&hp_files_current(p)&&hp_context_current(p,phase);}
static unsigned long real_open(void *r,void **out){RUNTIME *x=r;if(!fixture_boundary(&x->platform.proof,"MODEL_TBS_MODULE_BEFORE_OPEN"))return ERROR_CANCELLED;x->native_open_calls++;return mock_open(&endpoint,out);}
static unsigned long real_send(void *r,void *context,const unsigned char *b,unsigned n,unsigned char *out,unsigned *size){RUNTIME *x=r;x->native_submit_calls++;return mock_send(&endpoint,context,b,n,out,size);}
static unsigned long real_close(void *r,void *context){RUNTIME *x=r;x->native_close_calls++;return mock_close(&endpoint,context);}
static void fixture_before_finish(RUNTIME *r){if(endpoint.private_key&&!r->platform.proof.retained&&(!r->platform.proof.session_attempted||r->platform.proof.session_ended)){if(BCryptDestroyKey(endpoint.private_key)<0){tp_fail(&r->platform.proof,"MODEL_PRIVATE_CLOSE_UNKNOWN",0);r->platform.proof.retained=1;}else{endpoint.private_key=NULL;puts("MODEL_ENDPOINT_PRIVATE_KEY_CLOSED_BEFORE_PROVIDER");}}}
static int child(const wchar_t *folder){wchar_t out[MAX_PATH],a[65],b[65];FILE *file=NULL;int result;wchar_t *argv[4]={L"FIXTURE_ONLY",L"--owner-manual-proof",a,b};
 if(wcslen(folder)+32>=MAX_PATH)return 3;swprintf_s(fixture_folder,MAX_PATH,L"%s\\",folder);swprintf_s(out,MAX_PATH,L"%sresult.txt",fixture_folder);
 if(_wfreopen_s(&file,out,L"wx",stdout)!=0||!file)return 3;
 wmemset(a,L'a',64);a[64]=0;wmemset(b,L'b',64);b[64]=0;puts("FIXTURE TOKEN_AUTHORITY_PARENT_FILE_MODEL; REAL_C0_GUARD_KERNEL_CONSOLE_AND_METRICS; NO_OPERATIONAL_ADMIN_CREDIT");result=fixture_production_entry(4,argv);
 puts("FIXTURE_ONLY_AUTHORITY_MODEL_NO_TPM_NO_REAL_APPROVAL");if(fclose(file)!=0)return 3;return result;
}
static int controller(const wchar_t *base,const wchar_t *mode){STARTUPINFOW si;PROCESS_INFORMATION pi;wchar_t exe[MAX_PATH],folder[MAX_PATH],cmd[3*MAX_PATH],line[160];HANDLE input;INPUT_RECORD records[192];DWORD i,len,wrote,start,exitCode=999;int attached=0;BOOL created;DWORD attach_error=0;
 if(!FreeConsole()&&GetLastError()!=ERROR_INVALID_HANDLE){printf("MODEL_DETACH_DENY error=%lu\n",GetLastError());return 1;}
 if(!GetModuleFileNameW(NULL,exe,MAX_PATH))return 1;if(swprintf_s(folder,MAX_PATH,L"%s\\%s",base,mode)<0||!CreateDirectoryW(folder,NULL))return 1;
 memset(&si,0,sizeof(si));memset(&pi,0,sizeof(pi));si.cb=sizeof(si);si.dwFlags=STARTF_USESHOWWINDOW;si.wShowWindow=SW_HIDE;
 if(swprintf_s(cmd,3*MAX_PATH,L"\"%s\" %s \"%s\"",exe,!wcscmp(mode,L"maximum")?L"--fixture-child-max":L"--fixture-child",folder)<0)return 1;
 created=CreateProcessW(exe,cmd,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);if(!created)return 1;
 start=GetTickCount();while(GetTickCount()-start<3000){if(AttachConsole(pi.dwProcessId)){attached=1;break;}attach_error=GetLastError();Sleep(5);}
 if(!attached){printf("UNKNOWN fixture child attach pid=%lu error=%lu no kill\n",pi.dwProcessId,attach_error);return 1;}
 input=CreateFileW(L"CONIN$",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);if(input==INVALID_HANDLE_VALUE)return 1;
 if(!wcscmp(mode,L"wrong"))wcscpy_s(line,160,L"WRONG\r");else{ULONGLONG raw=0;ULONG used=0;DWORD aux;HANDLE h;FILETIME pc,ac,e,k,u;wchar_t hash[65];HP_NATIVE_QUERY query=(HP_NATIVE_QUERY)(void *)GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtQueryInformationProcess");
 if(!query||query(pi.hProcess,49,&raw,8,&used)||used!=8||!GetProcessTimes(GetCurrentProcess(),&pc,&e,&k,&u))return 1;aux=(DWORD)(raw&~3ULL);h=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,aux);if(!h)return 1;
 if(!GetProcessTimes(h,&ac,&e,&k,&u)||!CloseHandle(h))return 1;wmemset(hash,L'c',64);hash[64]=0;
 if(swprintf_s(line,160,L"PROVE_EK_SESSION %s %lu %llu %lu %llu\r",hash,GetCurrentProcessId(),ft(&pc),aux,ft(&ac))<0)return 1;}

 len=(DWORD)wcslen(line);if(len+1>sizeof(records)/sizeof(records[0]))return 1;memset(records,0,sizeof(records));for(i=0;i<len;i++){records[i].EventType=KEY_EVENT;records[i].Event.KeyEvent.bKeyDown=TRUE;records[i].Event.KeyEvent.wRepeatCount=1;records[i].Event.KeyEvent.uChar.UnicodeChar=line[i];records[i].Event.KeyEvent.wVirtualKeyCode=line[i]==L'\r'?VK_RETURN:0;}
 if(!wcscmp(mode,L"escape")){records[len].EventType=KEY_EVENT;records[len].Event.KeyEvent.bKeyDown=TRUE;records[len].Event.KeyEvent.wRepeatCount=1;records[len].Event.KeyEvent.wVirtualKeyCode=VK_ESCAPE;records[len].Event.KeyEvent.uChar.UnicodeChar=27;len++;}
 if(!WriteConsoleInputW(input,records,len,&wrote)||wrote!=len||!CloseHandle(input))return 1;
 if(WaitForSingleObject(pi.hProcess,15000)!=WAIT_OBJECT_0){printf("UNKNOWN fixture child retained pid=%lu no kill\n",pi.dwProcessId);return 1;}
 if(!GetExitCodeProcess(pi.hProcess,&exitCode)||!CloseHandle(pi.hThread)||!CloseHandle(pi.hProcess))return 1;
 if(!FreeConsole())return 1;printf("FIXTURE mode=%ls exit=%lu; child refs closed; no TBS\n",mode,exitCode);
 return exitCode==((!wcscmp(mode,L"clean")||!wcscmp(mode,L"maximum"))?0UL:2UL)?0:1;
}
#ifndef TP_E2E_ENTRY
#define TP_E2E_ENTRY wmain
#endif
int TP_E2E_ENTRY(int argc,wchar_t **argv){if(argc==3&&(!wcscmp(argv[1],L"--fixture-child")||!wcscmp(argv[1],L"--fixture-child-max"))){fixture_maximum=!wcscmp(argv[1],L"--fixture-child-max");return child(argv[2]);}if(argc!=3||wcscmp(argv[1],L"--fixture-controller"))return 2;
 if(controller(argv[2],L"clean")||controller(argv[2],L"wrong")||controller(argv[2],L"escape")||controller(argv[2],L"maximum"))return 1;puts("PASS FULL_CONTROL_FLOW_PRIVATE_CONSOLE_MOCK_TPM_ONLY");return 0;}
