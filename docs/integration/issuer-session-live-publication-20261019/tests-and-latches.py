from pathlib import Path
r=Path.cwd();d=r/'docs/integration/issuer-session-live-publication-20261019'
def put(p,s):
 f=d/'proposed'/p.replace('/','__')
 if not f.exists():(d/'preimages'/p.replace('/','__')).write_bytes((r/p).read_bytes())
 f.write_text(s)
def get(p):
 f=d/'proposed'/p.replace('/','__');return f.read_text() if f.exists() else (r/p).read_text()
p='apps/server/server_launcher_owner_win32.h';s=get(p).replace('issuer_projection_validated,issuer_commit_started;','issuer_projection_validated,issuer_commit_started,issuer_publication_attempted;',1);put(p,s)
p='apps/server/server_configuration_root_transaction_win32.inc';s=get(p).replace('o->issuer_commit_started=PAPACC_FALSE;}','o->issuer_commit_started=PAPACC_FALSE;o->issuer_publication_attempted=PAPACC_FALSE;}',1)
s=s.replace('if(c->prepared||c->committed_pending||c->pending_commit.active||','if(issuer06_root_output_overlap(o,image,PAPACC_ISSUER_IMAGE_BYTES)||c->prepared||c->committed_pending||c->pending_commit.active||',1)
a=' EnterCriticalSection(&c->lock);\n result=papacc_configuration_issuer_publish06(c);';b=' if(o->issuer_publication_attempted)return PAPACC_RESULT_INVALID_STATE;\n o->issuer_publication_attempted=PAPACC_TRUE;\n EnterCriticalSection(&c->lock);\n result=papacc_configuration_issuer_publish06(c);';assert a in s;s=s.replace(a,b,1)
s=s.replace('o->issuer_projection_validated=PAPACC_FALSE;o->issuer_commit_started=PAPACC_FALSE;o->issuer_expected_revision=0;','o->issuer_projection_validated=PAPACC_FALSE;o->issuer_commit_started=PAPACC_FALSE;o->issuer_publication_attempted=PAPACC_FALSE;o->issuer_expected_revision=0;',1);put(p,s)
p='tests/canonical_root_transaction_test.c';s=get(p)
a=' if(argc>1&&!strcmp(argv[1],"catalog-issuer06"))sprintf_s(relative';assert a in s
s=s.replace(a,' if(argc>1&&!strncmp(argv[1],"catalog-live",12))sprintf_s(relative,sizeof(relative),"build/issuer-session-live-publication-20261019/root-%lu-%s.bin",GetCurrentProcessId(),argv[1]);\n else if(argc>1&&!strcmp(argv[1],"catalog-issuer06"))sprintf_s(relative',1)
a=' if(argc>1&&!strcmp(argv[1],"catalog-issuer06")){';assert a in s
s=s.replace(a,''' if(argc>1&&!strncmp(argv[1],"catalog-live",12)){
  static PAPACC_U8 image[8192],digest[32],old_image[8192];PAPACC_ISSUER_VIEW iv;PAPACC_ISSUER_RECORD record;
  PAPACC_ROOT_CURRENT_PIN fresh,third,bad;PAPACC_ISSUER_CAS_RESULT outcome;
  PAPACC_U64 original_p=t->current_policy_epoch,original_a=pin.administrative_epoch;DWORD started;
  PAPACC_SERVER_CONFIG changed=config;PAPACC_SERVER_C2_OWNER_WIN32 *consumer;
  PAPACC_C2_SESSION *older,*newer;HANDLE held=INVALID_HANDLE_VALUE;unsigned tries=0;
  int fail_conflict=!strcmp(argv[1],"catalog-live-conflict"),fail_unknown=!strcmp(argv[1],"catalog-live-unknown"),stop_case=!strcmp(argv[1],"catalog-live-stop");
  memset(&fresh,0,sizeof(fresh));memset(&third,0,sizeof(third));memset(&iv,0,sizeof(iv));memset(&record,0,sizeof(record));
  memcpy(iv.root,o->configuration_commit.domain,32);memcpy(iv.policy,config.policy.identity,32);
  iv.authority[0]=1;iv.generation[0]=2;iv.prefix[0]=3;iv.ca_spki_digest[0]=4;iv.revision=2;iv.high_water=1;iv.ca_not_after=1000000;
  CHECK(papacc_configuration_issuer_hash06(&o->configuration_commit,o->configuration_commit.committed,8192,digest));
  CHECK(papacc_issuer_image_init(image,&iv,digest,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
  CHECK(pin.epoch==original_p&&original_a&&pin.view.revision==1&&!pin.issuer_present);
  /* Existing normal runtime/wait-set attachment; no listener or Connection. */
  CHECK(papacc_server_launcher_attach_closed(o,t->combined_reserved+4096U)==0);
  consumer=o->io_loop.c2_owner;CHECK(consumer&&consumer->initialized&&consumer->manager->initialized);
  older=&consumer->sessions[0];newer=&consumer->sessions[1];
  /* Synthetic REVOKED lifecycle metadata; not authentication or a grant.
   * Real root pin acquired before exposing the simulated Session reference. */
  CHECK(papacc_server_launcher_configuration_current_pin(o,&o->session_revision_pins[0])==0);
  o->session_revision_pins[0].session_incarnation=55;
  older->owner=consumer->manager->owner;older->incarnation=55;older->initialized=PAPACC_TRUE;
  older->state=PAPACC_C2_SESSION_REVOKED;older->root_bound=PAPACC_TRUE;older->root_invocation=o->invocation;
  older->root_revision=1;older->policy_epoch=original_p;older->control_connection=700;
  memcpy(older->root_domain,pin.view.domain,32);consumer->manager->sessions[0].session=older;
  CHECK(!papacc_server_launcher_c2_session_current(older));
  changed.control_port++;CHECK(!papacc_configuration_issuer_projection06(&config,&changed));
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&changed,1,1)!=0);
  changed=config;changed.policy.identity[1]^=1;CHECK(!papacc_configuration_issuer_projection06(&config,&changed));
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&changed,1,1)!=0);
  changed=config;changed.site_tls.max_bytes++;CHECK(!papacc_configuration_issuer_projection06(&config,&changed));
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&changed,1,1)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,0,1)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,1,1)==0);
  CHECK(papacc_configuration_issuer_publish06(&o->configuration_commit)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,1,1)!=0);
  if(fail_conflict){DWORD n;unsigned char v=0;held=CreateFileA(path,GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);CHECK(held!=INVALID_HANDLE_VALUE);CHECK(WriteFile(held,&v,1,&n,NULL)&&n==1&&FlushFileBuffers(held));CHECK(CloseHandle(held));held=INVALID_HANDLE_VALUE;}
  if(fail_unknown){held=CreateFileA(path,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);CHECK(held!=INVALID_HANDLE_VALUE);}
  CHECK(papacc_server_launcher_configuration_issuer_publish_live(o,&outcome)==0&&outcome==PAPACC_COMMIT_PENDING);
  CHECK(!o->configuration_commit.committed_pending&&t->model.revision==1&&t->current_policy_epoch==original_p);
  CHECK(papacc_server_launcher_configuration_current_pin(o,&fresh)!=0&&!fresh.pin.ticket);
  CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)!=0);
  if(stop_case)CHECK(papacc_configuration_stop(&o->configuration_commit)==0);
  started=GetTickCount();do{
   result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);tries++;
   if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);
   CHECK(GetTickCount()-started<10000U);
  }while(outcome==PAPACC_COMMIT_PENDING);
  if(fail_conflict||fail_unknown||stop_case){
   CHECK(result!=0&&t->model.revision==1&&t->current_policy_epoch==original_p&&older->root_revision==1);
   CHECK(o->session_revision_pins[0].pin.ticket&&older->initialized);
   if(fail_conflict)CHECK(outcome==PAPACC_CAS_CONFLICT&&o->configuration_commit.authority_lost);
   if(fail_unknown){CHECK(outcome==PAPACC_CAS_UNKNOWN&&o->configuration_commit.authority_lost);CHECK(CloseHandle(held));held=INVALID_HANDLE_VALUE;}
   CHECK(!papacc_server_launcher_configuration_current(o,&pin));
   CHECK(papacc_server_launcher_configuration_issuer_publish_live(o,&outcome)!=0);
   CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)!=0);
   printf("PASS checks=%u LIVE_FAILURE=%s outcome=%d P_UNCHANGED OLD_SESSION_RETAINED NO_CURRENT_NO_RETRY NO_NETWORK\\n",checks,argv[1],(int)outcome);return 0;
  }
  CHECK(result==0&&outcome==PAPACC_CAS_SUCCESS&&t->model.revision==2&&t->current_policy_epoch==original_p);
  CHECK(!papacc_server_launcher_configuration_current(o,&pin));
  CHECK(papacc_server_launcher_site_profile(o,&pin,7,1,&selected)==0); /* held memory + independent CURRENT projection */
  CHECK(papacc_server_launcher_configuration_current_pin(o,&fresh)==0&&fresh.view.revision==2&&fresh.issuer_present&&fresh.issuer_revision==2);
  CHECK(fresh.epoch==original_p&&fresh.administrative_epoch&&fresh.administrative_epoch!=original_a);
  CHECK(older->root_revision==1&&older->policy_epoch==original_p&&older->grant_count==0);
  CHECK(papacc_server_launcher_configuration_current_pin(o,&o->session_revision_pins[1])==0);
  o->session_revision_pins[1].session_incarnation=56;
  newer->owner=consumer->manager->owner;newer->incarnation=56;newer->initialized=PAPACC_TRUE;newer->state=PAPACC_C2_SESSION_REVOKED;
  newer->root_bound=PAPACC_TRUE;newer->root_invocation=o->invocation;newer->root_revision=2;newer->policy_epoch=original_p;
  memcpy(newer->root_domain,fresh.view.domain,32);consumer->manager->sessions[1].session=newer;
  CHECK(!papacc_server_launcher_c2_session_current(newer));
  bad=pin;bad.epoch^=1;CHECK(papacc_server_launcher_site_profile(o,&bad,7,1,&selected)!=0);
  bad=pin;bad.administrative_epoch^=1;CHECK(papacc_server_launcher_site_profile(o,&bad,7,1,&selected)!=0);
  bad=pin;bad.pin.serial++;CHECK(papacc_server_launcher_site_profile(o,&bad,7,1,&selected)!=0);
  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&pin.pin,old_image)!=0); /* explicit absence at initial R */
  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,old_image)==0);
  record.request.id[0]=1;record.request.device[0]=7;record.request.key_digest[0]=8;record.request.csr_digest[0]=9;
  record.request.not_before=100;record.request.not_after=500;record.request.purpose=1;record.approved_epoch=original_p;record.state=PAPACC_ISSUER_PREPARED;
  memcpy(record.authority,iv.authority,16);memcpy(record.generation,iv.generation,16);memcpy(record.policy,iv.policy,32);
  CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,1,1)!=0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,2,1)==0);
  started=GetTickCount();do{result=papacc_server_launcher_configuration_issuer_publish_live(o,&outcome);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);CHECK(GetTickCount()-started<10000U);}while(outcome==PAPACC_COMMIT_PENDING);
  CHECK(result==0&&t->model.revision==3&&t->current_policy_epoch==original_p);
  CHECK(papacc_server_launcher_configuration_current_pin(o,&third)==0&&third.issuer_revision==3&&third.view.revision==3);
  CHECK(third.epoch==original_p&&third.administrative_epoch!=fresh.administrative_epoch);
  CHECK(older->root_revision==1&&newer->root_revision==2&&newer->grant_count==0);
  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,digest)==PAPACC_RESULT_INVALID_ARGUMENT); /* alias/size tested separately; no undersized output */
  record.state=PAPACC_ISSUER_SERIAL_RESERVED;record.counter=2;memcpy(record.serial,iv.prefix,8);record.serial[15]=2;
  CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&o->configuration_commit)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,3,1)==PAPACC_RESULT_LIMIT_EXCEEDED);
  CHECK(t->model.revision==3&&older->root_revision==1&&newer->root_revision==2);
  /* Each simulated borrower individually blocks actual reaper/unpin. Restore
   * test-only metadata only; no native resource is fabricated or released. */
  consumer->manager->operations[0].session=older;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);
  CHECK(papacc_server_c2_owner_win32_retire_sessions(consumer)==0&&older->initialized);consumer->manager->operations[0].session=NULL;
  consumer->writes[0].denial_session=older;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);consumer->writes[0].denial_session=NULL;
  {PAPACC_CHANNEL saved=o->channel_storage[0];o->channel_storage[0].state=(PAPACC_CHANNEL_STATE)1;o->channel_storage[0].session_instance_id=55;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->channel_storage[0]=saved;}
  {PAPACC_SERVER_PROTOCOL_SLOT_WIN32 saved=o->processor_storage[0];o->processor_storage[0].in_use=PAPACC_TRUE;o->processor_storage[0].connection_instance_id=700;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->processor_storage[0]=saved;}
  {PAPACC_U64 id=o->secure_candidates[0].control_close_result.session_instance_id;o->secure_candidates[0].control_close_result.session_instance_id=55;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->secure_candidates[0].control_close_result.session_instance_id=id;}
  o->session_revision_pins[0].pin.serial++;CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);o->session_revision_pins[0].pin.serial--;
  CHECK(papacc_server_c2_owner_win32_retire_sessions(consumer)==0&&!older->initialized&&!newer->initialized);
  CHECK(!o->session_revision_pins[0].pin.ticket&&!o->session_revision_pins[1].pin.ticket);
  CHECK(papacc_server_launcher_c2_session_unpin(consumer,older)!=0);
  CHECK(papacc_server_launcher_configuration_current_unpin(o,&fresh)==0);
  CHECK(papacc_server_launcher_configuration_issuer_prepare_live(o,image,&config,3,1)==0); /* freed middle slot is now reusable */
  /* Staging is explicit, not publication. Stop leaves it owned until abort. */
  CHECK(papacc_configuration_stop(&o->configuration_commit)==0);CHECK(papacc_configuration_abort(&o->configuration_commit)==0);
  CHECK(papacc_server_launcher_configuration_current_unpin(o,&third)==0);
  CHECK(papacc_server_launcher_configuration_current_unpin(o,&pin)==0);
  CHECK(papacc_server_launcher_detach_closed(o)==0);
  CHECK(papacc_server_launcher_configuration_transaction_close(o)==0);
  CHECK(papacc_server_launcher_owner_finish(o,&network,PAPACC_TRUE)==0);
  printf("PASS checks=%u LIVE_CURRENT_A_FACTUAL P_PRESERVED THREE_REVISIONS REAPER_PINS_RELEASED SYNTHETIC_REVOKED_SESSIONS NO_NETWORK\\n",checks);return 0;
 }
''' +a,1)
# Remove deliberately unsafe undersized output probe: API has fixed 8192 contract.
s=s.replace('  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,digest)==PAPACC_RESULT_INVALID_ARGUMENT); /* alias/size tested separately; no undersized output */\n','  CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,o->configuration_commit.issuer_staged)==PAPACC_RESULT_INVALID_ARGUMENT);\n')
# Full-size preserved snapshot comparison, never an undersized output.
s=s.replace('  CHECK(third.epoch==original_p&&third.administrative_epoch!=fresh.administrative_epoch);','  CHECK(third.epoch==original_p&&third.administrative_epoch!=fresh.administrative_epoch);\n  {PAPACC_U8 copy[8192];CHECK(papacc_configuration_issuer_snapshot06(&o->configuration_commit,&fresh.pin,copy)==0&&!memcmp(copy,old_image,8192));}',1)
put(p,s)
p='CMakeLists.txt';s=get(p);s+='''
# Session/live publication: .inc dependencies remain explicit for localized MSVC.
set_property(SOURCE apps/server/server_launcher_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c2_root_binding_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_admission_preflight_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_listener_composition_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c2_owner_win32.h")
set_property(SOURCE apps/server/server_c2_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_launcher_owner_win32.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c2_owner_win32.h")
''';put(p,s)
print('Prepared normal-library live root, negative CAS/Stop and synthetic Session reaper tests')
