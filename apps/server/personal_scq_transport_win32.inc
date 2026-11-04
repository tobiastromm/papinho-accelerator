#include "personal_scq_transport_win32.h"
#include <bcrypt.h>
#include "personal_scq_start_assurance_win32.inc"
/* Native owner attaches its actual observed G3 scope. Component-only legacy
 * models/profiles do not gain capacity by this hook; native entry gates still
 * require the scope. Check AGE at the effect, not only query duration. */
static DWORD scqt_resource_effect(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop,int creating)
{
 PAPACC_SCQ_START_ASSURANCE *g=c->transport.resource_scope;LARGE_INTEGER now;DWORD count,e;
 if(!g)return 0;
 if(g->self!=g||g->owner!=owner||g->thread!=GetCurrentThreadId()||!stop||*stop||!g->global_until||GetTickCount64()>=g->global_until)return ERROR_ACCESS_DENIED;
 if(g->source){if(!scqg_scope(g,owner,stop)||g->phase!=2U)return ERROR_ACCESS_DENIED;}
 else if(g->phase!=5U)return ERROR_ACCESS_DENIED;
 if(!QueryPerformanceCounter(&now)||now.QuadPart<0)return ERROR_INVALID_DATA;
 e=scqg_sample(g,&g->observations.last,(ULONGLONG)now.QuadPart,creating);if(e)return e;
 if(creating&&(!GetProcessHandleCount(GetCurrentProcess(),&count)||count>=PAPACC_SCQ_HANDLE_GUARD(g->p1_handles)))return ERROR_NOT_ENOUGH_QUOTA;
 return 0;
}
/* Destination IDs occupy the peer table. Measure through its already-held
 * original process, never by opening another PID reference. No authority is
 * inferred from this count. The existing peer checks bracket delivery. */
static DWORD scqt_destination_effect(PAPACC_SCQ_SESSION_DRIVER *c,HANDLE process,volatile LONG *stop)
{
 PAPACC_SCQ_START_ASSURANCE *g=c->transport.resource_scope;DWORD count;LARGE_INTEGER begin,end,frequency;
 if(!g||!g->p1_handles)return 0; /* old profiles keep their existing contract */
 if(g->self!=g||g->owner!=c->owner||g->thread!=GetCurrentThreadId()||
  (g->phase!=2U&&g->phase!=5U)||!process||!stop||*stop||g->limits.max_handles!=PAPACC_SCQ_P1_CAP)return ERROR_ACCESS_DENIED;
 if(!QueryPerformanceFrequency(&frequency)||frequency.QuadPart<=0||
  !QueryPerformanceCounter(&begin)||begin.QuadPart<0||!GetProcessHandleCount(process,&count)||
  !QueryPerformanceCounter(&end)||end.QuadPart<begin.QuadPart)return ERROR_INVALID_DATA;
 if(!scqg_within((ULONGLONG)(end.QuadPart-begin.QuadPart),(ULONGLONG)frequency.QuadPart,g->limits.freshness_ms))return ERROR_TIMEOUT;
 if(count>=PAPACC_SCQ_P1_GUARD)return ERROR_NOT_ENOUGH_QUOTA;
 if(*stop||GetTickCount64()>=g->global_until)return ERROR_CANCELLED;
 return 0;
}
/* Win32 IO adapter; no endpoint creation/connection, no generic transport API.
 * All effectful APIs are replaced at compile time by the isolated test. */
