/* Existing launcher root owns this synchronous preflight and all retained
 * resources. Never calls bootstrap/custody-create/CA/commit/publication. */
#include "config_source.h"
#include <bcrypt.h>
static int personal_console_write(HANDLE h,const char *s)
{DWORD n,w;size_t z=strlen(s);if(z>MAXDWORD)return 0;n=(DWORD)z;return WriteConsoleA(h,s,n,&w,NULL)&&w==n;}
static int personal_console_token(HANDLE h,const char *s)
{
 static const char hex[]="0123456789abcdef";char b[5];const unsigned char *p=(const unsigned char *)s;
 if(!personal_console_write(h,"\""))return 0;
 while(*p){if(*p>=32U&&*p<127U&&*p!='"'&&*p!='\\'){b[0]=(char)*p;b[1]=0;}
 else{b[0]='\\';b[1]='x';b[2]=hex[*p>>4];b[3]=hex[*p&15U];b[4]=0;}
 if(!personal_console_write(h,b))return 0;++p;}
 return personal_console_write(h,"\"");
}
static int personal_console_confirm(HANDLE in,HANDLE out,const char *expected)
{
 INPUT_RECORD event;DWORD count;char reply[128],text[2];size_t used=0;
 DWORD wait;unsigned repeat;unsigned char ch;
 /* ReadConsoleInput does not echo: explicitly display only this nonsecret
  * ASCII declaration. Never change shared console modes or log input. */
 if(!personal_console_write(out,"> "))return 0;
 for(;;){
 if(papacc_server_console_win32_stop_requested())return 0;
 wait=WaitForSingleObject(in,50U);if(wait==WAIT_TIMEOUT)continue;if(wait!=WAIT_OBJECT_0)return 0;
 if(!ReadConsoleInputA(in,&event,1U,&count)||count!=1U)return 0;
 if(event.EventType!=KEY_EVENT||!event.Event.KeyEvent.bKeyDown)continue;
 if(event.Event.KeyEvent.wVirtualKeyCode==VK_ESCAPE){(void)personal_console_write(out,"\r\nCONFIRMATION_CANCELLED\r\n");return 0;}
 if(event.Event.KeyEvent.wVirtualKeyCode==VK_RETURN){
  if(!personal_console_write(out,"\r\n"))return 0;
  reply[used]=0;return strcmp(reply,expected)==0;
 }
 if(event.Event.KeyEvent.wVirtualKeyCode==VK_BACK){
  for(repeat=0;repeat<event.Event.KeyEvent.wRepeatCount&&used;repeat++){
   --used;if(!personal_console_write(out,"\b \b"))return 0;
  }
  continue;
 }
 ch=(unsigned char)event.Event.KeyEvent.uChar.AsciiChar;
 if(ch<32U||ch>126U)continue;
 text[0]=(char)ch;text[1]=0;
 for(repeat=0;repeat<event.Event.KeyEvent.wRepeatCount;repeat++){
  if(used>=sizeof(reply)-1U){(void)personal_console_write(out,"\r\nCONFIRMATION_TOO_LONG_DENY\r\n");return 0;}
  reply[used++]=(char)ch;if(!personal_console_write(out,text))return 0;
 }
 }
}
int papacc_server_launcher_personal_first_install(const char *source,const char *parent,const char *leaf,FILE *err)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 PC_DOCUMENT document;PC_DIAGNOSTIC diagnostic;PC_TARGET target={1U,0};
 PAPACC_PERSONAL_CUSTODY_DATA custody;
 HANDLE input=GetStdHandle(STD_INPUT_HANDLE),output=GetStdHandle(STD_OUTPUT_HANDLE);DWORD mode,count;
 LONG old;unsigned i,j;char expected[96],number[80];unsigned char id[32];int clean=1,confirmed=0;size_t bytes,len;
 static const char hex[]="0123456789abcdef";
 if(!source||!parent||!leaf||!err)return 1;
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
 root.reserved_bytes=PAPACC_C8_KNOWN_MANAGED_BYTES+sizeof(document)+sizeof(diagnostic)+sizeof(expected)+sizeof(number)+sizeof(id)+16384U;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 root.reserved_bytes+=sizeof(PS_ADMISSION); /* one owner extension, no credit */
