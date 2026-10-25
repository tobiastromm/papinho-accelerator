static DWORD cycle_access_file(HANDLE file,unsigned index)
{
 DWORD e,n=1024,granted=0;BOOL allowed=FALSE;BYTE privileges[1024];
 GENERIC_MAPPING map={FILE_GENERIC_READ,FILE_GENERIC_WRITE,FILE_GENERIC_EXECUTE,FILE_ALL_ACCESS};
 if(cycle_sd)return ERROR_INVALID_STATE;
 e=GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&cycle_sd);
 if(!e&&(!AccessCheck(cycle_sd,cycle_access,MAXIMUM_ALLOWED,&map,(PRIVILEGE_SET *)privileges,&n,&granted,&allowed)||!allowed||
 (granted&(FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|DELETE|WRITE_DAC|WRITE_OWNER|FILE_DELETE_CHILD))||
 (granted&(FILE_READ_DATA|(index==2?FILE_EXECUTE:0)))!=(DWORD)(FILE_READ_DATA|(index==2?FILE_EXECUTE:0))))e=ERROR_ACCESS_DENIED;
 printf("LAB_EFFECTIVE_ACL index=%u granted=%08lx status=%lu\n",index,(unsigned long)granted,(unsigned long)e);
 if(cycle_sd){if(LocalFree(cycle_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_sd=NULL;}return e;
}
