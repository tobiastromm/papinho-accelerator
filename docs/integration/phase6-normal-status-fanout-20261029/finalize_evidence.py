"""Reconcile offline artifacts only; no product/operational execution."""
from pathlib import Path
import hashlib,json,difflib,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()
def put(name,value): (here/name).write_text(json.dumps(value,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
baseline=json.loads((here/'BASELINE.json').read_text(encoding='utf-8-sig'))
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==baseline['head']
native=json.loads((here/'FOCUSED-NATIVE-RESULTS.json').read_text(encoding='utf-8-sig'))
assert len(native)==32 and all(x['exit']==0 for x in native)
base=(repo/'build/phase6-normal-status-fanout-20261029/final').resolve()
for row in native:
    path=Path(row['path']).resolve()
    assert path.is_relative_to(base)
    assert path.exists() != (row['mode']=='codec-only')
    assert 'PASS' in (here/row['log']).read_text(encoding='utf-8-sig')
runs=[]
for profile in ('Selected','Legacy'):
    for config in ('Debug','Release'):
        tag=f'{profile}-{config}'
        build=(here/f'{tag}-final-build.log').read_text(encoding='utf-8-sig')
        tests=(here/f'{tag}-final-tests.log').read_text(encoding='utf-8-sig')
        assert '100% tests passed, 0 tests failed out of 10' in tests
        assert not re.search(r'(?im)^.*(?:warning C\d+|warning:|LNK2019|error C\d+).*$',build)
        folder=repo/f'build/phase6-functional-20261028/{profile}/{config}'
        artifacts={name:sha(folder/name) for name in ('papacc_server.exe','papacc_canonical_root_transaction_test.exe','papacc_issuer_root_store_test.exe','papacc_credential_revocation_test.exe')}
        runs.append(dict(profile=profile,config=config,build='PASS',warnings=0,ctests=10,ctest_result='PASS',
          layout=(here/f'{tag}-layout.log').read_text(encoding='utf-8-sig').strip(),artifacts=artifacts))
sources=[x['path'] for x in baseline['files']]+[
 'apps/server/server_credential_revocation_win32.h',
 'apps/server/server_credential_revocation_win32.inc',
 'tests/credential_revocation_win32_test.c',
 'docs/integration/phase6-original-scope-recovery-20261028/build_matrix.py',
 'docs/integration/phase6-original-scope-recovery-20261028/REQUIREMENTS.csv',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',
 'docs/capabilities/canonical-phase6-consolidation.md']
changes=[];delta=[]
for name in sources:
    path=repo/name;before=here/'preimages'/path.name
    changes.append(dict(path=name,before=sha(before) if before.exists() else None,after=sha(path)))
    old=before.read_text(encoding='utf-8-sig').splitlines(True) if before.exists() else []
    delta.extend(difflib.unified_diff(old,path.read_text(encoding='utf-8-sig').splitlines(True),fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=40,focused_runs=32,focused_result='PASS',
 status_source='IMPLEMENTED_GUARDED_CURRENT',m7='NATIVE_PORT_WIRED_EFFECT_COHORT_QUALIFIED_ISOLATED',
 m7_native_positive='NOT_DEMONSTRATED_ORIGINAL_REVOKED_SOURCE_ABSENT',
 normal_producer='NOT_COMPLETE_ORIGINAL_CA_CUSTODY_HISTORY_WRITER_UNAVAILABLE',
 published='DENY_ACK_REPRESENTATION_INCOMPLETE',normal_full='NOT_DEMONSTRATED',
 operational_effects='NONE',productive_authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
put('ARTIFACT-HASHES.json',[dict(file=str(path.relative_to(here)),bytes=path.stat().st_size,sha256=sha(path))
 for path in sorted(here.rglob('*')) if path.is_file() and path.name!='ARTIFACT-HASHES.json'])
print('PASS evidence: four builds, 40 CTests, 32 focused runs; NORMAL/PUBLISHED remain DENY')
