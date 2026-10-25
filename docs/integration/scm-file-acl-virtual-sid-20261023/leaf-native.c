#include <windows.h>
#include <aclapi.h>
#include <sddl.h>
#include <stdio.h>
#define PAPACC_SCM_DESCRIPTOR_TEST 1
#include "C:/Projetos/PapinhoAccelerator/apps/server/server_launcher_scm_prepare_win32.inc"
static int check(const WCHAR *leaf,DWORD high,DWORD low,PSID sid){
 WCHAR path[512];HANDLE h;BY_HANDLE_FILE_INFORMATION i;PSECURITY_DESCRIPTOR sd=NULL;int ok=0,result=-1;
 lstrcpyW(path,L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\");lstrcatW(path,leaf);
 h=CreateFileW(path,READ_CONTROL|FILE_READ_ATTRIBUTES,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(h==INVALID_HANDLE_VALUE)return 0;
 if(!GetFileInformationByHandle(h,&i)||i.dwVolumeSerialNumber!=417614100UL||i.nFileIndexHigh!=high||i.nFileIndexLow!=low||(i.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))goto done;
 if(GetSecurityInfo(h,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&sd)!=ERROR_SUCCESS)goto done;
 result=scm_descriptor(sd,(BYTE *)sid);ok=result==0;
 printf("LEAF=%ls; ID=%lu:%lu:%lu; EXISTING_SCM_DESCRIPTOR=%d; EXPECTED_DENY=%d\n",leaf,i.dwVolumeSerialNumber,i.nFileIndexHigh,i.nFileIndexLow,result,ok);
 done:if(sd&&LocalFree(sd))ok=0;if(!CloseHandle(h))ok=0;return ok;
}
int main(void){PSID sid=NULL;int a,b;if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&sid))return 2;
 a=check(L"papacc.conf",393216UL,1120240UL,sid);b=check(L"papacc_server.exe",262144UL,1119989UL,sid);if(LocalFree(sid))return 2;
 puts("READ_ONLY; NO_SCM_BINDING_INVOCATION; NO_ACL_MUTATION; OPERATOR_SID_FROM_RECEIPT_NOT_AUTHORIZATION");return a&&b?0:2;}
