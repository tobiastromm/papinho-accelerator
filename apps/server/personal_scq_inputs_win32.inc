#include "personal_scq_inputs_win32.h"
#include <sddl.h>
#include "personal_scq_handle_policy_win32.h"
#include "personal_scq_profile_win32.inc"
/* Same Win32 reference-custody pattern used by runtime binding. This module
 * does not open caller paths, print file contents, confirm a plan or authorize
 * dispatch. The original controller must separately prove provenance. */
static DWORD personal_scq_inputs_fail(PAPACC_PERSONAL_SCQ_INPUTS *c,DWORD stage,DWORD error)
{if(!c->first){c->first=stage;c->error=error;}return error?error:ERROR_ACCESS_DENIED;}
static int personal_scq_inputs_same(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{
 return !(a->dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))&&a->nNumberOfLinks==1U&&
 !(b->dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))&&b->nNumberOfLinks==1U&&
 a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow&&
 a->nFileSizeHigh==b->nFileSizeHigh&&a->nFileSizeLow==b->nFileSizeLow;
}
static int personal_scq_inputs_sd_policy(PSECURITY_DESCRIPTOR sd,PSID actor,int protected_to_image)
{
 PSID owner;PACL acl;BOOL present,def;SECURITY_DESCRIPTOR_CONTROL control;DWORD revision;unsigned i;
 if(!sd||!IsValidSecurityDescriptor(sd)||!GetSecurityDescriptorControl(sd,&control,&revision)||!(control&SE_DACL_PROTECTED)||
 !GetSecurityDescriptorOwner(sd,&owner,&def)||!owner||
 (!EqualSid(owner,actor)&&!(protected_to_image&&IsWellKnownSid(owner,WinBuiltinAdministratorsSid)))||
 !GetSecurityDescriptorDacl(sd,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl)||!acl->AceCount)return 0;
 for(i=0;i<acl->AceCount;i++){
  ACCESS_ALLOWED_ACE *ace;PSID sid;DWORD n;
  if(!GetAce(acl,i,(void **)&ace)||ace->Header.AceType!=ACCESS_ALLOWED_ACE_TYPE||ace->Header.AceFlags||
   ace->Header.AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U)return 0;
  sid=(PSID)&ace->SidStart;n=8U+4U*((BYTE *)sid)[1];
  if(((BYTE *)sid)[1]>SID_MAX_SUB_AUTHORITIES||n>(DWORD)ace->Header.AceSize-(DWORD)FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)||!IsValidSid(sid))return 0;
  if((ace->Mask&(GENERIC_ALL|GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|FILE_DELETE_CHILD))&&
   !EqualSid(sid,actor)&&!IsWellKnownSid(sid,WinLocalSystemSid)&&!IsWellKnownSid(sid,WinBuiltinAdministratorsSid))return 0;
 }
 return 1; /* narrow metadata profile only; not a universal RAW1 proof */
}
/* K6/L0-L6 specialized only to two NEW MACHINE-READ originals. Exact
 * no-inheritance operator/SY/BA full + selected virtual SID read, no generic
 * descriptor equivalence or authority inferred from four ACEs. */
