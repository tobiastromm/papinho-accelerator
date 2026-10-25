/* Isolated focal SD tests/file evidence. No admission, SCM, launcher or writer. */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <aclapi.h>
#include <sddl.h>
#include <stdio.h>
#include <string.h>
#include "../tools/qualification/storage-hash-capsule/storage_sha256.h"
static unsigned cases;
#define T(x) do{if(!(x)){printf("SD_TEST_FAIL line=%d case=%u\n",__LINE__,cases);return 1;}}while(0)
static void hex(const BYTE *bytes,DWORD n)
{DWORD i;for(i=0;i<n;i++)printf("%02x",(unsigned)bytes[i]);}
static int digest(PSECURITY_DESCRIPTOR sd,BYTE out[32])
{DWORD n=GetSecurityDescriptorLength(sd);return IsValidSecurityDescriptor(sd)&&n&&n<=65536&&pacc_storage_hash_callback(NULL,(BYTE *)sd,n,out);}
static int sid_equal(PSID a,PSID b)
{return (!a&&!b)||(a&&b&&IsValidSid(a)&&IsValidSid(b)&&EqualSid(a,b));}
/* Ordered security fields only; no claim of arbitrary access-check equivalence. */
static int fields_equal(PSECURITY_DESCRIPTOR a,PSECURITY_DESCRIPTOR b)
{
 SECURITY_DESCRIPTOR_CONTROL ac,bc;DWORD ar,br;PSID ao,bo,ag,bg;BOOL ad,bd,ap,bp;PACL aa,ba;
 if(!IsValidSecurityDescriptor(a)||!IsValidSecurityDescriptor(b)||!GetSecurityDescriptorControl(a,&ac,&ar)||!GetSecurityDescriptorControl(b,&bc,&br)||ac!=bc||ar!=br)return 0;
 if(!GetSecurityDescriptorOwner(a,&ao,&ad)||!GetSecurityDescriptorOwner(b,&bo,&bd)||ad!=bd||!sid_equal(ao,bo)||!GetSecurityDescriptorGroup(a,&ag,&ad)||!GetSecurityDescriptorGroup(b,&bg,&bd)||ad!=bd||!sid_equal(ag,bg))return 0;
 if(!GetSecurityDescriptorDacl(a,&ap,&aa,&ad)||!GetSecurityDescriptorDacl(b,&bp,&ba,&bd)||ap!=bp||ad!=bd)return 0;
 if((!aa)!=(!ba)||(aa&&(aa->AclSize!=ba->AclSize||memcmp(aa,ba,aa->AclSize))))return 0;
 if(!GetSecurityDescriptorSacl(a,&ap,&aa,&ad)||!GetSecurityDescriptorSacl(b,&bp,&ba,&bd)||ap!=bp||ad!=bd)return 0;
 return (!aa&&!ba)||(aa&&ba&&aa->AclSize==ba->AclSize&&!memcmp(aa,ba,aa->AclSize));
}
static int model(void)
{
 PSECURITY_DESCRIPTOR a=NULL,b=NULL;DWORD n;BYTE original[32],other[32];
 union {ULONGLONG align;BYTE data[1024];} moved;SECURITY_DESCRIPTOR_RELATIVE *relative;
 static const WCHAR *changed[]={
 L"O:SYG:BAD:P(A;;FR;;;BA)(A;;FR;;;SY)",
 L"O:BAG:BAD:P(A;;FA;;;BA)(A;;FR;;;SY)",
 L"O:SYG:SYD:P(A;;FA;;;BA)(A;;FR;;;SY)",
 L"O:SYG:BAD:(A;;FA;;;BA)(A;;FR;;;SY)",
 L"O:SYG:BAD:P(A;;FR;;;SY)(A;;FA;;;BA)",
 L"O:SYG:BAD:P(A;CI;FA;;;BA)(A;;FR;;;SY)",
 L"O:SYG:BAD:P(A;;FA;;;BA)(A;;FR;;;SY)S:(AU;SA;FR;;;WD)",
 L"O:SYG:BA",
 L"O:SYG:BAD:P",
 L"O:SYG:BAD:NO_ACCESS_CONTROL"};unsigned i;
 T(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:SYG:BAD:P(A;;FA;;;BA)(A;;FR;;;SY)",SDDL_REVISION_1,&a,NULL));T(digest(a,original));++cases;
 T(digest(a,other)&&!memcmp(original,other,32));++cases;
 n=GetSecurityDescriptorLength(a);T(n+16<=sizeof(moved.data));ZeroMemory(&moved,sizeof(moved));
 memcpy(moved.data,a,sizeof(SECURITY_DESCRIPTOR_RELATIVE));memcpy(moved.data+sizeof(SECURITY_DESCRIPTOR_RELATIVE)+16,(BYTE *)a+sizeof(SECURITY_DESCRIPTOR_RELATIVE),n-sizeof(SECURITY_DESCRIPTOR_RELATIVE));
 relative=(SECURITY_DESCRIPTOR_RELATIVE *)moved.data;
 if(relative->Owner)relative->Owner+=16;if(relative->Group)relative->Group+=16;if(relative->Dacl)relative->Dacl+=16;if(relative->Sacl)relative->Sacl+=16;
 T(fields_equal(a,moved.data));T(digest(moved.data,other)&&memcmp(original,other,32));++cases;
 /* SAME security fields, different valid relative offsets => exact-byte gate DENY.
  * Never apply descriptor changes to any file or normalize permissions. */
 for(i=0;i<sizeof(changed)/sizeof(changed[0]);i++){
  T(ConvertStringSecurityDescriptorToSecurityDescriptorW(changed[i],SDDL_REVISION_1,&b,NULL));T(!fields_equal(a,b)&&digest(b,other)&&memcmp(original,other,32));T(LocalFree(b)==NULL);b=NULL;++cases;
 }
 T(LocalFree(a)==NULL);
 printf("N0_SD_MODEL=PASS cases=%u; EXACT_SHA256_PRESERVED; EQUIVALENT_FIELDS_DIFFERENT_LAYOUT_DENY; ACL_OWNER_GROUP_CONTROL_ORDER_INHERITANCE_DIFFERENCES_DENY; NO_OS_ACL_MUTATION\n",cases);return 0;
}
static int file_evidence(void)
{
 HANDLE file=NULL;PSECURITY_DESCRIPTOR sd=NULL,after=NULL;LPWSTR sddl=NULL;BY_HANDLE_FILE_INFORMATION id,end;DWORD e=0,n=0,size,revision;SECURITY_DESCRIPTOR_CONTROL control;
 BYTE hash[32],raw_hash[32],sddl_hash[32],after_hash[32],block[1024];PACC_STORAGE_SHA256 sha;WCHAR path[512];int close_ok=1;const char *stage="OPEN_FILE";
 file=CreateFileW(L"C:\\Windows\\System32\\conhost.exe",GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(file==INVALID_HANDLE_VALUE){file=NULL;e=GetLastError();goto done;}
 if(!GetFileInformationByHandle(file,&id)||GetFileType(file)!=FILE_TYPE_DISK||(id.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY))){e=ERROR_INVALID_DATA;goto done;}
 n=GetFinalPathNameByHandleW(file,path,512,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);if(!n||n>=512||wcscmp(path,L"\\\\?\\C:\\Windows\\System32\\conhost.exe")){e=ERROR_INVALID_NAME;goto done;}
 stage="GET_SD";e=GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&sd);if(e)goto done;
 stage="SD_RAW_AND_CONTROL";if(!digest(sd,raw_hash)||!GetSecurityDescriptorControl(sd,&control,&revision)||!(control&SE_SELF_RELATIVE)){e=ERROR_INVALID_DATA;goto done;}
 printf("N0_SD_FIRST_NATIVE_SHA256=");hex(raw_hash,32);printf("\n");
 size=GetSecurityDescriptorLength(sd);if(size>512){e=ERROR_INSUFFICIENT_BUFFER;goto done;}
 stage="SD_TO_SDDL";if(!ConvertSecurityDescriptorToStringSecurityDescriptorW(sd,SDDL_REVISION_1,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,&sddl,NULL)||wcslen(sddl)>4096||!pacc_storage_hash_callback(NULL,(BYTE *)sddl,wcslen(sddl)*sizeof(WCHAR),sddl_hash)){e=ERROR_INVALID_DATA;goto done;}
 stage="HASH_FILE";ZeroMemory(&sha,sizeof(sha));if(pacc_storage_sha_init(&sha)!=PACC_HASH_OK){e=ERROR_INVALID_DATA;goto done;}
 for(;;){if(!ReadFile(file,block,sizeof(block),&n,NULL)){e=GetLastError();goto done;}if(!n)break;if(pacc_storage_sha_update(&sha,block,n,NULL,NULL)!=PACC_HASH_OK){e=ERROR_INVALID_DATA;goto done;}}
 if(pacc_storage_sha_final(&sha,hash,32,NULL,NULL)!=PACC_HASH_OK){e=ERROR_INVALID_DATA;goto done;}
 stage="REVALIDATE_FILE_ID";if(!GetFileInformationByHandle(file,&end)){e=GetLastError();goto done;}
 /* LastAccessTime is read-side metadata, not native object identity. */
 if(id.dwVolumeSerialNumber!=end.dwVolumeSerialNumber||id.nFileIndexHigh!=end.nFileIndexHigh||id.nFileIndexLow!=end.nFileIndexLow||id.nFileSizeHigh!=end.nFileSizeHigh||id.nFileSizeLow!=end.nFileSizeLow||id.nNumberOfLinks!=end.nNumberOfLinks||id.dwFileAttributes!=end.dwFileAttributes||memcmp(&id.ftLastWriteTime,&end.ftLastWriteTime,sizeof(FILETIME))){e=ERROR_INVALID_DATA;goto done;}
 stage="REVALIDATE_SD";e=GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&after);if(e)goto done;
 if(!digest(after,after_hash)||memcmp(raw_hash,after_hash,32)||!fields_equal(sd,after)){e=ERROR_INVALID_DATA;goto done;}
 printf("N0_SD_FILE identity=%lu:%lu:%lu length=%lu:%lu links=%lu; ORIGINAL_FILE_HANDLE_HELD; NO_PROCESS_IMAGE_CLAIM\n",(unsigned long)id.dwVolumeSerialNumber,(unsigned long)id.nFileIndexHigh,(unsigned long)id.nFileIndexLow,(unsigned long)id.nFileSizeHigh,(unsigned long)id.nFileSizeLow,(unsigned long)id.nNumberOfLinks);
 printf("N0_SD_FILE_SHA256=");hex(hash,32);printf("\nN0_SD_NATIVE_SHA256=");hex(raw_hash,32);printf("\nN0_SD_SDDL_UTF16LE_SHA256=");hex(sddl_hash,32);
 printf("\nN0_SD_NATIVE length=%lu control=0x%04x revision=%lu owner=%lu group=%lu dacl=%lu sacl=%lu; QUERY=OWNER_GROUP_DACL; SACL_NOT_REQUESTED_NOT_PROVED_ABSENT\n",(unsigned long)size,(unsigned)control,(unsigned long)revision,(unsigned long)((SECURITY_DESCRIPTOR_RELATIVE *)sd)->Owner,(unsigned long)((SECURITY_DESCRIPTOR_RELATIVE *)sd)->Group,(unsigned long)((SECURITY_DESCRIPTOR_RELATIVE *)sd)->Dacl,(unsigned long)((SECURITY_DESCRIPTOR_RELATIVE *)sd)->Sacl);
 printf("N0_SD_NATIVE_HEX=");hex((BYTE *)sd,size);printf("\nN0_SD_SDDL=%ls\nN0_SD_TWO_SAMPLES_STABLE=YES; NOT_CONTINUOUS_PROOF; NO_NORMALIZATION\n",sddl);
 done:
 if(sddl&&LocalFree(sddl))close_ok=0;if(after&&LocalFree(after))close_ok=0;if(sd&&LocalFree(sd))close_ok=0;if(file&&!CloseHandle(file))close_ok=0;
 if(!close_ok){printf("N0_SD_FILE_RETAINED first=%lu; CLOSE_UNKNOWN_NO_RETRY\n",(unsigned long)e);fflush(stdout);Sleep(INFINITE);return 2;}
 printf("N0_SD_FILE_FINAL stage=%s error=%lu refs_closed=1; NO_SCM_NO_PREFLIGHT_NO_ACL_MUTATION\n",stage,(unsigned long)e);return e?2:0;
}
int main(int argc,char **argv)
{if(argc!=2)return 2;if(!strcmp(argv[1],"--model"))return model();if(!strcmp(argv[1],"--file"))return file_evidence();return 2;}
