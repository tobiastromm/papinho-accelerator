PAPACC_RESULT papacc_configuration_personal_scq_controller_inconclusive(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;
 if(!o||o->writer!=GetCurrentThreadId()||o->unresolved||o->stopped||o->pending_commit.stop)return PAPACC_RESULT_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 if(!scqt_owned(&n->scq_session,o)||n->scq_session.phase!=4U||n->scq_session.first||n->scq_session.peer_lost||n->scq_session.transport.phase!=1U||n->scq_session.transport.first||n->scq_session.transport.cancelled||n->scq_profile_data.read_diagnostic!=1U||n->scq_session.role!=1U||n->scq_read_diagnostic.phase!=3U||!n->scq_read_diagnostic.consumed||
  n->scq_read_diagnostic.source!=&n->scq_admin||n->scq_session.read_act!=&n->scq_read_diagnostic||
  n->machine_fixture.phase!=2U||!n->machine_fixture.key.diagnostic_control_confirmed||
  !papacc_personal_cng_inconclusive(&n->runtime_cng_probe)||!scqt_read_result_closed(&n->scq_session)||
  !n->scq_session.transport.close_accepted||!(n->scq_session.transport.legs&16U)||n->scq_session.transport.pending||n->scq_session.transport.operation)return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK; /* original correlated snapshot, never runtime liveness */
}
