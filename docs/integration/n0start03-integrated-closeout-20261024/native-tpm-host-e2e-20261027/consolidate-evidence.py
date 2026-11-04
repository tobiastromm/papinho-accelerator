from pathlib import Path
import json,hashlib,re,csv,shutil,subprocess
root=Path.cwd();base=root/'docs/integration/n0start03-integrated-closeout-20261024';p=base/'native-tpm-host-e2e-20261027'
sha=lambda f:hashlib.sha256(Path(f).read_bytes()).hexdigest()
write=lambda n,x:(p/n).write_text(json.dumps(x,indent=2,ensure_ascii=True)+'\n',encoding='utf-8')
# Copy final successful fixtures into stable evidence paths; leave originals and earlier failures.
summary=[];ledger=[]
for config in ['Debug','Release']:
 d=root/f'build/v3-backings-20261021/Selected/{config}/Testing'
 latest=max(d.glob('tpm-console-*'),key=lambda q:q.stat().st_mtime_ns)
 for mode in ['clean','wrong','escape']:
  raw=(latest/mode/'result.txt').read_text(encoding='utf-8')
  (p/f'{config}-{mode}-FINAL.txt').write_text(raw,encoding='utf-8')
  j=json.loads(next(line for line in raw.splitlines() if line.startswith('{')))
  assert j['transport']=='MOCK_ENDPOINT_NO_TPM' and j['authority']=='DENY'
  if mode=='clean':assert j['proof'] and j['context_close'] and not j['retained'] and j['controlled_refs']==0
  for s in j['samples']:ledger.append(dict(configuration=config,case=mode,**s))
  summary.append(dict(configuration=config,case=mode,first=j['first'],mock_submits=j['calls'],proof_model=j['proof'],sampled_peak=j['sampled_peak'],retained=j['retained'],controlled_refs=j['controlled_refs'],samples=len(j['samples']),admin_sample_count=len(j['administrative_samples']),min_own=min(x['count'] for x in j['samples'] if x['known']),max_private=max(x['private_bytes'] for x in j['samples'] if x['memory_known']),max_working_set=max(x['working_set'] for x in j['samples'] if x['memory_known']),elapsed_sample_ms=j['samples'][-1]['tick_ms']-j['samples'][0]['tick_ms'],original_fixture=str(latest.relative_to(root)).replace('\\','/')))
write('FINAL-RESOURCE-SUMMARY.json',summary)
with (p/'PHASE-LEDGER.csv').open('w',encoding='utf-8',newline='') as f:
 w=csv.DictWriter(f,fieldnames=list(ledger[0]));w.writeheader();w.writerows(ledger)
frames=[]
for f in (p/'layout').glob('*.asm'):
 txt=f.read_text(errors='replace');current=None
 for line in txt.splitlines():
  m=re.match(r'^(\w+)\s+PROC\b',line)
  if m:current=m.group(1)
  m=re.match(r'\s+sub\s+rsp,\s*(\d+)\b',line)
  if m and current:frames.append(dict(source=f.stem,function=current,rsp_sub_bytes=int(m.group(1)),scope='own RSP subtraction; excludes pushes/callees/OS/transitive/reserved stack'))
write('COMPILED-STACK-FRAMES.json',frames)
# Preserve all known historical pins, exact source roles, accepted proposal hashes.
checks=[]
for item in json.loads((base/'tpm-l1-native-proof-and-parallel-closure-20261026/E17-E18-REUSE-VERIFICATION.json').read_text()):
 q=root/item['path'];actual=sha(q);checks.append(dict(path=item['path'],expected=item['expected'],actual=actual,unchanged=actual==item['expected']))
old=base/'tpm-native-headroom-result-20261027'
for n,h in json.loads((base/'tpm-observed-249-resolution-20261027/HISTORICAL-PINS-PRESERVED.json').read_text()).items():
 q=old/n
 if not q.exists():
  found=[f for f in old.iterdir() if f.is_file() and sha(f)==h]
  assert found,(n,h);q=found[0]
 checks.append(dict(path=str(q.relative_to(root)).replace('\\','/'),expected=h,actual=sha(q),unchanged=sha(q)==h))
for expected in ['c99e4a8c599a9f3937e3dda08458b043136ecee1a14c1bcc7a1d3ae1ed3a057a','eb32e4e79476f53f9aa7be53dfe911a4b5688c4b67ce1f2fefed6f87c32d90c8','335dd1f829d959220ab1e9e0c2038cae9209ec5b9c205e52a15952d55686d5e3']:
 fs=[q for q in (root/'docs/adr').glob('*.md') if sha(q)==expected];assert len(fs)==1
 checks.append(dict(path=str(fs[0].relative_to(root)).replace('\\','/'),expected=expected,actual=sha(fs[0]),unchanged=True))
