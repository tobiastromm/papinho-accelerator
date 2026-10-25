/* Real closed bootstrap + real private IPC; synthetic body only, no admission. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../tools/qualification/scm-n0-private-stdout-native.h"
#define PAPACC_N0START03_PRIVATE_POSITIVE 1
static int ig_transport_body(int argc,char **argv){if(argc!=9||strcmp(argv[1],"--cycle-n0"))return 70;
 if(PaccTransportPrintf("CLOSED_POSITIVE_ENTRY_REAL_IPC_SYNTHETIC_BODY; NO_OWNER_NO_SCM_NO_INTENT\n")<0)return 71;return 0;}
__declspec(noreturn) static void ig_transport_write_failed(void){fprintf(stderr,"ISOLATED_TRANSPORT_UNKNOWN_CONTAINMENT_NOT_PRODUCT_CLEANUP\n");ExitProcess(2);}
#define main closed_entry
#include "../tools/qualification/scm-n0-private-stdout-entry.inc"
#undef main
static ULONGLONG created(HANDLE h){FILETIME c,e,k,u;if(!GetProcessTimes(h,&c,&e,&k,&u))return 0;return ((ULONGLONG)c.dwHighDateTime<<32)|c.dwLowDateTime;}
int wmain(int argc,WCHAR **argv){WCHAR exe[1024],args[1600];DWORD e,n,code=0;BYTE bytes[4096];char text[8192];size_t used=0;
 if(argc==13){char storage[13][1024],*a[13];unsigned i;for(i=0;i<13;i++){if(!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,argv[i],-1,storage[i],1024,NULL,NULL))return 72;a[i]=storage[i];}return closed_entry(13,a);}
 if(argc!=1||!GetModuleFileNameW(NULL,exe,1024))return 73;
 if(_snwprintf_s(args,1600,_TRUNCATE,L"--cycle-n0 0b1de869e11493b7cb08fb329578f6ce %lu %llu %lu 1 1 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",(unsigned long)GetCurrentProcessId(),created(GetCurrentProcess()),(unsigned long)GetCurrentThreadId())<0)return 74;
 if((e=PaccTransportOpen())||(e=PaccTransportCreate(exe,args))||(e=PaccTransportAuthenticate())){fprintf(stderr,"REAL_ENTRY_PARENT_DENY=%lu\n",(unsigned long)e);return 75;}
 do{e=PaccTransportRead(bytes,sizeof(bytes),&n);if(e||used+n>=sizeof(text))return 76;if(n){memcpy(text+used,bytes,n);used+=n;}}while(n);text[used]=0;
 if(PaccTransportWait(&code)||code||!strstr(text,"CLOSED_POSITIVE_ENTRY_REAL_IPC_SYNTHETIC_BODY")||PaccTransportClose())return 77;
 puts("REAL_POSITIVE_BOOTSTRAP_TRANSPORT=PASS; CLOSED_ARGV_REAL_ORIGINAL_PARENT_AUTH; SYNTHETIC_BODY_NO_ADMISSION_NO_EFFECT");return 0;}
