#include "destination.h"
#include <string.h>

static PAPACC_BOOL hostname(const PAPACC_U8 *s,PAPACC_SIZE n,char *out)
{
    PAPACC_SIZE i,label=0U;if(n==0U||n>PAPACC_DESTINATION_HOSTNAME_CAPACITY)return PAPACC_FALSE;
    for(i=0U;i<n;++i){PAPACC_U8 c=s[i];if(c>127U)return PAPACC_FALSE;if(c>='A'&&c<='Z')c=(PAPACC_U8)(c+32U);
        if(c=='.'){if(label==0U||out[i-1]=='-')return PAPACC_FALSE;label=0U;}
        else {if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='-')||label>=63U||(label==0U&&c=='-'))return PAPACC_FALSE;++label;}out[i]=(char)c;}
    if(label==0U||out[n-1]=='-')return PAPACC_FALSE;out[n]='\0';return PAPACC_TRUE;
}
static void canonicalize_endpoint(PAPACC_NETWORK_ENDPOINT *endpoint)
{
    PAPACC_U8 *b;
    if(endpoint==NULL||endpoint->address.family!=PAPACC_IP_FAMILY_IPV6)return;
    b=endpoint->address.bytes;
    if(b[0]==0U&&b[1]==0U&&b[2]==0U&&b[3]==0U&&b[4]==0U&&b[5]==0U&&
        b[6]==0U&&b[7]==0U&&b[8]==0U&&b[9]==0U&&b[10]==0xFFU&&b[11]==0xFFU){
        PAPACC_U8 a=b[12],c=b[13],d=b[14],e=b[15];
        (void)papacc_ip_address_set_ipv4(&endpoint->address,a,c,d,e);
        endpoint->scope_id=0U;
    }
}
PAPACC_RESULT papacc_destination_normalize(const PAPACC_NETWORK_OPEN *o,
    PAPACC_NORMALIZED_DESTINATION *d)
{
    PAPACC_NORMALIZED_DESTINATION v=PAPACC_NORMALIZED_DESTINATION_INITIALIZER;PAPACC_RESULT r;
    if(d!=NULL)*d=v;if(o==NULL||d==NULL||o->port==0U)return PAPACC_RESULT_INVALID_ARGUMENT;v.kind=o->destination_kind;v.port=o->port;
    if(o->destination_kind==PAPACC_NETWORK_DESTINATION_DNS_ASCII){if(hostname(o->destination,o->destination_size,v.hostname)!=PAPACC_TRUE)return PAPACC_RESULT_PROTOCOL_ERROR;v.hostname_size=o->destination_size;}
    else {r=o->destination_kind==PAPACC_NETWORK_DESTINATION_IPV4&&o->destination_size==4U?papacc_ip_address_set_ipv4(&v.literal_endpoint.address,o->destination[0],o->destination[1],o->destination[2],o->destination[3]):
        o->destination_kind==PAPACC_NETWORK_DESTINATION_IPV6&&o->destination_size==16U?papacc_ip_address_set_ipv6(&v.literal_endpoint.address,o->destination):PAPACC_RESULT_PROTOCOL_ERROR;
        if(r!=PAPACC_RESULT_OK)return r;v.literal_endpoint.port=o->port;}
    *d=v;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_destination_resolve_and_filter(const PAPACC_NORMALIZED_DESTINATION *d,
    const PAPACC_DNS_RESOLVER *resolver,const PAPACC_EGRESS_POLICY *policy,
    PAPACC_NETWORK_ENDPOINT *out,PAPACC_SIZE cap,PAPACC_SIZE *count)
{
    PAPACC_NETWORK_ENDPOINT candidates[32];PAPACC_SIZE n=0U,i,used=0U;PAPACC_RESULT r;
    if(count!=NULL)*count=0U;if(d==NULL||policy==NULL||out==NULL||count==NULL||cap==0U)return PAPACC_RESULT_INVALID_ARGUMENT;
    if(d->kind==PAPACC_NETWORK_DESTINATION_DNS_ASCII){if(resolver==NULL||resolver->resolve==NULL||
        papacc_egress_hostname_allowed(policy,d->hostname,d->hostname_size,d->port)==PAPACC_FALSE)return PAPACC_RESULT_INVALID_STATE;
        r=resolver->resolve(resolver->context,d->hostname,d->hostname_size,d->port,candidates,32U,&n);if(r!=PAPACC_RESULT_OK)return r;if(n>32U)return PAPACC_RESULT_INTERNAL_ERROR;}
    else {candidates[0]=d->literal_endpoint;n=1U;}
    for(i=0U;i<n&&used<cap;++i){canonicalize_endpoint(&candidates[i]);if(papacc_egress_endpoint_allowed(policy,&candidates[i])==PAPACC_TRUE)out[used++]=candidates[i];}
    if(used==0U)return PAPACC_RESULT_INVALID_STATE;*count=used;return PAPACC_RESULT_OK;
}
