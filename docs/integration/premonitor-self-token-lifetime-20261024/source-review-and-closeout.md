# Pre-monitor self-token lifetime correction — source review and closeout

TASK=PACC-PHASE6-PREMONITOR-SELF-TOKEN-LIFETIME-1

## Scope, integrity and authority

Owner-authorized functional patch only. Branch main, HEAD 913c90cf2328f7939d8f5b927640601903ac6e91. Baseline records 10,754 preexisting tracked/untracked nonignored files; original WIP is preserved. The candidate SHA256 is exactly `7b1dc4b012c5d1495290e38630d899e8e94941f7ecb6ea17e5b9e455db7fd4d1`. Context/hunks passed `git apply --check`; the resulting source compared against its preimage differs only by the approved insertion. A copy of the owner-authorized patch is preserved as [owner-authorized.APPLIED.patch](owner-authorized.APPLIED.patch). No accepted ADR was edited.

Applicable boundaries remain Model E, ED11 B0–B6 revision 4, N0–N9, L0–L9 and H0–H9, A1 two-slot ownership/Stop/retention and B1 callback correction. The prior [source/resource review](../native-c-premonitor-handle-pressure-20261024/source-resource-review-and-next-slice.md), A–J ownership ledger and A1/B1 evidence remain historical authorities/evidence in their original roles. Future C2 direction is unaffected. B2 is not implemented.

## Exact functional change and ownership

The only functional change is in `tools/qualification/scm-instrumented-cycle-lab.inc`, immediately after successful initial lab_operator, applicable ig_actor and ig_boot checks, before observer initialization and PROTECTED_BINDING. The branch requires both positive and n0. The patch's original “CANDIDATE ONLY” comment is retained as part of its exact approved text; this does not grant runtime authority.

lab_token owns the qualifier self-token, whose last handle use is in the synchronous lab_operator call. It is not the protected PowerShell/auxiliary token, and no borrow remains at this point. lab_stats is an independent copied structure. Actual lab_operator_current remains unchanged: it rejects unexpected thread impersonation, opens the actual current primary token under A1, compares TokenId, AuthenticationId and ModifiedId, and closes through its scoped owner. N0/RAW1 identities, original protected files, freshness, SCM, canonical mutex, Stop and first-error checks are unchanged.

Successful CloseHandle NULLs lab_token before proceeding; later cleanup cannot close it twice. Failure preserves the same original global HANDLE, records lab_error, freezes A1 via a1_retain/lab_aux_sync, propagates Stop/retention and goes directly to done. It does not move this already-owned reference into a full auxiliary array. Cleanup guards retain dependencies and cannot retry this uncertain close. Missing lab_token fails before the close. Initial validation failures skip the new branch entirely. Read-only and legacy combinations do not execute it.

Source SHA256 after application: `ab7c06210c163e8687f0872b3d66eafc4d6bdcf7951da247bbc29b8d1572d120`.

## Builds and offline tests

Selected/Legacy Debug/Release complete with MSVC x64, /W4 /WX, zero compiler warnings. All three existing LAB qualifier targets and fifteen isolated test targets were built per configuration. No qualifier executable was run. Positive LAB profile remains OFF; existing read-only pins are not activated or updated. Production pin remains 0.6.4; existing qualified build dependencies/toolchain are preserved.

[offline-tests.json](offline-tests.json) records 60 successful test invocations, 15 per configuration. The new test uses actual lab_operator/lab_operator_current and cleanup/census bodies with API simulation, plus the exact extracted applied branch. It checks 217 assertions/configuration: four route combinations, saved statistics and 32 successful current checks per route, TokenId/AuthId/ModifiedId mismatch, impersonation, Stop without new token acquisition, close-UNKNOWN with immutable first error and no second close, initial token/user/elevation/statistics/membership failures and initial boot-failure control flow. Existing A1 directed/finite exploration/mutant/caller suites, B+D source/model, runtime/replay/freshness, policy, RAW1 model and default/opt-in pins tests pass. These simulated/model checks are not native admission evidence. Freshness/replay algorithms are unchanged, exercised by the existing negative suites, not claimed as a new end-to-end positive operation.

The only build-system addition is the isolated test target and source dependencies. No production build policy or quota changed.

## Real isolated measurement

