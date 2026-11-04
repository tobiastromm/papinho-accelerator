#include "server_launcher_owner_win32.h"
#include "server_c8_private_storage_layout_win32.h"
#include <sddl.h>
#include <stdio.h>
#include <string.h>
/* Test-only console/native references. No token grant, child, key or CURRENT. */
static PAPACC_SERVER_LAUNCHER_OWNER_WIN32 root;
static volatile LONG state;
static PAPACC_U64 next_invocation;
static unsigned scenario,reads,checks,writes;
static char transcript[65536],answer[112];static size_t answer_index;
static PSID operator_sid,service_sid;
static BOOL WINAPI fake_mode(HANDLE h,LPDWORD mode){(void)h;*mode=0;return scenario!=7;}
static BOOL WINAPI fake_write(HANDLE h,const void *b,DWORD n,LPDWORD used,LPVOID unused)
{size_t z=strlen(transcript);const char *s=b;(void)h;(void)unused;if(scenario==9)return FALSE;if(z+n>=sizeof(transcript))return FALSE;memcpy(transcript+z,s,n);transcript[z+n]=0;*used=n;writes++;
 if(n>18&&!memcmp(s,"OWNER_NEW_INSTALL",17)){memcpy(answer,s,n);answer[n]=0;if(scenario==10)answer[0]='X';}return TRUE;}
static DWORD WINAPI fake_wait(HANDLE h,DWORD ms){(void)h;(void)ms;return WAIT_OBJECT_0;}
static BOOL WINAPI fake_read(HANDLE h,PINPUT_RECORD event,DWORD cap,LPDWORD count)
{(void)h;(void)cap;memset(event,0,sizeof(*event));*count=1;event->EventType=KEY_EVENT;event->Event.KeyEvent.bKeyDown=TRUE;event->Event.KeyEvent.wRepeatCount=1;reads++;
 if(scenario==1){event->Event.KeyEvent.wVirtualKeyCode=VK_ESCAPE;return TRUE;}
 if(answer[answer_index])event->Event.KeyEvent.uChar.AsciiChar=answer[answer_index++];else event->Event.KeyEvent.wVirtualKeyCode=VK_RETURN;
 if(scenario==3&&event->Event.KeyEvent.wVirtualKeyCode==VK_RETURN)root.configuration_commit.pending_commit.next[0]^=1;
 return TRUE;}
