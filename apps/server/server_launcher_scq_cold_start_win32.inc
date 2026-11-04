/* Private owner-manual candidate entry. argv contains no approval, nonce,
 * HANDLE, PID/token or secret; immutable originals and three console acts
 * are acquired in this process before the existing SCM CLI can be entered. */
static int scq_cold_u64(const char *s,ULONGLONG *out)
{
 ULONGLONG v=0;unsigned d;if(!s||!*s||!out)return 0;
 while(*s){if(*s<'0'||*s>'9')return 0;d=(unsigned)(*s++-'0');if(v>(((ULONGLONG)-1)-d)/10U)return 0;v=v*10U+d;}
 if(!v)return 0;*out=v;return 1;
}
static int scq_cold_hex(const char *s,BYTE out[32])
{
 unsigned i,j;BYTE value;if(!s||strlen(s)!=64U)return 0;
 for(i=0;i<32U;i++){value=0;for(j=0;j<2U;j++){char ch=s[2U*i+j];if(!((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')))return 0;value=(BYTE)((value<<4)|(ch<='9'?ch-'0':ch-'a'+10));}out[i]=value;}return 1;
}
static DWORD scq_boundary_count(DWORD *sample)
{
 DWORD count=0,error;
 if(!GetProcessHandleCount(GetCurrentProcess(),&count)){error=GetLastError();sample[2]=error?error:ERROR_GEN_FAILURE;return sample[2];}
 sample[0]=1;sample[1]=count;return 0;
}
static int scq_boundary_observe(FILE *out)
{
 PAPACC_SERVER_NETWORK empty=PAPACC_SERVER_NETWORK_INITIALIZER;PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;
 PAPACC_LOGGER logger;PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *owned=NULL;DWORD samples[15]={0},error;unsigned i;int closed=0;
 error=scq_boundary_count(samples);memset(&logger,0,sizeof(logger));
 if(!error&&papacc_server_launcher_owner_acquire(&empty,&config,&logger,&owned)!=PAPACC_RESULT_OK)error=ERROR_NOT_ENOUGH_QUOTA;
 if(!error){
  /* Additional fixed sample storage; no handle-table allocation or traversal. */
  if(owned->reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT-sizeof(samples))error=ERROR_NOT_ENOUGH_QUOTA;
  else owned->reserved_bytes+=sizeof(samples);
 }
 if(!error){owned->configuration_commit.writer=owned->owner_thread;owned->configuration_commit.pending_commit.file=INVALID_HANDLE_VALUE;
  error=scq_boundary_count(samples+3);if(!error)error=papacc_configuration_personal_cold_handle_observe(&owned->configuration_commit,samples);
 }
 if(owned){PAPACC_PERSONAL_NATIVE_CONTEXT *n=&owned->configuration_commit.pending_commit.personal_native;
  if(papacc_configuration_personal_native_close(&owned->configuration_commit)==PAPACC_RESULT_OK&&
    (!n->console.is_installed||papacc_server_console_win32_uninstall(&n->console)==PAPACC_RESULT_OK)&&
    papacc_server_launcher_owner_finish(owned,&empty,PAPACC_TRUE)==PAPACC_RESULT_OK)closed=1;
 }else closed=1;
 for(i=0;i<5U;i++)fprintf(out,"COLD_BOUNDARY slot=%u known=%lu handles=%lu error=%lu; POST_CRT_NOT_PRELOADER; NOT_CAPACITY\n",i,(unsigned long)samples[3U*i],(unsigned long)samples[3U*i+1U],(unsigned long)samples[3U*i+2U]);
 fprintf(out,"COLD_BOUNDARY_RESULT first=%lu close_known=%u sample_bytes=%u; AUTHORITY_DENY; NO_ORIGINALS_NO_RNG_NO_SCM_NO_ENDPOINT_NO_START_NO_RETRY; OS_QUIESCENCE=UNKNOWN\n",(unsigned long)error,(unsigned)closed,(unsigned)sizeof(samples));
 if(!closed){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);fflush(out);Sleep(INFINITE);return 2;}
 return error?2:0;
}
static int scq_interval_observe(FILE *out)
{
 PAPACC_SERVER_NETWORK empty=PAPACC_SERVER_NETWORK_INITIALIZER;PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;
 PAPACC_LOGGER logger;PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *owned=NULL;PAPACC_SCQ_INTERVAL_OBSERVATION v;
 DWORD e=0;unsigned i;int closed=0;
 memset(&v,0,sizeof(v));v.first_cut=MAXDWORD;memset(&logger,0,sizeof(logger));
 if(papacc_server_launcher_owner_acquire(&empty,&config,&logger,&owned)!=PAPACC_RESULT_OK)e=ERROR_NOT_ENOUGH_QUOTA;
 if(!e){
  if(owned->reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT-sizeof(v))e=ERROR_NOT_ENOUGH_QUOTA;
  else {owned->reserved_bytes+=sizeof(v);owned->configuration_commit.writer=owned->owner_thread;owned->configuration_commit.pending_commit.file=INVALID_HANDLE_VALUE;
   e=papacc_configuration_personal_request_interval_observe(&owned->configuration_commit,&v);}
 }
 if(owned&&!owned->configuration_commit.unresolved){PAPACC_PERSONAL_NATIVE_CONTEXT *n=&owned->configuration_commit.pending_commit.personal_native;
  if(papacc_configuration_personal_native_close(&owned->configuration_commit)==PAPACC_RESULT_OK&&
   (!n->console.is_installed||papacc_server_console_win32_uninstall(&n->console)==PAPACC_RESULT_OK)&&
   papacc_server_launcher_owner_finish(owned,&empty,PAPACC_TRUE)==PAPACC_RESULT_OK)closed=1;
 }else if(!owned)closed=1;
 for(i=0;i<9U;i++)fprintf(out,"P1_INTERVAL cut=%u known=%lu handles=%lu error=%lu slots=0x%02lx; OBSERVATION_NOT_CAPACITY\n",i,
  (unsigned long)v.sample[i][0],(unsigned long)v.sample[i][1],(unsigned long)v.sample[i][2],(unsigned long)v.sample[i][3]);
 if(owned){const PAPACC_SCQ_COLD_DIAGNOSTIC *d=&owned->configuration_commit.pending_commit.personal_native.scq_cold_diagnostic;
  fprintf(out,"P1_INTERVAL_NATIVE attempted=%lu ntstatus=0x%08lx iosb_status_snapshot=0x%08lx information_snapshot=%llu returned_handle=%lu hash_called=%lu hash_status=0x%08lx hash_cleanup=0x%08lx; STATUS_SNAPSHOTS_NOT_NATIVE_CALL_PROOFS; NOT_AUTHORITY\n",
   (unsigned long)d->open_attempted,(unsigned long)d->open_status,(unsigned long)d->io_status,(unsigned long long)d->io_information,(unsigned long)d->returned_handle,(unsigned long)v.hash_called,(unsigned long)v.hash_status,(unsigned long)v.hash_cleanup_status);
 }
 fprintf(out,"P1_INTERVAL_RESULT first=%lu first_cut=%lu close_known=%u identity_checked=%lu final_guard=%lu sample_bytes=144 observation_bytes=%u; WORKSPACE_DIAGNOSTIC_NOT_INSTALLED_SOURCE; NO_DECISION_OPEN_NO_RNG_NO_SCM_NO_ENDPOINT_NO_START; AUTHORITY_DENY; NO_RETRY; OS_QUIESCENCE=UNKNOWN\n",
  (unsigned long)(v.first_error?v.first_error:e),(unsigned long)v.first_cut,(unsigned)closed,(unsigned long)v.identity_checked,(unsigned long)v.final_guard,(unsigned)sizeof(v));
 if(!closed){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);fflush(out);Sleep(INFINITE);return 2;}
 return e?2:0;
}
static int scq_machine_fixture_cli(int argc,const char *const *argv,FILE *out,FILE *err)
{
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_OFFLINE_D3_WORKER) && !defined(PAPACC_SCM_INSTRUMENTED_LAB) && !defined(PAPACC_SCM_REGISTRATION_LAB)
 PAPACC_SERVER_NETWORK empty=PAPACC_SERVER_NETWORK_INITIALIZER;PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;
 PAPACC_LOGGER logger;PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *owned=NULL;PAPACC_MACHINE_FIXTURE_INPUT input;
 PAPACC_MACHINE_FIXTURE_CONTROL *f;WCHAR request[MAX_PATH],decision[MAX_PATH];DWORD e;unsigned i;int closed=0;
 if(argc!=9||!argv||!out||!err)return 1;memset(&input,0,sizeof(input));
 if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argv[2],-1,request,MAX_PATH)||
  !MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argv[4],-1,decision,MAX_PATH)||
  !scq_cold_hex(argv[3],input.pins[0])||!scq_cold_hex(argv[5],input.pins[1]))return 1;
 for(i=0;i<3U;i++)if(!scq_cold_hex(argv[6U+i],input.pins[2U+i]))return 1;
 input.request_path=request;input.decision_path=decision;input.known_managed=papacc_server_launcher_machine_fixture_known_bytes();
 if(input.known_managed>PAPACC_LAUNCHER_MANAGED_LIMIT||papacc_server_console_win32_stop_requested())return 2;
 memset(&logger,0,sizeof(logger));if(papacc_server_launcher_owner_acquire(&empty,&config,&logger,&owned)!=PAPACC_RESULT_OK)return 2;
 owned->reserved_bytes=input.known_managed;owned->configuration_commit.writer=owned->owner_thread;owned->configuration_commit.pending_commit.file=INVALID_HANDLE_VALUE;
 f=&owned->configuration_commit.pending_commit.personal_native.machine_fixture;
 e=papacc_configuration_personal_machine_fixture_source(&owned->configuration_commit,&input);
 if(!e)e=papacc_configuration_personal_machine_fixture_execute(&owned->configuration_commit,out);
 (void)fprintf(out,"MACHINE_FIXTURE_OBSERVATION first=%lu source_stage=%lu consumed=%lu created=%u positive_control=%u identity_known=%u ntstatus=0x%08lx sd_ntstatus=0x%08lx budget_first=%lu; NO_E_Q_S_P_NO_CA_CURRENT_NO_RETRY\n",
  (unsigned long)e,(unsigned long)f->source_stage,(unsigned long)f->source_consumed,(unsigned)f->key.created,(unsigned)f->key.diagnostic_control_confirmed,f->phase==2U?1U:0U,
  (unsigned long)f->source_status,(unsigned long)owned->configuration_commit.pending_commit.personal_native.scq_inputs.native_sd.status,(unsigned long)f->key.diagnostic_budget.first);
 (void)fprintf(out,"MACHINE_FIXTURE_NATIVE_CAPTURE document_calls=%lu document_last=0x%08lx chain_calls=%lu chain_last=0x%08lx rng_calls=%lu rng_last=0x%08lx; LAST_CAPTURE_NOT_CAUSE_OR_AUTHORITY; NO_NEW_QUERY\n",
  (unsigned long)f->source_open_calls,(unsigned long)f->source_status,(unsigned long)f->source_chain_calls,(unsigned long)owned->configuration_commit.pending_commit.personal_native.scq_inputs.image_chain.open_status,(unsigned long)f->source_rng_calls,(unsigned long)f->source_rng_status);
 {PAPACC_CA_FIXTURE_BUDGET *b=&f->key.diagnostic_budget;
  (void)fprintf(out,"MACHINE_FIXTURE_BUDGET_CAPTURE attempts=%lu samples=%lu query_error=%lu query_stage=%lu initial_handles=%lu last_handles=%lu physical=%llu commit=%llu; EXISTING_CAPTURE_ONLY_NO_NEW_QUERY; HISTORICAL_CAUSE_UNKNOWN; NOT_CAPACITY_NOT_AUTHORITY\n",
   (unsigned long)b->observation.attempts,(unsigned long)b->observation.samples,(unsigned long)b->observation.first_error,(unsigned long)b->observation.error_stage,
   (unsigned long)b->observation.first.handles,(unsigned long)b->observation.last.handles,b->observation.last.physical_available,b->observation.last.commit_available);
  (void)fprintf(out,"MACHINE_FIXTURE_GROWTH_CAPTURE sample_known=%u initial_private=%llu last_private=%llu initial_working=%llu last_working=%llu; EXISTING_CAPTURE_ONLY_NO_NEW_QUERY; BYTE_VALUES_NOT_NATIVE_MAXIMA_NOT_AUTHORITY\n",
   b->observation.samples?1U:0U,b->observation.first.private_bytes,b->observation.last.private_bytes,b->observation.first.working_set,b->observation.last.working_set);
 }
 if(f->phase==2U){
  (void)fprintf(out,"MACHINE_FIXTURE_KEY_NAME=%ls\nMACHINE_FIXTURE_PUBLIC_SHA256=",f->key.name);
  for(i=0;i<32U;i++)(void)fprintf(out,"%02x",(unsigned)f->public_id[i]);(void)fprintf(out,"\nPUBLIC_DATA_NOT_AUTHORITY; FUTURE_READ_REQUIRES_NEW_ORIGINALS_AND_POSITIVE_REOPEN\n");
 }
 if(papacc_configuration_personal_native_close(&owned->configuration_commit)==PAPACC_RESULT_OK&&
  (!owned->configuration_commit.pending_commit.personal_native.console.is_installed||papacc_server_console_win32_uninstall(&owned->configuration_commit.pending_commit.personal_native.console)==PAPACC_RESULT_OK)&&
  papacc_server_launcher_owner_finish(owned,&empty,PAPACC_TRUE)==PAPACC_RESULT_OK)closed=1;
 (void)fprintf(out,"MACHINE_FIXTURE_RESULT first=%lu close_known=%u persistent_key_created=%u; NO_RETRY_NO_DELETE_NO_PRODUCTIVE_PROMOTION; HISTORICAL_RETENTION_AND_OS_QUIESCENCE=UNKNOWN\n",(unsigned long)e,(unsigned)closed,(unsigned)f->key.created);
 if(!closed){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);fflush(out);fflush(err);Sleep(INFINITE);return 2;}
 return e?2:0;
