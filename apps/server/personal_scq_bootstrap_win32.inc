/* Private first capture on the SAME pipe/packet. SQB1 carries only the
 * existing typed SCQ projection and three destination-table candidate IDs.
 * No ID/frame/nonce is an original or an authorization. Independent immutable
 * source selection, kernel peer/process/token, rights, original content/SD,
 * runtime/image correlation and final native close remain mandatory. */
#include <winternl.h>
static DWORD scqb_guard(PAPACC_SCQ_SESSION_DRIVER *c,volatile LONG *stop)
{
 DWORD count,e;if(!stop||*stop)return ERROR_CANCELLED;
 if(c->transport.capture_query&&!c->transport.resource_scope)return ERROR_ACCESS_DENIED;
 e=scqt_resource_effect(c,c->owner,stop,1);if(e)return e;
 if(c->transport.capture_query&&(!GetProcessHandleCount(GetCurrentProcess(),&count)||count>=PAPACC_SCQ_HANDLE_GUARD(c->transport.resource_scope&&c->transport.resource_scope->p1_handles)))return ERROR_NOT_ENOUGH_QUOTA;
 return 0;
}
/* First original QUERY-write predicate/return, no queries or sensitive references. */
static DWORD scqb_write_result(PAPACC_SCQ_SESSION_DRIVER *c,DWORD predicate,DWORD e)
{
 if(e&&e!=ERROR_IO_PENDING&&e!=ERROR_MORE_DATA)
  (void)fprintf(stderr,"SCQ_QUERY_WRITE_FIRST_CAUSE predicate=%lu error=%lu session_phase=%u transport_phase=%u operation=%lu pending=%lu read=%u refs3=%u; ORIGINAL_RETURN_ONLY_NO_NEW_QUERY_NO_AUTHORITY\n",(unsigned long)predicate,(unsigned long)e,c?c->phase:0U,c?c->transport.phase:0U,c?(unsigned long)c->transport.operation:0UL,c?(unsigned long)c->transport.pending:0UL,c?c->transport.read_diagnostic:0U,c?c->transport.query_refs3:0U);
 return e;
}
static DWORD scqb_write(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_TRANSPORT *t;DWORD e;unsigned i,j;ULONGLONG v;
 if(!scqt_owned(c,owner)||c->role!=1U||c->phase!=3U||!c->source||!c->source->start_consumed||!stop)return scqb_write_result(c,1U,ERROR_INVALID_STATE);
 t=&c->transport;
 if(t->global_until&&(!t->resource_scope||t->global_until!=t->resource_scope->global_until))return scqb_write_result(c,2U,ERROR_ACCESS_DENIED);
 if(scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p))return scqb_write_result(c,3U,ERROR_ACCESS_DENIED);
 if(!scqa_closed_diagnostic_scope(c->source,&p))return scqb_write_result(c,4U,ERROR_ACCESS_DENIED);
 if(t->read_diagnostic!=c->profile->read_diagnostic)return scqb_write_result(c,5U,ERROR_ACCESS_DENIED);
 if(!t->delivery_process)return scqb_write_result(c,6U,ERROR_ACCESS_DENIED);
 if(!t->operation){
  if(t->legs&1U)return scqb_write_result(c,7U,ERROR_INVALID_STATE);
  memcpy(c->buffer,t->read_diagnostic?"SQK1":t->query_refs3?"SQB3":t->global_until?"SQB2":"SQB1",4);memcpy(c->buffer+4,c->profile->record,PAPACC_SCQ_RECORD_BYTES);
  if(t->global_until)for(j=0;j<8U;j++)c->buffer[PAPACC_SCQ_CLOSE_BYTES+j]=(BYTE)(t->global_until>>(8U*j));
  if(t->query_refs3)for(i=0;i<2U;i++){
   if(!t->remote_query[i])return scqb_write_result(c,8U,ERROR_NOT_FOUND);v=(ULONGLONG)(ULONG_PTR)t->remote_query[i];
   for(j=0;j<8U;j++)c->buffer[PAPACC_SCQ_BOOTSTRAP_BYTES+8U*i+j]=(BYTE)(v>>(8U*j));
  }
  if(t->read_diagnostic){if(!t->query_refs3||!scqp_nonzero(t->read_public,32))return scqb_write_result(c,9U,ERROR_ACCESS_DENIED);memcpy(c->buffer+PAPACC_SCQ_BOOTSTRAP3_BYTES,t->read_public,32);}
  for(i=0;i<3U;i++){
   if(!t->remote_files[i])return scqb_write_result(c,10U,ERROR_NOT_FOUND);v=(ULONGLONG)(ULONG_PTR)t->remote_files[i];
   for(j=0;j<8U;j++)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*i+j]=(BYTE)(v>>(8U*j));
  }
 }
 if(t->pending)e=scqt_poll(c,owner);
 else e=scqt_start_io(c,owner,64U,&p.administrator,&p.runtime,p.refs,&c->source->value,stop);
 if(e)return scqb_write_result(c,11U,e);
 if(t->operation!=64U||t->pending||t->offset!=t->total||t->total!=scqt_bootstrap_size(t))return scqb_write_result(c,12U,ERROR_INVALID_STATE);
 e=scqt_current(c,owner,&p.administrator,&p.runtime,p.refs,&c->source->value,stop);if(e)return scqb_write_result(c,13U,e);
 t->legs|=1U;t->operation=t->total=t->offset=0;
 memcpy(c->buffer,c->profile->record,PAPACC_SCQ_RECORD_BYTES);c->bytes=PAPACC_SCQ_RECORD_BYTES;return 0;
}
static int scqb_scope(const PS_SCQ_CARRIER *v,const PAPACC_SCQ_OWNER_DECISION *r)
{
 ULONGLONG now,from,until;
 if(!v||!r||r->read_diagnostic>1U||r->purpose!=(r->read_diagnostic?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)||v->controller_pid!=r->controller_pid||
  memcmp(v->controller_creation,r->controller_creation,sizeof(FILETIME))||memcmp(v->until,r->valid_until,sizeof(v->until))||
  memcmp(v->attempt,r->attempt,32)||memcmp(v->policy,r->policy,32)||memcmp(v->target,r->target,32)||
  memcmp(v->request,r->request,32)||memcmp(v->decision,r->decision,32)||memcmp(v->image,r->image,32)||memcmp(v->endpoint,r->attempt,16)||
  v->runtime_sid_bytes!=r->runtime_bytes||v->runtime_sid_bytes>68U||memcmp(v->runtime_sid,r->runtime,68))return 0;
 now=GetTickCount64();from=((ULONGLONG)r->valid_from[1]<<32)|r->valid_from[0];until=((ULONGLONG)r->valid_until[1]<<32)|r->valid_until[0];
 return now>=from&&now<until;
}
static DWORD scqb_current(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;DWORD pid,n,flags;TOKEN_STATISTICS statistics;FILETIME creation,exit,kernel,user;
 if(!scqt_owned(c,owner)||c->phase||c->role!=2U||!stop)return ERROR_INVALID_STATE;t=&c->transport;
 if(t->phase!=3U||!t->capture_registration||!scqb_scope(&t->capture_carrier,t->capture_registration)||
  (t->global_until&&GetTickCount64()>=t->global_until)||
  t->cancelled||InterlockedCompareExchange(stop,0,0)||papacc_personal_runtime_current(c->runtime))return scqt_fail(c,ERROR_ACCESS_DENIED);
 if(!GetHandleInformation(t->pipe,&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(t->pipe)!=FILE_TYPE_PIPE||
  !GetNamedPipeServerProcessId(t->pipe,&pid)||pid!=t->capture_registration->controller_pid)return scqt_fail(c,ERROR_ACCESS_DENIED);
 if(t->query_refs3&&!t->capture_controller&&!t->capture_token&&!t->capture_valid&&t->capture_query==1U)return 0;
 if(
  !GetNamedPipeServerProcessId(t->pipe,&pid)||pid!=t->capture_registration->controller_pid||
  !t->capture_controller||GetProcessId(t->capture_controller)!=pid||WaitForSingleObject(t->capture_controller,0)!=WAIT_TIMEOUT||
  !GetProcessTimes(t->capture_controller,&creation,&exit,&kernel,&user)||memcmp(&creation,t->capture_registration->controller_creation,sizeof(creation))||
  !t->capture_token||!GetTokenInformation(t->capture_token,TokenStatistics,&statistics,sizeof(statistics),&n)||n!=sizeof(statistics)||
  memcmp(&statistics.TokenId,t->capture_registration->controller_token,sizeof(LUID))||
  memcmp(&statistics.AuthenticationId,t->capture_registration->controller_authentication,sizeof(LUID))||
  memcmp(&statistics.ModifiedId,t->capture_registration->controller_modified,sizeof(LUID)))return scqt_fail(c,ERROR_ACCESS_DENIED);
 return 0; /* kernel capture scope only, no administrative act promoted */
}
/* Only the pending first QUERY read may wait without an acquired peer.
 * The held pipe/event, local runtime, scope and nonrenewable deadline still gate it. */
static int scqb_initial_wait(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;
 return !c->phase&&t->query_refs3&&t->capture_query==1U&&!t->capture_valid&&
 !t->capture_controller&&!t->capture_token&&t->pending&&t->operation==65U&&
 t->event&&t->pipe&&!scqb_current(c,owner,stop);
}
static DWORD scqb_open_mode(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,const PS_SCQ_CARRIER *carrier,
 const PAPACC_SCQ_OWNER_DECISION *registered,PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,
 PAPACC_SCQ_PROFILE_DATA *profile,PAPACC_PERSONAL_SCQ_RECEIVER *receiver,PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,volatile LONG *stop,int query)
{
 static const WCHAR prefix[]=L"\\\\.\\pipe\\PapinhoAccelerator.SCQ.";
 PAPACC_SCQ_TRANSPORT *t;unsigned i,n=(unsigned)(sizeof(prefix)/sizeof(WCHAR)-1U);DWORD e;
 if(!c||c->phase||c->self||!owner||!carrier||!registered||!runtime||!profile||!receiver||!dispatch||!stop||
  !scqb_scope(carrier,registered)||runtime->closed_preparation!=1U||runtime->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||
  runtime->sid_bytes!=carrier->runtime_sid_bytes||memcmp(runtime->sid,carrier->runtime_sid,68)||papacc_personal_runtime_current(runtime)||InterlockedCompareExchange(stop,0,0))return ERROR_ACCESS_DENIED;
 c->self=c;c->owner=owner;c->thread=GetCurrentThreadId();c->role=2U;c->runtime=runtime;c->profile=profile;c->receiver=receiver;c->dispatch=dispatch;
 t=&c->transport;t->read_diagnostic=registered->read_diagnostic;t->self=t;t->owner=owner;t->thread=GetCurrentThreadId();t->phase=3U;t->capture_registration=registered;t->capture_carrier=*carrier;t->capture_query=query?1U:0U;
 if(query)t->global_until=((ULONGLONG)carrier->global_until[1]<<32)|carrier->global_until[0];
 memcpy(t->name,prefix,sizeof(prefix)-sizeof(WCHAR));
 for(i=0;i<16U;i++){t->name[n+2U*i]=L"0123456789abcdef"[carrier->endpoint[i]>>4];t->name[n+2U*i+1U]=L"0123456789abcdef"[carrier->endpoint[i]&15U];}t->name[n+32U]=0;
 e=scqb_guard(c,stop);if(e)return scqt_fail(c,e);
 t->event=CreateEventW(NULL,TRUE,FALSE,NULL);if(!t->event)return scqt_fail(c,GetLastError());
 if(InterlockedCompareExchange(stop,0,0)||!scqb_scope(carrier,registered))return scqt_fail(c,ERROR_CANCELLED);
 e=scqb_guard(c,stop);if(e)return scqt_fail(c,e);
 t->pipe=CreateFileW(t->name,0x00120183UL,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED|SECURITY_SQOS_PRESENT|SECURITY_ANONYMOUS,NULL);
 if(t->pipe==INVALID_HANDLE_VALUE){t->pipe=NULL;return scqt_fail(c,GetLastError());}
 if(query&&t->resource_scope&&t->resource_scope->p1_handles){
  if(!t->global_until)return scqt_fail(c,ERROR_ACCESS_DENIED);
  t->query_refs3=1U;return scqb_current(c,owner,stop);
 }
 e=scqb_guard(c,stop);if(e)return scqt_fail(c,e);
 t->capture_controller=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,registered->controller_pid);
 if(!t->capture_controller)return scqt_fail(c,GetLastError());e=scqb_guard(c,stop);if(e)return scqt_fail(c,e);
 if(!OpenProcessToken(t->capture_controller,TOKEN_QUERY,&t->capture_token))return scqt_fail(c,GetLastError());
 if(query){TOKEN_STATISTICS statistics;DWORD bytes;PAPACC_SCQ_OWNER_DECISION *candidate=(PAPACC_SCQ_OWNER_DECISION *)registered;
  if(!GetTokenInformation(t->capture_token,TokenStatistics,&statistics,sizeof(statistics),&bytes)||bytes!=sizeof(statistics))return scqt_fail(c,ERROR_ACCESS_DENIED);
  memcpy(candidate->controller_token,&statistics.TokenId,sizeof(LUID));memcpy(candidate->controller_authentication,&statistics.AuthenticationId,sizeof(LUID));memcpy(candidate->controller_modified,&statistics.ModifiedId,sizeof(LUID));
 }
 e=scqb_current(c,owner,stop);return e;
}
static DWORD scqb_open(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,const PS_SCQ_CARRIER *carrier,
 const PAPACC_SCQ_OWNER_DECISION *registered,PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,
 PAPACC_SCQ_PROFILE_DATA *profile,PAPACC_PERSONAL_SCQ_RECEIVER *receiver,PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,volatile LONG *stop)
{return scqb_open_mode(c,owner,carrier,registered,runtime,profile,receiver,dispatch,stop,0);}
/* Accepted initial QUERY policy only. Local selector backing is not an act or
 * remote pointer. No source/confirmation/consumption is fabricated here. */
