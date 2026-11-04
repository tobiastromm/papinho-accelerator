/* One native caller for the existing acquisition/presenter/source components.
 * Inputs must ALREADY be native-held originals in this owner, not argv/DATA
 * approvals. This function never takes either act or opens an endpoint/SCM. */
DWORD papacc_configuration_personal_scq_first_controller(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA plan;PAPACC_RESULT r;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->initialized||o->published||
  o->prepared||o->committed_bytes||o->committed_pending||o->recovery_selected||o->recovery_guard||
  o->pending_commit.active||o->pending_commit.thread||o->pending_commit.stop||papacc_server_console_win32_stop_requested())return ERROR_ACCESS_DENIED;
 n=&o->pending_commit.personal_native;
 if(n->scq_inputs.phase==4U||n->scq_inputs.image_chain.phase==4U||n->scq_inputs.content_hash.retained||
  n->scq_admin.value.retained){o->unresolved=1;return ERROR_IO_INCOMPLETE;}
 if(!scqp_owned(&n->scq_profile_data,o)||n->scq_profile_data.role!=1U||n->scq_profile_data.phase!=1U||
  !n->scq_profile_data.pre_readiness||!n->scq_profile_data.waiting_runtime||n->scq_session.phase||
  n->scq_admin.phase>1U||n->scq_admin.start_phase||n->scq_admin.start_consumed||n->scq_admin.endpoint_phase||n->scq_admin.endpoint_consumed||
  scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&plan)||
  !PAPACC_SCQ_EXISTING_PATH_PURPOSE(&n->install_paths,plan.purpose)||n->scq_profile_data.read_diagnostic!=n->install_paths.read_diagnostic||!scqp_unbound(&plan.runtime)||!scqp_static(&plan)||!scqp_window(&plan))return ERROR_NOT_FOUND;
 /* session_acquire_originals already captures this same source. Resume only
  * that unconfirmed original; never recapture it or accept confirmed replay. */
 if(n->scq_admin.phase==1U&&(!n->scq_selection_captured||n->scq_admin.registration!=&n->scq_selection||
  scqa_current(&n->scq_admin,o,&o->pending_commit.stop)))goto failed;
 if(n->scq_inputs.phase!=1U||n->scq_inputs.first||!n->scq_inputs.file[0]||!n->scq_inputs.file[1]||!n->scq_inputs.file[2])return ERROR_NOT_FOUND;
 if(!n->scq_inputs.content_verified){
  r=papacc_configuration_personal_scq_acquire_contents(o);if(r!=PAPACC_RESULT_OK)goto failed;
 }
 if(!n->scq_selection_captured&&personal_scq_selection_capture(o))goto failed;
 if(!n->scq_admin.phase&&papacc_configuration_personal_scq_admin_prepare(o)!=PAPACC_RESULT_OK)goto failed;
 if(!n->console.is_installed&&papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK)goto failed;
 if(papacc_configuration_personal_scq_admin_confirm_console(o)!=PAPACC_RESULT_OK)goto failed;
 if(papacc_configuration_personal_scq_endpoint_confirm_console(o)!=PAPACC_RESULT_OK)goto failed;
 if(papacc_configuration_personal_scq_start_confirm_console(o)!=PAPACC_RESULT_OK)goto failed;
 if(!scqa_endpoint_selection(&n->scq_admin,o,&o->pending_commit.stop)||!scqa_start_selection(&n->scq_admin,o,&o->pending_commit.stop)||
  papacc_configuration_personal_scq_session_prepare(o,1U)!=PAPACC_RESULT_OK)goto failed;
 return 0; /* only prepared/confirmed: consumption and every effect gate remain */
failed:
 if(n->scq_inputs.phase==4U||n->scq_inputs.image_chain.phase==4U||n->scq_inputs.content_hash.retained||
  n->scq_admin.value.retained||o->unresolved){o->unresolved=1;return ERROR_IO_INCOMPLETE;}
 return ERROR_ACCESS_DENIED;
}
