# SCM LAB ACL post-apply verifier correction — 2026-10-23

TASK=PACC-PHASE6-SCM-LAB-ACL-POSTAPPLY-VERIFICATION-1. Active branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91, dirty WIP preserved. Only qualification verifier/runner and evidence/live documentation changed; no product validator, accepted ADR, executable, configuration or historical manual receipt changed. Exact source hunks/preimages submitted and applied through official executor approval; operations.json preserves hashes. No remediation launcher was run in any mode in this task.

## Diagnosis and bounded correction

Historical manual intent expected control0x9004 (D:P); factual manual result control0x9404 (D:PAI). XOR0x0400 only. Owner, group and all ordered ACE bytes matched. The prior full-SDDL string comparison incorrectly rejected this allowed flag variation; manual exit2 and receipts remain preserved, not rewritten into a successful historical execution.

The existing launcher's post-effect condition now calls Test-PaccLabAclPostimage from tools/qualification/scm-lab-acl-postimage.ps1. Pre-effect comparison and all mutation/authorization/one-shot gates remain unchanged. The launcher is NOT to be rerun; existing manual-intent prevents safe repeat independently.

Comparison is structural: both descriptors must have SE_DACL_PROTECTED0x1000, same approved owner, group, seven ACEs, ACL revision, resource-manager control, and all control bits except0x0400. ACE binary comparison requires identical ordered types, principals, masks, inheritance/propagation flags and any opaque bytes. Missing/null DACL, additional ACEs, order changes and all other flag differences reject. Object identity must equal417614100:1966080:79426. It does not sort ACEs, collapse DENY/ALLOW, ignore INHERIT_ONLY flags, translate generic rights or accept altered rights as equivalent. Conservatively preserving order avoids changing effective access where ACE order matters. No SACL is queried or modified; this comparison is explicitly scoped to OWNER/GROUP/DACL, and rejects supplied SACL data rather than pretending to qualify it.

Official semantics: https://learn.microsoft.com/en-us/windows/win32/secauthz/security-descriptor-control documents AUTO_INHERITED0x0400 separately from PROTECTED0x1000. The latter prevents incoming inheritable ACEs modifying the DACL. Accepting0x0400 here does not assert that it is universally irrelevant to inheritance, prove descendant ACLs safe, or relax any other flag. This exception is limited to the protected pinned parent and exact approved ACEs under the owner's explicit task authorization.

## Verification results

selftest.log: 11 cases pass. Protected P and PAI accepted; unprotected DACL, divergent owner, additional ACE, excessive rights, divergent principal, incompatible order, incompatible ACE flags, another control flag and substituted native object rejected. Models operate on descriptor copies only.

native-readonly.log: separate read-only runner (NOT the remediation launcher), with only read-side native APIs, opened same object with READ_CONTROL|READ_ATTRIBUTES and shareREAD; native identity/reparse checks passed; current native owner/group/ordered ACEs matched expected; DACL protected and AUTO_INHERITED present; verifier PASS. Reader descriptor length bounded65536 and checked LocalFree/CloseHandle; no security-write API in runner.

native-parent-validator.log: isolated C89-compatible MSVC /W4 /WX /O2 harness directly includes and calls the UNCHANGED scm_descriptor implementation via its existing PAPACC_SCM_DESCRIPTOR_TEST boundary. Reads real object's descriptor and invokes actual native predicate with approved operator SID from receipt, not a fabricated native token or positive authorization. Result PARENT_ACL_VALIDATOR=PASS, exact same identity, HANDLE_CLOSED=1. Product executable and validator source unchanged. Initial harness command lacked the existing BearSSL include directory and placed library after /TC without /link; corrected build recipe uses existing capsule header and explicit linker boundary. Final build is warning-free; no new cryptographic implementation or product rebuild.

Do not confuse native predicate PASS with full native administrative invocation. No operator ceremony was replayed merely to repeat already passed operator/context evidence. Full SCM read-only binding remains pending eligible-host invocation; it may reach a later configuration/image gate. Existing command, solely read-only and already pinned:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-protected-binding-readonly.ps1"
```

Do NOT run scm-lab-parent-acl-uac.ps1 again. Do not rerun correction, delete intent, reset ACL or retry after UNKNOWN. Even full read-only binding0 would authorize preparation only; CreateService and all operational authority remain DENY.

## Preservation/resources

No SetSecurityInfo/SetNamedSecurityInfo/icacls/ACL mutation, UAC launch, account/service/key changes, bootstrap, certificate, CURRENT, network, quota, PST/Browser/wire or production0.6.4 change. Read-only native validator/runner use one target handle and one native descriptor at a time; predicate has existing fixed SID scratch; existing C stack/runtime calls reused. PowerShell/.NET/CLR, compiler and kernel costs are not a full bounded product ED11 proof; UNKNOWN remains UNKNOWN, total PARTIAL. No second owner/worker/pin/journal. Existing manual attempt and result preserved with hashes; new report supersedes only the earlier pending-verification state, not history.

CURRENT_TASK_COMPLETED=YES
ACL_POSTAPPLY_VERIFIER=CORRECTED_11_TESTS_PASS_NATIVE_READONLY_PASS
LAB_OWNER=EXPECTED_OPERATOR_SID_CONFIRMED
LAB_DACL_PROTECTED=YES
LAB_ACE_CONFORMANCE=SEVEN_ORDERED_ACES_EXACT_PASS
AUTO_INHERITED_FLAG=ACCEPTED_IF_SEMANTICALLY_EQUIVALENT
PARENT_ACL_VALIDATOR=PASS_EXISTING_NATIVE_FUNCTION_REAL_OBJECT
SCM_READONLY_BINDING=FULL_INVOCATION_PENDING_ELIGIBLE_HOST_OTHER_GATES_NOT_QUALIFIED
ACL_MUTATIONS=NONE
SCM_EFFECTS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_INTENTIONAL_VERIFIER_AND_DOCUMENTATION_CHANGES_ONLY
FIRST_UNRESOLVED_DIVERGENCE=FULL_READONLY_SCM_BINDING_AND_SUBSEQUENT_RESOURCE_AUTHORITY_GATES_PENDING
NEXT=WINDOWS_VIRTUAL_SERVICE_NATIVE_LAB_LIFECYCLE_AFTER_ALL_GATES
