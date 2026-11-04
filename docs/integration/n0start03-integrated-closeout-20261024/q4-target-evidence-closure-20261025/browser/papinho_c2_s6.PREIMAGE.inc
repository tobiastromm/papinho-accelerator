/* S6 phase algorithms adapted to the current canonical API3 composition. */
#include "papinho_c2_s6.h"
#include "../third_party/pacc-c2-private/c2_codec.h"
#include "../core/networking/papinho_site_pump.h"
#define C2_HTTP_CAP 524288U
#define C2_FLOW_CAP 65536U
#define C2_FRAME_CAP 16400U
#include "papinho_c2_first_cause.inc"
typedef struct C2_ROLE {
 papinho_accelerator_stream_state *stream;
 unsigned char rx[C2_FRAME_CAP],tx[C2_FRAME_CAP];
 size_t rx_used,rx_needed,tx_size,tx_sent;unsigned type;int eof,shutdown_done,tx_attempted;
} C2_ROLE;
struct papinho_c2_s6_context {
 C2_CAUSE_RECORD cause;papinho_c2_s6_facts receipt;
 struct papinho_c2_s6_context *next;
 papinho_accelerator_persistent_session *session;
 papinho_accelerator_client_config config;
 papinho_accelerator_stream_state record;
 C2_ROLE channel[3];pst_wait_set *waitset;
 HANDLE cancel;papinho_credential_close_receipt cancel_close;
 DWORD thread,started,last_progress,disposition_started;
 int disposition_started_valid;unsigned long navigation;
 papinho_site_pump *site;papinho_site_token site_token;papinho_endpoint_snapshot endpoint;
 unsigned char identity[32],tokens[64],http[C2_HTTP_CAP],record_ab[C2_FLOW_CAP];
 size_t http_size,record_ab_size;PAPACC_U64 count[4],site_ab_accepted;
 PAPACC_U64 request,operation,generation,epoch;unsigned long resolution;
 int tls_ready,freeze_ba,site_eof,record_event_sent,revoked,retained,final_seen;
 int bound[3];int fence_sent[4],fence_seen[4],ack_seen[4],ack_written[4];PAPACC_U64 fences[4];
 unsigned short remote_fact;int remote_outcome,remote_ownership;
 size_t reserved;unsigned long release_attempts,site_close_attempts;
 int site_closed,root_grants,local_cleanup_done,local_destroyed,record_read_paused;
};
static papinho_c2_s6_context *c2_retained;
static unsigned long c2_s6_identity;
int papinho_c2_s6_enabled(void){char v[8];return GetEnvironmentVariableA("PAPINHO_C2_MODE_C",v,sizeof(v))==1U&&v[0]=='1';}
static void c2_trace(papinho_c2_s6_context *c,const char *event){(void)c;(void)event;}
/* Existing first-cause/publication sequence is the sole local event domain.
 * Receipt loss/exhaustion poisons completeness, never functional authority. */
static unsigned long c2_receipt_tick(papinho_c2_s6_context *c){
 if(c->cause.owner_thread!=GetCurrentThreadId() || c->cause.sequence==0xffffffffUL){c->receipt.capture_incomplete=1;return 0;}
 return ++c->cause.sequence;
}
static void c2_receipt_once(papinho_c2_s6_context *c,unsigned long *ref){
 if(*ref){c->receipt.capture_incomplete=1;return;}*ref=c2_receipt_tick(c);
}
/* Only existing producer-local Win32 observations; no handles are acquired.
 * Image-file hashing is performed by the existing controlled offline launcher
 * against this observed mapping/path, independently from expected build hashes. */
#ifndef PAPINHO_C2_NATIVE_BUILD_DEBUG
#define PAPINHO_C2_NATIVE_BUILD_DEBUG (-1)
#endif
#define C2_NATIVE_SDK_MANIFEST "1a945de88199a8317355b924e0255d4a214fd3f849d7c793aae14c05abc4f3af"
static PAPACC_U64 c2_native_time(FILETIME t){return ((PAPACC_U64)t.dwHighDateTime<<32)|t.dwLowDateTime;}
static void c2_native_observe(papinho_c2_s6_facts *f){
 FILETIME birth,end,kernel,user;MEMORY_BASIC_INFORMATION m;HMODULE image;DWORD n;
 ZeroMemory(&f->native,sizeof(f->native));f->process_id=GetCurrentProcessId();f->owner_thread=GetCurrentThreadId();
 f->native.observed_tick=GetTickCount();f->native.presence|=16U;
 if(GetProcessTimes(GetCurrentProcess(),&birth,&end,&kernel,&user)){
  f->native.creation=c2_native_time(birth);f->native.presence|=1U;
 }else f->native.error=GetLastError();
 if(GetThreadTimes(GetCurrentThread(),&birth,&end,&kernel,&user)){
  f->native.thread_creation=c2_native_time(birth);f->native.presence|=2U;
 }else if(!f->native.error)f->native.error=GetLastError();
 image=GetModuleHandleA(NULL);ZeroMemory(&m,sizeof(m));
 if(image && VirtualQuery((LPCVOID)image,&m,sizeof(m))==sizeof(m) && m.Type==MEM_IMAGE && m.AllocationBase==(LPVOID)image){
  f->native.mapped_base=(unsigned long)m.AllocationBase;f->native.presence|=4U;
 }
 n=GetModuleFileNameW(image,f->native.image_path,MAX_PATH);
 if(n && n<MAX_PATH)f->native.presence|=8U;else f->native.image_path[0]=0;
 f->native.root_identity=(unsigned long)root_read(&papinho_root_mutex_identity);
 f->native.root_state=(unsigned long)papinho_pst_process_root_state();
 if(f->native.root_identity)f->native.presence|=64U;
 f->native.validated_tick=GetTickCount();f->native.presence|=32U;
}

/* Local layouts avoid a hard dependency on a later SDK/PSAPI import.
 * On older systems a missing optional API stays absent. No DLL is loaded. */
typedef struct C2_PROCESS_MEMORY {
 DWORD cb,faults,peak_working,working,peak_paged,paged,peak_nonpaged,nonpaged;
 DWORD pagefile,peak_pagefile,private_usage;
} C2_PROCESS_MEMORY;
typedef struct C2_MEMORY_STATUS {
 DWORD length,load;PAPACC_U64 physical,available_physical,pagefile,available_pagefile;
 PAPACC_U64 virtual_bytes,available_virtual,extended;
} C2_MEMORY_STATUS;
typedef BOOL (WINAPI *C2_MEMORY_QUERY)(HANDLE,C2_PROCESS_MEMORY *,DWORD);
typedef BOOL (WINAPI *C2_HANDLE_QUERY)(HANDLE,LPDWORD);
typedef BOOL (WINAPI *C2_CAPACITY_QUERY)(C2_MEMORY_STATUS *);
void papinho_candidate_resource_observe(papinho_candidate_resource_sample *r){
 HMODULE k;C2_MEMORY_QUERY memory;C2_HANDLE_QUERY handles;C2_CAPACITY_QUERY capacity;
 C2_PROCESS_MEMORY m;C2_MEMORY_STATUS status;FILETIME birth,end,kernel,user;DWORD count;
 ZeroMemory(r,sizeof(*r));r->begin_tick=GetTickCount();r->process=GetCurrentProcessId();
 r->mapped_base=(DWORD)GetModuleHandleA(NULL);
 if(GetProcessTimes(GetCurrentProcess(),&birth,&end,&kernel,&user)){
  r->creation=c2_native_time(birth);r->presence|=1U;
 }else r->identity_error=GetLastError();
 /* kernel32 is a process-lifetime dependency. An arbitrary loaded PSAPI
  * module has no proved durable lease here and is deliberately not borrowed. */
 k=GetModuleHandleA("kernel32.dll");
 memory=k?(C2_MEMORY_QUERY)GetProcAddress(k,"K32GetProcessMemoryInfo"):NULL;
 if(memory)r->source|=1U;
 if(memory){
  ZeroMemory(&m,sizeof(m));m.cb=sizeof(m);
  if(memory(GetCurrentProcess(),&m,sizeof(m))){
   if(m.cb>=sizeof(m)){r->private_bytes=m.private_usage;r->presence|=2U;r->source|=4U;}else r->source|=8U;
  }else{
   /* Old PROCESS_MEMORY_COUNTERS has no PrivateUsage. Successful fallback
    * preserves commit/OS peak but never substitutes WorkingSet for private. */
   ZeroMemory(&m,sizeof(m));m.cb=sizeof(m)-sizeof(DWORD);
   if(memory(GetCurrentProcess(),&m,m.cb))r->source|=8U;
   else r->memory_error=GetLastError();
  }
  if(r->source&12U){r->commit_bytes=m.pagefile;r->os_peak_commit=m.peak_pagefile;r->presence|=20U;}
 }else r->memory_error=ERROR_CALL_NOT_IMPLEMENTED;
 handles=k?(C2_HANDLE_QUERY)GetProcAddress(k,"GetProcessHandleCount"):NULL;
 if(handles){if(handles(GetCurrentProcess(),&count)){r->handles=count;r->presence|=8U;}else r->handles_error=GetLastError();}
 else r->handles_error=ERROR_CALL_NOT_IMPLEMENTED;
 capacity=k?(C2_CAPACITY_QUERY)GetProcAddress(k,"GlobalMemoryStatusEx"):NULL;
 if(capacity){ZeroMemory(&status,sizeof(status));status.length=sizeof(status);
  if(capacity(&status)){r->available_physical=status.available_physical;r->available_pagefile=status.available_pagefile;r->presence|=96U;}
  else r->capacity_error=GetLastError();
 }else r->capacity_error=ERROR_CALL_NOT_IMPLEMENTED;
 r->end_tick=GetTickCount();if(r->end_tick>=r->begin_tick)r->presence|=128U;
}
static int c2_resource_identity(const papinho_c2_s6_facts *f,const papinho_candidate_resource_sample *r){
 return (f->native.presence&5U)==5U && (r->presence&1U) &&
  r->process==f->process_id && r->creation==f->native.creation && r->mapped_base==f->native.mapped_base;
}
static void c2_resource_phase(papinho_c2_s6_facts *f,unsigned phase,unsigned long witness,int retained,int stopped,int teardown){
 if(phase>=8U || (f->resource_mask&(1UL<<phase)))return;
 papinho_candidate_resource_observe(&f->resource[phase].sample);
 f->resource[phase].witness=witness;f->resource[phase].retained=retained;
 f->resource[phase].stopped=stopped;f->resource[phase].teardown=teardown;
 f->resource_mask|=1UL<<phase;
}
static void c2_resource_boundary(papinho_c2_s6_context *c,unsigned phase){
 c2_resource_phase(&c->receipt,phase,c->cause.sequence,c->retained,c->local_cleanup_done,c->local_destroyed);
}
static void c2_resource_number(char *out,int present,PAPACC_U64 value){
 if(present)_snprintf(out,32U,"%I64u",value);else strcpy(out,"null");
}
/* One bounded extension inside the existing line, not another event stream.
 * Column order is pinned in the existing offline decoder/handoff. */
