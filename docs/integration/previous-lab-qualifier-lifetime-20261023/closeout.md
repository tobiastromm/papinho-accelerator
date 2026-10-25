# Previous LAB qualifier lifetime reconciliation — 2026-10-23

TASK=PACC-PHASE6-PREVIOUS-LAB-QUALIFIER-LIFETIME-1. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Bounded existing-evidence reconciliation concluded STOP. This documentary task is complete; original instance termination/resource release are NOT proved and no new Start is authorized.

## Exact historical identity and evidence

Only attempt563826bacc49449f954b20cc48ce9b36 is examined. The retained intent names it explicitly and the incremental log SHA256 is037f207ee4e40c889a16af06173a551066eef7b53aca32507ed8e900414bf832. See [hashed evidence index](evidence/historical-evidence-index.json). No new attempt ID was generated.

Its fixed service/account are PapinhoAcceleratorLab20261023 / NT SERVICE\PapinhoAcceleratorLab20261023. Operator SID S-1-5-21-785699550-1440458426-1682014700-1108. Qualifier artifact SHA256219d13960ac3bfdc98cb94c5e3baa8e834d8f5b52afe731006cc47ec1e02387d; installed candidate identity417614100:589824:2690484/hash62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716. These identify an administrative invocation and artifacts, NOT a proven native process instance. PID28452 is a historical observation only; no creation-time-bound instance handle/termination receipt was retained. Original service PID/instance handle was not captured.

Read in full: preceding native-status closeout/manual-handoff, original launcher/cycle code, original intent/log/plan/handoffs/resource report, relevant lifecycle/identity authorities and existing diagnostic observations. Earlier continuous-task accepted authorities remain in effect; no new implementation or architecture adopted.

## Source/evidence reconciliation

| Fact | Evidence | Decision |
|---|---|---|
| Authorization consumed before effects | exact intent; cycle_intent CREATE_NEW; launcher rejects existing intent | consumed, no replay |
| Exclusive candidate created | log LAB_IMAGE_CREATED with native identity/hash | persistent effect confirmed, not deleted |
| Existing service rebound and Start requested | source order before retained RUNNING_READINESS; SCM events7040/7023; existing readback | partial persistent effects, no rollback |
| Readiness | log STOPPED/PID0/Win325; qualifier5023 secondary | RUNNING not observed |
| Service instance handle/token | cycle_live at line181 runs only after readiness wait succeeds (line271) | not acquired by qualifier on this observed path; not a cleanup receipt |
| Controlled Stop | follows cycle_live; retained path precedes it | not requested on observed path |
| Qualifier retirement | cycle_can_retire line222 requires no Start or cycle_quiescent; quiescent set only line281 after Stop/status/process wait | guard false; no eligible retirement |
| Qualifier cleanup | retained branch line285 parks before cycle_cleanup line286 | no final cleanup performed on logged path |
| Final receipt | launcher writes JSON only after native command returns; exact manual-attempt JSON absent | no native final or outer completion receipt |
| Historical CLOSED=1 outputs | pinned-cycle preflight tests: EFFECT0, START_REQUESTED0 | different invocations; cannot retire this attempt |
| Later window/process observations | owner reports window not open; already recorded OpenProcess87/name absence | no original-instance termination/cleanup proof |
| System events |7023 service access denied,7040 startup change | service events, not qualifier lifetime evidence |
| Security log | prior query unavailable/access denied | unavailable, not empty; no repeat/bypass |

Source claims are bounded reconstruction from the pinned operation and logged stage, not retroactive per-handle receipts. The source cannot recover stack/static owner state after losing the original process instance. No foreign PID/observation handle can stand in for it. Current instrumented source and offline test releases do not establish historical qualifier release.

## Resource separation

| Class | Reconstructed ownership at retained stage | Proven disposition |
|---|---|---|
| Qualifier process-local refs | manager/service handles; protected original image/parent/config; administrative query token; candidate source/new candidate; two DLL handles; SID allocation and console-handler/process state | not individually released by logged path; final lifetime UNKNOWN |
| Launcher process-local refs | four pinned files, incremental log, native pipeline/list/output state and CLR/console resources | no attempt-specific final-close receipt; not inferred from qualifier/window absence |
| Qualifier late observation resources | service process/token/access-check token/thread/snapshot slots acquired in cycle_live | stage not reached in observed path; not counted as acquired or released |
| Service-host process resources | own process/runtime/SCM/token/config refs independent of qualifier | STOPPED5 and source suggest checked prepare-error path, not complete exact-instance resource attestation |
| Persistent objects | previously created SCM service, DEMAND/image binding, exclusive candidate; parent/config/original image/DLLs | retained existing effects, not removed or restored; handle closure would not remove them |
| Administrative records | exact intent/log/plan, prior registration/ACL evidence, diagnostics and hashes | preserved, consumed authorization remains non-replayable |
| Historical CA LAB/KSP and other LABs | independent owners/evidence; no operation in this path | unchanged; no classification borrowed from this task |

