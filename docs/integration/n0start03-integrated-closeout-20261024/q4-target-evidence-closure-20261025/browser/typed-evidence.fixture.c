#include <windows.h>
#include <stdio.h>
#include <string.h>
typedef unsigned __int64 PAPACC_U64;
#define C2_NATIVE_SDK_MANIFEST "1a945de88199a8317355b924e0255d4a214fd3f849d7c793aae14c05abc4f3af"
typedef struct papinho_candidate_resource_sample {
 DWORD presence,begin_tick,end_tick,process,mapped_base,source;
 DWORD identity_error,memory_error,handles_error,capacity_error;
 PAPACC_U64 creation,private_bytes,commit_bytes,handles,os_peak_commit,available_physical,available_pagefile;
} papinho_candidate_resource_sample;
typedef struct papinho_c2_s6_facts {
 int grants, tls_ready, local_destroyed, remote_outcome, remote_ownership;
 unsigned short remote_fact; unsigned long navigation_id;
 PAPACC_U64 operation, generation, prefixes[4];
 unsigned long pst_release_attempts, site_close_attempts;
 /* Public value references only. Zero sequence means absent, never success.
  * Sequence domain is the EXISTING owner-thread first-cause record. */
 PAPACC_U64 request,epoch,fence_values[4],ack_values[4];
 unsigned long process_id,owner_thread,attempt,resolution,session_generation,group_serial;
 unsigned long open_result_seq,bind_seq[2],bind_ack_seq[2],tls_seq,prefix_seq;
 unsigned long fence_seq[4],ack_seq[4],final_seq,stop_seq,teardown_seq;
 unsigned long first_cause_seq,publication_seq;
 unsigned short tls_version,tls_verification;char selected_alpn[9];
 struct {unsigned long identity,generation,attempts;long result;
  unsigned long disposition;int quiesced,finalized,membership_absent;} release[3];
 int capture_incomplete,retained,site_closed,cancel_closed;
 int session_quiesced,auxiliary_confirmed,group_destroyed,session_finished;
 unsigned long live_borrowers,bridge_join_result;
 int stop_attempted,bridge_joined,bridge_close_succeeded,cancel_attempted,cancel_succeeded;
 /* Native OS observations, not expected image hashes or policy authority.
  * presence bits: process1/thread2/mapping4/path8/observed16/validated32/root64. */
 struct {unsigned long presence,error,mapped_base,observed_tick,validated_tick;
  unsigned long root_identity,root_state,sequence;
  PAPACC_U64 creation,thread_creation;WCHAR image_path[MAX_PATH];} native;
 /* Fixed first sample at each reached boundary, not continuous surveillance.
  * Sample presence: identity1/private2/commit4/handles8/OS peak16/
  * available physical32/available pagefile64/nonwrapping local interval128. */
 struct {papinho_candidate_resource_sample sample;
  unsigned long witness;int retained,stopped,teardown;} resource[8];
 unsigned long resource_scope,resource_mask;
} papinho_c2_s6_facts;
static int c2_resource_identity(const papinho_c2_s6_facts *f,const papinho_candidate_resource_sample *r){
 return (f->native.presence&5U)==5U && (r->presence&1U) &&
  r->process==f->process_id && r->creation==f->native.creation && r->mapped_base==f->native.mapped_base;
}
#include "C:/Projetos/PapinhoBrowser/src/papinho_c2_target_evidence.inc"
/* Borrow only the original first-reached slot. No resampling or expiry renewal. */
static int c2_resource_view(const papinho_c2_s6_facts *f,unsigned phase,unsigned metric,C2_TARGET_VIEW *v){
 const papinho_candidate_resource_sample *r;
 ZeroMemory(v,sizeof(*v));if(phase>=8U || metric>5U || !(f->resource_mask&(1UL<<phase)))return 0;
 r=&f->resource[phase].sample;v->version=1U;v->bytes=sizeof(*v);v->origin=1U;v->clock=1U;
 v->architecture=32U;v->backend=1U;strcpy(v->sdk_sha256,C2_NATIVE_SDK_MANIFEST);
 v->metric=metric;v->unit=metric==2U?2U:1U;v->scope=metric<3U?1U:(metric==3U?2U:3U);v->phase=phase;
 v->process=r->process;v->creation=r->creation;v->mapping=r->mapped_base;
 v->thread=f->owner_thread;v->thread_creation=f->native.thread_creation;
 v->root=f->native.root_identity;v->root_state=f->native.root_state;v->witness=f->resource[phase].witness;
 v->request=f->request;v->operation=f->operation;v->generation=f->generation;v->epoch=f->epoch;
 v->begin=r->begin_tick;v->end=r->end_tick;
 v->lifetime=f->resource[phase].retained?2U:(f->resource[phase].teardown?4U:(f->resource[phase].stopped?3U:1U));
 if(c2_resource_identity(f,r) && !r->identity_error && (r->presence&128U))v->presence|=1U;
 if((r->presence&(2UL<<metric)) &&
    ((metric==2U && !r->handles_error) || (metric>=4U && !r->capacity_error) ||
     ((metric==0U || metric==1U || metric==3U) && !r->memory_error)))v->presence|=2U;
 if(metric==0U)v->value=r->private_bytes;else if(metric==1U)v->value=r->commit_bytes;
 else if(metric==2U)v->value=r->handles;else if(metric==3U)v->value=r->os_peak_commit;
 else if(metric==4U)v->value=r->available_physical;else v->value=r->available_pagefile;
 return 1;
}

