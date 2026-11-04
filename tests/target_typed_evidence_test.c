/* Isolated existing root/native own-process observations; no LAB/SCM/network. */
#include "server_c8_private_storage_layout_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_SERVER_NETWORK network;
static PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;
static PAPACC_LOGGER logger={NULL,NULL,PAPACC_LOG_LEVEL_OFF};
static unsigned checks;
#define C(x) do{checks++;if(!(x)){printf("FAIL line=%d checks=%u\n",__LINE__,checks);return 1;}}while(0)
typedef struct TEST_THREAD {PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *owner;const PAPACC_C8_TARGET_VIEW *view;const PAPACC_C8_TARGET_BINDING *binding;} TEST_THREAD;
static DWORD WINAPI other_thread(void *arg){
 TEST_THREAD *c=(TEST_THREAD *)arg;PAPACC_C8_TARGET_VIEW v;
 PAPACC_C8_PASSIVE_SNAPSHOT snapshot,before;memset(&snapshot,0x5a,sizeof(snapshot));before=snapshot;
 return papacc_server_launcher_target_passive_snapshot(c->owner,&snapshot)==PAPACC_RESULT_INVALID_STATE&&
 !memcmp(&snapshot,&before,sizeof(snapshot))&&
 papacc_server_launcher_target_evidence_view(c->owner,&v)==PAPACC_RESULT_INVALID_STATE&&
 papacc_c8_target_view_validate(&c->owner->target_producer,c->binding,c->view,c->view->verified_ms)==PAPACC_C8_RESOURCE_STATE_ERROR?0U:1U;
}
int main(void)
{
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o=NULL;PAPACC_C8_TARGET_VIEW v,x,old;
 PAPACC_C8_TARGET_BINDING b,changed;PAPACC_U64 now,seq;HANDLE t;DWORD exit;unsigned i;PAPACC_RESULT acquire;TEST_THREAD context;
 config.listener_profile=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL;
 acquire=papacc_server_launcher_owner_acquire(&network,&config,&logger,&o);printf("OWNER_ACQUIRE=%u sizeof_owner=%zu cap=%u known=%zu\n",(unsigned)acquire,sizeof(*o),(unsigned)PAPACC_LAUNCHER_MANAGED_LIMIT,(size_t)PAPACC_C8_KNOWN_MANAGED_BYTES);C(acquire==PAPACC_RESULT_OK);
 memset(&v,0x6a,sizeof(v));old=v;
 C(papacc_server_launcher_target_evidence_view(o,&v)!=PAPACC_RESULT_OK&&!memcmp(&v,&old,sizeof(v)));
 C(papacc_server_launcher_target_refresh(o)==PAPACC_RESULT_OK);
 C(papacc_server_launcher_target_evidence_view(o,&v)==PAPACC_RESULT_OK);
 C(papacc_server_launcher_target_observation_binding(o,&b)==PAPACC_RESULT_OK);
 now=v.verified_ms;
 C(v.version==1U&&v.bytes==sizeof(v)&&v.origin==2U&&!v.policy&&v.package_backend==PAPACC_C8_VIEW_PACKAGE_OPENSSL_X64);
 C(v.provenance.process==GetCurrentProcessId()&&v.provenance.creation&&v.owner==GetCurrentThreadId());
 C(v.provenance.observed_ms==o->target_producer.evidence.sampled_ms&&v.provenance.valid_until_ms==o->target_producer.evidence.expires_ms);
 C(v.metrics[4].observed.classification==PAPACC_C8_EVIDENCE_UNKNOWN&&v.metrics[5].observed.classification==PAPACC_C8_EVIDENCE_UNKNOWN);
 C(papacc_c8_target_view_validate(&o->target_producer,&b,&v,now)==PAPACC_C8_RESOURCE_UNKNOWN);
#define BAD(field,value,result) do{x=v;x.field=(value);C(papacc_c8_target_view_validate(&o->target_producer,&b,&x,now)==(result));}while(0)
 BAD(version,2U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(bytes,sizeof(v)-1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(origin,1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(policy,1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(provider_runtime,1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(image_identity,2U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(package_backend,2U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(pointer_bytes,4U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(series,v.series+1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(first_sequence,v.first_sequence+1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(owner,v.owner+1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(provenance.process,v.provenance.process+1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(provenance.creation,v.provenance.creation+1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(provenance.sequence,v.provenance.sequence+1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(provenance.thread,v.provenance.thread+1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(provenance.revision,1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(provenance.epoch,1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(provenance.configured,1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(provenance.time_presence,3U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(provenance.clock_source,2U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(provenance.domain[0],v.provenance.domain[0]^1U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(recipe[0],v.recipe[0]^1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(provenance.sdk_inventory[0],v.provenance.sdk_inventory[0]^1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(provenance.image_path[0],L'X',PAPACC_C8_RESOURCE_STATE_ERROR);BAD(provenance.mapped_path[0],L'X',PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(verified_ms,v.provenance.valid_until_ms+1U,PAPACC_C8_RESOURCE_STALE);
 BAD(provenance.observed_ms,v.provenance.observed_ms+1U,PAPACC_C8_RESOURCE_STALE);BAD(provenance.valid_until_ms,v.provenance.valid_until_ms+1U,PAPACC_C8_RESOURCE_STALE);
 C(papacc_c8_target_view_validate(&o->target_producer,&b,&v,v.provenance.valid_until_ms+1U)==PAPACC_C8_RESOURCE_STALE);
 C(papacc_c8_target_view_validate(&o->target_producer,&b,&v,now-1U)==PAPACC_C8_RESOURCE_STALE);
 for(i=0;i<PAPACC_C8_RESOURCE_METRICS;i++){
 BAD(metrics[i].unit,99U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(metrics[i].scope,99U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(metrics[i].observed.kind,99U,PAPACC_C8_RESOURCE_STATE_ERROR);BAD(metrics[i].observed.value,v.metrics[i].observed.value+1U,PAPACC_C8_RESOURCE_STATE_ERROR);
 BAD(metrics[i].observed.classification,99U,PAPACC_C8_RESOURCE_STATE_ERROR);
 }
 changed=b;changed.epoch++;C(papacc_c8_target_view_validate(&o->target_producer,&changed,&v,now)==PAPACC_C8_RESOURCE_STATE_ERROR);
 changed=b;changed.stopping=1U;C(papacc_c8_target_view_validate(&o->target_producer,&changed,&v,now)==PAPACC_C8_RESOURCE_STATE_ERROR);
 seq=o->target_producer.sequence;o->target_producer.sequence++;C(papacc_c8_target_view_validate(&o->target_producer,&b,&v,now)==PAPACC_C8_RESOURCE_STATE_ERROR);o->target_producer.sequence=seq;
 old=v;
 o->target_producer.errors[2]=ERROR_ACCESS_DENIED;C(papacc_server_launcher_target_evidence_view(o,&v)!=PAPACC_RESULT_OK&&!memcmp(&v,&old,sizeof(v)));o->target_producer.errors[2]=0U;
 o->target_producer.evidence.origin=1U;C(papacc_server_launcher_target_evidence_view(o,&v)!=PAPACC_RESULT_OK&&!memcmp(&v,&old,sizeof(v)));o->target_producer.evidence.origin=2U;
 o->target_producer.history_overflow=1U;C(papacc_server_launcher_target_evidence_view(o,&v)!=PAPACC_RESULT_OK&&!memcmp(&v,&old,sizeof(v)));o->target_producer.history_overflow=0U;
 o->target_producer.evidence.metrics[4].classification=PAPACC_C8_BOUNDED_BY_CONTRACT;C(papacc_server_launcher_target_evidence_view(o,&v)!=PAPACC_RESULT_OK&&!memcmp(&v,&old,sizeof(v)));C(papacc_c8_target_view_validate(&o->target_producer,&b,&v,now)==PAPACC_C8_RESOURCE_STATE_ERROR);o->target_producer.evidence.metrics[4].classification=PAPACC_C8_EVIDENCE_UNKNOWN;
 C(papacc_c8_target_view(&o->target_producer,&b,(PAPACC_C8_TARGET_VIEW *)&b)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(papacc_server_launcher_target_evidence_view(o,(PAPACC_C8_TARGET_VIEW *)o)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(papacc_c8_target_view(&o->target_producer,&b,(PAPACC_C8_TARGET_VIEW *)&o->target_producer)==PAPACC_RESULT_INVALID_ARGUMENT);
 context.owner=o;context.view=&v;context.binding=&b;
 for(i=0;i<8U;i++){t=CreateThread(NULL,0,other_thread,&context,0,NULL);C(t!=NULL);C(WaitForSingleObject(t,5000)==WAIT_OBJECT_0);C(GetExitCodeThread(t,&exit)&&exit==0);C(CloseHandle(t));}
 o->secure_io.dispatch_active=PAPACC_TRUE;C(papacc_server_launcher_target_evidence_view(o,&x)==PAPACC_RESULT_INVALID_STATE);o->secure_io.dispatch_active=PAPACC_FALSE;
 o->resource_admission.stopping=1U;old=v;C(papacc_server_launcher_target_evidence_view(o,&v)!=PAPACC_RESULT_OK&&!memcmp(&v,&old,sizeof(v)));o->resource_admission.stopping=0U;
 {
 PAPACC_C8_PASSIVE_SNAPSHOT snap,unchanged;PAPACC_C8_TARGET_PRODUCER producer;
 PAPACC_C8_TARGET_BINDING current;PAPACC_U64 original_expiry=o->target_producer.evidence.expires_ms;
 producer=o->target_producer;seq=o->target_producer.sequence;
 C(papacc_server_launcher_target_passive_snapshot(o,&snap)==PAPACC_RESULT_OK);
 C(snap.version==1U&&snap.bytes==sizeof(snap)&&!snap.policy&&!snap.capture_incomplete);
 C(!memcmp(&snap.original,&producer,sizeof(producer)));
 C(snap.original.evidence.expires_ms==original_expiry&&o->target_producer.sequence==seq);
 C(papacc_c8_target_admission(&o->target_producer,&b,o->target_producer.evidence.target,now)!=PAPACC_C8_RESOURCE_OK);
 o->network.is_active=PAPACC_TRUE;
 C(papacc_server_launcher_target_passive_snapshot(o,&snap)==PAPACC_RESULT_OK);
 C(snap.current_binding.phase==PAPACC_C8_OBSERVATION_ACTIVE&&!snap.capture_matches_current);
 C(!memcmp(&snap.original,&producer,sizeof(producer)));
 old=v;C(papacc_server_launcher_target_evidence_view(o,&v)==PAPACC_RESULT_INVALID_STATE&&!memcmp(&v,&old,sizeof(v)));
 o->network.is_active=PAPACC_FALSE;o->resource_admission.stopping=1U;
 C(papacc_server_launcher_target_passive_snapshot(o,&snap)==PAPACC_RESULT_OK&&snap.current_binding.stopping);
 C(snap.original.evidence.expires_ms==original_expiry&&!snap.policy);
 o->resource_admission.stopping=0U;o->target_producer.history_overflow=1U;
 C(papacc_server_launcher_target_passive_snapshot(o,&snap)==PAPACC_RESULT_OK&&snap.capture_incomplete&&snap.original.history_overflow);
 o->target_producer.history_overflow=0U;
 unchanged=snap;o->secure_io.dispatch_active=PAPACC_TRUE;
 C(papacc_server_launcher_target_passive_snapshot(o,&snap)==PAPACC_RESULT_INVALID_STATE&&!memcmp(&snap,&unchanged,sizeof(snap)));
 o->secure_io.dispatch_active=PAPACC_FALSE;
 C(papacc_server_launcher_target_passive_snapshot(o,(PAPACC_C8_PASSIVE_SNAPSHOT*)o)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(papacc_c8_target_passive_copy(&producer,&b,(PAPACC_C8_PASSIVE_SNAPSHOT*)&producer)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(papacc_c8_target_passive_copy(&producer,&b,(PAPACC_C8_PASSIVE_SNAPSHOT*)&b)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(papacc_server_launcher_target_observation_binding(o,&current)==PAPACC_RESULT_OK);
 current.epoch++;
 C(papacc_c8_target_passive_copy(&producer,&current,&snap)==PAPACC_RESULT_OK&&!snap.capture_matches_current&&!snap.policy);
 C(papacc_c8_target_passive_copy(NULL,&current,&snap)==PAPACC_RESULT_INVALID_ARGUMENT);
 C(!memcmp(&o->target_producer,&producer,sizeof(producer)));
 printf("PASSIVE_SNAPSHOT bytes=%zu producer=%zu; NO_PERSISTENT_STORAGE_NO_OBSERVATION_NO_CREDIT\n",sizeof(snap),sizeof(producer));
 }
 C(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)==PAPACC_RESULT_OK);
 C(papacc_server_launcher_target_evidence_view(o,&v)!=PAPACC_RESULT_OK);
 printf("TYPED_TARGET_EVIDENCE=PASS checks=%u view_bytes=%zu metric_bytes=%zu producer_bytes=%zu owner_bytes=%zu; NATIVE_OWN_PROCESS_ONLY; POLICY_ABSENT; CAPACITY_UNKNOWN\n",checks,sizeof(v),sizeof(v.metrics[0]),sizeof(PAPACC_C8_TARGET_PRODUCER),sizeof(*o));return 0;
}
