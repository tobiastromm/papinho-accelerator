/* ALL endpoint, IO, duplication and close APIs below are models. No endpoint
 * is ever created, connected or opened. SID helpers are real Windows APIs. */
static unsigned mt_query_fault,mt_admin_reopens;static void (*mt_local_dup_hook)(void);
#define PAPACC_SCQ_TRANSPORT_IMPLEMENTATION 1
#define PAPACC_SCQ_SESSION_ENTRY preserved_session_main
#include "scq_session_driver_test.c"
static DWORD mcase,mpeer,mcalls,mduplicates,mcloses,mposition,mincomplete,mcancel,mrights,mremote_closed;
static BYTE wire[PAPACC_SCQ_BOOTSTRAP_READ_BYTES];static BYTE *pending_buffer;static DWORD pending_bytes,pending_write;
static BOOL WINAPI mt_duplicate(HANDLE source,HANDLE h,HANDLE target,LPHANDLE out,DWORD rights,BOOL inherit,DWORD options)
{
 (void)source;(void)target;mduplicates++;
 if(target==(HANDLE)301&&((mt_query_fault==2U&&h==(HANDLE)44)||(mt_query_fault==3U&&h==(HANDLE)45)||
  (mt_query_fault>=4U&&mt_query_fault<=6U&&h==(HANDLE)(ULONG_PTR)(41U+mt_query_fault-4U)))){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
mrights=rights;if(inherit||options){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(mcase==8||(mcase==38&&h==(HANDLE)42&&target==(HANDLE)301)){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}
 if(h==(HANDLE)101){if(rights!=(FILE_READ_DATA|FILE_WRITE_DATA|SYNCHRONIZE))return FALSE;*out=(HANDLE)201;}
 else if(h==(HANDLE)102){if(rights!=(EVENT_MODIFY_STATE|SYNCHRONIZE))return FALSE;*out=(HANDLE)202;}
 else if(h==(HANDLE)44&&rights==(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE))*out=receiver_admin_copy_model?(HANDLE)544:(HANDLE)44;
 else if(h==(HANDLE)45&&rights==TOKEN_QUERY)*out=receiver_admin_copy_model?(HANDLE)545:(HANDLE)45;
 else {if(rights!=PAPACC_SCQ_FILE_READ_RIGHTS)return FALSE;*out=mcase==23?(HANDLE)41:h;
 if(receiver_identity_alias&&mcase!=23){if(target==(HANDLE)301)*out=(HANDLE)((ULONG_PTR)h+500U);else if((ULONG_PTR)h>=500U)*out=(HANDLE)((ULONG_PTR)h+100U);}}
 return TRUE;
}
static DWORD WINAPI mt_type(HANDLE h){return h==(HANDLE)201?(mcase==29?FILE_TYPE_DISK:FILE_TYPE_PIPE):FILE_TYPE_DISK;}
static BOOL WINAPI mt_flags(HANDLE h,LPDWORD flags){(void)h;*flags=mcase==21?HANDLE_FLAG_INHERIT:0;return TRUE;}
static BOOL WINAPI mt_peer(HANDLE h,PULONG pid){(void)h;*pid=mpeer+(mcase==5?1U:0U);return TRUE;}
static BOOL WINAPI mt_reset(HANDLE h){(void)h;if(mcase==22){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}return TRUE;}
static BOOL WINAPI mt_close(HANDLE h){(void)h;mcloses++;if(mcase==13){SetLastError(ERROR_INVALID_HANDLE);return FALSE;}if(receiver_admin_copy_model){
  if(h==(HANDLE)544)receiver_admin_closed_mask|=1U;if(h==(HANDLE)545)receiver_admin_closed_mask|=2U;
  if(h==(HANDLE)644)receiver_admin_closed_mask|=4U;if(h==(HANDLE)645)receiver_admin_closed_mask|=8U;
 }return TRUE;}
static DWORD mt_transfer(BYTE *b,DWORD count,int write)
{
 DWORD n=(mcase==1&&count>17)?17:count;
 if(mcase==15)return 0;if(mcase==16)return count+1;
 if(write)memcpy(wire+mposition,b,n);else memcpy(b,wire+mposition,n);mposition+=n;return n;
}
static BOOL mt_io(BYTE *b,DWORD count,LPDWORD n,LPOVERLAPPED ov,int write)
{
 (void)ov;mcalls++;
 if(mcase==17||(mcase==18&&!write)){SetLastError(ERROR_BROKEN_PIPE);return FALSE;}
 if(mcase==2||mcase==12||mcase==32||mcase==34||mcase==35||mcase==40){pending_buffer=b;pending_bytes=count;pending_write=(DWORD)write;SetLastError(ERROR_IO_PENDING);return FALSE;}
 *n=mt_transfer(b,count,write);return TRUE;
}
static BOOL WINAPI mt_write(HANDLE h,LPCVOID b,DWORD count,LPDWORD n,LPOVERLAPPED ov){(void)h;return mt_io((BYTE *)b,count,n,ov,1);}
static BOOL WINAPI mt_read(HANDLE h,LPVOID b,DWORD count,LPDWORD n,LPOVERLAPPED ov){(void)h;return mt_io(b,count,n,ov,0);}
static BOOL WINAPI mt_result(HANDLE h,LPOVERLAPPED ov,LPDWORD n,BOOL wait)
{
 (void)h;(void)ov;if(wait)return FALSE;
 if(mcancel){SetLastError(ERROR_OPERATION_ABORTED);return FALSE;}
 if(mcase==34){SetLastError(ERROR_BROKEN_PIPE);return FALSE;}
 if((mcase==2||mcase==11)&&!mincomplete++){SetLastError(ERROR_IO_INCOMPLETE);return FALSE;}
 if(mcase==12||mcase==35){SetLastError(ERROR_IO_INCOMPLETE);return FALSE;}
 *n=mt_transfer(pending_buffer,pending_bytes,(int)pending_write);return TRUE;
}
static HANDLE WINAPI mt_process(DWORD rights,BOOL inherit,DWORD pid)
{if(rights!=(PROCESS_DUP_HANDLE|PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE)||inherit||pid!=77)return NULL;return (HANDLE)301;}
static DWORD WINAPI mt_wait(HANDLE h,DWORD ms){if(h==(HANDLE)301)return mremote_closed?WAIT_OBJECT_0:WAIT_TIMEOUT;return receiver_wait(h,ms);}
static BOOL WINAPI mt_cancel(HANDLE h,LPOVERLAPPED ov){(void)h;(void)ov;mcancel++;if(mcase==32){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}
static DWORD mcreated,mevents,mconnected,mopened;static volatile LONG *mstop;
static HANDLE WINAPI mt_event(LPSECURITY_ATTRIBUTES sa,BOOL manual,BOOL initial,LPCWSTR name)
{if(sa||!manual||initial||name)return NULL;mevents++;if(mcase==108&&mstop)*mstop=1;if(mcase==107){SetLastError(ERROR_ACCESS_DENIED);return NULL;}return (HANDLE)202;}
static HANDLE WINAPI mt_create(LPCWSTR name,DWORD access,DWORD mode,DWORD instances,DWORD out,DWORD in,DWORD timeout,LPSECURITY_ATTRIBUTES sa)
{
 PACL acl=NULL;BOOL present,def;PSID owner=NULL;unsigned i;ACCESS_ALLOWED_ACE *ace;
 mcreated++;if(!name||access!=(PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED|FILE_FLAG_FIRST_PIPE_INSTANCE)||
 mode!=(PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT|PIPE_REJECT_REMOTE_CLIENTS)||instances!=1||out!=PAPACC_SCQ_RECEIPT_BYTES||in!=PAPACC_SCQ_RECEIPT_BYTES||timeout||!sa||sa->bInheritHandle||
 !IsValidSecurityDescriptor(sa->lpSecurityDescriptor)||!GetSecurityDescriptorOwner(sa->lpSecurityDescriptor,&owner,&def)||!EqualSid(owner,sam_sid)||
 !GetSecurityDescriptorDacl(sa->lpSecurityDescriptor,&present,&acl,&def)||!present||!acl||acl->AceCount!=3)return INVALID_HANDLE_VALUE;
 for(i=0;i<3;i++)if(!GetAce(acl,i,(void **)&ace)||ace->Mask!=0x00120183UL||ace->Header.AceFlags)return INVALID_HANDLE_VALUE;
 if(mcase==100){SetLastError(ERROR_ACCESS_DENIED);return INVALID_HANDLE_VALUE;}return (HANDLE)201;
}
static BOOL WINAPI mt_connect(HANDLE pipe,LPOVERLAPPED ov)
{(void)pipe;(void)ov;mconnected++;if(mcase==103||mcase==110){SetLastError(ERROR_IO_PENDING);return FALSE;}
 if(mcase==106){SetLastError(ERROR_ACCESS_DENIED);return FALSE;}return TRUE;}
static HANDLE WINAPI mt_open(LPCWSTR name,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES sa,DWORD disposition,DWORD flags,HANDLE template_file)
{mopened++;if(!name||access!=0x00120183UL||share||sa||disposition!=OPEN_EXISTING||flags!=(FILE_FLAG_OVERLAPPED|SECURITY_SQOS_PRESENT|SECURITY_ANONYMOUS)||template_file)return INVALID_HANDLE_VALUE;
 if(mcase==109){SetLastError(ERROR_ACCESS_DENIED);return INVALID_HANDLE_VALUE;}return (HANDLE)201;}
#define CreateEventW mt_event
#define CreateNamedPipeW mt_create
#define ConnectNamedPipe mt_connect
#define CreateFileW mt_open
#undef DuplicateHandle
#undef CloseHandle
#define DuplicateHandle mt_duplicate
#define CloseHandle mt_close
#define GetFileType mt_type
#define GetFileInformationByHandle receiver_file
#define WaitForSingleObject mt_wait
#undef OpenProcess
#define OpenProcess mt_process
#define GetHandleInformation mt_flags
#define GetNamedPipeClientProcessId mt_peer
#define GetNamedPipeServerProcessId mt_peer
#define ResetEvent mt_reset
#define WriteFile mt_write
#define ReadFile mt_read
#define GetOverlappedResult mt_result
#define CancelIoEx mt_cancel
#include <bcrypt.h>
/* Scope SSO-B excludes real CNG, including CSPRNG calls. This deterministic
 * fixture is never used by product code or as evidence of random strength. */
static NTSTATUS WINAPI mt_random(BCRYPT_ALG_HANDLE algorithm,PUCHAR bytes,ULONG count,ULONG flags)
{
 ULONG i;if(algorithm||!bytes||count!=16U||flags!=BCRYPT_USE_SYSTEM_PREFERRED_RNG)return (NTSTATUS)0xC000000DL;
 for(i=0;i<count;i++)bytes[i]=(BYTE)(i+1U);return 0;
}
#define BCryptGenRandom mt_random
#include "../apps/server/personal_scq_transport_win32.inc"
#undef BCryptGenRandom
static SESSION_FIXTURE af[41],gf[12];
static DWORD WINAPI adapter_foreign(LPVOID value)
{SESSION_FIXTURE *f=value;return scqt_start_io(&f->controller,&f->administrator_owner,1,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->source.value,&f->stop);}

static int transfer(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,DWORD op,SESSION_FIXTURE *f,PAPACC_PERSONAL_SCQ_ORIGINAL *proof)
{
 DWORD e;unsigned limit=0;const PAPACC_SCQ_PEER_DATA *a=c->role==1?&f->plan.administrator:&f->plan.runtime,*b=c->role==1?&f->plan.runtime:&f->plan.administrator;
 mposition=mincomplete=0;
 do {e=scqt_start_io(c,owner,op,a,b,f->plan.refs,proof,&f->stop);
 while(e==ERROR_IO_PENDING&&limit++<100)e=scqt_poll(c,owner);
 }while(e==ERROR_MORE_DATA&&limit++<100);
 if(e)return (int)e;if(op==4U||op==8U||op==16U||op==32U)return (int)scqt_apply_close(c,owner,op,a,b,f->plan.refs,proof,&f->stop);return (int)scqt_apply(c,owner,op,a,b,f->plan.refs,proof,&f->stop);
}
#ifndef PAPACC_SCQ_ADAPTER_ENTRY
#define PAPACC_SCQ_ADAPTER_ENTRY main
#endif
int PAPACC_SCQ_ADAPTER_ENTRY(void)
{
 SESSION_FIXTURE *f;PAPACC_PERSONAL_SCQ_ORIGINAL *proof,forwarded;PAPACC_SCQ_REF_DATA refs[3];HANDLE originals[3]={(HANDLE)41,(HANDLE)42,(HANDLE)43};
 DWORD e,before,result,retained=0;HANDLE worker;unsigned i,start=checks;
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<41;i++){
  f=&af[i];mcase=mcalls=mduplicates=mcloses=mposition=mincomplete=mcancel=mremote_closed=0;C(!setup_session(f,0));
  C(!scqs_prepare(&f->controller,&f->administrator_owner,1,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
  C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
  C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));proof=&f->source.value;
  if(i==3)proof=NULL;if(i==4)proof->controller_consumed=0;
  if(i==24){forwarded=*proof;proof=&forwarded;}
  mcase=i;mpeer=f->plan.runtime.pid;before=mduplicates;
  if(i==19)thread++;
  e=scqt_prepare(&f->controller,&f->administrator_owner,(HANDLE)101,(HANDLE)102,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop);
  if(i==19)thread--;
  if(i==3||i==4||i==19||i==24){C(e!=0&&mduplicates==before);goto done;}
  if(i==5||i==8||i==21||i==29){C(e!=0&&f->controller.transport.phase==6);goto done;}
  C(!e&&f->controller.transport.pipe&&f->controller.transport.event);
  if(i==20){PAPACC_SCQ_SESSION_DRIVER copy=f->controller;C(scqt_start_io(&copy,&f->administrator_owner,1,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)==ERROR_INVALID_STATE);goto done;}
  if(i==37){
   use_native_thread=1;f->controller.thread=f->controller.transport.thread=GetCurrentThreadId();before=mcalls;
   worker=CreateThread(NULL,0,adapter_foreign,f,0,NULL);C(worker!=NULL);C(native_wait(worker,10000)==WAIT_OBJECT_0);
   C(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE&&mcalls==before);C(native_thread_close(worker));
   use_native_thread=0;f->controller.thread=f->controller.transport.thread=thread;goto done;
  }
  if(i==39){f->recipient.owner=&f->administrator_owner;C(scqt_prepare(&f->recipient,&f->runtime_owner,(HANDLE)101,(HANDLE)102,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop)!=0);f->recipient.owner=&f->runtime_owner;goto done;}
  if(i==9)f->stop=1;if(i==10)f->plan.runtime.token_id[0]++;if(i==30)f->runtime.expected_sid_type^=1;
  if(i==12||i==32||i==34||i==35||i==40){
   e=scqt_start_io(&f->controller,&f->administrator_owner,1,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop);C(e==ERROR_IO_PENDING);
   C(scqs_ack(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,wire,PAPACC_SCQ_RECEIPT_BYTES,&f->stop)==ERROR_INVALID_STATE&&f->controller.phase==3);
   before=mcalls;C(scqt_start_io(&f->controller,&f->administrator_owner,1,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)==ERROR_INVALID_STATE&&mcalls==before);
   if(i==34){C(scqt_poll(&f->controller,&f->administrator_owner)!=0&&!f->controller.transport.pending);goto done;}
   if(i==40){C(scqs_peer_lost(&f->controller,&f->administrator_owner)!=0&&mcancel==1);}
   else if(i==35){C(scqs_close(&f->controller,&f->administrator_owner,1)!=0);}
   else C(scqt_stop(&f->controller,&f->administrator_owner)!=0);
   C(f->controller.transport.pending&&f->controller.transport.pipe&&f->controller.phase==6);C(scqt_poll(&f->controller,&f->administrator_owner)!=0&&!f->controller.transport.pending);C(scqt_close(&f->controller,&f->administrator_owner)!=0);goto done;
  }
  e=(DWORD)transfer(&f->controller,&f->administrator_owner,1,f,proof);
  if(i==9||i==10||i==15||i==16||i==17||i==22||i==30){C(e!=0&&f->controller.phase==6);goto done;}
  C(!e);
  if(i==11){C(scqt_poll(&f->controller,&f->administrator_owner)==ERROR_INVALID_STATE);goto done;}
  if(i==33){before=mcalls;C(scqt_start_io(&f->controller,&f->runtime_owner,2,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)!=0&&mcalls==before);goto done;}
  e=scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop);
  if(i==38){C(e!=0&&f->controller.transport.remote_files[0]&&!f->controller.transport.remote_files[1]&&f->controller.transport.delivery_process&&proof->retained);goto done;}C(!e);
  C(f->controller.transport.delivery_process==(HANDLE)301&&mrights==PAPACC_SCQ_FILE_READ_RIGHTS);
  C(scqt_deliver_refs(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)!=0);
  if(i==26){before=mcalls;C(scqt_start_io(&f->controller,&f->administrator_owner,1,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)!=0&&mcalls==before);goto done;}
  if(i==14){C(scqs_peer_lost(&f->controller,&f->administrator_owner)!=0);C(scqt_start_io(&f->controller,&f->administrator_owner,2,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)!=0);goto done;}
  mpeer=f->plan.administrator.pid;
  C(!scqt_prepare(&f->recipient,&f->runtime_owner,(HANDLE)101,(HANDLE)102,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop));
  memcpy(refs,f->plan.refs,sizeof(refs));if(i==6)refs[0].rights|=WRITE_DAC;if(i==7)refs[0].low++;if(i==28)originals[0]=NULL;
  if(i==31)proof->recipient=&f->administrator_owner;
  e=scqt_accept_refs(&f->recipient,&f->runtime_owner,originals,&f->plan.runtime,&f->plan.administrator,refs,proof,&f->stop);originals[0]=(HANDLE)41;
  if(i==6||i==7||i==23||i==28||i==31){C(e!=0&&f->recipient.phase==6);goto done;}C(!e&&mrights==PAPACC_SCQ_FILE_READ_RIGHTS);
  C(scqt_accept_refs(&f->recipient,&f->runtime_owner,originals,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop)!=0);
  e=(DWORD)transfer(&f->recipient,&f->runtime_owner,2,f,proof);if(i==18||i==31){C(e!=0);goto done;}C(!e);C(f->runtime_receiver.phase==2&&proof->consumed);
  if(i==25){C(transfer(&f->recipient,&f->runtime_owner,2,f,proof)!=0);goto done;}
  C(!scqs_receipt(&f->recipient,&f->runtime_owner,&f->plan.runtime,&f->plan.administrator,f->plan.refs,&f->stop));
  C(!transfer(&f->recipient,&f->runtime_owner,1,f,proof));mpeer=f->plan.runtime.pid;
  if(i==27)wire[0]^=1;
  e=(DWORD)transfer(&f->controller,&f->administrator_owner,2,f,proof);
  if(i==27){C(e!=0);goto done;}C(!e&&f->controller.phase==4);
  if(i==13){C(scqs_close(&f->recipient,&f->runtime_owner,1)!=0&&f->recipient.transport.phase==6);goto done;}
  C(!scqs_close(&f->recipient,&f->runtime_owner,1));
  if(i==36){C(scqs_close(&f->controller,&f->administrator_owner,1)!=0&&f->controller.transport.delivery_process&&f->controller.transport.remote_files[0]);goto done;}mremote_closed=1;C(!scqs_close(&f->controller,&f->administrator_owner,1));
  C(!f->recipient.transport.pipe&&!f->controller.transport.pipe&&!f->source.value.borrowed);C(!scqt_close(&f->controller,&f->administrator_owner));
 done:
  if(f->recipient.phase!=6&&f->recipient.phase!=5)(void)scqs_close(&f->recipient,&f->runtime_owner,1);
  if(f->controller.phase!=6&&f->controller.phase!=5){mremote_closed=1;(void)scqs_close(&f->controller,&f->administrator_owner,1);}
  if(f->controller.phase==6||f->recipient.phase==6){retained++;C(f->source.value.controller_consumed||i==4);}
  /* Static fixture storage and SID backing remain alive for UNKNOWN cases. */
 }
 for(i=0;i<12;i++){
  f=&gf[i];mcase=mcreated=mevents=mconnected=mopened=mcancel=mremote_closed=0;C(!setup_session(f,0));
  C(!scqs_prepare(&f->controller,&f->administrator_owner,1,&f->controller_profile,&f->source,&f->controller_receiver,&f->controller_dispatch,&f->runtime));
  C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));
  C(!scqs_offer(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,&f->stop));proof=&f->source.value;mcase=100+i;mstop=&f->stop;
  if(i==11){proof->controller_consumed=0;C(scqt_create(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)!=0&&!mcreated&&!mevents);continue;}
  e=scqt_create(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop);
  if(i==0||i==7||i==8){C(e!=0&&f->controller.phase==6&&mevents==1);if(i==7||i==8)C(!mcreated);continue;}
  C(!e&&mcreated==1&&mevents==1&&f->controller.transport.phase==2);
  C(scqt_create(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)!=0&&mcreated==1);
  e=scqt_connect(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop);
  if(i==6){C(e!=0&&f->controller.phase==6);continue;}
  if(i==3||i==10){C(e==ERROR_IO_PENDING&&f->controller.transport.pending);
   if(i==10){C(scqs_peer_lost(&f->controller,&f->administrator_owner)!=0&&mcancel==1);C(scqt_poll(&f->controller,&f->administrator_owner)!=0&&!f->controller.transport.pending);continue;}
   C(!scqt_poll(&f->controller,&f->administrator_owner));
  }else C(!e);
  C(scqt_connect(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop)!=0&&mconnected==1);
  mpeer=f->plan.runtime.pid;if(i==5)mpeer++;
  e=scqt_connected(&f->controller,&f->administrator_owner,&f->plan.administrator,&f->plan.runtime,f->plan.refs,proof,&f->stop);
  if(i==5){C(e!=0&&f->controller.phase==6);continue;}C(!e);
  mpeer=f->plan.administrator.pid;
  if(i==4){C(scqt_open_local(&f->recipient,&f->runtime_owner,L"//remote/pipe/x",&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop)==ERROR_INVALID_NAME&&!mopened);}
  else{
   e=scqt_open_local(&f->recipient,&f->runtime_owner,f->controller.transport.name,&f->plan.runtime,&f->plan.administrator,f->plan.refs,proof,&f->stop);
   if(i==9){C(e!=0&&f->recipient.phase==6&&mopened==1);continue;}C(!e&&mopened==1);
  }
  C(!scqt_close(&f->recipient,&f->runtime_owner));C(!scqt_close(&f->controller,&f->administrator_owner));
 }
 retained=0;
 for(i=0;i<41;i++)if(af[i].controller.phase==6||af[i].recipient.phase==6)retained++;
 for(i=0;i<12;i++){
  f=&gf[i];if(f->recipient.phase!=6&&f->recipient.phase!=5)(void)scqs_close(&f->recipient,&f->runtime_owner,1);
  if(f->controller.phase!=6&&f->controller.phase!=5)(void)scqs_close(&f->controller,&f->administrator_owner,1);
  if(f->controller.phase==6||f->recipient.phase==6)retained++;
 }
 printf("PASS SCQ_NATIVE_ADAPTER cases=53 checks=%u retained_models=%lu transport_bytes=%zu driver_bytes=%zu MODELED_ENDPOINT_APIS REAL_SID_SD_HELPERS MODEL_RNG NO_REAL_CNG NO_ENDPOINT NO_NATIVE_BILATERAL_AUTH\n",checks-start,(unsigned long)retained,sizeof(PAPACC_SCQ_TRANSPORT),sizeof(PAPACC_SCQ_SESSION_DRIVER));return 0;
}
