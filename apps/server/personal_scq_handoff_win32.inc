#include "personal_scq_handoff_win32.h"
/* Original-source view PRIVATE to the native controller. This is not an input
 * record/serialization and cannot be reconstructed from a receipt or CLI.
 * References and immutable backing belong to the existing controller owner.
 * The native acquisition/confirmation adapter must establish their origin
 * BEFORE calling this receiver. Merely filling this view never does so. */
static DWORD personal_scq_receive_fail(PAPACC_PERSONAL_SCQ_RECEIVER *r,DWORD reason)
{if(!r->first)r->first=reason;return reason;}

/* Structural/native identity checks are NOT the missing original acquisition.
 * No hash/flag can satisfy the independent origin gate at the root entry. */
static int personal_scq_original_file(HANDLE h,const BY_HANDLE_FILE_INFORMATION *expected)
{
 BY_HANDLE_FILE_INFORMATION got;
 return h&&h!=INVALID_HANDLE_VALUE&&GetFileType(h)==FILE_TYPE_DISK&&
 GetFileInformationByHandle(h,&got)&&!(got.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY))&&
 got.dwVolumeSerialNumber==expected->dwVolumeSerialNumber&&got.nFileIndexHigh==expected->nFileIndexHigh&&got.nFileIndexLow==expected->nFileIndexLow&&
 got.nFileSizeHigh==expected->nFileSizeHigh&&got.nFileSizeLow==expected->nFileSizeLow;
}
static int personal_scq_original_refs(const PAPACC_PERSONAL_SCQ_ORIGINAL *s)
{
 union {TOKEN_USER align;BYTE data[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];} u;
 TOKEN_STATISTICS t;DWORD bytes;ULONG_PTR offset;FILETIME creation,exit,kernel,user;
 if(!personal_scq_original_file(s->request,&s->request_id)||
 !personal_scq_original_file(s->decision,&s->decision_id)||!personal_scq_original_file(s->image,&s->image_id)||
 !s->operator_process||!s->operator_token||!s->operator_sid_bytes||s->operator_sid_bytes>SECURITY_MAX_SID_SIZE||
 !IsValidSid((PSID)s->operator_sid)||GetLengthSid((PSID)s->operator_sid)!=s->operator_sid_bytes||
 GetProcessId(s->operator_process)!=s->operator_pid||!GetProcessTimes(s->operator_process,&creation,&exit,&kernel,&user)||WaitForSingleObject(s->operator_process,0)!=WAIT_TIMEOUT||
 !GetTokenInformation(s->operator_token,TokenStatistics,&t,sizeof(t),&bytes)||bytes!=sizeof(t)||t.TokenType!=TokenPrimary)return 0;
 if(memcmp(&creation,&s->operator_creation,sizeof(creation))||!GetTokenInformation(s->operator_token,TokenUser,&u,sizeof(u),&bytes)||
  bytes<sizeof(TOKEN_USER)||bytes>sizeof(u)||(ULONG_PTR)u.align.User.Sid<(ULONG_PTR)&u)return 0;
 offset=(ULONG_PTR)u.align.User.Sid-(ULONG_PTR)&u;
 if(offset>bytes||bytes-offset<8U||8U+4U*((BYTE *)u.align.User.Sid)[1]>bytes-offset||
  !IsValidSid(u.align.User.Sid)||!EqualSid(u.align.User.Sid,(PSID)s->operator_sid))return 0;
 return !memcmp(&t.TokenId,&s->operator_statistics.TokenId,sizeof(LUID))&&
 !memcmp(&t.AuthenticationId,&s->operator_statistics.AuthenticationId,sizeof(LUID))&&
 !memcmp(&t.ModifiedId,&s->operator_statistics.ModifiedId,sizeof(LUID));
}
static int personal_scq_original_binding(const PAPACC_PERSONAL_SCQ_ORIGINAL *s,const PAPACC_PERSONAL_SCQ_DISPATCH *d,const PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime)
{
 return d&&runtime&&s->purpose==d->purpose&&scqd_key_binding(s->purpose,s->key)&&scqd_key_binding(d->purpose,d->name)&&
 (s->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||(runtime->closed_preparation>=1U&&runtime->closed_preparation<=2U))&&d->phase==1U&&d->thread==GetCurrentThreadId()&&
 runtime->phase==1U&&runtime->thread==GetCurrentThreadId()&&runtime->profile==PAPACC_PERSONAL_RUNTIME_VIRTUAL&&
 s->runtime_pid==runtime->pid&&!memcmp(&s->runtime_creation,&runtime->creation,sizeof(FILETIME))&&
 !memcmp(&s->runtime_token.TokenId,&runtime->statistics.TokenId,sizeof(LUID))&&
 !memcmp(&s->runtime_token.AuthenticationId,&runtime->statistics.AuthenticationId,sizeof(LUID))&&
 !memcmp(&s->runtime_token.ModifiedId,&runtime->statistics.ModifiedId,sizeof(LUID))&&
 !memcmp(s->association,d->association,32U)&&!wcscmp(s->key,d->name);
}
static DWORD personal_scq_receive_original(PAPACC_PERSONAL_SCQ_RECEIVER *r,
 PAPACC_PERSONAL_SCQ_ORIGINAL *s,const void *owner,const PAPACC_PERSONAL_SCQ_DISPATCH *d,
 const PAPACC_PERSONAL_RUNTIME_IDENTITY *runtime,volatile LONG *stop)
{
 if(!r||!owner||!d||!runtime||!stop)return ERROR_INVALID_PARAMETER;
 if(r->phase)return ERROR_INVALID_STATE;
 r->identity=r;r->thread=GetCurrentThreadId();r->owner=owner;r->phase=3;
 if(!s)return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_ABSENT);
 if(InterlockedCompareExchange(stop,0,0))return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_STOP);
 if(s->original!=s||!s->controller||s->recipient!=owner||!personal_scq_original_refs(s))return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_ORIGIN);
 if(s->purpose!=d->purpose||!scqd_key_binding(s->purpose,s->key)||!s->controller_consumed)return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_PURPOSE);
 if(!s->deadline||GetTickCount64()>=s->deadline)return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_EXPIRED);
 if(!personal_scq_original_binding(s,d,runtime))return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_IDENTITY);
 if(InterlockedCompareExchange(&s->retained,0,0)||InterlockedCompareExchange(&s->consumed,0,0)||
 InterlockedCompareExchange(&s->borrowed,1,0))return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_CONSUMED);
 r->original=s;r->dispatch=d;r->runtime=runtime;r->deadline=s->deadline;memcpy(r->association,s->association,32U);r->phase=1;
 return 0;
}
static DWORD personal_scq_receive_consume(PAPACC_PERSONAL_SCQ_RECEIVER *r,const void *owner,volatile LONG *stop)
{
 PAPACC_PERSONAL_SCQ_ORIGINAL *s;
 if(!r||r->identity!=r||r->phase!=1U||r->thread!=GetCurrentThreadId()||r->owner!=owner||!r->original||!stop)return ERROR_INVALID_STATE;
 s=(PAPACC_PERSONAL_SCQ_ORIGINAL *)r->original;
 /* Burn in the controlling original BEFORE any protected effect. Even a
  * subsequent failure keeps this original consumed, without rollback/rearm. */
 r->phase=2;
 if(InterlockedCompareExchange(&s->consumed,1,0))return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_CONSUMED);
 if(InterlockedCompareExchange(stop,0,0))return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_STOP);
 if(GetTickCount64()>=r->deadline)return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_EXPIRED);
 if(!s->controller_consumed||InterlockedCompareExchange(&s->retained,0,0)||s->original!=s||s->recipient!=owner||s->deadline!=r->deadline||memcmp(s->association,r->association,32U)||
 !personal_scq_original_binding(s,r->dispatch,r->runtime)||!personal_scq_original_refs(s))return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_ORIGIN);
 return 0;
}
/* Close the borrow, not the originals: only their original controller may
 * close its handles/backings after all consumers are terminal. UNKNOWN never
 * releases or retries. `closed` is observed by the existing close root, not a
 * caller-supplied grant. */
static DWORD personal_scq_receive_finish(PAPACC_PERSONAL_SCQ_RECEIVER *r,const void *owner,int closed)
{
 PAPACC_PERSONAL_SCQ_ORIGINAL *s;
 if(!r||r->identity!=r||r->owner!=owner||r->thread!=GetCurrentThreadId()||!r->original||r->phase==3U||r->phase==4U)return ERROR_INVALID_STATE;
 s=(PAPACC_PERSONAL_SCQ_ORIGINAL *)r->original;
 if(!closed){InterlockedExchange(&s->retained,1);InterlockedExchange(&s->consumed,1);r->phase=4;
  r->close_error=PAPACC_SCQ_RECEIVE_CLOSE_UNKNOWN;return personal_scq_receive_fail(r,PAPACC_SCQ_RECEIVE_CLOSE_UNKNOWN);}
 /* Cancelled borrows cannot rearm the original either. */
 InterlockedExchange(&s->consumed,1);InterlockedExchange(&s->borrowed,0);r->original=NULL;r->phase=3;return 0;
}
