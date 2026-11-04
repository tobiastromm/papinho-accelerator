from pathlib import Path
import json,csv,hashlib,shutil,re,subprocess,difflib
root=Path.cwd();base=Path('docs/integration/n0start03-integrated-closeout-20261024');p=base/'c0-c9-console-integration-20261028';sha=lambda q:hashlib.sha256(Path(q).read_bytes()).hexdigest();proposal=Path('docs/adr/ADR-CANDIDATE-tpm-proof-explicit-console-host-binding-20261027.md');decision=Path('docs/adr/DECISION-tpm-proof-explicit-console-host-binding-20261028.md')
assert sha(proposal)=='7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985'
# Summarize final process/resource receipts, not sampled peaks as universal caps.
summary=[];ledger=[];native=[]
for config in ['Debug','Release']:
 log=(p/f'{config}-FINAL-ctest.log').read_text();assert '100% tests passed' in log and 'out of 9' in log
 assert not re.search(r'warning C\d+|error C\d+', (p/f'{config}-integrated-build.log').read_text(), re.I)
 for mode in ['clean','maximum','wrong','escape']:
  j=json.loads(next(x for x in (p/f'{config}-{mode}-FINAL.txt').read_text().splitlines() if x.startswith('{')))
  assert j['transport']=='MOCK_ENDPOINT_NO_TPM' and j['controlled_refs']==0 and not j['retained']
  summary.append(dict(configuration=config,case=mode,first=j['first'],sampled_peak=j['sampled_peak'],samples=len(j['samples']),administrative_observations=len(j['administrative_samples']),direct_mock_submits=j['calls'],proof_model=j['proof'],refs=j['controlled_refs'],retained=j['retained'],file_mask=j['role_file_verification_mask_at_last_admin_attempt'],scope='Production C0 native identity/class49/census/metrics; token authority and parent-file claim modelled; real auxiliary/ABI file profiles; no actual TBS'))
  ledger.extend(dict(configuration=config,case=mode,**s) for s in j['samples'])
 d=root/f'build/v3-backings-20261021/Selected/{config}/Testing';latest=max(d.glob('tpm-admin-*'),key=lambda q:q.stat().st_mtime_ns)
 for name in ['classic','pseudo','concurrent0','concurrent1']:
  raw=(latest/f'{name}.json').read_text();j=json.loads(next(line for line in raw.splitlines() if line.startswith('{')));assert j['first']=='NONE' and j['controlled_refs']==0 and not j['retained']
  (p/f'{config}-native-{name}.json').write_text(raw)
  native.append(dict(configuration=config,case=name,peak=j['sampled_peak'],parent=j['topology_parent'],auxiliary=j['topology_console'],window=j['window_owner_metadata'],admin_samples=j['administrative_samples'],scope='Real native process/console graph and production hp_admin body; role authority and parent image modelled explicitly; not operational admission'))
(p/'RESOURCE-RESULTS.json').write_text(json.dumps(summary,indent=2)+'\n');(p/'NATIVE-ADMIN-GRAPH-RESULTS.json').write_text(json.dumps(native,indent=2)+'\n')
with (p/'PHASE-LEDGER.csv').open('w',encoding='utf-8',newline='') as f:w=csv.DictWriter(f,fieldnames=list(ledger[0]));w.writeheader();w.writerows(ledger)
# Layout list includes stack probes. Own frame only, not reserved/transitive memory.
frames=[]
for q in (p/'layout').glob('*.asm'):
 current=None;eax=None
 for line in q.read_text(errors='replace').splitlines():
  m=re.match(r'^(\w+)\s+PROC\b',line)
  if m:current=m.group(1);eax=None
  m=re.match(r'\s+mov\s+eax,\s*(\d+)\b',line)
  if m:eax=int(m.group(1))
  m=re.match(r'\s+sub\s+rsp,\s*(\d+|rax)\b',line)
  if m and current:frames.append(dict(source=q.stem,function=current,rsp_sub_bytes=eax if m.group(1)=='rax' else int(m.group(1)),scope='Own Release RSP sub only including __chkstk; excludes pushes/callees/internals/reservation'))
(p/'COMPILED-STACK-FRAMES.json').write_text(json.dumps(frames,indent=2)+'\n')
# Preserve byte-exact historical host, requests and approvals; reuse independent sources.
history=[]
old=base/'native-tpm-host-e2e-20261027'
for name,h in json.loads((old/'OWNER-APPROVED-PACKAGE-PINS.json').read_text()).items():
 actual=sha(old/name);assert actual==h;history.append(dict(path=str(old/name),sha256=actual,unchanged=True))
