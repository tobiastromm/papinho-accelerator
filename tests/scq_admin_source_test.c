/* Qualified existing receiver/dispatch + explicit owner-decision MODEL.
 * No operational registration, endpoint, console session or SCQ effects. */
#define _WIN32_WINNT 0x0600
#include <windows.h>
static ULONGLONG WINAPI admin_clock(void);
#define GetTickCount64 admin_clock
#define PAPACC_SCQ_DISPATCH_NO_MAIN 1
#include "personal_scq_dispatch_test.c"
static ULONGLONG WINAPI admin_clock(void){return receiver_now;}
#define GetTickCount64 receiver_clock
#include "../apps/server/personal_scq_admin_win32.inc"
#undef WaitForSingleObject
static unsigned console_case,console_position;
static const char *console_reply;
static BOOL WINAPI admin_console_write(HANDLE h,const VOID *bytes,DWORD n,LPDWORD written,LPVOID reserved)
{(void)h;(void)bytes;(void)reserved;if(console_case==3)return FALSE;*written=n;return TRUE;}
static BOOL WINAPI admin_console_read(HANDLE h,PINPUT_RECORD event,DWORD cap,LPDWORD count)
{unsigned char ch;(void)h;(void)cap;if(console_case==4)return FALSE;memset(event,0,sizeof(*event));event->EventType=KEY_EVENT;
 event->Event.KeyEvent.bKeyDown=TRUE;event->Event.KeyEvent.wRepeatCount=1;*count=1;
 if(console_case==1){event->Event.KeyEvent.wVirtualKeyCode=VK_ESCAPE;return TRUE;}
 ch=console_case==6?'X':(unsigned char)console_reply[console_position++];
 if(!ch)event->Event.KeyEvent.wVirtualKeyCode=VK_RETURN;else event->Event.KeyEvent.uChar.AsciiChar=(CHAR)(console_case==2&&console_position==1?'X':ch);return TRUE;}
