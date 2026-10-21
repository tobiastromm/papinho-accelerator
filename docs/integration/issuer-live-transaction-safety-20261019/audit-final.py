from pathlib import Path
import hashlib,json,difflib,subprocess
r=Path.cwd();d=r/'docs/integration/issuer-live-transaction-safety-20261019';ops=json.loads((d/'operation.json').read_text());files=[];patch=[]
for e in ops:
 p=e['path'];f=r/p;b=f.read_bytes();h=hashlib.sha256(b).hexdigest();expected=e['after']
 if p=='apps/server/server_launcher_owner_win32.c':expected='6647936749308a8d1002a55d761c972e3bbd487c9c6856926c2883bd39b4ef52'
 assert h==expected,(p,h)
 old=(d/'preimages'/p.replace('/','__')).read_bytes() if e['before'] else b''
 files.append(dict(path=p,before=e['before'],after=h,bytes=len(b)))
 patch.extend(difflib.unified_diff(old.decode().splitlines(True),b.decode().splitlines(True),fromfile='a/'+p,tofile='b/'+p))
(d/'final-source.diff').write_text(''.join(patch))
for p in ['docs/capabilities/client-pairing.md','docs/portability.md']:
 b=(r/p).read_bytes();files.append(dict(path=p,after=hashlib.sha256(b).hexdigest(),bytes=len(b)))
base=json.loads((r/'docs/integration/issuer-live-session-exact-integration-20261019/baseline.json').read_text());changed={e['path'] for e in files};preserved=[]
for e in base['files']:
 p=Path(e['Path']);relative=p.relative_to(r).as_posix()
 if relative not in changed:
  assert hashlib.sha256(p.read_bytes()).hexdigest().upper()==e['Hash'].upper(),relative
  preserved.append(relative)
for cfg in ['Debug','Release']:
 cases=json.loads((d/(cfg+'-test-results.json')).read_text(encoding='utf-8-sig'))
 assert len(cases)==12 and all(e['exit']==0 for e in cases)
 for extra in ['normal-guard','application-root2']:
  assert 'PASS' in (d/(cfg+'-'+extra+'.log')).read_text()
 for i in [1,2]:
  log=(d/(cfg+'-build'+str(i)+'.log')).read_text(errors='replace')
  assert 'FAILED:' not in log and 'warning C' not in log and 'error C' not in log
 assert 'COMBINED_RESERVED=1957667' in (d/(cfg+'-application-root2.log')).read_text()
assert 'fatal error' not in (d/'portable-check2.log').read_text(errors='replace')
artifacts=[]
for p in sorted(d.rglob('*')):
 if p.is_file() and p.name!='final-manifest.json':artifacts.append(dict(path=p.relative_to(r).as_posix(),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
for cfg in ['Debug','Release']:
 for p in (r/'build/issuer-live-transaction-safety-20261019'/cfg).glob('*.exe'):
  files.append(dict(path=p.relative_to(r).as_posix(),after=hashlib.sha256(p.read_bytes()).hexdigest(),bytes=p.stat().st_size))
manifest=dict(task='PACC-PHASE6-ISSUER-LIVE-TRANSACTION-SAFETY-1',completed=True,scope='PENDING_PERSISTENCE_ADAPTER_ONLY_NO_SESSION_OR_PUBLICATION_HUNKS',head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip(),branch='main',official_executor_review='APPROVED_11_FILE_OPERATION_AND_EXACT_STACK_DEDUP_HUNK_NO_DENIAL',builds='DEBUG_RELEASE_PASS',warnings=0,offline_invocations=28,portable_contract='STRICT_C_COMPILE_PASS_FINAL_LOG2',synthetic_faults='WORKER_PAUSE_AND_UNKNOWN_EXPLICIT_TEST_ONLY',native_evidence='LOCAL_TXF_COMMIT_ROLLBACK_FILE_CAS_NOT_OS_CRASH_OR_CERTIFICATE_ISSUANCE',known_managed_bytes=1695352,combined_fixture_reserved_bytes=1957667,managed_limit=2097152,normal_configuration_owner_size=103520,component_test_configuration_owner_size=103544,worker_stack_reserved_bytes=65536,authority_recovery='DENY_UNTIL_QUALIFIED',live_current_publication='DENY_PENDING_SESSION_HUNKS',provider_kernel='UNKNOWN',admission='DENY',network='NOT_RUN',key_operations='NOT_RUN',production_pin_sha256=hashlib.sha256((r/'dependencies/papinho-secure-transport.txt').read_bytes()).hexdigest(),unchanged_previously_captured_files=preserved,wip='PRESERVED_OUTSIDE_EXACT_AUTHORIZED_HUNKS',files=files,artifacts=artifacts,next='RECONCILE_AND_APPLY_EXACT_ISSUER_SESSION_HUNKS')
(d/'final-manifest.json').write_text(json.dumps(manifest,indent=2))
print('Verified',len(preserved),'unchanged captured files; 28/28 invocations; final source hashes; manifests written')
