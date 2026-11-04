/* Original return snapshots only: no API, clock, token, resource or handle query. */
static DWORD scqtc_result(PAPACC_CONFIGURATION_COMMIT *o,DWORD branch,DWORD e)
{
 const PAPACC_PERSONAL_NATIVE_CONTEXT *n=o?&o->pending_commit.personal_native:NULL;
 if(e&&e!=ERROR_IO_PENDING&&e!=ERROR_MORE_DATA)
  (void)fprintf(stderr,"SCQ_TRANSPORT_CONTINUE_FIRST_CAUSE branch=%lu error=%lu role=%u session_phase=%u transport_phase=%u legs=%lu read_profile=%u read_transport=%u query_refs3=%u; ORIGINAL_RETURN_ONLY_NO_NEW_QUERY_NO_AUTHORITY\n",(unsigned long)branch,(unsigned long)e,n?n->scq_session.role:0U,n?n->scq_session.phase:0U,n?n->scq_session.transport.phase:0U,n?(unsigned long)n->scq_session.transport.legs:0UL,n?n->scq_profile_data.read_diagnostic:0U,n?n->scq_session.transport.read_diagnostic:0U,n?n->scq_session.transport.query_refs3:0U);
 return e;
}
DWORD papacc_configuration_personal_scq_transport_continue(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_SCQ_ORIGINAL *proof;
 const PAPACC_SCQ_OWNER_DECISION *registered=personal_scq_registered_endpoint_decision(o);
 if(!o||o->writer!=GetCurrentThreadId()||!registered||registered!=personal_scq_registered_owner_decision(o)||(registered->purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&!(registered->read_diagnostic==1U&&registered->purpose==PAPACC_SCQ_PURPOSE_READ)))return scqtc_result(o,1U,ERROR_NOT_SUPPORTED);
 n=&o->pending_commit.personal_native;
 if(n->scq_session.role==2U){DWORD resource=personal_scq_query_resources(o,0);if(resource)return scqtc_result(o,2U,resource);}
 else {DWORD resource=papacc_configuration_personal_scq_start_resource_current(o,0);if(resource)return scqtc_result(o,3U,resource);}
 if(papacc_server_console_win32_stop_requested())InterlockedExchange(&o->pending_commit.stop,1);
 if(o->pending_commit.stop){if(n->scq_session.transport.phase)(void)scqt_stop(&n->scq_session,o);return scqtc_result(o,4U,ERROR_CANCELLED);}
 /* Only an already-submitted final READ write can be completed without
  * reacquiring a live-peer original. New writes keep the normal live gate. */
 if(registered->read_diagnostic==1U&&n->scq_session.role==1U&&n->scq_session.phase==4U&&n->scq_session.transport.operation==16U&&
  papacc_personal_cng_inconclusive(&n->runtime_cng_probe)){
  if(!personal_scq_admin_native_current(o))return scqtc_result(o,11U,ERROR_ACCESS_DENIED);
  return scqtc_result(o,11U,scqkm_return_inconclusive(&n->scq_session,o,&n->scq_read_diagnostic,&n->machine_fixture,&n->runtime_cng_probe,&o->pending_commit.stop));
 }
 if(!personal_scq_session_native_current(o)){
  if(n->scq_session.transport.pending&&scqt_owned(&n->scq_session,o))(void)scqt_stop(&n->scq_session,o);
  return scqtc_result(o,5U,ERROR_NOT_SUPPORTED);
 }
 proof=personal_scq_native_original(o);
 if(!proof&&n->scq_session.role==2U&&n->scq_recipient.phase==1U&&n->scq_session.transport.query_only)proof=&n->scq_recipient.value;
 if(!proof)return scqtc_result(o,6U,ERROR_NOT_SUPPORTED);
 if(n->scq_session.role==1U&&n->scq_admin.start_consumed&&n->scq_session.phase==3U&&!(n->scq_session.transport.legs&1U)){
  DWORD written=scqb_write(&n->scq_session,o,&o->pending_commit.stop);return written?scqtc_result(o,7U,written):ERROR_MORE_DATA;
 }
 if(registered->read_diagnostic){
  if(n->scq_session.role==1U&&!personal_scq_admin_native_current(o))return scqtc_result(o,8U,ERROR_ACCESS_DENIED);
  return scqtc_result(o,9U,scqkm_continue_session(&n->scq_session,o,&n->scq_read_diagnostic,&n->scq_read_recipient,&n->scq_recipient,&n->scq_inputs,&n->machine_fixture,&n->runtime_cng_probe,n->install_paths.read_public,proof,&o->pending_commit.stop));
 }
 return scqtc_result(o,10U,scqr_continue_transport(&n->scq_session,o,n->scq_session.role==2U?&n->scq_recipient:NULL,n->scq_session.role==2U?&n->scq_inputs:NULL,proof,&o->pending_commit.stop));
}
