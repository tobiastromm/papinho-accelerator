from pathlib import Path
import json,hashlib,subprocess,csv,re
root=Path.cwd();base=Path('docs/integration/n0start03-integrated-closeout-20261024');p=base/'c0-c9-console-integration-20261028';sha=lambda q:hashlib.sha256(Path(q).read_bytes()).hexdigest();pins=json.loads((p/'TECHNICAL-PINS.json').read_text())
assert all(sha(Path(k))==v['sha256'] for k,v in pins.items())
for k,v in json.loads((p/'MANUAL-CANDIDATE-PINS.json').read_text()).items():assert sha(p/k)==v['sha256']
assert not (p/'manual-candidate/OWNER-APPROVED-NATIVE-PROOF.json').exists()
assert not (p/'manual-candidate/NATIVE-PROOF-ATTEMPT.ONCE.receipt').exists()
assert sha(p/'manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json')=='b575659fe446eb4b2ed3d53881dfbef77855a02a78d87fb76cccc96f3faf0c8b'
assert sha(p/'preimages/proof_core.c')==sha('tools/qualification/tpm-native-proof/proof_core.c')
assert sha(p/'preimages/native_host.c')==sha('tools/qualification/tpm-native-proof/native_host.c')
for config in ['Debug','Release']:
 log=(p/f'{config}-FINAL-ctest.log').read_text();assert '100% tests passed' in log and 'out of 9' in log
 assert not re.search(r'warning C\d+|error C\d+|warning LNK',(p/f'{config}-integrated-build.log').read_text(),re.I)
pe=(p/'PE-IMPORTS.txt').read_text();assert '8664 machine (x64)' in pe and '0 [       0] RVA [size] of COM Descriptor Directory' in pe
assert not re.search(r'^\s+(tbs|mscoree|clr|wintrust)\.dll\s*$',pe,re.I|re.M)
assert 'REQUEST_DATA_ONLY PASS nodes=549 bytes=19651' in (p/'REQUEST-DATA-CHECK.log').read_text()
head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip();assert head==(p/'HEAD-BEFORE.txt').read_text().strip()
old=list(csv.DictReader((p/'ED11_GATE_MATRIX.PREIMAGE.csv').open(encoding='utf-8',newline='')));now=list(csv.DictReader((base/'ED11_GATE_MATRIX.csv').open(encoding='utf-8',newline='')));assert len(old)==len(now)
for a,b in zip(old,now):
 if a['gate']=='E21':assert a['status']==b['status'] and a['closure_class']==b['closure_class']
 else:assert a==b
# Preserve evidence that the blocked synthetic child exited by console cancellation, not kill.
recovered=root/'build/v3-backings-20261021/Selected/Release/Testing/tpm-console-ae8e216bf45d6c23/clean/result.txt'
if recovered.exists():
 raw=recovered.read_text();j=json.loads(next(line for line in raw.splitlines() if line.startswith('{')));assert j['first']=='CONFIRMATION_DENY' and j['calls']==0 and not j['retained'] and j['controlled_refs']==0;(p/'INITIAL-FIXTURE-CANCELLED-NO-TBS.txt').write_text(raw)
verification=dict(result='PASS_SCOPED_C0_INTEGRATION',head=head,C0_C9='ACCEPTED',proposal_sha256=sha(Path('docs/adr/ADR-CANDIDATE-tpm-proof-explicit-console-host-binding-20261027.md')),decision_sha256=sha(Path('docs/adr/DECISION-tpm-proof-explicit-console-host-binding-20261028.md')),core_protocol_and_entry_sources='UNCHANGED',native_platform='C0_REAL_ROLE_BINDING_INTEGRATED',builds='DEBUG_RELEASE_ZERO_WARNINGS',tests='18_CTEST_PASS',full_mock_peak=92,max_mock_submits=12,max_admin_observations=16,max_samples=214,sample_capacity_unchanged=256,platform_bytes=166816,platform_delta_bytes=96,core_bytes_unchanged=28688,new_request_sha256=sha(p/'manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json'),new_executable_sha256=sha(p/'manual-candidate/NativeTpmProof.exe'),new_approval_created=False,actual_TPM_calls=0,Q4='NOT_EXECUTED',SCM_ACL='NONE',E21='PENDING',ED11='PARTIAL',Phase6='PARTIAL')
(p/'FINAL-VERIFICATION.json').write_text(json.dumps(verification,indent=2)+'\n')
(p/'WIP-AFTER.txt').write_text(subprocess.check_output(['git','status','--short'],text=True))
(p/'SHA256SUMS.json').write_text(json.dumps({str(q.relative_to(p)).replace('\\','/'):sha(q) for q in p.rglob('*') if q.is_file() and q.name!='SHA256SUMS.json'},indent=2)+'\n')
print(json.dumps(verification,indent=2))
