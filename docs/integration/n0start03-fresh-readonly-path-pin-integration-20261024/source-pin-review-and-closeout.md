# Current closeout — fresh read-only approval address and pins integrated

TASK=PACC-PHASE6-N0START03-FRESH-READONLY-APPROVAL-PATH-PIN-INTEGRATION-1

This is the **current integration report**. The prior [preparation STOP](../n0start03-fresh-native-readonly-preparation-20261024/build-pin-review-and-preparation-STOP.md) is preserved as historical evidence and does not describe the newly completed integration. Source, Selected Release build, launcher pins and inert request are now consistent. The package is ready for specific owner approval of ONE future manual fresh read-only observation. No execution is authorized by this implementation closeout, and no exact fresh approval record has been created.

## Authority, baseline and exact diff

Owner attachment explicitly authorized the exact two-reference approval-address patch, rebuild, derived launcher executable path/hash updates and offline qualification in one scope, superseding the prior no-functional-change restriction only for these edits. [Owner authorization](owner-authorization.txt) is implementation authority, not a read-only execution approval. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91.

Candidate patch SHA256 `dcc6535f29937ec8692142dde7ad9ef1c211f522bb91d7ba208aedd67d7a2b57` and both source preimages matched before application. `git apply --check` passed, then the patch was applied. The only functional-source changes are:

1. The successor read-only literal in ig_approval now points to `docs/integration/n0start03-fresh-native-readonly-preparation-20261024/EXACT-FRESH-READONLY-APPROVED.json`.
2. The same Pin address in the existing canonical launcher changes identically.
3. Only the launcher's $Qualifier and $QualifierHash fields change to the actual rebuilt executable path/hash below.

[Full focal diff](authorized-focal-diff.APPLIED.patch), SHA256 `bd965c15eabee852186f50fa6593968e83fe8f4dd76b66a838c4a1df500a684f`. The C file differs from its raw preimage by exactly one literal replacement; the launcher by exactly three literal replacements. Initial git-apply normalized line endings around the hunk. Strict byte checks found this, so unrelated bytes were restored from preimages without changing the authorized semantics; build/tests were repeated. Final checks prove no unrelated EOL/text differences. Preimages and intermediate post-apply bytes are retained for review. No ownership, policy, guard, callback, cleanup or quota changes.

A1 and B1 source hashes match baseline; self-token remains `if(positive&&n0)` after its last use and before PROTECTED_BINDING. Read-only therefore does not receive that positive-only handle reduction. B2 is not implemented. PowerShell single-process flow, Console.Out forwarding, other Write-Host calls and confirmations remain unchanged. N0–N9/L/H/RAW1/ED11/Model E/Stop/replay/retention restrictions remain. No accepted decision is edited.

## Final build and artifact pins

New canonical build tree preserves both historical executable directories:

`C:\Projetos\PapinhoAccelerator\build\n0start03-fresh-readonly-path-pin-integration-20261024\Selected\Release`

This is a build-output directory, not a checkout/clone/worktree. It uses the same documented MSVC19.51 x64/Ninja recipe/dependencies/Selected profile. [Build command](build-Selected-Release.cmd), [first log](build-Selected-Release.log), [final rebuild log](build-Selected-Release-final.log) and [exact compile command](qualifier-compile-command.json). Both builds pass; final warnings0. Production0.6.4 preserved; the existing isolated dependency SDK is not a production upgrade.

| Final artifact | Bytes | SHA256 |
|---|---:|---|
| `papacc_scm_instrumented_lab.exe` in the new build tree | 91136 | `deeff3b9911ea720ebf5df2c31a1ae10657a19a7d64b6e37fee48029aee1944f` |
| `tools/qualification/scm-n0-protected-single-powershell.ps1` | 11871 | `daf6c078e08474cbb112198a02dcbc90efe441344a530f34e8ea8017b287c8ee` |

The earlier3093beaa26d2b57fe6fe14fe9b5d9cfbb7ea7f5ef36ff936850c1d7de7e0583d build and4e7f9cd090fb132d741d57cbb1754a410474b897375a272d8708d51439903407 historical qualifier remain unchanged and are not used as final pins. New artifact hashes are factual, not inferred or reused. [Final artifact hashes](final-artifact-hashes.json) and [source/policy/dependency manifest](final-source-policy-dependency-hashes.json) identify the final bytes. Source manifests do not convert absent native observations into proof.

Opt-in compilation has PAPACC_N0START03_READONLY_PINS=1 and PAPACC_N0_READONLY_PINNED_LAB=1. PAPACC_N0_POSITIVE_PINNED_LAB is absent and CMake positive optionOFF. General defaultEMPTY_DENY is preserved; launcher Cycle still throws before acquisition. These pins permit no autonomous execution or positive admission.

## Offline qualification

[Final offline tests](offline-test-results.json):15/15 existing executables pass in Selected Release. A1 directed/finite/model/mutant and actual caller simulations, B1/BD/source gate, self-token217 assertions, runtime/cycle/policy/freshness/replay, Stop/UNKNOWN/retention, RAW1 --model and default/opt-in pins are covered. No qualifier main/--prepare-n0/--cycle-n0, native preflight, launcher or service entry was invoked. No SCM query/mutation. These are offline model/API-simulated results, not a factual native admission or resource measurement.

PowerShell Parser.ParseFile passed for final script with zero syntax errors; script not invoked. [Cross review](cross-review-checks.json) confirms both approval addresses match, launcher path resolves to the actual final executable and its hash, historical approval is byte-exact, new approval is absent, default/positive guards and required macros preserved. Checked source bytes are exact authorized replacements only.

