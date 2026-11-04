"""Derived offline evidence only; never performs CNG, SCM or filesystem effects outside repository."""
from pathlib import Path
import hashlib,json,re,subprocess
here=Path(__file__).resolve().parent;repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,value):(here/n).write_text(json.dumps(value,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
candidate=here/'BINARY-DESTINATION-NEW-INSTALL.CANDIDATE.md'
assert sha(candidate)=='e6d5ab6d57486dbe15d548e68e35dea4fd9abb124852ae36f9143378e9335d4d'
assert sha(repo/'docs/adr/ADR-CANDIDATE-personal-scm-virtual-account-20261029.md')=='09c5ee8ccf4bfe1d5e3025e5eb0f7ce79318735494154ee8a2e4330f641bdd93'
assert sha(repo/'docs/adr/DECISION-personal-scm-virtual-account-20261029.md')=='4d0851f69fb2749c7c14a84461bdb04c74adf1542d7d08dcf7b0bb61bb0f9375'
old_sources={r['path']:r['sha256'] for r in json.loads((here/'SOURCE-HASHES.json').read_text(encoding='utf-8-sig'))}
for name,digest in old_sources.items():
 if name.startswith('docs/adr/') or name in ('apps/server/server_ca_key_win32.c','apps/server/server_personal_token_checks_win32.inc'):
  assert sha(repo/name)==digest,name
assert 'static int ps_binding_ready(void){return 0;}' in (repo/'apps/server/server_service_win32.c').read_text(encoding='utf-8-sig')
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text(encoding='utf-8-sig')
probe=(repo/'apps/server/personal_cng_probe_win32.inc').read_text(encoding='utf-8-sig')
for banned in ('NCryptCreate','NCryptFinalize','NCryptSignHash','NCryptExportKey','NCryptDeleteKey','CreateService','StartService','R0R9'):
 assert banned not in probe,banned
old=here.parent/'phase6-persistent-ca-fixture-20261029'
observed=json.loads((old/'PERSISTENT-OBSERVATION.json').read_text(encoding='utf-8-sig'))
assert sha(old/'ONE-ATTEMPT.receipt.txt')==observed['receipt_sha256']
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag='SCQ-'+profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig')
  tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  layout=(here/(tag+'-layout.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 22' in tests
  assert 'CNG_PROBE checks=95' in last and 'NO_REAL_CNG NO_PRIVATE_OPERATIONS bytes=304' in last
  assert 'PERSONAL_RUNTIME_IDENTITY checks=91' in last
  assert 'ENROLLMENT_ISSUER_MODEL checks=77' in last
  assert 'NATIVE_EPHEMERAL_COMPOSITION checks=89' in last and 'PERSISTENT_KEYS=NONE' in last
  assert 'NATIVE_ADMISSION=DENY' in layout
  managed,limit=map(int,re.search(r'KNOWN_MANAGED=(\d+) LIMIT=(\d+)',layout).groups());assert managed<limit
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=22,
   cng_probe_checks=95,identity_checks=91,new_fixed_bytes=304,known_managed=managed,limit=limit,
   binaries={n:sha(folder/n) for n in ('papacc_server.exe','papacc_personal_cng_probe_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_runtime_identity_test.exe')}))
put('SCQ-QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=88,
 candidate_status='PROPOSED_NOT_ACCEPTED',application_probe_entry='DENY_NOT_SUPPORTED_BEFORE_KSP',
 probe_APIs='MODELED',identity_helpers='WINDOWS_REAL_SID_HELPERS',
 prior_crypto_regression='REAL_EPHEMERAL_CNG_ONLY',MACHINE_native='NOT_EXECUTED',
 native_SCM='NOT_EXECUTED',retained_key='NOT_ACCESSED_RECEIPT_UNCHANGED',
 operational_request='NOT_CREATED; ORIGINAL_AUTHORITY_DISPATCH_AND_BINDINGS_REQUIRED',
 authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
names=list(dict.fromkeys(list(old_sources)+[
 'apps/server/personal_cng_probe_win32.h','apps/server/personal_cng_probe_win32.inc',
 'tests/personal_cng_probe_test.c','apps/server/server_service_win32.c',
 'docs/adr/DECISION-personal-windows-optional-scm-20261022.md']))
put('SCQ-SOURCE-HASHES.json',[dict(path=n,before_sha256=old_sources.get(n,'NOT_IN_PREVIOUS_SOURCE_MANIFEST'),
 sha256=sha(repo/n),bytes=(repo/n).stat().st_size) for n in names])
artifacts=[p for p in here.iterdir() if p.is_file() and (p.name.startswith('SCQ-') or p.name in ('BINARY-DESTINATION-NEW-INSTALL.CANDIDATE.md','NEXT-SCQ-DECISION-AND-NATIVE-HANDOFF.md')) and p.name!='SCQ-ARTIFACT-HASHES.json']
put('SCQ-ARTIFACT-HASHES.json',[dict(path=p.name,bytes=p.stat().st_size,sha256=sha(p)) for p in sorted(artifacts)])
print('PASS four builds zero warnings; 88 CTests; 95 probe checks/config; native gate DENY; proposal PROPOSED; historical key receipt preserved')