static int personal_scq_inputs_machine_read_sd(PSECURITY_DESCRIPTOR sd,PSID actor,const PAPACC_SCQ_PLAN_DATA *plan)
{
 PSID owner,runtime;PACL acl;BOOL present,def;DWORD revision,i,seen=0,bit,mask;SECURITY_DESCRIPTOR_CONTROL control;
 if(!plan||plan->purpose!=PAPACC_SCQ_PURPOSE_READ||plan->runtime.sid_bytes<8U||plan->runtime.sid_bytes>68U)return 0;
 runtime=(PSID)plan->runtime.sid;
 if(!IsValidSid(runtime)||GetLengthSid(runtime)!=plan->runtime.sid_bytes||EqualSid(actor,runtime)||
  IsWellKnownSid(actor,WinLocalSystemSid)||IsWellKnownSid(actor,WinBuiltinAdministratorsSid)||
  !sd||!IsValidSecurityDescriptor(sd)||!GetSecurityDescriptorControl(sd,&control,&revision)||!(control&SE_DACL_PROTECTED)||
  !GetSecurityDescriptorOwner(sd,&owner,&def)||!owner||!EqualSid(owner,actor)||
  !GetSecurityDescriptorDacl(sd,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl)||acl->AceCount!=4U)return 0;
 for(i=0;i<4U;i++){
  ACCESS_ALLOWED_ACE *ace;PSID sid;DWORD n;
  if(!GetAce(acl,i,(void **)&ace)||ace->Header.AceType!=ACCESS_ALLOWED_ACE_TYPE||ace->Header.AceFlags||
   ace->Header.AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U)return 0;
  sid=&ace->SidStart;n=8U+4U*((BYTE *)sid)[1];
  if(((BYTE *)sid)[1]>SID_MAX_SUB_AUTHORITIES||n>(DWORD)ace->Header.AceSize-(DWORD)FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)||!IsValidSid(sid))return 0;
  mask=FILE_ALL_ACCESS;
  if(EqualSid(sid,actor))bit=1U;else if(IsWellKnownSid(sid,WinLocalSystemSid))bit=2U;
  else if(IsWellKnownSid(sid,WinBuiltinAdministratorsSid))bit=4U;else if(EqualSid(sid,runtime)){bit=8U;mask=PAPACC_SCQ_FILE_READ_RIGHTS;}else return 0;
  if((seen&bit)||ace->Mask!=mask)return 0;seen|=bit;
 }
 return seen==15U;
}
static int personal_scq_inputs_protected(PSECURITY_DESCRIPTOR sd,PSID actor)
{return personal_scq_inputs_sd_policy(sd,actor,0);}
#include "personal_scq_native_name_win32.inc"
#include "personal_scq_native_sd_win32.inc"
#include "personal_scq_image_chain_win32.inc"
static DWORD personal_scq_inputs_check(PAPACC_PERSONAL_SCQ_INPUTS *c,PSID actor,volatile LONG *stop,int capture)
{
 BY_HANDLE_FILE_INFORMATION info;unsigned i;DWORD e;int same;
 if(!c||c->first||c->thread!=GetCurrentThreadId()||c->phase!=1U||c->checking||!actor||!IsValidSid(actor)||!stop)return ERROR_INVALID_STATE;
 if(c->image_chain.phase&&scqi_chain_current(&c->image_chain,actor,stop))return personal_scq_inputs_fail(c,26,ERROR_ACCESS_DENIED);
 for(i=0;i<3;i++){
  if(InterlockedCompareExchange(stop,0,0))return personal_scq_inputs_fail(c,1,ERROR_CANCELLED);
  if(!c->file[i]||GetFileType(c->file[i])!=FILE_TYPE_DISK||!GetFileInformationByHandle(c->file[i],&info)||
   !personal_scq_inputs_same(&c->identity[i],&info))return personal_scq_inputs_fail(c,2,ERROR_INVALID_DATA);
  e=c->image_chain.phase==2U&&PAPACC_SCQ_EXISTING_PATH_PURPOSE(c->image_chain.paths,c->image_chain.plan.purpose)&&
   c->image_chain.paths&&c->image_chain.paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT?
   scqi_native_file_sd(c->file[i],&c->checking,&c->native_sd,stop):
   GetSecurityInfo(c->file[i],SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&c->checking);
  if(e||!c->checking){if(e&&c->checking)c->phase=4;return personal_scq_inputs_fail(c,3,e?e:ERROR_INVALID_SECURITY_DESCR);}
  same=i==2U&&c->image_chain.phase==2U&&PAPACC_SCQ_EXISTING_PATH_PURPOSE(c->image_chain.paths,c->image_chain.plan.purpose)?
   personal_scq_inputs_sd_policy(c->checking,actor,1):personal_scq_inputs_protected(c->checking,actor);
  if(i<2U&&c->image_chain.phase==2U&&c->image_chain.plan.purpose==PAPACC_SCQ_PURPOSE_READ)
   same=same&&personal_scq_inputs_machine_read_sd(c->checking,actor,&c->image_chain.plan);
  if(capture&&same){c->descriptor[i]=c->checking;c->descriptor_bytes[i]=GetSecurityDescriptorLength(c->checking);c->checking=NULL;}
  else if(!capture)same=same&&c->descriptor[i]&&c->descriptor_bytes[i]==GetSecurityDescriptorLength(c->checking)&&
   !memcmp(c->descriptor[i],c->checking,c->descriptor_bytes[i]);
  if(c->checking){if(LocalFree(c->checking)){c->phase=4;return personal_scq_inputs_fail(c,4,ERROR_INVALID_HANDLE);}c->checking=NULL;}
  if(!same)return personal_scq_inputs_fail(c,5,ERROR_ACCESS_DENIED);
 }
 return 0;
}
static DWORD personal_scq_inputs_capture_mode(PAPACC_PERSONAL_SCQ_INPUTS *c,const HANDLE originals[3],PSID actor,
 const PAPACC_SCQ_PLAN_DATA *transport_plan,const PAPACC_PERSONAL_INSTALL_PATHS *paths,const void *owner,volatile LONG *stop)
{
 unsigned i,j;DWORD own;BY_HANDLE_FILE_INFORMATION before;
 if(!c||c->phase||!originals||!actor||!IsValidSid(actor)||!stop)return ERROR_INVALID_STATE;
 if(c->image_chain.phase&&(!transport_plan||!paths||paths->mode!=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT||
  paths->selected_role!=1U||c->image_chain.phase!=2U||c->image_chain.owner!=owner||c->image_chain.paths!=paths))return ERROR_INVALID_STATE;
 if(transport_plan&&(!owner||!paths||!PAPACC_SCQ_EXISTING_PATH_PURPOSE(paths,transport_plan->purpose)||
  !scqp_static(transport_plan)||!scqp_window(transport_plan)))return ERROR_ACCESS_DENIED;
 c->thread=GetCurrentThreadId();c->phase=1;
 for(i=0;i<3;i++){
  if(InterlockedCompareExchange(stop,0,0))return personal_scq_inputs_fail(c,1,ERROR_CANCELLED);
  if(transport_plan){
   if(!GetProcessHandleCount(GetCurrentProcess(),&own))return personal_scq_inputs_fail(c,30,GetLastError());
   if(own>=PAPACC_SCQ_HANDLE_GUARD(PAPACC_SCQ_P1_PATHS(paths)))return personal_scq_inputs_fail(c,30,ERROR_NOT_ENOUGH_QUOTA);
  }
  if(!originals[i]||originals[i]==INVALID_HANDLE_VALUE||GetFileType(originals[i])!=FILE_TYPE_DISK||
   !GetFileInformationByHandle(originals[i],&before))return personal_scq_inputs_fail(c,6,ERROR_INVALID_HANDLE);
  if(!DuplicateHandle(GetCurrentProcess(),originals[i],GetCurrentProcess(),&c->file[i],GENERIC_READ|READ_CONTROL,FALSE,0)){
   if(c->file[i])c->phase=4;return personal_scq_inputs_fail(c,7,GetLastError());}
  if(!c->file[i])return personal_scq_inputs_fail(c,7,ERROR_INVALID_HANDLE);
  if(!GetFileInformationByHandle(c->file[i],&c->identity[i])||!personal_scq_inputs_same(&before,&c->identity[i]))return personal_scq_inputs_fail(c,8,ERROR_INVALID_DATA);
  for(j=0;j<i;j++)if(c->identity[j].dwVolumeSerialNumber==c->identity[i].dwVolumeSerialNumber&&
   c->identity[j].nFileIndexHigh==c->identity[i].nFileIndexHigh&&c->identity[j].nFileIndexLow==c->identity[i].nFileIndexLow)
   return personal_scq_inputs_fail(c,9,ERROR_INVALID_DATA);
 }
 if(transport_plan){DWORD e;
  if(transport_plan->refs[2].volume!=c->identity[2].dwVolumeSerialNumber||transport_plan->refs[2].high!=c->identity[2].nFileIndexHigh||
   transport_plan->refs[2].low!=c->identity[2].nFileIndexLow)return personal_scq_inputs_fail(c,27,ERROR_INVALID_DATA);
  e=c->image_chain.phase?scqi_chain_current(&c->image_chain,actor,stop):scqi_chain_capture(&c->image_chain,owner,paths,transport_plan,&c->identity[2],actor,stop);
  if(!e&&paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT&&memcmp(c->image_chain.plan.target,transport_plan->target,32))e=ERROR_CRC;
  if(e){if(c->image_chain.phase==4U)c->phase=4U;return personal_scq_inputs_fail(c,28,e);}
 }
 return personal_scq_inputs_check(c,actor,stop,1);
}
static DWORD personal_scq_inputs_capture(PAPACC_PERSONAL_SCQ_INPUTS *c,const HANDLE originals[3],PSID actor,volatile LONG *stop)
{return personal_scq_inputs_capture_mode(c,originals,actor,NULL,NULL,NULL,stop);}
static DWORD personal_scq_inputs_capture_transport(PAPACC_PERSONAL_SCQ_INPUTS *c,const HANDLE originals[3],PSID actor,
 const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_PERSONAL_INSTALL_PATHS *paths,const void *owner,volatile LONG *stop)
{if(!plan||plan->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)return ERROR_INVALID_PARAMETER;return personal_scq_inputs_capture_mode(c,originals,actor,plan,paths,owner,stop);}
/* Explicit K3 custody entry; never inferred from READ purpose alone. */
static DWORD personal_scq_inputs_capture_machine_read(PAPACC_PERSONAL_SCQ_INPUTS *c,const HANDLE originals[3],PSID actor,
 const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_PERSONAL_INSTALL_PATHS *paths,const void *owner,volatile LONG *stop)
{if(!plan||plan->purpose!=PAPACC_SCQ_PURPOSE_READ||!paths||paths->read_diagnostic!=1U)return ERROR_ACCESS_DENIED;
 return personal_scq_inputs_capture_mode(c,originals,actor,plan,paths,owner,stop);}
