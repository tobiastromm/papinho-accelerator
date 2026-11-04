/* Actual K1 source constructor/act and actual CA owner. Native calls modeled.
 * No real file, token, SCM, RNG, provider/key or administrative act is used. */
#define _WIN32_WINNT 0x0600
#include "personal_machine_fixture_win32.h"
static int fs_public_hash(PAPACC_STORE_HASH *,const unsigned char *,unsigned,unsigned char [32]);
#define PAPACC_MACHINE_FIXTURE_DATA_HASH fs_public_hash
#define PAPACC_MACHINE_FIXTURE_ENTRY preserved_machine_fixture_main
#include "scq_machine_fixture_test.c"
#include "server_configuration_commit_win32.h"
#include "../tools/qualification/storage-hash-capsule/storage_sha256.h"
#include <aclapi.h>
static PAPACC_CONFIGURATION_COMMIT fs_owner;
static unsigned fs_policy_seen,fs_scope_seen,fs_sd_policy_seen;
static unsigned fs_case,fs_bad,fs_opens,fs_rng,fs_closes,fs_cursor[5],fs_close_mask;
static HLOCAL WINAPI fs_real_free(HLOCAL h){return LocalFree(h);}
static PSECURITY_DESCRIPTOR fs_doc,fs_image,fs_directory,fs_changed;
static PAPACC_CONFIGURATION_COMMIT *fs_active;
static const WCHAR fs_parent[]=L"C:\\Program Files\\Papinho\\Accelerator";
static const WCHAR fs_image_native[]=L"\\Device\\HarddiskVolume3\\Program Files\\Papinho\\Accelerator\\papacc_server.exe";
static const char *const fs_content[5]={"fixture original request","actual owner fixture decision","protected executor","qualified ssl","qualified crypto"};
static int fs_slot(HANDLE h){ULONG_PTR v=(ULONG_PTR)h;return v>=500U&&v<505U?(int)(v-500U):-1;}
static BOOL WINAPI fs_info(HANDLE h,LPBY_HANDLE_FILE_INFORMATION p)
{
 ULONG_PTR v=(ULONG_PTR)h;memset(p,0,sizeof(*p));p->dwVolumeSerialNumber=17;p->nFileIndexLow=(DWORD)v;p->nNumberOfLinks=1;
 if(v>=100U&&v<=103U)p->dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;else{int slot=fs_slot(h);if(slot<0){fs_bad++;return FALSE;}p->dwFileAttributes=FILE_ATTRIBUTE_NORMAL;p->nFileSizeLow=(DWORD)strlen(fs_content[slot]);}
 if(fs_case==4U&&v==101U)p->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;
 return TRUE;
}
static BOOL WINAPI fs_flags(HANDLE h,LPDWORD out){(void)h;*out=0;return TRUE;}
static DWORD WINAPI fs_type(HANDLE h){(void)h;return FILE_TYPE_DISK;}
static BOOL WINAPI fs_close(HANDLE h){fs_closes++;if(fs_case==24U&&h==(HANDLE)500)return FALSE;return TRUE;}
static HLOCAL WINAPI fs_free(HLOCAL h){if(fs_case==25U&&h==fs_active->pending_commit.personal_native.machine_fixture.source_scratch)return h;return LocalFree(h);}
static BOOL WINAPI fs_seek(HANDLE h,LARGE_INTEGER distance,PLARGE_INTEGER result,DWORD method)
{int i=fs_slot(h);(void)result;if(i<0||distance.QuadPart||method!=FILE_BEGIN){fs_bad++;return FALSE;}fs_cursor[i]=0;return TRUE;}
static BOOL WINAPI fs_read(HANDLE h,LPVOID out,DWORD capacity,LPDWORD bytes,LPOVERLAPPED ov)
{int i=fs_slot(h);size_t n;(void)ov;if(i<0){fs_bad++;return FALSE;}n=strlen(fs_content[i])-fs_cursor[i];if(n>capacity)n=capacity;
 memcpy(out,fs_content[i]+fs_cursor[i],n);fs_cursor[i]+=(unsigned)n;*bytes=(DWORD)n;return TRUE;}
