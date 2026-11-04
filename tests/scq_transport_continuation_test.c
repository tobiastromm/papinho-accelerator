/* Same native continuation; every SCM/token/transport/RNG/KSP effect is modeled.
 * No endpoint, service, credential or production operation is executed. */
#define PAPACC_SCQ_RECIPIENT_ENTRY preserved_close_test_main
#include "scq_recipient_close_test.c"
static SESSION_FIXTURE tf[22];static PAPACC_SCQ_RECIPIENT_ORIGIN tr[22];static PAPACC_PERSONAL_SCQ_INPUTS ti[22];
static DWORD step(SESSION_FIXTURE *f,PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_PERSONAL_SCQ_INPUTS *in,int recipient)
{
 DWORD e,before=mcalls;mpeer=recipient?f->plan.administrator.pid:f->plan.runtime.pid;
 e=scqr_continue_transport(recipient?&f->recipient:&f->controller,recipient?&f->runtime_owner:&f->administrator_owner,recipient?r:NULL,recipient?in:NULL,recipient?&r->value:&f->source.value,&f->stop);
 if(mcalls-before>1U){printf("FAIL more than one IO per step\n");return ERROR_GEN_FAILURE;}return e;
}
static int advance(SESSION_FIXTURE *f,PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_PERSONAL_SCQ_INPUTS *in,int recipient,DWORD leg)
{
 unsigned k;DWORD e;mposition=0;
 for(k=0;k<1024U;k++){
  e=step(f,r,in,recipient);
  C(e==0||e==ERROR_MORE_DATA||e==ERROR_IO_PENDING);
  if((recipient?f->recipient.transport.legs:f->controller.transport.legs)&leg)return 0;
 }
 return 1; /* Harness bound only, not an operational duration/iteration policy. */
}
static int pair(SESSION_FIXTURE *f,PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_PERSONAL_SCQ_INPUTS *in)
{
 unsigned j;runtime_service_state=SERVICE_START_PENDING;mcase=mcalls=mcloses=mposition=mincomplete=mcancel=mremote_closed=0;origin_fault=receiver_identity_alias=0;
 C(!setup_session_purpose(f,0,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
 C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
 C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
 C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
 in->phase=1;in->content_verified=1;in->reader[0]=(HANDLE)41;in->reader[1]=(HANDLE)42;in->reader[2]=(HANDLE)43;
 in->identity[0]=f->original.request_id;in->identity[1]=f->original.decision_id;in->identity[2]=f->original.image_id;
 for(j=0;j<3U;j++){memcpy(in->digest[j],f->plan.refs[j].digest,32);memcpy(in->descriptor_digest[j],f->plan.refs[j].descriptor,32);}
 C(!scqr_acquire(r,&f->runtime_owner,&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,(HANDLE)44,&f->stop));
 mpeer=f->plan.runtime.pid;
 C(!scqt_prepare(&f->controller,&f->administrator_owner,(HANDLE)101,(HANDLE)102,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
 receiver_identity_alias=1U;
 C(!scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
 return 0;
}
static DWORD WINAPI foreign_step(LPVOID p)
{SESSION_FIXTURE *f=p;return scqr_continue_transport(&f->controller,&f->administrator_owner,NULL,NULL,&f->source.value,&f->stop);}
int main(void)
{
 unsigned i,j,start=checks,complete=0,retained=0;DWORD e,n,before,result;HANDLE moved[3],worker;
 SESSION_FIXTURE *f;PAPACC_SCQ_RECIPIENT_ORIGIN *r;PAPACC_PERSONAL_SCQ_INPUTS *in;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<22U;i++){
  f=&tf[i];r=&tr[i];in=&ti[i];C(!pair(f,r,in));
  if(i==3U){f->source.registration=NULL;C(step(f,r,in,0)!=0);retained++;continue;}
  if(i==4U){f->runtime.creation.dwLowDateTime++;C(step(f,r,in,0)!=0);retained++;continue;}
  if(i==5U){f->controller_dispatch.purpose=PAPACC_SCQ_PURPOSE_READ;C(step(f,r,in,0)!=0);retained++;continue;}
  if(i==6U){f->stop=1;C(step(f,r,in,0)!=0);retained++;continue;}
  if(i==7U){mcase=12;C(step(f,r,in,0)==ERROR_IO_PENDING);f->stop=1;C(step(f,r,in,0)==ERROR_CANCELLED&&f->controller.transport.pending&&mcancel==1U);C(step(f,r,in,0)!=0&&mcancel==1U);retained++;continue;}
  if(i==8U){f->source.value.purpose=PAPACC_SCQ_PURPOSE_READ;C(step(f,r,in,0)!=0);retained++;continue;}
  if(i==9U){use_native_thread=1;f->controller.thread=f->controller.transport.thread=GetCurrentThreadId();before=mcalls;
   worker=CreateThread(NULL,0,foreign_step,f,0,NULL);C(worker!=NULL);C(native_wait(worker,10000)==WAIT_OBJECT_0);C(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE&&before==mcalls);C(native_thread_close(worker));use_native_thread=0;f->controller.thread=f->controller.transport.thread=thread;}
  mcase=i==1U?1U:i==2U?2U:0U;
  C(!advance(f,r,in,0,1U));
  mpeer=f->plan.administrator.pid;
  C(!scqr_open(r,&f->recipient,&f->runtime_owner,in,L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.0123456789abcdef0123456789abcdef",&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
  mposition=0;
  for(j=0;j<1024U;j++){e=step(f,r,in,1);if(e==ERROR_NOT_READY)break;C(e==ERROR_IO_PENDING||e==ERROR_MORE_DATA);}
  C(j<1024U&&r->phase==2U&&!f->recipient.transport.query_only&&f->recipient.phase==1U);
  if(i==10U){before=mcalls;C(step(f,r,in,1)==ERROR_NOT_READY&&mcalls==before);retained++;continue;}
  moved[0]=(HANDLE)541;moved[1]=(HANDLE)542;moved[2]=(HANDLE)543;mpeer=f->plan.administrator.pid;
  C(!scqt_accept_refs(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
  C(!scqt_adopt_delivery(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
  if(i==11U){f->recipient.peer_lost=1U;C(step(f,r,in,1)!=0);retained++;continue;}
  C(step(f,r,in,1)==ERROR_MORE_DATA&&f->recipient.phase==3U);
  if(i==12U){r->value.key[111]=L'X';C(step(f,r,in,1)!=0);retained++;continue;}
  C(!advance(f,r,in,1,1U));
  C(!advance(f,r,in,0,2U));
  if(i==13U)mcase=13U;
  e=step(f,r,in,1);
  if(i==13U){C(e!=0&&f->recipient.transport.close_produced==2U);before=mcloses;C(step(f,r,in,1)!=0&&mcloses==before);retained++;continue;}
  C(e==ERROR_MORE_DATA&&f->recipient.transport.close_produced==1U);
  C(!advance(f,r,in,1,4U));
  if(i==14U)wire[PAPACC_SCQ_RECEIPT_BYTES]^=1U;
  if(i==14U){mposition=0;C(step(f,r,in,0)!=0&&!f->controller.transport.close_accepted);retained++;continue;}
  C(!advance(f,r,in,0,8U));C(f->controller.transport.close_accepted&&f->recipient.transport.close_produced==3U);
  C(!f->recipient.transport.close_confirmed);C(scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop)!=0);
  if(i==21U){f->controller.buffer[0]^=1U;before=mcalls;mposition=0;C(step(f,r,in,0)!=0&&mcalls==before);retained++;continue;}
  C(!advance(f,r,in,0,16U));C(!f->recipient.transport.close_confirmed);
  if(i==16U){wire[PAPACC_SCQ_RECEIPT_BYTES]^=1U;mposition=0;C(step(f,r,in,1)!=0&&!f->recipient.transport.close_confirmed);retained++;continue;}
  if(i==17U){mcase=5;mposition=0;C(step(f,r,in,1)!=0&&!f->recipient.transport.close_confirmed);retained++;continue;}
  if(i==18U){f->runtime_dispatch.purpose=PAPACC_SCQ_PURPOSE_READ;before=mcalls;
   C(scqt_start_io(&f->recipient,&f->runtime_owner,32U,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop)!=0&&mcalls==before);retained++;continue;}
  if(i==20U){mcase=12;mposition=0;C(step(f,r,in,1)==ERROR_IO_PENDING);f->stop=1;C(step(f,r,in,1)==ERROR_CANCELLED&&f->recipient.transport.pending&&mcancel==1U&&!f->recipient.transport.close_confirmed);retained++;continue;}
  C(!advance(f,r,in,1,32U));C(f->recipient.transport.close_confirmed);
  if(i==19U){before=mcalls;C(scqt_start_io(&f->recipient,&f->runtime_owner,32U,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop)!=0&&mcalls==before);}
  before=mcalls;n=mcloses;C(!step(f,r,in,0)&&!step(f,r,in,1)&&mcalls==before&&mcloses==n); /* terminal status, not replay of IO */
  C(!qopens&&!qproviders); /* No KSP, and no readiness/SCM calls from continuation. */
  if(i==15U){C(scqs_close(&f->recipient,&f->runtime_owner,0)!=0);retained++;continue;}
  C(!scqs_close(&f->recipient,&f->runtime_owner,1));C(!scqr_close(r,&f->runtime_owner));C(!scqs_close(&f->controller,&f->administrator_owner,1));complete++;
 }
 printf("PASS SCQ_TRANSPORT_CONTINUATION cases=22 checks=%u complete=%u retained=%u ONE_IO_PER_STEP NO_KSP NO_ENDPOINT MODELED_ORIGINALS_PEERS_IO TERMINAL_IS_LOCAL_NOT_READINESS\n",checks-start,complete,retained);return 0;
}
