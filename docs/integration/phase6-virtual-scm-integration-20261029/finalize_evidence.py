"""Verify existing offline results and write derived evidence; no operations."""
from pathlib import Path
import hashlib, json, re, subprocess

here = Path(__file__).resolve().parent
repo = here.parents[2]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def put(name, value):
    (here/name).write_text(json.dumps(value, indent=2, ensure_ascii=False)+'\n', encoding='utf-8')

head = subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head == 'f341985924d29870d8ff627ed37c8a0874ad6469'
proposal = repo/'docs/adr/ADR-CANDIDATE-personal-scm-virtual-account-20261029.md'
assert sha(proposal) == '09c5ee8ccf4bfe1d5e3025e5eb0f7ce79318735494154ee8a2e4330f641bdd93'
decision = repo/'docs/adr/DECISION-personal-scm-virtual-account-20261029.md'
assert 'status: accepted' in decision.read_text(encoding='utf-8-sig')
source = (repo/'apps/server/personal_runtime_identity_win32.inc').read_text(encoding='utf-8-sig')
for forbidden in ('CreateService','StartService','ChangeServiceConfig','NCrypt','PapinhoAcceleratorLab','S-1-5-80-'):
    assert forbidden not in source
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text(encoding='utf-8-sig')
previous = here.parent/'phase6-persistent-ca-fixture-20261029'
observed = json.loads((previous/'PERSISTENT-OBSERVATION.json').read_text(encoding='utf-8-sig'))
assert sha(previous/'ONE-ATTEMPT.receipt.txt') == observed['receipt_sha256']
runs=[]
for profile in ('Selected','Legacy'):
    for config in ('Debug','Release'):
        tag=profile+'-'+config
        build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig')
        tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
        last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
        layout=(here/(tag+'-layout.log')).read_text(encoding='utf-8-sig')
        assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
        assert '100% tests passed, 0 tests failed out of 21' in tests
        assert 'PERSONAL_RUNTIME_IDENTITY checks=91' in last
        assert 'REAL_OTHER_THREAD MODELED_PROCESS_TOKEN_LOOKUP_SCM' in last
        assert 'ENROLLMENT_ISSUER_MODEL checks=77' in last
        assert 'NATIVE_EPHEMERAL_COMPOSITION checks=89' in last
        assert 'NATIVE_ADMISSION=DENY' in layout and 'PROVIDER_KERNEL=UNKNOWN' in layout
        managed,limit=map(int,re.search(r'KNOWN_MANAGED=(\d+) LIMIT=(\d+)',layout).groups())
        assert managed<limit
        folder=repo/'build/phase6-functional-20261028'/profile/config
        runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=21,
            runtime_identity_checks=91,identity_substate_bytes=216,known_managed=managed,limit=limit,
            binaries={name:sha(folder/name) for name in ('papacc_server.exe','papacc_personal_runtime_identity_test.exe','papacc_personal_ca_native_test.exe')}))
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=84,authority='DENY',
    native_runtime_token='NOT_OBSERVED',native_SCM='NOT_EXECUTED',native_CNG_runtime_access='NOT_DEMONSTRATED',
    identity_tests='REAL_SID_HELPERS_AND_CONCURRENT_THREAD; MODELED_PROCESS_TOKEN_SCM',
    CNG='PRIOR_EPHEMERAL_FIXTURE_REAL; KEY_RUNTIME_DENIAL_GUARD_MODELED_TOKEN',
    persistent_fixture_receipt_unchanged=True,ED11='PARTIAL',Phase6='PARTIAL'))
names=[
 'CMakeLists.txt','apps/server/personal_native_win32.h','apps/server/personal_runtime_identity_win32.h',
 'apps/server/personal_runtime_identity_win32.inc','apps/server/server_configuration_commit_win32.c',
 'apps/server/server_configuration_commit_win32.h','apps/server/server_configuration_personal_native_win32.inc',
 'apps/server/server_configuration_personal_data_win32.inc','apps/server/server_configuration_personal_ca_win32.inc',
 'tests/personal_runtime_identity_test.c','tests/personal_ca_binding_win32_test.c',
 'apps/server/server_personal_token_checks_win32.inc','apps/server/server_ca_key_win32.c',
 'docs/adr/ADR-CANDIDATE-personal-scm-virtual-account-20261029.md',
 'docs/adr/DECISION-personal-scm-virtual-account-20261029.md',
 'docs/adr/DECISION-personal-ca-preinitialization-20261022.md',
 'docs/adr/DECISION-phase6-personal-mvp-20261022.md',
 'docs/adr/DECISION-personal-first-install-authorization-f0-f9-20261022.md']
put('SOURCE-HASHES.json',[dict(path=n,bytes=(repo/n).stat().st_size,sha256=sha(repo/n)) for n in names])
put('ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p))
    for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS: accepted hash; four builds; 84 CTests; 91 identity checks/configuration; retained receipt unchanged; authority DENY')
