#define PAPACC_SCQ_PRE_READINESS_ENTRY preserved_native_selection_chain_main
#include "scq_pre_readiness_chain_test.c"
#include "server_configuration_commit_win32.h"
/* Native acquisition boundaries are MODELED; the constructor body and owner
 * types are the production ones. No decision/confirmation is synthesized by
 * the constructor, and no endpoint, RNG, CNG or SCM is invoked. */
static PAPACC_CONFIGURATION_COMMIT selection_owner;
static SESSION_FIXTURE capture_selection_fixture;
static DWORD selection_fault,selection_read_chain;
static int selection_token(PAPACC_CONFIG_PENDING *pending,int capture)
{(void)pending;(void)capture;return selection_fault!=1U;}
static DWORD selection_paths(const PAPACC_PERSONAL_INSTALL_PATHS *paths,volatile LONG *stop)
{return selection_fault==2U||*stop||paths->phase!=PAPACC_PERSONAL_PATHS_READY?ERROR_ACCESS_DENIED:0;}
static DWORD selection_binary_paths(const PAPACC_PERSONAL_INSTALL_PATHS *paths,volatile LONG *stop)
{return paths->mode!=PAPACC_PERSONAL_PATHS_BINARY_ONLY||paths->parent[0]||paths->data[0]||paths->leaf[0]?ERROR_ACCESS_DENIED:selection_paths(paths,stop);}
/* This fixture has no original native chain: selected-object proof must deny. */
static DWORD selection_original_chain(PAPACC_SCQ_IMAGE_CHAIN *chain,PSID actor,volatile LONG *stop)
{(void)actor;if(selection_read_chain&&chain&&chain->self==chain&&chain->owner==&selection_owner&&chain->phase==2U&&chain->paths&&chain->paths->read_diagnostic==1U&&chain->plan.purpose==PAPACC_SCQ_PURPOSE_READ&&!*stop)return 0;return ERROR_ACCESS_DENIED;}
#define scqi_chain_current selection_original_chain
static DWORD WINAPI selection_pid(void){return capture_selection_fixture.plan.administrator.pid;}
static BOOL WINAPI selection_times(HANDLE h,LPFILETIME c,LPFILETIME e,LPFILETIME k,LPFILETIME u)
{(void)h;memcpy(c,capture_selection_fixture.plan.administrator.creation,sizeof(*c));memset(e,0,sizeof(*e));memset(k,0,sizeof(*k));memset(u,0,sizeof(*u));return selection_fault!=3U;}
#define personal_token selection_token
#define papacc_personal_paths_current selection_paths
#define papacc_personal_paths_binary_current selection_binary_paths
#undef GetCurrentProcessId
#define GetCurrentProcessId selection_pid
#undef GetProcessTimes
#define GetProcessTimes selection_times
#include "../apps/server/personal_scq_selection_win32.inc"
#ifndef PAPACC_NATIVE_SELECTION_ENTRY
#define PAPACC_NATIVE_SELECTION_ENTRY main
#endif
int PAPACC_NATIVE_SELECTION_ENTRY(void)
{
 PAPACC_CONFIGURATION_COMMIT *o=&selection_owner;PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;
 PAPACC_PERSONAL_SCQ_ORIGINAL original;char confirmation[75];
 SESSION_FIXTURE *f=&capture_selection_fixture;unsigned i,j,before=checks;DWORD e;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(personal_scq_selection_capture(NULL)!=0);
 for(i=0;i<24U;i++){
  C(!(i>=20U?setup_session_typed_capture(f,1,1U,PAPACC_SCQ_PURPOSE_READ,1U,0):setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)));
  C(!scqp_decode_fields(f->controller_profile.record,sizeof(f->controller_profile.record),&f->plan));
  memset(o,0,sizeof(*o));o->writer=GetCurrentThreadId();selection_fault=origin_fault=selection_read_chain=0;
  C(!personal_scq_registered_owner_decision(o));
  C(!(i>=20U?(i==21U?papacc_scq_profile_prepare_closed_data:papacc_scq_profile_prepare_machine_read_data)(&n->scq_profile_data,o,1U,&f->plan,&f->plan.administrator):papacc_scq_profile_prepare_transport_data(&n->scq_profile_data,o,1U,&f->plan,&f->plan.administrator)));
  n->install_paths.phase=PAPACC_PERSONAL_PATHS_READY;n->install_paths.mode=PAPACC_PERSONAL_PATHS_NEW;
  wcscpy_s(n->install_paths.binary,MAX_PATH,L"C:\\Program Files\\Papinho\\Accelerator");
  wcscpy_s(n->install_paths.parent,MAX_PATH,L"C:\\ProgramData\\Papinho");
  wcscpy_s(n->install_paths.data,MAX_PATH,L"C:\\ProgramData\\Papinho\\Accelerator");
  n->sid_bytes=f->plan.administrator.sid_bytes;memcpy(n->sid,f->plan.administrator.sid,n->sid_bytes);
  memcpy(&n->token.TokenId,f->plan.administrator.token_id,sizeof(LUID));
  memcpy(&n->token.AuthenticationId,f->plan.administrator.authentication,sizeof(LUID));
  memcpy(&n->token.ModifiedId,f->plan.administrator.modified,sizeof(LUID));
  n->scq_inputs.phase=1U;n->scq_inputs.content_verified=1U;
  n->scq_inputs.identity[0]=f->original.request_id;n->scq_inputs.identity[1]=f->original.decision_id;n->scq_inputs.identity[2]=f->original.image_id;
  for(j=0;j<3U;j++){n->scq_inputs.reader[j]=(HANDLE)(ULONG_PTR)(41U+j);memcpy(n->scq_inputs.digest[j],f->plan.refs[j].digest,32);memcpy(n->scq_inputs.descriptor_digest[j],f->plan.refs[j].descriptor,32);}
  if(i>=1U&&i<=3U)selection_fault=i;
  if(i==4U)o->writer++;
  if(i==5U)o->pending_commit.stop=1;
  if(i==6U)n->scq_inputs.content_verified=0;
  if(i==7U)n->scq_inputs.digest[0][0]^=1U;
  if(i==8U)n->scq_inputs.descriptor_digest[1][0]^=1U;
  if(i==9U)n->sid[8]^=1U;
  if(i==10U)n->token.TokenId.LowPart++;
  if(i==11U)n->scq_admin.phase=1U;
  if(i==12U)n->scq_selection_captured=1U;
  if(i==13U)n->install_paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;
  if(i==14U)n->scq_inputs.reader[2]=NULL;
  if(i==15U)n->scq_profile_data.waiting_runtime=0U;
  if(i>=16U){n->install_paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;n->install_paths.parent[0]=n->install_paths.data[0]=n->install_paths.leaf[0]=0;}
  if(i==17U)n->install_paths.data[0]=L'X';
  if(i==18U)selection_fault=2U;
  if(i==19U)n->install_paths.mode=PAPACC_PERSONAL_PATHS_EXISTING;
  if(i>=20U){
   PAPACC_SCQ_IMAGE_CHAIN *g=&n->scq_inputs.image_chain;selection_read_chain=1U;
   n->install_paths.mode=i==22U?PAPACC_PERSONAL_PATHS_BINARY_ONLY:PAPACC_PERSONAL_PATHS_SELECTED_OBJECT;n->install_paths.read_diagnostic=1U;
   n->install_paths.thread=GetCurrentThreadId();n->install_paths.selected_role=1U;n->install_paths.original_chain=g;
   n->install_paths.parent[0]=n->install_paths.data[0]=n->install_paths.leaf[0]=0;
   g->self=g;g->owner=o;g->paths=&n->install_paths;g->phase=2U;g->plan=f->plan;
   if(i==23U){n->install_paths.read_diagnostic=0U;}
  }
  e=personal_scq_selection_capture(o);
  if(i&&i!=16U&&i!=20U){C(e!=0&&!n->scq_selection.controller&&!n->scq_admin.start_consumed);continue;}
  C(!e&&n->scq_selection_captured==1U&&scqa_scope(&n->scq_selection,&f->plan));C(n->scq_selection.read_diagnostic==(i==20U?1U:0U));
  C(n->scq_selection.controller==o&&n->scq_selection.source==&n->scq_admin);
  C(n->scq_selection.binary==n->install_paths.binary&&!n->scq_admin.phase&&!n->scq_admin.start_phase&&!n->scq_admin.start_consumed);
  C(!personal_scq_registered_owner_decision(o)); /* candidate alone is not an original act */
  C(!scqa_start_selection(&n->scq_admin,o,&o->pending_commit.stop));
  C(personal_scq_selection_capture(o)!=0);
  original=f->original;original.controller=o;original.recipient=o;
  C(!scqa_capture(&n->scq_admin,o,&n->scq_selection,&n->scq_profile_data,&original,&o->pending_commit.stop));
  C(personal_scq_registered_owner_decision(o)==&n->scq_selection);
  C(!scqa_start_selection(&n->scq_admin,o,&o->pending_commit.stop));
  C(!scqa_present(&n->scq_admin,o,&o->pending_commit.stop));
  memcpy(confirmation,"OWNER_SCQ ",10);
  for(j=0;j<32U;j++){confirmation[10U+2U*j]="0123456789abcdef"[f->plan.attempt[j]>>4];confirmation[11U+2U*j]="0123456789abcdef"[f->plan.attempt[j]&15U];}confirmation[74]=0;
  C(!scqa_confirm(&n->scq_admin,o,confirmation,74U,&o->pending_commit.stop));
  C(!scqa_start_selection(&n->scq_admin,o,&o->pending_commit.stop)); /* SCQ confirmation does not authorize Start */
  o->unresolved=1;C(!personal_scq_registered_owner_decision(o));
 }
 {PAPACC_PERSONAL_INSTALL_PATHS paths;volatile LONG stop=0;
  memset(&paths,0,sizeof(paths));paths.phase=PAPACC_PERSONAL_PATHS_READY;paths.mode=PAPACC_PERSONAL_PATHS_BINARY_ONLY;selection_fault=0;
  C(personal_scq_paths_current(&paths,PAPACC_SCQ_PURPOSE_READ,&stop)!=0);
  C(!personal_scq_paths_current(&paths,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY,&stop));
  stop=1;C(personal_scq_paths_current(&paths,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY,&stop)!=0);
 }
 C(!mcreated&&!mopened&&!mevents&&!cs_running);
 printf("PASS NATIVE_SELECTION_CAPTURE cases=24 checks=%u CONSTRUCTOR_BODY_NATIVE ORIGINAL_ACQUISITION_AND_CNG_BOUNDARIES_MODELED CANDIDATE_NOT_AUTHORITY NO_EFFECTS\n",checks-before);
 return 0;
}
