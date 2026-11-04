static int bootstrap_identity(HANDLE h,DWORD *volume,DWORD *high,DWORD *low,int first)
{
 BY_HANDLE_FILE_INFORMATION f;
 if(!GetFileInformationByHandle(h,&f)||!(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||
    (f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return 0;
 if(first){*volume=f.dwVolumeSerialNumber;*high=f.nFileIndexHigh;*low=f.nFileIndexLow;return 1;}
 return *volume==f.dwVolumeSerialNumber&&*high==f.nFileIndexHigh&&*low==f.nFileIndexLow;
}
/* Shared exclusive child primitive: caller retains the SAME parent handle.
 * No parent reopen, authority capture, key operation, commit or publication. */
static int bootstrap_create_child(PAPACC_CONFIG_PENDING *p)
{
 HMODULE module;FARPROC address;BOOTSTRAP_NT_CREATE create;
 WCHAR leaf[65];UNICODE_STRING name;OBJECT_ATTRIBUTES attr;IO_STATUS_BLOCK io;
 NTSTATUS status;unsigned i,n;OBJECT_ATTRIBUTES *attributes;IO_STATUS_BLOCK *result;
 if(!p->bootstrap_parent||p->bootstrap_container||p->bootstrap_created||
    InterlockedCompareExchange(&p->stop,0,0))return 0;
 if(!bootstrap_identity(p->bootstrap_parent,&p->bootstrap_parent_volume,&p->bootstrap_parent_high,&p->bootstrap_parent_low,0)){
  p->result=PAPACC_CAS_CONFLICT;return 0;}
 for(n=0;n<65U&&p->previous[n];n++);
 if(!n||n>64U||p->previous[n-1U]=='.')return 0;
 for(i=0;i<n;i++){
  unsigned char c=p->previous[i];
  if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='-'||c=='_'||c=='.'))return 0;
  leaf[i]=(WCHAR)c;
 }
 leaf[n]=0;module=GetModuleHandleA("ntdll.dll");if(!module)return 0;
 address=GetProcAddress(module,"NtCreateFile");if(!address)return 0;memcpy(&create,&address,sizeof(create));
 memset(&attr,0,sizeof(attr));memset(&io,0,sizeof(io));name.Buffer=leaf;name.Length=(USHORT)(n*2U);name.MaximumLength=name.Length;
 attr.Length=sizeof(attr);attr.RootDirectory=p->bootstrap_parent;attr.ObjectName=&name;attr.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 if(p->personal_native.first_owner==p){
  if(p->personal_native.first_phase!=3U||!p->personal_native.first_child_sd)return 0;
  attr.SecurityDescriptor=p->personal_native.first_child_sd;
 }
 attributes=&attr;result=&io;
 if(p->personal_native.first_owner==p){PAPACC_PERSONAL_NATIVE_CONTEXT *native=&p->personal_native;
  memcpy(native->scq_cold_name,leaf,(n+1U)*sizeof(WCHAR));native->scq_cold_unicode=name;native->scq_cold_unicode.Buffer=native->scq_cold_name;
  native->scq_cold_attributes=attr;native->scq_cold_attributes.ObjectName=&native->scq_cold_unicode;memset(&native->scq_cold_io,0,sizeof(native->scq_cold_io));
  attributes=&native->scq_cold_attributes;result=&native->scq_cold_io;
 }
 if(InterlockedCompareExchange(&p->stop,0,0))return 0;
 if(p->personal_native.first_owner==p){
  PAPACC_PERSONAL_NATIVE_CONTEXT *native=&p->personal_native;PAPACC_CA_KEY *key=native->first_key;
  PAPACC_SCM_RESOURCE_SAMPLE *sample=&native->first_worker_resources.last;LARGE_INTEGER clock;
  if(!key||!key->first_preparation||key->diagnostic_only||key->diagnostic_stop!=&p->stop||key->diagnostic_budget.active!=3U||
   key->diagnostic_budget.first||native->first_error||native->first_worker_thread!=GetCurrentThreadId()||
   !native->first_worker_resources.samples||native->first_worker_resources.first_error){if(!native->first_error)native->first_error=ERROR_INVALID_STATE;return 0;}
  if(sample->handles>=188U){native->first_error=ERROR_NOT_ENOUGH_QUOTA;return 0;}
  if(!key->diagnostic_budget.until||GetTickCount64()>=key->diagnostic_budget.until||!sample->frequency||
   !QueryPerformanceCounter(&clock)||clock.QuadPart<0||(ULONGLONG)clock.QuadPart<sample->end||
   (ULONGLONG)clock.QuadPart<sample->begin||(ULONGLONG)clock.QuadPart-sample->begin>sample->frequency/10U){native->first_error=ERROR_TIMEOUT;return 0;}
 }
 status=create(&p->bootstrap_container,FILE_LIST_DIRECTORY|FILE_TRAVERSE|FILE_ADD_FILE|FILE_READ_ATTRIBUTES|SYNCHRONIZE|
  (p->personal_native.first_owner==p?READ_CONTROL:0U),
  attributes,result,NULL,FILE_ATTRIBUTE_DIRECTORY,FILE_SHARE_READ,2U,0x00200021U,NULL,0U);
 if(p->personal_native.first_owner==p&&(status||result->Status||result->Information!=2U||!p->bootstrap_container)){
  p->native_error=(DWORD)status;p->personal_native.scq_cold_status=status;
  p->result=status==(NTSTATUS)0xc0000035L&&!p->bootstrap_container?PAPACC_CAS_CONFLICT:PAPACC_CAS_UNKNOWN;
  if(status>0||result->Status==(NTSTATUS)0x103L||p->bootstrap_container)p->personal_native.phase=2U;
  return 0;
 }
 if(status<0){
  p->native_error=(DWORD)status;
  /* Name collision is definite nonreplacement. Other failures are not
   * promoted to proven absence/rollback; preserve any returned reference. */
  p->result=(status==(NTSTATUS)0xc0000035L&&!p->bootstrap_container)?PAPACC_CAS_CONFLICT:PAPACC_CAS_UNKNOWN;
  return 0;
 }
 p->bootstrap_created=1U;
 if(result->Information!=2U||!bootstrap_identity(p->bootstrap_container,&p->bootstrap_volume,&p->bootstrap_high,&p->bootstrap_low,1)){
  p->result=PAPACC_CAS_UNKNOWN;return 0;}
 return 1;
}
