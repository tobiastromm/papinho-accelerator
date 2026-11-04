/* Native security query modeled; only test-owned descriptor memory is real. */
#include <windows.h>
#include <winternl.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
#include "../apps/server/personal_scq_native_sd_win32.h"
static unsigned fault,calls,allocations,checks;static DWORD fixture_bytes;static PSECURITY_DESCRIPTOR fixture;static volatile LONG model_stop;
#define C(x) do{checks++;if(!(x)){fprintf(stderr,"FAIL native SD line=%u fault=%u\n",(unsigned)__LINE__,fault);return 1;}}while(0)
static HLOCAL WINAPI model_alloc(UINT flags,SIZE_T bytes)
{HLOCAL p;allocations++;if(fault==11U)return NULL;p=LocalAlloc(flags,bytes);if(fault==12U)model_stop=1;return p;}
static NTSTATUS NTAPI model_query(HANDLE file,SECURITY_INFORMATION mask,PSECURITY_DESCRIPTOR out,ULONG bytes,PULONG needed)
{
 calls++;if(file!=(HANDLE)71||mask!=(OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION))return (NTSTATUS)0xc000000dL;
 if(!out){
  if(bytes)return (NTSTATUS)0xc000000dL;*needed=fixture_bytes;
  if(fault==3U)return (NTSTATUS)0xc0000022L;if(fault==4U)return 0;if(fault==5U)return (NTSTATUS)0x103L;
  if(fault==6U)*needed=0;if(fault==7U)*needed=65537U;if(fault==8U)*needed=19U;
  if(fault==10U)model_stop=1;return (NTSTATUS)0xc0000023L;
 }
 if(bytes!=fixture_bytes)return (NTSTATUS)0xc000000dL;memcpy(out,fixture,fixture_bytes);*needed=fixture_bytes;
 if(fault==13U){*needed=bytes+4U;return (NTSTATUS)0xc0000023L;}
 if(fault==14U)return (NTSTATUS)0xc0000022L;if(fault==15U)return (NTSTATUS)0x103L;
 if(fault==16U)((SECURITY_DESCRIPTOR_RELATIVE *)out)->Control&=(SECURITY_DESCRIPTOR_CONTROL)~SE_SELF_RELATIVE;
 if(fault==17U)((SECURITY_DESCRIPTOR_RELATIVE *)out)->Owner=bytes-4U;
 if(fault==18U)((SECURITY_DESCRIPTOR_RELATIVE *)out)->Dacl=bytes+4U;
 if(fault==19U)((SECURITY_DESCRIPTOR_RELATIVE *)out)->Owner=1U;
 if(fault==20U)((SECURITY_DESCRIPTOR_RELATIVE *)out)->Revision=0;
 if(fault==21U){DWORD at=((SECURITY_DESCRIPTOR_RELATIVE *)out)->Owner;((BYTE *)out)[at+1U]=255;}
 if(fault==22U){DWORD at=((SECURITY_DESCRIPTOR_RELATIVE *)out)->Dacl;((ACL *)((BYTE *)out+at))->AclSize=65535U;}
 if(fault==23U)*needed=0;if(fault==24U)*needed=bytes+1U;if(fault==25U)*needed=19U;
 if(fault==26U)*needed=bytes-1U;if(fault==27U)model_stop=1;return 0;
}
static HMODULE WINAPI model_module(LPCWSTR name){(void)name;return fault==1U?NULL:(HMODULE)1;}
static FARPROC WINAPI model_symbol(HMODULE module,LPCSTR name)
{FARPROC result=NULL;NTSTATUS (NTAPI *query)(HANDLE,SECURITY_INFORMATION,PSECURITY_DESCRIPTOR,ULONG,PULONG)=model_query;if(module&&fault!=2U&&!strcmp(name,"NtQuerySecurityObject"))memcpy(&result,&query,sizeof(result));return result;}
#define GetModuleHandleW model_module
#define GetProcAddress model_symbol
#define LocalAlloc model_alloc
#include "../apps/server/personal_scq_native_sd_win32.inc"
int main(void)
{
 unsigned i;DWORD e;PAPACC_SCQ_NATIVE_SD_STATUS d;PSECURITY_DESCRIPTOR out;
 C(ConvertStringSecurityDescriptorToSecurityDescriptorA("O:BAG:BAD:P(A;;FA;;;BA)",SDDL_REVISION_1,&fixture,&fixture_bytes));
 for(i=0;i<28U;i++){
  fault=i;calls=allocations=0;model_stop=i==9U?1:0;out=NULL;memset(&d,0,sizeof(d));
  e=scqi_native_file_sd((HANDLE)71,&out,&d,&model_stop);
  if(!i){C(!e&&calls==2U&&allocations==1U&&out&&d.capacity==fixture_bytes&&d.needed==fixture_bytes&&!memcmp(out,fixture,fixture_bytes));}
  else C(e!=0);
  C(calls<=2U&&allocations<=1U&&d.calls==calls);
  if(i==1U||i==2U||i==9U)C(!calls&&!allocations&&!out);
  if(i>=3U&&i<=8U)C(calls==1U&&!allocations&&!out);
  if(i==10U)C(e==ERROR_CANCELLED&&calls==1U&&!allocations&&!out);
  if(i==11U)C(e==ERROR_NOT_ENOUGH_MEMORY&&calls==1U&&allocations==1U&&!out);
  if(i==12U)C(e==ERROR_CANCELLED&&calls==1U&&out);
  if(i==13U||i==14U||i==15U)C(out&&d.status==(i==13U?0xc0000023U:i==14U?0xc0000022U:0x103U));
  if(i==27U)C(e==ERROR_CANCELLED&&out&&calls==2U);
  /* Only test-created memory, never a production retry of an uncertain free. */
  if(out)C(!LocalFree(out));
 }
 fault=0;model_stop=0;calls=0;out=(PSECURITY_DESCRIPTOR)(ULONG_PTR)1;
 C(scqi_native_file_sd((HANDLE)71,&out,&d,&model_stop)==ERROR_INVALID_STATE&&!calls);
 out=NULL;C(scqi_native_file_sd(INVALID_HANDLE_VALUE,&out,&d,&model_stop)==ERROR_INVALID_STATE&&!calls);
 C(scqi_native_file_sd((HANDLE)71,&out,&d,NULL)==ERROR_INVALID_STATE&&!calls);
 C(!LocalFree(fixture));
 printf("PASS NATIVE_SD cases=31 checks=%u SAME_HANDLE_READ_CONTROL_OWNER_DACL_NO_RETRY_NO_FALLBACK_ALL_NATIVE_CALLS_MODELED\n",checks);return 0;
}
