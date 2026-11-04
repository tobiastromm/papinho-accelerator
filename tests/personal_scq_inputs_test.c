/* Actual token-predicate code + SID/SD APIs. Token/file/handle APIs modeled.
 * No file opens, content reads, ACL changes, confirmation or native effects. */
#include <wchar.h>
#define main preserved_token_predicate_main
#include "personal_token_predicate_test.c"
#undef main
#undef CloseHandle
static BOOL input_real_close(HANDLE h){return CloseHandle(h);}
#include <aclapi.h>
#include "personal_scq_inputs_win32.h"
#include "../apps/server/server_store_hash_win32.inc"
static unsigned input_fault,duplicates,input_closes,security_queries;
static int fail_free,mutated;static unsigned offsets[3],reader_fault;static volatile LONG *reader_stop;
static unsigned to_read,to_mode,to_fault,to_opens,to_violations,to_mutation,to_last_close;
static unsigned namespace_fault,namespace_calls,namespace_volume[5];static volatile LONG *namespace_stop;
static DWORD WINAPI input_chain_device(LPCWSTR drive,LPWSTR output,DWORD capacity)
{
 const WCHAR *target=namespace_fault==2U?L"\\??\\D:\\subst":namespace_fault==3U?L"\\Device\\FixtureVolume\\sub":
  namespace_fault==6U?L"\\Device\\OtherVolume":namespace_fault==9U?L"\\Device\\":namespace_fault==10U?L"\\Device\\..":L"\\Device\\FixtureVolume";
 namespace_calls++;
 if(wcscmp(drive,L"C:"))to_violations++;
 if(namespace_fault==1U){SetLastError(ERROR_ACCESS_DENIED);return 0;}
 if(namespace_fault==4U)return capacity+1U;
 if(namespace_fault==5U){wmemset(output,L'X',capacity);return capacity;}
 if(namespace_fault==8U&&namespace_stop)InterlockedExchange(namespace_stop,1);
 wcscpy_s(output,capacity,target);
 if(namespace_fault==11U){size_t n=wcslen(target);wcscpy_s(output+n+1U,capacity-n-1U,L"\\??\\OLD:");return (DWORD)(n+1U+wcslen(output+n+1U)+2U);}
 return (DWORD)wcslen(target)+2U;
}
#define QueryDosDeviceW input_chain_device
static HLOCAL WINAPI input_real_free(HLOCAL h){return LocalFree(h);}
static DWORD WINAPI input_type(HANDLE h){(void)h;return input_fault==1?FILE_TYPE_PIPE:FILE_TYPE_DISK;}
static BOOL WINAPI input_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION out)
{
 DWORD role=(DWORD)(ULONG_PTR)h%100U;
 if(to_mode&&(ULONG_PTR)h>=400U){
  unsigned index=(unsigned)(ULONG_PTR)h-400U;memset(out,0,sizeof(*out));out->dwVolumeSerialNumber=7;out->nNumberOfLinks=1;
  if(index<5U&&namespace_volume[index])out->dwVolumeSerialNumber=namespace_volume[index];
  out->nFileIndexLow=index==4U?12U:1000U+index;out->dwFileAttributes=index==4U?0:FILE_ATTRIBUTE_DIRECTORY;out->nFileSizeLow=index==4U?8U:0;
  if(to_fault==5U&&index==2U)out->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;
  if(to_fault==6U&&index==4U)out->nFileIndexLow++;
  if(to_mutation&&index==2U)out->nFileIndexLow++;
  return TRUE;
 }
 memset(out,0,sizeof(*out));out->dwVolumeSerialNumber=7;out->nFileIndexLow=role+10;out->nFileSizeLow=8;out->nNumberOfLinks=1;
 if(input_fault==2)out->dwFileAttributes=FILE_ATTRIBUTE_REPARSE_POINT;
 if(input_fault==3)out->nNumberOfLinks=2;
 if(input_fault==4&&(ULONG_PTR)h>=200U)out->nFileIndexLow++;
 if(reader_fault==101&&(ULONG_PTR)h>=300U)out->nFileIndexLow++;
 if(mutated==1)out->nFileIndexLow++;
 if(input_fault==5){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;
}
static BOOL WINAPI input_duplicate(HANDLE a,HANDLE h,HANDLE b,LPHANDLE out,DWORD rights,BOOL inherit,DWORD flags)
{
 (void)a;(void)b;duplicates++;
 if(rights!=(GENERIC_READ|READ_CONTROL)||inherit||flags)return FALSE;
 if(input_fault==6){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 *out=(HANDLE)((ULONG_PTR)h+100U);
 if(input_fault==7){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;
}
static DWORD WINAPI input_security(HANDLE h,SE_OBJECT_TYPE kind,SECURITY_INFORMATION info,PSID *owner,PSID *group,PACL *dacl,PACL *sacl,PSECURITY_DESCRIPTOR *sd)
{
 const char *text="O:S-1-5-21-1-2-3-1001G:BAD:P(A;;GA;;;S-1-5-21-1-2-3-1001)(A;;GA;;;SY)(A;;GA;;;BA)";
 (void)h;(void)owner;(void)group;(void)dacl;(void)sacl;security_queries++;
 if(kind!=SE_FILE_OBJECT||info!=(OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION))return ERROR_INVALID_PARAMETER;
 if(input_fault==8)return ERROR_ACCESS_DENIED;
 if(input_fault==9)text="O:SYG:BAD:P(A;;GA;;;SY)";
 if(input_fault==10)text="O:S-1-5-21-1-2-3-1001G:BAD:P(A;;GA;;;WD)";
 if(input_fault==11)text="O:S-1-5-21-1-2-3-1001G:BAD:(A;;GA;;;S-1-5-21-1-2-3-1001)";
 if(input_fault==12)text="O:S-1-5-21-1-2-3-1001G:BAD:P(D;;GA;;;WD)(A;;GA;;;S-1-5-21-1-2-3-1001)";
 if(mutated==2)text="O:S-1-5-21-1-2-3-1001G:BAD:P(A;;GR;;;S-1-5-21-1-2-3-1001)(A;;GA;;;SY)(A;;GA;;;BA)";
 if(to_mode){
  if((ULONG_PTR)h>=400U&&(ULONG_PTR)h<=403U){
   text="O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)";
   if(h==(HANDLE)400)text=to_fault==3U?"O:BAG:BAD:P(A;;FA;;;BA)(A;;GW;;;AU)":
    to_fault==26U?"O:BAG:BAD:P(A;;FA;;;BA)(A;;0x00040004;;;AU)":
    to_fault==21U?"O:BAG:BAD:P(A;;FA;;;BA)(A;;0x00010004;;;AU)":"O:BAG:BAD:P(A;;FA;;;BA)(A;;0x4;;;AU)";
   if(to_fault==4U&&h==(HANDLE)401)text="O:BAG:BAD:(A;;FA;;;BA)(A;;GRGX;;;AU)";
  }
  if(h==(HANDLE)202||h==(HANDLE)404){
   text="O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)";
   if(to_fault==9U)text="O:AUG:BAD:P(A;;FA;;;BA)(A;;GR;;;AU)";
   if(to_fault==11U)text="O:BAG:BAD:P(A;;FA;;;BA)(A;;GW;;;AU)";
   if(to_fault==12U)text="O:SYG:BAD:P(A;;FA;;;SY)";
   if(to_fault==13U)text="O:BAG:BAD:(A;;FA;;;BA)";
   if(to_fault==27U)text="O:BAG:BAD:P(A;OI;FA;;;BA)(A;;GRGX;;;AU)";
  }
  if((to_fault==7U&&h==(HANDLE)200)||(to_fault==8U&&h==(HANDLE)201))text="O:BAG:BAD:P(A;;FA;;;BA)";
 }
 if(to_read&&((ULONG_PTR)h==100U||(ULONG_PTR)h==101U||(ULONG_PTR)h==200U||(ULONG_PTR)h==201U)){
  text="O:S-1-5-21-1-2-3-1001G:SYD:P(A;;FA;;;S-1-5-21-1-2-3-1001)(A;;FA;;;SY)(A;;FA;;;BA)(A;;0x120089;;;S-1-5-80-1-2-3-4-5)";
  if(to_read==2U)text="O:S-1-5-21-1-2-3-1001G:SYD:P(A;;FA;;;S-1-5-21-1-2-3-1001)(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;S-1-5-80-1-2-3-4-5)";
  if(to_read==3U)text="O:S-1-5-21-1-2-3-1001G:SYD:P(A;;FA;;;S-1-5-21-1-2-3-1001)(A;;FA;;;SY)(A;;FA;;;BA)";
 }
 if(!ConvertStringSecurityDescriptorToSecurityDescriptorA(text,SDDL_REVISION_1,sd,NULL))return GetLastError();
 if(to_mode&&h==(HANDLE)400&&(to_fault==34U||(to_fault==35U&&security_queries>1U)))return ERROR_ACCESS_DENIED;
 return 0;
}
static BOOL WINAPI input_close(HANDLE h)
{input_closes++;
 if(to_mode&&(ULONG_PTR)h>=400U&&(ULONG_PTR)h<=404U){
  if((unsigned)(ULONG_PTR)h>=to_last_close)to_violations++;to_last_close=(unsigned)(ULONG_PTR)h;
  if(to_fault==18U&&h==(HANDLE)404){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}
 }
 if((reader_fault==106&&(ULONG_PTR)h>=300U)||input_fault==13){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static HLOCAL WINAPI input_free(HLOCAL h){if(fail_free)return h;return input_real_free(h);}
static HANDLE WINAPI input_reopen(HANDLE h,DWORD access,DWORD share,DWORD flags)
{unsigned k=(unsigned)(ULONG_PTR)h%100U;if(reader_fault==100){SetLastError(ERROR_SHARING_VIOLATION);return INVALID_HANDLE_VALUE;}
 if(access!=(GENERIC_READ|READ_CONTROL)||share!=FILE_SHARE_READ||flags!=FILE_FLAG_SEQUENTIAL_SCAN||k>=3U)return INVALID_HANDLE_VALUE;
 if(reader_fault>=120U&&reader_fault<=122U&&k==reader_fault-120U){SetLastError(ERROR_ACCESS_DENIED);return INVALID_HANDLE_VALUE;}
 offsets[k]=0;return (HANDLE)((ULONG_PTR)h+100U);}
static BOOL WINAPI input_read(HANDLE h,LPVOID out,DWORD cap,LPDWORD n,LPOVERLAPPED overlapped)
{unsigned k=(unsigned)(ULONG_PTR)h%100U;DWORD z;static const BYTE content[]="abcdefgh";
 if(overlapped||k>=3U||reader_fault==102){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 z=8U-offsets[k];if(z>cap)z=cap;if(reader_fault==103)z=0;if(reader_fault==111&&z>4U)z=4U;
 memcpy(out,content+offsets[k],z);offsets[k]+=z;*n=z;
 if(reader_fault==118)mutated=2;
 if(reader_fault==105&&reader_stop)InterlockedExchange(reader_stop,1);return TRUE;}
DWORD papacc_personal_runtime_current(PAPACC_PERSONAL_RUNTIME_IDENTITY *r)
{return r->phase==1&&r->thread==GetCurrentThreadId()&&reader_fault!=112?0:ERROR_INVALID_STATE;}
static BOOL WINAPI input_process_name(HANDLE h,DWORD flags,LPWSTR out,PDWORD n)
{static const WCHAR path[]=L"\\Device\\FixtureVolume\\image.exe";(void)h;if(flags!=PROCESS_NAME_NATIVE||reader_fault==113)return FALSE;
 wcscpy_s(out,*n,path);*n=(DWORD)wcslen(path);return TRUE;}
static DWORD WINAPI input_final_name(HANDLE h,LPWSTR out,DWORD n,DWORD flags)
{const WCHAR *path=reader_fault==114?L"\\Device\\FixtureVolume\\wrong.exe":L"\\Device\\FixtureVolume\\image.exe";
 (void)h;if(reader_fault==119&&reader_stop)InterlockedExchange(reader_stop,1);if(flags!=(FILE_NAME_NORMALIZED|VOLUME_NAME_NT))return 0;wcscpy_s(out,n,path);return reader_fault==115?n:(DWORD)wcslen(path);}
#define QueryFullProcessImageNameW input_process_name
#define GetFinalPathNameByHandleW input_final_name
static NTSTATUS WINAPI input_algorithm(BCRYPT_ALG_HANDLE *a,LPCWSTR n,LPCWSTR provider,ULONG flags)
{if(reader_fault==117)return (NTSTATUS)0xc0000001L;return BCryptOpenAlgorithmProvider(a,n,provider,flags);}
#define BCryptOpenAlgorithmProvider input_algorithm
static NTSTATUS WINAPI input_destroy(BCRYPT_HASH_HANDLE h)
{if(reader_fault==116)return (NTSTATUS)0xc0000001L;return BCryptDestroyHash(h);}
#define BCryptDestroyHash input_destroy
#define ReOpenFile input_reopen
#define ReadFile input_read
#define GetFileType input_type
#define GetFileInformationByHandle input_file
#define DuplicateHandle input_duplicate
#define GetSecurityInfo input_security
#undef CloseHandle
#define CloseHandle input_close
#define LocalFree input_free
static BOOL WINAPI input_chain_flags(HANDLE h,LPDWORD flags){(void)h;*flags=0;return TRUE;}
static BOOL WINAPI input_chain_count(HANDLE h,LPDWORD count){(void)h;if(to_fault==33U){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 *count=to_fault==17U?124U:to_fault==32U?123U+duplicates+to_opens:to_fault==37U?116U+duplicates+to_opens:100U;return TRUE;}
static NTSTATUS NTAPI input_chain_open(PHANDLE h,ACCESS_MASK rights,POBJECT_ATTRIBUTES a,PIO_STATUS_BLOCK io,PLARGE_INTEGER size,ULONG attributes,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_size)
{
 unsigned index=to_opens++;int directory=!!(options&1U);(void)size;(void)attributes;(void)ea;(void)ea_size;
 if(!to_mode||index>4U||!a||!a->ObjectName||share!=FILE_SHARE_READ||disposition!=1U||!(a->Attributes&0x1000U)||
  rights!=(directory?FILE_READ_ATTRIBUTES|FILE_TRAVERSE|READ_CONTROL|SYNCHRONIZE:PAPACC_SCQ_FILE_READ_RIGHTS)||
  (index==0U?a->RootDirectory!=NULL:a->RootDirectory!=(HANDLE)(ULONG_PTR)(399U+index)))to_violations++;
 if(index==0U){
  const WCHAR *expected=namespace_calls?(namespace_fault==6U?L"\\Device\\OtherVolume\\":L"\\Device\\FixtureVolume\\"):L"\\??\\C:\\";
  if(wcscmp(a->ObjectName->Buffer,expected))to_violations++;
 }
 if(namespace_fault==7U){*h=NULL;return (NTSTATUS)0xc000050bL;}
 if(index<5U)namespace_volume[index]=namespace_fault==6U?8U:7U;
 *h=(HANDLE)(ULONG_PTR)(400U+index);io->Information=1U;io->Status=to_fault==15U?(NTSTATUS)0x103L:0;
 return io->Status;
}
static HMODULE WINAPI input_chain_module(LPCWSTR name){(void)name;return (HMODULE)1;}
static NTSTATUS NTAPI input_chain_sd(HANDLE file,SECURITY_INFORMATION mask,PSECURITY_DESCRIPTOR out,ULONG capacity,PULONG needed)
{
 PSECURITY_DESCRIPTOR sd=NULL;DWORD e,n;
 if(mask!=(OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION))return (NTSTATUS)0xc000000dL;
 e=input_security(file,SE_FILE_OBJECT,mask,NULL,NULL,NULL,NULL,&sd);
 if(e||!sd){if(sd)input_real_free(sd);return (NTSTATUS)0xc0000022L;}
 n=GetSecurityDescriptorLength(sd);*needed=n;
 if(!out||capacity<n){input_real_free(sd);return (NTSTATUS)0xc0000023L;}
 memcpy(out,sd,n);input_real_free(sd);return 0;
}
static FARPROC WINAPI input_chain_address(HMODULE module,LPCSTR name)
{
 NTSTATUS (NTAPI *f)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG)=input_chain_open;FARPROC p;
 if(!strcmp(name,"NtQuerySecurityObject")){
  NTSTATUS (NTAPI *q)(HANDLE,SECURITY_INFORMATION,PSECURITY_DESCRIPTOR,ULONG,PULONG)=input_chain_sd;
  memcpy(&p,&q,sizeof(p));return p;
 }
 (void)module;if(strcmp(name,"NtCreateFile")||to_fault==16U)return NULL;memcpy(&p,&f,sizeof(p));return p;
}
static DWORD input_chain_paths(const PAPACC_PERSONAL_INSTALL_PATHS *p,volatile LONG *stop)
{return !p||*stop||p->phase!=PAPACC_PERSONAL_PATHS_READY||wcscmp(p->binary,L"C:\\Program Files\\Papinho\\Accelerator")?ERROR_ACCESS_DENIED:0;}
#undef GetHandleInformation
#define GetHandleInformation input_chain_flags
#undef GetProcessHandleCount
#define GetProcessHandleCount input_chain_count
#undef GetModuleHandleW
#define GetModuleHandleW input_chain_module
#undef GetProcAddress
#define GetProcAddress input_chain_address
#define papacc_personal_paths_current input_chain_paths
#define papacc_personal_paths_binary_current input_chain_paths
#include "../apps/server/personal_scq_inputs_win32.inc"
#undef LocalFree
#include "scq_profile_fixture.h"
static int input_transport_suite(void)
{
 static struct {PAPACC_PERSONAL_SCQ_INPUTS c;PAPACC_PERSONAL_INSTALL_PATHS paths;volatile LONG stop;} fixtures[36];
 PAPACC_PERSONAL_SCQ_INPUTS *c;PAPACC_PERSONAL_INSTALL_PATHS *paths;volatile LONG *stop;PAPACC_SCQ_PLAN_DATA p;
 PSID actor=NULL;HANDLE refs[3]={(HANDLE)100,(HANDLE)101,(HANDLE)102};
 unsigned scenario,k,before=checks;DWORD e;
 CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&actor));
 for(scenario=0;scenario<36U;scenario++){
  c=&fixtures[scenario].c;paths=&fixtures[scenario].paths;stop=&fixtures[scenario].stop;to_mode=1;to_fault=scenario;to_mutation=to_opens=to_violations=0;
  input_fault=mutated=reader_fault=fail_free=0;duplicates=security_queries=0;*stop=0;to_last_close=405U;CHECK(scq_fixture_plan(&p));
  p.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;if(scenario!=1U&&scenario!=2U)memset(p.key,0,sizeof(p.key));
  for(k=0;k<3;k++){p.refs[k].volume=7;p.refs[k].low=10U+k;p.refs[k].size_low=8U;p.refs[k].attributes=0;}
  paths->phase=PAPACC_PERSONAL_PATHS_READY;paths->thread=GetCurrentThreadId();paths->mode=PAPACC_PERSONAL_PATHS_NEW;
  wcscpy_s(paths->binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
  if(scenario==1U||scenario==2U){p.purpose=PAPACC_SCQ_PURPOSE_READ;
   p.runtime.image[0]=p.refs[2].volume;p.runtime.image[1]=p.refs[2].high;p.runtime.image[2]=p.refs[2].low;
   CHECK(scqp_static(&p)); /* Valid READ, not rejection caused by a missing key. */
  }
  if(scenario==14U)p.key[0]='x';if(scenario==20U)*stop=1;
  if(scenario==28U)p.administrator.sid[8]^=1U;
  if(scenario==29U){p.valid_until[0]=1;p.valid_until[1]=0;}
  e=scenario==1U?personal_scq_inputs_capture(c,refs,actor,stop):
   personal_scq_inputs_capture_transport(c,refs,actor,&p,scenario==10U?NULL:paths,paths,stop);
  CHECK(!to_violations);
  if(scenario==32U)CHECK(duplicates==1U&&!to_opens); /* guard checked BETWEEN copies */
  if(scenario==33U)CHECK(!duplicates&&!to_opens);
  if(scenario==0U||scenario==18U||scenario==19U||(scenario>=22U&&scenario<=25U)||scenario==30U||scenario==31U){
   CHECK(!e&&c->phase==1U&&c->image_chain.phase==2U&&c->image_chain.count==4U);
   CHECK(!personal_scq_inputs_check(c,actor,stop,0));
   if(scenario==19U){to_mutation=1;CHECK(personal_scq_inputs_check(c,actor,stop,0)!=0);}
   if(scenario==22U){*stop=1;CHECK(personal_scq_inputs_check(c,actor,stop,0)!=0);}
   if(scenario==23U){paths->binary[3]=L'X';CHECK(personal_scq_inputs_check(c,actor,stop,0)!=0);}
   if(scenario==24U){c->image_chain.plan.purpose=PAPACC_SCQ_PURPOSE_READ;CHECK(personal_scq_inputs_check(c,actor,stop,0)!=0);}
   if(scenario==25U){c->image_chain.self=NULL;CHECK(personal_scq_inputs_check(c,actor,stop,0)!=0);}
   if(scenario==30U){to_fault=11U;CHECK(personal_scq_inputs_check(c,actor,stop,0)!=0);}
   if(scenario==31U){PAPACC_SCQ_REF_DATA changed[3];memcpy(changed,p.refs,sizeof(changed));changed[2].digest[0]^=1U;
    CHECK(personal_scq_inputs_contents(c,actor,changed,GetTickCount64()+10000U,stop)!=0);}
   if(scenario==18U){CHECK(personal_scq_inputs_close(c)!=0&&c->phase==4U&&c->image_chain.phase==4U);CHECK(personal_scq_inputs_close(c)!=0);continue;}
   if(scenario==25U){CHECK(personal_scq_inputs_close(c)!=0&&c->phase==4U);continue;}
  }else CHECK(e!=0);
  if(scenario==15U||scenario==34U||scenario==35U){CHECK(c->phase==4U&&c->image_chain.phase==4U);CHECK(personal_scq_inputs_close(c)!=0);}
  else CHECK(!personal_scq_inputs_close(c));
  CHECK(!to_violations);
 }
 to_mode=0;CHECK(!input_real_free(actor));
 printf("PASS SCQ_TO_IMAGE_OWNER cases=36 checks=%u PROTECTED_ORIGINAL_CHAIN_REQUIRED REQUEST_DECISION_READ_STRICT ALL_OPENS_MODELED NO_CNG_RNG_ENDPOINT_OR_SCM\n",checks-before);return 0;
}
static int input_existing_suite(void)
{
 static struct {PAPACC_PERSONAL_SCQ_INPUTS c;PAPACC_PERSONAL_INSTALL_PATHS paths;volatile LONG stop;} f[29];
 PAPACC_SCQ_PLAN_DATA p;PSID actor=NULL;unsigned i,k,before=checks;DWORD e;BYTE sealed[32];
 CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&actor));
 for(i=0;i<29U;i++){
  PAPACC_SCQ_IMAGE_CHAIN *g=&f[i].c.image_chain;PAPACC_PERSONAL_INSTALL_PATHS *paths=&f[i].paths;
  CHECK(scq_fixture_plan(&p));p.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;memset(p.key,0,sizeof(p.key));
  for(k=0;k<3U;k++){p.refs[k].volume=7;p.refs[k].low=10U+k;p.refs[k].size_low=8;p.refs[k].attributes=0;}
  if(i==10U||i==11U)p=f[0].c.image_chain.plan;
  paths->phase=PAPACC_PERSONAL_PATHS_READY;paths->thread=GetCurrentThreadId();paths->mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;
  paths->original_chain=g;paths->selected_role=(i==10U||i==11U)?2U:1U;paths->selected.architecture=0x8664U;
  memset(paths->selected.provenance,42,32);wcscpy_s(paths->binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
  paths->selected.identity[0]=paths->selected.identity[3]=paths->selected.identity[6]=7;
  paths->selected.identity[2]=1001;paths->selected.identity[5]=1003;paths->selected.identity[8]=12;
  to_mode=1;to_fault=to_mutation=to_opens=to_violations=0;input_fault=mutated=reader_fault=fail_free=security_queries=0;to_last_close=405;
  namespace_fault=i>=18U?i-17U:0U;namespace_calls=0;namespace_stop=&f[i].stop;
  if(i==1U)paths->selected.identity[2]++;if(i==2U)to_fault=4;if(i==3U)to_fault=5;
  if(i==4U)paths->original_chain=NULL;if(i==5U)paths->selected.architecture=0x14c;
  if(i==6U)paths->parent[0]='X';if(i==7U)f[i].stop=1;
  if(i==8U)p.purpose=PAPACC_SCQ_PURPOSE_READ;if(i==9U)to_fault=33;
  if(i==11U)p.target[0]^=1;
  e=scqi_chain_capture(g,paths,paths,&p,&(BY_HANDLE_FILE_INFORMATION){.dwVolumeSerialNumber=7,.nFileIndexLow=12,.nNumberOfLinks=1,.nFileSizeLow=8},actor,&f[i].stop);
  CHECK(!to_violations);
  if(i==0U||i==10U||(i>=12U&&i<=17U)||i==28U){
   CHECK(!e&&g->phase==2U);CHECK(!scqi_chain_current(g,actor,&f[i].stop));
   CHECK(!scqi_target_digest(g,&g->plan,sealed)&&!memcmp(sealed,g->plan.target,32));
   CHECK(namespace_calls==1U); /* Once, before guard, never a name-based re-open. */
   if(i==17U){namespace_fault=6U;CHECK(!scqi_chain_current(g,actor,&f[i].stop));namespace_fault=0U;CHECK(scqi_chain_capture(g,paths,paths,&p,&g->identity[g->count],actor,&f[i].stop)==ERROR_INVALID_STATE);}
   if(i==12U){g->plan.request[0]^=1;CHECK(scqi_chain_current(g,actor,&f[i].stop)!=0);}
   if(i==13U){to_mutation=1;CHECK(scqi_chain_current(g,actor,&f[i].stop)!=0);}
   if(i==14U){paths->selected.provenance[0]^=1;CHECK(scqi_chain_current(g,actor,&f[i].stop)!=0);}
   if(i==15U){to_fault=18;CHECK(personal_scq_inputs_close(&f[i].c)!=0&&g->phase==4U&&f[i].c.phase==4U);CHECK(personal_scq_inputs_close(&f[i].c)!=0);continue;}
   if(i==16U){
    HANDLE refs[3]={(HANDLE)100,(HANDLE)101,(HANDLE)102};
    p=g->plan;duplicates=0;to_fault=37;
    CHECK(!personal_scq_inputs_capture_transport(&f[i].c,refs,actor,&p,paths,paths,&f[i].stop));
    CHECK(duplicates==3U&&to_opens==5U); /* raw3 + chain5 + copies3 =>124, B=113 */
    for(k=0;k<3U;k++){
     CHECK(papacc_store_hash(&f[i].c.content_hash,(BYTE *)f[i].c.descriptor[k],f[i].c.descriptor_bytes[k],p.refs[k].descriptor));
     memcpy(g->plan.refs[k].descriptor,p.refs[k].descriptor,32);
    }
    e=personal_scq_inputs_contents(&f[i].c,actor,p.refs,GetTickCount64()+10000U,&f[i].stop);
    CHECK(e!=ERROR_NOT_ENOUGH_QUOTA); /* old 124 denied; explicit P1 keeps originals and all readers */
    CHECK(f[i].c.reader[0]&&f[i].c.reader[1]&&f[i].c.reader[2]);
   }
  }else {CHECK(e!=0);if(i>=18U&&i<=22U)CHECK(!to_opens);if(i==25U)CHECK(!to_opens&&f[i].stop);}
  CHECK(!personal_scq_inputs_close(&f[i].c)); /* pre-input acquisition must also close/retain */
 }
 to_mode=0;CHECK(!input_real_free(actor));
 namespace_fault=namespace_calls=0;namespace_stop=NULL;
 printf("PASS P0_P7_EXISTING_TARGET cases=29 checks=%u SHARED_NATIVE_RESOLVER RETAINED_OBJECT_NOT_NAMESPACE_STRING ALL_OPENS_MODELED NO_SELECTION_AUTHORITY_NO_ENDPOINT_NO_SCM\n",checks-before);return 0;
}
static int input_machine_read_suite(void)
{
 static struct {PAPACC_PERSONAL_SCQ_INPUTS c;PAPACC_PERSONAL_INSTALL_PATHS paths;volatile LONG stop;} f[12];
 PAPACC_SCQ_PLAN_DATA p;PSID actor=NULL;unsigned i,k,before=checks;DWORD e;
 HANDLE refs[3]={(HANDLE)100,(HANDLE)101,(HANDLE)102};
 CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&actor));
 for(i=0;i<12U;i++){
  PAPACC_SCQ_IMAGE_CHAIN *g=&f[i].c.image_chain;PAPACC_PERSONAL_INSTALL_PATHS *paths=&f[i].paths;
  CHECK(scq_fixture_plan(&p));for(k=0;k<3U;k++){p.refs[k].volume=7;p.refs[k].low=10U+k;p.refs[k].size_low=8;p.refs[k].attributes=0;}
  paths->phase=PAPACC_PERSONAL_PATHS_READY;paths->thread=GetCurrentThreadId();paths->mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;paths->read_diagnostic=1U;memset(paths->read_public,7,32);
  paths->original_chain=g;paths->selected_role=1U;paths->selected.architecture=0x8664U;
  memset(paths->selected.provenance,42,32);wcscpy_s(paths->binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
  paths->selected.identity[0]=paths->selected.identity[3]=paths->selected.identity[6]=7;
  paths->selected.identity[2]=1001;paths->selected.identity[5]=1003;paths->selected.identity[8]=12;
  to_mode=to_read=1;to_fault=to_mutation=to_opens=to_violations=0;input_fault=mutated=reader_fault=fail_free=security_queries=duplicates=0;to_last_close=405;
  namespace_fault=namespace_calls=0;namespace_stop=&f[i].stop;
  if(i==1U)paths->read_diagnostic=0;if(i==2U){p.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;memset(p.key,0,sizeof(p.key));}
  if(i==3U)paths->read_diagnostic=2U;if(i==4U)paths->selected.identity[2]++;if(i==7U)f[i].stop=1;
  if(i==8U)paths->mode=PAPACC_PERSONAL_PATHS_NEW;if(i==9U)paths->original_chain=NULL;
  e=scqi_chain_capture(g,paths,paths,&p,&(BY_HANDLE_FILE_INFORMATION){.dwVolumeSerialNumber=7,.nFileIndexLow=12,.nNumberOfLinks=1,.nFileSizeLow=8},actor,&f[i].stop);
  if(i==1U||i==2U||i==3U||i==4U||i==7U||i==8U||i==9U){CHECK(e!=0&&!duplicates);}
  else {
   CHECK(!e&&g->phase==2U);CHECK(!scqi_chain_current(g,actor,&f[i].stop));
   if(i==5U){to_mutation=1;CHECK(scqi_chain_current(g,actor,&f[i].stop)!=0);}
   else if(i==6U){paths->read_diagnostic=0;CHECK(scqi_chain_current(g,actor,&f[i].stop)!=0);}
   else {
    p=g->plan;if(i==10U)to_read=2U;if(i==11U)to_read=3U;
    CHECK(personal_scq_inputs_capture_transport(&f[i].c,refs,actor,&p,paths,paths,&f[i].stop)!=0&&!duplicates);
    e=personal_scq_inputs_capture_machine_read(&f[i].c,refs,actor,&p,paths,paths,&f[i].stop);
    if(i>=10U)CHECK(e!=0&&!f[i].c.content_verified);
    else {CHECK(!e&&duplicates==3U&&to_opens==5U);CHECK(!personal_scq_inputs_check(&f[i].c,actor,&f[i].stop,0));}
   }
  }
  CHECK(!to_violations);CHECK(!personal_scq_inputs_close(&f[i].c));
 }
 to_read=to_mode=0;namespace_fault=namespace_calls=0;namespace_stop=NULL;CHECK(!input_real_free(actor));
 printf("PASS K3_K6_MACHINE_READ_ORIGINAL_CHAIN cases=12 checks=%u TYPED_ONLY EXACT_FOUR_ACES RETAINED_REFERENCES NO_OPERATIONAL_AUTHORITY ALL_FILE_APIS_MODELED\n",checks-before);return 0;
}
static PAPACC_PERSONAL_SCQ_INPUTS foreign_inputs,retained_contents;
static DWORD WINAPI input_foreign(LPVOID p)
{PAPACC_SCQ_REF_DATA expected[3];volatile LONG stop=0;DWORD e;(void)p;memset(expected,0,sizeof(expected));
 e=personal_scq_inputs_contents(&foreign_inputs,fixture_sid,expected,GetTickCount64()+10000,&stop);
 return e==ERROR_INVALID_STATE&&personal_scq_inputs_close(&foreign_inputs)==ERROR_INVALID_STATE?ERROR_INVALID_STATE:0;}

int main(int argc,char **argv)
{
 if(argc==2&&!strcmp(argv[1],"--existing-target")){int e=input_existing_suite();return e?e:input_machine_read_suite();}
 if(argc==2&&!strcmp(argv[1],"--transport-owner"))return input_transport_suite();
 if(argc==2&&!strcmp(argv[1],"--staged-owner-only")){
  PSID actor=NULL;PSECURITY_DESCRIPTOR staged=NULL;
  CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&actor));
  CHECK(ConvertStringSecurityDescriptorToSecurityDescriptorA("O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)",SDDL_REVISION_1,&staged,NULL));
  CHECK(!personal_scq_inputs_protected(staged,actor)); /* Ordinary guard remains strict. */
  CHECK(!input_real_free(staged));CHECK(!input_real_free(actor));
  puts("PASS STAGED_OWNER_BOUNDARY ORDINARY_GUARD_REJECTS_STAGER_O_BA SD_IN_MEMORY_ONLY NO_FILE_NO_CNG_NO_TOKEN_NO_EFFECTS ORDINARY_POLICY_UNCHANGED");
  return 0;
 }
 PAPACC_PERSONAL_SCQ_INPUTS c;HANDLE refs[3]={(HANDLE)100,(HANDLE)101,(HANDLE)102};unsigned i,before;volatile LONG stop=0;
 DWORD e,result;HANDLE worker;
 CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&fixture_sid));
 for(i=0;i<=15;i++){
  memset(&c,0,sizeof(c));memset(&packet,0,sizeof(packet));packet.personal_native.phase=1;fault=0;
  input_fault=i;fail_free=0;mutated=0;duplicates=input_closes=security_queries=0;stop=i==14;
  CHECK(personal_token(&packet,1));
  if(i==15)refs[1]=refs[0];else refs[1]=(HANDLE)101;
  e=personal_scq_inputs_capture(&c,refs,fixture_sid,&stop);
  if(!i||i==13){CHECK(!e&&duplicates==3&&security_queries==3);CHECK(!personal_scq_inputs_check(&c,fixture_sid,&stop,0));
   CHECK(personal_scq_inputs_capture(&c,refs,fixture_sid,&stop)==ERROR_INVALID_STATE);}
  else CHECK(e!=0&&c.first);
  before=input_closes;input_fault=0;
  if(i==7){CHECK(c.phase==4&&c.file[0]);CHECK(personal_scq_inputs_close(&c)!=0&&input_closes==before);}
  else if(i==13){input_fault=13;CHECK(personal_scq_inputs_close(&c)!=0&&c.phase==4);before=input_closes;input_fault=0;CHECK(personal_scq_inputs_close(&c)!=0&&input_closes==before);}
  else{CHECK(!personal_scq_inputs_close(&c)&&c.phase==3);before=input_closes;CHECK(!personal_scq_inputs_close(&c)&&input_closes==before);}
 }
 for(i=0;i<4;i++){
  memset(&c,0,sizeof(c));input_fault=mutated=fail_free=0;stop=0;refs[1]=(HANDLE)101;
  CHECK(!personal_scq_inputs_capture(&c,refs,fixture_sid,&stop));mutated=i<2?(int)i+1:0;stop=i==2;fail_free=i==3;
  CHECK(personal_scq_inputs_check(&c,fixture_sid,&stop,0)!=0);before=input_closes;
  {int saved=fail_free;fail_free=0;mutated=0;stop=0;CHECK(personal_scq_inputs_check(&c,fixture_sid,&stop,0)==ERROR_INVALID_STATE);fail_free=saved;}
  if(i==3){CHECK(c.phase==4&&c.checking);fail_free=0;CHECK(personal_scq_inputs_close(&c)!=0&&input_closes==before);}
  else CHECK(!personal_scq_inputs_close(&c));
 }
 {PAPACC_SCQ_REF_DATA expected[3];PAPACC_PERSONAL_RUNTIME_IDENTITY runtime;PAPACC_SCQ_RUNTIME_OBSERVATION observation;PAPACC_STORE_HASH h;unsigned k,f;BYTE content[]="abcdefgh";ULONGLONG deadline;
  for(f=0;f<16;f++){
   static const unsigned failures[]={0,100,101,102,103,104,105,106,109,110,111,117,118,120,121,122};
   memset(&c,0,sizeof(c));memset(&h,0,sizeof(h));memset(expected,0,sizeof(expected));input_fault=mutated=fail_free=0;reader_fault=0;reader_stop=&stop;stop=0;
   refs[0]=(HANDLE)100;refs[1]=(HANDLE)101;refs[2]=(HANDLE)102;CHECK(!personal_scq_inputs_capture(&c,refs,fixture_sid,&stop));
   for(k=0;k<3;k++){expected[k].role=k+1;expected[k].rights=PAPACC_SCQ_FILE_READ_RIGHTS;expected[k].volume=c.identity[k].dwVolumeSerialNumber;
    expected[k].high=c.identity[k].nFileIndexHigh;expected[k].low=c.identity[k].nFileIndexLow;expected[k].size_low=8;expected[k].links=1;expected[k].attributes=c.identity[k].dwFileAttributes;
    CHECK(papacc_store_hash(&h,content,8,expected[k].digest));CHECK(papacc_store_hash(&h,(BYTE *)c.descriptor[k],c.descriptor_bytes[k],expected[k].descriptor));}
   reader_fault=failures[f];if(reader_fault==104)expected[1].digest[0]^=1;if(reader_fault==110)expected[2].descriptor[0]^=1;
   deadline=reader_fault==109?1:GetTickCount64()+10000;
   e=personal_scq_inputs_contents(&c,fixture_sid,expected,deadline,&stop);
   if(!reader_fault||reader_fault==106||reader_fault==111){CHECK(!e&&c.content_verified);CHECK(c.reader[0]&&c.reader[1]&&c.reader[2]);CHECK(!memcmp(c.digest[2],expected[2].digest,32));
    CHECK(personal_scq_inputs_contents(&c,fixture_sid,expected,deadline,&stop)!=0);}
   else CHECK(e&&!c.content_verified);
   if(reader_fault>=120U&&reader_fault<=122U){unsigned role=reader_fault-120U;
    CHECK(e==ERROR_ACCESS_DENIED&&c.first==15U&&c.error==ERROR_ACCESS_DENIED);
    for(k=0;k<3U;k++)CHECK((c.reader[k]!=NULL)==(k<role));
   }
   if(reader_fault==118U)CHECK(e==ERROR_ACCESS_DENIED&&c.first==5U);
   if(!reader_fault){unsigned f2;for(f2=0;f2<6;f2++){
    memset(&runtime,0,sizeof(runtime));runtime.phase=1;runtime.thread=GetCurrentThreadId();runtime.profile=PAPACC_PERSONAL_RUNTIME_VIRTUAL;runtime.pid=77;
    memset(&observation,0,sizeof(observation));observation.known=15;observation.peer.pid=77;reader_fault=f2==5?119:f2?111+f2:0;
    e=personal_scq_inputs_image_data(&c,fixture_sid,&runtime,&observation,&stop);
    if(!f2)CHECK(!e&&observation.known==31&&observation.peer.image[2]==c.identity[2].nFileIndexLow&&!memcmp(observation.peer.image_digest,c.digest[2],32));
    else CHECK(e&&!observation.known&&!observation.peer.image[0]);
   }reader_fault=0;stop=0;}
   before=input_closes;
   if(reader_fault==106){CHECK(personal_scq_inputs_close(&c)!=0&&c.phase==4&&c.reader[0]);reader_fault=0;CHECK(personal_scq_inputs_close(&c)!=0&&input_closes==before+1);}
   else{reader_fault=0;mutated=0;stop=0;CHECK(!personal_scq_inputs_close(&c));before=input_closes;CHECK(!personal_scq_inputs_close(&c)&&input_closes==before);}
  }
  reader_fault=0;reader_stop=NULL;
 }
 memset(&foreign_inputs,0,sizeof(foreign_inputs));input_fault=mutated=0;stop=0;
 CHECK(!personal_scq_inputs_capture(&foreign_inputs,refs,fixture_sid,&stop));
 worker=CreateThread(NULL,0,input_foreign,NULL,0,NULL);CHECK(worker!=NULL);CHECK(WaitForSingleObject(worker,10000)==WAIT_OBJECT_0);
 CHECK(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE);CHECK(input_real_close(worker));
 /* use the real close API without routing it through modeled input handles */
 CHECK(!personal_scq_inputs_close(&foreign_inputs));
 {PAPACC_SCQ_REF_DATA expected[3];PAPACC_STORE_HASH h;BYTE content[]="abcdefgh";unsigned k;
  memset(&h,0,sizeof(h));memset(expected,0,sizeof(expected));reader_fault=input_fault=mutated=fail_free=0;stop=0;
  CHECK(!personal_scq_inputs_capture(&retained_contents,refs,fixture_sid,&stop));
  for(k=0;k<3;k++){expected[k].role=k+1;expected[k].rights=PAPACC_SCQ_FILE_READ_RIGHTS;expected[k].volume=7;expected[k].low=10+k;expected[k].size_low=8;expected[k].links=1;
   CHECK(papacc_store_hash(&h,content,8,expected[k].digest));CHECK(papacc_store_hash(&h,(BYTE *)retained_contents.descriptor[k],retained_contents.descriptor_bytes[k],expected[k].descriptor));}
  reader_fault=116;CHECK(personal_scq_inputs_contents(&retained_contents,fixture_sid,expected,GetTickCount64()+10000,&stop)!=0);
  CHECK(retained_contents.phase==4&&retained_contents.content_hash.retained&&retained_contents.content_hash.hash&&!retained_contents.content_verified);
  before=input_closes;reader_fault=0;CHECK(personal_scq_inputs_close(&retained_contents)!=0&&input_closes==before);
  /* Retained native CNG references intentionally stay in the fixture owner.
   * Process exit is not reported as an observed provider cleanup. */
 }
 CHECK(!input_real_free(fixture_sid));
 printf("PASS SCQ_INPUTS cases=41 checks=%u REAL_CNG_SID_SD_THREAD MODELED_TOKEN_FILE_READS_AND_IMAGE NO_ENDPOINT_NO_AUTHORITY bytes=%zu\n",checks,sizeof(c));return 0;
}
