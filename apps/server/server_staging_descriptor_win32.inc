enum { STAGE_SD_VALID=0U,STAGE_SD_INVALID=1U,STAGE_SD_OWNER_QUERY=2U,
 STAGE_SD_OWNER_INVALID=3U,STAGE_SD_PLATFORM_SID=4U,STAGE_SD_DACL_QUERY=5U,
 STAGE_SD_DACL_INVALID=6U,STAGE_SD_OWNER_UNTRUSTED=7U,STAGE_SD_ACE_INVALID=8U,
 STAGE_SD_ACE_UNSUPPORTED=9U,STAGE_SD_ACE_SID_INVALID=10U,STAGE_SD_FOREIGN_WRITE=11U,
 STAGE_SD_APPLICATION_POLICY=12U,STAGE_SD_ANCHOR_CHILD=13U };
/* Private policy composition, never a caller-supplied authority or setter. */
static DWORD staging_descriptor_classify_policy(PSECURITY_DESCRIPTOR d,int ancestor,int anchor,DWORD *ace,DWORD *mask)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&root.configuration_commit.pending_commit.personal_native;
 PAPACC_QUALIFICATION_STAGING *s=&n->staging;PSID owner;BOOL def,present;PACL acl;DWORD i;
 BYTE system[SECURITY_MAX_SID_SIZE],admins[SECURITY_MAX_SID_SIZE];DWORD sn=sizeof(system),an=sizeof(admins);
 const DWORD write_mask=GENERIC_ALL|GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|FILE_DELETE_CHILD;
 *ace=MAXDWORD;*mask=0;
 if(!ancestor)return scm_descriptor(d,n->sid)?STAGE_SD_VALID:STAGE_SD_APPLICATION_POLICY;
 if(!IsValidSecurityDescriptor(d))return STAGE_SD_INVALID;
 if(!GetSecurityDescriptorOwner(d,&owner,&def))return STAGE_SD_OWNER_QUERY;
 if(!owner||!IsValidSid(owner))return STAGE_SD_OWNER_INVALID;
 if(!CreateWellKnownSid(WinLocalSystemSid,NULL,system,&sn)||!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admins,&an))return STAGE_SD_PLATFORM_SID;
 if(!GetSecurityDescriptorDacl(d,&present,&acl,&def))return STAGE_SD_DACL_QUERY;
 if(!present||!acl||!IsValidAcl(acl))return STAGE_SD_DACL_INVALID;
 if(!EqualSid(owner,n->sid)&&!EqualSid(owner,system)&&!EqualSid(owner,admins)&&!EqualSid(owner,s->trusted_installer))return STAGE_SD_OWNER_UNTRUSTED;
 for(i=0;i<acl->AceCount;i++){
  PACE_HEADER h;ACCESS_ALLOWED_ACE *a;PSID sid;
  *ace=i;
  if(!GetAce(acl,i,(void **)&h)||h->AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U)return STAGE_SD_ACE_INVALID;
  if(h->AceFlags&INHERIT_ONLY_ACE)continue;
  if(h->AceType!=ACCESS_ALLOWED_ACE_TYPE&&h->AceType!=ACCESS_DENIED_ACE_TYPE)return STAGE_SD_ACE_UNSUPPORTED;
  a=(ACCESS_ALLOWED_ACE *)h;sid=(PSID)&a->SidStart;*mask=a->Mask;
  if(!IsValidSid(sid)||GetLengthSid(sid)>(DWORD)(h->AceSize-FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)))return STAGE_SD_ACE_SID_INVALID;
  if(h->AceType==ACCESS_ALLOWED_ACE_TYPE&&(a->Mask&write_mask)&&!EqualSid(sid,n->sid)&&!EqualSid(sid,system)&&!EqualSid(sid,admins)&&!EqualSid(sid,s->trusted_installer)){
   /* Exact self-only AU create-child ACE at a volume anchor. No generic
    * right, inheritance, DELETE or other write bit is waived. */
   if(anchor&&h->AceFlags==0U&&a->Mask==FILE_ADD_SUBDIRECTORY&&IsWellKnownSid(sid,WinAuthenticatedUserSid))continue;
   return STAGE_SD_FOREIGN_WRITE;
  }
 }
 *ace=MAXDWORD;*mask=0;return STAGE_SD_VALID;
}
static DWORD staging_descriptor_classify(PSECURITY_DESCRIPTOR d,int ancestor,DWORD *ace,DWORD *mask)
{return staging_descriptor_classify_policy(d,ancestor,0,ace,mask);}
static DWORD staging_anchor_classify(PSECURITY_DESCRIPTOR anchor,PSECURITY_DESCRIPTOR child,DWORD *ace,DWORD *mask)
{
 DWORD reason,revision;SECURITY_DESCRIPTOR_CONTROL control;
 if(!anchor||!child){*ace=MAXDWORD;*mask=0;return STAGE_SD_ANCHOR_CHILD;}
 reason=staging_descriptor_classify(anchor,1,ace,mask);
 if(reason!=STAGE_SD_FOREIGN_WRITE)return reason; /* strict roots unchanged */
 if(!IsValidSecurityDescriptor(child)||!GetSecurityDescriptorControl(child,&control,&revision)||!(control&SE_DACL_PROTECTED)){
  *ace=MAXDWORD;*mask=0;return STAGE_SD_ANCHOR_CHILD;
 }
 reason=staging_descriptor_classify(child,1,ace,mask);if(reason)return reason;
 return staging_descriptor_classify_policy(anchor,1,1,ace,mask);
}
