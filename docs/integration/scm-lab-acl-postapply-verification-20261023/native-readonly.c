#include <windows.h>
#include <aclapi.h>
#include <sddl.h>
#include <stdio.h>
#define PAPACC_SCM_DESCRIPTOR_TEST 1
#include "C:/Projetos/PapinhoAccelerator/apps/server/server_launcher_scm_prepare_win32.inc"
int main(void){
 HANDLE h;BY_HANDLE_FILE_INFORMATION i;PSECURITY_DESCRIPTOR sd=NULL;PSID sid=NULL;DWORD e;int ok=0,close_ok;
 h=CreateFileW(L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release",READ_CONTROL|FILE_READ_ATTRIBUTES,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(h==INVALID_HANDLE_VALUE)return 2;
 if(!GetFileInformationByHandle(h,&i)||i.dwVolumeSerialNumber!=417614100UL||i.nFileIndexHigh!=1966080UL||i.nFileIndexLow!=79426UL||(i.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))!=FILE_ATTRIBUTE_DIRECTORY)goto done;
 e=GetSecurityInfo(h,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&sd);if(e!=ERROR_SUCCESS)goto done;
 if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&sid))goto done;
 ok=scm_descriptor(sd,(BYTE *)sid);printf("PARENT_ACL_VALIDATOR=%s; OBJECT=%lu:%lu:%lu; OPERATOR_PARAMETER=PINNED_RECEIPT_NOT_AUTHORIZATION; ACL_MUTATIONS=NONE\n",ok?"PASS":"FAIL",i.dwVolumeSerialNumber,i.nFileIndexHigh,i.nFileIndexLow);
 done:
 if(sid&&LocalFree(sid))ok=0;if(sd&&LocalFree(sd))ok=0;close_ok=CloseHandle(h)!=0;printf("HANDLE_CLOSED=%d\n",close_ok);return ok&&close_ok?0:2;
}
