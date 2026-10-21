#include "../../src/core/ed11_process_profile.h"
/* Private part of the existing C2 owner. No transport or Session activation. */
#include "server_c2_owner_win32.h"
#ifndef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#error This_file_requires_the_exclusive_private_C8_type_profile
#endif
#include <string.h>
static int dormant_empty(const PAPACC_SERVER_C2_OWNER_WIN32 *o)
{
 unsigned i;
 if(o==NULL||!o->manager||!o->manager->initialized||o->manager->owner!=GetCurrentThreadId())return 0;
 if(o->configuration.io_attached){
  if(!o->initialized||!o->loop||!o->secure||!o->loop->initialized||!o->secure->initialized||
     o->loop->acceptor!=NULL||o->loop->c2_owner!=o||o->secure->protocol_loop!=o->loop)return 0;
 }else if(o->initialized||o->loop||o->secure)return 0;
 for(i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i)
  if(o->manager->sessions[i].session||o->sessions[i].initialized)return 0;
 for(i=0U;i<PAPACC_C2_OPERATION_CAPACITY;++i)
  if(o->manager->operations[i].state!=PAPACC_C2_OP_EMPTY||o->bridges[i])return 0;
 for(i=0U;i<6U;++i)if(o->writes[i].pending||o->writes[i].secret_borrow||o->writes[i].quarantined)return 0;
 return !o->e10.enabled&&!o->e10.busy;
}
static int dormant_same_model(const PAPACC_CONFIG_VIEW *a,const PAPACC_CONFIG_VIEW *b)
{
 PAPACC_SERVER_CONFIG x=a->config,y=b->config;PAPACC_SIZE count=x.bind_selection.interface_persistent_id_count;
 if(count>PAPACC_CONFIG_IDS||count!=y.bind_selection.interface_persistent_id_count)return 0;
 x.bind_selection.interface_persistent_ids=NULL;y.bind_selection.interface_persistent_ids=NULL;
 return memcmp(&x,&y,sizeof(x))==0&&(!count||memcmp(a->ids,b->ids,count*sizeof(a->ids[0]))==0);
}
PAPACC_RESULT papacc_server_c2_configuration_authority_current(const PAPACC_SERVER_C2_OWNER_WIN32 *o)
{
 const PAPACC_C2_DORMANT_CONFIGURATION *c;
 if(!o||!o->manager||!o->manager->initialized||o->manager->owner!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration;
 if(c->owner!=GetCurrentThreadId()||
    (c->state!=PAPACC_C2_CONFIG_APPLIED&&c->state!=PAPACC_C2_CONFIG_CURRENT)||
    !c->source||!c->source_ticket||!c->secure_lease||c->reserved>PAPACC_ED11_PROCESS_LIMIT||
    c->source->state!=PAPACC_SECURITY_SOURCE_PUBLISHED||
    papacc_server_security_source_validate(c->source)!=PAPACC_RESULT_OK||
    papacc_security_composition_is_ready(c->secure_lease)!=PAPACC_TRUE||
    papacc_server_config_validate(&c->view.config)!=PAPACC_RESULT_OK)
  return PAPACC_RESULT_INVALID_STATE;
 if(c->source->other_reserved>PAPACC_ED11_PROCESS_LIMIT-sizeof(*c->source)||
    c->source->credit>PAPACC_ED11_PROCESS_LIMIT-sizeof(*c->source)-c->source->other_reserved||
    c->reserved!=c->source->other_reserved+sizeof(*c->source)+c->source->credit)
  return PAPACC_RESULT_LIMIT_EXCEEDED;
 {unsigned i;int found=0;for(i=0U;i<8U;++i)if(c->source->tickets[i]==c->source_ticket)found=1;
  if(!found)return PAPACC_RESULT_INVALID_STATE;}
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_c2_configuration_validate(const PAPACC_SERVER_C2_OWNER_WIN32 *o)
{
 if(!dormant_empty(o))return PAPACC_RESULT_INVALID_STATE;
 return papacc_server_c2_configuration_authority_current(o);
}
PAPACC_RESULT papacc_server_c2_configuration_apply(PAPACC_SERVER_C2_OWNER_WIN32 *o,
 PAPACC_C2_OPERATION_MANAGER *m,const PAPACC_CONFIG_VIEW *view,
 const PAPACC_CAPABILITY_EVALUATION_INPUTS *e,PAPACC_SECURITY_SOURCE_TRANSACTION *source,
 const PAPACC_SECURITY_COMPOSITION *secure,PAPACC_SIZE reserved)
{
 const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *lease=NULL;
 PAPACC_SECURITY_CONFIGURATION_REF reference=PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;
 PAPACC_C2_DORMANT_CONFIGURATION *c;PAPACC_U64 ticket;PAPACC_RESULT result;PAPACC_SIZE i;
 if(!o||!m||!view||!e||!source||!secure)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(o->initialized||o->configuration.state!=PAPACC_C2_CONFIG_EMPTY||o->manager||
    !m->initialized||m->owner!=GetCurrentThreadId()||m->stopping||
    view->config.listener_profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL||
    !view->config.policy.present||papacc_server_config_validate(&view->config)!=PAPACC_RESULT_OK||
    view->config.bind_selection.interface_persistent_id_count>PAPACC_CONFIG_IDS||
    view->revision||view->policy_epoch||!e->registry||!e->registry->initialized||
    !e->registry->storage||e->registry->count>e->registry->capacity||
    !e->server_supported||!e->server_enabled||!e->server_supported->initialized||!e->server_enabled->initialized||
    e->server_enabled->count>3U||e->server_enabled->count>e->server_enabled->capacity||
    e->server_supported->count>e->server_supported->capacity||
    (e->server_enabled->count&&!e->server_enabled->storage)||
    (e->server_supported->count&&!e->server_supported->storage)||
    e->server_enabled->count!=view->config.policy.enabled_count||
    source->state!=PAPACC_SECURITY_SOURCE_PUBLISHED||
    papacc_server_security_source_validate(source)!=PAPACC_RESULT_OK||
    papacc_security_composition_is_ready(secure)!=PAPACC_TRUE)
  return PAPACC_RESULT_INVALID_STATE;
 if(reserved<sizeof(*o)+sizeof(*m)+sizeof(*source)||reserved>PAPACC_ED11_PROCESS_LIMIT)return PAPACC_RESULT_LIMIT_EXCEEDED;
 if(source->other_reserved>PAPACC_ED11_PROCESS_LIMIT-sizeof(*source)||
    source->credit>PAPACC_ED11_PROCESS_LIMIT-sizeof(*source)-source->other_reserved||
    reserved!=source->other_reserved+sizeof(*source)+source->credit)
  return PAPACC_RESULT_LIMIT_EXCEEDED;
 /* Initial dormant apply consumes fresh zeroed durable storage. Reject any
  * stale callback/bridge/transport/observer reference before borrowing. */
 {const unsigned char *bytes=(const unsigned char *)o;
  for(i=0U;i<sizeof(*o);++i)if(bytes[i])return PAPACC_RESULT_INVALID_STATE;}
 for(i=0U;i<PAPACC_C2_SESSION_CAPACITY;++i)if(m->sessions[i].session)return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<PAPACC_C2_OPERATION_CAPACITY;++i)if(m->operations[i].state!=PAPACC_C2_OP_EMPTY)return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<e->server_enabled->count;++i){const PAPACC_CAPABILITY_DESCRIPTOR *d;
  if(!papacc_capability_set_contains(e->server_supported,&e->server_enabled->storage[i])||
     papacc_capability_registry_find(e->registry,&e->server_enabled->storage[i],&d)!=PAPACC_RESULT_OK)
   return PAPACC_RESULT_INVALID_STATE;
 }
 for(i=0U;i<view->config.policy.enabled_count;++i)
  if(!papacc_capability_set_contains(e->server_enabled,&view->config.policy.enabled[i]))return PAPACC_RESULT_INVALID_STATE;
 result=papacc_security_configuration_ref_set(&reference,view->config.security_configuration_ref,view->config.security_configuration_ref_length);
 if(result!=PAPACC_RESULT_OK||!papacc_security_configuration_ref_equal(&reference,&source->reference))return PAPACC_RESULT_INVALID_STATE;
 result=papacc_server_security_source_borrow(source,&ticket,&lease);
 if(result!=PAPACC_RESULT_OK)return result;
 /* No failing operations follow this ownership transfer. Dependencies are
  * durable leases; all model/IDs/keys are copied, no caller stack survives. */
 o->manager=m;c=&o->configuration;c->view=*view;
 if(view->config.bind_selection.interface_persistent_id_count)
  memcpy(c->view.ids,view->config.bind_selection.interface_persistent_ids,view->config.bind_selection.interface_persistent_id_count*sizeof(c->view.ids[0]));
 c->view.config.bind_selection.interface_persistent_ids=view->config.bind_selection.interface_persistent_id_count?c->view.ids:NULL;
 c->source=source;c->secure_lease=secure;c->source_ticket=ticket;c->owner=GetCurrentThreadId();c->reserved=reserved;
 c->enabled_count=e->server_enabled->count;
 if(c->enabled_count)memcpy(c->enabled,e->server_enabled->storage,c->enabled_count*sizeof(c->enabled[0]));
 c->state=PAPACC_C2_CONFIG_APPLIED;(void)lease;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_c2_configuration_associate(PAPACC_SERVER_C2_OWNER_WIN32 *o,const PAPACC_CONFIG_VIEW *v,PAPACC_U64 epoch)
{
 PAPACC_C2_DORMANT_CONFIGURATION *c;unsigned i,nonzero=0;
 if(!v||!epoch||papacc_server_c2_configuration_validate(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration;
 for(i=0U;i<32U;++i)nonzero|=v->domain[i];
 if(!nonzero||!v->revision||!dormant_same_model(&c->view,v))return PAPACC_RESULT_INVALID_STATE;
 if(c->state==PAPACC_C2_CONFIG_CURRENT)
  return c->epoch==epoch&&c->view.revision==v->revision&&memcmp(c->view.domain,v->domain,32U)==0?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
 memcpy(c->view.domain,v->domain,32U);c->view.revision=v->revision;c->view.predecessor=v->predecessor;
 c->epoch=epoch;c->state=PAPACC_C2_CONFIG_CURRENT;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_c2_configuration_unapply(PAPACC_SERVER_C2_OWNER_WIN32 *o)
{
 PAPACC_C2_DORMANT_CONFIGURATION *c;PAPACC_RESULT result;
 if(papacc_server_c2_configuration_validate(o)!=PAPACC_RESULT_OK||o->configuration.io_attached)return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration;
 result=papacc_server_security_source_unborrow(c->source,c->source_ticket);
 if(result!=PAPACC_RESULT_OK)return result;
 c->source_ticket=0U;c->source=NULL;c->secure_lease=NULL;c->epoch=0U;
 c->state=PAPACC_C2_CONFIG_CLOSED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_c2_configuration_retain(PAPACC_SERVER_C2_OWNER_WIN32 *o)
{
 if(!o||o->configuration.owner!=GetCurrentThreadId()||
    (o->configuration.state!=PAPACC_C2_CONFIG_APPLIED&&o->configuration.state!=PAPACC_C2_CONFIG_CURRENT))
  return PAPACC_RESULT_INVALID_STATE;
 o->configuration.state=PAPACC_C2_CONFIG_RETAINED;return PAPACC_RESULT_OK;
}
