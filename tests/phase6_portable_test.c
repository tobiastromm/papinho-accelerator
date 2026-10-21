#include "builtin_capabilities.h"
#include "capability_negotiation.h"
#include "destination.h"
#include "network_stream.h"
#include "network_authorization.h"
#include "egress_runtime_config.h"
#include <string.h>

#define CHECK(x,n) do{if(!(x))return (n);}while(0)

typedef struct TEST_POLICY {int host_calls;int endpoint_calls;PAPACC_EGRESS_DECISION decision;} TEST_POLICY;
typedef struct TEST_IO {PAPACC_U8 written[128],read_data[128];PAPACC_SIZE written_size,
 read_size,read_offset,write_limit,read_limit;int write_would_block,read_would_block,
 read_error,closed,half_closed,shutdown_calls,shutdown_complete_after;} TEST_IO;
typedef struct BULK_IO {PAPACC_SIZE remaining,read_calls;int closed;} BULK_IO;

static PAPACC_RESULT allow_capability(void *context,const PAPACC_CAPABILITY_KEY *key,
    PAPACC_CAPABILITY_POLICY_DECISION *decision)
{(void)context;(void)key;*decision=PAPACC_CAPABILITY_POLICY_ALLOW;return PAPACC_RESULT_OK;}
static PAPACC_EGRESS_DECISION host_policy(void *context,const char *host,PAPACC_SIZE size,PAPACC_U16 port)
{TEST_POLICY *p=(TEST_POLICY*)context;++p->host_calls;return host!=NULL&&size!=0U&&port!=0U?p->decision:PAPACC_EGRESS_ERROR;}
static PAPACC_EGRESS_DECISION endpoint_policy(void *context,const PAPACC_NETWORK_ENDPOINT *endpoint,PAPACC_U32 classes)
{TEST_POLICY *p=(TEST_POLICY*)context;(void)endpoint;(void)classes;++p->endpoint_calls;return p->decision;}
static PAPACC_RESULT resolve_mixed(void *context,const char *host,PAPACC_SIZE size,PAPACC_U16 port,
    PAPACC_NETWORK_ENDPOINT *out,PAPACC_SIZE capacity,PAPACC_SIZE *count)
{PAPACC_U8 mapped[16]={0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0xFFU,0xFFU,127U,0U,0U,1U};
 (void)context;(void)host;(void)size;if(capacity<2U)return PAPACC_RESULT_LIMIT_EXCEEDED;
 out[0]=(PAPACC_NETWORK_ENDPOINT)PAPACC_NETWORK_ENDPOINT_INITIALIZER;
 out[1]=(PAPACC_NETWORK_ENDPOINT)PAPACC_NETWORK_ENDPOINT_INITIALIZER;
 (void)papacc_ip_address_set_ipv6(&out[0].address,mapped);out[0].port=port;
 (void)papacc_ip_address_set_ipv4(&out[1].address,8U,8U,8U,8U);out[1].port=port;*count=2U;return PAPACC_RESULT_OK;}
static PAPACC_RESULT io_read(void *context,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *n,PAPACC_BOOL *eof)
{TEST_IO *io=(TEST_IO*)context;PAPACC_SIZE amount;if(cap==0U)return PAPACC_RESULT_LIMIT_EXCEEDED;
 *n=0U;*eof=PAPACC_FALSE;if(io->read_would_block){--io->read_would_block;return PAPACC_RESULT_OK;}
 if(io->read_offset==io->read_size){*eof=PAPACC_TRUE;return PAPACC_RESULT_OK;}
 amount=io->read_size-io->read_offset;if(amount>cap)amount=cap;if(io->read_limit!=0U&&amount>io->read_limit)amount=io->read_limit;
 memcpy(out,io->read_data+io->read_offset,amount);io->read_offset+=amount;*n=amount;
 return io->read_error?PAPACC_RESULT_INTERNAL_ERROR:PAPACC_RESULT_OK;}
static PAPACC_RESULT io_write(void *context,const PAPACC_U8 *data,PAPACC_SIZE size,PAPACC_SIZE *n)
{TEST_IO *io=(TEST_IO*)context;PAPACC_SIZE amount=size;if(io->write_would_block){--io->write_would_block;*n=0U;return PAPACC_RESULT_OK;}
 if(io->write_limit!=0U&&amount>io->write_limit)amount=io->write_limit;
 if(amount>sizeof(io->written)-io->written_size)return PAPACC_RESULT_LIMIT_EXCEEDED;
 memcpy(io->written+io->written_size,data,amount);io->written_size+=amount;*n=amount;return PAPACC_RESULT_OK;}
static PAPACC_RESULT io_half(void *context){++((TEST_IO*)context)->half_closed;return PAPACC_RESULT_OK;}
static PAPACC_RESULT io_shutdown(void *context,PAPACC_BOOL *done)
{TEST_IO *io=(TEST_IO*)context;++io->shutdown_calls;*done=
 (io->shutdown_calls>=io->shutdown_complete_after)?PAPACC_TRUE:PAPACC_FALSE;
 return PAPACC_RESULT_OK;}
