/* Existing original-source and Windows effect fixtures; never a native
 * approval, SCM process, endpoint, RNG or capacity measurement. */
#define PAPACC_SCQ_PRE_READINESS_ENTRY preserved_selection_readiness_main
#include "scq_pre_readiness_chain_test.c"
static SESSION_FIXTURE selection_fixture;
int main(void)
{
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";
 SESSION_FIXTURE *f=&selection_fixture;unsigned i,j,before=checks;char reply[sizeof(prefix)+64U];
 PAPACC_SERVICE_DIAGNOSTIC_START_REPORT report;const PAPACC_SCQ_OWNER_DECISION *selected;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(!scqa_start_selection(NULL,f,&f->stop));
 for(i=0;i<18U;i++){
  C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
  f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";
  f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
  C(!scqa_start_selection(&f->source,&f->administrator_owner,&f->stop));
  C(!scqa_start_present(&f->source,&f->administrator_owner,&f->stop));
  C(!scqa_start_selection(&f->source,&f->administrator_owner,&f->stop));
  memcpy(reply,prefix,sizeof(prefix)-1U);
  for(j=0;j<32U;j++){reply[sizeof(prefix)-1U+2U*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[sizeof(prefix)+2U*j]="0123456789abcdef"[f->plan.attempt[j]&15U];}
  reply[sizeof(reply)-1U]=0;
  C(!scqa_start_confirm(&f->source,&f->administrator_owner,reply,sizeof(reply)-1U,&f->stop));
  if(i==1U)f->source.start_phase=1U;
  if(i==2U)f->source.phase=1U;
  if(i==3U)f->source.self=NULL;
  if(i==5U)f->source.thread++;
  if(i==6U)f->policy.valid_until[0]^=1U;
  if(i==7U)f->policy.key[0]='x';
  if(i==8U)f->source.value.image_id.nFileIndexLow++;
  if(i==9U)f->policy.runtime[8]^=1U;
  if(i==10U)f->policy.controller_token[0]^=1U;
  if(i==11U)f->stop=1;
  if(i==12U)f->source.start_consumed=1;
  if(i==13U)f->source.start_phase=4U;
  if(i==14U)f->source.registration=NULL;
  if(i==15U)f->policy.source=NULL;
  if(i==16U)f->source.value.request=NULL;
  if(i==17U)f->policy.parent=NULL;
  selected=scqa_start_selection(&f->source,i==4U?(const void *)&f->runtime_owner:(const void *)&f->administrator_owner,&f->stop);
  if(i){C(!selected);continue;}
  C(selected==&f->policy&&!f->source.start_consumed&&!f->source.value.controller_consumed);
  C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
  C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));
  C(!scqa_start_take(&f->source,&f->administrator_owner,&f->stop));
  C(scqa_start_selection(&f->source,&f->administrator_owner,&f->stop)==selected&&f->source.start_consumed);
  C(scqa_start_take(&f->source,&f->administrator_owner,&f->stop)!=0);
 }
 C(papacc_server_service_diagnostic_start_status(&report)==ERROR_ACCESS_DENIED);
 C(report.missing_pre_start==15U&&report.resource_pre_missing==63U&&report.resource_effect_obligations==7U);
 C(report.scm_compiled_storage_bytes==sizeof(native)+sizeof(host));
 C(report.carrier_binary_bytes==PS_SCQ_CARRIER_BYTES&&report.carrier_text_characters==PS_SCQ_CARRIER_CHARS);
 C(report.scq_input_storage_bytes==sizeof(PAPACC_PERSONAL_SCQ_INPUTS)&&report.scq_image_chain_storage_bytes==sizeof(PAPACC_SCQ_IMAGE_CHAIN));
 C(report.controller_plan_storage_bytes==sizeof(PAPACC_SCQ_PLAN_DATA)&&report.resource_pre_missing==63U);
 C(report.scq_image_chain_handle_slots==PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS+1U);
 C(!mcreated&&!mopened&&!mevents&&!cs_running);
 printf("PASS SCQ_START_SELECTION cases=18 checks=%u ORIGINAL_BORROW_ONLY NO_NATIVE_DECISION_OR_CAPACITY SOURCE_BOOTSTRAP_NOT_IMPLIED PRE_MISSING=%lu EFFECT_OBLIGATIONS=%lu SCM_COMPILED_BYTES=%zu\n",checks-before,(unsigned long)report.resource_pre_missing,(unsigned long)report.resource_effect_obligations,report.scm_compiled_storage_bytes);
 return 0;
}
