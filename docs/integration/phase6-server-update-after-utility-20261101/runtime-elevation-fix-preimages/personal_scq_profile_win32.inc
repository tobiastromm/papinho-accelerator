#ifndef PAPACC_PERSONAL_SCQ_PROFILE_IMPLEMENTATION
#define PAPACC_PERSONAL_SCQ_PROFILE_IMPLEMENTATION
#include "personal_scq_profile_win32.h"
#include <string.h>
/* Strict projection: unknown schemas/types/rights/extra bytes are rejected,
 * not printed or silently removed. Native origin must be checked separately. */
static int scqp_nonzero(const BYTE *p,unsigned n)
{unsigned i,v=0;for(i=0;i<n;i++)v|=p[i];return v!=0;}
static int scqp_key(const char s[112])
{
 static const char prefix[]="PapinhoAccelerator.CA.";unsigned i,j,k=sizeof(prefix)-1U,z;
 if(memcmp(s,prefix,k)||s[k+32]!='.'||s[k+65])return 0;
 for(j=0;j<2;j++){z=0;for(i=k+j*33;i<k+j*33+32;i++){
  if(!((s[i]>='0'&&s[i]<='9')||(s[i]>='a'&&s[i]<='f')))return 0;z|=s[i]!='0';}if(!z)return 0;}
 for(i=k+66;i<112;i++)if(s[i])return 0;
 return scqp_nonzero((const BYTE *)s+k,32)&&scqp_nonzero((const BYTE *)s+k+33,32);
}
static int scqp_overlap(const void *a,size_t n,const void *b,size_t m)
{ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;return x>=y?x-y<m:y-x<n;}
static int scqp_peer_state(const PAPACC_SCQ_PEER_DATA *p,int runtime,DWORD state)
{
 unsigned i;PSID sid=(PSID)p->sid;SID_IDENTIFIER_AUTHORITY nt=SECURITY_NT_AUTHORITY;
 if(!p->image[0]||!(p->image[1]|p->image[2])||!scqp_nonzero(p->image_digest,32)||!p->pid||!(p->creation[0]|p->creation[1])||!(p->token_id[0]|p->token_id[1])||
 p->sid_bytes<8U||p->sid_bytes>68U||p->sid[1]>SID_MAX_SUB_AUTHORITIES||8U+4U*p->sid[1]!=p->sid_bytes||
 !IsValidSid(sid)||GetLengthSid(sid)!=p->sid_bytes||p->token_type!=TokenPrimary||p->query_error||p->wait_status!=WAIT_TIMEOUT||p->exit_code!=STILL_ACTIVE||p->groups!=p->enumerated||p->restricted)return 0;
 for(i=p->sid_bytes;i<68;i++)if(p->sid[i])return 0;
 if(runtime){
  if(memcmp(GetSidIdentifierAuthority(sid),&nt,sizeof(nt))||*GetSidSubAuthorityCount(sid)!=6U||*GetSidSubAuthority(sid,0)!=80U||
   p->elevated||p->elevation_type!=TokenElevationTypeDefault||p->matches>1U||p->session||p->logon!=1U||p->administrator_attributes||
   p->service_type!=SERVICE_WIN32_OWN_PROCESS||p->service_state!=state||p->service_sid_type>1U)return 0;
  if(!p->matches)return !p->attributes;
 }else if(!p->session||p->logon!=2U||p->administrator_attributes!=p->attributes||p->service_type||p->service_state||p->service_sid_type||!p->elevated||(p->elevation_type!=TokenElevationTypeFull&&p->elevation_type!=TokenElevationTypeDefault)||!p->groups||p->matches!=1U)return 0;
 return (p->attributes&SE_GROUP_ENABLED)&&!(p->attributes&SE_GROUP_USE_FOR_DENY_ONLY);
}
static int scqp_peer(const PAPACC_SCQ_PEER_DATA *p,int runtime)
{return scqp_peer_state(p,runtime,SERVICE_RUNNING);}
static int scqp_static(const PAPACC_SCQ_PLAN_DATA *p)
{
 unsigned i,j;
 if(!p||p->version!=1U||p->profile!=PAPACC_SCQ_PROFILE||(p->purpose!=PAPACC_SCQ_PURPOSE_READ&&p->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY)||p->schema!=PAPACC_SCQ_NON_SECRET_SCHEMA||p->service!=PAPACC_SCQ_SERVICE_PAPINHO||
 !scqp_nonzero(p->attempt,32)||!scqp_nonzero(p->policy,32)||!scqp_nonzero(p->request,32)||!scqp_nonzero(p->decision,32)||!scqp_nonzero(p->target,32)||
 (p->purpose==PAPACC_SCQ_PURPOSE_READ?!scqp_key(p->key):scqp_nonzero((const BYTE *)p->key,sizeof(p->key)))||!scqp_peer(&p->administrator,0))return 0;
 for(i=0;i<3;i++){
  const PAPACC_SCQ_REF_DATA *r=&p->refs[i];
  if(r->role!=i+1U||r->rights!=PAPACC_SCQ_FILE_READ_RIGHTS||!r->volume||!(r->high|r->low)||!(r->size_low|r->size_high)||r->links!=1U||
   (r->attributes&~(FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_READONLY))||!scqp_nonzero(r->digest,32)||!scqp_nonzero(r->descriptor,32))return 0;
  for(j=0;j<i;j++)if(r->volume==p->refs[j].volume&&r->high==p->refs[j].high&&r->low==p->refs[j].low)return 0;
 }
 return 1;
}
static int scqp_window(const PAPACC_SCQ_PLAN_DATA *p)
{ULONGLONG from=((ULONGLONG)p->valid_from[1]<<32)|p->valid_from[0],until=((ULONGLONG)p->valid_until[1]<<32)|p->valid_until[0],now=GetTickCount64();return until>from&&now>=from&&now<until;}
static int scqp_plan_state(const PAPACC_SCQ_PLAN_DATA *p,DWORD state)
{return scqp_static(p)&&(p->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||state==SERVICE_START_PENDING)&&scqp_peer_state(&p->runtime,1,state)&&p->runtime.image[0]==p->refs[2].volume&&p->runtime.image[1]==p->refs[2].high&&p->runtime.image[2]==p->refs[2].low&&!memcmp(p->runtime.image_digest,p->refs[2].digest,32)&&p->administrator.pid!=p->runtime.pid&&!EqualSid((PSID)p->administrator.sid,(PSID)p->runtime.sid);}
static int scqp_plan(const PAPACC_SCQ_PLAN_DATA *p)
{return scqp_plan_state(p,SERVICE_RUNNING);}
static int scqp_unbound(const PAPACC_SCQ_PEER_DATA *p)
{
 unsigned i;SID_IDENTIFIER_AUTHORITY nt=SECURITY_NT_AUTHORITY;PSID sid=(PSID)p->sid;
 if(p->pid||p->creation[0]||p->creation[1]||p->token_id[0]||p->token_id[1]||p->authentication[0]||p->authentication[1]||
  p->modified[0]||p->modified[1]||p->groups||p->enumerated||p->restricted||p->elevated||p->elevation_type||p->matches||p->attributes||p->token_type||p->query_error||p->wait_status||p->exit_code||p->session||p->logon||p->administrator_attributes||p->service_type||p->service_state||p->service_sid_type||p->image[0]||p->image[1]||p->image[2]||scqp_nonzero(p->image_digest,32)||
  p->sid_bytes<8||p->sid_bytes>68||p->sid[1]>SID_MAX_SUB_AUTHORITIES||8U+4U*p->sid[1]!=p->sid_bytes||!IsValidSid(sid)||GetLengthSid(sid)!=p->sid_bytes)return 0;
 for(i=p->sid_bytes;i<68;i++)if(p->sid[i])return 0;
 return !memcmp(GetSidIdentifierAuthority(sid),&nt,sizeof(nt))&&*GetSidSubAuthorityCount(sid)==6U&&*GetSidSubAuthority(sid,0)==80U;
}
static void scqp_put(BYTE **p,DWORD x)
{unsigned i;for(i=0;i<4;i++){*(*p)++=(BYTE)x;x>>=8;}}
static DWORD scqp_get(const BYTE **p)
{unsigned i;DWORD x=0;for(i=0;i<4;i++)x|=(DWORD)*(*p)++<<(8U*i);return x;}
#define SCQP_PEER_FIELDS(X) X(pid) X(creation[0]) X(creation[1]) X(token_id[0]) X(token_id[1]) X(authentication[0]) X(authentication[1]) X(modified[0]) X(modified[1]) X(groups) X(enumerated) X(restricted) X(elevated) X(elevation_type) X(matches) X(attributes) X(sid_bytes) X(token_type) X(query_error) X(wait_status) X(exit_code) X(session) X(logon) X(administrator_attributes) X(service_type) X(service_state) X(service_sid_type) X(image[0]) X(image[1]) X(image[2])
#define SCQP_REF_FIELDS(X) X(role) X(rights) X(volume) X(high) X(low) X(size_low) X(size_high) X(attributes) X(links)
static DWORD scqp_encode_fields(const PAPACC_SCQ_PLAN_DATA *v,BYTE *out,DWORD n)
{
 BYTE *p=out;unsigned i;
 if(!v||!out||n!=PAPACC_SCQ_RECORD_BYTES||scqp_overlap(out,n,v,sizeof(*v)))return ERROR_INVALID_DATA;
 /* Explicit endian encoding; native structs/padding/pointers never exported. */
 scqp_put(&p,v->version);scqp_put(&p,v->profile);scqp_put(&p,v->purpose);scqp_put(&p,v->schema);scqp_put(&p,v->service);
 scqp_put(&p,v->valid_from[0]);scqp_put(&p,v->valid_from[1]);scqp_put(&p,v->valid_until[0]);scqp_put(&p,v->valid_until[1]);
#define SCQP_COPY(f) memcpy(p,v->f,sizeof(v->f));p+=sizeof(v->f)
 SCQP_COPY(attempt);SCQP_COPY(policy);SCQP_COPY(request);SCQP_COPY(decision);SCQP_COPY(target);SCQP_COPY(key);
#undef SCQP_COPY
 for(i=0;i<2;i++){const PAPACC_SCQ_PEER_DATA *q=i?&v->runtime:&v->administrator;
#define SCQP_PUT_PEER(f) scqp_put(&p,q->f);
 SCQP_PEER_FIELDS(SCQP_PUT_PEER)
#undef SCQP_PUT_PEER
 memcpy(p,q->sid,68);p+=68;memcpy(p,q->image_digest,32);p+=32;
 }
 for(i=0;i<3;i++){const PAPACC_SCQ_REF_DATA *q=&v->refs[i];
#define SCQP_PUT_REF(f) scqp_put(&p,q->f);
 SCQP_REF_FIELDS(SCQP_PUT_REF)
#undef SCQP_PUT_REF
 memcpy(p,q->digest,32);p+=32;memcpy(p,q->descriptor,32);p+=32;
 }
 return p==out+n?0:ERROR_INVALID_DATA;
}
static DWORD scqp_decode_fields(const BYTE *in,DWORD n,PAPACC_SCQ_PLAN_DATA *v)
{
 const BYTE *p=in;unsigned i;
 if(!in||!v||n!=PAPACC_SCQ_RECORD_BYTES||scqp_overlap(in,n,v,sizeof(*v)))return ERROR_INVALID_DATA;
 memset(v,0,sizeof(*v));v->version=scqp_get(&p);v->profile=scqp_get(&p);v->purpose=scqp_get(&p);v->schema=scqp_get(&p);v->service=scqp_get(&p);
 v->valid_from[0]=scqp_get(&p);v->valid_from[1]=scqp_get(&p);v->valid_until[0]=scqp_get(&p);v->valid_until[1]=scqp_get(&p);
#define SCQP_COPY(f) memcpy(v->f,p,sizeof(v->f));p+=sizeof(v->f)
 SCQP_COPY(attempt);SCQP_COPY(policy);SCQP_COPY(request);SCQP_COPY(decision);SCQP_COPY(target);SCQP_COPY(key);
#undef SCQP_COPY
 for(i=0;i<2;i++){PAPACC_SCQ_PEER_DATA *q=i?&v->runtime:&v->administrator;
#define SCQP_GET_PEER(f) q->f=scqp_get(&p);
 SCQP_PEER_FIELDS(SCQP_GET_PEER)
#undef SCQP_GET_PEER
 memcpy(q->sid,p,68);p+=68;memcpy(q->image_digest,p,32);p+=32;
 }
 for(i=0;i<3;i++){PAPACC_SCQ_REF_DATA *q=&v->refs[i];
#define SCQP_GET_REF(f) q->f=scqp_get(&p);
 SCQP_REF_FIELDS(SCQP_GET_REF)
#undef SCQP_GET_REF
 memcpy(q->digest,p,32);p+=32;memcpy(q->descriptor,p,32);p+=32;
 }
 return p==in+n?0:ERROR_INVALID_DATA;
}
DWORD papacc_scq_plan_encode(const PAPACC_SCQ_PLAN_DATA *v,BYTE *out,DWORD n)
{return scqp_plan(v)?scqp_encode_fields(v,out,n):ERROR_INVALID_DATA;}
DWORD papacc_scq_plan_decode(const BYTE *in,DWORD n,PAPACC_SCQ_PLAN_DATA *v)
{DWORD e=scqp_decode_fields(in,n,v);return e?e:scqp_plan(v)?0:ERROR_INVALID_DATA;}
#undef SCQP_PEER_FIELDS
#undef SCQP_REF_FIELDS
static DWORD scqp_fail(PAPACC_SCQ_PROFILE_DATA *c,DWORD e)
{if(!c->first)c->first=e;c->phase=5;return e;}
static int scqp_owned(const PAPACC_SCQ_PROFILE_DATA *c,const void *owner)
{return c&&c->self==c&&c->owner==owner&&c->thread==GetCurrentThreadId()&&c->pre_readiness<=1U;}
static int scqp_peers(const PAPACC_SCQ_PLAN_DATA *v,DWORD role,const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3])
{
 return local&&remote&&refs&&!memcmp(local,role==1U?&v->administrator:&v->runtime,sizeof(*local))&&
 !memcmp(remote,role==1U?&v->runtime:&v->administrator,sizeof(*remote))&&!memcmp(refs,v->refs,sizeof(v->refs));
}
static DWORD scqp_prepare(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,DWORD role,const PAPACC_SCQ_PLAN_DATA *v,const PAPACC_SCQ_PEER_DATA *local,unsigned closed)
{
 DWORD e;
 if(!c||!owner||c->phase||!local||!v||(role!=1U&&role!=2U)||scqp_overlap(c,sizeof(*c),v,sizeof(*v))||scqp_overlap(c,sizeof(*c),local,sizeof(*local)))return ERROR_INVALID_STATE;
 c->self=c;c->owner=owner;c->role=role;c->thread=GetCurrentThreadId();c->phase=1;
 c->pre_readiness=closed;
 if(v->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&!closed)return scqp_fail(c,ERROR_ACCESS_DENIED);
 c->waiting_runtime=role==1U&&scqp_unbound(&v->runtime);
 if(!scqp_window(v))return scqp_fail(c,ERROR_TIMEOUT);
 if(c->waiting_runtime){if(!scqp_static(v))return scqp_fail(c,ERROR_INVALID_DATA);e=scqp_encode_fields(v,c->record,sizeof(c->record));}
 else e=scqp_plan_state(v,closed?SERVICE_START_PENDING:SERVICE_RUNNING)?scqp_encode_fields(v,c->record,sizeof(c->record)):ERROR_INVALID_DATA;if(e)return scqp_fail(c,e);
 if(memcmp(local,role==1U?&v->administrator:&v->runtime,sizeof(*local)))return scqp_fail(c,ERROR_ACCESS_DENIED);
 return 0;
}
DWORD papacc_scq_profile_prepare_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,DWORD role,const PAPACC_SCQ_PLAN_DATA *v,const PAPACC_SCQ_PEER_DATA *local)
{return scqp_prepare(c,owner,role,v,local,0U);}
DWORD papacc_scq_profile_prepare_closed_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,DWORD role,const PAPACC_SCQ_PLAN_DATA *v,const PAPACC_SCQ_PEER_DATA *local)
{return v&&v->purpose==PAPACC_SCQ_PURPOSE_READ?scqp_prepare(c,owner,role,v,local,1U):ERROR_INVALID_DATA;}
DWORD papacc_scq_profile_prepare_transport_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,DWORD role,const PAPACC_SCQ_PLAN_DATA *v,const PAPACC_SCQ_PEER_DATA *local)
{return v&&v->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY?scqp_prepare(c,owner,role,v,local,1U):ERROR_INVALID_DATA;}
static DWORD scqp_current(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;
 if(!scqp_owned(c,owner)||!stop||(c->phase!=1U&&c->phase!=2U))return ERROR_INVALID_STATE;
 if(InterlockedCompareExchange(stop,0,0))return scqp_fail(c,ERROR_CANCELLED);
 if(scqp_decode_fields(c->record,sizeof(c->record),&p))return scqp_fail(c,ERROR_INVALID_DATA);
 if(p.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&!c->pre_readiness)return scqp_fail(c,ERROR_ACCESS_DENIED);
 if(!scqp_window(&p))return scqp_fail(c,ERROR_TIMEOUT);
 if(c->waiting_runtime){
  if(c->role!=1U||!scqp_static(&p)||!scqp_unbound(&p.runtime)||!local||remote||!refs||memcmp(local,&p.administrator,sizeof(*local))||memcmp(refs,p.refs,sizeof(p.refs)))return scqp_fail(c,ERROR_ACCESS_DENIED);
 }else if(!scqp_plan_state(&p,c->pre_readiness?SERVICE_START_PENDING:SERVICE_RUNNING)||!scqp_peers(&p,c->role,local,remote,refs))return scqp_fail(c,ERROR_ACCESS_DENIED);
 return 0;
}
DWORD papacc_scq_profile_take_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 DWORD e;
 if(!scqp_owned(c,owner)||c->role!=1U||c->phase!=1U)return ERROR_INVALID_STATE;
 e=scqp_current(c,owner,local,remote,refs,stop);if(e)return e;
 if(InterlockedCompareExchange(&c->taken,1,0))return scqp_fail(c,ERROR_ALREADY_EXISTS);
 c->phase=2;return 0; /* DATA ordering, never durable operational consumption */
}
DWORD papacc_scq_profile_bind_runtime_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,const PAPACC_SCQ_PEER_DATA *administrator,const PAPACC_SCQ_PEER_DATA *runtime,const PAPACC_SCQ_REF_DATA refs[3],volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!scqp_owned(c,owner)||c->role!=1U||c->phase!=2U||!c->taken||!c->waiting_runtime||c->used||!runtime)return ERROR_INVALID_STATE;
 e=scqp_current(c,owner,administrator,NULL,refs,stop);if(e)return e;
 if(scqp_decode_fields(c->record,sizeof(c->record),&p)||!scqp_peer_state(runtime,1,c->pre_readiness?SERVICE_START_PENDING:SERVICE_RUNNING)||p.runtime.sid_bytes!=runtime->sid_bytes||memcmp(p.runtime.sid,runtime->sid,68))return scqp_fail(c,ERROR_ACCESS_DENIED);
 p.runtime=*runtime;if(!scqp_plan_state(&p,c->pre_readiness?SERVICE_START_PENDING:SERVICE_RUNNING))return scqp_fail(c,ERROR_ACCESS_DENIED);
 e=scqp_encode_fields(&p,c->record,sizeof(c->record));if(e)return scqp_fail(c,e);
 c->waiting_runtime=0;return 0;
}
DWORD papacc_scq_profile_offer_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],BYTE *out,DWORD n,volatile LONG *stop)
{
 DWORD e;
 if(!scqp_owned(c,owner)||c->role!=1U||c->phase!=2U||!c->taken||c->waiting_runtime||!out||n!=sizeof(c->record)||scqp_overlap(out,n,c,sizeof(*c)))return ERROR_INVALID_STATE;
 e=scqp_current(c,owner,local,remote,refs,stop);if(e)return e;
 if(InterlockedCompareExchange(&c->used,1,0))return scqp_fail(c,ERROR_ALREADY_EXISTS);
 memcpy(out,c->record,n);return 0;
}
DWORD papacc_scq_profile_receive_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],const BYTE *in,DWORD n,volatile LONG *stop)
{
 PAPACC_SCQ_PLAN_DATA p;DWORD e;
 if(!scqp_owned(c,owner)||c->role!=2U||c->phase!=1U||!in)return ERROR_INVALID_STATE;
 e=scqp_current(c,owner,local,remote,refs,stop);if(e)return e;
 if(n!=sizeof(c->record)||scqp_decode_fields(in,n,&p)||!scqp_plan_state(&p,c->pre_readiness?SERVICE_START_PENDING:SERVICE_RUNNING)||memcmp(in,c->record,n))return scqp_fail(c,ERROR_INVALID_DATA);
 if(InterlockedCompareExchange(&c->used,1,0))return scqp_fail(c,ERROR_ALREADY_EXISTS);
 c->phase=2;return 0; /* validated projection DATA, not proof of a live peer */
}
DWORD papacc_scq_profile_receipt_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,BYTE *out,DWORD n)
{
 if(!scqp_owned(c,owner)||c->role!=2U||c->phase!=2U||!c->used||!out||n!=PAPACC_SCQ_RECEIPT_BYTES||scqp_overlap(out,n,c,sizeof(*c)))return ERROR_INVALID_STATE;
 if(InterlockedCompareExchange(&c->ack,1,0))return scqp_fail(c,ERROR_ALREADY_EXISTS);
 memcpy(out,"SQR1",4);memcpy(out+4,c->record,sizeof(c->record));return 0;
}
DWORD papacc_scq_profile_ack_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,const PAPACC_SCQ_PEER_DATA *local,const PAPACC_SCQ_PEER_DATA *remote,const PAPACC_SCQ_REF_DATA refs[3],const BYTE *in,DWORD n,volatile LONG *stop)
{
 DWORD e;
 if(!scqp_owned(c,owner)||c->role!=1U||c->phase!=2U||!c->used||!in)return ERROR_INVALID_STATE;
 e=scqp_current(c,owner,local,remote,refs,stop);if(e)return e;
 if(n!=PAPACC_SCQ_RECEIPT_BYTES||memcmp(in,"SQR1",4)||memcmp(in+4,c->record,sizeof(c->record)))return scqp_fail(c,ERROR_INVALID_DATA);
 if(InterlockedCompareExchange(&c->ack,1,0))return scqp_fail(c,ERROR_ALREADY_EXISTS);
 return 0; /* DATA receipt only, not authenticated operational acknowledgement */
}
DWORD papacc_scq_profile_close_data(PAPACC_SCQ_PROFILE_DATA *c,const void *owner,int quiescent)
{
 if(!c||!c->phase)return 0;
 if(!scqp_owned(c,owner)||c->phase==4U)return ERROR_INVALID_STATE;
 if(c->phase==3U)return 0;
 if(!quiescent||(c->used&&!c->ack)){if(!c->first)c->first=ERROR_INVALID_HANDLE;c->phase=4;return ERROR_INVALID_HANDLE;}
 c->phase=3;return 0;
}
#endif /* PAPACC_PERSONAL_SCQ_PROFILE_IMPLEMENTATION */