static LONG failures,checks;
#define CHECK(x) do{InterlockedIncrement(&checks);if(!(x)){InterlockedIncrement(&failures);printf("FAIL line=%d\n",__LINE__);}}while(0)
static void setup(papinho_c2_s6_facts *f){
 ZeroMemory(f,sizeof(*f));f->process_id=42;f->owner_thread=21;f->native.presence=127;f->native.creation=123;
 f->native.thread_creation=456;f->native.mapped_base=4096;f->native.root_identity=7;f->native.root_state=1;
 f->operation=17;f->generation=18;f->request=19;f->epoch=20;f->resource_mask=1;
 f->resource[0].sample.process=42;f->resource[0].sample.creation=123;f->resource[0].sample.mapped_base=4096;
 f->resource[0].sample.presence=255;f->resource[0].sample.begin_tick=100;f->resource[0].sample.end_tick=101;
 f->resource[0].sample.private_bytes=11;f->resource[0].sample.commit_bytes=12;f->resource[0].sample.handles=13;
 f->resource[0].sample.os_peak_commit=14;f->resource[0].sample.available_physical=15;f->resource[0].sample.available_pagefile=16;
}
static DWORD WINAPI worker(void *arg){
 papinho_c2_s6_facts f;C2_TARGET_VIEW v,o;unsigned i;(void)arg;setup(&f);
 for(i=0;i<500U;++i){f.operation=i+1U;CHECK(c2_resource_view(&f,0,2,&v));o=v;
 CHECK(c2_target_view_classify(&v,&o)==1);v.operation++;CHECK(c2_target_view_classify(&v,&o)==2);
 CHECK(c2_target_view_admission(&o,&o)==0);}return 0;
}
int main(void){papinho_c2_s6_facts f;C2_TARGET_VIEW v,o;unsigned i;HANDLE a,b;DWORD ca,cb;
 setup(&f);for(i=0;i<6U;i++){CHECK(c2_resource_view(&f,0,i,&v));o=v;CHECK(v.value==11U+i);CHECK(c2_target_view_classify(&v,&o)==1);CHECK(!c2_target_view_admission(&v,&o));}
 CHECK(!c2_resource_view(&f,8,0,&v));CHECK(!c2_resource_view(&f,0,6,&v));CHECK(!c2_resource_view(&f,1,0,&v));
 c2_resource_view(&f,0,0,&o);
#define MUT(field) v=o;v.field++;CHECK(c2_target_view_classify(&v,&o)==2)
 MUT(version);MUT(bytes);MUT(origin);MUT(clock);MUT(unit);MUT(scope);MUT(phase);MUT(architecture);MUT(backend);
 MUT(process);MUT(thread);MUT(mapping);MUT(root);MUT(root_state);MUT(witness);MUT(lifetime);
 MUT(begin);MUT(end);MUT(presence);MUT(creation);MUT(thread_creation);MUT(request);MUT(operation);MUT(generation);MUT(epoch);MUT(value);
 MUT(policy_present);MUT(expiry_present);MUT(recipe_present);v=o;memset(v.sdk_sha256,'x',65);CHECK(c2_target_view_classify(&v,&o)==2);
 v=o;v.presence=0;CHECK(c2_target_view_classify(&v,&v)==0);v=o;v.begin=102;CHECK(c2_target_view_classify(&v,&v)==0);
 f.resource[0].sample.memory_error=5;c2_resource_view(&f,0,0,&v);CHECK(c2_target_view_classify(&v,&v)==0);
 setup(&f);f.resource[0].sample.handles_error=5;c2_resource_view(&f,0,2,&v);CHECK(c2_target_view_classify(&v,&v)==0);
 setup(&f);f.resource[0].sample.capacity_error=5;c2_resource_view(&f,0,4,&v);CHECK(c2_target_view_classify(&v,&v)==0);
 setup(&f);f.resource[0].retained=1;c2_resource_view(&f,0,0,&v);CHECK(v.lifetime==2 && !c2_target_view_admission(&v,&v));
 setup(&f);f.resource[0].teardown=1;c2_resource_view(&f,0,0,&v);CHECK(v.lifetime==4 && !c2_target_view_admission(&v,&v));
 a=CreateThread(NULL,0,worker,NULL,0,NULL);b=CreateThread(NULL,0,worker,NULL,0,NULL);CHECK(a && b);
 if(a && b){CHECK(WaitForSingleObject(a,10000)==WAIT_OBJECT_0);CHECK(WaitForSingleObject(b,10000)==WAIT_OBJECT_0);
 CHECK(GetExitCodeThread(a,&ca)&&ca==0);CHECK(GetExitCodeThread(b,&cb)&&cb==0);}
 if(a)CHECK(CloseHandle(a));if(b)CHECK(CloseHandle(b));
 printf("CHECKS=%ld FAILURES=%ld VIEW_BYTES=%u FACTS_BYTES=%u NO_NEW_PRODUCT_HANDLES\n",checks,failures,(unsigned)sizeof(v),(unsigned)sizeof(f));return failures?1:0;}