/* ReOpenFile binds to the retained original object, not a caller pathname.
 * Synchronous readers have private cursors and deny concurrent write/delete.
 * All three remain owned until terminal close. Content pins are necessary
 * evidence, never the operational decision or its attributed confirmation. */
static DWORD personal_scq_contents_cleanup(PAPACC_PERSONAL_SCQ_INPUTS *c)
{
 NTSTATUS s;
 if(c->content_hash.hash){s=BCryptDestroyHash(c->content_hash.hash);if(s){c->content_hash.cleanup_status=s;goto unknown;}c->content_hash.hash=NULL;}
 if(c->content_hash.algorithm){s=BCryptCloseAlgorithmProvider(c->content_hash.algorithm,0);if(s){c->content_hash.cleanup_status=s;goto unknown;}c->content_hash.algorithm=NULL;}
 InterlockedExchange(&c->content_hash.active,0);return 0;
unknown:c->content_hash.retained=1;c->phase=4;InterlockedExchange(&c->content_hash.active,0);return personal_scq_inputs_fail(c,12,ERROR_INVALID_HANDLE);
}
static DWORD personal_scq_inputs_contents(PAPACC_PERSONAL_SCQ_INPUTS *c,PSID actor,
 const PAPACC_SCQ_REF_DATA expected[3],ULONGLONG deadline,volatile LONG *stop)
{
 BYTE chunk[4096],value[32];BY_HANDLE_FILE_INFORMATION info;ULONGLONG remaining;
 DWORD n,e=0;NTSTATUS status;unsigned i;const PAPACC_SCQ_REF_DATA *r;
 if(!c||!expected||!stop||c->content_verified||c->content_hash.retained||c->content_hash.active||c->content_hash.hash||c->content_hash.algorithm)return ERROR_INVALID_STATE;
 if(c->image_chain.phase&&memcmp(expected,c->image_chain.plan.refs,sizeof(c->image_chain.plan.refs)))return personal_scq_inputs_fail(c,29,ERROR_INVALID_DATA);
 if(personal_scq_inputs_check(c,actor,stop,0))return ERROR_INVALID_STATE;
 for(i=0;i<3;i++){
  r=&expected[i];
  if(c->reader[i]||r->role!=i+1U||r->rights!=PAPACC_SCQ_FILE_READ_RIGHTS||r->volume!=c->identity[i].dwVolumeSerialNumber||
   r->high!=c->identity[i].nFileIndexHigh||r->low!=c->identity[i].nFileIndexLow||r->size_low!=c->identity[i].nFileSizeLow||r->size_high!=c->identity[i].nFileSizeHigh||r->attributes!=c->identity[i].dwFileAttributes||r->links!=c->identity[i].nNumberOfLinks)
   return personal_scq_inputs_fail(c,13,ERROR_INVALID_DATA);
  if(!papacc_store_hash(&c->content_hash,(const BYTE *)c->descriptor[i],c->descriptor_bytes[i],value)||memcmp(value,r->descriptor,32)){
   if(c->content_hash.retained)c->phase=4;return personal_scq_inputs_fail(c,25,ERROR_INVALID_SECURITY_DESCR);}
  memcpy(c->descriptor_digest[i],value,32);
  if(InterlockedCompareExchange(stop,0,0)||!deadline||GetTickCount64()>=deadline)return personal_scq_inputs_fail(c,14,ERROR_CANCELLED);
  if(c->image_chain.phase&&PAPACC_SCQ_EXISTING_PATH_PURPOSE(c->image_chain.paths,c->image_chain.plan.purpose)){
   DWORD own;
   if(!GetProcessHandleCount(GetCurrentProcess(),&own))return personal_scq_inputs_fail(c,31,GetLastError());
   if(own>=PAPACC_SCQ_HANDLE_GUARD(PAPACC_SCQ_P1_PATHS(c->image_chain.paths)))return personal_scq_inputs_fail(c,31,ERROR_NOT_ENOUGH_QUOTA);
  }
  c->reader[i]=ReOpenFile(c->file[i],GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,FILE_FLAG_SEQUENTIAL_SCAN);
  if(c->reader[i]==INVALID_HANDLE_VALUE){c->reader[i]=NULL;return personal_scq_inputs_fail(c,15,GetLastError());}
  if(!c->reader[i]||GetFileType(c->reader[i])!=FILE_TYPE_DISK||!GetFileInformationByHandle(c->reader[i],&info)||!personal_scq_inputs_same(&c->identity[i],&info))return personal_scq_inputs_fail(c,16,ERROR_INVALID_DATA);
 }
 for(i=0;i<3;i++){
  remaining=((ULONGLONG)c->identity[i].nFileSizeHigh<<32)|c->identity[i].nFileSizeLow;
  if(!remaining){e=personal_scq_inputs_fail(c,17,ERROR_INVALID_DATA);goto done;}
  c->content_hash.active=1;status=BCryptOpenAlgorithmProvider(&c->content_hash.algorithm,BCRYPT_SHA256_ALGORITHM,NULL,0);
  if(status||!c->content_hash.algorithm){c->content_hash.operation_status=status;if(c->content_hash.algorithm){c->content_hash.retained=1;c->phase=4;}e=personal_scq_inputs_fail(c,18,ERROR_INVALID_DATA);goto done;}
  status=BCryptCreateHash(c->content_hash.algorithm,&c->content_hash.hash,NULL,0,NULL,0,0);
  if(status||!c->content_hash.hash){c->content_hash.operation_status=status;if(c->content_hash.hash){c->content_hash.retained=1;c->phase=4;}e=personal_scq_inputs_fail(c,19,ERROR_INVALID_DATA);goto done;}
  while(remaining){
   if(InterlockedCompareExchange(stop,0,0)||GetTickCount64()>=deadline){e=personal_scq_inputs_fail(c,14,ERROR_CANCELLED);goto done;}
   n=0;if(!ReadFile(c->reader[i],chunk,remaining>sizeof(chunk)?sizeof(chunk):(DWORD)remaining,&n,NULL)||!n||n>remaining||n>sizeof(chunk)){
    e=personal_scq_inputs_fail(c,20,ERROR_INVALID_DATA);goto done;}
   status=BCryptHashData(c->content_hash.hash,chunk,n,0);SecureZeroMemory(chunk,sizeof(chunk));
   if(status){c->content_hash.operation_status=status;e=personal_scq_inputs_fail(c,21,ERROR_INVALID_DATA);goto done;}remaining-=n;
  }
  status=BCryptFinishHash(c->content_hash.hash,value,sizeof(value),0);
  if(status){c->content_hash.operation_status=status;e=personal_scq_inputs_fail(c,22,ERROR_INVALID_DATA);goto done;}
  if(personal_scq_contents_cleanup(c)){e=ERROR_INVALID_HANDLE;goto done;}
  if(memcmp(value,expected[i].digest,32)){e=personal_scq_inputs_fail(c,23,ERROR_INVALID_DATA);goto done;}
  memcpy(c->digest[i],value,32);
 }
 if(personal_scq_inputs_check(c,actor,stop,0)||GetTickCount64()>=deadline){e=personal_scq_inputs_fail(c,24,ERROR_ACCESS_DENIED);goto done;}
 c->content_verified=1;
done:SecureZeroMemory(chunk,sizeof(chunk));SecureZeroMemory(value,sizeof(value));
 if(c->content_hash.active&&!c->content_hash.retained&&personal_scq_contents_cleanup(c))e=ERROR_INVALID_HANDLE;
 return e;
}
/* Correlation of an already held, protected, content-verified image object
 * with the original live SCM process. Paths are an additional consistency
 * predicate, not the source, identity, approval or mapped-image attestation. */
