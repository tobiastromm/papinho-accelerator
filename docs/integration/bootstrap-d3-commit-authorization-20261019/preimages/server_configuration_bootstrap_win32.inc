/* Namespace phase of the existing pending worker; no registry, journal or
 * publication. The parent object, not a re-opened spelling, is the namespace.
 * Privileged host/kernel compromise is outside this local capability proof. */
#include <winternl.h>
#ifndef OBJ_DONT_REPARSE
#define OBJ_DONT_REPARSE 0x1000L
#endif
typedef NTSTATUS (NTAPI *BOOTSTRAP_NT_CREATE)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,
 PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
static int bootstrap_admin(PAPACC_CONFIG_PENDING *p)
{
 BYTE sid[SECURITY_MAX_SID_SIZE];DWORD bytes=sizeof(sid),e;BOOL member=FALSE;HANDLE token=NULL;
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&token)){
  if(!CloseHandle(token)){p->file=token;p->close_error=GetLastError();}return 0;
 }
 e=GetLastError();if(e!=ERROR_NO_TOKEN)return 0;
 return CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,sid,&bytes)&&
  CheckTokenMembership(NULL,sid,&member)&&member;
}
static int bootstrap_identity(HANDLE h,DWORD *volume,DWORD *high,DWORD *low,int first)
{
 BY_HANDLE_FILE_INFORMATION f;
 if(!GetFileInformationByHandle(h,&f)||!(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||
    (f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return 0;
 if(first){*volume=f.dwVolumeSerialNumber;*high=f.nFileIndexHigh;*low=f.nFileIndexLow;return 1;}
 return *volume==f.dwVolumeSerialNumber&&*high==f.nFileIndexHigh&&*low==f.nFileIndexLow;
}
static void bootstrap_worker(PAPACC_CONFIG_PENDING *p)
{
 HMODULE module;FARPROC address;BOOTSTRAP_NT_CREATE create;
 WCHAR path[MAX_PATH+5],leaf[65];UNICODE_STRING name;OBJECT_ATTRIBUTES attr;IO_STATUS_BLOCK io;
 NTSTATUS status;unsigned i,n,k;int admin;char drive[3],device[512];
 admin=bootstrap_admin(p);p->bootstrap_admin=admin?1U:0U;
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
 /* Native custody primitive only; not a production authority bypass. */
 if(p->checkpoint_only==91)admin=1;
#endif
 if(!admin||p->close_error||InterlockedCompareExchange(&p->stop,0,0))return;
 module=GetModuleHandleA("ntdll.dll");if(!module)return;address=GetProcAddress(module,"NtCreateFile");
 if(!address)return;memcpy(&create,&address,sizeof(create));
 /* Resolve the DOS drive ONCE, then address the observed native device.
  * OBJ_DONT_REPARSE also rejects the object-manager DOS-drive link itself. */
 drive[0]=p->path[0];drive[1]=':';drive[2]=0;
 if(!QueryDosDeviceA(drive,device,sizeof(device))||strncmp(device,"\\Device\\",8U)){p->native_error=GetLastError();return;}
 k=(unsigned)strlen(device);n=(unsigned)strlen(p->path);
 if(k+n-2U>=MAX_PATH+5U){p->native_error=ERROR_BUFFER_OVERFLOW;return;}
 for(i=0;i<k;i++)path[i]=(WCHAR)(unsigned char)device[i];
 for(i=2U;i<=n;i++)path[k+i-2U]=(WCHAR)(unsigned char)p->path[i];n=k+n-2U;
 memset(&attr,0,sizeof(attr));memset(&io,0,sizeof(io));name.Buffer=path;name.Length=(USHORT)(n*2U);name.MaximumLength=name.Length;
 attr.Length=sizeof(attr);attr.ObjectName=&name;attr.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 status=create(&p->bootstrap_parent,FILE_LIST_DIRECTORY|FILE_TRAVERSE|FILE_ADD_SUBDIRECTORY|FILE_READ_ATTRIBUTES|SYNCHRONIZE,
  &attr,&io,NULL,FILE_ATTRIBUTE_DIRECTORY,FILE_SHARE_READ,1U,0x00200021U,NULL,0U);
 if(status<0){p->native_error=(DWORD)status;p->bootstrap_parent=NULL;return;}
 if(!bootstrap_identity(p->bootstrap_parent,&p->bootstrap_parent_volume,&p->bootstrap_parent_high,&p->bootstrap_parent_low,1))goto unknown;
 if(InterlockedCompareExchange(&p->stop,0,0))return;
 n=(unsigned)strlen((char *)p->previous);for(i=0;i<=n;i++)leaf[i]=(WCHAR)p->previous[i];
 name.Buffer=leaf;name.Length=(USHORT)(n*2U);name.MaximumLength=name.Length;attr.RootDirectory=p->bootstrap_parent;
 /* FILE_CREATE returns the NEW directory handle atomically; no CreateDirectory/open gap. */
 status=create(&p->bootstrap_container,FILE_LIST_DIRECTORY|FILE_TRAVERSE|FILE_ADD_FILE|FILE_READ_ATTRIBUTES|SYNCHRONIZE,
  &attr,&io,NULL,FILE_ATTRIBUTE_DIRECTORY,FILE_SHARE_READ,2U,0x00200021U,NULL,0U);
 if(status<0){p->native_error=(DWORD)status;p->bootstrap_container=NULL;return;}
 p->bootstrap_created=1U;
 if(io.Information!=2U||!bootstrap_identity(p->bootstrap_container,&p->bootstrap_volume,&p->bootstrap_high,&p->bootstrap_low,1))goto unknown;
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 if(p->test_entered){
  if(!SetEvent(p->test_entered)||WaitForSingleObject(p->test_release,INFINITE)!=WAIT_OBJECT_0)goto unknown;
 }
 if(p->test_unknown)goto unknown;
#endif
 if(InterlockedCompareExchange(&p->stop,0,0))return;
 if(!bootstrap_identity(p->bootstrap_parent,&p->bootstrap_parent_volume,&p->bootstrap_parent_high,&p->bootstrap_parent_low,0)||
    !bootstrap_identity(p->bootstrap_container,&p->bootstrap_volume,&p->bootstrap_high,&p->bootstrap_low,0))goto unknown;
 p->bootstrap_ready=1U;p->result=PAPACC_CAS_SUCCESS;return;
unknown:p->result=PAPACC_CAS_UNKNOWN;p->native_error=GetLastError();
}
