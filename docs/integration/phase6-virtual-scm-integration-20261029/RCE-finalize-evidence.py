from pathlib import Path
import json,hashlib,re,subprocess
BASE=Path(__file__).resolve().parent;ROOT=BASE.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
old=json.loads((BASE/'NTA-SOURCE-HASHES.json').read_text(encoding='utf-8'))
allowed={'CMakeLists.txt','apps/server/personal_native_win32.h','apps/server/personal_scq_inputs_win32.h','apps/server/personal_scq_inputs_win32.inc','apps/server/server_configuration_commit_win32.c','apps/server/server_configuration_commit_win32.h','apps/server/server_configuration_personal_data_win32.inc','apps/server/server_configuration_personal_native_win32.inc','apps/server/server_service_win32.c','tests/personal_ca_binding_win32_test.c','tests/personal_scq_dispatch_test.c','apps/server/personal_scq_session_win32.h','apps/server/personal_scq_session_win32.inc','apps/server/personal_scq_transport_win32.h','apps/server/personal_scq_transport_win32.inc','tests/scq_native_adapter_test.c','tests/scq_session_driver_test.c'}
changed=set();sources=[]
for r in old:
 h=sha(ROOT/r['path']);sources.append(dict(path=r['path'],before_sha256=r['sha256'],sha256=h))
 if h!=r['sha256']:changed.add(r['path'])
assert changed==allowed,changed
for n in ['apps/server/personal_scq_recipient_win32.h','apps/server/personal_scq_recipient_win32.inc','tests/scq_recipient_close_test.c','apps/server/server_service_win32.h','tests/service_host_model_test.c']:
 sources.append(dict(path=n,before_sha256=None,sha256=sha(ROOT/n),prior_hash='NOT_CAPTURED' if n.endswith('server_service_win32.h') or n.endswith('service_host_model_test.c') else 'NEW_FILE'))
