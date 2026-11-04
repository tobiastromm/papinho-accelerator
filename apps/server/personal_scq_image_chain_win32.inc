#include "personal_scq_handle_policy_win32.h"
/* Read-only original-chain acquisition in the EXISTING SCQ input packet.
 * No creation, ACL edit, staging, administrative authority or endpoint. */
static DWORD scqi_chain_fail(PAPACC_SCQ_IMAGE_CHAIN *g,DWORD e)
{if(!g->first)g->first=e?e:ERROR_ACCESS_DENIED;return g->first;}
/* Conservative effective-write policy from the existing anchor/child rules:
 * unsupported ACEs deny; deny ACEs do not grant; INHERIT_ONLY does not apply
 * to this object. Only the self-only AU create-child bit is allowed at the
 * volume anchor, witnessed by its independently protected first child. */
#include "personal_directory_protection_win32.inc"
#include "personal_directory_reference_rights_win32.inc"
static int scqi_directory_same(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return (a->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&!(a->dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)&&
 a->dwFileAttributes==b->dwFileAttributes&&a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&
 a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow;}
/* P3: PXT1 LE32 tag, AMD64, purpose; 3 object IDs; 3 SD digests;
 * request/decision/attempt/policy/provenance; operator9words/SID; window4words.
 * Exactly428bytes, no native struct padding, pointers or new wire fields. */
static DWORD scqi_target_digest(PAPACC_SCQ_IMAGE_CHAIN *g,const PAPACC_SCQ_PLAN_DATA *p,BYTE digest[32])
{
 BYTE *at=g->target_bytes;unsigned i,j,index;const DWORD *words;const BY_HANDLE_FILE_INFORMATION *id;
 if(g->count<4U||!g->paths||g->paths->selected.architecture!=0x8664U)return ERROR_ACCESS_DENIED;
 scqp_put(&at,0x31545850U);scqp_put(&at,0x8664U);scqp_put(&at,p->purpose);
 for(i=0;i<3U;i++){
  index=i==0U?g->count-3U:i==1U?g->count-1U:g->count;id=&g->identity[index];
  scqp_put(&at,id->dwVolumeSerialNumber);scqp_put(&at,id->nFileIndexHigh);scqp_put(&at,id->nFileIndexLow);
 }
 for(i=0;i<3U;i++){
  index=i==0U?g->count-3U:i==1U?g->count-1U:g->count;
  if(!papacc_store_hash(&g->target_hash,(BYTE *)g->sd[index],g->length[index],at))goto unknown;at+=32;
 }
 memcpy(at,p->request,32);at+=32;memcpy(at,p->decision,32);at+=32;memcpy(at,p->attempt,32);at+=32;
 memcpy(at,p->policy,32);at+=32;memcpy(at,g->paths->selected.provenance,32);at+=32;
 scqp_put(&at,p->administrator.pid);
 for(i=0;i<4U;i++){words=i==0U?p->administrator.creation:i==1U?p->administrator.token_id:i==2U?p->administrator.authentication:p->administrator.modified;
  for(j=0;j<2U;j++)scqp_put(&at,words[j]);}
 scqp_put(&at,p->administrator.sid_bytes);memcpy(at,p->administrator.sid,68);at+=68;
 for(i=0;i<2U;i++)scqp_put(&at,p->valid_from[i]);for(i=0;i<2U;i++)scqp_put(&at,p->valid_until[i]);
 if((size_t)(at-g->target_bytes)!=428U)return ERROR_INVALID_DATA;
 if(g->paths->read_diagnostic){
  if(p->purpose!=PAPACC_SCQ_PURPOSE_READ||!scqp_nonzero(g->paths->read_public,32)||!scqp_key(p->key))return ERROR_ACCESS_DENIED;
  g->target_bytes[2]='R';memcpy(at,p->key,112);at+=112;memcpy(at,g->paths->read_public,32);at+=32;
 }
 if(!papacc_store_hash(&g->target_hash,g->target_bytes,(unsigned)(at-g->target_bytes),digest))goto unknown;return 0;
unknown:g->phase=4U;return ERROR_IO_INCOMPLETE;
}
static DWORD scqi_selected_scope(PAPACC_SCQ_IMAGE_CHAIN *g)
{
 const PAPACC_PERSONAL_INSTALL_PATHS *p=g->paths;unsigned i,index;const BY_HANDLE_FILE_INFORMATION *id;
 if(!p||p->mode!=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT||p->original_chain!=g||p->thread!=g->thread||p->phase!=PAPACC_PERSONAL_PATHS_READY||
  (p->selected_role!=1U&&p->selected_role!=2U)||p->selected.architecture!=0x8664U||g->count<4U||
  scqp_nonzero((BYTE *)p->parent,sizeof(p->parent))||scqp_nonzero((BYTE *)p->data,sizeof(p->data))||scqp_nonzero((BYTE *)p->leaf,sizeof(p->leaf)))return ERROR_ACCESS_DENIED;
 if(p->selected_role==1U)for(i=0;i<3U;i++){
  index=i==0U?g->count-3U:i==1U?g->count-1U:g->count;id=&g->identity[index];
  if(p->selected.identity[3U*i]!=id->dwVolumeSerialNumber||p->selected.identity[3U*i+1U]!=id->nFileIndexHigh||p->selected.identity[3U*i+2U]!=id->nFileIndexLow)return ERROR_REVISION_MISMATCH;
 }
 return 0;
}
static DWORD scqi_chain_current(PAPACC_SCQ_IMAGE_CHAIN *g,PSID actor,volatile LONG *stop)
{
 unsigned i;HANDLE h;DWORD e,flags,n;BY_HANDLE_FILE_INFORMATION f;
 if(!g||g->self!=g||!g->owner||g->thread!=GetCurrentThreadId()||g->phase!=2U||g->first||!stop||g->stop!=stop||
  !g->paths||g->count<2U||g->count>PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS||!g->installer)return ERROR_INVALID_STATE;
 if(InterlockedCompareExchange(stop,0,0)||!PAPACC_SCQ_EXISTING_PATH_PURPOSE(g->paths,g->plan.purpose)||
  !scqp_static(&g->plan)||!scqp_window(&g->plan)||!EqualSid(actor,(PSID)g->plan.administrator.sid))return scqi_chain_fail(g,ERROR_ACCESS_DENIED);
 e=g->paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT?scqi_selected_scope(g):g->paths->mode==PAPACC_PERSONAL_PATHS_BINARY_ONLY?papacc_personal_paths_binary_current(g->paths,stop):
  g->paths->mode==PAPACC_PERSONAL_PATHS_NEW?papacc_personal_paths_current(g->paths,stop):ERROR_INVALID_STATE;
 if(e||wcscmp(g->binary,g->paths->binary))return scqi_chain_fail(g,e?e:ERROR_REVISION_MISMATCH);
 for(i=0;i<=g->count;i++){
  h=i==g->count?g->image:g->directory[i];
  if(InterlockedCompareExchange(stop,0,0)||!h||!GetHandleInformation(h,&flags)||(flags&HANDLE_FLAG_INHERIT)||
   GetFileType(h)!=FILE_TYPE_DISK||!GetFileInformationByHandle(h,&f)||
   !(i==g->count?personal_scq_inputs_same(&g->identity[i],&f):scqi_directory_same(&g->identity[i],&f)))return scqi_chain_fail(g,ERROR_INVALID_DATA);
  if(g->scratch)return scqi_chain_fail(g,ERROR_INVALID_STATE);
  e=g->paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT?
   scqi_native_file_sd(h,&g->scratch,&g->native_sd,stop):
   GetSecurityInfo(h,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&g->scratch);
  if(e||!g->scratch){if(e&&g->scratch)g->phase=4U;return scqi_chain_fail(g,e?e:ERROR_INVALID_SECURITY_DESCR);}
  n=GetSecurityDescriptorLength(g->scratch);
  e=!n||n>65536U||n!=g->length[i]||!g->sd[i]||memcmp(g->sd[i],g->scratch,n)||
   (i<g->count&&!scqi_directory_sd(g->scratch,actor,g->installer,i==0U))?ERROR_ACCESS_DENIED:0;
  if(LocalFree(g->scratch)){g->phase=4U;return scqi_chain_fail(g,ERROR_INVALID_HANDLE);}g->scratch=NULL;
  if(e)return scqi_chain_fail(g,e);
 }
 /* Every directory, including the first child of the volume anchor, passed
  * the protected DACL rule above. No self-only create-child waiver elsewhere. */
 if(InterlockedCompareExchange(stop,0,0)||!scqp_window(&g->plan))return scqi_chain_fail(g,ERROR_CANCELLED);
 if(g->paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT){BYTE digest[32];e=scqi_target_digest(g,&g->plan,digest);if(e||memcmp(digest,g->plan.target,32))return scqi_chain_fail(g,e?e:ERROR_CRC);}
 return 0;
}
typedef NTSTATUS (NTAPI *SCQI_NT_OPEN)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
static DWORD scqi_chain_open_kind(PAPACC_SCQ_IMAGE_CHAIN *g,unsigned i,HANDLE parent,const WCHAR *name,int directory,int parent_metadata)
{
 SCQI_NT_OPEN open_file;FARPROC address;HMODULE module;NTSTATUS status;DWORD count,e;HANDLE *out;
 if(i>PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS||(directory&&i==PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS)||!name||wcsnlen(name,MAX_PATH+4U)>=MAX_PATH+4U)return scqi_chain_fail(g,ERROR_INVALID_PARAMETER);
 if(!g->stop||InterlockedCompareExchange(g->stop,0,0))return scqi_chain_fail(g,ERROR_CANCELLED);
 out=directory?&g->directory[i]:&g->image;if(*out)return scqi_chain_fail(g,ERROR_INVALID_STATE);
 if(!GetProcessHandleCount(GetCurrentProcess(),&count))return scqi_chain_fail(g,GetLastError());
 if(count>=((g->plan.purpose==0x4b310001U||g->plan.purpose==PAPACC_FIRST_PREPARE_CHAIN_TAG)?188U:PAPACC_SCQ_HANDLE_GUARD(PAPACC_SCQ_EXISTING_PATH_PURPOSE(g->paths,g->plan.purpose)&&PAPACC_SCQ_P1_PATHS(g->paths))))return scqi_chain_fail(g,ERROR_NOT_ENOUGH_QUOTA); /* custody guard only, never authority */
 module=GetModuleHandleW(L"ntdll.dll");address=module?GetProcAddress(module,"NtCreateFile"):NULL;
 if(!address)return scqi_chain_fail(g,ERROR_NOT_SUPPORTED);memcpy(&open_file,&address,sizeof(open_file));
 wcscpy_s(g->name,MAX_PATH+4U,name);memset(&g->attributes,0,sizeof(g->attributes));memset(&g->io,0,sizeof(g->io));
 g->unicode.Buffer=g->name;g->unicode.Length=(USHORT)(wcslen(g->name)*sizeof(WCHAR));g->unicode.MaximumLength=g->unicode.Length;
 g->attributes.Length=sizeof(g->attributes);g->attributes.RootDirectory=parent;g->attributes.ObjectName=&g->unicode;
 g->attributes.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 if(InterlockedCompareExchange(g->stop,0,0)||!scqp_window(&g->plan))return scqi_chain_fail(g,ERROR_CANCELLED);
 status=open_file(out,directory?scqi_directory_rights(parent_metadata):PAPACC_SCQ_FILE_READ_RIGHTS,
  &g->attributes,&g->io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1U,0x00200020U|(directory?1U:0x40U),NULL,0);
 g->open_status=status;
 if(status||g->io.Status||g->io.Information!=1U||!*out){
  if(status>0||g->io.Status==(NTSTATUS)0x103L||*out)g->phase=4U;
  return scqi_chain_fail(g,ERROR_INVALID_DATA);
 }
 if(!GetFileInformationByHandle(*out,&g->identity[i]))return scqi_chain_fail(g,GetLastError());
 e=(g->paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT||g->plan.purpose==0x4b310001U||g->plan.purpose==PAPACC_FIRST_PREPARE_CHAIN_TAG)?
  scqi_native_file_sd(*out,&g->sd[i],&g->native_sd,g->stop):
  GetSecurityInfo(*out,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&g->sd[i]);
 if(e||!g->sd[i]){if(e&&g->sd[i])g->phase=4U;return scqi_chain_fail(g,e?e:ERROR_INVALID_SECURITY_DESCR);}
 g->length[i]=GetSecurityDescriptorLength(g->sd[i]);
 if(!g->length[i]||g->length[i]>65536U)return scqi_chain_fail(g,ERROR_INVALID_SECURITY_DESCR);
 return 0;
}
static DWORD scqi_chain_open(PAPACC_SCQ_IMAGE_CHAIN *g,unsigned i,HANDLE parent,const WCHAR *name,int directory)
{return scqi_chain_open_kind(g,i,parent,name,directory,0);}
/* Only the cold preparation producer has an independent retained writable
 * parent original. This extra reference is metadata only, never a traversal
 * or create source. Share mode remains FILE_SHARE_READ for both references. */
static DWORD scqi_chain_parent_metadata(PAPACC_SCQ_IMAGE_CHAIN *g,unsigned i,HANDLE parent,const WCHAR *name)
{
 if(!g||g->plan.purpose!=PAPACC_FIRST_PREPARE_CHAIN_TAG||!parent||!i)return ERROR_INVALID_STATE;
 return scqi_chain_open_kind(g,i,parent,name,1,1);
}
static DWORD scqi_chain_capture(PAPACC_SCQ_IMAGE_CHAIN *g,const void *owner,const PAPACC_PERSONAL_INSTALL_PATHS *paths,
 const PAPACC_SCQ_PLAN_DATA *plan,const BY_HANDLE_FILE_INFORMATION *image,PSID actor,volatile LONG *stop)
{
 WCHAR root_name[MAX_PATH+4U],drive_root[4],part[MAX_PATH];size_t i,start,n;DWORD e;
 if(!g||g->phase||!owner||!paths||!plan||!image||!actor||!stop||!PAPACC_SCQ_EXISTING_PATH_PURPOSE(paths,plan->purpose)||
  !scqp_static(plan)||!scqp_window(plan)||!IsValidSid(actor)||!EqualSid(actor,(PSID)plan->administrator.sid)||
  paths->phase!=PAPACC_PERSONAL_PATHS_READY||paths->thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 e=paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT?(paths->original_chain==g?0:ERROR_ACCESS_DENIED):paths->mode==PAPACC_PERSONAL_PATHS_BINARY_ONLY?papacc_personal_paths_binary_current(paths,stop):
  paths->mode==PAPACC_PERSONAL_PATHS_NEW?papacc_personal_paths_current(paths,stop):ERROR_INVALID_STATE;
 if(e)return e;
 n=wcsnlen(paths->binary,MAX_PATH);
 if(n<3U||n>=MAX_PATH||paths->binary[1]!=L':'||paths->binary[2]!=L'\\'||
  paths->binary[0]<L'A'||paths->binary[0]>L'Z')return ERROR_INVALID_NAME;
 g->self=g;g->owner=owner;g->paths=paths;g->stop=stop;g->thread=GetCurrentThreadId();g->phase=1U;g->plan=*plan;
 wcscpy_s(g->binary,MAX_PATH,paths->binary);
 if(InterlockedCompareExchange(stop,0,0))return scqi_chain_fail(g,ERROR_CANCELLED);
 if(!ConvertStringSidToSidW(L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464",&g->installer)){
  if(g->installer)g->phase=4U;return scqi_chain_fail(g,GetLastError());
 }
 if(paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT){
  if((paths->selected_role!=1U&&paths->selected_role!=2U)||paths->selected.architecture!=0x8664U)return scqi_chain_fail(g,ERROR_ACCESS_DENIED);
  drive_root[0]=paths->binary[0];drive_root[1]=L':';drive_root[2]=L'\\';drive_root[3]=0;
  e=scq_p1_native_name(drive_root,1,root_name);if(e)return scqi_chain_fail(g,e);
 }else {wcscpy_s(root_name,MAX_PATH+4U,L"\\??\\C:\\");root_name[4]=paths->binary[0];}
 if((e=scqi_chain_open(g,0U,NULL,root_name,1))!=0)return e;g->count=1U;
 start=3U;
 for(i=start;i<=n;i++)if(!paths->binary[i]||paths->binary[i]==L'\\'){
  if(i==start||i-start>=MAX_PATH||g->count==PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS)return scqi_chain_fail(g,ERROR_INSUFFICIENT_BUFFER);
  if(InterlockedCompareExchange(stop,0,0))return scqi_chain_fail(g,ERROR_CANCELLED);
  memcpy(part,paths->binary+start,(i-start)*sizeof(WCHAR));part[i-start]=0;
  if(!wcscmp(part,L".")||!wcscmp(part,L".."))return scqi_chain_fail(g,ERROR_INVALID_NAME);
  if((e=scqi_chain_open(g,g->count,g->directory[g->count-1U],part,1))!=0)return e;g->count++;start=i+1U;
 }
 if((e=scqi_chain_open(g,g->count,g->directory[g->count-1U],L"papacc_server.exe",0))!=0)return e;
 if(!personal_scq_inputs_same(image,&g->identity[g->count]))return scqi_chain_fail(g,ERROR_INVALID_DATA);
 if(paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT){BYTE digest[32];
  e=scqi_selected_scope(g);if(e)return scqi_chain_fail(g,e);
  e=scqi_target_digest(g,plan,digest);if(e)return scqi_chain_fail(g,e);
  if(paths->selected_role==2U&&memcmp(digest,plan->target,32))return scqi_chain_fail(g,ERROR_CRC);
  memcpy(g->plan.target,digest,32);
 }
 g->phase=2U;return scqi_chain_current(g,actor,stop);
}
#include "personal_scq_image_chain_close_win32.inc"