static DWORD WINAPI admin_console_wait(HANDLE h,DWORD ms){(void)h;(void)ms;return WAIT_OBJECT_0;}
static int admin_console_stop(void){return console_case==5;}
#define WriteConsoleA admin_console_write
#define ReadConsoleInputA admin_console_read
#define WaitForSingleObject admin_console_wait
#define papacc_server_console_win32_stop_requested admin_console_stop
#include "../apps/server/personal_confirmation_console_win32.inc"
#undef WriteConsoleA
#undef ReadConsoleInputA
#undef WaitForSingleObject
#undef papacc_server_console_win32_stop_requested
static DWORD WINAPI admin_foreign(LPVOID context)
{PAPACC_SCQ_ADMIN_SOURCE *c=context;volatile LONG stop=0;return scqa_take(c,c->controller,&stop);}
static void registration(PAPACC_SCQ_OWNER_DECISION *a,const PAPACC_SCQ_PLAN_DATA *p)
{memset(a,0,sizeof(*a));memcpy(a->valid_from,p->valid_from,sizeof(a->valid_from));memcpy(a->valid_until,p->valid_until,sizeof(a->valid_until));a->purpose=p->purpose;memcpy(a->originals,p->refs,sizeof(a->originals));a->controller_pid=p->administrator.pid;memcpy(a->controller_creation,p->administrator.creation,sizeof(FILETIME));memcpy(a->controller_token,p->administrator.token_id,sizeof(LUID));memcpy(a->controller_authentication,p->administrator.authentication,sizeof(LUID));memcpy(a->controller_modified,p->administrator.modified,sizeof(LUID));memcpy(a->request,p->request,32);memcpy(a->decision,p->decision,32);memcpy(a->attempt,p->attempt,32);memcpy(a->policy,p->policy,32);memcpy(a->target,p->target,32);memcpy(a->image,p->refs[2].digest,32);memcpy(a->key,p->key,112);memcpy(a->administrator,p->administrator.sid,68);memcpy(a->runtime,p->runtime.sid,68);a->administrator_bytes=p->administrator.sid_bytes;a->runtime_bytes=p->runtime.sid_bytes;}
#ifndef PAPACC_SCQ_ADMIN_ENTRY
#define PAPACC_SCQ_ADMIN_ENTRY main
#endif
int PAPACC_SCQ_ADMIN_ENTRY(void)
{
 static PAPACC_SCQ_ADMIN_SOURCE c,copy;static PAPACC_SCQ_OWNER_DECISION policy;static PAPACC_SCQ_PROFILE_DATA projection;
 static PAPACC_SCQ_PLAN_DATA p;static PAPACC_PERSONAL_SCQ_ORIGINAL view;PAPACC_PERSONAL_SCQ_ORIGINAL *borrow;static PAPACC_PERSONAL_RUNTIME_IDENTITY r;
 static PAPACC_PERSONAL_SCQ_DISPATCH d;static PAPACC_PERSONAL_SCQ_RECEIVER receiver;volatile LONG stop=0;
 int owner,other;DWORD n,result,e;unsigned i,j,scenario,start_checks;char reply[75];HANDLE thread_handle;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 C(receiver_cases()==0);C(LocalFree(virtual_sid)==NULL);C(LocalFree(sam_sid)==NULL);start_checks=checks;
 console_reply="OWNER_SCQ 0101010101010101010101010101010101010101010101010101010101010101";
 for(console_case=0;console_case<7;console_case++){console_position=0;e=(DWORD)personal_console_confirm((HANDLE)1,(HANDLE)2,console_reply);C(console_case?e==0:e==1);}
 printf("PASS SCQ_CONFIRMATION_CONSOLE cases=7 MODELED_CONSOLE_APIS SAME_F1_F6_HELPER NO_REAL_CONSOLE\n");
 {char data[256],short_reply[4];console_reply="P1 TRANSPORT_ONLY AMD64 DATA_NOT_APPROVAL";console_case=console_position=0;
  C(personal_console_line((HANDLE)1,(HANDLE)2,data,sizeof(data))&&!strcmp(data,console_reply));
  console_position=0;C(!personal_console_line((HANDLE)1,(HANDLE)2,short_reply,sizeof(short_reply)));
  C(!c.phase&&!c.value.controller_consumed&&!c.start_consumed&&!c.endpoint_consumed);
 }

 for(scenario=0;scenario<48;scenario++){
  i=scenario==47U?32U:(scenario>=32U?scenario+1U:scenario); /* retention stays last */
  C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
  reset(&r,0,2);receiver_fault=0;receiver_now=100;qfault=0;qstop=0;stop=0;
  C(!papacc_personal_runtime_capture(&r,(HANDLE)1,virtual_sid,2,1));C(scq_fixture_plan(&p));
  p.administrator.pid=88;p.administrator.creation[0]=99;p.administrator.token_id[0]=1;p.administrator.authentication[0]=0;p.administrator.modified[0]=3;
  memset(p.administrator.sid,0,68);p.administrator.sid_bytes=GetLengthSid(sam_sid);CopySid(68,p.administrator.sid,sam_sid);
  memset(p.runtime.sid,0,68);p.runtime.sid_bytes=GetLengthSid(virtual_sid);CopySid(68,p.runtime.sid,virtual_sid);
  p.runtime.pid=r.pid;p.runtime.creation[0]=r.creation.dwLowDateTime;p.runtime.token_id[0]=1;p.runtime.authentication[0]=0;p.runtime.modified[0]=3;
  for(j=0;j<3;j++){p.refs[j].volume=10;p.refs[j].low=41+j;p.refs[j].size_low=8;p.refs[j].attributes=0;}
  p.runtime.image[0]=10;p.runtime.image[2]=43;memcpy(p.request,p.refs[0].digest,32);memcpy(p.decision,p.refs[1].digest,32);
  if(i>=33U){BYTE sid[68];DWORD sid_bytes=p.runtime.sid_bytes;memcpy(sid,p.runtime.sid,68);memset(&p.runtime,0,sizeof(p.runtime));memcpy(p.runtime.sid,sid,68);p.runtime.sid_bytes=sid_bytes;C(scqp_unbound(&p.runtime));}
  memset(&projection,0,sizeof(projection));C(!papacc_scq_profile_prepare_data(&projection,&owner,1,&p,&p.administrator));registration(&policy,&p);policy.controller=&owner;policy.source=&c;
  memset(&view,0,sizeof(view));view.original=&view;view.controller=&owner;view.recipient=&owner;view.purpose=1;view.deadline=((ULONGLONG)p.valid_until[1]<<32)|p.valid_until[0];
  view.request=(HANDLE)41;view.decision=(HANDLE)42;view.image=(HANDLE)43;view.operator_process=(HANDLE)44;view.operator_pid=88;view.operator_creation.dwLowDateTime=99;view.operator_token=(HANDLE)45;
  receiver_file(view.request,&view.request_id);receiver_file(view.decision,&view.decision_id);receiver_file(view.image,&view.image_id);
  CopySid(68,view.operator_sid,sam_sid);view.operator_sid_bytes=GetLengthSid(sam_sid);receiver_token(view.operator_token,TokenStatistics,&view.operator_statistics,sizeof(view.operator_statistics),&n);
  memset(&c,0,sizeof(c));memset(&d,0,sizeof(d));memset(&receiver,0,sizeof(receiver));C(!papacc_personal_scq_prepare(&d,&r,p.attempt,fixture_name));
  memcpy(reply,"OWNER_SCQ ",10);for(j=0;j<32;j++){reply[10+2*j]="0123456789abcdef"[p.attempt[j]>>4];reply[11+2*j]="0123456789abcdef"[p.attempt[j]&15];}reply[74]=0;
  if(i==1)policy.decision[0]^=1;if(i==2)policy.purpose=2;if(i==3)view.controller=&other;if(i==4)view.operator_pid++;
  if(i==30)policy.controller_pid++;
  if(i==5)view.operator_creation.dwLowDateTime++;if(i==6)view.consumed=1;if(i==7)stop=1;
  e=scqa_capture(&c,&owner,i==0?NULL:&policy,&projection,&view,&stop);
  if(i<8||i==30){C(e!=0);C(!scqa_borrow(&c,&owner,&d,&r,&stop));goto end;}
  C(!e);{PAPACC_SCQ_ADMIN_SOURCE another;memset(&another,0,sizeof(another));
   C(scqa_capture(&another,&owner,&policy,&projection,&view,&stop)!=0);}
  copy=c;C(scqa_present(&copy,&owner,&stop)!=0);C(scqa_present(&c,&other,&stop)!=0);
  C(!scqa_present(&c,&owner,&stop));
  if(i>=24&&i<=29)receiver_fault=7+i-24;if(i==20)receiver_now=p.valid_until[0];
  if(i==8)reply[10]='f';if(i==9)receiver_fault=2;if(i==10)policy.target[0]^=1;
  e=scqa_confirm(&c,&owner,reply,74,&stop);
  if(i<=10||(i>=24&&i<=29)||i==20){C(e!=0&&!c.value.controller_consumed);goto end;}
  C(!e);C(scqa_confirm(&c,&owner,reply,74,&stop)!=0);
  if(i==11){stop=1;C(scqa_take(&c,&owner,&stop)!=0&&!c.value.controller_consumed);goto end;}
  if(i==12){use_native_thread=1;c.thread=GetCurrentThreadId();projection.thread=c.thread;
   thread_handle=CreateThread(NULL,0,admin_foreign,&c,0,NULL);C(thread_handle!=NULL);C(WaitForSingleObject(thread_handle,10000)==WAIT_OBJECT_0);
   C(GetExitCodeThread(thread_handle,&result)&&result==ERROR_INVALID_STATE&&!c.value.controller_consumed);C(native_thread_close(thread_handle));use_native_thread=0;c.thread=thread;projection.thread=thread;}
  if(i==31){c.value.retained=1;C(scqa_take(&c,&owner,&stop)!=0&&!c.value.controller_consumed);goto end;}
  if(i==23){receiver_fault=6;C(scqa_take(&c,&owner,&stop)!=0&&!c.value.controller_consumed);goto end;}
  if(i==21){C(scqa_take(&c,&other,&stop)!=0&&!c.value.controller_consumed);}
  C(!scqa_take(&c,&owner,&stop)&&c.value.controller_consumed);C(scqa_take(&c,&owner,&stop)!=0);
  if(i>=33U){
   C(scqa_startup_original(&c,&owner,&stop)==NULL); /* source consumed, profile not yet taken */
   C(!papacc_scq_profile_take_data(&projection,&owner,&p.administrator,NULL,p.refs,&stop));
   C(scqa_startup_original(&c,&owner,&stop)==&c.value);
   C(!c.value.runtime_pid&&!c.value.runtime_creation.dwLowDateTime&&!c.value.runtime_creation.dwHighDateTime);
   if(i==34U){copy=c;C(scqa_startup_original(&copy,&owner,&stop)==NULL);}
   if(i==35U)policy.purpose=2;if(i==36U)policy.target[0]^=1;if(i==37U)stop=1;
   if(i==38U)receiver_now=p.valid_until[0];if(i==39U)c.value.runtime_pid=123;
   if(i==40U)projection.used=1;if(i==41U)c.value.retained=1;if(i==42U)c.value.consumed=1;
   if(i==43U)projection.record[0]^=1;if(i==44U)c.thread++;
   if(i==45U)policy.source=&copy;
   if(i==46U)c.value.borrowed=1; /* conflicting live consumer is ineligible */
   if(i==47U){PAPACC_SCQ_ADMIN_SOURCE restart;memset(&restart,0,sizeof(restart));C(scqa_startup_original(&restart,&owner,&stop)==NULL);}
   if(i>=35U&&i<=46U)C(scqa_startup_original(&c,&owner,&stop)==NULL);
   C(scqa_startup_original(&c,&other,&stop)==NULL);
   goto end;
  }
  if(i==21){policy.decision[0]^=1;C(!scqa_borrow(&c,&owner,&d,&r,&stop)&&c.value.controller_consumed);goto end;}
  if(i==22){PAPACC_SCQ_ADMIN_SOURCE restarted;PAPACC_SCQ_PLAN_DATA restarted_plan=p;PAPACC_SCQ_PROFILE_DATA restarted_projection;
   memset(&restarted,0,sizeof(restarted));C(!scqa_borrow(&restarted,&owner,&d,&r,&stop));
   restarted_plan.administrator.creation[0]++;memset(&restarted_projection,0,sizeof(restarted_projection));
   C(!papacc_scq_profile_prepare_data(&restarted_projection,&owner,1,&restarted_plan,&restarted_plan.administrator));
   C(scqa_capture(&restarted,&owner,&policy,&restarted_projection,&view,&stop)!=0);goto end;}
  if(i==13)receiver_fault=2;if(i==14)receiver_now=p.valid_until[0];if(i==15)r.creation.dwLowDateTime++;if(i==16)r.pid++;if(i==17)d.association[0]^=1;
  borrow=scqa_borrow(&c,&owner,&d,&r,&stop);
  if(i>=13&&i<=17){C(!borrow);goto end;}
  C(borrow==&c.value&&borrow->controller_consumed&&!borrow->consumed);
  C(!personal_scq_receive_original(&receiver,borrow,&owner,&d,&r,&stop));
  if(i==18)stop=1;if(i==19)receiver_fault=2;
  e=personal_scq_receive_consume(&receiver,&owner,&stop);C(borrow->consumed);C((i==18||i==19)?e!=0:e==0);
  C(personal_scq_receive_consume(&receiver,&owner,&stop)!=0);
  if(i==32){C(personal_scq_receive_finish(&receiver,&owner,0)!=0);C(scqa_close(&c,&owner,0)!=0&&c.phase==6&&c.value.retained);C(scqa_close(&c,&owner,1)!=0);goto end;}
  C(!personal_scq_receive_finish(&receiver,&owner,1));C(!scqa_close(&c,&owner,1));C(scqa_borrow(&c,&owner,&d,&r,&stop)==NULL);
end:
  receiver_fault=fault=0;stop=0;if(i==15)r.creation.dwLowDateTime--;if(i==16)r.pid--;
  if(i==32){C(c.phase==6&&receiver.phase==4&&c.value.borrowed);/* Last case: static retained owner and SID backing survive to fixture exit. No cleanup claimed. */}
  else{C(!papacc_personal_runtime_close(&r));C(LocalFree(virtual_sid)==NULL);C(LocalFree(sam_sid)==NULL);}
 }
 printf("PASS SCQ_ADMIN_SOURCE cases=48 SSO_PRE_RUNTIME_CASES=15 checks=%u source_bytes=%zu policy_bytes=%zu original_bytes=%zu MODEL_OPERATIONAL_DECISION REAL_SID_THREAD NO_NATIVE_APPROVAL_NO_ENDPOINT\n",checks-start_checks,sizeof(c),sizeof(policy),sizeof(view));return 0;
}
