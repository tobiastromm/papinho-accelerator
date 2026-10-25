# N0START-03 — Fresh native read-only preparation and STOP

TASK=PACC-PHASE6-N0START03-FRESH-NATIVE-READONLY-PREPARATION-1

The current Selected Release read-only qualifier is built and qualified offline. The final manual operation package is **STOP / NOT READY FOR EXECUTION APPROVAL**, because the unchanged C and launcher bind the previously used historical read-only approval path. Preserving history requires a new path, a separately authorized pin integration and rebuild, then exact new hashes/approval. This task does not perform that source/pin change.

## Current build

Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Same MSVC19.51 x64/Ninja/qualified dependencies and Selected managed profile as the prior self-token build; production0.6.4 unchanged. [Build recipe](build-Selected-Release.cmd) and log preserve all flags. New tree `build/n0start03-fresh-native-readonly-20261024/Selected/Release`; no historical artifact overwritten. Qualifier compilation defines PAPACC_N0START03_READONLY_PINS=1 and PAPACC_N0_READONLY_PINNED_LAB=1, and does NOT define PAPACC_N0_POSITIVE_PINNED_LAB. CMake positive optionOFF, default general profileEMPTY_DENY; opt-in build does not authorize any observation.

Executable path: C:\Projetos\PapinhoAccelerator\build\n0start03-fresh-native-readonly-20261024\Selected\Release\papacc_scm_instrumented_lab.exe

SHA256: `3093beaa26d2b57fe6fe14fe9b5d9cfbb7ea7f5ef36ff936850c1d7de7e0583d`;91,136bytes. Build PASS, zero compiler warnings. Fifteen isolated model/API-simulated executables pass ([offline results](offline-test-results.json)); no qualifier entry, --prepare-n0 or --cycle-n0 was run. Includes A1 actual caller/model/retention, B1 source/BD checks, self-token217 assertions, policy/freshness/replay/Stop/UNKNOWN, RAW1 model and default/opt-in pins. No native admission evidence is asserted from those tests.

A1/B1/self-token sources are present unchanged. The read-only route intentionally does not close lab_token early, because its condition is positive&&n0. Do not execute a positive route or alter that predicate to measure the saving. Read-only also differs in protected content/metric-window behavior, as documented by the coverage review.

## Review and pin matrix

Applicable sources: preceding coverage decision, positive resource harness, self-token, A1 and B1 reports; N0–N9, L0–L9, H0–H9, ED11 B0–B6 revision4 and Model E, existing governance/future C2 context. No contract/ADR changes. [Pin review](pin-review-matrix.csv) separates historical expected native facts, candidate byte verification and future fresh validation. [Source/policy/dependency hashes](reviewed-source-policy-dependency-hashes.json) include current qualifier, canonical sources/pins/observer/helper, policies and directly built static libraries. Existing candidate evidence7573bytes and record397bytes still match approved hashes. They are NOT substitutes for opening the actual protected originals under the future authorized token; no ProgramData native query was executed here.

Current launcher SHA256 remains `a96ad14d8ded2d9033599579462af154046fdee738e2d804512e6c40779ec009`. It still pins historical executable4e7f9cd090fb132d741d57cbb1754a410474b897375a272d8708d51439903407, which is not the current build. The old executable and exact record remain untouched. Console.Out patch, other Write-Host confirmations, one PowerShell process, callbacks and cleanup behavior remain unchanged.

## First unresolved divergence: exact approval address

Both `ig_approval()` under the successor read-only macro and the launcher Pin call refer to docs/integration/n0start03-operational-pin-integration-20261024/EXACT-READONLY-APPROVED.json. Its existing owner approval authorized the earlier one-shot read-only observation. Replacing only the executable hash would keep that obsolete approval address. Passing a new approval hash cannot make a new record at a different path visible to either hardcoded caller. Overwriting the old record, reusing its hash or pretending it approves this build is not permitted.

The [two-literal path patch](fresh-approval-paths.NOT-APPLIED.patch), SHA256 `dcc6535f29937ec8692142dde7ad9ef1c211f522bb91d7ba208aedd67d7a2b57`, changes only those two read-only references to this package's future EXCLUSIVE EXACT-FRESH-READONLY-APPROVED.json. `git apply --check` passed; patch is NOT applied, not compiled and no future record exists. It does not alter positive approvals or guards. Because section9 forbids functional code changes and section3 forbids automatic pin activation, the new C literal and launcher references require a separate scoped integration authorization before application. [Launcher pin update plan](launcher-pin-update.PLAN.json) additionally records the executable path/hash update needed AFTER that rebuild.

