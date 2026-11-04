/* Existing personal presenter/owner, separate attributed SCM_REGISTER act.
 * Same native operator/parent primitives as F1-F6, never reuse a NEW_INSTALL,
 * CA_INITIALIZE, LAB or SCQ confirmation. No protected effect here. */
#include "personal_console_token_win32.inc"
#include "server_scm_registration_resources_win32.inc"
#include "server_original_file_win32.inc"
static int scm_register_note(PAPACC_SCM_REGISTRATION_ACT *a,DWORD stage,DWORD kind,DWORD value,DWORD object)
{
 if(!a->diagnostic_stage){a->diagnostic_stage=stage;a->diagnostic_kind=kind;a->diagnostic_value=value;
  a->diagnostic_object=object;a->diagnostic_phase=a->phase;}
 return 0; /* metadata cannot supply authority */
}
static void scm_register_diagnostic_emit(const PAPACC_SCM_REGISTRATION_ACT *a,FILE *err)
{
 if(a->diagnostic_stage)(void)fprintf(err,"SCM_REGISTER_FIRST_CAUSE stage=%lu kind=%lu value=0x%08lx object=%lu act_phase=%lu native_stage=%lu native_kind=%lu native_error=%lu; NO_RAW_REFERENCES_OR_SECRETS; NOT_AUTHORITY\n",
  (unsigned long)a->diagnostic_stage,(unsigned long)a->diagnostic_kind,(unsigned long)a->diagnostic_value,
  (unsigned long)a->diagnostic_object,(unsigned long)a->diagnostic_phase,(unsigned long)a->diagnostic_native_stage,
  (unsigned long)a->diagnostic_native_kind,(unsigned long)a->diagnostic_native_error);
}
DWORD papacc_server_launcher_scm_registration_target(WCHAR *out,size_t cap)
{
 PAPACC_PERSONAL_INSTALL_PATHS paths;volatile LONG stop=0;DWORD e;size_t n;
 static const WCHAR leaf[]=L"\\papacc_server.exe";
 if(!out||!cap||papacc_server_console_win32_stop_requested())return ERROR_INVALID_STATE;
 ZeroMemory(&paths,sizeof(paths));e=papacc_personal_paths_binary(&paths,&stop);if(e)return e;
 n=wcslen(paths.binary);if(n+wcslen(leaf)+1U>cap)return ERROR_INSUFFICIENT_BUFFER;
 if(papacc_server_console_win32_stop_requested())return ERROR_CANCELLED;
 memcpy(out,paths.binary,n*sizeof(WCHAR));memcpy(out+n,leaf,sizeof(leaf));return 0;
}
/* Hashes are original content evidence, not approval or loaded-image proof. */
static int scm_register_dependency(PAPACC_CONFIGURATION_COMMIT *c,unsigned index,int capture)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&c->pending_commit.personal_native;
 PAPACC_SCM_DEPENDENCY_REFERENCE *d=&n->scm_registration.dependencies[index];BY_HANDLE_FILE_INFORMATION f;BYTE digest[32],acl[32];
 PAPACC_SCM_REGISTRATION_ACT *a=&n->scm_registration;DWORD object=index+1U;
 if(papacc_server_console_win32_stop_requested()||c->pending_commit.stop)return scm_register_note(a,SCM_D_STOP,SCM_DIAG_POLICY,0,object);
 if(!d->handle)return scm_register_note(a,SCM_D_DEPENDENCY_REFERENCE,SCM_DIAG_POLICY,0,object);
 if(!GetFileInformationByHandle(d->handle,&f))return scm_register_note(a,SCM_D_DEPENDENCY_QUERY,SCM_DIAG_WIN32,GetLastError(),object);
 if(f.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))return scm_register_note(a,SCM_D_DEPENDENCY_ATTRIBUTES,SCM_DIAG_POLICY,0,object);
 if(f.dwVolumeSerialNumber!=n->parent.dwVolumeSerialNumber)return scm_register_note(a,SCM_D_DEPENDENCY_VOLUME,SCM_DIAG_POLICY,0,object);
 if(!f.nFileSizeLow&&!f.nFileSizeHigh)return scm_register_note(a,SCM_D_DEPENDENCY_SIZE,SCM_DIAG_POLICY,0,object);
 if(!scm_acl_hash(n,d->handle,acl))return scm_register_note(a,SCM_D_DEPENDENCY_SD,SCM_DIAG_HELPER_UNKNOWN,0,object);
 if(!scm_register_hash(d->handle,digest))return scm_register_note(a,SCM_D_DEPENDENCY_HASH,SCM_DIAG_HELPER_UNKNOWN,0,object);
 if((f.dwVolumeSerialNumber==n->scm_registration.captured.image.identity.dwVolumeSerialNumber&&
  f.nFileIndexHigh==n->scm_registration.captured.image.identity.nFileIndexHigh&&f.nFileIndexLow==n->scm_registration.captured.image.identity.nFileIndexLow)||
  (index&&f.dwVolumeSerialNumber==n->scm_registration.dependencies[0].identity.dwVolumeSerialNumber&&
  f.nFileIndexHigh==n->scm_registration.dependencies[0].identity.nFileIndexHigh&&f.nFileIndexLow==n->scm_registration.dependencies[0].identity.nFileIndexLow))return scm_register_note(a,SCM_D_DEPENDENCY_ALIAS,SCM_DIAG_POLICY,0,object);
 if(capture){d->identity=f;memcpy(d->digest,digest,32);memcpy(d->acl,acl,32);return 1;}
 if(!scm_register_id(&f,&d->identity))return scm_register_note(a,SCM_D_DEPENDENCY_IDENTITY,SCM_DIAG_POLICY,0,object);
 if(memcmp(d->digest,digest,32))return scm_register_note(a,SCM_D_DEPENDENCY_HASH_CHANGED,SCM_DIAG_POLICY,0,object);
 if(memcmp(d->acl,acl,32))return scm_register_note(a,SCM_D_DEPENDENCY_SD_CHANGED,SCM_DIAG_POLICY,0,object);
 return 1;
}
static int scm_register_text(const WCHAR *s,size_t capacity)
{size_t i;if(!s)return 0;for(i=0;i<capacity;i++){if(!s[i])return i!=0U;if(s[i]<32U)return 0;}return 0;}
static int scm_register_scope(const PAPACC_SERVICE_REGISTRATION_VIEW *v)
{
 size_t n;static const WCHAR suffix[]=L"\" service host scq-closed";
 if(!v||!papacc_server_service_registration_view_current(v)||!v->recipient||!v->image.handle||!v->image.sha256||v->image.owner_thread!=GetCurrentThreadId()||
 !scm_register_text(v->image.path,MAX_PATH)||!scm_register_text(v->command,MAX_PATH+32U)||
 !scm_register_text(v->service,64U)||!scm_register_text(v->account,96U)||
 wcscmp(v->service,L"PapinhoAccelerator")||wcscmp(v->account,L"NT SERVICE\\PapinhoAccelerator")||
 v->start_type!=SERVICE_DEMAND_START||v->sid_type!=SERVICE_SID_TYPE_UNRESTRICTED)return 0;
 n=wcslen(v->image.path);
 return v->command[0]==L'"'&&wcslen(v->command)==1U+n+wcslen(suffix)&&
 !memcmp(v->command+1,v->image.path,n*sizeof(WCHAR))&&!wcscmp(v->command+1+n,suffix);
}
static DWORD scm_register_fail(PAPACC_SCM_REGISTRATION_ACT *a,DWORD error)
{if(!a->first)a->first=error;if(a->phase!=6U)a->phase=7U;return error;}
static DWORD scm_register_current(const PAPACC_SERVICE_REGISTRATION_VIEW *v,unsigned minimum)
{
 PAPACC_CONFIGURATION_COMMIT *c=&root.configuration_commit;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&c->pending_commit.personal_native;
 PAPACC_SCM_REGISTRATION_ACT *a=&n->scm_registration;BY_HANDLE_FILE_INFORMATION f,g;BYTE digest[32],acl[32];unsigned i;PAPACC_RESULT checked;
 if(state!=PAPACC_LAUNCHER_ACTIVE||root.owner_thread!=GetCurrentThreadId()||a->self!=a||a->thread!=GetCurrentThreadId()||
 a->invocation!=root.invocation||a->original!=v||a->phase<minimum||a->phase>4U)return ERROR_INVALID_STATE;
#define SCM_REJECT(stage,kind,value,error) do{(void)scm_register_note(a,stage,kind,(DWORD)(value),0);return scm_register_fail(a,error);}while(0)
 if((a->phase==4U)!=(a->consumed!=0U))SCM_REJECT(SCM_D_CONSUMPTION_STATE,SCM_DIAG_POLICY,0,ERROR_INVALID_STATE);
 if(papacc_server_console_win32_stop_requested()||c->pending_commit.stop)SCM_REJECT(SCM_D_STOP,SCM_DIAG_POLICY,0,ERROR_CANCELLED);
 if(!scm_register_scope(v))SCM_REJECT(SCM_D_SCOPE,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(v->recipient!=a->captured.recipient)SCM_REJECT(SCM_D_RECIPIENT,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(v->image.handle!=a->captured.image.handle)SCM_REJECT(SCM_D_IMAGE_REFERENCE,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(!scm_register_id(&v->image.identity,&a->captured.image.identity))SCM_REJECT(SCM_D_CAPTURED_IDENTITY,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(wcscmp(v->image.path,a->path))SCM_REJECT(SCM_D_PATH,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(wcscmp(v->command,a->command))SCM_REJECT(SCM_D_COMMAND,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(memcmp(v->image.sha256,a->digest,32))SCM_REJECT(SCM_D_CAPTURED_DIGEST,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 /* The exact binary-only layout must use its existing governed validator.
  * Generic NEW/EXISTING validation deliberately rejects BINARY_ONLY. */
 checked=papacc_configuration_personal_layout_binary_current(c);
 if(checked!=PAPACC_RESULT_OK)SCM_REJECT(SCM_D_LAYOUT_CURRENT,SCM_DIAG_RESULT,checked,ERROR_ACCESS_DENIED);
 checked=papacc_configuration_personal_native_current(c);
 if(checked!=PAPACC_RESULT_OK){
  if(!a->diagnostic_stage){a->diagnostic_native_stage=n->diagnostic_first;a->diagnostic_native_kind=n->diagnostic_kind;a->diagnostic_native_error=n->diagnostic_error;}
  SCM_REJECT(SCM_D_NATIVE_CURRENT,SCM_DIAG_RESULT,checked,ERROR_ACCESS_DENIED);
 }
 if(!n->scm_image)SCM_REJECT(SCM_D_RELATIVE_REFERENCE,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(!GetFileInformationByHandle(v->image.handle,&f))SCM_REJECT(SCM_D_ORIGINAL_QUERY,SCM_DIAG_WIN32,GetLastError(),ERROR_ACCESS_DENIED);
 if(!GetFileInformationByHandle(n->scm_image,&g))SCM_REJECT(SCM_D_RELATIVE_QUERY,SCM_DIAG_WIN32,GetLastError(),ERROR_ACCESS_DENIED);
 if(!scm_register_id(&f,&a->captured.image.identity))SCM_REJECT(SCM_D_ORIGINAL_IDENTITY,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(!scm_register_id(&f,&g))SCM_REJECT(SCM_D_RELATIVE_IDENTITY,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(f.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))SCM_REJECT(SCM_D_IMAGE_ATTRIBUTES,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(!scm_acl_hash(n,n->scm_image,acl))SCM_REJECT(SCM_D_IMAGE_SD,SCM_DIAG_HELPER_UNKNOWN,0,ERROR_ACCESS_DENIED);
 if(memcmp(acl,a->image_acl,32))SCM_REJECT(SCM_D_IMAGE_SD_CHANGED,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(!scm_register_hash(n->scm_image,digest))SCM_REJECT(SCM_D_IMAGE_HASH,SCM_DIAG_HELPER_UNKNOWN,0,ERROR_ACCESS_DENIED);
 if(memcmp(digest,a->digest,32))SCM_REJECT(SCM_D_IMAGE_HASH_CHANGED,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
 if(!a->package_captured)SCM_REJECT(SCM_D_PACKAGE,SCM_DIAG_POLICY,0,ERROR_ACCESS_DENIED);
#undef SCM_REJECT
 for(i=0;i<2U;i++)if(!scm_register_dependency(c,i,0))return scm_register_fail(a,ERROR_ACCESS_DENIED);
 return 0;
}
DWORD papacc_server_launcher_scm_registration_current(const PAPACC_SERVICE_REGISTRATION_VIEW *v)
{return scm_register_current(v,3U);}
DWORD papacc_server_launcher_scm_registration_take(const PAPACC_SERVICE_REGISTRATION_VIEW *v)
{
 PAPACC_SCM_REGISTRATION_ACT *a=&root.configuration_commit.pending_commit.personal_native.scm_registration;DWORD e;
 if(root.owner_thread!=GetCurrentThreadId())return ERROR_INVALID_STATE;
 if(a->phase!=3U||a->consumed)return ERROR_ALREADY_EXISTS;
 e=scm_register_current(v,3U);if(e)return e;
 e=scm_register_effect_begin(a,0U);if(e)return scm_register_fail(a,e);
 a->consumed=1U;a->phase=4U; /* irreversible, before CreateService */
 return scm_register_current(v,4U);
}
DWORD papacc_server_launcher_scm_registration_effect(const PAPACC_SERVICE_REGISTRATION_VIEW *v,unsigned effect,unsigned returned,DWORD result)
{
 PAPACC_SCM_REGISTRATION_ACT *a=&root.configuration_commit.pending_commit.personal_native.scm_registration;
 LARGE_INTEGER now,frequency;unsigned index;DWORD e;
 if(effect<1U||effect>2U||returned>1U||root.owner_thread!=GetCurrentThreadId()||
  a->self!=a||a->original!=v||a->thread!=GetCurrentThreadId()||a->invocation!=root.invocation||
  a->assurance!=PAPACC_SCM_ASSURANCE_LIMITED_R0_R9||!a->consumed||a->phase!=4U)return ERROR_INVALID_STATE;
 index=effect-1U;
 if(!returned){
  if(index!=1U||result)return ERROR_INVALID_STATE;
  e=scm_register_current(v,4U);if(e)return e;
  e=scm_register_effect_begin(a,index);return e?scm_register_fail(a,e):0;
 }
 /* A returned effect is recorded even after Stop: this is disposition, not
  * authority for the next mutation. A failed clock keeps the cohort unknown. */
 if(a->effect_state[index]!=1U)return ERROR_INVALID_STATE;
 if(!QueryPerformanceFrequency(&frequency)||!QueryPerformanceCounter(&now)||frequency.QuadPart<=0||
  (ULONGLONG)frequency.QuadPart!=a->resources.last.frequency||now.QuadPart<0||
  (ULONGLONG)now.QuadPart<a->effect_begin[index])return scm_register_fail(a,ERROR_INVALID_DATA);
 a->effect_result[index]=result;a->effect_end[index]=(ULONGLONG)now.QuadPart;a->effect_state[index]=2U;
 if(a->resources.sink&&fprintf(a->resources.sink,"SCM_LIMITED_EFFECT effect=%u begin=%llu end=%llu frequency=%llu result=%lu; OPAQUE_MAX=UNKNOWN; OS_QUIESCENCE=UNKNOWN; NO_RETRY\n",
  effect,a->effect_begin[index],a->effect_end[index],a->resources.last.frequency,(unsigned long)result)<0)return scm_register_fail(a,ERROR_WRITE_FAULT);
 return 0;
}
DWORD papacc_server_launcher_scm_registration_finish(const PAPACC_SERVICE_REGISTRATION_VIEW *v,int originals_closed)
{
 PAPACC_CONFIGURATION_COMMIT *c=&root.configuration_commit;PAPACC_SCM_REGISTRATION_ACT *a=&c->pending_commit.personal_native.scm_registration;
 if(root.owner_thread!=GetCurrentThreadId()||a->self!=a||a->original!=v||a->invocation!=root.invocation||a->phase==5U||a->phase==6U)return ERROR_INVALID_STATE;
 if(!originals_closed){a->phase=6U;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return ERROR_INVALID_HANDLE;}
 (void)scm_register_observe(a,PAPACC_SCM_RESOURCE_BEFORE_CLOSE);
 /* Borrowing SCM owner has closed or proved no pending API. Only then close
  * canonical image/token/parent. No revalidation of already closed borrows. */
 if(papacc_configuration_personal_native_close(c)!=PAPACC_RESULT_OK){a->phase=6U;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return ERROR_INVALID_HANDLE;}
 (void)scm_register_observe(a,PAPACC_SCM_RESOURCE_CLOSED);
 if(a->phase==4U&&a->consumed&&a->effect_state[0]==2U&&a->effect_state[1]==2U&&!a->effect_result[0]&&!a->effect_result[1]){
  if(a->resources.sink)(void)fprintf(a->resources.sink,"SCM_LIMITED_REGISTRATION_RESULT=STOPPED_REGISTRATION_QUALIFIED; consumed=1; controlled_close=KNOWN; OPAQUE_MAX=UNKNOWN; OS_QUIESCENCE=UNKNOWN; NO_START; NO_PRODUCTIVE_PROMOTION; NO_RETRY\n");
 }
 a->phase=5U;InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);return 0;
}
int papacc_server_launcher_scm_registration_present(const PAPACC_SERVICE_REGISTRATION_VIEW *v,FILE *out,FILE *err)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCM_REGISTRATION_ACT *a;
 HANDLE input=GetStdHandle(STD_INPUT_HANDLE),output=GetStdHandle(STD_OUTPUT_HANDLE);DWORD console_mode;
 WCHAR expected_path[MAX_PATH];char parent[MAX_PATH],text[MAX_PATH*4+128],reply[96];size_t i,z;int ready=0;
 PAPACC_RESULT checked;NTSTATUS rng_status;DWORD output_written;
 static const char first_plan[]="\nSCM_REGISTER_CLOSED - PERSONAL_LOCAL\nOperator SID: ";
 static const char hex[]="0123456789abcdef";
 if(!out||!err)return 2;
 if(!scm_register_scope(v)){(void)fprintf(err,"SCM_REGISTER_FIRST_CAUSE stage=%u kind=%u value=0x00000000 act_phase=0; NOT_AUTHORITY\n",SCM_D_SCOPE,SCM_DIAG_POLICY);return 2;}
 if(!GetConsoleMode(input,&console_mode)||!GetConsoleMode(output,&console_mode)){
  DWORD console_error=GetLastError();(void)fprintf(err,"SCM_REGISTER_FIRST_CAUSE stage=%u kind=%u value=0x%08lx act_phase=0; NOT_AUTHORITY\n",SCM_D_ENTRY,SCM_DIAG_WIN32,(unsigned long)console_error);return 2;
 }
 if(InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,0)!=0)return 2;
 if(next_invocation==(PAPACC_U64)-1){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return 2;}
 memset(&root,0,sizeof(root));root.invocation=++next_invocation;root.owner_thread=GetCurrentThreadId();
 c=&root.configuration_commit;c->writer=root.owner_thread;c->pending_commit.file=INVALID_HANDLE_VALUE;
 n=&c->pending_commit.personal_native;a=&n->scm_registration;a->self=a;a->original=v;a->captured=*v;
 a->thread=root.owner_thread;a->invocation=root.invocation;a->phase=1U;
 a->assurance=PAPACC_SCM_ASSURANCE_LIMITED_R0_R9; /* private exact presenter scope, not a caller grant */
 a->resources.sink=out;
 a->progress_stage=SCM_D_ENTRY;
 if(scm_register_observe(a,PAPACC_SCM_RESOURCE_ENTRY))goto done;
 /* Known local arrays only, not a compiler/CRT/provider/kernel stack ceiling.
  * Unknown operational capacity is still denied by the SCM admission gate. */
 root.reserved_bytes=PAPACC_C8_KNOWN_MANAGED_BYTES+papacc_server_service_storage_bytes()+sizeof(LONG)+
 sizeof(expected_path)+sizeof(parent)+sizeof(text)+sizeof(reply)+sizeof(PACC_STORAGE_SHA256)+1024U+2U*sizeof(BY_HANDLE_FILE_INFORMATION)+64U+
 SCM_RESOURCE_KNOWN_AUTOMATIC_BYTES+sizeof(PAPACC_SCM_RESOURCE_REPORT)+
 /* Sum new DATA resolver/relative-open/dependency/status/report frames,
  * including mutually exclusive frames, without treating ABI/CRT as bounded. */
 sizeof(PAPACC_PERSONAL_INSTALL_PATHS)+MAX_PATH*sizeof(WCHAR)+sizeof(OBJECT_ATTRIBUTES)+sizeof(UNICODE_STRING)+
 sizeof(BY_HANDLE_FILE_INFORMATION)+64U+sizeof(SERVICE_STATUS_PROCESS)+15U*sizeof(void *)+15U*sizeof(DWORD)+2U*sizeof(size_t)+sizeof(int)+4U*sizeof(LARGE_INTEGER)+4U*sizeof(DWORD)+sizeof(PAPACC_RESULT)+sizeof(NTSTATUS)+sizeof(DWORD)+
 /* Additional named current/dependency locals and diagnostic helper arguments;
  * conservative sum, not a claim to bound the ABI/stdio/native stack. */
 sizeof(PAPACC_RESULT)+sizeof(void *)+sizeof(DWORD)+3U*sizeof(void *)+4U*sizeof(DWORD);
 a->progress_stage=SCM_D_RESERVATION;
 if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT){(void)scm_register_note(a,SCM_D_RESERVATION,SCM_DIAG_POLICY,0,0);goto done;}
 if(papacc_server_console_win32_stop_requested()){(void)scm_register_note(a,SCM_D_STOP,SCM_DIAG_POLICY,0,0);goto done;}
 a->progress_stage=SCM_D_LAYOUT_PREPARE;checked=papacc_configuration_personal_layout_binary(c);
 if(checked!=PAPACC_RESULT_OK){(void)scm_register_note(a,SCM_D_LAYOUT_PREPARE,SCM_DIAG_RESULT,(DWORD)checked,0);goto done;}
 /* No staging/adoption: exact known-folder target must already be protected.
  * This observes the original image beneath that directory, not a fixture path. */
 a->progress_stage=SCM_D_TARGET_PATH;
 z=wcslen(n->install_paths.binary);if(z+sizeof(L"\\papacc_server.exe")/sizeof(WCHAR)>MAX_PATH)goto done;
 memcpy(expected_path,n->install_paths.binary,z*sizeof(WCHAR));memcpy(expected_path+z,L"\\papacc_server.exe",sizeof(L"\\papacc_server.exe"));
 if(_wcsicmp(expected_path,v->image.path)){(void)scm_register_note(a,SCM_D_TARGET_PATH,SCM_DIAG_POLICY,0,0);goto done;}
 if(!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,n->install_paths.binary,-1,parent,sizeof(parent),NULL,NULL)){(void)scm_register_note(a,SCM_D_TARGET_PATH,SCM_DIAG_WIN32,GetLastError(),0);goto done;}
 checked=papacc_configuration_personal_native_open(c,parent);
 if(checked!=PAPACC_RESULT_OK){
  if(!a->diagnostic_stage){a->diagnostic_native_stage=n->diagnostic_first;a->diagnostic_native_kind=n->diagnostic_kind;a->diagnostic_native_error=n->diagnostic_error;}
  (void)scm_register_note(a,SCM_D_PARENT_OPEN,SCM_DIAG_RESULT,(DWORD)checked,0);goto done;
 }
 if(!scm_descriptor(n->descriptor,n->sid)){(void)scm_register_note(a,SCM_D_PARENT_SD,SCM_DIAG_HELPER_UNKNOWN,0,0);goto done;}
 checked=papacc_configuration_scm_registration_image_open(c,L"papacc_server.exe");
 if(checked!=PAPACC_RESULT_OK){(void)scm_register_note(a,SCM_D_EXE_OPEN,SCM_DIAG_RESULT,(DWORD)checked,0);goto done;}
 if(!scm_acl_hash(n,n->scm_image,a->image_acl)){(void)scm_register_note(a,SCM_D_EXE_SD,SCM_DIAG_HELPER_UNKNOWN,0,0);goto done;}
 for(i=0;i<2U;i++){
  checked=papacc_configuration_scm_registration_dependency_open(c,(unsigned)i);
  if(checked!=PAPACC_RESULT_OK){(void)scm_register_note(a,SCM_D_DEPENDENCY_OPEN,SCM_DIAG_RESULT,(DWORD)checked,(DWORD)i+1U);goto done;}
  if(!scm_register_dependency(c,(unsigned)i,1))goto done;
 }
 a->package_captured=1U;
 a->progress_stage=SCM_D_ORIGINALS_SAMPLE;
 if(scm_register_observe(a,PAPACC_SCM_RESOURCE_ORIGINALS))goto done;
 memcpy(a->path,v->image.path,(wcslen(v->image.path)+1U)*sizeof(WCHAR));memcpy(a->command,v->command,(wcslen(v->command)+1U)*sizeof(WCHAR));memcpy(a->digest,v->image.sha256,32);
 rng_status=BCryptGenRandom(NULL,a->attempt,32,BCRYPT_USE_SYSTEM_PREFERRED_RNG);
 if(rng_status<0){(void)scm_register_note(a,SCM_D_RNG_API,SCM_DIAG_NTSTATUS,(DWORD)rng_status,0);goto done;}
 {BYTE nonzero=0;for(i=0;i<32;i++)nonzero|=a->attempt[i];if(!nonzero){(void)scm_register_note(a,SCM_D_RNG_ZERO,SCM_DIAG_POLICY,0,0);goto done;}}
 a->progress_stage=SCM_D_OWNER;if(scm_register_current(v,1U))goto done;
 if(!WriteConsoleA(output,first_plan,(DWORD)(sizeof(first_plan)-1U),&output_written,NULL)){
  (void)scm_register_note(a,SCM_D_PLAN_FIRST_WRITE,SCM_DIAG_WIN32,GetLastError(),0);goto done;}
 if(output_written!=sizeof(first_plan)-1U){(void)scm_register_note(a,SCM_D_PLAN_FIRST_WRITE,SCM_DIAG_SHORT_OUTPUT,output_written,0);goto done;}
 a->progress_stage=SCM_D_PLAN_OUTPUT;
 if(!personal_console_write(output,n->sid_text)||
 !personal_console_write(output,"\nService: PapinhoAccelerator\nAccount: NT SERVICE\\PapinhoAccelerator\nStart: DEMAND_START (3)\nService SID type: UNRESTRICTED (1)\nOriginal image: "))goto done;
 if(!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,a->path,-1,text,sizeof(text),NULL,NULL)||!personal_console_token(output,text)||
 !personal_console_write(output,"\nExact diagnostic command: ")||!WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,a->command,-1,text,sizeof(text),NULL,NULL)||!personal_console_token(output,text))goto done;
 sprintf_s(text,sizeof(text),"\nLIMITED_R0_R9: host physical/commit floor=%llu; controlled reservation=%llu; opaque RPC/kernel maxima UNKNOWN; no peak/latency guarantee; partial registration retained; NO START/NO PRODUCTIVE PROMOTION.\n",
  (unsigned long long)PAPACC_LAUNCHER_MANAGED_LIMIT,(unsigned long long)root.reserved_bytes);
 if(!personal_console_write(output,text))goto done;
 sprintf_s(text,sizeof(text),"\nProtected binary target identity: %lu:%lu:%lu\nOriginal image identity: %lu:%lu:%lu\nImage SHA256: ",n->parent.dwVolumeSerialNumber,n->parent.nFileIndexHigh,n->parent.nFileIndexLow,v->image.identity.dwVolumeSerialNumber,v->image.identity.nFileIndexHigh,v->image.identity.nFileIndexLow);
 if(!personal_console_write(output,text))goto done;
 for(i=0;i<32;i++){text[i*2U]=hex[a->digest[i]>>4];text[i*2U+1U]=hex[a->digest[i]&15U];}text[64]=0;
 if(!personal_console_write(output,text)||!personal_console_write(output,"\nPurpose: register this service once and configure/read back this SID type. No Start, endpoint, parent creation, CA, CURRENT or network.\nPolicy: M5/P3 + virtual SCM; local attributed mechanics F1-F6.\nDeclare legitimate ownership/administration and confirm ONLY the registration plan. Existing service/conflict or uncertainty denies; no retry/deletion.\nQuantitative and separate operational gates remain required. Esc cancels.\n"))goto done;
 for(i=0;i<2U;i++){
  unsigned j;const char *leaf=i?"libcrypto-3-x64.dll":"libssl-3-x64.dll";
  sprintf_s(text,sizeof(text),"Dependency: %s identity=%lu:%lu:%lu bytes=%llu sha256=",leaf,
   a->dependencies[i].identity.dwVolumeSerialNumber,a->dependencies[i].identity.nFileIndexHigh,a->dependencies[i].identity.nFileIndexLow,
   ((unsigned long long)a->dependencies[i].identity.nFileSizeHigh<<32)|a->dependencies[i].identity.nFileSizeLow);
  if(!personal_console_write(output,text))goto done;
  for(j=0;j<32U;j++){text[j*2U]=hex[a->dependencies[i].digest[j]>>4];text[j*2U+1U]=hex[a->dependencies[i].digest[j]&15U];}text[64]=0;
  if(!personal_console_write(output,text)||!personal_console_write(output,"\n"))goto done;
 }
 strcpy_s(reply,sizeof(reply),"OWNER_SCM_REGISTER ");z=strlen(reply);for(i=0;i<32;i++){reply[z+i*2U]=hex[a->attempt[i]>>4];reply[z+i*2U+1U]=hex[a->attempt[i]&15U];}reply[z+64U]=0;
 a->phase=2U;
 a->progress_stage=SCM_D_CONFIRMATION;
 if(!personal_console_write(output,reply)||!personal_console_write(output,"\n")||!personal_console_confirm(input,output,reply)||scm_register_current(v,2U))goto done;
 a->progress_stage=SCM_D_CONFIRMED_SAMPLE;if(scm_register_observe(a,PAPACC_SCM_RESOURCE_CONFIRMED))goto done;
 a->phase=3U;ready=1;
 fprintf(out,"SCM_REGISTER_ACT=ATTRIBUTED_CONFIRMED; SOURCE=NATIVE_REFERENCES; CONSUMED=NO; SCM_EFFECT=NONE; OPERATIONAL_ADMISSION=SEPARATE\n");
done:
 if(!ready){
  (void)scm_register_note(a,a->progress_stage,SCM_DIAG_HELPER_UNKNOWN,0,a->progress_object);
  scm_register_diagnostic_emit(a,err); /* before close can change OS LastError */
  (void)scm_register_fail(a,ERROR_ACCESS_DENIED);(void)papacc_server_launcher_scm_registration_finish(v,1);
 }
 fprintf(err,"SCM_REGISTER_PRESENTER=%s; PHASE=%lu; AUTHORITY_NOT_IMPLIED_BY_METADATA\n",ready?"CONFIRMED":"DENY_OR_CANCELLED",(unsigned long)a->phase);
 return ready?0:2;
}