static void io_close(void *context){++((TEST_IO*)context)->closed;}
static PAPACC_RESULT bulk_read(void *context,PAPACC_U8 *out,PAPACC_SIZE cap,
 PAPACC_SIZE *n,PAPACC_BOOL *eof)
{BULK_IO*io=(BULK_IO*)context;PAPACC_SIZE amount=io->remaining<cap?
 io->remaining:cap;memset(out,0xA5,amount);io->remaining-=amount;
 ++io->read_calls;*n=amount;*eof=io->remaining==0U?PAPACC_TRUE:PAPACC_FALSE;
 return PAPACC_RESULT_OK;}
static PAPACC_RESULT bulk_write(void *context,const PAPACC_U8 *data,
 PAPACC_SIZE size,PAPACC_SIZE *n)
{(void)context;(void)data;*n=size;return PAPACC_RESULT_OK;}
static PAPACC_RESULT bulk_half(void *context){(void)context;return PAPACC_RESULT_OK;}
static void bulk_close(void *context){++((BULK_IO*)context)->closed;}
static PAPACC_NETWORK_TERMINAL_REASON io_terminal_reason(void *context)
{(void)context;return PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED;}
static PAPACC_RESULT select_profile(void *context,const PAPACC_NETWORK_ENDPOINT *e,
 PAPACC_NETWORK_TRANSPORT_MODE mode,PAPACC_U32 *id){PAPACC_U16 *port=(PAPACC_U16*)context;if(e->port!=*port||mode!=PAPACC_NETWORK_TRANSPORT_TCP_PLAIN)return PAPACC_RESULT_INVALID_STATE;*id=7U;return PAPACC_RESULT_OK;}

