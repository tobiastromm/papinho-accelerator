"""Verify already-run offline results. Never executes native operations/tests."""
from pathlib import Path
import hashlib,json,re,subprocess
here=Path(__file__).resolve().parent;repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
proposal=here/'BINARY-DESTINATION-NEW-INSTALL.CANDIDATE.md'
decision=repo/'docs/adr/DECISION-personal-binary-destination-new-install-20261029.md'
assert sha(proposal)=='e6d5ab6d57486dbe15d548e68e35dea4fd9abb124852ae36f9143378e9335d4d'
assert sha(decision)=='4f8485983a6ad6cd2d8b0527fea1df37a7c522686333b0c45e8eb01446b443f3'
assert 'status: accepted' in decision.read_text(encoding='utf-8-sig')
old={r['path']:r['sha256'] for r in json.loads((here/'SCQ-SOURCE-HASHES.json').read_text(encoding='utf-8-sig'))}
for name,digest in old.items():
 if name.startswith('docs/adr/') or name in ('apps/server/server_service_win32.c','apps/server/server_ca_key_win32.c','apps/server/server_personal_token_checks_win32.inc'):
  assert sha(repo/name)==digest,name
assert 'static int ps_binding_ready(void){return 0;}' in (repo/'apps/server/server_service_win32.c').read_text(encoding='utf-8-sig')
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text(encoding='utf-8-sig')
paths=(repo/'apps/server/personal_install_paths_win32.inc').read_text(encoding='utf-8-sig')
dispatch=(repo/'apps/server/personal_scq_dispatch_win32.inc').read_text(encoding='utf-8-sig')
for forbidden in ('CreateDirectory','CreateFile','CreateService','StartService','NCryptCreate','NCryptSignHash','NCryptExportKey'):
 assert forbidden not in paths and forbidden not in dispatch,forbidden
assert 'KF_FLAG_DONT_VERIFY' in paths and 'KF_FLAG_CREATE' not in paths
old_key=here.parent/'phase6-persistent-ca-fixture-20261029'
obs=json.loads((old_key/'PERSISTENT-OBSERVATION.json').read_text(encoding='utf-8-sig'))
assert sha(old_key/'ONE-ATTEMPT.receipt.txt')==obs['receipt_sha256']
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag='BDI-'+profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig');tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig');layout=(here/(tag+'-layout.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 24' in tests
  for marker in ('INSTALL_PATHS checks=39','SCQ_DISPATCH checks=59','CNG_PROBE checks=95','PERSONAL_RUNTIME_IDENTITY checks=91',
                 'PRESENTER_ISOLATED checks=38','ENROLLMENT_ISSUER_MODEL checks=77','NATIVE_EPHEMERAL_COMPOSITION checks=89','PERSISTENT_KEYS=NONE'):
   assert marker in last,(tag,marker)
  assert 'NATIVE_ADMISSION=DENY' in layout and 'PROVIDER_KERNEL=UNKNOWN' in layout
  managed,limit=map(int,re.search(r'KNOWN_MANAGED=(\d+) LIMIT=(\d+)',layout).groups());assert managed<limit
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=24,path_checks=39,dispatch_checks=59,
   probe_checks=95,identity_checks=91,presenter_checks=38,known_managed=managed,limit=limit,
   binaries={n:sha(folder/n) for n in ('papacc_server.exe','papacc_personal_paths_test.exe','papacc_personal_scq_dispatch_test.exe','papacc_personal_presenter_test.exe','papacc_personal_ca_native_test.exe')}))
put('BDI-QUALIFICATION.json',dict(head=head,decision='ACCEPTED_NEW_INSTALL_ONLY',proposal_sha256=sha(proposal),decision_sha256=sha(decision),
 runs=runs,ctest_runs=96,paths_bytes=1708,dispatch_bytes=344,new_root_bytes_with_alignment=2056,
 installation='NOT_EXECUTED',directory_creation='NONE_FOR_INSTALLATION',SCM_native='NOT_EXECUTED',CNG_MACHINE='NOT_EXECUTED',
 path_tests='MODELED_KNOWN_FOLDER_APIS; EXISTING_PATHS_PRESERVED',presenter='KNOWN_FOLDER_READONLY; TEST_CONSOLE_AND_TOKEN',
 dispatch='SOURCE_LINKED_BODY_MODELED_NATIVE_APIS; ROOT_ADMISSION_DENIED',retained_key='NOT_ACCESSED_RECEIPT_UNCHANGED',
 first_operation='ONE_SCM_CNG_SESSION; SCOPE_ONLY_PENDING_ORIGINAL_AUTHORITY_AND_BINDINGS',
 post_test_source_edit='EOF_WHITESPACE_NORMALIZATION_ONLY; NO_FUNCTIONAL_CHANGE_OR_CAMPAIGN_REPEAT',authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
names=list(dict.fromkeys(list(old)+[
 'apps/server/personal_install_paths_win32.h','apps/server/personal_install_paths_win32.inc',
 'apps/server/personal_scq_dispatch_win32.h','apps/server/personal_scq_dispatch_win32.inc',
 'apps/server/server_launcher_personal_win32.inc','tests/personal_install_paths_test.c','tests/personal_scq_dispatch_test.c',
 'tests/personal_presenter_win32_test.c','tests/personal_cng_probe_test.c',
 'docs/adr/DECISION-personal-binary-destination-new-install-20261029.md']))
put('BDI-SOURCE-HASHES.json',[dict(path=n,before_sha256=old.get(n,'NOT_RECORDED_IN_PREVIOUS_MANIFEST'),sha256=sha(repo/n),bytes=(repo/n).stat().st_size) for n in names])
put('BDI-ARTIFACT-HASHES.json',[dict(path=p.name,bytes=p.stat().st_size,sha256=sha(p)) for p in sorted(here.iterdir())
 if p.is_file() and (p.name.startswith('BDI-') or p.name=='NEXT-BDI-ONE-SCQ-REQUEST.md') and p.name!='BDI-ARTIFACT-HASHES.json'])
print('PASS accepted exact destination; 4 builds zero warnings; 96 CTests; paths39/dispatch59/probe95; app DENY; retained receipt unchanged')
