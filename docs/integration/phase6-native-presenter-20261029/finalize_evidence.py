"""Derived offline evidence; never invokes installation or persistent CNG."""
from pathlib import Path
import json,hashlib,difflib,re,subprocess
here=Path(__file__).resolve().parent;repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text());head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip();assert head==base['head']
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text()
entry=(repo/'apps/server/server_launcher_personal_win32.inc').read_text()
for prohibited in ('papacc_ca_key_create(', 'papacc_configuration_personal_target_begin(', 'NCryptCreatePersistedKey(', 'papacc_configuration_publish('):assert prohibited not in entry
assert 'OWNER_NEW_INSTALL_CA_INITIALIZE ' in entry and 'OPERATIONAL_AUTHORITY=DENY' in entry
native=json.loads((here/'NATIVE-RESULTS.json').read_text(encoding='utf-8-sig'));assert len(native)==8 and all(r['exit']==0 for r in native)
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig');tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig');last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C[0-9]+|warning:|error C[0-9]+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 18' in tests
  assert 'PASS PRESENTER_ISOLATED checks=37 scenarios=13' in last
  assert 'NATIVE_READ_ONLY_PREFLIGHT_DENIED_NO_ELIGIBLE_CONTEXT_OR_PARENT; POSITIVE_NOT_QUALIFIED' in last
  assert 'PASS 2718 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last
  layout=(here/(tag+'-layout.log')).read_text();assert 'NATIVE_ADMISSION=DENY' in layout
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=18,presenter_checks=37,presenter_scenarios=13,native_preflight='DENIED; POSITIVE_NOT_QUALIFIED',layout=layout.strip(),
   binaries={n:sha(folder/n) for n in ('papacc_server.exe','papacc_personal_presenter_test.exe','papacc_canonical_root_transaction_test.exe')}))
# Preserve the prior retained key's public evidence without opening/deleting it.
old=here.parent/'phase6-persistent-ca-fixture-20261029'
observation=json.loads((old/'PERSISTENT-OBSERVATION.json').read_text())
assert sha(old/'ONE-ATTEMPT.receipt.txt')==observation['receipt_sha256']
changes=[];delta=[]
for name in base['files']+['tests/personal_presenter_win32_test.c','tests/fixtures/presenter-settings.conf']:
 p=repo/name;pre=here/'preimages'/p.name;a=pre.read_text(encoding='utf-8-sig').splitlines(True) if pre.exists() else [];b=p.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre) if pre.exists() else 'ABSENT',after=sha(p),changed=a!=b));delta.extend(difflib.unified_diff(a,b,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes);(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=72,native_fixture_runs=8,production_operations='NONE',
 presenter='EXISTING_LAUNCHER_EXTENDED_JOINT_PURPOSE_READONLY',positive_presenter_context='TEST_ONLY; NOT_NATIVE_IDENTITY_PROOF',
 read_only_handoff='EXACT_PLAN_CONSUMED_ONCE; SAME_WORKER; NO_TARGET_CREATE',
 custody_contract='MACHINE_KSP_AND_DISTINCT_OPERATIONAL_IDENTITY; CURRENTUSER_FIXTURE_NOT_EQUIVALENT',
 retained_persistent_fixture='NOT_OPENED_OR_MODIFIED_THIS_TASK; EVIDENCE_HASH_PRESERVED',
 service='NOT_STARTED_OR_RECONFIGURED',E8='SOURCE_ABSENT',enrollment='L2_AND_SOURCES_PENDING',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY'))
put('ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p)) for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS four builds; 72 CTests; 8 native fixtures; presenter offline qualified; real preflight DENY preserved')