static int c2_resource_json(const papinho_c2_s6_facts *f,char *out,size_t cap){
 size_t used;int n;unsigned i,j;unsigned long peak_mask=0;PAPACC_U64 peak[3];
 char values[6][32],peak_values[3][32];const papinho_candidate_resource_sample *r;
 ZeroMemory(peak,sizeof(peak));
 n=_snprintf(out,cap,"{\"scope\":%lu,\"mask\":%lu,\"clock\":\"WIN32_GETTICKCOUNT_U32\",\"slots\":[",f->resource_scope,f->resource_mask);
 if(n<0 || (size_t)n>=cap)return 0;used=(size_t)n;
 for(i=0;i<8U;i++){
  if(!(f->resource_mask&(1UL<<i)))n=_snprintf(out+used,cap-used,"%snull",i?",":"");
  else{
   r=&f->resource[i].sample;
   c2_resource_number(values[0],r->presence&2U,r->private_bytes);
   c2_resource_number(values[1],r->presence&4U,r->commit_bytes);
   c2_resource_number(values[2],r->presence&8U,r->handles);
   c2_resource_number(values[3],r->presence&16U,r->os_peak_commit);
   c2_resource_number(values[4],r->presence&32U,r->available_physical);
   c2_resource_number(values[5],r->presence&64U,r->available_pagefile);
   if(c2_resource_identity(f,r) && (r->presence&128U))for(j=0;j<3U;j++){
    PAPACC_U64 value=j==0U?r->private_bytes:(j==1U?r->commit_bytes:r->handles);
    if(r->presence&(2UL<<j)){peak_mask|=2UL<<j;if(value>peak[j])peak[j]=value;}
   }
   n=_snprintf(out+used,cap-used,"%s[%lu,%lu,%lu,%lu,%I64u,%lu,%lu,%lu,%lu,%lu,%lu,%s,%s,%s,%s,%s,%s,%lu,%d,%d,%d]",i?",":"",r->presence,r->begin_tick,r->end_tick,r->process,r->creation,r->mapped_base,r->source,r->identity_error,r->memory_error,r->handles_error,r->capacity_error,values[0],values[1],values[2],values[3],values[4],values[5],f->resource[i].witness,f->resource[i].retained,f->resource[i].stopped,f->resource[i].teardown);
  }
  if(n<0 || (size_t)n>=cap-used)return 0;used+=(size_t)n;
 }
 for(j=0;j<3U;j++)c2_resource_number(peak_values[j],peak_mask&(2UL<<j),peak[j]);
 n=_snprintf(out+used,cap-used,"],\"observed_peak\":[%s,%s,%s],\"sampling\":\"FIRST_REACHED_BOUNDARIES_UNSAMPLED_GAPS\",\"validity\":\"INCOMPLETE\"}",peak_values[0],peak_values[1],peak_values[2]);
 return n>=0 && (size_t)n<cap-used;
}
static int c2_receipt_emit(const papinho_c2_s6_facts *,const char *);

static void c2_native_escape(const WCHAR *source,char *out){
 static const char hex[]="0123456789abcdef";unsigned i,j=0,v;
 for(i=0;i<MAX_PATH && source[i];i++){
  v=(unsigned)source[i];
  if(v>=32U && v<127U && v!=34U && v!=92U)out[j++]=(char)v;
  else{out[j++]=92;out[j++]='u';out[j++]=hex[(v>>12)&15U];out[j++]=hex[(v>>8)&15U];out[j++]=hex[(v>>4)&15U];out[j++]=hex[v&15U];}
 }out[j]=0;
}
void papinho_candidate_native_join_report(DWORD thread,const FILETIME *thread_birth,int birth_present,int joined,int closed,int retained,DWORD error,const papinho_candidate_resource_sample *baseline,const papinho_candidate_resource_sample *stop){
 FILETIME birth,end,kernel,user;PAPACC_U64 creation=0,t=0;int present;
 papinho_c2_s6_facts f;char resources[4096];
 ZeroMemory(&f,sizeof(f));c2_native_observe(&f);f.resource_scope=3U;
 if(baseline){f.resource[0].sample=*baseline;f.resource_mask|=1U;}
 if(stop){f.resource[6].sample=*stop;f.resource[6].stopped=1;f.resource_mask|=64U;}
 /* Producer close is reported separately; policy/other owners may survive. */
 c2_resource_phase(&f,7U,0,retained,1,0);
 if(!c2_resource_json(&f,resources,sizeof(resources)))strcpy(resources,"null");
 present=GetProcessTimes(GetCurrentProcess(),&birth,&end,&kernel,&user)!=0;
 if(present)creation=c2_native_time(birth);if(birth_present && thread_birth)t=c2_native_time(*thread_birth);
 fprintf(stderr,"MODE_C_PRODUCER_JOIN {\"version\":1,\"process\":%lu,\"creation\":%I64u,\"creation_present\":%d,\"thread\":%lu,\"thread_creation\":%I64u,\"thread_creation_present\":%d,\"joined\":%d,\"closed\":%d,\"retained\":%d,\"error\":%lu,\"resources\":%s}\n",GetCurrentProcessId(),creation,present,thread,t,birth_present,joined,closed,retained,error,resources);
}
static void c2_receipt_session(papinho_c2_s6_context *c,papinho_session_storage *s){
 papinho_group_borrow *b;unsigned i;
 if(!s)return;
 c->receipt.session_generation=s->generation;c->receipt.group_serial=s->group->serial;
 c->receipt.stop_attempted=s->stop_attempted;c->receipt.bridge_joined=s->joined;
 c->receipt.bridge_join_result=s->join_result;c->receipt.bridge_close_succeeded=s->thread_close.succeeded;
 c->receipt.session_quiesced=root_read(&s->state)==SESSION_QUIESCED;
 c->receipt.auxiliary_confirmed=s->auxiliary_confirmed;
 c->receipt.group_destroyed=s->group->state==GROUP_DESTROYED;
 c->receipt.live_borrowers=0;
 for(b=s->group->borrows;b;b=b->next)if(b->live)++c->receipt.live_borrowers;
 for(i=0;i<3U;++i){
  if(!s->c2_release[i].identity)continue;
  c->receipt.release[i].identity=s->c2_release[i].identity;
  c->receipt.release[i].generation=s->c2_release[i].generation;
  c->receipt.release[i].attempts=s->c2_release[i].attempts;
  c->receipt.release[i].result=s->c2_release[i].result;
  c->receipt.release[i].disposition=s->c2_release[i].disposition;
  c->receipt.release[i].quiesced=s->c2_release[i].quiesced;
  c->receipt.release[i].finalized=s->c2_release[i].finalized;
  c->receipt.release[i].membership_absent=s->c2_release[i].membership_absent;
 }
}
static void c2_receipt_copy(papinho_c2_s6_context *c,papinho_c2_s6_facts *f){
 if(!f)return;*f=c->receipt;
 f->grants=c->root_grants;f->tls_ready=c->tls_ready;f->local_destroyed=c->local_destroyed&&c->site_closed;
 f->remote_fact=c->remote_fact;f->remote_outcome=c->remote_outcome;f->remote_ownership=c->remote_ownership;
 f->navigation_id=c->navigation;f->request=c->request;f->operation=c->operation;f->generation=c->generation;f->epoch=c->epoch;
 f->process_id=GetCurrentProcessId();f->owner_thread=c->thread;f->attempt=c->navigation;f->resolution=c->endpoint.resolution_generation;
 memcpy(f->prefixes,c->count,sizeof(c->count));f->pst_release_attempts=c->release_attempts;f->site_close_attempts=c->site_close_attempts;
 f->cancel_attempted=c->cancel_close.attempted;f->cancel_succeeded=c->cancel_close.succeeded;
 f->retained=c->retained;f->site_closed=c->site_closed;f->cancel_closed=c->cancel==NULL;
 f->first_cause_seq=c->cause.first_sequence;f->publication_seq=c->cause.publication_sequence;
}

