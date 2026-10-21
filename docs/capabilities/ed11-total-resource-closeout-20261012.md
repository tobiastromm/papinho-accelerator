# ED11 total resource budget and admission qualification closeout

2026-10-12. Current task scope: isolated resource/policy engineering. No C2 wire, production C3, E1-E12, Browser/PST changes, promotion or publication.

## Outcome and authority

Managed fixed-reservation/rollback/identity/Session/host-credit/fairness policy is QUALIFIED_FOR_ISOLATED_TEST_SCOPE. Total user/provider/kernel and full Browser/C2 target envelopes remain NOT_QUALIFIED. This distinction prevents premature encoding/runtime approval.

The [accepted owner decision](../adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md) was recorded before implementation: user/kernel envelopes are incremental beyond factual baseline; numerical values remain candidates. Admission requires available host/process capacity too. Existing accepted ADRs remain unchanged.

Accelerator main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, preexisting dirty WIP preserved. Current PST homologacao/pst-0.6.5 HEAD4dbd2783ebaf135114da2b7247e36c0ad6bc550a was read-only confirmed. New measurements consume the byte-exact previously governed normal SDK commit da7190930700f54ecc92611f5edbcb0932caa5ec; no reissued SDK/test-only package was mixed. Future package hashes require separate impact analysis. Production pin0.6.4 unchanged.

## Deliverables and evidence classes

- [Total reconciled ledger](ed11-total-resource-ledger-reconciled-20261012.md).
- [Immutable measurements](ed11-total-resource-measurements-20261012.json).
- [Final conformance/source hashes](ed11-total-resource-final-conformance-20261012.json).
- [Protocol RFC supplement0.3.4](../protocol/pacc-c2-ed11-total-budget-supplement-draft-0.3.4.md).
- [Proposed admission ADR](../adr/ADR-CANDIDATE-ed11-total-resource-admission.md).
- [Final owner handoff](../integration/ed11-total-resource-final-owner-handoff-20261012.md).
- Standalone sources/reproduction: [README](../../tests/diagnostic/ed11_total_probe/README.md).

Earlier23 instrumented probes,4 models, five warning-free builds and28 ED08 probes remain historical valid evidence in their scopes. New policy extends their scope, does not promote the old limited model to independent CONTROL lifecycle proof. Old artifacts/traces remain unchanged.

Synthetic policy failure inputs, real normal PST PRECONDITION/DESTROYED, current-host C1 observations and native close-failure R4 are not interchangeable. NSS and R4 are separate owners.

## Measurements: total, baseline, increment, peak and final

Normal public-PST single owner, at most2 logical fixture operations/4 TLS endpoints, fixed public test PKI and exact normal OpenSSL target.676 lifecycle cycles /719 logical operations across Debug32, Release128, Release512 and final Debug4 runs. Larger run was justified by residual growth in128 cycles, not repetition of a completed qualification.

| Run | Baseline private bytes | Sampled peak private bytes | Historical peak commit | Final private bytes |
| --- | ---: | ---: | ---: | ---: |
| Debug32 |1482752 |3493888 |4149248 |2854912 |
| Release128 |1466368 |3833856 |4468736 |3137536 |
| Release512 |1470464 |3747840 |4407296 |3158016 |

Private increment at sampled peak: Release1282367488 bytes; its process historical peak-commit increment3002368 bytes. These expose a larger transient than the earlier1728512-byte scoped snapshot. They are overlapping metrics, not additive allocation categories.

Release512 final private increment1687552 bytes. Post-close private values at cycles7/127/255/383/511:2641920/3096576/3559424/3702784/3158016. Last256 cycles vary3112960..3702784; no monotonic per-cycle growth or hard infinite-duration guarantee is established. A final drop does not prove allocator/provider leak freedom. Shared/lazy runtime/allocator attribution remains incomplete.

Normal loops: baseline/maximum/final handles75/116/94; post-cycle eligible-destruction snapshots98 handles consistently. Threads observed4. These resource counters are budget evidence, not per-lifetime native exactly-once proof. Remaining19 handle delta cannot be attributed from a global count; do not label it destroyed, leaked or provider-owned without more proof.

Release512 minimum observed available physical2848354304 bytes and commit8529133568 bytes. Fixture64MiB pre-cycle safety floor is explicit diagnostic configuration, not an accepted product reserve. Available capacity is measured separately from process/increment counters and can change with unrelated host workloads.

Known harness-native sockets:4 per logical test operation,8 with two operations, plus bounded transient listeners/candidate-frame pipe (source ceiling11 simultaneous acquired sockets including a pipe setup). This is source-audited fixture inventory, not the future12-context C2 topology or total kernel bytes.

