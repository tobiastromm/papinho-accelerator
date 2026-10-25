# Windows UAC read-only qualification launcher — 2026-10-22

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-UAC-LAUNCHER-1.
Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
CURRENT_TASK_COMPLETED=YES, scoped to launcher preparation and negative testing. POSITIVE_NATIVE_QUALIFICATION=PREPARED_NOT_EXECUTED.

## Delivery and authorization

Launcher: tools/qualification/personal-first-install-uac.ps1; fixed disabled/no-listener plan: personal-first-install-readonly.conf in the same directory. Exact initial two files and subsequent Bypass/status-validation hunk passed official executor review and pre/postimage checks. See operations.json, bypass-operations.json, bypass.patch and final-source-hashes.json. No functional C/backend/worker or accepted contract changed.

The initial unsigned negative script was refused by Windows PowerShell under LocalMachine=AllSigned before any code/UAC executed (UnauthorizedAccess). The owner then explicitly instructed use of Bypass. Qualification commands and the manually activated UAC child now use -ExecutionPolicy Bypass per process only. No Set-ExecutionPolicy, UAC policy, ACL, antivirus or other protection was modified. Final policy observation still reports LocalMachine=AllSigned, CurrentUser/Process/UserPolicy/MachinePolicy=Undefined. This is recorded owner authorization, not an automatic exception or a persistent policy change.

## Fixed execution and identity boundary

Absolute canonical Release executable is pinned to06ba7240678de8fff23183b551621d4932a391468ac022083d10237cee55fd70. Adjacent libcrypto/libssl and source plan are separately pinned. PE must be x64 PE32+ console. Files are held for read with write/delete sharing denied, including script across UAC. Both launcher stages revalidate pins; expected script hash binds the child to the same reviewed parent invocation. No executable, command, source or parent override exists. Noncanonical/reparse paths deny. Only OS PowerShell modules are imported by absolute path, with NoProfile and fixed working directories. Windows PowerShell5.1 x64 is the supported prepared host; other hosts deny. See pinned-artifact-hashes.json.

Default Check never requests UAC or starts the CLI, and returns2 intentionally. Launch requires actual standard console input/output before reaching the native Start-Process -Verb RunAs, with a new visible window. Only the owner may manually activate it; no positive Launch was run by the executor. UAC alone owns credentials/consent. There is no password parameter/Read-Host/password variable, credential file, alternate elevation service, scheduled task or unelevated fallback.

The elevated stage observes effective SID/name, enabled administrator eligibility, elevation type, interactive/service membership, impersonation absence, active console session and actual console handles. It never equates the original and elevated user. Different SID requires explicit selection of the effective operator; this selection does not substitute the CLI's own exact F0–F9 confirmation. A memory-only token-statistics witness plus SID, eligibility and console are rechecked before/after the CLI. These facts are eligibility/context observations, not legal ownership or an OS approval of the plan. Native CLI independently binds its own actual operator/parent/plan/attempt and worker.

Fixed CLI arguments request only NEW_INSTALL with the disabled plan, existing Release parent and hypothetical personal-readonly-target-not-created leaf. No CA_INITIALIZE, child creation, key generation, certificate, signature or CURRENT action is requested. stdin/stdout stay console-attached; only stderr status is read into memory and strictly matched against exactly two allowed nonsecret lines. No plan/nonce/password/key/secret transcript, screenshot or raw diagnostic log is saved. Confirmation is entered manually in the CLI, never injected.

A launcher success means only exact confirmation/read-only worker status, CLOSED and an after-run factual not-found observation for the child. The CLI's expected final code remains2 (DENY). Unknown/error/unexpected output or context yields DENY. UAC cancellation1223 is classified without fallback. RETAINED_UNKNOWN never permits automatic retry or a cleanup/rollback inference from process exit. The wrapper does not force-kill or terminate the native owner. Child absence uses GetFileAttributes with exact not-found errors; access failure is UNKNOWN/DENY, not absence. No global absence/history or CA resource cleanup is claimed.

## Tests and operational limits

Final delivered harness: eight negative native process cases PASS and17 helper checks PASS, all without elevation. Cases include default Check, Launch without console denying BEFORE UAC, missing elevated binding, unexpected internal arguments, invalid mode, SID injection syntax, divergent script hash and wrong script location. Helpers cover pin mismatch, read-only sharing, invalid PE/relative path, identity/token/context divergence, strict confirmation/retained/error parsing and1223 versus other errors. Model identity/error cases do not simulate a factual eligible operator or actual UAC cancellation. Initial helper reliance on auto-loaded Get-FileHash failed in a Python-spawned host; the isolated harness was corrected to use a fixed independent fixture digest and explicit OS module import. Final reproducible tests pass; launcher itself already used explicit OS imports and .NET hashing.

The fixed plan validates structurally: SOURCE=VALIDATED; MODEL_BOUND=NO; RESOLVED=NO; CURRENT=NOT_READ; APPLY=DENY. Check's real token remains ineligible and console absent in the executor. No UAC prompt, elevation, positive native ceremony, native worker submission, target creation, CA/key/certificate, CURRENT, system-clock change or network was executed. No manual-results directory/receipt existed after these negative tests. Builds NOT_RUN: no compiled product source changed; PowerShell parser/helper/runtime checks replace no product qualification.

