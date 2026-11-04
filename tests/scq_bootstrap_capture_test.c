/* Two independent logical process packets. Native peer/file/rights/IO APIs
 * are modeled, never an installed callback or authority in product code. */
#define PAPACC_SCQ_PRE_READINESS_ENTRY preserved_capture_chain_main
#include "scq_pre_readiness_chain_test.c"
#include <winternl.h>
static unsigned capture_fault;
static HANDLE WINAPI capture_process(DWORD rights,BOOL inherit,DWORD pid)
{if(rights==(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE)&&!inherit&&pid==88U){mt_admin_reopens++;return mt_query_fault==1U?NULL:(HANDLE)44;}return mt_process(rights,inherit,pid);}
static NTSTATUS NTAPI capture_access(HANDLE h,OBJECT_INFORMATION_CLASS kind,PVOID out,ULONG cap,PULONG needed)
{PUBLIC_OBJECT_BASIC_INFORMATION *p=out;
 if(kind==(OBJECT_INFORMATION_CLASS)2){UNICODE_STRING *u=out;const WCHAR *type=(h==(HANDLE)44||h==(HANDLE)544)?L"Process":L"Token";ULONG bytes;if((mt_query_fault==9U&&(h==(HANDLE)44||h==(HANDLE)544))||(mt_query_fault==10U&&(h==(HANDLE)45||h==(HANDLE)545)))type=L"Event";bytes=(ULONG)(wcslen(type)*sizeof(WCHAR));
  if(cap<sizeof(*u)+bytes)return (NTSTATUS)0xc000000dL;memset(out,0,cap);u->Buffer=(WCHAR *)((BYTE *)out+sizeof(*u));u->Length=(USHORT)bytes;u->MaximumLength=(USHORT)bytes;memcpy(u->Buffer,type,bytes);*needed=(ULONG)sizeof(*u)+bytes;return 0;}
 if(kind!=ObjectBasicInformation||cap<sizeof(*p))return (NTSTATUS)0xc000000dL;memset(p,0,sizeof(*p));p->GrantedAccess=((h==(HANDLE)44||h==(HANDLE)544)?(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE):(h==(HANDLE)45||h==(HANDLE)545)?TOKEN_QUERY:PAPACC_SCQ_FILE_READ_RIGHTS)|(capture_fault==1U?WRITE_DAC:0U);if((mt_query_fault==7U&&(h==(HANDLE)44||h==(HANDLE)544))||(mt_query_fault==8U&&(h==(HANDLE)45||h==(HANDLE)545)))p->GrantedAccess|=WRITE_DAC;*needed=sizeof(*p);return 0;}