## User mappings and shared attribution

Two independent Debug/Release virtual-map probes PASS with six phases each: baseline; one-operation setup; factual active-membership retention; two operations/four contexts sharing one runtime; second operation destroyed while shared parent remains; all children destroyed/WinSock cleaned.

Release classified committed virtual views:
- baseline29249536 bytes (about27.9MiB);
- two-operation observed peak35573760;
- final35061760;
- observed view increment6324224.

Classified baseline private/image/mapped views577536/25747456/2924544. Two-operation views2183168/27090944/6299648. These are address-view partitions, not physical RAM, owned allocation census or values to add to PrivateUsage. Reserved virtual bytes around4.36GB are address-space reservation, not committed memory/minimum RAM. Mappings may alias shared backing; copy-on-write image pages complicate attribution. Every required allocation category remains listed independently in the ledger.

## Browser C1 available-target evidence

An external modern observer ran the immutable qualified C1 x86 executable on Windows10 without modifying Browser. Final207 valid memory samples and sampled thread counts1..5; actual native suite exit0. First sample is already after process creation, not an idle Browser/pre-C2 phase barrier. Final means last sample before exit, not proven phase-final resource reclamation. Both initial and final observer outputs are preserved.

VC6 extracted-layout probe measured pump412 bytes, status344, endpoint288, token8, critical section24. Nested values are included in pump, not additional allocations. No payload queues/caller buffers exist in C1 itself. Requested16KiB socket buffers are not kernel bounds. Window/event/WinSock/worker stack and caller app storage remain separate.

This is the C1 test process, containing fixtures and raw TCP, not full Browser UI plus CONTROL/DATA/Site TLS. No new NSS DLL or PST R4 retention is qualified by these measurements. Real NT4/2000/XP execution is absent.

## Admission correctness and factual corrections

Final50 checks PASS each in modern x64 Debug/Release and VC6/C89 x86/current Windows:150 assertions, zero failures.

Corrected isolated model defects before closeout:
1. Closed operation ID could be reused: monotonic committed IDs and old callback fencing added.
2. Shared host-capacity snapshot could be double-spent: reserve/refund host credits; retained resources keep them.
3. Last operation refunded Session storage while CONTROL remained alive: separate Session close, generation and borrower checks.
4. Independent test populations could overlap retained storage: fairness population now ends before persistent-retention population.
5. Expanded commit, physical and process capacity exhaustion tests; allocation partial/cancel rollback before modeled external effects.

Complete pre/post SHA256 and diffs are retained in build/ed11-total-20261012. These are cause-based harness/policy corrections, not functional Accelerator/Browser changes.

| Required negative | Evidence class / result |
| --- | --- |
| Insufficient quota, exact bound, request one byte over cap | Controlled allocator/policy PASS |
| Partial allocation | Three deterministic nth-allocation failure points, rollback PASS |
| Checked arithmetic overflow | C89/x64 size_t boundary PASS |
| Repeated/third admission and concurrent Sessions | Finite slot/one-op-per-Session guards PASS |
| CONTROL/DATA starvation | Filled staging/eight ready flow spans, rotating7 DATA +1 CONTROL opportunities,16 wakes; model PASS |
| Persistent retention/no eviction |1000 attempts, stable original op/Session/host credits; synthetic policy PASS |
| Cancel during reservation | Fresh unconsumed allocations refunded, no modeled effects PASS |
| Late callback/stale generation | Old op/Session cannot retarget later incarnations PASS |
| Duplicate release | Op and Session credits reclaimed at most once PASS |
| Provider unavailable/kernel unavailable/measurement failure | Controlled guard inputs deny before allocation/effects; no native fault induced |
| Host/process near exhaustion despite fitting increment | Commit/physical/process and double-spend tests PASS |
| Session alive at zero operations | Reservation persists; callback/retained child prevents Session free PASS |

Fairness is qualified for the isolated storage scheduler, not production PACC/TLS CONTROL dispatch. Real full-C2 liveness and serialized partial CONTROL remain future gates. No native OOM/kernel-close failure was induced.

Final applicable builds: x64 Debug and Release /MD /W4 /WX; VC6 policy and VC6 layout /MD /W4 /WX, PASS zero warnings. Initial VC6 layout included an unused RPC header producingC4115; WIN32_LEAN_AND_MEAN limited headers to required types, with warnings still enabled and original SDK/Browser unchanged. Failed build log preserved; no stale executable used.

## Limits and stop boundary

