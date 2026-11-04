"""Offline build/test evidence only. Never acquires a native SCQ source."""
from pathlib import Path
import hashlib,json,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
proposal=here/'OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md'
assert sha(proposal)=='34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561'
decision=repo/'docs/adr/DECISION-personal-scq-original-handoff-20261030.md'
assert 'status: accepted' in decision.read_text(encoding='utf-8')
native=(repo/'apps/server/server_configuration_personal_native_win32.inc').read_text(encoding='utf-8-sig')
assert 'return PAPACC_SCQ_GATE_NATIVE_SOURCE_UNAVAILABLE;' in native
assert 'personal_scq_receive_original(&n->scq_receiver,NULL,o' in native
assert 'personal_scq_receive_consume(&n->scq_receiver,o' in native
assert 'static int ps_binding_ready(void){return 0;}' in (repo/'apps/server/server_service_win32.c').read_text(encoding='utf-8-sig')
old={r['path']:r['sha256'] for r in json.loads((here/'OSR-SOURCE-HASHES.json').read_text(encoding='utf-8-sig'))}
changed={'apps/server/personal_native_win32.h','apps/server/server_configuration_commit_win32.c',
 'apps/server/server_configuration_commit_win32.h','apps/server/server_configuration_personal_data_win32.inc',
 'apps/server/server_configuration_personal_native_win32.inc','tests/personal_scq_dispatch_test.c','tests/personal_ca_binding_win32_test.c'}
for n,digest in old.items():
 if n not in changed:assert sha(repo/n)==digest,n
sources=set(old)|changed|{'apps/server/personal_scq_handoff_win32.h','apps/server/personal_scq_handoff_win32.inc'}
receipt=here.parent/'phase6-persistent-ca-fixture-20261029'
observation=json.loads((receipt/'PERSISTENT-OBSERVATION.json').read_text(encoding='utf-8-sig'))
assert sha(receipt/'ONE-ATTEMPT.receipt.txt')==observation['receipt_sha256']
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag='OHI-'+profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig')
  tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  layout=(here/(tag+'-layout.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C\d+|warning:|error C\d+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 24' in tests
  for marker in ('SCQ_RECEIVER cases=24 ORIGINAL_REFERENCE_MODEL NOT_NATIVE_ORIGIN_PROOF bytes=96',
   'SCQ_DISPATCH checks=348','CA_BINDING_NATIVE_NEGATIVE','CA_BINDING_MODEL',
   'NATIVE_EPHEMERAL_COMPOSITION checks=89','PERSISTENT_KEYS=NONE'):
   assert marker in last,(tag,marker)
  managed,limit=map(int,re.search(r'KNOWN_MANAGED=(\d+) LIMIT=(\d+)',layout).groups())
  assert managed==({'Selected':1974736,'Legacy':1806168}[profile]) and managed<limit
  assert 'PROVIDER_KERNEL=UNKNOWN' in layout and 'NATIVE_ADMISSION=DENY' in layout
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=24,
   known_managed=managed,limit=limit,binaries={n:sha(folder/n) for n in
   ('papacc_server.exe','papacc_personal_scq_dispatch_test.exe','papacc_personal_ca_native_test.exe','papacc_personal_ca_model_test.exe')}))
put('OHI-QUALIFICATION.json',dict(task='PACC-PHASE6-OSR-SCQ-HANDOFF-ACCEPTANCE-AND-INTEGRATION',head=head,
 proposal_sha256=sha(proposal),decision_sha256=sha(decision),normative_status='ACCEPTED',
 receiver='INTEGRATED_OFFLINE_COMPONENT_ONLY',native_integration='INCOMPLETE',
 native_source='NO_AUTHENTICATED_ACQUISITION_OR_DELIVERY_PRODUCER',native_entry_source='NULL_DENIED',
 first_remaining_gate='IMPLEMENT_NATIVE_ORIGINAL_ACQUISITION_CONFIRMATION_CONSUMPTION_AND_DELIVERY',
 operational_request='NOT_CREATED_NOT_READY',new_operational_approval='NONE',
 runs=runs,ctest_runs=96,receiver_scenarios=24,new_fixed_storage_bytes=96,
 native_origin_proof='NOT_DEMONSTRATED',durable_consumption='NOT_DEMONSTRATED',cross_process_transfer='NOT_IMPLEMENTED',
 SCM_token='NOT_OBSERVED',MACHINE_KSP='NOT_OBSERVED',parent='NOT_ACQUIRED_NOT_CREATED',
 protected_effects='NONE',retained_key='NOT_ACCESSED_RECEIPT_UNCHANGED',provider_kernel='UNKNOWN',
 authority='DENY',ED11='PARTIAL',Phase6='PARTIAL'))
put('OHI-SOURCE-HASHES.json',[dict(path=n,before_sha256=old.get(n),sha256=sha(repo/n)) for n in sorted(sources)])
put('OHI-ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(repo)).replace('\\','/'),bytes=p.stat().st_size,sha256=sha(p))
 for p in sorted(list(here.glob('OHI-*'))+[here/'NEXT-OHI-NATIVE-ORIGINAL-ACQUISITION.md',decision])
 if p.is_file() and p.name!='OHI-ARTIFACT-HASHES.json'])
print('PASS 4 builds zero warnings; 96 CTests; receiver96 bytes; origin producer NOT_IMPLEMENTED; native DENY')
