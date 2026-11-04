/* FCR0-FCR7, same original packet/key owner. No create, signer, worker or
 * generic authority. A current public digest is never a historical pin. */
static DWORD personal_prepared_guard(PAPACC_CONFIG_PENDING *p,int acquiring)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 if(n->first_owner!=p||n->first_mode!=3U||!n->first_key||n->first_key->first_preparation!=2U||
  n->first_phase!=1U||n->phase!=1U||p->active||p->thread||p->bootstrap_created||p->bootstrap_container||
  !personal_first_exclusive(p))return ERROR_INVALID_STATE;
 return personal_first_resources(p,0,acquiring);
}
static PAPACC_RESULT personal_prepared_reject(PAPACC_CONFIG_PENDING *p,DWORD stage,DWORD error)
{if(!p->personal_native.prepared_stage)p->personal_native.prepared_stage=stage;(void)personal_first_fail(p,error);return PAPACC_RESULT_INVALID_STATE;}
static int personal_prepared_string(const BYTE *buffer,size_t capacity,const WCHAR *s,const WCHAR *expected)
{
 ULONG_PTR base=(ULONG_PTR)buffer,at=(ULONG_PTR)s;size_t i,limit;
 if(!s||at<base||at-base>=capacity||(at-base)%sizeof(WCHAR))return 0;
 limit=(capacity-(size_t)(at-base))/sizeof(WCHAR);
 for(i=0;i<limit;i++)if(!s[i])return wcslen(expected)==i&&!memcmp(s,expected,i*sizeof(WCHAR));
 return 0;
}
static DWORD personal_prepared_scm(PAPACC_CONFIG_PENDING *p,int acquire)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 union {ULONGLONG align;BYTE bytes[4096];} buffer;QUERY_SERVICE_CONFIGW *q=(QUERY_SERVICE_CONFIGW *)buffer.bytes;
 SERVICE_STATUS_PROCESS status;SERVICE_SID_INFO sid;DWORD bytes;WCHAR command[MAX_PATH+40U];
 if(personal_prepared_guard(p,1))return ERROR_INVALID_STATE;
 if(acquire){
  if(n->prepared_manager||n->prepared_service)return ERROR_INVALID_STATE;
  n->prepared_manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT);
  if(!n->prepared_manager)return GetLastError();
  if(personal_prepared_guard(p,1))return ERROR_NOT_ENOUGH_QUOTA;
  n->prepared_service=OpenServiceW(n->prepared_manager,L"PapinhoAccelerator",SERVICE_QUERY_CONFIG|SERVICE_QUERY_STATUS);
 }
 if(!n->prepared_manager||!n->prepared_service)return ERROR_INVALID_HANDLE;
 if(swprintf_s(command,MAX_PATH+40U,L"\"%s\\papacc_server.exe\" service host scq-closed",n->install_paths.binary)<0)return ERROR_INVALID_NAME;
 memset(&buffer,0,sizeof(buffer));bytes=0;
 if(personal_prepared_guard(p,1)||!QueryServiceConfigW(n->prepared_service,q,sizeof(buffer.bytes),&bytes))return ERROR_INVALID_DATA;
 if(q->dwServiceType!=SERVICE_WIN32_OWN_PROCESS||q->dwStartType!=SERVICE_DEMAND_START||q->dwErrorControl!=SERVICE_ERROR_NORMAL||
  !personal_prepared_string(buffer.bytes,sizeof(buffer.bytes),q->lpBinaryPathName,command)||
  !personal_prepared_string(buffer.bytes,sizeof(buffer.bytes),q->lpServiceStartName,L"NT SERVICE\\PapinhoAccelerator")||
  !personal_prepared_string(buffer.bytes,sizeof(buffer.bytes),q->lpLoadOrderGroup,L"")||
  !personal_prepared_string(buffer.bytes,sizeof(buffer.bytes),q->lpDependencies,L""))return ERROR_ACCESS_DENIED;
 memset(&sid,0,sizeof(sid));bytes=0;
 if(personal_prepared_guard(p,1)||!QueryServiceConfig2W(n->prepared_service,SERVICE_CONFIG_SERVICE_SID_INFO,(BYTE *)&sid,sizeof(sid),&bytes)||
  sid.dwServiceSidType!=SERVICE_SID_TYPE_UNRESTRICTED)return ERROR_ACCESS_DENIED;
 memset(&status,0,sizeof(status));bytes=0;
 if(personal_prepared_guard(p,1)||!QueryServiceStatusEx(n->prepared_service,SC_STATUS_PROCESS_INFO,(BYTE *)&status,sizeof(status),&bytes)||
  bytes!=sizeof(status)||status.dwServiceType!=SERVICE_WIN32_OWN_PROCESS||status.dwCurrentState!=SERVICE_STOPPED||status.dwProcessId)return ERROR_SERVICE_CANNOT_ACCEPT_CTRL;
 return personal_prepared_guard(p,0);
}
static DWORD personal_prepared_hash(PAPACC_CONFIG_PENDING *p,BYTE digest[32])
{
 PAPACC_PERSONAL_SCQ_INPUTS *c=&p->personal_native.scq_inputs;BYTE chunk[1024];DWORD remaining,n,e=ERROR_INVALID_DATA;
 NTSTATUS status;LARGE_INTEGER zero;zero.QuadPart=0;
 if(personal_prepared_guard(p,1)||c->content_hash.retained||c->content_hash.active||c->content_hash.hash||c->content_hash.algorithm||
  !c->file[2]||c->identity[2].nFileSizeHigh||!c->identity[2].nFileSizeLow||!SetFilePointerEx(c->file[2],zero,NULL,FILE_BEGIN))return ERROR_INVALID_STATE;
 remaining=c->identity[2].nFileSizeLow;c->content_hash.active=1;
 if(personal_prepared_guard(p,1))goto done;
 status=BCryptOpenAlgorithmProvider(&c->content_hash.algorithm,BCRYPT_SHA256_ALGORITHM,NULL,0);
 if(status||!c->content_hash.algorithm){c->content_hash.operation_status=status;if(c->content_hash.algorithm){c->content_hash.retained=1;c->phase=4U;}goto done;}
 if(personal_prepared_guard(p,1))goto done;
 status=BCryptCreateHash(c->content_hash.algorithm,&c->content_hash.hash,NULL,0,NULL,0,0);
 if(status||!c->content_hash.hash){c->content_hash.operation_status=status;if(c->content_hash.hash){c->content_hash.retained=1;c->phase=4U;}goto done;}
 while(remaining){
  if(personal_prepared_guard(p,0))goto done;n=0;
  if(!ReadFile(c->file[2],chunk,remaining>sizeof(chunk)?sizeof(chunk):remaining,&n,NULL)||!n||n>remaining||n>sizeof(chunk))goto done;
  if(personal_prepared_guard(p,0))goto done;
  status=BCryptHashData(c->content_hash.hash,chunk,n,0);if(status){c->content_hash.operation_status=status;goto done;}remaining-=n;
 }
 if(personal_prepared_guard(p,0))goto done;
 status=BCryptFinishHash(c->content_hash.hash,digest,32U,0);c->content_hash.operation_status=status;
 if(!status)e=0;
done:SecureZeroMemory(chunk,sizeof(chunk));
 if(c->content_hash.active&&!c->content_hash.retained&&personal_scq_contents_cleanup(c))e=ERROR_INVALID_HANDLE;
 return e;
}
static DWORD personal_prepared_image(PAPACC_CONFIG_PENDING *p,int acquire)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_PERSONAL_SCQ_INPUTS *c=&n->scq_inputs;PAPACC_SCQ_IMAGE_CHAIN *g=&c->image_chain;
 BOOTSTRAP_NT_CREATE open_file;FARPROC address;HMODULE module;NTSTATUS status;BY_HANDLE_FILE_INFORMATION f;DWORD e,flags;BYTE hash[32];int valid;
 WCHAR expected[MAX_PATH],actual[MAX_PATH];size_t z;
 if(personal_prepared_guard(p,1)||g->count<5U||g->phase!=2U||g->scratch)return ERROR_INVALID_STATE;
 if(swprintf_s(expected,MAX_PATH,L"%s\\papacc_server.exe",n->install_paths.binary)<0)return ERROR_INVALID_NAME;
 memset(actual,0,sizeof(actual));e=GetModuleFileNameW(NULL,actual,MAX_PATH);
 if(!e||e>=MAX_PATH||wcscmp(actual,expected))return ERROR_REVISION_MISMATCH;
 /* The source must be under THIS protected binary directory, not an unrelated
  * chain whose last directory happens to have the same spelling. */
 z=wcslen(n->install_paths.binary);
 if(z>=MAX_PATH||wcscmp(g->binary,n->install_paths.binary))return ERROR_REVISION_MISMATCH;
 if(acquire){
  if(c->phase||c->file[2])return ERROR_INVALID_STATE;c->phase=1U;c->thread=GetCurrentThreadId();
  module=GetModuleHandleW(L"ntdll.dll");address=module?GetProcAddress(module,"NtCreateFile"):NULL;
  if(!address)return ERROR_NOT_SUPPORTED;memcpy(&open_file,&address,sizeof(open_file));
  wcscpy_s(g->name,MAX_PATH+4U,L"papacc_server.exe");memset(&g->attributes,0,sizeof(g->attributes));memset(&g->io,0,sizeof(g->io));
  g->unicode.Buffer=g->name;g->unicode.Length=(USHORT)(wcslen(g->name)*sizeof(WCHAR));g->unicode.MaximumLength=g->unicode.Length;
  g->attributes.Length=sizeof(g->attributes);g->attributes.RootDirectory=g->directory[g->count-1U];g->attributes.ObjectName=&g->unicode;g->attributes.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
  if(personal_prepared_guard(p,1))return ERROR_NOT_ENOUGH_QUOTA;
  status=open_file(&c->file[2],PAPACC_SCQ_FILE_READ_RIGHTS,&g->attributes,&g->io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1U,0x00200060U,NULL,0);
  g->open_status=status;
  if(status||g->io.Status||g->io.Information!=1U||!c->file[2]){if(status>0||g->io.Status==(NTSTATUS)0x103L||c->file[2])c->phase=4U;return ERROR_INVALID_DATA;}
 }
 if(!c->file[2]||!GetHandleInformation(c->file[2],&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(c->file[2])!=FILE_TYPE_DISK||!GetFileInformationByHandle(c->file[2],&f)||
  (f.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||f.nNumberOfLinks!=1U||f.nFileSizeHigh||!f.nFileSizeLow)return ERROR_INVALID_DATA;
 if(!acquire&&!personal_scq_inputs_same(&c->identity[2],&f))return ERROR_REVISION_MISMATCH;
 e=scqi_native_file_sd(c->file[2],&g->scratch,&g->native_sd,&p->stop);
 if(e||!g->scratch){if(g->scratch)c->phase=4U;return e?e:ERROR_INVALID_SECURITY_DESCR;}
 valid=GetSecurityDescriptorLength(g->scratch)<=PAPACC_SCQ_FILE_SD_BYTES&&personal_scq_inputs_sd_policy(g->scratch,n->sid,1);
 if(acquire&&valid){c->descriptor[2]=g->scratch;c->descriptor_bytes[2]=GetSecurityDescriptorLength(g->scratch);g->scratch=NULL;c->identity[2]=f;}
 else if(!acquire)valid=valid&&c->descriptor[2]&&GetSecurityDescriptorLength(g->scratch)==c->descriptor_bytes[2]&&!memcmp(c->descriptor[2],g->scratch,c->descriptor_bytes[2]);
 if(g->scratch){if(LocalFree(g->scratch)){c->phase=4U;return ERROR_INVALID_HANDLE;}g->scratch=NULL;}
 if(!valid)return ERROR_ACCESS_DENIED;
 e=personal_prepared_hash(p,hash);if(e)return e;
 if(acquire)memcpy(c->digest[2],hash,32U);else if(memcmp(c->digest[2],hash,32U))return ERROR_REVISION_MISMATCH;
 return personal_prepared_guard(p,0);
}
static DWORD personal_prepared_current(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_SCQ_IMAGE_CHAIN *g=&n->scq_inputs.image_chain;DWORD i;
 BY_HANDLE_FILE_INFORMATION *f[3];const DWORD *selected[3]={n->prepared_parent,n->prepared_target,n->prepared_source};
 if(n->first_parent_index>=PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS||n->first_parent_index+1U>=g->count)return ERROR_INVALID_STATE;
 f[0]=&n->parent;f[1]=&g->identity[n->first_parent_index+1U];f[2]=&n->source;
 if(personal_prepared_guard(p,1)||n->first_parent_index+1U>=g->count||!personal_first_child_protected(g->sd[n->first_parent_index+1U],n->sid)||
  personal_packet_current(p)!=PAPACC_RESULT_OK||personal_packet_source(p)!=PAPACC_RESULT_OK||personal_first_chain_current(p,0))return ERROR_INVALID_STATE;
 for(i=0;i<3U;i++)if(f[i]->dwVolumeSerialNumber!=selected[i][0]||f[i]->nFileIndexHigh!=selected[i][1]||f[i]->nFileIndexLow!=selected[i][2])return ERROR_REVISION_MISMATCH;
 if(personal_prepared_image(p,0)||personal_prepared_scm(p,0))return ERROR_INVALID_STATE;
 return personal_prepared_guard(p,0);
}
static void personal_prepared_id(const BY_HANDLE_FILE_INFORMATION *f,BYTE id[12])
{DWORD value[3];unsigned i,j;value[0]=f->dwVolumeSerialNumber;value[1]=f->nFileIndexHigh;value[2]=f->nFileIndexLow;for(i=0;i<3U;i++)for(j=0;j<4U;j++)id[4U*i+j]=(BYTE)(value[i]>>(8U*j));}
static int personal_prepared_spec(PAPACC_CONFIG_PENDING *p,PAPACC_PERSONAL_PREPARED_SPEC *s,BYTE ids[4][12])
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_SCQ_IMAGE_CHAIN *g=&n->scq_inputs.image_chain;
 if(n->first_parent_index+1U>=g->count||!n->first_service_bytes||strcmp(n->plan_leaf,"Accelerator"))return 0;
 personal_prepared_id(&n->parent,ids[0]);personal_prepared_id(&g->identity[n->first_parent_index+1U],ids[1]);personal_prepared_id(&n->source,ids[2]);personal_prepared_id(&n->scq_inputs.identity[2],ids[3]);
 memset(s,0,sizeof(*s));s->selection.operator_id=n->sid;s->selection.operator_bytes=n->sid_bytes;s->selection.parent_id=ids[0];s->selection.leaf=(BYTE *)n->plan_leaf;s->selection.leaf_bytes=11U;
 s->selection.authority=n->first_authority;s->selection.generation=n->first_generation;s->selection.service_id=n->first_service;s->selection.service_bytes=n->first_service_bytes;
 s->selection.configuration=p->next;s->selection.configuration_bytes=43U;s->target_id=ids[1];s->source_id=ids[2];s->image_id=ids[3];s->image_digest=n->scq_inputs.digest[2];return 1;
}
PAPACC_RESULT papacc_configuration_personal_prepared_plan(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_PREPARED_SPEC s;BYTE ids[4][12];size_t bytes;DWORD e;
 if(!o||o->writer!=GetCurrentThreadId()||o->initialized||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;p=&o->pending_commit;n=&p->personal_native;
 if(personal_prepared_guard(p,1)||p->personal_data.phase!=PAPACC_PERSONAL_DATA_CAPTURED||p->personal_data.effectful||p->personal_data.consumed||p->personal_data.bytes!=43U||
  strcmp(n->plan_leaf,"Accelerator")||memcmp(p->previous,n->plan_leaf,sizeof(n->plan_leaf)))return PAPACC_RESULT_INVALID_STATE;
 e=personal_prepared_image(p,1);if(e)return personal_prepared_reject(p,31U,e);
 e=personal_prepared_scm(p,1);if(e)return personal_prepared_reject(p,32U,e);
 if(!personal_prepared_spec(p,&s,ids))return personal_prepared_reject(p,33U,ERROR_INVALID_DATA);
 bytes=papacc_personal_prepared_build(&s,p->previous+1024U,PAPACC_PERSONAL_PLAN_MAX);
 if(!bytes||!papacc_personal_prepared_upgrade(&p->personal_data,p->previous+1024U,bytes)||!personal_first_configuration(p)||personal_prepared_current(p))return personal_prepared_reject(p,34U,ERROR_INVALID_DATA);
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_prepared_validate(PAPACC_CONFIGURATION_COMMIT *o,BYTE public_digest[32])
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_PREPARED_SPEC s;BYTE ids[4][12],blob[283];DWORD bytes=0;SECURITY_STATUS status;
 if(!o||!public_digest||o->writer!=GetCurrentThreadId()||o->initialized||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;p=&o->pending_commit;n=&p->personal_native;
 if(personal_prepared_guard(p,1)||n->presenter_phase!=2U||n->presenter_ca!=3U||p->personal_data.effectful!=3U||p->personal_data.phase!=PAPACC_PERSONAL_DATA_CONFIRMED||p->personal_data.consumed||
  memcmp(n->presenter_attempt,p->personal_data.attempt,32U)||!personal_first_configuration(p)||!personal_prepared_spec(p,&s,ids)||
  !papacc_personal_prepared_matches(p->personal_data.plan,p->personal_data.bytes,&s)||personal_prepared_current(p)||
  papacc_ca_key_first_budget_arm(n->first_key,&p->stop)||!papacc_personal_effect_consume(&p->personal_data,n->presenter_attempt,p->previous+1024U,p->personal_data.bytes))return personal_prepared_reject(p,35U,ERROR_INVALID_STATE);
 status=papacc_ca_key_prepared_read_open(n->first_key,n->first_authority,n->first_generation,n->sid,n->first_service);
 if(!status)status=papacc_ca_key_prepared_public(n->first_key,blob,sizeof(blob),&bytes);
 if(status||bytes!=sizeof(blob)||!papacc_store_hash(&n->scq_inputs.content_hash,blob,bytes,public_digest)||personal_prepared_current(p)){
  if(n->scq_inputs.content_hash.retained)n->scq_inputs.phase=4U;return personal_prepared_reject(p,36U,status?(DWORD)status:ERROR_INVALID_DATA);
 }
 n->first_phase=6U;return PAPACC_RESULT_OK; /* evidence only; no borrow or publication */
}
