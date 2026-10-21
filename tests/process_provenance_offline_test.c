/* Real own-process/root baseline observation. No socket/wait-set/TLS/auth. */
#include "server_c8_private_storage_layout_win32.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static PAPACC_SERVER_NETWORK network;
static PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;
static PAPACC_LOGGER logger={NULL,NULL,PAPACC_LOG_LEVEL_OFF};
static void path_json(const WCHAR*p){unsigned i;putchar('"');for(i=0;p[i];++i)printf("\\u%04x",(unsigned)p[i]);putchar('"');}
int main(void){
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32*o=NULL;PAPACC_C8_PROCESS_PROVENANCE p,prior;
 PAPACC_C8_TARGET_BINDING b;PAPACC_U32 saved;PAPACC_U64 expiration;unsigned i;
 config.listener_profile=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL;
 CHECK(papacc_server_launcher_owner_acquire(&network,&config,&logger,&o)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_target_process_provenance(o,&p)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_target_refresh(o)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_target_process_provenance(o,&p)==PAPACC_RESULT_OK);
 CHECK(p.process==GetCurrentProcessId()&&p.thread==GetCurrentThreadId()&&p.creation&&p.invocation==o->invocation);
 CHECK(!p.current&&!p.configured&&!p.epoch&&!p.revision);
 CHECK(p.time_presence==7U&&p.clock_source==1U);
 CHECK(p.observed_ms==o->target_producer.evidence.sampled_ms&&p.valid_until_ms==o->target_producer.evidence.expires_ms);
 CHECK(p.observed_ms<=p.validated_ms&&p.validated_ms<=p.valid_until_ms);
 prior=p;CHECK(papacc_server_launcher_target_observation_binding(o,&b)==PAPACC_RESULT_OK);
 ++b.invocation;CHECK(papacc_c8_target_process_provenance(&o->target_producer,&b,&p)!=PAPACC_RESULT_OK&&!memcmp(&p,&prior,sizeof(p)));--b.invocation;
 saved=o->target_producer.process;o->target_producer.process++;CHECK(papacc_server_launcher_target_process_provenance(o,&p)!=PAPACC_RESULT_OK);o->target_producer.process=saved;
 saved=o->target_producer.evidence.origin;o->target_producer.evidence.origin=1U;CHECK(papacc_server_launcher_target_process_provenance(o,&p)!=PAPACC_RESULT_OK);o->target_producer.evidence.origin=saved;
 ++o->target_producer.creation;CHECK(papacc_server_launcher_target_process_provenance(o,&p)!=PAPACC_RESULT_OK);--o->target_producer.creation;
 expiration=o->target_producer.evidence.expires_ms;o->target_producer.evidence.expires_ms=0U;CHECK(papacc_server_launcher_target_process_provenance(o,&p)!=PAPACC_RESULT_OK);o->target_producer.evidence.expires_ms=expiration;
 o->resource_admission.stopping=1U;CHECK(papacc_server_launcher_target_process_provenance(o,&p)!=PAPACC_RESULT_OK);o->resource_admission.stopping=0U;
 o->secure_io.dispatch_active=PAPACC_TRUE;CHECK(papacc_server_launcher_target_process_provenance(o,&p)!=PAPACC_RESULT_OK);o->secure_io.dispatch_active=PAPACC_FALSE;
 CHECK(papacc_server_launcher_target_process_provenance(o,(PAPACC_C8_PROCESS_PROVENANCE*)o)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_target_process_provenance(o,&p)==PAPACC_RESULT_OK);
 printf("{\"format\":\"C8_OWN_PROCESS_PROVENANCE\",\"version\":1,\"origin\":\"NATIVE\",\"process\":%u,\"creation\":%llu,\"thread\":%u,\"invocation\":%llu,\"sequence\":%llu,\"revision\":%llu,\"epoch\":%llu,\"configured\":%u,\"current\":%u,\"phase\":%u,\"build_debug\":%u,\"sdk_inventory\":\"",p.process,(unsigned long long)p.creation,p.thread,(unsigned long long)p.invocation,(unsigned long long)p.sequence,(unsigned long long)p.revision,(unsigned long long)p.epoch,p.configured,p.current,p.phase,p.build_debug);
 for(i=0;i<32U;++i)printf("%02x",p.sdk_inventory[i]);
 printf("\",\"timestamps\":{\"observed_ms\":%llu,\"validated_ms\":%llu,\"valid_until_ms\":%llu,\"clock\":{\"source\":\"PAL_MONOTONIC_NS_TO_MS\",\"process\":%u,\"creation\":%llu},\"recorded_validity\":{\"producer\":\"PAPACC_C8_TARGET_EVIDENCE\",\"sequence\":%llu,\"invocation\":%llu},\"mode_c_policy_ref\":null}",(unsigned long long)p.observed_ms,(unsigned long long)p.validated_ms,(unsigned long long)p.valid_until_ms,p.process,(unsigned long long)p.creation,(unsigned long long)p.sequence,(unsigned long long)p.invocation);printf(",\"image_path\":");path_json(p.image_path);printf(",\"mapped_path\":");path_json(p.mapped_path);
 printf(",\"ed11\":{\"private\":%llu,\"baseline\":%llu,\"peak\":%llu,\"handles\":%llu,\"physical\":%llu,\"commit\":%llu},\"provenance_size\":%llu,\"provider\":\"UNKNOWN\",\"kernel\":\"UNKNOWN\",\"admission\":\"DENY\"}\n",(unsigned long long)o->target_producer.evidence.metrics[2].value,(unsigned long long)o->target_producer.evidence.baseline,(unsigned long long)o->target_producer.evidence.peak,(unsigned long long)o->target_producer.evidence.metrics[3].value,(unsigned long long)o->target_producer.evidence.metrics[0].value,(unsigned long long)o->target_producer.evidence.metrics[1].value,(unsigned long long)sizeof(p));
 CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)==PAPACC_RESULT_OK);
 return 0;
}