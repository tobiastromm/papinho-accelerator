"""Reconcile local offline evidence; never invoke product/protected operations."""
from pathlib import Path
import hashlib,json,difflib,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def put(name,value): (here/name).write_text(json.dumps(value,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
baseline=json.loads((here/'BASELINE.json').read_text(encoding='utf-8-sig'))
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==baseline['head']
previous=here.parent/'phase6-published-ack-v2-20261029'
prior=json.loads((previous/'SOURCE-CHANGES.json').read_text())
unchanged={}
for row in prior:
 if row['path'].startswith('src/issuer/issuer_core.') or row['path'].startswith('src/issuer/issuer_codec.') or row['path']=='src/issuer/issuer_publication_v2.inc':
  assert sha(repo/row['path'])==row['after'],row['path']
  unchanged[row['path']]=row['after']
acceptance=json.loads((previous/'OWNER-ACCEPTANCE.json').read_text())
assert sha(repo/acceptance['decision'])==acceptance['decision_sha256']
assert sha(repo/acceptance['proposal'])==acceptance['proposal_sha256']
native=json.loads((here/'NATIVE-RESULTS.json').read_text(encoding='utf-8-sig'))
assert len(native)==8 and all(x['exit']==0 for x in native)
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=f'{profile}-{config}';folder=repo/f'build/phase6-functional-20261028/{profile}/{config}'
  build=(here/f'{tag}-build.log').read_text(encoding='utf-8-sig')
  tests=(here/f'{tag}-ctest.log').read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 13' in tests
  last=(folder/'Testing/Temporary/LastTest.log').read_text(encoding='utf-8-sig')
  assert 'PASS 1354 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last
  assert 'PASS NORMAL_SOURCE_COMPOSITION_MODEL' in last and 'native_private_operations=NOT_RUN backend=808' in last
  assert 'CA_BINDING_NATIVE_NEGATIVE' in last and 'CA_BINDING_MODEL' in last
  assert 'portable_issuer_checks=279' in last
  for n in native:
   if n['profile']==profile and n['config']==config:
    content=(here/n['log']).read_text();assert 'PASS NATIVE_PORT=' in content and 'FAIL' not in content
    assert Path(n['path']).is_file()
  runs.append(dict(profile=profile,config=config,build='PASS',warnings=0,ctests=13,
   cng_model_checks=1354,core_checks=279,native_fixture_runs=2,
   layout=(here/f'{tag}-layout.log').read_text().strip(),
   artifacts={name:sha(folder/name) for name in ('papacc_server.exe','papacc_canonical_root_transaction_test.exe','papacc_issuer_cng_x509_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe')}))
sources=[x['path'] for x in baseline['files']]+[
 'apps/server/server_configuration_root_transaction_win32.inc','apps/server/server_launcher_owner_win32.h',
 'tests/canonical_root_transaction_test.c','tests/issuer_normal_source_composition_cases.inc']
changes=[];delta=[]
for name in dict.fromkeys(sources):
 path=repo/name;pre=here/'preimages'/path.name
 before=pre.read_text(encoding='utf-8-sig').splitlines(True) if pre.exists() else []
 after=path.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre) if pre.exists() else 'ABSENT',after=sha(path),changed=before!=after))
 delta.extend(difflib.unified_diff(before,after,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=52,native_fixture_runs=8,
 normal_source_composition='IMPLEMENTED_MODEL_POSITIVE_NATIVE_NEGATIVE',
 original_product_ca_instance='NOT_ACQUIRED',original_ca_certificate_provenance='NOT_DEMONSTRATED',
 ca_profile_helper='PUBLIC_FIELDS_ONLY_NOT_SELF_SIGNATURE_OR_AUTHORITY',
 source_lifetime='SAME_BACKEND_LATCH_72_BYTES_CONTINUOUS_KEY_BORROW_NOT_DURABLE_RECOVERY',
 original_normal_proof_producer='NATIVE_FACTS_INCOMPLETE_NO_FLAGS_MANUFACTURED',
 m7='NATIVE_HISTORY_EMPTY_COHORT; REAL_AUTHENTICATED_DEPENDENTS_NOT_DEMONSTRATED',
 e8='SOURCE_ABSENT_DENY',enrollment='DATA_PATH_PRESERVED_L2_PERSISTENCE_PENDING',
 d4_d5='INCOMPLETE_DENY',backend_bytes=808,workspace_test_bytes=4096,
 unchanged_issuer_and_published_sources=unchanged,
 protected_operations='NONE',private_key_creation='NONE',native_private_signatures='NONE',
 normal_product='DENY',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY'))
put('FIXTURE-HASHES.json',[dict(path=x['path'],sha256=sha(Path(x['path']))) for x in native])
put('ARTIFACT-HASHES.json',[dict(file=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p))
 for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS: four builds, 52 CTests, 1354 CNG model checks/config, 8 native fixtures; native original source absent; DENY preserved')
