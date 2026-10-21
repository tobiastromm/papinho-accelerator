/* Existing root, immutable committed pin and existing listener intent only.
 * No bind/listen, source/config recreation, grant, callback or PST release. */
static int root_listener_intent_equal(const PAPACC_SERVER_LISTENER_CONFIGURATION *l,
 const PAPACC_SERVER_CONFIG *c,const PAPACC_SECURITY_CONFIGURATION_REF *reference)
{
 PAPACC_SIZE i,count=c->bind_selection.interface_persistent_id_count;
 if(count>PAPACC_CONFIG_IDS||l->port!=c->control_port||
    l->transport_profile!=PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL||
    c->listener_profile!=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL||
    l->bind_selection.mode!=c->bind_selection.mode||
    l->bind_selection.interface_persistent_id_count!=count||
    !papacc_security_configuration_ref_equal(&l->security_configuration_ref,reference))return 0;
 if(count&&(!l->bind_selection.interface_persistent_ids||!c->bind_selection.interface_persistent_ids))return 0;
 for(i=0U;i<count;++i)
  if(l->bind_selection.interface_persistent_ids[i].is_valid!=c->bind_selection.interface_persistent_ids[i].is_valid||
     l->bind_selection.interface_persistent_ids[i].value!=c->bind_selection.interface_persistent_ids[i].value)return 0;
 return 1;
}
static PAPACC_RESULT root_listener_check_mode(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,int active)
{
 PAPACC_ROOT_LISTENER_COMPOSITION *l;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
 PAPACC_C2_DORMANT_CONFIGURATION *c;PAPACC_ADMISSION_PREFLIGHT report;PAPACC_SIZE i;
 if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
 l=&o->listener_composition;t=&o->configuration_preparation;c=&configuration_c2_owner.configuration;
 if(l->invocation!=o->invocation||!l->pin.pin.ticket||
    (l->state!=PAPACC_ROOT_LISTENER_PREPARED&&l->state!=PAPACC_ROOT_LISTENER_VALIDATED&&l->state!=PAPACC_ROOT_LISTENER_READY)||
    l->reserved!=t->combined_reserved||
    root_cohort_preflight(o,&l->pin,&report,active)!=PAPACC_RESULT_OK)
  return PAPACC_RESULT_INVALID_STATE;
 /* Include predecessor and full domain/revision, not only U64 epoch equality. */
 if(l->pin.view.predecessor!=t->model.predecessor||l->pin.view.predecessor!=c->view.predecessor||
    l->pin.view.revision!=c->view.revision||memcmp(l->pin.view.domain,c->view.domain,32U)||
    !root_listener_intent_equal(&t->participants.listener,&l->pin.view.config,&t->source.reference)||
    !root_listener_intent_equal(&o->secure_listener,&l->pin.view.config,&t->source.reference)||
    o->secure_io.security_composition!=&o->security_composition||
    o->secure_io.security_configuration!=&t->source.configuration||
    c->enabled_count!=t->enabled.count||c->enabled_count!=l->pin.view.config.policy.enabled_count)
  return PAPACC_RESULT_INVALID_STATE;
 for(i=0U;i<c->enabled_count;++i)
  if(c->enabled[i].id!=t->enabled.storage[i].id||c->enabled[i].major!=t->enabled.storage[i].major||
     !papacc_capability_set_contains(&t->enabled,&l->pin.view.config.policy.enabled[i]))return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK;
}
static PAPACC_RESULT root_listener_check(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{return root_listener_check_mode(o,0);}
PAPACC_BOOL papacc_server_launcher_active_cohort_current(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 return preparation_owned(o)&&o->listener_composition.state==PAPACC_ROOT_LISTENER_READY&&
  root_listener_check_mode(o,1)==PAPACC_RESULT_OK?PAPACC_TRUE:PAPACC_FALSE;
}
PAPACC_RESULT papacc_server_launcher_listener_prepare(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_LISTENER_COMPOSITION *l;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;
 PAPACC_ADMISSION_PREFLIGHT report;PAPACC_RESULT result;
 if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
 l=&o->listener_composition;t=&o->configuration_preparation;
 if(l->state!=PAPACC_ROOT_LISTENER_EMPTY||l->pin.pin.ticket||o->attachment_state!=3U)
  return PAPACC_RESULT_INVALID_STATE;
 /* CLOSED may be captured as PREPARED intent, but never validates without the
  * separately governed access authentication. No such provider is invented. */
 result=papacc_server_launcher_admission_preflight(o,&o->attachment_pin,&report);
 if(result!=PAPACC_RESULT_OK&&report.missing!=PAPACC_ADMISSION_ACCESS)return PAPACC_RESULT_INVALID_STATE;
 result=papacc_server_launcher_configuration_current_pin(o,&l->pin);
 if(result!=PAPACC_RESULT_OK)return result;
 l->invocation=o->invocation;l->reserved=t->combined_reserved;l->state=PAPACC_ROOT_LISTENER_PREPARED;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_listener_validate(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 if(!preparation_owned(o)||o->listener_composition.state!=PAPACC_ROOT_LISTENER_PREPARED||
    root_listener_check(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 o->listener_composition.state=PAPACC_ROOT_LISTENER_VALIDATED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_listener_ready(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 if(!preparation_owned(o)||o->listener_composition.state!=PAPACC_ROOT_LISTENER_VALIDATED||
    root_listener_check(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 o->listener_composition.state=PAPACC_ROOT_LISTENER_READY;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_listener_report(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ROOT_LISTENER_REPORT *out)
{
 PAPACC_ROOT_LISTENER_COMPOSITION *l;
 if(!out)return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 if(o!=&root||GetCurrentThreadId()!=o->owner_thread||
    (papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_ACTIVE&&papacc_server_launcher_owner_state()!=PAPACC_LAUNCHER_RETAINED))return PAPACC_RESULT_INVALID_STATE;
 l=&o->listener_composition;
 if(l->invocation!=o->invocation)return PAPACC_RESULT_INVALID_STATE;
 memcpy(out->domain,l->pin.view.domain,32U);out->invocation=l->invocation;
 out->revision=l->pin.view.revision;out->predecessor=l->pin.view.predecessor;
 out->epoch=l->pin.epoch;out->reserved=l->reserved;out->state=(PAPACC_U32)l->state;
 out->profile=(PAPACC_U32)l->pin.view.config.listener_profile;out->admission=l->pin.view.config.policy.admission;
 out->retained=papacc_server_launcher_owner_state()==PAPACC_LAUNCHER_RETAINED?PAPACC_TRUE:PAPACC_FALSE;
 out->ready=l->state==PAPACC_ROOT_LISTENER_READY&&root_listener_check_mode(o,o->network.is_active?1:0)==PAPACC_RESULT_OK?PAPACC_TRUE:PAPACC_FALSE;
 out->published=o->network.is_active&&o->activation_lifetime.phase==2U?PAPACC_TRUE:PAPACC_FALSE;
 /* This report is observation only; publication_allowed remains false. */
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_listener_abort(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 PAPACC_ROOT_LISTENER_COMPOSITION *l;PAPACC_RESULT result;
 if(!preparation_owned(o))return PAPACC_RESULT_INVALID_STATE;
 l=&o->listener_composition;
 if(papacc_server_c2_owner_win32_retire_sessions(&configuration_c2_owner)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 if(papacc_server_c2_owner_win32_has_dependencies(&configuration_c2_owner))return PAPACC_RESULT_INVALID_STATE;
 {unsigned i;for(i=0U;i<4U;++i)if(configuration_c2_owner.sessions[i].initialized)return PAPACC_RESULT_INVALID_STATE;}
 if(l->invocation!=o->invocation||!l->pin.pin.ticket||
    (l->state!=PAPACC_ROOT_LISTENER_PREPARED&&l->state!=PAPACC_ROOT_LISTENER_VALIDATED&&l->state!=PAPACC_ROOT_LISTENER_READY)||
    o->network.is_active||o->network.listener_set.count)return PAPACC_RESULT_INVALID_STATE;
 result=papacc_server_launcher_configuration_current_unpin(o,&l->pin);
 if(result!=PAPACC_RESULT_OK){l->state=PAPACC_ROOT_LISTENER_RETAINED;return attachment_retain(o,result);}
 l->pin.pin.ticket=0U;l->state=PAPACC_ROOT_LISTENER_CLOSED;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_server_launcher_listener_publication_preflight(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o)
{
 if(root_listener_check(o)!=PAPACC_RESULT_OK||o->listener_composition.state!=PAPACC_ROOT_LISTENER_READY)
  return PAPACC_RESULT_INVALID_STATE;
 /* READY alone never authorizes bind, accept, authentication or grants.
  * Operational evidence is independent and UNKNOWN denies. */
 return root_resource_current(o);
}
