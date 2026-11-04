/* Same issuer model, explicitly test-only authority. No product source grant. */
#define main previous_issuer_model_main
#include "issuer_core_test.c"
#undef main
#include "../src/provisioning/enrollment_issuer.h"
static PAPACC_ISSUER_CORE consumer_core;
static EP_TRACE consumer_trace;
static EP_PAIR consumer_pair;
static PAPACC_U8 quarantine[128],recipient[128];
static PAPACC_ISSUER_RESULT mutate_request(void *ctx,const PAPACC_ISSUER_RECORD *r,const PAPACC_U8 *der,PAPACC_SIZE n)
{PAPACC_ISSUER_RESULT result=verify_req(ctx,r,der,n);m.records[0].request.device[0]^=1;return result;}
static int consumer_setup(void)
{
 PAPACC_ISSUER_REQUEST q;setup();memset(&consumer_core,0,sizeof(consumer_core));
 m.view.continuity_profile=PAPACC_ISSUER_CONTINUITY_PERSONAL_NORMAL;m.view.proofs=PAPACC_ISSUER_NORMAL_PROOFS;
 backend.verify_request_data=verify_req;
 CHECK(papacc_issuer_init_normal(&consumer_core,&root,&backend)==0);q=request(1);
 CHECK(papacc_issuer_prepare(&consumer_core,&q)==0&&papacc_issuer_reserve(&consumer_core,q.id)==0);
 memset(&consumer_trace,0,sizeof(consumer_trace));memset(&consumer_pair,0,sizeof(consumer_pair));
 consumer_trace.phase=5;consumer_trace.csr[0]=7;consumer_trace.csr_size=1;memcpy(consumer_trace.request,q.id,16);memcpy(consumer_pair.request,q.id,16);
 consumer_pair.initialized=consumer_pair.submitted=consumer_pair.claimed=1;consumer_pair.connection=11;consumer_pair.generation=12;consumer_pair.start=100;consumer_pair.last=101;consumer_pair.deadline=300100;
 memset(quarantine,0,sizeof(quarantine));memset(recipient,0xa5,sizeof(recipient));m.owner=&consumer_core;return 0;
}
#define SUBMIT() ep_issuer_submit(&consumer_core,&consumer_trace,&consumer_pair,11,12,102,quarantine,sizeof(quarantine))
#define POLL() ep_issuer_poll(&consumer_core,&consumer_trace,&consumer_pair,11,12,102,quarantine,sizeof(quarantine))
#define PUBLISH() ep_issuer_publish(&consumer_core,&consumer_trace,&consumer_pair,11,12,102)
#define DELIVER() ep_issuer_deliver(&consumer_core,&consumer_trace,&consumer_pair,11,12,102,quarantine,sizeof(quarantine),recipient,sizeof(recipient),&used)
int main(void)
{
 unsigned fault,before,turn;PAPACC_SIZE used=77;PAPACC_ISSUER_RESULT r;
 for(fault=0;fault<7;fault++){
  CHECK(!consumer_setup());before=m.signs;
  if(fault==0)m.deny_auth=1;if(fault==1)consumer_pair.generation++;
  if(fault==2)consumer_trace.terminal=1;if(fault==3)m.view.epoch++;
  if(fault==4)m.records[0].request.purpose=PAPACC_ISSUER_PACKAGE;
  if(fault==5)consumer_trace.stopped=1;if(fault==6)m.view.proofs=0;
  CHECK(SUBMIT()!=0&&m.signs==before&&recipient[0]==0xa5);
 }
 CHECK(!consumer_setup());CHECK(DELIVER()!=0&&used==77&&recipient[0]==0xa5);
 CHECK(SUBMIT()==0&&m.signs==1);CHECK(SUBMIT()==PAPACC_ISSUER_CONFLICT&&m.signs==1);
 CHECK(DELIVER()!=0&&used==77&&recipient[0]==0xa5);
 m.allow_publish=1;CHECK(PUBLISH()==0);
 memset(recipient,0xa5,sizeof(recipient));used=77;backend.verify_request_data=NULL;CHECK(DELIVER()==PAPACC_ISSUER_UNSUPPORTED&&used==77&&recipient[0]==0xa5);backend.verify_request_data=verify_req;
 m.view.epoch++;CHECK(DELIVER()!=0&&used==77&&recipient[0]==0xa5);m.view.epoch--;
 backend.verify_request_data=mutate_request;CHECK(DELIVER()==PAPACC_ISSUER_CONFLICT&&used==77&&recipient[0]==0xa5);m.records[0].request.device[0]^=1;backend.verify_request_data=verify_req;
 m.bad_request=1;CHECK(DELIVER()!=0&&used==77&&recipient[0]==0xa5);m.bad_request=0;
 m.bad_cert=1;CHECK(DELIVER()!=0&&used==77&&recipient[0]==0xa5);m.bad_cert=0;
 CHECK(ep_issuer_deliver(&consumer_core,&consumer_trace,&consumer_pair,11,12,102,quarantine,sizeof(quarantine),quarantine,sizeof(quarantine),&used)==PAPACC_ISSUER_INVALID);
 CHECK(DELIVER()==0&&used==3&&!memcmp(recipient,"DER",3)&&m.signs==1&&consumer_trace.credential_released==1);
 CHECK(DELIVER()!=0&&used==3&&!memcmp(recipient,"DER",3)&&m.signs==1);
 m.records[0].state=PAPACC_ISSUER_REVOKED;CHECK(DELIVER()!=0&&used==3&&!memcmp(recipient,"DER",3));
 CHECK(!consumer_setup());m.async_mode=1;CHECK(SUBMIT()==PAPACC_ISSUER_PENDING&&!m.signs);
 before=m.starts;CHECK(SUBMIT()==PAPACC_ISSUER_BUSY&&m.starts==before);
 CHECK(POLL()==PAPACC_ISSUER_PENDING&&!m.signs);CHECK(POLL()==PAPACC_ISSUER_PENDING&&m.signs==1);
 CHECK(POLL()==PAPACC_ISSUER_PENDING&&m.signs==1);CHECK(POLL()==0&&m.signs==1&&!consumer_core.lease);
 m.allow_publish=1;CHECK(PUBLISH()==PAPACC_ISSUER_PENDING);CHECK(POLL()==PAPACC_ISSUER_PENDING);CHECK(POLL()==0&&m.signs==1);
 CHECK(DELIVER()==0&&used==3&&m.signs==1);
 CHECK(!consumer_setup());m.async_mode=1;CHECK(SUBMIT()==PAPACC_ISSUER_PENDING);
 before=m.polls;CHECK(ep_issuer_poll(&consumer_core,&consumer_trace,&consumer_pair,11,99,102,quarantine,sizeof(quarantine))==PAPACC_ISSUER_RETAINED&&m.polls==before&&!m.signs);
 CHECK(ep_issuer_poll(&consumer_core,&consumer_trace,&consumer_pair,11,12,consumer_pair.deadline,quarantine,sizeof(quarantine))==PAPACC_ISSUER_RETAINED&&m.polls==before);
 papacc_issuer_stop(&consumer_core);r=PAPACC_ISSUER_PENDING;for(turn=0;turn<3&&r==PAPACC_ISSUER_PENDING;turn++)r=POLL();
 CHECK(r==PAPACC_ISSUER_STOPPED&&!m.signs); /* factual owner query, not adapter retry */
 CHECK(!consumer_setup());m.fail_release=1;CHECK(SUBMIT()==PAPACC_ISSUER_RETAINED&&m.signs==1&&consumer_core.lease);
 CHECK(SUBMIT()==PAPACC_ISSUER_RETAINED&&m.signs==1);CHECK(DELIVER()!=0&&recipient[0]==0xa5);
 CHECK(!consumer_setup());m.async_mode=1;CHECK(SUBMIT()==PAPACC_ISSUER_PENDING);consumer_trace.csr[0]=8;
 CHECK(POLL()==PAPACC_ISSUER_PENDING);CHECK(POLL()!=0&&!m.signs);
 printf("PASS ENROLLMENT_ISSUER_MODEL checks=%u ORIGINAL_CORE_PENDING_POLL_NO_RESIGN RECIPIENT_QUARANTINED NO_PRODUCT_AUTHORITY\n",checks);return 0;
}
