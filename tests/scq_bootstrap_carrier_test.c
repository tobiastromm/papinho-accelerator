/* Native ServiceMain/SCQ helpers, with existing Windows effect models only. */
#define PAPACC_SCQ_PRE_READINESS_ENTRY preserved_pre_readiness_carrier_main
#include "scq_pre_readiness_chain_test.c"
static SESSION_FIXTURE carrier_fixture;
static PS_SCQ_CARRIER expected_carrier;
static unsigned carrier_runs;
static unsigned carrier_expected_global;
static int carrier_run(int argc,char **argv)
{
 PS_SCQ_CARRIER received;PAPACC_SERVICE_DIAGNOSTIC_START_REPORT report;(void)argc;(void)argv;
 C(host.life.phase==PS_START_PENDING&&papacc_server_service_scq_carrier(&received));
 C(ps_scq_carrier_equal(&received,&expected_carrier));
 C(received.global_until[0]==carrier_expected_global&&!received.global_until[1]);
 C(!papacc_server_service_runtime_ready()&&!papacc_server_service_scq_closed_ready()&&!cs_running);
 C(papacc_server_service_diagnostic_start_status(&report)==ERROR_ACCESS_DENIED&&report.missing_pre_start==15U);
 carrier_runs++;papacc_server_service_runtime_closed();return 2;
}
int main(void)
{
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";
 SESSION_FIXTURE *f=&carrier_fixture;PS_SCQ_CARRIER got,mutant,sentinel;
 char reply[sizeof(prefix)+64U],text[PS_SCQ_CARRIER_CHARS],changed[PS_SCQ_CARRIER_CHARS];
 WCHAR wide[PS_SCQ_CARRIER_CHARS],*args[3];unsigned i,before=checks,reports_before;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
 f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
 C(scqa_start_carrier(&f->source,&f->administrator_owner,&got,&f->stop)!=0);
 C(!scqa_start_selection(&f->source,&f->administrator_owner,&f->stop));
 C(!scqa_start_present(&f->source,&f->administrator_owner,&f->stop));
 C(!scqa_start_selection(&f->source,&f->administrator_owner,&f->stop));
 memcpy(reply,prefix,sizeof(prefix)-1U);for(i=0;i<32U;i++){reply[sizeof(prefix)-1U+2U*i]="0123456789abcdef"[f->plan.attempt[i]>>4];reply[sizeof(prefix)+2U*i]="0123456789abcdef"[f->plan.attempt[i]&15U];}reply[sizeof(reply)-1U]=0;
 C(!scqa_start_confirm(&f->source,&f->administrator_owner,reply,sizeof(reply)-1U,&f->stop));
 C(scqa_start_selection(&f->source,&f->administrator_owner,&f->stop)==&f->policy);
 C(!scqa_start_carrier(&f->source,&f->administrator_owner,&expected_carrier,&f->stop));
 C(!f->source.start_consumed&&!f->source.value.controller_consumed&&!f->runtime.phase);
 C(ps_scq_carrier_encode(&expected_carrier,text,sizeof(text)));
 C(strlen(text)==PS_SCQ_CARRIER_CHARS-1U&&ps_scq_carrier_decode(text,(unsigned)strlen(text),&got)&&ps_scq_carrier_equal(&got,&expected_carrier));
 C(!ps_scq_carrier_encode(&got,changed,sizeof(changed)-1U));
 {unsigned remaining=999U;PS_SCQ_CARRIER timed=got;
  C(!ps_scq_remaining_ms(&timed,100U,0U,&remaining)&&remaining==999U);
  timed.until[0]=200U;timed.until[1]=0;timed.global_until[0]=150U;C(ps_scq_remaining_ms(&timed,100U,0U,&remaining)&&remaining==50U);
  C(!ps_scq_remaining_ms(&timed,150U,0U,&remaining));
  timed.global_until[0]=250U;C(ps_scq_remaining_ms(&timed,100U,0U,&remaining)&&remaining==100U);
  timed.until[0]=timed.global_until[0]=60101U;C(!ps_scq_remaining_ms(&timed,100U,0U,&remaining));
  timed.until[0]=0;timed.until[1]=2U;timed.global_until[0]=0xffffffffU;timed.global_until[1]=1U;
  C(ps_scq_remaining_ms(&timed,0xfffffffeU,1U,&remaining)&&remaining==1U);
  C(!ps_scq_remaining_ms(NULL,0U,0U,&remaining));
 }
 memset(&sentinel,0xa5,sizeof(sentinel));mutant=sentinel;
 C(!ps_scq_carrier_decode(text,(unsigned)strlen(text)-1U,&mutant)&&!memcmp(&sentinel,&mutant,sizeof(mutant)));
 C(!ps_scq_carrier_decode(text,(unsigned)strlen(text)+1U,&mutant));
 memcpy(changed,text,sizeof(text));changed[0]='X';C(!ps_scq_carrier_decode(changed,(unsigned)strlen(changed),&mutant));
 memcpy(changed,text,sizeof(text));changed[5]='A';C(!ps_scq_carrier_decode(changed,(unsigned)strlen(changed),&mutant));
 memcpy(changed,text,sizeof(text));changed[13]='3';C(!ps_scq_carrier_decode(changed,(unsigned)strlen(changed),&mutant));
 mutant=got;mutant.runtime_sid_bytes=7U;C(!ps_scq_carrier_encode(&mutant,changed,sizeof(changed)));
 mutant=got;mutant.runtime_sid[67]=1U;C(!ps_scq_carrier_encode(&mutant,changed,sizeof(changed)));
 mutant=got;memset(mutant.endpoint,0,16);C(!ps_scq_carrier_encode(&mutant,changed,sizeof(changed)));
 mutant=got;mutant.attempt[0]^=1U;C(ps_scq_carrier_encode(&mutant,changed,sizeof(changed))&&!ps_scq_carrier_equal(&mutant,&expected_carrier));
 mutant=got;mutant.controller_pid++;C(!ps_scq_carrier_equal(&mutant,&expected_carrier));
 mutant=got;mutant.target[0]^=1U;C(!ps_scq_carrier_equal(&mutant,&expected_carrier));
 mutant=got;mutant.image[0]^=1U;C(!ps_scq_carrier_equal(&mutant,&expected_carrier));
 for(i=0;i<sizeof(text);i++)wide[i]=(WCHAR)(BYTE)text[i];
 args[0]=(WCHAR *)PAPACC_SCM_NAME_LITERAL;args[1]=L"scq-to";args[2]=wide;
 memset(&host,0,sizeof(host));host.closed_mode=1U;host.run=carrier_run;host.argc=1;
 C(!papacc_server_service_scq_carrier(&got));ps_main(3U,args);
 C(carrier_runs==1U&&!cs_running&&!host.closed_ready&&host.life.phase==PS_STOPPED&&!mcreated&&!mopened&&!mevents);
 C(!papacc_server_service_scq_carrier(&got));reports_before=cs_reports;ps_main(3U,args);C(cs_reports==reports_before&&carrier_runs==1U);
 memset(&host,0,sizeof(host));host.closed_mode=1U;host.run=carrier_run;wide[6]=0x100;ps_main(3U,args);C(!host.handle&&!host.carrier_present&&carrier_runs==1U);
 /* G3 Start metadata is bounded, canonical and DATA only, not admission. */
 {WCHAR *g3args[5],until[17];unsigned k;
  for(i=0;i<sizeof(text);i++)wide[i]=(WCHAR)(BYTE)text[i];
  g3args[0]=args[0];g3args[1]=args[1];g3args[2]=wide;g3args[3]=L"g3-until";g3args[4]=until;
  for(k=0;k<7U;k++){
   memset(&host,0,sizeof(host));host.closed_mode=1U;host.run=carrier_run;host.argc=1;
   wcscpy_s(until,17U,L"0000000000000096");g3args[3]=L"g3-until";
   if(k==1U)wcscpy_s(until,17U,L"0000000000000064");
   if(k==2U)wcscpy_s(until,17U,L"0000000000002775");
   if(k==3U)until[15]=0;if(k==4U)until[15]=L'A';if(k==5U)g3args[3]=L"other";
   if(k==6U)wcscpy_s(until,17U,L"ffffffffffffffff");
   carrier_expected_global=150U;ps_main(5U,g3args);
   if(!k)C(carrier_runs==2U&&host.life.phase==PS_STOPPED&&!cs_running);
   else C(!host.handle&&carrier_runs==2U);
  }
  carrier_expected_global=0;
 }
 /* Same pre-Start endpoint selector survives the fresh runtime binding. */
 C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
 C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));
 C(!scqt_reserve(&f->controller,&f->administrator_owner,&f->stop));
 C(!f->runtime.phase&&!f->source.start_consumed&&f->controller.transport.phase==2U);
 C(scqt_reserve(&f->controller,&f->administrator_owner,&f->stop)!=0&&mcreated==1U);
 C(!scqa_start_take(&f->source,&f->administrator_owner,&f->stop));
 C(scqa_start_selection(&f->source,&f->administrator_owner,&f->stop)==&f->policy&&f->source.start_consumed);
 runtime_service_state=SERVICE_START_PENDING;C(!observed_runtime_mode(f,1U));
 C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
 C(!scqa_start_carrier(&f->source,&f->administrator_owner,&got,&f->stop)&&ps_scq_carrier_equal(&got,&expected_carrier));
 C(!scqt_connect(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
 mpeer=f->plan.runtime.pid;C(!scqt_connected(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
 {size_t n=wcslen(L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.");
  for(i=0;i<16U;i++)C(f->controller.transport.name[n+2U*i]==L"0123456789abcdef"[got.endpoint[i]>>4]&&f->controller.transport.name[n+2U*i+1U]==L"0123456789abcdef"[got.endpoint[i]&15U]);
  C(!f->controller.transport.name[n+32U]);
 }
 C(mcreated==1U&&mevents==1U&&!mopened); /* Model API counters only. */
 C(scqs_close(&f->controller,&f->administrator_owner,0)!=0&&f->source.value.retained);
 printf("PASS SCQ_BOOTSTRAP_CARRIER metadata_cases=7 checks=%u wire=%u chars=%u source=%zu host=%zu DATA_NOT_AUTHORITY PRESTART_SELECTOR_TO_SERVICEMAIN NO_PREMATURE_RUNNING MODEL_ENDPOINT_ONLY\n",checks-before,PS_SCQ_CARRIER_BYTES,PS_SCQ_CARRIER_CHARS,sizeof(f->source),sizeof(host));return 0;
}