static void c2_observe(papinho_c2_s6_context *c,unsigned cause,unsigned source,long detail){
 if(c && cause==C2_CAUSE_BOUNDARY && c->site){papinho_site_status native;
  papinho_site_status_get(c->site,&native);
  if(native.terminal==PAPINHO_SITE_RESET || native.terminal==PAPINHO_SITE_SOCKET_ERROR){cause=C2_CAUSE_SITE_ERROR;detail=native.socket_error;}
 }
 if(c)(void)c2_cause_observe(&c->cause,cause,source,detail);
}
static void c2_publish_cause(papinho_c2_s6_context *c,int action){(void)action;(void)c2_cause_publish(&c->cause);}
static int c2_live(papinho_c2_s6_context *c){return c && c->thread==GetCurrentThreadId() && !c->retained && !c->revoked && !root_read(&papinho_pst_admission_stopped);}
static int c2_count_add(PAPACC_U64 *v,size_t n){if((PAPACC_U64)n>~(PAPACC_U64)0-*v)return 0;*v+=(PAPACC_U64)n;return 1;}

static unsigned c2_u16(const unsigned char *p){return ((unsigned)p[0]<<8)|p[1];}

static void c2_put16(unsigned char *p,unsigned v){p[0]=(unsigned char)(v>>8);p[1]=(unsigned char)v;}

static void c2_put32(unsigned char *p,unsigned long v){p[0]=(unsigned char)(v>>24);p[1]=(unsigned char)(v>>16);p[2]=(unsigned char)(v>>8);p[3]=(unsigned char)v;}

static PAPACC_U64 c2_u64(const unsigned char *p){PAPACC_U64 v=0;unsigned i;for(i=0;i<8U;++i)v=(v<<8)|p[i];return v;}

static void c2_put64(unsigned char *p,PAPACC_U64 v){unsigned i;for(i=0;i<8U;++i)p[7U-i]=(unsigned char)(v>>(8U*i));}

static int c2_queue(papinho_c2_s6_context *c,unsigned ch,unsigned type,const unsigned char *p,size_t n){
 C2_ROLE *r=&c->channel[ch];PAPACC_SIZE written;
 if(!c2_live(c)||r->tx_size)return 0;
 if(papacc_c2_encode_frame((PAPACC_U16)type,p,n,ch?PAPACC_C2_ROLE_CHANNEL:PAPACC_C2_CONTROL,PAPACC_C2_BROWSER,r->tx,sizeof(r->tx),&written)!=0)return 0;
 r->tx_size=written;r->tx_sent=0;r->tx_attempted=0;r->type=type;return 1;
}

static int c2_write_once(papinho_c2_s6_context *c,unsigned ch){
 C2_ROLE *r=&c->channel[ch];PST_IO_RESULT io;size_t payload,amount;unsigned flow;
 if(!r->tx_size)return 1;if(c->retained||!r->stream->secure_connection)return 0;
 amount=r->tx_size-r->tx_sent;
 
 
 r->tx_attempted=1;
 {PST_RESULT actual=pst_connection_write(r->stream->secure_connection,r->tx+r->tx_sent,(pst_size)amount,&io);if(actual!=0||io.operation==PST_OPERATION_FAILED||io.operation==PST_OPERATION_CLOSED){c2_observe(c,C2_CAUSE_PST_IO,(unsigned)__LINE__,(long)actual);return 0;}}
 if(io.bytes_transferred>r->tx_size-r->tx_sent)return 0;r->tx_sent+=(size_t)io.bytes_transferred;
 
 
 if(io.bytes_transferred)c->last_progress=GetTickCount();
 
 if(r->tx_sent==r->tx_size){
  if(r->type==22U){payload=r->tx_size-44U;flow=(ch-1U)*2U;if(!c2_count_add(&c->count[flow],payload))return 0;}
  if(r->type==23U){flow=(ch-1U)*2U;c->fence_sent[flow]=1;}
  if(r->type==24U){flow=(ch-1U)*2U+1U;c->ack_written[flow]=1;}
  if(r->type==22U){c->receipt.prefix_seq=c2_receipt_tick(c);if(c->tls_ready)c2_resource_boundary(c,5U);}
  if(r->type==23U){c->receipt.fence_values[flow]=c2_u64(r->tx+36);c2_receipt_once(c,&c->receipt.fence_seq[flow]);}
  if(r->type==24U){c->receipt.ack_values[flow]=c2_u64(r->tx+36);c2_receipt_once(c,&c->receipt.ack_seq[flow]);}
  if(r->type==18U && ch>=1U && ch<=2U)c2_receipt_once(c,&c->receipt.bind_seq[ch-1U]);
  if(r->type==18U)papinho_c2_wipe(c->tokens+(ch-1U)*32U,32U);
  papinho_c2_wipe(r->tx,r->tx_size);r->tx_size=r->tx_sent=0;r->tx_attempted=0;
 }
 return 1;
}

static int c2_read_once(papinho_c2_s6_context *c,unsigned ch,PAPACC_C2_VIEW *view){
 C2_ROLE *r=&c->channel[ch];PST_IO_RESULT io;size_t wanted;unsigned type;size_t payload;
 view->payload=NULL;view->length=0;view->type=0;if(r->eof)return 0;if(!r->stream||!r->stream->secure_connection)return -1;
 if(!r->rx_needed)r->rx_needed=16U;wanted=r->rx_needed-r->rx_used;
 {PST_RESULT actual=pst_connection_read(r->stream->secure_connection,r->rx+r->rx_used,(pst_size)wanted,&io);if(actual!=0||io.operation==PST_OPERATION_FAILED){c2_observe(c,C2_CAUSE_PST_IO,(unsigned)__LINE__,(long)actual);return -1;}}
 if(io.operation==PST_OPERATION_CLOSED){if(io.close_kind!=PST_CLOSE_CLEAN)return -1;r->eof=1;return r->rx_used?-1:0;}
 if(io.bytes_transferred>wanted)return -1;r->rx_used+=(size_t)io.bytes_transferred;if(io.bytes_transferred)c->last_progress=GetTickCount();
 
 if(r->rx_used==16U&&r->rx_needed==16U){
  if(memcmp(r->rx,"PACC",4U)||r->rx[4]!=1U||r->rx[5]||c2_u16(r->rx+6)!=16U||c2_u16(r->rx+10))return -1;
  type=c2_u16(r->rx+8);payload=((size_t)r->rx[12]<<24)|((size_t)r->rx[13]<<16)|((size_t)r->rx[14]<<8)|r->rx[15];
  if(type<16U||type>27U||payload>sizeof(r->rx)-16U)return -1;
  if((!ch&&(type!=17U&&type!=21U&&type!=26U&&type!=27U))||(ch&&(type!=19U&&type!=22U&&type!=23U&&type!=24U)))return -1;
  if(!ch&&payload>803U)return -1;r->rx_needed=16U+payload;
 }
 if(r->rx_used!=r->rx_needed)return 0;
 if(papacc_c2_decode_frame(r->rx,r->rx_used,ch?PAPACC_C2_ROLE_CHANNEL:PAPACC_C2_CONTROL,PAPACC_C2_ACCELERATOR,view)!=0)return -1;
 return 1;
}

static void c2_rx_done(C2_ROLE *r){papinho_c2_wipe(r->rx,r->rx_used);r->rx_used=0;r->rx_needed=16U;}

static int c2_wait(papinho_c2_s6_context *c,DWORD max_ms){PST_WAIT_EVENT events[3];PST_WAIT_SET_RESULT result;PST_RESULT waited;DWORD remaining=max_ms;
 if(remaining>100U)remaining=0U;if(max_ms<remaining)remaining=max_ms;
 if(!c2_capacity())return 0;
 /* Do not block the joint owner while C1 has actual readiness AND bounded
  * pending work. WOULD_BLOCK clears C1 readiness; a subsequent no-work round
  * retains the normal finite PST wait. Readiness alone never means progress. */
 if(c->site){int interests=0,ready;
  if(c->record_ab_size)interests|=PAPINHO_SITE_WRITE;
  if(!c->freeze_ba&&!c->site_eof&&!c->channel[2].tx_size)interests|=PAPINHO_SITE_READ;
  if(interests){ready=papinho_site_wait(c->site,c->site_token,interests,0U);if(ready<0)return 0;
   if(ready&interests){remaining=0U;}
  }
 }
 {waited=pst_wait_set_wait(c->waitset,remaining,events,3U,&result);
 }

 /* A finite readiness wait may expire normally. This confers no protocol
  * completion: callers still enforce factual events and phase deadlines. */
 if(waited==PST_RESULT_WAIT_TIMEOUT){c2_trace(c,"FINITE_READINESS_WAIT_EXPIRED_NO_PHASE_COMPLETION");return 1;}
 if(waited!=PST_RESULT_OK)fprintf(stderr,"S6_WAIT_FAILED operation=%I64u generation=%I64u result=%ld\n",c->operation,c->generation,(long)waited);
 return waited==PST_RESULT_OK;
}

