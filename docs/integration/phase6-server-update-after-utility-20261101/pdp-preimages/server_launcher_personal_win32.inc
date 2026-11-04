/* Existing launcher root owns this synchronous preflight and all retained
 * resources. FCA preparation is explicit; never commits or publishes. */
#include "config_source.h"
#include <bcrypt.h>
#include "personal_confirmation_console_win32.inc"
#include "personal_console_token_win32.inc"
static PAPACC_SIZE personal_present_base_known_bytes(void)
{
 PAPACC_SIZE bytes=PAPACC_C8_KNOWN_MANAGED_BYTES+sizeof(PC_DOCUMENT)+sizeof(PC_DIAGNOSTIC)+112U+80U+MAX_PATH*4U+32U+16U+16U+SECURITY_MAX_SID_SIZE+16384U;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 bytes+=sizeof(PS_ADMISSION);
#endif
 return bytes;
}
PAPACC_SIZE papacc_server_launcher_first_prepare_known_bytes(void)
{return personal_present_base_known_bytes()+PAPACC_FIRST_PREPARE_DYNAMIC_BYTES;}
static int personal_present(const char *source,const char *parent,const char *leaf,const PAPACC_U8 *authority,const PAPACC_U8 *generation,const PAPACC_U8 *service,DWORD service_bytes,FILE *err,int preparation)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 PC_DOCUMENT document;PC_DIAGNOSTIC diagnostic;PC_TARGET target={1U,0};
 PAPACC_PERSONAL_CUSTODY_DATA custody;
 HANDLE input=GetStdHandle(STD_INPUT_HANDLE),output=GetStdHandle(STD_OUTPUT_HANDLE);DWORD mode,count;
 LONG old;unsigned i,j;char expected[112],number[80],binary_utf8[MAX_PATH*4];unsigned char id[32],requested_authority[16],requested_generation[16],requested_service[SECURITY_MAX_SID_SIZE];int clean=1,confirmed=0;size_t bytes,len;
 static const char hex[]="0123456789abcdef";
 if(!source||!parent||!leaf||!err)return 1;
 if((authority||generation||service||service_bytes)&&(!authority||!generation||!service||!service_bytes||service_bytes>SECURITY_MAX_SID_SIZE||!IsValidSid((PSID)service)||GetLengthSid((PSID)service)!=service_bytes))return 1;
 if(authority){memcpy(requested_authority,authority,16);memcpy(requested_generation,generation,16);memcpy(requested_service,service,service_bytes);authority=requested_authority;generation=requested_generation;service=requested_service;}
 /* No acquisition when redirected; neither argv nor piped stdin confirms. */
 if(!GetConsoleMode(input,&mode)||!GetConsoleMode(output,&mode)){
 fputs("FIRST_INSTALL=NO_INTERACTIVE_CONSOLE; OPERATOR=UNBOUND; AUTHORIZATION=ABSENT; TARGET=NATIVE_UNBOUND; CUSTODY=NOT_ACQUIRED; BOOTSTRAP=DENY\n",err);return 2;}
 old=InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,0);
 if(old!=0){fputs("FIRST_INSTALL=ROOT_NOT_EMPTY; BOOTSTRAP=DENY\n",err);return 2;}
 if(next_invocation==(PAPACC_U64)-1){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return 2;}
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(papacc_server_service_admission_enter(NULL,NULL)!=ERROR_SUCCESS){InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);return 2;}
#endif
 memset(&root,0,sizeof(root));root.invocation=++next_invocation;root.owner_thread=GetCurrentThreadId();
 c=&root.configuration_commit;p=&c->pending_commit;n=&p->personal_native;c->writer=root.owner_thread;p->file=INVALID_HANDLE_VALUE;
 root.reserved_bytes=preparation?papacc_server_launcher_first_prepare_known_bytes():personal_present_base_known_bytes();
 if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT)goto done;
 if(preparation&&papacc_ca_key_first_budget_begin(&root.first_preparation_key,&p->stop))goto done;
 if(papacc_server_console_win32_stop_requested()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK)goto done;
 if(!preparation&&papacc_configuration_personal_native_open(c,parent)!=PAPACC_RESULT_OK)goto done;
 if(papacc_configuration_personal_layout_new(c)!=PAPACC_RESULT_OK||
 !WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,n->install_paths.binary,-1,binary_utf8,sizeof(binary_utf8),NULL,NULL))goto done;
 if(preparation){
  if(papacc_configuration_personal_first_open(c,&root.first_preparation_key,parent,source)!=PAPACC_RESULT_OK)goto done;
  authority=n->first_authority;generation=n->first_generation;service=n->first_service;service_bytes=n->first_service_bytes;
  if(wcscmp(n->install_paths.leaf,L"Accelerator")||strcmp(leaf,"Accelerator"))goto done;
 }
 len=strlen(leaf);if(!len||len>64U||!strcmp(leaf,".")||!strcmp(leaf,"..")||leaf[len-1U]=='.')goto done;
 for(i=0;i<len;i++)if(!((leaf[i]>='a'&&leaf[i]<='z')||(leaf[i]>='A'&&leaf[i]<='Z')||(leaf[i]>='0'&&leaf[i]<='9')||leaf[i]=='-'||leaf[i]=='_'||leaf[i]=='.'))goto done;
 memcpy(p->previous,leaf,len+1U);memcpy(n->plan_leaf,leaf,len+1U);
 if(!preparation)p->file=CreateFileA(source,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(p->file==INVALID_HANDLE_VALUE||!GetFileInformationByHandle(p->file,&n->source)||(n->source.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||n->source.nFileSizeHigh||n->source.nFileSizeLow>PC_SOURCE_MAX)goto done;
 if(!ReadFile(p->file,p->next,PC_SOURCE_MAX+1U,&count,NULL)||!count||count>PC_SOURCE_MAX||count!=n->source.nFileSizeLow)goto done;bytes=count;
 if(pc_parse(p->next,bytes,&document,&diagnostic)!=PC_OK||pc_validate(&document,&target,&diagnostic)!=PC_OK)goto done;
 if((preparation&&papacc_ca_key_first_budget_check(&root.first_preparation_key,&p->stop))||BCryptGenRandom(NULL,id,sizeof(id),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0)goto done;
 if(!papacc_personal_data_capture(&p->personal_data,id,p->next,bytes))goto done;
 if(authority){if(EqualSid(n->sid,(PSID)service)||papacc_configuration_personal_effect_plan_data(c,authority,generation,service,service_bytes)!=PAPACC_RESULT_OK)goto done;}
 memcpy(n->presenter_attempt,id,32);n->presenter_ca=authority?1U:0U;n->presenter_phase=1U;
 if(!personal_console_write(output,"\nNEW_INSTALL - PERSONAL_LOCAL\nOperator SID: ")||!personal_console_write(output,n->sid_text)||
 !personal_console_write(output,"\nBinary destination: ")||!personal_console_token(output,binary_utf8)||
 !personal_console_write(output,"\nProtected parent: ")||!personal_console_token(output,p->path)||!personal_console_write(output,"\nNew relative container: ")||!personal_console_token(output,(const char *)p->previous))goto done;
 sprintf_s(number,sizeof(number),"\nParent native identity: %lu:%lu:%lu\nSource native identity: ",n->parent.dwVolumeSerialNumber,n->parent.nFileIndexHigh,n->parent.nFileIndexLow);
 if(!personal_console_write(output,number))goto done;
 sprintf_s(number,sizeof(number),"%lu:%lu:%lu\nExact configuration (escaped tokens):\n",n->source.dwVolumeSerialNumber,n->source.nFileIndexHigh,n->source.nFileIndexLow);
 if(!personal_console_write(output,number))goto done;
 for(i=0;i<document.line_count;i++){
 for(j=0;j<document.lines[i].count;j++)if(!personal_console_token(output,document.lines[i].tokens[j].text)||!personal_console_write(output," "))goto done;
 if(!personal_console_write(output,"\n"))goto done;}
 if(authority){
  if(!personal_console_write(output,"Joint purpose: NEW_INSTALL + CA_INITIALIZE. Requested CA IDs and operational SID below are candidates, not observed custody. Exact escaped plan bytes:\n"))goto done;
  for(i=0;i<p->personal_data.bytes;i++){char escaped[5];unsigned char ch=p->personal_data.plan[i];
   escaped[0]='\\';escaped[1]='x';escaped[2]=hex[ch>>4];escaped[3]=hex[ch&15U];escaped[4]=0;if(!personal_console_write(output,escaped))goto done;}
  if(!personal_console_write(output,preparation?"\nFCA PREPARATION ONLY: one exclusive protected container and one MACHINE RSA2048 nonexportable signing-only CA key. No signature/certificate/CSR/CURRENT/network/Start. Virtual account is candidate identity, not a proven runtime token. Opaque costs remain UNKNOWN; partial persistent objects may remain; no retry/delete/rollback. entry128/guard188/cap192, private1994944/working3145728, physical7239172096/commit3221225472, freshness100ms/window240000/global60000 without renewal.\n":"\nRequested MACHINE KSP model; custody NOT_ACQUIRED; distinct runtime identity; private export forbidden. No CA or CURRENT is executed by this presenter.\n"))goto done;
 }
 strcpy_s(expected,sizeof(expected),authority?"OWNER_NEW_INSTALL_CA_INITIALIZE ":"OWNER_NEW_INSTALL ");len=strlen(expected);
 for(i=0;i<32U;i++){expected[len+i*2U]=hex[id[i]>>4];expected[len+i*2U+1U]=hex[id[i]&15U];}expected[len+64U]=0;
 if(!personal_console_write(output,preparation?"Policy: accepted personal F0-F9 + FCA0-FCA7 / 2026-11-03. Declare legitimate ownership/administration and a NEW installation, not restore. Confirm explicitly BOTH NEW_INSTALL and CA_INITIALIZE preparation as displayed, including residual risk. This is not production authority. Esc cancels. Type the ENTIRE line; input is visible; no passwords.\n":"Policy: personal F0-F9 / 2026-10-22.\nDeclare legitimate ownership/administration and a NEW installation, not restore.\nPersistent bootstrap effects, local CA identity and nonexportable custody require their separate gates.\nA key/certificate/signature/CURRENT requires separate native admission; none is executed here.\nNative/material/CA/ED11 admission remains DENY. Esc cancels.\nType the ENTIRE line below, then Enter. Input is visible; do not type passwords.\nDeclaration for THIS displayed attempt only:\n")||!personal_console_write(output,expected)||!personal_console_write(output,"\n"))goto done;
 if(!papacc_personal_data_present(&p->personal_data)||!personal_console_confirm(input,output,expected)||papacc_server_console_win32_stop_requested())goto done;
 if(papacc_configuration_personal_layout_current(c)!=PAPACC_RESULT_OK||papacc_configuration_personal_native_current(c)!=PAPACC_RESULT_OK||strcmp((char *)p->previous,leaf)||strcmp(p->path,parent))goto done;
 if(papacc_configuration_personal_source_current(c)!=PAPACC_RESULT_OK)goto done;
 if(papacc_server_console_win32_stop_requested()||!(authority?papacc_personal_effect_confirm(&p->personal_data,id,p->previous+1024U,p->personal_data.bytes,expected,strlen(expected)):papacc_personal_data_confirm(&p->personal_data,id,p->next,bytes)))goto done;
 /* Historical presenter remains DATA-only. FCA separately proves originals
  * and its bounded preparation scope before dispatch; no generic grant. */
 if(preparation){n->presenter_phase=2U;if(papacc_configuration_personal_first_confirm(c,id)!=PAPACC_RESULT_OK)goto done;}
 if(papacc_server_console_win32_stop_requested()||(preparation?
  papacc_configuration_personal_target_begin(c,id,root.reserved_bytes):papacc_configuration_personal_handoff_begin(c,id,root.reserved_bytes))!=PAPACC_RESULT_OK)goto done;
 while(p->active){
  if(papacc_server_console_win32_stop_requested()){InterlockedExchange(&p->stop,1);break;}
  if(preparation&&GetTickCount64()>=root.first_preparation_key.diagnostic_budget.until){InterlockedExchange(&p->stop,1);break;}
  if(papacc_configuration_personal_handoff_poll(c)!=PAPACC_RESULT_OK)break;
  if(p->active)Sleep(10U);
 }
 if(!p->active&&!p->thread&&n->handoff_state==PAPACC_PERSONAL_HANDOFF_VERIFIED&&!papacc_server_console_win32_stop_requested()){confirmed=1;n->presenter_phase=2U;}
 if(preparation&&confirmed){
  if(papacc_configuration_personal_ca_initialize(c,&root.first_preparation_key,id,n->first_authority,n->first_generation,n->first_service,n->first_service_bytes,root.reserved_bytes)!=ERROR_SUCCESS){confirmed=0;goto done;}
  n->first_phase=5U;
 }
done:
 if(preparation&&(p->active||p->thread)){
  /* Same retained-process containment as existing SCQ. Never read mutable
   * worker results or let process exit dispose a potentially live backing. */
  InterlockedExchange(&p->stop,1);papacc_personal_data_invalidate(&p->personal_data);InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);
  fputs("FCA_PREPARATION=RETAINED_PENDING; WORKER_OR_CLOSE=UNKNOWN; BACKING_PRESERVED; NO_RETRY_NO_EXIT_DISPOSAL_NO_STOP_NO_KILL_NO_NEW_EFFECT; HISTORICAL_RETENTION_OS_QUIESCENCE_UNKNOWN\n",err);
  fflush(err);Sleep(INFINITE);return 2;
 }
 if(n->presenter_phase==1U)n->presenter_phase=3U;
 papacc_personal_data_invalidate(&p->personal_data);
 if(preparation){
  if(p->active||p->thread)clean=0;
  else if(root.first_preparation_key.thread&&root.first_preparation_key.state!=PAPACC_CA_KEY_CLOSED&&papacc_ca_key_close(&root.first_preparation_key)!=ERROR_SUCCESS)clean=0;
 }
 if(preparation){
  /* Keep the cooperative Stop handler live during uncertain retention.
   * Only a factually closed cohort may release its console registration. */
  if(papacc_configuration_personal_native_close(c)!=PAPACC_RESULT_OK)clean=0;
  if(clean&&n->console.is_installed&&papacc_server_console_win32_uninstall(&n->console)!=PAPACC_RESULT_OK)clean=0;
 }else{
  if(n->console.is_installed&&papacc_server_console_win32_uninstall(&n->console)!=PAPACC_RESULT_OK)clean=0;
  if(papacc_configuration_personal_native_close(c)!=PAPACC_RESULT_OK)clean=0;
 }
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(clean){if(papacc_server_service_admission_finish(1)!=ERROR_SUCCESS)clean=0;}
 if(!clean)papacc_server_service_admission_retain();
#endif
 if(clean){(void)papacc_personal_data_retire(&p->personal_data);SecureZeroMemory(p->next,sizeof(p->next));SecureZeroMemory(p->observed,sizeof(p->observed));InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);}
 else {n->presenter_phase=4U;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);}
 /* Historical presenters still never acquire a key. FCA reports only its
  * own original results, never production admission or a transferable grant. */
 if(n->presenter_phase){for(i=0;i<32U;i++){expected[i*2U]=hex[n->presenter_attempt[i]>>4];expected[i*2U+1U]=hex[n->presenter_attempt[i]&15U];}expected[64]=0;
 fprintf(err,"PRESENTER_RECEIPT purpose=%s phase=%u attempt=%s; DECLARATION=ATTRIBUTED; OPERATIONAL_AUTHORITY=DENY\n",n->presenter_ca?"NEW_INSTALL_CA_INITIALIZE":"NEW_INSTALL",n->presenter_phase,expected);}
 custody=papacc_configuration_personal_custody_data(c,NULL);
 fprintf(err,"CUSTODY_METADATA_ONLY: target=%u ca=%u effect_denied=%u\n",custody.target,custody.ca,custody.effect_denied);
 if(preparation){
  fprintf(err,"FCA_LAST_CAPTURE parent_stage=%lu parent_kind=%lu parent_error=0x%08lx chain_first=%lu chain_open=0x%08lx sd_calls=%lu sd_status=0x%08lx child_open=0x%08lx io_snapshot_emitted=%u child_information=%llu budget_first=%lu sample_stage=%lu; LAST_CAPTURES_NOT_FIRST_CAUSE_PROOF; INITIALIZED_FIELDS_NOT_PROOF; NO_NEW_QUERY_NO_AUTHORITY\n",n->diagnostic_first,n->diagnostic_kind,n->diagnostic_error,n->scq_inputs.image_chain.first,(DWORD)n->scq_inputs.image_chain.open_status,n->scq_inputs.image_chain.native_sd.calls,n->scq_inputs.image_chain.native_sd.status,(DWORD)n->scq_cold_status,n->phase==2U?0U:1U,n->phase==2U?0ULL:(unsigned long long)n->scq_cold_io.Information,root.first_preparation_key.diagnostic_budget.first,root.first_preparation_key.diagnostic_budget.observation.error_stage);
  fprintf(err,"FCA_PREPARATION prepared=%u container_created=%u container_identity=%lu:%lu:%lu key_created=%u key_name=%ls first=%lu key_error=0x%08lx close_known=%u; CREATED_FLAGS_NOT_ABSENCE_PROOF; NO_RETRY_NO_DELETE_NO_SIGN_NO_CURRENT_NO_START; READINESS=DENY; HISTORICAL_RETENTION_OS_QUIESCENCE_UNKNOWN\n",confirmed?1U:0U,p->bootstrap_created,p->bootstrap_volume,p->bootstrap_high,p->bootstrap_low,root.first_preparation_key.created?1U:0U,root.first_preparation_key.name,n->first_error,(unsigned long)p->ca_initialize_first,clean?1U:0U);
  if(!clean){fflush(err);Sleep(INFINITE);}return confirmed&&clean?0:2;
 }
 fprintf(err,"FIRST_INSTALL=%s; RESOURCES=%s; CA_CUSTODY=ABSENT; ED11=PARTIAL; BOOTSTRAP=DENY; CURRENT_PUBLICATION=DENY\n",confirmed?"EXACT_CONFIRMATION_DATA_ONLY":"DENY_OR_CANCELLED",clean?"CLOSED":"RETAINED_UNKNOWN");return 2;
}

int papacc_server_launcher_personal_first_install(const char *source,const char *parent,const char *leaf,FILE *err)
{return personal_present(source,parent,leaf,NULL,NULL,NULL,0,err,0);}
int papacc_server_launcher_personal_ca_present(const char *source,const char *parent,const char *leaf,const PAPACC_U8 authority[16],const PAPACC_U8 generation[16],const PAPACC_U8 *service,DWORD service_bytes,FILE *err)
{if(!authority||!generation||!service)return 1;return personal_present(source,parent,leaf,authority,generation,service,service_bytes,err,0);}
int papacc_server_launcher_personal_first_prepare(const char *source,const char *parent,const char *leaf,FILE *err)
{
#if defined(PAPACC_ED11_WINDOWS_3MIB) && defined(_M_X64)
 return personal_present(source,parent,leaf,NULL,NULL,NULL,0,err,1);
#else
 (void)source;(void)parent;(void)leaf;fputs("FCA_PREPARATION=UNSUPPORTED_PROFILE; NO_EFFECT\n",err);return 2;
#endif
}
