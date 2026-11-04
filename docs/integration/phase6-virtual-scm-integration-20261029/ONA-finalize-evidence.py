"""Evidence only; never opens source files or executes a protected session."""
from pathlib import Path
import hashlib,json,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
assert sha(here/'OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md')=='34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561'
assert sha(repo/'docs/adr/DECISION-personal-scq-original-handoff-20261030.md')=='63d43b212f6ef4d44a8200e98d08e7f99a40efbd6c42443c5e4bac3d2f4d44c5'
before={r['path']:r['sha256'] for r in json.loads((here/'OHI-SOURCE-HASHES.json').read_text(encoding='utf-8-sig'))}
changed={'CMakeLists.txt','apps/server/personal_native_win32.h','apps/server/personal_scq_dispatch_win32.h',
 'apps/server/server_configuration_commit_win32.c','apps/server/server_configuration_commit_win32.h',
 'apps/server/server_configuration_personal_data_win32.inc','apps/server/server_configuration_personal_native_win32.inc',
 'tests/personal_ca_binding_win32_test.c'}
for n,digest in before.items():
 if n not in changed:assert sha(repo/n)==digest,n
new={'apps/server/personal_scq_inputs_win32.h','apps/server/personal_scq_inputs_win32.inc',
 'tests/personal_scq_inputs_test.c','tests/scq_transport_scope_test.c'}
inputs=(repo/'apps/server/personal_scq_inputs_win32.inc').read_text(encoding='utf-8-sig')
for forbidden in ('CreateFile','ReadFile','WriteConsole','SetSecurityInfo','NCrypt','printf'):
 assert forbidden not in inputs,forbidden
assert 'GENERIC_READ|READ_CONTROL,FALSE,0' in inputs
native=(repo/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8-sig')
assert 'personal_scq_receive_original(&n->scq_receiver,NULL,o' in native
assert 'PAPACC_SCQ_GATE_ORIGINAL_PROVENANCE_UNAVAILABLE:PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE' in native
assert 'static int ps_binding_ready(void){return 0;}' in (repo/'apps/server/server_service_win32.c').read_text(encoding='utf-8-sig')
assert not subprocess.check_output(['git','diff','--','tools/qualification/scm-n0-private-stdout-native.c'],cwd=repo)
receipt=here.parent/'phase6-persistent-ca-fixture-20261029'
observation=json.loads((receipt/'PERSISTENT-OBSERVATION.json').read_text(encoding='utf-8-sig'))
assert sha(receipt/'ONE-ATTEMPT.receipt.txt')==observation['receipt_sha256']
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag='ONA-'+profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig')
  tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  layout=(here/(tag+'-layout.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 26' in tests
  for marker in ('SCQ_INPUTS cases=21 checks=93','NO_CONTENT_NO_AUTHORITY bytes=248','SCQ_TRANSPORT_SCOPE cases=4',
   'PIPE_CALLS=0 NO_AUTHORITY','CA_BINDING_NATIVE_NEGATIVE','CA_BINDING_MODEL','SCQ_RECEIVER cases=24',
   'NATIVE_EPHEMERAL_COMPOSITION checks=89','PERSISTENT_KEYS=NONE'):
   assert marker in last,(tag,marker)
  managed,limit=map(int,re.search(r'KNOWN_MANAGED=(\d+) LIMIT=(\d+)',layout).groups())
  assert managed==({'Selected':1974984,'Legacy':1806416}[profile]) and managed<limit
  assert 'PROVIDER_KERNEL=UNKNOWN' in layout and 'NATIVE_ADMISSION=DENY' in layout
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=26,
   known_managed=managed,limit=limit,binaries={n:sha(folder/n) for n in
    ('papacc_server.exe','papacc_personal_scq_inputs_test.exe','papacc_scq_transport_scope_test.exe','papacc_personal_scq_dispatch_test.exe','papacc_personal_ca_native_test.exe')}))
proposal=here/'ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md'
put('ONA-QUALIFICATION.json',dict(task='PACC-PHASE6-OHI-NATIVE-ORIGINAL-ACQUISITION',head=head,
 OHI='ACCEPTED_UNCHANGED',metadata_custody='NATIVE_CODE_INTEGRATED_OFFLINE_QUALIFIED',
 administrative_token='EXISTING_NATIVE_PREDICATES_REUSED; MODELED_API_TESTS',
 content_authentication='NOT_IMPLEMENTED_NOT_DEMONSTRATED',attributed_SCQ_confirmation='NOT_IMPLEMENTED',
 cross_principal_delivery='BLOCKED_BY_EXISTING_TRANSPORT_T0_T2_T11_SCOPE',
 conflict='PROPOSED_REUSE_CONTRADICTS_ACCEPTED_SCOPED_TRANSPORT; NOT_CONFLICT_BETWEEN_ACCEPTED_DECISIONS',
 transport_cases=4,transport_pipe_calls=0,metadata_cases=21,metadata_checks=93,runs=runs,ctest_runs=104,
 native_entry='NULL_SOURCE_DENIED_BEFORE_KSP',operation_request='NOT_CREATED_NOT_READY',
 normative_candidate='PROPOSED_NOT_ACCEPTED',candidate_sha256=sha(proposal),
 new_fixed_storage_bytes=248,dynamic_SD_storage='THREE_SNAPSHOTS_PLUS_CHECKING_OVERLAP_NOT_OPERATIONALLY_QUALIFIED',
 auto_review_rejected_patch='RAW_SOURCE_PRESENTATION_AND_UNAUTHENTICATED_LOCATOR_ORIGIN; NOT_APPLIED',
 safe_alternative='EXISTING_HANDLE_METADATA_ONLY; NO_PATH_OPEN_NO_CONTENT_READ_NO_PRINT_NO_GRANT',
 retained_key='NOT_ACCESSED_RECEIPT_UNCHANGED',SCM_token='NOT_OBSERVED',MACHINE_KSP='NOT_OBSERVED',
 provider_kernel='UNKNOWN',protected_effects='NONE',authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
put('ONA-SOURCE-HASHES.json',[dict(path=n,before_sha256=before.get(n),sha256=sha(repo/n)) for n in sorted(set(before)|changed|new)])
put('ONA-ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(repo)).replace('\\','/'),bytes=p.stat().st_size,sha256=sha(p))
 for p in sorted(list(here.glob('ONA-*'))+[here/'NEXT-ONA-DELIVERY-DECISION.md'])
 if p.is_file() and p.name!='ONA-ARTIFACT-HASHES.json'])
print('PASS 4 builds zero warnings; 104 CTests; metadata248 bytes; transport scope mismatch demonstrated; native authority DENY')
