#include "connection_classifier.h"
#include "data_attach_processor.h"

#include <string.h>

typedef struct TEST_IO { PAPACC_U8 input[64]; PAPACC_SIZE length,offset,limit;
    PAPACC_U8 output[64]; PAPACC_SIZE output_length,write_limit;
    PAPACC_BOOL would_block,write_error,write_eof; PAPACC_U32 reads,closes;
} TEST_IO;
typedef struct FIXTURE {
    PAPACC_CONNECTION_MANAGER connections; PAPACC_CONNECTION connection_storage[4];
    PAPACC_SESSION_MANAGER sessions; PAPACC_SESSION session_storage[2];
    PAPACC_CHANNEL_MANAGER channels; PAPACC_CHANNEL channel_storage[4];
    PAPACC_DATA_ASSOCIATION_MANAGER associations;
    PAPACC_DATA_ASSOCIATION_ENTRY entries[2]; TEST_IO io[4]; PAPACC_U8 seed;
} FIXTURE;
static PAPACC_RESULT rd(void *x,PAPACC_U8*b,PAPACC_SIZE c,PAPACC_SIZE*n,
    PAPACC_TRANSPORT_IO_STATUS*s){TEST_IO*i=(TEST_IO*)x;PAPACC_SIZE a;++i->reads;
    if(i->offset==i->length){*n=0;*s=PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;return PAPACC_RESULT_OK;}
    a=i->length-i->offset;if(a>c)a=c;if(a>i->limit)a=i->limit;
    memcpy(b,&i->input[i->offset],a);i->offset+=a;*n=a;*s=PAPACC_TRANSPORT_IO_STATUS_PROGRESS;return PAPACC_RESULT_OK;}
static PAPACC_RESULT wr(void*x,const PAPACC_U8*b,PAPACC_SIZE l,PAPACC_SIZE*n,
    PAPACC_TRANSPORT_IO_STATUS*s){TEST_IO*i=(TEST_IO*)x;PAPACC_SIZE a=l;
    if(i->write_error)return PAPACC_RESULT_INTERNAL_ERROR;
    if(i->write_eof){*n=0;*s=PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM;return PAPACC_RESULT_OK;}
    if(i->would_block){i->would_block=PAPACC_FALSE;*n=0;*s=PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;return PAPACC_RESULT_OK;}
    if(a>i->write_limit)a=i->write_limit;memcpy(&i->output[i->output_length],b,a);
    i->output_length+=a;*n=a;*s=PAPACC_TRANSPORT_IO_STATUS_PROGRESS;return PAPACC_RESULT_OK;}
static void cl(void*x){++((TEST_IO*)x)->closes;}
static PAPACC_RESULT gen(void*x,PAPACC_DATA_ASSOCIATION_TICKET*t){FIXTURE*f=(FIXTURE*)x;PAPACC_SIZE i;
    for(i=0;i<16;++i)t->bytes[i]=(PAPACC_U8)(f->seed+i);++f->seed;return PAPACC_RESULT_OK;}
static int init(FIXTURE*f){memset(f,0,sizeof(*f));f->connections=(PAPACC_CONNECTION_MANAGER)PAPACC_CONNECTION_MANAGER_INITIALIZER;
    f->sessions=(PAPACC_SESSION_MANAGER)PAPACC_SESSION_MANAGER_INITIALIZER;f->channels=(PAPACC_CHANNEL_MANAGER)PAPACC_CHANNEL_MANAGER_INITIALIZER;
    f->associations=(PAPACC_DATA_ASSOCIATION_MANAGER)PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER;f->seed=1;
    return papacc_connection_manager_init(&f->connections,f->connection_storage,4)!=PAPACC_RESULT_OK||
      papacc_session_manager_init(&f->sessions,f->session_storage,2)!=PAPACC_RESULT_OK||
      papacc_channel_manager_init(&f->channels,f->channel_storage,4,&f->connections,&f->sessions)!=PAPACC_RESULT_OK||
      papacc_data_association_manager_init(&f->associations,f->entries,2,&f->sessions,&f->channels,gen,f,100)!=PAPACC_RESULT_OK;}