#endif
 if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT)goto done;
 if(papacc_server_console_win32_stop_requested()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK)goto done;
 if(papacc_configuration_personal_native_open(c,parent)!=PAPACC_RESULT_OK)goto done;
 len=strlen(leaf);if(!len||len>64U||!strcmp(leaf,".")||!strcmp(leaf,"..")||leaf[len-1U]=='.')goto done;
 for(i=0;i<len;i++)if(!((leaf[i]>='a'&&leaf[i]<='z')||(leaf[i]>='A'&&leaf[i]<='Z')||(leaf[i]>='0'&&leaf[i]<='9')||leaf[i]=='-'||leaf[i]=='_'||leaf[i]=='.'))goto done;
 memcpy(p->previous,leaf,len+1U);memcpy(n->plan_leaf,leaf,len+1U);
 p->file=CreateFileA(source,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(p->file==INVALID_HANDLE_VALUE||!GetFileInformationByHandle(p->file,&n->source)||(n->source.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))||n->source.nFileSizeHigh||n->source.nFileSizeLow>PC_SOURCE_MAX)goto done;
 if(!ReadFile(p->file,p->next,PC_SOURCE_MAX+1U,&count,NULL)||!count||count>PC_SOURCE_MAX||count!=n->source.nFileSizeLow)goto done;bytes=count;
 if(pc_parse(p->next,bytes,&document,&diagnostic)!=PC_OK||pc_validate(&document,&target,&diagnostic)!=PC_OK)goto done;
 if(BCryptGenRandom(NULL,id,sizeof(id),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0)goto done;
 if(!papacc_personal_data_capture(&p->personal_data,id,p->next,bytes))goto done;
 if(!personal_console_write(output,"\nNEW_INSTALL - PERSONAL_LOCAL\nOperator SID: ")||!personal_console_write(output,n->sid_text)||
 !personal_console_write(output,"\nProtected parent: ")||!personal_console_token(output,parent)||!personal_console_write(output,"\nNew relative container: ")||!personal_console_token(output,leaf))goto done;
 sprintf_s(number,sizeof(number),"\nParent native identity: %lu:%lu:%lu\nSource native identity: ",n->parent.dwVolumeSerialNumber,n->parent.nFileIndexHigh,n->parent.nFileIndexLow);
 if(!personal_console_write(output,number))goto done;
 sprintf_s(number,sizeof(number),"%lu:%lu:%lu\nExact configuration (escaped tokens):\n",n->source.dwVolumeSerialNumber,n->source.nFileIndexHigh,n->source.nFileIndexLow);
 if(!personal_console_write(output,number))goto done;
 for(i=0;i<document.line_count;i++){
 for(j=0;j<document.lines[i].count;j++)if(!personal_console_token(output,document.lines[i].tokens[j].text)||!personal_console_write(output," "))goto done;
 if(!personal_console_write(output,"\n"))goto done;}
 strcpy_s(expected,sizeof(expected),"OWNER_NEW_INSTALL ");len=strlen(expected);
 for(i=0;i<32U;i++){expected[len+i*2U]=hex[id[i]>>4];expected[len+i*2U+1U]=hex[id[i]&15U];}expected[len+64U]=0;
 if(!personal_console_write(output,"Policy: personal F0-F9 / 2026-10-22.\nDeclare legitimate ownership/administration and a NEW installation, not restore.\nPersistent bootstrap effects, local CA identity and nonexportable custody require their separate gates.\nCA_INITIALIZE is NOT requested here. No key/certificate/signature/CURRENT will be created.\nNative/material/CA/ED11 admission remains DENY. Esc cancels.\nType the ENTIRE line below, then Enter. Input is visible; do not type passwords.\nDeclaration for THIS displayed attempt only:\n")||!personal_console_write(output,expected)||!personal_console_write(output,"\n"))goto done;
 if(!papacc_personal_data_present(&p->personal_data)||!personal_console_confirm(input,output,expected)||papacc_server_console_win32_stop_requested())goto done;
 if(papacc_configuration_personal_native_current(c)!=PAPACC_RESULT_OK||strcmp((char *)p->previous,leaf))goto done;
 if(papacc_configuration_personal_source_current(c)!=PAPACC_RESULT_OK)goto done;
 if(papacc_server_console_win32_stop_requested()||!papacc_personal_data_confirm(&p->personal_data,id,p->next,bytes))goto done;
 /* DATA consume is not operational admissibility. Source/material and CA are
  * absent, full ED11/continuity is unqualified: native creation stays denied. */
 if(papacc_server_console_win32_stop_requested()||papacc_configuration_personal_handoff_begin(c,id,root.reserved_bytes)!=PAPACC_RESULT_OK)goto done;
 while(p->active){
  if(papacc_server_console_win32_stop_requested()){InterlockedExchange(&p->stop,1);break;}
  if(papacc_configuration_personal_handoff_poll(c)!=PAPACC_RESULT_OK)break;
  if(p->active)Sleep(10U);
 }
 if(!p->active&&!p->thread&&n->handoff_state==PAPACC_PERSONAL_HANDOFF_VERIFIED&&!papacc_server_console_win32_stop_requested())confirmed=1;
done:
 papacc_personal_data_invalidate(&p->personal_data);
 if(n->console.is_installed&&papacc_server_console_win32_uninstall(&n->console)!=PAPACC_RESULT_OK)clean=0;
 if(papacc_configuration_personal_native_close(c)!=PAPACC_RESULT_OK)clean=0;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(clean){if(papacc_server_service_admission_finish(1)!=ERROR_SUCCESS)clean=0;}
 if(!clean)papacc_server_service_admission_retain();
#endif
 if(clean){(void)papacc_personal_data_retire(&p->personal_data);SecureZeroMemory(p->next,sizeof(p->next));SecureZeroMemory(p->observed,sizeof(p->observed));InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);}
 else InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);
 /* No canonical CA owner has been supplied. Never synthesize/open one. */
 custody=papacc_configuration_personal_custody_data(c,NULL);
 fprintf(err,"CUSTODY_METADATA_ONLY: target=%u ca=%u effect_denied=%u\n",custody.target,custody.ca,custody.effect_denied);
 fprintf(err,"FIRST_INSTALL=%s; RESOURCES=%s; CA_CUSTODY=ABSENT; ED11=PARTIAL; BOOTSTRAP=DENY; CURRENT_PUBLICATION=DENY\n",confirmed?"EXACT_CONFIRMATION_DATA_ONLY":"DENY_OR_CANCELLED",clean?"CLOSED":"RETAINED_UNKNOWN");return 2;
}
