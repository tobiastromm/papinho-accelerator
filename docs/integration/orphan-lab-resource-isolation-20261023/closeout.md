# Orphan LAB resource isolation and ED11 closure — 2026-10-23

TASK=PACC-PHASE6-ORPHAN-LAB-RESOURCE-ISOLATION-1. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Focused source/artifact/resource qualification concluded STOP. O0–O5 ACCEPTED remains unchanged; no new ADR or normative exception is needed for this conclusion.

## Concrete result

The existing prepared path is not an independent attempt: it uses the same mutable SCM object, fixed account, parent/configuration and candidate leaf, and the consumed intent. Replacing an attempt GUID does not change that graph. Clearing intent, marking cycle_quiescent or swapping a hash would bypass guards and is not a correction.

A registered stopped service is a persistent configuration object, not evidence of an active process. The logged retained qualifier source path goes to Sleep(INFINITE) then returns without scheduling another SCM operation; the console Ctrl-C/Break handler only records Stop intent. Holding a mutable service handle does not itself demonstrate another active Start/Stop. This narrows the interference hypothesis: do not claim old mutation is occurring. However, this static path analysis is not a termination/cleanup receipt, a cross-owner exclusivity protocol or a proof of whole-host resource availability. It cannot authorize reuse of unknown references or mutable shared service state.

The first control dependency without factual qualification is M_SAME_SERVICE_MUTATION_AND_INSTANCE_EXCLUSIVITY: current candidate rebind/Start targets the old SCM object, while no completed owner/instance handoff or equivalent qualified exclusivity proof exists. PID0/window absence is not that proof. No second service/environment is created or proposed as a convenience workaround.

The first missing numeric variable in the coexistence calculation is R_OLD_QUALIFIER_LAUNCHER_NATIVE_USER_KERNEL_RETAINED_MAX. The original exact-instance receipt is absent; its native/CLR/kernel/stack resource obligation has no qualified upper bound in the reviewed evidence. The98-handle snapshot cannot determine it. New driver/launcher/receiver native peak and service transitive stack/provider/kernel bounds are also unqualified. Thus coexistence maximum cannot be calculated from the present evidence without inventing a bound or treating UNKNOWN as zero. See [focused ledger](resource-ledger.md).

## Alternatives A–D

| Alternative | Concrete path assessed | Result and first blocker |
|---|---|---|
| A — reconcile shared existing resources | same registered service, old native identities, canonical owner, actual original-instance/retirement evidence or equivalent control proof | Existing log/intent/SCM observations provide no handoff/retirement proof. Old parked source reduces active-control risk but does not create an exclusive new operation. STOP; do not repeat inconclusive PID/SCM/security-log probes |
| B — independently owned graph | new administrative references/lifetime must not adopt old refs; known immutable files could be shared only with proved compatibility | Current host/driver fixes service/account, parent/config IDs, candidate leaf and intent. No implemented disjoint target/control boundary. A new record/GUID does not isolate them. No new service/environment/identity is allocated; no replacement of UNKNOWN objects |
| C — bounded coexistence at current quota | retain old charges; add qualified new peak, compatible shared attribution and factual host margin | Static components are bounded, full native/CLR/kernel/stack costs are not. No finite justified R_OLD bound, new peak or available host admission proof. Existing3MiB/2MiB managed ceilings are not host capacity. STOP without quota increase |
| D — preserve STOP | retain old state/effects/history and do not submit another Start | Chosen; no false resource release, permanent global ban or fabricated readiness |

No alternative A–C is qualified for execution with the current resource graph and evidence. O0–O5 already permits discussing future genuinely independent qualification; its normative decision is not reopened. The blocker is specific to these dependencies and unknown bounds, not to the mere existence of any UNKNOWN historical record.

## Minimum technical work, and its limits

No permission change, larger quota, arithmetic reserve, code constant or retrospective report can supply missing lifetime/control/native-cost evidence. No safe source-only patch to this historical attempt is demonstrated, so no functional correction is applied.

For a future exact shared-service operation, the minimum proof work is an existing-owner/exact-instance handoff or separately qualified exclusivity mechanism that preserves all old unknown refs, followed by a retained/native/new-peak admission bound and factual host margin. For an independent operation, a genuinely disjoint or proved-compatible resource graph must be specified and reviewed first. Neither is accomplished by the current instrumented binary alone. No next binding/launcher is armed here.

One concrete prospective driver deficiency was located: cycle_live obtains the service process handle only AFTER the RUNNING wait succeeds. An immediate preparation failure therefore reaches the retention branch without an original child-instance handle, and cycle_quiescent can only be set after the successful Stop/status/process-wait path. A future focal driver improvement can capture and bind an original instance as early as factually available, retain it, and distinguish a proven non-clean failed-start terminal outcome from successful-cycle PASS. Early polling cannot guarantee capturing an instantaneous failure; if no exact-instance proof exists, retention must remain. Such a change would not repair this old attempt, clear its intent, refund its resources, prove ED11 or authorize Start. No weakened guard or alternate cleanup is proposed/applied.

## Checks and governance

Read O0–O5 decision/isolation review, relevant Model E/Stop/quiescence/retirement/ED08/ED11 contracts from the continuous task; inspect original driver/launcher, actual host instrumentation, allocator accounting and existing model evidence. Relevant future listener direction remains unaffected. Accepted decisions, four historical LABs and previous intent/log remain unchanged. The user-supplied resource-isolation-review path with an escaped period was resolved to the existing resource-isolation-review.md; no alternate input invented.

Read-only PE parsing rechecked the three exact pinned binary hashes and image/main-stack fields; no executable was run, rebuilt or replaced. PE size and configured main-stack reserve/commit are not measured process/callback/kernel bounds. The current service/native process was not queried again; prior snapshots retain their timestamps. Source/PE/census validation and8027-file WIP/hash baseline are the checks; builds/native/network tests NOT_RUN because no functional change or native experiment is authorized/needed for this audit.

No Start, SCM/ACL/account/process mutation, release/kill/retry, new service/environment, bootstrap, CA/key/certificate/CURRENT/network, quota change or commit/push/tag/release/worktree/clone. The static qualified components below do not turn ED11 into PASS.

```text
CURRENT_TASK_COMPLETED=YES
QUALIFICATION_DECISION=STOP_SPECIFIC_CONTROL_AND_NATIVE_BOUND_GAPS
ORPHAN_DISPOSITION=ACCEPTED
PREVIOUS_ATTEMPT=UNKNOWN_PRESERVED
SHARED_RESOURCES=SAME_MUTABLE_SCM_OBJECT_FIXED_FILES_CONFIG_AND_HOST_NATIVE_CAPACITY
RESOURCE_ISOLATION=NOT_PROVED_CURRENT_GRAPH
COEXISTENCE_ED11=NOT_CLOSABLE_FIRST_MISSING_OLD_NATIVE_RETAINED_UPPER_BOUND
MINIMUM_TECHNICAL_CORRECTION=NO_SAFE_RETROACTIVE_PATCH_CONTROL_AND_NATIVE_ADMISSION_PROOF_REQUIRED
NEW_START_AUTHORIZED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_EXACT_DOCUMENTARY_CHANGES_ONLY
FIRST_UNRESOLVED_DIVERGENCE=M_SAME_SERVICE_EXCLUSIVITY_AND_R_OLD_NATIVE_RETAINED_MAX_UNPROVED
NEXT=INSTRUMENTED_NATIVE_LAB_START_AFTER_RESOURCE_QUALIFICATION
```