static int scqt_owned(const PAPACC_SCQ_SESSION_DRIVER *c,const void *owner)
{return scqs_owned(c,owner)&&c->transport.self==&c->transport&&c->transport.owner==owner&&c->transport.thread==GetCurrentThreadId();}
static DWORD scqt_fail(PAPACC_SCQ_SESSION_DRIVER *c,DWORD e)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;if(!t->first)t->first=e?e:ERROR_INVALID_DATA;
 t->phase=6;if(t->original){InterlockedExchange(&t->original->retained,1);if(!t->query_only)InterlockedExchange(&t->original->controller_consumed,1);}
 (void)scqs_fail(c,t->first);c->phase=6;return t->first;
}
static int scqt_process(HANDLE h,const PAPACC_SCQ_PEER_DATA *p)
{
 FILETIME creation,exit,kernel,user;
 return h&&p&&GetProcessId(h)==p->pid&&WaitForSingleObject(h,0)==WAIT_TIMEOUT&&
 GetProcessTimes(h,&creation,&exit,&kernel,&user)&&!memcmp(&creation,p->creation,sizeof(creation));
}
static DWORD scqt_current(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 DWORD e,pid,flags;const PAPACC_SCQ_PEER_DATA *admin=c->role==1U?local:remote,*runtime=c->role==1U?remote:local;
 if(!scqt_owned(c,owner)||c->transport.phase!=1U||!proof||c->transport.original!=proof)return ERROR_INVALID_STATE;
 if(c->transport.global_until&&GetTickCount64()>=c->transport.global_until)return scqt_fail(c,ERROR_TIMEOUT);
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return scqt_fail(c,e);
 if(proof->original!=proof||(c->role==1U?proof->controller!=owner:proof->recipient!=owner)||(!proof->controller_consumed&&!(c->role==2U&&c->transport.query_only&&c->phase==1U))||proof->retained||proof->purpose!=c->dispatch->purpose||
 !personal_scq_original_refs(proof)||!scqa_operator(proof,admin)||
 !personal_scq_original_binding(proof,c->dispatch,c->runtime)||!scqs_runtime_snapshot(c->runtime,runtime)||
 !scqt_process(proof->operator_process,admin)||!scqt_process(c->runtime->process,runtime))return scqt_fail(c,ERROR_ACCESS_DENIED);
 if(!GetHandleInformation(c->transport.pipe,&flags)||(flags&HANDLE_FLAG_INHERIT)||
 !GetHandleInformation(c->transport.event,&flags)||(flags&HANDLE_FLAG_INHERIT))return scqt_fail(c,ERROR_INVALID_HANDLE);
 /* Kernel endpoint PID is correlated with retained originals AND full token,
  * SCM/image/profile/source checks above. PID/name alone is never authority. */
 if(c->role==1U){if(!GetNamedPipeClientProcessId(c->transport.pipe,&pid))return scqt_fail(c,GetLastError());}
 else if(!GetNamedPipeServerProcessId(c->transport.pipe,&pid))return scqt_fail(c,GetLastError());
 if(pid!=remote->pid)return scqt_fail(c,ERROR_ACCESS_DENIED);
 return 0;
}
static DWORD scqt_prepare(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,HANDLE original_pipe,HANDLE original_event,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;DWORD e;
 if(!scqs_owned(c,owner)||c->transport.phase||!stop||!original_pipe||!original_event||original_pipe==original_event||
 c->phase!=(c->role==1U?3U:1U)||!proof||!proof->controller_consumed||proof->original!=proof)return ERROR_INVALID_STATE;
 /* Borrow only from the future original-endpoint custodian. This function
  * verifies references but CANNOT prove creation provenance from two HANDLEs.
  * The root has no constructor/activation until that separate gate exists. */
 t=&c->transport;t->self=t;t->owner=owner;t->original=proof;t->thread=GetCurrentThreadId();t->phase=1;
 if(!DuplicateHandle(GetCurrentProcess(),original_pipe,GetCurrentProcess(),&t->pipe,
 FILE_READ_DATA|FILE_WRITE_DATA|SYNCHRONIZE,FALSE,0))return scqt_fail(c,GetLastError());
 if(!DuplicateHandle(GetCurrentProcess(),original_event,GetCurrentProcess(),&t->event,
 EVENT_MODIFY_STATE|SYNCHRONIZE,FALSE,0))return scqt_fail(c,GetLastError());
 if(GetFileType(t->pipe)!=FILE_TYPE_PIPE)return scqt_fail(c,ERROR_INVALID_HANDLE);
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 return 0;
}
/* Endpoint factory is compiled but has NO activation from the native root.
 * Offline tests replace creation/connection/open APIs. This does not designate
 * an operation, select a service or manufacture an original-source proof. */
/* Reserve only the protected namespace from the already-consumed controller
 * source, before a runtime PID exists. This is not connection/authentication.
 * The caller still requires its independent original endpoint authorization. */
