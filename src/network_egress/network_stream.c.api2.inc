#include "network_stream.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static PAPACC_BOOL stream_trace_enabled(void){const char*v=getenv("PAPACC_PST_RECIPROCAL_TRACE");return v!=NULL&&v[0]=='1'&&v[1]==0?PAPACC_TRUE:PAPACC_FALSE;}
static PAPACC_BOOL valid(const PAPACC_NETWORK_STREAM_MANAGER *m){return(m!=NULL&&m->initialized==PAPACC_TRUE&&m->count<=m->capacity&&(m->capacity==0U||m->storage!=NULL))?PAPACC_TRUE:PAPACC_FALSE;}
static PAPACC_BOOL io_valid(const PAPACC_NETWORK_STREAM_IO *io){return(io!=NULL&&io->context!=NULL&&io->read!=NULL&&io->write!=NULL&&io->half_close!=NULL&&io->close!=NULL)?PAPACC_TRUE:PAPACC_FALSE;}
static void close_transport(PAPACC_NETWORK_STREAM*s){if(s->io.close!=NULL){if(stream_trace_enabled()==PAPACC_TRUE){(void)fprintf(stderr,"PST_RECIPROCAL_TRACE event=stream_transport_close stream=%llu state=%d remote_closed=%d terminal_signalled=%d\n",(unsigned long long)s->stream_instance_id,(int)s->state,(int)s->remote_half_closed,(int)s->remote_half_close_signalled);(void)fflush(stderr);}s->io.close(s->io.context);s->io.close=NULL;}}
PAPACC_RESULT papacc_network_stream_manager_init(PAPACC_NETWORK_STREAM_MANAGER *m,
    PAPACC_NETWORK_STREAM *s,PAPACC_U8 *tx,PAPACC_U8 *rx,PAPACC_SIZE cap,
    PAPACC_SIZE per,PAPACC_SIZE bcap){PAPACC_SIZE i;if(m==NULL||(cap>0U&&(s==NULL||tx==NULL||rx==NULL||per==0U||bcap==0U)))return PAPACC_RESULT_INVALID_ARGUMENT;if(m->initialized==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
    for(i=0U;i<cap;++i){s[i]=(PAPACC_NETWORK_STREAM)PAPACC_NETWORK_STREAM_INITIALIZER;s[i].to_remote=tx+i*bcap;s[i].to_client=rx+i*bcap;s[i].buffer_capacity=bcap;}
    m->storage=s;m->capacity=cap;m->count=0U;m->per_session_limit=per;m->buffer_capacity=bcap;m->cursor=0U;m->next_instance_id=1U;m->next_schedule_order=1U;m->last_session_id=0U;m->initialized=PAPACC_TRUE;return PAPACC_RESULT_OK;}
static PAPACC_SIZE owner_count(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 sid){PAPACC_SIZE i,n=0U;for(i=0U;i<m->capacity;++i)if(m->storage[i].state!=PAPACC_NETWORK_STREAM_UNUSED&&m->storage[i].owner_session_instance_id==sid)++n;return n;}
PAPACC_RESULT papacc_network_stream_reserve(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 sid,PAPACC_U64 cid,PAPACC_NETWORK_TRANSPORT_MODE mode,PAPACC_NETWORK_STREAM **out){PAPACC_SIZE i;if(out!=NULL)*out=NULL;if(valid(m)!=PAPACC_TRUE||out==NULL||sid==0U||cid==0U||(mode!=PAPACC_NETWORK_TRANSPORT_TCP_PLAIN&&mode!=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED))return PAPACC_RESULT_INVALID_ARGUMENT;if(m->count>=m->capacity||owner_count(m,sid)>=m->per_session_limit)return PAPACC_RESULT_LIMIT_EXCEEDED;
    for(i=0U;i<m->capacity;++i){if(m->storage[i].state!=PAPACC_NETWORK_STREAM_UNUSED&&m->storage[i].data_channel_instance_id==cid)return PAPACC_RESULT_INVALID_STATE;}
    for(i=0U;i<m->capacity;++i)if(m->storage[i].state==PAPACC_NETWORK_STREAM_UNUSED){PAPACC_U8 *tx=m->storage[i].to_remote,*rx=m->storage[i].to_client;PAPACC_SIZE bc=m->storage[i].buffer_capacity;m->storage[i]=(PAPACC_NETWORK_STREAM)PAPACC_NETWORK_STREAM_INITIALIZER;m->storage[i].to_remote=tx;m->storage[i].to_client=rx;m->storage[i].buffer_capacity=bc;m->storage[i].owner_session_instance_id=sid;m->storage[i].data_channel_instance_id=cid;m->storage[i].transport_mode=mode;m->storage[i].state=PAPACC_NETWORK_STREAM_RESERVED;++m->count;*out=&m->storage[i];return PAPACC_RESULT_OK;}return PAPACC_RESULT_INTERNAL_ERROR;}
PAPACC_RESULT papacc_network_stream_publish(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_NETWORK_STREAM *s,const PAPACC_NETWORK_STREAM_IO *io){if(valid(m)!=PAPACC_TRUE||s==NULL||io_valid(io)!=PAPACC_TRUE||s->state!=PAPACC_NETWORK_STREAM_RESERVED)return PAPACC_RESULT_INVALID_ARGUMENT;s->stream_instance_id=m->next_instance_id++;if(m->next_instance_id==0U)m->next_instance_id=1U;s->io=*io;s->state=PAPACC_NETWORK_STREAM_OPEN;return PAPACC_RESULT_OK;}
void papacc_network_stream_release_reservation(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_NETWORK_STREAM *s){PAPACC_U8 *tx,*rx;PAPACC_SIZE bc;if(valid(m)!=PAPACC_TRUE||s==NULL||s->state!=PAPACC_NETWORK_STREAM_RESERVED)return;tx=s->to_remote;rx=s->to_client;bc=s->buffer_capacity;if(tx!=NULL&&bc!=0U)memset(tx,0,bc);if(rx!=NULL&&bc!=0U)memset(rx,0,bc);*s=(PAPACC_NETWORK_STREAM)PAPACC_NETWORK_STREAM_INITIALIZER;s->to_remote=tx;s->to_client=rx;s->buffer_capacity=bc;--m->count;}
PAPACC_NETWORK_STREAM *papacc_network_stream_find_for_session(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 sid,PAPACC_U64 id){PAPACC_SIZE i;if(valid(m)!=PAPACC_TRUE||sid==0U||id==0U)return NULL;for(i=0U;i<m->capacity;++i)if(m->storage[i].state>=PAPACC_NETWORK_STREAM_OPEN&&m->storage[i].stream_instance_id==id&&m->storage[i].owner_session_instance_id==sid)return &m->storage[i];return NULL;}
PAPACC_NETWORK_STREAM *papacc_network_stream_find_channel(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 cid){PAPACC_SIZE i;if(valid(m)!=PAPACC_TRUE||cid==0U)return NULL;for(i=0U;i<m->capacity;++i)if((m->storage[i].state==PAPACC_NETWORK_STREAM_OPEN||m->storage[i].state==PAPACC_NETWORK_STREAM_CLOSING)&&m->storage[i].data_channel_instance_id==cid)return &m->storage[i];return NULL;}
const PAPACC_NETWORK_STREAM *papacc_network_stream_inspect_at(const PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_SIZE i){if(valid(m)!=PAPACC_TRUE||i>=m->capacity||m->storage[i].state<PAPACC_NETWORK_STREAM_OPEN)return NULL;return &m->storage[i];}
PAPACC_RESULT papacc_network_stream_enqueue_client(PAPACC_NETWORK_STREAM *s,const PAPACC_U8 *p,PAPACC_SIZE n){if(s==NULL||p==NULL||s->state!=PAPACC_NETWORK_STREAM_OPEN||s->client_half_close_requested==PAPACC_TRUE||s->client_half_closed==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;if(n>s->buffer_capacity-s->to_remote_size)return PAPACC_RESULT_LIMIT_EXCEEDED;memcpy(s->to_remote+s->to_remote_size,p,n);s->to_remote_size+=n;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_network_stream_take_client(PAPACC_NETWORK_STREAM *s,PAPACC_U8 *p,PAPACC_SIZE cap,PAPACC_SIZE *n){PAPACC_SIZE take;if(n!=NULL)*n=0U;if(s==NULL||p==NULL||n==NULL||s->state!=PAPACC_NETWORK_STREAM_OPEN)return PAPACC_RESULT_INVALID_ARGUMENT;take=s->to_client_size<cap?s->to_client_size:cap;if(take>0U){memcpy(p,s->to_client,take);memmove(s->to_client,s->to_client+take,s->to_client_size-take);s->to_client_size-=take;}*n=take;return PAPACC_RESULT_OK;}
PAPACC_SIZE papacc_network_stream_client_space(const PAPACC_NETWORK_STREAM *s){return s!=NULL&&s->state==PAPACC_NETWORK_STREAM_OPEN&&s->to_remote_size<=s->buffer_capacity?s->buffer_capacity-s->to_remote_size:0U;}
PAPACC_RESULT papacc_network_stream_peek_client(const PAPACC_NETWORK_STREAM *s,const PAPACC_U8 **p,PAPACC_SIZE *n){if(p!=NULL)*p=NULL;if(n!=NULL)*n=0U;if(s==NULL||p==NULL||n==NULL||s->state!=PAPACC_NETWORK_STREAM_OPEN)return PAPACC_RESULT_INVALID_ARGUMENT;*p=s->to_client;*n=s->to_client_size;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_network_stream_consume_client(PAPACC_NETWORK_STREAM *s,PAPACC_SIZE n){if(s==NULL||s->state!=PAPACC_NETWORK_STREAM_OPEN||n>s->to_client_size)return PAPACC_RESULT_INVALID_ARGUMENT;if(n>0U){memmove(s->to_client,s->to_client+n,s->to_client_size-n);s->to_client_size-=n;}return PAPACC_RESULT_OK;}
static void close_one(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_NETWORK_STREAM *s){PAPACC_U8 *tx=s->to_remote,*rx=s->to_client;PAPACC_SIZE bc=s->buffer_capacity;if(s->state>=PAPACC_NETWORK_STREAM_OPEN)close_transport(s);if(tx!=NULL&&bc!=0U)memset(tx,0,bc);if(rx!=NULL&&bc!=0U)memset(rx,0,bc);*s=(PAPACC_NETWORK_STREAM)PAPACC_NETWORK_STREAM_INITIALIZER;s->to_remote=tx;s->to_client=rx;s->buffer_capacity=bc;--m->count;}
static PAPACC_NETWORK_STREAM *least_recent_for_session(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 sid){PAPACC_SIZE i;PAPACC_NETWORK_STREAM *selected=NULL;for(i=0U;i<m->capacity;++i){PAPACC_NETWORK_STREAM *candidate=&m->storage[i];if((candidate->state==PAPACC_NETWORK_STREAM_OPEN||candidate->state==PAPACC_NETWORK_STREAM_CLOSING)&&candidate->owner_session_instance_id==sid&&(selected==NULL||candidate->schedule_order<selected->schedule_order))selected=candidate;}return selected;}
static PAPACC_NETWORK_STREAM *select_one(PAPACC_NETWORK_STREAM_MANAGER *m){PAPACC_SIZE k,i;PAPACC_NETWORK_STREAM *fallback=NULL,*selected;PAPACC_U64 sid=0U;for(k=1U;k<=m->capacity;++k){i=(m->cursor+k)%m->capacity;if(m->storage[i].state==PAPACC_NETWORK_STREAM_OPEN||m->storage[i].state==PAPACC_NETWORK_STREAM_CLOSING){if(fallback==NULL)fallback=&m->storage[i];if(m->storage[i].owner_session_instance_id!=m->last_session_id){sid=m->storage[i].owner_session_instance_id;break;}}}if(sid==0U&&fallback!=NULL)sid=fallback->owner_session_instance_id;if(sid==0U)return NULL;selected=least_recent_for_session(m,sid);if(selected!=NULL){selected->schedule_order=m->next_schedule_order++;if(m->next_schedule_order==0U)m->next_schedule_order=1U;m->cursor=(PAPACC_SIZE)(selected-m->storage);}return selected;}
PAPACC_RESULT papacc_network_stream_progress_one_observed_at(
    PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_SIZE quantum,PAPACC_U64 now,
    PAPACC_NETWORK_STREAM **out,PAPACC_BOOL *made_progress)
{
    PAPACC_NETWORK_STREAM *s;PAPACC_SIZE n=0U,amount;
    PAPACC_BOOL eof=PAPACC_FALSE,done=PAPACC_FALSE;PAPACC_RESULT r;
    if(out!=NULL)*out=NULL;if(made_progress!=NULL)*made_progress=PAPACC_FALSE;
    if(valid(m)!=PAPACC_TRUE||out==NULL||made_progress==NULL||quantum==0U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    s=select_one(m);if(s==NULL)return PAPACC_RESULT_OK;*out=s;
    m->last_session_id=s->owner_session_instance_id;
    if(s->terminal_pending==PAPACC_TRUE)return PAPACC_RESULT_OK;
    if(s->state==PAPACC_NETWORK_STREAM_CLOSING){
        if(s->io.shutdown==NULL){
            close_one(m,s);*made_progress=PAPACC_TRUE;return PAPACC_RESULT_OK;}
        if(now>=s->close_deadline_ns){
            s->terminal_cause=PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SHUTDOWN_DEADLINE;
            s->terminal_pending=PAPACC_TRUE;close_transport(s);
            *made_progress=PAPACC_TRUE;return PAPACC_RESULT_OK;}
        r=s->io.shutdown(s->io.context,&done);
        if(stream_trace_enabled()==PAPACC_TRUE){(void)fprintf(stderr,"PST_RECIPROCAL_TRACE event=stream_shutdown_step stream=%llu result=%d done=%d\n",(unsigned long long)s->stream_instance_id,(int)r,(int)done);(void)fflush(stderr);}
        if(r!=PAPACC_RESULT_OK){
            close_one(m,s);*made_progress=PAPACC_TRUE;return r;}
        if(done==PAPACC_TRUE){
            s->state=PAPACC_NETWORK_STREAM_OPEN;
            s->remote_half_closed=PAPACC_TRUE;
            s->remote_half_close_signalled=PAPACC_TRUE;
            *made_progress=PAPACC_TRUE;}
        return r;}
    if(s->to_remote_size>0U){
        amount=s->to_remote_size<quantum?s->to_remote_size:quantum;
        r=s->io.write(s->io.context,s->to_remote,amount,&n);
        if(r!=PAPACC_RESULT_OK){close_one(m,s);*made_progress=PAPACC_TRUE;return r;}
        if(n>amount){close_one(m,s);*made_progress=PAPACC_TRUE;
            return PAPACC_RESULT_INTERNAL_ERROR;}
        if(n>0U){memmove(s->to_remote,s->to_remote+n,s->to_remote_size-n);
            s->to_remote_size-=n;s->bytes_to_remote+=n;
            *made_progress=PAPACC_TRUE;}
        return PAPACC_RESULT_OK;}
    if(s->client_half_close_requested==PAPACC_TRUE&&
        s->client_half_closed!=PAPACC_TRUE){
        r=s->io.half_close(s->io.context);
        if(r!=PAPACC_RESULT_OK){close_one(m,s);*made_progress=PAPACC_TRUE;return r;}
        s->client_half_closed=PAPACC_TRUE;*made_progress=PAPACC_TRUE;
        return PAPACC_RESULT_OK;}
    if(s->remote_half_closed==PAPACC_TRUE)return PAPACC_RESULT_OK;
    amount=s->buffer_capacity-s->to_client_size;
    if(amount>quantum)amount=quantum;if(amount==0U)return PAPACC_RESULT_OK;
    r=s->io.read(s->io.context,s->to_client+s->to_client_size,amount,&n,&eof);
    if(n>amount){close_one(m,s);*made_progress=PAPACC_TRUE;
        return PAPACC_RESULT_INTERNAL_ERROR;}
    s->to_client_size+=n;s->bytes_to_client+=n;
    if(n>0U)*made_progress=PAPACC_TRUE;
    if(r!=PAPACC_RESULT_OK){
        s->terminal_reason=s->io.terminal_reason!=NULL?
            s->io.terminal_reason(s->io.context):
            PAPACC_NETWORK_TERMINAL_REASON_UNSPECIFIED;
        if(s->terminal_reason==PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED){
            s->terminal_pending=PAPACC_TRUE;close_transport(s);
            *made_progress=PAPACC_TRUE;return PAPACC_RESULT_OK;}
        close_one(m,s);*made_progress=PAPACC_TRUE;return r;}
    if(eof==PAPACC_TRUE){s->remote_half_closed=PAPACC_TRUE;
        *made_progress=PAPACC_TRUE;}
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_network_stream_progress_one_at(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_SIZE quantum,PAPACC_U64 now,PAPACC_NETWORK_STREAM **out){PAPACC_BOOL made_progress;return papacc_network_stream_progress_one_observed_at(m,quantum,now,out,&made_progress);}
PAPACC_RESULT papacc_network_stream_progress_one(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_SIZE quantum,PAPACC_NETWORK_STREAM **out){return papacc_network_stream_progress_one_at(m,quantum,0U,out);}
PAPACC_RESULT papacc_network_stream_half_close_client(PAPACC_NETWORK_STREAM *s){PAPACC_RESULT r;if(s==NULL||s->state!=PAPACC_NETWORK_STREAM_OPEN)return PAPACC_RESULT_INVALID_STATE;if(s->transport_mode==PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)return PAPACC_RESULT_NOT_SUPPORTED;if(s->client_half_closed==PAPACC_TRUE||s->client_half_close_requested==PAPACC_TRUE)return PAPACC_RESULT_OK;s->client_half_close_requested=PAPACC_TRUE;if(s->to_remote_size!=0U)return PAPACC_RESULT_OK;r=s->io.half_close(s->io.context);if(r==PAPACC_RESULT_OK)s->client_half_closed=PAPACC_TRUE;return r;}
PAPACC_RESULT papacc_network_stream_request_close(PAPACC_NETWORK_STREAM *s,PAPACC_U64 deadline_ns){if(s==NULL||s->state!=PAPACC_NETWORK_STREAM_OPEN||deadline_ns==0U)return PAPACC_RESULT_INVALID_STATE;if(s->transport_mode!=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED||s->io.shutdown==NULL)return PAPACC_RESULT_NOT_SUPPORTED;s->state=PAPACC_NETWORK_STREAM_CLOSING;s->close_deadline_ns=deadline_ns;s->to_remote_size=0U;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_network_stream_close_channel(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 cid){PAPACC_SIZE i;if(valid(m)!=PAPACC_TRUE||cid==0U)return PAPACC_RESULT_INVALID_ARGUMENT;for(i=0U;i<m->capacity;++i)if(m->storage[i].state!=PAPACC_NETWORK_STREAM_UNUSED&&m->storage[i].data_channel_instance_id==cid){close_one(m,&m->storage[i]);return PAPACC_RESULT_OK;}return PAPACC_RESULT_INVALID_ARGUMENT;}
PAPACC_RESULT papacc_network_stream_close_session_with_cause(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 sid,PAPACC_NETWORK_STREAM_TERMINAL_CAUSE cause){PAPACC_SIZE i;if(valid(m)!=PAPACC_TRUE||sid==0U)return PAPACC_RESULT_INVALID_ARGUMENT;for(i=0U;i<m->capacity;++i)if(m->storage[i].state!=PAPACC_NETWORK_STREAM_UNUSED&&m->storage[i].owner_session_instance_id==sid){if(m->storage[i].state==PAPACC_NETWORK_STREAM_RESERVED)papacc_network_stream_release_reservation(m,&m->storage[i]);else{if(m->storage[i].terminal_cause==PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE)m->storage[i].terminal_cause=cause;close_one(m,&m->storage[i]);}}return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_network_stream_close_session(PAPACC_NETWORK_STREAM_MANAGER *m,PAPACC_U64 sid){return papacc_network_stream_close_session_with_cause(m,sid,PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_NONE);}
void papacc_network_stream_manager_shutdown(PAPACC_NETWORK_STREAM_MANAGER *m){PAPACC_SIZE i;if(valid(m)!=PAPACC_TRUE)return;for(i=0U;i<m->capacity;++i){PAPACC_NETWORK_STREAM*s=&m->storage[i];if(s->state>=PAPACC_NETWORK_STREAM_OPEN&&s->terminal_pending!=PAPACC_TRUE&&s->transport_mode==PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED&&s->io.shutdown!=NULL){PAPACC_BOOL done=PAPACC_FALSE;(void)s->io.shutdown(s->io.context,&done);}if(s->state>=PAPACC_NETWORK_STREAM_OPEN&&s->io.close!=NULL)s->io.close(s->io.context);if(s->to_remote!=NULL&&s->buffer_capacity!=0U)memset(s->to_remote,0,s->buffer_capacity);if(s->to_client!=NULL&&s->buffer_capacity!=0U)memset(s->to_client,0,s->buffer_capacity);}*m=(PAPACC_NETWORK_STREAM_MANAGER)PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;}
