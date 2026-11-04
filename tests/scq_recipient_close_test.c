/* Native-origin and close implementations; ALL effects/processes are models.
 * No endpoint/SCM/credential operations are executed by this test. */
#define PAPACC_SCQ_ADAPTER_ENTRY preserved_adapter_main
#include "scq_native_adapter_test.c"
#include "../apps/server/personal_scq_recipient_win32.h"
static DWORD origin_fault;
static DWORD personal_scq_inputs_check(PAPACC_PERSONAL_SCQ_INPUTS *in,PSID sid,volatile LONG *stop,int capture)
{(void)sid;(void)capture;if(*stop)return ERROR_CANCELLED;return origin_fault||!in->content_verified||in->phase!=1U?ERROR_INVALID_DATA:0;}
static BOOL WINAPI role_duplicate(HANDLE a,HANDLE b,HANDLE d,LPHANDLE out,DWORD rights,BOOL inherit,DWORD options)
{(void)a;(void)d;if((b==(HANDLE)45||(receiver_admin_copy_model&&b==(HANDLE)545))&&rights==TOKEN_QUERY&&!inherit&&!options){if(mt_query_fault==21U)return FALSE;*out=b==(HANDLE)545?(HANDLE)645:(HANDLE)45;return TRUE;}if(rights!=(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE)||inherit||options||(b!=(HANDLE)44&&!(receiver_admin_copy_model&&b==(HANDLE)544)))return FALSE;*out=b==(HANDLE)544?(HANDLE)644:(HANDLE)44;if(mt_local_dup_hook)mt_local_dup_hook();return TRUE;}
static BOOL WINAPI role_open(HANDLE h,DWORD rights,PHANDLE out)
{mt_admin_reopens++;if(mt_query_fault==1U||h!=(HANDLE)44||rights!=TOKEN_QUERY)return FALSE;*out=(HANDLE)45;return TRUE;}
#undef DuplicateHandle
#undef OpenProcessToken
#define DuplicateHandle role_duplicate
#define OpenProcessToken role_open
#include "../apps/server/personal_scq_recipient_win32.inc"
static SESSION_FIXTURE rf[13],cf[10],bf[8];static PAPACC_SCQ_RECIPIENT_ORIGIN br[8];static PAPACC_PERSONAL_SCQ_INPUTS bi[8];static PAPACC_SCQ_RECIPIENT_ORIGIN origins[13];static PAPACC_PERSONAL_SCQ_INPUTS inputs[13];
static int ready_pair(SESSION_FIXTURE *f)
{
 C(!setup_session(f,0));C(!scqs_prepare(&f->controller,&f->administrator_owner,1,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
 C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
 C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));return 0;
}
#ifndef PAPACC_SCQ_RECIPIENT_ENTRY
#define PAPACC_SCQ_RECIPIENT_ENTRY main
#endif
int PAPACC_SCQ_RECIPIENT_ENTRY(void)
{
 unsigned i,j,start=checks,retained=0;SESSION_FIXTURE *f;PAPACC_SCQ_RECIPIENT_ORIGIN *r;PAPACC_PERSONAL_SCQ_INPUTS *in;DWORD e;HANDLE moved[3];PAPACC_PERSONAL_SCQ_ORIGINAL *proof;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<13;i++){
  f=&rf[i];r=&origins[i];in=&inputs[i];mcase=origin_fault=0;C(!ready_pair(f));in->phase=1;in->content_verified=1;
  in->reader[0]=(HANDLE)41;in->reader[1]=(HANDLE)42;in->reader[2]=(HANDLE)43;
  in->identity[0]=f->original.request_id;in->identity[1]=f->original.decision_id;in->identity[2]=f->original.image_id;
  for(j=0;j<3;j++){memcpy(in->digest[j],f->plan.refs[j].digest,32);memcpy(in->descriptor_digest[j],f->plan.refs[j].descriptor,32);}
  if(i==1)in->content_verified=0;if(i==2)origin_fault=1;if(i==3)f->runtime.pid++;
  if(i==11){f->plan.administrator.token_id[0]++;f->policy.controller_token[0]++;memset(&f->runtime_profile,0,sizeof(f->runtime_profile));C(!papacc_scq_profile_prepare_data(&f->runtime_profile,&f->runtime_owner,2,&f->plan,&f->plan.runtime));}
  if(i==12)in->digest[0][0]^=1;
  if(i==4)receiver_fault=9;if(i==5)f->stop=1;if(i==6)fault=11;if(i==7)thread++;
  e=scqr_acquire(r,&f->runtime_owner,i==8?NULL:&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,(HANDLE)44,&f->stop);
  if(i==7)thread--;receiver_fault=fault=0;
  if((i>=1&&i<=8)||i==11||i==12){C(e!=0&&!r->value.controller_consumed);if(r->phase==4)retained++;continue;}
  C(!e&&r->phase==1&&r->value.original==&r->value&&r->value.controller==r&&r->value.recipient==&f->runtime_owner);
  C(!r->value.controller_consumed&&!r->value.consumed&&!r->value.borrowed);
  C(scqr_acquire(r,&f->runtime_owner,&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,(HANDLE)44,&f->stop)!=0);
  C(!scqr_current(r,&f->runtime_owner,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,&f->stop));
  /* Query origin alone can NEVER enter the existing SCQ receiver. */
  C(personal_scq_receive_original(&f->runtime_receiver,&r->value,&f->runtime_owner,&f->runtime_dispatch,&f->runtime,&f->stop)!=0&&!r->value.consumed);
  if(i==9){mcase=13;C(scqr_close(r,&f->runtime_owner)!=0&&r->phase==4);retained++;continue;}
  if(i==10){f->stop=1;C(scqr_current(r,&f->runtime_owner,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,&f->stop)!=0);retained++;continue;}
  C(!scqr_close(r,&f->runtime_owner)&&r->phase==3&&!r->value.operator_token&&!r->value.operator_process);C(!scqr_close(r,&f->runtime_owner));
 }
 for(i=0;i<10;i++){
  f=&cf[i];mcase=mremote_closed=mcancel=0;receiver_identity_alias=1;C(!ready_pair(f));proof=&f->source.value;mpeer=f->plan.runtime.pid;
  C(!scqt_prepare(&f->controller,&f->administrator_owner,(HANDLE)101,(HANDLE)102,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop));
  C(!transfer(&f->controller,&f->administrator_owner,1,f,proof));C(!scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop));
  mpeer=f->plan.administrator.pid;C(!scqt_prepare(&f->recipient,&f->runtime_owner,(HANDLE)101,(HANDLE)102,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop));
  for(j=0;j<3;j++)moved[j]=(HANDLE)(ULONG_PTR)((i==9?741:541)+j);
  C(!scqt_accept_refs(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop));
  /* The model duplicates create distinct destination-table references. */
  if(i==1)moved[0]=proof->request;if(i==8)moved[0]=f->recipient.transport.files[0];
  e=scqt_adopt_delivery(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop);
  if(i==1||i==8){C(e!=0&&f->recipient.phase==6);retained++;continue;}C(!e&&!moved[0]&&!moved[1]&&!moved[2]);
  C(!transfer(&f->recipient,&f->runtime_owner,2,f,proof));C(!scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
  C(!transfer(&f->recipient,&f->runtime_owner,1,f,proof));mpeer=f->plan.runtime.pid;C(!transfer(&f->controller,&f->administrator_owner,2,f,proof));
  mpeer=f->plan.administrator.pid;
  if(i==2)mcase=13;if(i==3)f->stop=1;if(i==4)f->plan.runtime.token_id[0]++;
  e=scqt_produce_close(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop);
  if(i==2||i==3||i==4){C(e!=0&&f->recipient.phase==6);retained++;continue;}C(!e&&f->recipient.transport.close_produced==1);
  C(!memcmp(f->recipient.buffer,"SQC1",4));C(scqt_produce_close(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop)!=0);
  if(i==0)mcase=1; /* partial IO also covers the separate close stage */
  C(!transfer(&f->recipient,&f->runtime_owner,4,f,proof));C(f->recipient.transport.close_produced==3);
  mpeer=f->plan.runtime.pid;if(i==5)memcpy(wire,"SQR1",4);if(i==6)wire[30]^=1;if(i==7)mpeer++;
  e=(DWORD)transfer(&f->controller,&f->administrator_owner,8,f,proof);
  if(i==5||i==6||i==7||i==9){C(e!=0&&f->controller.phase==6&&!f->controller.transport.close_accepted);retained++;continue;}
  C(!e&&f->controller.transport.close_accepted&&!mremote_closed);
  C(transfer(&f->controller,&f->administrator_owner,8,f,proof)!=0);
  C(!scqs_close(&f->recipient,&f->runtime_owner,1));C(!scqs_close(&f->controller,&f->administrator_owner,1));
  C(!f->controller.transport.delivery_process&&!f->controller.transport.remote_files[0]&&!proof->borrowed);
 }
 for(i=0;i<8;i++){
  f=&bf[i];r=&br[i];in=&bi[i];mcase=origin_fault=receiver_identity_alias=0;C(!ready_pair(f));
  in->phase=1;in->content_verified=1;in->reader[0]=(HANDLE)41;in->reader[1]=(HANDLE)42;in->reader[2]=(HANDLE)43;
  in->identity[0]=f->original.request_id;in->identity[1]=f->original.decision_id;in->identity[2]=f->original.image_id;
  for(j=0;j<3;j++){memcpy(in->digest[j],f->plan.refs[j].digest,32);memcpy(in->descriptor_digest[j],f->plan.refs[j].descriptor,32);}
  C(!scqr_acquire(r,&f->runtime_owner,&f->policy,&f->runtime_profile,in,&f->runtime,&f->runtime_dispatch,(HANDLE)44,&f->stop));C(!r->value.controller_consumed);
  mpeer=f->plan.runtime.pid;C(!scqt_prepare(&f->controller,&f->administrator_owner,(HANDLE)101,(HANDLE)102,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
  C(!transfer(&f->controller,&f->administrator_owner,1,f,&f->source.value));mpeer=f->plan.administrator.pid;
  e=scqr_open(r,&f->recipient,&f->runtime_owner,in,i==4?L"invalid":L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.0123456789abcdef0123456789abcdef",&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop);
  if(i==4){C(e!=0&&r->phase==4&&!r->value.controller_consumed);retained++;continue;}
  C(!e&&f->recipient.transport.query_only&&!r->value.controller_consumed);
  C(scqt_start_io(&f->recipient,&f->runtime_owner,1,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop)==ERROR_INVALID_STATE);
  if(i==1)wire[0]^=1;mposition=0;
  C(!scqt_start_io(&f->recipient,&f->runtime_owner,2,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
  C(scqt_apply(&f->recipient,&f->runtime_owner,2,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop)==ERROR_INVALID_STATE&&!r->value.controller_consumed);
  if(i==2)mpeer++;if(i==3)in->content_verified=0;if(i==5)f->stop=1;
  if(i==6){PAPACC_SCQ_RECIPIENT_ORIGIN forwarded=*r;e=scqr_admit_offer(&forwarded,&f->recipient,&f->runtime_owner,in,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop);}
  else e=scqr_admit_offer(r,&f->recipient,&f->runtime_owner,in,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop);
  if(i==1||i==2||i==3||i==5||i==6){C(e!=0&&!r->value.controller_consumed);retained++;continue;}
  C(!e&&r->phase==2&&r->value.controller_consumed&&!f->recipient.transport.query_only);
  C(scqr_admit_offer(r,&f->recipient,&f->runtime_owner,in,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop)!=0);
  moved[0]=(HANDLE)41;moved[1]=(HANDLE)42;moved[2]=(HANDLE)43;
  C(!scqt_accept_refs(&f->recipient,&f->runtime_owner,moved,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
  C(!scqt_apply(&f->recipient,&f->runtime_owner,2,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&r->value,&f->stop));
  C(f->runtime_receiver.original==&r->value&&r->value.consumed&&r->value.borrowed&&f->runtime_receiver.original!=&f->source.value);
  C(!scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop));
  {const PAPACC_PERSONAL_SCQ_ORIGINAL *saved=f->runtime_receiver.original;
   f->runtime_receiver.original=&f->source.value;C(scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop)!=0);f->runtime_receiver.original=saved;
   f->recipient.transport.query_only=1;C(scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop)!=0);f->recipient.transport.query_only=0;
   f->recipient.transport.pending=1;C(scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop)!=0);f->recipient.transport.pending=0;
   f->recipient.peer_lost=1;C(scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop)!=0);f->recipient.peer_lost=0;
   r->value.consumed=0;C(scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop)!=0);r->value.consumed=1;
   C(scqr_closed_ready_current(r,&f->recipient,&f->administrator_owner,in,&f->stop)!=0);
   C(!scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop));}
  C(!scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
  C(scqr_closed_ready_current(r,&f->recipient,&f->runtime_owner,in,&f->stop)!=0); /* receipt DATA is not readiness */
  C(!scqs_close(&f->recipient,&f->runtime_owner,1));C(!scqr_close(r,&f->runtime_owner));
 }
 printf("PASS SCQ_RECIPIENT_CLOSE cases=31 checks=%u retained_models=%u origin_bytes=%zu transport_bytes=%zu QUERY_ORIGIN_IS_NOT_AUTHORITY MODELED_NATIVE_CLOSE NO_ENDPOINT\n",checks-start,retained,sizeof(PAPACC_SCQ_RECIPIENT_ORIGIN),sizeof(PAPACC_SCQ_TRANSPORT));return 0;
}

