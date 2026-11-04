PAPACC_RESULT papacc_configuration_personal_scq_session_acquire_originals(PAPACC_CONFIGURATION_COMMIT *o,const HANDLE originals[3])
{
 PAPACC_RESULT e;PAPACC_SCQ_PLAN_DATA plan;int capture_candidate;
 if(!o||o->writer!=GetCurrentThreadId()||o->stopped||o->unresolved||o->pending_commit.stop||o->pending_commit.active||o->pending_commit.thread)return PAPACC_RESULT_INVALID_STATE;
 capture_candidate=personal_scq_registered_owner_decision(o)==NULL;
 /* K3 cold READ is an explicit selected-object diagnostic only. This is
  * candidate acquisition, never authority: original capture/selection remain. */
 if(capture_candidate&&(scqp_decode_fields(o->pending_commit.personal_native.scq_profile_data.record,
  sizeof(o->pending_commit.personal_native.scq_profile_data.record),&plan)||
  (plan.purpose!=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY&&
   !(plan.purpose==PAPACC_SCQ_PURPOSE_READ&&o->pending_commit.personal_native.scq_profile_data.read_diagnostic==1U&&
    o->pending_commit.personal_native.install_paths.selected_role==1U&&
    PAPACC_SCQ_EXISTING_PATH_PURPOSE(&o->pending_commit.personal_native.install_paths,plan.purpose)))||
  !o->pending_commit.personal_native.scq_profile_data.pre_readiness||
  !o->pending_commit.personal_native.scq_profile_data.waiting_runtime))return PAPACC_RESULT_NOT_SUPPORTED;
 /* Original references only; no path, frame or receipt creates this source. */
 e=papacc_configuration_personal_scq_capture_inputs(o,originals);if(e!=PAPACC_RESULT_OK)return e;
 e=papacc_configuration_personal_scq_acquire_contents(o);if(e!=PAPACC_RESULT_OK)return e;
 if(capture_candidate&&personal_scq_selection_capture(o))return PAPACC_RESULT_INVALID_STATE;
 return papacc_configuration_personal_scq_admin_prepare(o);
}