static int test_capabilities(void)
{
 PAPACC_CAPABILITY_DESCRIPTOR descriptors[4];PAPACC_CAPABILITY_REGISTRY registry=PAPACC_CAPABILITY_REGISTRY_INITIALIZER;
 PAPACC_CAPABILITY_KEY supported_store[3],enabled_store[3],client_store[3],preference_store[3],decoded_s[3],decoded_p[3],effective_store[3];
 PAPACC_CAPABILITY_SET supported=PAPACC_CAPABILITY_SET_INITIALIZER,enabled=PAPACC_CAPABILITY_SET_INITIALIZER,client=PAPACC_CAPABILITY_SET_INITIALIZER,preference=PAPACC_CAPABILITY_SET_INITIALIZER,ds=PAPACC_CAPABILITY_SET_INITIALIZER,dp=PAPACC_CAPABILITY_SET_INITIALIZER;
 PAPACC_SESSION_CAPABILITY_SNAPSHOT snapshot=PAPACC_SESSION_CAPABILITY_SNAPSHOT_INITIALIZER;PAPACC_CAPABILITY_NEGOTIATOR negotiator=PAPACC_CAPABILITY_NEGOTIATOR_INITIALIZER;
 PAPACC_CAPABILITY_EVALUATION_INPUTS inputs;PAPACC_U8 wire[64],result[64];PAPACC_SIZE written=0U;
 CHECK(papacc_capability_registry_init(&registry,descriptors,4U)==PAPACC_RESULT_OK,1);
 CHECK(papacc_builtin_capabilities_register(&registry)==PAPACC_RESULT_OK&&registry.count==3U,2);
 CHECK(PAPACC_CAPABILITY_NETWORK_EGRESS_V1.id==1U&&PAPACC_CAPABILITY_NETWORK_EGRESS_V1.major==1U&&
  PAPACC_CAPABILITY_NETWORK_EGRESS_V2.id==1U&&PAPACC_CAPABILITY_NETWORK_EGRESS_V2.major==2U&&
  PAPACC_CAPABILITY_TLS_OFFLOAD_V1.id==2U&&
  !papacc_capability_key_equal(&PAPACC_CAPABILITY_NETWORK_EGRESS_V1,&PAPACC_CAPABILITY_NETWORK_EGRESS_V2)&&
  !papacc_capability_key_equal(&PAPACC_CAPABILITY_NETWORK_EGRESS_V2,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1),3);
 CHECK(papacc_capability_set_init(&supported,supported_store,3U)==PAPACC_RESULT_OK&&papacc_capability_set_init(&enabled,enabled_store,3U)==PAPACC_RESULT_OK&&papacc_capability_set_init(&client,client_store,3U)==PAPACC_RESULT_OK&&papacc_capability_set_init(&preference,preference_store,3U)==PAPACC_RESULT_OK,4);
 CHECK(papacc_capability_set_insert(&supported,&PAPACC_CAPABILITY_NETWORK_EGRESS_V1)==PAPACC_RESULT_OK&&papacc_capability_set_insert(&supported,&PAPACC_CAPABILITY_NETWORK_EGRESS_V2)==PAPACC_RESULT_OK&&papacc_capability_set_insert(&supported,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1)==PAPACC_RESULT_OK,5);
 CHECK(papacc_capability_set_insert(&enabled,&PAPACC_CAPABILITY_NETWORK_EGRESS_V1)==PAPACC_RESULT_OK&&papacc_capability_set_insert(&enabled,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1)==PAPACC_RESULT_OK,6);
 CHECK(papacc_capability_set_insert(&client,&PAPACC_CAPABILITY_NETWORK_EGRESS_V1)==PAPACC_RESULT_OK&&papacc_capability_set_insert(&client,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1)==PAPACC_RESULT_OK,7);
 CHECK(papacc_capability_set_insert(&preference,&PAPACC_CAPABILITY_NETWORK_EGRESS_V1)==PAPACC_RESULT_OK&&papacc_capability_set_insert(&preference,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1)==PAPACC_RESULT_OK,8);
 CHECK(papacc_capability_negotiate_encode(&client,&preference,wire,sizeof(wire),&written)==PAPACC_RESULT_OK,9);
 CHECK(papacc_capability_set_init(&ds,decoded_s,3U)==PAPACC_RESULT_OK&&papacc_capability_set_init(&dp,decoded_p,3U)==PAPACC_RESULT_OK,10);
 CHECK(papacc_capability_negotiate_decode(wire,written,&ds,&dp)==PAPACC_RESULT_OK&&ds.count==2U&&dp.count==2U,11);
 memset(&inputs,0,sizeof(inputs));inputs.registry=&registry;inputs.server_supported=&supported;inputs.server_enabled=&enabled;inputs.principal_policy=allow_capability;
 CHECK(papacc_capability_negotiator_init(&negotiator,9U,decoded_s,3U,decoded_p,3U,&snapshot)==PAPACC_RESULT_OK,12);
 CHECK(papacc_capability_negotiator_process(&negotiator,wire,written,&inputs,effective_store,3U,result,sizeof(result),&written)==PAPACC_RESULT_OK&&snapshot.published&&snapshot.effective.count==2U&&papacc_session_capability_snapshot_contains(&snapshot,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1),13);
 CHECK(papacc_network_open_authorize(PAPACC_TRUE,&snapshot,&enabled,allow_capability,NULL,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN)==PAPACC_RESULT_OK,16);
 CHECK(papacc_network_open_authorize(PAPACC_TRUE,&snapshot,&enabled,allow_capability,NULL,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)==PAPACC_RESULT_OK,17);
 CHECK(papacc_network_open_authorize(PAPACC_FALSE,&snapshot,&enabled,allow_capability,NULL,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN)==PAPACC_RESULT_INVALID_STATE,18);
 CHECK(papacc_capability_set_insert(&enabled,&PAPACC_CAPABILITY_NETWORK_EGRESS_V2)==PAPACC_RESULT_OK,19);
 effective_store[0]=PAPACC_CAPABILITY_NETWORK_EGRESS_V2;
 CHECK(papacc_network_open_authorize(PAPACC_TRUE,&snapshot,&enabled,allow_capability,NULL,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)==PAPACC_RESULT_OK,20);
 CHECK(papacc_capability_negotiator_process(&negotiator,wire,written,&inputs,effective_store,3U,result,sizeof(result),&written)==PAPACC_RESULT_INVALID_STATE,14);
 wire[0]=0U;wire[1]=1U;wire[2]=0U;wire[3]=1U;memset(wire+4,0,6U);memcpy(wire+10,"\0\0\0\1\0\1",6U);
 ds.count=dp.count=0U;CHECK(papacc_capability_negotiate_decode(wire,16U,&ds,&dp)==PAPACC_RESULT_PROTOCOL_ERROR,15);
 {
  PAPACC_CAPABILITY_KEY v2_supported_storage[2],v2_preference_storage[2];
  PAPACC_CAPABILITY_KEY v2_decoded_supported[2],v2_decoded_preference[2];
  PAPACC_CAPABILITY_KEY v2_effective[2];
  PAPACC_CAPABILITY_SET v2_supported=PAPACC_CAPABILITY_SET_INITIALIZER;
  PAPACC_CAPABILITY_SET v2_preference=PAPACC_CAPABILITY_SET_INITIALIZER;
  PAPACC_CAPABILITY_SET v2_ds=PAPACC_CAPABILITY_SET_INITIALIZER;
  PAPACC_CAPABILITY_SET v2_dp=PAPACC_CAPABILITY_SET_INITIALIZER;
  PAPACC_SESSION_CAPABILITY_SNAPSHOT v2_snapshot=
   PAPACC_SESSION_CAPABILITY_SNAPSHOT_INITIALIZER;
  PAPACC_CAPABILITY_NEGOTIATOR v2_negotiator=
   PAPACC_CAPABILITY_NEGOTIATOR_INITIALIZER;
  CHECK(papacc_capability_set_init(&v2_supported,v2_supported_storage,2U)==PAPACC_RESULT_OK&&
   papacc_capability_set_init(&v2_preference,v2_preference_storage,2U)==PAPACC_RESULT_OK&&
   papacc_capability_set_insert(&v2_supported,&PAPACC_CAPABILITY_NETWORK_EGRESS_V2)==PAPACC_RESULT_OK&&
   papacc_capability_set_insert(&v2_supported,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1)==PAPACC_RESULT_OK&&
   papacc_capability_set_insert(&v2_preference,&PAPACC_CAPABILITY_NETWORK_EGRESS_V2)==PAPACC_RESULT_OK&&
   papacc_capability_set_insert(&v2_preference,&PAPACC_CAPABILITY_TLS_OFFLOAD_V1)==PAPACC_RESULT_OK,21);
  CHECK(papacc_capability_negotiate_encode(&v2_supported,&v2_preference,wire,
   sizeof(wire),&written)==PAPACC_RESULT_OK&&
   papacc_capability_set_init(&v2_ds,v2_decoded_supported,2U)==PAPACC_RESULT_OK&&
   papacc_capability_set_init(&v2_dp,v2_decoded_preference,2U)==PAPACC_RESULT_OK&&
   papacc_capability_negotiator_init(&v2_negotiator,10U,v2_decoded_supported,
   2U,v2_decoded_preference,2U,&v2_snapshot)==PAPACC_RESULT_OK&&
   papacc_capability_negotiator_process(&v2_negotiator,wire,written,&inputs,
   v2_effective,2U,result,sizeof(result),&written)==PAPACC_RESULT_OK&&
   v2_snapshot.effective.count==2U&&
   papacc_session_capability_snapshot_contains(&v2_snapshot,
   &PAPACC_CAPABILITY_NETWORK_EGRESS_V2)==PAPACC_TRUE&&
   papacc_session_capability_snapshot_contains(&v2_snapshot,
   &PAPACC_CAPABILITY_NETWORK_EGRESS_V1)==PAPACC_FALSE&&
   papacc_session_capability_snapshot_contains(&v2_snapshot,
   &PAPACC_CAPABILITY_TLS_OFFLOAD_V1)==PAPACC_TRUE,22);
 }
 return 0;
}

