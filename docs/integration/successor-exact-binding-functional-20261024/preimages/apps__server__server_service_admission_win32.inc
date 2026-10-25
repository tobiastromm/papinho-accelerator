/* Included only in the SAME PS_NATIVE owner; not a second manager. */
#include <aclapi.h>
#include <sddl.h>
#ifndef PS_ADMISSION_TEST
static const WCHAR psa_name[]=L"Global\\PapinhoAccelerator.ED11.SingleLabAdmission";
static const WCHAR psa_launch_path[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.launch.log";
static const WCHAR psa_path[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.intent";
#endif
#ifndef PSA_DIAGNOSTIC
#define PSA_DIAGNOSTIC(api,res,code,value) ((void)0)
#endif
#define PSA native.admission
static DWORD psa_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
static DWORD psa_absent(void)
{
 DWORD a,e;
 if(!PSA.launch){a=GetFileAttributesW(psa_launch_path);if(a!=INVALID_FILE_ATTRIBUTES)return ERROR_ALREADY_EXISTS;e=GetLastError();if(e!=ERROR_FILE_NOT_FOUND)return e;}
 a=GetFileAttributesW(psa_path);if(a!=INVALID_FILE_ATTRIBUTES)return ERROR_ALREADY_EXISTS;
 e=GetLastError();return e==ERROR_FILE_NOT_FOUND?0:e;
}
static DWORD psa_sd(void)
{
 static const WCHAR text[]=L"O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;GA;;;S-1-5-21-785699550-1440458426-1682014700-1108)(A;;GA;;;SY)(A;;GA;;;BA)";
 if(PSA.pending)return ERROR_INVALID_STATE;
 if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(text,SDDL_REVISION_1,&PSA.pending,NULL))return psa_error();return 0;
}
static DWORD psa_free_sd(void)
{if(PSA.pending){if(LocalFree(PSA.pending)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.pending=NULL;}return 0;}
static DWORD psa_verify_mutex(void)
{
 PSECURITY_DESCRIPTOR expected=NULL;PACL actual_acl,expected_acl;PSID owner,expected_owner;BOOL d,present;DWORD e;unsigned i;void *a,*b;
 expected=PSA.pending;PSA.expected=expected;
 if(!GetSecurityDescriptorOwner(expected,&expected_owner,&d)||!GetSecurityDescriptorDacl(expected,&present,&expected_acl,&d)||!present)return ERROR_INVALID_SECURITY_DESCR;
 PSA.pending=NULL;
 e=GetSecurityInfo(PSA.mutex,SE_KERNEL_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,&owner,NULL,&actual_acl,NULL,&PSA.pending);
#ifdef PS_ADMISSION_TEST
 {LPWSTR observed=NULL;if(!e&&ConvertSecurityDescriptorToStringSecurityDescriptorW(PSA.pending,SDDL_REVISION_1,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,&observed,NULL)){wprintf(L"MUTEX_SD=%ls\n",observed);LocalFree(observed);}printf("MUTEX_QUERY=%lu\n",(unsigned long)e);}
#endif
 if(e)PSA_DIAGNOSTIC("GetSecurityInfo","canonical_mutex_descriptor",e,e);
 if(!e){
 if(!owner||!EqualSid(owner,expected_owner)||!actual_acl||actual_acl->AceCount!=3)e=ERROR_ACCESS_DENIED;
 for(i=0;!e&&i<3;i++){
  if(!GetAce(actual_acl,i,&a)||!GetAce(expected_acl,i,&b)||((ACE_HEADER *)a)->AceType!=ACCESS_ALLOWED_ACE_TYPE||((ACE_HEADER *)a)->AceFlags||((ACE_HEADER *)a)->AceSize!=((ACE_HEADER *)b)->AceSize||!EqualSid((PSID)&((ACCESS_ALLOWED_ACE *)a)->SidStart,(PSID)&((ACCESS_ALLOWED_ACE *)b)->SidStart)||(((ACCESS_ALLOWED_ACE *)a)->Mask!=GENERIC_ALL&&((ACCESS_ALLOWED_ACE *)a)->Mask!=MUTEX_ALL_ACCESS))e=ERROR_ACCESS_DENIED;
 }
 }
 if(LocalFree(expected)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.expected=NULL;
 if(psa_free_sd())return ERROR_INVALID_HANDLE;return e;
}
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
#include "server_service_admission_bd_win32.inc"
#endif
DWORD papacc_server_service_admission_bind_launch(DWORD pid,unsigned __int64 creation)
{
 FILETIME ct,et,kt,ut;BY_HANDLE_FILE_INFORMATION id;char expected[200],observed[200];DWORD n;int length;
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 if(PSA.bd.used)return ERROR_INVALID_STATE;
#endif
 if(PSA.phase||PSA.mutex||PSA.launch||PSA.launcher||!pid||!creation)return ERROR_INVALID_STATE;
 PSA.launcher=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,pid);if(!PSA.launcher)return psa_error();
 if(!GetProcessTimes(PSA.launcher,&ct,&et,&kt,&ut)||(((unsigned __int64)ct.dwHighDateTime<<32)|ct.dwLowDateTime)!=creation||WaitForSingleObject(PSA.launcher,0)!=WAIT_TIMEOUT)return ERROR_INVALID_DATA;
 PSA.launch=CreateFileW(psa_launch_path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(PSA.launch==INVALID_HANDLE_VALUE){PSA.launch=NULL;return psa_error();}
 if(!GetFileInformationByHandle(PSA.launch,&id)||id.nNumberOfLinks!=1||(id.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))return ERROR_INVALID_DATA;
 length=sprintf_s(expected,sizeof(expected),"ADMISSION=75330d11cc7e4123ae62eadc9a59c2f5; OWNER=%lu; CREATION=%llu\n",(unsigned long)pid,creation);
 if(length<=0||!ReadFile(PSA.launch,observed,(DWORD)length,&n,NULL)||n!=(DWORD)length||memcmp(expected,observed,n))return ERROR_INVALID_DATA;
 return 0;
}
DWORD papacc_server_service_admission_enter(HANDLE configuration,const char *attempt)
{
 DWORD e,w;SECURITY_ATTRIBUTES sa;BY_HANDLE_FILE_INFORMATION id;
 if(PSA.phase||PSA.mutex||PSA.intent||PSA.pending)return ERROR_INVALID_STATE;
 if(attempt&&strcmp(attempt,"75330d11cc7e4123ae62eadc9a59c2f5"))return ERROR_INVALID_DATA;
 if(attempt){
  if(!PSA.launch||!PSA.launcher)return ERROR_ACCESS_DENIED;
  if(!configuration||!GetFileInformationByHandle(configuration,&id))return ERROR_INVALID_HANDLE;
#ifndef PS_ADMISSION_TEST
  if(id.dwVolumeSerialNumber!=417614100UL||id.nFileIndexHigh!=393216UL||id.nFileIndexLow!=1120240UL)return ERROR_INVALID_DATA;
#else
  if(id.dwVolumeSerialNumber!=psa_identity.dwVolumeSerialNumber||id.nFileIndexHigh!=psa_identity.nFileIndexHigh||id.nFileIndexLow!=psa_identity.nFileIndexLow)return ERROR_INVALID_DATA;
#endif
  if(id.nNumberOfLinks!=1||(id.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))return ERROR_INVALID_DATA;
 }
 if((e=psa_absent())!=0)return e;
 if((e=psa_sd())!=0)return e;
 sa.nLength=sizeof(sa);sa.lpSecurityDescriptor=PSA.pending;sa.bInheritHandle=FALSE;
 PSA.mutex=CreateMutexExW(&sa,psa_name,0,SYNCHRONIZE|MUTEX_MODIFY_STATE|READ_CONTROL);
 if(!PSA.mutex){e=psa_error();PSA_DIAGNOSTIC("CreateMutexExW","canonical_mutex",e,e);
#ifdef PS_ADMISSION_TEST
 printf("MUTEX_CREATE_ERROR=%lu\n",(unsigned long)e);
#endif
if(psa_free_sd())return ERROR_INVALID_HANDLE;return e;}
 if((e=psa_verify_mutex())!=0){if(PSA.phase==3)return e;goto rejected;}
 w=WaitForSingleObject(PSA.mutex,0);
 if(w==WAIT_ABANDONED){PSA_DIAGNOSTIC("WaitForSingleObject","canonical_mutex_abandoned",ERROR_ABANDONED_WAIT_0,w);PSA.owned=1;PSA.phase=3;PSA.thread=GetCurrentThreadId();return ERROR_ABANDONED_WAIT_0;}
 if(w!=WAIT_OBJECT_0){e=w==WAIT_TIMEOUT?ERROR_BUSY:psa_error();PSA_DIAGNOSTIC("WaitForSingleObject",w==WAIT_TIMEOUT?"canonical_mutex_timeout":"canonical_mutex_wait_failed",e,w);goto rejected;}
 PSA.owned=1;PSA.thread=GetCurrentThreadId();PSA.phase=1;PSA.lab=attempt!=NULL;
 if((e=psa_absent())!=0){DWORD close=papacc_server_service_admission_finish(1);return close?close:e;}
 return 0;
 rejected:
 if(!CloseHandle(PSA.mutex)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.mutex=NULL;return e;
}


#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
static DWORD psa_n0_metrics_fresh(const char *gate)
{
 ULONGLONG now=GetTickCount64();
 if(PSA.bd.metrics_valid&&now>=PSA.bd.metrics_at&&now-PSA.bd.metrics_at<=1000ULL)return 0;
 PSA_DIAGNOSTIC("GetTickCount64","n0_metric_sample_stale_or_missing",ERROR_TIMEOUT,ERROR_TIMEOUT);
 printf("IG_FRESHNESS gate=%s now=%llu metrics_at=%llu valid=%u age_known=%u age_ms=%llu dossier_began=%llu validated_at=%llu; LIMIT_MS=1000; NO_REFRESH_NO_RETRY\n",gate,now,PSA.bd.metrics_at,PSA.bd.metrics_valid,(unsigned)(PSA.bd.metrics_valid&&now>=PSA.bd.metrics_at),PSA.bd.metrics_valid&&now>=PSA.bd.metrics_at?now-PSA.bd.metrics_at:0ULL,PSA.bd.context.began,PSA.bd.context.validated_at);
 return ERROR_TIMEOUT;
}
#endif

/* Same owner, no new lock/claim. N0 positive binding requires full protected
 * context and factual metrics; invocation cannot synthesize the binding. */
DWORD papacc_server_service_admission_n0_arm(void)
{
#if defined(PAPACC_N0_POSITIVE_PINNED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 DWORD e;if(PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId()||PSA.stop||PSA.consumed||PSA.lab||!PSA.bd.bound||!PSA.bd.binding.n0)return ERROR_INVALID_STATE;
 if((e=psa_absent())||(e=papacc_server_service_admission_bd_revalidate(PSA.bd.check)))return e;
 if((e=psa_n0_metrics_fresh("ARM")))return e;
 PSA.lab=1;return 0;
#else
 return ERROR_NOT_SUPPORTED;
#endif
}

DWORD papacc_server_service_admission_consume(void)
{
 static const char record[]="ATTEMPT=75330d11cc7e4123ae62eadc9a59c2f5; CONSUMED; NO_RETRY_NO_RECOVERY; UNKNOWN_UNTIL_FACTUAL_CLOSE\r\n";
 SECURITY_ATTRIBUTES sa;DWORD e,n;BY_HANDLE_FILE_INFORMATION id;
 if(PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId()||PSA.consumed||PSA.stop||!PSA.lab)return ERROR_INVALID_STATE;
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 if(PSA.bd.used){DWORD z=papacc_server_service_admission_bd_revalidate(PSA.bd.check);if(z)return z;z=ps_bd_inventory(&PSA.bd.binding,&PSA.bd.inventory,GetTickCount64());if(z)return z;if(PSA.bd.binding.n0&&(z=psa_n0_metrics_fresh("CONSUME")))return z;}
#endif
 PSA.consumed=1;
 if((e=psa_sd())!=0){PSA.phase=3;return e;}
 sa.nLength=sizeof(sa);sa.lpSecurityDescriptor=PSA.pending;sa.bInheritHandle=FALSE;
 PSA.intent=CreateFileW(psa_path,GENERIC_WRITE,FILE_SHARE_READ,&sa,CREATE_NEW,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(PSA.intent==INVALID_HANDLE_VALUE){PSA.intent=NULL;e=psa_error();PSA.phase=3;if(psa_free_sd())return ERROR_INVALID_HANDLE;return e;}
 if(psa_free_sd())return ERROR_INVALID_HANDLE;
 if(!GetFileInformationByHandle(PSA.intent,&id)||id.nNumberOfLinks!=1||(id.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||!WriteFile(PSA.intent,record,sizeof(record)-1,&n,NULL)||n!=sizeof(record)-1||!FlushFileBuffers(PSA.intent)){PSA.phase=3;return psa_error();}
 return 0;
}
DWORD papacc_server_service_admission_effect(void)
{
 if(PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId()||!PSA.consumed||!PSA.intent||PSA.stop)return ERROR_INVALID_STATE;
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 if(PSA.bd.used){DWORD e=papacc_server_service_admission_bd_revalidate(PSA.bd.check);if(e)return e;e=ps_bd_inventory(&PSA.bd.binding,&PSA.bd.inventory,GetTickCount64());if(e)return e;if(PSA.bd.binding.n0&&(e=psa_n0_metrics_fresh("EFFECT")))return e;}
#endif
 PSA.effects=1;return 0;
}
DWORD papacc_server_service_admission_stop(void)
{if(PSA.phase!=1||PSA.thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;PSA.stop=1;return 0;}
void papacc_server_service_admission_retain(void){if(PSA.phase)PSA.phase=3;}
DWORD papacc_server_service_admission_finish(int quiescent)
{
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 /* Context borrows/files must close factually BEFORE releasing this owner. */
 if(ps_bd_context_pending(&PSA.bd.context)){if(PSA.phase==1&&PSA.owned&&PSA.thread==GetCurrentThreadId())PSA.phase=3;return ERROR_INVALID_STATE;}
#endif
 if(!PSA.phase&&!PSA.mutex){
  if(!quiescent)return ERROR_INVALID_STATE;
 #if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 {DWORD bd_close=papacc_server_service_admission_bd_close(1);if(bd_close)return bd_close;}
#endif
 if(PSA.launch){if(!CloseHandle(PSA.launch)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.launch=NULL;}
  if(PSA.launcher){if(!CloseHandle(PSA.launcher)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.launcher=NULL;}
  return 0;
 }
 if(PSA.phase!=1||!PSA.owned||PSA.thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if(!quiescent){PSA.phase=3;return ERROR_INVALID_STATE;}
#if defined(PAPACC_SCM_INSTRUMENTED_LAB) || defined(PAPACC_SERVICE_BD_MODEL)
 {DWORD bd_close=papacc_server_service_admission_bd_close(1);if(bd_close)return bd_close;}
#endif
 if(PSA.launch){if(!CloseHandle(PSA.launch)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.launch=NULL;}
 if(PSA.launcher){if(!CloseHandle(PSA.launcher)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.launcher=NULL;}
 if(PSA.intent){if(!CloseHandle(PSA.intent)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.intent=NULL;}
 if(!ReleaseMutex(PSA.mutex)){PSA.phase=3;return psa_error();}PSA.owned=0;
 if(!CloseHandle(PSA.mutex)){PSA.phase=3;return ERROR_INVALID_HANDLE;}PSA.mutex=NULL;PSA.phase=2;
 /* Persistent consumed record is NEVER deleted, rewritten or interpreted as
  * a clean-release receipt. Fresh owner objects still deny replay. */
 return 0;
}
#undef PSA