static int c2_round(papinho_c2_s6_context *c){unsigned i;if(!c2_capacity())return 0;for(i=0;i<3U;++i)if(!c2_write_once(c,i))return 0;return c2_wait(c,100U);}

static int c2_read_frame(papinho_c2_s6_context *c,unsigned ch,unsigned expected,PAPACC_C2_VIEW *view){DWORD start=GetTickCount();int result;
 while((DWORD)(GetTickCount()-start)<5000U){if(papinho_accel_cancelled(c->cancel))return 0;result=c2_read_once(c,ch,view);if(result<0)return 0;if(result>0)return view->type==expected;if(!c2_round(c))return 0;}return 0;}

static int c2_endpoint(papinho_c2_s6_context *c,const char *host,unsigned short port){
 struct hostent *answer;unsigned i;size_t n=strlen(host);
 if(!port||n==0U||n>=sizeof(c->endpoint.original_hostname)||c->resolution==0xffffffffUL)return 0;
 answer=gethostbyname(host);if(!answer||answer->h_addrtype!=AF_INET||answer->h_length!=4||!answer->h_addr_list||!answer->h_addr_list[0])return 0;
 for(i=0U;answer->h_addr_list[i];++i){const unsigned char *p=(const unsigned char*)answer->h_addr_list[i];if(i>=32U||p[0]!=127U||p[1]||p[2]||p[3]!=1U)return 0;}
 memset(&c->endpoint,0,sizeof(c->endpoint));memcpy(c->endpoint.original_hostname,host,n+1U);c->endpoint.address_family=PAPINHO_ENDPOINT_IPV4;c->endpoint.address_size=4U;
 memcpy(c->endpoint.numeric_address,answer->h_addr_list[0],4U);c->endpoint.port=port;c->endpoint.resolution_generation=++c->resolution;
 c2_trace(c,"DNS_ONCE_ALL_CANDIDATES_APPROVED_NUMERIC_SNAPSHOT");return 1;
}

static int c2_accept_open_result(papinho_c2_s6_context *c,const PAPACC_C2_VIEW *v,const unsigned char *attempt){
 const unsigned char *p=v->payload;
 if(!c2_live(c) || papinho_accel_cancelled(c->cancel) || !c->root_grants || c->operation ||
    v->type!=17U || v->length!=108U || !p ||
    papacc_c2_validate_payload(17U,p,v->length,PAPACC_C2_CONTROL,PAPACC_C2_ACCELERATOR)!=PAPACC_RESULT_OK ||
    c2_u64(p)!=c->request || c2_u16(p+40)!=0U || p[42]!=1U || memcmp(p+32,attempt,8U))return 0;
 memcpy(c->identity,p+8,24U);memcpy(c->identity+24,p+32,8U);memcpy(c->tokens,p+44,64U);
 c->operation=c2_u64(c->identity);c->generation=c2_u64(c->identity+8);c->epoch=c2_u64(c->identity+16);c2_receipt_once(c,&c->receipt.open_result_seq);return 1;
}
static int c2_accept_bind_ack(papinho_c2_s6_context *c,unsigned role,const PAPACC_C2_VIEW *v){
 const unsigned char *p=v->payload;
 if(role<1U || role>2U || !c2_live(c) || papinho_accel_cancelled(c->cancel) ||
    !c->root_grants || !c->operation || c->bound[role] || v->type!=19U || v->length!=28U || !p ||
    papacc_c2_validate_payload(19U,p,v->length,PAPACC_C2_ROLE_CHANNEL,PAPACC_C2_ACCELERATOR)!=PAPACC_RESULT_OK ||
    memcmp(p,c->identity,24U) || p[24]!=role || p[25])return 0;
 c->bound[role]=1;c2_receipt_once(c,&c->receipt.bind_ack_seq[role-1U]);if(c->bound[1]&&c->bound[2])c2_resource_boundary(c,3U);return 1;
}

static int c2_add_stream(papinho_c2_s6_context *c,unsigned index){
 papinho_accelerator_stream_state *s;papinho_pst_owner *o;papinho_session_storage *storage;
 if(!c || index>=3U || !c->session || !c->waitset)return 0;
 storage=candidate_storage(c->session);s=c->channel[index].stream;
 if(!storage || !s || (index && (!c->operation || !c->generation)))return 0;
 o=(papinho_pst_owner *)s->candidate_owner;
 if(!o || o->dependencies.context!=storage->group || s->candidate_group!=storage->group ||
    o->connection!=s->secure_connection || !o->connection || o->wait_set || o->release_attempts ||
    !papinho_pst_owner_callback_allowed(o,o->identity,o->generation))return 0;
 if(pst_wait_set_add_connection(c->waitset,s->secure_connection,(pst_wait_token)(index+1U))!=PST_RESULT_OK)return 0;
 o->role=index+1U;o->wait_set=c->waitset;
 c->receipt.release[index].identity=o->identity;c->receipt.release[index].generation=o->generation;
 c->receipt.session_generation=storage->generation;c->receipt.group_serial=storage->group->serial;return 1;
}

static int c2_old_read_exact(papinho_c2_s6_context *c,papinho_accelerator_stream_state *s,unsigned char *p,size_t n){
 size_t used=0;int amount;while(used<n){if(!c2_capacity())return 0;amount=papinho_accel_stream_receive(s,p+used,(int)(n-used),&c->config,c->cancel);if(amount<=0||amount>(int)(n-used))return 0;used+=(size_t)amount;}return 1;
}

static int c2_role_attach(papinho_c2_s6_context *c,unsigned ch){unsigned char request[16]={0x50,0x41,0x43,0x43,1,0,0,16,0,3,0,0,0,0,0,0};unsigned char ticket[32],attach[32],accept[16],bind[68];PAPACC_C2_VIEW v;int success=0;
 papinho_accelerator_stream_state *s;
 if(!c2_live(c) || ch<1U || ch>2U || c->bound[ch] || !c->root_grants || !c->operation)return 0;
 s=c->channel[ch].stream;
 if(papinho_accel_stream_send_all(c->channel[0].stream,request,16,&c->config,c->cancel)!=0)goto done;
 if(!c2_old_read_exact(c,c->channel[0].stream,ticket,32U))goto done;
 if(memcmp(ticket,"PACC",4)||ticket[9]!=4U||ticket[15]!=16U)goto done;
 memcpy(attach,request,16);attach[9]=5U;attach[15]=16U;memcpy(attach+16,ticket+16,16);
 s->socket=papinho_accel_connect(&c->config,c->cancel);if(s->socket==INVALID_SOCKET)goto done;s->socket_open=1;
 if(papinho_accel_secure_attach(s,&c->config,c->cancel,NULL)!=0||!c2_add_stream(c,ch))goto done;
 if(papinho_accel_stream_send_all(s,attach,32,&c->config,c->cancel)!=0||!c2_old_read_exact(c,s,accept,16U))goto done;
 if(memcmp(accept,"PACC",4)||accept[9]!=6U||accept[15]!=0U)goto done;
 memcpy(bind,c->identity,32);bind[32]=(unsigned char)ch;memset(bind+33,0,3);memcpy(bind+36,c->tokens+(ch-1U)*32U,32U);
 
 if(!c2_queue(c,ch,18U,bind,sizeof(bind)))goto done;papinho_c2_wipe(bind,sizeof(bind));
 if(!c2_read_frame(c,ch,19U,&v)||!c2_accept_bind_ack(c,ch,&v))goto done;
 c2_rx_done(&c->channel[ch]);c2_trace(c,ch==1U?"APPLICATION_BIND_ACK_FULL_RECEIPT":"RECORD_BIND_ACK_FULL_RECEIPT");success=1;
done:
 papinho_c2_wipe(ticket,sizeof(ticket));papinho_c2_wipe(attach,sizeof(attach));papinho_c2_wipe(bind,sizeof(bind));return success;
}

static int c2_fence(papinho_c2_s6_context *c,unsigned flow,int ack){unsigned char p[32];unsigned ch=flow/2U+1U;memset(p,0,sizeof(p));memcpy(p,c->identity,16U);p[16]=(unsigned char)ch;p[17]=(unsigned char)(flow%2U);c2_put64(p+20,ack?c->fences[flow]:c->count[flow]);if(!ack)p[29]=1U;return c2_queue(c,ch,ack?24U:23U,p,sizeof(p));}