#else
 (void)argc;(void)argv;(void)out;(void)err;return 2;
#endif
}
int papacc_server_launcher_scq_original_cli(int argc,const char *const *argv,FILE *out,FILE *err)
{
 if(argc>1&&argv&&argv[1]&&!strcmp(argv[1],"--owner-manual-machine-fixture-prepare"))return scq_machine_fixture_cli(argc,argv,out,err);
#if defined(PAPACC_C8_PRIVATE_STORAGE_PROFILE) && defined(PAPACC_OFFLINE_D3_WORKER) && !defined(PAPACC_SCM_INSTRUMENTED_LAB) && !defined(PAPACC_SCM_REGISTRATION_LAB)
 PAPACC_SERVER_NETWORK empty=PAPACC_SERVER_NETWORK_INITIALIZER;
 PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;PAPACC_LOGGER logger;
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *owned=NULL;PAPACC_SCQ_COLD_START_INPUT input;PAPACC_SCQ_READ_COLD_INPUT read;unsigned machine_read;
 WCHAR request[MAX_PATH],decision[MAX_PATH];ULONGLONG values[8];DWORD e;unsigned i;int result=2;
 const char *start[]={"PapinhoAccelerator","service","start","--scq-closed"};
 if(argc==2&&argv&&argv[1]&&out&&!strcmp(argv[1],"--observe-cold-handle-boundaries"))return scq_boundary_observe(out);
 if(argc==2&&argv&&argv[1]&&out&&!strcmp(argv[1],"--observe-p1-request-decision-interval"))return scq_interval_observe(out);
 if(argc<2||!argv||!argv[1]||
  (strcmp(argv[1],"--owner-manual-first-scq-start")&&strcmp(argv[1],"--owner-manual-first-scq-start-to-existing")&&strcmp(argv[1],"--owner-manual-machine-read-scq-start")))return -1;
 machine_read=!strcmp(argv[1],"--owner-manual-machine-read-scq-start");if(argc!=(machine_read?17:15)||!out||!err)return 1;memset(&input,0,sizeof(input));memset(&read,0,sizeof(read));
 if(machine_read){if(strlen(argv[15])>=sizeof(read.key)||!scq_cold_hex(argv[16],read.public_id))return 1;strcpy_s(read.key,sizeof(read.key),argv[15]);}
 /* Explicit fixed-purpose adapter: no tuple/owner/permission arguments added.
  * Selection DATA comes through the existing native console after token checks. */
 input.existing_console_data=machine_read||!strcmp(argv[1],"--owner-manual-first-scq-start-to-existing")?1U:0U;
 if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argv[2],-1,request,MAX_PATH)||
  !MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argv[4],-1,decision,MAX_PATH)||
  !scq_cold_hex(argv[3],input.pins[0])||!scq_cold_hex(argv[5],input.pins[1])||!scq_cold_hex(argv[6],input.pins[2]))return 1;
 for(i=0;i<8U;i++)if(!scq_cold_u64(argv[7U+i],&values[i]))return 1;
 for(i=4;i<8U;i++)if(values[i]>MAXDWORD)return 1;
 input.request_path=request;input.decision_path=decision;
 input.limits.private_growth=values[0];input.limits.working_growth=values[1];input.limits.physical_floor=values[2];input.limits.commit_floor=values[3];
 input.limits.max_handles=(DWORD)values[4];input.limits.freshness_ms=(DWORD)values[5];input.limits.duration_ms=(DWORD)values[6];input.authorization_ms=(DWORD)values[7];
 if(input.limits.max_handles!=(input.existing_console_data?PAPACC_SCQ_P1_CAP:128U)||papacc_server_console_win32_stop_requested())return 2;
 memset(&logger,0,sizeof(logger));
 if(papacc_server_launcher_owner_acquire(&empty,&config,&logger,&owned)!=PAPACC_RESULT_OK)return 2;
 owned->configuration_commit.writer=owned->owner_thread;owned->configuration_commit.pending_commit.file=INVALID_HANDLE_VALUE;
 if(machine_read){read.cold=input;e=papacc_configuration_personal_scq_cold_machine_read_controller(&owned->configuration_commit,&read);}
 else e=papacc_configuration_personal_scq_cold_controller(&owned->configuration_commit,&input);
 if(!e)result=papacc_server_service_cli(4,start,out,err,NULL);
 else {
  const PAPACC_SCQ_COLD_DIAGNOSTIC *d=&owned->configuration_commit.pending_commit.personal_native.scq_cold_diagnostic;
  (void)fprintf(err,"SCQ_COLD_ORIGINAL=DENY; WIN32=%lu; NO_SCM_START; NO_RETRY\n",(unsigned long)e);
  (void)fprintf(err,"SCQ_COLD_FIRST_CAUSE stage=%lu reference=%lu mapped=%lu native_error=%lu query_attempted=%lu query_succeeded=%lu count_known=%lu own_count=%lu query_reference=%lu guard=%lu cap=%lu initial_known=%lu initial_handles=%lu; SCOPE=ORIGINAL_PRE_OPEN_QUERY_NOT_NEW_OBSERVATION; UNKNOWN_NOT_ZERO\n",
   (unsigned long)d->stage,(unsigned long)d->reference,(unsigned long)d->mapped,(unsigned long)d->native_error,
   (unsigned long)d->query_attempted,(unsigned long)d->query_succeeded,(unsigned long)d->query_succeeded,
   (unsigned long)d->own_count,(unsigned long)d->query_reference,
   (unsigned long)PAPACC_SCQ_HANDLE_GUARD(input.existing_console_data),(unsigned long)PAPACC_SCQ_HANDLE_CAP(input.existing_console_data),
   (unsigned long)d->initial_known,(unsigned long)d->initial_handles);
  (void)fprintf(err,"SCQ_COLD_NATIVE_OPEN attempted=%lu reference=%lu ntstatus=0x%08lx iosb_status_snapshot=0x%08lx iosb_information_snapshot=%llu handle_returned=%lu; CAPTURE_NOT_NEW_QUERY; IOSB_INITIALIZED_NOT_PROVEN_WRITTEN_ON_FAILURE; METADATA_NOT_AUTHORITY\n",
   (unsigned long)d->open_attempted,(unsigned long)d->open_reference,(unsigned long)d->open_status,(unsigned long)d->io_status,
   (unsigned long long)d->io_information,(unsigned long)d->returned_handle);
  (void)fprintf(err,"SCQ_COLD_NATIVE_SD calls=%lu ntstatus=0x%08lx needed=%lu capacity=%lu; ORIGINAL_RETAINED_FILE_REFERENCE; NO_HANDLE_REDUCTION_INFERRED; METADATA_NOT_AUTHORITY\n",
   (unsigned long)d->native_sd.calls,(unsigned long)d->native_sd.status,(unsigned long)d->native_sd.needed,(unsigned long)d->native_sd.capacity);
  (void)fprintf(err,"SCQ_COLD_FINAL_PLAN attempted=%lu passed_mask=%lu rejected=%lu unbound_reason=%lu unbound_field=%lu window_known=%lu from=%llu until=%llu now=%llu; REJECTED_1_STATIC_2_UNBOUND_3_WINDOW; NO_FIELD_VALUES_NO_AUTHORITY\n",
   (unsigned long)d->plan_attempted,(unsigned long)d->plan_passed,(unsigned long)d->plan_rejected,
   (unsigned long)d->unbound_reason,(unsigned long)d->unbound_field,(unsigned long)d->window_known,
   (unsigned long long)d->plan_from,(unsigned long long)d->plan_until,(unsigned long long)d->plan_now);
 }
 if(papacc_server_launcher_owner_state()==PAPACC_LAUNCHER_ACTIVE){
  PAPACC_PERSONAL_NATIVE_CONTEXT *n=&owned->configuration_commit.pending_commit.personal_native;
  if(owned->configuration_commit.unresolved||n->scq_admin.value.controller_consumed||n->scq_admin.start_consumed||n->scq_admin.endpoint_consumed||n->scq_session.transport.phase)goto retained;
  if(papacc_configuration_personal_native_close(&owned->configuration_commit)!=PAPACC_RESULT_OK||
   (n->console.is_installed&&papacc_server_console_win32_uninstall(&n->console)!=PAPACC_RESULT_OK)||
   papacc_server_launcher_owner_finish(owned,&empty,PAPACC_TRUE)!=PAPACC_RESULT_OK)goto retained;
 }
 if(papacc_server_launcher_owner_state()==PAPACC_LAUNCHER_RETAINED)goto retained;
 return result;
retained:
 InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);
 (void)fprintf(err,"SCQ_COLD_CLOSE=UNKNOWN; RETAINED=1; NO_RETRY; OS_QUIESCENCE=UNKNOWN\n");fflush(err);Sleep(INFINITE);return 2;
#else
 (void)argc;(void)argv;(void)out;(void)err;return -1;
#endif
}
