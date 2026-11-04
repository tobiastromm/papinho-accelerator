"""Offline evidence reconciliation; never calls a product/operational path."""
from pathlib import Path
import hashlib,json,difflib,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def put(name,value): (here/name).write_text(json.dumps(value,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
baseline=json.loads((here/'BASELINE.json').read_text(encoding='utf-8-sig'))
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==baseline['head']
native=json.loads((here/'FOCUSED-NATIVE-RESULTS.json').read_text(encoding='utf-8-sig'))
assert len(native)==32 and all(x['exit']==0 for x in native)
base=(repo/'build/phase6-normal-ap-confirmation-20261029/final').resolve()
for row in native:
    path=Path(row['path']).resolve()
    assert path.is_relative_to(base) and path.exists() != (row['mode']=='codec-only')
    assert 'PASS' in (here/row['log']).read_text(encoding='utf-8-sig')
runs=[]
for profile in ('Selected','Legacy'):
    for config in ('Debug','Release'):
        tag=f'{profile}-{config}'
        build=(here/f'{tag}-qualified-build.log').read_text(encoding='utf-8-sig')
        tests=(here/f'{tag}-qualified-tests.log').read_text(encoding='utf-8-sig')
        assert '100% tests passed, 0 tests failed out of 10' in tests
        assert not re.search(r'(?im)^.*(?:warning C\d+|warning:|LNK2019|error C\d+).*$',build)
        folder=repo/f'build/phase6-functional-20261028/{profile}/{config}'
        issuer=subprocess.check_output([str(folder/'papacc_issuer_offline_test.exe')],cwd=repo,text=True).strip()
        assert 'portable_issuer_checks=279' in issuer
        artifacts={x:sha(folder/x) for x in ('papacc_server.exe','papacc_canonical_root_transaction_test.exe','papacc_issuer_offline_test.exe','papacc_issuer_cng_x509_test.exe')}
        runs.append(dict(profile=profile,config=config,build='PASS',warnings=0,ctests=10,result='PASS',issuer=issuer,
         layout=(here/f'{tag}-layout.log').read_text(encoding='utf-8-sig').strip(),artifacts=artifacts))
sources=[x['path'] for x in baseline['files']]+[
 'tests/canonical_root_transaction_test.c','tests/issuer_root_store_win32_test.c',
 'docs/integration/phase6-original-scope-recovery-20261028/build_matrix.py',
 'docs/integration/phase6-original-scope-recovery-20261028/REQUIREMENTS.csv',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',
 'docs/capabilities/canonical-phase6-consolidation.md']
changes=[];delta=[]
for name in sources:
    path=repo/name;before=here/'preimages'/path.name
    assert before.is_file(),name
    changes.append(dict(path=name,before=sha(before),after=sha(path)))
    delta.extend(difflib.unified_diff(before.read_text(encoding='utf-8-sig').splitlines(True),path.read_text(encoding='utf-8-sig').splitlines(True),fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=40,focused_runs=32,focused_result='PASS',
 ap_consumer='CORRECTED_EXPLICIT_A_AND_P_PRIVATE_ONLY',persisted_formats='UNCHANGED',
 normative_proposal=dict(file='PUBLISHED-INTENT-AND-ACK.CANDIDATE.md',status='PROPOSED',sha256=sha(here/'PUBLISHED-INTENT-AND-ACK.CANDIDATE.md')),
 original_ca_custody='NOT_AVAILABLE_FOR_THIS_COMPOSITION',normal_full='NOT_DEMONSTRATED',
 published_native='DENY_ACK_REPRESENTATION_PENDING',m7='AP_SUBSTITUTION_REJECTED_MODEL_NATIVE_POSITIVE_NOT_DEMONSTRATED',
 operational_effects='NONE',authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
put('ARTIFACT-HASHES.json',[dict(file=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p))
 for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS evidence: four builds, 40 CTests, 32 focused runs, 279 core model checks/config; formats unchanged')