for row in json.loads((base/'tpm-l1-native-proof-and-parallel-closure-20261026/E17-E18-REUSE-VERIFICATION.json').read_text()):
 actual=sha(root/row['path']);assert actual==row['expected'];history.append(dict(path=row['path'],sha256=actual,unchanged=True))
(p/'HISTORY-AND-PARALLEL-REUSE.json').write_text(json.dumps(history,indent=2)+'\n')
# Exact prospective technical source/artifact set, no existing operational pin update.
files=[q for q in Path('tools/qualification/tpm-native-proof').rglob('*') if q.is_file() and '__pycache__' not in str(q)];files += [Path('CMakeLists.txt'),proposal,decision]
for c in ['Debug','Release']:
 for n in ['proof','core_test','platform_test','artifact_test','e2e_fixture','admin_fixture']:files.append(Path(f'build/v3-backings-20261021/Selected/{c}/papacc_tpm_native_{n}.exe'))
pins={str(q).replace('\\','/'):dict(bytes=q.stat().st_size,sha256=sha(q)) for q in files};(p/'TECHNICAL-PINS.json').write_text(json.dumps(pins,indent=2)+'\n')
# New inert manual candidate, no approved file and no executable manual command.
c=p/'manual-candidate';c.mkdir(exist_ok=False);exe=Path('build/v3-backings-20261021/Selected/Release/papacc_tpm_native_proof.exe');shutil.copyfile(exe,c/'NativeTpmProof.exe');shutil.copyfile('tools/qualification/tpm-native-proof/fixtures/ek-certificate.PUBLIC.der',c/'ek-certificate.PUBLIC.der')
r=json.loads((old/'manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json').read_text());r.update(status='C0_C9_QUALIFIED_OFFLINE_NOT_APPROVED_NOT_EXECUTED',candidate_operation='TPMPROOF-C0C9-20261028-CANDIDATE-INERT',executable_sha256=sha(exe),source_sha256=sha(Path('tools/qualification/tpm-native-proof/native_host.c')),console_binding_revision='C0-C9',console_binding_proposal_sha256=sha(proposal),console_binding_decision_sha256=sha(decision))
r['profile_sources']={name:v['sha256'] for name,v in pins.items() if Path(name).suffix in ['.c','.h','.inc','.txt','.cmake','.md']}
r['prior_attempt_disposition']={'request_sha256':'2f225db7af94ffb0879d9057f3d9a04f7a787174f93cb1dc274e3cf46686fbf3','approval_sha256':'4a5f2e001feb29a0607333c40dd46e413b3e75a3957b1c527d7090b2eab1376c','state':'USED_ONE_INVOCATION_TOPOLOGY_DENIED_NOT_REUSABLE','own_count':94,'TBS_calls':0,'context':'NOT_OPENED','retained':False,'no_retry':True}
r['administrative_resource_gate']['topology']='exact two current inherited console clients: actual native child and actual parent; distinct original auxiliary discovered by matching repeated class49 profile; HWND metadata may alias either client. No static historical PID seed.'
r['administrative_resource_gate']['identity']='original process handles, GetProcessId, creation/liveness before and after; same approved primary SID/elevation/session/logon and role LUID continuity; pinned Windows file volume/ID/size/content/descriptor, same native ABI file; no mapped-image/authenticated association assertion'
r['administrative_resource_gate']['reentrancy']='owner-thread only and BUSY over complete helper lifetime; other thread denied with no state mutation; reentry freezes before new acquisition'
r['administrative_resource_gate']['four_role_limit']='Preserved applicable role/census obligations; only parent/native/one real helper in this standalone profile. Additional operation participant denies; outside baseline not hidden or credited.'
r['owned_native_resources']['observation_storage']='TP_OWNER28688 unchanged; TP_PLATFORM166816 delta96 vs original native host; includes new temporary file handle, token continuity/binding scalars and BUSY state; not total runtime/transitive memory'
r['one_use_admin_receipt']='CREATE_NEW local NATIVE-PROOF-ATTEMPT.ONCE.receipt before TBS; administrative supplement only, not durable Q4 ledger. One invocation uses approval even if guard denies before marker; no retry.'
r['runtime_bindings']='current original PID/creation and primary-token LUIDs of native/actual parent/auxiliary, request/approval hashes and EK Name. Parent and helper identities resolved live, never copied from historical console. Fresh salt/nonces remain only in memory.'
r['console_role_file_pins']=json.loads((p/'ROLE-FILE-PINS.json').read_text())
r['console_risk']='C0-C9 C4 accepted only for standalone mathematical experiment: complete cryptographic console association and mapped-image provenance UNPROVEN. File/security/identity/token/lifetime/resource obligations not waived. No LAB N7 transfer, hardware trust or operational budget grant.'
r['isolated_resource_evidence']=summary
r['implementation_change'].update(protocol='UNMODIFIED salted unbound HMAC; original core source/vectors unchanged',integration='C0-C9 accepted, same canonical standalone native host guard and existing owner; no launcher/service/IPC changes',qualified_scope='18 CTests Debug/Release including production C0 body in real native classic/ConPTY/concurrent graphs and original Windows file/descriptor checks. Authority of fixture role tokens/parent image explicitly modelled; no actual TBS.',not_demonstrated=['actual TBS/RPC costs and native EK possession','future fresh actual operator/helper 1024 and native128','unsampled peak/kernel/transitive resources','hardware generation/revocation/custody/NV/Q4'])
r['resource_observation']='Native mock-endpoint full flow sampled92 both configs; maximum12Submit fixture214/256sample slots and16admin observations, including modeled module boundary and file/token lifetime. Role authority/parent image modelled; no real TBS/operational capacity demonstrated. Runtime always rechecks124/128/1024. Unsampled/internal peaks UNKNOWN.'
r['fresh_execution_conditions']=[
 'New exact owner approval only; 4a5f and all previous approvals used; one invocation, no automatic execution or retry',
 'Windows PowerShell5.1 x64 already elevated; actual caller parent with approved SID, token LUID continuity/no impersonation and current host/boot237900 exact pins',
 'Inherited interactive console has exactly actual parent and proof as clients; repeated original parent/native class49 point to one distinct qualified original auxiliary. Historical17204/20976/13944 identities not reused as pins',
 'Windows original image volume/FileID/size/hash/security descriptor and ntdll ABI pins exact; no mapped-image/console authentication beyond C4 accepted risk',
 'Fresh preobserver own<=124, held own<=128, unique actual parent+auxiliary<=1024, all applicable roles/freshness/headroom/UNKNOWN obligations; no credits',
 'Exact file pins for native exe/request/new approval/public certificate. Runtime confirmation includes literal requestSHA and live parentPID creation auxiliaryPID creation. Do not use historical confirmation',
 'Any divergence or uncertain close freezes acquisitions/commands and retains original dependencies. Host loss UNKNOWN; no kill/FlushContext/cleanup/rearming',
 'No NV/CreatePrimary/AK/hierarchy/Clear/Q4/LAB/Start/Stop/SCM/ACL/CURRENT/signature/recovery authority']