static int test_destination_policy(void)
{
 PAPACC_NETWORK_OPEN open=PAPACC_NETWORK_OPEN_INITIALIZER;PAPACC_NORMALIZED_DESTINATION destination=PAPACC_NORMALIZED_DESTINATION_INITIALIZER;
 PAPACC_DNS_RESOLVER resolver={NULL,resolve_mixed};TEST_POLICY state={0,0,PAPACC_EGRESS_ALLOW};PAPACC_EGRESS_POLICY policy={PAPACC_TRUE,0U,host_policy,endpoint_policy,&state};
 PAPACC_NETWORK_ENDPOINT output[2];PAPACC_SIZE count=0U;const char *name="ExAmPle.COM";
 open.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;open.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;open.port=443U;open.destination_size=(PAPACC_U16)strlen(name);memcpy(open.destination,name,open.destination_size);
 CHECK(papacc_destination_normalize(&open,&destination)==PAPACC_RESULT_OK&&strcmp(destination.hostname,"example.com")==0,20);
 CHECK(papacc_destination_resolve_and_filter(&destination,&resolver,&policy,output,2U,&count)==PAPACC_RESULT_OK&&count==1U&&state.host_calls==1&&state.endpoint_calls==1,21);
 CHECK(output[0].address.family==PAPACC_IP_FAMILY_IPV4&&output[0].address.bytes[0]==8U,22);
 policy.explicitly_allowed_sensitive_classes=PAPACC_EGRESS_SENSITIVE_LOOPBACK;
 CHECK(papacc_destination_resolve_and_filter(&destination,&resolver,&policy,output,2U,&count)==PAPACC_RESULT_OK&&count==2U&&output[0].address.family==PAPACC_IP_FAMILY_IPV4&&output[0].address.bytes[0]==127U,23);
 policy.master_enabled=PAPACC_FALSE;CHECK(papacc_destination_resolve_and_filter(&destination,&resolver,&policy,output,2U,&count)==PAPACC_RESULT_INVALID_STATE,24);
 open.destination[0]=0xC3U;open.destination_size=1U;CHECK(papacc_destination_normalize(&open,&destination)==PAPACC_RESULT_PROTOCOL_ERROR,25);
 open.destination_kind=PAPACC_NETWORK_DESTINATION_IPV4;open.destination_size=4U;
 open.destination[0]=192U;open.destination[1]=0U;open.destination[2]=2U;open.destination[3]=1U;
 CHECK(papacc_destination_normalize(&open,&destination)==PAPACC_RESULT_OK&&destination.literal_endpoint.address.family==PAPACC_IP_FAMILY_IPV4&&destination.literal_endpoint.address.bytes[0]==192U,26);
 open.destination_kind=PAPACC_NETWORK_DESTINATION_IPV6;open.destination_size=16U;memset(open.destination,0,16U);open.destination[15]=1U;
 CHECK(papacc_destination_normalize(&open,&destination)==PAPACC_RESULT_OK&&destination.literal_endpoint.address.family==PAPACC_IP_FAMILY_IPV6&&destination.literal_endpoint.address.bytes[15]==1U,27);
 return 0;
}