Preserve Browser managed2MiB / incremental user20MiB / incremental kernel2MiB and Accelerator2/32/8MiB candidates. Managed layout fits tested caps including actual controller metadata; full adequacy is unproven.

Public normal PST has no hard allocator reservation/usage cap and no total socket-kernel accounting. Pool quota counters and socket hints are incomplete. Full C2 composition, secure CONTROL/DATA, Browser process baseline, worst supported peer/config inputs, full retained-provider footprint and actual old-target headroom are missing. Current measurements cannot safely establish the required hard totals. Full admission remains fail-closed at that missing-evidence boundary.

Do not invent new numerical caps or turn an empirical fixture envelope into a guarantee. Owner must approve target metric/headroom/freshness and either existing supported hard bounds or explicitly restricted empirical lab uncertainty. New public contracts/upstream modifications require a separate decision; none were performed here. Real NT4 remains C12, not inferred from VC6 on Windows10.

## Result

CURRENT_TASK_COMPLETED=NO
SLICE_COMPLETED=YES
ED11_STATUS=MANAGED_POLICY_QUALIFIED_FOR_SCOPE; TOTAL_TARGET_CAPS_OPEN
ED11_MANAGED_MEMORY=FIXED_LEDGER_AND_ROLLBACK_PASS
ED11_TOTAL_USER_MEMORY=PRIVATE_COMMIT_WORKING_SET_AND_VIRTUAL_VIEWS_MEASURED; FULL_ATTRIBUTION_UNQUALIFIED
ED11_KERNEL_RESOURCES=HANDLES_THREADS_POOL_QUOTAS_AND_SOCKET_INVENTORY_OBSERVED; TOTAL_UNKNOWN
BROWSER_MANAGED_CAP=2MiB_CANDIDATE_MODEL_PASS
BROWSER_TOTAL_USER_CAP=20MiB_INCREMENTAL_CANDIDATE_NOT_QUALIFIED
BROWSER_KERNEL_CAP=2MiB_INCREMENTAL_CANDIDATE_NOT_QUALIFIED
ACCELERATOR_MANAGED_CAP=2MiB_CANDIDATE_MODEL_PASS
ACCELERATOR_TOTAL_USER_CAP=32MiB_INCREMENTAL_CANDIDATE_EMPIRICAL_FIXTURE_ONLY
ACCELERATOR_KERNEL_CAP=8MiB_INCREMENTAL_CANDIDATE_NOT_QUALIFIED
ADMISSION_POLICY=QUALIFIED_ISOLATED_C89_SCOPE_NOT_INTEGRATED_PRODUCT
FAIL_CLOSED=PASS_GUARDS_AND_UNCONSUMED_ROLLBACK
RETAINED_QUOTA=PASS_CONTROLLED_MODEL; ACTUAL_PRECONDITION_OBSERVED
CONTROL_RESERVES=PASS_FIXED_STAGING_NOTICES_AND_SESSION_LIFETIME_MODEL
FAIRNESS=PASS_BOUNDED_STORAGE_SCHEDULER_MODEL
MEASUREMENTS=676_CYCLES_719_OPERATIONS; TWO_MAP_PROBES; C1_X86_OBSERVER_AND_LAYOUT
NEGATIVES=50_CHECKS_PER_BUILD_150_PASS
BUILDS=PASS_X64_DEBUG_RELEASE_VC6_POLICY_LAYOUT
WARNINGS=0_FINAL_BUILDS
NT4_REAL=NOT_RUN
C12_STATUS=PENDING
C2_WIRE_READY_FOR_APPROVAL=NO
C2_ADR_STATUS=PROPOSED; INCREMENTAL_INTERPRETATION_ACCEPTED_SEPARATELY
PACC_SPECIFICATION_STATUS=0.3_PRESERVED; SUPPLEMENT0.3.4_PROPOSED
PST_NSS_UPSTREAM=SEPARATE_OWNER
PST_R4_UPSTREAM=SEPARATE_OWNER
PRODUCTION_PIN=0.6.4
PST065_PROMOTED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
WIP_PRESERVED=YES
DIFF_CHECK=PASS
TEST_PROCESSES_REMAINING=0
FIRST_UNRESOLVED_DIVERGENCE=ED11_FULL_PROVIDER_KERNEL_AND_BROWSER_TARGET_BOUNDS_UNAVAILABLE
NEXT=OWNER_ED11_METRIC_HEADROOM_AND_HARD_VS_EMPIRICAL_SCOPE_DECISION; SUPPORTED_TOTAL_BOUNDS_AND_FULL_TARGET_EVIDENCE

No commit/push/tag/release. All test-only classes and artifact identities remain distinct.
