# SCM LAB parent ACL focused correction — prepared, not executed

TASK=PACC-PHASE6-SCM-LAB-PARENT-ACL-CORRECTION-1. Date2026-10-23. Active main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. The owner authorizes preparation; actual security descriptor mutation requires manual elevated execution and exact confirmation. No functional validator or accepted ADR changes. See prior root-cause-report and security-descriptor.json in ../scm-lab-parent-acl-root-cause-20261023/.

## Exact target and pre-effect gate

Only C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release, native ID417614100:1966080:79426. Check mode re-opened this same native ID, directory/nonreparse, compared exact owner/group/DACL SDDL to observed preimage and passed. Pinned executable3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb; configurationa44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44 remain unchanged. No re-run of operator eligibility ceremony or SCM binding. Prior screenshot is supplied evidence, not a newly fabricated native receipt.

In Elevated mode the same exact object is reopened without delete sharing, with MAXIMUM_ALLOWED and share0, retained through confirmation, preimage recheck, effect and readback. Identity/reparse check precedes security use; owner/group/DACL must match full pinned SDDL, not only owner. Reparse ancestors deny. Current operator must exactly equal S-1-5-21-785699550-1440458426-1682014700-1108, have factual TokenElevation, nonlimited token, enabled Administrators, no impersonation, and real input/output console. No credential capture, identity swap, automatic elevation in executor, or gate bypass. UAC prompts directly during owner-only Launch mode; cancellation denies.

## Exact proposed change

Owner: S-1-5-21-896877160-3262526087-2538948522-1003 -> S-1-5-21-785699550-1440458426-1682014700-1108. Group unchanged.

Seven ACEs preserved, in same order, same types and trustees. Masks of ACE0 (sandbox group), ACE1 (second observed SID) and ACE5 (Authenticated Users) change0x001301BF ->0x001200A9, removing only write-data/add-file, append/add-subdirectory, write-EA, write-attributes and DELETE. Read/execute/synchronize remain. ACE2 Administrators and ACE3 SYSTEM keep their existing rights (no new FullControl grant); ACE4 Users read/execute unchanged; ACE6 inherit-only Authenticated Users mask unchanged.

To prevent upstream inheritance immediately reintroducing the three unsafe masks, protect ONLY this parent's DACL and preserve its existing entries as explicit ACEs (clear INHERITED_ACE only; OI/CI/IO scope flags preserved). This is a deliberate single-object inheritance change, not an ACL reset or recursive disable. All other resources, ancestor ACLs, existing descendants, SACL and integrity remain outside the write scope. Existing descendants and future inherited grants, especially ACE6, are NOT qualified by this parent correction and may still fail later configuration/image gates. No child creation or operational installation is authorized.

Effect uses supported SetSecurityInfo on the retained handle with OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION|PROTECTED_DACL_SECURITY_INFORMATION only. Documentation explicitly states that a handle opened with MAXIMUM_ALLOWED prevents ACE propagation to children. This is essential: ordinary SetSecurityInfo can otherwise propagate inheritable ACEs automatically. No Set-Acl path mutation, recursive icacls, SetKernelObjectSecurity filesystem workaround, privilege enabling or parent ACL modification is used.

Sources: https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-setsecurityinfo and https://learn.microsoft.com/en-us/windows/win32/secauthz/automatic-propagation-of-inheritable-aces . SetKernelObjectSecurity documentation explicitly discourages filesystem use; it was evaluated and rejected, not called.

## Manual operation and safety

