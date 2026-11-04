from pathlib import Path
import hashlib,json,re,subprocess
BASE=Path(__file__).resolve().parent;ROOT=BASE.parents[2]
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip();assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
baseline=json.loads((BASE/'ASA-SOURCE-HASHES.json').read_text(encoding='utf-8'))
allowed={'CMakeLists.txt','apps/server/personal_native_win32.h','apps/server/server_configuration_commit_win32.c','apps/server/server_configuration_commit_win32.h','apps/server/server_configuration_personal_data_win32.inc','apps/server/server_configuration_personal_native_win32.inc','tests/personal_ca_binding_win32_test.c','tests/scq_admin_source_test.c'}
new={'apps/server/personal_scq_session_win32.h','apps/server/personal_scq_session_win32.inc','tests/scq_session_driver_test.c'}
changed=set();rows=[]
for x in baseline:
 h=sha(ROOT/x['path']);rows.append(dict(path=x['path'],before_sha256=x['sha256'],sha256=h))
 if h!=x['sha256']:changed.add(x['path'])
assert changed==allowed,changed
for n in sorted(new):rows.append(dict(path=n,before_sha256=None,sha256=sha(ROOT/n)))
assert sha(BASE/'ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md')=='e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1'
assert sha(BASE/'OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md')=='34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561'
assert sha(ROOT/'docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt')=='7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667'
for n,h in [('scm-n0-private-stdout-native.c','a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8'),('scm-n0-private-stdout-native.h','a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471')]:assert sha(ROOT/'tools/qualification'/n)==h
native=(ROOT/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8')
selector=re.search(r'personal_scq_registered_owner_decision\(void\)\s*\{([^}]+)\}',native);assert selector and 'return NULL;' in selector.group(1)
core=(ROOT/'apps/server/personal_scq_session_win32.inc').read_text(encoding='utf-8')
for api in ['CreateNamedPipe','ConnectNamedPipe','CreateFile','ReadFile','WriteFile','GetOverlappedResult','StartService','ControlService','CreateService','ImpersonateNamedPipeClient','NCrypt','personal_scq_dispatch_body']:assert api not in core
for name in ['session_acquire_originals','session_prepare','session_take_data','session_offer_data','session_receive_data','session_receipt_data','session_ack_data','session_output_data','session_peer_lost']:assert 'papacc_configuration_personal_scq_'+name in native
assert 'scqs_close(&n->scq_session,o,1)' in native
configs=[]
for b in ['Selected','Legacy']:
 for c in ['Debug','Release']:
  t=f'SDR-{b}-{c}';log=(BASE/(t+'-build.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'warning\s+[A-Z]+\d+|error\s+[A-Z]+\d+|FAILED:',log,re.I)
  assert '0 tests failed out of 12' in (BASE/(t+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(BASE/(t+'-LastTest.log')).read_text(encoding='utf-8-sig');assert 'cases=35 checks=711 retained_models=19 driver_bytes=1136 buffer_bytes=1052' in last
  assert 'ECHO_IS_DATA_ONLY NO_NATIVE_AUTHENTICATED_ACK' in last
  bins=[]
  for n in ['papacc_server.exe','papacc_scq_session_driver_test.exe','papacc_scq_admin_source_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe']:
   f=ROOT/'build/phase6-functional-20261028'/b/c/n;assert f.exists();bins.append(dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)))
  configs.append(dict(backend=b,configuration=c,build='PASS_ZERO_WARNINGS',ctests=12,driver_cases=35,driver_checks=711,retained_model_cases=19,binaries=bins,layout=(BASE/(t+'-layout.log')).read_text(encoding='utf-8-sig').strip()))
(BASE/'SDR-SOURCE-HASHES.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
q=dict(task='PACC-PHASE6-ASA-SCQ-SESSION-DRIVER-OFFLINE',HEAD=head,driver='CONNECTED_TO_EXISTING_PACKET_SOURCE_PROFILE_RECEIVER_CLOSE',configs=configs,ctest_executions=48,embedded_driver_bytes=1136,owned_buffer_bytes=1052,persistent_growth_bytes=1136,extra_endpoint_handles=0,io_backend='NONE_DATA_COMPOSITION_ONLY',native_operational_registration='EMPTY_DENY',governed_consumption='SOURCE_CONTROLLER_TAKE_THEN_RECIPIENT_USE_MODEL',restart_or_persistence='NOT_PROVED_NATIVELY',receipt='DATA_ONLY_ECHO_NOT_NATIVE_AUTHENTICATION',native_bilateral_authentication='NOT_DEMONSTRATED',endpoint='NOT_CREATED_NOT_CONNECTED_NOT_TESTED',SCM_CNG='NOT_EXECUTED',new_install='NOT_EXECUTED',operational_request='NOT_READY_NOT_EMITTED',first_native_gap='NATIVE_IO_AND_LOCAL_ORIGIN_HANDOFF_ADAPTER_UNQUALIFIED_REAL_VIRTUAL_PEER_NOT_OBSERVED',persistent_key='NOT_OPENED_RECEIPT_UNCHANGED',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY')
(BASE/'SDR-QUALIFICATION.json').write_text(json.dumps(q,indent=2)+'\n',encoding='utf-8')
files=list(BASE.glob('SDR-*'))+[BASE/'NEXT-SDR-NATIVE-ENDPOINT-ADAPTER-OFFLINE.md']
(BASE/'SDR-ARTIFACT-HASHES.json').write_text(json.dumps([dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)) for f in sorted(files) if f.is_file() and f.name!='SDR-ARTIFACT-HASHES.json'],indent=2)+'\n',encoding='utf-8')
print('PASS SDR: 4 builds / 48 CTests / 35 driver cases / 711 checks/config; no endpoint or authenticated native ack; original registration EMPTY/DENY')
