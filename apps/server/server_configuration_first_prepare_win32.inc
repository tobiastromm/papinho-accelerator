#include "config_source.h"
/* FCA0-FCA7: specialization of the existing original packet and worker.
 * No generic native-ceremony grant; no SCQ purpose, lease or publication. */
static int personal_first_enabled(void)
{
#if defined(PAPACC_ED11_WINDOWS_3MIB) && defined(_M_X64)
 return 1;
#else
 return 0;
#endif
}
static DWORD personal_first_fail(PAPACC_CONFIG_PENDING *p,DWORD error)
{if(!p->personal_native.first_error)p->personal_native.first_error=error?error:ERROR_INVALID_DATA;return p->personal_native.first_error;}
static int personal_first_exclusive(const PAPACC_CONFIG_PENDING *p)
{
 const PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 return !n->machine_fixture.phase&&!n->staging.phase&&!n->staged_update.phase&&!n->scm_registration.phase&&
  !n->runtime_identity.phase&&!n->runtime_cng_probe.phase&&!n->scq_session.phase&&!n->scq_admin.phase&&
  !n->scq_profile_data.phase&&!n->scq_dispatch.phase&&!n->scq_recipient.phase&&!n->scq_receiver.phase;
}
static int personal_first_leaf(const char *s)
{
 size_t i,n;if(!s||(n=strnlen(s,65U))==0U||n>64U||s[n-1U]=='.')return 0;
 for(i=0;i<n;i++)if(!((s[i]>='a'&&s[i]<='z')||(s[i]>='A'&&s[i]<='Z')||(s[i]>='0'&&s[i]<='9')||s[i]=='_'||s[i]=='-'||s[i]=='.'))return 0;
 return strcmp(s,".")&&strcmp(s,"..");
}
static DWORD personal_first_resources(PAPACC_CONFIG_PENDING *p,int worker,int acquiring)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_CA_KEY *key=n->first_key;
 PAPACC_SCM_RESOURCE_SAMPLE *v,*base;PAPACC_CA_FIXTURE_BUDGET *b;LARGE_INTEGER now;DWORD e;
 /* FCR is synchronous: bridge the existing console Stop into this packet
  * before each dependent observation/call, without introducing a worker. */
 if(n->first_mode==3U&&papacc_server_console_win32_stop_requested())InterlockedExchange(&p->stop,1);
 if(!personal_first_enabled()||n->first_owner!=p||!key||!key->first_preparation||key->diagnostic_only||key->diagnostic_stop!=&p->stop||n->first_error)return ERROR_INVALID_STATE;
 if(!worker){e=acquiring?papacc_ca_key_first_budget_check(key,&p->stop):papacc_ca_key_first_budget_postcheck(key,&p->stop);return e?personal_first_fail(p,e):0;}
 b=&key->diagnostic_budget;
 if(n->first_phase!=3U||n->first_worker_thread!=GetCurrentThreadId()||b->self!=b||b->active!=3U||b->first||p->stop||!b->until||GetTickCount64()>=b->until)return personal_first_fail(p,ERROR_INVALID_STATE);
 /* Serialized worker observation, compared to the ORIGINAL process baseline.
  * Does not call the owner-thread key sampler or rebase its budget. */
 e=personal_resource_collect(&n->first_worker_resources,2U,0);if(e)return personal_first_fail(p,e);
 v=&n->first_worker_resources.last;base=&b->observation.first;
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0||v->frequency!=base->frequency||
  (ULONGLONG)now.QuadPart<v->end||v->end<v->begin||
  (ULONGLONG)now.QuadPart-v->begin>v->frequency/10U||GetTickCount64()>=b->until||p->stop)return personal_first_fail(p,ERROR_TIMEOUT);
 if(v->handles>(acquiring?187U:192U)||v->physical_available<3221225472ULL||v->commit_available<3221225472ULL||
  (v->private_bytes>base->private_bytes&&v->private_bytes-base->private_bytes>1994944ULL)||
  (v->working_set>base->working_set&&v->working_set-base->working_set>3145728ULL))return personal_first_fail(p,ERROR_NOT_ENOUGH_QUOTA);
 return 0;
}
static int personal_first_configuration(PAPACC_CONFIG_PENDING *p)
{
 /* Deliberately narrow existing config syntax for PREPARATION, no listener,
  * resolver, network, issuance or implicit default enabling admission. */
 static const unsigned char initial[]="format_version 1\nclient_admission DISABLED\n";
 const unsigned char *source;size_t bytes;
 if(!p->personal_data.effectful||!papacc_personal_effect_source(p->personal_data.plan,p->personal_data.bytes,&source,&bytes))return 0;
 return bytes==sizeof(initial)-1U&&!memcmp(source,initial,bytes)&&!memcmp(p->next,initial,bytes);
}
static int personal_first_child_protected(PSECURITY_DESCRIPTOR sd,PSID actor)
{
 PSID own;PACL acl;BOOL def,present;SECURITY_DESCRIPTOR_CONTROL control;DWORD revision,i,seen=0;
 if(!sd||!IsValidSecurityDescriptor(sd)||!GetSecurityDescriptorOwner(sd,&own,&def)||!own||!EqualSid(own,actor)||
  !GetSecurityDescriptorControl(sd,&control,&revision)||!(control&SE_DACL_PROTECTED)||
  !GetSecurityDescriptorDacl(sd,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl)||acl->AceCount!=3U)return 0;
 for(i=0;i<3U;i++){
  ACCESS_ALLOWED_ACE *a;PSID sid;DWORD bit;
  if(!GetAce(acl,i,(void **)&a)||a->Header.AceType!=ACCESS_ALLOWED_ACE_TYPE||
   a->Header.AceFlags!=(OBJECT_INHERIT_ACE|CONTAINER_INHERIT_ACE)||a->Mask!=FILE_ALL_ACCESS||
   a->Header.AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U)return 0;
  sid=&a->SidStart;if(!IsValidSid(sid)||GetLengthSid(sid)>(DWORD)a->Header.AceSize-(DWORD)FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart))return 0;
  bit=EqualSid(sid,actor)?1U:IsWellKnownSid(sid,WinLocalSystemSid)?2U:IsWellKnownSid(sid,WinBuiltinAdministratorsSid)?4U:0U;
  if(!bit||(seen&bit))return 0;seen|=bit;
 }return seen==7U;
}
static int personal_first_descriptors_same(PSECURITY_DESCRIPTOR a,PSECURITY_DESCRIPTOR b)
{
 PSID first,second;PACL x,y;BOOL present_x,present_y,def;SECURITY_DESCRIPTOR_CONTROL cx,cy;DWORD revision;
 /* Same original object, same OWNER/DACL query mask. Compare the owner and
  * exact ACL, not the packing/order of self-relative SD offsets across APIs.
  * Each producer still revalidates its OWN original SD byte-for-byte. */
 return a&&b&&IsValidSecurityDescriptor(a)&&IsValidSecurityDescriptor(b)&&
  GetSecurityDescriptorOwner(a,&first,&def)&&GetSecurityDescriptorOwner(b,&second,&def)&&first&&second&&EqualSid(first,second)&&
  GetSecurityDescriptorControl(a,&cx,&revision)&&GetSecurityDescriptorControl(b,&cy,&revision)&&
  (cx&SE_DACL_PROTECTED)&&(cy&SE_DACL_PROTECTED)&&
  GetSecurityDescriptorDacl(a,&present_x,&x,&def)&&GetSecurityDescriptorDacl(b,&present_y,&y,&def)&&
  present_x&&present_y&&x&&y&&IsValidAcl(x)&&IsValidAcl(y)&&x->AclSize==y->AclSize&&!memcmp(x,y,x->AclSize);
}
static DWORD personal_first_chain_current(PAPACC_CONFIG_PENDING *p,int worker)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_SCQ_IMAGE_CHAIN *g=&n->scq_inputs.image_chain;
 BY_HANDLE_FILE_INFORMATION f;DWORD i,e,flags,shared=0;HANDLE h;int good;size_t z;
 if(n->first_owner!=p||!n->first_key||g->self!=g||g->owner!=p||g->phase!=2U||g->first||g->stop!=&p->stop||g->paths!=&n->install_paths||g->thread!=n->first_key->diagnostic_budget.thread||
  g->plan.purpose!=PAPACC_FIRST_PREPARE_CHAIN_TAG||!g->count||g->count>PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS||g->image!=p->file||!g->installer||g->scratch)return ERROR_INVALID_STATE;
 z=wcsnlen(n->install_paths.parent,MAX_PATH);
 if(z<11U||z>=MAX_PATH||wcscmp(n->install_paths.parent+z-8U,L"\\Papinho")||(n->first_mode!=1U&&n->first_mode!=2U&&n->first_mode!=3U))return ERROR_INVALID_STATE;
 for(i=3U;i<z;i++)if(n->install_paths.parent[i]==L'\\')shared++;
 if(!shared||n->first_shared_index!=shared||n->first_parent_index!=shared+(n->first_mode!=2U?1U:0U))return ERROR_INVALID_STATE;
 for(i=0;i<=g->count;i++){
  if((e=personal_first_resources(p,worker,1))!=0)return e;
  h=i==g->count?g->image:g->directory[i];
  if(!h||!GetHandleInformation(h,&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(h)!=FILE_TYPE_DISK||!GetFileInformationByHandle(h,&f)||
   !(i==g->count?personal_scq_inputs_same(&g->identity[i],&f):scqi_directory_same(&g->identity[i],&f)))return personal_first_fail(p,ERROR_REVISION_MISMATCH);
  e=scqi_native_file_sd(h,&g->scratch,&g->native_sd,&p->stop);
  if(e||!g->scratch){if(g->scratch)g->phase=4U;return personal_first_fail(p,e?e:ERROR_INVALID_SECURITY_DESCR);}
  good=GetSecurityDescriptorLength(g->scratch)==g->length[i]&&g->sd[i]&&!memcmp(g->sd[i],g->scratch,g->length[i])&&
   (i==g->count?personal_scq_inputs_protected(g->scratch,n->sid):scqi_directory_sd_scope(g->scratch,n->sid,g->installer,i==0U,
    (n->first_mode>=1U&&n->first_mode<=3U)&&i==n->first_shared_index&&i>0U&&i<=n->first_parent_index));
  if(LocalFree(g->scratch)){g->phase=4U;return personal_first_fail(p,ERROR_INVALID_HANDLE);}g->scratch=NULL;
  if(!good)return personal_first_fail(p,ERROR_ACCESS_DENIED);
 }
 if(n->first_parent_index>=g->count||!scqi_directory_same(&g->identity[n->first_parent_index],&n->parent)||
  !personal_first_descriptors_same(g->sd[n->first_parent_index],n->descriptor))return personal_first_fail(p,ERROR_REVISION_MISMATCH);
 return personal_first_resources(p,worker,0);
}
static int personal_first_ready(PAPACC_CONFIG_PENDING *p,int worker)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_PERSONAL_EFFECT_SPEC spec;BYTE parent[12];DWORD ids[3];unsigned i,j;const unsigned char *source;size_t bytes;
 if(!personal_first_enabled()||n->first_owner!=p||!n->first_key||n->first_error||n->phase!=1U||n->presenter_phase!=2U||n->presenter_ca!=n->first_mode||
  n->first_phase<2U||n->first_phase>5U||(n->first_mode!=1U&&n->first_mode!=2U)||p->personal_data.effectful!=n->first_mode||
  !n->first_child_sd||!personal_first_child_protected(n->first_child_sd,n->sid)||
  !personal_first_exclusive(p)||n->install_paths.mode!=PAPACC_PERSONAL_PATHS_NEW||n->install_paths.phase!=PAPACC_PERSONAL_PATHS_READY||
  n->install_paths.thread!=n->first_key->diagnostic_budget.thread||!personal_first_configuration(p)||
  memcmp(n->presenter_attempt,p->personal_data.attempt,32U)||(n->first_mode==1U&&!n->first_service_bytes)||
  !personal_first_leaf(n->plan_leaf)||memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf))||
  (worker?(n->first_phase!=3U):
   (p->personal_data.phase!=PAPACC_PERSONAL_DATA_CONFIRMED&&p->personal_data.phase!=PAPACC_PERSONAL_DATA_CONSUMED))){if(n->first_owner==p&&n->first_phase>=2U)(void)personal_first_fail(p,ERROR_INVALID_STATE);return 0;}
 if(!papacc_personal_effect_source(p->personal_data.plan,p->personal_data.bytes,&source,&bytes)){(void)personal_first_fail(p,ERROR_INVALID_DATA);return 0;}
 ids[0]=n->parent.dwVolumeSerialNumber;ids[1]=n->parent.nFileIndexHigh;ids[2]=n->parent.nFileIndexLow;
 for(i=0;i<3U;i++)for(j=0;j<4U;j++)parent[i*4U+j]=(BYTE)(ids[i]>>(j*8U));
 memset(&spec,0,sizeof(spec));spec.operator_id=n->sid;spec.operator_bytes=n->sid_bytes;spec.parent_id=parent;
 spec.leaf=(BYTE *)n->plan_leaf;spec.leaf_bytes=strlen(n->plan_leaf);spec.configuration=source;spec.configuration_bytes=bytes;
 if(n->first_mode==1U){spec.authority=n->first_authority;spec.generation=n->first_generation;spec.service_id=n->first_service;spec.service_bytes=n->first_service_bytes;}
 if(!(n->first_mode==2U?papacc_personal_parent_matches(p->personal_data.plan,p->personal_data.bytes,&spec):
  papacc_personal_effect_matches(p->personal_data.plan,p->personal_data.bytes,&spec))){(void)personal_first_fail(p,ERROR_INVALID_DATA);return 0;}
 if(personal_first_resources(p,worker,1)||personal_packet_current(p)!=PAPACC_RESULT_OK||personal_packet_source(p)!=PAPACC_RESULT_OK||
  personal_first_chain_current(p,worker)){(void)personal_first_fail(p,ERROR_INVALID_STATE);return 0;}
 return !personal_first_resources(p,worker,1);
}
static int personal_first_child_current(PAPACC_CONFIG_PENDING *p,int worker)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;PAPACC_SCQ_IMAGE_CHAIN *g=&n->scq_inputs.image_chain;
 BY_HANDLE_FILE_INFORMATION f;DWORD e;int valid;
 if(!personal_first_ready(p,worker)){(void)personal_first_fail(p,ERROR_INVALID_STATE);return 0;}
 if(!p->bootstrap_created||!p->bootstrap_container||g->scratch||
  !GetFileInformationByHandle(p->bootstrap_container,&f)||!(f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||
  (f.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||f.dwVolumeSerialNumber!=p->bootstrap_volume||
  f.nFileIndexHigh!=p->bootstrap_high||f.nFileIndexLow!=p->bootstrap_low){(void)personal_first_fail(p,ERROR_REVISION_MISMATCH);return 0;}
 e=scqi_native_file_sd(p->bootstrap_container,&g->scratch,&g->native_sd,&p->stop);
 if(e||!g->scratch){if(g->scratch)g->phase=4U;(void)personal_first_fail(p,e?e:ERROR_INVALID_SECURITY_DESCR);return 0;}
 valid=personal_first_child_protected(g->scratch,n->sid);
 if(LocalFree(g->scratch)){g->phase=4U;(void)personal_first_fail(p,ERROR_INVALID_HANDLE);return 0;}g->scratch=NULL;
 if(!valid){(void)personal_first_fail(p,ERROR_ACCESS_DENIED);return 0;}
 return !personal_first_resources(p,worker,0);
}
/* Original acquisition rejection only. No new query, state, authority or
 * guessed Win32 mapping. Stable predicate numbers follow source order. */
static PAPACC_RESULT personal_first_open_reject(unsigned predicate,PAPACC_RESULT result)
{
 fprintf(stderr,"FCA_ORIGINAL_ACQUISITION_REJECT predicate=%u result=%u; ORIGINAL_BRANCH_NOT_NATIVE_STATUS; NO_NEW_QUERY_NO_AUTHORITY\n",predicate,(unsigned)result);
 return result;
}
static PAPACC_RESULT personal_first_open_mode(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CA_KEY *key,const char *parent,const char *source,DWORD mode,const DWORD *selected,const BYTE *authority,const BYTE *generation)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_IMAGE_CHAIN *g;
 WCHAR root_name[MAX_PATH+4U],part[MAX_PATH],wide_parent[MAX_PATH],wide_source[MAX_PATH];size_t len,i,start,z,last,shared;DWORD e,domain_bytes;WCHAR domain[32],sd[512];SID_NAME_USE use;HANDLE source_parent;SID_IDENTIFIER_AUTHORITY nt=SECURITY_NT_AUTHORITY;
 if(!o||!key||!parent||!source||o->writer!=GetCurrentThreadId()||o->initialized||o->stopped||o->unresolved||o->published||o->prepared||o->committed_pending||o->committed_bytes||o->recovery_selected||o->recovery_guard||o->authority_lost)return personal_first_open_reject(1U,PAPACC_RESULT_INVALID_STATE);
 if(!personal_first_enabled())return personal_first_open_reject(2U,PAPACC_RESULT_NOT_SUPPORTED);
 if((mode!=1U&&mode!=2U&&mode!=3U)||(mode==3U&&(!selected||!authority||!generation)))return personal_first_open_reject(3U,PAPACC_RESULT_INVALID_ARGUMENT);
 {ULONG_PTR begin=(ULONG_PTR)o,at=(ULONG_PTR)key;
  if(sizeof(*o)>MAXULONG_PTR-begin||sizeof(*key)>MAXULONG_PTR-at||
   (begin<at+sizeof(*key)&&at<begin+sizeof(*o)))return personal_first_open_reject(4U,PAPACC_RESULT_INVALID_ARGUMENT);
 }
 p=&o->pending_commit;n=&p->personal_native;g=&n->scq_inputs.image_chain;
 if(n->phase||n->first_owner||n->first_key||g->phase||n->scq_inputs.phase||p->active||p->thread||p->personal_data.phase||p->personal_ca||p->personal_ca_candidate||!personal_first_exclusive(p)||
  key->first_preparation!=(mode==3U?2U:1U)||key->diagnostic_budget.active!=2U||key->diagnostic_budget.self!=&key->diagnostic_budget||
  key->diagnostic_budget.thread!=o->writer||key->diagnostic_stop!=&p->stop||key->state!=PAPACC_CA_KEY_EMPTY)return personal_first_open_reject(5U,PAPACC_RESULT_INVALID_STATE);
 len=strnlen(parent,MAX_PATH);z=strnlen(source,MAX_PATH);
 if(len<3U||len>=MAX_PATH||parent[len-1U]=='\\'||z<4U||z>=MAX_PATH||source[0]!=parent[0]||source[1]!=':'||source[2]!='\\')return personal_first_open_reject(6U,PAPACC_RESULT_INVALID_ARGUMENT);
 last=2U;for(i=3U;i<z;i++){if((unsigned char)source[i]<32U||source[i]=='/'||source[i]==':'||source[i]=='"')return personal_first_open_reject(7U,PAPACC_RESULT_INVALID_ARGUMENT);if(source[i]=='\\')last=i;}
 if(!personal_first_leaf(source+last+1U)||!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,source,-1,wide_source,MAX_PATH))return personal_first_open_reject(8U,PAPACC_RESULT_INVALID_ARGUMENT);
 if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,parent,-1,wide_parent,MAX_PATH)||
  n->install_paths.mode!=PAPACC_PERSONAL_PATHS_NEW||n->install_paths.phase!=PAPACC_PERSONAL_PATHS_READY||n->install_paths.thread!=o->writer)return personal_first_open_reject(9U,PAPACC_RESULT_INVALID_ARGUMENT);
 shared=wcsnlen(n->install_paths.parent,MAX_PATH);
 if(shared<11U||shared>=MAX_PATH||wcscmp(n->install_paths.parent+shared-8U,L"\\Papinho"))return personal_first_open_reject(10U,PAPACC_RESULT_INVALID_ARGUMENT);
 shared-=8U;
 if((mode!=2U&&wcscmp(wide_parent,n->install_paths.parent))||
  (mode==2U&&(wcslen(wide_parent)!=shared||wcsncmp(wide_parent,n->install_paths.parent,shared))))return personal_first_open_reject(11U,PAPACC_RESULT_INVALID_ARGUMENT);
 if(mode==3U){
  memcpy(n->prepared_parent,selected,12U);memcpy(n->prepared_target,selected+3U,12U);memcpy(n->prepared_source,selected+6U,12U);
  memcpy(n->first_authority,authority,16U);memcpy(n->first_generation,generation,16U);
 }
 n->first_owner=p;n->first_key=key;n->first_phase=1U;n->first_mode=mode;n->first_shared_index=MAXDWORD;
 if(personal_first_resources(p,0,1)||papacc_configuration_personal_native_open(o,parent)!=PAPACC_RESULT_OK)return personal_first_open_reject(12U,PAPACC_RESULT_INVALID_STATE);
 /* Bind the worker's pre-existing identity slot to THIS original query.
  * Historical DATA presentation never populated these effect-side fields. */
 p->bootstrap_parent_volume=n->parent.dwVolumeSerialNumber;p->bootstrap_parent_high=n->parent.nFileIndexHigh;p->bootstrap_parent_low=n->parent.nFileIndexLow;
 g->self=g;g->owner=p;g->stop=&p->stop;g->paths=&n->install_paths;g->thread=GetCurrentThreadId();g->phase=1U;g->plan.purpose=PAPACC_FIRST_PREPARE_CHAIN_TAG;
 g->plan.valid_from[0]=0;g->plan.valid_from[1]=0;g->plan.valid_until[0]=(DWORD)key->diagnostic_budget.until;g->plan.valid_until[1]=(DWORD)(key->diagnostic_budget.until>>32);
 if(personal_first_resources(p,0,1)||!ConvertStringSidToSidW(L"S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464",&g->installer))return personal_first_open_reject(13U,PAPACC_RESULT_INVALID_STATE);
 {WCHAR drive[4]={wide_parent[0],L':',L'\\',0};e=scq_p1_native_name(drive,1,root_name);if(e)return personal_first_open_reject(14U,PAPACC_RESULT_INVALID_STATE);}
 if(personal_first_resources(p,0,1)||scqi_chain_open(g,0U,NULL,root_name,1))return personal_first_open_reject(15U,PAPACC_RESULT_INVALID_STATE);g->count=1U;
 start=3U;
 for(i=start;i<=len;i++)if(!wide_parent[i]||wide_parent[i]==L'\\'){
  if(i==start||i-start>=MAX_PATH||g->count==PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS)return personal_first_open_reject(16U,PAPACC_RESULT_LIMIT_EXCEEDED);
  memcpy(part,wide_parent+start,(i-start)*sizeof(WCHAR));part[i-start]=0;
  if(!wcscmp(part,L".")||!wcscmp(part,L"..")||personal_first_resources(p,0,1)||
   (i==len?scqi_chain_parent_metadata(g,g->count,g->directory[g->count-1U],part):
    scqi_chain_open(g,g->count,g->directory[g->count-1U],part,1)))return personal_first_open_reject(17U,PAPACC_RESULT_INVALID_STATE);
  g->count++;if(i==shared)n->first_shared_index=g->count-1U;start=i+1U;
 }
 if(mode==3U){size_t binary=wcsnlen(n->install_paths.binary,MAX_PATH);
  if(binary>=MAX_PATH||last!=binary||wcsncmp(wide_source,n->install_paths.binary,binary))return personal_first_open_reject(30U,PAPACC_RESULT_INVALID_ARGUMENT);
  wcscpy_s(g->binary,MAX_PATH,n->install_paths.binary);
 }
 n->first_parent_index=g->count-1U;source_parent=p->bootstrap_parent; /* original traversal right, never metadata reference */
 if(mode==3U){
  if(n->parent.dwVolumeSerialNumber!=selected[0]||n->parent.nFileIndexHigh!=selected[1]||n->parent.nFileIndexLow!=selected[2]||
   g->count==PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS||personal_first_resources(p,0,1)||
   scqi_chain_open(g,g->count,p->bootstrap_parent,L"Accelerator",1))return personal_first_open_reject(27U,PAPACC_RESULT_INVALID_STATE);
  {BY_HANDLE_FILE_INFORMATION *f=&g->identity[g->count];
   if(f->dwVolumeSerialNumber!=selected[3]||f->nFileIndexHigh!=selected[4]||f->nFileIndexLow!=selected[5]||
    !(f->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)||(f->dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)||
    !personal_first_child_protected(g->sd[g->count],n->sid))return personal_first_open_reject(28U,PAPACC_RESULT_INVALID_STATE);
  }g->count++;
 }
 /* Reuse the same bounded custody for a separately protected source branch.
  * No new chain quota, path authority or requirement to put config in data. */
 if(last!=len||memcmp(parent,source,len)){
  source_parent=g->directory[0];start=3U;
  for(i=start;wide_source[i];i++)if(wide_source[i]==L'\\'){
   if(i==start||i-start>=MAX_PATH||g->count==PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS)return personal_first_open_reject(18U,PAPACC_RESULT_LIMIT_EXCEEDED);
   memcpy(part,wide_source+start,(i-start)*sizeof(WCHAR));part[i-start]=0;
   if(!wcscmp(part,L".")||!wcscmp(part,L"..")||personal_first_resources(p,0,1)||scqi_chain_open(g,g->count,source_parent,part,1))return personal_first_open_reject(19U,PAPACC_RESULT_INVALID_STATE);
   source_parent=g->directory[g->count++];start=i+1U;
  }
 }
 if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,source+last+1U,-1,part,MAX_PATH)||personal_first_resources(p,0,1)||
  scqi_chain_open(g,g->count,source_parent,part,0))return personal_first_open_reject(20U,PAPACC_RESULT_INVALID_STATE);
 p->file=g->image;n->source=g->identity[g->count];g->phase=2U;
 if((n->source.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||n->source.nNumberOfLinks!=1U||
  n->source.nFileSizeHigh||!n->source.nFileSizeLow||n->source.nFileSizeLow>PC_SOURCE_MAX||personal_first_chain_current(p,0))return personal_first_open_reject(21U,PAPACC_RESULT_INVALID_STATE);
 if(n->first_shared_index==MAXDWORD||n->first_shared_index>n->first_parent_index)return personal_first_open_reject(22U,PAPACC_RESULT_INVALID_STATE);
 if(mode==3U&&(n->source.dwVolumeSerialNumber!=selected[6]||n->source.nFileIndexHigh!=selected[7]||n->source.nFileIndexLow!=selected[8]))return personal_first_open_reject(29U,PAPACC_RESULT_INVALID_STATE);
 if(mode!=2U){ /* PDP does not look up runtime or generate CA identities. */
 /* Selected virtual ACCOUNT is candidate identity only, not a live token/P4. */
 domain_bytes=32U;n->first_service_bytes=sizeof(n->first_service);
 if(personal_first_resources(p,0,1)||!LookupAccountNameW(NULL,L"NT SERVICE\\PapinhoAccelerator",n->first_service,&n->first_service_bytes,domain,&domain_bytes,&use)||
  n->first_service_bytes<8U||n->first_service_bytes>sizeof(n->first_service)||!IsValidSid(n->first_service)||GetLengthSid(n->first_service)>n->first_service_bytes||
  (use!=SidTypeUser&&use!=SidTypeWellKnownGroup)||EqualSid(n->first_service,n->sid)||
  memcmp(GetSidIdentifierAuthority(n->first_service),&nt,sizeof(nt))||*GetSidSubAuthorityCount(n->first_service)!=6U||*GetSidSubAuthority(n->first_service,0)!=80U)return personal_first_open_reject(23U,PAPACC_RESULT_INVALID_STATE);
 /* cbSid is buffer capacity, not an authoritative encoded SID length on
  * successful LookupAccountName. Bind only the validated actual SID bytes,
  * as the existing service-SID and runtime-identity producers already do. */
 n->first_service_bytes=GetLengthSid(n->first_service);
 if(mode==1U&&(personal_first_resources(p,0,1)||BCryptGenRandom(NULL,n->first_authority,16U,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0||
  personal_first_resources(p,0,1)||BCryptGenRandom(NULL,n->first_generation,16U,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0))return personal_first_open_reject(24U,PAPACC_RESULT_INVALID_STATE);
 {BYTE a=0,b=0;for(i=0;i<16U;i++){a|=n->first_authority[i];b|=n->first_generation[i];}
  if(!a||!b||!memcmp(n->first_authority,n->first_generation,16U))return personal_first_open_reject(25U,PAPACC_RESULT_INVALID_STATE);
 }}
 if(mode==3U)return PAPACC_RESULT_OK; /* existing object, no creation SD/template */
 if(_snwprintf_s(sd,512,_TRUNCATE,L"O:%SD:P(A;OICI;FA;;;%S)(A;OICI;FA;;;SY)(A;OICI;FA;;;BA)",n->sid_text,n->sid_text)<0||
  personal_first_resources(p,0,1)||!ConvertStringSecurityDescriptorToSecurityDescriptorW(sd,SDDL_REVISION_1,&n->first_child_sd,NULL)||
  GetSecurityDescriptorLength(n->first_child_sd)>2048U||!personal_first_child_protected(n->first_child_sd,n->sid))return personal_first_open_reject(26U,PAPACC_RESULT_INVALID_STATE);
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_first_open(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CA_KEY *key,const char *parent,const char *source)
{return personal_first_open_mode(o,key,parent,source,1U,NULL,NULL,NULL);}
PAPACC_RESULT papacc_configuration_personal_parent_open(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CA_KEY *key,const char *parent,const char *source)
{return personal_first_open_mode(o,key,parent,source,2U,NULL,NULL,NULL);}
/* Only an already acquired private preparation packet may build this DATA.
 * The generic DATA/initialized-owner entrypoints are intentionally unchanged. */
PAPACC_RESULT papacc_configuration_personal_preparation_plan(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_EFFECT_SPEC v;BYTE parent[12];DWORD ids[3];size_t bytes;unsigned i,j;
 if(!o||o->writer!=GetCurrentThreadId()||o->initialized||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(n->first_owner!=p||(n->first_mode!=1U&&n->first_mode!=2U)||n->first_phase!=1U||n->first_error||n->phase!=1U||
  p->active||p->thread||p->bootstrap_created||p->personal_data.phase!=PAPACC_PERSONAL_DATA_CAPTURED||p->personal_data.effectful||p->personal_data.consumed||
  strcmp(n->plan_leaf,n->first_mode==2U?"Papinho":"Accelerator")||memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf))||
  personal_first_resources(p,0,1)||personal_first_chain_current(p,0)||personal_packet_current(p)!=PAPACC_RESULT_OK||personal_packet_source(p)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 ids[0]=n->parent.dwVolumeSerialNumber;ids[1]=n->parent.nFileIndexHigh;ids[2]=n->parent.nFileIndexLow;
 for(i=0;i<3U;i++)for(j=0;j<4U;j++)parent[i*4U+j]=(BYTE)(ids[i]>>(j*8U));
 memset(&v,0,sizeof(v));v.operator_id=n->sid;v.operator_bytes=n->sid_bytes;v.parent_id=parent;v.leaf=(BYTE *)n->plan_leaf;v.leaf_bytes=strlen(n->plan_leaf);
 v.configuration=p->next;v.configuration_bytes=p->personal_data.bytes;
 if(n->first_mode==1U){v.authority=n->first_authority;v.generation=n->first_generation;v.service_id=n->first_service;v.service_bytes=n->first_service_bytes;}
 bytes=n->first_mode==2U?papacc_personal_parent_build(&v,p->previous+1024U,PAPACC_PERSONAL_PLAN_MAX):papacc_personal_effect_build(&v,p->previous+1024U,PAPACC_PERSONAL_PLAN_MAX);
 if(!bytes)return PAPACC_RESULT_INVALID_STATE;
 return (n->first_mode==2U?papacc_personal_parent_upgrade(&p->personal_data,p->previous+1024U,bytes):
  papacc_personal_effect_upgrade(&p->personal_data,p->previous+1024U,bytes))?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_personal_first_confirm(PAPACC_CONFIGURATION_COMMIT *o,const BYTE attempt[32])
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!o||!attempt||o->writer!=GetCurrentThreadId()||o->initialized||o->stopped||o->unresolved)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(n->first_phase!=1U||p->active||p->thread||memcmp(attempt,p->personal_data.attempt,32U)||
  n->presenter_phase!=2U||!n->presenter_ca||p->personal_data.phase!=PAPACC_PERSONAL_DATA_CONFIRMED||p->personal_data.consumed||
  !personal_first_configuration(p)||personal_first_chain_current(p,0)||papacc_ca_key_first_budget_arm(n->first_key,&p->stop)){
  if(n->first_owner==p)(void)personal_first_fail(p,ERROR_INVALID_STATE);return PAPACC_RESULT_INVALID_STATE;
 }
 n->first_phase=2U;
 if(!personal_first_ready(p,0)){(void)personal_first_fail(p,ERROR_INVALID_STATE);return PAPACC_RESULT_INVALID_STATE;}
 return PAPACC_RESULT_OK;
}

/* Selected original IDs are DATA. This producer independently reacquires the
 * actual protected objects; no creation/borrow/presenter authority is granted. */
PAPACC_RESULT papacc_configuration_personal_prepared_open(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_CA_KEY *key,const char *parent,const char *source,const DWORD selected[9],const BYTE a[16],const BYTE g[16])
{return personal_first_open_mode(o,key,parent,source,3U,selected,a,g);}
