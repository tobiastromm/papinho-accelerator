/* Real runtime/profile adapters, all process/token/SCM edges modeled. */
#define main preserved_runtime_token_main
#include "personal_runtime_identity_test.c"
#undef main
#include "scq_profile_fixture.h"
int main(void)
{
 PAPACC_PERSONAL_RUNTIME_IDENTITY r;PAPACC_SCQ_RUNTIME_OBSERVATION v;
 PAPACC_SCQ_PLAN_DATA p,q;PSID admin=NULL;volatile LONG stop=0;unsigned i;DWORD result;
 C(ConvertStringSidToSidA("S-1-5-6",&service_sid));
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));
 C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(ConvertStringSidToSidA("S-1-5-32-544",&admin));
 runtime_service_state=SERVICE_START_PENDING;
 for(i=0;i<2;i++){
  reset(&r,0,2);shape_elevation=i;observation_service=1;
  C(!papacc_personal_runtime_capture_transport_closed(&r,(HANDLE)1,virtual_sid,1));
  C(r.transport_only==1&&r.observed_elevation==i);
  C(!papacc_personal_runtime_observe_data(&r,&v,&stop)&&v.peer.elevated==i);
  C(v.peer.elevation_type==TokenElevationTypeDefault);
  C(papacc_personal_runtime_capture_transport_closed(&r,(HANDLE)1,virtual_sid,1)!=0);
  shape_elevation=i?0:1;C(papacc_personal_runtime_current(&r)!=0);
  C(!papacc_personal_runtime_close(&r));
 }
 /* Neither legacy READ closed identity nor SAM acquires this policy. */
 reset(&r,3,2);C(papacc_personal_runtime_capture_closed(&r,(HANDLE)1,virtual_sid,1)!=0);fault=0;C(!papacc_personal_runtime_close(&r));
 runtime_service_state=SERVICE_RUNNING;
 reset(&r,3,2);C(papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1)!=0);fault=0;C(!papacc_personal_runtime_close(&r));
 reset(&r,3,1);C(papacc_personal_runtime_capture(&r,(HANDLE)1,sam_sid,1,0)!=0);fault=0;C(!papacc_personal_runtime_close(&r));
 runtime_service_state=SERVICE_START_PENDING;
 reset(&r,0,2);shape_elevation=2;C(papacc_personal_runtime_capture_transport_closed(&r,(HANDLE)1,virtual_sid,1)!=0);C(!papacc_personal_runtime_close(&r));
 reset(&r,0,2);shape_elevation=1;observation_service=1;shape_admin_sid=admin;
 C(papacc_personal_runtime_capture_transport_closed(&r,(HANDLE)1,virtual_sid,1)!=0);shape_admin_sid=NULL;C(!papacc_personal_runtime_close(&r));
 for(i=0;i<5;i++){
  static const unsigned bad[]={2,7,8,19,23};reset(&r,bad[i],2);shape_elevation=1;
  C(papacc_personal_runtime_capture_transport_closed(&r,(HANDLE)1,virtual_sid,1)!=0);fault=0;C(!papacc_personal_runtime_close(&r));
 }
 C(scq_fixture_plan(&p));p.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;
 memset(p.key,0,sizeof(p.key));p.runtime.service_state=SERVICE_START_PENDING;p.runtime.elevated=1;
 C(scqp_plan_state(&p,SERVICE_START_PENDING));
 q=p;q.runtime.elevated=2;C(!scqp_plan_state(&q,SERVICE_START_PENDING));
 q=p;q.runtime.elevation_type=TokenElevationTypeFull;C(!scqp_plan_state(&q,SERVICE_START_PENDING));
 q=p;q.runtime.administrator_attributes=SE_GROUP_ENABLED;C(!scqp_plan_state(&q,SERVICE_START_PENDING));
 q=p;q.runtime.restricted=1;C(!scqp_plan_state(&q,SERVICE_START_PENDING));
 q=p;q.runtime.session=1;C(!scqp_plan_state(&q,SERVICE_START_PENDING));
 q=p;q.runtime.logon=2;C(!scqp_plan_state(&q,SERVICE_START_PENDING));
 q=p;q.purpose=PAPACC_SCQ_PURPOSE_READ;C(!scqp_plan_state(&q,SERVICE_START_PENDING));
 C(scq_fixture_plan(&q));q.runtime.elevated=1;C(!scqp_plan_state(&q,SERVICE_RUNNING));
 /* K0 typed READ only: preserve raw elevation and reject administrative
  * group membership, corruption, replay and implicit legacy fallback. */
 for(i=0;i<4U;i++){
  reset(&r,0,2);shape_elevation=i<2U?i:1U;observation_service=1;
  if(i==2U)shape_admin_sid=admin;
  if(i==3U)shape_elevation=2U;
  result=papacc_personal_runtime_capture_machine_read_closed(&r,(HANDLE)1,virtual_sid,1);
  if(i<2U){C(!result&&r.read_diagnostic==1U&&!r.transport_only&&r.observed_elevation==i);
   C(!papacc_personal_runtime_observe_data(&r,&v,&stop)&&v.peer.elevated==i);
   C(papacc_personal_runtime_capture_machine_read_closed(&r,(HANDLE)1,virtual_sid,1)!=0);
   shape_elevation=i?0U:1U;C(papacc_personal_runtime_current(&r)!=0);
  }else C(result!=0);
  shape_admin_sid=NULL;C(!papacc_personal_runtime_close(&r));
 }
 {
  PAPACC_SCQ_PROFILE_DATA typed,ordinary;BYTE bytes[PAPACC_SCQ_RECORD_BYTES];int owner;
  memset(&typed,0,sizeof(typed));memset(&ordinary,0,sizeof(ordinary));
  C(scq_fixture_plan(&q));q.runtime.service_state=SERVICE_START_PENDING;q.runtime.elevated=1U;
  C(!scqp_plan_state(&q,SERVICE_START_PENDING)&&scqp_plan_state_mode(&q,SERVICE_START_PENDING,1U));
  C(papacc_scq_profile_prepare_closed_data(&ordinary,&owner,2U,&q,&q.runtime)!=0);
  C(!papacc_scq_profile_prepare_machine_read_data(&typed,&owner,2U,&q,&q.runtime)&&typed.read_diagnostic==1U);
  C(!scqp_decode_fields(typed.record,sizeof(typed.record),&p)&&p.runtime.elevated==1U);
  C(papacc_scq_plan_encode(&q,bytes,sizeof(bytes))!=0); /* public/ordinary contract unchanged */
  q.runtime.elevated=2U;C(!scqp_plan_state_mode(&q,SERVICE_START_PENDING,1U));
  q.runtime.elevated=1U;q.runtime.elevation_type=TokenElevationTypeFull;C(!scqp_plan_state_mode(&q,SERVICE_START_PENDING,1U));
 }
 C(!LocalFree(admin)&&!LocalFree(service_sid)&&!LocalFree(virtual_sid)&&!LocalFree(sam_sid));
 printf("PASS RUNTIME_TOKEN_SHAPE checks=%u RAW_VALUE_PRESERVED_READ_SAM_UNCHANGED_NO_REAL_PROCESS_TOKEN_SCM\n",checks);return 0;
}