static int test_streams(void)
{
 PAPACC_NETWORK_STREAM records[2];PAPACC_U8 tx[8],rx[8],payload[5]={0U,1U,0x7FU,0x80U,0xFFU},taken[8];
 PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;PAPACC_NETWORK_STREAM *a,*b,*progressed;TEST_IO ia={0},ib={0};PAPACC_NETWORK_STREAM_IO aio={&ia,io_read,io_write,io_half,NULL,io_close},bio={&ib,io_read,io_write,io_half,NULL,io_close};PAPACC_SIZE n;const PAPACC_U8 *peek=NULL;
 ib.read_data[0]=0xA5U;ib.read_size=1U;ia.write_limit=2U;
 CHECK(papacc_network_stream_manager_init(&manager,records,tx,rx,2U,1U,4U)==PAPACC_RESULT_OK,30);
 CHECK(papacc_network_stream_reserve(&manager,1U,11U,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&a)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,a,&aio)==PAPACC_RESULT_OK,31);
 CHECK(papacc_network_stream_reserve(&manager,1U,12U,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&b)==PAPACC_RESULT_LIMIT_EXCEEDED,32);
 CHECK(papacc_network_stream_reserve(&manager,2U,12U,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&b)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,b,&bio)==PAPACC_RESULT_OK,33);
 CHECK(papacc_network_stream_find_for_session(&manager,2U,a->stream_instance_id)==NULL,34);
 CHECK(papacc_network_stream_enqueue_client(a,payload,4U)==PAPACC_RESULT_OK&&papacc_network_stream_client_space(a)==0U&&papacc_network_stream_enqueue_client(a,payload,1U)==PAPACC_RESULT_LIMIT_EXCEEDED,35);
 CHECK(papacc_network_stream_progress_one(&manager,1U,&progressed)==PAPACC_RESULT_OK&&progressed==b,36);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&progressed)==PAPACC_RESULT_OK&&progressed==a&&ia.written_size==2U&&a->to_remote_size==2U,37);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&progressed)==PAPACC_RESULT_OK&&progressed==b&&b->remote_half_closed==PAPACC_TRUE,43);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&progressed)==PAPACC_RESULT_OK&&progressed==a&&ia.written_size==4U&&a->to_remote_size==0U&&memcmp(ia.written,payload,4U)==0,44);
 CHECK(papacc_network_stream_peek_client(b,&peek,&n)==PAPACC_RESULT_OK&&n==1U&&peek[0]==0xA5U,45);
 CHECK(papacc_network_stream_consume_client(b,1U)==PAPACC_RESULT_OK&&b->to_client_size==0U,46);
 CHECK(papacc_network_stream_take_client(b,taken,sizeof(taken),&n)==PAPACC_RESULT_OK&&n==0U,38);
 CHECK(papacc_network_stream_half_close_client(a)==PAPACC_RESULT_OK&&ia.half_closed==1,39);
 CHECK(papacc_network_stream_half_close_client(b)==PAPACC_RESULT_NOT_SUPPORTED&&ib.half_closed==0,42);
 CHECK(papacc_network_stream_close_channel(&manager,11U)==PAPACC_RESULT_OK&&ia.closed==1&&manager.count==1U,40);
 CHECK(papacc_network_stream_close_session(&manager,2U)==PAPACC_RESULT_OK&&ib.closed==1&&manager.count==0U,41);
 return 0;
}

static int test_egress_runtime_config(void)
{
 PAPACC_NETWORK_ENDPOINT endpoint=PAPACC_NETWORK_ENDPOINT_INITIALIZER;
 PAPACC_EGRESS_PROFILE profile=PAPACC_EGRESS_PROFILE_INITIALIZER;
 PAPACC_EGRESS_POLICY policy=PAPACC_EGRESS_POLICY_INITIALIZER;
 PAPACC_EGRESS_RUNTIME_CONFIG config=PAPACC_EGRESS_RUNTIME_CONFIG_INITIALIZER;
 const PAPACC_EGRESS_PROFILE *selected=NULL;TEST_POLICY state={0,0,PAPACC_EGRESS_ALLOW};PAPACC_U16 port=32123U;
 (void)papacc_ip_address_set_ipv4(&endpoint.address,127U,0U,0U,1U);endpoint.port=port;
 profile.profile_id=7U;memcpy(profile.name,"fixture-default-route",22U);profile.use_default_route=PAPACC_TRUE;profile.available=PAPACC_TRUE;
 policy.master_enabled=PAPACC_TRUE;policy.explicitly_allowed_sensitive_classes=PAPACC_EGRESS_SENSITIVE_LOOPBACK;policy.endpoint_policy=endpoint_policy;policy.context=&state;
 CHECK(papacc_egress_runtime_resolve(&config,&endpoint,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&selected)==PAPACC_RESULT_INVALID_STATE,50);
 config.allow_network_egress=PAPACC_TRUE;CHECK(papacc_egress_runtime_resolve(&config,&endpoint,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&selected)==PAPACC_RESULT_INVALID_STATE,51);
 config.policy=&policy;config.profiles=&profile;config.profile_count=1U;config.select_profile=select_profile;config.select_profile_context=&port;
 CHECK(papacc_egress_runtime_resolve(&config,&endpoint,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&selected)==PAPACC_RESULT_OK&&selected==&profile,52);
 endpoint.port++;CHECK(papacc_egress_runtime_resolve(&config,&endpoint,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&selected)==PAPACC_RESULT_INVALID_STATE,53);
 endpoint.port=port;policy.explicitly_allowed_sensitive_classes=0U;CHECK(papacc_egress_runtime_resolve(&config,&endpoint,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&selected)==PAPACC_RESULT_INVALID_STATE,54);
 policy.explicitly_allowed_sensitive_classes=PAPACC_EGRESS_SENSITIVE_LOOPBACK;state.decision=PAPACC_EGRESS_ERROR;CHECK(papacc_egress_runtime_resolve(&config,&endpoint,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&selected)==PAPACC_RESULT_INVALID_STATE,55);
 return 0;
}

