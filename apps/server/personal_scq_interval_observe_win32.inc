/* Fixed, one-shot read-only diagnostic. Does not enter scqc_plan, install a
 * selected target, generate a nonce or confirm/consume any E/Q/S authority. */
DWORD papacc_configuration_personal_request_interval_observe(PAPACC_CONFIGURATION_COMMIT *o,PAPACC_SCQ_INTERVAL_OBSERVATION *v)
{
 static const WCHAR request[]=L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\phase6-server-update-after-utility-20261101\\FIRST-P1-CHAIN-SESSION-REQUEST.ORIGINAL.md";
 static const WCHAR decision[]=L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\phase6-server-update-after-utility-20261101\\FIRST-P1-CHAIN-OPERATIONAL-DECISION.ORIGINAL.txt";
 PAPACC_PERSONAL_NATIVE_CONTEXT *n;DWORD e=0,z;unsigned cut=0;
 if(!o||!v||o->writer!=GetCurrentThreadId()||o->stopped||o->initialized||o->published||o->unresolved||o->pending_commit.active||o->pending_commit.thread||o->pending_commit.stop)return ERROR_INVALID_STATE;
 n=&o->pending_commit.personal_native;
 if(n->phase||n->install_paths.phase||n->scq_inputs.phase||n->scq_admin.phase||n->scq_session.phase||n->scq_cold_raw[0]||n->scq_cold_raw[1]||n->scq_cold_raw[2])return ERROR_INVALID_STATE;
 memset(v,0,sizeof(*v));v->first_cut=MAXDWORD;n->phase=1U;
 if(papacc_server_console_win32_stop_requested()||papacc_server_console_win32_install(&n->console)!=PAPACC_RESULT_OK){e=ERROR_CANCELLED;goto done;}
 if(!personal_token(&o->pending_commit,1)){e=ERROR_ACCESS_DENIED;goto done;}
 /* Same module-name API as the P1 locator, but no false installed-image proof:
  * the workspace probe deliberately leaves install_paths uninitialized. */
 z=GetModuleFileNameW(NULL,v->module,MAX_PATH);if(!z||z>=MAX_PATH){e=ERROR_INVALID_NAME;goto done;}
 e=scqc_open_raw_mode(o,0U,request,v);
 if(e){cut=n->scq_cold_diagnostic.open_attempted?2U:n->scq_cold_diagnostic.query_succeeded?1U:0U;if(n->scq_cold_diagnostic.stage==2U)cut=0;goto done;}
 cut=3U;
 if(GetFileType(n->scq_cold_raw[0])!=FILE_TYPE_DISK||!GetFileInformationByHandle(n->scq_cold_raw[0],&v->identity)){e=ERROR_INVALID_HANDLE;goto done;}
 if(v->identity.dwVolumeSerialNumber!=417614100U||v->identity.nFileIndexHigh!=1441792U||v->identity.nFileIndexLow!=542844U||
  v->identity.nFileSizeHigh||v->identity.nFileSizeLow!=16483U||v->identity.nNumberOfLinks!=1U||
  (v->identity.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))){e=ERROR_REVISION_MISMATCH;goto done;}
 v->identity_checked=1U;e=scqc_interval_sample(o,v,3U);if(e)goto done;
 e=scqc_descriptor_digest(o,0U,v->descriptor_digest,v);if(e)goto done;
 cut=7U;v->drive[0]=decision[0];v->drive[1]=L':';v->drive[2]=L'\\';
 if(GetDriveTypeW(v->drive)!=DRIVE_FIXED){e=ERROR_INVALID_NAME;goto done;}
 e=scqc_interval_sample(o,v,7U);if(e)goto done;
 cut=8U;e=scq_p1_native_name(decision,0,n->scq_cold_name);if(e)goto done;
 e=scqc_interval_sample(o,v,8U);
 if(v->sample[8][0]&&v->sample[8][1]>=124U){v->final_guard=1U;if(!e)e=ERROR_NOT_ENOUGH_QUOTA;}
done:
 if(e&&!v->first_error)scqc_interval_error(v,cut,e);
 /* A successful synchronous SD acquisition followed only by our quota sample
  * is not an uncertain allocation. On this exact pre-hash diagnostic abort,
  * dispose the known-owned temporary once, then close raw0 below. Do not enter
  * CNG, resume the observation, retry a failed free or clear prior uncertainty.
  * The original quota denial/first cut and all captured samples stay intact. */
 if(!o->unresolved&&v->first_cut==4U&&v->first_error==ERROR_NOT_ENOUGH_QUOTA&&
  v->sample[4][0]&&v->sample[4][1]>=124U&&v->sample[4][1]<=128U&&
  !v->hash_called&&n->scq_inputs.checking&&!n->scq_inputs.content_hash.active&&
  !n->scq_inputs.content_hash.retained&&!n->scq_inputs.content_hash.hash&&!n->scq_inputs.content_hash.algorithm){
  if(LocalFree(n->scq_inputs.checking))o->unresolved=1;
  else n->scq_inputs.checking=NULL;
 }
 if(n->scq_inputs.checking||n->scq_inputs.content_hash.retained||n->scq_inputs.content_hash.active)o->unresolved=1;
 if(!o->unresolved){z=scqc_close_raw(o);if(z&&!v->first_error)scqc_interval_error(v,9U,z);}
 return v->first_error;
}
