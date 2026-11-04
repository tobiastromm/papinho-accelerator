/* Actual private guards and ServiceMain; ALL process/token/SCM/endpoint/RNG
 * effects are models inherited from the existing chain fixture. This test
 * deliberately does NOT claim an initial original-reference producer. */
#define PAPACC_SCQ_PRE_READINESS_ENTRY preserved_pre_readiness_chain_main
#include "scq_pre_readiness_chain_test.c"
static SESSION_FIXTURE first_packet;
static PAPACC_SCQ_RECIPIENT_ORIGIN first_origin;
static PAPACC_PERSONAL_SCQ_INPUTS first_inputs;
int main(void)
{
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";
 SESSION_FIXTURE *f=&first_packet;PAPACC_SERVICE_DIAGNOSTIC_START_REPORT report;
 unsigned before=checks,i;char reply[sizeof(prefix)+64U];DWORD e;WCHAR *args[3];
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
 f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";
 f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
 C(!scqa_start_present(&f->source,&f->administrator_owner,&f->stop));
 memcpy(reply,prefix,sizeof(prefix)-1U);
 for(i=0;i<32U;i++){reply[sizeof(prefix)-1U+2U*i]="0123456789abcdef"[f->plan.attempt[i]>>4];reply[sizeof(prefix)+2U*i]="0123456789abcdef"[f->plan.attempt[i]&15U];}
 reply[sizeof(reply)-1U]=0;
 C(!scqa_start_confirm(&f->source,&f->administrator_owner,reply,sizeof(reply)-1U,&f->stop));
 C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
 C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));
 C(!scqa_start_take(&f->source,&f->administrator_owner,&f->stop));
 C(f->source.start_consumed&&f->source.value.controller_consumed&&!f->runtime.phase);
 /* A Start act is NOT an endpoint constructor before a runtime original. */
 mcreated=mopened=mevents=0;
 e=scqt_create(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop);
 C(e!=0&&!mcreated&&!mopened&&!mevents);
 /* An invalid selector is rejected; valid typed carriers have a separate
  * positive ServiceMain test and still carry no authority. */
 memset(&host,0,sizeof(host));host.closed_mode=1U;
 args[0]=(WCHAR *)PAPACC_SCM_NAME_LITERAL;args[1]=L"scq-to";args[2]=L"0123456789abcdef0123456789abcdef";
 ps_main(3U,args);C(!host.handle&&!host.life.phase&&!cs_runs&&!cs_reports);
 /* Observing the actual (modeled) runtime does not create request/decision. */
 runtime_service_state=SERVICE_START_PENDING;C(!observed_runtime_identity_mode(f,1U));
 e=scqr_first_prepare(&first_origin,&f->recipient,&f->runtime_owner,&f->policy,&f->runtime_profile,&first_inputs,&f->runtime,&f->runtime_dispatch,&f->runtime_receiver,&f->plan,(HANDLE)44,&f->stop);
 C(e!=0&&!first_origin.phase&&!f->recipient.phase&&!mopened&&!mevents);
 /* Even a valid DATA profile cannot replace the missing local original. */
 {WCHAR empty[112]={0};
  C(!papacc_personal_scq_prepare_transport(&f->controller_dispatch,&f->runtime,f->plan.attempt,empty));
  C(!papacc_personal_scq_prepare_transport(&f->runtime_dispatch,&f->runtime,f->plan.attempt,empty));
 }
 C(!papacc_scq_profile_prepare_transport_data(&f->runtime_profile,&f->runtime_owner,2U,&f->plan,&f->plan.runtime));
 C(!scqs_prepare(&f->recipient,&f->runtime_owner,2U,&f->runtime_profile,NULL,&f->runtime_receiver,&f->runtime_dispatch,&f->runtime));
 e=scqt_open_local(&f->recipient,&f->runtime_owner,L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.0123456789abcdef0123456789abcdef",&f->plan.runtime,&f->plan.administrator,f->plan.refs,NULL,&f->stop);
 C(e!=0&&!mopened&&!mevents);
 /* The same consumed act has not become an original in the SCM owner. */
 C(papacc_server_service_diagnostic_start_status(&report)==ERROR_ACCESS_DENIED);
 C(report.missing_pre_start==15U&&report.later_obligations==15U&&!report.available_metadata);
 C(f->source.start_consumed&&f->source.value.controller_consumed&&!host.closed_ready);
 /* Preserve the consumed source; no assertion of peer/OS quiescence. */
 C(scqa_close(&f->source,&f->administrator_owner,0)!=0&&f->source.value.retained);
 printf("PASS FIRST_INGRESS_BOUNDARY checks=%u CONFIRMED_START_NOT_SCM_BINDING NO_ORIGINALS_NO_LEGACY_OPEN INVALID_TRANSIENT_CARRIER_DENIED NO_NATIVE_AUTHORITY_CLAIM NO_ENDPOINT_NO_REAL_RNG\n",checks-before);
 return 0;
}