static BOOL WINAPI fs_process_image(HANDLE h,DWORD flags,LPWSTR out,PDWORD bytes)
{(void)h;if(flags!=PROCESS_NAME_NATIVE){fs_bad++;return FALSE;}if(*bytes<=wcslen(fs_image_native))return FALSE;wcscpy_s(out,*bytes,fs_image_native);*bytes=(DWORD)wcslen(out);return TRUE;}
static DWORD WINAPI fs_final(HANDLE h,LPWSTR out,DWORD cap,DWORD flags)
{int i=fs_slot(h);const WCHAR *leaf=i==3?L"libssl-3-x64.dll":i==4?L"libcrypto-3-x64.dll":L"papacc_server.exe";
 if(i==2&&flags==(FILE_NAME_NORMALIZED|VOLUME_NAME_NT))wcscpy_s(out,cap,fs_image_native);
 else if(i>=3&&flags==(FILE_NAME_NORMALIZED|VOLUME_NAME_DOS)){swprintf_s(out,cap,L"\\\\?\\%s\\%s",fs_parent,leaf);if(fs_case==30U)out[5]^=1;}
 else {fs_bad++;return 0;}return (DWORD)wcslen(out);}
static DWORD WINAPI fs_module_name(HMODULE module,LPWSTR out,DWORD cap)
{const WCHAR *leaf=module==(HMODULE)2?L"libssl-3-x64.dll":module==(HMODULE)3?L"libcrypto-3-x64.dll":L"papacc_server.exe";
 swprintf_s(out,cap,L"%s\\%s",fs_parent,leaf);return (DWORD)wcslen(out);}
static HMODULE WINAPI fs_module(LPCWSTR name)
{return !wcscmp(name,L"ntdll.dll")?(HMODULE)1:!wcscmp(name,L"libssl-3-x64.dll")?(HMODULE)2:!wcscmp(name,L"libcrypto-3-x64.dll")?(HMODULE)3:NULL;}
static NTSTATUS NTAPI fs_open(PHANDLE out,ACCESS_MASK access,POBJECT_ATTRIBUTES a,PIO_STATUS_BLOCK io,PLARGE_INTEGER allocation,ULONG attributes,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_bytes)
{
 const WCHAR *name=a->ObjectName->Buffer;ULONG_PTR value=0;(void)allocation;(void)attributes;(void)ea;(void)ea_bytes;fs_opens++;
 if(a->Attributes!=(OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE)||share!=FILE_SHARE_READ||disposition!=1U||
  (access&(FILE_WRITE_DATA|FILE_APPEND_DATA|DELETE|WRITE_DAC|WRITE_OWNER))){fs_bad++;return (NTSTATUS)0xc0000022L;}
 if(fs_case==5U){*out=NULL;return (NTSTATUS)0xc000050bL;}if(fs_case==6U){*out=(HANDLE)999;io->Status=(NTSTATUS)0x103;return (NTSTATUS)0x103;}
 if(!a->RootDirectory&&!wcsncmp(name,L"\\Device\\HarddiskVolume3",23U))value=100;
 else if(!wcscmp(name,L"Program Files")&&a->RootDirectory==(HANDLE)100)value=101;
 else if(!wcscmp(name,L"Papinho")&&a->RootDirectory==(HANDLE)101)value=102;
 else if(!wcscmp(name,L"Accelerator")&&a->RootDirectory==(HANDLE)102)value=103;
 else if(a->RootDirectory==(HANDLE)103){
  if(!wcscmp(name,L"papacc_server.exe"))value=502;
  else if(!wcscmp(name,L".fixture.request.original"))value=500;
  else if(!wcscmp(name,L".fixture.decision.original"))value=fs_case==16U?500U:501U;
  else if(!wcscmp(name,L"libssl-3-x64.dll"))value=503;
  else if(!wcscmp(name,L"libcrypto-3-x64.dll"))value=504;
 }
 if(fs_case==7U&&value==500U){*out=NULL;return (NTSTATUS)0xc0000034L;}
 if(!value){fs_bad++;return (NTSTATUS)0xc0000034L;}
 if(value<500U&&options!=0x00200021U||value>=500U&&options!=0x00200060U){fs_bad++;return (NTSTATUS)0xc000000dL;}
 *out=(HANDLE)value;io->Status=0;io->Information=1;return 0;
}
static NTSTATUS NTAPI fs_sd(HANDLE h,SECURITY_INFORMATION info,PSECURITY_DESCRIPTOR out,ULONG capacity,PULONG bytes)
{
 PSECURITY_DESCRIPTOR v=(ULONG_PTR)h<500U?fs_directory:fs_slot(h)<2?fs_doc:fs_image;DWORD n;
 if(fs_case==2U&&h==(HANDLE)500||fs_case==3U&&h==(HANDLE)502)v=fs_changed;
 if(info!=(OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION)){fs_bad++;return (NTSTATUS)0xc000000dL;}
 n=GetSecurityDescriptorLength(v);*bytes=n;if(!out||capacity<n)return (NTSTATUS)0xc0000023L;memcpy(out,v,n);return 0;
}
static FARPROC WINAPI fs_proc(HMODULE mod,LPCSTR name)
{FARPROC p=NULL;(void)mod;
 if(!strcmp(name,"NtCreateFile")){NTSTATUS (NTAPI *f)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG)=fs_open;memcpy(&p,&f,sizeof(p));}
 if(!strcmp(name,"NtQuerySecurityObject")){NTSTATUS (NTAPI *f)(HANDLE,SECURITY_INFORMATION,PSECURITY_DESCRIPTOR,ULONG,PULONG)=fs_sd;memcpy(&p,&f,sizeof(p));}return p;}

