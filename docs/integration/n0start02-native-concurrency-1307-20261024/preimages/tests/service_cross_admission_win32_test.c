#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "../apps/server/server_service_admission_win32.h"
static WCHAR psa_name[200],psa_path[1024],psa_launch_path[1100];
static BY_HANDLE_FILE_INFORMATION psa_identity;
static struct {PS_ADMISSION admission;} native;
#include "../tools/qualification/scm-busy-diagnostic.h"
#define PSA_DIAGNOSTIC(api,res,code,value) ig_diag_note("CANONICAL_ADMISSION",api,res,code,value,0,4)
#define PS_ADMISSION_TEST 1
#include "../apps/server/server_service_admission_win32.inc"
int wmain(int argc,WCHAR **argv)
{
 HANDLE file;DWORD e,n;char mode;BY_HANDLE_FILE_INFORMATION id;
 if(argc!=5||wcslen(argv[1])>=200||wcslen(argv[2])>=1024)return 2;
 wcscpy_s(psa_name,200,argv[1]);wcscpy_s(psa_path,1024,argv[2]);
 file=CreateFileW(argv[3],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(file==INVALID_HANDLE_VALUE)return 2;
 if(!GetFileInformationByHandle(file,&id))return 2;psa_identity=id;
 if(!wcscmp(argv[4],L"identity"))psa_identity.nFileIndexLow++;
 swprintf_s(psa_launch_path,1100,L"%ls.launch",psa_path);
 if(wcscmp(argv[4],L"ordinary")){
  FILETIME ct,et,kt,ut;unsigned __int64 creation;char header[200];HANDLE log;DWORD written;int length;
  if(!GetProcessTimes(GetCurrentProcess(),&ct,&et,&kt,&ut))return 2;creation=((unsigned __int64)ct.dwHighDateTime<<32)|ct.dwLowDateTime;
  log=CreateFileW(psa_launch_path,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
  if(log==INVALID_HANDLE_VALUE){printf("ENTER=%lu PID=%lu STORAGE=%llu\n",(unsigned long)GetLastError(),(unsigned long)GetCurrentProcessId(),(unsigned long long)sizeof(native.admission));CloseHandle(file);return 0;}
  length=sprintf_s(header,sizeof(header),"ADMISSION=75330d11cc7e4123ae62eadc9a59c2f5; OWNER=%lu; CREATION=%llu\n",(unsigned long)GetCurrentProcessId(),creation);
  if(length<=0||!WriteFile(log,header,(DWORD)length,&written,NULL)||written!=(DWORD)length||!FlushFileBuffers(log))return 2;
  e=papacc_server_service_admission_bind_launch(GetCurrentProcessId(),creation);if(!CloseHandle(log)||e)return 2;
 }
 e=papacc_server_service_admission_enter(file,!wcscmp(argv[4],L"wrong")?"75330d11cc7e4123ae62eadc9a59c2f4":(!wcscmp(argv[4],L"ordinary")?NULL:"75330d11cc7e4123ae62eadc9a59c2f5"));
 if(!CloseHandle(file))return 2;
 printf("ENTER=%lu PID=%lu STORAGE=%llu\n",(unsigned long)e,(unsigned long)GetCurrentProcessId(),(unsigned long long)sizeof(native.admission));fflush(stdout);
 if(e){DWORD close=papacc_server_service_admission_finish(1);printf("REJECT_CLOSE=%lu\n",(unsigned long)close);return close?2:0;}
 for(;;){mode=(char)getchar();if(mode=='\n'||mode=='\r')continue;
  if(mode=='v'){papacc_server_service_admission_retain();puts("UNKNOWN_BEFORE_EFFECT=3");fflush(stdout);continue;}
  if(mode=='x')return 0; /* fixture orphan; NEVER a cleanup claim */
  if(mode=='c'||mode=='e'||mode=='d'||mode=='u'){
   e=papacc_server_service_admission_consume();printf("CONSUME=%lu\n",(unsigned long)e);
   if(mode=='d')printf("DOUBLE=%lu\n",(unsigned long)papacc_server_service_admission_consume());
   if(mode=='e'||mode=='u')printf("EFFECT_MODEL=%lu\n",(unsigned long)papacc_server_service_admission_effect());
   if(mode=='u'){printf("UNKNOWN=%lu\n",(unsigned long)papacc_server_service_admission_finish(0));fflush(stdout);continue;}
  }
  if(mode=='s'){
   printf("STOP=%lu\n",(unsigned long)papacc_server_service_admission_stop());
   printf("AFTER_STOP=%lu\n",(unsigned long)papacc_server_service_admission_consume());
  }
  e=papacc_server_service_admission_finish(1);n=native.admission.phase;
  printf("FINISH=%lu PHASE=%lu MUTEX=%u INTENT=%u; NO_SCM_NO_START\n",(unsigned long)e,(unsigned long)n,native.admission.mutex!=NULL,native.admission.intent!=NULL);fflush(stdout);return e?2:0;
 }
}
