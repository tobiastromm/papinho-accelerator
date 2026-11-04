/* Actual controller orchestration/selection/source/session state machines.
 * Acquisition, hashing, token/path and console boundaries are modeled.
 * No endpoint, RNG, KSP, SCM or native attributed confirmation is exercised. */
#define PAPACC_NATIVE_SELECTION_ENTRY preserved_native_selection_main
#include "scq_native_selection_capture_test.c"
#include "../apps/server/personal_scq_start_assurance_win32.inc"
static unsigned fc_fault,fc_calls,fc_prompts;
static PAPACC_RESULT fc_contents(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;fc_calls++;
 if(fc_fault==1U)return PAPACC_RESULT_INVALID_STATE;
 if(fc_fault==2U){n->scq_inputs.content_hash.retained=1U;return PAPACC_RESULT_INVALID_STATE;}
 n->scq_inputs.content_verified=1U;return PAPACC_RESULT_OK;
}
static PAPACC_RESULT fc_prepare(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 PAPACC_PERSONAL_SCQ_ORIGINAL v=capture_selection_fixture.original;
 v.controller=o;v.recipient=o;
 if(fc_fault==3U)v.request=NULL;
 return scqa_capture(&n->scq_admin,o,&n->scq_selection,&n->scq_profile_data,&v,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static PAPACC_RESULT fc_console(PAPACC_SERVER_CONSOLE_WIN32 *console)
{if(fc_fault==4U)return PAPACC_RESULT_INVALID_STATE;console->is_installed=1;return PAPACC_RESULT_OK;}
static PAPACC_RESULT fc_confirm(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;char text[75];unsigned j;
 fc_prompts++;if(scqa_present(&n->scq_admin,o,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 if(n->scq_start_assurance.phase&&scqg_presented(&n->scq_start_assurance,o,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 memcpy(text,"OWNER_SCQ ",10U);
 for(j=0;j<32U;j++){text[10U+2U*j]="0123456789abcdef"[capture_selection_fixture.plan.attempt[j]>>4];text[11U+2U*j]="0123456789abcdef"[capture_selection_fixture.plan.attempt[j]&15U];}text[74]=0;
 if(fc_fault==5U)text[0]='X';
 return scqa_confirm(&n->scq_admin,o,text,74U,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static PAPACC_RESULT fc_endpoint(PAPACC_CONFIGURATION_COMMIT *o)
{
 static const char prefix[]="OWNER_SCQ_PRIVATE_ENDPOINT ";char text[sizeof(prefix)+64U];unsigned j;
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 fc_prompts++;if(scqa_endpoint_present(&n->scq_admin,o,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 memcpy(text,prefix,sizeof(prefix)-1U);
 for(j=0;j<32U;j++){text[sizeof(prefix)-1U+2U*j]="0123456789abcdef"[capture_selection_fixture.plan.attempt[j]>>4];text[sizeof(prefix)+2U*j]="0123456789abcdef"[capture_selection_fixture.plan.attempt[j]&15U];}text[sizeof(text)-1U]=0;
 if(fc_fault==20U)text[0]='X';if(fc_fault==21U)o->pending_commit.stop=1;
 return scqa_endpoint_confirm(&n->scq_admin,o,text,sizeof(text)-1U,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static PAPACC_RESULT fc_start(PAPACC_CONFIGURATION_COMMIT *o)
{
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";char text[sizeof(prefix)+64U];unsigned j;
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 fc_prompts++;if(scqa_start_present(&n->scq_admin,o,&o->pending_commit.stop))return PAPACC_RESULT_INVALID_STATE;
 memcpy(text,prefix,sizeof(prefix)-1U);
 for(j=0;j<32U;j++){text[sizeof(prefix)-1U+2U*j]="0123456789abcdef"[capture_selection_fixture.plan.attempt[j]>>4];text[sizeof(prefix)+2U*j]="0123456789abcdef"[capture_selection_fixture.plan.attempt[j]&15U];}text[sizeof(text)-1U]=0;
 if(fc_fault==6U)text[0]='X';if(fc_fault==7U)o->pending_commit.stop=1;
 return scqa_start_confirm(&n->scq_admin,o,text,sizeof(text)-1U,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static PAPACC_RESULT fc_session(PAPACC_CONFIGURATION_COMMIT *o,DWORD role)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 if(fc_fault==8U)return PAPACC_RESULT_INVALID_STATE;
 return scqs_prepare(&n->scq_session,o,role,&n->scq_profile_data,&n->scq_admin,&n->scq_receiver,&n->scq_dispatch,&n->runtime_identity)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
#define papacc_configuration_personal_scq_acquire_contents fc_contents
#define papacc_configuration_personal_scq_admin_prepare fc_prepare
#define papacc_server_console_win32_install fc_console
#define papacc_configuration_personal_scq_admin_confirm_console fc_confirm
#define papacc_configuration_personal_scq_start_confirm_console fc_start
#define papacc_configuration_personal_scq_endpoint_confirm_console fc_endpoint
#define papacc_configuration_personal_scq_session_prepare fc_session
#include "../apps/server/personal_scq_first_controller_win32.inc"
#ifndef PAPACC_FIRST_CONTROLLER_ENTRY
#define PAPACC_FIRST_CONTROLLER_ENTRY main
#endif
int PAPACC_FIRST_CONTROLLER_ENTRY(void)
{
 PAPACC_CONFIGURATION_COMMIT *o=&selection_owner;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 SESSION_FIXTURE *f=&capture_selection_fixture;PS_SCQ_CARRIER original_carrier={0};unsigned i,j,before=checks;DWORD e;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(papacc_configuration_personal_scq_first_controller(NULL)!=0);
 for(i=0;i<24U;i++){
  C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
  C(!scqp_decode_fields(f->controller_profile.record,sizeof(f->controller_profile.record),&f->plan));
  memset(o,0,sizeof(*o));o->writer=GetCurrentThreadId();selection_fault=origin_fault=0;fc_fault=fc_calls=fc_prompts=0;
  C(!papacc_scq_profile_prepare_transport_data(&n->scq_profile_data,o,1U,&f->plan,&f->plan.administrator));
  n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;n->install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;
  wcscpy_s(n->install_paths.binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
  wcscpy_s(n->install_paths.parent,MAX_PATH,L"C:\\ProgramData\\Papinho");
  wcscpy_s(n->install_paths.data,MAX_PATH,L"C:\\ProgramData\\Papinho\\Accelerator");
  n->sid_bytes=f->plan.administrator.sid_bytes;memcpy(n->sid,f->plan.administrator.sid,n->sid_bytes);
  memcpy(&n->token.TokenId,f->plan.administrator.token_id,sizeof(LUID));
  memcpy(&n->token.AuthenticationId,f->plan.administrator.authentication,sizeof(LUID));
  memcpy(&n->token.ModifiedId,f->plan.administrator.modified,sizeof(LUID));
  n->scq_inputs.phase=1U;n->scq_inputs.identity[0]=f->original.request_id;n->scq_inputs.identity[1]=f->original.decision_id;n->scq_inputs.identity[2]=f->original.image_id;
  for(j=0;j<3U;j++){n->scq_inputs.file[j]=n->scq_inputs.reader[j]=(HANDLE)(ULONG_PTR)(41U+j);memcpy(n->scq_inputs.digest[j],f->plan.refs[j].digest,32);memcpy(n->scq_inputs.descriptor_digest[j],f->plan.refs[j].descriptor,32);}
  if((i>=1U&&i<=8U)||i>=20U)fc_fault=i;
  if(i==9U)n->scq_inputs.file[0]=NULL;
  if(i==10U)o->pending_commit.stop=1;
  if(i==11U)o->writer++;
  if(i==12U)n->scq_profile_data.waiting_runtime=0;
  if(i==13U)n->scq_inputs.digest[1][0]^=1U;
  if(i==14U)n->sid[8]^=1U;
  if(i==15U)n->scq_admin.start_consumed=1U;
  if(i==16U)n->scq_inputs.phase=4U;
  if(i==17U)n->scq_profile_data.self=NULL;
  if(i==18U)n->scq_inputs.reader[2]=NULL;
  if(i==19U)n->scq_inputs.descriptor_digest[0][0]^=1U;
  if(i>=22U){
   n->scq_inputs.content_verified=1U;
   C(!personal_scq_selection_capture(o));C(fc_prepare(o)==PAPACC_RESULT_OK);
   C(n->scq_admin.phase==1U&&n->scq_selection_captured==1U);
   if(i==23U)n->scq_admin.registration=NULL;
  }
  e=papacc_configuration_personal_scq_first_controller(o);
  if(i&&i!=22U){C(e!=0);if(i==2U||i==16U)C(e==ERROR_IO_INCOMPLETE&&o->unresolved);continue;}
  C(!e&&fc_calls==(i==22U?0U:1U)&&fc_prompts==3U&&n->scq_session.phase==1U);
  C(scqa_start_selection(&n->scq_admin,o,&o->pending_commit.stop)==&n->scq_selection);
  C(!n->scq_admin.start_consumed&&!n->scq_admin.endpoint_consumed&&!n->scq_admin.value.controller_consumed&&!n->scq_session.transport.phase);
  C(scqa_endpoint_selection(&n->scq_admin,o,&o->pending_commit.stop)==&n->scq_selection);
  C(!scqa_start_carrier(&n->scq_admin,o,&original_carrier,&o->pending_commit.stop));
  C(papacc_configuration_personal_scq_first_controller(o)!=0); /* no repeated presentation */
 }
 /* A fully valid discovery carrier in a fresh packet cannot recreate the
  * independently attributed source, even after the controller modeled success. */
 memset(o,0,sizeof(*o));o->writer=GetCurrentThreadId();n->scq_carrier=original_carrier;n->scq_carrier_captured=1U;
 {char text[PS_SCQ_CARRIER_CHARS];C(ps_scq_carrier_encode(&n->scq_carrier,text,sizeof(text)));}
 C(!personal_scq_registered_owner_decision(o));
 C(!n->scq_recipient.registration&&!n->scq_session.transport.phase&&!n->scq_admin.phase);
 C(papacc_configuration_personal_scq_first_controller(o)!=0);
 C(!mcreated&&!mopened&&!mevents&&!cs_running);
 printf("PASS FIRST_CONTROLLER cases=24 checks=%u PLAN_OBJECT_BYTES=%zu ACTUAL_ORCHESTRATION_SELECTION_SOURCE_SESSION MODELED_NATIVE_AND_CONSOLE_BOUNDARIES NO_ENDPOINT_NO_SCM_NO_RNG_NO_KSP\n",checks-before,sizeof(PAPACC_SCQ_PLAN_DATA));
 return 0;
}