static HMODULE WINAPI capture_module(LPCWSTR name){return !wcscmp(name,L"ntdll.dll")?(HMODULE)(ULONG_PTR)1:NULL;}
static FARPROC WINAPI capture_address(HMODULE module,LPCSTR name)
{FARPROC p=NULL;NTSTATUS (NTAPI *q)(HANDLE,OBJECT_INFORMATION_CLASS,PVOID,ULONG,PULONG)=capture_access;if(module&&(strcmp(name,"NtQueryObject")==0))memcpy(&p,&q,sizeof(p));return p;}
#undef OpenProcess
#define OpenProcess capture_process
#define GetModuleHandleW capture_module
#define GetProcAddress capture_address
#include "../apps/server/personal_scq_bootstrap_win32.inc"
typedef struct CAPTURE_FIXTURE {
 SESSION_FIXTURE f;PAPACC_PERSONAL_RUNTIME_IDENTITY controller_runtime;
 PAPACC_SCQ_OWNER_DECISION receiver_selection;PAPACC_SCQ_RECIPIENT_ORIGIN origin;
 PAPACC_PERSONAL_SCQ_INPUTS inputs;PS_SCQ_CARRIER carrier;
} CAPTURE_FIXTURE;
static CAPTURE_FIXTURE captures[20];
static int capture_input_model(CAPTURE_FIXTURE *b,const PAPACC_SCQ_PLAN_DATA *p)
{
 unsigned i;PAPACC_SCQ_TRANSPORT *t=&b->f.recipient.transport;
 b->inputs.phase=1U;
 for(i=0;i<3U;i++){
  C(t->capture_refs[i]);
  C(mt_duplicate(GetCurrentProcess(),t->capture_refs[i],GetCurrentProcess(),&b->inputs.file[i],PAPACC_SCQ_FILE_READ_RIGHTS,FALSE,0));
  C(mt_duplicate(GetCurrentProcess(),b->inputs.file[i],GetCurrentProcess(),&b->inputs.reader[i],PAPACC_SCQ_FILE_READ_RIGHTS,FALSE,0));
  C(receiver_file(b->inputs.file[i],&b->inputs.identity[i]));
  memcpy(b->inputs.digest[i],p->refs[i].digest,32);memcpy(b->inputs.descriptor_digest[i],p->refs[i].descriptor,32);
 }
 b->inputs.content_verified=1U; /* MODEL of separately qualified CNG/SD boundary */
 return 0;
}
#ifndef PAPACC_BOOTSTRAP_CAPTURE_ENTRY
#define PAPACC_BOOTSTRAP_CAPTURE_ENTRY main
#endif
int PAPACC_BOOTSTRAP_CAPTURE_ENTRY(void)
{
 static const char prefix[]="OWNER_SCQ_DIAGNOSTIC_START ";unsigned i,j,k,before=checks,completed=0;DWORD e;char reply[sizeof(prefix)+64U];
 CAPTURE_FIXTURE *b;SESSION_FIXTURE *f;PAPACC_SCQ_PLAN_DATA p;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<20U;i++){
  b=&captures[i];f=&b->f;mcase=mcreated=mopened=mevents=mposition=mincomplete=mcancel=origin_fault=capture_fault=0;receiver_identity_alias=1U;
  C(!setup_session_purpose(f,1,1U,PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
  f->source.value.recipient=&f->administrator_owner; /* sender-local view, never sent */
  f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
  C(!scqa_start_present(&f->source,&f->administrator_owner,&f->stop));
  memcpy(reply,prefix,sizeof(prefix)-1U);for(j=0;j<32U;j++){reply[sizeof(prefix)-1U+2U*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[sizeof(prefix)+2U*j]="0123456789abcdef"[f->plan.attempt[j]&15U];}reply[sizeof(reply)-1U]=0;
  C(!scqa_start_confirm(&f->source,&f->administrator_owner,reply,sizeof(reply)-1U,&f->stop));
  C(!scqa_start_carrier(&f->source,&f->administrator_owner,&b->carrier,&f->stop));
  /* Independent MODEL original selection, not reconstructed from carrier or
   * received bytes. No remote pointer is kept in the recipient's selection. */
  C(!scqp_decode_fields(f->controller_profile.record,sizeof(f->controller_profile.record),&p));
  registration(&b->receiver_selection,&p);b->receiver_selection.controller=NULL;b->receiver_selection.source=NULL;
  C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&b->controller_runtime));
  C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));
  C(!scqt_reserve(&f->controller,&f->administrator_owner,&f->stop));
  C(!f->runtime.phase&&!b->controller_runtime.phase&&!f->source.start_consumed);
  C(!scqa_start_take(&f->source,&f->administrator_owner,&f->stop));
  runtime_service_state=SERVICE_START_PENDING;C(!observed_runtime_identity_mode(f,1U));
  C(!papacc_personal_runtime_capture_closed(&b->controller_runtime,(HANDLE)1,virtual_sid,1U));
  {WCHAR empty[112]={0};C(!papacc_personal_scq_prepare_transport(&f->controller_dispatch,&b->controller_runtime,f->plan.attempt,empty));}
  C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
  C(!f->recipient.phase&&!f->runtime_profile.phase&&!f->runtime_dispatch.phase&&!b->inputs.phase&&!b->origin.phase);
  if(i==1U)b->carrier.attempt[0]^=1U;if(i==2U)b->carrier.endpoint[0]^=1U;if(i==3U)b->carrier.runtime_sid[8]^=1U;
  if(i==4U)f->stop=1;
  mpeer=f->plan.administrator.pid;if(i==5U)mpeer++;
  e=scqb_open(&f->recipient,&f->runtime_owner,i==19U?NULL:&b->carrier,i==6U?NULL:&b->receiver_selection,&f->runtime,&f->runtime_profile,&f->runtime_receiver,&f->runtime_dispatch,&f->stop);
  if((i>=1U&&i<=6U)||i==19U){C(e!=0&&!b->origin.phase&&!f->runtime_profile.phase);continue;}
  C(!e&&!f->recipient.phase&&!b->origin.phase&&!b->inputs.phase&&f->recipient.transport.phase==3U);
  C(scqb_open(&f->recipient,&f->runtime_owner,&b->carrier,&b->receiver_selection,&f->runtime,&f->runtime_profile,&f->runtime_receiver,&f->runtime_dispatch,&f->stop)!=0);
  mpeer=f->plan.runtime.pid;C(!scqt_connect(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
  C(!scqt_connected(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
  C(!scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop));
  if(i==7U)mcase=1U; /* partial bounded IO */
  if(i==8U)mcase=2U; /* pending and incomplete poll */
  for(k=0;k<PAPACC_SCQ_CLOSE_BYTES+1U;k++){e=scqb_write(&f->controller,&f->administrator_owner,&f->stop);if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)break;}
  C(!e&&(f->controller.transport.legs&1U));
  if(i==9U)wire[0]^=1U;
  if(i==10U)capture_fault=1U;
  if(i==11U){for(j=0;j<8U;j++)wire[PAPACC_SCQ_RECEIPT_BYTES+j]=(BYTE)((ULONGLONG)201U>>(8U*j));}
  mpeer=f->plan.administrator.pid;mposition=0;mincomplete=0;
  for(k=0;k<PAPACC_SCQ_CLOSE_BYTES+1U;k++){e=scqb_read(&f->recipient,&f->runtime_owner,&f->stop);if(e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)break;}
  C(!e&&!f->runtime_profile.phase&&!b->origin.phase);
  if(i==12U)receiver_fault=6U; /* wrong token user/identity at independent acquisition */
  e=scqb_candidates(&f->recipient,&f->runtime_owner,&p,&f->stop);
  if(i>=9U&&i<=12U){C(e!=0&&!b->origin.phase&&!f->runtime_profile.phase);continue;}
  C(!e&&f->recipient.transport.capture_valid==7U);
  C(!capture_input_model(b,&p));
  if(i==13U)b->inputs.digest[0][0]^=1U;
  e=scqr_first_prepare(&b->origin,&f->recipient,&f->runtime_owner,&b->receiver_selection,&f->runtime_profile,&b->inputs,&f->runtime,&f->runtime_dispatch,&f->runtime_receiver,&p,f->recipient.transport.capture_controller,&f->stop);
  if(i==12U||i==13U){C(e!=0);continue;}
  C(!e&&!b->origin.value.controller_consumed&&!b->origin.value.consumed);
  C(!scqb_promote(&f->recipient,&f->runtime_owner,&b->origin,&b->inputs,&f->stop));
  C(f->recipient.phase==3U&&b->origin.value.consumed&&!f->recipient.transport.capture_refs[0]);
  C(scqb_promote(&f->recipient,&f->runtime_owner,&b->origin,&b->inputs,&f->stop)!=0);
  C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);
  mcase=0;C(!scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
  C(scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);
  mpeer=f->plan.administrator.pid;C(!transfer(&f->recipient,&f->runtime_owner,1U,f,&b->origin.value));mpeer=f->plan.runtime.pid;
  if(i==18U)wire[0]^=1U;
  e=(DWORD)transfer(&f->controller,&f->administrator_owner,2U,f,&f->source.value);
  if(i==18U){C(e!=0&&scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop)!=0);continue;}
  C(!e);mpeer=f->plan.administrator.pid;
  C(!scqt_produce_close(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&b->origin.value,&f->stop));
  C(!transfer(&f->recipient,&f->runtime_owner,4U,f,&b->origin.value));mpeer=f->plan.runtime.pid;
  C(!transfer(&f->controller,&f->administrator_owner,8U,f,&f->source.value));
  C(!transfer(&f->controller,&f->administrator_owner,16U,f,&f->source.value));mpeer=f->plan.administrator.pid;
  C(!transfer(&f->recipient,&f->runtime_owner,32U,f,&b->origin.value));
  if(i==14U)f->stop=1U;if(i==15U)f->recipient.peer_lost=1U;if(i==16U)f->recipient.transport.close_confirmed=0U;
  e=scqr_closed_ready_current(&b->origin,&f->recipient,&f->runtime_owner,&b->inputs,&f->stop);
  if(i>=14U&&i<=16U){C(e!=0);continue;}
  C(!e);completed++;
  if(i==17U){mcase=13U;C(scqs_close(&f->recipient,&f->runtime_owner,1U)!=0&&b->origin.value.retained);continue;}
  C(!scqs_close(&f->recipient,&f->runtime_owner,1U));C(!scqr_close(&b->origin,&f->runtime_owner));
  C(!scqs_close(&f->controller,&f->administrator_owner,1U));
 }
 C(completed==4U);
 printf("PASS SCQ_BOOTSTRAP_CAPTURE cases=20 completed_before_fault=%u checks=%u DISTINCT_LOCAL_SELECTIONS_AND_RUNTIME_VIEWS NO_REMOTE_POINTERS CANDIDATES_NOT_AUTHORITY CNG_SD_IMAGE_BOUNDARY_MODELED NO_REAL_ENDPOINT_OR_RNG\n",completed,checks-before);return 0;
}
