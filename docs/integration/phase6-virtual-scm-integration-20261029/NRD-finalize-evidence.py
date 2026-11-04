from pathlib import Path
import hashlib,json,re,subprocess
BASE=Path(__file__).resolve().parent
ROOT=BASE.parents[2]
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
assert subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip()=='f341985924d29870d8ff627ed37c8a0874ad6469'
baseline=json.loads((BASE/'BQI-SOURCE-HASHES.json').read_text(encoding='utf-8'))
allowed={'apps/server/personal_runtime_identity_win32.h','apps/server/personal_runtime_identity_win32.inc','apps/server/server_configuration_commit_win32.h','apps/server/server_configuration_personal_native_win32.inc','tests/personal_ca_binding_win32_test.c','tests/personal_runtime_identity_test.c'}
rows=[];changed=set()
for x in baseline:
 f=ROOT/x['path'];h=sha(f)
 if h!=x['sha256']:changed.add(x['path'])
 rows.append(dict(path=x['path'],before_sha256=x['sha256'],sha256=h))
assert changed==allowed,changed
assert sha(BASE/'ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md')=='e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1'
assert sha(BASE/'OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md')=='34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561'
receipt=ROOT/'docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt'
assert sha(receipt)=='7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667'
for name,h in [('scm-n0-private-stdout-native.c','a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8'),('scm-n0-private-stdout-native.h','a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471')]:
 assert sha(ROOT/'tools/qualification'/name)==h
native=(ROOT/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8')
assert 'personal_scq_receive_original(&n->scq_receiver,NULL,' in native
adapter=(ROOT/'apps/server/personal_runtime_identity_win32.inc').read_text(encoding='utf-8')
section=adapter[adapter.index('DWORD papacc_personal_runtime_observe_data('):adapter.index('DWORD papacc_personal_runtime_close(')]
for forbidden in ['CreateNamedPipe','ConnectNamedPipe','CreateFile','ReadFile','StartService','NCrypt','OpenProcess(']:assert forbidden not in section
configs=[]
for b in ['Selected','Legacy']:
 for c in ['Debug','Release']:
  tag=f'NRD-{b}-{c}'
  log=(BASE/(tag+'-build.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'warning [A-Z]\d+|error [A-Z]\d+|FAILED:',log,re.I),tag
  ct=(BASE/(tag+'-ctest.log')).read_text(encoding='utf-8-sig');assert '0 tests failed out of 8' in ct
  last=(BASE/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  assert 'PERSONAL_RUNTIME_IDENTITY checks=163' in last and 'SCQ_PRIVATE_PROFILE checks=3338' in last
  binaries=[]
  for n in ['papacc_server.exe','papacc_personal_runtime_identity_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe','papacc_scq_private_profile_test.exe']:
   f=ROOT/'build/phase6-functional-20261028'/b/c/n;assert f.exists();binaries.append(dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)))
  configs.append(dict(backend=b,configuration=c,build='PASS_ZERO_WARNINGS',ctests=8,runtime_checks=163,profile_checks=3338,binaries=binaries,layout=(BASE/(tag+'-layout.log')).read_text(encoding='utf-8-sig').strip()))
(BASE/'NRD-SOURCE-HASHES.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
q=dict(task='PACC-PHASE6-BQI-NATIVE-EVIDENCE-READINESS',changed_sources=sorted(changed),configs=configs,ctest_executions=32,observation_bytes=224,runtime_bytes=216,persistent_growth_bytes=0,known_mask=15,complete_mask=31,missing_observation='ORIGINAL_IMAGE',original_source='NULL_DENY',native_process_token_scm_test_APIs='MODELED',native_bilateral_authentication='NOT_DEMONSTRATED',durable_consumption='NOT_DEMONSTRATED',endpoint='NOT_CREATED_NOT_CONNECTED_NOT_TESTED',SCM_CNG='NOT_EXECUTED',retained_key='NOT_OPENED_RECEIPT_UNCHANGED',authority='DENY',ED11='PARTIAL',Phase6='PARTIAL')
(BASE/'NRD-QUALIFICATION.json').write_text(json.dumps(q,indent=2)+'\n',encoding='utf-8')
files=sorted(f for f in BASE.glob('NRD-*') if f.is_file() and f.name!='NRD-ARTIFACT-HASHES.json')
(BASE/'NRD-ARTIFACT-HASHES.json').write_text(json.dumps([dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)) for f in files],indent=2)+'\n',encoding='utf-8')
print('PASS NRD: 4 builds, 32 CTests, 6 changed sources; original source DENY; no endpoint; retained receipt unchanged')
