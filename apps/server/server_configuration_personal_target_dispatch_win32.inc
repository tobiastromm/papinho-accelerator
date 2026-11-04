static PAPACC_RESULT personal_target_start(PAPACC_CONFIGURATION_COMMIT *o,
 const unsigned char id[32],PAPACC_SIZE reserved,int lab)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if((!owner(o)&&!(o&&!o->initialized&&o->writer==GetCurrentThreadId()&&o->pending_commit.personal_native.first_owner==&o->pending_commit))||!id||o->stopped||o->unresolved||o->published||o->prepared||o->committed_pending||o->committed_bytes)return PAPACC_RESULT_INVALID_STATE;
 p=&o->pending_commit;n=&p->personal_native;
 if(p->transaction||p->close_error||p->active||p->thread||p->bootstrap_created||p->bootstrap_container||!p->bootstrap_parent||
    n->handoff_state||n->phase!=1U||p->personal_data.phase!=PAPACC_PERSONAL_DATA_CONFIRMED||p->personal_data.consumed||
    memcmp(id,p->personal_data.attempt,32U)||memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf)))return PAPACC_RESULT_INVALID_STATE;
 if(reserved>PAPACC_ED11_PROCESS_LIMIT||reserved<sizeof(*o)+PAPACC_PENDING_STACK_BYTES)return PAPACC_RESULT_LIMIT_EXCEEDED;
 if(!bootstrap_identity(p->bootstrap_parent,&p->bootstrap_parent_volume,&p->bootstrap_parent_high,&p->bootstrap_parent_low,0))return PAPACC_RESULT_INVALID_STATE;
 if(!lab&&(papacc_configuration_personal_native_current(o)!=PAPACC_RESULT_OK||
    papacc_configuration_personal_source_current(o)!=PAPACC_RESULT_OK))return PAPACC_RESULT_INVALID_STATE;
 if(n->first_owner==p&&(!personal_first_ready(p,0)||n->first_phase!=2U))return PAPACC_RESULT_INVALID_STATE;
 if(lab){if(!papacc_personal_data_consume(&p->personal_data,id,p->next,p->personal_data.bytes))return PAPACC_RESULT_INVALID_STATE;}
 else if(!papacc_personal_effect_consume(&p->personal_data,id,p->previous+1024U,p->personal_data.bytes))return PAPACC_RESULT_INVALID_STATE;
 memcpy(n->handoff_attempt,id,32U);p->bootstrap_admin=0U;
 if(n->first_owner==p)n->first_phase=3U;
 p->packet_format=13U;p->result=PAPACC_COMMIT_PENDING;p->active=1;
 InterlockedExchange(&n->handoff_state,PAPACC_PERSONAL_HANDOFF_SUBMITTED);
 p->thread=CreateThread(NULL,PAPACC_PENDING_STACK_BYTES,pending_worker,p,STACK_SIZE_PARAM_IS_A_RESERVATION,NULL);
 if(!p->thread){p->active=0;p->native_error=GetLastError();InterlockedExchange(&n->handoff_state,PAPACC_PERSONAL_HANDOFF_DENIED);return PAPACC_RESULT_INTERNAL_ERROR;}
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_target_begin(PAPACC_CONFIGURATION_COMMIT *o,
 const unsigned char id[32],PAPACC_SIZE reserved)
{
 if(!o||!id||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||
    o->pending_commit.personal_data.consumed||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 /* Read-only F10 is not an effect grant. The independent canonical material,
  * authority and capacity producer must qualify before the shared start. */
 if(!o->pending_commit.personal_data.effectful)return PAPACC_RESULT_NOT_SUPPORTED;
 if(o->pending_commit.personal_native.first_owner==&o->pending_commit){
  if(!personal_first_ready(&o->pending_commit,0))return PAPACC_RESULT_INVALID_STATE;
 }else if(!personal_native_ceremony_ready(&o->pending_commit))return PAPACC_RESULT_NOT_SUPPORTED;
 return personal_target_start(o,id,reserved,0);
}
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
PAPACC_RESULT papacc_configuration_personal_target_component_begin(PAPACC_CONFIGURATION_COMMIT *o,
 const unsigned char id[32],PAPACC_SIZE reserved)
{
 if(!o||o->pending_commit.checkpoint_only!=93)return PAPACC_RESULT_INVALID_STATE;
 return personal_target_start(o,id,reserved,1);
}
#endif
PAPACC_RESULT papacc_configuration_personal_handoff_poll(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;DWORD wait;
 if(!o||o->writer!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;p=&o->pending_commit;
 if((p->packet_format!=12U&&p->packet_format!=13U)||p->thread_close_attempted)return PAPACC_RESULT_INVALID_STATE;
 if(p->personal_native.handoff_state==PAPACC_PERSONAL_HANDOFF_RETAINED){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(!p->active)return p->thread?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
 wait=WaitForSingleObject(p->thread,0);
 if(wait==WAIT_TIMEOUT)return PAPACC_RESULT_OK;
 if(wait!=WAIT_OBJECT_0){o->unresolved=1;return PAPACC_RESULT_INTERNAL_ERROR;}
 /* The signal proves quiescence, not closure. Never free on timeout. */
 p->thread_close_attempted=1;
 if(!CloseHandle(p->thread)){p->close_error=GetLastError();o->unresolved=1;InterlockedExchange(&p->personal_native.handoff_state,PAPACC_PERSONAL_HANDOFF_RETAINED);return PAPACC_RESULT_INTERNAL_ERROR;}
 p->thread=NULL;p->active=0;
 if(p->personal_native.phase==2U||p->result==PAPACC_CAS_UNKNOWN){o->unresolved=1;return PAPACC_RESULT_INTERNAL_ERROR;}
 return PAPACC_RESULT_OK;
}