static int c2_handle(papinho_c2_s6_context *c,unsigned ch,PAPACC_C2_VIEW *v){const unsigned char *p=v->payload;unsigned flow;PAPACC_U64 n;
 if(!ch){
  if(v->type==21U){if(!c->root_grants||!c->bound[1]||!c->bound[2]||c->tls_ready||memcmp(p,c->identity,24U)||c2_u16(p+24)!=0x0304U||c2_u16(p+26)!=7U||c2_u16(p+28)!=8U||memcmp(p+32,"http/1.1",8U))return 0;c->tls_ready=1;c->receipt.tls_version=(unsigned short)c2_u16(p+24);c->receipt.tls_verification=(unsigned short)c2_u16(p+26);memcpy(c->receipt.selected_alpn,p+32,8U);c->receipt.selected_alpn[8]=0;c2_receipt_once(c,&c->receipt.tls_seq);c2_resource_boundary(c,4U);c2_trace(c,"TLS_READY_FULL_RECEIPT_SITE_TLS13_CHAIN_NAME_AUTH_ALPN");return 1;}
  if(v->type==27U){if(c->final_seen||c2_u64(p)!=c->request||memcmp(p+8,c->identity,24U))return 0;c->final_seen=1;c->remote_fact=(unsigned short)c2_u16(p+32);c->remote_outcome=c2_u16(p+36);c->remote_ownership=c2_u16(p+40);
   if(c->remote_outcome==1){unsigned i;if(c->remote_fact||c2_u16(p+34)!=3U||c2_u16(p+38)!=2U||c->remote_ownership!=1||p[42]!=15U||!c->tls_ready)return 0;for(i=0;i<4U;++i)if(c2_u64(p+44U+8U*i)!=c->count[i]||!((i%2U)?c->ack_written[i]:c->ack_seen[i]))return 0;}
   c2_receipt_once(c,&c->receipt.final_seq);c2_observe(c,C2_CAUSE_REMOTE_FINAL,(unsigned)__LINE__,(long)c->remote_fact);if(c->remote_ownership>=2U)c->retained=1;c2_trace(c,"FINAL_FULL_RECEIPT_DISTINCT_FROM_LOCAL_DESTRUCTION");return 1;}
  if(v->type==26U){unsigned kind=c2_u16(p);if(c->revoked||(kind==1U?c2_u64(p+8)!=c->request:memcmp(p+16,c->identity,24U)!=0))return 0;c2_observe(c,C2_CAUSE_REMOTE_CANCEL,(unsigned)__LINE__,(long)kind);c->revoked=1;c2_trace(c,"REMOTE_CANCEL_AUTHORITY_REVOKED");return 1;}return 0;
 }
 if(v->type==19U)return 0;if(memcmp(p,c->identity,16U)||p[16]!=ch)return 0;flow=(ch-1U)*2U+p[17];n=c2_u64(p+20);
 if(v->type==22U){size_t amount=v->length-28U;if(c->revoked||c->retained||c->fence_seen[flow]||p[17]!=1U||n!=c->count[flow]||(ch==1U&&!c->tls_ready))return 0;
  if(ch==1U){if(amount>C2_HTTP_CAP-c->http_size)return 0;memcpy(c->http+c->http_size,p+28,amount);c->http_size+=amount;}else{if(amount>C2_FLOW_CAP-c->record_ab_size)return 0;memcpy(c->record_ab+c->record_ab_size,p+28,amount);c->record_ab_size+=amount;}
  if(!c2_count_add(&c->count[flow],amount))return 0;
  c->receipt.prefix_seq=c2_receipt_tick(c);if(c->tls_ready)c2_resource_boundary(c,5U);
  if(ch==1U) {
   c2_trace(c,"HTTP_APPLICATION_PREFIX_ACCEPTED_BEFORE_FINAL");
   if(c->final_seen) return 0;
  }
  return 1;}
 if(v->type==24U){if(p[17]||!c->fence_sent[flow]||c->ack_seen[flow]||n!=c->count[flow])return 0;c->ack_seen[flow]=1;c->receipt.ack_values[flow]=n;c2_receipt_once(c,&c->receipt.ack_seq[flow]);return 1;}
 if(v->type==23U){if(p[17]!=1U||c->fence_seen[flow]||n!=c->count[flow]||(ch==1U&&!c->tls_ready))return 0;c->fence_seen[flow]=1;c->fences[flow]=n;c->receipt.fence_values[flow]=n;c2_receipt_once(c,&c->receipt.fence_seq[flow]);if(ch==1U){c->freeze_ba=1;if(!c->disposition_started_valid){c->disposition_started=GetTickCount();c->disposition_started_valid=1;}c2_trace(c,"APP_AB_FENCE_RECEIVED_FREEZE_BA_PRESERVE_AB");}return 1;}
 return 0;
}

static int c2_close_roles(papinho_c2_s6_context *c){unsigned i;DWORD start=GetTickCount();int done;
 if(!c->disposition_started_valid){c->disposition_started=start;c->disposition_started_valid=1;}
 fprintf(stderr,"GLOBAL_ORACLE event=GRACEFUL_BEGIN tick=%lu navigation=%lu operation=%I64u generation=%I64u pid=%lu\n",start,c->navigation,c->operation,c->generation,GetCurrentProcessId());
 while((DWORD)(GetTickCount()-start)<5000U){done=1;for(i=1U;i<=2U;++i){C2_ROLE *r=&c->channel[i];pst_u32 operation;PST_RESULT error;if(r->shutdown_done)continue;done=0;
  /* Accelerator owns role completion after its factual ACK receipt. Wait for
   * its real clean link close_notify before reciprocal shutdown, rather than
   * racing EOF into a still-active protocol slot after only lower ACK acceptance. */
  if(!r->eof){PAPACC_C2_VIEW unexpected;int read=c2_read_once(c,i,&unexpected);if(read<0||read>0)return 0;if(!r->eof)continue;
   c2_trace(c,i==1U?"APPLICATION_LINK_CLEAN_PEER_CLOSE_NOTIFY":"RECORD_LINK_CLEAN_PEER_CLOSE_NOTIFY");}

  {PST_RESULT actual;operation=0U;error=PST_RESULT_OK;actual=pst_connection_shutdown(r->stream->secure_connection,&operation,&error);
   fprintf(stderr,"S6_ROLE_SHUTDOWN role=%u operation_id=%I64u generation=%I64u connection=%p result=%ld operation=%lu source_error=%ld\n",i,c->operation,c->generation,(void*)r->stream->secure_connection,(long)actual,(unsigned long)operation,(long)error);
   if(actual!=PST_RESULT_OK||operation==PST_OPERATION_FAILED)return 0;}

  if(operation==PST_OPERATION_COMPLETE||operation==PST_OPERATION_CLOSED)r->shutdown_done=1;
 }if(done)return 1;if(!c2_wait(c,100U))return 0;}return 0;
}

static void c2_terminal_notice(papinho_c2_s6_context*c,unsigned type,unsigned fact){
 unsigned char p[40];unsigned channel=type==26U?0U:2U;C2_ROLE*r;PAPACC_SIZE written;DWORD start;
 if(!c||c->retained||!c->operation||!c->root_grants||!c->channel[channel].stream||!c->channel[channel].stream->secure_connection)return;
 r=&c->channel[channel];
 if(r->tx_size){
  if(r->tx_sent==0U&&!r->tx_attempted){papinho_c2_wipe(r->tx,r->tx_size);r->tx_size=0U;r->type=0U;c2_trace(c,"UNSUBMITTED_FRAME_DISCARDED_NO_PREFIX_ESCAPED");}
  else{c2_trace(c,"TERMINAL_NOTICE_UNSENT_EXISTING_PARTIAL_FRAME_PRESERVED");return;}
 }
 memset(p,0,sizeof(p));if(type==26U){p[1]=2U;c2_put16(p+2,fact);p[5]=1U;memcpy(p+16,c->identity,24U);}
 else{memcpy(p,c->identity,16U);p[16]=2U;c2_put64(p+20,c->count[2]);p[29]=(unsigned char)fact;}
 if(papacc_c2_encode_frame((PAPACC_U16)type,p,type==26U?40U:32U,channel?PAPACC_C2_ROLE_CHANNEL:PAPACC_C2_CONTROL,PAPACC_C2_BROWSER,r->tx,sizeof(r->tx),&written)!=0)return;
 r->tx_size=written;r->tx_sent=0;r->tx_attempted=0;r->type=type;c->revoked=1;start=GetTickCount();
 while(r->tx_size&&(DWORD)(GetTickCount()-start)<500U){if(!c2_write_once(c,channel))break;if(r->tx_size&&!c2_wait(c,50U))break;}
 if(!r->tx_size){if(type==25U)c->record_event_sent=1;c2_trace(c,type==26U?"LOCAL_CANCEL_FULL_LOWER_ACCEPTANCE_NOT_PEER_RECEIPT":"NATIVE_SITE_FAILURE_FACT_FULL_LOWER_ACCEPTANCE_NOT_PEER_RECEIPT");}
 else c2_trace(c,"TERMINAL_NOTICE_UNSENT_BOUNDED_WAIT_NOT_SUCCESS");
}