assert all(x['unchanged'] for x in checks);write('HISTORICAL-AND-REUSE-PINS.json',checks)
# Source and artifact pins for native technical integration, NOT operational pins.
files=list((root/'tools/qualification/tpm-native-proof').glob('*.*'))+list((root/'tools/qualification/tpm-native-proof/fixtures').glob('*'))
files+=[root/'CMakeLists.txt']
for c in ['Debug','Release']:
 for n in ['proof','core_test','platform_test','artifact_test','e2e_fixture']:files.append(root/f'build/v3-backings-20261021/Selected/{c}/papacc_tpm_native_{n}.exe')
pins={str(f.relative_to(root)).replace('\\','/'):dict(bytes=f.stat().st_size,sha256=sha(f)) for f in files if f.is_file()};write('TECHNICAL-PINS.json',pins)
# Inert candidate only; CREATE_NEW directory and files, no owner approval file.
c=p/'manual-candidate';c.mkdir(exist_ok=False)
exe=root/'build/v3-backings-20261021/Selected/Release/papacc_tpm_native_proof.exe';shutil.copyfile(exe,c/'NativeTpmProof.exe');shutil.copyfile(root/'tools/qualification/tpm-native-proof/fixtures/ek-certificate.PUBLIC.der',c/'ek-certificate.PUBLIC.der')
r=json.loads((old/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json').read_text())
r.update(status='NATIVE_C_TECHNICALLY_QUALIFIED_OFFLINE_NOT_APPROVED_NOT_EXECUTED',executable='NativeTpmProof.exe',executable_sha256=sha(exe),source_sha256=sha(root/'tools/qualification/tpm-native-proof/native_host.c'),candidate_operation='TPMPROOF-NATIVE-C-20261027-CANDIDATE-INERT',native_mode_executed=False,approval_created=False,manual_command_activated=False)
r['profile_sources']={str(q.relative_to(root)).replace('\\','/'):sha(q) for q in files if q.is_file() and q.suffix in ['.h','.c','.txt']}
r['prior_attempt_disposition']={'request_sha256':'4aa1d74fc4dce21e17ae057b8b2c72ddcefc00a5a3a225b23c6cea24a78fe1d8','approval_sha256':'df36112d56b152f2f91e5609c85128770d47c809e8f49567a76d44e219cf7331','status':'ONE_MANUAL_INVOCATION_USED_BEFORE_TPM_NO_RETRY','first':'NATIVE_OBSERVER_HEADROOM','query_succeeded':True,'own_count':249,'query_error':0,'TPM_calls':0,'context':'NOT_OPENED','retained':False}
r.pop('diagnostic_change',None)
r['implementation_change']={'native_C':True,'CLR':False,'PowerShell_in_qualifier':False,'TBS_load':'System32-only after approval confirmation and receipt; no static TBS import','new_product_owner_service_IPC':False,'protocol':'same salted unbound HMAC; C# cross-language deterministic wire vectors and software-key endpoint only','integration':'opt-in CMake candidate OFF by default; historical launcher/product/operational pins unchanged','qualified_scope':'14 CTests across Debug/Release; full control-flow mockTPM real console; authority/artifacts/EK/admin role modelled','not_demonstrated':['real TBS/RPC resource cost','native EK knowledge/possession','real 1024 administration','genuine console topology at owner execution','physical TPM generation','trust/revocation','NV custody/persistence','Q4 readback/disposition']}
r['isolated_resource_evidence']=summary
r['resource_observation']='Checkpoint own process table and memory samples; modeled administration is NOT a real budget observation. No deduction of inherited or internal handles. All unsampled/TBS/RPC/kernel peaks UNKNOWN; no capacity grant.'
r['closure_scope']='Original TBS handle checked on close, then CNG providers then artifacts. Uncertain session/context/observer/key close retains original references/dependencies in live host; no kill/retry/guessed FlushContext; host loss UNKNOWN. Opaque Windows/CRT/provider internals not exhaustively observed.'
r['fresh_execution_conditions']=['New exact owner approval; previous approvals used','Windows x64 elevated PowerShell5.1 already open; no UAC bypass','Compiled operator/host/boot pins exact; change requires review and new bytes/hash/approval','Real exact two console clients parent+proof plus distinct console-window process; pseudoconsole divergence DENY','Fresh preobserver own<=124; held native<=128; parent+console<=1024; no global handle-flag edits','Exact exe/request/approval/certificate original-handle pins, current TokenId/AuthId/ModifiedId','Confirmation PROVE_EK_SESSION plus literal request SHA; absence of supplemental receipt does not restore spent approval','One invocation only; no automatic execution, retries, NV/Q4/service/SCM/ACL effects']
(c/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json').write_text(json.dumps(r,indent=2,ensure_ascii=True)+'\n',encoding='ascii')
write('MANUAL-CANDIDATE-PINS.json',{str(q.relative_to(p)).replace('\\','/'):dict(bytes=q.stat().st_size,sha256=sha(q)) for q in c.iterdir() if q.is_file()})
print(json.dumps(summary,indent=2));print('CANDIDATE_REQUEST_SHA256='+sha(c/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json'));print('EXE_SHA256='+sha(exe));print('REQUEST_BYTES='+str((c/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json').stat().st_size))