static PAPACC_CONNECTION*pub(FIXTURE*f,PAPACC_SIZE k){PAPACC_TRANSPORT_CONNECTION t=PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT e=PAPACC_NETWORK_ENDPOINT_INITIALIZER;PAPACC_CONNECTION*c=NULL;TEST_IO*i=&f->io[k];
    (void)papacc_ip_address_set_ipv4(&e.address,127,0,0,1);e.port=(PAPACC_U16)(4000+k);i->limit=64;i->write_limit=64;
    t.context=i;t.read_fn=rd;t.write_fn=wr;t.close_fn=cl;(void)papacc_connection_manager_publish(&f->connections,&t,&e,&e,&c);return c;}
static int active(FIXTURE*f,PAPACC_SESSION**s,PAPACC_CHANNEL**control){PAPACC_CONNECTION*c=pub(f,0);
    return c==NULL||papacc_session_manager_publish(&f->sessions,s)!=PAPACC_RESULT_OK||
      papacc_channel_manager_bind(&f->channels,(*s)->session_instance_id,c->connection_instance_id,PAPACC_CHANNEL_ROLE_CONTROL,control)!=PAPACC_RESULT_OK||
      papacc_session_activate(*s)!=PAPACC_RESULT_OK;}
static void attach_frame(TEST_IO*i,const PAPACC_DATA_ASSOCIATION_TICKET*t){PAPACC_FRAME_HEADER h=papacc_data_attach_frame_header();PAPACC_SIZE n;
    (void)papacc_frame_header_encode(&h,i->input,16,&n);memcpy(&i->input[16],t->bytes,16);i->length=32;}
static int adopt(FIXTURE*f,PAPACC_SIZE k,PAPACC_CONNECTION*c,PAPACC_DATA_ATTACH_PROCESSOR*p,PAPACC_U8*scratch,PAPACC_U64 deadline){
    PAPACC_CONNECTION_CLASSIFIER x=PAPACC_CONNECTION_CLASSIFIER_INITIALIZER;PAPACC_CONNECTION_CLASSIFIER_STATUS xs=PAPACC_CONNECTION_CLASSIFIER_STATUS_UNSPECIFIED;
    PAPACC_CONNECTION_CLASSIFICATION kind;PAPACC_FRAME_HEADER h;PAPACC_FRAMED_READER r=PAPACC_FRAMED_READER_INITIALIZER;PAPACC_U64 d;
    if(papacc_connection_classifier_init(&x,&f->connections,c->connection_instance_id,scratch,64,16,deadline)!=PAPACC_RESULT_OK)return 1;
    while(x.state==PAPACC_CONNECTION_CLASSIFIER_STATE_WAITING_FIRST_HEADER)
      if(papacc_connection_classifier_read_once(&x,&xs)!=PAPACC_RESULT_OK)return 1;
    if(xs!=PAPACC_CONNECTION_CLASSIFIER_STATUS_CLASSIFIED_DATA||
      papacc_connection_classifier_take(&x,&kind,&h,&r,&d)!=PAPACC_RESULT_OK||kind!=PAPACC_CONNECTION_CLASSIFICATION_DATA||
      papacc_data_attach_processor_init_from_reader(p,&f->connections,&f->sessions,&f->channels,&f->associations,
        c->connection_instance_id,&h,&r,d)!=PAPACC_RESULT_OK)return 1;(void)k;return 0;}
static int finish(PAPACC_DATA_ATTACH_PROCESSOR*p){PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS s=PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_UNSPECIFIED;
    while(p->state==PAPACC_DATA_ATTACH_PROCESSOR_STATE_WRITING_DATA_ACCEPT)
      if(papacc_data_attach_processor_write_once(p,&s)!=PAPACC_RESULT_OK)return 1;
    return s==PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_ESTABLISHED?0:1;}
