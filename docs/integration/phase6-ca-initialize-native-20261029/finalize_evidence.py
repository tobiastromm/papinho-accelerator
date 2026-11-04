"""Local offline evidence only. No native product or protected operation."""
from pathlib import Path
import hashlib,json,difflib,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def put(name,value): (here/name).write_text(json.dumps(value,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text(encoding='utf-8-sig'))
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==base['head']
previous=here.parent/'phase6-published-ack-v2-20261029'
unchanged={}
for row in json.loads((previous/'SOURCE-CHANGES.json').read_text()):
 if row['path'].startswith('src/issuer/issuer_core.') or row['path'].startswith('src/issuer/issuer_codec.') or row['path']=='src/issuer/issuer_publication_v2.inc':
  assert sha(repo/row['path'])==row['after'];unchanged[row['path']]=row['after']
# Explicit code authorization; no operational invocation.
assert sha(repo/'docs/integration/phase6-first-and-existing-ca-20261029/NATIVE-ENTRY-DECISION.md')==base['scope_sha256'].lower()
entry=(repo/'apps/server/server_configuration_personal_ca_win32.inc').read_text()
assert entry.count('status=papacc_ca_key_create(')==1
assert 'p->ca_initialize_phase=1U;p->personal_ca_candidate=ca;' in entry
assert 'p->ca_initialize_phase=4U;o->unresolved=1' in entry
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text()
assert '#define papacc_ca_key_create source_create' in (repo/'tests/issuer_normal_source_composition_cases.inc').read_text()
native=json.loads((here/'NATIVE-RESULTS.json').read_text(encoding='utf-8-sig'))
assert len(native)==8 and all(r['exit']==0 for r in native)
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=f'{profile}-{config}';folder=repo/f'build/phase6-functional-20261028/{profile}/{config}'
  build=(here/f'{tag}-build.log').read_text(encoding='utf-8-sig')
  tests=(here/f'{tag}-ctest.log').read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 13' in tests
  last=(folder/'Testing/Temporary/LastTest.log').read_text(encoding='utf-8-sig')
  assert 'PASS 2718 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last
  assert 'CA_INITIALIZE_REAL_ENTRY_TEST_ONLY_KEY_API' in last
  assert 'FIRST_INSTANCE_AND_EXISTING_CA_MODEL' in last and 'NO_NATIVE_KEY_CREATION' in last
  assert 'NORMAL_SOURCE_COMPOSITION_MODEL' in last and 'backend=808' in last
  assert 'CA_BINDING_NATIVE_NEGATIVE' in last and 'portable_issuer_checks=279' in last
  for r in native:
   if r['profile']==profile and r['config']==config:
    log=(here/r['log']).read_text();assert 'PASS NATIVE_PORT=' in log and 'FAIL' not in log
    assert Path(r['path']).is_file()
  runs.append(dict(profile=profile,config=config,build='PASS',warnings=0,ctests=13,cng_model_checks=2718,
   core_model_checks=279,native_fixture_runs=2,layout=(here/f'{tag}-layout.log').read_text().strip(),
   artifacts={n:sha(folder/n) for n in ('papacc_server.exe','papacc_issuer_cng_x509_test.exe','papacc_canonical_root_transaction_test.exe','papacc_personal_ca_native_test.exe')}))
changes=[];delta=[]
sources=[x['path'] for x in base['files']]
for name in dict.fromkeys(sources):
 p=repo/name;pre=here/'preimages'/p.name
 a=pre.read_text(encoding='utf-8-sig').splitlines(True) if pre.exists() else []
 b=p.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre) if pre.exists() else 'ABSENT',after=sha(p),changed=a!=b))
 delta.extend(difflib.unified_diff(a,b,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=52,native_fixture_runs=8,
 first_instance='MODEL_POSITIVE; UNSIGNED_PUBLIC_TBS_NATIVE_ENCODING; NO_PREVIOUS_CA_REQUIRED',
 existing_ca='BORROW_ONLY_BIND_AND_ACTIVATE_IMPLEMENTED; MODEL_RESTART_POSITIVE_NO_RECREATION',
 native_ca_creation='CODE_INTEGRATED; API_SUBSTITUTED_ONLY_IN_TESTS; REAL_NOT_EXECUTED',
 technical_scope='OWNER_EXPLICIT_CODE_AUTHORIZATION; NOT_OPERATIONAL',
 native_ceremony_authority='DATA_ONLY_DENY_UNCHANGED',native_normal_proofs='INCOMPLETE_DENY',
 native_cryptographic_end_to_end='NOT_DEMONSTRATED',native_private_operations='NONE',
 native_m7='CONFIRMED_HISTORY_WITH_EMPTY_COHORT_ONLY',e8='SOURCE_ABSENT',enrollment='L2_PENDING',
 unchanged_core_and_v2_sources=unchanged,backend_bytes=808,provider_kernel='UNKNOWN',
 productive_operations='NONE',NORMAL='DENY',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY'))
put('FIXTURE-HASHES.json',[dict(path=r['path'],sha256=sha(Path(r['path']))) for r in native])
put('ARTIFACT-HASHES.json',[dict(file=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p))
 for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS: four builds, 52 CTests, 2718 model checks/config, 8 native fixtures; CA_INITIALIZE code integrated; operational DENY')
