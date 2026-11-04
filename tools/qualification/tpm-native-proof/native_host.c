#include "host_platform.h"
#include <tbs.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
/* Only real-mode entry exists here. Tests use a separate transport with no TBS import. */
typedef TBS_RESULT (WINAPI *OPEN_CONTEXT)(PCTBS_CONTEXT_PARAMS,PTBS_HCONTEXT);
typedef TBS_RESULT (WINAPI *SUBMIT)(TBS_HCONTEXT,TBS_COMMAND_LOCALITY,TBS_COMMAND_PRIORITY,PCBYTE,UINT32,PBYTE,PUINT32);
typedef TBS_RESULT (WINAPI *CLOSE_CONTEXT)(TBS_HCONTEXT);
typedef struct RUNTIME {TP_PLATFORM platform;HMODULE module;OPEN_CONTEXT open;SUBMIT submit;CLOSE_CONTEXT close;unsigned native_open_calls,native_submit_calls,native_close_calls;} RUNTIME;
static RUNTIME runtime;
static BOOL WINAPI stop_handler(DWORD code){if(code==CTRL_C_EVENT||code==CTRL_BREAK_EVENT||code==CTRL_CLOSE_EVENT||code==CTRL_LOGOFF_EVENT||code==CTRL_SHUTDOWN_EVENT){InterlockedExchange(&runtime.platform.proof.stop,1);return TRUE;}return FALSE;}
#ifndef TP_HOST_FIXTURE
static unsigned long real_open(void *ctx,void **out){RUNTIME *r=ctx;TBS_CONTEXT_PARAMS2 parameters;TBS_HCONTEXT handle=NULL;TBS_RESULT result;TP_OWNER *o=&r->platform.proof;
 *out=NULL;if(r->module||o->frozen)return ERROR_INVALID_STATE;
 r->module=LoadLibraryExW(L"tbs.dll",NULL,LOAD_LIBRARY_SEARCH_SYSTEM32);if(!r->module)return GetLastError();o->controlled_refs++;
 r->open=(OPEN_CONTEXT)(void *)GetProcAddress(r->module,"Tbsi_Context_Create");r->submit=(SUBMIT)(void *)GetProcAddress(r->module,"Tbsip_Submit_Command");r->close=(CLOSE_CONTEXT)(void *)GetProcAddress(r->module,"Tbsip_Context_Close");
 if(!r->open||!r->submit||!r->close)return ERROR_PROC_NOT_FOUND;
 if(!tp_sample(o,"TBS_MODULE_READY")||!hp_boundary(o,"TBS_MODULE_BEFORE_OPEN"))return ERROR_CANCELLED;
 memset(&parameters,0,sizeof(parameters));parameters.version=TBS_CONTEXT_VERSION_TWO;parameters.includeTpm20=1;r->native_open_calls++;
 result=r->open((PCTBS_CONTEXT_PARAMS)&parameters,&handle);*out=handle;return result;
}
static unsigned long real_send(void *ctx,void *handle,const unsigned char *command,unsigned n,unsigned char *response,unsigned *size){RUNTIME *r=ctx;UINT32 count=*size;TBS_RESULT result;
 if(!r->submit||r->platform.proof.frozen)return ERROR_INVALID_STATE;r->native_submit_calls++;result=r->submit(handle,TBS_COMMAND_LOCALITY_ZERO,TBS_COMMAND_PRIORITY_NORMAL,command,n,response,&count);*size=count;return result;
}
static unsigned long real_close(void *ctx,void *handle){RUNTIME *r=ctx;if(!r->close)return ERROR_INVALID_STATE;r->native_close_calls++;return r->close(handle);}
#else
static unsigned long real_open(void *,void **);
static unsigned long real_send(void *,void *,const unsigned char *,unsigned,unsigned char *,unsigned *);
static unsigned long real_close(void *,void *);
static void fixture_before_finish(RUNTIME *r);
#endif
static int selfcheck(void){unsigned char hash[32];char hex[65];TP_OWNER *o=&runtime.platform.proof;hp_init(&runtime.platform);
 if(!tp_crypto_init(o)||!tp_hash(o,(const unsigned char *)"abc",3,hash))return 2;hp_hex(hash,32,hex);
 if(strcmp(hex,"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"))return 2;
 if(!tp_finish(o))return 2;printf("SELF_CHECK_PASS CNG_ONLY NO_TPM_NO_APPROVAL_NO_AUTHORITY owner_bytes=%Iu platform_bytes=%Iu sampled_peak=%u\n",sizeof(TP_OWNER),sizeof(TP_PLATFORM),o->peak);return 0;
}
int wmain(int argc,wchar_t **argv){TP_PLATFORM *p=&runtime.platform;TP_OWNER *o=&p->proof;char expected_exe[65],expected_approval[65];unsigned char initial[32]={0};int ok=0;
 if(argc==2&&!wcscmp(argv[1],L"--selfcheck"))return selfcheck();
 if(argc!=4||wcscmp(argv[1],L"--owner-manual-proof")){puts("DENY_NO_EXPLICIT_MANUAL_PROOF_PINS; NO_TPM");return 2;}
 hp_init(p);if(!hp_hex_input(argv[2],expected_exe)||!hp_hex_input(argv[3],expected_approval)){tp_fail(o,"COMMAND_PINS_DENY",0);goto finish;}
 if(!SetConsoleCtrlHandler(stop_handler,TRUE)){tp_fail(o,"STOP_HANDLER_INSTALL",GetLastError());goto finish;}
 if(!tp_sample(o,"ENTRY")||!tp_crypto_init(o))goto finish;o->stage="OWNER_METADATA";
 if(!hp_token(p)||!hp_boot(p))goto finish;
 o->stage="ARTIFACT_PINS";if(!hp_files(p,expected_exe,expected_approval))goto finish;
 o->stage="INITIAL_ADMIN";if(!hp_admin(p))goto finish;
 o->stage="OWNER_CONFIRMATION";if(!hp_confirm(p))goto finish;
 o->stage="ADMIN_RECEIPT";if(!hp_receipt(p))goto finish;
 o->stage="EK_PUBLIC_IMPORT";if(!hp_load_certificate(p))goto finish;
 o->transport.user=&runtime;o->transport.open=real_open;o->transport.send=real_send;o->transport.close=real_close;
 ok=tp_run(o,initial);
finish:
#ifdef TP_HOST_FIXTURE
 fixture_before_finish(&runtime);
#endif
 tp_finish(o);
 if(!o->retained&&runtime.module){if(!FreeLibrary(runtime.module)){tp_fail(o,"TBS_MODULE_CLOSE_UNKNOWN",GetLastError());o->retained=1;}else{runtime.module=NULL;o->controlled_refs--;}}
 if(!o->retained&&hp_close(p)&&!o->frozen)tp_sample(o,"PLATFORM_CLOSED");
 #ifdef TP_HOST_FIXTURE
 printf("MOCK_API_COUNTS open=%u submit=%u close=%u; OS_INTERNAL_ACCOUNTING=UNKNOWN\n",runtime.native_open_calls,runtime.native_submit_calls,runtime.native_close_calls);hp_print(p,"MOCK_ENDPOINT_NO_TPM");
#else
 printf("NATIVE_API_COUNTS open=%u submit=%u close=%u; OS_INTERNAL_ACCOUNTING=UNKNOWN\n",runtime.native_open_calls,runtime.native_submit_calls,runtime.native_close_calls);hp_print(p,"WINDOWS_TBS_MANUAL_ONLY");
#endif
 if(o->retained){puts("UNKNOWN: original references and dependencies retained; no retry/kill/guessed FlushContext. Host loss leaves UNKNOWN.");fflush(stdout);for(;;)Sleep(1000);}
 return ok&&o->proof&&o->context_closed&&!strcmp(o->first,"NONE")?0:2;
}
