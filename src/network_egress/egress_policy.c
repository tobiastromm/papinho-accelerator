#include "egress_policy.h"

PAPACC_RESULT papacc_egress_profile_validate(const PAPACC_EGRESS_PROFILE *p)
{
    PAPACC_SIZE i; PAPACC_BOOL any=PAPACC_FALSE;
    if(p==NULL||p->profile_id==0U||p->available!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
    for(i=0U;i<PAPACC_EGRESS_PROFILE_NAME_CAPACITY;++i){if(p->name[i]=='\0'){any=(i>0U)?PAPACC_TRUE:PAPACC_FALSE;break;}}
    if(any!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(p->use_default_route==PAPACC_TRUE){
        if(p->interface_id.is_valid==PAPACC_TRUE||p->source_endpoint.address.family!=PAPACC_IP_FAMILY_UNSPECIFIED)
            return PAPACC_RESULT_INVALID_ARGUMENT;
    } else if(p->interface_id.is_valid!=PAPACC_TRUE&&
        p->source_endpoint.address.family==PAPACC_IP_FAMILY_UNSPECIFIED)
        return PAPACC_RESULT_INVALID_STATE;
    return PAPACC_RESULT_OK;
}

PAPACC_U32 papacc_egress_endpoint_sensitive_classes(const PAPACC_NETWORK_ENDPOINT *e)
{
    const PAPACC_U8 *b; PAPACC_U32 c=0U;
    if(e==NULL)return 0U;b=e->address.bytes;
    if(papacc_ip_address_is_loopback(&e->address)==PAPACC_TRUE)c|=PAPACC_EGRESS_SENSITIVE_LOOPBACK;
    if(e->address.family==PAPACC_IP_FAMILY_IPV4){
        if((b[0]==169U&&b[1]==254U))c|=PAPACC_EGRESS_SENSITIVE_LINK_LOCAL;
        if(b[0]==10U||(b[0]==172U&&b[1]>=16U&&b[1]<=31U)||(b[0]==192U&&b[1]==168U))c|=PAPACC_EGRESS_SENSITIVE_PRIVATE;
        if(b[0]==100U&&b[1]>=64U&&b[1]<=127U)c|=PAPACC_EGRESS_SENSITIVE_SHARED;
        if(b[0]==169U&&b[1]==254U&&b[2]==169U&&b[3]==254U)c|=PAPACC_EGRESS_SENSITIVE_METADATA;
    } else if(e->address.family==PAPACC_IP_FAMILY_IPV6){
        if(b[0]==0xFEU&&(b[1]&0xC0U)==0x80U)c|=PAPACC_EGRESS_SENSITIVE_LINK_LOCAL;
        if((b[0]&0xFEU)==0xFCU)c|=PAPACC_EGRESS_SENSITIVE_PRIVATE;
    }
    return c;
}
static PAPACC_BOOL structural(const PAPACC_NETWORK_ENDPOINT *e)
{
    const PAPACC_U8 *b;if(e==NULL||e->port==0U||papacc_network_endpoint_validate(e)!=PAPACC_RESULT_OK||
        papacc_ip_address_is_unspecified(&e->address)==PAPACC_TRUE)return PAPACC_FALSE;b=e->address.bytes;
    if(e->address.family==PAPACC_IP_FAMILY_IPV4){if(b[0]>=224U||b[0]==255U)return PAPACC_FALSE;}
    else if(e->address.family==PAPACC_IP_FAMILY_IPV6&&b[0]==0xFFU)return PAPACC_FALSE;
    return PAPACC_TRUE;
}
PAPACC_BOOL papacc_egress_hostname_allowed(const PAPACC_EGRESS_POLICY *p,
    const char *h,PAPACC_SIZE n,PAPACC_U16 port)
{return (p!=NULL&&p->master_enabled==PAPACC_TRUE&&h!=NULL&&n>0U&&port!=0U&&p->host_policy!=NULL&&p->host_policy(p->context,h,n,port)==PAPACC_EGRESS_ALLOW)?PAPACC_TRUE:PAPACC_FALSE;}
PAPACC_BOOL papacc_egress_endpoint_allowed(const PAPACC_EGRESS_POLICY *p,
    const PAPACC_NETWORK_ENDPOINT *e)
{
    PAPACC_U32 classes;if(p==NULL||p->master_enabled!=PAPACC_TRUE||structural(e)!=PAPACC_TRUE||p->endpoint_policy==NULL)return PAPACC_FALSE;
    classes=papacc_egress_endpoint_sensitive_classes(e);
    if((classes&~p->explicitly_allowed_sensitive_classes)!=0U)return PAPACC_FALSE;
    return p->endpoint_policy(p->context,e,classes)==PAPACC_EGRESS_ALLOW?PAPACC_TRUE:PAPACC_FALSE;
}