static DWORD scqb_open_query_variant(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,const PS_SCQ_CARRIER *carrier,
 PAPACC_SCQ_OWNER_DECISION *candidate,PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,
 PAPACC_SCQ_PROFILE_DATA *profile,PAPACC_PERSONAL_SCQ_RECEIVER *receiver,PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,volatile LONG *stop,unsigned read_diagnostic)
{
 ULONGLONG now,until;if(!c||c->self||!carrier||!candidate)return ERROR_INVALID_STATE;
 if(read_diagnostic>1U||!runtime||runtime->read_diagnostic!=read_diagnostic||(read_diagnostic&&(!c->transport.resource_scope||c->transport.resource_scope->read_diagnostic!=1U||!c->transport.resource_scope->p1_handles)))return ERROR_ACCESS_DENIED;
 now=GetTickCount64();until=((ULONGLONG)carrier->until[1]<<32)|carrier->until[0];
 if(until<=now||until-now>240000U)return ERROR_ACCESS_DENIED;
 {ULONGLONG global=((ULONGLONG)carrier->global_until[1]<<32)|carrier->global_until[0];
  if(global<=now||global-now>60000U||global>until)return ERROR_ACCESS_DENIED;}
 memset(candidate,0,sizeof(*candidate));candidate->read_diagnostic=read_diagnostic;candidate->purpose=read_diagnostic?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;
 candidate->controller_pid=carrier->controller_pid;memcpy(candidate->controller_creation,carrier->controller_creation,sizeof(FILETIME));
 candidate->valid_from[0]=(DWORD)now;candidate->valid_from[1]=(DWORD)(now>>32);memcpy(candidate->valid_until,carrier->until,8);
 memcpy(candidate->attempt,carrier->attempt,32);memcpy(candidate->policy,carrier->policy,32);memcpy(candidate->target,carrier->target,32);
 memcpy(candidate->request,carrier->request,32);memcpy(candidate->decision,carrier->decision,32);memcpy(candidate->image,carrier->image,32);
 candidate->runtime_bytes=carrier->runtime_sid_bytes;memcpy(candidate->runtime,carrier->runtime_sid,68);
 return scqb_open_mode(c,owner,carrier,candidate,runtime,profile,receiver,dispatch,stop,1);
}
static DWORD scqb_open_query(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,const PS_SCQ_CARRIER *carrier,PAPACC_SCQ_OWNER_DECISION *candidate,PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,PAPACC_SCQ_PROFILE_DATA *profile,PAPACC_PERSONAL_SCQ_RECEIVER *receiver,PAPACC_PERSONAL_SCQ_DISPATCH *dispatch,volatile LONG *stop)
{return scqb_open_query_variant(c,owner,carrier,candidate,runtime,profile,receiver,dispatch,stop,0U);}
static DWORD scqb_read(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;DWORD e,n=0;BOOL ok;
 e=scqb_current(c,owner,stop);if(e)return e;t=&c->transport;
 if(t->pending)return scqt_poll(c,owner);
 if(!t->operation){t->operation=65U;t->total=scqt_bootstrap_size(t);}
 if(t->operation!=65U||t->offset>t->total)return scqt_fail(c,ERROR_INVALID_STATE);
 if(t->offset==t->total)return 0;
 if(!ResetEvent(t->event))return scqt_fail(c,GetLastError());
 memset(&t->ov,0,sizeof(t->ov));t->ov.hEvent=t->event;t->pending=1U;
 e=scqt_resource_effect(c,owner,stop,0);if(e){t->pending=0;return scqt_fail(c,e);}
 ok=ReadFile(t->pipe,c->buffer+t->offset,t->total-t->offset,&n,&t->ov);
 if(!ok){e=GetLastError();if(e==ERROR_IO_PENDING)return e;t->pending=0;return scqt_fail(c,e);}
 t->pending=0;return scqt_io_result(c,n);
}
static int scqb_read_access(HANDLE h)
{
 typedef NTSTATUS (NTAPI *QUERY)(HANDLE,OBJECT_INFORMATION_CLASS,PVOID,ULONG,PULONG);
 PUBLIC_OBJECT_BASIC_INFORMATION info;HMODULE module;FARPROC address;QUERY query;ULONG n=0;NTSTATUS status;
 module=GetModuleHandleW(L"ntdll.dll");if(!module)return 0;address=GetProcAddress(module,"NtQueryObject");
 if(!address||sizeof(address)!=sizeof(query))return 0;memcpy(&query,&address,sizeof(query));memset(&info,0,sizeof(info));
 status=query(h,ObjectBasicInformation,&info,sizeof(info),&n);
 return status>=0&&n==sizeof(info)&&info.GrantedAccess==PAPACC_SCQ_FILE_READ_RIGHTS;
}
/* Exact type and granted rights of a destination-local query candidate.
 * NtQueryObject returns metadata, never an administrative approval. */
