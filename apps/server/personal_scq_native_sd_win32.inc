#ifndef PAPACC_SCQ_NATIVE_SD_WIN32_INC
#define PAPACC_SCQ_NATIVE_SD_WIN32_INC
#include "personal_scq_native_sd_win32.h"
typedef NTSTATUS (NTAPI *SCQI_QUERY_SD)(HANDLE,SECURITY_INFORMATION,PSECURITY_DESCRIPTOR,ULONG,PULONG);
/* Bound offsets before asking the public SD accessors to inspect the buffer.
 * This is structural validation only; existing owner/DACL policies still run. */
static int scqi_sd_bounded(const BYTE *p,DWORD bytes)
{
 const SECURITY_DESCRIPTOR_RELATIVE *s;DWORD off,n,i;const ACL *acl;
 if(!p||bytes<sizeof(SECURITY_DESCRIPTOR_RELATIVE))return 0;s=(const SECURITY_DESCRIPTOR_RELATIVE *)p;
 if(s->Revision!=SECURITY_DESCRIPTOR_REVISION||!(s->Control&SE_SELF_RELATIVE))return 0;
 for(i=0;i<2U;i++){
  off=i?s->Group:s->Owner;if(!off)continue;
  if(off<sizeof(*s)||(off&3U)||off>bytes||bytes-off<8U||p[off+1U]>SID_MAX_SUB_AUTHORITIES)return 0;
  n=8U+4U*p[off+1U];if(n>bytes-off)return 0;
 }
 for(i=0;i<2U;i++){
  off=i?s->Sacl:s->Dacl;if(!off)continue;
  if(off<sizeof(*s)||(off&3U)||off>bytes||bytes-off<sizeof(ACL))return 0;
  acl=(const ACL *)(p+off);if(acl->AclSize<sizeof(ACL)||acl->AclSize>bytes-off||!IsValidAcl((PACL)acl))return 0;
 }
 return IsValidSecurityDescriptor((PSECURITY_DESCRIPTOR)p)!=FALSE;
}
/* Same retained file reference/READ_CONTROL, owner+DACL only. One sizing query
 * and one fill, no retry/reopen/fallback/privilege change. Memory is transferred
 * immediately to the existing checking/scratch slot, including uncertain errors. */
static DWORD scqi_native_file_sd(HANDLE file,PSECURITY_DESCRIPTOR *out,PAPACC_SCQ_NATIVE_SD_STATUS *d,volatile LONG *stop)
{
 HMODULE module;FARPROC address;SCQI_QUERY_SD query;NTSTATUS status;ULONG needed=0;DWORD bytes;
 if(!file||file==INVALID_HANDLE_VALUE||!out||*out||!d||!stop)return ERROR_INVALID_STATE;
 memset(d,0,sizeof(*d));if(InterlockedCompareExchange(stop,0,0))return ERROR_CANCELLED;
 module=GetModuleHandleW(L"ntdll.dll");address=module?GetProcAddress(module,"NtQuerySecurityObject"):NULL;
 if(!address)return ERROR_NOT_SUPPORTED;memcpy(&query,&address,sizeof(query));
 status=query(file,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,0,&needed);
 d->calls=1U;d->status=(DWORD)status;d->needed=needed;
 if(status!=(NTSTATUS)0xc0000023L||needed<sizeof(SECURITY_DESCRIPTOR_RELATIVE)||needed>PAPACC_SCQ_FILE_SD_BYTES)return ERROR_INVALID_SECURITY_DESCR;
 if(InterlockedCompareExchange(stop,0,0))return ERROR_CANCELLED;
 bytes=needed;*out=(PSECURITY_DESCRIPTOR)LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT,bytes);
 if(!*out)return ERROR_NOT_ENOUGH_MEMORY;d->capacity=bytes;
 if(InterlockedCompareExchange(stop,0,0))return ERROR_CANCELLED;
 needed=0;status=query(file,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,*out,bytes,&needed);
 d->calls=2U;d->status=(DWORD)status;d->needed=needed;
 if(status||needed<sizeof(SECURITY_DESCRIPTOR_RELATIVE)||needed>bytes||
  !scqi_sd_bounded((BYTE *)*out,needed)||GetSecurityDescriptorLength(*out)!=needed)return ERROR_INVALID_SECURITY_DESCR;
 return InterlockedCompareExchange(stop,0,0)?ERROR_CANCELLED:0;
}
#endif