static PAPACC_BOOL fake_stop(void){return scenario==6&&reads?PAPACC_TRUE:PAPACC_FALSE;}
static PAPACC_RESULT fake_install(PAPACC_SERVER_CONSOLE_WIN32 *c){c->is_installed=PAPACC_TRUE;return 0;}
static PAPACC_RESULT fake_uninstall(PAPACC_SERVER_CONSOLE_WIN32 *c){c->is_installed=PAPACC_FALSE;return 0;}
static PAPACC_RESULT fake_open(PAPACC_CONFIGURATION_COMMIT *c,const char *parent)
{PAPACC_PERSONAL_NATIVE_CONTEXT *n=&c->pending_commit.personal_native;strcpy_s(c->pending_commit.path,sizeof(c->pending_commit.path),parent);n->phase=1;n->sid_bytes=GetLengthSid(operator_sid);memcpy(n->sid,operator_sid,n->sid_bytes);n->sid_text="TEST_OPERATOR_NOT_NATIVE_AUTHORITY";n->parent.dwVolumeSerialNumber=20;n->parent.nFileIndexHigh=30;n->parent.nFileIndexLow=40;return 0;}
static PAPACC_RESULT fake_current(PAPACC_CONFIGURATION_COMMIT *c){(void)c;return scenario==2&&reads?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
static PAPACC_RESULT fake_source(PAPACC_CONFIGURATION_COMMIT *c){return c->pending_commit.next[0]=='f'?0:PAPACC_RESULT_INVALID_STATE;}
static PAPACC_RESULT fake_effect(PAPACC_CONFIGURATION_COMMIT *c,const BYTE *a,const BYTE *g,const BYTE *sid,PAPACC_SIZE count)
{PAPACC_CONFIG_PENDING *p=&c->pending_commit;PAPACC_PERSONAL_EFFECT_SPEC spec;BYTE parent[12]={20,0,0,0,30,0,0,0,40,0,0,0};size_t n;if(scenario==4)return PAPACC_RESULT_INVALID_STATE;
 memset(&spec,0,sizeof(spec));spec.operator_id=p->personal_native.sid;spec.operator_bytes=p->personal_native.sid_bytes;spec.parent_id=parent;spec.leaf=(BYTE *)p->previous;spec.leaf_bytes=strlen((const char *)p->previous);spec.authority=a;spec.generation=g;spec.service_id=sid;spec.service_bytes=count;spec.configuration=p->next;spec.configuration_bytes=p->personal_data.bytes;
 n=papacc_personal_effect_build(&spec,p->previous+1024,PAPACC_PERSONAL_PLAN_MAX);return n&&papacc_personal_effect_upgrade(&p->personal_data,p->previous+1024,n)?0:PAPACC_RESULT_INVALID_STATE;}
static PAPACC_RESULT fake_begin(PAPACC_CONFIGURATION_COMMIT *c,const BYTE *id,PAPACC_SIZE reserve)
{PAPACC_CONFIG_PENDING *p=&c->pending_commit;(void)reserve;if(!(p->personal_data.effectful?papacc_personal_effect_consume(&p->personal_data,id,p->previous+1024,p->personal_data.bytes):papacc_personal_data_consume(&p->personal_data,id,p->next,p->personal_data.bytes)))return PAPACC_RESULT_INVALID_STATE;
 if(scenario==11){p->active=1;p->thread=(HANDLE)(ULONG_PTR)1;return 0;}p->personal_native.handoff_state=PAPACC_PERSONAL_HANDOFF_VERIFIED;return 0;}
static PAPACC_RESULT fake_poll(PAPACC_CONFIGURATION_COMMIT *c){(void)c;return PAPACC_RESULT_INVALID_STATE;}
static PAPACC_RESULT fake_close(PAPACC_CONFIGURATION_COMMIT *c)
{PAPACC_CONFIG_PENDING *p=&c->pending_commit;if(scenario==5||p->active)return PAPACC_RESULT_INVALID_STATE;if(p->file&&p->file!=INVALID_HANDLE_VALUE){if(!CloseHandle(p->file))return PAPACC_RESULT_INVALID_STATE;p->file=INVALID_HANDLE_VALUE;}p->personal_native.phase=3;return 0;}
#define GetConsoleMode fake_mode
#define WriteConsoleA fake_write
#define ReadConsoleInputA fake_read
#define WaitForSingleObject fake_wait
#define papacc_server_console_win32_stop_requested fake_stop
#define papacc_server_console_win32_install fake_install
#define papacc_server_console_win32_uninstall fake_uninstall
#define papacc_configuration_personal_native_open fake_open
#define papacc_configuration_personal_native_current fake_current
#define papacc_configuration_personal_source_current fake_source
#define papacc_configuration_personal_effect_plan_data fake_effect
#define papacc_configuration_personal_handoff_begin fake_begin
#define papacc_configuration_personal_handoff_poll fake_poll
#define papacc_configuration_personal_native_close fake_close
#define papacc_server_launcher_personal_first_install fixture_first
#define papacc_server_launcher_personal_ca_present fixture_ca_present
#include "../apps/server/server_launcher_personal_win32.inc"
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL scenario=%u line=%u phase=%u\n",scenario,(unsigned)__LINE__,root.configuration_commit.pending_commit.personal_native.presenter_phase);return 1;}}while(0)
int main(int argc,char **argv)
{BYTE a[16]={1},g[16]={2};FILE *err;unsigned i;PAPACC_CONFIG_PENDING *p=&root.configuration_commit.pending_commit;
 if(argc!=2)return 2;CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&operator_sid));CHECK(ConvertStringSidToSidA("S-1-5-21-1-2-3-1002",&service_sid));CHECK(fopen_s(&err,"NUL","w")==0);
 for(i=0;i<12;i++){
  scenario=i;state=i==8?PAPACC_LAUNCHER_ACTIVE:0;reads=writes=0;answer_index=0;transcript[0]=answer[0]=0;memset(&root,0,sizeof(root));
  CHECK(fixture_ca_present(argv[1],"C:\\TEST_PARENT_NOT_OPENED","test-child",a,g,service_sid,GetLengthSid(service_sid),err)==2);
  if(i==0){CHECK(p->personal_native.presenter_phase==2&&p->personal_native.presenter_ca==1);CHECK(strstr(transcript,"NEW_INSTALL + CA_INITIALIZE")&&strstr(transcript,"MACHINE KSP"));CHECK(p->personal_data.consumed&&!p->bootstrap_created&&!p->personal_ca);CHECK(strstr(transcript,"Binary destination:")&&p->personal_native.install_paths.phase==2&&wcsstr(p->personal_native.install_paths.binary,L"\\Papinho\\Accelerator"));}
  if(i==5||i==11)CHECK(p->personal_native.presenter_phase==4&&state==PAPACC_LAUNCHER_RETAINED);
  else if(i!=0)CHECK(p->personal_native.presenter_phase!=2);
  if(i==7||i==8)CHECK(!reads&&!writes);
  /* Release only this test's read-only source; no retained product resource. */
  if(p->file&&p->file!=INVALID_HANDLE_VALUE){CHECK(CloseHandle(p->file));p->file=INVALID_HANDLE_VALUE;}
 }
 scenario=0;state=0;reads=writes=0;answer_index=0;answer[0]=transcript[0]=0;
 CHECK(fixture_first(argv[1],"C:\\TEST_PARENT_NOT_OPENED","test-child",err)==2);CHECK(p->personal_native.presenter_phase==2&&!p->personal_native.presenter_ca);
 CHECK(fclose(err)==0);CHECK(!LocalFree(operator_sid)&&!LocalFree(service_sid));
 printf("PASS PRESENTER_ISOLATED checks=%u scenarios=13 REAL_CSPRNG_AND_CONFIG_READ TEST_CONSOLE_AND_NATIVE_CONTEXT NO_TARGET_KEY_OR_CURRENT\n",checks);return 0;
}