static int scqb_query_access(HANDLE h,DWORD rights,const WCHAR *type)
{
 typedef NTSTATUS (NTAPI *QUERY)(HANDLE,OBJECT_INFORMATION_CLASS,PVOID,ULONG,PULONG);
 PUBLIC_OBJECT_BASIC_INFORMATION info;union {UNICODE_STRING align;BYTE bytes[512];} object;
 HMODULE module;FARPROC address;QUERY query;ULONG n=0;UNICODE_STRING *name=&object.align;ULONG_PTR begin,end,v;
 module=GetModuleHandleW(L"ntdll.dll");if(!module)return 0;address=GetProcAddress(module,"NtQueryObject");
 if(!address||sizeof(address)!=sizeof(query))return 0;memcpy(&query,&address,sizeof(query));memset(&info,0,sizeof(info));
 if(query(h,ObjectBasicInformation,&info,sizeof(info),&n)<0||n!=sizeof(info)||info.GrantedAccess!=rights)return 0;
 memset(&object,0,sizeof(object));n=0;
 if(query(h,(OBJECT_INFORMATION_CLASS)2,&object,sizeof(object),&n)<0||n<sizeof(UNICODE_STRING)||n>sizeof(object))return 0;
 begin=(ULONG_PTR)object.bytes;end=begin+n;v=(ULONG_PTR)name->Buffer;
 if(!name->Buffer||name->Length!=wcslen(type)*sizeof(WCHAR)||name->MaximumLength<name->Length||v<begin+sizeof(UNICODE_STRING)||v>end||name->Length>end-v)return 0;
 return !memcmp(name->Buffer,type,name->Length);
}
static DWORD scqb_query_candidates(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,const PAPACC_SCQ_PLAN_DATA *p,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;HANDLE candidates[2];ULONGLONG v;DWORD flags,n;unsigned i,j;TOKEN_STATISTICS stats;
 if(!t->query_refs3||!t->resource_scope||!t->resource_scope->p1_handles||t->capture_controller||t->capture_token)return scqt_fail(c,ERROR_INVALID_STATE);
 for(i=0;i<2U;i++){
  v=0;for(j=0;j<8U;j++)v|=(ULONGLONG)c->buffer[PAPACC_SCQ_BOOTSTRAP_BYTES+8U*i+j]<<(8U*j);
  if(!v||(ULONGLONG)(ULONG_PTR)v!=v)return scqt_fail(c,ERROR_INVALID_HANDLE);candidates[i]=(HANDLE)(ULONG_PTR)v;
  if(candidates[i]==INVALID_HANDLE_VALUE||candidates[i]==t->pipe||candidates[i]==t->event||candidates[i]==c->runtime->process||candidates[i]==c->runtime->token)return scqt_fail(c,ERROR_INVALID_HANDLE);
  for(j=0;j<i;j++)if(candidates[i]==candidates[j])return scqt_fail(c,ERROR_INVALID_HANDLE);
  for(j=0;j<3U;j++){ULONGLONG f=0;unsigned k;for(k=0;k<8U;k++)f|=(ULONGLONG)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*j+k]<<(8U*k);if(f==v)return scqt_fail(c,ERROR_INVALID_HANDLE);}
  if(!GetHandleInformation(candidates[i],&flags)||(flags&HANDLE_FLAG_INHERIT)||!scqb_query_access(candidates[i],i?TOKEN_QUERY:(PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE),i?L"Token":L"Process"))return scqt_fail(c,ERROR_ACCESS_DENIED);
 }
 if(!scqt_process(candidates[0],&p->administrator)||!GetTokenInformation(candidates[1],TokenStatistics,&stats,sizeof(stats),&n)||n!=sizeof(stats)||
  memcmp(&stats.TokenId,p->administrator.token_id,sizeof(LUID))||memcmp(&stats.AuthenticationId,p->administrator.authentication,sizeof(LUID))||memcmp(&stats.ModifiedId,p->administrator.modified,sizeof(LUID)))return scqt_fail(c,ERROR_ACCESS_DENIED);
 /* Custody only after both typed candidates correlate to the frozen sender plan.
  * Full original files/SD/provenance and native admin role are still mandatory. */
 t->capture_controller=candidates[0];t->capture_token=candidates[1];
 {PAPACC_SCQ_OWNER_DECISION *r=(PAPACC_SCQ_OWNER_DECISION *)t->capture_registration;
  memcpy(r->controller_token,&stats.TokenId,sizeof(LUID));memcpy(r->controller_authentication,&stats.AuthenticationId,sizeof(LUID));memcpy(r->controller_modified,&stats.ModifiedId,sizeof(LUID));}
 return scqb_current(c,owner,stop);
}
/* IDs only locate candidates. Full source scope/peer and native access/object
 * checks precede original input capture; content/SD/image are still separate. */
