/* K1 source in the SAME native owner. No SCQ registration/confirmation is
 * imported. Request/decision originals remain locked until terminal close;
 * their hashes do not grant creation. The original administrator emits the
 * independent fixture-only act at this live console. */
#define SCQKF_SOURCE_TAG 0x4b310001U
static DWORD scqkf_source_fail(PAPACC_CONFIGURATION_COMMIT *o,DWORD stage,DWORD e)
{
 PAPACC_MACHINE_FIXTURE_CONTROL *f=&o->pending_commit.personal_native.machine_fixture;
 if(o->unresolved||o->pending_commit.personal_native.scq_inputs.image_chain.phase==4U||f->source_status>0||f->source_io.Status==(NTSTATUS)0x103L)f->source_unknown=1U;
 if(!f->source_first){f->source_first=e?e:ERROR_INVALID_DATA;f->source_stage=stage;}f->source_phase=3U;return f->source_first;
}
static DWORD scqkf_file_digest(PAPACC_MACHINE_FIXTURE_CONTROL *f,HANDLE h,BYTE out[32],volatile LONG *stop)
{
 PACC_STORAGE_SHA256 c;BYTE block[1024],digest[32];LARGE_INTEGER zero;DWORD n;int e;
 memset(&c,0,sizeof(c));zero.QuadPart=0;
 if(*stop||GetTickCount64()>=f->source_until)return ERROR_CANCELLED;
 if(!SetFilePointerEx(h,zero,NULL,FILE_BEGIN)){DWORD native_error=GetLastError();return native_error?native_error:ERROR_GEN_FAILURE;}
 if(pacc_storage_sha_init(&c)!=PACC_HASH_OK)return ERROR_INVALID_DATA;
 for(;;){
  if(*stop||GetTickCount64()>=f->source_until)return ERROR_CANCELLED;
  if(!ReadFile(h,block,sizeof(block),&n,NULL)){DWORD native_error=GetLastError();return native_error?native_error:ERROR_GEN_FAILURE;}if(!n)break;
  e=pacc_storage_sha_update(&c,block,n,NULL,NULL);if(e!=PACC_HASH_OK)return ERROR_INVALID_DATA;
 }
 if(*stop||pacc_storage_sha_final(&c,digest,sizeof(digest),NULL,NULL)!=PACC_HASH_OK||*stop||GetTickCount64()>=f->source_until)return ERROR_CANCELLED;
 memcpy(out,digest,32);return 0; /* existing pure DATA hash capsule, no provider */
}
static DWORD scqkf_source_open(PAPACC_CONFIGURATION_COMMIT *o,unsigned index,HANDLE parent,const WCHAR *name)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;PAPACC_MACHINE_FIXTURE_CONTROL *f=&n->machine_fixture;
 PAPACC_MACHINE_FIXTURE_REFERENCE *r=&f->references[index];SCQI_NT_OPEN open_file;HMODULE module;FARPROC address;DWORD count,e;
 if(index>=5U||index==2U||r->handle||!name||wcsnlen(name,MAX_PATH+4U)>=MAX_PATH+4U)return ERROR_INVALID_STATE;
 if(o->pending_commit.stop||GetTickCount64()>=f->source_until)return ERROR_CANCELLED;
 if(!GetProcessHandleCount(GetCurrentProcess(),&count)||count>=188U)return ERROR_NOT_ENOUGH_QUOTA;
 module=GetModuleHandleW(L"ntdll.dll");address=module?GetProcAddress(module,"NtCreateFile"):NULL;
 if(!address)return ERROR_NOT_SUPPORTED;memcpy(&open_file,&address,sizeof(open_file));
 wcscpy_s(f->native_name,MAX_PATH+4U,name);memset(&f->source_attributes,0,sizeof(f->source_attributes));memset(&f->source_io,0,sizeof(f->source_io));
 f->source_unicode.Buffer=f->native_name;f->source_unicode.Length=(USHORT)(wcslen(f->native_name)*sizeof(WCHAR));f->source_unicode.MaximumLength=f->source_unicode.Length;
 f->source_attributes.Length=sizeof(f->source_attributes);f->source_attributes.RootDirectory=parent;f->source_attributes.ObjectName=&f->source_unicode;
 f->source_attributes.Attributes=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE;
 f->source_open_calls++;f->source_status=open_file(&r->handle,PAPACC_SCQ_FILE_READ_RIGHTS,&f->source_attributes,&f->source_io,NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1U,0x00200060U,NULL,0);
 if(f->source_status||f->source_io.Status||f->source_io.Information!=1U||!r->handle){
  if(f->source_status>0||f->source_io.Status==(NTSTATUS)0x103L||r->handle)o->unresolved=1;
  return ERROR_INVALID_DATA; /* raw status/IOSB remain in original owner */
 }
 if(!GetFileInformationByHandle(r->handle,&r->identity)||r->identity.nFileSizeHigh||!r->identity.nFileSizeLow||r->identity.nFileSizeLow>16777216U||
  (r->identity.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||r->identity.nNumberOfLinks!=1U)return ERROR_INVALID_DATA;
 e=scqi_native_file_sd(r->handle,&r->sd,&n->scq_inputs.native_sd,&o->pending_commit.stop);
 if(e||!r->sd){if(e&&r->sd)o->unresolved=1;return e?e:ERROR_INVALID_SECURITY_DESCR;}r->sd_bytes=GetSecurityDescriptorLength(r->sd);
 if(!r->sd_bytes||r->sd_bytes>65536U||!personal_scq_inputs_sd_policy(r->sd,n->sid,index>=2U))return ERROR_ACCESS_DENIED;
 return 0;
}
static DWORD scqkf_chain_digest(PAPACC_MACHINE_FIXTURE_CONTROL *f,PAPACC_SCQ_IMAGE_CHAIN *g,BYTE digest[32])
{
 BYTE *at=g->target_bytes,sd[32];unsigned i;
 if(g->count>PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS)return ERROR_INVALID_DATA;
 scqp_put(&at,SCQKF_SOURCE_TAG);scqp_put(&at,g->count);
 for(i=0;i<=g->count;i++){
  if(!g->sd[i]||!g->length[i]||g->length[i]>65536U||!store_data_hash(&f->hash,(BYTE*)g->sd[i],g->length[i],sd))return ERROR_INVALID_DATA;
  scqp_put(&at,g->identity[i].dwVolumeSerialNumber);scqp_put(&at,g->identity[i].nFileIndexHigh);scqp_put(&at,g->identity[i].nFileIndexLow);memcpy(at,sd,32);at+=32;
 }
 return store_data_hash(&g->target_hash,g->target_bytes,(unsigned)(at-g->target_bytes),digest)?0:ERROR_INVALID_DATA;
}
static DWORD scqkf_plan_build(PAPACC_CONFIGURATION_COMMIT *o,BYTE buffer[1024],DWORD *bytes)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;PAPACC_MACHINE_FIXTURE_CONTROL *f=&n->machine_fixture;BYTE *out;unsigned i;
 if(!n->sid_bytes||n->sid_bytes>68U||!f->runtime_sid_bytes||f->runtime_sid_bytes>68U)return ERROR_INVALID_SID;
 /* Immutable original-source preimage, not an authorization record. */
 out=buffer;memcpy(out,"KFX1",4);out+=4;
 {static const BYTE accepted[32]={0x84,0x89,0xc0,0x09,0xdf,0xa6,0x2d,0xf0,0x83,0x54,0x15,0xd4,0x54,0x4c,0x06,0x32,0xab,0xd8,0x50,0x92,0x4a,0x35,0x21,0x7b,0x2e,0xc4,0x51,0x71,0x81,0x47,0xad,0x38};memcpy(out,accepted,32);out+=32;}
 {static const BYTE envelope[32]={0x61,0x1c,0x1a,0xd8,0xa7,0x8d,0xe7,0xcf,0x7c,0x07,0xaa,0x87,0x73,0x90,0x68,0x75,0xc6,0x4a,0xeb,0xaf,0x25,0xb7,0x2c,0x6e,0xd2,0xd8,0x0e,0x75,0x4e,0xf5,0xfe,0xda};memcpy(out,envelope,32);out+=32;} /* accepted F0-F5; not operational authority */
 {static const BYTE growth[32]={0xfa,0x44,0x3a,0x1c,0xc7,0x00,0x11,0xcc,0xd6,0x0e,0xd4,0x0a,0x60,0x2d,0xf4,0x5c,0x13,0xac,0xb5,0x9d,0x4a,0xe1,0xc0,0xfe,0x48,0xcd,0x26,0x67,0x43,0x6f,0x80,0x00};memcpy(out,growth,32);out+=32;} /* accepted R0-R4, not operational authority */
 {static const BYTE readback[32]={0x3b,0x09,0x09,0x74,0xb4,0xab,0xbf,0x5d,0xb2,0x54,0x3e,0x8e,0xf2,0x23,0xce,0xbf,0x78,0xec,0x8a,0x4b,0x08,0x58,0x1d,0x9f,0xa3,0xd7,0xe7,0xbe,0xb9,0xb5,0x3d,0x47};memcpy(out,readback,32);out+=32;} /* accepted S0-S5, not authority */
 memcpy(out,f->attempt,32);out+=32;
 memcpy(out,f->authority,16);out+=16;memcpy(out,f->generation,16);out+=16;
 scqp_put(&out,n->sid_bytes);memcpy(out,n->sid,n->sid_bytes);out+=n->sid_bytes;scqp_put(&out,f->runtime_sid_bytes);memcpy(out,f->runtime_sid,f->runtime_sid_bytes);out+=f->runtime_sid_bytes;
 for(i=0;i<5U;i++){BYTE sd[32];scqp_put(&out,f->references[i].identity.dwVolumeSerialNumber);scqp_put(&out,f->references[i].identity.nFileIndexHigh);scqp_put(&out,f->references[i].identity.nFileIndexLow);memcpy(out,f->references[i].digest,32);out+=32;
  if(!store_data_hash(&f->hash,(BYTE*)f->references[i].sd,f->references[i].sd_bytes,sd))return scqkf_source_fail(o,8U,ERROR_INVALID_DATA);memcpy(out,sd,32);out+=32;
 }
 memcpy(out,f->chain_id,32);out+=32;
 scqp_put(&out,(DWORD)f->source_until);scqp_put(&out,(DWORD)(f->source_until>>32));
 *bytes=(DWORD)(out-buffer);return 0;
}
static DWORD scqkf_source_current(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;PAPACC_MACHINE_FIXTURE_CONTROL *f=&n->machine_fixture;
 PAPACC_SCQ_IMAGE_CHAIN *g=&n->scq_inputs.image_chain;BY_HANDLE_FILE_INFORMATION info;DWORD i,e,z,bytes,flags;BYTE digest[32];
 WCHAR process[MAX_PATH+4U],original[MAX_PATH+4U],key_name[96];HANDLE h;BYTE rebuilt[1024];DWORD rebuilt_bytes;
 if(o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||f->source_owner!=o||
  f->source_thread!=GetCurrentThreadId()||(f->source_phase!=1U&&f->source_phase!=2U)||f->source_first||
  !f->runtime_sid_bytes||f->runtime_sid_bytes>68U||!IsValidSid((PSID)f->runtime_sid)||GetLengthSid((PSID)f->runtime_sid)!=f->runtime_sid_bytes||
  !f->source_until||f->source_until!=f->key.diagnostic_budget.until||f->source_until!=(((ULONGLONG)g->plan.valid_until[1]<<32)|g->plan.valid_until[0])||
  !f->plan_bytes_count||f->plan_bytes_count>sizeof(f->plan_bytes)||GetTickCount64()>=f->source_until||!personal_token(&o->pending_commit,0)||
  g->self!=g||g->owner!=o||g->thread!=GetCurrentThreadId()||g->phase!=2U||g->first||g->plan.purpose!=SCQKF_SOURCE_TAG||
  g->count<2U||g->count>PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS||!g->installer)return ERROR_ACCESS_DENIED;
 memset(key_name,0,sizeof(key_name));papacc_ca_key_name_data(key_name,f->authority,f->generation);
 if(memcmp(key_name,f->key_name,sizeof(key_name)))return ERROR_INVALID_DATA;
 for(i=0;i<g->count;i++){
  h=g->directory[i];if(!h||!GetHandleInformation(h,&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(h)!=FILE_TYPE_DISK||
   !GetFileInformationByHandle(h,&info)||!scqi_directory_same(&g->identity[i],&info))return ERROR_INVALID_DATA;
  if(f->source_scratch)return ERROR_INVALID_STATE;
  e=scqi_native_file_sd(h,&f->source_scratch,&n->scq_inputs.native_sd,&o->pending_commit.stop);if(e||!f->source_scratch){if(e&&f->source_scratch)o->unresolved=1;return e?e:ERROR_INVALID_SECURITY_DESCR;}
  bytes=GetSecurityDescriptorLength(f->source_scratch);
  e=bytes!=g->length[i]||!bytes||!g->sd[i]||memcmp(g->sd[i],f->source_scratch,bytes)||!scqi_directory_sd(f->source_scratch,n->sid,g->installer,i==0U)?ERROR_ACCESS_DENIED:0;
  f->source_close|=1U<<16;if(LocalFree(f->source_scratch)){o->unresolved=1;return ERROR_INVALID_HANDLE;}f->source_scratch=NULL;f->source_close&=~(1U<<16);if(e)return e;
 }
 for(i=0;i<5U;i++){
  PAPACC_MACHINE_FIXTURE_REFERENCE *r=&f->references[i];
  if(!r->handle||!GetHandleInformation(r->handle,&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(r->handle)!=FILE_TYPE_DISK||
   !GetFileInformationByHandle(r->handle,&info)||!personal_scq_inputs_same(&r->identity,&info))return ERROR_INVALID_DATA;
  e=scqi_native_file_sd(r->handle,&f->source_scratch,&n->scq_inputs.native_sd,&o->pending_commit.stop);if(e||!f->source_scratch){if(e&&f->source_scratch)o->unresolved=1;return e?e:ERROR_INVALID_SECURITY_DESCR;}
  bytes=GetSecurityDescriptorLength(f->source_scratch);
  e=bytes!=r->sd_bytes||!bytes||!r->sd||memcmp(r->sd,f->source_scratch,bytes)||!personal_scq_inputs_sd_policy(f->source_scratch,n->sid,i>=2U)?ERROR_ACCESS_DENIED:0;
  f->source_close|=1U<<16;if(LocalFree(f->source_scratch)){o->unresolved=1;return ERROR_INVALID_HANDLE;}f->source_scratch=NULL;f->source_close&=~(1U<<16);if(e)return e;
  e=scqkf_file_digest(f,r->handle,digest,&o->pending_commit.stop);if(e||memcmp(digest,r->digest,32))return e?e:ERROR_CRC;
 }
 z=MAX_PATH+4U;if(!QueryFullProcessImageNameW(GetCurrentProcess(),PROCESS_NAME_NATIVE,process,&z)||!z||z>=MAX_PATH+4U)return ERROR_ACCESS_DENIED;
 bytes=GetFinalPathNameByHandleW(g->image,original,MAX_PATH+4U,FILE_NAME_NORMALIZED|VOLUME_NAME_NT);
 if(!bytes||bytes!=z||bytes>=MAX_PATH+4U||CompareStringOrdinal(process,(int)z,original,(int)bytes,FALSE)!=CSTR_EQUAL)return ERROR_ACCESS_DENIED;
 for(i=0;i<2U;i++){
  HMODULE mod=GetModuleHandleW(i?L"libcrypto-3-x64.dll":L"libssl-3-x64.dll");
  if(!mod)return ERROR_MOD_NOT_FOUND;z=GetModuleFileNameW(mod,process,MAX_PATH+4U);
  bytes=GetFinalPathNameByHandleW(f->references[3U+i].handle,original,MAX_PATH+4U,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
  if(!z||z>=MAX_PATH||bytes!=z+4U||wcsncmp(original,L"\\\\?\\",4U)||CompareStringOrdinal(process,(int)z,original+4,(int)z,FALSE)!=CSTR_EQUAL)return ERROR_ACCESS_DENIED;
 }
 e=scqkf_chain_digest(f,g,digest);if(e||memcmp(digest,f->chain_id,32))return e?e:ERROR_CRC;
 e=scqkf_plan_build(o,rebuilt,&rebuilt_bytes);if(e||rebuilt_bytes!=f->plan_bytes_count||memcmp(rebuilt,f->plan_bytes,rebuilt_bytes))return e?e:ERROR_CRC;
 if(!store_data_hash(&f->hash,rebuilt,rebuilt_bytes,digest)||memcmp(digest,f->plan_id,32))return ERROR_CRC;
 if(o->pending_commit.stop||GetTickCount64()>=f->source_until)return ERROR_CANCELLED;
 return 0; /* association since acquisition, not mapped-page attestation */
}
DWORD papacc_configuration_personal_machine_fixture_source(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_MACHINE_FIXTURE_INPUT *input)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_MACHINE_FIXTURE_CONTROL *f;PAPACC_SCQ_IMAGE_CHAIN *g;PAPACC_SCQ_PEER_DATA runtime;
 WCHAR directory[MAX_PATH],part[MAX_PATH],root[4],native[MAX_PATH+4U],domain[128];DWORD e,i,j,count;size_t len,end,start;
 if(!o||!input||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.stop)return ERROR_ACCESS_DENIED;
 if(input->known_managed<sizeof(*o)+PAPACC_MACHINE_FIXTURE_MANAGED_EXTRA||input->known_managed>PAPACC_ED11_PROCESS_LIMIT)return ERROR_NOT_ENOUGH_MEMORY;
 n=&o->pending_commit.personal_native;f=&n->machine_fixture;g=&n->scq_inputs.image_chain;
 if(n->phase||f->source_phase||f->source_owner||f->phase||g->phase||n->scq_profile_data.phase||n->scq_admin.phase||n->scq_session.phase)return ERROR_INVALID_STATE;
 n->phase=1U;f->source_owner=o;f->source_thread=GetCurrentThreadId();f->source_phase=1U;
 e=papacc_ca_key_fixture_budget_begin(&f->key);if(e)return scqkf_source_fail(o,1U,e);f->source_until=f->key.diagnostic_budget.until;
 if(papacc_server_console_win32_stop_requested()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK||!personal_token(&o->pending_commit,1))return scqkf_source_fail(o,2U,ERROR_ACCESS_DENIED);
 count=GetModuleFileNameW(NULL,f->executable,MAX_PATH);if(count<4U||count>=MAX_PATH||f->executable[1]!=L':'||f->executable[2]!=L'\\')return scqkf_source_fail(o,3U,ERROR_INVALID_NAME);
 e=scq_p1_native_name(f->executable,0,native);if(e)return scqkf_source_fail(o,3U,e);
 wcscpy_s(directory,MAX_PATH,f->executable);len=wcslen(directory);end=len;while(end>3U&&directory[end-1U]!=L'\\')end--;
 if(end<=3U||wcscmp(directory+end,L"papacc_server.exe"))return scqkf_source_fail(o,3U,ERROR_INVALID_NAME);directory[end-1U]=0;
 g->self=g;g->owner=o;g->thread=GetCurrentThreadId();g->phase=1U;g->stop=&o->pending_commit.stop;g->paths=&n->install_paths;
 g->plan.purpose=SCQKF_SOURCE_TAG;g->plan.valid_until[0]=(DWORD)f->source_until;g->plan.valid_until[1]=(DWORD)(f->source_until>>32);
 root[0]=f->executable[0];root[1]=L':';root[2]=L'\\';root[3]=0;
 if(!ConvertStringSidToSidW(L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464",&g->installer))return scqkf_source_fail(o,4U,ERROR_INVALID_SID);
 e=scq_p1_native_name(root,1,native);if(e)return scqkf_source_fail(o,4U,e);
 f->source_chain_calls++;e=scqi_chain_open(g,0U,NULL,native,1);if(e)return scqkf_source_fail(o,4U,e);g->count=1U;
 start=3U;len=wcslen(directory);
 for(j=(DWORD)start;j<=len;j++)if(!directory[j]||directory[j]==L'\\'){
  if(j==start||g->count==PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS)return scqkf_source_fail(o,4U,ERROR_INVALID_NAME);
  memcpy(part,directory+start,(j-start)*sizeof(WCHAR));part[j-start]=0;
  f->source_chain_calls++;e=scqi_chain_open(g,g->count,g->directory[g->count-1U],part,1);if(e)return scqkf_source_fail(o,4U,e);g->count++;start=j+1U;
 }
 f->source_chain_calls++;e=scqi_chain_open(g,g->count,g->directory[g->count-1U],L"papacc_server.exe",0);if(e)return scqkf_source_fail(o,4U,e);g->phase=2U;
 f->references[2].handle=g->image;f->references[2].identity=g->identity[g->count];f->references[2].sd=g->sd[g->count];f->references[2].sd_bytes=g->length[g->count];
 for(i=0;i<5U;i++){
  if(!scqp_nonzero(input->pins[i],32))return scqkf_source_fail(o,5U,ERROR_INVALID_DATA);
  if(i<2U){const WCHAR *path=i?input->decision_path:input->request_path;size_t parent_bytes=wcslen(directory);
   if(!path||wcsnlen(path,MAX_PATH)>=MAX_PATH||wcslen(path)<=parent_bytes+1U||
    CompareStringOrdinal(path,(int)parent_bytes,directory,(int)parent_bytes,TRUE)!=CSTR_EQUAL||path[parent_bytes]!=L'\\'||
    wcschr(path+parent_bytes+1U,L'\\')||wcschr(path+parent_bytes+1U,L':')||wcschr(path+parent_bytes+1U,L'/')||wcsstr(path+parent_bytes+1U,L".."))return scqkf_source_fail(o,5U,ERROR_ACCESS_DENIED);
   e=scqkf_source_open(o,i,g->directory[g->count-1U],path+parent_bytes+1U);
  }
  else if(i>2U)e=scqkf_source_open(o,i,g->directory[g->count-1U],i==3U?L"libssl-3-x64.dll":L"libcrypto-3-x64.dll");else e=0;
  if(e)return scqkf_source_fail(o,5U,e);
  for(j=0;j<i;j++)if(f->references[i].identity.dwVolumeSerialNumber==f->references[j].identity.dwVolumeSerialNumber&&
   f->references[i].identity.nFileIndexHigh==f->references[j].identity.nFileIndexHigh&&f->references[i].identity.nFileIndexLow==f->references[j].identity.nFileIndexLow)return scqkf_source_fail(o,5U,ERROR_ACCESS_DENIED);
  e=scqkf_file_digest(f,f->references[i].handle,f->references[i].digest,&o->pending_commit.stop);
  if(e||memcmp(f->references[i].digest,input->pins[i],32))return scqkf_source_fail(o,6U,e?e:ERROR_CRC);
 }
 memset(&runtime,0,sizeof(runtime));e=scqc_runtime_sid(&runtime,domain);if(e)return scqkf_source_fail(o,7U,e);
 f->runtime_sid_bytes=runtime.sid_bytes;memcpy(f->runtime_sid,runtime.sid,runtime.sid_bytes);
 for(i=0;i<3U;i++){
  if(o->pending_commit.stop||GetTickCount64()>=f->source_until)return scqkf_source_fail(o,8U,ERROR_CANCELLED);
  e=papacc_ca_key_fixture_budget_check(&f->key,&o->pending_commit.stop);if(e)return scqkf_source_fail(o,8U,e);
  f->source_rng_calls++;f->source_rng_status=BCryptGenRandom(NULL,i==0U?f->attempt:i==1U?f->authority:f->generation,i==0U?32U:16U,BCRYPT_USE_SYSTEM_PREFERRED_RNG);
  if(f->source_rng_status)return scqkf_source_fail(o,8U,ERROR_GEN_FAILURE);
 }
 if(!scqp_nonzero(f->attempt,32)||!scqp_nonzero(f->authority,16)||!scqp_nonzero(f->generation,16))return scqkf_source_fail(o,8U,ERROR_INVALID_DATA);
 papacc_ca_key_name_data(f->key_name,f->authority,f->generation);
 e=scqkf_chain_digest(f,g,f->chain_id);if(e)return scqkf_source_fail(o,8U,e);
 e=scqkf_plan_build(o,f->plan_bytes,&f->plan_bytes_count);if(e)return e;
 if(!store_data_hash(&f->hash,f->plan_bytes,f->plan_bytes_count,f->plan_id))return scqkf_source_fail(o,8U,ERROR_INVALID_DATA);
 e=scqkf_source_current(o);return e?scqkf_source_fail(o,9U,e):0;
}
static DWORD scqkf_source_close(PAPACC_MACHINE_FIXTURE_CONTROL *f)
{
 unsigned i;
 if(!f->source_phase)return 0;
 if(f->source_unknown||f->source_thread!=GetCurrentThreadId()||f->source_status>0||f->source_io.Status==(NTSTATUS)0x103L)return ERROR_IO_INCOMPLETE;
 for(i=0;i<5U;i++)if(i!=2U){
  if(f->references[i].handle){if(f->source_close&(1U<<i))return ERROR_IO_INCOMPLETE;f->source_close|=1U<<i;
   if(!CloseHandle(f->references[i].handle))return ERROR_IO_INCOMPLETE;f->references[i].handle=NULL;}
  if(f->references[i].sd){if(f->source_close&(1U<<(8U+i)))return ERROR_IO_INCOMPLETE;f->source_close|=1U<<(8U+i);if(LocalFree(f->references[i].sd))return ERROR_IO_INCOMPLETE;f->references[i].sd=NULL;}
 }
 if(f->source_scratch){if(f->source_close&(1U<<16))return ERROR_IO_INCOMPLETE;f->source_close|=1U<<16;if(LocalFree(f->source_scratch))return ERROR_IO_INCOMPLETE;f->source_scratch=NULL;}
 f->source_phase=4U;return 0; /* image/ancestors remain owned by the existing input chain */
}

/* One fixture-only attributed act. These are not E/Q/S or runtime P. */
DWORD papacc_configuration_personal_machine_fixture_execute(PAPACC_CONFIGURATION_COMMIT *o,FILE *out)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_MACHINE_FIXTURE_CONTROL *f;HANDLE console_in,console_out;DWORD mode,e,i,j;SECURITY_STATUS s;
 char challenge[128],text[512];static const char hex[]="0123456789abcdef";
 if(!o||!out||o->writer!=GetCurrentThreadId())return ERROR_ACCESS_DENIED;n=&o->pending_commit.personal_native;f=&n->machine_fixture;
 if(f->source_phase!=1U||f->source_consumed||f->used||f->key.diagnostic_only||!f->key.diagnostic_budget.active||f->key.diagnostic_budget.first)return ERROR_INVALID_STATE;
 e=scqkf_source_current(o);if(!e)e=papacc_ca_key_fixture_budget_check(&f->key,&o->pending_commit.stop);if(e)return scqkf_source_fail(o,10U,e);
 console_in=GetStdHandle(STD_INPUT_HANDLE);console_out=GetStdHandle(STD_OUTPUT_HANDLE);
 if(!GetConsoleMode(console_in,&mode)||!GetConsoleMode(console_out,&mode)||
  !personal_console_write(console_out,"ONE MACHINE TEST_ONLY FIXTURE; MS Software KSP/RSA2048/export=0. No certificate/sign/delete/CA/CURRENT/E_Q_S/P. Original administrator only. The retained historical request is DATA only; the NEW actual decision must approve THIS live fixture attempt. No consumed authority is restored.\r\n"))return scqkf_source_fail(o,10U,ERROR_ACCESS_DENIED);
 if(!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,f->key_name,-1,text,sizeof(text),NULL,NULL)||
  !personal_console_write(console_out,text)||!personal_console_write(console_out,"\r\nSOURCE ORIGINALS (DATA, not authority):\r\n"))return scqkf_source_fail(o,10U,ERROR_WRITE_FAULT);
 for(i=0;i<5U;i++){
  PAPACC_MACHINE_FIXTURE_REFERENCE *r=&f->references[i];
  if(sprintf_s(text,sizeof(text),"reference=%u identity=%lu:%lu:%lu size=%lu sha256=",i,(unsigned long)r->identity.dwVolumeSerialNumber,(unsigned long)r->identity.nFileIndexHigh,(unsigned long)r->identity.nFileIndexLow,(unsigned long)r->identity.nFileSizeLow)<0||!personal_console_write(console_out,text))return scqkf_source_fail(o,10U,ERROR_WRITE_FAULT);
  for(j=0;j<32U;j++){text[2U*j]=hex[r->digest[j]>>4];text[2U*j+1U]=hex[r->digest[j]&15U];}text[64]=0;
  if(!personal_console_write(console_out,text)||!personal_console_write(console_out,"\r\n"))return scqkf_source_fail(o,10U,ERROR_WRITE_FAULT);
 }
 if(!personal_console_write(console_out,"K1 F0_F5 R0_R4 NECESSARY GUARDS ONLY: entry128/cap192/precall188/private16777216/working16777216/physical7239172096/commit3221225472; freshness100ms/global60000ms; NOT K7 READ. Opaque provider/OS costs and sufficiency UNKNOWN. Persistent fixture is retained even on failure; no automatic cleanup/retry. Confirm ONLY if the separately approved fixture request/decision and its own assurance apply to these originals.\r\n"))return scqkf_source_fail(o,10U,ERROR_WRITE_FAULT);
 if(!personal_console_write(console_out,"S0_S5 FIXTURE SD READBACK: exactly GAx3 OR D01F01FFx3, same owner/protected/three explicit original trustees. SETTER=GA; NO_FILE_MAPPING_NO_MIXED_MASKS; NOT_PRIVATE_ISOLATION_PROOF.\r\n"))return scqkf_source_fail(o,10U,ERROR_WRITE_FAULT);
 strcpy_s(challenge,sizeof(challenge),"OWNER_MACHINE_FIXTURE_CREATE_ONCE ");i=(DWORD)strlen(challenge);
 for(j=0;j<32U;j++){challenge[i+2U*j]=hex[f->plan_id[j]>>4];challenge[i+2U*j+1U]=hex[f->plan_id[j]&15U];}challenge[i+64U]=0;
 if(!personal_console_write(console_out,challenge)||!personal_console_write(console_out,"\r\n")||!personal_console_confirm(console_in,console_out,challenge))return scqkf_source_fail(o,11U,ERROR_CANCELLED);
 e=scqkf_source_current(o);if(!e)e=papacc_ca_key_fixture_budget_check(&f->key,&o->pending_commit.stop);if(e)return scqkf_source_fail(o,11U,e);
 if(InterlockedCompareExchange(&f->source_consumed,1,0))return scqkf_source_fail(o,11U,ERROR_ALREADY_EXISTS);f->source_phase=2U;
 s=papacc_ca_key_fixture_create(&f->key,f->authority,f->generation,(PSID)n->sid,(PSID)f->runtime_sid,&o->pending_commit.stop);
 if(s)return scqkf_source_fail(o,12U,(DWORD)s);
 if(wcscmp(f->key.name,f->key_name))return scqkf_source_fail(o,12U,ERROR_REVISION_MISMATCH);
 e=scqkf_source_current(o);if(e)return scqkf_source_fail(o,13U,e);
 e=scqkf_identity(f,o,&o->pending_commit.stop);if(e)return scqkf_source_fail(o,14U,e);
 e=scqkf_source_current(o);if(e)return scqkf_source_fail(o,15U,e);
 return 0; /* originals/key/hash still owned; caller must reconcile native close */
}