void papinho_c2_s6_abandon(papinho_c2_s6_context *c){
 papinho_session_storage *s;int ok=1;unsigned i;
 if(!c || c->thread!=GetCurrentThreadId() || c->local_cleanup_done)return;
 c->local_cleanup_done=1;c->revoked=1;c2_receipt_once(c,&c->receipt.stop_seq);c2_resource_boundary(c,6U);
 if(c->site){papinho_site_status status;
  if(papinho_site_close(c->site)!=0)ok=0;
  papinho_site_status_get(c->site,&status);c->site_close_attempts=(unsigned long)status.close_attempts;
  c->site_closed=status.close_completed;
  if(status.close_completed && papinho_site_unref(c->site))c->site=NULL;else ok=0;
 }else c->site_closed=1;
 if(!candidate_native_close(&c->cancel,&c->cancel_close))ok=0;
 if(c->session){s=candidate_storage(c->session);
  /* This owner has returned from the last C2 I/O/wait and owns every view.
   * No C2 context/Session is ever published to UI or another PST executor. */
  if(s)s->c2_executing=0;
  if(c->retained || !ok){papinho_candidate_session_retain(c->session);ok=0;}
  else{
   int disposed=candidate_session_teardown(c->session);
   c->release_attempts=s->c2_release_attempts;c2_receipt_session(c,s);
   if(disposed && papinho_candidate_session_finish(c->session)){
    c->receipt.session_finished=1;c->session=NULL;c->waitset=NULL;for(i=0;i<3;i++)c->channel[i].stream=NULL;
   }else ok=0;
  }
 }
 if(c->session)c2_receipt_session(c,candidate_storage(c->session));
 c2_receipt_once(c,&c->receipt.teardown_seq);
 if(!ok){c->retained=1;c2_resource_boundary(c,7U);InterlockedExchange(&papinho_pst_admission_stopped,1);return;}
 c->local_destroyed=1;c2_resource_boundary(c,7U);
 papinho_c2_wipe(c->tokens,sizeof(c->tokens));
 for(i=0;i<3;i++){papinho_c2_wipe(c->channel[i].rx,sizeof(c->channel[i].rx));papinho_c2_wipe(c->channel[i].tx,sizeof(c->channel[i].tx));}
}
static papinho_c2_s6_context *c2_prepare_observed(const papinho_accelerator_client_config *config,HANDLE cancel,unsigned long navigation,papinho_accelerator_error *error,const papinho_c2_s6_facts *baseline){
 papinho_c2_s6_context *c;papinho_session_storage *s;size_t reserve;
 if(error)*error=PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE;
 if(!config || !config->candidate_group || !config->secure_runtime || !cancel || !navigation ||
    config->profile!=PAPINHO_ACCELERATOR_TRANSPORT_SECURE_PRINCIPAL || !papinho_c2_s6_enabled()){if(baseline)(void)c2_receipt_emit(baseline,"NATIVE");return NULL;}
 reserve=sizeof(*c)+sizeof(papinho_session_storage)+sizeof(papinho_accel_pst_group)+
  4U*sizeof(papinho_accelerator_persistent_request_job)+3U*sizeof(papinho_pst_owner)+
  2U*C2_HTTP_CAP+65536U; /* shared parent lease/store charged once, outside child increment */
 if(!c2_reserve(reserve)){if(baseline)(void)c2_receipt_emit(baseline,"NATIVE");return NULL;}
 c=(papinho_c2_s6_context *)calloc(1,sizeof(*c));if(!c){c2_refund(reserve);if(baseline)(void)c2_receipt_emit(baseline,"NATIVE");return NULL;}
 if(baseline)c->receipt=*baseline;else{c2_native_observe(&c->receipt);c->receipt.resource_scope=1U;c2_resource_phase(&c->receipt,0U,0,0,0,0);}
 c->reserved=reserve;c->thread=GetCurrentThreadId();c->cause.owner_thread=c->thread;c->navigation=navigation;
 c->config=*config;c->config.candidate_c2=1;c->config.network_egress_v1_fallback_allowed=0;
 papinho_accel_stream_init(&c->record);
 if(!DuplicateHandle(GetCurrentProcess(),cancel,GetCurrentProcess(),&c->cancel,0,FALSE,DUPLICATE_SAME_ACCESS))goto fail;
 c->session=papinho_candidate_session_create();if(!c->session)goto fail;
 s=candidate_storage(c->session);s->c2_record=&c->record;c2_resource_boundary(c,1U);
 if(papinho_accelerator_persistent_session_establish(c->session,&c->config,c->cancel,error)!=0)goto fail;
 c->config.candidate_group=s->group;c->config.secure_runtime=c->session->secure_runtime;
 c->config.secure_credentials=c->session->secure_credentials;c->config.secure_trust=c->session->secure_trust;
 c->channel[0].stream=&c->session->runtime.control;c->channel[1].stream=&c->session->runtime.data;c->channel[2].stream=&c->record;
 if(pst_wait_set_create(&c->waitset)!=PST_RESULT_OK)goto fail;
 c->session->wait_set=c->waitset;s->group->wait_set=c->waitset;
 if(!c2_add_stream(c,0U))goto fail;
 s->c2_executing=1;c->root_grants=1;c->started=c->last_progress=GetTickCount();c2_resource_boundary(c,2U);return c;
fail:
 papinho_c2_s6_abandon(c);
 c2_receipt_copy(c,&c->receipt);(void)c2_receipt_emit(&c->receipt,"NATIVE");
 if(c->retained){c->next=c2_retained;c2_retained=c;}else{c2_refund(c->reserved);papinho_c2_wipe(c,sizeof(*c));free(c);}
 return NULL;
}

papinho_c2_s6_context *papinho_c2_s6_prepare(const papinho_accelerator_client_config *config,HANDLE cancel,unsigned long navigation,papinho_accelerator_error *error){
 return c2_prepare_observed(config,cancel,navigation,error,NULL);
}

