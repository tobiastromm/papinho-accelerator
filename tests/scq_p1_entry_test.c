/* Actual CLI adapter; ALL owner/native/SCM entry boundaries modeled.
 * No console, token, file, resource query, RNG, SCM or endpoint execution. */
#include "server_launcher_owner_win32.h"
#include "server_console_win32.h"
#include "personal_scq_cold_start_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_SERVER_LAUNCHER_OWNER_WIN32 fixture;
static volatile LONG state;
static ULONGLONG working_seen;
static unsigned acquired,called,started,p1_finished,checks,interval_called;static DWORD selected;
static PAPACC_RESULT model_acquire(PAPACC_SERVER_NETWORK *n,const PAPACC_SERVER_CONFIG *c,const PAPACC_LOGGER *l,PAPACC_SERVER_LAUNCHER_OWNER_WIN32 **out)
{(void)n;(void)c;(void)l;acquired++;*out=&fixture;fixture.owner_thread=GetCurrentThreadId();return PAPACC_RESULT_OK;}
static PAPACC_RESULT model_finish(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_SERVER_NETWORK *n,PAPACC_BOOL known)
{(void)o;(void)n;(void)known;p1_finished++;return PAPACC_RESULT_OK;}
static PAPACC_U32 model_state(void){return PAPACC_LAUNCHER_ACTIVE;}
static DWORD model_cold(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_COLD_START_INPUT *in)
{(void)o;called++;working_seen=in->limits.working_growth;selected=in->existing_console_data;if(in->existing)return ERROR_INVALID_DATA;return ERROR_ACCESS_DENIED;}
static int model_stop(void){return 0;}
static PAPACC_RESULT model_close(PAPACC_CONFIGURATION_COMMIT *o){(void)o;return PAPACC_RESULT_OK;}
static PAPACC_RESULT model_uninstall(PAPACC_SERVER_CONSOLE_WIN32 *c){(void)c;return PAPACC_RESULT_OK;}
static DWORD model_observe(PAPACC_CONFIGURATION_COMMIT *o,DWORD *samples){(void)o;(void)samples;return ERROR_ACCESS_DENIED;}
static DWORD model_interval(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_SCQ_INTERVAL_OBSERVATION *v){(void)o;(void)v;interval_called++;return ERROR_ACCESS_DENIED;}
static BOOL WINAPI model_count(HANDLE h,PDWORD n){(void)h;(void)n;return FALSE;}
static int model_service(int argc,const char *const *argv,FILE *out,FILE *err,void *hooks)
{(void)argc;(void)argv;(void)out;(void)err;(void)hooks;started++;return 2;}
static void WINAPI model_sleep(DWORD ms){(void)ms;}
static unsigned fixture_sources,fixture_executes;
static size_t model_fixture_known(void){return sizeof(PAPACC_CONFIGURATION_COMMIT)+PAPACC_MACHINE_FIXTURE_MANAGED_EXTRA;}
static DWORD model_fixture_source(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_MACHINE_FIXTURE_INPUT *input)
{(void)o;(void)input;fixture_sources++;return ERROR_ACCESS_DENIED;}
static DWORD model_fixture_execute(PAPACC_CONFIGURATION_COMMIT *o,FILE *out)
{(void)o;(void)out;fixture_executes++;return ERROR_ACCESS_DENIED;}
#define papacc_server_launcher_machine_fixture_known_bytes model_fixture_known
#define papacc_configuration_personal_machine_fixture_source model_fixture_source
#define papacc_configuration_personal_machine_fixture_execute model_fixture_execute
#define papacc_server_launcher_owner_acquire model_acquire
#define papacc_server_launcher_owner_finish model_finish
#define papacc_server_launcher_owner_state model_state
#define papacc_configuration_personal_scq_cold_machine_read_controller(o,i) model_cold(o,&(i)->cold)
#define papacc_configuration_personal_scq_cold_controller model_cold
#define papacc_server_console_win32_stop_requested model_stop
#define papacc_configuration_personal_native_close model_close
#define papacc_server_console_win32_uninstall model_uninstall
#define papacc_configuration_personal_cold_handle_observe model_observe
#define papacc_configuration_personal_request_interval_observe model_interval
#define GetProcessHandleCount model_count
#define papacc_server_service_cli model_service
#define Sleep model_sleep
#include "../apps/server/server_launcher_scq_cold_start_win32.inc"
#define CHECK(x) do{checks++;if(!(x)){printf("P1_ENTRY_FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
int main(void)
{
 const char *pin="0101010101010101010101010101010101010101010101010101010101010101";
 const char *args[16]={"controller","--owner-manual-first-scq-start-to-existing","C:\\request.md",NULL,"C:\\decision.md",NULL,NULL,
  "1994944","1994944","7239172096","9823326208","128","100","60000","240000",NULL};
 unsigned i;int result;
 {const char *fixture_args[]={"controller","--owner-manual-machine-fixture-prepare","C:\\request",NULL,"C:\\decision",NULL,NULL,NULL,NULL,"extra"};
  fixture_args[3]=fixture_args[5]=fixture_args[6]=fixture_args[7]=fixture_args[8]=pin;
  for(i=0;i<3U;i++){
   acquired=called=started=p1_finished=fixture_sources=fixture_executes=0;memset(&fixture,0,sizeof(fixture));
   result=papacc_server_launcher_scq_original_cli(i==1U?8:i==2U?10:9,fixture_args,stdout,stderr);
   CHECK(result!=0&&!started&&!called&&!fixture_executes);
   CHECK(i?(!acquired&&!fixture_sources):(acquired==1U&&fixture_sources==1U));
  }
 }

 args[3]=args[5]=args[6]=pin;
 for(i=0;i<11U;i++){
  memset(&fixture,0,sizeof(fixture));acquired=called=started=p1_finished=0;selected=99;
  args[1]=(i==1U||i==7U)?"--owner-manual-first-scq-start":i==2U?"--owner-manual-first-scq-start-read":i==3U?"--owner-manual-first-scq-start-new-install":"--owner-manual-first-scq-start-to-existing";
  args[8]=i==1U?"1994944":"3145728";
  args[10]=i==1U?"9823326208":"3221225472";
  args[11]=i==6U?"129":(i==1U||i==8U)?"128":i==9U?"18446744073709551616":i==10U?"4294967295":"192";
  result=papacc_server_launcher_scq_original_cli(i==4U?16:i==5U?14:15,args,stdout,stderr);
  CHECK(result!=0&&!started&&!fixture.configuration_commit.pending_commit.personal_native.scq_admin.value.controller_consumed);
  if(i<2U){CHECK(working_seen==(i?1994944ULL:3145728ULL));CHECK(acquired==1U&&called==1U&&p1_finished==1U&&selected==(i?0U:1U));}
  else CHECK(!acquired&&!called&&!p1_finished);
 }
 for(i=0;i<3U;i++){
  args[1]="--owner-manual-first-scq-start-to-existing";args[11]="192";
  args[10]=i==0U?"18446744073709551616":i==1U?"0":"-1";
  memset(&fixture,0,sizeof(fixture));acquired=called=started=p1_finished=0;
  CHECK(papacc_server_launcher_scq_original_cli(15,args,stdout,stderr)!=0&&!acquired&&!called&&!started);
 }
 for(i=0;i<2U;i++){
  const char *observe[]={"controller","--observe-p1-request-decision-interval","forbidden-argument"};
  memset(&fixture,0,sizeof(fixture));acquired=called=started=p1_finished=interval_called=0;
  result=papacc_server_launcher_scq_original_cli(i?3:2,observe,stdout,stderr);
  CHECK(result!=0&&!called&&!started&&!fixture.configuration_commit.pending_commit.personal_native.install_paths.phase);
  CHECK(i?(!acquired&&!interval_called&&!p1_finished):(acquired==1U&&interval_called==1U&&p1_finished==1U));
 }

 {const char *read_args[18];unsigned k,j;char too_long[114];
  memset(too_long,'x',113);too_long[113]=0;
  for(k=0;k<7U;k++){
   for(j=0;j<15U;j++)read_args[j]=args[j];read_args[1]="--owner-manual-machine-read-scq-start";read_args[10]="3221225472";read_args[11]="192";
   read_args[15]=k==2U?too_long:"papacc-test-data";read_args[16]=k==3U?"bad":pin;read_args[17]="extra";
   if(k==4U)read_args[11]="128";if(k==5U)read_args[1]="--owner-manual-first-scq-start-read";
   memset(&fixture,0,sizeof(fixture));acquired=called=started=p1_finished=0;selected=99;
   result=papacc_server_launcher_scq_original_cli(k==1U?16:k==6U?18:17,read_args,stdout,stderr);
   CHECK(result!=0&&!started);CHECK(k==0U?(acquired==1U&&called==1U&&selected==1U):(!acquired&&!called));
  }
  printf("PASS READ_ENTRY cases=7 NARROW_CALLER_NO_ARGV_AUTHORITY_NO_FALLBACK\n");
 }
 printf("PASS P1_ENTRY cases=13 checks=%u FIXED_DIAGNOSTIC_NO_PATH_ARGS_NO_AUTHORITY_ALL_BOUNDARIES_MODELED\n",checks);return 0;
}
