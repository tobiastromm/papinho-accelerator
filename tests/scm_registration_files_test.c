/* Fixed-name native original acquisition and retained IOSB. No live filesystem. */
#include "server_configuration_commit_win32.h"
#include <winternl.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#ifndef OBJ_DONT_REPARSE
#define OBJ_DONT_REPARSE 0x1000U
#endif
static PAPACC_CONFIGURATION_COMMIT packet;
static PAPACC_SERVICE_REGISTRATION_VIEW original;
static unsigned scenario,opens,closes,checks,violations;
static PAPACC_RESULT model_current(PAPACC_CONFIGURATION_COMMIT *o)
{return o->pending_commit.personal_native.phase==1U&&!o->pending_commit.stop?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;}
static NTSTATUS NTAPI model_open(PHANDLE out,ACCESS_MASK access,POBJECT_ATTRIBUTES a,PIO_STATUS_BLOCK io,PLARGE_INTEGER allocation,ULONG attr,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_size)
{
 (void)allocation;(void)ea;(void)ea_size;opens++;
 if(access!=(FILE_READ_DATA|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE)||
  a->RootDirectory!=(HANDLE)99||!(a->Attributes&OBJ_DONT_REPARSE)||a->SecurityDescriptor||
  attr!=FILE_ATTRIBUTE_NORMAL||share!=FILE_SHARE_READ||disposition!=1U||options!=0x00200060U||
  io!=&packet.pending_commit.personal_native.scm_registration.original_io)violations++;
 if(scenario==1U)return (NTSTATUS)0xc0000022L;
 if(scenario==2U){io->Status=(NTSTATUS)0x103L;return (NTSTATUS)0x103L;}
 *out=(HANDLE)(ULONG_PTR)opens;io->Information=scenario==3U?0U:1U;
 if(scenario==4U)return (NTSTATUS)0xc0000022L;
 return 0;
}
static HMODULE WINAPI model_module(LPCSTR name){(void)name;return (HMODULE)1;}
static FARPROC WINAPI model_proc(HMODULE h,LPCSTR name)
{
 FARPROC result;NTSTATUS (NTAPI *fn)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG)=model_open;
 (void)h;if(strcmp(name,"NtCreateFile"))return NULL;memcpy(&result,&fn,sizeof(result));return result;
}
static BOOL WINAPI model_close(HANDLE h)
{(void)h;closes++;return scenario!=5U;}
#define papacc_configuration_personal_native_current model_current
#define GetModuleHandleA model_module
#define GetProcAddress model_proc
#define CloseHandle model_close
#include "../apps/server/server_scm_registration_files_win32.inc"
#define T(x) do{checks++;if(!(x)){printf("FAIL SCM originals case=%u line=%u opens=%u closes=%u\n",scenario,(unsigned)__LINE__,opens,closes);return 1;}}while(0)
static void reset(unsigned which)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCM_REGISTRATION_ACT *a;
 ZeroMemory(&packet,sizeof(packet));scenario=which;opens=closes=violations=0;
 packet.writer=GetCurrentThreadId();packet.pending_commit.bootstrap_parent=(HANDLE)99;
 n=&packet.pending_commit.personal_native;n->phase=1U;a=&n->scm_registration;
 a->self=a;a->original=&original;a->thread=GetCurrentThreadId();a->invocation=1;a->phase=1;
}
int main(void)
{
 unsigned i,before;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&packet.pending_commit.personal_native;
 reset(0);T(papacc_configuration_scm_registration_image_open(&packet,L"papacc_server.exe")==0);
 T(papacc_configuration_scm_registration_dependency_open(&packet,0)==0&&papacc_configuration_scm_registration_dependency_open(&packet,1)==0);
 T(opens==3&&!violations);T(scm_registration_files_close(n)&&closes==2);T(scm_registration_files_close(n)&&closes==2);
 T(papacc_configuration_scm_registration_dependency_open(&packet,0)!=0&&opens==3);
 T(papacc_configuration_scm_registration_image_open(&packet,L"papacc_server.exe")!=0&&opens==3);
 for(i=1;i<=4;i++){
  reset(i);T(papacc_configuration_scm_registration_dependency_open(&packet,0)!=0&&!violations);
  if(i>=2U){T(n->phase==2U&&packet.unresolved);before=opens;
   T(papacc_configuration_scm_registration_dependency_open(&packet,1)!=0&&opens==before);
   T(!scm_registration_files_close(n)&&!closes);}
 }
 reset(5);T(papacc_configuration_scm_registration_dependency_open(&packet,0)==0);
 T(!scm_registration_files_close(n)&&closes==1&&n->phase==2U);T(!scm_registration_files_close(n)&&closes==1);
 reset(0);packet.pending_commit.stop=1;T(papacc_configuration_scm_registration_dependency_open(&packet,0)!=0&&!opens);
 reset(0);n->scm_registration.consumed=1;T(papacc_configuration_scm_registration_dependency_open(&packet,0)!=0&&!opens);
 reset(0);n->scm_registration.phase=3;T(papacc_configuration_scm_registration_dependency_open(&packet,0)!=0&&!opens);
 reset(0);n->scm_registration.self=NULL;T(papacc_configuration_scm_registration_dependency_open(&packet,0)!=0&&!opens);
 reset(0);T(papacc_configuration_scm_registration_dependency_open(&packet,2)!=0&&!opens);
 T(papacc_configuration_scm_registration_image_open(&packet,L"..\\other.exe")!=0&&!opens);
 printf("PASS SCM_REGISTRATION_FILES checks=%u modeled opens/close only; pending retained; no directory/file/service created\n",checks);return 0;
}
