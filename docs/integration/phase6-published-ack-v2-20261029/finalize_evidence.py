"""Local evidence only; no product or protected operations."""
from pathlib import Path
import hashlib,json,difflib,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def put(name,value): (here/name).write_text(json.dumps(value,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text(encoding='utf-8-sig'))
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==base['head']
approval=json.loads((here/'OWNER-ACCEPTANCE.json').read_text())
assert approval['proposal_sha256']==sha(repo/approval['proposal'])=='3fcce389e58299aaa161779b9c7976f00f8e79d76dc5622d40d60b091e34f684'
assert approval['decision_sha256']==sha(repo/approval['decision'])
focused=json.loads((here/'FOCUSED-NATIVE-RESULTS.json').read_text(encoding='utf-8-sig'))
assert len(focused)==112 and all(r['exit']==0 for r in focused)
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=f'{profile}-{config}'
  build=(here/f'{tag}-build.log').read_text(encoding='utf-8-sig')
  tests=(here/f'{tag}-ctest.log').read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019)',build)
  assert '100% tests passed, 0 tests failed out of 13' in tests
  selected=[r for r in focused if r['profile']==profile and r['config']==config]
  assert len(selected)==28
  folder=repo/f'build/phase6-functional-20261028/{profile}/{config}'
  last=(folder/'Testing/Temporary/LastTest.log').read_text(encoding='utf-8-sig')
  assert 'portable_issuer_checks=279' in last and 'PASS 614 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last
  assert 'CA_BINDING_NATIVE_NEGATIVE' in last and 'CA_BINDING_MODEL' in last
  assert 'codec_checks=142' in (here/f'{tag}-codec-only.log').read_text()
  for r in selected:
   log=(here/r['log']).read_text(encoding='utf-8-sig')
   assert 'PASS' in log and 'FAIL' not in log
   if 'codec-only' not in r['mode']: assert Path(r['path']).is_file()
   else: assert not Path(r['path']).exists()
  runs.append(dict(profile=profile,config=config,build='PASS',warnings=0,ctests=13,focused_runs=28,
   codec_checks=142,core_model_checks=279,cng_model_checks=614,
   layout=(here/f'{tag}-layout.log').read_text().strip(),
   artifacts={name:sha(folder/name) for name in ('papacc_server.exe','papacc_canonical_root_transaction_test.exe','papacc_issuer_root_store_test.exe','papacc_issuer_cng_x509_test.exe')}))
sources=[x['path'] for x in base['files']]+[
 'src/issuer/issuer_publication_v2.inc','apps/server/server_issuer_publication_v2_win32.inc',
 'tests/issuer_publication_v2_cases.inc',approval['decision'],
 'docs/capabilities/phase6-scope-and-closure-20261028.md','docs/capabilities/canonical-phase6-consolidation.md',
 'docs/integration/phase6-original-scope-recovery-20261028/build_matrix.py',
 'docs/integration/phase6-original-scope-recovery-20261028/REQUIREMENTS.csv']
changes=[];delta=[]
for name in dict.fromkeys(sources):
 path=repo/name;pre=here/'preimages'/path.name
 before=pre.read_text(encoding='utf-8-sig').splitlines(True) if pre.is_file() else []
 after=path.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre) if pre.is_file() else 'ABSENT',after=sha(path),changed=before!=after))
 delta.extend(difflib.unified_diff(before,after,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=52,focused_runs=112,
 normative_decision='ACCEPTED_BYTE_EXACT',persisted_formats='EXPLICIT_V2_WITH_V1_COMPATIBILITY_NO_MIGRATION',
 native_publication='NEW_EXCLUSIVE_METADATA_FIXTURES_ONLY',native_crash='DELIBERATE_PROCESS_EXIT_AT_DURABLE_BOUNDARIES_NOT_POWER_LOSS',
 native_m7='CONFIRMED_HISTORY_WITH_EMPTY_COHORT_ONLY',cohort_effects='ISOLATED_SYNTHETIC_METADATA',
 original_ca_source='NOT_ACQUIRED',normal_full='DENY',productive_operations='NONE',
 signature='NO_PRODUCTIVE_SIGNATURE; SINGLE_SYNTHETIC_SIGNATURE_MODEL_REUSED',
 provider_kernel='UNKNOWN',authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
put('FIXTURE-HASHES.json',[dict(path=r['path'],mode=r['mode'],sha256=sha(Path(r['path']))) for r in focused if Path(r['path']).is_file()])
put('ARTIFACT-HASHES.json',[dict(file=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p))
 for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS: exact accepted decision, four builds, 52 CTests, 112 focused runs; original NORMAL source absent; DENY preserved')
