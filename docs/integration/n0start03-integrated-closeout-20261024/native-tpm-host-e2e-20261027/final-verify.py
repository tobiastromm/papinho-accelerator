from pathlib import Path
import hashlib,json,subprocess,csv,re
root=Path.cwd();p=Path('docs/integration/n0start03-integrated-closeout-20261024/native-tpm-host-e2e-20261027');sha=lambda q:hashlib.sha256(Path(q).read_bytes()).hexdigest()
pins=json.loads((p/'TECHNICAL-PINS.json').read_text());assert all(sha(k)==v['sha256'] for k,v in pins.items())
manual=json.loads((p/'MANUAL-CANDIDATE-PINS.json').read_text());assert all(sha(p/k)==v['sha256'] for k,v in manual.items())
assert not (p/'manual-candidate/OWNER-APPROVED-NATIVE-PROOF.json').exists()
assert not (p/'manual-candidate/NATIVE-PROOF-ATTEMPT.ONCE.receipt').exists()
assert sha(p/'manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json')=='2f225db7af94ffb0879d9057f3d9a04f7a787174f93cb1dc274e3cf46686fbf3'
head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip();assert head=='f341985924d29870d8ff627ed37c8a0874ad6469'
for c in ['Debug','Release']:
 full=(p/f'{c}-FINAL-ctest.log').read_text();partial=(p/f'{c}-FINAL-CLOSE-ctest.log').read_text();assert '100% tests passed' in full and 'out of 7' in full and '100% tests passed' in partial and 'out of 3' in partial
 build=(p/f'{c}-integrated-build.log').read_text();assert not re.search(r'warning C\d+|error C\d+|warning LNK',build,re.I)
pe=(p/'PE-IMPORTS-RELEASE.txt').read_text();assert '8664 machine (x64)' in pe and '0 [       0] RVA [size] of COM Descriptor Directory' in pe
assert not re.search(r'^\s+(mscoree|clr|tbs)\.dll\s*$',pe,re.I|re.M)
# Tracked WIP outside task paths is not reverted or folded into the review patch.
subprocess.run(['git','status','--short'],stdout=(p/'WIP-AFTER.txt').open('w',encoding='utf-8'),check=True)
# Record current matrix/live pins and final report hashes separately from product/source pins.
verification={'result':'PASS','head':head,'source_and_exe_pins':'PASS','candidate_request_parser':'PASS nodes450 bytes14032','approval_created':False,'manual_execution':False,'TPM_commands':0,'Q4':'NOT_EXECUTED','ED11':'PARTIAL','E21':'PENDING','Phase6':'PARTIAL','operational_pins':'UNCHANGED_BY_TASK','report_sha256':sha(p/'CONSOLIDATED-CLOSEOUT.md'),'request_sha256':sha(p/'manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json'),'executable_sha256':sha(p/'manual-candidate/NativeTpmProof.exe')}
(p/'FINAL-VERIFICATION.json').write_text(json.dumps(verification,indent=2)+'\n')
files=[q for q in p.rglob('*') if q.is_file() and q.name!='SHA256SUMS.json'];(p/'SHA256SUMS.json').write_text(json.dumps({str(q.relative_to(p)).replace('\\','/'):sha(q) for q in sorted(files)},indent=2)+'\n')
print(json.dumps(verification,indent=2))
