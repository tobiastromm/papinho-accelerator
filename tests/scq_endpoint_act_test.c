/* Actual original-source/endpoint act; native references and console input
 * modeled. Never calls a native endpoint, Start, RNG, KSP or SCM. */
#define PAPACC_SCQ_PRE_READINESS_ENTRY preserved_endpoint_chain_main
#include "scq_pre_readiness_chain_test.c"
static SESSION_FIXTURE endpoint_fixture;
static DWORD endpoint_reply(SESSION_FIXTURE *f,unsigned bad)
{
 static const char prefix[]="OWNER_SCQ_PRIVATE_ENDPOINT ";char reply[sizeof(prefix)+64U];unsigned j;
 memcpy(reply,prefix,sizeof(prefix)-1U);
 for(j=0;j<32U;j++){reply[sizeof(prefix)-1U+2U*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[sizeof(prefix)+2U*j]="0123456789abcdef"[f->plan.attempt[j]&15U];}reply[sizeof(reply)-1U]=0;
 if(bad)reply[0]='X';return scqa_endpoint_confirm(&f->source,&f->administrator_owner,reply,sizeof(reply)-1U,&f->stop);
}
int main(void)
{
 SESSION_FIXTURE *f=&endpoint_fixture;unsigned i,before=checks;DWORD e;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(!scqa_endpoint_selection(NULL,f,&f->stop));
 for(i=0;i<17U;i++){
  C(!setup_session_purpose(f,1,1U,i==1U?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
  f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
  C(!scqa_endpoint_selection(&f->source,&f->administrator_owner,&f->stop));
  C(!scqa_start_selection(&f->source,&f->administrator_owner,&f->stop));
  if(i==2U)f->source.phase=1U;
  if(i==3U)f->stop=1;
  e=scqa_endpoint_present(&f->source,&f->administrator_owner,&f->stop);
  if(i>=1U&&i<=3U){C(e!=0&&!f->source.endpoint_consumed);continue;}
  C(!e&&!scqa_endpoint_selection(&f->source,&f->administrator_owner,&f->stop));
  e=endpoint_reply(f,i==4U);if(i==4U){C(e!=0&&!f->source.endpoint_consumed&&f->source.value.retained);continue;}
  C(!e&&!f->source.endpoint_consumed&&!f->source.start_phase&&!f->source.start_consumed);
  if(i==5U)f->policy.attempt[0]^=1U;
  if(i==6U)f->source.thread++;
  if(i==7U)f->source.registration=NULL;
  if(i==8U)f->source.value.image_id.nFileIndexLow++;
  if(i==9U)f->source.endpoint_consumed=1;
  if(i==10U)f->source.endpoint_phase=4;
  if(i==11U)f->stop=1;
  if(i==12U)f->source.value.retained=1;
  if(i==13U)f->policy.valid_until[0]^=1U;
  if(i>=5U&&i<=14U){C(!scqa_endpoint_selection(&f->source,i==14U?(const void *)&f->runtime_owner:(const void *)&f->administrator_owner,&f->stop));continue;}
  C(scqa_endpoint_selection(&f->source,&f->administrator_owner,&f->stop)==&f->policy);
  C(scqa_endpoint_present(&f->source,&f->administrator_owner,&f->stop)!=0); /* cannot re-present */
  C(scqa_endpoint_take(&f->source,&f->administrator_owner,&f->stop)!=0); /* SCQ not yet consumed */
  /* A failed take fails closed. A separate modeled process for the success
   * cases above is built in the following independent fixture iteration. */
  if(i==15U){C(!f->source.endpoint_consumed);continue;}
  if(i==16U){C(scqa_close(&f->source,&f->administrator_owner,0)!=0);C(f->source.phase==6U&&f->source.value.retained);continue;}
 }
 /* Valid original: confirmation is not consumption; SCQ take precedes the
  * independent endpoint consumption. Still no endpoint effect is invoked. */
 C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
 f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
 C(!scqa_endpoint_present(&f->source,&f->administrator_owner,&f->stop));C(!endpoint_reply(f,0U));
 C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
 C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));
 C(!scqa_endpoint_take(&f->source,&f->administrator_owner,&f->stop)&&f->source.endpoint_consumed==1);
 C(scqa_endpoint_selection(&f->source,&f->administrator_owner,&f->stop)==&f->policy);
 {PAPACC_SERVICE_DIAGNOSTIC_START_REPORT report;
  C(papacc_server_service_diagnostic_start_status(&report)==ERROR_ACCESS_DENIED);
  C(report.resource_pre_missing==63U&&report.resource_effect_obligations==7U);
 }
 C(scqa_endpoint_take(&f->source,&f->administrator_owner,&f->stop)!=0);
 C(!f->source.start_phase&&!f->source.start_consumed&&!mcreated&&!mopened&&!mevents&&!cs_running);
 printf("PASS ENDPOINT_ACT cases=18 checks=%u INDEPENDENT_ORIGINAL_ONE_USE MODEL_ONLY NO_ENDPOINT_NO_START_NO_AUTHORITY_FROM_DATA\n",checks-before);return 0;
}
