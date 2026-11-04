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
