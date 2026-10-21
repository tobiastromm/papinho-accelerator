#include "server_egress_source_win32.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int ref_equal(const PAPACC_U8 *value,PAPACC_SIZE size,const char *expected)
{return expected!=NULL&&strlen(expected)==size&&memcmp(value,expected,size)==0;}
static PAPACC_RESULT select_profile(void *context,const PAPACC_NETWORK_ENDPOINT *e,
    PAPACC_NETWORK_TRANSPORT_MODE mode,PAPACC_U32 *id)
{PAPACC_SERVER_EGRESS_SOURCE_WIN32 *s=(PAPACC_SERVER_EGRESS_SOURCE_WIN32 *)context;
 (void)e;(void)mode;if(s==NULL||id==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;*id=s->profile.profile_id;return PAPACC_RESULT_OK;}
static PAPACC_RESULT resolve_dns(void *context,const char *hostname,
    PAPACC_SIZE hostname_size,PAPACC_U16 port,PAPACC_NETWORK_ENDPOINT *out,
    PAPACC_SIZE capacity,PAPACC_SIZE *count)
{
 PAPACC_SERVER_EGRESS_SOURCE_WIN32 *s=
  (PAPACC_SERVER_EGRESS_SOURCE_WIN32 *)context;
 PAPACC_SIZE i,used=0U;
 if(count!=NULL)*count=0U;
 if(s==NULL||hostname==NULL||hostname_size==0U||out==NULL||count==NULL||
    capacity==0U||port==0U)return PAPACC_RESULT_INVALID_ARGUMENT;
 for(i=0U;i<s->snapshot.endpoint_rule_count&&used<capacity;++i){
  if(s->snapshot.endpoint_rules[i].endpoint.port==port)
   out[used++]=s->snapshot.endpoint_rules[i].endpoint;
 }
 *count=used;
 return used!=0U?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}

static PAPACC_RESULT read_environment(const char *name,char *value,size_t capacity,
    PAPACC_BOOL *present)
{
 size_t required=0U;
 if(name==NULL||value==NULL||capacity==0U||present==NULL)
  return PAPACC_RESULT_INVALID_ARGUMENT;
 value[0]='\0';*present=PAPACC_FALSE;
 if(getenv_s(&required,value,capacity,name)!=0)return PAPACC_RESULT_INTERNAL_ERROR;
 if(required==0U)return PAPACC_RESULT_OK;
 if(required>capacity)return PAPACC_RESULT_LIMIT_EXCEEDED;
 *present=PAPACC_TRUE;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_egress_environment_resolve(const PAPACC_SERVER_CONFIG *c,
    PAPACC_SERVER_EGRESS_SOURCE_WIN32 *s)
{PAPACC_SIZE i;char pr[65],fr[65];PAPACC_BOOL pr_present,fr_present;
 if(c==NULL||s==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;*s=(PAPACC_SERVER_EGRESS_SOURCE_WIN32)PAPACC_SERVER_EGRESS_SOURCE_WIN32_INITIALIZER;
 if(read_environment("PAPACC_EGRESS_POLICY_REF",pr,sizeof(pr),&pr_present)!=PAPACC_RESULT_OK||read_environment("PAPACC_EGRESS_PROFILE_REF",fr,sizeof(fr),&fr_present)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 if(c->allow_network_egress!=PAPACC_TRUE||c->listener_profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL||pr_present!=PAPACC_TRUE||fr_present!=PAPACC_TRUE||strcmp(fr,"default-route")!=0||!ref_equal(c->egress_policy_ref,c->egress_policy_ref_length,pr)||!ref_equal(c->egress_profile_ref,c->egress_profile_ref_length,fr))return PAPACC_RESULT_INVALID_STATE;
 if(papacc_exact_egress_policy_init(&s->snapshot)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INTERNAL_ERROR;
 for(i=0U;i<PAPACC_EXACT_EGRESS_RULE_CAPACITY;++i){char hn[48],en[48],pn[48],h[256],a[48],pt[16],*end;PAPACC_BOOL hp,ap,pp;unsigned long port;PAPACC_NETWORK_ENDPOINT ep=PAPACC_NETWORK_ENDPOINT_INITIALIZER;PAPACC_NETWORK_OPEN o=PAPACC_NETWORK_OPEN_INITIALIZER;PAPACC_NORMALIZED_DESTINATION n=PAPACC_NORMALIZED_DESTINATION_INITIALIZER;IN_ADDR v4;
  sprintf_s(hn,sizeof(hn),"PAPACC_EGRESS_RULE_%u_HOST",(unsigned)i);sprintf_s(en,sizeof(en),"PAPACC_EGRESS_RULE_%u_IPV4",(unsigned)i);sprintf_s(pn,sizeof(pn),"PAPACC_EGRESS_RULE_%u_PORT",(unsigned)i);if(read_environment(hn,h,sizeof(h),&hp)!=PAPACC_RESULT_OK||read_environment(en,a,sizeof(a),&ap)!=PAPACC_RESULT_OK||read_environment(pn,pt,sizeof(pt),&pp)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;if(hp!=PAPACC_TRUE&&ap!=PAPACC_TRUE&&pp!=PAPACC_TRUE)continue;if(ap!=PAPACC_TRUE||pp!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;end=NULL;port=strtoul(pt,&end,10);if(end==pt||*end!='\0'||port==0UL||port>65535UL||InetPtonA(AF_INET,a,&v4)!=1)return PAPACC_RESULT_INVALID_STATE;{const PAPACC_U8 *b=(const PAPACC_U8 *)&v4;if(papacc_ip_address_set_ipv4(&ep.address,b[0],b[1],b[2],b[3])!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;}ep.port=(PAPACC_U16)port;if(papacc_exact_egress_policy_add_endpoint(&s->snapshot,&ep)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
  if(hp==PAPACC_TRUE){size_t z=strlen(h);if(z==0U||z>sizeof(o.destination))return PAPACC_RESULT_INVALID_STATE;o.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;o.transport_mode=PAPACC_NETWORK_TRANSPORT_TCP_PLAIN;o.port=(PAPACC_U16)port;o.destination_size=(PAPACC_U16)z;memcpy(o.destination,h,z);if(papacc_destination_normalize(&o,&n)!=PAPACC_RESULT_OK||papacc_exact_egress_policy_add_host(&s->snapshot,n.hostname,n.hostname_size,n.port)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;}}
 {char overflow[2];PAPACC_BOOL present;
  if(read_environment("PAPACC_EGRESS_RULE_16_HOST",overflow,sizeof(overflow),&present)!=PAPACC_RESULT_OK||present==PAPACC_TRUE||read_environment("PAPACC_EGRESS_RULE_16_IPV4",overflow,sizeof(overflow),&present)!=PAPACC_RESULT_OK||present==PAPACC_TRUE||read_environment("PAPACC_EGRESS_RULE_16_PORT",overflow,sizeof(overflow),&present)!=PAPACC_RESULT_OK||present==PAPACC_TRUE)return PAPACC_RESULT_LIMIT_EXCEEDED;}
 if(papacc_exact_egress_policy_publish(&s->snapshot,&s->policy)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 s->profile.profile_id=1U;memcpy(s->profile.name,"default-route",14U);s->profile.use_default_route=PAPACC_TRUE;s->profile.available=PAPACC_TRUE;
 s->runtime.allow_network_egress=PAPACC_TRUE;s->runtime.policy=&s->policy;s->runtime.profiles=&s->profile;s->runtime.profile_count=1U;s->runtime.select_profile=select_profile;s->runtime.select_profile_context=s;s->resolver.resolve=resolve_dns;s->resolver.context=s;return PAPACC_RESULT_OK;}