int papinho_c2_s6_http(papinho_c2_s6_context *c,const char *hostname,unsigned short port,const unsigned char *request,size_t request_size,unsigned char **out,size_t *out_size,papinho_c2_s6_facts *facts,papinho_accelerator_error *error){
 unsigned char payload[803],original[255],canonical[254];size_t original_size,canonical_size,total;unsigned i;PAPACC_C2_VIEW v;DWORD phase;int app_sent=0,clean_roles=0,observed_deadline=0;
 if(out)*out=NULL;if(out_size)*out_size=0;if(facts)memset(facts,0,sizeof(*facts));if(error)*error=PAPINHO_ACCELERATOR_ERROR_PROTOCOL;
 if(!c2_live(c)||!hostname||!request||!request_size||request_size>4096U||!out||!out_size||!facts)return -1;
 

 if(!c2_endpoint(c,hostname,port))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);original_size=strlen(hostname);if(original_size>254U)return -1;memcpy(original,hostname,original_size);canonical_size=original_size;if(canonical_size&&original[canonical_size-1U]=='.')--canonical_size;if(!canonical_size||canonical_size>253U)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
 for(i=0;i<canonical_size;++i)canonical[i]=(unsigned char)((original[i]>='A'&&original[i]<='Z')?original[i]+32U:original[i]);
 memset(payload,0,sizeof(payload));c->request=1U;c2_put64(payload,c->request);c2_put32(payload+8,c->navigation);c2_put32(payload+12,c->endpoint.resolution_generation);payload[16]=payload[17]=1U;c2_put16(payload+18,port);memcpy(payload+20,c->endpoint.numeric_address,4);c2_put16(payload+24,(unsigned)original_size);c2_put16(payload+26,(unsigned)canonical_size);payload[32]=payload[33]=1U;c2_put16(payload+34,9U);total=40U;memcpy(payload+total,original,original_size);total+=original_size;memcpy(payload+total,canonical,canonical_size);total+=canonical_size;payload[total++]=8U;memcpy(payload+total,"http/1.1",8U);total+=8U;
 if(!c2_queue(c,0,16U,payload,total)||!c2_read_frame(c,0,17U,&v)||!c2_accept_open_result(c,&v,payload+8))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
 c2_rx_done(&c->channel[0]);
 if(c2_s6_identity==0xffffffffUL)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);c->site_token.identity=++c2_s6_identity;c->site_token.generation=c->navigation;c->site=papinho_site_create(&c->endpoint,c->site_token,60000U);if(!c->site)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);phase=GetTickCount();
 for(;;){papinho_site_status status;papinho_site_status_get(c->site,&status);if(status.connected)break;if(status.terminal!=PAPINHO_SITE_ACTIVE||(DWORD)(GetTickCount()-phase)>=5000U||papinho_accel_cancelled(c->cancel))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);if(papinho_site_wait(c->site,c->site_token,PAPINHO_SITE_CONNECT,100U)<0)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
 c2_trace(c,"BROWSER_C1_NUMERIC_SITE_SOCKET_CONNECTED");
 if(!c2_role_attach(c,2U)||!c2_role_attach(c,1U)||!c2_queue(c,0,20U,c->identity,32U))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
 c2_trace(c,"SITE_READY_AFTER_C1_CONNECT_AND_BOTH_FULL_ACKS");
 phase=GetTickCount();
 while(!c->final_seen){
  int rc,n,interests;unsigned char p[16384];
  if(c->disposition_started_valid && (DWORD)(GetTickCount()-c->disposition_started)>=5000U){observed_deadline=1;goto failed;}
  if(papinho_accel_cancelled(c->cancel)){c2_observe(c,C2_CAUSE_CANCEL,(unsigned)__LINE__,0);if(error)*error=PAPINHO_ACCELERATOR_ERROR_CANCELLED;do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  if((!c->tls_ready&&(DWORD)(GetTickCount()-phase)>=10000U)||(DWORD)(GetTickCount()-c->last_progress)>=30000U){observed_deadline=1;c2_observe(c,C2_CAUSE_DEADLINE,(unsigned)__LINE__,0);fprintf(stderr,"C7_LOCAL_DEADLINE pid=%lu operation=%I64u generation=%I64u tls_ready=%d phase_start=%lu last_actual_progress=%lu observed=%lu\n",GetCurrentProcessId(),c->operation,c->generation,c->tls_ready,phase,c->last_progress,GetTickCount());if(error)*error=PAPINHO_ACCELERATOR_ERROR_TIMEOUT;do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  if(!c2_capacity()){if(error)*error=PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE;goto failed;}
  
  /* CONTROL receives one opportunity before bounded DATA service. */
  rc=c2_read_once(c,0U,&v);if(rc<0||c->channel[0].eof)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);if(rc){if(!c2_handle(c,0U,&v))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);c2_rx_done(&c->channel[0]);}
  
  if(c->final_seen)break;if(c->revoked)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
  if(c->record_ab_size>=49152U)c->record_read_paused=1;else if(c->record_ab_size<=24576U)c->record_read_paused=0;
  for(i=1U;i<=2U;++i){if(i==2U&&c->record_read_paused)continue;rc=c2_read_once(c,i,&v);if(rc<0||(c->channel[i].eof&&!clean_roles&&!(c->ack_seen[0]&&c->ack_seen[2]&&c->ack_written[1]&&c->ack_written[3]&&!c->channel[1].tx_size&&!c->channel[2].tx_size)))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);if(rc){if(!c2_handle(c,i,&v))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);c2_rx_done(&c->channel[i]);}}
  interests=0;if(!c->freeze_ba&&!c->site_eof&&!c->channel[2].tx_size)interests|=PAPINHO_SITE_READ;if(c->record_ab_size)interests|=PAPINHO_SITE_WRITE;
  rc=papinho_site_wait(c->site,c->site_token,interests,0U);if(rc<0)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
  if(c->record_ab_size){n=papinho_site_send(c->site,c->site_token,c->record_ab,(int)c->record_ab_size);if(n>0){memmove(c->record_ab,c->record_ab+n,c->record_ab_size-(size_t)n);c->record_ab_size-=(size_t)n;if(!c2_count_add(&c->site_ab_accepted,(size_t)n))goto failed;c->last_progress=GetTickCount();}else if(n!=PAPINHO_SITE_WOULD_BLOCK)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  if(!c->freeze_ba&&!c->site_eof&&!c->channel[2].tx_size){n=papinho_site_recv(c->site,c->site_token,p+28,16356);if(n>0){memcpy(p,c->identity,16);p[16]=2;p[17]=0;p[18]=p[19]=0;c2_put64(p+20,c->count[2]);if(!c2_queue(c,2,22U,p,28U+(size_t)n))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}else if(n==0){c2_observe(c,C2_CAUSE_SITE_EOF,(unsigned)__LINE__,0);c->site_eof=1;}else if(n!=PAPINHO_SITE_WOULD_BLOCK)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  if(c->tls_ready&&!app_sent&&!c->channel[1].tx_size){size_t offset=(size_t)c->count[0],amount=request_size-offset;if(amount>16356U)amount=16356U;memcpy(p,c->identity,16);p[16]=1;p[17]=0;p[18]=p[19]=0;c2_put64(p+20,c->count[0]);memcpy(p+28,request+offset,amount);if(!c2_queue(c,1,22U,p,28U+amount))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);app_sent=offset+amount==request_size;}
  if(app_sent&&c->count[0]==request_size&&!c->fence_sent[0]&&!c->channel[1].tx_size){if(!c2_fence(c,0,0))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  if(c->fence_seen[1]&&!c->ack_written[1]&&!c->channel[1].tx_size){if(!c2_fence(c,1,1))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  if(c->site_eof&&!c->record_event_sent&&!c->channel[2].tx_size){memset(p,0,32);memcpy(p,c->identity,16);p[16]=2;c2_put64(p+20,c->count[2]);p[29]=1;if(!c2_queue(c,2,25U,p,32))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);c->record_event_sent=1;}
  if(c->freeze_ba&&!c->fence_sent[2]&&!c->channel[2].tx_size&&1){if(!c2_fence(c,2,0))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  if(c->fence_seen[3]&&c->site_ab_accepted==c->fences[3]&&!c->record_ab_size&&!c->ack_written[3]&&!c->channel[2].tx_size){if(!c2_fence(c,3,1))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);}
  for(i=0U;i<3U;++i)if(!c2_write_once(c,i))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
  if(!clean_roles&&c->ack_seen[0]&&c->ack_seen[2]&&c->ack_written[1]&&c->ack_written[3]&&!c->channel[1].tx_size&&!c->channel[2].tx_size){if(!c2_close_roles(c))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);clean_roles=1;c2_trace(c,"FOUR_FENCES_AND_ROLE_TLS_SHUTDOWN_FACTUAL");}
  if(!c2_wait(c,100U))do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
 }
 if(c->remote_outcome!=1||!clean_roles)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);
 *out=(unsigned char*)malloc(c->http_size+1U);if(!*out)do{c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);fprintf(stderr,"S6_HTTP_BOUNDARY_FAIL line=%u operation=%I64u generation=%I64u\n",(unsigned)__LINE__,c->operation,c->generation);goto failed;}while(0);memcpy(*out,c->http,c->http_size);(*out)[c->http_size]=0;*out_size=c->http_size;
 papinho_c2_s6_abandon(c);c2_receipt_copy(c,facts);
 if(!facts->local_destroyed){c2_observe(c,C2_CAUSE_CLEANUP_RETAINED,(unsigned)__LINE__,0);if(error)*error=PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE;free(*out);*out=NULL;*out_size=0;c2_publish_cause(c,error?(int)*error:-1);c2_receipt_copy(c,facts);return -1;}c2_observe(c,C2_CAUSE_COMPLETE,(unsigned)__LINE__,0);if(error)*error=PAPINHO_ACCELERATOR_ERROR_NONE;c2_publish_cause(c,PAPINHO_ACCELERATOR_ERROR_NONE);c2_receipt_copy(c,facts);return 0;
failed:
 if(!c->cause.first_cause)c2_observe(c,C2_CAUSE_BOUNDARY,(unsigned)__LINE__,error?(long)*error:0);
 fprintf(stderr,"GLOBAL_ORACLE event=FAILURE_ENTRY tick=%lu navigation=%lu operation=%I64u generation=%I64u pid=%lu error_before_cancel_action=%d cancelled=%d\n",GetTickCount(),c->navigation,c->operation,c->generation,GetCurrentProcessId(),error?(int)*error:-1,papinho_accel_cancelled(c->cancel));
 if(papinho_accel_cancelled(c->cancel)){c2_observe(c,C2_CAUSE_CANCEL,(unsigned)__LINE__,0);if(error)*error=PAPINHO_ACCELERATOR_ERROR_CANCELLED;c2_terminal_notice(c,26U,1U);}
 else if(observed_deadline)c2_terminal_notice(c,26U,3U);
 else if(c->site&&!c->revoked&&!c->record_event_sent){papinho_site_status native;papinho_site_status_get(c->site,&native);
  if(native.terminal==PAPINHO_SITE_RESET||native.terminal==PAPINHO_SITE_SOCKET_ERROR){
   if(error)*error=PAPINHO_ACCELERATOR_ERROR_TRANSPORT;
   /* RECORD_EVENT requires factual peer activation. Before any received
    * RECORD AB, cancel the live operation; never finish a revoked SITE_READY
    * or invent permission to emit native facts on an unactivated role. */
   if(c->count[3]>0U)c2_terminal_notice(c,25U,native.terminal==PAPINHO_SITE_RESET?2U:3U);
   else{c2_trace(c,"PRE_ACTIVATION_NATIVE_FAILURE_LOCAL_FACT_PRESERVED_REQUEST_GOVERNED_CANCEL");c2_terminal_notice(c,26U,1U);}
  }
 }
 papinho_c2_s6_abandon(c);c2_publish_cause(c,error?(int)*error:-1);c2_receipt_copy(c,facts);return -1;
}


/* One public value projection through the existing owner stderr producer.
 * No file/collector/owner is created. External provenance remains absent. */