static DWORD scqb_candidates(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_SCQ_PLAN_DATA *plan,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t;DWORD e,flags,n;unsigned i,j;ULONGLONG value;HANDLE h;BY_HANDLE_FILE_INFORMATION info;PAPACC_PERSONAL_SCQ_ORIGINAL observer;
 union {TOKEN_USER align;BYTE bytes[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];} user;
 e=scqb_current(c,owner,stop);if(e)return e;t=&c->transport;
 if(!plan||t->pending||t->operation!=65U||t->offset!=scqt_bootstrap_size(t)||t->capture_valid||memcmp(c->buffer,t->read_diagnostic?"SQK1":t->query_refs3?"SQB3":t->capture_query?"SQB2":"SQB1",4)||
  scqp_decode_fields(c->buffer+4,PAPACC_SCQ_RECORD_BYTES,plan)||(!t->capture_query&&!scqa_scope(t->capture_registration,plan))||!scqa_carrier_scope_variant(&t->capture_carrier,plan,t->read_diagnostic)||
  !scqs_runtime_snapshot(c->runtime,&plan->runtime))return scqt_fail(c,ERROR_INVALID_DATA);
 if(plan->purpose!=(t->read_diagnostic?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)||!scqp_plan_state_mode(plan,SERVICE_START_PENDING,t->read_diagnostic))return scqt_fail(c,ERROR_INVALID_DATA);
 if(t->read_diagnostic){memcpy(t->read_public,c->buffer+PAPACC_SCQ_BOOTSTRAP3_BYTES,32);if(!scqp_nonzero(t->read_public,32))return scqt_fail(c,ERROR_INVALID_DATA);}
 if(t->capture_query){value=0;for(j=0;j<8U;j++)value|=(ULONGLONG)c->buffer[PAPACC_SCQ_CLOSE_BYTES+j]<<(8U*j);if(value!=t->global_until)return scqt_fail(c,ERROR_ACCESS_DENIED);}
 if(t->query_refs3){e=scqb_query_candidates(c,owner,plan,stop);if(e)return e;}
 memset(&observer,0,sizeof(observer));observer.operator_process=t->capture_controller;observer.operator_token=t->capture_token;
 if(!GetTokenInformation(t->capture_token,TokenUser,&user,sizeof(user),&n)||n<sizeof(TOKEN_USER)||n>sizeof(user)||
  !pri_sid_inside(user.bytes,n,user.align.User.Sid)||!EqualSid(user.align.User.Sid,(PSID)plan->administrator.sid)||!scqa_operator(&observer,&plan->administrator))return scqt_fail(c,ERROR_ACCESS_DENIED);
 for(i=0;i<3U;i++){
  value=0;for(j=0;j<8U;j++)value|=(ULONGLONG)c->buffer[PAPACC_SCQ_RECEIPT_BYTES+8U*i+j]<<(8U*j);
  if(!value||(ULONGLONG)(ULONG_PTR)value!=value)return scqt_fail(c,ERROR_INVALID_HANDLE);h=(HANDLE)(ULONG_PTR)value;
  if(h==INVALID_HANDLE_VALUE||h==t->pipe||h==t->event||h==t->capture_controller||h==t->capture_token||h==c->runtime->process||h==c->runtime->token)return scqt_fail(c,ERROR_INVALID_HANDLE);
  for(j=0;j<i;j++)if(h==t->capture_refs[j])return scqt_fail(c,ERROR_INVALID_HANDLE);
  if(!GetHandleInformation(h,&flags)||(flags&HANDLE_FLAG_INHERIT)||GetFileType(h)!=FILE_TYPE_DISK||!scqb_read_access(h)||
   !GetFileInformationByHandle(h,&info)||!scqa_original_identity(&info,&plan->refs[i]))return scqt_fail(c,ERROR_ACCESS_DENIED);
  t->capture_refs[i]=h;t->capture_valid|=1U<<i;
 }
 return scqb_current(c,owner,stop); /* candidates remain UNAPPROVED */
}
/* P0-P7 receiver locator selection precedes profile preparation. The profile
 * is deliberately empty here; pre_readiness is set only by first_prepare.
 * This predicate selects acquisition, never authority or admission. */
