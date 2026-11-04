"""Derived evidence only. No elevated/manual, persistent key or installation run."""
from pathlib import Path
import hashlib,json,re,difflib,subprocess
here=Path(__file__).resolve().parent;repo=here.parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def put(n,v):(here/n).write_text(json.dumps(v,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
base=json.loads((here/'BASELINE.json').read_text());head=subprocess.check_output(['git','rev-parse','HEAD'],cwd=repo,text=True).strip();assert head==base['head']
assert '(void)p;return 0;' in (repo/'apps/server/server_configuration_personal_data_win32.inc').read_text()
native=json.loads((here/'NATIVE-RESULTS.json').read_text(encoding='utf-8-sig'));assert len(native)==8 and all(x['exit']==0 for x in native)
runs=[]
for profile in ('Selected','Legacy'):
 for config in ('Debug','Release'):
  tag=profile+'-'+config;folder=repo/'build/phase6-functional-20261028'/profile/config
  build=(here/(tag+'-build.log')).read_text(encoding='utf-8-sig');test=(here/(tag+'-ctest.log')).read_text(encoding='utf-8-sig');last=(here/(tag+'-LastTest.log')).read_text(encoding='utf-8-sig')
  assert not re.search(r'(?im)(warning C[0-9]+|warning:|error C[0-9]+|FAILED:|LNK2019|LNK1120)',build)
  assert '100% tests passed, 0 tests failed out of 19' in test
  assert 'PASS TOKEN_PREDICATE_FAULTS checks=88' in last and 'PASS 1405 DATA_MODEL assertions' in last
  observations=[json.loads(line) for line in last.splitlines() if line.startswith('{') and ('first_predicate' in line or 'INDEPENDENT_AFTER_FIRST_FAILURE' in line)]
  first=next(x for x in observations if 'first_predicate' in x)
  assert first['first_predicate']=='ADMIN_ENABLED' and first['failure_kind']==2 and first['native_error']==0
  assert not first['elevated'] and not first['parent_opened'] and first['native_close_result']==0 and first['observer_closed']
  groups=next(x for x in observations if 'admin_group_query_known' in x);assert groups['admin_group_query_known'] and not groups['admin_sid_present']
  runs.append(dict(profile=profile,configuration=config,build='PASS',warnings=0,ctests=19,token_fault_checks=88,enrollment_data_checks=1405,context=observations,
   binaries={n:sha(folder/n) for n in ('papacc_server.exe','papacc_personal_native_test.exe','papacc_personal_token_predicate_test.exe','papacc_enrollment_test.exe')},layout=(here/(tag+'-layout.log')).read_text().strip()))
release=repo/'build/phase6-functional-20261028/Selected/Release/papacc_personal_native_test.exe'
script=(here/'collect-owner-context.MANUAL.ps1').read_text(encoding='utf-8');assert sha(release).upper() in script
assert sha(here/'collect-owner-context.MANUAL.ps1') in (here/'MANUAL-COMMAND.txt').read_text()
old=here.parent/'phase6-persistent-ca-fixture-20261029';observation=json.loads((old/'PERSISTENT-OBSERVATION.json').read_text());assert sha(old/'ONE-ATTEMPT.receipt.txt')==observation['receipt_sha256']
changes=[];delta=[]
for name in base['files']+['apps/server/server_personal_token_checks_win32.inc','tests/personal_token_predicate_test.c']:
 p=repo/name;pre=here/'preimages'/p.name;a=pre.read_text(encoding='utf-8-sig').splitlines(True) if pre.exists() else [];b=p.read_text(encoding='utf-8-sig').splitlines(True)
 changes.append(dict(path=name,before=sha(pre) if pre.exists() else 'ABSENT',after=sha(p),changed=a!=b));delta.extend(difflib.unified_diff(a,b,fromfile='before/'+name,tofile='after/'+name))
put('SOURCE-CHANGES.json',changes);(here/'FUNCTIONAL-DELTA.patch').write_text(''.join(delta),encoding='utf-8')
put('QUALIFICATION.json',dict(head=head,runs=runs,ctest_runs=76,native_fixture_runs=8,
 first_failure='ADMIN_ENABLED_FALSE; QUERY_SUCCESS; STANDARD_UNELEVATED_CONTEXT',
 guard_changes='DIAGNOSTICS_ONLY; SAME_ORDER_AND_REQUIREMENTS',
 enrollment='CORRELATED_ORIGINAL_CSR_DATA_BORROW_IMPLEMENTED; NO_AUTHORITY_POP_L2',
 manual_E8='NO_FACTUAL_PRODUCERS; SOURCE_ABSENT_BEFORE_KDF',
 manual_collector=dict(script_sha256=sha(here/'collect-owner-context.MANUAL.ps1'),binary_sha256=sha(release),status='SYNTAX_VALIDATED_NOT_EXECUTED',requires='OWNER_WINDOWS_POWERSHELL_51_X64_ALREADY_ELEVATED'),
 retained_key='NOT_ACCESSED_OR_CHANGED; EVIDENCE_PRESERVED',ED11='PARTIAL',Phase6='PARTIAL',authority='DENY'))
put('ARTIFACT-HASHES.json',[dict(path=str(p.relative_to(here)),bytes=p.stat().st_size,sha256=sha(p)) for p in sorted(here.rglob('*')) if p.is_file() and p.name!='ARTIFACT-HASHES.json'])
print('PASS four builds; 76 CTests; eight native fixtures; ADMIN_ENABLED exact; manual collector pinned, not executed')
