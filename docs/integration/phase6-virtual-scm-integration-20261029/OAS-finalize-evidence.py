from pathlib import Path
import json,hashlib,re,subprocess
BASE=Path(__file__).resolve().parent;ROOT=BASE.parents[2]
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip();assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
base=json.loads((BASE/'NRD-SOURCE-HASHES.json').read_text(encoding='utf-8'))
expected={'CMakeLists.txt','apps/server/personal_scq_inputs_win32.h','apps/server/personal_scq_inputs_win32.inc','apps/server/server_configuration_commit_win32.h','apps/server/server_configuration_personal_native_win32.inc','tests/personal_ca_binding_win32_test.c','tests/personal_scq_inputs_test.c'}
changed=set();rows=[]
for x in base:
 h=sha(ROOT/x['path']);rows.append(dict(path=x['path'],before_sha256=x['sha256'],sha256=h))
 if h!=x['sha256']:changed.add(x['path'])
assert changed==expected,changed
assert sha(BASE/'ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md')=='e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1'
assert sha(BASE/'OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md')=='34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561'
assert sha(ROOT/'docs/integration/phase6-persistent-ca-fixture-20261029/ONE-ATTEMPT.receipt.txt')=='7bf799c34a70a5dae672cfeba4d2bb6cae4a1acbb9d6569758e73b7fbe68f667'
assert 'personal_scq_receive_original(&n->scq_receiver,NULL,' in (ROOT/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8')
for n,h in [('scm-n0-private-stdout-native.c','a2235963f6427cf686b2a4791fa30f71bc8425c9d54039e66b0925c798dd21b8'),('scm-n0-private-stdout-native.h','a60594a081ea2863d409e89d7a95ef504f53dbaef41773b86cf75879182db471')]:assert sha(ROOT/'tools/qualification'/n)==h
configs=[]
for b in ['Selected','Legacy']:
 for c in ['Debug','Release']:
  tag=f'OAS-{b}-{c}';log=(BASE/(tag+'-build.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'warning [A-Z]\d+|error [A-Z]\d+|FAILED:',log,re.I)
  assert '0 tests failed out of 9' in (BASE/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(BASE/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig');assert 'cases=41 checks=248' in last
  binaries=[]
  for n in ['papacc_server.exe','papacc_personal_scq_inputs_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe']:
   f=ROOT/'build/phase6-functional-20261028'/b/c/n;assert f.exists();binaries.append(dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)))
  configs.append(dict(backend=b,configuration=c,build='PASS_ZERO_WARNINGS',ctests=9,input_checks=248,input_cases=41,binaries=binaries,layout=(BASE/(tag+'-layout.log')).read_text(encoding='utf-8-sig').strip()))
(BASE/'OAS-SOURCE-HASHES.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
q=dict(task='PACC-PHASE6-SCQ-ORIGINAL-IMAGE-AND-ADMIN-SOURCE',HEAD=head,changed_sources=sorted(changed),configs=configs,ctest_executions=36,custody_bytes=416,persistent_growth_bytes=168,additional_readers=3,original_content_acquisition='INTEGRATED_OFFLINE_FILE_APIS_MODELED_CNG_REAL',image_correlation='INTEGRATED_OFFLINE_PROCESS_PATH_APIS_MODELED_NOT_MAPPED_IMAGE_ATTESTATION',administrative_authority_source='NOT_COMPLETED_NULL_DENY',nonsecret_document_semantics='NOT_DEMONSTRATED_BY_HASH',native_bilateral_authentication='NOT_DEMONSTRATED',durable_consumption='NOT_DEMONSTRATED',endpoint='NOT_CREATED_NOT_CONNECTED_NOT_TESTED',SCM_CNG_session='NOT_EXECUTED',test_hash_close_failure='EXPECTED_UNKNOWN_CNG_REFS_RETAINED_IN_STATIC_FIXTURE_OWNER_NO_RETRY',persistent_fixture_key='NOT_OPENED_RECEIPT_UNCHANGED',operational_request='NOT_READY_NOT_EMITTED',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY')
(BASE/'OAS-QUALIFICATION.json').write_text(json.dumps(q,indent=2)+'\n',encoding='utf-8')
files=list(BASE.glob('OAS-*'))+[BASE/'NEXT-OAS-ATTRIBUTED-ADMIN-SOURCE.md']
(BASE/'OAS-ARTIFACT-HASHES.json').write_text(json.dumps([dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)) for f in sorted(files) if f.is_file() and f.name!='OAS-ARTIFACT-HASHES.json'],indent=2)+'\n',encoding='utf-8')
print('PASS OAS 4 builds / 36 CTests / 248 input checks per configuration; source NULL/DENY; no endpoint; historical pins unchanged')
