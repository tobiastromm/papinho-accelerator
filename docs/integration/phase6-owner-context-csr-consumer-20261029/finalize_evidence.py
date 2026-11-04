"""Derived evidence only. Never reruns the owner's elevated collection."""
from pathlib import Path
import hashlib,json,re,difflib,subprocess
here=Path(__file__).resolve().parent;repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text());head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip();assert head==base['head']
owner=json.loads((here/'OWNER-MANUAL-RESULT.json').read_text())
previous=here.parent/'phase6-native-context-20261029'
assert sha(previous/'collect-owner-context.MANUAL.ps1')==owner['script_sha256_checked']
assert owner['binary_sha256_checked']=='0cf471896a5f40cc4319054ee35f6b980201ae77c29843470906bcbb35ef31dc'
assert owner['preflight_result']==0 and owner['admin_enabled'] and owner['elevated'] and owner['native_close_result']==0 and owner['observer_closed']
assert owner['selected_install_parent']=='UNKNOWN_NOT_SELECTED' and owner['selected_runtime_account']=='UNKNOWN_NOT_SELECTED'
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text()
consumer=(repo/'src/provisioning/enrollment_issuer.c').read_text()
for forbidden in ('papacc_issuer_prepare(', 'papacc_issuer_reserve(', 'NCryptSignHash(', 'CreateService', 'CURRENT=PASS'):assert forbidden not in consumer
assert 'record.state!=PAPACC_ISSUER_SERIAL_RESERVED' in consumer and 'before.state!=PAPACC_ISSUER_PUBLISHED' in consumer and 't->credential_released=1' in consumer
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig');tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig');last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C[0-9]+|warning:|error C[0-9]+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 20' in tests
  assert 'ENROLLMENT_ISSUER_MODEL checks=77' in last
  assert 'NATIVE_EPHEMERAL_COMPOSITION checks=89' in last and 'signatures=3 verifications=7' in last
  assert 'PASS 2718 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last and 'backend=816' in last and 'PASS 1405 DATA_MODEL assertions' in last
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=20,consumer_model_checks=77,native_ephemeral_checks=89,cng_model_checks=2718,
   binaries={n:sha(folder/n) for n in ('papacc_server.exe','papacc_enrollment_issuer_test.exe','papacc_issuer_cng_x509_test.exe')},layout=(here/(tag+'-layout.log')).read_text().strip()))
old=here.parent/'phase6-persistent-ca-fixture-20261029';observation=json.loads((old/'PERSISTENT-OBSERVATION.json').read_text());assert sha(old/'ONE-ATTEMPT.receipt.txt')==observation['receipt_sha256']
changes=[];delta=[]
for name in dict.fromkeys(base['files']+['src/provisioning/enrollment_issuer.c','src/provisioning/enrollment_issuer.h','tests/enrollment_issuer_model_test.c']):
 p=repo/name;pre=here/'preimages'/p.name;a=pre.read_text(encoding='utf-8-sig').splitlines(True) if pre.exists() else [];b=p.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre) if pre.exists() else 'ABSENT',after=sha(p),changed=a!=b));delta.extend(difflib.unified_diff(a,b,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes);(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=80,
 owner_context='PASS_AT_REPORTED_OBSERVATION; NO_INSTALLATION_AUTHORITY',manual_collection_reexecuted=False,
 composition='HELLO_CHALLENGE_CONFIRM_ORIGINAL_CSR_NORMAL_PUBLISHED_SINGLE_DELIVERY_CREDENTIALS_FRAME_OFFLINE',
 authority='DENY',test_authority='MODEL_ONLY',crypto='REAL_EPHEMERAL_CNG; NO_PERSISTENT_CREATION',
 normal_sign_phase_guard='PRESERVED; PUBLIC_DATA_VALIDATOR_SEPARATE_NO_AUTHORITY',
 pending='SAME_CORE_POLL; NO_RESUBMIT_OR_RESIGN; FACTUAL_POLL_AFTER_EXPLICIT_ISSUER_STOP',
 manual_E8='FACTUAL_PRODUCERS_NOT_IMPLEMENTED; DENY_BEFORE_KDF',
 path_account_recommendations='CANDIDATES_NOT_SELECTED_NOT_CREATED',key_retained='NOT_ACCESSED_OR_MODIFIED',
 ED11='PARTIAL',Phase6='PARTIAL',productive_effects='NONE'))
put('ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p)) for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS owner context reconciled without repeat; four builds; 80 CTests; CSR consumer qualified offline; choices still candidates')
