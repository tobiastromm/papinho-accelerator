DWORD papacc_server_launcher_scq_start_wait(HANDLE process)
{
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_OFFLINE_D3_WORKER)
 PS_SCQ_CARRIER scope;ULONGLONG now;unsigned remaining;DWORD observed,cause;PAPACC_RESULT result;
 HANDLE event=root.configuration_commit.pending_commit.personal_native.scq_session.transport.event;
 if(!process||!event){(void)fprintf(stderr,"SCQ_START_WAIT_FIRST_CAUSE step=1 mapped=5; ORIGINAL_GUARD_ONLY; METADATA_NOT_AUTHORITY\n");return ERROR_ACCESS_DENIED;}
 cause=papacc_server_launcher_scq_start_carrier(&scope);if(cause){(void)fprintf(stderr,"SCQ_START_WAIT_FIRST_CAUSE step=2 mapped=5 cause=%lu; ORIGINAL_RETURN_ONLY; METADATA_NOT_AUTHORITY\n",(unsigned long)cause);return ERROR_ACCESS_DENIED;}
 now=GetTickCount64();if(!ps_scq_remaining_ms(&scope,(unsigned)now,(unsigned)(now>>32),&remaining)){(void)fprintf(stderr,"SCQ_START_WAIT_FIRST_CAUSE step=3 mapped=1460; ORIGINAL_DEADLINE_ONLY; METADATA_NOT_AUTHORITY\n");return ERROR_TIMEOUT;}
 result=papacc_server_console_win32_wait_original(event,process,remaining,&observed);if(result!=PAPACC_RESULT_OK){(void)fprintf(stderr,"SCQ_START_WAIT_FIRST_CAUSE step=4 mapped=5023 result=%u; ORIGINAL_RETURN_ONLY; METADATA_NOT_AUTHORITY\n",(unsigned)result);return ERROR_INVALID_STATE;}
 if(observed==WAIT_TIMEOUT){
  const PAPACC_PERSONAL_NATIVE_CONTEXT *n=&root.configuration_commit.pending_commit.personal_native;
  const PAPACC_SCQ_TRANSPORT *t=&n->scq_session.transport;
  /* Existing packet scalars before Stop/retention can overwrite phase. No
   * native query, handle value, token, nonce, path, buffer or authority output. */
  (void)fprintf(stderr,"SCQ_START_WAIT_FIRST_CAUSE step=5 mapped=1460 runtime_phase=%lu session_phase=%lu transport_phase=%lu transport_first=%lu operation=%lu pending=%lu offset=%lu total=%lu legs=%lu connect_started=%lu delivery_bound=%u remote_files_mask=%u remote_query_mask=%u query_refs3=%lu close_produced=%lu close_accepted=%lu close_confirmed=%lu; ORIGINAL_WAIT_AND_PACKET_SCALARS_ONLY; ZERO_NOT_PROOF; METADATA_NOT_AUTHORITY\n",
   (unsigned long)n->runtime_identity.phase,(unsigned long)n->scq_session.phase,
   (unsigned long)t->phase,(unsigned long)t->first,(unsigned long)t->operation,(unsigned long)t->pending,
   (unsigned long)t->offset,(unsigned long)t->total,(unsigned long)t->legs,(unsigned long)t->connect_started,
   t->delivery_process?1U:0U,(t->remote_files[0]?1U:0U)|(t->remote_files[1]?2U:0U)|(t->remote_files[2]?4U:0U),
   (t->remote_query[0]?1U:0U)|(t->remote_query[1]?2U:0U),(unsigned long)t->query_refs3,
   (unsigned long)t->close_produced,(unsigned long)t->close_accepted,(unsigned long)t->close_confirmed);
  return ERROR_TIMEOUT;
 }
 if(observed!=WAIT_OBJECT_0)(void)fprintf(stderr,"SCQ_START_WAIT_FIRST_CAUSE step=6 mapped=1067 observed=%lu; ORIGINAL_WAIT_ONLY; METADATA_NOT_AUTHORITY\n",(unsigned long)observed);
 return observed==WAIT_OBJECT_0?0:ERROR_PROCESS_ABORTED;
#else
 (void)process;return ERROR_NOT_SUPPORTED;
#endif
}