## Inert package for separate approval

[Current request DRAFT](FRESH-READONLY-OPERATION-REQUEST.DRAFT.json), SHA256 `44cbc2ac6b377b75983d8c63210e1783510ef216eff34022140f9d621c996c38`. StatusDRAFT_READY_FOR_SPECIFIC_OWNER_APPROVAL_NOT_EXECUTABLE; not an owner approval. [Updated matrix](pin-review-matrix.csv), SHA256 `87e82d9efb850cdea7742816e781ad0813f3878f0e323a6c89b66e9c88bd007b`, separates preserved historical evidence, final reviewed artifact bytes and facts still required from the future original handles/token/host/boot/metrics. No PID/token/challenge/future observation is fabricated. No new LAB attempt identity or claim is created.

Future exclusive approval-record path, integrated in C and launcher, remains **ABSENT**:

`C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-fresh-native-readonly-preparation-20261024\EXACT-FRESH-READONLY-APPROVED.json`

The historical `n0start03-operational-pin-integration-20261024/EXACT-READONLY-APPROVED.json` remains SHA256ea272d2a4aa33f42f1cb1c12c8418fdd793fde1e529067325bd9ab8b430dcb7e and must not authorize or be overwritten for this observation. After explicit owner approval, a separate exact record must be registered exclusively and hashed; only then can the last command value become a literal executable approval hash.

[Candidate command](manual-command.CANDIDATE.txt), **NOT EXECUTABLE/NOT AUTHORIZED YET**:

```text
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Prepare -Profile N0START03ReadOnly -ExpectedScriptSha256 daf6c078e08474cbb112198a02dcbc90efe441344a530f34e8ea8017b287c8ee -ExactApprovalSha256 <SHA256_OF_FUTURE_OWNER_APPROVED_RECORD>
```

All currently known values are final. The single placeholder is deliberately the nonexistent future owner-approved record, not a pending implementation pin. Do not use the DRAFT hash as approval. No automatic Prepare. Future manual execution must use one already elevated Windows PowerShell5.1 x64, existing console and two literal confirmations generated from fresh native output:

```text
PREPARE_N0_READ_ONLY 0b1de869e11493b7cb08fb329578f6ce <AUX_PID> <AUX_CREATION>
BIND_BD_READ_ONLY 0b1de869e11493b7cb08fb329578f6ce <QUALIFIER_PID> <QUALIFIER_CREATION> <NATIVE_CHALLENGE>
```

## Remaining factual gates and proof limits

The future observation validates operator/token/no impersonation, current exact host/boot, protected RAW1 originals/IDs/hash/provenance applicability, decisionsN7/H/P3, service identity/configuration/recovery/triggers by read-only SCM, temporary canonical exclusivity, real census/admin/qualifier/auxiliary, full PSS/stack-VA, headroom/freshness and factual close/retention. Expected host/boot and native object IDs are unchanged historical bindings, never fresh observations in this task. Protected files were not recreated/reopened by this integration. N7 accepted scope is not expanded and obligations UNKNOWN/real reservations are not refunded.

IG_PROCESS remains pre-PSS handle count. Intermediate capture/OpenThread/VirtualQueryEx peak is NOT_AVAILABLE from existing logs; end-to-end monotonic duration remains UNKNOWN absent a factual measurement. No instrumentation was added to pretend otherwise. Future PASS read-only does not prove the entire positive peak, grant an independent budget or close ED11.128native/1024shared administrative/four roles/60s1s/headroom/NONE_GRANTED are unchanged.

Stop/deny on any pin/approval/identity/RAW1/host/boot/token/impersonation/provenance/SCM/recovery/census discrepancy, missing fact, stale metric, exceeded limit/headroom, exact confirmation mismatch or uncertain close. Preserve original references/dependencies and first failure in UNKNOWN; no retry/delete/overwrite/rollback/kill/forced mutex release. No intent arm/consume, placement, image/sink/arm creation, rebind, Start/Stop, ACL, CA/certs/bootstrap/CURRENT. Local close receipt does not establish historical global quiescence.

Integration is complete; only separate owner approval and its exact record are pending. That is an authorization gate, not an unresolved implementation divergence. The prior STOP and all preexisting WIP/history remain preserved except the explicitly authorized focal source/pin changes and factual live-document append required by project governance.

CURRENT_TASK_COMPLETED=YES
FRESH_APPROVAL_PATH_INTEGRATION=COMPLETE
READONLY_EXECUTABLE_AND_LAUNCHER_PINS=CONSISTENT
OPERATIONAL_PINS=INTEGRATED_READONLY_ONLY_EXECUTION_NOT_AUTHORIZED
READONLY_PACKAGE=READY_FOR_SPECIFIC_OWNER_APPROVAL
EXACT_FRESH_APPROVAL_RECORD=NOT_REGISTERED
EXECUTION_AUTHORIZED=NO
BUILD=SELECTED_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=15_PASS
FRESH_OBSERVATION=NOT_EXECUTED
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_QUERIES=NOT_RUN
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_EXCEPT_AUTHORIZED_FOCAL_DIFF
FIRST_UNRESOLVED_DIVERGENCE=NONE_IN_INTEGRATION
NEXT=SPECIFIC_OWNER_APPROVAL_FOR_ONE_MANUAL_FRESH_READONLY_OBSERVATION