static int success_replay(void){static const PAPACC_U8 accept[16]={0x50,0x41,0x43,0x43,1,0,0,16,0,6,0,0,0,0,0,0};
    FIXTURE f;PAPACC_SESSION*s;PAPACC_CHANNEL*ctl;PAPACC_CONNECTION*c,*replay;PAPACC_DATA_ASSOCIATION_TICKET t;PAPACC_U64 dl;
    PAPACC_DATA_ATTACH_PROCESSOR p=PAPACC_DATA_ATTACH_PROCESSOR_INITIALIZER,q=PAPACC_DATA_ATTACH_PROCESSOR_INITIALIZER;
    PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS st;PAPACC_U8 a[64],b[64];if(init(&f)||active(&f,&s,&ctl))return 1;
    if(papacc_data_association_manager_issue(&f.associations,s->session_instance_id,0,&t,&dl)!=PAPACC_RESULT_OK)return 2;
    c=pub(&f,1);attach_frame(&f.io[1],&t);f.io[1].write_limit=5;
    if(adopt(&f,1,c,&p,a,200)||papacc_data_attach_processor_read_once(&p,1,&st)!=PAPACC_RESULT_OK||
      f.io[1].reads!=1||p.state!=PAPACC_DATA_ATTACH_PROCESSOR_STATE_WRITING_DATA_ACCEPT||f.associations.count!=0||
      c->state!=PAPACC_CONNECTION_STATE_ASSOCIATED||finish(&p)||memcmp(f.io[1].output,accept,16)||s->state!=PAPACC_SESSION_STATE_ACTIVE||ctl->state!=PAPACC_CHANNEL_STATE_BOUND)return 3;
    replay=pub(&f,2);attach_frame(&f.io[2],&t);if(adopt(&f,2,replay,&q,b,200)||
      papacc_data_attach_processor_read_once(&q,2,&st)!=PAPACC_RESULT_INVALID_STATE||replay->state!=PAPACC_CONNECTION_STATE_CLOSED||
      p.data_channel_instance_id==0||papacc_channel_manager_find(&f.channels,p.data_channel_instance_id)->state!=PAPACC_CHANNEL_STATE_BOUND)return 4;
    return 0;}
static int zero_unknown_expired(void){PAPACC_SIZE mode;for(mode=0;mode<3;++mode){FIXTURE f;PAPACC_SESSION*s;PAPACC_CHANNEL*ctl;
    PAPACC_CONNECTION*c;PAPACC_DATA_ASSOCIATION_TICKET t=PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;PAPACC_U64 dl=0;PAPACC_U8 scratch[64];
    PAPACC_DATA_ATTACH_PROCESSOR p=PAPACC_DATA_ATTACH_PROCESSOR_INITIALIZER;PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS st;PAPACC_RESULT expected;
    if(init(&f)||active(&f,&s,&ctl))return 10;if(mode==1)t.bytes[0]=9;if(mode==2&&papacc_data_association_manager_issue(&f.associations,s->session_instance_id,0,&t,&dl)!=PAPACC_RESULT_OK)return 11;
    c=pub(&f,1);attach_frame(&f.io[1],&t);if(adopt(&f,1,c,&p,scratch,200))return 12;expected=mode==0?PAPACC_RESULT_PROTOCOL_ERROR:PAPACC_RESULT_INVALID_STATE;
    if(papacc_data_attach_processor_read_once(&p,mode==2?dl:1,&st)!=expected||c->state!=PAPACC_CONNECTION_STATE_CLOSED||
      s->state!=PAPACC_SESSION_STATE_ACTIVE||ctl->state!=PAPACC_CHANNEL_STATE_BOUND)return 13;}return 0;}
