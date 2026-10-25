---
status: accepted
revision: 1
decision-date: 2026-10-23
scope-level: project
scope-target: administrative disposition of LAB attempt 563826bacc49449f954b20cc48ce9b36
supersedes: []
---
# Decision — Orphan LAB qualifier administrative disposition

## Authority and exact acceptance

Owner TASK=PACC-PHASE6-ORPHAN-LAB-QUALIFIER-DISPOSITION-ACCEPTANCE-1 conditionally authorizes acceptance after exact conformity review and official executor approval. This record takes effect only after that exact reviewed write and hash verification. This is normative acceptance of administrative O0–O5, not implementation, retirement, resource release or Start authority.

Original [proposal](ADR-CANDIDATE-previous-lab-qualifier-orphan-disposition-20261023.md) remains byte-identical with historical PROPOSED status, SHA256 98ae6e5b68fa2e613e0cc89533bcab3ddd237ce9bbf61eb5f918aa5d2b0e74aa. The O0–O5 body below is incorporated byte-exact, SHA256 aa58ae0d95863ec03e3b1385abc20a3f8dfda55a84fe50d20010aa8b1751b1a5. No prior accepted ADR is amended. Review/evidence: [closeout](../integration/orphan-lab-qualifier-disposition-acceptance-20261023/closeout.md) and [resource-isolation assessment](../integration/orphan-lab-qualifier-disposition-acceptance-20261023/resource-isolation-review.md).

## Scope and relationship to future experiments

Acceptance resolves the proposal's historical conditional wording only within its administrative scope. Attempt563826bacc49449f954b20cc48ce9b36 is classified ORPHANED_UNRESOLVED/NON_CLEAN as an evidence disposition; its termination, original cleanup, release and quiescence remain UNKNOWN/unproved. No canonical in-memory owner state or retirement guard is changed.

O0/O3/O5 allocate or authorize no new attempt and no operating effect. They do not impose a perpetual global freeze on all independent LAB work. Retention/non-reuse obligations apply to the affected references/resources and their dependencies; any shared capacity remains a real admission dependency. A future genuinely independent experiment can be reviewed on its own proven isolation, ownership, Stop/lifetime and capacity, preserving every old UNKNOWN fact. It cannot use a new name/GUID as evidence of isolation or bypass affected retirement guards. This reading creates no general exception, does not certify a new experiment and does not authorize a second service.

For the current same-service/same-target launcher, isolation is NOT demonstrated and ED11 coexistence is NOT qualified: STOP. Prior termination is one potential source of admissible evidence, not a substitute for resource-level proof and not a universal prerequisite imposed on every unrelated future experiment by this decision. If a future mechanism changes a fixed LAB service/profile/target or operation eligibility, O5 requires separate exact normative/functional review and the factual proof demanded by existing contracts. Approval of a document cannot replace that proof.

Model E/ED08/Stop ownership and retained-reference constraints, virtual identity V5–V8, process/shared-host ED11 charges and quotas remain intact. The earlier CA non-overwrite decision is restricted to its named KSP experiment; no authority or isolation qualification is transferred to SCM. No generic orphan recovery/alternate cleanup mechanism is introduced.

## Accepted O0–O5 (exact original body)

O0. Restrict the administrative disposition to historical attempt563826bacc49449f954b20cc48ce9b36 and its hashed intent/log/plan. No new invocation, authorization or operational identity is allocated.

O1. Classify its evidence record ORPHANED_UNRESOLVED / NON_CLEAN, preserving original5/secondary5023 and all UNKNOWN lifetime/resource facts. This classification means evidence unavailable for final reconciliation; it does not assert termination, successful shutdown, closed handles, destruction or actual resource release.

O2. Keep original authorization consumed and non-replayable. Preserve intent/log/hash history and the existing canonical owners. Do not create another authority, journal, worker, runtime or administrative registry. Do not synthesize native/outer final receipts or mark cycle_quiescent/cycle_closed by record edit.

O3. Preserve persistent service/candidate/configuration/dependencies and their independent owners; no delete, overwrite, restore, ACL change, Start/Stop/rebind, process kill or release is granted. Distinguish process refs, persistent objects and administrative history. Four historical LABs and KSP states remain untouched.

O4. Retain uncertain resource accounting and non-reuse constraints without fictional zero/refund or quota increase. Documentary disposition does not reconcile an unavailable in-memory owner or satisfy its retirement guard. ED11 remains PARTIAL; CURRENT/signature/recovery remain DENY.

O5. This acceptance, if later issued, would close only the administrative evidence classification. It does NOT resolve the missing factual lifetime guarantee or authorize a new Start/diagnostic binding. Any future eligibility-changing mechanism requires separate explicit normative approval consistent with accepted guards, factual proof and exact manual-operation review. No implicit exception is created.

## Current limits

PREVIOUS_ATTEMPT=UNKNOWN_PRESERVED
RESOURCE_ISOLATION=NOT_QUALIFIED_FOR_CURRENT_SHARED_SERVICE
NEW_ATTEMPT_ELIGIBILITY=CONDITIONAL_REVIEW_ONLY_CURRENT_EXECUTION_STOP
NEW_START_AUTHORIZED=NO
IMPLEMENTATION_AUTHORIZED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PRODUCTION_PIN=0.6.4

No code, service/account/file/ACL/KSP effect, process release, quota refund, bootstrap, key/certificate, CURRENT, network, commit/push/tag/release follows. Original evidence and historical LABs are preserved. No operational PASS is asserted.