static int c2_receipt_emit(const papinho_c2_s6_facts *f,const char *origin){
 char birth[32],thread_birth[32],image_path[6U*MAX_PATH+1U],resources[4096];
 if(!f)return 0;
 strcpy(birth,"null");strcpy(thread_birth,"null");
 if(f->native.presence&1U)_snprintf(birth,sizeof(birth),"%I64u",f->native.creation);
 if(f->native.presence&2U)_snprintf(thread_birth,sizeof(thread_birth),"%I64u",f->native.thread_creation);
 c2_native_escape(f->native.image_path,image_path);
 if(!c2_resource_json(f,resources,sizeof(resources)))return 0;
 return fprintf(stderr,"MODE_C_MINIMAL_RECEIPT {\"format\":\"BROWSER_C2_PUBLIC_REFERENCES\",\"version\":1,\"origin\":\"%s\",\"creation\":%s,\"image\":null,\"sdk\":null,\"build_ref\":null,\"producer_join\":null,\"process\":%lu,\"owner_thread\":%lu,\"session_generation\":%lu,\"group_serial\":%lu,\"request\":%I64u,\"operation\":%I64u,\"generation\":%I64u,\"epoch\":%I64u,\"attempt\":%lu,\"resolution\":%lu,\"open_result_seq\":%lu,\"tls_seq\":%lu,\"prefix_seq\":%lu,\"final_seq\":%lu,\"stop_seq\":%lu,\"teardown_seq\":%lu,\"first_cause_seq\":%lu,\"publication_seq\":%lu,\"retained\":%d,\"local_destroyed\":%d,\"site_closed\":%d,\"cancel_closed\":%d,\"session_quiesced\":%d,\"auxiliary_confirmed\":%d,\"group_destroyed\":%d,\"session_finished\":%d,\"capture_incomplete\":%d,\"remote_outcome\":%d,\"remote_ownership\":%d,\"site_close_attempts\":%lu,\"pst_release_attempts\":%lu,\"live_borrowers\":%lu,\"tls_version\":%u,\"verification\":%u,\"alpn\":\"%s\",\"prefixes\":[%I64u,%I64u,%I64u,%I64u],\"bind_seq\":[%lu,%lu],\"bind_ack_seq\":[%lu,%lu],\"fence_seq\":[%lu,%lu,%lu,%lu],\"ack_seq\":[%lu,%lu,%lu,%lu],\"fence_values\":[%I64u,%I64u,%I64u,%I64u],\"ack_values\":[%I64u,%I64u,%I64u,%I64u],\"stop_attempted\":%d,\"bridge_joined\":%d,\"bridge_join_result\":%lu,\"bridge_close_succeeded\":%d,\"cancel_attempted\":%d,\"cancel_succeeded\":%d,\"native\":{\"presence\":%lu,\"thread_creation\":%s,\"mapped_base\":%lu,\"image_path\":\"%s\",\"root_kind\":\"PST_PROCESS_ROOT\",\"root_identity\":%lu,\"root_state\":%lu,\"sequence\":%lu,\"clock\":\"WIN32_GETTICKCOUNT_U32\",\"observed_tick\":%lu,\"validated_tick\":%lu,\"valid_until\":null,\"configuration_root\":null,\"policy_revision\":null,\"sdk_manifest\":\"%s\",\"build_debug\":%d,\"error\":%lu},\"resources\":%s,\"release\":[[%lu,%lu,%lu,%ld,%lu,%d,%d,%d],[%lu,%lu,%lu,%ld,%lu,%d,%d,%d],[%lu,%lu,%lu,%ld,%lu,%d,%d,%d]]}\n",
  origin,birth,f->process_id,f->owner_thread,f->session_generation,f->group_serial,f->request,
  f->operation,f->generation,f->epoch,f->attempt,f->resolution,f->open_result_seq,
  f->tls_seq,f->prefix_seq,f->final_seq,f->stop_seq,f->teardown_seq,f->first_cause_seq,
  f->publication_seq,f->retained,f->local_destroyed,f->site_closed,f->cancel_closed,f->session_quiesced,
  f->auxiliary_confirmed,f->group_destroyed,f->session_finished,f->capture_incomplete,f->remote_outcome,f->remote_ownership,
  f->site_close_attempts,f->pst_release_attempts,f->live_borrowers,(unsigned)f->tls_version,(unsigned)f->tls_verification,f->selected_alpn,f->prefixes[0],f->prefixes[1],
  f->prefixes[2],f->prefixes[3],f->bind_seq[0],f->bind_seq[1],f->bind_ack_seq[0],f->bind_ack_seq[1],
  f->fence_seq[0],f->fence_seq[1],f->fence_seq[2],f->fence_seq[3],f->ack_seq[0],f->ack_seq[1],
  f->ack_seq[2],f->ack_seq[3],f->fence_values[0],f->fence_values[1],f->fence_values[2],f->fence_values[3],f->ack_values[0],f->ack_values[1],f->ack_values[2],f->ack_values[3],f->stop_attempted,f->bridge_joined,f->bridge_join_result,f->bridge_close_succeeded,f->cancel_attempted,f->cancel_succeeded,
  f->native.presence,thread_birth,f->native.mapped_base,image_path,f->native.root_identity,f->native.root_state,f->native.sequence,f->native.observed_tick,f->native.validated_tick,C2_NATIVE_SDK_MANIFEST,PAPINHO_C2_NATIVE_BUILD_DEBUG,f->native.error,resources,
  f->release[0].identity,f->release[0].generation,f->release[0].attempts,f->release[0].result,
  f->release[0].disposition,f->release[0].quiesced,f->release[0].finalized,f->release[0].membership_absent,f->release[1].identity,f->release[1].generation,
  f->release[1].attempts,f->release[1].result,f->release[1].disposition,f->release[1].quiesced,f->release[1].finalized,f->release[1].membership_absent,
  f->release[2].identity,f->release[2].generation,f->release[2].attempts,f->release[2].result,f->release[2].disposition,f->release[2].quiesced,
  f->release[2].finalized,f->release[2].membership_absent)>=0;
}
/* Existing offline controller selftest calls this same receipt producer.
 * Baseline-only: no OPEN/operation/epoch, grant, PST Connection or network. */
int papinho_candidate_native_binding_selftest(void){
 papinho_accelerator_persistent_session *v;papinho_session_storage *s;
 papinho_c2_s6_facts f;int finished;
 ZeroMemory(&f,sizeof(f));f.resource_scope=2U;c2_resource_phase(&f,0U,0,0,0,0);
 v=papinho_candidate_session_create();if(!v)return 1;
 s=candidate_storage(v);f.session_generation=s->generation;f.group_serial=s->group->serial;
 c2_native_observe(&f);c2_resource_phase(&f,1U,0,0,0,0);c2_resource_phase(&f,6U,0,0,1,0);
 finished=papinho_candidate_session_finish(v);f.session_finished=finished;f.retained=!finished;
 c2_resource_phase(&f,7U,0,!finished,1,finished);
 if(!c2_receipt_emit(&f,"NATIVE"))return 1;
 return !finished || (f.native.presence&15U)!=15U;
}
static int candidate_c2_execute(papinho_accelerator_persistent_session *parent,papinho_accelerator_persistent_request_job *job,papinho_accelerator_mode_b_request *request,papinho_accelerator_error *error){
 papinho_accelerator_client_config cfg=parent->config;papinho_c2_s6_context *c;
 papinho_c2_s6_facts facts;unsigned char *bytes=NULL;size_t size=0;int rc;
 cfg.candidate_group=candidate_storage(parent)->group;cfg.secure_runtime=parent->secure_runtime;
 cfg.secure_credentials=parent->secure_credentials;cfg.secure_trust=parent->secure_trust;
 ZeroMemory(&facts,sizeof(facts));c2_native_observe(&facts);facts.resource_scope=1U;
 c2_resource_phase(&facts,0U,0,0,0,0);
 c=c2_prepare_observed(&cfg,parent->active_operation_cancel_event?parent->active_operation_cancel_event:job->cancel_event,job->c2_navigation,error,&facts);
 if(!c){if(root_read(&papinho_pst_admission_stopped))papinho_candidate_session_retain(parent);return -1;}
 c2_receipt_once(c,&c->receipt.native.sequence);
 rc=papinho_c2_s6_http(c,request->destination,request->port,request->request_bytes,request->request_size,&bytes,&size,&facts,error);
 /* Also covers argument/size rejection before HTTP enters its failure label.
  * A live child/RECORD alias must never outlive a freed C2 context. */
 papinho_c2_s6_abandon(c);
 c2_receipt_copy(c,&facts);(void)c2_receipt_emit(&facts,"NATIVE");
 if(c->retained || !c->local_destroyed){rc=-1;if(error)*error=PAPINHO_ACCELERATOR_ERROR_UNAVAILABLE;}
 if(rc==0){
  /* Transfer the independent result allocation to the existing job owner. */
  papinho_accel_response_storage_release(&job->response_storage);
  job->response_storage.data=bytes;job->response_storage.capacity=size+1U;
  job->response_storage.size=size;job->response_storage.max_size=C2_HTTP_CAP;
  request->response_size=size;bytes=NULL;
 }
 free(bytes);
 if(c->retained){c->next=c2_retained;c2_retained=c;papinho_candidate_session_retain(parent);}
 else{
  /* All C2 native sources ended; independent result remains charged until
   * the job's last caller returns. No UI payload borrows this context. */
  EnterCriticalSection(&parent->job_lock);job->c2_reserved=c->reserved;LeaveCriticalSection(&parent->job_lock);
  papinho_c2_wipe(c,sizeof(*c));free(c);
 }
 return rc;
}