static int would_block_failure_deadline(void){FIXTURE f;PAPACC_SESSION*s;PAPACC_CHANNEL*ctl;PAPACC_CONNECTION*c;
    PAPACC_DATA_ASSOCIATION_TICKET t;PAPACC_U64 dl;PAPACC_U8 scratch[64];PAPACC_DATA_ATTACH_PROCESSOR p=PAPACC_DATA_ATTACH_PROCESSOR_INITIALIZER;
    PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS st;if(init(&f)||active(&f,&s,&ctl)||papacc_data_association_manager_issue(&f.associations,s->session_instance_id,0,&t,&dl)!=PAPACC_RESULT_OK)return 20;
    c=pub(&f,1);attach_frame(&f.io[1],&t);if(adopt(&f,1,c,&p,scratch,50)||papacc_data_attach_processor_read_once(&p,1,&st)!=PAPACC_RESULT_OK)return 21;
    f.io[1].would_block=PAPACC_TRUE;if(papacc_data_attach_processor_write_once(&p,&st)!=PAPACC_RESULT_OK||st!=PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_WOULD_BLOCK)return 22;
    if(papacc_data_attach_processor_check_deadline(&p,50,&st)!=PAPACC_RESULT_OK||st!=PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS_CLOSED||
      s->state!=PAPACC_SESSION_STATE_ACTIVE||ctl->state!=PAPACC_CHANNEL_STATE_BOUND)return 23;return 0;}
static int fragmented_and_capacity(void){
    FIXTURE f;PAPACC_SESSION*s;PAPACC_CHANNEL*ctl;PAPACC_CONNECTION*c;PAPACC_DATA_ASSOCIATION_TICKET t,new_ticket;PAPACC_U64 dl,new_dl;
    PAPACC_U8 scratch[64];PAPACC_DATA_ATTACH_PROCESSOR p=PAPACC_DATA_ATTACH_PROCESSOR_INITIALIZER;PAPACC_DATA_ATTACH_PROCESSOR_STEP_STATUS st;
    if(init(&f)||active(&f,&s,&ctl)||papacc_data_association_manager_issue(&f.associations,s->session_instance_id,0,&t,&dl)!=PAPACC_RESULT_OK)return 30;
    c=pub(&f,1);attach_frame(&f.io[1],&t);f.io[1].limit=1;
    if(adopt(&f,1,c,&p,scratch,200))return 31;
    while(p.state==PAPACC_DATA_ATTACH_PROCESSOR_STATE_READING_DATA_ATTACH)
      if(papacc_data_attach_processor_read_once(&p,1,&st)!=PAPACC_RESULT_OK)return 32;
    if(p.attach_payload_received!=16||finish(&p))return 33;
    if(init(&f)||active(&f,&s,&ctl)||papacc_data_association_manager_issue(&f.associations,s->session_instance_id,0,&t,&dl)!=PAPACC_RESULT_OK)return 34;
    f.channels.capacity=1;c=pub(&f,1);attach_frame(&f.io[1],&t);p=(PAPACC_DATA_ATTACH_PROCESSOR)PAPACC_DATA_ATTACH_PROCESSOR_INITIALIZER;
    if(adopt(&f,1,c,&p,scratch,200)||papacc_data_attach_processor_read_once(&p,1,&st)!=PAPACC_RESULT_LIMIT_EXCEEDED||
      f.associations.count!=0||c->state!=PAPACC_CONNECTION_STATE_CLOSED||s->state!=PAPACC_SESSION_STATE_ACTIVE||ctl->state!=PAPACC_CHANNEL_STATE_BOUND)return 35;
    if(papacc_data_association_manager_issue(&f.associations,s->session_instance_id,2,&new_ticket,&new_dl)!=PAPACC_RESULT_OK||
      papacc_data_association_ticket_equal(&t,&new_ticket)==PAPACC_TRUE)return 36;
    return 0;}
int main(void){int r=success_replay();if(r)return r;r=zero_unknown_expired();if(r)return r;r=would_block_failure_deadline();return r?r:fragmented_and_capacity();}
