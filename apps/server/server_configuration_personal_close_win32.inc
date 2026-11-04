PAPACC_RESULT papacc_configuration_personal_native_close(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;HANDLE *h[5];HLOCAL m[3];unsigned i;
 if(!o||o->writer!=GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;p=&o->pending_commit;n=&p->personal_native;
 if(n->first_owner==p&&p->personal_ca_candidate==n->first_key&&n->first_key&&
  n->first_key->state==PAPACC_CA_KEY_CLOSED&&n->first_key->close_confirmed&&!n->first_key->key&&!n->first_key->provider&&!p->active&&!p->thread)
  p->personal_ca_candidate=NULL; /* exact preparation owner closed; no borrow was permitted */
 if(p->personal_ca||p->personal_ca_candidate||p->active||p->thread||n->phase==2U||n->scm_registration.phase==6U)return PAPACC_RESULT_INVALID_STATE;
 if(n->first_mode==3U&&(n->scq_inputs.phase==4U||n->scq_inputs.image_chain.phase==4U||
  n->scq_inputs.content_hash.active||n->scq_inputs.content_hash.retained||
  (n->first_key&&(n->first_key->key||n->first_key->provider||n->first_key->state==PAPACC_CA_KEY_RETAINED))))return PAPACC_RESULT_INVALID_STATE;
 /* FCR query-only SCM references: once each; failure retains the cohort. */
 if(n->prepared_service){if(n->prepared_close_attempts&1U)goto retained;n->prepared_close_attempts|=1U;
  if(!CloseServiceHandle(n->prepared_service))goto retained;n->prepared_service=NULL;}
 if(n->prepared_manager){if(n->prepared_close_attempts&2U)goto retained;n->prepared_close_attempts|=2U;
  if(!CloseServiceHandle(n->prepared_manager))goto retained;n->prepared_manager=NULL;}
 if(scqkf_close(&n->machine_fixture,o)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(scqkf_source_close(&n->machine_fixture)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(n->staging.phase&&n->staging.phase!=5U&&n->staging.phase!=6U)return PAPACC_RESULT_INVALID_STATE;
 if(n->staged_update.phase&&n->staged_update.phase!=4U)return PAPACC_RESULT_INVALID_STATE;
 if(n->scq_dispatch.phase==2U||n->scq_dispatch.phase==4U)return PAPACC_RESULT_INVALID_STATE;
 if(n->scq_receiver.phase==4U){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(scqs_close(&n->scq_session,o,1)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(scqr_close(&n->scq_recipient,o)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 /* This substate has NO endpoint/API; true here is DATA quiescence only.
  * It does not assert that any external transport or reference has closed. */
 if(papacc_scq_profile_close_data(&n->scq_profile_data,o,1)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(n->scq_receiver.original&&personal_scq_receive_finish(&n->scq_receiver,o,1U)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(n->scq_dispatch.phase==1U){n->scq_dispatch.result=PAPACC_CNG_PROBE_STOPPED;n->scq_dispatch.phase=3U;}
 if(n->runtime_cng_probe.phase&&n->runtime_cng_probe.phase!=3U){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(scqa_close(&n->scq_admin,o,1)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(scqc_close_raw(o)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(n->first_owner==p&&p->file==n->scq_inputs.image_chain.image)p->file=NULL; /* borrowed from chain, not a second close owner */
 if(personal_scq_inputs_close(&n->scq_inputs)){o->unresolved=1;return PAPACC_RESULT_INVALID_STATE;}
 if(n->runtime_identity.phase&&n->runtime_identity.phase!=3U&&papacc_configuration_personal_runtime_close(o)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 if(n->phase==3U)return PAPACC_RESULT_OK;
 /* The child must close before its protecting parent, after worker signal.
  * This bit is disjoint from old handles and LocalFree attempt bits. */
 if(!scm_registration_files_close(n))goto retained;
 if(n->scm_image){
  if(n->close_attempts&512U)goto retained;n->close_attempts|=512U;
  if(!CloseHandle(n->scm_image))goto retained;n->scm_image=NULL;
 }
 if(p->bootstrap_container){
  if(n->close_attempts&256U)goto retained;n->close_attempts|=256U;
#ifdef PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST
  if(p->test_unknown==2){SetLastError(ERROR_INVALID_HANDLE);goto retained;}
#endif
  if(!CloseHandle(p->bootstrap_container))goto retained;p->bootstrap_container=NULL;
 }
 h[0]=&p->file;h[1]=&n->unexpected_thread;h[2]=&p->bootstrap_token;h[3]=&p->bootstrap_parent;h[4]=&n->check_token;
 for(i=0;i<5;i++)if(*h[i]&&*h[i]!=INVALID_HANDLE_VALUE){
 if(n->close_attempts&(1U<<i))goto retained;n->close_attempts|=1U<<i;
 if(!CloseHandle(*h[i]))goto retained;*h[i]=NULL;}
 m[0]=n->check_descriptor;m[1]=n->descriptor;m[2]=n->sid_text;
 for(i=0;i<3;i++)if(m[i]){if(n->close_attempts&(32U<<i))goto retained;n->close_attempts|=32U<<i;if(LocalFree(m[i]))goto retained;if(i==0U)n->check_descriptor=NULL;else if(i==1U)n->descriptor=NULL;else n->sid_text=NULL;}
 if(n->first_child_sd){if(n->close_attempts&1024U)goto retained;n->close_attempts|=1024U;if(LocalFree(n->first_child_sd))goto retained;n->first_child_sd=NULL;}
 n->phase=3U;return PAPACC_RESULT_OK;
retained:n->error=GetLastError();n->phase=2U;o->unresolved=1;return PAPACC_RESULT_INTERNAL_ERROR;
}

