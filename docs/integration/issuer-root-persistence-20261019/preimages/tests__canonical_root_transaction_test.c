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
 sprintf_s(relative,sizeof(relative),"build/phase6-canonical-consolidation-20261018/root-tx-%lu-%s.bin",GetCurrentProcessId(),abort_case?"abort":"commit");
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

