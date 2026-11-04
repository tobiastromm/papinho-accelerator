/* Two logical packets in one isolated model; NEVER a native peer/endpoint. */
#define PAPACC_SCQ_ADMIN_ENTRY preserved_admin_source_main
#include "scq_admin_source_test.c"
#include "../apps/server/personal_scq_session_win32.inc"
#ifndef PAPACC_SCQ_TRANSPORT_IMPLEMENTATION
#include "../apps/server/personal_scq_transport_win32.inc"
#endif
typedef struct SESSION_FIXTURE {
 int administrator_owner,runtime_owner;PAPACC_SCQ_PLAN_DATA plan;
 PAPACC_SCQ_PROFILE_DATA controller_profile,runtime_profile;
 PAPACC_SCQ_ADMIN_SOURCE source;PAPACC_SCQ_OWNER_DECISION policy;
 PAPACC_PERSONAL_SCQ_ORIGINAL original,forwarded;
 PAPACC_PERSONAL_RUNTIME_IDENTITY runtime;
 PAPACC_PERSONAL_SCQ_DISPATCH controller_dispatch,runtime_dispatch;
 PAPACC_PERSONAL_SCQ_RECEIVER controller_receiver,runtime_receiver;
 PAPACC_SCQ_SESSION_DRIVER controller,recipient;volatile LONG stop;
 DWORD machine_read,model_effects;BYTE changed[PAPACC_SCQ_RECEIPT_BYTES];
} SESSION_FIXTURE;
static SESSION_FIXTURE fixtures[35];static BYTE machine_read_policy[32];
static int observed_runtime_identity_mode(SESSION_FIXTURE *f,unsigned closed)
{
 PAPACC_SCQ_PEER_DATA *p=&f->plan.runtime;
 C(!(f->machine_read?papacc_personal_runtime_capture_machine_read_closed(&f->runtime,(HANDLE)1,virtual_sid,1):closed?papacc_personal_runtime_capture_closed(&f->runtime,(HANDLE)1,virtual_sid,1):papacc_personal_runtime_capture(&f->runtime,(HANDLE)1,virtual_sid,2,1)));
 p->service_state=closed?SERVICE_START_PENDING:SERVICE_RUNNING;
 p->pid=f->runtime.pid;p->creation[0]=f->runtime.creation.dwLowDateTime;p->creation[1]=f->runtime.creation.dwHighDateTime;
 memcpy(p->token_id,&f->runtime.statistics.TokenId,sizeof(LUID));memcpy(p->authentication,&f->runtime.statistics.AuthenticationId,sizeof(LUID));memcpy(p->modified,&f->runtime.statistics.ModifiedId,sizeof(LUID));
 p->groups=p->enumerated=f->runtime.group_count;p->matches=f->runtime.specific_matches;p->attributes=f->runtime.specific_attributes;
 return 0;
}
static int observed_runtime_mode(SESSION_FIXTURE *f,unsigned closed)
{
 C(!observed_runtime_identity_mode(f,closed));
 if(f->plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY){WCHAR empty[112]={0};
  C(!papacc_personal_scq_prepare_transport(&f->controller_dispatch,&f->runtime,f->plan.attempt,empty));
  C(!papacc_personal_scq_prepare_transport(&f->runtime_dispatch,&f->runtime,f->plan.attempt,empty));
 }else{C(!papacc_personal_scq_prepare(&f->controller_dispatch,&f->runtime,f->plan.attempt,fixture_name));
 C(!papacc_personal_scq_prepare(&f->runtime_dispatch,&f->runtime,f->plan.attempt,fixture_name));}
 C(!(f->machine_read?papacc_scq_profile_prepare_machine_read_data(&f->runtime_profile,&f->runtime_owner,2,&f->plan,&f->plan.runtime):f->plan.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?papacc_scq_profile_prepare_transport_data(&f->runtime_profile,&f->runtime_owner,2,&f->plan,&f->plan.runtime):closed?papacc_scq_profile_prepare_closed_data(&f->runtime_profile,&f->runtime_owner,2,&f->plan,&f->plan.runtime):papacc_scq_profile_prepare_data(&f->runtime_profile,&f->runtime_owner,2,&f->plan,&f->plan.runtime)));
 C(!scqs_prepare(&f->recipient,&f->runtime_owner,2,&f->runtime_profile,NULL,&f->runtime_receiver,&f->runtime_dispatch,&f->runtime));return 0;
}
static int observed_runtime(SESSION_FIXTURE *f){return observed_runtime_mode(f,0U);}
static int setup_session_typed_capture(SESSION_FIXTURE *f,int future,unsigned closed,DWORD purpose,DWORD machine_read,int confirmed)
{
 PAPACC_PERSONAL_SCQ_ORIGINAL *v=&f->original;PAPACC_SCQ_PLAN_DATA controller_plan;DWORD n;unsigned j;char reply[75];
 memset(f,0,sizeof(*f));f->machine_read=machine_read;reset(&f->runtime,0,2);receiver_fault=0;receiver_now=100;qfault=0;qstop=0;
 C(scq_fixture_plan(&f->plan));f->plan.purpose=purpose;if(purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)memset(f->plan.key,0,sizeof(f->plan.key));
 f->plan.administrator.pid=88;f->plan.administrator.creation[0]=99;f->plan.administrator.token_id[0]=1;f->plan.administrator.authentication[0]=0;f->plan.administrator.modified[0]=3;
 memset(f->plan.administrator.sid,0,68);f->plan.administrator.sid_bytes=GetLengthSid(sam_sid);CopySid(68,f->plan.administrator.sid,sam_sid);
 memset(f->plan.runtime.sid,0,68);f->plan.runtime.sid_bytes=GetLengthSid(virtual_sid);CopySid(68,f->plan.runtime.sid,virtual_sid);
 for(j=0;j<3;j++){f->plan.refs[j].volume=10;f->plan.refs[j].low=41+j;f->plan.refs[j].size_low=8;f->plan.refs[j].attributes=0;}
 if(machine_read){f->plan.administrator.image[0]=10;f->plan.administrator.image[1]=0;f->plan.administrator.image[2]=43;memcpy(f->plan.administrator.image_digest,f->plan.refs[2].digest,32);if(scqp_nonzero(machine_read_policy,32))memcpy(f->plan.policy,machine_read_policy,32);}
 f->plan.runtime.image[0]=10;f->plan.runtime.image[2]=43;memcpy(f->plan.request,f->plan.refs[0].digest,32);memcpy(f->plan.decision,f->plan.refs[1].digest,32);
 if(!future){C(!observed_runtime_mode(f,closed));controller_plan=f->plan;}
 else{controller_plan=f->plan;memset(&controller_plan.runtime,0,sizeof(controller_plan.runtime));controller_plan.runtime.sid_bytes=f->plan.runtime.sid_bytes;memcpy(controller_plan.runtime.sid,f->plan.runtime.sid,68);}
 C(!(machine_read?papacc_scq_profile_prepare_machine_read_data(&f->controller_profile,&f->administrator_owner,1,&controller_plan,&controller_plan.administrator):purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?papacc_scq_profile_prepare_transport_data(&f->controller_profile,&f->administrator_owner,1,&controller_plan,&controller_plan.administrator):closed?papacc_scq_profile_prepare_closed_data(&f->controller_profile,&f->administrator_owner,1,&controller_plan,&controller_plan.administrator):papacc_scq_profile_prepare_data(&f->controller_profile,&f->administrator_owner,1,&controller_plan,&controller_plan.administrator)));
 registration(&f->policy,&controller_plan);f->policy.read_diagnostic=machine_read;f->policy.controller=&f->administrator_owner;f->policy.source=&f->source;
 v->original=v;v->controller=&f->administrator_owner;v->recipient=&f->runtime_owner;v->purpose=purpose;v->deadline=((ULONGLONG)controller_plan.valid_until[1]<<32)|controller_plan.valid_until[0];
 v->request=(HANDLE)41;v->decision=(HANDLE)42;v->image=(HANDLE)43;v->operator_process=(HANDLE)44;v->operator_pid=88;v->operator_creation.dwLowDateTime=99;v->operator_token=(HANDLE)45;
 receiver_file(v->request,&v->request_id);receiver_file(v->decision,&v->decision_id);receiver_file(v->image,&v->image_id);
 CopySid(68,v->operator_sid,sam_sid);v->operator_sid_bytes=GetLengthSid(sam_sid);receiver_token(v->operator_token,TokenStatistics,&v->operator_statistics,sizeof(v->operator_statistics),&n);
 C(!scqa_capture(&f->source,&f->administrator_owner,&f->policy,&f->controller_profile,v,&f->stop));if(!confirmed)return 0;C(!scqa_present(&f->source,&f->administrator_owner,&f->stop));
 memcpy(reply,"OWNER_SCQ ",10);for(j=0;j<32;j++){reply[10+2*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[11+2*j]="0123456789abcdef"[f->plan.attempt[j]&15];}reply[74]=0;
 C(!scqa_confirm(&f->source,&f->administrator_owner,reply,74,&f->stop));return 0;
}
static int setup_session_typed(SESSION_FIXTURE *f,int future,unsigned closed,DWORD purpose,DWORD machine_read){return setup_session_typed_capture(f,future,closed,purpose,machine_read,1);}
static int setup_session_purpose(SESSION_FIXTURE *f,int future,unsigned closed,DWORD purpose){return setup_session_typed(f,future,closed,purpose,0U);}
static int setup_session_machine_read(SESSION_FIXTURE *f,int future){return setup_session_typed(f,future,1U,PAPACC_SCQ_PURPOSE_READ,1U);}
static int setup_session_mode(SESSION_FIXTURE *f,int future,unsigned closed){return setup_session_purpose(f,future,closed,PAPACC_SCQ_PURPOSE_READ);}
static int setup_session(SESSION_FIXTURE *f,int future){return setup_session_mode(f,future,0U);}
static DWORD WINAPI session_foreign(LPVOID value)
{SESSION_FIXTURE *f=value;return scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop);}
#ifndef PAPACC_SCQ_SESSION_ENTRY
#define PAPACC_SCQ_SESSION_ENTRY main
#endif
int PAPACC_SCQ_SESSION_ENTRY(void)
{
 SESSION_FIXTURE *f;PAPACC_SCQ_PEER_DATA wrong;PAPACC_PERSONAL_SCQ_ORIGINAL *proof;PAPACC_SCQ_SESSION_DRIVER copied;
 unsigned i,start=checks,retained=0;DWORD e,result;HANDLE worker;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<35;i++){
  f=&fixtures[i];C(!setup_session(f,i==31));
  if(i==3)f->source.phase=2;
  e=scqs_prepare(&f->controller,&f->administrator_owner,1,&f->controller_profile,i==1?NULL:&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime);
  if(i==1||i==3){C(e!=0&&!f->source.value.controller_consumed);goto done;}
  C(!e);copied=f->controller;C(scqs_take(&copied,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop)==ERROR_INVALID_STATE);
  if(i==2)f->source.registration=NULL;if(i==6)f->stop=1;
  if(i==18){C(scqs_peer_lost(&f->controller,&f->administrator_owner)!=0&&!f->source.value.controller_consumed);goto done;}
  if(i==25){use_native_thread=1;f->controller.thread=GetCurrentThreadId();worker=CreateThread(NULL,0,session_foreign,f,0,NULL);C(worker!=NULL);C(WaitForSingleObject(worker,10000)==WAIT_OBJECT_0);C(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE&&!f->source.value.controller_consumed);C(native_thread_close(worker));use_native_thread=0;f->controller.thread=thread;}
  if(i==30)C(!scqa_take(&f->source,&f->administrator_owner,&f->stop));
  wrong=f->plan.administrator;if(i==4)wrong.token_id[0]++;
  e=scqs_take(&f->controller,&f->administrator_owner,&wrong,i==31?NULL:&f->plan.runtime,f->plan.refs,&f->stop);
  if(i==2||i==4||i==6||i==30){C(e!=0);if(i!=30)C(!f->source.value.controller_consumed);goto done;}
  C(!e&&f->source.value.controller_consumed&&f->controller_profile.taken);
  if(i==5)C(scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop)!=0);
  if(i==31){C(!f->runtime.phase&&!f->controller_dispatch.phase&&!f->runtime_profile.phase&&f->controller_profile.waiting_runtime);f->model_effects++;C(!observed_runtime(f));}
  if(i==7)f->stop=1;if(i==26)f->runtime.pid++;
  wrong=f->plan.runtime;if(i==8)wrong.token_id[0]++;if(i==34)wrong.service_sid_type^=1;
  e=scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&wrong,f->plan.refs,&f->stop);
  if(i==7||i==8||i==26||i==34){C(e!=0&&f->source.value.controller_consumed);goto done;}
  C(!e&&f->controller.bytes==PAPACC_SCQ_RECORD_BYTES);
  if(i==14)C(scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop)!=0);
  if(i==29)C(scqs_offer(&f->controller,&f->runtime_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop)!=0);
  if(i==32){
   memcpy(f->changed,"SQR1",4);memcpy(f->changed+4,f->controller.buffer,PAPACC_SCQ_RECORD_BYTES);
   C(!scqs_ack(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,f->changed,PAPACC_SCQ_RECEIPT_BYTES,&f->stop));
   C(f->controller.phase==4&&!f->source.value.consumed&&!f->runtime_receiver.phase&&!f->model_effects);
   /* Echoed nonsecret bytes prove DATA matching only, NOT recipient execution
    * or native authentication. No API here grants a protected operation. */
   goto done;
  }
  if(i==17){C(scqs_peer_lost(&f->controller,&f->administrator_owner)!=0&&f->controller.phase==6&&f->source.phase==6);goto done;}
  proof=&f->source.value;if(i==9)proof->recipient=&f->administrator_owner;if(i==10)proof=NULL;
  if(i==28){f->forwarded=f->source.value;proof=&f->forwarded;}
  memcpy(f->changed,f->controller.buffer,f->controller.bytes);if(i==11)f->changed[0]^=1;
  if(i==12)f->runtime_profile.record[4]=1;if(i==24)receiver_now=f->plan.valid_until[0];
  wrong=f->plan.runtime;if(i==13)wrong.pid++;
  e=scqs_receive(&f->recipient,&f->runtime_owner,&wrong,&f->plan.administrator,f->plan.refs,f->changed,PAPACC_SCQ_RECORD_BYTES,proof,&f->stop);
  if((i>=9&&i<=13)||i==24||i==28){C(e!=0);goto done;}
  C(!e&&f->source.value.consumed&&f->source.value.borrowed&&f->runtime_receiver.phase==2);
  if(i==15)C(scqs_receive(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,f->controller.buffer,PAPACC_SCQ_RECORD_BYTES,proof,&f->stop)!=0);
  if(i==16)f->stop=1;if(i==33)receiver_fault=5;
  e=scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop);
  if(i==16||i==33){C(e!=0&&f->recipient.phase==6&&f->source.value.retained);goto done;}
  C(!e&&f->recipient.bytes==PAPACC_SCQ_RECEIPT_BYTES);
  if(i==20)C(scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop)!=0);
  memcpy(f->changed,f->recipient.buffer,f->recipient.bytes);if(i==19)f->changed[0]^=1;if(i==27)f->policy.decision[0]^=1;
  e=scqs_ack(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,f->changed,PAPACC_SCQ_RECEIPT_BYTES,&f->stop);
  if(i==19||i==27){C(e!=0&&f->controller.phase==6);goto done;}
  C(!e&&f->controller.phase==4); /* DATA ACK, never native authentication */
  if(i==21)C(scqs_ack(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,f->changed,PAPACC_SCQ_RECEIPT_BYTES,&f->stop)!=0);
  if(i==22){C(scqs_close(&f->controller,&f->administrator_owner,1)!=0&&f->controller.phase==6&&f->source.value.borrowed);goto done;}
  if(i==23){C(scqs_close(&f->recipient,&f->runtime_owner,0)!=0&&f->recipient.phase==6&&f->source.value.retained);goto done;}
  C(!scqs_close(&f->recipient,&f->runtime_owner,1));C(!f->source.value.borrowed);C(!scqs_close(&f->controller,&f->administrator_owner,1));
  C(f->source.value.controller_consumed&&f->source.value.consumed);C(!scqs_close(&f->controller,&f->administrator_owner,1));
  C(scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop)!=0);
