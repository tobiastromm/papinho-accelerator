"""Read-only product inspection and derived evidence; no product execution."""
from pathlib import Path
import json,hashlib,difflib,re,subprocess
here=Path(__file__).resolve().parent
repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text())
head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip()
assert head==base['head']
rows=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig')
  tests=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig')
  last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C[0-9]+|warning:|error C[0-9]+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 14' in tests
  assert 'PASS 2718 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last
  assert 'PASS NATIVE_EPHEMERAL_COMPOSITION checks=67 CNG_RSA2048_REAL signatures=3 verifications=4' in last
  assert 'PERSISTENT_KEYS=NONE PRODUCT_AUTHORITY=DENY' in last
  rows.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=14,
   synthetic_model_checks=2718,native_ephemeral_checks=67,real_signatures=3,
   authority='DENY',private_export='NONE',persistent_keys='NONE',
   binaries={n:sha(folder/n) for n in ('papacc_server.exe','papacc_issuer_cng_x509_test.exe','papacc_issuer_cng_production_gate_test.exe')}))
# The real native guards and historical contracts are untouched by this continuation.
previous=here.parent/'phase6-ca-initialize-native-20261029'
prior={r['path']:r['after'] for r in json.loads((previous/'SOURCE-CHANGES.json').read_text())}
unchanged={}
for path in ('apps/server/server_configuration_personal_ca_win32.inc','apps/server/server_configuration_personal_data_win32.inc','apps/server/server_configuration_root_transaction_win32.inc','apps/server/server_configuration_pending_win32.h'):
 assert sha(repo/path)==prior[path];unchanged[path]=prior[path]
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text()
for path in ('src/issuer/issuer_core.c','src/issuer/issuer_codec.c','src/issuer/issuer_publication_v2.inc'):
 # Reused algorithms/formats, not another implementation.
 old=json.loads((here.parent/'phase6-published-ack-v2-20261029/SOURCE-CHANGES.json').read_text())
 item=next(r for r in old if r['path']==path);assert sha(repo/path)==item['after'];unchanged[path]=item['after']
changes=[];delta=[]
for name in dict.fromkeys(base['files']+['tests/issuer_native_ephemeral_cases.inc']):
 p=repo/name;pre=here/'preimages'/p.name
 a=pre.read_text(encoding='utf-8-sig').splitlines(True) if pre.exists() else []
 b=p.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre) if pre.exists() else 'ABSENT',after=sha(p),changed=a!=b))
 delta.extend(difflib.unified_diff(a,b,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes)
(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=rows,ctest_runs=56,
 native_crypto='CNG_SOFTWARE_KSP_EPHEMERAL_RSA2048_SHA256_REAL',
 creation_route='REAL_CA_INITIALIZE_BODY_TEST_TRANSLATION; TEST_ONLY_NATIVE_CONTEXT_AND_KEY_CREATOR',
 ca_self_signature='REAL_CNG_TEST_ORCHESTRATION_WITH_PRODUCT_TBS',
 leaf_signature='REAL_PRODUCT_CNG_BACKEND_WITH_TEST_ONLY_ROOT_AUTHORITY',
 publication='REAL_V2_CODEC_AND_SHA256; IN_MEMORY_CURRENT_AND_AUTHORITY_SIMULATED',
 custody='REAL_BORROW_TRANSFER_RETURN; PERSISTENT_ACL_AND_CUSTODY_NOT_QUALIFIED',
 m7='ACTUAL_ISSUED_DER_DIGEST_TO_SIMULATED_SESSION_COHORT; REAL_EFFECT_ONLY_API; NO_CLOSE_OR_QUIESCENCE_CLAIM',
 product_operations='NONE',ephemeral_crypto='EXECUTED_AUTHORIZED_ISOLATED',
 provider_kernel='UNKNOWN',NORMAL='DENY',ED11='PARTIAL',Phase6='PARTIAL',
 reused_unchanged_sources=unchanged))
put('ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p))
 for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS four builds; 56 CTests; four real ephemeral CNG journeys; productive gates unchanged DENY')