static DWORD personal_scq_inputs_image_data(PAPACC_PERSONAL_SCQ_INPUTS *c,PSID actor,
 PAPACC_PERSONAL_RUNTIME_IDENTITY *r,PAPACC_SCQ_RUNTIME_OBSERVATION *out,volatile LONG *stop)
{
 WCHAR process_name[1024],file_name[1024];DWORD n=1024U,z,e;ULONG_PTR a=(ULONG_PTR)out,b=(ULONG_PTR)c,d=(ULONG_PTR)r;
 if(!c||!r||!out||!stop)return ERROR_INVALID_PARAMETER;
 if((a>=b?a-b<sizeof(*c):b-a<sizeof(*out))||(a>=d?a-d<sizeof(*r):d-a<sizeof(*out)))return ERROR_INVALID_PARAMETER;
 if(out->known!=15U||r->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||out->peer.pid!=r->pid||
  out->peer.creation[0]!=r->creation.dwLowDateTime||out->peer.creation[1]!=r->creation.dwHighDateTime||
  out->peer.token_id[0]!=r->statistics.TokenId.LowPart||out->peer.token_id[1]!=(DWORD)r->statistics.TokenId.HighPart||
  !c->content_verified||!c->reader[2]){memset(out,0,sizeof(*out));return ERROR_INVALID_STATE;}
 e=personal_scq_inputs_check(c,actor,stop,0);if(e)goto failed;
 e=papacc_personal_runtime_current(r);if(e)goto failed;
 memset(process_name,0,sizeof(process_name));memset(file_name,0,sizeof(file_name));
 if(!QueryFullProcessImageNameW(r->process,PROCESS_NAME_NATIVE,process_name,&n)||!n||n>=1024U||process_name[n]){e=ERROR_INVALID_DATA;goto failed;}
 z=GetFinalPathNameByHandleW(c->reader[2],file_name,1024U,FILE_NAME_NORMALIZED|VOLUME_NAME_NT);
 if(!z||z>=1024U||file_name[z]||n!=z||CompareStringOrdinal(process_name,(int)n,file_name,(int)z,FALSE)!=CSTR_EQUAL){e=ERROR_ACCESS_DENIED;goto failed;}
 e=personal_scq_inputs_check(c,actor,stop,0);if(e)goto failed;
 e=papacc_personal_runtime_current(r);if(e)goto failed;
 if(InterlockedCompareExchange(stop,0,0)){e=ERROR_CANCELLED;goto failed;}
 out->peer.image[0]=c->identity[2].dwVolumeSerialNumber;out->peer.image[1]=c->identity[2].nFileIndexHigh;out->peer.image[2]=c->identity[2].nFileIndexLow;
 memcpy(out->peer.image_digest,c->digest[2],32);out->known|=PAPACC_SCQ_OBS_IMAGE;return 0;
failed:memset(out,0,sizeof(*out));return e;
}
#include "personal_scq_inputs_close_win32.inc"
