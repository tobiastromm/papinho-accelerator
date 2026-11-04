/* Original return values only: no sampling, reopening, clock or authority. */
#include "personal_scq_resource_failure_win32.inc"
#include "personal_scq_inconclusive_win32.h"
static DWORD scq_progress_reject(PAPACC_CONFIGURATION_COMMIT *o,DWORD step,DWORD mapped,unsigned result_known,PAPACC_RESULT result)
{
 const PAPACC_PERSONAL_NATIVE_CONTEXT *n=o?&o->pending_commit.personal_native:NULL;
 (void)fprintf(stderr,"SCQ_START_PROGRESS_FIRST_CAUSE step=%lu mapped=%lu result_known=%u result=%u runtime_phase=%u runtime_first=%lu transport_phase=%lu transport_first=%lu operation=%lu pending=%lu; ORIGINAL_RETURN_ONLY; NO_NEW_QUERY; ZERO_NOT_PROOF; METADATA_NOT_AUTHORITY\n",
  (unsigned long)step,(unsigned long)mapped,result_known,(unsigned)result,n?n->runtime_identity.phase:0U,
  n?(unsigned long)n->runtime_identity.first_error:0UL,n?(unsigned long)n->scq_session.transport.phase:0UL,
  n?(unsigned long)n->scq_session.transport.first:0UL,n?(unsigned long)n->scq_session.transport.operation:0UL,n?(unsigned long)n->scq_session.transport.pending:0UL);
 if(n&&step==2U)scqg_failure_projection(stderr,&n->scq_start_assurance,mapped);
 return mapped;
}
DWORD papacc_configuration_personal_scq_start_progress(PAPACC_CONFIGURATION_COMMIT *o,HANDLE process)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_TRANSPORT *t;const PAPACC_SCQ_OWNER_DECISION *registered=personal_scq_registered_owner_decision(o);DWORD e;PAPACC_RESULT result;
 if(!registered||registered!=personal_scq_registered_endpoint_decision(o)||!personal_scq_admin_native_current(o)||!process)return scq_progress_reject(o,1U,ERROR_NOT_SUPPORTED,0U,PAPACC_RESULT_OK);
 n=&o->pending_commit.personal_native;t=&n->scq_session.transport;
 e=papacc_configuration_personal_scq_start_resource_current(o,1);if(e)return scq_progress_reject(o,2U,e,0U,PAPACC_RESULT_OK);
 if(!n->scq_admin.start_consumed||!t->phase||!t->pipe||t->original!=&n->scq_admin.value)return scq_progress_reject(o,3U,ERROR_ACCESS_DENIED,0U,PAPACC_RESULT_OK);
 if(papacc_server_console_win32_stop_requested()){InterlockedExchange(&o->pending_commit.stop,1);return scqt_stop(&n->scq_session,o);}
 if(!n->runtime_identity.phase){
  result=papacc_configuration_personal_runtime_bind(o,process,(PSID)registered->runtime,PAPACC_PERSONAL_RUNTIME_VIRTUAL,SERVICE_SID_TYPE_UNRESTRICTED);
  if(result!=PAPACC_RESULT_OK)return scq_progress_reject(o,4U,ERROR_ACCESS_DENIED,1U,result);
 }
 if(n->scq_profile_data.waiting_runtime){result=papacc_configuration_personal_scq_session_offer_data(o);if(result!=PAPACC_RESULT_OK)return scq_progress_reject(o,5U,ERROR_ACCESS_DENIED,1U,result);}
 if(t->phase==2U){
  if(!t->connect_started){e=papacc_configuration_personal_scq_transport_enter(o,1U,NULL);if(e){if(e==ERROR_IO_PENDING||e==ERROR_MORE_DATA)return e;return scq_progress_reject(o,6U,e,0U,PAPACC_RESULT_OK);}}
  if(t->pending){e=scqt_poll(&n->scq_session,o);if(e){if(e==ERROR_IO_PENDING||e==ERROR_MORE_DATA)return e;return scq_progress_reject(o,7U,e,0U,PAPACC_RESULT_OK);}}
  e=papacc_configuration_personal_scq_transport_enter(o,2U,NULL);if(e){if(e==ERROR_IO_PENDING||e==ERROR_MORE_DATA)return e;return scq_progress_reject(o,8U,e,0U,PAPACC_RESULT_OK);}
 }
 if(!t->delivery_process){e=papacc_configuration_personal_scq_transport_references(o,NULL);if(e){if(e==ERROR_IO_PENDING||e==ERROR_MORE_DATA)return e;return scq_progress_reject(o,9U,e,0U,PAPACC_RESULT_OK);}}
 e=papacc_configuration_personal_scq_transport_continue(o);
 /* A raw native ERROR_NOT_FOUND at any earlier gate remains an error. Only
  * the original READ terminal binding can produce the typed completion. */
 if(e==ERROR_NOT_FOUND&&papacc_configuration_personal_scq_controller_inconclusive(o)==PAPACC_RESULT_OK)return PAPACC_SCQ_PROGRESS_CLOSED_INCONCLUSIVE;
 if(e&&e!=ERROR_IO_PENDING&&e!=ERROR_MORE_DATA)return scq_progress_reject(o,10U,e,0U,PAPACC_RESULT_OK);
 return e;
}