static DWORD scqt_reserve(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_TRANSPORT *t;PAPACC_SCQ_ADMIN_SOURCE *source;
 SECURITY_ATTRIBUTES sa;BYTE system[SECURITY_MAX_SID_SIZE];DWORD n=sizeof(system),e;unsigned i;
 static const WCHAR prefix[]=L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.";
 if(!scqs_owned(c,owner)||c->role!=1U||c->phase!=2U||c->transport.phase||!c->source||!stop)return ERROR_INVALID_STATE;
 source=c->source;
 if(!scqa_startup_original(source,owner,stop)||source->start_phase!=2U||source->start_consumed||
  scqa_start_current(source,owner,stop)||scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p)||
  !scqa_closed_diagnostic_scope(source,&p)||!scqp_nonzero(p.attempt,16))return ERROR_ACCESS_DENIED;
 t=&c->transport;t->self=t;t->owner=owner;t->original=&source->value;t->thread=GetCurrentThreadId();t->phase=2U;
 t->read_diagnostic=c->profile->read_diagnostic;
 if(t->read_diagnostic){PAPACC_SCQ_READ_DIAGNOSTIC *k=c->read_act;
  if(!k||k->self!=k||k->owner!=owner||k->source!=source||k->thread!=GetCurrentThreadId()||k->phase!=2U||k->consumed||!scqp_nonzero(k->key_public,32))return scqt_fail(c,ERROR_ACCESS_DENIED);
  memcpy(t->read_public,k->key_public,32);
 }
 memcpy(t->name,prefix,sizeof(prefix)-sizeof(WCHAR));n=(DWORD)(sizeof(prefix)/sizeof(WCHAR)-1U);
 for(i=0;i<16U;i++){t->name[n+2U*i]=L"0123456789abcdef"[p.attempt[i]>>4];t->name[n+2U*i+1U]=L"0123456789abcdef"[p.attempt[i]&15U];}t->name[n+32U]=0;
 n=sizeof(system);
 if(!CreateWellKnownSid(WinLocalSystemSid,NULL,system,&n)||!InitializeAcl((PACL)t->acl,sizeof(t->acl),ACL_REVISION)||
  !AddAccessAllowedAceEx((PACL)t->acl,ACL_REVISION,0,0x00120183UL,system)||
  !AddAccessAllowedAceEx((PACL)t->acl,ACL_REVISION,0,0x00120183UL,(PSID)p.administrator.sid)||
  !AddAccessAllowedAceEx((PACL)t->acl,ACL_REVISION,0,0x00120183UL,(PSID)p.runtime.sid)||
  !InitializeSecurityDescriptor(&t->security,SECURITY_DESCRIPTOR_REVISION)||!SetSecurityDescriptorOwner(&t->security,(PSID)p.administrator.sid,FALSE)||
  !SetSecurityDescriptorDacl(&t->security,TRUE,(PACL)t->acl,FALSE)||!SetSecurityDescriptorControl(&t->security,SE_DACL_PROTECTED,SE_DACL_PROTECTED))return scqt_fail(c,ERROR_INVALID_SECURITY_DESCR);
 sa.nLength=sizeof(sa);sa.lpSecurityDescriptor=&t->security;sa.bInheritHandle=FALSE;
 e=scqa_current(source,owner,stop);if(e)return scqt_fail(c,e);
 e=scqt_resource_effect(c,owner,stop,1);if(e)return scqt_fail(c,e);
 t->event=CreateEventW(NULL,TRUE,FALSE,NULL);if(!t->event)return scqt_fail(c,GetLastError());
 e=scqa_current(source,owner,stop);if(e)return scqt_fail(c,e);
 e=scqt_resource_effect(c,owner,stop,1);if(e)return scqt_fail(c,e);
 t->pipe=CreateNamedPipeW(t->name,PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED|FILE_FLAG_FIRST_PIPE_INSTANCE,
  PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT|PIPE_REJECT_REMOTE_CLIENTS,1,PAPACC_SCQ_RECEIPT_BYTES,PAPACC_SCQ_RECEIPT_BYTES,0,&sa);
 if(t->pipe==INVALID_HANDLE_VALUE){t->pipe=NULL;return scqt_fail(c,GetLastError());}
 return t->pipe?0:scqt_fail(c,ERROR_INVALID_HANDLE);
}
static DWORD scqt_create(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 static const WCHAR prefix[]=L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.";
 PAPACC_SCQ_TRANSPORT *t;SECURITY_ATTRIBUTES sa;BYTE random[16],system[SECURITY_MAX_SID_SIZE];DWORD n=sizeof(system),e;unsigned i;
 if(!scqs_owned(c,owner)||c->role!=1U||c->phase!=3U||c->transport.phase||!proof||proof->original!=proof||proof->controller!=owner||!proof->controller_consumed)return ERROR_INVALID_STATE;
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return e;
 if(!scqa_operator(proof,local)||!personal_scq_original_refs(proof)||!personal_scq_original_binding(proof,c->dispatch,c->runtime)||!scqs_runtime_snapshot(c->runtime,remote))return ERROR_ACCESS_DENIED;
 t=&c->transport;t->self=t;t->owner=owner;t->original=proof;t->thread=GetCurrentThreadId();t->phase=2;
 if(c->source&&c->source->start_consumed){PS_SCQ_CARRIER carrier;
  e=scqa_start_carrier(c->source,owner,&carrier,stop);if(e)return scqt_fail(c,e);
  memcpy(random,carrier.endpoint,sizeof(random));
 }else if(BCryptGenRandom(NULL,random,sizeof(random),BCRYPT_USE_SYSTEM_PREFERRED_RNG))return scqt_fail(c,ERROR_GEN_FAILURE);
 memcpy(t->name,prefix,sizeof(prefix)-sizeof(WCHAR));n=(DWORD)(sizeof(prefix)/sizeof(WCHAR)-1);
 for(i=0;i<16;i++){t->name[n+2*i]=L"0123456789abcdef"[random[i]>>4];t->name[n+2*i+1]=L"0123456789abcdef"[random[i]&15];}t->name[n+32]=0;
 n=sizeof(system);
 if(!CreateWellKnownSid(WinLocalSystemSid,NULL,system,&n)||!InitializeAcl((PACL)t->acl,sizeof(t->acl),ACL_REVISION)||
 !AddAccessAllowedAceEx((PACL)t->acl,ACL_REVISION,0,0x00120183UL,system)||
 !AddAccessAllowedAceEx((PACL)t->acl,ACL_REVISION,0,0x00120183UL,(PSID)local->sid)||
 !AddAccessAllowedAceEx((PACL)t->acl,ACL_REVISION,0,0x00120183UL,(PSID)remote->sid)||
 !InitializeSecurityDescriptor(&t->security,SECURITY_DESCRIPTOR_REVISION)||!SetSecurityDescriptorOwner(&t->security,(PSID)proof->operator_sid,FALSE)||
 !SetSecurityDescriptorDacl(&t->security,TRUE,(PACL)t->acl,FALSE)||!SetSecurityDescriptorControl(&t->security,SE_DACL_PROTECTED,SE_DACL_PROTECTED))return scqt_fail(c,ERROR_INVALID_SECURITY_DESCR);
 sa.nLength=sizeof(sa);sa.lpSecurityDescriptor=&t->security;sa.bInheritHandle=FALSE;
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return scqt_fail(c,e);
 t->event=CreateEventW(NULL,TRUE,FALSE,NULL);if(!t->event)return scqt_fail(c,GetLastError());
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return scqt_fail(c,e);
 t->pipe=CreateNamedPipeW(t->name,PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED|FILE_FLAG_FIRST_PIPE_INSTANCE,
 PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT|PIPE_REJECT_REMOTE_CLIENTS,1,PAPACC_SCQ_RECEIPT_BYTES,PAPACC_SCQ_RECEIPT_BYTES,0,&sa);
 if(t->pipe==INVALID_HANDLE_VALUE){t->pipe=NULL;return scqt_fail(c,GetLastError());}
 if(!t->pipe)return scqt_fail(c,ERROR_INVALID_HANDLE);return 0;
}
static DWORD scqt_connect(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD e;
 if(!scqt_owned(c,owner)||c->role!=1U||t->phase!=2U||t->pending||t->operation||t->cancelled||t->connect_started)return ERROR_INVALID_STATE;
 if(t->original!=proof)return scqt_fail(c,ERROR_ACCESS_DENIED);
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return scqt_fail(c,e);
 t->connect_started=1;if(!ResetEvent(t->event))return scqt_fail(c,GetLastError());memset(&t->ov,0,sizeof(t->ov));t->ov.hEvent=t->event;t->operation=3;t->pending=1;
 if(!ConnectNamedPipe(t->pipe,&t->ov)){e=GetLastError();if(e==ERROR_IO_PENDING)return e;
 t->pending=0;if(e!=ERROR_PIPE_CONNECTED)return scqt_fail(c,e);}
 t->pending=0;t->operation=0;return 0;
}
static DWORD scqt_connected(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 if(!scqt_owned(c,owner))return ERROR_INVALID_STATE;
 if(c->transport.phase!=2U||c->transport.pending||c->transport.operation)return ERROR_INVALID_STATE;
 c->transport.phase=1;return scqt_current(c,owner,local,remote,refs,proof,stop);
}
static DWORD scqt_open_local(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,const WCHAR *name,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 static const WCHAR prefix[]=L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.";
 unsigned i,n=(unsigned)(sizeof(prefix)/sizeof(WCHAR)-1);PAPACC_SCQ_TRANSPORT *t;DWORD e;
 if(!scqs_owned(c,owner)||c->role!=2U||c->phase!=1U||c->transport.phase||!name||!proof||proof->original!=proof||proof->recipient!=owner||(!proof->controller_consumed&&!c->transport.query_only))return ERROR_INVALID_STATE;
 for(i=0;i<n;i++)if(name[i]!=prefix[i])return ERROR_INVALID_NAME;
 for(;i<n+32;i++)if(!((name[i]>=L'0'&&name[i]<=L'9')||(name[i]>=L'a'&&name[i]<=L'f')))return ERROR_INVALID_NAME;
 if(name[i])return ERROR_INVALID_NAME;
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return e;
 if(!scqa_operator(proof,remote)||!personal_scq_original_refs(proof)||!personal_scq_original_binding(proof,c->dispatch,c->runtime)||!scqs_runtime_snapshot(c->runtime,local))return ERROR_ACCESS_DENIED;
 t=&c->transport;t->self=t;t->owner=owner;t->original=proof;t->thread=GetCurrentThreadId();t->phase=1;
 memcpy(t->name,name,(i+1U)*sizeof(WCHAR));
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return scqt_fail(c,e);
 t->event=CreateEventW(NULL,TRUE,FALSE,NULL);if(!t->event)return scqt_fail(c,GetLastError());
 e=scqs_current(c,owner,local,remote,refs,stop);if(e)return scqt_fail(c,e);
 t->pipe=CreateFileW(t->name,0x00120183UL,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED|SECURITY_SQOS_PRESENT|SECURITY_ANONYMOUS,NULL);
 if(t->pipe==INVALID_HANDLE_VALUE){t->pipe=NULL;return scqt_fail(c,GetLastError());}
 return scqt_current(c,owner,local,remote,refs,proof,stop);
}
/* Sender-only push. PROCESS_DUP_HANDLE stays in the elevated controller:
 * never give that reusable administrative capability to the runtime. The
 * three resulting values belong to the destination handle table; they are
 * custody metadata, not serialized as original authority in the DATA frame. */
