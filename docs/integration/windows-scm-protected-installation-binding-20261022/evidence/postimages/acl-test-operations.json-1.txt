/* In-memory native descriptor MODEL, not an operator or real file ACL. */
#include <windows.h>
#include <aclapi.h>
#include <stdio.h>
#define PAPACC_SCM_DESCRIPTOR_TEST 1
#include "../apps/server/server_launcher_scm_prepare_win32.inc"
#define T(x) do{if(!(x)){printf("FAIL line=%d'+chr(92)+'n",__LINE__);return 1;}}while(0)
int main(void)
{
 BYTE admin[SECURITY_MAX_SID_SIZE],system[SECURITY_MAX_SID_SIZE],world[SECURITY_MAX_SID_SIZE],storage[1024];
 DWORD an=sizeof(admin),sn=sizeof(system),wn=sizeof(world);SECURITY_DESCRIPTOR sd;PACL acl=(PACL)storage;
 T(CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admin,&an));T(CreateWellKnownSid(WinLocalSystemSid,NULL,system,&sn));T(CreateWellKnownSid(WinWorldSid,NULL,world,&wn));
 T(InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION));T(SetSecurityDescriptorOwner(&sd,admin,FALSE));
 T(SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE));T(!scm_descriptor(&sd,admin));
 T(InitializeAcl(acl,sizeof(storage),ACL_REVISION));T(AddAccessAllowedAce(acl,ACL_REVISION,GENERIC_ALL,admin));T(AddAccessAllowedAce(acl,ACL_REVISION,GENERIC_ALL,system));
 T(SetSecurityDescriptorDacl(&sd,TRUE,acl,FALSE));T(scm_descriptor(&sd,admin));
 T(AddAccessAllowedAce(acl,ACL_REVISION,GENERIC_READ,world));T(scm_descriptor(&sd,admin));
 T(AddAccessAllowedAce(acl,ACL_REVISION,FILE_WRITE_DATA,world));T(!scm_descriptor(&sd,admin));
 T(InitializeAcl(acl,sizeof(storage),ACL_REVISION));T(AddAccessAllowedAce(acl,ACL_REVISION,WRITE_DAC,world));T(!scm_descriptor(&sd,admin));
 T(InitializeAcl(acl,sizeof(storage),ACL_REVISION));T(AddAccessAllowedAce(acl,ACL_REVISION,GENERIC_ALL,admin));T(scm_descriptor(&sd,admin));
 T(SetSecurityDescriptorOwner(&sd,world,FALSE));T(!scm_descriptor(&sd,admin));
 T(SetSecurityDescriptorOwner(&sd,admin,FALSE));T(SetSecurityDescriptorDacl(&sd,FALSE,NULL,FALSE));T(!scm_descriptor(&sd,admin));
 puts("Native descriptor MODEL: protected owner/writers, world read, unsafe write/owner/null/absent DACL PASS");return 0;
}
