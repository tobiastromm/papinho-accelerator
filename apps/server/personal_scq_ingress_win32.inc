int papacc_configuration_personal_scq_initial_wait_current(PAPACC_CONFIGURATION_COMMIT *o)
{
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||!papacc_server_service_scq_preparation_current())return 0;
 /* Failed admission remains unresolved; only the already-submitted negative
  * write may wait on held originals, never resume the positive actor. */
 if(o->unresolved)return scqf_wait_current(&o->pending_commit.personal_native.scq_session,o,&o->pending_commit.stop);
 return scqb_initial_wait(&o->pending_commit.personal_native.scq_session,o,&o->pending_commit.stop)||
  scqf_wait_current(&o->pending_commit.personal_native.scq_session,o,&o->pending_commit.stop);
#else
 (void)o;return 0;
#endif
}
DWORD papacc_configuration_personal_scq_ingress_continue(PAPACC_CONFIGURATION_COMMIT *o)
{
#ifdef PAPACC_PERSONAL_SCM_HOST_GATE
 PAPACC_SCQ_OWNER_DECISION *registered;
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_RUNTIME_OBSERVATION observed;DWORD e,stage=1U;
 if(!o||o->writer!=GetCurrentThreadId()||!papacc_server_service_scq_preparation_current())return ERROR_NOT_SUPPORTED;
 n=&o->pending_commit.personal_native;
 /* Only observe the already submitted negative write, never re-enter admission. */
 if(n->scq_session.transport.failure_stage)return scqf_send(&n->scq_session,o,&o->pending_commit.stop);
 registered=&n->scq_selection; /* QUERY backing only, not an approval getter */
 if(o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.personal_ca||o->pending_commit.personal_ca_candidate||o->initialized||o->published||
  !n->scq_carrier_captured)return ERROR_ACCESS_DENIED;
 if(n->scq_session.phase){stage=4U;e=personal_scq_query_resources(o,0);if(e)goto failed;
  stage=n->scq_session.phase==3U?19U:18U;e=papacc_configuration_personal_scq_transport_continue(o);
  if(e&&e!=ERROR_MORE_DATA&&e!=ERROR_IO_PENDING)goto failed;return e; }
 if(!n->runtime_identity.phase){
  PAPACC_SCQ_START_LIMITS selected;BYTE preimage[PAPACC_SCQ_G3_PREIMAGE_BYTES],digest[32];unsigned read_diagnostic=0U;
  stage=3U;scqg_selected_p1(&selected);
  if(scqk_policy_data(&selected,preimage)||!store_data_hash(&n->scq_inputs.content_hash,preimage,sizeof(preimage),digest)){e=ERROR_INVALID_DATA;goto failed;}
  if(!memcmp(digest,n->scq_carrier.policy,32))read_diagnostic=1U;
  else if(scqg_plan_data_mode(&selected,240000U,preimage,1)||!store_data_hash(&n->scq_inputs.content_hash,preimage,sizeof(preimage),digest)||memcmp(digest,n->scq_carrier.policy,32)){e=ERROR_ACCESS_DENIED;goto failed;}
  stage=2U;e=(read_diagnostic?papacc_personal_runtime_capture_machine_read_closed(&n->runtime_identity,GetCurrentProcess(),(PSID)n->scq_carrier.runtime_sid,SERVICE_SID_TYPE_UNRESTRICTED):papacc_personal_runtime_capture_transport_closed(&n->runtime_identity,GetCurrentProcess(),(PSID)n->scq_carrier.runtime_sid,SERVICE_SID_TYPE_UNRESTRICTED));if(e)goto failed;
  if(n->runtime_identity.pid!=GetCurrentProcessId()){e=ERROR_ACCESS_DENIED;goto failed;}
 }
 if(!n->scq_session.transport.phase){
  PAPACC_SCQ_START_LIMITS selected;BYTE preimage[PAPACC_SCQ_G3_PREIMAGE_BYTES],digest[32];
  scqg_selected_p1(&selected);
  stage=3U;if((n->runtime_identity.read_diagnostic?scqk_policy_data(&selected,preimage):scqg_plan_data_mode(&selected,240000U,preimage,1))||!papacc_store_hash(&n->scq_inputs.content_hash,preimage,sizeof(preimage),digest)||memcmp(digest,n->scq_carrier.policy,32)){e=ERROR_ACCESS_DENIED;goto failed;}
  stage=4U;e=personal_scq_query_resources(o,1);if(e)goto failed;
  n->scq_session.transport.resource_scope=&n->scq_start_assurance;
  stage=5U;e=scqb_open_query_variant(&n->scq_session,o,&n->scq_carrier,registered,&n->runtime_identity,&n->scq_profile_data,&n->scq_receiver,&n->scq_dispatch,&o->pending_commit.stop,n->runtime_identity.read_diagnostic);
  if(e)goto failed;return ERROR_MORE_DATA;
 }
 stage=4U;e=personal_scq_query_resources(o,0);if(e)goto failed;
 stage=6U;e=scqb_read(&n->scq_session,o,&o->pending_commit.stop);if(e==ERROR_MORE_DATA||e==ERROR_IO_PENDING)return e;if(e)goto failed;
 stage=7U;e=scqb_candidates(&n->scq_session,o,&p,&o->pending_commit.stop);if(e)goto failed;
 stage=8U;if(!n->install_paths.phase){
  /* P0-P7 is confined to closed, pre-readiness TRANSPORT_ONLY QUERY. */
  if((p.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY||n->scq_session.transport.read_diagnostic)&&n->scq_session.transport.capture_query){
   /* Fail closed rather than fall back to NEW_INSTALL discovery. Profile
    * pre_readiness does not exist until the later first_prepare transition. */
   if(!scqb_existing_target_query(&n->scq_session,&p)){e=ERROR_ACCESS_DENIED;goto failed;}
   e=n->scq_session.transport.read_diagnostic?papacc_personal_paths_selected_machine_read(&n->install_paths,NULL,2U,&n->scq_inputs.image_chain,&o->pending_commit.stop,n->scq_session.transport.read_public):papacc_personal_paths_selected(&n->install_paths,NULL,2U,&n->scq_inputs.image_chain,&o->pending_commit.stop);
  }else e=papacc_personal_paths_new(&n->install_paths,&o->pending_commit.stop);
  if(e)goto failed;
 }
 stage=9U;e=(n->scq_session.transport.read_diagnostic?personal_scq_inputs_capture_machine_read(&n->scq_inputs,n->scq_session.transport.capture_refs,(PSID)p.administrator.sid,&p,&n->install_paths,o,&o->pending_commit.stop):personal_scq_inputs_capture_transport(&n->scq_inputs,n->scq_session.transport.capture_refs,(PSID)p.administrator.sid,&p,&n->install_paths,o,&o->pending_commit.stop));if(e)goto failed;
 stage=10U;e=personal_scq_inputs_contents(&n->scq_inputs,(PSID)p.administrator.sid,p.refs,((ULONGLONG)p.valid_until[1]<<32)|p.valid_until[0],&o->pending_commit.stop);if(e)goto failed;
 stage=11U;e=papacc_personal_runtime_observe_data(&n->runtime_identity,&observed,&o->pending_commit.stop);if(e)goto failed;
 stage=12U;e=personal_scq_inputs_image_data(&n->scq_inputs,(PSID)p.administrator.sid,&n->runtime_identity,&observed,&o->pending_commit.stop);if(e)goto failed;
 if(observed.known!=PAPACC_SCQ_OBS_COMPLETE||memcmp(&observed.peer,&p.runtime,sizeof(p.runtime))){e=ERROR_ACCESS_DENIED;goto failed;}
 stage=13U;e=scqb_query_image(&n->scq_session,&p,&n->scq_inputs);if(e)goto failed;
 stage=14U;e=scqb_bind_query(&n->scq_session,o,registered,&p,&n->scq_inputs,&n->install_paths,&o->pending_commit.stop);if(e)goto failed;
 stage=15U;e=scqb_original_deadline(&n->scq_session,&p);if(e)goto failed;
 stage=4U;e=personal_scq_query_resources(o,0);if(e)goto failed;
 stage=16U;e=scqr_first_prepare(&n->scq_recipient,&n->scq_session,o,registered,&n->scq_profile_data,&n->scq_inputs,&n->runtime_identity,&n->scq_dispatch,&n->scq_receiver,&p,n->scq_session.transport.capture_controller,&o->pending_commit.stop);if(e)goto failed;
 stage=17U;e=scqb_promote(&n->scq_session,o,&n->scq_recipient,&n->scq_inputs,&o->pending_commit.stop);if(e)goto failed;
 return ERROR_MORE_DATA;
failed:
 scqf_freeze_inputs(&n->scq_session.transport,stage,e,&n->scq_inputs);
 if(!n->error)n->error=e;o->unresolved=1;
 if(scqt_owned(&n->scq_session,o))(void)scqt_fail(&n->scq_session,e);
 return scqf_send(&n->scq_session,o,&o->pending_commit.stop);
#else
 (void)o;return ERROR_NOT_SUPPORTED;
#endif
}
