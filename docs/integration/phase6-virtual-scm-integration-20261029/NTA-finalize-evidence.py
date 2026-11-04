from pathlib import Path
import hashlib,json,re,subprocess,csv,io
BASE=Path(__file__).resolve().parent;ROOT=BASE.parents[2]
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
baseline=json.loads((BASE/'SDR-SOURCE-HASHES.json').read_text(encoding='utf-8'))
allowed={'CMakeLists.txt','apps/server/personal_scq_session_win32.h','apps/server/personal_scq_session_win32.inc','tests/scq_session_driver_test.c','apps/server/server_configuration_personal_native_win32.inc','apps/server/server_configuration_commit_win32.h','tests/personal_ca_binding_win32_test.c'}
new={'apps/server/personal_scq_transport_win32.h','apps/server/personal_scq_transport_win32.inc','tests/scq_native_adapter_test.c'}
changed=set();sources=[]
for r in baseline:
 h=sha(ROOT/r['path']);sources.append(dict(path=r['path'],before_sha256=r['sha256'],sha256=h))
 if h!=r['sha256']:changed.add(r['path'])
assert changed==allowed,changed
for n in sorted(new):sources.append(dict(path=n,before_sha256=None,sha256=sha(ROOT/n)))
for n,h in [('ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md','e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1'),('OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md','34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561')]:assert sha(BASE/n)==h
for n,h in [('scm-n0-private-stdout-native.c','a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8'),('scm-n0-private-stdout-native.h','a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471')]:assert sha(ROOT/'tools/qualification'/n)==h
receipt=ROOT/'docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt'
assert sha(receipt)=='7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667'
native=(ROOT/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8')
assert 'return NULL;' in re.search(r'personal_scq_registered_owner_decision\(void\)\s*\{([^}]+)\}',native).group(1)
assert 'scqt_start_io(&n->scq_session' in native and 'scqt_apply(&n->scq_session' in native
# Factories have no native call site/activation. The only invoking unit is modeled.
assert 'scqt_create(' not in native and 'scqt_open_local(' not in native
core=(ROOT/'apps/server/personal_scq_transport_win32.inc').read_text(encoding='utf-8')
unit=(ROOT/'tests/scq_native_adapter_test.c').read_text(encoding='utf-8')
modeled=['CreateNamedPipeW','ConnectNamedPipe','CreateFileW','CreateEventW','DuplicateHandle','CloseHandle','OpenProcess','ReadFile','WriteFile','GetOverlappedResult','CancelIoEx','GetNamedPipeClientProcessId','GetNamedPipeServerProcessId','ResetEvent','GetHandleInformation']
for name in modeled:assert re.search(r'#define '+name+r' mt_',unit),name
for name in ['ImpersonateNamedPipeClient','StartService','ControlService','CreateService','NCrypt','DUPLICATE_CLOSE_SOURCE']:assert not re.search(r'\b'+name+r'\s*\(',core),name
assert 'static DWORD scqt_deliver_refs' in core and 'static DWORD scqt_accept_refs' in core
configs=[]
for b in ['Selected','Legacy']:
 for c in ['Debug','Release']:
  name=f'NTA-{b}-{c}'
  build=(BASE/(name+'-build.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'warning\s+[A-Z]+\d+|error\s+[A-Z]+\d+|FAILED:',build,re.I),name
  assert '0 tests failed out of 13' in (BASE/(name+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(BASE/(name+'-LastTest.log')).read_text(encoding='utf-8-sig')
  assert 'cases=53 checks=1131 retained_models=50 transport_bytes=664 driver_bytes=1800' in last,name
  assert 'cases=35 checks=711 retained_models=19 driver_bytes=1800' in last,name
  assert 'NO_ENDPOINT NO_NATIVE_BILATERAL_AUTH' in last
  layout=(BASE/(name+'-layout.log')).read_text(encoding='utf-8-sig')
  assert ('KNOWN_MANAGED=1978816' if b=='Selected' else 'KNOWN_MANAGED=1810248') in layout
  assert 'PROVIDER_KERNEL=UNKNOWN NATIVE_ADMISSION=DENY' in layout
  binaries=[]
  for exe in ['papacc_server.exe','papacc_scq_native_adapter_test.exe','papacc_scq_session_driver_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe']:
   f=ROOT/'build/phase6-functional-20261028'/b/c/exe;binaries.append(dict(path=f.relative_to(ROOT).as_posix(),sha256=sha(f)))
  configs.append(dict(backend=b,configuration=c,build='PASS_ZERO_WARNINGS',ctests=13,adapter_cases=53,adapter_checks=1131,retained_model_fixtures=50,driver_regression_cases=35,driver_regression_checks=711,layout=layout.strip(),binaries=binaries))
rows=list(csv.DictReader(io.StringIO((ROOT/'docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv').read_text(encoding='utf-8'))));assert len(rows)==36
(BASE/'NTA-SOURCE-HASHES.json').write_text(json.dumps(sources,indent=2)+'\n',encoding='utf-8')
q=dict(task='PACC-PHASE6-SDR-NATIVE-ENDPOINT-ADAPTER-OFFLINE',HEAD=head,configs=configs,ctest_executions=52,transport_bytes=664,driver_bytes=1800,fixed_growth=664,buffer_bytes=1052,endpoint_effect_apis='COMPILE_TIME_MODELED_ONLY',modeled_apis=modeled,real_helpers=['Windows SID/SD/ACL memory operations','BCryptGenRandom','isolated thread for ownership rejection'],factories='COMPILED_OFFLINE_NO_PRODUCT_ACTIVATION',native_registry='EMPTY_DENY',references='SENDER_ONLY_PUSH_READ_ONLY_NO_ADMIN_PROCESS_DUP_RIGHTS_TO_RUNTIME',pending_io='CUSTODY_AND_CANCEL_COMPLETION_SEPARATED_MODEL',remote_close='DATA_ACK_NOT_PROOF_RETAIN_UNTIL_NATIVE_CLOSE_PROOF_OR_ORIGINAL_PROCESS_EXIT',original_source='INDEPENDENT_LOCAL_ORIGIN_REQUIRED_NOT_REBUILT_FROM_FRAME',native_bilateral_authentication='NOT_DEMONSTRATED',endpoint='NOT_CREATED_NOT_CONNECTED_NOT_TESTED_NATIVELY',operational_request='NOT_READY_NOT_EMITTED',first_gate='ORIGINAL_ADMIN_RUNTIME_SCM_PAIR_AND_EXACT_OPERATIONAL_BINDINGS',SCM_CNG='NOT_EXECUTED',retained_persistent_key='NOT_OPENED_RECEIPT_UNCHANGED',NEW_INSTALL='NOT_EXECUTED',authority='DENY',E21='PENDING',ED11='PARTIAL',Phase6='PARTIAL')
(BASE/'NTA-QUALIFICATION.json').write_text(json.dumps(q,indent=2)+'\n',encoding='utf-8')
files=list(BASE.glob('NTA-*'))+[BASE/'NEXT-NTA-ORIGINAL-PAIR-AND-ENDPOINT-REQUEST.md',ROOT/'docs/capabilities/canonical-phase6-consolidation.md',ROOT/'docs/integration/n0start03-integrated-closeout-20261024/ED11_GATE_MATRIX.csv']
(BASE/'NTA-ARTIFACT-HASHES.json').write_text(json.dumps([dict(path=f.relative_to(ROOT).as_posix(),sha256=sha(f)) for f in sorted(files) if f.is_file() and f.name!='NTA-ARTIFACT-HASHES.json'],indent=2)+'\n',encoding='utf-8')
print('PASS NTA: 4 builds zero warnings; 52 CTests; 53 cases/1131 checks per config; endpoints modeled only; registry EMPTY/DENY')