static int test_stream_fairness_and_isolation(void)
{
 PAPACC_NETWORK_STREAM records[3],*a1,*a2,*b,*selected;
 PAPACC_U8 tx[24],rx[24],one[2]={0x00U,0xFFU},two[2]={0xA5U,0x5AU};
 PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;
 TEST_IO ia1={0},ia2={0},ib={0};
 PAPACC_NETWORK_STREAM_IO ioa1={&ia1,io_read,io_write,io_half,NULL,io_close};
 PAPACC_NETWORK_STREAM_IO ioa2={&ia2,io_read,io_write,io_half,NULL,io_close};
 PAPACC_NETWORK_STREAM_IO iob={&ib,io_read,io_write,io_half,NULL,io_close};
 CHECK(papacc_network_stream_manager_init(&manager,records,tx,rx,3U,2U,8U)==PAPACC_RESULT_OK,60);
 CHECK(papacc_network_stream_reserve(&manager,1U,101U,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&a1)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,a1,&ioa1)==PAPACC_RESULT_OK,61);
 CHECK(papacc_network_stream_reserve(&manager,1U,102U,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&a2)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,a2,&ioa2)==PAPACC_RESULT_OK,62);
 CHECK(papacc_network_stream_reserve(&manager,2U,201U,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&b)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,b,&iob)==PAPACC_RESULT_OK,63);
 CHECK(papacc_network_stream_enqueue_client(a1,one,2U)==PAPACC_RESULT_OK&&papacc_network_stream_enqueue_client(a2,two,2U)==PAPACC_RESULT_OK&&papacc_network_stream_enqueue_client(b,one,2U)==PAPACC_RESULT_OK,64);
 CHECK(papacc_network_stream_progress_one(&manager,2U,&selected)==PAPACC_RESULT_OK&&selected->owner_session_instance_id==1U,65);
 CHECK(papacc_network_stream_progress_one(&manager,2U,&selected)==PAPACC_RESULT_OK&&selected==b&&ib.written_size==2U,66);
 CHECK(papacc_network_stream_progress_one(&manager,2U,&selected)==PAPACC_RESULT_OK&&selected->owner_session_instance_id==1U,67);
 CHECK(ia1.written_size+ia2.written_size==4U&&memcmp(ib.written,one,2U)==0&&papacc_network_stream_find_for_session(&manager,2U,a1->stream_instance_id)==NULL,68);
 papacc_network_stream_manager_shutdown(&manager);
 return 0;
}

static int test_stream_lifecycle_and_reuse(void)
{
 PAPACC_NETWORK_STREAM records[1],*stream,*selected;
 PAPACC_U8 tx[4],rx[4],payload[4]={0x00U,0xA5U,0x5AU,0xFFU};
 PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;
 TEST_IO first={0},second={0};
 PAPACC_NETWORK_STREAM_IO first_io={&first,io_read,io_write,io_half,NULL,io_close};
 PAPACC_NETWORK_STREAM_IO second_io={&second,io_read,io_write,io_half,NULL,io_close};
 const PAPACC_U8*peek=NULL;PAPACC_SIZE size=0U;
 memcpy(first.read_data,payload,sizeof(payload));first.read_size=sizeof(payload);
 CHECK(papacc_network_stream_manager_init(&manager,records,tx,rx,1U,1U,4U)==PAPACC_RESULT_OK,70);
 CHECK(papacc_network_stream_reserve(&manager,7U,701U,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&stream)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,stream,&first_io)==PAPACC_RESULT_OK,71);
 CHECK(papacc_network_stream_enqueue_client(stream,payload,sizeof(payload))==PAPACC_RESULT_OK&&papacc_network_stream_half_close_client(stream)==PAPACC_RESULT_OK&&stream->client_half_close_requested==PAPACC_TRUE&&stream->client_half_closed==PAPACC_FALSE&&first.half_closed==0,72);
 CHECK(papacc_network_stream_enqueue_client(stream,payload,1U)==PAPACC_RESULT_INVALID_STATE,73);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&selected)==PAPACC_RESULT_OK&&selected==stream&&first.written_size==4U&&first.half_closed==0,74);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&selected)==PAPACC_RESULT_OK&&first.half_closed==1&&stream->client_half_closed==PAPACC_TRUE,75);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&selected)==PAPACC_RESULT_OK&&stream->to_client_size==4U,76);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&selected)==PAPACC_RESULT_OK&&stream->remote_half_closed==PAPACC_FALSE,77);
 CHECK(papacc_network_stream_consume_client(stream,4U)==PAPACC_RESULT_OK&&papacc_network_stream_progress_one(&manager,4U,&selected)==PAPACC_RESULT_OK&&stream->remote_half_closed==PAPACC_TRUE,78);
 stream->remote_half_close_signalled=PAPACC_TRUE;
 CHECK(papacc_network_stream_close_channel(&manager,701U)==PAPACC_RESULT_OK&&first.closed==1&&manager.count==0U,79);
 CHECK(tx[0]==0U&&tx[1]==0U&&tx[2]==0U&&tx[3]==0U&&rx[0]==0U&&rx[1]==0U&&rx[2]==0U&&rx[3]==0U,80);
 CHECK(papacc_network_stream_close_channel(&manager,701U)==PAPACC_RESULT_INVALID_ARGUMENT&&first.closed==1,81);
 CHECK(papacc_network_stream_reserve(&manager,8U,801U,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&stream)==PAPACC_RESULT_OK&&stream==&records[0]&&stream->to_remote_size==0U&&stream->to_client_size==0U&&papacc_network_stream_publish(&manager,stream,&second_io)==PAPACC_RESULT_OK,82);
 CHECK(papacc_network_stream_peek_client(stream,&peek,&size)==PAPACC_RESULT_OK&&size==0U,83);
 CHECK(papacc_network_stream_enqueue_client(stream,payload,4U)==PAPACC_RESULT_OK,85);
 papacc_network_stream_manager_shutdown(&manager);
 CHECK(second.closed==1&&tx[0]==0U&&tx[1]==0U&&tx[2]==0U&&tx[3]==0U,84);
 return 0;
}

