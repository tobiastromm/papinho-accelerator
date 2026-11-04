#define _WIN32_WINNT 0x0600
#include "server_configuration_commit_win32.h"
#include "server_issuer_x509_win32.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "scq_profile_fixture.h"
static int eligible=1,target_ok=1,validate_ok=1;
#ifdef PAPACC_PERSONAL_CA_MODEL
static int personal_native_ceremony_ready(const PAPACC_CONFIG_PENDING *p){(void)p;return eligible;}
static int personal_first_child_current(PAPACC_CONFIG_PENDING *p,int worker){(void)p;(void)worker;return 0;} /* FCA originals are not fabricated by this historical model */
static PAPACC_RESULT personal_packet_current(PAPACC_CONFIG_PENDING *p){(void)p;return PAPACC_RESULT_OK;}
static PAPACC_RESULT personal_packet_source(PAPACC_CONFIG_PENDING *p){(void)p;return PAPACC_RESULT_OK;}
static BOOL WINAPI model_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION f){(void)h;memset(f,0,sizeof(*f));f->dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;f->dwVolumeSerialNumber=20;f->nFileIndexHigh=30;f->nFileIndexLow=target_ok?40:41;return TRUE;}
static SECURITY_STATUS model_validate(PAPACC_CA_KEY *k){(void)k;return validate_ok?ERROR_SUCCESS:NTE_BAD_KEY_STATE;}
#define papacc_configuration_personal_ca_initialize papacc_configuration_personal_ca_initialize_model
#define papacc_configuration_personal_ca_match_data papacc_configuration_personal_ca_match_data_model
#define papacc_configuration_personal_ca_bind papacc_configuration_personal_ca_bind_model
#define papacc_configuration_personal_ca_current papacc_configuration_personal_ca_current_model
#define papacc_configuration_personal_ca_release papacc_configuration_personal_ca_release_model
#define papacc_configuration_personal_ca_handoff papacc_configuration_personal_ca_handoff_model
#define papacc_configuration_personal_ca_activate_normal papacc_configuration_personal_ca_activate_normal_model
#define GetFileInformationByHandle model_file
#define papacc_ca_key_validate model_validate
#include "../apps/server/server_configuration_personal_ca_win32.inc"
#undef papacc_ca_key_validate
#undef GetFileInformationByHandle
#endif
static PAPACC_CONFIGURATION_COMMIT o;
static PAPACC_CA_KEY ca;
static unsigned char id[32]={1},authority[16]={1},generation[16]={2},op[4]={1,2,3,4},svc[4]={4,3,2,1};
#define CHECK(x) do{if(!(x)){printf("FAIL %u\n",(unsigned)__LINE__);return 1;}}while(0)
static int setup(void)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_EFFECT_SPEC v;
 unsigned char parent[12]={10,0,0,0,11,0,0,0,12,0,0,0};size_t bytes,i,z;char reply[128];static const char hex[]="0123456789abcdef",prefix[]="OWNER_NEW_INSTALL_CA_INITIALIZE ";
 static const unsigned char config[]="format_version 1\nclient_admission DISABLED\n";
 memset(&o,0,sizeof(o));memset(&ca,0,sizeof(ca));p=&o.pending_commit;n=&p->personal_native;o.writer=GetCurrentThreadId();o.initialized=1;
 n->phase=1;n->handoff_state=PAPACC_PERSONAL_HANDOFF_VERIFIED;memcpy(n->handoff_attempt,id,32);memcpy(n->plan_leaf,"child",6);memcpy(p->previous,"child",6);memcpy(p->next,config,sizeof(config)-1);
 n->parent.dwVolumeSerialNumber=10;n->parent.nFileIndexHigh=11;n->parent.nFileIndexLow=12;memcpy(n->sid,op,4);n->sid_bytes=4;
 p->bootstrap_created=1;p->bootstrap_parent=(HANDLE)(ULONG_PTR)1;p->bootstrap_container=(HANDLE)(ULONG_PTR)2;p->bootstrap_volume=20;p->bootstrap_high=30;p->bootstrap_low=40;p->result=PAPACC_CAS_SUCCESS;
 ca.thread=o.writer;ca.state=PAPACC_CA_KEY_READY;ca.key=(NCRYPT_KEY_HANDLE)3;ca.provider=(NCRYPT_PROV_HANDLE)4;ca.created=1;ca.diagnostic_finalization=2;ca.operator_size=4;ca.service_size=4;memcpy(ca.operator_sid,op,4);memcpy(ca.service_sid,svc,4);
 wcscpy_s(ca.name,96,L"PapinhoAccelerator.CA.01000000000000000000000000000000.02000000000000000000000000000000");
 memset(&v,0,sizeof(v));v.operator_id=op;v.operator_bytes=4;v.parent_id=parent;v.leaf=(const unsigned char *)"child";v.leaf_bytes=5;v.authority=authority;v.generation=generation;v.service_id=svc;v.service_bytes=4;v.configuration=config;v.configuration_bytes=sizeof(config)-1;
 bytes=papacc_personal_effect_build(&v,p->previous+1024,PAPACC_PERSONAL_PLAN_MAX);if(!bytes)return 0;
 if(!papacc_personal_data_capture(&p->personal_data,id,config,sizeof(config)-1)||!papacc_personal_effect_upgrade(&p->personal_data,p->previous+1024,bytes)||!papacc_personal_data_present(&p->personal_data))return 0;
 z=sizeof(prefix)-1;memcpy(reply,prefix,z);for(i=0;i<32;i++){reply[z+i*2]=hex[id[i]>>4];reply[z+i*2+1]=hex[id[i]&15];}
 return papacc_personal_effect_confirm(&p->personal_data,id,p->previous+1024,bytes,reply,z+64)&&papacc_personal_effect_consume(&p->personal_data,id,p->previous+1024,bytes);
}
int main(void)
{
 PAPACC_CONFIG_PENDING *p=&o.pending_commit;unsigned char other[32]={2};PAPACC_RESULT r=PAPACC_RESULT_OK;
 CHECK(setup());
 {HANDLE refs[3]={(HANDLE)1,(HANDLE)2,(HANDLE)3};CHECK(papacc_configuration_personal_scq_capture_inputs(&o,refs)==PAPACC_RESULT_INVALID_STATE&&!p->personal_native.scq_inputs.phase);}
 CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK);
 p->personal_native.runtime_identity.thread=GetCurrentThreadId();p->personal_native.runtime_identity.profile=PAPACC_PERSONAL_RUNTIME_SAM;
 CHECK(papacc_configuration_personal_runtime_cng_probe(&o,L"PapinhoAccelerator.CA.11111111111111111111111111111111.22222222222222222222222222222222")==PAPACC_RESULT_NOT_SUPPORTED);
 CHECK(!p->personal_native.runtime_cng_probe.phase&&!p->personal_native.runtime_cng_probe.provider_calls);
 CHECK(p->personal_native.scq_dispatch.gate_reason==PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE);
 memset(&p->personal_native.runtime_identity,0,sizeof(p->personal_native.runtime_identity));
 p->personal_native.runtime_identity.phase=1;p->personal_native.runtime_identity.sid_bytes=ca.service_size;
 memcpy(p->personal_native.runtime_identity.sid,ca.service_sid,ca.service_size);
 CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK);
 p->personal_native.runtime_identity.sid[0]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);
 memset(&p->personal_native.runtime_identity,0,sizeof(p->personal_native.runtime_identity));
 o.stopped=1;CHECK(papacc_configuration_personal_runtime_bind(&o,(HANDLE)1,NULL,PAPACC_PERSONAL_RUNTIME_VIRTUAL,1)!=PAPACC_RESULT_OK);CHECK(papacc_configuration_personal_runtime_current(&o)!=PAPACC_RESULT_OK);o.stopped=0;
 p->stop=1;CHECK(papacc_configuration_personal_runtime_bind(&o,(HANDLE)1,NULL,PAPACC_PERSONAL_RUNTIME_VIRTUAL,1)!=PAPACC_RESULT_OK);p->stop=0;
 p->personal_native.runtime_identity.phase=3;p->personal_native.runtime_identity.thread=GetCurrentThreadId();p->personal_ca=&ca;
 CHECK(papacc_configuration_personal_runtime_close(&o)!=PAPACC_RESULT_OK);p->personal_ca=NULL;
 memset(&p->personal_native.runtime_identity,0,sizeof(p->personal_native.runtime_identity));
 CHECK(papacc_configuration_personal_ca_match_data(&o,NULL,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,other,authority,generation)!=PAPACC_RESULT_OK);
 ca.operator_sid[0]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.operator_sid[0]^=1;
 p->personal_native.parent.nFileIndexLow^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);p->personal_native.parent.nFileIndexLow^=1;
 p->next[0]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);p->next[0]^=1;
 ca.name[22]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.name[22]^=1;
 ca.service_sid[0]^=1;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.service_sid[0]^=1;
 ca.provider=0;CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);ca.provider=4;
 wcscpy_s(ca.name,96,L"PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0");CHECK(papacc_configuration_personal_ca_match_data(&o,&ca,id,authority,generation)==PAPACC_RESULT_NOT_SUPPORTED);
 wcscpy_s(ca.name,96,L"PapinhoAccelerator.CA.01000000000000000000000000000000.02000000000000000000000000000000");
 o.stopped=1;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);o.stopped=0;
