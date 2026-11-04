from pathlib import Path
import hashlib,json,re,subprocess
BASE=Path(__file__).resolve().parent;ROOT=BASE.parents[2]
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip();assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
baseline=json.loads((BASE/'OAS-SOURCE-HASHES.json').read_text(encoding='utf-8'))
allowed={'CMakeLists.txt','apps/server/personal_native_win32.h','apps/server/personal_scq_handoff_win32.h','apps/server/personal_scq_handoff_win32.inc','apps/server/server_configuration_commit_win32.c','apps/server/server_configuration_commit_win32.h','apps/server/server_configuration_personal_data_win32.inc','apps/server/server_configuration_personal_native_win32.inc','apps/server/server_launcher_personal_win32.inc','tests/personal_ca_binding_win32_test.c','tests/personal_scq_dispatch_test.c'}
new={'apps/server/personal_scq_admin_win32.h','apps/server/personal_scq_admin_win32.inc','apps/server/personal_confirmation_console_win32.inc','tests/scq_admin_source_test.c'}
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
assert 'personal_scq_receive_original(&n->scq_receiver,personal_scq_native_original(o),' in native
selector=re.search(r'personal_scq_registered_owner_decision\(void\)\s*\{([^}]+)\}',native);assert selector and 'return NULL;' in selector.group(1)
shared=(ROOT/'apps/server/personal_confirmation_console_win32.inc').read_text(encoding='utf-8');i=shared.index('static int personal_console_confirm(')
launcher=(ROOT/'apps/server/server_launcher_personal_win32.inc').read_text(encoding='utf-8')
restored=launcher.replace('#include "personal_confirmation_console_win32.inc"\n',shared[:i]).replace('static int personal_present(',shared[i:]+'static int personal_present(',1)
original_launcher=next(x['sha256'] for x in baseline if x['path']=='apps/server/server_launcher_personal_win32.inc')
assert any(hashlib.sha256(x).hexdigest()==original_launcher for x in [restored.encode('utf-8'),restored.replace('\n','\r\n').encode('utf-8')])
core=(ROOT/'apps/server/personal_scq_admin_win32.inc').read_text(encoding='utf-8')
for forbidden in ['CreateNamedPipe','ConnectNamedPipe','StartService','ControlService','NCrypt','ImpersonateNamedPipeClient','CreateDirectory','CreateService']:assert forbidden not in core
configs=[]
for b in ['Selected','Legacy']:
 for c in ['Debug','Release']:
  t=f'ASA-{b}-{c}';build=(BASE/(t+'-build.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'warning [A-Z]\d+|error [A-Z]\d+|FAILED:',build,re.I)
  assert '0 tests failed out of 11' in (BASE/(t+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(BASE/(t+'-LastTest.log')).read_text(encoding='utf-8-sig');assert 'cases=33 checks=549 source_bytes=768 policy_bytes=832 original_bytes=720' in last
  assert 'SCQ_CONFIRMATION_CONSOLE cases=7 MODELED_CONSOLE_APIS' in last
  bins=[]
  for n in ['papacc_server.exe','papacc_scq_admin_source_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_scq_dispatch_test.exe']:
   f=ROOT/'build/phase6-functional-20261028'/b/c/n;assert f.exists();bins.append(dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)))
  configs.append(dict(backend=b,configuration=c,build='PASS_ZERO_WARNINGS',ctests=11,source_cases=33,source_checks=549,console_cases=7,binaries=bins,layout=(BASE/(t+'-layout.log')).read_text(encoding='utf-8-sig').strip()))
(BASE/'ASA-SOURCE-HASHES.json').write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
q=dict(task='PACC-PHASE6-SCQ-ATTRIBUTED-ADMIN-AUTHORITY',HEAD=head,source_integration='CONNECTED_OFFLINE_CONTROLLER_TO_EXISTING_RECEIVER',native_operational_registration='EMPTY_DENY_NO_ARGV_FILE_IPC_SELECTOR',positive_decision_and_process_file_token_console_APIs='MODELED',real_helpers='SID_SD_FOREIGN_THREAD_AND_REGRESSION_CNG_HASHING',native_bilateral_authentication='NOT_DEMONSTRATED',native_operational_approval='NOT_CREATED',endpoint='NOT_CREATED_NOT_CONNECTED_NOT_TESTED',SCM_CNG_session='NOT_EXECUTED',configs=configs,ctest_executions=44,embedded_source_bytes=768,original_view_bytes=720,registration_backing_bytes=832,persistent_growth_bytes=768,registration_backing_in_native='ABSENT',new_install_confirmation_helpers='RECONSTRUCTED_SHA_EQUALS_OAS',retained_key='NOT_OPENED_RECEIPT_UNCHANGED',restart='OLD_CONTEXT_REJECTED_IN_MODEL_NOT_GLOBAL_ROLLBACK_PROOF',close_unknown='CONSUMED_BORROW_AND_STATIC_FIXTURE_BACKING_RETAINED_NO_RETRY',operational_request='NOT_READY_NOT_EMITTED',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY')
(BASE/'ASA-QUALIFICATION.json').write_text(json.dumps(q,indent=2)+'\n',encoding='utf-8')
files=list(BASE.glob('ASA-*'))+[BASE/'NEXT-ASA-SCQ-SESSION-DRIVER-OFFLINE.md']
(BASE/'ASA-ARTIFACT-HASHES.json').write_text(json.dumps([dict(path=str(f.relative_to(ROOT)).replace(chr(92),'/'),sha256=sha(f)) for f in sorted(files) if f.is_file() and f.name!='ASA-ARTIFACT-HASHES.json'],indent=2)+'\n',encoding='utf-8')
print('PASS ASA: 4 builds / 44 CTests / 549 source checks per configuration; producer connected; operational registry EMPTY/DENY; historical pins unchanged')