static int test_tls_incremental_shutdown(void)
{
 PAPACC_NETWORK_STREAM records[1],*stream,*selected;
 PAPACC_U8 tx[4],rx[4],byte=0xA5U;
 PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;
 TEST_IO first={0},second={0};
 PAPACC_NETWORK_STREAM_IO first_io={&first,io_read,io_write,io_half,io_shutdown,io_close};
 PAPACC_NETWORK_STREAM_IO second_io={&second,io_read,io_write,io_half,io_shutdown,io_close};
 first.shutdown_complete_after=2;second.shutdown_complete_after=99;
 CHECK(papacc_network_stream_manager_init(&manager,records,tx,rx,1U,1U,4U)==PAPACC_RESULT_OK,90);
 CHECK(papacc_network_stream_reserve(&manager,9U,901U,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&stream)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,stream,&first_io)==PAPACC_RESULT_OK,91);
 CHECK(papacc_network_stream_enqueue_client(stream,&byte,1U)==PAPACC_RESULT_OK&&papacc_network_stream_request_close(stream,100U)==PAPACC_RESULT_OK&&stream->state==PAPACC_NETWORK_STREAM_CLOSING&&stream->to_remote_size==0U,92);
 CHECK(papacc_network_stream_enqueue_client(stream,&byte,1U)==PAPACC_RESULT_INVALID_STATE&&papacc_network_stream_progress_one_at(&manager,1U,10U,&selected)==PAPACC_RESULT_OK&&first.shutdown_calls==1&&first.closed==0&&manager.count==1U,93);
 CHECK(papacc_network_stream_progress_one_at(&manager,1U,20U,&selected)==PAPACC_RESULT_OK&&first.shutdown_calls==2&&first.closed==0&&manager.count==1U&&stream->state==PAPACC_NETWORK_STREAM_OPEN&&stream->remote_half_closed==PAPACC_TRUE&&stream->remote_half_close_signalled==PAPACC_TRUE,940);
 CHECK(papacc_network_stream_close_channel(&manager,901U)==PAPACC_RESULT_OK&&first.closed==1&&manager.count==0U,941);
 CHECK(papacc_network_stream_progress_one_at(&manager,1U,20U,&selected)==PAPACC_RESULT_OK&&first.shutdown_calls==2&&first.closed==1&&manager.count==0U,94);
 CHECK(papacc_network_stream_reserve(&manager,10U,1001U,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&stream)==PAPACC_RESULT_OK&&stream==records&&papacc_network_stream_publish(&manager,stream,&second_io)==PAPACC_RESULT_OK&&papacc_network_stream_request_close(stream,50U)==PAPACC_RESULT_OK,95);
 CHECK(papacc_network_stream_progress_one_at(&manager,1U,50U,&selected)==PAPACC_RESULT_OK&&second.shutdown_calls==0&&second.closed==1&&manager.count==1U&&stream->terminal_pending==PAPACC_TRUE&&stream->terminal_cause==PAPACC_NETWORK_STREAM_TERMINAL_CAUSE_SHUTDOWN_DEADLINE,960);
 CHECK(papacc_network_stream_close_channel(&manager,1001U)==PAPACC_RESULT_OK&&second.closed==1&&manager.count==0U,961);
 CHECK(papacc_network_stream_progress_one_at(&manager,1U,50U,&selected)==PAPACC_RESULT_OK&&second.shutdown_calls==0&&second.closed==1&&manager.count==0U,96);
 CHECK(papacc_network_stream_reserve(&manager,11U,1101U,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&stream)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,stream,&second_io)==PAPACC_RESULT_OK,97);
 papacc_network_stream_manager_shutdown(&manager);
 CHECK(second.shutdown_calls==1&&second.closed==2,98);return 0;
}

static int test_truncated_terminal_preserves_buffered_data(void)
{
 PAPACC_NETWORK_STREAM records[1],*stream,*selected;
 PAPACC_U8 tx[4],rx[4],payload[2]={0xA5U,0x5AU};
 PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;
 TEST_IO source={0};
 PAPACC_NETWORK_STREAM_IO io={&source,io_read,io_write,io_half,NULL,io_close,
  io_terminal_reason};
 memcpy(source.read_data,payload,sizeof(payload));source.read_size=sizeof(payload);
 source.read_error=1;
 CHECK(papacc_network_stream_manager_init(&manager,records,tx,rx,1U,1U,4U)==PAPACC_RESULT_OK,100);
 CHECK(papacc_network_stream_reserve(&manager,12U,1201U,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&stream)==PAPACC_RESULT_OK&&papacc_network_stream_publish(&manager,stream,&io)==PAPACC_RESULT_OK,101);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&selected)==PAPACC_RESULT_OK&&
  selected==stream&&stream->state==PAPACC_NETWORK_STREAM_OPEN&&
  stream->to_client_size==sizeof(payload)&&stream->terminal_pending==PAPACC_TRUE&&
  stream->terminal_reason==PAPACC_NETWORK_TERMINAL_REASON_TLS_TRUNCATED&&
  source.closed==1&&manager.count==1U,102);
 CHECK(papacc_network_stream_progress_one(&manager,4U,&selected)==PAPACC_RESULT_OK&&
  stream->to_client_size==sizeof(payload)&&source.closed==1,103);
 CHECK(papacc_network_stream_consume_client(stream,sizeof(payload))==PAPACC_RESULT_OK&&
  papacc_network_stream_close_channel(&manager,1201U)==PAPACC_RESULT_OK&&
  source.closed==1&&manager.count==0U,104);
 return 0;
}