static DWORD scqt_deliver_refs(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;HANDLE originals[3],queries[2];unsigned i;DWORD e;
 if(!scqt_owned(c,owner)||c->role!=1U||t->delivery_process)return ERROR_INVALID_STATE;
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 originals[0]=proof->request;originals[1]=proof->decision;originals[2]=proof->image;
 for(i=0;i<3;i++)if(refs[i].role!=i+1U||refs[i].rights!=PAPACC_SCQ_FILE_READ_RIGHTS)return scqt_fail(c,ERROR_ACCESS_DENIED);
 /* Candidate acquired only while the ORIGINAL runtime reference remains live;
  * PID/creation are checked again, against that original, before duplication. */
 e=scqt_resource_effect(c,owner,stop,1);if(e)return scqt_fail(c,e);
 t->delivery_process=OpenProcess(PROCESS_DUP_HANDLE|PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,remote->pid);
 if(!t->delivery_process||!scqt_process(t->delivery_process,remote)||!scqt_process(c->runtime->process,remote))return scqt_fail(c,ERROR_ACCESS_DENIED);
 for(i=0;i<3;i++){
  e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
  e=scqt_resource_effect(c,owner,stop,1);if(e)return scqt_fail(c,e);
  e=scqt_destination_effect(c,t->delivery_process,stop);if(e)return scqt_fail(c,e);
  if(!DuplicateHandle(GetCurrentProcess(),originals[i],t->delivery_process,&t->remote_files[i],PAPACC_SCQ_FILE_READ_RIGHTS,FALSE,0))return scqt_fail(c,GetLastError());
  if(!t->remote_files[i])return scqt_fail(c,ERROR_INVALID_HANDLE);
 }
 if(t->resource_scope&&t->resource_scope->p1_handles){
  if((c->dispatch->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&!(c->profile->read_diagnostic==1U&&c->dispatch->purpose==PAPACC_SCQ_PURPOSE_READ&&t->read_diagnostic==1U))||!c->profile->pre_readiness||!t->global_until)return scqt_fail(c,ERROR_ACCESS_DENIED);
  t->query_refs3=1U;queries[0]=proof->operator_process;queries[1]=proof->operator_token;
  for(i=0;i<2U;i++){
   e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
   e=scqt_resource_effect(c,owner,stop,1);if(e)return scqt_fail(c,e);
   e=scqt_destination_effect(c,t->delivery_process,stop);if(e)return scqt_fail(c,e);
   if(!DuplicateHandle(GetCurrentProcess(),queries[i],t->delivery_process,&t->remote_query[i],i?TOKEN_QUERY:(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE),FALSE,0))return scqt_fail(c,GetLastError());
   if(!t->remote_query[i])return scqt_fail(c,ERROR_INVALID_HANDLE);
  }
 }
 return scqt_current(c,owner,local,remote,refs,proof,stop);
}
/* Receiver-local originals are supplied by the independent original-reference
 * handoff, NOT deserialized from a sender HANDLE/pointer or the DATA frame.
 * Capturing these metadata alone does not manufacture the OHI source. */
