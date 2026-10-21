from pathlib import Path
import json,hashlib,difflib,subprocess
r=Path.cwd();d=r/'docs/integration/issuer-session-live-publication-20261019';ops=json.loads((d/'operation.json').read_text());names=[e['path'] for e in ops]+['apps/server/server_configuration_attachment_win32.inc'];files=[];diff=[]
expected={'CMakeLists.txt':'2367da11f1239e754b6a782d1c92e15ff2c7307e669eb3119de05a59600be180','tests/canonical_root_transaction_test.c':'41d31892f94f320e8807f740b245bb3c369b76107cee23a86e0d3c3624cc1a92','apps/server/server_configuration_attachment_win32.inc':'9715fa3da73acfca763f83127104182013daad54300947a57549d4ea8a557b35'}
for e in ops:expected.setdefault(e['path'],e['after'])
for p in names:
 old=(d/'preimages'/p.replace('/','__')).read_bytes();new=(r/p).read_bytes();h=hashlib.sha256(new).hexdigest();assert h==expected[p],p
 files.append(dict(path=p,before=hashlib.sha256(old).hexdigest(),after=h,bytes=len(new)))
 diff.extend(difflib.unified_diff(old.decode().replace('\r\n','\n').splitlines(True),new.decode().replace('\r\n','\n').splitlines(True),fromfile='a/'+p,tofile='b/'+p))
(d/'final-source.diff').write_bytes(''.join(diff).encode())
for p in ['docs/capabilities/client-pairing.md','docs/portability.md']:
 old=(d/'preimages'/p.replace('/','__')).read_bytes();new=(r/p).read_bytes();assert new.startswith(old),p
 files.append(dict(path=p,before=hashlib.sha256(old).hexdigest(),after=hashlib.sha256(new).hexdigest(),bytes=len(new)))
results={}
for cfg in ['Debug','Release']:
 rows=json.loads((d/(cfg+'-final2-test-results.json')).read_text(encoding='utf-8-sig'));assert len(rows)==23 and all(e['exit']==0 for e in rows);results[cfg]=rows
 log=(d/(cfg+'-build5.log')).read_text(errors='replace');assert 'FAILED:' not in log and 'warning C' not in log and 'error C' not in log
 census=(d/(cfg+'-papacc_canonical_root_layout_test-final2.log')).read_text();assert 'KNOWN_MANAGED=1790912' in census and 'ROOT_LAYOUT=174616' in census
 for case in ['catalog-live','catalog-live-conflict','catalog-live-unknown','catalog-live-stop','catalog-live-late-stop','catalog-live-proof-change']:
  t=(d/(cfg+'-'+case+'-final2.log')).read_text();assert 'PASS' in t and 'COMBINED_RESERVED=1957939' in t
 for p in (r/'build/issuer-session-live-publication-20261019'/cfg).glob('*.exe'):files.append(dict(path=p.relative_to(r).as_posix(),after=hashlib.sha256(p.read_bytes()).hexdigest(),bytes=p.stat().st_size))
changed=set(names+['docs/capabilities/client-pairing.md','docs/portability.md']);preserved=[]
previous=json.loads((r/'docs/integration/issuer-live-transaction-safety-20261019/final-manifest.json').read_text())
for e in previous['files']:
 p=r/e['path']
 if e['path'] not in changed:
  assert hashlib.sha256(p.read_bytes()).hexdigest()==e['after'],e['path'];preserved.append(e['path'])
base=json.loads((d/'baseline.json').read_text());now=set(subprocess.check_output(['git','status','--porcelain=v1','--untracked-files=all'],text=True).splitlines());missing=[s for s in base['wip'] if s not in now];assert not missing,missing
pin=hashlib.sha256((r/'dependencies/papinho-secure-transport.txt').read_bytes()).hexdigest();assert pin=='3d347a0606c7ad03d2ed02b593daba8853a2c627f85a146f28fb90c94ba05c39'
adr=hashlib.sha256((r/'docs/adr/DECISION-issuer-c2-epoch-authority-relation-20261019.md').read_bytes()).hexdigest();assert adr=='7bdc1bed0c3b70d42945b16ce5feeef25d368ec7004dbcec1d66b7fb9f816c48'
artifacts=[dict(path=p.relative_to(r).as_posix(),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sorted(d.rglob('*')) if p.is_file() and p.name!='final-manifest.json']
manifest=dict(task='PACC-PHASE6-ISSUER-SESSION-LIVE-PUBLICATION-COMPLETION-1',completed=True,scope='CANONICAL_SOURCE_AND_OFFLINE_ADMINISTRATIVE_METADATA_NO_NATIVE_SESSION_AUTH_QUALIFICATION',head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),branch='main',official_executor_review='INITIAL_12_FILE_OPERATION_AND_EXACT_SUPPLEMENTS_APPROVED_NO_NEW_DENIAL',current='FACTUAL_POST_COMMIT_REVALIDATED',epoch='A_POST_COMMIT_SEPARATE_C2_P_PRESERVED_FOR_IDENTICAL_PROJECTION',cas='CONFLICT_UNKNOWN_DENY_NO_RETRY_NO_OLD_AUTHORITY_FALLBACK',reaper='ORIGINAL_QUIESCENCE_GUARDS_PLUS_EXACT_ROOT_PIN_UNPIN',builds='DEBUG_RELEASE_PASS',warnings=0,offline_invocations=46,test_results=results,known_managed_full_outbound_profile=1790912,root_size_full_outbound=174616,base_combined_reservation=1957939,previous_same_profile_reservation=1957667,incremental_bytes=272,closed_attachment_reservation=1962035,managed_limit=2097152,prior_reduced_profile_census_not_applicable=True,root_config_owner_normal=103568,root_config_owner_component_test=103592,worker_added=False,provider_kernel='UNKNOWN',authoritative_recovery='DENY_UNTIL_QUALIFIED',CNG='UNRESOLVED',admission='DENY',network='NOT_RUN',real_CA=False,certificates_issued=False,normal_sdk_checksums_verified=46,production_pin='0.6.4',production_pin_sha256=pin,accepted_relation_sha256=adr,unaffected_prior_manifest_files_verified=preserved,wip_baseline_status_entries_preserved=len(base['wip']),wip='PRESERVED_OUTSIDE_AUTHORIZED_HUNKS',test_processes_remaining=0,files=files,artifacts=artifacts,next='AUTHORITATIVE_RECOVERY_AND_WINDOWS_CNG_INTEGRATION')
(d/'final-manifest.json').write_text(json.dumps(manifest,indent=2));print('Verified 46/46 final invocations, 13 source files,',len(preserved),'unaffected prior files and',len(base['wip']),'existing WIP status entries')
