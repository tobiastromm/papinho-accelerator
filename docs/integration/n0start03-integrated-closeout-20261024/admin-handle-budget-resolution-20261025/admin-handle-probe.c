/* Isolated native PSS attribution only. No LAB/admission/SCM/ACL operations. */
#define _WIN32_WINNT 0x0603
#include <windows.h>
#include <processsnapshot.h>
#include <stdio.h>
static HPSS snapshot;static HPSSWALK marker;static DWORD first;
__declspec(dllexport) DWORD WINAPI AdminCapture(WCHAR *out,DWORD capacity){DWORD e,z,before,after,rows=0;size_t used=0;PSS_HANDLE_ENTRY entry;int n;
 if(first||snapshot||marker)return ERROR_INVALID_STATE;
 if(!out||capacity<65536)return ERROR_INSUFFICIENT_BUFFER;
 if(!GetProcessHandleCount(GetCurrentProcess(),&before))return GetLastError();
 e=PssCaptureSnapshot(GetCurrentProcess(),PSS_CAPTURE_HANDLES|PSS_CAPTURE_HANDLE_BASIC_INFORMATION|PSS_CAPTURE_HANDLE_NAME_INFORMATION|PSS_CAPTURE_HANDLE_TYPE_SPECIFIC_INFORMATION,0,&snapshot);if(e){if(snapshot){first=e;return e;}goto done;}
 e=PssWalkMarkerCreate(NULL,&marker);if(e){if(marker){first=e;return e;}goto done;}
 for(;;){ZeroMemory(&entry,sizeof(entry));e=PssWalkSnapshot(snapshot,PSS_WALK_HANDLES,marker,&entry,sizeof(entry));if(e==ERROR_NO_MORE_ITEMS){e=0;break;}if(e)break;
  if(++rows>2048){e=ERROR_NOT_ENOUGH_QUOTA;break;}
  n=_snwprintf_s(out+used,capacity-used,_TRUNCATE,L"%llu;%.*ls;%lu\n",(ULONGLONG)(ULONG_PTR)entry.Handle,(entry.Flags&PSS_HANDLE_HAVE_TYPE)?entry.TypeNameLength/2:7,(entry.Flags&PSS_HANDLE_HAVE_TYPE)?entry.TypeName:L"UNKNOWN",entry.GrantedAccess);
  if(n<0){e=ERROR_INSUFFICIENT_BUFFER;break;}used+=(size_t)n;
 }
 done:
 if(marker){z=PssWalkMarkerFree(marker);if(z){first=e?e:z;return first;}marker=NULL;}
 if(snapshot){z=PssFreeSnapshot(GetCurrentProcess(),snapshot);if(z){first=e?e:z;return first;}snapshot=NULL;}
 if(e){first=e;return e;}
 if(!GetProcessHandleCount(GetCurrentProcess(),&after))return GetLastError();
 n=_snwprintf_s(out+used,capacity-used,_TRUNCATE,L"SUMMARY;%lu;%lu;%lu",before,after,rows);return n<0?ERROR_INSUFFICIENT_BUFFER:0;
}