#ifdef PAPACC_PERSONAL_CA_MODEL
 target_ok=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);target_ok=1;
 validate_ok=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);validate_ok=1;
 eligible=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);eligible=1;
 CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK&&p->personal_ca==&ca&&ca.borrower==p);
 CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_ca_key_close(&ca)==NTE_BAD_KEY_STATE&&ca.key&&ca.provider); /* real borrow guard, no native FreeObject */
 CHECK(papacc_configuration_personal_ca_current(&o,id,authority,generation)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_current(&o,other,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_native_close(&o)!=PAPACC_RESULT_OK);CHECK(papacc_configuration_personal_data_retire(&o)!=PAPACC_RESULT_OK);
 p->active=1;CHECK(papacc_configuration_personal_ca_release(&o)!=PAPACC_RESULT_OK&&p->personal_ca==&ca);p->active=0;
 o.stopped=1;p->stop=1;CHECK(papacc_configuration_personal_ca_current(&o,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_release(&o)==PAPACC_RESULT_OK&&!p->personal_ca&&!ca.borrower);
 CHECK(papacc_configuration_personal_ca_release(&o)!=PAPACC_RESULT_OK);
 o.stopped=0;p->stop=0;CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(setup());CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK);
 {PAPACC_ISSUER_CNG_BACKEND backend;PAPACC_ISSUER_CORE core;memset(&backend,0,sizeof(backend));memset(&core,0,sizeof(core));
  backend.initialized=1;backend.thread=o.writer;backend.key=&ca;backend.owner=&core;backend.ports.ctx=&backend;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,other,authority,generation)!=0&&p->personal_ca==&ca&&ca.borrower==p&&!backend.custody);
  backend.key=NULL;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);backend.key=&ca;
  validate_ok=0;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p&&!backend.custody);validate_ok=1;
  target_ok=0;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);target_ok=1;
  eligible=0;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);eligible=1;
  backend.active=1;CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);backend.active=0;
  core.initialized=1;core.continuity_profile=PAPACC_ISSUER_CONTINUITY_RECOVERY;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);core.initialized=0;
  core.initialized=1;core.continuity_profile=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==p);core.initialized=0;
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)==0&&ca.borrower==&backend&&!p->personal_ca&&backend.custody);
  CHECK(papacc_configuration_personal_ca_handoff(&o,&backend,id,authority,generation)!=0&&ca.borrower==&backend);
  CHECK(papacc_configuration_personal_ca_release(&o)!=0&&ca.borrower==&backend); /* source cannot release recipient */
  CHECK(papacc_ca_key_return(&ca,&backend)==0); /* model recipient returns lifetime reference, no native key close */
 }
 CHECK(setup());CHECK(papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation)==PAPACC_RESULT_OK);
 ca.state=PAPACC_CA_KEY_RETAINED;CHECK(papacc_configuration_personal_ca_release(&o)!=PAPACC_RESULT_OK&&o.unresolved&&p->personal_ca==&ca&&ca.borrower==p);
