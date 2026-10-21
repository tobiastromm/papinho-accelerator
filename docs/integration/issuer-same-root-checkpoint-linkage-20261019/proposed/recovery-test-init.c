/* Local administrative transaction only. No wait-set, socket or authentication. */
#include "server_c8_private_storage_layout_win32.h"
#include <stdio.h>
#include <string.h>
extern PAPACC_RESULT papacc_server_launcher_c2_deadline(const PAPACC_C2_SESSION *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U64 *);
static PAPACC_NETWORK_INTERFACE interfaces[32];
static PAPACC_NETWORK_INTERFACE_ADDRESS addresses[128];
static char presentation[8192];
static PAPACC_SERVER_NETWORK network;
static PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;
static PAPACC_LOGGER logger={NULL,NULL,PAPACC_LOG_LEVEL_OFF};
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line=%u expression=%s\n",(unsigned)__LINE__,#x);return 1;}}while(0)
/* Real same-file TxF checkpoint; synthetic issuer metadata, no signing. */
static int checkpoint_roundtrip(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const char *path)
{
 static PAPACC_U8 disk[16896],old[16896],bad[512];static int have_old;
 static PAPACC_CONFIGURATION_COMMIT recovered;
 PAPACC_ISSUER_LINK link,parsed;PAPACC_ISSUER_CHECKPOINT_REPORT report;
 PAPACC_ISSUER_CAS_RESULT outcome;PAPACC_RESULT result;HANDLE f;DWORD n,start;
 PAPACC_U64 r=o->configuration_preparation.model.revision;
 PAPACC_U64 a=o->configuration_commit.slots[o->configuration_commit.current].context_epoch;
 PAPACC_U64 p=o->configuration_preparation.current_policy_epoch;unsigned checks=0;
 CHECK(papacc_configuration_checkpoint_data07(&o->configuration_commit,&link)!=PAPACC_RESULT_NOT_SUPPORTED);
 CHECK(papacc_server_launcher_configuration_checkpoint_begin(o)==0);
 CHECK(papacc_server_launcher_configuration_checkpoint_begin(o)!=0);
 CHECK(papacc_server_launcher_configuration_current_pin(o,&o->session_revision_pins[3])!=0);
 start=GetTickCount();do{result=papacc_server_launcher_configuration_checkpoint_poll(o,&outcome);
  if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);CHECK(GetTickCount()-start<10000U);
 }while(outcome==PAPACC_COMMIT_PENDING);
 CHECK(result==0&&outcome==PAPACC_CAS_SUCCESS&&o->configuration_commit.committed_bytes==16896);
 CHECK(o->configuration_preparation.model.revision==r&&o->configuration_preparation.current_policy_epoch==p);
 CHECK(papacc_configuration_checkpoint_data07(&o->configuration_commit,&link)==PAPACC_RESULT_NOT_SUPPORTED);
 CHECK(link.revision==r&&link.predecessor==r-1&&link.administrative_epoch==a&&link.policy_epoch==p);
 CHECK(papacc_server_launcher_configuration_checkpoint_begin(o)!=0); /* no replay */
 f=CreateFileA(path,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);CHECK(f!=INVALID_HANDLE_VALUE);
 CHECK(ReadFile(f,disk,sizeof(disk),&n,NULL)&&n==sizeof(disk));CHECK(CloseHandle(f));
 CHECK(!memcmp(disk,o->configuration_commit.committed,sizeof(disk)));
 CHECK(papacc_issuer_link_decode(disk+16384,512,disk,16384,&parsed,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
 CHECK(parsed.revision==r&&parsed.administrative_epoch==a&&parsed.policy_epoch==p);
 CHECK(papacc_issuer_link_decode(disk+16384,511,disk,16384,&parsed,papacc_configuration_issuer_hash06,&o->configuration_commit)==PAPACC_ISSUER_INVALID);
 memcpy(bad,disk+16384,512);bad[64]^=1;
 CHECK(papacc_issuer_link_decode(bad,512,disk,16384,&parsed,papacc_configuration_issuer_hash06,&o->configuration_commit)==PAPACC_ISSUER_INVALID);
 memcpy(bad,disk+16384,512);memset(bad+72,0,8);
 CHECK(papacc_configuration_issuer_hash06(&o->configuration_commit,bad,480,bad+480));
 CHECK(papacc_issuer_link_decode(bad,512,disk,16384,&parsed,papacc_configuration_issuer_hash06,&o->configuration_commit)==PAPACC_ISSUER_INVALID);
 if(have_old){
  CHECK(papacc_issuer_link_decode(old+16384,512,disk,16384,&parsed,papacc_configuration_issuer_hash06,&o->configuration_commit)==PAPACC_ISSUER_INVALID);
  CHECK(papacc_issuer_link_decode(disk+16384,512,old,16384,&parsed,papacc_configuration_issuer_hash06,&o->configuration_commit)==PAPACC_ISSUER_INVALID);
  CHECK(papacc_issuer_checkpoint_assess(old+8192,8192,disk+8192,8192,papacc_configuration_issuer_hash06,&o->configuration_commit,0,&report)==PAPACC_ISSUER_CONFLICT);
  CHECK(report.state==PAPACC_ISSUER_CHECKPOINT_ROLLBACK);
 }
 memcpy(old,disk,sizeof(old));have_old=1;
 /* Recovery has an exclusive data read lease, never live CURRENT/P/A. */
 memset(&recovered,0,sizeof(recovered));CHECK(papacc_configuration_recovery_init02(&recovered,path)==0);
 CHECK(papacc_configuration_issuer_recovery06(&recovered,path)==PAPACC_RESULT_NOT_SUPPORTED);
 CHECK(recovered.recovery_selected&&!recovered.published&&!recovered.slots[0].context_epoch&&!recovered.slots[0].operational_epoch);
 CHECK(papacc_configuration_checkpoint_data07(&recovered,&parsed)==PAPACC_RESULT_NOT_SUPPORTED&&parsed.administrative_epoch==a&&parsed.policy_epoch==p);
 CHECK(papacc_configuration_checkpoint_begin07(&recovered)!=0);
 CHECK(papacc_configuration_stop(&recovered)==0&&papacc_configuration_finish(&recovered)==0);
 printf("CHECKPOINT_ROUNDTRIP R=%llu A_PRESENT P_MATCH high_water=%llu checks=%u RECOVERY_DENY\n",(unsigned long long)r,(unsigned long long)o->configuration_commit.committed[8192+175],checks);
 return 0;
}
int main(int argc,char **argv)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=NULL;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
 PAPACC_NETWORK_DISCOVERY_SNAPSHOT snapshot;PAPACC_SIZE ir,ar,pr;
 PAPACC_ROOT_CURRENT_PIN pin;PAPACC_RESULT result;unsigned checks=0,i;
 char path[MAX_PATH],relative[MAX_PATH];PAPACC_BOOL abort_case=argc>1&&!strcmp(argv[1],"abort");
 PAPACC_BOOL retain_case=argc>1&&(!strcmp(argv[1],"retain")||!strcmp(argv[1],"catalog-retain"));
 PAPACC_BOOL catalog_case=argc>1&&(!strncmp(argv[1],"catalog",7));
 PAPACC_SITE_TLS_CATALOG selected;FILETIME ft;ULARGE_INTEGER clock;
 config.control_port=4433U;config.listener_profile=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL;
 config.bind_selection.mode=PAPACC_BIND_SELECTION_ALL_INTERFACES;
 config.security_configuration_ref_valid=PAPACC_TRUE;config.security_configuration_ref_length=15U;
 memcpy(config.security_configuration_ref,"offline-runtime",15U);
 config.policy.present=PAPACC_TRUE;config.policy.representation=PAPACC_ADMIN_REPRESENTATION_04;
 config.policy.identity[0]=0xc8;config.policy.profile=1;config.policy.listener_port=4433;
 config.policy.enabled_count=2;config.policy.rule_count=2;
 for(i=0;i<2;++i){config.policy.enabled[i].id=2U+i;config.policy.enabled[i].major=1;
 config.policy.rules[i].key=config.policy.enabled[i];config.policy.rules[i].action=PAPACC_ADMIN_POLICY_DENY;}
 if(catalog_case){
 PAPACC_SITE_TLS_CATALOG *c=&config.site_tls;memset(c,0,sizeof(*c));
 GetSystemTimeAsFileTime(&ft);clock.LowPart=ft.dwLowDateTime;clock.HighPart=ft.dwHighDateTime;
 c->present=c->version=c->trust_system=c->hostname_required=c->sni_required=c->tls13_only=1;
 c->identity[0]=1;c->profile_identity[0]=2;c->administrative_origin[0]=3;
 c->revision=c->profile_revision=1;c->reference=7;c->max_concurrent=1;
 c->not_before_utc_ms=clock.QuadPart/10000U-1000U;c->expires_utc_ms=clock.QuadPart/10000U+60000U;
 c->max_bytes=65536;c->max_duration_ms=1000;c->alpn_length=9;c->alpn[0]=8;memcpy(c->alpn+1,"http/1.1",8);
 }
 {PAPACC_U64 denied_deadline=123U;PAPACC_BOOL permit=PAPACC_TRUE;
 CHECK(papacc_server_launcher_c2_deadline(NULL,NULL,0U,&denied_deadline)!=PAPACC_RESULT_OK&&!denied_deadline);
 CHECK(papacc_server_launcher_c2_capacity(NULL,0U,&permit)!=PAPACC_RESULT_OK&&!permit);}
 CHECK(papacc_server_config_validate(&config)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_owner_acquire(&network,&config,&logger,&o)==PAPACC_RESULT_OK);
 t=&o->configuration_preparation;memset(&pin,0,sizeof(pin));
 CHECK(papacc_server_launcher_configuration_current_pin(o,&pin)!=PAPACC_RESULT_OK&&!pin.epoch);
 CHECK(papacc_server_launcher_configuration_transaction_publish(o)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_apply_dormant(o)!=PAPACC_RESULT_OK);
 CHECK(SetEnvironmentVariableA("PAPACC_SECURITY_CONFIGURATION_REF","offline-runtime"));
 CHECK(SetEnvironmentVariableA("PAPACC_SERVER_CERTIFICATE_DER_B64","tests/fixtures/security/server.der.b64"));
 CHECK(SetEnvironmentVariableA("PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64","tests/fixtures/security/server.pk8.b64"));
 CHECK(SetEnvironmentVariableA("PAPACC_CLIENT_CA_DER_B64","tests/fixtures/security/ca.der.b64"));
 /* Enrollment fixture, never a Principal or authentication result. */
 CHECK(GetEnvironmentVariableA("PAPACC_ALLOWED_CLIENT_CERT_SHA256",path,sizeof(path))==64);
 CHECK(papacc_server_launcher_configuration_prepare_source(o,131072U,32768U)==PAPACC_RESULT_NOT_SUPPORTED);
 CHECK(papacc_server_launcher_configuration_prepare_runtime(o)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_prepare_provider(o)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_policy_prepare_model(o)==PAPACC_RESULT_OK);
 CHECK(!t->current_policy_epoch&&!t->administrative_published&&!t->normative_committed);
 result=papacc_network_discover_local_snapshot(interfaces,32,addresses,128,presentation,sizeof(presentation),&snapshot,&ir,&ar,&pr);
 CHECK(result==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_prepare_listener(o,&snapshot)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_prepare_capabilities(o)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_apply_dormant(o)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_prepare_c2(o)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_apply_dormant(o)==PAPACC_RESULT_OK);
 CHECK(!t->current_policy_epoch&&!t->normative_committed&&!t->administrative_published);
 {PAPACC_BOOL policy=0,site=0;CHECK(papacc_server_launcher_dispatch_preflight(o,&policy,&site)==PAPACC_RESULT_OK&&policy&&site==catalog_case);}
 printf("COMBINED_RESERVED=%llu MATERIAL_CREDIT=%llu RUNTIME_CREDIT=%llu\n",(unsigned long long)t->combined_reserved,(unsigned long long)t->material_credit,(unsigned long long)t->runtime_credit);
 CHECK(t->combined_reserved<=2097152U);
 CHECK(papacc_server_launcher_configuration_transaction_publish(o)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)!=PAPACC_RESULT_OK);
 if(argc>2)strcpy_s(relative,sizeof(relative),argv[2]);
 else if(argc>1&&!strncmp(argv[1],"catalog-live",12))sprintf_s(relative,sizeof(relative),"build/issuer-session-live-publication-20261019/root-%lu-%s.bin",GetCurrentProcessId(),argv[1]);
 else if(argc>1&&!strcmp(argv[1],"catalog-issuer06"))sprintf_s(relative,sizeof(relative),"build/issuer-live-root-recovery-20261019/app-root-%lu-issuer06.bin",GetCurrentProcessId());
 else sprintf_s(relative,sizeof(relative),"build/phase6-canonical-consolidation-20261018/root-tx-%lu-%s.bin",GetCurrentProcessId(),abort_case?"abort":"commit");
 CHECK(GetFullPathNameA(relative,sizeof(path),path,NULL)>0U);
 CHECK(GetFileAttributesA(path)==INVALID_FILE_ATTRIBUTES&&GetLastError()==ERROR_FILE_NOT_FOUND);
 if(catalog_case){
  pst_trust *held=o->pst_system_trust;PAPACC_RESULT missing_trust;
  CHECK(held!=NULL);
  /* Negative-only invalidation; keep actual ownership locally and restore
   * before assertions. No PST call, callback or cleanup during this interval. */
  o->pst_system_trust=NULL;
  missing_trust=papacc_server_launcher_configuration_transaction_prepare_store(o,path,0);
  o->pst_system_trust=held;
  CHECK(missing_trust!=PAPACC_RESULT_OK&&!o->configuration_commit.initialized);
 }
 CHECK(papacc_server_launcher_configuration_transaction_prepare_store(o,path,0)==PAPACC_RESULT_OK);
 if(argc>1&&!strcmp(argv[1],"catalog-fail")){
 HANDLE collision=CreateFileA(path,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);DWORD n;
 CHECK(collision!=INVALID_HANDLE_VALUE);
 CHECK(WriteFile(collision,"COMMIT_COLLISION_ONLY",21,&n,NULL)&&n==21&&CloseHandle(collision));
 CHECK(papacc_server_launcher_configuration_transaction_commit(o)!=PAPACC_RESULT_OK);
 CHECK(!t->normative_committed&&!t->current_policy_epoch&&!t->administrative_published);
 CHECK(papacc_server_launcher_configuration_transaction_publish(o)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_transaction_abort(o)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)==PAPACC_RESULT_OK);
 printf("PASS case=commit_collision checks=%u NO_NETWORK NO_ADMISSION\n",checks);return 0;
 }
 if(abort_case){
 CHECK(papacc_server_launcher_configuration_transaction_abort(o)==PAPACC_RESULT_OK);
 CHECK(GetFileAttributesA(path)==INVALID_FILE_ATTRIBUTES);
 }else{
 result=papacc_server_launcher_configuration_transaction_commit(o);
 printf("NATIVE_LOCAL_COMMIT_RESULT=%d\n",(int)result);
 CHECK(result==PAPACC_RESULT_OK);
 CHECK(t->normative_committed&&!t->current_policy_epoch&&!t->administrative_published);
 if(catalog_case)CHECK(!memcmp(&o->configuration_commit.slots[o->configuration_commit.pending].view.config.site_tls,&config.site_tls,sizeof(config.site_tls)));
 CHECK(GetFileAttributesA(path)!=INVALID_FILE_ATTRIBUTES);
 CHECK(papacc_server_launcher_configuration_current_pin(o,&pin)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_transaction_publish(o)==PAPACC_RESULT_OK);
 CHECK(t->current_policy_epoch&&t->administrative_published);
 CHECK(papacc_server_launcher_configuration_current_pin(o,&pin)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_current(o,&pin));
 if(catalog_case){
 PAPACC_ROOT_CURRENT_PIN wrong=pin;
 CHECK(o->configuration_commit.store02==5);
 CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)==PAPACC_RESULT_OK&&selected.profile_identity[0]==2);
 CHECK(!memcmp(&selected,&config.site_tls,sizeof(selected)));
 CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&o->config.site_tls)==PAPACC_RESULT_INVALID_ARGUMENT);
 CHECK(!memcmp(&o->config.site_tls,&config.site_tls,sizeof(config.site_tls)));
 CHECK(papacc_server_launcher_site_profile(o,&pin,8,1,&selected)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_site_profile(o,&pin,7,2,&selected)!=PAPACC_RESULT_OK);
 wrong.epoch^=1;
 CHECK(papacc_server_launcher_site_profile(o,&wrong,7,1,&selected)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_c2_site_template(NULL,7,NULL,&selected)!=PAPACC_RESULT_OK);
 }
 CHECK(papacc_server_launcher_listener_publication_preflight(o)!=PAPACC_RESULT_OK);
 CHECK(!o->network.is_active&&!o->io_loop.initialized&&!o->secure_io.initialized);
 if(argc>1&&!strncmp(argv[1],"catalog-live",12)){
  static PAPACC_U8 image[8192],digest[32],old_image[8192];PAPACC_ISSUER_VIEW iv;PAPACC_ISSUER_RECORD record;
  PAPACC_ROOT_CURRENT_PIN fresh,third,bad;PAPACC_ISSUER_CAS_RESULT outcome;
  PAPACC_U64 original_p=t->current_policy_epoch,original_a=pin.administrative_epoch;DWORD started;
  PAPACC_SERVER_CONFIG changed=config;PAPACC_SERVER_C2_OWNER_WIN32 *consumer;
  PAPACC_C2_SESSION *older,*newer;HANDLE held=INVALID_HANDLE_VALUE;unsigned tries=0;
  int fail_conflict=!strcmp(argv[1],"catalog-live-conflict"),fail_unknown=!strcmp(argv[1],"catalog-live-unknown"),stop_case=(!strcmp(argv[1],"catalog-live-stop")||!strcmp(argv[1],"catalog-live-late-stop")),proof_change=!strcmp(argv[1],"catalog-live-proof-change");
  memset(&fresh,0,sizeof(fresh));memset(&third,0,sizeof(third));memset(&iv,0,sizeof(iv));memset(&record,0,sizeof(record));
  memcpy(iv.root,o->configuration_commit.domain,32);memcpy(iv.policy,config.policy.identity,32);
  iv.authority[0]=1;iv.generation[0]=2;iv.prefix[0]=3;iv.ca_spki_digest[0]=4;iv.revision=2;iv.high_water=1;iv.ca_not_after=1000000;
  CHECK(papacc_configuration_issuer_hash06(&o->configuration_commit,o->configuration_commit.committed,8192,digest));
  CHECK(papacc_issuer_image_init(image,&iv,digest,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
  CHECK(pin.epoch==original_p&&original_a&&pin.view.revision==1&&!pin.issuer_present);
  /* Existing normal runtime/wait-set attachment; no listener or Connection. */
  CHECK(papacc_server_launcher_attach_closed(o,t->combined_reserved+4096U)==0);
  consumer=o->io_loop.c2_owner;CHECK(consumer&&consumer->initialized&&consumer->manager->initialized);
  older=&consumer->sessions[0];newer=&consumer->sessions[1];
  /* Synthetic REVOKED lifecycle metadata; not authentication or a grant.
   * Real root pin acquired before exposing the simulated Session reference. */
  CHECK(papacc_server_launcher_configuration_current_pin(o,&o->session_revision_pins[0])==0);
  o->session_revision_pins[0].session_incarnation=55;
  older->owner=consumer->manager->owner;older->incarnation=55;older->initialized=PAPACC_TRUE;
  older->state=PAPACC_C2_SESSION_REVOKED;older->root_bound=PAPACC_TRUE;older->root_invocation=o->invocation;
  older->root_revision=1;older->policy_epoch=original_p;older->control_connection=700;
  memcpy(older->root_domain,pin.view.domain,32);consumer->manager->sessions[0].session=older;
  CHECK(!papacc_server_launcher_c2_session_current(older));
  changed.control_port++;CHECK(!papacc_configuration_issuer_projection06(&config,&changed));
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&changed,1,1)!=0);
  changed=config;changed.policy.identity[1]^=1;CHECK(!papacc_configuration_issuer_projection06(&config,&changed));
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&changed,1,1)!=0);
  changed=config;changed.site_tls.max_bytes++;CHECK(!papacc_configuration_issuer_projection06(&config,&changed));
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&changed,1,1)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,0,1)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,1,1)==0);
  CHECK(papacc_configuration_issuer_publish06(&o->configuration_commit)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,1,1)!=0);
  if(fail_conflict){DWORD n;unsigned char v=0;held=CreateFileA(path,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);CHECK(held!=INVALID_HANDLE_VALUE);CHECK(WriteFile(held,&v,1,&n,NULL)&&n==1&&FlushFileBuffers(held));CHECK(CloseHandle(held));held=INVALID_HANDLE_VALUE;}
  if(fail_unknown){held=CreateFileA(path,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);CHECK(held!=INVALID_HANDLE_VALUE);}
  CHECK(papacc_server_launcher_configuration_issuer_publish_live(o,&outcome)==0&&outcome==PAPACC_COMMIT_PENDING);
  CHECK(!o->configuration_commit.committed_pending&&t->model.revision==1&&t->current_policy_epoch==original_p);
  CHECK(papacc_server_launcher_configuration_current_pin(o,&fresh)!=0&&!fresh.pin.ticket);
  CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)!=0);
  if(!strcmp(argv[1],"catalog-live-late-stop")){started=GetTickCount();while(WaitForSingleObject(o->configuration_commit.pending_commit.thread,0)==WAIT_TIMEOUT){Sleep(1);CHECK(GetTickCount()-started<10000U);}CHECK(o->configuration_commit.pending_commit.committed);}
  if(proof_change)o->config.site_tls.max_bytes++; /* explicit negative-only operational-proof fault */
  if(stop_case)CHECK(papacc_configuration_stop(&o->configuration_commit)==0);
  started=GetTickCount();do{
   result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);tries++;
   if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);
   CHECK(GetTickCount()-started<10000U);
  }while(outcome==PAPACC_COMMIT_PENDING);
  if(fail_conflict||fail_unknown||stop_case||proof_change){
   CHECK(result!=0&&t->model.revision==1&&t->current_policy_epoch==original_p&&older->root_revision==1);
   CHECK(o->session_revision_pins[0].pin.ticket&&older->initialized);
   if(fail_conflict)CHECK(outcome==PAPACC_CAS_CONFLICT&&o->configuration_commit.authority_lost);
   if(fail_unknown){CHECK(outcome==PAPACC_CAS_UNKNOWN&&o->configuration_commit.authority_lost);CHECK(CloseHandle(held));held=INVALID_HANDLE_VALUE;}
   if(proof_change||!strcmp(argv[1],"catalog-live-late-stop"))CHECK(outcome==PAPACC_CAS_SUCCESS&&o->configuration_commit.committed_pending&&o->configuration_commit.slots[o->configuration_commit.current].context_epoch==original_a);
   CHECK(!papacc_server_launcher_configuration_current(o,&pin));
   CHECK(papacc_server_launcher_configuration_issuer_publish_live(o,&outcome)!=0);
   CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)!=0);
   printf("PASS checks=%u LIVE_FAILURE=%s outcome=%d P_UNCHANGED OLD_SESSION_RETAINED NO_CURRENT_NO_RETRY NO_NETWORK\n",checks,argv[1],(int)outcome);return 0;
  }
  CHECK(result==0&&outcome==PAPACC_CAS_SUCCESS&&t->model.revision==2&&t->current_policy_epoch==original_p);
  if(!strncmp(argv[1],"catalog-live-checkpoint-",24)){
   int conflict=!strcmp(argv[1],"catalog-live-checkpoint-conflict");
   int unknown=!strcmp(argv[1],"catalog-live-checkpoint-unknown");
   if(conflict){DWORD n;unsigned char v=0;held=CreateFileA(path,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);CHECK(held!=INVALID_HANDLE_VALUE);CHECK(WriteFile(held,&v,1,&n,NULL)&&n==1&&FlushFileBuffers(held));CHECK(CloseHandle(held));held=INVALID_HANDLE_VALUE;}
   if(unknown){held=CreateFileA(path,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);CHECK(held!=INVALID_HANDLE_VALUE);}
   CHECK(papacc_server_launcher_configuration_checkpoint_begin(o)==0);
   if(!conflict&&!unknown)CHECK(papacc_configuration_stop(&o->configuration_commit)==0);
   started=GetTickCount();do{result=papacc_server_launcher_configuration_checkpoint_poll(o,&outcome);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);CHECK(GetTickCount()-started<10000U);}while(outcome==PAPACC_COMMIT_PENDING);
   CHECK(result==0&&t->model.revision==2&&older->root_revision==1&&o->session_revision_pins[0].pin.ticket);
   if(conflict)CHECK(outcome==PAPACC_CAS_CONFLICT&&o->configuration_commit.authority_lost);
   if(unknown){CHECK(outcome==PAPACC_CAS_UNKNOWN&&o->configuration_commit.authority_lost);CHECK(CloseHandle(held));}
   CHECK(papacc_server_launcher_configuration_checkpoint_begin(o)!=0);
   CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)!=0);
   printf("PASS CHECKPOINT_FAILURE=%s outcome=%d OLD_PINS_RETAINED NO_RECOVERY\n",argv[1],(int)outcome);return 0;
  }
  CHECK(checkpoint_roundtrip(o,path)==0);
  CHECK(!papacc_server_launcher_configuration_current(o,&pin));
  CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)==0); /* held memory + independent CURRENT projection */
  CHECK(papacc_server_launcher_configuration_current_pin(o,&fresh)==0&&fresh.view.revision==2&&fresh.issuer_present&&fresh.issuer_revision==2);
  CHECK(fresh.epoch==original_p&&fresh.administrative_epoch&&fresh.administrative_epoch!=original_a);
  CHECK(older->root_revision==1&&older->policy_epoch==original_p&&older->grant_count==0);
  CHECK(papacc_server_launcher_attachment_wake(o)==0); /* old attachment R, independently CURRENT P */
  CHECK(papacc_server_launcher_configuration_current_pin(o,&o->session_revision_pins[1])==0);
  o->session_revision_pins[1].session_incarnation=56;
  newer->owner=consumer->manager->owner;newer->incarnation=56;newer->initialized=PAPACC_TRUE;newer->state=PAPACC_C2_SESSION_REVOKED;
  newer->root_bound=PAPACC_TRUE;newer->root_invocation=o->invocation;newer->root_revision=2;newer->policy_epoch=original_p;
  memcpy(newer->root_domain,fresh.view.domain,32);consumer->manager->sessions[1].session=newer;
  CHECK(!papacc_server_launcher_c2_session_current(newer));
  bad=pin;bad.epoch^=1;CHECK(papacc_server_launcher_site_profile(o,&bad,7,1,&selected)!=0);
  bad=pin;bad.administrative_epoch^=1;CHECK(papacc_server_launcher_site_profile(o,&bad,7,1,&selected)!=0);
  bad=pin;bad.pin.serial++;CHECK(papacc_server_launcher_site_profile(o,&bad,7,1,&selected)!=0);
  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&pin.pin,old_image)!=0); /* explicit absence at initial R */
  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,old_image)==0);
  record.request.id[0]=1;record.request.device[0]=7;record.request.key_digest[0]=8;record.request.csr_digest[0]=9;
  record.request.not_before=100;record.request.not_after=500;record.request.purpose=1;record.approved_epoch=original_p;record.state=PAPACC_ISSUER_PREPARED;
  memcpy(record.authority,iv.authority,16);memcpy(record.generation,iv.generation,16);memcpy(record.policy,iv.policy,32);
  CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,1,1)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,2,1)==0);
  started=GetTickCount();do{result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);CHECK(GetTickCount()-started<10000U);}while(outcome==PAPACC_COMMIT_PENDING);
  CHECK(result==0&&t->model.revision==3&&t->current_policy_epoch==original_p);
  CHECK(checkpoint_roundtrip(o,path)==0);
  CHECK(papacc_server_launcher_configuration_current_pin(o,&third)==0&&third.issuer_revision==3&&third.view.revision==3);
  CHECK(third.epoch==original_p&&third.administrative_epoch!=fresh.administrative_epoch);
  {PAPACC_U8 copy[8192];CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,copy)==0&&!memcmp(copy,old_image,8192));}
  CHECK(older->root_revision==1&&newer->root_revision==2&&newer->grant_count==0);
  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,o->configuration_commit.issuer_staged)==PAPACC_RESULT_INVALID_ARGUMENT);
  record.state=PAPACC_ISSUER_SERIAL_RESERVED;record.counter=2;memcpy(record.serial,iv.prefix,8);record.serial[15]=2;
  CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,3,1)==PAPACC_RESULT_LIMIT_EXCEEDED);
  CHECK(t->model.revision==3&&older->root_revision==1&&newer->root_revision==2);
  /* Each simulated borrower individually blocks actual reaper/unpin. Restore
   * test-only metadata only; no native resource is fabricated or released. */
  consumer->manager->operations[0].session=older;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);
  CHECK(papacc_server_c2_owner_win32_retire_sessions(consumer)==0&&older->initialized);consumer->manager->operations[0].session=NULL;
  consumer->writes[0].denial_session=older;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);consumer->writes[0].denial_session=NULL;
  {PAPACC_CHANNEL saved=o->channel_storage[0];o->channel_storage[0].state=(PAPACC_CHANNEL_STATE)1;o->channel_storage[0].session_instance_id=55;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->channel_storage[0]=saved;}
  {PAPACC_SERVER_PROTOCOL_SLOT_WIN32 saved=o->processor_storage[0];o->processor_storage[0].in_use=PAPACC_TRUE;o->processor_storage[0].connection_instance_id=700;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->processor_storage[0]=saved;}
  {PAPACC_U64 id=o->secure_candidates[0].control_close_result.session_instance_id;o->secure_candidates[0].control_close_result.session_instance_id=55;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->secure_candidates[0].control_close_result.session_instance_id=id;}
  o->session_revision_pins[0].pin.serial++;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->session_revision_pins[0].pin.serial--;
  CHECK(papacc_server_c2_owner_win32_retire_sessions(consumer)==0&&!older->initialized&&!newer->initialized);
  CHECK(!o->session_revision_pins[0].pin.ticket&&!o->session_revision_pins[1].pin.ticket);
  CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);
  CHECK(papacc_server_launcher_configuration_current_unpin(o,&fresh)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,3,1)==0); /* freed middle slot is now reusable */
  started=GetTickCount();do{result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);CHECK(GetTickCount()-started<10000U);}while(outcome==PAPACC_COMMIT_PENDING);
  CHECK(result==0&&t->model.revision==4&&t->current_policy_epoch==original_p);
  CHECK(checkpoint_roundtrip(o,path)==0); /* durable reserved high-water2 */
  CHECK(papacc_configuration_stop(&o->configuration_commit)==0);
  CHECK(papacc_server_launcher_configuration_current_unpin(o,&third)==0);
  CHECK(papacc_server_launcher_configuration_current_unpin(o,&pin)==0);
  CHECK(papacc_server_launcher_detach_closed(o)==0);
  CHECK(papacc_server_launcher_configuration_transaction_close(o)==0);
  CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)==0);
  printf("PASS checks=%u LIVE_CURRENT_A_FACTUAL P_PRESERVED THREE_REVISIONS REAPER_PINS_RELEASED SYNTHETIC_REVOKED_SESSIONS NO_NETWORK\n",checks);return 0;
 }
 if(argc>1&&!strcmp(argv[1],"catalog-issuer06")){
  static PAPACC_U8 issuer[8192],base_digest[32];PAPACC_ISSUER_VIEW iv;PAPACC_U64 original_epoch=t->current_policy_epoch;
  memset(&iv,0,sizeof(iv));memcpy(iv.root,o->configuration_commit.domain,32);memcpy(iv.policy,t->policy.configured_model.policy.identity,32);
  iv.authority[0]=1;iv.generation[0]=2;iv.prefix[0]=3;iv.ca_spki_digest[0]=4;iv.revision=2;iv.high_water=1;iv.ca_not_after=1000000;
  CHECK(papacc_configuration_issuer_hash06(&o->configuration_commit,o->configuration_commit.committed,8192,base_digest));
  CHECK(papacc_issuer_image_init(issuer,&iv,base_digest,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_dormant(o,issuer,1,1)!=0); /* outstanding pin */
  CHECK(papacc_server_launcher_configuration_current_unpin(o,&pin)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_dormant(o,issuer,1,1)==0);
  CHECK(papacc_server_launcher_configuration_issuer_commit_dormant(o)==0);
  CHECK(o->configuration_commit.store02==6&&o->configuration_commit.committed_pending&&t->current_policy_epoch==original_epoch);
  {PAPACC_ISSUER_ROOT_PORT issuer_port;PAPACC_ISSUER_VIEW observed;PAPACC_ISSUER_RECORD absent;PAPACC_U8 no_request[16]={0};int exists=1;
   CHECK(papacc_server_launcher_configuration_issuer_port(o,&issuer_port)==0&&issuer_port.ctx==o&&!issuer_port.test_only);
   CHECK(issuer_port.read(issuer_port.ctx,no_request,&observed,&absent,&exists)==0&&!exists&&!observed.proofs&&!observed.epoch&&observed.revision==2);
   CHECK(issuer_port.cas(issuer_port.ctx,&observed,NULL,&absent,1,NULL,0,1)==PAPACC_ISSUER_UNSUPPORTED);}
  CHECK(papacc_server_launcher_configuration_transaction_publish(o)!=0);CHECK(papacc_configuration_publish(&o->configuration_commit)==PAPACC_RESULT_NOT_SUPPORTED);
  CHECK(papacc_server_launcher_listener_publication_preflight(o)!=0&&!o->network.is_active&&!o->io_loop.initialized);
  CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)!=0);CHECK(papacc_server_launcher_owner_state()==PAPACC_LAUNCHER_RETAINED);
  printf("PASS checks=%u ROOT06_APPLICATION_OWNER_COMMIT=FACTUAL PUBLICATION=DENY ORIGINAL_EPOCH_PRESERVED ROOT_RETAINED path=%s\n",checks,path);return 0;
 }
 if(retain_case){
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *other=NULL;
 CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_owner_state()==PAPACC_LAUNCHER_RETAINED);
 CHECK(papacc_server_launcher_owner_acquire(&network,&config,&logger,&other)!=PAPACC_RESULT_OK&&!other);
 CHECK(o->configuration_commit.initialized&&t->source.borrowers==1U);
 printf("PASS checks=%u case=retain GRAPH_PRESERVED NO_REUSE NO_NETWORK NO_AUTHENTICATION\n",checks);
 return 0;
 }
 CHECK(papacc_server_launcher_configuration_current_unpin(o,&pin)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_transaction_close(o)==PAPACC_RESULT_OK);
 }
 CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)==PAPACC_RESULT_OK);
 printf("PASS checks=%u case=%s NO_NETWORK NO_AUTHENTICATION NO_ADMISSION\n",checks,abort_case?"abort":"commit");
 return 0;
}

