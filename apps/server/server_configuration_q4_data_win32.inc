/* Exact logical root/pin association only; no durable use is inferred. */
PAPACC_RESULT papacc_configuration_q4_binding_data(PAPACC_CONFIGURATION_COMMIT *o,
 const PAPACC_CONFIG_PIN *p,PAPACC_Q4_ROOT_BINDING_DATA *out)
{
 PAPACC_Q4_ROOT_BINDING_DATA value;PAPACC_CONFIG_SLOT *slot;
 PAPACC_RESULT result=PAPACC_RESULT_INVALID_STATE;unsigned i,nonzero=0U;
 if(!owner(o)||!p||!out||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS||
  issuer06_overlap(out,sizeof(*out),o,sizeof(*o))||
  issuer06_overlap(out,sizeof(*out),p,sizeof(*p)))return PAPACC_RESULT_INVALID_ARGUMENT;
 EnterCriticalSection(&o->lock);
 if(!o->published||o->stopped||o->unresolved||o->prepared||o->committed_pending||
  o->recovery_selected||pending_blocks(o)||PAPACC_V3_UPDATE_BUSY(o)||
  p->index!=o->current||!p->ticket||!p->serial)goto done;
 slot=&o->slots[p->index];
 if(!slot->readers||slot->retained||slot->serial!=p->serial||
  !slot->view.revision||slot->view.predecessor!=slot->view.revision-1U||
  !slot->context_epoch||!slot->operational_epoch||
  memcmp(slot->view.domain,o->domain,32U))goto done;
 for(i=0U;i<32U;i++)if(o->tickets[i].issuer==o&&o->tickets[i].ticket==p->ticket&&
  o->tickets[i].index==p->index&&o->tickets[i].serial==p->serial)break;
 if(i==32U)goto done;
 for(i=0U;i<32U;i++)nonzero|=o->domain[i];
 if(!nonzero)goto done;
 memset(&value,0,sizeof(value));value.version=1U;value.scope_data_only=1U;
 memcpy(value.domain,o->domain,32U);value.revision=slot->view.revision;
 value.predecessor=slot->view.predecessor;value.administrative_epoch=slot->context_epoch;
 value.policy_epoch=slot->operational_epoch;value.ticket=p->ticket;value.serial=p->serial;
 *out=value;result=PAPACC_RESULT_OK;
done:LeaveCriticalSection(&o->lock);return result;
}

/* Existing owner/pin guard is mandatory for each new worker boundary. */
PAPACC_RESULT papacc_configuration_q4_binding_current(PAPACC_CONFIGURATION_COMMIT *o,
 const PAPACC_CONFIG_PIN *p,const PAPACC_Q4_ROOT_BINDING_DATA *expected)
{
 PAPACC_Q4_ROOT_BINDING_DATA current;PAPACC_RESULT result;
 if(!o||!p||!expected||issuer06_overlap(expected,sizeof(*expected),o,sizeof(*o))||
  issuer06_overlap(expected,sizeof(*expected),p,sizeof(*p)))return PAPACC_RESULT_INVALID_ARGUMENT;
 result=papacc_configuration_q4_binding_data(o,p,&current);
 if(result!=PAPACC_RESULT_OK)return result;
 return memcmp(expected,&current,sizeof(current))?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}