The current build hash above identifies the unmodified review build. Applying the candidate C path changes source and requires a new executable hash; applying final launcher pins changes launcher hash. Those future hashes are deliberately null. This prevents an inconsistent executable approval from being generated around an unbuilt candidate or old file. No pin is activated here.

## Prepared package and candidate command

[FRESH-READONLY-OPERATION-REQUEST.DRAFT.json](FRESH-READONLY-OPERATION-REQUEST.DRAFT.json) is explicitly DRAFT_STOP_NOT_EXECUTABLE, no operational authority. [Confirmation/observation/Stop plan](confirmation-and-observation-plan.md) preserves exact confirmation formats with PID/creation/challenge generated factually later, never copied from prior transcripts. No new attempt, nonce, intent, image, sink, arm or dossier is created.

[Manual command shape](manual-command.CANDIDATE.txt) is deliberately non-executable until path integration, rebuild, final pin review and separate owner approval have supplied the literal final script and approval hashes:

```text
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Prepare -Profile N0START03ReadOnly -ExpectedScriptSha256 <NEW_REVIEWED_LAUNCHER_SHA256> -ExactApprovalSha256 <NEW_OWNER_APPROVED_RECORD_SHA256>
```

Do not run that template or substitute old hashes. A final command with literal complete hashes is not possible before those still-unauthorized mutations/approval exist. No wrapper, alternate owner or second launcher is introduced to bypass the guard.

Future permitted observation remains one already elevated PowerShell5.1 x64, operator/token/no impersonation, exact host/boot, original protected RAW1 file and policy identities, temporary canonical mutex, SCM read-only configuration/status/recovery/triggers, real census and three current roles, PSS/stack-VA and host/ED11 metrics, exact confirmations and factual close/retention. No future service PID invented; no consume/rebind/Start/Stop/placement.

## Observation limits and Stop

`IG_PROCESS` handle count is pre-PSS in actual ig_measure_process. Current logs do not supply intermediate handle counts while capture/OpenThread/VirtualQueryEx execute; report NOT_AVAILABLE, not a fictional peak or the fixture delta. Stack-VA itself does execute in the real observer. First-error and close/retention receipts remain. IG_HOST supplies the metric generation tick; full end-to-end monotonic duration is not exposed by the unchanged launcher. Operator administrative timing may be recorded if factual; otherwise duration UNKNOWN. No extra recorder/instrumentation added here.

Deny for any script/executable/approval/policy/native identity/hash/RAW1/reparse discrepancy, old authorization, token/boot/host mismatch, missing facts, extra process/concurrency, SCM/recovery/triggers mismatch, metric failure, stale60s/1s, headroom, cap128native/1024sharedadmin, unexpected Stop or confirmation mismatch. Preserve first error and original references/dependencies on uncertain close, with no retry/rollback/delete/kill/forced mutex release. NONE_GRANTED, H0–H9/P3 retained UNKNOWN and real reservations remain. PASS read-only is not an independent budget or positive authority.

## Required authorization sequence

1. Separate scoped authorization to apply the reviewed two-reference path candidate and final launcher executable pins, rebuild/requalify offline, and emit final hashes; still NO Prepare or Start.
2. Only after that consistent final package, a specific approval for ONE manual fresh read-only execution and exclusive approval-record registration.
3. Owner manual execution only. No automatic execution by Codex. No positive operation approval implied.

The build/preparation work is complete within its authorized bounds, but the requested final manual package is not yet qualified/executable. Report CURRENT_TASK_COMPLETED=NO rather than label the path conflict ready for operational approval.

CURRENT_TASK_COMPLETED=NO
CURRENT_QUALIFIER_BUILD=SELECTED_RELEASE_PASS_ZERO_WARNINGS_NEW_HASH
READONLY_PACKAGE=DRAFT_STOP_APPROVAL_PATH_INTEGRATION_REQUIRED
OPERATIONAL_PINS=PREPARED_NOT_ACTIVATED
FRESH_OBSERVATION=NOT_EXECUTED
OFFLINE_TESTS=15_PASS
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_ALL_PREEXISTING_FILES_UNCHANGED
FIRST_UNRESOLVED_DIVERGENCE=HISTORICAL_EXACT_APPROVAL_PATH_HARDCODED_IN_C_AND_LAUNCHER
NEXT=SCOPED_FRESH_READONLY_APPROVAL_PATH_AND_PIN_INTEGRATION_THEN_OWNER_APPROVES_FRESH_NATIVE_READONLY_OBSERVATION
