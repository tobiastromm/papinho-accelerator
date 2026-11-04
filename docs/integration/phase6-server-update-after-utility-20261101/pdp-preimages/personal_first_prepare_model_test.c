#define _WIN32_WINNT 0x0600
#include "server_configuration_commit_win32.h"
#include <sddl.h>
#include <aclapi.h>
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
/* All host queries, handles, CNG/RNG and file operations are substituted.
 * Real Win32 SID/SD functions below operate only on model-owned memory. */
static PAPACC_CONFIGURATION_COMMIT model_owner;static PAPACC_CA_KEY model_key;
static PSID model_actor,runtime;static PSECURITY_DESCRIPTOR descriptor;
static DWORD first_fault,chain_fault,budget_fault,metadata_fault,source_fault,token_fault,sd_fault;
static unsigned acquisitions,rng_calls,checks;
static unsigned child_calls,parent_calls,parent_fault,key_calls,thread_calls,close_calls,submit_fault,wait_fault,close_fault,native_fault,key_fault;
static BYTE closed_handles[1024];
static ULONGLONG tick=100;static DWORD handles=120;
static const BYTE model_attempt[32]={7};
static const unsigned char config[]="format_version 1\nclient_admission DISABLED\n";
#define C(x) do{checks++;if(!(x)){printf("FCA_MODEL_FAIL line=%u checks=%u phase=%lu native_phase=%u result=%u handoff=%ld first=%lu native=0x%08lx child_calls=%u created=%u id=%lu:%lu:%lu iosb=%llu\n",(unsigned)__LINE__,checks,model_owner.pending_commit.personal_native.first_phase,model_owner.pending_commit.personal_native.phase,(unsigned)model_owner.pending_commit.result,model_owner.pending_commit.personal_native.handoff_state,model_owner.pending_commit.personal_native.first_error,model_owner.pending_commit.native_error,child_calls,model_owner.pending_commit.bootstrap_created,model_owner.pending_commit.bootstrap_volume,model_owner.pending_commit.bootstrap_high,model_owner.pending_commit.bootstrap_low,(unsigned long long)model_owner.pending_commit.personal_native.scq_cold_io.Information);return 1;}}while(0)
static DWORD WINAPI f_count(void){return GetCurrentThreadId();}
static ULONGLONG WINAPI f_tick(void){return tick;}
static BOOL WINAPI f_qpc(LARGE_INTEGER *v){v->QuadPart=100;return TRUE;}
static BOOL WINAPI f_handle(HANDLE h,DWORD *flags){if(metadata_fault==4U||!h)return FALSE;*flags=0;return TRUE;}
static DWORD WINAPI f_type(HANDLE h){(void)h;return FILE_TYPE_DISK;}
static BOOL WINAPI f_info(HANDLE h,BY_HANDLE_FILE_INFORMATION *v)
{
 ULONG_PTR value=(ULONG_PTR)h;memset(v,0,sizeof(*v));v->dwVolumeSerialNumber=1;v->nNumberOfLinks=1;
 v->dwFileAttributes=(value==500U)?FILE_ATTRIBUTE_NORMAL:FILE_ATTRIBUTE_DIRECTORY;
 v->nFileIndexLow=(DWORD)(value==500U?200U:value==600U?102U:value);if(value==500U)v->nFileSizeLow=sizeof(config)-1U;
 if(metadata_fault==1U)v->dwFileAttributes|=FILE_ATTRIBUTE_REPARSE_POINT;
 if(metadata_fault==2U)v->nFileIndexLow++;
 if(metadata_fault==3U&&value==500U)v->nNumberOfLinks=2U;
 return TRUE;
}
static DWORD f_budget(PAPACC_CA_KEY *k,volatile LONG *stop)
{return budget_fault?budget_fault:!k||*stop||k->diagnostic_budget.first?ERROR_NOT_ENOUGH_QUOTA:0;}
static DWORD f_arm(PAPACC_CA_KEY *k,volatile LONG *stop)
{if(f_budget(k,stop)||k->diagnostic_budget.active!=2U)return ERROR_INVALID_STATE;k->diagnostic_budget.active=3U;k->diagnostic_budget.until=60100;return 0;}
static DWORD personal_resource_collect(PAPACC_SCM_RESOURCE_OBSERVATION *r,DWORD phase,int legacy)
{
 (void)legacy;memset(&r->last,0,sizeof(r->last));r->last.frequency=1000;r->last.begin=r->last.end=100;
 r->last.phase=phase;r->last.handles=handles;r->last.physical_available=7239172096ULL;r->last.commit_available=3221225472ULL;
 r->last.private_bytes=r->last.working_set=4096;r->samples++;return budget_fault;
}
static PSECURITY_DESCRIPTOR clone_descriptor(void)
{DWORD z=GetSecurityDescriptorLength(descriptor);PSECURITY_DESCRIPTOR v=LocalAlloc(LMEM_FIXED,z);if(v)memcpy(v,descriptor,z);return v;}
static DWORD scqi_native_file_sd(HANDLE h,PSECURITY_DESCRIPTOR *out,PAPACC_SCQ_NATIVE_SD_STATUS *status,volatile LONG *stop)
{
 (void)status;if(*stop||sd_fault)return ERROR_ACCESS_DENIED;
 if(h==(HANDLE)(ULONG_PTR)900){DWORD z=GetSecurityDescriptorLength(model_owner.pending_commit.personal_native.first_child_sd);*out=LocalAlloc(LMEM_FIXED,z);if(*out)memcpy(*out,model_owner.pending_commit.personal_native.first_child_sd,z);return *out?0:ERROR_OUTOFMEMORY;}
 *out=clone_descriptor();return *out?0:ERROR_OUTOFMEMORY;
}
static int scqi_directory_same(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return (a->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&!(a->dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)&&!memcmp(a,b,sizeof(*a));}
static int personal_scq_inputs_same(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return !(a->dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))&&!memcmp(a,b,sizeof(*a));}
static int scqi_directory_sd(PSECURITY_DESCRIPTOR sd,PSID who,PSID installer,int anchor)
{(void)sd;(void)who;(void)installer;(void)anchor;return sd_fault==0U;}
static int personal_scq_inputs_protected(PSECURITY_DESCRIPTOR sd,PSID who)
{(void)sd;(void)who;return sd_fault==0U;}
static DWORD scqi_chain_open(PAPACC_SCQ_IMAGE_CHAIN *g,unsigned index,HANDLE parent,const WCHAR *name,int directory)
{
 acquisitions++;if(chain_fault==acquisitions)return ERROR_ACCESS_DENIED;
 if(g->plan.purpose!=PAPACC_FIRST_PREPARE_CHAIN_TAG||g->stop!=&model_owner.pending_commit.stop||!name||!name[0]||
  (index&&parent!=(HANDLE)(ULONG_PTR)(99U+index)&&!(index==3U&&parent==(HANDLE)(ULONG_PTR)100U)))return ERROR_INVALID_DATA;
 if(directory){g->directory[index]=(HANDLE)(ULONG_PTR)(100U+index);f_info(g->directory[index],&g->identity[index]);}
 else {g->image=(HANDLE)(ULONG_PTR)500;f_info(g->image,&g->identity[index]);}
 g->sd[index]=clone_descriptor();g->length[index]=GetSecurityDescriptorLength(g->sd[index]);return 0;
}
static DWORD scq_p1_native_name(const WCHAR *drive,int directory,WCHAR out[MAX_PATH+4U])
{(void)drive;(void)directory;wcscpy_s(out,MAX_PATH+4U,L"\\Device\\Model\\");return 0;}
static int personal_deny(PAPACC_CONFIG_PENDING *p,DWORD stage,DWORD error,DWORD kind)
{PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;if(!n->diagnostic_first){n->diagnostic_first=stage;n->diagnostic_error=error;n->diagnostic_kind=kind;}return 0;}
static int personal_token(PAPACC_CONFIG_PENDING *p,int capture)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 if(first_fault||token_fault||p->stop)return personal_deny(p,2,ERROR_ACCESS_DENIED,1);
 if(capture){n->sid_bytes=GetLengthSid(model_actor);CopySid(sizeof(n->sid),n->sid,model_actor);p->bootstrap_token=(HANDLE)(ULONG_PTR)800;}
 return 1;
}
static PAPACC_RESULT personal_packet_current(PAPACC_CONFIG_PENDING *p)
{(void)p;return token_fault?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
static PAPACC_RESULT personal_packet_source(PAPACC_CONFIG_PENDING *p)
{return source_fault||memcmp(p->next,config,sizeof(config)-1U)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
static PAPACC_RESULT f_current(PAPACC_CONFIGURATION_COMMIT *c){return personal_packet_current(&c->pending_commit);}
static PAPACC_RESULT f_source(PAPACC_CONFIGURATION_COMMIT *c){return personal_packet_source(&c->pending_commit);}
static DWORD WINAPI f_device(LPCSTR drive,LPSTR output,DWORD cap)
{static const char native[]="\\Device\\Model";if(strcmp(drive,"C:")||cap<sizeof(native))return 0;memcpy(output,native,sizeof(native));return sizeof(native);}
static DWORD WINAPI f_security(HANDLE h,SE_OBJECT_TYPE type,SECURITY_INFORMATION mask,PSID *own,PSID *group,PACL *dacl,PACL *sacl,PSECURITY_DESCRIPTOR *out)
{(void)type;(void)own;(void)group;(void)dacl;(void)sacl;if(h!=(HANDLE)(ULONG_PTR)600||mask!=(OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION))return ERROR_INVALID_PARAMETER;*out=clone_descriptor();return *out?0:ERROR_OUTOFMEMORY;}
static HMODULE WINAPI f_module(LPCSTR);
static FARPROC WINAPI f_address(HMODULE,LPCSTR);
typedef NTSTATUS (NTAPI *BOOTSTRAP_NT_CREATE)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
static BOOL WINAPI f_lookup(LPCWSTR system,LPCWSTR account,PSID sid,LPDWORD bytes,LPWSTR domain,LPDWORD domain_bytes,PSID_NAME_USE use)
{(void)system;(void)domain;(void)domain_bytes;if(wcscmp(account,L"NT SERVICE\\PapinhoAccelerator"))return FALSE;*bytes=GetLengthSid(runtime);*use=SidTypeUser;return CopySid(SECURITY_MAX_SID_SIZE,sid,runtime);}
static NTSTATUS WINAPI f_rng(BCRYPT_ALG_HANDLE a,PUCHAR out,ULONG bytes,ULONG flags)
{(void)a;(void)flags;rng_calls++;memset(out,(int)rng_calls,bytes);return 0;}
#define GetTickCount64 f_tick
#define QueryPerformanceCounter f_qpc
#define GetHandleInformation f_handle
#define GetFileType f_type
#define GetFileInformationByHandle f_info
#define papacc_ca_key_first_budget_check f_budget
#define papacc_ca_key_first_budget_postcheck f_budget
#define papacc_ca_key_first_budget_arm f_arm
#define papacc_configuration_personal_native_open f_open
#define papacc_configuration_personal_native_current f_current
#define papacc_configuration_personal_source_current f_source
#define QueryDosDeviceA f_device
#define GetSecurityInfo f_security
#define GetModuleHandleA f_module
#define GetProcAddress f_address
#define LookupAccountNameW f_lookup
#define BCryptGenRandom f_rng
#define papacc_configuration_personal_first_open first_open_model
#define papacc_configuration_personal_first_confirm first_confirm_model
#ifndef OBJ_DONT_REPARSE
#define OBJ_DONT_REPARSE 0x1000L
#endif
#include "../apps/server/server_configuration_personal_open_win32.inc"
#include "../apps/server/server_configuration_first_prepare_win32.inc"
/* Exercise the production exclusive primitive, worker and dispatch, with
 * only their native effects substituted. No real thread or directory exists. */
#ifndef OBJ_DONT_REPARSE
#define OBJ_DONT_REPARSE 0x1000L
#endif
static NTSTATUS NTAPI f_create(PHANDLE out,ACCESS_MASK rights,POBJECT_ATTRIBUTES attr,PIO_STATUS_BLOCK io,PLARGE_INTEGER size,ULONG file_attributes,ULONG share,ULONG disposition,ULONG options,PVOID ea,ULONG ea_size)
{
 PAPACC_CONFIG_PENDING *p=&model_owner.pending_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 (void)size;(void)ea;(void)ea_size;
 if(!attr->RootDirectory){
  parent_calls++;
  if(attr!=&n->scq_cold_attributes||io!=&n->scq_cold_io||!(attr->Attributes&OBJ_DONT_REPARSE)||
   attr->ObjectName->Buffer!=n->scq_cold_name||wcscmp(n->scq_cold_name,L"\\Device\\Model\\ProgramData\\Papinho")||
   !(rights&FILE_ADD_SUBDIRECTORY)||!(rights&READ_CONTROL)||share!=FILE_SHARE_READ||disposition!=1U)return (NTSTATUS)0xc000000dL;
  if(parent_fault==1U)return (NTSTATUS)0xc0000022L;
  if(parent_fault==2U){io->Status=(NTSTATUS)0x103L;return (NTSTATUS)0x103L;}
  if(parent_fault==3U)return (NTSTATUS)0xc000050bL;
  *out=(HANDLE)(ULONG_PTR)600;io->Status=0;io->Information=1U;return 0;
 }
 child_calls++;
 if(attr!=&n->scq_cold_attributes||io!=&n->scq_cold_io||attr->RootDirectory!=p->bootstrap_parent||
  !(attr->Attributes&OBJ_DONT_REPARSE)||attr->SecurityDescriptor!=n->first_child_sd||attr->ObjectName->Buffer!=n->scq_cold_name||
  wcscmp(n->scq_cold_name,L"Accelerator")||!(rights&READ_CONTROL)||share!=FILE_SHARE_READ||disposition!=2U||
  file_attributes!=FILE_ATTRIBUTE_DIRECTORY||options!=0x00200021U)return (NTSTATUS)0xc000000dL;
 if(native_fault==1U)return (NTSTATUS)0xc0000035L;
 if(native_fault==2U){io->Status=(NTSTATUS)0x103L;return (NTSTATUS)0x103L;}
 *out=(HANDLE)(ULONG_PTR)900;io->Status=0;io->Information=native_fault==3U?1U:2U;return 0;
}
static HMODULE WINAPI f_module(LPCSTR name){(void)name;return (HMODULE)(ULONG_PTR)1;}
static FARPROC WINAPI f_address(HMODULE module,LPCSTR name)
{FARPROC result;BOOTSTRAP_NT_CREATE create=f_create;(void)module;(void)name;if(native_fault==4U)tick=60200;memcpy(&result,&create,sizeof(result));return result;}
#include "../apps/server/server_configuration_bootstrap_child_win32.inc"
static int personal_native_ceremony_ready(const PAPACC_CONFIG_PENDING *p){(void)p;return 0;}
#include "../apps/server/server_configuration_personal_target_worker_win32.inc"
static DWORD WINAPI pending_worker(void *input){personal_target_worker(input);return 0;}
static HANDLE WINAPI f_thread(LPSECURITY_ATTRIBUTES security,SIZE_T stack,LPTHREAD_START_ROUTINE entry,LPVOID context,DWORD flags,LPDWORD thread_id)
{
 (void)security;(void)stack;(void)flags;(void)thread_id;thread_calls++;
 if(submit_fault){SetLastError(ERROR_NOT_ENOUGH_MEMORY);return NULL;}
 (void)entry(context);return (HANDLE)(ULONG_PTR)700;
}
static DWORD WINAPI f_wait(HANDLE h,DWORD ms){(void)h;(void)ms;return wait_fault?WAIT_TIMEOUT:WAIT_OBJECT_0;}
static BOOL WINAPI f_close(HANDLE h)
{ULONG_PTR value=(ULONG_PTR)h;close_calls++;if(close_fault||value>=sizeof(closed_handles)||closed_handles[value])return FALSE;closed_handles[value]=1;return TRUE;}
static int owner(PAPACC_CONFIGURATION_COMMIT *c){return c&&c->initialized&&c->writer==f_count();}
#define CreateThread f_thread
#define WaitForSingleObject f_wait
#define CloseHandle f_close
#define papacc_configuration_personal_target_begin first_target_begin_model
#define papacc_configuration_personal_handoff_poll first_poll_model
#include "../apps/server/server_configuration_personal_target_dispatch_win32.inc"
static SECURITY_STATUS f_key_create(PAPACC_CA_KEY *k,const BYTE a[16],const BYTE g[16],PSID who,PSID service)
{
 key_calls++;if(key_fault)return key_fault==1U?NTE_EXISTS:NTE_FAIL;k->thread=f_count();k->state=PAPACC_CA_KEY_READY;k->created=1;k->diagnostic_finalization=2;
 k->key=(NCRYPT_KEY_HANDLE)1;k->provider=(NCRYPT_PROV_HANDLE)2;k->operator_size=GetLengthSid(who);k->service_size=GetLengthSid(service);
 CopySid(sizeof(k->operator_sid),k->operator_sid,who);CopySid(sizeof(k->service_sid),k->service_sid,service);papacc_ca_key_name_data(k->name,a,g);return 0;
}
#define papacc_ca_key_create f_key_create
#define papacc_configuration_personal_ca_initialize first_ca_initialize_model
#define papacc_configuration_personal_ca_match_data first_ca_match_model
#define papacc_configuration_personal_ca_bind first_ca_bind_model
#define papacc_configuration_personal_ca_current first_ca_current_model
#define papacc_configuration_personal_ca_release first_ca_release_model
#define papacc_configuration_personal_ca_handoff first_ca_handoff_model
#define papacc_configuration_personal_ca_activate_normal first_ca_activate_model
#include "../apps/server/server_configuration_personal_ca_win32.inc"
static DWORD scqi_chain_fail(PAPACC_SCQ_IMAGE_CHAIN *g,DWORD e){if(!g->first)g->first=e;return g->first;}
static DWORD personal_scq_inputs_fail(PAPACC_PERSONAL_SCQ_INPUTS *c,DWORD stage,DWORD e){(void)stage;if(!c->first)c->first=e;return c->first;}
#include "../apps/server/personal_scq_image_chain_close_win32.inc"
#include "../apps/server/personal_scq_inputs_close_win32.inc"
static DWORD f_other_close(const void *sub,const void *own,int quiet){(void)sub;(void)own;(void)quiet;return 0;}
static DWORD f_other_close_pair(const void *sub,const void *own){(void)sub;(void)own;return 0;}
static DWORD f_fixture_close(PAPACC_MACHINE_FIXTURE_CONTROL *f,const void *own){(void)own;return f->phase?ERROR_INVALID_STATE:0;}
static DWORD f_fixture_source_close(PAPACC_MACHINE_FIXTURE_CONTROL *f){return f->phase?ERROR_INVALID_STATE:0;}
static DWORD f_raw_close(PAPACC_CONFIGURATION_COMMIT *c){return c->pending_commit.personal_native.scq_cold_raw[0]?ERROR_INVALID_STATE:0;}
static int scm_registration_files_close(PAPACC_PERSONAL_NATIVE_CONTEXT *context){(void)context;return 1;}
#define scqkf_close f_fixture_close
#define scqkf_source_close f_fixture_source_close
#define scqs_close f_other_close
#define scqr_close f_other_close_pair
#define scqa_close f_other_close
#define papacc_scq_profile_close_data f_other_close
#define personal_scq_receive_finish f_other_close
#define scqc_close_raw f_raw_close
#define papacc_configuration_personal_native_close first_close_model
#include "../apps/server/server_configuration_personal_close_win32.inc"
static void dispose(void)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&model_owner.pending_commit.personal_native;PAPACC_SCQ_IMAGE_CHAIN *g=&n->scq_inputs.image_chain;unsigned i;
 for(i=0;i<=PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS;i++)if(g->sd[i])LocalFree(g->sd[i]);
 if(g->scratch)LocalFree(g->scratch);if(g->installer)LocalFree(g->installer);if(n->descriptor)LocalFree(n->descriptor);
 if(n->sid_text)LocalFree(n->sid_text);if(n->first_child_sd)LocalFree(n->first_child_sd);
}
static void setup(void)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;dispose();memset(&model_owner,0,sizeof(model_owner));memset(&model_key,0,sizeof(model_key));
 first_fault=chain_fault=budget_fault=metadata_fault=source_fault=token_fault=sd_fault=0;acquisitions=rng_calls=0;tick=100;handles=120;
 child_calls=parent_calls=parent_fault=key_calls=thread_calls=close_calls=submit_fault=wait_fault=close_fault=native_fault=key_fault=0;memset(closed_handles,0,sizeof(closed_handles));
 model_owner.writer=f_count();p=&model_owner.pending_commit;n=&p->personal_native;p->file=INVALID_HANDLE_VALUE;
 n->install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;
 n->install_paths.thread=model_owner.writer;
 wcscpy_s(n->install_paths.parent,MAX_PATH,L"C:\\ProgramData\\Papinho");
 model_key.first_preparation=1;model_key.diagnostic_stop=&p->stop;model_key.diagnostic_budget.self=&model_key.diagnostic_budget;
 model_key.diagnostic_budget.thread=model_owner.writer;model_key.diagnostic_budget.active=2;model_key.diagnostic_budget.until=240100;
 model_key.diagnostic_budget.observation.first.frequency=1000;model_key.diagnostic_budget.observation.first.private_bytes=4096;model_key.diagnostic_budget.observation.first.working_set=4096;
}
static int plan_confirm(void)
{
 PAPACC_CONFIG_PENDING *p=&model_owner.pending_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_PERSONAL_EFFECT_SPEC v;
 BYTE parent[12]={1,0,0,0,0,0,0,0,102,0,0,0};char reply[128];size_t bytes,i,z;static const char hex[]="0123456789abcdef",prefix[]="OWNER_NEW_INSTALL_CA_INITIALIZE ";
 memcpy(p->next,config,sizeof(config)-1);memcpy(p->previous,"Accelerator",12);memcpy(n->plan_leaf,"Accelerator",12);
 memset(&v,0,sizeof(v));v.operator_id=n->sid;v.operator_bytes=n->sid_bytes;v.parent_id=parent;v.leaf=(BYTE *)n->plan_leaf;v.leaf_bytes=11;
 v.authority=n->first_authority;v.generation=n->first_generation;v.service_id=n->first_service;v.service_bytes=n->first_service_bytes;v.configuration=config;v.configuration_bytes=sizeof(config)-1;
 bytes=papacc_personal_effect_build(&v,p->previous+1024,PAPACC_PERSONAL_PLAN_MAX);if(!bytes)return 0;
 if(!papacc_personal_data_capture(&p->personal_data,model_attempt,config,sizeof(config)-1)||!papacc_personal_effect_upgrade(&p->personal_data,p->previous+1024,bytes)||!papacc_personal_data_present(&p->personal_data))return 0;
 z=sizeof(prefix)-1;memcpy(reply,prefix,z);for(i=0;i<32;i++){reply[z+i*2]=hex[model_attempt[i]>>4];reply[z+i*2+1]=hex[model_attempt[i]&15U];}
 n->presenter_phase=2;n->presenter_ca=1;memcpy(n->presenter_attempt,model_attempt,32);
 return papacc_personal_effect_confirm(&p->personal_data,model_attempt,p->previous+1024,bytes,reply,z+64);
}
int main(void)
{
 unsigned i;PAPACC_CONFIG_PENDING *p=&model_owner.pending_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PSECURITY_DESCRIPTOR child=NULL;DWORD child_bytes;
 if(!ConvertStringSidToSidW(L"S-1-5-21-1-2-3-1108",&model_actor)||!ConvertStringSidToSidW(L"S-1-5-80-1-2-3-4-5",&runtime)||
  !ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:S-1-5-21-1-2-3-1108D:P(A;;FA;;;S-1-5-21-1-2-3-1108)(A;;FA;;;SY)(A;;FA;;;BA)",1,&descriptor,NULL))return 2;
 setup();
#if !defined(PAPACC_ED11_WINDOWS_3MIB)
 (void)plan_confirm;
 C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_NOT_SUPPORTED&&!acquisitions);
 n->first_owner=p;n->first_key=&model_key;n->first_phase=2U;C(!personal_first_ready(p,0));
#else
 C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK&&acquisitions==4&&rng_calls==2);
 {SECURITY_DESCRIPTOR absolute;PACL acl;BOOL present,def;
  C(InitializeSecurityDescriptor(&absolute,SECURITY_DESCRIPTOR_REVISION));C(GetSecurityDescriptorDacl(n->descriptor,&present,&acl,&def));
  C(SetSecurityDescriptorOwner(&absolute,model_actor,FALSE)&&SetSecurityDescriptorDacl(&absolute,present,acl,FALSE)&&SetSecurityDescriptorControl(&absolute,SE_DACL_PROTECTED,SE_DACL_PROTECTED));
  C(personal_first_descriptors_same(n->descriptor,&absolute));C(SetSecurityDescriptorOwner(&absolute,runtime,FALSE));C(!personal_first_descriptors_same(n->descriptor,&absolute));
 }
 C(!personal_first_ready(p,0));C(plan_confirm());C(!personal_first_ready(p,0));
 C(first_confirm_model(&model_owner,model_attempt)==PAPACC_RESULT_OK&&personal_first_ready(p,0));
 /* A duplicate confirmation invalidates this attempted reuse. Exercise it
  * in its own packet below; do not restore a rejected act to get PASS. */
 C(papacc_personal_effect_consume(&p->personal_data,model_attempt,p->previous+1024,p->personal_data.bytes));n->first_phase=3;n->first_worker_thread=f_count();
 C(personal_first_ready(p,1));handles=188;C(!personal_first_ready(p,1));
 for(i=0;i<8U;i++){
  setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);C(plan_confirm());
  if(i==0U)p->stop=1;if(i==1U)metadata_fault=1;if(i==2U)source_fault=1;if(i==3U)token_fault=1;
  if(i==4U)n->presenter_phase=1;if(i==5U)n->first_authority[0]^=1;if(i==6U)sd_fault=1;if(i==7U)p->next[0]^=1;
  C(first_confirm_model(&model_owner,model_attempt)!=PAPACC_RESULT_OK);
 }
 for(i=2U;i<=3U;i++){
  setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);C(plan_confirm());metadata_fault=i;
  C(first_confirm_model(&model_owner,model_attempt)!=PAPACC_RESULT_OK&&n->first_error&&!child_calls&&!key_calls);
  metadata_fault=0;C(first_confirm_model(&model_owner,model_attempt)!=PAPACC_RESULT_OK&&!child_calls&&!key_calls);
 }
 for(i=0;i<3U;i++){
  setup();if(i==0U)n->machine_fixture.phase=1;if(i==1U)n->scq_admin.phase=1;if(i==2U)n->install_paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;
  C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")!=PAPACC_RESULT_OK&&!acquisitions&&!rng_calls);
 }
 for(i=1;i<=4U;i++){setup();chain_fault=i;C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")!=PAPACC_RESULT_OK&&acquisitions==i);}
 setup();C(first_open_model(&model_owner,&model_key,"C:\\Other","C:\\Other\\initial.conf")!=PAPACC_RESULT_OK&&!acquisitions);
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","D:\\Elsewhere\\initial.conf")!=PAPACC_RESULT_OK&&!acquisitions);
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\Program Files\\Papinho\\Accelerator\\initial.conf")==PAPACC_RESULT_OK&&acquisitions==7U&&n->first_parent_index==2U);C(plan_confirm());C(first_confirm_model(&model_owner,model_attempt)==PAPACC_RESULT_OK);
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\..")!=PAPACC_RESULT_OK&&!acquisitions);
 setup();model_key.first_preparation=0;C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")!=PAPACC_RESULT_OK&&!acquisitions);
 setup();first_fault=1;C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")!=PAPACC_RESULT_OK&&!parent_calls&&!acquisitions&&!rng_calls);
 for(i=1U;i<=3U;i++){
  setup();parent_fault=i;C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")!=PAPACC_RESULT_OK&&parent_calls==1U&&!acquisitions&&!rng_calls&&!child_calls);
  C(n->scq_cold_attributes.ObjectName==&n->scq_cold_unicode&&n->scq_cold_unicode.Buffer==n->scq_cold_name&&n->diagnostic_first==32U&&n->diagnostic_kind==3U);
  C(n->diagnostic_error==(i==1U?0xc0000022U:i==2U?0x103U:0xc000050bU));
  if(i==2U){C(n->phase==2U&&first_close_model(&model_owner)!=PAPACC_RESULT_OK&&!close_calls&&p->bootstrap_token);}
 }
 {char long_parent[MAX_PATH];size_t length=MAX_PATH+4U-strlen("\\Device\\Model")+2U;
  setup();n->first_owner=p;n->first_key=&model_key;memset(long_parent,'a',length);memcpy(long_parent,"C:\\",3);long_parent[length]=0;
  C(f_open(&model_owner,long_parent)==PAPACC_RESULT_INVALID_ARGUMENT&&!parent_calls); /* exact durable-buffer boundary */
 }
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);
 C(personal_first_child_protected(n->first_child_sd,model_actor));
 child_bytes=GetSecurityDescriptorLength(n->first_child_sd);child=LocalAlloc(LMEM_FIXED,child_bytes);C(child!=NULL);memcpy(child,n->first_child_sd,child_bytes);
 C(SetSecurityDescriptorControl(child,SE_DACL_PROTECTED,0));C(!personal_first_child_protected(child,model_actor));LocalFree(child);child=NULL;
 /* Complete preparation path, no borrowed key, signature or publication. */
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);C(plan_confirm());C(first_confirm_model(&model_owner,model_attempt)==PAPACC_RESULT_OK);
 C(first_target_begin_model(&model_owner,model_attempt,3140000U)==PAPACC_RESULT_OK&&thread_calls==1U&&child_calls==1U&&p->personal_data.consumed);
 C(first_poll_model(&model_owner)==PAPACC_RESULT_OK&&!p->active&&!p->thread&&n->handoff_state==PAPACC_PERSONAL_HANDOFF_VERIFIED);
 C(first_ca_initialize_model(&model_owner,&model_key,model_attempt,n->first_authority,n->first_generation,n->first_service,n->first_service_bytes,3140000U)==0&&key_calls==1U&&p->ca_initialize_phase==2U);
 C(first_ca_initialize_model(&model_owner,&model_key,model_attempt,n->first_authority,n->first_generation,n->first_service,n->first_service_bytes,3140000U)!=0&&key_calls==1U);
 C(first_ca_bind_model(&model_owner,&model_key,model_attempt,n->first_authority,n->first_generation)!=PAPACC_RESULT_OK&&!model_key.borrower&&!p->personal_ca_used);
 C(!model_owner.published&&!model_owner.committed_pending&&!model_owner.committed_bytes);
 C(first_target_begin_model(&model_owner,model_attempt,3140000U)!=PAPACC_RESULT_OK&&thread_calls==1U&&child_calls==1U);
 /* Close only the exact finalized owner; persistent objects are NOT deleted. */
 model_key.key=model_key.provider=0;model_key.state=PAPACC_CA_KEY_CLOSED;model_key.close_confirmed=1;
 C(first_close_model(&model_owner)==PAPACC_RESULT_OK&&n->phase==3U&&!p->file&&!p->bootstrap_parent&&!p->bootstrap_container&&!p->personal_ca_candidate);
 C(closed_handles[500]&&closed_handles[600]&&closed_handles[900]&&closed_handles[700]&&model_key.created&&p->bootstrap_created);
 {unsigned before=close_calls;C(first_close_model(&model_owner)==PAPACC_RESULT_OK&&close_calls==before);}
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);C(plan_confirm());C(first_confirm_model(&model_owner,model_attempt)==PAPACC_RESULT_OK);
 C(first_confirm_model(&model_owner,model_attempt)!=PAPACC_RESULT_OK&&n->first_error);C(first_target_begin_model(&model_owner,model_attempt,3140000U)!=PAPACC_RESULT_OK&&!thread_calls);
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\a\\b\\c\\d\\e\\f\\initial.conf")==PAPACC_RESULT_LIMIT_EXCEEDED&&n->scq_inputs.image_chain.count==8U&&!n->scq_inputs.image_chain.image);
 for(i=0;i<6U;i++){
  setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);C(plan_confirm());C(first_confirm_model(&model_owner,model_attempt)==PAPACC_RESULT_OK);
  if(i==0U)submit_fault=1;if(i>=1U&&i<=3U)native_fault=i;if(i==4U)close_fault=1;if(i==5U)wait_fault=1;
  if(i==0U){C(first_target_begin_model(&model_owner,model_attempt,3140000U)!=PAPACC_RESULT_OK&&p->personal_data.consumed&&!child_calls);}
  else {C(first_target_begin_model(&model_owner,model_attempt,3140000U)==PAPACC_RESULT_OK&&child_calls==1U);(void)first_poll_model(&model_owner);
   if(i==1U)C(p->result==PAPACC_CAS_CONFLICT&&!p->bootstrap_created);
   if(i==2U||i==3U)C(p->result==PAPACC_CAS_UNKNOWN&&n->phase==2U&&!p->bootstrap_created&&n->scq_cold_attributes.ObjectName==&n->scq_cold_unicode);
   if(i>=4U)C(p->active&&p->thread);}
  C(first_target_begin_model(&model_owner,model_attempt,3140000U)!=PAPACC_RESULT_OK&&thread_calls==1U&&child_calls<=1U&&!key_calls);
  if(i>=2U){unsigned before=close_calls;PSECURITY_DESCRIPTOR held=n->first_child_sd;
   C(first_close_model(&model_owner)!=PAPACC_RESULT_OK&&n->first_child_sd==held&&n->scq_inputs.image_chain.image);
   C(close_calls==before);}
 }
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);
 close_fault=1;C(first_close_model(&model_owner)!=PAPACC_RESULT_OK&&model_owner.unresolved&&n->scq_inputs.image_chain.phase==4U);
 {unsigned before=close_calls;close_fault=0;C(first_close_model(&model_owner)!=PAPACC_RESULT_OK&&close_calls==before);}
 setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);C(plan_confirm());C(first_confirm_model(&model_owner,model_attempt)==PAPACC_RESULT_OK);
 native_fault=4;C(first_target_begin_model(&model_owner,model_attempt,3140000U)==PAPACC_RESULT_OK&&!child_calls&&p->personal_data.consumed&&n->first_error==ERROR_TIMEOUT);
 C(first_poll_model(&model_owner)==PAPACC_RESULT_OK&&!key_calls&&n->handoff_state==PAPACC_PERSONAL_HANDOFF_DENIED);
 for(i=1U;i<=2U;i++){
  setup();C(first_open_model(&model_owner,&model_key,"C:\\ProgramData\\Papinho","C:\\ProgramData\\Papinho\\initial.conf")==PAPACC_RESULT_OK);C(plan_confirm());C(first_confirm_model(&model_owner,model_attempt)==PAPACC_RESULT_OK);
  C(first_target_begin_model(&model_owner,model_attempt,3140000U)==PAPACC_RESULT_OK);C(first_poll_model(&model_owner)==PAPACC_RESULT_OK);key_fault=i;
  C(first_ca_initialize_model(&model_owner,&model_key,model_attempt,n->first_authority,n->first_generation,n->first_service,n->first_service_bytes,3140000U)==(i==1U?NTE_EXISTS:NTE_FAIL)&&model_owner.unresolved&&p->bootstrap_created&&key_calls==1U);
  C(first_ca_initialize_model(&model_owner,&model_key,model_attempt,n->first_authority,n->first_generation,n->first_service,n->first_service_bytes,3140000U)!=0&&key_calls==1U);
 }
#endif
 (void)i;(void)child;(void)child_bytes;dispose();LocalFree(model_actor);LocalFree(runtime);LocalFree(descriptor);
 printf("FCA_ORIGINAL_PACKET_MODEL checks=%u; NO_NATIVE_FILE_CNG_RESOURCE_OR_SCM_EFFECTS; NOT_AUTHORITY\n",checks);return 0;
}
