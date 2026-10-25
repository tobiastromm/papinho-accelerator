/* Private adapter for PS_ADMISSION. explicit N0 LAB entries only; no new owner. */
#include <bcrypt.h>
#include "scm-n0-context-model.h"
#include "scm-n0-context-pins.h"
#define ig_n0 native.admission.bd.context
static const WCHAR ig_n0_dossier_path[]=L"C:\\ProgramData\\PapinhoAccelerator\\Protected\\N0\\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.approved";
static const WCHAR ig_n0_evidence_path[]=L"C:\\ProgramData\\PapinhoAccelerator\\Protected\\N0\\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.evidence";
static const WCHAR ig_n0_os_path[]=L"C:\\Windows\\System32\\conhost.exe";
static const WCHAR *ig_n0_policies[]={
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\adr\\DECISION-first-native-lab-diagnostic-boundary-20261024.md",
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\adr\\DECISION-canonical-lab-console-topology-bd-20261023.md",
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\adr\\DECISION-ed11-single-lab-diagnostic-native-boundary-20261023.md",
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\adr\\DECISION-n0-n9-security-descriptor-digest-profile-20261024.md"};
static const char *ig_n0_policy_hashes[]={PS_BD_N0_POLICY,"eb93002cf3f6d863a060fb97b7380389aa66b6fbe68e66c1af4fb17b988d3290","e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7","8b042e2bbf6500aa90aa3407fcd3423f45d96cf6b16568ffa95b98f3d081c972"};
static void ig_n0_hex(const BYTE bytes[32],char out[65])
{unsigned i;for(i=0;i<32;i++){out[2*i]="0123456789abcdef"[bytes[i]>>4];out[2*i+1]="0123456789abcdef"[bytes[i]&15];}out[64]=0;}
static DWORD ig_n0_plan(const PS_BD_DESCRIPTOR *d,char out[65])
{
 BYTE digest[32];char text[2304];int n;
 n=sprintf_s(text,sizeof(text),
 "%s;%s;%s;%s;%s;%s;%llu;%lu;%llu;%lu;%llu;%lu;%lu;%lu:%lu:%lu:%lu:%lu:%lu;%llu;"
 "config=%lu:%lu:%lu:%lu:%lu;image=%lu:%lu:%lu:%lu:%lu;aux=%lu:%llu:%lu:%lu;file=%lu:%lu:%lu:%lu:%lu:%lu;%s;%s;%s;"
 "APPROVAL=%s;INPUT_PROFILE=N2-SD-RAW1-OWNER-GROUP-DACL;INPUTS=417614100:131072:2841430:326,417614100:196608:2841429:5358;PLACEMENT=N2P-20261024-02;SERVICE=PapinhoAcceleratorLab20261023;SID=S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451;OPERATOR=S-1-5-21-785699550-1440458426-1682014700-1108;ASSOCIATION=UNPROVEN;CREDITS=NONE_GRANTED",
 ig_n0_positive?"N0START-20261024-01;CREATE_IMAGE;CREATE_SINK;REBIND;START1;STOP_MAX1":"N0_READ_ONLY",d->attempt,d->config_hash,d->image_hash,d->policy,d->host,d->boot,(unsigned long)d->owner_pid,d->owner_creation,
 (unsigned long)d->actor.pid,d->actor.creation,(unsigned long)d->actor.session,(unsigned long)d->control_tid,
 (unsigned long)d->statistics.TokenId.LowPart,(unsigned long)d->statistics.TokenId.HighPart,
 (unsigned long)d->statistics.AuthenticationId.LowPart,(unsigned long)d->statistics.AuthenticationId.HighPart,
 (unsigned long)d->statistics.ModifiedId.LowPart,(unsigned long)d->statistics.ModifiedId.HighPart,d->challenge,
 (unsigned long)d->config_id.dwVolumeSerialNumber,(unsigned long)d->config_id.nFileIndexHigh,(unsigned long)d->config_id.nFileIndexLow,(unsigned long)d->config_id.nFileSizeHigh,(unsigned long)d->config_id.nFileSizeLow,
 (unsigned long)d->image_id.dwVolumeSerialNumber,(unsigned long)d->image_id.nFileIndexHigh,(unsigned long)d->image_id.nFileIndexLow,(unsigned long)d->image_id.nFileSizeHigh,(unsigned long)d->image_id.nFileSizeLow,
 (unsigned long)d->auxiliary.instance.pid,d->auxiliary.instance.creation,(unsigned long)d->auxiliary.instance.session,(unsigned long)d->auxiliary.parent,
 (unsigned long)d->auxiliary.file_id.dwVolumeSerialNumber,(unsigned long)d->auxiliary.file_id.nFileIndexHigh,(unsigned long)d->auxiliary.file_id.nFileIndexLow,(unsigned long)d->auxiliary.file_id.nFileSizeHigh,(unsigned long)d->auxiliary.file_id.nFileSizeLow,(unsigned long)d->auxiliary.file_id.nNumberOfLinks,
 d->auxiliary.file_hash,d->auxiliary.security_hash,d->auxiliary.provenance_hash,ig_n0_positive?ig_n0_approval_sha:"NONE");
 if(n<=0||!pacc_storage_hash_callback(NULL,(const BYTE *)text,(size_t)n,digest))return ERROR_INVALID_DATA;ig_n0_hex(digest,out);return 0;
}
static DWORD ig_n0_sd(HANDLE file,const char *expected,int protect)
{
 BYTE digest[32];char hash[65];DWORD e,n,revision;SECURITY_DESCRIPTOR_CONTROL control;int ok;
 if(ig_n0.sd)return ERROR_INVALID_STATE;
 e=GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&ig_n0.sd);if(e)return e;
 n=GetSecurityDescriptorLength(ig_n0.sd);ok=n&&n<=65536&&IsValidSecurityDescriptor(ig_n0.sd)&&GetSecurityDescriptorControl(ig_n0.sd,&control,&revision)&&revision==1&&(control&SE_SELF_RELATIVE)&&(!protect||scm_descriptor(ig_n0.sd,(BYTE *)lab_sid));
 if(ok&&expected){ok=pacc_storage_hash_callback(NULL,(BYTE *)ig_n0.sd,n,digest);if(ok){ig_n0_hex(digest,hash);ok=!strcmp(hash,expected);}}
 if(LocalFree(ig_n0.sd)){native.retained=1;return ERROR_INVALID_HANDLE;}ig_n0.sd=NULL;return ok?0:ERROR_ACCESS_DENIED;
}
static DWORD ig_n0_open(const WCHAR *path,HANDLE *out,BY_HANDLE_FILE_INFORMATION *id,DWORD cap,int windows_file)
{
 WCHAR actual[1100];DWORD n,flags;*out=CreateFileW(path,GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(*out==INVALID_HANDLE_VALUE){*out=NULL;return lab_error();}
 if(!GetHandleInformation(*out,&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(*out)!=FILE_TYPE_DISK||!GetFileInformationByHandle(*out,id)||id->nFileSizeHigh||!id->nFileSizeLow||id->nFileSizeLow>cap||(!windows_file&&id->nNumberOfLinks!=1)||!id->nNumberOfLinks||(id->dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))return ERROR_INVALID_DATA;
 n=GetFinalPathNameByHandleW(*out,actual,1100,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
 if(n<4||n>=1100||wcsncmp(actual,L"\\\\?\\",4)||_wcsicmp(actual+4,path))return ERROR_INVALID_DATA;
 return 0;
}

#define IG_N0_INPUT_SD_SHA "0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814"
static const WCHAR *ig_n0_roots[]={L"C:\\",L"C:\\ProgramData",L"C:\\ProgramData\\PapinhoAccelerator",L"C:\\ProgramData\\PapinhoAccelerator\\Protected",L"C:\\ProgramData\\PapinhoAccelerator\\Protected\\N0"};
static const char *ig_n0_root_hashes[]={"9205e0643299861cdba58173a4e9e01db5a3bc63c4d8c5538bc2292dae214fc8","c0811643edc9703aa06642dfe053b7a0c3062cbaba03b6b169bde6962248f6a6","688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579","688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579","688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579"};
static DWORD ig_n0_roots_current(int acquire)
{
 unsigned i;DWORD e,n,flags,serial,component;BY_HANDLE_FILE_INFORMATION id;WCHAR actual[1100],filesystem[32];
 if(!GetVolumeInformationW(L"C:\\",NULL,0,&serial,&component,&flags,filesystem,32))return lab_error();
 if(!ig_n0_volume_profile(serial,flags,filesystem))return ERROR_INVALID_DATA;
 for(i=0;i<5;i++){
  if(acquire){if(ig_n0.input_root[i])return ERROR_INVALID_STATE;
   ig_n0.input_root[i]=CreateFileW(ig_n0_roots[i],READ_CONTROL|FILE_READ_ATTRIBUTES|SYNCHRONIZE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
   if(ig_n0.input_root[i]==INVALID_HANDLE_VALUE){ig_n0.input_root[i]=NULL;return lab_error();}}
  if(!ig_n0.input_root[i]||!GetHandleInformation(ig_n0.input_root[i],&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(ig_n0.input_root[i])!=FILE_TYPE_DISK||!GetFileInformationByHandle(ig_n0.input_root[i],&id)||!ig_n0_root_identity(&id,i))return ERROR_INVALID_DATA;
  n=GetFinalPathNameByHandleW(ig_n0.input_root[i],actual,1100,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
  if(n<4||n>=1100||wcsncmp(actual,L"\\\\?\\",4)||_wcsicmp(actual+4,ig_n0_roots[i]))return ERROR_INVALID_DATA;
  if((e=ig_n0_sd(ig_n0.input_root[i],ig_n0_root_hashes[i],0)))return e;
 }
 return 0;
}
static DWORD ig_n0_current(const PS_BD_DESCRIPTOR *d)
{
 DWORD e,i;BY_HANDLE_FILE_INFORMATION id;SERVICE_STATUS_PROCESS status;char plan[65];ULONGLONG now=GetTickCount64();
 if(!ig_n0.prepared||!ig_n0_fresh(now,ig_n0.began,60000ULL)||!ps_bd_hex(IG_N0_DOSSIER_SHA)||!ps_bd_hex(IG_N0_EVIDENCE_SHA))return ERROR_ACCESS_DENIED;
 if((e=ig_n0_plan(d,plan))||strcmp(plan,ig_n0.plan)||strcmp(d->plan,ig_n0.plan)||strcmp(d->auxiliary.file_hash,ig_n0.record.hash)||strcmp(d->auxiliary.security_hash,ig_n0.record.security)||strcmp(d->auxiliary.provenance_hash,IG_N0_DOSSIER_SHA))return e?e:ERROR_INVALID_DATA;
 if(!ig_n0_input_identity(&ig_n0.dossier_id,0)||!ig_n0_input_identity(&ig_n0.evidence_id,1))return ERROR_INVALID_DATA;
 if((e=ig_n0_roots_current(0)))return e;
 if(!GetFileInformationByHandle(ig_n0.dossier,&id)||!psa_bd_file(&id,&ig_n0.dossier_id)||!GetFileInformationByHandle(ig_n0.evidence,&id)||!psa_bd_file(&id,&ig_n0.evidence_id)||
 !GetFileInformationByHandle(d->auxiliary.file,&id)||!psa_bd_aux_file(&id,&ig_n0.os_id))return ERROR_INVALID_DATA;
 if((e=lab_operator_current())||(e=lab_bound(1))||(e=cycle_dependencies_current(0))||(e=lab_hash(cycle_source_file,cycle_sha))||
 (e=lab_hash(ig_n0.dossier,IG_N0_DOSSIER_SHA))||(e=lab_hash(ig_n0.evidence,IG_N0_EVIDENCE_SHA))||(e=ig_n0_sd(ig_n0.dossier,IG_N0_INPUT_SD_SHA,1))||(e=ig_n0_sd(ig_n0.evidence,IG_N0_INPUT_SD_SHA,1))||
 (e=lab_hash(d->auxiliary.file,ig_n0.record.hash))||(e=ig_n0_sd(d->auxiliary.file,ig_n0.record.security,0)))return e;
 for(i=0;i<4;i++)if((e=lab_hash(ig_n0.policy[i],ig_n0_policy_hashes[i])))return e;
 if((e=cycle_readback(SERVICE_DEMAND_START,ig_n0_rebound))||(e=cycle_status(&status)))return e;
 if(ig_n0_positive&&((e=ig_recovery_readback())||(e=lab_hash(ig_approval_file,ig_n0_approval_sha))))return e;
 if(!cycle_start_requested&&(status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId))return ERROR_INVALID_STATE;
 if(cycle_start_requested&&status.dwCurrentState!=SERVICE_START_PENDING&&status.dwCurrentState!=SERVICE_RUNNING&&status.dwCurrentState!=SERVICE_STOP_PENDING&&status.dwCurrentState!=SERVICE_STOPPED)return ERROR_INVALID_STATE;
 {static const DWORD sid[]={80UL,3567404667UL,26456103UL,3018599297UL,139793421UL,3962028451UL};
 if(!IsValidSid(native.service_sid)||*GetSidSubAuthorityCount(native.service_sid)!=6)return ERROR_INVALID_SID;
 for(i=0;i<6;i++)if(*GetSidSubAuthority(native.service_sid,i)!=sid[i])return ERROR_INVALID_SID;}
 return papacc_server_console_win32_stop_requested()?ERROR_OPERATION_ABORTED:0;
}
static DWORD ig_n0_close(void)
{
 HANDLE *v[]={&ig_n0.actor_token,&ig_n0.control,&ig_n0.actor,&ig_n0.auxiliary,&ig_n0.osfile,&ig_n0.dossier,&ig_n0.evidence,&ig_n0.policy[0],&ig_n0.policy[1],&ig_n0.policy[2],&ig_n0.policy[3],&ig_n0.input_root[4],&ig_n0.input_root[3],&ig_n0.input_root[2],&ig_n0.input_root[1],&ig_n0.input_root[0]};unsigned i;
 if(native.retained||native.admission.phase==3)return ERROR_INVALID_STATE;
 for(i=0;i<sizeof(v)/sizeof(v[0]);i++)if(*v[i]){if(!CloseHandle(*v[i])){native.retained=1;return ERROR_INVALID_HANDLE;}*v[i]=NULL;}
 if(ig_n0.sd){if(LocalFree(ig_n0.sd)){native.retained=1;return ERROR_INVALID_HANDLE;}ig_n0.sd=NULL;}ig_n0.prepared=0;return 0;
}
static DWORD ig_n0_bind(DWORD actor,ULONGLONG birth,DWORD tid,DWORD auxiliary,ULONGLONG auxiliary_birth)
{
 DWORD e,n,i;FILETIME ct,et,kt,ut;BY_HANDLE_FILE_INFORMATION id;PS_BD_DESCRIPTOR *d=&ig_n0.descriptor;char text[512];LARGE_INTEGER zero;
 /* No arbitrary argv hash/boolean may substitute separately reviewed provenance. */
 if(!ps_bd_hex(IG_N0_DOSSIER_SHA)||!ps_bd_hex(IG_N0_EVIDENCE_SHA)){puts("IG_N0_PROVENANCE=UNKNOWN; QUALIFIED_DOSSIER_PIN_MISSING; DENY");return ERROR_NOT_SUPPORTED;}
 if(ig_n0.prepared||ig_n0.dossier||ig_n0.evidence||ig_n0.auxiliary||ig_n0.actor||native.admission.bd.used)return ERROR_INVALID_STATE;
 ig_n0.began=GetTickCount64();ZeroMemory(d,sizeof(*d));
 if((e=ig_n0_roots_current(1)))return e;
 if((e=ig_n0_open(ig_n0_dossier_path,&ig_n0.dossier,&ig_n0.dossier_id,511,0))||(e=lab_hash(ig_n0.dossier,IG_N0_DOSSIER_SHA))||(e=ig_n0_sd(ig_n0.dossier,IG_N0_INPUT_SD_SHA,1)))return e;
 if(!ig_n0_input_identity(&ig_n0.dossier_id,0))return ERROR_INVALID_DATA;
 zero.QuadPart=0;if(!SetFilePointerEx(ig_n0.dossier,zero,NULL,FILE_BEGIN)||!ReadFile(ig_n0.dossier,text,ig_n0.dossier_id.nFileSizeLow,&n,NULL)||n!=ig_n0.dossier_id.nFileSizeLow)return lab_error();text[n]=0;
 if(!ig_n0_parse(text,n,&ig_n0.record)||strcmp(ig_n0.record.evidence,IG_N0_EVIDENCE_SHA))return ERROR_INVALID_DATA;
 if((e=ig_n0_open(ig_n0_evidence_path,&ig_n0.evidence,&ig_n0.evidence_id,16384,0))||(e=lab_hash(ig_n0.evidence,IG_N0_EVIDENCE_SHA))||(e=ig_n0_sd(ig_n0.evidence,IG_N0_INPUT_SD_SHA,1))||
 (e=ig_n0_open(ig_n0_os_path,&ig_n0.osfile,&ig_n0.os_id,16777216,1)))return e;
 if(!ig_n0_input_identity(&ig_n0.evidence_id,1))return ERROR_INVALID_DATA;
 if(ig_n0.os_id.dwVolumeSerialNumber!=ig_n0.record.volume||ig_n0.os_id.nFileIndexHigh!=ig_n0.record.high||ig_n0.os_id.nFileIndexLow!=ig_n0.record.low||ig_n0.os_id.nFileSizeLow!=ig_n0.record.size||ig_n0.os_id.nNumberOfLinks!=ig_n0.record.links)return ERROR_INVALID_DATA;
 if((e=lab_hash(ig_n0.osfile,ig_n0.record.hash))||(e=ig_n0_sd(ig_n0.osfile,ig_n0.record.security,0)))return e;
 for(i=0;i<4;i++)if((e=ig_n0_open(ig_n0_policies[i],&ig_n0.policy[i],&id,65536,0))||(e=lab_hash(ig_n0.policy[i],ig_n0_policy_hashes[i])))return e;
 ig_n0.actor=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,actor);if(!ig_n0.actor)return lab_error();
 ig_n0.control=OpenThread(THREAD_QUERY_INFORMATION|SYNCHRONIZE,FALSE,tid);if(!ig_n0.control)return lab_error();
 ig_n0.auxiliary=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|SYNCHRONIZE,FALSE,auxiliary);if(!ig_n0.auxiliary)return lab_error();
 d->actor.handle=ig_n0.actor;d->actor.pid=actor;d->actor.creation=birth;d->control_thread=ig_n0.control;d->control_tid=tid;
 if(!ProcessIdToSessionId(actor,&d->actor.session)||!GetProcessTimes(GetCurrentProcess(),&ct,&et,&kt,&ut))return lab_error();
 d->owner_pid=GetCurrentProcessId();d->owner_creation=((ULONGLONG)ct.dwHighDateTime<<32)|ct.dwLowDateTime;
 if(!OpenProcessToken(ig_n0.actor,TOKEN_QUERY,&ig_n0.actor_token)||!GetTokenInformation(ig_n0.actor_token,TokenStatistics,&d->statistics,sizeof(d->statistics),&n)||n!=sizeof(d->statistics))return lab_error();
 d->sid_size=GetLengthSid(lab_sid);if(d->sid_size>sizeof(d->sid)||!CopySid(sizeof(d->sid),d->sid,lab_sid))return ERROR_INVALID_SID;
 d->configuration=lab_config;d->image=cycle_source_file;if(!GetFileInformationByHandle(lab_config,&d->config_id)||!GetFileInformationByHandle(cycle_source_file,&d->image_id))return lab_error();
 strcpy_s(d->attempt,sizeof(d->attempt),IG_ATTEMPT);strcpy_s(d->config_hash,sizeof(d->config_hash),"a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44");strcpy_s(d->image_hash,sizeof(d->image_hash),cycle_sha);
 strcpy_s(d->policy,sizeof(d->policy),PS_BD_N0_POLICY);strcpy_s(d->host,sizeof(d->host),"2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff");d->boot=236441ULL;d->n0=1;
 if(BCryptGenRandom(NULL,(BYTE *)&d->challenge,sizeof(d->challenge),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0||!d->challenge)return ERROR_GEN_FAILURE;
 d->auxiliary.instance.handle=ig_n0.auxiliary;d->auxiliary.instance.pid=auxiliary;d->auxiliary.instance.creation=auxiliary_birth;d->auxiliary.instance.session=d->actor.session;
 d->auxiliary.parent=actor;d->auxiliary.file=ig_n0.osfile;d->auxiliary.file_id=ig_n0.os_id;
 strcpy_s(d->auxiliary.file_hash,sizeof(d->auxiliary.file_hash),ig_n0.record.hash);strcpy_s(d->auxiliary.security_hash,sizeof(d->auxiliary.security_hash),ig_n0.record.security);strcpy_s(d->auxiliary.provenance_hash,sizeof(d->auxiliary.provenance_hash),IG_N0_DOSSIER_SHA);
 if((e=ig_n0_plan(d,ig_n0.plan)))return e;strcpy_s(d->plan,sizeof(d->plan),ig_n0.plan);ig_n0.prepared=1;
 e=papacc_server_service_admission_bd_bind(d,ig_n0_current);if(e)return e;
 /* Canonical owner now holds the original objects through noninheritable
  * duplicates. End adapter acquisition borrows; do not retain extra copies. */
 {HANDLE *refs[]={&ig_n0.actor_token,&ig_n0.control,&ig_n0.actor,&ig_n0.auxiliary,&ig_n0.osfile};
 for(i=0;i<sizeof(refs)/sizeof(refs[0]);i++){if(!CloseHandle(*refs[i])){native.retained=1;return ERROR_INVALID_HANDLE;}*refs[i]=NULL;}}
 return 0;
}
