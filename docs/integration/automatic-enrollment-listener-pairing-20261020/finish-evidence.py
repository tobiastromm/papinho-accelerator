from pathlib import Path
import hashlib,json,re,shutil
r=Path.cwd();d=r/'docs/integration/automatic-enrollment-listener-pairing-20261020'
sha=lambda b:hashlib.sha256(b).hexdigest()
base=json.loads((d/'baseline.json').read_text(encoding='utf-8-sig'))
changes=[p for p,h in base.items() if not (r/p).is_file() or sha((r/p).read_bytes())!=h]
assert sorted(changes)==['CMakeLists.txt'],changes
ops=json.loads((d/'source-operations.json').read_text())
for o in ops: assert sha((r/o['target']).read_bytes())==o['after'],o['target']
results=[]
for profile in ['Selected','Legacy']:
 for config in ['Debug','Release']:
  log=(d/f'build-{profile}-{config}.log').read_text(errors='replace')
  assert '100% tests passed, 0 tests failed out of 10' in log
  assert not re.search(r'warning C\d+|error C\d+|FAILED:',log)
  src=r/f'build/root08-issuer-factual-transaction-binding-20261020/{profile}/{config}/Testing/Temporary/LastTest.log'
  dest=d/f'tests-{profile}-{config}.log';shutil.copyfile(src,dest)
  results.append({'profile':profile,'config':config,'suites_passed':10,'compiler_warnings':0,'detail':dest.name})
(d/'validation-results.json').write_text(json.dumps(results,indent=2)+'\n')
(d/'baseline-verification.json').write_text(json.dumps({'baseline_files':len(base),'changed_existing_files':changes,'approved_source_operations':len(ops),'preexisting_WIP_preserved':True},indent=2)+'\n')
budget={'windows_limit':3145728,'legacy_limit':2097152,'known_windows_base':2103847,'known_legacy_base':2067655,'runtime_fixed_delta':0,'sizeof_reader':64,'sizeof_trace':4272,'sizeof_pair':160,'sizeof_limiter':1128,'sizeof_tls_profile':280,'two_RX_TX':65600,'illustrative_owned_two_connection_data':2*(64+4272+160+280)+65600+1128,'native_RNG_caller_scratch':55,'enrollment_subcap':262144,'full_activation_peak':'UNKNOWN','provider_kernel':'UNKNOWN','operational_reservation':'NOT_QUALIFIED','total_status':'PARTIAL'}
(d/'ed11-census.json').write_text(json.dumps(budget,indent=2)+'\n')
print(json.dumps({'baseline_files':len(base),'build_configurations':len(results),'known_local_bytes':budget['illustrative_owned_two_connection_data']}))
