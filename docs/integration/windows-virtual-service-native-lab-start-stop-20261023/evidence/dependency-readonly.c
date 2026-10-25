#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <sddl.h>
#define PAPACC_SCM_DESCRIPTOR_TEST 1
#include "C:/Projetos/PapinhoAccelerator/apps/server/server_launcher_scm_prepare_win32.inc"
int main(void){
 const WCHAR *v[]={L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\libcrypto-3-x64.dll",L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\libssl-3-x64.dll"};
 unsigned i;PSID op=NULL;DWORD close_errors=0;
 if(!ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&op))return 3;
 for(i=0;i<2;i++){
 HANDLE h;BY_HANDLE_FILE_INFORMATION a;PSECURITY_DESCRIPTOR sd=NULL;LPWSTR text=NULL;DWORD e,rev;SECURITY_DESCRIPTOR_CONTROL control;
 h=CreateFileW(v[i],GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);if(h==INVALID_HANDLE_VALUE)return 3;
 if(!GetFileInformationByHandle(h,&a)||(a.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))return 3;
 e=GetSecurityInfo(h,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&sd);if(e)return 3;
 if(!GetSecurityDescriptorControl(sd,&control,&rev)||!ConvertSecurityDescriptorToStringSecurityDescriptorW(sd,SDDL_REVISION_1,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,&text,NULL))return 3;
 printf("FILE=%ls\nNATIVE=%lu:%lu:%lu; BYTES=%lu; DACL_PROTECTED=%u; EXISTING_SCM_DESCRIPTOR=%s\nSDDL=%ls\n",v[i],(unsigned long)a.dwVolumeSerialNumber,(unsigned long)a.nFileIndexHigh,(unsigned long)a.nFileIndexLow,(unsigned long)a.nFileSizeLow,(control&SE_DACL_PROTECTED)?1:0,scm_descriptor(sd,(BYTE *)op)?"PASS":"DENY",text);
 if(LocalFree(text))close_errors++;if(LocalFree(sd))close_errors++;if(!CloseHandle(h))close_errors++;
 }
 if(LocalFree(op))close_errors++;printf("CHECKED_CLOSE_ERRORS=%lu; MUTATIONS=NONE\n",(unsigned long)close_errors);return close_errors?3:0;
}