[native-measurements.json](native-measurements.json) records 36 expected outcomes across the four configurations: clean, deliberately mismatched saved TokenId/ModifiedId and Stop for both current/applied modes, plus injected uncertain-close containment for applied mode. Four standalone /W4 /WX builds have zero warnings. The fixture uses real self tokens and unchanged A1/core bridge/current-token body, and the exact applied branch. It creates no files, intent, service, SCM effects or operational authority. Saved-statistic mutation is synthetic; it does not alter the Windows token.

Every current run measured a same-process before/after delta of 0. Every applied non-UNKNOWN run measured -1. Clean runs retained 64 successful current-token revalidations with no stable drift. Sampled scoped-token transient peak was one handle above each post-close stable count. Absolute counts differ between processes (e.g. current59 and applied53 before the branch); these are not used as the reduction estimate. Only each process's own before/after delta is causal evidence for this reference.

The four UNKNOWN cases inject a failure without actually closing the original real token. Each confirms original retention/freeze/Stop and zero retry/new acquisition, then returns code2 as expected for process-exit containment. Exit is not labelled native CloseHandle success or global quiescence.

## Storage, stack and limits

[resource-ledger.csv](resource-ledger.csv) separates the one-reference lifetime reduction from metadata and unknown costs. No HANDLE field, array, lease, scalar local or frame-local declaration is added; the patch reuses e and existing cycle_stage. The stage literal occupies 26 bytes including NUL before compiler pooling/alignment. A1 owner remains48 bytes with two slots. No total-memory saving, stack-commit saving or credit is claimed.

[compiled-frames.json](compiled-frames.json) compares assembly probes of the normal positive-disabled qualifier with the previous A1 build under the same four profiles: main stack subtraction remains2880 Debug/288 Release; lab_operator_current remains376 Debug/200 Release. Other recorded relevant functions also remain unchanged. These probes do not establish a positive-enabled whole-cycle frame size: disabled positive branches can be optimized away. The actual applied branch is separately compiled/exercised in the isolated test/fixture. There is no claim of zero total compiled memory cost or full positive stack qualification.

ED11 128 native handles,1024 shared administrative handles, four roles,60s/1s freshness, headroom and NONE_GRANTED remain unchanged. Historical126/128 is not a future baseline. Conditional125+3=128 is still only a projection; full first-monitor/PSS/transient cost and fresh capacity remain unproven. UNKNOWN historical obligations remain retained under H0–H9; no refund, reserve or independent budget is granted.

## Preservation and next gate

[artifact-hashes.json](artifact-hashes.json) lists new offline artifact hashes, not executable approvals or operational pins. [preservation-and-source-checks.json](preservation-and-source-checks.json) confirms exact hunk, extraction, HEAD and baseline preservation. Launcher SHA256 remains a96ad14d8ded2d9033599579462af154046fdee738e2d804512e6c40779ec009; historical03 qualifier remains4e7f9cd090fb132d741d57cbb1754a410474b897375a272d8708d51439903407. No dossiê, accepted contract, PowerShell, launcher or pin changed. The live capability receives only a factual append.

First unresolved property: full fresh positive-cycle simultaneous resource capacity, including PSS and required positive references, has not been demonstrated. Next gate is N0START03 positive resource requalification using a separately scoped isolated experiment/review. No positive command is prepared, no consumed attempt reused, no new operation authorized.

CURRENT_TASK_COMPLETED=YES
SELF_TOKEN_EARLY_CLOSE=EXACT_AUTHORIZED_PATCH_OFFLINE_QUALIFIED
OWNERSHIP=ORIGINAL_GLOBAL_RETAINED_ON_UNKNOWN_NO_RETRY
SECURITY_REVALIDATION=PRESERVED_ACTUAL_CURRENT_TOKEN
NATIVE_HANDLE_REDUCTION=ONE_REFERENCE_ISOLATED_NOT_LAB
POSITIVE_CYCLE_VIABILITY=NOT_YET_DEMONSTRATED
BUILDS=4_CONFIGURATIONS_PASS
WARNINGS=0
OFFLINE_TESTS=60_PASS
NATIVE_TESTS=36_EXPECTED_OUTCOMES_ISOLATED
NETWORK_TESTS=NOT_RUN
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_EXCEPT_AUTHORIZED_PATCH_TEST_TARGET_AND_LIVE_APPEND
FIRST_UNRESOLVED_DIVERGENCE=FULL_POSITIVE_RESOURCE_CAPACITY_UNPROVEN
NEXT=N0START03_POSITIVE_RESOURCE_REQUALIFICATION