done:
  receiver_now=100;f->stop=0;receiver_fault=fault=0;
  if(f->recipient.phase&&f->recipient.phase!=5&&f->recipient.phase!=6)(void)scqs_close(&f->recipient,&f->runtime_owner,1);
  if(f->controller.phase&&f->controller.phase!=5&&f->controller.phase!=6)(void)scqs_close(&f->controller,&f->administrator_owner,1);
  if(f->controller.phase==6||f->recipient.phase==6){retained++;C(scqs_close(f->controller.phase==6?&f->controller:&f->recipient,f->controller.phase==6?&f->administrator_owner:&f->runtime_owner,1)!=0);}
  else{if(f->source.phase!=5)C(!scqa_close(&f->source,&f->administrator_owner,1));if(f->runtime.phase)C(!papacc_personal_runtime_close(&f->runtime));}
 }
 printf("PASS SCQ_SESSION_DRIVER cases=35 checks=%u retained_models=%u driver_bytes=%zu buffer_bytes=%u MODEL_PEERS_DECISIONS_IO NO_ENDPOINT ECHO_IS_DATA_ONLY NO_NATIVE_AUTHENTICATED_ACK\n",checks-start,retained,sizeof(fixtures[0].controller),(unsigned)sizeof(fixtures[0].controller.buffer));
 /* Static fixtures/real SID backing deliberately survive UNKNOWN cases. */
 return 0;
}