PowerShell/CLR/UAC/console/streams and SHA256 provider resources are qualification-host costs, not zero-cost product additions. Up to two launcher processes may coexist with the CLI during manual execution, each holding five input streams; system UAC/provider/kernel/full stack/host headroom remain UNKNOWN. Fixed product census1,799,600 remains historical unchanged; no ED11 operational PASS, quota change or3MiB selection. ED11_TOTAL_STATUS=PARTIAL.

## Result and handoff

UAC_LAUNCHER=PREPARED_MANUAL_ONLY
EXECUTABLE_IDENTITY=ABSOLUTE_PATH_SHA256_PE_X64_CONSOLE_PINNED
INTERACTIVE_CONSOLE=REQUIRED_NO_CONSOLE_NEGATIVE_PASS
ELEVATED_OPERATOR_BINDING=IMPLEMENTED_POSITIVE_NOT_EXECUTED
POSITIVE_NATIVE_QUALIFICATION=PREPARED_NOT_EXECUTED
BOOTSTRAP_EXECUTION=NOT_RUN
TARGET_CHILD_CREATION=NOT_RUN
CA_CUSTODY=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=OWNER_MANUAL_UAC_POSITIVE_EXECUTION_PENDING
NEXT=OWNER_EXECUTES_ELEVATED_READ_ONLY_QUALIFICATION

[Manual command, pins, expected results and Stop handling](README.md). Existing WIP/four LABs retained; no clone/worktree/commit/push/tag/release. Diagnostic receipts are intentional evidence files, never installation state, an approval journal or an authority store.

## Manual follow-up — result visibility, 2026-10-22

Owner manual receipts establish eligible=true and interactive_console=true, with the pinned binary and DENIED_OR_CANCELLED, exit2, target absent. Presentation is visible in the owner screenshot; exact confirmation/worker verification is not established. This supersedes PREPARED_NOT_EXECUTED as the current attempt state: ATTEMPTED_DENIED, not positively qualified. No automatic elevation was executed by the agent.

The owner reported that the window closed immediately after Enter. The old single ReadKey could dismiss on queued key events; caught errors also had no final pause. Reviewed pause.patch now flushes native console input where available and requires F10 in a final wait, including caught errors. Enter and other residual keys cannot dismiss the normal wait. Redirected tests never wait. No confirmation semantics, gates or binary pins changed. Final eight negative process cases and17 helper checks pass; visible F10 behavior awaits the next owner invocation.

Current launcher SHA-256: `69b94f58a59fdd44a024ef0f7a18f7b42a0cb500b2d2905b507e0f7d0484c5c9`. Original manifests and receipts are preserved. CA/CURRENT/signature/recovery remain DENY, ED11 PARTIAL.

Visible-confirmation follow-up: raw console input now echoes the nonsecret declaration and supports Backspace; the old any-key pause is removed. New reviewed binary/script pins are in README.md. [Exact changes and validation](../personal-first-install-visible-confirmation-20261022/closeout.md). Positive echo/ceremony awaits owner execution.

## Owner successful manual read-only path — 2026-10-22

Verified receipt023060eb825043248664b625f85bec6c and owner screenshots establish READ_ONLY_WORKER_VERIFIED_FINAL_DENY, eligible console/operator, matching current binary, absent child and CLI2. The previously reported launcher0 was an agent reading error: owner corrected the outer result to -1073741510 (STATUS_CONTROL_C_EXIT), so normal launcher completion remains unproved. This supersedes the missing positive manual path only: PASS_SCOPED_MANUAL_READ_ONLY. Actual child/CA/bootstrap/CURRENT/signature/recovery remain denied/not run; ED11 PARTIAL and broader fault qualification remain pending. [Evidence and scope](manual-readonly-success-20261022.md).

Correction of outer result (2026-10-22): -1073741510 converts to0xC000013A / STATUS_CONTROL_C_EXIT per Windows SDK shared/ntstatus.h. CLI receipt remains valid scoped read-only evidence, while outer launcher was interrupted; no clean whole-launcher completion or rollback is inferred. No code, binary or receipt altered.

## Subsequent normal F10 completion — 2026-10-22

New distinct owner execution: receipt [48091b1698c841e6a63bb11ee5b0ab30.json](manual-results/48091b1698c841e6a63bb11ee5b0ab30.json), SHA-256 be68a3660c53fb673dd940f99eb3f9311a8cbe17ebedb089d80c6696277a3dfe. Verified eligible operator, interactive console, matching current binary, READ_ONLY_WORKER_VERIFIED_FINAL_DENY, CLI2 and child absent. Owner explicitly reports F10 closure; supplied outer-console screenshot clearly shows ELEVATED_QUALIFICATION_EXIT=0 and returned prompt. This is a separate normal-completion observation, not a reinterpretation of the earlier STATUS_CONTROL_C_EXIT.

POSITIVE_NATIVE_CEREMONY=PASS_SCOPED_MANUAL_READ_ONLY
QUALIFICATION_LAUNCHER_EXIT=0_NORMAL_AFTER_F10
BOOTSTRAP_EXECUTION=NOT_RUN
TARGET_CHILD_CREATION=NOT_RUN
CA_CUSTODY=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL

This establishes the single successful manual read-only path and normal launcher return, not full negative/failure/operational resource qualification. Previous interruption evidence remains preserved. No code, binary or original receipt changed.
