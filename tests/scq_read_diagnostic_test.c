/* Same original-source actors; all OS/process/file/KSP APIs are modeled. */
#define PAPACC_SCQ_SESSION_ENTRY preserved_read_session_main
#include "scq_session_driver_test.c"
#include "../apps/server/personal_scq_read_diagnostic_win32.inc"
static SESSION_FIXTURE read_fixture[18];
static void read_reply(char *out,const BYTE attempt[32])
{unsigned i;memcpy(out,"OWNER_SCQ_MACHINE_READ ",23);for(i=0;i<32;i++){out[23+2*i]="0123456789abcdef"[attempt[i]>>4];out[24+2*i]="0123456789abcdef"[attempt[i]&15];}out[87]=0;}
int main(void)
{
 PAPACC_SCQ_READ_DIAGNOSTIC k,copy;PAPACC_PERSONAL_CNG_PROBE p;BYTE public_id[32];char reply[88];unsigned i,before=checks;DWORD e;SESSION_FIXTURE *f;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 memset(public_id,7,sizeof(public_id));
 for(i=0;i<18;i++){
  runtime_service_state=SERVICE_START_PENDING;f=&read_fixture[i];C(!setup_session_machine_read(f,0));memset(&k,0,sizeof(k));
  C(!scqk_prepare(&k,&f->administrator_owner,&f->source,public_id,&f->stop));read_reply(reply,k.attempt);
  if(i==1)reply[23]^=1;if(i==2)f->stop=1;if(i==3)f->source.value.request_id.nFileIndexLow^=1;
  if(i==4)k.policy[0]^=1;if(i==5)receiver_now=k.until;if(i==6)k.key[0]^=1;
  if(i==7){copy=k;C(scqk_confirm(&copy,&f->administrator_owner,reply,87,&f->stop)!=0);}
  if(i==8)k.thread++;
  if(i==9)k.request[0]^=1;if(i==10)k.decision[0]^=1;if(i==11)k.attempt[0]^=1;
  if(i==12)k.until++;if(i==13)f->source.value.retained=1;if(i==14)k.source=NULL;
  e=scqk_confirm(&k,&f->administrator_owner,reply,87,&f->stop);
  if(i>=1&&i<=6||i>=8&&i<=14){C(e!=0&&!k.consumed);continue;}
  C(!e);if(i==15)f->stop=1;if(i==16)receiver_now=k.until;
  e=scqk_take(&k,&f->administrator_owner,&f->stop);C((i==15||i==16)?e!=0:!e);
  if(!e){C(k.consumed&&k.phase==3U);C(scqk_take(&k,&f->administrator_owner,&f->stop)!=0&&k.consumed);}
 }
 /* Ordinary closed READ and a purpose bit cannot acquire independent P. */
 {
  static SESSION_FIXTURE plain;PAPACC_SCQ_PROFILE_DATA cp;PAPACC_SCQ_PLAN_DATA wrong;unsigned z;
  for(z=0;z<5U;z++){
   runtime_service_state=SERVICE_START_PENDING;memset(&k,0,sizeof(k));
   C(!(z==0U?setup_session_mode(&plain,0,1U):setup_session_machine_read(&plain,0)));
   if(z==1U)plain.policy.read_diagnostic=0U;if(z==2U)plain.controller_profile.read_diagnostic=0U;
   if(z==3U)plain.controller_profile.read_diagnostic=2U;if(z==4U)plain.policy.read_diagnostic=2U;
   C(scqk_prepare(&k,&plain.administrator_owner,&plain.source,public_id,&plain.stop)!=0&&!k.consumed);
  }
  memset(&cp,0,sizeof(cp));wrong=plain.plan;wrong.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;memset(wrong.key,0,sizeof(wrong.key));
  C(papacc_scq_profile_prepare_machine_read_data(&cp,&plain.administrator_owner,1U,&wrong,&wrong.administrator)!=0&&!cp.phase&&!cp.read_diagnostic);
 }
 memset(&p,0,sizeof(p));p.phase=3;p.provider_calls=p.open_calls=1;p.open_result=NTE_PERM;p.classification=PAPACC_CNG_PROBE_PERMISSION_OBSERVED;
 C(scqk_result(&p,1)==1U&&!scqk_result(&p,0));p.open_result=NTE_BAD_KEYSET;C(!scqk_result(&p,1));
 p.classification=PAPACC_CNG_PROBE_UNEXPECTED_OPEN;C(scqk_result(&p,1)==2U);p.key=1;C(!scqk_result(&p,1));p.key=0;p.phase=4;C(!scqk_result(&p,1));
 /* K4 internal probe mode is usable only by the future original-bound actor;
  * test the real provider body without asserting that app admission exists. */
 for(i=0;i<5;i++){
  PAPACC_PERSONAL_RUNTIME_IDENTITY r;runtime_service_state=SERVICE_START_PENDING;reset(&r,0,2);C(!papacc_personal_runtime_capture_machine_read_closed(&r,(HANDLE)1,virtual_sid,1));
  memset(&p,0,sizeof(p));qfault=qopens=qproviders=0;qstop=0;
  if(i==1)r.transport_only=1;if(i==2)r.closed_preparation=2;if(i==3)r.closed_preparation=0;
  e=pcp_probe_mode(&p,&r,fixture_name,&qstop,1);
  C(e==PAPACC_CNG_PROBE_LOCAL_DENY||e==PAPACC_CNG_PROBE_QUERY_FAILURE);C(!qproviders&&!qopens); /* private mode without admitted P scope never invokes KSP */
  memset(&p,0,sizeof(p));qproviders=qopens=0;r.closed_preparation=1;r.transport_only=0;r.read_diagnostic=0U;
  C(pcp_probe_mode(&p,&r,fixture_name,&qstop,1)==PAPACC_CNG_PROBE_LOCAL_DENY&&qproviders==0);
  memset(&p,0,sizeof(p));qproviders=qopens=0;r.closed_preparation=1;r.transport_only=0;
  C(papacc_personal_cng_probe(&p,&r,fixture_name,&qstop)==PAPACC_CNG_PROBE_LOCAL_DENY&&qproviders==0);
  memset(&p,0,sizeof(p));qproviders=qopens=0;r.closed_preparation=0;r.transport_only=1;
  C(papacc_personal_cng_probe(&p,&r,fixture_name,&qstop)==PAPACC_CNG_PROBE_LOCAL_DENY&&qproviders==0);
 }
 {
  BYTE frame[128],bad[128],attempt[32];PAPACC_PERSONAL_CNG_PROBE observed;unsigned z;
  memset(attempt,3,32);memset(&p,0,sizeof(p));p.phase=3;p.close_attempted=1;p.close_calls=1;
  p.classification=PAPACC_CNG_PROBE_PERMISSION_OBSERVED;p.provider_calls=p.open_calls=1;
  p.open_result=p.first_error=NTE_PERM;p.first_stage=4;p.first_origin=3;
  C(!scqk_result_encode(&p,attempt,public_id,frame));C(!scqk_result_decode(frame,128,attempt,public_id,&observed));C(scqk_result(&observed,1)==1U&&!scqk_result(&observed,0));
  for(z=0;z<16;z++){memcpy(bad,frame,128);bad[z]^=0x80;C(scqk_result_decode(bad,128,attempt,public_id,&observed)!=0);}
  for(z=64;z<128;z++){memcpy(bad,frame,128);bad[z]^=1;C(scqk_result_decode(bad,128,attempt,public_id,&observed)!=0);}
  C(scqk_result_decode(frame,127,attempt,public_id,&observed)!=0);p.phase=4;C(scqk_result_encode(&p,attempt,public_id,frame)!=0);
 }
 {
  static SESSION_FIXTURE gfixture;PAPACC_SCQ_READ_DIAGNOSTIC act;PAPACC_SCQ_START_ASSURANCE gscope;
  PAPACC_SCM_RESOURCE_SAMPLE sample;PAPACC_SCQ_START_LIMITS limits;BYTE policy[84],old[84];DWORD counts[9]={127,128,129,187,188,189,191,192,193};unsigned z;
  for(z=0;z<9;z++){
   runtime_service_state=SERVICE_START_PENDING;C(!setup_session_machine_read(&gfixture,0));memset(&act,0,sizeof(act));
   C(!scqk_prepare(&act,&gfixture.administrator_owner,&gfixture.source,public_id,&gfixture.stop));memset(&gscope,0,sizeof(gscope));memset(&sample,0,sizeof(sample));
   sample.frequency=1000;sample.begin=sample.end=100;sample.private_bytes=sample.working_set=4096;sample.physical_available=7239172096ULL;sample.commit_available=3221225472ULL;sample.handles=z<3?counts[z]:100;
   e=scqk_resource_ready(&act,&gfixture.administrator_owner,&gscope,&sample,100,100,act.until,&gfixture.stop);
   if(z<3){C(z==2?e!=0:!e);continue;}C(!e);sample.begin=sample.end=101;sample.handles=counts[z];
   e=scqk_resource_admit(&act,&gfixture.administrator_owner,&gscope,&sample,101,101,z<6,&gfixture.stop);
   C((z==4||z==5||z==8)?e!=0:!e);
  }
  scqg_selected_p1(&limits);C(!scqk_policy_preimage(&act,&gfixture.administrator_owner,&limits,policy,&gfixture.stop));
  C(!scqg_plan_data_mode(&limits,240000U,old,1)&&memcmp(policy,old,84));limits.max_handles=193;C(scqk_policy_preimage(&act,&gfixture.administrator_owner,&limits,policy,&gfixture.stop)!=0);
  C(scqk_resource_ready(NULL,&gfixture.administrator_owner,&gscope,&sample,101,101,act.until,&gfixture.stop)!=0);
 }
 {
  static SESSION_FIXTURE typed;PAPACC_SCQ_START_ASSURANCE g,old;PAPACC_SCQ_START_LIMITS limits;
  PAPACC_SCM_RESOURCE_SAMPLE s;char q[75],ep[100],st[100];unsigned z,j;DWORD error;
  static const char *prefix[3]={"OWNER_SCQ ","OWNER_SCQ_PRIVATE_ENDPOINT ","OWNER_SCQ_DIAGNOSTIC_START "};
  for(z=0;z<6U;z++){
   runtime_service_state=SERVICE_START_PENDING;memset(&g,0,sizeof(g));memset(&old,0,sizeof(old));memset(&k,0,sizeof(k));scqg_selected_p1(&limits);
   C(!setup_session_typed_capture(&typed,1,1U,PAPACC_SCQ_PURPOSE_READ,z!=1U,0));
   typed.policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";typed.policy.parent=L"";typed.policy.data=L"";
   if(z==2U)typed.policy.read_diagnostic=0;if(z==3U)typed.stop=1;if(z==4U)limits.max_handles=193;
   C(scqg_capture_mode(&old,&typed.administrator_owner,&typed.source,&limits,&typed.stop,1)!=0&&!old.phase);
   error=scqg_capture_machine_read(&g,&typed.administrator_owner,&typed.source,&limits,&typed.stop);
   if(z>=1U&&z<=4U){C(error!=0&&!g.phase);continue;}C(!error&&g.read_diagnostic&&g.p1_handles);
   g.initial_known=1;g.initial_handles=128;C(!scqg_presented(&g,&typed.administrator_owner,&typed.stop));
   for(j=0;j<3U;j++){
    char *out=j==0U?q:j==1U?ep:st;unsigned n=(unsigned)strlen(prefix[j]),hindex;
    memcpy(out,prefix[j],n);for(hindex=0;hindex<32U;hindex++){out[n+2U*hindex]="0123456789abcdef"[typed.plan.attempt[hindex]>>4];out[n+2U*hindex+1U]="0123456789abcdef"[typed.plan.attempt[hindex]&15];}out[n+64U]=0;
   }
   C(!scqa_present(&typed.source,&typed.administrator_owner,&typed.stop));C(!scqa_confirm(&typed.source,&typed.administrator_owner,q,74,&typed.stop));
   C(!typed.source.endpoint_phase&&!typed.source.start_phase&&!k.consumed);
   C(!scqa_endpoint_present(&typed.source,&typed.administrator_owner,&typed.stop));C(!scqa_endpoint_confirm(&typed.source,&typed.administrator_owner,ep,strlen(ep),&typed.stop));
   C(!typed.source.start_phase&&!typed.source.endpoint_consumed);
   C(!scqa_start_present(&typed.source,&typed.administrator_owner,&typed.stop));C(!scqa_start_confirm(&typed.source,&typed.administrator_owner,st,strlen(st),&typed.stop));
   C(!scqk_prepare(&k,&typed.administrator_owner,&typed.source,public_id,&typed.stop));C(!k.consumed&&!typed.source.start_consumed);
   memset(&s,0,sizeof(s));s.frequency=1000;s.begin=s.end=100;s.private_bytes=s.working_set=4096;s.handles=118;s.physical_available=7239172096ULL;s.commit_available=3221225472ULL;
   if(z==5U)s.handles=188;error=scqg_ready(&g,&typed.administrator_owner,&s,100,&typed.stop);
   C(z==5U?error!=0:!error);C(!typed.source.start_consumed&&!typed.source.endpoint_consumed&&!k.consumed);
   if(!error){C(g.phase==2U);C(!scqg_controller_deadline(&g,100));C(g.global_until==10100);}
  }
 }
 C(LocalFree(virtual_sid)==NULL);C(LocalFree(sam_sid)==NULL);
 printf("PASS READ_DIAGNOSTIC acts=18 probe_modes=5 checks=%u SOURCE_ACTORS_AND_PROVIDER_MOCKED NO_NATIVE_AUTHORITY_OR_KSP\n",checks-before);return 0;
}