static DWORD scqt_accept_refs(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,const HANDLE local_originals[3],
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 unsigned i,j;DWORD e;BY_HANDLE_FILE_INFORMATION info;PAPACC_SCQ_TRANSPORT *t=&c->transport;
 if(!scqt_owned(c,owner)||c->transport.query_only||c->role!=2U||!local_originals||t->files[0]||t->files[1]||t->files[2])return ERROR_INVALID_STATE;
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 for(i=0;i<3;i++){
  if(refs[i].role!=i+1U||refs[i].rights!=PAPACC_SCQ_FILE_READ_RIGHTS||!local_originals[i])return scqt_fail(c,ERROR_ACCESS_DENIED);
  e=scqt_resource_effect(c,owner,stop,1);if(e)return scqt_fail(c,e);
  if(!DuplicateHandle(GetCurrentProcess(),local_originals[i],GetCurrentProcess(),&t->files[i],PAPACC_SCQ_FILE_READ_RIGHTS,FALSE,0))return scqt_fail(c,GetLastError());
  if(!t->files[i]||GetFileType(t->files[i])!=FILE_TYPE_DISK||!GetFileInformationByHandle(t->files[i],&info)||
  info.dwVolumeSerialNumber!=refs[i].volume||info.nFileIndexHigh!=refs[i].high||info.nFileIndexLow!=refs[i].low||
  info.nFileSizeLow!=refs[i].size_low||info.nFileSizeHigh!=refs[i].size_high||info.dwFileAttributes!=refs[i].attributes||
  info.nNumberOfLinks!=refs[i].links||(info.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))return scqt_fail(c,ERROR_INVALID_DATA);
  for(j=0;j<i;j++)if(t->files[j]==t->files[i])return scqt_fail(c,ERROR_INVALID_HANDLE);
 }
 return scqt_current(c,owner,local,remote,refs,proof,stop);
}
#include "personal_scq_failure_win32.inc"
static DWORD scqt_io_result(PAPACC_SCQ_SESSION_DRIVER *c,DWORD n)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;
 if(!n||n>t->total-t->offset)return scqt_fail(c,ERROR_INVALID_DATA);
 t->offset+=n;
 if(c->role==1U&&t->query_refs3&&t->operation==2U&&t->offset>=4U){
  if(!memcmp(c->buffer,"SQF1",4)){
   if(t->offset>56U)return scqt_fail(c,ERROR_INVALID_DATA);
   t->total=56U;if(t->offset==56U)return scqf_received(c);
  }else if(memcmp(c->buffer,"SQR1",4))return scqt_fail(c,ERROR_INVALID_DATA);
 }
 if(t->offset==t->total)return 0;return ERROR_MORE_DATA;
}
static DWORD scqt_bootstrap_size(const PAPACC_SCQ_TRANSPORT *t)
{return t->read_diagnostic?PAPACC_SCQ_BOOTSTRAP_READ_BYTES:t->query_refs3?PAPACC_SCQ_BOOTSTRAP3_BYTES:t->global_until?PAPACC_SCQ_BOOTSTRAP_BYTES:PAPACC_SCQ_CLOSE_BYTES;}
static DWORD scqt_close_size(const PAPACC_SCQ_TRANSPORT *t)
{return t->query_refs3?PAPACC_SCQ_CLOSE3_BYTES:PAPACC_SCQ_CLOSE_BYTES;}
static int scqt_query_ids(const BYTE *frame,DWORD base,const ULONGLONG expected[2])
{
 unsigned i,j;ULONGLONG v;
 for(i=0;i<2U;i++){v=0;for(j=0;j<8U;j++)v|=(ULONGLONG)frame[base+8U*i+j]<<(8U*j);if(!v||v!=expected[i])return 0;}
 return 1;
}
/* K3/K6: local original P custody, not a DATA bit or purpose-only grant.
 * The caller still brackets every leg with scqt_current native peer/refs.
 * Ordinary READ has neither of these producer-owned bindings. */
static int scqt_read_result_closed(const PAPACC_SCQ_SESSION_DRIVER *c)
{
 PAPACC_SCQ_PLAN_DATA p;unsigned i;
 if(!c||!c->profile||!c->dispatch||!c->profile->pre_readiness||c->dispatch->purpose!=PAPACC_SCQ_PURPOSE_READ||
  !c->transport.query_refs3||!c->transport.resource_scope||!c->transport.global_until||
  c->transport.global_until!=c->transport.resource_scope->global_until||GetTickCount64()>=c->transport.global_until||
  scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p)||p.purpose!=PAPACC_SCQ_PURPOSE_READ)return 0;
 if(c->role==1U){
  PAPACC_SCQ_READ_DIAGNOSTIC *k=c->read_act;
  if(!k||k->self!=k||k->owner!=c->owner||k->thread!=GetCurrentThreadId()||k->source!=c->source||k->phase!=3U||k->first||!k->consumed||
   (c->transport.legs&(128U|1024U))!=(128U|1024U)||memcmp(k->attempt,p.attempt,32)||memcmp(k->policy,p.policy,32)||
   memcmp(k->request,p.request,32)||memcmp(k->decision,p.decision,32))return 0;
  for(i=0;i<112U;i++)if(k->key[i]!=(WCHAR)(BYTE)p.key[i])return 0;
 }else if(c->role==2U){
  PAPACC_SCQ_READ_RECIPIENT *r=c->read_recipient;
  if(!r||r->self!=r||r->owner!=c->owner||r->thread!=GetCurrentThreadId()||r->session!=c||r->phase!=3U||r->first||!r->probe_used||
   !r->origin||!c->receiver||c->receiver->original!=&r->origin->value||c->transport.original!=&r->origin->value||
   !r->probe||r->probe->phase!=3U||r->probe->close_error||r->probe->provider||r->probe->key||
   (c->transport.legs&(256U|512U))!=(256U|512U))return 0;
 }else return 0;
 return 1;
}
static int scqt_closed_return_scope(const PAPACC_SCQ_SESSION_DRIVER *c)
{return c&&c->profile&&c->dispatch&&c->profile->pre_readiness&&
 (c->dispatch->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||scqt_read_result_closed(c));}