r['approval_conditions']='New separate exact one-manual-invocation approval under C0-C9 accepted decision, with revocation/internal accounting UNKNOWN/no hardware admission acknowledged. Normative acceptance is not this approval.'
r['success']='same qualified public EK/Name before and after, fresh salted challenge valid HMAC and authenticated continue-clear session end, factual TBS close; actual console role/identity/security/resource checks never waived'
r['next_manual_confirmation']='PROVE_EK_SESSION <literal request SHA> <live actual parent PID> <parent creation FILETIME> <live auxiliary PID> <auxiliary creation FILETIME>; exact full line displayed at runtime, values not fabricated during preparation'
r['authorization_required']='New specific operational owner decision for this exact request. C0-C9 normative acceptance does not permit execution.'
q=c/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json';q.write_text(json.dumps(r,indent=2,ensure_ascii=True)+'\n',encoding='ascii');assert q.stat().st_size<32768
(p/'MANUAL-CANDIDATE-PINS.json').write_text(json.dumps({str(f.relative_to(p)).replace('\\','/'):dict(bytes=f.stat().st_size,sha256=sha(f)) for f in c.iterdir()},indent=2)+'\n')
# Reviewable patch is relative to task preimages, not unrelated historical HEAD WIP.
diffs=[]
for pre in (p/'preimages').iterdir():
 current=Path('CMakeLists.txt') if pre.name=='CMakeLists.txt' else Path('tools/qualification/tpm-native-proof')/pre.name
 if current.exists() and current.read_bytes()!=pre.read_bytes():diffs.append(''.join(difflib.unified_diff(pre.read_text().splitlines(True),current.read_text().splitlines(True),fromfile=str(current)+'.before-task',tofile=str(current))))
(p/'SCOPED-WIP-RELATIVE.patch').write_text('\n'.join(diffs),encoding='utf-8')
print('EXE='+sha(exe));print('REQUEST='+sha(q));print('REQUEST_BYTES='+str(q.stat().st_size));print('DECISION='+sha(decision));print('ROLE_FRAMES',[(x['function'],x['rsp_sub_bytes']) for x in frames if x['function'] in ['hp_admin_current','hp_admin','hp_role_file','hp_role_token','wmain']])
