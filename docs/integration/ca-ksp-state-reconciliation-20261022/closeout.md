# CA KSP state reconciliation — 2026-10-22

Task PACC-PHASE6-CA-KSP-STATE-RECONCILIATION-1. Preparation only; no new key generation, KSP query, deletion, UAC launch, certificate, bootstrap, CURRENT or network operation occurred in this task.

## Evidence and conclusion

The original MACHINE key in Microsoft Software Key Storage Provider remains UNKNOWN:
`PapinhoAccelerator.CA.cb87632463bb48dd8a34fa8ea6f3911b.030fa34dd44f41e8b143b9da09532cf2`.
CreatePersistedKey was successful; the first native failure was FinalizeKey(SILENT), NTE_PERM 0x80090010, function1/stage20/API7. Earlier property setters were successful. Finalization was attempted, not confirmed. Subsequent confirmed handle closure does not establish deletion of a persisted object.

The owner describes the original context as limited. Current filtered-context evidence supports a MACHINE write-access barrier, but the original token statistics were not recorded and this does not prove the unique cause of the historical Finalize failure. The elevated observation used the expected Tobias SID, full elevated token, enabled Administrators and a real console. OpenKey(MACHINE|SILENT) returned NTE_BAD_KEYSET 0x80090016, function2/stage12/API3. Provider close was confirmed, no key acquired. This is a successfully completed read-only diagnostic, not successful CA initialization or absolute proof of absence.

Verified receipt: `../ca-ncrypt-finalize-nte-perm-20261022/manual-results/607304b978194007a40ab1132474878a.json`, SHA-256 `6f3dbbff73beda7f17ad393fb583b521437cc3f760a8f66b6cb97a7615dbb019`.
See the prior [manual elevated observation](../ca-ncrypt-finalize-nte-perm-20261022/manual-elevated-observation-20261022.md) and [closeout](../ca-ncrypt-finalize-nte-perm-20261022/closeout.md). No nondiscriminating query was repeated. No defect in the existing creation sequence is proven; no product fix, ACL, provider, scope or parameter change is justified.

## Conditional next experiment

[Prepared identity and criteria](new-lab-plan.json) and [manual operation gate](manual-operation.md) preserve the old name and use a distinct disposable LAB name. The standalone harness reuses the existing CA owner unchanged. It has no root, worker, certificate or signing path. It checks the actual elevated operator, records token statistics before creating, uses no overwrite, validates only public properties, exercises the existing borrow guard and deletes only its own newly initialized and validated object after the borrow is returned. A failed or uncertain operation stops without retry/reopen. No private material is inspected or exported.

The intentionally denied close-while-borrowed test may populate the sticky diagnostic first-error field after successful initialization. Its expected NTE_BAD_KEY_STATE is reported separately; it is not a Finalize failure. Creation diagnostics are recorded before that negative test. The harness is not an authorization source: its effectful argument must not be run until the separate exact native-operation approval and manual preconditions below are satisfied.

## ED11

This adds a standalone LAB executable, not product root/worker layout. At most one new attempt is prepared. Previous UNKNOWN namespace debt is retained independently; successful old handle closure does not remove it. Future new retention could therefore coexist with the previous unknown persisted object. No third attempt, automatic rename or retry is authorized.

Caller-controlled static CA owner is 480 bytes in the current x64 layout, with a 4-byte Stop flag and 4-byte holder; fixed identifiers/name and backend code/data are additional. Existing actor/token/SID and public validation buffers, nested stack, CRT, console/PowerShell, provider/kernel allocations and persisted key resource costs are not zero. Full transitive byte bound and operational margin remain unproven; provider/kernel and retention costs remain UNKNOWN. The harness stops after uncertain cleanup, never frees by timeout, and process exit is not proof of key destruction. No complete ED11 PASS or quota increase is claimed.

## Validation and preservation

MSVC 19.51 x64 Debug (/Od /Zi /MDd /RTC1) and Release (/O2 /MD), /TC /W4 /WX: both builds passed without warnings. Only next-lab.c and the existing server_ca_key_win32.c were compiled, linked with ncrypt.lib/advapi32.lib into a fresh build directory. Six safe branch tests passed: --describe returns0, no args and --invalid return2, for both configurations. These branches precede token/KSP acquisition. The effectful --execute-new-lab branch was NOT run; no native generation/cleanup success is claimed. VC6 and other targets were not qualified here.

Branch main, HEAD `913c90cf2328f7939d8f5b927640601903ac6e91`; all 1324 baseline WIP file hashes were unchanged before this documentation/live append. Source/header backend hashes remain those in artifacts.json. Four historical LABs and prior receipts are preserved; only a new standalone build namespace was used. PST, Browser, PACC wire, production0.6.4 and quotas unchanged. No clone/worktree, commit, push, tag or release.

## Result

CURRENT_TASK_COMPLETED=YES
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
ORIGINAL_FINALIZE_CAUSE=UNRESOLVED_NTE_PERM_NOT_PROVEN_BY_ELEVATED_OPEN_FAILURE
ELEVATED_DIAGNOSTIC=PASS_READ_ONLY
NEW_LAB_IDENTITY=PREPARED
NEW_LAB_EXECUTION=NOT_RUN
CA_CUSTODY_INITIALIZATION=DENY
INITIAL_ROOT_PREPARATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=PREVIOUS_PERSISTED_KEY_DISPOSITION_AND_ORIGINAL_FINALIZE_CAUSE_UNPROVEN
NEXT=CA_CUSTODY_ELEVATED_LAB_FINALIZATION_AFTER_EXPLICIT_APPROVAL
