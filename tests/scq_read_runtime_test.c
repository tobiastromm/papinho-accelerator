/* Source-linked READ/P receiver and same native I/O actors. OS/KSP modeled. */
#define _WIN32_WINNT 0x0600
#include <windows.h>
static LONGLONG read_qpc=100;
static BOOL WINAPI read_counter(LARGE_INTEGER *v){v->QuadPart=read_qpc;return TRUE;}
#define QueryPerformanceCounter read_counter
static unsigned read_handle_fault;
static BOOL WINAPI read_handles(HANDLE h,PDWORD n){(void)h;if(read_handle_fault==1U||read_handle_fault==2U){SetLastError(read_handle_fault==1U?0U:ERROR_INVALID_PARAMETER);return FALSE;}*n=read_handle_fault==3U?188U:100U;return TRUE;}
#define GetProcessHandleCount read_handles
#define PAPACC_SCQ_RECIPIENT_ENTRY preserved_read_close_model_main
#include "scq_recipient_close_test.c"
#include "../apps/server/personal_scq_read_diagnostic_win32.inc"
#include "../apps/server/personal_scq_read_runtime_win32.inc"
static SESSION_FIXTURE read_pair[21];static PAPACC_SCQ_RECIPIENT_ORIGIN read_origin[21];
static PAPACC_PERSONAL_SCQ_INPUTS read_inputs[21];static PAPACC_SCQ_READ_DIAGNOSTIC read_act[21];
static PAPACC_SCQ_READ_RECIPIENT read_recipient[21];static PAPACC_SCQ_START_ASSURANCE read_resources[21][2];
static PAPACC_PERSONAL_CNG_PROBE read_probe[21];
static void read_scope(PAPACC_SCQ_START_ASSURANCE *g,const void *owner,ULONGLONG until)
{
 memset(g,0,sizeof(*g));g->self=g;g->owner=owner;g->thread=thread;g->phase=5;g->p1_handles=g->read_diagnostic=g->initial_known=1;g->initial_handles=100;g->global_until=until;scqg_selected_p1(&g->limits);
 g->observations.last.frequency=1000;g->observations.last.begin=g->observations.last.end=100;g->observations.last.handles=100;
 g->observations.last.private_bytes=g->observations.last.working_set=4096;g->observations.last.physical_available=g->limits.physical_floor;g->observations.last.commit_available=g->limits.commit_floor;g->ready=g->observations.last;
}
static int prepare_read_pair(unsigned i,const BYTE public_id[32])
{
 SESSION_FIXTURE *f=&read_pair[i];PAPACC_SCQ_RECIPIENT_ORIGIN *r=&read_origin[i];PAPACC_PERSONAL_SCQ_INPUTS *in=&read_inputs[i];
 char reply[88];unsigned j;HANDLE moved[3];
 runtime_service_state=SERVICE_START_PENDING;mcase=mcalls=mcloses=mposition=mincomplete=mcancel=mremote_closed=0;origin_fault=receiver_identity_alias=0;read_qpc=100;
 C(!setup_session_machine_read(f,0));
 C(!scqk_prepare(&read_act[i],&f->administrator_owner,&f->source,public_id,&f->stop));
 memcpy(reply,"OWNER_SCQ_MACHINE_READ ",23);for(j=0;j<32;j++){reply[23+2*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[24+2*j]="0123456789abcdef"[f->plan.attempt[j]&15];}reply[87]=0;
 C(!scqk_confirm(&read_act[i],&f->administrator_owner,reply,87,&f->stop));
 C(!scqs_prepare(&f->controller,&f->administrator_owner,1,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
 C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
 in->phase=1;in->content_verified=1;in->reader[0]=(HANDLE)41;in->reader[1]=(HANDLE)42;in->reader[2]=(HANDLE)43;in->identity[0]=f->original.request_id;in->identity[1]=f->original.decision_id;in->identity[2]=f->original.image_id;
 for(j=0;j<3;j++){memcpy(in->digest[j],f->plan.refs[j].digest,32);memcpy(in->descriptor_digest[j],f->plan.refs[j].descriptor,32);}
 C(!scqr_acquire(r,&f->runtime_owner,&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,(HANDLE)44,&f->stop));
 receiver_identity_alias=1U; /* distinct modeled destination table before delivery */
 mpeer=f->plan.runtime.pid;C(!scqt_prepare(&f->controller,&f->administrator_owner,(HANDLE)101,(HANDLE)102,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
 C(!transfer(&f->controller,&f->administrator_owner,1,f,&f->source.value));C(!scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
 mpeer=f->plan.administrator.pid;
 C(!scqr_open(r,&f->recipient,&f->runtime_owner,in,L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.0123456789abcdef0123456789abcdef",&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
 mposition=0;C(!scqt_start_io(&f->recipient,&f->runtime_owner,2,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 C(!scqr_admit_offer(r,&f->recipient,&f->runtime_owner,in,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
 receiver_identity_alias=1;for(j=0;j<3;j++)moved[j]=(HANDLE)(ULONG_PTR)(541+j);
 C(!scqt_accept_refs(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 C(!scqt_adopt_delivery(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 C(!scqt_apply(&f->recipient,&f->runtime_owner,2,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
 /* Five-reference acquisition/QUERY mode is a modeled boundary here, not
  * a claim that the cold READ bootstrap producer is already integrated. */
 f->controller.transport.query_refs3=f->recipient.transport.query_refs3=1;
 read_scope(&read_resources[i][0],&f->administrator_owner,read_act[i].until);read_scope(&read_resources[i][1],&f->runtime_owner,read_act[i].until);
 f->controller.transport.resource_scope=&read_resources[i][0];f->recipient.transport.resource_scope=&read_resources[i][1];
 f->controller.transport.global_until=f->recipient.transport.global_until=read_act[i].until;
 C(!scqt_read_result_closed(&f->controller)&&!scqt_read_result_closed(&f->recipient));
 C(!scqkr_prepare(&read_recipient[i],&f->runtime_owner,r,&f->recipient,in,public_id,&f->stop));
 {DWORD ready;unsigned z;mpeer=f->plan.administrator.pid;mposition=mincomplete=0;
  for(z=0;z<1000U;z++){ready=scqkr_ready_io(&f->recipient,&f->runtime_owner,NULL,&read_recipient[i],&r->value,&f->stop);if(ready!=ERROR_MORE_DATA&&ready!=ERROR_IO_PENDING)break;}C(!ready);
  mpeer=f->plan.runtime.pid;mposition=mincomplete=0;
  for(z=0;z<1000U;z++){ready=scqkr_ready_io(&f->controller,&f->administrator_owner,&read_act[i],NULL,&f->source.value,&f->stop);if(ready!=ERROR_MORE_DATA&&ready!=ERROR_IO_PENDING)break;}C(!ready);
  C(!scqk_take(&read_act[i],&f->administrator_owner,&f->stop));
 }

 {PAPACC_SCQ_READ_RECIPIENT duplicate={0};C(scqkr_prepare(&duplicate,&f->runtime_owner,r,&f->recipient,in,public_id,&f->stop)!=0&&!duplicate.phase);}
 return 0;
}
static DWORD read_leg(unsigned i,int runtime,DWORD operation)
{
 SESSION_FIXTURE *f=&read_pair[i];PAPACC_SCQ_SESSION_DRIVER *c=runtime?&f->recipient:&f->controller;
 unsigned j;DWORD e;
 mpeer=runtime?f->plan.administrator.pid:f->plan.runtime.pid;mposition=mincomplete=0;
 for(j=0;j<1000;j++){
  e=scqkr_io(c,runtime?&f->runtime_owner:&f->administrator_owner,operation,runtime?&read_origin[i].value:&f->source.value,runtime?NULL:&read_act[i],runtime?&read_recipient[i]:NULL,&f->stop);
  if(!e||e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)return e;
 }
 return ERROR_TIMEOUT; /* harness bound only */
}
/* Only the two destination QUERY close identifiers are modeled here; cold
 * capture/duplication remains a distinct producer qualification obligation. */
static DWORD read_close_step(unsigned i,int runtime)
{
 SESSION_FIXTURE *f=&read_pair[i];unsigned n;DWORD e;
 mpeer=runtime?f->plan.administrator.pid:f->plan.runtime.pid;mposition=mincomplete=0;
 for(n=0;n<1000U;n++){
  e=scqr_continue_transport(runtime?&f->recipient:&f->controller,runtime?&f->runtime_owner:&f->administrator_owner,
   runtime?&read_origin[i]:NULL,runtime?&read_inputs[i]:NULL,runtime?&read_origin[i].value:&f->source.value,&f->stop);
  if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)return e;
  if(!(runtime?f->recipient.transport.operation:f->controller.transport.operation))return 0;
 }
 return ERROR_TIMEOUT;
}
int main(void)
{
 BYTE public_id[32];unsigned i,start=checks,before;DWORD e;PAPACC_PERSONAL_CNG_PROBE observed;SESSION_FIXTURE *f;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));memset(public_id,7,32);
 for(i=0;i<21;i++){
  C(!prepare_read_pair(i,public_id));f=&read_pair[i];mpeer=f->plan.runtime.pid;
  C(!scqkr_projection(&read_act[i],&f->controller,&f->administrator_owner,&f->stop));
  if(i==1)f->controller.buffer[112]^=1;if(i==2)f->stop=1;if(i==3)read_act[i].consumed=0;
  if(i==15)f->controller.transport.resource_scope=NULL;if(i==16)f->controller_profile.pre_readiness=0;
  if(i==17)f->controller.transport.query_refs3=0;if(i==18)read_resources[i][0].observations.last.handles=193;
  before=mcalls;e=read_leg(i,0,SCQK_P_SEND);
  if(i>=1&&i<=3||i>=15&&i<=18){C(e!=0&&mcalls==before&&!read_recipient[i].probe_used);continue;}C(!e);
  C(!scqkr_finish_send(&f->controller,&f->administrator_owner,SCQK_P_SEND,&f->source.value,&f->stop));
  if(i==4)wire[112]^=1;if(i==5)wire[16]^=1;if(i==6)wire[48]^=1;
  if(i==7)mcase=1;if(i==8)mcase=2;if(i==9)mcase=17;
  e=read_leg(i,1,SCQK_P_READ);if(i==9){C(e!=0&&!read_recipient[i].probe_used);continue;}C(!e);
  e=scqkr_accept(&read_recipient[i],&f->runtime_owner,&f->stop);
  if(i>=4&&i<=6){C(e!=0&&!read_recipient[i].probe_used);continue;}C(!e);C(scqkr_accept(&read_recipient[i],&f->runtime_owner,&f->stop)!=0);
  qfault=i==10?3:i==11?5:i==12?9:0;qopens=qproviders=0;qstop=0;
  if(i==13)f->stop=1;if(i==14)read_resources[i][1].observations.last.handles=193;
  if(i==0U){unsigned k;read_probe[i].read_scope=&read_recipient[i];read_recipient[i].probe=&read_probe[i];read_recipient[i].probe_used=1U;
   for(k=0;k<5U;k++){read_handle_fault=k<4U?k:0U;read_qpc=k==4U?201:100;
    e=pcp_read_resource(&read_probe[i],&f->runtime,&f->stop);
    C(e==(k==0U?0U:k==1U?ERROR_GEN_FAILURE:k==2U?ERROR_INVALID_PARAMETER:k==3U?ERROR_NOT_ENOUGH_QUOTA:ERROR_TIMEOUT));C(!qproviders&&!qopens);
   }
   read_handle_fault=0;read_qpc=100;read_recipient[i].probe=NULL;read_recipient[i].probe_used=0;read_probe[i].read_scope=NULL;
  }
  e=scqkr_probe(&read_recipient[i],&f->runtime_owner,&read_probe[i],&f->stop);
  if(i>=12&&i<=14){C(e!=0);if(i>=13)C(!qproviders&&!read_recipient[i].probe_used);continue;}C(!e&&qproviders==1&&qopens==1);
  C(scqkr_probe(&read_recipient[i],&f->runtime_owner,&read_probe[i],&f->stop)!=0&&qopens==1);
  C(!scqkr_result_ready(&read_recipient[i],&f->runtime_owner,&f->stop));C(!read_leg(i,1,SCQK_RESULT_SEND));
  mpeer=f->plan.administrator.pid;C(!scqkr_finish_send(&f->recipient,&f->runtime_owner,SCQK_RESULT_SEND,&read_origin[i].value,&f->stop));
  if(i==19)wire[64]^=1;if(i==20)wire[96]^=1;
  C(!read_leg(i,0,SCQK_RESULT_READ));e=scqkr_result_accept(&read_act[i],&f->controller,&f->administrator_owner,&observed,&f->stop);
  if(i==19||i==20){C(e!=0&&read_act[i].consumed&&!f->controller_profile.ack);continue;}C(!e);
  C(scqk_result(&observed,1)==(i==10?0U:i==11?2U:1U));C(!f->controller_profile.ack&&!f->runtime_profile.ack); /* DATA never close/receipt */
  if(i==0){
   C(scqt_read_result_closed(&f->controller)&&scqt_read_result_closed(&f->recipient));
   f->recipient.transport.global_until=0;C(!scqt_read_result_closed(&f->recipient));f->recipient.transport.global_until=read_act[i].until;
   f->controller.transport.global_until++;C(!scqt_read_result_closed(&f->controller));f->controller.transport.global_until--;
   f->recipient.read_recipient=NULL;C(!scqt_read_result_closed(&f->recipient));
   C(scqr_continue_transport(&f->recipient,&f->runtime_owner,&read_origin[i],&read_inputs[i],&read_origin[i].value,&f->stop)!=0);f->recipient.read_recipient=&read_recipient[i];
   read_act[i].consumed=0;C(!scqt_read_result_closed(&f->controller));
   C(scqr_continue_transport(&f->controller,&f->administrator_owner,NULL,NULL,&f->source.value,&f->stop)!=0);read_act[i].consumed=1;
   f->recipient.transport.closed_query[0]=544;f->recipient.transport.closed_query[1]=545;
   f->controller.transport.remote_query[0]=(HANDLE)544;f->controller.transport.remote_query[1]=(HANDLE)545;
   C(!read_close_step(i,1)); /* receipt production */
   C(!read_close_step(i,1)); /* receipt send */
   C(!read_close_step(i,0)); /* receipt read/ack */
   C(!read_close_step(i,1)); /* original delivered-file close */
   C(!read_close_step(i,1)); /* SQC3 send */
   C(!read_close_step(i,0)); /* SQC3 validate */
   C(!read_close_step(i,0)); /* original close return */
   C(!read_close_step(i,1)); /* original close confirmation */
   mpeer=f->plan.administrator.pid;C(!scqr_closed_ready_current(&read_origin[i],&f->recipient,&f->runtime_owner,&read_inputs[i],&f->stop));
   C(f->recipient.transport.close_confirmed&&f->controller.transport.close_accepted&&qopens==1);
   C(!scqs_close(&f->recipient,&f->runtime_owner,1));C(!scqr_close(&read_origin[i],&f->runtime_owner));
   mpeer=f->plan.runtime.pid;C(!scqs_close(&f->controller,&f->administrator_owner,1));
   C(!read_origin[i].value.borrowed&&!f->controller.transport.delivery_process);
  }

 }
 C(LocalFree(virtual_sid)==NULL);C(LocalFree(sam_sid)==NULL);
 printf("PASS READ_RUNTIME cases=21 checks=%u ACTUAL_P_ORIGIN_AND_IO_PROBE_BODY NATIVE_APIS_AND_QUERY_CAPTURE_MODELED NO_REAL_ENDPOINT_OR_KSP\n",checks-start);return 0;
}
