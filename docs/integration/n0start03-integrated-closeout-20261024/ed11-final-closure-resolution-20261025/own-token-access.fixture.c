#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
static unsigned checks;
#define CHECK(x) do{checks++;if(!(x)){first=GetLastError();if(!first)first=ERROR_INVALID_DATA;goto done;}}while(0)
int main(void){
 HANDLE primary=NULL,access=NULL;DWORD first=0,n,granted;BOOL allowed,member;BYTE ub[1024],ab[1024],pb[1024],system_sid[SECURITY_MAX_SID_SIZE];DWORD system_n=sizeof(system_sid);TOKEN_USER *u=(TOKEN_USER *)ub;TOKEN_STATISTICS a,b;SECURITY_DESCRIPTOR sd;GENERIC_MAPPING map={FILE_GENERIC_READ,FILE_GENERIC_WRITE,FILE_GENERIC_EXECUTE,FILE_ALL_ACCESS};const DWORD desired[]={FILE_READ_DATA,FILE_EXECUTE,FILE_WRITE_DATA,WRITE_DAC};unsigned i;
 CHECK(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY|TOKEN_DUPLICATE,&primary));CHECK(GetTokenInformation(primary,TokenStatistics,&a,sizeof(a),&n));CHECK(a.TokenType==TokenPrimary);CHECK(GetTokenInformation(primary,TokenUser,ub,sizeof(ub),&n));CHECK(n<=sizeof(ub)&&u->User.Sid&&(BYTE *)u->User.Sid>=ub&&(BYTE *)u->User.Sid+8<=ub+n);CHECK(IsValidSid(u->User.Sid));CHECK((BYTE *)u->User.Sid+GetLengthSid(u->User.Sid)<=ub+n);
 CHECK(DuplicateToken(primary,SecurityImpersonation,&access));CHECK(CheckTokenMembership(access,u->User.Sid,&member));CHECK(member);
 CHECK(InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION));CHECK(CreateWellKnownSid(WinLocalSystemSid,NULL,system_sid,&system_n));CHECK(!EqualSid(u->User.Sid,system_sid));CHECK(SetSecurityDescriptorOwner(&sd,system_sid,FALSE));CHECK(SetSecurityDescriptorGroup(&sd,u->User.Sid,FALSE));CHECK(InitializeAcl((PACL)ab,sizeof(ab),ACL_REVISION));CHECK(AddAccessDeniedAce((PACL)ab,ACL_REVISION,FILE_WRITE_DATA|WRITE_DAC,u->User.Sid));CHECK(AddAccessAllowedAce((PACL)ab,ACL_REVISION,FILE_GENERIC_READ|FILE_GENERIC_EXECUTE,u->User.Sid));CHECK(SetSecurityDescriptorDacl(&sd,TRUE,(PACL)ab,FALSE));
 for(i=0;i<4;i++){n=sizeof(pb);allowed=FALSE;granted=0;CHECK(AccessCheck(&sd,access,desired[i],&map,(PPRIVILEGE_SET)pb,&n,&granted,&allowed));CHECK((allowed?1:0)==(i<2?1:0));}
 /* Separate owner-rights case: an owner may receive WRITE_DAC implicitly. */
 CHECK(SetSecurityDescriptorOwner(&sd,u->User.Sid,FALSE));n=sizeof(pb);CHECK(AccessCheck(&sd,access,WRITE_DAC,&map,(PPRIVILEGE_SET)pb,&n,&granted,&allowed));CHECK(allowed);
 CHECK(GetTokenInformation(primary,TokenStatistics,&b,sizeof(b),&n));CHECK(a.TokenId.LowPart==b.TokenId.LowPart&&a.TokenId.HighPart==b.TokenId.HighPart&&a.AuthenticationId.LowPart==b.AuthenticationId.LowPart&&a.AuthenticationId.HighPart==b.AuthenticationId.HighPart&&a.ModifiedId.LowPart==b.ModifiedId.LowPart&&a.ModifiedId.HighPart==b.ModifiedId.HighPart);
 done:
 if(access){if(!CloseHandle(access)){if(!first)first=GetLastError();puts("ACCESS_CLOSE=UNKNOWN_NO_RETRY");}else access=NULL;}
 if(primary){if(!CloseHandle(primary)){if(!first)first=GetLastError();puts("PRIMARY_CLOSE=UNKNOWN_NO_RETRY");}else primary=NULL;}
 printf("OWN_TOKEN_ACCESS_FIXTURE checks=%u first=%lu closed=%u; IN_MEMORY_SD_ONLY; NOT_SERVICE_TOKEN_NOT_OPERATIONAL_PROOF\n",checks,(unsigned long)first,(unsigned)(access==NULL&&primary==NULL));return first?1:0;
}
