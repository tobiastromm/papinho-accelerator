"""Derived evidence only; never invokes the persistent fixture."""
from pathlib import Path
import json,hashlib,difflib,re,subprocess
here=Path(__file__).resolve().parent;repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text());head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip();assert head==base['head']
observation=json.loads((here/'PERSISTENT-OBSERVATION.json').read_text())
assert observation['run_count']==1 and observation['checks']==85 and observation['key_retained']
assert observation['receipt_sha256']==sha(here/'ONE-ATTEMPT.receipt.txt')
assert 'PASS NATIVE_PERSISTENT_FIXTURE checks=85' in (here/'NATIVE-PERSISTENT.log').read_text()
assert 'NATIVE_REFS_CLOSED; KEY_RETAINED; NO_DELETE' in (here/'ONE-ATTEMPT.receipt.txt').read_text()
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig');ctest=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig');last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C[0-9]+|warning:|error C[0-9]+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 3' in ctest
  assert 'PASS 2718 SYNTHETIC_SIGNATURES_AND_ROOT_ONLY' in last and 'PASS NATIVE_EPHEMERAL_COMPOSITION checks=67' in last
  digest=sha(folder/'papacc_issuer_cng_x509_test.exe')
  if tag=='Selected-Debug':assert digest==observation['binary_sha256']
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=3,model_checks=2718,binary_sha256=digest))
changes=[];delta=[]
for name in base['files']:
 p=repo/name;pre=here/'preimages'/p.name;a=pre.read_text(encoding='utf-8-sig').splitlines(True);b=p.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre),after=sha(p),changed=a!=b));delta.extend(difflib.unified_diff(a,b,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes);(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
previous=here.parent/'phase6-first-native-crypto-20261029';unchanged={}
for row in json.loads((previous/'SOURCE-CHANGES.json').read_text()):
 if row['path'].startswith('apps/server/'):
  assert sha(repo/row['path'])==row['after'];unchanged[row['path']]=row['after']
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text()
put('QUALIFICATION.json',dict(head=head,runs=runs,regression_runs=12,persistent_experiments=1,observation=observation,unchanged_product_sources=unchanged,
 ceremony='PRODUCT_DENY; FIXTURE_COMPOSITION_ONLY',custody='CURRENTUSER_NATIVE_REOPEN_QUALIFIED_AT_OBSERVATION; MACHINE_AND_REBOOT_NOT_QUALIFIED',
 M7='REAL_CREDENTIAL_DIGEST_TO_MODEL_COHORT; NO_QUIESCENCE_CLAIM',E8='SOURCE_ABSENT',enrollment='L2_PENDING',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY'))
put('ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p)) for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS four builds; 12 affected regressions; one persistent CurrentUser fixture/reopen; retained test key; product DENY')
