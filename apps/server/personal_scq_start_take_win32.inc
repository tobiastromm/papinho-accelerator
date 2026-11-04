/* Same native owner and effect ordering. Only the failed original call is
 * projected; no additional validation, query, authority or retry. */
#include "personal_scq_resource_failure_win32.inc"
static PAPACC_RESULT scq_take_reject(PAPACC_CONFIGURATION_COMMIT *o,DWORD step,PAPACC_RESULT result,DWORD cause)
{
 const PAPACC_PERSONAL_NATIVE_CONTEXT *n=o?&o->pending_commit.personal_native:NULL;
 (void)fprintf(stderr,"SCQ_START_TAKE_FIRST_CAUSE step=%lu result=%u cause=%lu context_known=%u input_first=%lu input_error=%lu chain_first=%lu source_first=%lu start_first=%lu resource_first=%lu resource_phase=%lu; ORIGINAL_FAILED_PATH_ONLY; NO_NEW_QUERY; METADATA_NOT_AUTHORITY\n",
  (unsigned long)step,(unsigned)result,(unsigned long)cause,n?1U:0U,
  n?(unsigned long)n->scq_inputs.first:0UL,n?(unsigned long)n->scq_inputs.error:0UL,
  n?(unsigned long)n->scq_inputs.image_chain.first:0UL,n?(unsigned long)n->scq_admin.first:0UL,
  n?(unsigned long)n->scq_admin.start_first:0UL,n?(unsigned long)n->scq_start_assurance.first:0UL,
  n?(unsigned long)n->scq_start_assurance.phase:0UL);
 if(n&&(step==4U||n->scq_start_assurance.first))scqg_failure_projection(stderr,&n->scq_start_assurance,cause);
 return result;
}
PAPACC_RESULT papacc_configuration_personal_scq_start_take(PAPACC_CONFIGURATION_COMMIT *o)
{
 DWORD e;
 if(!personal_scq_admin_native_current(o))return scq_take_reject(o,12U,PAPACC_RESULT_NOT_SUPPORTED,0);
 e=scqa_start_take(&o->pending_commit.personal_native.scq_admin,o,&o->pending_commit.stop);
 if(e)return scq_take_reject(o,13U,PAPACC_RESULT_INVALID_STATE,e);
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_configuration_personal_scq_start_take_image(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_SERVICE_IMAGE_VIEW *view)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_SCQ_PLAN_DATA p;BY_HANDLE_FILE_INFORMATION observed;WCHAR path[MAX_PATH];DWORD e;
 if(!view||!view->handle||!view->path||!view->sha256||view->owner_thread!=GetCurrentThreadId())return scq_take_reject(o,2U,PAPACC_RESULT_INVALID_STATE,0);
 if(!personal_scq_admin_native_current(o))return scq_take_reject(o,3U,PAPACC_RESULT_INVALID_STATE,0);
 n=&o->pending_commit.personal_native;
 e=papacc_configuration_personal_scq_start_resource_current(o,1);if(e)return scq_take_reject(o,4U,PAPACC_RESULT_INVALID_STATE,e);
 e=scqp_decode_fields(n->scq_profile_data.record,sizeof(n->scq_profile_data.record),&p);if(e)return scq_take_reject(o,5U,PAPACC_RESULT_INVALID_STATE,e);
 if(!GetFileInformationByHandle(view->handle,&observed))return scq_take_reject(o,6U,PAPACC_RESULT_INVALID_STATE,GetLastError());
 if(!scqa_original_identity(&observed,&p.refs[2]))return scq_take_reject(o,7U,PAPACC_RESULT_INVALID_STATE,0);
 if(!scqa_original_identity(&view->identity,&p.refs[2]))return scq_take_reject(o,8U,PAPACC_RESULT_INVALID_STATE,0);
 if(memcmp(view->sha256,p.refs[2].digest,32))return scq_take_reject(o,9U,PAPACC_RESULT_INVALID_STATE,0);
 if(_snwprintf_s(path,MAX_PATH,_TRUNCATE,L"%ls\\papacc_server.exe",n->install_paths.binary)<0)return scq_take_reject(o,10U,PAPACC_RESULT_INVALID_STATE,0);
 if(wcscmp(path,view->path))return scq_take_reject(o,11U,PAPACC_RESULT_INVALID_STATE,0);
 return papacc_configuration_personal_scq_start_take(o);
}
