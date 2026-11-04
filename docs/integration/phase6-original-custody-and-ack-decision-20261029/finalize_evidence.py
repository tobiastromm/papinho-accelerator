"""Reconcile qualification and hashes; no product or protected operations."""
from pathlib import Path
import hashlib,json,difflib,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()
def put(name,value): (here/name).write_text(json.dumps(value,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text(encoding='utf-8-sig'))
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==base['head']
request=json.loads((here/'OWNER-DECISION-REQUEST.json').read_text(encoding='utf-8-sig'))
assert request['proposal_sha256']==sha(here/request['proposal'])
assert request['status']=='PENDING_OWNER_DECISION' and request['approval_record']=='NOT_CREATED'
runs=[]
for profile in ('Selected','Legacy'):
    for config in ('Debug','Release'):
        tag=f'{profile}-{config}'
        build=(here/f'{tag}-final-build.log').read_text(encoding='utf-8-sig')
        tests=(here/f'{tag}-final-tests.log').read_text(encoding='utf-8-sig')
        assert '100% tests passed, 0 tests failed out of 13' in tests
        assert not re.search(r'(?im)^.*(?:warning C\d+|warning:|LNK2019|error C\d+).*$',build)
        folder=repo/f'build/phase6-functional-20261028/{profile}/{config}'
        last=(folder/'Testing/Temporary/LastTest.log').read_text(encoding='utf-8-sig')
        assert 'PASS 614 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last
        assert 'argument_and_lifetime_checks=15/15' in last
        assert 'CA_BINDING_NATIVE_NEGATIVE' in last and 'CA_BINDING_MODEL' in last
        artifacts={name:sha(folder/name) for name in ('papacc_server.exe','papacc_ca_key_lifetime_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe','papacc_issuer_cng_x509_test.exe')}
        runs.append(dict(profile=profile,config=config,build='PASS',warnings=0,ctests=13,result='PASS',
          ca_lifetime_checks=15,cng_model_checks=614,native_ca_source='NEGATIVE_ONLY_NO_KEY_CREATION',
          layout=(here/f'{tag}-layout.log').read_text(encoding='utf-8-sig').strip(),artifacts=artifacts))
sources=[x['path'] for x in base['files']]+[
 'CMakeLists.txt',
 'docs/integration/phase6-original-scope-recovery-20261028/build_matrix.py',
 'docs/integration/phase6-original-scope-recovery-20261028/REQUIREMENTS.csv',
 'docs/capabilities/phase6-scope-and-closure-20261028.md',
 'docs/capabilities/canonical-phase6-consolidation.md']
changes=[];delta=[]
for name in sources:
    path=repo/name;pre=here/'preimages'/path.name
    assert pre.is_file(),name
    changes.append(dict(path=name,before=sha(pre),after=sha(path)))
    delta.extend(difflib.unified_diff(pre.read_text(encoding='utf-8-sig').splitlines(True),path.read_text(encoding='utf-8-sig').splitlines(True),fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=52,
 custody_transfer='IMPLEMENTED_SOURCE_REVALIDATED_MODELED_POSITIVE_NATIVE_NEGATIVE',
 actual_original_source='NOT_ACQUIRED',normal_full='DENY',
 m7_native_positive='NOT_DEMONSTRATED_NO_LEGITIMATE_PUBLISHED_REVOKED_SOURCE',
 persisted_formats='UNCHANGED',normative_decision=request,
 protected_operations='NONE',authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
put('ARTIFACT-HASHES.json',[dict(file=str(path.relative_to(here)),bytes=path.stat().st_size,sha256=sha(path))
 for path in sorted(here.rglob('*')) if path.is_file() and path.name!='ARTIFACT-HASHES.json'])
print('PASS evidence: four builds, 52 CTests; original source absent; exact normative request remains unapproved')
