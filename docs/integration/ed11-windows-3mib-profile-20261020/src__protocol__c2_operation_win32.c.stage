#include "../core/ed11_process_profile.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "c2_operation_win32.h"
#include <string.h>
#include <stdio.h>
#ifdef PAPACC_ENABLE_CANONICAL_C8_ROOT
extern PAPACC_RESULT papacc_server_launcher_c2_deadline(const PAPACC_C2_SESSION *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U64 *);
extern PAPACC_RESULT papacc_server_launcher_c2_flow_limit(PAPACC_C2_OPERATION_MANAGER *,PAPACC_C2_OPERATION *,PAPACC_SIZE);
#endif
#ifdef PAPACC_ENABLE_C2_DIAGNOSTIC
static void c7_flow_metric(const PAPACC_C2_OPERATION_MANAGER*m,const PAPACC_C2_OPERATION*o,unsigned i,const char*event){char v[8];const PAPACC_C2_FLOW*f=&o->flow[i];static unsigned long long seq;
 if(GetEnvironmentVariableA("PAPACC_C7_METRICS",v,sizeof(v))!=1U||v[0]!='1')return;
 fprintf(stderr,"C7_QUEUE pid=%lu thread=%lu sequence=%llu session=%llu principal=%llu operation=%llu generation=%llu flow=%u event=%s occupancy=%llu paused=%d admitted=%llu accepted=%llu submitted=%llu escaped=%llu high=49152 low=24576 cap=65536\n",GetCurrentProcessId(),GetCurrentThreadId(),++seq,(unsigned long long)o->session->incarnation,(unsigned long long)o->session->principal,(unsigned long long)o->id,(unsigned long long)o->generation,i,event,(unsigned long long)f->size,f->paused,(unsigned long long)f->admitted,(unsigned long long)f->accepted,(unsigned long long)f->submitted,(unsigned long long)f->escaped);(void)m;
}

