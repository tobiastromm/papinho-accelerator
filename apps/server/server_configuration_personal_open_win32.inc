PAPACC_RESULT papacc_configuration_personal_native_open(PAPACC_CONFIGURATION_COMMIT *o,const char *parent)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;unsigned i,k,len;char drive[3],device[512];
 WCHAR path[MAX_PATH+5];UNICODE_STRING name;OBJECT_ATTRIBUTES a;IO_STATUS_BLOCK io;HMODULE mod;FARPROC proc;BOOTSTRAP_NT_CREATE create;
 PACL dacl;BOOL present,defaulted;NTSTATUS status;ACCESS_MASK parent_access;OBJECT_ATTRIBUTES *attributes;IO_STATUS_BLOCK *result;
 if(!o||o->writer!=GetCurrentThreadId()||o->initialized||!parent||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(n->phase||p->active||p->thread||p->bootstrap_parent||p->bootstrap_token)return PAPACC_RESULT_INVALID_STATE;
 len=(unsigned)strlen(parent);if(len<3U||len>=MAX_PATH||parent[1]!=':'||parent[2]!='\\'||strstr(parent,".."))return PAPACC_RESULT_INVALID_ARGUMENT;
 for(i=0;i<len;i++)if((unsigned char)parent[i]<32U||(unsigned char)parent[i]>126U||parent[i]=='/'||(i!=1U&&parent[i]==':'))return PAPACC_RESULT_INVALID_ARGUMENT;
 n->diagnostic_first=n->diagnostic_error=n->diagnostic_kind=0;n->phase=1U;p->file=INVALID_HANDLE_VALUE;memcpy(p->path,parent,len+1U);
 if(!personal_token(p,1))return PAPACC_RESULT_INVALID_STATE;
 drive[0]=parent[0];drive[1]=':';drive[2]=0;
 if(!QueryDosDeviceA(drive,device,sizeof(device))){(void)personal_deny(p,30,GetLastError(),1);return PAPACC_RESULT_NOT_SUPPORTED;}
 if(strncmp(device,"\\Device\\",8U)){(void)personal_deny(p,31,0,2);return PAPACC_RESULT_NOT_SUPPORTED;}
 k=(unsigned)strlen(device);if(k+len-2U>=MAX_PATH+5U)return PAPACC_RESULT_INVALID_ARGUMENT;
 for(i=0;i<k;i++)path[i]=(WCHAR)(unsigned char)device[i];
 for(i=2U;i<=len;i++)path[k+i-2U]=(WCHAR)(unsigned char)parent[i];len=k+len-2U;
 mod=GetModuleHandleA("ntdll.dll");if(!mod)return PAPACC_RESULT_NOT_SUPPORTED;proc=GetProcAddress(mod,"NtCreateFile");if(!proc)return PAPACC_RESULT_NOT_SUPPORTED;memcpy(&create,&proc,sizeof(create));
 memset(&a,0,sizeof(a));memset(&io,0,sizeof(io));name.Buffer=path;name.Length=(USHORT)(len*2U);name.MaximumLength=name.Length;
 a.Length=sizeof(a);a.ObjectName=&name;a.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 attributes=&a;result=&io;
 if(n->first_owner==p){
  if(len>=sizeof(n->scq_cold_name)/sizeof(n->scq_cold_name[0]))return PAPACC_RESULT_INVALID_ARGUMENT;
  memcpy(n->scq_cold_name,path,(len+1U)*sizeof(WCHAR));n->scq_cold_unicode=name;n->scq_cold_unicode.Buffer=n->scq_cold_name;
  n->scq_cold_attributes=a;n->scq_cold_attributes.ObjectName=&n->scq_cold_unicode;memset(&n->scq_cold_io,0,sizeof(n->scq_cold_io));
  attributes=&n->scq_cold_attributes;result=&n->scq_cold_io;
 }
 parent_access=FILE_READ_ATTRIBUTES|FILE_TRAVERSE|READ_CONTROL|SYNCHRONIZE;
 /* U2 UPDATE only traverses the volume anchor. Its held installation parent
  * acquires FILE_ADD_FILE separately; no volume-root creation right is needed.
  * All pre-existing profiles retain their original acquisition contract. */
 if(!(n->staged_update.self==&n->staged_update&&n->staged_update.phase==1U&&n->staged_update.invocation&&
  n->staged_update.thread==o->writer&&n->install_paths.mode==PAPACC_PERSONAL_PATHS_BINARY_ONLY&&
  n->install_paths.phase==PAPACC_PERSONAL_PATHS_READY&&n->install_paths.thread==o->writer )&&!(n->first_owner==p&&n->first_mode==3U&&n->first_key&&n->first_key->first_preparation==2U))parent_access|=FILE_ADD_SUBDIRECTORY;
 status=create(&p->bootstrap_parent,parent_access,attributes,result,NULL,
 FILE_ATTRIBUTE_DIRECTORY,FILE_SHARE_READ,1U,0x00200021U,NULL,0U);
 if(n->first_owner==p&&(status||result->Status||result->Information!=1U||!p->bootstrap_parent)){
  n->scq_cold_status=status;if(status>0||result->Status==(NTSTATUS)0x103L||p->bootstrap_parent)n->phase=2U;
  (void)personal_deny(p,32,(DWORD)status,3);return PAPACC_RESULT_INVALID_STATE;
 }
 if(status<0){(void)personal_deny(p,32,(DWORD)status,3);return PAPACC_RESULT_INVALID_STATE;}
 if(!GetFileInformationByHandle(p->bootstrap_parent,&n->parent)){(void)personal_deny(p,33,GetLastError(),1);return PAPACC_RESULT_INVALID_STATE;}
 if(!(n->parent.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(n->parent.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)){(void)personal_deny(p,34,0,2);return PAPACC_RESULT_INVALID_STATE;}
 n->error=GetSecurityInfo(p->bootstrap_parent,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->descriptor);
 if(n->error!=ERROR_SUCCESS){(void)personal_deny(p,35,n->error,1);return PAPACC_RESULT_INVALID_STATE;}
 if(!IsValidSecurityDescriptor(n->descriptor)){(void)personal_deny(p,36,0,2);return PAPACC_RESULT_INVALID_STATE;}
 if(!GetSecurityDescriptorDacl(n->descriptor,&present,&dacl,&defaulted)){(void)personal_deny(p,37,GetLastError(),1);return PAPACC_RESULT_INVALID_STATE;}
 if(!present||!dacl){(void)personal_deny(p,38,0,2);return PAPACC_RESULT_INVALID_STATE;}
 n->descriptor_bytes=GetSecurityDescriptorLength(n->descriptor);
 if(n->first_owner==p&&(!n->descriptor_bytes||n->descriptor_bytes>PAPACC_SCQ_FILE_SD_BYTES)){(void)personal_deny(p,36,ERROR_NOT_ENOUGH_MEMORY,2);return PAPACC_RESULT_INVALID_STATE;}
 if(!ConvertSidToStringSidA(n->sid,&n->sid_text)){(void)personal_deny(p,39,GetLastError(),1);return PAPACC_RESULT_INVALID_STATE;}
 if(n->first_owner==p&&strlen(n->sid_text)>511U)return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_personal_native_current(o);
}