static int scqb_existing_target_query(const PAPACC_SCQ_SESSION_DRIVER *c,const PAPACC_SCQ_PLAN_DATA *p)
{
 return c&&p&&c->role==2U&&!c->phase&&c->runtime&&c->profile&&!c->profile->phase&&
  c->runtime->closed_preparation==1U&&c->runtime->profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL&&
  p->purpose==(c->transport.read_diagnostic?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)&&c->runtime->read_diagnostic==c->transport.read_diagnostic&&c->transport.phase==3U&&
  c->transport.capture_query==1U&&c->transport.capture_valid==7U&&
  c->transport.resource_scope&&c->transport.resource_scope->p1_handles;
}
/* After the actual files/content/SD and native peer have been independently
 * verified, freeze their local QUERY scope. Still not an approval or grant. */
static DWORD scqb_bind_query(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,
 PAPACC_SCQ_OWNER_DECISION *r,const PAPACC_SCQ_PLAN_DATA *p,PAPACC_PERSONAL_SCQ_INPUTS *local_inputs,
 const PAPACC_PERSONAL_INSTALL_PATHS *paths,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD e;ULONGLONG from,until;
 if(!r||!p||!local_inputs||!paths||t->capture_query!=1U||r!=t->capture_registration||r->source||r->controller||
 t->capture_valid!=7U||!scqr_inputs_scope(local_inputs,p)||paths->read_diagnostic!=t->read_diagnostic||(t->read_diagnostic&&memcmp(paths->read_public,t->read_public,32))||!scqa_carrier_scope_variant(&t->capture_carrier,p,t->read_diagnostic))return ERROR_ACCESS_DENIED;
 e=scqb_current(c,owner,stop);if(e)return e;
 t->capture_query=3U; /* one freeze attempt; failure never rearms it */
 from=((ULONGLONG)p->valid_from[1]<<32)|p->valid_from[0];until=((ULONGLONG)p->valid_until[1]<<32)|p->valid_until[0];
 if(until<from||until-from!=240000U)return ERROR_ACCESS_DENIED;
 r->binary=paths->binary;r->parent=paths->parent;r->data=paths->data;
 memcpy(r->valid_from,p->valid_from,8);memcpy(r->valid_until,p->valid_until,8);
 memcpy(r->originals,p->refs,sizeof(r->originals));memcpy(r->administrator,p->administrator.sid,68);r->administrator_bytes=p->administrator.sid_bytes;
 memcpy(r->key,p->key,sizeof(r->key));
 if(!scqa_scope(r,p))return ERROR_ACCESS_DENIED;t->capture_query=2U;return 0;
}
static DWORD scqb_query_image(PAPACC_SCQ_SESSION_DRIVER *c,const PAPACC_SCQ_PLAN_DATA *p,PAPACC_PERSONAL_SCQ_INPUTS *local_inputs)
{
 WCHAR process_name[1024],file_name[1024];DWORD a=1024U,b;
 if(!c||!p||!local_inputs||!c->transport.capture_query||!scqr_inputs_scope(local_inputs,p)||
 p->administrator.image[0]!=local_inputs->identity[2].dwVolumeSerialNumber||p->administrator.image[1]!=local_inputs->identity[2].nFileIndexHigh||
 p->administrator.image[2]!=local_inputs->identity[2].nFileIndexLow||memcmp(p->administrator.image_digest,local_inputs->digest[2],32))return ERROR_ACCESS_DENIED;
 if(!QueryFullProcessImageNameW(c->transport.capture_controller,PROCESS_NAME_NATIVE,process_name,&a)||!a||a>=1024U)return ERROR_ACCESS_DENIED;
 b=GetFinalPathNameByHandleW(local_inputs->reader[2],file_name,1024U,FILE_NAME_NORMALIZED|VOLUME_NAME_NT);
 return b&&b<1024U&&a==b&&CompareStringOrdinal(process_name,(int)a,file_name,(int)b,FALSE)==CSTR_EQUAL?0:ERROR_ACCESS_DENIED;
}
/* Exact deadline on the original authenticated offer must equal the frozen
 * SCM candidate, not renew it. No deadline or DATA value grants authority. */