#else
static void c7_flow_metric(const PAPACC_C2_OPERATION_MANAGER *m,const PAPACC_C2_OPERATION *o,unsigned i,const char *event)
{(void)m;(void)o;(void)i;(void)event;}
#endif
static PAPACC_U64 read64(const PAPACC_U8*p){PAPACC_U64 v=0;unsigned i;for(i=0;i<8U;++i)v=(v<<8)|p[i];return v;}
static void put64(PAPACC_U8*p,PAPACC_U64 v){unsigned i=8;while(i--!=0U){p[i]=(PAPACC_U8)v;v>>=8;}}
static int manager_ok(const PAPACC_C2_OPERATION_MANAGER*m){return m!=NULL&&m->initialized&&m->owner==(PAPACC_U32)GetCurrentThreadId();}
static int owned(const PAPACC_C2_OPERATION_MANAGER*m,const PAPACC_C2_OPERATION*o){unsigned i;if(!manager_ok(m)||o==NULL)return 0;for(i=0;i<2;++i)if(o==&m->operations[i])return o->state!=PAPACC_C2_OP_EMPTY;return 0;}
static PAPACC_C2_OPERATION_SESSION *session_record(PAPACC_C2_OPERATION_MANAGER*m,const PAPACC_C2_SESSION*s){unsigned i;for(i=0;i<4;++i)if(m->sessions[i].session==s)return &m->sessions[i];return NULL;}
static int session_ok(const PAPACC_C2_OPERATION_MANAGER*m,const PAPACC_C2_SESSION*s){return s!=NULL&&papacc_c2_session_authorized(s,m->owner,s->incarnation,s->principal,s->policy_epoch);}
static int live(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o){PAPACC_BOOL permit=PAPACC_FALSE;PAPACC_RESULT result;
 if(!owned(m,o)||m->stopping||o->revoked||o->state==PAPACC_C2_OP_RELEASED)return 0;
 if(o->epoch!=o->session->policy_epoch||!session_ok(m,o->session)){(void)papacc_c2_operation_revoke(m,o,2U);return 0;}
 #ifdef PAPACC_ENABLE_CANONICAL_C8_ROOT
 if(!o->policy_deadline_ms||(PAPACC_U64)GetTickCount64()>=o->policy_deadline_ms){(void)papacc_c2_operation_revoke(m,o,3U);return 0;}
#endif
 if((PAPACC_U64)GetTickCount64()>=o->deadline_ms){(void)papacc_c2_operation_revoke(m,o,3U);return 0;}
 result=m->policy(m->context,o->session,o->open,o->open_length,&permit);if(result!=0||permit!=PAPACC_TRUE){(void)papacc_c2_operation_revoke(m,o,2U);return 0;}return 1;
}
PAPACC_BOOL papacc_c2_operation_authorized(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o){return live(m,o)?PAPACC_TRUE:PAPACC_FALSE;}
static PAPACC_C2_OPERATION *find(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_SESSION*s,PAPACC_U64 id,PAPACC_U64 gen){unsigned i;for(i=0;i<2;++i){PAPACC_C2_OPERATION*o=&m->operations[i];if(o->state!=PAPACC_C2_OP_EMPTY&&o->session==s&&o->id==id&&o->generation==gen)return o;}return NULL;}
PAPACC_RESULT papacc_c2_operation_manager_init(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_U32 owner,PAPACC_C2_OPEN_POLICY policy,PAPACC_C2_CAPACITY_GATE capacity,void*context){
 if(m==NULL||policy==NULL||capacity==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(m->initialized||owner!=(PAPACC_U32)GetCurrentThreadId()||sizeof(*m)>PAPACC_ED11_PROCESS_LIMIT)return PAPACC_RESULT_INVALID_STATE;
 memset(m,0,sizeof(*m));m->owner=owner;m->next_operation=1;m->policy=policy;m->capacity=capacity;m->context=context;m->initialized=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_session_attach(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_SESSION*s,const PAPACC_PRINCIPAL*p){unsigned i;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
 if(s==NULL||p==NULL||!p->valid||memcmp(s->authenticated_principal,p->value,16U)!=0)return PAPACC_RESULT_INVALID_STATE;
#endif
 if(!manager_ok(m)||!session_ok(m,s)||m->stopping||p==NULL||!p->valid||s->assurance!=1U||session_record(m,s)!=NULL)return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<4;++i)if(m->sessions[i].session==NULL){m->sessions[i].session=s;m->sessions[i].principal=*p;m->sessions[i].next_generation=1;return PAPACC_RESULT_OK;}return PAPACC_RESULT_LIMIT_EXCEEDED;
}
PAPACC_RESULT papacc_c2_operation_open(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_SESSION*s,const PAPACC_U8*p,PAPACC_SIZE n,PAPACC_U8 result[108],PAPACC_C2_OPERATION**out){
 PAPACC_C2_OPERATION_SESSION*sr;PAPACC_C2_OPERATION*o=NULL;PAPACC_C2_TOKENS live_tokens[2];PAPACC_U8 identity[32];PAPACC_BOOL permit=PAPACC_FALSE;PAPACC_RESULT r;PAPACC_U64 request,now;unsigned i,count=0;PAPACC_U16 status=0;
 if(out!=NULL)*out=NULL;if(result==NULL||out==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!manager_ok(m)||!session_ok(m,s)||m->stopping||(sr=session_record(m,s))==NULL)return PAPACC_RESULT_INVALID_STATE;
 r=papacc_c2_validate_payload(16,p,n,PAPACC_C2_CONTROL,PAPACC_C2_BROWSER);if(r!=0)return r;
 request=read64(p);if(request<=sr->highest_request)return PAPACC_RESULT_PROTOCOL_ERROR;sr->highest_request=request;
 memset(result,0,108);memcpy(result,p,8);memcpy(result+32,p+8,8);result[42]=s->assurance;
 r=m->policy(m->context,s,p,n,&permit);if(r!=0||!permit)status=1;
 for(i=0;i<2;++i){if(m->operations[i].state==PAPACC_C2_OP_EMPTY)o=&m->operations[i];else if(m->operations[i].session==s)status=3;}
 if(o==NULL||m->next_operation==0||sr->next_generation==0)status=3;
 if(status==0){permit=PAPACC_FALSE;r=m->capacity(m->context,sizeof(*m),&permit);if(r!=0||!permit)status=3;}
 now=(PAPACC_U64)GetTickCount64();if(now>(PAPACC_U64)-1-5000U)status=4;
 if(status==0){
  memset(o,0,sizeof(*o));o->state=PAPACC_C2_OP_RESERVED;o->session=s;o->request=request;o->id=m->next_operation++;o->generation=sr->next_generation++;o->epoch=s->policy_epoch;o->deadline_ms=now+5000U;
  put64(identity,o->id);put64(identity+8,o->generation);put64(identity+16,o->epoch);memcpy(identity+24,p+8,8);
  memset(live_tokens,0,sizeof(live_tokens));for(i=0;i<2;++i)if(&m->operations[i]!=o&&m->operations[i].state!=PAPACC_C2_OP_EMPTY)live_tokens[count++]=m->operations[i].tokens;
  #ifdef PAPACC_ENABLE_CANONICAL_C8_ROOT
  r=papacc_server_launcher_c2_deadline(s,p,n,&o->policy_deadline_ms);
  if(r!=PAPACC_RESULT_OK){SecureZeroMemory(o,sizeof(*o));result[41]=1U;return PAPACC_RESULT_OK;}
#endif
  r=papacc_c2_tokens_generate(&o->tokens,s,m->owner,identity,live_tokens,count);SecureZeroMemory(live_tokens,sizeof(live_tokens));
  if(r!=0){SecureZeroMemory(o,sizeof(*o));status=5;}else{memcpy(o->open,p,n);o->open_length=n;memcpy(result+8,identity,24);memcpy(result+44,o->tokens.bytes[0],32);memcpy(result+76,o->tokens.bytes[1],32);memcpy(o->result,result,108);*out=o;}
 }
 result[40]=(PAPACC_U8)(status>>8);result[41]=(PAPACC_U8)status;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_result_written(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o){if(!live(m,o))return PAPACC_RESULT_INVALID_STATE;return papacc_c2_tokens_grant_written(&o->tokens,m->owner);}
PAPACC_RESULT papacc_c2_operation_bind(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_SESSION*s,const PAPACC_PRINCIPAL*p,PAPACC_U64 channel,const PAPACC_U8*b,PAPACC_SIZE n,PAPACC_C2_OPERATION**out){
 PAPACC_C2_OPERATION_SESSION*sr;PAPACC_C2_OPERATION*o;PAPACC_RESULT r;PAPACC_U64 now;
 if(out!=NULL)*out=NULL;if(out==NULL||p==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!manager_ok(m)||!session_ok(m,s)||(sr=session_record(m,s))==NULL||!papacc_principal_equal(&sr->principal,p))return PAPACC_RESULT_INVALID_STATE;
 now=(PAPACC_U64)GetTickCount64();if(now-sr->failed_window_ms>=10000U){sr->failed_window_ms=now;sr->failed_candidates=0;}
 if(sr->failed_candidates>=8U)return PAPACC_RESULT_LIMIT_EXCEEDED;
 r=papacc_c2_validate_payload(18,b,n,PAPACC_C2_ROLE_CHANNEL,PAPACC_C2_BROWSER);
 o=r==0?find(m,s,read64(b),read64(b+8)):NULL;
 if(r==0&&(o==NULL||!live(m,o)||o->state!=PAPACC_C2_OP_RESERVED))r=PAPACC_RESULT_INVALID_STATE;
 if(r==0)r=papacc_c2_tokens_claim(&o->tokens,m->owner,s->incarnation,s->principal,channel,b,n);
 if(r!=0){++sr->failed_candidates;return r;}*out=o;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_ack_written(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,unsigned role,PAPACC_U64 channel){if(!live(m,o)||o->state!=PAPACC_C2_OP_RESERVED)return PAPACC_RESULT_INVALID_STATE;return papacc_c2_tokens_ack_written(&o->tokens,m->owner,role,channel);}
PAPACC_RESULT papacc_c2_operation_site_ready(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,const PAPACC_U8*p,PAPACC_SIZE n){PAPACC_RESULT r;PAPACC_U64 now;
 if(!live(m,o)||o->site_ready||!o->tokens.ack_written[0]||!o->tokens.ack_written[1])return PAPACC_RESULT_INVALID_STATE;
 r=papacc_c2_validate_payload(20,p,n,PAPACC_C2_CONTROL,PAPACC_C2_BROWSER);if(r!=0)return r;if(memcmp(p,o->tokens.identity,32)!=0)return PAPACC_RESULT_INVALID_STATE;
 now=(PAPACC_U64)GetTickCount64();if(now>(PAPACC_U64)-1-10000U)return PAPACC_RESULT_INVALID_STATE;
 o->site_ready=PAPACC_TRUE;o->state=PAPACC_C2_OP_HANDSHAKE;o->deadline_ms=now+10000U;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_tls_ready_written(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o){PAPACC_U64 now;
 if(!live(m,o)||o->state!=PAPACC_C2_OP_HANDSHAKE||o->tls_fact!=1U||o->tls_ready_written)return PAPACC_RESULT_INVALID_STATE;
 now=(PAPACC_U64)GetTickCount64();if(now>(PAPACC_U64)-1-30000U)return PAPACC_RESULT_INVALID_STATE;o->tls_ready_written=PAPACC_TRUE;o->state=PAPACC_C2_OP_APPLICATION;o->deadline_ms=now+30000U;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_data(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,PAPACC_U64 channel,const PAPACC_U8*p,PAPACC_SIZE n){PAPACC_RESULT r;PAPACC_C2_FLOW*f;unsigned index;PAPACC_SIZE bytes;
 if(!live(m,o))return PAPACC_RESULT_INVALID_STATE;r=papacc_c2_validate_payload(22,p,n,PAPACC_C2_ROLE_CHANNEL,PAPACC_C2_BROWSER);if(r!=0)return r;
 if(read64(p)!=o->id||read64(p+8)!=o->generation||o->tokens.channels[p[16]-1U]!=channel)return PAPACC_RESULT_INVALID_STATE;
 if((p[16]==1U&&!o->tls_ready_written)||(p[16]==2U&&!o->site_ready))return PAPACC_RESULT_INVALID_STATE;
 index=(p[16]-1U)*2U;f=&o->flow[index];bytes=n-28U;
 if(f->sealed||f->paused||(index==2U&&o->record_events[0])||read64(p+20)!=f->admitted)return PAPACC_RESULT_INVALID_STATE;
 if(bytes>65536U-f->size||f->admitted>(PAPACC_U64)-1-bytes)return PAPACC_RESULT_LIMIT_EXCEEDED;
 #ifdef PAPACC_ENABLE_CANONICAL_C8_ROOT
 if(papacc_server_launcher_c2_flow_limit(m,o,bytes)!=PAPACC_RESULT_OK)return PAPACC_RESULT_LIMIT_EXCEEDED;
#endif
 memcpy(f->bytes+f->size,p+28,bytes);f->size+=bytes;f->admitted+=bytes;if(f->size>=49152U)f->paused=PAPACC_TRUE;c7_flow_metric(m,o,index,"ADMIT");return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_downstream(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,unsigned index,PAPACC_SIZE n){PAPACC_C2_FLOW*f;
 if(!live(m,o)||index>=4U||n==0U||n>16384U)return PAPACC_RESULT_INVALID_STATE;f=&o->flow[index];
 if(n>f->size||f->accepted>(PAPACC_U64)-1-n)return PAPACC_RESULT_INVALID_ARGUMENT;f->accepted+=n;f->size-=n;memmove(f->bytes,f->bytes+n,f->size);if(f->size<=24576U)f->paused=PAPACC_FALSE;
 if(o->state==PAPACC_C2_OP_APPLICATION)o->deadline_ms=(PAPACC_U64)GetTickCount64()+30000U;c7_flow_metric(m,o,index,"DOWNSTREAM");return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_revoke(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,PAPACC_U16 fact){
 if(!owned(m,o)||fact==0U||fact>14U||o->state==PAPACC_C2_OP_RELEASED)return PAPACC_RESULT_INVALID_STATE;
 if(o->first_cause==0U)o->first_cause=fact;if(o->revoked)return PAPACC_RESULT_OK;
 o->revoked=PAPACC_TRUE;o->state=PAPACC_C2_OP_TERMINATING;o->cleanup_deadline_ms=(PAPACC_U64)GetTickCount64()+5000U;
 return papacc_c2_tokens_revoke(&o->tokens,m->owner,PAPACC_FALSE);
}
PAPACC_RESULT papacc_c2_operation_cancel(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_SESSION*s,const PAPACC_U8*p,PAPACC_SIZE n){PAPACC_C2_OPERATION*o=NULL;PAPACC_RESULT r;unsigned i;
 if(!manager_ok(m)||session_record(m,s)==NULL)return PAPACC_RESULT_INVALID_STATE;r=papacc_c2_validate_payload(26,p,n,PAPACC_C2_CONTROL,PAPACC_C2_BROWSER);if(r!=0)return r;
 if(p[3]==2U)return PAPACC_RESULT_PROTOCOL_ERROR;
 if(p[1]==1U){for(i=0;i<2;++i)if(m->operations[i].session==s&&m->operations[i].request==read64(p+8)&&m->operations[i].state!=PAPACC_C2_OP_EMPTY)o=&m->operations[i];}
 else{o=find(m,s,read64(p+16),read64(p+24));if(o!=NULL&&read64(p+32)!=o->epoch)o=NULL;}
 if(o==NULL||o->revoked)return PAPACC_RESULT_PROTOCOL_ERROR;return papacc_c2_operation_revoke(m,o,(PAPACC_U16)p[3]);
}
PAPACC_RESULT papacc_c2_operation_tick(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_U64 now){unsigned i;if(!manager_ok(m))return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<2;++i){PAPACC_C2_OPERATION*o=&m->operations[i];if(o->state==PAPACC_C2_OP_EMPTY||o->revoked||o->state==PAPACC_C2_OP_RELEASED)continue;
  if(!session_ok(m,o->session)||o->epoch!=o->session->policy_epoch)(void)papacc_c2_operation_revoke(m,o,2U);else if(now>=o->deadline_ms)(void)papacc_c2_operation_revoke(m,o,3U);else (void)live(m,o);
 }return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_stop(PAPACC_C2_OPERATION_MANAGER*m){unsigned i;if(!manager_ok(m))return PAPACC_RESULT_INVALID_STATE;m->stopping=PAPACC_TRUE;for(i=0;i<2;++i)if(m->operations[i].state!=PAPACC_C2_OP_EMPTY&&m->operations[i].state!=PAPACC_C2_OP_RELEASED)(void)papacc_c2_operation_revoke(m,&m->operations[i],1U);return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_c2_operation_release_slot(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o){
 if(!owned(m,o)||!o->cleanup_confirmed||(!o->final_written&&!o->summary_unavailable)||o->ownership>=2U||o->callbacks!=0U||o->tokens.borrowers!=0U||!o->revoked)return PAPACC_RESULT_INVALID_STATE;
 SecureZeroMemory(o,sizeof(*o));return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_c2_operation_output(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,unsigned index,const PAPACC_U8*p,PAPACC_SIZE n){PAPACC_C2_FLOW*f;
 if(!live(m,o)||(index!=1U&&index!=3U)||p==NULL||n==0U||n>16356U)return PAPACC_RESULT_INVALID_STATE;
 if((index==1U&&!o->tls_ready_written)||(index==3U&&!o->site_ready))return PAPACC_RESULT_INVALID_STATE;
 f=&o->flow[index];if(f->sealed||f->paused||n>65536U-f->size||f->admitted>(PAPACC_U64)-1-n)return PAPACC_RESULT_LIMIT_EXCEEDED;
 #ifdef PAPACC_ENABLE_CANONICAL_C8_ROOT
 if(papacc_server_launcher_c2_flow_limit(m,o,n)!=PAPACC_RESULT_OK)return PAPACC_RESULT_LIMIT_EXCEEDED;
#endif
 memcpy(f->bytes+f->size,p,n);f->size+=n;f->admitted+=n;if(f->size>=49152U)f->paused=PAPACC_TRUE;c7_flow_metric(m,o,index,"ADMIT");return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_output_accepted(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,unsigned index,PAPACC_SIZE n){PAPACC_C2_FLOW*f;
 if(!live(m,o)||(index!=1U&&index!=3U)||n==0U||n>16356U)return PAPACC_RESULT_INVALID_STATE;f=&o->flow[index];
 if(n>f->size||f->submitted>(PAPACC_U64)-1-n)return PAPACC_RESULT_INVALID_ARGUMENT;
 f->submitted+=n;f->escaped+=n;f->size-=n;memmove(f->bytes,f->bytes+n,f->size);if(f->size<=24576U)f->paused=PAPACC_FALSE;c7_flow_metric(m,o,index,"OUTPUT_ACCEPTED");return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_fence(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,PAPACC_U64 channel,PAPACC_U16 type,const PAPACC_U8*p,PAPACC_SIZE n){PAPACC_RESULT r;PAPACC_C2_FLOW*f;unsigned index;PAPACC_U64 count;
 if(!live(m,o)||!o->site_ready||(type!=23U&&type!=24U))return PAPACC_RESULT_INVALID_STATE;
 r=papacc_c2_validate_payload(type,p,n,PAPACC_C2_ROLE_CHANNEL,PAPACC_C2_BROWSER);if(r!=0)return r;
 if(read64(p)!=o->id||read64(p+8)!=o->generation||o->tokens.channels[p[16]-1U]!=channel)return PAPACC_RESULT_INVALID_STATE;
 if(p[16]==1U&&!o->tls_ready_written)return PAPACC_RESULT_INVALID_STATE;index=(p[16]-1U)*2U+p[17];f=&o->flow[index];count=read64(p+20);
 if(type==23U&&index==2U&&(!o->reciprocal_complete||!o->flow[1].fence_written))return PAPACC_RESULT_PROTOCOL_ERROR;
 if(type==23U){if(f->fence_received||f->sealed||count!=f->admitted)return PAPACC_RESULT_PROTOCOL_ERROR;f->fence=count;f->sealed=PAPACC_TRUE;f->fence_received=PAPACC_TRUE;}
 else{if(!f->fence_written||f->ack_received||count!=f->fence||f->submitted!=count)return PAPACC_RESULT_PROTOCOL_ERROR;f->ack_received=PAPACC_TRUE;f->accepted=count;}
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_fence_written(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,unsigned index,PAPACC_BOOL ack){PAPACC_C2_FLOW*f;
 if(!live(m,o)||index>=4U||!o->site_ready)return PAPACC_RESULT_INVALID_STATE;f=&o->flow[index];
 if(ack){if((index&1U)!=0U||!f->fence_received||f->ack_written||f->accepted!=f->fence||f->size!=0U)return PAPACC_RESULT_INVALID_STATE;f->ack_written=PAPACC_TRUE;}
 else{if((index&1U)==0U||f->fence_written||f->size||f->submitted!=f->admitted||(index==1U&&!o->reciprocal_complete)||(index==3U&&(!o->child_destroyed||!o->ordered_eof)))return PAPACC_RESULT_INVALID_STATE;
  f->fence=f->submitted;f->sealed=PAPACC_TRUE;f->fence_written=PAPACC_TRUE;}
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_record_event(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,const PAPACC_U8*p,PAPACC_SIZE n){PAPACC_RESULT r;unsigned index;PAPACC_U16 event;
 if(!live(m,o)||!o->site_ready)return PAPACC_RESULT_INVALID_STATE;r=papacc_c2_validate_payload(25,p,n,PAPACC_C2_ROLE_CHANNEL,PAPACC_C2_BROWSER);if(r!=0)return r;
 if(read64(p)!=o->id||read64(p+8)!=o->generation)return PAPACC_RESULT_INVALID_STATE;index=p[17];event=(PAPACC_U16)p[29];
 if(o->record_events[index]||read64(p+20)!=(index==0U?o->flow[2].admitted:o->flow[3].accepted))return PAPACC_RESULT_PROTOCOL_ERROR;o->record_events[index]=PAPACC_TRUE;
 if(event==2U||event==3U)return papacc_c2_operation_revoke(m,o,event==2U?7U:8U);return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_c2_operation_final_payload(PAPACC_C2_OPERATION_MANAGER*m,PAPACC_C2_OPERATION*o,PAPACC_U8 p[76]){unsigned i;PAPACC_U8 mask=0;PAPACC_U16 fact,action,outcome;
 if(!owned(m,o)||p==NULL||o->final_written)return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<4;++i)if((i&1U)?o->flow[i].ack_received:o->flow[i].ack_written)mask|=(PAPACC_U8)(1U<<i);
 if(o->ownership>=2U){if(!o->revoked)return PAPACC_RESULT_INVALID_STATE;fact=o->first_cause;action=2;outcome=3;}
 else if(o->revoked){if(!o->cleanup_confirmed)return PAPACC_RESULT_INVALID_STATE;fact=o->first_cause;action=1;outcome=2;}
 else{if(!o->cleanup_confirmed||!o->reciprocal_complete||!o->child_destroyed||!o->ordered_eof||mask!=15U||o->ownership!=1U)return PAPACC_RESULT_INVALID_STATE;fact=0;action=3;outcome=1;}
 if(o->callbacks>(o->final_queued?1U:0U)||o->tokens.borrowers!=0U)return PAPACC_RESULT_INVALID_STATE;
 memset(p,0,76);put64(p,o->request);put64(p+8,o->id);put64(p+16,o->generation);put64(p+24,o->epoch);p[33]=(PAPACC_U8)fact;p[35]=(PAPACC_U8)action;p[37]=(PAPACC_U8)outcome;p[39]=(PAPACC_U8)o->tls_fact;p[41]=(PAPACC_U8)o->ownership;p[42]=mask;p[43]=o->session->assurance;
 for(i=0;i<4;++i)put64(p+44+i*8U,o->flow[i].admitted);
 return papacc_c2_validate_payload(27,p,76,PAPACC_C2_CONTROL,PAPACC_C2_ACCELERATOR);
}
