"""Derived readiness evidence only; never runs an operational session."""
from pathlib import Path
import hashlib,json,re,subprocess
here=Path(__file__).resolve().parent;repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
old={r['path']:r['sha256'] for r in json.loads((here/'BDI-SOURCE-HASHES.json').read_text(encoding='utf-8-sig'))}
changed={'apps/server/personal_scq_dispatch_win32.h','apps/server/server_configuration_personal_native_win32.inc','tests/personal_ca_binding_win32_test.c'}
for n,digest in old.items():
 if n not in changed:assert sha(repo/n)==digest,n
native=(repo/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8-sig')
assert 'return PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE;' in native
assert 'gate_reason=personal_scq_native_source_status(p)' in native
dispatch=native[native.index('PAPACC_RESULT papacc_configuration_personal_scq_dispatch('):native.index('PAPACC_RESULT papacc_configuration_personal_runtime_cng_probe(')]
assert 'personal_native_ceremony_ready' not in dispatch
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text(encoding='utf-8-sig')
assert 'static int ps_binding_ready(void){return 0;}' in (repo/'apps/server/server_service_win32.c').read_text(encoding='utf-8-sig')
test=(repo/'tests/personal_ca_binding_win32_test.c').read_text(encoding='utf-8-sig')
assert test.count('gate_reason==PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE')>=2
assert 'provider_calls' in test
receipt=here.parent/'phase6-persistent-ca-fixture-20261029'
observation=json.loads((receipt/'PERSISTENT-OBSERVATION.json').read_text(encoding='utf-8-sig'))
assert sha(receipt/'ONE-ATTEMPT.receipt.txt')==observation['receipt_sha256']
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag='OSR-'+profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig');tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig');layout=(here/(tag+'-layout.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 24' in tests
  for marker in ('CA_BINDING_NATIVE_NEGATIVE','CA_BINDING_MODEL','INSTALL_PATHS checks=39','SCQ_DISPATCH checks=59','CNG_PROBE checks=95','PERSONAL_RUNTIME_IDENTITY checks=91','PRESENTER_ISOLATED checks=38','NATIVE_EPHEMERAL_COMPOSITION checks=89','PERSISTENT_KEYS=NONE'):
   assert marker in last,(tag,marker)
  managed,limit=map(int,re.search(r'KNOWN_MANAGED=(\d+) LIMIT=(\d+)',layout).groups());assert managed<limit
  assert 'NATIVE_ADMISSION=DENY' in layout and 'PROVIDER_KERNEL=UNKNOWN' in layout
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=24,known_managed=managed,limit=limit,
   binaries={n:sha(folder/n) for n in ('papacc_server.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe','papacc_personal_scq_dispatch_test.exe')}))
proposal=here/'OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md'
put('OSR-QUALIFICATION.json',dict(task='PACC-PHASE6-PERSONAL-INSTALL-ONE-SCQ-READINESS',head=head,runs=runs,ctest_runs=96,
 first_dependency='SCQ_ORIGINAL_ADMIN_TO_RUNTIME_AUTHORITY_HANDOFF_CONTRACT',gate_reason='NATIVE_SOURCE_UNAVAILABLE',gate_reason_value=1,
 operation_request='NOT_CREATED_NOT_EXECUTABLE',policy_candidate='PROPOSED_NOT_ACCEPTED',policy_candidate_sha256=sha(proposal),
 normative_paths_identity='ALREADY_ACCEPTED_NOT_REOPENED',parent='NOT_ACQUIRED; DO_NOT_ASSUME_ABSENT',
 parent_creation='OWN_APPROVAL_IF_REQUIRED_NOT_IMPLICIT_IN_SCQ',operator='NO_FRESH_SESSION_BINDING; NO_HISTORY_REUSE',
 consumption='ASSOCIATION_REPLAY_GUARD_IS_NOT_OPERATIONAL_APPROVAL_CONSUMPTION',probe='MODEL_ONLY; NO_NATIVE_MACHINE_KEY_OR_SCM',
 retained_key='NOT_ACCESSED_RECEIPT_UNCHANGED',new_fixed_storage_bytes=0,scope='FIRST_INSTALL_ONLY_NO_OTHER_WORKSTREAM',authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
put('OSR-SOURCE-HASHES.json',[dict(path=n,before_sha256=old[n],sha256=sha(repo/n),changed=sha(repo/n)!=old[n]) for n in old])
put('OSR-ARTIFACT-HASHES.json',[dict(path=p.name,bytes=p.stat().st_size,sha256=sha(p)) for p in sorted(here.iterdir())
 if p.is_file() and (p.name.startswith('OSR-') or p.name=='NEXT-OSR-SCQ-HANDOFF-CONTRACT.md') and p.name!='OSR-ARTIFACT-HASHES.json'])
print('PASS purpose-specific source-absence diagnosis; 4 builds zero warnings; 96 CTests; native calls denied; operational request NOT_CREATED')
