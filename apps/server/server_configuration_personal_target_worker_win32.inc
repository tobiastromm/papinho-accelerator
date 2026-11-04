static void personal_target_worker(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;int lab=0;
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
 lab=p->checkpoint_only==93;
#endif
 if(InterlockedCompareExchange(&n->handoff_state,0,0)!=PAPACC_PERSONAL_HANDOFF_SUBMITTED)return;
 p->result=PAPACC_COMMIT_FAILED;
 if(n->first_owner==p&&n->first_phase==3U){
  if(n->first_worker_thread)goto denied;n->first_worker_thread=GetCurrentThreadId();
  if(!personal_first_ready(p,1))goto denied;
 }else if(!lab&&!personal_native_ceremony_ready(p))goto denied;
 if(InterlockedCompareExchange(&p->stop,0,0)||
    memcmp(n->handoff_attempt,p->personal_data.attempt,32U)||
    !n->plan_leaf[0]||!memchr(n->plan_leaf,0,sizeof(n->plan_leaf))||
    memcmp(n->plan_leaf,p->previous,sizeof(n->plan_leaf)))goto denied;
 if(!lab&&(personal_packet_current(p)!=PAPACC_RESULT_OK||personal_packet_source(p)!=PAPACC_RESULT_OK))goto denied;
 if(n->first_owner==p&&personal_first_resources(p,1,1))goto denied;
 if(!bootstrap_create_child(p))goto denied;
 if(n->first_owner==p&&!personal_first_child_current(p,1)){p->result=PAPACC_CAS_UNKNOWN;goto denied;}
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
 if(lab&&p->test_entered){
  if(!SetEvent(p->test_entered)||WaitForSingleObject(p->test_release,INFINITE)!=WAIT_OBJECT_0){p->result=PAPACC_CAS_UNKNOWN;goto denied;}
 }
 if(lab&&p->test_unknown==1){p->result=PAPACC_CAS_UNKNOWN;goto denied;}
#endif
 if(InterlockedCompareExchange(&p->stop,0,0))goto denied;
 if(!bootstrap_identity(p->bootstrap_parent,&p->bootstrap_parent_volume,&p->bootstrap_parent_high,&p->bootstrap_parent_low,0)||
    !bootstrap_identity(p->bootstrap_container,&p->bootstrap_volume,&p->bootstrap_high,&p->bootstrap_low,0)){
  p->result=PAPACC_CAS_UNKNOWN;goto denied;}
 /* Only the namespace fact. No CA reference/acquisition is fabricated. */
 p->result=PAPACC_CAS_SUCCESS;
 InterlockedExchange(&n->handoff_state,PAPACC_PERSONAL_HANDOFF_VERIFIED);return;
denied:
 InterlockedExchange(&n->handoff_state,p->result==PAPACC_CAS_UNKNOWN?PAPACC_PERSONAL_HANDOFF_RETAINED:PAPACC_PERSONAL_HANDOFF_DENIED);
}