static DWORD scqb_original_deadline(const PAPACC_SCQ_SESSION_DRIVER *c,const PAPACC_SCQ_PLAN_DATA *p)
{
 ULONGLONG shared,original,received=0;unsigned j;if(!c||!p)return ERROR_INVALID_PARAMETER;
 if(!c->transport.capture_query)return 0;
 shared=((ULONGLONG)c->transport.capture_carrier.global_until[1]<<32)|c->transport.capture_carrier.global_until[0];
 original=((ULONGLONG)p->valid_until[1]<<32)|p->valid_until[0];
 for(j=0;j<8U;j++)received|=(ULONGLONG)c->buffer[PAPACC_SCQ_CLOSE_BYTES+j]<<(8U*j);
 return shared&&shared<=original&&shared==received&&shared==c->transport.global_until&&GetTickCount64()<shared?0:ERROR_ACCESS_DENIED;
}
/* Called only after inputs capture, original content/SD and image correlation
 * and scqr_first_prepare. Reuses the already-authenticated same pipe/offer. */
static DWORD scqb_promote(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,PAPACC_SCQ_RECIPIENT_ORIGIN *r,PAPACC_PERSONAL_SCQ_INPUTS *local_inputs,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_TRANSPORT *t;DWORD e;
 if(!c||!r||!local_inputs||!scqs_owned(c,owner)||c->role!=2U||c->phase!=1U||r->phase!=1U||
  scqp_decode_fields(c->profile->record,sizeof(c->profile->record),&p))return ERROR_INVALID_STATE;
 e=scqb_original_deadline(c,&p);if(e)return e;
 t=&c->transport;
 if(t->capture_query&&t->capture_query!=2U)return ERROR_ACCESS_DENIED;
 if(t->phase!=3U||t->capture_valid!=7U||t->pending||t->operation!=65U||t->offset!=scqt_bootstrap_size(t)||
  r->registration!=t->capture_registration||memcmp(c->buffer,t->read_diagnostic?"SQK1":t->query_refs3?"SQB3":t->capture_query?"SQB2":"SQB1",4)||memcmp(c->buffer+4,c->profile->record,PAPACC_SCQ_RECORD_BYTES))return ERROR_ACCESS_DENIED;
 e=scqr_current(r,owner,c->profile,local_inputs,c->runtime,c->dispatch,stop);if(e)return e;
 memmove(c->buffer,c->buffer+4,PAPACC_SCQ_RECORD_BYTES);c->bytes=PAPACC_SCQ_RECORD_BYTES;
 t->original=&r->value;t->phase=1U;t->query_only=1U;t->operation=2U;t->offset=t->total=PAPACC_SCQ_RECORD_BYTES;
 e=scqr_admit_offer(r,c,owner,local_inputs,&p.runtime,&p.administrator,p.refs,stop);if(e)return e;
 e=scqt_accept_refs(c,owner,t->capture_refs,&p.runtime,&p.administrator,p.refs,&r->value,stop);if(e)return e;
 e=scqt_adopt_delivery(c,owner,t->capture_refs,&p.runtime,&p.administrator,p.refs,&r->value,stop);if(e)return e;
 e=scqt_apply(c,owner,2U,&p.runtime,&p.administrator,p.refs,&r->value,stop);if(e)return e;
 if(t->capture_token){ULONGLONG id=(ULONGLONG)(ULONG_PTR)t->capture_token;if(!CloseHandle(t->capture_token))return scqt_fail(c,GetLastError());t->capture_token=NULL;if(t->query_refs3)t->closed_query[1]=id;}
 if(t->capture_controller){ULONGLONG id=(ULONGLONG)(ULONG_PTR)t->capture_controller;if(!CloseHandle(t->capture_controller))return scqt_fail(c,GetLastError());t->capture_controller=NULL;if(t->query_refs3)t->closed_query[0]=id;}
 return 0; /* Native receipt/close/return still required before readiness. */
}