#else
 (void)eligible;(void)target_ok;(void)validate_ok;
 {PAPACC_CA_KEY candidate;memset(&candidate,0,sizeof(candidate));
 CHECK(papacc_configuration_personal_ca_initialize(&o,&candidate,id,authority,generation,svc,4,sizeof(o)+sizeof(candidate)+8192)!=ERROR_SUCCESS);
 CHECK(!candidate.key&&!candidate.provider&&!p->personal_ca_candidate&&!p->ca_initialize_phase);}

 r=papacc_configuration_personal_ca_bind(&o,&ca,id,authority,generation);CHECK(r==PAPACC_RESULT_NOT_SUPPORTED&&!p->personal_ca&&!ca.borrower);
 CHECK(papacc_configuration_personal_ca_bind(&o,NULL,id,authority,generation)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_ca_current(&o,id,authority,generation)!=PAPACC_RESULT_OK);
#endif
 {PAPACC_SCQ_PLAN_DATA plan;CHECK(scq_fixture_plan(&plan));memset(&o,0,sizeof(o));o.writer=GetCurrentThreadId();p=&o.pending_commit;
  CHECK(papacc_configuration_personal_layout_new(&o)==PAPACC_RESULT_OK);
  CHECK(papacc_configuration_personal_scq_profile_prepare_data(&o,2,&plan,&plan.runtime)==PAPACC_RESULT_OK);
  CHECK(papacc_configuration_personal_layout_existing(&o,L"E:\\other",L"E:\\data",L"other")!=PAPACC_RESULT_OK);
  CHECK(papacc_configuration_personal_runtime_cng_probe(&o,L"PapinhoAccelerator.CA.11111111111111111111111111111111.22222222222222222222222222222222")==PAPACC_RESULT_NOT_SUPPORTED);
  CHECK(!p->personal_native.runtime_cng_probe.provider_calls&&!p->personal_native.runtime_cng_probe.phase);
  CHECK(papacc_configuration_personal_data_retire(&o)!=PAPACC_RESULT_OK);
  CHECK(papacc_configuration_personal_native_close(&o)==PAPACC_RESULT_OK&&p->personal_native.scq_profile_data.phase==3);
 }
 {PAPACC_SCQ_RUNTIME_OBSERVATION observation;memset(&observation,0xff,sizeof(observation));
  {DWORD output_bytes=99;
   CHECK(papacc_configuration_personal_scq_session_acquire_originals(&o,NULL)==PAPACC_RESULT_NOT_SUPPORTED&&!o.pending_commit.personal_native.scq_session.phase);
   CHECK(papacc_configuration_personal_scq_session_prepare(&o,1)==PAPACC_RESULT_NOT_SUPPORTED&&!o.pending_commit.personal_native.scq_session.phase);
   CHECK(papacc_configuration_personal_scq_session_prepare(&o,2)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_transport_data(&o,1,0)==ERROR_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_transport_data(&o,2,1)==ERROR_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_recipient_prepare(&o,NULL,NULL,NULL)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_recipient_acquire(&o,NULL)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_transport_enter(&o,0,NULL)==ERROR_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_transport_references(&o,NULL)==ERROR_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_transport_produce_close(&o)==ERROR_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_transport_poll(&o)==ERROR_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_transport_stop(&o)==ERROR_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_session_take_data(&o)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_session_offer_data(&o)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_session_receive_data(&o,NULL,0)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_session_receipt_data(&o)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_session_ack_data(&o,NULL,0)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(papacc_configuration_personal_scq_session_output_data(&o,&output_bytes)==NULL&&!output_bytes);
  }
  CHECK(papacc_configuration_personal_scq_admin_prepare(&o)==PAPACC_RESULT_NOT_SUPPORTED&&!o.pending_commit.personal_native.scq_admin.phase);
  CHECK(papacc_configuration_personal_scq_admin_confirm_console(&o)==PAPACC_RESULT_NOT_SUPPORTED);
  CHECK(papacc_configuration_personal_scq_admin_take(&o)==PAPACC_RESULT_NOT_SUPPORTED);
  CHECK(papacc_configuration_personal_scq_acquire_contents(&o)!=PAPACC_RESULT_OK);
  CHECK(papacc_configuration_personal_scq_observe_image_data(&o,&observation)!=PAPACC_RESULT_OK&&!observation.known);
  o.stopped=1;CHECK(papacc_configuration_personal_scq_observe_runtime_data(&o,&observation)==PAPACC_RESULT_INVALID_STATE&&!observation.known&&!observation.peer.pid);o.stopped=0;
  CHECK(papacc_configuration_personal_scq_observe_runtime_data(&o,(PAPACC_SCQ_RUNTIME_OBSERVATION *)&o)==PAPACC_RESULT_INVALID_ARGUMENT);
 }
 {static PAPACC_CONFIGURATION_COMMIT held_origin;
  held_origin.writer=GetCurrentThreadId();held_origin.pending_commit.personal_native.scq_recipient.phase=4;
  held_origin.pending_commit.personal_native.scq_recipient.value.operator_token=(HANDLE)(ULONG_PTR)0x7654;
  CHECK(papacc_configuration_personal_native_close(&held_origin)==PAPACC_RESULT_INVALID_STATE&&held_origin.unresolved);
  CHECK(held_origin.pending_commit.personal_native.scq_recipient.value.operator_token==(HANDLE)(ULONG_PTR)0x7654);
 }
 (void)r;CHECK(!o.published&&!o.committed_bytes&&!o.committed_pending);
 {unsigned scenario;BYTE association[32]={1},wrong[32]={2};const WCHAR *name=L"PapinhoAccelerator.CA.11111111111111111111111111111111.22222222222222222222222222222222";
 for(scenario=0;scenario<5;scenario++){
  memset(&o,0,sizeof(o));o.writer=GetCurrentThreadId();p=&o.pending_commit;
  o.initialized=1;CHECK(papacc_configuration_personal_layout_new(&o)!=PAPACC_RESULT_OK&&!p->personal_native.install_paths.phase);o.initialized=0;
  CHECK(papacc_configuration_personal_layout_existing(&o,L"E:\\Original\\PapinhoAccelerator",L"E:\\OriginalData",L"Kept")==PAPACC_RESULT_OK);
  CHECK(papacc_configuration_personal_layout_new(&o)!=PAPACC_RESULT_OK&&!wcscmp(p->personal_native.install_paths.binary,L"E:\\Original\\PapinhoAccelerator"));
  p->personal_native.runtime_identity.thread=GetCurrentThreadId();p->personal_native.runtime_identity.phase=1;p->personal_native.runtime_identity.profile=PAPACC_PERSONAL_RUNTIME_VIRTUAL;
  p->personal_native.runtime_identity.pid=77;p->personal_native.runtime_identity.creation.dwLowDateTime=99;
  CHECK(papacc_configuration_personal_scq_prepare_data(&o,association,name)==PAPACC_RESULT_OK);
  if(scenario==4){p->personal_native.scq_inputs.phase=1;p->personal_native.scq_inputs.content_verified=1;p->personal_native.scq_admin.phase=4;p->personal_native.scq_admin.self=&p->personal_native.scq_admin;p->personal_native.scq_admin.controller=&o;p->personal_native.scq_admin.value.controller_consumed=1;CHECK(papacc_configuration_personal_scq_receive_original(&o)==PAPACC_RESULT_NOT_SUPPORTED);CHECK(papacc_configuration_personal_scq_dispatch(&o,association)==PAPACC_RESULT_NOT_SUPPORTED&&p->personal_native.scq_dispatch.gate_reason==PAPACC_SCQ_GATE_ORIGINAL_PROVENANCE_UNAVAILABLE);}
  if(scenario==0){CHECK(papacc_configuration_personal_scq_receive_original(&o)==PAPACC_RESULT_NOT_SUPPORTED);
   CHECK(p->personal_native.scq_receiver.first==PAPACC_SCQ_RECEIVE_ABSENT&&!p->personal_native.scq_receiver.original);
   CHECK(papacc_configuration_personal_scq_receive_original(&o)!=PAPACC_RESULT_OK);}
  if(scenario==0)CHECK(papacc_configuration_personal_scq_dispatch(&o,association)==PAPACC_RESULT_NOT_SUPPORTED&&p->personal_native.scq_dispatch.gate_result==PAPACC_RESULT_NOT_SUPPORTED&&p->personal_native.scq_dispatch.gate_reason==PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE);
  if(scenario==1)CHECK(papacc_configuration_personal_scq_dispatch(&o,wrong)!=PAPACC_RESULT_OK);
  if(scenario==2){o.stopped=1;CHECK(papacc_configuration_personal_scq_dispatch(&o,association)!=PAPACC_RESULT_OK&&p->personal_native.scq_dispatch.result==PAPACC_CNG_PROBE_STOPPED);}
  if(scenario==3)CHECK(papacc_configuration_personal_native_close(&o)==PAPACC_RESULT_OK&&p->personal_native.scq_dispatch.result==PAPACC_CNG_PROBE_STOPPED);
  CHECK(p->personal_native.scq_dispatch.phase==3&&!p->personal_native.runtime_cng_probe.phase&&!p->personal_native.runtime_cng_probe.provider_calls);
  CHECK(papacc_configuration_personal_scq_dispatch(&o,association)!=PAPACC_RESULT_OK);
 }}
 {static PAPACC_CONFIGURATION_COMMIT retained_driver;
  retained_driver.writer=GetCurrentThreadId();retained_driver.pending_commit.personal_native.scq_session.self=&retained_driver.pending_commit.personal_native.scq_session;
  retained_driver.pending_commit.personal_native.scq_session.owner=&retained_driver;retained_driver.pending_commit.personal_native.scq_session.thread=GetCurrentThreadId();
  retained_driver.pending_commit.personal_native.scq_session.phase=6;retained_driver.pending_commit.personal_native.scq_session.buffer[0]=0x5a;
  CHECK(papacc_configuration_personal_native_close(&retained_driver)==PAPACC_RESULT_INVALID_STATE&&retained_driver.unresolved);
  CHECK(retained_driver.pending_commit.personal_native.scq_session.phase==6&&retained_driver.pending_commit.personal_native.scq_session.buffer[0]==0x5a);
 }
 printf("PASS CA_BINDING_%s NO_KEYS_NO_AUTHORITY root=%zu pending=%zu ca=%zu\n",
#ifdef PAPACC_PERSONAL_CA_MODEL
 "MODEL",
#else
 "NATIVE_NEGATIVE",
#endif
 sizeof(o),sizeof(*p),sizeof(ca));return 0;
}
