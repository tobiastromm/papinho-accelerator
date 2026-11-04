PAPACC_RESULT papacc_configuration_personal_scq_session_offer_data(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;PAPACC_SCQ_RUNTIME_OBSERVATION r;WCHAR key[112];unsigned i;
 if(!personal_scq_session_native_current(o))return PAPACC_RESULT_NOT_SUPPORTED;n=&o->pending_commit.personal_native;
 if(scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p)||papacc_configuration_personal_scq_observe_image_data(o,&r)!=PAPACC_RESULT_OK||r.known!=PAPACC_SCQ_OBS_COMPLETE)return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<112;i++)key[i]=(WCHAR)(BYTE)p.key[i];
 if(!n->scq_dispatch.phase){
  if(p.purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY){
   if(!n->scq_profile_data.pre_readiness||personal_scq_paths_current(&n->install_paths,p.purpose,&o->pending_commit.stop)||
    papacc_personal_scq_prepare_transport(&n->scq_dispatch,&n->runtime_identity,p.attempt,key))return PAPACC_RESULT_INVALID_STATE;
  }else if(n->scq_profile_data.read_diagnostic){
   /* K3/K4 diagnostic selected-object path; generic READ remains unchanged.
    * Only prepare dispatch DATA here, never consume P or touch KSP. */
   if(p.purpose!=PAPACC_SCQ_PURPOSE_READ||n->scq_profile_data.read_diagnostic!=1U||
    !n->scq_profile_data.pre_readiness||n->install_paths.read_diagnostic!=1U||
    n->install_paths.selected_role!=1U||!PAPACC_SCQ_P1_PATHS(&n->install_paths)||
    n->scq_session.read_act!=&n->scq_read_diagnostic||n->scq_read_diagnostic.phase!=2U||
    personal_scq_paths_current(&n->install_paths,p.purpose,&o->pending_commit.stop)||
    papacc_personal_scq_prepare(&n->scq_dispatch,&n->runtime_identity,p.attempt,key))return PAPACC_RESULT_INVALID_STATE;
  }else if(papacc_configuration_personal_scq_prepare_data(o,p.attempt,key)!=PAPACC_RESULT_OK)return PAPACC_RESULT_INVALID_STATE;
 }
 return scqs_offer(&n->scq_session,o,&p.administrator,&r.peer,p.refs,&o->pending_commit.stop)?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
}
