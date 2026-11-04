"""Read-only qualification reconciliation; writes derived evidence here only."""
from pathlib import Path
import hashlib, json, difflib, subprocess

here = Path(__file__).resolve().parent
repo = here.parents[2]
def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()
def put(name, value):
    (here/name).write_text(json.dumps(value, indent=2, ensure_ascii=False)+'\n', encoding='utf-8')

sources = [
 'CMakeLists.txt',
 'apps/server/server_launcher_owner_win32.h',
 'apps/server/server_configuration_root_transaction_win32.inc',
 'apps/server/server_configuration_pending_win32.inc',
 'apps/server/server_c2_root_binding_win32.inc',
 'apps/server/server_secure_io_win32.h.api2.inc',
 'apps/server/server_secure_io_win32.h.api3.inc',
 'apps/server/server_secure_io_win32.c.api2.inc',
 'apps/server/server_secure_io_win32.c.api3.inc',
 'tests/canonical_root_transaction_test.c',
 'tests/issuer_root_store_win32_test.c',
 'docs/integration/phase6-original-scope-recovery-20261028/build_matrix.py',
 'docs/integration/phase6-original-scope-recovery-20261028/REQUIREMENTS.csv',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',
 'docs/capabilities/canonical-phase6-consolidation.md',
]
changes=[]
delta=[]
for name in sources:
    current=repo/name
    before=here/'preimages'/current.name
    assert before.is_file(), name
    changes.append(dict(path=name,before=digest(before),after=digest(current),changed=before.read_bytes()!=current.read_bytes()))
    delta.extend(difflib.unified_diff(before.read_text(encoding='utf-8-sig').splitlines(True),current.read_text(encoding='utf-8-sig').splitlines(True),fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
native=json.loads((here/'NATIVE-FIXTURE-RESULTS-integrated.json').read_text(encoding='utf-8-sig'))
assert len(native)==68 and all(x['exit']==0 for x in native)
allowed=(repo/'build/phase6-native-cas-20261029/integrated').resolve()
for x in native:
    path=Path(x['fixture']).resolve()
    assert path.is_relative_to(allowed) and path.is_file(), x
    assert 'PASS' in (here/x['log']).read_text(encoding='utf-8-sig')
runs=[]
for profile in ('Selected','Legacy'):
    for config in ('Debug','Release'):
        tag=f'{profile}-{config}'
        build=(here/f'{tag}-integrated2-build.log').read_text(encoding='utf-8-sig')
        tests=(here/f'{tag}-integrated2-tests.log').read_text(encoding='utf-8-sig')
        assert '100% tests passed, 0 tests failed out of 9' in tests
        assert not any('warning C' in line or 'error C' in line or 'LNK2019' in line for line in build.splitlines())
        artifacts={}
        for exe in ('papacc_server','papacc_canonical_root_transaction_test','papacc_issuer_root_store_test','papacc_issuer_offline_test'):
            artifacts[exe+'.exe']=digest(repo/f'build/phase6-functional-20261028/{profile}/{config}/{exe}.exe')
        runs.append(dict(profile=profile,config=config,build='PASS',warnings=0,ctests=9,result='PASS',artifacts=artifacts,layout=(here/f'{tag}-layout.log').read_text(encoding='utf-8-sig').strip()))
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==json.loads((here/'BASELINE.json').read_text(encoding='utf-8-sig'))['head']
put('QUALIFICATION.json',dict(head=head,runs=runs,native_fixture_runs=68,native_result='PASS',
 bridge='QUALIFIED_OFFLINE_METADATA_ONLY',normal_full_composition='NOT_DEMONSTRATED_SOURCE_GATES_DENY',
 normal_native_producer='NOT_COMPLETE_NO_SYNTHETIC_PROOFS',m7='EXACT_CREDENTIAL_CAPTURE_IMPLEMENTED_FANOUT_INCOMPLETE',
 native_cng_signature='NOT_EXECUTED',api2_runtime='NOT_QUALIFIED_BY_THIS_CAMPAIGN',
 productive_effects='NONE',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY'))
put('ARTIFACT-HASHES.json',[dict(file=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=digest(p)) for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS evidence reconciled: four builds, 36 CTests, 68 native fixtures; NORMAL full remains DENY')
