/* Actual cold controller body, constructors, three original acts and G0.
 * Native file/token/path/hash/console acquisition boundaries MODELED; selectors
 * and synthetic budgets are not operational approval or native evidence. */
#define PAPACC_FIRST_CONTROLLER_ENTRY preserved_first_controller_main
#include "scq_first_controller_test.c"
static unsigned cold_fault,cold_closes;
static DWORD cold_query_plan(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_COLD_START_INPUT *input,PAPACC_SCQ_PLAN_DATA *plan)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;SESSION_FIXTURE *f=&capture_selection_fixture;unsigned i;
  if(cold_fault==1U){n->scq_cold_raw[0]=(HANDLE)(ULONG_PTR)541U;return ERROR_FILE_NOT_FOUND;}
 if(cold_fault==15U||cold_fault==16U){n->scq_cold_raw[0]=(HANDLE)(ULONG_PTR)541U;
  if(cold_fault==15U)o->unresolved=1;else n->scq_inputs.content_hash.retained=1;
  return ERROR_IO_PENDING;}
 *plan=f->plan;n->sid_bytes=f->plan.administrator.sid_bytes;memcpy(n->sid,f->plan.administrator.sid,n->sid_bytes);
 memcpy(&n->token.TokenId,f->plan.administrator.token_id,sizeof(LUID));memcpy(&n->token.AuthenticationId,f->plan.administrator.authentication,sizeof(LUID));memcpy(&n->token.ModifiedId,f->plan.administrator.modified,sizeof(LUID));
 n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;n->install_paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;
 wcscpy_s(n->install_paths.binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");n->install_paths.parent[0]=n->install_paths.data[0]=n->install_paths.leaf[0]=0;
 for(i=0;i<3U;i++)n->scq_cold_raw[i]=(HANDLE)(ULONG_PTR)(541U+i);
 if(cold_fault==2U)n->scq_cold_raw[1]=NULL;
 if(cold_fault==3U)plan->administrator.pid++;
 if(cold_fault==4U)plan->purpose=PAPACC_SCQ_PURPOSE_READ;
 if(!input->authorization_ms)return ERROR_INVALID_DATA;return 0;
}
static PAPACC_RESULT cold_profile(PAPACC_CONFIGURATION_COMMIT *o,DWORD role,const PAPACC_SCQ_PLAN_DATA *plan,const PAPACC_SCQ_PEER_DATA *local)
{return papacc_scq_profile_prepare_transport_data(&o->pending_commit.personal_native.scq_profile_data,o,role,plan,local)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;}
static PAPACC_RESULT cold_acquire(PAPACC_CONFIGURATION_COMMIT *o,const HANDLE originals[3])
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;SESSION_FIXTURE *f=&capture_selection_fixture;unsigned i;
 if(!originals[0]||!originals[1]||!originals[2])return PAPACC_RESULT_INVALID_STATE;
 n->scq_inputs.phase=1U;n->scq_inputs.content_verified=1U;
 n->scq_inputs.identity[0]=f->original.request_id;n->scq_inputs.identity[1]=f->original.decision_id;n->scq_inputs.identity[2]=f->original.image_id;
 for(i=0;i<3U;i++){n->scq_inputs.file[i]=(HANDLE)((ULONG_PTR)originals[i]+100U);n->scq_inputs.reader[i]=(HANDLE)((ULONG_PTR)originals[i]+200U);memcpy(n->scq_inputs.digest[i],f->plan.refs[i].digest,32);memcpy(n->scq_inputs.descriptor_digest[i],f->plan.refs[i].descriptor,32);}
 if(cold_fault==5U)n->scq_inputs.digest[0][0]^=1U;
 if(cold_fault==6U){n->scq_inputs.phase=4U;return PAPACC_RESULT_INVALID_STATE;}
 if(personal_scq_selection_capture(o))return PAPACC_RESULT_INVALID_STATE;
 {PAPACC_PERSONAL_SCQ_ORIGINAL v=f->original;
  v.controller=o;v.recipient=o;v.request=n->scq_inputs.reader[0];v.decision=n->scq_inputs.reader[1];v.image=n->scq_inputs.reader[2];
  return scqa_capture(&n->scq_admin,o,&n->scq_selection,&n->scq_profile_data,&v,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
 }
}
static PAPACC_RESULT cold_limits(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SCQ_START_LIMITS *limits)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 return scqg_capture(&n->scq_start_assurance,o,&n->scq_admin,limits,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
static BOOL WINAPI cold_raw_close(HANDLE h)
{cold_closes++;return cold_fault==7U?FALSE:((ULONG_PTR)h>=541U&&(ULONG_PTR)h<=543U);}
static int scqa_console_hex(HANDLE out,const char *label,const BYTE *data,DWORD count);
#define scqc_plan_variant(o,i,p,r) ((void)(r),cold_query_plan(o,i,p))
#define papacc_configuration_personal_scq_profile_prepare_machine_read_data cold_profile
#define personal_scq_admin_native_current(o) ((void)(o),0)
#define scqk_prepare(k,o,s,p,st) ((void)(k),(void)(o),(void)(s),(void)(p),(void)(st),ERROR_ACCESS_DENIED)
#define scqk_confirm(k,o,r,z,st) ((void)(k),(void)(o),(void)(r),(void)(z),(void)(st),ERROR_ACCESS_DENIED)
#define scqc_plan cold_query_plan
#define papacc_configuration_personal_scq_profile_prepare_transport_data cold_profile
#define papacc_configuration_personal_scq_session_acquire_originals cold_acquire
#define papacc_configuration_personal_scq_start_plan_capture cold_limits
#undef CloseHandle
#define CloseHandle cold_raw_close
#include "../apps/server/personal_scq_cold_start_win32.inc"

/* Actual presenter body; only original-source and console boundaries modeled.
 * No operational approval, path acquisition, SCM or endpoint is executed. */
static PAPACC_SCQ_OWNER_DECISION projection_policy;
static unsigned projection_case,projection_writes,projection_fail;
static char projection_text[32768];
static const PAPACC_SCQ_OWNER_DECISION *projection_source(PAPACC_CONFIGURATION_COMMIT *o)
{return projection_case==4U||o->pending_commit.stop?NULL:&projection_policy;}
static int projection_write(HANDLE out,const char *text)
{size_t n=strlen(projection_text),z=strlen(text);(void)out;projection_writes++;
 if(projection_fail==projection_writes||n+z>=sizeof(projection_text))return 0;
 memcpy(projection_text+n,text,z+1U);return 1;}
static BOOL WINAPI projection_wide(HANDLE out,const VOID *buffer,DWORD n,LPDWORD written,LPVOID unused)
{DWORD k;char text[MAX_PATH];const WCHAR *wide=buffer;(void)unused;
 if(n>=MAX_PATH)return FALSE;for(k=0;k<n;k++)text[k]=(char)wide[k];text[n]=0;
 if(!projection_write(out,text))return FALSE;*written=n;return TRUE;}
#define personal_scq_registered_owner_decision projection_source
#define personal_console_write projection_write
#define WriteConsoleW projection_wide
#include "../apps/server/personal_scq_console_projection_win32.inc"
#undef WriteConsoleW
#undef personal_console_write
#undef personal_scq_registered_owner_decision
static int projection_cases(void)
{
 PAPACC_CONFIGURATION_COMMIT *o=&selection_owner;PAPACC_PERSONAL_INSTALL_PATHS paths;
 PAPACC_SCQ_PLAN_DATA plan;unsigned i,total_writes=0;int result;
 for(i=0;i<12U;i++){
  projection_case=i;projection_fail=projection_writes=0;projection_text[0]=0;
  C(!setup_session_purpose(&capture_selection_fixture,1,1U,(i==7U||i==10U)?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
  plan=capture_selection_fixture.plan;memset(o,0,sizeof(*o));memset(&paths,0,sizeof(paths));
  registration(&projection_policy,&plan);
  paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;
  wcscpy_s(paths.binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
  if(i==1U)paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;
  if(i==2U)paths.parent[0]=L'X';if(i==3U)paths.data[0]=L'X';
  if(i==5U)paths.leaf[0]=L'X';if(i==6U)paths.binary[0]=0;
  if(i==7U){plan.purpose=PAPACC_SCQ_PURPOSE_READ;registration(&projection_policy,&plan);}
  if(i>=8U&&i<=10U)paths.mode=PAPACC_PERSONAL_PATHS_NEW;
  if(i==9U||i==10U){wcscpy_s(paths.parent,MAX_PATH,L"C:\\ProgramData\\Papinho");wcscpy_s(paths.data,MAX_PATH,L"C:\\ProgramData\\Papinho\\Accelerator");}
  if(i==10U){plan.purpose=PAPACC_SCQ_PURPOSE_READ;registration(&projection_policy,&plan);}
  if(i==11U)o->pending_commit.stop=1;
  result=scqa_console_projection(o,(HANDLE)2,&plan,&paths);
  if(result!=(i==0U||i==1U||i==9U||i==10U))printf("projection case=%u result=%d writes=%u scope=%d\n",i,result,projection_writes,scqa_scope(&projection_policy,&plan));
  C(result==(i==0U||i==1U||i==9U||i==10U));
  C(!o->pending_commit.personal_native.scq_admin.value.controller_consumed&&
   !o->pending_commit.personal_native.scq_admin.endpoint_consumed&&!o->pending_commit.personal_native.scq_admin.start_consumed);
  if(i==0U){total_writes=projection_writes;C(strstr(projection_text,"DATA_PARENT=NOT_APPLICABLE_TRANSPORT_ONLY")&&strstr(projection_text,"KEY=NOT_APPLICABLE_TRANSPORT_ONLY_NO_KSP"));}
  if(i==9U||i==10U)C(strstr(projection_text,"DATA_CONTAINER=C:\\ProgramData"));
 }
 projection_case=0;o->pending_commit.stop=0;C(!setup_session_purpose(&capture_selection_fixture,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));plan=capture_selection_fixture.plan;registration(&projection_policy,&plan);
 memset(&paths,0,sizeof(paths));paths.mode=PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;
 wcscpy_s(paths.binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
 for(i=1;i<=total_writes;i++){
  projection_writes=0;projection_text[0]=0;projection_fail=i;
  C(!scqa_console_projection(o,(HANDLE)2,&plan,&paths));
  C(!o->pending_commit.personal_native.scq_admin.value.controller_consumed);
 }
 printf("PASS ACTUAL_SCQ_PROJECTION profiles=12 write_faults=%u NO_CONFIRMATION_NO_AUTHORITY\n",total_writes);
 return 0;
}
int main(void)
{
 PAPACC_CONFIGURATION_COMMIT *o=&selection_owner;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 SESSION_FIXTURE *f=&capture_selection_fixture;unsigned i,before=checks;DWORD e;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<17U;i++){
  PAPACC_SCQ_COLD_START_INPUT input={0};PAPACC_SCM_RESOURCE_SAMPLE sample={1000U,100U,101U,1000U,2000U,1000000U,1000000U,100U,1U};
  C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));C(!scqp_decode_fields(f->controller_profile.record,sizeof(f->controller_profile.record),&f->plan));
  memset(o,0,sizeof(*o));o->writer=GetCurrentThreadId();cold_fault=cold_closes=fc_fault=fc_prompts=fc_calls=selection_fault=origin_fault=0;receiver_identity_alias=1U;
  input.authorization_ms=1000U;input.limits.private_growth=input.limits.working_growth=1000U;
  input.limits.physical_floor=input.limits.commit_floor=100000U;input.limits.max_handles=128U;input.limits.freshness_ms=10U;input.limits.duration_ms=100U;
  if(i>=1U&&i<=7U)cold_fault=i;
  if(i>=15U)cold_fault=i;
  if(i==8U)input.limits.physical_floor=0;
  if(i==9U)fc_fault=5U;if(i==10U)fc_fault=20U;if(i==11U)fc_fault=6U;
  if(i==12U)o->writer++;if(i==13U)o->pending_commit.stop=1;if(i==14U)n->phase=1U;
  e=papacc_configuration_personal_scq_cold_controller(o,&input);
  if(i){C(e!=0&&!n->scq_admin.value.controller_consumed&&!n->scq_admin.start_consumed&&!n->scq_admin.endpoint_consumed);
   if(i==1U)C(cold_closes==1U&&!n->scq_cold_raw[0]&&!o->unresolved);
   if(i>=2U&&i<=6U)C(!n->scq_cold_raw[0]&&!n->scq_cold_raw[1]&&!n->scq_cold_raw[2]&&!o->unresolved);
   if(i>=15U)C(o->unresolved&&!cold_closes&&n->scq_cold_raw[0]);
   if(i==7U)C(o->unresolved&&cold_closes==1U&&n->scq_cold_raw[2]);continue;}
  C(!e&&cold_closes==3U&&fc_prompts==3U&&n->scq_session.phase==1U);
  C(!n->scq_cold_raw[0]&&!n->scq_cold_raw[1]&&!n->scq_cold_raw[2]);
  C(n->scq_start_assurance.presented&&n->scq_admin.phase==3U&&n->scq_admin.start_phase==2U&&n->scq_admin.endpoint_phase==2U);
  C(!scqg_ready(&n->scq_start_assurance,o,&sample,101U,&o->pending_commit.stop));
  C(!n->scq_admin.start_consumed&&!n->scq_admin.endpoint_consumed&&!n->scq_admin.value.controller_consumed);
  C(papacc_configuration_personal_scq_cold_controller(o,&input)!=0);
 }
 C(!projection_cases());
 C(!mcreated&&!mopened&&!mevents&&!cs_running);
 printf("PASS COLD_CONTROLLER cases=17 checks=%u ACTUAL_COLD_ORCHESTRATION_SOURCE_ACTS_G0 NATIVE_ACQUISITION_CONSOLE_MODELED NO_ENDPOINT_NO_START_NO_RNG_NO_KSP\n",checks-before);return 0;
}