Launcher: tools/qualification/scm-lab-parent-acl-uac.ps1; SHA256 06583bab21b38c67c806ad40c973f5a92244518714eafcb8bd1c513ec7e4b826.

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-lab-parent-acl-uac.ps1" -Mode Launch
```

Owner reviews BEFORE/PROPOSED, exact target native ID, and warning that build tooling may lose write access. Only typing the complete displayed CORRECT_LAB_PARENT <new-attempt-id> line authorizes the effect; Enter alone denies. No password in parameters/logs. ExecutionPolicy Bypass affects only this child process and does not bypass UAC. Fixed Windows PowerShell and script paths, controlled arguments, read-held script/binary/config hash pins; Elevated child must match launcher script hash.

Before effect, manual-intent.json is created exclusively in this documentary evidence directory and flushed, recording the attempt/pre/post descriptors. It is an audit receipt preventing blind repeat, not a product registry/journal or source of authority. If it already exists, STOP, never overwrite/delete it automatically. SetSecurityInfo called at most once. Any error, interruption, partial effect or postimage discrepancy yields STOP, not rollback. manual-result.json records observed postimage when obtainable, effect attempt, close/result status. Failed closure is not reported as success. No retry, ACL rollback, service creation, key/certificate/CURRENT effects. The F10 pause preserves the displayed outcome; F10 only closes the result window.

Exact proposed SDDL is in model-test.log/read-only-check.log. Success requires exact native identity and complete post-readback, plus the model of the unchanged scm_descriptor mask/owner predicate. The script's predicate is corroboration, NOT execution of the compiled native validator. After manual receipt inspection, the separately pinned existing SCM read-only binding must run once to exercise the actual validator. A later CONFIGURATION/IMAGE ACL gate is possible; do not silently expand this parent-only correction. Neither launcher exit0 nor parent predicate success authorizes service installation.

## Verification and review

Exact launcher postimages were submitted via official executor approval before writing; only script changes were approved/applied, no ACL effect executed. Initial negative test exposed StrictMode undefined args; corrected by explicit bounded remaining-arguments parameter. An explicit TokenElevation check was added before positive preparation. Final evidence:

- ModelTest0: rejects unsafe preimage and changed preimage; verifies three reduced masks, preserved Users and inherit-only ACE, proposed owner/write predicate passes; NO native open/mutation.
- Check0: exact native ID and original SDDL, hash pins, no mutation.
- Unexpected argument denied (exit1); no effect.
- Elevated mode wrong script hash denied before target mutation path (exit2); no UAC/positive effect.
- Native C# helper compiles through Add-Type on Windows PowerShell5.1. No product build or binary replacement required. Actual elevated ACL effect and compiled parent validator remain NOT_RUN.

Existing dirty sources, accepted ADRs, binary and configuration preimages verified unchanged. Other LABs and external projects untouched. Prior historical closeout remains preserved rather than rewritten as positive evidence.

## ED11

Qualification is an external PowerShell/.NET host, not a new runtime component or new process-memory admission. At most one target directory handle plus three held read-only script/image/config streams; intent/result stream occurs sequentially, and UAC parent/child hosts coexist in manual mode with their own three pins. One GetSecurityInfo LocalAlloc descriptor at a time, bounded65536bytes; checked LocalFree. Managed descriptor copies/RawSecurityDescriptor, generated binary, SHA/runtime buffers, Add-Type compiler/CLR/PowerShell, UAC/console, provider/kernel/internal stack costs are not fully bounded here and remain UNKNOWN. No material UNKNOWN is zeroed; no product quota increase, no operational ED11 PASS. Close uncertainty stops. No product owner/pin/worker/journal additions.

CURRENT_TASK_COMPLETED=NO
LAB_PARENT_ACL_CORRECTION=PREPARED_EXACT_MANUAL_UAC_OPERATION_NOT_EXECUTED
OWNER_CONFORMANCE=PROPOSED_PASS_OBSERVED_PREIMAGE_STILL_FAIL
DACL_CONFORMANCE=PROPOSED_PASS_FOR_PARENT_ONLY_OBSERVED_PREIMAGE_STILL_FAIL
PARENT_ACL_VALIDATOR=NATIVE_POST_CORRECTION_NOT_RUN_MODEL_PASS
SCM_READONLY_BINDING=STOP_PARENT_ACL_PENDING_MANUAL_EFFECT_AND_NATIVE_REVALIDATION
ACL_CHANGES=NONE_EXECUTOR; LAB_ONLY_IF_MANUALLY_APPROVED
SCM_EFFECTS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=PREEXISTING_CONTENT_HASHES_VERIFIED
FIRST_UNRESOLVED_DIVERGENCE=MANUAL_ELEVATED_EXACT_PARENT_CORRECTION_AND_POST_READBACK_PENDING
NEXT=WINDOWS_VIRTUAL_SERVICE_NATIVE_LAB_LIFECYCLE_AFTER_REMAINING_GATES