This is not a complete process handle/resource count. Historical98 handles were one snapshot, not exactly98 owned refs or proof of later zero. No resource cost UNKNOWN becomes zero. No explicit caller of this executor released, killed, closed, retired or reset original references.

## Applicable contracts and missing procedure

Accepted virtual-service identity V5 requires factual closure/retained ownership and rejects presumed rollback; V7 requires target/resource/retention accounting; V8 forbids removing guards. ADR0030 persistent failure/runner retains owners and rejects destructive fallback; ADR0032 distinguishes intent/READY from completed close. ADR0031 exact instance/generation and CLOSED prerequisites concern Secure Candidates, not an administrative orphan-recovery API. They constrain unsafe inference but cannot be transplanted as a qualifier recovery mechanism. C2 specification section8 retention/FINAL (lines259–261) distinguishes release/result/disposition and safe query-only terminal retention; this task is NOT a C2 operation or a new FINAL. ED11 incremental-envelope decision retains original charges/unknown evidence and denies admission without bounds.

No explicit procedure for an unreceipted, unavailable administrative LAB qualifier was found in this applicable reviewed set or its launcher/owner. The only implemented cycle retirement is no Start OR factual cycle_quiescent followed by checked cleanup. Process absence, elapsed time, a new process, reboot, admin privilege or documentary approval is not a substitute. No existing accepted recovery/CA-abandonment procedure is extended to this case.

Missing guarantee: correlation of this exact original instance to termination/quiescence and final disposition of all relevant owned/borrowed refs. Existing evidence cannot reconstruct it. Irrecoverable HERE means unavailable from this reviewed existing evidence, not proof that no external record could ever exist. No more same-result PID/SCM/security-log queries were executed.

## Minimal governed disposition and endpoint

Prepare only [PROPOSED administrative disposition](../../adr/ADR-CANDIDATE-previous-lab-qualifier-orphan-disposition-20261023.md): classify this evidence set as ORPHANED_UNRESOLVED/NON_CLEAN for administrative tracking, retain history/effects and UNKNOWN, prohibit replay and do not credit released capacity. This is not an implemented resource-state transition, CLEAN/RELEASED receipt, exception to retirement guards or approval for another Start. Owner review is required for this proposed durable classification. No acceptance is inferred.

Current decision is STOP. Reconciliation is finite and complete on current evidence. No updated binding/launcher/new test is prepared: original disposition remains unresolved. Existing proposed diagnostic capture remains conditional on a separate admissible lifetime solution and receiver/exact-operation qualification. A future decision that actually changes operation eligibility would require its own exact normative review and factual safety proof; it is not hidden in the administrative proposal.

No source/build/test changes were needed. No Windows permission/SCM/process/audit mutation, no repeated original-instance native query, no build/native/network tests. Existing read-only observations are reused with their original timestamps. WIP/preimages/hash audit and documentary consistency are the checks for this slice. Future listener direction unaffected; no accepted ADR changed.

```text
CURRENT_TASK_COMPLETED=YES
RECONCILIATION_DECISION=STOP_INSUFFICIENT_EXISTING_EVIDENCE
PREVIOUS_ATTEMPT_LIFETIME=UNKNOWN_UNRECEIPTED_RETAINED_ATTEMPT
ORIGINAL_INSTANCE_TERMINATION=NOT_PROVED
ORIGINAL_RESOURCE_RELEASE=NOT_PROVED
CANONICAL_OWNER_RECONCILIATION=DOCUMENTARY_ONLY_NO_RUNTIME_RETIREMENT
ORPHAN_DISPOSITION=PROPOSED_ADMINISTRATIVE_ONLY_NOT_ACCEPTED
NEW_START_AUTHORIZED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_EXACT_DOCUMENTARY_CHANGES_ONLY
FIRST_UNRESOLVED_DIVERGENCE=EXACT_INSTANCE_TERMINATION_AND_OWNED_REFERENCE_DISPOSITION_UNPROVED
NEXT=INSTRUMENTED_NATIVE_LAB_START_AFTER_LIFETIME_RESOLUTION
```
