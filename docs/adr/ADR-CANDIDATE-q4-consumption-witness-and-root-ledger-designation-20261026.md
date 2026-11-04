---
status: proposed
revision: 1
date: 2026-10-26
scope-level: project
supersedes: []
---
# Q4 durable consumption: same-root reuse and independent continuity witness

## L0 - Finding and bounded decision
No reviewed existing mechanism supplies both Q4 documentary use-once scope and rollback-independent latest-consumption proof. The accepted cross-boot prerequisite remains unsatisfied. This proposal does not designate a nonexistent ledger or grant execution. It preserves the exact historic10584f02e11f92c92b43cddf0d9d316f operation, original guards, unknown historical native IDs and R7 proof requirements.

## L1 - Recommended minimal structural reuse
If approved, reuse the existing single administrative root owner, exact-native-preimage CAS/worker and retained-history lifecycle as the LOCAL consumption substrate. Never encode Q4 consumption as an issuer serial/certificate event, synthesize CURRENT/epoch, overwrite the consumed intent or repurpose reserved codec bytes. A narrowly typed versioned documentary component/transition needs its own exact schema and root participant review, immutable approval/history binding and full ED11 accounting before implementation. Reuse of primitives is not current authority to publish such a component. No second journal owner, service or generic approval registry is proposed.

## L2 - Independent witness is indispensable
To preserve protection against coherent rollback of ALL local storage, require a factual independently retained latest consumption witness OUTSIDE that storage rollback domain, with authenticated authority/identity, atomic compare-and-consume or equivalent exclusive one-use sequencing, durable query/reconciliation and anti-fork guarantees. The local root hash chain/checkpoint alone cannot supply it. Existing human approval authority may designate an already governed custody mechanism, but chat/git/file existence alone is not that mechanism or a technical witness. No remote service, TPM, hardware counter, storage or signing authority is presumed installed or authorized.

Witness identity, ownership, persistence boundary, failure/power-loss semantics, rollback independence, authentication, serialization and resource costs must be reviewed concretely. If no existing witness qualifies, a separate explicit architectural decision is required to introduce one; this proposal does not silently create a new authority. Until then STOP. This is a necessary dependency, not weakened rollback protection.

## L3 - State protocol to qualify after designation
Bind one newly approved request, historic identity, current factual host/boot/operator/token, original target objects, script/dependencies and local root identity/predecessor to the independent witness. Consume there BEFORE protected read; ambiguous consumption permanently denies this approval pending query-only reconciliation, never retry. Local record preparation/commit and witness ordering require an exact reviewed protocol: witness-confirmed/local-missing is spent/INCOMPLETE, local-consumed/witness-missing is inconsistent/DENY, unequal roots/approvals or fork is INVALID. Both exact records and required current-context/object proofs are necessary but not sufficient: only the original consuming invocation, bound to its exact live reader PID/creation and nontransferable retained lease, may continue once. A restarted/querying invocation seeing CONSUMED is denied even if every byte matches; reconciliation never reissues execution authority. Final result preserves first failure/close/UNKNOWN; no state rolls back from consumed to unused.

## L4 - Loss, restore and substitution
A restored older local root must be compared to the independently newer witness and denied. Loss/unavailability/rollback uncertainty of the witness is DENY, never fallback to local CREATE_NEW. Known bad or missing local state cannot bootstrap as virgin. Original-handle native identity, RAW1/protected ancestors, no reparse and exact content remain mandatory; historical absent creation IDs cannot be recovered from hashes. An identity discontinuity cannot be fixed by selecting a matching old witness.

## L5 - Alternatives and recommendation
A: preserve full rollback requirement, retain existing root as candidate substrate and designate/qualify an independent existing witness; if absent, decide explicitly on one narrow external persistence/custody boundary. Recommended because the guarantee cannot be proved locally after complete local rollback. No new permanent service is required by this abstract choice, but a concrete suitable witness has NOT been identified.
B: retain current contract and do not execute cross-boot readback. Safe and immediately available; E21 remains pending.
C: accept a normal-local-storage/no-external-rollback assumption resembling personal M3. This would REDUCE the requested guarantee, requires a separate explicit exception and is NOT recommended or accepted here. M3 does not currently specialize Q4. Do not present C as preservation of full rollback protection.

## L6 - Qualification before integration
Require actual canonical component/worker source tests, native IDs/RAW1 and swapped-parent/claim tests, witness wrong identity/replay/concurrency, consumed-result loss, crash at each cross-store boundary, interrupted persistence, restoring local only and both copies, witness loss/unavailability, Stop and uncertain close retaining original references. A witness restored with all local state is not independent. Simulated data tests establish only comparisons, not a deployed witness or power-loss persistence. No command until normative designation, implementation qualification, current bindings and a separate exact operational approval.

## L7 - Limits
No current root/epoch/CURRENT publication, operational consumption, Q4 read, LAB, Start/Stop, SCM/ACL mutation, cleanup/refund, signature/recovery or quota change. R7/E21 remain pending; UNKNOWN/reservations preserved; ED11/Phase6 PARTIAL and productive authorities DENY. Original cross-boot decision remains accepted and unmodified. This document remains PROPOSED until exact owner decision and does not itself authorize implementation of new persistence authority.
