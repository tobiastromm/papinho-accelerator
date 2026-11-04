static int scqi_directory_sd_scope(PSECURITY_DESCRIPTOR sd,PSID actor,PSID installer,int anchor,int shared)
{
 PSID owner;PACL acl;BOOL def,present;DWORD i,revision;SECURITY_DESCRIPTOR_CONTROL control;
 const DWORD write=GENERIC_ALL|GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|FILE_DELETE_CHILD;
 if(!sd||!IsValidSecurityDescriptor(sd)||!GetSecurityDescriptorControl(sd,&control,&revision)||!(control&SE_DACL_PROTECTED)||
  !GetSecurityDescriptorOwner(sd,&owner,&def)||!owner||!IsValidSid(owner)||
  (!EqualSid(owner,actor)&&!IsWellKnownSid(owner,WinLocalSystemSid)&&!IsWellKnownSid(owner,WinBuiltinAdministratorsSid)&&!EqualSid(owner,installer))||
  !GetSecurityDescriptorDacl(sd,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl)||!acl->AceCount)return 0;
 for(i=0;i<acl->AceCount;i++){
  ACCESS_ALLOWED_ACE *a;PSID sid;
  if(!GetAce(acl,i,(void **)&a)||a->Header.AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U||
   (a->Header.AceType!=ACCESS_ALLOWED_ACE_TYPE&&a->Header.AceType!=ACCESS_DENIED_ACE_TYPE))return 0;
  sid=&a->SidStart;
  if(!IsValidSid(sid)||GetLengthSid(sid)>(DWORD)(a->Header.AceSize-FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)))return 0;
  if(a->Header.AceFlags&INHERIT_ONLY_ACE)continue;
  if(a->Header.AceType==ACCESS_ALLOWED_ACE_TYPE&&(a->Mask&write)&&!EqualSid(sid,actor)&&
   !IsWellKnownSid(sid,WinLocalSystemSid)&&!IsWellKnownSid(sid,WinBuiltinAdministratorsSid)&&!EqualSid(sid,installer)){
   if(anchor&&!a->Header.AceFlags&&a->Mask==FILE_ADD_SUBDIRECTORY&&IsWellKnownSid(sid,WinAuthenticatedUserSid))continue;
   if(shared&&a->Header.AceFlags==CONTAINER_INHERIT_ACE&&a->Mask==0x116U&&IsWellKnownSid(sid,WinBuiltinUsersSid))continue;
   return 0;
  }
 }
 return 1;
}
static int scqi_directory_sd(PSECURITY_DESCRIPTOR sd,PSID actor,PSID installer,int anchor)
{return scqi_directory_sd_scope(sd,actor,installer,anchor,0);}