static int test_bounded_bulk_progress(void)
{
 PAPACC_NETWORK_STREAM records[2],*first,*second,*selected;
 PAPACC_U8 tx[2048],rx[2048];PAPACC_SIZE loops=0U;
 PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;
 BULK_IO a={64U*1024U,0U,0},b={1024U*1024U,0U,0};
 PAPACC_NETWORK_STREAM_IO aio={&a,bulk_read,bulk_write,bulk_half,NULL,bulk_close};
 PAPACC_NETWORK_STREAM_IO bio={&b,bulk_read,bulk_write,bulk_half,NULL,bulk_close};
 PAPACC_BOOL progressed=PAPACC_FALSE;
 CHECK(papacc_network_stream_manager_init(&manager,records,tx,rx,2U,1U,
  1024U)==PAPACC_RESULT_OK,110);
 CHECK(papacc_network_stream_reserve(&manager,21U,2101U,
  PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&first)==PAPACC_RESULT_OK&&
  papacc_network_stream_publish(&manager,first,&aio)==PAPACC_RESULT_OK&&
  papacc_network_stream_reserve(&manager,22U,2201U,
  PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&second)==PAPACC_RESULT_OK&&
  papacc_network_stream_publish(&manager,second,&bio)==PAPACC_RESULT_OK,111);
 while((first->remote_half_closed!=PAPACC_TRUE||
        second->remote_half_closed!=PAPACC_TRUE)&&loops<10000U){
  CHECK(papacc_network_stream_progress_one_observed_at(&manager,256U,0U,
   &selected,&progressed)==PAPACC_RESULT_OK,112);
  if(selected!=NULL&&selected->to_client_size!=0U)
   CHECK(papacc_network_stream_consume_client(selected,
    selected->to_client_size)==PAPACC_RESULT_OK,113);
  if(a.remaining!=0U&&b.remaining!=0U)
   CHECK(first->bytes_to_client<=second->bytes_to_client+256U&&
    second->bytes_to_client<=first->bytes_to_client+256U,114);
  ++loops;
 }
 CHECK(first->bytes_to_client==64U*1024U&&
  second->bytes_to_client==1024U*1024U&&loops<10000U,115);
 CHECK(a.read_calls<300U&&b.read_calls<4200U,116);
 papacc_network_stream_manager_shutdown(&manager);
 return 0;
}

static int test_would_block_backpressure_and_cancel(void)
{
 PAPACC_NETWORK_STREAM record,*stream,*selected;
 PAPACC_U8 tx[1024],rx[1024];PAPACC_SIZE index;
 PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;
 BULK_IO source={64U*1024U,0U,0};
 PAPACC_NETWORK_STREAM_IO io={&source,bulk_read,bulk_write,bulk_half,NULL,
  bulk_close};PAPACC_BOOL progressed=PAPACC_FALSE;
 CHECK(papacc_network_stream_manager_init(&manager,&record,tx,rx,1U,1U,
  1024U)==PAPACC_RESULT_OK&&papacc_network_stream_reserve(&manager,31U,
  3101U,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&stream)==PAPACC_RESULT_OK&&
  papacc_network_stream_publish(&manager,stream,&io)==PAPACC_RESULT_OK,120);
 for(index=0U;index<4U;++index)CHECK(
  papacc_network_stream_progress_one_observed_at(&manager,256U,0U,&selected,
   &progressed)==PAPACC_RESULT_OK&&progressed==PAPACC_TRUE,121);
 CHECK(stream->to_client_size==1024U&&source.read_calls==4U,122);
 CHECK(papacc_network_stream_progress_one_observed_at(&manager,256U,0U,
  &selected,&progressed)==PAPACC_RESULT_OK&&progressed==PAPACC_FALSE&&
  source.read_calls==4U,123);
 CHECK(papacc_network_stream_close_session(&manager,31U)==PAPACC_RESULT_OK&&
  source.closed==1&&manager.count==0U,124);
 return 0;
}

int main(void)
{int r=test_capabilities();if(r!=0)return r;r=test_destination_policy();if(r!=0)return r;r=test_egress_runtime_config();if(r!=0)return r;r=test_streams();if(r!=0)return r;r=test_stream_fairness_and_isolation();if(r!=0)return r;r=test_stream_lifecycle_and_reuse();if(r!=0)return r;r=test_tls_incremental_shutdown();if(r!=0)return r;r=test_truncated_terminal_preserves_buffered_data();if(r!=0)return r;r=test_bounded_bulk_progress();if(r!=0)return r;return test_would_block_backpressure_and_cancel();}