static DWORD scqt_start_io(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,DWORD operation,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD e,n=0;BOOL ok;unsigned i,j;ULONGLONG value;
 if(!scqt_owned(c,owner)||(t->query_only&&operation!=2U)||t->pending||t->cancelled||t->phase!=1U||(operation!=1U&&operation!=2U&&operation!=4U&&operation!=8U&&operation!=16U&&operation!=32U&&operation!=64U))return ERROR_INVALID_STATE;
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 if(!t->operation){
  if(t->legs&operation)return ERROR_INVALID_STATE;
  if(t->total)return ERROR_INVALID_STATE; /* completed IO must be explicitly applied before next leg */
  if(operation==64U){
   if(c->role!=1U||c->phase!=3U||!c->source||!c->source->start_consumed||!t->delivery_process||scqa_start_current(c->source,owner,stop))return ERROR_ACCESS_DENIED;
   t->total=scqt_bootstrap_size(t);
  }else if(operation==16U||operation==32U){
   if(!scqt_closed_return_scope(c)||c->phase!=4U||
    (operation==16U&&(c->role!=1U||!t->close_accepted||!(t->legs&8U)))||
    (operation==32U&&(c->role!=2U||t->close_produced!=3U||!(t->legs&4U))))return ERROR_INVALID_STATE;
   t->total=scqt_close_size(t);
  }else if(operation==4U){if(c->role!=2U||c->phase!=4U||t->close_produced!=1U)return ERROR_INVALID_STATE;t->total=scqt_close_size(t);}
  else if(operation==8U){if(c->role!=1U||c->phase!=4U||!t->delivery_process||t->close_accepted)return ERROR_INVALID_STATE;t->total=scqt_close_size(t);}
  else if(operation==1U){if(c->phase!=3U&&!(c->role==2U&&c->phase==4U))return ERROR_INVALID_STATE;t->total=c->bytes;}
  else {if(c->phase!=(c->role==1U?3U:1U))return ERROR_INVALID_STATE;t->total=c->role==1U?PAPACC_SCQ_RECEIPT_BYTES:PAPACC_SCQ_RECORD_BYTES;}
  if(!t->total||t->total>sizeof(c->buffer))return scqt_fail(c,ERROR_INVALID_DATA);
  t->operation=operation;
 }else if(t->operation!=operation||t->offset==t->total)return ERROR_INVALID_STATE;
 /* The return is the EXACT already-accepted original, not arbitrary DATA.
  * Validate before each write/submission, not only after bytes escaped. */
 if(operation==16U||operation==64U){
  if(operation==64U&&t->read_diagnostic&&(!t->query_refs3||memcmp(c->buffer+PAPACC_SCQ_BOOTSTRAP3_BYTES,t->read_public,32)))return scqt_fail(c,ERROR_INVALID_DATA);
  if(memcmp(c->buffer,operation==64U?(t->read_diagnostic?"SQK1":t->query_refs3?"SQB3":t->global_until?"SQB2":"SQB1"):(t->query_refs3?"SQC3":"SQC1"),4)||memcmp(c->buffer+4,c->profile->record,PAPACC_SCQ_RECORD_BYTES))return scqt_fail(c,ERROR_INVALID_DATA);
  if(operation==64U&&t->global_until){value=0;for(j=0;j<8U;j++)value|=(ULONGLONG)c->buffer[PAPACC_SCQ_CLOSE_BYTES+j]<<(8U*j);if(value!=t->global_until)return scqt_fail(c,ERROR_INVALID_DATA);}
  for(i=0;i<3U;i++){
   value=0;for(j=0;j<8U;j++)value|=(ULONGLONG)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*i+j]<<(8U*j);
   if(!t->remote_files[i]||value!=(ULONGLONG)(ULONG_PTR)t->remote_files[i])return scqt_fail(c,ERROR_INVALID_DATA);
  }
 }
 if(t->query_refs3&&(operation==16U||operation==64U)){
  ULONGLONG ids[2];ids[0]=(ULONGLONG)(ULONG_PTR)t->remote_query[0];ids[1]=(ULONGLONG)(ULONG_PTR)t->remote_query[1];
  if(!scqt_query_ids(c->buffer,operation==64U?PAPACC_SCQ_BOOTSTRAP_BYTES:PAPACC_SCQ_CLOSE_BYTES,ids))return scqt_fail(c,ERROR_INVALID_DATA);
 }
 if(!ResetEvent(t->event))return scqt_fail(c,GetLastError());
 memset(&t->ov,0,sizeof(t->ov));t->ov.hEvent=t->event;t->pending=1;
 e=scqt_resource_effect(c,owner,stop,0);if(e){t->pending=0;return scqt_fail(c,e);}
 if(operation==1U||operation==4U||operation==16U||operation==64U)ok=WriteFile(t->pipe,c->buffer+t->offset,t->total-t->offset,&n,&t->ov);
 else ok=ReadFile(t->pipe,c->buffer+t->offset,t->total-t->offset,&n,&t->ov);
 if(!ok){e=GetLastError();if(e==ERROR_IO_PENDING)return e;t->pending=0;return scqt_fail(c,e);}
 t->pending=0;return scqt_io_result(c,n);
}
static DWORD scqt_poll(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD n=0,e;
 if(!scqt_owned(c,owner)||!t->pending)return ERROR_INVALID_STATE;
 if(!GetOverlappedResult(t->pipe,&t->ov,&n,FALSE)){e=GetLastError();if(e==ERROR_IO_INCOMPLETE)return ERROR_IO_PENDING;
 t->pending=0;return scqt_fail(c,e);}
 t->pending=0;if(t->phase==6U)return t->first;if(t->operation==3U){t->operation=0;return 0;}return scqt_io_result(c,n);
}
static DWORD scqt_stop(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;
 if(!scqt_owned(c,owner))return ERROR_INVALID_STATE;
 if(t->pending&&!t->cancelled){t->cancelled=1;
 if(!CancelIoEx(t->pipe,&t->ov)&&GetLastError()!=ERROR_NOT_FOUND)return scqt_fail(c,GetLastError());}
 /* Cancellation is a request, not completion. UNKNOWN keeps buffers/OV/refs. */
 return scqt_fail(c,ERROR_CANCELLED);
}
static DWORD scqt_apply(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,DWORD operation,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD e;
 if(!scqt_owned(c,owner)||t->query_only||t->phase!=1U||t->pending||t->operation!=operation||!t->total||t->offset!=t->total)return ERROR_INVALID_STATE;
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 if(operation==2U){
  t->applying=1;
  if(c->role==1U)e=scqs_ack(c,owner,local,remote,refs,c->buffer,t->total,stop);
  else {
   /* Metadata acquisition alone is NOT independent content/SD/provenance.
    * Require the existing receiver-local original, never build one here. */
   if(!t->files[0]||!t->files[1]||!t->files[2]||!personal_scq_original_file(t->files[0],&proof->request_id)||!personal_scq_original_file(t->files[1],&proof->decision_id)||!personal_scq_original_file(t->files[2],&proof->image_id)){t->applying=0;return scqt_fail(c,ERROR_ACCESS_DENIED);}
   e=scqs_receive(c,owner,local,remote,refs,c->buffer,t->total,proof,stop);
  }
  t->applying=0;if(e)return scqt_fail(c,e);
 }else if(operation!=1U)return ERROR_INVALID_PARAMETER;
 t->legs|=operation;t->operation=t->total=t->offset=0;return 0; /* DATA-only receipt; native auth NOT asserted */
}
/* Move ownership of the three originally delivered local references. The
 * independent receiver-origin readers MUST be separate references: closing
 * these cannot invalidate the operator/decision evidence still in use. */