/* SQF1 is independent of failed admission, but NOT of local pipe/runtime/G3
 * originals. Suppression preserves the frozen first error and every reference. */
static int scqf_send_scope(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD pid,flags,bytes;TOKEN_STATISTICS statistics;FILETIME creation,exit,kernel,user;
 if(!scqt_owned(c,owner)||c->role!=2U||!stop||*stop||!t->query_refs3||
 !t->resource_scope||!t->resource_scope->p1_handles||!t->capture_registration||
 !scqb_scope(&t->capture_carrier,t->capture_registration)||!t->global_until||
 GetTickCount64()>=t->global_until||t->cancelled||!c->runtime||
 c->runtime->closed_preparation!=1U||c->runtime->profile!=PAPACC_PERSONAL_RUNTIME_VIRTUAL||
 c->runtime->phase!=1U||c->runtime->first_error||!c->runtime->process||!c->runtime->token||
 GetProcessId(c->runtime->process)!=c->runtime->pid||WaitForSingleObject(c->runtime->process,0)!=WAIT_TIMEOUT||
 !GetProcessTimes(c->runtime->process,&creation,&exit,&kernel,&user)||memcmp(&creation,&c->runtime->creation,sizeof(creation))||
 !GetTokenInformation(c->runtime->token,TokenStatistics,&statistics,sizeof(statistics),&bytes)||bytes!=sizeof(statistics)||
 memcmp(&statistics.TokenId,&c->runtime->statistics.TokenId,sizeof(LUID))||
 memcmp(&statistics.AuthenticationId,&c->runtime->statistics.AuthenticationId,sizeof(LUID))||
 memcmp(&statistics.ModifiedId,&c->runtime->statistics.ModifiedId,sizeof(LUID))||!t->pipe||!t->event||
 GetFileType(t->pipe)!=FILE_TYPE_PIPE||!GetHandleInformation(t->pipe,&flags)||(flags&HANDLE_FLAG_INHERIT)||
 !GetHandleInformation(t->event,&flags)||(flags&HANDLE_FLAG_INHERIT)||
 !GetNamedPipeServerProcessId(t->pipe,&pid)||pid!=t->capture_carrier.controller_pid)return 0;
 return scqt_resource_effect(c,owner,stop,0)==0;
}
static int scqf_wait_current(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{return c&&c->transport.failure_state==2U&&c->transport.failure_pending&&c->transport.pending&&scqf_send_scope(c,owner,stop);}
static DWORD scqf_terminal(const PAPACC_SCQ_TRANSPORT *t)
{return t->failure_error==ERROR_IO_PENDING||t->failure_error==ERROR_MORE_DATA?ERROR_ACCESS_DENIED:t->failure_error;}
static DWORD scqf_send(PAPACC_SCQ_SESSION_DRIVER *c,const void *owner,volatile LONG *stop)
{
 PAPACC_SCQ_TRANSPORT *t=&c->transport;DWORD bytes=0,e;BOOL ok;
 if(!t->failure_stage)return ERROR_INVALID_STATE;
 if(t->failure_state==2U){
  ok=GetOverlappedResult(t->pipe,&t->ov,&bytes,FALSE);
  if(!ok){e=GetLastError();if(e==ERROR_IO_INCOMPLETE)return ERROR_IO_PENDING;
   t->failure_secondary=e;t->failure_state=3U;return scqf_terminal(t); /* completion UNKNOWN: keep pending/backing */}
  if(bytes!=56U)t->failure_secondary=ERROR_INVALID_DATA;
  t->failure_pending=t->pending=0;t->failure_state=3U;return scqf_terminal(t);
 }
 if(t->failure_state)return scqf_terminal(t);
 /* Consume the only send attempt even when it is suppressed; never rearm. */
 t->failure_state=1U;
 if(t->pending||!scqf_frame(t->failure_frame,56U,t->capture_carrier.attempt)||!scqf_send_scope(c,owner,stop)){
  t->failure_secondary=ERROR_ACCESS_DENIED;t->failure_state=3U;return scqf_terminal(t);
 }
 if(!ResetEvent(t->event)){t->failure_secondary=GetLastError();t->failure_state=3U;return scqf_terminal(t);}
 memset(&t->ov,0,sizeof(t->ov));t->ov.hEvent=t->event;
 /* Bracket all scope queries/reset with freshness at the actual effect. */
 e=scqt_resource_effect(c,owner,stop,0);
 if(e){t->failure_secondary=e;t->failure_state=3U;return scqf_terminal(t);}
 t->failure_pending=t->pending=1U;t->failure_state=2U;
 ok=WriteFile(t->pipe,t->failure_frame,56U,&bytes,&t->ov);
 if(!ok){e=GetLastError();if(e==ERROR_IO_PENDING)return e;
  t->failure_secondary=e;t->failure_state=3U;return scqf_terminal(t); /* conservatively retain OV/buffer */}
 if(bytes!=56U)t->failure_secondary=ERROR_INVALID_DATA;
 t->failure_pending=t->pending=0;t->failure_state=3U;return scqf_terminal(t);
}