/* Accepted TC strict profile. No optional UNKNOWN exception selected. */
static const unsigned char q4_tc_ek[32]={0x1cU,0x64U,0x30U,0x66U,0xb1U,0x7bU,0xdfU,0x9eU,0x81U,0xc0U,0x98U,0x70U,0x66U,0x1aU,0xebU,0x7cU,0x9cU,0xffU,0xf1U,0xf2U,0xffU,0xbeU,0xf7U,0x1bU,0x43U,0x53U,0x2cU,0x14U,0xa2U,0x5cU,0x47U,0x4eU};
static const unsigned char q4_tc_cert[32]={0x96U,0x5fU,0x56U,0xaaU,0x0dU,0x4cU,0x5bU,0x57U,0x72U,0xb4U,0x23U,0xd6U,0x5dU,0x8aU,0x06U,0xa6U,0x23U,0x10U,0x9fU,0x58U,0x28U,0xc0U,0xedU,0xdcU,0xb8U,0xecU,0x4aU,0xe8U,0xfaU,0x3dU,0xe7U,0x61U};
static const unsigned char q4_tc_anchor[32]={0x2eU,0x1bU,0x3bU,0xa7U,0x9aU,0xf5U,0x6dU,0x75U,0x8bU,0xe5U,0x16U,0x97U,0x62U,0x1bU,0xc4U,0xb9U,0xe8U,0xceU,0xe0U,0x98U,0x3dU,0xb3U,0xe7U,0x49U,0xc5U,0x5eU,0xb9U,0xb3U,0x7cU,0x6dU,0x2aU,0xe0U};
static int q4_tc_pair(const unsigned char *a,const unsigned char *b)
{
 unsigned i,n=0U,d=0U;for(i=0U;i<32U;i++){n|=a[i];d|=a[i]^b[i];}return n!=0U&&d==0U;
}
PAPACC_RESULT papacc_configuration_q4_tc_policy_data(PAPACC_CONFIGURATION_COMMIT *o,
 const PAPACC_CONFIG_PIN *live_pin,const PAPACC_Q4_TC_REVIEW *review,PAPACC_Q4_TC_POLICY_DATA *out)
{
 PAPACC_Q4_TC_POLICY_DATA value;PAPACC_RESULT result;unsigned i;
 if(!o||!live_pin||!review||!out||
  issuer06_overlap(review,sizeof(*review),o,sizeof(*o))||issuer06_overlap(review,sizeof(*review),live_pin,sizeof(*live_pin))||
  issuer06_overlap(out,sizeof(*out),o,sizeof(*o))||issuer06_overlap(out,sizeof(*out),live_pin,sizeof(*live_pin))||
  issuer06_overlap(out,sizeof(*out),review,sizeof(*review)))return PAPACC_RESULT_INVALID_ARGUMENT;
 result=papacc_configuration_q4_binding_current(o,live_pin,&review->root);
 if(result!=PAPACC_RESULT_OK)return result;
 if(review->version!=1U||review->profile!=1U||review->written>1U)return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(&value,0,sizeof(value));value.version=1U;value.scope_data_only=1U;value.authority_denied=1U;value.missing=PAPACC_Q4_TC_SOURCE_MISSING;
 for(i=0U;i<PAPACC_Q4_TC_FACTS;i++)if(review->facts[i]!=PAPACC_Q4_TC_VERIFIED)value.missing|=1UL<<i;
 if(memcmp(review->ek_spki,q4_tc_ek,32U)||memcmp(review->certificate,q4_tc_cert,32U)||memcmp(review->anchor,q4_tc_anchor,32U))value.missing|=1UL<<12;
 if(!q4_tc_pair(review->approved_generation,review->current_generation))value.missing|=1UL<<13;
 if(!q4_tc_pair(review->approved_custody,review->current_custody))value.missing|=1UL<<14;
 if(!q4_tc_pair(review->original_predecessor,review->current_predecessor))value.missing|=1UL<<15;
 if(review->index!=0x01804143U||review->attributes!=(0x00040014U|(review->written?0x20000000U:0U))||review->data_bytes!=8U)value.missing|=1UL<<16;
 if(review->baseline==(PAPACC_U64)-1||review->expected!=review->baseline+1U||review->observed!=review->baseline)value.missing|=1UL<<17;
 /* Supplied statuses/digests are not native authentication or authority.
  * This DATA result NEVER clears SOURCE_MISSING or grants any effect. */
 *out=value;return PAPACC_RESULT_OK;
}

/* Normative designation and actual delivery/authority remain separate. */
static const unsigned char q4_st20_custodian[32]={0xdbU,0xc8U,0x28U,0x8dU,0xedU,0xf0U,0xf7U,0x1bU,0x3cU,0x8fU,0x92U,0x82U,0x03U,0x97U,0x7eU,0x84U,0xebU,0x41U,0x59U,0x90U,0xe6U,0x58U,0xb1U,0x28U,0xd0U,0xc9U,0x78U,0x6bU,0xa9U,0x1aU,0xd5U,0xfcU};
static const unsigned char q4_st20_source[32]={0x2dU,0x50U,0x21U,0x78U,0x8cU,0x01U,0xbeU,0xf6U,0xbfU,0xa2U,0x1aU,0x47U,0x64U,0x70U,0xddU,0x5bU,0x47U,0x2fU,0x16U,0xf2U,0x36U,0x5eU,0x8cU,0x67U,0x01U,0x5aU,0xc4U,0x2aU,0x5fU,0x70U,0x94U,0x69U};
PAPACC_RESULT papacc_configuration_q4_st20_designation_data(PAPACC_CONFIGURATION_COMMIT *o,
 const PAPACC_CONFIG_PIN *p,PAPACC_Q4_ST20_DESIGNATION_DATA *out)
{
 PAPACC_Q4_ST20_DESIGNATION_DATA value;PAPACC_RESULT result;
 if(!o||!p||!out||issuer06_overlap(out,sizeof(*out),o,sizeof(*o))||issuer06_overlap(out,sizeof(*out),p,sizeof(*p)))return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(&value,0,sizeof(value));result=papacc_configuration_q4_binding_data(o,p,&value.root);
 if(result!=PAPACC_RESULT_OK)return result;
 value.version=1U;value.scope_data_only=1U;value.custodian_designated=1U;value.source_designated=1U;
 value.source_type=13U;value.index=0x01804143U;value.credential_access_denied=1U;
 memcpy(value.custodian_sid,"S-1-5-21-785699550-1440458426-1682014700-1108",sizeof("S-1-5-21-785699550-1440458426-1682014700-1108"));
 memcpy(value.custodian_decision,q4_st20_custodian,32U);memcpy(value.source_decision,q4_st20_source,32U);
 memcpy(value.ek_spki,q4_tc_ek,32U);memcpy(value.certificate,q4_tc_cert,32U);memcpy(out,&value,sizeof(value));return PAPACC_RESULT_OK;
}