static DWORD scqt_adopt_delivery(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,HANDLE originals[3],
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 unsigned i,j;DWORD e;
 if(!scqt_owned(c,owner)||c->role!=2U||!originals||c->transport.delivered_inputs[0]||c->transport.delivered_inputs[1]||c->transport.delivered_inputs[2])return ERROR_INVALID_STATE;
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 for(i=0;i<3;i++){
  if(!originals[i]||originals[i]==proof->request||originals[i]==proof->decision||originals[i]==proof->image)return scqt_fail(c,ERROR_INVALID_HANDLE);
  for(j=0;j<3;j++)if(originals[i]==c->transport.files[j])return scqt_fail(c,ERROR_INVALID_HANDLE);
  for(j=0;j<i;j++)if(originals[j]==originals[i])return scqt_fail(c,ERROR_INVALID_HANDLE);
  if(!personal_scq_original_file(originals[i],i==0?&proof->request_id:i==1?&proof->decision_id:&proof->image_id))return scqt_fail(c,ERROR_INVALID_DATA);
 }
 for(i=0;i<3;i++){c->transport.delivered_inputs[i]=originals[i];originals[i]=NULL;}
 return 0;
}
static DWORD scqt_produce_close(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;unsigned i,j;DWORD e;ULONGLONG closed;
 if(!scqt_owned(c,owner)||c->role!=2U||c->phase!=4U||t->pending||t->operation||t->total||t->close_produced||!(t->legs&1U))return ERROR_INVALID_STATE;
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 for(i=0;i<3;i++)if(!t->files[i]||!t->delivered_inputs[i])return scqt_fail(c,ERROR_NOT_FOUND);
 t->close_produced=2; /* attempted, never retry even a failed native close */
 for(i=0;i<3;i++){
  if(!CloseHandle(t->files[i]))return scqt_fail(c,GetLastError());t->files[i]=NULL;
  closed=(ULONGLONG)(ULONG_PTR)t->delivered_inputs[i];
  if(!CloseHandle(t->delivered_inputs[i]))return scqt_fail(c,GetLastError());t->closed_values[i]=closed;t->delivered_inputs[i]=NULL;
 }
 /* Separate private close stage. SQR1 remains unchanged DATA. The endpoint
  * must still be correlated with original peers before and after close IO. */
 memcpy(c->buffer,t->query_refs3?"SQC3":"SQC1",4);memcpy(c->buffer+4,c->profile->record,PAPACC_SCQ_RECORD_BYTES);
 for(i=0;i<3;i++)for(j=0;j<8;j++)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*i+j]=(BYTE)(t->closed_values[i]>>(8U*j));
 if(t->query_refs3){
  if(t->capture_controller||t->capture_token||!t->closed_query[0]||!t->closed_query[1])return scqt_fail(c,ERROR_IO_INCOMPLETE);
  for(i=0;i<2U;i++)for(j=0;j<8U;j++)c->buffer[PAPACC_SCQ_CLOSE_BYTES+8U*i+j]=(BYTE)(t->closed_query[i]>>(8U*j));
 }
 c->bytes=scqt_close_size(t);
 t->close_produced=1;return scqt_current(c,owner,local,remote,refs,proof,stop);
}
static DWORD scqt_apply_close(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,DWORD operation,
 const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],
 PAPACC_PERSONAL_SCQ_ORIGINAL *proof,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD e;unsigned i,j;ULONGLONG value;
 if(!scqt_owned(c,owner)||t->phase!=1U||t->pending||t->operation!=operation||t->total!=scqt_close_size(t)||t->offset!=t->total||t->legs&operation)return ERROR_INVALID_STATE;
 if((operation!=4U&&operation!=8U&&operation!=16U&&operation!=32U)||(operation==4U&&(c->role!=2U||t->close_produced!=1U))||(operation==8U&&(c->role!=1U||!t->delivery_process||t->close_accepted)))return ERROR_INVALID_STATE;
 if(operation==16U||operation==32U){
  if(!scqt_closed_return_scope(c)||
   (operation==16U&&(c->role!=1U||!t->close_accepted||!(t->legs&8U)))||
   (operation==32U&&(c->role!=2U||t->close_produced!=3U||!(t->legs&4U)||t->close_confirmed)))return ERROR_INVALID_STATE;
 }
 e=scqt_current(c,owner,local,remote,refs,proof,stop);if(e)return e;
 if(memcmp(c->buffer,t->query_refs3?"SQC3":"SQC1",4)||memcmp(c->buffer+4,c->profile->record,PAPACC_SCQ_RECORD_BYTES))return scqt_fail(c,ERROR_INVALID_DATA);
 if(t->query_refs3){
  ULONGLONG ids[2];
  if(c->role==1U){ids[0]=(ULONGLONG)(ULONG_PTR)t->remote_query[0];ids[1]=(ULONGLONG)(ULONG_PTR)t->remote_query[1];}
  else {ids[0]=t->closed_query[0];ids[1]=t->closed_query[1];}
  if(!scqt_query_ids(c->buffer,PAPACC_SCQ_CLOSE_BYTES,ids))return scqt_fail(c,ERROR_INVALID_DATA);
 }
 if(operation==8U||operation==16U){
  for(i=0;i<3;i++){
   value=0;for(j=0;j<8;j++)value|=(ULONGLONG)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*i+j]<<(8U*j);
   /* These are CLOSED destination-table IDs from this exact transfer. Never
    * cast received values to HANDLE or invoke an API against them. */
   if(!t->remote_files[i]||value!=(ULONGLONG)(ULONG_PTR)t->remote_files[i])return scqt_fail(c,ERROR_INVALID_DATA);
  }
  t->close_accepted=1;
 }else if(operation==32U){
  for(i=0;i<3;i++){
   value=0;for(j=0;j<8;j++)value|=(ULONGLONG)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*i+j]<<(8U*j);
   /* The authenticated original controller can return this record only
    * after validating its exact original delivery. IDs stay comparisons. */
   if(!t->closed_values[i]||value!=t->closed_values[i])return scqt_fail(c,ERROR_INVALID_DATA);
  }
  t->close_confirmed=1;
 }else t->close_produced=3;
 t->legs|=operation;t->operation=t->total=t->offset=0;return 0;
}
static DWORD scqt_close(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;unsigned i;
 if(!t->phase)return 0;if(!scqt_owned(c,owner)||t->phase==6U)return ERROR_INVALID_STATE;if(t->phase==5U)return 0;
 if(t->pending||t->operation||t->total){if(t->pending)(void)scqt_stop(c,owner);return scqt_fail(c,ERROR_IO_INCOMPLETE);}
 /* DATA receipt cannot prove remote handles closed. Require the separate
  * close stage correlated with the original peer, or observed exit of the
  * retained destination process proving its handle table was destroyed.
  * Never issue DUPLICATE_CLOSE_SOURCE against stale remote numeric values. */
 if(t->delivery_process){
  if(!t->close_accepted&&WaitForSingleObject(t->delivery_process,0)!=WAIT_OBJECT_0)return scqt_fail(c,ERROR_IO_INCOMPLETE);
  memset(t->remote_files,0,sizeof(t->remote_files));memset(t->remote_query,0,sizeof(t->remote_query));
 }
 if(t->close_attempted)return ERROR_INVALID_STATE;t->close_attempted=1;
 for(i=0;i<3;i++)if(t->delivered_inputs[i]){if(!CloseHandle(t->delivered_inputs[i]))return scqt_fail(c,GetLastError());t->delivered_inputs[i]=NULL;}
 for(i=0;i<3;i++)if(t->files[i]){if(!CloseHandle(t->files[i]))return scqt_fail(c,GetLastError());t->files[i]=NULL;}
 if(t->event){if(!CloseHandle(t->event))return scqt_fail(c,GetLastError());t->event=NULL;}
 if(t->pipe){if(!CloseHandle(t->pipe))return scqt_fail(c,GetLastError());t->pipe=NULL;}
 if(t->delivery_process){if(!CloseHandle(t->delivery_process))return scqt_fail(c,GetLastError());t->delivery_process=NULL;}
 t->phase=5;return 0;
}
