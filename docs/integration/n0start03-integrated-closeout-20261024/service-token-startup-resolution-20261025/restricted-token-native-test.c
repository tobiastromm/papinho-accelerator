/* Real isolated token/AccessCheck APIs; never a service token. */
#include <windows.h>
#include <stdio.h>
static HANDLE primary,restricted,access_token;
#define T(x) do{if(!(x)){printf("FAIL line=%d error=%lu OWNED_REFS_PRESERVED_NO_RETRY\n",__LINE__,(unsigned long)GetLastError());return 1;}}while(0)
int main(void){BYTE world[SECURITY_MAX_SID_SIZE],nullsid[SECURITY_MAX_SID_SIZE],acl_bytes[256],priv_bytes[512];union {TOKEN_GROUPS aligned;BYTE bytes[4096];} restricted_groups;DWORD wn=sizeof(world),nn=sizeof(nullsid),used,granted,i;SID_AND_ATTRIBUTES restriction;SECURITY_DESCRIPTOR sd;GENERIC_MAPPING map={FILE_GENERIC_READ,FILE_GENERIC_WRITE,FILE_GENERIC_EXECUTE,FILE_ALL_ACCESS};BOOL allowed=TRUE;HANDLE thread=NULL;
 T(!OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&thread)&&GetLastError()==ERROR_NO_TOKEN&&thread==NULL);
 T(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_DUPLICATE|TOKEN_ASSIGN_PRIMARY,&primary));
 T(CreateWellKnownSid(WinWorldSid,NULL,world,&wn)&&CreateWellKnownSid(WinRestrictedCodeSid,NULL,nullsid,&nn));restriction.Sid=nullsid;restriction.Attributes=0;
 /* An already restricted execution context permits only intersection, never
  * invent an unrestricted base token or request elevation for this fixture. */
 if(IsTokenRestricted(primary)){
  T(GetTokenInformation(primary,TokenRestrictedSids,&restricted_groups,sizeof(restricted_groups),&used));
  T(used<=sizeof(restricted_groups));restriction.Sid=NULL;
  for(i=0;i<restricted_groups.aligned.GroupCount;i++)if(!EqualSid(restricted_groups.aligned.Groups[i].Sid,world)){restriction.Sid=restricted_groups.aligned.Groups[i].Sid;break;}
  T(restriction.Sid!=NULL);
 }

 T(CreateRestrictedToken(primary,DISABLE_MAX_PRIVILEGE,0,NULL,0,NULL,1,&restriction,&restricted));
 T(IsTokenRestricted(restricted));
 T(DuplicateToken(restricted,SecurityImpersonation,&access_token));
 T(InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION)&&SetSecurityDescriptorOwner(&sd,world,FALSE)&&SetSecurityDescriptorGroup(&sd,world,FALSE));T(InitializeAcl((PACL)acl_bytes,sizeof(acl_bytes),ACL_REVISION)&&AddAccessAllowedAce((PACL)acl_bytes,ACL_REVISION,FILE_GENERIC_READ,world)&&SetSecurityDescriptorDacl(&sd,TRUE,(PACL)acl_bytes,FALSE));
 used=sizeof(priv_bytes);T(AccessCheck(&sd,access_token,FILE_GENERIC_READ,&map,(PPRIVILEGE_SET)priv_bytes,&used,&granted,&allowed));T(!allowed);T(CloseHandle(access_token));access_token=NULL;T(CloseHandle(restricted));restricted=NULL;T(CloseHandle(primary));primary=NULL;
 puts("RESTRICTED_TOKEN_NATIVE=PASS restriction_denies_even_when_world_allow_present THREAD_NO_IMPERSONATION CLOSE_CONFIRMED NOT_SERVICE_IDENTITY_EVIDENCE");return 0;}
