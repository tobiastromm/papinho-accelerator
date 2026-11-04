/* Fixed registration bundle: relative originals in the SAME existing packet. */
typedef NTSTATUS (NTAPI *SCM_REG_NT_OPEN)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
static PAPACC_RESULT scm_registration_file_open(PAPACC_CONFIGURATION_COMMIT *o,unsigned index)
{
 static const WCHAR *const leaves[3]={L"papacc_server.exe",L"libssl-3-x64.dll",L"libcrypto-3-x64.dll"};
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCM_REGISTRATION_ACT *act;HANDLE *handle;
 SCM_REG_NT_OPEN open_file;FARPROC proc;HMODULE module;OBJECT_ATTRIBUTES a;UNICODE_STRING name;NTSTATUS status;
 if(!o||index>=3U||o->writer!=GetCurrentThreadId()||papacc_configuration_personal_native_current(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;act=&n->scm_registration;
 if(n->phase!=1U||!o->pending_commit.bootstrap_parent)return PAPACC_RESULT_INVALID_STATE;
 if(act->self!=act||!act->original||act->thread!=GetCurrentThreadId()||!act->invocation||act->phase!=1U||act->consumed||o->pending_commit.stop)return PAPACC_RESULT_INVALID_STATE;
 handle=index?&act->dependencies[index-1U].handle:&n->scm_image;if(*handle)return PAPACC_RESULT_INVALID_STATE;
 if(act->package_captured||(index&&(act->dependency_close_attempts&(1U<<(index-1U)))))return PAPACC_RESULT_INVALID_STATE;
 module=GetModuleHandleA("ntdll.dll");proc=module?GetProcAddress(module,"NtCreateFile"):NULL;
 if(!proc)return PAPACC_RESULT_NOT_SUPPORTED;memcpy(&open_file,&proc,sizeof(open_file));
 ZeroMemory(&a,sizeof(a));a.Length=sizeof(a);a.RootDirectory=o->pending_commit.bootstrap_parent;
 name.Buffer=(WCHAR *)leaves[index];name.Length=(USHORT)(wcslen(leaves[index])*sizeof(WCHAR));name.MaximumLength=name.Length;
 a.ObjectName=&name;a.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 ZeroMemory(&act->original_io,sizeof(act->original_io));
 status=open_file(handle,FILE_READ_DATA|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,&a,&act->original_io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1U,0x00200060U,NULL,0U);
 if(status==0&&act->original_io.Status==0&&act->original_io.Information==1U&&*handle)return PAPACC_RESULT_OK;
 if(status>0||act->original_io.Status==(NTSTATUS)0x103L||*handle){n->phase=2U;o->unresolved=1;}
 return PAPACC_RESULT_INVALID_STATE; /* pending IOSB/name/handle remain owned */
}
PAPACC_RESULT papacc_configuration_scm_registration_image_open(PAPACC_CONFIGURATION_COMMIT *o,const WCHAR *leaf)
{if(!leaf||wcscmp(leaf,L"papacc_server.exe"))return PAPACC_RESULT_INVALID_ARGUMENT;return scm_registration_file_open(o,0U);}
PAPACC_RESULT papacc_configuration_scm_registration_dependency_open(PAPACC_CONFIGURATION_COMMIT *o,unsigned index)
{return index<2U?scm_registration_file_open(o,index+1U):PAPACC_RESULT_INVALID_ARGUMENT;}
static int scm_registration_files_close(PAPACC_PERSONAL_NATIVE_CONTEXT *n)
{
 unsigned i;if(n->phase==2U)return 0;
 for(i=0;i<2U;i++)if(n->scm_registration.dependencies[i].handle){
  if(n->scm_registration.dependency_close_attempts&(1U<<i)){n->phase=2U;return 0;}
  n->scm_registration.dependency_close_attempts|=1U<<i;
  if(!CloseHandle(n->scm_registration.dependencies[i].handle)){n->phase=2U;return 0;}
  n->scm_registration.dependencies[i].handle=NULL;
 }
 return 1;
}
