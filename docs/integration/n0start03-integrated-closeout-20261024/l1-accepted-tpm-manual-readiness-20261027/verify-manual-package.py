from pathlib import Path
import hashlib,json
root=Path.cwd();pkg=Path(__file__).resolve().parent
old=root/'docs/integration/n0start03-integrated-closeout-20261024/tpm-l1-native-proof-and-parallel-closure-20261026'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
checks=[]
def pin(p,h):
 a=sha(p);checks.append(dict(path=str(p.relative_to(root)),expected=h,actual=a,pass_check=a==h));assert a==h,str(p)
pin(root/'docs/adr/ADR-CANDIDATE-q4-l1-single-root-component-revision1-20261026.md','335dd1f829d959220ab1e9e0c2038cae9209ec5b9c205e52a15952d55686d5e3')
pin(root/'docs/adr/DECISION-q4-l1-single-root-component-20261027.md','40a746438fb3dfb75ac447964227aff25293dd49f4f1c08ad1d65cea90e3a1cd')
pin(old/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json','357c33ceafb7e5a33c69c87f7a70e0db685a16547e0e7fe8741b33e0d07d77a1')
pin(old/'NativeProofRelease.exe','ffd02de63a9b8e95741707532b6e81b33307f7745741232ae91f4c35d4148334')
pin(old/'NativeProofHost.CANDIDATE.cs','354b980bd3b86e2d9dd77e61a4072f8611c2f5fcb00ce3f8872a4c26deea4a4c')
pin(old/'OWNER-APPROVED-NATIVE-PROOF.json','d7417145a73a2b8ec03fd4b57df23ecc10b045178d02eac543557be3fa76a146')
req=json.loads((old/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json').read_text(encoding='utf-8-sig'))
for name,h in req['profile_sources'].items():pin(old/name,h)
pin(old/'ek-certificate.PUBLIC.der',req['observed_EK']['certificate_der_sha256'])
a=json.loads((old/'OWNER-APPROVED-NATIVE-PROOF.json').read_text(encoding='utf-8-sig'))
assert a['kind']=='OWNER_APPROVAL_TPM_SALTED_PROOF_ONLY' and a['request_sha256']==sha(old/'NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json') and a['executable_sha256']==sha(old/'NativeProofRelease.exe')
assert all(a[k] is True for k in ['one_manual_execution','ack_unknown_revocation','ack_no_hardware_admission','ack_internal_tpm_accounting_unknown'])
assert not (old/'NATIVE-PROOF-ATTEMPT.ONCE.receipt').exists(),'STOP existing receipt, no retry'
reuse=json.loads((old/'E17-E18-REUSE-VERIFICATION.json').read_text(encoding='utf-8-sig'))
for x in reuse:pin(root/x['path'],x['expected'])
# No invocation of the proof executable or TPM APIs occurs here.
(pkg/'MANUAL-PACKAGE-VERIFICATION.json').write_text(json.dumps(dict(checks=checks,checks_passed=len(checks),one_shot_receipt='ABSENT_AT_REVIEW_NOT_RESERVATION',native_mode='NOT_EXECUTED',fresh_guards='REQUIRED_AT_MANUAL_RUN'),indent=2),encoding='utf-8')
print('PASS',len(checks),'pins; no TPM execution; one-shot receipt absent at review')
