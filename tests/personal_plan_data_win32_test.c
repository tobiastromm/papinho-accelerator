#include "server_configuration_commit_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT owner_state;
static unsigned char nonce[32]={1},other[32]={2},plan[]={1,2,3,4},changed[]={1,2,3,5};
#define CHECK(x) do {if(!(x)){printf("FAIL %u\n",(unsigned)__LINE__);return 1;}}while(0)
int main(void)
{
 PAPACC_PERSONAL_PLAN_DATA *p=&owner_state.pending_commit.personal_data;unsigned i;
 PAPACC_PERSONAL_CUSTODY_DATA q;PAPACC_CA_KEY ca,ca_before;
 /* MODEL owner initialization only: no CSPRNG, filesystem, token or key call. */
 InitializeCriticalSection(&owner_state.lock);
 owner_state.initialized=1;owner_state.writer=GetCurrentThreadId();
 /* MODEL metadata only. No synthetic native handles or custody proof. */
 memset(&ca,0,sizeof(ca));ca_before=ca;
 q=papacc_configuration_personal_custody_data(&owner_state,NULL);
 CHECK(q.target==PAPACC_PERSONAL_CUSTODY_ABSENT&&q.ca==PAPACC_PERSONAL_CUSTODY_ABSENT&&q.effect_denied);
 CHECK(papacc_configuration_bootstrap_custody_begin(&owner_state,"C:\\MODEL_ONLY_NOT_CREATED","candidate",0)!=PAPACC_RESULT_OK);
 CHECK(!owner_state.pending_commit.active&&!owner_state.pending_commit.thread&&!owner_state.pending_commit.bootstrap_parent&&!owner_state.pending_commit.bootstrap_container);
 owner_state.pending_commit.bootstrap_created=1U;
 CHECK(papacc_configuration_bootstrap_custody_begin(&owner_state,"C:\\MODEL_ONLY_NOT_CREATED","candidate",0)!=PAPACC_RESULT_OK);
 CHECK(owner_state.pending_commit.bootstrap_created);
 q=papacc_configuration_personal_custody_data(&owner_state,&ca);
 CHECK(q.target==PAPACC_PERSONAL_CUSTODY_METADATA&&q.effect_denied);
 CHECK(!memcmp(&ca,&ca_before,sizeof(ca))); /* no mutation/acquisition */
 owner_state.pending_commit.result=PAPACC_CAS_UNKNOWN;owner_state.stopped=1;
 q=papacc_configuration_personal_custody_data(&owner_state,&ca);
 CHECK(q.target==PAPACC_PERSONAL_CUSTODY_RETAINED&&q.effect_denied);
 CHECK(owner_state.pending_commit.bootstrap_created); /* Stop never clears history */
 CHECK(papacc_configuration_bootstrap_custody_begin(&owner_state,"C:\\MODEL_ONLY_NOT_CREATED","candidate",0)!=PAPACC_RESULT_OK);
 CHECK(owner_state.pending_commit.result==PAPACC_CAS_UNKNOWN);
 ca.state=PAPACC_CA_KEY_RETAINED;ca_before=ca;
 q=papacc_configuration_personal_custody_data(&owner_state,&ca);
 CHECK(q.ca==PAPACC_PERSONAL_CUSTODY_RETAINED&&q.effect_denied&&!memcmp(&ca,&ca_before,sizeof(ca)));
 ca.state=PAPACC_CA_KEY_READY; /* metadata only, no actual key */
 q=papacc_configuration_personal_custody_data(&owner_state,&ca);
 CHECK(q.ca==PAPACC_PERSONAL_CUSTODY_METADATA&&q.effect_denied);
 ca.borrower=&owner_state;
 q=papacc_configuration_personal_custody_data(&owner_state,&ca);
 CHECK(q.ca==PAPACC_PERSONAL_CUSTODY_RETAINED&&q.effect_denied);
 ca.thread=GetCurrentThreadId()+1U;
 q=papacc_configuration_personal_custody_data(&owner_state,&ca);
 CHECK(q.ca==PAPACC_PERSONAL_CUSTODY_UNAVAILABLE&&q.effect_denied);
 owner_state.writer=0;
 q=papacc_configuration_personal_custody_data(&owner_state,NULL);
 CHECK(q.target==PAPACC_PERSONAL_CUSTODY_UNAVAILABLE&&q.effect_denied);
 owner_state.writer=GetCurrentThreadId();owner_state.stopped=0;
 owner_state.pending_commit.bootstrap_created=0;owner_state.pending_commit.result=PAPACC_CAS_SUCCESS;

 CHECK(!papacc_personal_data_capture(p,nonce,plan,0));
 CHECK(!papacc_personal_data_capture(p,nonce,plan,PAPACC_PERSONAL_PLAN_MAX+1U));
 CHECK(papacc_configuration_personal_data_capture(&owner_state,nonce,plan,sizeof(plan))==0);
 CHECK(papacc_configuration_personal_handoff_begin(&owner_state,nonce,0)!=PAPACC_RESULT_OK);
 CHECK(!owner_state.pending_commit.thread&&!owner_state.pending_commit.active&&!p->consumed);
 CHECK(papacc_configuration_personal_handoff_poll(&owner_state)!=PAPACC_RESULT_OK);

 CHECK(!papacc_personal_data_consume(p,nonce,plan,sizeof(plan)));
 CHECK(!papacc_personal_data_confirm(p,nonce,plan,sizeof(plan)));
 CHECK(papacc_personal_data_present(p));
 CHECK(!papacc_personal_data_confirm(p,other,plan,sizeof(plan)));
 CHECK(p->phase==PAPACC_PERSONAL_DATA_INVALID);
 CHECK(papacc_configuration_personal_data_retire(&owner_state)==0);
 CHECK(!papacc_personal_data_capture(p,nonce,plan,sizeof(plan)));
 memset(p,0,sizeof(*p));
 CHECK(papacc_configuration_personal_data_capture(&owner_state,nonce,plan,sizeof(plan))==0);
 CHECK(papacc_personal_data_present(p)&&papacc_personal_data_confirm(p,nonce,plan,sizeof(plan)));
 owner_state.pending_commit.personal_native.phase=1U; /* MODEL metadata, no native refs */
 CHECK(papacc_configuration_personal_handoff_begin(&owner_state,other,0)!=PAPACC_RESULT_OK);
 CHECK(p->phase==PAPACC_PERSONAL_DATA_INVALID&&!p->consumed&&!owner_state.pending_commit.thread);
 owner_state.pending_commit.personal_native.phase=0U;
 memset(p,0,sizeof(*p));
 CHECK(papacc_configuration_personal_data_capture(&owner_state,nonce,plan,sizeof(plan))==0);
 CHECK(papacc_personal_data_present(p)&&papacc_personal_data_confirm(p,nonce,plan,sizeof(plan)));
 owner_state.pending_commit.personal_native.phase=1U;
 CHECK(papacc_configuration_personal_handoff_begin(&owner_state,nonce,0)!=PAPACC_RESULT_OK); /* no factual parent */
 CHECK(!p->consumed&&!owner_state.pending_commit.active&&!owner_state.pending_commit.thread);
 memcpy(owner_state.pending_commit.personal_native.plan_leaf,"candidate",sizeof("candidate"));
 memcpy(owner_state.pending_commit.previous,"different",sizeof("different"));
 CHECK(papacc_configuration_personal_handoff_begin(&owner_state,nonce,0)!=PAPACC_RESULT_OK);
 CHECK(!p->consumed&&!owner_state.pending_commit.thread);
 memcpy(owner_state.pending_commit.previous,"candidate",sizeof("candidate"));
 CHECK(papacc_configuration_personal_handoff_begin(&owner_state,nonce,0)!=PAPACC_RESULT_OK); /* matching MODEL leaf cannot invent parent */
 owner_state.stopped=1;
 CHECK(papacc_configuration_personal_handoff_begin(&owner_state,nonce,0)!=PAPACC_RESULT_OK);
 CHECK(!p->consumed&&!owner_state.pending_commit.thread);owner_state.stopped=0;
 owner_state.pending_commit.personal_native.handoff_state=PAPACC_PERSONAL_HANDOFF_VERIFIED; /* MODEL one-shot replay */
 CHECK(papacc_configuration_personal_handoff_begin(&owner_state,nonce,0)!=PAPACC_RESULT_OK);
 owner_state.pending_commit.personal_native.handoff_state=PAPACC_PERSONAL_HANDOFF_RETAINED;
 owner_state.pending_commit.packet_format=12U;owner_state.pending_commit.active=1;
 CHECK(papacc_configuration_personal_handoff_poll(&owner_state)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_native_close(&owner_state)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_personal_data_retire(&owner_state)!=PAPACC_RESULT_OK);
 CHECK(owner_state.pending_commit.active&&owner_state.pending_commit.personal_native.handoff_state==PAPACC_PERSONAL_HANDOFF_RETAINED);
 CHECK(owner_state.unresolved); /* retained owner is now explicitly sticky */
 /* Fixture-only disposal of metadata: no OS resource ever existed. */
 owner_state.unresolved=0; /* MODEL fixture only: never a product rearm. */
 owner_state.pending_commit.active=0;owner_state.pending_commit.packet_format=0;
 owner_state.pending_commit.personal_native.handoff_state=0;owner_state.pending_commit.personal_native.phase=0U;
 memset(p,0,sizeof(*p)); /* MODEL reset only, never a product retry. */
 CHECK(papacc_configuration_personal_data_capture(&owner_state,nonce,plan,sizeof(plan))==0);
 CHECK(papacc_personal_data_present(p));
 CHECK(!papacc_personal_data_confirm(p,nonce,changed,sizeof(changed)));
 CHECK(papacc_configuration_personal_data_retire(&owner_state)==0);
 memset(p,0,sizeof(*p));
 CHECK(papacc_configuration_personal_data_capture(&owner_state,nonce,plan,sizeof(plan))==0);
 CHECK(papacc_personal_data_present(p));
 CHECK(papacc_personal_data_confirm(p,nonce,plan,sizeof(plan)));
 CHECK(papacc_personal_data_consume(p,nonce,plan,sizeof(plan)));
 CHECK(!papacc_personal_data_consume(p,nonce,plan,sizeof(plan)));
 CHECK(!papacc_personal_data_confirm(p,nonce,plan,sizeof(plan)));
 owner_state.pending_commit.personal_native.phase=2U; /* MODEL retained native graph */
 CHECK(papacc_configuration_personal_data_retire(&owner_state)!=0&&p->phase==PAPACC_PERSONAL_DATA_CONSUMED);
 owner_state.pending_commit.personal_native.phase=0U; /* fixture only */
 owner_state.pending_commit.active=1;
 CHECK(papacc_configuration_personal_data_retire(&owner_state)!=0&&p->phase==PAPACC_PERSONAL_DATA_CONSUMED&&p->consumed);
 owner_state.pending_commit.active=0;owner_state.stopped=1;
 CHECK(papacc_configuration_personal_data_retire(&owner_state)==0&&p->consumed);
 for(i=0;i<PAPACC_PERSONAL_PLAN_MAX;i++)CHECK(p->plan[i]==0);
 CHECK(!papacc_personal_data_present(p));
 CHECK(papacc_configuration_personal_data_capture(&owner_state,other,plan,sizeof(plan))!=0);
 memset(p,0,sizeof(*p));owner_state.stopped=0;
 CHECK(!papacc_personal_data_capture(p,nonce,p->plan,1));
 CHECK(papacc_configuration_personal_data_capture(&owner_state,nonce,plan,sizeof(plan))==0);
 CHECK(papacc_configuration_prepare(&owner_state,NULL,0,0)!=0); /* never authority */
 CHECK(papacc_personal_data_present(p));
 CHECK(papacc_personal_data_confirm(p,nonce,plan,sizeof(plan)));
 CHECK(papacc_configuration_stop(&owner_state)==0);
 CHECK(p->phase==PAPACC_PERSONAL_DATA_INVALID);
 CHECK(!papacc_personal_data_consume(p,nonce,plan,sizeof(plan)));
 CHECK(papacc_configuration_personal_data_retire(&owner_state)==0);
 DeleteCriticalSection(&owner_state.lock);
 printf("PERSONAL_DATA_ONLY_PASS STATE_BYTES=%lu NO_ELIGIBILITY_NO_CUSTODY_NO_EFFECT\n",(unsigned long)sizeof(*p));return 0;
}