static DWORD WINAPI fs_device(LPCWSTR drive,LPWSTR out,DWORD cap)
{(void)drive;wcscpy_s(out,cap,fs_case==31U?L"\\??\\D:\\alias":L"\\Device\\HarddiskVolume3");return (DWORD)wcslen(out)+2U;}
static NTSTATUS WINAPI fs_random(BCRYPT_ALG_HANDLE alg,PUCHAR out,ULONG bytes,ULONG flags)
{fs_rng++;if(alg||flags!=BCRYPT_USE_SYSTEM_PREFERRED_RNG){fs_bad++;return (NTSTATUS)0xc000000dL;}memset(out,fs_case==34U?0:(int)fs_rng,bytes);return 0;}
static BOOL WINAPI fs_lookup(LPCWSTR system,LPCWSTR account,PSID sid,LPDWORD bytes,LPWSTR domain,LPDWORD domain_bytes,PSID_NAME_USE use)
{DWORD n=GetLengthSid(virtual_sid);(void)system;if(fs_case==33U||wcscmp(account,L"NT SERVICE\\PapinhoAccelerator")||*bytes<n)return FALSE;
 memcpy(sid,virtual_sid,n);*bytes=n;if(*domain_bytes<2U)return FALSE;domain[0]=L'.';domain[1]=0;*domain_bytes=1;*use=SidTypeUser;return TRUE;}
static int fs_token(PAPACC_CONFIG_PENDING *p,int capture)
{if(fs_case==8U&&!capture)return 0;p->personal_native.sid_bytes=GetLengthSid(admin_sid);CopySid(sizeof(p->personal_native.sid),p->personal_native.sid,admin_sid);return 1;}
static PAPACC_RESULT fs_install(PAPACC_SERVER_CONSOLE_WIN32 *p){p->is_installed=1;return PAPACC_RESULT_OK;}
static int fs_stopped(void){return fs_active->pending_commit.stop?1:0;}
static BOOL WINAPI fs_console(HANDLE h,LPDWORD mode){(void)h;*mode=0;return TRUE;}
static int fs_write(HANDLE h,const char *text){(void)h;
 if(strstr(text,"K1 ")){fs_policy_seen++;if(!strstr(text,"entry128/cap192/precall188/private16777216/working16777216/physical7239172096/commit3221225472")||strstr(text,"precall124")||strstr(text,"commit9823326208"))fs_bad++;}
 if(strstr(text,"S0_S5 FIXTURE SD READBACK:")){fs_sd_policy_seen++;if(!strstr(text,"exactly GAx3 OR D01F01FFx3")||!strstr(text,"SETTER=GA"))fs_bad++;}
 if(strstr(text,"historical request is DATA only"))fs_scope_seen++;
 return 1;}
