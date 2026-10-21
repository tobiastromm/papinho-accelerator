#include "exact_policy.h"
#include <string.h>

static PAPACC_EGRESS_DECISION host_policy(void *context,const char *hostname,
    PAPACC_SIZE size,PAPACC_U16 port)
{
    PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *s=(PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *)context;
    PAPACC_SIZE i;if(s==NULL||hostname==NULL||size==0U||port==0U||
        s->valid!=PAPACC_TRUE)return PAPACC_EGRESS_ERROR;
    for(i=0U;i<s->host_rule_count;++i)if(s->host_rules[i].port==port&&
        s->host_rules[i].hostname_size==size&&memcmp(s->host_rules[i].hostname,hostname,size)==0)
        return PAPACC_EGRESS_ALLOW;return PAPACC_EGRESS_DENY;
}
static PAPACC_EGRESS_DECISION endpoint_policy(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint,PAPACC_U32 classes)
{
    PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *s=(PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *)context;
    PAPACC_SIZE i;(void)classes;if(s==NULL||endpoint==NULL||
        s->valid!=PAPACC_TRUE)return PAPACC_EGRESS_ERROR;
    for(i=0U;i<s->endpoint_rule_count;++i)if(papacc_network_endpoint_equal(
        &s->endpoint_rules[i].endpoint,endpoint))return PAPACC_EGRESS_ALLOW;
    return PAPACC_EGRESS_DENY;
}
PAPACC_RESULT papacc_exact_egress_policy_init(PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *s)
{if(s==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;memset(s,0,sizeof(*s));return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_exact_egress_policy_add_host(PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *s,
    const char *h,PAPACC_SIZE n,PAPACC_U16 p)
{PAPACC_SIZE i;if(s==NULL||h==NULL||n==0U||n>PAPACC_DESTINATION_HOSTNAME_CAPACITY||p==0U||s->valid==PAPACC_TRUE)return PAPACC_RESULT_INVALID_ARGUMENT;
 for(i=0U;i<s->host_rule_count;++i)if(s->host_rules[i].port==p&&s->host_rules[i].hostname_size==n&&memcmp(s->host_rules[i].hostname,h,n)==0)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(s->host_rule_count>=PAPACC_EXACT_EGRESS_RULE_CAPACITY)return PAPACC_RESULT_LIMIT_EXCEEDED;i=s->host_rule_count++;memcpy(s->host_rules[i].hostname,h,n);s->host_rules[i].hostname[n]='\0';s->host_rules[i].hostname_size=n;s->host_rules[i].port=p;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_exact_egress_policy_add_endpoint(PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *s,
    const PAPACC_NETWORK_ENDPOINT *e)
{PAPACC_SIZE i;if(s==NULL||e==NULL||s->valid==PAPACC_TRUE||papacc_network_endpoint_validate(e)!=PAPACC_RESULT_OK||e->port==0U)return PAPACC_RESULT_INVALID_ARGUMENT;
 for(i=0U;i<s->endpoint_rule_count;++i)if(papacc_network_endpoint_equal(&s->endpoint_rules[i].endpoint,e))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(s->endpoint_rule_count>=PAPACC_EXACT_EGRESS_RULE_CAPACITY)return PAPACC_RESULT_LIMIT_EXCEEDED;s->endpoint_rules[s->endpoint_rule_count++].endpoint=*e;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_exact_egress_policy_publish(PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *s,
    PAPACC_EGRESS_POLICY *p)
{PAPACC_SIZE i;if(s==NULL||p==NULL||s->endpoint_rule_count==0U||s->valid==PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
 *p=(PAPACC_EGRESS_POLICY)PAPACC_EGRESS_POLICY_INITIALIZER;p->master_enabled=PAPACC_TRUE;p->host_policy=host_policy;p->endpoint_policy=endpoint_policy;p->context=s;
 for(i=0U;i<s->endpoint_rule_count;++i)p->explicitly_allowed_sensitive_classes|=papacc_egress_endpoint_sensitive_classes(&s->endpoint_rules[i].endpoint);s->valid=PAPACC_TRUE;return PAPACC_RESULT_OK;}
