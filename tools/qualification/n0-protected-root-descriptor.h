#ifndef PAPACC_N0_PROTECTED_ROOT_DESCRIPTOR_H
#define PAPACC_N0_PROTECTED_ROOT_DESCRIPTOR_H
#include <windows.h>
#include <aclapi.h>
#include <string.h>
#include "storage-hash-capsule/storage_sha256.h"
/* Approved ORIGINAL creation bytes. Never regenerate via SDDL or normalize. */
__declspec(align(4)) static const BYTE n0_root_descriptor[104]={
 0x01,0x00,0x04,0x90,0x14,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x34,0x00,0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
 0x20,0x02,0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
 0x20,0x02,0x00,0x00,0x02,0x00,0x34,0x00,0x02,0x00,0x00,0x00,0x00,0x03,0x14,0x00,
 0xff,0x01,0x1f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,
 0x00,0x03,0x18,0x00,0xff,0x01,0x1f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,
 0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00
};
static int n0_hash_equal(const BYTE *bytes,size_t count,const char *expected)
{
 BYTE hash[32];char hex[65];unsigned i;
 if(!pacc_storage_hash_callback(NULL,bytes,count,hash))return 0;
 for(i=0;i<32;i++){hex[2*i]="0123456789abcdef"[hash[i]>>4];hex[2*i+1]="0123456789abcdef"[hash[i]&15];}
 hex[64]=0;return !strcmp(hex,expected);
}
/* Field validation is separate from exact input hashing. No equivalence bypass. */
static int n0_sd_policy(PSECURITY_DESCRIPTOR sd,DWORD bytes)
{
 BYTE admin[SECURITY_MAX_SID_SIZE],system[SECURITY_MAX_SID_SIZE];DWORD an=sizeof(admin),sn=sizeof(system),revision,i;
 PSID owner,group;BOOL def,present;PACL acl;SECURITY_DESCRIPTOR_CONTROL control;
 if(!sd||bytes<20||bytes>65536||!IsValidSecurityDescriptor(sd)||GetSecurityDescriptorLength(sd)!=bytes||
 !GetSecurityDescriptorControl(sd,&control,&revision)||revision!=1||control!=0x9004||
 !CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admin,&an)||!CreateWellKnownSid(WinLocalSystemSid,NULL,system,&sn)||
 !GetSecurityDescriptorOwner(sd,&owner,&def)||!owner||!EqualSid(owner,admin)||
 !GetSecurityDescriptorGroup(sd,&group,&def)||!group||!EqualSid(group,admin)||
 !GetSecurityDescriptorDacl(sd,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl)||acl->AclRevision!=2||acl->AclSize!=52||acl->AceCount!=2)return 0;
 for(i=0;i<2;i++){
  ACCESS_ALLOWED_ACE *ace;void *p;
  if(!GetAce(acl,i,&p))return 0;ace=(ACCESS_ALLOWED_ACE *)p;
  if(ace->Header.AceType!=ACCESS_ALLOWED_ACE_TYPE||ace->Header.AceFlags!=3||ace->Header.AceSize!=(i?24:20)||
  ace->Mask!=FILE_ALL_ACCESS||!IsValidSid(&ace->SidStart)||!EqualSid(&ace->SidStart,i?admin:system))return 0;
 }
 return 1;
}
static int n0_sd_input(const BYTE *b,size_t n)
{
 return n==104&&n0_hash_equal(b,n,"688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579")&&
 n0_sd_policy((PSECURITY_DESCRIPTOR)b,(DWORD)n);
}
#endif
