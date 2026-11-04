#include "personal_install_paths_win32.h"
#include <shlobj.h>
#include <knownfolders.h>
#include <wchar.h>
#include <string.h>
/* Provenance selector for the one previously qualified installation. It is
 * NOT authority; selection still needs originals and attributed E/Q/S acts. */
static const BYTE pip_existing_provenance[32]={0x43,0xfb,0x67,0x12,0xbe,0x30,0xce,0x5b,0x7a,0xfc,0xa8,0x11,0x15,0x12,0xbd,0x28,0x46,0x72,0xef,0xa4,0xbb,0x4b,0x86,0x78,0xb2,0x1a,0x04,0xeb,0x7b,0x36,0xdd,0x87};
static DWORD pip_selected_variant(PAPACC_PERSONAL_INSTALL_PATHS *p,const PAPACC_SCQ_EXISTING_SELECTION *selection,DWORD role,const void *chain,volatile LONG *stop,unsigned read_diagnostic,const BYTE *public_id)
{
 WCHAR image[MAX_PATH],*last;DWORD n;unsigned i;size_t len,suffix;
 if(!p||p->phase||p->read_diagnostic||read_diagnostic>1U||!chain||!stop||InterlockedCompareExchange(stop,0,0)||(role!=1U&&role!=2U))return ERROR_INVALID_STATE;
if(read_diagnostic){unsigned any=0;if(!public_id)return ERROR_INVALID_DATA;for(i=0;i<32U;i++)any|=public_id[i];if(!any)return ERROR_INVALID_DATA;}
#if !defined(_M_X64)
 (void)selection;return ERROR_NOT_SUPPORTED;
#else
 if(role==1U){
  if(!selection||selection->architecture!=0x8664U||memcmp(selection->provenance,pip_existing_provenance,32))return ERROR_ACCESS_DENIED;
  for(i=0;i<3U;i++)if(!selection->identity[3U*i]||!(selection->identity[3U*i+1U]|selection->identity[3U*i+2U]))return ERROR_INVALID_DATA;
 }
 if(role==2U&&selection)return ERROR_ACCESS_DENIED; /* receiver derives, never imports expected IDs */
 /* Locator only, never an independently trusted destination. No Shell/env. */
 n=GetModuleFileNameW(NULL,image,MAX_PATH);if(!n||n>=MAX_PATH)return ERROR_INVALID_NAME;
 last=wcsrchr(image,L'\\');if(!last||wcscmp(last,L"\\papacc_server.exe"))return ERROR_INVALID_NAME;*last=0;
 len=wcslen(image);suffix=wcslen(L"\\Papinho\\Accelerator");
 if(len<=suffix||CompareStringOrdinal(image+len-suffix,-1,L"\\Papinho\\Accelerator",-1,TRUE)!=CSTR_EQUAL)return ERROR_INVALID_NAME;
 if(!((image[0]>=L'A'&&image[0]<=L'Z')||(image[0]>=L'a'&&image[0]<=L'z'))||image[1]!=L':'||image[2]!=L'\\')return ERROR_INVALID_NAME;
 memset(p,0,sizeof(*p));p->thread=GetCurrentThreadId();p->phase=PAPACC_PERSONAL_PATHS_READY;p->mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;p->read_diagnostic=read_diagnostic;if(read_diagnostic)memcpy(p->read_public,public_id,32);
 memcpy(p->binary,image,(len+1U)*sizeof(WCHAR));p->original_chain=chain;p->selected_role=role;
 if(selection)p->selected=*selection;else {p->selected.architecture=0x8664U;memcpy(p->selected.provenance,pip_existing_provenance,32);}
 return 0;
#endif
}
DWORD papacc_personal_paths_selected(PAPACC_PERSONAL_INSTALL_PATHS *p,const PAPACC_SCQ_EXISTING_SELECTION *s,DWORD role,const void *chain,volatile LONG *stop)
{return pip_selected_variant(p,s,role,chain,stop,0U,NULL);}
DWORD papacc_personal_paths_selected_machine_read(PAPACC_PERSONAL_INSTALL_PATHS *p,const PAPACC_SCQ_EXISTING_SELECTION *s,DWORD role,const void *chain,volatile LONG *stop,const BYTE public_id[32])
{return pip_selected_variant(p,s,role,chain,stop,1U,public_id);}
static DWORD pip_copy(WCHAR *out,const WCHAR *source,size_t cap)
{
 size_t n=0;if(!source)return ERROR_INVALID_PARAMETER;
 while(n<cap&&source[n])n++;if(!n||n==cap)return ERROR_INSUFFICIENT_BUFFER;
 memcpy(out,source,(n+1U)*sizeof(WCHAR));return 0;
}
static int pip_absolute(const WCHAR *p)
{
 size_t i,n=wcslen(p);
 if(n<3U||!((p[0]>=L'A'&&p[0]<=L'Z')||(p[0]>=L'a'&&p[0]<=L'z'))||p[1]!=L':'||p[2]!=L'\\')return 0;
 for(i=0;i<n;i++)if(p[i]<32U||p[i]==L'"'||p[i]==L'/'||(i!=1U&&p[i]==L':'))return 0;
 for(i=3;i<n;i++)if((i==3U||p[i-1]==L'\\')&&p[i]==L'.'&&(p[i+1]==0||p[i+1]==L'\\'||(p[i+1]==L'.'&&(p[i+2]==0||p[i+2]==L'\\'))))return 0;
 return 1;
}
static DWORD pip_join(WCHAR out[MAX_PATH],const WCHAR *parent,const WCHAR *leaf)
{
 size_t a=wcslen(parent),b=wcslen(leaf);int separator=a&&parent[a-1]!=L'\\';
 if(!pip_absolute(parent)||!b||a+b+(separator?1U:0U)>=MAX_PATH)return ERROR_INVALID_NAME;
 memcpy(out,parent,a*sizeof(WCHAR));if(separator)out[a++]=L'\\';memcpy(out+a,leaf,(b+1U)*sizeof(WCHAR));return 0;
}
static DWORD pip_resolve(PAPACC_PERSONAL_INSTALL_PATHS *p,volatile LONG *stop,int binary_only)
{
 PWSTR program=NULL,data=NULL;HRESULT hr;DWORD e=ERROR_OPERATION_ABORTED;
 if(InterlockedCompareExchange(stop,0,0))return e;
 /* DONT_VERIFY never creates a directory. Returned paths are DATA only. */
 hr=SHGetKnownFolderPath(&FOLDERID_ProgramFiles,KF_FLAG_DONT_VERIFY,NULL,&program);
 if(FAILED(hr)){e=(DWORD)hr;goto done;}if(!program){e=ERROR_INVALID_DATA;goto done;}
 if(InterlockedCompareExchange(stop,0,0))goto done;
 if(binary_only){
  WCHAR a[MAX_PATH];e=pip_copy(a,program,MAX_PATH);if(e)goto done;
  e=pip_join(p->binary,a,L"Papinho\\Accelerator");goto done;
 }
 hr=SHGetKnownFolderPath(&FOLDERID_ProgramData,KF_FLAG_DONT_VERIFY,NULL,&data);
 if(FAILED(hr)){e=(DWORD)hr;goto done;}if(!data){e=ERROR_INVALID_DATA;goto done;}
 if(InterlockedCompareExchange(stop,0,0))goto done;
 /* Bound source lengths BEFORE string operations on API results. */
 {WCHAR a[MAX_PATH],b[MAX_PATH];
 e=pip_copy(a,program,MAX_PATH);if(e)goto done;e=pip_copy(b,data,MAX_PATH);if(e)goto done;
 e=pip_join(p->binary,a,L"Papinho\\Accelerator");if(e)goto done;
 e=pip_join(p->parent,b,L"Papinho");if(e)goto done;
 e=pip_copy(p->leaf,L"Accelerator",65U);if(e)goto done;e=pip_join(p->data,p->parent,p->leaf);}
done:
 /* CoTaskMemFree has no result; record no stronger allocator-close proof. */
 if(data)CoTaskMemFree(data);if(program)CoTaskMemFree(program);return e;
}
DWORD papacc_personal_paths_new(PAPACC_PERSONAL_INSTALL_PATHS *p,volatile LONG *stop)
{
 DWORD e;if(!p||!stop||p->phase)return ERROR_INVALID_STATE;
 memset(p,0,sizeof(*p));
 p->thread=GetCurrentThreadId();p->phase=1;p->mode=PAPACC_PERSONAL_PATHS_NEW;
 e=pip_resolve(p,stop,0);p->error=e;p->phase=e?4U:PAPACC_PERSONAL_PATHS_READY;return e;
}
DWORD papacc_personal_paths_binary(PAPACC_PERSONAL_INSTALL_PATHS *p,volatile LONG *stop)
{
 DWORD e;if(!p||!stop||p->phase)return ERROR_INVALID_STATE;
 memset(p,0,sizeof(*p));p->thread=GetCurrentThreadId();p->phase=1;p->mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;
 e=pip_resolve(p,stop,1);p->error=e;p->phase=e?4U:PAPACC_PERSONAL_PATHS_READY;return e;
}
DWORD papacc_personal_paths_binary_current(const PAPACC_PERSONAL_INSTALL_PATHS *p,volatile LONG *stop)
{
 PAPACC_PERSONAL_INSTALL_PATHS current;DWORD e;
 if(!p||!stop||p->thread!=GetCurrentThreadId()||p->phase!=PAPACC_PERSONAL_PATHS_READY||
 p->mode!=PAPACC_PERSONAL_PATHS_BINARY_ONLY||InterlockedCompareExchange(stop,0,0))return ERROR_INVALID_STATE;
 memset(&current,0,sizeof(current));e=pip_resolve(&current,stop,1);if(e)return e;
 /* Unused data fields must remain empty: cannot smuggle a full-install target. */
 return !memcmp(p->binary,current.binary,sizeof(p->binary))&&!memcmp(p->parent,current.parent,sizeof(p->parent))&&
 !memcmp(p->data,current.data,sizeof(p->data))&&!memcmp(p->leaf,current.leaf,sizeof(p->leaf))?0:ERROR_INVALID_DATA;
}
DWORD papacc_personal_paths_existing(PAPACC_PERSONAL_INSTALL_PATHS *p,const WCHAR *binary,const WCHAR *parent,const WCHAR *leaf)
{
 DWORD e;size_t i,n;ULONG_PTR a,b;const WCHAR *inputs[3];PAPACC_PERSONAL_INSTALL_PATHS value;
 if(!p||p->phase||!binary||!parent||!leaf)return ERROR_INVALID_STATE;
 inputs[0]=binary;inputs[1]=parent;inputs[2]=leaf;a=(ULONG_PTR)p;
 for(i=0;i<3;i++){b=(ULONG_PTR)inputs[i];if(b>=a&&b-a<sizeof(*p))return ERROR_INVALID_PARAMETER;}
 memset(&value,0,sizeof(value));value.thread=GetCurrentThreadId();value.phase=1;value.mode=PAPACC_PERSONAL_PATHS_EXISTING;
 e=pip_copy(value.binary,binary,MAX_PATH);if(e)goto done;e=pip_copy(value.parent,parent,MAX_PATH);if(e)goto done;
 if(!pip_absolute(value.binary)||!pip_absolute(value.parent)){e=ERROR_INVALID_NAME;goto done;}
 e=pip_copy(value.leaf,leaf,65U);if(e)goto done;n=wcslen(value.leaf);
 for(i=0;i<n;i++)if(!((leaf[i]>=L'a'&&leaf[i]<=L'z')||(leaf[i]>=L'A'&&leaf[i]<=L'Z')||(leaf[i]>=L'0'&&leaf[i]<=L'9')||leaf[i]==L'_'||leaf[i]==L'-'||leaf[i]==L'.')){e=ERROR_INVALID_NAME;goto done;}
 if(!wcscmp(leaf,L".")||!wcscmp(leaf,L"..")||leaf[n-1U]==L'.'){e=ERROR_INVALID_NAME;goto done;}
 e=pip_join(value.data,value.parent,value.leaf);
done:value.error=e;value.phase=e?4U:PAPACC_PERSONAL_PATHS_READY;*p=value;return e;
}
DWORD papacc_personal_paths_current(const PAPACC_PERSONAL_INSTALL_PATHS *p,volatile LONG *stop)
{
 PAPACC_PERSONAL_INSTALL_PATHS current;DWORD e;
 if(!p||!stop||p->phase!=PAPACC_PERSONAL_PATHS_READY||InterlockedCompareExchange(stop,0,0))return ERROR_INVALID_STATE;
 if(p->mode==PAPACC_PERSONAL_PATHS_EXISTING)return 0; /* original refs validated separately; no migration */
 if(p->mode!=PAPACC_PERSONAL_PATHS_NEW)return ERROR_INVALID_DATA;
 memset(&current,0,sizeof(current));e=pip_resolve(&current,stop,0);if(e)return e;
 return !memcmp(p->binary,current.binary,sizeof(p->binary))&&!memcmp(p->parent,current.parent,sizeof(p->parent))&&
 !memcmp(p->data,current.data,sizeof(p->data))&&!memcmp(p->leaf,current.leaf,sizeof(p->leaf))?0:ERROR_INVALID_DATA;
}
