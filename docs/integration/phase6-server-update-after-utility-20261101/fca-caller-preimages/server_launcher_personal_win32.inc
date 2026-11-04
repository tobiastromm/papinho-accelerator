/* Existing launcher root owns this synchronous preflight and all retained
 * resources. Never calls bootstrap/custody-create/CA/commit/publication. */
#include "config_source.h"
#include <bcrypt.h>
#include "personal_confirmation_console_win32.inc"
#include "personal_console_token_win32.inc"
static int personal_present(const char *source,const char *parent,const char *leaf,const PAPACC_U8 *authority,const PAPACC_U8 *generation,const PAPACC_U8 *service,DWORD service_bytes,FILE *err)
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
 root.reserved_bytes=PAPACC_C8_KNOWN_MANAGED_BYTES+sizeof(document)+sizeof(diagnostic)+sizeof(expected)+sizeof(number)+sizeof(binary_utf8)+sizeof(id)+sizeof(requested_authority)+sizeof(requested_generation)+sizeof(requested_service)+16384U;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 root.reserved_bytes+=sizeof(PS_ADMISSION); /* one owner extension, no credit */
#endif
 if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT)goto done;
 if(papacc_server_console_win32_stop_requested()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK)goto done;
 if(papacc_configuration_personal_native_open(c,parent)!=PAPACC_RESULT_OK)goto done;
 if(papacc_configuration_personal_layout_new(c)!=PAPACC_RESULT_OK||
 !WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,n->install_paths.binary,-1,binary_utf8,sizeof(binary_utf8),NULL,NULL))goto done;
 len=strlen(leaf);if(!len||len>64U||!strcmp(leaf,".")||!strcmp(leaf,"..")||leaf[len-1U]=='.')goto done;
 for(i=0;i<len;i++)if(!((leaf[i]>='a'&&leaf[i]<='z')||(leaf[i]>='A'&&leaf[i]<='Z')||(leaf[i]>='0'&&leaf[i]<='9')||leaf[i]=='-'||leaf[i]=='_'||leaf[i]=='.'))goto done;
 memcpy(p->previous,leaf,len+1U);memcpy(n->plan_leaf,leaf,len+1U);
 p->file=CreateFileA(source,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(p->file==INVALID_HANDLE_VALUE||!GetFileInformationByHandle(p->file,&n->source)||(n->source.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||n->source.nFileSizeHigh||n->source.nFileSizeLow>PC_SOURCE_MAX)goto done;
 if(!ReadFile(p->file,p->next,PC_SOURCE_MAX+1U,&count,NULL)||!count||count>PC_SOURCE_MAX||count!=n->source.nFileSizeLow)goto done;bytes=count;
 if(pc_parse(p->next,bytes,&document,&diagnostic)!=PC_OK||pc_validate(&document,&target,&diagnostic)!=PC_OK)goto done;
 if(BCryptGenRandom(NULL,id,sizeof(id),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0)goto done;
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
  if(!personal_console_write(output,"\nRequested MACHINE KSP model; custody NOT_ACQUIRED; distinct runtime identity; private export forbidden. No CA or CURRENT is executed by this presenter.\n"))goto done;
 }
 strcpy_s(expected,sizeof(expected),authority?"OWNER_NEW_INSTALL_CA_INITIALIZE ":"OWNER_NEW_INSTALL ");len=strlen(expected);
 for(i=0;i<32U;i++){expected[len+i*2U]=hex[id[i]>>4];expected[len+i*2U+1U]=hex[id[i]&15U];}expected[len+64U]=0;
 if(!personal_console_write(output,"Policy: personal F0-F9 / 2026-10-22.\nDeclare legitimate ownership/administration and a NEW installation, not restore.\nPersistent bootstrap effects, local CA identity and nonexportable custody require their separate gates.\nA key/certificate/signature/CURRENT requires separate native admission; none is executed here.\nNative/material/CA/ED11 admission remains DENY. Esc cancels.\nType the ENTIRE line below, then Enter. Input is visible; do not type passwords.\nDeclaration for THIS displayed attempt only:\n")||!personal_console_write(output,expected)||!personal_console_write(output,"\n"))goto done;
 if(!papacc_personal_data_present(&p->personal_data)||!personal_console_confirm(input,output,expected)||papacc_server_console_win32_stop_requested())goto done;
 if(papacc_configuration_personal_layout_current(c)!=PAPACC_RESULT_OK||papacc_configuration_personal_native_current(c)!=PAPACC_RESULT_OK||strcmp((char *)p->previous,leaf)||strcmp(p->path,parent))goto done;
 if(papacc_configuration_personal_source_current(c)!=PAPACC_RESULT_OK)goto done;
 if(papacc_server_console_win32_stop_requested()||!(authority?papacc_personal_effect_confirm(&p->personal_data,id,p->previous+1024U,p->personal_data.bytes,expected,strlen(expected)):papacc_personal_data_confirm(&p->personal_data,id,p->next,bytes)))goto done;
 /* DATA consume is not operational admissibility. Source/material and CA are
  * absent, full ED11/continuity is unqualified: native creation stays denied. */
 if(papacc_server_console_win32_stop_requested()||papacc_configuration_personal_handoff_begin(c,id,root.reserved_bytes)!=PAPACC_RESULT_OK)goto done;
 while(p->active){
  if(papacc_server_console_win32_stop_requested()){InterlockedExchange(&p->stop,1);break;}
  if(papacc_configuration_personal_handoff_poll(c)!=PAPACC_RESULT_OK)break;
  if(p->active)Sleep(10U);
 }
 if(!p->active&&!p->thread&&n->handoff_state==PAPACC_PERSONAL_HANDOFF_VERIFIED&&!papacc_server_console_win32_stop_requested()){confirmed=1;n->presenter_phase=2U;}
done:
 if(n->presenter_phase==1U)n->presenter_phase=3U;
 papacc_personal_data_invalidate(&p->personal_data);
 if(n->console.is_installed&&papacc_server_console_win32_uninstall(&n->console)!=PAPACC_RESULT_OK)clean=0;
 if(papacc_configuration_personal_native_close(c)!=PAPACC_RESULT_OK)clean=0;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(clean){if(papacc_server_service_admission_finish(1)!=ERROR_SUCCESS)clean=0;}
 if(!clean)papacc_server_service_admission_retain();
#endif
 if(clean){(void)papacc_personal_data_retire(&p->personal_data);SecureZeroMemory(p->next,sizeof(p->next));SecureZeroMemory(p->observed,sizeof(p->observed));InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);}
 else {n->presenter_phase=4U;InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);}
 /* No canonical CA owner has been supplied. Never synthesize/open one. */
 if(n->presenter_phase){for(i=0;i<32U;i++){expected[i*2U]=hex[n->presenter_attempt[i]>>4];expected[i*2U+1U]=hex[n->presenter_attempt[i]&15U];}expected[64]=0;
 fprintf(err,"PRESENTER_RECEIPT purpose=%s phase=%u attempt=%s; DECLARATION=ATTRIBUTED; OPERATIONAL_AUTHORITY=DENY\n",n->presenter_ca?"NEW_INSTALL_CA_INITIALIZE":"NEW_INSTALL",n->presenter_phase,expected);}
 custody=papacc_configuration_personal_custody_data(c,NULL);
 fprintf(err,"CUSTODY_METADATA_ONLY: target=%u ca=%u effect_denied=%u\n",custody.target,custody.ca,custody.effect_denied);
 fprintf(err,"FIRST_INSTALL=%s; RESOURCES=%s; CA_CUSTODY=ABSENT; ED11=PARTIAL; BOOTSTRAP=DENY; CURRENT_PUBLICATION=DENY\n",confirmed?"EXACT_CONFIRMATION_DATA_ONLY":"DENY_OR_CANCELLED",clean?"CLOSED":"RETAINED_UNKNOWN");return 2;
}

int papacc_server_launcher_personal_first_install(const char *source,const char *parent,const char *leaf,FILE *err)
{return personal_present(source,parent,leaf,NULL,NULL,NULL,0,err);}
int papacc_server_launcher_personal_ca_present(const char *source,const char *parent,const char *leaf,const PAPACC_U8 authority[16],const PAPACC_U8 generation[16],const PAPACC_U8 *service,DWORD service_bytes,FILE *err)
{if(!authority||!generation||!service)return 1;return personal_present(source,parent,leaf,authority,generation,service,service_bytes,err);}