static int fs_confirm(HANDLE in,HANDLE out,const char *expected)
{(void)in;(void)out;if(fs_policy_seen!=1U||fs_scope_seen!=1U||fs_sd_policy_seen!=1U)fs_bad++;if(strncmp(expected,"OWNER_MACHINE_FIXTURE_CREATE_ONCE ",34U))fs_bad++;if(fs_case==18U)fs_active->pending_commit.stop=1;return fs_case!=17U;}
#undef GetFileInformationByHandle
#undef GetFileType
#undef GetHandleInformation
#undef CloseHandle
#undef GetModuleHandleW
#undef GetProcAddress
#undef GetModuleFileNameW
#undef GetFinalPathNameByHandleW
#undef QueryDosDeviceW
#define GetFileInformationByHandle fs_info
#define GetFileType fs_type
#define GetHandleInformation fs_flags
#define CloseHandle fs_close
#define LocalFree fs_free
#define SetFilePointerEx fs_seek
#define ReadFile fs_read
#define QueryFullProcessImageNameW fs_process_image
#define GetFinalPathNameByHandleW fs_final
#define GetModuleFileNameW fs_module_name
#define GetModuleHandleW fs_module
#define GetProcAddress fs_proc
#define QueryDosDeviceW fs_device
#define BCryptGenRandom fs_random
#define LookupAccountNameW fs_lookup
#define GetConsoleMode fs_console
#define personal_token fs_token
#define papacc_server_console_win32_install fs_install
#define papacc_server_console_win32_stop_requested fs_stopped
#define personal_console_write fs_write
#define personal_console_confirm fs_confirm
static int owner(PAPACC_CONFIGURATION_COMMIT *o){(void)o;return 0;}
#include "../apps/server/server_store_data_hash_win32.inc"
static int fs_public_hash(PAPACC_STORE_HASH *h,const unsigned char *p,unsigned n,unsigned char out[32])
{if(fs_case==20U){h->retained=1;return 0;}return store_data_hash(h,p,n,out);}
#include "../apps/server/personal_install_paths_win32.inc"
#include "../apps/server/personal_scq_inputs_win32.inc"
DWORD papacc_personal_runtime_current(PAPACC_PERSONAL_RUNTIME_IDENTITY *r){(void)r;return ERROR_INVALID_STATE;}
#include "../apps/server/personal_service_sid_win32.inc"
#include "../apps/server/personal_machine_fixture_source_win32.inc"
static int fs_setup_sd(void)
{
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&admin_sid));C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 C(ConvertStringSecurityDescriptorToSecurityDescriptorA("O:S-1-5-21-111-222-333-1001G:SYD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;S-1-5-21-111-222-333-1001)",1,&fs_doc,NULL));
 C(ConvertStringSecurityDescriptorToSecurityDescriptorA("O:BAG:SYD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;S-1-5-21-111-222-333-1001)",1,&fs_image,NULL));
 C(ConvertStringSecurityDescriptorToSecurityDescriptorA("O:BAG:SYD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;S-1-5-21-111-222-333-1001)",1,&fs_directory,NULL));
 C(ConvertStringSecurityDescriptorToSecurityDescriptorA("O:BAG:SYD:P(A;;FA;;;WD)",1,&fs_changed,NULL));return 0;
}
static void fs_teardown(void)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&fs_owner.pending_commit.personal_native;unsigned i;
 /* Only memory allocated by this test's SD query is disposed here. No fake
  * HANDLE is ever passed to native CloseHandle and no product close is retried. */
 for(i=0;i<5U;i++)if(i!=2U&&n->machine_fixture.references[i].sd){fs_real_free(n->machine_fixture.references[i].sd);n->machine_fixture.references[i].sd=NULL;}
 if(n->machine_fixture.source_scratch){fs_real_free(n->machine_fixture.source_scratch);n->machine_fixture.source_scratch=NULL;}
 for(i=0;i<=PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS;i++)if(n->scq_inputs.image_chain.sd[i]){fs_real_free(n->scq_inputs.image_chain.sd[i]);n->scq_inputs.image_chain.sd[i]=NULL;}
 if(n->scq_inputs.image_chain.installer){fs_real_free(n->scq_inputs.image_chain.installer);n->scq_inputs.image_chain.installer=NULL;}
}
int main(int argc,char **argv)
{
 PAPACC_MACHINE_FIXTURE_INPUT input;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_MACHINE_FIXTURE_CONTROL *f;DWORD e;unsigned k,j,before=checks;
 unsigned observed_sd=argc==2&&!strcmp(argv[1],"observed-sd")?1U:0U;C(argc==1||(argc==2&&observed_sd));
 C(!fs_setup_sd());memset(&input,0,sizeof(input));input.request_path=L"C:\\Program Files\\Papinho\\Accelerator\\.fixture.request.original";input.decision_path=L"C:\\Program Files\\Papinho\\Accelerator\\.fixture.decision.original";
 input.known_managed=sizeof(fs_owner)+PAPACC_MACHINE_FIXTURE_MANAGED_EXTRA;
 for(j=0;j<5U;j++)C(pacc_storage_hash_callback(NULL,(BYTE*)fs_content[j],(unsigned)strlen(fs_content[j]),input.pins[j])==1);
 for(k=0;k<37U;k++){
  memset(&fs_owner,0,sizeof(fs_owner));fs_active=&fs_owner;fs_owner.writer=GetCurrentThreadId();n=&fs_owner.pending_commit.personal_native;f=&n->machine_fixture;
  fs_policy_seen=fs_scope_seen=fs_sd_policy_seen=0;fixture_sd_fault=observed_sd?5U:0U;fixture_sd_custom=fixture_sd_shape=fixture_generic_model=0;fs_case=k;fs_opens=fs_rng=fs_bad=fs_closes=fs_close_mask=0;memset(fs_cursor,0,sizeof(fs_cursor));
  fail_at=free_fail=mismatch=stop_after=0;calls=creates=opens=finals=properties=exports=freed=0;fixture_stop=0;fb_count=100;fb_query_fault=fb_qpc_calls=fb_stale=fb_stop=0;fb_tick=100;fh_fault=fh_calls=0;
  if(k==1U)input.pins[0][0]^=1;
  if(k==26U)input.known_managed=1;if(k==27U)fb_count=129;if(k==29U)n->scq_admin.phase=1U;
  if(k==32U)input.request_path=L"C:\\workspace\\request";
  e=papacc_configuration_personal_machine_fixture_source(&fs_owner,&input);
  if(k==1U)input.pins[0][0]^=1;
  input.known_managed=sizeof(fs_owner)+PAPACC_MACHINE_FIXTURE_MANAGED_EXTRA;input.request_path=L"C:\\Program Files\\Papinho\\Accelerator\\.fixture.request.original";
  if(k==1U||k>=2U&&k<=8U||k==16U||k==25U||k==26U||k==27U||k==29U||k==30U||k==31U||k==32U||k==33U||k==34U){C(e!=0&&!creates);
   if(k==6U||k==25U){unsigned saved=fs_closes;C(f->source_unknown&&scqkf_source_close(f)!=0&&fs_closes==saved);}
   fs_teardown();continue;}
  C(!e&&!creates&&f->source_phase==1U&&fs_rng==3U);
  if(k==9U)f->generation[0]^=1;if(k==10U)f->attempt[0]^=1;if(k==11U)f->plan_id[0]^=1;if(k==12U)f->source_until++;
  if(k==13U)f->references[0].identity.nFileIndexLow++;
  if(k==14U)f->references[0].digest[0]^=1;
  if(k==15U)f->references[0].sd_bytes++;
  if(k==20U)fh_fault=5U;if(k==21U)mismatch=1;
  if(k==22U)fail_at=2U;if(k==23U)fail_at=1U;if(k==28U)f->source_thread++;if(k==35U)fs_owner.pending_commit.stop=1;if(k==36U)f->key_name[0]^=1;
  e=papacc_configuration_personal_machine_fixture_execute(&fs_owner,stdout);
  if(k>=9U&&k<=18U||k==28U||k==35U||k==36U){C(e!=0&&!creates&&!f->source_consumed);}
  else if(k==22U||k==23U){C(e!=0&&!f->key.created&&f->source_consumed);}
  else if(k==20U||k==21U){C(e!=0&&creates==1U&&f->source_consumed&&!f->public_id[0]);}
  else {C(!e&&creates==1U&&f->phase==2U&&f->source_consumed);C(papacc_configuration_personal_machine_fixture_execute(&fs_owner,stdout)!=0&&creates==1U);}
  C(!fs_bad);
  if(k==20U){C(scqkf_close(f,&fs_owner)!=0&&f->hash.retained);fs_teardown();continue;}
  C(!scqkf_close(f,&fs_owner));
  if(k==28U){unsigned saved=fs_closes;C(scqkf_source_close(f)!=0&&fs_closes==saved);fs_teardown();continue;}
  if(k==24U){unsigned saved;C(scqkf_source_close(f)!=0);saved=fs_closes;C(scqkf_source_close(f)!=0&&fs_closes==saved);fs_teardown();continue;}
  C(!scqkf_source_close(f));C(!scqi_chain_close(&n->scq_inputs.image_chain));fs_teardown();
 }
 printf("SOURCE_S1_SD_FORM=%s\n",observed_sd?"D01F01FF":"GENERIC_ALL");
 printf("PASS MACHINE_FIXTURE_SOURCE cases=37 checks=%u ACTUAL_CONSTRUCTOR_ACT_AND_CA_NATIVE_BOUNDARIES_MODELED NO_REAL_FILE_TOKEN_RNG_KSP_SCM\n",checks-before);return 0;
}