for n,h in [('ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md','e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1'),('OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md','34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561')]:assert sha(BASE/n)==h
for n,h in [('scm-n0-private-stdout-native.c','a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8'),('scm-n0-private-stdout-native.h','a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471')]:assert sha(ROOT/'tools/qualification'/n)==h
assert sha(ROOT/'docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt')=='7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667'
native=(ROOT/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8')
for name in ['owner','endpoint','cng']:
 body=re.search('personal_scq_registered_'+name+r'_decision\(void\)\s*\{([^}]+)\}',native)
 assert body and 'return NULL;' in body.group(1)
assert 'scqt_create(' in native and 'scqr_open(' in native and 'scqr_admit_offer(' in native
assert 'personal_scq_recipient_native_current' in native and 'papacc_server_service_scq_host_current' in native
service=(ROOT/'apps/server/server_service_win32.c').read_text(encoding='utf-8');assert 'static int ps_binding_ready(void){return 0;}' in service
origin=(ROOT/'apps/server/personal_scq_recipient_win32.inc').read_text(encoding='utf-8')
assert 'TOKEN_QUERY' in origin and 'PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE' in origin
assert 'scqr_operator_scope' in origin and 'descriptor_digest' in origin and 'scqr_inputs_scope' in origin
assert 'r->value.controller_consumed=1;r->phase=2;t->query_only=0;' in origin
core=(ROOT/'apps/server/personal_scq_transport_win32.inc').read_text(encoding='utf-8')
assert '"SQC1"' in core and 'close_accepted' in core and 'delivered_inputs' in core and 'closed_values' in core and 'value!=(ULONGLONG)(ULONG_PTR)t->remote_files[i]' in core
# The same compile-time mocked adapter translation unit contains all effectful calls.
unit=(ROOT/'tests/scq_native_adapter_test.c').read_text(encoding='utf-8')
for api in ['CreateNamedPipeW','ConnectNamedPipe','CreateFileW','CreateEventW','CloseHandle','ReadFile','WriteFile','GetOverlappedResult','CancelIoEx','GetNamedPipeClientProcessId','GetNamedPipeServerProcessId']:
 assert re.search('#define '+api+r' mt_',unit),api
configs=[]
for b in ['Selected','Legacy']:
 for c in ['Debug','Release']:
  t=f'RCE-{b}-{c}'
  build=(BASE/(t+'-build.log')).read_text(encoding='utf-8-sig');assert not re.search(r'warning\s+[A-Z]+\d+|error\s+[A-Z]+\d+|FAILED:',build,re.I),t
  assert '0 tests failed out of 16' in (BASE/(t+'-ctest.log')).read_text(encoding='utf-8-sig'),t
  log=(BASE/(t+'-LastTest.log')).read_text(encoding='utf-8-sig')
  for marker in ['cases=31 checks=739 retained_models=19 origin_bytes=760 transport_bytes=728','cases=53 checks=1131 retained_models=50 transport_bytes=728 driver_bytes=1888','cases=35 checks=711 retained_models=19 driver_bytes=1888','cases=41 checks=248','NO_ENDPOINT','Host dispatcher/name/args/denied-start/readiness-race/Stop/quiescence/retention MODEL PASS']:
   assert marker in log,(t,marker)
  layout=(BASE/(t+'-layout.log')).read_text(encoding='utf-8-sig');assert ('KNOWN_MANAGED=1979760' if b=='Selected' else 'KNOWN_MANAGED=1811192') in layout
  assert 'PROVIDER_KERNEL=UNKNOWN NATIVE_ADMISSION=DENY' in layout
  binaries=[]
  for name in ['papacc_server.exe','papacc_scq_recipient_close_test.exe','papacc_scq_native_adapter_test.exe','papacc_scq_session_driver_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe','papacc_service_host_product_model.exe','papacc_service_host_lab_model.exe']:
   p=ROOT/'build/phase6-functional-20261028'/b/c/name;binaries.append(dict(path=p.relative_to(ROOT).as_posix(),sha256=sha(p)))
  configs.append(dict(backend=b,configuration=c,build='PASS_ZERO_WARNINGS',ctests=16,recipient_cases=31,recipient_checks=739,retained_model_fixtures=19,layout=layout.strip(),binaries=binaries))
(BASE/'RCE-SOURCE-HASHES.json').write_text(json.dumps(sources,indent=2)+'\n',encoding='utf-8')
q=dict(task='PACC-PHASE6-SCQ-NATIVE-ROLE-ENTRY-AND-CLOSE-PROOF-OFFLINE',HEAD=head,configs=configs,ctest_executions=64,recipient_origin_bytes=760,transport_bytes=728,driver_bytes=1888,inputs_bytes=512,fixed_growth_bytes=944,buffer_bytes=1076,receiver_origin='RECEIVER_LOCAL_QUERY_THEN_INDEPENDENT_ORIGINAL_SCOPE_AND_CORRELATED_OFFER',query_only='NO_WRITES_DUP_OR_DIAGNOSTIC_USE_BEFORE_ORIGIN_ADMISSION',private_close_stage='SQC1_RECORD1048_CLOSED_REFERENCE_IDS24_BYTES1076_SEPARATE_FROM_SQR1_DATA',close_proof='CONTROLLED_CLOSE_RETURN_THEN_ORIGINAL_PEER_VALIDATION_MODELED_NOT_NATIVE_PROOF',effect_scopes='OWNER_ENDPOINT_CNG_SEPARATE_EMPTY_IMMUTABLE_DESIGNATIONS',host_bridge='OBSERVED_ORIGINAL_SERVICEMAIN_CALLBACK_AND_READY_STATE_ONLY_NOT_AUTHORITY',startup_binding='PS_BINDING_READY_LITERAL_ZERO_POSITIVE_SOURCE_NOT_COMPLETED',native_bilateral_authentication='NOT_DEMONSTRATED',endpoint='NOT_CREATED_NOT_CONNECTED_NOT_TESTED_NATIVELY',SCM_CNG='NOT_EXECUTED',persistent_key='NOT_OPENED_RECEIPT_UNCHANGED',operational_request='NOT_READY_NOT_EMITTED',first_remaining_gate='ORIGINAL_POSITIVE_SERVICE_STARTUP_SOURCE_TARGET_AND_QUANTITATIVE_POLICY',new_approvals_or_intents='NONE',E21='PENDING',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY')
(BASE/'RCE-QUALIFICATION.json').write_text(json.dumps(q,indent=2)+'\n',encoding='utf-8')
files=list(BASE.glob('RCE-*'))+[BASE/'NEXT-RCE-SERVICE-STARTUP-ORIGINAL-SOURCE.md',ROOT/'docs/capabilities/canonical-phase6-consolidation.md',ROOT/'docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv']
(BASE/'RCE-ARTIFACT-HASHES.json').write_text(json.dumps([dict(path=p.relative_to(ROOT).as_posix(),sha256=sha(p)) for p in sorted(files) if p.is_file() and p.name!='RCE-ARTIFACT-HASHES.json'],indent=2)+'\n',encoding='utf-8')
print('PASS RCE: 4 builds zero warnings / 64 CTests / 31 cases 739 checks/config; startup still closed; no operational effects')
