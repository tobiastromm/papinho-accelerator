/* Actual presenter/producer + SCM primitive; native acquisition is modeled.
 * No modeled callback supplies authority: actual confirmation/take must pass. */
#define WIN32_LEAN_AND_MEAN 1
#define PAPACC_SCM_ORIGINAL_PRODUCER_MODEL 1
int papacc_server_launcher_scq_closed_evidence_current(void){return 0;}
int papacc_server_launcher_scq_closed_arm_running(void){return 0;}
#define main primitive_regression_main
#include "service_scm_virtual_model_test.c"
#undef main
#undef papacc_server_launcher_scm_registration_target
#include "server_launcher_owner_win32.h"
#include "server_c8_private_storage_layout_win32.h"
#include <bcrypt.h>
#include <psapi.h>
static PAPACC_SERVER_LAUNCHER_OWNER_WIN32 root;
static volatile LONG state;
static PAPACC_U64 next_invocation;
static unsigned reg_case,reg_reads,reg_answer_pos,reg_checks,reg_native_closes,reg_scenarios;
static char reg_transcript[16384],reg_answer[96];
static unsigned resource_case,resource_calls,resource_handle_calls;
static unsigned diag_case,diag_rng_calls,diag_write_calls,diag_acl_calls,generic_layout_checks,binary_layout_checks;
static LONGLONG resource_clock;
static BOOL WINAPI reg_frequency(LARGE_INTEGER *f)
{resource_calls++;if(resource_case==1U){SetLastError(ERROR_NOT_SUPPORTED);return FALSE;}f->QuadPart=resource_case==7U?0:resource_case==15U&&resource_handle_calls?20000000:10000000;return TRUE;}
static BOOL WINAPI reg_counter(LARGE_INTEGER *t)
{resource_calls++;resource_clock++;t->QuadPart=resource_case==6U&&!(resource_clock&1)?resource_clock-2:resource_clock;return TRUE;}
static BOOL WINAPI reg_handles(HANDLE h,LPDWORD count)
{(void)h;resource_calls++;resource_handle_calls++;if(resource_case==2U||resource_case==21U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 *count=resource_case==12U?125U:resource_case==13U?129U:resource_case==14U&&resource_handle_calls>1U?125U:resource_case==19U?124U:80U;return TRUE;}
static BOOL WINAPI reg_memory(HANDLE h,PPROCESS_MEMORY_COUNTERS out,DWORD bytes)
{PROCESS_MEMORY_COUNTERS_EX *m=(PROCESS_MEMORY_COUNTERS_EX *)out;(void)h;(void)bytes;resource_calls++;if(resource_case==3U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}m->PrivateUsage=1024U+resource_handle_calls;m->WorkingSetSize=2048U;return TRUE;}
static BOOL WINAPI reg_global(LPMEMORYSTATUSEX m)
{resource_calls++;if(resource_case==4U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}m->ullTotalPhys=33554432U;m->ullAvailPhys=resource_case==11U?67108864U:resource_case==16U?0U:resource_case==18U?1U:16777216U; if(resource_case==22U||(resource_case==25U&&creates))m->ullAvailPhys=PAPACC_LAUNCHER_MANAGED_LIMIT-1U; if(resource_case==24U)m->ullAvailPhys=PAPACC_LAUNCHER_MANAGED_LIMIT; if(resource_case==26U&&creates)model_stopped=1;return TRUE;}
static BOOL WINAPI reg_performance(PPERFORMANCE_INFORMATION p,DWORD bytes)
{(void)bytes;resource_calls++;if(resource_case==5U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 p->PageSize=resource_case==8U?0:4096;p->CommitLimit=resource_case==10U?(SIZE_T)-1:4096U;p->CommitTotal=resource_case==9U?4097U:resource_case==17U?4096U:1024U; if(resource_case==23U){p->CommitLimit=PAPACC_LAUNCHER_MANAGED_LIMIT/4096U;p->CommitTotal=1U;} if(resource_case==24U){p->CommitLimit=PAPACC_LAUNCHER_MANAGED_LIMIT/4096U;p->CommitTotal=0U;}return TRUE;}
static BOOL WINAPI reg_mode(HANDLE h,LPDWORD m){(void)h;*m=0;if(reg_case==12U){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static HANDLE WINAPI reg_std(DWORD which){return (HANDLE)(ULONG_PTR)(which==STD_INPUT_HANDLE?21U:22U);}
static BOOL WINAPI reg_write(HANDLE h,const void *data,DWORD bytes,LPDWORD used,LPVOID reserved)
{
 size_t z=strlen(reg_transcript);(void)h;(void)reserved;diag_write_calls++;
 if((diag_case==20U&&diag_write_calls==1U)||(diag_case==22U&&diag_write_calls==2U)){SetLastError(ERROR_WRITE_FAULT);return FALSE;}
 if(diag_case==21U&&diag_write_calls==1U){*used=bytes-1U;return TRUE;}
 if(z+bytes>=sizeof(reg_transcript))return FALSE;
 memcpy(reg_transcript+z,data,bytes);reg_transcript[z+bytes]=0;*used=bytes;
 if(bytes>19U&&!memcmp(data,"OWNER_SCM_REGISTER ",19U)){memcpy(reg_answer,data,bytes);reg_answer[bytes]=0;if(reg_case==2U)reg_answer[0]='X';}return TRUE;
}
static DWORD WINAPI reg_wait(HANDLE h,DWORD ms){(void)h;(void)ms;return WAIT_OBJECT_0;}
static BOOL WINAPI reg_read(HANDLE h,PINPUT_RECORD event,DWORD capacity,LPDWORD count)
{
 (void)h;(void)capacity;ZeroMemory(event,sizeof(*event));*count=1;event->EventType=KEY_EVENT;event->Event.KeyEvent.bKeyDown=TRUE;event->Event.KeyEvent.wRepeatCount=1;reg_reads++;
 if(reg_case==1U||reg_case==11U){event->Event.KeyEvent.wVirtualKeyCode=VK_ESCAPE;return TRUE;}
 if(reg_answer[reg_answer_pos])event->Event.KeyEvent.uChar.AsciiChar=reg_answer[reg_answer_pos++];else event->Event.KeyEvent.wVirtualKeyCode=VK_RETURN;
 if(reg_case==4U&&event->Event.KeyEvent.wVirtualKeyCode==VK_RETURN)native.command[5]^=1;
 if(reg_case==24U&&event->Event.KeyEvent.wVirtualKeyCode==VK_RETURN)root.configuration_commit.pending_commit.personal_native.scm_registration.dependencies[0].handle=NULL;
 if(reg_case==25U&&event->Event.KeyEvent.wVirtualKeyCode==VK_RETURN)root.configuration_commit.pending_commit.personal_native.scm_registration.package_captured=0;
 return TRUE;
}
static NTSTATUS WINAPI reg_rng(BCRYPT_ALG_HANDLE h,PUCHAR bytes,ULONG count,ULONG flags)
{
 ULONG i;(void)h;(void)flags;diag_rng_calls++;
 if(reg_case==10U||diag_case==1U||diag_case==35U)return (NTSTATUS)0xC0000001L;
 for(i=0;i<count;i++)bytes[i]=diag_case==2U?0:(BYTE)(i+1U);
 if(diag_case==23U)model_stopped=1;
 if(diag_case==24U)root.configuration_commit.pending_commit.personal_native.scm_registration.phase=4U;
 if(diag_case==25U)native.registration_view.service=L"Other";
 if(diag_case==26U)native.registration_view.recipient=&root;
 if(diag_case==27U)root.configuration_commit.pending_commit.personal_native.scm_registration.captured.image.identity.nFileIndexLow++;
 if(diag_case==28U)root.configuration_commit.pending_commit.personal_native.scm_registration.path[3]^=1;
 if(diag_case==29U)root.configuration_commit.pending_commit.personal_native.scm_registration.command[3]^=1;
 if(diag_case==30U)root.configuration_commit.pending_commit.personal_native.scm_registration.digest[0]^=1;
 if(diag_case==31U)root.configuration_commit.pending_commit.personal_native.scm_registration.package_captured=0;
 if(diag_case==32U)root.configuration_commit.pending_commit.personal_native.scm_image=NULL;
 if(diag_case==33U)root.configuration_commit.pending_commit.personal_native.install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;
 return 0;
}
static BOOL WINAPI reg_seek(HANDLE h,LARGE_INTEGER distance,PLARGE_INTEGER position,DWORD origin)
{(void)h;(void)distance;(void)position;(void)origin;reads=0;return TRUE;}
static PAPACC_RESULT reg_layout(PAPACC_CONFIGURATION_COMMIT *c)
{
 PAPACC_PERSONAL_INSTALL_PATHS *p=&c->pending_commit.personal_native.install_paths;p->phase=2U;p->mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;p->thread=GetCurrentThreadId();
 wcscpy_s(p->binary,MAX_PATH,reg_case==5U?L"C:\\WrongTarget":L"C:\\Lab Space");return 0;
}
static PAPACC_RESULT reg_layout_current(PAPACC_CONFIGURATION_COMMIT *c)
{
 generic_layout_checks++;
 /* Actual generic validator rejects BINARY_ONLY BEFORE any OS query. */
 if(c->pending_commit.personal_native.install_paths.mode!=PAPACC_PERSONAL_PATHS_BINARY_ONLY)return PAPACC_RESULT_INVALID_STATE;
 return papacc_personal_paths_current(&c->pending_commit.personal_native.install_paths,&c->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static PAPACC_RESULT reg_layout_binary_current(PAPACC_CONFIGURATION_COMMIT *c)
{
 PAPACC_PERSONAL_INSTALL_PATHS *p=&c->pending_commit.personal_native.install_paths;binary_layout_checks++;
 return (reg_case==13U&&reg_reads)||diag_case==3U||p->mode!=PAPACC_PERSONAL_PATHS_BINARY_ONLY||p->phase!=PAPACC_PERSONAL_PATHS_READY||
  p->thread!=GetCurrentThreadId()||p->parent[0]||p->data[0]||p->leaf[0]||wcscmp(p->binary,L"C:\\Lab Space")?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static PAPACC_RESULT reg_native_open(PAPACC_CONFIGURATION_COMMIT *c,const char *path)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&c->pending_commit.personal_native;(void)path;if(reg_case==6U)return PAPACC_RESULT_INVALID_STATE;
 if(diag_case==36U){n->diagnostic_first=6U;n->diagnostic_kind=1U;n->diagnostic_error=ERROR_ACCESS_DENIED;return PAPACC_RESULT_INVALID_STATE;}
 n->phase=1U;n->sid_text="MODEL_ELIGIBLE_OPERATOR_NOT_NATIVE_PROOF";n->parent.dwVolumeSerialNumber=42;n->parent.nFileIndexLow=200;
 n->descriptor=(PSECURITY_DESCRIPTOR)(ULONG_PTR)4;c->pending_commit.bootstrap_token=(HANDLE)(ULONG_PTR)12;c->pending_commit.bootstrap_parent=(HANDLE)(ULONG_PTR)11;return 0;
}
static PAPACC_RESULT reg_native_current(PAPACC_CONFIGURATION_COMMIT *c)
{if(diag_case==4U){c->pending_commit.personal_native.diagnostic_first=17U;c->pending_commit.personal_native.diagnostic_kind=1U;c->pending_commit.personal_native.diagnostic_error=ERROR_ACCESS_DENIED;return PAPACC_RESULT_INVALID_STATE;}
 return reg_case==3U&&reg_reads?PAPACC_RESULT_INVALID_STATE:c->pending_commit.personal_native.phase==1U?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;}
static PAPACC_RESULT reg_image_open(PAPACC_CONFIGURATION_COMMIT *c,const WCHAR *leaf)
{if(reg_case==9U||wcscmp(leaf,L"papacc_server.exe"))return PAPACC_RESULT_INVALID_STATE;c->pending_commit.personal_native.scm_image=(HANDLE)(ULONG_PTR)13;return 0;}
static PAPACC_RESULT reg_dependency_open(PAPACC_CONFIGURATION_COMMIT *c,unsigned index)
{if(index>=2U||(reg_case==14U&&index==0U)||(reg_case==15U&&index==1U))return PAPACC_RESULT_INVALID_STATE;c->pending_commit.personal_native.scm_registration.dependencies[index].handle=(HANDLE)(ULONG_PTR)(14U+index);return 0;}
static BOOL WINAPI reg_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION f)
{
 if((diag_case==5U&&h==(HANDLE)3)||(diag_case==6U&&h==(HANDLE)13)||(diag_case==14U&&diag_rng_calls&&h==(HANDLE)14)){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(!model_file(h,f))return FALSE;
 if((diag_case==7U&&h==(HANDLE)3)||(diag_case==8U&&h==(HANDLE)13))f->nFileIndexLow++;
 if(diag_case==9U&&h==(HANDLE)3)f->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;
 if(h==(HANDLE)14||h==(HANDLE)15){
  f->nFileIndexLow=300U+(DWORD)(ULONG_PTR)h;f->nFileSizeHigh=0;f->nFileSizeLow=3;
  if(reg_case==16U)f->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;
  if(reg_case==17U)f->dwVolumeSerialNumber++;
  if(reg_case==18U)f->nFileIndexLow=file_id.nFileIndexLow;
  if(reg_case==19U&&h==(HANDLE)15)f->nFileIndexLow=314;
  if(reg_case==20U&&reg_reads)f->nFileIndexLow++;
  if(reg_case==23U&&reg_reads)f->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;
  if(diag_case==15U&&diag_rng_calls&&h==(HANDLE)14)f->nFileIndexLow++;
 }
 return TRUE;
}
static BOOL WINAPI reg_read_file(HANDLE h,LPVOID out,DWORD cap,LPDWORD used,LPOVERLAPPED ov)
{BOOL ok;if((diag_case==12U&&h==(HANDLE)13)||(diag_case==18U&&diag_rng_calls&&h==(HANDLE)14)){SetLastError(ERROR_READ_FAULT);return FALSE;}
 ok=model_read_file(h,out,cap,used,ov);if(ok&&*used&&((reg_case==21U&&reg_reads&&(h==(HANDLE)14||h==(HANDLE)15))||
 (diag_case==13U&&h==(HANDLE)13)||(diag_case==19U&&diag_rng_calls&&h==(HANDLE)14)))((BYTE *)out)[0]^=1;return ok;}
static PAPACC_RESULT reg_native_close(PAPACC_CONFIGURATION_COMMIT *c)
{
 reg_native_closes++;if(reg_case==11U||diag_case==35U){SetLastError(ERROR_INVALID_HANDLE);return PAPACC_RESULT_INVALID_STATE;}
 c->pending_commit.personal_native.scm_registration.dependencies[0].handle=NULL;
 c->pending_commit.personal_native.scm_registration.dependencies[1].handle=NULL;
 c->pending_commit.bootstrap_parent=c->pending_commit.bootstrap_token=c->pending_commit.personal_native.scm_image=NULL;c->pending_commit.personal_native.phase=3U;return 0;
}
static int scm_descriptor(const void *d,PSID sid){(void)d;(void)sid;return reg_case!=7U;}
static int scm_acl_hash(PAPACC_PERSONAL_NATIVE_CONTEXT *n,HANDLE h,unsigned char out[32])
{(void)n;if(h==(HANDLE)13)diag_acl_calls++;
 if((diag_case==10U&&h==(HANDLE)13&&diag_acl_calls>1U)||(diag_case==16U&&diag_rng_calls&&h==(HANDLE)14))return 0;
 memset(out,(reg_case==8U&&reg_reads)||(reg_case==22U&&reg_reads&&(h==(HANDLE)14||h==(HANDLE)15))||
 (diag_case==11U&&h==(HANDLE)13&&diag_acl_calls>1U)||(diag_case==17U&&diag_rng_calls&&h==(HANDLE)14)?2:1,32);return 1;}
#define GetConsoleMode reg_mode
#define GetStdHandle reg_std
#define WriteConsoleA reg_write
#define ReadConsoleInputA reg_read
#define WaitForSingleObject reg_wait
#define BCryptGenRandom reg_rng
#define SetFilePointerEx reg_seek
#define papacc_configuration_personal_layout_new reg_layout
#define papacc_configuration_personal_layout_binary reg_layout
#undef GetFileInformationByHandle
#define GetFileInformationByHandle reg_file
#undef ReadFile
#define ReadFile reg_read_file
#define papacc_configuration_personal_layout_current reg_layout_current
#define papacc_configuration_personal_layout_binary_current reg_layout_binary_current
#define papacc_configuration_personal_native_open reg_native_open
#define papacc_configuration_personal_native_current reg_native_current
#define papacc_configuration_personal_native_close reg_native_close
#define papacc_configuration_scm_registration_image_open reg_image_open
#define papacc_configuration_scm_registration_dependency_open reg_dependency_open
#undef GetProcessMemoryInfo
#undef GetPerformanceInfo
#define GetProcessMemoryInfo reg_memory
#define GetPerformanceInfo reg_performance
#define GetProcessHandleCount reg_handles
#define GlobalMemoryStatusEx reg_global
#define QueryPerformanceFrequency reg_frequency
#define QueryPerformanceCounter reg_counter
#include "../apps/server/personal_confirmation_console_win32.inc"
#include "../apps/server/server_launcher_scm_registration_win32.inc"
#undef WaitForSingleObject
#define R(x) do{reg_checks++;if(!(x)){printf("FAIL registration scenario=%u resource_case=%u line=%u phase=%lu\n",reg_case,resource_case,(unsigned)__LINE__,(unsigned long)root.configuration_commit.pending_commit.personal_native.scm_registration.phase);return 1;}}while(0)
static int reg_reset(unsigned scenario)
{
 reset_model(0);ZeroMemory(&root,sizeof(root));state=0;reg_case=scenario;reg_reads=reg_answer_pos=reg_native_closes=0;reg_transcript[0]=reg_answer[0]=0;
 reg_scenarios++;
 resource_case=resource_calls=resource_handle_calls=0;resource_clock=0;
 diag_case=diag_rng_calls=diag_write_calls=diag_acl_calls=generic_layout_checks=binary_layout_checks=0;
 native.manager=NULL;native.image=NULL;native.image_ready=0;native.thread=GetCurrentThreadId();native.install_profile=PS_INSTALL_SCQ_CLOSED;model_closed=1U;
 file_id.dwVolumeSerialNumber=42;file_id.nFileSizeLow=3;
 if(ps_image_open(NULL))return 0;ps_registration_view_bind();(void)model_console_install(&native.console);InterlockedExchange(&cli_active,1);return 1;
}
static DWORD WINAPI reg_other_thread(LPVOID unused)
{(void)unused;return papacc_server_launcher_scm_registration_take(&native.registration_view);}
static int resource_cases(void)
{
 unsigned s,before;DWORD e;PAPACC_SCM_RESOURCE_REPORT report;PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT source;
 FILE *out=tmpfile(),*err=tmpfile();R(out&&err);
 for(s=0;s<=19U;s++){
  R(reg_reset(0));resource_case=s;
  e=(DWORD)papacc_server_launcher_scm_registration_present(&native.registration_view,out,err);
  R(papacc_server_launcher_scm_registration_resource_report(&native.registration_view,&report)==0);
  if(s==0U||s==12U||s==13U||s==14U||s==18U||s==19U){
   R(!e&&report.samples==3U&&!report.first_error&&report.peak_handles==(s==19U?124U:s==12U||s==14U?125U:s==13U?129U:80U));
   R(report.missing==63U&&report.minimum_commit==12582912U&&report.frequency==10000000U&&report.end>=report.begin);
   R(report.pre_missing==31U&&report.effect_missing==32U); /* effect evidence is not a pre-Create prerequisite */
   native.registration_active=1U;
   R(papacc_server_service_registration_source_status(&source)==(DWORD)(s==18U?ERROR_ACCESS_DENIED:ERROR_SUCCESS)&&source.resource_missing==63U&&source.resource_observations==3U);
   if(s==18U)R(report.minimum_physical==1U); /* nonzero is not headroom approval */
   R(ps_close()==0);R(root.configuration_commit.pending_commit.personal_native.scm_registration.resources.samples==5U);
  }else{
   R(e!=0&&report.first_error&&!creates&&state==PAPACC_LAUNCHER_FINISHED);
   before=resource_calls;R(scm_register_observe(&root.configuration_commit.pending_commit.personal_native.scm_registration,PAPACC_SCM_RESOURCE_CLOSED)!=0&&resource_calls==before);
   if(s==12U||s==13U||s==14U)R(report.first_error==ERROR_NOT_ENOUGH_QUOTA);
   if(s==12U||s==13U)R(report.samples==1U&&report.peak_handles==(s==12U?125U:129U)); /* queried, not UNKNOWN/zero */
   if(s==10U)R(report.first_error==ERROR_ARITHMETIC_OVERFLOW);
   if(s==16U||s==17U)R(report.first_error==ERROR_NOT_ENOUGH_MEMORY);
   R(ps_close()==0);
  }
 }
 R(reg_reset(0));R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)==0);native.registration_active=1U;resource_case=21U;
 R(ps_close()==0&&root.configuration_commit.pending_commit.personal_native.scm_registration.phase==5U&&state==PAPACC_LAUNCHER_FINISHED);
 R(papacc_server_launcher_scm_registration_resource_report(&native.registration_view,&report)==0&&report.first_error==ERROR_ACCESS_DENIED&&report.samples==3U&&(report.missing&PAPACC_SCM_RESOURCE_MISSING_OBSERVATION));
 /* A failed counter query is not a failed reference close, nor a retry grant. */
 R(fclose(out)==0&&fclose(err)==0);
 printf("PASS RESOURCE_OBSERVATIONS checks=%u scenarios=%u ALL_WINDOWS_COUNTERS_MODELED UNKNOWN_POLICY_DENIES ZERO_SCM_ENDPOINT_RNG_KSP\n",reg_checks,reg_scenarios);return 0;
}
static int first_cause_cases(void)
{
 static const struct {unsigned scenario;DWORD stage,kind;} cases[]={
  {1,SCM_D_RNG_API,SCM_DIAG_NTSTATUS},{2,SCM_D_RNG_ZERO,SCM_DIAG_POLICY},
  {3,SCM_D_LAYOUT_CURRENT,SCM_DIAG_RESULT},{4,SCM_D_NATIVE_CURRENT,SCM_DIAG_RESULT},
  {5,SCM_D_ORIGINAL_QUERY,SCM_DIAG_WIN32},{6,SCM_D_RELATIVE_QUERY,SCM_DIAG_WIN32},
  {7,SCM_D_ORIGINAL_IDENTITY,SCM_DIAG_POLICY},{8,SCM_D_RELATIVE_IDENTITY,SCM_DIAG_POLICY},
  {9,SCM_D_ORIGINAL_IDENTITY,SCM_DIAG_POLICY},{10,SCM_D_IMAGE_SD,SCM_DIAG_HELPER_UNKNOWN},
  {11,SCM_D_IMAGE_SD_CHANGED,SCM_DIAG_POLICY},{12,SCM_D_IMAGE_HASH,SCM_DIAG_HELPER_UNKNOWN},
  {13,SCM_D_IMAGE_HASH_CHANGED,SCM_DIAG_POLICY},{14,SCM_D_DEPENDENCY_QUERY,SCM_DIAG_WIN32},
  {15,SCM_D_DEPENDENCY_IDENTITY,SCM_DIAG_POLICY},{16,SCM_D_DEPENDENCY_SD,SCM_DIAG_HELPER_UNKNOWN},
  {17,SCM_D_DEPENDENCY_SD_CHANGED,SCM_DIAG_POLICY},{18,SCM_D_DEPENDENCY_HASH,SCM_DIAG_HELPER_UNKNOWN},
  {19,SCM_D_DEPENDENCY_HASH_CHANGED,SCM_DIAG_POLICY},{20,SCM_D_PLAN_FIRST_WRITE,SCM_DIAG_WIN32},
  {21,SCM_D_PLAN_FIRST_WRITE,SCM_DIAG_SHORT_OUTPUT},{22,SCM_D_PLAN_OUTPUT,SCM_DIAG_HELPER_UNKNOWN},
  {23,SCM_D_STOP,SCM_DIAG_POLICY},{24,SCM_D_CONSUMPTION_STATE,SCM_DIAG_POLICY},
  {25,SCM_D_SCOPE,SCM_DIAG_POLICY},{26,SCM_D_SCOPE,SCM_DIAG_POLICY},
  {27,SCM_D_CAPTURED_IDENTITY,SCM_DIAG_POLICY},{28,SCM_D_PATH,SCM_DIAG_POLICY},
  {29,SCM_D_COMMAND,SCM_DIAG_POLICY},{30,SCM_D_CAPTURED_DIGEST,SCM_DIAG_POLICY},
  {31,SCM_D_PACKAGE,SCM_DIAG_POLICY},{32,SCM_D_RELATIVE_REFERENCE,SCM_DIAG_POLICY},
  {33,SCM_D_LAYOUT_CURRENT,SCM_DIAG_RESULT},{35,SCM_D_RNG_API,SCM_DIAG_NTSTATUS},
  {36,SCM_D_PARENT_OPEN,SCM_DIAG_RESULT}
 };
 unsigned i;char line[512],expected[80];DWORD stage,value;FILE *out=tmpfile(),*err=tmpfile();
 PAPACC_SCM_REGISTRATION_ACT *a;R(out&&err);
 for(i=0;i<sizeof(cases)/sizeof(cases[0]);i++){
  R(reg_reset(0));diag_case=cases[i].scenario;rewind(err);SetLastError(0xdeadbeefUL);
  R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)!=0);
  a=&root.configuration_commit.pending_commit.personal_native.scm_registration;
  R(a->diagnostic_stage==cases[i].stage&&a->diagnostic_kind==cases[i].kind);
  R(!a->consumed&&!creates&&!changes&&!forbidden_managers&&!forbidden_starts&&!forbidden_deletes);
  if(cases[i].kind==SCM_DIAG_NTSTATUS)R(a->diagnostic_value==0xc0000001UL);
  if(cases[i].kind==SCM_DIAG_HELPER_UNKNOWN||cases[i].kind==SCM_DIAG_POLICY)R(a->diagnostic_value==0U);
  if(diag_case==5U||diag_case==6U||diag_case==14U)R(a->diagnostic_value==ERROR_ACCESS_DENIED);
  if(diag_case==20U)R(a->diagnostic_value==ERROR_WRITE_FAULT);
  if(diag_case==4U)R(a->diagnostic_native_stage==17U&&a->diagnostic_native_kind==1U&&a->diagnostic_native_error==ERROR_ACCESS_DENIED);
  if(diag_case==36U)R(a->diagnostic_native_stage==6U&&a->diagnostic_native_kind==1U&&a->diagnostic_native_error==ERROR_ACCESS_DENIED);
  if(diag_case==35U)R(state==PAPACC_LAUNCHER_RETAINED&&a->phase==6U);else R(state==PAPACC_LAUNCHER_FINISHED&&a->phase==5U);
  rewind(err);R(fgets(line,sizeof(line),err)!=NULL);sprintf_s(expected,sizeof(expected),"SCM_REGISTER_FIRST_CAUSE stage=%lu kind=%lu",(unsigned long)cases[i].stage,(unsigned long)cases[i].kind);
  R(strstr(line,expected)&&!strstr(line,"OWNER_SCM_REGISTER")&&!strstr(line,"ba7816bf"));
  stage=a->diagnostic_stage;value=a->diagnostic_value;
  (void)scm_register_note(a,SCM_D_PLAN_OUTPUT,SCM_DIAG_WIN32,ERROR_INVALID_HANDLE,2U);
  R(a->diagnostic_stage==stage&&a->diagnostic_value==value); /* first wins after close */
 }
 R(reg_reset(12));rewind(err);R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)!=0&&state==0);
 rewind(err);R(fgets(line,sizeof(line),err)!=NULL&&strstr(line,"act_phase=0"));
 R(fclose(out)==0&&fclose(err)==0);
 printf("PASS SCM_PREPLAN_FIRST_CAUSE cases=%u checks=%u MODELED_RNG_TOKEN_FILES_CONSOLE; NO_SCM_OR_REAL_RNG\n",(unsigned)(sizeof(cases)/sizeof(cases[0])+1U),reg_checks);return 0;
}
int main(int argc,char **argv)
{
 unsigned s,n;DWORD code;HANDLE thread;PAPACC_SERVICE_REGISTRATION_VIEW copy;PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT report;
 FILE *out=tmpfile(),*err=tmpfile();const char *v[]={"p","service","install","--scq-closed"};
 if(argc==2&&!strcmp(argv[1],"--first-cause")){if(out)fclose(out);if(err)fclose(err);return first_cause_cases();}
 if(argc==2&&!strcmp(argv[1],"--resource-observations")){if(out)fclose(out);if(err)fclose(err);return resource_cases();}
 R(argc==1);
 R(out&&err);
 for(s=14U;s<=25U;s++){
  R(reg_reset(s));R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)!=0);
  R(!creates&&!changes&&!root.configuration_commit.pending_commit.personal_native.scm_registration.consumed);
  R(!root.configuration_commit.pending_commit.personal_native.scm_registration.dependencies[0].handle&&
    !root.configuration_commit.pending_commit.personal_native.scm_registration.dependencies[1].handle);
 }
 for(s=0;s<=13U;s++){
  R(reg_reset(s));code=(DWORD)papacc_server_launcher_scm_registration_present(&native.registration_view,out,err);
  if(!s){R(!code&&state==PAPACC_LAUNCHER_ACTIVE);R(root.configuration_commit.pending_commit.personal_native.scm_registration.phase==3U);
   R(binary_layout_checks&&!generic_layout_checks);
   /* Reproduces the old mismatched validator with the real implementation:
    * BINARY_ONLY rejects before it can call a Windows known-folder API. */
   R(reg_layout_current(&root.configuration_commit)==PAPACC_RESULT_INVALID_STATE);
   R(strstr(reg_transcript,"SCM_REGISTER_CLOSED")&&strstr(reg_transcript,"DEMAND_START (3)")&&strstr(reg_transcript,"OWNER_SCM_REGISTER "));
   R(!strstr(reg_transcript,"OWNER_SCQ ")&&!strstr(reg_transcript,"OWNER_NEW_INSTALL "));
   R(strstr(reg_transcript,"libssl-3-x64.dll")&&strstr(reg_transcript,"libcrypto-3-x64.dll")&&
    strstr(reg_transcript,"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"));
   R(strstr(reg_transcript,"Image SHA256: ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"));
   native.registration_active=1U;
   R(papacc_server_service_registration_source_status(&report)==ERROR_SUCCESS&&report.reason==PS_REGISTRATION_LIMITED_ELIGIBLE&&!report.missing_source_fields&&report.resource_missing==63U);
   thread=CreateThread(NULL,0,reg_other_thread,NULL,0,NULL);R(thread!=NULL);R(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);R(GetExitCodeThread(thread,&code)&&code==ERROR_INVALID_STATE);R(CloseHandle(thread));
   R(!root.configuration_commit.pending_commit.personal_native.scm_registration.consumed);
   native.manager=(SC_HANDLE)(ULONG_PTR)1;R(ps_install_virtual()==0&&creates==1U&&changes==1U&&queries==1U&&!violations);
   R(root.configuration_commit.pending_commit.personal_native.scm_registration.consumed&&root.configuration_commit.pending_commit.personal_native.scm_registration.phase==4U);
   R(papacc_server_launcher_scm_registration_take(&native.registration_view)==ERROR_ALREADY_EXISTS);R(ps_install_virtual()==ERROR_INVALID_STATE&&creates==1U);
   R(ps_close()==0&&state==PAPACC_LAUNCHER_FINISHED&&reg_native_closes==1U);R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)!=0);
  }else{R(code!=0&&!creates&&!changes);if(s==11U)R(state==PAPACC_LAUNCHER_RETAINED);else R(state!=PAPACC_LAUNCHER_ACTIVE);}
 }
 for(s=0;s<7U;s++){
  R(reg_reset(0));R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)==0);native.registration_active=1U;copy=native.registration_view;
  if(s==0U)R(papacc_server_launcher_scm_registration_current(&copy)!=0);
  else {if(s==1U)native.registration_view.recipient=&copy;if(s==2U)native.registration_view.image.identity.nFileIndexLow++;
   if(s==3U)native.registration_view.account=L"LocalSystem";if(s==4U)native.registration_view.start_type=SERVICE_AUTO_START;
   if(s==5U)native.registration_view.sid_type=SERVICE_SID_TYPE_NONE;if(s==6U)native.registration_view.command=L"wrong";
   R(papacc_server_launcher_scm_registration_current(&native.registration_view)!=0);}
  R(!creates&&!root.configuration_commit.pending_commit.personal_native.scm_registration.consumed);R(ps_close()==0);
 }
 for(s=1U;s<=7U;s++){
  R(reg_reset(0));R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)==0);native.registration_active=1U;native.manager=(SC_HANDLE)(ULONG_PTR)1;
  mode=s==1U?2U:s==2U?3U:s==3U?35U:s==4U?16U:s==6U?1U:s==7U?15U:0U;if(s==5U)model_stopped=1;
  code=ps_install_virtual();if(s==4U)R(!code);else R(code!=0);
  if(s==5U)R(!creates&&!root.configuration_commit.pending_commit.personal_native.scm_registration.consumed);
  else R(creates==1U&&root.configuration_commit.pending_commit.personal_native.scm_registration.consumed);
  n=closes;if(s!=5U&&s!=6U){R(ps_close()!=0&&native.retained&&state==PAPACC_LAUNCHER_RETAINED);R(root.configuration_commit.pending_commit.personal_native.scm_registration.phase==6U&&reg_native_closes==0U);R(closes==n+(s==4U?1U:0U));}
  else R(ps_close()==0);
 }
 for(s=37U;s<=43U;s++){
  if(s>39U&&s<43U)continue;
  R(reg_reset(0));R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)==0);
  native.registration_active=1U;native.manager=(SC_HANDLE)(ULONG_PTR)1;mode=s;
  R(ps_install_virtual()!=0&&creates==1U&&native.install_disposition==3U);
  R(forbidden_starts==0&&forbidden_deletes==0);R(ps_close()!=0&&native.retained);
 }
 R(reg_reset(0));InterlockedExchange(&cli_active,1);R(papacc_server_service_cli(4,v,out,err,NULL)==2&&!creates&&state==0);InterlockedExchange(&cli_active,0);R(ps_close()==0);
 for(s=0;s<6U;s++){
  R(reg_reset(0));if(s==0U)native.registration_view.service=L"OtherService";if(s==1U)native.registration_view.account=L"LocalSystem";
  if(s==2U)native.registration_view.start_type=SERVICE_AUTO_START;if(s==3U)native.registration_view.sid_type=SERVICE_SID_TYPE_NONE;
  if(s==4U)native.registration_view.command=L"product host";if(s==5U)native.registration_view.recipient=NULL;
  R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)!=0&&state==0&&!reg_reads&&!creates);R(ps_close()==0);
 }
 /* A confirmed act alone never reaches SCM: the operational resource gate
  * remains independently closed, and the source closes without consumption. */
 R(reg_reset(0));native.image=NULL;native.image_ready=0;reads=0;
 InterlockedExchange(&cli_active,0);
 R(papacc_server_service_cli(4,v,out,err,NULL)==2&&forbidden_managers==1U&&!creates&&state==PAPACC_LAUNCHER_FINISHED);
 R(root.configuration_commit.pending_commit.personal_native.scm_registration.phase==5U&&!root.configuration_commit.pending_commit.personal_native.scm_registration.consumed);
 R(forbidden_managers==1U&&forbidden_starts==0&&forbidden_deletes==0);
 /* R0-R9: strict domain preserved, limited risk never clears opaque bits. */
 R(reg_reset(0));resource_case=12U;
 root.configuration_commit.pending_commit.personal_native.scm_registration.self=&root.configuration_commit.pending_commit.personal_native.scm_registration;
 root.configuration_commit.pending_commit.personal_native.scm_registration.thread=GetCurrentThreadId();
 R(scm_register_observe(&root.configuration_commit.pending_commit.personal_native.scm_registration,PAPACC_SCM_RESOURCE_ENTRY)==ERROR_NOT_ENOUGH_QUOTA);
 for(s=0;s<11U;s++){
  PAPACC_SCM_REGISTRATION_ACT *a;
  R(reg_reset(0));R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)==0);
  native.registration_active=1U;native.manager=(SC_HANDLE)(ULONG_PTR)1;a=&root.configuration_commit.pending_commit.personal_native.scm_registration;
  if(s==0U||s==1U||s==2U)resource_case=s==0U?22U:s==1U?23U:25U;
  if(s==3U)root.reserved_bytes=PAPACC_LAUNCHER_MANAGED_LIMIT+1U;
  if(s==4U)a->assurance=PAPACC_SCM_ASSURANCE_STRICT;
  if(s==5U)mode=40U; /* existing service; no adoption or consumption */
  if(s==6U)mode=41U; /* absence query unavailable */
  if(s==7U)model_stopped=1;
  if(s==8U){resource_case=13U;} if(s==9U)resource_case=24U; if(s==10U)resource_case=26U; /* 129 modeled kernel handles not a new ceiling */
  code=ps_install_virtual();
  if(s==2U||s==10U){R(code!=0&&creates==1U&&!changes&&a->consumed);R(ps_close()!=0&&native.retained&&state==PAPACC_LAUNCHER_RETAINED);}
  else if(s==8U||s==9U){R(!code&&creates==1U&&changes==1U&&a->effect_state[0]==2U&&a->effect_state[1]==2U);
   R(a->effect_end[0]>=a->effect_begin[0]&&a->effect_begin[1]>=a->effect_end[0]);
   R(papacc_server_launcher_scm_registration_effect(&native.registration_view,2U,0U,0)==ERROR_INVALID_STATE);
   R(papacc_server_launcher_scm_registration_effect(&native.registration_view,2U,1U,0)==ERROR_INVALID_STATE);
   R(ps_close()==0);
  }else{R(code!=0&&!creates&&!changes&&!a->consumed);R(ps_close()==0);}
  R(!forbidden_starts&&!forbidden_deletes);
 }
 /* Exact compiled floor boundary is sufficient only for the accepted risk,
  * never proof of opaque/native capacity. Complete CLI remains model-only. */
 R(reg_reset(0));native.image=NULL;native.image_ready=0;reads=0;mode=42U;InterlockedExchange(&cli_active,0);
 R(papacc_server_service_cli(4,v,out,err,NULL)==0&&creates==1U&&changes==1U&&state==PAPACC_LAUNCHER_FINISHED);
 R(!forbidden_starts&&!forbidden_deletes&&!violations); R(!ps_binding_ready());
 R(reg_reset(0));R(papacc_server_launcher_scm_registration_present(&native.registration_view,out,err)==0);native.registration_active=1U;
 root.configuration_commit.pending_commit.personal_native.scm_registration.assurance=PAPACC_SCM_ASSURANCE_STRICT;
 R(papacc_server_service_registration_source_status(&report)==ERROR_ACCESS_DENIED);R(ps_close()==0);
 R(fclose(out)==0&&fclose(err)==0);
 printf("PASS ORIGINAL_REGISTRATION checks=%u scenarios=%u ACTUAL_PRESENTER_CURRENT_TAKE_CLOSE MODELED_NATIVE_FACTS ZERO_SCM_KSP_ENDPOINT\n",reg_checks,reg_scenarios);return 0;
}
