"""Read existing public/source evidence only; no tests or native operations."""
from pathlib import Path
import hashlib,json,subprocess

here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
sources=json.loads((here/'SOURCE-HASHES.json').read_text(encoding='utf-8-sig'))
for row in sources:assert sha(repo/row['path'])==row['sha256'],row['path']
qualification=json.loads((here/'QUALIFICATION.json').read_text(encoding='utf-8-sig'))
binary_count=0
for run in qualification['runs']:
 for name,digest in run['binaries'].items():
  assert sha(repo/'build/phase6-functional-20261028'/run['profile']/run['configuration']/name)==digest
  binary_count+=1
service=(repo/'apps/server/server_service_win32.c').read_text(encoding='utf-8-sig')
native=(repo/'apps/server/server_configuration_personal_data_win32.inc').read_text(encoding='utf-8-sig')
assert 'static int ps_binding_ready(void){return 0;}' in service
assert '(void)p;return 0;' in native
optional=repo/'docs/adr/DECISION-personal-windows-optional-scm-20261022.md'
assert 'Select %ProgramFiles%\\PapinhoAccelerator as' in optional.read_text(encoding='utf-8-sig')
trust=repo/'src/provisioning/shared_structure/trust_unavailable.c'
assert 'SP_TRUST_DENIED' in trust.read_text(encoding='utf-8-sig')
old=here.parent/'phase6-persistent-ca-fixture-20261029'
observation=json.loads((old/'PERSISTENT-OBSERVATION.json').read_text(encoding='utf-8-sig'))
assert sha(old/'ONE-ATTEMPT.receipt.txt')==observation['receipt_sha256']
result=dict(task='PACC-PHASE6-PERSONAL-TARGET-AND-SCM-CNG-READINESS',head=head,
 source_hashes_unchanged=len(sources),binary_hashes_unchanged=binary_count,
 builds_reused=4,ctests_reused=84,builds_run=0,tests_run=0,
 retained_key_receipt_unchanged=True,retained_key_access='NONE',
 known_parent='NOT_ACQUIRED; PRIOR_PATH_METADATA_ONLY',
 production_guards='SCM_AND_NATIVE_CEREMONY_DENY_PRESERVED',
 local_guard_denial_is_native_CNG_isolation=False,
 accepted_binary_destination='%ProgramFiles%\\PapinhoAccelerator',
 preferred_binary_destination='%ProgramFiles%\\Papinho\\Accelerator',
 governance_conflict='DESTINATION_SELECTION_ONLY; VIRTUAL_ACCOUNT_ALREADY_ACCEPTED',
 minimum_session='ONE_FUTURE_SESSION; TECHNICAL_DESIGN_ONLY',
 operational_request='NOT_READY_NOT_CREATED',
 blockers=['binary destination reconciliation or retain accepted',
  'same-owner governed parent/protection/diagnostic-host path',
  'native KSP open probe under observed runtime distinct from local guard',
  'final artifacts bindings descriptor and transitive resource bounds'],
 authority='DENY',ED11='PARTIAL',Phase6='PARTIAL')
(here/'TARGET-READINESS.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
paths=[here/n for n in ('TARGET-READINESS-20261029.md','TARGET-BINARY-DESTINATION.CANDIDATE.md',
 'NEXT-BOUNDED-SCM-CNG-QUALIFICATION-PATH.md','TARGET-READINESS-CHECK.py','TARGET-READINESS.json')]
paths.extend([optional,trust,repo/'tests/ca_key_readonly_diagnostic_win32.c',
 repo/'src/provisioning/manual_package_validation_openssl.c',
 repo/'apps/server/server_configuration_bootstrap_win32.inc'])
(here/'TARGET-HASHES.json').write_text(json.dumps([dict(path=str(p.relative_to(repo)),
 bytes=p.stat().st_size,sha256=sha(p)) for p in paths],indent=2)+'\n',encoding='utf-8')
print(f'PASS existing evidence integrity: {len(sources)} sources/{binary_count} binaries; no campaigns or operations; request NOT_READY')
